/*
 * This file is part of wl1271
 *
 * Copyright (C) 2009 Nokia Corporation
 *
 * Contact: Luciano Coelho <luciano.coelho@nokia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

<<<<<<< HEAD
=======
#include "debug.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include "init.h"
#include "wl12xx_80211.h"
#include "acx.h"
#include "cmd.h"
#include "reg.h"
#include "tx.h"
#include "io.h"

<<<<<<< HEAD
int wl1271_sta_init_templates_config(struct wl1271 *wl)
{
	int ret, i;

	/* send empty templates for fw memory reservation */
	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_CFG_PROBE_REQ_2_4, NULL,
=======
int wl1271_init_templates_config(struct wl1271 *wl)
{
	int ret, i;
	size_t max_size;

	/* send empty templates for fw memory reservation */
	ret = wl1271_cmd_template_set(wl, WL12XX_INVALID_ROLE_ID,
				      CMD_TEMPL_CFG_PROBE_REQ_2_4, NULL,
>>>>>>> refs/remotes/origin/cm-10.0
				      WL1271_CMD_TEMPL_MAX_SIZE,
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_CFG_PROBE_REQ_5,
=======
	ret = wl1271_cmd_template_set(wl, WL12XX_INVALID_ROLE_ID,
				      CMD_TEMPL_CFG_PROBE_REQ_5,
>>>>>>> refs/remotes/origin/cm-10.0
				      NULL, WL1271_CMD_TEMPL_MAX_SIZE, 0,
				      WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_NULL_DATA, NULL,
=======
	ret = wl1271_cmd_template_set(wl, WL12XX_INVALID_ROLE_ID,
				      CMD_TEMPL_NULL_DATA, NULL,
>>>>>>> refs/remotes/origin/cm-10.0
				      sizeof(struct wl12xx_null_data_template),
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_PS_POLL, NULL,
=======
	ret = wl1271_cmd_template_set(wl, WL12XX_INVALID_ROLE_ID,
				      CMD_TEMPL_PS_POLL, NULL,
>>>>>>> refs/remotes/origin/cm-10.0
				      sizeof(struct wl12xx_ps_poll_template),
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_QOS_NULL_DATA, NULL,
				      sizeof
				      (struct wl12xx_qos_null_data_template),
=======
	ret = wl1271_cmd_template_set(wl, WL12XX_INVALID_ROLE_ID,
				      CMD_TEMPL_QOS_NULL_DATA, NULL,
				      sizeof
				      (struct ieee80211_qos_hdr),
>>>>>>> refs/remotes/origin/cm-10.0
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_PROBE_RESPONSE, NULL,
				      sizeof
				      (struct wl12xx_probe_resp_template),
=======
	ret = wl1271_cmd_template_set(wl, WL12XX_INVALID_ROLE_ID,
				      CMD_TEMPL_PROBE_RESPONSE, NULL,
				      WL1271_CMD_TEMPL_DFLT_SIZE,
>>>>>>> refs/remotes/origin/cm-10.0
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_BEACON, NULL,
				      sizeof
				      (struct wl12xx_beacon_template),
=======
	ret = wl1271_cmd_template_set(wl, WL12XX_INVALID_ROLE_ID,
				      CMD_TEMPL_BEACON, NULL,
				      WL1271_CMD_TEMPL_DFLT_SIZE,
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

	max_size = sizeof(struct wl12xx_arp_rsp_template) +
		   WL1271_EXTRA_SPACE_MAX;
	ret = wl1271_cmd_template_set(wl, WL12XX_INVALID_ROLE_ID,
				      CMD_TEMPL_ARP_RSP, NULL,
				      max_size,
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

	/*
	 * Put very large empty placeholders for all templates. These
	 * reserve memory for later.
	 */
	ret = wl1271_cmd_template_set(wl, WL12XX_INVALID_ROLE_ID,
				      CMD_TEMPL_AP_PROBE_RESPONSE, NULL,
				      WL1271_CMD_TEMPL_MAX_SIZE,
>>>>>>> refs/remotes/origin/cm-10.0
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_ARP_RSP, NULL,
				      sizeof
				      (struct wl12xx_arp_rsp_template),
=======
	ret = wl1271_cmd_template_set(wl, WL12XX_INVALID_ROLE_ID,
				      CMD_TEMPL_AP_BEACON, NULL,
				      WL1271_CMD_TEMPL_MAX_SIZE,
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

	ret = wl1271_cmd_template_set(wl, WL12XX_INVALID_ROLE_ID,
				      CMD_TEMPL_DEAUTH_AP, NULL,
				      sizeof
				      (struct wl12xx_disconn_template),
>>>>>>> refs/remotes/origin/cm-10.0
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

	for (i = 0; i < CMD_TEMPL_KLV_IDX_MAX; i++) {
<<<<<<< HEAD
		ret = wl1271_cmd_template_set(wl, CMD_TEMPL_KLV, NULL,
					      WL1271_CMD_TEMPL_MAX_SIZE, i,
					      WL1271_RATE_AUTOMATIC);
=======
		ret = wl1271_cmd_template_set(wl, WL12XX_INVALID_ROLE_ID,
					      CMD_TEMPL_KLV, NULL,
					      sizeof(struct ieee80211_qos_hdr),
					      i, WL1271_RATE_AUTOMATIC);
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret < 0)
			return ret;
	}

	return 0;
}

<<<<<<< HEAD
static int wl1271_ap_init_deauth_template(struct wl1271 *wl)
{
	struct wl12xx_disconn_template *tmpl;
	int ret;
=======
static int wl1271_ap_init_deauth_template(struct wl1271 *wl,
					  struct wl12xx_vif *wlvif)
{
	struct wl12xx_disconn_template *tmpl;
	int ret;
	u32 rate;
>>>>>>> refs/remotes/origin/cm-10.0

	tmpl = kzalloc(sizeof(*tmpl), GFP_KERNEL);
	if (!tmpl) {
		ret = -ENOMEM;
		goto out;
	}

	tmpl->header.frame_ctl = cpu_to_le16(IEEE80211_FTYPE_MGMT |
					     IEEE80211_STYPE_DEAUTH);

<<<<<<< HEAD
	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_DEAUTH_AP,
				      tmpl, sizeof(*tmpl), 0,
				      wl1271_tx_min_rate_get(wl));
=======
	rate = wl1271_tx_min_rate_get(wl, wlvif->basic_rate_set);
	ret = wl1271_cmd_template_set(wl, wlvif->role_id,
				      CMD_TEMPL_DEAUTH_AP,
				      tmpl, sizeof(*tmpl), 0, rate);
>>>>>>> refs/remotes/origin/cm-10.0

out:
	kfree(tmpl);
	return ret;
}

<<<<<<< HEAD
static int wl1271_ap_init_null_template(struct wl1271 *wl)
{
	struct ieee80211_hdr_3addr *nullfunc;
	int ret;
=======
static int wl1271_ap_init_null_template(struct wl1271 *wl,
					struct ieee80211_vif *vif)
{
	struct wl12xx_vif *wlvif = wl12xx_vif_to_data(vif);
	struct ieee80211_hdr_3addr *nullfunc;
	int ret;
	u32 rate;
>>>>>>> refs/remotes/origin/cm-10.0

	nullfunc = kzalloc(sizeof(*nullfunc), GFP_KERNEL);
	if (!nullfunc) {
		ret = -ENOMEM;
		goto out;
	}

	nullfunc->frame_control = cpu_to_le16(IEEE80211_FTYPE_DATA |
					      IEEE80211_STYPE_NULLFUNC |
					      IEEE80211_FCTL_FROMDS);

	/* nullfunc->addr1 is filled by FW */

<<<<<<< HEAD
	memcpy(nullfunc->addr2, wl->mac_addr, ETH_ALEN);
	memcpy(nullfunc->addr3, wl->mac_addr, ETH_ALEN);

	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_NULL_DATA, nullfunc,
				      sizeof(*nullfunc), 0,
				      wl1271_tx_min_rate_get(wl));
=======
	memcpy(nullfunc->addr2, vif->addr, ETH_ALEN);
	memcpy(nullfunc->addr3, vif->addr, ETH_ALEN);

	rate = wl1271_tx_min_rate_get(wl, wlvif->basic_rate_set);
	ret = wl1271_cmd_template_set(wl, wlvif->role_id,
				      CMD_TEMPL_NULL_DATA, nullfunc,
				      sizeof(*nullfunc), 0, rate);
>>>>>>> refs/remotes/origin/cm-10.0

out:
	kfree(nullfunc);
	return ret;
}

<<<<<<< HEAD
static int wl1271_ap_init_qos_null_template(struct wl1271 *wl)
{
	struct ieee80211_qos_hdr *qosnull;
	int ret;
=======
static int wl1271_ap_init_qos_null_template(struct wl1271 *wl,
					    struct ieee80211_vif *vif)
{
	struct wl12xx_vif *wlvif = wl12xx_vif_to_data(vif);
	struct ieee80211_qos_hdr *qosnull;
	int ret;
	u32 rate;
>>>>>>> refs/remotes/origin/cm-10.0

	qosnull = kzalloc(sizeof(*qosnull), GFP_KERNEL);
	if (!qosnull) {
		ret = -ENOMEM;
		goto out;
	}

	qosnull->frame_control = cpu_to_le16(IEEE80211_FTYPE_DATA |
					     IEEE80211_STYPE_QOS_NULLFUNC |
					     IEEE80211_FCTL_FROMDS);

	/* qosnull->addr1 is filled by FW */

<<<<<<< HEAD
	memcpy(qosnull->addr2, wl->mac_addr, ETH_ALEN);
	memcpy(qosnull->addr3, wl->mac_addr, ETH_ALEN);

	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_QOS_NULL_DATA, qosnull,
				      sizeof(*qosnull), 0,
				      wl1271_tx_min_rate_get(wl));
=======
	memcpy(qosnull->addr2, vif->addr, ETH_ALEN);
	memcpy(qosnull->addr3, vif->addr, ETH_ALEN);

	rate = wl1271_tx_min_rate_get(wl, wlvif->basic_rate_set);
	ret = wl1271_cmd_template_set(wl, wlvif->role_id,
				      CMD_TEMPL_QOS_NULL_DATA, qosnull,
				      sizeof(*qosnull), 0, rate);
>>>>>>> refs/remotes/origin/cm-10.0

out:
	kfree(qosnull);
	return ret;
}

<<<<<<< HEAD
static int wl1271_ap_init_templates_config(struct wl1271 *wl)
{
	int ret;

	/*
	 * Put very large empty placeholders for all templates. These
	 * reserve memory for later.
	 */
	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_AP_PROBE_RESPONSE, NULL,
				      sizeof
				      (struct wl12xx_probe_resp_template),
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_AP_BEACON, NULL,
				      sizeof
				      (struct wl12xx_beacon_template),
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_DEAUTH_AP, NULL,
				      sizeof
				      (struct wl12xx_disconn_template),
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_NULL_DATA, NULL,
				      sizeof(struct wl12xx_null_data_template),
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

	ret = wl1271_cmd_template_set(wl, CMD_TEMPL_QOS_NULL_DATA, NULL,
				      sizeof
				      (struct wl12xx_qos_null_data_template),
				      0, WL1271_RATE_AUTOMATIC);
	if (ret < 0)
		return ret;

	return 0;
}

static int wl1271_init_rx_config(struct wl1271 *wl, u32 config, u32 filter)
=======
static int wl12xx_init_rx_config(struct wl1271 *wl)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int ret;

	ret = wl1271_acx_rx_msdu_life_time(wl);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	ret = wl1271_acx_rx_config(wl, config, filter);
	if (ret < 0)
		return ret;

	return 0;
}

int wl1271_init_phy_config(struct wl1271 *wl)
{
	int ret;

	ret = wl1271_acx_pd_threshold(wl);
	if (ret < 0)
		return ret;

	ret = wl1271_acx_slot(wl, DEFAULT_SLOT_TIME);
	if (ret < 0)
		return ret;

	ret = wl1271_acx_service_period_timeout(wl);
	if (ret < 0)
		return ret;

	ret = wl1271_acx_rts_threshold(wl, wl->hw->wiphy->rts_threshold);
=======
	return 0;
}

static int wl12xx_init_phy_vif_config(struct wl1271 *wl,
					    struct wl12xx_vif *wlvif)
{
	int ret;

	ret = wl1271_acx_slot(wl, wlvif, DEFAULT_SLOT_TIME);
	if (ret < 0)
		return ret;

	ret = wl1271_acx_service_period_timeout(wl, wlvif);
	if (ret < 0)
		return ret;

	ret = wl1271_acx_rts_threshold(wl, wlvif, wl->hw->wiphy->rts_threshold);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return ret;

	return 0;
}

<<<<<<< HEAD
static int wl1271_init_beacon_filter(struct wl1271 *wl)
{
	int ret;

	/* disable beacon filtering at this stage */
	ret = wl1271_acx_beacon_filter_opt(wl, false);
	if (ret < 0)
		return ret;

	ret = wl1271_acx_beacon_filter_table(wl);
=======
static int wl1271_init_sta_beacon_filter(struct wl1271 *wl,
					 struct wl12xx_vif *wlvif)
{
	int ret;

	ret = wl1271_acx_beacon_filter_table(wl, wlvif);
	if (ret < 0)
		return ret;

	/* enable beacon filtering */
	ret = wl1271_acx_beacon_filter_opt(wl, wlvif, true);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return ret;

	return 0;
}

int wl1271_init_pta(struct wl1271 *wl)
{
	int ret;

<<<<<<< HEAD
	if (wl->bss_type == BSS_TYPE_AP_BSS)
		ret = wl1271_acx_ap_sg_cfg(wl);
	else
		ret = wl1271_acx_sta_sg_cfg(wl);
=======
	ret = wl12xx_acx_sg_cfg(wl);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return ret;

	ret = wl1271_acx_sg_enable(wl, wl->sg_enabled);
	if (ret < 0)
		return ret;

	return 0;
}

int wl1271_init_energy_detection(struct wl1271 *wl)
{
	int ret;

	ret = wl1271_acx_cca_threshold(wl);
	if (ret < 0)
		return ret;

	return 0;
}

<<<<<<< HEAD
static int wl1271_init_beacon_broadcast(struct wl1271 *wl)
{
	int ret;

	ret = wl1271_acx_bcn_dtim_options(wl);
=======
static int wl1271_init_beacon_broadcast(struct wl1271 *wl,
					struct wl12xx_vif *wlvif)
{
	int ret;

	ret = wl1271_acx_bcn_dtim_options(wl, wlvif);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return ret;

	return 0;
}

<<<<<<< HEAD
static int wl1271_sta_hw_init(struct wl1271 *wl)
{
	int ret;

	if (wl->chip.id != CHIP_ID_1283_PG20) {
		ret = wl1271_cmd_ext_radio_parms(wl);
		if (ret < 0)
			return ret;
	}

	/* PS config */
	ret = wl1271_acx_config_ps(wl);
	if (ret < 0)
		return ret;

	ret = wl1271_sta_init_templates_config(wl);
	if (ret < 0)
		return ret;

	ret = wl1271_acx_group_address_tbl(wl, true, NULL, 0);
	if (ret < 0)
		return ret;

	/* Initialize connection monitoring thresholds */
	ret = wl1271_acx_conn_monit_params(wl, false);
	if (ret < 0)
		return ret;

	/* Beacon filtering */
	ret = wl1271_init_beacon_filter(wl);
=======
static int wl12xx_init_fwlog(struct wl1271 *wl)
{
	int ret;

	if (wl->quirks & WL12XX_QUIRK_FWLOG_NOT_IMPLEMENTED)
		return 0;

	ret = wl12xx_cmd_config_fwlog(wl);
	if (ret < 0)
		return ret;

	return 0;
}

/* generic sta initialization (non vif-specific) */
static int wl1271_sta_hw_init(struct wl1271 *wl, struct wl12xx_vif *wlvif)
{
	int ret;

	/* PS config */
	ret = wl12xx_acx_config_ps(wl, wlvif);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return ret;

	/* FM WLAN coexistence */
	ret = wl1271_acx_fm_coex(wl);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	/* Beacons and broadcast settings */
	ret = wl1271_init_beacon_broadcast(wl);
	if (ret < 0)
		return ret;

	/* Configure for ELP power saving */
	ret = wl1271_acx_sleep_auth(wl, WL1271_PSM_ELP);
	if (ret < 0)
		return ret;

	/* Configure rssi/snr averaging weights */
	ret = wl1271_acx_rssi_snr_avg_weights(wl);
	if (ret < 0)
		return ret;

	ret = wl1271_acx_sta_rate_policies(wl);
	if (ret < 0)
		return ret;

	ret = wl1271_acx_sta_mem_cfg(wl);
=======
	ret = wl1271_acx_sta_rate_policies(wl, wlvif);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return ret;

	return 0;
}

<<<<<<< HEAD
static int wl1271_sta_hw_init_post_mem(struct wl1271 *wl)
{
	int ret, i;

	ret = wl1271_cmd_set_sta_default_wep_key(wl, wl->default_key);
	if (ret < 0) {
		wl1271_warning("couldn't set default key");
		return ret;
	}

	/* disable all keep-alive templates */
	for (i = 0; i < CMD_TEMPL_KLV_IDX_MAX; i++) {
		ret = wl1271_acx_keep_alive_config(wl, i,
=======
static int wl1271_sta_hw_init_post_mem(struct wl1271 *wl,
				       struct ieee80211_vif *vif)
{
	struct wl12xx_vif *wlvif = wl12xx_vif_to_data(vif);
	int ret, i;

	/* disable all keep-alive templates */
	for (i = 0; i < CMD_TEMPL_KLV_IDX_MAX; i++) {
		ret = wl1271_acx_keep_alive_config(wl, wlvif, i,
>>>>>>> refs/remotes/origin/cm-10.0
						   ACX_KEEP_ALIVE_TPL_INVALID);
		if (ret < 0)
			return ret;
	}

	/* disable the keep-alive feature */
<<<<<<< HEAD
	ret = wl1271_acx_keep_alive_mode(wl, false);
=======
	ret = wl1271_acx_keep_alive_mode(wl, wlvif, false);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return ret;

	return 0;
}

<<<<<<< HEAD
static int wl1271_ap_hw_init(struct wl1271 *wl)
{
	int ret;

	ret = wl1271_ap_init_templates_config(wl);
	if (ret < 0)
		return ret;

	/* Configure for power always on */
	ret = wl1271_acx_sleep_auth(wl, WL1271_PSM_CAM);
	if (ret < 0)
		return ret;

	ret = wl1271_init_ap_rates(wl);
	if (ret < 0)
		return ret;

	ret = wl1271_acx_max_tx_retry(wl);
	if (ret < 0)
		return ret;

	ret = wl1271_acx_ap_mem_cfg(wl);
=======
/* generic ap initialization (non vif-specific) */
static int wl1271_ap_hw_init(struct wl1271 *wl, struct wl12xx_vif *wlvif)
{
	int ret;

	ret = wl1271_init_ap_rates(wl, wlvif);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return ret;

	return 0;
}

<<<<<<< HEAD
int wl1271_ap_init_templates(struct wl1271 *wl)
{
	int ret;

	ret = wl1271_ap_init_deauth_template(wl);
	if (ret < 0)
		return ret;

	ret = wl1271_ap_init_null_template(wl);
	if (ret < 0)
		return ret;

	ret = wl1271_ap_init_qos_null_template(wl);
=======
int wl1271_ap_init_templates(struct wl1271 *wl, struct ieee80211_vif *vif)
{
	struct wl12xx_vif *wlvif = wl12xx_vif_to_data(vif);
	int ret;

	ret = wl1271_ap_init_deauth_template(wl, wlvif);
	if (ret < 0)
		return ret;

	ret = wl1271_ap_init_null_template(wl, vif);
	if (ret < 0)
		return ret;

	ret = wl1271_ap_init_qos_null_template(wl, vif);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return ret;

	/*
	 * when operating as AP we want to receive external beacons for
	 * configuring ERP protection.
	 */
<<<<<<< HEAD
	ret = wl1271_acx_set_ap_beacon_filter(wl, false);
=======
	ret = wl1271_acx_beacon_filter_opt(wl, wlvif, false);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return ret;

	return 0;
}

<<<<<<< HEAD
static int wl1271_ap_hw_init_post_mem(struct wl1271 *wl)
{
	return wl1271_ap_init_templates(wl);
}

int wl1271_init_ap_rates(struct wl1271 *wl)
=======
static int wl1271_ap_hw_init_post_mem(struct wl1271 *wl,
				      struct ieee80211_vif *vif)
{
	return wl1271_ap_init_templates(wl, vif);
}

int wl1271_init_ap_rates(struct wl1271 *wl, struct wl12xx_vif *wlvif)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int i, ret;
	struct conf_tx_rate_class rc;
	u32 supported_rates;

<<<<<<< HEAD
	wl1271_debug(DEBUG_AP, "AP basic rate set: 0x%x", wl->basic_rate_set);

	if (wl->basic_rate_set == 0)
		return -EINVAL;

	rc.enabled_rates = wl->basic_rate_set;
	rc.long_retry_limit = 10;
	rc.short_retry_limit = 10;
	rc.aflags = 0;
	ret = wl1271_acx_ap_rate_policy(wl, &rc, ACX_TX_AP_MODE_MGMT_RATE);
=======
	wl1271_debug(DEBUG_AP, "AP basic rate set: 0x%x",
		     wlvif->basic_rate_set);

	if (wlvif->basic_rate_set == 0)
		return -EINVAL;

	rc.enabled_rates = wlvif->basic_rate_set;
	rc.long_retry_limit = 10;
	rc.short_retry_limit = 10;
	rc.aflags = 0;
	ret = wl1271_acx_ap_rate_policy(wl, &rc, wlvif->ap.mgmt_rate_idx);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return ret;

	/* use the min basic rate for AP broadcast/multicast */
<<<<<<< HEAD
	rc.enabled_rates = wl1271_tx_min_rate_get(wl);
	rc.short_retry_limit = 10;
	rc.long_retry_limit = 10;
	rc.aflags = 0;
	ret = wl1271_acx_ap_rate_policy(wl, &rc, ACX_TX_AP_MODE_BCST_RATE);
=======
	rc.enabled_rates = wl1271_tx_min_rate_get(wl, wlvif->basic_rate_set);
	rc.short_retry_limit = 10;
	rc.long_retry_limit = 10;
	rc.aflags = 0;
	ret = wl1271_acx_ap_rate_policy(wl, &rc, wlvif->ap.bcast_rate_idx);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return ret;

	/*
	 * If the basic rates contain OFDM rates, use OFDM only
	 * rates for unicast TX as well. Else use all supported rates.
	 */
<<<<<<< HEAD
	if ((wl->basic_rate_set & CONF_TX_OFDM_RATES))
=======
	if ((wlvif->basic_rate_set & CONF_TX_OFDM_RATES))
>>>>>>> refs/remotes/origin/cm-10.0
		supported_rates = CONF_TX_OFDM_RATES;
	else
		supported_rates = CONF_TX_AP_ENABLED_RATES;

<<<<<<< HEAD
=======
	/* unconditionally enable HT rates */
	supported_rates |= CONF_TX_MCS_RATES;

>>>>>>> refs/remotes/origin/cm-10.0
	/* configure unicast TX rate classes */
	for (i = 0; i < wl->conf.tx.ac_conf_count; i++) {
		rc.enabled_rates = supported_rates;
		rc.short_retry_limit = 10;
		rc.long_retry_limit = 10;
		rc.aflags = 0;
<<<<<<< HEAD
		ret = wl1271_acx_ap_rate_policy(wl, &rc, i);
=======
		ret = wl1271_acx_ap_rate_policy(wl, &rc,
						wlvif->ap.ucast_rate_idx[i]);
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret < 0)
			return ret;
	}

	return 0;
}

<<<<<<< HEAD
static void wl1271_check_ba_support(struct wl1271 *wl)
{
	/* validate FW cose ver x.x.x.50-60.x */
	if ((wl->chip.fw_ver[3] >= WL12XX_BA_SUPPORT_FW_COST_VER2_START) &&
	    (wl->chip.fw_ver[3] < WL12XX_BA_SUPPORT_FW_COST_VER2_END)) {
		wl->ba_support = true;
		return;
	}

	wl->ba_support = false;
}

static int wl1271_set_ba_policies(struct wl1271 *wl)
{
	u8 tid_index;
	int ret = 0;

	/* Reset the BA RX indicators */
	wl->ba_rx_bitmap = 0;

	/* validate that FW support BA */
	wl1271_check_ba_support(wl);

	if (wl->ba_support)
		/* 802.11n initiator BA session setting */
		for (tid_index = 0; tid_index < CONF_TX_MAX_TID_COUNT;
		     ++tid_index) {
			ret = wl1271_acx_set_ba_session(wl, WLAN_BACK_INITIATOR,
							tid_index, true);
			if (ret < 0)
				break;
		}

	return ret;
=======
static int wl1271_set_ba_policies(struct wl1271 *wl, struct wl12xx_vif *wlvif)
{
	/* Reset the BA RX indicators */
	wlvif->ba_allowed = true;
	wl->ba_rx_session_count = 0;

	/* BA is supported in STA/AP modes */
	if (wlvif->bss_type != BSS_TYPE_AP_BSS &&
	    wlvif->bss_type != BSS_TYPE_STA_BSS) {
		wlvif->ba_support = false;
		return 0;
	}

	wlvif->ba_support = true;

	/* 802.11n initiator BA session setting */
	return wl12xx_acx_set_ba_initiator_policy(wl, wlvif);
>>>>>>> refs/remotes/origin/cm-10.0
}

int wl1271_chip_specific_init(struct wl1271 *wl)
{
	int ret = 0;

	if (wl->chip.id == CHIP_ID_1283_PG20) {
		u32 host_cfg_bitmap = HOST_IF_CFG_RX_FIFO_ENABLE;

<<<<<<< HEAD
		if (wl->quirks & WL12XX_QUIRK_BLOCKSIZE_ALIGNMENT)
=======
		if (!(wl->quirks & WL12XX_QUIRK_NO_BLOCKSIZE_ALIGNMENT))
>>>>>>> refs/remotes/origin/cm-10.0
			/* Enable SDIO padding */
			host_cfg_bitmap |= HOST_IF_CFG_TX_PAD_TO_SDIO_BLK;

		/* Must be before wl1271_acx_init_mem_config() */
		ret = wl1271_acx_host_if_cfg_bitmap(wl, host_cfg_bitmap);
		if (ret < 0)
			goto out;
	}
out:
	return ret;
}

<<<<<<< HEAD

int wl1271_hw_init(struct wl1271 *wl)
{
	struct conf_tx_ac_category *conf_ac;
	struct conf_tx_tid *conf_tid;
	int ret, i;
	bool is_ap = (wl->bss_type == BSS_TYPE_AP_BSS);

	if (wl->chip.id == CHIP_ID_1283_PG20)
		ret = wl128x_cmd_general_parms(wl);
	else
		ret = wl1271_cmd_general_parms(wl);
	if (ret < 0)
		return ret;

	if (wl->chip.id == CHIP_ID_1283_PG20)
		ret = wl128x_cmd_radio_parms(wl);
	else
		ret = wl1271_cmd_radio_parms(wl);
	if (ret < 0)
		return ret;

=======
/* vif-specifc initialization */
static int wl12xx_init_sta_role(struct wl1271 *wl, struct wl12xx_vif *wlvif)
{
	int ret;

	ret = wl1271_acx_group_address_tbl(wl, wlvif, true, NULL, 0);
	if (ret < 0)
		return ret;

	/* Initialize connection monitoring thresholds */
	ret = wl1271_acx_conn_monit_params(wl, wlvif, false);
	if (ret < 0)
		return ret;

	/* Beacon filtering */
	ret = wl1271_init_sta_beacon_filter(wl, wlvif);
	if (ret < 0)
		return ret;

	/* Beacons and broadcast settings */
	ret = wl1271_init_beacon_broadcast(wl, wlvif);
	if (ret < 0)
		return ret;

	/* Configure rssi/snr averaging weights */
	ret = wl1271_acx_rssi_snr_avg_weights(wl, wlvif);
	if (ret < 0)
		return ret;

	return 0;
}

/* vif-specific intialization */
static int wl12xx_init_ap_role(struct wl1271 *wl, struct wl12xx_vif *wlvif)
{
	int ret;

	ret = wl1271_acx_ap_max_tx_retry(wl, wlvif);
	if (ret < 0)
		return ret;

	/* initialize Tx power */
	ret = wl1271_acx_tx_power(wl, wlvif, wlvif->power_level);
	if (ret < 0)
		return ret;

	return 0;
}

int wl1271_init_vif_specific(struct wl1271 *wl, struct ieee80211_vif *vif)
{
	struct wl12xx_vif *wlvif = wl12xx_vif_to_data(vif);
	struct conf_tx_ac_category *conf_ac;
	struct conf_tx_tid *conf_tid;
	bool is_ap = (wlvif->bss_type == BSS_TYPE_AP_BSS);
	int ret, i;

	/*
	 * consider all existing roles before configuring psm.
	 * TODO: reconfigure on interface removal.
	 */
	if (!wl->ap_count) {
		if (is_ap) {
			/* Configure for power always on */
			ret = wl1271_acx_sleep_auth(wl, WL1271_PSM_CAM);
			if (ret < 0)
				return ret;
		} else if (!wl->sta_count) {
			/* Configure for ELP power saving */
			ret = wl1271_acx_sleep_auth(wl, WL1271_PSM_ELP);
			if (ret < 0)
				return ret;
		}
	}

	/* Mode specific init */
	if (is_ap) {
		ret = wl1271_ap_hw_init(wl, wlvif);
		if (ret < 0)
			return ret;

		ret = wl12xx_init_ap_role(wl, wlvif);
		if (ret < 0)
			return ret;
	} else {
		ret = wl1271_sta_hw_init(wl, wlvif);
		if (ret < 0)
			return ret;

		ret = wl12xx_init_sta_role(wl, wlvif);
		if (ret < 0)
			return ret;
	}

	wl12xx_init_phy_vif_config(wl, wlvif);

	/* Default TID/AC configuration */
	BUG_ON(wl->conf.tx.tid_conf_count != wl->conf.tx.ac_conf_count);
	for (i = 0; i < wl->conf.tx.tid_conf_count; i++) {
		conf_ac = &wl->conf.tx.ac_conf[i];
		ret = wl1271_acx_ac_cfg(wl, wlvif, conf_ac->ac,
					conf_ac->cw_min, conf_ac->cw_max,
					conf_ac->aifsn, conf_ac->tx_op_limit);
		if (ret < 0)
			return ret;

		conf_tid = &wl->conf.tx.tid_conf[i];
		ret = wl1271_acx_tid_cfg(wl, wlvif,
					 conf_tid->queue_id,
					 conf_tid->channel_type,
					 conf_tid->tsid,
					 conf_tid->ps_scheme,
					 conf_tid->ack_policy,
					 conf_tid->apsd_conf[0],
					 conf_tid->apsd_conf[1]);
		if (ret < 0)
			return ret;
	}

	/* Configure HW encryption */
	ret = wl1271_acx_feature_cfg(wl, wlvif);
	if (ret < 0)
		return ret;

	/* Mode specific init - post mem init */
	if (is_ap)
		ret = wl1271_ap_hw_init_post_mem(wl, vif);
	else
		ret = wl1271_sta_hw_init_post_mem(wl, vif);

	if (ret < 0)
		return ret;

	/* Configure initiator BA sessions policies */
	ret = wl1271_set_ba_policies(wl, wlvif);
	if (ret < 0)
		return ret;

	return 0;
}

int wl1271_hw_init(struct wl1271 *wl)
{
	int ret;

	if (wl->chip.id == CHIP_ID_1283_PG20) {
		ret = wl128x_cmd_general_parms(wl);
		if (ret < 0)
			return ret;
		ret = wl128x_cmd_radio_parms(wl);
		if (ret < 0)
			return ret;
	} else {
		ret = wl1271_cmd_general_parms(wl);
		if (ret < 0)
			return ret;
		ret = wl1271_cmd_radio_parms(wl);
		if (ret < 0)
			return ret;
		ret = wl1271_cmd_ext_radio_parms(wl);
		if (ret < 0)
			return ret;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	/* Chip-specific init */
	ret = wl1271_chip_specific_init(wl);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	/* Mode specific init */
	if (is_ap)
		ret = wl1271_ap_hw_init(wl);
	else
		ret = wl1271_sta_hw_init(wl);

=======
	/* Init templates */
	ret = wl1271_init_templates_config(wl);
	if (ret < 0)
		return ret;

	ret = wl12xx_acx_mem_cfg(wl);
	if (ret < 0)
		return ret;

	/* Configure the FW logger */
	ret = wl12xx_init_fwlog(wl);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return ret;

	/* Bluetooth WLAN coexistence */
	ret = wl1271_init_pta(wl);
	if (ret < 0)
		return ret;

	/* Default memory configuration */
	ret = wl1271_acx_init_mem_config(wl);
	if (ret < 0)
		return ret;

	/* RX config */
<<<<<<< HEAD
	ret = wl1271_init_rx_config(wl,
				    RX_CFG_PROMISCUOUS | RX_CFG_TSF,
				    RX_FILTER_OPTION_DEF);
	/* RX_CONFIG_OPTION_ANY_DST_ANY_BSS,
	   RX_FILTER_OPTION_FILTER_ALL); */
	if (ret < 0)
		goto out_free_memmap;

	/* PHY layer config */
	ret = wl1271_init_phy_config(wl);
=======
	ret = wl12xx_init_rx_config(wl);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		goto out_free_memmap;

	ret = wl1271_acx_dco_itrim_params(wl);
	if (ret < 0)
		goto out_free_memmap;

	/* Configure TX patch complete interrupt behavior */
	ret = wl1271_acx_tx_config_options(wl);
	if (ret < 0)
		goto out_free_memmap;

	/* RX complete interrupt pacing */
	ret = wl1271_acx_init_rx_interrupt(wl);
	if (ret < 0)
		goto out_free_memmap;

	/* Energy detection */
	ret = wl1271_init_energy_detection(wl);
	if (ret < 0)
		goto out_free_memmap;

	/* Default fragmentation threshold */
	ret = wl1271_acx_frag_threshold(wl, wl->hw->wiphy->frag_threshold);
	if (ret < 0)
		goto out_free_memmap;

<<<<<<< HEAD
	/* Default TID/AC configuration */
	BUG_ON(wl->conf.tx.tid_conf_count != wl->conf.tx.ac_conf_count);
	for (i = 0; i < wl->conf.tx.tid_conf_count; i++) {
		conf_ac = &wl->conf.tx.ac_conf[i];
		ret = wl1271_acx_ac_cfg(wl, conf_ac->ac, conf_ac->cw_min,
					conf_ac->cw_max, conf_ac->aifsn,
					conf_ac->tx_op_limit);
		if (ret < 0)
			goto out_free_memmap;

		conf_tid = &wl->conf.tx.tid_conf[i];
		ret = wl1271_acx_tid_cfg(wl, conf_tid->queue_id,
					 conf_tid->channel_type,
					 conf_tid->tsid,
					 conf_tid->ps_scheme,
					 conf_tid->ack_policy,
					 conf_tid->apsd_conf[0],
					 conf_tid->apsd_conf[1]);
		if (ret < 0)
			goto out_free_memmap;
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* Enable data path */
	ret = wl1271_cmd_data_path(wl, 1);
	if (ret < 0)
		goto out_free_memmap;

<<<<<<< HEAD
	/* Configure HW encryption */
	ret = wl1271_acx_feature_cfg(wl);
	if (ret < 0)
		goto out_free_memmap;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* configure PM */
	ret = wl1271_acx_pm_config(wl);
	if (ret < 0)
		goto out_free_memmap;

<<<<<<< HEAD
	/* Mode specific init - post mem init */
	if (is_ap)
		ret = wl1271_ap_hw_init_post_mem(wl);
	else
		ret = wl1271_sta_hw_init_post_mem(wl);

	if (ret < 0)
		goto out_free_memmap;

	/* Configure initiator BA sessions policies */
	ret = wl1271_set_ba_policies(wl);
=======
	ret = wl12xx_acx_set_rate_mgmt_params(wl);
	if (ret < 0)
		goto out_free_memmap;

	/* configure hangover */
	ret = wl12xx_acx_config_hangover(wl);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		goto out_free_memmap;

	return 0;

 out_free_memmap:
	kfree(wl->target_mem_map);
	wl->target_mem_map = NULL;

	return ret;
}
