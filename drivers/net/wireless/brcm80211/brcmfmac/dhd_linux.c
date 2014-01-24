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

<<<<<<< HEAD
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/mmc/sdio_func.h>
#include <linux/random.h>
#include <linux/spinlock.h>
#include <linux/ethtool.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/hardirq.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/module.h>
#include <net/cfg80211.h>
#include <net/rtnetlink.h>
#include <defs.h>
=======
#include <linux/kernel.h>
#include <linux/etherdevice.h>
#include <linux/module.h>
#include <net/cfg80211.h>
#include <net/rtnetlink.h>
>>>>>>> refs/remotes/origin/master
#include <brcmu_utils.h>
#include <brcmu_wifi.h>

#include "dhd.h"
#include "dhd_bus.h"
#include "dhd_proto.h"
#include "dhd_dbg.h"
<<<<<<< HEAD
#include "wl_cfg80211.h"

MODULE_AUTHOR("Broadcom Corporation");
MODULE_DESCRIPTION("Broadcom 802.11n wireless LAN fullmac driver.");
MODULE_SUPPORTED_DEVICE("Broadcom 802.11n WLAN fullmac cards");
MODULE_LICENSE("Dual BSD/GPL");


/* Interface control information */
struct brcmf_if {
	struct brcmf_pub *drvr;	/* back pointer to brcmf_pub */
	/* OS/stack specifics */
	struct net_device *ndev;
	struct net_device_stats stats;
	int idx;		/* iface idx in dongle */
	u8 mac_addr[ETH_ALEN];	/* assigned MAC address */
};

/* Error bits */
int brcmf_msg_level = BRCMF_ERROR_VAL;
module_param(brcmf_msg_level, int, 0);

int brcmf_ifname2idx(struct brcmf_pub *drvr, char *name)
{
	int i = BRCMF_MAX_IFS;
	struct brcmf_if *ifp;

	if (name == NULL || *name == '\0')
		return 0;

	while (--i > 0) {
		ifp = drvr->iflist[i];
		if (ifp && !strncmp(ifp->ndev->name, name, IFNAMSIZ))
			break;
	}

	brcmf_dbg(TRACE, "return idx %d for \"%s\"\n", i, name);

	return i;		/* default - the primary interface */
}
=======
#include "fwil_types.h"
#include "p2p.h"
#include "wl_cfg80211.h"
#include "fwil.h"
#include "fwsignal.h"

MODULE_AUTHOR("Broadcom Corporation");
MODULE_DESCRIPTION("Broadcom 802.11 wireless LAN fullmac driver.");
MODULE_LICENSE("Dual BSD/GPL");

#define MAX_WAIT_FOR_8021X_TX		50	/* msecs */

/* AMPDU rx reordering definitions */
#define BRCMF_RXREORDER_FLOWID_OFFSET		0
#define BRCMF_RXREORDER_MAXIDX_OFFSET		2
#define BRCMF_RXREORDER_FLAGS_OFFSET		4
#define BRCMF_RXREORDER_CURIDX_OFFSET		6
#define BRCMF_RXREORDER_EXPIDX_OFFSET		8

#define BRCMF_RXREORDER_DEL_FLOW		0x01
#define BRCMF_RXREORDER_FLUSH_ALL		0x02
#define BRCMF_RXREORDER_CURIDX_VALID		0x04
#define BRCMF_RXREORDER_EXPIDX_VALID		0x08
#define BRCMF_RXREORDER_NEW_HOLE		0x10

/* Error bits */
int brcmf_msg_level;
module_param_named(debug, brcmf_msg_level, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(debug, "level of debug output");

/* P2P0 enable */
static int brcmf_p2p_enable;
#ifdef CONFIG_BRCMDBG
module_param_named(p2pon, brcmf_p2p_enable, int, 0);
MODULE_PARM_DESC(p2pon, "enable p2p management functionality");
#endif
>>>>>>> refs/remotes/origin/master

char *brcmf_ifname(struct brcmf_pub *drvr, int ifidx)
{
	if (ifidx < 0 || ifidx >= BRCMF_MAX_IFS) {
<<<<<<< HEAD
		brcmf_dbg(ERROR, "ifidx %d out of range\n", ifidx);
=======
		brcmf_err("ifidx %d out of range\n", ifidx);
>>>>>>> refs/remotes/origin/master
		return "<if_bad>";
	}

	if (drvr->iflist[ifidx] == NULL) {
<<<<<<< HEAD
		brcmf_dbg(ERROR, "null i/f %d\n", ifidx);
=======
		brcmf_err("null i/f %d\n", ifidx);
>>>>>>> refs/remotes/origin/master
		return "<if_null>";
	}

	if (drvr->iflist[ifidx]->ndev)
		return drvr->iflist[ifidx]->ndev->name;

	return "<if_none>";
}

static void _brcmf_set_multicast_list(struct work_struct *work)
{
<<<<<<< HEAD
	struct net_device *ndev;
	struct netdev_hw_addr *ha;
	u32 dcmd_value, cnt;
	__le32 cnt_le;
	__le32 dcmd_le_value;

	struct brcmf_dcmd dcmd;
	char *buf, *bufp;
	uint buflen;
	int ret;

	struct brcmf_pub *drvr = container_of(work, struct brcmf_pub,
						    multicast_work);

	ndev = drvr->iflist[0]->ndev;
	cnt = netdev_mc_count(ndev);

	/* Determine initial value of allmulti flag */
	dcmd_value = (ndev->flags & IFF_ALLMULTI) ? true : false;

	/* Send down the multicast list first. */

	buflen = sizeof("mcast_list") + sizeof(cnt) + (cnt * ETH_ALEN);
	bufp = buf = kmalloc(buflen, GFP_ATOMIC);
	if (!bufp)
		return;

	strcpy(bufp, "mcast_list");
	bufp += strlen("mcast_list") + 1;

	cnt_le = cpu_to_le32(cnt);
	memcpy(bufp, &cnt_le, sizeof(cnt));
=======
	struct brcmf_if *ifp;
	struct net_device *ndev;
	struct netdev_hw_addr *ha;
	u32 cmd_value, cnt;
	__le32 cnt_le;
	char *buf, *bufp;
	u32 buflen;
	s32 err;

	ifp = container_of(work, struct brcmf_if, multicast_work);

	brcmf_dbg(TRACE, "Enter, idx=%d\n", ifp->bssidx);

	ndev = ifp->ndev;

	/* Determine initial value of allmulti flag */
	cmd_value = (ndev->flags & IFF_ALLMULTI) ? true : false;

	/* Send down the multicast list first. */
	cnt = netdev_mc_count(ndev);
	buflen = sizeof(cnt) + (cnt * ETH_ALEN);
	buf = kmalloc(buflen, GFP_ATOMIC);
	if (!buf)
		return;
	bufp = buf;

	cnt_le = cpu_to_le32(cnt);
	memcpy(bufp, &cnt_le, sizeof(cnt_le));
>>>>>>> refs/remotes/origin/master
	bufp += sizeof(cnt_le);

	netdev_for_each_mc_addr(ha, ndev) {
		if (!cnt)
			break;
		memcpy(bufp, ha->addr, ETH_ALEN);
		bufp += ETH_ALEN;
		cnt--;
	}

<<<<<<< HEAD
	memset(&dcmd, 0, sizeof(dcmd));
	dcmd.cmd = BRCMF_C_SET_VAR;
	dcmd.buf = buf;
	dcmd.len = buflen;
	dcmd.set = true;

	ret = brcmf_proto_dcmd(drvr, 0, &dcmd, dcmd.len);
	if (ret < 0) {
		brcmf_dbg(ERROR, "%s: set mcast_list failed, cnt %d\n",
			  brcmf_ifname(drvr, 0), cnt);
		dcmd_value = cnt ? true : dcmd_value;
=======
	err = brcmf_fil_iovar_data_set(ifp, "mcast_list", buf, buflen);
	if (err < 0) {
		brcmf_err("Setting mcast_list failed, %d\n", err);
		cmd_value = cnt ? true : cmd_value;
>>>>>>> refs/remotes/origin/master
	}

	kfree(buf);

<<<<<<< HEAD
	/* Now send the allmulti setting.  This is based on the setting in the
	 * net_device flags, but might be modified above to be turned on if we
	 * were trying to set some addresses and dongle rejected it...
	 */

	buflen = sizeof("allmulti") + sizeof(dcmd_value);
	buf = kmalloc(buflen, GFP_ATOMIC);
	if (!buf)
		return;

	dcmd_le_value = cpu_to_le32(dcmd_value);

	if (!brcmf_c_mkiovar
	    ("allmulti", (void *)&dcmd_le_value,
	    sizeof(dcmd_le_value), buf, buflen)) {
		brcmf_dbg(ERROR, "%s: mkiovar failed for allmulti, datalen %d buflen %u\n",
			  brcmf_ifname(drvr, 0),
			  (int)sizeof(dcmd_value), buflen);
		kfree(buf);
		return;
	}

	memset(&dcmd, 0, sizeof(dcmd));
	dcmd.cmd = BRCMF_C_SET_VAR;
	dcmd.buf = buf;
	dcmd.len = buflen;
	dcmd.set = true;

	ret = brcmf_proto_dcmd(drvr, 0, &dcmd, dcmd.len);
	if (ret < 0) {
		brcmf_dbg(ERROR, "%s: set allmulti %d failed\n",
			  brcmf_ifname(drvr, 0),
			  le32_to_cpu(dcmd_le_value));
	}

	kfree(buf);

	/* Finally, pick up the PROMISC flag as well, like the NIC
		 driver does */

	dcmd_value = (ndev->flags & IFF_PROMISC) ? true : false;
	dcmd_le_value = cpu_to_le32(dcmd_value);

	memset(&dcmd, 0, sizeof(dcmd));
	dcmd.cmd = BRCMF_C_SET_PROMISC;
	dcmd.buf = &dcmd_le_value;
	dcmd.len = sizeof(dcmd_le_value);
	dcmd.set = true;

	ret = brcmf_proto_dcmd(drvr, 0, &dcmd, dcmd.len);
	if (ret < 0) {
		brcmf_dbg(ERROR, "%s: set promisc %d failed\n",
			  brcmf_ifname(drvr, 0),
			  le32_to_cpu(dcmd_le_value));
	}
=======
	/*
	 * Now send the allmulti setting.  This is based on the setting in the
	 * net_device flags, but might be modified above to be turned on if we
	 * were trying to set some addresses and dongle rejected it...
	 */
	err = brcmf_fil_iovar_int_set(ifp, "allmulti", cmd_value);
	if (err < 0)
		brcmf_err("Setting allmulti failed, %d\n", err);

	/*Finally, pick up the PROMISC flag */
	cmd_value = (ndev->flags & IFF_PROMISC) ? true : false;
	err = brcmf_fil_cmd_int_set(ifp, BRCMF_C_SET_PROMISC, cmd_value);
	if (err < 0)
		brcmf_err("Setting BRCMF_C_SET_PROMISC failed, %d\n",
			  err);
>>>>>>> refs/remotes/origin/master
}

static void
_brcmf_set_mac_address(struct work_struct *work)
{
<<<<<<< HEAD
	char buf[32];
	struct brcmf_dcmd dcmd;
	int ret;

	struct brcmf_pub *drvr = container_of(work, struct brcmf_pub,
						    setmacaddr_work);

	brcmf_dbg(TRACE, "enter\n");
	if (!brcmf_c_mkiovar("cur_etheraddr", (char *)drvr->macvalue,
			   ETH_ALEN, buf, 32)) {
		brcmf_dbg(ERROR, "%s: mkiovar failed for cur_etheraddr\n",
			  brcmf_ifname(drvr, 0));
		return;
	}
	memset(&dcmd, 0, sizeof(dcmd));
	dcmd.cmd = BRCMF_C_SET_VAR;
	dcmd.buf = buf;
	dcmd.len = 32;
	dcmd.set = true;

	ret = brcmf_proto_dcmd(drvr, 0, &dcmd, dcmd.len);
	if (ret < 0)
		brcmf_dbg(ERROR, "%s: set cur_etheraddr failed\n",
			  brcmf_ifname(drvr, 0));
	else
		memcpy(drvr->iflist[0]->ndev->dev_addr,
		       drvr->macvalue, ETH_ALEN);

	return;
=======
	struct brcmf_if *ifp;
	s32 err;

	ifp = container_of(work, struct brcmf_if, setmacaddr_work);

	brcmf_dbg(TRACE, "Enter, idx=%d\n", ifp->bssidx);

	err = brcmf_fil_iovar_data_set(ifp, "cur_etheraddr", ifp->mac_addr,
				       ETH_ALEN);
	if (err < 0) {
		brcmf_err("Setting cur_etheraddr failed, %d\n", err);
	} else {
		brcmf_dbg(TRACE, "MAC address updated to %pM\n",
			  ifp->mac_addr);
		memcpy(ifp->ndev->dev_addr, ifp->mac_addr, ETH_ALEN);
	}
>>>>>>> refs/remotes/origin/master
}

static int brcmf_netdev_set_mac_address(struct net_device *ndev, void *addr)
{
	struct brcmf_if *ifp = netdev_priv(ndev);
<<<<<<< HEAD
	struct brcmf_pub *drvr = ifp->drvr;
	struct sockaddr *sa = (struct sockaddr *)addr;

	memcpy(&drvr->macvalue, sa->sa_data, ETH_ALEN);
	schedule_work(&drvr->setmacaddr_work);
=======
	struct sockaddr *sa = (struct sockaddr *)addr;

	memcpy(&ifp->mac_addr, sa->sa_data, ETH_ALEN);
	schedule_work(&ifp->setmacaddr_work);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static void brcmf_netdev_set_multicast_list(struct net_device *ndev)
{
	struct brcmf_if *ifp = netdev_priv(ndev);
<<<<<<< HEAD
	struct brcmf_pub *drvr = ifp->drvr;

	schedule_work(&drvr->multicast_work);
}

int brcmf_sendpkt(struct brcmf_pub *drvr, int ifidx, struct sk_buff *pktbuf)
{
	/* Reject if down */
	if (!drvr->bus_if->drvr_up || (drvr->bus_if->state == BRCMF_BUS_DOWN))
		return -ENODEV;

	/* Update multicast statistic */
	if (pktbuf->len >= ETH_ALEN) {
		u8 *pktdata = (u8 *) (pktbuf->data);
		struct ethhdr *eh = (struct ethhdr *)pktdata;

		if (is_multicast_ether_addr(eh->h_dest))
			drvr->tx_multicast++;
		if (ntohs(eh->h_proto) == ETH_P_PAE)
			atomic_inc(&drvr->pend_8021x_cnt);
	}

	/* If the protocol uses a data header, apply it */
	brcmf_proto_hdrpush(drvr, ifidx, pktbuf);

	/* Use bus module to send data frame */
	return drvr->bus_if->brcmf_bus_txdata(drvr->dev, pktbuf);
}

static int brcmf_netdev_start_xmit(struct sk_buff *skb, struct net_device *ndev)
=======

	schedule_work(&ifp->multicast_work);
}

static netdev_tx_t brcmf_netdev_start_xmit(struct sk_buff *skb,
					   struct net_device *ndev)
>>>>>>> refs/remotes/origin/master
{
	int ret;
	struct brcmf_if *ifp = netdev_priv(ndev);
	struct brcmf_pub *drvr = ifp->drvr;
<<<<<<< HEAD

	brcmf_dbg(TRACE, "Enter\n");

	/* Reject if down */
	if (!drvr->bus_if->drvr_up ||
	    (drvr->bus_if->state == BRCMF_BUS_DOWN)) {
		brcmf_dbg(ERROR, "xmit rejected drvup=%d state=%d\n",
			  drvr->bus_if->drvr_up,
			  drvr->bus_if->state);
		netif_stop_queue(ndev);
		return -ENODEV;
	}

	if (!drvr->iflist[ifp->idx]) {
		brcmf_dbg(ERROR, "bad ifidx %d\n", ifp->idx);
		netif_stop_queue(ndev);
		return -ENODEV;
=======
	struct ethhdr *eh;

	brcmf_dbg(DATA, "Enter, idx=%d\n", ifp->bssidx);

	/* Can the device send data? */
	if (drvr->bus_if->state != BRCMF_BUS_DATA) {
		brcmf_err("xmit rejected state=%d\n", drvr->bus_if->state);
		netif_stop_queue(ndev);
		dev_kfree_skb(skb);
		ret = -ENODEV;
		goto done;
	}

	if (!drvr->iflist[ifp->bssidx]) {
		brcmf_err("bad ifidx %d\n", ifp->bssidx);
		netif_stop_queue(ndev);
		dev_kfree_skb(skb);
		ret = -ENODEV;
		goto done;
>>>>>>> refs/remotes/origin/master
	}

	/* Make sure there's enough room for any header */
	if (skb_headroom(skb) < drvr->hdrlen) {
		struct sk_buff *skb2;

		brcmf_dbg(INFO, "%s: insufficient headroom\n",
<<<<<<< HEAD
			  brcmf_ifname(drvr, ifp->idx));
=======
			  brcmf_ifname(drvr, ifp->bssidx));
>>>>>>> refs/remotes/origin/master
		drvr->bus_if->tx_realloc++;
		skb2 = skb_realloc_headroom(skb, drvr->hdrlen);
		dev_kfree_skb(skb);
		skb = skb2;
		if (skb == NULL) {
<<<<<<< HEAD
			brcmf_dbg(ERROR, "%s: skb_realloc_headroom failed\n",
				  brcmf_ifname(drvr, ifp->idx));
=======
			brcmf_err("%s: skb_realloc_headroom failed\n",
				  brcmf_ifname(drvr, ifp->bssidx));
>>>>>>> refs/remotes/origin/master
			ret = -ENOMEM;
			goto done;
		}
	}

<<<<<<< HEAD
	ret = brcmf_sendpkt(drvr, ifp->idx, skb);

done:
	if (ret)
		drvr->bus_if->dstats.tx_dropped++;
	else
		drvr->bus_if->dstats.tx_packets++;

	/* Return ok: we always eat the packet */
	return 0;
}

void brcmf_txflowcontrol(struct device *dev, int ifidx, bool state)
{
	struct net_device *ndev;
=======
	/* validate length for ether packet */
	if (skb->len < sizeof(*eh)) {
		ret = -EINVAL;
		dev_kfree_skb(skb);
		goto done;
	}

	ret = brcmf_fws_process_skb(ifp, skb);

done:
	if (ret) {
		ifp->stats.tx_dropped++;
	} else {
		ifp->stats.tx_packets++;
		ifp->stats.tx_bytes += skb->len;
	}

	/* Return ok: we always eat the packet */
	return NETDEV_TX_OK;
}

void brcmf_txflowblock_if(struct brcmf_if *ifp,
			  enum brcmf_netif_stop_reason reason, bool state)
{
	unsigned long flags;

	if (!ifp || !ifp->ndev)
		return;

	brcmf_dbg(TRACE, "enter: idx=%d stop=0x%X reason=%d state=%d\n",
		  ifp->bssidx, ifp->netif_stop, reason, state);

	spin_lock_irqsave(&ifp->netif_stop_lock, flags);
	if (state) {
		if (!ifp->netif_stop)
			netif_stop_queue(ifp->ndev);
		ifp->netif_stop |= reason;
	} else {
		ifp->netif_stop &= ~reason;
		if (!ifp->netif_stop)
			netif_wake_queue(ifp->ndev);
	}
	spin_unlock_irqrestore(&ifp->netif_stop_lock, flags);
}

void brcmf_txflowblock(struct device *dev, bool state)
{
>>>>>>> refs/remotes/origin/master
	struct brcmf_bus *bus_if = dev_get_drvdata(dev);
	struct brcmf_pub *drvr = bus_if->drvr;

	brcmf_dbg(TRACE, "Enter\n");

<<<<<<< HEAD
	ndev = drvr->iflist[ifidx]->ndev;
	if (state == ON)
		netif_stop_queue(ndev);
	else
		netif_wake_queue(ndev);
}

static int brcmf_host_event(struct brcmf_pub *drvr, int *ifidx,
			    void *pktdata, struct brcmf_event_msg *event,
			    void **data)
{
	int bcmerror = 0;

	bcmerror = brcmf_c_host_event(drvr, ifidx, pktdata, event, data);
	if (bcmerror != 0)
		return bcmerror;

	if (drvr->iflist[*ifidx]->ndev)
		brcmf_cfg80211_event(drvr->iflist[*ifidx]->ndev,
				     event, *data);

	return bcmerror;
}

void brcmf_rx_frame(struct device *dev, int ifidx,
		    struct sk_buff_head *skb_list)
{
	unsigned char *eth;
	uint len;
	void *data;
	struct sk_buff *skb, *pnext;
	struct brcmf_if *ifp;
	struct brcmf_event_msg event;
	struct brcmf_bus *bus_if = dev_get_drvdata(dev);
	struct brcmf_pub *drvr = bus_if->drvr;

	brcmf_dbg(TRACE, "Enter\n");

	skb_queue_walk_safe(skb_list, skb, pnext) {
		skb_unlink(skb, skb_list);

		/* Get the protocol, maintain skb around eth_type_trans()
		 * The main reason for this hack is for the limitation of
		 * Linux 2.4 where 'eth_type_trans' uses the
		 * 'net->hard_header_len'
		 * to perform skb_pull inside vs ETH_HLEN. Since to avoid
		 * coping of the packet coming from the network stack to add
		 * BDC, Hardware header etc, during network interface
		 * registration
		 * we set the 'net->hard_header_len' to ETH_HLEN + extra space
		 * required
		 * for BDC, Hardware header etc. and not just the ETH_HLEN
		 */
		eth = skb->data;
		len = skb->len;

		ifp = drvr->iflist[ifidx];
		if (ifp == NULL)
			ifp = drvr->iflist[0];

		if (!ifp || !ifp->ndev ||
		    ifp->ndev->reg_state != NETREG_REGISTERED) {
			brcmu_pkt_buf_free_skb(skb);
			continue;
		}

		skb->dev = ifp->ndev;
		skb->protocol = eth_type_trans(skb, skb->dev);

		if (skb->pkt_type == PACKET_MULTICAST)
			bus_if->dstats.multicast++;

		skb->data = eth;
		skb->len = len;

		/* Strip header, count, deliver upward */
		skb_pull(skb, ETH_HLEN);

		/* Process special event packets and then discard them */
		if (ntohs(skb->protocol) == ETH_P_LINK_CTL)
			brcmf_host_event(drvr, &ifidx,
					  skb_mac_header(skb),
					  &event, &data);

		if (drvr->iflist[ifidx]) {
			ifp = drvr->iflist[ifidx];
			ifp->ndev->last_rx = jiffies;
		}

		bus_if->dstats.rx_bytes += skb->len;
		bus_if->dstats.rx_packets++;	/* Local count */

		if (in_interrupt())
			netif_rx(skb);
		else
			/* If the receive is not processed inside an ISR,
			 * the softirqd must be woken explicitly to service
			 * the NET_RX_SOFTIRQ.  In 2.6 kernels, this is handled
			 * by netif_rx_ni(), but in earlier kernels, we need
			 * to do it manually.
			 */
			netif_rx_ni(skb);
	}
}

void brcmf_txcomplete(struct device *dev, struct sk_buff *txp, bool success)
{
	uint ifidx;
	struct ethhdr *eh;
	u16 type;
	struct brcmf_bus *bus_if = dev_get_drvdata(dev);
	struct brcmf_pub *drvr = bus_if->drvr;

	brcmf_proto_hdrpull(dev, &ifidx, txp);

	eh = (struct ethhdr *)(txp->data);
	type = ntohs(eh->h_proto);

	if (type == ETH_P_PAE)
		atomic_dec(&drvr->pend_8021x_cnt);

}

static struct net_device_stats *brcmf_netdev_get_stats(struct net_device *ndev)
{
	struct brcmf_if *ifp = netdev_priv(ndev);
	struct brcmf_bus *bus_if = ifp->drvr->bus_if;

	brcmf_dbg(TRACE, "Enter\n");

	/* Copy dongle stats to net device stats */
	ifp->stats.rx_packets = bus_if->dstats.rx_packets;
	ifp->stats.tx_packets = bus_if->dstats.tx_packets;
	ifp->stats.rx_bytes = bus_if->dstats.rx_bytes;
	ifp->stats.tx_bytes = bus_if->dstats.tx_bytes;
	ifp->stats.rx_errors = bus_if->dstats.rx_errors;
	ifp->stats.tx_errors = bus_if->dstats.tx_errors;
	ifp->stats.rx_dropped = bus_if->dstats.rx_dropped;
	ifp->stats.tx_dropped = bus_if->dstats.tx_dropped;
	ifp->stats.multicast = bus_if->dstats.multicast;

	return &ifp->stats;
}

/* Retrieve current toe component enables, which are kept
	 as a bitmap in toe_ol iovar */
static int brcmf_toe_get(struct brcmf_pub *drvr, int ifidx, u32 *toe_ol)
{
	struct brcmf_dcmd dcmd;
	__le32 toe_le;
	char buf[32];
	int ret;

	memset(&dcmd, 0, sizeof(dcmd));

	dcmd.cmd = BRCMF_C_GET_VAR;
	dcmd.buf = buf;
	dcmd.len = (uint) sizeof(buf);
	dcmd.set = false;

	strcpy(buf, "toe_ol");
	ret = brcmf_proto_dcmd(drvr, ifidx, &dcmd, dcmd.len);
	if (ret < 0) {
		/* Check for older dongle image that doesn't support toe_ol */
		if (ret == -EIO) {
			brcmf_dbg(ERROR, "%s: toe not supported by device\n",
				  brcmf_ifname(drvr, ifidx));
			return -EOPNOTSUPP;
		}

		brcmf_dbg(INFO, "%s: could not get toe_ol: ret=%d\n",
			  brcmf_ifname(drvr, ifidx), ret);
		return ret;
	}

	memcpy(&toe_le, buf, sizeof(u32));
	*toe_ol = le32_to_cpu(toe_le);
	return 0;
}

/* Set current toe component enables in toe_ol iovar,
	 and set toe global enable iovar */
static int brcmf_toe_set(struct brcmf_pub *drvr, int ifidx, u32 toe_ol)
{
	struct brcmf_dcmd dcmd;
	char buf[32];
	int ret;
	__le32 toe_le = cpu_to_le32(toe_ol);

	memset(&dcmd, 0, sizeof(dcmd));

	dcmd.cmd = BRCMF_C_SET_VAR;
	dcmd.buf = buf;
	dcmd.len = (uint) sizeof(buf);
	dcmd.set = true;

	/* Set toe_ol as requested */
	strcpy(buf, "toe_ol");
	memcpy(&buf[sizeof("toe_ol")], &toe_le, sizeof(u32));

	ret = brcmf_proto_dcmd(drvr, ifidx, &dcmd, dcmd.len);
	if (ret < 0) {
		brcmf_dbg(ERROR, "%s: could not set toe_ol: ret=%d\n",
			  brcmf_ifname(drvr, ifidx), ret);
		return ret;
	}

	/* Enable toe globally only if any components are enabled. */
	toe_le = cpu_to_le32(toe_ol != 0);

	strcpy(buf, "toe");
	memcpy(&buf[sizeof("toe")], &toe_le, sizeof(u32));

	ret = brcmf_proto_dcmd(drvr, ifidx, &dcmd, dcmd.len);
	if (ret < 0) {
		brcmf_dbg(ERROR, "%s: could not set toe: ret=%d\n",
			  brcmf_ifname(drvr, ifidx), ret);
		return ret;
	}

	return 0;
=======
	brcmf_fws_bus_blocked(drvr, state);
}

static void brcmf_netif_rx(struct brcmf_if *ifp, struct sk_buff *skb)
{
	skb->dev = ifp->ndev;
	skb->protocol = eth_type_trans(skb, skb->dev);

	if (skb->pkt_type == PACKET_MULTICAST)
		ifp->stats.multicast++;

	/* Process special event packets */
	brcmf_fweh_process_skb(ifp->drvr, skb);

	if (!(ifp->ndev->flags & IFF_UP)) {
		brcmu_pkt_buf_free_skb(skb);
		return;
	}

	ifp->stats.rx_bytes += skb->len;
	ifp->stats.rx_packets++;

	brcmf_dbg(DATA, "rx proto=0x%X\n", ntohs(skb->protocol));
	if (in_interrupt())
		netif_rx(skb);
	else
		/* If the receive is not processed inside an ISR,
		 * the softirqd must be woken explicitly to service
		 * the NET_RX_SOFTIRQ.  This is handled by netif_rx_ni().
		 */
		netif_rx_ni(skb);
}

static void brcmf_rxreorder_get_skb_list(struct brcmf_ampdu_rx_reorder *rfi,
					 u8 start, u8 end,
					 struct sk_buff_head *skb_list)
{
	/* initialize return list */
	__skb_queue_head_init(skb_list);

	if (rfi->pend_pkts == 0) {
		brcmf_dbg(INFO, "no packets in reorder queue\n");
		return;
	}

	do {
		if (rfi->pktslots[start]) {
			__skb_queue_tail(skb_list, rfi->pktslots[start]);
			rfi->pktslots[start] = NULL;
		}
		start++;
		if (start > rfi->max_idx)
			start = 0;
	} while (start != end);
	rfi->pend_pkts -= skb_queue_len(skb_list);
}

static void brcmf_rxreorder_process_info(struct brcmf_if *ifp, u8 *reorder_data,
					 struct sk_buff *pkt)
{
	u8 flow_id, max_idx, cur_idx, exp_idx, end_idx;
	struct brcmf_ampdu_rx_reorder *rfi;
	struct sk_buff_head reorder_list;
	struct sk_buff *pnext;
	u8 flags;
	u32 buf_size;

	flow_id = reorder_data[BRCMF_RXREORDER_FLOWID_OFFSET];
	flags = reorder_data[BRCMF_RXREORDER_FLAGS_OFFSET];

	/* validate flags and flow id */
	if (flags == 0xFF) {
		brcmf_err("invalid flags...so ignore this packet\n");
		brcmf_netif_rx(ifp, pkt);
		return;
	}

	rfi = ifp->drvr->reorder_flows[flow_id];
	if (flags & BRCMF_RXREORDER_DEL_FLOW) {
		brcmf_dbg(INFO, "flow-%d: delete\n",
			  flow_id);

		if (rfi == NULL) {
			brcmf_dbg(INFO, "received flags to cleanup, but no flow (%d) yet\n",
				  flow_id);
			brcmf_netif_rx(ifp, pkt);
			return;
		}

		brcmf_rxreorder_get_skb_list(rfi, rfi->exp_idx, rfi->exp_idx,
					     &reorder_list);
		/* add the last packet */
		__skb_queue_tail(&reorder_list, pkt);
		kfree(rfi);
		ifp->drvr->reorder_flows[flow_id] = NULL;
		goto netif_rx;
	}
	/* from here on we need a flow reorder instance */
	if (rfi == NULL) {
		buf_size = sizeof(*rfi);
		max_idx = reorder_data[BRCMF_RXREORDER_MAXIDX_OFFSET];

		buf_size += (max_idx + 1) * sizeof(pkt);

		/* allocate space for flow reorder info */
		brcmf_dbg(INFO, "flow-%d: start, maxidx %d\n",
			  flow_id, max_idx);
		rfi = kzalloc(buf_size, GFP_ATOMIC);
		if (rfi == NULL) {
			brcmf_err("failed to alloc buffer\n");
			brcmf_netif_rx(ifp, pkt);
			return;
		}

		ifp->drvr->reorder_flows[flow_id] = rfi;
		rfi->pktslots = (struct sk_buff **)(rfi+1);
		rfi->max_idx = max_idx;
	}
	if (flags & BRCMF_RXREORDER_NEW_HOLE)  {
		if (rfi->pend_pkts) {
			brcmf_rxreorder_get_skb_list(rfi, rfi->exp_idx,
						     rfi->exp_idx,
						     &reorder_list);
			WARN_ON(rfi->pend_pkts);
		} else {
			__skb_queue_head_init(&reorder_list);
		}
		rfi->cur_idx = reorder_data[BRCMF_RXREORDER_CURIDX_OFFSET];
		rfi->exp_idx = reorder_data[BRCMF_RXREORDER_EXPIDX_OFFSET];
		rfi->max_idx = reorder_data[BRCMF_RXREORDER_MAXIDX_OFFSET];
		rfi->pktslots[rfi->cur_idx] = pkt;
		rfi->pend_pkts++;
		brcmf_dbg(DATA, "flow-%d: new hole %d (%d), pending %d\n",
			  flow_id, rfi->cur_idx, rfi->exp_idx, rfi->pend_pkts);
	} else if (flags & BRCMF_RXREORDER_CURIDX_VALID) {
		cur_idx = reorder_data[BRCMF_RXREORDER_CURIDX_OFFSET];
		exp_idx = reorder_data[BRCMF_RXREORDER_EXPIDX_OFFSET];

		if ((exp_idx == rfi->exp_idx) && (cur_idx != rfi->exp_idx)) {
			/* still in the current hole */
			/* enqueue the current on the buffer chain */
			if (rfi->pktslots[cur_idx] != NULL) {
				brcmf_dbg(INFO, "HOLE: ERROR buffer pending..free it\n");
				brcmu_pkt_buf_free_skb(rfi->pktslots[cur_idx]);
				rfi->pktslots[cur_idx] = NULL;
			}
			rfi->pktslots[cur_idx] = pkt;
			rfi->pend_pkts++;
			rfi->cur_idx = cur_idx;
			brcmf_dbg(DATA, "flow-%d: store pkt %d (%d), pending %d\n",
				  flow_id, cur_idx, exp_idx, rfi->pend_pkts);

			/* can return now as there is no reorder
			 * list to process.
			 */
			return;
		}
		if (rfi->exp_idx == cur_idx) {
			if (rfi->pktslots[cur_idx] != NULL) {
				brcmf_dbg(INFO, "error buffer pending..free it\n");
				brcmu_pkt_buf_free_skb(rfi->pktslots[cur_idx]);
				rfi->pktslots[cur_idx] = NULL;
			}
			rfi->pktslots[cur_idx] = pkt;
			rfi->pend_pkts++;

			/* got the expected one. flush from current to expected
			 * and update expected
			 */
			brcmf_dbg(DATA, "flow-%d: expected %d (%d), pending %d\n",
				  flow_id, cur_idx, exp_idx, rfi->pend_pkts);

			rfi->cur_idx = cur_idx;
			rfi->exp_idx = exp_idx;

			brcmf_rxreorder_get_skb_list(rfi, cur_idx, exp_idx,
						     &reorder_list);
			brcmf_dbg(DATA, "flow-%d: freeing buffers %d, pending %d\n",
				  flow_id, skb_queue_len(&reorder_list),
				  rfi->pend_pkts);
		} else {
			u8 end_idx;

			brcmf_dbg(DATA, "flow-%d (0x%x): both moved, old %d/%d, new %d/%d\n",
				  flow_id, flags, rfi->cur_idx, rfi->exp_idx,
				  cur_idx, exp_idx);
			if (flags & BRCMF_RXREORDER_FLUSH_ALL)
				end_idx = rfi->exp_idx;
			else
				end_idx = exp_idx;

			/* flush pkts first */
			brcmf_rxreorder_get_skb_list(rfi, rfi->exp_idx, end_idx,
						     &reorder_list);

			if (exp_idx == ((cur_idx + 1) % (rfi->max_idx + 1))) {
				__skb_queue_tail(&reorder_list, pkt);
			} else {
				rfi->pktslots[cur_idx] = pkt;
				rfi->pend_pkts++;
			}
			rfi->exp_idx = exp_idx;
			rfi->cur_idx = cur_idx;
		}
	} else {
		/* explicity window move updating the expected index */
		exp_idx = reorder_data[BRCMF_RXREORDER_EXPIDX_OFFSET];

		brcmf_dbg(DATA, "flow-%d (0x%x): change expected: %d -> %d\n",
			  flow_id, flags, rfi->exp_idx, exp_idx);
		if (flags & BRCMF_RXREORDER_FLUSH_ALL)
			end_idx =  rfi->exp_idx;
		else
			end_idx =  exp_idx;

		brcmf_rxreorder_get_skb_list(rfi, rfi->exp_idx, end_idx,
					     &reorder_list);
		__skb_queue_tail(&reorder_list, pkt);
		/* set the new expected idx */
		rfi->exp_idx = exp_idx;
	}
netif_rx:
	skb_queue_walk_safe(&reorder_list, pkt, pnext) {
		__skb_unlink(pkt, &reorder_list);
		brcmf_netif_rx(ifp, pkt);
	}
}

void brcmf_rx_frame(struct device *dev, struct sk_buff *skb)
{
	struct brcmf_if *ifp;
	struct brcmf_bus *bus_if = dev_get_drvdata(dev);
	struct brcmf_pub *drvr = bus_if->drvr;
	struct brcmf_skb_reorder_data *rd;
	u8 ifidx;
	int ret;

	brcmf_dbg(DATA, "Enter: %s: rxp=%p\n", dev_name(dev), skb);

	/* process and remove protocol-specific header */
	ret = brcmf_proto_hdrpull(drvr, true, &ifidx, skb);
	ifp = drvr->iflist[ifidx];

	if (ret || !ifp || !ifp->ndev) {
		if ((ret != -ENODATA) && ifp)
			ifp->stats.rx_errors++;
		brcmu_pkt_buf_free_skb(skb);
		return;
	}

	rd = (struct brcmf_skb_reorder_data *)skb->cb;
	if (rd->reorder)
		brcmf_rxreorder_process_info(ifp, rd->reorder, skb);
	else
		brcmf_netif_rx(ifp, skb);
}

void brcmf_txfinalize(struct brcmf_pub *drvr, struct sk_buff *txp,
		      bool success)
{
	struct brcmf_if *ifp;
	struct ethhdr *eh;
	u8 ifidx;
	u16 type;
	int res;

	res = brcmf_proto_hdrpull(drvr, false, &ifidx, txp);

	ifp = drvr->iflist[ifidx];
	if (!ifp)
		goto done;

	if (res == 0) {
		eh = (struct ethhdr *)(txp->data);
		type = ntohs(eh->h_proto);

		if (type == ETH_P_PAE) {
			atomic_dec(&ifp->pend_8021x_cnt);
			if (waitqueue_active(&ifp->pend_8021x_wait))
				wake_up(&ifp->pend_8021x_wait);
		}
	}
	if (!success)
		ifp->stats.tx_errors++;
done:
	brcmu_pkt_buf_free_skb(txp);
}

void brcmf_txcomplete(struct device *dev, struct sk_buff *txp, bool success)
{
	struct brcmf_bus *bus_if = dev_get_drvdata(dev);
	struct brcmf_pub *drvr = bus_if->drvr;

	/* await txstatus signal for firmware if active */
	if (brcmf_fws_fc_active(drvr->fws)) {
		if (!success)
			brcmf_fws_bustxfail(drvr->fws, txp);
	} else {
		brcmf_txfinalize(drvr, txp, success);
	}
}

static struct net_device_stats *brcmf_netdev_get_stats(struct net_device *ndev)
{
	struct brcmf_if *ifp = netdev_priv(ndev);

	brcmf_dbg(TRACE, "Enter, idx=%d\n", ifp->bssidx);

	return &ifp->stats;
}

/*
 * Set current toe component enables in toe_ol iovar,
 * and set toe global enable iovar
 */
static int brcmf_toe_set(struct brcmf_if *ifp, u32 toe_ol)
{
	s32 err;

	err = brcmf_fil_iovar_int_set(ifp, "toe_ol", toe_ol);
	if (err < 0) {
		brcmf_err("Setting toe_ol failed, %d\n", err);
		return err;
	}

	err = brcmf_fil_iovar_int_set(ifp, "toe", (toe_ol != 0));
	if (err < 0)
		brcmf_err("Setting toe failed, %d\n", err);

	return err;

>>>>>>> refs/remotes/origin/master
}

static void brcmf_ethtool_get_drvinfo(struct net_device *ndev,
				    struct ethtool_drvinfo *info)
{
	struct brcmf_if *ifp = netdev_priv(ndev);
	struct brcmf_pub *drvr = ifp->drvr;

<<<<<<< HEAD
	sprintf(info->driver, KBUILD_MODNAME);
	sprintf(info->version, "%lu", drvr->drv_version);
	sprintf(info->bus_info, "%s", dev_name(drvr->dev));
=======
	strlcpy(info->driver, KBUILD_MODNAME, sizeof(info->driver));
	snprintf(info->version, sizeof(info->version), "%lu",
		 drvr->drv_version);
	strlcpy(info->bus_info, dev_name(drvr->bus_if->dev),
		sizeof(info->bus_info));
>>>>>>> refs/remotes/origin/master
}

static const struct ethtool_ops brcmf_ethtool_ops = {
	.get_drvinfo = brcmf_ethtool_get_drvinfo,
};

<<<<<<< HEAD
static int brcmf_ethtool(struct brcmf_pub *drvr, void __user *uaddr)
{
=======
static int brcmf_ethtool(struct brcmf_if *ifp, void __user *uaddr)
{
	struct brcmf_pub *drvr = ifp->drvr;
>>>>>>> refs/remotes/origin/master
	struct ethtool_drvinfo info;
	char drvname[sizeof(info.driver)];
	u32 cmd;
	struct ethtool_value edata;
	u32 toe_cmpnt, csum_dir;
	int ret;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "Enter\n");
=======
	brcmf_dbg(TRACE, "Enter, idx=%d\n", ifp->bssidx);
>>>>>>> refs/remotes/origin/master

	/* all ethtool calls start with a cmd word */
	if (copy_from_user(&cmd, uaddr, sizeof(u32)))
		return -EFAULT;

	switch (cmd) {
	case ETHTOOL_GDRVINFO:
		/* Copy out any request driver name */
		if (copy_from_user(&info, uaddr, sizeof(info)))
			return -EFAULT;
		strncpy(drvname, info.driver, sizeof(info.driver));
		drvname[sizeof(info.driver) - 1] = '\0';

		/* clear struct for return */
		memset(&info, 0, sizeof(info));
		info.cmd = cmd;

		/* if requested, identify ourselves */
		if (strcmp(drvname, "?dhd") == 0) {
			sprintf(info.driver, "dhd");
			strcpy(info.version, BRCMF_VERSION_STR);
		}
<<<<<<< HEAD

		/* otherwise, require dongle to be up */
		else if (!drvr->bus_if->drvr_up) {
			brcmf_dbg(ERROR, "dongle is not up\n");
			return -ENODEV;
		}

		/* finally, report dongle driver type */
		else if (drvr->iswl)
			sprintf(info.driver, "wl");
		else
			sprintf(info.driver, "xx");
=======
		/* report dongle driver type */
		else
			sprintf(info.driver, "wl");
>>>>>>> refs/remotes/origin/master

		sprintf(info.version, "%lu", drvr->drv_version);
		if (copy_to_user(uaddr, &info, sizeof(info)))
			return -EFAULT;
<<<<<<< HEAD
		brcmf_dbg(CTL, "given %*s, returning %s\n",
=======
		brcmf_dbg(TRACE, "given %*s, returning %s\n",
>>>>>>> refs/remotes/origin/master
			  (int)sizeof(drvname), drvname, info.driver);
		break;

		/* Get toe offload components from dongle */
	case ETHTOOL_GRXCSUM:
	case ETHTOOL_GTXCSUM:
<<<<<<< HEAD
		ret = brcmf_toe_get(drvr, 0, &toe_cmpnt);
=======
		ret = brcmf_fil_iovar_int_get(ifp, "toe_ol", &toe_cmpnt);
>>>>>>> refs/remotes/origin/master
		if (ret < 0)
			return ret;

		csum_dir =
		    (cmd == ETHTOOL_GTXCSUM) ? TOE_TX_CSUM_OL : TOE_RX_CSUM_OL;

		edata.cmd = cmd;
		edata.data = (toe_cmpnt & csum_dir) ? 1 : 0;

		if (copy_to_user(uaddr, &edata, sizeof(edata)))
			return -EFAULT;
		break;

		/* Set toe offload components in dongle */
	case ETHTOOL_SRXCSUM:
	case ETHTOOL_STXCSUM:
		if (copy_from_user(&edata, uaddr, sizeof(edata)))
			return -EFAULT;

		/* Read the current settings, update and write back */
<<<<<<< HEAD
		ret = brcmf_toe_get(drvr, 0, &toe_cmpnt);
=======
		ret = brcmf_fil_iovar_int_get(ifp, "toe_ol", &toe_cmpnt);
>>>>>>> refs/remotes/origin/master
		if (ret < 0)
			return ret;

		csum_dir =
		    (cmd == ETHTOOL_STXCSUM) ? TOE_TX_CSUM_OL : TOE_RX_CSUM_OL;

		if (edata.data != 0)
			toe_cmpnt |= csum_dir;
		else
			toe_cmpnt &= ~csum_dir;

<<<<<<< HEAD
		ret = brcmf_toe_set(drvr, 0, toe_cmpnt);
=======
		ret = brcmf_toe_set(ifp, toe_cmpnt);
>>>>>>> refs/remotes/origin/master
		if (ret < 0)
			return ret;

		/* If setting TX checksum mode, tell Linux the new mode */
		if (cmd == ETHTOOL_STXCSUM) {
			if (edata.data)
<<<<<<< HEAD
				drvr->iflist[0]->ndev->features |=
				    NETIF_F_IP_CSUM;
			else
				drvr->iflist[0]->ndev->features &=
				    ~NETIF_F_IP_CSUM;
=======
				ifp->ndev->features |= NETIF_F_IP_CSUM;
			else
				ifp->ndev->features &= ~NETIF_F_IP_CSUM;
>>>>>>> refs/remotes/origin/master
		}

		break;

	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

static int brcmf_netdev_ioctl_entry(struct net_device *ndev, struct ifreq *ifr,
				    int cmd)
{
	struct brcmf_if *ifp = netdev_priv(ndev);
	struct brcmf_pub *drvr = ifp->drvr;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "ifidx %d, cmd 0x%04x\n", ifp->idx, cmd);

	if (!drvr->iflist[ifp->idx])
		return -1;

	if (cmd == SIOCETHTOOL)
		return brcmf_ethtool(drvr, ifr->ifr_data);
=======
	brcmf_dbg(TRACE, "Enter, idx=%d, cmd=0x%04x\n", ifp->bssidx, cmd);

	if (!drvr->iflist[ifp->bssidx])
		return -1;

	if (cmd == SIOCETHTOOL)
		return brcmf_ethtool(ifp, ifr->ifr_data);
>>>>>>> refs/remotes/origin/master

	return -EOPNOTSUPP;
}

<<<<<<< HEAD
/* called only from within this driver. Sends a command to the dongle. */
s32 brcmf_exec_dcmd(struct net_device *ndev, u32 cmd, void *arg, u32 len)
{
	struct brcmf_dcmd dcmd;
	s32 err = 0;
	int buflen = 0;
	bool is_set_key_cmd;
	struct brcmf_if *ifp = netdev_priv(ndev);
	struct brcmf_pub *drvr = ifp->drvr;

	memset(&dcmd, 0, sizeof(dcmd));
	dcmd.cmd = cmd;
	dcmd.buf = arg;
	dcmd.len = len;

	if (dcmd.buf != NULL)
		buflen = min_t(uint, dcmd.len, BRCMF_DCMD_MAXLEN);

	/* send to dongle (must be up, and wl) */
	if ((drvr->bus_if->state != BRCMF_BUS_DATA)) {
		brcmf_dbg(ERROR, "DONGLE_DOWN\n");
		err = -EIO;
		goto done;
	}

	if (!drvr->iswl) {
		err = -EIO;
		goto done;
	}

	/*
	 * Intercept BRCMF_C_SET_KEY CMD - serialize M4 send and
	 * set key CMD to prevent M4 encryption.
	 */
	is_set_key_cmd = ((dcmd.cmd == BRCMF_C_SET_KEY) ||
			  ((dcmd.cmd == BRCMF_C_SET_VAR) &&
			   !(strncmp("wsec_key", dcmd.buf, 9))) ||
			  ((dcmd.cmd == BRCMF_C_SET_VAR) &&
			   !(strncmp("bsscfg:wsec_key", dcmd.buf, 15))));
	if (is_set_key_cmd)
		brcmf_netdev_wait_pend8021x(ndev);

	err = brcmf_proto_dcmd(drvr, ifp->idx, &dcmd, buflen);

done:
	if (err > 0)
		err = 0;

	return err;
}

static int brcmf_netdev_stop(struct net_device *ndev)
{
	struct brcmf_if *ifp = netdev_priv(ndev);
	struct brcmf_pub *drvr = ifp->drvr;

	brcmf_dbg(TRACE, "Enter\n");
	brcmf_cfg80211_down(drvr->config);
	if (drvr->bus_if->drvr_up == 0)
		return 0;

	/* Set state and stop OS transmissions */
	drvr->bus_if->drvr_up = false;
=======
static int brcmf_netdev_stop(struct net_device *ndev)
{
	struct brcmf_if *ifp = netdev_priv(ndev);

	brcmf_dbg(TRACE, "Enter, idx=%d\n", ifp->bssidx);

	brcmf_cfg80211_down(ndev);

	/* Set state and stop OS transmissions */
>>>>>>> refs/remotes/origin/master
	netif_stop_queue(ndev);

	return 0;
}

static int brcmf_netdev_open(struct net_device *ndev)
{
	struct brcmf_if *ifp = netdev_priv(ndev);
	struct brcmf_pub *drvr = ifp->drvr;
	struct brcmf_bus *bus_if = drvr->bus_if;
	u32 toe_ol;
	s32 ret = 0;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "ifidx %d\n", ifp->idx);

	if (ifp->idx == 0) {	/* do it only for primary eth0 */
		/* If bus is not ready, can't continue */
		if (bus_if->state != BRCMF_BUS_DATA) {
			brcmf_dbg(ERROR, "failed bus is not ready\n");
			return -EAGAIN;
		}

		atomic_set(&drvr->pend_8021x_cnt, 0);

		memcpy(ndev->dev_addr, drvr->mac, ETH_ALEN);

		/* Get current TOE mode from dongle */
		if (brcmf_toe_get(drvr, ifp->idx, &toe_ol) >= 0
		    && (toe_ol & TOE_TX_CSUM_OL) != 0)
			drvr->iflist[ifp->idx]->ndev->features |=
				NETIF_F_IP_CSUM;
		else
			drvr->iflist[ifp->idx]->ndev->features &=
				~NETIF_F_IP_CSUM;
	}
	/* Allow transmit calls */
	netif_start_queue(ndev);
	drvr->bus_if->drvr_up = true;
	if (brcmf_cfg80211_up(drvr->config)) {
		brcmf_dbg(ERROR, "failed to bring up cfg80211\n");
=======
	brcmf_dbg(TRACE, "Enter, idx=%d\n", ifp->bssidx);

	/* If bus is not ready, can't continue */
	if (bus_if->state != BRCMF_BUS_DATA) {
		brcmf_err("failed bus is not ready\n");
		return -EAGAIN;
	}

	atomic_set(&ifp->pend_8021x_cnt, 0);

	/* Get current TOE mode from dongle */
	if (brcmf_fil_iovar_int_get(ifp, "toe_ol", &toe_ol) >= 0
	    && (toe_ol & TOE_TX_CSUM_OL) != 0)
		ndev->features |= NETIF_F_IP_CSUM;
	else
		ndev->features &= ~NETIF_F_IP_CSUM;

	/* Allow transmit calls */
	netif_start_queue(ndev);
	if (brcmf_cfg80211_up(ndev)) {
		brcmf_err("failed to bring up cfg80211\n");
>>>>>>> refs/remotes/origin/master
		return -1;
	}

	return ret;
}

static const struct net_device_ops brcmf_netdev_ops_pri = {
	.ndo_open = brcmf_netdev_open,
	.ndo_stop = brcmf_netdev_stop,
	.ndo_get_stats = brcmf_netdev_get_stats,
	.ndo_do_ioctl = brcmf_netdev_ioctl_entry,
	.ndo_start_xmit = brcmf_netdev_start_xmit,
	.ndo_set_mac_address = brcmf_netdev_set_mac_address,
	.ndo_set_rx_mode = brcmf_netdev_set_multicast_list
};

<<<<<<< HEAD
int
brcmf_add_if(struct device *dev, int ifidx, char *name, u8 *mac_addr)
{
	struct brcmf_if *ifp;
	struct net_device *ndev;
	struct brcmf_bus *bus_if = dev_get_drvdata(dev);
	struct brcmf_pub *drvr = bus_if->drvr;

	brcmf_dbg(TRACE, "idx %d\n", ifidx);

	ifp = drvr->iflist[ifidx];
=======
int brcmf_net_attach(struct brcmf_if *ifp, bool rtnl_locked)
{
	struct brcmf_pub *drvr = ifp->drvr;
	struct net_device *ndev;
	s32 err;

	brcmf_dbg(TRACE, "Enter, idx=%d mac=%pM\n", ifp->bssidx,
		  ifp->mac_addr);
	ndev = ifp->ndev;

	/* set appropriate operations */
	ndev->netdev_ops = &brcmf_netdev_ops_pri;

	ndev->hard_header_len += drvr->hdrlen;
	ndev->ethtool_ops = &brcmf_ethtool_ops;

	drvr->rxsz = ndev->mtu + ndev->hard_header_len +
			      drvr->hdrlen;

	/* set the mac address */
	memcpy(ndev->dev_addr, ifp->mac_addr, ETH_ALEN);

	INIT_WORK(&ifp->setmacaddr_work, _brcmf_set_mac_address);
	INIT_WORK(&ifp->multicast_work, _brcmf_set_multicast_list);

	if (rtnl_locked)
		err = register_netdevice(ndev);
	else
		err = register_netdev(ndev);
	if (err != 0) {
		brcmf_err("couldn't register the net device\n");
		goto fail;
	}

	brcmf_dbg(INFO, "%s: Broadcom Dongle Host Driver\n", ndev->name);

	ndev->destructor = free_netdev;
	return 0;

fail:
	drvr->iflist[ifp->bssidx] = NULL;
	ndev->netdev_ops = NULL;
	free_netdev(ndev);
	return -EBADE;
}

static int brcmf_net_p2p_open(struct net_device *ndev)
{
	brcmf_dbg(TRACE, "Enter\n");

	return brcmf_cfg80211_up(ndev);
}

static int brcmf_net_p2p_stop(struct net_device *ndev)
{
	brcmf_dbg(TRACE, "Enter\n");

	return brcmf_cfg80211_down(ndev);
}

static int brcmf_net_p2p_do_ioctl(struct net_device *ndev,
				  struct ifreq *ifr, int cmd)
{
	brcmf_dbg(TRACE, "Enter\n");
	return 0;
}

static netdev_tx_t brcmf_net_p2p_start_xmit(struct sk_buff *skb,
					    struct net_device *ndev)
{
	if (skb)
		dev_kfree_skb_any(skb);

	return NETDEV_TX_OK;
}

static const struct net_device_ops brcmf_netdev_ops_p2p = {
	.ndo_open = brcmf_net_p2p_open,
	.ndo_stop = brcmf_net_p2p_stop,
	.ndo_do_ioctl = brcmf_net_p2p_do_ioctl,
	.ndo_start_xmit = brcmf_net_p2p_start_xmit
};

static int brcmf_net_p2p_attach(struct brcmf_if *ifp)
{
	struct net_device *ndev;

	brcmf_dbg(TRACE, "Enter, idx=%d mac=%pM\n", ifp->bssidx,
		  ifp->mac_addr);
	ndev = ifp->ndev;

	ndev->netdev_ops = &brcmf_netdev_ops_p2p;

	/* set the mac address */
	memcpy(ndev->dev_addr, ifp->mac_addr, ETH_ALEN);

	if (register_netdev(ndev) != 0) {
		brcmf_err("couldn't register the p2p net device\n");
		goto fail;
	}

	brcmf_dbg(INFO, "%s: Broadcom Dongle Host Driver\n", ndev->name);

	return 0;

fail:
	ifp->drvr->iflist[ifp->bssidx] = NULL;
	ndev->netdev_ops = NULL;
	free_netdev(ndev);
	return -EBADE;
}

struct brcmf_if *brcmf_add_if(struct brcmf_pub *drvr, s32 bssidx, s32 ifidx,
			      char *name, u8 *mac_addr)
{
	struct brcmf_if *ifp;
	struct net_device *ndev;

	brcmf_dbg(TRACE, "Enter, idx=%d, ifidx=%d\n", bssidx, ifidx);

	ifp = drvr->iflist[bssidx];
>>>>>>> refs/remotes/origin/master
	/*
	 * Delete the existing interface before overwriting it
	 * in case we missed the BRCMF_E_IF_DEL event.
	 */
	if (ifp) {
<<<<<<< HEAD
		brcmf_dbg(ERROR, "ERROR: netdev:%s already exists, try free & unregister\n",
			  ifp->ndev->name);
		netif_stop_queue(ifp->ndev);
		unregister_netdev(ifp->ndev);
		free_netdev(ifp->ndev);
		drvr->iflist[ifidx] = NULL;
	}

	/* Allocate netdev, including space for private structure */
	ndev = alloc_netdev(sizeof(struct brcmf_if), name, ether_setup);
	if (!ndev) {
		brcmf_dbg(ERROR, "OOM - alloc_netdev\n");
		return -ENOMEM;
	}

	ifp = netdev_priv(ndev);
	ifp->ndev = ndev;
	ifp->drvr = drvr;
	drvr->iflist[ifidx] = ifp;
	ifp->idx = ifidx;
	if (mac_addr != NULL)
		memcpy(&ifp->mac_addr, mac_addr, ETH_ALEN);

	if (brcmf_net_attach(drvr, ifp->idx)) {
		brcmf_dbg(ERROR, "brcmf_net_attach failed");
		free_netdev(ifp->ndev);
		drvr->iflist[ifidx] = NULL;
		return -EOPNOTSUPP;
	}

	brcmf_dbg(TRACE, " ==== pid:%x, net_device for if:%s created ===\n",
		  current->pid, ifp->ndev->name);

	return 0;
}

void brcmf_del_if(struct brcmf_pub *drvr, int ifidx)
{
	struct brcmf_if *ifp;

	brcmf_dbg(TRACE, "idx %d\n", ifidx);

	ifp = drvr->iflist[ifidx];
	if (!ifp) {
		brcmf_dbg(ERROR, "Null interface\n");
		return;
	}
	if (ifp->ndev) {
		if (ifidx == 0) {
=======
		brcmf_err("ERROR: netdev:%s already exists\n",
			  ifp->ndev->name);
		if (ifidx) {
			netif_stop_queue(ifp->ndev);
			unregister_netdev(ifp->ndev);
			free_netdev(ifp->ndev);
			drvr->iflist[bssidx] = NULL;
		} else {
			brcmf_err("ignore IF event\n");
			return ERR_PTR(-EINVAL);
		}
	}

	if (!brcmf_p2p_enable && bssidx == 1) {
		/* this is P2P_DEVICE interface */
		brcmf_dbg(INFO, "allocate non-netdev interface\n");
		ifp = kzalloc(sizeof(*ifp), GFP_KERNEL);
		if (!ifp)
			return ERR_PTR(-ENOMEM);
	} else {
		brcmf_dbg(INFO, "allocate netdev interface\n");
		/* Allocate netdev, including space for private structure */
		ndev = alloc_netdev(sizeof(*ifp), name, ether_setup);
		if (!ndev)
			return ERR_PTR(-ENOMEM);

		ifp = netdev_priv(ndev);
		ifp->ndev = ndev;
	}

	ifp->drvr = drvr;
	drvr->iflist[bssidx] = ifp;
	ifp->ifidx = ifidx;
	ifp->bssidx = bssidx;

	init_waitqueue_head(&ifp->pend_8021x_wait);
	spin_lock_init(&ifp->netif_stop_lock);

	if (mac_addr != NULL)
		memcpy(ifp->mac_addr, mac_addr, ETH_ALEN);

	brcmf_dbg(TRACE, " ==== pid:%x, if:%s (%pM) created ===\n",
		  current->pid, name, ifp->mac_addr);

	return ifp;
}

void brcmf_del_if(struct brcmf_pub *drvr, s32 bssidx)
{
	struct brcmf_if *ifp;

	ifp = drvr->iflist[bssidx];
	drvr->iflist[bssidx] = NULL;
	if (!ifp) {
		brcmf_err("Null interface, idx=%d\n", bssidx);
		return;
	}
	brcmf_dbg(TRACE, "Enter, idx=%d, ifidx=%d\n", bssidx, ifp->ifidx);
	if (ifp->ndev) {
		if (bssidx == 0) {
>>>>>>> refs/remotes/origin/master
			if (ifp->ndev->netdev_ops == &brcmf_netdev_ops_pri) {
				rtnl_lock();
				brcmf_netdev_stop(ifp->ndev);
				rtnl_unlock();
			}
		} else {
			netif_stop_queue(ifp->ndev);
		}

<<<<<<< HEAD
		unregister_netdev(ifp->ndev);
		drvr->iflist[ifidx] = NULL;
		if (ifidx == 0)
			brcmf_cfg80211_detach(drvr->config);
		free_netdev(ifp->ndev);
=======
		if (ifp->ndev->netdev_ops == &brcmf_netdev_ops_pri) {
			cancel_work_sync(&ifp->setmacaddr_work);
			cancel_work_sync(&ifp->multicast_work);
		}
		/* unregister will take care of freeing it */
		unregister_netdev(ifp->ndev);
		if (bssidx == 0)
			brcmf_cfg80211_detach(drvr->config);
	} else {
		kfree(ifp);
>>>>>>> refs/remotes/origin/master
	}
}

int brcmf_attach(uint bus_hdrlen, struct device *dev)
{
	struct brcmf_pub *drvr = NULL;
	int ret = 0;

	brcmf_dbg(TRACE, "Enter\n");

	/* Allocate primary brcmf_info */
	drvr = kzalloc(sizeof(struct brcmf_pub), GFP_ATOMIC);
	if (!drvr)
		return -ENOMEM;

	mutex_init(&drvr->proto_block);

	/* Link to bus module */
	drvr->hdrlen = bus_hdrlen;
	drvr->bus_if = dev_get_drvdata(dev);
	drvr->bus_if->drvr = drvr;
<<<<<<< HEAD
	drvr->dev = dev;
=======

	/* create device debugfs folder */
	brcmf_debugfs_attach(drvr);
>>>>>>> refs/remotes/origin/master

	/* Attach and link in the protocol */
	ret = brcmf_proto_attach(drvr);
	if (ret != 0) {
<<<<<<< HEAD
		brcmf_dbg(ERROR, "brcmf_prot_attach failed\n");
		goto fail;
	}

	INIT_WORK(&drvr->setmacaddr_work, _brcmf_set_mac_address);
	INIT_WORK(&drvr->multicast_work, _brcmf_set_multicast_list);
=======
		brcmf_err("brcmf_prot_attach failed\n");
		goto fail;
	}

	/* attach firmware event handler */
	brcmf_fweh_attach(drvr);

	INIT_LIST_HEAD(&drvr->bus_if->dcmd_list);
>>>>>>> refs/remotes/origin/master

	return ret;

fail:
	brcmf_detach(dev);

	return ret;
}

int brcmf_bus_start(struct device *dev)
{
	int ret = -1;
<<<<<<< HEAD
	/* Room for "event_msgs" + '\0' + bitvec */
	char iovbuf[BRCMF_EVENTING_MASK_LEN + 12];
	struct brcmf_bus *bus_if = dev_get_drvdata(dev);
	struct brcmf_pub *drvr = bus_if->drvr;
=======
	struct brcmf_bus *bus_if = dev_get_drvdata(dev);
	struct brcmf_pub *drvr = bus_if->drvr;
	struct brcmf_if *ifp;
	struct brcmf_if *p2p_ifp;
>>>>>>> refs/remotes/origin/master

	brcmf_dbg(TRACE, "\n");

	/* Bring up the bus */
<<<<<<< HEAD
	ret = bus_if->brcmf_bus_init(dev);
	if (ret != 0) {
		brcmf_dbg(ERROR, "brcmf_sdbrcm_bus_init failed %d\n", ret);
		return ret;
	}

	brcmf_c_mkiovar("event_msgs", drvr->eventmask, BRCMF_EVENTING_MASK_LEN,
		      iovbuf, sizeof(iovbuf));
	brcmf_proto_cdc_query_dcmd(drvr, 0, BRCMF_C_GET_VAR, iovbuf,
				    sizeof(iovbuf));
	memcpy(drvr->eventmask, iovbuf, BRCMF_EVENTING_MASK_LEN);

	setbit(drvr->eventmask, BRCMF_E_SET_SSID);
	setbit(drvr->eventmask, BRCMF_E_PRUNE);
	setbit(drvr->eventmask, BRCMF_E_AUTH);
	setbit(drvr->eventmask, BRCMF_E_REASSOC);
	setbit(drvr->eventmask, BRCMF_E_REASSOC_IND);
	setbit(drvr->eventmask, BRCMF_E_DEAUTH_IND);
	setbit(drvr->eventmask, BRCMF_E_DISASSOC_IND);
	setbit(drvr->eventmask, BRCMF_E_DISASSOC);
	setbit(drvr->eventmask, BRCMF_E_JOIN);
	setbit(drvr->eventmask, BRCMF_E_ASSOC_IND);
	setbit(drvr->eventmask, BRCMF_E_PSK_SUP);
	setbit(drvr->eventmask, BRCMF_E_LINK);
	setbit(drvr->eventmask, BRCMF_E_NDIS_LINK);
	setbit(drvr->eventmask, BRCMF_E_MIC_ERROR);
	setbit(drvr->eventmask, BRCMF_E_PMKID_CACHE);
	setbit(drvr->eventmask, BRCMF_E_TXFAIL);
	setbit(drvr->eventmask, BRCMF_E_JOIN_START);
	setbit(drvr->eventmask, BRCMF_E_SCAN_COMPLETE);

/* enable dongle roaming event */

	drvr->pktfilter_count = 1;
	/* Setup filter to allow only unicast */
	drvr->pktfilter[0] = "100 0 0 0 0x01 0x00";

	/* Bus is ready, do any protocol initialization */
	ret = brcmf_proto_init(drvr);
	if (ret < 0)
		return ret;

	/* signal bus ready */
	bus_if->state = BRCMF_BUS_DATA;
	return 0;
}

int brcmf_net_attach(struct brcmf_pub *drvr, int ifidx)
{
	struct net_device *ndev;
	u8 temp_addr[ETH_ALEN] = {
		0x00, 0x90, 0x4c, 0x11, 0x22, 0x33};

	brcmf_dbg(TRACE, "ifidx %d\n", ifidx);

	ndev = drvr->iflist[ifidx]->ndev;
	ndev->netdev_ops = &brcmf_netdev_ops_pri;

	/*
	 * We have to use the primary MAC for virtual interfaces
	 */
	if (ifidx != 0) {
		/* for virtual interfaces use the primary MAC  */
		memcpy(temp_addr, drvr->mac, ETH_ALEN);

	}

	if (ifidx == 1) {
		brcmf_dbg(TRACE, "ACCESS POINT MAC:\n");
		/*  ACCESSPOINT INTERFACE CASE */
		temp_addr[0] |= 0X02;	/* set bit 2 ,
			 - Locally Administered address  */

	}
	ndev->hard_header_len = ETH_HLEN + drvr->hdrlen;
	ndev->ethtool_ops = &brcmf_ethtool_ops;

	drvr->rxsz = ndev->mtu + ndev->hard_header_len +
			      drvr->hdrlen;

	memcpy(ndev->dev_addr, temp_addr, ETH_ALEN);

	/* attach to cfg80211 for primary interface */
	if (!ifidx) {
		drvr->config = brcmf_cfg80211_attach(ndev, drvr->dev, drvr);
		if (drvr->config == NULL) {
			brcmf_dbg(ERROR, "wl_cfg80211_attach failed\n");
			goto fail;
		}
	}

	if (register_netdev(ndev) != 0) {
		brcmf_dbg(ERROR, "couldn't register the net device\n");
		goto fail;
	}

	brcmf_dbg(INFO, "%s: Broadcom Dongle Host Driver\n", ndev->name);

	return 0;

fail:
	ndev->netdev_ops = NULL;
	return -EBADE;
=======
	ret = brcmf_bus_init(bus_if);
	if (ret != 0) {
		brcmf_err("brcmf_sdbrcm_bus_init failed %d\n", ret);
		return ret;
	}

	/* add primary networking interface */
	ifp = brcmf_add_if(drvr, 0, 0, "wlan%d", NULL);
	if (IS_ERR(ifp))
		return PTR_ERR(ifp);

	if (brcmf_p2p_enable)
		p2p_ifp = brcmf_add_if(drvr, 1, 0, "p2p%d", NULL);
	else
		p2p_ifp = NULL;
	if (IS_ERR(p2p_ifp))
		p2p_ifp = NULL;

	/* signal bus ready */
	bus_if->state = BRCMF_BUS_DATA;

	/* Bus is ready, do any initialization */
	ret = brcmf_c_preinit_dcmds(ifp);
	if (ret < 0)
		goto fail;

	ret = brcmf_fws_init(drvr);
	if (ret < 0)
		goto fail;

	brcmf_fws_add_interface(ifp);

	drvr->config = brcmf_cfg80211_attach(drvr, bus_if->dev);
	if (drvr->config == NULL) {
		ret = -ENOMEM;
		goto fail;
	}

	ret = brcmf_fweh_activate_events(ifp);
	if (ret < 0)
		goto fail;

	ret = brcmf_net_attach(ifp, false);
fail:
	if (ret < 0) {
		brcmf_err("failed: %d\n", ret);
		if (drvr->config)
			brcmf_cfg80211_detach(drvr->config);
		if (drvr->fws) {
			brcmf_fws_del_interface(ifp);
			brcmf_fws_deinit(drvr);
		}
		if (drvr->iflist[0]) {
			free_netdev(ifp->ndev);
			drvr->iflist[0] = NULL;
		}
		if (p2p_ifp) {
			free_netdev(p2p_ifp->ndev);
			drvr->iflist[1] = NULL;
		}
		return ret;
	}
	if ((brcmf_p2p_enable) && (p2p_ifp))
		if (brcmf_net_p2p_attach(p2p_ifp) < 0)
			brcmf_p2p_enable = 0;

	return 0;
>>>>>>> refs/remotes/origin/master
}

static void brcmf_bus_detach(struct brcmf_pub *drvr)
{
	brcmf_dbg(TRACE, "Enter\n");

	if (drvr) {
		/* Stop the protocol module */
		brcmf_proto_stop(drvr);

		/* Stop the bus module */
<<<<<<< HEAD
		drvr->bus_if->brcmf_bus_stop(drvr->dev);
	}
}

void brcmf_detach(struct device *dev)
{
	int i;
=======
		brcmf_bus_stop(drvr->bus_if);
	}
}

void brcmf_dev_reset(struct device *dev)
{
	struct brcmf_bus *bus_if = dev_get_drvdata(dev);
	struct brcmf_pub *drvr = bus_if->drvr;

	if (drvr == NULL)
		return;

	if (drvr->iflist[0])
		brcmf_fil_cmd_int_set(drvr->iflist[0], BRCMF_C_TERMINATED, 1);
}

void brcmf_detach(struct device *dev)
{
	s32 i;
>>>>>>> refs/remotes/origin/master
	struct brcmf_bus *bus_if = dev_get_drvdata(dev);
	struct brcmf_pub *drvr = bus_if->drvr;

	brcmf_dbg(TRACE, "Enter\n");

<<<<<<< HEAD

	/* make sure primary interface removed last */
	for (i = BRCMF_MAX_IFS-1; i > -1; i--)
		if (drvr->iflist[i])
			brcmf_del_if(drvr, i);

	brcmf_bus_detach(drvr);

	if (drvr->prot) {
		cancel_work_sync(&drvr->setmacaddr_work);
		cancel_work_sync(&drvr->multicast_work);
		brcmf_proto_detach(drvr);
	}

=======
	if (drvr == NULL)
		return;

	/* stop firmware event handling */
	brcmf_fweh_detach(drvr);

	/* make sure primary interface removed last */
	for (i = BRCMF_MAX_IFS-1; i > -1; i--)
		if (drvr->iflist[i]) {
			brcmf_fws_del_interface(drvr->iflist[i]);
			brcmf_del_if(drvr, i);
		}

	brcmf_bus_detach(drvr);

	if (drvr->prot)
		brcmf_proto_detach(drvr);

	brcmf_fws_deinit(drvr);

	brcmf_debugfs_detach(drvr);
>>>>>>> refs/remotes/origin/master
	bus_if->drvr = NULL;
	kfree(drvr);
}

<<<<<<< HEAD
static int brcmf_get_pend_8021x_cnt(struct brcmf_pub *drvr)
{
	return atomic_read(&drvr->pend_8021x_cnt);
}

#define MAX_WAIT_FOR_8021X_TX	10

int brcmf_netdev_wait_pend8021x(struct net_device *ndev)
{
	struct brcmf_if *ifp = netdev_priv(ndev);
	struct brcmf_pub *drvr = ifp->drvr;
	int timeout = 10 * HZ / 1000;
	int ntimes = MAX_WAIT_FOR_8021X_TX;
	int pend = brcmf_get_pend_8021x_cnt(drvr);

	while (ntimes && pend) {
		if (pend) {
			set_current_state(TASK_INTERRUPTIBLE);
			schedule_timeout(timeout);
			set_current_state(TASK_RUNNING);
			ntimes--;
		}
		pend = brcmf_get_pend_8021x_cnt(drvr);
	}
	return pend;
}

#ifdef DEBUG
int brcmf_write_to_file(struct brcmf_pub *drvr, const u8 *buf, int size)
{
	int ret = 0;
	struct file *fp;
	mm_segment_t old_fs;
	loff_t pos = 0;

	/* change to KERNEL_DS address limit */
	old_fs = get_fs();
	set_fs(KERNEL_DS);

	/* open file to write */
	fp = filp_open("/tmp/mem_dump", O_WRONLY | O_CREAT, 0640);
	if (!fp) {
		brcmf_dbg(ERROR, "open file error\n");
		ret = -1;
		goto exit;
	}

	/* Write buf to file */
	fp->f_op->write(fp, (char __user *)buf, size, &pos);

exit:
	/* free buf before return */
	kfree(buf);
	/* close file before return */
	if (fp)
		filp_close(fp, current->files);
	/* restore previous address limit */
	set_fs(old_fs);

	return ret;
}
#endif				/* DEBUG */

static void brcmf_driver_init(struct work_struct *work)
{
#ifdef CONFIG_BRCMFMAC_SDIO
	brcmf_sdio_init();
#endif
#ifdef CONFIG_BRCMFMAC_USB
	brcmf_usb_init();
#endif
}
static DECLARE_WORK(brcmf_driver_work, brcmf_driver_init);

static int __init brcmfmac_module_init(void)
{
=======
static int brcmf_get_pend_8021x_cnt(struct brcmf_if *ifp)
{
	return atomic_read(&ifp->pend_8021x_cnt);
}

int brcmf_netdev_wait_pend8021x(struct net_device *ndev)
{
	struct brcmf_if *ifp = netdev_priv(ndev);
	int err;

	err = wait_event_timeout(ifp->pend_8021x_wait,
				 !brcmf_get_pend_8021x_cnt(ifp),
				 msecs_to_jiffies(MAX_WAIT_FOR_8021X_TX));

	WARN_ON(!err);

	return !err;
}

/*
 * return chip id and rev of the device encoded in u32.
 */
u32 brcmf_get_chip_info(struct brcmf_if *ifp)
{
	struct brcmf_bus *bus = ifp->drvr->bus_if;

	return bus->chip << 4 | bus->chiprev;
}

static void brcmf_driver_register(struct work_struct *work)
{
#ifdef CONFIG_BRCMFMAC_SDIO
	brcmf_sdio_register();
#endif
#ifdef CONFIG_BRCMFMAC_USB
	brcmf_usb_register();
#endif
}
static DECLARE_WORK(brcmf_driver_work, brcmf_driver_register);

static int __init brcmfmac_module_init(void)
{
	brcmf_debugfs_init();
#ifdef CONFIG_BRCMFMAC_SDIO
	brcmf_sdio_init();
#endif
>>>>>>> refs/remotes/origin/master
	if (!schedule_work(&brcmf_driver_work))
		return -EBUSY;

	return 0;
}

static void __exit brcmfmac_module_exit(void)
{
	cancel_work_sync(&brcmf_driver_work);

#ifdef CONFIG_BRCMFMAC_SDIO
	brcmf_sdio_exit();
#endif
#ifdef CONFIG_BRCMFMAC_USB
	brcmf_usb_exit();
#endif
<<<<<<< HEAD
=======
	brcmf_debugfs_exit();
>>>>>>> refs/remotes/origin/master
}

module_init(brcmfmac_module_init);
module_exit(brcmfmac_module_exit);
