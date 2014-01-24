/*
 * Copyright (c) 2010 Broadcom Corporation
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _BRCMF_BUS_H_
#define _BRCMF_BUS_H_

/* The level of bus communication with the dongle */
enum brcmf_bus_state {
	BRCMF_BUS_DOWN,		/* Not ready for frame transfers */
	BRCMF_BUS_LOAD,		/* Download access only (CPU reset) */
	BRCMF_BUS_DATA		/* Ready for frame transfers */
};

<<<<<<< HEAD
struct dngl_stats {
	unsigned long rx_packets;	/* total packets received */
	unsigned long tx_packets;	/* total packets transmitted */
	unsigned long rx_bytes;	/* total bytes received */
	unsigned long tx_bytes;	/* total bytes transmitted */
	unsigned long rx_errors;	/* bad packets received */
	unsigned long tx_errors;	/* packet transmit problems */
	unsigned long rx_dropped;	/* packets dropped by dongle */
	unsigned long tx_dropped;	/* packets dropped by dongle */
	unsigned long multicast;	/* multicast packets received */
};

/* interface structure between common and bus layer */
struct brcmf_bus {
	u8 type;		/* bus type */
=======
struct brcmf_bus_dcmd {
	char *name;
	char *param;
	int param_len;
	struct list_head list;
};

/**
 * struct brcmf_bus_ops - bus callback operations.
 *
 * @init: prepare for communication with dongle.
 * @stop: clear pending frames, disable data flow.
 * @txdata: send a data frame to the dongle. When the data
 *	has been transferred, the common driver must be
 *	notified using brcmf_txcomplete(). The common
 *	driver calls this function with interrupts
 *	disabled.
 * @txctl: transmit a control request message to dongle.
 * @rxctl: receive a control response message from dongle.
 * @gettxq: obtain a reference of bus transmit queue (optional).
 *
 * This structure provides an abstract interface towards the
 * bus specific driver. For control messages to common driver
 * will assure there is only one active transaction. Unless
 * indicated otherwise these callbacks are mandatory.
 */
struct brcmf_bus_ops {
	int (*init)(struct device *dev);
	void (*stop)(struct device *dev);
	int (*txdata)(struct device *dev, struct sk_buff *skb);
	int (*txctl)(struct device *dev, unsigned char *msg, uint len);
	int (*rxctl)(struct device *dev, unsigned char *msg, uint len);
	struct pktq * (*gettxq)(struct device *dev);
};

/**
 * struct brcmf_bus - interface structure between common and bus layer
 *
 * @bus_priv: pointer to private bus device.
 * @dev: device pointer of bus device.
 * @drvr: public driver information.
 * @state: operational state of the bus interface.
 * @maxctl: maximum size for rxctl request message.
 * @tx_realloc: number of tx packets realloced for headroom.
 * @dstats: dongle-based statistical data.
 * @dcmd_list: bus/device specific dongle initialization commands.
 * @chip: device identifier of the dongle chip.
 * @chiprev: revision of the dongle chip.
 */
struct brcmf_bus {
>>>>>>> refs/remotes/origin/master
	union {
		struct brcmf_sdio_dev *sdio;
		struct brcmf_usbdev *usb;
	} bus_priv;
<<<<<<< HEAD
	struct brcmf_pub *drvr;	/* pointer to driver pub structure brcmf_pub */
	enum brcmf_bus_state state;
	uint maxctl;		/* Max size rxctl request from proto to bus */
	bool drvr_up;		/* Status flag of driver up/down */
	unsigned long tx_realloc;	/* Tx packets realloced for headroom */
	struct dngl_stats dstats;	/* Stats for dongle-based data */
	u8 align;		/* bus alignment requirement */

	/* interface functions pointers */
	/* Stop bus module: clear pending frames, disable data flow */
	void (*brcmf_bus_stop)(struct device *);
	/* Initialize bus module: prepare for communication w/dongle */
	int (*brcmf_bus_init)(struct device *);
	/* Send a data frame to the dongle.  Callee disposes of txp. */
	int (*brcmf_bus_txdata)(struct device *, struct sk_buff *);
	/* Send/receive a control message to/from the dongle.
	 * Expects caller to enforce a single outstanding transaction.
	 */
	int (*brcmf_bus_txctl)(struct device *, unsigned char *, uint);
	int (*brcmf_bus_rxctl)(struct device *, unsigned char *, uint);
};

/*
 * interface functions from common layer
 */

/* Remove any protocol-specific data header. */
extern int brcmf_proto_hdrpull(struct device *dev, int *ifidx,
			       struct sk_buff *rxp);

extern bool brcmf_c_prec_enq(struct device *dev, struct pktq *q,
			 struct sk_buff *pkt, int prec);

/* Receive frame for delivery to OS.  Callee disposes of rxp. */
extern void brcmf_rx_frame(struct device *dev, int ifidx,
			   struct sk_buff_head *rxlist);
static inline void brcmf_rx_packet(struct device *dev, int ifidx,
				   struct sk_buff *pkt)
{
	struct sk_buff_head q;

	skb_queue_head_init(&q);
	skb_queue_tail(&q, pkt);
	brcmf_rx_frame(dev, ifidx, &q);
}

/* Indication from bus module regarding presence/insertion of dongle. */
extern int brcmf_attach(uint bus_hdrlen, struct device *dev);
/* Indication from bus module regarding removal/absence of dongle */
extern void brcmf_detach(struct device *dev);

/* Indication from bus module to change flow-control state */
extern void brcmf_txflowcontrol(struct device *dev, int ifidx, bool on);

/* Notify tx completion */
extern void brcmf_txcomplete(struct device *dev, struct sk_buff *txp,
			     bool success);

extern int brcmf_bus_start(struct device *dev);

extern int brcmf_add_if(struct device *dev, int ifidx,
			char *name, u8 *mac_addr);

#ifdef CONFIG_BRCMFMAC_SDIO
extern void brcmf_sdio_exit(void);
extern void brcmf_sdio_init(void);
#endif
#ifdef CONFIG_BRCMFMAC_USB
extern void brcmf_usb_exit(void);
extern void brcmf_usb_init(void);
=======
	struct device *dev;
	struct brcmf_pub *drvr;
	enum brcmf_bus_state state;
	uint maxctl;
	unsigned long tx_realloc;
	u32 chip;
	u32 chiprev;
	struct list_head dcmd_list;

	struct brcmf_bus_ops *ops;
};

/*
 * callback wrappers
 */
static inline int brcmf_bus_init(struct brcmf_bus *bus)
{
	return bus->ops->init(bus->dev);
}

static inline void brcmf_bus_stop(struct brcmf_bus *bus)
{
	bus->ops->stop(bus->dev);
}

static inline int brcmf_bus_txdata(struct brcmf_bus *bus, struct sk_buff *skb)
{
	return bus->ops->txdata(bus->dev, skb);
}

static inline
int brcmf_bus_txctl(struct brcmf_bus *bus, unsigned char *msg, uint len)
{
	return bus->ops->txctl(bus->dev, msg, len);
}

static inline
int brcmf_bus_rxctl(struct brcmf_bus *bus, unsigned char *msg, uint len)
{
	return bus->ops->rxctl(bus->dev, msg, len);
}

static inline
struct pktq *brcmf_bus_gettxq(struct brcmf_bus *bus)
{
	if (!bus->ops->gettxq)
		return ERR_PTR(-ENOENT);

	return bus->ops->gettxq(bus->dev);
}
/*
 * interface functions from common layer
 */

bool brcmf_c_prec_enq(struct device *dev, struct pktq *q, struct sk_buff *pkt,
		      int prec);

/* Receive frame for delivery to OS.  Callee disposes of rxp. */
void brcmf_rx_frame(struct device *dev, struct sk_buff *rxp);

/* Indication from bus module regarding presence/insertion of dongle. */
int brcmf_attach(uint bus_hdrlen, struct device *dev);
/* Indication from bus module regarding removal/absence of dongle */
void brcmf_detach(struct device *dev);
/* Indication from bus module that dongle should be reset */
void brcmf_dev_reset(struct device *dev);
/* Indication from bus module to change flow-control state */
void brcmf_txflowblock(struct device *dev, bool state);

/* Notify the bus has transferred the tx packet to firmware */
void brcmf_txcomplete(struct device *dev, struct sk_buff *txp, bool success);

int brcmf_bus_start(struct device *dev);

#ifdef CONFIG_BRCMFMAC_SDIO
void brcmf_sdio_exit(void);
void brcmf_sdio_init(void);
void brcmf_sdio_register(void);
#endif
#ifdef CONFIG_BRCMFMAC_USB
void brcmf_usb_exit(void);
void brcmf_usb_register(void);
>>>>>>> refs/remotes/origin/master
#endif

#endif				/* _BRCMF_BUS_H_ */
