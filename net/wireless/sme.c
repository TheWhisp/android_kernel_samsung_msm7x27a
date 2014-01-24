/*
<<<<<<< HEAD
 * SME code for cfg80211's connect emulation.
=======
 * SME code for cfg80211
 * both driver SME event handling and the SME implementation
 * (for nl80211's connect() and wext)
>>>>>>> refs/remotes/origin/master
 *
 * Copyright 2009	Johannes Berg <johannes@sipsolutions.net>
 * Copyright (C) 2009   Intel Corporation. All rights reserved.
 */

#include <linux/etherdevice.h>
#include <linux/if_arp.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/wireless.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <net/iw_handler.h>
#include <net/cfg80211.h>
#include <net/rtnetlink.h>
#include "nl80211.h"
#include "reg.h"
<<<<<<< HEAD
=======
#include "rdev-ops.h"

/*
 * Software SME in cfg80211, using auth/assoc/deauth calls to the
 * driver. This is is for implementing nl80211's connect/disconnect
 * and wireless extensions (if configured.)
 */
>>>>>>> refs/remotes/origin/master

struct cfg80211_conn {
	struct cfg80211_connect_params params;
	/* these are sub-states of the _CONNECTING sme_state */
	enum {
<<<<<<< HEAD
		CFG80211_CONN_IDLE,
=======
>>>>>>> refs/remotes/origin/master
		CFG80211_CONN_SCANNING,
		CFG80211_CONN_SCAN_AGAIN,
		CFG80211_CONN_AUTHENTICATE_NEXT,
		CFG80211_CONN_AUTHENTICATING,
<<<<<<< HEAD
		CFG80211_CONN_ASSOCIATE_NEXT,
		CFG80211_CONN_ASSOCIATING,
		CFG80211_CONN_DEAUTH_ASSOC_FAIL,
=======
		CFG80211_CONN_AUTH_FAILED,
		CFG80211_CONN_ASSOCIATE_NEXT,
		CFG80211_CONN_ASSOCIATING,
		CFG80211_CONN_ASSOC_FAILED,
		CFG80211_CONN_DEAUTH,
		CFG80211_CONN_CONNECTED,
>>>>>>> refs/remotes/origin/master
	} state;
	u8 bssid[ETH_ALEN], prev_bssid[ETH_ALEN];
	u8 *ie;
	size_t ie_len;
	bool auto_auth, prev_bssid_valid;
};

<<<<<<< HEAD
static bool cfg80211_is_all_idle(void)
{
	struct cfg80211_registered_device *rdev;
	struct wireless_dev *wdev;
	bool is_all_idle = true;

	mutex_lock(&cfg80211_mutex);

	/*
	 * All devices must be idle as otherwise if you are actively
	 * scanning some new beacon hints could be learned and would
	 * count as new regulatory hints.
	 */
	list_for_each_entry(rdev, &cfg80211_rdev_list, list) {
		cfg80211_lock_rdev(rdev);
		list_for_each_entry(wdev, &rdev->netdev_list, list) {
			wdev_lock(wdev);
			if (wdev->sme_state != CFG80211_SME_IDLE)
				is_all_idle = false;
			wdev_unlock(wdev);
		}
		cfg80211_unlock_rdev(rdev);
	}

	mutex_unlock(&cfg80211_mutex);

	return is_all_idle;
}

static void disconnect_work(struct work_struct *work)
{
	if (!cfg80211_is_all_idle())
		return;

	regulatory_hint_disconnect();
}

static DECLARE_WORK(cfg80211_disconnect_work, disconnect_work);

=======
static void cfg80211_sme_free(struct wireless_dev *wdev)
{
	if (!wdev->conn)
		return;

	kfree(wdev->conn->ie);
	kfree(wdev->conn);
	wdev->conn = NULL;
}

>>>>>>> refs/remotes/origin/master
static int cfg80211_conn_scan(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct cfg80211_scan_request *request;
	int n_channels, err;

	ASSERT_RTNL();
	ASSERT_RDEV_LOCK(rdev);
	ASSERT_WDEV_LOCK(wdev);

	if (rdev->scan_req)
		return -EBUSY;

	if (wdev->conn->params.channel) {
		n_channels = 1;
	} else {
		enum ieee80211_band band;
		n_channels = 0;

		for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
			if (!wdev->wiphy->bands[band])
				continue;
			n_channels += wdev->wiphy->bands[band]->n_channels;
		}
	}
	request = kzalloc(sizeof(*request) + sizeof(request->ssids[0]) +
			  sizeof(request->channels[0]) * n_channels,
			  GFP_KERNEL);
	if (!request)
		return -ENOMEM;

	if (wdev->conn->params.channel)
		request->channels[0] = wdev->conn->params.channel;
	else {
		int i = 0, j;
		enum ieee80211_band band;
<<<<<<< HEAD
<<<<<<< HEAD

		for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
			if (!wdev->wiphy->bands[band])
				continue;
			for (j = 0; j < wdev->wiphy->bands[band]->n_channels;
			     i++, j++)
				request->channels[i] =
					&wdev->wiphy->bands[band]->channels[j];
		}
=======
=======
>>>>>>> refs/remotes/origin/master
		struct ieee80211_supported_band *bands;
		struct ieee80211_channel *channel;

		for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
			bands = wdev->wiphy->bands[band];
			if (!bands)
				continue;
			for (j = 0; j < bands->n_channels; j++) {
				channel = &bands->channels[j];
				if (channel->flags & IEEE80211_CHAN_DISABLED)
					continue;
				request->channels[i++] = channel;
			}
			request->rates[band] = (1 << bands->n_bitrates) - 1;
		}
		n_channels = i;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	request->n_channels = n_channels;
	request->ssids = (void *)&request->channels[n_channels];
	request->n_ssids = 1;

	memcpy(request->ssids[0].ssid, wdev->conn->params.ssid,
		wdev->conn->params.ssid_len);
	request->ssids[0].ssid_len = wdev->conn->params.ssid_len;

<<<<<<< HEAD
	request->dev = wdev->netdev;
	request->wiphy = &rdev->wiphy;

	rdev->scan_req = request;

	err = rdev->ops->scan(wdev->wiphy, wdev->netdev, request);
	if (!err) {
		wdev->conn->state = CFG80211_CONN_SCANNING;
		nl80211_send_scan_start(rdev, wdev->netdev);
=======
	request->wdev = wdev;
	request->wiphy = &rdev->wiphy;
	request->scan_start = jiffies;

	rdev->scan_req = request;

	err = rdev_scan(rdev, request);
	if (!err) {
		wdev->conn->state = CFG80211_CONN_SCANNING;
		nl80211_send_scan_start(rdev, wdev);
>>>>>>> refs/remotes/origin/master
		dev_hold(wdev->netdev);
	} else {
		rdev->scan_req = NULL;
		kfree(request);
	}
	return err;
}

static int cfg80211_conn_do_work(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct cfg80211_connect_params *params;
<<<<<<< HEAD
	const u8 *prev_bssid = NULL;
=======
	struct cfg80211_assoc_request req = {};
>>>>>>> refs/remotes/origin/master
	int err;

	ASSERT_WDEV_LOCK(wdev);

	if (!wdev->conn)
		return 0;

	params = &wdev->conn->params;

	switch (wdev->conn->state) {
<<<<<<< HEAD
=======
	case CFG80211_CONN_SCANNING:
		/* didn't find it during scan ... */
		return -ENOENT;
>>>>>>> refs/remotes/origin/master
	case CFG80211_CONN_SCAN_AGAIN:
		return cfg80211_conn_scan(wdev);
	case CFG80211_CONN_AUTHENTICATE_NEXT:
		BUG_ON(!rdev->ops->auth);
		wdev->conn->state = CFG80211_CONN_AUTHENTICATING;
<<<<<<< HEAD
		return __cfg80211_mlme_auth(rdev, wdev->netdev,
					    params->channel, params->auth_type,
					    params->bssid,
					    params->ssid, params->ssid_len,
					    NULL, 0,
					    params->key, params->key_len,
<<<<<<< HEAD
					    params->key_idx, false);
=======
					    params->key_idx);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return cfg80211_mlme_auth(rdev, wdev->netdev,
					  params->channel, params->auth_type,
					  params->bssid,
					  params->ssid, params->ssid_len,
					  NULL, 0,
					  params->key, params->key_len,
					  params->key_idx, NULL, 0);
	case CFG80211_CONN_AUTH_FAILED:
		return -ENOTCONN;
>>>>>>> refs/remotes/origin/master
	case CFG80211_CONN_ASSOCIATE_NEXT:
		BUG_ON(!rdev->ops->assoc);
		wdev->conn->state = CFG80211_CONN_ASSOCIATING;
		if (wdev->conn->prev_bssid_valid)
<<<<<<< HEAD
			prev_bssid = wdev->conn->prev_bssid;
		err = __cfg80211_mlme_assoc(rdev, wdev->netdev,
					    params->channel, params->bssid,
					    prev_bssid,
					    params->ssid, params->ssid_len,
					    params->ie, params->ie_len,
<<<<<<< HEAD
					    false, &params->crypto);
=======
					    false, &params->crypto,
					    params->flags, &params->ht_capa,
					    &params->ht_capa_mask);
>>>>>>> refs/remotes/origin/cm-10.0
		if (err)
			__cfg80211_mlme_deauth(rdev, wdev->netdev, params->bssid,
					       NULL, 0,
					       WLAN_REASON_DEAUTH_LEAVING,
					       false);
		return err;
	case CFG80211_CONN_DEAUTH_ASSOC_FAIL:
		__cfg80211_mlme_deauth(rdev, wdev->netdev, params->bssid,
				       NULL, 0,
				       WLAN_REASON_DEAUTH_LEAVING, false);
		/* return an error so that we call __cfg80211_connect_result() */
		return -EINVAL;
=======
			req.prev_bssid = wdev->conn->prev_bssid;
		req.ie = params->ie;
		req.ie_len = params->ie_len;
		req.use_mfp = params->mfp != NL80211_MFP_NO;
		req.crypto = params->crypto;
		req.flags = params->flags;
		req.ht_capa = params->ht_capa;
		req.ht_capa_mask = params->ht_capa_mask;
		req.vht_capa = params->vht_capa;
		req.vht_capa_mask = params->vht_capa_mask;

		err = cfg80211_mlme_assoc(rdev, wdev->netdev, params->channel,
					  params->bssid, params->ssid,
					  params->ssid_len, &req);
		if (err)
			cfg80211_mlme_deauth(rdev, wdev->netdev, params->bssid,
					     NULL, 0,
					     WLAN_REASON_DEAUTH_LEAVING,
					     false);
		return err;
	case CFG80211_CONN_ASSOC_FAILED:
		cfg80211_mlme_deauth(rdev, wdev->netdev, params->bssid,
				     NULL, 0,
				     WLAN_REASON_DEAUTH_LEAVING, false);
		return -ENOTCONN;
	case CFG80211_CONN_DEAUTH:
		cfg80211_mlme_deauth(rdev, wdev->netdev, params->bssid,
				     NULL, 0,
				     WLAN_REASON_DEAUTH_LEAVING, false);
		/* free directly, disconnected event already sent */
		cfg80211_sme_free(wdev);
		return 0;
>>>>>>> refs/remotes/origin/master
	default:
		return 0;
	}
}

void cfg80211_conn_work(struct work_struct *work)
{
	struct cfg80211_registered_device *rdev =
		container_of(work, struct cfg80211_registered_device, conn_work);
	struct wireless_dev *wdev;
	u8 bssid_buf[ETH_ALEN], *bssid = NULL;

	rtnl_lock();
<<<<<<< HEAD
	cfg80211_lock_rdev(rdev);
	mutex_lock(&rdev->devlist_mtx);

	list_for_each_entry(wdev, &rdev->netdev_list, list) {
=======

	list_for_each_entry(wdev, &rdev->wdev_list, list) {
		if (!wdev->netdev)
			continue;

>>>>>>> refs/remotes/origin/master
		wdev_lock(wdev);
		if (!netif_running(wdev->netdev)) {
			wdev_unlock(wdev);
			continue;
		}
<<<<<<< HEAD
		if (wdev->sme_state != CFG80211_SME_CONNECTING) {
=======
		if (!wdev->conn ||
		    wdev->conn->state == CFG80211_CONN_CONNECTED) {
>>>>>>> refs/remotes/origin/master
			wdev_unlock(wdev);
			continue;
		}
		if (wdev->conn->params.bssid) {
			memcpy(bssid_buf, wdev->conn->params.bssid, ETH_ALEN);
			bssid = bssid_buf;
		}
<<<<<<< HEAD
		if (cfg80211_conn_do_work(wdev))
=======
		if (cfg80211_conn_do_work(wdev)) {
>>>>>>> refs/remotes/origin/master
			__cfg80211_connect_result(
					wdev->netdev, bssid,
					NULL, 0, NULL, 0,
					WLAN_STATUS_UNSPECIFIED_FAILURE,
					false, NULL);
<<<<<<< HEAD
		wdev_unlock(wdev);
	}

	mutex_unlock(&rdev->devlist_mtx);
	cfg80211_unlock_rdev(rdev);
	rtnl_unlock();
}

=======
			cfg80211_sme_free(wdev);
		}
		wdev_unlock(wdev);
	}

	rtnl_unlock();
}

/* Returned bss is reference counted and must be cleaned up appropriately. */
>>>>>>> refs/remotes/origin/master
static struct cfg80211_bss *cfg80211_get_conn_bss(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct cfg80211_bss *bss;
	u16 capa = WLAN_CAPABILITY_ESS;

	ASSERT_WDEV_LOCK(wdev);

	if (wdev->conn->params.privacy)
		capa |= WLAN_CAPABILITY_PRIVACY;

	bss = cfg80211_get_bss(wdev->wiphy, wdev->conn->params.channel,
			       wdev->conn->params.bssid,
			       wdev->conn->params.ssid,
			       wdev->conn->params.ssid_len,
			       WLAN_CAPABILITY_ESS | WLAN_CAPABILITY_PRIVACY,
			       capa);
	if (!bss)
		return NULL;

	memcpy(wdev->conn->bssid, bss->bssid, ETH_ALEN);
	wdev->conn->params.bssid = wdev->conn->bssid;
	wdev->conn->params.channel = bss->channel;
	wdev->conn->state = CFG80211_CONN_AUTHENTICATE_NEXT;
	schedule_work(&rdev->conn_work);

	return bss;
}

static void __cfg80211_sme_scan_done(struct net_device *dev)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct cfg80211_bss *bss;

	ASSERT_WDEV_LOCK(wdev);

<<<<<<< HEAD
	if (wdev->sme_state != CFG80211_SME_CONNECTING)
		return;

=======
>>>>>>> refs/remotes/origin/master
	if (!wdev->conn)
		return;

	if (wdev->conn->state != CFG80211_CONN_SCANNING &&
	    wdev->conn->state != CFG80211_CONN_SCAN_AGAIN)
		return;

	bss = cfg80211_get_conn_bss(wdev);
<<<<<<< HEAD
	if (bss) {
		cfg80211_put_bss(bss);
	} else {
		/* not found */
		if (wdev->conn->state == CFG80211_CONN_SCAN_AGAIN)
			schedule_work(&rdev->conn_work);
		else
			__cfg80211_connect_result(
					wdev->netdev,
					wdev->conn->params.bssid,
					NULL, 0, NULL, 0,
					WLAN_STATUS_UNSPECIFIED_FAILURE,
					false, NULL);
	}
=======
	if (bss)
		cfg80211_put_bss(&rdev->wiphy, bss);
	else
		schedule_work(&rdev->conn_work);
>>>>>>> refs/remotes/origin/master
}

void cfg80211_sme_scan_done(struct net_device *dev)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;

<<<<<<< HEAD
	mutex_lock(&wiphy_to_dev(wdev->wiphy)->devlist_mtx);
	wdev_lock(wdev);
	__cfg80211_sme_scan_done(dev);
	wdev_unlock(wdev);
	mutex_unlock(&wiphy_to_dev(wdev->wiphy)->devlist_mtx);
}

void cfg80211_sme_rx_auth(struct net_device *dev,
			  const u8 *buf, size_t len)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
=======
	wdev_lock(wdev);
	__cfg80211_sme_scan_done(dev);
	wdev_unlock(wdev);
}

void cfg80211_sme_rx_auth(struct wireless_dev *wdev, const u8 *buf, size_t len)
{
>>>>>>> refs/remotes/origin/master
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)buf;
	u16 status_code = le16_to_cpu(mgmt->u.auth.status_code);

	ASSERT_WDEV_LOCK(wdev);

<<<<<<< HEAD
	/* should only RX auth frames when connecting */
	if (wdev->sme_state != CFG80211_SME_CONNECTING)
		return;

	if (WARN_ON(!wdev->conn))
=======
	if (!wdev->conn || wdev->conn->state == CFG80211_CONN_CONNECTED)
>>>>>>> refs/remotes/origin/master
		return;

	if (status_code == WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG &&
	    wdev->conn->auto_auth &&
	    wdev->conn->params.auth_type != NL80211_AUTHTYPE_NETWORK_EAP) {
		/* select automatically between only open, shared, leap */
		switch (wdev->conn->params.auth_type) {
		case NL80211_AUTHTYPE_OPEN_SYSTEM:
			if (wdev->connect_keys)
				wdev->conn->params.auth_type =
					NL80211_AUTHTYPE_SHARED_KEY;
			else
				wdev->conn->params.auth_type =
					NL80211_AUTHTYPE_NETWORK_EAP;
			break;
		case NL80211_AUTHTYPE_SHARED_KEY:
			wdev->conn->params.auth_type =
				NL80211_AUTHTYPE_NETWORK_EAP;
			break;
		default:
			/* huh? */
			wdev->conn->params.auth_type =
				NL80211_AUTHTYPE_OPEN_SYSTEM;
			break;
		}
		wdev->conn->state = CFG80211_CONN_AUTHENTICATE_NEXT;
		schedule_work(&rdev->conn_work);
	} else if (status_code != WLAN_STATUS_SUCCESS) {
<<<<<<< HEAD
		__cfg80211_connect_result(dev, mgmt->bssid, NULL, 0, NULL, 0,
					  status_code, false, NULL);
	} else if (wdev->sme_state == CFG80211_SME_CONNECTING &&
		 wdev->conn->state == CFG80211_CONN_AUTHENTICATING) {
=======
		__cfg80211_connect_result(wdev->netdev, mgmt->bssid,
					  NULL, 0, NULL, 0,
					  status_code, false, NULL);
	} else if (wdev->conn->state == CFG80211_CONN_AUTHENTICATING) {
>>>>>>> refs/remotes/origin/master
		wdev->conn->state = CFG80211_CONN_ASSOCIATE_NEXT;
		schedule_work(&rdev->conn_work);
	}
}

<<<<<<< HEAD
bool cfg80211_sme_failed_reassoc(struct wireless_dev *wdev)
{
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	if (WARN_ON(!wdev->conn))
		return false;

	if (!wdev->conn->prev_bssid_valid)
		return false;

	/*
	 * Some stupid APs don't accept reassoc, so we
	 * need to fall back to trying regular assoc.
	 */
	wdev->conn->prev_bssid_valid = false;
	wdev->conn->state = CFG80211_CONN_ASSOCIATE_NEXT;
	schedule_work(&rdev->conn_work);

	return true;
}

void cfg80211_sme_failed_assoc(struct wireless_dev *wdev)
{
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	wdev->conn->state = CFG80211_CONN_DEAUTH_ASSOC_FAIL;
	schedule_work(&rdev->conn_work);
}

=======
bool cfg80211_sme_rx_assoc_resp(struct wireless_dev *wdev, u16 status)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);

	if (!wdev->conn)
		return false;

	if (status == WLAN_STATUS_SUCCESS) {
		wdev->conn->state = CFG80211_CONN_CONNECTED;
		return false;
	}

	if (wdev->conn->prev_bssid_valid) {
		/*
		 * Some stupid APs don't accept reassoc, so we
		 * need to fall back to trying regular assoc;
		 * return true so no event is sent to userspace.
		 */
		wdev->conn->prev_bssid_valid = false;
		wdev->conn->state = CFG80211_CONN_ASSOCIATE_NEXT;
		schedule_work(&rdev->conn_work);
		return true;
	}

	wdev->conn->state = CFG80211_CONN_ASSOC_FAILED;
	schedule_work(&rdev->conn_work);
	return false;
}

void cfg80211_sme_deauth(struct wireless_dev *wdev)
{
	cfg80211_sme_free(wdev);
}

void cfg80211_sme_auth_timeout(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);

	if (!wdev->conn)
		return;

	wdev->conn->state = CFG80211_CONN_AUTH_FAILED;
	schedule_work(&rdev->conn_work);
}

void cfg80211_sme_disassoc(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);

	if (!wdev->conn)
		return;

	wdev->conn->state = CFG80211_CONN_DEAUTH;
	schedule_work(&rdev->conn_work);
}

void cfg80211_sme_assoc_timeout(struct wireless_dev *wdev)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);

	if (!wdev->conn)
		return;

	wdev->conn->state = CFG80211_CONN_ASSOC_FAILED;
	schedule_work(&rdev->conn_work);
}

static int cfg80211_sme_connect(struct wireless_dev *wdev,
				struct cfg80211_connect_params *connect,
				const u8 *prev_bssid)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct cfg80211_bss *bss;
	int err;

	if (!rdev->ops->auth || !rdev->ops->assoc)
		return -EOPNOTSUPP;

	if (wdev->current_bss)
		return -EALREADY;

	if (WARN_ON(wdev->conn))
		return -EINPROGRESS;

	wdev->conn = kzalloc(sizeof(*wdev->conn), GFP_KERNEL);
	if (!wdev->conn)
		return -ENOMEM;

	/*
	 * Copy all parameters, and treat explicitly IEs, BSSID, SSID.
	 */
	memcpy(&wdev->conn->params, connect, sizeof(*connect));
	if (connect->bssid) {
		wdev->conn->params.bssid = wdev->conn->bssid;
		memcpy(wdev->conn->bssid, connect->bssid, ETH_ALEN);
	}

	if (connect->ie) {
		wdev->conn->ie = kmemdup(connect->ie, connect->ie_len,
					GFP_KERNEL);
		wdev->conn->params.ie = wdev->conn->ie;
		if (!wdev->conn->ie) {
			kfree(wdev->conn);
			wdev->conn = NULL;
			return -ENOMEM;
		}
	}

	if (connect->auth_type == NL80211_AUTHTYPE_AUTOMATIC) {
		wdev->conn->auto_auth = true;
		/* start with open system ... should mostly work */
		wdev->conn->params.auth_type =
			NL80211_AUTHTYPE_OPEN_SYSTEM;
	} else {
		wdev->conn->auto_auth = false;
	}

	wdev->conn->params.ssid = wdev->ssid;
	wdev->conn->params.ssid_len = connect->ssid_len;

	/* see if we have the bss already */
	bss = cfg80211_get_conn_bss(wdev);

	if (prev_bssid) {
		memcpy(wdev->conn->prev_bssid, prev_bssid, ETH_ALEN);
		wdev->conn->prev_bssid_valid = true;
	}

	/* we're good if we have a matching bss struct */
	if (bss) {
		wdev->conn->state = CFG80211_CONN_AUTHENTICATE_NEXT;
		err = cfg80211_conn_do_work(wdev);
		cfg80211_put_bss(wdev->wiphy, bss);
	} else {
		/* otherwise we'll need to scan for the AP first */
		err = cfg80211_conn_scan(wdev);

		/*
		 * If we can't scan right now, then we need to scan again
		 * after the current scan finished, since the parameters
		 * changed (unless we find a good AP anyway).
		 */
		if (err == -EBUSY) {
			err = 0;
			wdev->conn->state = CFG80211_CONN_SCAN_AGAIN;
		}
	}

	if (err)
		cfg80211_sme_free(wdev);

	return err;
}

static int cfg80211_sme_disconnect(struct wireless_dev *wdev, u16 reason)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	int err;

	if (!wdev->conn)
		return 0;

	if (!rdev->ops->deauth)
		return -EOPNOTSUPP;

	if (wdev->conn->state == CFG80211_CONN_SCANNING ||
	    wdev->conn->state == CFG80211_CONN_SCAN_AGAIN) {
		err = 0;
		goto out;
	}

	/* wdev->conn->params.bssid must be set if > SCANNING */
	err = cfg80211_mlme_deauth(rdev, wdev->netdev,
				   wdev->conn->params.bssid,
				   NULL, 0, reason, false);
 out:
	cfg80211_sme_free(wdev);
	return err;
}

/*
 * code shared for in-device and software SME
 */

static bool cfg80211_is_all_idle(void)
{
	struct cfg80211_registered_device *rdev;
	struct wireless_dev *wdev;
	bool is_all_idle = true;

	/*
	 * All devices must be idle as otherwise if you are actively
	 * scanning some new beacon hints could be learned and would
	 * count as new regulatory hints.
	 */
	list_for_each_entry(rdev, &cfg80211_rdev_list, list) {
		list_for_each_entry(wdev, &rdev->wdev_list, list) {
			wdev_lock(wdev);
			if (wdev->conn || wdev->current_bss)
				is_all_idle = false;
			wdev_unlock(wdev);
		}
	}

	return is_all_idle;
}

static void disconnect_work(struct work_struct *work)
{
	rtnl_lock();
	if (cfg80211_is_all_idle())
		regulatory_hint_disconnect();
	rtnl_unlock();
}

static DECLARE_WORK(cfg80211_disconnect_work, disconnect_work);


/*
 * API calls for drivers implementing connect/disconnect and
 * SME event handling
 */

/* This method must consume bss one way or another */
>>>>>>> refs/remotes/origin/master
void __cfg80211_connect_result(struct net_device *dev, const u8 *bssid,
			       const u8 *req_ie, size_t req_ie_len,
			       const u8 *resp_ie, size_t resp_ie_len,
			       u16 status, bool wextev,
			       struct cfg80211_bss *bss)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
	u8 *country_ie;
=======
	const u8 *country_ie;
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_CFG80211_WEXT
	union iwreq_data wrqu;
#endif

	ASSERT_WDEV_LOCK(wdev);

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_STATION &&
<<<<<<< HEAD
		    wdev->iftype != NL80211_IFTYPE_P2P_CLIENT))
		return;

	if (wdev->sme_state != CFG80211_SME_CONNECTING)
		return;
=======
		    wdev->iftype != NL80211_IFTYPE_P2P_CLIENT)) {
		cfg80211_put_bss(wdev->wiphy, bss);
		return;
	}
>>>>>>> refs/remotes/origin/master

	nl80211_send_connect_result(wiphy_to_dev(wdev->wiphy), dev,
				    bssid, req_ie, req_ie_len,
				    resp_ie, resp_ie_len,
				    status, GFP_KERNEL);

#ifdef CONFIG_CFG80211_WEXT
	if (wextev) {
		if (req_ie && status == WLAN_STATUS_SUCCESS) {
			memset(&wrqu, 0, sizeof(wrqu));
			wrqu.data.length = req_ie_len;
			wireless_send_event(dev, IWEVASSOCREQIE, &wrqu, req_ie);
		}

		if (resp_ie && status == WLAN_STATUS_SUCCESS) {
			memset(&wrqu, 0, sizeof(wrqu));
			wrqu.data.length = resp_ie_len;
			wireless_send_event(dev, IWEVASSOCRESPIE, &wrqu, resp_ie);
		}

		memset(&wrqu, 0, sizeof(wrqu));
		wrqu.ap_addr.sa_family = ARPHRD_ETHER;
		if (bssid && status == WLAN_STATUS_SUCCESS) {
			memcpy(wrqu.ap_addr.sa_data, bssid, ETH_ALEN);
			memcpy(wdev->wext.prev_bssid, bssid, ETH_ALEN);
			wdev->wext.prev_bssid_valid = true;
		}
		wireless_send_event(dev, SIOCGIWAP, &wrqu, NULL);
	}
#endif

<<<<<<< HEAD
	if (wdev->current_bss) {
		cfg80211_unhold_bss(wdev->current_bss);
		cfg80211_put_bss(&wdev->current_bss->pub);
		wdev->current_bss = NULL;
	}

	if (wdev->conn)
		wdev->conn->state = CFG80211_CONN_IDLE;

	if (status != WLAN_STATUS_SUCCESS) {
		wdev->sme_state = CFG80211_SME_IDLE;
		if (wdev->conn)
			kfree(wdev->conn->ie);
		kfree(wdev->conn);
		wdev->conn = NULL;
		kfree(wdev->connect_keys);
		wdev->connect_keys = NULL;
		wdev->ssid_len = 0;
<<<<<<< HEAD
=======
		cfg80211_put_bss(bss);
>>>>>>> refs/remotes/origin/cm-10.0
		return;
	}

	if (!bss)
		bss = cfg80211_get_bss(wdev->wiphy,
				       wdev->conn ? wdev->conn->params.channel :
				       NULL,
				       bssid,
				       wdev->ssid, wdev->ssid_len,
				       WLAN_CAPABILITY_ESS,
				       WLAN_CAPABILITY_ESS);

	if (WARN_ON(!bss))
		return;

	cfg80211_hold_bss(bss_from_pub(bss));
	wdev->current_bss = bss_from_pub(bss);

	wdev->sme_state = CFG80211_SME_CONNECTED;
	cfg80211_upload_connect_keys(wdev);

	country_ie = (u8 *) ieee80211_bss_get_ie(bss, WLAN_EID_COUNTRY);
=======
	if (!bss && (status == WLAN_STATUS_SUCCESS)) {
		WARN_ON_ONCE(!wiphy_to_dev(wdev->wiphy)->ops->connect);
		bss = cfg80211_get_bss(wdev->wiphy, NULL, bssid,
				       wdev->ssid, wdev->ssid_len,
				       WLAN_CAPABILITY_ESS,
				       WLAN_CAPABILITY_ESS);
		if (bss)
			cfg80211_hold_bss(bss_from_pub(bss));
	}

	if (wdev->current_bss) {
		cfg80211_unhold_bss(wdev->current_bss);
		cfg80211_put_bss(wdev->wiphy, &wdev->current_bss->pub);
		wdev->current_bss = NULL;
	}

	if (status != WLAN_STATUS_SUCCESS) {
		kfree(wdev->connect_keys);
		wdev->connect_keys = NULL;
		wdev->ssid_len = 0;
		if (bss) {
			cfg80211_unhold_bss(bss_from_pub(bss));
			cfg80211_put_bss(wdev->wiphy, bss);
		}
		return;
	}

	if (WARN_ON(!bss))
		return;

	wdev->current_bss = bss_from_pub(bss);

	cfg80211_upload_connect_keys(wdev);

	rcu_read_lock();
	country_ie = ieee80211_bss_get_ie(bss, WLAN_EID_COUNTRY);
	if (!country_ie) {
		rcu_read_unlock();
		return;
	}

	country_ie = kmemdup(country_ie, 2 + country_ie[1], GFP_ATOMIC);
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master

	if (!country_ie)
		return;

	/*
	 * ieee80211_bss_get_ie() ensures we can access:
	 * - country_ie + 2, the start of the country ie data, and
	 * - and country_ie[1] which is the IE length
	 */
<<<<<<< HEAD
	regulatory_hint_11d(wdev->wiphy,
			    bss->channel->band,
			    country_ie + 2,
			    country_ie[1]);
=======
	regulatory_hint_country_ie(wdev->wiphy, bss->channel->band,
				   country_ie + 2, country_ie[1]);
	kfree(country_ie);
>>>>>>> refs/remotes/origin/master
}

void cfg80211_connect_result(struct net_device *dev, const u8 *bssid,
			     const u8 *req_ie, size_t req_ie_len,
			     const u8 *resp_ie, size_t resp_ie_len,
			     u16 status, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct cfg80211_event *ev;
	unsigned long flags;

<<<<<<< HEAD
	CFG80211_DEV_WARN_ON(wdev->sme_state != CFG80211_SME_CONNECTING);

=======
>>>>>>> refs/remotes/origin/master
	ev = kzalloc(sizeof(*ev) + req_ie_len + resp_ie_len, gfp);
	if (!ev)
		return;

	ev->type = EVENT_CONNECT_RESULT;
	if (bssid)
		memcpy(ev->cr.bssid, bssid, ETH_ALEN);
	if (req_ie_len) {
		ev->cr.req_ie = ((u8 *)ev) + sizeof(*ev);
		ev->cr.req_ie_len = req_ie_len;
		memcpy((void *)ev->cr.req_ie, req_ie, req_ie_len);
	}
	if (resp_ie_len) {
		ev->cr.resp_ie = ((u8 *)ev) + sizeof(*ev) + req_ie_len;
		ev->cr.resp_ie_len = resp_ie_len;
		memcpy((void *)ev->cr.resp_ie, resp_ie, resp_ie_len);
	}
	ev->cr.status = status;

	spin_lock_irqsave(&wdev->event_lock, flags);
	list_add_tail(&ev->list, &wdev->event_list);
	spin_unlock_irqrestore(&wdev->event_lock, flags);
	queue_work(cfg80211_wq, &rdev->event_work);
}
EXPORT_SYMBOL(cfg80211_connect_result);

<<<<<<< HEAD
void __cfg80211_roamed(struct wireless_dev *wdev,
<<<<<<< HEAD
		       struct ieee80211_channel *channel,
		       const u8 *bssid,
		       const u8 *req_ie, size_t req_ie_len,
		       const u8 *resp_ie, size_t resp_ie_len)
{
	struct cfg80211_bss *bss;
#ifdef CONFIG_CFG80211_WEXT
	union iwreq_data wrqu;
#endif

=======
=======
/* Consumes bss object one way or another */
void __cfg80211_roamed(struct wireless_dev *wdev,
>>>>>>> refs/remotes/origin/master
		       struct cfg80211_bss *bss,
		       const u8 *req_ie, size_t req_ie_len,
		       const u8 *resp_ie, size_t resp_ie_len)
{
#ifdef CONFIG_CFG80211_WEXT
	union iwreq_data wrqu;
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ASSERT_WDEV_LOCK(wdev);

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_STATION &&
		    wdev->iftype != NL80211_IFTYPE_P2P_CLIENT))
<<<<<<< HEAD
<<<<<<< HEAD
		return;

	if (wdev->sme_state != CFG80211_SME_CONNECTED)
		return;

	/* internal error -- how did we get to CONNECTED w/o BSS? */
	if (WARN_ON(!wdev->current_bss)) {
		return;
=======
		goto out;

	if (wdev->sme_state != CFG80211_SME_CONNECTED)
		goto out;

	/* internal error -- how did we get to CONNECTED w/o BSS? */
	if (WARN_ON(!wdev->current_bss)) {
		goto out;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	cfg80211_unhold_bss(wdev->current_bss);
	cfg80211_put_bss(&wdev->current_bss->pub);
	wdev->current_bss = NULL;

<<<<<<< HEAD
	bss = cfg80211_get_bss(wdev->wiphy, channel, bssid,
			       wdev->ssid, wdev->ssid_len,
			       WLAN_CAPABILITY_ESS, WLAN_CAPABILITY_ESS);

	if (WARN_ON(!bss))
		return;

	cfg80211_hold_bss(bss_from_pub(bss));
	wdev->current_bss = bss_from_pub(bss);

	nl80211_send_roamed(wiphy_to_dev(wdev->wiphy), wdev->netdev, bssid,
=======
=======
		goto out;

	if (WARN_ON(!wdev->current_bss))
		goto out;

	cfg80211_unhold_bss(wdev->current_bss);
	cfg80211_put_bss(wdev->wiphy, &wdev->current_bss->pub);
	wdev->current_bss = NULL;

>>>>>>> refs/remotes/origin/master
	cfg80211_hold_bss(bss_from_pub(bss));
	wdev->current_bss = bss_from_pub(bss);

	nl80211_send_roamed(wiphy_to_dev(wdev->wiphy), wdev->netdev, bss->bssid,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			    req_ie, req_ie_len, resp_ie, resp_ie_len,
			    GFP_KERNEL);

#ifdef CONFIG_CFG80211_WEXT
	if (req_ie) {
		memset(&wrqu, 0, sizeof(wrqu));
		wrqu.data.length = req_ie_len;
		wireless_send_event(wdev->netdev, IWEVASSOCREQIE,
				    &wrqu, req_ie);
	}

	if (resp_ie) {
		memset(&wrqu, 0, sizeof(wrqu));
		wrqu.data.length = resp_ie_len;
		wireless_send_event(wdev->netdev, IWEVASSOCRESPIE,
				    &wrqu, resp_ie);
	}

	memset(&wrqu, 0, sizeof(wrqu));
	wrqu.ap_addr.sa_family = ARPHRD_ETHER;
<<<<<<< HEAD
<<<<<<< HEAD
	memcpy(wrqu.ap_addr.sa_data, bssid, ETH_ALEN);
	memcpy(wdev->wext.prev_bssid, bssid, ETH_ALEN);
	wdev->wext.prev_bssid_valid = true;
	wireless_send_event(wdev->netdev, SIOCGIWAP, &wrqu, NULL);
#endif
=======
=======
>>>>>>> refs/remotes/origin/master
	memcpy(wrqu.ap_addr.sa_data, bss->bssid, ETH_ALEN);
	memcpy(wdev->wext.prev_bssid, bss->bssid, ETH_ALEN);
	wdev->wext.prev_bssid_valid = true;
	wireless_send_event(wdev->netdev, SIOCGIWAP, &wrqu, NULL);
#endif

	return;
out:
<<<<<<< HEAD
	cfg80211_put_bss(bss);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cfg80211_put_bss(wdev->wiphy, bss);
>>>>>>> refs/remotes/origin/master
}

void cfg80211_roamed(struct net_device *dev,
		     struct ieee80211_channel *channel,
		     const u8 *bssid,
		     const u8 *req_ie, size_t req_ie_len,
		     const u8 *resp_ie, size_t resp_ie_len, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct cfg80211_bss *bss;

	CFG80211_DEV_WARN_ON(wdev->sme_state != CFG80211_SME_CONNECTED);

=======
	struct cfg80211_bss *bss;

>>>>>>> refs/remotes/origin/master
	bss = cfg80211_get_bss(wdev->wiphy, channel, bssid, wdev->ssid,
			       wdev->ssid_len, WLAN_CAPABILITY_ESS,
			       WLAN_CAPABILITY_ESS);
	if (WARN_ON(!bss))
		return;

	cfg80211_roamed_bss(dev, bss, req_ie, req_ie_len, resp_ie,
			    resp_ie_len, gfp);
}
EXPORT_SYMBOL(cfg80211_roamed);

<<<<<<< HEAD
=======
/* Consumes bss object one way or another */
>>>>>>> refs/remotes/origin/master
void cfg80211_roamed_bss(struct net_device *dev,
			 struct cfg80211_bss *bss, const u8 *req_ie,
			 size_t req_ie_len, const u8 *resp_ie,
			 size_t resp_ie_len, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct cfg80211_event *ev;
	unsigned long flags;

<<<<<<< HEAD
	CFG80211_DEV_WARN_ON(wdev->sme_state != CFG80211_SME_CONNECTED);

<<<<<<< HEAD
	ev = kzalloc(sizeof(*ev) + req_ie_len + resp_ie_len, gfp);
	if (!ev)
		return;

	ev->type = EVENT_ROAMED;
	ev->rm.channel = channel;
	memcpy(ev->rm.bssid, bssid, ETH_ALEN);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (WARN_ON(!bss))
		return;

	ev = kzalloc(sizeof(*ev) + req_ie_len + resp_ie_len, gfp);
	if (!ev) {
<<<<<<< HEAD
		cfg80211_put_bss(bss);
=======
		cfg80211_put_bss(wdev->wiphy, bss);
>>>>>>> refs/remotes/origin/master
		return;
	}

	ev->type = EVENT_ROAMED;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ev->rm.req_ie = ((u8 *)ev) + sizeof(*ev);
	ev->rm.req_ie_len = req_ie_len;
	memcpy((void *)ev->rm.req_ie, req_ie, req_ie_len);
	ev->rm.resp_ie = ((u8 *)ev) + sizeof(*ev) + req_ie_len;
	ev->rm.resp_ie_len = resp_ie_len;
	memcpy((void *)ev->rm.resp_ie, resp_ie, resp_ie_len);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	ev->rm.bss = bss;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ev->rm.bss = bss;
>>>>>>> refs/remotes/origin/master

	spin_lock_irqsave(&wdev->event_lock, flags);
	list_add_tail(&ev->list, &wdev->event_list);
	spin_unlock_irqrestore(&wdev->event_lock, flags);
	queue_work(cfg80211_wq, &rdev->event_work);
}
<<<<<<< HEAD
<<<<<<< HEAD
EXPORT_SYMBOL(cfg80211_roamed);
=======
EXPORT_SYMBOL(cfg80211_roamed_bss);
>>>>>>> refs/remotes/origin/cm-10.0
=======
EXPORT_SYMBOL(cfg80211_roamed_bss);
>>>>>>> refs/remotes/origin/master

void __cfg80211_disconnected(struct net_device *dev, const u8 *ie,
			     size_t ie_len, u16 reason, bool from_ap)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	int i;
#ifdef CONFIG_CFG80211_WEXT
	union iwreq_data wrqu;
#endif

	ASSERT_WDEV_LOCK(wdev);

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_STATION &&
		    wdev->iftype != NL80211_IFTYPE_P2P_CLIENT))
		return;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#ifndef CONFIG_CFG80211_ALLOW_RECONNECT
	if (wdev->sme_state != CFG80211_SME_CONNECTED)
		return;
#endif

	if (wdev->current_bss) {
		cfg80211_unhold_bss(wdev->current_bss);
		cfg80211_put_bss(&wdev->current_bss->pub);
	}

	wdev->current_bss = NULL;
	wdev->sme_state = CFG80211_SME_IDLE;
	wdev->ssid_len = 0;

	if (wdev->conn) {
<<<<<<< HEAD
		const u8 *bssid;
		int ret;

=======
>>>>>>> refs/remotes/origin/cm-10.0
		kfree(wdev->conn->ie);
		wdev->conn->ie = NULL;
		kfree(wdev->conn);
		wdev->conn = NULL;
<<<<<<< HEAD

		/*
		 * If this disconnect was due to a disassoc, we
		 * we might still have an auth BSS around. For
		 * the userspace SME that's currently expected,
		 * but for the kernel SME (nl80211 CONNECT or
		 * wireless extensions) we want to clear up all
		 * state.
		 */
		for (i = 0; i < MAX_AUTH_BSSES; i++) {
			if (!wdev->auth_bsses[i])
				continue;
			bssid = wdev->auth_bsses[i]->pub.bssid;
			ret = __cfg80211_mlme_deauth(rdev, dev, bssid, NULL, 0,
						WLAN_REASON_DEAUTH_LEAVING,
						false);
			WARN(ret, "deauth failed: %d\n", ret);
		}
=======
>>>>>>> refs/remotes/origin/cm-10.0
	}

=======
	if (wdev->current_bss) {
		cfg80211_unhold_bss(wdev->current_bss);
		cfg80211_put_bss(wdev->wiphy, &wdev->current_bss->pub);
	}

	wdev->current_bss = NULL;
	wdev->ssid_len = 0;

>>>>>>> refs/remotes/origin/master
	nl80211_send_disconnected(rdev, dev, reason, ie, ie_len, from_ap);

	/*
	 * Delete all the keys ... pairwise keys can't really
	 * exist any more anyway, but default keys might.
	 */
	if (rdev->ops->del_key)
		for (i = 0; i < 6; i++)
<<<<<<< HEAD
			rdev->ops->del_key(wdev->wiphy, dev, i, false, NULL);
=======
			rdev_del_key(rdev, dev, i, false, NULL);
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_CFG80211_WEXT
	memset(&wrqu, 0, sizeof(wrqu));
	wrqu.ap_addr.sa_family = ARPHRD_ETHER;
	wireless_send_event(dev, SIOCGIWAP, &wrqu, NULL);
	wdev->wext.connect.ssid_len = 0;
#endif

	schedule_work(&cfg80211_disconnect_work);
}

void cfg80211_disconnected(struct net_device *dev, u16 reason,
			   u8 *ie, size_t ie_len, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct cfg80211_event *ev;
	unsigned long flags;

<<<<<<< HEAD
	CFG80211_DEV_WARN_ON(wdev->sme_state != CFG80211_SME_CONNECTED);

=======
>>>>>>> refs/remotes/origin/master
	ev = kzalloc(sizeof(*ev) + ie_len, gfp);
	if (!ev)
		return;

	ev->type = EVENT_DISCONNECTED;
	ev->dc.ie = ((u8 *)ev) + sizeof(*ev);
	ev->dc.ie_len = ie_len;
	memcpy((void *)ev->dc.ie, ie, ie_len);
	ev->dc.reason = reason;

	spin_lock_irqsave(&wdev->event_lock, flags);
	list_add_tail(&ev->list, &wdev->event_list);
	spin_unlock_irqrestore(&wdev->event_lock, flags);
	queue_work(cfg80211_wq, &rdev->event_work);
}
EXPORT_SYMBOL(cfg80211_disconnected);

<<<<<<< HEAD
int __cfg80211_connect(struct cfg80211_registered_device *rdev,
		       struct net_device *dev,
		       struct cfg80211_connect_params *connect,
		       struct cfg80211_cached_keys *connkeys,
		       const u8 *prev_bssid)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_bss *bss = NULL;
=======
/*
 * API calls for nl80211/wext compatibility code
 */
int cfg80211_connect(struct cfg80211_registered_device *rdev,
		     struct net_device *dev,
		     struct cfg80211_connect_params *connect,
		     struct cfg80211_cached_keys *connkeys,
		     const u8 *prev_bssid)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
>>>>>>> refs/remotes/origin/master
	int err;

	ASSERT_WDEV_LOCK(wdev);

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#ifndef CONFIG_CFG80211_ALLOW_RECONNECT
	if (wdev->sme_state != CFG80211_SME_IDLE)
		return -EALREADY;

	if (WARN_ON(wdev->connect_keys)) {
#else
	if (wdev->connect_keys) {
#endif
<<<<<<< HEAD
=======
	if (WARN_ON(wdev->connect_keys)) {
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		kfree(wdev->connect_keys);
		wdev->connect_keys = NULL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
	cfg80211_oper_and_ht_capa(&connect->ht_capa_mask,
				  rdev->wiphy.ht_capa_mod_mask);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	cfg80211_oper_and_ht_capa(&connect->ht_capa_mask,
				  rdev->wiphy.ht_capa_mod_mask);

>>>>>>> refs/remotes/origin/master
	if (connkeys && connkeys->def >= 0) {
		int idx;
		u32 cipher;

		idx = connkeys->def;
		cipher = connkeys->params[idx].cipher;
		/* If given a WEP key we may need it for shared key auth */
		if (cipher == WLAN_CIPHER_SUITE_WEP40 ||
		    cipher == WLAN_CIPHER_SUITE_WEP104) {
			connect->key_idx = idx;
			connect->key = connkeys->params[idx].key;
			connect->key_len = connkeys->params[idx].key_len;

			/*
			 * If ciphers are not set (e.g. when going through
			 * iwconfig), we have to set them appropriately here.
			 */
			if (connect->crypto.cipher_group == 0)
				connect->crypto.cipher_group = cipher;

			if (connect->crypto.n_ciphers_pairwise == 0) {
				connect->crypto.n_ciphers_pairwise = 1;
				connect->crypto.ciphers_pairwise[0] = cipher;
			}
		}
	}

<<<<<<< HEAD
	if (!rdev->ops->connect) {
		if (!rdev->ops->auth || !rdev->ops->assoc)
			return -EOPNOTSUPP;

		if (WARN_ON(wdev->conn))
			return -EINPROGRESS;

		wdev->conn = kzalloc(sizeof(*wdev->conn), GFP_KERNEL);
		if (!wdev->conn)
			return -ENOMEM;

		/*
		 * Copy all parameters, and treat explicitly IEs, BSSID, SSID.
		 */
		memcpy(&wdev->conn->params, connect, sizeof(*connect));
		if (connect->bssid) {
			wdev->conn->params.bssid = wdev->conn->bssid;
			memcpy(wdev->conn->bssid, connect->bssid, ETH_ALEN);
		}

		if (connect->ie) {
			wdev->conn->ie = kmemdup(connect->ie, connect->ie_len,
						GFP_KERNEL);
			wdev->conn->params.ie = wdev->conn->ie;
			if (!wdev->conn->ie) {
				kfree(wdev->conn);
				wdev->conn = NULL;
				return -ENOMEM;
			}
		}

		if (connect->auth_type == NL80211_AUTHTYPE_AUTOMATIC) {
			wdev->conn->auto_auth = true;
			/* start with open system ... should mostly work */
			wdev->conn->params.auth_type =
				NL80211_AUTHTYPE_OPEN_SYSTEM;
		} else {
			wdev->conn->auto_auth = false;
		}

		memcpy(wdev->ssid, connect->ssid, connect->ssid_len);
		wdev->ssid_len = connect->ssid_len;
		wdev->conn->params.ssid = wdev->ssid;
		wdev->conn->params.ssid_len = connect->ssid_len;

		/* see if we have the bss already */
		bss = cfg80211_get_conn_bss(wdev);

		wdev->sme_state = CFG80211_SME_CONNECTING;
		wdev->connect_keys = connkeys;

		if (prev_bssid) {
			memcpy(wdev->conn->prev_bssid, prev_bssid, ETH_ALEN);
			wdev->conn->prev_bssid_valid = true;
		}

		/* we're good if we have a matching bss struct */
		if (bss) {
			wdev->conn->state = CFG80211_CONN_AUTHENTICATE_NEXT;
			err = cfg80211_conn_do_work(wdev);
			cfg80211_put_bss(bss);
		} else {
			/* otherwise we'll need to scan for the AP first */
			err = cfg80211_conn_scan(wdev);
			/*
			 * If we can't scan right now, then we need to scan again
			 * after the current scan finished, since the parameters
			 * changed (unless we find a good AP anyway).
			 */
			if (err == -EBUSY) {
				err = 0;
				wdev->conn->state = CFG80211_CONN_SCAN_AGAIN;
			}
		}
		if (err) {
			kfree(wdev->conn->ie);
			kfree(wdev->conn);
			wdev->conn = NULL;
			wdev->sme_state = CFG80211_SME_IDLE;
			wdev->connect_keys = NULL;
			wdev->ssid_len = 0;
		}

		return err;
	} else {
		wdev->sme_state = CFG80211_SME_CONNECTING;
		wdev->connect_keys = connkeys;
		err = rdev->ops->connect(&rdev->wiphy, dev, connect);
		if (err) {
			wdev->connect_keys = NULL;
			wdev->sme_state = CFG80211_SME_IDLE;
			return err;
		}

		memcpy(wdev->ssid, connect->ssid, connect->ssid_len);
		wdev->ssid_len = connect->ssid_len;

		return 0;
	}
}

int cfg80211_connect(struct cfg80211_registered_device *rdev,
		     struct net_device *dev,
		     struct cfg80211_connect_params *connect,
		     struct cfg80211_cached_keys *connkeys)
{
	int err;

	mutex_lock(&rdev->devlist_mtx);
	wdev_lock(dev->ieee80211_ptr);
	err = __cfg80211_connect(rdev, dev, connect, connkeys, NULL);
	wdev_unlock(dev->ieee80211_ptr);
	mutex_unlock(&rdev->devlist_mtx);

	return err;
}

int __cfg80211_disconnect(struct cfg80211_registered_device *rdev,
			  struct net_device *dev, u16 reason, bool wextev)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	int err;

	ASSERT_WDEV_LOCK(wdev);

	if (wdev->sme_state == CFG80211_SME_IDLE)
		return -EINVAL;

	kfree(wdev->connect_keys);
	wdev->connect_keys = NULL;

	if (!rdev->ops->disconnect) {
		if (!rdev->ops->deauth)
			return -EOPNOTSUPP;

		/* was it connected by userspace SME? */
		if (!wdev->conn) {
			cfg80211_mlme_down(rdev, dev);
			return 0;
		}

		if (wdev->sme_state == CFG80211_SME_CONNECTING &&
		    (wdev->conn->state == CFG80211_CONN_SCANNING ||
		     wdev->conn->state == CFG80211_CONN_SCAN_AGAIN)) {
			wdev->sme_state = CFG80211_SME_IDLE;
			kfree(wdev->conn->ie);
			kfree(wdev->conn);
			wdev->conn = NULL;
			wdev->ssid_len = 0;
			return 0;
		}

		/* wdev->conn->params.bssid must be set if > SCANNING */
		err = __cfg80211_mlme_deauth(rdev, dev,
					     wdev->conn->params.bssid,
					     NULL, 0, reason, false);
		if (err)
			return err;
	} else {
		err = rdev->ops->disconnect(&rdev->wiphy, dev, reason);
		if (err)
			return err;
	}

	if (wdev->sme_state == CFG80211_SME_CONNECTED)
		__cfg80211_disconnected(dev, NULL, 0, 0, false);
	else if (wdev->sme_state == CFG80211_SME_CONNECTING)
		__cfg80211_connect_result(dev, NULL, NULL, 0, NULL, 0,
					  WLAN_STATUS_UNSPECIFIED_FAILURE,
					  wextev, NULL);

	return 0;
}

int cfg80211_disconnect(struct cfg80211_registered_device *rdev,
			struct net_device *dev,
			u16 reason, bool wextev)
{
	int err;

	wdev_lock(dev->ieee80211_ptr);
	err = __cfg80211_disconnect(rdev, dev, reason, wextev);
	wdev_unlock(dev->ieee80211_ptr);

	return err;
}

<<<<<<< HEAD
void cfg80211_sme_disassoc(struct net_device *dev, int idx)
=======
void cfg80211_sme_disassoc(struct net_device *dev,
			   struct cfg80211_internal_bss *bss)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	u8 bssid[ETH_ALEN];

	ASSERT_WDEV_LOCK(wdev);

	if (!wdev->conn)
		return;

	if (wdev->conn->state == CFG80211_CONN_IDLE)
		return;

	/*
	 * Ok, so the association was made by this SME -- we don't
	 * want it any more so deauthenticate too.
	 */

<<<<<<< HEAD
	if (!wdev->auth_bsses[idx])
		return;

	memcpy(bssid, wdev->auth_bsses[idx]->pub.bssid, ETH_ALEN);
	if (__cfg80211_mlme_deauth(rdev, dev, bssid,
				   NULL, 0, WLAN_REASON_DEAUTH_LEAVING,
				   false)) {
		/* whatever -- assume gone anyway */
		cfg80211_unhold_bss(wdev->auth_bsses[idx]);
		cfg80211_put_bss(&wdev->auth_bsses[idx]->pub);
		wdev->auth_bsses[idx] = NULL;
	}
=======
	memcpy(bssid, bss->pub.bssid, ETH_ALEN);

	__cfg80211_mlme_deauth(rdev, dev, bssid, NULL, 0,
			       WLAN_REASON_DEAUTH_LEAVING, false);
>>>>>>> refs/remotes/origin/cm-10.0
}
=======
	wdev->connect_keys = connkeys;
	memcpy(wdev->ssid, connect->ssid, connect->ssid_len);
	wdev->ssid_len = connect->ssid_len;

	if (!rdev->ops->connect)
		err = cfg80211_sme_connect(wdev, connect, prev_bssid);
	else
		err = rdev_connect(rdev, dev, connect);

	if (err) {
		wdev->connect_keys = NULL;
		wdev->ssid_len = 0;
		return err;
	}

	return 0;
}

int cfg80211_disconnect(struct cfg80211_registered_device *rdev,
			struct net_device *dev, u16 reason, bool wextev)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	int err = 0;

	ASSERT_WDEV_LOCK(wdev);

	kfree(wdev->connect_keys);
	wdev->connect_keys = NULL;

	if (wdev->conn)
		err = cfg80211_sme_disconnect(wdev, reason);
	else if (!rdev->ops->disconnect)
		cfg80211_mlme_down(rdev, dev);
	else if (wdev->current_bss)
		err = rdev_disconnect(rdev, dev, reason);

	return err;
}
>>>>>>> refs/remotes/origin/master
