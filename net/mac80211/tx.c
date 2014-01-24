/*
 * Copyright 2002-2005, Instant802 Networks, Inc.
 * Copyright 2005-2006, Devicescape Software, Inc.
 * Copyright 2006-2007	Jiri Benc <jbenc@suse.cz>
 * Copyright 2007	Johannes Berg <johannes@sipsolutions.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Transmit and frame generation functions.
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/etherdevice.h>
#include <linux/bitmap.h>
#include <linux/rcupdate.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <net/net_namespace.h>
#include <net/ieee80211_radiotap.h>
#include <net/cfg80211.h>
#include <net/mac80211.h>
#include <asm/unaligned.h>

#include "ieee80211_i.h"
#include "driver-ops.h"
#include "led.h"
#include "mesh.h"
#include "wep.h"
#include "wpa.h"
#include "wme.h"
#include "rate.h"

/* misc utils */

<<<<<<< HEAD
<<<<<<< HEAD
static __le16 ieee80211_duration(struct ieee80211_tx_data *tx, int group_addr,
=======
static __le16 ieee80211_duration(struct ieee80211_tx_data *tx,
				 struct sk_buff *skb, int group_addr,
>>>>>>> refs/remotes/origin/cm-10.0
				 int next_frag_len)
{
	int rate, mrate, erp, dur, i;
=======
static __le16 ieee80211_duration(struct ieee80211_tx_data *tx,
				 struct sk_buff *skb, int group_addr,
				 int next_frag_len)
{
	int rate, mrate, erp, dur, i, shift = 0;
>>>>>>> refs/remotes/origin/master
	struct ieee80211_rate *txrate;
	struct ieee80211_local *local = tx->local;
	struct ieee80211_supported_band *sband;
	struct ieee80211_hdr *hdr;
<<<<<<< HEAD
<<<<<<< HEAD
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx->skb);
=======
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
>>>>>>> refs/remotes/origin/cm-10.0

	/* assume HW handles this */
	if (info->control.rates[0].flags & IEEE80211_TX_RC_MCS)
		return 0;

	/* uh huh? */
	if (WARN_ON_ONCE(info->control.rates[0].idx < 0))
		return 0;

	sband = local->hw.wiphy->bands[tx->channel->band];
	txrate = &sband->bitrates[info->control.rates[0].idx];
=======
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct ieee80211_chanctx_conf *chanctx_conf;
	u32 rate_flags = 0;

	rcu_read_lock();
	chanctx_conf = rcu_dereference(tx->sdata->vif.chanctx_conf);
	if (chanctx_conf) {
		shift = ieee80211_chandef_get_shift(&chanctx_conf->def);
		rate_flags = ieee80211_chandef_rate_flags(&chanctx_conf->def);
	}
	rcu_read_unlock();

	/* assume HW handles this */
	if (tx->rate.flags & IEEE80211_TX_RC_MCS)
		return 0;

	/* uh huh? */
	if (WARN_ON_ONCE(tx->rate.idx < 0))
		return 0;

	sband = local->hw.wiphy->bands[info->band];
	txrate = &sband->bitrates[tx->rate.idx];
>>>>>>> refs/remotes/origin/master

	erp = txrate->flags & IEEE80211_RATE_ERP_G;

	/*
	 * data and mgmt (except PS Poll):
	 * - during CFP: 32768
	 * - during contention period:
	 *   if addr1 is group address: 0
	 *   if more fragments = 0 and addr1 is individual address: time to
	 *      transmit one ACK plus SIFS
	 *   if more fragments = 1 and addr1 is individual address: time to
	 *      transmit next fragment plus 2 x ACK plus 3 x SIFS
	 *
	 * IEEE 802.11, 9.6:
	 * - control response frame (CTS or ACK) shall be transmitted using the
	 *   same rate as the immediately previous frame in the frame exchange
	 *   sequence, if this rate belongs to the PHY mandatory rates, or else
	 *   at the highest possible rate belonging to the PHY rates in the
	 *   BSSBasicRateSet
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	hdr = (struct ieee80211_hdr *)tx->skb->data;
=======
	hdr = (struct ieee80211_hdr *)skb->data;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	hdr = (struct ieee80211_hdr *)skb->data;
>>>>>>> refs/remotes/origin/master
	if (ieee80211_is_ctl(hdr->frame_control)) {
		/* TODO: These control frames are not currently sent by
		 * mac80211, but should they be implemented, this function
		 * needs to be updated to support duration field calculation.
		 *
		 * RTS: time needed to transmit pending data/mgmt frame plus
		 *    one CTS frame plus one ACK frame plus 3 x SIFS
		 * CTS: duration of immediately previous RTS minus time
		 *    required to transmit CTS and its SIFS
		 * ACK: 0 if immediately previous directed data/mgmt had
		 *    more=0, with more=1 duration in ACK frame is duration
		 *    from previous frame minus time needed to transmit ACK
		 *    and its SIFS
		 * PS Poll: BIT(15) | BIT(14) | aid
		 */
		return 0;
	}

	/* data/mgmt */
	if (0 /* FIX: data/mgmt during CFP */)
		return cpu_to_le16(32768);

	if (group_addr) /* Group address as the destination - no ACK */
		return 0;

	/* Individual destination address:
	 * IEEE 802.11, Ch. 9.6 (after IEEE 802.11g changes)
	 * CTS and ACK frames shall be transmitted using the highest rate in
	 * basic rate set that is less than or equal to the rate of the
	 * immediately previous frame and that is using the same modulation
	 * (CCK or OFDM). If no basic rate set matches with these requirements,
	 * the highest mandatory rate of the PHY that is less than or equal to
	 * the rate of the previous frame is used.
	 * Mandatory rates for IEEE 802.11g PHY: 1, 2, 5.5, 11, 6, 12, 24 Mbps
	 */
	rate = -1;
	/* use lowest available if everything fails */
	mrate = sband->bitrates[0].bitrate;
	for (i = 0; i < sband->n_bitrates; i++) {
		struct ieee80211_rate *r = &sband->bitrates[i];

		if (r->bitrate > txrate->bitrate)
			break;

<<<<<<< HEAD
		if (tx->sdata->vif.bss_conf.basic_rates & BIT(i))
			rate = r->bitrate;
=======
		if ((rate_flags & r->flags) != rate_flags)
			continue;

		if (tx->sdata->vif.bss_conf.basic_rates & BIT(i))
			rate = DIV_ROUND_UP(r->bitrate, 1 << shift);
>>>>>>> refs/remotes/origin/master

		switch (sband->band) {
		case IEEE80211_BAND_2GHZ: {
			u32 flag;
			if (tx->sdata->flags & IEEE80211_SDATA_OPERATING_GMODE)
				flag = IEEE80211_RATE_MANDATORY_G;
			else
				flag = IEEE80211_RATE_MANDATORY_B;
			if (r->flags & flag)
				mrate = r->bitrate;
			break;
		}
		case IEEE80211_BAND_5GHZ:
			if (r->flags & IEEE80211_RATE_MANDATORY_A)
				mrate = r->bitrate;
			break;
<<<<<<< HEAD
=======
		case IEEE80211_BAND_60GHZ:
			/* TODO, for now fall through */
>>>>>>> refs/remotes/origin/master
		case IEEE80211_NUM_BANDS:
			WARN_ON(1);
			break;
		}
	}
	if (rate == -1) {
		/* No matching basic rate found; use highest suitable mandatory
		 * PHY rate */
<<<<<<< HEAD
		rate = mrate;
	}

<<<<<<< HEAD
	/* Time needed to transmit ACK
	 * (10 bytes + 4-byte FCS = 112 bits) plus SIFS; rounded up
	 * to closest integer */

	dur = ieee80211_frame_duration(local, 10, rate, erp,
=======
	/* Don't calculate ACKs for QoS Frames with NoAck Policy set */
	if (ieee80211_is_data_qos(hdr->frame_control) &&
	    *(ieee80211_get_qos_ctl(hdr)) | IEEE80211_QOS_CTL_ACK_POLICY_NOACK)
=======
		rate = DIV_ROUND_UP(mrate, 1 << shift);
	}

	/* Don't calculate ACKs for QoS Frames with NoAck Policy set */
	if (ieee80211_is_data_qos(hdr->frame_control) &&
	    *(ieee80211_get_qos_ctl(hdr)) & IEEE80211_QOS_CTL_ACK_POLICY_NOACK)
>>>>>>> refs/remotes/origin/master
		dur = 0;
	else
		/* Time needed to transmit ACK
		 * (10 bytes + 4-byte FCS = 112 bits) plus SIFS; rounded up
		 * to closest integer */
<<<<<<< HEAD
		dur = ieee80211_frame_duration(local, 10, rate, erp,
>>>>>>> refs/remotes/origin/cm-10.0
				tx->sdata->vif.bss_conf.use_short_preamble);
=======
		dur = ieee80211_frame_duration(sband->band, 10, rate, erp,
				tx->sdata->vif.bss_conf.use_short_preamble,
				shift);
>>>>>>> refs/remotes/origin/master

	if (next_frag_len) {
		/* Frame is fragmented: duration increases with time needed to
		 * transmit next fragment plus ACK and 2 x SIFS. */
		dur *= 2; /* ACK + SIFS */
		/* next fragment */
<<<<<<< HEAD
		dur += ieee80211_frame_duration(local, next_frag_len,
				txrate->bitrate, erp,
				tx->sdata->vif.bss_conf.use_short_preamble);
=======
		dur += ieee80211_frame_duration(sband->band, next_frag_len,
				txrate->bitrate, erp,
				tx->sdata->vif.bss_conf.use_short_preamble,
				shift);
>>>>>>> refs/remotes/origin/master
	}

	return cpu_to_le16(dur);
}

<<<<<<< HEAD
static inline int is_ieee80211_device(struct ieee80211_local *local,
				      struct net_device *dev)
{
	return local == wdev_priv(dev->ieee80211_ptr);
}

=======
>>>>>>> refs/remotes/origin/master
/* tx handlers */
static ieee80211_tx_result debug_noinline
ieee80211_tx_h_dynamic_ps(struct ieee80211_tx_data *tx)
{
	struct ieee80211_local *local = tx->local;
	struct ieee80211_if_managed *ifmgd;

	/* driver doesn't support power save */
	if (!(local->hw.flags & IEEE80211_HW_SUPPORTS_PS))
		return TX_CONTINUE;

	/* hardware does dynamic power save */
	if (local->hw.flags & IEEE80211_HW_SUPPORTS_DYNAMIC_PS)
		return TX_CONTINUE;

	/* dynamic power save disabled */
	if (local->hw.conf.dynamic_ps_timeout <= 0)
		return TX_CONTINUE;

	/* we are scanning, don't enable power save */
	if (local->scanning)
		return TX_CONTINUE;

	if (!local->ps_sdata)
		return TX_CONTINUE;

	/* No point if we're going to suspend */
	if (local->quiescing)
		return TX_CONTINUE;

	/* dynamic ps is supported only in managed mode */
	if (tx->sdata->vif.type != NL80211_IFTYPE_STATION)
		return TX_CONTINUE;

	ifmgd = &tx->sdata->u.mgd;

	/*
	 * Don't wakeup from power save if u-apsd is enabled, voip ac has
	 * u-apsd enabled and the frame is in voip class. This effectively
	 * means that even if all access categories have u-apsd enabled, in
	 * practise u-apsd is only used with the voip ac. This is a
	 * workaround for the case when received voip class packets do not
	 * have correct qos tag for some reason, due the network or the
	 * peer application.
	 *
<<<<<<< HEAD
<<<<<<< HEAD
	 * Note: local->uapsd_queues access is racy here. If the value is
=======
	 * Note: ifmgd->uapsd_queues access is racy here. If the value is
>>>>>>> refs/remotes/origin/cm-10.0
	 * changed via debugfs, user needs to reassociate manually to have
	 * everything in sync.
	 */
	if ((ifmgd->flags & IEEE80211_STA_UAPSD_ENABLED)
<<<<<<< HEAD
	    && (local->uapsd_queues & IEEE80211_WMM_IE_STA_QOSINFO_AC_VO)
=======
	    && (ifmgd->uapsd_queues & IEEE80211_WMM_IE_STA_QOSINFO_AC_VO)
>>>>>>> refs/remotes/origin/cm-10.0
	    && skb_get_queue_mapping(tx->skb) == 0)
=======
	 * Note: ifmgd->uapsd_queues access is racy here. If the value is
	 * changed via debugfs, user needs to reassociate manually to have
	 * everything in sync.
	 */
	if ((ifmgd->flags & IEEE80211_STA_UAPSD_ENABLED) &&
	    (ifmgd->uapsd_queues & IEEE80211_WMM_IE_STA_QOSINFO_AC_VO) &&
	    skb_get_queue_mapping(tx->skb) == IEEE80211_AC_VO)
>>>>>>> refs/remotes/origin/master
		return TX_CONTINUE;

	if (local->hw.conf.flags & IEEE80211_CONF_PS) {
		ieee80211_stop_queues_by_reason(&local->hw,
<<<<<<< HEAD
=======
						IEEE80211_MAX_QUEUE_MAP,
>>>>>>> refs/remotes/origin/master
						IEEE80211_QUEUE_STOP_REASON_PS);
		ifmgd->flags &= ~IEEE80211_STA_NULLFUNC_ACKED;
		ieee80211_queue_work(&local->hw,
				     &local->dynamic_ps_disable_work);
	}

	/* Don't restart the timer if we're not disassociated */
	if (!ifmgd->associated)
		return TX_CONTINUE;

	mod_timer(&local->dynamic_ps_timer, jiffies +
		  msecs_to_jiffies(local->hw.conf.dynamic_ps_timeout));

	return TX_CONTINUE;
}

static ieee80211_tx_result debug_noinline
ieee80211_tx_h_check_assoc(struct ieee80211_tx_data *tx)
{

	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)tx->skb->data;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx->skb);
<<<<<<< HEAD
<<<<<<< HEAD
	u32 sta_flags;
=======
	bool assoc = false;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bool assoc = false;
>>>>>>> refs/remotes/origin/master

	if (unlikely(info->flags & IEEE80211_TX_CTL_INJECTED))
		return TX_CONTINUE;

	if (unlikely(test_bit(SCAN_SW_SCANNING, &tx->local->scanning)) &&
	    test_bit(SDATA_STATE_OFFCHANNEL, &tx->sdata->state) &&
	    !ieee80211_is_probe_req(hdr->frame_control) &&
	    !ieee80211_is_nullfunc(hdr->frame_control))
		/*
		 * When software scanning only nullfunc frames (to notify
		 * the sleep state to the AP) and probe requests (for the
		 * active scan) are allowed, all other frames should not be
		 * sent and we should not get here, but if we do
		 * nonetheless, drop them to avoid sending them
		 * off-channel. See the link below and
		 * ieee80211_start_scan() for more.
		 *
		 * http://article.gmane.org/gmane.linux.kernel.wireless.general/30089
		 */
		return TX_DROP;

	if (tx->sdata->vif.type == NL80211_IFTYPE_WDS)
		return TX_CONTINUE;

	if (tx->sdata->vif.type == NL80211_IFTYPE_MESH_POINT)
		return TX_CONTINUE;

	if (tx->flags & IEEE80211_TX_PS_BUFFERED)
		return TX_CONTINUE;

<<<<<<< HEAD
<<<<<<< HEAD
	sta_flags = tx->sta ? get_sta_flags(tx->sta) : 0;

	if (likely(tx->flags & IEEE80211_TX_UNICAST)) {
		if (unlikely(!(sta_flags & WLAN_STA_ASSOC) &&
			     tx->sdata->vif.type != NL80211_IFTYPE_ADHOC &&
=======
=======
>>>>>>> refs/remotes/origin/master
	if (tx->sta)
		assoc = test_sta_flag(tx->sta, WLAN_STA_ASSOC);

	if (likely(tx->flags & IEEE80211_TX_UNICAST)) {
		if (unlikely(!assoc &&
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			     ieee80211_is_data(hdr->frame_control))) {
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
			printk(KERN_DEBUG "%s: dropped data frame to not "
			       "associated station %pM\n",
			       tx->sdata->name, hdr->addr1);
#endif /* CONFIG_MAC80211_VERBOSE_DEBUG */
			I802_DEBUG_INC(tx->local->tx_handlers_drop_not_assoc);
			return TX_DROP;
		}
<<<<<<< HEAD
	} else {
		if (unlikely(ieee80211_is_data(hdr->frame_control) &&
			     tx->local->num_sta == 0 &&
			     tx->sdata->vif.type != NL80211_IFTYPE_ADHOC)) {
			/*
			 * No associated STAs - no need to send multicast
			 * frames.
			 */
			return TX_DROP;
		}
		return TX_CONTINUE;
=======
	} else if (unlikely(tx->sdata->vif.type == NL80211_IFTYPE_AP &&
			    ieee80211_is_data(hdr->frame_control) &&
			    !atomic_read(&tx->sdata->u.ap.num_sta_authorized))) {
=======
			     ieee80211_is_data(hdr->frame_control))) {
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
			sdata_info(tx->sdata,
				   "dropped data frame to not associated station %pM\n",
				   hdr->addr1);
#endif
			I802_DEBUG_INC(tx->local->tx_handlers_drop_not_assoc);
			return TX_DROP;
		}
	} else if (unlikely(tx->sdata->vif.type == NL80211_IFTYPE_AP &&
			    ieee80211_is_data(hdr->frame_control) &&
			    !atomic_read(&tx->sdata->u.ap.num_mcast_sta))) {
>>>>>>> refs/remotes/origin/master
		/*
		 * No associated STAs - no need to send multicast
		 * frames.
		 */
		return TX_DROP;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	return TX_CONTINUE;
}

/* This function is called whenever the AP is about to exceed the maximum limit
 * of buffered frames for power saving STAs. This situation should not really
 * happen often during normal operation, so dropping the oldest buffered packet
 * from each queue should be OK to make some room for new frames. */
static void purge_old_ps_buffers(struct ieee80211_local *local)
{
	int total = 0, purged = 0;
	struct sk_buff *skb;
	struct ieee80211_sub_if_data *sdata;
	struct sta_info *sta;

<<<<<<< HEAD
	/*
	 * virtual interfaces are protected by RCU
	 */
	rcu_read_lock();

	list_for_each_entry_rcu(sdata, &local->interfaces, list) {
		struct ieee80211_if_ap *ap;
		if (sdata->vif.type != NL80211_IFTYPE_AP)
			continue;
		ap = &sdata->u.ap;
		skb = skb_dequeue(&ap->ps_bc_buf);
=======
	list_for_each_entry_rcu(sdata, &local->interfaces, list) {
		struct ps_data *ps;

		if (sdata->vif.type == NL80211_IFTYPE_AP)
			ps = &sdata->u.ap.ps;
		else if (ieee80211_vif_is_mesh(&sdata->vif))
			ps = &sdata->u.mesh.ps;
		else
			continue;

		skb = skb_dequeue(&ps->bc_buf);
>>>>>>> refs/remotes/origin/master
		if (skb) {
			purged++;
			dev_kfree_skb(skb);
		}
<<<<<<< HEAD
		total += skb_queue_len(&ap->ps_bc_buf);
	}

<<<<<<< HEAD
	list_for_each_entry_rcu(sta, &local->sta_list, list) {
		skb = skb_dequeue(&sta->ps_tx_buf);
		if (skb) {
			purged++;
			dev_kfree_skb(skb);
		}
		total += skb_queue_len(&sta->ps_tx_buf);
=======
=======
		total += skb_queue_len(&ps->bc_buf);
	}

>>>>>>> refs/remotes/origin/master
	/*
	 * Drop one frame from each station from the lowest-priority
	 * AC that has frames at all.
	 */
	list_for_each_entry_rcu(sta, &local->sta_list, list) {
		int ac;

		for (ac = IEEE80211_AC_BK; ac >= IEEE80211_AC_VO; ac--) {
			skb = skb_dequeue(&sta->ps_tx_buf[ac]);
			total += skb_queue_len(&sta->ps_tx_buf[ac]);
			if (skb) {
				purged++;
<<<<<<< HEAD
				dev_kfree_skb(skb);
				break;
			}
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	rcu_read_unlock();

	local->total_ps_buffered = total;
#ifdef CONFIG_MAC80211_VERBOSE_PS_DEBUG
	wiphy_debug(local->hw.wiphy, "PS buffers full - purged %d frames\n",
		    purged);
#endif
=======
				ieee80211_free_txskb(&local->hw, skb);
				break;
			}
		}
	}

	local->total_ps_buffered = total;
	ps_dbg_hw(&local->hw, "PS buffers full - purged %d frames\n", purged);
>>>>>>> refs/remotes/origin/master
}

static ieee80211_tx_result
ieee80211_tx_h_multicast_ps_buf(struct ieee80211_tx_data *tx)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx->skb);
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)tx->skb->data;
<<<<<<< HEAD
=======
	struct ps_data *ps;
>>>>>>> refs/remotes/origin/master

	/*
	 * broadcast/multicast frame
	 *
<<<<<<< HEAD
	 * If any of the associated stations is in power save mode,
=======
	 * If any of the associated/peer stations is in power save mode,
>>>>>>> refs/remotes/origin/master
	 * the frame is buffered to be sent after DTIM beacon frame.
	 * This is done either by the hardware or us.
	 */

<<<<<<< HEAD
	/* powersaving STAs only in AP/VLAN mode */
	if (!tx->sdata->bss)
		return TX_CONTINUE;
=======
	/* powersaving STAs currently only in AP/VLAN/mesh mode */
	if (tx->sdata->vif.type == NL80211_IFTYPE_AP ||
	    tx->sdata->vif.type == NL80211_IFTYPE_AP_VLAN) {
		if (!tx->sdata->bss)
			return TX_CONTINUE;

		ps = &tx->sdata->bss->ps;
	} else if (ieee80211_vif_is_mesh(&tx->sdata->vif)) {
		ps = &tx->sdata->u.mesh.ps;
	} else {
		return TX_CONTINUE;
	}

>>>>>>> refs/remotes/origin/master

	/* no buffering for ordered frames */
	if (ieee80211_has_order(hdr->frame_control))
		return TX_CONTINUE;

<<<<<<< HEAD
	/* no stations in PS mode */
	if (!atomic_read(&tx->sdata->bss->num_sta_ps))
=======
	if (tx->local->hw.flags & IEEE80211_HW_QUEUE_CONTROL)
		info->hw_queue = tx->sdata->vif.cab_queue;

	/* no stations in PS mode */
	if (!atomic_read(&ps->num_sta_ps))
>>>>>>> refs/remotes/origin/master
		return TX_CONTINUE;

	info->flags |= IEEE80211_TX_CTL_SEND_AFTER_DTIM;

	/* device releases frame after DTIM beacon */
	if (!(tx->local->hw.flags & IEEE80211_HW_HOST_BROADCAST_PS_BUFFERING))
		return TX_CONTINUE;

	/* buffered in mac80211 */
	if (tx->local->total_ps_buffered >= TOTAL_MAX_TX_BUFFER)
		purge_old_ps_buffers(tx->local);

<<<<<<< HEAD
	if (skb_queue_len(&tx->sdata->bss->ps_bc_buf) >= AP_MAX_BC_BUFFER) {
#ifdef CONFIG_MAC80211_VERBOSE_PS_DEBUG
		if (net_ratelimit())
			printk(KERN_DEBUG "%s: BC TX buffer full - dropping the oldest frame\n",
			       tx->sdata->name);
#endif
		dev_kfree_skb(skb_dequeue(&tx->sdata->bss->ps_bc_buf));
	} else
		tx->local->total_ps_buffered++;

	skb_queue_tail(&tx->sdata->bss->ps_bc_buf, tx->skb);
=======
	if (skb_queue_len(&ps->bc_buf) >= AP_MAX_BC_BUFFER) {
		ps_dbg(tx->sdata,
		       "BC TX buffer full - dropping the oldest frame\n");
		dev_kfree_skb(skb_dequeue(&ps->bc_buf));
	} else
		tx->local->total_ps_buffered++;

	skb_queue_tail(&ps->bc_buf, tx->skb);
>>>>>>> refs/remotes/origin/master

	return TX_QUEUED;
}

static int ieee80211_use_mfp(__le16 fc, struct sta_info *sta,
			     struct sk_buff *skb)
{
	if (!ieee80211_is_mgmt(fc))
		return 0;

<<<<<<< HEAD
<<<<<<< HEAD
	if (sta == NULL || !test_sta_flags(sta, WLAN_STA_MFP))
=======
	if (sta == NULL || !test_sta_flag(sta, WLAN_STA_MFP))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (sta == NULL || !test_sta_flag(sta, WLAN_STA_MFP))
>>>>>>> refs/remotes/origin/master
		return 0;

	if (!ieee80211_is_robust_mgmt_frame((struct ieee80211_hdr *)
					    skb->data))
		return 0;

	return 1;
}

static ieee80211_tx_result
ieee80211_tx_h_unicast_ps_buf(struct ieee80211_tx_data *tx)
{
	struct sta_info *sta = tx->sta;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx->skb);
<<<<<<< HEAD
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)tx->skb->data;
	struct ieee80211_local *local = tx->local;
<<<<<<< HEAD
	u32 staflags;

	if (unlikely(!sta ||
		     ieee80211_is_probe_resp(hdr->frame_control) ||
		     ieee80211_is_auth(hdr->frame_control) ||
		     ieee80211_is_assoc_resp(hdr->frame_control) ||
		     ieee80211_is_reassoc_resp(hdr->frame_control)))
		return TX_CONTINUE;

	staflags = get_sta_flags(sta);

	if (unlikely((staflags & (WLAN_STA_PS_STA | WLAN_STA_PS_DRIVER)) &&
		     !(info->flags & IEEE80211_TX_CTL_PSPOLL_RESPONSE))) {
#ifdef CONFIG_MAC80211_VERBOSE_PS_DEBUG
		printk(KERN_DEBUG "STA %pM aid %d: PS buffer (entries "
		       "before %d)\n",
		       sta->sta.addr, sta->sta.aid,
		       skb_queue_len(&sta->ps_tx_buf));
#endif /* CONFIG_MAC80211_VERBOSE_PS_DEBUG */
		if (tx->local->total_ps_buffered >= TOTAL_MAX_TX_BUFFER)
			purge_old_ps_buffers(tx->local);
		if (skb_queue_len(&sta->ps_tx_buf) >= STA_MAX_TX_BUFFER) {
			struct sk_buff *old = skb_dequeue(&sta->ps_tx_buf);
#ifdef CONFIG_MAC80211_VERBOSE_PS_DEBUG
			if (net_ratelimit()) {
				printk(KERN_DEBUG "%s: STA %pM TX "
				       "buffer full - dropping oldest frame\n",
				       tx->sdata->name, sta->sta.addr);
			}
=======
=======
	struct ieee80211_local *local = tx->local;
>>>>>>> refs/remotes/origin/master

	if (unlikely(!sta))
		return TX_CONTINUE;

	if (unlikely((test_sta_flag(sta, WLAN_STA_PS_STA) ||
		      test_sta_flag(sta, WLAN_STA_PS_DRIVER)) &&
		     !(info->flags & IEEE80211_TX_CTL_NO_PS_BUFFER))) {
		int ac = skb_get_queue_mapping(tx->skb);

<<<<<<< HEAD
		/* only deauth, disassoc and action are bufferable MMPDUs */
		if (ieee80211_is_mgmt(hdr->frame_control) &&
		    !ieee80211_is_deauth(hdr->frame_control) &&
		    !ieee80211_is_disassoc(hdr->frame_control) &&
		    !ieee80211_is_action(hdr->frame_control)) {
			info->flags |= IEEE80211_TX_CTL_NO_PS_BUFFER;
			return TX_CONTINUE;
		}

#ifdef CONFIG_MAC80211_VERBOSE_PS_DEBUG
		printk(KERN_DEBUG "STA %pM aid %d: PS buffer for AC %d\n",
		       sta->sta.addr, sta->sta.aid, ac);
#endif /* CONFIG_MAC80211_VERBOSE_PS_DEBUG */
=======
		ps_dbg(sta->sdata, "STA %pM aid %d: PS buffer for AC %d\n",
		       sta->sta.addr, sta->sta.aid, ac);
>>>>>>> refs/remotes/origin/master
		if (tx->local->total_ps_buffered >= TOTAL_MAX_TX_BUFFER)
			purge_old_ps_buffers(tx->local);
		if (skb_queue_len(&sta->ps_tx_buf[ac]) >= STA_MAX_TX_BUFFER) {
			struct sk_buff *old = skb_dequeue(&sta->ps_tx_buf[ac]);
<<<<<<< HEAD
#ifdef CONFIG_MAC80211_VERBOSE_PS_DEBUG
			if (net_ratelimit())
				printk(KERN_DEBUG "%s: STA %pM TX buffer for "
				       "AC %d full - dropping oldest frame\n",
				       tx->sdata->name, sta->sta.addr, ac);
>>>>>>> refs/remotes/origin/cm-10.0
#endif
			dev_kfree_skb(old);
		} else
			tx->local->total_ps_buffered++;

<<<<<<< HEAD
		/*
		 * Queue frame to be sent after STA wakes up/polls,
		 * but don't set the TIM bit if the driver is blocking
		 * wakeup or poll response transmissions anyway.
		 */
		if (skb_queue_empty(&sta->ps_tx_buf) &&
		    !(staflags & WLAN_STA_PS_DRIVER))
			sta_info_set_tim_bit(sta);

		info->control.jiffies = jiffies;
		info->control.vif = &tx->sdata->vif;
		info->flags |= IEEE80211_TX_INTFL_NEED_TXPROCESSING;
		skb_queue_tail(&sta->ps_tx_buf, tx->skb);
=======
=======
			ps_dbg(tx->sdata,
			       "STA %pM TX buffer for AC %d full - dropping oldest frame\n",
			       sta->sta.addr, ac);
			ieee80211_free_txskb(&local->hw, old);
		} else
			tx->local->total_ps_buffered++;

>>>>>>> refs/remotes/origin/master
		info->control.jiffies = jiffies;
		info->control.vif = &tx->sdata->vif;
		info->flags |= IEEE80211_TX_INTFL_NEED_TXPROCESSING;
		skb_queue_tail(&sta->ps_tx_buf[ac], tx->skb);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		if (!timer_pending(&local->sta_cleanup))
			mod_timer(&local->sta_cleanup,
				  round_jiffies(jiffies +
						STA_INFO_CLEANUP_INTERVAL));

<<<<<<< HEAD
<<<<<<< HEAD
		return TX_QUEUED;
	}
#ifdef CONFIG_MAC80211_VERBOSE_PS_DEBUG
	else if (unlikely(staflags & WLAN_STA_PS_STA)) {
		printk(KERN_DEBUG "%s: STA %pM in PS mode, but pspoll "
		       "set -> send frame\n", tx->sdata->name,
		       sta->sta.addr);
=======
=======
>>>>>>> refs/remotes/origin/master
		/*
		 * We queued up some frames, so the TIM bit might
		 * need to be set, recalculate it.
		 */
		sta_info_recalc_tim(sta);

		return TX_QUEUED;
<<<<<<< HEAD
	}
#ifdef CONFIG_MAC80211_VERBOSE_PS_DEBUG
	else if (unlikely(test_sta_flag(sta, WLAN_STA_PS_STA))) {
		printk(KERN_DEBUG
		       "%s: STA %pM in PS mode, but polling/in SP -> send frame\n",
		       tx->sdata->name, sta->sta.addr);
>>>>>>> refs/remotes/origin/cm-10.0
	}
#endif /* CONFIG_MAC80211_VERBOSE_PS_DEBUG */
=======
	} else if (unlikely(test_sta_flag(sta, WLAN_STA_PS_STA))) {
		ps_dbg(tx->sdata,
		       "STA %pM in PS mode, but polling/in SP -> send frame\n",
		       sta->sta.addr);
	}
>>>>>>> refs/remotes/origin/master

	return TX_CONTINUE;
}

static ieee80211_tx_result debug_noinline
ieee80211_tx_h_ps_buf(struct ieee80211_tx_data *tx)
{
<<<<<<< HEAD
	if (unlikely(tx->flags & IEEE80211_TX_PS_BUFFERED))
		return TX_CONTINUE;

=======
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx->skb);
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)tx->skb->data;

	if (unlikely(tx->flags & IEEE80211_TX_PS_BUFFERED))
		return TX_CONTINUE;

	/* only deauth, disassoc and action are bufferable MMPDUs */
	if (ieee80211_is_mgmt(hdr->frame_control) &&
	    !ieee80211_is_deauth(hdr->frame_control) &&
	    !ieee80211_is_disassoc(hdr->frame_control) &&
	    !ieee80211_is_action(hdr->frame_control)) {
		if (tx->flags & IEEE80211_TX_UNICAST)
			info->flags |= IEEE80211_TX_CTL_NO_PS_BUFFER;
		return TX_CONTINUE;
	}

>>>>>>> refs/remotes/origin/master
	if (tx->flags & IEEE80211_TX_UNICAST)
		return ieee80211_tx_h_unicast_ps_buf(tx);
	else
		return ieee80211_tx_h_multicast_ps_buf(tx);
}

static ieee80211_tx_result debug_noinline
ieee80211_tx_h_check_control_port_protocol(struct ieee80211_tx_data *tx)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx->skb);

<<<<<<< HEAD
	if (unlikely(tx->sdata->control_port_protocol == tx->skb->protocol &&
		     tx->sdata->control_port_no_encrypt))
		info->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
=======
	if (unlikely(tx->sdata->control_port_protocol == tx->skb->protocol)) {
		if (tx->sdata->control_port_no_encrypt)
			info->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
		info->control.flags |= IEEE80211_TX_CTRL_PORT_CTRL_PROTO;
	}
>>>>>>> refs/remotes/origin/master

	return TX_CONTINUE;
}

static ieee80211_tx_result debug_noinline
ieee80211_tx_h_select_key(struct ieee80211_tx_data *tx)
{
<<<<<<< HEAD
	struct ieee80211_key *key = NULL;
=======
	struct ieee80211_key *key;
>>>>>>> refs/remotes/origin/master
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx->skb);
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)tx->skb->data;

	if (unlikely(info->flags & IEEE80211_TX_INTFL_DONT_ENCRYPT))
		tx->key = NULL;
	else if (tx->sta && (key = rcu_dereference(tx->sta->ptk)))
		tx->key = key;
	else if (ieee80211_is_mgmt(hdr->frame_control) &&
		 is_multicast_ether_addr(hdr->addr1) &&
		 ieee80211_is_robust_mgmt_frame(hdr) &&
		 (key = rcu_dereference(tx->sdata->default_mgmt_key)))
		tx->key = key;
	else if (is_multicast_ether_addr(hdr->addr1) &&
		 (key = rcu_dereference(tx->sdata->default_multicast_key)))
		tx->key = key;
	else if (!is_multicast_ether_addr(hdr->addr1) &&
		 (key = rcu_dereference(tx->sdata->default_unicast_key)))
		tx->key = key;
<<<<<<< HEAD
	else if (tx->sdata->drop_unencrypted &&
		 (tx->skb->protocol != tx->sdata->control_port_protocol) &&
		 !(info->flags & IEEE80211_TX_CTL_INJECTED) &&
		 (!ieee80211_is_robust_mgmt_frame(hdr) ||
		  (ieee80211_is_action(hdr->frame_control) &&
<<<<<<< HEAD
		   tx->sta && test_sta_flags(tx->sta, WLAN_STA_MFP)))) {
=======
		   tx->sta && test_sta_flag(tx->sta, WLAN_STA_MFP)))) {
>>>>>>> refs/remotes/origin/cm-10.0
		I802_DEBUG_INC(tx->local->tx_handlers_drop_unencrypted);
		return TX_DROP;
	} else
		tx->key = NULL;
=======
	else if (info->flags & IEEE80211_TX_CTL_INJECTED)
		tx->key = NULL;
	else if (!tx->sdata->drop_unencrypted)
		tx->key = NULL;
	else if (tx->skb->protocol == tx->sdata->control_port_protocol)
		tx->key = NULL;
	else if (ieee80211_is_robust_mgmt_frame(hdr) &&
		 !(ieee80211_is_action(hdr->frame_control) &&
		   tx->sta && test_sta_flag(tx->sta, WLAN_STA_MFP)))
		tx->key = NULL;
	else if (ieee80211_is_mgmt(hdr->frame_control) &&
		 !ieee80211_is_robust_mgmt_frame(hdr))
		tx->key = NULL;
	else {
		I802_DEBUG_INC(tx->local->tx_handlers_drop_unencrypted);
		return TX_DROP;
	}
>>>>>>> refs/remotes/origin/master

	if (tx->key) {
		bool skip_hw = false;

		tx->key->tx_rx_count++;
		/* TODO: add threshold stuff again */

		switch (tx->key->conf.cipher) {
		case WLAN_CIPHER_SUITE_WEP40:
		case WLAN_CIPHER_SUITE_WEP104:
<<<<<<< HEAD
<<<<<<< HEAD
			if (ieee80211_is_auth(hdr->frame_control))
				break;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		case WLAN_CIPHER_SUITE_TKIP:
			if (!ieee80211_is_data_present(hdr->frame_control))
				tx->key = NULL;
			break;
		case WLAN_CIPHER_SUITE_CCMP:
			if (!ieee80211_is_data_present(hdr->frame_control) &&
			    !ieee80211_use_mfp(hdr->frame_control, tx->sta,
					       tx->skb))
				tx->key = NULL;
			else
				skip_hw = (tx->key->conf.flags &
<<<<<<< HEAD
					   IEEE80211_KEY_FLAG_SW_MGMT) &&
=======
					   IEEE80211_KEY_FLAG_SW_MGMT_TX) &&
>>>>>>> refs/remotes/origin/master
					ieee80211_is_mgmt(hdr->frame_control);
			break;
		case WLAN_CIPHER_SUITE_AES_CMAC:
			if (!ieee80211_is_mgmt(hdr->frame_control))
				tx->key = NULL;
			break;
		}

<<<<<<< HEAD
<<<<<<< HEAD
=======
		if (unlikely(tx->key && tx->key->flags & KEY_FLAG_TAINTED))
			return TX_DROP;

>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (unlikely(tx->key && tx->key->flags & KEY_FLAG_TAINTED &&
			     !ieee80211_is_deauth(hdr->frame_control)))
			return TX_DROP;

>>>>>>> refs/remotes/origin/master
		if (!skip_hw && tx->key &&
		    tx->key->flags & KEY_FLAG_UPLOADED_TO_HARDWARE)
			info->control.hw_key = &tx->key->conf;
	}

	return TX_CONTINUE;
}

static ieee80211_tx_result debug_noinline
ieee80211_tx_h_rate_ctrl(struct ieee80211_tx_data *tx)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx->skb);
	struct ieee80211_hdr *hdr = (void *)tx->skb->data;
	struct ieee80211_supported_band *sband;
<<<<<<< HEAD
	struct ieee80211_rate *rate;
	int i;
	u32 len;
	bool inval = false, rts = false, short_preamble = false;
	struct ieee80211_tx_rate_control txrc;
<<<<<<< HEAD
	u32 sta_flags;
=======
	bool assoc = false;
>>>>>>> refs/remotes/origin/cm-10.0

	memset(&txrc, 0, sizeof(txrc));

	sband = tx->local->hw.wiphy->bands[tx->channel->band];
=======
	u32 len;
	struct ieee80211_tx_rate_control txrc;
	struct ieee80211_sta_rates *ratetbl = NULL;
	bool assoc = false;

	memset(&txrc, 0, sizeof(txrc));

	sband = tx->local->hw.wiphy->bands[info->band];
>>>>>>> refs/remotes/origin/master

	len = min_t(u32, tx->skb->len + FCS_LEN,
			 tx->local->hw.wiphy->frag_threshold);

	/* set up the tx rate control struct we give the RC algo */
<<<<<<< HEAD
<<<<<<< HEAD
	txrc.hw = local_to_hw(tx->local);
=======
	txrc.hw = &tx->local->hw;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	txrc.hw = &tx->local->hw;
>>>>>>> refs/remotes/origin/master
	txrc.sband = sband;
	txrc.bss_conf = &tx->sdata->vif.bss_conf;
	txrc.skb = tx->skb;
	txrc.reported_rate.idx = -1;
<<<<<<< HEAD
	txrc.rate_idx_mask = tx->sdata->rc_rateidx_mask[tx->channel->band];
=======
	txrc.rate_idx_mask = tx->sdata->rc_rateidx_mask[info->band];
>>>>>>> refs/remotes/origin/master
	if (txrc.rate_idx_mask == (1 << sband->n_bitrates) - 1)
		txrc.max_rate_idx = -1;
	else
		txrc.max_rate_idx = fls(txrc.rate_idx_mask) - 1;
<<<<<<< HEAD
<<<<<<< HEAD
	txrc.bss = (tx->sdata->vif.type == NL80211_IFTYPE_AP ||
=======
	memcpy(txrc.rate_idx_mcs_mask,
	       tx->sdata->rc_rateidx_mcs_mask[tx->channel->band],
	       sizeof(txrc.rate_idx_mcs_mask));
	txrc.bss = (tx->sdata->vif.type == NL80211_IFTYPE_AP ||
		    tx->sdata->vif.type == NL80211_IFTYPE_MESH_POINT ||
>>>>>>> refs/remotes/origin/cm-10.0
=======

	if (tx->sdata->rc_has_mcs_mask[info->band])
		txrc.rate_idx_mcs_mask =
			tx->sdata->rc_rateidx_mcs_mask[info->band];

	txrc.bss = (tx->sdata->vif.type == NL80211_IFTYPE_AP ||
		    tx->sdata->vif.type == NL80211_IFTYPE_MESH_POINT ||
>>>>>>> refs/remotes/origin/master
		    tx->sdata->vif.type == NL80211_IFTYPE_ADHOC);

	/* set up RTS protection if desired */
	if (len > tx->local->hw.wiphy->rts_threshold) {
<<<<<<< HEAD
		txrc.rts = rts = true;
	}

=======
		txrc.rts = true;
	}

	info->control.use_rts = txrc.rts;
	info->control.use_cts_prot = tx->sdata->vif.bss_conf.use_cts_prot;

>>>>>>> refs/remotes/origin/master
	/*
	 * Use short preamble if the BSS can handle it, but not for
	 * management frames unless we know the receiver can handle
	 * that -- the management frame might be to a station that
	 * just wants a probe response.
	 */
	if (tx->sdata->vif.bss_conf.use_short_preamble &&
	    (ieee80211_is_data(hdr->frame_control) ||
<<<<<<< HEAD
<<<<<<< HEAD
	     (tx->sta && test_sta_flags(tx->sta, WLAN_STA_SHORT_PREAMBLE))))
		txrc.short_preamble = short_preamble = true;

	sta_flags = tx->sta ? get_sta_flags(tx->sta) : 0;
=======
	     (tx->sta && test_sta_flag(tx->sta, WLAN_STA_SHORT_PREAMBLE))))
		txrc.short_preamble = short_preamble = true;

	if (tx->sta)
		assoc = test_sta_flag(tx->sta, WLAN_STA_ASSOC);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	     (tx->sta && test_sta_flag(tx->sta, WLAN_STA_SHORT_PREAMBLE))))
		txrc.short_preamble = true;

	info->control.short_preamble = txrc.short_preamble;

	if (tx->sta)
		assoc = test_sta_flag(tx->sta, WLAN_STA_ASSOC);
>>>>>>> refs/remotes/origin/master

	/*
	 * Lets not bother rate control if we're associated and cannot
	 * talk to the sta. This should not happen.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (WARN(test_bit(SCAN_SW_SCANNING, &tx->local->scanning) &&
		 (sta_flags & WLAN_STA_ASSOC) &&
=======
	if (WARN(test_bit(SCAN_SW_SCANNING, &tx->local->scanning) && assoc &&
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (WARN(test_bit(SCAN_SW_SCANNING, &tx->local->scanning) && assoc &&
>>>>>>> refs/remotes/origin/master
		 !rate_usable_index_exists(sband, &tx->sta->sta),
		 "%s: Dropped data frame as no usable bitrate found while "
		 "scanning and associated. Target station: "
		 "%pM on %d GHz band\n",
		 tx->sdata->name, hdr->addr1,
<<<<<<< HEAD
		 tx->channel->band ? 5 : 2))
=======
		 info->band ? 5 : 2))
>>>>>>> refs/remotes/origin/master
		return TX_DROP;

	/*
	 * If we're associated with the sta at this point we know we can at
	 * least send the frame at the lowest bit rate.
	 */
	rate_control_get_rate(tx->sdata, tx->sta, &txrc);

<<<<<<< HEAD
	if (unlikely(info->control.rates[0].idx < 0))
		return TX_DROP;

	if (txrc.reported_rate.idx < 0) {
		txrc.reported_rate = info->control.rates[0];
=======
	if (tx->sta && !info->control.skip_table)
		ratetbl = rcu_dereference(tx->sta->sta.rates);

	if (unlikely(info->control.rates[0].idx < 0)) {
		if (ratetbl) {
			struct ieee80211_tx_rate rate = {
				.idx = ratetbl->rate[0].idx,
				.flags = ratetbl->rate[0].flags,
				.count = ratetbl->rate[0].count
			};

			if (ratetbl->rate[0].idx < 0)
				return TX_DROP;

			tx->rate = rate;
		} else {
			return TX_DROP;
		}
	} else {
		tx->rate = info->control.rates[0];
	}

	if (txrc.reported_rate.idx < 0) {
		txrc.reported_rate = tx->rate;
>>>>>>> refs/remotes/origin/master
		if (tx->sta && ieee80211_is_data(hdr->frame_control))
			tx->sta->last_tx_rate = txrc.reported_rate;
	} else if (tx->sta)
		tx->sta->last_tx_rate = txrc.reported_rate;

<<<<<<< HEAD
=======
	if (ratetbl)
		return TX_CONTINUE;

>>>>>>> refs/remotes/origin/master
	if (unlikely(!info->control.rates[0].count))
		info->control.rates[0].count = 1;

	if (WARN_ON_ONCE((info->control.rates[0].count > 1) &&
			 (info->flags & IEEE80211_TX_CTL_NO_ACK)))
		info->control.rates[0].count = 1;

<<<<<<< HEAD
	if (is_multicast_ether_addr(hdr->addr1)) {
		/*
		 * XXX: verify the rate is in the basic rateset
		 */
		return TX_CONTINUE;
	}

	/*
	 * set up the RTS/CTS rate as the fastest basic rate
	 * that is not faster than the data rate
	 *
	 * XXX: Should this check all retry rates?
	 */
	if (!(info->control.rates[0].flags & IEEE80211_TX_RC_MCS)) {
		s8 baserate = 0;

		rate = &sband->bitrates[info->control.rates[0].idx];

		for (i = 0; i < sband->n_bitrates; i++) {
			/* must be a basic rate */
			if (!(tx->sdata->vif.bss_conf.basic_rates & BIT(i)))
				continue;
			/* must not be faster than the data rate */
			if (sband->bitrates[i].bitrate > rate->bitrate)
				continue;
			/* maximum */
			if (sband->bitrates[baserate].bitrate <
			     sband->bitrates[i].bitrate)
				baserate = i;
		}

		info->control.rts_cts_rate_idx = baserate;
	}

	for (i = 0; i < IEEE80211_TX_MAX_RATES; i++) {
		/*
		 * make sure there's no valid rate following
		 * an invalid one, just in case drivers don't
		 * take the API seriously to stop at -1.
		 */
		if (inval) {
			info->control.rates[i].idx = -1;
			continue;
		}
		if (info->control.rates[i].idx < 0) {
			inval = true;
			continue;
		}

		/*
		 * For now assume MCS is already set up correctly, this
		 * needs to be fixed.
		 */
		if (info->control.rates[i].flags & IEEE80211_TX_RC_MCS) {
			WARN_ON(info->control.rates[i].idx > 76);
			continue;
		}

		/* set up RTS protection if desired */
		if (rts)
			info->control.rates[i].flags |=
				IEEE80211_TX_RC_USE_RTS_CTS;

		/* RC is busted */
		if (WARN_ON_ONCE(info->control.rates[i].idx >=
				 sband->n_bitrates)) {
			info->control.rates[i].idx = -1;
			continue;
		}

		rate = &sband->bitrates[info->control.rates[i].idx];

		/* set up short preamble */
		if (short_preamble &&
		    rate->flags & IEEE80211_RATE_SHORT_PREAMBLE)
			info->control.rates[i].flags |=
				IEEE80211_TX_RC_USE_SHORT_PREAMBLE;

		/* set up G protection */
		if (!rts && tx->sdata->vif.bss_conf.use_cts_prot &&
		    rate->flags & IEEE80211_RATE_ERP_G)
			info->control.rates[i].flags |=
				IEEE80211_TX_RC_USE_CTS_PROTECT;
	}

=======
>>>>>>> refs/remotes/origin/master
	return TX_CONTINUE;
}

static ieee80211_tx_result debug_noinline
ieee80211_tx_h_sequence(struct ieee80211_tx_data *tx)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx->skb);
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)tx->skb->data;
	u16 *seq;
	u8 *qc;
	int tid;

	/*
	 * Packet injection may want to control the sequence
	 * number, if we have no matching interface then we
	 * neither assign one ourselves nor ask the driver to.
	 */
	if (unlikely(info->control.vif->type == NL80211_IFTYPE_MONITOR))
		return TX_CONTINUE;

	if (unlikely(ieee80211_is_ctl(hdr->frame_control)))
		return TX_CONTINUE;

	if (ieee80211_hdrlen(hdr->frame_control) < 24)
		return TX_CONTINUE;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (ieee80211_is_qos_nullfunc(hdr->frame_control))
		return TX_CONTINUE;

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Anything but QoS data that has a sequence number field
	 * (is long enough) gets a sequence number from the global
	 * counter.
	 */
	if (!ieee80211_is_data_qos(hdr->frame_control)) {
=======
	if (ieee80211_is_qos_nullfunc(hdr->frame_control))
		return TX_CONTINUE;

	/*
	 * Anything but QoS data that has a sequence number field
	 * (is long enough) gets a sequence number from the global
	 * counter.  QoS data frames with a multicast destination
	 * also use the global counter (802.11-2012 9.3.2.10).
	 */
	if (!ieee80211_is_data_qos(hdr->frame_control) ||
	    is_multicast_ether_addr(hdr->addr1)) {
>>>>>>> refs/remotes/origin/master
		/* driver should assign sequence number */
		info->flags |= IEEE80211_TX_CTL_ASSIGN_SEQ;
		/* for pure STA mode without beacons, we can do it */
		hdr->seq_ctrl = cpu_to_le16(tx->sdata->sequence_number);
		tx->sdata->sequence_number += 0x10;
		return TX_CONTINUE;
	}

	/*
	 * This should be true for injected/management frames only, for
	 * management frames we have set the IEEE80211_TX_CTL_ASSIGN_SEQ
	 * above since they are not QoS-data frames.
	 */
	if (!tx->sta)
		return TX_CONTINUE;

	/* include per-STA, per-TID sequence counter */

	qc = ieee80211_get_qos_ctl(hdr);
	tid = *qc & IEEE80211_QOS_CTL_TID_MASK;
	seq = &tx->sta->tid_seq[tid];

	hdr->seq_ctrl = cpu_to_le16(*seq);

	/* Increase the sequence number. */
	*seq = (*seq + 0x10) & IEEE80211_SCTL_SEQ;

	return TX_CONTINUE;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int ieee80211_fragment(struct ieee80211_local *local,
			      struct sk_buff *skb, int hdrlen,
			      int frag_threshold)
{
	struct sk_buff *tail = skb, *tmp;
=======
=======
>>>>>>> refs/remotes/origin/master
static int ieee80211_fragment(struct ieee80211_tx_data *tx,
			      struct sk_buff *skb, int hdrlen,
			      int frag_threshold)
{
	struct ieee80211_local *local = tx->local;
	struct ieee80211_tx_info *info;
	struct sk_buff *tmp;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	int per_fragm = frag_threshold - hdrlen - FCS_LEN;
	int pos = hdrlen + per_fragm;
	int rem = skb->len - hdrlen - per_fragm;

	if (WARN_ON(rem < 0))
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* first fragment was already added to queue by caller */

>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* first fragment was already added to queue by caller */

>>>>>>> refs/remotes/origin/master
	while (rem) {
		int fraglen = per_fragm;

		if (fraglen > rem)
			fraglen = rem;
		rem -= fraglen;
		tmp = dev_alloc_skb(local->tx_headroom +
				    frag_threshold +
				    IEEE80211_ENCRYPT_HEADROOM +
				    IEEE80211_ENCRYPT_TAILROOM);
		if (!tmp)
			return -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD
		tail->next = tmp;
		tail = tmp;
=======

		__skb_queue_tail(&tx->skbs, tmp);

>>>>>>> refs/remotes/origin/cm-10.0
=======

		__skb_queue_tail(&tx->skbs, tmp);

>>>>>>> refs/remotes/origin/master
		skb_reserve(tmp, local->tx_headroom +
				 IEEE80211_ENCRYPT_HEADROOM);
		/* copy control information */
		memcpy(tmp->cb, skb->cb, sizeof(tmp->cb));
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

		info = IEEE80211_SKB_CB(tmp);
		info->flags &= ~(IEEE80211_TX_CTL_CLEAR_PS_FILT |
				 IEEE80211_TX_CTL_FIRST_FRAGMENT);

		if (rem)
			info->flags |= IEEE80211_TX_CTL_MORE_FRAMES;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		skb_copy_queue_mapping(tmp, skb);
		tmp->priority = skb->priority;
		tmp->dev = skb->dev;

		/* copy header and data */
		memcpy(skb_put(tmp, hdrlen), skb->data, hdrlen);
		memcpy(skb_put(tmp, fraglen), skb->data + pos, fraglen);

		pos += fraglen;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* adjust first fragment's length */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* adjust first fragment's length */
>>>>>>> refs/remotes/origin/master
	skb->len = hdrlen + per_fragm;
	return 0;
}

static ieee80211_tx_result debug_noinline
ieee80211_tx_h_fragment(struct ieee80211_tx_data *tx)
{
	struct sk_buff *skb = tx->skb;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct ieee80211_hdr *hdr = (void *)skb->data;
	int frag_threshold = tx->local->hw.wiphy->frag_threshold;
	int hdrlen;
	int fragnum;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!(tx->flags & IEEE80211_TX_FRAGMENTED))
=======
=======
>>>>>>> refs/remotes/origin/master
	/* no matter what happens, tx->skb moves to tx->skbs */
	__skb_queue_tail(&tx->skbs, skb);
	tx->skb = NULL;

	if (info->flags & IEEE80211_TX_CTL_DONTFRAG)
		return TX_CONTINUE;

	if (tx->local->ops->set_frag_threshold)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		return TX_CONTINUE;

	/*
	 * Warn when submitting a fragmented A-MPDU frame and drop it.
	 * This scenario is handled in ieee80211_tx_prepare but extra
	 * caution taken here as fragmented ampdu may cause Tx stop.
	 */
	if (WARN_ON(info->flags & IEEE80211_TX_CTL_AMPDU))
		return TX_DROP;

	hdrlen = ieee80211_hdrlen(hdr->frame_control);

<<<<<<< HEAD
<<<<<<< HEAD
	/* internal error, why is TX_FRAGMENTED set? */
=======
	/* internal error, why isn't DONTFRAG set? */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* internal error, why isn't DONTFRAG set? */
>>>>>>> refs/remotes/origin/master
	if (WARN_ON(skb->len + FCS_LEN <= frag_threshold))
		return TX_DROP;

	/*
	 * Now fragment the frame. This will allocate all the fragments and
	 * chain them (using skb as the first fragment) to skb->next.
	 * During transmission, we will remove the successfully transmitted
	 * fragments from this list. When the low-level driver rejects one
	 * of the fragments then we will simply pretend to accept the skb
	 * but store it away as pending.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (ieee80211_fragment(tx->local, skb, hdrlen, frag_threshold))
=======
	if (ieee80211_fragment(tx, skb, hdrlen, frag_threshold))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (ieee80211_fragment(tx, skb, hdrlen, frag_threshold))
>>>>>>> refs/remotes/origin/master
		return TX_DROP;

	/* update duration/seq/flags of fragments */
	fragnum = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	do {
=======

	skb_queue_walk(&tx->skbs, skb) {
>>>>>>> refs/remotes/origin/cm-10.0
		int next_len;
=======

	skb_queue_walk(&tx->skbs, skb) {
>>>>>>> refs/remotes/origin/master
		const __le16 morefrags = cpu_to_le16(IEEE80211_FCTL_MOREFRAGS);

		hdr = (void *)skb->data;
		info = IEEE80211_SKB_CB(skb);

<<<<<<< HEAD
<<<<<<< HEAD
		if (skb->next) {
			hdr->frame_control |= morefrags;
			next_len = skb->next->len;
=======
		if (!skb_queue_is_last(&tx->skbs, skb)) {
			hdr->frame_control |= morefrags;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (!skb_queue_is_last(&tx->skbs, skb)) {
			hdr->frame_control |= morefrags;
>>>>>>> refs/remotes/origin/master
			/*
			 * No multi-rate retries for fragmented frames, that
			 * would completely throw off the NAV at other STAs.
			 */
			info->control.rates[1].idx = -1;
			info->control.rates[2].idx = -1;
			info->control.rates[3].idx = -1;
<<<<<<< HEAD
			info->control.rates[4].idx = -1;
			BUILD_BUG_ON(IEEE80211_TX_MAX_RATES != 5);
			info->flags &= ~IEEE80211_TX_CTL_RATE_CTRL_PROBE;
		} else {
			hdr->frame_control &= ~morefrags;
			next_len = 0;
		}
<<<<<<< HEAD
		hdr->duration_id = ieee80211_duration(tx, 0, next_len);
		hdr->seq_ctrl |= cpu_to_le16(fragnum & IEEE80211_SCTL_FRAG);
		fragnum++;
	} while ((skb = skb->next));
=======
		hdr->seq_ctrl |= cpu_to_le16(fragnum & IEEE80211_SCTL_FRAG);
		fragnum++;
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
			BUILD_BUG_ON(IEEE80211_TX_MAX_RATES != 4);
			info->flags &= ~IEEE80211_TX_CTL_RATE_CTRL_PROBE;
		} else {
			hdr->frame_control &= ~morefrags;
		}
		hdr->seq_ctrl |= cpu_to_le16(fragnum & IEEE80211_SCTL_FRAG);
		fragnum++;
	}
>>>>>>> refs/remotes/origin/master

	return TX_CONTINUE;
}

static ieee80211_tx_result debug_noinline
ieee80211_tx_h_stats(struct ieee80211_tx_data *tx)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct sk_buff *skb = tx->skb;
=======
	struct sk_buff *skb;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct sk_buff *skb;
	int ac = -1;
>>>>>>> refs/remotes/origin/master

	if (!tx->sta)
		return TX_CONTINUE;

<<<<<<< HEAD
	tx->sta->tx_packets++;
<<<<<<< HEAD
	do {
		tx->sta->tx_fragments++;
		tx->sta->tx_bytes += skb->len;
	} while ((skb = skb->next));
=======
	skb_queue_walk(&tx->skbs, skb) {
		tx->sta->tx_fragments++;
		tx->sta->tx_bytes += skb->len;
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	skb_queue_walk(&tx->skbs, skb) {
		ac = skb_get_queue_mapping(skb);
		tx->sta->tx_fragments++;
		tx->sta->tx_bytes[ac] += skb->len;
	}
	if (ac >= 0)
		tx->sta->tx_packets[ac]++;
>>>>>>> refs/remotes/origin/master

	return TX_CONTINUE;
}

static ieee80211_tx_result debug_noinline
ieee80211_tx_h_encrypt(struct ieee80211_tx_data *tx)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx->skb);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (!tx->key)
		return TX_CONTINUE;

	switch (tx->key->conf.cipher) {
	case WLAN_CIPHER_SUITE_WEP40:
	case WLAN_CIPHER_SUITE_WEP104:
		return ieee80211_crypto_wep_encrypt(tx);
	case WLAN_CIPHER_SUITE_TKIP:
		return ieee80211_crypto_tkip_encrypt(tx);
	case WLAN_CIPHER_SUITE_CCMP:
		return ieee80211_crypto_ccmp_encrypt(tx);
	case WLAN_CIPHER_SUITE_AES_CMAC:
		return ieee80211_crypto_aes_cmac_encrypt(tx);
	default:
<<<<<<< HEAD
<<<<<<< HEAD
		/* handle hw-only algorithm */
		if (info->control.hw_key) {
			ieee80211_tx_set_protected(tx);
			return TX_CONTINUE;
		}
		break;

=======
		return ieee80211_crypto_hw_encrypt(tx);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return ieee80211_crypto_hw_encrypt(tx);
>>>>>>> refs/remotes/origin/master
	}

	return TX_DROP;
}

static ieee80211_tx_result debug_noinline
ieee80211_tx_h_calculate_duration(struct ieee80211_tx_data *tx)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct sk_buff *skb = tx->skb;
=======
	struct sk_buff *skb;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct sk_buff *skb;
>>>>>>> refs/remotes/origin/master
	struct ieee80211_hdr *hdr;
	int next_len;
	bool group_addr;

<<<<<<< HEAD
<<<<<<< HEAD
	do {
		hdr = (void *) skb->data;
		if (unlikely(ieee80211_is_pspoll(hdr->frame_control)))
			break; /* must not overwrite AID */
		next_len = skb->next ? skb->next->len : 0;
		group_addr = is_multicast_ether_addr(hdr->addr1);

		hdr->duration_id =
			ieee80211_duration(tx, group_addr, next_len);
	} while ((skb = skb->next));
=======
=======
>>>>>>> refs/remotes/origin/master
	skb_queue_walk(&tx->skbs, skb) {
		hdr = (void *) skb->data;
		if (unlikely(ieee80211_is_pspoll(hdr->frame_control)))
			break; /* must not overwrite AID */
		if (!skb_queue_is_last(&tx->skbs, skb)) {
			struct sk_buff *next = skb_queue_next(&tx->skbs, skb);
			next_len = next->len;
		} else
			next_len = 0;
		group_addr = is_multicast_ether_addr(hdr->addr1);

		hdr->duration_id =
			ieee80211_duration(tx, skb, group_addr, next_len);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return TX_CONTINUE;
}

/* actual transmit path */

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * deal with packet injection down monitor interface
 * with Radiotap Header -- only called for monitor mode interface
 */
static bool __ieee80211_parse_tx_radiotap(struct ieee80211_tx_data *tx,
					  struct sk_buff *skb)
{
	/*
	 * this is the moment to interpret and discard the radiotap header that
	 * must be at the start of the packet injected in Monitor mode
	 *
	 * Need to take some care with endian-ness since radiotap
	 * args are little-endian
	 */

	struct ieee80211_radiotap_iterator iterator;
	struct ieee80211_radiotap_header *rthdr =
		(struct ieee80211_radiotap_header *) skb->data;
	bool hw_frag;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	int ret = ieee80211_radiotap_iterator_init(&iterator, rthdr, skb->len,
						   NULL);

	info->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	tx->flags &= ~IEEE80211_TX_FRAGMENTED;

	/* packet is fragmented in HW if we have a non-NULL driver callback */
	hw_frag = (tx->local->ops->set_frag_threshold != NULL);

	/*
	 * for every radiotap entry that is present
	 * (ieee80211_radiotap_iterator_next returns -ENOENT when no more
	 * entries present, or -EINVAL on error)
	 */

	while (!ret) {
		ret = ieee80211_radiotap_iterator_next(&iterator);

		if (ret)
			continue;

		/* see if this argument is something we can use */
		switch (iterator.this_arg_index) {
		/*
		 * You must take care when dereferencing iterator.this_arg
		 * for multibyte types... the pointer is not aligned.  Use
		 * get_unaligned((type *)iterator.this_arg) to dereference
		 * iterator.this_arg for type "type" safely on all arches.
		*/
		case IEEE80211_RADIOTAP_FLAGS:
			if (*iterator.this_arg & IEEE80211_RADIOTAP_F_FCS) {
				/*
				 * this indicates that the skb we have been
				 * handed has the 32-bit FCS CRC at the end...
				 * we should react to that by snipping it off
				 * because it will be recomputed and added
				 * on transmission
				 */
				if (skb->len < (iterator._max_length + FCS_LEN))
					return false;

				skb_trim(skb, skb->len - FCS_LEN);
			}
			if (*iterator.this_arg & IEEE80211_RADIOTAP_F_WEP)
				info->flags &= ~IEEE80211_TX_INTFL_DONT_ENCRYPT;
			if ((*iterator.this_arg & IEEE80211_RADIOTAP_F_FRAG) &&
								!hw_frag)
				tx->flags |= IEEE80211_TX_FRAGMENTED;
			break;

		/*
		 * Please update the file
		 * Documentation/networking/mac80211-injection.txt
		 * when parsing new fields here.
		 */

		default:
			break;
		}
	}

	if (ret != -ENOENT) /* ie, if we didn't simply run out of fields */
		return false;

	/*
	 * remove the radiotap header
	 * iterator->_max_length was sanity-checked against
	 * skb->len by iterator init
	 */
	skb_pull(skb, iterator._max_length);

	return true;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static bool ieee80211_tx_prep_agg(struct ieee80211_tx_data *tx,
				  struct sk_buff *skb,
				  struct ieee80211_tx_info *info,
				  struct tid_ampdu_tx *tid_tx,
				  int tid)
{
	bool queued = false;
<<<<<<< HEAD
<<<<<<< HEAD

	if (test_bit(HT_AGG_STATE_OPERATIONAL, &tid_tx->state)) {
		info->flags |= IEEE80211_TX_CTL_AMPDU;
=======
=======
>>>>>>> refs/remotes/origin/master
	bool reset_agg_timer = false;
	struct sk_buff *purge_skb = NULL;

	if (test_bit(HT_AGG_STATE_OPERATIONAL, &tid_tx->state)) {
		info->flags |= IEEE80211_TX_CTL_AMPDU;
		reset_agg_timer = true;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	} else if (test_bit(HT_AGG_STATE_WANT_START, &tid_tx->state)) {
		/*
		 * nothing -- this aggregation session is being started
		 * but that might still fail with the driver
		 */
	} else {
		spin_lock(&tx->sta->lock);
		/*
		 * Need to re-check now, because we may get here
		 *
		 *  1) in the window during which the setup is actually
		 *     already done, but not marked yet because not all
		 *     packets are spliced over to the driver pending
		 *     queue yet -- if this happened we acquire the lock
		 *     either before or after the splice happens, but
		 *     need to recheck which of these cases happened.
		 *
		 *  2) during session teardown, if the OPERATIONAL bit
		 *     was cleared due to the teardown but the pointer
		 *     hasn't been assigned NULL yet (or we loaded it
		 *     before it was assigned) -- in this case it may
		 *     now be NULL which means we should just let the
		 *     packet pass through because splicing the frames
		 *     back is already done.
		 */
		tid_tx = rcu_dereference_protected_tid_tx(tx->sta, tid);

		if (!tid_tx) {
			/* do nothing, let packet pass through */
		} else if (test_bit(HT_AGG_STATE_OPERATIONAL, &tid_tx->state)) {
			info->flags |= IEEE80211_TX_CTL_AMPDU;
<<<<<<< HEAD
<<<<<<< HEAD
=======
			reset_agg_timer = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			reset_agg_timer = true;
>>>>>>> refs/remotes/origin/master
		} else {
			queued = true;
			info->control.vif = &tx->sdata->vif;
			info->flags |= IEEE80211_TX_INTFL_NEED_TXPROCESSING;
			__skb_queue_tail(&tid_tx->pending, skb);
<<<<<<< HEAD
<<<<<<< HEAD
		}
		spin_unlock(&tx->sta->lock);
	}

=======
=======
>>>>>>> refs/remotes/origin/master
			if (skb_queue_len(&tid_tx->pending) > STA_MAX_TX_BUFFER)
				purge_skb = __skb_dequeue(&tid_tx->pending);
		}
		spin_unlock(&tx->sta->lock);

		if (purge_skb)
<<<<<<< HEAD
			dev_kfree_skb(purge_skb);
=======
			ieee80211_free_txskb(&tx->local->hw, purge_skb);
>>>>>>> refs/remotes/origin/master
	}

	/* reset session timer */
	if (reset_agg_timer && tid_tx->timeout)
<<<<<<< HEAD
		mod_timer(&tid_tx->session_timer,
			  TU_TO_EXP_TIME(tid_tx->timeout));

>>>>>>> refs/remotes/origin/cm-10.0
=======
		tid_tx->last_tx = jiffies;

>>>>>>> refs/remotes/origin/master
	return queued;
}

/*
 * initialises @tx
 */
static ieee80211_tx_result
ieee80211_tx_prepare(struct ieee80211_sub_if_data *sdata,
		     struct ieee80211_tx_data *tx,
		     struct sk_buff *skb)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_hdr *hdr;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
<<<<<<< HEAD
<<<<<<< HEAD
	int hdrlen, tid;
=======
	int tid;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int tid;
>>>>>>> refs/remotes/origin/master
	u8 *qc;

	memset(tx, 0, sizeof(*tx));
	tx->skb = skb;
	tx->local = local;
	tx->sdata = sdata;
<<<<<<< HEAD
	tx->channel = local->hw.conf.channel;
<<<<<<< HEAD
	/*
	 * Set this flag (used below to indicate "automatic fragmentation"),
	 * it will be cleared/left by radiotap as desired.
	 * Only valid when fragmentation is done by the stack.
	 */
	if (!local->ops->set_frag_threshold)
		tx->flags |= IEEE80211_TX_FRAGMENTED;

	/* process and remove the injection radiotap header */
	if (unlikely(info->flags & IEEE80211_TX_INTFL_HAS_RADIOTAP)) {
		if (!__ieee80211_parse_tx_radiotap(tx, skb))
			return TX_DROP;

		/*
		 * __ieee80211_parse_tx_radiotap has now removed
		 * the radiotap header that was present and pre-filled
		 * 'tx' with tx control information.
		 */
		info->flags &= ~IEEE80211_TX_INTFL_HAS_RADIOTAP;
	}
=======
	__skb_queue_head_init(&tx->skbs);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	__skb_queue_head_init(&tx->skbs);
>>>>>>> refs/remotes/origin/master

	/*
	 * If this flag is set to true anywhere, and we get here,
	 * we are doing the needed processing, so remove the flag
	 * now.
	 */
	info->flags &= ~IEEE80211_TX_INTFL_NEED_TXPROCESSING;

	hdr = (struct ieee80211_hdr *) skb->data;

	if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN) {
		tx->sta = rcu_dereference(sdata->u.vlan.sta);
		if (!tx->sta && sdata->dev->ieee80211_ptr->use_4addr)
			return TX_DROP;
<<<<<<< HEAD
	} else if (info->flags & IEEE80211_TX_CTL_INJECTED ||
=======
	} else if (info->flags & (IEEE80211_TX_CTL_INJECTED |
				  IEEE80211_TX_INTFL_NL80211_FRAME_TX) ||
>>>>>>> refs/remotes/origin/master
		   tx->sdata->control_port_protocol == tx->skb->protocol) {
		tx->sta = sta_info_get_bss(sdata, hdr->addr1);
	}
	if (!tx->sta)
		tx->sta = sta_info_get(sdata, hdr->addr1);

	if (tx->sta && ieee80211_is_data_qos(hdr->frame_control) &&
<<<<<<< HEAD
<<<<<<< HEAD
	    (local->hw.flags & IEEE80211_HW_AMPDU_AGGREGATION)) {
=======
	    !ieee80211_is_qos_nullfunc(hdr->frame_control) &&
	    (local->hw.flags & IEEE80211_HW_AMPDU_AGGREGATION) &&
	    !(local->hw.flags & IEEE80211_HW_TX_AMPDU_SETUP_IN_HW)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	    !ieee80211_is_qos_nullfunc(hdr->frame_control) &&
	    (local->hw.flags & IEEE80211_HW_AMPDU_AGGREGATION) &&
	    !(local->hw.flags & IEEE80211_HW_TX_AMPDU_SETUP_IN_HW)) {
>>>>>>> refs/remotes/origin/master
		struct tid_ampdu_tx *tid_tx;

		qc = ieee80211_get_qos_ctl(hdr);
		tid = *qc & IEEE80211_QOS_CTL_TID_MASK;

		tid_tx = rcu_dereference(tx->sta->ampdu_mlme.tid_tx[tid]);
		if (tid_tx) {
			bool queued;

			queued = ieee80211_tx_prep_agg(tx, skb, info,
						       tid_tx, tid);

			if (unlikely(queued))
				return TX_QUEUED;
		}
	}

	if (is_multicast_ether_addr(hdr->addr1)) {
		tx->flags &= ~IEEE80211_TX_UNICAST;
		info->flags |= IEEE80211_TX_CTL_NO_ACK;
<<<<<<< HEAD
<<<<<<< HEAD
	} else {
		tx->flags |= IEEE80211_TX_UNICAST;
		if (unlikely(local->wifi_wme_noack_test))
			info->flags |= IEEE80211_TX_CTL_NO_ACK;
		else
			info->flags &= ~IEEE80211_TX_CTL_NO_ACK;
	}

	if (tx->flags & IEEE80211_TX_FRAGMENTED) {
		if ((tx->flags & IEEE80211_TX_UNICAST) &&
		    skb->len + FCS_LEN > local->hw.wiphy->frag_threshold &&
		    !(info->flags & IEEE80211_TX_CTL_AMPDU))
			tx->flags |= IEEE80211_TX_FRAGMENTED;
		else
			tx->flags &= ~IEEE80211_TX_FRAGMENTED;
=======
=======
>>>>>>> refs/remotes/origin/master
	} else
		tx->flags |= IEEE80211_TX_UNICAST;

	if (!(info->flags & IEEE80211_TX_CTL_DONTFRAG)) {
		if (!(tx->flags & IEEE80211_TX_UNICAST) ||
		    skb->len + FCS_LEN <= local->hw.wiphy->frag_threshold ||
		    info->flags & IEEE80211_TX_CTL_AMPDU)
			info->flags |= IEEE80211_TX_CTL_DONTFRAG;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	if (!tx->sta)
		info->flags |= IEEE80211_TX_CTL_CLEAR_PS_FILT;
<<<<<<< HEAD
<<<<<<< HEAD
	else if (test_and_clear_sta_flags(tx->sta, WLAN_STA_CLEAR_PS_FILT))
		info->flags |= IEEE80211_TX_CTL_CLEAR_PS_FILT;

	hdrlen = ieee80211_hdrlen(hdr->frame_control);
	if (skb->len > hdrlen + sizeof(rfc1042_header) + 2) {
		u8 *pos = &skb->data[hdrlen + sizeof(rfc1042_header)];
		tx->ethertype = (pos[0] << 8) | pos[1];
	}
=======
	else if (test_and_clear_sta_flag(tx->sta, WLAN_STA_CLEAR_PS_FILT))
		info->flags |= IEEE80211_TX_CTL_CLEAR_PS_FILT;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	else if (test_and_clear_sta_flag(tx->sta, WLAN_STA_CLEAR_PS_FILT))
		info->flags |= IEEE80211_TX_CTL_CLEAR_PS_FILT;

>>>>>>> refs/remotes/origin/master
	info->flags |= IEEE80211_TX_CTL_FIRST_FRAGMENT;

	return TX_CONTINUE;
}

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * Returns false if the frame couldn't be transmitted but was queued instead.
 */
static bool __ieee80211_tx(struct ieee80211_local *local, struct sk_buff **skbp,
			   struct sta_info *sta, bool txpending)
{
	struct sk_buff *skb = *skbp, *next;
	struct ieee80211_tx_info *info;
	struct ieee80211_sub_if_data *sdata;
	unsigned long flags;
	int len;
	bool fragm = false;

	while (skb) {
		int q = skb_get_queue_mapping(skb);
		__le16 fc;
=======
=======
>>>>>>> refs/remotes/origin/master
static bool ieee80211_tx_frags(struct ieee80211_local *local,
			       struct ieee80211_vif *vif,
			       struct ieee80211_sta *sta,
			       struct sk_buff_head *skbs,
			       bool txpending)
{
<<<<<<< HEAD
	struct sk_buff *skb, *tmp;
	struct ieee80211_tx_info *info;
	unsigned long flags;

	skb_queue_walk_safe(skbs, skb, tmp) {
		int q = skb_get_queue_mapping(skb);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ieee80211_tx_control control;
	struct sk_buff *skb, *tmp;
	unsigned long flags;

	skb_queue_walk_safe(skbs, skb, tmp) {
		struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
		int q = info->hw_queue;

#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
		if (WARN_ON_ONCE(q >= local->hw.queues)) {
			__skb_unlink(skb, skbs);
			ieee80211_free_txskb(&local->hw, skb);
			continue;
		}
#endif
>>>>>>> refs/remotes/origin/master

		spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
		if (local->queue_stop_reasons[q] ||
		    (!txpending && !skb_queue_empty(&local->pending[q]))) {
<<<<<<< HEAD
			/*
			 * Since queue is stopped, queue up frames for later
			 * transmission from the tx-pending tasklet when the
			 * queue is woken again.
			 */
<<<<<<< HEAD

			do {
				next = skb->next;
				skb->next = NULL;
				/*
				 * NB: If txpending is true, next must already
				 * be NULL since we must've gone through this
				 * loop before already; therefore we can just
				 * queue the frame to the head without worrying
				 * about reordering of fragments.
				 */
				if (unlikely(txpending))
					__skb_queue_head(&local->pending[q],
							 skb);
				else
					__skb_queue_tail(&local->pending[q],
							 skb);
			} while ((skb = next));
=======
			if (txpending)
				skb_queue_splice_init(skbs, &local->pending[q]);
			else
				skb_queue_splice_tail_init(skbs,
							   &local->pending[q]);
>>>>>>> refs/remotes/origin/cm-10.0

			spin_unlock_irqrestore(&local->queue_stop_reason_lock,
					       flags);
			return false;
		}
		spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);

		info = IEEE80211_SKB_CB(skb);
<<<<<<< HEAD

		if (fragm)
			info->flags &= ~(IEEE80211_TX_CTL_CLEAR_PS_FILT |
					 IEEE80211_TX_CTL_FIRST_FRAGMENT);

		next = skb->next;
		len = skb->len;

		if (next)
			info->flags |= IEEE80211_TX_CTL_MORE_FRAMES;

		sdata = vif_to_sdata(info->control.vif);

		switch (sdata->vif.type) {
		case NL80211_IFTYPE_MONITOR:
			info->control.vif = NULL;
			break;
		case NL80211_IFTYPE_AP_VLAN:
			info->control.vif = &container_of(sdata->bss,
				struct ieee80211_sub_if_data, u.ap)->vif;
			break;
		default:
			/* keep */
			break;
		}

		if (sta && sta->uploaded)
			info->control.sta = &sta->sta;
		else
			info->control.sta = NULL;

		fc = ((struct ieee80211_hdr *)skb->data)->frame_control;
		drv_tx(local, skb);

		ieee80211_tpt_led_trig_tx(local, fc, len);
		*skbp = skb = next;
		ieee80211_led_tx(local, 1);
		fragm = true;
	}

	return true;
=======
		info->control.vif = vif;
		info->control.sta = sta;

		__skb_unlink(skb, skbs);
		drv_tx(local, skb);
=======
			if (unlikely(info->flags &
				     IEEE80211_TX_INTFL_OFFCHAN_TX_OK)) {
				if (local->queue_stop_reasons[q] &
				    ~BIT(IEEE80211_QUEUE_STOP_REASON_OFFCHANNEL)) {
					/*
					 * Drop off-channel frames if queues
					 * are stopped for any reason other
					 * than off-channel operation. Never
					 * queue them.
					 */
					spin_unlock_irqrestore(
						&local->queue_stop_reason_lock,
						flags);
					ieee80211_purge_tx_queue(&local->hw,
								 skbs);
					return true;
				}
			} else {

				/*
				 * Since queue is stopped, queue up frames for
				 * later transmission from the tx-pending
				 * tasklet when the queue is woken again.
				 */
				if (txpending)
					skb_queue_splice_init(skbs,
							      &local->pending[q]);
				else
					skb_queue_splice_tail_init(skbs,
								   &local->pending[q]);

				spin_unlock_irqrestore(&local->queue_stop_reason_lock,
						       flags);
				return false;
			}
		}
		spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);

		info->control.vif = vif;
		control.sta = sta;

		__skb_unlink(skb, skbs);
		drv_tx(local, &control, skb);
>>>>>>> refs/remotes/origin/master
	}

	return true;
}

/*
 * Returns false if the frame couldn't be transmitted but was queued instead.
 */
static bool __ieee80211_tx(struct ieee80211_local *local,
			   struct sk_buff_head *skbs, int led_len,
			   struct sta_info *sta, bool txpending)
{
	struct ieee80211_tx_info *info;
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_vif *vif;
	struct ieee80211_sta *pubsta;
	struct sk_buff *skb;
	bool result = true;
	__le16 fc;

	if (WARN_ON(skb_queue_empty(skbs)))
		return true;

	skb = skb_peek(skbs);
	fc = ((struct ieee80211_hdr *)skb->data)->frame_control;
	info = IEEE80211_SKB_CB(skb);
	sdata = vif_to_sdata(info->control.vif);
	if (sta && !sta->uploaded)
		sta = NULL;

	if (sta)
		pubsta = &sta->sta;
	else
		pubsta = NULL;

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_MONITOR:
<<<<<<< HEAD
		sdata = NULL;
		vif = NULL;
=======
		if (sdata->u.mntr_flags & MONITOR_FLAG_ACTIVE) {
			vif = &sdata->vif;
			break;
		}
		sdata = rcu_dereference(local->monitor_sdata);
		if (sdata) {
			vif = &sdata->vif;
			info->hw_queue =
				vif->hw_queue[skb_get_queue_mapping(skb)];
		} else if (local->hw.flags & IEEE80211_HW_QUEUE_CONTROL) {
			dev_kfree_skb(skb);
			return true;
		} else
			vif = NULL;
>>>>>>> refs/remotes/origin/master
		break;
	case NL80211_IFTYPE_AP_VLAN:
		sdata = container_of(sdata->bss,
				     struct ieee80211_sub_if_data, u.ap);
		/* fall through */
	default:
		vif = &sdata->vif;
		break;
	}

<<<<<<< HEAD
	if (local->ops->tx_frags)
		drv_tx_frags(local, vif, pubsta, skbs);
	else
		result = ieee80211_tx_frags(local, vif, pubsta, skbs,
					    txpending);

	ieee80211_tpt_led_trig_tx(local, fc, led_len);
	ieee80211_led_tx(local, 1);
=======
	result = ieee80211_tx_frags(local, vif, pubsta, skbs,
				    txpending);

	ieee80211_tpt_led_trig_tx(local, fc, led_len);
>>>>>>> refs/remotes/origin/master

	WARN_ON_ONCE(!skb_queue_empty(skbs));

	return result;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * Invoke TX handlers, return 0 on success and non-zero if the
 * frame was dropped or queued.
 */
static int invoke_tx_handlers(struct ieee80211_tx_data *tx)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct sk_buff *skb = tx->skb;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
=======
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx->skb);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx->skb);
>>>>>>> refs/remotes/origin/master
	ieee80211_tx_result res = TX_DROP;

#define CALL_TXH(txh) \
	do {				\
		res = txh(tx);		\
		if (res != TX_CONTINUE)	\
			goto txh_done;	\
	} while (0)

	CALL_TXH(ieee80211_tx_h_dynamic_ps);
	CALL_TXH(ieee80211_tx_h_check_assoc);
	CALL_TXH(ieee80211_tx_h_ps_buf);
	CALL_TXH(ieee80211_tx_h_check_control_port_protocol);
	CALL_TXH(ieee80211_tx_h_select_key);
	if (!(tx->local->hw.flags & IEEE80211_HW_HAS_RATE_CONTROL))
		CALL_TXH(ieee80211_tx_h_rate_ctrl);

<<<<<<< HEAD
<<<<<<< HEAD
	if (unlikely(info->flags & IEEE80211_TX_INTFL_RETRANSMISSION))
		goto txh_done;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (unlikely(info->flags & IEEE80211_TX_INTFL_RETRANSMISSION)) {
		__skb_queue_tail(&tx->skbs, tx->skb);
		tx->skb = NULL;
		goto txh_done;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	CALL_TXH(ieee80211_tx_h_michael_mic_add);
	CALL_TXH(ieee80211_tx_h_sequence);
	CALL_TXH(ieee80211_tx_h_fragment);
	/* handlers after fragment must be aware of tx info fragmentation! */
	CALL_TXH(ieee80211_tx_h_stats);
	CALL_TXH(ieee80211_tx_h_encrypt);
	if (!(tx->local->hw.flags & IEEE80211_HW_HAS_RATE_CONTROL))
		CALL_TXH(ieee80211_tx_h_calculate_duration);
#undef CALL_TXH

 txh_done:
	if (unlikely(res == TX_DROP)) {
		I802_DEBUG_INC(tx->local->tx_handlers_drop);
<<<<<<< HEAD
<<<<<<< HEAD
		while (skb) {
			struct sk_buff *next;

			next = skb->next;
			dev_kfree_skb(skb);
			skb = next;
		}
=======
		if (tx->skb)
			dev_kfree_skb(tx->skb);
		else
			ieee80211_purge_tx_queue(&tx->local->hw, &tx->skbs);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (tx->skb)
			ieee80211_free_txskb(&tx->local->hw, tx->skb);
		else
			ieee80211_purge_tx_queue(&tx->local->hw, &tx->skbs);
>>>>>>> refs/remotes/origin/master
		return -1;
	} else if (unlikely(res == TX_QUEUED)) {
		I802_DEBUG_INC(tx->local->tx_handlers_queued);
		return -1;
	}

	return 0;
}

<<<<<<< HEAD
=======
bool ieee80211_tx_prepare_skb(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif, struct sk_buff *skb,
			      int band, struct ieee80211_sta **sta)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct ieee80211_tx_data tx;

	if (ieee80211_tx_prepare(sdata, &tx, skb) == TX_DROP)
		return false;

	info->band = band;
	info->control.vif = vif;
	info->hw_queue = vif->hw_queue[skb_get_queue_mapping(skb)];

	if (invoke_tx_handlers(&tx))
		return false;

	if (sta) {
		if (tx.sta)
			*sta = &tx.sta->sta;
		else
			*sta = NULL;
	}

	return true;
}
EXPORT_SYMBOL(ieee80211_tx_prepare_skb);

>>>>>>> refs/remotes/origin/master
/*
 * Returns false if the frame couldn't be transmitted but was queued instead.
 */
static bool ieee80211_tx(struct ieee80211_sub_if_data *sdata,
<<<<<<< HEAD
			 struct sk_buff *skb, bool txpending)
=======
			 struct sk_buff *skb, bool txpending,
			 enum ieee80211_band band)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_tx_data tx;
	ieee80211_tx_result res_prepare;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	bool result = true;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int led_len;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int led_len;
>>>>>>> refs/remotes/origin/master

	if (unlikely(skb->len < 10)) {
		dev_kfree_skb(skb);
		return true;
	}

<<<<<<< HEAD
	rcu_read_lock();

	/* initialises tx */
<<<<<<< HEAD
=======
	led_len = skb->len;
>>>>>>> refs/remotes/origin/cm-10.0
	res_prepare = ieee80211_tx_prepare(sdata, &tx, skb);

	if (unlikely(res_prepare == TX_DROP)) {
		dev_kfree_skb(skb);
		goto out;
	} else if (unlikely(res_prepare == TX_QUEUED)) {
		goto out;
	}

	tx.channel = local->hw.conf.channel;
	info->band = tx.channel->band;

	if (!invoke_tx_handlers(&tx))
<<<<<<< HEAD
		result = __ieee80211_tx(local, &tx.skb, tx.sta, txpending);
=======
		result = __ieee80211_tx(local, &tx.skbs, led_len,
					tx.sta, txpending);
>>>>>>> refs/remotes/origin/cm-10.0
 out:
	rcu_read_unlock();
=======
	/* initialises tx */
	led_len = skb->len;
	res_prepare = ieee80211_tx_prepare(sdata, &tx, skb);

	if (unlikely(res_prepare == TX_DROP)) {
		ieee80211_free_txskb(&local->hw, skb);
		return true;
	} else if (unlikely(res_prepare == TX_QUEUED)) {
		return true;
	}

	info->band = band;

	/* set up hw_queue value early */
	if (!(info->flags & IEEE80211_TX_CTL_TX_OFFCHAN) ||
	    !(local->hw.flags & IEEE80211_HW_QUEUE_CONTROL))
		info->hw_queue =
			sdata->vif.hw_queue[skb_get_queue_mapping(skb)];

	if (!invoke_tx_handlers(&tx))
		result = __ieee80211_tx(local, &tx.skbs, led_len,
					tx.sta, txpending);

>>>>>>> refs/remotes/origin/master
	return result;
}

/* device xmit handlers */

<<<<<<< HEAD
<<<<<<< HEAD
static int ieee80211_skb_resize(struct ieee80211_local *local,
				struct sk_buff *skb,
				int head_need, bool may_encrypt)
{
	int tail_need = 0;

	/*
	 * This could be optimised, devices that do full hardware
	 * crypto (including TKIP MMIC) need no tailroom... But we
	 * have no drivers for such devices currently.
	 */
	if (may_encrypt) {
=======
=======
>>>>>>> refs/remotes/origin/master
static int ieee80211_skb_resize(struct ieee80211_sub_if_data *sdata,
				struct sk_buff *skb,
				int head_need, bool may_encrypt)
{
	struct ieee80211_local *local = sdata->local;
	int tail_need = 0;

	if (may_encrypt && sdata->crypto_tx_tailroom_needed_cnt) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		tail_need = IEEE80211_ENCRYPT_TAILROOM;
		tail_need -= skb_tailroom(skb);
		tail_need = max_t(int, tail_need, 0);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (head_need || tail_need) {
		/* Sorry. Can't account for this any more */
		skb_orphan(skb);
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (skb_cloned(skb))
		I802_DEBUG_INC(local->tx_expand_skb_head_cloned);
	else if (head_need || tail_need)
		I802_DEBUG_INC(local->tx_expand_skb_head);
	else
		return 0;

	if (pskb_expand_head(skb, head_need, tail_need, GFP_ATOMIC)) {
		wiphy_debug(local->hw.wiphy,
			    "failed to reallocate TX buffer\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/* update truesize too */
	skb->truesize += head_need + tail_need;

	return 0;
}

static void ieee80211_xmit(struct ieee80211_sub_if_data *sdata,
			   struct sk_buff *skb)
=======
	return 0;
}

void ieee80211_xmit(struct ieee80211_sub_if_data *sdata, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return 0;
}

void ieee80211_xmit(struct ieee80211_sub_if_data *sdata, struct sk_buff *skb,
		    enum ieee80211_band band)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *) skb->data;
<<<<<<< HEAD
<<<<<<< HEAD
	struct ieee80211_sub_if_data *tmp_sdata;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	int headroom;
	bool may_encrypt;

	rcu_read_lock();

<<<<<<< HEAD
	if (unlikely(sdata->vif.type == NL80211_IFTYPE_MONITOR)) {
		int hdrlen;
		u16 len_rthdr;

		info->flags |= IEEE80211_TX_CTL_INJECTED |
			       IEEE80211_TX_INTFL_HAS_RADIOTAP;

		len_rthdr = ieee80211_get_radiotap_len(skb->data);
		hdr = (struct ieee80211_hdr *)(skb->data + len_rthdr);
		hdrlen = ieee80211_hdrlen(hdr->frame_control);

		/* check the header is complete in the frame */
		if (likely(skb->len >= len_rthdr + hdrlen)) {
			/*
			 * We process outgoing injected frames that have a
			 * local address we handle as though they are our
			 * own frames.
			 * This code here isn't entirely correct, the local
			 * MAC address is not necessarily enough to find
			 * the interface to use; for that proper VLAN/WDS
			 * support we will need a different mechanism.
			 */

			list_for_each_entry_rcu(tmp_sdata, &local->interfaces,
						list) {
				if (!ieee80211_sdata_running(tmp_sdata))
					continue;
				if (tmp_sdata->vif.type ==
				    NL80211_IFTYPE_MONITOR ||
				    tmp_sdata->vif.type ==
				    NL80211_IFTYPE_AP_VLAN ||
					tmp_sdata->vif.type ==
				    NL80211_IFTYPE_WDS)
					continue;
				if (compare_ether_addr(tmp_sdata->vif.addr,
						       hdr->addr2) == 0) {
					sdata = tmp_sdata;
					break;
				}
			}
		}
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int headroom;
	bool may_encrypt;

>>>>>>> refs/remotes/origin/master
	may_encrypt = !(info->flags & IEEE80211_TX_INTFL_DONT_ENCRYPT);

	headroom = local->tx_headroom;
	if (may_encrypt)
		headroom += IEEE80211_ENCRYPT_HEADROOM;
	headroom -= skb_headroom(skb);
	headroom = max_t(int, 0, headroom);

<<<<<<< HEAD
<<<<<<< HEAD
	if (ieee80211_skb_resize(local, skb, headroom, may_encrypt)) {
=======
	if (ieee80211_skb_resize(sdata, skb, headroom, may_encrypt)) {
>>>>>>> refs/remotes/origin/cm-10.0
		dev_kfree_skb(skb);
		rcu_read_unlock();
=======
	if (ieee80211_skb_resize(sdata, skb, headroom, may_encrypt)) {
		ieee80211_free_txskb(&local->hw, skb);
>>>>>>> refs/remotes/origin/master
		return;
	}

	hdr = (struct ieee80211_hdr *) skb->data;
	info->control.vif = &sdata->vif;

<<<<<<< HEAD
	if (ieee80211_vif_is_mesh(&sdata->vif) &&
	    ieee80211_is_data(hdr->frame_control) &&
		!is_multicast_ether_addr(hdr->addr1))
<<<<<<< HEAD
			if (mesh_nexthop_lookup(skb, sdata)) {
=======
			if (mesh_nexthop_resolve(skb, sdata)) {
>>>>>>> refs/remotes/origin/cm-10.0
				/* skb queued: don't free */
				rcu_read_unlock();
				return;
			}

<<<<<<< HEAD
	ieee80211_set_qos_hdr(local, skb);
=======
	ieee80211_set_qos_hdr(sdata, skb);
>>>>>>> refs/remotes/origin/cm-10.0
	ieee80211_tx(sdata, skb, false);
	rcu_read_unlock();
}

<<<<<<< HEAD
=======
=======
	if (ieee80211_vif_is_mesh(&sdata->vif)) {
		if (ieee80211_is_data(hdr->frame_control) &&
		    is_unicast_ether_addr(hdr->addr1)) {
			if (mesh_nexthop_resolve(sdata, skb))
				return; /* skb queued: don't free */
		} else {
			ieee80211_mps_set_frame_flags(sdata, NULL, hdr);
		}
	}

	ieee80211_set_qos_hdr(sdata, skb);
	ieee80211_tx(sdata, skb, false, band);
}

>>>>>>> refs/remotes/origin/master
static bool ieee80211_parse_tx_radiotap(struct sk_buff *skb)
{
	struct ieee80211_radiotap_iterator iterator;
	struct ieee80211_radiotap_header *rthdr =
		(struct ieee80211_radiotap_header *) skb->data;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	int ret = ieee80211_radiotap_iterator_init(&iterator, rthdr, skb->len,
						   NULL);
	u16 txflags;

	info->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT |
		       IEEE80211_TX_CTL_DONTFRAG;

	/*
	 * for every radiotap entry that is present
	 * (ieee80211_radiotap_iterator_next returns -ENOENT when no more
	 * entries present, or -EINVAL on error)
	 */

	while (!ret) {
		ret = ieee80211_radiotap_iterator_next(&iterator);

		if (ret)
			continue;

		/* see if this argument is something we can use */
		switch (iterator.this_arg_index) {
		/*
		 * You must take care when dereferencing iterator.this_arg
		 * for multibyte types... the pointer is not aligned.  Use
		 * get_unaligned((type *)iterator.this_arg) to dereference
		 * iterator.this_arg for type "type" safely on all arches.
		*/
		case IEEE80211_RADIOTAP_FLAGS:
			if (*iterator.this_arg & IEEE80211_RADIOTAP_F_FCS) {
				/*
				 * this indicates that the skb we have been
				 * handed has the 32-bit FCS CRC at the end...
				 * we should react to that by snipping it off
				 * because it will be recomputed and added
				 * on transmission
				 */
				if (skb->len < (iterator._max_length + FCS_LEN))
					return false;

				skb_trim(skb, skb->len - FCS_LEN);
			}
			if (*iterator.this_arg & IEEE80211_RADIOTAP_F_WEP)
				info->flags &= ~IEEE80211_TX_INTFL_DONT_ENCRYPT;
			if (*iterator.this_arg & IEEE80211_RADIOTAP_F_FRAG)
				info->flags &= ~IEEE80211_TX_CTL_DONTFRAG;
			break;

		case IEEE80211_RADIOTAP_TX_FLAGS:
			txflags = get_unaligned_le16(iterator.this_arg);
			if (txflags & IEEE80211_RADIOTAP_F_TX_NOACK)
				info->flags |= IEEE80211_TX_CTL_NO_ACK;
			break;

		/*
		 * Please update the file
		 * Documentation/networking/mac80211-injection.txt
		 * when parsing new fields here.
		 */

		default:
			break;
		}
	}

	if (ret != -ENOENT) /* ie, if we didn't simply run out of fields */
		return false;

	/*
	 * remove the radiotap header
	 * iterator->_max_length was sanity-checked against
	 * skb->len by iterator init
	 */
	skb_pull(skb, iterator._max_length);

	return true;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
netdev_tx_t ieee80211_monitor_start_xmit(struct sk_buff *skb,
					 struct net_device *dev)
{
	struct ieee80211_local *local = wdev_priv(dev->ieee80211_ptr);
<<<<<<< HEAD
	struct ieee80211_channel *chan = local->hw.conf.channel;
	struct ieee80211_radiotap_header *prthdr =
		(struct ieee80211_radiotap_header *)skb->data;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
<<<<<<< HEAD
	u16 len_rthdr;
=======
=======
	struct ieee80211_chanctx_conf *chanctx_conf;
	struct ieee80211_channel *chan;
	struct ieee80211_radiotap_header *prthdr =
		(struct ieee80211_radiotap_header *)skb->data;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
>>>>>>> refs/remotes/origin/master
	struct ieee80211_hdr *hdr;
	struct ieee80211_sub_if_data *tmp_sdata, *sdata;
	u16 len_rthdr;
	int hdrlen;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * Frame injection is not allowed if beaconing is not allowed
	 * or if we need radar detection. Beaconing is usually not allowed when
	 * the mode or operation (Adhoc, AP, Mesh) does not support DFS.
	 * Passive scan is also used in world regulatory domains where
	 * your country is not known and as such it should be treated as
	 * NO TX unless the channel is explicitly allowed in which case
	 * your current regulatory domain would not have the passive scan
	 * flag.
	 *
	 * Since AP mode uses monitor interfaces to inject/TX management
	 * frames we can make AP mode the exception to this rule once it
	 * supports radar detection as its implementation can deal with
	 * radar detection by itself. We can do that later by adding a
	 * monitor flag interfaces used for AP support.
	 */
	if ((chan->flags & (IEEE80211_CHAN_NO_IBSS | IEEE80211_CHAN_RADAR |
	     IEEE80211_CHAN_PASSIVE_SCAN)))
		goto fail;
=======
>>>>>>> refs/remotes/origin/master

	/* check for not even having the fixed radiotap header part */
	if (unlikely(skb->len < sizeof(struct ieee80211_radiotap_header)))
		goto fail; /* too short to be possibly valid */

	/* is it a header version we can trust to find length from? */
	if (unlikely(prthdr->it_version))
		goto fail; /* only version 0 is supported */

	/* then there must be a radiotap header with a length we can use */
	len_rthdr = ieee80211_get_radiotap_len(skb->data);

	/* does the skb contain enough to deliver on the alleged length? */
	if (unlikely(skb->len < len_rthdr))
		goto fail; /* skb too short for claimed rt header extent */

	/*
	 * fix up the pointers accounting for the radiotap
	 * header still being in there.  We are being given
	 * a precooked IEEE80211 header so no need for
	 * normal processing
	 */
	skb_set_mac_header(skb, len_rthdr);
	/*
	 * these are just fixed to the end of the rt area since we
	 * don't have any better information and at this point, nobody cares
	 */
	skb_set_network_header(skb, len_rthdr);
	skb_set_transport_header(skb, len_rthdr);

<<<<<<< HEAD
<<<<<<< HEAD
	memset(info, 0, sizeof(*info));

	info->flags |= IEEE80211_TX_CTL_REQ_TX_STATUS;

	/* pass the radiotap header up to xmit */
	ieee80211_xmit(IEEE80211_DEV_TO_SUB_IF(dev), skb);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (skb->len < len_rthdr + 2)
		goto fail;

	hdr = (struct ieee80211_hdr *)(skb->data + len_rthdr);
	hdrlen = ieee80211_hdrlen(hdr->frame_control);

	if (skb->len < len_rthdr + hdrlen)
		goto fail;

	/*
	 * Initialize skb->protocol if the injected frame is a data frame
	 * carrying a rfc1042 header
	 */
	if (ieee80211_is_data(hdr->frame_control) &&
	    skb->len >= len_rthdr + hdrlen + sizeof(rfc1042_header) + 2) {
		u8 *payload = (u8 *)hdr + hdrlen;

<<<<<<< HEAD
		if (compare_ether_addr(payload, rfc1042_header) == 0)
=======
		if (ether_addr_equal(payload, rfc1042_header))
>>>>>>> refs/remotes/origin/master
			skb->protocol = cpu_to_be16((payload[6] << 8) |
						    payload[7]);
	}

	memset(info, 0, sizeof(*info));

	info->flags = IEEE80211_TX_CTL_REQ_TX_STATUS |
		      IEEE80211_TX_CTL_INJECTED;

	/* process and remove the injection radiotap header */
	if (!ieee80211_parse_tx_radiotap(skb))
		goto fail;

	rcu_read_lock();

	/*
	 * We process outgoing injected frames that have a local address
	 * we handle as though they are non-injected frames.
	 * This code here isn't entirely correct, the local MAC address
	 * isn't always enough to find the interface to use; for proper
	 * VLAN/WDS support we will need a different mechanism (which
	 * likely isn't going to be monitor interfaces).
	 */
	sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	list_for_each_entry_rcu(tmp_sdata, &local->interfaces, list) {
		if (!ieee80211_sdata_running(tmp_sdata))
			continue;
		if (tmp_sdata->vif.type == NL80211_IFTYPE_MONITOR ||
		    tmp_sdata->vif.type == NL80211_IFTYPE_AP_VLAN ||
		    tmp_sdata->vif.type == NL80211_IFTYPE_WDS)
			continue;
<<<<<<< HEAD
		if (compare_ether_addr(tmp_sdata->vif.addr, hdr->addr2) == 0) {
=======
		if (ether_addr_equal(tmp_sdata->vif.addr, hdr->addr2)) {
>>>>>>> refs/remotes/origin/master
			sdata = tmp_sdata;
			break;
		}
	}

<<<<<<< HEAD
	ieee80211_xmit(sdata, skb);
	rcu_read_unlock();

>>>>>>> refs/remotes/origin/cm-10.0
	return NETDEV_TX_OK;

=======
	chanctx_conf = rcu_dereference(sdata->vif.chanctx_conf);
	if (!chanctx_conf) {
		tmp_sdata = rcu_dereference(local->monitor_sdata);
		if (tmp_sdata)
			chanctx_conf =
				rcu_dereference(tmp_sdata->vif.chanctx_conf);
	}

	if (chanctx_conf)
		chan = chanctx_conf->def.chan;
	else if (!local->use_chanctx)
		chan = local->_oper_chandef.chan;
	else
		goto fail_rcu;

	/*
	 * Frame injection is not allowed if beaconing is not allowed
	 * or if we need radar detection. Beaconing is usually not allowed when
	 * the mode or operation (Adhoc, AP, Mesh) does not support DFS.
	 * Passive scan is also used in world regulatory domains where
	 * your country is not known and as such it should be treated as
	 * NO TX unless the channel is explicitly allowed in which case
	 * your current regulatory domain would not have the passive scan
	 * flag.
	 *
	 * Since AP mode uses monitor interfaces to inject/TX management
	 * frames we can make AP mode the exception to this rule once it
	 * supports radar detection as its implementation can deal with
	 * radar detection by itself. We can do that later by adding a
	 * monitor flag interfaces used for AP support.
	 */
	if ((chan->flags & (IEEE80211_CHAN_NO_IBSS | IEEE80211_CHAN_RADAR |
			    IEEE80211_CHAN_PASSIVE_SCAN)))
		goto fail_rcu;

	ieee80211_xmit(sdata, skb, chan->band);
	rcu_read_unlock();

	return NETDEV_TX_OK;

fail_rcu:
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master
fail:
	dev_kfree_skb(skb);
	return NETDEV_TX_OK; /* meaning, we dealt with the skb */
}

/**
 * ieee80211_subif_start_xmit - netif start_xmit function for Ethernet-type
 * subinterfaces (wlan#, WDS, and VLAN interfaces)
 * @skb: packet to be sent
 * @dev: incoming interface
 *
 * Returns: 0 on success (and frees skb in this case) or 1 on failure (skb will
 * not be freed, and caller is responsible for either retrying later or freeing
 * skb).
 *
 * This function takes in an Ethernet header and encapsulates it with suitable
 * IEEE 802.11 header based on which interface the packet is coming in. The
 * encapsulated packet will then be passed to master interface, wlan#.11, for
 * transmission (through low-level driver).
 */
netdev_tx_t ieee80211_subif_start_xmit(struct sk_buff *skb,
				    struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_tx_info *info;
<<<<<<< HEAD
	int ret = NETDEV_TX_BUSY, head_need;
=======
	int head_need;
>>>>>>> refs/remotes/origin/master
	u16 ethertype, hdrlen,  meshhdrlen = 0;
	__le16 fc;
	struct ieee80211_hdr hdr;
	struct ieee80211s_hdr mesh_hdr __maybe_unused;
<<<<<<< HEAD
	struct mesh_path __maybe_unused *mppath = NULL;
=======
	struct mesh_path __maybe_unused *mppath = NULL, *mpath = NULL;
>>>>>>> refs/remotes/origin/master
	const u8 *encaps_data;
	int encaps_len, skip_header_bytes;
	int nh_pos, h_pos;
	struct sta_info *sta = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	u32 sta_flags = 0;
	struct sk_buff *tmp_skb;
=======
=======
>>>>>>> refs/remotes/origin/master
	bool wme_sta = false, authorized = false, tdls_auth = false;
	bool tdls_direct = false;
	bool multicast;
	u32 info_flags = 0;
	u16 info_id = 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	if (unlikely(skb->len < ETH_HLEN)) {
		ret = NETDEV_TX_OK;
		goto fail;
	}
=======
	struct ieee80211_chanctx_conf *chanctx_conf;
	struct ieee80211_sub_if_data *ap_sdata;
	enum ieee80211_band band;

	if (unlikely(skb->len < ETH_HLEN))
		goto fail;
>>>>>>> refs/remotes/origin/master

	/* convert Ethernet header to proper 802.11 header (based on
	 * operation mode) */
	ethertype = (skb->data[12] << 8) | skb->data[13];
	fc = cpu_to_le16(IEEE80211_FTYPE_DATA | IEEE80211_STYPE_DATA);

<<<<<<< HEAD
	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP_VLAN:
		rcu_read_lock();
=======
	rcu_read_lock();

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP_VLAN:
>>>>>>> refs/remotes/origin/master
		sta = rcu_dereference(sdata->u.vlan.sta);
		if (sta) {
			fc |= cpu_to_le16(IEEE80211_FCTL_FROMDS | IEEE80211_FCTL_TODS);
			/* RA TA DA SA */
			memcpy(hdr.addr1, sta->sta.addr, ETH_ALEN);
			memcpy(hdr.addr2, sdata->vif.addr, ETH_ALEN);
			memcpy(hdr.addr3, skb->data, ETH_ALEN);
			memcpy(hdr.addr4, skb->data + ETH_ALEN, ETH_ALEN);
			hdrlen = 30;
<<<<<<< HEAD
<<<<<<< HEAD
			sta_flags = get_sta_flags(sta);
=======
			authorized = test_sta_flag(sta, WLAN_STA_AUTHORIZED);
			wme_sta = test_sta_flag(sta, WLAN_STA_WME);
>>>>>>> refs/remotes/origin/cm-10.0
		}
		rcu_read_unlock();
=======
			authorized = test_sta_flag(sta, WLAN_STA_AUTHORIZED);
			wme_sta = test_sta_flag(sta, WLAN_STA_WME);
		}
		ap_sdata = container_of(sdata->bss, struct ieee80211_sub_if_data,
					u.ap);
		chanctx_conf = rcu_dereference(ap_sdata->vif.chanctx_conf);
		if (!chanctx_conf)
			goto fail_rcu;
		band = chanctx_conf->def.chan->band;
>>>>>>> refs/remotes/origin/master
		if (sta)
			break;
		/* fall through */
	case NL80211_IFTYPE_AP:
<<<<<<< HEAD
=======
		if (sdata->vif.type == NL80211_IFTYPE_AP)
			chanctx_conf = rcu_dereference(sdata->vif.chanctx_conf);
		if (!chanctx_conf)
			goto fail_rcu;
>>>>>>> refs/remotes/origin/master
		fc |= cpu_to_le16(IEEE80211_FCTL_FROMDS);
		/* DA BSSID SA */
		memcpy(hdr.addr1, skb->data, ETH_ALEN);
		memcpy(hdr.addr2, sdata->vif.addr, ETH_ALEN);
		memcpy(hdr.addr3, skb->data + ETH_ALEN, ETH_ALEN);
		hdrlen = 24;
<<<<<<< HEAD
=======
		band = chanctx_conf->def.chan->band;
>>>>>>> refs/remotes/origin/master
		break;
	case NL80211_IFTYPE_WDS:
		fc |= cpu_to_le16(IEEE80211_FCTL_FROMDS | IEEE80211_FCTL_TODS);
		/* RA TA DA SA */
		memcpy(hdr.addr1, sdata->u.wds.remote_addr, ETH_ALEN);
		memcpy(hdr.addr2, sdata->vif.addr, ETH_ALEN);
		memcpy(hdr.addr3, skb->data, ETH_ALEN);
		memcpy(hdr.addr4, skb->data + ETH_ALEN, ETH_ALEN);
		hdrlen = 30;
<<<<<<< HEAD
		break;
#ifdef CONFIG_MAC80211_MESH
	case NL80211_IFTYPE_MESH_POINT:
		if (!sdata->u.mesh.mshcfg.dot11MeshTTL) {
			/* Do not send frames with mesh_ttl == 0 */
			sdata->u.mesh.mshstats.dropped_frames_ttl++;
			ret = NETDEV_TX_OK;
			goto fail;
		}
		rcu_read_lock();
		if (!is_multicast_ether_addr(skb->data))
			mppath = mpp_path_lookup(skb->data, sdata);
=======
		/*
		 * This is the exception! WDS style interfaces are prohibited
		 * when channel contexts are in used so this must be valid
		 */
		band = local->hw.conf.chandef.chan->band;
		break;
#ifdef CONFIG_MAC80211_MESH
	case NL80211_IFTYPE_MESH_POINT:
		if (!is_multicast_ether_addr(skb->data)) {
			struct sta_info *next_hop;
			bool mpp_lookup = true;

			mpath = mesh_path_lookup(sdata, skb->data);
			if (mpath) {
				mpp_lookup = false;
				next_hop = rcu_dereference(mpath->next_hop);
				if (!next_hop ||
				    !(mpath->flags & (MESH_PATH_ACTIVE |
						      MESH_PATH_RESOLVING)))
					mpp_lookup = true;
			}

			if (mpp_lookup)
				mppath = mpp_path_lookup(sdata, skb->data);

			if (mppath && mpath)
				mesh_path_del(mpath->sdata, mpath->dst);
		}
>>>>>>> refs/remotes/origin/master

		/*
		 * Use address extension if it is a packet from
		 * another interface or if we know the destination
		 * is being proxied by a portal (i.e. portal address
		 * differs from proxied address)
		 */
<<<<<<< HEAD
		if (compare_ether_addr(sdata->vif.addr,
				       skb->data + ETH_ALEN) == 0 &&
		    !(mppath && compare_ether_addr(mppath->mpp, skb->data))) {
			hdrlen = ieee80211_fill_mesh_addresses(&hdr, &fc,
					skb->data, skb->data + ETH_ALEN);
			rcu_read_unlock();
			meshhdrlen = ieee80211_new_mesh_header(&mesh_hdr,
					sdata, NULL, NULL);
		} else {
			int is_mesh_mcast = 1;
			const u8 *mesh_da;

			if (is_multicast_ether_addr(skb->data))
				/* DA TA mSA AE:SA */
				mesh_da = skb->data;
			else {
				static const u8 bcast[ETH_ALEN] =
					{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
				if (mppath) {
					/* RA TA mDA mSA AE:DA SA */
					mesh_da = mppath->mpp;
					is_mesh_mcast = 0;
				} else {
					/* DA TA mSA AE:SA */
					mesh_da = bcast;
				}
			}
			hdrlen = ieee80211_fill_mesh_addresses(&hdr, &fc,
					mesh_da, sdata->vif.addr);
			rcu_read_unlock();
			if (is_mesh_mcast)
				meshhdrlen =
					ieee80211_new_mesh_header(&mesh_hdr,
							sdata,
							skb->data + ETH_ALEN,
							NULL);
			else
				meshhdrlen =
					ieee80211_new_mesh_header(&mesh_hdr,
							sdata,
							skb->data,
							skb->data + ETH_ALEN);

		}
		break;
#endif
	case NL80211_IFTYPE_STATION:
<<<<<<< HEAD
		memcpy(hdr.addr1, sdata->u.mgd.bssid, ETH_ALEN);
		if (sdata->u.mgd.use_4addr &&
		    cpu_to_be16(ethertype) != sdata->control_port_protocol) {
			fc |= cpu_to_le16(IEEE80211_FCTL_FROMDS | IEEE80211_FCTL_TODS);
			/* RA TA DA SA */
=======
		if (sdata->wdev.wiphy->flags & WIPHY_FLAG_SUPPORTS_TDLS) {
			bool tdls_peer = false;

			rcu_read_lock();
=======
		if (ether_addr_equal(sdata->vif.addr, skb->data + ETH_ALEN) &&
		    !(mppath && !ether_addr_equal(mppath->mpp, skb->data))) {
			hdrlen = ieee80211_fill_mesh_addresses(&hdr, &fc,
					skb->data, skb->data + ETH_ALEN);
			meshhdrlen = ieee80211_new_mesh_header(sdata, &mesh_hdr,
							       NULL, NULL);
		} else {
			/* DS -> MBSS (802.11-2012 13.11.3.3).
			 * For unicast with unknown forwarding information,
			 * destination might be in the MBSS or if that fails
			 * forwarded to another mesh gate. In either case
			 * resolution will be handled in ieee80211_xmit(), so
			 * leave the original DA. This also works for mcast */
			const u8 *mesh_da = skb->data;

			if (mppath)
				mesh_da = mppath->mpp;
			else if (mpath)
				mesh_da = mpath->dst;

			hdrlen = ieee80211_fill_mesh_addresses(&hdr, &fc,
					mesh_da, sdata->vif.addr);
			if (is_multicast_ether_addr(mesh_da))
				/* DA TA mSA AE:SA */
				meshhdrlen = ieee80211_new_mesh_header(
						sdata, &mesh_hdr,
						skb->data + ETH_ALEN, NULL);
			else
				/* RA TA mDA mSA AE:DA SA */
				meshhdrlen = ieee80211_new_mesh_header(
						sdata, &mesh_hdr, skb->data,
						skb->data + ETH_ALEN);

		}
		chanctx_conf = rcu_dereference(sdata->vif.chanctx_conf);
		if (!chanctx_conf)
			goto fail_rcu;
		band = chanctx_conf->def.chan->band;
		break;
#endif
	case NL80211_IFTYPE_STATION:
		if (sdata->wdev.wiphy->flags & WIPHY_FLAG_SUPPORTS_TDLS) {
			bool tdls_peer = false;

>>>>>>> refs/remotes/origin/master
			sta = sta_info_get(sdata, skb->data);
			if (sta) {
				authorized = test_sta_flag(sta,
							WLAN_STA_AUTHORIZED);
				wme_sta = test_sta_flag(sta, WLAN_STA_WME);
				tdls_peer = test_sta_flag(sta,
							 WLAN_STA_TDLS_PEER);
				tdls_auth = test_sta_flag(sta,
						WLAN_STA_TDLS_PEER_AUTH);
			}
<<<<<<< HEAD
			rcu_read_unlock();
=======
>>>>>>> refs/remotes/origin/master

			/*
			 * If the TDLS link is enabled, send everything
			 * directly. Otherwise, allow TDLS setup frames
			 * to be transmitted indirectly.
			 */
			tdls_direct = tdls_peer && (tdls_auth ||
				 !(ethertype == ETH_P_TDLS && skb->len > 14 &&
				   skb->data[14] == WLAN_TDLS_SNAP_RFTYPE));
		}

		if (tdls_direct) {
			/* link during setup - throw out frames to peer */
<<<<<<< HEAD
			if (!tdls_auth) {
				ret = NETDEV_TX_OK;
				goto fail;
			}
=======
			if (!tdls_auth)
				goto fail_rcu;
>>>>>>> refs/remotes/origin/master

			/* DA SA BSSID */
			memcpy(hdr.addr1, skb->data, ETH_ALEN);
			memcpy(hdr.addr2, skb->data + ETH_ALEN, ETH_ALEN);
			memcpy(hdr.addr3, sdata->u.mgd.bssid, ETH_ALEN);
			hdrlen = 24;
		}  else if (sdata->u.mgd.use_4addr &&
			    cpu_to_be16(ethertype) != sdata->control_port_protocol) {
			fc |= cpu_to_le16(IEEE80211_FCTL_FROMDS |
					  IEEE80211_FCTL_TODS);
			/* RA TA DA SA */
			memcpy(hdr.addr1, sdata->u.mgd.bssid, ETH_ALEN);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			memcpy(hdr.addr2, sdata->vif.addr, ETH_ALEN);
			memcpy(hdr.addr3, skb->data, ETH_ALEN);
			memcpy(hdr.addr4, skb->data + ETH_ALEN, ETH_ALEN);
			hdrlen = 30;
		} else {
			fc |= cpu_to_le16(IEEE80211_FCTL_TODS);
			/* BSSID SA DA */
<<<<<<< HEAD
<<<<<<< HEAD
=======
			memcpy(hdr.addr1, sdata->u.mgd.bssid, ETH_ALEN);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			memcpy(hdr.addr1, sdata->u.mgd.bssid, ETH_ALEN);
>>>>>>> refs/remotes/origin/master
			memcpy(hdr.addr2, skb->data + ETH_ALEN, ETH_ALEN);
			memcpy(hdr.addr3, skb->data, ETH_ALEN);
			hdrlen = 24;
		}
<<<<<<< HEAD
=======
		chanctx_conf = rcu_dereference(sdata->vif.chanctx_conf);
		if (!chanctx_conf)
			goto fail_rcu;
		band = chanctx_conf->def.chan->band;
>>>>>>> refs/remotes/origin/master
		break;
	case NL80211_IFTYPE_ADHOC:
		/* DA SA BSSID */
		memcpy(hdr.addr1, skb->data, ETH_ALEN);
		memcpy(hdr.addr2, skb->data + ETH_ALEN, ETH_ALEN);
		memcpy(hdr.addr3, sdata->u.ibss.bssid, ETH_ALEN);
		hdrlen = 24;
<<<<<<< HEAD
		break;
	default:
		ret = NETDEV_TX_OK;
		goto fail;
=======
		chanctx_conf = rcu_dereference(sdata->vif.chanctx_conf);
		if (!chanctx_conf)
			goto fail_rcu;
		band = chanctx_conf->def.chan->band;
		break;
	default:
		goto fail_rcu;
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * There's no need to try to look up the destination
	 * if it is a multicast address (which can only happen
	 * in AP mode)
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!is_multicast_ether_addr(hdr.addr1)) {
		rcu_read_lock();
		sta = sta_info_get(sdata, hdr.addr1);
		if (sta)
			sta_flags = get_sta_flags(sta);
		rcu_read_unlock();
	}

	/* receiver and we are QoS enabled, use a QoS type frame */
	if ((sta_flags & WLAN_STA_WME) && local->hw.queues >= 4) {
=======
	multicast = is_multicast_ether_addr(hdr.addr1);
	if (!multicast) {
		rcu_read_lock();
=======
	multicast = is_multicast_ether_addr(hdr.addr1);
	if (!multicast) {
>>>>>>> refs/remotes/origin/master
		sta = sta_info_get(sdata, hdr.addr1);
		if (sta) {
			authorized = test_sta_flag(sta, WLAN_STA_AUTHORIZED);
			wme_sta = test_sta_flag(sta, WLAN_STA_WME);
		}
<<<<<<< HEAD
		rcu_read_unlock();
=======
>>>>>>> refs/remotes/origin/master
	}

	/* For mesh, the use of the QoS header is mandatory */
	if (ieee80211_vif_is_mesh(&sdata->vif))
		wme_sta = true;

	/* receiver and we are QoS enabled, use a QoS type frame */
<<<<<<< HEAD
	if (wme_sta && local->hw.queues >= 4) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (wme_sta && local->hw.queues >= IEEE80211_NUM_ACS) {
>>>>>>> refs/remotes/origin/master
		fc |= cpu_to_le16(IEEE80211_STYPE_QOS_DATA);
		hdrlen += 2;
	}

	/*
	 * Drop unicast frames to unauthorised stations unless they are
	 * EAPOL frames from the local station.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!ieee80211_vif_is_mesh(&sdata->vif) &&
		unlikely(!is_multicast_ether_addr(hdr.addr1) &&
		      !(sta_flags & WLAN_STA_AUTHORIZED) &&
		      !(cpu_to_be16(ethertype) == sdata->control_port_protocol &&
		       compare_ether_addr(sdata->vif.addr,
					  skb->data + ETH_ALEN) == 0))) {
=======
	if (unlikely(!ieee80211_vif_is_mesh(&sdata->vif) &&
		     !is_multicast_ether_addr(hdr.addr1) && !authorized &&
		     (cpu_to_be16(ethertype) != sdata->control_port_protocol ||
		      compare_ether_addr(sdata->vif.addr, skb->data + ETH_ALEN)))) {
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
		if (net_ratelimit())
			printk(KERN_DEBUG "%s: dropped frame to %pM"
			       " (unauthorized port)\n", dev->name,
			       hdr.addr1);
=======
	if (unlikely(!ieee80211_vif_is_mesh(&sdata->vif) &&
		     !multicast && !authorized &&
		     (cpu_to_be16(ethertype) != sdata->control_port_protocol ||
		      !ether_addr_equal(sdata->vif.addr, skb->data + ETH_ALEN)))) {
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
		net_info_ratelimited("%s: dropped frame to %pM (unauthorized port)\n",
				    dev->name, hdr.addr1);
>>>>>>> refs/remotes/origin/master
#endif

		I802_DEBUG_INC(local->tx_handlers_drop_unauth_port);

<<<<<<< HEAD
		ret = NETDEV_TX_OK;
		goto fail;
	}

<<<<<<< HEAD
=======
=======
		goto fail_rcu;
	}

>>>>>>> refs/remotes/origin/master
	if (unlikely(!multicast && skb->sk &&
		     skb_shinfo(skb)->tx_flags & SKBTX_WIFI_STATUS)) {
		struct sk_buff *orig_skb = skb;

		skb = skb_clone(skb, GFP_ATOMIC);
		if (skb) {
			unsigned long flags;
<<<<<<< HEAD
			int id, r;

			spin_lock_irqsave(&local->ack_status_lock, flags);
			r = idr_get_new_above(&local->ack_status_frames,
					      orig_skb, 1, &id);
			if (r == -EAGAIN) {
				idr_pre_get(&local->ack_status_frames,
					    GFP_ATOMIC);
				r = idr_get_new_above(&local->ack_status_frames,
						      orig_skb, 1, &id);
			}
			if (WARN_ON(!id) || id > 0xffff) {
				idr_remove(&local->ack_status_frames, id);
				r = -ERANGE;
			}
			spin_unlock_irqrestore(&local->ack_status_lock, flags);

			if (!r) {
=======
			int id;

			spin_lock_irqsave(&local->ack_status_lock, flags);
			id = idr_alloc(&local->ack_status_frames, orig_skb,
				       1, 0x10000, GFP_ATOMIC);
			spin_unlock_irqrestore(&local->ack_status_lock, flags);

			if (id >= 0) {
>>>>>>> refs/remotes/origin/master
				info_id = id;
				info_flags |= IEEE80211_TX_CTL_REQ_TX_STATUS;
			} else if (skb_shared(skb)) {
				kfree_skb(orig_skb);
			} else {
				kfree_skb(skb);
				skb = orig_skb;
			}
		} else {
			/* couldn't clone -- lose tx status ... */
			skb = orig_skb;
		}
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * If the skb is shared we need to obtain our own copy.
	 */
	if (skb_shared(skb)) {
<<<<<<< HEAD
<<<<<<< HEAD
		tmp_skb = skb;
=======
=======
>>>>>>> refs/remotes/origin/master
		struct sk_buff *tmp_skb = skb;

		/* can't happen -- skb is a clone if info_id != 0 */
		WARN_ON(info_id);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		skb = skb_clone(skb, GFP_ATOMIC);
		kfree_skb(tmp_skb);

		if (!skb) {
			ret = NETDEV_TX_OK;
			goto fail;
		}
=======
		skb = skb_clone(skb, GFP_ATOMIC);
		kfree_skb(tmp_skb);

		if (!skb)
			goto fail_rcu;
>>>>>>> refs/remotes/origin/master
	}

	hdr.frame_control = fc;
	hdr.duration_id = 0;
	hdr.seq_ctrl = 0;

	skip_header_bytes = ETH_HLEN;
	if (ethertype == ETH_P_AARP || ethertype == ETH_P_IPX) {
		encaps_data = bridge_tunnel_header;
		encaps_len = sizeof(bridge_tunnel_header);
		skip_header_bytes -= 2;
<<<<<<< HEAD
	} else if (ethertype >= 0x600) {
=======
	} else if (ethertype >= ETH_P_802_3_MIN) {
>>>>>>> refs/remotes/origin/master
		encaps_data = rfc1042_header;
		encaps_len = sizeof(rfc1042_header);
		skip_header_bytes -= 2;
	} else {
		encaps_data = NULL;
		encaps_len = 0;
	}

	nh_pos = skb_network_header(skb) - skb->data;
	h_pos = skb_transport_header(skb) - skb->data;

	skb_pull(skb, skip_header_bytes);
	nh_pos -= skip_header_bytes;
	h_pos -= skip_header_bytes;

	head_need = hdrlen + encaps_len + meshhdrlen - skb_headroom(skb);

	/*
	 * So we need to modify the skb header and hence need a copy of
	 * that. The head_need variable above doesn't, so far, include
	 * the needed header space that we don't need right away. If we
	 * can, then we don't reallocate right now but only after the
	 * frame arrives at the master device (if it does...)
	 *
	 * If we cannot, however, then we will reallocate to include all
	 * the ever needed space. Also, if we need to reallocate it anyway,
	 * make it big enough for everything we may ever need.
	 */

	if (head_need > 0 || skb_cloned(skb)) {
		head_need += IEEE80211_ENCRYPT_HEADROOM;
		head_need += local->tx_headroom;
		head_need = max_t(int, 0, head_need);
<<<<<<< HEAD
<<<<<<< HEAD
		if (ieee80211_skb_resize(local, skb, head_need, true))
=======
		if (ieee80211_skb_resize(sdata, skb, head_need, true))
>>>>>>> refs/remotes/origin/cm-10.0
			goto fail;
=======
		if (ieee80211_skb_resize(sdata, skb, head_need, true)) {
			ieee80211_free_txskb(&local->hw, skb);
			skb = NULL;
			goto fail_rcu;
		}
>>>>>>> refs/remotes/origin/master
	}

	if (encaps_data) {
		memcpy(skb_push(skb, encaps_len), encaps_data, encaps_len);
		nh_pos += encaps_len;
		h_pos += encaps_len;
	}

#ifdef CONFIG_MAC80211_MESH
	if (meshhdrlen > 0) {
		memcpy(skb_push(skb, meshhdrlen), &mesh_hdr, meshhdrlen);
		nh_pos += meshhdrlen;
		h_pos += meshhdrlen;
	}
#endif

	if (ieee80211_is_data_qos(fc)) {
		__le16 *qos_control;

		qos_control = (__le16*) skb_push(skb, 2);
		memcpy(skb_push(skb, hdrlen - 2), &hdr, hdrlen - 2);
		/*
		 * Maybe we could actually set some fields here, for now just
		 * initialise to zero to indicate no special operation.
		 */
		*qos_control = 0;
	} else
		memcpy(skb_push(skb, hdrlen), &hdr, hdrlen);

	nh_pos += hdrlen;
	h_pos += hdrlen;

	dev->stats.tx_packets++;
	dev->stats.tx_bytes += skb->len;

	/* Update skb pointers to various headers since this modified frame
	 * is going to go through Linux networking code that may potentially
	 * need things like pointer to IP header. */
	skb_set_mac_header(skb, 0);
	skb_set_network_header(skb, nh_pos);
	skb_set_transport_header(skb, h_pos);

	info = IEEE80211_SKB_CB(skb);
	memset(info, 0, sizeof(*info));

	dev->trans_start = jiffies;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

	info->flags = info_flags;
	info->ack_frame_id = info_id;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	ieee80211_xmit(sdata, skb);

	return NETDEV_TX_OK;

 fail:
	if (ret == NETDEV_TX_OK)
		dev_kfree_skb(skb);

	return ret;
=======
	ieee80211_xmit(sdata, skb, band);
	rcu_read_unlock();

	return NETDEV_TX_OK;

 fail_rcu:
	rcu_read_unlock();
 fail:
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
>>>>>>> refs/remotes/origin/master
}


/*
 * ieee80211_clear_tx_pending may not be called in a context where
 * it is possible that it packets could come in again.
 */
void ieee80211_clear_tx_pending(struct ieee80211_local *local)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int i;

	for (i = 0; i < local->hw.queues; i++)
		skb_queue_purge(&local->pending[i]);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct sk_buff *skb;
	int i;

	for (i = 0; i < local->hw.queues; i++) {
		while ((skb = skb_dequeue(&local->pending[i])) != NULL)
			ieee80211_free_txskb(&local->hw, skb);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * Returns false if the frame couldn't be transmitted but was queued instead,
 * which in this case means re-queued -- take as an indication to stop sending
 * more pending frames.
 */
static bool ieee80211_tx_pending_skb(struct ieee80211_local *local,
				     struct sk_buff *skb)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct ieee80211_sub_if_data *sdata;
	struct sta_info *sta;
	struct ieee80211_hdr *hdr;
	bool result;
<<<<<<< HEAD
=======
	struct ieee80211_chanctx_conf *chanctx_conf;
>>>>>>> refs/remotes/origin/master

	sdata = vif_to_sdata(info->control.vif);

	if (info->flags & IEEE80211_TX_INTFL_NEED_TXPROCESSING) {
<<<<<<< HEAD
		result = ieee80211_tx(sdata, skb, true);
	} else {
<<<<<<< HEAD
		hdr = (struct ieee80211_hdr *)skb->data;
		sta = sta_info_get(sdata, hdr->addr1);

		result = __ieee80211_tx(local, &skb, sta, true);
=======
=======
		chanctx_conf = rcu_dereference(sdata->vif.chanctx_conf);
		if (unlikely(!chanctx_conf)) {
			dev_kfree_skb(skb);
			return true;
		}
		result = ieee80211_tx(sdata, skb, true,
				      chanctx_conf->def.chan->band);
	} else {
>>>>>>> refs/remotes/origin/master
		struct sk_buff_head skbs;

		__skb_queue_head_init(&skbs);
		__skb_queue_tail(&skbs, skb);

		hdr = (struct ieee80211_hdr *)skb->data;
		sta = sta_info_get(sdata, hdr->addr1);

		result = __ieee80211_tx(local, &skbs, skb->len, sta, true);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	return result;
}

/*
 * Transmit all pending packets. Called from tasklet.
 */
void ieee80211_tx_pending(unsigned long data)
{
	struct ieee80211_local *local = (struct ieee80211_local *)data;
<<<<<<< HEAD
	struct ieee80211_sub_if_data *sdata;
=======
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	int i;
	bool txok;

	rcu_read_lock();

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	for (i = 0; i < local->hw.queues; i++) {
		/*
		 * If queue is stopped by something other than due to pending
		 * frames, or we have no pending frames, proceed to next queue.
		 */
		if (local->queue_stop_reasons[i] ||
		    skb_queue_empty(&local->pending[i]))
			continue;

		while (!skb_queue_empty(&local->pending[i])) {
			struct sk_buff *skb = __skb_dequeue(&local->pending[i]);
			struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);

			if (WARN_ON(!info->control.vif)) {
<<<<<<< HEAD
				kfree_skb(skb);
=======
				ieee80211_free_txskb(&local->hw, skb);
>>>>>>> refs/remotes/origin/master
				continue;
			}

			spin_unlock_irqrestore(&local->queue_stop_reason_lock,
						flags);

			txok = ieee80211_tx_pending_skb(local, skb);
			spin_lock_irqsave(&local->queue_stop_reason_lock,
					  flags);
			if (!txok)
				break;
		}

		if (skb_queue_empty(&local->pending[i]))
<<<<<<< HEAD
			list_for_each_entry_rcu(sdata, &local->interfaces, list)
				netif_wake_subqueue(sdata->dev, i);
=======
			ieee80211_propagate_queue_wake(local, i);
>>>>>>> refs/remotes/origin/master
	}
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);

	rcu_read_unlock();
}

/* functions for drivers to get certain frames */

<<<<<<< HEAD
<<<<<<< HEAD
static void ieee80211_beacon_add_tim(struct ieee80211_if_ap *bss,
=======
static void ieee80211_beacon_add_tim(struct ieee80211_sub_if_data *sdata,
				     struct ieee80211_if_ap *bss,
>>>>>>> refs/remotes/origin/cm-10.0
				     struct sk_buff *skb,
				     struct beacon_data *beacon)
=======
static void __ieee80211_beacon_add_tim(struct ieee80211_sub_if_data *sdata,
				       struct ps_data *ps, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	u8 *pos, *tim;
	int aid0 = 0;
	int i, have_bits = 0, n1, n2;

	/* Generate bitmap for TIM only if there are any STAs in power save
	 * mode. */
<<<<<<< HEAD
	if (atomic_read(&bss->num_sta_ps) > 0)
		/* in the hope that this is faster than
		 * checking byte-for-byte */
		have_bits = !bitmap_empty((unsigned long*)bss->tim,
					  IEEE80211_MAX_AID+1);

	if (bss->dtim_count == 0)
<<<<<<< HEAD
		bss->dtim_count = beacon->dtim_period - 1;
=======
		bss->dtim_count = sdata->vif.bss_conf.dtim_period - 1;
>>>>>>> refs/remotes/origin/cm-10.0
	else
		bss->dtim_count--;
=======
	if (atomic_read(&ps->num_sta_ps) > 0)
		/* in the hope that this is faster than
		 * checking byte-for-byte */
		have_bits = !bitmap_empty((unsigned long*)ps->tim,
					  IEEE80211_MAX_AID+1);

	if (ps->dtim_count == 0)
		ps->dtim_count = sdata->vif.bss_conf.dtim_period - 1;
	else
		ps->dtim_count--;
>>>>>>> refs/remotes/origin/master

	tim = pos = (u8 *) skb_put(skb, 6);
	*pos++ = WLAN_EID_TIM;
	*pos++ = 4;
<<<<<<< HEAD
	*pos++ = bss->dtim_count;
<<<<<<< HEAD
	*pos++ = beacon->dtim_period;
=======
	*pos++ = sdata->vif.bss_conf.dtim_period;
>>>>>>> refs/remotes/origin/cm-10.0

	if (bss->dtim_count == 0 && !skb_queue_empty(&bss->ps_bc_buf))
		aid0 = 1;

	bss->dtim_bc_mc = aid0 == 1;
=======
	*pos++ = ps->dtim_count;
	*pos++ = sdata->vif.bss_conf.dtim_period;

	if (ps->dtim_count == 0 && !skb_queue_empty(&ps->bc_buf))
		aid0 = 1;

	ps->dtim_bc_mc = aid0 == 1;
>>>>>>> refs/remotes/origin/master

	if (have_bits) {
		/* Find largest even number N1 so that bits numbered 1 through
		 * (N1 x 8) - 1 in the bitmap are 0 and number N2 so that bits
		 * (N2 + 1) x 8 through 2007 are 0. */
		n1 = 0;
		for (i = 0; i < IEEE80211_MAX_TIM_LEN; i++) {
<<<<<<< HEAD
			if (bss->tim[i]) {
=======
			if (ps->tim[i]) {
>>>>>>> refs/remotes/origin/master
				n1 = i & 0xfe;
				break;
			}
		}
		n2 = n1;
		for (i = IEEE80211_MAX_TIM_LEN - 1; i >= n1; i--) {
<<<<<<< HEAD
			if (bss->tim[i]) {
=======
			if (ps->tim[i]) {
>>>>>>> refs/remotes/origin/master
				n2 = i;
				break;
			}
		}

		/* Bitmap control */
		*pos++ = n1 | aid0;
		/* Part Virt Bitmap */
<<<<<<< HEAD
<<<<<<< HEAD
		memcpy(pos, bss->tim + n1, n2 - n1 + 1);

		tim[1] = n2 - n1 + 4;
		skb_put(skb, n2 - n1);
=======
		skb_put(skb, n2 - n1);
		memcpy(pos, bss->tim + n1, n2 - n1 + 1);

		tim[1] = n2 - n1 + 4;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		skb_put(skb, n2 - n1);
		memcpy(pos, ps->tim + n1, n2 - n1 + 1);

		tim[1] = n2 - n1 + 4;
>>>>>>> refs/remotes/origin/master
	} else {
		*pos++ = aid0; /* Bitmap control */
		*pos++ = 0; /* Part Virt Bitmap */
	}
}

<<<<<<< HEAD
=======
static int ieee80211_beacon_add_tim(struct ieee80211_sub_if_data *sdata,
				    struct ps_data *ps, struct sk_buff *skb)
{
	struct ieee80211_local *local = sdata->local;

	/*
	 * Not very nice, but we want to allow the driver to call
	 * ieee80211_beacon_get() as a response to the set_tim()
	 * callback. That, however, is already invoked under the
	 * sta_lock to guarantee consistent and race-free update
	 * of the tim bitmap in mac80211 and the driver.
	 */
	if (local->tim_in_locked_section) {
		__ieee80211_beacon_add_tim(sdata, ps, skb);
	} else {
		spin_lock_bh(&local->tim_lock);
		__ieee80211_beacon_add_tim(sdata, ps, skb);
		spin_unlock_bh(&local->tim_lock);
	}

	return 0;
}

void ieee80211_csa_finish(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	ieee80211_queue_work(&sdata->local->hw,
			     &sdata->csa_finalize_work);
}
EXPORT_SYMBOL(ieee80211_csa_finish);

static void ieee80211_update_csa(struct ieee80211_sub_if_data *sdata,
				 struct beacon_data *beacon)
{
	struct probe_resp *resp;
	int counter_offset_beacon = sdata->csa_counter_offset_beacon;
	int counter_offset_presp = sdata->csa_counter_offset_presp;
	u8 *beacon_data;
	size_t beacon_data_len;

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP:
		beacon_data = beacon->tail;
		beacon_data_len = beacon->tail_len;
		break;
	case NL80211_IFTYPE_ADHOC:
		beacon_data = beacon->head;
		beacon_data_len = beacon->head_len;
		break;
	case NL80211_IFTYPE_MESH_POINT:
		beacon_data = beacon->head;
		beacon_data_len = beacon->head_len;
		break;
	default:
		return;
	}
	if (WARN_ON(counter_offset_beacon >= beacon_data_len))
		return;

	/* warn if the driver did not check for/react to csa completeness */
	if (WARN_ON(beacon_data[counter_offset_beacon] == 0))
		return;

	beacon_data[counter_offset_beacon]--;

	if (sdata->vif.type == NL80211_IFTYPE_AP && counter_offset_presp) {
		rcu_read_lock();
		resp = rcu_dereference(sdata->u.ap.probe_resp);

		/* if nl80211 accepted the offset, this should not happen. */
		if (WARN_ON(!resp)) {
			rcu_read_unlock();
			return;
		}
		resp->data[counter_offset_presp]--;
		rcu_read_unlock();
	}
}

bool ieee80211_csa_is_complete(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct beacon_data *beacon = NULL;
	u8 *beacon_data;
	size_t beacon_data_len;
	int counter_beacon = sdata->csa_counter_offset_beacon;
	int ret = false;

	if (!ieee80211_sdata_running(sdata))
		return false;

	rcu_read_lock();
	if (vif->type == NL80211_IFTYPE_AP) {
		struct ieee80211_if_ap *ap = &sdata->u.ap;

		beacon = rcu_dereference(ap->beacon);
		if (WARN_ON(!beacon || !beacon->tail))
			goto out;
		beacon_data = beacon->tail;
		beacon_data_len = beacon->tail_len;
	} else if (vif->type == NL80211_IFTYPE_ADHOC) {
		struct ieee80211_if_ibss *ifibss = &sdata->u.ibss;

		beacon = rcu_dereference(ifibss->presp);
		if (!beacon)
			goto out;

		beacon_data = beacon->head;
		beacon_data_len = beacon->head_len;
	} else if (vif->type == NL80211_IFTYPE_MESH_POINT) {
		struct ieee80211_if_mesh *ifmsh = &sdata->u.mesh;

		beacon = rcu_dereference(ifmsh->beacon);
		if (!beacon)
			goto out;

		beacon_data = beacon->head;
		beacon_data_len = beacon->head_len;
	} else {
		WARN_ON(1);
		goto out;
	}

	if (WARN_ON(counter_beacon > beacon_data_len))
		goto out;

	if (beacon_data[counter_beacon] == 0)
		ret = true;
 out:
	rcu_read_unlock();

	return ret;
}
EXPORT_SYMBOL(ieee80211_csa_is_complete);

>>>>>>> refs/remotes/origin/master
struct sk_buff *ieee80211_beacon_get_tim(struct ieee80211_hw *hw,
					 struct ieee80211_vif *vif,
					 u16 *tim_offset, u16 *tim_length)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct sk_buff *skb = NULL;
	struct ieee80211_tx_info *info;
	struct ieee80211_sub_if_data *sdata = NULL;
<<<<<<< HEAD
	struct ieee80211_if_ap *ap = NULL;
	struct beacon_data *beacon;
	struct ieee80211_supported_band *sband;
	enum ieee80211_band band = local->hw.conf.channel->band;
	struct ieee80211_tx_rate_control txrc;

	sband = local->hw.wiphy->bands[band];
=======
	enum ieee80211_band band;
	struct ieee80211_tx_rate_control txrc;
	struct ieee80211_chanctx_conf *chanctx_conf;
>>>>>>> refs/remotes/origin/master

	rcu_read_lock();

	sdata = vif_to_sdata(vif);
<<<<<<< HEAD

	if (!ieee80211_sdata_running(sdata))
=======
	chanctx_conf = rcu_dereference(sdata->vif.chanctx_conf);

	if (!ieee80211_sdata_running(sdata) || !chanctx_conf)
>>>>>>> refs/remotes/origin/master
		goto out;

	if (tim_offset)
		*tim_offset = 0;
	if (tim_length)
		*tim_length = 0;

	if (sdata->vif.type == NL80211_IFTYPE_AP) {
<<<<<<< HEAD
		ap = &sdata->u.ap;
		beacon = rcu_dereference(ap->beacon);
		if (beacon) {
=======
		struct ieee80211_if_ap *ap = &sdata->u.ap;
		struct beacon_data *beacon = rcu_dereference(ap->beacon);

		if (beacon) {
			if (sdata->vif.csa_active)
				ieee80211_update_csa(sdata, beacon);

>>>>>>> refs/remotes/origin/master
			/*
			 * headroom, head length,
			 * tail length and maximum TIM length
			 */
			skb = dev_alloc_skb(local->tx_headroom +
					    beacon->head_len +
					    beacon->tail_len + 256);
			if (!skb)
				goto out;

			skb_reserve(skb, local->tx_headroom);
			memcpy(skb_put(skb, beacon->head_len), beacon->head,
			       beacon->head_len);

<<<<<<< HEAD
			/*
			 * Not very nice, but we want to allow the driver to call
			 * ieee80211_beacon_get() as a response to the set_tim()
			 * callback. That, however, is already invoked under the
			 * sta_lock to guarantee consistent and race-free update
			 * of the tim bitmap in mac80211 and the driver.
			 */
			if (local->tim_in_locked_section) {
<<<<<<< HEAD
				ieee80211_beacon_add_tim(ap, skb, beacon);
			} else {
				unsigned long flags;

				spin_lock_irqsave(&local->sta_lock, flags);
				ieee80211_beacon_add_tim(ap, skb, beacon);
				spin_unlock_irqrestore(&local->sta_lock, flags);
=======
				ieee80211_beacon_add_tim(sdata, ap, skb,
							 beacon);
			} else {
				unsigned long flags;

				spin_lock_irqsave(&local->tim_lock, flags);
				ieee80211_beacon_add_tim(sdata, ap, skb,
							 beacon);
				spin_unlock_irqrestore(&local->tim_lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
			}
=======
			ieee80211_beacon_add_tim(sdata, &ap->ps, skb);
>>>>>>> refs/remotes/origin/master

			if (tim_offset)
				*tim_offset = beacon->head_len;
			if (tim_length)
				*tim_length = skb->len - beacon->head_len;

			if (beacon->tail)
				memcpy(skb_put(skb, beacon->tail_len),
				       beacon->tail, beacon->tail_len);
		} else
			goto out;
	} else if (sdata->vif.type == NL80211_IFTYPE_ADHOC) {
		struct ieee80211_if_ibss *ifibss = &sdata->u.ibss;
		struct ieee80211_hdr *hdr;
<<<<<<< HEAD
		struct sk_buff *presp = rcu_dereference(ifibss->presp);
=======
		struct beacon_data *presp = rcu_dereference(ifibss->presp);
>>>>>>> refs/remotes/origin/master

		if (!presp)
			goto out;

<<<<<<< HEAD
		skb = skb_copy(presp, GFP_ATOMIC);
		if (!skb)
			goto out;
=======
		if (sdata->vif.csa_active)
			ieee80211_update_csa(sdata, presp);


		skb = dev_alloc_skb(local->tx_headroom + presp->head_len);
		if (!skb)
			goto out;
		skb_reserve(skb, local->tx_headroom);
		memcpy(skb_put(skb, presp->head_len), presp->head,
		       presp->head_len);
>>>>>>> refs/remotes/origin/master

		hdr = (struct ieee80211_hdr *) skb->data;
		hdr->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
						 IEEE80211_STYPE_BEACON);
	} else if (ieee80211_vif_is_mesh(&sdata->vif)) {
<<<<<<< HEAD
		struct ieee80211_mgmt *mgmt;
		u8 *pos;
<<<<<<< HEAD
=======
		int hdr_len = offsetof(struct ieee80211_mgmt, u.beacon) +
			      sizeof(mgmt->u.beacon);
>>>>>>> refs/remotes/origin/cm-10.0

#ifdef CONFIG_MAC80211_MESH
		if (!sdata->u.mesh.mesh_id_len)
			goto out;
#endif

<<<<<<< HEAD
		/* headroom, head length, tail length and maximum TIM length */
		skb = dev_alloc_skb(local->tx_headroom + 400 +
				sdata->u.mesh.ie_len);
=======
		skb = dev_alloc_skb(local->tx_headroom +
				    hdr_len +
				    2 + /* NULL SSID */
				    2 + 8 + /* supported rates */
				    2 + 3 + /* DS params */
				    2 + (IEEE80211_MAX_SUPP_RATES - 8) +
				    2 + sizeof(struct ieee80211_ht_cap) +
				    2 + sizeof(struct ieee80211_ht_info) +
				    2 + sdata->u.mesh.mesh_id_len +
				    2 + sizeof(struct ieee80211_meshconf_ie) +
				    sdata->u.mesh.ie_len);
>>>>>>> refs/remotes/origin/cm-10.0
		if (!skb)
			goto out;

		skb_reserve(skb, local->hw.extra_tx_headroom);
<<<<<<< HEAD
		mgmt = (struct ieee80211_mgmt *)
			skb_put(skb, 24 + sizeof(mgmt->u.beacon));
		memset(mgmt, 0, 24 + sizeof(mgmt->u.beacon));
=======
		mgmt = (struct ieee80211_mgmt *) skb_put(skb, hdr_len);
		memset(mgmt, 0, hdr_len);
>>>>>>> refs/remotes/origin/cm-10.0
		mgmt->frame_control =
		    cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_BEACON);
		memset(mgmt->da, 0xff, ETH_ALEN);
		memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
		memcpy(mgmt->bssid, sdata->vif.addr, ETH_ALEN);
		mgmt->u.beacon.beacon_int =
			cpu_to_le16(sdata->vif.bss_conf.beacon_int);
<<<<<<< HEAD
		mgmt->u.beacon.capab_info = 0x0; /* 0x0 for MPs */
=======
		mgmt->u.beacon.capab_info |= cpu_to_le16(
			sdata->u.mesh.security ? WLAN_CAPABILITY_PRIVACY : 0);
>>>>>>> refs/remotes/origin/cm-10.0

		pos = skb_put(skb, 2);
		*pos++ = WLAN_EID_SSID;
		*pos++ = 0x0;

<<<<<<< HEAD
		mesh_mgmt_ies_add(skb, sdata);
=======
		if (ieee80211_add_srates_ie(&sdata->vif, skb) ||
		    mesh_add_ds_params_ie(skb, sdata) ||
		    ieee80211_add_ext_srates_ie(&sdata->vif, skb) ||
		    mesh_add_rsn_ie(skb, sdata) ||
		    mesh_add_ht_cap_ie(skb, sdata) ||
		    mesh_add_ht_info_ie(skb, sdata) ||
		    mesh_add_meshid_ie(skb, sdata) ||
		    mesh_add_meshconf_ie(skb, sdata) ||
		    mesh_add_vendor_ies(skb, sdata)) {
			pr_err("o11s: couldn't add ies!\n");
			goto out;
		}
>>>>>>> refs/remotes/origin/cm-10.0
=======
		struct ieee80211_if_mesh *ifmsh = &sdata->u.mesh;
		struct beacon_data *bcn = rcu_dereference(ifmsh->beacon);

		if (!bcn)
			goto out;

		if (sdata->vif.csa_active)
			ieee80211_update_csa(sdata, bcn);

		if (ifmsh->sync_ops)
			ifmsh->sync_ops->adjust_tbtt(
						sdata);

		skb = dev_alloc_skb(local->tx_headroom +
				    bcn->head_len +
				    256 + /* TIM IE */
				    bcn->tail_len);
		if (!skb)
			goto out;
		skb_reserve(skb, local->tx_headroom);
		memcpy(skb_put(skb, bcn->head_len), bcn->head, bcn->head_len);
		ieee80211_beacon_add_tim(sdata, &ifmsh->ps, skb);
		memcpy(skb_put(skb, bcn->tail_len), bcn->tail, bcn->tail_len);
>>>>>>> refs/remotes/origin/master
	} else {
		WARN_ON(1);
		goto out;
	}

<<<<<<< HEAD
=======
	band = chanctx_conf->def.chan->band;

>>>>>>> refs/remotes/origin/master
	info = IEEE80211_SKB_CB(skb);

	info->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	info->flags |= IEEE80211_TX_CTL_NO_ACK;
	info->band = band;

	memset(&txrc, 0, sizeof(txrc));
	txrc.hw = hw;
<<<<<<< HEAD
	txrc.sband = sband;
=======
	txrc.sband = local->hw.wiphy->bands[band];
>>>>>>> refs/remotes/origin/master
	txrc.bss_conf = &sdata->vif.bss_conf;
	txrc.skb = skb;
	txrc.reported_rate.idx = -1;
	txrc.rate_idx_mask = sdata->rc_rateidx_mask[band];
<<<<<<< HEAD
	if (txrc.rate_idx_mask == (1 << sband->n_bitrates) - 1)
		txrc.max_rate_idx = -1;
	else
		txrc.max_rate_idx = fls(txrc.rate_idx_mask) - 1;
<<<<<<< HEAD
=======
	memcpy(txrc.rate_idx_mcs_mask, sdata->rc_rateidx_mcs_mask[band],
	       sizeof(txrc.rate_idx_mcs_mask));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (txrc.rate_idx_mask == (1 << txrc.sband->n_bitrates) - 1)
		txrc.max_rate_idx = -1;
	else
		txrc.max_rate_idx = fls(txrc.rate_idx_mask) - 1;
>>>>>>> refs/remotes/origin/master
	txrc.bss = true;
	rate_control_get_rate(sdata, NULL, &txrc);

	info->control.vif = vif;

	info->flags |= IEEE80211_TX_CTL_CLEAR_PS_FILT |
			IEEE80211_TX_CTL_ASSIGN_SEQ |
			IEEE80211_TX_CTL_FIRST_FRAGMENT;
 out:
	rcu_read_unlock();
	return skb;
}
EXPORT_SYMBOL(ieee80211_beacon_get_tim);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
struct sk_buff *ieee80211_proberesp_get(struct ieee80211_hw *hw,
					struct ieee80211_vif *vif)
{
	struct ieee80211_if_ap *ap = NULL;
<<<<<<< HEAD
	struct sk_buff *presp = NULL, *skb = NULL;
=======
	struct sk_buff *skb = NULL;
	struct probe_resp *presp = NULL;
>>>>>>> refs/remotes/origin/master
	struct ieee80211_hdr *hdr;
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	if (sdata->vif.type != NL80211_IFTYPE_AP)
		return NULL;

	rcu_read_lock();

	ap = &sdata->u.ap;
	presp = rcu_dereference(ap->probe_resp);
	if (!presp)
		goto out;

<<<<<<< HEAD
	skb = skb_copy(presp, GFP_ATOMIC);
	if (!skb)
		goto out;

=======
	skb = dev_alloc_skb(presp->len);
	if (!skb)
		goto out;

	memcpy(skb_put(skb, presp->len), presp->data, presp->len);

>>>>>>> refs/remotes/origin/master
	hdr = (struct ieee80211_hdr *) skb->data;
	memset(hdr->addr1, 0, sizeof(hdr->addr1));

out:
	rcu_read_unlock();
	return skb;
}
EXPORT_SYMBOL(ieee80211_proberesp_get);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct sk_buff *ieee80211_pspoll_get(struct ieee80211_hw *hw,
				     struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_if_managed *ifmgd;
	struct ieee80211_pspoll *pspoll;
	struct ieee80211_local *local;
	struct sk_buff *skb;

	if (WARN_ON(vif->type != NL80211_IFTYPE_STATION))
		return NULL;

	sdata = vif_to_sdata(vif);
	ifmgd = &sdata->u.mgd;
	local = sdata->local;

	skb = dev_alloc_skb(local->hw.extra_tx_headroom + sizeof(*pspoll));
<<<<<<< HEAD
<<<<<<< HEAD
	if (!skb) {
		printk(KERN_DEBUG "%s: failed to allocate buffer for "
		       "pspoll template\n", sdata->name);
		return NULL;
	}
=======
	if (!skb)
		return NULL;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!skb)
		return NULL;

>>>>>>> refs/remotes/origin/master
	skb_reserve(skb, local->hw.extra_tx_headroom);

	pspoll = (struct ieee80211_pspoll *) skb_put(skb, sizeof(*pspoll));
	memset(pspoll, 0, sizeof(*pspoll));
	pspoll->frame_control = cpu_to_le16(IEEE80211_FTYPE_CTL |
					    IEEE80211_STYPE_PSPOLL);
	pspoll->aid = cpu_to_le16(ifmgd->aid);

	/* aid in PS-Poll has its two MSBs each set to 1 */
	pspoll->aid |= cpu_to_le16(1 << 15 | 1 << 14);

	memcpy(pspoll->bssid, ifmgd->bssid, ETH_ALEN);
	memcpy(pspoll->ta, vif->addr, ETH_ALEN);

	return skb;
}
EXPORT_SYMBOL(ieee80211_pspoll_get);

struct sk_buff *ieee80211_nullfunc_get(struct ieee80211_hw *hw,
				       struct ieee80211_vif *vif)
{
	struct ieee80211_hdr_3addr *nullfunc;
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_if_managed *ifmgd;
	struct ieee80211_local *local;
	struct sk_buff *skb;

	if (WARN_ON(vif->type != NL80211_IFTYPE_STATION))
		return NULL;

	sdata = vif_to_sdata(vif);
	ifmgd = &sdata->u.mgd;
	local = sdata->local;

	skb = dev_alloc_skb(local->hw.extra_tx_headroom + sizeof(*nullfunc));
<<<<<<< HEAD
<<<<<<< HEAD
	if (!skb) {
		printk(KERN_DEBUG "%s: failed to allocate buffer for nullfunc "
		       "template\n", sdata->name);
		return NULL;
	}
=======
	if (!skb)
		return NULL;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!skb)
		return NULL;

>>>>>>> refs/remotes/origin/master
	skb_reserve(skb, local->hw.extra_tx_headroom);

	nullfunc = (struct ieee80211_hdr_3addr *) skb_put(skb,
							  sizeof(*nullfunc));
	memset(nullfunc, 0, sizeof(*nullfunc));
	nullfunc->frame_control = cpu_to_le16(IEEE80211_FTYPE_DATA |
					      IEEE80211_STYPE_NULLFUNC |
					      IEEE80211_FCTL_TODS);
	memcpy(nullfunc->addr1, ifmgd->bssid, ETH_ALEN);
	memcpy(nullfunc->addr2, vif->addr, ETH_ALEN);
	memcpy(nullfunc->addr3, ifmgd->bssid, ETH_ALEN);

	return skb;
}
EXPORT_SYMBOL(ieee80211_nullfunc_get);

struct sk_buff *ieee80211_probereq_get(struct ieee80211_hw *hw,
				       struct ieee80211_vif *vif,
				       const u8 *ssid, size_t ssid_len,
<<<<<<< HEAD
				       const u8 *ie, size_t ie_len)
=======
				       size_t tailroom)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_local *local;
	struct ieee80211_hdr_3addr *hdr;
	struct sk_buff *skb;
	size_t ie_ssid_len;
	u8 *pos;

	sdata = vif_to_sdata(vif);
	local = sdata->local;
	ie_ssid_len = 2 + ssid_len;

	skb = dev_alloc_skb(local->hw.extra_tx_headroom + sizeof(*hdr) +
<<<<<<< HEAD
			    ie_ssid_len + ie_len);
<<<<<<< HEAD
	if (!skb) {
		printk(KERN_DEBUG "%s: failed to allocate buffer for probe "
		       "request template\n", sdata->name);
		return NULL;
	}
=======
	if (!skb)
		return NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			    ie_ssid_len + tailroom);
	if (!skb)
		return NULL;
>>>>>>> refs/remotes/origin/master

	skb_reserve(skb, local->hw.extra_tx_headroom);

	hdr = (struct ieee80211_hdr_3addr *) skb_put(skb, sizeof(*hdr));
	memset(hdr, 0, sizeof(*hdr));
	hdr->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
					 IEEE80211_STYPE_PROBE_REQ);
<<<<<<< HEAD
	memset(hdr->addr1, 0xff, ETH_ALEN);
	memcpy(hdr->addr2, vif->addr, ETH_ALEN);
	memset(hdr->addr3, 0xff, ETH_ALEN);
=======
	eth_broadcast_addr(hdr->addr1);
	memcpy(hdr->addr2, vif->addr, ETH_ALEN);
	eth_broadcast_addr(hdr->addr3);
>>>>>>> refs/remotes/origin/master

	pos = skb_put(skb, ie_ssid_len);
	*pos++ = WLAN_EID_SSID;
	*pos++ = ssid_len;
<<<<<<< HEAD
	if (ssid)
		memcpy(pos, ssid, ssid_len);
	pos += ssid_len;

	if (ie) {
		pos = skb_put(skb, ie_len);
		memcpy(pos, ie, ie_len);
	}

=======
	if (ssid_len)
		memcpy(pos, ssid, ssid_len);
	pos += ssid_len;

>>>>>>> refs/remotes/origin/master
	return skb;
}
EXPORT_SYMBOL(ieee80211_probereq_get);

void ieee80211_rts_get(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
		       const void *frame, size_t frame_len,
		       const struct ieee80211_tx_info *frame_txctl,
		       struct ieee80211_rts *rts)
{
	const struct ieee80211_hdr *hdr = frame;

	rts->frame_control =
	    cpu_to_le16(IEEE80211_FTYPE_CTL | IEEE80211_STYPE_RTS);
	rts->duration = ieee80211_rts_duration(hw, vif, frame_len,
					       frame_txctl);
	memcpy(rts->ra, hdr->addr1, sizeof(rts->ra));
	memcpy(rts->ta, hdr->addr2, sizeof(rts->ta));
}
EXPORT_SYMBOL(ieee80211_rts_get);

void ieee80211_ctstoself_get(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			     const void *frame, size_t frame_len,
			     const struct ieee80211_tx_info *frame_txctl,
			     struct ieee80211_cts *cts)
{
	const struct ieee80211_hdr *hdr = frame;

	cts->frame_control =
	    cpu_to_le16(IEEE80211_FTYPE_CTL | IEEE80211_STYPE_CTS);
	cts->duration = ieee80211_ctstoself_duration(hw, vif,
						     frame_len, frame_txctl);
	memcpy(cts->ra, hdr->addr1, sizeof(cts->ra));
}
EXPORT_SYMBOL(ieee80211_ctstoself_get);

struct sk_buff *
ieee80211_get_buffered_bc(struct ieee80211_hw *hw,
			  struct ieee80211_vif *vif)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct sk_buff *skb = NULL;
	struct ieee80211_tx_data tx;
	struct ieee80211_sub_if_data *sdata;
<<<<<<< HEAD
	struct ieee80211_if_ap *bss = NULL;
	struct beacon_data *beacon;
	struct ieee80211_tx_info *info;

	sdata = vif_to_sdata(vif);
	bss = &sdata->u.ap;

	rcu_read_lock();
	beacon = rcu_dereference(bss->beacon);

	if (sdata->vif.type != NL80211_IFTYPE_AP || !beacon || !beacon->head)
		goto out;

	if (bss->dtim_count != 0 || !bss->dtim_bc_mc)
		goto out; /* send buffered bc/mc only after DTIM beacon */

	while (1) {
		skb = skb_dequeue(&bss->ps_bc_buf);
=======
	struct ps_data *ps;
	struct ieee80211_tx_info *info;
	struct ieee80211_chanctx_conf *chanctx_conf;

	sdata = vif_to_sdata(vif);

	rcu_read_lock();
	chanctx_conf = rcu_dereference(sdata->vif.chanctx_conf);

	if (!chanctx_conf)
		goto out;

	if (sdata->vif.type == NL80211_IFTYPE_AP) {
		struct beacon_data *beacon =
				rcu_dereference(sdata->u.ap.beacon);

		if (!beacon || !beacon->head)
			goto out;

		ps = &sdata->u.ap.ps;
	} else if (ieee80211_vif_is_mesh(&sdata->vif)) {
		ps = &sdata->u.mesh.ps;
	} else {
		goto out;
	}

	if (ps->dtim_count != 0 || !ps->dtim_bc_mc)
		goto out; /* send buffered bc/mc only after DTIM beacon */

	while (1) {
		skb = skb_dequeue(&ps->bc_buf);
>>>>>>> refs/remotes/origin/master
		if (!skb)
			goto out;
		local->total_ps_buffered--;

<<<<<<< HEAD
		if (!skb_queue_empty(&bss->ps_bc_buf) && skb->len >= 2) {
=======
		if (!skb_queue_empty(&ps->bc_buf) && skb->len >= 2) {
>>>>>>> refs/remotes/origin/master
			struct ieee80211_hdr *hdr =
				(struct ieee80211_hdr *) skb->data;
			/* more buffered multicast/broadcast frames ==> set
			 * MoreData flag in IEEE 802.11 header to inform PS
			 * STAs */
			hdr->frame_control |=
				cpu_to_le16(IEEE80211_FCTL_MOREDATA);
		}

<<<<<<< HEAD
=======
		if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
			sdata = IEEE80211_DEV_TO_SUB_IF(skb->dev);
>>>>>>> refs/remotes/origin/master
		if (!ieee80211_tx_prepare(sdata, &tx, skb))
			break;
		dev_kfree_skb_any(skb);
	}

	info = IEEE80211_SKB_CB(skb);

	tx.flags |= IEEE80211_TX_PS_BUFFERED;
<<<<<<< HEAD
	tx.channel = local->hw.conf.channel;
	info->band = tx.channel->band;
=======
	info->band = chanctx_conf->def.chan->band;
>>>>>>> refs/remotes/origin/master

	if (invoke_tx_handlers(&tx))
		skb = NULL;
 out:
	rcu_read_unlock();

	return skb;
}
EXPORT_SYMBOL(ieee80211_get_buffered_bc);

<<<<<<< HEAD
<<<<<<< HEAD
void ieee80211_tx_skb(struct ieee80211_sub_if_data *sdata, struct sk_buff *skb)
=======
void ieee80211_tx_skb_tid(struct ieee80211_sub_if_data *sdata,
			  struct sk_buff *skb, int tid)
>>>>>>> refs/remotes/origin/cm-10.0
{
=======
void __ieee80211_tx_skb_tid_band(struct ieee80211_sub_if_data *sdata,
				 struct sk_buff *skb, int tid,
				 enum ieee80211_band band)
{
	int ac = ieee802_1d_to_ac[tid & 7];

>>>>>>> refs/remotes/origin/master
	skb_set_mac_header(skb, 0);
	skb_set_network_header(skb, 0);
	skb_set_transport_header(skb, 0);

<<<<<<< HEAD
<<<<<<< HEAD
	/* Send all internal mgmt frames on VO. Accordingly set TID to 7. */
	skb_set_queue_mapping(skb, IEEE80211_AC_VO);
	skb->priority = 7;
=======
	skb_set_queue_mapping(skb, ieee802_1d_to_ac[tid]);
	skb->priority = tid;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	skb_set_queue_mapping(skb, ac);
	skb->priority = tid;

	skb->dev = sdata->dev;
>>>>>>> refs/remotes/origin/master

	/*
	 * The other path calling ieee80211_xmit is from the tasklet,
	 * and while we can handle concurrent transmissions locking
	 * requirements are that we do not come into tx with bhs on.
	 */
	local_bh_disable();
<<<<<<< HEAD
	ieee80211_xmit(sdata, skb);
=======
	ieee80211_xmit(sdata, skb, band);
>>>>>>> refs/remotes/origin/master
	local_bh_enable();
}
