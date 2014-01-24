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
=======
#include <linux/dma-mapping.h>
>>>>>>> refs/remotes/origin/master
#include "ath9k.h"

#define FUDGE 2

static void ath9k_reset_beacon_status(struct ath_softc *sc)
{
	sc->beacon.tx_processed = false;
	sc->beacon.tx_last = false;
}

/*
 *  This function will modify certain transmit queue properties depending on
 *  the operating mode of the station (AP or AdHoc).  Parameters are AIFS
 *  settings and channel width min/max
*/
<<<<<<< HEAD
int ath_beaconq_config(struct ath_softc *sc)
=======
static void ath9k_beaconq_config(struct ath_softc *sc)
>>>>>>> refs/remotes/origin/master
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath9k_tx_queue_info qi, qi_be;
	struct ath_txq *txq;

	ath9k_hw_get_txq_props(ah, sc->beacon.beaconq, &qi);
<<<<<<< HEAD
	if (sc->sc_ah->opmode == NL80211_IFTYPE_AP) {
=======

	if (sc->sc_ah->opmode == NL80211_IFTYPE_AP ||
	    sc->sc_ah->opmode == NL80211_IFTYPE_MESH_POINT) {
>>>>>>> refs/remotes/origin/master
		/* Always burst out beacon and CAB traffic. */
		qi.tqi_aifs = 1;
		qi.tqi_cwmin = 0;
		qi.tqi_cwmax = 0;
	} else {
		/* Adhoc mode; important thing is to use 2x cwmin. */
<<<<<<< HEAD
		txq = sc->tx.txq_map[WME_AC_BE];
		ath9k_hw_get_txq_props(ah, txq->axq_qnum, &qi_be);
		qi.tqi_aifs = qi_be.tqi_aifs;
		qi.tqi_cwmin = 4*qi_be.tqi_cwmin;
=======
		txq = sc->tx.txq_map[IEEE80211_AC_BE];
		ath9k_hw_get_txq_props(ah, txq->axq_qnum, &qi_be);
		qi.tqi_aifs = qi_be.tqi_aifs;
		if (ah->slottime == ATH9K_SLOT_TIME_20)
			qi.tqi_cwmin = 2*qi_be.tqi_cwmin;
		else
			qi.tqi_cwmin = 4*qi_be.tqi_cwmin;
>>>>>>> refs/remotes/origin/master
		qi.tqi_cwmax = qi_be.tqi_cwmax;
	}

	if (!ath9k_hw_set_txq_props(ah, sc->beacon.beaconq, &qi)) {
<<<<<<< HEAD
		ath_err(common,
			"Unable to update h/w beacon queue parameters\n");
		return 0;
	} else {
		ath9k_hw_resettxqueue(ah, sc->beacon.beaconq);
		return 1;
=======
		ath_err(common, "Unable to update h/w beacon queue parameters\n");
	} else {
		ath9k_hw_resettxqueue(ah, sc->beacon.beaconq);
>>>>>>> refs/remotes/origin/master
	}
}

/*
 *  Associates the beacon frame buffer with a transmit descriptor.  Will set
 *  up rate codes, and channel flags. Beacons are always sent out at the
 *  lowest rate, and are not retried.
*/
<<<<<<< HEAD
<<<<<<< HEAD
static void ath_beacon_setup(struct ath_softc *sc, struct ath_vif *avp,
=======
static void ath_beacon_setup(struct ath_softc *sc, struct ieee80211_vif *vif,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void ath9k_beacon_setup(struct ath_softc *sc, struct ieee80211_vif *vif,
>>>>>>> refs/remotes/origin/master
			     struct ath_buf *bf, int rateidx)
{
	struct sk_buff *skb = bf->bf_mpdu;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
<<<<<<< HEAD
<<<<<<< HEAD
	struct ath_desc *ds;
	struct ath9k_11n_rate_series series[4];
	int flags, ctsrate = 0, ctsduration = 0;
	struct ieee80211_supported_band *sband;
=======
	struct ath_tx_info info;
	struct ieee80211_supported_band *sband;
	u8 chainmask = ah->txchainmask;
>>>>>>> refs/remotes/origin/cm-10.0
	u8 rate = 0;

	ath9k_reset_beacon_status(sc);

<<<<<<< HEAD
	ds = bf->bf_desc;
	flags = ATH9K_TXDESC_NOACK;

	ds->ds_link = 0;

	sband = &sc->sbands[common->hw->conf.channel->band];
	rate = sband->bitrates[rateidx].hw_value;
	if (sc->sc_flags & SC_OP_PREAMBLE_SHORT)
		rate |= sband->bitrates[rateidx].hw_value_short;

	ath9k_hw_set11n_txdesc(ah, ds, skb->len + FCS_LEN,
			       ATH9K_PKT_TYPE_BEACON,
			       MAX_RATE_POWER,
			       ATH9K_TXKEYIX_INVALID,
			       ATH9K_KEY_TYPE_CLEAR,
			       flags);

	/* NB: beacon's BufLen must be a multiple of 4 bytes */
	ath9k_hw_filltxdesc(ah, ds, roundup(skb->len, 4),
			    true, true, ds, bf->bf_buf_addr,
			    sc->beacon.beaconq);

	memset(series, 0, sizeof(struct ath9k_11n_rate_series) * 4);
	series[0].Tries = 1;
	series[0].Rate = rate;
	series[0].ChSel = ath_txchainmask_reduction(sc,
			common->tx_chainmask, series[0].Rate);
	series[0].RateFlags = (ctsrate) ? ATH9K_RATESERIES_RTS_CTS : 0;
	ath9k_hw_set11n_ratescenario(ah, ds, ds, 0, ctsrate, ctsduration,
				     series, 4, 0);
=======
	sband = &sc->sbands[common->hw->conf.channel->band];
=======
	struct ath_tx_info info;
	struct ieee80211_supported_band *sband;
	u8 chainmask = ah->txchainmask;
	u8 rate = 0;

	sband = &sc->sbands[common->hw->conf.chandef.chan->band];
>>>>>>> refs/remotes/origin/master
	rate = sband->bitrates[rateidx].hw_value;
	if (vif->bss_conf.use_short_preamble)
		rate |= sband->bitrates[rateidx].hw_value_short;

	memset(&info, 0, sizeof(info));
	info.pkt_len = skb->len + FCS_LEN;
	info.type = ATH9K_PKT_TYPE_BEACON;
	info.txpower = MAX_RATE_POWER;
	info.keyix = ATH9K_TXKEYIX_INVALID;
	info.keytype = ATH9K_KEY_TYPE_CLEAR;
<<<<<<< HEAD
	info.flags = ATH9K_TXDESC_NOACK | ATH9K_TXDESC_INTREQ;
=======
	info.flags = ATH9K_TXDESC_NOACK | ATH9K_TXDESC_CLRDMASK;
>>>>>>> refs/remotes/origin/master

	info.buf_addr[0] = bf->bf_buf_addr;
	info.buf_len[0] = roundup(skb->len, 4);

	info.is_first = true;
	info.is_last = true;

	info.qcu = sc->beacon.beaconq;

	info.rates[0].Tries = 1;
	info.rates[0].Rate = rate;
	info.rates[0].ChSel = ath_txchainmask_reduction(sc, chainmask, rate);

	ath9k_hw_set_txdesc(ah, bf->bf_desc, &info);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static void ath_tx_cabq(struct ieee80211_hw *hw, struct sk_buff *skb)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_tx_control txctl;

	memset(&txctl, 0, sizeof(struct ath_tx_control));
	txctl.txq = sc->beacon.cabq;

<<<<<<< HEAD
	ath_dbg(common, ATH_DBG_XMIT,
		"transmitting CABQ packet, skb: %p\n", skb);

	if (ath_tx_start(hw, skb, &txctl) != 0) {
		ath_dbg(common, ATH_DBG_XMIT, "CABQ TX failed\n");
		dev_kfree_skb_any(skb);
=======
	ath_dbg(common, XMIT, "transmitting CABQ packet, skb: %p\n", skb);

	if (ath_tx_start(hw, skb, &txctl) != 0) {
		ath_dbg(common, XMIT, "CABQ TX failed\n");
		ieee80211_free_txskb(hw, skb);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}
}

static struct ath_buf *ath_beacon_generate(struct ieee80211_hw *hw,
					   struct ieee80211_vif *vif)
=======
}

static struct ath_buf *ath9k_beacon_generate(struct ieee80211_hw *hw,
					     struct ieee80211_vif *vif)
>>>>>>> refs/remotes/origin/master
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_buf *bf;
<<<<<<< HEAD
	struct ath_vif *avp;
	struct sk_buff *skb;
	struct ath_txq *cabq;
	struct ieee80211_tx_info *info;
	int cabq_depth;

	ath9k_reset_beacon_status(sc);

	avp = (void *)vif->drv_priv;
	cabq = sc->beacon.cabq;

	if ((avp->av_bcbuf == NULL) || !avp->is_bslot_active)
		return NULL;

	/* Release the old beacon first */

=======
	struct ath_vif *avp = (void *)vif->drv_priv;
	struct sk_buff *skb;
	struct ath_txq *cabq = sc->beacon.cabq;
	struct ieee80211_tx_info *info;
	struct ieee80211_mgmt *mgmt_hdr;
	int cabq_depth;

	if (avp->av_bcbuf == NULL)
		return NULL;

>>>>>>> refs/remotes/origin/master
	bf = avp->av_bcbuf;
	skb = bf->bf_mpdu;
	if (skb) {
		dma_unmap_single(sc->dev, bf->bf_buf_addr,
				 skb->len, DMA_TO_DEVICE);
		dev_kfree_skb_any(skb);
		bf->bf_buf_addr = 0;
		bf->bf_mpdu = NULL;
	}

<<<<<<< HEAD
	/* Get a new beacon from mac80211 */

	skb = ieee80211_beacon_get(hw, vif);
	bf->bf_mpdu = skb;
	if (skb == NULL)
		return NULL;
	((struct ieee80211_mgmt *)skb->data)->u.beacon.timestamp =
		avp->tsf_adjust;
=======
	skb = ieee80211_beacon_get(hw, vif);
	if (skb == NULL)
		return NULL;

	bf->bf_mpdu = skb;

	mgmt_hdr = (struct ieee80211_mgmt *)skb->data;
	mgmt_hdr->u.beacon.timestamp = avp->tsf_adjust;
>>>>>>> refs/remotes/origin/master

	info = IEEE80211_SKB_CB(skb);
	if (info->flags & IEEE80211_TX_CTL_ASSIGN_SEQ) {
		/*
		 * TODO: make sure the seq# gets assigned properly (vs. other
		 * TX frames)
		 */
		struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;
		sc->tx.seq_no += 0x10;
		hdr->seq_ctrl &= cpu_to_le16(IEEE80211_SCTL_FRAG);
		hdr->seq_ctrl |= cpu_to_le16(sc->tx.seq_no);
	}

	bf->bf_buf_addr = dma_map_single(sc->dev, skb->data,
					 skb->len, DMA_TO_DEVICE);
	if (unlikely(dma_mapping_error(sc->dev, bf->bf_buf_addr))) {
		dev_kfree_skb_any(skb);
		bf->bf_mpdu = NULL;
		bf->bf_buf_addr = 0;
		ath_err(common, "dma_mapping_error on beaconing\n");
		return NULL;
	}

	skb = ieee80211_get_buffered_bc(hw, vif);

	/*
	 * if the CABQ traffic from previous DTIM is pending and the current
	 *  beacon is also a DTIM.
	 *  1) if there is only one vif let the cab traffic continue.
	 *  2) if there are more than one vif and we are using staggered
	 *     beacons, then drain the cabq by dropping all the frames in
	 *     the cabq so that the current vifs cab traffic can be scheduled.
	 */
	spin_lock_bh(&cabq->axq_lock);
	cabq_depth = cabq->axq_depth;
	spin_unlock_bh(&cabq->axq_lock);

	if (skb && cabq_depth) {
		if (sc->nvifs > 1) {
<<<<<<< HEAD
<<<<<<< HEAD
			ath_dbg(common, ATH_DBG_BEACON,
=======
			ath_dbg(common, BEACON,
>>>>>>> refs/remotes/origin/cm-10.0
				"Flushing previous cabq traffic\n");
			ath_draintxq(sc, cabq, false);
		}
	}

<<<<<<< HEAD
	ath_beacon_setup(sc, avp, bf, info->control.rates[0].idx);
=======
	ath_beacon_setup(sc, vif, bf, info->control.rates[0].idx);
>>>>>>> refs/remotes/origin/cm-10.0

	while (skb) {
		ath_tx_cabq(hw, skb);
		skb = ieee80211_get_buffered_bc(hw, vif);
	}
=======
			ath_dbg(common, BEACON,
				"Flushing previous cabq traffic\n");
			ath_draintxq(sc, cabq);
		}
	}

	ath9k_beacon_setup(sc, vif, bf, info->control.rates[0].idx);

	if (skb)
		ath_tx_cabq(hw, vif, skb);
>>>>>>> refs/remotes/origin/master

	return bf;
}

<<<<<<< HEAD
int ath_beacon_alloc(struct ath_softc *sc, struct ieee80211_vif *vif)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp;
	struct ath_buf *bf;
	struct sk_buff *skb;
	struct ath_beacon_config *cur_conf = &sc->cur_beacon_conf;
	__le64 tstamp;

	avp = (void *)vif->drv_priv;

	/* Allocate a beacon descriptor if we haven't done so. */
	if (!avp->av_bcbuf) {
		/* Allocate beacon state for hostap/ibss.  We know
		 * a buffer is available. */
		avp->av_bcbuf = list_first_entry(&sc->beacon.bbuf,
						 struct ath_buf, list);
		list_del(&avp->av_bcbuf->list);

		if (ath9k_uses_beacons(vif->type)) {
			int slot;
			/*
			 * Assign the vif to a beacon xmit slot. As
			 * above, this cannot fail to find one.
			 */
			avp->av_bslot = 0;
			for (slot = 0; slot < ATH_BCBUF; slot++)
				if (sc->beacon.bslot[slot] == NULL) {
					avp->av_bslot = slot;
					avp->is_bslot_active = false;

					/* NB: keep looking for a double slot */
					if (slot == 0 || !sc->beacon.bslot[slot-1])
						break;
				}
			BUG_ON(sc->beacon.bslot[avp->av_bslot] != NULL);
			sc->beacon.bslot[avp->av_bslot] = vif;
			sc->nbcnvifs++;
		}
	}

	/* release the previous beacon frame, if it already exists. */
	bf = avp->av_bcbuf;
	if (bf->bf_mpdu != NULL) {
		skb = bf->bf_mpdu;
=======
void ath9k_beacon_assign_slot(struct ath_softc *sc, struct ieee80211_vif *vif)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp = (void *)vif->drv_priv;
	int slot;

	avp->av_bcbuf = list_first_entry(&sc->beacon.bbuf, struct ath_buf, list);
	list_del(&avp->av_bcbuf->list);

	for (slot = 0; slot < ATH_BCBUF; slot++) {
		if (sc->beacon.bslot[slot] == NULL) {
			avp->av_bslot = slot;
			break;
		}
	}

	sc->beacon.bslot[avp->av_bslot] = vif;
	sc->nbcnvifs++;

	ath_dbg(common, CONFIG, "Added interface at beacon slot: %d\n",
		avp->av_bslot);
}

void ath9k_beacon_remove_slot(struct ath_softc *sc, struct ieee80211_vif *vif)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp = (void *)vif->drv_priv;
	struct ath_buf *bf = avp->av_bcbuf;

	ath_dbg(common, CONFIG, "Removing interface at beacon slot: %d\n",
		avp->av_bslot);

	tasklet_disable(&sc->bcon_tasklet);

	if (bf && bf->bf_mpdu) {
		struct sk_buff *skb = bf->bf_mpdu;
>>>>>>> refs/remotes/origin/master
		dma_unmap_single(sc->dev, bf->bf_buf_addr,
				 skb->len, DMA_TO_DEVICE);
		dev_kfree_skb_any(skb);
		bf->bf_mpdu = NULL;
		bf->bf_buf_addr = 0;
	}

<<<<<<< HEAD
	/* NB: the beacon data buffer must be 32-bit aligned. */
	skb = ieee80211_beacon_get(sc->hw, vif);
	if (skb == NULL)
		return -ENOMEM;

	tstamp = ((struct ieee80211_mgmt *)skb->data)->u.beacon.timestamp;
	sc->beacon.bc_tstamp = (u32) le64_to_cpu(tstamp);
	/* Calculate a TSF adjustment factor required for staggered beacons. */
	if (avp->av_bslot > 0) {
		u64 tsfadjust;
		int intval;

		intval = cur_conf->beacon_interval ? : ATH_DEFAULT_BINTVAL;

		/*
		 * Calculate the TSF offset for this beacon slot, i.e., the
		 * number of usecs that need to be added to the timestamp field
		 * in Beacon and Probe Response frames. Beacon slot 0 is
		 * processed at the correct offset, so it does not require TSF
		 * adjustment. Other slots are adjusted to get the timestamp
		 * close to the TBTT for the BSS.
		 */
		tsfadjust = TU_TO_USEC(intval * avp->av_bslot) / ATH_BCBUF;
		avp->tsf_adjust = cpu_to_le64(tsfadjust);

<<<<<<< HEAD
		ath_dbg(common, ATH_DBG_BEACON,
=======
		ath_dbg(common, BEACON,
>>>>>>> refs/remotes/origin/cm-10.0
			"stagger beacons, bslot %d intval %u tsfadjust %llu\n",
			avp->av_bslot, intval, (unsigned long long)tsfadjust);

		((struct ieee80211_mgmt *)skb->data)->u.beacon.timestamp =
			avp->tsf_adjust;
	} else
		avp->tsf_adjust = cpu_to_le64(0);

	bf->bf_mpdu = skb;
	bf->bf_buf_addr = dma_map_single(sc->dev, skb->data,
					 skb->len, DMA_TO_DEVICE);
	if (unlikely(dma_mapping_error(sc->dev, bf->bf_buf_addr))) {
		dev_kfree_skb_any(skb);
		bf->bf_mpdu = NULL;
		bf->bf_buf_addr = 0;
		ath_err(common, "dma_mapping_error on beacon alloc\n");
		return -ENOMEM;
	}
	avp->is_bslot_active = true;

	return 0;
}

void ath_beacon_return(struct ath_softc *sc, struct ath_vif *avp)
{
	if (avp->av_bcbuf != NULL) {
		struct ath_buf *bf;

		avp->is_bslot_active = false;
		if (avp->av_bslot != -1) {
			sc->beacon.bslot[avp->av_bslot] = NULL;
			sc->nbcnvifs--;
			avp->av_bslot = -1;
		}

		bf = avp->av_bcbuf;
		if (bf->bf_mpdu != NULL) {
			struct sk_buff *skb = bf->bf_mpdu;
			dma_unmap_single(sc->dev, bf->bf_buf_addr,
					 skb->len, DMA_TO_DEVICE);
			dev_kfree_skb_any(skb);
			bf->bf_mpdu = NULL;
			bf->bf_buf_addr = 0;
		}
		list_add_tail(&bf->list, &sc->beacon.bbuf);

		avp->av_bcbuf = NULL;
	}
}

void ath_beacon_tasklet(unsigned long data)
{
	struct ath_softc *sc = (struct ath_softc *)data;
	struct ath_beacon_config *cur_conf = &sc->cur_beacon_conf;
=======
	avp->av_bcbuf = NULL;
	sc->beacon.bslot[avp->av_bslot] = NULL;
	sc->nbcnvifs--;
	list_add_tail(&bf->list, &sc->beacon.bbuf);

	tasklet_enable(&sc->bcon_tasklet);
}

static int ath9k_beacon_choose_slot(struct ath_softc *sc)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_beacon_config *cur_conf = &sc->cur_beacon_conf;
	u16 intval;
	u32 tsftu;
	u64 tsf;
	int slot;

	if (sc->sc_ah->opmode != NL80211_IFTYPE_AP &&
	    sc->sc_ah->opmode != NL80211_IFTYPE_MESH_POINT) {
		ath_dbg(common, BEACON, "slot 0, tsf: %llu\n",
			ath9k_hw_gettsf64(sc->sc_ah));
		return 0;
	}

	intval = cur_conf->beacon_interval ? : ATH_DEFAULT_BINTVAL;
	tsf = ath9k_hw_gettsf64(sc->sc_ah);
	tsf += TU_TO_USEC(sc->sc_ah->config.sw_beacon_response_time);
	tsftu = TSF_TO_TU((tsf * ATH_BCBUF) >>32, tsf * ATH_BCBUF);
	slot = (tsftu % (intval * ATH_BCBUF)) / intval;

	ath_dbg(common, BEACON, "slot: %d tsf: %llu tsftu: %u\n",
		slot, tsf, tsftu / ATH_BCBUF);

	return slot;
}

void ath9k_set_tsfadjust(struct ath_softc *sc, struct ieee80211_vif *vif)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_beacon_config *cur_conf = &sc->cur_beacon_conf;
	struct ath_vif *avp = (void *)vif->drv_priv;
	u64 tsfadjust;

	if (avp->av_bslot == 0)
		return;

	tsfadjust = cur_conf->beacon_interval * avp->av_bslot / ATH_BCBUF;
	avp->tsf_adjust = cpu_to_le64(TU_TO_USEC(tsfadjust));

	ath_dbg(common, CONFIG, "tsfadjust is: %llu for bslot: %d\n",
		(unsigned long long)tsfadjust, avp->av_bslot);
}

bool ath9k_csa_is_finished(struct ath_softc *sc)
{
	struct ieee80211_vif *vif;

	vif = sc->csa_vif;
	if (!vif || !vif->csa_active)
		return false;

	if (!ieee80211_csa_is_complete(vif))
		return false;

	ieee80211_csa_finish(vif);

	sc->csa_vif = NULL;
	return true;
}

void ath9k_beacon_tasklet(unsigned long data)
{
	struct ath_softc *sc = (struct ath_softc *)data;
>>>>>>> refs/remotes/origin/master
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath_buf *bf = NULL;
	struct ieee80211_vif *vif;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	bool edma = !!(ah->caps.hw_caps & ATH9K_HW_CAP_EDMA);
>>>>>>> refs/remotes/origin/cm-10.0
	int slot;
	u32 bfaddr, bc = 0;
=======
	bool edma = !!(ah->caps.hw_caps & ATH9K_HW_CAP_EDMA);
	int slot;

	if (test_bit(SC_OP_HW_RESET, &sc->sc_flags)) {
		ath_dbg(common, RESET,
			"reset work is pending, skip beaconing now\n");
		return;
	}
>>>>>>> refs/remotes/origin/master

	/*
	 * Check if the previous beacon has gone out.  If
	 * not don't try to post another, skip this period
	 * and wait for the next.  Missed beacons indicate
	 * a problem and should not occur.  If we miss too
	 * many consecutive beacons reset the device.
	 */
	if (ath9k_hw_numtxpending(ah, sc->beacon.beaconq) != 0) {
		sc->beacon.bmisscnt++;

<<<<<<< HEAD
		if (sc->beacon.bmisscnt < BSTUCK_THRESH * sc->nbcnvifs) {
<<<<<<< HEAD
			ath_dbg(common, ATH_DBG_BSTUCK,
=======
			ath_dbg(common, BSTUCK,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ath9k_hw_check_nav(ah);

		if (!ath9k_hw_check_alive(ah))
			ieee80211_queue_work(sc->hw, &sc->hw_check_work);

		if (sc->beacon.bmisscnt < BSTUCK_THRESH * sc->nbcnvifs) {
			ath_dbg(common, BSTUCK,
>>>>>>> refs/remotes/origin/master
				"missed %u consecutive beacons\n",
				sc->beacon.bmisscnt);
			ath9k_hw_stop_dma_queue(ah, sc->beacon.beaconq);
			if (sc->beacon.bmisscnt > 3)
				ath9k_hw_bstuck_nfcal(ah);
		} else if (sc->beacon.bmisscnt >= BSTUCK_THRESH) {
<<<<<<< HEAD
<<<<<<< HEAD
			ath_dbg(common, ATH_DBG_BSTUCK,
				"beacon is officially stuck\n");
			sc->sc_flags |= SC_OP_TSF_RESET;
			ath_reset(sc, true);
=======
			ath_dbg(common, BSTUCK, "beacon is officially stuck\n");
			sc->sc_flags |= SC_OP_TSF_RESET;
			ieee80211_queue_work(sc->hw, &sc->hw_reset_work);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			ath_dbg(common, BSTUCK, "beacon is officially stuck\n");
			sc->beacon.bmisscnt = 0;
			ath9k_queue_reset(sc, RESET_TYPE_BEACON_STUCK);
>>>>>>> refs/remotes/origin/master
		}

		return;
	}

<<<<<<< HEAD
	/*
	 * Generate beacon frames. we are sending frames
	 * staggered so calculate the slot for this frame based
	 * on the tsf to safeguard against missing an swba.
	 */


	if (ah->opmode == NL80211_IFTYPE_AP) {
		u16 intval;
		u32 tsftu;
		u64 tsf;

		intval = cur_conf->beacon_interval ? : ATH_DEFAULT_BINTVAL;
		tsf = ath9k_hw_gettsf64(ah);
		tsf += TU_TO_USEC(ah->config.sw_beacon_response_time);
		tsftu = TSF_TO_TU((tsf * ATH_BCBUF) >>32, tsf * ATH_BCBUF);
		slot = (tsftu % (intval * ATH_BCBUF)) / intval;
		vif = sc->beacon.bslot[slot];

<<<<<<< HEAD
		ath_dbg(common, ATH_DBG_BEACON,
=======
		ath_dbg(common, BEACON,
>>>>>>> refs/remotes/origin/cm-10.0
			"slot %d [tsf %llu tsftu %u intval %u] vif %p\n",
			slot, tsf, tsftu / ATH_BCBUF, intval, vif);
	} else {
		slot = 0;
		vif = sc->beacon.bslot[slot];
	}


	bfaddr = 0;
	if (vif) {
		bf = ath_beacon_generate(sc->hw, vif);
		if (bf != NULL) {
			bfaddr = bf->bf_daddr;
			bc = 1;
		}

		if (sc->beacon.bmisscnt != 0) {
<<<<<<< HEAD
			ath_dbg(common, ATH_DBG_BSTUCK,
=======
			ath_dbg(common, BSTUCK,
>>>>>>> refs/remotes/origin/cm-10.0
				"resume beacon xmit after %u misses\n",
				sc->beacon.bmisscnt);
			sc->beacon.bmisscnt = 0;
		}
=======
	/* EDMA devices check that in the tx completion function. */
	if (!edma && ath9k_csa_is_finished(sc))
		return;

	slot = ath9k_beacon_choose_slot(sc);
	vif = sc->beacon.bslot[slot];

	if (!vif || !vif->bss_conf.enable_beacon)
		return;

	bf = ath9k_beacon_generate(sc->hw, vif);

	if (sc->beacon.bmisscnt != 0) {
		ath_dbg(common, BSTUCK, "resume beacon xmit after %u misses\n",
			sc->beacon.bmisscnt);
		sc->beacon.bmisscnt = 0;
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Handle slot time change when a non-ERP station joins/leaves
	 * an 11g network.  The 802.11 layer notifies us via callback,
	 * we mark updateslot, then wait one beacon before effecting
	 * the change.  This gives associated stations at least one
	 * beacon interval to note the state change.
	 *
	 * NB: The slot time change state machine is clocked according
	 *     to whether we are bursting or staggering beacons.  We
	 *     recognize the request to update and record the current
	 *     slot then don't transition until that slot is reached
	 *     again.  If we miss a beacon for that slot then we'll be
	 *     slow to transition but we'll be sure at least one beacon
	 *     interval has passed.  When bursting slot is always left
	 *     set to ATH_BCBUF so this check is a noop.
	 */
	if (sc->beacon.updateslot == UPDATE) {
<<<<<<< HEAD
		sc->beacon.updateslot = COMMIT; /* commit next beacon */
		sc->beacon.slotupdate = slot;
	} else if (sc->beacon.updateslot == COMMIT && sc->beacon.slotupdate == slot) {
=======
		sc->beacon.updateslot = COMMIT;
		sc->beacon.slotupdate = slot;
	} else if (sc->beacon.updateslot == COMMIT &&
		   sc->beacon.slotupdate == slot) {
>>>>>>> refs/remotes/origin/master
		ah->slottime = sc->beacon.slottime;
		ath9k_hw_init_global_settings(ah);
		sc->beacon.updateslot = OK;
	}
<<<<<<< HEAD
	if (bfaddr != 0) {
		/* NB: cabq traffic should already be queued and primed */
		ath9k_hw_puttxbuf(ah, sc->beacon.beaconq, bfaddr);
<<<<<<< HEAD
		ath9k_hw_txstart(ah, sc->beacon.beaconq);
=======

		if (!edma)
			ath9k_hw_txstart(ah, sc->beacon.beaconq);
>>>>>>> refs/remotes/origin/cm-10.0

		sc->beacon.ast_be_xmit += bc;     /* XXX per-vif? */
	}
}

static void ath9k_beacon_init(struct ath_softc *sc,
			      u32 next_beacon,
			      u32 beacon_period)
{
	if (sc->sc_flags & SC_OP_TSF_RESET) {
		ath9k_ps_wakeup(sc);
		ath9k_hw_reset_tsf(sc->sc_ah);
	}

	ath9k_hw_beaconinit(sc->sc_ah, next_beacon, beacon_period);

	if (sc->sc_flags & SC_OP_TSF_RESET) {
		ath9k_ps_restore(sc);
		sc->sc_flags &= ~SC_OP_TSF_RESET;
	}
=======

	if (bf) {
		ath9k_reset_beacon_status(sc);

		ath_dbg(common, BEACON,
			"Transmitting beacon for slot: %d\n", slot);

		/* NB: cabq traffic should already be queued and primed */
		ath9k_hw_puttxbuf(ah, sc->beacon.beaconq, bf->bf_daddr);

		if (!edma)
			ath9k_hw_txstart(ah, sc->beacon.beaconq);
	}
}

/*
 * Both nexttbtt and intval have to be in usecs.
 */
static void ath9k_beacon_init(struct ath_softc *sc, u32 nexttbtt,
			      u32 intval, bool reset_tsf)
{
	struct ath_hw *ah = sc->sc_ah;

	ath9k_hw_disable_interrupts(ah);
	if (reset_tsf)
		ath9k_hw_reset_tsf(ah);
	ath9k_beaconq_config(sc);
	ath9k_hw_beaconinit(ah, nexttbtt, intval);
	sc->beacon.bmisscnt = 0;
	ath9k_hw_set_interrupts(ah);
	ath9k_hw_enable_interrupts(ah);
>>>>>>> refs/remotes/origin/master
}

/*
 * For multi-bss ap support beacons are either staggered evenly over N slots or
 * burst together.  For the former arrange for the SWBA to be delivered for each
 * slot. Slots that are not occupied will generate nothing.
 */
<<<<<<< HEAD
static void ath_beacon_config_ap(struct ath_softc *sc,
				 struct ath_beacon_config *conf)
{
	struct ath_hw *ah = sc->sc_ah;
	u32 nexttbtt, intval;

	/* NB: the beacon interval is kept internally in TU's */
<<<<<<< HEAD
	intval = TU_TO_USEC(conf->beacon_interval & ATH9K_BEACON_PERIOD);
=======
	intval = TU_TO_USEC(conf->beacon_interval);
>>>>>>> refs/remotes/origin/cm-10.0
	intval /= ATH_BCBUF;    /* for staggered beacons */
	nexttbtt = intval;

	/*
	 * In AP mode we enable the beacon timers and SWBA interrupts to
	 * prepare beacon frames.
	 */
	ah->imask |= ATH9K_INT_SWBA;
	ath_beaconq_config(sc);

	/* Set the computed AP beacon timers */

	ath9k_hw_disable_interrupts(ah);
<<<<<<< HEAD
	ath9k_beacon_init(sc, nexttbtt, intval);
	sc->beacon.bmisscnt = 0;
	ath9k_hw_set_interrupts(ah, ah->imask);
=======
	sc->sc_flags |= SC_OP_TSF_RESET;
	ath9k_beacon_init(sc, nexttbtt, intval);
	sc->beacon.bmisscnt = 0;
	ath9k_hw_set_interrupts(ah);
	ath9k_hw_enable_interrupts(ah);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void ath9k_beacon_config_ap(struct ath_softc *sc,
				   struct ath_beacon_config *conf)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	u32 nexttbtt, intval;

	/* NB: the beacon interval is kept internally in TU's */
	intval = TU_TO_USEC(conf->beacon_interval);
	intval /= ATH_BCBUF;
	nexttbtt = intval;

	if (conf->enable_beacon)
		ah->imask |= ATH9K_INT_SWBA;
	else
		ah->imask &= ~ATH9K_INT_SWBA;

	ath_dbg(common, BEACON,
		"AP (%s) nexttbtt: %u intval: %u conf_intval: %u\n",
		(conf->enable_beacon) ? "Enable" : "Disable",
		nexttbtt, intval, conf->beacon_interval);

	ath9k_beacon_init(sc, nexttbtt, intval, true);
>>>>>>> refs/remotes/origin/master
}

/*
 * This sets up the beacon timers according to the timestamp of the last
 * received beacon and the current TSF, configures PCF and DTIM
 * handling, programs the sleep registers so the hardware will wakeup in
 * time to receive beacons, and configures the beacon miss handling so
 * we'll receive a BMISS interrupt when we stop seeing beacons from the AP
 * we've associated with.
 */
<<<<<<< HEAD
static void ath_beacon_config_sta(struct ath_softc *sc,
				  struct ath_beacon_config *conf)
=======
static void ath9k_beacon_config_sta(struct ath_softc *sc,
				    struct ath_beacon_config *conf)
>>>>>>> refs/remotes/origin/master
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath9k_beacon_state bs;
	int dtimperiod, dtimcount, sleepduration;
	int cfpperiod, cfpcount;
	u32 nexttbtt = 0, intval, tsftu;
	u64 tsf;
	int num_beacons, offset, dtim_dec_count, cfp_dec_count;

	/* No need to configure beacon if we are not associated */
<<<<<<< HEAD
	if (!common->curaid) {
<<<<<<< HEAD
		ath_dbg(common, ATH_DBG_BEACON,
=======
		ath_dbg(common, BEACON,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!test_bit(SC_OP_PRIM_STA_VIF, &sc->sc_flags)) {
		ath_dbg(common, BEACON,
>>>>>>> refs/remotes/origin/master
			"STA is not yet associated..skipping beacon config\n");
		return;
	}

	memset(&bs, 0, sizeof(bs));
<<<<<<< HEAD
<<<<<<< HEAD
	intval = conf->beacon_interval & ATH9K_BEACON_PERIOD;
=======
	intval = conf->beacon_interval;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	intval = conf->beacon_interval;
>>>>>>> refs/remotes/origin/master

	/*
	 * Setup dtim and cfp parameters according to
	 * last beacon we received (which may be none).
	 */
	dtimperiod = conf->dtim_period;
	dtimcount = conf->dtim_count;
	if (dtimcount >= dtimperiod)	/* NB: sanity check */
		dtimcount = 0;
	cfpperiod = 1;			/* NB: no PCF support yet */
	cfpcount = 0;

	sleepduration = conf->listen_interval * intval;

	/*
	 * Pull nexttbtt forward to reflect the current
	 * TSF and calculate dtim+cfp state for the result.
	 */
	tsf = ath9k_hw_gettsf64(ah);
	tsftu = TSF_TO_TU(tsf>>32, tsf) + FUDGE;

	num_beacons = tsftu / intval + 1;
	offset = tsftu % intval;
	nexttbtt = tsftu - offset;
	if (offset)
		nexttbtt += intval;

	/* DTIM Beacon every dtimperiod Beacon */
	dtim_dec_count = num_beacons % dtimperiod;
	/* CFP every cfpperiod DTIM Beacon */
	cfp_dec_count = (num_beacons / dtimperiod) % cfpperiod;
	if (dtim_dec_count)
		cfp_dec_count++;

	dtimcount -= dtim_dec_count;
	if (dtimcount < 0)
		dtimcount += dtimperiod;

	cfpcount -= cfp_dec_count;
	if (cfpcount < 0)
		cfpcount += cfpperiod;

	bs.bs_intval = intval;
	bs.bs_nexttbtt = nexttbtt;
	bs.bs_dtimperiod = dtimperiod*intval;
	bs.bs_nextdtim = bs.bs_nexttbtt + dtimcount*intval;
	bs.bs_cfpperiod = cfpperiod*bs.bs_dtimperiod;
	bs.bs_cfpnext = bs.bs_nextdtim + cfpcount*bs.bs_dtimperiod;
	bs.bs_cfpmaxduration = 0;

	/*
	 * Calculate the number of consecutive beacons to miss* before taking
	 * a BMISS interrupt. The configuration is specified in TU so we only
	 * need calculate based	on the beacon interval.  Note that we clamp the
	 * result to at most 15 beacons.
	 */
	if (sleepduration > intval) {
		bs.bs_bmissthreshold = conf->listen_interval *
			ATH_DEFAULT_BMISS_LIMIT / 2;
	} else {
		bs.bs_bmissthreshold = DIV_ROUND_UP(conf->bmiss_timeout, intval);
		if (bs.bs_bmissthreshold > 15)
			bs.bs_bmissthreshold = 15;
		else if (bs.bs_bmissthreshold <= 0)
			bs.bs_bmissthreshold = 1;
	}

	/*
	 * Calculate sleep duration. The configuration is given in ms.
	 * We ensure a multiple of the beacon period is used. Also, if the sleep
	 * duration is greater than the DTIM period then it makes senses
	 * to make it a multiple of that.
	 *
	 * XXX fixed at 100ms
	 */

	bs.bs_sleepduration = roundup(IEEE80211_MS_TO_TU(100), sleepduration);
	if (bs.bs_sleepduration > bs.bs_dtimperiod)
		bs.bs_sleepduration = bs.bs_dtimperiod;

	/* TSF out of range threshold fixed at 1 second */
	bs.bs_tsfoor_threshold = ATH9K_TSFOOR_THRESHOLD;

<<<<<<< HEAD
<<<<<<< HEAD
	ath_dbg(common, ATH_DBG_BEACON, "tsf: %llu tsftu: %u\n", tsf, tsftu);
	ath_dbg(common, ATH_DBG_BEACON,
=======
	ath_dbg(common, BEACON, "tsf: %llu tsftu: %u\n", tsf, tsftu);
	ath_dbg(common, BEACON,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ath_dbg(common, BEACON, "tsf: %llu tsftu: %u\n", tsf, tsftu);
	ath_dbg(common, BEACON,
>>>>>>> refs/remotes/origin/master
		"bmiss: %u sleep: %u cfp-period: %u maxdur: %u next: %u\n",
		bs.bs_bmissthreshold, bs.bs_sleepduration,
		bs.bs_cfpperiod, bs.bs_cfpmaxduration, bs.bs_cfpnext);

	/* Set the computed STA beacon timers */

	ath9k_hw_disable_interrupts(ah);
	ath9k_hw_set_sta_beacon_timers(ah, &bs);
	ah->imask |= ATH9K_INT_BMISS;

<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * If the beacon config is called beacause of TSFOOR,
	 * Interrupts will be enabled back at the end of ath9k_tasklet
	 */
	if (!(sc->ps_flags & PS_TSFOOR_SYNC))
		ath9k_hw_set_interrupts(ah, ah->imask);
=======
	ath9k_hw_set_interrupts(ah);
	ath9k_hw_enable_interrupts(ah);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void ath_beacon_config_adhoc(struct ath_softc *sc,
				    struct ath_beacon_config *conf)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
<<<<<<< HEAD
	u32 tsf, delta, intval, nexttbtt;

	ath9k_reset_beacon_status(sc);

	tsf = ath9k_hw_gettsf32(ah) + TU_TO_USEC(FUDGE);
	intval = TU_TO_USEC(conf->beacon_interval & ATH9K_BEACON_PERIOD);

	if (!sc->beacon.bc_tstamp)
		nexttbtt = tsf + intval;
	else {
		if (tsf > sc->beacon.bc_tstamp)
			delta = (tsf - sc->beacon.bc_tstamp);
		else
			delta = (tsf + 1 + (~0U - sc->beacon.bc_tstamp));
		nexttbtt = tsf + intval - (delta % intval);
	}

	ath_dbg(common, ATH_DBG_BEACON,
		"IBSS nexttbtt %u intval %u (%u)\n",
=======
	u32 tsf, intval, nexttbtt;

	ath9k_reset_beacon_status(sc);

	intval = TU_TO_USEC(conf->beacon_interval);
	tsf = roundup(ath9k_hw_gettsf32(ah) + TU_TO_USEC(FUDGE), intval);
	nexttbtt = tsf + intval;

	ath_dbg(common, BEACON, "IBSS nexttbtt %u intval %u (%u)\n",
>>>>>>> refs/remotes/origin/cm-10.0
		nexttbtt, intval, conf->beacon_interval);

	/*
	 * In IBSS mode enable the beacon timers but only enable SWBA interrupts
	 * if we need to manually prepare beacon frames.  Otherwise we use a
	 * self-linked tx descriptor and let the hardware deal with things.
	 */
	ah->imask |= ATH9K_INT_SWBA;

	ath_beaconq_config(sc);

	/* Set the computed ADHOC beacon timers */

	ath9k_hw_disable_interrupts(ah);
	ath9k_beacon_init(sc, nexttbtt, intval);
	sc->beacon.bmisscnt = 0;
<<<<<<< HEAD
	/*
	 * If the beacon config is called beacause of TSFOOR,
	 * Interrupts will be enabled back at the end of ath9k_tasklet
	 */
	if (!(sc->ps_flags & PS_TSFOOR_SYNC))
		ath9k_hw_set_interrupts(ah, ah->imask);
=======

	ath9k_hw_set_interrupts(ah);
	ath9k_hw_enable_interrupts(ah);
>>>>>>> refs/remotes/origin/cm-10.0
}

static bool ath9k_allow_beacon_config(struct ath_softc *sc,
				      struct ieee80211_vif *vif)
{
	struct ath_beacon_config *cur_conf = &sc->cur_beacon_conf;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ieee80211_bss_conf *bss_conf = &vif->bss_conf;
	struct ath_vif *avp = (void *)vif->drv_priv;

	/*
	 * Can not have different beacon interval on multiple
	 * AP interface case
	 */
	if ((sc->sc_ah->opmode == NL80211_IFTYPE_AP) &&
	    (sc->nbcnvifs > 1) &&
	    (vif->type == NL80211_IFTYPE_AP) &&
	    (cur_conf->beacon_interval != bss_conf->beacon_int)) {
<<<<<<< HEAD
		ath_dbg(common, ATH_DBG_CONFIG,
			"Changing beacon interval of multiple \
			AP interfaces !\n");
=======
		ath_dbg(common, CONFIG,
			"Changing beacon interval of multiple AP interfaces !\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return false;
	}
	/*
	 * Can not configure station vif's beacon config
	 * while on AP opmode
	 */
	if ((sc->sc_ah->opmode == NL80211_IFTYPE_AP) &&
	    (vif->type != NL80211_IFTYPE_AP)) {
<<<<<<< HEAD
		ath_dbg(common, ATH_DBG_CONFIG,
=======
		ath_dbg(common, CONFIG,
>>>>>>> refs/remotes/origin/cm-10.0
			"STA vif's beacon not allowed on AP mode\n");
		return false;
	}
	/*
	 * Do not allow beacon config if HW was already configured
	 * with another STA vif
	 */
	if ((sc->sc_ah->opmode == NL80211_IFTYPE_STATION) &&
	    (vif->type == NL80211_IFTYPE_STATION) &&
	    (sc->sc_flags & SC_OP_BEACONS) &&
	    !avp->primary_sta_vif) {
<<<<<<< HEAD
		ath_dbg(common, ATH_DBG_CONFIG,
=======
		ath_dbg(common, CONFIG,
>>>>>>> refs/remotes/origin/cm-10.0
			"Beacon already configured for a station interface\n");
		return false;
	}
	return true;
}

void ath_beacon_config(struct ath_softc *sc, struct ieee80211_vif *vif)
{
	struct ath_beacon_config *cur_conf = &sc->cur_beacon_conf;
	struct ieee80211_bss_conf *bss_conf = &vif->bss_conf;

	if (!ath9k_allow_beacon_config(sc, vif))
		return;

	/* Setup the beacon configuration parameters */
=======
	ath9k_hw_set_interrupts(ah);
	ath9k_hw_enable_interrupts(ah);
}

static void ath9k_beacon_config_adhoc(struct ath_softc *sc,
				      struct ath_beacon_config *conf)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	u32 intval, nexttbtt;

	ath9k_reset_beacon_status(sc);

	intval = TU_TO_USEC(conf->beacon_interval);

	if (conf->ibss_creator) {
		nexttbtt = intval;
	} else {
		u32 tbtt, offset, tsftu;
		u64 tsf;

		/*
		 * Pull nexttbtt forward to reflect the current
		 * sync'd TSF.
		 */
		tsf = ath9k_hw_gettsf64(ah);
		tsftu = TSF_TO_TU(tsf >> 32, tsf) + FUDGE;
		offset = tsftu % conf->beacon_interval;
		tbtt = tsftu - offset;
		if (offset)
			tbtt += conf->beacon_interval;

		nexttbtt = TU_TO_USEC(tbtt);
	}

	if (conf->enable_beacon)
		ah->imask |= ATH9K_INT_SWBA;
	else
		ah->imask &= ~ATH9K_INT_SWBA;

	ath_dbg(common, BEACON,
		"IBSS (%s) nexttbtt: %u intval: %u conf_intval: %u\n",
		(conf->enable_beacon) ? "Enable" : "Disable",
		nexttbtt, intval, conf->beacon_interval);

	ath9k_beacon_init(sc, nexttbtt, intval, conf->ibss_creator);

	/*
	 * Set the global 'beacon has been configured' flag for the
	 * joiner case in IBSS mode.
	 */
	if (!conf->ibss_creator && conf->enable_beacon)
		set_bit(SC_OP_BEACONS, &sc->sc_flags);
}

bool ath9k_allow_beacon_config(struct ath_softc *sc, struct ieee80211_vif *vif)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp = (void *)vif->drv_priv;

	if (sc->sc_ah->opmode == NL80211_IFTYPE_AP) {
		if ((vif->type != NL80211_IFTYPE_AP) ||
		    (sc->nbcnvifs > 1)) {
			ath_dbg(common, CONFIG,
				"An AP interface is already present !\n");
			return false;
		}
	}

	if (sc->sc_ah->opmode == NL80211_IFTYPE_STATION) {
		if ((vif->type == NL80211_IFTYPE_STATION) &&
		    test_bit(SC_OP_BEACONS, &sc->sc_flags) &&
		    !avp->primary_sta_vif) {
			ath_dbg(common, CONFIG,
				"Beacon already configured for a station interface\n");
			return false;
		}
	}

	return true;
}

static void ath9k_cache_beacon_config(struct ath_softc *sc,
				      struct ieee80211_bss_conf *bss_conf)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_beacon_config *cur_conf = &sc->cur_beacon_conf;

	ath_dbg(common, BEACON,
		"Caching beacon data for BSS: %pM\n", bss_conf->bssid);

>>>>>>> refs/remotes/origin/master
	cur_conf->beacon_interval = bss_conf->beacon_int;
	cur_conf->dtim_period = bss_conf->dtim_period;
	cur_conf->listen_interval = 1;
	cur_conf->dtim_count = 1;
<<<<<<< HEAD
=======
	cur_conf->ibss_creator = bss_conf->ibss_creator;
>>>>>>> refs/remotes/origin/master
	cur_conf->bmiss_timeout =
		ATH_DEFAULT_BMISS_LIMIT * cur_conf->beacon_interval;

	/*
	 * It looks like mac80211 may end up using beacon interval of zero in
	 * some cases (at least for mesh point). Avoid getting into an
	 * infinite loop by using a bit safer value instead. To be safe,
	 * do sanity check on beacon interval for all operating modes.
	 */
	if (cur_conf->beacon_interval == 0)
		cur_conf->beacon_interval = 100;

	/*
	 * We don't parse dtim period from mac80211 during the driver
	 * initialization as it breaks association with hidden-ssid
	 * AP and it causes latency in roaming
	 */
	if (cur_conf->dtim_period == 0)
		cur_conf->dtim_period = 1;

<<<<<<< HEAD
	ath_set_beacon(sc);
}

static bool ath_has_valid_bslot(struct ath_softc *sc)
{
	struct ath_vif *avp;
	int slot;
	bool found = false;

	for (slot = 0; slot < ATH_BCBUF; slot++) {
		if (sc->beacon.bslot[slot]) {
			avp = (void *)sc->beacon.bslot[slot]->drv_priv;
			if (avp->is_bslot_active) {
				found = true;
				break;
			}
		}
	}
	return found;
}


void ath_set_beacon(struct ath_softc *sc)
=======
}

void ath9k_beacon_config(struct ath_softc *sc, struct ieee80211_vif *vif,
			 u32 changed)
{
	struct ieee80211_bss_conf *bss_conf = &vif->bss_conf;
	struct ath_beacon_config *cur_conf = &sc->cur_beacon_conf;
	unsigned long flags;
	bool skip_beacon = false;

	if (sc->sc_ah->opmode == NL80211_IFTYPE_STATION) {
		ath9k_cache_beacon_config(sc, bss_conf);
		ath9k_set_beacon(sc);
		set_bit(SC_OP_BEACONS, &sc->sc_flags);
		return;

	}

	/*
	 * Take care of multiple interfaces when
	 * enabling/disabling SWBA.
	 */
	if (changed & BSS_CHANGED_BEACON_ENABLED) {
		if (!bss_conf->enable_beacon &&
		    (sc->nbcnvifs <= 1)) {
			cur_conf->enable_beacon = false;
		} else if (bss_conf->enable_beacon) {
			cur_conf->enable_beacon = true;
			ath9k_cache_beacon_config(sc, bss_conf);
		}
	}

	/*
	 * Configure the HW beacon registers only when we have a valid
	 * beacon interval.
	 */
	if (cur_conf->beacon_interval) {
		/*
		 * If we are joining an existing IBSS network, start beaconing
		 * only after a TSF-sync has taken place. Ensure that this
		 * happens by setting the appropriate flags.
		 */
		if ((changed & BSS_CHANGED_IBSS) && !bss_conf->ibss_creator &&
		    bss_conf->enable_beacon) {
			spin_lock_irqsave(&sc->sc_pm_lock, flags);
			sc->ps_flags |= PS_BEACON_SYNC | PS_WAIT_FOR_BEACON;
			spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
			skip_beacon = true;
		} else {
			ath9k_set_beacon(sc);
		}

		/*
		 * Do not set the SC_OP_BEACONS flag for IBSS joiner mode
		 * here, it is done in ath9k_beacon_config_adhoc().
		 */
		if (cur_conf->enable_beacon && !skip_beacon)
			set_bit(SC_OP_BEACONS, &sc->sc_flags);
		else
			clear_bit(SC_OP_BEACONS, &sc->sc_flags);
	}
}

void ath9k_set_beacon(struct ath_softc *sc)
>>>>>>> refs/remotes/origin/master
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_beacon_config *cur_conf = &sc->cur_beacon_conf;

	switch (sc->sc_ah->opmode) {
	case NL80211_IFTYPE_AP:
<<<<<<< HEAD
		if (ath_has_valid_bslot(sc))
			ath_beacon_config_ap(sc, cur_conf);
		break;
	case NL80211_IFTYPE_ADHOC:
	case NL80211_IFTYPE_MESH_POINT:
		ath_beacon_config_adhoc(sc, cur_conf);
		break;
	case NL80211_IFTYPE_STATION:
		ath_beacon_config_sta(sc, cur_conf);
		break;
	default:
<<<<<<< HEAD
		ath_dbg(common, ATH_DBG_CONFIG,
			"Unsupported beaconing mode\n");
=======
		ath_dbg(common, CONFIG, "Unsupported beaconing mode\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return;
	}

	sc->sc_flags |= SC_OP_BEACONS;
}

void ath9k_set_beaconing_status(struct ath_softc *sc, bool status)
{
	struct ath_hw *ah = sc->sc_ah;

	if (!ath_has_valid_bslot(sc))
		return;

	ath9k_ps_wakeup(sc);
	if (status) {
		/* Re-enable beaconing */
		ah->imask |= ATH9K_INT_SWBA;
<<<<<<< HEAD
		ath9k_hw_set_interrupts(ah, ah->imask);
	} else {
		/* Disable SWBA interrupt */
		ah->imask &= ~ATH9K_INT_SWBA;
		ath9k_hw_set_interrupts(ah, ah->imask);
=======
		ath9k_hw_set_interrupts(ah);
	} else {
		/* Disable SWBA interrupt */
		ah->imask &= ~ATH9K_INT_SWBA;
		ath9k_hw_set_interrupts(ah);
>>>>>>> refs/remotes/origin/cm-10.0
		tasklet_kill(&sc->bcon_tasklet);
		ath9k_hw_stop_dma_queue(ah, sc->beacon.beaconq);
	}
	ath9k_ps_restore(sc);
=======
	case NL80211_IFTYPE_MESH_POINT:
		ath9k_beacon_config_ap(sc, cur_conf);
		break;
	case NL80211_IFTYPE_ADHOC:
		ath9k_beacon_config_adhoc(sc, cur_conf);
		break;
	case NL80211_IFTYPE_STATION:
		ath9k_beacon_config_sta(sc, cur_conf);
		break;
	default:
		ath_dbg(common, CONFIG, "Unsupported beaconing mode\n");
		return;
	}
>>>>>>> refs/remotes/origin/master
}
