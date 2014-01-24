/*
 *	Apple Peripheral System Controller (PSC)
 *
 *	The PSC is used on the AV Macs to control IO functions not handled
 *	by the VIAs (Ethernet, DSP, SCC).
 *
 * TO DO:
 *
 * Try to figure out what's going on in pIFR5 and pIFR6. There seem to be
 * persisant interrupt conditions in those registers and I have no idea what
 * they are. Granted it doesn't affect since we're not enabling any interrupts
 * on those levels at the moment, but it would be nice to know. I have a feeling
 * they aren't actually interrupt lines but data lines (to the DSP?)
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/init.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/irq.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <asm/traps.h>
#include <asm/bootinfo.h>
=======
#include <linux/irq.h>

#include <asm/traps.h>
>>>>>>> refs/remotes/origin/master
#include <asm/macintosh.h>
#include <asm/macints.h>
#include <asm/mac_psc.h>

#define DEBUG_PSC

int psc_present;
volatile __u8 *psc;

<<<<<<< HEAD
<<<<<<< HEAD
irqreturn_t psc_irq(int, void *);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Debugging dump, used in various places to see what's going on.
 */

static void psc_debug_dump(void)
{
	int	i;

	if (!psc_present) return;
	for (i = 0x30 ; i < 0x70 ; i += 0x10) {
		printk("PSC #%d:  IFR = 0x%02X IER = 0x%02X\n",
			i >> 4,
			(int) psc_read_byte(pIFRbase + i),
			(int) psc_read_byte(pIERbase + i));
	}
}

/*
 * Try to kill all DMA channels on the PSC. Not sure how this his
 * supposed to work; this is code lifted from macmace.c and then
 * expanded to cover what I think are the other 7 channels.
 */

<<<<<<< HEAD
static void psc_dma_die_die_die(void)
=======
static __init void psc_dma_die_die_die(void)
>>>>>>> refs/remotes/origin/master
{
	int i;

	printk("Killing all PSC DMA channels...");
	for (i = 0 ; i < 9 ; i++) {
		psc_write_word(PSC_CTL_BASE + (i << 4), 0x8800);
		psc_write_word(PSC_CTL_BASE + (i << 4), 0x1000);
		psc_write_word(PSC_CMD_BASE + (i << 5), 0x1100);
		psc_write_word(PSC_CMD_BASE + (i << 5) + 0x10, 0x1100);
	}
	printk("done!\n");
}

/*
 * Initialize the PSC. For now this just involves shutting down all
 * interrupt sources using the IERs.
 */

void __init psc_init(void)
{
	int i;

	if (macintosh_config->ident != MAC_MODEL_C660
	 && macintosh_config->ident != MAC_MODEL_Q840)
	{
		psc = NULL;
		psc_present = 0;
		return;
	}

	/*
	 * The PSC is always at the same spot, but using psc
	 * keeps things consistent with the psc_xxxx functions.
	 */

	psc = (void *) PSC_BASE;
	psc_present = 1;

	printk("PSC detected at %p\n", psc);

	psc_dma_die_die_die();

#ifdef DEBUG_PSC
	psc_debug_dump();
#endif
	/*
	 * Mask and clear all possible interrupts
	 */

	for (i = 0x30 ; i < 0x70 ; i += 0x10) {
		psc_write_byte(pIERbase + i, 0x0F);
		psc_write_byte(pIFRbase + i, 0x0F);
	}
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Register the PSC interrupt dispatchers for autovector interrupts 3-6.
 */

void __init psc_register_interrupts(void)
{
	if (request_irq(IRQ_AUTO_3, psc_irq, 0, "psc3", (void *) 0x30))
		pr_err("Couldn't register psc%d interrupt\n", 3);
	if (request_irq(IRQ_AUTO_4, psc_irq, 0, "psc4", (void *) 0x40))
		pr_err("Couldn't register psc%d interrupt\n", 4);
	if (request_irq(IRQ_AUTO_5, psc_irq, 0, "psc5", (void *) 0x50))
		pr_err("Couldn't register psc%d interrupt\n", 5);
	if (request_irq(IRQ_AUTO_6, psc_irq, 0, "psc6", (void *) 0x60))
		pr_err("Couldn't register psc%d interrupt\n", 6);
}

/*
 * PSC interrupt handler. It's a lot like the VIA interrupt handler.
 */

irqreturn_t psc_irq(int irq, void *dev_id)
{
	int pIFR	= pIFRbase + ((int) dev_id);
	int pIER	= pIERbase + ((int) dev_id);
=======
=======
>>>>>>> refs/remotes/origin/master
 * PSC interrupt handler. It's a lot like the VIA interrupt handler.
 */

static void psc_irq(unsigned int irq, struct irq_desc *desc)
{
	unsigned int offset = (unsigned int)irq_desc_get_handler_data(desc);
	int pIFR	= pIFRbase + offset;
	int pIER	= pIERbase + offset;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	int irq_num;
	unsigned char irq_bit, events;

#ifdef DEBUG_IRQS
<<<<<<< HEAD
<<<<<<< HEAD
	printk("psc_irq: irq %d pIFR = 0x%02X pIER = 0x%02X\n",
=======
	printk("psc_irq: irq %u pIFR = 0x%02X pIER = 0x%02X\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
	printk("psc_irq: irq %u pIFR = 0x%02X pIER = 0x%02X\n",
>>>>>>> refs/remotes/origin/master
		irq, (int) psc_read_byte(pIFR), (int) psc_read_byte(pIER));
#endif

	events = psc_read_byte(pIFR) & psc_read_byte(pIER) & 0xF;
	if (!events)
<<<<<<< HEAD
<<<<<<< HEAD
		return IRQ_NONE;
=======
		return;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return;
>>>>>>> refs/remotes/origin/master

	irq_num = irq << 3;
	irq_bit = 1;
	do {
		if (events & irq_bit) {
			psc_write_byte(pIFR, irq_bit);
<<<<<<< HEAD
<<<<<<< HEAD
			m68k_handle_int(irq_num);
=======
			generic_handle_irq(irq_num);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			generic_handle_irq(irq_num);
>>>>>>> refs/remotes/origin/master
		}
		irq_num++;
		irq_bit <<= 1;
	} while (events >= irq_bit);
<<<<<<< HEAD
<<<<<<< HEAD
	return IRQ_HANDLED;
=======
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * Register the PSC interrupt dispatchers for autovector interrupts 3-6.
 */

void __init psc_register_interrupts(void)
{
	irq_set_chained_handler(IRQ_AUTO_3, psc_irq);
	irq_set_handler_data(IRQ_AUTO_3, (void *)0x30);
	irq_set_chained_handler(IRQ_AUTO_4, psc_irq);
	irq_set_handler_data(IRQ_AUTO_4, (void *)0x40);
	irq_set_chained_handler(IRQ_AUTO_5, psc_irq);
	irq_set_handler_data(IRQ_AUTO_5, (void *)0x50);
	irq_set_chained_handler(IRQ_AUTO_6, psc_irq);
	irq_set_handler_data(IRQ_AUTO_6, (void *)0x60);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

void psc_irq_enable(int irq) {
	int irq_src	= IRQ_SRC(irq);
	int irq_idx	= IRQ_IDX(irq);
	int pIER	= pIERbase + (irq_src << 4);

#ifdef DEBUG_IRQUSE
	printk("psc_irq_enable(%d)\n", irq);
#endif
	psc_write_byte(pIER, (1 << irq_idx) | 0x80);
}

void psc_irq_disable(int irq) {
	int irq_src	= IRQ_SRC(irq);
	int irq_idx	= IRQ_IDX(irq);
	int pIER	= pIERbase + (irq_src << 4);

#ifdef DEBUG_IRQUSE
	printk("psc_irq_disable(%d)\n", irq);
#endif
	psc_write_byte(pIER, 1 << irq_idx);
}
<<<<<<< HEAD
<<<<<<< HEAD

void psc_irq_clear(int irq) {
	int irq_src	= IRQ_SRC(irq);
	int irq_idx	= IRQ_IDX(irq);
	int pIFR	= pIERbase + (irq_src << 4);

	psc_write_byte(pIFR, 1 << irq_idx);
}

int psc_irq_pending(int irq)
{
	int irq_src	= IRQ_SRC(irq);
	int irq_idx	= IRQ_IDX(irq);
	int pIFR	= pIERbase + (irq_src << 4);

	return psc_read_byte(pIFR) & (1 << irq_idx);
}
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
