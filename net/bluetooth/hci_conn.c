/*
   BlueZ - Bluetooth protocol stack for Linux
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
   Copyright (c) 2000-2001, The Linux Foundation. All rights reserved.
   Copyright (c) 2010-2013, The Linux Foundation. All rights reserved.
=======
   Copyright (c) 2000-2001, 2010-2012 The Linux Foundation.  All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
   Copyright (c) 2000-2001, 2010, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
   Copyright (c) 2000-2001, 2010-2012 The Linux Foundation.  All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0

   Written 2000,2001 by Maxim Krasnyansky <maxk@qualcomm.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as
   published by the Free Software Foundation;

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS.
   IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) AND AUTHOR(S) BE LIABLE FOR ANY
   CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

   ALL LIABILITY, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PATENTS,
   COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS, RELATING TO USE OF THIS
   SOFTWARE IS DISCLAIMED.
*/

/* Bluetooth HCI connection handling. */

<<<<<<< HEAD
#include <linux/module.h>

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/interrupt.h>
#include <linux/notifier.h>
#include <net/sock.h>

#include <asm/system.h>
#include <linux/uaccess.h>
#include <asm/unaligned.h>

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>
#include <net/bluetooth/l2cap.h>

struct hci_conn *hci_le_connect(struct hci_dev *hdev, __u16 pkt_type,
				bdaddr_t *dst, __u8 sec_level, __u8 auth_type,
				struct bt_le_params *le_params)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct hci_conn *le, *le_wlist_conn;
=======
	struct hci_conn *le;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct hci_conn *le, *le_wlist_conn;
>>>>>>> refs/remotes/origin/cm-11.0
	struct hci_cp_le_create_conn cp;
	struct adv_entry *entry;
	struct link_key *key;

	BT_DBG("%p", hdev);

	le = hci_conn_hash_lookup_ba(hdev, LE_LINK, dst);
	if (le) {
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		le_wlist_conn = hci_conn_hash_lookup_ba(hdev, LE_LINK,
								BDADDR_ANY);
		if (!le_wlist_conn) {
			hci_conn_hold(le);
			return le;
		} else {
			BT_DBG("remove wlist conn");
			le->out = 1;
			le->link_mode |= HCI_LM_MASTER;
			le->sec_level = BT_SECURITY_LOW;
			le->type = LE_LINK;
			hci_proto_connect_cfm(le, 0);
			hci_conn_del(le_wlist_conn);
			return le;
		}
<<<<<<< HEAD
=======
		hci_conn_hold(le);
		return le;
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}

	key = hci_find_link_key_type(hdev, dst, KEY_TYPE_LTK);
	if (!key) {
		entry = hci_find_adv_entry(hdev, dst);
		if (entry)
			le = hci_le_conn_add(hdev, dst,
					entry->bdaddr_type);
		else
			le = hci_le_conn_add(hdev, dst, 0);
	} else {
		le = hci_le_conn_add(hdev, dst, key->addr_type);
	}

	if (!le)
		return ERR_PTR(-ENOMEM);

	hci_conn_hold(le);

	le->state = BT_CONNECT;
	le->out = 1;
	le->link_mode |= HCI_LM_MASTER;
	le->sec_level = BT_SECURITY_LOW;
	le->type = LE_LINK;

	memset(&cp, 0, sizeof(cp));
	if (l2cap_sock_le_params_valid(le_params)) {
		cp.supervision_timeout =
				cpu_to_le16(le_params->supervision_timeout);
		cp.scan_interval = cpu_to_le16(le_params->scan_interval);
		cp.scan_window = cpu_to_le16(le_params->scan_window);
		cp.conn_interval_min = cpu_to_le16(le_params->interval_min);
		cp.conn_interval_max = cpu_to_le16(le_params->interval_max);
		cp.conn_latency = cpu_to_le16(le_params->latency);
		cp.min_ce_len = cpu_to_le16(le_params->min_ce_len);
		cp.max_ce_len = cpu_to_le16(le_params->max_ce_len);
		le->conn_timeout = le_params->conn_timeout;
	} else {
		cp.supervision_timeout = cpu_to_le16(BT_LE_SUP_TO_DEFAULT);
		cp.scan_interval = cpu_to_le16(BT_LE_SCAN_INTERVAL_DEF);
		cp.scan_window = cpu_to_le16(BT_LE_SCAN_WINDOW_DEF);
		cp.conn_interval_min = cpu_to_le16(BT_LE_CONN_INTERVAL_MIN_DEF);
		cp.conn_interval_max = cpu_to_le16(BT_LE_CONN_INTERVAL_MAX_DEF);
		cp.conn_latency = cpu_to_le16(BT_LE_LATENCY_DEF);
		le->conn_timeout = 5;
	}
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (!bacmp(&le->dst, BDADDR_ANY)) {
		cp.filter_policy = 0x01;
		le->conn_timeout = 0;
	} else {
		bacpy(&cp.peer_addr, &le->dst);
		cp.peer_addr_type = le->dst_type;
	}
<<<<<<< HEAD
=======
	bacpy(&cp.peer_addr, &le->dst);
	cp.peer_addr_type = le->dst_type;
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

	hci_send_cmd(hdev, HCI_OP_LE_CREATE_CONN, sizeof(cp), &cp);

	return le;
}
EXPORT_SYMBOL(hci_le_connect);

static void hci_le_connect_cancel(struct hci_conn *conn)
{
	hci_send_cmd(conn->hdev, HCI_OP_LE_CREATE_CONN_CANCEL, 0, NULL);
}
<<<<<<< HEAD

void hci_le_cancel_create_connect(struct hci_dev *hdev, bdaddr_t *dst)
{
	struct hci_conn *le;

	BT_DBG("%p", hdev);

	le = hci_conn_hash_lookup_ba(hdev, LE_LINK, dst);
	if (le) {
		BT_DBG("send hci connect cancel");
		hci_le_connect_cancel(le);
		hci_conn_del(le);
	}
}
EXPORT_SYMBOL(hci_le_cancel_create_connect);

void hci_le_add_dev_white_list(struct hci_dev *hdev, bdaddr_t *dst)
{
	struct hci_cp_le_add_dev_white_list cp;
	struct adv_entry *entry;
	struct link_key *key;

	BT_DBG("%p", hdev);

	memset(&cp, 0, sizeof(cp));
	bacpy(&cp.addr, dst);

	key = hci_find_link_key_type(hdev, dst, KEY_TYPE_LTK);
	if (!key) {
		entry = hci_find_adv_entry(hdev, dst);
		if (entry)
			cp.addr_type = entry->bdaddr_type;
		else
			cp.addr_type = 0x00;
	} else {
		cp.addr_type = key->addr_type;
	}

	hci_send_cmd(hdev, HCI_OP_LE_ADD_DEV_WHITE_LIST, sizeof(cp), &cp);
}
EXPORT_SYMBOL(hci_le_add_dev_white_list);

void hci_le_remove_dev_white_list(struct hci_dev *hdev, bdaddr_t *dst)
{
	struct hci_cp_le_remove_dev_white_list cp;
	struct adv_entry *entry;
	struct link_key *key;

	BT_DBG("%p", hdev);

	memset(&cp, 0, sizeof(cp));
	bacpy(&cp.addr, dst);

	key = hci_find_link_key_type(hdev, dst, KEY_TYPE_LTK);
	if (!key) {
		entry = hci_find_adv_entry(hdev, dst);
		if (entry)
			cp.addr_type = entry->bdaddr_type;
		else
			cp.addr_type = 0x00;
	} else {
		cp.addr_type = key->addr_type;
	}

	hci_send_cmd(hdev, HCI_OP_LE_REMOVE_DEV_WHITE_LIST, sizeof(cp), &cp);
}
EXPORT_SYMBOL(hci_le_remove_dev_white_list);

=======
>>>>>>> refs/remotes/origin/cm-10.0
static inline bool is_role_switch_possible(struct hci_dev *hdev)
{
	if (hci_conn_hash_lookup_state(hdev, ACL_LINK, BT_CONNECTED))
		return false;
	return true;
}

void hci_le_cancel_create_connect(struct hci_dev *hdev, bdaddr_t *dst)
{
	struct hci_conn *le;

	BT_DBG("%p", hdev);

	le = hci_conn_hash_lookup_ba(hdev, LE_LINK, dst);
	if (le) {
		BT_DBG("send hci connect cancel");
		hci_le_connect_cancel(le);
		hci_conn_del(le);
	}
}
EXPORT_SYMBOL(hci_le_cancel_create_connect);

void hci_le_add_dev_white_list(struct hci_dev *hdev, bdaddr_t *dst)
{
	struct hci_cp_le_add_dev_white_list cp;
	struct adv_entry *entry;
	struct link_key *key;

	BT_DBG("%p", hdev);

	memset(&cp, 0, sizeof(cp));
	bacpy(&cp.addr, dst);

	key = hci_find_link_key_type(hdev, dst, KEY_TYPE_LTK);
	if (!key) {
		entry = hci_find_adv_entry(hdev, dst);
		if (entry)
			cp.addr_type = entry->bdaddr_type;
		else
			cp.addr_type = 0x00;
	} else {
		cp.addr_type = key->addr_type;
	}

	hci_send_cmd(hdev, HCI_OP_LE_ADD_DEV_WHITE_LIST, sizeof(cp), &cp);
}
EXPORT_SYMBOL(hci_le_add_dev_white_list);

void hci_le_remove_dev_white_list(struct hci_dev *hdev, bdaddr_t *dst)
{
	struct hci_cp_le_remove_dev_white_list cp;
	struct adv_entry *entry;
	struct link_key *key;

	BT_DBG("%p", hdev);

	memset(&cp, 0, sizeof(cp));
	bacpy(&cp.addr, dst);

	key = hci_find_link_key_type(hdev, dst, KEY_TYPE_LTK);
	if (!key) {
		entry = hci_find_adv_entry(hdev, dst);
		if (entry)
			cp.addr_type = entry->bdaddr_type;
		else
			cp.addr_type = 0x00;
	} else {
		cp.addr_type = key->addr_type;
	}

	hci_send_cmd(hdev, HCI_OP_LE_REMOVE_DEV_WHITE_LIST, sizeof(cp), &cp);
}
EXPORT_SYMBOL(hci_le_remove_dev_white_list);

static inline bool is_role_switch_possible(struct hci_dev *hdev)
{
	if (hci_conn_hash_lookup_state(hdev, ACL_LINK, BT_CONNECTED))
		return false;
	return true;
}

void hci_acl_connect(struct hci_conn *conn)
=======
#include <linux/export.h>

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>

#include "smp.h"
#include "a2mp.h"

struct sco_param {
	u16 pkt_type;
	u16 max_latency;
};

static const struct sco_param sco_param_cvsd[] = {
	{ EDR_ESCO_MASK & ~ESCO_2EV3, 0x000a }, /* S3 */
	{ EDR_ESCO_MASK & ~ESCO_2EV3, 0x0007 }, /* S2 */
	{ EDR_ESCO_MASK | ESCO_EV3,   0x0007 }, /* S1 */
	{ EDR_ESCO_MASK | ESCO_HV3,   0xffff }, /* D1 */
	{ EDR_ESCO_MASK | ESCO_HV1,   0xffff }, /* D0 */
};

static const struct sco_param sco_param_wideband[] = {
	{ EDR_ESCO_MASK & ~ESCO_2EV3, 0x000d }, /* T2 */
	{ EDR_ESCO_MASK | ESCO_EV3,   0x0008 }, /* T1 */
};

static void hci_le_create_connection_cancel(struct hci_conn *conn)
{
	hci_send_cmd(conn->hdev, HCI_OP_LE_CREATE_CONN_CANCEL, 0, NULL);
}

static void hci_acl_create_connection(struct hci_conn *conn)
>>>>>>> refs/remotes/origin/master
{
	struct hci_dev *hdev = conn->hdev;
	struct inquiry_entry *ie;
	struct hci_cp_create_conn cp;

<<<<<<< HEAD
<<<<<<< HEAD
	BT_DBG("%p", conn);

	conn->state = BT_CONNECT;
	conn->out = 1;
=======
	BT_DBG("hcon %p", conn);

	conn->state = BT_CONNECT;
	conn->out = true;
>>>>>>> refs/remotes/origin/master
=======
	BT_DBG("%p", conn);

	conn->state = BT_CONNECT;
	conn->out = 1;
>>>>>>> refs/remotes/origin/cm-11.0

	conn->link_mode = HCI_LM_MASTER;

	conn->attempt++;

	conn->link_policy = hdev->link_policy;

	memset(&cp, 0, sizeof(cp));
	bacpy(&cp.bdaddr, &conn->dst);
	cp.pscan_rep_mode = 0x02;

	ie = hci_inquiry_cache_lookup(hdev, &conn->dst);
	if (ie) {
		if (inquiry_entry_age(ie) <= INQUIRY_ENTRY_AGE_MAX) {
			cp.pscan_rep_mode = ie->data.pscan_rep_mode;
			cp.pscan_mode     = ie->data.pscan_mode;
			cp.clock_offset   = ie->data.clock_offset |
<<<<<<< HEAD
							cpu_to_le16(0x8000);
		}

		memcpy(conn->dev_class, ie->data.dev_class, 3);
		conn->ssp_mode = ie->data.ssp_mode;
<<<<<<< HEAD
	}

	cp.pkt_type = cpu_to_le16(conn->pkt_type);
	if (lmp_rswitch_capable(hdev) && !(hdev->link_mode & HCI_LM_MASTER)
		&& is_role_switch_possible(hdev))
=======
					    __constant_cpu_to_le16(0x8000);
		}

		memcpy(conn->dev_class, ie->data.dev_class, 3);
		if (ie->data.ssp_mode > 0)
			set_bit(HCI_CONN_SSP_ENABLED, &conn->flags);
	}

	cp.pkt_type = cpu_to_le16(conn->pkt_type);
	if (lmp_rswitch_capable(hdev) && !(hdev->link_mode & HCI_LM_MASTER))
>>>>>>> refs/remotes/origin/master
=======
	}

	cp.pkt_type = cpu_to_le16(conn->pkt_type);
	if (lmp_rswitch_capable(hdev) && !(hdev->link_mode & HCI_LM_MASTER)
		&& is_role_switch_possible(hdev))
>>>>>>> refs/remotes/origin/cm-11.0
		cp.role_switch = 0x01;
	else
		cp.role_switch = 0x00;

	hci_send_cmd(hdev, HCI_OP_CREATE_CONN, sizeof(cp), &cp);
}

<<<<<<< HEAD
static void hci_acl_connect_cancel(struct hci_conn *conn)
{
	struct hci_cp_create_conn_cancel cp;

	BT_DBG("%p", conn);

	if (conn->hdev->hci_ver < 2)
<<<<<<< HEAD
=======
static void hci_acl_create_connection_cancel(struct hci_conn *conn)
{
	struct hci_cp_create_conn_cancel cp;

	BT_DBG("hcon %p", conn);

	if (conn->hdev->hci_ver < BLUETOOTH_VER_1_2)
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		return;

	bacpy(&cp.bdaddr, &conn->dst);
	hci_send_cmd(conn->hdev, HCI_OP_CREATE_CONN_CANCEL, sizeof(cp), &cp);
}

<<<<<<< HEAD
void hci_acl_disconn(struct hci_conn *conn, __u8 reason)
{
	BT_DBG("%p", conn);
<<<<<<< HEAD

	conn->state = BT_DISCONN;

	if (conn->hdev->dev_type == HCI_BREDR) {
		struct hci_cp_disconnect cp;
		cp.handle = cpu_to_le16(conn->handle);
		cp.reason = reason;
		hci_send_cmd(conn->hdev, HCI_OP_DISCONNECT, sizeof(cp), &cp);
	} else {
		struct hci_cp_disconn_phys_link cp;
		cp.phy_handle = (u8) conn->handle;
		cp.reason = reason;
		hci_send_cmd(conn->hdev, HCI_OP_DISCONN_PHYS_LINK,
				sizeof(cp), &cp);
	}
}

void hci_add_sco(struct hci_conn *conn, __u16 handle)
=======
static void hci_reject_sco(struct hci_conn *conn)
{
	struct hci_cp_reject_sync_conn_req cp;

	cp.reason = HCI_ERROR_REMOTE_USER_TERM;
	bacpy(&cp.bdaddr, &conn->dst);

	hci_send_cmd(conn->hdev, HCI_OP_REJECT_SYNC_CONN_REQ, sizeof(cp), &cp);
}

void hci_disconnect(struct hci_conn *conn, __u8 reason)
{
	struct hci_cp_disconnect cp;

	BT_DBG("hcon %p", conn);
=======
>>>>>>> refs/remotes/origin/cm-11.0

	conn->state = BT_DISCONN;

	if (conn->hdev->dev_type == HCI_BREDR) {
		struct hci_cp_disconnect cp;
		cp.handle = cpu_to_le16(conn->handle);
		cp.reason = reason;
		hci_send_cmd(conn->hdev, HCI_OP_DISCONNECT, sizeof(cp), &cp);
	} else {
		struct hci_cp_disconn_phys_link cp;
		cp.phy_handle = (u8) conn->handle;
		cp.reason = reason;
		hci_send_cmd(conn->hdev, HCI_OP_DISCONN_PHYS_LINK,
				sizeof(cp), &cp);
	}
}

static void hci_amp_disconn(struct hci_conn *conn, __u8 reason)
{
	struct hci_cp_disconn_phy_link cp;

	BT_DBG("hcon %p", conn);

	conn->state = BT_DISCONN;

	cp.phy_handle = HCI_PHY_HANDLE(conn->handle);
	cp.reason = reason;
	hci_send_cmd(conn->hdev, HCI_OP_DISCONN_PHY_LINK,
		     sizeof(cp), &cp);
}

static void hci_add_sco(struct hci_conn *conn, __u16 handle)
>>>>>>> refs/remotes/origin/master
{
	struct hci_dev *hdev = conn->hdev;
	struct hci_cp_add_sco cp;

<<<<<<< HEAD
	BT_DBG("%p", conn);

	conn->state = BT_CONNECT;
	conn->out = 1;
<<<<<<< HEAD
=======
	BT_DBG("hcon %p", conn);

	conn->state = BT_CONNECT;
	conn->out = true;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	conn->attempt++;

	cp.handle   = cpu_to_le16(handle);
	cp.pkt_type = cpu_to_le16(conn->pkt_type);

	hci_send_cmd(hdev, HCI_OP_ADD_SCO, sizeof(cp), &cp);
}

<<<<<<< HEAD
void hci_setup_sync(struct hci_conn *conn, __u16 handle)
{
	struct hci_dev *hdev = conn->hdev;
	struct hci_cp_setup_sync_conn cp;

	BT_DBG("%p", conn);

	conn->state = BT_CONNECT;
	conn->out = 1;
<<<<<<< HEAD
=======
bool hci_setup_sync(struct hci_conn *conn, __u16 handle)
{
	struct hci_dev *hdev = conn->hdev;
	struct hci_cp_setup_sync_conn cp;
	const struct sco_param *param;

	BT_DBG("hcon %p", conn);

	conn->state = BT_CONNECT;
	conn->out = true;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	conn->attempt++;

	cp.handle   = cpu_to_le16(handle);

<<<<<<< HEAD
	cp.tx_bandwidth   = cpu_to_le32(0x00001f40);
	cp.rx_bandwidth   = cpu_to_le32(0x00001f40);
	if (conn->hdev->is_wbs) {
		/* Transparent Data */
		uint16_t voice_setting = hdev->voice_setting | ACF_TRANS;
		cp.max_latency    = cpu_to_le16(0x000D);
		cp.pkt_type = cpu_to_le16(ESCO_WBS);
		cp.voice_setting  = cpu_to_le16(voice_setting);
		/* Retransmission Effort */
		cp.retrans_effort = RE_LINK_QUALITY;
	} else {
		cp.max_latency    = cpu_to_le16(0x000A);
		cp.pkt_type = cpu_to_le16(conn->pkt_type);
		cp.voice_setting  = cpu_to_le16(hdev->voice_setting);
		cp.retrans_effort = RE_POWER_CONSUMP;
	}

	hci_send_cmd(hdev, HCI_OP_SETUP_SYNC_CONN, sizeof(cp), &cp);
}

void hci_le_conn_update(struct hci_conn *conn, u16 min, u16 max,
					u16 latency, u16 to_multiplier)
=======
	cp.tx_bandwidth   = __constant_cpu_to_le32(0x00001f40);
	cp.rx_bandwidth   = __constant_cpu_to_le32(0x00001f40);
	cp.voice_setting  = cpu_to_le16(conn->setting);

	switch (conn->setting & SCO_AIRMODE_MASK) {
	case SCO_AIRMODE_TRANSP:
		if (conn->attempt > ARRAY_SIZE(sco_param_wideband))
			return false;
		cp.retrans_effort = 0x02;
		param = &sco_param_wideband[conn->attempt - 1];
		break;
	case SCO_AIRMODE_CVSD:
		if (conn->attempt > ARRAY_SIZE(sco_param_cvsd))
			return false;
		cp.retrans_effort = 0x01;
		param = &sco_param_cvsd[conn->attempt - 1];
		break;
	default:
		return false;
	}

	cp.pkt_type = __cpu_to_le16(param->pkt_type);
	cp.max_latency = __cpu_to_le16(param->max_latency);

	if (hci_send_cmd(hdev, HCI_OP_SETUP_SYNC_CONN, sizeof(cp), &cp) < 0)
		return false;

	return true;
}

void hci_le_conn_update(struct hci_conn *conn, u16 min, u16 max,
			u16 latency, u16 to_multiplier)
>>>>>>> refs/remotes/origin/master
{
	struct hci_cp_le_conn_update cp;
	struct hci_dev *hdev = conn->hdev;

	memset(&cp, 0, sizeof(cp));

	cp.handle		= cpu_to_le16(conn->handle);
	cp.conn_interval_min	= cpu_to_le16(min);
	cp.conn_interval_max	= cpu_to_le16(max);
	cp.conn_latency		= cpu_to_le16(latency);
	cp.supervision_timeout	= cpu_to_le16(to_multiplier);
<<<<<<< HEAD
	cp.min_ce_len		= cpu_to_le16(0x0001);
	cp.max_ce_len		= cpu_to_le16(0x0001);

	hci_send_cmd(hdev, HCI_OP_LE_CONN_UPDATE, sizeof(cp), &cp);
}
EXPORT_SYMBOL(hci_le_conn_update);

void hci_read_rssi(struct hci_conn *conn)
{
	struct hci_cp_read_rssi cp;
	struct hci_dev *hdev = conn->hdev;

	memset(&cp, 0, sizeof(cp));
	cp.handle   = cpu_to_le16(conn->handle);

	hci_send_cmd(hdev, HCI_OP_READ_RSSI, sizeof(cp), &cp);
}
EXPORT_SYMBOL(hci_read_rssi);

void hci_le_start_enc(struct hci_conn *conn, __le16 ediv, __u8 rand[8],
							__u8 ltk[16])
=======
	cp.min_ce_len		= __constant_cpu_to_le16(0x0001);
	cp.max_ce_len		= __constant_cpu_to_le16(0x0001);

	hci_send_cmd(hdev, HCI_OP_LE_CONN_UPDATE, sizeof(cp), &cp);
}

void hci_le_start_enc(struct hci_conn *conn, __le16 ediv, __u8 rand[8],
		      __u8 ltk[16])
>>>>>>> refs/remotes/origin/master
{
	struct hci_dev *hdev = conn->hdev;
	struct hci_cp_le_start_enc cp;

<<<<<<< HEAD
	BT_DBG("%p", conn);
=======
	BT_DBG("hcon %p", conn);
>>>>>>> refs/remotes/origin/master

	memset(&cp, 0, sizeof(cp));

	cp.handle = cpu_to_le16(conn->handle);
	memcpy(cp.ltk, ltk, sizeof(cp.ltk));
	cp.ediv = ediv;
	memcpy(cp.rand, rand, sizeof(cp.rand));

	hci_send_cmd(hdev, HCI_OP_LE_START_ENC, sizeof(cp), &cp);
}
<<<<<<< HEAD
EXPORT_SYMBOL(hci_le_start_enc);

void hci_le_ltk_reply(struct hci_conn *conn, u8 ltk[16])
{
	struct hci_dev *hdev = conn->hdev;
	struct hci_cp_le_ltk_reply cp;

	BT_DBG("%p", conn);

	memset(&cp, 0, sizeof(cp));

	cp.handle = cpu_to_le16(conn->handle);
	memcpy(cp.ltk, ltk, sizeof(ltk));

	hci_send_cmd(hdev, HCI_OP_LE_LTK_REPLY, sizeof(cp), &cp);
}
EXPORT_SYMBOL(hci_le_ltk_reply);

void hci_le_ltk_neg_reply(struct hci_conn *conn)
{
	struct hci_dev *hdev = conn->hdev;
	struct hci_cp_le_ltk_neg_reply cp;

	BT_DBG("%p", conn);

	memset(&cp, 0, sizeof(cp));

	cp.handle = cpu_to_le16(conn->handle);

	hci_send_cmd(hdev, HCI_OP_LE_LTK_NEG_REPLY, sizeof(cp), &cp);
}
=======
>>>>>>> refs/remotes/origin/master

/* Device _must_ be locked */
void hci_sco_setup(struct hci_conn *conn, __u8 status)
{
	struct hci_conn *sco = conn->link;

<<<<<<< HEAD
	BT_DBG("%p", conn);

	if (!sco)
		return;

=======
	if (!sco)
		return;

	BT_DBG("hcon %p", conn);

>>>>>>> refs/remotes/origin/master
	if (!status) {
		if (lmp_esco_capable(conn->hdev))
			hci_setup_sync(sco, conn->handle);
		else
			hci_add_sco(sco, conn->handle);
	} else {
		hci_proto_connect_cfm(sco, status);
		hci_conn_del(sco);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static void hci_conn_timeout(unsigned long arg)
{
	struct hci_conn *conn = (void *) arg;
	struct hci_dev *hdev = conn->hdev;
	__u8 reason;

	BT_DBG("conn %p state %d", conn, conn->state);

	hci_dev_lock(hdev);
=======
static void hci_conn_disconnect(struct hci_conn *conn)
{
	__u8 reason = hci_proto_disconn_ind(conn);

	switch (conn->type) {
	case AMP_LINK:
		hci_amp_disconn(conn, reason);
		break;
	default:
		hci_disconnect(conn, reason);
		break;
	}
}

static void hci_conn_timeout(struct work_struct *work)
{
	struct hci_conn *conn = container_of(work, struct hci_conn,
					     disc_work.work);

	BT_DBG("hcon %p state %s", conn, state_to_string(conn->state));

	if (atomic_read(&conn->refcnt))
		return;
>>>>>>> refs/remotes/origin/master
=======
static void hci_conn_timeout(unsigned long arg)
{
	struct hci_conn *conn = (void *) arg;
	struct hci_dev *hdev = conn->hdev;
	__u8 reason;

	BT_DBG("conn %p state %d", conn, conn->state);

	hci_dev_lock(hdev);
>>>>>>> refs/remotes/origin/cm-11.0

	switch (conn->state) {
	case BT_CONNECT:
	case BT_CONNECT2:
		if (conn->out) {
			if (conn->type == ACL_LINK)
<<<<<<< HEAD
				hci_acl_connect_cancel(conn);
			else if (conn->type == LE_LINK)
				hci_le_connect_cancel(conn);
=======
				hci_acl_create_connection_cancel(conn);
			else if (conn->type == LE_LINK)
				hci_le_create_connection_cancel(conn);
		} else if (conn->type == SCO_LINK || conn->type == ESCO_LINK) {
			hci_reject_sco(conn);
>>>>>>> refs/remotes/origin/master
		}
		break;
	case BT_CONFIG:
	case BT_CONNECTED:
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		if (!atomic_read(&conn->refcnt)) {
			reason = hci_proto_disconn_ind(conn);
			hci_acl_disconn(conn, reason);
		}
<<<<<<< HEAD
		break;
	default:
		if (!atomic_read(&conn->refcnt))
			conn->state = BT_CLOSED;
		break;
	}

	hci_dev_unlock(hdev);
}

static void hci_conn_idle(unsigned long arg)
{
	struct hci_conn *conn = (void *) arg;

	BT_DBG("conn %p mode %d", conn, conn->mode);

	hci_conn_enter_sniff_mode(conn);
}

static void hci_conn_rssi_update(struct work_struct *work)
{
	struct delayed_work *delayed =
		container_of(work, struct delayed_work, work);
	struct hci_conn *conn =
		container_of(delayed, struct hci_conn, rssi_update_work);

	BT_DBG("conn %p mode %d", conn, conn->mode);

	hci_read_rssi(conn);
}

static void encryption_disabled_timeout(unsigned long userdata)
{
	struct hci_conn *conn = (struct hci_conn *)userdata;
	BT_INFO("conn %p Grace Prd Exp ", conn);

	hci_encrypt_cfm(conn, 0, 0);

	if (test_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend)) {
		struct hci_cp_set_conn_encrypt cp;
		BT_INFO("HCI_CONN_ENCRYPT_PEND is set");
		cp.handle  = cpu_to_le16(conn->handle);
		cp.encrypt = 1;
		hci_send_cmd(conn->hdev, HCI_OP_SET_CONN_ENCRYPT,
						sizeof(cp), &cp);
	}

}

struct hci_conn *hci_conn_add(struct hci_dev *hdev, int type,
					__u16 pkt_type, bdaddr_t *dst)
{
	struct hci_conn *conn;

	BT_DBG("%s dst %s", hdev->name, batostr(dst));

	conn = kzalloc(sizeof(struct hci_conn), GFP_ATOMIC);
=======
		hci_conn_disconnect(conn);
=======
>>>>>>> refs/remotes/origin/cm-11.0
		break;
	default:
		if (!atomic_read(&conn->refcnt))
			conn->state = BT_CLOSED;
		break;
	}

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
/* Enter sniff mode */
static void hci_conn_idle(struct work_struct *work)
{
	struct hci_conn *conn = container_of(work, struct hci_conn,
					     idle_work.work);
	struct hci_dev *hdev = conn->hdev;
=======
static void hci_conn_idle(unsigned long arg)
{
	struct hci_conn *conn = (void *) arg;
>>>>>>> refs/remotes/origin/cm-11.0

	BT_DBG("hcon %p mode %d", conn, conn->mode);

<<<<<<< HEAD
	if (test_bit(HCI_RAW, &hdev->flags))
		return;

	if (!lmp_sniff_capable(hdev) || !lmp_sniff_capable(conn))
		return;

	if (conn->mode != HCI_CM_ACTIVE || !(conn->link_policy & HCI_LP_SNIFF))
		return;

	if (lmp_sniffsubr_capable(hdev) && lmp_sniffsubr_capable(conn)) {
		struct hci_cp_sniff_subrate cp;
		cp.handle             = cpu_to_le16(conn->handle);
		cp.max_latency        = __constant_cpu_to_le16(0);
		cp.min_remote_timeout = __constant_cpu_to_le16(0);
		cp.min_local_timeout  = __constant_cpu_to_le16(0);
		hci_send_cmd(hdev, HCI_OP_SNIFF_SUBRATE, sizeof(cp), &cp);
	}

	if (!test_and_set_bit(HCI_CONN_MODE_CHANGE_PEND, &conn->flags)) {
		struct hci_cp_sniff_mode cp;
		cp.handle       = cpu_to_le16(conn->handle);
		cp.max_interval = cpu_to_le16(hdev->sniff_max_interval);
		cp.min_interval = cpu_to_le16(hdev->sniff_min_interval);
		cp.attempt      = __constant_cpu_to_le16(4);
		cp.timeout      = __constant_cpu_to_le16(1);
		hci_send_cmd(hdev, HCI_OP_SNIFF_MODE, sizeof(cp), &cp);
	}
}

static void hci_conn_auto_accept(struct work_struct *work)
{
	struct hci_conn *conn = container_of(work, struct hci_conn,
					     auto_accept_work.work);

	hci_send_cmd(conn->hdev, HCI_OP_USER_CONFIRM_REPLY, sizeof(conn->dst),
		     &conn->dst);
=======
	hci_conn_enter_sniff_mode(conn);
}

static void hci_conn_rssi_update(struct work_struct *work)
{
	struct delayed_work *delayed =
		container_of(work, struct delayed_work, work);
	struct hci_conn *conn =
		container_of(delayed, struct hci_conn, rssi_update_work);

	BT_DBG("conn %p mode %d", conn, conn->mode);

	hci_read_rssi(conn);
}

static void encryption_disabled_timeout(unsigned long userdata)
{
	struct hci_conn *conn = (struct hci_conn *)userdata;
	BT_INFO("conn %p Grace Prd Exp ", conn);

	hci_encrypt_cfm(conn, 0, 0);

	if (test_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend)) {
		struct hci_cp_set_conn_encrypt cp;
		BT_INFO("HCI_CONN_ENCRYPT_PEND is set");
		cp.handle  = cpu_to_le16(conn->handle);
		cp.encrypt = 1;
		hci_send_cmd(conn->hdev, HCI_OP_SET_CONN_ENCRYPT,
						sizeof(cp), &cp);
	}

>>>>>>> refs/remotes/origin/cm-11.0
}

struct hci_conn *hci_conn_add(struct hci_dev *hdev, int type,
					__u16 pkt_type, bdaddr_t *dst)
{
	struct hci_conn *conn;

	BT_DBG("%s dst %pMR", hdev->name, dst);

<<<<<<< HEAD
	conn = kzalloc(sizeof(struct hci_conn), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
=======
	conn = kzalloc(sizeof(struct hci_conn), GFP_ATOMIC);
>>>>>>> refs/remotes/origin/cm-11.0
	if (!conn)
		return NULL;

	bacpy(&conn->dst, dst);
<<<<<<< HEAD
=======
	bacpy(&conn->src, &hdev->bdaddr);
>>>>>>> refs/remotes/origin/master
	conn->hdev  = hdev;
	conn->type  = type;
	conn->mode  = HCI_CM_ACTIVE;
	conn->state = BT_OPEN;
	conn->auth_type = HCI_AT_GENERAL_BONDING;
	conn->io_capability = hdev->io_capability;
	conn->remote_auth = 0xff;
<<<<<<< HEAD
<<<<<<< HEAD

	conn->power_save = 1;
	conn->disc_timeout = HCI_DISCONN_TIMEOUT;
	conn->conn_valid = true;
	spin_lock_init(&conn->lock);
<<<<<<< HEAD
=======
	wake_lock_init(&conn->idle_lock, WAKE_LOCK_SUSPEND, "bt_idle");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	conn->key_type = 0xff;
=======
>>>>>>> refs/remotes/origin/cm-11.0

	conn->power_save = 1;
	conn->disc_timeout = HCI_DISCONN_TIMEOUT;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
	conn->conn_valid = true;
	spin_lock_init(&conn->lock);
	wake_lock_init(&conn->idle_lock, WAKE_LOCK_SUSPEND, "bt_idle");
>>>>>>> refs/remotes/origin/cm-11.0

	switch (type) {
	case ACL_LINK:
		conn->pkt_type = hdev->pkt_type & ACL_PTYPE_MASK;
<<<<<<< HEAD
<<<<<<< HEAD
		conn->link_policy = hdev->link_policy;
		break;
	case SCO_LINK:
		if (!pkt_type)
			pkt_type = SCO_ESCO_MASK;
	case ESCO_LINK:
		if (!pkt_type)
			pkt_type = ALL_ESCO_MASK;
		if (lmp_esco_capable(hdev)) {
			/* HCI Setup Synchronous Connection Command uses
			   reverse logic on the EDR_ESCO_MASK bits */
			conn->pkt_type = (pkt_type ^ EDR_ESCO_MASK) &
					hdev->esco_type;
		} else {
			/* Legacy HCI Add Sco Connection Command uses a
			   shifted bitmask */
			conn->pkt_type = (pkt_type << 5) & hdev->pkt_type &
					SCO_PTYPE_MASK;
		}
=======
=======
		conn->link_policy = hdev->link_policy;
>>>>>>> refs/remotes/origin/cm-11.0
		break;
	case SCO_LINK:
		if (!pkt_type)
			pkt_type = SCO_ESCO_MASK;
	case ESCO_LINK:
<<<<<<< HEAD
		conn->pkt_type = hdev->esco_type & ~EDR_ESCO_MASK;
>>>>>>> refs/remotes/origin/master
=======
		if (!pkt_type)
			pkt_type = ALL_ESCO_MASK;
		if (lmp_esco_capable(hdev)) {
			/* HCI Setup Synchronous Connection Command uses
			   reverse logic on the EDR_ESCO_MASK bits */
			conn->pkt_type = (pkt_type ^ EDR_ESCO_MASK) &
					hdev->esco_type;
		} else {
			/* Legacy HCI Add Sco Connection Command uses a
			   shifted bitmask */
			conn->pkt_type = (pkt_type << 5) & hdev->pkt_type &
					SCO_PTYPE_MASK;
		}
>>>>>>> refs/remotes/origin/cm-11.0
		break;
	}

	skb_queue_head_init(&conn->data_q);

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	setup_timer(&conn->disc_timer, hci_conn_timeout, (unsigned long)conn);
	setup_timer(&conn->idle_timer, hci_conn_idle, (unsigned long)conn);
	INIT_DELAYED_WORK(&conn->rssi_update_work, hci_conn_rssi_update);
	setup_timer(&conn->encrypt_pause_timer, encryption_disabled_timeout,
			(unsigned long)conn);
<<<<<<< HEAD
=======
	INIT_LIST_HEAD(&conn->chan_list);

	INIT_DELAYED_WORK(&conn->disc_work, hci_conn_timeout);
	INIT_DELAYED_WORK(&conn->auto_accept_work, hci_conn_auto_accept);
	INIT_DELAYED_WORK(&conn->idle_work, hci_conn_idle);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	atomic_set(&conn->refcnt, 0);

	hci_dev_hold(hdev);

<<<<<<< HEAD
<<<<<<< HEAD
	tasklet_disable(&hdev->tx_task);

=======
>>>>>>> refs/remotes/origin/master
=======
	tasklet_disable(&hdev->tx_task);

>>>>>>> refs/remotes/origin/cm-11.0
	hci_conn_hash_add(hdev, conn);
	if (hdev->notify)
		hdev->notify(hdev, HCI_NOTIFY_CONN_ADD);

<<<<<<< HEAD
	atomic_set(&conn->devref, 0);

	hci_conn_init_sysfs(conn);

	tasklet_enable(&hdev->tx_task);

	return conn;
}

struct hci_conn *hci_le_conn_add(struct hci_dev *hdev, bdaddr_t *dst,
							__u8 addr_type)
{
	struct hci_conn *conn = hci_conn_add(hdev, LE_LINK, 0, dst);
	if (!conn)
		return NULL;

	conn->dst_type = addr_type;

<<<<<<< HEAD
=======
	hci_conn_init_sysfs(conn);

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	return conn;
}

int hci_conn_del(struct hci_conn *conn)
{
	struct hci_dev *hdev = conn->hdev;

<<<<<<< HEAD
	BT_DBG("%s conn %p handle %d", hdev->name, conn, conn->handle);

	spin_lock_bh(&conn->lock);
	conn->conn_valid = false; /* conn data is being released */
	spin_unlock_bh(&conn->lock);

	/* Make sure no timers are running */
	del_timer(&conn->idle_timer);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	wake_lock_destroy(&conn->idle_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	wake_lock_destroy(&conn->idle_lock);
>>>>>>> refs/remotes/origin/cm-11.0
	del_timer(&conn->disc_timer);
	del_timer(&conn->smp_timer);
	__cancel_delayed_work(&conn->rssi_update_work);
	del_timer(&conn->encrypt_pause_timer);
<<<<<<< HEAD
=======
	BT_DBG("%s hcon %p handle %d", hdev->name, conn, conn->handle);

	cancel_delayed_work_sync(&conn->disc_work);
	cancel_delayed_work_sync(&conn->auto_accept_work);
	cancel_delayed_work_sync(&conn->idle_work);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	if (conn->type == ACL_LINK) {
		struct hci_conn *sco = conn->link;
		if (sco)
			sco->link = NULL;

		/* Unacked frames */
		hdev->acl_cnt += conn->sent;
	} else if (conn->type == LE_LINK) {
		if (hdev->le_pkts)
			hdev->le_cnt += conn->sent;
		else
			hdev->acl_cnt += conn->sent;
	} else {
		struct hci_conn *acl = conn->link;
		if (acl) {
			acl->link = NULL;
<<<<<<< HEAD
			hci_conn_put(acl);
		}
	}

	tasklet_disable(&hdev->tx_task);
<<<<<<< HEAD
=======
			hci_conn_drop(acl);
		}
	}

	hci_chan_list_flush(conn);
=======
>>>>>>> refs/remotes/origin/cm-11.0

	if (conn->amp_mgr)
		amp_mgr_put(conn->amp_mgr);
>>>>>>> refs/remotes/origin/master

	hci_conn_hash_del(hdev, conn);
	if (hdev->notify)
		hdev->notify(hdev, HCI_NOTIFY_CONN_DEL);

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	tasklet_schedule(&hdev->tx_task);

	tasklet_enable(&hdev->tx_task);

	skb_queue_purge(&conn->data_q);

	hci_conn_put_device(conn);

<<<<<<< HEAD
	if (conn->hidp_session_valid)
		hci_conn_put_device(conn);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	hci_dev_put(hdev);

	return 0;
}

struct hci_chan *hci_chan_add(struct hci_dev *hdev)
{
	struct hci_chan *chan;

	BT_DBG("%s", hdev->name);

	chan = kzalloc(sizeof(struct hci_chan), GFP_ATOMIC);
	if (!chan)
		return NULL;

	atomic_set(&chan->refcnt, 0);

	hci_dev_hold(hdev);

	chan->hdev = hdev;

	list_add(&chan->list, &hdev->chan_list.list);

	return chan;
}
EXPORT_SYMBOL(hci_chan_add);

int hci_chan_del(struct hci_chan *chan)
{
	BT_DBG("%s chan %p", chan->hdev->name, chan);

	list_del(&chan->list);

	hci_conn_put(chan->conn);
	hci_dev_put(chan->hdev);

	kfree(chan);
<<<<<<< HEAD
=======
	skb_queue_purge(&conn->data_q);

	hci_conn_del_sysfs(conn);

	hci_dev_put(hdev);

	hci_conn_put(conn);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
int hci_chan_put(struct hci_chan *chan)
{
	struct hci_cp_disconn_logical_link cp;
	struct hci_conn *hcon;
	u16 ll_handle;

	BT_DBG("chan %p refcnt %d", chan, atomic_read(&chan->refcnt));
	if (!atomic_dec_and_test(&chan->refcnt))
		return 0;

	hcon = chan->conn;
	ll_handle = chan->ll_handle;

	hci_chan_del(chan);

	BT_DBG("chan->conn->state %d", hcon->state);
	if (hcon->state == BT_CONNECTED) {
		cp.log_handle = cpu_to_le16(ll_handle);
		hci_send_cmd(hcon->hdev, HCI_OP_DISCONN_LOGICAL_LINK,
				sizeof(cp), &cp);
	}

	return 1;
}
EXPORT_SYMBOL(hci_chan_put);

struct hci_dev *hci_get_route(bdaddr_t *dst, bdaddr_t *src)
{
	int use_src = bacmp(src, BDADDR_ANY);
	struct hci_dev *hdev = NULL;
	struct list_head *p;

	BT_DBG("%s -> %s", batostr(src), batostr(dst));

	read_lock_bh(&hci_dev_list_lock);
<<<<<<< HEAD

	list_for_each(p, &hci_dev_list) {
		struct hci_dev *d = list_entry(p, struct hci_dev, list);

		if (d->dev_type != HCI_BREDR)
			continue;
		if (!test_bit(HCI_UP, &d->flags) || test_bit(HCI_RAW, &d->flags))
=======
struct hci_dev *hci_get_route(bdaddr_t *dst, bdaddr_t *src)
{
	int use_src = bacmp(src, BDADDR_ANY);
	struct hci_dev *hdev = NULL, *d;

	BT_DBG("%pMR -> %pMR", src, dst);

	read_lock(&hci_dev_list_lock);

	list_for_each_entry(d, &hci_dev_list, list) {
		if (!test_bit(HCI_UP, &d->flags) ||
		    test_bit(HCI_RAW, &d->flags) ||
		    test_bit(HCI_USER_CHANNEL, &d->dev_flags) ||
		    d->dev_type != HCI_BREDR)
>>>>>>> refs/remotes/origin/master
=======

	list_for_each(p, &hci_dev_list) {
		struct hci_dev *d = list_entry(p, struct hci_dev, list);

		if (d->dev_type != HCI_BREDR)
			continue;
		if (!test_bit(HCI_UP, &d->flags) || test_bit(HCI_RAW, &d->flags))
>>>>>>> refs/remotes/origin/cm-11.0
			continue;

		/* Simple routing:
		 *   No source address - find interface with bdaddr != dst
		 *   Source address    - find interface with bdaddr == src
		 */

		if (use_src) {
			if (!bacmp(&d->bdaddr, src)) {
				hdev = d; break;
			}
		} else {
			if (bacmp(&d->bdaddr, dst)) {
				hdev = d; break;
			}
		}
	}

	if (hdev)
		hdev = hci_dev_hold(hdev);

<<<<<<< HEAD
<<<<<<< HEAD
	read_unlock_bh(&hci_dev_list_lock);
=======
	read_unlock(&hci_dev_list_lock);
>>>>>>> refs/remotes/origin/master
=======
	read_unlock_bh(&hci_dev_list_lock);
>>>>>>> refs/remotes/origin/cm-11.0
	return hdev;
}
EXPORT_SYMBOL(hci_get_route);

<<<<<<< HEAD
<<<<<<< HEAD
struct hci_dev *hci_dev_get_type(u8 amp_type)
{
	struct hci_dev *hdev = NULL;
	struct hci_dev *d;

	BT_DBG("amp_type %d", amp_type);

	read_lock_bh(&hci_dev_list_lock);

	list_for_each_entry(d, &hci_dev_list, list) {
		if ((d->amp_type == amp_type) && test_bit(HCI_UP, &d->flags)) {
			hdev = d;
			break;
		}
	}

	if (hdev)
		hdev = hci_dev_hold(hdev);

	read_unlock_bh(&hci_dev_list_lock);
	return hdev;
}
EXPORT_SYMBOL(hci_dev_get_type);

struct hci_dev *hci_dev_get_amp(bdaddr_t *dst)
{
	struct hci_dev *d;
	struct hci_dev *hdev = NULL;

	BT_DBG("%s dst %s", hdev->name, batostr(dst));

	read_lock_bh(&hci_dev_list_lock);

	list_for_each_entry(d, &hci_dev_list, list) {
		struct hci_conn *conn;
		if (d->dev_type == HCI_BREDR)
			continue;
		conn = hci_conn_hash_lookup_ba(d, ACL_LINK, dst);
		if (conn) {
			hdev = d;
			break;
		}
	}

	if (hdev)
		hdev = hci_dev_hold(hdev);

	read_unlock_bh(&hci_dev_list_lock);
	return hdev;
}
EXPORT_SYMBOL(hci_dev_get_amp);

/* Create SCO, ACL or LE connection.
 * Device _must_ be locked */
struct hci_conn *hci_connect(struct hci_dev *hdev, int type,
					__u16 pkt_type, bdaddr_t *dst,
					__u8 sec_level, __u8 auth_type)
{
	struct hci_conn *acl;
	struct hci_conn *sco;
=======
struct hci_dev *hci_dev_get_type(u8 amp_type)
{
	struct hci_dev *hdev = NULL;
	struct hci_dev *d;
>>>>>>> refs/remotes/origin/cm-11.0

	BT_DBG("amp_type %d", amp_type);

<<<<<<< HEAD
	if (type == LE_LINK)
		return hci_le_connect(hdev, pkt_type, dst, sec_level,
							auth_type, NULL);

	acl = hci_conn_hash_lookup_ba(hdev, ACL_LINK, dst);
	if (!acl) {
		acl = hci_conn_add(hdev, ACL_LINK, 0, dst);
		if (!acl)
			return NULL;
=======
static void create_le_conn_complete(struct hci_dev *hdev, u8 status)
{
	struct hci_conn *conn;

	if (status == 0)
		return;

	BT_ERR("HCI request failed to create LE connection: status 0x%2.2x",
	       status);

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_state(hdev, LE_LINK, BT_CONNECT);
	if (!conn)
		goto done;

	conn->state = BT_CLOSED;

	mgmt_connect_failed(hdev, &conn->dst, conn->type, conn->dst_type,
			    status);

	hci_proto_connect_cfm(conn, status);

	hci_conn_del(conn);

done:
	hci_dev_unlock(hdev);
}

static int hci_create_le_conn(struct hci_conn *conn)
{
	struct hci_dev *hdev = conn->hdev;
	struct hci_cp_le_create_conn cp;
	struct hci_request req;
	int err;

	hci_req_init(&req, hdev);

	memset(&cp, 0, sizeof(cp));
	cp.scan_interval = cpu_to_le16(hdev->le_scan_interval);
	cp.scan_window = cpu_to_le16(hdev->le_scan_window);
	bacpy(&cp.peer_addr, &conn->dst);
	cp.peer_addr_type = conn->dst_type;
	cp.own_address_type = conn->src_type;
	cp.conn_interval_min = cpu_to_le16(hdev->le_conn_min_interval);
	cp.conn_interval_max = cpu_to_le16(hdev->le_conn_max_interval);
	cp.supervision_timeout = __constant_cpu_to_le16(0x002a);
	cp.min_ce_len = __constant_cpu_to_le16(0x0000);
	cp.max_ce_len = __constant_cpu_to_le16(0x0000);

	hci_req_add(&req, HCI_OP_LE_CREATE_CONN, sizeof(cp), &cp);

	err = hci_req_run(&req, create_le_conn_complete);
	if (err) {
		hci_conn_del(conn);
		return err;
	}

	return 0;
}

static struct hci_conn *hci_connect_le(struct hci_dev *hdev, bdaddr_t *dst,
				    u8 dst_type, u8 sec_level, u8 auth_type)
{
	struct hci_conn *conn;
	int err;

	if (test_bit(HCI_ADVERTISING, &hdev->flags))
		return ERR_PTR(-ENOTSUPP);

	/* Some devices send ATT messages as soon as the physical link is
	 * established. To be able to handle these ATT messages, the user-
	 * space first establishes the connection and then starts the pairing
	 * process.
	 *
	 * So if a hci_conn object already exists for the following connection
	 * attempt, we simply update pending_sec_level and auth_type fields
	 * and return the object found.
	 */
	conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, dst);
	if (conn) {
		conn->pending_sec_level = sec_level;
		conn->auth_type = auth_type;
		goto done;
	}

	/* Since the controller supports only one LE connection attempt at a
	 * time, we return -EBUSY if there is any connection attempt running.
	 */
	conn = hci_conn_hash_lookup_state(hdev, LE_LINK, BT_CONNECT);
	if (conn)
		return ERR_PTR(-EBUSY);

	conn = hci_conn_add(hdev, LE_LINK, dst);
	if (!conn)
		return ERR_PTR(-ENOMEM);

	if (dst_type == BDADDR_LE_PUBLIC)
		conn->dst_type = ADDR_LE_DEV_PUBLIC;
	else
		conn->dst_type = ADDR_LE_DEV_RANDOM;

	conn->src_type = hdev->own_addr_type;

	conn->state = BT_CONNECT;
	conn->out = true;
	conn->link_mode |= HCI_LM_MASTER;
	conn->sec_level = BT_SECURITY_LOW;
	conn->pending_sec_level = sec_level;
	conn->auth_type = auth_type;

	err = hci_create_le_conn(conn);
	if (err)
		return ERR_PTR(err);

done:
	hci_conn_hold(conn);
	return conn;
}

static struct hci_conn *hci_connect_acl(struct hci_dev *hdev, bdaddr_t *dst,
						u8 sec_level, u8 auth_type)
{
	struct hci_conn *acl;

	if (!test_bit(HCI_BREDR_ENABLED, &hdev->dev_flags))
		return ERR_PTR(-ENOTSUPP);
=======
	read_lock_bh(&hci_dev_list_lock);

	list_for_each_entry(d, &hci_dev_list, list) {
		if ((d->amp_type == amp_type) && test_bit(HCI_UP, &d->flags)) {
			hdev = d;
			break;
		}
	}

	if (hdev)
		hdev = hci_dev_hold(hdev);

	read_unlock_bh(&hci_dev_list_lock);
	return hdev;
}
EXPORT_SYMBOL(hci_dev_get_type);

struct hci_dev *hci_dev_get_amp(bdaddr_t *dst)
{
	struct hci_dev *d;
	struct hci_dev *hdev = NULL;

	BT_DBG("%s dst %s", hdev->name, batostr(dst));

	read_lock_bh(&hci_dev_list_lock);

	list_for_each_entry(d, &hci_dev_list, list) {
		struct hci_conn *conn;
		if (d->dev_type == HCI_BREDR)
			continue;
		conn = hci_conn_hash_lookup_ba(d, ACL_LINK, dst);
		if (conn) {
			hdev = d;
			break;
		}
	}
>>>>>>> refs/remotes/origin/cm-11.0

	if (hdev)
		hdev = hci_dev_hold(hdev);

	read_unlock_bh(&hci_dev_list_lock);
	return hdev;
}
EXPORT_SYMBOL(hci_dev_get_amp);

/* Create SCO, ACL or LE connection.
 * Device _must_ be locked */
struct hci_conn *hci_connect(struct hci_dev *hdev, int type,
					__u16 pkt_type, bdaddr_t *dst,
					__u8 sec_level, __u8 auth_type)
{
	struct hci_conn *acl;
	struct hci_conn *sco;

	BT_DBG("%s dst %s", hdev->name, batostr(dst));

	if (type == LE_LINK)
		return hci_le_connect(hdev, pkt_type, dst, sec_level,
							auth_type, NULL);

	acl = hci_conn_hash_lookup_ba(hdev, ACL_LINK, dst);
	if (!acl) {
		acl = hci_conn_add(hdev, ACL_LINK, 0, dst);
		if (!acl)
<<<<<<< HEAD
			return ERR_PTR(-ENOMEM);
>>>>>>> refs/remotes/origin/master
=======
			return NULL;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	hci_conn_hold(acl);

	if (acl->state == BT_OPEN || acl->state == BT_CLOSED) {
		acl->sec_level = BT_SECURITY_LOW;
		acl->pending_sec_level = sec_level;
		acl->auth_type = auth_type;
<<<<<<< HEAD
		hci_acl_connect(acl);
	}

	if (type == ACL_LINK)
		return acl;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* type of connection already existing can be ESCO or SCO
	 * so check for both types before creating new */

	sco = hci_conn_hash_lookup_ba(hdev, type, dst);

	if (!sco && type == ESCO_LINK) {
		sco = hci_conn_hash_lookup_ba(hdev, SCO_LINK, dst);
	} else if (!sco && type == SCO_LINK) {
		/* this case can be practically not possible */
		sco = hci_conn_hash_lookup_ba(hdev, ESCO_LINK, dst);
	}

<<<<<<< HEAD
=======
	sco = hci_conn_hash_lookup_ba(hdev, type, dst);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (!sco) {
		sco = hci_conn_add(hdev, type, pkt_type, dst);
		if (!sco) {
			hci_conn_put(acl);
			return NULL;
<<<<<<< HEAD
=======
		hci_acl_create_connection(acl);
	}

	return acl;
}

struct hci_conn *hci_connect_sco(struct hci_dev *hdev, int type, bdaddr_t *dst,
				 __u16 setting)
{
	struct hci_conn *acl;
	struct hci_conn *sco;

	acl = hci_connect_acl(hdev, dst, BT_SECURITY_LOW, HCI_AT_NO_BONDING);
	if (IS_ERR(acl))
		return acl;

	sco = hci_conn_hash_lookup_ba(hdev, type, dst);
	if (!sco) {
		sco = hci_conn_add(hdev, type, dst);
		if (!sco) {
			hci_conn_drop(acl);
			return ERR_PTR(-ENOMEM);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		}
	}

	acl->link = sco;
	sco->link = acl;

	hci_conn_hold(sco);

<<<<<<< HEAD
	if (acl->state == BT_CONNECTED &&
			(sco->state == BT_OPEN || sco->state == BT_CLOSED)) {
		acl->power_save = 1;
		hci_conn_enter_active_mode(acl, 1);
<<<<<<< HEAD

		if (test_bit(HCI_CONN_MODE_CHANGE_PEND, &acl->pend)) {
			/* defer SCO setup until mode change completed */
			set_bit(HCI_CONN_SCO_SETUP_PEND, &acl->pend);
=======
	sco->setting = setting;

	if (acl->state == BT_CONNECTED &&
	    (sco->state == BT_OPEN || sco->state == BT_CLOSED)) {
		set_bit(HCI_CONN_POWER_SAVE, &acl->flags);
		hci_conn_enter_active_mode(acl, BT_POWER_FORCE_ACTIVE_ON);
=======
>>>>>>> refs/remotes/origin/cm-11.0

		if (test_bit(HCI_CONN_MODE_CHANGE_PEND, &acl->pend)) {
			/* defer SCO setup until mode change completed */
<<<<<<< HEAD
			set_bit(HCI_CONN_SCO_SETUP_PEND, &acl->flags);
>>>>>>> refs/remotes/origin/master
=======
			set_bit(HCI_CONN_SCO_SETUP_PEND, &acl->pend);
>>>>>>> refs/remotes/origin/cm-11.0
			return sco;
		}

		hci_sco_setup(acl, 0x00);
	}

	return sco;
}
<<<<<<< HEAD
EXPORT_SYMBOL(hci_connect);

void hci_disconnect(struct hci_conn *conn, __u8 reason)
{
	BT_DBG("conn %p", conn);

	hci_proto_disconn_cfm(conn, reason, 0);
}
EXPORT_SYMBOL(hci_disconnect);

void hci_disconnect_amp(struct hci_conn *conn, __u8 reason)
{
	struct hci_dev *hdev = NULL;

	BT_DBG("conn %p", conn);

	read_lock_bh(&hci_dev_list_lock);

	list_for_each_entry(hdev, &hci_dev_list, list) {
		struct hci_conn *c;
		if (hdev == conn->hdev)
			continue;
		if (hdev->amp_type == HCI_BREDR)
			continue;
		c = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &conn->dst);
		if (c)
			hci_disconnect(c, reason);
	}

	read_unlock_bh(&hci_dev_list_lock);
<<<<<<< HEAD
=======

/* Create SCO, ACL or LE connection. */
struct hci_conn *hci_connect(struct hci_dev *hdev, int type, bdaddr_t *dst,
			     __u8 dst_type, __u8 sec_level, __u8 auth_type)
{
	BT_DBG("%s dst %pMR type 0x%x", hdev->name, dst, type);

	switch (type) {
	case LE_LINK:
		return hci_connect_le(hdev, dst, dst_type, sec_level, auth_type);
	case ACL_LINK:
		return hci_connect_acl(hdev, dst, sec_level, auth_type);
	}

	return ERR_PTR(-EINVAL);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

/* Check link security requirement */
int hci_conn_check_link_mode(struct hci_conn *conn)
{
<<<<<<< HEAD
	BT_DBG("conn %p", conn);

	if (conn->ssp_mode > 0 && conn->hdev->ssp_mode > 0 &&
					!(conn->link_mode & HCI_LM_ENCRYPT))
<<<<<<< HEAD
=======
	BT_DBG("hcon %p", conn);

	if (hci_conn_ssp_enabled(conn) && !(conn->link_mode & HCI_LM_ENCRYPT))
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		return 0;

	return 1;
}
<<<<<<< HEAD
EXPORT_SYMBOL(hci_conn_check_link_mode);
=======
>>>>>>> refs/remotes/origin/master

/* Authenticate remote device */
static int hci_conn_auth(struct hci_conn *conn, __u8 sec_level, __u8 auth_type)
{
<<<<<<< HEAD
	BT_DBG("conn %p", conn);
=======
	BT_DBG("hcon %p", conn);
>>>>>>> refs/remotes/origin/master

	if (conn->pending_sec_level > sec_level)
		sec_level = conn->pending_sec_level;

	if (sec_level > conn->sec_level)
		conn->pending_sec_level = sec_level;
	else if (conn->link_mode & HCI_LM_AUTH)
		return 1;

	/* Make sure we preserve an existing MITM requirement*/
	auth_type |= (conn->auth_type & 0x01);
<<<<<<< HEAD
<<<<<<< HEAD
	conn->auth_type = auth_type;
	conn->auth_initiator = 1;

	if (!test_and_set_bit(HCI_CONN_AUTH_PEND, &conn->pend)) {
		struct hci_cp_auth_requested cp;

		/* encrypt must be pending if auth is also pending */
		set_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend);

		cp.handle = cpu_to_le16(conn->handle);
		hci_send_cmd(conn->hdev, HCI_OP_AUTH_REQUESTED,
							sizeof(cp), &cp);
=======

=======
>>>>>>> refs/remotes/origin/cm-11.0
	conn->auth_type = auth_type;
	conn->auth_initiator = 1;

	if (!test_and_set_bit(HCI_CONN_AUTH_PEND, &conn->pend)) {
		struct hci_cp_auth_requested cp;

		/* encrypt must be pending if auth is also pending */
		set_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend);

		cp.handle = cpu_to_le16(conn->handle);
		hci_send_cmd(conn->hdev, HCI_OP_AUTH_REQUESTED,
<<<<<<< HEAD
			     sizeof(cp), &cp);
		if (conn->key_type != 0xff)
			set_bit(HCI_CONN_REAUTH_PEND, &conn->flags);
>>>>>>> refs/remotes/origin/master
=======
							sizeof(cp), &cp);
>>>>>>> refs/remotes/origin/cm-11.0
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Enable security */
int hci_conn_security(struct hci_conn *conn, __u8 sec_level, __u8 auth_type)
{
	BT_DBG("conn %p %d %d", conn, sec_level, auth_type);

	if (sec_level == BT_SECURITY_SDP)
		return 1;

	if (sec_level == BT_SECURITY_LOW &&
				(!conn->ssp_mode || !conn->hdev->ssp_mode))
		return 1;

	if (conn->type == LE_LINK) {
		if (conn->pending_sec_level > sec_level)
			sec_level = conn->pending_sec_level;

		if (sec_level > conn->sec_level)
			conn->pending_sec_level = sec_level;
		hci_proto_connect_cfm(conn, 0);
		return 0;
	} else if (conn->link_mode & HCI_LM_ENCRYPT) {
		return hci_conn_auth(conn, sec_level, auth_type);
	} else if (test_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend)) {
		return 0;
	}

	if (hci_conn_auth(conn, sec_level, auth_type)) {
		struct hci_cp_set_conn_encrypt cp;
		if (timer_pending(&conn->encrypt_pause_timer)) {
			BT_INFO("encrypt_pause_timer is pending");
			return 0;
		}
		cp.handle  = cpu_to_le16(conn->handle);
		cp.encrypt = 1;
		hci_send_cmd(conn->hdev, HCI_OP_SET_CONN_ENCRYPT,
							sizeof(cp), &cp);
	}

=======
/* Encrypt the the link */
static void hci_conn_encrypt(struct hci_conn *conn)
{
	BT_DBG("hcon %p", conn);

	if (!test_and_set_bit(HCI_CONN_ENCRYPT_PEND, &conn->flags)) {
		struct hci_cp_set_conn_encrypt cp;
		cp.handle  = cpu_to_le16(conn->handle);
		cp.encrypt = 0x01;
		hci_send_cmd(conn->hdev, HCI_OP_SET_CONN_ENCRYPT, sizeof(cp),
			     &cp);
	}
}

/* Enable security */
int hci_conn_security(struct hci_conn *conn, __u8 sec_level, __u8 auth_type)
{
	BT_DBG("hcon %p", conn);

	if (conn->type == LE_LINK)
		return smp_conn_security(conn, sec_level);
=======
/* Enable security */
int hci_conn_security(struct hci_conn *conn, __u8 sec_level, __u8 auth_type)
{
	BT_DBG("conn %p %d %d", conn, sec_level, auth_type);
>>>>>>> refs/remotes/origin/cm-11.0

	if (sec_level == BT_SECURITY_SDP)
		return 1;

	if (sec_level == BT_SECURITY_LOW &&
				(!conn->ssp_mode || !conn->hdev->ssp_mode))
		return 1;

<<<<<<< HEAD
	/* For other security levels we need the link key. */
	if (!(conn->link_mode & HCI_LM_AUTH))
		goto auth;

	/* An authenticated combination key has sufficient security for any
	   security level. */
	if (conn->key_type == HCI_LK_AUTH_COMBINATION)
		goto encrypt;

	/* An unauthenticated combination key has sufficient security for
	   security level 1 and 2. */
	if (conn->key_type == HCI_LK_UNAUTH_COMBINATION &&
	    (sec_level == BT_SECURITY_MEDIUM || sec_level == BT_SECURITY_LOW))
		goto encrypt;

	/* A combination key has always sufficient security for the security
	   levels 1 or 2. High security level requires the combination key
	   is generated using maximum PIN code length (16).
	   For pre 2.1 units. */
	if (conn->key_type == HCI_LK_COMBINATION &&
	    (sec_level != BT_SECURITY_HIGH || conn->pin_length == 16))
		goto encrypt;

auth:
	if (test_bit(HCI_CONN_ENCRYPT_PEND, &conn->flags))
		return 0;
=======
	if (conn->type == LE_LINK) {
		if (conn->pending_sec_level > sec_level)
			sec_level = conn->pending_sec_level;
>>>>>>> refs/remotes/origin/cm-11.0

		if (sec_level > conn->sec_level)
			conn->pending_sec_level = sec_level;
		hci_proto_connect_cfm(conn, 0);
		return 0;
	} else if (conn->link_mode & HCI_LM_ENCRYPT) {
		return hci_conn_auth(conn, sec_level, auth_type);
	} else if (test_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend)) {
		return 0;
	}

	if (hci_conn_auth(conn, sec_level, auth_type)) {
		struct hci_cp_set_conn_encrypt cp;
		if (timer_pending(&conn->encrypt_pause_timer)) {
			BT_INFO("encrypt_pause_timer is pending");
			return 0;
		}
		cp.handle  = cpu_to_le16(conn->handle);
		cp.encrypt = 1;
		hci_send_cmd(conn->hdev, HCI_OP_SET_CONN_ENCRYPT,
							sizeof(cp), &cp);
	}

<<<<<<< HEAD
	hci_conn_encrypt(conn);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	return 0;
}
EXPORT_SYMBOL(hci_conn_security);

<<<<<<< HEAD
<<<<<<< HEAD
/* Change link key */
int hci_conn_change_link_key(struct hci_conn *conn)
{
	BT_DBG("conn %p", conn);

	if (!test_and_set_bit(HCI_CONN_AUTH_PEND, &conn->pend)) {
		struct hci_cp_change_conn_link_key cp;
		cp.handle = cpu_to_le16(conn->handle);
		hci_send_cmd(conn->hdev, HCI_OP_CHANGE_CONN_LINK_KEY,
							sizeof(cp), &cp);
=======
/* Check secure link requirement */
int hci_conn_check_secure(struct hci_conn *conn, __u8 sec_level)
{
	BT_DBG("hcon %p", conn);

	if (sec_level != BT_SECURITY_HIGH)
		return 1; /* Accept if non-secure is required */

	if (conn->sec_level == BT_SECURITY_HIGH)
		return 1;

	return 0; /* Reject not secure link */
}
EXPORT_SYMBOL(hci_conn_check_secure);

=======
>>>>>>> refs/remotes/origin/cm-11.0
/* Change link key */
int hci_conn_change_link_key(struct hci_conn *conn)
{
	BT_DBG("hcon %p", conn);

	if (!test_and_set_bit(HCI_CONN_AUTH_PEND, &conn->pend)) {
		struct hci_cp_change_conn_link_key cp;
		cp.handle = cpu_to_le16(conn->handle);
		hci_send_cmd(conn->hdev, HCI_OP_CHANGE_CONN_LINK_KEY,
			     sizeof(cp), &cp);
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL(hci_conn_change_link_key);
=======
>>>>>>> refs/remotes/origin/master

/* Switch role */
int hci_conn_switch_role(struct hci_conn *conn, __u8 role)
{
<<<<<<< HEAD
	BT_DBG("conn %p", conn);
=======
	BT_DBG("hcon %p", conn);
>>>>>>> refs/remotes/origin/master

	if (!role && conn->link_mode & HCI_LM_MASTER)
		return 1;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!test_and_set_bit(HCI_CONN_RSWITCH_PEND, &conn->pend)) {
=======
	if (!test_and_set_bit(HCI_CONN_RSWITCH_PEND, &conn->flags)) {
>>>>>>> refs/remotes/origin/master
=======
	if (!test_and_set_bit(HCI_CONN_RSWITCH_PEND, &conn->pend)) {
>>>>>>> refs/remotes/origin/cm-11.0
		struct hci_cp_switch_role cp;
		bacpy(&cp.bdaddr, &conn->dst);
		cp.role = role;
		hci_send_cmd(conn->hdev, HCI_OP_SWITCH_ROLE, sizeof(cp), &cp);
	}

	return 0;
}
EXPORT_SYMBOL(hci_conn_switch_role);

/* Enter active mode */
void hci_conn_enter_active_mode(struct hci_conn *conn, __u8 force_active)
{
	struct hci_dev *hdev = conn->hdev;

<<<<<<< HEAD
	BT_DBG("conn %p mode %d", conn, conn->mode);
=======
	BT_DBG("hcon %p mode %d", conn, conn->mode);
>>>>>>> refs/remotes/origin/master

	if (test_bit(HCI_RAW, &hdev->flags))
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	if (conn->type == LE_LINK)
		return;

	if (conn->mode != HCI_CM_SNIFF)
		goto timer;

	if (!conn->power_save && !force_active)
		goto timer;

	if (!test_and_set_bit(HCI_CONN_MODE_CHANGE_PEND, &conn->pend)) {
=======
=======
	if (conn->type == LE_LINK)
		return;

>>>>>>> refs/remotes/origin/cm-11.0
	if (conn->mode != HCI_CM_SNIFF)
		goto timer;

	if (!conn->power_save && !force_active)
		goto timer;

<<<<<<< HEAD
	if (!test_and_set_bit(HCI_CONN_MODE_CHANGE_PEND, &conn->flags)) {
>>>>>>> refs/remotes/origin/master
=======
	if (!test_and_set_bit(HCI_CONN_MODE_CHANGE_PEND, &conn->pend)) {
>>>>>>> refs/remotes/origin/cm-11.0
		struct hci_cp_exit_sniff_mode cp;
		cp.handle = cpu_to_le16(conn->handle);
		hci_send_cmd(hdev, HCI_OP_EXIT_SNIFF_MODE, sizeof(cp), &cp);
	}

timer:
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (hdev->idle_timeout > 0) {
		spin_lock_bh(&conn->lock);
		if (conn->conn_valid) {
			mod_timer(&conn->idle_timer,
				jiffies + msecs_to_jiffies(hdev->idle_timeout));
<<<<<<< HEAD
<<<<<<< HEAD
=======
			wake_lock(&conn->idle_lock);
>>>>>>> refs/remotes/origin/cm-10.0
		}
		spin_unlock_bh(&conn->lock);
	}
}

static inline void hci_conn_stop_rssi_timer(struct hci_conn *conn)
{
	BT_DBG("conn %p", conn);
	cancel_delayed_work(&conn->rssi_update_work);
}

static inline void hci_conn_start_rssi_timer(struct hci_conn *conn,
	u16 interval)
{
	struct hci_dev *hdev = conn->hdev;
	BT_DBG("conn %p, pending %d", conn,
			delayed_work_pending(&conn->rssi_update_work));
	if (!delayed_work_pending(&conn->rssi_update_work)) {
		queue_delayed_work(hdev->workqueue, &conn->rssi_update_work,
				msecs_to_jiffies(interval));
	}
}

void hci_conn_set_rssi_reporter(struct hci_conn *conn,
	s8 rssi_threshold, u16 interval, u8 updateOnThreshExceed)
{
	if (conn) {
		conn->rssi_threshold = rssi_threshold;
		conn->rssi_update_interval = interval;
		conn->rssi_update_thresh_exceed = updateOnThreshExceed;
		hci_conn_start_rssi_timer(conn, interval);
	}
}

void hci_conn_unset_rssi_reporter(struct hci_conn *conn)
{
	if (conn) {
		BT_DBG("Deleting the rssi_update_timer");
		hci_conn_stop_rssi_timer(conn);
	}
}

/* Enter sniff mode */
void hci_conn_enter_sniff_mode(struct hci_conn *conn)
{
	struct hci_dev *hdev = conn->hdev;

	BT_DBG("conn %p mode %d", conn, conn->mode);

	if (test_bit(HCI_RAW, &hdev->flags))
		return;

	if (conn->type == LE_LINK)
		return;

	if (!lmp_sniff_capable(hdev) || !lmp_sniff_capable(conn))
		return;

	if (conn->mode != HCI_CM_ACTIVE ||
		!(conn->link_policy & HCI_LP_SNIFF) ||
		(hci_find_link_key(hdev, &conn->dst) == NULL))
		return;

	if (lmp_sniffsubr_capable(hdev) && lmp_sniffsubr_capable(conn)) {
		struct hci_cp_sniff_subrate cp;
		cp.handle             = cpu_to_le16(conn->handle);
		cp.max_latency        = cpu_to_le16(0);
		cp.min_remote_timeout = cpu_to_le16(0);
		cp.min_local_timeout  = cpu_to_le16(0);
		hci_send_cmd(hdev, HCI_OP_SNIFF_SUBRATE, sizeof(cp), &cp);
	}

	if (!test_and_set_bit(HCI_CONN_MODE_CHANGE_PEND, &conn->pend)) {
		struct hci_cp_sniff_mode cp;
		cp.handle       = cpu_to_le16(conn->handle);
		cp.max_interval = cpu_to_le16(hdev->sniff_max_interval);
		cp.min_interval = cpu_to_le16(hdev->sniff_min_interval);
		cp.attempt      = cpu_to_le16(4);
		cp.timeout      = cpu_to_le16(1);
		hci_send_cmd(hdev, HCI_OP_SNIFF_MODE, sizeof(cp), &cp);
	}
}

struct hci_chan *hci_chan_create(struct hci_chan *chan,
			struct hci_ext_fs *tx_fs, struct hci_ext_fs *rx_fs)
{
	struct hci_cp_create_logical_link cp;

	chan->state = BT_CONNECT;
	chan->tx_fs = *tx_fs;
	chan->rx_fs = *rx_fs;
	cp.phy_handle = chan->conn->handle;
	cp.tx_fs.id = chan->tx_fs.id;
	cp.tx_fs.type = chan->tx_fs.type;
	cp.tx_fs.max_sdu = cpu_to_le16(chan->tx_fs.max_sdu);
	cp.tx_fs.sdu_arr_time = cpu_to_le32(chan->tx_fs.sdu_arr_time);
	cp.tx_fs.acc_latency = cpu_to_le32(chan->tx_fs.acc_latency);
	cp.tx_fs.flush_to = cpu_to_le32(chan->tx_fs.flush_to);
	cp.rx_fs.id = chan->rx_fs.id;
	cp.rx_fs.type = chan->rx_fs.type;
	cp.rx_fs.max_sdu = cpu_to_le16(chan->rx_fs.max_sdu);
	cp.rx_fs.sdu_arr_time = cpu_to_le32(chan->rx_fs.sdu_arr_time);
	cp.rx_fs.acc_latency = cpu_to_le32(chan->rx_fs.acc_latency);
	cp.rx_fs.flush_to = cpu_to_le32(chan->rx_fs.flush_to);
	hci_conn_hold(chan->conn);
	if (chan->conn->out)
		hci_send_cmd(chan->conn->hdev, HCI_OP_CREATE_LOGICAL_LINK,
							sizeof(cp), &cp);
	else
		hci_send_cmd(chan->conn->hdev, HCI_OP_ACCEPT_LOGICAL_LINK,
							sizeof(cp), &cp);
	return chan;
}
EXPORT_SYMBOL(hci_chan_create);

void hci_chan_modify(struct hci_chan *chan,
			struct hci_ext_fs *tx_fs, struct hci_ext_fs *rx_fs)
{
	struct hci_cp_flow_spec_modify cp;

	chan->tx_fs = *tx_fs;
	chan->rx_fs = *rx_fs;
	cp.log_handle = cpu_to_le16(chan->ll_handle);
	cp.tx_fs.id = tx_fs->id;
	cp.tx_fs.type = tx_fs->type;
	cp.tx_fs.max_sdu = cpu_to_le16(tx_fs->max_sdu);
	cp.tx_fs.sdu_arr_time = cpu_to_le32(tx_fs->sdu_arr_time);
	cp.tx_fs.acc_latency = cpu_to_le32(tx_fs->acc_latency);
	cp.tx_fs.flush_to = cpu_to_le32(tx_fs->flush_to);
	cp.rx_fs.id = rx_fs->id;
	cp.rx_fs.type = rx_fs->type;
	cp.rx_fs.max_sdu = cpu_to_le16(rx_fs->max_sdu);
	cp.rx_fs.sdu_arr_time = cpu_to_le32(rx_fs->sdu_arr_time);
	cp.rx_fs.acc_latency = cpu_to_le32(rx_fs->acc_latency);
	cp.rx_fs.flush_to = cpu_to_le32(rx_fs->flush_to);
	hci_conn_hold(chan->conn);
	hci_send_cmd(chan->conn->hdev, HCI_OP_FLOW_SPEC_MODIFY, sizeof(cp),
									&cp);
}
EXPORT_SYMBOL(hci_chan_modify);

/* Drop all connection on the device */
void hci_conn_hash_flush(struct hci_dev *hdev, u8 is_process)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	struct list_head *p;

	BT_DBG("hdev %s", hdev->name);

	p = h->list.next;
	while (p != &h->list) {
		struct hci_conn *c;

		c = list_entry(p, struct hci_conn, list);
		p = p->next;

		c->state = BT_CLOSED;

		hci_proto_disconn_cfm(c, 0x16, is_process);
=======
	if (hdev->idle_timeout > 0)
		queue_delayed_work(hdev->workqueue, &conn->idle_work,
				   msecs_to_jiffies(hdev->idle_timeout));
=======
			wake_lock(&conn->idle_lock);
		}
		spin_unlock_bh(&conn->lock);
	}
>>>>>>> refs/remotes/origin/cm-11.0
}

static inline void hci_conn_stop_rssi_timer(struct hci_conn *conn)
{
	BT_DBG("conn %p", conn);
	cancel_delayed_work(&conn->rssi_update_work);
}

static inline void hci_conn_start_rssi_timer(struct hci_conn *conn,
	u16 interval)
{
	struct hci_dev *hdev = conn->hdev;
	BT_DBG("conn %p, pending %d", conn,
			delayed_work_pending(&conn->rssi_update_work));
	if (!delayed_work_pending(&conn->rssi_update_work)) {
		queue_delayed_work(hdev->workqueue, &conn->rssi_update_work,
				msecs_to_jiffies(interval));
	}
}

void hci_conn_set_rssi_reporter(struct hci_conn *conn,
	s8 rssi_threshold, u16 interval, u8 updateOnThreshExceed)
{
	if (conn) {
		conn->rssi_threshold = rssi_threshold;
		conn->rssi_update_interval = interval;
		conn->rssi_update_thresh_exceed = updateOnThreshExceed;
		hci_conn_start_rssi_timer(conn, interval);
	}
}

void hci_conn_unset_rssi_reporter(struct hci_conn *conn)
{
	if (conn) {
		BT_DBG("Deleting the rssi_update_timer");
		hci_conn_stop_rssi_timer(conn);
	}
}

/* Enter sniff mode */
void hci_conn_enter_sniff_mode(struct hci_conn *conn)
{
	struct hci_dev *hdev = conn->hdev;

	BT_DBG("conn %p mode %d", conn, conn->mode);

	if (test_bit(HCI_RAW, &hdev->flags))
		return;

	if (conn->type == LE_LINK)
		return;

	if (!lmp_sniff_capable(hdev) || !lmp_sniff_capable(conn))
		return;

	if (conn->mode != HCI_CM_ACTIVE ||
		!(conn->link_policy & HCI_LP_SNIFF) ||
		(hci_find_link_key(hdev, &conn->dst) == NULL))
		return;

	if (lmp_sniffsubr_capable(hdev) && lmp_sniffsubr_capable(conn)) {
		struct hci_cp_sniff_subrate cp;
		cp.handle             = cpu_to_le16(conn->handle);
		cp.max_latency        = cpu_to_le16(0);
		cp.min_remote_timeout = cpu_to_le16(0);
		cp.min_local_timeout  = cpu_to_le16(0);
		hci_send_cmd(hdev, HCI_OP_SNIFF_SUBRATE, sizeof(cp), &cp);
	}

	if (!test_and_set_bit(HCI_CONN_MODE_CHANGE_PEND, &conn->pend)) {
		struct hci_cp_sniff_mode cp;
		cp.handle       = cpu_to_le16(conn->handle);
		cp.max_interval = cpu_to_le16(hdev->sniff_max_interval);
		cp.min_interval = cpu_to_le16(hdev->sniff_min_interval);
		cp.attempt      = cpu_to_le16(4);
		cp.timeout      = cpu_to_le16(1);
		hci_send_cmd(hdev, HCI_OP_SNIFF_MODE, sizeof(cp), &cp);
	}
}

struct hci_chan *hci_chan_create(struct hci_chan *chan,
			struct hci_ext_fs *tx_fs, struct hci_ext_fs *rx_fs)
{
	struct hci_cp_create_logical_link cp;

	chan->state = BT_CONNECT;
	chan->tx_fs = *tx_fs;
	chan->rx_fs = *rx_fs;
	cp.phy_handle = chan->conn->handle;
	cp.tx_fs.id = chan->tx_fs.id;
	cp.tx_fs.type = chan->tx_fs.type;
	cp.tx_fs.max_sdu = cpu_to_le16(chan->tx_fs.max_sdu);
	cp.tx_fs.sdu_arr_time = cpu_to_le32(chan->tx_fs.sdu_arr_time);
	cp.tx_fs.acc_latency = cpu_to_le32(chan->tx_fs.acc_latency);
	cp.tx_fs.flush_to = cpu_to_le32(chan->tx_fs.flush_to);
	cp.rx_fs.id = chan->rx_fs.id;
	cp.rx_fs.type = chan->rx_fs.type;
	cp.rx_fs.max_sdu = cpu_to_le16(chan->rx_fs.max_sdu);
	cp.rx_fs.sdu_arr_time = cpu_to_le32(chan->rx_fs.sdu_arr_time);
	cp.rx_fs.acc_latency = cpu_to_le32(chan->rx_fs.acc_latency);
	cp.rx_fs.flush_to = cpu_to_le32(chan->rx_fs.flush_to);
	hci_conn_hold(chan->conn);
	if (chan->conn->out)
		hci_send_cmd(chan->conn->hdev, HCI_OP_CREATE_LOGICAL_LINK,
							sizeof(cp), &cp);
	else
		hci_send_cmd(chan->conn->hdev, HCI_OP_ACCEPT_LOGICAL_LINK,
							sizeof(cp), &cp);
	return chan;
}
EXPORT_SYMBOL(hci_chan_create);

void hci_chan_modify(struct hci_chan *chan,
			struct hci_ext_fs *tx_fs, struct hci_ext_fs *rx_fs)
{
	struct hci_cp_flow_spec_modify cp;

	chan->tx_fs = *tx_fs;
	chan->rx_fs = *rx_fs;
	cp.log_handle = cpu_to_le16(chan->ll_handle);
	cp.tx_fs.id = tx_fs->id;
	cp.tx_fs.type = tx_fs->type;
	cp.tx_fs.max_sdu = cpu_to_le16(tx_fs->max_sdu);
	cp.tx_fs.sdu_arr_time = cpu_to_le32(tx_fs->sdu_arr_time);
	cp.tx_fs.acc_latency = cpu_to_le32(tx_fs->acc_latency);
	cp.tx_fs.flush_to = cpu_to_le32(tx_fs->flush_to);
	cp.rx_fs.id = rx_fs->id;
	cp.rx_fs.type = rx_fs->type;
	cp.rx_fs.max_sdu = cpu_to_le16(rx_fs->max_sdu);
	cp.rx_fs.sdu_arr_time = cpu_to_le32(rx_fs->sdu_arr_time);
	cp.rx_fs.acc_latency = cpu_to_le32(rx_fs->acc_latency);
	cp.rx_fs.flush_to = cpu_to_le32(rx_fs->flush_to);
	hci_conn_hold(chan->conn);
	hci_send_cmd(chan->conn->hdev, HCI_OP_FLOW_SPEC_MODIFY, sizeof(cp),
									&cp);
}
EXPORT_SYMBOL(hci_chan_modify);

/* Drop all connection on the device */
void hci_conn_hash_flush(struct hci_dev *hdev, u8 is_process)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	struct list_head *p;

	BT_DBG("hdev %s", hdev->name);

	p = h->list.next;
	while (p != &h->list) {
		struct hci_conn *c;

		c = list_entry(p, struct hci_conn, list);
		p = p->next;

		c->state = BT_CLOSED;

<<<<<<< HEAD
		hci_proto_disconn_cfm(c, HCI_ERROR_LOCAL_HOST_TERM);
>>>>>>> refs/remotes/origin/master
=======
		hci_proto_disconn_cfm(c, 0x16, is_process);
>>>>>>> refs/remotes/origin/cm-11.0
		hci_conn_del(c);
	}
}

/* Check pending connect attempts */
void hci_conn_check_pending(struct hci_dev *hdev)
{
	struct hci_conn *conn;

	BT_DBG("hdev %s", hdev->name);

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_state(hdev, ACL_LINK, BT_CONNECT2);
	if (conn)
<<<<<<< HEAD
		hci_acl_connect(conn);
=======
		hci_acl_create_connection(conn);
>>>>>>> refs/remotes/origin/master

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
void hci_conn_hold_device(struct hci_conn *conn)
{
	atomic_inc(&conn->devref);
}
EXPORT_SYMBOL(hci_conn_hold_device);

void hci_conn_put_device(struct hci_conn *conn)
{
<<<<<<< HEAD
	if (atomic_dec_and_test(&conn->devref)) {
		conn->hidp_session_valid = false;
		hci_conn_del_sysfs(conn);
	}
=======
	if (atomic_dec_and_test(&conn->devref))
		hci_conn_del_sysfs(conn);
>>>>>>> refs/remotes/origin/cm-10.0
}
EXPORT_SYMBOL(hci_conn_put_device);

int hci_get_conn_list(void __user *arg)
{
	struct hci_conn_list_req req, *cl;
	struct hci_conn_info *ci;
	struct hci_dev *hdev;
	struct list_head *p;
<<<<<<< HEAD
=======
int hci_get_conn_list(void __user *arg)
{
	struct hci_conn *c;
	struct hci_conn_list_req req, *cl;
	struct hci_conn_info *ci;
	struct hci_dev *hdev;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	int n = 0, size, err;

	if (copy_from_user(&req, arg, sizeof(req)))
		return -EFAULT;

	if (!req.conn_num || req.conn_num > (PAGE_SIZE * 2) / sizeof(*ci))
		return -EINVAL;

	size = sizeof(req) + req.conn_num * sizeof(*ci);

	cl = kmalloc(size, GFP_KERNEL);
	if (!cl)
		return -ENOMEM;

	hdev = hci_dev_get(req.dev_id);
	if (!hdev) {
		kfree(cl);
		return -ENODEV;
	}

	ci = cl->conn_info;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	hci_dev_lock_bh(hdev);
	list_for_each(p, &hdev->conn_hash.list) {
		register struct hci_conn *c;
		c = list_entry(p, struct hci_conn, list);

<<<<<<< HEAD
=======
	hci_dev_lock(hdev);
	list_for_each_entry(c, &hdev->conn_hash.list, list) {
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		bacpy(&(ci + n)->bdaddr, &c->dst);
		(ci + n)->handle = c->handle;
		(ci + n)->type  = c->type;
		(ci + n)->out   = c->out;
		(ci + n)->state = c->state;
		(ci + n)->link_mode = c->link_mode;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		if (c->type == SCO_LINK) {
			(ci + n)->mtu = hdev->sco_mtu;
			(ci + n)->cnt = hdev->sco_cnt;
			(ci + n)->pkts = hdev->sco_pkts;
		} else {
			(ci + n)->mtu = hdev->acl_mtu;
			(ci + n)->cnt = hdev->acl_cnt;
			(ci + n)->pkts = hdev->acl_pkts;
		}
<<<<<<< HEAD
		if (++n >= req.conn_num)
			break;
	}
	hci_dev_unlock_bh(hdev);
=======
		if (++n >= req.conn_num)
			break;
	}
	hci_dev_unlock(hdev);
>>>>>>> refs/remotes/origin/master
=======
		if (++n >= req.conn_num)
			break;
	}
	hci_dev_unlock_bh(hdev);
>>>>>>> refs/remotes/origin/cm-11.0

	cl->dev_id = hdev->id;
	cl->conn_num = n;
	size = sizeof(req) + n * sizeof(*ci);

	hci_dev_put(hdev);

	err = copy_to_user(arg, cl, size);
	kfree(cl);

	return err ? -EFAULT : 0;
}

int hci_get_conn_info(struct hci_dev *hdev, void __user *arg)
{
	struct hci_conn_info_req req;
	struct hci_conn_info ci;
	struct hci_conn *conn;
	char __user *ptr = arg + sizeof(req);

	if (copy_from_user(&req, arg, sizeof(req)))
		return -EFAULT;

<<<<<<< HEAD
<<<<<<< HEAD
	hci_dev_lock_bh(hdev);
=======
	hci_dev_lock(hdev);
>>>>>>> refs/remotes/origin/master
=======
	hci_dev_lock_bh(hdev);
>>>>>>> refs/remotes/origin/cm-11.0
	conn = hci_conn_hash_lookup_ba(hdev, req.type, &req.bdaddr);
	if (conn) {
		bacpy(&ci.bdaddr, &conn->dst);
		ci.handle = conn->handle;
		ci.type  = conn->type;
		ci.out   = conn->out;
		ci.state = conn->state;
		ci.link_mode = conn->link_mode;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		if (req.type == SCO_LINK) {
			ci.mtu = hdev->sco_mtu;
			ci.cnt = hdev->sco_cnt;
			ci.pkts = hdev->sco_pkts;
		} else {
			ci.mtu = hdev->acl_mtu;
			ci.cnt = hdev->acl_cnt;
			ci.pkts = hdev->acl_pkts;
		}
		ci.pending_sec_level = conn->pending_sec_level;
		ci.ssp_mode = conn->ssp_mode;
<<<<<<< HEAD
	}
	hci_dev_unlock_bh(hdev);
=======
	}
	hci_dev_unlock(hdev);
>>>>>>> refs/remotes/origin/master
=======
	}
	hci_dev_unlock_bh(hdev);
>>>>>>> refs/remotes/origin/cm-11.0

	if (!conn)
		return -ENOENT;

	return copy_to_user(ptr, &ci, sizeof(ci)) ? -EFAULT : 0;
}

int hci_get_auth_info(struct hci_dev *hdev, void __user *arg)
{
	struct hci_auth_info_req req;
	struct hci_conn *conn;

	if (copy_from_user(&req, arg, sizeof(req)))
		return -EFAULT;

<<<<<<< HEAD
<<<<<<< HEAD
	hci_dev_lock_bh(hdev);
	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &req.bdaddr);
	if (conn)
		req.type = conn->auth_type;
	hci_dev_unlock_bh(hdev);
=======
	hci_dev_lock(hdev);
	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &req.bdaddr);
	if (conn)
		req.type = conn->auth_type;
	hci_dev_unlock(hdev);
>>>>>>> refs/remotes/origin/master
=======
	hci_dev_lock_bh(hdev);
	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &req.bdaddr);
	if (conn)
		req.type = conn->auth_type;
	hci_dev_unlock_bh(hdev);
>>>>>>> refs/remotes/origin/cm-11.0

	if (!conn)
		return -ENOENT;

	return copy_to_user(arg, &req, sizeof(req)) ? -EFAULT : 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
int hci_set_auth_info(struct hci_dev *hdev, void __user *arg)
{
	struct hci_auth_info_req req;
	struct hci_conn *conn;

	if (copy_from_user(&req, arg, sizeof(req)))
		return -EFAULT;

	hci_dev_lock_bh(hdev);
	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &req.bdaddr);
<<<<<<< HEAD
	if (conn) {
		conn->auth_type = req.type;
		switch (conn->auth_type) {
		case HCI_AT_NO_BONDING:
			conn->pending_sec_level = BT_SECURITY_LOW;
			break;
		case HCI_AT_DEDICATED_BONDING:
		case HCI_AT_GENERAL_BONDING:
			conn->pending_sec_level = BT_SECURITY_MEDIUM;
			break;
		case HCI_AT_DEDICATED_BONDING_MITM:
		case HCI_AT_GENERAL_BONDING_MITM:
			conn->pending_sec_level = BT_SECURITY_HIGH;
			break;
		default:
			break;
		}
	}
=======
	if (conn)
		conn->auth_type = req.type;
>>>>>>> refs/remotes/origin/cm-10.0
	hci_dev_unlock_bh(hdev);

	if (!conn)
		return -ENOENT;

	return copy_to_user(arg, &req, sizeof(req)) ? -EFAULT : 0;
=======
struct hci_chan *hci_chan_create(struct hci_conn *conn)
{
	struct hci_dev *hdev = conn->hdev;
	struct hci_chan *chan;

	BT_DBG("%s hcon %p", hdev->name, conn);

	chan = kzalloc(sizeof(struct hci_chan), GFP_KERNEL);
	if (!chan)
		return NULL;

	chan->conn = conn;
	skb_queue_head_init(&chan->data_q);
	chan->state = BT_CONNECTED;

	list_add_rcu(&chan->list, &conn->chan_list);

	return chan;
}

void hci_chan_del(struct hci_chan *chan)
{
	struct hci_conn *conn = chan->conn;
	struct hci_dev *hdev = conn->hdev;

	BT_DBG("%s hcon %p chan %p", hdev->name, conn, chan);

	list_del_rcu(&chan->list);

	synchronize_rcu();

	hci_conn_drop(conn);

	skb_queue_purge(&chan->data_q);
	kfree(chan);
}
=======
int hci_set_auth_info(struct hci_dev *hdev, void __user *arg)
{
	struct hci_auth_info_req req;
	struct hci_conn *conn;

	if (copy_from_user(&req, arg, sizeof(req)))
		return -EFAULT;
>>>>>>> refs/remotes/origin/cm-11.0

	hci_dev_lock_bh(hdev);
	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &req.bdaddr);
	if (conn) {
		conn->auth_type = req.type;
		switch (conn->auth_type) {
		case HCI_AT_NO_BONDING:
			conn->pending_sec_level = BT_SECURITY_LOW;
			break;
		case HCI_AT_DEDICATED_BONDING:
		case HCI_AT_GENERAL_BONDING:
			conn->pending_sec_level = BT_SECURITY_MEDIUM;
			break;
		case HCI_AT_DEDICATED_BONDING_MITM:
		case HCI_AT_GENERAL_BONDING_MITM:
			conn->pending_sec_level = BT_SECURITY_HIGH;
			break;
		default:
			break;
		}
	}
	hci_dev_unlock_bh(hdev);

<<<<<<< HEAD
	BT_DBG("hcon %p", conn);
=======
	if (!conn)
		return -ENOENT;
>>>>>>> refs/remotes/origin/cm-11.0

	return copy_to_user(arg, &req, sizeof(req)) ? -EFAULT : 0;
}

static struct hci_chan *__hci_chan_lookup_handle(struct hci_conn *hcon,
						 __u16 handle)
{
	struct hci_chan *hchan;

	list_for_each_entry(hchan, &hcon->chan_list, list) {
		if (hchan->handle == handle)
			return hchan;
	}

	return NULL;
}

struct hci_chan *hci_chan_lookup_handle(struct hci_dev *hdev, __u16 handle)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	struct hci_conn *hcon;
	struct hci_chan *hchan = NULL;

	rcu_read_lock();

	list_for_each_entry_rcu(hcon, &h->list, list) {
		hchan = __hci_chan_lookup_handle(hcon, handle);
		if (hchan)
			break;
	}

	rcu_read_unlock();

	return hchan;
>>>>>>> refs/remotes/origin/master
}
