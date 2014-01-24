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

/*******************************************************************************
 * Communicates with the dongle by using dcmd codes.
 * For certain dcmd codes, the dongle interprets string data from the host.
 ******************************************************************************/

<<<<<<< HEAD
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/sched.h>
#include <defs.h>
=======
#include <linux/types.h>
#include <linux/netdevice.h>
>>>>>>> refs/remotes/origin/master

#include <brcmu_utils.h>
#include <brcmu_wifi.h>

#include "dhd.h"
#include "dhd_proto.h"
#include "dhd_bus.h"
<<<<<<< HEAD
#include "dhd_dbg.h"
=======
#include "fwsignal.h"
#include "dhd_dbg.h"
#include "tracepoint.h"
>>>>>>> refs/remotes/origin/master

struct brcmf_proto_cdc_dcmd {
	__le32 cmd;	/* dongle command value */
	__le32 len;	/* lower 16: output buflen;
			 * upper 16: input buflen (excludes header) */
	__le32 flags;	/* flag defns given below */
	__le32 status;	/* status code returned from the device */
};

/* Max valid buffer size that can be sent to the dongle */
#define CDC_MAX_MSG_SIZE	(ETH_FRAME_LEN+ETH_FCS_LEN)

/* CDC flag definitions */
#define CDC_DCMD_ERROR		0x01	/* 1=cmd failed */
#define CDC_DCMD_SET		0x02	/* 0=get, 1=set cmd */
#define CDC_DCMD_IF_MASK	0xF000		/* I/F index */
#define CDC_DCMD_IF_SHIFT	12
#define CDC_DCMD_ID_MASK	0xFFFF0000	/* id an cmd pairing */
#define CDC_DCMD_ID_SHIFT	16		/* ID Mask shift bits */
#define CDC_DCMD_ID(flags)	\
	(((flags) & CDC_DCMD_ID_MASK) >> CDC_DCMD_ID_SHIFT)

/*
 * BDC header - Broadcom specific extension of CDC.
 * Used on data packets to convey priority across USB.
 */
#define	BDC_HEADER_LEN		4
#define BDC_PROTO_VER		2	/* Protocol version */
#define BDC_FLAG_VER_MASK	0xf0	/* Protocol version mask */
#define BDC_FLAG_VER_SHIFT	4	/* Protocol version shift */
#define BDC_FLAG_SUM_GOOD	0x04	/* Good RX checksums */
#define BDC_FLAG_SUM_NEEDED	0x08	/* Dongle needs to do TX checksums */
#define BDC_PRIORITY_MASK	0x7
#define BDC_FLAG2_IF_MASK	0x0f	/* packet rx interface in APSTA */
#define BDC_FLAG2_IF_SHIFT	0

#define BDC_GET_IF_IDX(hdr) \
	((int)((((hdr)->flags2) & BDC_FLAG2_IF_MASK) >> BDC_FLAG2_IF_SHIFT))
#define BDC_SET_IF_IDX(hdr, idx) \
	((hdr)->flags2 = (((hdr)->flags2 & ~BDC_FLAG2_IF_MASK) | \
	((idx) << BDC_FLAG2_IF_SHIFT)))

<<<<<<< HEAD
struct brcmf_proto_bdc_header {
	u8 flags;
	u8 priority;	/* 802.1d Priority, 4:7 flow control info for usb */
=======
/**
 * struct brcmf_proto_bdc_header - BDC header format
 *
 * @flags: flags contain protocol and checksum info.
 * @priority: 802.1d priority and USB flow control info (bit 4:7).
 * @flags2: additional flags containing dongle interface index.
 * @data_offset: start of packet data. header is following by firmware signals.
 */
struct brcmf_proto_bdc_header {
	u8 flags;
	u8 priority;
>>>>>>> refs/remotes/origin/master
	u8 flags2;
	u8 data_offset;
};

<<<<<<< HEAD
=======
/*
 * maximum length of firmware signal data between
 * the BDC header and packet data in the tx path.
 */
#define BRCMF_PROT_FW_SIGNAL_MAX_TXBYTES	12
>>>>>>> refs/remotes/origin/master

#define RETRIES 2 /* # of retries to retrieve matching dcmd response */
#define BUS_HEADER_LEN	(16+64)		/* Must be atleast SDPCM_RESERVE
					 * (amount of header tha might be added)
					 * plus any space that might be needed
					 * for bus alignment padding.
					 */
#define ROUND_UP_MARGIN	2048	/* Biggest bus block size possible for
				 * round off at the end of buffer
				 * Currently is SDIO
				 */

struct brcmf_proto {
	u16 reqid;
<<<<<<< HEAD
	u8 pending;
	u32 lastcmd;
=======
>>>>>>> refs/remotes/origin/master
	u8 bus_header[BUS_HEADER_LEN];
	struct brcmf_proto_cdc_dcmd msg;
	unsigned char buf[BRCMF_DCMD_MAXLEN + ROUND_UP_MARGIN];
};

static int brcmf_proto_cdc_msg(struct brcmf_pub *drvr)
{
	struct brcmf_proto *prot = drvr->prot;
	int len = le32_to_cpu(prot->msg.len) +
			sizeof(struct brcmf_proto_cdc_dcmd);

<<<<<<< HEAD
	brcmf_dbg(TRACE, "Enter\n");
=======
	brcmf_dbg(CDC, "Enter\n");
>>>>>>> refs/remotes/origin/master

	/* NOTE : cdc->msg.len holds the desired length of the buffer to be
	 *        returned. Only up to CDC_MAX_MSG_SIZE of this buffer area
	 *        is actually sent to the dongle
	 */
	if (len > CDC_MAX_MSG_SIZE)
		len = CDC_MAX_MSG_SIZE;

	/* Send request */
<<<<<<< HEAD
	return drvr->bus_if->brcmf_bus_txctl(drvr->dev,
					     (unsigned char *)&prot->msg,
					     len);
=======
	return brcmf_bus_txctl(drvr->bus_if, (unsigned char *)&prot->msg, len);
>>>>>>> refs/remotes/origin/master
}

static int brcmf_proto_cdc_cmplt(struct brcmf_pub *drvr, u32 id, u32 len)
{
	int ret;
	struct brcmf_proto *prot = drvr->prot;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "Enter\n");

	do {
		ret = drvr->bus_if->brcmf_bus_rxctl(drvr->dev,
				(unsigned char *)&prot->msg,
				len + sizeof(struct brcmf_proto_cdc_dcmd));
=======
	brcmf_dbg(CDC, "Enter\n");
	len += sizeof(struct brcmf_proto_cdc_dcmd);
	do {
		ret = brcmf_bus_rxctl(drvr->bus_if, (unsigned char *)&prot->msg,
				      len);
>>>>>>> refs/remotes/origin/master
		if (ret < 0)
			break;
	} while (CDC_DCMD_ID(le32_to_cpu(prot->msg.flags)) != id);

	return ret;
}

int
brcmf_proto_cdc_query_dcmd(struct brcmf_pub *drvr, int ifidx, uint cmd,
			       void *buf, uint len)
{
	struct brcmf_proto *prot = drvr->prot;
	struct brcmf_proto_cdc_dcmd *msg = &prot->msg;
	void *info;
	int ret = 0, retries = 0;
	u32 id, flags;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "Enter\n");
	brcmf_dbg(CTL, "cmd %d len %d\n", cmd, len);

	/* Respond "bcmerror" and "bcmerrorstr" with local cache */
	if (cmd == BRCMF_C_GET_VAR && buf) {
		if (!strcmp((char *)buf, "bcmerrorstr")) {
			strncpy((char *)buf, "bcm_error",
				BCME_STRLEN);
			goto done;
		} else if (!strcmp((char *)buf, "bcmerror")) {
			*(int *)buf = drvr->dongle_error;
			goto done;
		}
	}
=======
	brcmf_dbg(CDC, "Enter, cmd %d len %d\n", cmd, len);
>>>>>>> refs/remotes/origin/master

	memset(msg, 0, sizeof(struct brcmf_proto_cdc_dcmd));

	msg->cmd = cpu_to_le32(cmd);
	msg->len = cpu_to_le32(len);
	flags = (++prot->reqid << CDC_DCMD_ID_SHIFT);
	flags = (flags & ~CDC_DCMD_IF_MASK) |
		(ifidx << CDC_DCMD_IF_SHIFT);
	msg->flags = cpu_to_le32(flags);

	if (buf)
		memcpy(prot->buf, buf, len);

	ret = brcmf_proto_cdc_msg(drvr);
	if (ret < 0) {
<<<<<<< HEAD
		brcmf_dbg(ERROR, "brcmf_proto_cdc_msg failed w/status %d\n",
=======
		brcmf_err("brcmf_proto_cdc_msg failed w/status %d\n",
>>>>>>> refs/remotes/origin/master
			  ret);
		goto done;
	}

retry:
	/* wait for interrupt and get first fragment */
	ret = brcmf_proto_cdc_cmplt(drvr, prot->reqid, len);
	if (ret < 0)
		goto done;

	flags = le32_to_cpu(msg->flags);
	id = (flags & CDC_DCMD_ID_MASK) >> CDC_DCMD_ID_SHIFT;

	if ((id < prot->reqid) && (++retries < RETRIES))
		goto retry;
	if (id != prot->reqid) {
<<<<<<< HEAD
		brcmf_dbg(ERROR, "%s: unexpected request id %d (expected %d)\n",
=======
		brcmf_err("%s: unexpected request id %d (expected %d)\n",
>>>>>>> refs/remotes/origin/master
			  brcmf_ifname(drvr, ifidx), id, prot->reqid);
		ret = -EINVAL;
		goto done;
	}

	/* Check info buffer */
	info = (void *)&msg[1];

	/* Copy info buffer */
	if (buf) {
		if (ret < (int)len)
			len = ret;
		memcpy(buf, info, len);
	}

	/* Check the ERROR flag */
<<<<<<< HEAD
	if (flags & CDC_DCMD_ERROR) {
		ret = le32_to_cpu(msg->status);
		/* Cache error from dongle */
		drvr->dongle_error = ret;
	}
=======
	if (flags & CDC_DCMD_ERROR)
		ret = le32_to_cpu(msg->status);
>>>>>>> refs/remotes/origin/master

done:
	return ret;
}

int brcmf_proto_cdc_set_dcmd(struct brcmf_pub *drvr, int ifidx, uint cmd,
				 void *buf, uint len)
{
	struct brcmf_proto *prot = drvr->prot;
	struct brcmf_proto_cdc_dcmd *msg = &prot->msg;
	int ret = 0;
	u32 flags, id;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "Enter\n");
	brcmf_dbg(CTL, "cmd %d len %d\n", cmd, len);
=======
	brcmf_dbg(CDC, "Enter, cmd %d len %d\n", cmd, len);
>>>>>>> refs/remotes/origin/master

	memset(msg, 0, sizeof(struct brcmf_proto_cdc_dcmd));

	msg->cmd = cpu_to_le32(cmd);
	msg->len = cpu_to_le32(len);
	flags = (++prot->reqid << CDC_DCMD_ID_SHIFT) | CDC_DCMD_SET;
	flags = (flags & ~CDC_DCMD_IF_MASK) |
		(ifidx << CDC_DCMD_IF_SHIFT);
	msg->flags = cpu_to_le32(flags);

	if (buf)
		memcpy(prot->buf, buf, len);

	ret = brcmf_proto_cdc_msg(drvr);
	if (ret < 0)
		goto done;

	ret = brcmf_proto_cdc_cmplt(drvr, prot->reqid, len);
	if (ret < 0)
		goto done;

	flags = le32_to_cpu(msg->flags);
	id = (flags & CDC_DCMD_ID_MASK) >> CDC_DCMD_ID_SHIFT;

	if (id != prot->reqid) {
<<<<<<< HEAD
		brcmf_dbg(ERROR, "%s: unexpected request id %d (expected %d)\n",
=======
		brcmf_err("%s: unexpected request id %d (expected %d)\n",
>>>>>>> refs/remotes/origin/master
			  brcmf_ifname(drvr, ifidx), id, prot->reqid);
		ret = -EINVAL;
		goto done;
	}

	/* Check the ERROR flag */
<<<<<<< HEAD
	if (flags & CDC_DCMD_ERROR) {
		ret = le32_to_cpu(msg->status);
		/* Cache error from dongle */
		drvr->dongle_error = ret;
	}
=======
	if (flags & CDC_DCMD_ERROR)
		ret = le32_to_cpu(msg->status);
>>>>>>> refs/remotes/origin/master

done:
	return ret;
}

<<<<<<< HEAD
int
brcmf_proto_dcmd(struct brcmf_pub *drvr, int ifidx, struct brcmf_dcmd *dcmd,
		  int len)
{
	struct brcmf_proto *prot = drvr->prot;
	int ret = -1;

	if (drvr->bus_if->state == BRCMF_BUS_DOWN) {
		brcmf_dbg(ERROR, "bus is down. we have nothing to do.\n");
		return ret;
	}
	mutex_lock(&drvr->proto_block);

	brcmf_dbg(TRACE, "Enter\n");

	if (len > BRCMF_DCMD_MAXLEN)
		goto done;

	if (prot->pending == true) {
		brcmf_dbg(TRACE, "CDC packet is pending!!!! cmd=0x%x (%lu) lastcmd=0x%x (%lu)\n",
			  dcmd->cmd, (unsigned long)dcmd->cmd, prot->lastcmd,
			  (unsigned long)prot->lastcmd);
		if (dcmd->cmd == BRCMF_C_SET_VAR ||
		    dcmd->cmd == BRCMF_C_GET_VAR)
			brcmf_dbg(TRACE, "iovar cmd=%s\n", (char *)dcmd->buf);

		goto done;
	}

	prot->pending = true;
	prot->lastcmd = dcmd->cmd;
	if (dcmd->set)
		ret = brcmf_proto_cdc_set_dcmd(drvr, ifidx, dcmd->cmd,
						   dcmd->buf, len);
	else {
		ret = brcmf_proto_cdc_query_dcmd(drvr, ifidx, dcmd->cmd,
						     dcmd->buf, len);
		if (ret > 0)
			dcmd->used = ret -
					sizeof(struct brcmf_proto_cdc_dcmd);
	}

	if (ret >= 0)
		ret = 0;
	else {
		struct brcmf_proto_cdc_dcmd *msg = &prot->msg;
		/* len == needed when set/query fails from dongle */
		dcmd->needed = le32_to_cpu(msg->len);
	}

	/* Intercept the wme_dp dongle cmd here */
	if (!ret && dcmd->cmd == BRCMF_C_SET_VAR &&
	    !strcmp(dcmd->buf, "wme_dp")) {
		int slen;
		__le32 val = 0;

		slen = strlen("wme_dp") + 1;
		if (len >= (int)(slen + sizeof(int)))
			memcpy(&val, (char *)dcmd->buf + slen, sizeof(int));
		drvr->wme_dp = (u8) le32_to_cpu(val);
	}

	prot->pending = false;

done:
	mutex_unlock(&drvr->proto_block);

	return ret;
}

=======
>>>>>>> refs/remotes/origin/master
static bool pkt_sum_needed(struct sk_buff *skb)
{
	return skb->ip_summed == CHECKSUM_PARTIAL;
}

static void pkt_set_sum_good(struct sk_buff *skb, bool x)
{
	skb->ip_summed = (x ? CHECKSUM_UNNECESSARY : CHECKSUM_NONE);
}

<<<<<<< HEAD
void brcmf_proto_hdrpush(struct brcmf_pub *drvr, int ifidx,
=======
void brcmf_proto_hdrpush(struct brcmf_pub *drvr, int ifidx, u8 offset,
>>>>>>> refs/remotes/origin/master
			 struct sk_buff *pktbuf)
{
	struct brcmf_proto_bdc_header *h;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "Enter\n");

	/* Push BDC header used to convey priority for buses that don't */

=======
	brcmf_dbg(CDC, "Enter\n");

	/* Push BDC header used to convey priority for buses that don't */
>>>>>>> refs/remotes/origin/master
	skb_push(pktbuf, BDC_HEADER_LEN);

	h = (struct brcmf_proto_bdc_header *)(pktbuf->data);

	h->flags = (BDC_PROTO_VER << BDC_FLAG_VER_SHIFT);
	if (pkt_sum_needed(pktbuf))
		h->flags |= BDC_FLAG_SUM_NEEDED;

	h->priority = (pktbuf->priority & BDC_PRIORITY_MASK);
	h->flags2 = 0;
<<<<<<< HEAD
	h->data_offset = 0;
	BDC_SET_IF_IDX(h, ifidx);
}

int brcmf_proto_hdrpull(struct device *dev, int *ifidx,
			struct sk_buff *pktbuf)
{
	struct brcmf_proto_bdc_header *h;
	struct brcmf_bus *bus_if = dev_get_drvdata(dev);
	struct brcmf_pub *drvr = bus_if->drvr;

	brcmf_dbg(TRACE, "Enter\n");

	/* Pop BDC header used to convey priority for buses that don't */

	if (pktbuf->len < BDC_HEADER_LEN) {
		brcmf_dbg(ERROR, "rx data too short (%d < %d)\n",
=======
	h->data_offset = offset;
	BDC_SET_IF_IDX(h, ifidx);
	trace_brcmf_bdchdr(pktbuf->data);
}

int brcmf_proto_hdrpull(struct brcmf_pub *drvr, bool do_fws, u8 *ifidx,
			struct sk_buff *pktbuf)
{
	struct brcmf_proto_bdc_header *h;

	brcmf_dbg(CDC, "Enter\n");

	/* Pop BDC header used to convey priority for buses that don't */

	if (pktbuf->len <= BDC_HEADER_LEN) {
		brcmf_dbg(INFO, "rx data too short (%d <= %d)\n",
>>>>>>> refs/remotes/origin/master
			  pktbuf->len, BDC_HEADER_LEN);
		return -EBADE;
	}

<<<<<<< HEAD
=======
	trace_brcmf_bdchdr(pktbuf->data);
>>>>>>> refs/remotes/origin/master
	h = (struct brcmf_proto_bdc_header *)(pktbuf->data);

	*ifidx = BDC_GET_IF_IDX(h);
	if (*ifidx >= BRCMF_MAX_IFS) {
<<<<<<< HEAD
		brcmf_dbg(ERROR, "rx data ifnum out of range (%d)\n", *ifidx);
		return -EBADE;
	}

	if (((h->flags & BDC_FLAG_VER_MASK) >> BDC_FLAG_VER_SHIFT) !=
	    BDC_PROTO_VER) {
		brcmf_dbg(ERROR, "%s: non-BDC packet received, flags 0x%x\n",
=======
		brcmf_err("rx data ifnum out of range (%d)\n", *ifidx);
		return -EBADE;
	}
	/* The ifidx is the idx to map to matching netdev/ifp. When receiving
	 * events this is easy because it contains the bssidx which maps
	 * 1-on-1 to the netdev/ifp. But for data frames the ifidx is rcvd.
	 * bssidx 1 is used for p2p0 and no data can be received or
	 * transmitted on it. Therefor bssidx is ifidx + 1 if ifidx > 0
	 */
	if (*ifidx)
		(*ifidx)++;

	if (((h->flags & BDC_FLAG_VER_MASK) >> BDC_FLAG_VER_SHIFT) !=
	    BDC_PROTO_VER) {
		brcmf_err("%s: non-BDC packet received, flags 0x%x\n",
>>>>>>> refs/remotes/origin/master
			  brcmf_ifname(drvr, *ifidx), h->flags);
		return -EBADE;
	}

	if (h->flags & BDC_FLAG_SUM_GOOD) {
<<<<<<< HEAD
		brcmf_dbg(INFO, "%s: BDC packet received with good rx-csum, flags 0x%x\n",
=======
		brcmf_dbg(CDC, "%s: BDC rcv, good checksum, flags 0x%x\n",
>>>>>>> refs/remotes/origin/master
			  brcmf_ifname(drvr, *ifidx), h->flags);
		pkt_set_sum_good(pktbuf, true);
	}

	pktbuf->priority = h->priority & BDC_PRIORITY_MASK;

	skb_pull(pktbuf, BDC_HEADER_LEN);
<<<<<<< HEAD

=======
	if (do_fws)
		brcmf_fws_hdrpull(drvr, *ifidx, h->data_offset << 2, pktbuf);
	else
		skb_pull(pktbuf, h->data_offset << 2);

	if (pktbuf->len == 0)
		return -ENODATA;
>>>>>>> refs/remotes/origin/master
	return 0;
}

int brcmf_proto_attach(struct brcmf_pub *drvr)
{
	struct brcmf_proto *cdc;

	cdc = kzalloc(sizeof(struct brcmf_proto), GFP_ATOMIC);
	if (!cdc)
		goto fail;

	/* ensure that the msg buf directly follows the cdc msg struct */
	if ((unsigned long)(&cdc->msg + 1) != (unsigned long)cdc->buf) {
<<<<<<< HEAD
		brcmf_dbg(ERROR, "struct brcmf_proto is not correctly defined\n");
=======
		brcmf_err("struct brcmf_proto is not correctly defined\n");
>>>>>>> refs/remotes/origin/master
		goto fail;
	}

	drvr->prot = cdc;
<<<<<<< HEAD
	drvr->hdrlen += BDC_HEADER_LEN;
=======
	drvr->hdrlen += BDC_HEADER_LEN + BRCMF_PROT_FW_SIGNAL_MAX_TXBYTES;
>>>>>>> refs/remotes/origin/master
	drvr->bus_if->maxctl = BRCMF_DCMD_MAXLEN +
			sizeof(struct brcmf_proto_cdc_dcmd) + ROUND_UP_MARGIN;
	return 0;

fail:
	kfree(cdc);
	return -ENOMEM;
}

/* ~NOTE~ What if another thread is waiting on the semaphore?  Holding it? */
void brcmf_proto_detach(struct brcmf_pub *drvr)
{
	kfree(drvr->prot);
	drvr->prot = NULL;
}

<<<<<<< HEAD
int brcmf_proto_init(struct brcmf_pub *drvr)
{
	int ret = 0;
	char buf[128];

	brcmf_dbg(TRACE, "Enter\n");

	mutex_lock(&drvr->proto_block);

	/* Get the device MAC address */
	strcpy(buf, "cur_etheraddr");
	ret = brcmf_proto_cdc_query_dcmd(drvr, 0, BRCMF_C_GET_VAR,
					  buf, sizeof(buf));
	if (ret < 0) {
		mutex_unlock(&drvr->proto_block);
		return ret;
	}
	memcpy(drvr->mac, buf, ETH_ALEN);

	mutex_unlock(&drvr->proto_block);

	ret = brcmf_c_preinit_dcmds(drvr);

	/* Always assumes wl for now */
	drvr->iswl = true;

	return ret;
}

=======
>>>>>>> refs/remotes/origin/master
void brcmf_proto_stop(struct brcmf_pub *drvr)
{
	/* Nothing to do for CDC */
}
