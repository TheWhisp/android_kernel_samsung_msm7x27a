/* $Id: saphir.c,v 1.10.2.4 2004/01/13 23:48:39 keil Exp $
 *
 * low level stuff for HST Saphir 1
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
 * Thanks to    HST High Soft Tech GmbH
 *
 */

#include <linux/init.h>
#include "hisax.h"
#include "isac.h"
#include "hscx.h"
#include "isdnl1.h"

static char *saphir_rev = "$Revision: 1.10.2.4 $";

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

#define ISAC_DATA	0
#define HSCX_DATA	1
#define ADDRESS_REG	2
#define IRQ_REG		3
#define SPARE_REG	4
#define RESET_REG	5

static inline u_char
readreg(unsigned int ale, unsigned int adr, u_char off)
{
	register u_char ret;

	byteout(ale, off);
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
	byteout(ale, off);
	insb(adr, data, size);
}


static inline void
writereg(unsigned int ale, unsigned int adr, u_char off, u_char data)
{
	byteout(ale, off);
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
	byteout(ale, off);
	outsb(adr, data, size);
}

/* Interface functions */

static u_char
ReadISAC(struct IsdnCardState *cs, u_char offset)
{
	return (readreg(cs->hw.saphir.ale, cs->hw.saphir.isac, offset));
}

static void
WriteISAC(struct IsdnCardState *cs, u_char offset, u_char value)
{
	writereg(cs->hw.saphir.ale, cs->hw.saphir.isac, offset, value);
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
	readfifo(cs->hw.saphir.ale, cs->hw.saphir.isac, 0, data, size);
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
	writefifo(cs->hw.saphir.ale, cs->hw.saphir.isac, 0, data, size);
}

static u_char
ReadHSCX(struct IsdnCardState *cs, int hscx, u_char offset)
{
	return (readreg(cs->hw.saphir.ale, cs->hw.saphir.hscx,
<<<<<<< HEAD
<<<<<<< HEAD
		offset + (hscx ? 0x40 : 0)));
=======
			offset + (hscx ? 0x40 : 0)));
>>>>>>> refs/remotes/origin/cm-10.0
=======
			offset + (hscx ? 0x40 : 0)));
>>>>>>> refs/remotes/origin/master
}

static void
WriteHSCX(struct IsdnCardState *cs, int hscx, u_char offset, u_char value)
{
	writereg(cs->hw.saphir.ale, cs->hw.saphir.hscx,
<<<<<<< HEAD
<<<<<<< HEAD
		offset + (hscx ? 0x40 : 0), value);
}

#define READHSCX(cs, nr, reg) readreg(cs->hw.saphir.ale, \
		cs->hw.saphir.hscx, reg + (nr ? 0x40 : 0))
#define WRITEHSCX(cs, nr, reg, data) writereg(cs->hw.saphir.ale, \
		cs->hw.saphir.hscx, reg + (nr ? 0x40 : 0), data)

#define READHSCXFIFO(cs, nr, ptr, cnt) readfifo(cs->hw.saphir.ale, \
		cs->hw.saphir.hscx, (nr ? 0x40 : 0), ptr, cnt)

#define WRITEHSCXFIFO(cs, nr, ptr, cnt) writefifo(cs->hw.saphir.ale, \
		cs->hw.saphir.hscx, (nr ? 0x40 : 0), ptr, cnt)
=======
=======
>>>>>>> refs/remotes/origin/master
		 offset + (hscx ? 0x40 : 0), value);
}

#define READHSCX(cs, nr, reg) readreg(cs->hw.saphir.ale,		\
				      cs->hw.saphir.hscx, reg + (nr ? 0x40 : 0))
#define WRITEHSCX(cs, nr, reg, data) writereg(cs->hw.saphir.ale,	\
					      cs->hw.saphir.hscx, reg + (nr ? 0x40 : 0), data)

#define READHSCXFIFO(cs, nr, ptr, cnt) readfifo(cs->hw.saphir.ale,	\
						cs->hw.saphir.hscx, (nr ? 0x40 : 0), ptr, cnt)

#define WRITEHSCXFIFO(cs, nr, ptr, cnt) writefifo(cs->hw.saphir.ale,	\
						  cs->hw.saphir.hscx, (nr ? 0x40 : 0), ptr, cnt)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#include "hscx_irq.c"

static irqreturn_t
saphir_interrupt(int intno, void *dev_id)
{
	struct IsdnCardState *cs = dev_id;
	u_char val;
	u_long flags;

	spin_lock_irqsave(&cs->lock, flags);
	val = readreg(cs->hw.saphir.ale, cs->hw.saphir.hscx, HSCX_ISTA + 0x40);
<<<<<<< HEAD
<<<<<<< HEAD
      Start_HSCX:
	if (val)
		hscx_int_main(cs, val);
	val = readreg(cs->hw.saphir.ale, cs->hw.saphir.isac, ISAC_ISTA);
      Start_ISAC:
=======
=======
>>>>>>> refs/remotes/origin/master
Start_HSCX:
	if (val)
		hscx_int_main(cs, val);
	val = readreg(cs->hw.saphir.ale, cs->hw.saphir.isac, ISAC_ISTA);
Start_ISAC:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (val)
		isac_interrupt(cs, val);
	val = readreg(cs->hw.saphir.ale, cs->hw.saphir.hscx, HSCX_ISTA + 0x40);
	if (val) {
		if (cs->debug & L1_DEB_HSCX)
			debugl1(cs, "HSCX IntStat after IntRoutine");
		goto Start_HSCX;
	}
	val = readreg(cs->hw.saphir.ale, cs->hw.saphir.isac, ISAC_ISTA);
	if (val) {
		if (cs->debug & L1_DEB_ISAC)
			debugl1(cs, "ISAC IntStat after IntRoutine");
		goto Start_ISAC;
	}
	/* Watchdog */
<<<<<<< HEAD
<<<<<<< HEAD
	if (cs->hw.saphir.timer.function) 
		mod_timer(&cs->hw.saphir.timer, jiffies+1*HZ);
=======
	if (cs->hw.saphir.timer.function)
		mod_timer(&cs->hw.saphir.timer, jiffies + 1 * HZ);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (cs->hw.saphir.timer.function)
		mod_timer(&cs->hw.saphir.timer, jiffies + 1 * HZ);
>>>>>>> refs/remotes/origin/master
	else
		printk(KERN_WARNING "saphir: Spurious timer!\n");
	writereg(cs->hw.saphir.ale, cs->hw.saphir.hscx, HSCX_MASK, 0xFF);
	writereg(cs->hw.saphir.ale, cs->hw.saphir.hscx, HSCX_MASK + 0x40, 0xFF);
	writereg(cs->hw.saphir.ale, cs->hw.saphir.isac, ISAC_MASK, 0xFF);
	writereg(cs->hw.saphir.ale, cs->hw.saphir.isac, ISAC_MASK, 0);
	writereg(cs->hw.saphir.ale, cs->hw.saphir.hscx, HSCX_MASK, 0);
	writereg(cs->hw.saphir.ale, cs->hw.saphir.hscx, HSCX_MASK + 0x40, 0);
	spin_unlock_irqrestore(&cs->lock, flags);
	return IRQ_HANDLED;
}

static void
SaphirWatchDog(struct IsdnCardState *cs)
{
	u_long flags;

	spin_lock_irqsave(&cs->lock, flags);
<<<<<<< HEAD
<<<<<<< HEAD
        /* 5 sec WatchDog, so read at least every 4 sec */
	cs->readisac(cs, ISAC_RBCH);
	spin_unlock_irqrestore(&cs->lock, flags);
	mod_timer(&cs->hw.saphir.timer, jiffies+1*HZ);
=======
=======
>>>>>>> refs/remotes/origin/master
	/* 5 sec WatchDog, so read at least every 4 sec */
	cs->readisac(cs, ISAC_RBCH);
	spin_unlock_irqrestore(&cs->lock, flags);
	mod_timer(&cs->hw.saphir.timer, jiffies + 1 * HZ);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void
release_io_saphir(struct IsdnCardState *cs)
{
	byteout(cs->hw.saphir.cfg_reg + IRQ_REG, 0xff);
	del_timer(&cs->hw.saphir.timer);
	cs->hw.saphir.timer.function = NULL;
	if (cs->hw.saphir.cfg_reg)
		release_region(cs->hw.saphir.cfg_reg, 6);
}

static int
saphir_reset(struct IsdnCardState *cs)
{
	u_char irq_val;

<<<<<<< HEAD
<<<<<<< HEAD
	switch(cs->irq) {
		case 5: irq_val = 0;
			break;
		case 3: irq_val = 1;
			break;
		case 11:
			irq_val = 2;
			break;
		case 12:
			irq_val = 3;
			break;
		case 15:
			irq_val = 4;
			break;
		default:
			printk(KERN_WARNING "HiSax: saphir wrong IRQ %d\n",
				cs->irq);
			return (1);
=======
=======
>>>>>>> refs/remotes/origin/master
	switch (cs->irq) {
	case 5: irq_val = 0;
		break;
	case 3: irq_val = 1;
		break;
	case 11:
		irq_val = 2;
		break;
	case 12:
		irq_val = 3;
		break;
	case 15:
		irq_val = 4;
		break;
	default:
		printk(KERN_WARNING "HiSax: saphir wrong IRQ %d\n",
		       cs->irq);
		return (1);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	byteout(cs->hw.saphir.cfg_reg + IRQ_REG, irq_val);
	byteout(cs->hw.saphir.cfg_reg + RESET_REG, 1);
	mdelay(10);
	byteout(cs->hw.saphir.cfg_reg + RESET_REG, 0);
	mdelay(10);
	byteout(cs->hw.saphir.cfg_reg + IRQ_REG, irq_val);
	byteout(cs->hw.saphir.cfg_reg + SPARE_REG, 0x02);
	return (0);
}

static int
saphir_card_msg(struct IsdnCardState *cs, int mt, void *arg)
{
	u_long flags;

	switch (mt) {
<<<<<<< HEAD
<<<<<<< HEAD
		case CARD_RESET:
			spin_lock_irqsave(&cs->lock, flags);
			saphir_reset(cs);
			spin_unlock_irqrestore(&cs->lock, flags);
			return(0);
		case CARD_RELEASE:
			release_io_saphir(cs);
			return(0);
		case CARD_INIT:
			spin_lock_irqsave(&cs->lock, flags);
			inithscxisac(cs, 3);
			spin_unlock_irqrestore(&cs->lock, flags);
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
		saphir_reset(cs);
		spin_unlock_irqrestore(&cs->lock, flags);
		return (0);
	case CARD_RELEASE:
		release_io_saphir(cs);
		return (0);
	case CARD_INIT:
		spin_lock_irqsave(&cs->lock, flags);
		inithscxisac(cs, 3);
		spin_unlock_irqrestore(&cs->lock, flags);
		return (0);
	case CARD_TEST:
		return (0);
	}
	return (0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}


int __devinit
setup_saphir(struct IsdnCard *card)
=======
}


int setup_saphir(struct IsdnCard *card)
>>>>>>> refs/remotes/origin/master
{
	struct IsdnCardState *cs = card->cs;
	char tmp[64];

	strcpy(tmp, saphir_rev);
	printk(KERN_INFO "HiSax: HST Saphir driver Rev. %s\n", HiSax_getrev(tmp));
	if (cs->typ != ISDN_CTYPE_HSTSAPHIR)
		return (0);

	/* IO-Ports */
	cs->hw.saphir.cfg_reg = card->para[1];
	cs->hw.saphir.isac = card->para[1] + ISAC_DATA;
	cs->hw.saphir.hscx = card->para[1] + HSCX_DATA;
	cs->hw.saphir.ale = card->para[1] + ADDRESS_REG;
	cs->irq = card->para[0];
	if (!request_region(cs->hw.saphir.cfg_reg, 6, "saphir")) {
		printk(KERN_WARNING
<<<<<<< HEAD
<<<<<<< HEAD
			"HiSax: HST Saphir config port %x-%x already in use\n",
			cs->hw.saphir.cfg_reg,
			cs->hw.saphir.cfg_reg + 5);
=======
		       "HiSax: HST Saphir config port %x-%x already in use\n",
		       cs->hw.saphir.cfg_reg,
		       cs->hw.saphir.cfg_reg + 5);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		       "HiSax: HST Saphir config port %x-%x already in use\n",
		       cs->hw.saphir.cfg_reg,
		       cs->hw.saphir.cfg_reg + 5);
>>>>>>> refs/remotes/origin/master
		return (0);
	}

	printk(KERN_INFO "HiSax: HST Saphir config irq:%d io:0x%X\n",
	       cs->irq, cs->hw.saphir.cfg_reg);

	setup_isac(cs);
	cs->hw.saphir.timer.function = (void *) SaphirWatchDog;
	cs->hw.saphir.timer.data = (long) cs;
	init_timer(&cs->hw.saphir.timer);
<<<<<<< HEAD
<<<<<<< HEAD
	cs->hw.saphir.timer.expires = jiffies + 4*HZ;
=======
	cs->hw.saphir.timer.expires = jiffies + 4 * HZ;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cs->hw.saphir.timer.expires = jiffies + 4 * HZ;
>>>>>>> refs/remotes/origin/master
	add_timer(&cs->hw.saphir.timer);
	if (saphir_reset(cs)) {
		release_io_saphir(cs);
		return (0);
	}
	cs->readisac = &ReadISAC;
	cs->writeisac = &WriteISAC;
	cs->readisacfifo = &ReadISACfifo;
	cs->writeisacfifo = &WriteISACfifo;
	cs->BC_Read_Reg = &ReadHSCX;
	cs->BC_Write_Reg = &WriteHSCX;
	cs->BC_Send_Data = &hscx_fill_fifo;
	cs->cardmsg = &saphir_card_msg;
	cs->irq_func = &saphir_interrupt;
	ISACVersion(cs, "saphir:");
	if (HscxVersion(cs, "saphir:")) {
		printk(KERN_WARNING
<<<<<<< HEAD
<<<<<<< HEAD
		    "saphir: wrong HSCX versions check IO address\n");
=======
		       "saphir: wrong HSCX versions check IO address\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		       "saphir: wrong HSCX versions check IO address\n");
>>>>>>> refs/remotes/origin/master
		release_io_saphir(cs);
		return (0);
	}
	return (1);
}
