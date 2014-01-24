/*
 *  Atheros Communication Bluetooth HCIATH3K UART protocol
 *
 *  HCIATH3K (HCI Atheros AR300x Protocol) is a Atheros Communication's
 *  power management protocol extension to H4 to support AR300x Bluetooth Chip.
 *
 *  Copyright (c) 2009-2010 Atheros Communications Inc.
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
 *  Copyright (c) 2012-2013 The Linux Foundation. All rights reserved.
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
 *  Copyright (c) 2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 *  Acknowledgements:
 *  This file is based on hci_h4.c, which was written
 *  by Maxim Krasnyansky and Marcel Holtmann.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/skbuff.h>
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/proc_fs.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
#include <linux/platform_device.h>
#include <linux/gpio.h>
>>>>>>> refs/remotes/origin/cm-11.0

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>

#include "hci_uart.h"
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_SERIAL_MSM_HS
#include <mach/msm_serial_hs.h>
#endif

static int enableuartsleep = 1;
module_param(enableuartsleep, int, 0644);
MODULE_PARM_DESC(enableuartsleep, "Enable Atheros Sleep Protocol");

/*
 * Global variables
 */

/** Device table */
static struct of_device_id bluesleep_match_table[] = {
	{ .compatible = "qca,ar3002_bluesleep" },
	{}
};

/** Global state flags */
static unsigned long flags;

/** To Check LPM is enabled */
static bool is_lpm_enabled;

/** Workqueue to respond to change in hostwake line */
static void wakeup_host_work(struct work_struct *work);

/** Transmission timer */
static void bluesleep_tx_timer_expire(unsigned long data);
static DEFINE_TIMER(tx_timer, bluesleep_tx_timer_expire, 0, 0);

/** Lock for state transitions */
static spinlock_t rw_lock;

#define PROC_DIR	"bluetooth/sleep"

#define POLARITY_LOW 0
#define POLARITY_HIGH 1

struct bluesleep_info {
	unsigned host_wake;			/* wake up host */
	unsigned ext_wake;			/* wake up device */
	unsigned host_wake_irq;
	int irq_polarity;
	struct uart_port *uport;
};

struct work_struct ws_sleep;

/* 1 second timeout */
#define TX_TIMER_INTERVAL  1

/* state variable names and bit positions */
#define BT_TXEXPIRED    0x01
#define BT_SLEEPENABLE  0x02
#define BT_SLEEPCMD	0x03

/* global pointer to a single hci device. */
static struct bluesleep_info *bsi;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

unsigned int enableuartsleep = 1;
module_param(enableuartsleep, uint, 0644);
/*
 * Global variables
 */
/** Global state flags */
static unsigned long flags;

/** Tasklet to respond to change in hostwake line */
static struct tasklet_struct hostwake_task;

/** Transmission timer */
static void bluesleep_tx_timer_expire(unsigned long data);
static DEFINE_TIMER(tx_timer, bluesleep_tx_timer_expire, 0, 0);

/** Lock for state transitions */
static spinlock_t rw_lock;

#define POLARITY_LOW 0
#define POLARITY_HIGH 1

struct bluesleep_info {
	unsigned host_wake;			/* wake up host */
	unsigned ext_wake;			/* wake up device */
	unsigned host_wake_irq;
	int irq_polarity;
};

/* 1 second timeout */
#define TX_TIMER_INTERVAL  1

/* state variable names and bit positions */
#define BT_TXEXPIRED    0x01
#define BT_SLEEPENABLE  0x02
#define BT_SLEEPCMD	0x03

/* global pointer to a single hci device. */
static struct bluesleep_info *bsi;

struct ath_struct {
	struct hci_uart *hu;
	unsigned int cur_sleep;

	struct sk_buff_head txq;
	struct work_struct ctxtsw;
};

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
static void hsuart_serial_clock_on(struct uart_port *port)
{
	BT_DBG("");
	if (port)
		msm_hs_request_clock_on(port);
	else
		BT_INFO("Uart has not voted for Clock ON");
}

static void hsuart_serial_clock_off(struct uart_port *port)
{
	BT_DBG("");
	if (port)
		msm_hs_request_clock_off(port);
	else
		BT_INFO("Uart has not voted for Clock OFF");
}

static void modify_timer_task(void)
{
	spin_lock(&rw_lock);
	mod_timer(&tx_timer, jiffies + (TX_TIMER_INTERVAL * HZ));
	clear_bit(BT_TXEXPIRED, &flags);
	spin_unlock(&rw_lock);

}

static int ath_wakeup_ar3k(void)
{
	int status = 0;
	if (test_bit(BT_TXEXPIRED, &flags)) {
		hsuart_serial_clock_on(bsi->uport);
		BT_DBG("wakeup device\n");
		gpio_set_value(bsi->ext_wake, 0);
		msleep(20);
		gpio_set_value(bsi->ext_wake, 1);
	}
	if (!is_lpm_enabled)
		modify_timer_task();
	return status;
}

static void wakeup_host_work(struct work_struct *work)
{

	BT_DBG("wake up host");
	if (test_bit(BT_SLEEPENABLE, &flags)) {
		if (test_bit(BT_TXEXPIRED, &flags))
			hsuart_serial_clock_on(bsi->uport);
	}
	if (!is_lpm_enabled)
		modify_timer_task();
=======
=======
>>>>>>> refs/remotes/origin/master
static int ath_wakeup_ar3k(struct tty_struct *tty)
{
	struct ktermios ktermios;
	int status = tty->driver->ops->tiocmget(tty);

	if (status & TIOCM_CTS)
		return status;

	/* Disable Automatic RTSCTS */
<<<<<<< HEAD
	memcpy(&ktermios, tty->termios, sizeof(ktermios));
=======
	ktermios = tty->termios;
>>>>>>> refs/remotes/origin/master
	ktermios.c_cflag &= ~CRTSCTS;
	tty_set_termios(tty, &ktermios);

	/* Clear RTS first */
	status = tty->driver->ops->tiocmget(tty);
	tty->driver->ops->tiocmset(tty, 0x00, TIOCM_RTS);
	mdelay(20);

	/* Set RTS, wake up board */
	status = tty->driver->ops->tiocmget(tty);
	tty->driver->ops->tiocmset(tty, TIOCM_RTS, 0x00);
	mdelay(20);
=======
static void hostwake_interrupt(unsigned long data)
{
	BT_INFO(" wakeup host\n");
}
>>>>>>> refs/remotes/origin/cm-11.0

static void modify_timer_task(void)
{
	spin_lock(&rw_lock);
	mod_timer(&tx_timer, jiffies + (TX_TIMER_INTERVAL * HZ));
	clear_bit(BT_TXEXPIRED, &flags);
	spin_unlock(&rw_lock);

}

static int ath_wakeup_ar3k(struct tty_struct *tty)
{
	int status = 0;
	if (test_bit(BT_TXEXPIRED, &flags)) {
		BT_INFO("wakeup device\n");
		gpio_set_value(bsi->ext_wake, 0);
		msleep(20);
		gpio_set_value(bsi->ext_wake, 1);
	}
	modify_timer_task();
	return status;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void ath_hci_uart_work(struct work_struct *work)
{
	int status;
	struct ath_struct *ath;
	struct hci_uart *hu;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct tty_struct *tty;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct tty_struct *tty;
>>>>>>> refs/remotes/origin/master

	ath = container_of(work, struct ath_struct, ctxtsw);

	hu = ath->hu;
<<<<<<< HEAD
<<<<<<< HEAD

	/* verify and wake up controller */
	if (test_bit(BT_SLEEPENABLE, &flags))
		status = ath_wakeup_ar3k();
=======
=======
>>>>>>> refs/remotes/origin/master
	tty = hu->tty;

	/* verify and wake up controller */
	if (test_bit(BT_SLEEPENABLE, &flags))
		status = ath_wakeup_ar3k(tty);
<<<<<<< HEAD
		if (!(status & TIOCM_CTS))
			return;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* Ready to send Data */
	clear_bit(HCI_UART_SENDING, &hu->tx_state);
	hci_uart_tx_wakeup(hu);
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
static irqreturn_t bluesleep_hostwake_isr(int irq, void *dev_id)
{
	/* schedule a work to global shared workqueue to handle
	 * the change in the host wake line
	 */
	schedule_work(&ws_sleep);

=======
static irqreturn_t bluesleep_hostwake_isr(int irq, void *dev_id)
{
	/* schedule a tasklet to handle the change in the host wake line */
	tasklet_schedule(&hostwake_task);
>>>>>>> refs/remotes/origin/cm-11.0
	return IRQ_HANDLED;
}

static int ath_bluesleep_gpio_config(int on)
{
	int ret = 0;

	BT_INFO("%s config: %d", __func__, on);
	if (!on) {
		if (disable_irq_wake(bsi->host_wake_irq))
			BT_ERR("Couldn't disable hostwake IRQ wakeup mode\n");
		goto free_host_wake_irq;
	}

	ret = gpio_request(bsi->host_wake, "bt_host_wake");
	if (ret < 0) {
		BT_ERR("failed to request gpio pin %d, error %d\n",
			bsi->host_wake, ret);
		goto gpio_config_failed;
	}

	/* configure host_wake as input */
	ret = gpio_direction_input(bsi->host_wake);
	if (ret < 0) {
		BT_ERR("failed to config GPIO %d as input pin, err %d\n",
			bsi->host_wake, ret);
		goto gpio_host_wake;
	}

	ret = gpio_request(bsi->ext_wake, "bt_ext_wake");
	if (ret < 0) {
		BT_ERR("failed to request gpio pin %d, error %d\n",
			bsi->ext_wake, ret);
		goto gpio_host_wake;
	}

	ret = gpio_direction_output(bsi->ext_wake, 1);
	if (ret < 0) {
		BT_ERR("failed to config GPIO %d as output pin, err %d\n",
			bsi->ext_wake, ret);
		goto gpio_ext_wake;
	}

	gpio_set_value(bsi->ext_wake, 1);

	/* Initialize spinlock. */
	spin_lock_init(&rw_lock);

	/* Initialize timer */
	init_timer(&tx_timer);
	tx_timer.function = bluesleep_tx_timer_expire;
	tx_timer.data = 0;

<<<<<<< HEAD
=======
	/* initialize host wake tasklet */
	tasklet_init(&hostwake_task, hostwake_interrupt, 0);

>>>>>>> refs/remotes/origin/cm-11.0
	if (bsi->irq_polarity == POLARITY_LOW) {
		ret = request_irq(bsi->host_wake_irq, bluesleep_hostwake_isr,
				IRQF_DISABLED | IRQF_TRIGGER_FALLING,
				"bluetooth hostwake", NULL);
	} else  {
		ret = request_irq(bsi->host_wake_irq, bluesleep_hostwake_isr,
				IRQF_DISABLED | IRQF_TRIGGER_RISING,
				"bluetooth hostwake", NULL);
	}
	if (ret  < 0) {
		BT_ERR("Couldn't acquire BT_HOST_WAKE IRQ");
		goto delete_timer;
	}

	ret = enable_irq_wake(bsi->host_wake_irq);
	if (ret < 0) {
		BT_ERR("Couldn't enable BT_HOST_WAKE as wakeup interrupt");
		goto free_host_wake_irq;
	}

	return 0;

free_host_wake_irq:
	free_irq(bsi->host_wake_irq, NULL);
delete_timer:
	del_timer(&tx_timer);
gpio_ext_wake:
	gpio_free(bsi->ext_wake);
gpio_host_wake:
	gpio_free(bsi->host_wake);
gpio_config_failed:
	return ret;
}

<<<<<<< HEAD
static int ath_lpm_start(void)
{
	BT_DBG("Start LPM mode");

	if (!bsi) {
		BT_ERR("HCIATH3K bluesleep info does not exist");
		return -EIO;
	}

	bsi->uport = msm_hs_get_uart_port(0);
	if (!bsi->uport) {
		BT_ERR("UART Port is not available");
		return -ENODEV;
	}

	INIT_WORK(&ws_sleep, wakeup_host_work);

	if (ath_bluesleep_gpio_config(1) < 0) {
		BT_ERR("HCIATH3K GPIO Config failed");
		return -EIO;
	}

	return 0;
}

static int ath_lpm_stop(void)
{
	BT_DBG("Stop LPM mode");
	cancel_work_sync(&ws_sleep);

	if (bsi) {
		bsi->uport = NULL;
		ath_bluesleep_gpio_config(0);
	}

	return 0;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
/* Initialize protocol */
static int ath_open(struct hci_uart *hu)
{
	struct ath_struct *ath;
<<<<<<< HEAD
<<<<<<< HEAD
	struct uart_state *state;

	BT_DBG("hu %p, bsi %p", hu, bsi);

	if (!bsi) {
		BT_ERR("HCIATH3K bluesleep info does not exist");
		return -EIO;
	}

	ath = kzalloc(sizeof(*ath), GFP_ATOMIC);
	if (!ath) {
		BT_ERR("HCIATH3K Memory not enough to init driver");
		return -ENOMEM;
	}
=======

	BT_DBG("hu %p", hu);

	ath = kzalloc(sizeof(*ath), GFP_ATOMIC);
	if (!ath)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0
=======

	BT_DBG("hu %p, bsi %p", hu, bsi);

	if (!bsi)
		return -EIO;

	if (ath_bluesleep_gpio_config(1) < 0) {
		BT_ERR("HCIATH3K GPIO Config failed");
		return -EIO;
	}

	ath = kzalloc(sizeof(*ath), GFP_ATOMIC);
	if (!ath)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master

	skb_queue_head_init(&ath->txq);

	hu->priv = ath;
	ath->hu = hu;
<<<<<<< HEAD
<<<<<<< HEAD
	state = hu->tty->driver_data;

	if (!state) {
		BT_ERR("HCIATH3K tty driver data does not exist");
		return -ENXIO;
	}
	bsi->uport = state->uart_port;

	if (ath_bluesleep_gpio_config(1) < 0) {
		BT_ERR("HCIATH3K GPIO Config failed");
		hu->priv = NULL;
		kfree(ath);
		return -EIO;
	}

	ath->cur_sleep = enableuartsleep;
	if (ath->cur_sleep == 1) {
		set_bit(BT_SLEEPENABLE, &flags);
		modify_timer_task();
	}
	INIT_WORK(&ath->ctxtsw, ath_hci_uart_work);
	INIT_WORK(&ws_sleep, wakeup_host_work);
=======

	ath->cur_sleep = enableuartsleep;
	if (ath->cur_sleep == 1) {
		set_bit(BT_SLEEPENABLE, &flags);
		modify_timer_task();
	}
	INIT_WORK(&ath->ctxtsw, ath_hci_uart_work);

>>>>>>> refs/remotes/origin/cm-10.0
=======

	INIT_WORK(&ath->ctxtsw, ath_hci_uart_work);

>>>>>>> refs/remotes/origin/master
	return 0;
}

/* Flush protocol data */
static int ath_flush(struct hci_uart *hu)
{
	struct ath_struct *ath = hu->priv;

	BT_DBG("hu %p", hu);

	skb_queue_purge(&ath->txq);

	return 0;
}

/* Close protocol */
static int ath_close(struct hci_uart *hu)
{
	struct ath_struct *ath = hu->priv;

	BT_DBG("hu %p", hu);

	skb_queue_purge(&ath->txq);

	cancel_work_sync(&ath->ctxtsw);

<<<<<<< HEAD
<<<<<<< HEAD
	cancel_work_sync(&ws_sleep);

	if (bsi)
		ath_bluesleep_gpio_config(0);

	hu->priv = NULL;
	bsi->uport = NULL;
=======
	hu->priv = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	hu->priv = NULL;
>>>>>>> refs/remotes/origin/master
	kfree(ath);

	if (bsi)
		ath_bluesleep_gpio_config(0);

	return 0;
}

#define HCI_OP_ATH_SLEEP 0xFC04

/* Enqueue frame for transmittion */
static int ath_enqueue(struct hci_uart *hu, struct sk_buff *skb)
{
	struct ath_struct *ath = hu->priv;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	BT_DBG("");

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	BT_DBG("");

>>>>>>> refs/remotes/origin/cm-11.0
	if (bt_cb(skb)->pkt_type == HCI_SCODATA_PKT) {
		kfree_skb(skb);
		return 0;
	}

	/*
	 * Update power management enable flag with parameters of
	 * HCI sleep enable vendor specific HCI command.
	 */
	if (bt_cb(skb)->pkt_type == HCI_COMMAND_PKT) {
		struct hci_command_hdr *hdr = (void *)skb->data;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		if (__le16_to_cpu(hdr->opcode) == HCI_OP_ATH_SLEEP) {
			set_bit(BT_SLEEPCMD, &flags);
			ath->cur_sleep = skb->data[HCI_COMMAND_HDR_SIZE];
		}
=======

		if (__le16_to_cpu(hdr->opcode) == HCI_OP_ATH_SLEEP)
			ath->cur_sleep = skb->data[HCI_COMMAND_HDR_SIZE];
>>>>>>> refs/remotes/origin/cm-10.0
=======

		if (__le16_to_cpu(hdr->opcode) == HCI_OP_ATH_SLEEP)
			ath->cur_sleep = skb->data[HCI_COMMAND_HDR_SIZE];
>>>>>>> refs/remotes/origin/master
=======
		if (__le16_to_cpu(hdr->opcode) == HCI_OP_ATH_SLEEP) {
			set_bit(BT_SLEEPCMD, &flags);
			ath->cur_sleep = skb->data[HCI_COMMAND_HDR_SIZE];
		}
>>>>>>> refs/remotes/origin/cm-11.0
	}

	BT_DBG("hu %p skb %p", hu, skb);

	/* Prepend skb with frame type */
	memcpy(skb_push(skb, 1), &bt_cb(skb)->pkt_type, 1);

	skb_queue_tail(&ath->txq, skb);
	set_bit(HCI_UART_SENDING, &hu->tx_state);

	schedule_work(&ath->ctxtsw);

	return 0;
}

static struct sk_buff *ath_dequeue(struct hci_uart *hu)
{
	struct ath_struct *ath = hu->priv;

	return skb_dequeue(&ath->txq);
}

/* Recv data */
static int ath_recv(struct hci_uart *hu, void *data, int count)
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	struct ath_struct *ath = hu->priv;
	unsigned int type;

	BT_DBG("");

	if (hci_recv_stream_fragment(hu->hdev, data, count) < 0)
		BT_ERR("Frame Reassembly Failed");

	if (count & test_bit(BT_SLEEPCMD, &flags)) {
		struct sk_buff *skb = hu->hdev->reassembly[0];

		if (!skb) {
			struct { char type; } *pkt;

			/* Start of the frame */
			pkt = data;
			type = pkt->type;
		} else
			type = bt_cb(skb)->pkt_type;

		if (type == HCI_EVENT_PKT) {
			clear_bit(BT_SLEEPCMD, &flags);
			BT_INFO("cur_sleep:%d\n", ath->cur_sleep);
			if (ath->cur_sleep == 1)
				set_bit(BT_SLEEPENABLE, &flags);
			else
				clear_bit(BT_SLEEPENABLE, &flags);
		}
		if (test_bit(BT_SLEEPENABLE, &flags))
			modify_timer_task();
	}
	return count;
}

static void bluesleep_tx_timer_expire(unsigned long data)
{

	if (!test_bit(BT_SLEEPENABLE, &flags))
		return;
	BT_INFO("Tx timer expired\n");

	set_bit(BT_TXEXPIRED, &flags);
	hsuart_serial_clock_off(bsi->uport);
=======
=======
>>>>>>> refs/remotes/origin/master
	int ret;
=======
	struct ath_struct *ath = hu->priv;
	unsigned int type;
>>>>>>> refs/remotes/origin/cm-11.0

	BT_DBG("");

	if (hci_recv_stream_fragment(hu->hdev, data, count) < 0)
		BT_ERR("Frame Reassembly Failed");

	if (count & test_bit(BT_SLEEPCMD, &flags)) {
		struct sk_buff *skb = hu->hdev->reassembly[0];

		if (!skb) {
			struct { char type; } *pkt;

			/* Start of the frame */
			pkt = data;
			type = pkt->type;
		} else
			type = bt_cb(skb)->pkt_type;

		if (type == HCI_EVENT_PKT) {
			clear_bit(BT_SLEEPCMD, &flags);
			BT_INFO("cur_sleep:%d\n", ath->cur_sleep);
			if (ath->cur_sleep == 1)
				set_bit(BT_SLEEPENABLE, &flags);
			else
				clear_bit(BT_SLEEPENABLE, &flags);
		}
		if (test_bit(BT_SLEEPENABLE, &flags))
			modify_timer_task();
	}
	return count;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void bluesleep_tx_timer_expire(unsigned long data)
{
	if (!test_bit(BT_SLEEPENABLE, &flags))
		return;
	BT_INFO("Tx timer expired\n");

	set_bit(BT_TXEXPIRED, &flags);
}

static struct hci_uart_proto athp = {
	.id = HCI_UART_ATH3K,
	.open = ath_open,
	.close = ath_close,
	.recv = ath_recv,
	.enqueue = ath_enqueue,
	.dequeue = ath_dequeue,
	.flush = ath_flush,
};

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
static int lpm_enabled;

static int bluesleep_lpm_set(const char *val, const struct kernel_param *kp)
{
	int ret;

	ret = param_set_int(val, kp);

	if (ret) {
		BT_ERR("HCIATH3K: lpm enable parameter set failed");
		return ret;
	}

	BT_DBG("lpm : %d", lpm_enabled);

	if ((lpm_enabled == 0) && is_lpm_enabled) {
		ath_lpm_stop();
		clear_bit(BT_SLEEPENABLE, &flags);
		is_lpm_enabled = false;
	} else if ((lpm_enabled == 1) && !is_lpm_enabled) {
		if (ath_lpm_start() < 0) {
			BT_ERR("HCIATH3K LPM mode failed");
			return -EIO;
		}
		set_bit(BT_SLEEPENABLE, &flags);
		is_lpm_enabled = true;
	} else {
		BT_ERR("HCIATH3K invalid lpm value");
		return -EINVAL;
	}
	return 0;

}

static struct kernel_param_ops bluesleep_lpm_ops = {
	.set = bluesleep_lpm_set,
	.get = param_get_int,
};

module_param_cb(ath_lpm, &bluesleep_lpm_ops,
		&lpm_enabled, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(ath_lpm, "Enable Atheros LPM sleep Protocol");

static int lpm_btwrite;

static int bluesleep_lpm_btwrite(const char *val, const struct kernel_param *kp)
{
	int ret;

	ret = param_set_int(val, kp);

	if (ret) {
		BT_ERR("HCIATH3K: lpm btwrite parameter set failed");
		return ret;
	}

	BT_DBG("btwrite : %d", lpm_btwrite);
	if (is_lpm_enabled) {
		if (lpm_btwrite == 0) {
			/*Setting TXEXPIRED bit to make it
			compatible with current solution*/
			set_bit(BT_TXEXPIRED, &flags);
			hsuart_serial_clock_off(bsi->uport);
		} else if (lpm_btwrite == 1) {
			ath_wakeup_ar3k();
			clear_bit(BT_TXEXPIRED, &flags);
		} else {
			BT_ERR("HCIATH3K invalid btwrite value");
			return -EINVAL;
		}
	}
	return 0;
}

static struct kernel_param_ops bluesleep_lpm_btwrite_ops = {
	.set = bluesleep_lpm_btwrite,
	.get = param_get_int,
};

module_param_cb(ath_btwrite, &bluesleep_lpm_btwrite_ops,
		&lpm_btwrite, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(ath_lpm, "Assert/Deassert the sleep");

static int bluesleep_populate_dt_pinfo(struct platform_device *pdev)
{
	BT_DBG("");

	if (!bsi)
		return -ENOMEM;

	bsi->host_wake = of_get_named_gpio(pdev->dev.of_node,
					 "host-wake-gpio", 0);
	if (bsi->host_wake < 0) {
		BT_ERR("couldn't find host_wake gpio\n");
		return -ENODEV;
	}

	bsi->ext_wake = of_get_named_gpio(pdev->dev.of_node,
					 "ext-wake-gpio", 0);
	if (bsi->ext_wake < 0) {
		BT_ERR("couldn't find ext_wake gpio\n");
		return -ENODEV;
	}

	return 0;
}

static int bluesleep_populate_pinfo(struct platform_device *pdev)
{
	struct resource *res;

	BT_DBG("");

	res = platform_get_resource_byname(pdev, IORESOURCE_IO,
				"gpio_host_wake");
	if (!res) {
		BT_ERR("couldn't find host_wake gpio\n");
		return -ENODEV;
=======
static int __init bluesleep_probe(struct platform_device *pdev)
{
	int ret;
	struct resource *res;

	BT_DBG("");

	bsi = kzalloc(sizeof(struct bluesleep_info), GFP_KERNEL);
	if (!bsi) {
		ret = -ENOMEM;
		goto failed;
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_IO,
						"gpio_host_wake");
	if (!res) {
		BT_ERR("couldn't find host_wake gpio\n");
		ret = -ENODEV;
		goto free_bsi;
>>>>>>> refs/remotes/origin/cm-11.0
	}
	bsi->host_wake = res->start;

	res = platform_get_resource_byname(pdev, IORESOURCE_IO,
<<<<<<< HEAD
				"gpio_ext_wake");
	if (!res) {
		BT_ERR("couldn't find ext_wake gpio\n");
		return -ENODEV;
	}
	bsi->ext_wake = res->start;

	return 0;
}

static int __devinit bluesleep_probe(struct platform_device *pdev)
{
	int ret;

	BT_DBG("");

	bsi = kzalloc(sizeof(struct bluesleep_info), GFP_KERNEL);
	if (!bsi) {
		ret = -ENOMEM;
		goto failed;
	}

	if (pdev->dev.of_node) {
		ret = bluesleep_populate_dt_pinfo(pdev);
		if (ret < 0) {
			BT_ERR("Failed to populate device tree info");
			goto free_bsi;
		}
	} else {
		ret = bluesleep_populate_pinfo(pdev);
		if (ret < 0) {
			BT_ERR("Failed to populate device info");
			goto free_bsi;
		}
	}

	BT_DBG("host_wake_gpio: %d ext_wake_gpio: %d",
				bsi->host_wake, bsi->ext_wake);

=======
						"gpio_ext_wake");
	if (!res) {
		BT_ERR("couldn't find ext_wake gpio\n");
		ret = -ENODEV;
		goto free_bsi;
	}
	bsi->ext_wake = res->start;

>>>>>>> refs/remotes/origin/cm-11.0
	bsi->host_wake_irq = platform_get_irq_byname(pdev, "host_wake");
	if (bsi->host_wake_irq < 0) {
		BT_ERR("couldn't find host_wake irq\n");
		ret = -ENODEV;
		goto free_bsi;
	}

	bsi->irq_polarity = POLARITY_LOW;	/* low edge (falling edge) */

	return 0;

free_bsi:
	kfree(bsi);
	bsi = NULL;
failed:
	return ret;
}

static int bluesleep_remove(struct platform_device *pdev)
{
	kfree(bsi);
	return 0;
}

static struct platform_driver bluesleep_driver = {
<<<<<<< HEAD
	.probe = bluesleep_probe,
=======
>>>>>>> refs/remotes/origin/cm-11.0
	.remove = bluesleep_remove,
	.driver = {
		.name = "bluesleep",
		.owner = THIS_MODULE,
<<<<<<< HEAD
		.of_match_table = bluesleep_match_table,
	},
};

int __init ath_init(void)
{
	int ret;

	ret = hci_uart_register_proto(&athp);

	if (!ret)
		BT_INFO("HCIATH3K protocol initialized");
	else {
		BT_ERR("HCIATH3K protocol registration failed");
		return ret;
	}

	ret = platform_driver_register(&bluesleep_driver);
	if (ret) {
		BT_ERR("Failed to register bluesleep driver");
		return ret;
	}

	return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
=======
	},
};

>>>>>>> refs/remotes/origin/cm-11.0
int __init ath_init(void)
{
	int ret;

	ret = hci_uart_register_proto(&athp);

	if (!ret)
		BT_INFO("HCIATH3K protocol initialized");
	else {
		BT_ERR("HCIATH3K protocol registration failed");
<<<<<<< HEAD

	return err;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
		return ret;
	}
	ret = platform_driver_probe(&bluesleep_driver, bluesleep_probe);
	if (ret)
		return ret;
	return 0;
>>>>>>> refs/remotes/origin/cm-11.0
}

int __exit ath_deinit(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	platform_driver_unregister(&bluesleep_driver);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	platform_driver_unregister(&bluesleep_driver);
>>>>>>> refs/remotes/origin/cm-11.0
	return hci_uart_unregister_proto(&athp);
}
