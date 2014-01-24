/*
<<<<<<< HEAD
    comedi/drivers/pcmmio.c
    Driver for Winsystems PC-104 based multifunction IO board.

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 2007 Calin A. Culianu <calin@ajvar.org>

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
Driver: pcmmio
Description: A driver for the PCM-MIO multifunction board
Devices: [Winsystems] PCM-MIO (pcmmio)
Author: Calin Culianu <calin@ajvar.org>
Updated: Wed, May 16 2007 16:21:10 -0500
Status: works

A driver for the relatively new PCM-MIO multifunction board from
Winsystems.  This board is a PC-104 based I/O board.  It contains
four subdevices:
  subdevice 0 - 16 channels of 16-bit AI
  subdevice 1 - 8 channels of 16-bit AO
  subdevice 2 - first 24 channels of the 48 channel of DIO
	(with edge-triggered interrupt support)
  subdevice 3 - last 24 channels of the 48 channel DIO
	(no interrupt support for this bank of channels)

  Some notes:

  Synchronous reads and writes are the only things implemented for AI and AO,
  even though the hardware itself can do streaming acquisition, etc.  Anyone
  want to add asynchronous I/O for AI/AO as a feature?  Be my guest...

  Asynchronous I/O for the DIO subdevices *is* implemented, however!  They are
  basically edge-triggered interrupts for any configuration of the first
  24 DIO-lines.

  Also note that this interrupt support is untested.

  A few words about edge-detection IRQ support (commands on DIO):

  * To use edge-detection IRQ support for the DIO subdevice, pass the IRQ
    of the board to the comedi_config command.  The board IRQ is not jumpered
    but rather configured through software, so any IRQ from 1-15 is OK.

  * Due to the genericity of the comedi API, you need to create a special
    comedi_command in order to use edge-triggered interrupts for DIO.

  * Use comedi_commands with TRIG_NOW.  Your callback will be called each
    time an edge is detected on the specified DIO line(s), and the data
    values will be two sample_t's, which should be concatenated to form
    one 32-bit unsigned int.  This value is the mask of channels that had
    edges detected from your channel list.  Note that the bits positions
    in the mask correspond to positions in your chanlist when you
    specified the command and *not* channel id's!

 *  To set the polarity of the edge-detection interrupts pass a nonzero value
    for either CR_RANGE or CR_AREF for edge-up polarity, or a zero
    value for both CR_RANGE and CR_AREF if you want edge-down polarity.

Configuration Options:
  [0] - I/O port base address
  [1] - IRQ (optional -- for edge-detect interrupt support only,
	leave out if you don't need this feature)
*/

#include <linux/interrupt.h>
#include <linux/slab.h>
#include "../comedidev.h"
#include "pcm_common.h"
#include <linux/pci.h>		/* for PCI devices */

/* This stuff is all from pcmuio.c -- it refers to the DIO subdevices only */
#define CHANS_PER_PORT   8
#define PORTS_PER_ASIC   6
#define INTR_PORTS_PER_ASIC   3
#define MAX_CHANS_PER_SUBDEV 24	/* number of channels per comedi subdevice */
#define PORTS_PER_SUBDEV (MAX_CHANS_PER_SUBDEV/CHANS_PER_PORT)
#define CHANS_PER_ASIC (CHANS_PER_PORT*PORTS_PER_ASIC)
#define INTR_CHANS_PER_ASIC 24
#define INTR_PORTS_PER_SUBDEV (INTR_CHANS_PER_ASIC/CHANS_PER_PORT)
#define MAX_DIO_CHANS   (PORTS_PER_ASIC*1*CHANS_PER_PORT)
#define MAX_ASICS       (MAX_DIO_CHANS/CHANS_PER_ASIC)
#define SDEV_NO ((int)(s - dev->subdevices))
#define CALC_N_DIO_SUBDEVS(nchans) ((nchans)/MAX_CHANS_PER_SUBDEV + (!!((nchans)%MAX_CHANS_PER_SUBDEV)) /*+ (nchans > INTR_CHANS_PER_ASIC ? 2 : 1)*/)
/* IO Memory sizes */
#define ASIC_IOSIZE (0x0B)
#define PCMMIO48_IOSIZE ASIC_IOSIZE

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
#define REG_PAGELOCK 0x7	/*
				 * page selector register, upper 2 bits select
				 * a page and bits 0-5 are used to 'lock down'
				 * a particular port above to make it readonly.
				 */
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
#define REG_LOCK_MASK (~(REG_PAGE_MASK))
#define PAGE_POL 1
#define PAGE_ENAB 2
#define PAGE_INT_ID 3

static int ai_rinsn(struct comedi_device *, struct comedi_subdevice *,
		    struct comedi_insn *, unsigned int *);
static int ao_rinsn(struct comedi_device *, struct comedi_subdevice *,
		    struct comedi_insn *, unsigned int *);
static int ao_winsn(struct comedi_device *, struct comedi_subdevice *,
		    struct comedi_insn *, unsigned int *);

/*
 * Board descriptions for two imaginary boards.  Describing the
 * boards in this way is optional, and completely driver-dependent.
 * Some drivers use arrays such as this, other do not.
 */
struct pcmmio_board {
	const char *name;
	const int dio_num_asics;
	const int dio_num_ports;
	const int total_iosize;
	const int ai_bits;
	const int ao_bits;
	const int n_ai_chans;
	const int n_ao_chans;
	const struct comedi_lrange *ai_range_table, *ao_range_table;
	int (*ai_rinsn) (struct comedi_device *dev,
			struct comedi_subdevice *s,
			struct comedi_insn *insn,
			unsigned int *data);
	int (*ao_rinsn) (struct comedi_device *dev,
			struct comedi_subdevice *s,
			struct comedi_insn *insn,
			unsigned int *data);
	int (*ao_winsn) (struct comedi_device *dev,
			struct comedi_subdevice *s,
			struct comedi_insn *insn,
			unsigned int *data);
};

static const struct comedi_lrange ranges_ai = {
	4, {RANGE(-5., 5.), RANGE(-10., 10.), RANGE(0., 5.), RANGE(0., 10.)}
};

static const struct comedi_lrange ranges_ao = {
	6, {RANGE(0., 5.), RANGE(0., 10.), RANGE(-5., 5.), RANGE(-10., 10.),
	  RANGE(-2.5, 2.5), RANGE(-2.5, 7.5)}
};

static const struct pcmmio_board pcmmio_boards[] = {
	{
	 .name = "pcmmio",
	 .dio_num_asics = 1,
	 .dio_num_ports = 6,
	 .total_iosize = 32,
	 .ai_bits = 16,
	 .ao_bits = 16,
	 .n_ai_chans = 16,
	 .n_ao_chans = 8,
	 .ai_range_table = &ranges_ai,
	 .ao_range_table = &ranges_ao,
	 .ai_rinsn = ai_rinsn,
	 .ao_rinsn = ao_rinsn,
	 .ao_winsn = ao_winsn},
};

/*
 * Useful for shorthand access to the particular board structure
 */
#define thisboard ((const struct pcmmio_board *)dev->board_ptr)

/* this structure is for data unique to this subdevice.  */
struct pcmmio_subdev_private {

	union {
		/* for DIO: mapping of halfwords (bytes)
		   in port/chanarray to iobase */
		unsigned long iobases[PORTS_PER_SUBDEV];

		/* for AI/AO */
		unsigned long iobase;
	};
	union {
		struct {

			/* The below is only used for intr subdevices */
			struct {
				/*
				 * if non-negative, this subdev has an
				 * interrupt asic
				 */
				int asic;
				/*
				 * if nonnegative, the first channel id for
				 * interrupts.
				 */
				int first_chan;
				/*
				 * the number of asic channels in this subdev
				 * that have interrutps
				 */
				int num_asic_chans;
				/*
				 * if nonnegative, the first channel id with
				 * respect to the asic that has interrupts
				 */
				int asic_chan;
				/*
				 * subdev-relative channel mask for channels
				 * we are interested in
				 */
				int enabled_mask;
				int active;
				int stop_count;
				int continuous;
				spinlock_t spinlock;
			} intr;
		} dio;
		struct {
			/* the last unsigned int data written */
			unsigned int shadow_samples[8];
		} ao;
	};
};

/*
 * this structure is for data unique to this hardware driver.  If
 * several hardware drivers keep similar information in this structure,
 * feel free to suggest moving the variable to the struct comedi_device struct.
 */
struct pcmmio_private {
	/* stuff for DIO */
	struct {
		unsigned char pagelock;	/* current page and lock */
		/* shadow of POLx registers */
		unsigned char pol[NUM_PAGED_REGS];
		/* shadow of ENABx registers */
		unsigned char enab[NUM_PAGED_REGS];
		int num;
		unsigned long iobase;
		unsigned int irq;
		spinlock_t spinlock;
	} asics[MAX_ASICS];
	struct pcmmio_subdev_private *sprivs;
};

/*
 * most drivers define the following macro to make it easy to
 * access the private structure.
 */
#define devpriv ((struct pcmmio_private *)dev->private)
#define subpriv ((struct pcmmio_subdev_private *)s->private)
/*
 * The struct comedi_driver structure tells the Comedi core module
 * which functions to call to configure/deconfigure (attach/detach)
 * the board, and also about the kernel module that contains
 * the device code.
 */
static int pcmmio_attach(struct comedi_device *dev,
			 struct comedi_devconfig *it);
static int pcmmio_detach(struct comedi_device *dev);

static struct comedi_driver driver = {
	.driver_name = "pcmmio",
	.module = THIS_MODULE,
	.attach = pcmmio_attach,
	.detach = pcmmio_detach,
/* It is not necessary to implement the following members if you are
 * writing a driver for a ISA PnP or PCI card */
	/* Most drivers will support multiple types of boards by
	 * having an array of board structures.  These were defined
	 * in pcmmio_boards[] above.  Note that the element 'name'
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
	.board_name = &pcmmio_boards[0].name,
	.offset = sizeof(struct pcmmio_board),
	.num_names = ARRAY_SIZE(pcmmio_boards),
};

static int pcmmio_dio_insn_bits(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn, unsigned int *data);
static int pcmmio_dio_insn_config(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  struct comedi_insn *insn, unsigned int *data);

static irqreturn_t interrupt_pcmmio(int irq, void *d);
static void pcmmio_stop_intr(struct comedi_device *, struct comedi_subdevice *);
static int pcmmio_cancel(struct comedi_device *dev, struct comedi_subdevice *s);
static int pcmmio_cmd(struct comedi_device *dev, struct comedi_subdevice *s);
static int pcmmio_cmdtest(struct comedi_device *dev, struct comedi_subdevice *s,
			  struct comedi_cmd *cmd);

/* some helper functions to deal with specifics of this device's registers */
/* sets up/clears ASIC chips to defaults */
static void init_asics(struct comedi_device *dev);
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
static int pcmmio_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	struct comedi_subdevice *s;
	int sdev_no, chans_left, n_dio_subdevs, n_subdevs, port, asic,
	    thisasic_chanct = 0;
	unsigned long iobase;
	unsigned int irq[MAX_ASICS];

	iobase = it->options[0];
	irq[0] = it->options[1];

<<<<<<< HEAD
	printk("comedi%d: %s: io: %lx ", dev->minor, driver.driver_name,
	       iobase);
=======
	printk(KERN_INFO "comedi%d: %s: io: %lx attaching...\n", dev->minor,
			driver.driver_name, iobase);
>>>>>>> refs/remotes/origin/cm-10.0

	dev->iobase = iobase;

	if (!iobase || !request_region(iobase,
				       thisboard->total_iosize,
				       driver.driver_name)) {
<<<<<<< HEAD
		printk("I/O port conflict\n");
=======
		printk(KERN_ERR "comedi%d: I/O port conflict\n", dev->minor);
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
	if (alloc_private(dev, sizeof(struct pcmmio_private)) < 0) {
<<<<<<< HEAD
		printk("cannot allocate private data structure\n");
=======
		printk(KERN_ERR "comedi%d: cannot allocate private data structure\n",
				dev->minor);
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENOMEM;
	}

	for (asic = 0; asic < MAX_ASICS; ++asic) {
		devpriv->asics[asic].num = asic;
		devpriv->asics[asic].iobase =
		    dev->iobase + 16 + asic * ASIC_IOSIZE;
		/*
		 * this gets actually set at the end of this function when we
		 * request_irqs
		 */
		devpriv->asics[asic].irq = 0;
		spin_lock_init(&devpriv->asics[asic].spinlock);
	}

	chans_left = CHANS_PER_ASIC * thisboard->dio_num_asics;
	n_dio_subdevs = CALC_N_DIO_SUBDEVS(chans_left);
	n_subdevs = n_dio_subdevs + 2;
	devpriv->sprivs =
	    kcalloc(n_subdevs, sizeof(struct pcmmio_subdev_private),
		    GFP_KERNEL);
	if (!devpriv->sprivs) {
<<<<<<< HEAD
		printk("cannot allocate subdevice private data structures\n");
=======
		printk(KERN_ERR "comedi%d: cannot allocate subdevice private data structures\n",
				dev->minor);
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENOMEM;
	}
	/*
	 * Allocate the subdevice structures.  alloc_subdevice() is a
	 * convenient macro defined in comedidev.h.
	 *
	 * Allocate 1 AI + 1 AO + 2 DIO subdevs (24 lines per DIO)
	 */
	if (alloc_subdevices(dev, n_subdevs) < 0) {
<<<<<<< HEAD
		printk("cannot allocate subdevice data structures\n");
=======
		printk(KERN_ERR "comedi%d: cannot allocate subdevice data structures\n",
				dev->minor);
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENOMEM;
	}

	/* First, AI */
	sdev_no = 0;
	s = dev->subdevices + sdev_no;
	s->private = devpriv->sprivs + sdev_no;
	s->maxdata = (1 << thisboard->ai_bits) - 1;
	s->range_table = thisboard->ai_range_table;
	s->subdev_flags = SDF_READABLE | SDF_GROUND | SDF_DIFF;
	s->type = COMEDI_SUBD_AI;
	s->n_chan = thisboard->n_ai_chans;
	s->len_chanlist = s->n_chan;
	s->insn_read = thisboard->ai_rinsn;
	subpriv->iobase = dev->iobase + 0;
	/* initialize the resource enable register by clearing it */
	outb(0, subpriv->iobase + 3);
	outb(0, subpriv->iobase + 4 + 3);

	/* Next, AO */
	++sdev_no;
	s = dev->subdevices + sdev_no;
	s->private = devpriv->sprivs + sdev_no;
	s->maxdata = (1 << thisboard->ao_bits) - 1;
	s->range_table = thisboard->ao_range_table;
	s->subdev_flags = SDF_READABLE;
	s->type = COMEDI_SUBD_AO;
	s->n_chan = thisboard->n_ao_chans;
	s->len_chanlist = s->n_chan;
	s->insn_read = thisboard->ao_rinsn;
	s->insn_write = thisboard->ao_winsn;
	subpriv->iobase = dev->iobase + 8;
	/* initialize the resource enable register by clearing it */
	outb(0, subpriv->iobase + 3);
	outb(0, subpriv->iobase + 4 + 3);

	++sdev_no;
	port = 0;
	asic = 0;
	for (; sdev_no < (int)dev->n_subdevices; ++sdev_no) {
		int byte_no;

		s = dev->subdevices + sdev_no;
		s->private = devpriv->sprivs + sdev_no;
		s->maxdata = 1;
		s->range_table = &range_digital;
		s->subdev_flags = SDF_READABLE | SDF_WRITABLE;
		s->type = COMEDI_SUBD_DIO;
		s->insn_bits = pcmmio_dio_insn_bits;
		s->insn_config = pcmmio_dio_insn_config;
		s->n_chan = min(chans_left, MAX_CHANS_PER_SUBDEV);
		subpriv->dio.intr.asic = -1;
		subpriv->dio.intr.first_chan = -1;
		subpriv->dio.intr.asic_chan = -1;
		subpriv->dio.intr.num_asic_chans = -1;
		subpriv->dio.intr.active = 0;
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
			    && subpriv->dio.intr.asic < 0) {
				/*
				 * this is an interrupt subdevice,
				 * so setup the struct
				 */
				subpriv->dio.intr.asic = asic;
				subpriv->dio.intr.active = 0;
				subpriv->dio.intr.stop_count = 0;
				subpriv->dio.intr.first_chan = byte_no * 8;
				subpriv->dio.intr.asic_chan = thisasic_chanct;
				subpriv->dio.intr.num_asic_chans =
				    s->n_chan - subpriv->dio.intr.first_chan;
				s->cancel = pcmmio_cancel;
				s->do_cmd = pcmmio_cmd;
				s->do_cmdtest = pcmmio_cmdtest;
				s->len_chanlist =
				    subpriv->dio.intr.num_asic_chans;
			}
			thisasic_chanct += CHANS_PER_PORT;
		}
		spin_lock_init(&subpriv->dio.intr.spinlock);

		chans_left -= s->n_chan;

		if (!chans_left) {
			/*
			 * reset the asic to our first asic,
			 * to do intr subdevs
			 */
			asic = 0;
			port = 0;
		}

	}

	init_asics(dev);	/* clear out all the registers, basically */

	for (asic = 0; irq[0] && asic < MAX_ASICS; ++asic) {
		if (irq[asic]
		    && request_irq(irq[asic], interrupt_pcmmio,
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

	dev->irq = irq[0];	/*
				 * grr.. wish comedi dev struct supported
				 * multiple irqs..
				 */

	if (irq[0]) {
<<<<<<< HEAD
		printk("irq: %u ", irq[0]);
		if (thisboard->dio_num_asics == 2 && irq[1])
			printk("second ASIC irq: %u ", irq[1]);
	} else {
		printk("(IRQ mode disabled) ");
	}

	printk("attached\n");
=======
		printk(KERN_DEBUG "comedi%d: irq: %u\n", dev->minor, irq[0]);
		if (thisboard->dio_num_asics == 2 && irq[1])
			printk(KERN_DEBUG "comedi%d: second ASIC irq: %u\n",
					dev->minor, irq[1]);
	} else {
		printk(KERN_INFO "comedi%d: (IRQ mode disabled)\n", dev->minor);
	}

	printk(KERN_INFO "comedi%d: attached\n", dev->minor);
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
static int pcmmio_detach(struct comedi_device *dev)
{
	int i;

<<<<<<< HEAD
	printk("comedi%d: %s: remove\n", dev->minor, driver.driver_name);
=======
	printk(KERN_INFO "comedi%d: %s: remove\n", dev->minor, driver.driver_name);
>>>>>>> refs/remotes/origin/cm-10.0
	if (dev->iobase)
		release_region(dev->iobase, thisboard->total_iosize);

	for (i = 0; i < MAX_ASICS; ++i) {
		if (devpriv && devpriv->asics[i].irq)
			free_irq(devpriv->asics[i].irq, dev);
	}

	if (devpriv && devpriv->sprivs)
		kfree(devpriv->sprivs);

	return 0;
}

/* DIO devices are slightly special.  Although it is possible to
 * implement the insn_read/insn_write interface, it is much more
 * useful to applications if you implement the insn_bits interface.
 * This allows packed reading/writing of the DIO channels.  The
 * comedi core can convert between insn_bits and insn_read/write */
static int pcmmio_dio_insn_bits(struct comedi_device *dev,
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
	printk(KERN_DEBUG "write mask: %08x  data: %08x\n", data[0], data[1]);
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
<<<<<<< HEAD
		    ("byte %d wmb %02x db %02x offset %02d io %04x, data_in %02x ",
		     byte_no, (unsigned)write_mask_byte, (unsigned)data_byte,
		     offset, ioaddr, (unsigned)byte);
=======
		    (KERN_DEBUG "byte %d wmb %02x db %02x offset %02d io %04x,"
		     " data_in %02x ", byte_no, (unsigned)write_mask_byte,
		     (unsigned)data_byte, offset, ioaddr, (unsigned)byte);
>>>>>>> refs/remotes/origin/cm-10.0
#endif

		if (write_mask_byte) {
			/*
			 * this byte has some write_bits
			 * -- so set the output lines
			 */
			/* clear bits for write mask */
			byte &= ~write_mask_byte;
			/* set to inverted data_byte */
			byte |= ~data_byte & write_mask_byte;
			/* Write out the new digital output state */
			outb(byte, ioaddr);
		}
#ifdef DAMMIT_ITS_BROKEN
		/* DEBUG */
		printk("data_out_byte %02x\n", (unsigned)byte);
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
	printk(KERN_DEBUG "s->state %08x data_out %08x\n", s->state, data[1]);
>>>>>>> refs/remotes/origin/cm-10.0
#endif

	return 2;
}

/* The input or output configuration of each digital line is
 * configured by a special insn_config instruction.  chanspec
 * contains the channel to be changed, and data[0] contains the
 * value COMEDI_INPUT or COMEDI_OUTPUT. */
static int pcmmio_dio_insn_config(struct comedi_device *dev,
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

		/*
		 * write out byte -- this is the only time we actually affect
		 * the hardware as all channels are implicitly output
		 * -- but input channels are set to float-high
		 */
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

	for (asic = 0; asic < thisboard->dio_num_asics; ++asic) {
		int port, page;
		unsigned long baseaddr = devpriv->asics[asic].iobase;

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

		/* switch back to default page 0 */
		switch_page(dev, asic, 0);
	}
}

static void switch_page(struct comedi_device *dev, int asic, int page)
{
	if (asic < 0 || asic >= thisboard->dio_num_asics)
		return;		/* paranoia */
	if (page < 0 || page >= NUM_PAGES)
		return;		/* more paranoia */

	devpriv->asics[asic].pagelock &= ~REG_PAGE_MASK;
	devpriv->asics[asic].pagelock |= page << REG_PAGE_BITOFFSET;

	/* now write out the shadow register */
	outb(devpriv->asics[asic].pagelock,
	     devpriv->asics[asic].iobase + REG_PAGELOCK);
}

#ifdef notused
static void lock_port(struct comedi_device *dev, int asic, int port)
{
	if (asic < 0 || asic >= thisboard->dio_num_asics)
		return;		/* paranoia */
	if (port < 0 || port >= PORTS_PER_ASIC)
		return;		/* more paranoia */

	devpriv->asics[asic].pagelock |= 0x1 << port;
	/* now write out the shadow register */
	outb(devpriv->asics[asic].pagelock,
	     devpriv->asics[asic].iobase + REG_PAGELOCK);
	return;
}

static void unlock_port(struct comedi_device *dev, int asic, int port)
{
	if (asic < 0 || asic >= thisboard->dio_num_asics)
		return;		/* paranoia */
	if (port < 0 || port >= PORTS_PER_ASIC)
		return;		/* more paranoia */
	devpriv->asics[asic].pagelock &= ~(0x1 << port) | REG_LOCK_MASK;
	/* now write out the shadow register */
	outb(devpriv->asics[asic].pagelock,
	     devpriv->asics[asic].iobase + REG_PAGELOCK);
}
#endif /* notused */

static irqreturn_t interrupt_pcmmio(int irq, void *d)
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
							/*
							 * clear pending
							 * interrupt
							 */
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
				/*
				 * TODO here: dispatch io lines to subdevs
				 * with commands..
				 */
				printk
<<<<<<< HEAD
				    ("PCMMIO DEBUG: got edge detect interrupt %d asic %d which_chans: %06x\n",
=======
				    (KERN_DEBUG "got edge detect interrupt %d asic %d which_chans: %06x\n",
>>>>>>> refs/remotes/origin/cm-10.0
				     irq, asic, triggered);
				for (s = dev->subdevices + 2;
				     s < dev->subdevices + dev->n_subdevices;
				     ++s) {
					/*
					 * this is an interrupt subdev,
					 * and it matches this asic!
					 */
					if (subpriv->dio.intr.asic == asic) {
						unsigned long flags;
						unsigned oldevents;

						spin_lock_irqsave(&subpriv->dio.
								  intr.spinlock,
								  flags);

						oldevents = s->async->events;

						if (subpriv->dio.intr.active) {
							unsigned mytrig =
							    ((triggered >>
							      subpriv->dio.intr.asic_chan)
							     &
							     ((0x1 << subpriv->
							       dio.intr.
							       num_asic_chans) -
							      1)) << subpriv->
							    dio.intr.first_chan;
							if (mytrig &
							    subpriv->dio.
							    intr.enabled_mask) {
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
									if (mytrig & (1U << ch))
										val |= (1U << n);
								}
								/* Write the scan to the buffer. */
								if (comedi_buf_put(s->async, ((short *)&val)[0])
								    &&
								    comedi_buf_put
								    (s->async,
								     ((short *)
								      &val)[1])) {
									s->async->events |= (COMEDI_CB_BLOCK | COMEDI_CB_EOS);
								} else {
									/* Overflow! Stop acquisition!! */
									/* TODO: STOP_ACQUISITION_CALL_HERE!! */
									pcmmio_stop_intr
									    (dev,
									     s);
								}

								/* Check for end of acquisition. */
								if (!subpriv->dio.intr.continuous) {
									/* stop_src == TRIG_COUNT */
									if (subpriv->dio.intr.stop_count > 0) {
										subpriv->dio.intr.stop_count--;
										if (subpriv->dio.intr.stop_count == 0) {
											s->async->events |= COMEDI_CB_EOA;
											/* TODO: STOP_ACQUISITION_CALL_HERE!! */
											pcmmio_stop_intr
											    (dev,
											     s);
										}
									}
								}
							}
						}

						spin_unlock_irqrestore
						    (&subpriv->dio.intr.
						     spinlock, flags);

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

static void pcmmio_stop_intr(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	int nports, firstport, asic, port;

	asic = subpriv->dio.intr.asic;
	if (asic < 0)
		return;		/* not an interrupt subdev */

	subpriv->dio.intr.enabled_mask = 0;
	subpriv->dio.intr.active = 0;
	s->async->inttrig = 0;
	nports = subpriv->dio.intr.num_asic_chans / CHANS_PER_PORT;
	firstport = subpriv->dio.intr.asic_chan / CHANS_PER_PORT;
	switch_page(dev, asic, PAGE_ENAB);
	for (port = firstport; port < firstport + nports; ++port) {
		/* disable all intrs for this subdev.. */
		outb(0, devpriv->asics[asic].iobase + REG_ENAB0 + port);
	}
}

static int pcmmio_start_intr(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	if (!subpriv->dio.intr.continuous && subpriv->dio.intr.stop_count == 0) {
		/* An empty acquisition! */
		s->async->events |= COMEDI_CB_EOA;
		subpriv->dio.intr.active = 0;
		return 1;
	} else {
		unsigned bits = 0, pol_bits = 0, n;
		int nports, firstport, asic, port;
		struct comedi_cmd *cmd = &s->async->cmd;

		asic = subpriv->dio.intr.asic;
		if (asic < 0)
			return 1;	/* not an interrupt
					   subdev */
		subpriv->dio.intr.enabled_mask = 0;
		subpriv->dio.intr.active = 1;
		nports = subpriv->dio.intr.num_asic_chans / CHANS_PER_PORT;
		firstport = subpriv->dio.intr.asic_chan / CHANS_PER_PORT;
		if (cmd->chanlist) {
			for (n = 0; n < cmd->chanlist_len; n++) {
				bits |= (1U << CR_CHAN(cmd->chanlist[n]));
				pol_bits |= (CR_AREF(cmd->chanlist[n])
					     || CR_RANGE(cmd->
							 chanlist[n]) ? 1U : 0U)
				    << CR_CHAN(cmd->chanlist[n]);
			}
		}
		bits &= ((0x1 << subpriv->dio.intr.num_asic_chans) -
			 1) << subpriv->dio.intr.first_chan;
		subpriv->dio.intr.enabled_mask = bits;

		{
			/*
			 * the below code configures the board
			 * to use a specific IRQ from 0-15.
			 */
			unsigned char b;
			/*
			 * set resource enable register
			 * to enable IRQ operation
			 */
			outb(1 << 4, dev->iobase + 3);
			/* set bits 0-3 of b to the irq number from 0-15 */
			b = dev->irq & ((1 << 4) - 1);
			outb(b, dev->iobase + 2);
			/* done, we told the board what irq to use */
		}

		switch_page(dev, asic, PAGE_ENAB);
		for (port = firstport; port < firstport + nports; ++port) {
			unsigned enab =
			    bits >> (subpriv->dio.intr.first_chan + (port -
								     firstport)
				     * 8) & 0xff, pol =
			    pol_bits >> (subpriv->dio.intr.first_chan +
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
 * pcmmio.c
 * Driver for Winsystems PC-104 based multifunction IO board.
 *
 * COMEDI - Linux Control and Measurement Device Interface
 * Copyright (C) 2007 Calin A. Culianu <calin@ajvar.org>
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
 * Driver: pcmmio
 * Description: A driver for the PCM-MIO multifunction board
 * Devices: (Winsystems) PCM-MIO [pcmmio]
 * Author: Calin Culianu <calin@ajvar.org>
 * Updated: Wed, May 16 2007 16:21:10 -0500
 * Status: works
 *
 * A driver for the PCM-MIO multifunction board from Winsystems. This
 * is a PC-104 based I/O board. It contains four subdevices:
 *
 *	subdevice 0 - 16 channels of 16-bit AI
 *	subdevice 1 - 8 channels of 16-bit AO
 *	subdevice 2 - first 24 channels of the 48 channel of DIO
 *			(with edge-triggered interrupt support)
 *	subdevice 3 - last 24 channels of the 48 channel DIO
 *			(no interrupt support for this bank of channels)
 *
 * Some notes:
 *
 * Synchronous reads and writes are the only things implemented for analog
 * input and output. The hardware itself can do streaming acquisition, etc.
 *
 * Asynchronous I/O for the DIO subdevices *is* implemented, however! They
 * are basically edge-triggered interrupts for any configuration of the
 * channels in subdevice 2.
 *
 * Also note that this interrupt support is untested.
 *
 * A few words about edge-detection IRQ support (commands on DIO):
 *
 * To use edge-detection IRQ support for the DIO subdevice, pass the IRQ
 * of the board to the comedi_config command. The board IRQ is not jumpered
 * but rather configured through software, so any IRQ from 1-15 is OK.
 *
 * Due to the genericity of the comedi API, you need to create a special
 * comedi_command in order to use edge-triggered interrupts for DIO.
 *
 * Use comedi_commands with TRIG_NOW.  Your callback will be called each
 * time an edge is detected on the specified DIO line(s), and the data
 * values will be two sample_t's, which should be concatenated to form
 * one 32-bit unsigned int. This value is the mask of channels that had
 * edges detected from your channel list. Note that the bits positions
 * in the mask correspond to positions in your chanlist when you
 * specified the command and *not* channel id's!
 *
 * To set the polarity of the edge-detection interrupts pass a nonzero value
 * for either CR_RANGE or CR_AREF for edge-up polarity, or a zero
 * value for both CR_RANGE and CR_AREF if you want edge-down polarity.
 *
 * Configuration Options:
 *   [0] - I/O port base address
 *   [1] - IRQ (optional -- for edge-detect interrupt support only,
 *		leave out if you don't need this feature)
 */

#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>

#include "../comedidev.h"

#include "comedi_fc.h"

/*
 * Register I/O map
 */
#define PCMMIO_AI_LSB_REG			0x00
#define PCMMIO_AI_MSB_REG			0x01
#define PCMMIO_AI_CMD_REG			0x02
#define PCMMIO_AI_CMD_SE			(1 << 7)
#define PCMMIO_AI_CMD_ODD_CHAN			(1 << 6)
#define PCMMIO_AI_CMD_CHAN_SEL(x)		(((x) & 0x3) << 4)
#define PCMMIO_AI_CMD_RANGE(x)			(((x) & 0x3) << 2)
#define PCMMIO_RESOURCE_REG			0x02
#define PCMMIO_RESOURCE_IRQ(x)			(((x) & 0xf) << 0)
#define PCMMIO_AI_STATUS_REG			0x03
#define PCMMIO_AI_STATUS_DATA_READY		(1 << 7)
#define PCMMIO_AI_STATUS_DATA_DMA_PEND		(1 << 6)
#define PCMMIO_AI_STATUS_CMD_DMA_PEND		(1 << 5)
#define PCMMIO_AI_STATUS_IRQ_PEND		(1 << 4)
#define PCMMIO_AI_STATUS_DATA_DRQ_ENA		(1 << 2)
#define PCMMIO_AI_STATUS_REG_SEL		(1 << 3)
#define PCMMIO_AI_STATUS_CMD_DRQ_ENA		(1 << 1)
#define PCMMIO_AI_STATUS_IRQ_ENA		(1 << 0)
#define PCMMIO_AI_RES_ENA_REG			0x03
#define PCMMIO_AI_RES_ENA_CMD_REG_ACCESS	(0 << 3)
#define PCMMIO_AI_RES_ENA_AI_RES_ACCESS		(1 << 3)
#define PCMMIO_AI_RES_ENA_DIO_RES_ACCESS	(1 << 4)
#define PCMMIO_AI_2ND_ADC_OFFSET		0x04

#define PCMMIO_AO_LSB_REG			0x08
#define PCMMIO_AO_LSB_SPAN(x)			(((x) & 0xf) << 0)
#define PCMMIO_AO_MSB_REG			0x09
#define PCMMIO_AO_CMD_REG			0x0a
#define PCMMIO_AO_CMD_WR_SPAN			(0x2 << 4)
#define PCMMIO_AO_CMD_WR_CODE			(0x3 << 4)
#define PCMMIO_AO_CMD_UPDATE			(0x4 << 4)
#define PCMMIO_AO_CMD_UPDATE_ALL		(0x5 << 4)
#define PCMMIO_AO_CMD_WR_SPAN_UPDATE		(0x6 << 4)
#define PCMMIO_AO_CMD_WR_CODE_UPDATE		(0x7 << 4)
#define PCMMIO_AO_CMD_WR_SPAN_UPDATE_ALL	(0x8 << 4)
#define PCMMIO_AO_CMD_WR_CODE_UPDATE_ALL	(0x9 << 4)
#define PCMMIO_AO_CMD_RD_B1_SPAN		(0xa << 4)
#define PCMMIO_AO_CMD_RD_B1_CODE		(0xb << 4)
#define PCMMIO_AO_CMD_RD_B2_SPAN		(0xc << 4)
#define PCMMIO_AO_CMD_RD_B2_CODE		(0xd << 4)
#define PCMMIO_AO_CMD_NOP			(0xf << 4)
#define PCMMIO_AO_CMD_CHAN_SEL(x)		(((x) & 0x03) << 1)
#define PCMMIO_AO_CMD_CHAN_SEL_ALL		(0x0f << 0)
#define PCMMIO_AO_STATUS_REG			0x0b
#define PCMMIO_AO_STATUS_DATA_READY		(1 << 7)
#define PCMMIO_AO_STATUS_DATA_DMA_PEND		(1 << 6)
#define PCMMIO_AO_STATUS_CMD_DMA_PEND		(1 << 5)
#define PCMMIO_AO_STATUS_IRQ_PEND		(1 << 4)
#define PCMMIO_AO_STATUS_DATA_DRQ_ENA		(1 << 2)
#define PCMMIO_AO_STATUS_REG_SEL		(1 << 3)
#define PCMMIO_AO_STATUS_CMD_DRQ_ENA		(1 << 1)
#define PCMMIO_AO_STATUS_IRQ_ENA		(1 << 0)
#define PCMMIO_AO_RESOURCE_ENA_REG		0x0b
#define PCMMIO_AO_2ND_DAC_OFFSET		0x04

/*
 * WinSystems WS16C48
 *
 * Offset    Page 0       Page 1       Page 2       Page 3
 * ------  -----------  -----------  -----------  -----------
 *  0x10   Port 0 I/O   Port 0 I/O   Port 0 I/O   Port 0 I/O
 *  0x11   Port 1 I/O   Port 1 I/O   Port 1 I/O   Port 1 I/O
 *  0x12   Port 2 I/O   Port 2 I/O   Port 2 I/O   Port 2 I/O
 *  0x13   Port 3 I/O   Port 3 I/O   Port 3 I/O   Port 3 I/O
 *  0x14   Port 4 I/O   Port 4 I/O   Port 4 I/O   Port 4 I/O
 *  0x15   Port 5 I/O   Port 5 I/O   Port 5 I/O   Port 5 I/O
 *  0x16   INT_PENDING  INT_PENDING  INT_PENDING  INT_PENDING
 *  0x17    Page/Lock    Page/Lock    Page/Lock    Page/Lock
 *  0x18       N/A         POL_0       ENAB_0       INT_ID0
 *  0x19       N/A         POL_1       ENAB_1       INT_ID1
 *  0x1a       N/A         POL_2       ENAB_2       INT_ID2
 */
#define PCMMIO_PORT_REG(x)			(0x10 + (x))
#define PCMMIO_INT_PENDING_REG			0x16
#define PCMMIO_PAGE_LOCK_REG			0x17
#define PCMMIO_LOCK_PORT(x)			((1 << (x)) & 0x3f)
#define PCMMIO_PAGE(x)				(((x) & 0x3) << 6)
#define PCMMIO_PAGE_MASK			PCMUIO_PAGE(3)
#define PCMMIO_PAGE_POL				1
#define PCMMIO_PAGE_ENAB			2
#define PCMMIO_PAGE_INT_ID			3
#define PCMMIO_PAGE_REG(x)			(0x18 + (x))

static const struct comedi_lrange pcmmio_ai_ranges = {
	4, {
		BIP_RANGE(5),
		BIP_RANGE(10),
		UNI_RANGE(5),
		UNI_RANGE(10)
	}
};

static const struct comedi_lrange pcmmio_ao_ranges = {
	6, {
		UNI_RANGE(5),
		UNI_RANGE(10),
		BIP_RANGE(5),
		BIP_RANGE(10),
		BIP_RANGE(2.5),
		RANGE(-2.5, 7.5)
	}
};

struct pcmmio_private {
	spinlock_t pagelock;	/* protects the page registers */
	spinlock_t spinlock;	/* protects the member variables */
	unsigned int enabled_mask;
	unsigned int stop_count;
	unsigned int active:1;
	unsigned int continuous:1;

	unsigned int ao_readback[8];
};

static void pcmmio_dio_write(struct comedi_device *dev, unsigned int val,
			     int page, int port)
{
	struct pcmmio_private *devpriv = dev->private;
	unsigned long iobase = dev->iobase;
	unsigned long flags;

	spin_lock_irqsave(&devpriv->pagelock, flags);
	if (page == 0) {
		/* Port registers are valid for any page */
		outb(val & 0xff, iobase + PCMMIO_PORT_REG(port + 0));
		outb((val >> 8) & 0xff, iobase + PCMMIO_PORT_REG(port + 1));
		outb((val >> 16) & 0xff, iobase + PCMMIO_PORT_REG(port + 2));
	} else {
		outb(PCMMIO_PAGE(page), iobase + PCMMIO_PAGE_LOCK_REG);
		outb(val & 0xff, iobase + PCMMIO_PAGE_REG(0));
		outb((val >> 8) & 0xff, iobase + PCMMIO_PAGE_REG(1));
		outb((val >> 16) & 0xff, iobase + PCMMIO_PAGE_REG(2));
	}
	spin_unlock_irqrestore(&devpriv->pagelock, flags);
}

static unsigned int pcmmio_dio_read(struct comedi_device *dev,
				    int page, int port)
{
	struct pcmmio_private *devpriv = dev->private;
	unsigned long iobase = dev->iobase;
	unsigned long flags;
	unsigned int val;

	spin_lock_irqsave(&devpriv->pagelock, flags);
	if (page == 0) {
		/* Port registers are valid for any page */
		val = inb(iobase + PCMMIO_PORT_REG(port + 0));
		val |= (inb(iobase + PCMMIO_PORT_REG(port + 1)) << 8);
		val |= (inb(iobase + PCMMIO_PORT_REG(port + 2)) << 16);
	} else {
		outb(PCMMIO_PAGE(page), iobase + PCMMIO_PAGE_LOCK_REG);
		val = inb(iobase + PCMMIO_PAGE_REG(0));
		val |= (inb(iobase + PCMMIO_PAGE_REG(1)) << 8);
		val |= (inb(iobase + PCMMIO_PAGE_REG(2)) << 16);
	}
	spin_unlock_irqrestore(&devpriv->pagelock, flags);

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
static int pcmmio_dio_insn_bits(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn,
				unsigned int *data)
{
	/* subdevice 2 uses ports 0-2, subdevice 3 uses ports 3-5 */
	int port = s->index == 2 ? 0 : 3;
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
		pcmmio_dio_write(dev, val, 0, port);
	}

	/* get inverted state of the channels from the port */
	val = pcmmio_dio_read(dev, 0, port);

	/* return the true state of the channels */
	data[1] = ~val & chanmask;

	return insn->n;
}

static int pcmmio_dio_insn_config(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  struct comedi_insn *insn,
				  unsigned int *data)
{
	/* subdevice 2 uses ports 0-2, subdevice 3 uses ports 3-5 */
	int port = s->index == 2 ? 0 : 3;
	int ret;

	ret = comedi_dio_insn_config(dev, s, insn, data, 0);
	if (ret)
		return ret;

	if (data[0] == INSN_CONFIG_DIO_INPUT)
		pcmmio_dio_write(dev, s->io_bits, 0, port);

	return insn->n;
}

static void pcmmio_reset(struct comedi_device *dev)
{
	/* Clear all the DIO port bits */
	pcmmio_dio_write(dev, 0, 0, 0);
	pcmmio_dio_write(dev, 0, 0, 3);

	/* Clear all the paged registers */
	pcmmio_dio_write(dev, 0, PCMMIO_PAGE_POL, 0);
	pcmmio_dio_write(dev, 0, PCMMIO_PAGE_ENAB, 0);
	pcmmio_dio_write(dev, 0, PCMMIO_PAGE_INT_ID, 0);
}

/* devpriv->spinlock is already locked */
static void pcmmio_stop_intr(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	struct pcmmio_private *devpriv = dev->private;

	devpriv->enabled_mask = 0;
	devpriv->active = 0;
	s->async->inttrig = NULL;

	/* disable all dio interrupts */
	pcmmio_dio_write(dev, 0, PCMMIO_PAGE_ENAB, 0);
}

static void pcmmio_handle_dio_intr(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   unsigned int triggered)
{
	struct pcmmio_private *devpriv = dev->private;
	unsigned int oldevents = s->async->events;
	unsigned int len = s->async->cmd.chanlist_len;
	unsigned int val = 0;
	unsigned long flags;
	int i;

	spin_lock_irqsave(&devpriv->spinlock, flags);

	if (!devpriv->active)
		goto done;

	if (!(triggered & devpriv->enabled_mask))
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
		pcmmio_stop_intr(dev, s);
	}

	/* Check for end of acquisition. */
	if (!devpriv->continuous) {
		/* stop_src == TRIG_COUNT */
		if (devpriv->stop_count > 0) {
			devpriv->stop_count--;
			if (devpriv->stop_count == 0) {
				s->async->events |= COMEDI_CB_EOA;
				/* TODO: STOP_ACQUISITION_CALL_HERE!! */
				pcmmio_stop_intr(dev, s);
			}
		}
	}

done:
	spin_unlock_irqrestore(&devpriv->spinlock, flags);

	if (oldevents != s->async->events)
		comedi_event(dev, s);
}

static irqreturn_t interrupt_pcmmio(int irq, void *d)
{
	struct comedi_device *dev = d;
	struct comedi_subdevice *s = dev->read_subdev;
	unsigned int triggered;
	unsigned char int_pend;

	/* are there any interrupts pending */
	int_pend = inb(dev->iobase + PCMMIO_INT_PENDING_REG) & 0x07;
	if (!int_pend)
		return IRQ_NONE;

	/* get, and clear, the pending interrupts */
	triggered = pcmmio_dio_read(dev, PCMMIO_PAGE_INT_ID, 0);
	pcmmio_dio_write(dev, 0, PCMMIO_PAGE_INT_ID, 0);

	pcmmio_handle_dio_intr(dev, s, triggered);

	return IRQ_HANDLED;
}

/* devpriv->spinlock is already locked */
static int pcmmio_start_intr(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	struct pcmmio_private *devpriv = dev->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	unsigned int bits = 0;
	unsigned int pol_bits = 0;
	int i;

	if (!devpriv->continuous && devpriv->stop_count == 0) {
		/* An empty acquisition! */
		s->async->events |= COMEDI_CB_EOA;
		devpriv->active = 0;
		return 1;
	}

	devpriv->enabled_mask = 0;
	devpriv->active = 1;
	if (cmd->chanlist) {
		for (i = 0; i < cmd->chanlist_len; i++) {
			unsigned int chanspec = cmd->chanlist[i];
			unsigned int chan = CR_CHAN(chanspec);
			unsigned int range = CR_RANGE(chanspec);
			unsigned int aref = CR_AREF(chanspec);

			bits |= (1 << chan);
			pol_bits |= (((aref || range) ? 1 : 0) << chan);
		}
	}
	bits &= ((1 << s->n_chan) - 1);
	devpriv->enabled_mask = bits;

	/* set polarity and enable interrupts */
	pcmmio_dio_write(dev, pol_bits, PCMMIO_PAGE_POL, 0);
	pcmmio_dio_write(dev, bits, PCMMIO_PAGE_ENAB, 0);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static int pcmmio_cancel(struct comedi_device *dev, struct comedi_subdevice *s)
{
<<<<<<< HEAD
	unsigned long flags;

	spin_lock_irqsave(&subpriv->dio.intr.spinlock, flags);
	if (subpriv->dio.intr.active)
		pcmmio_stop_intr(dev, s);
	spin_unlock_irqrestore(&subpriv->dio.intr.spinlock, flags);
=======
	struct pcmmio_private *devpriv = dev->private;
	unsigned long flags;

	spin_lock_irqsave(&devpriv->spinlock, flags);
	if (devpriv->active)
		pcmmio_stop_intr(dev, s);
	spin_unlock_irqrestore(&devpriv->spinlock, flags);
>>>>>>> refs/remotes/origin/master

	return 0;
}

/*
 * Internal trigger function to start acquisition for an 'INTERRUPT' subdevice.
 */
static int
pcmmio_inttrig_start_intr(struct comedi_device *dev, struct comedi_subdevice *s,
			  unsigned int trignum)
{
<<<<<<< HEAD
=======
	struct pcmmio_private *devpriv = dev->private;
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	int event = 0;

	if (trignum != 0)
		return -EINVAL;

<<<<<<< HEAD
	spin_lock_irqsave(&subpriv->dio.intr.spinlock, flags);
	s->async->inttrig = 0;
	if (subpriv->dio.intr.active)
		event = pcmmio_start_intr(dev, s);
	spin_unlock_irqrestore(&subpriv->dio.intr.spinlock, flags);
=======
	spin_lock_irqsave(&devpriv->spinlock, flags);
	s->async->inttrig = NULL;
	if (devpriv->active)
		event = pcmmio_start_intr(dev, s);
	spin_unlock_irqrestore(&devpriv->spinlock, flags);
>>>>>>> refs/remotes/origin/master

	if (event)
		comedi_event(dev, s);

	return 1;
}

/*
 * 'do_cmd' function for an 'INTERRUPT' subdevice.
 */
static int pcmmio_cmd(struct comedi_device *dev, struct comedi_subdevice *s)
{
<<<<<<< HEAD
=======
	struct pcmmio_private *devpriv = dev->private;
>>>>>>> refs/remotes/origin/master
	struct comedi_cmd *cmd = &s->async->cmd;
	unsigned long flags;
	int event = 0;

<<<<<<< HEAD
	spin_lock_irqsave(&subpriv->dio.intr.spinlock, flags);
	subpriv->dio.intr.active = 1;
=======
	spin_lock_irqsave(&devpriv->spinlock, flags);
	devpriv->active = 1;
>>>>>>> refs/remotes/origin/master

	/* Set up end of acquisition. */
	switch (cmd->stop_src) {
	case TRIG_COUNT:
<<<<<<< HEAD
		subpriv->dio.intr.continuous = 0;
		subpriv->dio.intr.stop_count = cmd->stop_arg;
		break;
	default:
		/* TRIG_NONE */
		subpriv->dio.intr.continuous = 1;
		subpriv->dio.intr.stop_count = 0;
=======
		devpriv->continuous = 0;
		devpriv->stop_count = cmd->stop_arg;
		break;
	default:
		/* TRIG_NONE */
		devpriv->continuous = 1;
		devpriv->stop_count = 0;
>>>>>>> refs/remotes/origin/master
		break;
	}

	/* Set up start of acquisition. */
	switch (cmd->start_src) {
	case TRIG_INT:
		s->async->inttrig = pcmmio_inttrig_start_intr;
		break;
	default:
		/* TRIG_NOW */
		event = pcmmio_start_intr(dev, s);
		break;
	}
<<<<<<< HEAD
	spin_unlock_irqrestore(&subpriv->dio.intr.spinlock, flags);
=======
	spin_unlock_irqrestore(&devpriv->spinlock, flags);
>>>>>>> refs/remotes/origin/master

	if (event)
		comedi_event(dev, s);

	return 0;
}

<<<<<<< HEAD
static int
pcmmio_cmdtest(struct comedi_device *dev, struct comedi_subdevice *s,
	       struct comedi_cmd *cmd)
{
	return comedi_pcm_cmdtest(dev, s, cmd);
}

static int adc_wait_ready(unsigned long iobase)
{
	unsigned long retry = 100000;
	while (retry--)
		if (inb(iobase + 3) & 0x80)
			return 0;
	return 1;
}

/* All this is for AI and AO */
static int ai_rinsn(struct comedi_device *dev, struct comedi_subdevice *s,
		    struct comedi_insn *insn, unsigned int *data)
{
	int n;
	unsigned long iobase = subpriv->iobase;

	/*
	   1. write the CMD byte (to BASE+2)
	   2. read junk lo byte (BASE+0)
	   3. read junk hi byte (BASE+1)
	   4. (mux settled so) write CMD byte again (BASE+2)
	   5. read valid lo byte(BASE+0)
	   6. read valid hi byte(BASE+1)

	   Additionally note that the BASE += 4 if the channel >= 8
	 */

	/* convert n samples */
	for (n = 0; n < insn->n; n++) {
		unsigned chan = CR_CHAN(insn->chanspec), range =
		    CR_RANGE(insn->chanspec), aref = CR_AREF(insn->chanspec);
		unsigned char command_byte = 0;
		unsigned iooffset = 0;
		short sample, adc_adjust = 0;

		if (chan > 7)
			chan -= 8, iooffset = 4;	/*
							 * use the second dword
							 * for channels > 7
							 */

		if (aref != AREF_DIFF) {
			aref = AREF_GROUND;
			command_byte |= 1 << 7;	/*
						 * set bit 7 to indicate
						 * single-ended
						 */
		}
		if (range < 2)
			adc_adjust = 0x8000;	/*
						 * bipolar ranges
						 * (-5,5 .. -10,10 need to be
						 * adjusted -- that is.. they
						 * need to wrap around by
						 * adding 0x8000
						 */

		if (chan % 2) {
			command_byte |= 1 << 6;	/*
						 * odd-numbered channels
						 * have bit 6 set
						 */
		}

		/* select the channel, bits 4-5 == chan/2 */
		command_byte |= ((chan / 2) & 0x3) << 4;

		/* set the range, bits 2-3 */
		command_byte |= (range & 0x3) << 2;

		/* need to do this twice to make sure mux settled */
		/* chan/range/aref select */
		outb(command_byte, iobase + iooffset + 2);

		/* wait for the adc to say it finised the conversion */
		adc_wait_ready(iobase + iooffset);

		/* select the chan/range/aref AGAIN */
		outb(command_byte, iobase + iooffset + 2);

		adc_wait_ready(iobase + iooffset);

		/* read data lo byte */
		sample = inb(iobase + iooffset + 0);

		/* read data hi byte */
		sample |= inb(iobase + iooffset + 1) << 8;
		sample += adc_adjust;	/* adjustment .. munge data */
		data[n] = sample;
	}
	/* return the number of samples read/written */
	return n;
}

static int ao_rinsn(struct comedi_device *dev, struct comedi_subdevice *s,
		    struct comedi_insn *insn, unsigned int *data)
{
	int n;
	for (n = 0; n < insn->n; n++) {
		unsigned chan = CR_CHAN(insn->chanspec);
		if (chan < s->n_chan)
			data[n] = subpriv->ao.shadow_samples[chan];
	}
	return n;
}

static int wait_dac_ready(unsigned long iobase)
{
	unsigned long retry = 100000L;

	/* This may seem like an absurd way to handle waiting and violates the
	   "no busy waiting" policy. The fact is that the hardware is
	   normally so fast that we usually only need one time through the loop
	   anyway. The longer timeout is for rare occasions and for detecting
	   non-existent hardware.  */

	while (retry--) {
		if (inb(iobase + 3) & 0x80)
			return 0;

	}
	return 1;
}

static int ao_winsn(struct comedi_device *dev, struct comedi_subdevice *s,
		    struct comedi_insn *insn, unsigned int *data)
{
	int n;
	unsigned iobase = subpriv->iobase, iooffset = 0;

	for (n = 0; n < insn->n; n++) {
		unsigned chan = CR_CHAN(insn->chanspec), range =
		    CR_RANGE(insn->chanspec);
		if (chan < s->n_chan) {
			unsigned char command_byte = 0, range_byte =
			    range & ((1 << 4) - 1);
			if (chan >= 4)
				chan -= 4, iooffset += 4;
			/* set the range.. */
			outb(range_byte, iobase + iooffset + 0);
			outb(0, iobase + iooffset + 1);

			/* tell it to begin */
			command_byte = (chan << 1) | 0x60;
			outb(command_byte, iobase + iooffset + 2);

			wait_dac_ready(iobase + iooffset);

			/* low order byte */
			outb(data[n] & 0xff, iobase + iooffset + 0);

			/* high order byte */
			outb((data[n] >> 8) & 0xff, iobase + iooffset + 1);

			/*
			 * set bit 4 of command byte to indicate
			 * data is loaded and trigger conversion
			 */
			command_byte = 0x70 | (chan << 1);
			/* trigger converion */
			outb(command_byte, iobase + iooffset + 2);

			wait_dac_ready(iobase + iooffset);

			/* save to shadow register for ao_rinsn */
			subpriv->ao.shadow_samples[chan] = data[n];
		}
	}
	return n;
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

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi low-level driver");
=======
static int pcmmio_cmdtest(struct comedi_device *dev,
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

static int pcmmio_ai_wait_for_eoc(unsigned long iobase, unsigned int timeout)
{
	unsigned char status;

	while (timeout--) {
		status = inb(iobase + PCMMIO_AI_STATUS_REG);
		if (status & PCMMIO_AI_STATUS_DATA_READY)
			return 0;
	}
	return -ETIME;
}

static int pcmmio_ai_insn_read(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn,
			       unsigned int *data)
{
	unsigned long iobase = dev->iobase;
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int range = CR_RANGE(insn->chanspec);
	unsigned int aref = CR_AREF(insn->chanspec);
	unsigned char cmd = 0;
	unsigned int val;
	int ret;
	int i;

	/*
	 * The PCM-MIO uses two Linear Tech LTC1859CG 8-channel A/D converters.
	 * The devices use a full duplex serial interface which transmits and
	 * receives data simultaneously. An 8-bit command is shifted into the
	 * ADC interface to configure it for the next conversion. At the same
	 * time, the data from the previous conversion is shifted out of the
	 * device. Consequently, the conversion result is delayed by one
	 * conversion from the command word.
	 *
	 * Setup the cmd for the conversions then do a dummy conversion to
	 * flush the junk data. Then do each conversion requested by the
	 * comedi_insn. Note that the last conversion will leave junk data
	 * in ADC which will get flushed on the next comedi_insn.
	 */

	if (chan > 7) {
		chan -= 8;
		iobase += PCMMIO_AI_2ND_ADC_OFFSET;
	}

	if (aref == AREF_GROUND)
		cmd |= PCMMIO_AI_CMD_SE;
	if (chan % 2)
		cmd |= PCMMIO_AI_CMD_ODD_CHAN;
	cmd |= PCMMIO_AI_CMD_CHAN_SEL(chan / 2);
	cmd |= PCMMIO_AI_CMD_RANGE(range);

	outb(cmd, iobase + PCMMIO_AI_CMD_REG);
	ret = pcmmio_ai_wait_for_eoc(iobase, 100000);
	if (ret)
		return ret;

	val = inb(iobase + PCMMIO_AI_LSB_REG);
	val |= inb(iobase + PCMMIO_AI_MSB_REG) << 8;

	for (i = 0; i < insn->n; i++) {
		outb(cmd, iobase + PCMMIO_AI_CMD_REG);
		ret = pcmmio_ai_wait_for_eoc(iobase, 100000);
		if (ret)
			return ret;

		val = inb(iobase + PCMMIO_AI_LSB_REG);
		val |= inb(iobase + PCMMIO_AI_MSB_REG) << 8;

		/* bipolar data is two's complement */
		if (comedi_range_is_bipolar(s, range))
			val = comedi_offset_munge(s, val);

		data[i] = val;
	}

	return insn->n;
}

static int pcmmio_ao_insn_read(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn,
			       unsigned int *data)
{
	struct pcmmio_private *devpriv = dev->private;
	unsigned int chan = CR_CHAN(insn->chanspec);
	int i;

	for (i = 0; i < insn->n; i++)
		data[i] = devpriv->ao_readback[chan];

	return insn->n;
}

static int pcmmio_ao_wait_for_eoc(unsigned long iobase, unsigned int timeout)
{
	unsigned char status;

	while (timeout--) {
		status = inb(iobase + PCMMIO_AO_STATUS_REG);
		if (status & PCMMIO_AO_STATUS_DATA_READY)
			return 0;
	}
	return -ETIME;
}

static int pcmmio_ao_insn_write(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn,
				unsigned int *data)
{
	struct pcmmio_private *devpriv = dev->private;
	unsigned long iobase = dev->iobase;
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int range = CR_RANGE(insn->chanspec);
	unsigned int val = devpriv->ao_readback[chan];
	unsigned char cmd = 0;
	int ret;
	int i;

	/*
	 * The PCM-MIO has two Linear Tech LTC2704 DAC devices. Each device
	 * is a 4-channel converter with software-selectable output range.
	 */

	if (chan > 3) {
		cmd |= PCMMIO_AO_CMD_CHAN_SEL(chan - 4);
		iobase += PCMMIO_AO_2ND_DAC_OFFSET;
	} else {
		cmd |= PCMMIO_AO_CMD_CHAN_SEL(chan);
	}

	/* set the range for the channel */
	outb(PCMMIO_AO_LSB_SPAN(range), iobase + PCMMIO_AO_LSB_REG);
	outb(0, iobase + PCMMIO_AO_MSB_REG);
	outb(cmd | PCMMIO_AO_CMD_WR_SPAN_UPDATE, iobase + PCMMIO_AO_CMD_REG);
	ret = pcmmio_ao_wait_for_eoc(iobase, 100000);
	if (ret)
		return ret;

	for (i = 0; i < insn->n; i++) {
		val = data[i];

		/* write the data to the channel */
		outb(val & 0xff, iobase + PCMMIO_AO_LSB_REG);
		outb((val >> 8) & 0xff, iobase + PCMMIO_AO_MSB_REG);
		outb(cmd | PCMMIO_AO_CMD_WR_CODE_UPDATE,
		     iobase + PCMMIO_AO_CMD_REG);
		ret = pcmmio_ao_wait_for_eoc(iobase, 100000);
		if (ret)
			return ret;

		devpriv->ao_readback[chan] = val;
	}

	return insn->n;
}

static int pcmmio_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	struct pcmmio_private *devpriv;
	struct comedi_subdevice *s;
	int ret;

	ret = comedi_request_region(dev, it->options[0], 32);
	if (ret)
		return ret;

	devpriv = comedi_alloc_devpriv(dev, sizeof(*devpriv));
	if (!devpriv)
		return -ENOMEM;

	spin_lock_init(&devpriv->pagelock);
	spin_lock_init(&devpriv->spinlock);

	pcmmio_reset(dev);

	if (it->options[1]) {
		ret = request_irq(it->options[1], interrupt_pcmmio, 0,
				  dev->board_name, dev);
		if (ret == 0) {
			dev->irq = it->options[1];

			/* configure the interrupt routing on the board */
			outb(PCMMIO_AI_RES_ENA_DIO_RES_ACCESS,
			     dev->iobase + PCMMIO_AI_RES_ENA_REG);
			outb(PCMMIO_RESOURCE_IRQ(dev->irq),
			     dev->iobase + PCMMIO_RESOURCE_REG);
		}
	}

	ret = comedi_alloc_subdevices(dev, 4);
	if (ret)
		return ret;

	/* Analog Input subdevice */
	s = &dev->subdevices[0];
	s->type		= COMEDI_SUBD_AI;
	s->subdev_flags	= SDF_READABLE | SDF_GROUND | SDF_DIFF;
	s->n_chan	= 16;
	s->maxdata	= 0xffff;
	s->range_table	= &pcmmio_ai_ranges;
	s->insn_read	= pcmmio_ai_insn_read;

	/* initialize the resource enable register by clearing it */
	outb(PCMMIO_AI_RES_ENA_CMD_REG_ACCESS,
	     dev->iobase + PCMMIO_AI_RES_ENA_REG);
	outb(PCMMIO_AI_RES_ENA_CMD_REG_ACCESS,
	     dev->iobase + PCMMIO_AI_RES_ENA_REG + PCMMIO_AI_2ND_ADC_OFFSET);

	/* Analog Output subdevice */
	s = &dev->subdevices[1];
	s->type		= COMEDI_SUBD_AO;
	s->subdev_flags	= SDF_READABLE;
	s->n_chan	= 8;
	s->maxdata	= 0xffff;
	s->range_table	= &pcmmio_ao_ranges;
	s->insn_read	= pcmmio_ao_insn_read;
	s->insn_write	= pcmmio_ao_insn_write;

	/* initialize the resource enable register by clearing it */
	outb(0, dev->iobase + PCMMIO_AO_RESOURCE_ENA_REG);
	outb(0, dev->iobase + PCMMIO_AO_2ND_DAC_OFFSET +
		PCMMIO_AO_RESOURCE_ENA_REG);

	/* Digital I/O subdevice with interrupt support */
	s = &dev->subdevices[2];
	s->type		= COMEDI_SUBD_DIO;
	s->subdev_flags	= SDF_READABLE | SDF_WRITABLE;
	s->n_chan	= 24;
	s->maxdata	= 1;
	s->len_chanlist	= 1;
	s->range_table	= &range_digital;
	s->insn_bits	= pcmmio_dio_insn_bits;
	s->insn_config	= pcmmio_dio_insn_config;
	if (dev->irq) {
		dev->read_subdev = s;
		s->subdev_flags	|= SDF_CMD_READ;
		s->len_chanlist	= s->n_chan;
		s->cancel	= pcmmio_cancel;
		s->do_cmd	= pcmmio_cmd;
		s->do_cmdtest	= pcmmio_cmdtest;
	}

	/* Digital I/O subdevice */
	s = &dev->subdevices[3];
	s->type		= COMEDI_SUBD_DIO;
	s->subdev_flags	= SDF_READABLE | SDF_WRITABLE;
	s->n_chan	= 24;
	s->maxdata	= 1;
	s->range_table	= &range_digital;
	s->insn_bits	= pcmmio_dio_insn_bits;
	s->insn_config	= pcmmio_dio_insn_config;

	return 0;
}

static struct comedi_driver pcmmio_driver = {
	.driver_name	= "pcmmio",
	.module		= THIS_MODULE,
	.attach		= pcmmio_attach,
	.detach		= comedi_legacy_detach,
};
module_comedi_driver(pcmmio_driver);

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi driver for Winsystems PCM-MIO PC/104 board");
>>>>>>> refs/remotes/origin/master
MODULE_LICENSE("GPL");
