/*
 * Copyright 2002-2005, Instant802 Networks, Inc.
 * Copyright 2005-2006, Devicescape Software, Inc.
 * Copyright 2006-2007	Jiri Benc <jbenc@suse.cz>
 * Copyright 2008-2010	Johannes Berg <johannes@sipsolutions.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <net/mac80211.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/export.h>
#include <linux/etherdevice.h>
#include <net/mac80211.h>
#include <asm/unaligned.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include "ieee80211_i.h"
#include "rate.h"
#include "mesh.h"
#include "led.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "wme.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "wme.h"
>>>>>>> refs/remotes/origin/master


void ieee80211_tx_status_irqsafe(struct ieee80211_hw *hw,
				 struct sk_buff *skb)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	int tmp;

	skb->pkt_type = IEEE80211_TX_STATUS_MSG;
	skb_queue_tail(info->flags & IEEE80211_TX_CTL_REQ_TX_STATUS ?
		       &local->skb_queue : &local->skb_queue_unreliable, skb);
	tmp = skb_queue_len(&local->skb_queue) +
		skb_queue_len(&local->skb_queue_unreliable);
	while (tmp > IEEE80211_IRQSAFE_QUEUE_LIMIT &&
	       (skb = skb_dequeue(&local->skb_queue_unreliable))) {
<<<<<<< HEAD
		dev_kfree_skb_irq(skb);
=======
		ieee80211_free_txskb(hw, skb);
>>>>>>> refs/remotes/origin/master
		tmp--;
		I802_DEBUG_INC(local->tx_status_drop);
	}
	tasklet_schedule(&local->tasklet);
}
EXPORT_SYMBOL(ieee80211_tx_status_irqsafe);

static void ieee80211_handle_filtered_frame(struct ieee80211_local *local,
					    struct sta_info *sta,
					    struct sk_buff *skb)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct ieee80211_hdr *hdr = (void *)skb->data;
	int ac;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ieee80211_hdr *hdr = (void *)skb->data;
	int ac;
>>>>>>> refs/remotes/origin/master

	/*
	 * This skb 'survived' a round-trip through the driver, and
	 * hopefully the driver didn't mangle it too badly. However,
	 * we can definitely not rely on the control information
	 * being correct. Clear it so we don't get junk there, and
	 * indicate that it needs new processing, but must not be
	 * modified/encrypted again.
	 */
	memset(&info->control, 0, sizeof(info->control));

	info->control.jiffies = jiffies;
	info->control.vif = &sta->sdata->vif;
	info->flags |= IEEE80211_TX_INTFL_NEED_TXPROCESSING |
		       IEEE80211_TX_INTFL_RETRANSMISSION;
	info->flags &= ~IEEE80211_TX_TEMPORARY_FLAGS;

	sta->tx_filtered_count++;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	 * Clear more-data bit on filtered frames, it might be set
	 * but later frames might time out so it might have to be
	 * clear again ... It's all rather unlikely (this frame
	 * should time out first, right?) but let's not confuse
	 * peers unnecessarily.
	 */
	if (hdr->frame_control & cpu_to_le16(IEEE80211_FCTL_MOREDATA))
		hdr->frame_control &= ~cpu_to_le16(IEEE80211_FCTL_MOREDATA);

	if (ieee80211_is_data_qos(hdr->frame_control)) {
		u8 *p = ieee80211_get_qos_ctl(hdr);
		int tid = *p & IEEE80211_QOS_CTL_TID_MASK;

		/*
		 * Clear EOSP if set, this could happen e.g.
		 * if an absence period (us being a P2P GO)
		 * shortens the SP.
		 */
		if (*p & IEEE80211_QOS_CTL_EOSP)
			*p &= ~IEEE80211_QOS_CTL_EOSP;
		ac = ieee802_1d_to_ac[tid & 7];
	} else {
		ac = IEEE80211_AC_BE;
	}

	/*
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	 * Clear the TX filter mask for this STA when sending the next
	 * packet. If the STA went to power save mode, this will happen
	 * when it wakes up for the next time.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	set_sta_flags(sta, WLAN_STA_CLEAR_PS_FILT);
=======
	set_sta_flag(sta, WLAN_STA_CLEAR_PS_FILT);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	set_sta_flag(sta, WLAN_STA_CLEAR_PS_FILT);
>>>>>>> refs/remotes/origin/master

	/*
	 * This code races in the following way:
	 *
	 *  (1) STA sends frame indicating it will go to sleep and does so
	 *  (2) hardware/firmware adds STA to filter list, passes frame up
	 *  (3) hardware/firmware processes TX fifo and suppresses a frame
	 *  (4) we get TX status before having processed the frame and
	 *	knowing that the STA has gone to sleep.
	 *
	 * This is actually quite unlikely even when both those events are
	 * processed from interrupts coming in quickly after one another or
	 * even at the same time because we queue both TX status events and
	 * RX frames to be processed by a tasklet and process them in the
	 * same order that they were received or TX status last. Hence, there
	 * is no race as long as the frame RX is processed before the next TX
	 * status, which drivers can ensure, see below.
	 *
	 * Note that this can only happen if the hardware or firmware can
	 * actually add STAs to the filter list, if this is done by the
	 * driver in response to set_tim() (which will only reduce the race
	 * this whole filtering tries to solve, not completely solve it)
	 * this situation cannot happen.
	 *
	 * To completely solve this race drivers need to make sure that they
	 *  (a) don't mix the irq-safe/not irq-safe TX status/RX processing
	 *	functions and
	 *  (b) always process RX events before TX status events if ordering
	 *      can be unknown, for example with different interrupt status
	 *	bits.
	 *  (c) if PS mode transitions are manual (i.e. the flag
	 *      %IEEE80211_HW_AP_LINK_PS is set), always process PS state
	 *      changes before calling TX status events if ordering can be
	 *	unknown.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (test_sta_flags(sta, WLAN_STA_PS_STA) &&
	    skb_queue_len(&sta->tx_filtered) < STA_MAX_TX_BUFFER) {
		skb_queue_tail(&sta->tx_filtered, skb);
		return;
	}

	if (!test_sta_flags(sta, WLAN_STA_PS_STA) &&
=======
=======
>>>>>>> refs/remotes/origin/master
	if (test_sta_flag(sta, WLAN_STA_PS_STA) &&
	    skb_queue_len(&sta->tx_filtered[ac]) < STA_MAX_TX_BUFFER) {
		skb_queue_tail(&sta->tx_filtered[ac], skb);
		sta_info_recalc_tim(sta);

		if (!timer_pending(&local->sta_cleanup))
			mod_timer(&local->sta_cleanup,
				  round_jiffies(jiffies +
						STA_INFO_CLEANUP_INTERVAL));
		return;
	}

	if (!test_sta_flag(sta, WLAN_STA_PS_STA) &&
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	    !(info->flags & IEEE80211_TX_INTFL_RETRIED)) {
		/* Software retry the packet once */
		info->flags |= IEEE80211_TX_INTFL_RETRIED;
		ieee80211_add_pending_skb(local, skb);
		return;
	}

<<<<<<< HEAD
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
	if (net_ratelimit())
		wiphy_debug(local->hw.wiphy,
			    "dropped TX filtered frame, queue_len=%d PS=%d @%lu\n",
<<<<<<< HEAD
			    skb_queue_len(&sta->tx_filtered),
			    !!test_sta_flags(sta, WLAN_STA_PS_STA), jiffies);
=======
			    skb_queue_len(&sta->tx_filtered[ac]),
			    !!test_sta_flag(sta, WLAN_STA_PS_STA), jiffies);
>>>>>>> refs/remotes/origin/cm-10.0
#endif
	dev_kfree_skb(skb);
}

<<<<<<< HEAD
=======
=======
	ps_dbg_ratelimited(sta->sdata,
			   "dropped TX filtered frame, queue_len=%d PS=%d @%lu\n",
			   skb_queue_len(&sta->tx_filtered[ac]),
			   !!test_sta_flag(sta, WLAN_STA_PS_STA), jiffies);
	ieee80211_free_txskb(&local->hw, skb);
}

>>>>>>> refs/remotes/origin/master
static void ieee80211_check_pending_bar(struct sta_info *sta, u8 *addr, u8 tid)
{
	struct tid_ampdu_tx *tid_tx;

	tid_tx = rcu_dereference(sta->ampdu_mlme.tid_tx[tid]);
	if (!tid_tx || !tid_tx->bar_pending)
		return;

	tid_tx->bar_pending = false;
	ieee80211_send_bar(&sta->sdata->vif, addr, tid, tid_tx->failed_bar_ssn);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void ieee80211_frame_acked(struct sta_info *sta, struct sk_buff *skb)
{
	struct ieee80211_mgmt *mgmt = (void *) skb->data;
	struct ieee80211_local *local = sta->local;
	struct ieee80211_sub_if_data *sdata = sta->sdata;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	if (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS)
		sta->last_rx = jiffies;

	if (ieee80211_is_data_qos(mgmt->frame_control)) {
		struct ieee80211_hdr *hdr = (void *) skb->data;
		u8 *qc = ieee80211_get_qos_ctl(hdr);
		u16 tid = qc[0] & 0xf;

		ieee80211_check_pending_bar(sta, hdr->addr1, tid);
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (ieee80211_is_action(mgmt->frame_control) &&
	    sdata->vif.type == NL80211_IFTYPE_STATION &&
	    mgmt->u.action.category == WLAN_CATEGORY_HT &&
	    mgmt->u.action.u.ht_smps.action == WLAN_HT_ACTION_SMPS) {
		/*
		 * This update looks racy, but isn't -- if we come
		 * here we've definitely got a station that we're
		 * talking to, and on a managed interface that can
		 * only be the AP. And the only other place updating
		 * this variable is before we're associated.
		 */
		switch (mgmt->u.action.u.ht_smps.smps_control) {
		case WLAN_HT_SMPS_CONTROL_DYNAMIC:
			sta->sdata->u.mgd.ap_smps = IEEE80211_SMPS_DYNAMIC;
			break;
		case WLAN_HT_SMPS_CONTROL_STATIC:
			sta->sdata->u.mgd.ap_smps = IEEE80211_SMPS_STATIC;
			break;
		case WLAN_HT_SMPS_CONTROL_DISABLED:
		default: /* shouldn't happen since we don't send that */
			sta->sdata->u.mgd.ap_smps = IEEE80211_SMPS_OFF;
			break;
		}

		ieee80211_queue_work(&local->hw, &local->recalc_smps);
	}
}

<<<<<<< HEAD
=======
=======
	if (ieee80211_is_action(mgmt->frame_control) &&
	    mgmt->u.action.category == WLAN_CATEGORY_HT &&
	    mgmt->u.action.u.ht_smps.action == WLAN_HT_ACTION_SMPS &&
	    ieee80211_sdata_running(sdata)) {
		enum ieee80211_smps_mode smps_mode;

		switch (mgmt->u.action.u.ht_smps.smps_control) {
		case WLAN_HT_SMPS_CONTROL_DYNAMIC:
			smps_mode = IEEE80211_SMPS_DYNAMIC;
			break;
		case WLAN_HT_SMPS_CONTROL_STATIC:
			smps_mode = IEEE80211_SMPS_STATIC;
			break;
		case WLAN_HT_SMPS_CONTROL_DISABLED:
		default: /* shouldn't happen since we don't send that */
			smps_mode = IEEE80211_SMPS_OFF;
			break;
		}

		if (sdata->vif.type == NL80211_IFTYPE_STATION) {
			/*
			 * This update looks racy, but isn't -- if we come
			 * here we've definitely got a station that we're
			 * talking to, and on a managed interface that can
			 * only be the AP. And the only other place updating
			 * this variable in managed mode is before association.
			 */
			sdata->smps_mode = smps_mode;
			ieee80211_queue_work(&local->hw, &sdata->recalc_smps);
		} else if (sdata->vif.type == NL80211_IFTYPE_AP ||
			   sdata->vif.type == NL80211_IFTYPE_AP_VLAN) {
			sta->known_smps_mode = smps_mode;
		}
	}
}

>>>>>>> refs/remotes/origin/master
static void ieee80211_set_bar_pending(struct sta_info *sta, u8 tid, u16 ssn)
{
	struct tid_ampdu_tx *tid_tx;

	tid_tx = rcu_dereference(sta->ampdu_mlme.tid_tx[tid]);
	if (!tid_tx)
		return;

	tid_tx->failed_bar_ssn = ssn;
	tid_tx->bar_pending = true;
}

static int ieee80211_tx_radiotap_len(struct ieee80211_tx_info *info)
{
	int len = sizeof(struct ieee80211_radiotap_header);

	/* IEEE80211_RADIOTAP_RATE rate */
	if (info->status.rates[0].idx >= 0 &&
<<<<<<< HEAD
	    !(info->status.rates[0].flags & IEEE80211_TX_RC_MCS))
=======
	    !(info->status.rates[0].flags & (IEEE80211_TX_RC_MCS |
					     IEEE80211_TX_RC_VHT_MCS)))
>>>>>>> refs/remotes/origin/master
		len += 2;

	/* IEEE80211_RADIOTAP_TX_FLAGS */
	len += 2;

	/* IEEE80211_RADIOTAP_DATA_RETRIES */
	len += 1;

<<<<<<< HEAD
	/* IEEE80211_TX_RC_MCS */
	if (info->status.rates[0].idx >= 0 &&
	    info->status.rates[0].flags & IEEE80211_TX_RC_MCS)
		len += 3;
=======
	/* IEEE80211_RADIOTAP_MCS
	 * IEEE80211_RADIOTAP_VHT */
	if (info->status.rates[0].idx >= 0) {
		if (info->status.rates[0].flags & IEEE80211_TX_RC_MCS)
			len += 3;
		else if (info->status.rates[0].flags & IEEE80211_TX_RC_VHT_MCS)
			len = ALIGN(len, 2) + 12;
	}
>>>>>>> refs/remotes/origin/master

	return len;
}

<<<<<<< HEAD
static void ieee80211_add_tx_radiotap_header(struct ieee80211_supported_band
					     *sband, struct sk_buff *skb,
					     int retry_count, int rtap_len)
=======
static void
ieee80211_add_tx_radiotap_header(struct ieee80211_local *local,
				 struct ieee80211_supported_band *sband,
				 struct sk_buff *skb, int retry_count,
				 int rtap_len, int shift)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *) skb->data;
	struct ieee80211_radiotap_header *rthdr;
	unsigned char *pos;
	u16 txflags;

	rthdr = (struct ieee80211_radiotap_header *) skb_push(skb, rtap_len);

	memset(rthdr, 0, rtap_len);
	rthdr->it_len = cpu_to_le16(rtap_len);
	rthdr->it_present =
		cpu_to_le32((1 << IEEE80211_RADIOTAP_TX_FLAGS) |
			    (1 << IEEE80211_RADIOTAP_DATA_RETRIES));
	pos = (unsigned char *)(rthdr + 1);

	/*
	 * XXX: Once radiotap gets the bitmap reset thing the vendor
	 *	extensions proposal contains, we can actually report
	 *	the whole set of tries we did.
	 */

	/* IEEE80211_RADIOTAP_RATE */
	if (info->status.rates[0].idx >= 0 &&
<<<<<<< HEAD
	    !(info->status.rates[0].flags & IEEE80211_TX_RC_MCS)) {
		rthdr->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_RATE);
		*pos = sband->bitrates[info->status.rates[0].idx].bitrate / 5;
=======
	    !(info->status.rates[0].flags & (IEEE80211_TX_RC_MCS |
					     IEEE80211_TX_RC_VHT_MCS))) {
		u16 rate;

		rthdr->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_RATE);
		rate = sband->bitrates[info->status.rates[0].idx].bitrate;
		*pos = DIV_ROUND_UP(rate, 5 * (1 << shift));
>>>>>>> refs/remotes/origin/master
		/* padding for tx flags */
		pos += 2;
	}

	/* IEEE80211_RADIOTAP_TX_FLAGS */
	txflags = 0;
	if (!(info->flags & IEEE80211_TX_STAT_ACK) &&
	    !is_multicast_ether_addr(hdr->addr1))
		txflags |= IEEE80211_RADIOTAP_F_TX_FAIL;

	if ((info->status.rates[0].flags & IEEE80211_TX_RC_USE_RTS_CTS) ||
	    (info->status.rates[0].flags & IEEE80211_TX_RC_USE_CTS_PROTECT))
		txflags |= IEEE80211_RADIOTAP_F_TX_CTS;
	else if (info->status.rates[0].flags & IEEE80211_TX_RC_USE_RTS_CTS)
		txflags |= IEEE80211_RADIOTAP_F_TX_RTS;

	put_unaligned_le16(txflags, pos);
	pos += 2;

	/* IEEE80211_RADIOTAP_DATA_RETRIES */
	/* for now report the total retry_count */
	*pos = retry_count;
	pos++;

<<<<<<< HEAD
	/* IEEE80211_TX_RC_MCS */
	if (info->status.rates[0].idx >= 0 &&
	    info->status.rates[0].flags & IEEE80211_TX_RC_MCS) {
=======
	if (info->status.rates[0].idx < 0)
		return;

	/* IEEE80211_RADIOTAP_MCS
	 * IEEE80211_RADIOTAP_VHT */
	if (info->status.rates[0].flags & IEEE80211_TX_RC_MCS) {
>>>>>>> refs/remotes/origin/master
		rthdr->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_MCS);
		pos[0] = IEEE80211_RADIOTAP_MCS_HAVE_MCS |
			 IEEE80211_RADIOTAP_MCS_HAVE_GI |
			 IEEE80211_RADIOTAP_MCS_HAVE_BW;
		if (info->status.rates[0].flags & IEEE80211_TX_RC_SHORT_GI)
			pos[1] |= IEEE80211_RADIOTAP_MCS_SGI;
		if (info->status.rates[0].flags & IEEE80211_TX_RC_40_MHZ_WIDTH)
			pos[1] |= IEEE80211_RADIOTAP_MCS_BW_40;
		if (info->status.rates[0].flags & IEEE80211_TX_RC_GREEN_FIELD)
			pos[1] |= IEEE80211_RADIOTAP_MCS_FMT_GF;
		pos[2] = info->status.rates[0].idx;
		pos += 3;
<<<<<<< HEAD
	}

}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	} else if (info->status.rates[0].flags & IEEE80211_TX_RC_VHT_MCS) {
		u16 known = local->hw.radiotap_vht_details &
			(IEEE80211_RADIOTAP_VHT_KNOWN_GI |
			 IEEE80211_RADIOTAP_VHT_KNOWN_BANDWIDTH);

		rthdr->it_present |= cpu_to_le32(1 << IEEE80211_RADIOTAP_VHT);

		/* required alignment from rthdr */
		pos = (u8 *)rthdr + ALIGN(pos - (u8 *)rthdr, 2);

		/* u16 known - IEEE80211_RADIOTAP_VHT_KNOWN_* */
		put_unaligned_le16(known, pos);
		pos += 2;

		/* u8 flags - IEEE80211_RADIOTAP_VHT_FLAG_* */
		if (info->status.rates[0].flags & IEEE80211_TX_RC_SHORT_GI)
			*pos |= IEEE80211_RADIOTAP_VHT_FLAG_SGI;
		pos++;

		/* u8 bandwidth */
		if (info->status.rates[0].flags & IEEE80211_TX_RC_40_MHZ_WIDTH)
			*pos = 1;
		else if (info->status.rates[0].flags & IEEE80211_TX_RC_80_MHZ_WIDTH)
			*pos = 4;
		else if (info->status.rates[0].flags & IEEE80211_TX_RC_160_MHZ_WIDTH)
			*pos = 11;
		else /* IEEE80211_TX_RC_{20_MHZ_WIDTH,FIXME:DUP_DATA} */
			*pos = 0;
		pos++;

		/* u8 mcs_nss[4] */
		*pos = (ieee80211_rate_get_vht_mcs(&info->status.rates[0]) << 4) |
			ieee80211_rate_get_vht_nss(&info->status.rates[0]);
		pos += 4;

		/* u8 coding */
		pos++;
		/* u8 group_id */
		pos++;
		/* u16 partial_aid */
		pos += 2;
	}
}

static void ieee80211_report_used_skb(struct ieee80211_local *local,
				      struct sk_buff *skb, bool dropped)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct ieee80211_hdr *hdr = (void *)skb->data;
	bool acked = info->flags & IEEE80211_TX_STAT_ACK;

	if (dropped)
		acked = false;

	if (info->flags & (IEEE80211_TX_INTFL_NL80211_FRAME_TX |
			   IEEE80211_TX_INTFL_MLME_CONN_TX)) {
		struct ieee80211_sub_if_data *sdata = NULL;
		struct ieee80211_sub_if_data *iter_sdata;
		u64 cookie = (unsigned long)skb;

		rcu_read_lock();

		if (skb->dev) {
			list_for_each_entry_rcu(iter_sdata, &local->interfaces,
						list) {
				if (!iter_sdata->dev)
					continue;

				if (skb->dev == iter_sdata->dev) {
					sdata = iter_sdata;
					break;
				}
			}
		} else {
			sdata = rcu_dereference(local->p2p_sdata);
		}

		if (!sdata) {
			skb->dev = NULL;
		} else if (info->flags & IEEE80211_TX_INTFL_MLME_CONN_TX) {
			ieee80211_mgd_conn_tx_status(sdata, hdr->frame_control,
						     acked);
		} else if (ieee80211_is_nullfunc(hdr->frame_control) ||
			   ieee80211_is_qos_nullfunc(hdr->frame_control)) {
			cfg80211_probe_status(sdata->dev, hdr->addr1,
					      cookie, acked, GFP_ATOMIC);
		} else {
			cfg80211_mgmt_tx_status(&sdata->wdev, cookie, skb->data,
						skb->len, acked, GFP_ATOMIC);
		}

		rcu_read_unlock();
	}

	if (unlikely(info->ack_frame_id)) {
		struct sk_buff *ack_skb;
		unsigned long flags;

		spin_lock_irqsave(&local->ack_status_lock, flags);
		ack_skb = idr_find(&local->ack_status_frames,
				   info->ack_frame_id);
		if (ack_skb)
			idr_remove(&local->ack_status_frames,
				   info->ack_frame_id);
		spin_unlock_irqrestore(&local->ack_status_lock, flags);

		if (ack_skb) {
			if (!dropped) {
				/* consumes ack_skb */
				skb_complete_wifi_ack(ack_skb, acked);
			} else {
				dev_kfree_skb_any(ack_skb);
			}
		}
	}
}

>>>>>>> refs/remotes/origin/master
/*
 * Use a static threshold for now, best value to be determined
 * by testing ...
 * Should it depend on:
 *  - on # of retransmissions
 *  - current throughput (higher value for higher tpt)?
 */
#define STA_LOST_PKT_THRESHOLD	50

void ieee80211_tx_status(struct ieee80211_hw *hw, struct sk_buff *skb)
{
	struct sk_buff *skb2;
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *) skb->data;
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
<<<<<<< HEAD
<<<<<<< HEAD
	u16 frag, type;
	__le16 fc;
	struct ieee80211_supported_band *sband;
	struct ieee80211_tx_status_rtap_hdr *rthdr;
=======
	__le16 fc;
	struct ieee80211_supported_band *sband;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	__le16 fc;
	struct ieee80211_supported_band *sband;
>>>>>>> refs/remotes/origin/master
	struct ieee80211_sub_if_data *sdata;
	struct net_device *prev_dev = NULL;
	struct sta_info *sta, *tmp;
	int retry_count = -1, i;
	int rates_idx = -1;
	bool send_to_cooked;
	bool acked;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct ieee80211_bar *bar;
	int rtap_len;
>>>>>>> refs/remotes/origin/cm-10.0

	for (i = 0; i < IEEE80211_TX_MAX_RATES; i++) {
		if (info->status.rates[i].idx < 0) {
=======
	struct ieee80211_bar *bar;
	int rtap_len;
	int shift = 0;

	for (i = 0; i < IEEE80211_TX_MAX_RATES; i++) {
		if ((info->flags & IEEE80211_TX_CTL_AMPDU) &&
		    !(info->flags & IEEE80211_TX_STAT_AMPDU)) {
			/* just the first aggr frame carry status info */
			info->status.rates[i].idx = -1;
			info->status.rates[i].count = 0;
			break;
		} else if (info->status.rates[i].idx < 0) {
>>>>>>> refs/remotes/origin/master
			break;
		} else if (i >= hw->max_report_rates) {
			/* the HW cannot have attempted that rate */
			info->status.rates[i].idx = -1;
			info->status.rates[i].count = 0;
			break;
		}

		retry_count += info->status.rates[i].count;
	}
	rates_idx = i - 1;

	if (retry_count < 0)
		retry_count = 0;

	rcu_read_lock();

	sband = local->hw.wiphy->bands[info->band];
	fc = hdr->frame_control;

	for_each_sta_info(local, hdr->addr1, sta, tmp) {
		/* skip wrong virtual interface */
<<<<<<< HEAD
<<<<<<< HEAD
		if (memcmp(hdr->addr2, sta->sdata->vif.addr, ETH_ALEN))
			continue;

		acked = !!(info->flags & IEEE80211_TX_STAT_ACK);
		if (!acked && test_sta_flags(sta, WLAN_STA_PS_STA)) {
=======
		if (compare_ether_addr(hdr->addr2, sta->sdata->vif.addr))
			continue;
=======
		if (!ether_addr_equal(hdr->addr2, sta->sdata->vif.addr))
			continue;

		shift = ieee80211_vif_get_shift(&sta->sdata->vif);
>>>>>>> refs/remotes/origin/master

		if (info->flags & IEEE80211_TX_STATUS_EOSP)
			clear_sta_flag(sta, WLAN_STA_SP);

		acked = !!(info->flags & IEEE80211_TX_STAT_ACK);
		if (!acked && test_sta_flag(sta, WLAN_STA_PS_STA)) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			/*
			 * The STA is in power save mode, so assume
			 * that this TX packet failed because of that.
			 */
			ieee80211_handle_filtered_frame(local, sta, skb);
			rcu_read_unlock();
			return;
		}

<<<<<<< HEAD
=======
		/* mesh Peer Service Period support */
		if (ieee80211_vif_is_mesh(&sta->sdata->vif) &&
		    ieee80211_is_data_qos(fc))
			ieee80211_mpsp_trigger_process(
					ieee80211_get_qos_ctl(hdr),
					sta, true, acked);

>>>>>>> refs/remotes/origin/master
		if ((local->hw.flags & IEEE80211_HW_HAS_RATE_CONTROL) &&
		    (rates_idx != -1))
			sta->last_tx_rate = info->status.rates[rates_idx];

		if ((info->flags & IEEE80211_TX_STAT_AMPDU_NO_BACK) &&
		    (ieee80211_is_data_qos(fc))) {
			u16 tid, ssn;
			u8 *qc;

			qc = ieee80211_get_qos_ctl(hdr);
			tid = qc[0] & 0xf;
			ssn = ((le16_to_cpu(hdr->seq_ctrl) + 0x10)
						& IEEE80211_SCTL_SEQ);
<<<<<<< HEAD
<<<<<<< HEAD
			ieee80211_send_bar(sta->sdata, hdr->addr1,
					   tid, ssn);
		}

=======
=======
>>>>>>> refs/remotes/origin/master
			ieee80211_send_bar(&sta->sdata->vif, hdr->addr1,
					   tid, ssn);
		}

		if (!acked && ieee80211_is_back_req(fc)) {
			u16 tid, control;

			/*
			 * BAR failed, store the last SSN and retry sending
			 * the BAR when the next unicast transmission on the
			 * same TID succeeds.
			 */
			bar = (struct ieee80211_bar *) skb->data;
			control = le16_to_cpu(bar->control);
			if (!(control & IEEE80211_BAR_CTRL_MULTI_TID)) {
				u16 ssn = le16_to_cpu(bar->start_seq_num);

				tid = (control &
				       IEEE80211_BAR_CTRL_TID_INFO_MASK) >>
				      IEEE80211_BAR_CTRL_TID_INFO_SHIFT;

				ieee80211_set_bar_pending(sta, tid, ssn);
			}
		}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (info->flags & IEEE80211_TX_STAT_TX_FILTERED) {
			ieee80211_handle_filtered_frame(local, sta, skb);
			rcu_read_unlock();
			return;
		} else {
			if (!acked)
				sta->tx_retry_failed++;
			sta->tx_retry_count += retry_count;
		}

		rate_control_tx_status(local, sband, sta, skb);
		if (ieee80211_vif_is_mesh(&sta->sdata->vif))
			ieee80211s_update_metric(local, sta, skb);

		if (!(info->flags & IEEE80211_TX_CTL_INJECTED) && acked)
			ieee80211_frame_acked(sta, skb);

		if ((sta->sdata->vif.type == NL80211_IFTYPE_STATION) &&
		    (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS))
			ieee80211_sta_tx_notify(sta->sdata, (void *) skb->data, acked);

		if (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS) {
			if (info->flags & IEEE80211_TX_STAT_ACK) {
				if (sta->lost_packets)
					sta->lost_packets = 0;
			} else if (++sta->lost_packets >= STA_LOST_PKT_THRESHOLD) {
				cfg80211_cqm_pktloss_notify(sta->sdata->dev,
							    sta->sta.addr,
							    sta->lost_packets,
							    GFP_ATOMIC);
				sta->lost_packets = 0;
			}
		}
<<<<<<< HEAD
=======

		if (acked)
			sta->last_ack_signal = info->status.ack_signal;
>>>>>>> refs/remotes/origin/master
	}

	rcu_read_unlock();

<<<<<<< HEAD
	ieee80211_led_tx(local, 0);
=======
	ieee80211_led_tx(local);
>>>>>>> refs/remotes/origin/master

	/* SNMP counters
	 * Fragments are passed to low-level drivers as separate skbs, so these
	 * are actually fragments, not frames. Update frame counters only for
	 * the first fragment of the frame. */
<<<<<<< HEAD
<<<<<<< HEAD

	frag = le16_to_cpu(hdr->seq_ctrl) & IEEE80211_SCTL_FRAG;
	type = le16_to_cpu(hdr->frame_control) & IEEE80211_FCTL_FTYPE;

	if (info->flags & IEEE80211_TX_STAT_ACK) {
		if (frag == 0) {
=======
	if (info->flags & IEEE80211_TX_STAT_ACK) {
		if (ieee80211_is_first_frag(hdr->seq_ctrl)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (info->flags & IEEE80211_TX_STAT_ACK) {
		if (ieee80211_is_first_frag(hdr->seq_ctrl)) {
>>>>>>> refs/remotes/origin/master
			local->dot11TransmittedFrameCount++;
			if (is_multicast_ether_addr(hdr->addr1))
				local->dot11MulticastTransmittedFrameCount++;
			if (retry_count > 0)
				local->dot11RetryCount++;
			if (retry_count > 1)
				local->dot11MultipleRetryCount++;
		}

		/* This counter shall be incremented for an acknowledged MPDU
		 * with an individual address in the address 1 field or an MPDU
		 * with a multicast address in the address 1 field of type Data
		 * or Management. */
		if (!is_multicast_ether_addr(hdr->addr1) ||
<<<<<<< HEAD
<<<<<<< HEAD
		    type == IEEE80211_FTYPE_DATA ||
		    type == IEEE80211_FTYPE_MGMT)
			local->dot11TransmittedFragmentCount++;
	} else {
		if (frag == 0)
=======
=======
>>>>>>> refs/remotes/origin/master
		    ieee80211_is_data(fc) ||
		    ieee80211_is_mgmt(fc))
			local->dot11TransmittedFragmentCount++;
	} else {
		if (ieee80211_is_first_frag(hdr->seq_ctrl))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			local->dot11FailedCount++;
	}

	if (ieee80211_is_nullfunc(fc) && ieee80211_has_pm(fc) &&
	    (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS) &&
	    !(info->flags & IEEE80211_TX_CTL_INJECTED) &&
	    local->ps_sdata && !(local->scanning)) {
		if (info->flags & IEEE80211_TX_STAT_ACK) {
			local->ps_sdata->u.mgd.flags |=
					IEEE80211_STA_NULLFUNC_ACKED;
		} else
			mod_timer(&local->dynamic_ps_timer, jiffies +
					msecs_to_jiffies(10));
	}

<<<<<<< HEAD
	if (info->flags & IEEE80211_TX_INTFL_NL80211_FRAME_TX) {
<<<<<<< HEAD
		struct ieee80211_work *wk;
		u64 cookie = (unsigned long)skb;

		rcu_read_lock();
		list_for_each_entry_rcu(wk, &local->work_list, list) {
			if (wk->type != IEEE80211_WORK_OFFCHANNEL_TX)
				continue;
			if (wk->offchan_tx.frame != skb)
				continue;
			wk->offchan_tx.status = true;
			break;
		}
		rcu_read_unlock();
		if (local->hw_roc_skb_for_status == skb) {
			cookie = local->hw_roc_cookie ^ 2;
			local->hw_roc_skb_for_status = NULL;
		}

		if (cookie == local->hw_offchan_tx_cookie)
			local->hw_offchan_tx_cookie = 0;

		cfg80211_mgmt_tx_status(
			skb->dev, cookie, skb->data, skb->len,
			!!(info->flags & IEEE80211_TX_STAT_ACK), GFP_ATOMIC);
=======
		u64 cookie = (unsigned long)skb;

		if (ieee80211_is_nullfunc(hdr->frame_control) ||
		    ieee80211_is_qos_nullfunc(hdr->frame_control)) {
			acked = info->flags & IEEE80211_TX_STAT_ACK;

			cfg80211_probe_status(skb->dev, hdr->addr1,
					      cookie, acked, GFP_ATOMIC);
		} else {
			struct ieee80211_work *wk;

			rcu_read_lock();
			list_for_each_entry_rcu(wk, &local->work_list, list) {
				if (wk->type != IEEE80211_WORK_OFFCHANNEL_TX)
					continue;
				if (wk->offchan_tx.frame != skb)
					continue;
				wk->offchan_tx.status = true;
				break;
			}
			rcu_read_unlock();
			if (local->hw_roc_skb_for_status == skb) {
				cookie = local->hw_roc_cookie ^ 2;
				local->hw_roc_skb_for_status = NULL;
			}

			cfg80211_mgmt_tx_status(
				skb->dev, cookie, skb->data, skb->len,
				!!(info->flags & IEEE80211_TX_STAT_ACK),
				GFP_ATOMIC);
		}
	}

	if (unlikely(info->ack_frame_id)) {
		struct sk_buff *ack_skb;
		unsigned long flags;

		spin_lock_irqsave(&local->ack_status_lock, flags);
		ack_skb = idr_find(&local->ack_status_frames,
				   info->ack_frame_id);
		if (ack_skb)
			idr_remove(&local->ack_status_frames,
				   info->ack_frame_id);
		spin_unlock_irqrestore(&local->ack_status_lock, flags);

		/* consumes ack_skb */
		if (ack_skb)
			skb_complete_wifi_ack(ack_skb,
				info->flags & IEEE80211_TX_STAT_ACK);
>>>>>>> refs/remotes/origin/cm-10.0
	}
=======
	ieee80211_report_used_skb(local, skb, false);
>>>>>>> refs/remotes/origin/master

	/* this was a transmitted frame, but now we want to reuse it */
	skb_orphan(skb);

	/* Need to make a copy before skb->cb gets cleared */
	send_to_cooked = !!(info->flags & IEEE80211_TX_CTL_INJECTED) ||
<<<<<<< HEAD
<<<<<<< HEAD
			(type != IEEE80211_FTYPE_DATA);
=======
			 !(ieee80211_is_data(fc));
>>>>>>> refs/remotes/origin/cm-10.0
=======
			 !(ieee80211_is_data(fc));
>>>>>>> refs/remotes/origin/master

	/*
	 * This is a bit racy but we can avoid a lot of work
	 * with this test...
	 */
	if (!local->monitors && (!send_to_cooked || !local->cooked_mntrs)) {
		dev_kfree_skb(skb);
		return;
	}

	/* send frame to monitor interfaces now */
<<<<<<< HEAD
<<<<<<< HEAD

	if (skb_headroom(skb) < sizeof(*rthdr)) {
=======
	rtap_len = ieee80211_tx_radiotap_len(info);
	if (WARN_ON_ONCE(skb_headroom(skb) < rtap_len)) {
>>>>>>> refs/remotes/origin/cm-10.0
		printk(KERN_ERR "ieee80211_tx_status: headroom too small\n");
		dev_kfree_skb(skb);
		return;
	}
<<<<<<< HEAD

	rthdr = (struct ieee80211_tx_status_rtap_hdr *)
				skb_push(skb, sizeof(*rthdr));

	memset(rthdr, 0, sizeof(*rthdr));
	rthdr->hdr.it_len = cpu_to_le16(sizeof(*rthdr));
	rthdr->hdr.it_present =
		cpu_to_le32((1 << IEEE80211_RADIOTAP_TX_FLAGS) |
			    (1 << IEEE80211_RADIOTAP_DATA_RETRIES) |
			    (1 << IEEE80211_RADIOTAP_RATE));

	if (!(info->flags & IEEE80211_TX_STAT_ACK) &&
	    !is_multicast_ether_addr(hdr->addr1))
		rthdr->tx_flags |= cpu_to_le16(IEEE80211_RADIOTAP_F_TX_FAIL);

	/*
	 * XXX: Once radiotap gets the bitmap reset thing the vendor
	 *	extensions proposal contains, we can actually report
	 *	the whole set of tries we did.
	 */
	if ((info->status.rates[0].flags & IEEE80211_TX_RC_USE_RTS_CTS) ||
	    (info->status.rates[0].flags & IEEE80211_TX_RC_USE_CTS_PROTECT))
		rthdr->tx_flags |= cpu_to_le16(IEEE80211_RADIOTAP_F_TX_CTS);
	else if (info->status.rates[0].flags & IEEE80211_TX_RC_USE_RTS_CTS)
		rthdr->tx_flags |= cpu_to_le16(IEEE80211_RADIOTAP_F_TX_RTS);
	if (info->status.rates[0].idx >= 0 &&
	    !(info->status.rates[0].flags & IEEE80211_TX_RC_MCS))
		rthdr->rate = sband->bitrates[
				info->status.rates[0].idx].bitrate / 5;

	/* for now report the total retry_count */
	rthdr->data_retries = retry_count;
=======
	ieee80211_add_tx_radiotap_header(sband, skb, retry_count, rtap_len);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rtap_len = ieee80211_tx_radiotap_len(info);
	if (WARN_ON_ONCE(skb_headroom(skb) < rtap_len)) {
		pr_err("ieee80211_tx_status: headroom too small\n");
		dev_kfree_skb(skb);
		return;
	}
	ieee80211_add_tx_radiotap_header(local, sband, skb, retry_count,
					 rtap_len, shift);
>>>>>>> refs/remotes/origin/master

	/* XXX: is this sufficient for BPF? */
	skb_set_mac_header(skb, 0);
	skb->ip_summed = CHECKSUM_UNNECESSARY;
	skb->pkt_type = PACKET_OTHERHOST;
	skb->protocol = htons(ETH_P_802_2);
	memset(skb->cb, 0, sizeof(skb->cb));

	rcu_read_lock();
	list_for_each_entry_rcu(sdata, &local->interfaces, list) {
		if (sdata->vif.type == NL80211_IFTYPE_MONITOR) {
			if (!ieee80211_sdata_running(sdata))
				continue;

			if ((sdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES) &&
			    !send_to_cooked)
				continue;

			if (prev_dev) {
				skb2 = skb_clone(skb, GFP_ATOMIC);
				if (skb2) {
					skb2->dev = prev_dev;
					netif_rx(skb2);
				}
			}

			prev_dev = sdata->dev;
		}
	}
	if (prev_dev) {
		skb->dev = prev_dev;
		netif_rx(skb);
		skb = NULL;
	}
	rcu_read_unlock();
	dev_kfree_skb(skb);
}
EXPORT_SYMBOL(ieee80211_tx_status);

void ieee80211_report_low_ack(struct ieee80211_sta *pubsta, u32 num_packets)
{
	struct sta_info *sta = container_of(pubsta, struct sta_info, sta);
	cfg80211_cqm_pktloss_notify(sta->sdata->dev, sta->sta.addr,
				    num_packets, GFP_ATOMIC);
}
EXPORT_SYMBOL(ieee80211_report_low_ack);
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

void ieee80211_free_txskb(struct ieee80211_hw *hw, struct sk_buff *skb)
{
	struct ieee80211_local *local = hw_to_local(hw);
<<<<<<< HEAD
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);

	if (unlikely(info->ack_frame_id)) {
		struct sk_buff *ack_skb;
		unsigned long flags;

		spin_lock_irqsave(&local->ack_status_lock, flags);
		ack_skb = idr_find(&local->ack_status_frames,
				   info->ack_frame_id);
		if (ack_skb)
			idr_remove(&local->ack_status_frames,
				   info->ack_frame_id);
		spin_unlock_irqrestore(&local->ack_status_lock, flags);

		/* consumes ack_skb */
		if (ack_skb)
			dev_kfree_skb_any(ack_skb);
	}

=======

	ieee80211_report_used_skb(local, skb, true);
>>>>>>> refs/remotes/origin/master
	dev_kfree_skb_any(skb);
}
EXPORT_SYMBOL(ieee80211_free_txskb);

void ieee80211_purge_tx_queue(struct ieee80211_hw *hw,
			      struct sk_buff_head *skbs)
{
	struct sk_buff *skb;

	while ((skb = __skb_dequeue(skbs)))
		ieee80211_free_txskb(hw, skb);
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
