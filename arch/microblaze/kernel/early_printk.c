/*
 * Early printk support for Microblaze.
 *
 * Copyright (C) 2007-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2007-2009 PetaLogix
 * Copyright (C) 2003-2006 Yasushi SHOJI <yashi@atmark-techno.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/console.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/tty.h>
#include <linux/io.h>
#include <asm/processor.h>
#include <linux/fcntl.h>
#include <asm/setup.h>
#include <asm/prom.h>

<<<<<<< HEAD
static u32 early_console_initialized;
=======
>>>>>>> refs/remotes/origin/master
static u32 base_addr;

#ifdef CONFIG_SERIAL_UARTLITE_CONSOLE
static void early_printk_uartlite_putc(char c)
{
	/*
	 * Limit how many times we'll spin waiting for TX FIFO status.
	 * This will prevent lockups if the base address is incorrectly
	 * set, or any other issue on the UARTLITE.
	 * This limit is pretty arbitrary, unless we are at about 10 baud
	 * we'll never timeout on a working UART.
	 */

<<<<<<< HEAD
<<<<<<< HEAD
	unsigned retries = 10000;
=======
	unsigned retries = 1000000;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned retries = 1000000;
>>>>>>> refs/remotes/origin/master
	/* read status bit - 0x8 offset */
	while (--retries && (in_be32(base_addr + 8) & (1 << 3)))
		;

	/* Only attempt the iowrite if we didn't timeout */
	/* write to TX_FIFO - 0x4 offset */
	if (retries)
		out_be32(base_addr + 4, c & 0xff);
}

static void early_printk_uartlite_write(struct console *unused,
					const char *s, unsigned n)
{
	while (*s && n-- > 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		early_printk_uartlite_putc(*s);
		if (*s == '\n')
			early_printk_uartlite_putc('\r');
=======
		if (*s == '\n')
			early_printk_uartlite_putc('\r');
		early_printk_uartlite_putc(*s);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (*s == '\n')
			early_printk_uartlite_putc('\r');
		early_printk_uartlite_putc(*s);
>>>>>>> refs/remotes/origin/master
		s++;
	}
}

static struct console early_serial_uartlite_console = {
	.name = "earlyser",
	.write = early_printk_uartlite_write,
<<<<<<< HEAD
<<<<<<< HEAD
	.flags = CON_PRINTBUFFER,
=======
	.flags = CON_PRINTBUFFER | CON_BOOT,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.flags = CON_PRINTBUFFER | CON_BOOT,
>>>>>>> refs/remotes/origin/master
	.index = -1,
};
#endif /* CONFIG_SERIAL_UARTLITE_CONSOLE */

#ifdef CONFIG_SERIAL_8250_CONSOLE
static void early_printk_uart16550_putc(char c)
{
	/*
	 * Limit how many times we'll spin waiting for TX FIFO status.
	 * This will prevent lockups if the base address is incorrectly
	 * set, or any other issue on the UARTLITE.
	 * This limit is pretty arbitrary, unless we are at about 10 baud
	 * we'll never timeout on a working UART.
	 */

	#define UART_LSR_TEMT	0x40 /* Transmitter empty */
	#define UART_LSR_THRE	0x20 /* Transmit-hold-register empty */
	#define BOTH_EMPTY (UART_LSR_TEMT | UART_LSR_THRE)

	unsigned retries = 10000;

	while (--retries &&
		!((in_be32(base_addr + 0x14) & BOTH_EMPTY) == BOTH_EMPTY))
		;

	if (retries)
		out_be32(base_addr, c & 0xff);
}

static void early_printk_uart16550_write(struct console *unused,
					const char *s, unsigned n)
{
	while (*s && n-- > 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		early_printk_uart16550_putc(*s);
		if (*s == '\n')
			early_printk_uart16550_putc('\r');
=======
		if (*s == '\n')
			early_printk_uart16550_putc('\r');
		early_printk_uart16550_putc(*s);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (*s == '\n')
			early_printk_uart16550_putc('\r');
		early_printk_uart16550_putc(*s);
>>>>>>> refs/remotes/origin/master
		s++;
	}
}

static struct console early_serial_uart16550_console = {
	.name = "earlyser",
	.write = early_printk_uart16550_write,
<<<<<<< HEAD
<<<<<<< HEAD
	.flags = CON_PRINTBUFFER,
=======
	.flags = CON_PRINTBUFFER | CON_BOOT,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.flags = CON_PRINTBUFFER | CON_BOOT,
>>>>>>> refs/remotes/origin/master
	.index = -1,
};
#endif /* CONFIG_SERIAL_8250_CONSOLE */

<<<<<<< HEAD
static struct console *early_console;

void early_printk(const char *fmt, ...)
{
	char buf[512];
	int n;
	va_list ap;

	if (early_console_initialized) {
		va_start(ap, fmt);
		n = vscnprintf(buf, 512, fmt, ap);
		early_console->write(early_console, buf, n);
		va_end(ap);
	}
}

int __init setup_early_printk(char *opt)
{
<<<<<<< HEAD
	if (early_console_initialized)
		return 1;

#ifdef CONFIG_SERIAL_UARTLITE_CONSOLE
	base_addr = early_uartlite_console();
	if (base_addr) {
		early_console_initialized = 1;
#ifdef CONFIG_MMU
		early_console_reg_tlb_alloc(base_addr);
#endif
		early_console = &early_serial_uartlite_console;
		early_printk("early_printk_console is enabled at 0x%08x\n",
							base_addr);

		/* register_console(early_console); */

		return 0;
	}
#endif /* CONFIG_SERIAL_UARTLITE_CONSOLE */

#ifdef CONFIG_SERIAL_8250_CONSOLE
	base_addr = early_uart16550_console();
	base_addr &= ~3; /* clear register offset */
	if (base_addr) {
		early_console_initialized = 1;
#ifdef CONFIG_MMU
		early_console_reg_tlb_alloc(base_addr);
#endif
		early_console = &early_serial_uart16550_console;

		early_printk("early_printk_console is enabled at 0x%08x\n",
							base_addr);

		/* register_console(early_console); */

		return 0;
	}
#endif /* CONFIG_SERIAL_8250_CONSOLE */

	return 1;
}

=======
	int version = 0;

	if (early_console_initialized)
=======
int __init setup_early_printk(char *opt)
{
	int version = 0;

	if (early_console)
>>>>>>> refs/remotes/origin/master
		return 1;

	base_addr = of_early_console(&version);
	if (base_addr) {
#ifdef CONFIG_MMU
		early_console_reg_tlb_alloc(base_addr);
#endif
		switch (version) {
#ifdef CONFIG_SERIAL_UARTLITE_CONSOLE
		case UARTLITE:
<<<<<<< HEAD
			printk(KERN_INFO "Early console on uartlite "
						"at 0x%08x\n", base_addr);
=======
			pr_info("Early console on uartlite at 0x%08x\n",
								base_addr);
>>>>>>> refs/remotes/origin/master
			early_console = &early_serial_uartlite_console;
			break;
#endif
#ifdef CONFIG_SERIAL_8250_CONSOLE
		case UART16550:
<<<<<<< HEAD
			printk(KERN_INFO "Early console on uart16650 "
						"at 0x%08x\n", base_addr);
=======
			pr_info("Early console on uart16650 at 0x%08x\n",
								base_addr);
>>>>>>> refs/remotes/origin/master
			early_console = &early_serial_uart16550_console;
			break;
#endif
		default:
<<<<<<< HEAD
			printk(KERN_INFO  "Unsupported early console %d\n",
=======
			pr_info("Unsupported early console %d\n",
>>>>>>> refs/remotes/origin/master
								version);
			return 1;
		}

		register_console(early_console);
<<<<<<< HEAD
		early_console_initialized = 1;
=======
>>>>>>> refs/remotes/origin/master
		return 0;
	}
	return 1;
}

/* Remap early console to virtual address and do not allocate one TLB
 * only for early console because of performance degression */
void __init remap_early_printk(void)
{
<<<<<<< HEAD
	if (!early_console_initialized || !early_console)
		return;
	printk(KERN_INFO "early_printk_console remapping from 0x%x to ",
								base_addr);
	base_addr = (u32) ioremap(base_addr, PAGE_SIZE);
	printk(KERN_CONT "0x%x\n", base_addr);
=======
	if (!early_console)
		return;
	pr_info("early_printk_console remapping from 0x%x to ", base_addr);
	base_addr = (u32) ioremap(base_addr, PAGE_SIZE);
	pr_cont("0x%x\n", base_addr);
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_MMU
	/*
	 * Early console is on the top of skipped TLB entries
	 * decrease tlb_skip size ensure that hardcoded TLB entry will be
	 * used by generic algorithm
	 * FIXME check if early console mapping is on the top by rereading
	 * TLB entry and compare baseaddr
	 *  mts  rtlbx, (tlb_skip - 1)
	 *  nop
	 *  mfs  rX, rtlblo
	 *  nop
	 *  cmp rX, orig_base_addr
	 */
	tlb_skip -= 1;
#endif
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
void __init disable_early_printk(void)
{
	if (!early_console_initialized || !early_console)
		return;
	printk(KERN_WARNING "disabling early console\n");
	unregister_console(early_console);
	early_console_initialized = 0;
=======
void __init disable_early_printk(void)
{
	if (!early_console)
		return;
	pr_warn("disabling early console\n");
	unregister_console(early_console);
	early_console = NULL;
>>>>>>> refs/remotes/origin/master
}
