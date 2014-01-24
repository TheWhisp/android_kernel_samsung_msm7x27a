<<<<<<< HEAD
/*
 * drivers/net/ethernet/freescale/gianfar.c
=======
/* drivers/net/ethernet/freescale/gianfar.c
>>>>>>> refs/remotes/origin/master
 *
 * Gianfar Ethernet Driver
 * This driver is designed for the non-CPM ethernet controllers
 * on the 85xx and 83xx family of integrated processors
 * Based on 8260_io/fcc_enet.c
 *
 * Author: Andy Fleming
 * Maintainer: Kumar Gala
 * Modifier: Sandeep Gopalpet <sandeep.kumar@freescale.com>
 *
 * Copyright 2002-2009, 2011 Freescale Semiconductor, Inc.
 * Copyright 2007 MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 *  Gianfar:  AKA Lambda Draconis, "Dragon"
 *  RA 11 31 24.2
 *  Dec +69 19 52
 *  V 3.84
 *  B-V +1.62
 *
 *  Theory of operation
 *
 *  The driver is initialized through of_device. Configuration information
 *  is therefore conveyed through an OF-style device tree.
 *
 *  The Gianfar Ethernet Controller uses a ring of buffer
 *  descriptors.  The beginning is indicated by a register
 *  pointing to the physical address of the start of the ring.
 *  The end is determined by a "wrap" bit being set in the
 *  last descriptor of the ring.
 *
 *  When a packet is received, the RXF bit in the
 *  IEVENT register is set, triggering an interrupt when the
 *  corresponding bit in the IMASK register is also set (if
 *  interrupt coalescing is active, then the interrupt may not
 *  happen immediately, but will wait until either a set number
 *  of frames or amount of time have passed).  In NAPI, the
 *  interrupt handler will signal there is work to be done, and
 *  exit. This method will start at the last known empty
 *  descriptor, and process every subsequent descriptor until there
 *  are none left with data (NAPI will stop after a set number of
 *  packets to give time to other tasks, but will eventually
 *  process all the packets).  The data arrives inside a
 *  pre-allocated skb, and so after the skb is passed up to the
 *  stack, a new skb must be allocated, and the address field in
 *  the buffer descriptor must be updated to indicate this new
 *  skb.
 *
 *  When the kernel requests that a packet be transmitted, the
 *  driver starts where it left off last time, and points the
 *  descriptor at the buffer which was passed in.  The driver
 *  then informs the DMA engine that there are packets ready to
 *  be transmitted.  Once the controller is finished transmitting
 *  the packet, an interrupt may be triggered (under the same
 *  conditions as for reception, but depending on the TXF bit).
 *  The driver then cleans up the buffer.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#define DEBUG

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/if_vlan.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
<<<<<<< HEAD
=======
#include <linux/of_address.h>
#include <linux/of_irq.h>
>>>>>>> refs/remotes/origin/master
#include <linux/of_mdio.h>
#include <linux/of_platform.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/in.h>
#include <linux/net_tstamp.h>

#include <asm/io.h>
#include <asm/reg.h>
<<<<<<< HEAD
=======
#include <asm/mpc85xx.h>
>>>>>>> refs/remotes/origin/master
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/dma-mapping.h>
#include <linux/crc32.h>
#include <linux/mii.h>
#include <linux/phy.h>
#include <linux/phy_fixed.h>
#include <linux/of.h>
#include <linux/of_net.h>

#include "gianfar.h"
<<<<<<< HEAD
#include "fsl_pq_mdio.h"
=======
>>>>>>> refs/remotes/origin/master

#define TX_TIMEOUT      (1*HZ)

const char gfar_driver_version[] = "1.3";

static int gfar_enet_open(struct net_device *dev);
static int gfar_start_xmit(struct sk_buff *skb, struct net_device *dev);
static void gfar_reset_task(struct work_struct *work);
static void gfar_timeout(struct net_device *dev);
static int gfar_close(struct net_device *dev);
struct sk_buff *gfar_new_skb(struct net_device *dev);
static void gfar_new_rxbdp(struct gfar_priv_rx_q *rx_queue, struct rxbd8 *bdp,
<<<<<<< HEAD
		struct sk_buff *skb);
=======
			   struct sk_buff *skb);
>>>>>>> refs/remotes/origin/master
static int gfar_set_mac_address(struct net_device *dev);
static int gfar_change_mtu(struct net_device *dev, int new_mtu);
static irqreturn_t gfar_error(int irq, void *dev_id);
static irqreturn_t gfar_transmit(int irq, void *dev_id);
static irqreturn_t gfar_interrupt(int irq, void *dev_id);
static void adjust_link(struct net_device *dev);
static void init_registers(struct net_device *dev);
static int init_phy(struct net_device *dev);
static int gfar_probe(struct platform_device *ofdev);
static int gfar_remove(struct platform_device *ofdev);
static void free_skb_resources(struct gfar_private *priv);
static void gfar_set_multi(struct net_device *dev);
static void gfar_set_hash_for_addr(struct net_device *dev, u8 *addr);
static void gfar_configure_serdes(struct net_device *dev);
static int gfar_poll(struct napi_struct *napi, int budget);
<<<<<<< HEAD
=======
static int gfar_poll_sq(struct napi_struct *napi, int budget);
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_NET_POLL_CONTROLLER
static void gfar_netpoll(struct net_device *dev);
#endif
int gfar_clean_rx_ring(struct gfar_priv_rx_q *rx_queue, int rx_work_limit);
<<<<<<< HEAD
static int gfar_clean_tx_ring(struct gfar_priv_tx_q *tx_queue);
static int gfar_process_frame(struct net_device *dev, struct sk_buff *skb,
			      int amount_pull);
=======
static void gfar_clean_tx_ring(struct gfar_priv_tx_q *tx_queue);
static void gfar_process_frame(struct net_device *dev, struct sk_buff *skb,
			       int amount_pull, struct napi_struct *napi);
>>>>>>> refs/remotes/origin/master
void gfar_halt(struct net_device *dev);
static void gfar_halt_nodisable(struct net_device *dev);
void gfar_start(struct net_device *dev);
static void gfar_clear_exact_match(struct net_device *dev);
static void gfar_set_mac_for_addr(struct net_device *dev, int num,
				  const u8 *addr);
static int gfar_ioctl(struct net_device *dev, struct ifreq *rq, int cmd);

MODULE_AUTHOR("Freescale Semiconductor, Inc");
MODULE_DESCRIPTION("Gianfar Ethernet Driver");
MODULE_LICENSE("GPL");

static void gfar_init_rxbdp(struct gfar_priv_rx_q *rx_queue, struct rxbd8 *bdp,
			    dma_addr_t buf)
{
	u32 lstatus;

	bdp->bufPtr = buf;

	lstatus = BD_LFLAG(RXBD_EMPTY | RXBD_INTERRUPT);
	if (bdp == rx_queue->rx_bd_base + rx_queue->rx_ring_size - 1)
		lstatus |= BD_LFLAG(RXBD_WRAP);

	eieio();

	bdp->lstatus = lstatus;
}

static int gfar_init_bds(struct net_device *ndev)
{
	struct gfar_private *priv = netdev_priv(ndev);
	struct gfar_priv_tx_q *tx_queue = NULL;
	struct gfar_priv_rx_q *rx_queue = NULL;
	struct txbd8 *txbdp;
	struct rxbd8 *rxbdp;
	int i, j;

	for (i = 0; i < priv->num_tx_queues; i++) {
		tx_queue = priv->tx_queue[i];
		/* Initialize some variables in our dev structure */
		tx_queue->num_txbdfree = tx_queue->tx_ring_size;
		tx_queue->dirty_tx = tx_queue->tx_bd_base;
		tx_queue->cur_tx = tx_queue->tx_bd_base;
		tx_queue->skb_curtx = 0;
		tx_queue->skb_dirtytx = 0;

		/* Initialize Transmit Descriptor Ring */
		txbdp = tx_queue->tx_bd_base;
		for (j = 0; j < tx_queue->tx_ring_size; j++) {
			txbdp->lstatus = 0;
			txbdp->bufPtr = 0;
			txbdp++;
		}

		/* Set the last descriptor in the ring to indicate wrap */
		txbdp--;
		txbdp->status |= TXBD_WRAP;
	}

	for (i = 0; i < priv->num_rx_queues; i++) {
		rx_queue = priv->rx_queue[i];
		rx_queue->cur_rx = rx_queue->rx_bd_base;
		rx_queue->skb_currx = 0;
		rxbdp = rx_queue->rx_bd_base;

		for (j = 0; j < rx_queue->rx_ring_size; j++) {
			struct sk_buff *skb = rx_queue->rx_skbuff[j];

			if (skb) {
				gfar_init_rxbdp(rx_queue, rxbdp,
						rxbdp->bufPtr);
			} else {
				skb = gfar_new_skb(ndev);
				if (!skb) {
					netdev_err(ndev, "Can't allocate RX buffers\n");
<<<<<<< HEAD
					goto err_rxalloc_fail;
=======
					return -ENOMEM;
>>>>>>> refs/remotes/origin/master
				}
				rx_queue->rx_skbuff[j] = skb;

				gfar_new_rxbdp(rx_queue, rxbdp, skb);
			}

			rxbdp++;
		}

	}

	return 0;
<<<<<<< HEAD

err_rxalloc_fail:
	free_skb_resources(priv);
	return -ENOMEM;
=======
>>>>>>> refs/remotes/origin/master
}

static int gfar_alloc_skb_resources(struct net_device *ndev)
{
	void *vaddr;
	dma_addr_t addr;
	int i, j, k;
	struct gfar_private *priv = netdev_priv(ndev);
<<<<<<< HEAD
	struct device *dev = &priv->ofdev->dev;
=======
	struct device *dev = priv->dev;
>>>>>>> refs/remotes/origin/master
	struct gfar_priv_tx_q *tx_queue = NULL;
	struct gfar_priv_rx_q *rx_queue = NULL;

	priv->total_tx_ring_size = 0;
	for (i = 0; i < priv->num_tx_queues; i++)
		priv->total_tx_ring_size += priv->tx_queue[i]->tx_ring_size;

	priv->total_rx_ring_size = 0;
	for (i = 0; i < priv->num_rx_queues; i++)
		priv->total_rx_ring_size += priv->rx_queue[i]->rx_ring_size;

	/* Allocate memory for the buffer descriptors */
	vaddr = dma_alloc_coherent(dev,
<<<<<<< HEAD
			sizeof(struct txbd8) * priv->total_tx_ring_size +
			sizeof(struct rxbd8) * priv->total_rx_ring_size,
			&addr, GFP_KERNEL);
	if (!vaddr) {
		netif_err(priv, ifup, ndev,
			  "Could not allocate buffer descriptors!\n");
		return -ENOMEM;
	}
=======
				   (priv->total_tx_ring_size *
				    sizeof(struct txbd8)) +
				   (priv->total_rx_ring_size *
				    sizeof(struct rxbd8)),
				   &addr, GFP_KERNEL);
	if (!vaddr)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < priv->num_tx_queues; i++) {
		tx_queue = priv->tx_queue[i];
		tx_queue->tx_bd_base = vaddr;
		tx_queue->tx_bd_dma_base = addr;
		tx_queue->dev = ndev;
		/* enet DMA only understands physical addresses */
<<<<<<< HEAD
		addr    += sizeof(struct txbd8) *tx_queue->tx_ring_size;
		vaddr   += sizeof(struct txbd8) *tx_queue->tx_ring_size;
=======
		addr  += sizeof(struct txbd8) * tx_queue->tx_ring_size;
		vaddr += sizeof(struct txbd8) * tx_queue->tx_ring_size;
>>>>>>> refs/remotes/origin/master
	}

	/* Start the rx descriptor ring where the tx ring leaves off */
	for (i = 0; i < priv->num_rx_queues; i++) {
		rx_queue = priv->rx_queue[i];
		rx_queue->rx_bd_base = vaddr;
		rx_queue->rx_bd_dma_base = addr;
		rx_queue->dev = ndev;
<<<<<<< HEAD
		addr    += sizeof (struct rxbd8) * rx_queue->rx_ring_size;
		vaddr   += sizeof (struct rxbd8) * rx_queue->rx_ring_size;
=======
		addr  += sizeof(struct rxbd8) * rx_queue->rx_ring_size;
		vaddr += sizeof(struct rxbd8) * rx_queue->rx_ring_size;
>>>>>>> refs/remotes/origin/master
	}

	/* Setup the skbuff rings */
	for (i = 0; i < priv->num_tx_queues; i++) {
		tx_queue = priv->tx_queue[i];
<<<<<<< HEAD
		tx_queue->tx_skbuff = kmalloc(sizeof(*tx_queue->tx_skbuff) *
				  tx_queue->tx_ring_size, GFP_KERNEL);
		if (!tx_queue->tx_skbuff) {
			netif_err(priv, ifup, ndev,
				  "Could not allocate tx_skbuff\n");
			goto cleanup;
		}
=======
		tx_queue->tx_skbuff =
			kmalloc_array(tx_queue->tx_ring_size,
				      sizeof(*tx_queue->tx_skbuff),
				      GFP_KERNEL);
		if (!tx_queue->tx_skbuff)
			goto cleanup;
>>>>>>> refs/remotes/origin/master

		for (k = 0; k < tx_queue->tx_ring_size; k++)
			tx_queue->tx_skbuff[k] = NULL;
	}

	for (i = 0; i < priv->num_rx_queues; i++) {
		rx_queue = priv->rx_queue[i];
<<<<<<< HEAD
		rx_queue->rx_skbuff = kmalloc(sizeof(*rx_queue->rx_skbuff) *
				  rx_queue->rx_ring_size, GFP_KERNEL);

		if (!rx_queue->rx_skbuff) {
			netif_err(priv, ifup, ndev,
				  "Could not allocate rx_skbuff\n");
			goto cleanup;
		}
=======
		rx_queue->rx_skbuff =
			kmalloc_array(rx_queue->rx_ring_size,
				      sizeof(*rx_queue->rx_skbuff),
				      GFP_KERNEL);
		if (!rx_queue->rx_skbuff)
			goto cleanup;
>>>>>>> refs/remotes/origin/master

		for (j = 0; j < rx_queue->rx_ring_size; j++)
			rx_queue->rx_skbuff[j] = NULL;
	}

	if (gfar_init_bds(ndev))
		goto cleanup;

	return 0;

cleanup:
	free_skb_resources(priv);
	return -ENOMEM;
}

static void gfar_init_tx_rx_base(struct gfar_private *priv)
{
	struct gfar __iomem *regs = priv->gfargrp[0].regs;
	u32 __iomem *baddr;
	int i;

	baddr = &regs->tbase0;
<<<<<<< HEAD
	for(i = 0; i < priv->num_tx_queues; i++) {
		gfar_write(baddr, priv->tx_queue[i]->tx_bd_dma_base);
		baddr	+= 2;
	}

	baddr = &regs->rbase0;
	for(i = 0; i < priv->num_rx_queues; i++) {
		gfar_write(baddr, priv->rx_queue[i]->rx_bd_dma_base);
		baddr   += 2;
=======
	for (i = 0; i < priv->num_tx_queues; i++) {
		gfar_write(baddr, priv->tx_queue[i]->tx_bd_dma_base);
		baddr += 2;
	}

	baddr = &regs->rbase0;
	for (i = 0; i < priv->num_rx_queues; i++) {
		gfar_write(baddr, priv->rx_queue[i]->rx_bd_dma_base);
		baddr += 2;
>>>>>>> refs/remotes/origin/master
	}
}

static void gfar_init_mac(struct net_device *ndev)
{
	struct gfar_private *priv = netdev_priv(ndev);
	struct gfar __iomem *regs = priv->gfargrp[0].regs;
	u32 rctrl = 0;
	u32 tctrl = 0;
	u32 attrs = 0;

	/* write the tx/rx base registers */
	gfar_init_tx_rx_base(priv);

	/* Configure the coalescing support */
<<<<<<< HEAD
	gfar_configure_coalescing(priv, 0xFF, 0xFF);
=======
	gfar_configure_coalescing_all(priv);

	/* set this when rx hw offload (TOE) functions are being used */
	priv->uses_rxfcb = 0;
>>>>>>> refs/remotes/origin/master

	if (priv->rx_filer_enable) {
		rctrl |= RCTRL_FILREN;
		/* Program the RIR0 reg with the required distribution */
		gfar_write(&regs->rir0, DEFAULT_RIR0);
	}

<<<<<<< HEAD
	if (ndev->features & NETIF_F_RXCSUM)
		rctrl |= RCTRL_CHECKSUMMING;
=======
	/* Restore PROMISC mode */
	if (ndev->flags & IFF_PROMISC)
		rctrl |= RCTRL_PROM;

	if (ndev->features & NETIF_F_RXCSUM) {
		rctrl |= RCTRL_CHECKSUMMING;
		priv->uses_rxfcb = 1;
	}
>>>>>>> refs/remotes/origin/master

	if (priv->extended_hash) {
		rctrl |= RCTRL_EXTHASH;

		gfar_clear_exact_match(ndev);
		rctrl |= RCTRL_EMEN;
	}

	if (priv->padding) {
		rctrl &= ~RCTRL_PAL_MASK;
		rctrl |= RCTRL_PADDING(priv->padding);
	}

	/* Insert receive time stamps into padding alignment bytes */
	if (priv->device_flags & FSL_GIANFAR_DEV_HAS_TIMER) {
		rctrl &= ~RCTRL_PAL_MASK;
		rctrl |= RCTRL_PADDING(8);
		priv->padding = 8;
	}

	/* Enable HW time stamping if requested from user space */
<<<<<<< HEAD
	if (priv->hwts_rx_en)
		rctrl |= RCTRL_PRSDEP_INIT | RCTRL_TS_ENABLE;

	if (ndev->features & NETIF_F_HW_VLAN_RX)
		rctrl |= RCTRL_VLEX | RCTRL_PRSDEP_INIT;
=======
	if (priv->hwts_rx_en) {
		rctrl |= RCTRL_PRSDEP_INIT | RCTRL_TS_ENABLE;
		priv->uses_rxfcb = 1;
	}

	if (ndev->features & NETIF_F_HW_VLAN_CTAG_RX) {
		rctrl |= RCTRL_VLEX | RCTRL_PRSDEP_INIT;
		priv->uses_rxfcb = 1;
	}
>>>>>>> refs/remotes/origin/master

	/* Init rctrl based on our settings */
	gfar_write(&regs->rctrl, rctrl);

	if (ndev->features & NETIF_F_IP_CSUM)
		tctrl |= TCTRL_INIT_CSUM;

	if (priv->prio_sched_en)
		tctrl |= TCTRL_TXSCHED_PRIO;
	else {
		tctrl |= TCTRL_TXSCHED_WRRS;
		gfar_write(&regs->tr03wt, DEFAULT_WRRS_WEIGHT);
		gfar_write(&regs->tr47wt, DEFAULT_WRRS_WEIGHT);
	}

	gfar_write(&regs->tctrl, tctrl);

	/* Set the extraction length and index */
	attrs = ATTRELI_EL(priv->rx_stash_size) |
		ATTRELI_EI(priv->rx_stash_index);

	gfar_write(&regs->attreli, attrs);

	/* Start with defaults, and add stashing or locking
<<<<<<< HEAD
	 * depending on the approprate variables */
=======
	 * depending on the approprate variables
	 */
>>>>>>> refs/remotes/origin/master
	attrs = ATTR_INIT_SETTINGS;

	if (priv->bd_stash_en)
		attrs |= ATTR_BDSTASH;

	if (priv->rx_stash_size != 0)
		attrs |= ATTR_BUFSTASH;

	gfar_write(&regs->attr, attrs);

	gfar_write(&regs->fifo_tx_thr, priv->fifo_threshold);
	gfar_write(&regs->fifo_tx_starve, priv->fifo_starve);
	gfar_write(&regs->fifo_tx_starve_shutoff, priv->fifo_starve_off);
}

static struct net_device_stats *gfar_get_stats(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	unsigned long rx_packets = 0, rx_bytes = 0, rx_dropped = 0;
	unsigned long tx_packets = 0, tx_bytes = 0;
<<<<<<< HEAD
	int i = 0;

	for (i = 0; i < priv->num_rx_queues; i++) {
		rx_packets += priv->rx_queue[i]->stats.rx_packets;
		rx_bytes += priv->rx_queue[i]->stats.rx_bytes;
=======
	int i;

	for (i = 0; i < priv->num_rx_queues; i++) {
		rx_packets += priv->rx_queue[i]->stats.rx_packets;
		rx_bytes   += priv->rx_queue[i]->stats.rx_bytes;
>>>>>>> refs/remotes/origin/master
		rx_dropped += priv->rx_queue[i]->stats.rx_dropped;
	}

	dev->stats.rx_packets = rx_packets;
<<<<<<< HEAD
	dev->stats.rx_bytes = rx_bytes;
=======
	dev->stats.rx_bytes   = rx_bytes;
>>>>>>> refs/remotes/origin/master
	dev->stats.rx_dropped = rx_dropped;

	for (i = 0; i < priv->num_tx_queues; i++) {
		tx_bytes += priv->tx_queue[i]->stats.tx_bytes;
		tx_packets += priv->tx_queue[i]->stats.tx_packets;
	}

<<<<<<< HEAD
	dev->stats.tx_bytes = tx_bytes;
=======
	dev->stats.tx_bytes   = tx_bytes;
>>>>>>> refs/remotes/origin/master
	dev->stats.tx_packets = tx_packets;

	return &dev->stats;
}

static const struct net_device_ops gfar_netdev_ops = {
	.ndo_open = gfar_enet_open,
	.ndo_start_xmit = gfar_start_xmit,
	.ndo_stop = gfar_close,
	.ndo_change_mtu = gfar_change_mtu,
	.ndo_set_features = gfar_set_features,
	.ndo_set_rx_mode = gfar_set_multi,
	.ndo_tx_timeout = gfar_timeout,
	.ndo_do_ioctl = gfar_ioctl,
	.ndo_get_stats = gfar_get_stats,
	.ndo_set_mac_address = eth_mac_addr,
	.ndo_validate_addr = eth_validate_addr,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller = gfar_netpoll,
#endif
};

void lock_rx_qs(struct gfar_private *priv)
{
<<<<<<< HEAD
	int i = 0x0;
=======
	int i;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < priv->num_rx_queues; i++)
		spin_lock(&priv->rx_queue[i]->rxlock);
}

void lock_tx_qs(struct gfar_private *priv)
{
<<<<<<< HEAD
	int i = 0x0;
=======
	int i;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < priv->num_tx_queues; i++)
		spin_lock(&priv->tx_queue[i]->txlock);
}

void unlock_rx_qs(struct gfar_private *priv)
{
<<<<<<< HEAD
	int i = 0x0;
=======
	int i;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < priv->num_rx_queues; i++)
		spin_unlock(&priv->rx_queue[i]->rxlock);
}

void unlock_tx_qs(struct gfar_private *priv)
{
<<<<<<< HEAD
	int i = 0x0;
=======
	int i;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < priv->num_tx_queues; i++)
		spin_unlock(&priv->tx_queue[i]->txlock);
}

<<<<<<< HEAD
static bool gfar_is_vlan_on(struct gfar_private *priv)
{
	return (priv->ndev->features & NETIF_F_HW_VLAN_RX) ||
	       (priv->ndev->features & NETIF_F_HW_VLAN_TX);
}

/* Returns 1 if incoming frames use an FCB */
static inline int gfar_uses_fcb(struct gfar_private *priv)
{
	return gfar_is_vlan_on(priv) ||
		(priv->ndev->features & NETIF_F_RXCSUM) ||
		(priv->device_flags & FSL_GIANFAR_DEV_HAS_TIMER);
}

static void free_tx_pointers(struct gfar_private *priv)
{
	int i = 0;
=======
static void free_tx_pointers(struct gfar_private *priv)
{
	int i;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < priv->num_tx_queues; i++)
		kfree(priv->tx_queue[i]);
}

static void free_rx_pointers(struct gfar_private *priv)
{
<<<<<<< HEAD
	int i = 0;
=======
	int i;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < priv->num_rx_queues; i++)
		kfree(priv->rx_queue[i]);
}

static void unmap_group_regs(struct gfar_private *priv)
{
<<<<<<< HEAD
	int i = 0;
=======
	int i;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < MAXGROUPS; i++)
		if (priv->gfargrp[i].regs)
			iounmap(priv->gfargrp[i].regs);
}

<<<<<<< HEAD
static void disable_napi(struct gfar_private *priv)
{
	int i = 0;
=======
static void free_gfar_dev(struct gfar_private *priv)
{
	int i, j;

	for (i = 0; i < priv->num_grps; i++)
		for (j = 0; j < GFAR_NUM_IRQS; j++) {
			kfree(priv->gfargrp[i].irqinfo[j]);
			priv->gfargrp[i].irqinfo[j] = NULL;
		}

	free_netdev(priv->ndev);
}

static void disable_napi(struct gfar_private *priv)
{
	int i;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < priv->num_grps; i++)
		napi_disable(&priv->gfargrp[i].napi);
}

static void enable_napi(struct gfar_private *priv)
{
<<<<<<< HEAD
	int i = 0;
=======
	int i;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < priv->num_grps; i++)
		napi_enable(&priv->gfargrp[i].napi);
}

static int gfar_parse_group(struct device_node *np,
<<<<<<< HEAD
		struct gfar_private *priv, const char *model)
{
	u32 *queue_mask;

	priv->gfargrp[priv->num_grps].regs = of_iomap(np, 0);
	if (!priv->gfargrp[priv->num_grps].regs)
		return -ENOMEM;

	priv->gfargrp[priv->num_grps].interruptTransmit =
			irq_of_parse_and_map(np, 0);

	/* If we aren't the FEC we have multiple interrupts */
	if (model && strcasecmp(model, "FEC")) {
		priv->gfargrp[priv->num_grps].interruptReceive =
			irq_of_parse_and_map(np, 1);
		priv->gfargrp[priv->num_grps].interruptError =
			irq_of_parse_and_map(np,2);
		if (priv->gfargrp[priv->num_grps].interruptTransmit == NO_IRQ ||
		    priv->gfargrp[priv->num_grps].interruptReceive  == NO_IRQ ||
		    priv->gfargrp[priv->num_grps].interruptError    == NO_IRQ)
			return -EINVAL;
	}

	priv->gfargrp[priv->num_grps].grp_id = priv->num_grps;
	priv->gfargrp[priv->num_grps].priv = priv;
	spin_lock_init(&priv->gfargrp[priv->num_grps].grplock);
	if(priv->mode == MQ_MG_MODE) {
		queue_mask = (u32 *)of_get_property(np,
					"fsl,rx-bit-map", NULL);
		priv->gfargrp[priv->num_grps].rx_bit_map =
			queue_mask ?  *queue_mask :(DEFAULT_MAPPING >> priv->num_grps);
		queue_mask = (u32 *)of_get_property(np,
					"fsl,tx-bit-map", NULL);
		priv->gfargrp[priv->num_grps].tx_bit_map =
			queue_mask ? *queue_mask : (DEFAULT_MAPPING >> priv->num_grps);
	} else {
		priv->gfargrp[priv->num_grps].rx_bit_map = 0xFF;
		priv->gfargrp[priv->num_grps].tx_bit_map = 0xFF;
=======
			    struct gfar_private *priv, const char *model)
{
	struct gfar_priv_grp *grp = &priv->gfargrp[priv->num_grps];
	u32 *queue_mask;
	int i;

	for (i = 0; i < GFAR_NUM_IRQS; i++) {
		grp->irqinfo[i] = kzalloc(sizeof(struct gfar_irqinfo),
					  GFP_KERNEL);
		if (!grp->irqinfo[i])
			return -ENOMEM;
	}

	grp->regs = of_iomap(np, 0);
	if (!grp->regs)
		return -ENOMEM;

	gfar_irq(grp, TX)->irq = irq_of_parse_and_map(np, 0);

	/* If we aren't the FEC we have multiple interrupts */
	if (model && strcasecmp(model, "FEC")) {
		gfar_irq(grp, RX)->irq = irq_of_parse_and_map(np, 1);
		gfar_irq(grp, ER)->irq = irq_of_parse_and_map(np, 2);
		if (gfar_irq(grp, TX)->irq == NO_IRQ ||
		    gfar_irq(grp, RX)->irq == NO_IRQ ||
		    gfar_irq(grp, ER)->irq == NO_IRQ)
			return -EINVAL;
	}

	grp->priv = priv;
	spin_lock_init(&grp->grplock);
	if (priv->mode == MQ_MG_MODE) {
		queue_mask = (u32 *)of_get_property(np, "fsl,rx-bit-map", NULL);
		grp->rx_bit_map = queue_mask ?
			*queue_mask : (DEFAULT_MAPPING >> priv->num_grps);
		queue_mask = (u32 *)of_get_property(np, "fsl,tx-bit-map", NULL);
		grp->tx_bit_map = queue_mask ?
			*queue_mask : (DEFAULT_MAPPING >> priv->num_grps);
	} else {
		grp->rx_bit_map = 0xFF;
		grp->tx_bit_map = 0xFF;
>>>>>>> refs/remotes/origin/master
	}
	priv->num_grps++;

	return 0;
}

static int gfar_of_init(struct platform_device *ofdev, struct net_device **pdev)
{
	const char *model;
	const char *ctype;
	const void *mac_addr;
	int err = 0, i;
	struct net_device *dev = NULL;
	struct gfar_private *priv = NULL;
	struct device_node *np = ofdev->dev.of_node;
	struct device_node *child = NULL;
	const u32 *stash;
	const u32 *stash_len;
	const u32 *stash_idx;
	unsigned int num_tx_qs, num_rx_qs;
	u32 *tx_queues, *rx_queues;

	if (!np || !of_device_is_available(np))
		return -ENODEV;

	/* parse the num of tx and rx queues */
	tx_queues = (u32 *)of_get_property(np, "fsl,num_tx_queues", NULL);
	num_tx_qs = tx_queues ? *tx_queues : 1;

	if (num_tx_qs > MAX_TX_QS) {
		pr_err("num_tx_qs(=%d) greater than MAX_TX_QS(=%d)\n",
		       num_tx_qs, MAX_TX_QS);
		pr_err("Cannot do alloc_etherdev, aborting\n");
		return -EINVAL;
	}

	rx_queues = (u32 *)of_get_property(np, "fsl,num_rx_queues", NULL);
	num_rx_qs = rx_queues ? *rx_queues : 1;

	if (num_rx_qs > MAX_RX_QS) {
		pr_err("num_rx_qs(=%d) greater than MAX_RX_QS(=%d)\n",
		       num_rx_qs, MAX_RX_QS);
		pr_err("Cannot do alloc_etherdev, aborting\n");
		return -EINVAL;
	}

	*pdev = alloc_etherdev_mq(sizeof(*priv), num_tx_qs);
	dev = *pdev;
	if (NULL == dev)
		return -ENOMEM;

	priv = netdev_priv(dev);
<<<<<<< HEAD
	priv->node = ofdev->dev.of_node;
=======
>>>>>>> refs/remotes/origin/master
	priv->ndev = dev;

	priv->num_tx_queues = num_tx_qs;
	netif_set_real_num_rx_queues(dev, num_rx_qs);
	priv->num_rx_queues = num_rx_qs;
	priv->num_grps = 0x0;

<<<<<<< HEAD
	/* Init Rx queue filer rule set linked list*/
=======
	/* Init Rx queue filer rule set linked list */
>>>>>>> refs/remotes/origin/master
	INIT_LIST_HEAD(&priv->rx_list.list);
	priv->rx_list.count = 0;
	mutex_init(&priv->rx_queue_access);

	model = of_get_property(np, "model", NULL);

	for (i = 0; i < MAXGROUPS; i++)
		priv->gfargrp[i].regs = NULL;

	/* Parse and initialize group specific information */
	if (of_device_is_compatible(np, "fsl,etsec2")) {
		priv->mode = MQ_MG_MODE;
		for_each_child_of_node(np, child) {
			err = gfar_parse_group(child, priv, model);
			if (err)
				goto err_grp_init;
		}
	} else {
		priv->mode = SQ_SG_MODE;
		err = gfar_parse_group(np, priv, model);
<<<<<<< HEAD
		if(err)
=======
		if (err)
>>>>>>> refs/remotes/origin/master
			goto err_grp_init;
	}

	for (i = 0; i < priv->num_tx_queues; i++)
<<<<<<< HEAD
	       priv->tx_queue[i] = NULL;
=======
		priv->tx_queue[i] = NULL;
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < priv->num_rx_queues; i++)
		priv->rx_queue[i] = NULL;

	for (i = 0; i < priv->num_tx_queues; i++) {
		priv->tx_queue[i] = kzalloc(sizeof(struct gfar_priv_tx_q),
					    GFP_KERNEL);
		if (!priv->tx_queue[i]) {
			err = -ENOMEM;
			goto tx_alloc_failed;
		}
		priv->tx_queue[i]->tx_skbuff = NULL;
		priv->tx_queue[i]->qindex = i;
		priv->tx_queue[i]->dev = dev;
		spin_lock_init(&(priv->tx_queue[i]->txlock));
	}

	for (i = 0; i < priv->num_rx_queues; i++) {
		priv->rx_queue[i] = kzalloc(sizeof(struct gfar_priv_rx_q),
					    GFP_KERNEL);
		if (!priv->rx_queue[i]) {
			err = -ENOMEM;
			goto rx_alloc_failed;
		}
		priv->rx_queue[i]->rx_skbuff = NULL;
		priv->rx_queue[i]->qindex = i;
		priv->rx_queue[i]->dev = dev;
		spin_lock_init(&(priv->rx_queue[i]->rxlock));
	}


	stash = of_get_property(np, "bd-stash", NULL);

	if (stash) {
		priv->device_flags |= FSL_GIANFAR_DEV_HAS_BD_STASHING;
		priv->bd_stash_en = 1;
	}

	stash_len = of_get_property(np, "rx-stash-len", NULL);

	if (stash_len)
		priv->rx_stash_size = *stash_len;

	stash_idx = of_get_property(np, "rx-stash-idx", NULL);

	if (stash_idx)
		priv->rx_stash_index = *stash_idx;

	if (stash_len || stash_idx)
		priv->device_flags |= FSL_GIANFAR_DEV_HAS_BUF_STASHING;

	mac_addr = of_get_mac_address(np);
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	if (mac_addr)
		memcpy(dev->dev_addr, mac_addr, ETH_ALEN);

	if (model && !strcasecmp(model, "TSEC"))
<<<<<<< HEAD
		priv->device_flags =
			FSL_GIANFAR_DEV_HAS_GIGABIT |
			FSL_GIANFAR_DEV_HAS_COALESCE |
			FSL_GIANFAR_DEV_HAS_RMON |
			FSL_GIANFAR_DEV_HAS_MULTI_INTR;
	if (model && !strcasecmp(model, "eTSEC"))
		priv->device_flags =
			FSL_GIANFAR_DEV_HAS_GIGABIT |
			FSL_GIANFAR_DEV_HAS_COALESCE |
			FSL_GIANFAR_DEV_HAS_RMON |
			FSL_GIANFAR_DEV_HAS_MULTI_INTR |
			FSL_GIANFAR_DEV_HAS_PADDING |
			FSL_GIANFAR_DEV_HAS_CSUM |
			FSL_GIANFAR_DEV_HAS_VLAN |
			FSL_GIANFAR_DEV_HAS_MAGIC_PACKET |
			FSL_GIANFAR_DEV_HAS_EXTENDED_HASH |
			FSL_GIANFAR_DEV_HAS_TIMER;
=======
		priv->device_flags = FSL_GIANFAR_DEV_HAS_GIGABIT |
				     FSL_GIANFAR_DEV_HAS_COALESCE |
				     FSL_GIANFAR_DEV_HAS_RMON |
				     FSL_GIANFAR_DEV_HAS_MULTI_INTR;

	if (model && !strcasecmp(model, "eTSEC"))
		priv->device_flags = FSL_GIANFAR_DEV_HAS_GIGABIT |
				     FSL_GIANFAR_DEV_HAS_COALESCE |
				     FSL_GIANFAR_DEV_HAS_RMON |
				     FSL_GIANFAR_DEV_HAS_MULTI_INTR |
				     FSL_GIANFAR_DEV_HAS_PADDING |
				     FSL_GIANFAR_DEV_HAS_CSUM |
				     FSL_GIANFAR_DEV_HAS_VLAN |
				     FSL_GIANFAR_DEV_HAS_MAGIC_PACKET |
				     FSL_GIANFAR_DEV_HAS_EXTENDED_HASH |
				     FSL_GIANFAR_DEV_HAS_TIMER;
>>>>>>> refs/remotes/origin/master

	ctype = of_get_property(np, "phy-connection-type", NULL);

	/* We only care about rgmii-id.  The rest are autodetected */
	if (ctype && !strcmp(ctype, "rgmii-id"))
		priv->interface = PHY_INTERFACE_MODE_RGMII_ID;
	else
		priv->interface = PHY_INTERFACE_MODE_MII;

	if (of_get_property(np, "fsl,magic-packet", NULL))
		priv->device_flags |= FSL_GIANFAR_DEV_HAS_MAGIC_PACKET;

	priv->phy_node = of_parse_phandle(np, "phy-handle", 0);

	/* Find the TBI PHY.  If it's not there, we don't support SGMII */
	priv->tbi_node = of_parse_phandle(np, "tbi-handle", 0);

	return 0;

rx_alloc_failed:
	free_rx_pointers(priv);
tx_alloc_failed:
	free_tx_pointers(priv);
err_grp_init:
	unmap_group_regs(priv);
<<<<<<< HEAD
	free_netdev(dev);
=======
	free_gfar_dev(priv);
>>>>>>> refs/remotes/origin/master
	return err;
}

static int gfar_hwtstamp_ioctl(struct net_device *netdev,
<<<<<<< HEAD
			struct ifreq *ifr, int cmd)
=======
			       struct ifreq *ifr, int cmd)
>>>>>>> refs/remotes/origin/master
{
	struct hwtstamp_config config;
	struct gfar_private *priv = netdev_priv(netdev);

	if (copy_from_user(&config, ifr->ifr_data, sizeof(config)))
		return -EFAULT;

	/* reserved for future extensions */
	if (config.flags)
		return -EINVAL;

	switch (config.tx_type) {
	case HWTSTAMP_TX_OFF:
		priv->hwts_tx_en = 0;
		break;
	case HWTSTAMP_TX_ON:
		if (!(priv->device_flags & FSL_GIANFAR_DEV_HAS_TIMER))
			return -ERANGE;
		priv->hwts_tx_en = 1;
		break;
	default:
		return -ERANGE;
	}

	switch (config.rx_filter) {
	case HWTSTAMP_FILTER_NONE:
		if (priv->hwts_rx_en) {
			stop_gfar(netdev);
			priv->hwts_rx_en = 0;
			startup_gfar(netdev);
		}
		break;
	default:
		if (!(priv->device_flags & FSL_GIANFAR_DEV_HAS_TIMER))
			return -ERANGE;
		if (!priv->hwts_rx_en) {
			stop_gfar(netdev);
			priv->hwts_rx_en = 1;
			startup_gfar(netdev);
		}
		config.rx_filter = HWTSTAMP_FILTER_ALL;
		break;
	}

	return copy_to_user(ifr->ifr_data, &config, sizeof(config)) ?
		-EFAULT : 0;
}

/* Ioctl MII Interface */
static int gfar_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct gfar_private *priv = netdev_priv(dev);

	if (!netif_running(dev))
		return -EINVAL;

	if (cmd == SIOCSHWTSTAMP)
		return gfar_hwtstamp_ioctl(dev, rq, cmd);

	if (!priv->phydev)
		return -ENODEV;

	return phy_mii_ioctl(priv->phydev, rq, cmd);
}

static unsigned int reverse_bitmap(unsigned int bit_map, unsigned int max_qs)
{
	unsigned int new_bit_map = 0x0;
	int mask = 0x1 << (max_qs - 1), i;
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	for (i = 0; i < max_qs; i++) {
		if (bit_map & mask)
			new_bit_map = new_bit_map + (1 << i);
		mask = mask >> 0x1;
	}
	return new_bit_map;
}

static u32 cluster_entry_per_class(struct gfar_private *priv, u32 rqfar,
				   u32 class)
{
	u32 rqfpr = FPR_FILER_MASK;
	u32 rqfcr = 0x0;

	rqfar--;
	rqfcr = RQFCR_CLE | RQFCR_PID_MASK | RQFCR_CMP_EXACT;
	priv->ftp_rqfpr[rqfar] = rqfpr;
	priv->ftp_rqfcr[rqfar] = rqfcr;
	gfar_write_filer(priv, rqfar, rqfcr, rqfpr);

	rqfar--;
	rqfcr = RQFCR_CMP_NOMATCH;
	priv->ftp_rqfpr[rqfar] = rqfpr;
	priv->ftp_rqfcr[rqfar] = rqfcr;
	gfar_write_filer(priv, rqfar, rqfcr, rqfpr);

	rqfar--;
	rqfcr = RQFCR_CMP_EXACT | RQFCR_PID_PARSE | RQFCR_CLE | RQFCR_AND;
	rqfpr = class;
	priv->ftp_rqfcr[rqfar] = rqfcr;
	priv->ftp_rqfpr[rqfar] = rqfpr;
	gfar_write_filer(priv, rqfar, rqfcr, rqfpr);

	rqfar--;
	rqfcr = RQFCR_CMP_EXACT | RQFCR_PID_MASK | RQFCR_AND;
	rqfpr = class;
	priv->ftp_rqfcr[rqfar] = rqfcr;
	priv->ftp_rqfpr[rqfar] = rqfpr;
	gfar_write_filer(priv, rqfar, rqfcr, rqfpr);

	return rqfar;
}

static void gfar_init_filer_table(struct gfar_private *priv)
{
	int i = 0x0;
	u32 rqfar = MAX_FILER_IDX;
	u32 rqfcr = 0x0;
	u32 rqfpr = FPR_FILER_MASK;

	/* Default rule */
	rqfcr = RQFCR_CMP_MATCH;
	priv->ftp_rqfcr[rqfar] = rqfcr;
	priv->ftp_rqfpr[rqfar] = rqfpr;
	gfar_write_filer(priv, rqfar, rqfcr, rqfpr);

	rqfar = cluster_entry_per_class(priv, rqfar, RQFPR_IPV6);
	rqfar = cluster_entry_per_class(priv, rqfar, RQFPR_IPV6 | RQFPR_UDP);
	rqfar = cluster_entry_per_class(priv, rqfar, RQFPR_IPV6 | RQFPR_TCP);
	rqfar = cluster_entry_per_class(priv, rqfar, RQFPR_IPV4);
	rqfar = cluster_entry_per_class(priv, rqfar, RQFPR_IPV4 | RQFPR_UDP);
	rqfar = cluster_entry_per_class(priv, rqfar, RQFPR_IPV4 | RQFPR_TCP);

	/* cur_filer_idx indicated the first non-masked rule */
	priv->cur_filer_idx = rqfar;

	/* Rest are masked rules */
	rqfcr = RQFCR_CMP_NOMATCH;
	for (i = 0; i < rqfar; i++) {
		priv->ftp_rqfcr[i] = rqfcr;
		priv->ftp_rqfpr[i] = rqfpr;
		gfar_write_filer(priv, i, rqfcr, rqfpr);
	}
}

<<<<<<< HEAD
static void gfar_detect_errata(struct gfar_private *priv)
{
	struct device *dev = &priv->ofdev->dev;
=======
static void __gfar_detect_errata_83xx(struct gfar_private *priv)
{
>>>>>>> refs/remotes/origin/master
	unsigned int pvr = mfspr(SPRN_PVR);
	unsigned int svr = mfspr(SPRN_SVR);
	unsigned int mod = (svr >> 16) & 0xfff6; /* w/o E suffix */
	unsigned int rev = svr & 0xffff;

	/* MPC8313 Rev 2.0 and higher; All MPC837x */
	if ((pvr == 0x80850010 && mod == 0x80b0 && rev >= 0x0020) ||
<<<<<<< HEAD
			(pvr == 0x80861010 && (mod & 0xfff9) == 0x80c0))
=======
	    (pvr == 0x80861010 && (mod & 0xfff9) == 0x80c0))
>>>>>>> refs/remotes/origin/master
		priv->errata |= GFAR_ERRATA_74;

	/* MPC8313 and MPC837x all rev */
	if ((pvr == 0x80850010 && mod == 0x80b0) ||
<<<<<<< HEAD
			(pvr == 0x80861010 && (mod & 0xfff9) == 0x80c0))
		priv->errata |= GFAR_ERRATA_76;

	/* MPC8313 and MPC837x all rev */
	if ((pvr == 0x80850010 && mod == 0x80b0) ||
			(pvr == 0x80861010 && (mod & 0xfff9) == 0x80c0))
		priv->errata |= GFAR_ERRATA_A002;

	/* MPC8313 Rev < 2.0, MPC8548 rev 2.0 */
	if ((pvr == 0x80850010 && mod == 0x80b0 && rev < 0x0020) ||
			(pvr == 0x80210020 && mod == 0x8030 && rev == 0x0020))
		priv->errata |= GFAR_ERRATA_12;
=======
	    (pvr == 0x80861010 && (mod & 0xfff9) == 0x80c0))
		priv->errata |= GFAR_ERRATA_76;

	/* MPC8313 Rev < 2.0 */
	if (pvr == 0x80850010 && mod == 0x80b0 && rev < 0x0020)
		priv->errata |= GFAR_ERRATA_12;
}

static void __gfar_detect_errata_85xx(struct gfar_private *priv)
{
	unsigned int svr = mfspr(SPRN_SVR);

	if ((SVR_SOC_VER(svr) == SVR_8548) && (SVR_REV(svr) == 0x20))
		priv->errata |= GFAR_ERRATA_12;
	if (((SVR_SOC_VER(svr) == SVR_P2020) && (SVR_REV(svr) < 0x20)) ||
	    ((SVR_SOC_VER(svr) == SVR_P2010) && (SVR_REV(svr) < 0x20)))
		priv->errata |= GFAR_ERRATA_76; /* aka eTSEC 20 */
}

static void gfar_detect_errata(struct gfar_private *priv)
{
	struct device *dev = &priv->ofdev->dev;

	/* no plans to fix */
	priv->errata |= GFAR_ERRATA_A002;

	if (pvr_version_is(PVR_VER_E500V1) || pvr_version_is(PVR_VER_E500V2))
		__gfar_detect_errata_85xx(priv);
	else /* non-mpc85xx parts, i.e. e300 core based */
		__gfar_detect_errata_83xx(priv);
>>>>>>> refs/remotes/origin/master

	if (priv->errata)
		dev_info(dev, "enabled errata workarounds, flags: 0x%x\n",
			 priv->errata);
}

/* Set up the ethernet device structure, private data,
<<<<<<< HEAD
 * and anything else we need before we start */
=======
 * and anything else we need before we start
 */
>>>>>>> refs/remotes/origin/master
static int gfar_probe(struct platform_device *ofdev)
{
	u32 tempval;
	struct net_device *dev = NULL;
	struct gfar_private *priv = NULL;
	struct gfar __iomem *regs = NULL;
	int err = 0, i, grp_idx = 0;
	u32 rstat = 0, tstat = 0, rqueue = 0, tqueue = 0;
	u32 isrg = 0;
	u32 __iomem *baddr;

	err = gfar_of_init(ofdev, &dev);

	if (err)
		return err;

	priv = netdev_priv(dev);
	priv->ndev = dev;
	priv->ofdev = ofdev;
<<<<<<< HEAD
	priv->node = ofdev->dev.of_node;
=======
	priv->dev = &ofdev->dev;
>>>>>>> refs/remotes/origin/master
	SET_NETDEV_DEV(dev, &ofdev->dev);

	spin_lock_init(&priv->bflock);
	INIT_WORK(&priv->reset_task, gfar_reset_task);

<<<<<<< HEAD
	dev_set_drvdata(&ofdev->dev, priv);
=======
	platform_set_drvdata(ofdev, priv);
>>>>>>> refs/remotes/origin/master
	regs = priv->gfargrp[0].regs;

	gfar_detect_errata(priv);

<<<<<<< HEAD
	/* Stop the DMA engine now, in case it was running before */
	/* (The firmware could have used it, and left it running). */
=======
	/* Stop the DMA engine now, in case it was running before
	 * (The firmware could have used it, and left it running).
	 */
>>>>>>> refs/remotes/origin/master
	gfar_halt(dev);

	/* Reset MAC layer */
	gfar_write(&regs->maccfg1, MACCFG1_SOFT_RESET);

	/* We need to delay at least 3 TX clocks */
	udelay(2);

<<<<<<< HEAD
	tempval = (MACCFG1_TX_FLOW | MACCFG1_RX_FLOW);
=======
	tempval = 0;
	if (!priv->pause_aneg_en && priv->tx_pause_en)
		tempval |= MACCFG1_TX_FLOW;
	if (!priv->pause_aneg_en && priv->rx_pause_en)
		tempval |= MACCFG1_RX_FLOW;
	/* the soft reset bit is not self-resetting, so we need to
	 * clear it before resuming normal operation
	 */
>>>>>>> refs/remotes/origin/master
	gfar_write(&regs->maccfg1, tempval);

	/* Initialize MACCFG2. */
	tempval = MACCFG2_INIT_SETTINGS;
	if (gfar_has_errata(priv, GFAR_ERRATA_74))
		tempval |= MACCFG2_HUGEFRAME | MACCFG2_LENGTHCHECK;
	gfar_write(&regs->maccfg2, tempval);

	/* Initialize ECNTRL */
	gfar_write(&regs->ecntrl, ECNTRL_INIT_SETTINGS);

	/* Set the dev->base_addr to the gfar reg region */
	dev->base_addr = (unsigned long) regs;

<<<<<<< HEAD
	SET_NETDEV_DEV(dev, &ofdev->dev);

=======
>>>>>>> refs/remotes/origin/master
	/* Fill in the dev structure */
	dev->watchdog_timeo = TX_TIMEOUT;
	dev->mtu = 1500;
	dev->netdev_ops = &gfar_netdev_ops;
	dev->ethtool_ops = &gfar_ethtool_ops;

	/* Register for napi ...We are registering NAPI for each grp */
<<<<<<< HEAD
	for (i = 0; i < priv->num_grps; i++)
		netif_napi_add(dev, &priv->gfargrp[i].napi, gfar_poll, GFAR_DEV_WEIGHT);

	if (priv->device_flags & FSL_GIANFAR_DEV_HAS_CSUM) {
		dev->hw_features = NETIF_F_IP_CSUM | NETIF_F_SG |
			NETIF_F_RXCSUM;
		dev->features |= NETIF_F_IP_CSUM | NETIF_F_SG |
			NETIF_F_RXCSUM | NETIF_F_HIGHDMA;
	}

	if (priv->device_flags & FSL_GIANFAR_DEV_HAS_VLAN) {
		dev->hw_features |= NETIF_F_HW_VLAN_TX | NETIF_F_HW_VLAN_RX;
		dev->features |= NETIF_F_HW_VLAN_RX;
<<<<<<< HEAD
=======
	if (priv->mode == SQ_SG_MODE)
		netif_napi_add(dev, &priv->gfargrp[0].napi, gfar_poll_sq,
			       GFAR_DEV_WEIGHT);
	else
		for (i = 0; i < priv->num_grps; i++)
			netif_napi_add(dev, &priv->gfargrp[i].napi, gfar_poll,
				       GFAR_DEV_WEIGHT);

	if (priv->device_flags & FSL_GIANFAR_DEV_HAS_CSUM) {
		dev->hw_features = NETIF_F_IP_CSUM | NETIF_F_SG |
				   NETIF_F_RXCSUM;
		dev->features |= NETIF_F_IP_CSUM | NETIF_F_SG |
				 NETIF_F_RXCSUM | NETIF_F_HIGHDMA;
	}

	if (priv->device_flags & FSL_GIANFAR_DEV_HAS_VLAN) {
		dev->hw_features |= NETIF_F_HW_VLAN_CTAG_TX |
				    NETIF_F_HW_VLAN_CTAG_RX;
		dev->features |= NETIF_F_HW_VLAN_CTAG_RX;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}

	if (priv->device_flags & FSL_GIANFAR_DEV_HAS_EXTENDED_HASH) {
		priv->extended_hash = 1;
		priv->hash_width = 9;

		priv->hash_regs[0] = &regs->igaddr0;
		priv->hash_regs[1] = &regs->igaddr1;
		priv->hash_regs[2] = &regs->igaddr2;
		priv->hash_regs[3] = &regs->igaddr3;
		priv->hash_regs[4] = &regs->igaddr4;
		priv->hash_regs[5] = &regs->igaddr5;
		priv->hash_regs[6] = &regs->igaddr6;
		priv->hash_regs[7] = &regs->igaddr7;
		priv->hash_regs[8] = &regs->gaddr0;
		priv->hash_regs[9] = &regs->gaddr1;
		priv->hash_regs[10] = &regs->gaddr2;
		priv->hash_regs[11] = &regs->gaddr3;
		priv->hash_regs[12] = &regs->gaddr4;
		priv->hash_regs[13] = &regs->gaddr5;
		priv->hash_regs[14] = &regs->gaddr6;
		priv->hash_regs[15] = &regs->gaddr7;

	} else {
		priv->extended_hash = 0;
		priv->hash_width = 8;

		priv->hash_regs[0] = &regs->gaddr0;
		priv->hash_regs[1] = &regs->gaddr1;
		priv->hash_regs[2] = &regs->gaddr2;
		priv->hash_regs[3] = &regs->gaddr3;
		priv->hash_regs[4] = &regs->gaddr4;
		priv->hash_regs[5] = &regs->gaddr5;
		priv->hash_regs[6] = &regs->gaddr6;
		priv->hash_regs[7] = &regs->gaddr7;
	}

	if (priv->device_flags & FSL_GIANFAR_DEV_HAS_PADDING)
		priv->padding = DEFAULT_PADDING;
	else
		priv->padding = 0;

	if (dev->features & NETIF_F_IP_CSUM ||
<<<<<<< HEAD
			priv->device_flags & FSL_GIANFAR_DEV_HAS_TIMER)
		dev->hard_header_len += GMAC_FCB_LEN;
=======
	    priv->device_flags & FSL_GIANFAR_DEV_HAS_TIMER)
		dev->needed_headroom = GMAC_FCB_LEN;
>>>>>>> refs/remotes/origin/master

	/* Program the isrg regs only if number of grps > 1 */
	if (priv->num_grps > 1) {
		baddr = &regs->isrg0;
		for (i = 0; i < priv->num_grps; i++) {
			isrg |= (priv->gfargrp[i].rx_bit_map << ISRG_SHIFT_RX);
			isrg |= (priv->gfargrp[i].tx_bit_map << ISRG_SHIFT_TX);
			gfar_write(baddr, isrg);
			baddr++;
			isrg = 0x0;
		}
	}

	/* Need to reverse the bit maps as  bit_map's MSB is q0
	 * but, for_each_set_bit parses from right to left, which
<<<<<<< HEAD
	 * basically reverses the queue numbers */
	for (i = 0; i< priv->num_grps; i++) {
		priv->gfargrp[i].tx_bit_map = reverse_bitmap(
				priv->gfargrp[i].tx_bit_map, MAX_TX_QS);
		priv->gfargrp[i].rx_bit_map = reverse_bitmap(
				priv->gfargrp[i].rx_bit_map, MAX_RX_QS);
	}

	/* Calculate RSTAT, TSTAT, RQUEUE and TQUEUE values,
	 * also assign queues to groups */
	for (grp_idx = 0; grp_idx < priv->num_grps; grp_idx++) {
		priv->gfargrp[grp_idx].num_rx_queues = 0x0;
		for_each_set_bit(i, &priv->gfargrp[grp_idx].rx_bit_map,
				priv->num_rx_queues) {
=======
	 * basically reverses the queue numbers
	 */
	for (i = 0; i< priv->num_grps; i++) {
		priv->gfargrp[i].tx_bit_map =
			reverse_bitmap(priv->gfargrp[i].tx_bit_map, MAX_TX_QS);
		priv->gfargrp[i].rx_bit_map =
			reverse_bitmap(priv->gfargrp[i].rx_bit_map, MAX_RX_QS);
	}

	/* Calculate RSTAT, TSTAT, RQUEUE and TQUEUE values,
	 * also assign queues to groups
	 */
	for (grp_idx = 0; grp_idx < priv->num_grps; grp_idx++) {
		priv->gfargrp[grp_idx].num_rx_queues = 0x0;

		for_each_set_bit(i, &priv->gfargrp[grp_idx].rx_bit_map,
				 priv->num_rx_queues) {
>>>>>>> refs/remotes/origin/master
			priv->gfargrp[grp_idx].num_rx_queues++;
			priv->rx_queue[i]->grp = &priv->gfargrp[grp_idx];
			rstat = rstat | (RSTAT_CLEAR_RHALT >> i);
			rqueue = rqueue | ((RQUEUE_EN0 | RQUEUE_EX0) >> i);
		}
		priv->gfargrp[grp_idx].num_tx_queues = 0x0;
<<<<<<< HEAD
		for_each_set_bit(i, &priv->gfargrp[grp_idx].tx_bit_map,
				priv->num_tx_queues) {
=======

		for_each_set_bit(i, &priv->gfargrp[grp_idx].tx_bit_map,
				 priv->num_tx_queues) {
>>>>>>> refs/remotes/origin/master
			priv->gfargrp[grp_idx].num_tx_queues++;
			priv->tx_queue[i]->grp = &priv->gfargrp[grp_idx];
			tstat = tstat | (TSTAT_CLEAR_THALT >> i);
			tqueue = tqueue | (TQUEUE_EN0 >> i);
		}
		priv->gfargrp[grp_idx].rstat = rstat;
		priv->gfargrp[grp_idx].tstat = tstat;
		rstat = tstat =0;
	}

	gfar_write(&regs->rqueue, rqueue);
	gfar_write(&regs->tqueue, tqueue);

	priv->rx_buffer_size = DEFAULT_RX_BUFFER_SIZE;

	/* Initializing some of the rx/tx queue level parameters */
	for (i = 0; i < priv->num_tx_queues; i++) {
		priv->tx_queue[i]->tx_ring_size = DEFAULT_TX_RING_SIZE;
		priv->tx_queue[i]->num_txbdfree = DEFAULT_TX_RING_SIZE;
		priv->tx_queue[i]->txcoalescing = DEFAULT_TX_COALESCE;
		priv->tx_queue[i]->txic = DEFAULT_TXIC;
	}

	for (i = 0; i < priv->num_rx_queues; i++) {
		priv->rx_queue[i]->rx_ring_size = DEFAULT_RX_RING_SIZE;
		priv->rx_queue[i]->rxcoalescing = DEFAULT_RX_COALESCE;
		priv->rx_queue[i]->rxic = DEFAULT_RXIC;
	}

<<<<<<< HEAD
	/* always enable rx filer*/
=======
	/* always enable rx filer */
>>>>>>> refs/remotes/origin/master
	priv->rx_filer_enable = 1;
	/* Enable most messages by default */
	priv->msg_enable = (NETIF_MSG_IFUP << 1 ) - 1;
	/* use pritority h/w tx queue scheduling for single queue devices */
	if (priv->num_tx_queues == 1)
		priv->prio_sched_en = 1;

	/* Carrier starts down, phylib will bring it up */
	netif_carrier_off(dev);

	err = register_netdev(dev);

	if (err) {
		pr_err("%s: Cannot register net device, aborting\n", dev->name);
		goto register_fail;
	}

	device_init_wakeup(&dev->dev,
<<<<<<< HEAD
		priv->device_flags & FSL_GIANFAR_DEV_HAS_MAGIC_PACKET);

	/* fill out IRQ number and name fields */
	for (i = 0; i < priv->num_grps; i++) {
		if (priv->device_flags & FSL_GIANFAR_DEV_HAS_MULTI_INTR) {
			sprintf(priv->gfargrp[i].int_name_tx, "%s%s%c%s",
				dev->name, "_g", '0' + i, "_tx");
			sprintf(priv->gfargrp[i].int_name_rx, "%s%s%c%s",
				dev->name, "_g", '0' + i, "_rx");
			sprintf(priv->gfargrp[i].int_name_er, "%s%s%c%s",
				dev->name, "_g", '0' + i, "_er");
		} else
			strcpy(priv->gfargrp[i].int_name_tx, dev->name);
=======
			   priv->device_flags &
			   FSL_GIANFAR_DEV_HAS_MAGIC_PACKET);

	/* fill out IRQ number and name fields */
	for (i = 0; i < priv->num_grps; i++) {
		struct gfar_priv_grp *grp = &priv->gfargrp[i];
		if (priv->device_flags & FSL_GIANFAR_DEV_HAS_MULTI_INTR) {
			sprintf(gfar_irq(grp, TX)->name, "%s%s%c%s",
				dev->name, "_g", '0' + i, "_tx");
			sprintf(gfar_irq(grp, RX)->name, "%s%s%c%s",
				dev->name, "_g", '0' + i, "_rx");
			sprintf(gfar_irq(grp, ER)->name, "%s%s%c%s",
				dev->name, "_g", '0' + i, "_er");
		} else
			strcpy(gfar_irq(grp, TX)->name, dev->name);
>>>>>>> refs/remotes/origin/master
	}

	/* Initialize the filer table */
	gfar_init_filer_table(priv);

	/* Create all the sysfs files */
	gfar_init_sysfs(dev);

	/* Print out the device info */
	netdev_info(dev, "mac: %pM\n", dev->dev_addr);

<<<<<<< HEAD
	/* Even more device info helps when determining which kernel */
	/* provided which set of benchmarks. */
=======
	/* Even more device info helps when determining which kernel
	 * provided which set of benchmarks.
	 */
>>>>>>> refs/remotes/origin/master
	netdev_info(dev, "Running with NAPI enabled\n");
	for (i = 0; i < priv->num_rx_queues; i++)
		netdev_info(dev, "RX BD ring size for Q[%d]: %d\n",
			    i, priv->rx_queue[i]->rx_ring_size);
<<<<<<< HEAD
	for(i = 0; i < priv->num_tx_queues; i++)
=======
	for (i = 0; i < priv->num_tx_queues; i++)
>>>>>>> refs/remotes/origin/master
		netdev_info(dev, "TX BD ring size for Q[%d]: %d\n",
			    i, priv->tx_queue[i]->tx_ring_size);

	return 0;

register_fail:
	unmap_group_regs(priv);
	free_tx_pointers(priv);
	free_rx_pointers(priv);
	if (priv->phy_node)
		of_node_put(priv->phy_node);
	if (priv->tbi_node)
		of_node_put(priv->tbi_node);
<<<<<<< HEAD
	free_netdev(dev);
=======
	free_gfar_dev(priv);
>>>>>>> refs/remotes/origin/master
	return err;
}

static int gfar_remove(struct platform_device *ofdev)
{
<<<<<<< HEAD
	struct gfar_private *priv = dev_get_drvdata(&ofdev->dev);
=======
	struct gfar_private *priv = platform_get_drvdata(ofdev);
>>>>>>> refs/remotes/origin/master

	if (priv->phy_node)
		of_node_put(priv->phy_node);
	if (priv->tbi_node)
		of_node_put(priv->tbi_node);

<<<<<<< HEAD
	dev_set_drvdata(&ofdev->dev, NULL);

	unregister_netdev(priv->ndev);
	unmap_group_regs(priv);
	free_netdev(priv->ndev);
=======
	unregister_netdev(priv->ndev);
	unmap_group_regs(priv);
	free_gfar_dev(priv);
>>>>>>> refs/remotes/origin/master

	return 0;
}

#ifdef CONFIG_PM

static int gfar_suspend(struct device *dev)
{
	struct gfar_private *priv = dev_get_drvdata(dev);
	struct net_device *ndev = priv->ndev;
	struct gfar __iomem *regs = priv->gfargrp[0].regs;
	unsigned long flags;
	u32 tempval;

	int magic_packet = priv->wol_en &&
<<<<<<< HEAD
		(priv->device_flags & FSL_GIANFAR_DEV_HAS_MAGIC_PACKET);
=======
			   (priv->device_flags &
			    FSL_GIANFAR_DEV_HAS_MAGIC_PACKET);
>>>>>>> refs/remotes/origin/master

	netif_device_detach(ndev);

	if (netif_running(ndev)) {

		local_irq_save(flags);
		lock_tx_qs(priv);
		lock_rx_qs(priv);

		gfar_halt_nodisable(ndev);

		/* Disable Tx, and Rx if wake-on-LAN is disabled. */
		tempval = gfar_read(&regs->maccfg1);

		tempval &= ~MACCFG1_TX_EN;

		if (!magic_packet)
			tempval &= ~MACCFG1_RX_EN;

		gfar_write(&regs->maccfg1, tempval);

		unlock_rx_qs(priv);
		unlock_tx_qs(priv);
		local_irq_restore(flags);

		disable_napi(priv);

		if (magic_packet) {
			/* Enable interrupt on Magic Packet */
			gfar_write(&regs->imask, IMASK_MAG);

			/* Enable Magic Packet mode */
			tempval = gfar_read(&regs->maccfg2);
			tempval |= MACCFG2_MPEN;
			gfar_write(&regs->maccfg2, tempval);
		} else {
			phy_stop(priv->phydev);
		}
	}

	return 0;
}

static int gfar_resume(struct device *dev)
{
	struct gfar_private *priv = dev_get_drvdata(dev);
	struct net_device *ndev = priv->ndev;
	struct gfar __iomem *regs = priv->gfargrp[0].regs;
	unsigned long flags;
	u32 tempval;
	int magic_packet = priv->wol_en &&
<<<<<<< HEAD
		(priv->device_flags & FSL_GIANFAR_DEV_HAS_MAGIC_PACKET);
=======
			   (priv->device_flags &
			    FSL_GIANFAR_DEV_HAS_MAGIC_PACKET);
>>>>>>> refs/remotes/origin/master

	if (!netif_running(ndev)) {
		netif_device_attach(ndev);
		return 0;
	}

	if (!magic_packet && priv->phydev)
		phy_start(priv->phydev);

	/* Disable Magic Packet mode, in case something
	 * else woke us up.
	 */
	local_irq_save(flags);
	lock_tx_qs(priv);
	lock_rx_qs(priv);

	tempval = gfar_read(&regs->maccfg2);
	tempval &= ~MACCFG2_MPEN;
	gfar_write(&regs->maccfg2, tempval);

	gfar_start(ndev);

	unlock_rx_qs(priv);
	unlock_tx_qs(priv);
	local_irq_restore(flags);

	netif_device_attach(ndev);

	enable_napi(priv);

	return 0;
}

static int gfar_restore(struct device *dev)
{
	struct gfar_private *priv = dev_get_drvdata(dev);
	struct net_device *ndev = priv->ndev;

<<<<<<< HEAD
	if (!netif_running(ndev))
		return 0;

	gfar_init_bds(ndev);
=======
	if (!netif_running(ndev)) {
		netif_device_attach(ndev);

		return 0;
	}

	if (gfar_init_bds(ndev)) {
		free_skb_resources(priv);
		return -ENOMEM;
	}

>>>>>>> refs/remotes/origin/master
	init_registers(ndev);
	gfar_set_mac_address(ndev);
	gfar_init_mac(ndev);
	gfar_start(ndev);

	priv->oldlink = 0;
	priv->oldspeed = 0;
	priv->oldduplex = -1;

	if (priv->phydev)
		phy_start(priv->phydev);

	netif_device_attach(ndev);
	enable_napi(priv);

	return 0;
}

static struct dev_pm_ops gfar_pm_ops = {
	.suspend = gfar_suspend,
	.resume = gfar_resume,
	.freeze = gfar_suspend,
	.thaw = gfar_resume,
	.restore = gfar_restore,
};

#define GFAR_PM_OPS (&gfar_pm_ops)

#else

#define GFAR_PM_OPS NULL

#endif

/* Reads the controller's registers to determine what interface
 * connects it to the PHY.
 */
static phy_interface_t gfar_get_interface(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct gfar __iomem *regs = priv->gfargrp[0].regs;
	u32 ecntrl;

	ecntrl = gfar_read(&regs->ecntrl);

	if (ecntrl & ECNTRL_SGMII_MODE)
		return PHY_INTERFACE_MODE_SGMII;

	if (ecntrl & ECNTRL_TBI_MODE) {
		if (ecntrl & ECNTRL_REDUCED_MODE)
			return PHY_INTERFACE_MODE_RTBI;
		else
			return PHY_INTERFACE_MODE_TBI;
	}

	if (ecntrl & ECNTRL_REDUCED_MODE) {
<<<<<<< HEAD
		if (ecntrl & ECNTRL_REDUCED_MII_MODE)
			return PHY_INTERFACE_MODE_RMII;
		else {
			phy_interface_t interface = priv->interface;

			/*
			 * This isn't autodetected right now, so it must
=======
		if (ecntrl & ECNTRL_REDUCED_MII_MODE) {
			return PHY_INTERFACE_MODE_RMII;
		}
		else {
			phy_interface_t interface = priv->interface;

			/* This isn't autodetected right now, so it must
>>>>>>> refs/remotes/origin/master
			 * be set by the device tree or platform code.
			 */
			if (interface == PHY_INTERFACE_MODE_RGMII_ID)
				return PHY_INTERFACE_MODE_RGMII_ID;

			return PHY_INTERFACE_MODE_RGMII;
		}
	}

	if (priv->device_flags & FSL_GIANFAR_DEV_HAS_GIGABIT)
		return PHY_INTERFACE_MODE_GMII;

	return PHY_INTERFACE_MODE_MII;
}


/* Initializes driver's PHY state, and attaches to the PHY.
 * Returns 0 on success.
 */
static int init_phy(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	uint gigabit_support =
		priv->device_flags & FSL_GIANFAR_DEV_HAS_GIGABIT ?
<<<<<<< HEAD
		SUPPORTED_1000baseT_Full : 0;
=======
		GFAR_SUPPORTED_GBIT : 0;
>>>>>>> refs/remotes/origin/master
	phy_interface_t interface;

	priv->oldlink = 0;
	priv->oldspeed = 0;
	priv->oldduplex = -1;

	interface = gfar_get_interface(dev);

	priv->phydev = of_phy_connect(dev, priv->phy_node, &adjust_link, 0,
				      interface);
	if (!priv->phydev)
		priv->phydev = of_phy_connect_fixed_link(dev, &adjust_link,
							 interface);
	if (!priv->phydev) {
		dev_err(&dev->dev, "could not attach to PHY\n");
		return -ENODEV;
	}

	if (interface == PHY_INTERFACE_MODE_SGMII)
		gfar_configure_serdes(dev);

	/* Remove any features not supported by the controller */
	priv->phydev->supported &= (GFAR_SUPPORTED | gigabit_support);
	priv->phydev->advertising = priv->phydev->supported;

	return 0;
}

<<<<<<< HEAD
/*
 * Initialize TBI PHY interface for communicating with the
=======
/* Initialize TBI PHY interface for communicating with the
>>>>>>> refs/remotes/origin/master
 * SERDES lynx PHY on the chip.  We communicate with this PHY
 * through the MDIO bus on each controller, treating it as a
 * "normal" PHY at the address found in the TBIPA register.  We assume
 * that the TBIPA register is valid.  Either the MDIO bus code will set
 * it to a value that doesn't conflict with other PHYs on the bus, or the
 * value doesn't matter, as there are no other PHYs on the bus.
 */
static void gfar_configure_serdes(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct phy_device *tbiphy;

	if (!priv->tbi_node) {
		dev_warn(&dev->dev, "error: SGMII mode requires that the "
				    "device tree specify a tbi-handle\n");
		return;
	}

	tbiphy = of_phy_find_device(priv->tbi_node);
	if (!tbiphy) {
		dev_err(&dev->dev, "error: Could not get TBI device\n");
		return;
	}

<<<<<<< HEAD
	/*
	 * If the link is already up, we must already be ok, and don't need to
=======
	/* If the link is already up, we must already be ok, and don't need to
>>>>>>> refs/remotes/origin/master
	 * configure and reset the TBI<->SerDes link.  Maybe U-Boot configured
	 * everything for us?  Resetting it takes the link down and requires
	 * several seconds for it to come back.
	 */
	if (phy_read(tbiphy, MII_BMSR) & BMSR_LSTATUS)
		return;

	/* Single clk mode, mii mode off(for serdes communication) */
	phy_write(tbiphy, MII_TBICON, TBICON_CLK_SELECT);

	phy_write(tbiphy, MII_ADVERTISE,
<<<<<<< HEAD
			ADVERTISE_1000XFULL | ADVERTISE_1000XPAUSE |
			ADVERTISE_1000XPSE_ASYM);

	phy_write(tbiphy, MII_BMCR, BMCR_ANENABLE |
			BMCR_ANRESTART | BMCR_FULLDPLX | BMCR_SPEED1000);
=======
		  ADVERTISE_1000XFULL | ADVERTISE_1000XPAUSE |
		  ADVERTISE_1000XPSE_ASYM);

	phy_write(tbiphy, MII_BMCR,
		  BMCR_ANENABLE | BMCR_ANRESTART | BMCR_FULLDPLX |
		  BMCR_SPEED1000);
>>>>>>> refs/remotes/origin/master
}

static void init_registers(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct gfar __iomem *regs = NULL;
<<<<<<< HEAD
	int i = 0;
=======
	int i;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < priv->num_grps; i++) {
		regs = priv->gfargrp[i].regs;
		/* Clear IEVENT */
		gfar_write(&regs->ievent, IEVENT_INIT_CLEAR);

		/* Initialize IMASK */
		gfar_write(&regs->imask, IMASK_INIT_CLEAR);
	}

	regs = priv->gfargrp[0].regs;
	/* Init hash registers to zero */
	gfar_write(&regs->igaddr0, 0);
	gfar_write(&regs->igaddr1, 0);
	gfar_write(&regs->igaddr2, 0);
	gfar_write(&regs->igaddr3, 0);
	gfar_write(&regs->igaddr4, 0);
	gfar_write(&regs->igaddr5, 0);
	gfar_write(&regs->igaddr6, 0);
	gfar_write(&regs->igaddr7, 0);

	gfar_write(&regs->gaddr0, 0);
	gfar_write(&regs->gaddr1, 0);
	gfar_write(&regs->gaddr2, 0);
	gfar_write(&regs->gaddr3, 0);
	gfar_write(&regs->gaddr4, 0);
	gfar_write(&regs->gaddr5, 0);
	gfar_write(&regs->gaddr6, 0);
	gfar_write(&regs->gaddr7, 0);

	/* Zero out the rmon mib registers if it has them */
	if (priv->device_flags & FSL_GIANFAR_DEV_HAS_RMON) {
		memset_io(&(regs->rmon), 0, sizeof (struct rmon_mib));

		/* Mask off the CAM interrupts */
		gfar_write(&regs->rmon.cam1, 0xffffffff);
		gfar_write(&regs->rmon.cam2, 0xffffffff);
	}

	/* Initialize the max receive buffer length */
	gfar_write(&regs->mrblr, priv->rx_buffer_size);

	/* Initialize the Minimum Frame Length Register */
	gfar_write(&regs->minflr, MINFLR_INIT_SETTINGS);
}

static int __gfar_is_rx_idle(struct gfar_private *priv)
{
	u32 res;

<<<<<<< HEAD
	/*
	 * Normaly TSEC should not hang on GRS commands, so we should
	 * actually wait for IEVENT_GRSC flag.
	 */
	if (likely(!gfar_has_errata(priv, GFAR_ERRATA_A002)))
		return 0;

	/*
	 * Read the eTSEC register at offset 0xD1C. If bits 7-14 are
=======
	/* Normaly TSEC should not hang on GRS commands, so we should
	 * actually wait for IEVENT_GRSC flag.
	 */
	if (!gfar_has_errata(priv, GFAR_ERRATA_A002))
		return 0;

	/* Read the eTSEC register at offset 0xD1C. If bits 7-14 are
>>>>>>> refs/remotes/origin/master
	 * the same as bits 23-30, the eTSEC Rx is assumed to be idle
	 * and the Rx can be safely reset.
	 */
	res = gfar_read((void __iomem *)priv->gfargrp[0].regs + 0xd1c);
	res &= 0x7f807f80;
	if ((res & 0xffff) == (res >> 16))
		return 1;

	return 0;
}

/* Halt the receive and transmit queues */
static void gfar_halt_nodisable(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct gfar __iomem *regs = NULL;
	u32 tempval;
<<<<<<< HEAD
	int i = 0;
=======
	int i;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < priv->num_grps; i++) {
		regs = priv->gfargrp[i].regs;
		/* Mask all interrupts */
		gfar_write(&regs->imask, IMASK_INIT_CLEAR);

		/* Clear all interrupts */
		gfar_write(&regs->ievent, IEVENT_INIT_CLEAR);
	}

	regs = priv->gfargrp[0].regs;
	/* Stop the DMA, and wait for it to stop */
	tempval = gfar_read(&regs->dmactrl);
<<<<<<< HEAD
	if ((tempval & (DMACTRL_GRS | DMACTRL_GTS))
	    != (DMACTRL_GRS | DMACTRL_GTS)) {
=======
	if ((tempval & (DMACTRL_GRS | DMACTRL_GTS)) !=
	    (DMACTRL_GRS | DMACTRL_GTS)) {
>>>>>>> refs/remotes/origin/master
		int ret;

		tempval |= (DMACTRL_GRS | DMACTRL_GTS);
		gfar_write(&regs->dmactrl, tempval);

		do {
			ret = spin_event_timeout(((gfar_read(&regs->ievent) &
				 (IEVENT_GRSC | IEVENT_GTSC)) ==
				 (IEVENT_GRSC | IEVENT_GTSC)), 1000000, 0);
			if (!ret && !(gfar_read(&regs->ievent) & IEVENT_GRSC))
				ret = __gfar_is_rx_idle(priv);
		} while (!ret);
	}
}

/* Halt the receive and transmit queues */
void gfar_halt(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct gfar __iomem *regs = priv->gfargrp[0].regs;
	u32 tempval;

	gfar_halt_nodisable(dev);

	/* Disable Rx and Tx */
	tempval = gfar_read(&regs->maccfg1);
	tempval &= ~(MACCFG1_RX_EN | MACCFG1_TX_EN);
	gfar_write(&regs->maccfg1, tempval);
}

static void free_grp_irqs(struct gfar_priv_grp *grp)
{
<<<<<<< HEAD
	free_irq(grp->interruptError, grp);
	free_irq(grp->interruptTransmit, grp);
	free_irq(grp->interruptReceive, grp);
=======
	free_irq(gfar_irq(grp, TX)->irq, grp);
	free_irq(gfar_irq(grp, RX)->irq, grp);
	free_irq(gfar_irq(grp, ER)->irq, grp);
>>>>>>> refs/remotes/origin/master
}

void stop_gfar(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	unsigned long flags;
	int i;

	phy_stop(priv->phydev);


	/* Lock it down */
	local_irq_save(flags);
	lock_tx_qs(priv);
	lock_rx_qs(priv);

	gfar_halt(dev);

	unlock_rx_qs(priv);
	unlock_tx_qs(priv);
	local_irq_restore(flags);

	/* Free the IRQs */
	if (priv->device_flags & FSL_GIANFAR_DEV_HAS_MULTI_INTR) {
		for (i = 0; i < priv->num_grps; i++)
			free_grp_irqs(&priv->gfargrp[i]);
	} else {
		for (i = 0; i < priv->num_grps; i++)
<<<<<<< HEAD
			free_irq(priv->gfargrp[i].interruptTransmit,
					&priv->gfargrp[i]);
=======
			free_irq(gfar_irq(&priv->gfargrp[i], TX)->irq,
				 &priv->gfargrp[i]);
>>>>>>> refs/remotes/origin/master
	}

	free_skb_resources(priv);
}

static void free_skb_tx_queue(struct gfar_priv_tx_q *tx_queue)
{
	struct txbd8 *txbdp;
	struct gfar_private *priv = netdev_priv(tx_queue->dev);
	int i, j;

	txbdp = tx_queue->tx_bd_base;

	for (i = 0; i < tx_queue->tx_ring_size; i++) {
		if (!tx_queue->tx_skbuff[i])
			continue;

<<<<<<< HEAD
		dma_unmap_single(&priv->ofdev->dev, txbdp->bufPtr,
				txbdp->length, DMA_TO_DEVICE);
		txbdp->lstatus = 0;
		for (j = 0; j < skb_shinfo(tx_queue->tx_skbuff[i])->nr_frags;
				j++) {
			txbdp++;
			dma_unmap_page(&priv->ofdev->dev, txbdp->bufPtr,
					txbdp->length, DMA_TO_DEVICE);
=======
		dma_unmap_single(priv->dev, txbdp->bufPtr,
				 txbdp->length, DMA_TO_DEVICE);
		txbdp->lstatus = 0;
		for (j = 0; j < skb_shinfo(tx_queue->tx_skbuff[i])->nr_frags;
		     j++) {
			txbdp++;
			dma_unmap_page(priv->dev, txbdp->bufPtr,
				       txbdp->length, DMA_TO_DEVICE);
>>>>>>> refs/remotes/origin/master
		}
		txbdp++;
		dev_kfree_skb_any(tx_queue->tx_skbuff[i]);
		tx_queue->tx_skbuff[i] = NULL;
	}
	kfree(tx_queue->tx_skbuff);
<<<<<<< HEAD
=======
	tx_queue->tx_skbuff = NULL;
>>>>>>> refs/remotes/origin/master
}

static void free_skb_rx_queue(struct gfar_priv_rx_q *rx_queue)
{
	struct rxbd8 *rxbdp;
	struct gfar_private *priv = netdev_priv(rx_queue->dev);
	int i;

	rxbdp = rx_queue->rx_bd_base;

	for (i = 0; i < rx_queue->rx_ring_size; i++) {
		if (rx_queue->rx_skbuff[i]) {
<<<<<<< HEAD
			dma_unmap_single(&priv->ofdev->dev,
					rxbdp->bufPtr, priv->rx_buffer_size,
					DMA_FROM_DEVICE);
=======
			dma_unmap_single(priv->dev, rxbdp->bufPtr,
					 priv->rx_buffer_size,
					 DMA_FROM_DEVICE);
>>>>>>> refs/remotes/origin/master
			dev_kfree_skb_any(rx_queue->rx_skbuff[i]);
			rx_queue->rx_skbuff[i] = NULL;
		}
		rxbdp->lstatus = 0;
		rxbdp->bufPtr = 0;
		rxbdp++;
	}
	kfree(rx_queue->rx_skbuff);
<<<<<<< HEAD
}

/* If there are any tx skbs or rx skbs still around, free them.
 * Then free tx_skbuff and rx_skbuff */
=======
	rx_queue->rx_skbuff = NULL;
}

/* If there are any tx skbs or rx skbs still around, free them.
 * Then free tx_skbuff and rx_skbuff
 */
>>>>>>> refs/remotes/origin/master
static void free_skb_resources(struct gfar_private *priv)
{
	struct gfar_priv_tx_q *tx_queue = NULL;
	struct gfar_priv_rx_q *rx_queue = NULL;
	int i;

	/* Go through all the buffer descriptors and free their data buffers */
	for (i = 0; i < priv->num_tx_queues; i++) {
		struct netdev_queue *txq;
<<<<<<< HEAD
		tx_queue = priv->tx_queue[i];
		txq = netdev_get_tx_queue(tx_queue->dev, tx_queue->qindex);
		if(tx_queue->tx_skbuff)
=======

		tx_queue = priv->tx_queue[i];
		txq = netdev_get_tx_queue(tx_queue->dev, tx_queue->qindex);
		if (tx_queue->tx_skbuff)
>>>>>>> refs/remotes/origin/master
			free_skb_tx_queue(tx_queue);
		netdev_tx_reset_queue(txq);
	}

	for (i = 0; i < priv->num_rx_queues; i++) {
		rx_queue = priv->rx_queue[i];
<<<<<<< HEAD
		if(rx_queue->rx_skbuff)
			free_skb_rx_queue(rx_queue);
	}

	dma_free_coherent(&priv->ofdev->dev,
			sizeof(struct txbd8) * priv->total_tx_ring_size +
			sizeof(struct rxbd8) * priv->total_rx_ring_size,
			priv->tx_queue[0]->tx_bd_base,
			priv->tx_queue[0]->tx_bd_dma_base);
	skb_queue_purge(&priv->rx_recycle);
=======
		if (rx_queue->rx_skbuff)
			free_skb_rx_queue(rx_queue);
	}

	dma_free_coherent(priv->dev,
			  sizeof(struct txbd8) * priv->total_tx_ring_size +
			  sizeof(struct rxbd8) * priv->total_rx_ring_size,
			  priv->tx_queue[0]->tx_bd_base,
			  priv->tx_queue[0]->tx_bd_dma_base);
>>>>>>> refs/remotes/origin/master
}

void gfar_start(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct gfar __iomem *regs = priv->gfargrp[0].regs;
	u32 tempval;
	int i = 0;

	/* Enable Rx and Tx in MACCFG1 */
	tempval = gfar_read(&regs->maccfg1);
	tempval |= (MACCFG1_RX_EN | MACCFG1_TX_EN);
	gfar_write(&regs->maccfg1, tempval);

	/* Initialize DMACTRL to have WWR and WOP */
	tempval = gfar_read(&regs->dmactrl);
	tempval |= DMACTRL_INIT_SETTINGS;
	gfar_write(&regs->dmactrl, tempval);

	/* Make sure we aren't stopped */
	tempval = gfar_read(&regs->dmactrl);
	tempval &= ~(DMACTRL_GRS | DMACTRL_GTS);
	gfar_write(&regs->dmactrl, tempval);

	for (i = 0; i < priv->num_grps; i++) {
		regs = priv->gfargrp[i].regs;
		/* Clear THLT/RHLT, so that the DMA starts polling now */
		gfar_write(&regs->tstat, priv->gfargrp[i].tstat);
		gfar_write(&regs->rstat, priv->gfargrp[i].rstat);
		/* Unmask the interrupts we look for */
		gfar_write(&regs->imask, IMASK_DEFAULT);
	}

	dev->trans_start = jiffies; /* prevent tx timeout */
}

<<<<<<< HEAD
void gfar_configure_coalescing(struct gfar_private *priv,
	unsigned long tx_mask, unsigned long rx_mask)
{
	struct gfar __iomem *regs = priv->gfargrp[0].regs;
	u32 __iomem *baddr;
	int i = 0;

	/* Backward compatible case ---- even if we enable
	 * multiple queues, there's only single reg to program
	 */
	gfar_write(&regs->txic, 0);
	if(likely(priv->tx_queue[0]->txcoalescing))
		gfar_write(&regs->txic, priv->tx_queue[0]->txic);

	gfar_write(&regs->rxic, 0);
	if(unlikely(priv->rx_queue[0]->rxcoalescing))
		gfar_write(&regs->rxic, priv->rx_queue[0]->rxic);

	if (priv->mode == MQ_MG_MODE) {
		baddr = &regs->txic0;
		for_each_set_bit(i, &tx_mask, priv->num_tx_queues) {
			if (likely(priv->tx_queue[i]->txcoalescing)) {
				gfar_write(baddr + i, 0);
				gfar_write(baddr + i, priv->tx_queue[i]->txic);
			}
=======
static void gfar_configure_coalescing(struct gfar_private *priv,
			       unsigned long tx_mask, unsigned long rx_mask)
{
	struct gfar __iomem *regs = priv->gfargrp[0].regs;
	u32 __iomem *baddr;

	if (priv->mode == MQ_MG_MODE) {
		int i = 0;

		baddr = &regs->txic0;
		for_each_set_bit(i, &tx_mask, priv->num_tx_queues) {
			gfar_write(baddr + i, 0);
			if (likely(priv->tx_queue[i]->txcoalescing))
				gfar_write(baddr + i, priv->tx_queue[i]->txic);
>>>>>>> refs/remotes/origin/master
		}

		baddr = &regs->rxic0;
		for_each_set_bit(i, &rx_mask, priv->num_rx_queues) {
<<<<<<< HEAD
			if (likely(priv->rx_queue[i]->rxcoalescing)) {
				gfar_write(baddr + i, 0);
				gfar_write(baddr + i, priv->rx_queue[i]->rxic);
			}
		}
	}
}

=======
			gfar_write(baddr + i, 0);
			if (likely(priv->rx_queue[i]->rxcoalescing))
				gfar_write(baddr + i, priv->rx_queue[i]->rxic);
		}
	} else {
		/* Backward compatible case -- even if we enable
		 * multiple queues, there's only single reg to program
		 */
		gfar_write(&regs->txic, 0);
		if (likely(priv->tx_queue[0]->txcoalescing))
			gfar_write(&regs->txic, priv->tx_queue[0]->txic);

		gfar_write(&regs->rxic, 0);
		if (unlikely(priv->rx_queue[0]->rxcoalescing))
			gfar_write(&regs->rxic, priv->rx_queue[0]->rxic);
	}
}

void gfar_configure_coalescing_all(struct gfar_private *priv)
{
	gfar_configure_coalescing(priv, 0xFF, 0xFF);
}

>>>>>>> refs/remotes/origin/master
static int register_grp_irqs(struct gfar_priv_grp *grp)
{
	struct gfar_private *priv = grp->priv;
	struct net_device *dev = priv->ndev;
	int err;

	/* If the device has multiple interrupts, register for
<<<<<<< HEAD
	 * them.  Otherwise, only register for the one */
	if (priv->device_flags & FSL_GIANFAR_DEV_HAS_MULTI_INTR) {
		/* Install our interrupt handlers for Error,
		 * Transmit, and Receive */
		if ((err = request_irq(grp->interruptError, gfar_error, 0,
				grp->int_name_er,grp)) < 0) {
			netif_err(priv, intr, dev, "Can't get IRQ %d\n",
				  grp->interruptError);

			goto err_irq_fail;
		}

		if ((err = request_irq(grp->interruptTransmit, gfar_transmit,
				0, grp->int_name_tx, grp)) < 0) {
			netif_err(priv, intr, dev, "Can't get IRQ %d\n",
				  grp->interruptTransmit);
			goto tx_irq_fail;
		}

		if ((err = request_irq(grp->interruptReceive, gfar_receive, 0,
				grp->int_name_rx, grp)) < 0) {
			netif_err(priv, intr, dev, "Can't get IRQ %d\n",
				  grp->interruptReceive);
			goto rx_irq_fail;
		}
	} else {
		if ((err = request_irq(grp->interruptTransmit, gfar_interrupt, 0,
				grp->int_name_tx, grp)) < 0) {
			netif_err(priv, intr, dev, "Can't get IRQ %d\n",
				  grp->interruptTransmit);
=======
	 * them.  Otherwise, only register for the one
	 */
	if (priv->device_flags & FSL_GIANFAR_DEV_HAS_MULTI_INTR) {
		/* Install our interrupt handlers for Error,
		 * Transmit, and Receive
		 */
		err = request_irq(gfar_irq(grp, ER)->irq, gfar_error, 0,
				  gfar_irq(grp, ER)->name, grp);
		if (err < 0) {
			netif_err(priv, intr, dev, "Can't get IRQ %d\n",
				  gfar_irq(grp, ER)->irq);

			goto err_irq_fail;
		}
		err = request_irq(gfar_irq(grp, TX)->irq, gfar_transmit, 0,
				  gfar_irq(grp, TX)->name, grp);
		if (err < 0) {
			netif_err(priv, intr, dev, "Can't get IRQ %d\n",
				  gfar_irq(grp, TX)->irq);
			goto tx_irq_fail;
		}
		err = request_irq(gfar_irq(grp, RX)->irq, gfar_receive, 0,
				  gfar_irq(grp, RX)->name, grp);
		if (err < 0) {
			netif_err(priv, intr, dev, "Can't get IRQ %d\n",
				  gfar_irq(grp, RX)->irq);
			goto rx_irq_fail;
		}
	} else {
		err = request_irq(gfar_irq(grp, TX)->irq, gfar_interrupt, 0,
				  gfar_irq(grp, TX)->name, grp);
		if (err < 0) {
			netif_err(priv, intr, dev, "Can't get IRQ %d\n",
				  gfar_irq(grp, TX)->irq);
>>>>>>> refs/remotes/origin/master
			goto err_irq_fail;
		}
	}

	return 0;

rx_irq_fail:
<<<<<<< HEAD
	free_irq(grp->interruptTransmit, grp);
tx_irq_fail:
	free_irq(grp->interruptError, grp);
=======
	free_irq(gfar_irq(grp, TX)->irq, grp);
tx_irq_fail:
	free_irq(gfar_irq(grp, ER)->irq, grp);
>>>>>>> refs/remotes/origin/master
err_irq_fail:
	return err;

}

/* Bring the controller up and running */
int startup_gfar(struct net_device *ndev)
{
	struct gfar_private *priv = netdev_priv(ndev);
	struct gfar __iomem *regs = NULL;
	int err, i, j;

	for (i = 0; i < priv->num_grps; i++) {
		regs= priv->gfargrp[i].regs;
		gfar_write(&regs->imask, IMASK_INIT_CLEAR);
	}

	regs= priv->gfargrp[0].regs;
	err = gfar_alloc_skb_resources(ndev);
	if (err)
		return err;

	gfar_init_mac(ndev);

	for (i = 0; i < priv->num_grps; i++) {
		err = register_grp_irqs(&priv->gfargrp[i]);
		if (err) {
			for (j = 0; j < i; j++)
				free_grp_irqs(&priv->gfargrp[j]);
			goto irq_fail;
		}
	}

	/* Start the controller */
	gfar_start(ndev);

	phy_start(priv->phydev);

<<<<<<< HEAD
	gfar_configure_coalescing(priv, 0xFF, 0xFF);
=======
	gfar_configure_coalescing_all(priv);
>>>>>>> refs/remotes/origin/master

	return 0;

irq_fail:
	free_skb_resources(priv);
	return err;
}

<<<<<<< HEAD
/* Called when something needs to use the ethernet device */
/* Returns 0 for success. */
=======
/* Called when something needs to use the ethernet device
 * Returns 0 for success.
 */
>>>>>>> refs/remotes/origin/master
static int gfar_enet_open(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	int err;

	enable_napi(priv);

<<<<<<< HEAD
	skb_queue_head_init(&priv->rx_recycle);

=======
>>>>>>> refs/remotes/origin/master
	/* Initialize a bunch of registers */
	init_registers(dev);

	gfar_set_mac_address(dev);

	err = init_phy(dev);

	if (err) {
		disable_napi(priv);
		return err;
	}

	err = startup_gfar(dev);
	if (err) {
		disable_napi(priv);
		return err;
	}

	netif_tx_start_all_queues(dev);

	device_set_wakeup_enable(&dev->dev, priv->wol_en);

	return err;
}

static inline struct txfcb *gfar_add_fcb(struct sk_buff *skb)
{
	struct txfcb *fcb = (struct txfcb *)skb_push(skb, GMAC_FCB_LEN);

	memset(fcb, 0, GMAC_FCB_LEN);

	return fcb;
}

static inline void gfar_tx_checksum(struct sk_buff *skb, struct txfcb *fcb,
<<<<<<< HEAD
		int fcb_length)
{
	u8 flags = 0;

=======
				    int fcb_length)
{
>>>>>>> refs/remotes/origin/master
	/* If we're here, it's a IP packet with a TCP or UDP
	 * payload.  We set it to checksum, using a pseudo-header
	 * we provide
	 */
<<<<<<< HEAD
	flags = TXFCB_DEFAULT;

	/* Tell the controller what the protocol is */
	/* And provide the already calculated phcs */
=======
	u8 flags = TXFCB_DEFAULT;

	/* Tell the controller what the protocol is
	 * And provide the already calculated phcs
	 */
>>>>>>> refs/remotes/origin/master
	if (ip_hdr(skb)->protocol == IPPROTO_UDP) {
		flags |= TXFCB_UDP;
		fcb->phcs = udp_hdr(skb)->check;
	} else
		fcb->phcs = tcp_hdr(skb)->check;

	/* l3os is the distance between the start of the
	 * frame (skb->data) and the start of the IP hdr.
	 * l4os is the distance between the start of the
<<<<<<< HEAD
	 * l3 hdr and the l4 hdr */
=======
	 * l3 hdr and the l4 hdr
	 */
>>>>>>> refs/remotes/origin/master
	fcb->l3os = (u16)(skb_network_offset(skb) - fcb_length);
	fcb->l4os = skb_network_header_len(skb);

	fcb->flags = flags;
}

void inline gfar_tx_vlan(struct sk_buff *skb, struct txfcb *fcb)
{
	fcb->flags |= TXFCB_VLN;
	fcb->vlctl = vlan_tx_tag_get(skb);
}

static inline struct txbd8 *skip_txbd(struct txbd8 *bdp, int stride,
<<<<<<< HEAD
			       struct txbd8 *base, int ring_size)
=======
				      struct txbd8 *base, int ring_size)
>>>>>>> refs/remotes/origin/master
{
	struct txbd8 *new_bd = bdp + stride;

	return (new_bd >= (base + ring_size)) ? (new_bd - ring_size) : new_bd;
}

static inline struct txbd8 *next_txbd(struct txbd8 *bdp, struct txbd8 *base,
<<<<<<< HEAD
		int ring_size)
=======
				      int ring_size)
>>>>>>> refs/remotes/origin/master
{
	return skip_txbd(bdp, 1, base, ring_size);
}

<<<<<<< HEAD
/* This is called by the kernel when a frame is ready for transmission. */
/* It is pointed to by the dev->hard_start_xmit function pointer */
=======
/* eTSEC12: csum generation not supported for some fcb offsets */
static inline bool gfar_csum_errata_12(struct gfar_private *priv,
				       unsigned long fcb_addr)
{
	return (gfar_has_errata(priv, GFAR_ERRATA_12) &&
	       (fcb_addr % 0x20) > 0x18);
}

/* eTSEC76: csum generation for frames larger than 2500 may
 * cause excess delays before start of transmission
 */
static inline bool gfar_csum_errata_76(struct gfar_private *priv,
				       unsigned int len)
{
	return (gfar_has_errata(priv, GFAR_ERRATA_76) &&
	       (len > 2500));
}

/* This is called by the kernel when a frame is ready for transmission.
 * It is pointed to by the dev->hard_start_xmit function pointer
 */
>>>>>>> refs/remotes/origin/master
static int gfar_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct gfar_priv_tx_q *tx_queue = NULL;
	struct netdev_queue *txq;
	struct gfar __iomem *regs = NULL;
	struct txfcb *fcb = NULL;
	struct txbd8 *txbdp, *txbdp_start, *base, *txbdp_tstamp = NULL;
	u32 lstatus;
<<<<<<< HEAD
	int i, rq = 0, do_tstamp = 0;
	u32 bufaddr;
	unsigned long flags;
	unsigned int nr_frags, nr_txbds, length, fcb_length = GMAC_FCB_LEN;

	/*
	 * TOE=1 frames larger than 2500 bytes may see excess delays
	 * before start of transmission.
	 */
	if (unlikely(gfar_has_errata(priv, GFAR_ERRATA_76) &&
			skb->ip_summed == CHECKSUM_PARTIAL &&
			skb->len > 2500)) {
		int ret;

		ret = skb_checksum_help(skb);
		if (ret)
			return ret;
	}
=======
	int i, rq = 0;
	int do_tstamp, do_csum, do_vlan;
	u32 bufaddr;
	unsigned long flags;
	unsigned int nr_frags, nr_txbds, bytes_sent, fcb_len = 0;
>>>>>>> refs/remotes/origin/master

	rq = skb->queue_mapping;
	tx_queue = priv->tx_queue[rq];
	txq = netdev_get_tx_queue(dev, rq);
	base = tx_queue->tx_bd_base;
	regs = tx_queue->grp->regs;

<<<<<<< HEAD
	/* check if time stamp should be generated */
	if (unlikely(skb_shinfo(skb)->tx_flags & SKBTX_HW_TSTAMP &&
			priv->hwts_tx_en)) {
		do_tstamp = 1;
		fcb_length = GMAC_FCB_LEN + GMAC_TXPAL_LEN;
	}

	/* make space for additional header when fcb is needed */
	if (((skb->ip_summed == CHECKSUM_PARTIAL) ||
			vlan_tx_tag_present(skb) ||
			unlikely(do_tstamp)) &&
			(skb_headroom(skb) < fcb_length)) {
		struct sk_buff *skb_new;

		skb_new = skb_realloc_headroom(skb, fcb_length);
=======
	do_csum = (CHECKSUM_PARTIAL == skb->ip_summed);
	do_vlan = vlan_tx_tag_present(skb);
	do_tstamp = (skb_shinfo(skb)->tx_flags & SKBTX_HW_TSTAMP) &&
		    priv->hwts_tx_en;

	if (do_csum || do_vlan)
		fcb_len = GMAC_FCB_LEN;

	/* check if time stamp should be generated */
	if (unlikely(do_tstamp))
		fcb_len = GMAC_FCB_LEN + GMAC_TXPAL_LEN;

	/* make space for additional header when fcb is needed */
	if (fcb_len && unlikely(skb_headroom(skb) < fcb_len)) {
		struct sk_buff *skb_new;

		skb_new = skb_realloc_headroom(skb, fcb_len);
>>>>>>> refs/remotes/origin/master
		if (!skb_new) {
			dev->stats.tx_errors++;
			kfree_skb(skb);
			return NETDEV_TX_OK;
		}

		if (skb->sk)
			skb_set_owner_w(skb_new, skb->sk);
		consume_skb(skb);
		skb = skb_new;
	}

	/* total number of fragments in the SKB */
	nr_frags = skb_shinfo(skb)->nr_frags;

	/* calculate the required number of TxBDs for this skb */
	if (unlikely(do_tstamp))
		nr_txbds = nr_frags + 2;
	else
		nr_txbds = nr_frags + 1;

	/* check if there is space to queue this packet */
	if (nr_txbds > tx_queue->num_txbdfree) {
		/* no space, stop the queue */
		netif_tx_stop_queue(txq);
		dev->stats.tx_fifo_errors++;
		return NETDEV_TX_BUSY;
	}

	/* Update transmit stats */
<<<<<<< HEAD
	tx_queue->stats.tx_bytes += skb->len;
=======
	bytes_sent = skb->len;
	tx_queue->stats.tx_bytes += bytes_sent;
	/* keep Tx bytes on wire for BQL accounting */
	GFAR_CB(skb)->bytes_sent = bytes_sent;
>>>>>>> refs/remotes/origin/master
	tx_queue->stats.tx_packets++;

	txbdp = txbdp_start = tx_queue->cur_tx;
	lstatus = txbdp->lstatus;

	/* Time stamp insertion requires one additional TxBD */
	if (unlikely(do_tstamp))
		txbdp_tstamp = txbdp = next_txbd(txbdp, base,
<<<<<<< HEAD
				tx_queue->tx_ring_size);
=======
						 tx_queue->tx_ring_size);
>>>>>>> refs/remotes/origin/master

	if (nr_frags == 0) {
		if (unlikely(do_tstamp))
			txbdp_tstamp->lstatus |= BD_LFLAG(TXBD_LAST |
<<<<<<< HEAD
					TXBD_INTERRUPT);
=======
							  TXBD_INTERRUPT);
>>>>>>> refs/remotes/origin/master
		else
			lstatus |= BD_LFLAG(TXBD_LAST | TXBD_INTERRUPT);
	} else {
		/* Place the fragment addresses and lengths into the TxBDs */
		for (i = 0; i < nr_frags; i++) {
<<<<<<< HEAD
			/* Point at the next BD, wrapping as needed */
			txbdp = next_txbd(txbdp, base, tx_queue->tx_ring_size);

			length = skb_shinfo(skb)->frags[i].size;

			lstatus = txbdp->lstatus | length |
				BD_LFLAG(TXBD_READY);
=======
			unsigned int frag_len;
			/* Point at the next BD, wrapping as needed */
			txbdp = next_txbd(txbdp, base, tx_queue->tx_ring_size);

			frag_len = skb_shinfo(skb)->frags[i].size;

			lstatus = txbdp->lstatus | frag_len |
				  BD_LFLAG(TXBD_READY);
>>>>>>> refs/remotes/origin/master

			/* Handle the last BD specially */
			if (i == nr_frags - 1)
				lstatus |= BD_LFLAG(TXBD_LAST | TXBD_INTERRUPT);

<<<<<<< HEAD
			bufaddr = skb_frag_dma_map(&priv->ofdev->dev,
						   &skb_shinfo(skb)->frags[i],
						   0,
						   length,
=======
			bufaddr = skb_frag_dma_map(priv->dev,
						   &skb_shinfo(skb)->frags[i],
						   0,
						   frag_len,
>>>>>>> refs/remotes/origin/master
						   DMA_TO_DEVICE);

			/* set the TxBD length and buffer pointer */
			txbdp->bufPtr = bufaddr;
			txbdp->lstatus = lstatus;
		}

		lstatus = txbdp_start->lstatus;
	}

	/* Add TxPAL between FCB and frame if required */
	if (unlikely(do_tstamp)) {
		skb_push(skb, GMAC_TXPAL_LEN);
		memset(skb->data, 0, GMAC_TXPAL_LEN);
	}

<<<<<<< HEAD
	/* Set up checksumming */
	if (CHECKSUM_PARTIAL == skb->ip_summed) {
		fcb = gfar_add_fcb(skb);
		/* as specified by errata */
		if (unlikely(gfar_has_errata(priv, GFAR_ERRATA_12)
			     && ((unsigned long)fcb % 0x20) > 0x18)) {
			__skb_pull(skb, GMAC_FCB_LEN);
			skb_checksum_help(skb);
		} else {
			lstatus |= BD_LFLAG(TXBD_TOE);
			gfar_tx_checksum(skb, fcb, fcb_length);
		}
	}

	if (vlan_tx_tag_present(skb)) {
		if (unlikely(NULL == fcb)) {
			fcb = gfar_add_fcb(skb);
			lstatus |= BD_LFLAG(TXBD_TOE);
		}

		gfar_tx_vlan(skb, fcb);
	}
=======
	/* Add TxFCB if required */
	if (fcb_len) {
		fcb = gfar_add_fcb(skb);
		lstatus |= BD_LFLAG(TXBD_TOE);
	}

	/* Set up checksumming */
	if (do_csum) {
		gfar_tx_checksum(skb, fcb, fcb_len);

		if (unlikely(gfar_csum_errata_12(priv, (unsigned long)fcb)) ||
		    unlikely(gfar_csum_errata_76(priv, skb->len))) {
			__skb_pull(skb, GMAC_FCB_LEN);
			skb_checksum_help(skb);
			if (do_vlan || do_tstamp) {
				/* put back a new fcb for vlan/tstamp TOE */
				fcb = gfar_add_fcb(skb);
			} else {
				/* Tx TOE not used */
				lstatus &= ~(BD_LFLAG(TXBD_TOE));
				fcb = NULL;
			}
		}
	}

	if (do_vlan)
		gfar_tx_vlan(skb, fcb);
>>>>>>> refs/remotes/origin/master

	/* Setup tx hardware time stamping if requested */
	if (unlikely(do_tstamp)) {
		skb_shinfo(skb)->tx_flags |= SKBTX_IN_PROGRESS;
<<<<<<< HEAD
		if (fcb == NULL)
			fcb = gfar_add_fcb(skb);
		fcb->ptp = 1;
		lstatus |= BD_LFLAG(TXBD_TOE);
	}

	txbdp_start->bufPtr = dma_map_single(&priv->ofdev->dev, skb->data,
			skb_headlen(skb), DMA_TO_DEVICE);

	/*
	 * If time stamping is requested one additional TxBD must be set up. The
=======
		fcb->ptp = 1;
	}

	txbdp_start->bufPtr = dma_map_single(priv->dev, skb->data,
					     skb_headlen(skb), DMA_TO_DEVICE);

	/* If time stamping is requested one additional TxBD must be set up. The
>>>>>>> refs/remotes/origin/master
	 * first TxBD points to the FCB and must have a data length of
	 * GMAC_FCB_LEN. The second TxBD points to the actual frame data with
	 * the full frame length.
	 */
	if (unlikely(do_tstamp)) {
<<<<<<< HEAD
		txbdp_tstamp->bufPtr = txbdp_start->bufPtr + fcb_length;
		txbdp_tstamp->lstatus |= BD_LFLAG(TXBD_READY) |
				(skb_headlen(skb) - fcb_length);
=======
		txbdp_tstamp->bufPtr = txbdp_start->bufPtr + fcb_len;
		txbdp_tstamp->lstatus |= BD_LFLAG(TXBD_READY) |
					 (skb_headlen(skb) - fcb_len);
>>>>>>> refs/remotes/origin/master
		lstatus |= BD_LFLAG(TXBD_CRC | TXBD_READY) | GMAC_FCB_LEN;
	} else {
		lstatus |= BD_LFLAG(TXBD_CRC | TXBD_READY) | skb_headlen(skb);
	}

<<<<<<< HEAD
	netdev_tx_sent_queue(txq, skb->len);

	/*
	 * We can work in parallel with gfar_clean_tx_ring(), except
=======
	netdev_tx_sent_queue(txq, bytes_sent);

	/* We can work in parallel with gfar_clean_tx_ring(), except
>>>>>>> refs/remotes/origin/master
	 * when modifying num_txbdfree. Note that we didn't grab the lock
	 * when we were reading the num_txbdfree and checking for available
	 * space, that's because outside of this function it can only grow,
	 * and once we've got needed space, it cannot suddenly disappear.
	 *
	 * The lock also protects us from gfar_error(), which can modify
	 * regs->tstat and thus retrigger the transfers, which is why we
	 * also must grab the lock before setting ready bit for the first
	 * to be transmitted BD.
	 */
	spin_lock_irqsave(&tx_queue->txlock, flags);

<<<<<<< HEAD
	/*
	 * The powerpc-specific eieio() is used, as wmb() has too strong
=======
	/* The powerpc-specific eieio() is used, as wmb() has too strong
>>>>>>> refs/remotes/origin/master
	 * semantics (it requires synchronization between cacheable and
	 * uncacheable mappings, which eieio doesn't provide and which we
	 * don't need), thus requiring a more expensive sync instruction.  At
	 * some point, the set of architecture-independent barrier functions
	 * should be expanded to include weaker barriers.
	 */
	eieio();

	txbdp_start->lstatus = lstatus;

	eieio(); /* force lstatus write before tx_skbuff */

	tx_queue->tx_skbuff[tx_queue->skb_curtx] = skb;

	/* Update the current skb pointer to the next entry we will use
<<<<<<< HEAD
	 * (wrapping if necessary) */
	tx_queue->skb_curtx = (tx_queue->skb_curtx + 1) &
		TX_RING_MOD_MASK(tx_queue->tx_ring_size);
=======
	 * (wrapping if necessary)
	 */
	tx_queue->skb_curtx = (tx_queue->skb_curtx + 1) &
			      TX_RING_MOD_MASK(tx_queue->tx_ring_size);
>>>>>>> refs/remotes/origin/master

	tx_queue->cur_tx = next_txbd(txbdp, base, tx_queue->tx_ring_size);

	/* reduce TxBD free count */
	tx_queue->num_txbdfree -= (nr_txbds);

	/* If the next BD still needs to be cleaned up, then the bds
<<<<<<< HEAD
	   are full.  We need to tell the kernel to stop sending us stuff. */
=======
	 * are full.  We need to tell the kernel to stop sending us stuff.
	 */
>>>>>>> refs/remotes/origin/master
	if (!tx_queue->num_txbdfree) {
		netif_tx_stop_queue(txq);

		dev->stats.tx_fifo_errors++;
	}

	/* Tell the DMA to go go go */
	gfar_write(&regs->tstat, TSTAT_CLEAR_THALT >> tx_queue->qindex);

	/* Unlock priv */
	spin_unlock_irqrestore(&tx_queue->txlock, flags);

	return NETDEV_TX_OK;
}

/* Stops the kernel queue, and halts the controller */
static int gfar_close(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);

	disable_napi(priv);

	cancel_work_sync(&priv->reset_task);
	stop_gfar(dev);

	/* Disconnect from the PHY */
	phy_disconnect(priv->phydev);
	priv->phydev = NULL;

	netif_tx_stop_all_queues(dev);

	return 0;
}

/* Changes the mac address if the controller is not running. */
static int gfar_set_mac_address(struct net_device *dev)
{
	gfar_set_mac_for_addr(dev, 0, dev->dev_addr);

	return 0;
}

/* Check if rx parser should be activated */
void gfar_check_rx_parser_mode(struct gfar_private *priv)
{
	struct gfar __iomem *regs;
	u32 tempval;

	regs = priv->gfargrp[0].regs;

	tempval = gfar_read(&regs->rctrl);
	/* If parse is no longer required, then disable parser */
<<<<<<< HEAD
	if (tempval & RCTRL_REQ_PARSER)
		tempval |= RCTRL_PRSDEP_INIT;
	else
		tempval &= ~RCTRL_PRSDEP_INIT;
=======
	if (tempval & RCTRL_REQ_PARSER) {
		tempval |= RCTRL_PRSDEP_INIT;
		priv->uses_rxfcb = 1;
	} else {
		tempval &= ~RCTRL_PRSDEP_INIT;
		priv->uses_rxfcb = 0;
	}
>>>>>>> refs/remotes/origin/master
	gfar_write(&regs->rctrl, tempval);
}

/* Enables and disables VLAN insertion/extraction */
void gfar_vlan_mode(struct net_device *dev, netdev_features_t features)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct gfar __iomem *regs = NULL;
	unsigned long flags;
	u32 tempval;

	regs = priv->gfargrp[0].regs;
	local_irq_save(flags);
	lock_rx_qs(priv);

<<<<<<< HEAD
	if (features & NETIF_F_HW_VLAN_TX) {
=======
	if (features & NETIF_F_HW_VLAN_CTAG_TX) {
>>>>>>> refs/remotes/origin/master
		/* Enable VLAN tag insertion */
		tempval = gfar_read(&regs->tctrl);
		tempval |= TCTRL_VLINS;
		gfar_write(&regs->tctrl, tempval);
	} else {
		/* Disable VLAN tag insertion */
		tempval = gfar_read(&regs->tctrl);
		tempval &= ~TCTRL_VLINS;
		gfar_write(&regs->tctrl, tempval);
	}

<<<<<<< HEAD
	if (features & NETIF_F_HW_VLAN_RX) {
=======
	if (features & NETIF_F_HW_VLAN_CTAG_RX) {
>>>>>>> refs/remotes/origin/master
		/* Enable VLAN tag extraction */
		tempval = gfar_read(&regs->rctrl);
		tempval |= (RCTRL_VLEX | RCTRL_PRSDEP_INIT);
		gfar_write(&regs->rctrl, tempval);
<<<<<<< HEAD
=======
		priv->uses_rxfcb = 1;
>>>>>>> refs/remotes/origin/master
	} else {
		/* Disable VLAN tag extraction */
		tempval = gfar_read(&regs->rctrl);
		tempval &= ~RCTRL_VLEX;
		gfar_write(&regs->rctrl, tempval);

		gfar_check_rx_parser_mode(priv);
	}

	gfar_change_mtu(dev, dev->mtu);

	unlock_rx_qs(priv);
	local_irq_restore(flags);
}

static int gfar_change_mtu(struct net_device *dev, int new_mtu)
{
	int tempsize, tempval;
	struct gfar_private *priv = netdev_priv(dev);
	struct gfar __iomem *regs = priv->gfargrp[0].regs;
	int oldsize = priv->rx_buffer_size;
	int frame_size = new_mtu + ETH_HLEN;

<<<<<<< HEAD
	if (gfar_is_vlan_on(priv))
		frame_size += VLAN_HLEN;

=======
>>>>>>> refs/remotes/origin/master
	if ((frame_size < 64) || (frame_size > JUMBO_FRAME_SIZE)) {
		netif_err(priv, drv, dev, "Invalid MTU setting\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	if (gfar_uses_fcb(priv))
=======
	if (priv->uses_rxfcb)
>>>>>>> refs/remotes/origin/master
		frame_size += GMAC_FCB_LEN;

	frame_size += priv->padding;

<<<<<<< HEAD
	tempsize =
	    (frame_size & ~(INCREMENTAL_BUFFER_SIZE - 1)) +
	    INCREMENTAL_BUFFER_SIZE;

	/* Only stop and start the controller if it isn't already
	 * stopped, and we changed something */
=======
	tempsize = (frame_size & ~(INCREMENTAL_BUFFER_SIZE - 1)) +
		   INCREMENTAL_BUFFER_SIZE;

	/* Only stop and start the controller if it isn't already
	 * stopped, and we changed something
	 */
>>>>>>> refs/remotes/origin/master
	if ((oldsize != tempsize) && (dev->flags & IFF_UP))
		stop_gfar(dev);

	priv->rx_buffer_size = tempsize;

	dev->mtu = new_mtu;

	gfar_write(&regs->mrblr, priv->rx_buffer_size);
	gfar_write(&regs->maxfrm, priv->rx_buffer_size);

	/* If the mtu is larger than the max size for standard
	 * ethernet frames (ie, a jumbo frame), then set maccfg2
<<<<<<< HEAD
	 * to allow huge frames, and to check the length */
	tempval = gfar_read(&regs->maccfg2);

	if (priv->rx_buffer_size > DEFAULT_RX_BUFFER_SIZE ||
			gfar_has_errata(priv, GFAR_ERRATA_74))
=======
	 * to allow huge frames, and to check the length
	 */
	tempval = gfar_read(&regs->maccfg2);

	if (priv->rx_buffer_size > DEFAULT_RX_BUFFER_SIZE ||
	    gfar_has_errata(priv, GFAR_ERRATA_74))
>>>>>>> refs/remotes/origin/master
		tempval |= (MACCFG2_HUGEFRAME | MACCFG2_LENGTHCHECK);
	else
		tempval &= ~(MACCFG2_HUGEFRAME | MACCFG2_LENGTHCHECK);

	gfar_write(&regs->maccfg2, tempval);

	if ((oldsize != tempsize) && (dev->flags & IFF_UP))
		startup_gfar(dev);

	return 0;
}

/* gfar_reset_task gets scheduled when a packet has not been
 * transmitted after a set amount of time.
 * For now, assume that clearing out all the structures, and
 * starting over will fix the problem.
 */
static void gfar_reset_task(struct work_struct *work)
{
	struct gfar_private *priv = container_of(work, struct gfar_private,
<<<<<<< HEAD
			reset_task);
=======
						 reset_task);
>>>>>>> refs/remotes/origin/master
	struct net_device *dev = priv->ndev;

	if (dev->flags & IFF_UP) {
		netif_tx_stop_all_queues(dev);
		stop_gfar(dev);
		startup_gfar(dev);
		netif_tx_start_all_queues(dev);
	}

	netif_tx_schedule_all(dev);
}

static void gfar_timeout(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);

	dev->stats.tx_errors++;
	schedule_work(&priv->reset_task);
}

static void gfar_align_skb(struct sk_buff *skb)
{
	/* We need the data buffer to be aligned properly.  We will reserve
	 * as many bytes as needed to align the data properly
	 */
	skb_reserve(skb, RXBUF_ALIGNMENT -
<<<<<<< HEAD
		(((unsigned long) skb->data) & (RXBUF_ALIGNMENT - 1)));
}

/* Interrupt Handler for Transmit complete */
static int gfar_clean_tx_ring(struct gfar_priv_tx_q *tx_queue)
=======
		    (((unsigned long) skb->data) & (RXBUF_ALIGNMENT - 1)));
}

/* Interrupt Handler for Transmit complete */
static void gfar_clean_tx_ring(struct gfar_priv_tx_q *tx_queue)
>>>>>>> refs/remotes/origin/master
{
	struct net_device *dev = tx_queue->dev;
	struct netdev_queue *txq;
	struct gfar_private *priv = netdev_priv(dev);
<<<<<<< HEAD
	struct gfar_priv_rx_q *rx_queue = NULL;
=======
>>>>>>> refs/remotes/origin/master
	struct txbd8 *bdp, *next = NULL;
	struct txbd8 *lbdp = NULL;
	struct txbd8 *base = tx_queue->tx_bd_base;
	struct sk_buff *skb;
	int skb_dirtytx;
	int tx_ring_size = tx_queue->tx_ring_size;
	int frags = 0, nr_txbds = 0;
	int i;
	int howmany = 0;
	int tqi = tx_queue->qindex;
	unsigned int bytes_sent = 0;
	u32 lstatus;
	size_t buflen;

<<<<<<< HEAD
	rx_queue = priv->rx_queue[tqi];
=======
>>>>>>> refs/remotes/origin/master
	txq = netdev_get_tx_queue(dev, tqi);
	bdp = tx_queue->dirty_tx;
	skb_dirtytx = tx_queue->skb_dirtytx;

	while ((skb = tx_queue->tx_skbuff[skb_dirtytx])) {
		unsigned long flags;

		frags = skb_shinfo(skb)->nr_frags;

<<<<<<< HEAD
		/*
		 * When time stamping, one additional TxBD must be freed.
=======
		/* When time stamping, one additional TxBD must be freed.
>>>>>>> refs/remotes/origin/master
		 * Also, we need to dma_unmap_single() the TxPAL.
		 */
		if (unlikely(skb_shinfo(skb)->tx_flags & SKBTX_IN_PROGRESS))
			nr_txbds = frags + 2;
		else
			nr_txbds = frags + 1;

		lbdp = skip_txbd(bdp, nr_txbds - 1, base, tx_ring_size);

		lstatus = lbdp->lstatus;

		/* Only clean completed frames */
		if ((lstatus & BD_LFLAG(TXBD_READY)) &&
<<<<<<< HEAD
				(lstatus & BD_LENGTH_MASK))
=======
		    (lstatus & BD_LENGTH_MASK))
>>>>>>> refs/remotes/origin/master
			break;

		if (unlikely(skb_shinfo(skb)->tx_flags & SKBTX_IN_PROGRESS)) {
			next = next_txbd(bdp, base, tx_ring_size);
			buflen = next->length + GMAC_FCB_LEN + GMAC_TXPAL_LEN;
		} else
			buflen = bdp->length;

<<<<<<< HEAD
		dma_unmap_single(&priv->ofdev->dev, bdp->bufPtr,
				buflen, DMA_TO_DEVICE);
=======
		dma_unmap_single(priv->dev, bdp->bufPtr,
				 buflen, DMA_TO_DEVICE);
>>>>>>> refs/remotes/origin/master

		if (unlikely(skb_shinfo(skb)->tx_flags & SKBTX_IN_PROGRESS)) {
			struct skb_shared_hwtstamps shhwtstamps;
			u64 *ns = (u64*) (((u32)skb->data + 0x10) & ~0x7);
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
			memset(&shhwtstamps, 0, sizeof(shhwtstamps));
			shhwtstamps.hwtstamp = ns_to_ktime(*ns);
			skb_pull(skb, GMAC_FCB_LEN + GMAC_TXPAL_LEN);
			skb_tstamp_tx(skb, &shhwtstamps);
			bdp->lstatus &= BD_LFLAG(TXBD_WRAP);
			bdp = next;
		}

		bdp->lstatus &= BD_LFLAG(TXBD_WRAP);
		bdp = next_txbd(bdp, base, tx_ring_size);

		for (i = 0; i < frags; i++) {
<<<<<<< HEAD
			dma_unmap_page(&priv->ofdev->dev,
					bdp->bufPtr,
					bdp->length,
					DMA_TO_DEVICE);
=======
			dma_unmap_page(priv->dev, bdp->bufPtr,
				       bdp->length, DMA_TO_DEVICE);
>>>>>>> refs/remotes/origin/master
			bdp->lstatus &= BD_LFLAG(TXBD_WRAP);
			bdp = next_txbd(bdp, base, tx_ring_size);
		}

<<<<<<< HEAD
		bytes_sent += skb->len;

		/*
		 * If there's room in the queue (limit it to rx_buffer_size)
		 * we add this skb back into the pool, if it's the right size
		 */
		if (skb_queue_len(&priv->rx_recycle) < rx_queue->rx_ring_size &&
				skb_recycle_check(skb, priv->rx_buffer_size +
					RXBUF_ALIGNMENT)) {
			gfar_align_skb(skb);
			skb_queue_head(&priv->rx_recycle, skb);
		} else
			dev_kfree_skb_any(skb);
=======
		bytes_sent += GFAR_CB(skb)->bytes_sent;

		dev_kfree_skb_any(skb);
>>>>>>> refs/remotes/origin/master

		tx_queue->tx_skbuff[skb_dirtytx] = NULL;

		skb_dirtytx = (skb_dirtytx + 1) &
<<<<<<< HEAD
			TX_RING_MOD_MASK(tx_ring_size);
=======
			      TX_RING_MOD_MASK(tx_ring_size);
>>>>>>> refs/remotes/origin/master

		howmany++;
		spin_lock_irqsave(&tx_queue->txlock, flags);
		tx_queue->num_txbdfree += nr_txbds;
		spin_unlock_irqrestore(&tx_queue->txlock, flags);
	}

	/* If we freed a buffer, we can restart transmission, if necessary */
	if (netif_tx_queue_stopped(txq) && tx_queue->num_txbdfree)
		netif_wake_subqueue(dev, tqi);

	/* Update dirty indicators */
	tx_queue->skb_dirtytx = skb_dirtytx;
	tx_queue->dirty_tx = bdp;

	netdev_tx_completed_queue(txq, howmany, bytes_sent);
<<<<<<< HEAD

	return howmany;
=======
>>>>>>> refs/remotes/origin/master
}

static void gfar_schedule_cleanup(struct gfar_priv_grp *gfargrp)
{
	unsigned long flags;

	spin_lock_irqsave(&gfargrp->grplock, flags);
	if (napi_schedule_prep(&gfargrp->napi)) {
		gfar_write(&gfargrp->regs->imask, IMASK_RTX_DISABLED);
		__napi_schedule(&gfargrp->napi);
	} else {
<<<<<<< HEAD
		/*
		 * Clear IEVENT, so interrupts aren't called again
=======
		/* Clear IEVENT, so interrupts aren't called again
>>>>>>> refs/remotes/origin/master
		 * because of the packets that have already arrived.
		 */
		gfar_write(&gfargrp->regs->ievent, IEVENT_RTX_MASK);
	}
	spin_unlock_irqrestore(&gfargrp->grplock, flags);

}

/* Interrupt Handler for Transmit complete */
static irqreturn_t gfar_transmit(int irq, void *grp_id)
{
	gfar_schedule_cleanup((struct gfar_priv_grp *)grp_id);
	return IRQ_HANDLED;
}

static void gfar_new_rxbdp(struct gfar_priv_rx_q *rx_queue, struct rxbd8 *bdp,
<<<<<<< HEAD
		struct sk_buff *skb)
=======
			   struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct net_device *dev = rx_queue->dev;
	struct gfar_private *priv = netdev_priv(dev);
	dma_addr_t buf;

<<<<<<< HEAD
	buf = dma_map_single(&priv->ofdev->dev, skb->data,
=======
	buf = dma_map_single(priv->dev, skb->data,
>>>>>>> refs/remotes/origin/master
			     priv->rx_buffer_size, DMA_FROM_DEVICE);
	gfar_init_rxbdp(rx_queue, bdp, buf);
}

<<<<<<< HEAD
static struct sk_buff * gfar_alloc_skb(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct sk_buff *skb = NULL;
=======
static struct sk_buff *gfar_alloc_skb(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct sk_buff *skb;
>>>>>>> refs/remotes/origin/master

	skb = netdev_alloc_skb(dev, priv->rx_buffer_size + RXBUF_ALIGNMENT);
	if (!skb)
		return NULL;

	gfar_align_skb(skb);

	return skb;
}

<<<<<<< HEAD
struct sk_buff * gfar_new_skb(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct sk_buff *skb = NULL;

	skb = skb_dequeue(&priv->rx_recycle);
	if (!skb)
		skb = gfar_alloc_skb(dev);

	return skb;
=======
struct sk_buff *gfar_new_skb(struct net_device *dev)
{
	return gfar_alloc_skb(dev);
>>>>>>> refs/remotes/origin/master
}

static inline void count_errors(unsigned short status, struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct net_device_stats *stats = &dev->stats;
	struct gfar_extra_stats *estats = &priv->extra_stats;

<<<<<<< HEAD
	/* If the packet was truncated, none of the other errors
	 * matter */
	if (status & RXBD_TRUNCATED) {
		stats->rx_length_errors++;

		estats->rx_trunc++;
=======
	/* If the packet was truncated, none of the other errors matter */
	if (status & RXBD_TRUNCATED) {
		stats->rx_length_errors++;

		atomic64_inc(&estats->rx_trunc);
>>>>>>> refs/remotes/origin/master

		return;
	}
	/* Count the errors, if there were any */
	if (status & (RXBD_LARGE | RXBD_SHORT)) {
		stats->rx_length_errors++;

		if (status & RXBD_LARGE)
<<<<<<< HEAD
			estats->rx_large++;
		else
			estats->rx_short++;
	}
	if (status & RXBD_NONOCTET) {
		stats->rx_frame_errors++;
		estats->rx_nonoctet++;
	}
	if (status & RXBD_CRCERR) {
		estats->rx_crcerr++;
		stats->rx_crc_errors++;
	}
	if (status & RXBD_OVERRUN) {
		estats->rx_overrun++;
=======
			atomic64_inc(&estats->rx_large);
		else
			atomic64_inc(&estats->rx_short);
	}
	if (status & RXBD_NONOCTET) {
		stats->rx_frame_errors++;
		atomic64_inc(&estats->rx_nonoctet);
	}
	if (status & RXBD_CRCERR) {
		atomic64_inc(&estats->rx_crcerr);
		stats->rx_crc_errors++;
	}
	if (status & RXBD_OVERRUN) {
		atomic64_inc(&estats->rx_overrun);
>>>>>>> refs/remotes/origin/master
		stats->rx_crc_errors++;
	}
}

irqreturn_t gfar_receive(int irq, void *grp_id)
{
	gfar_schedule_cleanup((struct gfar_priv_grp *)grp_id);
	return IRQ_HANDLED;
}

static inline void gfar_rx_checksum(struct sk_buff *skb, struct rxfcb *fcb)
{
	/* If valid headers were found, and valid sums
	 * were verified, then we tell the kernel that no
<<<<<<< HEAD
	 * checksumming is necessary.  Otherwise, it is */
=======
	 * checksumming is necessary.  Otherwise, it is [FIXME]
	 */
>>>>>>> refs/remotes/origin/master
	if ((fcb->flags & RXFCB_CSUM_MASK) == (RXFCB_CIP | RXFCB_CTU))
		skb->ip_summed = CHECKSUM_UNNECESSARY;
	else
		skb_checksum_none_assert(skb);
}


<<<<<<< HEAD
/* gfar_process_frame() -- handle one incoming packet if skb
 * isn't NULL.  */
static int gfar_process_frame(struct net_device *dev, struct sk_buff *skb,
			      int amount_pull)
=======
/* gfar_process_frame() -- handle one incoming packet if skb isn't NULL. */
static void gfar_process_frame(struct net_device *dev, struct sk_buff *skb,
			       int amount_pull, struct napi_struct *napi)
>>>>>>> refs/remotes/origin/master
{
	struct gfar_private *priv = netdev_priv(dev);
	struct rxfcb *fcb = NULL;

<<<<<<< HEAD
	int ret;

	/* fcb is at the beginning if exists */
	fcb = (struct rxfcb *)skb->data;

	/* Remove the FCB from the skb */
	/* Remove the padded bytes, if there are any */
=======
	/* fcb is at the beginning if exists */
	fcb = (struct rxfcb *)skb->data;

	/* Remove the FCB from the skb
	 * Remove the padded bytes, if there are any
	 */
>>>>>>> refs/remotes/origin/master
	if (amount_pull) {
		skb_record_rx_queue(skb, fcb->rq);
		skb_pull(skb, amount_pull);
	}

	/* Get receive timestamp from the skb */
	if (priv->hwts_rx_en) {
		struct skb_shared_hwtstamps *shhwtstamps = skb_hwtstamps(skb);
		u64 *ns = (u64 *) skb->data;
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
		memset(shhwtstamps, 0, sizeof(*shhwtstamps));
		shhwtstamps->hwtstamp = ns_to_ktime(*ns);
	}

	if (priv->padding)
		skb_pull(skb, priv->padding);

	if (dev->features & NETIF_F_RXCSUM)
		gfar_rx_checksum(skb, fcb);

	/* Tell the skb what kind of packet this is */
	skb->protocol = eth_type_trans(skb, dev);

<<<<<<< HEAD
	/*
	 * There's need to check for NETIF_F_HW_VLAN_RX here.
	 * Even if vlan rx accel is disabled, on some chips
	 * RXFCB_VLN is pseudo randomly set.
	 */
	if (dev->features & NETIF_F_HW_VLAN_RX &&
	    fcb->flags & RXFCB_VLN)
		__vlan_hwaccel_put_tag(skb, fcb->vlctl);

	/* Send the packet up the stack */
	ret = netif_receive_skb(skb);

	if (NET_RX_DROP == ret)
		priv->extra_stats.kernel_dropped++;

	return 0;
}

/* gfar_clean_rx_ring() -- Processes each frame in the rx ring
 *   until the budget/quota has been reached. Returns the number
 *   of frames handled
=======
	/* There's need to check for NETIF_F_HW_VLAN_CTAG_RX here.
	 * Even if vlan rx accel is disabled, on some chips
	 * RXFCB_VLN is pseudo randomly set.
	 */
	if (dev->features & NETIF_F_HW_VLAN_CTAG_RX &&
	    fcb->flags & RXFCB_VLN)
		__vlan_hwaccel_put_tag(skb, htons(ETH_P_8021Q), fcb->vlctl);

	/* Send the packet up the stack */
	napi_gro_receive(napi, skb);

}

/* gfar_clean_rx_ring() -- Processes each frame in the rx ring
 * until the budget/quota has been reached. Returns the number
 * of frames handled
>>>>>>> refs/remotes/origin/master
 */
int gfar_clean_rx_ring(struct gfar_priv_rx_q *rx_queue, int rx_work_limit)
{
	struct net_device *dev = rx_queue->dev;
	struct rxbd8 *bdp, *base;
	struct sk_buff *skb;
	int pkt_len;
	int amount_pull;
	int howmany = 0;
	struct gfar_private *priv = netdev_priv(dev);

	/* Get the first full descriptor */
	bdp = rx_queue->cur_rx;
	base = rx_queue->rx_bd_base;

<<<<<<< HEAD
	amount_pull = (gfar_uses_fcb(priv) ? GMAC_FCB_LEN : 0);

	while (!((bdp->status & RXBD_EMPTY) || (--rx_work_limit < 0))) {
		struct sk_buff *newskb;
=======
	amount_pull = priv->uses_rxfcb ? GMAC_FCB_LEN : 0;

	while (!((bdp->status & RXBD_EMPTY) || (--rx_work_limit < 0))) {
		struct sk_buff *newskb;

>>>>>>> refs/remotes/origin/master
		rmb();

		/* Add another skb for the future */
		newskb = gfar_new_skb(dev);

		skb = rx_queue->rx_skbuff[rx_queue->skb_currx];

<<<<<<< HEAD
		dma_unmap_single(&priv->ofdev->dev, bdp->bufPtr,
				priv->rx_buffer_size, DMA_FROM_DEVICE);

		if (unlikely(!(bdp->status & RXBD_ERR) &&
				bdp->length > priv->rx_buffer_size))
=======
		dma_unmap_single(priv->dev, bdp->bufPtr,
				 priv->rx_buffer_size, DMA_FROM_DEVICE);

		if (unlikely(!(bdp->status & RXBD_ERR) &&
			     bdp->length > priv->rx_buffer_size))
>>>>>>> refs/remotes/origin/master
			bdp->status = RXBD_LARGE;

		/* We drop the frame if we failed to allocate a new buffer */
		if (unlikely(!newskb || !(bdp->status & RXBD_LAST) ||
<<<<<<< HEAD
				 bdp->status & RXBD_ERR)) {
=======
			     bdp->status & RXBD_ERR)) {
>>>>>>> refs/remotes/origin/master
			count_errors(bdp->status, dev);

			if (unlikely(!newskb))
				newskb = skb;
			else if (skb)
<<<<<<< HEAD
				skb_queue_head(&priv->rx_recycle, skb);
=======
				dev_kfree_skb(skb);
>>>>>>> refs/remotes/origin/master
		} else {
			/* Increment the number of packets */
			rx_queue->stats.rx_packets++;
			howmany++;

			if (likely(skb)) {
				pkt_len = bdp->length - ETH_FCS_LEN;
				/* Remove the FCS from the packet length */
				skb_put(skb, pkt_len);
				rx_queue->stats.rx_bytes += pkt_len;
				skb_record_rx_queue(skb, rx_queue->qindex);
<<<<<<< HEAD
				gfar_process_frame(dev, skb, amount_pull);
=======
				gfar_process_frame(dev, skb, amount_pull,
						   &rx_queue->grp->napi);
>>>>>>> refs/remotes/origin/master

			} else {
				netif_warn(priv, rx_err, dev, "Missing skb!\n");
				rx_queue->stats.rx_dropped++;
<<<<<<< HEAD
				priv->extra_stats.rx_skbmissing++;
=======
				atomic64_inc(&priv->extra_stats.rx_skbmissing);
>>>>>>> refs/remotes/origin/master
			}

		}

		rx_queue->rx_skbuff[rx_queue->skb_currx] = newskb;

		/* Setup the new bdp */
		gfar_new_rxbdp(rx_queue, bdp, newskb);

		/* Update to the next pointer */
		bdp = next_bd(bdp, base, rx_queue->rx_ring_size);

		/* update to point at the next skb */
<<<<<<< HEAD
		rx_queue->skb_currx =
		    (rx_queue->skb_currx + 1) &
		    RX_RING_MOD_MASK(rx_queue->rx_ring_size);
=======
		rx_queue->skb_currx = (rx_queue->skb_currx + 1) &
				      RX_RING_MOD_MASK(rx_queue->rx_ring_size);
>>>>>>> refs/remotes/origin/master
	}

	/* Update the current rxbd pointer to be the next one */
	rx_queue->cur_rx = bdp;

	return howmany;
}

<<<<<<< HEAD
static int gfar_poll(struct napi_struct *napi, int budget)
{
	struct gfar_priv_grp *gfargrp = container_of(napi,
			struct gfar_priv_grp, napi);
=======
static int gfar_poll_sq(struct napi_struct *napi, int budget)
{
	struct gfar_priv_grp *gfargrp =
		container_of(napi, struct gfar_priv_grp, napi);
	struct gfar __iomem *regs = gfargrp->regs;
	struct gfar_priv_tx_q *tx_queue = gfargrp->priv->tx_queue[0];
	struct gfar_priv_rx_q *rx_queue = gfargrp->priv->rx_queue[0];
	int work_done = 0;

	/* Clear IEVENT, so interrupts aren't called again
	 * because of the packets that have already arrived
	 */
	gfar_write(&regs->ievent, IEVENT_RTX_MASK);

	/* run Tx cleanup to completion */
	if (tx_queue->tx_skbuff[tx_queue->skb_dirtytx])
		gfar_clean_tx_ring(tx_queue);

	work_done = gfar_clean_rx_ring(rx_queue, budget);

	if (work_done < budget) {
		napi_complete(napi);
		/* Clear the halt bit in RSTAT */
		gfar_write(&regs->rstat, gfargrp->rstat);

		gfar_write(&regs->imask, IMASK_DEFAULT);

		/* If we are coalescing interrupts, update the timer
		 * Otherwise, clear it
		 */
		gfar_write(&regs->txic, 0);
		if (likely(tx_queue->txcoalescing))
			gfar_write(&regs->txic, tx_queue->txic);

		gfar_write(&regs->rxic, 0);
		if (unlikely(rx_queue->rxcoalescing))
			gfar_write(&regs->rxic, rx_queue->rxic);
	}

	return work_done;
}

static int gfar_poll(struct napi_struct *napi, int budget)
{
	struct gfar_priv_grp *gfargrp =
		container_of(napi, struct gfar_priv_grp, napi);
>>>>>>> refs/remotes/origin/master
	struct gfar_private *priv = gfargrp->priv;
	struct gfar __iomem *regs = gfargrp->regs;
	struct gfar_priv_tx_q *tx_queue = NULL;
	struct gfar_priv_rx_q *rx_queue = NULL;
<<<<<<< HEAD
	int rx_cleaned = 0, budget_per_queue = 0, rx_cleaned_per_queue = 0;
	int tx_cleaned = 0, i, left_over_budget = budget;
	unsigned long serviced_queues = 0;
	int num_queues = 0;

	num_queues = gfargrp->num_rx_queues;
	budget_per_queue = budget/num_queues;

	/* Clear IEVENT, so interrupts aren't called again
	 * because of the packets that have already arrived */
	gfar_write(&regs->ievent, IEVENT_RTX_MASK);

	while (num_queues && left_over_budget) {

		budget_per_queue = left_over_budget/num_queues;
		left_over_budget = 0;

		for_each_set_bit(i, &gfargrp->rx_bit_map, priv->num_rx_queues) {
			if (test_bit(i, &serviced_queues))
				continue;
			rx_queue = priv->rx_queue[i];
			tx_queue = priv->tx_queue[rx_queue->qindex];

			tx_cleaned += gfar_clean_tx_ring(tx_queue);
			rx_cleaned_per_queue = gfar_clean_rx_ring(rx_queue,
							budget_per_queue);
			rx_cleaned += rx_cleaned_per_queue;
			if(rx_cleaned_per_queue < budget_per_queue) {
				left_over_budget = left_over_budget +
					(budget_per_queue - rx_cleaned_per_queue);
				set_bit(i, &serviced_queues);
				num_queues--;
			}
		}
	}

	if (tx_cleaned)
		return budget;

	if (rx_cleaned < budget) {
=======
	int work_done = 0, work_done_per_q = 0;
	int i, budget_per_q = 0;
	int has_tx_work = 0;
	unsigned long rstat_rxf;
	int num_act_queues;

	/* Clear IEVENT, so interrupts aren't called again
	 * because of the packets that have already arrived
	 */
	gfar_write(&regs->ievent, IEVENT_RTX_MASK);

	rstat_rxf = gfar_read(&regs->rstat) & RSTAT_RXF_MASK;

	num_act_queues = bitmap_weight(&rstat_rxf, MAX_RX_QS);
	if (num_act_queues)
		budget_per_q = budget/num_act_queues;

	for_each_set_bit(i, &gfargrp->tx_bit_map, priv->num_tx_queues) {
		tx_queue = priv->tx_queue[i];
		/* run Tx cleanup to completion */
		if (tx_queue->tx_skbuff[tx_queue->skb_dirtytx]) {
			gfar_clean_tx_ring(tx_queue);
			has_tx_work = 1;
		}
	}

	for_each_set_bit(i, &gfargrp->rx_bit_map, priv->num_rx_queues) {
		/* skip queue if not active */
		if (!(rstat_rxf & (RSTAT_CLEAR_RXF0 >> i)))
			continue;

		rx_queue = priv->rx_queue[i];
		work_done_per_q =
			gfar_clean_rx_ring(rx_queue, budget_per_q);
		work_done += work_done_per_q;

		/* finished processing this queue */
		if (work_done_per_q < budget_per_q) {
			/* clear active queue hw indication */
			gfar_write(&regs->rstat,
				   RSTAT_CLEAR_RXF0 >> i);
			num_act_queues--;

			if (!num_act_queues)
				break;
		}
	}

	if (!num_act_queues && !has_tx_work) {

>>>>>>> refs/remotes/origin/master
		napi_complete(napi);

		/* Clear the halt bit in RSTAT */
		gfar_write(&regs->rstat, gfargrp->rstat);

		gfar_write(&regs->imask, IMASK_DEFAULT);

<<<<<<< HEAD
		/* If we are coalescing interrupts, update the timer */
		/* Otherwise, clear it */
		gfar_configure_coalescing(priv,
				gfargrp->rx_bit_map, gfargrp->tx_bit_map);
	}

	return rx_cleaned;
}

#ifdef CONFIG_NET_POLL_CONTROLLER
/*
 * Polling 'interrupt' - used by things like netconsole to send skbs
=======
		/* If we are coalescing interrupts, update the timer
		 * Otherwise, clear it
		 */
		gfar_configure_coalescing(priv, gfargrp->rx_bit_map,
					  gfargrp->tx_bit_map);
	}

	return work_done;
}

#ifdef CONFIG_NET_POLL_CONTROLLER
/* Polling 'interrupt' - used by things like netconsole to send skbs
>>>>>>> refs/remotes/origin/master
 * without having to re-enable interrupts. It's not called while
 * the interrupt routine is executing.
 */
static void gfar_netpoll(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
<<<<<<< HEAD
	int i = 0;
=======
	int i;
>>>>>>> refs/remotes/origin/master

	/* If the device has multiple interrupts, run tx/rx */
	if (priv->device_flags & FSL_GIANFAR_DEV_HAS_MULTI_INTR) {
		for (i = 0; i < priv->num_grps; i++) {
<<<<<<< HEAD
			disable_irq(priv->gfargrp[i].interruptTransmit);
			disable_irq(priv->gfargrp[i].interruptReceive);
			disable_irq(priv->gfargrp[i].interruptError);
			gfar_interrupt(priv->gfargrp[i].interruptTransmit,
						&priv->gfargrp[i]);
			enable_irq(priv->gfargrp[i].interruptError);
			enable_irq(priv->gfargrp[i].interruptReceive);
			enable_irq(priv->gfargrp[i].interruptTransmit);
		}
	} else {
		for (i = 0; i < priv->num_grps; i++) {
			disable_irq(priv->gfargrp[i].interruptTransmit);
			gfar_interrupt(priv->gfargrp[i].interruptTransmit,
						&priv->gfargrp[i]);
			enable_irq(priv->gfargrp[i].interruptTransmit);
=======
			struct gfar_priv_grp *grp = &priv->gfargrp[i];

			disable_irq(gfar_irq(grp, TX)->irq);
			disable_irq(gfar_irq(grp, RX)->irq);
			disable_irq(gfar_irq(grp, ER)->irq);
			gfar_interrupt(gfar_irq(grp, TX)->irq, grp);
			enable_irq(gfar_irq(grp, ER)->irq);
			enable_irq(gfar_irq(grp, RX)->irq);
			enable_irq(gfar_irq(grp, TX)->irq);
		}
	} else {
		for (i = 0; i < priv->num_grps; i++) {
			struct gfar_priv_grp *grp = &priv->gfargrp[i];

			disable_irq(gfar_irq(grp, TX)->irq);
			gfar_interrupt(gfar_irq(grp, TX)->irq, grp);
			enable_irq(gfar_irq(grp, TX)->irq);
>>>>>>> refs/remotes/origin/master
		}
	}
}
#endif

/* The interrupt handler for devices with one interrupt */
static irqreturn_t gfar_interrupt(int irq, void *grp_id)
{
	struct gfar_priv_grp *gfargrp = grp_id;

	/* Save ievent for future reference */
	u32 events = gfar_read(&gfargrp->regs->ievent);

	/* Check for reception */
	if (events & IEVENT_RX_MASK)
		gfar_receive(irq, grp_id);

	/* Check for transmit completion */
	if (events & IEVENT_TX_MASK)
		gfar_transmit(irq, grp_id);

	/* Check for errors */
	if (events & IEVENT_ERR_MASK)
		gfar_error(irq, grp_id);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
=======
static u32 gfar_get_flowctrl_cfg(struct gfar_private *priv)
{
	struct phy_device *phydev = priv->phydev;
	u32 val = 0;

	if (!phydev->duplex)
		return val;

	if (!priv->pause_aneg_en) {
		if (priv->tx_pause_en)
			val |= MACCFG1_TX_FLOW;
		if (priv->rx_pause_en)
			val |= MACCFG1_RX_FLOW;
	} else {
		u16 lcl_adv, rmt_adv;
		u8 flowctrl;
		/* get link partner capabilities */
		rmt_adv = 0;
		if (phydev->pause)
			rmt_adv = LPA_PAUSE_CAP;
		if (phydev->asym_pause)
			rmt_adv |= LPA_PAUSE_ASYM;

		lcl_adv = mii_advertise_flowctrl(phydev->advertising);

		flowctrl = mii_resolve_flowctrl_fdx(lcl_adv, rmt_adv);
		if (flowctrl & FLOW_CTRL_TX)
			val |= MACCFG1_TX_FLOW;
		if (flowctrl & FLOW_CTRL_RX)
			val |= MACCFG1_RX_FLOW;
	}

	return val;
}

>>>>>>> refs/remotes/origin/master
/* Called every time the controller might need to be made
 * aware of new link state.  The PHY code conveys this
 * information through variables in the phydev structure, and this
 * function converts those variables into the appropriate
 * register values, and can bring down the device if needed.
 */
static void adjust_link(struct net_device *dev)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct gfar __iomem *regs = priv->gfargrp[0].regs;
	unsigned long flags;
	struct phy_device *phydev = priv->phydev;
	int new_state = 0;

	local_irq_save(flags);
	lock_tx_qs(priv);

	if (phydev->link) {
<<<<<<< HEAD
=======
		u32 tempval1 = gfar_read(&regs->maccfg1);
>>>>>>> refs/remotes/origin/master
		u32 tempval = gfar_read(&regs->maccfg2);
		u32 ecntrl = gfar_read(&regs->ecntrl);

		/* Now we make sure that we can be in full duplex mode.
<<<<<<< HEAD
		 * If not, we operate in half-duplex mode. */
=======
		 * If not, we operate in half-duplex mode.
		 */
>>>>>>> refs/remotes/origin/master
		if (phydev->duplex != priv->oldduplex) {
			new_state = 1;
			if (!(phydev->duplex))
				tempval &= ~(MACCFG2_FULL_DUPLEX);
			else
				tempval |= MACCFG2_FULL_DUPLEX;

			priv->oldduplex = phydev->duplex;
		}

		if (phydev->speed != priv->oldspeed) {
			new_state = 1;
			switch (phydev->speed) {
			case 1000:
				tempval =
				    ((tempval & ~(MACCFG2_IF)) | MACCFG2_GMII);

				ecntrl &= ~(ECNTRL_R100);
				break;
			case 100:
			case 10:
				tempval =
				    ((tempval & ~(MACCFG2_IF)) | MACCFG2_MII);

				/* Reduced mode distinguishes
<<<<<<< HEAD
				 * between 10 and 100 */
=======
				 * between 10 and 100
				 */
>>>>>>> refs/remotes/origin/master
				if (phydev->speed == SPEED_100)
					ecntrl |= ECNTRL_R100;
				else
					ecntrl &= ~(ECNTRL_R100);
				break;
			default:
				netif_warn(priv, link, dev,
					   "Ack!  Speed (%d) is not 10/100/1000!\n",
					   phydev->speed);
				break;
			}

			priv->oldspeed = phydev->speed;
		}

<<<<<<< HEAD
=======
		tempval1 &= ~(MACCFG1_TX_FLOW | MACCFG1_RX_FLOW);
		tempval1 |= gfar_get_flowctrl_cfg(priv);

		gfar_write(&regs->maccfg1, tempval1);
>>>>>>> refs/remotes/origin/master
		gfar_write(&regs->maccfg2, tempval);
		gfar_write(&regs->ecntrl, ecntrl);

		if (!priv->oldlink) {
			new_state = 1;
			priv->oldlink = 1;
		}
	} else if (priv->oldlink) {
		new_state = 1;
		priv->oldlink = 0;
		priv->oldspeed = 0;
		priv->oldduplex = -1;
	}

	if (new_state && netif_msg_link(priv))
		phy_print_status(phydev);
	unlock_tx_qs(priv);
	local_irq_restore(flags);
}

/* Update the hash table based on the current list of multicast
 * addresses we subscribe to.  Also, change the promiscuity of
 * the device based on the flags (this function is called
<<<<<<< HEAD
 * whenever dev->flags is changed */
=======
 * whenever dev->flags is changed
 */
>>>>>>> refs/remotes/origin/master
static void gfar_set_multi(struct net_device *dev)
{
	struct netdev_hw_addr *ha;
	struct gfar_private *priv = netdev_priv(dev);
	struct gfar __iomem *regs = priv->gfargrp[0].regs;
	u32 tempval;

	if (dev->flags & IFF_PROMISC) {
		/* Set RCTRL to PROM */
		tempval = gfar_read(&regs->rctrl);
		tempval |= RCTRL_PROM;
		gfar_write(&regs->rctrl, tempval);
	} else {
		/* Set RCTRL to not PROM */
		tempval = gfar_read(&regs->rctrl);
		tempval &= ~(RCTRL_PROM);
		gfar_write(&regs->rctrl, tempval);
	}

	if (dev->flags & IFF_ALLMULTI) {
		/* Set the hash to rx all multicast frames */
		gfar_write(&regs->igaddr0, 0xffffffff);
		gfar_write(&regs->igaddr1, 0xffffffff);
		gfar_write(&regs->igaddr2, 0xffffffff);
		gfar_write(&regs->igaddr3, 0xffffffff);
		gfar_write(&regs->igaddr4, 0xffffffff);
		gfar_write(&regs->igaddr5, 0xffffffff);
		gfar_write(&regs->igaddr6, 0xffffffff);
		gfar_write(&regs->igaddr7, 0xffffffff);
		gfar_write(&regs->gaddr0, 0xffffffff);
		gfar_write(&regs->gaddr1, 0xffffffff);
		gfar_write(&regs->gaddr2, 0xffffffff);
		gfar_write(&regs->gaddr3, 0xffffffff);
		gfar_write(&regs->gaddr4, 0xffffffff);
		gfar_write(&regs->gaddr5, 0xffffffff);
		gfar_write(&regs->gaddr6, 0xffffffff);
		gfar_write(&regs->gaddr7, 0xffffffff);
	} else {
		int em_num;
		int idx;

		/* zero out the hash */
		gfar_write(&regs->igaddr0, 0x0);
		gfar_write(&regs->igaddr1, 0x0);
		gfar_write(&regs->igaddr2, 0x0);
		gfar_write(&regs->igaddr3, 0x0);
		gfar_write(&regs->igaddr4, 0x0);
		gfar_write(&regs->igaddr5, 0x0);
		gfar_write(&regs->igaddr6, 0x0);
		gfar_write(&regs->igaddr7, 0x0);
		gfar_write(&regs->gaddr0, 0x0);
		gfar_write(&regs->gaddr1, 0x0);
		gfar_write(&regs->gaddr2, 0x0);
		gfar_write(&regs->gaddr3, 0x0);
		gfar_write(&regs->gaddr4, 0x0);
		gfar_write(&regs->gaddr5, 0x0);
		gfar_write(&regs->gaddr6, 0x0);
		gfar_write(&regs->gaddr7, 0x0);

		/* If we have extended hash tables, we need to
		 * clear the exact match registers to prepare for
<<<<<<< HEAD
		 * setting them */
=======
		 * setting them
		 */
>>>>>>> refs/remotes/origin/master
		if (priv->extended_hash) {
			em_num = GFAR_EM_NUM + 1;
			gfar_clear_exact_match(dev);
			idx = 1;
		} else {
			idx = 0;
			em_num = 0;
		}

		if (netdev_mc_empty(dev))
			return;

		/* Parse the list, and set the appropriate bits */
		netdev_for_each_mc_addr(ha, dev) {
			if (idx < em_num) {
				gfar_set_mac_for_addr(dev, idx, ha->addr);
				idx++;
			} else
				gfar_set_hash_for_addr(dev, ha->addr);
		}
	}
}


/* Clears each of the exact match registers to zero, so they
<<<<<<< HEAD
 * don't interfere with normal reception */
=======
 * don't interfere with normal reception
 */
>>>>>>> refs/remotes/origin/master
static void gfar_clear_exact_match(struct net_device *dev)
{
	int idx;
	static const u8 zero_arr[ETH_ALEN] = {0, 0, 0, 0, 0, 0};

<<<<<<< HEAD
	for(idx = 1;idx < GFAR_EM_NUM + 1;idx++)
=======
	for (idx = 1; idx < GFAR_EM_NUM + 1; idx++)
>>>>>>> refs/remotes/origin/master
		gfar_set_mac_for_addr(dev, idx, zero_arr);
}

/* Set the appropriate hash bit for the given addr */
/* The algorithm works like so:
 * 1) Take the Destination Address (ie the multicast address), and
 * do a CRC on it (little endian), and reverse the bits of the
 * result.
 * 2) Use the 8 most significant bits as a hash into a 256-entry
 * table.  The table is controlled through 8 32-bit registers:
 * gaddr0-7.  gaddr0's MSB is entry 0, and gaddr7's LSB is
 * gaddr7.  This means that the 3 most significant bits in the
 * hash index which gaddr register to use, and the 5 other bits
 * indicate which bit (assuming an IBM numbering scheme, which
 * for PowerPC (tm) is usually the case) in the register holds
<<<<<<< HEAD
 * the entry. */
=======
 * the entry.
 */
>>>>>>> refs/remotes/origin/master
static void gfar_set_hash_for_addr(struct net_device *dev, u8 *addr)
{
	u32 tempval;
	struct gfar_private *priv = netdev_priv(dev);
	u32 result = ether_crc(ETH_ALEN, addr);
	int width = priv->hash_width;
	u8 whichbit = (result >> (32 - width)) & 0x1f;
	u8 whichreg = result >> (32 - width + 5);
	u32 value = (1 << (31-whichbit));

	tempval = gfar_read(priv->hash_regs[whichreg]);
	tempval |= value;
	gfar_write(priv->hash_regs[whichreg], tempval);
}


/* There are multiple MAC Address register pairs on some controllers
 * This function sets the numth pair to a given address
 */
static void gfar_set_mac_for_addr(struct net_device *dev, int num,
				  const u8 *addr)
{
	struct gfar_private *priv = netdev_priv(dev);
	struct gfar __iomem *regs = priv->gfargrp[0].regs;
	int idx;
	char tmpbuf[ETH_ALEN];
	u32 tempval;
	u32 __iomem *macptr = &regs->macstnaddr1;

	macptr += num*2;

<<<<<<< HEAD
	/* Now copy it into the mac registers backwards, cuz */
	/* little endian is silly */
=======
	/* Now copy it into the mac registers backwards, cuz
	 * little endian is silly
	 */
>>>>>>> refs/remotes/origin/master
	for (idx = 0; idx < ETH_ALEN; idx++)
		tmpbuf[ETH_ALEN - 1 - idx] = addr[idx];

	gfar_write(macptr, *((u32 *) (tmpbuf)));

	tempval = *((u32 *) (tmpbuf + 4));

	gfar_write(macptr+1, tempval);
}

/* GFAR error interrupt handler */
static irqreturn_t gfar_error(int irq, void *grp_id)
{
	struct gfar_priv_grp *gfargrp = grp_id;
	struct gfar __iomem *regs = gfargrp->regs;
	struct gfar_private *priv= gfargrp->priv;
	struct net_device *dev = priv->ndev;

	/* Save ievent for future reference */
	u32 events = gfar_read(&regs->ievent);

	/* Clear IEVENT */
	gfar_write(&regs->ievent, events & IEVENT_ERR_MASK);

	/* Magic Packet is not an error. */
	if ((priv->device_flags & FSL_GIANFAR_DEV_HAS_MAGIC_PACKET) &&
	    (events & IEVENT_MAG))
		events &= ~IEVENT_MAG;

	/* Hmm... */
	if (netif_msg_rx_err(priv) || netif_msg_tx_err(priv))
<<<<<<< HEAD
		netdev_dbg(dev, "error interrupt (ievent=0x%08x imask=0x%08x)\n",
=======
		netdev_dbg(dev,
			   "error interrupt (ievent=0x%08x imask=0x%08x)\n",
>>>>>>> refs/remotes/origin/master
			   events, gfar_read(&regs->imask));

	/* Update the error counters */
	if (events & IEVENT_TXE) {
		dev->stats.tx_errors++;

		if (events & IEVENT_LC)
			dev->stats.tx_window_errors++;
		if (events & IEVENT_CRL)
			dev->stats.tx_aborted_errors++;
		if (events & IEVENT_XFUN) {
			unsigned long flags;

			netif_dbg(priv, tx_err, dev,
				  "TX FIFO underrun, packet dropped\n");
			dev->stats.tx_dropped++;
<<<<<<< HEAD
			priv->extra_stats.tx_underrun++;
=======
			atomic64_inc(&priv->extra_stats.tx_underrun);
>>>>>>> refs/remotes/origin/master

			local_irq_save(flags);
			lock_tx_qs(priv);

			/* Reactivate the Tx Queues */
			gfar_write(&regs->tstat, gfargrp->tstat);

			unlock_tx_qs(priv);
			local_irq_restore(flags);
		}
		netif_dbg(priv, tx_err, dev, "Transmit Error\n");
	}
	if (events & IEVENT_BSY) {
		dev->stats.rx_errors++;
<<<<<<< HEAD
		priv->extra_stats.rx_bsy++;
=======
		atomic64_inc(&priv->extra_stats.rx_bsy);
>>>>>>> refs/remotes/origin/master

		gfar_receive(irq, grp_id);

		netif_dbg(priv, rx_err, dev, "busy error (rstat: %x)\n",
			  gfar_read(&regs->rstat));
	}
	if (events & IEVENT_BABR) {
		dev->stats.rx_errors++;
<<<<<<< HEAD
		priv->extra_stats.rx_babr++;
=======
		atomic64_inc(&priv->extra_stats.rx_babr);
>>>>>>> refs/remotes/origin/master

		netif_dbg(priv, rx_err, dev, "babbling RX error\n");
	}
	if (events & IEVENT_EBERR) {
<<<<<<< HEAD
		priv->extra_stats.eberr++;
=======
		atomic64_inc(&priv->extra_stats.eberr);
>>>>>>> refs/remotes/origin/master
		netif_dbg(priv, rx_err, dev, "bus error\n");
	}
	if (events & IEVENT_RXC)
		netif_dbg(priv, rx_status, dev, "control frame\n");

	if (events & IEVENT_BABT) {
<<<<<<< HEAD
		priv->extra_stats.tx_babt++;
=======
		atomic64_inc(&priv->extra_stats.tx_babt);
>>>>>>> refs/remotes/origin/master
		netif_dbg(priv, tx_err, dev, "babbling TX error\n");
	}
	return IRQ_HANDLED;
}

static struct of_device_id gfar_match[] =
{
	{
		.type = "network",
		.compatible = "gianfar",
	},
	{
		.compatible = "fsl,etsec2",
	},
	{},
};
MODULE_DEVICE_TABLE(of, gfar_match);

/* Structure for a device driver */
static struct platform_driver gfar_driver = {
	.driver = {
		.name = "fsl-gianfar",
		.owner = THIS_MODULE,
		.pm = GFAR_PM_OPS,
		.of_match_table = gfar_match,
	},
	.probe = gfar_probe,
	.remove = gfar_remove,
};

module_platform_driver(gfar_driver);
