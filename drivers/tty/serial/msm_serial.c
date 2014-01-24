/*
<<<<<<< HEAD
 * drivers/serial/msm_serial.c - driver for msm7k serial device and console
 *
 * Copyright (C) 2007 Google, Inc.
<<<<<<< HEAD
 * Copyright (c) 2009-2011, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2009-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
 * Author: Robert Love <rlove@google.com>
=======
 * Driver for msm7k serial device and console
 *
 * Copyright (C) 2007 Google, Inc.
 * Author: Robert Love <rlove@google.com>
 * Copyright (c) 2011, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#if defined(CONFIG_SERIAL_MSM_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
# define SUPPORT_SYSRQ
#endif

<<<<<<< HEAD
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/master
#include <linux/hrtimer.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/irq.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/delay.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/console.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
<<<<<<< HEAD
#include <linux/nmi.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <mach/msm_serial_pdata.h>
#include "msm_serial.h"


#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
enum msm_clk_states_e {
	MSM_CLK_PORT_OFF,     /* uart port not in use */
	MSM_CLK_OFF,          /* clock enabled */
	MSM_CLK_REQUEST_OFF,  /* disable after TX flushed */
	MSM_CLK_ON,           /* clock disabled */
};
#endif

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
/* optional low power wakeup, typically on a GPIO RX irq */
struct msm_wakeup {
	int irq;  /* < 0 indicates low power wakeup disabled */
	unsigned char ignore;  /* bool */

	/* bool: inject char into rx tty on wakeup */
	unsigned char inject_rx;
	char rx_to_inject;
};
#endif
=======
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_device.h>

#include "msm_serial.h"
>>>>>>> refs/remotes/origin/master

struct msm_port {
	struct uart_port	uart;
	char			name[16];
	struct clk		*clk;
<<<<<<< HEAD
	unsigned int		imr;
#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	enum msm_clk_states_e	clk_state;
	struct hrtimer		clk_off_timer;
	ktime_t			clk_off_delay;
#endif
#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	struct msm_wakeup wakeup;
#endif
<<<<<<< HEAD
=======
	int			uim;
>>>>>>> refs/remotes/origin/cm-10.0
};

#define UART_TO_MSM(uart_port)	((struct msm_port *) uart_port)
#define is_console(port)	((port)->cons && \
				(port)->cons->index == (port)->line)


static inline void msm_write(struct uart_port *port, unsigned int val,
			     unsigned int off)
{
	__raw_writel(val, port->membase + off);
}

static inline unsigned int msm_read(struct uart_port *port, unsigned int off)
{
	return __raw_readl(port->membase + off);
}

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
static inline unsigned int use_low_power_wakeup(struct msm_port *msm_port)
{
	return (msm_port->wakeup.irq >= 0);
}
#endif

=======
	struct clk		*pclk;
	unsigned int		imr;
	void __iomem		*gsbi_base;
	int			is_uartdm;
	unsigned int		old_snap_state;
};

static inline void wait_for_xmitr(struct uart_port *port)
{
	while (!(msm_read(port, UART_SR) & UART_SR_TX_EMPTY)) {
		if (msm_read(port, UART_ISR) & UART_ISR_TX_READY)
			break;
		udelay(1);
	}
	msm_write(port, UART_CR_CMD_RESET_TX_READY, UART_CR);
}

>>>>>>> refs/remotes/origin/master
static void msm_stop_tx(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

<<<<<<< HEAD
<<<<<<< HEAD
	clk_enable(msm_port->clk);

	msm_port->imr &= ~UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, UART_IMR);

	clk_disable(msm_port->clk);
=======
	msm_port->imr &= ~UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, UART_IMR);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	msm_port->imr &= ~UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, UART_IMR);
>>>>>>> refs/remotes/origin/master
}

static void msm_start_tx(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

<<<<<<< HEAD
<<<<<<< HEAD
	clk_enable(msm_port->clk);

	msm_port->imr |= UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, UART_IMR);

	clk_disable(msm_port->clk);
=======
	msm_port->imr |= UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, UART_IMR);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	msm_port->imr |= UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, UART_IMR);
>>>>>>> refs/remotes/origin/master
}

static void msm_stop_rx(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

<<<<<<< HEAD
<<<<<<< HEAD
	clk_enable(msm_port->clk);

	msm_port->imr &= ~(UART_IMR_RXLEV | UART_IMR_RXSTALE);
	msm_write(port, msm_port->imr, UART_IMR);

	clk_disable(msm_port->clk);
=======
	msm_port->imr &= ~(UART_IMR_RXLEV | UART_IMR_RXSTALE);
	msm_write(port, msm_port->imr, UART_IMR);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	msm_port->imr &= ~(UART_IMR_RXLEV | UART_IMR_RXSTALE);
	msm_write(port, msm_port->imr, UART_IMR);
>>>>>>> refs/remotes/origin/master
}

static void msm_enable_ms(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

<<<<<<< HEAD
<<<<<<< HEAD
	clk_enable(msm_port->clk);

	msm_port->imr |= UART_IMR_DELTA_CTS;
	msm_write(port, msm_port->imr, UART_IMR);

	clk_disable(msm_port->clk);
=======
	msm_port->imr |= UART_IMR_DELTA_CTS;
	msm_write(port, msm_port->imr, UART_IMR);
>>>>>>> refs/remotes/origin/cm-10.0
}

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
/* turn clock off if TX buffer is empty, otherwise reschedule */
static enum hrtimer_restart msm_serial_clock_off(struct hrtimer *timer) {
	struct msm_port *msm_port = container_of(timer, struct msm_port,
						 clk_off_timer);
	struct uart_port *port = &msm_port->uart;
	struct circ_buf *xmit = &port->state->xmit;
	unsigned long flags;
	int ret = HRTIMER_NORESTART;

	spin_lock_irqsave(&port->lock, flags);

	if (msm_port->clk_state == MSM_CLK_REQUEST_OFF) {
		if (uart_circ_empty(xmit)) {
			struct msm_port *msm_port = UART_TO_MSM(port);
			clk_disable(msm_port->clk);
			msm_port->clk_state = MSM_CLK_OFF;
#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
			if (use_low_power_wakeup(msm_port)) {
				msm_port->wakeup.ignore = 1;
				enable_irq(msm_port->wakeup.irq);
			}
#endif
		} else {
			hrtimer_forward_now(timer, msm_port->clk_off_delay);
			ret = HRTIMER_RESTART;
		}
	}

	spin_unlock_irqrestore(&port->lock, flags);

	return HRTIMER_NORESTART;
}

/* request to turn off uart clock once pending TX is flushed */
void msm_serial_clock_request_off(struct uart_port *port) {
	unsigned long flags;
	struct msm_port *msm_port = UART_TO_MSM(port);

	spin_lock_irqsave(&port->lock, flags);
	if (msm_port->clk_state == MSM_CLK_ON) {
		msm_port->clk_state = MSM_CLK_REQUEST_OFF;
		/* turn off TX later. unfortunately not all msm uart's have a
		 * TXDONE available, and TXLEV does not wait until completely
		 * flushed, so a timer is our only option
		 */
		hrtimer_start(&msm_port->clk_off_timer,
			      msm_port->clk_off_delay, HRTIMER_MODE_REL);
	}
	spin_unlock_irqrestore(&port->lock, flags);
}

/* request to immediately turn on uart clock.
 * ignored if there is a pending off request, unless force = 1.
 */
void msm_serial_clock_on(struct uart_port *port, int force) {
	unsigned long flags;
	struct msm_port *msm_port = UART_TO_MSM(port);

	spin_lock_irqsave(&port->lock, flags);

	switch (msm_port->clk_state) {
	case MSM_CLK_OFF:
		clk_enable(msm_port->clk);
#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
		if (use_low_power_wakeup(msm_port))
			disable_irq(msm_port->wakeup.irq);
#endif
		force = 1;
	case MSM_CLK_REQUEST_OFF:
		if (force) {
			hrtimer_try_to_cancel(&msm_port->clk_off_timer);
			msm_port->clk_state = MSM_CLK_ON;
		}
		break;
	case MSM_CLK_ON: break;
	case MSM_CLK_PORT_OFF: break;
	}

	spin_unlock_irqrestore(&port->lock, flags);
}
#endif

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
static irqreturn_t msm_rx_irq(int irq, void *dev_id)
{
	unsigned long flags;
	struct uart_port *port = dev_id;
	struct msm_port *msm_port = UART_TO_MSM(port);
	int inject_wakeup = 0;

	spin_lock_irqsave(&port->lock, flags);

	if (msm_port->clk_state == MSM_CLK_OFF) {
		/* ignore the first irq - it is a pending irq that occured
		 * before enable_irq() */
		if (msm_port->wakeup.ignore)
			msm_port->wakeup.ignore = 0;
		else
			inject_wakeup = 1;
	}

	msm_serial_clock_on(port, 0);

	/* we missed an rx while asleep - it must be a wakeup indicator
	 */
	if (inject_wakeup) {
		struct tty_struct *tty = port->state->port.tty;
		tty_insert_flip_char(tty, WAKE_UP_IND, TTY_NORMAL);
		tty_flip_buffer_push(tty);
	}

	spin_unlock_irqrestore(&port->lock, flags);
	return IRQ_HANDLED;
}
#endif

static void handle_rx(struct uart_port *port)
{
	struct tty_struct *tty = port->state->port.tty;
=======
	msm_port->imr |= UART_IMR_DELTA_CTS;
	msm_write(port, msm_port->imr, UART_IMR);
}

static void handle_rx_dm(struct uart_port *port, unsigned int misr)
{
	struct tty_port *tport = &port->state->port;
	unsigned int sr;
	int count = 0;
	struct msm_port *msm_port = UART_TO_MSM(port);

	if ((msm_read(port, UART_SR) & UART_SR_OVERRUN)) {
		port->icount.overrun++;
		tty_insert_flip_char(tport, 0, TTY_OVERRUN);
		msm_write(port, UART_CR_CMD_RESET_ERR, UART_CR);
	}

	if (misr & UART_IMR_RXSTALE) {
		count = msm_read(port, UARTDM_RX_TOTAL_SNAP) -
			msm_port->old_snap_state;
		msm_port->old_snap_state = 0;
	} else {
		count = 4 * (msm_read(port, UART_RFWR));
		msm_port->old_snap_state += count;
	}

	/* TODO: Precise error reporting */

	port->icount.rx += count;

	while (count > 0) {
		unsigned int c;

		sr = msm_read(port, UART_SR);
		if ((sr & UART_SR_RX_READY) == 0) {
			msm_port->old_snap_state -= count;
			break;
		}
		c = msm_read(port, UARTDM_RF);
		if (sr & UART_SR_RX_BREAK) {
			port->icount.brk++;
			if (uart_handle_break(port))
				continue;
		} else if (sr & UART_SR_PAR_FRAME_ERR)
			port->icount.frame++;

		/* TODO: handle sysrq */
		tty_insert_flip_string(tport, (char *)&c,
				       (count > 4) ? 4 : count);
		count -= 4;
	}

	spin_unlock(&port->lock);
	tty_flip_buffer_push(tport);
	spin_lock(&port->lock);

	if (misr & (UART_IMR_RXSTALE))
		msm_write(port, UART_CR_CMD_RESET_STALE_INT, UART_CR);
	msm_write(port, 0xFFFFFF, UARTDM_DMRX);
	msm_write(port, UART_CR_CMD_STALE_EVENT_ENABLE, UART_CR);
}

static void handle_rx(struct uart_port *port)
{
	struct tty_port *tport = &port->state->port;
>>>>>>> refs/remotes/origin/master
	unsigned int sr;

	/*
	 * Handle overrun. My understanding of the hardware is that overrun
	 * is not tied to the RX buffer, so we handle the case out of band.
	 */
	if ((msm_read(port, UART_SR) & UART_SR_OVERRUN)) {
		port->icount.overrun++;
<<<<<<< HEAD
		tty_insert_flip_char(tty, 0, TTY_OVERRUN);
=======
		tty_insert_flip_char(tport, 0, TTY_OVERRUN);
>>>>>>> refs/remotes/origin/master
		msm_write(port, UART_CR_CMD_RESET_ERR, UART_CR);
	}

	/* and now the main RX loop */
	while ((sr = msm_read(port, UART_SR)) & UART_SR_RX_READY) {
		unsigned int c;
		char flag = TTY_NORMAL;

		c = msm_read(port, UART_RF);

		if (sr & UART_SR_RX_BREAK) {
			port->icount.brk++;
			if (uart_handle_break(port))
				continue;
		} else if (sr & UART_SR_PAR_FRAME_ERR) {
			port->icount.frame++;
		} else {
			port->icount.rx++;
		}

		/* Mask conditions we're ignorning. */
		sr &= port->read_status_mask;

		if (sr & UART_SR_RX_BREAK) {
			flag = TTY_BREAK;
		} else if (sr & UART_SR_PAR_FRAME_ERR) {
			flag = TTY_FRAME;
		}

		if (!uart_handle_sysrq_char(port, c))
<<<<<<< HEAD
			tty_insert_flip_char(tty, c, flag);
	}

	tty_flip_buffer_push(tty);
=======
			tty_insert_flip_char(tport, c, flag);
	}

	spin_unlock(&port->lock);
	tty_flip_buffer_push(tport);
	spin_lock(&port->lock);
}

static void reset_dm_count(struct uart_port *port, int count)
{
	wait_for_xmitr(port);
	msm_write(port, count, UARTDM_NCF_TX);
	msm_read(port, UARTDM_NCF_TX);
>>>>>>> refs/remotes/origin/master
}

static void handle_tx(struct uart_port *port)
{
	struct circ_buf *xmit = &port->state->xmit;
	struct msm_port *msm_port = UART_TO_MSM(port);
<<<<<<< HEAD
	int sent_tx;

	if (port->x_char) {
		msm_write(port, port->x_char, UART_TF);
		port->icount.tx++;
		port->x_char = 0;
	}

	while (msm_read(port, UART_SR) & UART_SR_TX_READY) {
		if (uart_circ_empty(xmit)) {
			/* disable tx interrupts */
			msm_port->imr &= ~UART_IMR_TXLEV;
			msm_write(port, msm_port->imr, UART_IMR);
			break;
		}

		msm_write(port, xmit->buf[xmit->tail], UART_TF);

		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
		sent_tx = 1;
	}

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	if (sent_tx && msm_port->clk_state == MSM_CLK_REQUEST_OFF)
		/* new TX - restart the timer */
		if (hrtimer_try_to_cancel(&msm_port->clk_off_timer) == 1)
			hrtimer_start(&msm_port->clk_off_timer,
				msm_port->clk_off_delay, HRTIMER_MODE_REL);
#endif
=======
	unsigned int tx_count, num_chars;
	unsigned int tf_pointer = 0;

	tx_count = uart_circ_chars_pending(xmit);
	tx_count = min3(tx_count, (unsigned int)UART_XMIT_SIZE - xmit->tail,
			port->fifosize);

	if (port->x_char) {
		if (msm_port->is_uartdm)
			reset_dm_count(port, tx_count + 1);

		msm_write(port, port->x_char,
			  msm_port->is_uartdm ? UARTDM_TF : UART_TF);
		port->icount.tx++;
		port->x_char = 0;
	} else if (tx_count && msm_port->is_uartdm) {
		reset_dm_count(port, tx_count);
	}

	while (tf_pointer < tx_count) {
		int i;
		char buf[4] = { 0 };
		unsigned int *bf = (unsigned int *)&buf;

		if (!(msm_read(port, UART_SR) & UART_SR_TX_READY))
			break;

		if (msm_port->is_uartdm)
			num_chars = min(tx_count - tf_pointer,
					(unsigned int)sizeof(buf));
		else
			num_chars = 1;

		for (i = 0; i < num_chars; i++) {
			buf[i] = xmit->buf[xmit->tail + i];
			port->icount.tx++;
		}

		msm_write(port, *bf, msm_port->is_uartdm ? UARTDM_TF : UART_TF);
		xmit->tail = (xmit->tail + num_chars) & (UART_XMIT_SIZE - 1);
		tf_pointer += num_chars;
	}

	/* disable tx interrupts if nothing more to send */
	if (uart_circ_empty(xmit))
		msm_stop_tx(port);
>>>>>>> refs/remotes/origin/master

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);
}

static void handle_delta_cts(struct uart_port *port)
{
	msm_write(port, UART_CR_CMD_RESET_CTS, UART_CR);
	port->icount.cts++;
	wake_up_interruptible(&port->state->port.delta_msr_wait);
}

static irqreturn_t msm_irq(int irq, void *dev_id)
{
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> refs/remotes/origin/master
	struct uart_port *port = dev_id;
	struct msm_port *msm_port = UART_TO_MSM(port);
	unsigned int misr;

<<<<<<< HEAD
	spin_lock_irqsave(&port->lock, flags);
<<<<<<< HEAD
	clk_enable(msm_port->clk);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	misr = msm_read(port, UART_MISR);
	msm_write(port, 0, UART_IMR); /* disable interrupt */

	if (misr & (UART_IMR_RXLEV | UART_IMR_RXSTALE))
		handle_rx(port);
=======
	spin_lock(&port->lock);
	misr = msm_read(port, UART_MISR);
	msm_write(port, 0, UART_IMR); /* disable interrupt */

	if (misr & (UART_IMR_RXLEV | UART_IMR_RXSTALE)) {
		if (msm_port->is_uartdm)
			handle_rx_dm(port, misr);
		else
			handle_rx(port);
	}
>>>>>>> refs/remotes/origin/master
	if (misr & UART_IMR_TXLEV)
		handle_tx(port);
	if (misr & UART_IMR_DELTA_CTS)
		handle_delta_cts(port);

	msm_write(port, msm_port->imr, UART_IMR); /* restore interrupt */
<<<<<<< HEAD
<<<<<<< HEAD
	clk_disable(msm_port->clk);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock_irqrestore(&port->lock, flags);
=======
	spin_unlock(&port->lock);
>>>>>>> refs/remotes/origin/master

	return IRQ_HANDLED;
}

static unsigned int msm_tx_empty(struct uart_port *port)
{
<<<<<<< HEAD
	unsigned int ret;
<<<<<<< HEAD
	struct msm_port *msm_port = UART_TO_MSM(port);

	clk_enable(msm_port->clk);
	ret = (msm_read(port, UART_SR) & UART_SR_TX_EMPTY) ? TIOCSER_TEMT : 0;
	clk_disable(msm_port->clk);

=======

	ret = (msm_read(port, UART_SR) & UART_SR_TX_EMPTY) ? TIOCSER_TEMT : 0;
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
=======
	return (msm_read(port, UART_SR) & UART_SR_TX_EMPTY) ? TIOCSER_TEMT : 0;
>>>>>>> refs/remotes/origin/master
}

static unsigned int msm_get_mctrl(struct uart_port *port)
{
	return TIOCM_CAR | TIOCM_CTS | TIOCM_DSR | TIOCM_RTS;
}

<<<<<<< HEAD
static void msm_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	unsigned int mr;
<<<<<<< HEAD
	struct msm_port *msm_port = UART_TO_MSM(port);

	clk_enable(msm_port->clk);
=======
>>>>>>> refs/remotes/origin/cm-10.0

=======

static void msm_reset(struct uart_port *port)
{
	/* reset everything */
	msm_write(port, UART_CR_CMD_RESET_RX, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_TX, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_ERR, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_BREAK_INT, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_CTS, UART_CR);
	msm_write(port, UART_CR_CMD_SET_RFR, UART_CR);
}

static void msm_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	unsigned int mr;
>>>>>>> refs/remotes/origin/master
	mr = msm_read(port, UART_MR1);

	if (!(mctrl & TIOCM_RTS)) {
		mr &= ~UART_MR1_RX_RDY_CTL;
		msm_write(port, mr, UART_MR1);
		msm_write(port, UART_CR_CMD_RESET_RFR, UART_CR);
	} else {
		mr |= UART_MR1_RX_RDY_CTL;
		msm_write(port, mr, UART_MR1);
	}
<<<<<<< HEAD
<<<<<<< HEAD

	clk_disable(msm_port->clk);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void msm_break_ctl(struct uart_port *port, int break_ctl)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct msm_port *msm_port = UART_TO_MSM(port);

	clk_enable(msm_port->clk);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (break_ctl)
		msm_write(port, UART_CR_CMD_START_BREAK, UART_CR);
	else
		msm_write(port, UART_CR_CMD_STOP_BREAK, UART_CR);
<<<<<<< HEAD
<<<<<<< HEAD

	clk_disable(msm_port->clk);
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

static void msm_set_baud_rate(struct uart_port *port, unsigned int baud)
{
	unsigned int baud_code, rxstale, watermark;

	switch (baud) {
	case 300:
		baud_code = UART_CSR_300;
		rxstale = 1;
		break;
	case 600:
		baud_code = UART_CSR_600;
		rxstale = 1;
		break;
	case 1200:
		baud_code = UART_CSR_1200;
		rxstale = 1;
		break;
	case 2400:
		baud_code = UART_CSR_2400;
		rxstale = 1;
		break;
	case 4800:
		baud_code = UART_CSR_4800;
		rxstale = 1;
		break;
	case 9600:
		baud_code = UART_CSR_9600;
		rxstale = 2;
		break;
	case 14400:
		baud_code = UART_CSR_14400;
		rxstale = 3;
		break;
	case 19200:
		baud_code = UART_CSR_19200;
		rxstale = 4;
		break;
	case 28800:
		baud_code = UART_CSR_28800;
		rxstale = 6;
		break;
	case 38400:
		baud_code = UART_CSR_38400;
		rxstale = 8;
		break;
	case 57600:
		baud_code = UART_CSR_57600;
		rxstale = 16;
		break;
	case 115200:
	default:
		baud_code = UART_CSR_115200;
		rxstale = 31;
		break;
	}

	msm_write(port, baud_code, UART_CSR);

	/* RX stale watermark */
=======
}

struct msm_baud_map {
	u16	divisor;
	u8	code;
	u8	rxstale;
};

static const struct msm_baud_map *
msm_find_best_baud(struct uart_port *port, unsigned int baud)
{
	unsigned int i, divisor;
	const struct msm_baud_map *entry;
	static const struct msm_baud_map table[] = {
		{ 1536, 0x00,  1 },
		{  768, 0x11,  1 },
		{  384, 0x22,  1 },
		{  192, 0x33,  1 },
		{   96, 0x44,  1 },
		{   48, 0x55,  1 },
		{   32, 0x66,  1 },
		{   24, 0x77,  1 },
		{   16, 0x88,  1 },
		{   12, 0x99,  6 },
		{    8, 0xaa,  6 },
		{    6, 0xbb,  6 },
		{    4, 0xcc,  6 },
		{    3, 0xdd,  8 },
		{    2, 0xee, 16 },
		{    1, 0xff, 31 },
	};

	divisor = uart_get_divisor(port, baud);

	for (i = 0, entry = table; i < ARRAY_SIZE(table); i++, entry++)
		if (entry->divisor <= divisor)
			break;

	return entry; /* Default to smallest divider */
}

static int msm_set_baud_rate(struct uart_port *port, unsigned int baud)
{
	unsigned int rxstale, watermark;
	struct msm_port *msm_port = UART_TO_MSM(port);
	const struct msm_baud_map *entry;

	entry = msm_find_best_baud(port, baud);

	if (msm_port->is_uartdm)
		msm_write(port, UART_CR_CMD_RESET_RX, UART_CR);

	msm_write(port, entry->code, UART_CSR);

	/* RX stale watermark */
	rxstale = entry->rxstale;
>>>>>>> refs/remotes/origin/master
	watermark = UART_IPR_STALE_LSB & rxstale;
	watermark |= UART_IPR_RXSTALE_LAST;
	watermark |= UART_IPR_STALE_TIMEOUT_MSB & (rxstale << 2);
	msm_write(port, watermark, UART_IPR);

	/* set RX watermark */
	watermark = (port->fifosize * 3) / 4;
	msm_write(port, watermark, UART_RFWR);

	/* set TX watermark */
	msm_write(port, 10, UART_TFWR);
<<<<<<< HEAD
}

static void msm_reset(struct uart_port *port)
{
	/* reset everything */
	msm_write(port, UART_CR_CMD_RESET_RX, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_TX, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_ERR, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_BREAK_INT, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_CTS, UART_CR);
	msm_write(port, UART_CR_CMD_SET_RFR, UART_CR);
}

static void msm_init_clock(struct uart_port *port)
{
<<<<<<< HEAD
	struct msm_port *msm_port = UART_TO_MSM(port);

	clk_enable(msm_port->clk);
=======
	int ret;
	struct msm_port *msm_port = UART_TO_MSM(port);

	ret = clk_prepare_enable(msm_port->clk);
	if (ret) {
		pr_err("%s(): Can't enable uartclk. ret:%d\n", __func__, ret);
		return;
	}
>>>>>>> refs/remotes/origin/cm-10.0

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	msm_port->clk_state = MSM_CLK_ON;
#endif

<<<<<<< HEAD
	if (port->uartclk == 19200000) {
=======
	if (msm_port->uim) {
		msm_write(port,
			UART_SIM_CFG_UIM_TX_MODE |
			UART_SIM_CFG_UIM_RX_MODE |
			UART_SIM_CFG_STOP_BIT_LEN_N(1) |
			UART_SIM_CFG_SIM_CLK_ON |
			UART_SIM_CFG_SIM_CLK_STOP_HIGH |
			UART_SIM_CFG_SIM_SEL,
			UART_SIM_CFG);

		/* (TCXO * 16) / (5 * 372) = TCXO * 16 / 1860 */
		msm_write(port, 0x08, UART_MREG);
		msm_write(port, 0x19, UART_NREG);
		msm_write(port, 0xe8, UART_DREG);
		msm_write(port, 0x0e, UART_MNDREG);
	} else if (port->uartclk == 19200000) {
>>>>>>> refs/remotes/origin/cm-10.0
		/* clock is TCXO (19.2MHz) */
		msm_write(port, 0x06, UART_MREG);
		msm_write(port, 0xF1, UART_NREG);
		msm_write(port, 0x0F, UART_DREG);
		msm_write(port, 0x1A, UART_MNDREG);
	} else {
		/* clock must be TCXO/4 */
		msm_write(port, 0x18, UART_MREG);
		msm_write(port, 0xF6, UART_NREG);
		msm_write(port, 0x0F, UART_DREG);
		msm_write(port, 0x0A, UART_MNDREG);
	}
}

static void msm_deinit_clock(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	if (msm_port->clk_state != MSM_CLK_OFF)
		clk_disable(msm_port->clk);
	msm_port->clk_state = MSM_CLK_PORT_OFF;
#else
<<<<<<< HEAD
	clk_disable(msm_port->clk);
=======
	clk_disable_unprepare(msm_port->clk);
>>>>>>> refs/remotes/origin/cm-10.0
#endif

}
=======

	if (msm_port->is_uartdm) {
		msm_write(port, UART_CR_CMD_RESET_STALE_INT, UART_CR);
		msm_write(port, 0xFFFFFF, UARTDM_DMRX);
		msm_write(port, UART_CR_CMD_STALE_EVENT_ENABLE, UART_CR);
	}

	return baud;
}


static void msm_init_clock(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

	clk_prepare_enable(msm_port->clk);
	clk_prepare_enable(msm_port->pclk);
	msm_serial_set_mnd_regs(port);
}

>>>>>>> refs/remotes/origin/master
static int msm_startup(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);
	unsigned int data, rfr_level;
	int ret;

	snprintf(msm_port->name, sizeof(msm_port->name),
		 "msm_serial%d", port->line);

	ret = request_irq(port->irq, msm_irq, IRQF_TRIGGER_HIGH,
			  msm_port->name, port);
	if (unlikely(ret))
		return ret;

<<<<<<< HEAD

#ifndef CONFIG_PM_RUNTIME
	msm_init_clock(port);
#endif
	pm_runtime_get_sync(port->dev);
=======
	msm_init_clock(port);
>>>>>>> refs/remotes/origin/master

	if (likely(port->fifosize > 12))
		rfr_level = port->fifosize - 12;
	else
		rfr_level = port->fifosize;

	/* set automatic RFR level */
	data = msm_read(port, UART_MR1);
	data &= ~UART_MR1_AUTO_RFR_LEVEL1;
	data &= ~UART_MR1_AUTO_RFR_LEVEL0;
	data |= UART_MR1_AUTO_RFR_LEVEL1 & (rfr_level << 2);
	data |= UART_MR1_AUTO_RFR_LEVEL0 & rfr_level;
	msm_write(port, data, UART_MR1);

	/* make sure that RXSTALE count is non-zero */
	data = msm_read(port, UART_IPR);
	if (unlikely(!data)) {
		data |= UART_IPR_RXSTALE_LAST;
		data |= UART_IPR_STALE_LSB;
		msm_write(port, data, UART_IPR);
	}

<<<<<<< HEAD
	msm_reset(port);

	msm_write(port, 0x05, UART_CR);	/* enable TX & RX */
=======
	data = 0;
	if (!port->cons || (port->cons && !(port->cons->flags & CON_ENABLED))) {
		msm_write(port, UART_CR_CMD_PROTECTION_EN, UART_CR);
		msm_reset(port);
		data = UART_CR_TX_ENABLE;
	}

	data |= UART_CR_RX_ENABLE;
	msm_write(port, data, UART_CR);	/* enable TX & RX */

	/* Make sure IPR is not 0 to start with*/
	if (msm_port->is_uartdm)
		msm_write(port, UART_IPR_STALE_LSB, UART_IPR);
>>>>>>> refs/remotes/origin/master

	/* turn on RX and CTS interrupts */
	msm_port->imr = UART_IMR_RXLEV | UART_IMR_RXSTALE |
			UART_IMR_CURRENT_CTS;
<<<<<<< HEAD
	msm_write(port, msm_port->imr, UART_IMR);

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	if (use_low_power_wakeup(msm_port)) {
		ret = irq_set_irq_wake(msm_port->wakeup.irq, 1);
		if (unlikely(ret))
			return ret;
		ret = request_irq(msm_port->wakeup.irq, msm_rx_irq,
				  IRQF_TRIGGER_FALLING,
				  "msm_serial_wakeup", msm_port);
		if (unlikely(ret))
			return ret;
		disable_irq(msm_port->wakeup.irq);
	}
#endif

=======

	if (msm_port->is_uartdm) {
		msm_write(port, 0xFFFFFF, UARTDM_DMRX);
		msm_write(port, UART_CR_CMD_RESET_STALE_INT, UART_CR);
		msm_write(port, UART_CR_CMD_STALE_EVENT_ENABLE, UART_CR);
	}

	msm_write(port, msm_port->imr, UART_IMR);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static void msm_shutdown(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

<<<<<<< HEAD
<<<<<<< HEAD
	clk_enable(msm_port->clk);
=======
	if (msm_port->uim)
		msm_write(port,
			UART_SIM_CFG_SIM_CLK_STOP_HIGH,
			UART_SIM_CFG);
>>>>>>> refs/remotes/origin/cm-10.0

	msm_port->imr = 0;
	msm_write(port, 0, UART_IMR); /* disable interrupts */

<<<<<<< HEAD
	clk_disable(msm_port->clk);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	free_irq(port->irq, port);

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	if (use_low_power_wakeup(msm_port)) {
		irq_set_irq_wake(msm_port->wakeup.irq, 0);
		free_irq(msm_port->wakeup.irq, msm_port);
	}
#endif
#ifndef CONFIG_PM_RUNTIME
	msm_deinit_clock(port);
#endif
	pm_runtime_put_sync(port->dev);
=======
	msm_port->imr = 0;
	msm_write(port, 0, UART_IMR); /* disable interrupts */

	clk_disable_unprepare(msm_port->clk);

	free_irq(port->irq, port);
>>>>>>> refs/remotes/origin/master
}

static void msm_set_termios(struct uart_port *port, struct ktermios *termios,
			    struct ktermios *old)
{
	unsigned long flags;
	unsigned int baud, mr;
<<<<<<< HEAD
<<<<<<< HEAD
	struct msm_port *msm_port = UART_TO_MSM(port);

	spin_lock_irqsave(&port->lock, flags);
	clk_enable(msm_port->clk);
=======

	if (!termios->c_cflag)
		return;

	spin_lock_irqsave(&port->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0

	/* calculate and set baud rate */
	baud = uart_get_baud_rate(port, termios, old, 300, 115200);
	msm_set_baud_rate(port, baud);
=======

	spin_lock_irqsave(&port->lock, flags);

	/* calculate and set baud rate */
	baud = uart_get_baud_rate(port, termios, old, 300, 115200);
	baud = msm_set_baud_rate(port, baud);
	if (tty_termios_baud_rate(termios))
		tty_termios_encode_baud_rate(termios, baud, baud);
>>>>>>> refs/remotes/origin/master

	/* calculate parity */
	mr = msm_read(port, UART_MR2);
	mr &= ~UART_MR2_PARITY_MODE;
	if (termios->c_cflag & PARENB) {
		if (termios->c_cflag & PARODD)
			mr |= UART_MR2_PARITY_MODE_ODD;
		else if (termios->c_cflag & CMSPAR)
			mr |= UART_MR2_PARITY_MODE_SPACE;
		else
			mr |= UART_MR2_PARITY_MODE_EVEN;
	}

	/* calculate bits per char */
	mr &= ~UART_MR2_BITS_PER_CHAR;
	switch (termios->c_cflag & CSIZE) {
	case CS5:
		mr |= UART_MR2_BITS_PER_CHAR_5;
		break;
	case CS6:
		mr |= UART_MR2_BITS_PER_CHAR_6;
		break;
	case CS7:
		mr |= UART_MR2_BITS_PER_CHAR_7;
		break;
	case CS8:
	default:
		mr |= UART_MR2_BITS_PER_CHAR_8;
		break;
	}

	/* calculate stop bits */
	mr &= ~(UART_MR2_STOP_BIT_LEN_ONE | UART_MR2_STOP_BIT_LEN_TWO);
	if (termios->c_cflag & CSTOPB)
		mr |= UART_MR2_STOP_BIT_LEN_TWO;
	else
		mr |= UART_MR2_STOP_BIT_LEN_ONE;

	/* set parity, bits per char, and stop bit */
	msm_write(port, mr, UART_MR2);

	/* calculate and set hardware flow control */
	mr = msm_read(port, UART_MR1);
	mr &= ~(UART_MR1_CTS_CTL | UART_MR1_RX_RDY_CTL);
	if (termios->c_cflag & CRTSCTS) {
		mr |= UART_MR1_CTS_CTL;
		mr |= UART_MR1_RX_RDY_CTL;
	}
	msm_write(port, mr, UART_MR1);

	/* Configure status bits to ignore based on termio flags. */
	port->read_status_mask = 0;
	if (termios->c_iflag & INPCK)
		port->read_status_mask |= UART_SR_PAR_FRAME_ERR;
	if (termios->c_iflag & (BRKINT | PARMRK))
		port->read_status_mask |= UART_SR_RX_BREAK;

	uart_update_timeout(port, termios->c_cflag, baud);
<<<<<<< HEAD
<<<<<<< HEAD

	clk_disable(msm_port->clk);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&port->lock, flags);
}

static const char *msm_type(struct uart_port *port)
{
	return "MSM";
}

static void msm_release_port(struct uart_port *port)
{
	struct platform_device *pdev = to_platform_device(port->dev);
<<<<<<< HEAD
	struct resource *resource;
	resource_size_t size;

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return;
	size = resource->end - resource->start + 1;
=======
	struct msm_port *msm_port = UART_TO_MSM(port);
	struct resource *uart_resource;
	struct resource *gsbi_resource;
	resource_size_t size;

	uart_resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!uart_resource))
		return;
	size = resource_size(uart_resource);
>>>>>>> refs/remotes/origin/master

	release_mem_region(port->mapbase, size);
	iounmap(port->membase);
	port->membase = NULL;
<<<<<<< HEAD
=======

	if (msm_port->gsbi_base) {
		writel_relaxed(GSBI_PROTOCOL_IDLE,
				msm_port->gsbi_base + GSBI_CONTROL);

		gsbi_resource = platform_get_resource(pdev, IORESOURCE_MEM, 1);
		if (unlikely(!gsbi_resource))
			return;

		size = resource_size(gsbi_resource);
		release_mem_region(gsbi_resource->start, size);
		iounmap(msm_port->gsbi_base);
		msm_port->gsbi_base = NULL;
	}
>>>>>>> refs/remotes/origin/master
}

static int msm_request_port(struct uart_port *port)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(port->dev);
	struct resource *resource;
	resource_size_t size;

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return -ENXIO;
	size = resource->end - resource->start + 1;

	if (unlikely(!request_mem_region(port->mapbase, size, "msm_serial")))
=======
	struct msm_port *msm_port = UART_TO_MSM(port);
	struct platform_device *pdev = to_platform_device(port->dev);
	struct resource *uart_resource;
	struct resource *gsbi_resource;
	resource_size_t size;
	int ret;

	uart_resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!uart_resource))
		return -ENXIO;

	size = resource_size(uart_resource);

	if (!request_mem_region(port->mapbase, size, "msm_serial"))
>>>>>>> refs/remotes/origin/master
		return -EBUSY;

	port->membase = ioremap(port->mapbase, size);
	if (!port->membase) {
<<<<<<< HEAD
		release_mem_region(port->mapbase, size);
		return -EBUSY;
	}

	return 0;
=======
		ret = -EBUSY;
		goto fail_release_port;
	}

	gsbi_resource = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	/* Is this a GSBI-based port? */
	if (gsbi_resource) {
		size = resource_size(gsbi_resource);

		if (!request_mem_region(gsbi_resource->start, size,
						 "msm_serial")) {
			ret = -EBUSY;
			goto fail_release_port_membase;
		}

		msm_port->gsbi_base = ioremap(gsbi_resource->start, size);
		if (!msm_port->gsbi_base) {
			ret = -EBUSY;
			goto fail_release_gsbi;
		}
	}

	return 0;

fail_release_gsbi:
	release_mem_region(gsbi_resource->start, size);
fail_release_port_membase:
	iounmap(port->membase);
fail_release_port:
	release_mem_region(port->mapbase, size);
	return ret;
>>>>>>> refs/remotes/origin/master
}

static void msm_config_port(struct uart_port *port, int flags)
{
<<<<<<< HEAD
	if (flags & UART_CONFIG_TYPE) {
		port->type = PORT_MSM;
		msm_request_port(port);
	}
=======
	struct msm_port *msm_port = UART_TO_MSM(port);
	int ret;
	if (flags & UART_CONFIG_TYPE) {
		port->type = PORT_MSM;
		ret = msm_request_port(port);
		if (ret)
			return;
	}
	if (msm_port->gsbi_base)
		writel_relaxed(GSBI_PROTOCOL_UART,
				msm_port->gsbi_base + GSBI_CONTROL);
>>>>>>> refs/remotes/origin/master
}

static int msm_verify_port(struct uart_port *port, struct serial_struct *ser)
{
	if (unlikely(ser->type != PORT_UNKNOWN && ser->type != PORT_MSM))
		return -EINVAL;
	if (unlikely(port->irq != ser->irq))
		return -EINVAL;
	return 0;
}

static void msm_power(struct uart_port *port, unsigned int state,
		      unsigned int oldstate)
{
<<<<<<< HEAD
<<<<<<< HEAD
#ifndef CONFIG_SERIAL_MSM_CLOCK_CONTROL
=======
	int ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct msm_port *msm_port = UART_TO_MSM(port);

	switch (state) {
	case 0:
<<<<<<< HEAD
<<<<<<< HEAD
		clk_enable(msm_port->clk);
		break;
	case 3:
		clk_disable(msm_port->clk);
		break;
	default:
		printk(KERN_ERR "msm_serial: Unknown PM state %d\n", state);
	}
#endif
=======
		ret = clk_prepare_enable(msm_port->clk);
		if (ret)
			pr_err("msm_serial: %s(): Can't enable uartclk.\n",
						__func__);
		break;
	case 3:
		clk_disable_unprepare(msm_port->clk);
		break;
	default:
		pr_err("msm_serial: %s(): Unknown PM state %d\n",
						__func__, state);
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
		clk_prepare_enable(msm_port->clk);
		clk_prepare_enable(msm_port->pclk);
		break;
	case 3:
		clk_disable_unprepare(msm_port->clk);
		clk_disable_unprepare(msm_port->pclk);
		break;
	default:
		printk(KERN_ERR "msm_serial: Unknown PM state %d\n", state);
	}
>>>>>>> refs/remotes/origin/master
}

static struct uart_ops msm_uart_pops = {
	.tx_empty = msm_tx_empty,
	.set_mctrl = msm_set_mctrl,
	.get_mctrl = msm_get_mctrl,
	.stop_tx = msm_stop_tx,
	.start_tx = msm_start_tx,
	.stop_rx = msm_stop_rx,
	.enable_ms = msm_enable_ms,
	.break_ctl = msm_break_ctl,
	.startup = msm_startup,
	.shutdown = msm_shutdown,
	.set_termios = msm_set_termios,
	.type = msm_type,
	.release_port = msm_release_port,
	.request_port = msm_request_port,
	.config_port = msm_config_port,
	.verify_port = msm_verify_port,
	.pm = msm_power,
};

static struct msm_port msm_uart_ports[] = {
	{
		.uart = {
			.iotype = UPIO_MEM,
			.ops = &msm_uart_pops,
			.flags = UPF_BOOT_AUTOCONF,
<<<<<<< HEAD
			.fifosize = 512,
=======
			.fifosize = 64,
>>>>>>> refs/remotes/origin/master
			.line = 0,
		},
	},
	{
		.uart = {
			.iotype = UPIO_MEM,
			.ops = &msm_uart_pops,
			.flags = UPF_BOOT_AUTOCONF,
<<<<<<< HEAD
			.fifosize = 512,
=======
			.fifosize = 64,
>>>>>>> refs/remotes/origin/master
			.line = 1,
		},
	},
	{
		.uart = {
			.iotype = UPIO_MEM,
			.ops = &msm_uart_pops,
			.flags = UPF_BOOT_AUTOCONF,
			.fifosize = 64,
			.line = 2,
		},
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
#define UART_NR	ARRAY_SIZE(msm_uart_ports)

=======
#define UART_NR 256
>>>>>>> refs/remotes/origin/cm-10.0
static inline struct uart_port * get_port_from_line(unsigned int line)
=======
#define UART_NR	ARRAY_SIZE(msm_uart_ports)

static inline struct uart_port *get_port_from_line(unsigned int line)
>>>>>>> refs/remotes/origin/master
{
	return &msm_uart_ports[line].uart;
}

#ifdef CONFIG_SERIAL_MSM_CONSOLE
<<<<<<< HEAD

/*
 *  Wait for transmitter & holding register to empty
 *  Derived from wait_for_xmitr in 8250 serial driver by Russell King
 */
static inline void wait_for_xmitr(struct uart_port *port, int bits)
{
	unsigned int status, mr, tmout = 10000;

	/* Wait up to 10ms for the character(s) to be sent. */
	do {
		status = msm_read(port, UART_SR);

		if (--tmout == 0)
			break;
		udelay(1);
	} while ((status & bits) != bits);

	mr = msm_read(port, UART_MR1);

	/* Wait up to 1s for flow control if necessary */
	if (mr & UART_MR1_CTS_CTL) {
		unsigned int tmout;
		for (tmout = 1000000; tmout; tmout--) {
			unsigned int isr = msm_read(port, UART_ISR);

			/* CTS input is active lo */
			if (!(isr & UART_IMR_CURRENT_CTS))
				break;
			udelay(1);
			touch_nmi_watchdog();
		}
	}
}


static void msm_console_putchar(struct uart_port *port, int c)
{
	/* This call can incur significant delay if CTS flowcontrol is enabled
	 * on port and no serial cable is attached.
	 */
	wait_for_xmitr(port, UART_SR_TX_READY);

	msm_write(port, c, UART_TF);
}

static void msm_console_write(struct console *co, const char *s,
			      unsigned int count)
{
	struct uart_port *port;
	struct msm_port *msm_port;
	int locked;
=======
static void msm_console_write(struct console *co, const char *s,
			      unsigned int count)
{
	int i;
	struct uart_port *port;
	struct msm_port *msm_port;
	int num_newlines = 0;
	bool replaced = false;
>>>>>>> refs/remotes/origin/master

	BUG_ON(co->index < 0 || co->index >= UART_NR);

	port = get_port_from_line(co->index);
	msm_port = UART_TO_MSM(port);

<<<<<<< HEAD
	/* not pretty, but we can end up here via various convoluted paths */
	if (port->sysrq || oops_in_progress)
		locked = spin_trylock(&port->lock);
	else {
		locked = 1;
		spin_lock(&port->lock);
	}

	uart_console_write(port, s, count, msm_console_putchar);

	if (locked)
		spin_unlock(&port->lock);
=======
	/* Account for newlines that will get a carriage return added */
	for (i = 0; i < count; i++)
		if (s[i] == '\n')
			num_newlines++;
	count += num_newlines;

	spin_lock(&port->lock);
	if (msm_port->is_uartdm)
		reset_dm_count(port, count);

	i = 0;
	while (i < count) {
		int j;
		unsigned int num_chars;
		char buf[4] = { 0 };
		unsigned int *bf = (unsigned int *)&buf;

		if (msm_port->is_uartdm)
			num_chars = min(count - i, (unsigned int)sizeof(buf));
		else
			num_chars = 1;

		for (j = 0; j < num_chars; j++) {
			char c = *s;

			if (c == '\n' && !replaced) {
				buf[j] = '\r';
				j++;
				replaced = true;
			}
			if (j < num_chars) {
				buf[j] = c;
				s++;
				replaced = false;
			}
		}

		while (!(msm_read(port, UART_SR) & UART_SR_TX_READY))
			cpu_relax();

		msm_write(port, *bf, msm_port->is_uartdm ? UARTDM_TF : UART_TF);
		i += num_chars;
	}
	spin_unlock(&port->lock);
>>>>>>> refs/remotes/origin/master
}

static int __init msm_console_setup(struct console *co, char *options)
{
	struct uart_port *port;
<<<<<<< HEAD
	int baud = 0, flow, bits, parity;
=======
	struct msm_port *msm_port;
	int baud, flow, bits, parity;
>>>>>>> refs/remotes/origin/master

	if (unlikely(co->index >= UART_NR || co->index < 0))
		return -ENXIO;

	port = get_port_from_line(co->index);
<<<<<<< HEAD
=======
	msm_port = UART_TO_MSM(port);
>>>>>>> refs/remotes/origin/master

	if (unlikely(!port->membase))
		return -ENXIO;

<<<<<<< HEAD
	port->cons = co;

	pm_runtime_get_noresume(port->dev);

#ifndef CONFIG_PM_RUNTIME
	msm_init_clock(port);
#endif
	pm_runtime_resume(port->dev);
=======
	msm_init_clock(port);
>>>>>>> refs/remotes/origin/master

	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);

	bits = 8;
	parity = 'n';
	flow = 'n';
	msm_write(port, UART_MR2_BITS_PER_CHAR_8 | UART_MR2_STOP_BIT_LEN_ONE,
		  UART_MR2);	/* 8N1 */

	if (baud < 300 || baud > 115200)
		baud = 115200;
	msm_set_baud_rate(port, baud);

	msm_reset(port);

<<<<<<< HEAD
=======
	if (msm_port->is_uartdm) {
		msm_write(port, UART_CR_CMD_PROTECTION_EN, UART_CR);
		msm_write(port, UART_CR_TX_ENABLE, UART_CR);
	}

>>>>>>> refs/remotes/origin/master
	printk(KERN_INFO "msm_serial: console setup on port #%d\n", port->line);

	return uart_set_options(port, co, baud, parity, bits, flow);
}

static struct uart_driver msm_uart_driver;

static struct console msm_console = {
	.name = "ttyMSM",
	.write = msm_console_write,
	.device = uart_console_device,
	.setup = msm_console_setup,
	.flags = CON_PRINTBUFFER,
	.index = -1,
	.data = &msm_uart_driver,
};

<<<<<<< HEAD
#define MSM_CONSOLE	&msm_console
=======
#define MSM_CONSOLE	(&msm_console)
>>>>>>> refs/remotes/origin/master

#else
#define MSM_CONSOLE	NULL
#endif

static struct uart_driver msm_uart_driver = {
	.owner = THIS_MODULE,
	.driver_name = "msm_serial",
	.dev_name = "ttyMSM",
	.nr = UART_NR,
	.cons = MSM_CONSOLE,
};

<<<<<<< HEAD
=======
static atomic_t msm_uart_next_id = ATOMIC_INIT(0);

static const struct of_device_id msm_uartdm_table[] = {
	{ .compatible = "qcom,msm-uartdm" },
	{ }
};

>>>>>>> refs/remotes/origin/master
static int __init msm_serial_probe(struct platform_device *pdev)
{
	struct msm_port *msm_port;
	struct resource *resource;
	struct uart_port *port;
	int irq;
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	struct msm_serial_platform_data *pdata = pdev->dev.platform_data;
#endif
=======
	struct msm_serial_platform_data *pdata = pdev->dev.platform_data;
>>>>>>> refs/remotes/origin/cm-10.0
=======

	if (pdev->id == -1)
		pdev->id = atomic_inc_return(&msm_uart_next_id) - 1;
>>>>>>> refs/remotes/origin/master

	if (unlikely(pdev->id < 0 || pdev->id >= UART_NR))
		return -ENXIO;

	printk(KERN_INFO "msm_serial: detected port #%d\n", pdev->id);

	port = get_port_from_line(pdev->id);
	port->dev = &pdev->dev;
	msm_port = UART_TO_MSM(port);

<<<<<<< HEAD
	msm_port->clk = clk_get(&pdev->dev, "core_clk");
	if (unlikely(IS_ERR(msm_port->clk)))
		return PTR_ERR(msm_port->clk);
	port->uartclk = clk_get_rate(msm_port->clk);
	if (!port->uartclk)
		port->uartclk = 19200000;

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return -ENXIO;
	port->mapbase = resource->start;

	irq = platform_get_irq(pdev, 0);
	if (unlikely(irq < 0))
		return -ENXIO;
	port->irq = irq;

	platform_set_drvdata(pdev, port);


#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	if (pdata == NULL)
		msm_port->wakeup.irq = -1;
	else {
		msm_port->wakeup.irq = pdata->wakeup_irq;
		msm_port->wakeup.ignore = 1;
		msm_port->wakeup.inject_rx = pdata->inject_rx_on_wakeup;
		msm_port->wakeup.rx_to_inject = pdata->rx_to_inject;

		if (unlikely(msm_port->wakeup.irq <= 0))
			return -EINVAL;
	}
#endif

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	msm_port->clk_state = MSM_CLK_PORT_OFF;
	hrtimer_init(&msm_port->clk_off_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	msm_port->clk_off_timer.function = msm_serial_clock_off;
	msm_port->clk_off_delay = ktime_set(0, 1000000);  /* 1 ms */
#endif

	pm_runtime_enable(port->dev);
<<<<<<< HEAD
=======
	if (pdata != NULL && pdata->userid && pdata->userid <= UART_NR)
		port->line = pdata->userid;
	return uart_add_one_port(&msm_uart_driver, port);
}

static int __init msm_uim_probe(struct platform_device *pdev)
{
	struct msm_port *msm_port;
	struct resource *resource;
	struct uart_port *port;
	int irq;

	if (unlikely(pdev->id < 0 || pdev->id >= UART_NR))
		return -ENXIO;

	pr_info("msm_uim: detected port #%d\n", pdev->id);

	port = get_port_from_line(pdev->id);
	port->dev = &pdev->dev;
	msm_port = UART_TO_MSM(port);

	msm_port->uim = true;
=======
	if (of_match_device(msm_uartdm_table, &pdev->dev))
		msm_port->is_uartdm = 1;
	else
		msm_port->is_uartdm = 0;

	msm_port->clk = devm_clk_get(&pdev->dev, "core");
	if (IS_ERR(msm_port->clk))
		return PTR_ERR(msm_port->clk);

	if (msm_port->is_uartdm) {
		msm_port->pclk = devm_clk_get(&pdev->dev, "iface");
		if (IS_ERR(msm_port->pclk))
			return PTR_ERR(msm_port->pclk);

		clk_set_rate(msm_port->clk, 1843200);
	}

	port->uartclk = clk_get_rate(msm_port->clk);
	printk(KERN_INFO "uartclk = %d\n", port->uartclk);

>>>>>>> refs/remotes/origin/master

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return -ENXIO;
	port->mapbase = resource->start;

	irq = platform_get_irq(pdev, 0);
	if (unlikely(irq < 0))
		return -ENXIO;
	port->irq = irq;

	platform_set_drvdata(pdev, port);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	return uart_add_one_port(&msm_uart_driver, port);
}

static int __devexit msm_serial_remove(struct platform_device *pdev)
{
	struct msm_port *msm_port = platform_get_drvdata(pdev);

	pm_runtime_put_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);

	clk_put(msm_port->clk);

	return 0;
}

#ifdef CONFIG_PM
static int msm_serial_suspend(struct device *dev)
{
	struct uart_port *port;
	struct platform_device *pdev = to_platform_device(dev);
	port = get_port_from_line(pdev->id);

	if (port) {
		uart_suspend_port(&msm_uart_driver, port);
		if (is_console(port))
			msm_deinit_clock(port);
	}

	return 0;
}

static int msm_serial_resume(struct device *dev)
{
	struct uart_port *port;
	struct platform_device *pdev = to_platform_device(dev);
	port = get_port_from_line(pdev->id);

	if (port) {
		if (is_console(port))
			msm_init_clock(port);
		uart_resume_port(&msm_uart_driver, port);
	}

	return 0;
}
#else
#define msm_serial_suspend NULL
#define msm_serial_resume NULL
#endif

static int msm_serial_runtime_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct uart_port *port;
	port = get_port_from_line(pdev->id);

	dev_dbg(dev, "pm_runtime: suspending\n");
	msm_deinit_clock(port);
	return 0;
}

static int msm_serial_runtime_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct uart_port *port;
	port = get_port_from_line(pdev->id);

	dev_dbg(dev, "pm_runtime: resuming\n");
	msm_init_clock(port);
	return 0;
}

static struct dev_pm_ops msm_serial_dev_pm_ops = {
	.suspend = msm_serial_suspend,
	.resume = msm_serial_resume,
	.runtime_suspend = msm_serial_runtime_suspend,
	.runtime_resume = msm_serial_runtime_resume,
=======
	return uart_add_one_port(&msm_uart_driver, port);
}

static int msm_serial_remove(struct platform_device *pdev)
{
	struct uart_port *port = platform_get_drvdata(pdev);

	uart_remove_one_port(&msm_uart_driver, port);

	return 0;
}

static struct of_device_id msm_match_table[] = {
	{ .compatible = "qcom,msm-uart" },
	{ .compatible = "qcom,msm-uartdm" },
	{}
>>>>>>> refs/remotes/origin/master
};

static struct platform_driver msm_platform_driver = {
	.remove = msm_serial_remove,
	.driver = {
		.name = "msm_serial",
		.owner = THIS_MODULE,
<<<<<<< HEAD
		.pm = &msm_serial_dev_pm_ops,
	},
};

<<<<<<< HEAD
=======
static struct platform_driver msm_platform_uim_driver = {
	.remove = msm_serial_remove,
	.driver = {
		.name = "msm_uim",
		.owner = THIS_MODULE,
	},
};

>>>>>>> refs/remotes/origin/cm-10.0
=======
		.of_match_table = msm_match_table,
	},
};

>>>>>>> refs/remotes/origin/master
static int __init msm_serial_init(void)
{
	int ret;

	ret = uart_register_driver(&msm_uart_driver);
	if (unlikely(ret))
		return ret;

	ret = platform_driver_probe(&msm_platform_driver, msm_serial_probe);
	if (unlikely(ret))
		uart_unregister_driver(&msm_uart_driver);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	platform_driver_probe(&msm_platform_uim_driver, msm_uim_probe);

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	printk(KERN_INFO "msm_serial: driver initialized\n");

	return ret;
}

static void __exit msm_serial_exit(void)
{
#ifdef CONFIG_SERIAL_MSM_CONSOLE
	unregister_console(&msm_console);
#endif
	platform_driver_unregister(&msm_platform_driver);
	uart_unregister_driver(&msm_uart_driver);
}

module_init(msm_serial_init);
module_exit(msm_serial_exit);

MODULE_AUTHOR("Robert Love <rlove@google.com>");
MODULE_DESCRIPTION("Driver for msm7x serial device");
<<<<<<< HEAD
MODULE_LICENSE("GPL v2");
=======
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
