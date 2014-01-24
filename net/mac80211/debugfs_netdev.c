/*
 * Copyright (c) 2006	Jiri Benc <jbenc@suse.cz>
 * Copyright 2007	Johannes Berg <johannes@sipsolutions.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/if.h>
<<<<<<< HEAD
=======
#include <linux/if_ether.h>
>>>>>>> refs/remotes/origin/master
#include <linux/interrupt.h>
#include <linux/netdevice.h>
#include <linux/rtnetlink.h>
#include <linux/slab.h>
#include <linux/notifier.h>
#include <net/mac80211.h>
#include <net/cfg80211.h>
#include "ieee80211_i.h"
#include "rate.h"
#include "debugfs.h"
#include "debugfs_netdev.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "driver-ops.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "driver-ops.h"
>>>>>>> refs/remotes/origin/master

static ssize_t ieee80211_if_read(
	struct ieee80211_sub_if_data *sdata,
	char __user *userbuf,
	size_t count, loff_t *ppos,
	ssize_t (*format)(const struct ieee80211_sub_if_data *, char *, int))
{
	char buf[70];
	ssize_t ret = -EINVAL;

	read_lock(&dev_base_lock);
	if (sdata->dev->reg_state == NETREG_REGISTERED)
		ret = (*format)(sdata, buf, sizeof(buf));
	read_unlock(&dev_base_lock);

	if (ret >= 0)
		ret = simple_read_from_buffer(userbuf, count, ppos, buf, ret);

	return ret;
}

static ssize_t ieee80211_if_write(
	struct ieee80211_sub_if_data *sdata,
	const char __user *userbuf,
	size_t count, loff_t *ppos,
	ssize_t (*write)(struct ieee80211_sub_if_data *, const char *, int))
{
<<<<<<< HEAD
<<<<<<< HEAD
	u8 *buf;
	ssize_t ret;

	buf = kmalloc(count, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	ret = -EFAULT;
	if (copy_from_user(buf, userbuf, count))
		goto freebuf;
=======
=======
>>>>>>> refs/remotes/origin/master
	char buf[64];
	ssize_t ret;

	if (count >= sizeof(buf))
		return -E2BIG;

	if (copy_from_user(buf, userbuf, count))
		return -EFAULT;
	buf[count] = '\0';
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	ret = -ENODEV;
	rtnl_lock();
	if (sdata->dev->reg_state == NETREG_REGISTERED)
		ret = (*write)(sdata, buf, count);
	rtnl_unlock();

<<<<<<< HEAD
<<<<<<< HEAD
freebuf:
	kfree(buf);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return ret;
}

#define IEEE80211_IF_FMT(name, field, format_string)			\
static ssize_t ieee80211_if_fmt_##name(					\
	const struct ieee80211_sub_if_data *sdata, char *buf,		\
	int buflen)							\
{									\
	return scnprintf(buf, buflen, format_string, sdata->field);	\
}
#define IEEE80211_IF_FMT_DEC(name, field)				\
		IEEE80211_IF_FMT(name, field, "%d\n")
#define IEEE80211_IF_FMT_HEX(name, field)				\
		IEEE80211_IF_FMT(name, field, "%#x\n")
#define IEEE80211_IF_FMT_LHEX(name, field)				\
		IEEE80211_IF_FMT(name, field, "%#lx\n")
#define IEEE80211_IF_FMT_SIZE(name, field)				\
		IEEE80211_IF_FMT(name, field, "%zd\n")

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#define IEEE80211_IF_FMT_HEXARRAY(name, field)				\
static ssize_t ieee80211_if_fmt_##name(					\
	const struct ieee80211_sub_if_data *sdata,			\
	char *buf, int buflen)						\
{									\
	char *p = buf;							\
	int i;								\
	for (i = 0; i < sizeof(sdata->field); i++) {			\
		p += scnprintf(p, buflen + buf - p, "%.2x ",		\
				 sdata->field[i]);			\
	}								\
	p += scnprintf(p, buflen + buf - p, "\n");			\
	return p - buf;							\
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define IEEE80211_IF_FMT_ATOMIC(name, field)				\
static ssize_t ieee80211_if_fmt_##name(					\
	const struct ieee80211_sub_if_data *sdata,			\
	char *buf, int buflen)						\
{									\
	return scnprintf(buf, buflen, "%d\n", atomic_read(&sdata->field));\
}

#define IEEE80211_IF_FMT_MAC(name, field)				\
static ssize_t ieee80211_if_fmt_##name(					\
	const struct ieee80211_sub_if_data *sdata, char *buf,		\
	int buflen)							\
{									\
	return scnprintf(buf, buflen, "%pM\n", sdata->field);		\
}

#define IEEE80211_IF_FMT_DEC_DIV_16(name, field)			\
static ssize_t ieee80211_if_fmt_##name(					\
	const struct ieee80211_sub_if_data *sdata,			\
	char *buf, int buflen)						\
{									\
	return scnprintf(buf, buflen, "%d\n", sdata->field / 16);	\
}

<<<<<<< HEAD
=======
#define IEEE80211_IF_FMT_JIFFIES_TO_MS(name, field)			\
static ssize_t ieee80211_if_fmt_##name(					\
	const struct ieee80211_sub_if_data *sdata,			\
	char *buf, int buflen)						\
{									\
	return scnprintf(buf, buflen, "%d\n",				\
			 jiffies_to_msecs(sdata->field));		\
}

>>>>>>> refs/remotes/origin/master
#define __IEEE80211_IF_FILE(name, _write)				\
static ssize_t ieee80211_if_read_##name(struct file *file,		\
					char __user *userbuf,		\
					size_t count, loff_t *ppos)	\
{									\
	return ieee80211_if_read(file->private_data,			\
				 userbuf, count, ppos,			\
				 ieee80211_if_fmt_##name);		\
}									\
static const struct file_operations name##_ops = {			\
	.read = ieee80211_if_read_##name,				\
	.write = (_write),						\
<<<<<<< HEAD
<<<<<<< HEAD
	.open = mac80211_open_file_generic,				\
=======
	.open = simple_open,						\
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.open = simple_open,						\
>>>>>>> refs/remotes/origin/master
	.llseek = generic_file_llseek,					\
}

#define __IEEE80211_IF_FILE_W(name)					\
static ssize_t ieee80211_if_write_##name(struct file *file,		\
					 const char __user *userbuf,	\
					 size_t count, loff_t *ppos)	\
{									\
	return ieee80211_if_write(file->private_data, userbuf, count,	\
				  ppos, ieee80211_if_parse_##name);	\
}									\
__IEEE80211_IF_FILE(name, ieee80211_if_write_##name)


#define IEEE80211_IF_FILE(name, field, format)				\
		IEEE80211_IF_FMT_##format(name, field)			\
		__IEEE80211_IF_FILE(name, NULL)

/* common attributes */
IEEE80211_IF_FILE(drop_unencrypted, drop_unencrypted, DEC);
IEEE80211_IF_FILE(rc_rateidx_mask_2ghz, rc_rateidx_mask[IEEE80211_BAND_2GHZ],
		  HEX);
IEEE80211_IF_FILE(rc_rateidx_mask_5ghz, rc_rateidx_mask[IEEE80211_BAND_5GHZ],
		  HEX);
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
IEEE80211_IF_FILE(rc_rateidx_mcs_mask_2ghz,
		  rc_rateidx_mcs_mask[IEEE80211_BAND_2GHZ], HEXARRAY);
IEEE80211_IF_FILE(rc_rateidx_mcs_mask_5ghz,
		  rc_rateidx_mcs_mask[IEEE80211_BAND_5GHZ], HEXARRAY);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
IEEE80211_IF_FILE(flags, flags, HEX);
IEEE80211_IF_FILE(state, state, LHEX);
IEEE80211_IF_FILE(channel_type, vif.bss_conf.channel_type, DEC);
=======
IEEE80211_IF_FILE(flags, flags, HEX);
IEEE80211_IF_FILE(state, state, LHEX);
IEEE80211_IF_FILE(txpower, vif.bss_conf.txpower, DEC);
IEEE80211_IF_FILE(ap_power_level, ap_power_level, DEC);
IEEE80211_IF_FILE(user_power_level, user_power_level, DEC);

static ssize_t
ieee80211_if_fmt_hw_queues(const struct ieee80211_sub_if_data *sdata,
			   char *buf, int buflen)
{
	int len;

	len = scnprintf(buf, buflen, "AC queues: VO:%d VI:%d BE:%d BK:%d\n",
			sdata->vif.hw_queue[IEEE80211_AC_VO],
			sdata->vif.hw_queue[IEEE80211_AC_VI],
			sdata->vif.hw_queue[IEEE80211_AC_BE],
			sdata->vif.hw_queue[IEEE80211_AC_BK]);

	if (sdata->vif.type == NL80211_IFTYPE_AP)
		len += scnprintf(buf + len, buflen - len, "cab queue: %d\n",
				 sdata->vif.cab_queue);

	return len;
}
__IEEE80211_IF_FILE(hw_queues, NULL);
>>>>>>> refs/remotes/origin/master

/* STA attributes */
IEEE80211_IF_FILE(bssid, u.mgd.bssid, MAC);
IEEE80211_IF_FILE(aid, u.mgd.aid, DEC);
IEEE80211_IF_FILE(last_beacon, u.mgd.last_beacon_signal, DEC);
IEEE80211_IF_FILE(ave_beacon, u.mgd.ave_beacon_signal, DEC_DIV_16);
<<<<<<< HEAD
=======
IEEE80211_IF_FILE(beacon_timeout, u.mgd.beacon_timeout, JIFFIES_TO_MS);
>>>>>>> refs/remotes/origin/master

static int ieee80211_set_smps(struct ieee80211_sub_if_data *sdata,
			      enum ieee80211_smps_mode smps_mode)
{
	struct ieee80211_local *local = sdata->local;
	int err;

	if (!(local->hw.flags & IEEE80211_HW_SUPPORTS_STATIC_SMPS) &&
	    smps_mode == IEEE80211_SMPS_STATIC)
		return -EINVAL;

	/* auto should be dynamic if in PS mode */
	if (!(local->hw.flags & IEEE80211_HW_SUPPORTS_DYNAMIC_SMPS) &&
	    (smps_mode == IEEE80211_SMPS_DYNAMIC ||
	     smps_mode == IEEE80211_SMPS_AUTOMATIC))
		return -EINVAL;

<<<<<<< HEAD
	/* supported only on managed interfaces for now */
	if (sdata->vif.type != NL80211_IFTYPE_STATION)
		return -EOPNOTSUPP;

	mutex_lock(&sdata->u.mgd.mtx);
	err = __ieee80211_request_smps(sdata, smps_mode);
	mutex_unlock(&sdata->u.mgd.mtx);
=======
	if (sdata->vif.type != NL80211_IFTYPE_STATION &&
	    sdata->vif.type != NL80211_IFTYPE_AP)
		return -EOPNOTSUPP;

	sdata_lock(sdata);
	if (sdata->vif.type == NL80211_IFTYPE_STATION)
		err = __ieee80211_request_smps_mgd(sdata, smps_mode);
	else
		err = __ieee80211_request_smps_ap(sdata, smps_mode);
	sdata_unlock(sdata);
>>>>>>> refs/remotes/origin/master

	return err;
}

static const char *smps_modes[IEEE80211_SMPS_NUM_MODES] = {
	[IEEE80211_SMPS_AUTOMATIC] = "auto",
	[IEEE80211_SMPS_OFF] = "off",
	[IEEE80211_SMPS_STATIC] = "static",
	[IEEE80211_SMPS_DYNAMIC] = "dynamic",
};

static ssize_t ieee80211_if_fmt_smps(const struct ieee80211_sub_if_data *sdata,
				     char *buf, int buflen)
{
<<<<<<< HEAD
	if (sdata->vif.type != NL80211_IFTYPE_STATION)
		return -EOPNOTSUPP;

	return snprintf(buf, buflen, "request: %s\nused: %s\n",
			smps_modes[sdata->u.mgd.req_smps],
			smps_modes[sdata->u.mgd.ap_smps]);
=======
	if (sdata->vif.type == NL80211_IFTYPE_STATION)
		return snprintf(buf, buflen, "request: %s\nused: %s\n",
				smps_modes[sdata->u.mgd.req_smps],
				smps_modes[sdata->smps_mode]);
	if (sdata->vif.type == NL80211_IFTYPE_AP)
		return snprintf(buf, buflen, "request: %s\nused: %s\n",
				smps_modes[sdata->u.ap.req_smps],
				smps_modes[sdata->smps_mode]);
	return -EINVAL;
>>>>>>> refs/remotes/origin/master
}

static ssize_t ieee80211_if_parse_smps(struct ieee80211_sub_if_data *sdata,
				       const char *buf, int buflen)
{
	enum ieee80211_smps_mode mode;

	for (mode = 0; mode < IEEE80211_SMPS_NUM_MODES; mode++) {
		if (strncmp(buf, smps_modes[mode], buflen) == 0) {
			int err = ieee80211_set_smps(sdata, mode);
			if (!err)
				return buflen;
			return err;
		}
	}

	return -EINVAL;
}

__IEEE80211_IF_FILE_W(smps);

static ssize_t ieee80211_if_fmt_tkip_mic_test(
	const struct ieee80211_sub_if_data *sdata, char *buf, int buflen)
{
	return -EOPNOTSUPP;
}

<<<<<<< HEAD
static int hwaddr_aton(const char *txt, u8 *addr)
{
	int i;

	for (i = 0; i < ETH_ALEN; i++) {
		int a, b;

		a = hex_to_bin(*txt++);
		if (a < 0)
			return -1;
		b = hex_to_bin(*txt++);
		if (b < 0)
			return -1;
		*addr++ = (a << 4) | b;
		if (i < 5 && *txt++ != ':')
			return -1;
	}

	return 0;
}

=======
>>>>>>> refs/remotes/origin/master
static ssize_t ieee80211_if_parse_tkip_mic_test(
	struct ieee80211_sub_if_data *sdata, const char *buf, int buflen)
{
	struct ieee80211_local *local = sdata->local;
	u8 addr[ETH_ALEN];
	struct sk_buff *skb;
	struct ieee80211_hdr *hdr;
	__le16 fc;

<<<<<<< HEAD
	/*
	 * Assume colon-delimited MAC address with possible white space
	 * following.
	 */
	if (buflen < 3 * ETH_ALEN - 1)
		return -EINVAL;
	if (hwaddr_aton(buf, addr) < 0)
=======
	if (!mac_pton(buf, addr))
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	if (!ieee80211_sdata_running(sdata))
		return -ENOTCONN;

	skb = dev_alloc_skb(local->hw.extra_tx_headroom + 24 + 100);
	if (!skb)
		return -ENOMEM;
	skb_reserve(skb, local->hw.extra_tx_headroom);

	hdr = (struct ieee80211_hdr *) skb_put(skb, 24);
	memset(hdr, 0, 24);
	fc = cpu_to_le16(IEEE80211_FTYPE_DATA | IEEE80211_STYPE_DATA);

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP:
		fc |= cpu_to_le16(IEEE80211_FCTL_FROMDS);
		/* DA BSSID SA */
		memcpy(hdr->addr1, addr, ETH_ALEN);
		memcpy(hdr->addr2, sdata->vif.addr, ETH_ALEN);
		memcpy(hdr->addr3, sdata->vif.addr, ETH_ALEN);
		break;
	case NL80211_IFTYPE_STATION:
		fc |= cpu_to_le16(IEEE80211_FCTL_TODS);
		/* BSSID SA DA */
<<<<<<< HEAD
		if (sdata->vif.bss_conf.bssid == NULL) {
			dev_kfree_skb(skb);
			return -ENOTCONN;
		}
		memcpy(hdr->addr1, sdata->vif.bss_conf.bssid, ETH_ALEN);
		memcpy(hdr->addr2, sdata->vif.addr, ETH_ALEN);
		memcpy(hdr->addr3, addr, ETH_ALEN);
=======
		sdata_lock(sdata);
		if (!sdata->u.mgd.associated) {
			sdata_unlock(sdata);
			dev_kfree_skb(skb);
			return -ENOTCONN;
		}
		memcpy(hdr->addr1, sdata->u.mgd.associated->bssid, ETH_ALEN);
		memcpy(hdr->addr2, sdata->vif.addr, ETH_ALEN);
		memcpy(hdr->addr3, addr, ETH_ALEN);
		sdata_unlock(sdata);
>>>>>>> refs/remotes/origin/master
		break;
	default:
		dev_kfree_skb(skb);
		return -EOPNOTSUPP;
	}
	hdr->frame_control = fc;

	/*
	 * Add some length to the test frame to make it look bit more valid.
	 * The exact contents does not matter since the recipient is required
	 * to drop this because of the Michael MIC failure.
	 */
	memset(skb_put(skb, 50), 0, 50);

	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_TKIP_MIC_FAILURE;

	ieee80211_tx_skb(sdata, skb);

	return buflen;
}

__IEEE80211_IF_FILE_W(tkip_mic_test);

<<<<<<< HEAD
<<<<<<< HEAD
/* AP attributes */
=======
=======
>>>>>>> refs/remotes/origin/master
static ssize_t ieee80211_if_fmt_uapsd_queues(
	const struct ieee80211_sub_if_data *sdata, char *buf, int buflen)
{
	const struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	return snprintf(buf, buflen, "0x%x\n", ifmgd->uapsd_queues);
}

static ssize_t ieee80211_if_parse_uapsd_queues(
	struct ieee80211_sub_if_data *sdata, const char *buf, int buflen)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	u8 val;
	int ret;

	ret = kstrtou8(buf, 0, &val);
	if (ret)
		return ret;

	if (val & ~IEEE80211_WMM_IE_STA_QOSINFO_AC_MASK)
		return -ERANGE;

	ifmgd->uapsd_queues = val;

	return buflen;
}
__IEEE80211_IF_FILE_W(uapsd_queues);

static ssize_t ieee80211_if_fmt_uapsd_max_sp_len(
	const struct ieee80211_sub_if_data *sdata, char *buf, int buflen)
{
	const struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	return snprintf(buf, buflen, "0x%x\n", ifmgd->uapsd_max_sp_len);
}

static ssize_t ieee80211_if_parse_uapsd_max_sp_len(
	struct ieee80211_sub_if_data *sdata, const char *buf, int buflen)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	unsigned long val;
	int ret;

	ret = kstrtoul(buf, 0, &val);
	if (ret)
		return -EINVAL;

	if (val & ~IEEE80211_WMM_IE_STA_QOSINFO_SP_MASK)
		return -ERANGE;

	ifmgd->uapsd_max_sp_len = val;

	return buflen;
}
__IEEE80211_IF_FILE_W(uapsd_max_sp_len);

/* AP attributes */
<<<<<<< HEAD
IEEE80211_IF_FILE(num_sta_authorized, u.ap.num_sta_authorized, ATOMIC);
>>>>>>> refs/remotes/origin/cm-10.0
IEEE80211_IF_FILE(num_sta_ps, u.ap.num_sta_ps, ATOMIC);
IEEE80211_IF_FILE(dtim_count, u.ap.dtim_count, DEC);
=======
IEEE80211_IF_FILE(num_mcast_sta, u.ap.num_mcast_sta, ATOMIC);
IEEE80211_IF_FILE(num_sta_ps, u.ap.ps.num_sta_ps, ATOMIC);
IEEE80211_IF_FILE(dtim_count, u.ap.ps.dtim_count, DEC);
>>>>>>> refs/remotes/origin/master

static ssize_t ieee80211_if_fmt_num_buffered_multicast(
	const struct ieee80211_sub_if_data *sdata, char *buf, int buflen)
{
	return scnprintf(buf, buflen, "%u\n",
<<<<<<< HEAD
			 skb_queue_len(&sdata->u.ap.ps_bc_buf));
}
__IEEE80211_IF_FILE(num_buffered_multicast, NULL);

<<<<<<< HEAD
=======
=======
			 skb_queue_len(&sdata->u.ap.ps.bc_buf));
}
__IEEE80211_IF_FILE(num_buffered_multicast, NULL);

>>>>>>> refs/remotes/origin/master
/* IBSS attributes */
static ssize_t ieee80211_if_fmt_tsf(
	const struct ieee80211_sub_if_data *sdata, char *buf, int buflen)
{
	struct ieee80211_local *local = sdata->local;
	u64 tsf;

	tsf = drv_get_tsf(local, (struct ieee80211_sub_if_data *)sdata);

	return scnprintf(buf, buflen, "0x%016llx\n", (unsigned long long) tsf);
}

static ssize_t ieee80211_if_parse_tsf(
	struct ieee80211_sub_if_data *sdata, const char *buf, int buflen)
{
	struct ieee80211_local *local = sdata->local;
	unsigned long long tsf;
	int ret;
<<<<<<< HEAD
=======
	int tsf_is_delta = 0;
>>>>>>> refs/remotes/origin/master

	if (strncmp(buf, "reset", 5) == 0) {
		if (local->ops->reset_tsf) {
			drv_reset_tsf(local, sdata);
			wiphy_info(local->hw.wiphy, "debugfs reset TSF\n");
		}
	} else {
<<<<<<< HEAD
		ret = kstrtoull(buf, 10, &tsf);
		if (ret < 0)
			return -EINVAL;
=======
		if (buflen > 10 && buf[1] == '=') {
			if (buf[0] == '+')
				tsf_is_delta = 1;
			else if (buf[0] == '-')
				tsf_is_delta = -1;
			else
				return -EINVAL;
			buf += 2;
		}
		ret = kstrtoull(buf, 10, &tsf);
		if (ret < 0)
			return ret;
		if (tsf_is_delta)
			tsf = drv_get_tsf(local, sdata) + tsf_is_delta * tsf;
>>>>>>> refs/remotes/origin/master
		if (local->ops->set_tsf) {
			drv_set_tsf(local, sdata, tsf);
			wiphy_info(local->hw.wiphy,
				   "debugfs set TSF to %#018llx\n", tsf);
		}
	}

	return buflen;
}
__IEEE80211_IF_FILE_W(tsf);


<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* WDS attributes */
IEEE80211_IF_FILE(peer, u.wds.remote_addr, MAC);

#ifdef CONFIG_MAC80211_MESH
<<<<<<< HEAD
=======
IEEE80211_IF_FILE(estab_plinks, u.mesh.estab_plinks, ATOMIC);

>>>>>>> refs/remotes/origin/master
/* Mesh stats attributes */
IEEE80211_IF_FILE(fwded_mcast, u.mesh.mshstats.fwded_mcast, DEC);
IEEE80211_IF_FILE(fwded_unicast, u.mesh.mshstats.fwded_unicast, DEC);
IEEE80211_IF_FILE(fwded_frames, u.mesh.mshstats.fwded_frames, DEC);
IEEE80211_IF_FILE(dropped_frames_ttl, u.mesh.mshstats.dropped_frames_ttl, DEC);
<<<<<<< HEAD
<<<<<<< HEAD
=======
IEEE80211_IF_FILE(dropped_frames_congestion,
		u.mesh.mshstats.dropped_frames_congestion, DEC);
>>>>>>> refs/remotes/origin/cm-10.0
IEEE80211_IF_FILE(dropped_frames_no_route,
		u.mesh.mshstats.dropped_frames_no_route, DEC);
IEEE80211_IF_FILE(estab_plinks, u.mesh.mshstats.estab_plinks, ATOMIC);

/* Mesh parameters */
IEEE80211_IF_FILE(dot11MeshMaxRetries,
		u.mesh.mshcfg.dot11MeshMaxRetries, DEC);
IEEE80211_IF_FILE(dot11MeshRetryTimeout,
		u.mesh.mshcfg.dot11MeshRetryTimeout, DEC);
IEEE80211_IF_FILE(dot11MeshConfirmTimeout,
		u.mesh.mshcfg.dot11MeshConfirmTimeout, DEC);
IEEE80211_IF_FILE(dot11MeshHoldingTimeout,
		u.mesh.mshcfg.dot11MeshHoldingTimeout, DEC);
=======
IEEE80211_IF_FILE(dropped_frames_congestion,
		  u.mesh.mshstats.dropped_frames_congestion, DEC);
IEEE80211_IF_FILE(dropped_frames_no_route,
		  u.mesh.mshstats.dropped_frames_no_route, DEC);

/* Mesh parameters */
IEEE80211_IF_FILE(dot11MeshMaxRetries,
		  u.mesh.mshcfg.dot11MeshMaxRetries, DEC);
IEEE80211_IF_FILE(dot11MeshRetryTimeout,
		  u.mesh.mshcfg.dot11MeshRetryTimeout, DEC);
IEEE80211_IF_FILE(dot11MeshConfirmTimeout,
		  u.mesh.mshcfg.dot11MeshConfirmTimeout, DEC);
IEEE80211_IF_FILE(dot11MeshHoldingTimeout,
		  u.mesh.mshcfg.dot11MeshHoldingTimeout, DEC);
>>>>>>> refs/remotes/origin/master
IEEE80211_IF_FILE(dot11MeshTTL, u.mesh.mshcfg.dot11MeshTTL, DEC);
IEEE80211_IF_FILE(element_ttl, u.mesh.mshcfg.element_ttl, DEC);
IEEE80211_IF_FILE(auto_open_plinks, u.mesh.mshcfg.auto_open_plinks, DEC);
IEEE80211_IF_FILE(dot11MeshMaxPeerLinks,
<<<<<<< HEAD
		u.mesh.mshcfg.dot11MeshMaxPeerLinks, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPactivePathTimeout,
		u.mesh.mshcfg.dot11MeshHWMPactivePathTimeout, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPpreqMinInterval,
		u.mesh.mshcfg.dot11MeshHWMPpreqMinInterval, DEC);
<<<<<<< HEAD
=======
IEEE80211_IF_FILE(dot11MeshHWMPperrMinInterval,
		u.mesh.mshcfg.dot11MeshHWMPperrMinInterval, DEC);
>>>>>>> refs/remotes/origin/cm-10.0
IEEE80211_IF_FILE(dot11MeshHWMPnetDiameterTraversalTime,
		u.mesh.mshcfg.dot11MeshHWMPnetDiameterTraversalTime, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPmaxPREQretries,
		u.mesh.mshcfg.dot11MeshHWMPmaxPREQretries, DEC);
IEEE80211_IF_FILE(path_refresh_time,
		u.mesh.mshcfg.path_refresh_time, DEC);
IEEE80211_IF_FILE(min_discovery_timeout,
		u.mesh.mshcfg.min_discovery_timeout, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPRootMode,
		u.mesh.mshcfg.dot11MeshHWMPRootMode, DEC);
<<<<<<< HEAD
=======
IEEE80211_IF_FILE(dot11MeshGateAnnouncementProtocol,
		u.mesh.mshcfg.dot11MeshGateAnnouncementProtocol, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPRannInterval,
		u.mesh.mshcfg.dot11MeshHWMPRannInterval, DEC);
IEEE80211_IF_FILE(dot11MeshForwarding, u.mesh.mshcfg.dot11MeshForwarding, DEC);
IEEE80211_IF_FILE(rssi_threshold, u.mesh.mshcfg.rssi_threshold, DEC);
>>>>>>> refs/remotes/origin/cm-10.0
#endif


#define DEBUGFS_ADD(name) \
	debugfs_create_file(#name, 0400, sdata->debugfs.dir, \
			    sdata, &name##_ops);

#define DEBUGFS_ADD_MODE(name, mode) \
	debugfs_create_file(#name, mode, sdata->debugfs.dir, \
			    sdata, &name##_ops);

static void add_sta_files(struct ieee80211_sub_if_data *sdata)
{
	DEBUGFS_ADD(drop_unencrypted);
	DEBUGFS_ADD(flags);
	DEBUGFS_ADD(state);
	DEBUGFS_ADD(channel_type);
	DEBUGFS_ADD(rc_rateidx_mask_2ghz);
	DEBUGFS_ADD(rc_rateidx_mask_5ghz);
<<<<<<< HEAD
=======
	DEBUGFS_ADD(rc_rateidx_mcs_mask_2ghz);
	DEBUGFS_ADD(rc_rateidx_mcs_mask_5ghz);
>>>>>>> refs/remotes/origin/cm-10.0

=======
		  u.mesh.mshcfg.dot11MeshMaxPeerLinks, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPactivePathTimeout,
		  u.mesh.mshcfg.dot11MeshHWMPactivePathTimeout, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPpreqMinInterval,
		  u.mesh.mshcfg.dot11MeshHWMPpreqMinInterval, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPperrMinInterval,
		  u.mesh.mshcfg.dot11MeshHWMPperrMinInterval, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPnetDiameterTraversalTime,
		  u.mesh.mshcfg.dot11MeshHWMPnetDiameterTraversalTime, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPmaxPREQretries,
		  u.mesh.mshcfg.dot11MeshHWMPmaxPREQretries, DEC);
IEEE80211_IF_FILE(path_refresh_time,
		  u.mesh.mshcfg.path_refresh_time, DEC);
IEEE80211_IF_FILE(min_discovery_timeout,
		  u.mesh.mshcfg.min_discovery_timeout, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPRootMode,
		  u.mesh.mshcfg.dot11MeshHWMPRootMode, DEC);
IEEE80211_IF_FILE(dot11MeshGateAnnouncementProtocol,
		  u.mesh.mshcfg.dot11MeshGateAnnouncementProtocol, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPRannInterval,
		  u.mesh.mshcfg.dot11MeshHWMPRannInterval, DEC);
IEEE80211_IF_FILE(dot11MeshForwarding, u.mesh.mshcfg.dot11MeshForwarding, DEC);
IEEE80211_IF_FILE(rssi_threshold, u.mesh.mshcfg.rssi_threshold, DEC);
IEEE80211_IF_FILE(ht_opmode, u.mesh.mshcfg.ht_opmode, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPactivePathToRootTimeout,
		  u.mesh.mshcfg.dot11MeshHWMPactivePathToRootTimeout, DEC);
IEEE80211_IF_FILE(dot11MeshHWMProotInterval,
		  u.mesh.mshcfg.dot11MeshHWMProotInterval, DEC);
IEEE80211_IF_FILE(dot11MeshHWMPconfirmationInterval,
		  u.mesh.mshcfg.dot11MeshHWMPconfirmationInterval, DEC);
IEEE80211_IF_FILE(power_mode, u.mesh.mshcfg.power_mode, DEC);
IEEE80211_IF_FILE(dot11MeshAwakeWindowDuration,
		  u.mesh.mshcfg.dot11MeshAwakeWindowDuration, DEC);
#endif

#define DEBUGFS_ADD_MODE(name, mode) \
	debugfs_create_file(#name, mode, sdata->vif.debugfs_dir, \
			    sdata, &name##_ops);

#define DEBUGFS_ADD(name) DEBUGFS_ADD_MODE(name, 0400)

static void add_common_files(struct ieee80211_sub_if_data *sdata)
{
	DEBUGFS_ADD(drop_unencrypted);
	DEBUGFS_ADD(rc_rateidx_mask_2ghz);
	DEBUGFS_ADD(rc_rateidx_mask_5ghz);
	DEBUGFS_ADD(rc_rateidx_mcs_mask_2ghz);
	DEBUGFS_ADD(rc_rateidx_mcs_mask_5ghz);
	DEBUGFS_ADD(hw_queues);
}

static void add_sta_files(struct ieee80211_sub_if_data *sdata)
{
>>>>>>> refs/remotes/origin/master
	DEBUGFS_ADD(bssid);
	DEBUGFS_ADD(aid);
	DEBUGFS_ADD(last_beacon);
	DEBUGFS_ADD(ave_beacon);
<<<<<<< HEAD
	DEBUGFS_ADD_MODE(smps, 0600);
	DEBUGFS_ADD_MODE(tkip_mic_test, 0200);
<<<<<<< HEAD
=======
	DEBUGFS_ADD_MODE(uapsd_queues, 0600);
	DEBUGFS_ADD_MODE(uapsd_max_sp_len, 0600);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	DEBUGFS_ADD(beacon_timeout);
	DEBUGFS_ADD_MODE(smps, 0600);
	DEBUGFS_ADD_MODE(tkip_mic_test, 0200);
	DEBUGFS_ADD_MODE(uapsd_queues, 0600);
	DEBUGFS_ADD_MODE(uapsd_max_sp_len, 0600);
>>>>>>> refs/remotes/origin/master
}

static void add_ap_files(struct ieee80211_sub_if_data *sdata)
{
<<<<<<< HEAD
	DEBUGFS_ADD(drop_unencrypted);
	DEBUGFS_ADD(flags);
	DEBUGFS_ADD(state);
	DEBUGFS_ADD(channel_type);
	DEBUGFS_ADD(rc_rateidx_mask_2ghz);
	DEBUGFS_ADD(rc_rateidx_mask_5ghz);
<<<<<<< HEAD

=======
	DEBUGFS_ADD(rc_rateidx_mcs_mask_2ghz);
	DEBUGFS_ADD(rc_rateidx_mcs_mask_5ghz);

	DEBUGFS_ADD(num_sta_authorized);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	DEBUGFS_ADD(num_mcast_sta);
	DEBUGFS_ADD_MODE(smps, 0600);
>>>>>>> refs/remotes/origin/master
	DEBUGFS_ADD(num_sta_ps);
	DEBUGFS_ADD(dtim_count);
	DEBUGFS_ADD(num_buffered_multicast);
	DEBUGFS_ADD_MODE(tkip_mic_test, 0200);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static void add_ibss_files(struct ieee80211_sub_if_data *sdata)
{
	DEBUGFS_ADD(channel_type);
	DEBUGFS_ADD(rc_rateidx_mask_2ghz);
	DEBUGFS_ADD(rc_rateidx_mask_5ghz);
	DEBUGFS_ADD(rc_rateidx_mcs_mask_2ghz);
	DEBUGFS_ADD(rc_rateidx_mcs_mask_5ghz);

	DEBUGFS_ADD_MODE(tsf, 0600);
}

>>>>>>> refs/remotes/origin/cm-10.0
static void add_wds_files(struct ieee80211_sub_if_data *sdata)
{
	DEBUGFS_ADD(drop_unencrypted);
	DEBUGFS_ADD(flags);
	DEBUGFS_ADD(state);
	DEBUGFS_ADD(channel_type);
	DEBUGFS_ADD(rc_rateidx_mask_2ghz);
	DEBUGFS_ADD(rc_rateidx_mask_5ghz);
<<<<<<< HEAD
=======
	DEBUGFS_ADD(rc_rateidx_mcs_mask_2ghz);
	DEBUGFS_ADD(rc_rateidx_mcs_mask_5ghz);
>>>>>>> refs/remotes/origin/cm-10.0

	DEBUGFS_ADD(peer);
}

static void add_vlan_files(struct ieee80211_sub_if_data *sdata)
{
	DEBUGFS_ADD(drop_unencrypted);
	DEBUGFS_ADD(flags);
	DEBUGFS_ADD(state);
	DEBUGFS_ADD(channel_type);
	DEBUGFS_ADD(rc_rateidx_mask_2ghz);
	DEBUGFS_ADD(rc_rateidx_mask_5ghz);
<<<<<<< HEAD
=======
	DEBUGFS_ADD(rc_rateidx_mcs_mask_2ghz);
	DEBUGFS_ADD(rc_rateidx_mcs_mask_5ghz);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void add_monitor_files(struct ieee80211_sub_if_data *sdata)
{
	DEBUGFS_ADD(flags);
	DEBUGFS_ADD(state);
	DEBUGFS_ADD(channel_type);
}

#ifdef CONFIG_MAC80211_MESH

<<<<<<< HEAD
=======
static void add_mesh_files(struct ieee80211_sub_if_data *sdata)
{
	DEBUGFS_ADD_MODE(tsf, 0600);
}

>>>>>>> refs/remotes/origin/cm-10.0
static void add_mesh_stats(struct ieee80211_sub_if_data *sdata)
{
	struct dentry *dir = debugfs_create_dir("mesh_stats",
						sdata->debugfs.dir);
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void add_ibss_files(struct ieee80211_sub_if_data *sdata)
{
	DEBUGFS_ADD_MODE(tsf, 0600);
}

static void add_wds_files(struct ieee80211_sub_if_data *sdata)
{
	DEBUGFS_ADD(peer);
}

#ifdef CONFIG_MAC80211_MESH

static void add_mesh_files(struct ieee80211_sub_if_data *sdata)
{
	DEBUGFS_ADD_MODE(tsf, 0600);
	DEBUGFS_ADD_MODE(estab_plinks, 0400);
}

static void add_mesh_stats(struct ieee80211_sub_if_data *sdata)
{
	struct dentry *dir = debugfs_create_dir("mesh_stats",
						sdata->vif.debugfs_dir);
>>>>>>> refs/remotes/origin/master
#define MESHSTATS_ADD(name)\
	debugfs_create_file(#name, 0400, dir, sdata, &name##_ops);

	MESHSTATS_ADD(fwded_mcast);
	MESHSTATS_ADD(fwded_unicast);
	MESHSTATS_ADD(fwded_frames);
	MESHSTATS_ADD(dropped_frames_ttl);
	MESHSTATS_ADD(dropped_frames_no_route);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	MESHSTATS_ADD(dropped_frames_congestion);
>>>>>>> refs/remotes/origin/cm-10.0
	MESHSTATS_ADD(estab_plinks);
=======
	MESHSTATS_ADD(dropped_frames_congestion);
>>>>>>> refs/remotes/origin/master
#undef MESHSTATS_ADD
}

static void add_mesh_config(struct ieee80211_sub_if_data *sdata)
{
	struct dentry *dir = debugfs_create_dir("mesh_config",
<<<<<<< HEAD
						sdata->debugfs.dir);
=======
						sdata->vif.debugfs_dir);
>>>>>>> refs/remotes/origin/master

#define MESHPARAMS_ADD(name) \
	debugfs_create_file(#name, 0600, dir, sdata, &name##_ops);

	MESHPARAMS_ADD(dot11MeshMaxRetries);
	MESHPARAMS_ADD(dot11MeshRetryTimeout);
	MESHPARAMS_ADD(dot11MeshConfirmTimeout);
	MESHPARAMS_ADD(dot11MeshHoldingTimeout);
	MESHPARAMS_ADD(dot11MeshTTL);
	MESHPARAMS_ADD(element_ttl);
	MESHPARAMS_ADD(auto_open_plinks);
	MESHPARAMS_ADD(dot11MeshMaxPeerLinks);
	MESHPARAMS_ADD(dot11MeshHWMPactivePathTimeout);
	MESHPARAMS_ADD(dot11MeshHWMPpreqMinInterval);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	MESHPARAMS_ADD(dot11MeshHWMPperrMinInterval);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	MESHPARAMS_ADD(dot11MeshHWMPperrMinInterval);
>>>>>>> refs/remotes/origin/master
	MESHPARAMS_ADD(dot11MeshHWMPnetDiameterTraversalTime);
	MESHPARAMS_ADD(dot11MeshHWMPmaxPREQretries);
	MESHPARAMS_ADD(path_refresh_time);
	MESHPARAMS_ADD(min_discovery_timeout);
<<<<<<< HEAD
<<<<<<< HEAD

=======
	MESHPARAMS_ADD(dot11MeshHWMPRootMode);
	MESHPARAMS_ADD(dot11MeshHWMPRannInterval);
	MESHPARAMS_ADD(dot11MeshGateAnnouncementProtocol);
	MESHPARAMS_ADD(rssi_threshold);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	MESHPARAMS_ADD(dot11MeshHWMPRootMode);
	MESHPARAMS_ADD(dot11MeshHWMPRannInterval);
	MESHPARAMS_ADD(dot11MeshForwarding);
	MESHPARAMS_ADD(dot11MeshGateAnnouncementProtocol);
	MESHPARAMS_ADD(rssi_threshold);
	MESHPARAMS_ADD(ht_opmode);
	MESHPARAMS_ADD(dot11MeshHWMPactivePathToRootTimeout);
	MESHPARAMS_ADD(dot11MeshHWMProotInterval);
	MESHPARAMS_ADD(dot11MeshHWMPconfirmationInterval);
	MESHPARAMS_ADD(power_mode);
	MESHPARAMS_ADD(dot11MeshAwakeWindowDuration);
>>>>>>> refs/remotes/origin/master
#undef MESHPARAMS_ADD
}
#endif

static void add_files(struct ieee80211_sub_if_data *sdata)
{
<<<<<<< HEAD
	if (!sdata->debugfs.dir)
		return;

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_MESH_POINT:
#ifdef CONFIG_MAC80211_MESH
<<<<<<< HEAD
=======
		add_mesh_files(sdata);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!sdata->vif.debugfs_dir)
		return;

	DEBUGFS_ADD(flags);
	DEBUGFS_ADD(state);
	DEBUGFS_ADD(txpower);
	DEBUGFS_ADD(user_power_level);
	DEBUGFS_ADD(ap_power_level);

	if (sdata->vif.type != NL80211_IFTYPE_MONITOR)
		add_common_files(sdata);

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_MESH_POINT:
#ifdef CONFIG_MAC80211_MESH
		add_mesh_files(sdata);
>>>>>>> refs/remotes/origin/master
		add_mesh_stats(sdata);
		add_mesh_config(sdata);
#endif
		break;
	case NL80211_IFTYPE_STATION:
		add_sta_files(sdata);
		break;
	case NL80211_IFTYPE_ADHOC:
<<<<<<< HEAD
<<<<<<< HEAD
		/* XXX */
=======
		add_ibss_files(sdata);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		add_ibss_files(sdata);
>>>>>>> refs/remotes/origin/master
		break;
	case NL80211_IFTYPE_AP:
		add_ap_files(sdata);
		break;
	case NL80211_IFTYPE_WDS:
		add_wds_files(sdata);
		break;
<<<<<<< HEAD
	case NL80211_IFTYPE_MONITOR:
		add_monitor_files(sdata);
		break;
	case NL80211_IFTYPE_AP_VLAN:
		add_vlan_files(sdata);
		break;
=======
>>>>>>> refs/remotes/origin/master
	default:
		break;
	}
}

void ieee80211_debugfs_add_netdev(struct ieee80211_sub_if_data *sdata)
{
	char buf[10+IFNAMSIZ];

	sprintf(buf, "netdev:%s", sdata->name);
<<<<<<< HEAD
	sdata->debugfs.dir = debugfs_create_dir(buf,
		sdata->local->hw.wiphy->debugfsdir);
	if (sdata->debugfs.dir)
		sdata->debugfs.subdir_stations = debugfs_create_dir("stations",
			sdata->debugfs.dir);
=======
	sdata->vif.debugfs_dir = debugfs_create_dir(buf,
		sdata->local->hw.wiphy->debugfsdir);
	if (sdata->vif.debugfs_dir)
		sdata->debugfs.subdir_stations = debugfs_create_dir("stations",
			sdata->vif.debugfs_dir);
>>>>>>> refs/remotes/origin/master
	add_files(sdata);
}

void ieee80211_debugfs_remove_netdev(struct ieee80211_sub_if_data *sdata)
{
<<<<<<< HEAD
	if (!sdata->debugfs.dir)
		return;

	debugfs_remove_recursive(sdata->debugfs.dir);
	sdata->debugfs.dir = NULL;
=======
	if (!sdata->vif.debugfs_dir)
		return;

	debugfs_remove_recursive(sdata->vif.debugfs_dir);
	sdata->vif.debugfs_dir = NULL;
>>>>>>> refs/remotes/origin/master
}

void ieee80211_debugfs_rename_netdev(struct ieee80211_sub_if_data *sdata)
{
	struct dentry *dir;
	char buf[10 + IFNAMSIZ];

<<<<<<< HEAD
	dir = sdata->debugfs.dir;
=======
	dir = sdata->vif.debugfs_dir;
>>>>>>> refs/remotes/origin/master

	if (!dir)
		return;

	sprintf(buf, "netdev:%s", sdata->name);
	if (!debugfs_rename(dir->d_parent, dir, dir->d_parent, buf))
<<<<<<< HEAD
		printk(KERN_ERR "mac80211: debugfs: failed to rename debugfs "
		       "dir to %s\n", buf);
=======
		sdata_err(sdata,
			  "debugfs: failed to rename debugfs dir to %s\n",
			  buf);
>>>>>>> refs/remotes/origin/master
}
