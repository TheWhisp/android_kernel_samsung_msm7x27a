/*
 * Copyright (c) 2008-2011 Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/dma-mapping.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include "ath9k.h"
#include "ar9003_mac.h"

#define SKB_CB_ATHBUF(__skb)	(*((struct ath_buf **)__skb->cb))

static inline bool ath_is_alt_ant_ratio_better(int alt_ratio, int maxdelta,
					       int mindelta, int main_rssi_avg,
					       int alt_rssi_avg, int pkt_count)
{
	return (((alt_ratio >= ATH_ANT_DIV_COMB_ALT_ANT_RATIO2) &&
		(alt_rssi_avg > main_rssi_avg + maxdelta)) ||
		(alt_rssi_avg > main_rssi_avg + mindelta)) && (pkt_count > 50);
}

static inline bool ath_ant_div_comb_alt_check(u8 div_group, int alt_ratio,
					int curr_main_set, int curr_alt_set,
					int alt_rssi_avg, int main_rssi_avg)
{
	bool result = false;
	switch (div_group) {
	case 0:
		if (alt_ratio > ATH_ANT_DIV_COMB_ALT_ANT_RATIO)
			result = true;
		break;
	case 1:
<<<<<<< HEAD
=======
	case 2:
>>>>>>> refs/remotes/origin/cm-10.0
		if ((((curr_main_set == ATH_ANT_DIV_COMB_LNA2) &&
			(curr_alt_set == ATH_ANT_DIV_COMB_LNA1) &&
				(alt_rssi_avg >= (main_rssi_avg - 5))) ||
			((curr_main_set == ATH_ANT_DIV_COMB_LNA1) &&
			(curr_alt_set == ATH_ANT_DIV_COMB_LNA2) &&
				(alt_rssi_avg >= (main_rssi_avg - 2)))) &&
							(alt_rssi_avg >= 4))
			result = true;
		else
			result = false;
		break;
	}

	return result;
}
=======
#include <linux/dma-mapping.h>
#include <linux/relay.h>
#include "ath9k.h"
#include "ar9003_mac.h"

#define SKB_CB_ATHBUF(__skb)	(*((struct ath_rxbuf **)__skb->cb))
>>>>>>> refs/remotes/origin/master

static inline bool ath9k_check_auto_sleep(struct ath_softc *sc)
{
	return sc->ps_enabled &&
	       (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_AUTOSLEEP);
}

/*
 * Setup and link descriptors.
 *
 * 11N: we can no longer afford to self link the last descriptor.
 * MAC acknowledges BA status as long as it copies frames to host
 * buffer (or rx fifo). This can incorrectly acknowledge packets
 * to a sender if last desc is self-linked.
 */
<<<<<<< HEAD
static void ath_rx_buf_link(struct ath_softc *sc, struct ath_buf *bf)
=======
static void ath_rx_buf_link(struct ath_softc *sc, struct ath_rxbuf *bf)
>>>>>>> refs/remotes/origin/master
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath_desc *ds;
	struct sk_buff *skb;

<<<<<<< HEAD
<<<<<<< HEAD
	ATH_RXBUF_RESET(bf);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ds = bf->bf_desc;
	ds->ds_link = 0; /* link to null */
	ds->ds_data = bf->bf_buf_addr;

	/* virtual addr of the beginning of the buffer. */
	skb = bf->bf_mpdu;
	BUG_ON(skb == NULL);
	ds->ds_vdata = skb->data;

	/*
	 * setup rx descriptors. The rx_bufsize here tells the hardware
	 * how much data it can DMA to us and that we are prepared
	 * to process
	 */
	ath9k_hw_setuprxdesc(ah, ds,
			     common->rx_bufsize,
			     0);

	if (sc->rx.rxlink == NULL)
		ath9k_hw_putrxbuf(ah, bf->bf_daddr);
	else
		*sc->rx.rxlink = bf->bf_daddr;

	sc->rx.rxlink = &ds->ds_link;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static void ath_rx_buf_relink(struct ath_softc *sc, struct ath_buf *bf)
=======
static void ath_rx_buf_relink(struct ath_softc *sc, struct ath_rxbuf *bf)
>>>>>>> refs/remotes/origin/master
{
	if (sc->rx.buf_hold)
		ath_rx_buf_link(sc, sc->rx.buf_hold);

	sc->rx.buf_hold = bf;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void ath_setdefantenna(struct ath_softc *sc, u32 antenna)
{
	/* XXX block beacon interrupts */
	ath9k_hw_setantenna(sc->sc_ah, antenna);
	sc->rx.defant = antenna;
	sc->rx.rxotherant = 0;
}

static void ath_opmode_init(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);

	u32 rfilt, mfilt[2];

	/* configure rx filter */
	rfilt = ath_calcrxfilter(sc);
	ath9k_hw_setrxfilter(ah, rfilt);

	/* configure bssid mask */
	ath_hw_setbssidmask(common);

	/* configure operational mode */
	ath9k_hw_setopmode(ah);

	/* calculate and install multicast filter */
	mfilt[0] = mfilt[1] = ~0;
	ath9k_hw_setmcastfilter(ah, mfilt[0], mfilt[1]);
}

static bool ath_rx_edma_buf_link(struct ath_softc *sc,
				 enum ath9k_rx_qtype qtype)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_rx_edma *rx_edma;
	struct sk_buff *skb;
<<<<<<< HEAD
	struct ath_buf *bf;
=======
	struct ath_rxbuf *bf;
>>>>>>> refs/remotes/origin/master

	rx_edma = &sc->rx.rx_edma[qtype];
	if (skb_queue_len(&rx_edma->rx_fifo) >= rx_edma->rx_fifo_hwsize)
		return false;

<<<<<<< HEAD
	bf = list_first_entry(&sc->rx.rxbuf, struct ath_buf, list);
=======
	bf = list_first_entry(&sc->rx.rxbuf, struct ath_rxbuf, list);
>>>>>>> refs/remotes/origin/master
	list_del_init(&bf->list);

	skb = bf->bf_mpdu;

<<<<<<< HEAD
<<<<<<< HEAD
	ATH_RXBUF_RESET(bf);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	memset(skb->data, 0, ah->caps.rx_status_len);
	dma_sync_single_for_device(sc->dev, bf->bf_buf_addr,
				ah->caps.rx_status_len, DMA_TO_DEVICE);

	SKB_CB_ATHBUF(skb) = bf;
	ath9k_hw_addrxbuf_edma(ah, bf->bf_buf_addr, qtype);
<<<<<<< HEAD
	skb_queue_tail(&rx_edma->rx_fifo, skb);
=======
	__skb_queue_tail(&rx_edma->rx_fifo, skb);
>>>>>>> refs/remotes/origin/master

	return true;
}

static void ath_rx_addbuffer_edma(struct ath_softc *sc,
<<<<<<< HEAD
				  enum ath9k_rx_qtype qtype, int size)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
<<<<<<< HEAD
	u32 nbuf = 0;

	if (list_empty(&sc->rx.rxbuf)) {
		ath_dbg(common, ATH_DBG_QUEUE, "No free rx buf available\n");
		return;
	}

	while (!list_empty(&sc->rx.rxbuf)) {
		nbuf++;

		if (!ath_rx_edma_buf_link(sc, qtype))
			break;

		if (nbuf >= size)
			break;
	}
=======
	struct ath_buf *bf, *tbf;
=======
				  enum ath9k_rx_qtype qtype)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_rxbuf *bf, *tbf;
>>>>>>> refs/remotes/origin/master

	if (list_empty(&sc->rx.rxbuf)) {
		ath_dbg(common, QUEUE, "No free rx buf available\n");
		return;
	}

	list_for_each_entry_safe(bf, tbf, &sc->rx.rxbuf, list)
		if (!ath_rx_edma_buf_link(sc, qtype))
			break;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void ath_rx_remove_buffer(struct ath_softc *sc,
				 enum ath9k_rx_qtype qtype)
{
<<<<<<< HEAD
	struct ath_buf *bf;
=======
	struct ath_rxbuf *bf;
>>>>>>> refs/remotes/origin/master
	struct ath_rx_edma *rx_edma;
	struct sk_buff *skb;

	rx_edma = &sc->rx.rx_edma[qtype];

<<<<<<< HEAD
	while ((skb = skb_dequeue(&rx_edma->rx_fifo)) != NULL) {
=======
	while ((skb = __skb_dequeue(&rx_edma->rx_fifo)) != NULL) {
>>>>>>> refs/remotes/origin/master
		bf = SKB_CB_ATHBUF(skb);
		BUG_ON(!bf);
		list_add_tail(&bf->list, &sc->rx.rxbuf);
	}
}

static void ath_rx_edma_cleanup(struct ath_softc *sc)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
>>>>>>> refs/remotes/origin/cm-10.0
	struct ath_buf *bf;
=======
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath_rxbuf *bf;
>>>>>>> refs/remotes/origin/master

	ath_rx_remove_buffer(sc, ATH9K_RX_QUEUE_LP);
	ath_rx_remove_buffer(sc, ATH9K_RX_QUEUE_HP);

	list_for_each_entry(bf, &sc->rx.rxbuf, list) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (bf->bf_mpdu)
			dev_kfree_skb_any(bf->bf_mpdu);
=======
=======
>>>>>>> refs/remotes/origin/master
		if (bf->bf_mpdu) {
			dma_unmap_single(sc->dev, bf->bf_buf_addr,
					common->rx_bufsize,
					DMA_BIDIRECTIONAL);
			dev_kfree_skb_any(bf->bf_mpdu);
			bf->bf_buf_addr = 0;
			bf->bf_mpdu = NULL;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	}

	INIT_LIST_HEAD(&sc->rx.rxbuf);

	kfree(sc->rx.rx_bufptr);
	sc->rx.rx_bufptr = NULL;
=======
	}
>>>>>>> refs/remotes/origin/master
}

static void ath_rx_edma_init_queue(struct ath_rx_edma *rx_edma, int size)
{
<<<<<<< HEAD
	skb_queue_head_init(&rx_edma->rx_fifo);
<<<<<<< HEAD
	skb_queue_head_init(&rx_edma->rx_buffers);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	__skb_queue_head_init(&rx_edma->rx_fifo);
>>>>>>> refs/remotes/origin/master
	rx_edma->rx_fifo_hwsize = size;
}

static int ath_rx_edma_init(struct ath_softc *sc, int nbufs)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_hw *ah = sc->sc_ah;
	struct sk_buff *skb;
<<<<<<< HEAD
	struct ath_buf *bf;
=======
	struct ath_rxbuf *bf;
>>>>>>> refs/remotes/origin/master
	int error = 0, i;
	u32 size;

	ath9k_hw_set_rx_bufsize(ah, common->rx_bufsize -
				    ah->caps.rx_status_len);

	ath_rx_edma_init_queue(&sc->rx.rx_edma[ATH9K_RX_QUEUE_LP],
			       ah->caps.rx_lp_qdepth);
	ath_rx_edma_init_queue(&sc->rx.rx_edma[ATH9K_RX_QUEUE_HP],
			       ah->caps.rx_hp_qdepth);

<<<<<<< HEAD
	size = sizeof(struct ath_buf) * nbufs;
	bf = kzalloc(size, GFP_KERNEL);
=======
	size = sizeof(struct ath_rxbuf) * nbufs;
	bf = devm_kzalloc(sc->dev, size, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!bf)
		return -ENOMEM;

	INIT_LIST_HEAD(&sc->rx.rxbuf);
<<<<<<< HEAD
	sc->rx.rx_bufptr = bf;
=======
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < nbufs; i++, bf++) {
		skb = ath_rxbuf_alloc(common, common->rx_bufsize, GFP_KERNEL);
		if (!skb) {
			error = -ENOMEM;
			goto rx_init_fail;
		}

		memset(skb->data, 0, common->rx_bufsize);
		bf->bf_mpdu = skb;

		bf->bf_buf_addr = dma_map_single(sc->dev, skb->data,
						 common->rx_bufsize,
						 DMA_BIDIRECTIONAL);
		if (unlikely(dma_mapping_error(sc->dev,
						bf->bf_buf_addr))) {
				dev_kfree_skb_any(skb);
				bf->bf_mpdu = NULL;
				bf->bf_buf_addr = 0;
				ath_err(common,
					"dma_mapping_error() on RX init\n");
				error = -ENOMEM;
				goto rx_init_fail;
		}

		list_add_tail(&bf->list, &sc->rx.rxbuf);
	}

	return 0;

rx_init_fail:
	ath_rx_edma_cleanup(sc);
	return error;
}

static void ath_edma_start_recv(struct ath_softc *sc)
{
<<<<<<< HEAD
	spin_lock_bh(&sc->rx.rxbuflock);

	ath9k_hw_rxena(sc->sc_ah);

	ath_rx_addbuffer_edma(sc, ATH9K_RX_QUEUE_HP,
			      sc->rx.rx_edma[ATH9K_RX_QUEUE_HP].rx_fifo_hwsize);

	ath_rx_addbuffer_edma(sc, ATH9K_RX_QUEUE_LP,
			      sc->rx.rx_edma[ATH9K_RX_QUEUE_LP].rx_fifo_hwsize);

	ath_opmode_init(sc);

	ath9k_hw_startpcureceive(sc->sc_ah, (sc->sc_flags & SC_OP_OFFCHANNEL));

	spin_unlock_bh(&sc->rx.rxbuflock);
=======
	ath9k_hw_rxena(sc->sc_ah);
	ath_rx_addbuffer_edma(sc, ATH9K_RX_QUEUE_HP);
	ath_rx_addbuffer_edma(sc, ATH9K_RX_QUEUE_LP);
	ath_opmode_init(sc);
	ath9k_hw_startpcureceive(sc->sc_ah, !!(sc->hw->conf.flags & IEEE80211_CONF_OFFCHANNEL));
>>>>>>> refs/remotes/origin/master
}

static void ath_edma_stop_recv(struct ath_softc *sc)
{
	ath_rx_remove_buffer(sc, ATH9K_RX_QUEUE_HP);
	ath_rx_remove_buffer(sc, ATH9K_RX_QUEUE_LP);
}

int ath_rx_init(struct ath_softc *sc, int nbufs)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct sk_buff *skb;
<<<<<<< HEAD
	struct ath_buf *bf;
	int error = 0;

	spin_lock_init(&sc->sc_pcu_lock);
	sc->sc_flags &= ~SC_OP_RXFLUSH;
	spin_lock_init(&sc->rx.rxbuflock);
=======
	struct ath_rxbuf *bf;
	int error = 0;

	spin_lock_init(&sc->sc_pcu_lock);
>>>>>>> refs/remotes/origin/master

	common->rx_bufsize = IEEE80211_MAX_MPDU_LEN / 2 +
			     sc->sc_ah->caps.rx_status_len;

<<<<<<< HEAD
	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_EDMA) {
		return ath_rx_edma_init(sc, nbufs);
	} else {
<<<<<<< HEAD
		ath_dbg(common, ATH_DBG_CONFIG, "cachelsz %u rxbufsize %u\n",
=======
		ath_dbg(common, CONFIG, "cachelsz %u rxbufsize %u\n",
>>>>>>> refs/remotes/origin/cm-10.0
			common->cachelsz, common->rx_bufsize);

		/* Initialize rx descriptors */

		error = ath_descdma_setup(sc, &sc->rx.rxdma, &sc->rx.rxbuf,
				"rx", nbufs, 1, 0);
		if (error != 0) {
			ath_err(common,
				"failed to allocate rx descriptors: %d\n",
				error);
			goto err;
		}

		list_for_each_entry(bf, &sc->rx.rxbuf, list) {
			skb = ath_rxbuf_alloc(common, common->rx_bufsize,
					      GFP_KERNEL);
			if (skb == NULL) {
				error = -ENOMEM;
				goto err;
			}

			bf->bf_mpdu = skb;
			bf->bf_buf_addr = dma_map_single(sc->dev, skb->data,
					common->rx_bufsize,
					DMA_FROM_DEVICE);
			if (unlikely(dma_mapping_error(sc->dev,
							bf->bf_buf_addr))) {
				dev_kfree_skb_any(skb);
				bf->bf_mpdu = NULL;
				bf->bf_buf_addr = 0;
				ath_err(common,
					"dma_mapping_error() on RX init\n");
				error = -ENOMEM;
				goto err;
			}
		}
		sc->rx.rxlink = NULL;
	}

=======
	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_EDMA)
		return ath_rx_edma_init(sc, nbufs);

	ath_dbg(common, CONFIG, "cachelsz %u rxbufsize %u\n",
		common->cachelsz, common->rx_bufsize);

	/* Initialize rx descriptors */

	error = ath_descdma_setup(sc, &sc->rx.rxdma, &sc->rx.rxbuf,
				  "rx", nbufs, 1, 0);
	if (error != 0) {
		ath_err(common,
			"failed to allocate rx descriptors: %d\n",
			error);
		goto err;
	}

	list_for_each_entry(bf, &sc->rx.rxbuf, list) {
		skb = ath_rxbuf_alloc(common, common->rx_bufsize,
				      GFP_KERNEL);
		if (skb == NULL) {
			error = -ENOMEM;
			goto err;
		}

		bf->bf_mpdu = skb;
		bf->bf_buf_addr = dma_map_single(sc->dev, skb->data,
						 common->rx_bufsize,
						 DMA_FROM_DEVICE);
		if (unlikely(dma_mapping_error(sc->dev,
					       bf->bf_buf_addr))) {
			dev_kfree_skb_any(skb);
			bf->bf_mpdu = NULL;
			bf->bf_buf_addr = 0;
			ath_err(common,
				"dma_mapping_error() on RX init\n");
			error = -ENOMEM;
			goto err;
		}
	}
	sc->rx.rxlink = NULL;
>>>>>>> refs/remotes/origin/master
err:
	if (error)
		ath_rx_cleanup(sc);

	return error;
}

void ath_rx_cleanup(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct sk_buff *skb;
<<<<<<< HEAD
	struct ath_buf *bf;
=======
	struct ath_rxbuf *bf;
>>>>>>> refs/remotes/origin/master

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_EDMA) {
		ath_rx_edma_cleanup(sc);
		return;
<<<<<<< HEAD
	} else {
		list_for_each_entry(bf, &sc->rx.rxbuf, list) {
			skb = bf->bf_mpdu;
			if (skb) {
				dma_unmap_single(sc->dev, bf->bf_buf_addr,
						common->rx_bufsize,
						DMA_FROM_DEVICE);
				dev_kfree_skb(skb);
				bf->bf_buf_addr = 0;
				bf->bf_mpdu = NULL;
			}
		}

		if (sc->rx.rxdma.dd_desc_len != 0)
			ath_descdma_cleanup(sc, &sc->rx.rxdma, &sc->rx.rxbuf);
=======
	}

	list_for_each_entry(bf, &sc->rx.rxbuf, list) {
		skb = bf->bf_mpdu;
		if (skb) {
			dma_unmap_single(sc->dev, bf->bf_buf_addr,
					 common->rx_bufsize,
					 DMA_FROM_DEVICE);
			dev_kfree_skb(skb);
			bf->bf_buf_addr = 0;
			bf->bf_mpdu = NULL;
		}
>>>>>>> refs/remotes/origin/master
	}
}

/*
 * Calculate the receive filter according to the
 * operating mode and state:
 *
 * o always accept unicast, broadcast, and multicast traffic
 * o maintain current state of phy error reception (the hal
 *   may enable phy error frames for noise immunity work)
 * o probe request frames are accepted only when operating in
 *   hostap, adhoc, or monitor modes
 * o enable promiscuous mode according to the interface state
 * o accept beacons:
 *   - when operating in adhoc mode so the 802.11 layer creates
 *     node table entries for peers,
 *   - when operating in station mode for collecting rssi data when
 *     the station is otherwise quiet, or
 *   - when operating as a repeater so we see repeater-sta beacons
 *   - when scanning
 */

u32 ath_calcrxfilter(struct ath_softc *sc)
{
	u32 rfilt;

<<<<<<< HEAD
	rfilt = ATH9K_RX_FILTER_UCAST | ATH9K_RX_FILTER_BCAST
		| ATH9K_RX_FILTER_MCAST;

=======
	if (config_enabled(CONFIG_ATH9K_TX99))
		return 0;

	rfilt = ATH9K_RX_FILTER_UCAST | ATH9K_RX_FILTER_BCAST
		| ATH9K_RX_FILTER_MCAST;

	/* if operating on a DFS channel, enable radar pulse detection */
	if (sc->hw->conf.radar_enabled)
		rfilt |= ATH9K_RX_FILTER_PHYRADAR | ATH9K_RX_FILTER_PHYERR;

>>>>>>> refs/remotes/origin/master
	if (sc->rx.rxfilter & FIF_PROBE_REQ)
		rfilt |= ATH9K_RX_FILTER_PROBEREQ;

	/*
	 * Set promiscuous mode when FIF_PROMISC_IN_BSS is enabled for station
	 * mode interface or when in monitor mode. AP mode does not need this
	 * since it receives all in-BSS frames anyway.
	 */
	if (sc->sc_ah->is_monitoring)
		rfilt |= ATH9K_RX_FILTER_PROM;

	if (sc->rx.rxfilter & FIF_CONTROL)
		rfilt |= ATH9K_RX_FILTER_CONTROL;

	if ((sc->sc_ah->opmode == NL80211_IFTYPE_STATION) &&
	    (sc->nvifs <= 1) &&
	    !(sc->rx.rxfilter & FIF_BCN_PRBRESP_PROMISC))
		rfilt |= ATH9K_RX_FILTER_MYBEACON;
	else
		rfilt |= ATH9K_RX_FILTER_BEACON;

	if ((sc->sc_ah->opmode == NL80211_IFTYPE_AP) ||
	    (sc->rx.rxfilter & FIF_PSPOLL))
		rfilt |= ATH9K_RX_FILTER_PSPOLL;

	if (conf_is_ht(&sc->hw->conf))
		rfilt |= ATH9K_RX_FILTER_COMP_BAR;

	if (sc->nvifs > 1 || (sc->rx.rxfilter & FIF_OTHER_BSS)) {
<<<<<<< HEAD
		/* The following may also be needed for other older chips */
		if (sc->sc_ah->hw_version.macVersion == AR_SREV_VERSION_9160)
=======
		/* This is needed for older chips */
		if (sc->sc_ah->hw_version.macVersion <= AR_SREV_VERSION_9160)
>>>>>>> refs/remotes/origin/master
			rfilt |= ATH9K_RX_FILTER_PROM;
		rfilt |= ATH9K_RX_FILTER_MCAST_BCAST_ALL;
	}

<<<<<<< HEAD
	return rfilt;

<<<<<<< HEAD
#undef RX_FILTER_PRESERVE
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (AR_SREV_9550(sc->sc_ah))
		rfilt |= ATH9K_RX_FILTER_4ADDRESS;

	return rfilt;

>>>>>>> refs/remotes/origin/master
}

int ath_startrecv(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
<<<<<<< HEAD
	struct ath_buf *bf, *tbf;
=======
	struct ath_rxbuf *bf, *tbf;
>>>>>>> refs/remotes/origin/master

	if (ah->caps.hw_caps & ATH9K_HW_CAP_EDMA) {
		ath_edma_start_recv(sc);
		return 0;
	}

<<<<<<< HEAD
	spin_lock_bh(&sc->rx.rxbuflock);
	if (list_empty(&sc->rx.rxbuf))
		goto start_recv;

<<<<<<< HEAD
=======
	sc->rx.buf_hold = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (list_empty(&sc->rx.rxbuf))
		goto start_recv;

	sc->rx.buf_hold = NULL;
>>>>>>> refs/remotes/origin/master
	sc->rx.rxlink = NULL;
	list_for_each_entry_safe(bf, tbf, &sc->rx.rxbuf, list) {
		ath_rx_buf_link(sc, bf);
	}

	/* We could have deleted elements so the list may be empty now */
	if (list_empty(&sc->rx.rxbuf))
		goto start_recv;

<<<<<<< HEAD
	bf = list_first_entry(&sc->rx.rxbuf, struct ath_buf, list);
=======
	bf = list_first_entry(&sc->rx.rxbuf, struct ath_rxbuf, list);
>>>>>>> refs/remotes/origin/master
	ath9k_hw_putrxbuf(ah, bf->bf_daddr);
	ath9k_hw_rxena(ah);

start_recv:
	ath_opmode_init(sc);
<<<<<<< HEAD
	ath9k_hw_startpcureceive(ah, (sc->sc_flags & SC_OP_OFFCHANNEL));

	spin_unlock_bh(&sc->rx.rxbuflock);
=======
	ath9k_hw_startpcureceive(ah, !!(sc->hw->conf.flags & IEEE80211_CONF_OFFCHANNEL));
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
=======
static void ath_flushrecv(struct ath_softc *sc)
{
	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_EDMA)
		ath_rx_tasklet(sc, 1, true);
	ath_rx_tasklet(sc, 1, false);
}

>>>>>>> refs/remotes/origin/master
bool ath_stoprecv(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	bool stopped, reset = false;

<<<<<<< HEAD
	spin_lock_bh(&sc->rx.rxbuflock);
=======
>>>>>>> refs/remotes/origin/master
	ath9k_hw_abortpcurecv(ah);
	ath9k_hw_setrxfilter(ah, 0);
	stopped = ath9k_hw_stopdmarecv(ah, &reset);

<<<<<<< HEAD
=======
	ath_flushrecv(sc);

>>>>>>> refs/remotes/origin/master
	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_EDMA)
		ath_edma_stop_recv(sc);
	else
		sc->rx.rxlink = NULL;
<<<<<<< HEAD
	spin_unlock_bh(&sc->rx.rxbuflock);
=======
>>>>>>> refs/remotes/origin/master

	if (!(ah->ah_flags & AH_UNPLUGGED) &&
	    unlikely(!stopped)) {
		ath_err(ath9k_hw_common(sc->sc_ah),
			"Could not stop RX, we could be "
			"confusing the DMA engine when we start RX up\n");
		ATH_DBG_WARN_ON_ONCE(!stopped);
	}
	return stopped && !reset;
}

<<<<<<< HEAD
void ath_flushrecv(struct ath_softc *sc)
{
	sc->sc_flags |= SC_OP_RXFLUSH;
	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_EDMA)
		ath_rx_tasklet(sc, 1, true);
	ath_rx_tasklet(sc, 1, false);
	sc->sc_flags &= ~SC_OP_RXFLUSH;
}

=======
>>>>>>> refs/remotes/origin/master
static bool ath_beacon_dtim_pending_cab(struct sk_buff *skb)
{
	/* Check whether the Beacon frame has DTIM indicating buffered bc/mc */
	struct ieee80211_mgmt *mgmt;
	u8 *pos, *end, id, elen;
	struct ieee80211_tim_ie *tim;

	mgmt = (struct ieee80211_mgmt *)skb->data;
	pos = mgmt->u.beacon.variable;
	end = skb->data + skb->len;

	while (pos + 2 < end) {
		id = *pos++;
		elen = *pos++;
		if (pos + elen > end)
			break;

		if (id == WLAN_EID_TIM) {
			if (elen < sizeof(*tim))
				break;
			tim = (struct ieee80211_tim_ie *) pos;
			if (tim->dtim_count != 0)
				break;
			return tim->bitmap_ctrl & 0x01;
		}

		pos += elen;
	}

	return false;
}

static void ath_rx_ps_beacon(struct ath_softc *sc, struct sk_buff *skb)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct ieee80211_mgmt *mgmt;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);

	if (skb->len < 24 + 8 + 2 + 2)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	mgmt = (struct ieee80211_mgmt *)skb->data;
	if (memcmp(common->curbssid, mgmt->bssid, ETH_ALEN) != 0) {
		/* TODO:  This doesn't work well if you have stations
		 * associated to two different APs because curbssid
		 * is just the last AP that any of the stations associated
		 * with.
		 */
		return; /* not from our current AP */
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	sc->ps_flags &= ~PS_WAIT_FOR_BEACON;

	if (sc->ps_flags & PS_BEACON_SYNC) {
		sc->ps_flags &= ~PS_BEACON_SYNC;
<<<<<<< HEAD
<<<<<<< HEAD
		ath_dbg(common, ATH_DBG_PS,
			"Reconfigure Beacon timers based on timestamp from the AP\n");
		ath_set_beacon(sc);
		sc->ps_flags &= ~PS_TSFOOR_SYNC;
=======
		ath_dbg(common, PS,
			"Reconfigure Beacon timers based on timestamp from the AP\n");
		ath_set_beacon(sc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ath_dbg(common, PS,
			"Reconfigure beacon timers based on synchronized timestamp\n");
		ath9k_set_beacon(sc);
>>>>>>> refs/remotes/origin/master
	}

	if (ath_beacon_dtim_pending_cab(skb)) {
		/*
		 * Remain awake waiting for buffered broadcast/multicast
		 * frames. If the last broadcast/multicast frame is not
		 * received properly, the next beacon frame will work as
		 * a backup trigger for returning into NETWORK SLEEP state,
		 * so we are waiting for it as well.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		ath_dbg(common, ATH_DBG_PS,
=======
		ath_dbg(common, PS,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ath_dbg(common, PS,
>>>>>>> refs/remotes/origin/master
			"Received DTIM beacon indicating buffered broadcast/multicast frame(s)\n");
		sc->ps_flags |= PS_WAIT_FOR_CAB | PS_WAIT_FOR_BEACON;
		return;
	}

	if (sc->ps_flags & PS_WAIT_FOR_CAB) {
		/*
		 * This can happen if a broadcast frame is dropped or the AP
		 * fails to send a frame indicating that all CAB frames have
		 * been delivered.
		 */
		sc->ps_flags &= ~PS_WAIT_FOR_CAB;
<<<<<<< HEAD
<<<<<<< HEAD
		ath_dbg(common, ATH_DBG_PS,
			"PS wait for CAB frames timed out\n");
	}
}

static void ath_rx_ps(struct ath_softc *sc, struct sk_buff *skb)
=======
=======
>>>>>>> refs/remotes/origin/master
		ath_dbg(common, PS, "PS wait for CAB frames timed out\n");
	}
}

static void ath_rx_ps(struct ath_softc *sc, struct sk_buff *skb, bool mybeacon)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_hdr *hdr;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);

	hdr = (struct ieee80211_hdr *)skb->data;

	/* Process Beacon and CAB receive in PS state */
	if (((sc->ps_flags & PS_WAIT_FOR_BEACON) || ath9k_check_auto_sleep(sc))
<<<<<<< HEAD
<<<<<<< HEAD
	    && ieee80211_is_beacon(hdr->frame_control))
=======
	    && mybeacon)
>>>>>>> refs/remotes/origin/cm-10.0
		ath_rx_ps_beacon(sc, skb);
	else if ((sc->ps_flags & PS_WAIT_FOR_CAB) &&
		 (ieee80211_is_data(hdr->frame_control) ||
		  ieee80211_is_action(hdr->frame_control)) &&
		 is_multicast_ether_addr(hdr->addr1) &&
		 !ieee80211_has_moredata(hdr->frame_control)) {
=======
	    && mybeacon) {
		ath_rx_ps_beacon(sc, skb);
	} else if ((sc->ps_flags & PS_WAIT_FOR_CAB) &&
		   (ieee80211_is_data(hdr->frame_control) ||
		    ieee80211_is_action(hdr->frame_control)) &&
		   is_multicast_ether_addr(hdr->addr1) &&
		   !ieee80211_has_moredata(hdr->frame_control)) {
>>>>>>> refs/remotes/origin/master
		/*
		 * No more broadcast/multicast frames to be received at this
		 * point.
		 */
		sc->ps_flags &= ~(PS_WAIT_FOR_CAB | PS_WAIT_FOR_BEACON);
<<<<<<< HEAD
<<<<<<< HEAD
		ath_dbg(common, ATH_DBG_PS,
=======
		ath_dbg(common, PS,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ath_dbg(common, PS,
>>>>>>> refs/remotes/origin/master
			"All PS CAB frames received, back to sleep\n");
	} else if ((sc->ps_flags & PS_WAIT_FOR_PSPOLL_DATA) &&
		   !is_multicast_ether_addr(hdr->addr1) &&
		   !ieee80211_has_morefrags(hdr->frame_control)) {
		sc->ps_flags &= ~PS_WAIT_FOR_PSPOLL_DATA;
<<<<<<< HEAD
<<<<<<< HEAD
		ath_dbg(common, ATH_DBG_PS,
=======
		ath_dbg(common, PS,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ath_dbg(common, PS,
>>>>>>> refs/remotes/origin/master
			"Going back to sleep after having received PS-Poll data (0x%lx)\n",
			sc->ps_flags & (PS_WAIT_FOR_BEACON |
					PS_WAIT_FOR_CAB |
					PS_WAIT_FOR_PSPOLL_DATA |
					PS_WAIT_FOR_TX_ACK));
	}
}

static bool ath_edma_get_buffers(struct ath_softc *sc,
<<<<<<< HEAD
<<<<<<< HEAD
				 enum ath9k_rx_qtype qtype)
=======
				 enum ath9k_rx_qtype qtype,
				 struct ath_rx_status *rs,
				 struct ath_buf **dest)
>>>>>>> refs/remotes/origin/cm-10.0
=======
				 enum ath9k_rx_qtype qtype,
				 struct ath_rx_status *rs,
				 struct ath_rxbuf **dest)
>>>>>>> refs/remotes/origin/master
{
	struct ath_rx_edma *rx_edma = &sc->rx.rx_edma[qtype];
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct sk_buff *skb;
<<<<<<< HEAD
	struct ath_buf *bf;
=======
	struct ath_rxbuf *bf;
>>>>>>> refs/remotes/origin/master
	int ret;

	skb = skb_peek(&rx_edma->rx_fifo);
	if (!skb)
		return false;

	bf = SKB_CB_ATHBUF(skb);
	BUG_ON(!bf);

	dma_sync_single_for_cpu(sc->dev, bf->bf_buf_addr,
				common->rx_bufsize, DMA_FROM_DEVICE);

<<<<<<< HEAD
<<<<<<< HEAD
	ret = ath9k_hw_process_rxdesc_edma(ah, NULL, skb->data);
=======
	ret = ath9k_hw_process_rxdesc_edma(ah, rs, skb->data);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = ath9k_hw_process_rxdesc_edma(ah, rs, skb->data);
>>>>>>> refs/remotes/origin/master
	if (ret == -EINPROGRESS) {
		/*let device gain the buffer again*/
		dma_sync_single_for_device(sc->dev, bf->bf_buf_addr,
				common->rx_bufsize, DMA_FROM_DEVICE);
		return false;
	}

	__skb_unlink(skb, &rx_edma->rx_fifo);
	if (ret == -EINVAL) {
		/* corrupt descriptor, skip this one and the following one */
		list_add_tail(&bf->list, &sc->rx.rxbuf);
		ath_rx_edma_buf_link(sc, qtype);
<<<<<<< HEAD
<<<<<<< HEAD
		skb = skb_peek(&rx_edma->rx_fifo);
		if (!skb)
			return true;

		bf = SKB_CB_ATHBUF(skb);
		BUG_ON(!bf);

		__skb_unlink(skb, &rx_edma->rx_fifo);
		list_add_tail(&bf->list, &sc->rx.rxbuf);
		ath_rx_edma_buf_link(sc, qtype);
		return true;
	}
	skb_queue_tail(&rx_edma->rx_buffers, skb);

=======
=======
>>>>>>> refs/remotes/origin/master

		skb = skb_peek(&rx_edma->rx_fifo);
		if (skb) {
			bf = SKB_CB_ATHBUF(skb);
			BUG_ON(!bf);

			__skb_unlink(skb, &rx_edma->rx_fifo);
			list_add_tail(&bf->list, &sc->rx.rxbuf);
			ath_rx_edma_buf_link(sc, qtype);
		}

		bf = NULL;
	}

	*dest = bf;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	return true;
}

static struct ath_buf *ath_edma_get_next_rx_buf(struct ath_softc *sc,
						struct ath_rx_status *rs,
						enum ath9k_rx_qtype qtype)
{
<<<<<<< HEAD
	struct ath_rx_edma *rx_edma = &sc->rx.rx_edma[qtype];
	struct sk_buff *skb;
	struct ath_buf *bf;

	while (ath_edma_get_buffers(sc, qtype));
	skb = __skb_dequeue(&rx_edma->rx_buffers);
	if (!skb)
		return NULL;

	bf = SKB_CB_ATHBUF(skb);
	ath9k_hw_process_rxdesc_edma(sc->sc_ah, rs, skb->data);
	return bf;
=======
	struct ath_buf *bf = NULL;
=======
	return true;
}

static struct ath_rxbuf *ath_edma_get_next_rx_buf(struct ath_softc *sc,
						struct ath_rx_status *rs,
						enum ath9k_rx_qtype qtype)
{
	struct ath_rxbuf *bf = NULL;
>>>>>>> refs/remotes/origin/master

	while (ath_edma_get_buffers(sc, qtype, rs, &bf)) {
		if (!bf)
			continue;

		return bf;
	}
	return NULL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static struct ath_buf *ath_get_next_rx_buf(struct ath_softc *sc,
=======
}

static struct ath_rxbuf *ath_get_next_rx_buf(struct ath_softc *sc,
>>>>>>> refs/remotes/origin/master
					   struct ath_rx_status *rs)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath_desc *ds;
<<<<<<< HEAD
	struct ath_buf *bf;
=======
	struct ath_rxbuf *bf;
>>>>>>> refs/remotes/origin/master
	int ret;

	if (list_empty(&sc->rx.rxbuf)) {
		sc->rx.rxlink = NULL;
		return NULL;
	}

<<<<<<< HEAD
	bf = list_first_entry(&sc->rx.rxbuf, struct ath_buf, list);
<<<<<<< HEAD
=======
	if (bf == sc->rx.buf_hold)
		return NULL;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	bf = list_first_entry(&sc->rx.rxbuf, struct ath_rxbuf, list);
	if (bf == sc->rx.buf_hold)
		return NULL;

>>>>>>> refs/remotes/origin/master
	ds = bf->bf_desc;

	/*
	 * Must provide the virtual address of the current
	 * descriptor, the physical address, and the virtual
	 * address of the next descriptor in the h/w chain.
	 * This allows the HAL to look ahead to see if the
	 * hardware is done with a descriptor by checking the
	 * done bit in the following descriptor and the address
	 * of the current descriptor the DMA engine is working
	 * on.  All this is necessary because of our use of
	 * a self-linked list to avoid rx overruns.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	ret = ath9k_hw_rxprocdesc(ah, ds, rs, 0);
=======
	ret = ath9k_hw_rxprocdesc(ah, ds, rs);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret == -EINPROGRESS) {
		struct ath_rx_status trs;
		struct ath_buf *tbf;
=======
	ret = ath9k_hw_rxprocdesc(ah, ds, rs);
	if (ret == -EINPROGRESS) {
		struct ath_rx_status trs;
		struct ath_rxbuf *tbf;
>>>>>>> refs/remotes/origin/master
		struct ath_desc *tds;

		memset(&trs, 0, sizeof(trs));
		if (list_is_last(&bf->list, &sc->rx.rxbuf)) {
			sc->rx.rxlink = NULL;
			return NULL;
		}

<<<<<<< HEAD
		tbf = list_entry(bf->list.next, struct ath_buf, list);
=======
		tbf = list_entry(bf->list.next, struct ath_rxbuf, list);
>>>>>>> refs/remotes/origin/master

		/*
		 * On some hardware the descriptor status words could
		 * get corrupted, including the done bit. Because of
		 * this, check if the next descriptor's done bit is
		 * set or not.
		 *
		 * If the next descriptor's done bit is set, the current
		 * descriptor has been corrupted. Force s/w to discard
		 * this descriptor and continue...
		 */

		tds = tbf->bf_desc;
<<<<<<< HEAD
<<<<<<< HEAD
		ret = ath9k_hw_rxprocdesc(ah, tds, &trs, 0);
=======
		ret = ath9k_hw_rxprocdesc(ah, tds, &trs);
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret == -EINPROGRESS)
			return NULL;
	}

<<<<<<< HEAD
=======
	list_del(&bf->list);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ret = ath9k_hw_rxprocdesc(ah, tds, &trs);
		if (ret == -EINPROGRESS)
			return NULL;

		/*
		 * mark descriptor as zero-length and set the 'more'
		 * flag to ensure that both buffers get discarded
		 */
		rs->rs_datalen = 0;
		rs->rs_more = true;
	}

	list_del(&bf->list);
>>>>>>> refs/remotes/origin/master
	if (!bf->bf_mpdu)
		return bf;

	/*
	 * Synchronize the DMA transfer with CPU before
	 * 1. accessing the frame
	 * 2. requeueing the same buffer to h/w
	 */
	dma_sync_single_for_cpu(sc->dev, bf->bf_buf_addr,
			common->rx_bufsize,
			DMA_FROM_DEVICE);

	return bf;
}

/* Assumes you've already done the endian to CPU conversion */
static bool ath9k_rx_accept(struct ath_common *common,
			    struct ieee80211_hdr *hdr,
			    struct ieee80211_rx_status *rxs,
			    struct ath_rx_status *rx_stats,
			    bool *decrypt_error)
{
<<<<<<< HEAD
<<<<<<< HEAD
#define is_mc_or_valid_tkip_keyix ((is_mc ||			\
		(rx_stats->rs_keyix != ATH9K_RXKEYIX_INVALID && \
		test_bit(rx_stats->rs_keyix, common->tkip_keymap))))

=======
	struct ath_softc *sc = (struct ath_softc *) common->priv;
	bool is_mc, is_valid_tkip, strip_mic, mic_error;
>>>>>>> refs/remotes/origin/cm-10.0
	struct ath_hw *ah = common->ah;
	__le16 fc;
	u8 rx_status_len = ah->caps.rx_status_len;

	fc = hdr->frame_control;

<<<<<<< HEAD
=======
=======
	struct ath_softc *sc = (struct ath_softc *) common->priv;
	bool is_mc, is_valid_tkip, strip_mic, mic_error;
	struct ath_hw *ah = common->ah;
	__le16 fc;

	fc = hdr->frame_control;

>>>>>>> refs/remotes/origin/master
	is_mc = !!is_multicast_ether_addr(hdr->addr1);
	is_valid_tkip = rx_stats->rs_keyix != ATH9K_RXKEYIX_INVALID &&
		test_bit(rx_stats->rs_keyix, common->tkip_keymap);
	strip_mic = is_valid_tkip && ieee80211_is_data(fc) &&
<<<<<<< HEAD
=======
		ieee80211_has_protected(fc) &&
>>>>>>> refs/remotes/origin/master
		!(rx_stats->rs_status &
		(ATH9K_RXERR_DECRYPT | ATH9K_RXERR_CRC | ATH9K_RXERR_MIC |
		 ATH9K_RXERR_KEYMISS));

	/*
	 * Key miss events are only relevant for pairwise keys where the
	 * descriptor does contain a valid key index. This has been observed
	 * mostly with CCMP encryption.
	 */
	if (rx_stats->rs_keyix == ATH9K_RXKEYIX_INVALID ||
	    !test_bit(rx_stats->rs_keyix, common->ccmp_keymap))
		rx_stats->rs_status &= ~ATH9K_RXERR_KEYMISS;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (!rx_stats->rs_datalen)
		return false;
        /*
         * rs_status follows rs_datalen so if rs_datalen is too large
         * we can take a hint that hardware corrupted it, so ignore
         * those frames.
         */
	if (rx_stats->rs_datalen > (common->rx_bufsize - rx_status_len))
		return false;

	/* Only use error bits from the last fragment */
	if (rx_stats->rs_more)
		return true;

<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	mic_error = is_valid_tkip && !ieee80211_is_ctl(fc) &&
		!ieee80211_has_morefrags(fc) &&
		!(le16_to_cpu(hdr->seq_ctrl) & IEEE80211_SCTL_FRAG) &&
		(rx_stats->rs_status & ATH9K_RXERR_MIC);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * The rx_stats->rs_status will not be set until the end of the
	 * chained descriptors so it can be ignored if rs_more is set. The
	 * rs_more will be false at the last element of the chained
	 * descriptors.
	 */
	if (rx_stats->rs_status != 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (rx_stats->rs_status & ATH9K_RXERR_CRC)
			rxs->flag |= RX_FLAG_FAILED_FCS_CRC;
		if (rx_stats->rs_status & ATH9K_RXERR_PHY)
			return false;

		if (rx_stats->rs_status & ATH9K_RXERR_DECRYPT) {
			*decrypt_error = true;
		} else if (rx_stats->rs_status & ATH9K_RXERR_MIC) {
			bool is_mc;
			/*
			 * The MIC error bit is only valid if the frame
			 * is not a control frame or fragment, and it was
			 * decrypted using a valid TKIP key.
			 */
			is_mc = !!is_multicast_ether_addr(hdr->addr1);

			if (!ieee80211_is_ctl(fc) &&
			    !ieee80211_has_morefrags(fc) &&
			    !(le16_to_cpu(hdr->seq_ctrl) & IEEE80211_SCTL_FRAG) &&
			    is_mc_or_valid_tkip_keyix)
				rxs->flag |= RX_FLAG_MMIC_ERROR;
			else
				rx_stats->rs_status &= ~ATH9K_RXERR_MIC;
		}
=======
=======
>>>>>>> refs/remotes/origin/master
		u8 status_mask;

		if (rx_stats->rs_status & ATH9K_RXERR_CRC) {
			rxs->flag |= RX_FLAG_FAILED_FCS_CRC;
			mic_error = false;
		}
<<<<<<< HEAD
		if (rx_stats->rs_status & ATH9K_RXERR_PHY)
			return false;
=======
>>>>>>> refs/remotes/origin/master

		if ((rx_stats->rs_status & ATH9K_RXERR_DECRYPT) ||
		    (!is_mc && (rx_stats->rs_status & ATH9K_RXERR_KEYMISS))) {
			*decrypt_error = true;
			mic_error = false;
		}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		/*
		 * Reject error frames with the exception of
		 * decryption and MIC failures. For monitor mode,
		 * we also ignore the CRC error.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		if (ah->is_monitoring) {
			if (rx_stats->rs_status &
			    ~(ATH9K_RXERR_DECRYPT | ATH9K_RXERR_MIC |
			      ATH9K_RXERR_CRC))
				return false;
		} else {
			if (rx_stats->rs_status &
			    ~(ATH9K_RXERR_DECRYPT | ATH9K_RXERR_MIC)) {
				return false;
			}
		}
	}
=======
=======
>>>>>>> refs/remotes/origin/master
		status_mask = ATH9K_RXERR_DECRYPT | ATH9K_RXERR_MIC |
			      ATH9K_RXERR_KEYMISS;

		if (ah->is_monitoring && (sc->rx.rxfilter & FIF_FCSFAIL))
			status_mask |= ATH9K_RXERR_CRC;

		if (rx_stats->rs_status & ~status_mask)
			return false;
	}

	/*
	 * For unicast frames the MIC error bit can have false positives,
	 * so all MIC error reports need to be validated in software.
	 * False negatives are not common, so skip software verification
	 * if the hardware considers the MIC valid.
	 */
	if (strip_mic)
		rxs->flag |= RX_FLAG_MMIC_STRIPPED;
	else if (is_mc && mic_error)
		rxs->flag |= RX_FLAG_MMIC_ERROR;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return true;
}

static int ath9k_process_rate(struct ath_common *common,
			      struct ieee80211_hw *hw,
			      struct ath_rx_status *rx_stats,
			      struct ieee80211_rx_status *rxs)
{
	struct ieee80211_supported_band *sband;
	enum ieee80211_band band;
	unsigned int i = 0;
<<<<<<< HEAD

	band = hw->conf.channel->band;
	sband = hw->wiphy->bands[band];

	if (rx_stats->rs_rate & 0x80) {
		/* HT rate */
		rxs->flag |= RX_FLAG_HT;
		if (rx_stats->rs_flags & ATH9K_RX_2040)
			rxs->flag |= RX_FLAG_40MHZ;
		if (rx_stats->rs_flags & ATH9K_RX_GI)
			rxs->flag |= RX_FLAG_SHORT_GI;
=======
	struct ath_softc __maybe_unused *sc = common->priv;

	band = hw->conf.chandef.chan->band;
	sband = hw->wiphy->bands[band];

	switch (hw->conf.chandef.width) {
	case NL80211_CHAN_WIDTH_5:
		rxs->flag |= RX_FLAG_5MHZ;
		break;
	case NL80211_CHAN_WIDTH_10:
		rxs->flag |= RX_FLAG_10MHZ;
		break;
	default:
		break;
	}

	if (rx_stats->rs_rate & 0x80) {
		/* HT rate */
		rxs->flag |= RX_FLAG_HT;
		rxs->flag |= rx_stats->flag;
>>>>>>> refs/remotes/origin/master
		rxs->rate_idx = rx_stats->rs_rate & 0x7f;
		return 0;
	}

	for (i = 0; i < sband->n_bitrates; i++) {
		if (sband->bitrates[i].hw_value == rx_stats->rs_rate) {
			rxs->rate_idx = i;
			return 0;
		}
		if (sband->bitrates[i].hw_value_short == rx_stats->rs_rate) {
			rxs->flag |= RX_FLAG_SHORTPRE;
			rxs->rate_idx = i;
			return 0;
		}
	}

	/*
	 * No valid hardware bitrate found -- we should not get here
	 * because hardware has already validated this frame as OK.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	ath_dbg(common, ATH_DBG_XMIT,
=======
	ath_dbg(common, ANY,
>>>>>>> refs/remotes/origin/cm-10.0
		"unsupported hw bitrate detected 0x%02x using 1 Mbit\n",
		rx_stats->rs_rate);

=======
	ath_dbg(common, ANY,
		"unsupported hw bitrate detected 0x%02x using 1 Mbit\n",
		rx_stats->rs_rate);
	RX_STAT_INC(rx_rate_err);
>>>>>>> refs/remotes/origin/master
	return -EINVAL;
}

static void ath9k_process_rssi(struct ath_common *common,
			       struct ieee80211_hw *hw,
<<<<<<< HEAD
			       struct ieee80211_hdr *hdr,
			       struct ath_rx_status *rx_stats)
=======
			       struct ath_rx_status *rx_stats,
			       struct ieee80211_rx_status *rxs)
>>>>>>> refs/remotes/origin/master
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = common->ah;
	int last_rssi;
<<<<<<< HEAD
<<<<<<< HEAD
	__le16 fc;

	if ((ah->opmode != NL80211_IFTYPE_STATION) &&
	    (ah->opmode != NL80211_IFTYPE_ADHOC))
		return;

	fc = hdr->frame_control;
	if (!ieee80211_is_beacon(fc) ||
	    compare_ether_addr(hdr->addr3, common->curbssid)) {
		/* TODO:  This doesn't work well if you have stations
		 * associated to two different APs because curbssid
		 * is just the last AP that any of the stations associated
		 * with.
		 */
		return;
	}

=======
	int rssi = rx_stats->rs_rssi;

	if (!rx_stats->is_mybeacon ||
	    ((ah->opmode != NL80211_IFTYPE_STATION) &&
	     (ah->opmode != NL80211_IFTYPE_ADHOC)))
		return;

>>>>>>> refs/remotes/origin/cm-10.0
	if (rx_stats->rs_rssi != ATH9K_RSSI_BAD && !rx_stats->rs_moreaggr)
		ATH_RSSI_LPF(sc->last_rssi, rx_stats->rs_rssi);

	last_rssi = sc->last_rssi;
	if (likely(last_rssi != ATH_RSSI_DUMMY_MARKER))
<<<<<<< HEAD
		rx_stats->rs_rssi = ATH_EP_RND(last_rssi,
					      ATH_RSSI_EP_MULTIPLIER);
	if (rx_stats->rs_rssi < 0)
		rx_stats->rs_rssi = 0;

	/* Update Beacon RSSI, this is used by ANI. */
	ah->stats.avgbrssi = rx_stats->rs_rssi;
=======
		rssi = ATH_EP_RND(last_rssi, ATH_RSSI_EP_MULTIPLIER);
	if (rssi < 0)
		rssi = 0;

	/* Update Beacon RSSI, this is used by ANI. */
	ah->stats.avgbrssi = rssi;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int rssi = rx_stats->rs_rssi;

	/*
	 * RSSI is not available for subframes in an A-MPDU.
	 */
	if (rx_stats->rs_moreaggr) {
		rxs->flag |= RX_FLAG_NO_SIGNAL_VAL;
		return;
	}

	/*
	 * Check if the RSSI for the last subframe in an A-MPDU
	 * or an unaggregated frame is valid.
	 */
	if (rx_stats->rs_rssi == ATH9K_RSSI_BAD) {
		rxs->flag |= RX_FLAG_NO_SIGNAL_VAL;
		return;
	}

	/*
	 * Update Beacon RSSI, this is used by ANI.
	 */
	if (rx_stats->is_mybeacon &&
	    ((ah->opmode == NL80211_IFTYPE_STATION) ||
	     (ah->opmode == NL80211_IFTYPE_ADHOC))) {
		ATH_RSSI_LPF(sc->last_rssi, rx_stats->rs_rssi);
		last_rssi = sc->last_rssi;

		if (likely(last_rssi != ATH_RSSI_DUMMY_MARKER))
			rssi = ATH_EP_RND(last_rssi, ATH_RSSI_EP_MULTIPLIER);
		if (rssi < 0)
			rssi = 0;

		ah->stats.avgbrssi = rssi;
	}

	rxs->signal = ah->noise + rx_stats->rs_rssi;
}

static void ath9k_process_tsf(struct ath_rx_status *rs,
			      struct ieee80211_rx_status *rxs,
			      u64 tsf)
{
	u32 tsf_lower = tsf & 0xffffffff;

	rxs->mactime = (tsf & ~0xffffffffULL) | rs->rs_tstamp;
	if (rs->rs_tstamp > tsf_lower &&
	    unlikely(rs->rs_tstamp - tsf_lower > 0x10000000))
		rxs->mactime -= 0x100000000ULL;

	if (rs->rs_tstamp < tsf_lower &&
	    unlikely(tsf_lower - rs->rs_tstamp > 0x10000000))
		rxs->mactime += 0x100000000ULL;
}

#ifdef CONFIG_ATH9K_DEBUGFS
static s8 fix_rssi_inv_only(u8 rssi_val)
{
	if (rssi_val == 128)
		rssi_val = 0;
	return (s8) rssi_val;
}
#endif

/* returns 1 if this was a spectral frame, even if not handled. */
static int ath_process_fft(struct ath_softc *sc, struct ieee80211_hdr *hdr,
			   struct ath_rx_status *rs, u64 tsf)
{
#ifdef CONFIG_ATH9K_DEBUGFS
	struct ath_hw *ah = sc->sc_ah;
	u8 num_bins, *bins, *vdata = (u8 *)hdr;
	struct fft_sample_ht20 fft_sample_20;
	struct fft_sample_ht20_40 fft_sample_40;
	struct fft_sample_tlv *tlv;
	struct ath_radar_info *radar_info;
	int len = rs->rs_datalen;
	int dc_pos;
	u16 fft_len, length, freq = ah->curchan->chan->center_freq;
	enum nl80211_channel_type chan_type;

	/* AR9280 and before report via ATH9K_PHYERR_RADAR, AR93xx and newer
	 * via ATH9K_PHYERR_SPECTRAL. Haven't seen ATH9K_PHYERR_FALSE_RADAR_EXT
	 * yet, but this is supposed to be possible as well.
	 */
	if (rs->rs_phyerr != ATH9K_PHYERR_RADAR &&
	    rs->rs_phyerr != ATH9K_PHYERR_FALSE_RADAR_EXT &&
	    rs->rs_phyerr != ATH9K_PHYERR_SPECTRAL)
		return 0;

	/* check if spectral scan bit is set. This does not have to be checked
	 * if received through a SPECTRAL phy error, but shouldn't hurt.
	 */
	radar_info = ((struct ath_radar_info *)&vdata[len]) - 1;
	if (!(radar_info->pulse_bw_info & SPECTRAL_SCAN_BITMASK))
		return 0;

	chan_type = cfg80211_get_chandef_type(&sc->hw->conf.chandef);
	if ((chan_type == NL80211_CHAN_HT40MINUS) ||
	    (chan_type == NL80211_CHAN_HT40PLUS)) {
		fft_len = SPECTRAL_HT20_40_TOTAL_DATA_LEN;
		num_bins = SPECTRAL_HT20_40_NUM_BINS;
		bins = (u8 *)fft_sample_40.data;
	} else {
		fft_len = SPECTRAL_HT20_TOTAL_DATA_LEN;
		num_bins = SPECTRAL_HT20_NUM_BINS;
		bins = (u8 *)fft_sample_20.data;
	}

	/* Variation in the data length is possible and will be fixed later */
	if ((len > fft_len + 2) || (len < fft_len - 1))
		return 1;

	switch (len - fft_len) {
	case 0:
		/* length correct, nothing to do. */
		memcpy(bins, vdata, num_bins);
		break;
	case -1:
		/* first byte missing, duplicate it. */
		memcpy(&bins[1], vdata, num_bins - 1);
		bins[0] = vdata[0];
		break;
	case 2:
		/* MAC added 2 extra bytes at bin 30 and 32, remove them. */
		memcpy(bins, vdata, 30);
		bins[30] = vdata[31];
		memcpy(&bins[31], &vdata[33], num_bins - 31);
		break;
	case 1:
		/* MAC added 2 extra bytes AND first byte is missing. */
		bins[0] = vdata[0];
		memcpy(&bins[1], vdata, 30);
		bins[31] = vdata[31];
		memcpy(&bins[32], &vdata[33], num_bins - 32);
		break;
	default:
		return 1;
	}

	/* DC value (value in the middle) is the blind spot of the spectral
	 * sample and invalid, interpolate it.
	 */
	dc_pos = num_bins / 2;
	bins[dc_pos] = (bins[dc_pos + 1] + bins[dc_pos - 1]) / 2;

	if ((chan_type == NL80211_CHAN_HT40MINUS) ||
	    (chan_type == NL80211_CHAN_HT40PLUS)) {
		s8 lower_rssi, upper_rssi;
		s16 ext_nf;
		u8 lower_max_index, upper_max_index;
		u8 lower_bitmap_w, upper_bitmap_w;
		u16 lower_mag, upper_mag;
		struct ath9k_hw_cal_data *caldata = ah->caldata;
		struct ath_ht20_40_mag_info *mag_info;

		if (caldata)
			ext_nf = ath9k_hw_getchan_noise(ah, ah->curchan,
					caldata->nfCalHist[3].privNF);
		else
			ext_nf = ATH_DEFAULT_NOISE_FLOOR;

		length = sizeof(fft_sample_40) - sizeof(struct fft_sample_tlv);
		fft_sample_40.tlv.type = ATH_FFT_SAMPLE_HT20_40;
		fft_sample_40.tlv.length = __cpu_to_be16(length);
		fft_sample_40.freq = __cpu_to_be16(freq);
		fft_sample_40.channel_type = chan_type;

		if (chan_type == NL80211_CHAN_HT40PLUS) {
			lower_rssi = fix_rssi_inv_only(rs->rs_rssi_ctl0);
			upper_rssi = fix_rssi_inv_only(rs->rs_rssi_ext0);

			fft_sample_40.lower_noise = ah->noise;
			fft_sample_40.upper_noise = ext_nf;
		} else {
			lower_rssi = fix_rssi_inv_only(rs->rs_rssi_ext0);
			upper_rssi = fix_rssi_inv_only(rs->rs_rssi_ctl0);

			fft_sample_40.lower_noise = ext_nf;
			fft_sample_40.upper_noise = ah->noise;
		}
		fft_sample_40.lower_rssi = lower_rssi;
		fft_sample_40.upper_rssi = upper_rssi;

		mag_info = ((struct ath_ht20_40_mag_info *)radar_info) - 1;
		lower_mag = spectral_max_magnitude(mag_info->lower_bins);
		upper_mag = spectral_max_magnitude(mag_info->upper_bins);
		fft_sample_40.lower_max_magnitude = __cpu_to_be16(lower_mag);
		fft_sample_40.upper_max_magnitude = __cpu_to_be16(upper_mag);
		lower_max_index = spectral_max_index(mag_info->lower_bins);
		upper_max_index = spectral_max_index(mag_info->upper_bins);
		fft_sample_40.lower_max_index = lower_max_index;
		fft_sample_40.upper_max_index = upper_max_index;
		lower_bitmap_w = spectral_bitmap_weight(mag_info->lower_bins);
		upper_bitmap_w = spectral_bitmap_weight(mag_info->upper_bins);
		fft_sample_40.lower_bitmap_weight = lower_bitmap_w;
		fft_sample_40.upper_bitmap_weight = upper_bitmap_w;
		fft_sample_40.max_exp = mag_info->max_exp & 0xf;

		fft_sample_40.tsf = __cpu_to_be64(tsf);

		tlv = (struct fft_sample_tlv *)&fft_sample_40;
	} else {
		u8 max_index, bitmap_w;
		u16 magnitude;
		struct ath_ht20_mag_info *mag_info;

		length = sizeof(fft_sample_20) - sizeof(struct fft_sample_tlv);
		fft_sample_20.tlv.type = ATH_FFT_SAMPLE_HT20;
		fft_sample_20.tlv.length = __cpu_to_be16(length);
		fft_sample_20.freq = __cpu_to_be16(freq);

		fft_sample_20.rssi = fix_rssi_inv_only(rs->rs_rssi_ctl0);
		fft_sample_20.noise = ah->noise;

		mag_info = ((struct ath_ht20_mag_info *)radar_info) - 1;
		magnitude = spectral_max_magnitude(mag_info->all_bins);
		fft_sample_20.max_magnitude = __cpu_to_be16(magnitude);
		max_index = spectral_max_index(mag_info->all_bins);
		fft_sample_20.max_index = max_index;
		bitmap_w = spectral_bitmap_weight(mag_info->all_bins);
		fft_sample_20.bitmap_weight = bitmap_w;
		fft_sample_20.max_exp = mag_info->max_exp & 0xf;

		fft_sample_20.tsf = __cpu_to_be64(tsf);

		tlv = (struct fft_sample_tlv *)&fft_sample_20;
	}

	ath_debug_send_fft_sample(sc, tlv);
	return 1;
#else
	return 0;
#endif
}

static bool ath9k_is_mybeacon(struct ath_softc *sc, struct ieee80211_hdr *hdr)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);

	if (ieee80211_is_beacon(hdr->frame_control)) {
		RX_STAT_INC(rx_beacons);
		if (!is_zero_ether_addr(common->curbssid) &&
		    ether_addr_equal(hdr->addr3, common->curbssid))
			return true;
	}

	return false;
>>>>>>> refs/remotes/origin/master
}

/*
 * For Decrypt or Demic errors, we only mark packet status here and always push
 * up the frame up to let mac80211 handle the actual error case, be it no
 * decryption key or real decryption error. This let us keep statistics there.
 */
<<<<<<< HEAD
static int ath9k_rx_skb_preprocess(struct ath_common *common,
				   struct ieee80211_hw *hw,
				   struct ieee80211_hdr *hdr,
				   struct ath_rx_status *rx_stats,
				   struct ieee80211_rx_status *rx_status,
				   bool *decrypt_error)
{
<<<<<<< HEAD
	memset(rx_status, 0, sizeof(struct ieee80211_rx_status));
=======
	struct ath_hw *ah = common->ah;
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * everything but the rate is checked here, the rate check is done
	 * separately to avoid doing two lookups for a rate for each frame.
	 */
	if (!ath9k_rx_accept(common, hdr, rx_status, rx_stats, decrypt_error))
		return -EINVAL;
=======
static int ath9k_rx_skb_preprocess(struct ath_softc *sc,
				   struct sk_buff *skb,
				   struct ath_rx_status *rx_stats,
				   struct ieee80211_rx_status *rx_status,
				   bool *decrypt_error, u64 tsf)
{
	struct ieee80211_hw *hw = sc->hw;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ieee80211_hdr *hdr;
	bool discard_current = sc->rx.discard_next;
	int ret = 0;

	/*
	 * Discard corrupt descriptors which are marked in
	 * ath_get_next_rx_buf().
	 */
	sc->rx.discard_next = rx_stats->rs_more;
	if (discard_current)
		return -EINVAL;

	/*
	 * Discard zero-length packets.
	 */
	if (!rx_stats->rs_datalen) {
		RX_STAT_INC(rx_len_err);
		return -EINVAL;
	}

        /*
         * rs_status follows rs_datalen so if rs_datalen is too large
         * we can take a hint that hardware corrupted it, so ignore
         * those frames.
         */
	if (rx_stats->rs_datalen > (common->rx_bufsize - ah->caps.rx_status_len)) {
		RX_STAT_INC(rx_len_err);
		return -EINVAL;
	}
>>>>>>> refs/remotes/origin/master

	/* Only use status info from the last fragment */
	if (rx_stats->rs_more)
		return 0;

<<<<<<< HEAD
	ath9k_process_rssi(common, hw, hdr, rx_stats);

	if (ath9k_process_rate(common, hw, rx_stats, rx_status))
		return -EINVAL;

	rx_status->band = hw->conf.channel->band;
	rx_status->freq = hw->conf.channel->center_freq;
<<<<<<< HEAD
	rx_status->signal = ATH_DEFAULT_NOISE_FLOOR + rx_stats->rs_rssi;
	rx_status->antenna = rx_stats->rs_antenna;
	rx_status->flag |= RX_FLAG_MACTIME_MPDU;
=======
	rx_status->signal = ah->noise + rx_stats->rs_rssi;
	rx_status->antenna = rx_stats->rs_antenna;
	rx_status->flag |= RX_FLAG_MACTIME_MPDU;
	if (rx_stats->rs_moreaggr)
		rx_status->flag |= RX_FLAG_NO_SIGNAL_VAL;
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
=======
	/*
	 * Return immediately if the RX descriptor has been marked
	 * as corrupt based on the various error bits.
	 *
	 * This is different from the other corrupt descriptor
	 * condition handled above.
	 */
	if (rx_stats->rs_status & ATH9K_RXERR_CORRUPT_DESC) {
		ret = -EINVAL;
		goto exit;
	}

	hdr = (struct ieee80211_hdr *) (skb->data + ah->caps.rx_status_len);

	ath9k_process_tsf(rx_stats, rx_status, tsf);
	ath_debug_stat_rx(sc, rx_stats);

	/*
	 * Process PHY errors and return so that the packet
	 * can be dropped.
	 */
	if (rx_stats->rs_status & ATH9K_RXERR_PHY) {
		ath9k_dfs_process_phyerr(sc, hdr, rx_stats, rx_status->mactime);
		if (ath_process_fft(sc, hdr, rx_stats, rx_status->mactime))
			RX_STAT_INC(rx_spectral);

		ret = -EINVAL;
		goto exit;
	}

	/*
	 * everything but the rate is checked here, the rate check is done
	 * separately to avoid doing two lookups for a rate for each frame.
	 */
	if (!ath9k_rx_accept(common, hdr, rx_status, rx_stats, decrypt_error)) {
		ret = -EINVAL;
		goto exit;
	}

	rx_stats->is_mybeacon = ath9k_is_mybeacon(sc, hdr);
	if (rx_stats->is_mybeacon) {
		sc->hw_busy_count = 0;
		ath_start_rx_poll(sc, 3);
	}

	if (ath9k_process_rate(common, hw, rx_stats, rx_status)) {
		ret =-EINVAL;
		goto exit;
	}

	ath9k_process_rssi(common, hw, rx_stats, rx_status);

	rx_status->band = hw->conf.chandef.chan->band;
	rx_status->freq = hw->conf.chandef.chan->center_freq;
	rx_status->antenna = rx_stats->rs_antenna;
	rx_status->flag |= RX_FLAG_MACTIME_END;

#ifdef CONFIG_ATH9K_BTCOEX_SUPPORT
	if (ieee80211_is_data_present(hdr->frame_control) &&
	    !ieee80211_is_qos_nullfunc(hdr->frame_control))
		sc->rx.num_pkts++;
#endif

exit:
	sc->rx.discard_next = false;
	return ret;
>>>>>>> refs/remotes/origin/master
}

static void ath9k_rx_skb_postprocess(struct ath_common *common,
				     struct sk_buff *skb,
				     struct ath_rx_status *rx_stats,
				     struct ieee80211_rx_status *rxs,
				     bool decrypt_error)
{
	struct ath_hw *ah = common->ah;
	struct ieee80211_hdr *hdr;
	int hdrlen, padpos, padsize;
	u8 keyix;
	__le16 fc;

	/* see if any padding is done by the hw and remove it */
	hdr = (struct ieee80211_hdr *) skb->data;
	hdrlen = ieee80211_get_hdrlen_from_skb(skb);
	fc = hdr->frame_control;
<<<<<<< HEAD
	padpos = ath9k_cmn_padpos(hdr->frame_control);
=======
	padpos = ieee80211_hdrlen(fc);
>>>>>>> refs/remotes/origin/master

	/* The MAC header is padded to have 32-bit boundary if the
	 * packet payload is non-zero. The general calculation for
	 * padsize would take into account odd header lengths:
	 * padsize = (4 - padpos % 4) % 4; However, since only
	 * even-length headers are used, padding can only be 0 or 2
	 * bytes and we can optimize this a bit. In addition, we must
	 * not try to remove padding from short control frames that do
	 * not have payload. */
	padsize = padpos & 3;
	if (padsize && skb->len>=padpos+padsize+FCS_LEN) {
		memmove(skb->data + padsize, skb->data, padpos);
		skb_pull(skb, padsize);
	}

	keyix = rx_stats->rs_keyix;

	if (!(keyix == ATH9K_RXKEYIX_INVALID) && !decrypt_error &&
	    ieee80211_has_protected(fc)) {
		rxs->flag |= RX_FLAG_DECRYPTED;
	} else if (ieee80211_has_protected(fc)
		   && !decrypt_error && skb->len >= hdrlen + 4) {
		keyix = skb->data[hdrlen + 3] >> 6;

		if (test_bit(keyix, common->keymap))
			rxs->flag |= RX_FLAG_DECRYPTED;
	}
	if (ah->sw_mgmt_crypto &&
	    (rxs->flag & RX_FLAG_DECRYPTED) &&
	    ieee80211_is_mgmt(fc))
		/* Use software decrypt for management frames. */
		rxs->flag &= ~RX_FLAG_DECRYPTED;
}

<<<<<<< HEAD
static void ath_lnaconf_alt_good_scan(struct ath_ant_comb *antcomb,
				      struct ath_hw_antcomb_conf ant_conf,
				      int main_rssi_avg)
{
	antcomb->quick_scan_cnt = 0;

	if (ant_conf.main_lna_conf == ATH_ANT_DIV_COMB_LNA2)
		antcomb->rssi_lna2 = main_rssi_avg;
	else if (ant_conf.main_lna_conf == ATH_ANT_DIV_COMB_LNA1)
		antcomb->rssi_lna1 = main_rssi_avg;

	switch ((ant_conf.main_lna_conf << 4) | ant_conf.alt_lna_conf) {
<<<<<<< HEAD
	case (0x10): /* LNA2 A-B */
=======
	case 0x10: /* LNA2 A-B */
>>>>>>> refs/remotes/origin/cm-10.0
		antcomb->main_conf = ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2;
		antcomb->first_quick_scan_conf =
			ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2;
		antcomb->second_quick_scan_conf = ATH_ANT_DIV_COMB_LNA1;
		break;
<<<<<<< HEAD
	case (0x20): /* LNA1 A-B */
=======
	case 0x20: /* LNA1 A-B */
>>>>>>> refs/remotes/origin/cm-10.0
		antcomb->main_conf = ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2;
		antcomb->first_quick_scan_conf =
			ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2;
		antcomb->second_quick_scan_conf = ATH_ANT_DIV_COMB_LNA2;
		break;
<<<<<<< HEAD
	case (0x21): /* LNA1 LNA2 */
=======
	case 0x21: /* LNA1 LNA2 */
>>>>>>> refs/remotes/origin/cm-10.0
		antcomb->main_conf = ATH_ANT_DIV_COMB_LNA2;
		antcomb->first_quick_scan_conf =
			ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2;
		antcomb->second_quick_scan_conf =
			ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2;
		break;
<<<<<<< HEAD
	case (0x12): /* LNA2 LNA1 */
=======
	case 0x12: /* LNA2 LNA1 */
>>>>>>> refs/remotes/origin/cm-10.0
		antcomb->main_conf = ATH_ANT_DIV_COMB_LNA1;
		antcomb->first_quick_scan_conf =
			ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2;
		antcomb->second_quick_scan_conf =
			ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2;
		break;
<<<<<<< HEAD
	case (0x13): /* LNA2 A+B */
=======
	case 0x13: /* LNA2 A+B */
>>>>>>> refs/remotes/origin/cm-10.0
		antcomb->main_conf = ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2;
		antcomb->first_quick_scan_conf =
			ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2;
		antcomb->second_quick_scan_conf = ATH_ANT_DIV_COMB_LNA1;
		break;
<<<<<<< HEAD
	case (0x23): /* LNA1 A+B */
=======
	case 0x23: /* LNA1 A+B */
>>>>>>> refs/remotes/origin/cm-10.0
		antcomb->main_conf = ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2;
		antcomb->first_quick_scan_conf =
			ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2;
		antcomb->second_quick_scan_conf = ATH_ANT_DIV_COMB_LNA2;
		break;
	default:
		break;
	}
}

static void ath_select_ant_div_from_quick_scan(struct ath_ant_comb *antcomb,
				struct ath_hw_antcomb_conf *div_ant_conf,
				int main_rssi_avg, int alt_rssi_avg,
				int alt_ratio)
{
	/* alt_good */
	switch (antcomb->quick_scan_cnt) {
	case 0:
		/* set alt to main, and alt to first conf */
		div_ant_conf->main_lna_conf = antcomb->main_conf;
		div_ant_conf->alt_lna_conf = antcomb->first_quick_scan_conf;
		break;
	case 1:
		/* set alt to main, and alt to first conf */
		div_ant_conf->main_lna_conf = antcomb->main_conf;
		div_ant_conf->alt_lna_conf = antcomb->second_quick_scan_conf;
		antcomb->rssi_first = main_rssi_avg;
		antcomb->rssi_second = alt_rssi_avg;

		if (antcomb->main_conf == ATH_ANT_DIV_COMB_LNA1) {
			/* main is LNA1 */
			if (ath_is_alt_ant_ratio_better(alt_ratio,
						ATH_ANT_DIV_COMB_LNA1_DELTA_HI,
						ATH_ANT_DIV_COMB_LNA1_DELTA_LOW,
						main_rssi_avg, alt_rssi_avg,
						antcomb->total_pkt_count))
				antcomb->first_ratio = true;
			else
				antcomb->first_ratio = false;
		} else if (antcomb->main_conf == ATH_ANT_DIV_COMB_LNA2) {
			if (ath_is_alt_ant_ratio_better(alt_ratio,
						ATH_ANT_DIV_COMB_LNA1_DELTA_MID,
						ATH_ANT_DIV_COMB_LNA1_DELTA_LOW,
						main_rssi_avg, alt_rssi_avg,
						antcomb->total_pkt_count))
				antcomb->first_ratio = true;
			else
				antcomb->first_ratio = false;
		} else {
			if ((((alt_ratio >= ATH_ANT_DIV_COMB_ALT_ANT_RATIO2) &&
			    (alt_rssi_avg > main_rssi_avg +
			    ATH_ANT_DIV_COMB_LNA1_DELTA_HI)) ||
			    (alt_rssi_avg > main_rssi_avg)) &&
			    (antcomb->total_pkt_count > 50))
				antcomb->first_ratio = true;
			else
				antcomb->first_ratio = false;
		}
		break;
	case 2:
		antcomb->alt_good = false;
		antcomb->scan_not_start = false;
		antcomb->scan = false;
		antcomb->rssi_first = main_rssi_avg;
		antcomb->rssi_third = alt_rssi_avg;

		if (antcomb->second_quick_scan_conf == ATH_ANT_DIV_COMB_LNA1)
			antcomb->rssi_lna1 = alt_rssi_avg;
		else if (antcomb->second_quick_scan_conf ==
			 ATH_ANT_DIV_COMB_LNA2)
			antcomb->rssi_lna2 = alt_rssi_avg;
		else if (antcomb->second_quick_scan_conf ==
			 ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2) {
			if (antcomb->main_conf == ATH_ANT_DIV_COMB_LNA2)
				antcomb->rssi_lna2 = main_rssi_avg;
			else if (antcomb->main_conf == ATH_ANT_DIV_COMB_LNA1)
				antcomb->rssi_lna1 = main_rssi_avg;
		}

		if (antcomb->rssi_lna2 > antcomb->rssi_lna1 +
		    ATH_ANT_DIV_COMB_LNA1_LNA2_SWITCH_DELTA)
			div_ant_conf->main_lna_conf = ATH_ANT_DIV_COMB_LNA2;
		else
			div_ant_conf->main_lna_conf = ATH_ANT_DIV_COMB_LNA1;

		if (antcomb->main_conf == ATH_ANT_DIV_COMB_LNA1) {
			if (ath_is_alt_ant_ratio_better(alt_ratio,
						ATH_ANT_DIV_COMB_LNA1_DELTA_HI,
						ATH_ANT_DIV_COMB_LNA1_DELTA_LOW,
						main_rssi_avg, alt_rssi_avg,
						antcomb->total_pkt_count))
				antcomb->second_ratio = true;
			else
				antcomb->second_ratio = false;
		} else if (antcomb->main_conf == ATH_ANT_DIV_COMB_LNA2) {
			if (ath_is_alt_ant_ratio_better(alt_ratio,
						ATH_ANT_DIV_COMB_LNA1_DELTA_MID,
						ATH_ANT_DIV_COMB_LNA1_DELTA_LOW,
						main_rssi_avg, alt_rssi_avg,
						antcomb->total_pkt_count))
				antcomb->second_ratio = true;
			else
				antcomb->second_ratio = false;
		} else {
			if ((((alt_ratio >= ATH_ANT_DIV_COMB_ALT_ANT_RATIO2) &&
			    (alt_rssi_avg > main_rssi_avg +
			    ATH_ANT_DIV_COMB_LNA1_DELTA_HI)) ||
			    (alt_rssi_avg > main_rssi_avg)) &&
			    (antcomb->total_pkt_count > 50))
				antcomb->second_ratio = true;
			else
				antcomb->second_ratio = false;
		}

		/* set alt to the conf with maximun ratio */
		if (antcomb->first_ratio && antcomb->second_ratio) {
			if (antcomb->rssi_second > antcomb->rssi_third) {
				/* first alt*/
				if ((antcomb->first_quick_scan_conf ==
				    ATH_ANT_DIV_COMB_LNA1) ||
				    (antcomb->first_quick_scan_conf ==
				    ATH_ANT_DIV_COMB_LNA2))
					/* Set alt LNA1 or LNA2*/
					if (div_ant_conf->main_lna_conf ==
					    ATH_ANT_DIV_COMB_LNA2)
						div_ant_conf->alt_lna_conf =
							ATH_ANT_DIV_COMB_LNA1;
					else
						div_ant_conf->alt_lna_conf =
							ATH_ANT_DIV_COMB_LNA2;
				else
					/* Set alt to A+B or A-B */
					div_ant_conf->alt_lna_conf =
						antcomb->first_quick_scan_conf;
			} else if ((antcomb->second_quick_scan_conf ==
				   ATH_ANT_DIV_COMB_LNA1) ||
				   (antcomb->second_quick_scan_conf ==
				   ATH_ANT_DIV_COMB_LNA2)) {
				/* Set alt LNA1 or LNA2 */
				if (div_ant_conf->main_lna_conf ==
				    ATH_ANT_DIV_COMB_LNA2)
					div_ant_conf->alt_lna_conf =
						ATH_ANT_DIV_COMB_LNA1;
				else
					div_ant_conf->alt_lna_conf =
						ATH_ANT_DIV_COMB_LNA2;
			} else {
				/* Set alt to A+B or A-B */
				div_ant_conf->alt_lna_conf =
					antcomb->second_quick_scan_conf;
			}
		} else if (antcomb->first_ratio) {
			/* first alt */
			if ((antcomb->first_quick_scan_conf ==
			    ATH_ANT_DIV_COMB_LNA1) ||
			    (antcomb->first_quick_scan_conf ==
			    ATH_ANT_DIV_COMB_LNA2))
					/* Set alt LNA1 or LNA2 */
				if (div_ant_conf->main_lna_conf ==
				    ATH_ANT_DIV_COMB_LNA2)
					div_ant_conf->alt_lna_conf =
							ATH_ANT_DIV_COMB_LNA1;
				else
					div_ant_conf->alt_lna_conf =
							ATH_ANT_DIV_COMB_LNA2;
			else
				/* Set alt to A+B or A-B */
				div_ant_conf->alt_lna_conf =
						antcomb->first_quick_scan_conf;
		} else if (antcomb->second_ratio) {
				/* second alt */
			if ((antcomb->second_quick_scan_conf ==
			    ATH_ANT_DIV_COMB_LNA1) ||
			    (antcomb->second_quick_scan_conf ==
			    ATH_ANT_DIV_COMB_LNA2))
				/* Set alt LNA1 or LNA2 */
				if (div_ant_conf->main_lna_conf ==
				    ATH_ANT_DIV_COMB_LNA2)
					div_ant_conf->alt_lna_conf =
						ATH_ANT_DIV_COMB_LNA1;
				else
					div_ant_conf->alt_lna_conf =
						ATH_ANT_DIV_COMB_LNA2;
			else
				/* Set alt to A+B or A-B */
				div_ant_conf->alt_lna_conf =
						antcomb->second_quick_scan_conf;
		} else {
			/* main is largest */
			if ((antcomb->main_conf == ATH_ANT_DIV_COMB_LNA1) ||
			    (antcomb->main_conf == ATH_ANT_DIV_COMB_LNA2))
				/* Set alt LNA1 or LNA2 */
				if (div_ant_conf->main_lna_conf ==
				    ATH_ANT_DIV_COMB_LNA2)
					div_ant_conf->alt_lna_conf =
							ATH_ANT_DIV_COMB_LNA1;
				else
					div_ant_conf->alt_lna_conf =
							ATH_ANT_DIV_COMB_LNA2;
			else
				/* Set alt to A+B or A-B */
				div_ant_conf->alt_lna_conf = antcomb->main_conf;
		}
		break;
	default:
		break;
	}
}

static void ath_ant_div_conf_fast_divbias(struct ath_hw_antcomb_conf *ant_conf,
		struct ath_ant_comb *antcomb, int alt_ratio)
{
	if (ant_conf->div_group == 0) {
		/* Adjust the fast_div_bias based on main and alt lna conf */
		switch ((ant_conf->main_lna_conf << 4) |
				ant_conf->alt_lna_conf) {
<<<<<<< HEAD
		case (0x01): /* A-B LNA2 */
			ant_conf->fast_div_bias = 0x3b;
			break;
		case (0x02): /* A-B LNA1 */
			ant_conf->fast_div_bias = 0x3d;
			break;
		case (0x03): /* A-B A+B */
			ant_conf->fast_div_bias = 0x1;
			break;
		case (0x10): /* LNA2 A-B */
			ant_conf->fast_div_bias = 0x7;
			break;
		case (0x12): /* LNA2 LNA1 */
			ant_conf->fast_div_bias = 0x2;
			break;
		case (0x13): /* LNA2 A+B */
			ant_conf->fast_div_bias = 0x7;
			break;
		case (0x20): /* LNA1 A-B */
			ant_conf->fast_div_bias = 0x6;
			break;
		case (0x21): /* LNA1 LNA2 */
			ant_conf->fast_div_bias = 0x0;
			break;
		case (0x23): /* LNA1 A+B */
			ant_conf->fast_div_bias = 0x6;
			break;
		case (0x30): /* A+B A-B */
			ant_conf->fast_div_bias = 0x1;
			break;
		case (0x31): /* A+B LNA2 */
			ant_conf->fast_div_bias = 0x3b;
			break;
		case (0x32): /* A+B LNA1 */
=======
		case 0x01: /* A-B LNA2 */
			ant_conf->fast_div_bias = 0x3b;
			break;
		case 0x02: /* A-B LNA1 */
			ant_conf->fast_div_bias = 0x3d;
			break;
		case 0x03: /* A-B A+B */
			ant_conf->fast_div_bias = 0x1;
			break;
		case 0x10: /* LNA2 A-B */
			ant_conf->fast_div_bias = 0x7;
			break;
		case 0x12: /* LNA2 LNA1 */
			ant_conf->fast_div_bias = 0x2;
			break;
		case 0x13: /* LNA2 A+B */
			ant_conf->fast_div_bias = 0x7;
			break;
		case 0x20: /* LNA1 A-B */
			ant_conf->fast_div_bias = 0x6;
			break;
		case 0x21: /* LNA1 LNA2 */
			ant_conf->fast_div_bias = 0x0;
			break;
		case 0x23: /* LNA1 A+B */
			ant_conf->fast_div_bias = 0x6;
			break;
		case 0x30: /* A+B A-B */
			ant_conf->fast_div_bias = 0x1;
			break;
		case 0x31: /* A+B LNA2 */
			ant_conf->fast_div_bias = 0x3b;
			break;
		case 0x32: /* A+B LNA1 */
>>>>>>> refs/remotes/origin/cm-10.0
			ant_conf->fast_div_bias = 0x3d;
			break;
		default:
			break;
		}
<<<<<<< HEAD
=======
	} else if (ant_conf->div_group == 1) {
		/* Adjust the fast_div_bias based on main and alt_lna_conf */
		switch ((ant_conf->main_lna_conf << 4) |
			ant_conf->alt_lna_conf) {
		case 0x01: /* A-B LNA2 */
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
		case 0x02: /* A-B LNA1 */
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
		case 0x03: /* A-B A+B */
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
		case 0x10: /* LNA2 A-B */
			if (!(antcomb->scan) &&
			    (alt_ratio > ATH_ANT_DIV_COMB_ALT_ANT_RATIO))
				ant_conf->fast_div_bias = 0x3f;
			else
				ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
		case 0x12: /* LNA2 LNA1 */
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
		case 0x13: /* LNA2 A+B */
			if (!(antcomb->scan) &&
			    (alt_ratio > ATH_ANT_DIV_COMB_ALT_ANT_RATIO))
				ant_conf->fast_div_bias = 0x3f;
			else
				ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
		case 0x20: /* LNA1 A-B */
			if (!(antcomb->scan) &&
			    (alt_ratio > ATH_ANT_DIV_COMB_ALT_ANT_RATIO))
				ant_conf->fast_div_bias = 0x3f;
			else
				ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
		case 0x21: /* LNA1 LNA2 */
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
		case 0x23: /* LNA1 A+B */
			if (!(antcomb->scan) &&
			    (alt_ratio > ATH_ANT_DIV_COMB_ALT_ANT_RATIO))
				ant_conf->fast_div_bias = 0x3f;
			else
				ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
		case 0x30: /* A+B A-B */
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
		case 0x31: /* A+B LNA2 */
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
		case 0x32: /* A+B LNA1 */
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
		default:
			break;
		}
>>>>>>> refs/remotes/origin/cm-10.0
	} else if (ant_conf->div_group == 2) {
		/* Adjust the fast_div_bias based on main and alt_lna_conf */
		switch ((ant_conf->main_lna_conf << 4) |
				ant_conf->alt_lna_conf) {
<<<<<<< HEAD
		case (0x01): /* A-B LNA2 */
=======
		case 0x01: /* A-B LNA2 */
>>>>>>> refs/remotes/origin/cm-10.0
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
<<<<<<< HEAD
		case (0x02): /* A-B LNA1 */
=======
		case 0x02: /* A-B LNA1 */
>>>>>>> refs/remotes/origin/cm-10.0
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
<<<<<<< HEAD
		case (0x03): /* A-B A+B */
=======
		case 0x03: /* A-B A+B */
>>>>>>> refs/remotes/origin/cm-10.0
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
<<<<<<< HEAD
		case (0x10): /* LNA2 A-B */
=======
		case 0x10: /* LNA2 A-B */
>>>>>>> refs/remotes/origin/cm-10.0
			if (!(antcomb->scan) &&
				(alt_ratio > ATH_ANT_DIV_COMB_ALT_ANT_RATIO))
				ant_conf->fast_div_bias = 0x1;
			else
				ant_conf->fast_div_bias = 0x2;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
<<<<<<< HEAD
		case (0x12): /* LNA2 LNA1 */
=======
		case 0x12: /* LNA2 LNA1 */
>>>>>>> refs/remotes/origin/cm-10.0
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
<<<<<<< HEAD
		case (0x13): /* LNA2 A+B */
=======
		case 0x13: /* LNA2 A+B */
>>>>>>> refs/remotes/origin/cm-10.0
			if (!(antcomb->scan) &&
				(alt_ratio > ATH_ANT_DIV_COMB_ALT_ANT_RATIO))
				ant_conf->fast_div_bias = 0x1;
			else
				ant_conf->fast_div_bias = 0x2;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
<<<<<<< HEAD
		case (0x20): /* LNA1 A-B */
=======
		case 0x20: /* LNA1 A-B */
>>>>>>> refs/remotes/origin/cm-10.0
			if (!(antcomb->scan) &&
				(alt_ratio > ATH_ANT_DIV_COMB_ALT_ANT_RATIO))
				ant_conf->fast_div_bias = 0x1;
			else
				ant_conf->fast_div_bias = 0x2;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
<<<<<<< HEAD
		case (0x21): /* LNA1 LNA2 */
=======
		case 0x21: /* LNA1 LNA2 */
>>>>>>> refs/remotes/origin/cm-10.0
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
<<<<<<< HEAD
		case (0x23): /* LNA1 A+B */
=======
		case 0x23: /* LNA1 A+B */
>>>>>>> refs/remotes/origin/cm-10.0
			if (!(antcomb->scan) &&
				(alt_ratio > ATH_ANT_DIV_COMB_ALT_ANT_RATIO))
				ant_conf->fast_div_bias = 0x1;
			else
				ant_conf->fast_div_bias = 0x2;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
<<<<<<< HEAD
		case (0x30): /* A+B A-B */
=======
		case 0x30: /* A+B A-B */
>>>>>>> refs/remotes/origin/cm-10.0
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
<<<<<<< HEAD
		case (0x31): /* A+B LNA2 */
=======
		case 0x31: /* A+B LNA2 */
>>>>>>> refs/remotes/origin/cm-10.0
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
<<<<<<< HEAD
		case (0x32): /* A+B LNA1 */
=======
		case 0x32: /* A+B LNA1 */
>>>>>>> refs/remotes/origin/cm-10.0
			ant_conf->fast_div_bias = 0x1;
			ant_conf->main_gaintb = 0;
			ant_conf->alt_gaintb = 0;
			break;
		default:
			break;
		}
<<<<<<< HEAD

	}

=======
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

/* Antenna diversity and combining */
static void ath_ant_comb_scan(struct ath_softc *sc, struct ath_rx_status *rs)
{
	struct ath_hw_antcomb_conf div_ant_conf;
	struct ath_ant_comb *antcomb = &sc->ant_comb;
	int alt_ratio = 0, alt_rssi_avg = 0, main_rssi_avg = 0, curr_alt_set;
	int curr_main_set;
	int main_rssi = rs->rs_rssi_ctl0;
	int alt_rssi = rs->rs_rssi_ctl1;
	int rx_ant_conf,  main_ant_conf;
	bool short_scan = false;

	rx_ant_conf = (rs->rs_rssi_ctl2 >> ATH_ANT_RX_CURRENT_SHIFT) &
		       ATH_ANT_RX_MASK;
	main_ant_conf = (rs->rs_rssi_ctl2 >> ATH_ANT_RX_MAIN_SHIFT) &
			 ATH_ANT_RX_MASK;

	/* Record packet only when both main_rssi and  alt_rssi is positive */
	if (main_rssi > 0 && alt_rssi > 0) {
		antcomb->total_pkt_count++;
		antcomb->main_total_rssi += main_rssi;
		antcomb->alt_total_rssi  += alt_rssi;
		if (main_ant_conf == rx_ant_conf)
			antcomb->main_recv_cnt++;
		else
			antcomb->alt_recv_cnt++;
	}

	/* Short scan check */
	if (antcomb->scan && antcomb->alt_good) {
		if (time_after(jiffies, antcomb->scan_start_time +
		    msecs_to_jiffies(ATH_ANT_DIV_COMB_SHORT_SCAN_INTR)))
			short_scan = true;
		else
			if (antcomb->total_pkt_count ==
			    ATH_ANT_DIV_COMB_SHORT_SCAN_PKTCOUNT) {
				alt_ratio = ((antcomb->alt_recv_cnt * 100) /
					    antcomb->total_pkt_count);
				if (alt_ratio < ATH_ANT_DIV_COMB_ALT_ANT_RATIO)
					short_scan = true;
			}
	}

	if (((antcomb->total_pkt_count < ATH_ANT_DIV_COMB_MAX_PKTCOUNT) ||
	    rs->rs_moreaggr) && !short_scan)
		return;

	if (antcomb->total_pkt_count) {
		alt_ratio = ((antcomb->alt_recv_cnt * 100) /
			     antcomb->total_pkt_count);
		main_rssi_avg = (antcomb->main_total_rssi /
				 antcomb->total_pkt_count);
		alt_rssi_avg = (antcomb->alt_total_rssi /
				 antcomb->total_pkt_count);
	}


	ath9k_hw_antdiv_comb_conf_get(sc->sc_ah, &div_ant_conf);
	curr_alt_set = div_ant_conf.alt_lna_conf;
	curr_main_set = div_ant_conf.main_lna_conf;

	antcomb->count++;

	if (antcomb->count == ATH_ANT_DIV_COMB_MAX_COUNT) {
		if (alt_ratio > ATH_ANT_DIV_COMB_ALT_ANT_RATIO) {
			ath_lnaconf_alt_good_scan(antcomb, div_ant_conf,
						  main_rssi_avg);
			antcomb->alt_good = true;
		} else {
			antcomb->alt_good = false;
		}

		antcomb->count = 0;
		antcomb->scan = true;
		antcomb->scan_not_start = true;
	}

	if (!antcomb->scan) {
		if (ath_ant_div_comb_alt_check(div_ant_conf.div_group,
					alt_ratio, curr_main_set, curr_alt_set,
					alt_rssi_avg, main_rssi_avg)) {
			if (curr_alt_set == ATH_ANT_DIV_COMB_LNA2) {
				/* Switch main and alt LNA */
				div_ant_conf.main_lna_conf =
						ATH_ANT_DIV_COMB_LNA2;
				div_ant_conf.alt_lna_conf  =
						ATH_ANT_DIV_COMB_LNA1;
			} else if (curr_alt_set == ATH_ANT_DIV_COMB_LNA1) {
				div_ant_conf.main_lna_conf =
						ATH_ANT_DIV_COMB_LNA1;
				div_ant_conf.alt_lna_conf  =
						ATH_ANT_DIV_COMB_LNA2;
			}

			goto div_comb_done;
		} else if ((curr_alt_set != ATH_ANT_DIV_COMB_LNA1) &&
			   (curr_alt_set != ATH_ANT_DIV_COMB_LNA2)) {
			/* Set alt to another LNA */
			if (curr_main_set == ATH_ANT_DIV_COMB_LNA2)
				div_ant_conf.alt_lna_conf =
						ATH_ANT_DIV_COMB_LNA1;
			else if (curr_main_set == ATH_ANT_DIV_COMB_LNA1)
				div_ant_conf.alt_lna_conf =
						ATH_ANT_DIV_COMB_LNA2;

			goto div_comb_done;
		}

		if ((alt_rssi_avg < (main_rssi_avg +
						div_ant_conf.lna1_lna2_delta)))
			goto div_comb_done;
	}

	if (!antcomb->scan_not_start) {
		switch (curr_alt_set) {
		case ATH_ANT_DIV_COMB_LNA2:
			antcomb->rssi_lna2 = alt_rssi_avg;
			antcomb->rssi_lna1 = main_rssi_avg;
			antcomb->scan = true;
			/* set to A+B */
			div_ant_conf.main_lna_conf =
				ATH_ANT_DIV_COMB_LNA1;
			div_ant_conf.alt_lna_conf  =
				ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2;
			break;
		case ATH_ANT_DIV_COMB_LNA1:
			antcomb->rssi_lna1 = alt_rssi_avg;
			antcomb->rssi_lna2 = main_rssi_avg;
			antcomb->scan = true;
			/* set to A+B */
			div_ant_conf.main_lna_conf = ATH_ANT_DIV_COMB_LNA2;
			div_ant_conf.alt_lna_conf  =
				ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2;
			break;
		case ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2:
			antcomb->rssi_add = alt_rssi_avg;
			antcomb->scan = true;
			/* set to A-B */
			div_ant_conf.alt_lna_conf =
				ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2;
			break;
		case ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2:
			antcomb->rssi_sub = alt_rssi_avg;
			antcomb->scan = false;
			if (antcomb->rssi_lna2 >
			    (antcomb->rssi_lna1 +
			    ATH_ANT_DIV_COMB_LNA1_LNA2_SWITCH_DELTA)) {
				/* use LNA2 as main LNA */
				if ((antcomb->rssi_add > antcomb->rssi_lna1) &&
				    (antcomb->rssi_add > antcomb->rssi_sub)) {
					/* set to A+B */
					div_ant_conf.main_lna_conf =
						ATH_ANT_DIV_COMB_LNA2;
					div_ant_conf.alt_lna_conf  =
						ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2;
				} else if (antcomb->rssi_sub >
					   antcomb->rssi_lna1) {
					/* set to A-B */
					div_ant_conf.main_lna_conf =
						ATH_ANT_DIV_COMB_LNA2;
					div_ant_conf.alt_lna_conf =
						ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2;
				} else {
					/* set to LNA1 */
					div_ant_conf.main_lna_conf =
						ATH_ANT_DIV_COMB_LNA2;
					div_ant_conf.alt_lna_conf =
						ATH_ANT_DIV_COMB_LNA1;
				}
			} else {
				/* use LNA1 as main LNA */
				if ((antcomb->rssi_add > antcomb->rssi_lna2) &&
				    (antcomb->rssi_add > antcomb->rssi_sub)) {
					/* set to A+B */
					div_ant_conf.main_lna_conf =
						ATH_ANT_DIV_COMB_LNA1;
					div_ant_conf.alt_lna_conf  =
						ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2;
				} else if (antcomb->rssi_sub >
					   antcomb->rssi_lna1) {
					/* set to A-B */
					div_ant_conf.main_lna_conf =
						ATH_ANT_DIV_COMB_LNA1;
					div_ant_conf.alt_lna_conf =
						ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2;
				} else {
					/* set to LNA2 */
					div_ant_conf.main_lna_conf =
						ATH_ANT_DIV_COMB_LNA1;
					div_ant_conf.alt_lna_conf =
						ATH_ANT_DIV_COMB_LNA2;
				}
			}
			break;
		default:
			break;
		}
	} else {
		if (!antcomb->alt_good) {
			antcomb->scan_not_start = false;
			/* Set alt to another LNA */
			if (curr_main_set == ATH_ANT_DIV_COMB_LNA2) {
				div_ant_conf.main_lna_conf =
						ATH_ANT_DIV_COMB_LNA2;
				div_ant_conf.alt_lna_conf =
						ATH_ANT_DIV_COMB_LNA1;
			} else if (curr_main_set == ATH_ANT_DIV_COMB_LNA1) {
				div_ant_conf.main_lna_conf =
						ATH_ANT_DIV_COMB_LNA1;
				div_ant_conf.alt_lna_conf =
						ATH_ANT_DIV_COMB_LNA2;
			}
			goto div_comb_done;
		}
	}

	ath_select_ant_div_from_quick_scan(antcomb, &div_ant_conf,
					   main_rssi_avg, alt_rssi_avg,
					   alt_ratio);

	antcomb->quick_scan_cnt++;

div_comb_done:
	ath_ant_div_conf_fast_divbias(&div_ant_conf, antcomb, alt_ratio);
	ath9k_hw_antdiv_comb_conf_set(sc->sc_ah, &div_ant_conf);

	antcomb->scan_start_time = jiffies;
	antcomb->total_pkt_count = 0;
	antcomb->main_total_rssi = 0;
	antcomb->alt_total_rssi = 0;
	antcomb->main_recv_cnt = 0;
	antcomb->alt_recv_cnt = 0;
=======
/*
 * Run the LNA combining algorithm only in these cases:
 *
 * Standalone WLAN cards with both LNA/Antenna diversity
 * enabled in the EEPROM.
 *
 * WLAN+BT cards which are in the supported card list
 * in ath_pci_id_table and the user has loaded the
 * driver with "bt_ant_diversity" set to true.
 */
static void ath9k_antenna_check(struct ath_softc *sc,
				struct ath_rx_status *rs)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath9k_hw_capabilities *pCap = &ah->caps;
	struct ath_common *common = ath9k_hw_common(ah);

	if (!(ah->caps.hw_caps & ATH9K_HW_CAP_ANT_DIV_COMB))
		return;

	/*
	 * Change the default rx antenna if rx diversity
	 * chooses the other antenna 3 times in a row.
	 */
	if (sc->rx.defant != rs->rs_antenna) {
		if (++sc->rx.rxotherant >= 3)
			ath_setdefantenna(sc, rs->rs_antenna);
	} else {
		sc->rx.rxotherant = 0;
	}

	if (pCap->hw_caps & ATH9K_HW_CAP_BT_ANT_DIV) {
		if (common->bt_ant_diversity)
			ath_ant_comb_scan(sc, rs);
	} else {
		ath_ant_comb_scan(sc, rs);
	}
}

static void ath9k_apply_ampdu_details(struct ath_softc *sc,
	struct ath_rx_status *rs, struct ieee80211_rx_status *rxs)
{
	if (rs->rs_isaggr) {
		rxs->flag |= RX_FLAG_AMPDU_DETAILS | RX_FLAG_AMPDU_LAST_KNOWN;

		rxs->ampdu_reference = sc->rx.ampdu_ref;

		if (!rs->rs_moreaggr) {
			rxs->flag |= RX_FLAG_AMPDU_IS_LAST;
			sc->rx.ampdu_ref++;
		}

		if (rs->rs_flags & ATH9K_RX_DELIM_CRC_PRE)
			rxs->flag |= RX_FLAG_AMPDU_DELIM_CRC_ERROR;
	}
>>>>>>> refs/remotes/origin/master
}

int ath_rx_tasklet(struct ath_softc *sc, int flush, bool hp)
{
<<<<<<< HEAD
	struct ath_buf *bf;
=======
	struct ath_rxbuf *bf;
>>>>>>> refs/remotes/origin/master
	struct sk_buff *skb = NULL, *requeue_skb, *hdr_skb;
	struct ieee80211_rx_status *rxs;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * The hw can technically differ from common->hw when using ath9k
	 * virtual wiphy so to account for that we iterate over the active
	 * wiphys and find the appropriate wiphy and therefore hw.
	 */
=======
>>>>>>> refs/remotes/origin/cm-10.0
	struct ieee80211_hw *hw = sc->hw;
	struct ieee80211_hdr *hdr;
=======
	struct ieee80211_hw *hw = sc->hw;
>>>>>>> refs/remotes/origin/master
	int retval;
	struct ath_rx_status rs;
	enum ath9k_rx_qtype qtype;
	bool edma = !!(ah->caps.hw_caps & ATH9K_HW_CAP_EDMA);
	int dma_type;
<<<<<<< HEAD
	u8 rx_status_len = ah->caps.rx_status_len;
	u64 tsf = 0;
	u32 tsf_lower = 0;
	unsigned long flags;
=======
	u64 tsf = 0;
	unsigned long flags;
	dma_addr_t new_buf_addr;
>>>>>>> refs/remotes/origin/master

	if (edma)
		dma_type = DMA_BIDIRECTIONAL;
	else
		dma_type = DMA_FROM_DEVICE;

	qtype = hp ? ATH9K_RX_QUEUE_HP : ATH9K_RX_QUEUE_LP;
<<<<<<< HEAD
	spin_lock_bh(&sc->rx.rxbuflock);

	tsf = ath9k_hw_gettsf64(ah);
	tsf_lower = tsf & 0xffffffff;

	do {
		bool decrypt_error = false;
		/* If handling rx interrupt and flush is in progress => exit */
		if ((sc->sc_flags & SC_OP_RXFLUSH) && (flush == 0))
			break;
=======

	tsf = ath9k_hw_gettsf64(ah);

	do {
		bool decrypt_error = false;
>>>>>>> refs/remotes/origin/master

		memset(&rs, 0, sizeof(rs));
		if (edma)
			bf = ath_edma_get_next_rx_buf(sc, &rs, qtype);
		else
			bf = ath_get_next_rx_buf(sc, &rs);

		if (!bf)
			break;

		skb = bf->bf_mpdu;
		if (!skb)
			continue;

		/*
		 * Take frame header from the first fragment and RX status from
		 * the last one.
		 */
		if (sc->rx.frag)
			hdr_skb = sc->rx.frag;
		else
			hdr_skb = skb;

<<<<<<< HEAD
		hdr = (struct ieee80211_hdr *) (hdr_skb->data + rx_status_len);
		rxs = IEEE80211_SKB_RXCB(hdr_skb);
<<<<<<< HEAD
=======
		if (ieee80211_is_beacon(hdr->frame_control) &&
		    !is_zero_ether_addr(common->curbssid) &&
		    !compare_ether_addr(hdr->addr3, common->curbssid))
			rs.is_mybeacon = true;
		else
			rs.is_mybeacon = false;
>>>>>>> refs/remotes/origin/cm-10.0

		ath_debug_stat_rx(sc, &rs);

		/*
		 * If we're asked to flush receive queue, directly
		 * chain it back at the queue without processing it.
		 */
<<<<<<< HEAD
		if (flush)
			goto requeue_drop_frag;

		retval = ath9k_rx_skb_preprocess(common, hw, hdr, &rs,
						 rxs, &decrypt_error);
		if (retval)
			goto requeue_drop_frag;
=======
		if (sc->sc_flags & SC_OP_RXFLUSH)
			goto requeue_drop_frag;

		memset(rxs, 0, sizeof(struct ieee80211_rx_status));
>>>>>>> refs/remotes/origin/cm-10.0

		rxs->mactime = (tsf & ~0xffffffffULL) | rs.rs_tstamp;
		if (rs.rs_tstamp > tsf_lower &&
		    unlikely(rs.rs_tstamp - tsf_lower > 0x10000000))
			rxs->mactime -= 0x100000000ULL;

		if (rs.rs_tstamp < tsf_lower &&
		    unlikely(tsf_lower - rs.rs_tstamp > 0x10000000))
			rxs->mactime += 0x100000000ULL;

<<<<<<< HEAD
=======
		retval = ath9k_rx_skb_preprocess(common, hw, hdr, &rs,
						 rxs, &decrypt_error);
		if (retval)
			goto requeue_drop_frag;

>>>>>>> refs/remotes/origin/cm-10.0
=======
		rxs = IEEE80211_SKB_RXCB(hdr_skb);
		memset(rxs, 0, sizeof(struct ieee80211_rx_status));

		retval = ath9k_rx_skb_preprocess(sc, hdr_skb, &rs, rxs,
						 &decrypt_error, tsf);
		if (retval)
			goto requeue_drop_frag;

>>>>>>> refs/remotes/origin/master
		/* Ensure we always have an skb to requeue once we are done
		 * processing the current buffer's skb */
		requeue_skb = ath_rxbuf_alloc(common, common->rx_bufsize, GFP_ATOMIC);

		/* If there is no memory we ignore the current RX'd frame,
		 * tell hardware it can give us a new frame using the old
		 * skb and put it at the tail of the sc->rx.rxbuf list for
		 * processing. */
<<<<<<< HEAD
		if (!requeue_skb)
			goto requeue_drop_frag;

		/* Unmap the frame */
		dma_unmap_single(sc->dev, bf->bf_buf_addr,
				 common->rx_bufsize,
				 dma_type);
=======
		if (!requeue_skb) {
			RX_STAT_INC(rx_oom_err);
			goto requeue_drop_frag;
		}

		/* We will now give hardware our shiny new allocated skb */
		new_buf_addr = dma_map_single(sc->dev, requeue_skb->data,
					      common->rx_bufsize, dma_type);
		if (unlikely(dma_mapping_error(sc->dev, new_buf_addr))) {
			dev_kfree_skb_any(requeue_skb);
			goto requeue_drop_frag;
		}

		/* Unmap the frame */
		dma_unmap_single(sc->dev, bf->bf_buf_addr,
				 common->rx_bufsize, dma_type);

		bf->bf_mpdu = requeue_skb;
		bf->bf_buf_addr = new_buf_addr;
>>>>>>> refs/remotes/origin/master

		skb_put(skb, rs.rs_datalen + ah->caps.rx_status_len);
		if (ah->caps.rx_status_len)
			skb_pull(skb, ah->caps.rx_status_len);

		if (!rs.rs_more)
			ath9k_rx_skb_postprocess(common, hdr_skb, &rs,
						 rxs, decrypt_error);

<<<<<<< HEAD
		/* We will now give hardware our shiny new allocated skb */
		bf->bf_mpdu = requeue_skb;
		bf->bf_buf_addr = dma_map_single(sc->dev, requeue_skb->data,
						 common->rx_bufsize,
						 dma_type);
		if (unlikely(dma_mapping_error(sc->dev,
			  bf->bf_buf_addr))) {
			dev_kfree_skb_any(requeue_skb);
			bf->bf_mpdu = NULL;
			bf->bf_buf_addr = 0;
			ath_err(common, "dma_mapping_error() on RX\n");
			ieee80211_rx(hw, skb);
			break;
		}

		if (rs.rs_more) {
=======
		if (rs.rs_more) {
			RX_STAT_INC(rx_frags);
>>>>>>> refs/remotes/origin/master
			/*
			 * rs_more indicates chained descriptors which can be
			 * used to link buffers together for a sort of
			 * scatter-gather operation.
			 */
			if (sc->rx.frag) {
				/* too many fragments - cannot handle frame */
				dev_kfree_skb_any(sc->rx.frag);
				dev_kfree_skb_any(skb);
<<<<<<< HEAD
=======
				RX_STAT_INC(rx_too_many_frags_err);
>>>>>>> refs/remotes/origin/master
				skb = NULL;
			}
			sc->rx.frag = skb;
			goto requeue;
		}

		if (sc->rx.frag) {
			int space = skb->len - skb_tailroom(hdr_skb);

<<<<<<< HEAD
<<<<<<< HEAD
			sc->rx.frag = NULL;

=======
>>>>>>> refs/remotes/origin/cm-10.0
			if (pskb_expand_head(hdr_skb, 0, space, GFP_ATOMIC) < 0) {
				dev_kfree_skb(skb);
				goto requeue_drop_frag;
			}

<<<<<<< HEAD
=======
			sc->rx.frag = NULL;

>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (pskb_expand_head(hdr_skb, 0, space, GFP_ATOMIC) < 0) {
				dev_kfree_skb(skb);
				RX_STAT_INC(rx_oom_err);
				goto requeue_drop_frag;
			}

			sc->rx.frag = NULL;

>>>>>>> refs/remotes/origin/master
			skb_copy_from_linear_data(skb, skb_put(hdr_skb, skb->len),
						  skb->len);
			dev_kfree_skb_any(skb);
			skb = hdr_skb;
		}

<<<<<<< HEAD
<<<<<<< HEAD
		/*
		 * change the default rx antenna if rx diversity chooses the
		 * other antenna 3 times in a row.
		 */
		if (sc->rx.defant != rs.rs_antenna) {
			if (++sc->rx.rxotherant >= 3)
				ath_setdefantenna(sc, rs.rs_antenna);
		} else {
			sc->rx.rxotherant = 0;
		}

		spin_lock_irqsave(&sc->sc_pm_lock, flags);

		if ((sc->ps_flags & (PS_WAIT_FOR_BEACON |
					      PS_WAIT_FOR_CAB |
					      PS_WAIT_FOR_PSPOLL_DATA)) ||
						ath9k_check_auto_sleep(sc))
			ath_rx_ps(sc, skb);
		spin_unlock_irqrestore(&sc->sc_pm_lock, flags);

		if (ah->caps.hw_caps & ATH9K_HW_CAP_ANT_DIV_COMB)
=======

		if (ah->caps.hw_caps & ATH9K_HW_CAP_ANT_DIV_COMB) {

			/*
			 * change the default rx antenna if rx diversity
			 * chooses the other antenna 3 times in a row.
			 */
			if (sc->rx.defant != rs.rs_antenna) {
				if (++sc->rx.rxotherant >= 3)
					ath_setdefantenna(sc, rs.rs_antenna);
			} else {
				sc->rx.rxotherant = 0;
			}

		}

=======
>>>>>>> refs/remotes/origin/master
		if (rxs->flag & RX_FLAG_MMIC_STRIPPED)
			skb_trim(skb, skb->len - 8);

		spin_lock_irqsave(&sc->sc_pm_lock, flags);
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
		if ((sc->ps_flags & (PS_WAIT_FOR_BEACON |
				     PS_WAIT_FOR_CAB |
				     PS_WAIT_FOR_PSPOLL_DATA)) ||
		    ath9k_check_auto_sleep(sc))
			ath_rx_ps(sc, skb, rs.is_mybeacon);
		spin_unlock_irqrestore(&sc->sc_pm_lock, flags);

<<<<<<< HEAD
		if ((ah->caps.hw_caps & ATH9K_HW_CAP_ANT_DIV_COMB) && sc->ant_rx == 3)
>>>>>>> refs/remotes/origin/cm-10.0
			ath_ant_comb_scan(sc, &rs);
=======
		ath9k_antenna_check(sc, &rs);

		ath9k_apply_ampdu_details(sc, &rs, rxs);
>>>>>>> refs/remotes/origin/master

		ieee80211_rx(hw, skb);

requeue_drop_frag:
		if (sc->rx.frag) {
			dev_kfree_skb_any(sc->rx.frag);
			sc->rx.frag = NULL;
		}
requeue:
<<<<<<< HEAD
<<<<<<< HEAD
		if (edma) {
			list_add_tail(&bf->list, &sc->rx.rxbuf);
			ath_rx_edma_buf_link(sc, qtype);
		} else {
			list_move_tail(&bf->list, &sc->rx.rxbuf);
			ath_rx_buf_link(sc, bf);
=======
=======
>>>>>>> refs/remotes/origin/master
		list_add_tail(&bf->list, &sc->rx.rxbuf);
		if (flush)
			continue;

		if (edma) {
			ath_rx_edma_buf_link(sc, qtype);
		} else {
			ath_rx_buf_relink(sc, bf);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			ath9k_hw_rxena(ah);
		}
	} while (1);

<<<<<<< HEAD
	spin_unlock_bh(&sc->rx.rxbuflock);

<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!(ah->imask & ATH9K_INT_RXEOL)) {
		ah->imask |= (ATH9K_INT_RXEOL | ATH9K_INT_RXORN);
		ath9k_hw_set_interrupts(ah);
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}
