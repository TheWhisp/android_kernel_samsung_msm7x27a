/* $Id: hscx_irq.c,v 1.18.2.3 2004/02/11 13:21:34 keil Exp $
 *
 * low level b-channel stuff for Siemens HSCX
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
 * This is an include file for fast inline IRQ stuff
 *
 */


static inline void
waitforCEC(struct IsdnCardState *cs, int hscx)
{
	int to = 50;

	while ((READHSCX(cs, hscx, HSCX_STAR) & 0x04) && to) {
		udelay(1);
		to--;
	}
	if (!to)
		printk(KERN_WARNING "HiSax: waitforCEC timeout\n");
}


static inline void
waitforXFW(struct IsdnCardState *cs, int hscx)
{
	int to = 50;

	while (((READHSCX(cs, hscx, HSCX_STAR) & 0x44) != 0x40) && to) {
		udelay(1);
		to--;
	}
	if (!to)
		printk(KERN_WARNING "HiSax: waitforXFW timeout\n");
}

static inline void
WriteHSCXCMDR(struct IsdnCardState *cs, int hscx, u_char data)
{
	waitforCEC(cs, hscx);
	WRITEHSCX(cs, hscx, HSCX_CMDR, data);
}



static void
hscx_empty_fifo(struct BCState *bcs, int count)
{
	u_char *ptr;
	struct IsdnCardState *cs = bcs->cs;

	if ((cs->debug & L1_DEB_HSCX) && !(cs->debug & L1_DEB_HSCX_FIFO))
		debugl1(cs, "hscx_empty_fifo");

	if (bcs->hw.hscx.rcvidx + count > HSCX_BUFMAX) {
		if (cs->debug & L1_DEB_WARN)
			debugl1(cs, "hscx_empty_fifo: incoming packet too large");
		WriteHSCXCMDR(cs, bcs->hw.hscx.hscx, 0x80);
		bcs->hw.hscx.rcvidx = 0;
		return;
	}
	ptr = bcs->hw.hscx.rcvbuf + bcs->hw.hscx.rcvidx;
	bcs->hw.hscx.rcvidx += count;
	READHSCXFIFO(cs, bcs->hw.hscx.hscx, ptr, count);
	WriteHSCXCMDR(cs, bcs->hw.hscx.hscx, 0x80);
	if (cs->debug & L1_DEB_HSCX_FIFO) {
		char *t = bcs->blog;

		t += sprintf(t, "hscx_empty_fifo %c cnt %d",
			     bcs->hw.hscx.hscx ? 'B' : 'A', count);
		QuickHex(t, ptr, count);
<<<<<<< HEAD
		debugl1(cs, bcs->blog);
=======
		debugl1(cs, "%s", bcs->blog);
>>>>>>> refs/remotes/origin/master
	}
}

static void
hscx_fill_fifo(struct BCState *bcs)
{
	struct IsdnCardState *cs = bcs->cs;
	int more, count;
<<<<<<< HEAD
<<<<<<< HEAD
	int fifo_size = test_bit(HW_IPAC, &cs->HW_Flags)? 64: 32;
=======
	int fifo_size = test_bit(HW_IPAC, &cs->HW_Flags) ? 64 : 32;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int fifo_size = test_bit(HW_IPAC, &cs->HW_Flags) ? 64 : 32;
>>>>>>> refs/remotes/origin/master
	u_char *ptr;

	if ((cs->debug & L1_DEB_HSCX) && !(cs->debug & L1_DEB_HSCX_FIFO))
		debugl1(cs, "hscx_fill_fifo");

	if (!bcs->tx_skb)
		return;
	if (bcs->tx_skb->len <= 0)
		return;

	more = (bcs->mode == L1_MODE_TRANS) ? 1 : 0;
	if (bcs->tx_skb->len > fifo_size) {
		more = !0;
		count = fifo_size;
	} else
		count = bcs->tx_skb->len;

	waitforXFW(cs, bcs->hw.hscx.hscx);
	ptr = bcs->tx_skb->data;
	skb_pull(bcs->tx_skb, count);
	bcs->tx_cnt -= count;
	bcs->hw.hscx.count += count;
	WRITEHSCXFIFO(cs, bcs->hw.hscx.hscx, ptr, count);
	WriteHSCXCMDR(cs, bcs->hw.hscx.hscx, more ? 0x8 : 0xa);
	if (cs->debug & L1_DEB_HSCX_FIFO) {
		char *t = bcs->blog;

		t += sprintf(t, "hscx_fill_fifo %c cnt %d",
			     bcs->hw.hscx.hscx ? 'B' : 'A', count);
		QuickHex(t, ptr, count);
<<<<<<< HEAD
		debugl1(cs, bcs->blog);
=======
		debugl1(cs, "%s", bcs->blog);
>>>>>>> refs/remotes/origin/master
	}
}

static void
hscx_interrupt(struct IsdnCardState *cs, u_char val, u_char hscx)
{
	u_char r;
	struct BCState *bcs = cs->bcs + hscx;
	struct sk_buff *skb;
<<<<<<< HEAD
<<<<<<< HEAD
	int fifo_size = test_bit(HW_IPAC, &cs->HW_Flags)? 64: 32;
=======
	int fifo_size = test_bit(HW_IPAC, &cs->HW_Flags) ? 64 : 32;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int fifo_size = test_bit(HW_IPAC, &cs->HW_Flags) ? 64 : 32;
>>>>>>> refs/remotes/origin/master
	int count;

	if (!test_bit(BC_FLG_INIT, &bcs->Flag))
		return;

	if (val & 0x80) {	/* RME */
		r = READHSCX(cs, hscx, HSCX_RSTA);
		if ((r & 0xf0) != 0xa0) {
			if (!(r & 0x80)) {
				if (cs->debug & L1_DEB_WARN)
					debugl1(cs, "HSCX invalid frame");
#ifdef ERROR_STATISTIC
				bcs->err_inv++;
#endif
			}
			if ((r & 0x40) && bcs->mode) {
				if (cs->debug & L1_DEB_WARN)
					debugl1(cs, "HSCX RDO mode=%d",
						bcs->mode);
#ifdef ERROR_STATISTIC
				bcs->err_rdo++;
#endif
			}
			if (!(r & 0x20)) {
				if (cs->debug & L1_DEB_WARN)
					debugl1(cs, "HSCX CRC error");
#ifdef ERROR_STATISTIC
				bcs->err_crc++;
#endif
			}
			WriteHSCXCMDR(cs, hscx, 0x80);
		} else {
			count = READHSCX(cs, hscx, HSCX_RBCL) & (
<<<<<<< HEAD
<<<<<<< HEAD
				test_bit(HW_IPAC, &cs->HW_Flags)? 0x3f: 0x1f);
=======
				test_bit(HW_IPAC, &cs->HW_Flags) ? 0x3f : 0x1f);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				test_bit(HW_IPAC, &cs->HW_Flags) ? 0x3f : 0x1f);
>>>>>>> refs/remotes/origin/master
			if (count == 0)
				count = fifo_size;
			hscx_empty_fifo(bcs, count);
			if ((count = bcs->hw.hscx.rcvidx - 1) > 0) {
				if (cs->debug & L1_DEB_HSCX_FIFO)
					debugl1(cs, "HX Frame %d", count);
				if (!(skb = dev_alloc_skb(count)))
					printk(KERN_WARNING "HSCX: receive out of memory\n");
				else {
					memcpy(skb_put(skb, count), bcs->hw.hscx.rcvbuf, count);
					skb_queue_tail(&bcs->rqueue, skb);
				}
			}
		}
		bcs->hw.hscx.rcvidx = 0;
		schedule_event(bcs, B_RCVBUFREADY);
	}
	if (val & 0x40) {	/* RPF */
		hscx_empty_fifo(bcs, fifo_size);
		if (bcs->mode == L1_MODE_TRANS) {
			/* receive audio data */
			if (!(skb = dev_alloc_skb(fifo_size)))
				printk(KERN_WARNING "HiSax: receive out of memory\n");
			else {
				memcpy(skb_put(skb, fifo_size), bcs->hw.hscx.rcvbuf, fifo_size);
				skb_queue_tail(&bcs->rqueue, skb);
			}
			bcs->hw.hscx.rcvidx = 0;
			schedule_event(bcs, B_RCVBUFREADY);
		}
	}
	if (val & 0x10) {	/* XPR */
		if (bcs->tx_skb) {
			if (bcs->tx_skb->len) {
				hscx_fill_fifo(bcs);
				return;
			} else {
<<<<<<< HEAD
<<<<<<< HEAD
				if (test_bit(FLG_LLI_L1WAKEUP,&bcs->st->lli.flag) &&
					(PACKET_NOACK != bcs->tx_skb->pkt_type)) {
=======
				if (test_bit(FLG_LLI_L1WAKEUP, &bcs->st->lli.flag) &&
				    (PACKET_NOACK != bcs->tx_skb->pkt_type)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
				if (test_bit(FLG_LLI_L1WAKEUP, &bcs->st->lli.flag) &&
				    (PACKET_NOACK != bcs->tx_skb->pkt_type)) {
>>>>>>> refs/remotes/origin/master
					u_long	flags;
					spin_lock_irqsave(&bcs->aclock, flags);
					bcs->ackcnt += bcs->hw.hscx.count;
					spin_unlock_irqrestore(&bcs->aclock, flags);
					schedule_event(bcs, B_ACKPENDING);
				}
				dev_kfree_skb_irq(bcs->tx_skb);
<<<<<<< HEAD
<<<<<<< HEAD
				bcs->hw.hscx.count = 0; 
=======
				bcs->hw.hscx.count = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
				bcs->hw.hscx.count = 0;
>>>>>>> refs/remotes/origin/master
				bcs->tx_skb = NULL;
			}
		}
		if ((bcs->tx_skb = skb_dequeue(&bcs->squeue))) {
			bcs->hw.hscx.count = 0;
			test_and_set_bit(BC_FLG_BUSY, &bcs->Flag);
			hscx_fill_fifo(bcs);
		} else {
			test_and_clear_bit(BC_FLG_BUSY, &bcs->Flag);
			schedule_event(bcs, B_XMTBUFREADY);
		}
	}
}

static void
hscx_int_main(struct IsdnCardState *cs, u_char val)
{

	u_char exval;
	struct BCState *bcs;

	if (val & 0x01) {
		bcs = cs->bcs + 1;
		exval = READHSCX(cs, 1, HSCX_EXIR);
		if (exval & 0x40) {
			if (bcs->mode == 1)
				hscx_fill_fifo(bcs);
			else {
#ifdef ERROR_STATISTIC
				bcs->err_tx++;
#endif
				/* Here we lost an TX interrupt, so
<<<<<<< HEAD
<<<<<<< HEAD
				   * restart transmitting the whole frame.
=======
				 * restart transmitting the whole frame.
>>>>>>> refs/remotes/origin/cm-10.0
=======
				 * restart transmitting the whole frame.
>>>>>>> refs/remotes/origin/master
				 */
				if (bcs->tx_skb) {
					skb_push(bcs->tx_skb, bcs->hw.hscx.count);
					bcs->tx_cnt += bcs->hw.hscx.count;
					bcs->hw.hscx.count = 0;
				}
				WriteHSCXCMDR(cs, bcs->hw.hscx.hscx, 0x01);
				if (cs->debug & L1_DEB_WARN)
					debugl1(cs, "HSCX B EXIR %x Lost TX", exval);
			}
		} else if (cs->debug & L1_DEB_HSCX)
			debugl1(cs, "HSCX B EXIR %x", exval);
	}
	if (val & 0xf8) {
		if (cs->debug & L1_DEB_HSCX)
			debugl1(cs, "HSCX B interrupt %x", val);
		hscx_interrupt(cs, val, 1);
	}
	if (val & 0x02) {
		bcs = cs->bcs;
		exval = READHSCX(cs, 0, HSCX_EXIR);
		if (exval & 0x40) {
			if (bcs->mode == L1_MODE_TRANS)
				hscx_fill_fifo(bcs);
			else {
				/* Here we lost an TX interrupt, so
<<<<<<< HEAD
<<<<<<< HEAD
				   * restart transmitting the whole frame.
=======
				 * restart transmitting the whole frame.
>>>>>>> refs/remotes/origin/cm-10.0
=======
				 * restart transmitting the whole frame.
>>>>>>> refs/remotes/origin/master
				 */
#ifdef ERROR_STATISTIC
				bcs->err_tx++;
#endif
				if (bcs->tx_skb) {
					skb_push(bcs->tx_skb, bcs->hw.hscx.count);
					bcs->tx_cnt += bcs->hw.hscx.count;
					bcs->hw.hscx.count = 0;
				}
				WriteHSCXCMDR(cs, bcs->hw.hscx.hscx, 0x01);
				if (cs->debug & L1_DEB_WARN)
					debugl1(cs, "HSCX A EXIR %x Lost TX", exval);
			}
		} else if (cs->debug & L1_DEB_HSCX)
			debugl1(cs, "HSCX A EXIR %x", exval);
	}
	if (val & 0x04) {
		exval = READHSCX(cs, 0, HSCX_ISTA);
		if (cs->debug & L1_DEB_HSCX)
			debugl1(cs, "HSCX A interrupt %x", exval);
		hscx_interrupt(cs, exval, 0);
	}
}
