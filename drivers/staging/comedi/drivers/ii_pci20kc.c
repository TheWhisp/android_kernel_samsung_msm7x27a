/*
<<<<<<< HEAD
 *	comedi/drivers/ii_pci20kc.c
 *	Driver for Intelligent Instruments PCI-20001C carrier board
 *	and modules.
 *
 *	Copyright (C) 2000 Markus Kempf <kempf@matsci.uni-sb.de>
 *	with suggestions from David Schleef
 *			16.06.2000
 *
 *	Linux device driver for COMEDI
 *	Intelligent Instrumentation
 *	PCI-20001 C-2A Carrier Board
 *	PCI-20341 M-1A 16-Bit analog input module
 *				- differential
 *				- range (-5V - +5V)
 *				- 16 bit
 *	PCI-20006 M-2 16-Bit analog output module
 *				- ranges (-10V - +10V) (0V - +10V) (-5V - +5V)
 *				- 16 bit
 *
 *	only ONE PCI-20341 module possible
 *	only ONE PCI-20006 module possible
 *	no extern trigger implemented
 *
 *	NOT WORKING (but soon) only 4 on-board differential channels supported
 *	NOT WORKING (but soon) only ONE di-port and ONE do-port supported
 *			       instead of 4 digital ports
 *	di-port == Port 0
 *	do-port == Port 1
 *
 *	The state of this driver is only a starting point for a complete
 *	COMEDI-driver. The final driver should support all features of the
 *	carrier board and modules.
 *
 *	The test configuration:
 *
 *	kernel 2.2.14 with RTAI v1.2  and patch-2.2.14rthal2
 *	COMEDI 0.7.45
 *	COMEDILIB 0.7.9
 *
 */
/*
Driver: ii_pci20kc
Description: Intelligent Instruments PCI-20001C carrier board
Author: Markus Kempf <kempf@matsci.uni-sb.de>
Devices: [Intelligent Instrumentation] PCI-20001C (ii_pci20kc)
Status: works

Supports the PCI-20001 C-2a Carrier board, and could probably support
the other carrier boards with small modifications.  Modules supported
are:
	PCI-20006 M-2 16-bit analog output module
	PCI-20341 M-1A 16-bit analog input module

Options:
  0   Board base address
  1   IRQ
  2   first option for module 1
  3   second option for module 1
  4   first option for module 2
  5   second option for module 2
  6   first option for module 3
  7   second option for module 3

options for PCI-20006M:
  first:   Analog output channel 0 range configuration
	     0  bipolar 10  (-10V -- +10V)
	     1  unipolar 10  (0V -- +10V)
	     2  bipolar 5  (-5V -- 5V)
  second:  Analog output channel 1 range configuration

options for PCI-20341M:
  first:   Analog input gain configuration
	     0  1
	     1  10
	     2  100
	     3  200
*/

/* XXX needs to use ioremap() for compatibility with 2.4 kernels.  Should also
 * check_mem_region() etc. - fmhess */

#include "../comedidev.h"

#define PCI20000_ID			0x1d
#define PCI20341_ID			0x77
#define PCI20006_ID			0xe3
#define PCI20xxx_EMPTY_ID		0xff

#define PCI20000_OFFSET			0x100
#define PCI20000_MODULES		3

#define PCI20000_DIO_0			0x80
#define PCI20000_DIO_1			0x81
#define PCI20000_DIO_2			0xc0
#define PCI20000_DIO_3			0xc1
#define PCI20000_DIO_CONTROL_01		0x83	/* port 0, 1 control */
#define PCI20000_DIO_CONTROL_23		0xc3	/* port 2, 3 control */
#define PCI20000_DIO_BUFFER		0x82	/* buffer direction & enable */
#define PCI20000_DIO_EOC		0xef	/* even port, control output */
#define PCI20000_DIO_OOC		0xfd	/* odd port, control output */
#define PCI20000_DIO_EIC		0x90	/* even port, control input */
#define PCI20000_DIO_OIC		0x82	/* odd port, control input */
#define DIO_CAND			0x12	/* and bit 1 & 4 of control */
#define DIO_BE				0x01	/* buffer: port enable */
#define DIO_BO				0x04	/* buffer: output */
#define DIO_BI				0x05	/* buffer: input */
#define DIO_PS_0			0x00	/* buffer: port shift 0 */
#define DIO_PS_1			0x01	/* buffer: port shift 1 */
#define DIO_PS_2			0x04	/* buffer: port shift 2 */
#define DIO_PS_3			0x05	/* buffer: port shift 3 */

#define PCI20006_LCHAN0			0x0d
#define PCI20006_STROBE0		0x0b
#define PCI20006_LCHAN1			0x15
#define PCI20006_STROBE1		0x13

#define PCI20341_INIT			0x04
#define PCI20341_REPMODE		0x00	/* single shot mode */
#define PCI20341_PACER			0x00	/* Hardware Pacer disabled */
#define PCI20341_CHAN_NR		0x04	/* number of input channels */
#define PCI20341_CONFIG_REG		0x10
#define PCI20341_MOD_STATUS		0x01
#define PCI20341_OPT_REG		0x11
#define PCI20341_SET_TIME_REG		0x15
#define PCI20341_LCHAN_ADDR_REG		0x13
#define PCI20341_CHAN_LIST		0x80
#define PCI20341_CC_RESET		0x1b
#define PCI20341_CHAN_RESET		0x19
#define PCI20341_SOFT_PACER		0x04
#define PCI20341_STATUS_REG		0x12
#define PCI20341_LDATA			0x02
#define PCI20341_DAISY_CHAIN		0x20	/* On-board inputs only */
#define PCI20341_MUX			0x04	/* Enable on-board MUX */
#define PCI20341_SCANLIST		0x80	/* Channel/Gain Scan List */

union pci20xxx_subdev_private {
	void *iobase;
	struct {
		void *iobase;
		const struct comedi_lrange *ao_range_list[2];
					/* range of channels of ao module */
		unsigned int last_data[2];
	} pci20006;
	struct {
		void *iobase;
		int timebase;
		int settling_time;
		int ai_gain;
	} pci20341;
};

struct pci20xxx_private {

	void *ioaddr;
	union pci20xxx_subdev_private subdev_private[PCI20000_MODULES];
};

#define devpriv ((struct pci20xxx_private *)dev->private)
#define CHAN (CR_CHAN(it->chanlist[0]))

static int pci20xxx_attach(struct comedi_device *dev,
			   struct comedi_devconfig *it);
static int pci20xxx_detach(struct comedi_device *dev);

static struct comedi_driver driver_pci20xxx = {
	.driver_name = "ii_pci20kc",
	.module = THIS_MODULE,
	.attach = pci20xxx_attach,
	.detach = pci20xxx_detach,
};

static int pci20006_init(struct comedi_device *dev, struct comedi_subdevice *s,
			 int opt0, int opt1);
static int pci20341_init(struct comedi_device *dev, struct comedi_subdevice *s,
			 int opt0, int opt1);
static int pci20xxx_dio_init(struct comedi_device *dev,
			     struct comedi_subdevice *s);

/*
  options[0]	Board base address
  options[1]	IRQ
  options[2]	first option for module 1
  options[3]	second option for module 1
  options[4]	first option for module 2
  options[5]	second option for module 2
  options[6]	first option for module 3
  options[7]	second option for module 3

  options for PCI-20341M:
  first		Analog input gain configuration
		0 == 1
		1 == 10
		2 == 100
		3 == 200

  options for PCI-20006M:
  first		Analog output channel 0 range configuration
		0 == bipolar 10  (-10V -- +10V)
		1 == unipolar 10V  (0V -- +10V)
		2 == bipolar 5V  (-5V -- +5V)
  second	Analog output channel 1 range configuration
		0 == bipolar 10  (-10V -- +10V)
		1 == unipolar 10V  (0V -- +10V)
		2 == bipolar 5V  (-5V -- +5V)
*/
static int pci20xxx_attach(struct comedi_device *dev,
			   struct comedi_devconfig *it)
{
	unsigned char i;
	int ret;
	int id;
	struct comedi_subdevice *s;
	union pci20xxx_subdev_private *sdp;

	ret = alloc_subdevices(dev, 1 + PCI20000_MODULES);
	if (ret < 0)
		return ret;

	ret = alloc_private(dev, sizeof(struct pci20xxx_private));
	if (ret < 0)
		return ret;

	devpriv->ioaddr = (void *)(unsigned long)it->options[0];
	dev->board_name = "pci20kc";

	/* Check PCI-20001 C-2A Carrier Board ID */
	if ((readb(devpriv->ioaddr) & PCI20000_ID) != PCI20000_ID) {
		printk(KERN_WARNING "comedi%d: ii_pci20kc PCI-20001"
		       " C-2A Carrier Board at base=0x%p not found !\n",
		       dev->minor, devpriv->ioaddr);
		return -EINVAL;
	}
	printk(KERN_INFO "comedi%d: ii_pci20kc: PCI-20001 C-2A at base=0x%p\n",
	       dev->minor, devpriv->ioaddr);

	for (i = 0; i < PCI20000_MODULES; i++) {
		s = dev->subdevices + i;
		id = readb(devpriv->ioaddr + (i + 1) * PCI20000_OFFSET);
		s->private = devpriv->subdev_private + i;
		sdp = s->private;
		switch (id) {
		case PCI20006_ID:
			sdp->pci20006.iobase =
			    devpriv->ioaddr + (i + 1) * PCI20000_OFFSET;
			pci20006_init(dev, s, it->options[2 * i + 2],
				      it->options[2 * i + 3]);
			printk(KERN_INFO "comedi%d: "
			       "ii_pci20kc PCI-20006 module in slot %d\n",
			       dev->minor, i + 1);
			break;
		case PCI20341_ID:
			sdp->pci20341.iobase =
			    devpriv->ioaddr + (i + 1) * PCI20000_OFFSET;
			pci20341_init(dev, s, it->options[2 * i + 2],
				      it->options[2 * i + 3]);
			printk(KERN_INFO "comedi%d: "
			       "ii_pci20kc PCI-20341 module in slot %d\n",
			       dev->minor, i + 1);
			break;
		default:
			printk(KERN_WARNING "ii_pci20kc: unknown module "
			       "code 0x%02x in slot %d: module disabled\n",
			       id, i); /* XXX this looks like a bug! i + 1 ?? */
			/* fall through */
		case PCI20xxx_EMPTY_ID:
			s->type = COMEDI_SUBD_UNUSED;
			break;
		}
	}

	/* initialize struct pci20xxx_private */
	pci20xxx_dio_init(dev, dev->subdevices + PCI20000_MODULES);

	return 1;
}

static int pci20xxx_detach(struct comedi_device *dev)
{
	printk(KERN_INFO "comedi%d: pci20xxx: remove\n", dev->minor);

	return 0;
}

/* pci20006m */

static int pci20006_insn_read(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_insn *insn, unsigned int *data);
static int pci20006_insn_write(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data);

static const struct comedi_lrange *pci20006_range_list[] = {
	&range_bipolar10,
	&range_unipolar10,
	&range_bipolar5,
};

static int pci20006_init(struct comedi_device *dev, struct comedi_subdevice *s,
			 int opt0, int opt1)
{
	union pci20xxx_subdev_private *sdp = s->private;

	if (opt0 < 0 || opt0 > 2)
		opt0 = 0;
	if (opt1 < 0 || opt1 > 2)
		opt1 = 0;

	sdp->pci20006.ao_range_list[0] = pci20006_range_list[opt0];
	sdp->pci20006.ao_range_list[1] = pci20006_range_list[opt1];

	/* ao subdevice */
	s->type = COMEDI_SUBD_AO;
	s->subdev_flags = SDF_WRITABLE;
	s->n_chan = 2;
	s->len_chanlist = 2;
	s->insn_read = pci20006_insn_read;
	s->insn_write = pci20006_insn_write;
	s->maxdata = 0xffff;
	s->range_table_list = sdp->pci20006.ao_range_list;
	return 0;
}

static int pci20006_insn_read(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_insn *insn, unsigned int *data)
{
	union pci20xxx_subdev_private *sdp = s->private;

	data[0] = sdp->pci20006.last_data[CR_CHAN(insn->chanspec)];

	return 1;
}

static int pci20006_insn_write(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data)
{
	union pci20xxx_subdev_private *sdp = s->private;
	int hi, lo;
	unsigned int boarddata;

	sdp->pci20006.last_data[CR_CHAN(insn->chanspec)] = data[0];
	boarddata = (((unsigned int)data[0] + 0x8000) & 0xffff);
						/* comedi-data -> board-data */
	lo = (boarddata & 0xff);
	hi = ((boarddata >> 8) & 0xff);

	switch (CR_CHAN(insn->chanspec)) {
	case 0:
		writeb(lo, sdp->iobase + PCI20006_LCHAN0);
		writeb(hi, sdp->iobase + PCI20006_LCHAN0 + 1);
		writeb(0x00, sdp->iobase + PCI20006_STROBE0);
		break;
	case 1:
		writeb(lo, sdp->iobase + PCI20006_LCHAN1);
		writeb(hi, sdp->iobase + PCI20006_LCHAN1 + 1);
		writeb(0x00, sdp->iobase + PCI20006_STROBE1);
		break;
	default:
		printk(KERN_WARNING
		       " comedi%d: pci20xxx: ao channel Error!\n", dev->minor);
		return -EINVAL;
	}

	return 1;
}

/* PCI20341M */

static int pci20341_insn_read(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_insn *insn, unsigned int *data);

static const int pci20341_timebase[] = { 0x00, 0x00, 0x00, 0x04 };
static const int pci20341_settling_time[] = { 0x58, 0x58, 0x93, 0x99 };

static const struct comedi_lrange range_bipolar0_5 = {
	1,
	{BIP_RANGE(0.5)}
};

static const struct comedi_lrange range_bipolar0_05 = {
	1,
	{BIP_RANGE(0.05)}
};

static const struct comedi_lrange range_bipolar0_025 = {
	1,
	{BIP_RANGE(0.025)}
};

static const struct comedi_lrange *const pci20341_ranges[] = {
	&range_bipolar5,
	&range_bipolar0_5,
	&range_bipolar0_05,
	&range_bipolar0_025,
};

static int pci20341_init(struct comedi_device *dev, struct comedi_subdevice *s,
			 int opt0, int opt1)
{
	union pci20xxx_subdev_private *sdp = s->private;
	int option;

	/* options handling */
	if (opt0 < 0 || opt0 > 3)
		opt0 = 0;
	sdp->pci20341.timebase = pci20341_timebase[opt0];
	sdp->pci20341.settling_time = pci20341_settling_time[opt0];

	/* ai subdevice */
	s->type = COMEDI_SUBD_AI;
	s->subdev_flags = SDF_READABLE;
	s->n_chan = PCI20341_CHAN_NR;
	s->len_chanlist = PCI20341_SCANLIST;
	s->insn_read = pci20341_insn_read;
	s->maxdata = 0xffff;
	s->range_table = pci20341_ranges[opt0];

	/* depends on gain, trigger, repetition mode */
	option = sdp->pci20341.timebase | PCI20341_REPMODE;

	/* initialize Module */
	writeb(PCI20341_INIT, sdp->iobase + PCI20341_CONFIG_REG);
	/* set Pacer */
	writeb(PCI20341_PACER, sdp->iobase + PCI20341_MOD_STATUS);
	/* option register */
	writeb(option, sdp->iobase + PCI20341_OPT_REG);
	/* settling time counter */
	writeb(sdp->pci20341.settling_time,
		sdp->iobase + PCI20341_SET_TIME_REG);
	/* trigger not implemented */
	return 0;
}

static int pci20341_insn_read(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_insn *insn, unsigned int *data)
{
	union pci20xxx_subdev_private *sdp = s->private;
	unsigned int i = 0, j = 0;
	int lo, hi;
	unsigned char eoc;	/* end of conversion */
	unsigned int clb;	/* channel list byte */
	unsigned int boarddata;

	/* write number of input channels */
	writeb(1, sdp->iobase + PCI20341_LCHAN_ADDR_REG);
	clb = PCI20341_DAISY_CHAIN | PCI20341_MUX | (sdp->pci20341.ai_gain << 3)
	    | CR_CHAN(insn->chanspec);
	writeb(clb, sdp->iobase + PCI20341_CHAN_LIST);

	/* reset settling time counter and trigger delay counter */
	writeb(0x00, sdp->iobase + PCI20341_CC_RESET);

	writeb(0x00, sdp->iobase + PCI20341_CHAN_RESET);

	/* generate Pacer */

	for (i = 0; i < insn->n; i++) {
		/* data polling isn't the niciest way to get the data, I know,
		 * but there are only 6 cycles (mean) and it is easier than
		 * the whole interrupt stuff
		 */
		j = 0;
		/* generate Pacer */
		readb(sdp->iobase + PCI20341_SOFT_PACER);

		eoc = readb(sdp->iobase + PCI20341_STATUS_REG);
		/* poll Interrupt Flag */
		while ((eoc < 0x80) && j < 100) {
			j++;
			eoc = readb(sdp->iobase + PCI20341_STATUS_REG);
		}
		if (j >= 100) {
			printk(KERN_WARNING
			       "comedi%d:  pci20xxx: "
			       "AI interrupt channel %i polling exit !\n",
			       dev->minor, i);
			return -EINVAL;
		}
		lo = readb(sdp->iobase + PCI20341_LDATA);
		hi = readb(sdp->iobase + PCI20341_LDATA + 1);
		boarddata = lo + 0x100 * hi;

		/* board-data -> comedi-data */
		data[i] = (short)((boarddata + 0x8000) & 0xffff);
	}

	return i;
}

/* native DIO */

static void pci20xxx_dio_config(struct comedi_device *dev,
				struct comedi_subdevice *s);
static int pci20xxx_dio_insn_bits(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  struct comedi_insn *insn, unsigned int *data);
static int pci20xxx_dio_insn_config(struct comedi_device *dev,
				    struct comedi_subdevice *s,
				    struct comedi_insn *insn,
				    unsigned int *data);

/* initialize struct pci20xxx_private */
static int pci20xxx_dio_init(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{

	s->type = COMEDI_SUBD_DIO;
	s->subdev_flags = SDF_READABLE | SDF_WRITABLE;
	s->n_chan = 32;
	s->insn_bits = pci20xxx_dio_insn_bits;
	s->insn_config = pci20xxx_dio_insn_config;
	s->maxdata = 1;
	s->len_chanlist = 32;
	s->range_table = &range_digital;
	s->io_bits = 0;

	/* digital I/O lines default to input on board reset. */
	pci20xxx_dio_config(dev, s);

	return 0;
}

static int pci20xxx_dio_insn_config(struct comedi_device *dev,
				    struct comedi_subdevice *s,
				    struct comedi_insn *insn,
				    unsigned int *data)
{
	int mask, bits;

	mask = 1 << CR_CHAN(insn->chanspec);
	if (mask & 0x000000ff)
		bits = 0x000000ff;
	else if (mask & 0x0000ff00)
		bits = 0x0000ff00;
	else if (mask & 0x00ff0000)
		bits = 0x00ff0000;
	else
		bits = 0xff000000;
	if (data[0])
		s->io_bits |= bits;
	else
		s->io_bits &= ~bits;
	pci20xxx_dio_config(dev, s);

	return 1;
}

static int pci20xxx_dio_insn_bits(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  struct comedi_insn *insn, unsigned int *data)
{
	unsigned int mask = data[0];

	s->state &= ~mask;
	s->state |= (mask & data[1]);

	mask &= s->io_bits;
	if (mask & 0x000000ff)
		writeb((s->state >> 0) & 0xff,
		       devpriv->ioaddr + PCI20000_DIO_0);
	if (mask & 0x0000ff00)
		writeb((s->state >> 8) & 0xff,
		       devpriv->ioaddr + PCI20000_DIO_1);
	if (mask & 0x00ff0000)
		writeb((s->state >> 16) & 0xff,
		       devpriv->ioaddr + PCI20000_DIO_2);
	if (mask & 0xff000000)
		writeb((s->state >> 24) & 0xff,
		       devpriv->ioaddr + PCI20000_DIO_3);

	data[1] = readb(devpriv->ioaddr + PCI20000_DIO_0);
	data[1] |= readb(devpriv->ioaddr + PCI20000_DIO_1) << 8;
	data[1] |= readb(devpriv->ioaddr + PCI20000_DIO_2) << 16;
	data[1] |= readb(devpriv->ioaddr + PCI20000_DIO_3) << 24;

	return 2;
}

static void pci20xxx_dio_config(struct comedi_device *dev,
				struct comedi_subdevice *s)
{
	unsigned char control_01;
	unsigned char control_23;
	unsigned char buffer;

	control_01 = readb(devpriv->ioaddr + PCI20000_DIO_CONTROL_01);
	control_23 = readb(devpriv->ioaddr + PCI20000_DIO_CONTROL_23);
	buffer = readb(devpriv->ioaddr + PCI20000_DIO_BUFFER);

	if (s->io_bits & 0x000000ff) {
		/* output port 0 */
		control_01 &= PCI20000_DIO_EOC;
		buffer = (buffer & (~(DIO_BE << DIO_PS_0))) | (DIO_BO <<
							       DIO_PS_0);
	} else {
		/* input port 0 */
		control_01 = (control_01 & DIO_CAND) | PCI20000_DIO_EIC;
		buffer = (buffer & (~(DIO_BI << DIO_PS_0)));
	}
	if (s->io_bits & 0x0000ff00) {
		/* output port 1 */
		control_01 &= PCI20000_DIO_OOC;
		buffer = (buffer & (~(DIO_BE << DIO_PS_1))) | (DIO_BO <<
							       DIO_PS_1);
	} else {
		/* input port 1 */
		control_01 = (control_01 & DIO_CAND) | PCI20000_DIO_OIC;
		buffer = (buffer & (~(DIO_BI << DIO_PS_1)));
	}
	if (s->io_bits & 0x00ff0000) {
		/* output port 2 */
		control_23 &= PCI20000_DIO_EOC;
		buffer = (buffer & (~(DIO_BE << DIO_PS_2))) | (DIO_BO <<
							       DIO_PS_2);
	} else {
		/* input port 2 */
		control_23 = (control_23 & DIO_CAND) | PCI20000_DIO_EIC;
		buffer = (buffer & (~(DIO_BI << DIO_PS_2)));
	}
	if (s->io_bits & 0xff000000) {
		/* output port 3 */
		control_23 &= PCI20000_DIO_OOC;
		buffer = (buffer & (~(DIO_BE << DIO_PS_3))) | (DIO_BO <<
							       DIO_PS_3);
	} else {
		/* input port 3 */
		control_23 = (control_23 & DIO_CAND) | PCI20000_DIO_OIC;
		buffer = (buffer & (~(DIO_BI << DIO_PS_3)));
	}
	writeb(control_01, devpriv->ioaddr + PCI20000_DIO_CONTROL_01);
	writeb(control_23, devpriv->ioaddr + PCI20000_DIO_CONTROL_23);
	writeb(buffer, devpriv->ioaddr + PCI20000_DIO_BUFFER);
}

#if 0
static void pci20xxx_do(struct comedi_device *dev, struct comedi_subdevice *s)
{
	/* XXX if the channel is configured for input, does this
	   do bad things? */
	/* XXX it would be a good idea to only update the registers
	   that _need_ to be updated.  This requires changes to
	   comedi, however. */
	writeb((s->state >> 0) & 0xff, devpriv->ioaddr + PCI20000_DIO_0);
	writeb((s->state >> 8) & 0xff, devpriv->ioaddr + PCI20000_DIO_1);
	writeb((s->state >> 16) & 0xff, devpriv->ioaddr + PCI20000_DIO_2);
	writeb((s->state >> 24) & 0xff, devpriv->ioaddr + PCI20000_DIO_3);
}

static unsigned int pci20xxx_di(struct comedi_device *dev,
				struct comedi_subdevice *s)
{
	/* XXX same note as above */
	unsigned int bits;

	bits = readb(devpriv->ioaddr + PCI20000_DIO_0);
	bits |= readb(devpriv->ioaddr + PCI20000_DIO_1) << 8;
	bits |= readb(devpriv->ioaddr + PCI20000_DIO_2) << 16;
	bits |= readb(devpriv->ioaddr + PCI20000_DIO_3) << 24;

	return bits;
}
#endif

static int __init driver_pci20xxx_init_module(void)
{
	return comedi_driver_register(&driver_pci20xxx);
}

static void __exit driver_pci20xxx_cleanup_module(void)
{
	comedi_driver_unregister(&driver_pci20xxx);
}

module_init(driver_pci20xxx_init_module);
module_exit(driver_pci20xxx_cleanup_module);
=======
 * ii_pci20kc.c
 * Driver for Intelligent Instruments PCI-20001C carrier board and modules.
 *
 * Copyright (C) 2000 Markus Kempf <kempf@matsci.uni-sb.de>
 * with suggestions from David Schleef		16.06.2000
 */

/*
 * Driver: ii_pci20kc
 * Description: Intelligent Instruments PCI-20001C carrier board
 * Devices: (Intelligent Instrumentation) PCI-20001C [ii_pci20kc]
 * Author: Markus Kempf <kempf@matsci.uni-sb.de>
 * Status: works
 *
 * Supports the PCI-20001C-1a and PCI-20001C-2a carrier boards. The
 * -2a version has 32 on-board DIO channels. Three add-on modules
 * can be added to the carrier board for additional functionality.
 *
 * Supported add-on modules:
 *	PCI-20006M-1   1 channel, 16-bit analog output module
 *	PCI-20006M-2   2 channel, 16-bit analog output module
 *	PCI-20341M-1A  4 channel, 16-bit analog input module
 *
 * Options:
 *   0   Board base address
 *   1   IRQ (not-used)
 */

#include <linux/module.h>
#include "../comedidev.h"

/*
 * Register I/O map
 */
#define II20K_MOD_OFFSET		0x100
#define II20K_ID_REG			0x00
#define II20K_ID_MOD1_EMPTY		(1 << 7)
#define II20K_ID_MOD2_EMPTY		(1 << 6)
#define II20K_ID_MOD3_EMPTY		(1 << 5)
#define II20K_ID_MASK			0x1f
#define II20K_ID_PCI20001C_1A		0x1b	/* no on-board DIO */
#define II20K_ID_PCI20001C_2A		0x1d	/* on-board DIO */
#define II20K_MOD_STATUS_REG		0x40
#define II20K_MOD_STATUS_IRQ_MOD1	(1 << 7)
#define II20K_MOD_STATUS_IRQ_MOD2	(1 << 6)
#define II20K_MOD_STATUS_IRQ_MOD3	(1 << 5)
#define II20K_DIO0_REG			0x80
#define II20K_DIO1_REG			0x81
#define II20K_DIR_ENA_REG		0x82
#define II20K_DIR_DIO3_OUT		(1 << 7)
#define II20K_DIR_DIO2_OUT		(1 << 6)
#define II20K_BUF_DISAB_DIO3		(1 << 5)
#define II20K_BUF_DISAB_DIO2		(1 << 4)
#define II20K_DIR_DIO1_OUT		(1 << 3)
#define II20K_DIR_DIO0_OUT		(1 << 2)
#define II20K_BUF_DISAB_DIO1		(1 << 1)
#define II20K_BUF_DISAB_DIO0		(1 << 0)
#define II20K_CTRL01_REG		0x83
#define II20K_CTRL01_SET		(1 << 7)
#define II20K_CTRL01_DIO0_IN		(1 << 4)
#define II20K_CTRL01_DIO1_IN		(1 << 1)
#define II20K_DIO2_REG			0xc0
#define II20K_DIO3_REG			0xc1
#define II20K_CTRL23_REG		0xc3
#define II20K_CTRL23_SET		(1 << 7)
#define II20K_CTRL23_DIO2_IN		(1 << 4)
#define II20K_CTRL23_DIO3_IN		(1 << 1)

#define II20K_ID_PCI20006M_1		0xe2	/* 1 AO channels */
#define II20K_ID_PCI20006M_2		0xe3	/* 2 AO channels */
#define II20K_AO_STRB_REG(x)		(0x0b + ((x) * 0x08))
#define II20K_AO_LSB_REG(x)		(0x0d + ((x) * 0x08))
#define II20K_AO_MSB_REG(x)		(0x0e + ((x) * 0x08))
#define II20K_AO_STRB_BOTH_REG		0x1b

#define II20K_ID_PCI20341M_1		0x77	/* 4 AI channels */
#define II20K_AI_STATUS_CMD_REG		0x01
#define II20K_AI_STATUS_CMD_BUSY	(1 << 7)
#define II20K_AI_STATUS_CMD_HW_ENA	(1 << 1)
#define II20K_AI_STATUS_CMD_EXT_START	(1 << 0)
#define II20K_AI_LSB_REG		0x02
#define II20K_AI_MSB_REG		0x03
#define II20K_AI_PACER_RESET_REG	0x04
#define II20K_AI_16BIT_DATA_REG		0x06
#define II20K_AI_CONF_REG		0x10
#define II20K_AI_CONF_ENA		(1 << 2)
#define II20K_AI_OPT_REG		0x11
#define II20K_AI_OPT_TRIG_ENA		(1 << 5)
#define II20K_AI_OPT_TRIG_INV		(1 << 4)
#define II20K_AI_OPT_TIMEBASE(x)	(((x) & 0x3) << 1)
#define II20K_AI_OPT_BURST_MODE		(1 << 0)
#define II20K_AI_STATUS_REG		0x12
#define II20K_AI_STATUS_INT		(1 << 7)
#define II20K_AI_STATUS_TRIG		(1 << 6)
#define II20K_AI_STATUS_TRIG_ENA	(1 << 5)
#define II20K_AI_STATUS_PACER_ERR	(1 << 2)
#define II20K_AI_STATUS_DATA_ERR	(1 << 1)
#define II20K_AI_STATUS_SET_TIME_ERR	(1 << 0)
#define II20K_AI_LAST_CHAN_ADDR_REG	0x13
#define II20K_AI_CUR_ADDR_REG		0x14
#define II20K_AI_SET_TIME_REG		0x15
#define II20K_AI_DELAY_LSB_REG		0x16
#define II20K_AI_DELAY_MSB_REG		0x17
#define II20K_AI_CHAN_ADV_REG		0x18
#define II20K_AI_CHAN_RESET_REG		0x19
#define II20K_AI_START_TRIG_REG		0x1a
#define II20K_AI_COUNT_RESET_REG	0x1b
#define II20K_AI_CHANLIST_REG		0x80
#define II20K_AI_CHANLIST_ONBOARD_ONLY	(1 << 5)
#define II20K_AI_CHANLIST_GAIN(x)	(((x) & 0x3) << 3)
#define II20K_AI_CHANLIST_MUX_ENA	(1 << 2)
#define II20K_AI_CHANLIST_CHAN(x)	(((x) & 0x3) << 0)
#define II20K_AI_CHANLIST_LEN		0x80

/* the AO range is set by jumpers on the 20006M module */
static const struct comedi_lrange ii20k_ao_ranges = {
	3, {
		BIP_RANGE(5),	/* Chan 0 - W1/W3 in   Chan 1 - W2/W4 in  */
		UNI_RANGE(10),	/* Chan 0 - W1/W3 out  Chan 1 - W2/W4 in  */
		BIP_RANGE(10)	/* Chan 0 - W1/W3 in   Chan 1 - W2/W4 out */
	}
};

static const struct comedi_lrange ii20k_ai_ranges = {
	4, {
		BIP_RANGE(5),		/* gain 1 */
		BIP_RANGE(0.5),		/* gain 10 */
		BIP_RANGE(0.05),	/* gain 100 */
		BIP_RANGE(0.025)	/* gain 200 */
	},
};

struct ii20k_ao_private {
	unsigned int last_data[2];
};

struct ii20k_private {
	void __iomem *ioaddr;
};

static void __iomem *ii20k_module_iobase(struct comedi_device *dev,
					 struct comedi_subdevice *s)
{
	struct ii20k_private *devpriv = dev->private;

	return devpriv->ioaddr + (s->index + 1) * II20K_MOD_OFFSET;
}

static int ii20k_ao_insn_read(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_insn *insn,
			      unsigned int *data)
{
	struct ii20k_ao_private *ao_spriv = s->private;
	unsigned int chan = CR_CHAN(insn->chanspec);
	int i;

	for (i = 0; i < insn->n; i++)
		data[i] = ao_spriv->last_data[chan];

	return insn->n;
}

static int ii20k_ao_insn_write(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn,
			       unsigned int *data)
{
	struct ii20k_ao_private *ao_spriv = s->private;
	void __iomem *iobase = ii20k_module_iobase(dev, s);
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int val = ao_spriv->last_data[chan];
	int i;

	for (i = 0; i < insn->n; i++) {
		val = data[i];

		/* munge data */
		val += ((s->maxdata + 1) >> 1);
		val &= s->maxdata;

		writeb(val & 0xff, iobase + II20K_AO_LSB_REG(chan));
		writeb((val >> 8) & 0xff, iobase + II20K_AO_MSB_REG(chan));
		writeb(0x00, iobase + II20K_AO_STRB_REG(chan));
	}

	ao_spriv->last_data[chan] = val;

	return insn->n;
}

static int ii20k_ai_wait_eoc(struct comedi_device *dev,
			     struct comedi_subdevice *s,
			     int timeout)
{
	void __iomem *iobase = ii20k_module_iobase(dev, s);
	unsigned char status;

	do {
		status = readb(iobase + II20K_AI_STATUS_REG);
		if ((status & II20K_AI_STATUS_INT) == 0)
			return 0;
	} while (timeout--);

	return -ETIME;
}

static void ii20k_ai_setup(struct comedi_device *dev,
			   struct comedi_subdevice *s,
			   unsigned int chanspec)
{
	void __iomem *iobase = ii20k_module_iobase(dev, s);
	unsigned int chan = CR_CHAN(chanspec);
	unsigned int range = CR_RANGE(chanspec);
	unsigned char val;

	/* initialize module */
	writeb(II20K_AI_CONF_ENA, iobase + II20K_AI_CONF_REG);

	/* software conversion */
	writeb(0, iobase + II20K_AI_STATUS_CMD_REG);

	/* set the time base for the settling time counter based on the gain */
	val = (range < 3) ? II20K_AI_OPT_TIMEBASE(0) : II20K_AI_OPT_TIMEBASE(2);
	writeb(val, iobase + II20K_AI_OPT_REG);

	/* set the settling time counter based on the gain */
	val = (range < 2) ? 0x58 : (range < 3) ? 0x93 : 0x99;
	writeb(val, iobase + II20K_AI_SET_TIME_REG);

	/* set number of input channels */
	writeb(1, iobase + II20K_AI_LAST_CHAN_ADDR_REG);

	/* set the channel list byte */
	val = II20K_AI_CHANLIST_ONBOARD_ONLY |
	      II20K_AI_CHANLIST_MUX_ENA |
	      II20K_AI_CHANLIST_GAIN(range) |
	      II20K_AI_CHANLIST_CHAN(chan);
	writeb(val, iobase + II20K_AI_CHANLIST_REG);

	/* reset settling time counter and trigger delay counter */
	writeb(0, iobase + II20K_AI_COUNT_RESET_REG);

	/* reset channel scanner */
	writeb(0, iobase + II20K_AI_CHAN_RESET_REG);
}

static int ii20k_ai_insn_read(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_insn *insn,
			      unsigned int *data)
{
	void __iomem *iobase = ii20k_module_iobase(dev, s);
	int ret;
	int i;

	ii20k_ai_setup(dev, s, insn->chanspec);

	for (i = 0; i < insn->n; i++) {
		unsigned int val;

		/* generate a software start convert signal */
		readb(iobase + II20K_AI_PACER_RESET_REG);

		ret = ii20k_ai_wait_eoc(dev, s, 100);
		if (ret)
			return ret;

		val = readb(iobase + II20K_AI_LSB_REG);
		val |= (readb(iobase + II20K_AI_MSB_REG) << 8);

		/* munge two's complement data */
		val += ((s->maxdata + 1) >> 1);
		val &= s->maxdata;

		data[i] = val;
	}

	return insn->n;
}

static void ii20k_dio_config(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	struct ii20k_private *devpriv = dev->private;
	unsigned char ctrl01 = 0;
	unsigned char ctrl23 = 0;
	unsigned char dir_ena = 0;

	/* port 0 - channels 0-7 */
	if (s->io_bits & 0x000000ff) {
		/* output port */
		ctrl01 &= ~II20K_CTRL01_DIO0_IN;
		dir_ena &= ~II20K_BUF_DISAB_DIO0;
		dir_ena |= II20K_DIR_DIO0_OUT;
	} else {
		/* input port */
		ctrl01 |= II20K_CTRL01_DIO0_IN;
		dir_ena &= ~II20K_DIR_DIO0_OUT;
	}

	/* port 1 - channels 8-15 */
	if (s->io_bits & 0x0000ff00) {
		/* output port */
		ctrl01 &= ~II20K_CTRL01_DIO1_IN;
		dir_ena &= ~II20K_BUF_DISAB_DIO1;
		dir_ena |= II20K_DIR_DIO1_OUT;
	} else {
		/* input port */
		ctrl01 |= II20K_CTRL01_DIO1_IN;
		dir_ena &= ~II20K_DIR_DIO1_OUT;
	}

	/* port 2 - channels 16-23 */
	if (s->io_bits & 0x00ff0000) {
		/* output port */
		ctrl23 &= ~II20K_CTRL23_DIO2_IN;
		dir_ena &= ~II20K_BUF_DISAB_DIO2;
		dir_ena |= II20K_DIR_DIO2_OUT;
	} else {
		/* input port */
		ctrl23 |= II20K_CTRL23_DIO2_IN;
		dir_ena &= ~II20K_DIR_DIO2_OUT;
	}

	/* port 3 - channels 24-31 */
	if (s->io_bits & 0xff000000) {
		/* output port */
		ctrl23 &= ~II20K_CTRL23_DIO3_IN;
		dir_ena &= ~II20K_BUF_DISAB_DIO3;
		dir_ena |= II20K_DIR_DIO3_OUT;
	} else {
		/* input port */
		ctrl23 |= II20K_CTRL23_DIO3_IN;
		dir_ena &= ~II20K_DIR_DIO3_OUT;
	}

	ctrl23 |= II20K_CTRL01_SET;
	ctrl23 |= II20K_CTRL23_SET;

	/* order is important */
	writeb(ctrl01, devpriv->ioaddr + II20K_CTRL01_REG);
	writeb(ctrl23, devpriv->ioaddr + II20K_CTRL23_REG);
	writeb(dir_ena, devpriv->ioaddr + II20K_DIR_ENA_REG);
}

static int ii20k_dio_insn_config(struct comedi_device *dev,
				 struct comedi_subdevice *s,
				 struct comedi_insn *insn,
				 unsigned int *data)
{
	unsigned int chan = CR_CHAN(insn->chanspec);
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

	ii20k_dio_config(dev, s);

	return insn->n;
}

static int ii20k_dio_insn_bits(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn,
			       unsigned int *data)
{
	struct ii20k_private *devpriv = dev->private;
	unsigned int mask;

	mask = comedi_dio_update_state(s, data);
	if (mask) {
		if (mask & 0x000000ff)
			writeb((s->state >> 0) & 0xff,
			       devpriv->ioaddr + II20K_DIO0_REG);
		if (mask & 0x0000ff00)
			writeb((s->state >> 8) & 0xff,
			       devpriv->ioaddr + II20K_DIO1_REG);
		if (mask & 0x00ff0000)
			writeb((s->state >> 16) & 0xff,
			       devpriv->ioaddr + II20K_DIO2_REG);
		if (mask & 0xff000000)
			writeb((s->state >> 24) & 0xff,
			       devpriv->ioaddr + II20K_DIO3_REG);
	}

	data[1] = readb(devpriv->ioaddr + II20K_DIO0_REG);
	data[1] |= readb(devpriv->ioaddr + II20K_DIO1_REG) << 8;
	data[1] |= readb(devpriv->ioaddr + II20K_DIO2_REG) << 16;
	data[1] |= readb(devpriv->ioaddr + II20K_DIO3_REG) << 24;

	return insn->n;
}

static int ii20k_init_module(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	struct ii20k_ao_private *ao_spriv;
	void __iomem *iobase = ii20k_module_iobase(dev, s);
	unsigned char id;

	id = readb(iobase + II20K_ID_REG);
	switch (id) {
	case II20K_ID_PCI20006M_1:
	case II20K_ID_PCI20006M_2:
		ao_spriv = comedi_alloc_spriv(s, sizeof(*ao_spriv));
		if (!ao_spriv)
			return -ENOMEM;

		/* Analog Output subdevice */
		s->type		= COMEDI_SUBD_AO;
		s->subdev_flags	= SDF_WRITABLE;
		s->n_chan	= (id == II20K_ID_PCI20006M_2) ? 2 : 1;
		s->maxdata	= 0xffff;
		s->range_table	= &ii20k_ao_ranges;
		s->insn_read	= ii20k_ao_insn_read;
		s->insn_write	= ii20k_ao_insn_write;
		break;
	case II20K_ID_PCI20341M_1:
		/* Analog Input subdevice */
		s->type		= COMEDI_SUBD_AI;
		s->subdev_flags	= SDF_READABLE | SDF_DIFF;
		s->n_chan	= 4;
		s->maxdata	= 0xffff;
		s->range_table	= &ii20k_ai_ranges;
		s->insn_read	= ii20k_ai_insn_read;
		break;
	default:
		s->type = COMEDI_SUBD_UNUSED;
		break;
	}

	return 0;
}

static int ii20k_attach(struct comedi_device *dev,
			struct comedi_devconfig *it)
{
	struct ii20k_private *devpriv;
	struct comedi_subdevice *s;
	unsigned char id;
	bool has_dio;
	int ret;

	devpriv = comedi_alloc_devpriv(dev, sizeof(*devpriv));
	if (!devpriv)
		return -ENOMEM;

	devpriv->ioaddr = (void __iomem *)(unsigned long)it->options[0];

	id = readb(devpriv->ioaddr + II20K_ID_REG);
	switch (id & II20K_ID_MASK) {
	case II20K_ID_PCI20001C_1A:
		break;
	case II20K_ID_PCI20001C_2A:
		has_dio = true;
		break;
	default:
		return -ENODEV;
	}

	ret = comedi_alloc_subdevices(dev, 4);
	if (ret)
		return ret;

	s = &dev->subdevices[0];
	if (id & II20K_ID_MOD1_EMPTY) {
		s->type = COMEDI_SUBD_UNUSED;
	} else {
		ret = ii20k_init_module(dev, s);
		if (ret)
			return ret;
	}

	s = &dev->subdevices[1];
	if (id & II20K_ID_MOD2_EMPTY) {
		s->type = COMEDI_SUBD_UNUSED;
	} else {
		ret = ii20k_init_module(dev, s);
		if (ret)
			return ret;
	}

	s = &dev->subdevices[2];
	if (id & II20K_ID_MOD3_EMPTY) {
		s->type = COMEDI_SUBD_UNUSED;
	} else {
		ret = ii20k_init_module(dev, s);
		if (ret)
			return ret;
	}

	/* Digital I/O subdevice */
	s = &dev->subdevices[3];
	if (has_dio) {
		s->type		= COMEDI_SUBD_DIO;
		s->subdev_flags	= SDF_READABLE | SDF_WRITABLE;
		s->n_chan	= 32;
		s->maxdata	= 1;
		s->range_table	= &range_digital;
		s->insn_bits	= ii20k_dio_insn_bits;
		s->insn_config	= ii20k_dio_insn_config;

		/* default all channels to input */
		ii20k_dio_config(dev, s);
	} else {
		s->type = COMEDI_SUBD_UNUSED;
	}

	return 0;
}

static struct comedi_driver ii20k_driver = {
	.driver_name	= "ii_pci20kc",
	.module		= THIS_MODULE,
	.attach		= ii20k_attach,
	.detach		= comedi_legacy_detach,
};
module_comedi_driver(ii20k_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi low-level driver");
MODULE_LICENSE("GPL");
