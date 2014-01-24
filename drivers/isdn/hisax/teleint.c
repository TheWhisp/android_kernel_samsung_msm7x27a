/* $Id: teleint.c,v 1.16.2.5 2004/01/19 15:31:50 keil Exp $
 *
 * low level stuff for TeleInt isdn cards
 *
 * Author       Karsten Keil
 * Copyright    by Karsten Keil      <keil@isdn4linux.de>
<<<<<<< HEAD
<<<<<<< HEAD
 * 
=======
 *
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *
>>>>>>> refs/remotes/origin/master
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#include <linux/init.h>
#include "hisax.h"
#include "isac.h"
#include "hfc_2bs0.h"
#include "isdnl1.h"

static const char *TeleInt_revision = "$Revision: 1.16.2.5 $";

<<<<<<< HEAD
<<<<<<< HEAD
#define byteout(addr,val) outb(val,addr)
=======
#define byteout(addr, val) outb(val, addr)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define byteout(addr, val) outb(val, addr)
>>>>>>> refs/remotes/origin/master
#define bytein(addr) inb(addr)

static inline u_char
readreg(unsigned int ale, unsigned int adr, u_char off)
{
	register u_char ret;
	int max_delay = 2000;

	byteout(ale, off);
	ret = HFC_BUSY & bytein(ale);
	while (ret && --max_delay)
		ret = HFC_BUSY & bytein(ale);
	if (!max_delay) {
		printk(KERN_WARNING "TeleInt Busy not inactive\n");
		return (0);
	}
	ret = bytein(adr);
	return (ret);
}

static inline void
<<<<<<< HEAD
<<<<<<< HEAD
readfifo(unsigned int ale, unsigned int adr, u_char off, u_char * data, int size)
=======
readfifo(unsigned int ale, unsigned int adr, u_char off, u_char *data, int size)
>>>>>>> refs/remotes/origin/cm-10.0
=======
readfifo(unsigned int ale, unsigned int adr, u_char off, u_char *data, int size)
>>>>>>> refs/remotes/origin/master
{
	register u_char ret;
	register int max_delay = 20000;
	register int i;
<<<<<<< HEAD
<<<<<<< HEAD
	
	byteout(ale, off);
	for (i = 0; i<size; i++) {
=======

	byteout(ale, off);
	for (i = 0; i < size; i++) {
>>>>>>> refs/remotes/origin/cm-10.0
=======

	byteout(ale, off);
	for (i = 0; i < size; i++) {
>>>>>>> refs/remotes/origin/master
		ret = HFC_BUSY & bytein(ale);
		while (ret && --max_delay)
			ret = HFC_BUSY & bytein(ale);
		if (!max_delay) {
			printk(KERN_WARNING "TeleInt Busy not inactive\n");
			return;
		}
		data[i] = bytein(adr);
	}
}


static inline void
writereg(unsigned int ale, unsigned int adr, u_char off, u_char data)
{
	register u_char ret;
	int max_delay = 2000;

	byteout(ale, off);
	ret = HFC_BUSY & bytein(ale);
	while (ret && --max_delay)
		ret = HFC_BUSY & bytein(ale);
	if (!max_delay) {
		printk(KERN_WARNING "TeleInt Busy not inactive\n");
		return;
	}
	byteout(adr, data);
}

static inline void
<<<<<<< HEAD
<<<<<<< HEAD
writefifo(unsigned int ale, unsigned int adr, u_char off, u_char * data, int size)
=======
writefifo(unsigned int ale, unsigned int adr, u_char off, u_char *data, int size)
>>>>>>> refs/remotes/origin/cm-10.0
=======
writefifo(unsigned int ale, unsigned int adr, u_char off, u_char *data, int size)
>>>>>>> refs/remotes/origin/master
{
	register u_char ret;
	register int max_delay = 20000;
	register int i;
<<<<<<< HEAD
<<<<<<< HEAD
	
	byteout(ale, off);
	for (i = 0; i<size; i++) {
=======

	byteout(ale, off);
	for (i = 0; i < size; i++) {
>>>>>>> refs/remotes/origin/cm-10.0
=======

	byteout(ale, off);
	for (i = 0; i < size; i++) {
>>>>>>> refs/remotes/origin/master
		ret = HFC_BUSY & bytein(ale);
		while (ret && --max_delay)
			ret = HFC_BUSY & bytein(ale);
		if (!max_delay) {
			printk(KERN_WARNING "TeleInt Busy not inactive\n");
			return;
		}
		byteout(adr, data[i]);
	}
}

/* Interface functions */

static u_char
ReadISAC(struct IsdnCardState *cs, u_char offset)
{
	cs->hw.hfc.cip = offset;
	return (readreg(cs->hw.hfc.addr | 1, cs->hw.hfc.addr, offset));
}

static void
WriteISAC(struct IsdnCardState *cs, u_char offset, u_char value)
{
	cs->hw.hfc.cip = offset;
	writereg(cs->hw.hfc.addr | 1, cs->hw.hfc.addr, offset, value);
}

static void
<<<<<<< HEAD
<<<<<<< HEAD
ReadISACfifo(struct IsdnCardState *cs, u_char * data, int size)
=======
ReadISACfifo(struct IsdnCardState *cs, u_char *data, int size)
>>>>>>> refs/remotes/origin/cm-10.0
=======
ReadISACfifo(struct IsdnCardState *cs, u_char *data, int size)
>>>>>>> refs/remotes/origin/master
{
	cs->hw.hfc.cip = 0;
	readfifo(cs->hw.hfc.addr | 1, cs->hw.hfc.addr, 0, data, size);
}

static void
<<<<<<< HEAD
<<<<<<< HEAD
WriteISACfifo(struct IsdnCardState *cs, u_char * data, int size)
=======
WriteISACfifo(struct IsdnCardState *cs, u_char *data, int size)
>>>>>>> refs/remotes/origin/cm-10.0
=======
WriteISACfifo(struct IsdnCardState *cs, u_char *data, int size)
>>>>>>> refs/remotes/origin/master
{
	cs->hw.hfc.cip = 0;
	writefifo(cs->hw.hfc.addr | 1, cs->hw.hfc.addr, 0, data, size);
}

static u_char
ReadHFC(struct IsdnCardState *cs, int data, u_char reg)
{
	register u_char ret;

	if (data) {
		cs->hw.hfc.cip = reg;
		byteout(cs->hw.hfc.addr | 1, reg);
		ret = bytein(cs->hw.hfc.addr);
		if (cs->debug & L1_DEB_HSCX_FIFO && (data != 2))
			debugl1(cs, "hfc RD %02x %02x", reg, ret);
	} else
		ret = bytein(cs->hw.hfc.addr | 1);
	return (ret);
}

static void
WriteHFC(struct IsdnCardState *cs, int data, u_char reg, u_char value)
{
	byteout(cs->hw.hfc.addr | 1, reg);
	cs->hw.hfc.cip = reg;
	if (data)
		byteout(cs->hw.hfc.addr, value);
	if (cs->debug & L1_DEB_HSCX_FIFO && (data != 2))
		debugl1(cs, "hfc W%c %02x %02x", data ? 'D' : 'C', reg, value);
}

static irqreturn_t
TeleInt_interrupt(int intno, void *dev_id)
{
	struct IsdnCardState *cs = dev_id;
	u_char val;
	u_long flags;

	spin_lock_irqsave(&cs->lock, flags);
	val = readreg(cs->hw.hfc.addr | 1, cs->hw.hfc.addr, ISAC_ISTA);
<<<<<<< HEAD
<<<<<<< HEAD
      Start_ISAC:
=======
Start_ISAC:
>>>>>>> refs/remotes/origin/cm-10.0
=======
Start_ISAC:
>>>>>>> refs/remotes/origin/master
	if (val)
		isac_interrupt(cs, val);
	val = readreg(cs->hw.hfc.addr | 1, cs->hw.hfc.addr, ISAC_ISTA);
	if (val) {
		if (cs->debug & L1_DEB_ISAC)
			debugl1(cs, "ISAC IntStat after IntRoutine");
		goto Start_ISAC;
	}
	writereg(cs->hw.hfc.addr | 1, cs->hw.hfc.addr, ISAC_MASK, 0xFF);
	writereg(cs->hw.hfc.addr | 1, cs->hw.hfc.addr, ISAC_MASK, 0x0);
	spin_unlock_irqrestore(&cs->lock, flags);
	return IRQ_HANDLED;
}

static void
TeleInt_Timer(struct IsdnCardState *cs)
{
	int stat = 0;
	u_long flags;
<<<<<<< HEAD
<<<<<<< HEAD
	
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
	spin_lock_irqsave(&cs->lock, flags);
	if (cs->bcs[0].mode) {
		stat |= 1;
		main_irq_hfc(&cs->bcs[0]);
	}
	if (cs->bcs[1].mode) {
		stat |= 2;
		main_irq_hfc(&cs->bcs[1]);
	}
	spin_unlock_irqrestore(&cs->lock, flags);
<<<<<<< HEAD
<<<<<<< HEAD
	stat = HZ/100;
=======
	stat = HZ / 100;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	stat = HZ / 100;
>>>>>>> refs/remotes/origin/master
	if (!stat)
		stat = 1;
	cs->hw.hfc.timer.expires = jiffies + stat;
	add_timer(&cs->hw.hfc.timer);
}

static void
release_io_TeleInt(struct IsdnCardState *cs)
{
	del_timer(&cs->hw.hfc.timer);
	releasehfc(cs);
	if (cs->hw.hfc.addr)
		release_region(cs->hw.hfc.addr, 2);
}

static void
reset_TeleInt(struct IsdnCardState *cs)
{
	printk(KERN_INFO "TeleInt: resetting card\n");
	cs->hw.hfc.cirm |= HFC_RESET;
	byteout(cs->hw.hfc.addr | 1, cs->hw.hfc.cirm);	/* Reset On */
	mdelay(10);
	cs->hw.hfc.cirm &= ~HFC_RESET;
	byteout(cs->hw.hfc.addr | 1, cs->hw.hfc.cirm);	/* Reset Off */
	mdelay(10);
}

static int
TeleInt_card_msg(struct IsdnCardState *cs, int mt, void *arg)
{
	u_long flags;
	int delay;

	switch (mt) {
<<<<<<< HEAD
<<<<<<< HEAD
		case CARD_RESET:
			spin_lock_irqsave(&cs->lock, flags);
			reset_TeleInt(cs);
			spin_unlock_irqrestore(&cs->lock, flags);
			return(0);
		case CARD_RELEASE:
			release_io_TeleInt(cs);
			return(0);
		case CARD_INIT:
			spin_lock_irqsave(&cs->lock, flags);
			reset_TeleInt(cs);
			inithfc(cs);
			clear_pending_isac_ints(cs);
			initisac(cs);
			/* Reenable all IRQ */
			cs->writeisac(cs, ISAC_MASK, 0);
			cs->writeisac(cs, ISAC_CMDR, 0x41);
			spin_unlock_irqrestore(&cs->lock, flags);
			delay = HZ/100;
			if (!delay)
				delay = 1;
			cs->hw.hfc.timer.expires = jiffies + delay;
			add_timer(&cs->hw.hfc.timer);
			return(0);
		case CARD_TEST:
			return(0);
	}
	return(0);
=======
=======
>>>>>>> refs/remotes/origin/master
	case CARD_RESET:
		spin_lock_irqsave(&cs->lock, flags);
		reset_TeleInt(cs);
		spin_unlock_irqrestore(&cs->lock, flags);
		return (0);
	case CARD_RELEASE:
		release_io_TeleInt(cs);
		return (0);
	case CARD_INIT:
		spin_lock_irqsave(&cs->lock, flags);
		reset_TeleInt(cs);
		inithfc(cs);
		clear_pending_isac_ints(cs);
		initisac(cs);
		/* Reenable all IRQ */
		cs->writeisac(cs, ISAC_MASK, 0);
		cs->writeisac(cs, ISAC_CMDR, 0x41);
		spin_unlock_irqrestore(&cs->lock, flags);
		delay = HZ / 100;
		if (!delay)
			delay = 1;
		cs->hw.hfc.timer.expires = jiffies + delay;
		add_timer(&cs->hw.hfc.timer);
		return (0);
	case CARD_TEST:
		return (0);
	}
	return (0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

int __devinit
setup_TeleInt(struct IsdnCard *card)
=======
}

int setup_TeleInt(struct IsdnCard *card)
>>>>>>> refs/remotes/origin/master
{
	struct IsdnCardState *cs = card->cs;
	char tmp[64];

	strcpy(tmp, TeleInt_revision);
	printk(KERN_INFO "HiSax: TeleInt driver Rev. %s\n", HiSax_getrev(tmp));
	if (cs->typ != ISDN_CTYPE_TELEINT)
		return (0);

	cs->hw.hfc.addr = card->para[1] & 0x3fe;
	cs->irq = card->para[0];
	cs->hw.hfc.cirm = HFC_CIRM;
	cs->hw.hfc.isac_spcr = 0x00;
	cs->hw.hfc.cip = 0;
	cs->hw.hfc.ctmt = HFC_CTMT | HFC_CLTIMER;
	cs->bcs[0].hw.hfc.send = NULL;
	cs->bcs[1].hw.hfc.send = NULL;
	cs->hw.hfc.fifosize = 7 * 1024 + 512;
	cs->hw.hfc.timer.function = (void *) TeleInt_Timer;
	cs->hw.hfc.timer.data = (long) cs;
	init_timer(&cs->hw.hfc.timer);
	if (!request_region(cs->hw.hfc.addr, 2, "TeleInt isdn")) {
		printk(KERN_WARNING
		       "HiSax: TeleInt config port %x-%x already in use\n",
		       cs->hw.hfc.addr,
		       cs->hw.hfc.addr + 2);
		return (0);
	}
	/* HW IO = IO */
	byteout(cs->hw.hfc.addr, cs->hw.hfc.addr & 0xff);
	byteout(cs->hw.hfc.addr | 1, ((cs->hw.hfc.addr & 0x300) >> 8) | 0x54);
	switch (cs->irq) {
<<<<<<< HEAD
<<<<<<< HEAD
		case 3:
			cs->hw.hfc.cirm |= HFC_INTA;
			break;
		case 4:
			cs->hw.hfc.cirm |= HFC_INTB;
			break;
		case 5:
			cs->hw.hfc.cirm |= HFC_INTC;
			break;
		case 7:
			cs->hw.hfc.cirm |= HFC_INTD;
			break;
		case 10:
			cs->hw.hfc.cirm |= HFC_INTE;
			break;
		case 11:
			cs->hw.hfc.cirm |= HFC_INTF;
			break;
		default:
			printk(KERN_WARNING "TeleInt: wrong IRQ\n");
			release_io_TeleInt(cs);
			return (0);
=======
=======
>>>>>>> refs/remotes/origin/master
	case 3:
		cs->hw.hfc.cirm |= HFC_INTA;
		break;
	case 4:
		cs->hw.hfc.cirm |= HFC_INTB;
		break;
	case 5:
		cs->hw.hfc.cirm |= HFC_INTC;
		break;
	case 7:
		cs->hw.hfc.cirm |= HFC_INTD;
		break;
	case 10:
		cs->hw.hfc.cirm |= HFC_INTE;
		break;
	case 11:
		cs->hw.hfc.cirm |= HFC_INTF;
		break;
	default:
		printk(KERN_WARNING "TeleInt: wrong IRQ\n");
		release_io_TeleInt(cs);
		return (0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	byteout(cs->hw.hfc.addr | 1, cs->hw.hfc.cirm);
	byteout(cs->hw.hfc.addr | 1, cs->hw.hfc.ctmt);

	printk(KERN_INFO "TeleInt: defined at 0x%x IRQ %d\n",
<<<<<<< HEAD
<<<<<<< HEAD
		cs->hw.hfc.addr, cs->irq);
=======
	       cs->hw.hfc.addr, cs->irq);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	       cs->hw.hfc.addr, cs->irq);
>>>>>>> refs/remotes/origin/master

	setup_isac(cs);
	cs->readisac = &ReadISAC;
	cs->writeisac = &WriteISAC;
	cs->readisacfifo = &ReadISACfifo;
	cs->writeisacfifo = &WriteISACfifo;
	cs->BC_Read_Reg = &ReadHFC;
	cs->BC_Write_Reg = &WriteHFC;
	cs->cardmsg = &TeleInt_card_msg;
	cs->irq_func = &TeleInt_interrupt;
	ISACVersion(cs, "TeleInt:");
	return (1);
}
