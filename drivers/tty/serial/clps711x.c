/*
 *  Driver for CLPS711x serial ports
 *
 *  Based on drivers/char/serial.c, by Linus Torvalds, Theodore Ts'o.
 *
 *  Copyright 1999 ARM Limited
 *  Copyright (C) 2000 Deep Blue Solutions Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
<<<<<<< HEAD
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> refs/remotes/origin/master
 */

#if defined(CONFIG_SERIAL_CLPS711X_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
#define SUPPORT_SYSRQ
#endif

#include <linux/module.h>
<<<<<<< HEAD
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/hardware/clps7111.h>

#define UART_NR		2

#define SERIAL_CLPS711X_MAJOR	204
#define SERIAL_CLPS711X_MINOR	40
#define SERIAL_CLPS711X_NR	UART_NR

/*
 * We use the relevant SYSCON register as a base address for these ports.
 */
#define UBRLCR(port)		((port)->iobase + UBRLCR1 - SYSCON1)
#define UARTDR(port)		((port)->iobase + UARTDR1 - SYSCON1)
#define SYSFLG(port)		((port)->iobase + SYSFLG1 - SYSCON1)
#define SYSCON(port)		((port)->iobase + SYSCON1 - SYSCON1)

#define TX_IRQ(port)		((port)->irq)
#define RX_IRQ(port)		((port)->irq + 1)

#define UART_ANY_ERR		(UARTDR_FRMERR | UARTDR_PARERR | UARTDR_OVERR)

#define tx_enabled(port)	((port)->unused[0])

static void clps711xuart_stop_tx(struct uart_port *port)
{
	if (tx_enabled(port)) {
		disable_irq(TX_IRQ(port));
		tx_enabled(port) = 0;
	}
}

static void clps711xuart_start_tx(struct uart_port *port)
{
	if (!tx_enabled(port)) {
		enable_irq(TX_IRQ(port));
		tx_enabled(port) = 1;
	}
}

static void clps711xuart_stop_rx(struct uart_port *port)
{
	disable_irq(RX_IRQ(port));
}

static void clps711xuart_enable_ms(struct uart_port *port)
{
}

static irqreturn_t clps711xuart_int_rx(int irq, void *dev_id)
{
	struct uart_port *port = dev_id;
	struct tty_struct *tty = port->state->port.tty;
	unsigned int status, ch, flg;

	status = clps_readl(SYSFLG(port));
	while (!(status & SYSFLG_URXFE)) {
		ch = clps_readl(UARTDR(port));

		port->icount.rx++;

		flg = TTY_NORMAL;

		/*
		 * Note that the error handling code is
		 * out of the main execution path
		 */
		if (unlikely(ch & UART_ANY_ERR)) {
			if (ch & UARTDR_PARERR)
				port->icount.parity++;
			else if (ch & UARTDR_FRMERR)
				port->icount.frame++;
			if (ch & UARTDR_OVERR)
				port->icount.overrun++;

			ch &= port->read_status_mask;

			if (ch & UARTDR_PARERR)
				flg = TTY_PARITY;
			else if (ch & UARTDR_FRMERR)
				flg = TTY_FRAME;

#ifdef SUPPORT_SYSRQ
			port->sysrq = 0;
#endif
		}

		if (uart_handle_sysrq_char(port, ch))
			goto ignore_char;

		/*
		 * CHECK: does overrun affect the current character?
		 * ASSUMPTION: it does not.
		 */
		uart_insert_char(port, ch, UARTDR_OVERR, ch, flg);

	ignore_char:
		status = clps_readl(SYSFLG(port));
	}
	tty_flip_buffer_push(tty);
	return IRQ_HANDLED;
}

static irqreturn_t clps711xuart_int_tx(int irq, void *dev_id)
{
	struct uart_port *port = dev_id;
	struct circ_buf *xmit = &port->state->xmit;
	int count;

	if (port->x_char) {
		clps_writel(port->x_char, UARTDR(port));
=======
#include <linux/device.h>
#include <linux/console.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/ioport.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>

#include <linux/mfd/syscon.h>
#include <linux/mfd/syscon/clps711x.h>

#define UART_CLPS711X_DEVNAME	"ttyCL"
#define UART_CLPS711X_NR	2
#define UART_CLPS711X_MAJOR	204
#define UART_CLPS711X_MINOR	40

#define UARTDR_OFFSET		(0x00)
#define UBRLCR_OFFSET		(0x40)

#define UARTDR_FRMERR		(1 << 8)
#define UARTDR_PARERR		(1 << 9)
#define UARTDR_OVERR		(1 << 10)

#define UBRLCR_BAUD_MASK	((1 << 12) - 1)
#define UBRLCR_BREAK		(1 << 12)
#define UBRLCR_PRTEN		(1 << 13)
#define UBRLCR_EVENPRT		(1 << 14)
#define UBRLCR_XSTOP		(1 << 15)
#define UBRLCR_FIFOEN		(1 << 16)
#define UBRLCR_WRDLEN5		(0 << 17)
#define UBRLCR_WRDLEN6		(1 << 17)
#define UBRLCR_WRDLEN7		(2 << 17)
#define UBRLCR_WRDLEN8		(3 << 17)
#define UBRLCR_WRDLEN_MASK	(3 << 17)

struct clps711x_port {
	struct uart_port	port;
	unsigned int		tx_enabled;
	int			rx_irq;
	struct regmap		*syscon;
	bool			use_ms;
};

static struct uart_driver clps711x_uart = {
	.owner		= THIS_MODULE,
	.driver_name	= UART_CLPS711X_DEVNAME,
	.dev_name	= UART_CLPS711X_DEVNAME,
	.major		= UART_CLPS711X_MAJOR,
	.minor		= UART_CLPS711X_MINOR,
	.nr		= UART_CLPS711X_NR,
};

static void uart_clps711x_stop_tx(struct uart_port *port)
{
	struct clps711x_port *s = dev_get_drvdata(port->dev);

	if (s->tx_enabled) {
		disable_irq(port->irq);
		s->tx_enabled = 0;
	}
}

static void uart_clps711x_start_tx(struct uart_port *port)
{
	struct clps711x_port *s = dev_get_drvdata(port->dev);

	if (!s->tx_enabled) {
		s->tx_enabled = 1;
		enable_irq(port->irq);
	}
}

static irqreturn_t uart_clps711x_int_rx(int irq, void *dev_id)
{
	struct uart_port *port = dev_id;
	struct clps711x_port *s = dev_get_drvdata(port->dev);
	unsigned int status, flg;
	u16 ch;

	for (;;) {
		u32 sysflg = 0;

		regmap_read(s->syscon, SYSFLG_OFFSET, &sysflg);
		if (sysflg & SYSFLG_URXFE)
			break;

		ch = readw(port->membase + UARTDR_OFFSET);
		status = ch & (UARTDR_FRMERR | UARTDR_PARERR | UARTDR_OVERR);
		ch &= 0xff;

		port->icount.rx++;
		flg = TTY_NORMAL;

		if (unlikely(status)) {
			if (status & UARTDR_PARERR)
				port->icount.parity++;
			else if (status & UARTDR_FRMERR)
				port->icount.frame++;
			else if (status & UARTDR_OVERR)
				port->icount.overrun++;

			status &= port->read_status_mask;

			if (status & UARTDR_PARERR)
				flg = TTY_PARITY;
			else if (status & UARTDR_FRMERR)
				flg = TTY_FRAME;
			else if (status & UARTDR_OVERR)
				flg = TTY_OVERRUN;
		}

		if (uart_handle_sysrq_char(port, ch))
			continue;

		if (status & port->ignore_status_mask)
			continue;

		uart_insert_char(port, status, UARTDR_OVERR, ch, flg);
	}

	tty_flip_buffer_push(&port->state->port);

	return IRQ_HANDLED;
}

static irqreturn_t uart_clps711x_int_tx(int irq, void *dev_id)
{
	struct uart_port *port = dev_id;
	struct clps711x_port *s = dev_get_drvdata(port->dev);
	struct circ_buf *xmit = &port->state->xmit;

	if (port->x_char) {
		writew(port->x_char, port->membase + UARTDR_OFFSET);
>>>>>>> refs/remotes/origin/master
		port->icount.tx++;
		port->x_char = 0;
		return IRQ_HANDLED;
	}

<<<<<<< HEAD
	if (uart_circ_empty(xmit) || uart_tx_stopped(port))
		goto disable_tx_irq;

	count = port->fifosize >> 1;
	do {
		clps_writel(xmit->buf[xmit->tail], UARTDR(port));
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
		if (uart_circ_empty(xmit))
			break;
	} while (--count > 0);
=======
	if (uart_circ_empty(xmit) || uart_tx_stopped(port)) {
		if (s->tx_enabled) {
			disable_irq_nosync(port->irq);
			s->tx_enabled = 0;
		}
		return IRQ_HANDLED;
	}

	while (!uart_circ_empty(xmit)) {
		u32 sysflg = 0;

		writew(xmit->buf[xmit->tail], port->membase + UARTDR_OFFSET);
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;

		regmap_read(s->syscon, SYSFLG_OFFSET, &sysflg);
		if (sysflg & SYSFLG_UTXFF)
			break;
	}
>>>>>>> refs/remotes/origin/master

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);

<<<<<<< HEAD
	if (uart_circ_empty(xmit)) {
	disable_tx_irq:
		disable_irq_nosync(TX_IRQ(port));
		tx_enabled(port) = 0;
	}

	return IRQ_HANDLED;
}

static unsigned int clps711xuart_tx_empty(struct uart_port *port)
{
	unsigned int status = clps_readl(SYSFLG(port));
	return status & SYSFLG_UBUSY ? 0 : TIOCSER_TEMT;
}

static unsigned int clps711xuart_get_mctrl(struct uart_port *port)
{
	unsigned int port_addr;
	unsigned int result = 0;
	unsigned int status;

	port_addr = SYSFLG(port);
	if (port_addr == SYSFLG1) {
		status = clps_readl(SYSFLG1);
		if (status & SYSFLG1_DCD)
			result |= TIOCM_CAR;
		if (status & SYSFLG1_DSR)
			result |= TIOCM_DSR;
		if (status & SYSFLG1_CTS)
			result |= TIOCM_CTS;
	}
=======
	return IRQ_HANDLED;
}

static unsigned int uart_clps711x_tx_empty(struct uart_port *port)
{
	struct clps711x_port *s = dev_get_drvdata(port->dev);
	u32 sysflg = 0;

	regmap_read(s->syscon, SYSFLG_OFFSET, &sysflg);

	return (sysflg & SYSFLG_UBUSY) ? 0 : TIOCSER_TEMT;
}

static unsigned int uart_clps711x_get_mctrl(struct uart_port *port)
{
	struct clps711x_port *s = dev_get_drvdata(port->dev);
	unsigned int result = 0;

	if (s->use_ms) {
		u32 sysflg = 0;

		regmap_read(s->syscon, SYSFLG_OFFSET, &sysflg);
		if (sysflg & SYSFLG1_DCD)
			result |= TIOCM_CAR;
		if (sysflg & SYSFLG1_DSR)
			result |= TIOCM_DSR;
		if (sysflg & SYSFLG1_CTS)
			result |= TIOCM_CTS;
	} else
		result = TIOCM_DSR | TIOCM_CTS | TIOCM_CAR;
>>>>>>> refs/remotes/origin/master

	return result;
}

<<<<<<< HEAD
static void
clps711xuart_set_mctrl_null(struct uart_port *port, unsigned int mctrl)
{
}

static void clps711xuart_break_ctl(struct uart_port *port, int break_state)
{
	unsigned long flags;
	unsigned int ubrlcr;

	spin_lock_irqsave(&port->lock, flags);
	ubrlcr = clps_readl(UBRLCR(port));
	if (break_state == -1)
		ubrlcr |= UBRLCR_BREAK;
	else
		ubrlcr &= ~UBRLCR_BREAK;
	clps_writel(ubrlcr, UBRLCR(port));
	spin_unlock_irqrestore(&port->lock, flags);
}

static int clps711xuart_startup(struct uart_port *port)
{
	unsigned int syscon;
	int retval;

	tx_enabled(port) = 1;

	/*
	 * Allocate the IRQs
	 */
	retval = request_irq(TX_IRQ(port), clps711xuart_int_tx, 0,
			     "clps711xuart_tx", port);
	if (retval)
		return retval;

	retval = request_irq(RX_IRQ(port), clps711xuart_int_rx, 0,
			     "clps711xuart_rx", port);
	if (retval) {
		free_irq(TX_IRQ(port), port);
		return retval;
	}

	/*
	 * enable the port
	 */
	syscon = clps_readl(SYSCON(port));
	syscon |= SYSCON_UARTEN;
	clps_writel(syscon, SYSCON(port));

	return 0;
}

static void clps711xuart_shutdown(struct uart_port *port)
{
	unsigned int ubrlcr, syscon;

	/*
	 * Free the interrupt
	 */
	free_irq(TX_IRQ(port), port);	/* TX interrupt */
	free_irq(RX_IRQ(port), port);	/* RX interrupt */

	/*
	 * disable the port
	 */
	syscon = clps_readl(SYSCON(port));
	syscon &= ~SYSCON_UARTEN;
	clps_writel(syscon, SYSCON(port));

	/*
	 * disable break condition and fifos
	 */
	ubrlcr = clps_readl(UBRLCR(port));
	ubrlcr &= ~(UBRLCR_FIFOEN | UBRLCR_BREAK);
	clps_writel(ubrlcr, UBRLCR(port));
}

static void
clps711xuart_set_termios(struct uart_port *port, struct ktermios *termios,
			 struct ktermios *old)
{
	unsigned int ubrlcr, baud, quot;
	unsigned long flags;

	/*
	 * We don't implement CREAD.
	 */
	termios->c_cflag |= CREAD;

	/*
	 * Ask the core to calculate the divisor for us.
	 */
	baud = uart_get_baud_rate(port, termios, old, 0, port->uartclk/16); 
=======
static void uart_clps711x_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	/* Do nothing */
}

static void uart_clps711x_break_ctl(struct uart_port *port, int break_state)
{
	unsigned int ubrlcr;

	ubrlcr = readl(port->membase + UBRLCR_OFFSET);
	if (break_state)
		ubrlcr |= UBRLCR_BREAK;
	else
		ubrlcr &= ~UBRLCR_BREAK;
	writel(ubrlcr, port->membase + UBRLCR_OFFSET);
}

static void uart_clps711x_set_ldisc(struct uart_port *port, int ld)
{
	if (!port->line) {
		struct clps711x_port *s = dev_get_drvdata(port->dev);

		regmap_update_bits(s->syscon, SYSCON_OFFSET, SYSCON1_SIREN,
				   (ld == N_IRDA) ? SYSCON1_SIREN : 0);
	}
}

static int uart_clps711x_startup(struct uart_port *port)
{
	struct clps711x_port *s = dev_get_drvdata(port->dev);

	/* Disable break */
	writel(readl(port->membase + UBRLCR_OFFSET) & ~UBRLCR_BREAK,
	       port->membase + UBRLCR_OFFSET);

	/* Enable the port */
	return regmap_update_bits(s->syscon, SYSCON_OFFSET,
				  SYSCON_UARTEN, SYSCON_UARTEN);
}

static void uart_clps711x_shutdown(struct uart_port *port)
{
	struct clps711x_port *s = dev_get_drvdata(port->dev);

	/* Disable the port */
	regmap_update_bits(s->syscon, SYSCON_OFFSET, SYSCON_UARTEN, 0);
}

static void uart_clps711x_set_termios(struct uart_port *port,
				      struct ktermios *termios,
				      struct ktermios *old)
{
	u32 ubrlcr;
	unsigned int baud, quot;

	/* Mask termios capabilities we don't support */
	termios->c_cflag &= ~CMSPAR;
	termios->c_iflag &= ~(BRKINT | IGNBRK);

	/* Ask the core to calculate the divisor for us */
	baud = uart_get_baud_rate(port, termios, old, port->uartclk / 4096,
						      port->uartclk / 16);
>>>>>>> refs/remotes/origin/master
	quot = uart_get_divisor(port, baud);

	switch (termios->c_cflag & CSIZE) {
	case CS5:
		ubrlcr = UBRLCR_WRDLEN5;
		break;
	case CS6:
		ubrlcr = UBRLCR_WRDLEN6;
		break;
	case CS7:
		ubrlcr = UBRLCR_WRDLEN7;
		break;
<<<<<<< HEAD
	default: // CS8
		ubrlcr = UBRLCR_WRDLEN8;
		break;
	}
	if (termios->c_cflag & CSTOPB)
		ubrlcr |= UBRLCR_XSTOP;
=======
	case CS8:
	default:
		ubrlcr = UBRLCR_WRDLEN8;
		break;
	}

	if (termios->c_cflag & CSTOPB)
		ubrlcr |= UBRLCR_XSTOP;

>>>>>>> refs/remotes/origin/master
	if (termios->c_cflag & PARENB) {
		ubrlcr |= UBRLCR_PRTEN;
		if (!(termios->c_cflag & PARODD))
			ubrlcr |= UBRLCR_EVENPRT;
	}
<<<<<<< HEAD
	if (port->fifosize > 1)
		ubrlcr |= UBRLCR_FIFOEN;

	spin_lock_irqsave(&port->lock, flags);

	/*
	 * Update the per-port timeout.
	 */
	uart_update_timeout(port, termios->c_cflag, baud);

=======

	/* Enable FIFO */
	ubrlcr |= UBRLCR_FIFOEN;

	/* Set read status mask */
>>>>>>> refs/remotes/origin/master
	port->read_status_mask = UARTDR_OVERR;
	if (termios->c_iflag & INPCK)
		port->read_status_mask |= UARTDR_PARERR | UARTDR_FRMERR;

<<<<<<< HEAD
	/*
	 * Characters to ignore
	 */
	port->ignore_status_mask = 0;
	if (termios->c_iflag & IGNPAR)
		port->ignore_status_mask |= UARTDR_FRMERR | UARTDR_PARERR;
	if (termios->c_iflag & IGNBRK) {
		/*
		 * If we're ignoring parity and break indicators,
		 * ignore overruns to (for real raw support).
		 */
		if (termios->c_iflag & IGNPAR)
			port->ignore_status_mask |= UARTDR_OVERR;
	}

	quot -= 1;

	clps_writel(ubrlcr | quot, UBRLCR(port));

	spin_unlock_irqrestore(&port->lock, flags);
}

static const char *clps711xuart_type(struct uart_port *port)
{
	return port->type == PORT_CLPS711X ? "CLPS711x" : NULL;
}

/*
 * Configure/autoconfigure the port.
 */
static void clps711xuart_config_port(struct uart_port *port, int flags)
=======
	/* Set status ignore mask */
	port->ignore_status_mask = 0;
	if (!(termios->c_cflag & CREAD))
		port->ignore_status_mask |= UARTDR_OVERR | UARTDR_PARERR |
					    UARTDR_FRMERR;

	uart_update_timeout(port, termios->c_cflag, baud);

	writel(ubrlcr | (quot - 1), port->membase + UBRLCR_OFFSET);
}

static const char *uart_clps711x_type(struct uart_port *port)
{
	return (port->type == PORT_CLPS711X) ? "CLPS711X" : NULL;
}

static void uart_clps711x_config_port(struct uart_port *port, int flags)
>>>>>>> refs/remotes/origin/master
{
	if (flags & UART_CONFIG_TYPE)
		port->type = PORT_CLPS711X;
}

<<<<<<< HEAD
static void clps711xuart_release_port(struct uart_port *port)
{
}

static int clps711xuart_request_port(struct uart_port *port)
=======
static void uart_clps711x_nop_void(struct uart_port *port)
{
}

static int uart_clps711x_nop_int(struct uart_port *port)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

<<<<<<< HEAD
static struct uart_ops clps711x_pops = {
	.tx_empty	= clps711xuart_tx_empty,
	.set_mctrl	= clps711xuart_set_mctrl_null,
	.get_mctrl	= clps711xuart_get_mctrl,
	.stop_tx	= clps711xuart_stop_tx,
	.start_tx	= clps711xuart_start_tx,
	.stop_rx	= clps711xuart_stop_rx,
	.enable_ms	= clps711xuart_enable_ms,
	.break_ctl	= clps711xuart_break_ctl,
	.startup	= clps711xuart_startup,
	.shutdown	= clps711xuart_shutdown,
	.set_termios	= clps711xuart_set_termios,
	.type		= clps711xuart_type,
	.config_port	= clps711xuart_config_port,
	.release_port	= clps711xuart_release_port,
	.request_port	= clps711xuart_request_port,
};

static struct uart_port clps711x_ports[UART_NR] = {
	{
		.iobase		= SYSCON1,
		.irq		= IRQ_UTXINT1, /* IRQ_URXINT1, IRQ_UMSINT */
		.uartclk	= 3686400,
		.fifosize	= 16,
		.ops		= &clps711x_pops,
		.line		= 0,
		.flags		= UPF_BOOT_AUTOCONF,
	},
	{
		.iobase		= SYSCON2,
		.irq		= IRQ_UTXINT2, /* IRQ_URXINT2 */
		.uartclk	= 3686400,
		.fifosize	= 16,
		.ops		= &clps711x_pops,
		.line		= 1,
		.flags		= UPF_BOOT_AUTOCONF,
	}
};

#ifdef CONFIG_SERIAL_CLPS711X_CONSOLE
static void clps711xuart_console_putchar(struct uart_port *port, int ch)
{
	while (clps_readl(SYSFLG(port)) & SYSFLG_UTXFF)
		barrier();
	clps_writel(ch, UARTDR(port));
}

/*
 *	Print a string to the serial port trying not to disturb
 *	any possible real use of the port...
 *
 *	The console_lock must be held when we get here.
 *
 *	Note that this is called with interrupts already disabled
 */
static void
clps711xuart_console_write(struct console *co, const char *s,
			   unsigned int count)
{
	struct uart_port *port = clps711x_ports + co->index;
	unsigned int status, syscon;

	/*
	 *	Ensure that the port is enabled.
	 */
	syscon = clps_readl(SYSCON(port));
	clps_writel(syscon | SYSCON_UARTEN, SYSCON(port));

	uart_console_write(port, s, count, clps711xuart_console_putchar);

	/*
	 *	Finally, wait for transmitter to become empty
	 *	and restore the uart state.
	 */
	do {
		status = clps_readl(SYSFLG(port));
	} while (status & SYSFLG_UBUSY);

	clps_writel(syscon, SYSCON(port));
}

static void __init
clps711xuart_console_get_options(struct uart_port *port, int *baud,
				 int *parity, int *bits)
{
	if (clps_readl(SYSCON(port)) & SYSCON_UARTEN) {
		unsigned int ubrlcr, quot;

		ubrlcr = clps_readl(UBRLCR(port));

		*parity = 'n';
		if (ubrlcr & UBRLCR_PRTEN) {
			if (ubrlcr & UBRLCR_EVENPRT)
				*parity = 'e';
			else
				*parity = 'o';
		}

		if ((ubrlcr & UBRLCR_WRDLEN_MASK) == UBRLCR_WRDLEN7)
			*bits = 7;
		else
			*bits = 8;

		quot = ubrlcr & UBRLCR_BAUD_MASK;
		*baud = port->uartclk / (16 * (quot + 1));
	}
}

static int __init clps711xuart_console_setup(struct console *co, char *options)
{
	struct uart_port *port;
	int baud = 38400;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	/*
	 * Check whether an invalid uart number has been specified, and
	 * if so, search for the first available port that does have
	 * console support.
	 */
	port = uart_get_console(clps711x_ports, UART_NR, co);

	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);
	else
		clps711xuart_console_get_options(port, &baud, &parity, &bits);

	return uart_set_options(port, co, baud, parity, bits, flow);
}

static struct uart_driver clps711x_reg;
static struct console clps711x_console = {
	.name		= "ttyCL",
	.write		= clps711xuart_console_write,
	.device		= uart_console_device,
	.setup		= clps711xuart_console_setup,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
	.data		= &clps711x_reg,
};

static int __init clps711xuart_console_init(void)
{
	register_console(&clps711x_console);
	return 0;
}
console_initcall(clps711xuart_console_init);

#define CLPS711X_CONSOLE	&clps711x_console
#else
#define CLPS711X_CONSOLE	NULL
#endif

static struct uart_driver clps711x_reg = {
	.driver_name		= "ttyCL",
	.dev_name		= "ttyCL",
	.major			= SERIAL_CLPS711X_MAJOR,
	.minor			= SERIAL_CLPS711X_MINOR,
	.nr			= UART_NR,

	.cons			= CLPS711X_CONSOLE,
};

static int __init clps711xuart_init(void)
{
	int ret, i;

	printk(KERN_INFO "Serial: CLPS711x driver\n");

	ret = uart_register_driver(&clps711x_reg);
	if (ret)
		return ret;

	for (i = 0; i < UART_NR; i++)
		uart_add_one_port(&clps711x_reg, &clps711x_ports[i]);

	return 0;
}

static void __exit clps711xuart_exit(void)
{
	int i;

	for (i = 0; i < UART_NR; i++)
		uart_remove_one_port(&clps711x_reg, &clps711x_ports[i]);

	uart_unregister_driver(&clps711x_reg);
}

module_init(clps711xuart_init);
module_exit(clps711xuart_exit);

MODULE_AUTHOR("Deep Blue Solutions Ltd");
MODULE_DESCRIPTION("CLPS-711x generic serial driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS_CHARDEV(SERIAL_CLPS711X_MAJOR, SERIAL_CLPS711X_MINOR);
=======
static const struct uart_ops uart_clps711x_ops = {
	.tx_empty	= uart_clps711x_tx_empty,
	.set_mctrl	= uart_clps711x_set_mctrl,
	.get_mctrl	= uart_clps711x_get_mctrl,
	.stop_tx	= uart_clps711x_stop_tx,
	.start_tx	= uart_clps711x_start_tx,
	.stop_rx	= uart_clps711x_nop_void,
	.enable_ms	= uart_clps711x_nop_void,
	.break_ctl	= uart_clps711x_break_ctl,
	.set_ldisc	= uart_clps711x_set_ldisc,
	.startup	= uart_clps711x_startup,
	.shutdown	= uart_clps711x_shutdown,
	.set_termios	= uart_clps711x_set_termios,
	.type		= uart_clps711x_type,
	.config_port	= uart_clps711x_config_port,
	.release_port	= uart_clps711x_nop_void,
	.request_port	= uart_clps711x_nop_int,
};

#ifdef CONFIG_SERIAL_CLPS711X_CONSOLE
static void uart_clps711x_console_putchar(struct uart_port *port, int ch)
{
	struct clps711x_port *s = dev_get_drvdata(port->dev);
	u32 sysflg = 0;

	do {
		regmap_read(s->syscon, SYSFLG_OFFSET, &sysflg);
	} while (sysflg & SYSFLG_UTXFF);

	writew(ch, port->membase + UARTDR_OFFSET);
}

static void uart_clps711x_console_write(struct console *co, const char *c,
					unsigned n)
{
	struct uart_port *port = clps711x_uart.state[co->index].uart_port;
	struct clps711x_port *s = dev_get_drvdata(port->dev);
	u32 sysflg = 0;

	uart_console_write(port, c, n, uart_clps711x_console_putchar);

	/* Wait for transmitter to become empty */
	do {
		regmap_read(s->syscon, SYSFLG_OFFSET, &sysflg);
	} while (sysflg & SYSFLG_UBUSY);
}

static int uart_clps711x_console_setup(struct console *co, char *options)
{
	int baud = 38400, bits = 8, parity = 'n', flow = 'n';
	int ret, index = co->index;
	struct clps711x_port *s;
	struct uart_port *port;
	unsigned int quot;
	u32 ubrlcr;

	if (index < 0 || index >= UART_CLPS711X_NR)
		return -EINVAL;

	port = clps711x_uart.state[index].uart_port;
	if (!port)
		return -ENODEV;

	s = dev_get_drvdata(port->dev);

	if (!options) {
		u32 syscon = 0;

		regmap_read(s->syscon, SYSCON_OFFSET, &syscon);
		if (syscon & SYSCON_UARTEN) {
			ubrlcr = readl(port->membase + UBRLCR_OFFSET);

			if (ubrlcr & UBRLCR_PRTEN) {
				if (ubrlcr & UBRLCR_EVENPRT)
					parity = 'e';
				else
					parity = 'o';
			}

			if ((ubrlcr & UBRLCR_WRDLEN_MASK) == UBRLCR_WRDLEN7)
				bits = 7;

			quot = ubrlcr & UBRLCR_BAUD_MASK;
			baud = port->uartclk / (16 * (quot + 1));
		}
	} else
		uart_parse_options(options, &baud, &parity, &bits, &flow);

	ret = uart_set_options(port, co, baud, parity, bits, flow);
	if (ret)
		return ret;

	return regmap_update_bits(s->syscon, SYSCON_OFFSET,
				  SYSCON_UARTEN, SYSCON_UARTEN);
}

static struct console clps711x_console = {
	.name	= UART_CLPS711X_DEVNAME,
	.device	= uart_console_device,
	.write	= uart_clps711x_console_write,
	.setup	= uart_clps711x_console_setup,
	.flags	= CON_PRINTBUFFER,
	.index	= -1,
};
#endif

static int uart_clps711x_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	int ret, index = np ? of_alias_get_id(np, "serial") : pdev->id;
	struct clps711x_port *s;
	struct resource *res;
	struct clk *uart_clk;

	if (index < 0 || index >= UART_CLPS711X_NR)
		return -EINVAL;

	s = devm_kzalloc(&pdev->dev, sizeof(*s), GFP_KERNEL);
	if (!s)
		return -ENOMEM;

	uart_clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(uart_clk))
		return PTR_ERR(uart_clk);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	s->port.membase = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(s->port.membase))
		return PTR_ERR(s->port.membase);

	s->port.irq = platform_get_irq(pdev, 0);
	if (IS_ERR_VALUE(s->port.irq))
		return s->port.irq;

	s->rx_irq = platform_get_irq(pdev, 1);
	if (IS_ERR_VALUE(s->rx_irq))
		return s->rx_irq;

	if (!np) {
		char syscon_name[9];

		sprintf(syscon_name, "syscon.%i", index + 1);
		s->syscon = syscon_regmap_lookup_by_pdevname(syscon_name);
		if (IS_ERR(s->syscon))
			return PTR_ERR(s->syscon);

		s->use_ms = !index;
	} else {
		s->syscon = syscon_regmap_lookup_by_phandle(np, "syscon");
		if (IS_ERR(s->syscon))
			return PTR_ERR(s->syscon);

		if (!index)
			s->use_ms = of_property_read_bool(np, "uart-use-ms");
	}

	s->port.line		= index;
	s->port.dev		= &pdev->dev;
	s->port.iotype		= UPIO_MEM32;
	s->port.mapbase		= res->start;
	s->port.type		= PORT_CLPS711X;
	s->port.fifosize	= 16;
	s->port.flags		= UPF_SKIP_TEST | UPF_FIXED_TYPE;
	s->port.uartclk		= clk_get_rate(uart_clk);
	s->port.ops		= &uart_clps711x_ops;

	platform_set_drvdata(pdev, s);

	ret = uart_add_one_port(&clps711x_uart, &s->port);
	if (ret)
		return ret;

	/* Disable port */
	if (!uart_console(&s->port))
		regmap_update_bits(s->syscon, SYSCON_OFFSET, SYSCON_UARTEN, 0);

	s->tx_enabled = 1;

	ret = devm_request_irq(&pdev->dev, s->port.irq, uart_clps711x_int_tx, 0,
			       dev_name(&pdev->dev), &s->port);
	if (ret) {
		uart_remove_one_port(&clps711x_uart, &s->port);
		return ret;
	}

	ret = devm_request_irq(&pdev->dev, s->rx_irq, uart_clps711x_int_rx, 0,
			       dev_name(&pdev->dev), &s->port);
	if (ret)
		uart_remove_one_port(&clps711x_uart, &s->port);

	return ret;
}

static int uart_clps711x_remove(struct platform_device *pdev)
{
	struct clps711x_port *s = platform_get_drvdata(pdev);

	return uart_remove_one_port(&clps711x_uart, &s->port);
}

static const struct of_device_id __maybe_unused clps711x_uart_dt_ids[] = {
	{ .compatible = "cirrus,clps711x-uart", },
	{ }
};
MODULE_DEVICE_TABLE(of, clps711x_uart_dt_ids);

static struct platform_driver clps711x_uart_platform = {
	.driver = {
		.name		= "clps711x-uart",
		.owner		= THIS_MODULE,
		.of_match_table	= of_match_ptr(clps711x_uart_dt_ids),
	},
	.probe	= uart_clps711x_probe,
	.remove	= uart_clps711x_remove,
};

static int __init uart_clps711x_init(void)
{
	int ret;

#ifdef CONFIG_SERIAL_CLPS711X_CONSOLE
	clps711x_uart.cons = &clps711x_console;
	clps711x_console.data = &clps711x_uart;
#endif

	ret = uart_register_driver(&clps711x_uart);
	if (ret)
		return ret;

	return platform_driver_register(&clps711x_uart_platform);
}
module_init(uart_clps711x_init);

static void __exit uart_clps711x_exit(void)
{
	platform_driver_unregister(&clps711x_uart_platform);
	uart_unregister_driver(&clps711x_uart);
}
module_exit(uart_clps711x_exit);

MODULE_AUTHOR("Deep Blue Solutions Ltd");
MODULE_DESCRIPTION("CLPS711X serial driver");
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
