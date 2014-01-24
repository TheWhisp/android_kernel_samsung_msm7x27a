<<<<<<< HEAD
<<<<<<< HEAD
/* drivers/serial/msm_serial_hs.c
 *
 * MSM 7k High speed uart driver
 *
 * Copyright (c) 2008 Google Inc.
<<<<<<< HEAD
 * Copyright (c) 2007-2011, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2007-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
 * Modified: Nick Pelly <npelly@google.com>
 *
 * All source code in this file is licensed under the following license
 * except where indicated.
 *
=======
/*
 * MSM 7k/8k High speed uart driver
=======
/* drivers/serial/msm_serial_hs.c
 *
 * MSM 7k High speed uart driver
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * Copyright (c) 2008 Google Inc.
 * Copyright (c) 2007-2012, The Linux Foundation. All rights reserved.
 * Modified: Nick Pelly <npelly@google.com>
 *
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
 * All source code in this file is licensed under the following license
 * except where indicated.
 *
>>>>>>> refs/remotes/origin/cm-11.0
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Has optional support for uart power management independent of linux
 * suspend/resume:
 *
 * RX wakeup.
 * UART wakeup can be triggered by RX activity (using a wakeup GPIO on the
 * UART RX pin). This should only be used if there is not a wakeup
 * GPIO on the UART CTS, and the first RX byte is known (for example, with the
 * Bluetooth Texas Instruments HCILL protocol), since the first RX byte will
 * always be lost. RTS will be asserted even while the UART is off in this mode
 * of operation. See msm_serial_hs_platform_data.rx_wakeup_irq.
 */

#include <linux/module.h>

#include <linux/serial.h>
#include <linux/serial_core.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/tty.h>
#include <linux/tty_flip.h>
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/timer.h>
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/delay.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
#include <linux/delay.h>
>>>>>>> refs/remotes/origin/cm-11.0
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/tty_flip.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/wait.h>
#include <linux/sysfs.h>
#include <linux/stat.h>
#include <linux/device.h>
#include <linux/wakelock.h>
#include <linux/debugfs.h>
#include <asm/atomic.h>
#include <asm/irq.h>
<<<<<<< HEAD
#include <asm/system.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0

#include <mach/hardware.h>
#include <mach/dma.h>
#include <mach/msm_serial_hs.h>

#include "msm_serial_hs_hwreg.h"

static int hs_serial_debug_mask = 1;
module_param_named(debug_mask, hs_serial_debug_mask,
		   int, S_IRUGO | S_IWUSR | S_IWGRP);

=======
=======
#include <linux/tty_flip.h>
>>>>>>> refs/remotes/origin/cm-11.0
#include <linux/wait.h>
#include <linux/sysfs.h>
#include <linux/stat.h>
#include <linux/device.h>
#include <linux/wakelock.h>
#include <linux/debugfs.h>
#include <asm/atomic.h>
#include <asm/irq.h>

#include <mach/hardware.h>
#include <mach/dma.h>
<<<<<<< HEAD
#include <linux/platform_data/msm_serial_hs.h>

/* HSUART Registers */
#define UARTDM_MR1_ADDR 0x0
#define UARTDM_MR2_ADDR 0x4

/* Data Mover result codes */
#define RSLT_FIFO_CNTR_BMSK (0xE << 28)
#define RSLT_VLD            BIT(1)

/* write only register */
#define UARTDM_CSR_ADDR 0x8
#define UARTDM_CSR_115200 0xFF
#define UARTDM_CSR_57600  0xEE
#define UARTDM_CSR_38400  0xDD
#define UARTDM_CSR_28800  0xCC
#define UARTDM_CSR_19200  0xBB
#define UARTDM_CSR_14400  0xAA
#define UARTDM_CSR_9600   0x99
#define UARTDM_CSR_7200   0x88
#define UARTDM_CSR_4800   0x77
#define UARTDM_CSR_3600   0x66
#define UARTDM_CSR_2400   0x55
#define UARTDM_CSR_1200   0x44
#define UARTDM_CSR_600    0x33
#define UARTDM_CSR_300    0x22
#define UARTDM_CSR_150    0x11
#define UARTDM_CSR_75     0x00

/* write only register */
#define UARTDM_TF_ADDR 0x70
#define UARTDM_TF2_ADDR 0x74
#define UARTDM_TF3_ADDR 0x78
#define UARTDM_TF4_ADDR 0x7C

/* write only register */
#define UARTDM_CR_ADDR 0x10
#define UARTDM_IMR_ADDR 0x14

#define UARTDM_IPR_ADDR 0x18
#define UARTDM_TFWR_ADDR 0x1c
#define UARTDM_RFWR_ADDR 0x20
#define UARTDM_HCR_ADDR 0x24
#define UARTDM_DMRX_ADDR 0x34
#define UARTDM_IRDA_ADDR 0x38
#define UARTDM_DMEN_ADDR 0x3c

/* UART_DM_NO_CHARS_FOR_TX */
#define UARTDM_NCF_TX_ADDR 0x40

#define UARTDM_BADR_ADDR 0x44

#define UARTDM_SIM_CFG_ADDR 0x80
/* Read Only register */
#define UARTDM_SR_ADDR 0x8

/* Read Only register */
#define UARTDM_RF_ADDR  0x70
#define UARTDM_RF2_ADDR 0x74
#define UARTDM_RF3_ADDR 0x78
#define UARTDM_RF4_ADDR 0x7C

/* Read Only register */
#define UARTDM_MISR_ADDR 0x10

/* Read Only register */
#define UARTDM_ISR_ADDR 0x14
#define UARTDM_RX_TOTAL_SNAP_ADDR 0x38

#define UARTDM_RXFS_ADDR 0x50

/* Register field Mask Mapping */
#define UARTDM_SR_PAR_FRAME_BMSK        BIT(5)
#define UARTDM_SR_OVERRUN_BMSK          BIT(4)
#define UARTDM_SR_TXEMT_BMSK            BIT(3)
#define UARTDM_SR_TXRDY_BMSK            BIT(2)
#define UARTDM_SR_RXRDY_BMSK            BIT(0)

#define UARTDM_CR_TX_DISABLE_BMSK       BIT(3)
#define UARTDM_CR_RX_DISABLE_BMSK       BIT(1)
#define UARTDM_CR_TX_EN_BMSK            BIT(2)
#define UARTDM_CR_RX_EN_BMSK            BIT(0)

/* UARTDM_CR channel_comman bit value (register field is bits 8:4) */
#define RESET_RX                0x10
#define RESET_TX                0x20
#define RESET_ERROR_STATUS      0x30
#define RESET_BREAK_INT         0x40
#define START_BREAK             0x50
#define STOP_BREAK              0x60
#define RESET_CTS               0x70
#define RESET_STALE_INT         0x80
#define RFR_LOW                 0xD0
#define RFR_HIGH                0xE0
#define CR_PROTECTION_EN        0x100
#define STALE_EVENT_ENABLE      0x500
#define STALE_EVENT_DISABLE     0x600
#define FORCE_STALE_EVENT       0x400
#define CLEAR_TX_READY          0x300
#define RESET_TX_ERROR          0x800
#define RESET_TX_DONE           0x810

#define UARTDM_MR1_AUTO_RFR_LEVEL1_BMSK 0xffffff00
#define UARTDM_MR1_AUTO_RFR_LEVEL0_BMSK 0x3f
#define UARTDM_MR1_CTS_CTL_BMSK 0x40
#define UARTDM_MR1_RX_RDY_CTL_BMSK 0x80

#define UARTDM_MR2_ERROR_MODE_BMSK 0x40
#define UARTDM_MR2_BITS_PER_CHAR_BMSK 0x30

/* bits per character configuration */
#define FIVE_BPC  (0 << 4)
#define SIX_BPC   (1 << 4)
#define SEVEN_BPC (2 << 4)
#define EIGHT_BPC (3 << 4)

#define UARTDM_MR2_STOP_BIT_LEN_BMSK 0xc
#define STOP_BIT_ONE (1 << 2)
#define STOP_BIT_TWO (3 << 2)

#define UARTDM_MR2_PARITY_MODE_BMSK 0x3

/* Parity configuration */
#define NO_PARITY 0x0
#define EVEN_PARITY 0x1
#define ODD_PARITY 0x2
#define SPACE_PARITY 0x3

#define UARTDM_IPR_STALE_TIMEOUT_MSB_BMSK 0xffffff80
#define UARTDM_IPR_STALE_LSB_BMSK 0x1f

/* These can be used for both ISR and IMR register */
#define UARTDM_ISR_TX_READY_BMSK        BIT(7)
#define UARTDM_ISR_CURRENT_CTS_BMSK     BIT(6)
#define UARTDM_ISR_DELTA_CTS_BMSK       BIT(5)
#define UARTDM_ISR_RXLEV_BMSK           BIT(4)
#define UARTDM_ISR_RXSTALE_BMSK         BIT(3)
#define UARTDM_ISR_RXBREAK_BMSK         BIT(2)
#define UARTDM_ISR_RXHUNT_BMSK          BIT(1)
#define UARTDM_ISR_TXLEV_BMSK           BIT(0)

/* Field definitions for UART_DM_DMEN*/
#define UARTDM_TX_DM_EN_BMSK 0x1
#define UARTDM_RX_DM_EN_BMSK 0x2

#define UART_FIFOSIZE 64
#define UARTCLK 7372800

/* Rx DMA request states */
>>>>>>> refs/remotes/origin/master
=======
#include <mach/msm_serial_hs.h>

#include "msm_serial_hs_hwreg.h"

static int hs_serial_debug_mask = 1;
module_param_named(debug_mask, hs_serial_debug_mask,
		   int, S_IRUGO | S_IWUSR | S_IWGRP);

>>>>>>> refs/remotes/origin/cm-11.0
enum flush_reason {
	FLUSH_NONE,
	FLUSH_DATA_READY,
	FLUSH_DATA_INVALID,  /* values after this indicate invalid data */
	FLUSH_IGNORE = FLUSH_DATA_INVALID,
	FLUSH_STOP,
	FLUSH_SHUTDOWN,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
/* UART clock states */
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
enum msm_hs_clk_states_e {
	MSM_HS_CLK_PORT_OFF,     /* port not in use */
	MSM_HS_CLK_OFF,          /* clock disabled */
	MSM_HS_CLK_REQUEST_OFF,  /* disable after TX and RX flushed */
	MSM_HS_CLK_ON,           /* clock enabled */
};

/* Track the forced RXSTALE flush during clock off sequence.
 * These states are only valid during MSM_HS_CLK_REQUEST_OFF */
enum msm_hs_clk_req_off_state_e {
	CLK_REQ_OFF_START,
	CLK_REQ_OFF_RXSTALE_ISSUED,
	CLK_REQ_OFF_FLUSH_ISSUED,
	CLK_REQ_OFF_RXSTALE_FLUSHED,
};

<<<<<<< HEAD
<<<<<<< HEAD
struct msm_hs_tx {
	unsigned int tx_ready_int_en;  /* ok to dma more tx */
	unsigned int dma_in_flight;    /* tx dma in progress */
<<<<<<< HEAD
=======
	enum flush_reason flush;
	wait_queue_head_t wait;
>>>>>>> refs/remotes/origin/cm-10.0
=======
/**
 * struct msm_hs_tx
 * @tx_ready_int_en: ok to dma more tx?
 * @dma_in_flight: tx dma in progress
 * @xfer: top level DMA command pointer structure
 * @command_ptr: third level command struct pointer
 * @command_ptr_ptr: second level command list struct pointer
 * @mapped_cmd_ptr: DMA view of third level command struct
 * @mapped_cmd_ptr_ptr: DMA view of second level command list struct
 * @tx_count: number of bytes to transfer in DMA transfer
 * @dma_base: DMA view of UART xmit buffer
 *
 * This structure describes a single Tx DMA transaction. MSM DMA
 * commands have two levels of indirection. The top level command
 * ptr points to a list of command ptr which in turn points to a
 * single DMA 'command'. In our case each Tx transaction consists
 * of a single second level pointer pointing to a 'box type' command.
 */
struct msm_hs_tx {
	unsigned int tx_ready_int_en;
	unsigned int dma_in_flight;
>>>>>>> refs/remotes/origin/master
=======
struct msm_hs_tx {
	unsigned int tx_ready_int_en;  /* ok to dma more tx */
	unsigned int dma_in_flight;    /* tx dma in progress */
	enum flush_reason flush;
	wait_queue_head_t wait;
>>>>>>> refs/remotes/origin/cm-11.0
	struct msm_dmov_cmd xfer;
	dmov_box *command_ptr;
	u32 *command_ptr_ptr;
	dma_addr_t mapped_cmd_ptr;
	dma_addr_t mapped_cmd_ptr_ptr;
	int tx_count;
	dma_addr_t dma_base;
<<<<<<< HEAD
<<<<<<< HEAD
	struct tasklet_struct tlet;
};

=======
};

/**
 * struct msm_hs_rx
 * @flush: Rx DMA request state
 * @xfer: top level DMA command pointer structure
 * @cmdptr_dmaaddr: DMA view of second level command structure
 * @command_ptr: third level DMA command pointer structure
 * @command_ptr_ptr: second level DMA command list pointer
 * @mapped_cmd_ptr: DMA view of the third level command structure
 * @wait: wait for DMA completion before shutdown
 * @buffer: destination buffer for RX DMA
 * @rbuffer: DMA view of buffer
 * @pool: dma pool out of which coherent rx buffer is allocated
 * @tty_work: private work-queue for tty flip buffer push task
 *
 * This structure describes a single Rx DMA transaction. Rx DMA
 * transactions use box mode DMA commands.
 */
>>>>>>> refs/remotes/origin/master
=======
	struct tasklet_struct tlet;
};

>>>>>>> refs/remotes/origin/cm-11.0
struct msm_hs_rx {
	enum flush_reason flush;
	struct msm_dmov_cmd xfer;
	dma_addr_t cmdptr_dmaaddr;
	dmov_box *command_ptr;
	u32 *command_ptr_ptr;
	dma_addr_t mapped_cmd_ptr;
	wait_queue_head_t wait;
	dma_addr_t rbuffer;
	unsigned char *buffer;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int buffer_pending;
	struct dma_pool *pool;
	struct wake_lock wake_lock;
	struct delayed_work flip_insert_work;
	struct tasklet_struct tlet;
<<<<<<< HEAD
=======
	bool dma_in_flight;
	bool cmd_exec;
>>>>>>> refs/remotes/origin/cm-10.0
};

enum buffer_states {
	NONE_PENDING = 0x0,
	FIFO_OVERRUN = 0x1,
	PARITY_ERROR = 0x2,
	CHARS_NORMAL = 0x4,
};

/* optional low power wakeup, typically on a GPIO RX irq */
struct msm_hs_wakeup {
	int irq;  /* < 0 indicates low power wakeup disabled */
	unsigned char ignore;  /* bool */

	/* bool: inject char into rx tty on wakeup */
=======
=======
	unsigned int buffer_pending;
>>>>>>> refs/remotes/origin/cm-11.0
	struct dma_pool *pool;
	struct wake_lock wake_lock;
	struct delayed_work flip_insert_work;
	struct tasklet_struct tlet;
	bool dma_in_flight;
	bool cmd_exec;
};

enum buffer_states {
	NONE_PENDING = 0x0,
	FIFO_OVERRUN = 0x1,
	PARITY_ERROR = 0x2,
	CHARS_NORMAL = 0x4,
};

/* optional low power wakeup, typically on a GPIO RX irq */
struct msm_hs_wakeup {
	int irq;  /* < 0 indicates low power wakeup disabled */
<<<<<<< HEAD
	unsigned char ignore;
>>>>>>> refs/remotes/origin/master
=======
	unsigned char ignore;  /* bool */

	/* bool: inject char into rx tty on wakeup */
>>>>>>> refs/remotes/origin/cm-11.0
	unsigned char inject_rx;
	char rx_to_inject;
};

<<<<<<< HEAD
<<<<<<< HEAD
struct msm_hs_port {
	struct uart_port uport;
<<<<<<< HEAD
	unsigned long imr_reg;  /* shadow value of UARTDM_IMR */
=======
	unsigned int imr_reg;  /* shadow value of UARTDM_IMR */
>>>>>>> refs/remotes/origin/cm-10.0
	struct clk *clk;
	struct clk *pclk;
	struct msm_hs_tx tx;
	struct msm_hs_rx rx;
	/* gsbi uarts have to do additional writes to gsbi memory */
	/* block and top control status block. The following pointers */
	/* keep a handle to these blocks. */
	unsigned char __iomem	*mapped_gsbi;
=======
/**
 * struct msm_hs_port
 * @uport: embedded uart port structure
 * @imr_reg: shadow value of UARTDM_IMR
 * @clk: uart input clock handle
 * @tx: Tx transaction related data structure
 * @rx: Rx transaction related data structure
 * @dma_tx_channel: Tx DMA command channel
 * @dma_rx_channel Rx DMA command channel
 * @dma_tx_crci: Tx channel rate control interface number
 * @dma_rx_crci: Rx channel rate control interface number
 * @clk_off_timer: Timer to poll DMA event completion before clock off
 * @clk_off_delay: clk_off_timer poll interval
 * @clk_state: overall clock state
 * @clk_req_off_state: post flush clock states
 * @rx_wakeup: optional rx_wakeup feature related data
 * @exit_lpm_cb: optional callback to exit low power mode
 *
 * Low level serial port structure.
 */
=======
>>>>>>> refs/remotes/origin/cm-11.0
struct msm_hs_port {
	struct uart_port uport;
	unsigned int imr_reg;  /* shadow value of UARTDM_IMR */
	struct clk *clk;
	struct clk *pclk;
	struct msm_hs_tx tx;
	struct msm_hs_rx rx;
<<<<<<< HEAD

>>>>>>> refs/remotes/origin/master
=======
	/* gsbi uarts have to do additional writes to gsbi memory */
	/* block and top control status block. The following pointers */
	/* keep a handle to these blocks. */
	unsigned char __iomem	*mapped_gsbi;
>>>>>>> refs/remotes/origin/cm-11.0
	int dma_tx_channel;
	int dma_rx_channel;
	int dma_tx_crci;
	int dma_rx_crci;
<<<<<<< HEAD
<<<<<<< HEAD
	struct hrtimer clk_off_timer;  /* to poll TXEMT before clock off */
=======

	struct hrtimer clk_off_timer;
>>>>>>> refs/remotes/origin/master
=======
	struct hrtimer clk_off_timer;  /* to poll TXEMT before clock off */
>>>>>>> refs/remotes/origin/cm-11.0
	ktime_t clk_off_delay;
	enum msm_hs_clk_states_e clk_state;
	enum msm_hs_clk_req_off_state_e clk_req_off_state;

<<<<<<< HEAD
<<<<<<< HEAD
	struct msm_hs_wakeup wakeup;
	struct wake_lock dma_wake_lock;  /* held while any DMA active */
<<<<<<< HEAD
=======
=======
	struct msm_hs_wakeup wakeup;
	struct wake_lock dma_wake_lock;  /* held while any DMA active */
>>>>>>> refs/remotes/origin/cm-11.0

	struct dentry *loopback_dir;
	struct work_struct clock_off_w; /* work for actual clock off */
	struct workqueue_struct *hsuart_wq; /* hsuart workqueue */
	struct mutex clk_mutex; /* mutex to guard against clock off/clock on */
	bool tty_flush_receive;
	bool rx_discard_flush_issued;
	bool is_shutdown;
	bool termios_in_progress;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct msm_hs_rx_wakeup rx_wakeup;
	void (*exit_lpm_cb)(struct uart_port *);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
};

#define MSM_UARTDM_BURST_SIZE 16   /* DM burst size (in bytes) */
#define UARTDM_TX_BUF_SIZE UART_XMIT_SIZE
#define UARTDM_RX_BUF_SIZE 512
<<<<<<< HEAD
<<<<<<< HEAD
#define RETRY_TIMEOUT 5
#define UARTDM_NR 2
<<<<<<< HEAD
=======
#define RX_FLUSH_COMPLETE_TIMEOUT 300 /* In jiffies */
>>>>>>> refs/remotes/origin/cm-10.0

static struct dentry *debug_base;
=======

=======
#define RETRY_TIMEOUT 5
>>>>>>> refs/remotes/origin/cm-11.0
#define UARTDM_NR 2
#define RX_FLUSH_COMPLETE_TIMEOUT 300 /* In jiffies */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
static struct dentry *debug_base;
>>>>>>> refs/remotes/origin/cm-11.0
static struct msm_hs_port q_uart_port[UARTDM_NR];
static struct platform_driver msm_serial_hs_platform_driver;
static struct uart_driver msm_hs_driver;
static struct uart_ops msm_hs_ops;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
static void msm_hs_start_rx_locked(struct uart_port *uport);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct workqueue_struct *msm_hs_workqueue;
>>>>>>> refs/remotes/origin/master
=======
static void msm_hs_start_rx_locked(struct uart_port *uport);
>>>>>>> refs/remotes/origin/cm-11.0

#define UARTDM_TO_MSM(uart_port) \
	container_of((uart_port), struct msm_hs_port, uport)

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
static ssize_t show_clock(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	int state = 1;
	enum msm_hs_clk_states_e clk_state;
	unsigned long flags;

	struct platform_device *pdev = container_of(dev, struct
						    platform_device, dev);
	struct msm_hs_port *msm_uport = &q_uart_port[pdev->id];

	spin_lock_irqsave(&msm_uport->uport.lock, flags);
	clk_state = msm_uport->clk_state;
	spin_unlock_irqrestore(&msm_uport->uport.lock, flags);

	if (clk_state <= MSM_HS_CLK_OFF)
		state = 0;

	return snprintf(buf, PAGE_SIZE, "%d\n", state);
}

static ssize_t set_clock(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
<<<<<<< HEAD
{
	int state;
	struct platform_device *pdev = container_of(dev, struct
						    platform_device, dev);
	struct msm_hs_port *msm_uport = &q_uart_port[pdev->id];

	state = buf[0] - '0';
	switch (state) {
	case 0: {
		msm_hs_request_clock_off(&msm_uport->uport);
		break;
	}
	case 1: {
		msm_hs_request_clock_on(&msm_uport->uport);
		break;
	}
	default: {
		return -EINVAL;
	}
	}
	return count;
}

static DEVICE_ATTR(clock, S_IWUSR | S_IRUGO, show_clock, set_clock);

static inline unsigned int use_low_power_wakeup(struct msm_hs_port *msm_uport)
{
	return (msm_uport->wakeup.irq > 0);
}

static inline int is_gsbi_uart(struct msm_hs_port *msm_uport)
{
	/* assume gsbi uart if gsbi resource found in pdata */
	return ((msm_uport->mapped_gsbi != NULL));
}

static inline unsigned int msm_hs_read(struct uart_port *uport,
				       unsigned int offset)
{
	return readl_relaxed(uport->membase + offset);
}

static inline void msm_hs_write(struct uart_port *uport, unsigned int offset,
				 unsigned int value)
{
	writel_relaxed(value, uport->membase + offset);
}

<<<<<<< HEAD
=======
/*
 * This API read and provides UART Core registers information.
 */
static void print_uart_registers(struct msm_hs_port *msm_uport)
{
	struct uart_port *uport = &(msm_uport->uport);

	pr_info("============= UART Registers ================\n");

	pr_info("UART_DM_MR1:%x\n", msm_hs_read(uport, UARTDM_MR1_ADDR));
	pr_info("UART_DM_MR2:%x\n", msm_hs_read(uport, UARTDM_MR2_ADDR));
	pr_info("UART_DM_IPR:%x\n", msm_hs_read(uport, UARTDM_IPR_ADDR));
	pr_info("UART_DM_TFWR:%x\n", msm_hs_read(uport, UARTDM_TFWR_ADDR));
	pr_info("UART_DM_RFWR:%x\n", msm_hs_read(uport, UARTDM_RFWR_ADDR));
	pr_info("UART_DM_DMEN:%x\n", msm_hs_read(uport, UARTDM_DMEN_ADDR));
	pr_info("UART_DM_SR:%x\n", msm_hs_read(uport, UARTDM_SR_ADDR));
	pr_info("UART_DM_ISR:%x\n", msm_hs_read(uport, UARTDM_ISR_ADDR));
	pr_info("UART_DM_TXFS:%x\n", msm_hs_read(uport, UARTDM_TXFS_ADDR));
	pr_info("UART_DM_RXFS:%x\n", msm_hs_read(uport, UARTDM_RXFS_ADDR));
	pr_info("UART_DM_IMR:%x\n", msm_uport->imr_reg);

	pr_info("=============================================\n");

}

>>>>>>> refs/remotes/origin/cm-10.0
static void msm_hs_release_port(struct uart_port *port)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(port);
	struct platform_device *pdev = to_platform_device(port->dev);
	struct resource *gsbi_resource;
	resource_size_t size;

	if (is_gsbi_uart(msm_uport)) {
		iowrite32(GSBI_PROTOCOL_IDLE, msm_uport->mapped_gsbi +
			  GSBI_CONTROL_ADDR);
		gsbi_resource = platform_get_resource_byname(pdev,
							     IORESOURCE_MEM,
							     "gsbi_resource");
		if (unlikely(!gsbi_resource))
			return;

		size = resource_size(gsbi_resource);
		release_mem_region(gsbi_resource->start, size);
		iounmap(msm_uport->mapped_gsbi);
		msm_uport->mapped_gsbi = NULL;
	}
=======
static unsigned int use_low_power_rx_wakeup(struct msm_hs_port
						   *msm_uport)
=======
>>>>>>> refs/remotes/origin/cm-11.0
{
	int state;
	struct platform_device *pdev = container_of(dev, struct
						    platform_device, dev);
	struct msm_hs_port *msm_uport = &q_uart_port[pdev->id];

	state = buf[0] - '0';
	switch (state) {
	case 0: {
		msm_hs_request_clock_off(&msm_uport->uport);
		break;
	}
	case 1: {
		msm_hs_request_clock_on(&msm_uport->uport);
		break;
	}
	default: {
		return -EINVAL;
	}
	}
	return count;
}

static DEVICE_ATTR(clock, S_IWUSR | S_IRUGO, show_clock, set_clock);

static inline unsigned int use_low_power_wakeup(struct msm_hs_port *msm_uport)
{
	return (msm_uport->wakeup.irq > 0);
}

static inline int is_gsbi_uart(struct msm_hs_port *msm_uport)
{
	/* assume gsbi uart if gsbi resource found in pdata */
	return ((msm_uport->mapped_gsbi != NULL));
}

static inline unsigned int msm_hs_read(struct uart_port *uport,
				       unsigned int offset)
{
	return readl_relaxed(uport->membase + offset);
}

static inline void msm_hs_write(struct uart_port *uport, unsigned int offset,
				 unsigned int value)
{
	writel_relaxed(value, uport->membase + offset);
}

/*
 * This API read and provides UART Core registers information.
 */
static void print_uart_registers(struct msm_hs_port *msm_uport)
{
	struct uart_port *uport = &(msm_uport->uport);

	pr_info("============= UART Registers ================\n");

	pr_info("UART_DM_MR1:%x\n", msm_hs_read(uport, UARTDM_MR1_ADDR));
	pr_info("UART_DM_MR2:%x\n", msm_hs_read(uport, UARTDM_MR2_ADDR));
	pr_info("UART_DM_IPR:%x\n", msm_hs_read(uport, UARTDM_IPR_ADDR));
	pr_info("UART_DM_TFWR:%x\n", msm_hs_read(uport, UARTDM_TFWR_ADDR));
	pr_info("UART_DM_RFWR:%x\n", msm_hs_read(uport, UARTDM_RFWR_ADDR));
	pr_info("UART_DM_DMEN:%x\n", msm_hs_read(uport, UARTDM_DMEN_ADDR));
	pr_info("UART_DM_SR:%x\n", msm_hs_read(uport, UARTDM_SR_ADDR));
	pr_info("UART_DM_ISR:%x\n", msm_hs_read(uport, UARTDM_ISR_ADDR));
	pr_info("UART_DM_TXFS:%x\n", msm_hs_read(uport, UARTDM_TXFS_ADDR));
	pr_info("UART_DM_RXFS:%x\n", msm_hs_read(uport, UARTDM_RXFS_ADDR));
	pr_info("UART_DM_IMR:%x\n", msm_uport->imr_reg);

	pr_info("=============================================\n");

}

static void msm_hs_release_port(struct uart_port *port)
{
<<<<<<< HEAD
	iounmap(port->membase);
>>>>>>> refs/remotes/origin/master
=======
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(port);
	struct platform_device *pdev = to_platform_device(port->dev);
	struct resource *gsbi_resource;
	resource_size_t size;

	if (is_gsbi_uart(msm_uport)) {
		iowrite32(GSBI_PROTOCOL_IDLE, msm_uport->mapped_gsbi +
			  GSBI_CONTROL_ADDR);
		gsbi_resource = platform_get_resource_byname(pdev,
							     IORESOURCE_MEM,
							     "gsbi_resource");
		if (unlikely(!gsbi_resource))
			return;

		size = resource_size(gsbi_resource);
		release_mem_region(gsbi_resource->start, size);
		iounmap(msm_uport->mapped_gsbi);
		msm_uport->mapped_gsbi = NULL;
	}
>>>>>>> refs/remotes/origin/cm-11.0
}

static int msm_hs_request_port(struct uart_port *port)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(port);
	struct platform_device *pdev = to_platform_device(port->dev);
	struct resource *gsbi_resource;
	resource_size_t size;

	gsbi_resource = platform_get_resource_byname(pdev,
						     IORESOURCE_MEM,
						     "gsbi_resource");
	if (gsbi_resource) {
		size = resource_size(gsbi_resource);
		if (unlikely(!request_mem_region(gsbi_resource->start, size,
						 "msm_serial_hs")))
			return -EBUSY;
		msm_uport->mapped_gsbi = ioremap(gsbi_resource->start,
						 size);
		if (!msm_uport->mapped_gsbi) {
			release_mem_region(gsbi_resource->start, size);
			return -EBUSY;
		}
	}
	/* no gsbi uart */
	return 0;
}

static int msm_serial_loopback_enable_set(void *data, u64 val)
{
	struct msm_hs_port *msm_uport = data;
	struct uart_port *uport = &(msm_uport->uport);
	unsigned long flags;
	int ret = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	clk_enable(msm_uport->clk);
	if (msm_uport->pclk)
		clk_enable(msm_uport->pclk);
=======
	clk_prepare_enable(msm_uport->clk);
	if (msm_uport->pclk)
		clk_prepare_enable(msm_uport->pclk);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	clk_prepare_enable(msm_uport->clk);
	if (msm_uport->pclk)
		clk_prepare_enable(msm_uport->pclk);
>>>>>>> refs/remotes/origin/cm-11.0

	if (val) {
		spin_lock_irqsave(&uport->lock, flags);
		ret = msm_hs_read(uport, UARTDM_MR2_ADDR);
		ret |= UARTDM_MR2_LOOP_MODE_BMSK;
		msm_hs_write(uport, UARTDM_MR2_ADDR, ret);
		spin_unlock_irqrestore(&uport->lock, flags);
	} else {
		spin_lock_irqsave(&uport->lock, flags);
		ret = msm_hs_read(uport, UARTDM_MR2_ADDR);
		ret &= ~UARTDM_MR2_LOOP_MODE_BMSK;
		msm_hs_write(uport, UARTDM_MR2_ADDR, ret);
		spin_unlock_irqrestore(&uport->lock, flags);
	}
	/* Calling CLOCK API. Hence mb() requires here. */
	mb();
<<<<<<< HEAD
<<<<<<< HEAD
	clk_disable(msm_uport->clk);
	if (msm_uport->pclk)
		clk_disable(msm_uport->pclk);
=======
	clk_disable_unprepare(msm_uport->clk);
	if (msm_uport->pclk)
		clk_disable_unprepare(msm_uport->pclk);
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

static int msm_serial_loopback_enable_get(void *data, u64 *val)
{
	struct msm_hs_port *msm_uport = data;
	struct uart_port *uport = &(msm_uport->uport);
	unsigned long flags;
	int ret = 0;

<<<<<<< HEAD
	clk_enable(msm_uport->clk);
	if (msm_uport->pclk)
		clk_enable(msm_uport->pclk);
=======
	clk_prepare_enable(msm_uport->clk);
	if (msm_uport->pclk)
		clk_prepare_enable(msm_uport->pclk);
>>>>>>> refs/remotes/origin/cm-10.0

	spin_lock_irqsave(&uport->lock, flags);
	ret = msm_hs_read(&msm_uport->uport, UARTDM_MR2_ADDR);
	spin_unlock_irqrestore(&uport->lock, flags);

<<<<<<< HEAD
	clk_disable(msm_uport->clk);
	if (msm_uport->pclk)
		clk_disable(msm_uport->pclk);
=======
	clk_disable_unprepare(msm_uport->clk);
	if (msm_uport->pclk)
		clk_disable_unprepare(msm_uport->pclk);
>>>>>>> refs/remotes/origin/cm-10.0

	*val = (ret & UARTDM_MR2_LOOP_MODE_BMSK) ? 1 : 0;
	return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(loopback_enable_fops, msm_serial_loopback_enable_get,
			msm_serial_loopback_enable_set, "%llu\n");

/*
 * msm_serial_hs debugfs node: <debugfs_root>/msm_serial_hs/loopback.<id>
 * writing 1 turns on internal loopback mode in HW. Useful for automation
 * test scripts.
 * writing 0 disables the internal loopback mode. Default is disabled.
 */
<<<<<<< HEAD
static void __init msm_serial_debugfs_init(struct msm_hs_port *msm_uport,
=======
static void __devinit msm_serial_debugfs_init(struct msm_hs_port *msm_uport,
>>>>>>> refs/remotes/origin/cm-10.0
					   int id)
{
	char node_name[15];
	snprintf(node_name, sizeof(node_name), "loopback.%d", id);
<<<<<<< HEAD
	if (IS_ERR_OR_NULL(debugfs_create_file(node_name,
					       S_IRUGO | S_IWUSR,
					       debug_base,
					       msm_uport,
					       &loopback_enable_fops))) {
		debugfs_remove_recursive(debug_base);
	}
=======
	msm_uport->loopback_dir = debugfs_create_file(node_name,
						S_IRUGO | S_IWUSR,
						debug_base,
						msm_uport,
						&loopback_enable_fops);

	if (IS_ERR_OR_NULL(msm_uport->loopback_dir))
		pr_err("%s(): Cannot create loopback.%d debug entry",
							__func__, id);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int __devexit msm_hs_remove(struct platform_device *pdev)
=======
	port->membase = ioremap(port->mapbase, PAGE_SIZE);
	if (unlikely(!port->membase))
		return -ENOMEM;
=======
	clk_disable_unprepare(msm_uport->clk);
	if (msm_uport->pclk)
		clk_disable_unprepare(msm_uport->pclk);
>>>>>>> refs/remotes/origin/cm-11.0

	return 0;
}

<<<<<<< HEAD
static int msm_hs_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
=======
static int msm_serial_loopback_enable_get(void *data, u64 *val)
{
	struct msm_hs_port *msm_uport = data;
	struct uart_port *uport = &(msm_uport->uport);
	unsigned long flags;
	int ret = 0;

	clk_prepare_enable(msm_uport->clk);
	if (msm_uport->pclk)
		clk_prepare_enable(msm_uport->pclk);

	spin_lock_irqsave(&uport->lock, flags);
	ret = msm_hs_read(&msm_uport->uport, UARTDM_MR2_ADDR);
	spin_unlock_irqrestore(&uport->lock, flags);

	clk_disable_unprepare(msm_uport->clk);
	if (msm_uport->pclk)
		clk_disable_unprepare(msm_uport->pclk);

	*val = (ret & UARTDM_MR2_LOOP_MODE_BMSK) ? 1 : 0;
	return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(loopback_enable_fops, msm_serial_loopback_enable_get,
			msm_serial_loopback_enable_set, "%llu\n");

/*
 * msm_serial_hs debugfs node: <debugfs_root>/msm_serial_hs/loopback.<id>
 * writing 1 turns on internal loopback mode in HW. Useful for automation
 * test scripts.
 * writing 0 disables the internal loopback mode. Default is disabled.
 */
static void __devinit msm_serial_debugfs_init(struct msm_hs_port *msm_uport,
					   int id)
{
	char node_name[15];
	snprintf(node_name, sizeof(node_name), "loopback.%d", id);
	msm_uport->loopback_dir = debugfs_create_file(node_name,
						S_IRUGO | S_IWUSR,
						debug_base,
						msm_uport,
						&loopback_enable_fops);

	if (IS_ERR_OR_NULL(msm_uport->loopback_dir))
		pr_err("%s(): Cannot create loopback.%d debug entry",
							__func__, id);
}

static int __devexit msm_hs_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/cm-11.0
{

	struct msm_hs_port *msm_uport;
	struct device *dev;
<<<<<<< HEAD
<<<<<<< HEAD
	struct msm_serial_hs_platform_data *pdata = pdev->dev.platform_data;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (pdev->id < 0 || pdev->id >= UARTDM_NR) {
		printk(KERN_ERR "Invalid plaform device ID = %d\n", pdev->id);
		return -EINVAL;
	}

	msm_uport = &q_uart_port[pdev->id];
	dev = msm_uport->uport.dev;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	if (pdata && pdata->gpio_config)
		if (pdata->gpio_config(0))
			dev_err(dev, "GPIO config error\n");

	sysfs_remove_file(&pdev->dev.kobj, &dev_attr_clock.attr);
	debugfs_remove_recursive(debug_base);
=======
	sysfs_remove_file(&pdev->dev.kobj, &dev_attr_clock.attr);
	debugfs_remove(msm_uport->loopback_dir);
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
=======
	sysfs_remove_file(&pdev->dev.kobj, &dev_attr_clock.attr);
	debugfs_remove(msm_uport->loopback_dir);

>>>>>>> refs/remotes/origin/cm-11.0
	dma_unmap_single(dev, msm_uport->rx.mapped_cmd_ptr, sizeof(dmov_box),
			 DMA_TO_DEVICE);
	dma_pool_free(msm_uport->rx.pool, msm_uport->rx.buffer,
		      msm_uport->rx.rbuffer);
	dma_pool_destroy(msm_uport->rx.pool);

	dma_unmap_single(dev, msm_uport->rx.cmdptr_dmaaddr, sizeof(u32),
			 DMA_TO_DEVICE);
	dma_unmap_single(dev, msm_uport->tx.mapped_cmd_ptr_ptr, sizeof(u32),
			 DMA_TO_DEVICE);
	dma_unmap_single(dev, msm_uport->tx.mapped_cmd_ptr, sizeof(dmov_box),
			 DMA_TO_DEVICE);

<<<<<<< HEAD
<<<<<<< HEAD
	wake_lock_destroy(&msm_uport->rx.wake_lock);
	wake_lock_destroy(&msm_uport->dma_wake_lock);
<<<<<<< HEAD

	uart_remove_one_port(&msm_hs_driver, &msm_uport->uport);
	clk_put(msm_uport->clk);
=======
	destroy_workqueue(msm_uport->hsuart_wq);
	mutex_destroy(&msm_uport->clk_mutex);

	uart_remove_one_port(&msm_hs_driver, &msm_uport->uport);
	clk_put(msm_uport->clk);
	if (msm_uport->pclk)
		clk_put(msm_uport->pclk);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	uart_remove_one_port(&msm_hs_driver, &msm_uport->uport);
	clk_put(msm_uport->clk);
>>>>>>> refs/remotes/origin/master
=======
	wake_lock_destroy(&msm_uport->rx.wake_lock);
	wake_lock_destroy(&msm_uport->dma_wake_lock);
	destroy_workqueue(msm_uport->hsuart_wq);
	mutex_destroy(&msm_uport->clk_mutex);

	uart_remove_one_port(&msm_hs_driver, &msm_uport->uport);
	clk_put(msm_uport->clk);
	if (msm_uport->pclk)
		clk_put(msm_uport->pclk);
>>>>>>> refs/remotes/origin/cm-11.0

	/* Free the tx resources */
	kfree(msm_uport->tx.command_ptr);
	kfree(msm_uport->tx.command_ptr_ptr);

	/* Free the rx resources */
	kfree(msm_uport->rx.command_ptr);
	kfree(msm_uport->rx.command_ptr_ptr);

	iounmap(msm_uport->uport.membase);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int msm_hs_init_clk(struct uart_port *uport)
=======
static int msm_hs_init_clk_locked(struct uart_port *uport)
>>>>>>> refs/remotes/origin/master
=======
static int msm_hs_init_clk(struct uart_port *uport)
>>>>>>> refs/remotes/origin/cm-11.0
{
	int ret;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	wake_lock(&msm_uport->dma_wake_lock);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* Set up the MREG/NREG/DREG/MNDREG */
	ret = clk_set_rate(msm_uport->clk, uport->uartclk);
	if (ret) {
		printk(KERN_WARNING "Error setting clock rate on UART\n");
		return ret;
	}

<<<<<<< HEAD
	ret = clk_enable(msm_uport->clk);
=======
	ret = clk_prepare_enable(msm_uport->clk);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret) {
		printk(KERN_ERR "Error could not turn on UART clk\n");
		return ret;
	}
	if (msm_uport->pclk) {
<<<<<<< HEAD
		ret = clk_enable(msm_uport->pclk);
		if (ret) {
=======
		ret = clk_prepare_enable(msm_uport->pclk);
		if (ret) {
			clk_disable_unprepare(msm_uport->clk);
>>>>>>> refs/remotes/origin/cm-10.0
			dev_err(uport->dev,
				"Error could not turn on UART pclk\n");
			return ret;
		}
	}
=======
	ret = clk_enable(msm_uport->clk);
=======
	/* Set up the MREG/NREG/DREG/MNDREG */
	ret = clk_set_rate(msm_uport->clk, uport->uartclk);
>>>>>>> refs/remotes/origin/cm-11.0
	if (ret) {
		printk(KERN_WARNING "Error setting clock rate on UART\n");
		return ret;
	}

	ret = clk_prepare_enable(msm_uport->clk);
	if (ret) {
		printk(KERN_ERR "Error could not turn on UART clk\n");
		return ret;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
	if (msm_uport->pclk) {
		ret = clk_prepare_enable(msm_uport->pclk);
		if (ret) {
			clk_disable_unprepare(msm_uport->clk);
			dev_err(uport->dev,
				"Error could not turn on UART pclk\n");
			return ret;
		}
	}
>>>>>>> refs/remotes/origin/cm-11.0

	msm_uport->clk_state = MSM_HS_CLK_ON;
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
/* Enable and Disable clocks  (Used for power management) */
static void msm_hs_pm(struct uart_port *uport, unsigned int state,
		      unsigned int oldstate)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	if (use_low_power_rx_wakeup(msm_uport) ||
	    msm_uport->exit_lpm_cb)
		return;  /* ignore linux PM states,
			    use msm_hs_request_clock API */

	switch (state) {
	case 0:
		clk_enable(msm_uport->clk);
		break;
	case 3:
		clk_disable(msm_uport->clk);
		break;
	default:
		dev_err(uport->dev, "msm_serial: Unknown PM state %d\n",
			state);
	}
}

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
/*
 * programs the UARTDM_CSR register with correct bit rates
 *
 * Interrupts should be disabled before we are called, as
 * we modify Set Baud rate
<<<<<<< HEAD
<<<<<<< HEAD
 * Set receive stale interrupt level, dependant on Bit Rate
=======
 * Set receive stale interrupt level, dependent on Bit Rate
>>>>>>> refs/remotes/origin/master
=======
 * Set receive stale interrupt level, dependant on Bit Rate
>>>>>>> refs/remotes/origin/cm-11.0
 * Goal is to have around 8 ms before indicate stale.
 * roundup (((Bit Rate * .008) / 10) + 1
 */
static void msm_hs_set_bps_locked(struct uart_port *uport,
<<<<<<< HEAD
<<<<<<< HEAD
			       unsigned int bps)
{
	unsigned long rxstale;
	unsigned long data;
<<<<<<< HEAD
=======
	unsigned int curr_uartclk;
>>>>>>> refs/remotes/origin/cm-10.0
=======
				  unsigned int bps)
{
	unsigned long rxstale;
	unsigned long data;
>>>>>>> refs/remotes/origin/master
=======
			       unsigned int bps)
{
	unsigned long rxstale;
	unsigned long data;
	unsigned int curr_uartclk;
>>>>>>> refs/remotes/origin/cm-11.0
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	switch (bps) {
	case 300:
<<<<<<< HEAD
<<<<<<< HEAD
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x00);
		rxstale = 1;
		break;
	case 600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x11);
		rxstale = 1;
		break;
	case 1200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x22);
		rxstale = 1;
		break;
	case 2400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x33);
		rxstale = 1;
		break;
	case 4800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x44);
		rxstale = 1;
		break;
	case 9600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x55);
		rxstale = 2;
		break;
	case 14400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x66);
		rxstale = 3;
		break;
	case 19200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x77);
		rxstale = 4;
		break;
	case 28800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x88);
		rxstale = 6;
		break;
	case 38400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x99);
		rxstale = 8;
		break;
	case 57600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xaa);
		rxstale = 16;
		break;
	case 76800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xbb);
		rxstale = 16;
		break;
	case 115200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xcc);
		rxstale = 31;
		break;
	case 230400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xee);
		rxstale = 31;
		break;
	case 460800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xff);
=======
		msm_hs_write(uport, UARTDM_CSR_ADDR, UARTDM_CSR_75);
=======
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x00);
>>>>>>> refs/remotes/origin/cm-11.0
		rxstale = 1;
		break;
	case 600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x11);
		rxstale = 1;
		break;
	case 1200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x22);
		rxstale = 1;
		break;
	case 2400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x33);
		rxstale = 1;
		break;
	case 4800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x44);
		rxstale = 1;
		break;
	case 9600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x55);
		rxstale = 2;
		break;
	case 14400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x66);
		rxstale = 3;
		break;
	case 19200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x77);
		rxstale = 4;
		break;
	case 28800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x88);
		rxstale = 6;
		break;
	case 38400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x99);
		rxstale = 8;
		break;
	case 57600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xaa);
		rxstale = 16;
		break;
	case 76800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xbb);
		rxstale = 16;
		break;
	case 115200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xcc);
		rxstale = 31;
		break;
	case 230400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xee);
		rxstale = 31;
		break;
	case 460800:
<<<<<<< HEAD
		msm_hs_write(uport, UARTDM_CSR_ADDR, UARTDM_CSR_115200);
>>>>>>> refs/remotes/origin/master
=======
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xff);
>>>>>>> refs/remotes/origin/cm-11.0
		rxstale = 31;
		break;
	case 4000000:
	case 3686400:
	case 3200000:
	case 3500000:
	case 3000000:
	case 2500000:
	case 1500000:
	case 1152000:
	case 1000000:
	case 921600:
<<<<<<< HEAD
<<<<<<< HEAD
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xff);
		rxstale = 31;
		break;
	default:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xff);
=======
		msm_hs_write(uport, UARTDM_CSR_ADDR, UARTDM_CSR_115200);
		rxstale = 31;
		break;
	default:
		msm_hs_write(uport, UARTDM_CSR_ADDR, UARTDM_CSR_2400);
>>>>>>> refs/remotes/origin/master
=======
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xff);
		rxstale = 31;
		break;
	default:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xff);
>>>>>>> refs/remotes/origin/cm-11.0
		/* default to 9600 */
		bps = 9600;
		rxstale = 2;
		break;
	}
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/*
	 * uart baud rate depends on CSR and MND Values
	 * we are updating CSR before and then calling
	 * clk_set_rate which updates MND Values. Hence
	 * dsb requires here.
	 */
	mb();
<<<<<<< HEAD
<<<<<<< HEAD
=======
	curr_uartclk = uport->uartclk;
>>>>>>> refs/remotes/origin/cm-10.0
	if (bps > 460800) {
		uport->uartclk = bps * 16;
	} else {
		uport->uartclk = 7372800;
	}
<<<<<<< HEAD
	if (clk_set_rate(msm_uport->clk, uport->uartclk)) {
		printk(KERN_WARNING "Error setting clock rate on UART\n");
		return;
=======

	if (curr_uartclk != uport->uartclk) {
		if (clk_set_rate(msm_uport->clk, uport->uartclk)) {
			pr_err("%s(): Error setting clock rate on UART\n",
								__func__);
			WARN_ON(1);
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	data = rxstale & UARTDM_IPR_STALE_LSB_BMSK;
	data |= UARTDM_IPR_STALE_TIMEOUT_MSB_BMSK & (rxstale << 2);

	msm_hs_write(uport, UARTDM_IPR_ADDR, data);
<<<<<<< HEAD
	/*
	 * It is suggested to do reset of transmitter and receiver after
	 * changing any protocol configuration. Here Baud rate and stale
	 * timeout are getting updated. Hence reset transmitter and receiver.
	 */
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_TX);
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_RX);
=======
>>>>>>> refs/remotes/origin/cm-10.0
}


static void msm_hs_set_std_bps_locked(struct uart_port *uport,
			       unsigned int bps)
{
	unsigned long rxstale;
	unsigned long data;

	switch (bps) {
	case 9600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x99);
		rxstale = 2;
		break;
	case 14400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xaa);
		rxstale = 3;
		break;
	case 19200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xbb);
		rxstale = 4;
		break;
	case 28800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xcc);
		rxstale = 6;
		break;
	case 38400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xdd);
		rxstale = 8;
		break;
	case 57600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xee);
		rxstale = 16;
		break;
	case 115200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xff);
		rxstale = 31;
		break;
	default:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x99);
		/* default to 9600 */
		bps = 9600;
		rxstale = 2;
		break;
=======
	if (bps > 460800)
=======
	curr_uartclk = uport->uartclk;
	if (bps > 460800) {
>>>>>>> refs/remotes/origin/cm-11.0
		uport->uartclk = bps * 16;
	} else {
		uport->uartclk = 7372800;
	}

<<<<<<< HEAD
	if (clk_set_rate(msm_uport->clk, uport->uartclk)) {
		printk(KERN_WARNING "Error setting clock rate on UART\n");
		return;
>>>>>>> refs/remotes/origin/master
=======
	if (curr_uartclk != uport->uartclk) {
		if (clk_set_rate(msm_uport->clk, uport->uartclk)) {
			pr_err("%s(): Error setting clock rate on UART\n",
								__func__);
			WARN_ON(1);
		}
	}

	data = rxstale & UARTDM_IPR_STALE_LSB_BMSK;
	data |= UARTDM_IPR_STALE_TIMEOUT_MSB_BMSK & (rxstale << 2);

	msm_hs_write(uport, UARTDM_IPR_ADDR, data);
}


static void msm_hs_set_std_bps_locked(struct uart_port *uport,
			       unsigned int bps)
{
	unsigned long rxstale;
	unsigned long data;

	switch (bps) {
	case 9600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x99);
		rxstale = 2;
		break;
	case 14400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xaa);
		rxstale = 3;
		break;
	case 19200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xbb);
		rxstale = 4;
		break;
	case 28800:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xcc);
		rxstale = 6;
		break;
	case 38400:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xdd);
		rxstale = 8;
		break;
	case 57600:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xee);
		rxstale = 16;
		break;
	case 115200:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0xff);
		rxstale = 31;
		break;
	default:
		msm_hs_write(uport, UARTDM_CSR_ADDR, 0x99);
		/* default to 9600 */
		bps = 9600;
		rxstale = 2;
		break;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	data = rxstale & UARTDM_IPR_STALE_LSB_BMSK;
	data |= UARTDM_IPR_STALE_TIMEOUT_MSB_BMSK & (rxstale << 2);

	msm_hs_write(uport, UARTDM_IPR_ADDR, data);
}

/*
 * termios :  new ktermios
 * oldtermios:  old ktermios previous setting
 *
 * Configure the serial port
 */
static void msm_hs_set_termios(struct uart_port *uport,
<<<<<<< HEAD
<<<<<<< HEAD
				   struct ktermios *termios,
				   struct ktermios *oldtermios)
{
<<<<<<< HEAD
=======
	int ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			       struct ktermios *termios,
			       struct ktermios *oldtermios)
{
>>>>>>> refs/remotes/origin/master
=======
				   struct ktermios *termios,
				   struct ktermios *oldtermios)
{
	int ret;
>>>>>>> refs/remotes/origin/cm-11.0
	unsigned int bps;
	unsigned long data;
	unsigned long flags;
	unsigned int c_cflag = termios->c_cflag;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
	bool error_case = false;
>>>>>>> refs/remotes/origin/cm-11.0

	mutex_lock(&msm_uport->clk_mutex);
	spin_lock_irqsave(&uport->lock, flags);

	msm_uport->termios_in_progress = true;

	/* Disable RxStale Event Mechanism */
	msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_DISABLE);

	/* Disable all UART interrupts */
	msm_hs_write(uport, UARTDM_IMR_ADDR, 0);

	/* Enable RFR so remote UART doesn't send any data. */
	msm_hs_write(uport, UARTDM_CR_ADDR, RFR_LOW);

	/*
	 * It is quite possible that previous graceful flush is not
	 * completed and set_termios() request has been received.
	 * Hence wait here to make sure that it is completed and
	 * queued one more UART RX CMD with ADM.
	 */
	if (msm_uport->rx.dma_in_flight &&
			msm_uport->rx.flush == FLUSH_DATA_READY) {
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = wait_event_timeout(msm_uport->rx.wait,
			msm_uport->rx.flush == FLUSH_NONE,
			RX_FLUSH_COMPLETE_TIMEOUT);
		if (!ret) {
			pr_err("%s(): timeout for Rx cmd completion\n",
							__func__);
			spin_lock_irqsave(&uport->lock, flags);
			print_uart_registers(msm_uport);
			spin_unlock_irqrestore(&uport->lock, flags);
			BUG_ON(1);
		}

		spin_lock_irqsave(&uport->lock, flags);
	}

	/*
	 * Wait for queued Rx CMD to ADM driver to be programmed
	 * with ADM hardware before going and changing UART baud rate.
	 * Below udelay(500) is required as exec_cmd callback is called
	 * before actually programming ADM hardware with cmd.
	 */
	if (msm_uport->rx.dma_in_flight) {
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = wait_event_timeout(msm_uport->rx.wait,
			msm_uport->rx.cmd_exec == true,
			RX_FLUSH_COMPLETE_TIMEOUT);
		if (!ret)
			pr_err("%s(): timeout for rx cmd to be program\n",
								__func__);
		else
			udelay(500);

		spin_lock_irqsave(&uport->lock, flags);
	}

<<<<<<< HEAD
	/*
	 * Disable Rx channel of UARTDM
	 * DMA Rx Stall happens if enqueue and flush of Rx command happens
	 * concurrently. Hence before changing the baud rate/protocol
	 * configuration and sending flush command to ADM, disable the Rx
	 * channel of UARTDM.
	 * Note: should not reset the receiver here immediately as it is not
	 * suggested to do disable/reset or reset/disable at the same time.
	 */
	data = msm_hs_read(uport, UARTDM_DMEN_ADDR);
	data &= ~UARTDM_RX_DM_EN_BMSK;
	msm_hs_write(uport, UARTDM_DMEN_ADDR, data);
=======
	bool error_case = false;

	mutex_lock(&msm_uport->clk_mutex);
	spin_lock_irqsave(&uport->lock, flags);

	msm_uport->termios_in_progress = true;

	/* Disable RxStale Event Mechanism */
	msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_DISABLE);

	/* Disable all UART interrupts */
	msm_hs_write(uport, UARTDM_IMR_ADDR, 0);

	/* Enable RFR so remote UART doesn't send any data. */
	msm_hs_write(uport, UARTDM_CR_ADDR, RFR_LOW);

	/*
	 * It is quite possible that previous graceful flush is not
	 * completed and set_termios() request has been received.
	 * Hence wait here to make sure that it is completed and
	 * queued one more UART RX CMD with ADM.
	 */
	if (msm_uport->rx.dma_in_flight &&
			msm_uport->rx.flush == FLUSH_DATA_READY) {
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = wait_event_timeout(msm_uport->rx.wait,
			msm_uport->rx.flush == FLUSH_NONE,
			RX_FLUSH_COMPLETE_TIMEOUT);
		if (!ret) {
			pr_err("%s(): timeout for Rx cmd completion\n",
							__func__);
			spin_lock_irqsave(&uport->lock, flags);
			print_uart_registers(msm_uport);
			spin_unlock_irqrestore(&uport->lock, flags);
			BUG_ON(1);
		}

		spin_lock_irqsave(&uport->lock, flags);
	}

	/*
	 * Wait for queued Rx CMD to ADM driver to be programmed
	 * with ADM hardware before going and changing UART baud rate.
	 * Below udelay(500) is required as exec_cmd callback is called
	 * before actually programming ADM hardware with cmd.
	 */
	if (msm_uport->rx.dma_in_flight) {
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = wait_event_timeout(msm_uport->rx.wait,
			msm_uport->rx.cmd_exec == true,
			RX_FLUSH_COMPLETE_TIMEOUT);
		if (!ret)
			pr_err("%s(): timeout for rx cmd to be program\n",
								__func__);
		else
			udelay(500);

		spin_lock_irqsave(&uport->lock, flags);
	}
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
	/* 300 is the minimum baud support by the driver  */
	bps = uart_get_baud_rate(uport, termios, oldtermios, 200, 4000000);

	/* Temporary remapping  200 BAUD to 3.2 mbps */
	if (bps == 200)
		bps = 3200000;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	uport->uartclk = clk_get_rate(msm_uport->clk);
	if (!uport->uartclk)
		msm_hs_set_std_bps_locked(uport, bps);
	else
		msm_hs_set_bps_locked(uport, bps);
<<<<<<< HEAD
=======
	msm_hs_set_bps_locked(uport, bps);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	data = msm_hs_read(uport, UARTDM_MR2_ADDR);
	data &= ~UARTDM_MR2_PARITY_MODE_BMSK;
	/* set parity */
	if (PARENB == (c_cflag & PARENB)) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (PARODD == (c_cflag & PARODD)) {
			data |= ODD_PARITY;
		} else if (CMSPAR == (c_cflag & CMSPAR)) {
			data |= SPACE_PARITY;
		} else {
			data |= EVEN_PARITY;
		}
=======
		if (PARODD == (c_cflag & PARODD))
=======
		if (PARODD == (c_cflag & PARODD)) {
>>>>>>> refs/remotes/origin/cm-11.0
			data |= ODD_PARITY;
		} else if (CMSPAR == (c_cflag & CMSPAR)) {
			data |= SPACE_PARITY;
		} else {
			data |= EVEN_PARITY;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
		}
>>>>>>> refs/remotes/origin/cm-11.0
	}

	/* Set bits per char */
	data &= ~UARTDM_MR2_BITS_PER_CHAR_BMSK;

	switch (c_cflag & CSIZE) {
	case CS5:
		data |= FIVE_BPC;
		break;
	case CS6:
		data |= SIX_BPC;
		break;
	case CS7:
		data |= SEVEN_BPC;
		break;
	default:
		data |= EIGHT_BPC;
		break;
	}
	/* stop bits */
	if (c_cflag & CSTOPB) {
		data |= STOP_BIT_TWO;
	} else {
		/* otherwise 1 stop bit */
		data |= STOP_BIT_ONE;
	}
	data |= UARTDM_MR2_ERROR_MODE_BMSK;
	/* write parity/bits per char/stop bit configuration */
	msm_hs_write(uport, UARTDM_MR2_ADDR, data);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
	/* Configure HW flow control */
	data = msm_hs_read(uport, UARTDM_MR1_ADDR);
=======
	uport->ignore_status_mask = termios->c_iflag & INPCK;
	uport->ignore_status_mask |= termios->c_iflag & IGNPAR;
	uport->ignore_status_mask |= termios->c_iflag & IGNBRK;
>>>>>>> refs/remotes/origin/cm-11.0

	uport->read_status_mask = (termios->c_cflag & CREAD);

	/* Wait for baud rate and UART protocol parameters to set. */
	udelay(200);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	uport->ignore_status_mask = termios->c_iflag & INPCK;
	uport->ignore_status_mask |= termios->c_iflag & IGNPAR;
	uport->ignore_status_mask |= termios->c_iflag & IGNBRK;

	uport->read_status_mask = (termios->c_cflag & CREAD);

	/* Wait for baud rate and UART protocol parameters to set. */
	udelay(200);

=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* Set Transmit software time out */
	uart_update_timeout(uport, c_cflag, bps);

	/*
	 * Configure HW flow control
	 * UART Core would see status of CTS line when it is sending data
	 * to remote uart to confirm that it can receive or not.
	 * UART Core would trigger RFR if it is not having any space with
	 * RX FIFO.
	 */
	data = msm_hs_read(uport, UARTDM_MR1_ADDR);
	data &= ~(UARTDM_MR1_CTS_CTL_BMSK | UARTDM_MR1_RX_RDY_CTL_BMSK);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (c_cflag & CRTSCTS) {
		data |= UARTDM_MR1_CTS_CTL_BMSK;
		data |= UARTDM_MR1_RX_RDY_CTL_BMSK;
	}

	msm_hs_write(uport, UARTDM_MR1_ADDR, data);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
	uport->ignore_status_mask = termios->c_iflag & INPCK;
	uport->ignore_status_mask |= termios->c_iflag & IGNPAR;
	uport->read_status_mask = (termios->c_cflag & CREAD);

	msm_hs_write(uport, UARTDM_IMR_ADDR, 0);
=======
	/* Enable previously enabled all UART interrupts. */
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
>>>>>>> refs/remotes/origin/cm-11.0

	/*
	 * Invoke Force RxStale Interrupt
	 * On receiving this interrupt, send discard flush request
	 * to ADM driver and ignore all received data.
	 */
	msm_hs_write(uport, UARTDM_CR_ADDR, FORCE_STALE_EVENT);
	mb();

	msm_uport->rx_discard_flush_issued = true;

<<<<<<< HEAD
	if (msm_uport->rx.flush == FLUSH_NONE) {
<<<<<<< HEAD
		wake_lock(&msm_uport->rx.wake_lock);
		msm_uport->rx.flush = FLUSH_IGNORE;
		/*
		 * Before using dmov APIs make sure that
		 * previous writel are completed. Hence
		 * dsb requires here.
		 */
		mb();
		/* do discard flush */
		msm_dmov_stop_cmd(msm_uport->dma_rx_channel,
				  &msm_uport->rx.xfer, 0);
	}

	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
	/* calling other hardware component here clk_disable API. */
	mb();
	clk_disable(msm_uport->clk);
	spin_unlock_irqrestore(&uport->lock, flags);
=======
	/* Enable previously enabled all UART interrupts. */
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);

	/*
	 * Invoke Force RxStale Interrupt
	 * On receiving this interrupt, send discard flush request
	 * to ADM driver and ignore all received data.
	 */
	msm_hs_write(uport, UARTDM_CR_ADDR, FORCE_STALE_EVENT);
	mb();

	msm_uport->rx_discard_flush_issued = true;

=======
>>>>>>> refs/remotes/origin/cm-11.0
	/*
	 * Wait for above discard flush request for UART RX CMD to be
	 * completed. completion would be signal from rx_tlet without
	 * queueing any next UART RX CMD.
	 */
	if (msm_uport->rx.dma_in_flight) {
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = wait_event_timeout(msm_uport->rx.wait,
			msm_uport->rx_discard_flush_issued == false,
			msecs_to_jiffies(50));
			if (!ret) {
				pr_err("%s(): timeout for rx discard flush\n",
								__func__);
				spin_lock_irqsave(&uport->lock, flags);
				print_uart_registers(msm_uport);
				error_case = true;
			}
		if (!error_case)
			spin_lock_irqsave(&uport->lock, flags);
	} else {
		spin_unlock_irqrestore(&uport->lock, flags);
		pr_err("%s(): called with rx.dma_in_flight:%d\n",
				__func__, msm_uport->rx.dma_in_flight);
		print_uart_registers(msm_uport);
		spin_lock_irqsave(&uport->lock, flags);
<<<<<<< HEAD
	}

	/* Start Rx Transfer */
	msm_hs_start_rx_locked(&msm_uport->uport);

	/* Disable RFR so remote UART can send data. */
	msm_hs_write(uport, UARTDM_CR_ADDR, RFR_HIGH);
	mb();

	msm_uport->termios_in_progress = false;

	spin_unlock_irqrestore(&uport->lock, flags);
	mutex_unlock(&msm_uport->clk_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		msm_uport->rx.flush = FLUSH_IGNORE;
		msm_dmov_stop_cmd(msm_uport->dma_rx_channel, NULL, 1);
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}

	/* Start Rx Transfer */
	msm_hs_start_rx_locked(&msm_uport->uport);

	/* Disable RFR so remote UART can send data. */
	msm_hs_write(uport, UARTDM_CR_ADDR, RFR_HIGH);
	mb();

	msm_uport->termios_in_progress = false;

	spin_unlock_irqrestore(&uport->lock, flags);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
	mutex_unlock(&msm_uport->clk_mutex);
>>>>>>> refs/remotes/origin/cm-11.0
}

/*
 *  Standard API, Transmitter
 *  Any character in the transmit shift register is sent
 */
<<<<<<< HEAD
<<<<<<< HEAD
unsigned int msm_hs_tx_empty(struct uart_port *uport)
{
	unsigned int data;
	unsigned int ret = 0;
<<<<<<< HEAD
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	clk_enable(msm_uport->clk);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
static unsigned int msm_hs_tx_empty(struct uart_port *uport)
{
	unsigned int data;
	unsigned int ret = 0;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	clk_enable(msm_uport->clk);
>>>>>>> refs/remotes/origin/master
=======
unsigned int msm_hs_tx_empty(struct uart_port *uport)
{
	unsigned int data;
	unsigned int ret = 0;
>>>>>>> refs/remotes/origin/cm-11.0

	data = msm_hs_read(uport, UARTDM_SR_ADDR);
	if (data & UARTDM_SR_TXEMT_BMSK)
		ret = TIOCSER_TEMT;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	clk_disable(msm_uport->clk);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}
EXPORT_SYMBOL(msm_hs_tx_empty);
=======
	clk_disable(msm_uport->clk);

	return ret;
}
>>>>>>> refs/remotes/origin/master
=======
	return ret;
}
EXPORT_SYMBOL(msm_hs_tx_empty);
>>>>>>> refs/remotes/origin/cm-11.0

/*
 *  Standard API, Stop transmitter.
 *  Any character in the transmit shift register is sent as
 *  well as the current data mover transfer .
 */
static void msm_hs_stop_tx_locked(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	msm_uport->tx.tx_ready_int_en = 0;
}

/*
 *  Standard API, Stop receiver as soon as possible.
 *
 *  Function immediately terminates the operation of the
 *  channel receiver and any incoming characters are lost. None
 *  of the receiver status bits are affected by this command and
 *  characters that are already in the receive FIFO there.
 */
static void msm_hs_stop_rx_locked(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
	unsigned int data;
=======
>>>>>>> refs/remotes/origin/cm-11.0

	/* Disable RxStale Event Mechanism */
	msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_DISABLE);

	/* Enable RFR so remote UART doesn't send any data. */
	msm_hs_write(uport, UARTDM_CR_ADDR, RFR_LOW);

<<<<<<< HEAD
<<<<<<< HEAD
	/* calling DMOV or CLOCK API. Hence mb() */
	mb();
	/* Disable the receiver */
	if (msm_uport->rx.flush == FLUSH_NONE) {
		wake_lock(&msm_uport->rx.wake_lock);
		/* do discard flush */
		msm_dmov_stop_cmd(msm_uport->dma_rx_channel,
				  &msm_uport->rx.xfer, 0);
	}
=======
	/* Disable the receiver */
	if (msm_uport->rx.flush == FLUSH_NONE)
		msm_dmov_stop_cmd(msm_uport->dma_rx_channel, NULL, 1);
=======
	/* Allow to receive all pending data from UART RX FIFO */
	udelay(100);

	if (msm_uport->rx.flush == FLUSH_NONE) {
		wake_lock(&msm_uport->rx.wake_lock);

		msm_uport->rx_discard_flush_issued = true;

		/*
		 * Invoke Force RxStale Interrupt
		 * On receiving this interrupt, send discard flush request
		 * to ADM driver and ignore all received data.
		 */
		msm_hs_write(uport, UARTDM_CR_ADDR, FORCE_STALE_EVENT);
		mb();
	}
>>>>>>> refs/remotes/origin/cm-11.0

>>>>>>> refs/remotes/origin/master
	if (msm_uport->rx.flush != FLUSH_SHUTDOWN)
		msm_uport->rx.flush = FLUSH_STOP;
<<<<<<< HEAD

	clk_disable(msm_uport->clk);
<<<<<<< HEAD
=======

	/* Disable RxStale Event Mechanism */
	msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_DISABLE);

	/* Enable RFR so remote UART doesn't send any data. */
	msm_hs_write(uport, UARTDM_CR_ADDR, RFR_LOW);

	/* Allow to receive all pending data from UART RX FIFO */
	udelay(100);

	if (msm_uport->rx.flush == FLUSH_NONE) {
		wake_lock(&msm_uport->rx.wake_lock);

		msm_uport->rx_discard_flush_issued = true;

		/*
		 * Invoke Force RxStale Interrupt
		 * On receiving this interrupt, send discard flush request
		 * to ADM driver and ignore all received data.
		 */
		msm_hs_write(uport, UARTDM_CR_ADDR, FORCE_STALE_EVENT);
		mb();
	}

	if (msm_uport->rx.flush != FLUSH_SHUTDOWN)
		msm_uport->rx.flush = FLUSH_STOP;
>>>>>>> refs/remotes/origin/cm-10.0
}
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
}
>>>>>>> refs/remotes/origin/master

/*  Transmit the next chunk of data */
static void msm_hs_submit_tx_locked(struct uart_port *uport)
{
	int left;
	int tx_count;
<<<<<<< HEAD
<<<<<<< HEAD
	int aligned_tx_count;
	dma_addr_t src_addr;
	dma_addr_t aligned_src_addr;
=======
	dma_addr_t src_addr;
>>>>>>> refs/remotes/origin/master
=======
	int aligned_tx_count;
	dma_addr_t src_addr;
	dma_addr_t aligned_src_addr;
>>>>>>> refs/remotes/origin/cm-11.0
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	struct msm_hs_tx *tx = &msm_uport->tx;
	struct circ_buf *tx_buf = &msm_uport->uport.state->xmit;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (tx->dma_in_flight || msm_uport->is_shutdown)
		return;

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	if (tx->dma_in_flight || msm_uport->is_shutdown)
		return;

>>>>>>> refs/remotes/origin/cm-11.0
	if (uart_circ_empty(tx_buf) || uport->state->port.tty->stopped) {
		msm_hs_stop_tx_locked(uport);
		return;
	}

	tx->dma_in_flight = 1;

	tx_count = uart_circ_chars_pending(tx_buf);

	if (UARTDM_TX_BUF_SIZE < tx_count)
		tx_count = UARTDM_TX_BUF_SIZE;

	left = UART_XMIT_SIZE - tx_buf->tail;

	if (tx_count > left)
		tx_count = left;

	src_addr = tx->dma_base + tx_buf->tail;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* Mask the src_addr to align on a cache
	 * and add those bytes to tx_count */
	aligned_src_addr = src_addr & ~(dma_get_cache_alignment() - 1);
	aligned_tx_count = tx_count + src_addr - aligned_src_addr;

	dma_sync_single_for_device(uport->dev, aligned_src_addr,
			aligned_tx_count, DMA_TO_DEVICE);
<<<<<<< HEAD
=======
	dma_sync_single_for_device(uport->dev, src_addr, tx_count,
				   DMA_TO_DEVICE);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	tx->command_ptr->num_rows = (((tx_count + 15) >> 4) << 16) |
				     ((tx_count + 15) >> 4);
	tx->command_ptr->src_row_addr = src_addr;

	dma_sync_single_for_device(uport->dev, tx->mapped_cmd_ptr,
				   sizeof(dmov_box), DMA_TO_DEVICE);

	*tx->command_ptr_ptr = CMD_PTR_LP | DMOV_CMD_ADDR(tx->mapped_cmd_ptr);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	dma_sync_single_for_device(uport->dev, tx->mapped_cmd_ptr_ptr,
				   sizeof(u32), DMA_TO_DEVICE);

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* Save tx_count to use in Callback */
	tx->tx_count = tx_count;
	msm_hs_write(uport, UARTDM_NCF_TX_ADDR, tx_count);

	/* Disable the tx_ready interrupt */
	msm_uport->imr_reg &= ~UARTDM_ISR_TX_READY_BMSK;
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* Calling next DMOV API. Hence mb() here. */
	mb();

	dma_sync_single_for_device(uport->dev, tx->mapped_cmd_ptr_ptr,
				   sizeof(u32), DMA_TO_DEVICE);
<<<<<<< HEAD
<<<<<<< HEAD

=======
	msm_uport->tx.flush = FLUSH_NONE;
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	msm_uport->tx.flush = FLUSH_NONE;
>>>>>>> refs/remotes/origin/cm-11.0
	msm_dmov_enqueue_cmd(msm_uport->dma_tx_channel, &tx->xfer);
}

/* Start to receive the next chunk of data */
static void msm_hs_start_rx_locked(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int buffer_pending = msm_uport->rx.buffer_pending;
	unsigned int data;

<<<<<<< HEAD
=======
=======
	unsigned int buffer_pending = msm_uport->rx.buffer_pending;
	unsigned int data;

>>>>>>> refs/remotes/origin/cm-11.0
	if (msm_uport->rx.dma_in_flight) {
		pr_err("%s(): RX CMD is already queued.\n", __func__);
		print_uart_registers(msm_uport);
		BUG_ON(1);
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	msm_uport->rx.buffer_pending = 0;
	if (buffer_pending && hs_serial_debug_mask)
		printk(KERN_ERR "Error: rx started in buffer state = %x",
		       buffer_pending);
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_STALE_INT);
	msm_hs_write(uport, UARTDM_DMRX_ADDR, UARTDM_RX_BUF_SIZE);
	msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_ENABLE);
	msm_uport->imr_reg |= UARTDM_ISR_RXLEV_BMSK;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0

	/*
	 * Enable UARTDM Rx Interface as previously it has been
	 * disable in set_termios before configuring baud rate.
	 */
	data = msm_hs_read(uport, UARTDM_DMEN_ADDR);
	data |= UARTDM_RX_DM_EN_BMSK;
	msm_hs_write(uport, UARTDM_DMEN_ADDR, data);
<<<<<<< HEAD
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
	/* Calling next DMOV API. Hence mb() here. */
	mb();

	msm_uport->rx.flush = FLUSH_NONE;
<<<<<<< HEAD
	msm_dmov_enqueue_cmd(msm_uport->dma_rx_channel, &msm_uport->rx.xfer);

=======
	msm_uport->rx.dma_in_flight = true;
	msm_uport->rx.cmd_exec = false;
	msm_dmov_enqueue_cmd_ext(msm_uport->dma_rx_channel,
					&msm_uport->rx.xfer);
	/*
	 * wake up set_termios() process if it is waiting for
	 * completion of previous UART RX CMD and queueing of
	 * new UART RX CMD before going ahead with baud rate
	 * change process.
	 */
	if (msm_uport->termios_in_progress)
		wake_up(&msm_uport->rx.wait);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void flip_insert_work(struct work_struct *work)
{
	unsigned long flags;
	int retval;
	struct msm_hs_port *msm_uport =
		container_of(work, struct msm_hs_port,
			     rx.flip_insert_work.work);
	struct tty_struct *tty = msm_uport->uport.state->port.tty;

	spin_lock_irqsave(&msm_uport->uport.lock, flags);
	if (msm_uport->rx.buffer_pending == NONE_PENDING) {
		if (hs_serial_debug_mask)
			printk(KERN_ERR "Error: No buffer pending in %s",
			       __func__);
		return;
	}
	if (msm_uport->rx.buffer_pending & FIFO_OVERRUN) {
		retval = tty_insert_flip_char(tty, 0, TTY_OVERRUN);
		if (retval)
			msm_uport->rx.buffer_pending &= ~FIFO_OVERRUN;
	}
	if (msm_uport->rx.buffer_pending & PARITY_ERROR) {
		retval = tty_insert_flip_char(tty, 0, TTY_PARITY);
		if (retval)
			msm_uport->rx.buffer_pending &= ~PARITY_ERROR;
	}
	if (msm_uport->rx.buffer_pending & CHARS_NORMAL) {
		int rx_count, rx_offset;
		rx_count = (msm_uport->rx.buffer_pending & 0xFFFF0000) >> 16;
		rx_offset = (msm_uport->rx.buffer_pending & 0xFFD0) >> 5;
		retval = tty_insert_flip_string(tty, msm_uport->rx.buffer +
						rx_offset, rx_count);
		msm_uport->rx.buffer_pending &= (FIFO_OVERRUN |
						 PARITY_ERROR);
		if (retval != rx_count)
			msm_uport->rx.buffer_pending |= CHARS_NORMAL |
				retval << 8 | (rx_count - retval) << 16;
	}
	if (msm_uport->rx.buffer_pending)
		schedule_delayed_work(&msm_uport->rx.flip_insert_work,
				      msecs_to_jiffies(RETRY_TIMEOUT));
	else
		if ((msm_uport->clk_state == MSM_HS_CLK_ON) &&
		    (msm_uport->rx.flush <= FLUSH_IGNORE)) {
			if (hs_serial_debug_mask)
				printk(KERN_WARNING
				       "msm_serial_hs: "
				       "Pending buffers cleared. "
				       "Restarting\n");
			msm_hs_start_rx_locked(&msm_uport->uport);
		}
	spin_unlock_irqrestore(&msm_uport->uport.lock, flags);
	tty_flip_buffer_push(tty);
}

static void msm_serial_hs_rx_tlet(unsigned long tlet_ptr)
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
	/* Calling next DMOV API. Hence mb() here. */
	mb();

	msm_uport->rx.flush = FLUSH_NONE;
	msm_uport->rx.dma_in_flight = true;
	msm_uport->rx.cmd_exec = false;
	msm_dmov_enqueue_cmd_ext(msm_uport->dma_rx_channel,
					&msm_uport->rx.xfer);
	/*
	 * wake up set_termios() process if it is waiting for
	 * completion of previous UART RX CMD and queueing of
	 * new UART RX CMD before going ahead with baud rate
	 * change process.
	 */
	if (msm_uport->termios_in_progress)
		wake_up(&msm_uport->rx.wait);
}

static void flip_insert_work(struct work_struct *work)
{
	unsigned long flags;
	int retval;
	struct msm_hs_port *msm_uport =
		container_of(work, struct msm_hs_port,
			     rx.flip_insert_work.work);
	struct tty_struct *tty = msm_uport->uport.state->port.tty;

	spin_lock_irqsave(&msm_uport->uport.lock, flags);
	if (msm_uport->rx.buffer_pending == NONE_PENDING) {
		if (hs_serial_debug_mask)
			printk(KERN_ERR "Error: No buffer pending in %s",
			       __func__);
		return;
	}
	if (msm_uport->rx.buffer_pending & FIFO_OVERRUN) {
		retval = tty_insert_flip_char(tty, 0, TTY_OVERRUN);
		if (retval)
			msm_uport->rx.buffer_pending &= ~FIFO_OVERRUN;
	}
	if (msm_uport->rx.buffer_pending & PARITY_ERROR) {
		retval = tty_insert_flip_char(tty, 0, TTY_PARITY);
		if (retval)
			msm_uport->rx.buffer_pending &= ~PARITY_ERROR;
	}
	if (msm_uport->rx.buffer_pending & CHARS_NORMAL) {
		int rx_count, rx_offset;
		rx_count = (msm_uport->rx.buffer_pending & 0xFFFF0000) >> 16;
		rx_offset = (msm_uport->rx.buffer_pending & 0xFFD0) >> 5;
		retval = tty_insert_flip_string(tty, msm_uport->rx.buffer +
						rx_offset, rx_count);
		msm_uport->rx.buffer_pending &= (FIFO_OVERRUN |
						 PARITY_ERROR);
		if (retval != rx_count)
			msm_uport->rx.buffer_pending |= CHARS_NORMAL |
				retval << 8 | (rx_count - retval) << 16;
	}
	if (msm_uport->rx.buffer_pending)
		schedule_delayed_work(&msm_uport->rx.flip_insert_work,
				      msecs_to_jiffies(RETRY_TIMEOUT));
	else
		if ((msm_uport->clk_state == MSM_HS_CLK_ON) &&
		    (msm_uport->rx.flush <= FLUSH_IGNORE)) {
			if (hs_serial_debug_mask)
				printk(KERN_WARNING
				       "msm_serial_hs: "
				       "Pending buffers cleared. "
				       "Restarting\n");
			msm_hs_start_rx_locked(&msm_uport->uport);
		}
	spin_unlock_irqrestore(&msm_uport->uport.lock, flags);
	tty_flip_buffer_push(tty);
}

<<<<<<< HEAD
/*
 * This routine is called when we are done with a DMA transfer or the
 * a flush has been sent to the data mover driver.
 *
 * This routine is registered with Data mover when we set up a Data Mover
 *  transfer. It is called from Data mover ISR when the DMA transfer is done.
 */
static void msm_hs_dmov_rx_callback(struct msm_dmov_cmd *cmd_ptr,
					unsigned int result,
					struct msm_dmov_errdata *err)
>>>>>>> refs/remotes/origin/master
=======
static void msm_serial_hs_rx_tlet(unsigned long tlet_ptr)
>>>>>>> refs/remotes/origin/cm-11.0
{
	int retval;
	int rx_count;
	unsigned long status;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long flags;
	unsigned int error_f = 0;
	struct uart_port *uport;
	struct msm_hs_port *msm_uport;
	unsigned int flush;
	struct tty_struct *tty;

	msm_uport = container_of((struct tasklet_struct *)tlet_ptr,
				 struct msm_hs_port, rx.tlet);
	uport = &msm_uport->uport;
	tty = uport->state->port.tty;

	status = msm_hs_read(uport, UARTDM_SR_ADDR);

	spin_lock_irqsave(&uport->lock, flags);

<<<<<<< HEAD
	clk_enable(msm_uport->clk);
	msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_DISABLE);

=======
	msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_DISABLE);

	/* Rx DMA cmd is completed here. */
	msm_uport->rx.dma_in_flight = false;

>>>>>>> refs/remotes/origin/cm-10.0
	/* overflow is not connect to data in a FIFO */
	if (unlikely((status & UARTDM_SR_OVERRUN_BMSK) &&
		     (uport->read_status_mask & CREAD))) {
		retval = tty_insert_flip_char(tty, 0, TTY_OVERRUN);
		if (!retval)
			msm_uport->rx.buffer_pending |= TTY_OVERRUN;
=======
	unsigned int error_f = 0;
	unsigned long flags;
	unsigned int flush;
	struct tty_port *port;
=======
	unsigned long flags;
	unsigned int error_f = 0;
>>>>>>> refs/remotes/origin/cm-11.0
	struct uart_port *uport;
	struct msm_hs_port *msm_uport;
	unsigned int flush;
	struct tty_struct *tty;

	msm_uport = container_of((struct tasklet_struct *)tlet_ptr,
				 struct msm_hs_port, rx.tlet);
	uport = &msm_uport->uport;
	tty = uport->state->port.tty;

	status = msm_hs_read(uport, UARTDM_SR_ADDR);

<<<<<<< HEAD
	port = &uport->state->port;
=======
	spin_lock_irqsave(&uport->lock, flags);
>>>>>>> refs/remotes/origin/cm-11.0

	msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_DISABLE);

	/* Rx DMA cmd is completed here. */
	msm_uport->rx.dma_in_flight = false;

	/* overflow is not connect to data in a FIFO */
	if (unlikely((status & UARTDM_SR_OVERRUN_BMSK) &&
		     (uport->read_status_mask & CREAD))) {
<<<<<<< HEAD
		tty_insert_flip_char(port, 0, TTY_OVERRUN);
>>>>>>> refs/remotes/origin/master
=======
		retval = tty_insert_flip_char(tty, 0, TTY_OVERRUN);
		if (!retval)
			msm_uport->rx.buffer_pending |= TTY_OVERRUN;
>>>>>>> refs/remotes/origin/cm-11.0
		uport->icount.buf_overrun++;
		error_f = 1;
	}

	if (!(uport->ignore_status_mask & INPCK))
		status = status & ~(UARTDM_SR_PAR_FRAME_BMSK);

	if (unlikely(status & UARTDM_SR_PAR_FRAME_BMSK)) {
		/* Can not tell difference between parity & frame error */
		uport->icount.parity++;
		error_f = 1;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		if (uport->ignore_status_mask & IGNPAR) {
			retval = tty_insert_flip_char(tty, 0, TTY_PARITY);
			if (!retval)
				msm_uport->rx.buffer_pending |= TTY_PARITY;
		}
<<<<<<< HEAD
=======
		if (uport->ignore_status_mask & IGNPAR)
			tty_insert_flip_char(port, 0, TTY_PARITY);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}

	if (error_f)
		msm_hs_write(uport, UARTDM_CR_ADDR, RESET_ERROR_STATUS);

	if (msm_uport->clk_req_off_state == CLK_REQ_OFF_FLUSH_ISSUED)
		msm_uport->clk_req_off_state = CLK_REQ_OFF_RXSTALE_FLUSHED;
<<<<<<< HEAD
<<<<<<< HEAD
	flush = msm_uport->rx.flush;
<<<<<<< HEAD
	if (flush == FLUSH_IGNORE)
		if (!msm_uport->rx.buffer_pending)
			msm_hs_start_rx_locked(uport);

	if (flush == FLUSH_STOP) {
		msm_uport->rx.flush = FLUSH_SHUTDOWN;
		wake_up(&msm_uport->rx.wait);
	}
=======

	/*
	 * Part hs_isr() sets the flush as FLUSH_IGNORE if it is explictily
	 * trigger from set_termios().
	 */
	if (flush == FLUSH_IGNORE) {
		if (!msm_uport->rx_discard_flush_issued &&
					!msm_uport->rx.buffer_pending) {
			msm_hs_start_rx_locked(uport);
		} else {
			msm_uport->rx_discard_flush_issued = false;
			wake_up(&msm_uport->rx.wait);
			goto out;
		}
	}

	/* Part of stop_rx sets the flush as FLUSH_STOP */
	if (flush == FLUSH_STOP) {
		if (msm_uport->rx_discard_flush_issued)
			msm_uport->rx_discard_flush_issued = false;
		msm_uport->rx.flush = FLUSH_SHUTDOWN;
		wake_up(&msm_uport->rx.wait);
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======

=======
>>>>>>> refs/remotes/origin/cm-11.0
	flush = msm_uport->rx.flush;

	/*
	 * Part hs_isr() sets the flush as FLUSH_IGNORE if it is explictily
	 * trigger from set_termios().
	 */
	if (flush == FLUSH_IGNORE) {
		if (!msm_uport->rx_discard_flush_issued &&
					!msm_uport->rx.buffer_pending) {
			msm_hs_start_rx_locked(uport);
		} else {
			msm_uport->rx_discard_flush_issued = false;
			wake_up(&msm_uport->rx.wait);
			goto out;
		}
	}

	/* Part of stop_rx sets the flush as FLUSH_STOP */
	if (flush == FLUSH_STOP) {
		if (msm_uport->rx_discard_flush_issued)
			msm_uport->rx_discard_flush_issued = false;
		msm_uport->rx.flush = FLUSH_SHUTDOWN;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
		wake_up(&msm_uport->rx.wait);
	}

>>>>>>> refs/remotes/origin/cm-11.0
	if (flush >= FLUSH_DATA_INVALID)
		goto out;

	rx_count = msm_hs_read(uport, UARTDM_RX_TOTAL_SNAP_ADDR);

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* order the read of rx.buffer */
	rmb();

	if (0 != (uport->read_status_mask & CREAD)) {
		retval = tty_insert_flip_string(tty, msm_uport->rx.buffer,
						rx_count);
		if (retval != rx_count) {
			msm_uport->rx.buffer_pending |= CHARS_NORMAL |
				retval << 5 | (rx_count - retval) << 16;
		}
<<<<<<< HEAD
	}

	/* order the read of rx.buffer and the start of next rx xfer */
	wmb();

	if (!msm_uport->rx.buffer_pending)
		msm_hs_start_rx_locked(uport);

out:
	if (msm_uport->rx.buffer_pending) {
		if (hs_serial_debug_mask)
			printk(KERN_WARNING
			       "msm_serial_hs: "
			       "tty buffer exhausted. "
			       "Stalling\n");
		schedule_delayed_work(&msm_uport->rx.flip_insert_work
				      , msecs_to_jiffies(RETRY_TIMEOUT));
	}
<<<<<<< HEAD
	clk_disable(msm_uport->clk);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* release wakelock in 500ms, not immediately, because higher layers
	 * don't always take wakelocks when they should */
	wake_lock_timeout(&msm_uport->rx.wake_lock, HZ / 2);
	/* tty_flip_buffer_push() might call msm_hs_start(), so unlock */
	spin_unlock_irqrestore(&uport->lock, flags);
	if (flush < FLUSH_DATA_INVALID)
		tty_flip_buffer_push(tty);
}

/* Enable the transmitter Interrupt */
static void msm_hs_start_tx_locked(struct uart_port *uport )
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

<<<<<<< HEAD
	clk_enable(msm_uport->clk);
=======
	if (msm_uport->is_shutdown)
		return;
>>>>>>> refs/remotes/origin/cm-10.0

	if (msm_uport->tx.tx_ready_int_en == 0) {
		msm_uport->tx.tx_ready_int_en = 1;
		if (msm_uport->tx.dma_in_flight == 0)
			msm_hs_submit_tx_locked(uport);
	}
<<<<<<< HEAD

	clk_disable(msm_uport->clk);
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 *  This routine is called when we are done with a DMA transfer
 *
 *  This routine is registered with Data mover when we set
 *  up a Data Mover transfer. It is called from Data mover ISR
 *  when the DMA transfer is done.
 */
static void msm_hs_dmov_tx_callback(struct msm_dmov_cmd *cmd_ptr,
					unsigned int result,
					struct msm_dmov_errdata *err)
{
	struct msm_hs_port *msm_uport;

<<<<<<< HEAD
	WARN_ON(result != 0x80000002);  /* DMA did not finish properly */

	msm_uport = container_of(cmd_ptr, struct msm_hs_port, tx.xfer);
=======
	msm_uport = container_of(cmd_ptr, struct msm_hs_port, tx.xfer);
	if (msm_uport->tx.flush == FLUSH_STOP)
		/* DMA FLUSH unsuccesfful */
		WARN_ON(!(result & DMOV_RSLT_FLUSH));
	else
		/* DMA did not finish properly */
		WARN_ON(!(result & DMOV_RSLT_DONE));
>>>>>>> refs/remotes/origin/cm-10.0

	tasklet_schedule(&msm_uport->tx.tlet);
}

static void msm_serial_hs_tx_tlet(unsigned long tlet_ptr)
{
	unsigned long flags;
	struct msm_hs_port *msm_uport = container_of((struct tasklet_struct *)
				tlet_ptr, struct msm_hs_port, tx.tlet);
<<<<<<< HEAD

	spin_lock_irqsave(&(msm_uport->uport.lock), flags);
	clk_enable(msm_uport->clk);
=======
	struct msm_hs_tx *tx = &msm_uport->tx;

	spin_lock_irqsave(&(msm_uport->uport.lock), flags);

	tx->dma_in_flight = 0;
	if (msm_uport->tx.flush == FLUSH_STOP) {
		msm_uport->tx.flush = FLUSH_SHUTDOWN;
		wake_up(&msm_uport->tx.wait);
		spin_unlock_irqrestore(&(msm_uport->uport.lock), flags);
		return;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	msm_uport->imr_reg |= UARTDM_ISR_TX_READY_BMSK;
	msm_hs_write(&(msm_uport->uport), UARTDM_IMR_ADDR, msm_uport->imr_reg);
	/* Calling clk API. Hence mb() requires. */
	mb();

<<<<<<< HEAD
	clk_disable(msm_uport->clk);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock_irqrestore(&(msm_uport->uport.lock), flags);
}

/*
<<<<<<< HEAD
=======
 * This routine is called when ADM driver is about to program queued UART
 * RX CMD with ADM hardware.
 */
static void msm_hs_dmov_rx_exec_callback(struct msm_dmov_cmd *cmd_ptr)
{
	struct msm_hs_port *msm_uport;
	struct msm_hs_rx *rx;

	msm_uport = container_of(cmd_ptr, struct msm_hs_port, rx.xfer);
	rx = &msm_uport->rx;

	rx->cmd_exec = true;
	/*
	 * wakeup set_termios() as  it waits for UART RX CMD to be programmed
	 * with ADM hardware.
	 */
	wake_up(&msm_uport->rx.wait);
}

/*
>>>>>>> refs/remotes/origin/cm-10.0
 * This routine is called when we are done with a DMA transfer or the
 * a flush has been sent to the data mover driver.
 *
 * This routine is registered with Data mover when we set up a Data Mover
 *  transfer. It is called from Data mover ISR when the DMA transfer is done.
 */
static void msm_hs_dmov_rx_callback(struct msm_dmov_cmd *cmd_ptr,
					unsigned int result,
					struct msm_dmov_errdata *err)
{
	struct msm_hs_port *msm_uport;

<<<<<<< HEAD
=======
	if (result & DMOV_RSLT_ERROR)
		pr_err("%s(): DMOV_RSLT_ERROR\n", __func__);

>>>>>>> refs/remotes/origin/cm-10.0
	msm_uport = container_of(cmd_ptr, struct msm_hs_port, rx.xfer);

	tasklet_schedule(&msm_uport->rx.tlet);
=======
	if (0 != (uport->read_status_mask & CREAD)) {
		retval = tty_insert_flip_string(port, msm_uport->rx.buffer,
						rx_count);
		BUG_ON(retval != rx_count);
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}

	/* order the read of rx.buffer and the start of next rx xfer */
	wmb();

	if (!msm_uport->rx.buffer_pending)
		msm_hs_start_rx_locked(uport);

out:
	if (msm_uport->rx.buffer_pending) {
		if (hs_serial_debug_mask)
			printk(KERN_WARNING
			       "msm_serial_hs: "
			       "tty buffer exhausted. "
			       "Stalling\n");
		schedule_delayed_work(&msm_uport->rx.flip_insert_work
				      , msecs_to_jiffies(RETRY_TIMEOUT));
	}
	/* release wakelock in 500ms, not immediately, because higher layers
	 * don't always take wakelocks when they should */
	wake_lock_timeout(&msm_uport->rx.wake_lock, HZ / 2);
	/* tty_flip_buffer_push() might call msm_hs_start(), so unlock */
	spin_unlock_irqrestore(&uport->lock, flags);
	if (flush < FLUSH_DATA_INVALID)
		tty_flip_buffer_push(tty);
}

/* Enable the transmitter Interrupt */
static void msm_hs_start_tx_locked(struct uart_port *uport )
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	if (msm_uport->is_shutdown)
		return;

	if (msm_uport->tx.tx_ready_int_en == 0) {
		msm_uport->tx.tx_ready_int_en = 1;
		if (msm_uport->tx.dma_in_flight == 0)
			msm_hs_submit_tx_locked(uport);
	}
}

/*
 *  This routine is called when we are done with a DMA transfer
 *
 *  This routine is registered with Data mover when we set
 *  up a Data Mover transfer. It is called from Data mover ISR
 *  when the DMA transfer is done.
 */
static void msm_hs_dmov_tx_callback(struct msm_dmov_cmd *cmd_ptr,
					unsigned int result,
					struct msm_dmov_errdata *err)
{
	struct msm_hs_port *msm_uport;

	msm_uport = container_of(cmd_ptr, struct msm_hs_port, tx.xfer);
	if (msm_uport->tx.flush == FLUSH_STOP)
		/* DMA FLUSH unsuccesfful */
		WARN_ON(!(result & DMOV_RSLT_FLUSH));
	else
		/* DMA did not finish properly */
		WARN_ON(!(result & DMOV_RSLT_DONE));

	tasklet_schedule(&msm_uport->tx.tlet);
}

static void msm_serial_hs_tx_tlet(unsigned long tlet_ptr)
{
	unsigned long flags;
	struct msm_hs_port *msm_uport = container_of((struct tasklet_struct *)
				tlet_ptr, struct msm_hs_port, tx.tlet);
	struct msm_hs_tx *tx = &msm_uport->tx;

	spin_lock_irqsave(&(msm_uport->uport.lock), flags);

	tx->dma_in_flight = 0;
	if (msm_uport->tx.flush == FLUSH_STOP) {
		msm_uport->tx.flush = FLUSH_SHUTDOWN;
		wake_up(&msm_uport->tx.wait);
		spin_unlock_irqrestore(&(msm_uport->uport.lock), flags);
		return;
	}

	msm_uport->imr_reg |= UARTDM_ISR_TX_READY_BMSK;
	msm_hs_write(&(msm_uport->uport), UARTDM_IMR_ADDR, msm_uport->imr_reg);
	/* Calling clk API. Hence mb() requires. */
	mb();

	spin_unlock_irqrestore(&(msm_uport->uport.lock), flags);
}

/*
 * This routine is called when ADM driver is about to program queued UART
 * RX CMD with ADM hardware.
 */
static void msm_hs_dmov_rx_exec_callback(struct msm_dmov_cmd *cmd_ptr)
{
	struct msm_hs_port *msm_uport;
	struct msm_hs_rx *rx;

	msm_uport = container_of(cmd_ptr, struct msm_hs_port, rx.xfer);
	rx = &msm_uport->rx;

	rx->cmd_exec = true;
	/*
	 * wakeup set_termios() as  it waits for UART RX CMD to be programmed
	 * with ADM hardware.
	 */
	wake_up(&msm_uport->rx.wait);
}

/*
 * This routine is called when we are done with a DMA transfer or the
 * a flush has been sent to the data mover driver.
 *
 * This routine is registered with Data mover when we set up a Data Mover
 *  transfer. It is called from Data mover ISR when the DMA transfer is done.
 */
static void msm_hs_dmov_rx_callback(struct msm_dmov_cmd *cmd_ptr,
					unsigned int result,
					struct msm_dmov_errdata *err)
{
	struct msm_hs_port *msm_uport;

	if (result & DMOV_RSLT_ERROR)
		pr_err("%s(): DMOV_RSLT_ERROR\n", __func__);

<<<<<<< HEAD
static void msm_hs_tty_flip_buffer_work(struct work_struct *work)
{
	struct msm_hs_port *msm_uport =
			container_of(work, struct msm_hs_port, rx.tty_work);

	tty_flip_buffer_push(&msm_uport->uport.state->port);
>>>>>>> refs/remotes/origin/master
=======
	msm_uport = container_of(cmd_ptr, struct msm_hs_port, rx.xfer);

	tasklet_schedule(&msm_uport->rx.tlet);
>>>>>>> refs/remotes/origin/cm-11.0
}

/*
 *  Standard API, Current states of modem control inputs
 *
 * Since CTS can be handled entirely by HARDWARE we always
 * indicate clear to send and count on the TX FIFO to block when
 * it fills up.
 *
 * - TIOCM_DCD
 * - TIOCM_CTS
 * - TIOCM_DSR
 * - TIOCM_RI
 *  (Unsupported) DCD and DSR will return them high. RI will return low.
 */
static unsigned int msm_hs_get_mctrl_locked(struct uart_port *uport)
{
	return TIOCM_DSR | TIOCM_CAR | TIOCM_CTS;
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
 *  Standard API, Set or clear RFR_signal
 *
 * Set RFR high, (Indicate we are not ready for data), we disable auto
 * ready for receiving and then set RFR_N high. To set RFR to low we just turn
 * back auto ready for receiving and it should lower RFR signal
 * when hardware is ready
<<<<<<< HEAD
 */
void msm_hs_set_mctrl_locked(struct uart_port *uport,
				    unsigned int mctrl)
{
	unsigned int set_rts;
	unsigned int data;
<<<<<<< HEAD
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	clk_enable(msm_uport->clk);
=======
>>>>>>> refs/remotes/origin/cm-10.0

	/* RTS is active low */
	set_rts = TIOCM_RTS & mctrl ? 0 : 1;

	data = msm_hs_read(uport, UARTDM_MR1_ADDR);
	if (set_rts) {
		/*disable auto ready-for-receiving */
		data &= ~UARTDM_MR1_RX_RDY_CTL_BMSK;
		msm_hs_write(uport, UARTDM_MR1_ADDR, data);
		/* set RFR_N to high */
		msm_hs_write(uport, UARTDM_CR_ADDR, RFR_HIGH);
	} else {
		/* Enable auto ready-for-receiving */
		data |= UARTDM_MR1_RX_RDY_CTL_BMSK;
		msm_hs_write(uport, UARTDM_MR1_ADDR, data);
	}
<<<<<<< HEAD
	/* Calling CLOCK API. Hence mb() requires. */
	mb();
	clk_disable(msm_uport->clk);
=======
	mb();
>>>>>>> refs/remotes/origin/cm-10.0
}

void msm_hs_set_mctrl(struct uart_port *uport,
				    unsigned int mctrl)
{
	unsigned long flags;

	spin_lock_irqsave(&uport->lock, flags);
	msm_hs_set_mctrl_locked(uport, mctrl);
	spin_unlock_irqrestore(&uport->lock, flags);
}
EXPORT_SYMBOL(msm_hs_set_mctrl);
=======
 * True enables UART auto RFR, which indicates we are ready for data if the RX
 * buffer is not full. False disables auto RFR, and deasserts RFR to indicate
 * we are not ready for data. Must be called with UART clock on.
=======
>>>>>>> refs/remotes/origin/cm-11.0
 */
void msm_hs_set_mctrl_locked(struct uart_port *uport,
				    unsigned int mctrl)
{
	unsigned int set_rts;
	unsigned int data;

	/* RTS is active low */
	set_rts = TIOCM_RTS & mctrl ? 0 : 1;

	data = msm_hs_read(uport, UARTDM_MR1_ADDR);
	if (set_rts) {
		/*disable auto ready-for-receiving */
		data &= ~UARTDM_MR1_RX_RDY_CTL_BMSK;
		msm_hs_write(uport, UARTDM_MR1_ADDR, data);
		/* set RFR_N to high */
		msm_hs_write(uport, UARTDM_CR_ADDR, RFR_HIGH);
	} else {
		/* Enable auto ready-for-receiving */
		data |= UARTDM_MR1_RX_RDY_CTL_BMSK;
		msm_hs_write(uport, UARTDM_MR1_ADDR, data);
	}
	mb();
}

void msm_hs_set_mctrl(struct uart_port *uport,
				    unsigned int mctrl)
{
	unsigned long flags;

	spin_lock_irqsave(&uport->lock, flags);
	msm_hs_set_mctrl_locked(uport, mctrl);
	spin_unlock_irqrestore(&uport->lock, flags);
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
EXPORT_SYMBOL(msm_hs_set_mctrl);
>>>>>>> refs/remotes/origin/cm-11.0

/* Standard API, Enable modem status (CTS) interrupt  */
static void msm_hs_enable_ms_locked(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
	clk_enable(msm_uport->clk);

	/* Enable DELTA_CTS Interrupt */
	msm_uport->imr_reg |= UARTDM_ISR_DELTA_CTS_BMSK;
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
<<<<<<< HEAD
	/* Calling CLOCK API. Hence mb() requires here. */
	mb();
=======
	/* Enable DELTA_CTS Interrupt */
	msm_uport->imr_reg |= UARTDM_ISR_DELTA_CTS_BMSK;
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
	mb();

}
>>>>>>> refs/remotes/origin/cm-11.0

static void msm_hs_flush_buffer_locked(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

<<<<<<< HEAD
=======
	/* Enable DELTA_CTS Interrupt */
	msm_uport->imr_reg |= UARTDM_ISR_DELTA_CTS_BMSK;
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
	mb();

}

static void msm_hs_flush_buffer_locked(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	if (msm_uport->tx.dma_in_flight)
		msm_uport->tty_flush_receive = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======

	clk_disable(msm_uport->clk);

>>>>>>> refs/remotes/origin/master
=======
	if (msm_uport->tx.dma_in_flight)
		msm_uport->tty_flush_receive = true;
>>>>>>> refs/remotes/origin/cm-11.0
}

/*
 *  Standard API, Break Signal
 *
 * Control the transmission of a break signal. ctl eq 0 => break
 * signal terminate ctl ne 0 => start break signal
 */
static void msm_hs_break_ctl(struct uart_port *uport, int ctl)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long flags;
<<<<<<< HEAD
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	spin_lock_irqsave(&uport->lock, flags);
	clk_enable(msm_uport->clk);
	msm_hs_write(uport, UARTDM_CR_ADDR, ctl ? START_BREAK : STOP_BREAK);
	/* Calling CLOCK API. Hence mb() requires here. */
	mb();
	clk_disable(msm_uport->clk);
=======

	spin_lock_irqsave(&uport->lock, flags);
	msm_hs_write(uport, UARTDM_CR_ADDR, ctl ? START_BREAK : STOP_BREAK);
	mb();
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock_irqrestore(&uport->lock, flags);
=======
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
=======
	unsigned long flags;
>>>>>>> refs/remotes/origin/cm-11.0

	spin_lock_irqsave(&uport->lock, flags);
	msm_hs_write(uport, UARTDM_CR_ADDR, ctl ? START_BREAK : STOP_BREAK);
<<<<<<< HEAD
	clk_disable(msm_uport->clk);
>>>>>>> refs/remotes/origin/master
=======
	mb();
	spin_unlock_irqrestore(&uport->lock, flags);
>>>>>>> refs/remotes/origin/cm-11.0
}

static void msm_hs_config_port(struct uart_port *uport, int cfg_flags)
{
	unsigned long flags;
<<<<<<< HEAD
<<<<<<< HEAD
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

=======

	spin_lock_irqsave(&uport->lock, flags);
>>>>>>> refs/remotes/origin/master
=======
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

>>>>>>> refs/remotes/origin/cm-11.0
	if (cfg_flags & UART_CONFIG_TYPE) {
		uport->type = PORT_MSM;
		msm_hs_request_port(uport);
	}
<<<<<<< HEAD
<<<<<<< HEAD

<<<<<<< HEAD
	spin_lock_irqsave(&uport->lock, flags);
	if (is_gsbi_uart(msm_uport)) {
		if (msm_uport->pclk)
			clk_enable(msm_uport->pclk);
		iowrite32(GSBI_PROTOCOL_UART, msm_uport->mapped_gsbi +
			  GSBI_CONTROL_ADDR);
		if (msm_uport->pclk)
			clk_disable(msm_uport->pclk);
	}
	spin_unlock_irqrestore(&uport->lock, flags);
=======
=======

>>>>>>> refs/remotes/origin/cm-11.0
	if (is_gsbi_uart(msm_uport)) {
		if (msm_uport->pclk)
			clk_prepare_enable(msm_uport->pclk);
		spin_lock_irqsave(&uport->lock, flags);
		iowrite32(GSBI_PROTOCOL_UART, msm_uport->mapped_gsbi +
			  GSBI_CONTROL_ADDR);
		spin_unlock_irqrestore(&uport->lock, flags);
		if (msm_uport->pclk)
			clk_disable_unprepare(msm_uport->pclk);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_unlock_irqrestore(&uport->lock, flags);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

/*  Handle CTS changes (Called from interrupt handler) */
static void msm_hs_handle_delta_cts_locked(struct uart_port *uport)
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	clk_enable(msm_uport->clk);

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* clear interrupt */
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_CTS);
	/* Calling CLOCK API. Hence mb() requires here. */
	mb();
	uport->icount.cts++;

<<<<<<< HEAD
	clk_disable(msm_uport->clk);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* clear interrupt */
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_CTS);
	/* Calling CLOCK API. Hence mb() requires here. */
	mb();
	uport->icount.cts++;

<<<<<<< HEAD
	clk_disable(msm_uport->clk);

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* clear the IOCTL TIOCMIWAIT if called */
	wake_up_interruptible(&uport->state->port.delta_msr_wait);
}

/* check if the TX path is flushed, and if so clock off
 * returns 0 did not clock off, need to retry (still sending final byte)
 *        -1 did not clock off, do not retry
 *         1 if we clocked off
 */
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
static int msm_hs_check_clock_off_locked(struct uart_port *uport)
=======
static int msm_hs_check_clock_off(struct uart_port *uport)
>>>>>>> refs/remotes/origin/cm-11.0
{
	unsigned long sr_status;
	unsigned long flags;
	int ret;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	struct circ_buf *tx_buf = &uport->state->xmit;

<<<<<<< HEAD
<<<<<<< HEAD
=======
static int msm_hs_check_clock_off(struct uart_port *uport)
{
	unsigned long sr_status;
	unsigned long flags;
	int ret;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	struct circ_buf *tx_buf = &uport->state->xmit;

	mutex_lock(&msm_uport->clk_mutex);
	spin_lock_irqsave(&uport->lock, flags);

>>>>>>> refs/remotes/origin/cm-10.0
	/* Cancel if tx tty buffer is not empty, dma is in flight,
	 * or tx fifo is not empty */
	if (msm_uport->clk_state != MSM_HS_CLK_REQUEST_OFF ||
	    !uart_circ_empty(tx_buf) || msm_uport->tx.dma_in_flight ||
	    msm_uport->imr_reg & UARTDM_ISR_TXLEV_BMSK) {
<<<<<<< HEAD
=======
		spin_unlock_irqrestore(&uport->lock, flags);
		mutex_unlock(&msm_uport->clk_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
=======
=======
	mutex_lock(&msm_uport->clk_mutex);
	spin_lock_irqsave(&uport->lock, flags);

>>>>>>> refs/remotes/origin/cm-11.0
	/* Cancel if tx tty buffer is not empty, dma is in flight,
	 * or tx fifo is not empty */
	if (msm_uport->clk_state != MSM_HS_CLK_REQUEST_OFF ||
	    !uart_circ_empty(tx_buf) || msm_uport->tx.dma_in_flight ||
<<<<<<< HEAD
	    (msm_uport->imr_reg & UARTDM_ISR_TXLEV_BMSK) ||
	    !(msm_uport->imr_reg & UARTDM_ISR_RXLEV_BMSK))  {
>>>>>>> refs/remotes/origin/master
=======
	    msm_uport->imr_reg & UARTDM_ISR_TXLEV_BMSK) {
		spin_unlock_irqrestore(&uport->lock, flags);
		mutex_unlock(&msm_uport->clk_mutex);
>>>>>>> refs/remotes/origin/cm-11.0
		return -1;
	}

	/* Make sure the uart is finished with the last byte */
	sr_status = msm_hs_read(uport, UARTDM_SR_ADDR);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(sr_status & UARTDM_SR_TXEMT_BMSK))
		return 0;  /* retry */
=======
	if (!(sr_status & UARTDM_SR_TXEMT_BMSK)) {
		spin_unlock_irqrestore(&uport->lock, flags);
		mutex_unlock(&msm_uport->clk_mutex);
		return 0;  /* retry */
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!(sr_status & UARTDM_SR_TXEMT_BMSK))
		return 0;  /* retry */
>>>>>>> refs/remotes/origin/master
=======
	if (!(sr_status & UARTDM_SR_TXEMT_BMSK)) {
		spin_unlock_irqrestore(&uport->lock, flags);
		mutex_unlock(&msm_uport->clk_mutex);
		return 0;  /* retry */
	}
>>>>>>> refs/remotes/origin/cm-11.0

	/* Make sure forced RXSTALE flush complete */
	switch (msm_uport->clk_req_off_state) {
	case CLK_REQ_OFF_START:
		msm_uport->clk_req_off_state = CLK_REQ_OFF_RXSTALE_ISSUED;
		msm_hs_write(uport, UARTDM_CR_ADDR, FORCE_STALE_EVENT);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		/*
		 * Before returning make sure that device writel completed.
		 * Hence mb() requires here.
		 */
		mb();
<<<<<<< HEAD
<<<<<<< HEAD
		return 0;  /* RXSTALE flush not complete - retry */
	case CLK_REQ_OFF_RXSTALE_ISSUED:
	case CLK_REQ_OFF_FLUSH_ISSUED:
=======
		spin_unlock_irqrestore(&uport->lock, flags);
		mutex_unlock(&msm_uport->clk_mutex);
		return 0;  /* RXSTALE flush not complete - retry */
	case CLK_REQ_OFF_RXSTALE_ISSUED:
	case CLK_REQ_OFF_FLUSH_ISSUED:
		spin_unlock_irqrestore(&uport->lock, flags);
		mutex_unlock(&msm_uport->clk_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return 0;  /* RXSTALE flush not complete - retry */
	case CLK_REQ_OFF_RXSTALE_ISSUED:
	case CLK_REQ_OFF_FLUSH_ISSUED:
>>>>>>> refs/remotes/origin/master
=======
		spin_unlock_irqrestore(&uport->lock, flags);
		mutex_unlock(&msm_uport->clk_mutex);
		return 0;  /* RXSTALE flush not complete - retry */
	case CLK_REQ_OFF_RXSTALE_ISSUED:
	case CLK_REQ_OFF_FLUSH_ISSUED:
		spin_unlock_irqrestore(&uport->lock, flags);
		mutex_unlock(&msm_uport->clk_mutex);
>>>>>>> refs/remotes/origin/cm-11.0
		return 0;  /* RXSTALE flush not complete - retry */
	case CLK_REQ_OFF_RXSTALE_FLUSHED:
		break;  /* continue */
	}

<<<<<<< HEAD
	if (msm_uport->rx.flush != FLUSH_SHUTDOWN) {
<<<<<<< HEAD
		if (msm_uport->rx.flush == FLUSH_NONE)
			msm_hs_stop_rx_locked(uport);
		return 0;  /* come back later to really clock off */
	}

	/* we really want to clock off */
	clk_disable(msm_uport->clk);
	if (msm_uport->pclk)
		clk_disable(msm_uport->pclk);
	msm_uport->clk_state = MSM_HS_CLK_OFF;
=======
		if (msm_uport->rx.flush == FLUSH_NONE) {
			msm_hs_stop_rx_locked(uport);
			msm_uport->rx_discard_flush_issued = true;
		}

		spin_unlock_irqrestore(&uport->lock, flags);
		if (msm_uport->rx_discard_flush_issued) {
			pr_debug("%s(): wainting for flush completion.\n",
								__func__);
			ret = wait_event_timeout(msm_uport->rx.wait,
				msm_uport->rx_discard_flush_issued == false,
				RX_FLUSH_COMPLETE_TIMEOUT);
			if (!ret)
				pr_err("%s(): Flush complete pending.\n",
								__func__);
		}

		mutex_unlock(&msm_uport->clk_mutex);
		return 0;  /* come back later to really clock off */
	}

	spin_unlock_irqrestore(&uport->lock, flags);

	/* we really want to clock off */
	clk_disable_unprepare(msm_uport->clk);
	if (msm_uport->pclk)
		clk_disable_unprepare(msm_uport->pclk);

	msm_uport->clk_state = MSM_HS_CLK_OFF;

	spin_lock_irqsave(&uport->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
	if (use_low_power_wakeup(msm_uport)) {
		msm_uport->wakeup.ignore = 1;
		enable_irq(msm_uport->wakeup.irq);
	}
	wake_unlock(&msm_uport->dma_wake_lock);
<<<<<<< HEAD
	return 1;
}

static enum hrtimer_restart msm_hs_clk_off_retry(struct hrtimer *timer) {
=======
	if (msm_uport->rx.flush != FLUSH_SHUTDOWN) {
		if (msm_uport->rx.flush == FLUSH_NONE) {
			msm_hs_stop_rx_locked(uport);
			msm_uport->rx_discard_flush_issued = true;
		}

		spin_unlock_irqrestore(&uport->lock, flags);
		if (msm_uport->rx_discard_flush_issued) {
			pr_debug("%s(): wainting for flush completion.\n",
								__func__);
			ret = wait_event_timeout(msm_uport->rx.wait,
				msm_uport->rx_discard_flush_issued == false,
				RX_FLUSH_COMPLETE_TIMEOUT);
			if (!ret)
				pr_err("%s(): Flush complete pending.\n",
								__func__);
		}

		mutex_unlock(&msm_uport->clk_mutex);
		return 0;  /* come back later to really clock off */
	}

	spin_unlock_irqrestore(&uport->lock, flags);

	/* we really want to clock off */
	clk_disable_unprepare(msm_uport->clk);
	if (msm_uport->pclk)
		clk_disable_unprepare(msm_uport->pclk);

	msm_uport->clk_state = MSM_HS_CLK_OFF;

	spin_lock_irqsave(&uport->lock, flags);
	if (use_low_power_wakeup(msm_uport)) {
		msm_uport->wakeup.ignore = 1;
		enable_irq(msm_uport->wakeup.irq);
	}
	wake_unlock(&msm_uport->dma_wake_lock);

	spin_unlock_irqrestore(&uport->lock, flags);
	mutex_unlock(&msm_uport->clk_mutex);
	return 1;
}

static void hsuart_clock_off_work(struct work_struct *w)
{
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	int ret = HRTIMER_NORESTART;
	struct msm_hs_port *msm_uport = container_of(timer, struct msm_hs_port,
						     clk_off_timer);
=======
	struct msm_hs_port *msm_uport = container_of(w, struct msm_hs_port,
							clock_off_w);
>>>>>>> refs/remotes/origin/cm-11.0
	struct uart_port *uport = &msm_uport->uport;

	if (!msm_hs_check_clock_off(uport)) {
		hrtimer_start(&msm_uport->clk_off_timer,
				msm_uport->clk_off_delay,
				HRTIMER_MODE_REL);
	}
}

static enum hrtimer_restart msm_hs_clk_off_retry(struct hrtimer *timer)
{
	struct msm_hs_port *msm_uport = container_of(timer, struct msm_hs_port,
							clk_off_timer);

<<<<<<< HEAD
	return ret;
<<<<<<< HEAD
=======

	spin_unlock_irqrestore(&uport->lock, flags);
	mutex_unlock(&msm_uport->clk_mutex);
	return 1;
}

static void hsuart_clock_off_work(struct work_struct *w)
{
	struct msm_hs_port *msm_uport = container_of(w, struct msm_hs_port,
							clock_off_w);
	struct uart_port *uport = &msm_uport->uport;

	if (!msm_hs_check_clock_off(uport)) {
		hrtimer_start(&msm_uport->clk_off_timer,
				msm_uport->clk_off_delay,
				HRTIMER_MODE_REL);
	}
}

static enum hrtimer_restart msm_hs_clk_off_retry(struct hrtimer *timer)
{
	struct msm_hs_port *msm_uport = container_of(timer, struct msm_hs_port,
							clk_off_timer);

	queue_work(msm_uport->hsuart_wq, &msm_uport->clock_off_w);
	return HRTIMER_NORESTART;
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	queue_work(msm_uport->hsuart_wq, &msm_uport->clock_off_w);
	return HRTIMER_NORESTART;
>>>>>>> refs/remotes/origin/cm-11.0
}

static irqreturn_t msm_hs_isr(int irq, void *dev)
{
	unsigned long flags;
	unsigned long isr_status;
<<<<<<< HEAD
<<<<<<< HEAD
	struct msm_hs_port *msm_uport = (struct msm_hs_port *)dev;
=======
	struct msm_hs_port *msm_uport = dev;
>>>>>>> refs/remotes/origin/master
=======
	struct msm_hs_port *msm_uport = (struct msm_hs_port *)dev;
>>>>>>> refs/remotes/origin/cm-11.0
	struct uart_port *uport = &msm_uport->uport;
	struct circ_buf *tx_buf = &uport->state->xmit;
	struct msm_hs_tx *tx = &msm_uport->tx;
	struct msm_hs_rx *rx = &msm_uport->rx;

	spin_lock_irqsave(&uport->lock, flags);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (msm_uport->is_shutdown) {
		pr_err("%s(): Received UART interrupt after shutdown.\n",
								__func__);
		spin_unlock_irqrestore(&uport->lock, flags);
		return IRQ_HANDLED;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	isr_status = msm_hs_read(uport, UARTDM_MISR_ADDR);

	/* Uart RX starting */
	if (isr_status & UARTDM_ISR_RXLEV_BMSK) {
<<<<<<< HEAD
<<<<<<< HEAD
		wake_lock(&rx->wake_lock);  /* hold wakelock while rx dma */
		msm_uport->imr_reg &= ~UARTDM_ISR_RXLEV_BMSK;
		msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
		/* Complete device write for IMR. Hence mb() requires. */
		mb();
=======
		msm_uport->imr_reg &= ~UARTDM_ISR_RXLEV_BMSK;
		msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
>>>>>>> refs/remotes/origin/master
=======
		wake_lock(&rx->wake_lock);  /* hold wakelock while rx dma */
		msm_uport->imr_reg &= ~UARTDM_ISR_RXLEV_BMSK;
		msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
		/* Complete device write for IMR. Hence mb() requires. */
		mb();
>>>>>>> refs/remotes/origin/cm-11.0
	}
	/* Stale rx interrupt */
	if (isr_status & UARTDM_ISR_RXSTALE_BMSK) {
		msm_hs_write(uport, UARTDM_CR_ADDR, STALE_EVENT_DISABLE);
		msm_hs_write(uport, UARTDM_CR_ADDR, RESET_STALE_INT);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		/*
		 * Complete device write before calling DMOV API. Hence
		 * mb() requires here.
		 */
		mb();
<<<<<<< HEAD

		if (msm_uport->clk_req_off_state == CLK_REQ_OFF_RXSTALE_ISSUED)
			msm_uport->clk_req_off_state =
				CLK_REQ_OFF_FLUSH_ISSUED;

		if (rx->flush == FLUSH_NONE) {
<<<<<<< HEAD
			rx->flush = FLUSH_DATA_READY;
			msm_dmov_flush(msm_uport->dma_rx_channel);
		}
	}
=======
			if (!msm_uport->termios_in_progress) {
				rx->flush = FLUSH_DATA_READY;
				/* Graceful Flush */
				msm_dmov_flush(msm_uport->dma_rx_channel, 1);
			} else {
				rx->flush = FLUSH_IGNORE;
				/* Discard Flush */
				msm_dmov_flush(msm_uport->dma_rx_channel, 0);
			}
		}

		/*
		 * Force RxStale is performed from msm_hs_stop_rx_locked() and
		 * change rx.flush to FLUSH_STOP.
		 */
		if (rx->flush == FLUSH_STOP) {
			if (msm_uport->rx_discard_flush_issued)
				/* Discard Flush */
				msm_dmov_flush(msm_uport->dma_rx_channel, 0);
		}
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0

		if (msm_uport->clk_req_off_state == CLK_REQ_OFF_RXSTALE_ISSUED)
			msm_uport->clk_req_off_state =
				CLK_REQ_OFF_FLUSH_ISSUED;

		if (rx->flush == FLUSH_NONE) {
			if (!msm_uport->termios_in_progress) {
				rx->flush = FLUSH_DATA_READY;
				/* Graceful Flush */
				msm_dmov_flush(msm_uport->dma_rx_channel, 1);
			} else {
				rx->flush = FLUSH_IGNORE;
				/* Discard Flush */
				msm_dmov_flush(msm_uport->dma_rx_channel, 0);
			}
		}

		/*
		 * Force RxStale is performed from msm_hs_stop_rx_locked() and
		 * change rx.flush to FLUSH_STOP.
		 */
		if (rx->flush == FLUSH_STOP) {
			if (msm_uport->rx_discard_flush_issued)
				/* Discard Flush */
				msm_dmov_flush(msm_uport->dma_rx_channel, 0);
		}
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======

>>>>>>> refs/remotes/origin/cm-11.0
	/* tx ready interrupt */
	if (isr_status & UARTDM_ISR_TX_READY_BMSK) {
		/* Clear  TX Ready */
		msm_hs_write(uport, UARTDM_CR_ADDR, CLEAR_TX_READY);

		if (msm_uport->clk_state == MSM_HS_CLK_REQUEST_OFF) {
			msm_uport->imr_reg |= UARTDM_ISR_TXLEV_BMSK;
			msm_hs_write(uport, UARTDM_IMR_ADDR,
				     msm_uport->imr_reg);
		}
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		/*
		 * Complete both writes before starting new TX.
		 * Hence mb() requires here.
		 */
		mb();
<<<<<<< HEAD
		/* Complete DMA TX transactions and submit new transactions */
<<<<<<< HEAD
		tx_buf->tail = (tx_buf->tail + tx->tx_count) & ~UART_XMIT_SIZE;

		tx->dma_in_flight = 0;
=======

		/* Do not update tx_buf.tail if uart_flush_buffer already
						called in serial core */
		if (!msm_uport->tty_flush_receive)
			tx_buf->tail = (tx_buf->tail +
					tx->tx_count) & ~UART_XMIT_SIZE;
		else
			msm_uport->tty_flush_receive = false;
>>>>>>> refs/remotes/origin/cm-10.0
=======

=======
>>>>>>> refs/remotes/origin/cm-11.0
		/* Complete DMA TX transactions and submit new transactions */

<<<<<<< HEAD
		tx->dma_in_flight = 0;
>>>>>>> refs/remotes/origin/master
=======
		/* Do not update tx_buf.tail if uart_flush_buffer already
						called in serial core */
		if (!msm_uport->tty_flush_receive)
			tx_buf->tail = (tx_buf->tail +
					tx->tx_count) & ~UART_XMIT_SIZE;
		else
			msm_uport->tty_flush_receive = false;
>>>>>>> refs/remotes/origin/cm-11.0

		uport->icount.tx += tx->tx_count;
		if (tx->tx_ready_int_en)
			msm_hs_submit_tx_locked(uport);

		if (uart_circ_chars_pending(tx_buf) < WAKEUP_CHARS)
			uart_write_wakeup(uport);
	}
	if (isr_status & UARTDM_ISR_TXLEV_BMSK) {
		/* TX FIFO is empty */
		msm_uport->imr_reg &= ~UARTDM_ISR_TXLEV_BMSK;
		msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		/*
		 * Complete device write before starting clock_off request.
		 * Hence mb() requires here.
		 */
		mb();
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
		if (!msm_hs_check_clock_off_locked(uport))
			hrtimer_start(&msm_uport->clk_off_timer,
				      msm_uport->clk_off_delay,
				      HRTIMER_MODE_REL);
<<<<<<< HEAD
=======
		queue_work(msm_uport->hsuart_wq, &msm_uport->clock_off_w);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
		queue_work(msm_uport->hsuart_wq, &msm_uport->clock_off_w);
>>>>>>> refs/remotes/origin/cm-11.0
	}

	/* Change in CTS interrupt */
	if (isr_status & UARTDM_ISR_DELTA_CTS_BMSK)
		msm_hs_handle_delta_cts_locked(uport);

	spin_unlock_irqrestore(&uport->lock, flags);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
/* request to turn off uart clock once pending TX is flushed */
void msm_hs_request_clock_off(struct uart_port *uport) {
	unsigned long flags;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	spin_lock_irqsave(&uport->lock, flags);
	if (msm_uport->clk_state == MSM_HS_CLK_ON) {
		msm_uport->clk_state = MSM_HS_CLK_REQUEST_OFF;
		msm_uport->clk_req_off_state = CLK_REQ_OFF_START;
		msm_uport->imr_reg |= UARTDM_ISR_TXLEV_BMSK;
		msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
		/*
		 * Complete device write before retuning back.
		 * Hence mb() requires here.
		 */
		mb();
	}
	spin_unlock_irqrestore(&uport->lock, flags);
}
EXPORT_SYMBOL(msm_hs_request_clock_off);

<<<<<<< HEAD
static void msm_hs_request_clock_on_locked(struct uart_port *uport) {
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	unsigned int data;
	int ret = 0;

	switch (msm_uport->clk_state) {
	case MSM_HS_CLK_OFF:
		wake_lock(&msm_uport->dma_wake_lock);
		clk_enable(msm_uport->clk);
		if (msm_uport->pclk)
			ret = clk_enable(msm_uport->pclk);
		disable_irq_nosync(msm_uport->wakeup.irq);
		if (unlikely(ret)) {
			dev_err(uport->dev, "Clock ON Failure"
				"Stalling HSUART\n");
			break;
		}
=======
void msm_hs_request_clock_on(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	unsigned long flags;
	unsigned int data;
	int ret = 0;

	mutex_lock(&msm_uport->clk_mutex);
	spin_lock_irqsave(&uport->lock, flags);

	switch (msm_uport->clk_state) {
	case MSM_HS_CLK_OFF:
		wake_lock(&msm_uport->dma_wake_lock);
		disable_irq_nosync(msm_uport->wakeup.irq);
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = clk_prepare_enable(msm_uport->clk);
		if (ret) {
			dev_err(uport->dev, "Clock ON Failure"
			"For UART CLK Stalling HSUART\n");
			break;
		}

<<<<<<< HEAD
		if (msm_uport->pclk) {
			ret = clk_prepare_enable(msm_uport->pclk);
			if (unlikely(ret)) {
				clk_disable_unprepare(msm_uport->clk);
				dev_err(uport->dev, "Clock ON Failure"
				"For UART Pclk Stalling HSUART\n");
				break;
			}
		}
		spin_lock_irqsave(&uport->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
		/* else fall-through */
=======
void msm_hs_request_clock_off_locked(struct uart_port *uport)
{
=======
/* request to turn off uart clock once pending TX is flushed */
void msm_hs_request_clock_off(struct uart_port *uport) {
	unsigned long flags;
>>>>>>> refs/remotes/origin/cm-11.0
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);

	spin_lock_irqsave(&uport->lock, flags);
	if (msm_uport->clk_state == MSM_HS_CLK_ON) {
		msm_uport->clk_state = MSM_HS_CLK_REQUEST_OFF;
		msm_uport->clk_req_off_state = CLK_REQ_OFF_START;
		msm_uport->imr_reg |= UARTDM_ISR_TXLEV_BMSK;
		msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
		/*
		 * Complete device write before retuning back.
		 * Hence mb() requires here.
		 */
		mb();
	}
	spin_unlock_irqrestore(&uport->lock, flags);
}
EXPORT_SYMBOL(msm_hs_request_clock_off);

void msm_hs_request_clock_on(struct uart_port *uport)
{
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	unsigned long flags;
	unsigned int data;
	int ret = 0;

	mutex_lock(&msm_uport->clk_mutex);
	spin_lock_irqsave(&uport->lock, flags);

	switch (msm_uport->clk_state) {
	case MSM_HS_CLK_OFF:
<<<<<<< HEAD
		clk_enable(msm_uport->clk);
		disable_irq_nosync(msm_uport->rx_wakeup.irq);
		/* fall-through */
>>>>>>> refs/remotes/origin/master
=======
		wake_lock(&msm_uport->dma_wake_lock);
		disable_irq_nosync(msm_uport->wakeup.irq);
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = clk_prepare_enable(msm_uport->clk);
		if (ret) {
			dev_err(uport->dev, "Clock ON Failure"
			"For UART CLK Stalling HSUART\n");
			break;
		}

		if (msm_uport->pclk) {
			ret = clk_prepare_enable(msm_uport->pclk);
			if (unlikely(ret)) {
				clk_disable_unprepare(msm_uport->clk);
				dev_err(uport->dev, "Clock ON Failure"
				"For UART Pclk Stalling HSUART\n");
				break;
			}
		}
		spin_lock_irqsave(&uport->lock, flags);
		/* else fall-through */
>>>>>>> refs/remotes/origin/cm-11.0
	case MSM_HS_CLK_REQUEST_OFF:
		if (msm_uport->rx.flush == FLUSH_STOP ||
		    msm_uport->rx.flush == FLUSH_SHUTDOWN) {
			msm_hs_write(uport, UARTDM_CR_ADDR, RESET_RX);
			data = msm_hs_read(uport, UARTDM_DMEN_ADDR);
			data |= UARTDM_RX_DM_EN_BMSK;
			msm_hs_write(uport, UARTDM_DMEN_ADDR, data);
<<<<<<< HEAD
<<<<<<< HEAD
			/* Complete above device write. Hence mb() here. */
			mb();
=======
>>>>>>> refs/remotes/origin/master
=======
			/* Complete above device write. Hence mb() here. */
			mb();
>>>>>>> refs/remotes/origin/cm-11.0
		}
		hrtimer_try_to_cancel(&msm_uport->clk_off_timer);
		if (msm_uport->rx.flush == FLUSH_SHUTDOWN)
			msm_hs_start_rx_locked(uport);
<<<<<<< HEAD
<<<<<<< HEAD
=======
		if (!use_low_power_rx_wakeup(msm_uport))
			set_rfr_locked(uport, 1);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		if (msm_uport->rx.flush == FLUSH_STOP)
			msm_uport->rx.flush = FLUSH_IGNORE;
		msm_uport->clk_state = MSM_HS_CLK_ON;
		break;
	case MSM_HS_CLK_ON:
		break;
	case MSM_HS_CLK_PORT_OFF:
		break;
	}
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
}

void msm_hs_request_clock_on(struct uart_port *uport) {
	unsigned long flags;
	spin_lock_irqsave(&uport->lock, flags);
	msm_hs_request_clock_on_locked(uport);
	spin_unlock_irqrestore(&uport->lock, flags);
=======

	spin_unlock_irqrestore(&uport->lock, flags);
	mutex_unlock(&msm_uport->clk_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
}
EXPORT_SYMBOL(msm_hs_request_clock_on);

static irqreturn_t msm_hs_wakeup_isr(int irq, void *dev)
{
	unsigned int wakeup = 0;
	unsigned long flags;
	struct msm_hs_port *msm_uport = (struct msm_hs_port *)dev;
	struct uart_port *uport = &msm_uport->uport;
	struct tty_struct *tty = NULL;

	spin_lock_irqsave(&uport->lock, flags);
	if (msm_uport->clk_state == MSM_HS_CLK_OFF)  {
		/* ignore the first irq - it is a pending irq that occured
		 * before enable_irq() */
		if (msm_uport->wakeup.ignore)
			msm_uport->wakeup.ignore = 0;
=======
}

/**
 * msm_hs_request_clock_on - Switch the device from partially active low
 * power mode to fully active (i.e. clock on) mode.
 * @uport: uart_port structure for the device.
 *
 * This function switches on the input clock, puts UART HW into DMA mode
 * and enqueues an Rx DMA command if the device was in partially active
 * mode. It has no effect if called with the device in inactive state.
 */
void msm_hs_request_clock_on(struct uart_port *uport)
{
	unsigned long flags;
=======
>>>>>>> refs/remotes/origin/cm-11.0

	spin_unlock_irqrestore(&uport->lock, flags);
	mutex_unlock(&msm_uport->clk_mutex);
}
EXPORT_SYMBOL(msm_hs_request_clock_on);

static irqreturn_t msm_hs_wakeup_isr(int irq, void *dev)
{
	unsigned int wakeup = 0;
	unsigned long flags;
	struct msm_hs_port *msm_uport = (struct msm_hs_port *)dev;
	struct uart_port *uport = &msm_uport->uport;

	spin_lock_irqsave(&uport->lock, flags);
	if (msm_uport->clk_state == MSM_HS_CLK_OFF)  {
		/* ignore the first irq - it is a pending irq that occured
		 * before enable_irq() */
<<<<<<< HEAD
		if (msm_uport->rx_wakeup.ignore)
			msm_uport->rx_wakeup.ignore = 0;
>>>>>>> refs/remotes/origin/master
=======
		if (msm_uport->wakeup.ignore)
			msm_uport->wakeup.ignore = 0;
>>>>>>> refs/remotes/origin/cm-11.0
		else
			wakeup = 1;
	}

	if (wakeup) {
		/* the uart was clocked off during an rx, wake up and
		 * optionally inject char into tty rx */
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		msm_hs_request_clock_on_locked(uport);
=======
		spin_unlock_irqrestore(&uport->lock, flags);
		msm_hs_request_clock_on(uport);
		spin_lock_irqsave(&uport->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		spin_unlock_irqrestore(&uport->lock, flags);
		msm_hs_request_clock_on(uport);
		spin_lock_irqsave(&uport->lock, flags);
>>>>>>> refs/remotes/origin/cm-11.0
		if (msm_uport->wakeup.inject_rx) {
			tty = uport->state->port.tty;
			tty_insert_flip_char(tty,
					     msm_uport->wakeup.rx_to_inject,
<<<<<<< HEAD
					     TTY_NORMAL);
=======
		msm_hs_request_clock_on_locked(uport);
		if (msm_uport->rx_wakeup.inject_rx) {
			tty_insert_flip_char(&uport->state->port,
					     msm_uport->rx_wakeup.rx_to_inject,
					     TTY_NORMAL);
			queue_work(msm_hs_workqueue, &msm_uport->rx.tty_work);
>>>>>>> refs/remotes/origin/master
=======
					     TTY_NORMAL);
>>>>>>> refs/remotes/origin/cm-11.0
		}
	}

	spin_unlock_irqrestore(&uport->lock, flags);

<<<<<<< HEAD
<<<<<<< HEAD
	if (wakeup && msm_uport->wakeup.inject_rx)
		tty_flip_buffer_push(tty);
=======
>>>>>>> refs/remotes/origin/master
=======
	if (wakeup && msm_uport->wakeup.inject_rx)
		tty_flip_buffer_push(tty);
>>>>>>> refs/remotes/origin/cm-11.0
	return IRQ_HANDLED;
}

static const char *msm_hs_type(struct uart_port *port)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return ("MSM HS UART");
=======
	return (port->type == PORT_MSM) ? "MSM_HS_UART" : NULL;
>>>>>>> refs/remotes/origin/master
=======
	return ("MSM HS UART");
>>>>>>> refs/remotes/origin/cm-11.0
}

/* Called when port is opened */
static int msm_hs_startup(struct uart_port *uport)
{
	int ret;
	int rfr_level;
	unsigned long flags;
	unsigned int data;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	struct circ_buf *tx_buf = &uport->state->xmit;
	struct msm_hs_tx *tx = &msm_uport->tx;
=======
	struct platform_device *pdev = to_platform_device(uport->dev);
	const struct msm_serial_hs_platform_data *pdata =
					pdev->dev.platform_data;
=======
>>>>>>> refs/remotes/origin/master
=======
	struct platform_device *pdev = to_platform_device(uport->dev);
	const struct msm_serial_hs_platform_data *pdata =
					pdev->dev.platform_data;
>>>>>>> refs/remotes/origin/cm-11.0
	struct circ_buf *tx_buf = &uport->state->xmit;
	struct msm_hs_tx *tx = &msm_uport->tx;
	struct msm_hs_rx *rx = &msm_uport->rx;

<<<<<<< HEAD
<<<<<<< HEAD
	msm_uport->is_shutdown = false;
	msm_uport->termios_in_progress = false;
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
=======
	msm_uport->is_shutdown = false;
	msm_uport->termios_in_progress = false;

>>>>>>> refs/remotes/origin/cm-11.0
	rfr_level = uport->fifosize;
	if (rfr_level > 16)
		rfr_level -= 16;

	tx->dma_base = dma_map_single(uport->dev, tx_buf->buf, UART_XMIT_SIZE,
				      DMA_TO_DEVICE);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	/* turn on uart clk */
	ret = msm_hs_init_clk(uport);
	if (unlikely(ret))
		return ret;
=======
	wake_lock(&msm_uport->dma_wake_lock);
	/* turn on uart clk */
	ret = msm_hs_init_clk(uport);
	if (unlikely(ret)) {
		pr_err("Turning ON uartclk error\n");
		wake_unlock(&msm_uport->dma_wake_lock);
		return ret;
	}

	if (pdata && pdata->gpio_config)
		if (unlikely(pdata->gpio_config(1)))
			dev_err(uport->dev, "Cannot configure gpios\n");
>>>>>>> refs/remotes/origin/cm-10.0

=======
	/* do not let tty layer execute RX in global workqueue, use a
	 * dedicated workqueue managed by this driver */
	uport->state->port.low_latency = 1;

=======
	wake_lock(&msm_uport->dma_wake_lock);
>>>>>>> refs/remotes/origin/cm-11.0
	/* turn on uart clk */
	ret = msm_hs_init_clk(uport);
	if (unlikely(ret)) {
		pr_err("Turning ON uartclk error\n");
		wake_unlock(&msm_uport->dma_wake_lock);
		return ret;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
	if (pdata && pdata->gpio_config)
		if (unlikely(pdata->gpio_config(1)))
			dev_err(uport->dev, "Cannot configure gpios\n");

>>>>>>> refs/remotes/origin/cm-11.0
	/* Set auto RFR Level */
	data = msm_hs_read(uport, UARTDM_MR1_ADDR);
	data &= ~UARTDM_MR1_AUTO_RFR_LEVEL1_BMSK;
	data &= ~UARTDM_MR1_AUTO_RFR_LEVEL0_BMSK;
	data |= (UARTDM_MR1_AUTO_RFR_LEVEL1_BMSK & (rfr_level << 2));
	data |= (UARTDM_MR1_AUTO_RFR_LEVEL0_BMSK & rfr_level);
	msm_hs_write(uport, UARTDM_MR1_ADDR, data);

	/* Make sure RXSTALE count is non-zero */
	data = msm_hs_read(uport, UARTDM_IPR_ADDR);
	if (!data) {
		data |= 0x1f & UARTDM_IPR_STALE_LSB_BMSK;
		msm_hs_write(uport, UARTDM_IPR_ADDR, data);
	}

	/* Enable Data Mover Mode */
	data = UARTDM_TX_DM_EN_BMSK | UARTDM_RX_DM_EN_BMSK;
	msm_hs_write(uport, UARTDM_DMEN_ADDR, data);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	/* Reset TX */
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_TX);
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_RX);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0

	/* Reset both RX and TX HW state machine */
	msm_hs_write(uport, UARTDM_CR_ADDR, (RESET_RX | RESET_TX));
	/*
	 * Rx and Tx reset operation takes few clock cycles, hence as
	 * safe side adding 10us delay.
	 */
	udelay(10);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Reset TX */
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_TX);
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_RX);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_ERROR_STATUS);
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_BREAK_INT);
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_STALE_INT);
	msm_hs_write(uport, UARTDM_CR_ADDR, RESET_CTS);
	msm_hs_write(uport, UARTDM_CR_ADDR, RFR_LOW);
	/* Turn on Uart Receiver */
	msm_hs_write(uport, UARTDM_CR_ADDR, UARTDM_CR_RX_EN_BMSK);

	/* Turn on Uart Transmitter */
	msm_hs_write(uport, UARTDM_CR_ADDR, UARTDM_CR_TX_EN_BMSK);

	/* Initialize the tx */
	tx->tx_ready_int_en = 0;
	tx->dma_in_flight = 0;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	rx->dma_in_flight = false;
	rx->cmd_exec = false;
>>>>>>> refs/remotes/origin/cm-10.0

	tx->xfer.complete_func = msm_hs_dmov_tx_callback;
=======

	tx->xfer.complete_func = msm_hs_dmov_tx_callback;
	tx->xfer.execute_func = NULL;
>>>>>>> refs/remotes/origin/master
=======
	rx->dma_in_flight = false;
	rx->cmd_exec = false;

	tx->xfer.complete_func = msm_hs_dmov_tx_callback;
>>>>>>> refs/remotes/origin/cm-11.0

	tx->command_ptr->cmd = CMD_LC |
	    CMD_DST_CRCI(msm_uport->dma_tx_crci) | CMD_MODE_BOX;

	tx->command_ptr->src_dst_len = (MSM_UARTDM_BURST_SIZE << 16)
					   | (MSM_UARTDM_BURST_SIZE);

	tx->command_ptr->row_offset = (MSM_UARTDM_BURST_SIZE << 16);

	tx->command_ptr->dst_row_addr =
	    msm_uport->uport.mapbase + UARTDM_TF_ADDR;

<<<<<<< HEAD
<<<<<<< HEAD
=======

	/* Turn on Uart Receive */
	rx->xfer.complete_func = msm_hs_dmov_rx_callback;
	rx->xfer.execute_func = NULL;

	rx->command_ptr->cmd = CMD_LC |
	    CMD_SRC_CRCI(msm_uport->dma_rx_crci) | CMD_MODE_BOX;

	rx->command_ptr->src_dst_len = (MSM_UARTDM_BURST_SIZE << 16)
					   | (MSM_UARTDM_BURST_SIZE);
	rx->command_ptr->row_offset =  MSM_UARTDM_BURST_SIZE;
	rx->command_ptr->src_row_addr = uport->mapbase + UARTDM_RF_ADDR;


>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	msm_uport->imr_reg |= UARTDM_ISR_RXSTALE_BMSK;
	/* Enable reading the current CTS, no harm even if CTS is ignored */
	msm_uport->imr_reg |= UARTDM_ISR_CURRENT_CTS_BMSK;

	msm_hs_write(uport, UARTDM_TFWR_ADDR, 0);  /* TXLEV on empty TX fifo */
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/*
	 * Complete all device write related configuration before
	 * queuing RX request. Hence mb() requires here.
	 */
	mb();
<<<<<<< HEAD

	if (use_low_power_wakeup(msm_uport)) {
		ret = irq_set_irq_wake(msm_uport->wakeup.irq, 1);
<<<<<<< HEAD
		if (unlikely(ret))
			return ret;
=======
		if (unlikely(ret)) {
			pr_err("%s():Err setting wakeup irq\n", __func__);
			goto deinit_uart_clk;
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	ret = request_irq(uport->irq, msm_hs_isr, IRQF_TRIGGER_HIGH,
			  "msm_hs_uart", msm_uport);
<<<<<<< HEAD
	if (unlikely(ret))
		return ret;
	if (use_low_power_wakeup(msm_uport)) {
		ret = request_irq(msm_uport->wakeup.irq, msm_hs_wakeup_isr,
				  IRQF_TRIGGER_FALLING,
				  "msm_hs_wakeup", msm_uport);
		if (unlikely(ret))
			return ret;
=======
	if (unlikely(ret)) {
		pr_err("%s():Error getting uart irq\n", __func__);
		goto free_wake_irq;
	}
	if (use_low_power_wakeup(msm_uport)) {

		ret = request_threaded_irq(msm_uport->wakeup.irq, NULL,
					msm_hs_wakeup_isr,
					IRQF_TRIGGER_FALLING,
					"msm_hs_wakeup", msm_uport);

		if (unlikely(ret)) {
			pr_err("%s():Err getting uart wakeup_irq\n", __func__);
			goto free_uart_irq;
		}
>>>>>>> refs/remotes/origin/cm-10.0
		disable_irq(msm_uport->wakeup.irq);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0

	if (use_low_power_wakeup(msm_uport)) {
		ret = irq_set_irq_wake(msm_uport->wakeup.irq, 1);
		if (unlikely(ret)) {
			pr_err("%s():Err setting wakeup irq\n", __func__);
			goto deinit_uart_clk;
		}
	}

	ret = request_irq(uport->irq, msm_hs_isr, IRQF_TRIGGER_HIGH,
			  "msm_hs_uart", msm_uport);
	if (unlikely(ret)) {
		pr_err("%s():Error getting uart irq\n", __func__);
		goto free_wake_irq;
	}
	if (use_low_power_wakeup(msm_uport)) {

		ret = request_threaded_irq(msm_uport->wakeup.irq, NULL,
					msm_hs_wakeup_isr,
					IRQF_TRIGGER_FALLING,
					"msm_hs_wakeup", msm_uport);

		if (unlikely(ret)) {
			pr_err("%s():Err getting uart wakeup_irq\n", __func__);
			goto free_uart_irq;
		}
<<<<<<< HEAD
		disable_irq(msm_uport->rx_wakeup.irq);
>>>>>>> refs/remotes/origin/master
=======
		disable_irq(msm_uport->wakeup.irq);
>>>>>>> refs/remotes/origin/cm-11.0
	}

	spin_lock_irqsave(&uport->lock, flags);

<<<<<<< HEAD
<<<<<<< HEAD
	msm_hs_start_rx_locked(uport);

	spin_unlock_irqrestore(&uport->lock, flags);
<<<<<<< HEAD
=======
	msm_hs_write(uport, UARTDM_RFWR_ADDR, 0);
	msm_hs_start_rx_locked(uport);

	spin_unlock_irqrestore(&uport->lock, flags);
>>>>>>> refs/remotes/origin/master
	ret = pm_runtime_set_active(uport->dev);
	if (ret)
		dev_err(uport->dev, "set active error:%d\n", ret);
=======
	msm_hs_start_rx_locked(uport);

	spin_unlock_irqrestore(&uport->lock, flags);

>>>>>>> refs/remotes/origin/cm-11.0
	pm_runtime_enable(uport->dev);

<<<<<<< HEAD

	return 0;
=======

	pm_runtime_enable(uport->dev);

	return 0;

free_uart_irq:
	free_irq(uport->irq, msm_uport);
free_wake_irq:
	irq_set_irq_wake(msm_uport->wakeup.irq, 0);
deinit_uart_clk:
	clk_disable_unprepare(msm_uport->clk);
	if (msm_uport->pclk)
		clk_disable_unprepare(msm_uport->pclk);
	wake_unlock(&msm_uport->dma_wake_lock);

	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return 0;

free_uart_irq:
	free_irq(uport->irq, msm_uport);
free_wake_irq:
	irq_set_irq_wake(msm_uport->wakeup.irq, 0);
deinit_uart_clk:
	clk_disable_unprepare(msm_uport->clk);
	if (msm_uport->pclk)
		clk_disable_unprepare(msm_uport->pclk);
	wake_unlock(&msm_uport->dma_wake_lock);

	return ret;
>>>>>>> refs/remotes/origin/master
}

/* Initialize tx and rx data structures */
static int uartdm_init_port(struct uart_port *uport)
{
	int ret = 0;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	struct msm_hs_tx *tx = &msm_uport->tx;
	struct msm_hs_rx *rx = &msm_uport->rx;

	/* Allocate the command pointer. Needs to be 64 bit aligned */
	tx->command_ptr = kmalloc(sizeof(dmov_box), GFP_KERNEL | __GFP_DMA);
	if (!tx->command_ptr)
		return -ENOMEM;

	tx->command_ptr_ptr = kmalloc(sizeof(u32), GFP_KERNEL | __GFP_DMA);
	if (!tx->command_ptr_ptr) {
		ret = -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD
		goto free_tx_command_ptr;
=======
		goto err_tx_command_ptr_ptr;
>>>>>>> refs/remotes/origin/master
=======
		goto free_tx_command_ptr;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	tx->mapped_cmd_ptr = dma_map_single(uport->dev, tx->command_ptr,
					    sizeof(dmov_box), DMA_TO_DEVICE);
	tx->mapped_cmd_ptr_ptr = dma_map_single(uport->dev,
						tx->command_ptr_ptr,
						sizeof(u32), DMA_TO_DEVICE);
	tx->xfer.cmdptr = DMOV_CMD_ADDR(tx->mapped_cmd_ptr_ptr);

	init_waitqueue_head(&rx->wait);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	init_waitqueue_head(&tx->wait);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	init_waitqueue_head(&tx->wait);
>>>>>>> refs/remotes/origin/cm-11.0
	wake_lock_init(&rx->wake_lock, WAKE_LOCK_SUSPEND, "msm_serial_hs_rx");
	wake_lock_init(&msm_uport->dma_wake_lock, WAKE_LOCK_SUSPEND,
		       "msm_serial_hs_dma");

	tasklet_init(&rx->tlet, msm_serial_hs_rx_tlet,
			(unsigned long) &rx->tlet);
	tasklet_init(&tx->tlet, msm_serial_hs_tx_tlet,
			(unsigned long) &tx->tlet);
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	rx->pool = dma_pool_create("rx_buffer_pool", uport->dev,
				   UARTDM_RX_BUF_SIZE, 16, 0);
	if (!rx->pool) {
		pr_err("%s(): cannot allocate rx_buffer_pool", __func__);
		ret = -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD
		goto exit_tasket_init;
=======
		goto err_dma_pool_create;
>>>>>>> refs/remotes/origin/master
=======
		goto exit_tasket_init;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	rx->buffer = dma_pool_alloc(rx->pool, GFP_KERNEL, &rx->rbuffer);
	if (!rx->buffer) {
		pr_err("%s(): cannot allocate rx->buffer", __func__);
		ret = -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD
		goto free_pool;
=======
		goto err_dma_pool_alloc;
>>>>>>> refs/remotes/origin/master
=======
		goto free_pool;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	/* Allocate the command pointer. Needs to be 64 bit aligned */
	rx->command_ptr = kmalloc(sizeof(dmov_box), GFP_KERNEL | __GFP_DMA);
	if (!rx->command_ptr) {
		pr_err("%s(): cannot allocate rx->command_ptr", __func__);
		ret = -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD
		goto free_rx_buffer;
=======
		goto err_rx_command_ptr;
>>>>>>> refs/remotes/origin/master
=======
		goto free_rx_buffer;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	rx->command_ptr_ptr = kmalloc(sizeof(u32), GFP_KERNEL | __GFP_DMA);
	if (!rx->command_ptr_ptr) {
		pr_err("%s(): cannot allocate rx->command_ptr_ptr", __func__);
		ret = -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD
		goto free_rx_command_ptr;
=======
		goto err_rx_command_ptr_ptr;
>>>>>>> refs/remotes/origin/master
=======
		goto free_rx_command_ptr;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	rx->command_ptr->num_rows = ((UARTDM_RX_BUF_SIZE >> 4) << 16) |
					 (UARTDM_RX_BUF_SIZE >> 4);

	rx->command_ptr->dst_row_addr = rx->rbuffer;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* Set up Uart Receive */
	msm_hs_write(uport, UARTDM_RFWR_ADDR, 0);

	rx->xfer.complete_func = msm_hs_dmov_rx_callback;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	rx->xfer.exec_func = msm_hs_dmov_rx_exec_callback;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rx->xfer.exec_func = msm_hs_dmov_rx_exec_callback;
>>>>>>> refs/remotes/origin/cm-11.0

	rx->command_ptr->cmd = CMD_LC |
	    CMD_SRC_CRCI(msm_uport->dma_rx_crci) | CMD_MODE_BOX;

	rx->command_ptr->src_dst_len = (MSM_UARTDM_BURST_SIZE << 16)
					   | (MSM_UARTDM_BURST_SIZE);
	rx->command_ptr->row_offset =  MSM_UARTDM_BURST_SIZE;
	rx->command_ptr->src_row_addr = uport->mapbase + UARTDM_RF_ADDR;

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	rx->mapped_cmd_ptr = dma_map_single(uport->dev, rx->command_ptr,
					    sizeof(dmov_box), DMA_TO_DEVICE);

	*rx->command_ptr_ptr = CMD_PTR_LP | DMOV_CMD_ADDR(rx->mapped_cmd_ptr);

	rx->cmdptr_dmaaddr = dma_map_single(uport->dev, rx->command_ptr_ptr,
					    sizeof(u32), DMA_TO_DEVICE);
	rx->xfer.cmdptr = DMOV_CMD_ADDR(rx->cmdptr_dmaaddr);

<<<<<<< HEAD
<<<<<<< HEAD
	INIT_DELAYED_WORK(&rx->flip_insert_work, flip_insert_work);

	return ret;

free_rx_command_ptr:
	kfree(rx->command_ptr);

free_rx_buffer:
	dma_pool_free(msm_uport->rx.pool, msm_uport->rx.buffer,
			msm_uport->rx.rbuffer);

free_pool:
	dma_pool_destroy(msm_uport->rx.pool);

exit_tasket_init:
	wake_lock_destroy(&msm_uport->rx.wake_lock);
	wake_lock_destroy(&msm_uport->dma_wake_lock);
	tasklet_kill(&msm_uport->tx.tlet);
	tasklet_kill(&msm_uport->rx.tlet);
	dma_unmap_single(uport->dev, msm_uport->tx.mapped_cmd_ptr_ptr,
			sizeof(u32), DMA_TO_DEVICE);
	dma_unmap_single(uport->dev, msm_uport->tx.mapped_cmd_ptr,
			sizeof(dmov_box), DMA_TO_DEVICE);
	kfree(msm_uport->tx.command_ptr_ptr);

free_tx_command_ptr:
=======
	INIT_WORK(&rx->tty_work, msm_hs_tty_flip_buffer_work);
=======
	INIT_DELAYED_WORK(&rx->flip_insert_work, flip_insert_work);
>>>>>>> refs/remotes/origin/cm-11.0

	return ret;

free_rx_command_ptr:
	kfree(rx->command_ptr);

free_rx_buffer:
	dma_pool_free(msm_uport->rx.pool, msm_uport->rx.buffer,
			msm_uport->rx.rbuffer);

free_pool:
	dma_pool_destroy(msm_uport->rx.pool);

exit_tasket_init:
	wake_lock_destroy(&msm_uport->rx.wake_lock);
	wake_lock_destroy(&msm_uport->dma_wake_lock);
	tasklet_kill(&msm_uport->tx.tlet);
	tasklet_kill(&msm_uport->rx.tlet);
	dma_unmap_single(uport->dev, msm_uport->tx.mapped_cmd_ptr_ptr,
			sizeof(u32), DMA_TO_DEVICE);
	dma_unmap_single(uport->dev, msm_uport->tx.mapped_cmd_ptr,
			sizeof(dmov_box), DMA_TO_DEVICE);
	kfree(msm_uport->tx.command_ptr_ptr);
<<<<<<< HEAD
err_tx_command_ptr_ptr:
>>>>>>> refs/remotes/origin/master
=======

free_tx_command_ptr:
>>>>>>> refs/remotes/origin/cm-11.0
	kfree(msm_uport->tx.command_ptr);
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int __init msm_hs_probe(struct platform_device *pdev)
=======
static int __devinit msm_hs_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int msm_hs_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	int ret;
	struct uart_port *uport;
	struct msm_hs_port *msm_uport;
	struct resource *resource;
<<<<<<< HEAD
<<<<<<< HEAD
	struct msm_serial_hs_platform_data *pdata = pdev->dev.platform_data;
=======
	const struct msm_serial_hs_platform_data *pdata =
						dev_get_platdata(&pdev->dev);
>>>>>>> refs/remotes/origin/master
=======
	struct msm_serial_hs_platform_data *pdata = pdev->dev.platform_data;
>>>>>>> refs/remotes/origin/cm-11.0

	if (pdev->id < 0 || pdev->id >= UARTDM_NR) {
		printk(KERN_ERR "Invalid plaform device ID = %d\n", pdev->id);
		return -EINVAL;
	}

	msm_uport = &q_uart_port[pdev->id];
	uport = &msm_uport->uport;

	uport->dev = &pdev->dev;

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return -ENXIO;
<<<<<<< HEAD
<<<<<<< HEAD
	uport->mapbase = resource->start;  /* virtual address */

	uport->membase = ioremap(uport->mapbase, PAGE_SIZE);
	if (unlikely(!uport->membase))
		return -ENOMEM;

	uport->irq = platform_get_irq(pdev, 0);
	if (unlikely((int)uport->irq < 0))
		return -ENXIO;

	if (pdata == NULL)
		msm_uport->wakeup.irq = -1;
	else {
		msm_uport->wakeup.irq = pdata->wakeup_irq;
		msm_uport->wakeup.ignore = 1;
		msm_uport->wakeup.inject_rx = pdata->inject_rx_on_wakeup;
		msm_uport->wakeup.rx_to_inject = pdata->rx_to_inject;

		if (unlikely(msm_uport->wakeup.irq < 0))
			return -ENXIO;

<<<<<<< HEAD
		if (pdata->gpio_config)
			if (unlikely(pdata->gpio_config(1)))
				dev_err(uport->dev, "Cannot configure"
					"gpios\n");
=======
>>>>>>> refs/remotes/origin/cm-10.0
	}

=======
=======
	uport->mapbase = resource->start;  /* virtual address */
>>>>>>> refs/remotes/origin/cm-11.0

	uport->membase = ioremap(uport->mapbase, PAGE_SIZE);
	if (unlikely(!uport->membase))
		return -ENOMEM;

	uport->irq = platform_get_irq(pdev, 0);
	if (unlikely((int)uport->irq < 0))
		return -ENXIO;

	if (pdata == NULL)
		msm_uport->wakeup.irq = -1;
	else {
		msm_uport->wakeup.irq = pdata->wakeup_irq;
		msm_uport->wakeup.ignore = 1;
		msm_uport->wakeup.inject_rx = pdata->inject_rx_on_wakeup;
		msm_uport->wakeup.rx_to_inject = pdata->rx_to_inject;

		if (unlikely(msm_uport->wakeup.irq < 0))
			return -ENXIO;

	}

<<<<<<< HEAD
	if (pdata == NULL)
		msm_uport->exit_lpm_cb = NULL;
	else
		msm_uport->exit_lpm_cb = pdata->exit_lpm_cb;

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	resource = platform_get_resource_byname(pdev, IORESOURCE_DMA,
						"uartdm_channels");
	if (unlikely(!resource))
		return -ENXIO;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	msm_uport->dma_tx_channel = resource->start;
	msm_uport->dma_rx_channel = resource->end;

	resource = platform_get_resource_byname(pdev, IORESOURCE_DMA,
						"uartdm_crci");
	if (unlikely(!resource))
		return -ENXIO;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	msm_uport->dma_tx_crci = resource->start;
	msm_uport->dma_rx_crci = resource->end;

	uport->iotype = UPIO_MEM;
<<<<<<< HEAD
<<<<<<< HEAD
	uport->fifosize = 64;
	uport->ops = &msm_hs_ops;
	uport->flags = UPF_BOOT_AUTOCONF;
	uport->uartclk = 7372800;
	msm_uport->imr_reg = 0x0;

	msm_uport->clk = clk_get(&pdev->dev, "core_clk");
	if (IS_ERR(msm_uport->clk))
		return PTR_ERR(msm_uport->clk);

	msm_uport->pclk = clk_get(&pdev->dev, "iface_clk");
	/*
	 * Some configurations do not require explicit pclk control so
	 * do not flag error on pclk get failure.
	 */
	if (IS_ERR(msm_uport->pclk))
		msm_uport->pclk = NULL;

	ret = clk_set_rate(msm_uport->clk, uport->uartclk);
	if (ret) {
		printk(KERN_WARNING "Error setting clock rate on UART\n");
		return ret;
	}

<<<<<<< HEAD
=======
	uport->fifosize = UART_FIFOSIZE;
=======
	uport->fifosize = 64;
>>>>>>> refs/remotes/origin/cm-11.0
	uport->ops = &msm_hs_ops;
	uport->flags = UPF_BOOT_AUTOCONF;
	uport->uartclk = 7372800;
	msm_uport->imr_reg = 0x0;

	msm_uport->clk = clk_get(&pdev->dev, "core_clk");
	if (IS_ERR(msm_uport->clk))
		return PTR_ERR(msm_uport->clk);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
	msm_uport->pclk = clk_get(&pdev->dev, "iface_clk");
	/*
	 * Some configurations do not require explicit pclk control so
	 * do not flag error on pclk get failure.
	 */
	if (IS_ERR(msm_uport->pclk))
		msm_uport->pclk = NULL;

	ret = clk_set_rate(msm_uport->clk, uport->uartclk);
	if (ret) {
		printk(KERN_WARNING "Error setting clock rate on UART\n");
		return ret;
	}

	msm_uport->hsuart_wq = alloc_workqueue("k_hsuart",
					WQ_UNBOUND | WQ_MEM_RECLAIM, 1);
	if (!msm_uport->hsuart_wq) {
		pr_err("%s(): Unable to create workqueue hsuart_wq\n",
								__func__);
		return -ENOMEM;
	}

	INIT_WORK(&msm_uport->clock_off_w, hsuart_clock_off_work);
	mutex_init(&msm_uport->clk_mutex);

	clk_prepare_enable(msm_uport->clk);
	if (msm_uport->pclk)
		clk_prepare_enable(msm_uport->pclk);

>>>>>>> refs/remotes/origin/cm-11.0
	ret = uartdm_init_port(uport);
	if (unlikely(ret)) {
		clk_disable_unprepare(msm_uport->clk);
		if (msm_uport->pclk)
			clk_disable_unprepare(msm_uport->pclk);
		return ret;
	}

	/* configure the CR Protection to Enable */
	msm_hs_write(uport, UARTDM_CR_ADDR, CR_PROTECTION_EN);

	clk_disable_unprepare(msm_uport->clk);
	if (msm_uport->pclk)
		clk_disable_unprepare(msm_uport->pclk);

	/*
	 * Enable Command register protection before going ahead as this hw
	 * configuration makes sure that issued cmd to CR register gets complete
	 * before next issued cmd start. Hence mb() requires here.
	 */
	mb();

<<<<<<< HEAD
	/* configure the CR Protection to Enable */
	msm_hs_write(uport, UARTDM_CR_ADDR, CR_PROTECTION_EN);
=======
	msm_uport->hsuart_wq = alloc_workqueue("k_hsuart",
					WQ_UNBOUND | WQ_MEM_RECLAIM, 1);
	if (!msm_uport->hsuart_wq) {
		pr_err("%s(): Unable to create workqueue hsuart_wq\n",
								__func__);
		return -ENOMEM;
	}

	INIT_WORK(&msm_uport->clock_off_w, hsuart_clock_off_work);
	mutex_init(&msm_uport->clk_mutex);

	clk_prepare_enable(msm_uport->clk);
	if (msm_uport->pclk)
		clk_prepare_enable(msm_uport->pclk);

	ret = uartdm_init_port(uport);
	if (unlikely(ret)) {
		clk_disable_unprepare(msm_uport->clk);
		if (msm_uport->pclk)
			clk_disable_unprepare(msm_uport->pclk);
		return ret;
	}

	/* configure the CR Protection to Enable */
	msm_hs_write(uport, UARTDM_CR_ADDR, CR_PROTECTION_EN);

	clk_disable_unprepare(msm_uport->clk);
	if (msm_uport->pclk)
		clk_disable_unprepare(msm_uport->pclk);

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Enable Command register protection before going ahead as this hw
	 * configuration makes sure that issued cmd to CR register gets complete
	 * before next issued cmd start. Hence mb() requires here.
	 */
	mb();

=======
>>>>>>> refs/remotes/origin/master
	msm_uport->clk_state = MSM_HS_CLK_PORT_OFF;
	hrtimer_init(&msm_uport->clk_off_timer, CLOCK_MONOTONIC,
		     HRTIMER_MODE_REL);
	msm_uport->clk_off_timer.function = msm_hs_clk_off_retry;
	msm_uport->clk_off_delay = ktime_set(0, 1000000);  /* 1ms */

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	ret = sysfs_create_file(&pdev->dev.kobj, &dev_attr_clock.attr);
	if (unlikely(ret))
		return ret;

	msm_serial_debugfs_init(msm_uport, pdev->id);

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	uport->line = pdev->id;
	return uart_add_one_port(&msm_hs_driver, uport);
}

static int __init msm_serial_hs_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int ret;
	int i;
=======
	int ret, i;
>>>>>>> refs/remotes/origin/master
=======
	int ret;
	int i;
>>>>>>> refs/remotes/origin/cm-11.0

	/* Init all UARTS as non-configured */
	for (i = 0; i < UARTDM_NR; i++)
		q_uart_port[i].uport.type = PORT_UNKNOWN;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = uart_register_driver(&msm_hs_driver);
	if (unlikely(ret)) {
		printk(KERN_ERR "%s failed to load\n", __FUNCTION__);
		return ret;
	}
	debug_base = debugfs_create_dir("msm_serial_hs", NULL);
	if (IS_ERR_OR_NULL(debug_base))
		pr_info("msm_serial_hs: Cannot create debugfs dir\n");

<<<<<<< HEAD
	ret = platform_driver_probe(&msm_serial_hs_platform_driver,
					msm_hs_probe);
=======
	ret = platform_driver_register(&msm_serial_hs_platform_driver);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret) {
		printk(KERN_ERR "%s failed to load\n", __FUNCTION__);
		debugfs_remove_recursive(debug_base);
		uart_unregister_driver(&msm_hs_driver);
		return ret;
	}

	printk(KERN_INFO "msm_serial_hs module loaded\n");
	return ret;
}
=======
	msm_hs_workqueue = create_singlethread_workqueue("msm_serial_hs");
	if (unlikely(!msm_hs_workqueue))
		return -ENOMEM;

=======
>>>>>>> refs/remotes/origin/cm-11.0
	ret = uart_register_driver(&msm_hs_driver);
	if (unlikely(ret)) {
		printk(KERN_ERR "%s failed to load\n", __FUNCTION__);
		return ret;
	}
	debug_base = debugfs_create_dir("msm_serial_hs", NULL);
	if (IS_ERR_OR_NULL(debug_base))
		pr_info("msm_serial_hs: Cannot create debugfs dir\n");

	ret = platform_driver_register(&msm_serial_hs_platform_driver);
	if (ret) {
		printk(KERN_ERR "%s failed to load\n", __FUNCTION__);
		debugfs_remove_recursive(debug_base);
		uart_unregister_driver(&msm_hs_driver);
		return ret;
	}

	printk(KERN_INFO "msm_serial_hs module loaded\n");
	return ret;
}
<<<<<<< HEAD
module_init(msm_serial_hs_init);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

/*
 *  Called by the upper layer when port is closed.
 *     - Disables the port
 *     - Unhook the ISR
 */
static void msm_hs_shutdown(struct uart_port *uport)
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
	int ret;
	unsigned int data;
>>>>>>> refs/remotes/origin/cm-11.0
	unsigned long flags;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	struct platform_device *pdev = to_platform_device(uport->dev);
	const struct msm_serial_hs_platform_data *pdata =
				pdev->dev.platform_data;

<<<<<<< HEAD
	BUG_ON(msm_uport->rx.flush < FLUSH_STOP);
<<<<<<< HEAD
	tasklet_kill(&msm_uport->tx.tlet);
	wait_event(msm_uport->rx.wait, msm_uport->rx.flush == FLUSH_SHUTDOWN);
	tasklet_kill(&msm_uport->rx.tlet);
	cancel_delayed_work_sync(&msm_uport->rx.flip_insert_work);

	clk_enable(msm_uport->clk);

	pm_runtime_disable(uport->dev);
	pm_runtime_set_suspended(uport->dev);

	spin_lock_irqsave(&uport->lock, flags);
=======
	int ret;
	unsigned int data;
	unsigned long flags;
	struct msm_hs_port *msm_uport = UARTDM_TO_MSM(uport);
	struct platform_device *pdev = to_platform_device(uport->dev);
	const struct msm_serial_hs_platform_data *pdata =
				pdev->dev.platform_data;


	spin_lock_irqsave(&uport->lock, flags);

	/* deactivate if any clock off hrtimer is active. */
	hrtimer_try_to_cancel(&msm_uport->clk_off_timer);

	/* disable UART TX interface to DM */
	data = msm_hs_read(uport, UARTDM_DMEN_ADDR);
	data &= ~UARTDM_TX_DM_EN_BMSK;
	msm_hs_write(uport, UARTDM_DMEN_ADDR, data);
	mb();

	if (msm_uport->tx.dma_in_flight) {
		msm_uport->tx.flush = FLUSH_STOP;
		/* discard flush */
		msm_dmov_flush(msm_uport->dma_tx_channel, 0);
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = wait_event_timeout(msm_uport->tx.wait,
			msm_uport->tx.flush == FLUSH_SHUTDOWN, 100);
		if (!ret) {
			pr_err("%s():HSUART TX Stalls.\n", __func__);
			print_uart_registers(msm_uport);
		}
	} else {
		spin_unlock_irqrestore(&uport->lock, flags);
	}

	tasklet_kill(&msm_uport->tx.tlet);

	if (msm_uport->rx.dma_in_flight) {

		if (msm_uport->rx.flush < FLUSH_STOP) {
			pr_err("%s(): rx.flush is not correct.\n",
							__func__);
			print_uart_registers(msm_uport);
			BUG_ON(1);
		}

		ret = wait_event_timeout(msm_uport->rx.wait,
			msm_uport->rx.flush == FLUSH_SHUTDOWN,
			RX_FLUSH_COMPLETE_TIMEOUT);
		if (!ret) {
			pr_err("%s(): Rx completion failed.\n", __func__);
			print_uart_registers(msm_uport);
		}
	}

	tasklet_kill(&msm_uport->rx.tlet);

	spin_lock_irqsave(&uport->lock, flags);
	/* Disable all UART interrupts */
	msm_uport->imr_reg = 0;
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);

	/* Free the UART IRQ line */
	free_irq(uport->irq, msm_uport);

	msm_uport->is_shutdown = true;
	spin_unlock_irqrestore(&uport->lock, flags);

	/* disable UART RX interface to DM */
	data = msm_hs_read(uport, UARTDM_DMEN_ADDR);
	data &= ~UARTDM_RX_DM_EN_BMSK;
	msm_hs_write(uport, UARTDM_DMEN_ADDR, data);

	cancel_delayed_work_sync(&msm_uport->rx.flip_insert_work);
	flush_workqueue(msm_uport->hsuart_wq);
	pm_runtime_disable(uport->dev);

>>>>>>> refs/remotes/origin/cm-10.0
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0

	spin_lock_irqsave(&uport->lock, flags);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
	/* Disable the transmitter */
	msm_hs_write(uport, UARTDM_CR_ADDR, UARTDM_CR_TX_DISABLE_BMSK);
	/* Disable the receiver */
	msm_hs_write(uport, UARTDM_CR_ADDR, UARTDM_CR_RX_DISABLE_BMSK);

<<<<<<< HEAD
<<<<<<< HEAD
	msm_uport->imr_reg = 0;
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Complete all device write before actually disabling uartclk.
	 * Hence mb() requires here.
	 */
	mb();

<<<<<<< HEAD
	clk_disable(msm_uport->clk);  /* to balance local clk_enable() */
	if (msm_uport->clk_state != MSM_HS_CLK_OFF) {
		clk_disable(msm_uport->clk);  /* to balance clk_state */
		if (msm_uport->pclk)
			clk_disable(msm_uport->pclk);
		wake_unlock(&msm_uport->dma_wake_lock);
	}
=======
	pm_runtime_disable(uport->dev);
	pm_runtime_set_suspended(uport->dev);
=======
	/* deactivate if any clock off hrtimer is active. */
	hrtimer_try_to_cancel(&msm_uport->clk_off_timer);

	/* disable UART TX interface to DM */
	data = msm_hs_read(uport, UARTDM_DMEN_ADDR);
	data &= ~UARTDM_TX_DM_EN_BMSK;
	msm_hs_write(uport, UARTDM_DMEN_ADDR, data);
	mb();

	if (msm_uport->tx.dma_in_flight) {
		msm_uport->tx.flush = FLUSH_STOP;
		/* discard flush */
		msm_dmov_flush(msm_uport->dma_tx_channel, 0);
		spin_unlock_irqrestore(&uport->lock, flags);
		ret = wait_event_timeout(msm_uport->tx.wait,
			msm_uport->tx.flush == FLUSH_SHUTDOWN, 100);
		if (!ret) {
			pr_err("%s():HSUART TX Stalls.\n", __func__);
			print_uart_registers(msm_uport);
		}
	} else {
		spin_unlock_irqrestore(&uport->lock, flags);
	}
>>>>>>> refs/remotes/origin/cm-11.0

	tasklet_kill(&msm_uport->tx.tlet);

	if (msm_uport->rx.dma_in_flight) {

		if (msm_uport->rx.flush < FLUSH_STOP) {
			pr_err("%s(): rx.flush is not correct.\n",
							__func__);
			print_uart_registers(msm_uport);
			BUG_ON(1);
		}

		ret = wait_event_timeout(msm_uport->rx.wait,
			msm_uport->rx.flush == FLUSH_SHUTDOWN,
			RX_FLUSH_COMPLETE_TIMEOUT);
		if (!ret) {
			pr_err("%s(): Rx completion failed.\n", __func__);
			print_uart_registers(msm_uport);
		}
	}

	tasklet_kill(&msm_uport->rx.tlet);

	spin_lock_irqsave(&uport->lock, flags);
	/* Disable all UART interrupts */
	msm_uport->imr_reg = 0;
	msm_hs_write(uport, UARTDM_IMR_ADDR, msm_uport->imr_reg);

	/* Free the UART IRQ line */
	free_irq(uport->irq, msm_uport);

<<<<<<< HEAD
	clk_disable(msm_uport->clk);  /* to balance local clk_enable() */
	if (msm_uport->clk_state != MSM_HS_CLK_OFF)
		clk_disable(msm_uport->clk);  /* to balance clk_state */
>>>>>>> refs/remotes/origin/master
	msm_uport->clk_state = MSM_HS_CLK_PORT_OFF;
=======
	msm_uport->is_shutdown = true;
	spin_unlock_irqrestore(&uport->lock, flags);

	/* disable UART RX interface to DM */
	data = msm_hs_read(uport, UARTDM_DMEN_ADDR);
	data &= ~UARTDM_RX_DM_EN_BMSK;
	msm_hs_write(uport, UARTDM_DMEN_ADDR, data);

	cancel_delayed_work_sync(&msm_uport->rx.flip_insert_work);
	flush_workqueue(msm_uport->hsuart_wq);
	pm_runtime_disable(uport->dev);

	/* Disable the transmitter */
	msm_hs_write(uport, UARTDM_CR_ADDR, UARTDM_CR_TX_DISABLE_BMSK);
	/* Disable the receiver */
	msm_hs_write(uport, UARTDM_CR_ADDR, UARTDM_CR_RX_DISABLE_BMSK);

	/*
	 * Complete all device write before actually disabling uartclk.
	 * Hence mb() requires here.
	 */
	mb();

	if (msm_uport->clk_state != MSM_HS_CLK_OFF) {
		/* to balance clk_state */
		clk_disable_unprepare(msm_uport->clk);
		if (msm_uport->pclk)
			clk_disable_unprepare(msm_uport->pclk);
		wake_unlock(&msm_uport->dma_wake_lock);
	}
>>>>>>> refs/remotes/origin/cm-11.0

	msm_uport->clk_state = MSM_HS_CLK_PORT_OFF;
	dma_unmap_single(uport->dev, msm_uport->tx.dma_base,
			 UART_XMIT_SIZE, DMA_TO_DEVICE);

	if (use_low_power_wakeup(msm_uport))
		irq_set_irq_wake(msm_uport->wakeup.irq, 0);

	/* Free the interrupt */
	if (use_low_power_wakeup(msm_uport))
		free_irq(msm_uport->wakeup.irq, msm_uport);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (msm_uport->clk_state != MSM_HS_CLK_OFF) {
		/* to balance clk_state */
		clk_disable_unprepare(msm_uport->clk);
		if (msm_uport->pclk)
			clk_disable_unprepare(msm_uport->pclk);
		wake_unlock(&msm_uport->dma_wake_lock);
	}

	msm_uport->clk_state = MSM_HS_CLK_PORT_OFF;
	dma_unmap_single(uport->dev, msm_uport->tx.dma_base,
			 UART_XMIT_SIZE, DMA_TO_DEVICE);

>>>>>>> refs/remotes/origin/cm-10.0
	if (use_low_power_wakeup(msm_uport))
		irq_set_irq_wake(msm_uport->wakeup.irq, 0);

	/* Free the interrupt */
<<<<<<< HEAD
	free_irq(uport->irq, msm_uport);
	if (use_low_power_wakeup(msm_uport))
		free_irq(msm_uport->wakeup.irq, msm_uport);
=======
	if (use_low_power_wakeup(msm_uport))
		free_irq(msm_uport->wakeup.irq, msm_uport);

	if (pdata && pdata->gpio_config)
		if (pdata->gpio_config(0))
			dev_err(uport->dev, "GPIO config error\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (cancel_work_sync(&msm_uport->rx.tty_work))
		msm_hs_tty_flip_buffer_work(&msm_uport->rx.tty_work);
>>>>>>> refs/remotes/origin/master
=======
	if (pdata && pdata->gpio_config)
		if (pdata->gpio_config(0))
			dev_err(uport->dev, "GPIO config error\n");
>>>>>>> refs/remotes/origin/cm-11.0
}

static void __exit msm_serial_hs_exit(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "msm_serial_hs module removed\n");
<<<<<<< HEAD
=======
	debugfs_remove_recursive(debug_base);
>>>>>>> refs/remotes/origin/cm-10.0
	platform_driver_unregister(&msm_serial_hs_platform_driver);
	uart_unregister_driver(&msm_hs_driver);
}

=======
	flush_workqueue(msm_hs_workqueue);
	destroy_workqueue(msm_hs_workqueue);
=======
	printk(KERN_INFO "msm_serial_hs module removed\n");
	debugfs_remove_recursive(debug_base);
>>>>>>> refs/remotes/origin/cm-11.0
	platform_driver_unregister(&msm_serial_hs_platform_driver);
	uart_unregister_driver(&msm_hs_driver);
}

<<<<<<< HEAD
#ifdef CONFIG_PM_RUNTIME
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int msm_hs_runtime_idle(struct device *dev)
{
	/*
	 * returning success from idle results in runtime suspend to be
	 * called
	 */
	return 0;
}

static int msm_hs_runtime_resume(struct device *dev)
{
	struct platform_device *pdev = container_of(dev, struct
						    platform_device, dev);
	struct msm_hs_port *msm_uport = &q_uart_port[pdev->id];
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	msm_hs_request_clock_on(&msm_uport->uport);
	return 0;
}

static int msm_hs_runtime_suspend(struct device *dev)
{
	struct platform_device *pdev = container_of(dev, struct
						    platform_device, dev);
	struct msm_hs_port *msm_uport = &q_uart_port[pdev->id];
<<<<<<< HEAD
<<<<<<< HEAD
	msm_hs_request_clock_off(&msm_uport->uport);
	return 0;
}
=======

	msm_hs_request_clock_off(&msm_uport->uport);
	return 0;
}
#else
#define msm_hs_runtime_idle NULL
#define msm_hs_runtime_resume NULL
#define msm_hs_runtime_suspend NULL
#endif
>>>>>>> refs/remotes/origin/master
=======
	msm_hs_request_clock_off(&msm_uport->uport);
	return 0;
}
>>>>>>> refs/remotes/origin/cm-11.0

static const struct dev_pm_ops msm_hs_dev_pm_ops = {
	.runtime_suspend = msm_hs_runtime_suspend,
	.runtime_resume  = msm_hs_runtime_resume,
	.runtime_idle    = msm_hs_runtime_idle,
};

static struct platform_driver msm_serial_hs_platform_driver = {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	.remove = msm_hs_remove,
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	.probe	= msm_hs_probe,
	.remove = __devexit_p(msm_hs_remove),
>>>>>>> refs/remotes/origin/cm-10.0
	.driver = {
		.name = "msm_serial_hs",
=======
	.probe = msm_hs_probe,
	.remove = msm_hs_remove,
	.driver = {
		.name = "msm_serial_hs",
<<<<<<< HEAD
		.owner = THIS_MODULE,
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		.pm   = &msm_hs_dev_pm_ops,
	},
};

static struct uart_driver msm_hs_driver = {
	.owner = THIS_MODULE,
	.driver_name = "msm_serial_hs",
	.dev_name = "ttyHS",
	.nr = UARTDM_NR,
	.cons = 0,
};

static struct uart_ops msm_hs_ops = {
	.tx_empty = msm_hs_tx_empty,
	.set_mctrl = msm_hs_set_mctrl_locked,
	.get_mctrl = msm_hs_get_mctrl_locked,
	.stop_tx = msm_hs_stop_tx_locked,
	.start_tx = msm_hs_start_tx_locked,
	.stop_rx = msm_hs_stop_rx_locked,
	.enable_ms = msm_hs_enable_ms_locked,
	.break_ctl = msm_hs_break_ctl,
	.startup = msm_hs_startup,
	.shutdown = msm_hs_shutdown,
	.set_termios = msm_hs_set_termios,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.pm = msm_hs_pm,
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	.type = msm_hs_type,
	.config_port = msm_hs_config_port,
	.release_port = msm_hs_release_port,
	.request_port = msm_hs_request_port,
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.flush_buffer = msm_hs_flush_buffer_locked,
>>>>>>> refs/remotes/origin/cm-10.0
};

module_init(msm_serial_hs_init);
module_exit(msm_serial_hs_exit);
=======
};

>>>>>>> refs/remotes/origin/master
=======
	.flush_buffer = msm_hs_flush_buffer_locked,
};

module_init(msm_serial_hs_init);
module_exit(msm_serial_hs_exit);
>>>>>>> refs/remotes/origin/cm-11.0
MODULE_DESCRIPTION("High Speed UART Driver for the MSM chipset");
MODULE_VERSION("1.2");
MODULE_LICENSE("GPL v2");
