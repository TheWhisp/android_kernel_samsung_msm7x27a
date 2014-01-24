/* $Id: teles0.c,v 2.15.2.4 2004/01/13 23:48:39 keil Exp $
 *
 * low level stuff for Teles Memory IO isdn cards
 *
 * Author       Karsten Keil
 *              based on the teles driver from Jan den Ouden
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
 * Thanks to    Jan den Ouden
 *              Fritz Elfert
 *              Beat Doebeli
 *
 */

#include <linux/init.h>
#include "hisax.h"
#include "isdnl1.h"
#include "isac.h"
#include "hscx.h"

static const char *teles0_revision = "$Revision: 2.15.2.4 $";

#define TELES_IOMEM_SIZE	0x400
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
readisac(void __iomem *adr, u_char off)
{
	return readb(adr + ((off & 1) ? 0x2ff : 0x100) + off);
}

static inline void
writeisac(void __iomem *adr, u_char off, u_char data)
{
	writeb(data, adr + ((off & 1) ? 0x2ff : 0x100) + off); mb();
}


static inline u_char
readhscx(void __iomem *adr, int hscx, u_char off)
{
	return readb(adr + (hscx ? 0x1c0 : 0x180) +
		     ((off & 1) ? 0x1ff : 0) + off);
}

static inline void
writehscx(void __iomem *adr, int hscx, u_char off, u_char data)
{
	writeb(data, adr + (hscx ? 0x1c0 : 0x180) +
	       ((off & 1) ? 0x1ff : 0) + off); mb();
}

static inline void
<<<<<<< HEAD
<<<<<<< HEAD
read_fifo_isac(void __iomem *adr, u_char * data, int size)
=======
read_fifo_isac(void __iomem *adr, u_char *data, int size)
>>>>>>> refs/remotes/origin/cm-10.0
=======
read_fifo_isac(void __iomem *adr, u_char *data, int size)
>>>>>>> refs/remotes/origin/master
{
	register int i;
	register u_char __iomem *ad = adr + 0x100;
	for (i = 0; i < size; i++)
		data[i] = readb(ad);
}

static inline void
<<<<<<< HEAD
<<<<<<< HEAD
write_fifo_isac(void __iomem *adr, u_char * data, int size)
=======
write_fifo_isac(void __iomem *adr, u_char *data, int size)
>>>>>>> refs/remotes/origin/cm-10.0
=======
write_fifo_isac(void __iomem *adr, u_char *data, int size)
>>>>>>> refs/remotes/origin/master
{
	register int i;
	register u_char __iomem *ad = adr + 0x100;
	for (i = 0; i < size; i++) {
		writeb(data[i], ad); mb();
	}
}

static inline void
<<<<<<< HEAD
<<<<<<< HEAD
read_fifo_hscx(void __iomem *adr, int hscx, u_char * data, int size)
=======
read_fifo_hscx(void __iomem *adr, int hscx, u_char *data, int size)
>>>>>>> refs/remotes/origin/cm-10.0
=======
read_fifo_hscx(void __iomem *adr, int hscx, u_char *data, int size)
>>>>>>> refs/remotes/origin/master
{
	register int i;
	register u_char __iomem *ad = adr + (hscx ? 0x1c0 : 0x180);
	for (i = 0; i < size; i++)
		data[i] = readb(ad);
}

static inline void
<<<<<<< HEAD
<<<<<<< HEAD
write_fifo_hscx(void __iomem *adr, int hscx, u_char * data, int size)
=======
write_fifo_hscx(void __iomem *adr, int hscx, u_char *data, int size)
>>>>>>> refs/remotes/origin/cm-10.0
=======
write_fifo_hscx(void __iomem *adr, int hscx, u_char *data, int size)
>>>>>>> refs/remotes/origin/master
{
	int i;
	register u_char __iomem *ad = adr + (hscx ? 0x1c0 : 0x180);
	for (i = 0; i < size; i++) {
		writeb(data[i], ad); mb();
	}
}

/* Interface functions */

static u_char
ReadISAC(struct IsdnCardState *cs, u_char offset)
{
	return (readisac(cs->hw.teles0.membase, offset));
}

static void
WriteISAC(struct IsdnCardState *cs, u_char offset, u_char value)
{
	writeisac(cs->hw.teles0.membase, offset, value);
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
	read_fifo_isac(cs->hw.teles0.membase, data, size);
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
	write_fifo_isac(cs->hw.teles0.membase, data, size);
}

static u_char
ReadHSCX(struct IsdnCardState *cs, int hscx, u_char offset)
{
	return (readhscx(cs->hw.teles0.membase, hscx, offset));
}

static void
WriteHSCX(struct IsdnCardState *cs, int hscx, u_char offset, u_char value)
{
	writehscx(cs->hw.teles0.membase, hscx, offset, value);
}

/*
 * fast interrupt HSCX stuff goes here
 */

#define READHSCX(cs, nr, reg) readhscx(cs->hw.teles0.membase, nr, reg)
#define WRITEHSCX(cs, nr, reg, data) writehscx(cs->hw.teles0.membase, nr, reg, data)
#define READHSCXFIFO(cs, nr, ptr, cnt) read_fifo_hscx(cs->hw.teles0.membase, nr, ptr, cnt)
#define WRITEHSCXFIFO(cs, nr, ptr, cnt) write_fifo_hscx(cs->hw.teles0.membase, nr, ptr, cnt)

#include "hscx_irq.c"

static irqreturn_t
teles0_interrupt(int intno, void *dev_id)
{
	struct IsdnCardState *cs = dev_id;
	u_char val;
	u_long flags;
	int count = 0;

	spin_lock_irqsave(&cs->lock, flags);
	val = readhscx(cs->hw.teles0.membase, 1, HSCX_ISTA);
<<<<<<< HEAD
<<<<<<< HEAD
      Start_HSCX:
	if (val)
		hscx_int_main(cs, val);
	val = readisac(cs->hw.teles0.membase, ISAC_ISTA);
      Start_ISAC:
=======
=======
>>>>>>> refs/remotes/origin/master
Start_HSCX:
	if (val)
		hscx_int_main(cs, val);
	val = readisac(cs->hw.teles0.membase, ISAC_ISTA);
Start_ISAC:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (val)
		isac_interrupt(cs, val);
	count++;
	val = readhscx(cs->hw.teles0.membase, 1, HSCX_ISTA);
	if (val && count < 5) {
		if (cs->debug & L1_DEB_HSCX)
			debugl1(cs, "HSCX IntStat after IntRoutine");
		goto Start_HSCX;
	}
	val = readisac(cs->hw.teles0.membase, ISAC_ISTA);
	if (val && count < 5) {
		if (cs->debug & L1_DEB_ISAC)
			debugl1(cs, "ISAC IntStat after IntRoutine");
		goto Start_ISAC;
	}
	writehscx(cs->hw.teles0.membase, 0, HSCX_MASK, 0xFF);
	writehscx(cs->hw.teles0.membase, 1, HSCX_MASK, 0xFF);
	writeisac(cs->hw.teles0.membase, ISAC_MASK, 0xFF);
	writeisac(cs->hw.teles0.membase, ISAC_MASK, 0x0);
	writehscx(cs->hw.teles0.membase, 0, HSCX_MASK, 0x0);
	writehscx(cs->hw.teles0.membase, 1, HSCX_MASK, 0x0);
	spin_unlock_irqrestore(&cs->lock, flags);
	return IRQ_HANDLED;
}

static void
release_io_teles0(struct IsdnCardState *cs)
{
	if (cs->hw.teles0.cfg_reg)
		release_region(cs->hw.teles0.cfg_reg, 8);
	iounmap(cs->hw.teles0.membase);
	release_mem_region(cs->hw.teles0.phymem, TELES_IOMEM_SIZE);
}

static int
reset_teles0(struct IsdnCardState *cs)
{
	u_char cfval;

	if (cs->hw.teles0.cfg_reg) {
		switch (cs->irq) {
<<<<<<< HEAD
<<<<<<< HEAD
			case 2:
			case 9:
				cfval = 0x00;
				break;
			case 3:
				cfval = 0x02;
				break;
			case 4:
				cfval = 0x04;
				break;
			case 5:
				cfval = 0x06;
				break;
			case 10:
				cfval = 0x08;
				break;
			case 11:
				cfval = 0x0A;
				break;
			case 12:
				cfval = 0x0C;
				break;
			case 15:
				cfval = 0x0E;
				break;
			default:
				return(1);
=======
=======
>>>>>>> refs/remotes/origin/master
		case 2:
		case 9:
			cfval = 0x00;
			break;
		case 3:
			cfval = 0x02;
			break;
		case 4:
			cfval = 0x04;
			break;
		case 5:
			cfval = 0x06;
			break;
		case 10:
			cfval = 0x08;
			break;
		case 11:
			cfval = 0x0A;
			break;
		case 12:
			cfval = 0x0C;
			break;
		case 15:
			cfval = 0x0E;
			break;
		default:
			return (1);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
		cfval |= ((cs->hw.teles0.phymem >> 9) & 0xF0);
		byteout(cs->hw.teles0.cfg_reg + 4, cfval);
		HZDELAY(HZ / 10 + 1);
		byteout(cs->hw.teles0.cfg_reg + 4, cfval | 1);
		HZDELAY(HZ / 10 + 1);
	}
	writeb(0, cs->hw.teles0.membase + 0x80); mb();
	HZDELAY(HZ / 5 + 1);
	writeb(1, cs->hw.teles0.membase + 0x80); mb();
	HZDELAY(HZ / 5 + 1);
<<<<<<< HEAD
<<<<<<< HEAD
	return(0);
=======
	return (0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return (0);
>>>>>>> refs/remotes/origin/master
}

static int
Teles_card_msg(struct IsdnCardState *cs, int mt, void *arg)
{
	u_long flags;

	switch (mt) {
<<<<<<< HEAD
<<<<<<< HEAD
		case CARD_RESET:
			spin_lock_irqsave(&cs->lock, flags);
			reset_teles0(cs);
			spin_unlock_irqrestore(&cs->lock, flags);
			return(0);
		case CARD_RELEASE:
			release_io_teles0(cs);
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
		reset_teles0(cs);
		spin_unlock_irqrestore(&cs->lock, flags);
		return (0);
	case CARD_RELEASE:
		release_io_teles0(cs);
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
setup_teles0(struct IsdnCard *card)
=======
}

int setup_teles0(struct IsdnCard *card)
>>>>>>> refs/remotes/origin/master
{
	u_char val;
	struct IsdnCardState *cs = card->cs;
	char tmp[64];

	strcpy(tmp, teles0_revision);
	printk(KERN_INFO "HiSax: Teles 8.0/16.0 driver Rev. %s\n", HiSax_getrev(tmp));
	if ((cs->typ != ISDN_CTYPE_16_0) && (cs->typ != ISDN_CTYPE_8_0))
		return (0);

	if (cs->typ == ISDN_CTYPE_16_0)
		cs->hw.teles0.cfg_reg = card->para[2];
	else			/* 8.0 */
		cs->hw.teles0.cfg_reg = 0;

	if (card->para[1] < 0x10000) {
		card->para[1] <<= 4;
		printk(KERN_INFO
<<<<<<< HEAD
<<<<<<< HEAD
		   "Teles0: membase configured DOSish, assuming 0x%lx\n",
=======
		       "Teles0: membase configured DOSish, assuming 0x%lx\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
		       "Teles0: membase configured DOSish, assuming 0x%lx\n",
>>>>>>> refs/remotes/origin/master
		       (unsigned long) card->para[1]);
	}
	cs->irq = card->para[0];
	if (cs->hw.teles0.cfg_reg) {
		if (!request_region(cs->hw.teles0.cfg_reg, 8, "teles cfg")) {
			printk(KERN_WARNING
<<<<<<< HEAD
<<<<<<< HEAD
			  "HiSax: %s config port %x-%x already in use\n",
=======
			       "HiSax: %s config port %x-%x already in use\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			       "HiSax: %s config port %x-%x already in use\n",
>>>>>>> refs/remotes/origin/master
			       CardType[card->typ],
			       cs->hw.teles0.cfg_reg,
			       cs->hw.teles0.cfg_reg + 8);
			return (0);
		}
	}
	if (cs->hw.teles0.cfg_reg) {
		if ((val = bytein(cs->hw.teles0.cfg_reg + 0)) != 0x51) {
			printk(KERN_WARNING "Teles0: 16.0 Byte at %x is %x\n",
			       cs->hw.teles0.cfg_reg + 0, val);
			release_region(cs->hw.teles0.cfg_reg, 8);
			return (0);
		}
		if ((val = bytein(cs->hw.teles0.cfg_reg + 1)) != 0x93) {
			printk(KERN_WARNING "Teles0: 16.0 Byte at %x is %x\n",
			       cs->hw.teles0.cfg_reg + 1, val);
			release_region(cs->hw.teles0.cfg_reg, 8);
			return (0);
		}
		val = bytein(cs->hw.teles0.cfg_reg + 2);	/* 0x1e=without AB
<<<<<<< HEAD
<<<<<<< HEAD
								   * 0x1f=with AB
								   * 0x1c 16.3 ???
=======
								 * 0x1f=with AB
								 * 0x1c 16.3 ???
>>>>>>> refs/remotes/origin/cm-10.0
=======
								 * 0x1f=with AB
								 * 0x1c 16.3 ???
>>>>>>> refs/remotes/origin/master
								 */
		if (val != 0x1e && val != 0x1f) {
			printk(KERN_WARNING "Teles0: 16.0 Byte at %x is %x\n",
			       cs->hw.teles0.cfg_reg + 2, val);
			release_region(cs->hw.teles0.cfg_reg, 8);
			return (0);
		}
	}
	/* 16.0 and 8.0 designed for IOM1 */
	test_and_set_bit(HW_IOM1, &cs->HW_Flags);
	cs->hw.teles0.phymem = card->para[1];
	if (!request_mem_region(cs->hw.teles0.phymem, TELES_IOMEM_SIZE, "teles iomem")) {
		printk(KERN_WARNING
<<<<<<< HEAD
<<<<<<< HEAD
			"HiSax: %s memory region %lx-%lx already in use\n",
			CardType[card->typ],
			cs->hw.teles0.phymem,
			cs->hw.teles0.phymem + TELES_IOMEM_SIZE);
=======
=======
>>>>>>> refs/remotes/origin/master
		       "HiSax: %s memory region %lx-%lx already in use\n",
		       CardType[card->typ],
		       cs->hw.teles0.phymem,
		       cs->hw.teles0.phymem + TELES_IOMEM_SIZE);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (cs->hw.teles0.cfg_reg)
			release_region(cs->hw.teles0.cfg_reg, 8);
		return (0);
	}
	cs->hw.teles0.membase = ioremap(cs->hw.teles0.phymem, TELES_IOMEM_SIZE);
	printk(KERN_INFO
	       "HiSax: %s config irq:%d mem:%p cfg:0x%X\n",
	       CardType[cs->typ], cs->irq,
	       cs->hw.teles0.membase, cs->hw.teles0.cfg_reg);
	if (reset_teles0(cs)) {
		printk(KERN_WARNING "Teles0: wrong IRQ\n");
		release_io_teles0(cs);
		return (0);
	}
	setup_isac(cs);
	cs->readisac = &ReadISAC;
	cs->writeisac = &WriteISAC;
	cs->readisacfifo = &ReadISACfifo;
	cs->writeisacfifo = &WriteISACfifo;
	cs->BC_Read_Reg = &ReadHSCX;
	cs->BC_Write_Reg = &WriteHSCX;
	cs->BC_Send_Data = &hscx_fill_fifo;
	cs->cardmsg = &Teles_card_msg;
	cs->irq_func = &teles0_interrupt;
	ISACVersion(cs, "Teles0:");
	if (HscxVersion(cs, "Teles0:")) {
		printk(KERN_WARNING
<<<<<<< HEAD
<<<<<<< HEAD
		 "Teles0: wrong HSCX versions check IO/MEM addresses\n");
=======
		       "Teles0: wrong HSCX versions check IO/MEM addresses\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		       "Teles0: wrong HSCX versions check IO/MEM addresses\n");
>>>>>>> refs/remotes/origin/master
		release_io_teles0(cs);
		return (0);
	}
	return (1);
}
