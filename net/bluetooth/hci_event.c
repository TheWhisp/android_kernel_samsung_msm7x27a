/*
   BlueZ - Bluetooth protocol stack for Linux
<<<<<<< HEAD
<<<<<<< HEAD
   Copyright (c) 2000-2001, 2010-2013 The Linux Foundation. All rights reserved.
=======
   Copyright (c) 2000-2001, 2010-2012 The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
   Copyright (c) 2000-2001, 2010, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master

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

/* Bluetooth HCI event handling. */

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
=======
>>>>>>> refs/remotes/origin/master
#include <asm/unaligned.h>

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>
<<<<<<< HEAD
=======
#include <net/bluetooth/mgmt.h>

#include "a2mp.h"
#include "amp.h"
>>>>>>> refs/remotes/origin/master

/* Handle HCI Event packets */

static void hci_cc_inquiry_cancel(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	if (status)
		return;

	clear_bit(HCI_INQUIRY, &hdev->flags);
<<<<<<< HEAD

	hci_req_complete(hdev, HCI_OP_INQUIRY_CANCEL, status);
=======
	smp_mb__after_clear_bit(); /* wake_up_bit advises about this barrier */
	wake_up_bit(&hdev->flags, HCI_INQUIRY);
>>>>>>> refs/remotes/origin/master

	hci_conn_check_pending(hdev);
}

<<<<<<< HEAD
static void hci_cc_exit_periodic_inq(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%x", hdev->name, status);
=======
static void hci_cc_periodic_inq(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	if (status)
		return;

<<<<<<< HEAD
	clear_bit(HCI_INQUIRY, &hdev->flags);

	hci_conn_check_pending(hdev);
}

static void hci_cc_link_key_reply(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_link_key_reply *rp = (void *) skb->data;
	struct hci_conn *conn;
	struct hci_cp_link_key_reply *cp;

	BT_DBG("%s status 0x%x", hdev->name, rp->status);
	if (rp->status)
		return;

	cp = hci_sent_cmd_data(hdev, HCI_OP_LINK_KEY_REPLY);
	if (!cp)
		return;

	hci_dev_lock(hdev);
	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &cp->bdaddr);
	if (conn) {
		hci_conn_hold(conn);
		memcpy(conn->link_key, cp->link_key, sizeof(conn->link_key));
		conn->key_type = 5;
		hci_conn_put(conn);
	}
	hci_dev_unlock(hdev);
}

static void hci_cc_remote_name_req_cancel(struct hci_dev *hdev, struct sk_buff *skb)
=======
	set_bit(HCI_PERIODIC_INQ, &hdev->dev_flags);
}

static void hci_cc_exit_periodic_inq(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%2.2x", hdev->name, status);

	if (status)
		return;

	clear_bit(HCI_PERIODIC_INQ, &hdev->dev_flags);

	hci_conn_check_pending(hdev);
}

static void hci_cc_remote_name_req_cancel(struct hci_dev *hdev,
					  struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	BT_DBG("%s", hdev->name);
}

static void hci_cc_role_discovery(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_role_discovery *rp = (void *) skb->data;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(rp->handle));
	if (conn) {
		if (rp->role)
			conn->link_mode &= ~HCI_LM_MASTER;
		else
			conn->link_mode |= HCI_LM_MASTER;
	}

	hci_dev_unlock(hdev);
}

static void hci_cc_read_link_policy(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_read_link_policy *rp = (void *) skb->data;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(rp->handle));
	if (conn)
		conn->link_policy = __le16_to_cpu(rp->policy);

	hci_dev_unlock(hdev);
}

static void hci_cc_write_link_policy(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_write_link_policy *rp = (void *) skb->data;
	struct hci_conn *conn;
	void *sent;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

	sent = hci_sent_cmd_data(hdev, HCI_OP_WRITE_LINK_POLICY);
	if (!sent)
		return;

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(rp->handle));
	if (conn)
		conn->link_policy = get_unaligned_le16(sent + 2);

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static void hci_cc_read_def_link_policy(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_read_def_link_policy *rp = (void *) skb->data;

	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
static void hci_cc_read_def_link_policy(struct hci_dev *hdev,
					struct sk_buff *skb)
{
	struct hci_rp_read_def_link_policy *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

	hdev->link_policy = __le16_to_cpu(rp->policy);
}

<<<<<<< HEAD
static void hci_cc_write_def_link_policy(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_cc_write_def_link_policy(struct hci_dev *hdev,
					 struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	__u8 status = *((__u8 *) skb->data);
	void *sent;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	sent = hci_sent_cmd_data(hdev, HCI_OP_WRITE_DEF_LINK_POLICY);
	if (!sent)
		return;

	if (!status)
		hdev->link_policy = get_unaligned_le16(sent);
<<<<<<< HEAD

	hci_req_complete(hdev, HCI_OP_WRITE_DEF_LINK_POLICY, status);
=======
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_reset(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);

	clear_bit(HCI_RESET, &hdev->flags);

	hci_req_complete(hdev, HCI_OP_RESET, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);

	clear_bit(HCI_RESET, &hdev->flags);

	/* Reset all non-persistent flags */
	hdev->dev_flags &= ~HCI_PERSISTENT_MASK;

	hdev->discovery.state = DISCOVERY_STOPPED;
	hdev->inq_tx_power = HCI_TX_POWER_INVALID;
	hdev->adv_tx_power = HCI_TX_POWER_INVALID;

	memset(hdev->adv_data, 0, sizeof(hdev->adv_data));
	hdev->adv_data_len = 0;

	memset(hdev->scan_rsp_data, 0, sizeof(hdev->scan_rsp_data));
	hdev->scan_rsp_data_len = 0;

	hdev->ssp_debug_mode = 0;
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_write_local_name(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);
	void *sent;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	sent = hci_sent_cmd_data(hdev, HCI_OP_WRITE_LOCAL_NAME);
	if (!sent)
		return;
<<<<<<< HEAD
	hci_dev_lock(hdev);
	if (!status)
		memcpy(hdev->dev_name, sent, HCI_MAX_NAME_LENGTH);

	if (test_bit(HCI_MGMT, &hdev->flags))
		mgmt_set_local_name_complete(hdev->id, sent, status);
=======

	hci_dev_lock(hdev);

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		mgmt_set_local_name_complete(hdev, sent, status);
	else if (!status)
		memcpy(hdev->dev_name, sent, HCI_MAX_NAME_LENGTH);

>>>>>>> refs/remotes/origin/master
	hci_dev_unlock(hdev);
}

static void hci_cc_read_local_name(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_read_local_name *rp = (void *) skb->data;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

<<<<<<< HEAD
	memcpy(hdev->dev_name, rp->name, HCI_MAX_NAME_LENGTH);
=======
	if (test_bit(HCI_SETUP, &hdev->dev_flags))
		memcpy(hdev->dev_name, rp->name, HCI_MAX_NAME_LENGTH);
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_write_auth_enable(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);
	void *sent;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	sent = hci_sent_cmd_data(hdev, HCI_OP_WRITE_AUTH_ENABLE);
	if (!sent)
		return;

	if (!status) {
		__u8 param = *((__u8 *) sent);

		if (param == AUTH_ENABLED)
			set_bit(HCI_AUTH, &hdev->flags);
		else
			clear_bit(HCI_AUTH, &hdev->flags);
	}

<<<<<<< HEAD
	hci_req_complete(hdev, HCI_OP_WRITE_AUTH_ENABLE, status);
=======
	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		mgmt_auth_enable_complete(hdev, status);
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_write_encrypt_mode(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);
	void *sent;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	sent = hci_sent_cmd_data(hdev, HCI_OP_WRITE_ENCRYPT_MODE);
	if (!sent)
		return;

	if (!status) {
		__u8 param = *((__u8 *) sent);

		if (param)
			set_bit(HCI_ENCRYPT, &hdev->flags);
		else
			clear_bit(HCI_ENCRYPT, &hdev->flags);
	}
<<<<<<< HEAD

	hci_req_complete(hdev, HCI_OP_WRITE_ENCRYPT_MODE, status);
=======
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_write_scan_enable(struct hci_dev *hdev, struct sk_buff *skb)
{
<<<<<<< HEAD
	__u8 status = *((__u8 *) skb->data);
	void *sent;

	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	__u8 param, status = *((__u8 *) skb->data);
	int old_pscan, old_iscan;
	void *sent;

	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	sent = hci_sent_cmd_data(hdev, HCI_OP_WRITE_SCAN_ENABLE);
	if (!sent)
		return;

<<<<<<< HEAD
	if (!status) {
		__u8 param = *((__u8 *) sent);
		int old_pscan, old_iscan;
		hci_dev_lock(hdev);

		old_pscan = test_and_clear_bit(HCI_PSCAN, &hdev->flags);
		old_iscan = test_and_clear_bit(HCI_ISCAN, &hdev->flags);

		if (param & SCAN_INQUIRY) {
			set_bit(HCI_ISCAN, &hdev->flags);
			if (!old_iscan)
				mgmt_discoverable(hdev->id, 1);
		} else if (old_iscan)
			mgmt_discoverable(hdev->id, 0);

		if (param & SCAN_PAGE) {
			set_bit(HCI_PSCAN, &hdev->flags);
			if (!old_pscan)
				mgmt_connectable(hdev->id, 1);
		} else if (old_pscan)
			mgmt_connectable(hdev->id, 0);
		hci_dev_unlock(hdev);
	}

	hci_req_complete(hdev, HCI_OP_WRITE_SCAN_ENABLE, status);
=======
	param = *((__u8 *) sent);

	hci_dev_lock(hdev);

	if (status) {
		mgmt_write_scan_failed(hdev, param, status);
		hdev->discov_timeout = 0;
		goto done;
	}

	/* We need to ensure that we set this back on if someone changed
	 * the scan mode through a raw HCI socket.
	 */
	set_bit(HCI_BREDR_ENABLED, &hdev->dev_flags);

	old_pscan = test_and_clear_bit(HCI_PSCAN, &hdev->flags);
	old_iscan = test_and_clear_bit(HCI_ISCAN, &hdev->flags);

	if (param & SCAN_INQUIRY) {
		set_bit(HCI_ISCAN, &hdev->flags);
		if (!old_iscan)
			mgmt_discoverable(hdev, 1);
	} else if (old_iscan)
		mgmt_discoverable(hdev, 0);

	if (param & SCAN_PAGE) {
		set_bit(HCI_PSCAN, &hdev->flags);
		if (!old_pscan)
			mgmt_connectable(hdev, 1);
	} else if (old_pscan)
		mgmt_connectable(hdev, 0);

done:
	hci_dev_unlock(hdev);
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_read_class_of_dev(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_read_class_of_dev *rp = (void *) skb->data;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

	memcpy(hdev->dev_class, rp->dev_class, 3);

	BT_DBG("%s class 0x%.2x%.2x%.2x", hdev->name,
<<<<<<< HEAD
		hdev->dev_class[2], hdev->dev_class[1], hdev->dev_class[0]);
=======
	       hdev->dev_class[2], hdev->dev_class[1], hdev->dev_class[0]);
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_write_class_of_dev(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);
	void *sent;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);

	if (status)
		return;
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	sent = hci_sent_cmd_data(hdev, HCI_OP_WRITE_CLASS_OF_DEV);
	if (!sent)
		return;

<<<<<<< HEAD
	memcpy(hdev->dev_class, sent, 3);
=======
	hci_dev_lock(hdev);

	if (status == 0)
		memcpy(hdev->dev_class, sent, 3);

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		mgmt_set_class_of_dev_complete(hdev, sent, status);

	hci_dev_unlock(hdev);
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_read_voice_setting(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_read_voice_setting *rp = (void *) skb->data;
	__u16 setting;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

	setting = __le16_to_cpu(rp->voice_setting);

	if (hdev->voice_setting == setting)
		return;

	hdev->voice_setting = setting;

<<<<<<< HEAD
	BT_DBG("%s voice setting 0x%04x", hdev->name, setting);

	if (hdev->notify) {
		tasklet_disable(&hdev->tx_task);
		hdev->notify(hdev, HCI_NOTIFY_VOICE_SETTING);
		tasklet_enable(&hdev->tx_task);
	}
}

static void hci_cc_write_voice_setting(struct hci_dev *hdev, struct sk_buff *skb)
=======
	BT_DBG("%s voice setting 0x%4.4x", hdev->name, setting);

	if (hdev->notify)
		hdev->notify(hdev, HCI_NOTIFY_VOICE_SETTING);
}

static void hci_cc_write_voice_setting(struct hci_dev *hdev,
				       struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	__u8 status = *((__u8 *) skb->data);
	__u16 setting;
	void *sent;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	if (status)
		return;

	sent = hci_sent_cmd_data(hdev, HCI_OP_WRITE_VOICE_SETTING);
	if (!sent)
		return;

	setting = get_unaligned_le16(sent);

	if (hdev->voice_setting == setting)
		return;

	hdev->voice_setting = setting;

<<<<<<< HEAD
	BT_DBG("%s voice setting 0x%04x", hdev->name, setting);

	if (hdev->notify) {
		tasklet_disable(&hdev->tx_task);
		hdev->notify(hdev, HCI_NOTIFY_VOICE_SETTING);
		tasklet_enable(&hdev->tx_task);
	}
}

static void hci_cc_host_buffer_size(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%x", hdev->name, status);

	hci_req_complete(hdev, HCI_OP_HOST_BUFFER_SIZE, status);
}

<<<<<<< HEAD
static void hci_cc_le_clear_white_list(struct hci_dev *hdev,
							struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%x", hdev->name, status);

	hci_req_complete(hdev, HCI_OP_LE_CLEAR_WHITE_LIST, status);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static void hci_cc_read_ssp_mode(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_read_ssp_mode *rp = (void *) skb->data;

	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
	BT_DBG("%s voice setting 0x%4.4x", hdev->name, setting);

	if (hdev->notify)
		hdev->notify(hdev, HCI_NOTIFY_VOICE_SETTING);
}

static void hci_cc_read_num_supported_iac(struct hci_dev *hdev,
					  struct sk_buff *skb)
{
	struct hci_rp_read_num_supported_iac *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

<<<<<<< HEAD
	hdev->ssp_mode = rp->mode;
=======
	hdev->num_iac = rp->num_iac;

	BT_DBG("%s num iac %d", hdev->name, hdev->num_iac);
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_write_ssp_mode(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);
<<<<<<< HEAD
	void *sent;

	BT_DBG("%s status 0x%x", hdev->name, status);

	if (status)
		return;
=======
	struct hci_cp_write_ssp_mode *sent;

	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	sent = hci_sent_cmd_data(hdev, HCI_OP_WRITE_SSP_MODE);
	if (!sent)
		return;

<<<<<<< HEAD
	hdev->ssp_mode = *((__u8 *) sent);
}

static u8 hci_get_inquiry_mode(struct hci_dev *hdev)
{
	if (hdev->features[6] & LMP_EXT_INQ)
		return 2;

	if (hdev->features[3] & LMP_RSSI_INQ)
		return 1;

	if (hdev->manufacturer == 11 && hdev->hci_rev == 0x00 &&
						hdev->lmp_subver == 0x0757)
		return 1;

	if (hdev->manufacturer == 15) {
		if (hdev->hci_rev == 0x03 && hdev->lmp_subver == 0x6963)
			return 1;
		if (hdev->hci_rev == 0x09 && hdev->lmp_subver == 0x6963)
			return 1;
		if (hdev->hci_rev == 0x00 && hdev->lmp_subver == 0x6965)
			return 1;
	}

	if (hdev->manufacturer == 31 && hdev->hci_rev == 0x2005 &&
						hdev->lmp_subver == 0x1805)
		return 1;

	return 0;
}

static void hci_setup_inquiry_mode(struct hci_dev *hdev)
{
	u8 mode;

	mode = hci_get_inquiry_mode(hdev);

	hci_send_cmd(hdev, HCI_OP_WRITE_INQUIRY_MODE, 1, &mode);
}

static void hci_setup_event_mask(struct hci_dev *hdev)
{
	/* The second byte is 0xff instead of 0x9f (two reserved bits
	 * disabled) since a Broadcom 1.2 dongle doesn't respond to the
	 * command otherwise */
	u8 events[8] = { 0xff, 0xff, 0xfb, 0xff, 0x00, 0x00, 0x00, 0x00 };

	BT_DBG("");

	/* Events for 1.2 and newer controllers */
	if (hdev->lmp_ver > 1) {
		events[4] |= 0x01; /* Flow Specification Complete */
		events[4] |= 0x02; /* Inquiry Result with RSSI */
		events[4] |= 0x04; /* Read Remote Extended Features Complete */
		events[5] |= 0x08; /* Synchronous Connection Complete */
		events[5] |= 0x10; /* Synchronous Connection Changed */
	}

	if (hdev->features[3] & LMP_RSSI_INQ)
		events[4] |= 0x04; /* Inquiry Result with RSSI */

	if (hdev->features[5] & LMP_SNIFF_SUBR)
		events[5] |= 0x20; /* Sniff Subrating */

	if (hdev->features[5] & LMP_PAUSE_ENC)
		events[5] |= 0x80; /* Encryption Key Refresh Complete */

	if (hdev->features[6] & LMP_EXT_INQ)
		events[5] |= 0x40; /* Extended Inquiry Result */

	if (hdev->features[6] & LMP_NO_FLUSH)
		events[7] |= 0x01; /* Enhanced Flush Complete */

	if (hdev->features[7] & LMP_LSTO)
		events[6] |= 0x80; /* Link Supervision Timeout Changed */

	if (hdev->features[6] & LMP_SIMPLE_PAIR) {
		events[6] |= 0x01;	/* IO Capability Request */
		events[6] |= 0x02;	/* IO Capability Response */
		events[6] |= 0x04;	/* User Confirmation Request */
		events[6] |= 0x08;	/* User Passkey Request */
		events[6] |= 0x10;	/* Remote OOB Data Request */
		events[6] |= 0x20;	/* Simple Pairing Complete */
		events[7] |= 0x04;	/* User Passkey Notification */
		events[7] |= 0x08;	/* Keypress Notification */
		events[7] |= 0x10;	/* Remote Host Supported
					 * Features Notification */
	}

	if (hdev->features[4] & LMP_LE)
		events[7] |= 0x20;	/* LE Meta-Event */

	hci_send_cmd(hdev, HCI_OP_SET_EVENT_MASK, sizeof(events), events);
}

static void hci_setup(struct hci_dev *hdev)
{
	if (hdev->lmp_ver > 1)
		hci_send_cmd(hdev, HCI_OP_READ_LOCAL_COMMANDS, 0, NULL);

	if (hdev->features[6] & LMP_SIMPLE_PAIR) {
		u8 mode = 0x01;
		hci_send_cmd(hdev, HCI_OP_WRITE_SSP_MODE, sizeof(mode), &mode);
	}

	if (hdev->features[3] & LMP_RSSI_INQ)
		hci_setup_inquiry_mode(hdev);

	if (hdev->features[7] & LMP_INQ_TX_PWR)
		hci_send_cmd(hdev, HCI_OP_READ_INQ_RSP_TX_POWER, 0, NULL);
=======
	if (!status) {
		if (sent->mode)
			hdev->features[1][0] |= LMP_HOST_SSP;
		else
			hdev->features[1][0] &= ~LMP_HOST_SSP;
	}

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		mgmt_ssp_enable_complete(hdev, sent->mode, status);
	else if (!status) {
		if (sent->mode)
			set_bit(HCI_SSP_ENABLED, &hdev->dev_flags);
		else
			clear_bit(HCI_SSP_ENABLED, &hdev->dev_flags);
	}
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_read_local_version(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_read_local_version *rp = (void *) skb->data;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

<<<<<<< HEAD
	hdev->hci_ver = rp->hci_ver;
	hdev->hci_rev = __le16_to_cpu(rp->hci_rev);
	hdev->lmp_ver = rp->lmp_ver;
	hdev->manufacturer = __le16_to_cpu(rp->manufacturer);
	hdev->lmp_subver = __le16_to_cpu(rp->lmp_subver);

	BT_DBG("%s manufacturer %d hci ver %d:%d", hdev->name,
					hdev->manufacturer,
					hdev->hci_ver, hdev->hci_rev);

	if (hdev->dev_type == HCI_BREDR && test_bit(HCI_INIT, &hdev->flags))
		hci_setup(hdev);
}

static void hci_setup_link_policy(struct hci_dev *hdev)
{
	u16 link_policy = 0;

	if (hdev->features[0] & LMP_RSWITCH)
		link_policy |= HCI_LP_RSWITCH;
	if (hdev->features[0] & LMP_HOLD)
		link_policy |= HCI_LP_HOLD;
	if (hdev->features[0] & LMP_SNIFF)
		link_policy |= HCI_LP_SNIFF;
	if (hdev->features[1] & LMP_PARK)
		link_policy |= HCI_LP_PARK;

	link_policy = cpu_to_le16(link_policy);
	hci_send_cmd(hdev, HCI_OP_WRITE_DEF_LINK_POLICY,
					sizeof(link_policy), &link_policy);
}

static void hci_cc_read_local_commands(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_read_local_commands *rp = (void *) skb->data;

	BT_DBG("%s status 0x%x", hdev->name, rp->status);

	if (rp->status)
		goto done;

	memcpy(hdev->commands, rp->commands, sizeof(hdev->commands));

	if (test_bit(HCI_INIT, &hdev->flags) && (hdev->commands[5] & 0x10))
		hci_setup_link_policy(hdev);

done:
	hci_req_complete(hdev, HCI_OP_READ_LOCAL_COMMANDS, rp->status);
}

static void hci_cc_read_local_features(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_read_local_features *rp = (void *) skb->data;

	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
	if (test_bit(HCI_SETUP, &hdev->dev_flags)) {
		hdev->hci_ver = rp->hci_ver;
		hdev->hci_rev = __le16_to_cpu(rp->hci_rev);
		hdev->lmp_ver = rp->lmp_ver;
		hdev->manufacturer = __le16_to_cpu(rp->manufacturer);
		hdev->lmp_subver = __le16_to_cpu(rp->lmp_subver);
	}
}

static void hci_cc_read_local_commands(struct hci_dev *hdev,
				       struct sk_buff *skb)
{
	struct hci_rp_read_local_commands *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	if (!rp->status)
		memcpy(hdev->commands, rp->commands, sizeof(hdev->commands));
}

static void hci_cc_read_local_features(struct hci_dev *hdev,
				       struct sk_buff *skb)
{
	struct hci_rp_read_local_features *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

	memcpy(hdev->features, rp->features, 8);

<<<<<<< HEAD
	if (hdev->dev_type == HCI_BREDR && test_bit(HCI_INIT, &hdev->flags)) {
		if (hdev->features[6] & LMP_SIMPLE_PAIR) {
			u8 mode = 0x01;
			hci_send_cmd(hdev, HCI_OP_WRITE_SSP_MODE,
					sizeof(mode), &mode);
		}

		if (hdev->features[3] & LMP_RSSI_INQ)
			hci_setup_inquiry_mode(hdev);

		if (hdev->features[7] & LMP_INQ_TX_PWR)
			hci_send_cmd(hdev, HCI_OP_READ_INQ_RSP_TX_POWER,
								0, NULL);

		hci_setup_event_mask(hdev);
	}

	/* Adjust default settings according to features
	 * supported by device. */

	if (hdev->features[0] & LMP_3SLOT)
		hdev->pkt_type |= (HCI_DM3 | HCI_DH3);

	if (hdev->features[0] & LMP_5SLOT)
		hdev->pkt_type |= (HCI_DM5 | HCI_DH5);

	if (hdev->features[1] & LMP_HV2) {
=======
	/* Adjust default settings according to features
	 * supported by device. */

	if (hdev->features[0][0] & LMP_3SLOT)
		hdev->pkt_type |= (HCI_DM3 | HCI_DH3);

	if (hdev->features[0][0] & LMP_5SLOT)
		hdev->pkt_type |= (HCI_DM5 | HCI_DH5);

	if (hdev->features[0][1] & LMP_HV2) {
>>>>>>> refs/remotes/origin/master
		hdev->pkt_type  |= (HCI_HV2);
		hdev->esco_type |= (ESCO_HV2);
	}

<<<<<<< HEAD
	if (hdev->features[1] & LMP_HV3) {
=======
	if (hdev->features[0][1] & LMP_HV3) {
>>>>>>> refs/remotes/origin/master
		hdev->pkt_type  |= (HCI_HV3);
		hdev->esco_type |= (ESCO_HV3);
	}

<<<<<<< HEAD
	if (hdev->features[3] & LMP_ESCO)
		hdev->esco_type |= (ESCO_EV3);

	if (hdev->features[4] & LMP_EV4)
		hdev->esco_type |= (ESCO_EV4);

	if (hdev->features[4] & LMP_EV5)
		hdev->esco_type |= (ESCO_EV5);

	if (hdev->features[5] & LMP_EDR_ESCO_2M)
		hdev->esco_type |= (ESCO_2EV3);

	if (hdev->features[5] & LMP_EDR_ESCO_3M)
		hdev->esco_type |= (ESCO_3EV3);

	if (hdev->features[5] & LMP_EDR_3S_ESCO)
		hdev->esco_type |= (ESCO_2EV5 | ESCO_3EV5);

	BT_DBG("%s features 0x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x", hdev->name,
					hdev->features[0], hdev->features[1],
					hdev->features[2], hdev->features[3],
					hdev->features[4], hdev->features[5],
					hdev->features[6], hdev->features[7]);
}

static void hci_cc_read_flow_control_mode(struct hci_dev *hdev,
					struct sk_buff *skb)
{
	struct hci_rp_read_flow_control_mode *rp = (void *) skb->data;

	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
	if (lmp_esco_capable(hdev))
		hdev->esco_type |= (ESCO_EV3);

	if (hdev->features[0][4] & LMP_EV4)
		hdev->esco_type |= (ESCO_EV4);

	if (hdev->features[0][4] & LMP_EV5)
		hdev->esco_type |= (ESCO_EV5);

	if (hdev->features[0][5] & LMP_EDR_ESCO_2M)
		hdev->esco_type |= (ESCO_2EV3);

	if (hdev->features[0][5] & LMP_EDR_ESCO_3M)
		hdev->esco_type |= (ESCO_3EV3);

	if (hdev->features[0][5] & LMP_EDR_3S_ESCO)
		hdev->esco_type |= (ESCO_2EV5 | ESCO_3EV5);

	BT_DBG("%s features 0x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x", hdev->name,
	       hdev->features[0][0], hdev->features[0][1],
	       hdev->features[0][2], hdev->features[0][3],
	       hdev->features[0][4], hdev->features[0][5],
	       hdev->features[0][6], hdev->features[0][7]);
}

static void hci_cc_read_local_ext_features(struct hci_dev *hdev,
					   struct sk_buff *skb)
{
	struct hci_rp_read_local_ext_features *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

<<<<<<< HEAD
	hdev->flow_ctl_mode = rp->mode;
=======
	if (hdev->max_page < rp->max_page)
		hdev->max_page = rp->max_page;

	if (rp->page < HCI_MAX_PAGES)
		memcpy(hdev->features[rp->page], rp->features, 8);
}

static void hci_cc_read_flow_control_mode(struct hci_dev *hdev,
					  struct sk_buff *skb)
{
	struct hci_rp_read_flow_control_mode *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	if (!rp->status)
		hdev->flow_ctl_mode = rp->mode;
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_read_buffer_size(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_read_buffer_size *rp = (void *) skb->data;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

<<<<<<< HEAD
	if (hdev->flow_ctl_mode == HCI_PACKET_BASED_FLOW_CTL_MODE) {
		hdev->acl_mtu  = __le16_to_cpu(rp->acl_mtu);
		hdev->sco_mtu  = rp->sco_mtu;
		hdev->acl_pkts = __le16_to_cpu(rp->acl_max_pkt);
		hdev->sco_pkts = __le16_to_cpu(rp->sco_max_pkt);
		hdev->acl_cnt = hdev->acl_pkts;
		hdev->sco_cnt = hdev->sco_pkts;
	}
=======
	hdev->acl_mtu  = __le16_to_cpu(rp->acl_mtu);
	hdev->sco_mtu  = rp->sco_mtu;
	hdev->acl_pkts = __le16_to_cpu(rp->acl_max_pkt);
	hdev->sco_pkts = __le16_to_cpu(rp->sco_max_pkt);
>>>>>>> refs/remotes/origin/master

	if (test_bit(HCI_QUIRK_FIXUP_BUFFER_SIZE, &hdev->quirks)) {
		hdev->sco_mtu  = 64;
		hdev->sco_pkts = 8;
	}

<<<<<<< HEAD

	BT_DBG("%s acl mtu %d:%d sco mtu %d:%d", hdev->name,
					hdev->acl_mtu, hdev->acl_pkts,
					hdev->sco_mtu, hdev->sco_pkts);
=======
	hdev->acl_cnt = hdev->acl_pkts;
	hdev->sco_cnt = hdev->sco_pkts;

	BT_DBG("%s acl mtu %d:%d sco mtu %d:%d", hdev->name, hdev->acl_mtu,
	       hdev->acl_pkts, hdev->sco_mtu, hdev->sco_pkts);
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_read_bd_addr(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_read_bd_addr *rp = (void *) skb->data;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);

	if (!rp->status)
		bacpy(&hdev->bdaddr, &rp->bdaddr);

	hci_req_complete(hdev, HCI_OP_READ_BD_ADDR, rp->status);
}

static void hci_cc_write_ca_timeout(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%x", hdev->name, status);

	hci_req_complete(hdev, HCI_OP_WRITE_CA_TIMEOUT, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	if (!rp->status)
		bacpy(&hdev->bdaddr, &rp->bdaddr);
}

static void hci_cc_read_page_scan_activity(struct hci_dev *hdev,
					   struct sk_buff *skb)
{
	struct hci_rp_read_page_scan_activity *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	if (test_bit(HCI_INIT, &hdev->flags) && !rp->status) {
		hdev->page_scan_interval = __le16_to_cpu(rp->interval);
		hdev->page_scan_window = __le16_to_cpu(rp->window);
	}
}

static void hci_cc_write_page_scan_activity(struct hci_dev *hdev,
					    struct sk_buff *skb)
{
	u8 status = *((u8 *) skb->data);
	struct hci_cp_write_page_scan_activity *sent;

	BT_DBG("%s status 0x%2.2x", hdev->name, status);

	if (status)
		return;

	sent = hci_sent_cmd_data(hdev, HCI_OP_WRITE_PAGE_SCAN_ACTIVITY);
	if (!sent)
		return;

	hdev->page_scan_interval = __le16_to_cpu(sent->interval);
	hdev->page_scan_window = __le16_to_cpu(sent->window);
}

static void hci_cc_read_page_scan_type(struct hci_dev *hdev,
					   struct sk_buff *skb)
{
	struct hci_rp_read_page_scan_type *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	if (test_bit(HCI_INIT, &hdev->flags) && !rp->status)
		hdev->page_scan_type = rp->type;
}

static void hci_cc_write_page_scan_type(struct hci_dev *hdev,
					struct sk_buff *skb)
{
	u8 status = *((u8 *) skb->data);
	u8 *type;

	BT_DBG("%s status 0x%2.2x", hdev->name, status);

	if (status)
		return;

	type = hci_sent_cmd_data(hdev, HCI_OP_WRITE_PAGE_SCAN_TYPE);
	if (type)
		hdev->page_scan_type = *type;
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_read_data_block_size(struct hci_dev *hdev,
					struct sk_buff *skb)
{
	struct hci_rp_read_data_block_size *rp = (void *) skb->data;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

<<<<<<< HEAD
	if (hdev->flow_ctl_mode == HCI_BLOCK_BASED_FLOW_CTL_MODE) {
		hdev->acl_mtu  = __le16_to_cpu(rp->max_acl_len);
		hdev->sco_mtu = 0;
		hdev->data_block_len = __le16_to_cpu(rp->data_block_len);
		/* acl_pkts indicates the number of blocks */
		hdev->acl_pkts = __le16_to_cpu(rp->num_blocks);
		hdev->sco_pkts = 0;
		hdev->acl_cnt = hdev->acl_pkts;
		hdev->sco_cnt = 0;
	}

	BT_DBG("%s acl mtu %d:%d, data block len %d", hdev->name,
			hdev->acl_mtu, hdev->acl_cnt, hdev->data_block_len);
}

static void hci_cc_read_local_amp_info(struct hci_dev *hdev,
				struct sk_buff *skb)
{
	struct hci_rp_read_local_amp_info *rp = (void *) skb->data;

	BT_DBG("%s status 0x%x", hdev->name, rp->status);

	if (rp->status)
		return;
=======
	hdev->block_mtu = __le16_to_cpu(rp->max_acl_len);
	hdev->block_len = __le16_to_cpu(rp->block_len);
	hdev->num_blocks = __le16_to_cpu(rp->num_blocks);

	hdev->block_cnt = hdev->num_blocks;

	BT_DBG("%s blk mtu %d cnt %d len %d", hdev->name, hdev->block_mtu,
	       hdev->block_cnt, hdev->block_len);
}

static void hci_cc_read_local_amp_info(struct hci_dev *hdev,
				       struct sk_buff *skb)
{
	struct hci_rp_read_local_amp_info *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	if (rp->status)
		goto a2mp_rsp;
>>>>>>> refs/remotes/origin/master

	hdev->amp_status = rp->amp_status;
	hdev->amp_total_bw = __le32_to_cpu(rp->total_bw);
	hdev->amp_max_bw = __le32_to_cpu(rp->max_bw);
	hdev->amp_min_latency = __le32_to_cpu(rp->min_latency);
	hdev->amp_max_pdu = __le32_to_cpu(rp->max_pdu);
	hdev->amp_type = rp->amp_type;
	hdev->amp_pal_cap = __le16_to_cpu(rp->pal_cap);
	hdev->amp_assoc_size = __le16_to_cpu(rp->max_assoc_size);
	hdev->amp_be_flush_to = __le32_to_cpu(rp->be_flush_to);
	hdev->amp_max_flush_to = __le32_to_cpu(rp->max_flush_to);

<<<<<<< HEAD
	hci_req_complete(hdev, HCI_OP_READ_LOCAL_AMP_INFO, rp->status);
}

static void hci_cc_delete_stored_link_key(struct hci_dev *hdev,
							struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%x", hdev->name, status);

	hci_req_complete(hdev, HCI_OP_DELETE_STORED_LINK_KEY, status);
}

static void hci_cc_set_event_mask(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%x", hdev->name, status);

	hci_req_complete(hdev, HCI_OP_SET_EVENT_MASK, status);
}

static void hci_cc_write_inquiry_mode(struct hci_dev *hdev,
							struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%x", hdev->name, status);

	hci_req_complete(hdev, HCI_OP_WRITE_INQUIRY_MODE, status);
}

static void hci_cc_read_inq_rsp_tx_power(struct hci_dev *hdev,
							struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%x", hdev->name, status);

	hci_req_complete(hdev, HCI_OP_READ_INQ_RSP_TX_POWER, status);
}

static void hci_cc_set_event_flt(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%x", hdev->name, status);

	hci_req_complete(hdev, HCI_OP_SET_EVENT_FLT, status);
=======
a2mp_rsp:
	a2mp_send_getinfo_rsp(hdev);
}

static void hci_cc_read_local_amp_assoc(struct hci_dev *hdev,
					struct sk_buff *skb)
{
	struct hci_rp_read_local_amp_assoc *rp = (void *) skb->data;
	struct amp_assoc *assoc = &hdev->loc_assoc;
	size_t rem_len, frag_len;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	if (rp->status)
		goto a2mp_rsp;

	frag_len = skb->len - sizeof(*rp);
	rem_len = __le16_to_cpu(rp->rem_len);

	if (rem_len > frag_len) {
		BT_DBG("frag_len %zu rem_len %zu", frag_len, rem_len);

		memcpy(assoc->data + assoc->offset, rp->frag, frag_len);
		assoc->offset += frag_len;

		/* Read other fragments */
		amp_read_loc_assoc_frag(hdev, rp->phy_handle);

		return;
	}

	memcpy(assoc->data + assoc->offset, rp->frag, rem_len);
	assoc->len = assoc->offset + rem_len;
	assoc->offset = 0;

a2mp_rsp:
	/* Send A2MP Rsp when all fragments are received */
	a2mp_send_getampassoc_rsp(hdev, rp->status);
	a2mp_send_create_phy_link_req(hdev, rp->status);
}

static void hci_cc_read_inq_rsp_tx_power(struct hci_dev *hdev,
					 struct sk_buff *skb)
{
	struct hci_rp_read_inq_rsp_tx_power *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	if (!rp->status)
		hdev->inq_tx_power = rp->tx_power;
>>>>>>> refs/remotes/origin/master
}

static void hci_cc_pin_code_reply(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_pin_code_reply *rp = (void *) skb->data;
	struct hci_cp_pin_code_reply *cp;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);
	hci_dev_lock(hdev);

	if (test_bit(HCI_MGMT, &hdev->flags))
		mgmt_pin_code_reply_complete(hdev->id, &rp->bdaddr, rp->status);

	if (rp->status != 0)
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	hci_dev_lock(hdev);

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		mgmt_pin_code_reply_complete(hdev, &rp->bdaddr, rp->status);

	if (rp->status)
>>>>>>> refs/remotes/origin/master
		goto unlock;

	cp = hci_sent_cmd_data(hdev, HCI_OP_PIN_CODE_REPLY);
	if (!cp)
		goto unlock;

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &cp->bdaddr);
	if (conn)
		conn->pin_length = cp->pin_len;
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
unlock:
	hci_dev_unlock(hdev);
}

static void hci_cc_pin_code_neg_reply(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_pin_code_neg_reply *rp = (void *) skb->data;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);
	hci_dev_lock(hdev);

	if (test_bit(HCI_MGMT, &hdev->flags))
		mgmt_pin_code_neg_reply_complete(hdev->id, &rp->bdaddr,
								rp->status);
	hci_dev_unlock(hdev);
}
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	hci_dev_lock(hdev);

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		mgmt_pin_code_neg_reply_complete(hdev, &rp->bdaddr,
						 rp->status);

	hci_dev_unlock(hdev);
}

>>>>>>> refs/remotes/origin/master
static void hci_cc_le_read_buffer_size(struct hci_dev *hdev,
				       struct sk_buff *skb)
{
	struct hci_rp_le_read_buffer_size *rp = (void *) skb->data;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);
>>>>>>> refs/remotes/origin/master

	if (rp->status)
		return;

	hdev->le_mtu = __le16_to_cpu(rp->le_mtu);
	hdev->le_pkts = rp->le_max_pkt;

	hdev->le_cnt = hdev->le_pkts;

	BT_DBG("%s le mtu %d:%d", hdev->name, hdev->le_mtu, hdev->le_pkts);
<<<<<<< HEAD

	hci_req_complete(hdev, HCI_OP_LE_READ_BUFFER_SIZE, rp->status);
}

<<<<<<< HEAD
static void hci_cc_le_read_white_list_size(struct hci_dev *hdev,
				       struct sk_buff *skb)
{
	struct hci_rp_le_read_white_list_size *rp = (void *) skb->data;

	BT_DBG("%s status 0x%x", hdev->name, rp->status);

	if (rp->status)
		return;

	hdev->le_white_list_size = rp->size;

	BT_DBG("%s le white list %d", hdev->name, hdev->le_white_list_size);

	hci_req_complete(hdev, HCI_OP_LE_READ_WHITE_LIST_SIZE, rp->status);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
}

static void hci_cc_le_read_local_features(struct hci_dev *hdev,
					  struct sk_buff *skb)
{
	struct hci_rp_le_read_local_features *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	if (!rp->status)
		memcpy(hdev->le_features, rp->features, 8);
}

static void hci_cc_le_read_adv_tx_power(struct hci_dev *hdev,
					struct sk_buff *skb)
{
	struct hci_rp_le_read_adv_tx_power *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	if (!rp->status)
		hdev->adv_tx_power = rp->tx_power;
}

>>>>>>> refs/remotes/origin/master
static void hci_cc_user_confirm_reply(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_user_confirm_reply *rp = (void *) skb->data;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, rp->status);
	hci_dev_lock(hdev);

	if (test_bit(HCI_MGMT, &hdev->flags))
		mgmt_user_confirm_reply_complete(hdev->id, &rp->bdaddr,
								rp->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	hci_dev_lock(hdev);

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		mgmt_user_confirm_reply_complete(hdev, &rp->bdaddr, ACL_LINK, 0,
						 rp->status);

>>>>>>> refs/remotes/origin/master
	hci_dev_unlock(hdev);
}

static void hci_cc_user_confirm_neg_reply(struct hci_dev *hdev,
<<<<<<< HEAD
							struct sk_buff *skb)
{
	struct hci_rp_user_confirm_reply *rp = (void *) skb->data;

	BT_DBG("%s status 0x%x", hdev->name, rp->status);
	hci_dev_lock(hdev);

	if (test_bit(HCI_MGMT, &hdev->flags))
		mgmt_user_confirm_neg_reply_complete(hdev->id, &rp->bdaddr,
								rp->status);
	hci_dev_unlock(hdev);
}

static void hci_cc_read_rssi(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_conn *conn;
	struct hci_rp_read_rssi *rp = (void *) skb->data;

	BT_DBG("%s status 0x%x", hdev->name, rp->status);

	BT_DBG("%s rssi : %d handle : %d", hdev->name, rp->rssi, rp->handle);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(rp->handle));
	if (conn)
		mgmt_read_rssi_complete(hdev->id, rp->rssi, &conn->dst,
			__le16_to_cpu(rp->handle), rp->status);
}

static void hci_cc_read_local_oob_data_reply(struct hci_dev *hdev,
							struct sk_buff *skb)
{
	struct hci_rp_read_local_oob_data *rp = (void *) skb->data;

	BT_DBG("%s status 0x%x", hdev->name, rp->status);
	hci_dev_lock(hdev);

	mgmt_read_local_oob_data_reply_complete(hdev->id, rp->hash,
						rp->randomizer, rp->status);
	hci_dev_unlock(hdev);
}

static void hci_cc_le_ltk_reply(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_le_ltk_reply *rp = (void *) skb->data;

	BT_DBG("%s status 0x%x", hdev->name, rp->status);

	if (rp->status)
		return;

	hci_req_complete(hdev, HCI_OP_LE_LTK_REPLY, rp->status);
}

static void hci_cc_le_ltk_neg_reply(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_le_ltk_neg_reply *rp = (void *) skb->data;

	BT_DBG("%s status 0x%x", hdev->name, rp->status);

	if (rp->status)
		return;

	hci_req_complete(hdev, HCI_OP_LE_LTK_NEG_REPLY, rp->status);
}

static void hci_cc_le_set_scan_enable(struct hci_dev *hdev,
					struct sk_buff *skb)
{
	void *sent;
	__u8 param_scan_enable;
	__u8 status = *((__u8 *) skb->data);

	if (status)
		return;

	sent = hci_sent_cmd_data(hdev, HCI_OP_LE_SET_SCAN_ENABLE);
	if (!sent)
		return;

	param_scan_enable = *((__u8 *) sent);
	if (param_scan_enable == 0x01) {
		del_timer(&hdev->adv_timer);
	} else if (param_scan_enable == 0x00) {
		mod_timer(&hdev->adv_timer, jiffies + ADV_CLEAR_TIMEOUT);
	}
}

static inline void hci_cs_inquiry(struct hci_dev *hdev, __u8 status)
{
	BT_DBG("%s status 0x%x", hdev->name, status);

	if (status) {
		hci_req_complete(hdev, HCI_OP_INQUIRY, status);

		hci_conn_check_pending(hdev);
	} else {
		set_bit(HCI_INQUIRY, &hdev->flags);
		hci_dev_lock(hdev);
		if (test_bit(HCI_MGMT, &hdev->flags))
			mgmt_inquiry_started(hdev->id);
		hci_dev_unlock(hdev);
	}
}

static inline void hci_cs_create_conn(struct hci_dev *hdev, __u8 status)
=======
					  struct sk_buff *skb)
{
	struct hci_rp_user_confirm_reply *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	hci_dev_lock(hdev);

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		mgmt_user_confirm_neg_reply_complete(hdev, &rp->bdaddr,
						     ACL_LINK, 0, rp->status);

	hci_dev_unlock(hdev);
}

static void hci_cc_user_passkey_reply(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_rp_user_confirm_reply *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	hci_dev_lock(hdev);

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		mgmt_user_passkey_reply_complete(hdev, &rp->bdaddr, ACL_LINK,
						 0, rp->status);

	hci_dev_unlock(hdev);
}

static void hci_cc_user_passkey_neg_reply(struct hci_dev *hdev,
					  struct sk_buff *skb)
{
	struct hci_rp_user_confirm_reply *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	hci_dev_lock(hdev);

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		mgmt_user_passkey_neg_reply_complete(hdev, &rp->bdaddr,
						     ACL_LINK, 0, rp->status);

	hci_dev_unlock(hdev);
}

static void hci_cc_read_local_oob_data_reply(struct hci_dev *hdev,
					     struct sk_buff *skb)
{
	struct hci_rp_read_local_oob_data *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	hci_dev_lock(hdev);
	mgmt_read_local_oob_data_reply_complete(hdev, rp->hash,
						rp->randomizer, rp->status);
	hci_dev_unlock(hdev);
}

static void hci_cc_le_set_adv_enable(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 *sent, status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%2.2x", hdev->name, status);

	sent = hci_sent_cmd_data(hdev, HCI_OP_LE_SET_ADV_ENABLE);
	if (!sent)
		return;

	hci_dev_lock(hdev);

	if (!status) {
		if (*sent)
			set_bit(HCI_ADVERTISING, &hdev->dev_flags);
		else
			clear_bit(HCI_ADVERTISING, &hdev->dev_flags);
	}

	hci_dev_unlock(hdev);
}

static void hci_cc_le_set_scan_enable(struct hci_dev *hdev,
				      struct sk_buff *skb)
{
	struct hci_cp_le_set_scan_enable *cp;
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%2.2x", hdev->name, status);

	cp = hci_sent_cmd_data(hdev, HCI_OP_LE_SET_SCAN_ENABLE);
	if (!cp)
		return;

	if (status)
		return;

	switch (cp->enable) {
	case LE_SCAN_ENABLE:
		set_bit(HCI_LE_SCAN, &hdev->dev_flags);
		break;

	case LE_SCAN_DISABLE:
		clear_bit(HCI_LE_SCAN, &hdev->dev_flags);
		break;

	default:
		BT_ERR("Used reserved LE_Scan_Enable param %d", cp->enable);
		break;
	}
}

static void hci_cc_le_read_white_list_size(struct hci_dev *hdev,
					   struct sk_buff *skb)
{
	struct hci_rp_le_read_white_list_size *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x size %u", hdev->name, rp->status, rp->size);

	if (!rp->status)
		hdev->le_white_list_size = rp->size;
}

static void hci_cc_le_read_supported_states(struct hci_dev *hdev,
					    struct sk_buff *skb)
{
	struct hci_rp_le_read_supported_states *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x", hdev->name, rp->status);

	if (!rp->status)
		memcpy(hdev->le_states, rp->le_states, 8);
}

static void hci_cc_write_le_host_supported(struct hci_dev *hdev,
					   struct sk_buff *skb)
{
	struct hci_cp_write_le_host_supported *sent;
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status 0x%2.2x", hdev->name, status);

	sent = hci_sent_cmd_data(hdev, HCI_OP_WRITE_LE_HOST_SUPPORTED);
	if (!sent)
		return;

	if (!status) {
		if (sent->le) {
			hdev->features[1][0] |= LMP_HOST_LE;
			set_bit(HCI_LE_ENABLED, &hdev->dev_flags);
		} else {
			hdev->features[1][0] &= ~LMP_HOST_LE;
			clear_bit(HCI_LE_ENABLED, &hdev->dev_flags);
			clear_bit(HCI_ADVERTISING, &hdev->dev_flags);
		}

		if (sent->simul)
			hdev->features[1][0] |= LMP_HOST_LE_BREDR;
		else
			hdev->features[1][0] &= ~LMP_HOST_LE_BREDR;
	}
}

static void hci_cc_write_remote_amp_assoc(struct hci_dev *hdev,
					  struct sk_buff *skb)
{
	struct hci_rp_write_remote_amp_assoc *rp = (void *) skb->data;

	BT_DBG("%s status 0x%2.2x phy_handle 0x%2.2x",
	       hdev->name, rp->status, rp->phy_handle);

	if (rp->status)
		return;

	amp_write_rem_assoc_continue(hdev, rp->phy_handle);
}

static void hci_cs_inquiry(struct hci_dev *hdev, __u8 status)
{
	BT_DBG("%s status 0x%2.2x", hdev->name, status);

	if (status) {
		hci_conn_check_pending(hdev);
		return;
	}

	set_bit(HCI_INQUIRY, &hdev->flags);
}

static void hci_cs_create_conn(struct hci_dev *hdev, __u8 status)
>>>>>>> refs/remotes/origin/master
{
	struct hci_cp_create_conn *cp;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	cp = hci_sent_cmd_data(hdev, HCI_OP_CREATE_CONN);
	if (!cp)
		return;

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &cp->bdaddr);

<<<<<<< HEAD
	BT_DBG("%s bdaddr %s conn %p", hdev->name, batostr(&cp->bdaddr), conn);
=======
	BT_DBG("%s bdaddr %pMR hcon %p", hdev->name, &cp->bdaddr, conn);
>>>>>>> refs/remotes/origin/master

	if (status) {
		if (conn && conn->state == BT_CONNECT) {
			if (status != 0x0c || conn->attempt > 2) {
				conn->state = BT_CLOSED;
				hci_proto_connect_cfm(conn, status);
				hci_conn_del(conn);
			} else
				conn->state = BT_CONNECT2;
		}
	} else {
		if (!conn) {
<<<<<<< HEAD
			conn = hci_conn_add(hdev, ACL_LINK, 0, &cp->bdaddr);
			if (conn) {
				conn->out = 1;
=======
			conn = hci_conn_add(hdev, ACL_LINK, &cp->bdaddr);
			if (conn) {
				conn->out = true;
>>>>>>> refs/remotes/origin/master
				conn->link_mode |= HCI_LM_MASTER;
			} else
				BT_ERR("No memory for new connection");
		}
	}

	hci_dev_unlock(hdev);
}

static void hci_cs_add_sco(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_add_sco *cp;
	struct hci_conn *acl, *sco;
	__u16 handle;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	if (!status)
		return;

	cp = hci_sent_cmd_data(hdev, HCI_OP_ADD_SCO);
	if (!cp)
		return;

	handle = __le16_to_cpu(cp->handle);

<<<<<<< HEAD
	BT_DBG("%s handle %d", hdev->name, handle);
=======
	BT_DBG("%s handle 0x%4.4x", hdev->name, handle);
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	acl = hci_conn_hash_lookup_handle(hdev, handle);
	if (acl) {
		sco = acl->link;
		if (sco) {
			sco->state = BT_CLOSED;

			hci_proto_connect_cfm(sco, status);
			hci_conn_del(sco);
		}
	}

	hci_dev_unlock(hdev);
}

static void hci_cs_auth_requested(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_auth_requested *cp;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);

	if (!status)
		return;
>>>>>>> refs/remotes/origin/master

	cp = hci_sent_cmd_data(hdev, HCI_OP_AUTH_REQUESTED);
	if (!cp)
		return;

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(cp->handle));
	if (conn) {
<<<<<<< HEAD
		if (status) {
			mgmt_auth_failed(hdev->id, &conn->dst, status);
			clear_bit(HCI_CONN_AUTH_PEND, &conn->pend);

			if (conn->state == BT_CONFIG) {
				conn->state = BT_CONNECTED;
				hci_proto_connect_cfm(conn, status);
				hci_conn_put(conn);
			} else {
				hci_auth_cfm(conn, status);
				hci_conn_hold(conn);
				conn->disc_timeout = HCI_DISCONN_TIMEOUT;
				hci_conn_put(conn);
			}

			if (test_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend)) {
				clear_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend);
				hci_encrypt_cfm(conn, status, 0x00);
			}
		}
		conn->auth_initiator = 1;
=======
		if (conn->state == BT_CONFIG) {
			hci_proto_connect_cfm(conn, status);
			hci_conn_drop(conn);
		}
>>>>>>> refs/remotes/origin/master
	}

	hci_dev_unlock(hdev);
}

static void hci_cs_set_conn_encrypt(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_set_conn_encrypt *cp;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	if (!status)
		return;

	cp = hci_sent_cmd_data(hdev, HCI_OP_SET_CONN_ENCRYPT);
	if (!cp)
		return;

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(cp->handle));
	if (conn) {
		if (conn->state == BT_CONFIG) {
			hci_proto_connect_cfm(conn, status);
<<<<<<< HEAD
			hci_conn_put(conn);
=======
			hci_conn_drop(conn);
>>>>>>> refs/remotes/origin/master
		}
	}

	hci_dev_unlock(hdev);
}

static int hci_outgoing_auth_needed(struct hci_dev *hdev,
<<<<<<< HEAD
							struct hci_conn *conn)
=======
				    struct hci_conn *conn)
>>>>>>> refs/remotes/origin/master
{
	if (conn->state != BT_CONFIG || !conn->out)
		return 0;

	if (conn->pending_sec_level == BT_SECURITY_SDP)
		return 0;

	/* Only request authentication for SSP connections or non-SSP
<<<<<<< HEAD
	 * devices with sec_level >= BT_SECURITY_MEDIUM*/
	 BT_DBG("Pending sec level is %d", conn->pending_sec_level);
	if (!(hdev->ssp_mode > 0 && conn->ssp_mode > 0) &&
				conn->pending_sec_level < BT_SECURITY_MEDIUM)
=======
	 * devices with sec_level HIGH or if MITM protection is requested */
	if (!hci_conn_ssp_enabled(conn) && !(conn->auth_type & 0x01) &&
	    conn->pending_sec_level != BT_SECURITY_HIGH)
>>>>>>> refs/remotes/origin/master
		return 0;

	return 1;
}

<<<<<<< HEAD
=======
static int hci_resolve_name(struct hci_dev *hdev,
				   struct inquiry_entry *e)
{
	struct hci_cp_remote_name_req cp;

	memset(&cp, 0, sizeof(cp));

	bacpy(&cp.bdaddr, &e->data.bdaddr);
	cp.pscan_rep_mode = e->data.pscan_rep_mode;
	cp.pscan_mode = e->data.pscan_mode;
	cp.clock_offset = e->data.clock_offset;

	return hci_send_cmd(hdev, HCI_OP_REMOTE_NAME_REQ, sizeof(cp), &cp);
}

static bool hci_resolve_next_name(struct hci_dev *hdev)
{
	struct discovery_state *discov = &hdev->discovery;
	struct inquiry_entry *e;

	if (list_empty(&discov->resolve))
		return false;

	e = hci_inquiry_cache_lookup_resolve(hdev, BDADDR_ANY, NAME_NEEDED);
	if (!e)
		return false;

	if (hci_resolve_name(hdev, e) == 0) {
		e->name_state = NAME_PENDING;
		return true;
	}

	return false;
}

static void hci_check_pending_name(struct hci_dev *hdev, struct hci_conn *conn,
				   bdaddr_t *bdaddr, u8 *name, u8 name_len)
{
	struct discovery_state *discov = &hdev->discovery;
	struct inquiry_entry *e;

	if (conn && !test_and_set_bit(HCI_CONN_MGMT_CONNECTED, &conn->flags))
		mgmt_device_connected(hdev, bdaddr, ACL_LINK, 0x00, 0, name,
				      name_len, conn->dev_class);

	if (discov->state == DISCOVERY_STOPPED)
		return;

	if (discov->state == DISCOVERY_STOPPING)
		goto discov_complete;

	if (discov->state != DISCOVERY_RESOLVING)
		return;

	e = hci_inquiry_cache_lookup_resolve(hdev, bdaddr, NAME_PENDING);
	/* If the device was not found in a list of found devices names of which
	 * are pending. there is no need to continue resolving a next name as it
	 * will be done upon receiving another Remote Name Request Complete
	 * Event */
	if (!e)
		return;

	list_del(&e->list);
	if (name) {
		e->name_state = NAME_KNOWN;
		mgmt_remote_name(hdev, bdaddr, ACL_LINK, 0x00,
				 e->data.rssi, name, name_len);
	} else {
		e->name_state = NAME_NOT_KNOWN;
	}

	if (hci_resolve_next_name(hdev))
		return;

discov_complete:
	hci_discovery_set_state(hdev, DISCOVERY_STOPPED);
}

>>>>>>> refs/remotes/origin/master
static void hci_cs_remote_name_req(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_remote_name_req *cp;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	/* If successful wait for the name req complete event before
	 * checking for the need to do authentication */
	if (!status)
		return;

	cp = hci_sent_cmd_data(hdev, HCI_OP_REMOTE_NAME_REQ);
	if (!cp)
		return;

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &cp->bdaddr);
<<<<<<< HEAD
	if (conn && hci_outgoing_auth_needed(hdev, conn)) {
		struct hci_cp_auth_requested cp;
		cp.handle = __cpu_to_le16(conn->handle);
		hci_send_cmd(hdev, HCI_OP_AUTH_REQUESTED, sizeof(cp), &cp);
	}

=======

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		hci_check_pending_name(hdev, conn, &cp->bdaddr, NULL, 0);

	if (!conn)
		goto unlock;

	if (!hci_outgoing_auth_needed(hdev, conn))
		goto unlock;

	if (!test_and_set_bit(HCI_CONN_AUTH_PEND, &conn->flags)) {
		struct hci_cp_auth_requested auth_cp;

		auth_cp.handle = __cpu_to_le16(conn->handle);
		hci_send_cmd(hdev, HCI_OP_AUTH_REQUESTED,
			     sizeof(auth_cp), &auth_cp);
	}

unlock:
>>>>>>> refs/remotes/origin/master
	hci_dev_unlock(hdev);
}

static void hci_cs_read_remote_features(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_read_remote_features *cp;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	if (!status)
		return;

	cp = hci_sent_cmd_data(hdev, HCI_OP_READ_REMOTE_FEATURES);
	if (!cp)
		return;

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(cp->handle));
	if (conn) {
		if (conn->state == BT_CONFIG) {
			hci_proto_connect_cfm(conn, status);
<<<<<<< HEAD
			hci_conn_put(conn);
=======
			hci_conn_drop(conn);
>>>>>>> refs/remotes/origin/master
		}
	}

	hci_dev_unlock(hdev);
}

static void hci_cs_read_remote_ext_features(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_read_remote_ext_features *cp;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	if (!status)
		return;

	cp = hci_sent_cmd_data(hdev, HCI_OP_READ_REMOTE_EXT_FEATURES);
	if (!cp)
		return;

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(cp->handle));
	if (conn) {
		if (conn->state == BT_CONFIG) {
			hci_proto_connect_cfm(conn, status);
<<<<<<< HEAD
			hci_conn_put(conn);
=======
			hci_conn_drop(conn);
>>>>>>> refs/remotes/origin/master
		}
	}

	hci_dev_unlock(hdev);
}

static void hci_cs_setup_sync_conn(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_setup_sync_conn *cp;
	struct hci_conn *acl, *sco;
	__u16 handle;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	if (!status)
		return;

	cp = hci_sent_cmd_data(hdev, HCI_OP_SETUP_SYNC_CONN);
	if (!cp)
		return;

	handle = __le16_to_cpu(cp->handle);

<<<<<<< HEAD
	BT_DBG("%s handle %d", hdev->name, handle);
=======
	BT_DBG("%s handle 0x%4.4x", hdev->name, handle);
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	acl = hci_conn_hash_lookup_handle(hdev, handle);
	if (acl) {
		sco = acl->link;
		if (sco) {
			sco->state = BT_CLOSED;

			hci_proto_connect_cfm(sco, status);
			hci_conn_del(sco);
		}
	}

	hci_dev_unlock(hdev);
}

static void hci_cs_sniff_mode(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_sniff_mode *cp;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	if (!status)
		return;

	cp = hci_sent_cmd_data(hdev, HCI_OP_SNIFF_MODE);
	if (!cp)
		return;

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(cp->handle));
	if (conn) {
<<<<<<< HEAD
		clear_bit(HCI_CONN_MODE_CHANGE_PEND, &conn->pend);

		if (test_and_clear_bit(HCI_CONN_SCO_SETUP_PEND, &conn->pend))
=======
		clear_bit(HCI_CONN_MODE_CHANGE_PEND, &conn->flags);

		if (test_and_clear_bit(HCI_CONN_SCO_SETUP_PEND, &conn->flags))
>>>>>>> refs/remotes/origin/master
			hci_sco_setup(conn, status);
	}

	hci_dev_unlock(hdev);
}

static void hci_cs_exit_sniff_mode(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_exit_sniff_mode *cp;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status 0x%x", hdev->name, status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, status);
>>>>>>> refs/remotes/origin/master

	if (!status)
		return;

	cp = hci_sent_cmd_data(hdev, HCI_OP_EXIT_SNIFF_MODE);
	if (!cp)
		return;

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(cp->handle));
	if (conn) {
<<<<<<< HEAD
		clear_bit(HCI_CONN_MODE_CHANGE_PEND, &conn->pend);

		if (test_and_clear_bit(HCI_CONN_SCO_SETUP_PEND, &conn->pend))
=======
		clear_bit(HCI_CONN_MODE_CHANGE_PEND, &conn->flags);

		if (test_and_clear_bit(HCI_CONN_SCO_SETUP_PEND, &conn->flags))
>>>>>>> refs/remotes/origin/master
			hci_sco_setup(conn, status);
	}

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static void hci_cs_le_create_conn(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_le_create_conn *cp;
	struct hci_conn *conn;
	unsigned long exp = msecs_to_jiffies(5000);

	BT_DBG("%s status 0x%x", hdev->name, status);

	cp = hci_sent_cmd_data(hdev, HCI_OP_LE_CREATE_CONN);
=======
static void hci_cs_disconnect(struct hci_dev *hdev, u8 status)
{
	struct hci_cp_disconnect *cp;
	struct hci_conn *conn;

	if (!status)
		return;

	cp = hci_sent_cmd_data(hdev, HCI_OP_DISCONNECT);
>>>>>>> refs/remotes/origin/master
	if (!cp)
		return;

	hci_dev_lock(hdev);

<<<<<<< HEAD
	conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &cp->peer_addr);

	BT_DBG("%s bdaddr %s conn %p", hdev->name, batostr(&cp->peer_addr),
		conn);

	if (status) {
		if (conn && conn->state == BT_CONNECT) {
			conn->state = BT_CLOSED;
			hci_proto_connect_cfm(conn, status);
			hci_conn_del(conn);
		}
	} else {
		if (!conn) {
			conn = hci_le_conn_add(hdev, &cp->peer_addr,
						cp->peer_addr_type);
			if (conn)
				conn->out = 1;
			else
				BT_ERR("No memory for new connection");
		} else
			exp = msecs_to_jiffies(conn->conn_timeout * 1000);

		if (conn && exp)
			mod_timer(&conn->disc_timer, jiffies + exp);
	}
=======
	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(cp->handle));
	if (conn)
		mgmt_disconnect_failed(hdev, &conn->dst, conn->type,
				       conn->dst_type, status);
>>>>>>> refs/remotes/origin/master

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static void hci_cs_accept_logical_link(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_create_logical_link *ap;
	struct hci_chan *chan;

	BT_DBG("%s status 0x%x", hdev->name, status);

	ap = hci_sent_cmd_data(hdev, HCI_OP_ACCEPT_LOGICAL_LINK);
	if (!ap)
=======
static void hci_cs_create_phylink(struct hci_dev *hdev, u8 status)
{
	struct hci_cp_create_phy_link *cp;

	BT_DBG("%s status 0x%2.2x", hdev->name, status);

	cp = hci_sent_cmd_data(hdev, HCI_OP_CREATE_PHY_LINK);
	if (!cp)
>>>>>>> refs/remotes/origin/master
		return;

	hci_dev_lock(hdev);

<<<<<<< HEAD
	chan = hci_chan_list_lookup_id(hdev, ap->phy_handle);

	BT_DBG("%s chan %p", hdev->name, chan);

	if (status) {
		if (chan && chan->state == BT_CONNECT) {
			chan->state = BT_CLOSED;
			hci_proto_create_cfm(chan, status);
		}
	} else if (chan) {
		chan->state = BT_CONNECT2;
=======
	if (status) {
		struct hci_conn *hcon;

		hcon = hci_conn_hash_lookup_handle(hdev, cp->phy_handle);
		if (hcon)
			hci_conn_del(hcon);
	} else {
		amp_write_remote_assoc(hdev, cp->phy_handle);
>>>>>>> refs/remotes/origin/master
	}

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static void hci_cs_create_logical_link(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_create_logical_link *cp;
	struct hci_chan *chan;

	BT_DBG("%s status 0x%x", hdev->name, status);

	cp = hci_sent_cmd_data(hdev, HCI_OP_CREATE_LOGICAL_LINK);
	if (!cp)
		return;

	hci_dev_lock(hdev);

	chan = hci_chan_list_lookup_id(hdev, cp->phy_handle);

	BT_DBG("%s chan %p", hdev->name, chan);

	if (status) {
		if (chan && chan->state == BT_CONNECT) {
			chan->state = BT_CLOSED;
			hci_proto_create_cfm(chan, status);
		}
	} else if (chan)
			chan->state = BT_CONNECT2;

	hci_dev_unlock(hdev);
}

static void hci_cs_flow_spec_modify(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_flow_spec_modify *cp;
	struct hci_chan *chan;

	BT_DBG("%s status 0x%x", hdev->name, status);

	cp = hci_sent_cmd_data(hdev, HCI_OP_FLOW_SPEC_MODIFY);
	if (!cp)
		return;

	hci_dev_lock(hdev);

	chan = hci_chan_list_lookup_handle(hdev, cp->log_handle);
	if (chan) {
		if (status)
			hci_proto_modify_cfm(chan, status);
		else {
			chan->tx_fs = cp->tx_fs;
			chan->rx_fs = cp->rx_fs;
		}
	}

	hci_dev_unlock(hdev);
}

static void hci_cs_disconn_logical_link(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_disconn_logical_link *cp;
	struct hci_chan *chan;

	if (!status)
		return;

	BT_DBG("%s status 0x%x", hdev->name, status);

	cp = hci_sent_cmd_data(hdev, HCI_OP_DISCONN_LOGICAL_LINK);
	if (!cp)
		return;

	hci_dev_lock(hdev);

	chan = hci_chan_list_lookup_handle(hdev, cp->log_handle);
	if (chan)
		hci_chan_del(chan);

	hci_dev_unlock(hdev);
}

static void hci_cs_disconn_physical_link(struct hci_dev *hdev, __u8 status)
{
	struct hci_cp_disconn_phys_link *cp;
	struct hci_conn *conn;

	if (!status)
		return;

	BT_DBG("%s status 0x%x", hdev->name, status);

	cp = hci_sent_cmd_data(hdev, HCI_OP_DISCONN_PHYS_LINK);
	if (!cp)
=======
static void hci_cs_accept_phylink(struct hci_dev *hdev, u8 status)
{
	struct hci_cp_accept_phy_link *cp;

	BT_DBG("%s status 0x%2.2x", hdev->name, status);

	if (status)
		return;

	cp = hci_sent_cmd_data(hdev, HCI_OP_ACCEPT_PHY_LINK);
	if (!cp)
		return;

	amp_write_remote_assoc(hdev, cp->phy_handle);
}

static void hci_inquiry_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);
	struct discovery_state *discov = &hdev->discovery;
	struct inquiry_entry *e;

	BT_DBG("%s status 0x%2.2x", hdev->name, status);

	hci_conn_check_pending(hdev);

	if (!test_and_clear_bit(HCI_INQUIRY, &hdev->flags))
		return;

	smp_mb__after_clear_bit(); /* wake_up_bit advises about this barrier */
	wake_up_bit(&hdev->flags, HCI_INQUIRY);

	if (!test_bit(HCI_MGMT, &hdev->dev_flags))
>>>>>>> refs/remotes/origin/master
		return;

	hci_dev_lock(hdev);

<<<<<<< HEAD
	conn = hci_conn_hash_lookup_handle(hdev, cp->phy_handle);
	if (conn) {
		conn->state = BT_CLOSED;
		hci_conn_del(conn);
	}

	hci_dev_unlock(hdev);
}

static void hci_cs_le_start_enc(struct hci_dev *hdev, u8 status)
{
	BT_DBG("%s status 0x%x", hdev->name, status);
}

static inline void hci_inquiry_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	__u8 status = *((__u8 *) skb->data);

	BT_DBG("%s status %d", hdev->name, status);

	if (!hdev->disco_state)
		clear_bit(HCI_INQUIRY, &hdev->flags);

	hci_req_complete(hdev, HCI_OP_INQUIRY, status);
	hci_dev_lock(hdev);

	if (test_bit(HCI_MGMT, &hdev->flags))
		mgmt_inquiry_complete_evt(hdev->id, status);
	hci_dev_unlock(hdev);

	if (!lmp_le_capable(hdev))
		hci_conn_check_pending(hdev);
}

static inline void hci_inquiry_result_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
	if (discov->state != DISCOVERY_FINDING)
		goto unlock;

	if (list_empty(&discov->resolve)) {
		hci_discovery_set_state(hdev, DISCOVERY_STOPPED);
		goto unlock;
	}

	e = hci_inquiry_cache_lookup_resolve(hdev, BDADDR_ANY, NAME_NEEDED);
	if (e && hci_resolve_name(hdev, e) == 0) {
		e->name_state = NAME_PENDING;
		hci_discovery_set_state(hdev, DISCOVERY_RESOLVING);
	} else {
		hci_discovery_set_state(hdev, DISCOVERY_STOPPED);
	}

unlock:
	hci_dev_unlock(hdev);
}

static void hci_inquiry_result_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct inquiry_data data;
	struct inquiry_info *info = (void *) (skb->data + 1);
	int num_rsp = *((__u8 *) skb->data);

	BT_DBG("%s num_rsp %d", hdev->name, num_rsp);

	if (!num_rsp)
		return;

<<<<<<< HEAD
	hci_dev_lock(hdev);

	for (; num_rsp; num_rsp--, info++) {
=======
	if (test_bit(HCI_PERIODIC_INQ, &hdev->dev_flags))
		return;

	hci_dev_lock(hdev);

	for (; num_rsp; num_rsp--, info++) {
		bool name_known, ssp;

>>>>>>> refs/remotes/origin/master
		bacpy(&data.bdaddr, &info->bdaddr);
		data.pscan_rep_mode	= info->pscan_rep_mode;
		data.pscan_period_mode	= info->pscan_period_mode;
		data.pscan_mode		= info->pscan_mode;
		memcpy(data.dev_class, info->dev_class, 3);
		data.clock_offset	= info->clock_offset;
		data.rssi		= 0x00;
		data.ssp_mode		= 0x00;
<<<<<<< HEAD
		hci_inquiry_cache_update(hdev, &data);
		mgmt_device_found(hdev->id, &info->bdaddr, 0, 0,
					info->dev_class, 0, 0, NULL);
=======

		name_known = hci_inquiry_cache_update(hdev, &data, false, &ssp);
		mgmt_device_found(hdev, &info->bdaddr, ACL_LINK, 0x00,
				  info->dev_class, 0, !name_known, ssp, NULL,
				  0);
>>>>>>> refs/remotes/origin/master
	}

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_conn_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_conn_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_conn_complete *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, ev->link_type, &ev->bdaddr);
	if (!conn) {
		if (ev->link_type != SCO_LINK)
			goto unlock;

		conn = hci_conn_hash_lookup_ba(hdev, ESCO_LINK, &ev->bdaddr);
		if (!conn)
			goto unlock;

		conn->type = SCO_LINK;
	}

	if (!ev->status) {
		conn->handle = __le16_to_cpu(ev->handle);

		if (conn->type == ACL_LINK) {
			conn->state = BT_CONFIG;
			hci_conn_hold(conn);
<<<<<<< HEAD
			conn->disc_timeout = HCI_DISCONN_TIMEOUT;
			mgmt_connected(hdev->id, &ev->bdaddr, 0);
		} else if (conn->type == LE_LINK) {
			conn->state = BT_CONNECTED;
			conn->disc_timeout = HCI_DISCONN_TIMEOUT;
			mgmt_connected(hdev->id, &ev->bdaddr, 1);
		} else
			conn->state = BT_CONNECTED;

		hci_conn_hold_device(conn);
=======

			if (!conn->out && !hci_conn_ssp_enabled(conn) &&
			    !hci_find_link_key(hdev, &ev->bdaddr))
				conn->disc_timeout = HCI_PAIRING_TIMEOUT;
			else
				conn->disc_timeout = HCI_DISCONN_TIMEOUT;
		} else
			conn->state = BT_CONNECTED;

>>>>>>> refs/remotes/origin/master
		hci_conn_add_sysfs(conn);

		if (test_bit(HCI_AUTH, &hdev->flags))
			conn->link_mode |= HCI_LM_AUTH;

		if (test_bit(HCI_ENCRYPT, &hdev->flags))
			conn->link_mode |= HCI_LM_ENCRYPT;

<<<<<<< HEAD
		/* Get remote version */
		if (conn->type == ACL_LINK) {
			struct hci_cp_read_remote_version cp;
			cp.handle = ev->handle;
<<<<<<< HEAD
			hci_send_cmd(hdev, HCI_OP_READ_CLOCK_OFFSET,
				sizeof(cp), &cp);
=======
>>>>>>> refs/remotes/origin/cm-10.0
			hci_send_cmd(hdev, HCI_OP_READ_REMOTE_VERSION,
				sizeof(cp), &cp);
		}

		/* Set packet type for incoming connection */
		if (!conn->out && hdev->hci_ver < 3) {
			struct hci_cp_change_conn_ptype cp;
			cp.handle = ev->handle;
			cp.pkt_type = cpu_to_le16(conn->pkt_type);
			hci_send_cmd(hdev, HCI_OP_CHANGE_CONN_PTYPE,
							sizeof(cp), &cp);
		}
	} else {
		conn->state = BT_CLOSED;
		if (conn->type == ACL_LINK || conn->type == LE_LINK)
			mgmt_connect_failed(hdev->id, &ev->bdaddr, ev->status);
=======
		/* Get remote features */
		if (conn->type == ACL_LINK) {
			struct hci_cp_read_remote_features cp;
			cp.handle = ev->handle;
			hci_send_cmd(hdev, HCI_OP_READ_REMOTE_FEATURES,
				     sizeof(cp), &cp);
		}

		/* Set packet type for incoming connection */
		if (!conn->out && hdev->hci_ver < BLUETOOTH_VER_2_0) {
			struct hci_cp_change_conn_ptype cp;
			cp.handle = ev->handle;
			cp.pkt_type = cpu_to_le16(conn->pkt_type);
			hci_send_cmd(hdev, HCI_OP_CHANGE_CONN_PTYPE, sizeof(cp),
				     &cp);
		}
	} else {
		conn->state = BT_CLOSED;
		if (conn->type == ACL_LINK)
			mgmt_connect_failed(hdev, &ev->bdaddr, conn->type,
					    conn->dst_type, ev->status);
>>>>>>> refs/remotes/origin/master
	}

	if (conn->type == ACL_LINK)
		hci_sco_setup(conn, ev->status);

	if (ev->status) {
		hci_proto_connect_cfm(conn, ev->status);
		hci_conn_del(conn);
	} else if (ev->link_type != ACL_LINK)
		hci_proto_connect_cfm(conn, ev->status);

unlock:
	hci_dev_unlock(hdev);

	hci_conn_check_pending(hdev);
}

<<<<<<< HEAD
static inline bool is_sco_active(struct hci_dev *hdev)
{
	if (hci_conn_hash_lookup_state(hdev, SCO_LINK, BT_CONNECTED) ||
			(hci_conn_hash_lookup_state(hdev, ESCO_LINK,
						    BT_CONNECTED)))
		return true;
	return false;
}

static inline void hci_conn_request_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_conn_request *ev = (void *) skb->data;
	int mask = hdev->link_mode;

	BT_DBG("%s bdaddr %s type 0x%x", hdev->name,
					batostr(&ev->bdaddr), ev->link_type);

	mask |= hci_proto_connect_ind(hdev, &ev->bdaddr, ev->link_type);

	if ((mask & HCI_LM_ACCEPT) &&
			!hci_blacklist_lookup(hdev, &ev->bdaddr)) {
=======
static void hci_conn_request_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_conn_request *ev = (void *) skb->data;
	int mask = hdev->link_mode;
	__u8 flags = 0;

	BT_DBG("%s bdaddr %pMR type 0x%x", hdev->name, &ev->bdaddr,
	       ev->link_type);

	mask |= hci_proto_connect_ind(hdev, &ev->bdaddr, ev->link_type,
				      &flags);

	if ((mask & HCI_LM_ACCEPT) &&
	    !hci_blacklist_lookup(hdev, &ev->bdaddr, BDADDR_BREDR)) {
>>>>>>> refs/remotes/origin/master
		/* Connection accepted */
		struct inquiry_entry *ie;
		struct hci_conn *conn;

		hci_dev_lock(hdev);

		ie = hci_inquiry_cache_lookup(hdev, &ev->bdaddr);
		if (ie)
			memcpy(ie->data.dev_class, ev->dev_class, 3);

<<<<<<< HEAD
		conn = hci_conn_hash_lookup_ba(hdev, ev->link_type, &ev->bdaddr);
		if (!conn) {
			/* pkt_type not yet used for incoming connections */
			conn = hci_conn_add(hdev, ev->link_type, 0, &ev->bdaddr);
=======
		conn = hci_conn_hash_lookup_ba(hdev, ev->link_type,
					       &ev->bdaddr);
		if (!conn) {
			conn = hci_conn_add(hdev, ev->link_type, &ev->bdaddr);
>>>>>>> refs/remotes/origin/master
			if (!conn) {
				BT_ERR("No memory for new connection");
				hci_dev_unlock(hdev);
				return;
			}
		}

		memcpy(conn->dev_class, ev->dev_class, 3);
<<<<<<< HEAD
		/* For incoming connection update remote class to userspace */
		mgmt_remote_class(hdev->id, &ev->bdaddr, ev->dev_class);
		conn->state = BT_CONNECT;

		hci_dev_unlock(hdev);

		if (ev->link_type == ACL_LINK || !lmp_esco_capable(hdev)) {
			struct hci_cp_accept_conn_req cp;

			bacpy(&cp.bdaddr, &ev->bdaddr);

			if (lmp_rswitch_capable(hdev) && ((mask & HCI_LM_MASTER)
						|| is_sco_active(hdev)))
=======

		hci_dev_unlock(hdev);

		if (ev->link_type == ACL_LINK ||
		    (!(flags & HCI_PROTO_DEFER) && !lmp_esco_capable(hdev))) {
			struct hci_cp_accept_conn_req cp;
			conn->state = BT_CONNECT;

			bacpy(&cp.bdaddr, &ev->bdaddr);

			if (lmp_rswitch_capable(hdev) && (mask & HCI_LM_MASTER))
>>>>>>> refs/remotes/origin/master
				cp.role = 0x00; /* Become master */
			else
				cp.role = 0x01; /* Remain slave */

<<<<<<< HEAD
			hci_send_cmd(hdev, HCI_OP_ACCEPT_CONN_REQ,
							sizeof(cp), &cp);
		} else {
			struct hci_cp_accept_sync_conn_req cp;
=======
			hci_send_cmd(hdev, HCI_OP_ACCEPT_CONN_REQ, sizeof(cp),
				     &cp);
		} else if (!(flags & HCI_PROTO_DEFER)) {
			struct hci_cp_accept_sync_conn_req cp;
			conn->state = BT_CONNECT;
>>>>>>> refs/remotes/origin/master

			bacpy(&cp.bdaddr, &ev->bdaddr);
			cp.pkt_type = cpu_to_le16(conn->pkt_type);

<<<<<<< HEAD
			cp.tx_bandwidth   = cpu_to_le32(0x00001f40);
			cp.rx_bandwidth   = cpu_to_le32(0x00001f40);
			cp.max_latency    = cpu_to_le16(0x000A);
			cp.content_format = cpu_to_le16(hdev->voice_setting);
			cp.retrans_effort = 0x01;

			hci_send_cmd(hdev, HCI_OP_ACCEPT_SYNC_CONN_REQ,
							sizeof(cp), &cp);
=======
			cp.tx_bandwidth   = __constant_cpu_to_le32(0x00001f40);
			cp.rx_bandwidth   = __constant_cpu_to_le32(0x00001f40);
			cp.max_latency    = __constant_cpu_to_le16(0xffff);
			cp.content_format = cpu_to_le16(hdev->voice_setting);
			cp.retrans_effort = 0xff;

			hci_send_cmd(hdev, HCI_OP_ACCEPT_SYNC_CONN_REQ,
				     sizeof(cp), &cp);
		} else {
			conn->state = BT_CONNECT2;
			hci_proto_connect_cfm(conn, 0);
>>>>>>> refs/remotes/origin/master
		}
	} else {
		/* Connection rejected */
		struct hci_cp_reject_conn_req cp;

		bacpy(&cp.bdaddr, &ev->bdaddr);
<<<<<<< HEAD
		cp.reason = 0x0f;
=======
		cp.reason = HCI_ERROR_REJ_BAD_ADDR;
>>>>>>> refs/remotes/origin/master
		hci_send_cmd(hdev, HCI_OP_REJECT_CONN_REQ, sizeof(cp), &cp);
	}
}

<<<<<<< HEAD
static inline void hci_disconn_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static u8 hci_to_mgmt_reason(u8 err)
{
	switch (err) {
	case HCI_ERROR_CONNECTION_TIMEOUT:
		return MGMT_DEV_DISCONN_TIMEOUT;
	case HCI_ERROR_REMOTE_USER_TERM:
	case HCI_ERROR_REMOTE_LOW_RESOURCES:
	case HCI_ERROR_REMOTE_POWER_OFF:
		return MGMT_DEV_DISCONN_REMOTE;
	case HCI_ERROR_LOCAL_HOST_TERM:
		return MGMT_DEV_DISCONN_LOCAL_HOST;
	default:
		return MGMT_DEV_DISCONN_UNKNOWN;
	}
}

static void hci_disconn_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_disconn_complete *ev = (void *) skb->data;
	struct hci_conn *conn;

<<<<<<< HEAD
<<<<<<< HEAD
	BT_DBG("%s status %d reason %d", hdev->name, ev->status, ev->reason);
=======
	BT_DBG("%s status %d", hdev->name, ev->status);
>>>>>>> refs/remotes/origin/cm-10.0

	if (ev->status) {
		hci_dev_lock(hdev);
		mgmt_disconnect_failed(hdev->id);
		hci_dev_unlock(hdev);
		return;
	}
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, ev->status);
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(ev->handle));
	if (!conn)
		goto unlock;

<<<<<<< HEAD
	conn->state = BT_CLOSED;

	if (conn->type == ACL_LINK || conn->type == LE_LINK)
<<<<<<< HEAD
		mgmt_disconnected(hdev->id, &conn->dst, ev->reason);
=======
		mgmt_disconnected(hdev->id, &conn->dst);
>>>>>>> refs/remotes/origin/cm-10.0

	if (conn->type == LE_LINK)
		del_timer(&conn->smp_timer);

	hci_proto_disconn_cfm(conn, ev->reason, 0);
	hci_conn_del(conn);
=======
	if (ev->status == 0)
		conn->state = BT_CLOSED;

	if (test_and_clear_bit(HCI_CONN_MGMT_CONNECTED, &conn->flags) &&
	    (conn->type == ACL_LINK || conn->type == LE_LINK)) {
		if (ev->status) {
			mgmt_disconnect_failed(hdev, &conn->dst, conn->type,
					       conn->dst_type, ev->status);
		} else {
			u8 reason = hci_to_mgmt_reason(ev->reason);

			mgmt_device_disconnected(hdev, &conn->dst, conn->type,
						 conn->dst_type, reason);
		}
	}

	if (ev->status == 0) {
		u8 type = conn->type;

		if (type == ACL_LINK && conn->flush_key)
			hci_remove_link_key(hdev, &conn->dst);
		hci_proto_disconn_cfm(conn, ev->reason);
		hci_conn_del(conn);

		/* Re-enable advertising if necessary, since it might
		 * have been disabled by the connection. From the
		 * HCI_LE_Set_Advertise_Enable command description in
		 * the core specification (v4.0):
		 * "The Controller shall continue advertising until the Host
		 * issues an LE_Set_Advertise_Enable command with
		 * Advertising_Enable set to 0x00 (Advertising is disabled)
		 * or until a connection is created or until the Advertising
		 * is timed out due to Directed Advertising."
		 */
		if (type == LE_LINK)
			mgmt_reenable_advertising(hdev);
	}
>>>>>>> refs/remotes/origin/master

unlock:
	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_auth_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_auth_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_auth_complete *ev = (void *) skb->data;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status %d", hdev->name, ev->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, ev->status);
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(ev->handle));
<<<<<<< HEAD
	if (conn) {
		if (ev->status == 0x06 && hdev->ssp_mode > 0 &&
							conn->ssp_mode > 0) {
			struct hci_cp_auth_requested cp;
<<<<<<< HEAD
			hci_remove_link_key(hdev, &conn->dst);
			cp.handle = cpu_to_le16(conn->handle);
=======
			struct link_key *key;
			key = hci_find_link_key(hdev, &conn->dst);
			if (key && key->key_type == 0x05) {
				BT_INFO("Previous key was authenticated, "
					"updating the auth type for outgoing connection");
				conn->auth_type = HCI_AT_DEDICATED_BONDING_MITM;
			}
			hci_remove_link_key(hdev, &conn->dst);
			cp.handle = cpu_to_le16(conn->handle);
			/*Initiates dedicated bonding as pin or key is missing
			on remote device*/
			/*In case if remote device is ssp supported,
			reduce the security level to MEDIUM if it is HIGH*/
			if (conn->ssp_mode && conn->auth_initiator &&
				conn->io_capability != 0x03) {
				conn->pending_sec_level = BT_SECURITY_HIGH;
				conn->auth_type = HCI_AT_DEDICATED_BONDING_MITM;
			}
>>>>>>> refs/remotes/origin/cm-10.0
			hci_send_cmd(conn->hdev, HCI_OP_AUTH_REQUESTED,
							sizeof(cp), &cp);
			hci_dev_unlock(hdev);
			BT_INFO("Pin or key missing");
			return;
		}

		if (!ev->status) {
			conn->link_mode |= HCI_LM_AUTH;
			conn->sec_level = conn->pending_sec_level;
		} else {
			mgmt_auth_failed(hdev->id, &conn->dst, ev->status);
			conn->sec_level = BT_SECURITY_LOW;
		}

		clear_bit(HCI_CONN_AUTH_PEND, &conn->pend);

		if (conn->state == BT_CONFIG) {
			if (!ev->status && hdev->ssp_mode > 0 &&
							conn->ssp_mode > 0) {
				struct hci_cp_set_conn_encrypt cp;
				cp.handle  = ev->handle;
				cp.encrypt = 0x01;
				hci_send_cmd(hdev, HCI_OP_SET_CONN_ENCRYPT,
							sizeof(cp), &cp);
			} else {
				conn->state = BT_CONNECTED;
				hci_proto_connect_cfm(conn, ev->status);
				hci_conn_put(conn);
			}
		} else {
			hci_auth_cfm(conn, ev->status);

			hci_conn_hold(conn);
			conn->disc_timeout = HCI_DISCONN_TIMEOUT;
			hci_conn_put(conn);
		}

		if (test_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend)) {
			if (!ev->status) {
				if (conn->link_mode & HCI_LM_ENCRYPT) {
					/* Encryption implies authentication */
					conn->link_mode |= HCI_LM_AUTH;
					conn->link_mode |= HCI_LM_ENCRYPT;
					conn->sec_level =
						conn->pending_sec_level;
					clear_bit(HCI_CONN_ENCRYPT_PEND,
							&conn->pend);
					hci_encrypt_cfm(conn, ev->status, 1);

					if (test_bit(HCI_MGMT, &hdev->flags))
						mgmt_encrypt_change(hdev->id,
							&conn->dst,
							ev->status);

				} else {
					struct hci_cp_set_conn_encrypt cp;
					cp.handle  = ev->handle;
					cp.encrypt = 0x01;
					hci_send_cmd(hdev,
						HCI_OP_SET_CONN_ENCRYPT,
						sizeof(cp), &cp);
				}
			} else {
				clear_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend);
				hci_encrypt_cfm(conn, ev->status, 0x00);
			}
		}
	}

	hci_dev_unlock(hdev);
}

static inline void hci_remote_name_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
	if (!conn)
		goto unlock;

	if (!ev->status) {
		if (!hci_conn_ssp_enabled(conn) &&
		    test_bit(HCI_CONN_REAUTH_PEND, &conn->flags)) {
			BT_INFO("re-auth of legacy device is not possible.");
		} else {
			conn->link_mode |= HCI_LM_AUTH;
			conn->sec_level = conn->pending_sec_level;
		}
	} else {
		mgmt_auth_failed(hdev, &conn->dst, conn->type, conn->dst_type,
				 ev->status);
	}

	clear_bit(HCI_CONN_AUTH_PEND, &conn->flags);
	clear_bit(HCI_CONN_REAUTH_PEND, &conn->flags);

	if (conn->state == BT_CONFIG) {
		if (!ev->status && hci_conn_ssp_enabled(conn)) {
			struct hci_cp_set_conn_encrypt cp;
			cp.handle  = ev->handle;
			cp.encrypt = 0x01;
			hci_send_cmd(hdev, HCI_OP_SET_CONN_ENCRYPT, sizeof(cp),
				     &cp);
		} else {
			conn->state = BT_CONNECTED;
			hci_proto_connect_cfm(conn, ev->status);
			hci_conn_drop(conn);
		}
	} else {
		hci_auth_cfm(conn, ev->status);

		hci_conn_hold(conn);
		conn->disc_timeout = HCI_DISCONN_TIMEOUT;
		hci_conn_drop(conn);
	}

	if (test_bit(HCI_CONN_ENCRYPT_PEND, &conn->flags)) {
		if (!ev->status) {
			struct hci_cp_set_conn_encrypt cp;
			cp.handle  = ev->handle;
			cp.encrypt = 0x01;
			hci_send_cmd(hdev, HCI_OP_SET_CONN_ENCRYPT, sizeof(cp),
				     &cp);
		} else {
			clear_bit(HCI_CONN_ENCRYPT_PEND, &conn->flags);
			hci_encrypt_cfm(conn, ev->status, 0x00);
		}
	}

unlock:
	hci_dev_unlock(hdev);
}

static void hci_remote_name_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_remote_name *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s", hdev->name);

	hci_conn_check_pending(hdev);

	hci_dev_lock(hdev);

<<<<<<< HEAD
	if (test_bit(HCI_MGMT, &hdev->flags))
		mgmt_remote_name(hdev->id, &ev->bdaddr, ev->status, ev->name);

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &ev->bdaddr);
	if (conn && hci_outgoing_auth_needed(hdev, conn)) {
=======
	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &ev->bdaddr);

	if (!test_bit(HCI_MGMT, &hdev->dev_flags))
		goto check_auth;

	if (ev->status == 0)
		hci_check_pending_name(hdev, conn, &ev->bdaddr, ev->name,
				       strnlen(ev->name, HCI_MAX_NAME_LENGTH));
	else
		hci_check_pending_name(hdev, conn, &ev->bdaddr, NULL, 0);

check_auth:
	if (!conn)
		goto unlock;

	if (!hci_outgoing_auth_needed(hdev, conn))
		goto unlock;

	if (!test_and_set_bit(HCI_CONN_AUTH_PEND, &conn->flags)) {
>>>>>>> refs/remotes/origin/master
		struct hci_cp_auth_requested cp;
		cp.handle = __cpu_to_le16(conn->handle);
		hci_send_cmd(hdev, HCI_OP_AUTH_REQUESTED, sizeof(cp), &cp);
	}

<<<<<<< HEAD
	hci_dev_unlock(hdev);
}

static inline void hci_encrypt_change_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
unlock:
	hci_dev_unlock(hdev);
}

static void hci_encrypt_change_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_encrypt_change *ev = (void *) skb->data;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status %d", hdev->name, ev->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, ev->status);
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(ev->handle));
	if (conn) {
		if (!ev->status) {
			if (ev->encrypt) {
				/* Encryption implies authentication */
				conn->link_mode |= HCI_LM_AUTH;
				conn->link_mode |= HCI_LM_ENCRYPT;
				conn->sec_level = conn->pending_sec_level;
			} else
				conn->link_mode &= ~HCI_LM_ENCRYPT;
		}

<<<<<<< HEAD
		clear_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend);
=======
		clear_bit(HCI_CONN_ENCRYPT_PEND, &conn->flags);

		if (ev->status && conn->state == BT_CONNECTED) {
			hci_disconnect(conn, HCI_ERROR_AUTH_FAILURE);
			hci_conn_drop(conn);
			goto unlock;
		}
>>>>>>> refs/remotes/origin/master

		if (conn->state == BT_CONFIG) {
			if (!ev->status)
				conn->state = BT_CONNECTED;

			hci_proto_connect_cfm(conn, ev->status);
<<<<<<< HEAD
			hci_conn_put(conn);
		} else {
			/*
			* If the remote device does not support
			* Pause Encryption, usually during the
			* roleSwitch we see Encryption disable
			* for short duration. Allow remote device
			* to disable encryption
			* for short duration in this case.
			*/
			if ((ev->encrypt == 0) && (ev->status == 0) &&
				((conn->features[5] & LMP_PAUSE_ENC) == 0)) {
				mod_timer(&conn->encrypt_pause_timer,
					jiffies + msecs_to_jiffies(500));
				BT_INFO("enc pause timer, enc_pend_flag set");
			} else {
				del_timer(&conn->encrypt_pause_timer);
				hci_encrypt_cfm(conn, ev->status, ev->encrypt);
			}
		}

		if (test_bit(HCI_MGMT, &hdev->flags))
			mgmt_encrypt_change(hdev->id, &conn->dst, ev->status);
	}

	hci_dev_unlock(hdev);
}

static inline void hci_change_link_key_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
			hci_conn_drop(conn);
		} else
			hci_encrypt_cfm(conn, ev->status, ev->encrypt);
	}

unlock:
	hci_dev_unlock(hdev);
}

static void hci_change_link_key_complete_evt(struct hci_dev *hdev,
					     struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_change_link_key_complete *ev = (void *) skb->data;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status %d", hdev->name, ev->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, ev->status);
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(ev->handle));
	if (conn) {
		if (!ev->status)
			conn->link_mode |= HCI_LM_SECURE;

<<<<<<< HEAD
		clear_bit(HCI_CONN_AUTH_PEND, &conn->pend);
=======
		clear_bit(HCI_CONN_AUTH_PEND, &conn->flags);
>>>>>>> refs/remotes/origin/master

		hci_key_change_cfm(conn, ev->status);
	}

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_remote_features_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_remote_features_evt(struct hci_dev *hdev,
				    struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_remote_features *ev = (void *) skb->data;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status %d", hdev->name, ev->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, ev->status);
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(ev->handle));
	if (!conn)
		goto unlock;

<<<<<<< HEAD
	if (!ev->status) {
		memcpy(conn->features, ev->features, 8);
		mgmt_remote_features(hdev->id, &conn->dst, ev->features);
	}
=======
	if (!ev->status)
		memcpy(conn->features[0], ev->features, 8);
>>>>>>> refs/remotes/origin/master

	if (conn->state != BT_CONFIG)
		goto unlock;

	if (!ev->status && lmp_ssp_capable(hdev) && lmp_ssp_capable(conn)) {
		struct hci_cp_read_remote_ext_features cp;
		cp.handle = ev->handle;
		cp.page = 0x01;
		hci_send_cmd(hdev, HCI_OP_READ_REMOTE_EXT_FEATURES,
<<<<<<< HEAD
							sizeof(cp), &cp);
		goto unlock;
	} else  if (!(lmp_ssp_capable(conn)) && conn->auth_initiator &&
<<<<<<< HEAD
		(conn->pending_sec_level == BT_SECURITY_VERY_HIGH)) {
=======
		(conn->pending_sec_level == BT_SECURITY_HIGH)) {
>>>>>>> refs/remotes/origin/cm-10.0
		conn->pending_sec_level = BT_SECURITY_MEDIUM;
	}

	if (!ev->status) {
=======
			     sizeof(cp), &cp);
		goto unlock;
	}

	if (!ev->status && !test_bit(HCI_CONN_MGMT_CONNECTED, &conn->flags)) {
>>>>>>> refs/remotes/origin/master
		struct hci_cp_remote_name_req cp;
		memset(&cp, 0, sizeof(cp));
		bacpy(&cp.bdaddr, &conn->dst);
		cp.pscan_rep_mode = 0x02;
		hci_send_cmd(hdev, HCI_OP_REMOTE_NAME_REQ, sizeof(cp), &cp);
<<<<<<< HEAD
	}
=======
	} else if (!test_and_set_bit(HCI_CONN_MGMT_CONNECTED, &conn->flags))
		mgmt_device_connected(hdev, &conn->dst, conn->type,
				      conn->dst_type, 0, NULL, 0,
				      conn->dev_class);
>>>>>>> refs/remotes/origin/master

	if (!hci_outgoing_auth_needed(hdev, conn)) {
		conn->state = BT_CONNECTED;
		hci_proto_connect_cfm(conn, ev->status);
<<<<<<< HEAD
		hci_conn_put(conn);
=======
		hci_conn_drop(conn);
>>>>>>> refs/remotes/origin/master
	}

unlock:
	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_remote_version_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_remote_version *ev = (void *) skb->data;
	struct hci_cp_read_remote_features cp;
	struct hci_conn *conn;
	BT_DBG("%s status %d", hdev->name, ev->status);

	hci_dev_lock(hdev);
	cp.handle = ev->handle;
	hci_send_cmd(hdev, HCI_OP_READ_REMOTE_FEATURES,
				sizeof(cp), &cp);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(ev->handle));
	if (!conn)
		goto unlock;
	if (!ev->status)
		mgmt_remote_version(hdev->id, &conn->dst, ev->lmp_ver,
				ev->manufacturer, ev->lmp_subver);
unlock:
	hci_dev_unlock(hdev);
}

static inline void hci_qos_setup_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	BT_DBG("%s", hdev->name);
}

static inline void hci_cmd_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_cmd_complete *ev = (void *) skb->data;
=======
static void hci_cmd_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_cmd_complete *ev = (void *) skb->data;
	u8 status = skb->data[sizeof(*ev)];
>>>>>>> refs/remotes/origin/master
	__u16 opcode;

	skb_pull(skb, sizeof(*ev));

	opcode = __le16_to_cpu(ev->opcode);

<<<<<<< HEAD
	if (test_bit(HCI_RESET, &hdev->flags) && (opcode != HCI_OP_RESET))
		return;

=======
>>>>>>> refs/remotes/origin/master
	switch (opcode) {
	case HCI_OP_INQUIRY_CANCEL:
		hci_cc_inquiry_cancel(hdev, skb);
		break;

<<<<<<< HEAD
	case HCI_OP_EXIT_PERIODIC_INQ:
		hci_cc_exit_periodic_inq(hdev, skb);
		break;

	case HCI_OP_LINK_KEY_REPLY:
		hci_cc_link_key_reply(hdev, skb);
=======
	case HCI_OP_PERIODIC_INQ:
		hci_cc_periodic_inq(hdev, skb);
		break;

	case HCI_OP_EXIT_PERIODIC_INQ:
		hci_cc_exit_periodic_inq(hdev, skb);
>>>>>>> refs/remotes/origin/master
		break;

	case HCI_OP_REMOTE_NAME_REQ_CANCEL:
		hci_cc_remote_name_req_cancel(hdev, skb);
		break;

	case HCI_OP_ROLE_DISCOVERY:
		hci_cc_role_discovery(hdev, skb);
		break;

	case HCI_OP_READ_LINK_POLICY:
		hci_cc_read_link_policy(hdev, skb);
		break;

	case HCI_OP_WRITE_LINK_POLICY:
		hci_cc_write_link_policy(hdev, skb);
		break;

	case HCI_OP_READ_DEF_LINK_POLICY:
		hci_cc_read_def_link_policy(hdev, skb);
		break;

	case HCI_OP_WRITE_DEF_LINK_POLICY:
		hci_cc_write_def_link_policy(hdev, skb);
		break;

	case HCI_OP_RESET:
		hci_cc_reset(hdev, skb);
		break;

	case HCI_OP_WRITE_LOCAL_NAME:
		hci_cc_write_local_name(hdev, skb);
		break;

	case HCI_OP_READ_LOCAL_NAME:
		hci_cc_read_local_name(hdev, skb);
		break;

	case HCI_OP_WRITE_AUTH_ENABLE:
		hci_cc_write_auth_enable(hdev, skb);
		break;

	case HCI_OP_WRITE_ENCRYPT_MODE:
		hci_cc_write_encrypt_mode(hdev, skb);
		break;

	case HCI_OP_WRITE_SCAN_ENABLE:
		hci_cc_write_scan_enable(hdev, skb);
		break;

	case HCI_OP_READ_CLASS_OF_DEV:
		hci_cc_read_class_of_dev(hdev, skb);
		break;

	case HCI_OP_WRITE_CLASS_OF_DEV:
		hci_cc_write_class_of_dev(hdev, skb);
		break;

	case HCI_OP_READ_VOICE_SETTING:
		hci_cc_read_voice_setting(hdev, skb);
		break;

	case HCI_OP_WRITE_VOICE_SETTING:
		hci_cc_write_voice_setting(hdev, skb);
		break;

<<<<<<< HEAD
	case HCI_OP_HOST_BUFFER_SIZE:
		hci_cc_host_buffer_size(hdev, skb);
		break;

	case HCI_OP_READ_SSP_MODE:
		hci_cc_read_ssp_mode(hdev, skb);
=======
	case HCI_OP_READ_NUM_SUPPORTED_IAC:
		hci_cc_read_num_supported_iac(hdev, skb);
>>>>>>> refs/remotes/origin/master
		break;

	case HCI_OP_WRITE_SSP_MODE:
		hci_cc_write_ssp_mode(hdev, skb);
		break;

	case HCI_OP_READ_LOCAL_VERSION:
		hci_cc_read_local_version(hdev, skb);
		break;

	case HCI_OP_READ_LOCAL_COMMANDS:
		hci_cc_read_local_commands(hdev, skb);
		break;

	case HCI_OP_READ_LOCAL_FEATURES:
		hci_cc_read_local_features(hdev, skb);
		break;

<<<<<<< HEAD
=======
	case HCI_OP_READ_LOCAL_EXT_FEATURES:
		hci_cc_read_local_ext_features(hdev, skb);
		break;

>>>>>>> refs/remotes/origin/master
	case HCI_OP_READ_BUFFER_SIZE:
		hci_cc_read_buffer_size(hdev, skb);
		break;

	case HCI_OP_READ_BD_ADDR:
		hci_cc_read_bd_addr(hdev, skb);
		break;

<<<<<<< HEAD
	case HCI_OP_WRITE_CA_TIMEOUT:
		hci_cc_write_ca_timeout(hdev, skb);
		break;

	case HCI_OP_READ_FLOW_CONTROL_MODE:
		hci_cc_read_flow_control_mode(hdev, skb);
		break;

	case HCI_OP_READ_DATA_BLOCK_SIZE:
		hci_cc_read_data_block_size(hdev, skb);
		break;

	case HCI_OP_READ_LOCAL_AMP_INFO:
		hci_cc_read_local_amp_info(hdev, skb);
		break;

	case HCI_OP_READ_LOCAL_AMP_ASSOC:
	case HCI_OP_WRITE_REMOTE_AMP_ASSOC:
		hci_amp_cmd_complete(hdev, opcode, skb);
		break;

	case HCI_OP_DELETE_STORED_LINK_KEY:
		hci_cc_delete_stored_link_key(hdev, skb);
		break;

	case HCI_OP_SET_EVENT_MASK:
		hci_cc_set_event_mask(hdev, skb);
		break;

	case HCI_OP_WRITE_INQUIRY_MODE:
		hci_cc_write_inquiry_mode(hdev, skb);
=======
	case HCI_OP_READ_PAGE_SCAN_ACTIVITY:
		hci_cc_read_page_scan_activity(hdev, skb);
		break;

	case HCI_OP_WRITE_PAGE_SCAN_ACTIVITY:
		hci_cc_write_page_scan_activity(hdev, skb);
		break;

	case HCI_OP_READ_PAGE_SCAN_TYPE:
		hci_cc_read_page_scan_type(hdev, skb);
		break;

	case HCI_OP_WRITE_PAGE_SCAN_TYPE:
		hci_cc_write_page_scan_type(hdev, skb);
		break;

	case HCI_OP_READ_DATA_BLOCK_SIZE:
		hci_cc_read_data_block_size(hdev, skb);
		break;

	case HCI_OP_READ_FLOW_CONTROL_MODE:
		hci_cc_read_flow_control_mode(hdev, skb);
		break;

	case HCI_OP_READ_LOCAL_AMP_INFO:
		hci_cc_read_local_amp_info(hdev, skb);
		break;

	case HCI_OP_READ_LOCAL_AMP_ASSOC:
		hci_cc_read_local_amp_assoc(hdev, skb);
>>>>>>> refs/remotes/origin/master
		break;

	case HCI_OP_READ_INQ_RSP_TX_POWER:
		hci_cc_read_inq_rsp_tx_power(hdev, skb);
		break;

<<<<<<< HEAD
	case HCI_OP_SET_EVENT_FLT:
		hci_cc_set_event_flt(hdev, skb);
		break;

=======
>>>>>>> refs/remotes/origin/master
	case HCI_OP_PIN_CODE_REPLY:
		hci_cc_pin_code_reply(hdev, skb);
		break;

	case HCI_OP_PIN_CODE_NEG_REPLY:
		hci_cc_pin_code_neg_reply(hdev, skb);
		break;

	case HCI_OP_READ_LOCAL_OOB_DATA:
		hci_cc_read_local_oob_data_reply(hdev, skb);
		break;

	case HCI_OP_LE_READ_BUFFER_SIZE:
		hci_cc_le_read_buffer_size(hdev, skb);
		break;

<<<<<<< HEAD
<<<<<<< HEAD
	case HCI_OP_LE_READ_WHITE_LIST_SIZE:
		hci_cc_le_read_white_list_size(hdev, skb);
		break;

	case HCI_OP_LE_CLEAR_WHITE_LIST:
		hci_cc_le_clear_white_list(hdev, skb);
		break;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	case HCI_OP_READ_RSSI:
		hci_cc_read_rssi(hdev, skb);
=======
	case HCI_OP_LE_READ_LOCAL_FEATURES:
		hci_cc_le_read_local_features(hdev, skb);
		break;

	case HCI_OP_LE_READ_ADV_TX_POWER:
		hci_cc_le_read_adv_tx_power(hdev, skb);
>>>>>>> refs/remotes/origin/master
		break;

	case HCI_OP_USER_CONFIRM_REPLY:
		hci_cc_user_confirm_reply(hdev, skb);
		break;

	case HCI_OP_USER_CONFIRM_NEG_REPLY:
		hci_cc_user_confirm_neg_reply(hdev, skb);
		break;

<<<<<<< HEAD
	case HCI_OP_LE_LTK_REPLY:
		hci_cc_le_ltk_reply(hdev, skb);
		break;

	case HCI_OP_LE_LTK_NEG_REPLY:
		hci_cc_le_ltk_neg_reply(hdev, skb);
=======
	case HCI_OP_USER_PASSKEY_REPLY:
		hci_cc_user_passkey_reply(hdev, skb);
		break;

	case HCI_OP_USER_PASSKEY_NEG_REPLY:
		hci_cc_user_passkey_neg_reply(hdev, skb);
		break;

	case HCI_OP_LE_SET_ADV_ENABLE:
		hci_cc_le_set_adv_enable(hdev, skb);
>>>>>>> refs/remotes/origin/master
		break;

	case HCI_OP_LE_SET_SCAN_ENABLE:
		hci_cc_le_set_scan_enable(hdev, skb);
		break;

<<<<<<< HEAD
	default:
		BT_DBG("%s opcode 0x%x", hdev->name, opcode);
		break;
	}

	if (ev->opcode != HCI_OP_NOP)
		del_timer(&hdev->cmd_timer);

	if (ev->ncmd) {
		atomic_set(&hdev->cmd_cnt, 1);
		if (!skb_queue_empty(&hdev->cmd_q))
			tasklet_schedule(&hdev->cmd_task);
	}
}

static inline void hci_cmd_status_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
	case HCI_OP_LE_READ_WHITE_LIST_SIZE:
		hci_cc_le_read_white_list_size(hdev, skb);
		break;

	case HCI_OP_LE_READ_SUPPORTED_STATES:
		hci_cc_le_read_supported_states(hdev, skb);
		break;

	case HCI_OP_WRITE_LE_HOST_SUPPORTED:
		hci_cc_write_le_host_supported(hdev, skb);
		break;

	case HCI_OP_WRITE_REMOTE_AMP_ASSOC:
		hci_cc_write_remote_amp_assoc(hdev, skb);
		break;

	default:
		BT_DBG("%s opcode 0x%4.4x", hdev->name, opcode);
		break;
	}

	if (opcode != HCI_OP_NOP)
		del_timer(&hdev->cmd_timer);

	hci_req_cmd_complete(hdev, opcode, status);

	if (ev->ncmd && !test_bit(HCI_RESET, &hdev->flags)) {
		atomic_set(&hdev->cmd_cnt, 1);
		if (!skb_queue_empty(&hdev->cmd_q))
			queue_work(hdev->workqueue, &hdev->cmd_work);
	}
}

static void hci_cmd_status_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_cmd_status *ev = (void *) skb->data;
	__u16 opcode;

	skb_pull(skb, sizeof(*ev));

	opcode = __le16_to_cpu(ev->opcode);

	switch (opcode) {
	case HCI_OP_INQUIRY:
		hci_cs_inquiry(hdev, ev->status);
		break;

	case HCI_OP_CREATE_CONN:
		hci_cs_create_conn(hdev, ev->status);
		break;

	case HCI_OP_ADD_SCO:
		hci_cs_add_sco(hdev, ev->status);
		break;

	case HCI_OP_AUTH_REQUESTED:
		hci_cs_auth_requested(hdev, ev->status);
		break;

	case HCI_OP_SET_CONN_ENCRYPT:
		hci_cs_set_conn_encrypt(hdev, ev->status);
		break;

	case HCI_OP_REMOTE_NAME_REQ:
		hci_cs_remote_name_req(hdev, ev->status);
		break;

	case HCI_OP_READ_REMOTE_FEATURES:
		hci_cs_read_remote_features(hdev, ev->status);
		break;

	case HCI_OP_READ_REMOTE_EXT_FEATURES:
		hci_cs_read_remote_ext_features(hdev, ev->status);
		break;

	case HCI_OP_SETUP_SYNC_CONN:
		hci_cs_setup_sync_conn(hdev, ev->status);
		break;

	case HCI_OP_SNIFF_MODE:
		hci_cs_sniff_mode(hdev, ev->status);
		break;

	case HCI_OP_EXIT_SNIFF_MODE:
		hci_cs_exit_sniff_mode(hdev, ev->status);
		break;

<<<<<<< HEAD
	case HCI_OP_CREATE_LOGICAL_LINK:
		hci_cs_create_logical_link(hdev, ev->status);
		break;

	case HCI_OP_ACCEPT_LOGICAL_LINK:
		hci_cs_accept_logical_link(hdev, ev->status);
		break;

	case HCI_OP_DISCONN_LOGICAL_LINK:
		hci_cs_disconn_logical_link(hdev, ev->status);
		break;

	case HCI_OP_FLOW_SPEC_MODIFY:
		hci_cs_flow_spec_modify(hdev, ev->status);
		break;

	case HCI_OP_CREATE_PHYS_LINK:
	case HCI_OP_ACCEPT_PHYS_LINK:
		hci_amp_cmd_status(hdev, opcode, ev->status);
		break;

	case HCI_OP_DISCONN_PHYS_LINK:
		hci_cs_disconn_physical_link(hdev, ev->status);

	case HCI_OP_DISCONNECT:
		if (ev->status != 0)
			mgmt_disconnect_failed(hdev->id);
		break;

	case HCI_OP_LE_CREATE_CONN:
		hci_cs_le_create_conn(hdev, ev->status);
		break;

	case HCI_OP_LE_START_ENC:
		hci_cs_le_start_enc(hdev, ev->status);
		break;

	default:
		BT_DBG("%s opcode 0x%x", hdev->name, opcode);
		break;
	}

	if (ev->opcode != HCI_OP_NOP)
		del_timer(&hdev->cmd_timer);

	if (ev->ncmd && !test_bit(HCI_RESET, &hdev->flags)) {
		atomic_set(&hdev->cmd_cnt, 1);
		if (!skb_queue_empty(&hdev->cmd_q))
			tasklet_schedule(&hdev->cmd_task);
	}
}

<<<<<<< HEAD
static inline void hci_hardware_error_evt(struct hci_dev *hdev,
					struct sk_buff *skb)
{
	struct hci_ev_hardware_error *ev = (void *) skb->data;

	BT_ERR("hdev=%p, hw_err_code = %u", hdev, ev->hw_err_code);

	if (hdev && hdev->dev_type == HCI_BREDR) {
		hci_dev_lock_bh(hdev);
		mgmt_powered(hdev->id, 1);
		hci_dev_unlock_bh(hdev);
	}

}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static inline void hci_role_change_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
	case HCI_OP_DISCONNECT:
		hci_cs_disconnect(hdev, ev->status);
		break;

	case HCI_OP_CREATE_PHY_LINK:
		hci_cs_create_phylink(hdev, ev->status);
		break;

	case HCI_OP_ACCEPT_PHY_LINK:
		hci_cs_accept_phylink(hdev, ev->status);
		break;

	default:
		BT_DBG("%s opcode 0x%4.4x", hdev->name, opcode);
		break;
	}

	if (opcode != HCI_OP_NOP)
		del_timer(&hdev->cmd_timer);

	if (ev->status ||
	    (hdev->sent_cmd && !bt_cb(hdev->sent_cmd)->req.event))
		hci_req_cmd_complete(hdev, opcode, ev->status);

	if (ev->ncmd && !test_bit(HCI_RESET, &hdev->flags)) {
		atomic_set(&hdev->cmd_cnt, 1);
		if (!skb_queue_empty(&hdev->cmd_q))
			queue_work(hdev->workqueue, &hdev->cmd_work);
	}
}

static void hci_role_change_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_role_change *ev = (void *) skb->data;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status %d", hdev->name, ev->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, ev->status);
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &ev->bdaddr);
	if (conn) {
		if (!ev->status) {
			if (ev->role)
				conn->link_mode &= ~HCI_LM_MASTER;
			else
				conn->link_mode |= HCI_LM_MASTER;
		}

<<<<<<< HEAD
		clear_bit(HCI_CONN_RSWITCH_PEND, &conn->pend);
=======
		clear_bit(HCI_CONN_RSWITCH_PEND, &conn->flags);
>>>>>>> refs/remotes/origin/master

		hci_role_switch_cfm(conn, ev->status, ev->role);
	}

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_num_comp_pkts_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_num_comp_pkts *ev = (void *) skb->data;
	__le16 *ptr;
	int i;

	skb_pull(skb, sizeof(*ev));

	BT_DBG("%s num_hndl %d", hdev->name, ev->num_hndl);

	if (skb->len < ev->num_hndl * 4) {
=======
static void hci_num_comp_pkts_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_num_comp_pkts *ev = (void *) skb->data;
	int i;

	if (hdev->flow_ctl_mode != HCI_FLOW_CTL_MODE_PACKET_BASED) {
		BT_ERR("Wrong event for mode %d", hdev->flow_ctl_mode);
		return;
	}

	if (skb->len < sizeof(*ev) || skb->len < sizeof(*ev) +
	    ev->num_hndl * sizeof(struct hci_comp_pkts_info)) {
>>>>>>> refs/remotes/origin/master
		BT_DBG("%s bad parameters", hdev->name);
		return;
	}

<<<<<<< HEAD
	tasklet_disable(&hdev->tx_task);

	for (i = 0, ptr = (__le16 *) skb->data; i < ev->num_hndl; i++) {
		struct hci_conn *conn = NULL;
		struct hci_chan *chan;
		__u16  handle, count;

		handle = get_unaligned_le16(ptr++);
		count  = get_unaligned_le16(ptr++);

		if (hdev->dev_type == HCI_BREDR)
			conn = hci_conn_hash_lookup_handle(hdev, handle);
		else {
			chan = hci_chan_list_lookup_handle(hdev, handle);
			if (chan)
				conn = chan->conn;
		}
		if (conn) {
			conn->sent -= count;

			if (conn->type == ACL_LINK) {
				hdev->acl_cnt += count;
				if (hdev->acl_cnt > hdev->acl_pkts)
					hdev->acl_cnt = hdev->acl_pkts;
			} else if (conn->type == LE_LINK) {
				if (hdev->le_pkts) {
					hdev->le_cnt += count;
					if (hdev->le_cnt > hdev->le_pkts)
						hdev->le_cnt = hdev->le_pkts;
				} else {
					hdev->acl_cnt += count;
					if (hdev->acl_cnt > hdev->acl_pkts)
						hdev->acl_cnt = hdev->acl_pkts;
				}
			} else {
				hdev->sco_cnt += count;
				if (hdev->sco_cnt > hdev->sco_pkts)
					hdev->sco_cnt = hdev->sco_pkts;
			}
		}
	}

	tasklet_schedule(&hdev->tx_task);

	tasklet_enable(&hdev->tx_task);
}

static inline void hci_num_comp_blocks_evt(struct hci_dev *hdev,
					struct sk_buff *skb)
{
	struct hci_ev_num_comp_blocks *ev = (void *) skb->data;
	__le16 *ptr;
	int i;

	skb_pull(skb, sizeof(*ev));

	BT_DBG("%s total_num_blocks %d num_hndl %d",
		hdev->name, ev->total_num_blocks, ev->num_hndl);

	if (skb->len < ev->num_hndl * 6) {
=======
	BT_DBG("%s num_hndl %d", hdev->name, ev->num_hndl);

	for (i = 0; i < ev->num_hndl; i++) {
		struct hci_comp_pkts_info *info = &ev->handles[i];
		struct hci_conn *conn;
		__u16  handle, count;

		handle = __le16_to_cpu(info->handle);
		count  = __le16_to_cpu(info->count);

		conn = hci_conn_hash_lookup_handle(hdev, handle);
		if (!conn)
			continue;

		conn->sent -= count;

		switch (conn->type) {
		case ACL_LINK:
			hdev->acl_cnt += count;
			if (hdev->acl_cnt > hdev->acl_pkts)
				hdev->acl_cnt = hdev->acl_pkts;
			break;

		case LE_LINK:
			if (hdev->le_pkts) {
				hdev->le_cnt += count;
				if (hdev->le_cnt > hdev->le_pkts)
					hdev->le_cnt = hdev->le_pkts;
			} else {
				hdev->acl_cnt += count;
				if (hdev->acl_cnt > hdev->acl_pkts)
					hdev->acl_cnt = hdev->acl_pkts;
			}
			break;

		case SCO_LINK:
			hdev->sco_cnt += count;
			if (hdev->sco_cnt > hdev->sco_pkts)
				hdev->sco_cnt = hdev->sco_pkts;
			break;

		default:
			BT_ERR("Unknown type %d conn %p", conn->type, conn);
			break;
		}
	}

	queue_work(hdev->workqueue, &hdev->tx_work);
}

static struct hci_conn *__hci_conn_lookup_handle(struct hci_dev *hdev,
						 __u16 handle)
{
	struct hci_chan *chan;

	switch (hdev->dev_type) {
	case HCI_BREDR:
		return hci_conn_hash_lookup_handle(hdev, handle);
	case HCI_AMP:
		chan = hci_chan_lookup_handle(hdev, handle);
		if (chan)
			return chan->conn;
		break;
	default:
		BT_ERR("%s unknown dev_type %d", hdev->name, hdev->dev_type);
		break;
	}

	return NULL;
}

static void hci_num_comp_blocks_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_num_comp_blocks *ev = (void *) skb->data;
	int i;

	if (hdev->flow_ctl_mode != HCI_FLOW_CTL_MODE_BLOCK_BASED) {
		BT_ERR("Wrong event for mode %d", hdev->flow_ctl_mode);
		return;
	}

	if (skb->len < sizeof(*ev) || skb->len < sizeof(*ev) +
	    ev->num_hndl * sizeof(struct hci_comp_blocks_info)) {
>>>>>>> refs/remotes/origin/master
		BT_DBG("%s bad parameters", hdev->name);
		return;
	}

<<<<<<< HEAD
	tasklet_disable(&hdev->tx_task);

	for (i = 0, ptr = (__le16 *) skb->data; i < ev->num_hndl; i++) {
		struct hci_conn *conn = NULL;
		struct hci_chan *chan;
		__u16  handle, block_count;

		handle = get_unaligned_le16(ptr++);

		/* Skip packet count */
		ptr++;
		block_count  = get_unaligned_le16(ptr++);

		BT_DBG("%s handle %d count %d", hdev->name, handle,
			block_count);

		if (hdev->dev_type == HCI_BREDR)
			conn = hci_conn_hash_lookup_handle(hdev, handle);
		else {
			chan = hci_chan_list_lookup_handle(hdev, handle);
			if (chan)
				conn = chan->conn;
		}
		if (conn) {
			BT_DBG("%s conn %p sent %d", hdev->name,
				conn, conn->sent);

			conn->sent -= block_count;

			if (conn->type == ACL_LINK) {
				hdev->acl_cnt += block_count;
				if (hdev->acl_cnt > hdev->acl_pkts)
					hdev->acl_cnt = hdev->acl_pkts;
			} else {
				/* We should not find ourselves here */
				BT_DBG("Unexpected event for SCO connection");
			}
		}
	}

	tasklet_schedule(&hdev->tx_task);

	tasklet_enable(&hdev->tx_task);
}

static inline void hci_mode_change_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
	BT_DBG("%s num_blocks %d num_hndl %d", hdev->name, ev->num_blocks,
	       ev->num_hndl);

	for (i = 0; i < ev->num_hndl; i++) {
		struct hci_comp_blocks_info *info = &ev->handles[i];
		struct hci_conn *conn = NULL;
		__u16  handle, block_count;

		handle = __le16_to_cpu(info->handle);
		block_count = __le16_to_cpu(info->blocks);

		conn = __hci_conn_lookup_handle(hdev, handle);
		if (!conn)
			continue;

		conn->sent -= block_count;

		switch (conn->type) {
		case ACL_LINK:
		case AMP_LINK:
			hdev->block_cnt += block_count;
			if (hdev->block_cnt > hdev->num_blocks)
				hdev->block_cnt = hdev->num_blocks;
			break;

		default:
			BT_ERR("Unknown type %d conn %p", conn->type, conn);
			break;
		}
	}

	queue_work(hdev->workqueue, &hdev->tx_work);
}

static void hci_mode_change_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_mode_change *ev = (void *) skb->data;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status %d", hdev->name, ev->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, ev->status);
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(ev->handle));
	if (conn) {
		conn->mode = ev->mode;
<<<<<<< HEAD
		conn->interval = __le16_to_cpu(ev->interval);

		if (!test_and_clear_bit(HCI_CONN_MODE_CHANGE_PEND, &conn->pend)) {
			if (conn->mode == HCI_CM_ACTIVE)
				conn->power_save = 1;
			else
				conn->power_save = 0;
		}
<<<<<<< HEAD
=======
		if (conn->mode == HCI_CM_SNIFF)
			if (wake_lock_active(&conn->idle_lock))
				wake_unlock(&conn->idle_lock);
>>>>>>> refs/remotes/origin/cm-10.0

		if (test_and_clear_bit(HCI_CONN_SCO_SETUP_PEND, &conn->pend))
=======

		if (!test_and_clear_bit(HCI_CONN_MODE_CHANGE_PEND,
					&conn->flags)) {
			if (conn->mode == HCI_CM_ACTIVE)
				set_bit(HCI_CONN_POWER_SAVE, &conn->flags);
			else
				clear_bit(HCI_CONN_POWER_SAVE, &conn->flags);
		}

		if (test_and_clear_bit(HCI_CONN_SCO_SETUP_PEND, &conn->flags))
>>>>>>> refs/remotes/origin/master
			hci_sco_setup(conn, ev->status);
	}

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_pin_code_request_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_pin_code_request_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_pin_code_req *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &ev->bdaddr);
<<<<<<< HEAD
	if (conn && conn->state == BT_CONNECTED) {
		hci_conn_hold(conn);
		conn->disc_timeout = HCI_PAIRING_TIMEOUT;
		hci_conn_put(conn);
		hci_conn_enter_active_mode(conn, 0);
	}

	if (!test_bit(HCI_PAIRABLE, &hdev->flags))
		hci_send_cmd(hdev, HCI_OP_PIN_CODE_NEG_REPLY,
					sizeof(ev->bdaddr), &ev->bdaddr);

	if (test_bit(HCI_MGMT, &hdev->flags))
		mgmt_pin_code_request(hdev->id, &ev->bdaddr);

	hci_dev_unlock(hdev);
}

static inline void hci_link_key_request_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
	if (!conn)
		goto unlock;

	if (conn->state == BT_CONNECTED) {
		hci_conn_hold(conn);
		conn->disc_timeout = HCI_PAIRING_TIMEOUT;
		hci_conn_drop(conn);
	}

	if (!test_bit(HCI_PAIRABLE, &hdev->dev_flags))
		hci_send_cmd(hdev, HCI_OP_PIN_CODE_NEG_REPLY,
			     sizeof(ev->bdaddr), &ev->bdaddr);
	else if (test_bit(HCI_MGMT, &hdev->dev_flags)) {
		u8 secure;

		if (conn->pending_sec_level == BT_SECURITY_HIGH)
			secure = 1;
		else
			secure = 0;

		mgmt_pin_code_request(hdev, &ev->bdaddr, secure);
	}

unlock:
	hci_dev_unlock(hdev);
}

static void hci_link_key_request_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_link_key_req *ev = (void *) skb->data;
	struct hci_cp_link_key_reply cp;
	struct hci_conn *conn;
	struct link_key *key;

	BT_DBG("%s", hdev->name);

<<<<<<< HEAD
	if (!test_bit(HCI_LINK_KEYS, &hdev->flags))
=======
	if (!test_bit(HCI_MGMT, &hdev->dev_flags))
>>>>>>> refs/remotes/origin/master
		return;

	hci_dev_lock(hdev);

	key = hci_find_link_key(hdev, &ev->bdaddr);
	if (!key) {
<<<<<<< HEAD
		BT_DBG("%s link key not found for %s", hdev->name,
							batostr(&ev->bdaddr));
		goto not_found;
	}

	BT_DBG("%s found key type %u for %s", hdev->name, key->key_type,
							batostr(&ev->bdaddr));

	if (!test_bit(HCI_DEBUG_KEYS, &hdev->flags) && key->key_type == 0x03) {
=======
		BT_DBG("%s link key not found for %pMR", hdev->name,
		       &ev->bdaddr);
		goto not_found;
	}

	BT_DBG("%s found key type %u for %pMR", hdev->name, key->type,
	       &ev->bdaddr);

	if (!test_bit(HCI_DEBUG_KEYS, &hdev->dev_flags) &&
	    key->type == HCI_LK_DEBUG_COMBINATION) {
>>>>>>> refs/remotes/origin/master
		BT_DBG("%s ignoring debug key", hdev->name);
		goto not_found;
	}

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &ev->bdaddr);
<<<<<<< HEAD

	if (conn) {
		BT_DBG("Conn pending sec level is %d, ssp is %d, key len is %d",
			conn->pending_sec_level, conn->ssp_mode, key->pin_len);
	}
<<<<<<< HEAD
	if (conn && (conn->ssp_mode == 0) &&
		(conn->pending_sec_level == BT_SECURITY_VERY_HIGH) &&
		(key->pin_len != 16)) {
		BT_DBG("Security is high ignoring this key");
		goto not_found;
	}
=======
>>>>>>> refs/remotes/origin/cm-10.0

	if (key->key_type == 0x04 && conn && conn->auth_type != 0xff &&
						(conn->auth_type & 0x01)) {
		BT_DBG("%s ignoring unauthenticated key", hdev->name);
		goto not_found;
	}

	bacpy(&cp.bdaddr, &ev->bdaddr);
	memcpy(cp.link_key, key->val, 16);
=======
	if (conn) {
		if (key->type == HCI_LK_UNAUTH_COMBINATION &&
		    conn->auth_type != 0xff && (conn->auth_type & 0x01)) {
			BT_DBG("%s ignoring unauthenticated key", hdev->name);
			goto not_found;
		}

		if (key->type == HCI_LK_COMBINATION && key->pin_len < 16 &&
		    conn->pending_sec_level == BT_SECURITY_HIGH) {
			BT_DBG("%s ignoring key unauthenticated for high security",
			       hdev->name);
			goto not_found;
		}

		conn->key_type = key->type;
		conn->pin_length = key->pin_len;
	}

	bacpy(&cp.bdaddr, &ev->bdaddr);
	memcpy(cp.link_key, key->val, HCI_LINK_KEY_SIZE);
>>>>>>> refs/remotes/origin/master

	hci_send_cmd(hdev, HCI_OP_LINK_KEY_REPLY, sizeof(cp), &cp);

	hci_dev_unlock(hdev);

	return;

not_found:
	hci_send_cmd(hdev, HCI_OP_LINK_KEY_NEG_REPLY, 6, &ev->bdaddr);
	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_link_key_notify_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_link_key_notify_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_link_key_notify *ev = (void *) skb->data;
	struct hci_conn *conn;
	u8 pin_len = 0;

<<<<<<< HEAD
	BT_DBG("%s type %d", hdev->name, ev->key_type);
=======
	BT_DBG("%s", hdev->name);
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &ev->bdaddr);
	if (conn) {
		hci_conn_hold(conn);
		conn->disc_timeout = HCI_DISCONN_TIMEOUT;
<<<<<<< HEAD

		memcpy(conn->link_key, ev->link_key, 16);
		conn->key_type = ev->key_type;
		hci_disconnect_amp(conn, 0x06);

		conn->link_mode &= ~HCI_LM_ENCRYPT;
		pin_len = conn->pin_length;
		hci_conn_put(conn);
		hci_conn_enter_active_mode(conn, 0);
	}

	if (test_bit(HCI_LINK_KEYS, &hdev->flags))
		hci_add_link_key(hdev, 1, &ev->bdaddr, ev->link_key,
							ev->key_type, pin_len);
=======
		pin_len = conn->pin_length;

		if (ev->key_type != HCI_LK_CHANGED_COMBINATION)
			conn->key_type = ev->key_type;

		hci_conn_drop(conn);
	}

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		hci_add_link_key(hdev, conn, 1, &ev->bdaddr, ev->link_key,
				 ev->key_type, pin_len);
>>>>>>> refs/remotes/origin/master

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_clock_offset_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_clock_offset_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_clock_offset *ev = (void *) skb->data;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status %d", hdev->name, ev->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, ev->status);
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(ev->handle));
	if (conn && !ev->status) {
		struct inquiry_entry *ie;

		ie = hci_inquiry_cache_lookup(hdev, &conn->dst);
		if (ie) {
			ie->data.clock_offset = ev->clock_offset;
			ie->timestamp = jiffies;
		}
	}

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_pkt_type_change_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_pkt_type_change_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_pkt_type_change *ev = (void *) skb->data;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status %d", hdev->name, ev->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, ev->status);
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(ev->handle));
	if (conn && !ev->status)
		conn->pkt_type = __le16_to_cpu(ev->pkt_type);

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_pscan_rep_mode_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_pscan_rep_mode_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_pscan_rep_mode *ev = (void *) skb->data;
	struct inquiry_entry *ie;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	ie = hci_inquiry_cache_lookup(hdev, &ev->bdaddr);
	if (ie) {
		ie->data.pscan_rep_mode = ev->pscan_rep_mode;
		ie->timestamp = jiffies;
	}

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_inquiry_result_with_rssi_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct inquiry_data data;
	int num_rsp = *((__u8 *) skb->data);
=======
static void hci_inquiry_result_with_rssi_evt(struct hci_dev *hdev,
					     struct sk_buff *skb)
{
	struct inquiry_data data;
	int num_rsp = *((__u8 *) skb->data);
	bool name_known, ssp;
>>>>>>> refs/remotes/origin/master

	BT_DBG("%s num_rsp %d", hdev->name, num_rsp);

	if (!num_rsp)
		return;

<<<<<<< HEAD
=======
	if (test_bit(HCI_PERIODIC_INQ, &hdev->dev_flags))
		return;

>>>>>>> refs/remotes/origin/master
	hci_dev_lock(hdev);

	if ((skb->len - 1) / num_rsp != sizeof(struct inquiry_info_with_rssi)) {
		struct inquiry_info_with_rssi_and_pscan_mode *info;
		info = (void *) (skb->data + 1);

		for (; num_rsp; num_rsp--, info++) {
			bacpy(&data.bdaddr, &info->bdaddr);
			data.pscan_rep_mode	= info->pscan_rep_mode;
			data.pscan_period_mode	= info->pscan_period_mode;
			data.pscan_mode		= info->pscan_mode;
			memcpy(data.dev_class, info->dev_class, 3);
			data.clock_offset	= info->clock_offset;
			data.rssi		= info->rssi;
			data.ssp_mode		= 0x00;
<<<<<<< HEAD
			hci_inquiry_cache_update(hdev, &data);
			mgmt_device_found(hdev->id, &info->bdaddr, 0, 0,
						info->dev_class, info->rssi,
						0, NULL);
=======

			name_known = hci_inquiry_cache_update(hdev, &data,
							      false, &ssp);
			mgmt_device_found(hdev, &info->bdaddr, ACL_LINK, 0x00,
					  info->dev_class, info->rssi,
					  !name_known, ssp, NULL, 0);
>>>>>>> refs/remotes/origin/master
		}
	} else {
		struct inquiry_info_with_rssi *info = (void *) (skb->data + 1);

		for (; num_rsp; num_rsp--, info++) {
			bacpy(&data.bdaddr, &info->bdaddr);
			data.pscan_rep_mode	= info->pscan_rep_mode;
			data.pscan_period_mode	= info->pscan_period_mode;
			data.pscan_mode		= 0x00;
			memcpy(data.dev_class, info->dev_class, 3);
			data.clock_offset	= info->clock_offset;
			data.rssi		= info->rssi;
			data.ssp_mode		= 0x00;
<<<<<<< HEAD
			hci_inquiry_cache_update(hdev, &data);
			mgmt_device_found(hdev->id, &info->bdaddr, 0, 0,
						info->dev_class, info->rssi,
						0, NULL);
=======
			name_known = hci_inquiry_cache_update(hdev, &data,
							      false, &ssp);
			mgmt_device_found(hdev, &info->bdaddr, ACL_LINK, 0x00,
					  info->dev_class, info->rssi,
					  !name_known, ssp, NULL, 0);
>>>>>>> refs/remotes/origin/master
		}
	}

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_remote_ext_features_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_remote_ext_features_evt(struct hci_dev *hdev,
					struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_remote_ext_features *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(ev->handle));
	if (!conn)
		goto unlock;

<<<<<<< HEAD
=======
	if (ev->page < HCI_MAX_PAGES)
		memcpy(conn->features[ev->page], ev->features, 8);

>>>>>>> refs/remotes/origin/master
	if (!ev->status && ev->page == 0x01) {
		struct inquiry_entry *ie;

		ie = hci_inquiry_cache_lookup(hdev, &conn->dst);
		if (ie)
<<<<<<< HEAD
			ie->data.ssp_mode = (ev->features[0] & 0x01);

		conn->ssp_mode = (ev->features[0] & 0x01);
		/*In case if remote device ssp supported/2.0 device
<<<<<<< HEAD
		reduce the security level to MEDIUM if it is VERY HIGH*/
		if (!conn->ssp_mode && conn->auth_initiator &&
			(conn->pending_sec_level == BT_SECURITY_VERY_HIGH))
=======
		reduce the security level to MEDIUM if it is HIGH*/
		if (!conn->ssp_mode && conn->auth_initiator &&
			(conn->pending_sec_level == BT_SECURITY_HIGH))
>>>>>>> refs/remotes/origin/cm-10.0
			conn->pending_sec_level = BT_SECURITY_MEDIUM;

		if (conn->ssp_mode && conn->auth_initiator &&
			conn->io_capability != 0x03) {
<<<<<<< HEAD
			conn->pending_sec_level = BT_SECURITY_VERY_HIGH;
=======
			conn->pending_sec_level = BT_SECURITY_HIGH;
>>>>>>> refs/remotes/origin/cm-10.0
			conn->auth_type = HCI_AT_DEDICATED_BONDING_MITM;
=======
			ie->data.ssp_mode = (ev->features[0] & LMP_HOST_SSP);

		if (ev->features[0] & LMP_HOST_SSP) {
			set_bit(HCI_CONN_SSP_ENABLED, &conn->flags);
		} else {
			/* It is mandatory by the Bluetooth specification that
			 * Extended Inquiry Results are only used when Secure
			 * Simple Pairing is enabled, but some devices violate
			 * this.
			 *
			 * To make these devices work, the internal SSP
			 * enabled flag needs to be cleared if the remote host
			 * features do not indicate SSP support */
			clear_bit(HCI_CONN_SSP_ENABLED, &conn->flags);
>>>>>>> refs/remotes/origin/master
		}
	}

	if (conn->state != BT_CONFIG)
		goto unlock;

<<<<<<< HEAD
	if (!ev->status) {
=======
	if (!ev->status && !test_bit(HCI_CONN_MGMT_CONNECTED, &conn->flags)) {
>>>>>>> refs/remotes/origin/master
		struct hci_cp_remote_name_req cp;
		memset(&cp, 0, sizeof(cp));
		bacpy(&cp.bdaddr, &conn->dst);
		cp.pscan_rep_mode = 0x02;
		hci_send_cmd(hdev, HCI_OP_REMOTE_NAME_REQ, sizeof(cp), &cp);
<<<<<<< HEAD
	}
=======
	} else if (!test_and_set_bit(HCI_CONN_MGMT_CONNECTED, &conn->flags))
		mgmt_device_connected(hdev, &conn->dst, conn->type,
				      conn->dst_type, 0, NULL, 0,
				      conn->dev_class);
>>>>>>> refs/remotes/origin/master

	if (!hci_outgoing_auth_needed(hdev, conn)) {
		conn->state = BT_CONNECTED;
		hci_proto_connect_cfm(conn, ev->status);
<<<<<<< HEAD
		hci_conn_put(conn);
=======
		hci_conn_drop(conn);
>>>>>>> refs/remotes/origin/master
	}

unlock:
	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_sync_conn_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_sync_conn_complete_evt(struct hci_dev *hdev,
				       struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_sync_conn_complete *ev = (void *) skb->data;
	struct hci_conn *conn;

<<<<<<< HEAD
	BT_DBG("%s status %d", hdev->name, ev->status);
=======
	BT_DBG("%s status 0x%2.2x", hdev->name, ev->status);
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, ev->link_type, &ev->bdaddr);
	if (!conn) {
		if (ev->link_type == ESCO_LINK)
			goto unlock;

		conn = hci_conn_hash_lookup_ba(hdev, ESCO_LINK, &ev->bdaddr);
		if (!conn)
			goto unlock;

		conn->type = SCO_LINK;
	}

	switch (ev->status) {
	case 0x00:
		conn->handle = __le16_to_cpu(ev->handle);
		conn->state  = BT_CONNECTED;

<<<<<<< HEAD
		hci_conn_hold_device(conn);
		hci_conn_add_sysfs(conn);
		break;

=======
		hci_conn_add_sysfs(conn);
		break;

	case 0x0d:	/* Connection Rejected due to Limited Resources */
>>>>>>> refs/remotes/origin/master
	case 0x11:	/* Unsupported Feature or Parameter Value */
	case 0x1c:	/* SCO interval rejected */
	case 0x1a:	/* Unsupported Remote Feature */
	case 0x1f:	/* Unspecified error */
<<<<<<< HEAD
		if (conn->out && conn->attempt < 2) {
			if (!conn->hdev->is_wbs)
				conn->pkt_type =
					(hdev->esco_type & SCO_ESCO_MASK) |
					(hdev->esco_type & EDR_ESCO_MASK);
			hci_setup_sync(conn, conn->link->handle);
			goto unlock;
=======
		if (conn->out) {
			conn->pkt_type = (hdev->esco_type & SCO_ESCO_MASK) |
					(hdev->esco_type & EDR_ESCO_MASK);
			if (hci_setup_sync(conn, conn->link->handle))
				goto unlock;
>>>>>>> refs/remotes/origin/master
		}
		/* fall through */

	default:
		conn->state = BT_CLOSED;
		break;
	}

	hci_proto_connect_cfm(conn, ev->status);
	if (ev->status)
		hci_conn_del(conn);

unlock:
	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_sync_conn_changed_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	BT_DBG("%s", hdev->name);
}

static inline void hci_sniff_subrate_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_sniff_subrate *ev = (void *) skb->data;
	struct hci_conn *conn =
		hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(ev->handle));

	BT_DBG("%s status %d", hdev->name, ev->status);
	if (conn && (ev->max_rx_latency > hdev->sniff_max_interval)) {
		BT_ERR("value of rx_latency:%d", ev->max_rx_latency);
		hci_dev_lock(hdev);
		hci_conn_enter_active_mode(conn, 1);
		hci_dev_unlock(hdev);
	}
}

static inline void hci_extended_inquiry_result_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static inline size_t eir_get_length(u8 *eir, size_t eir_len)
{
	size_t parsed = 0;

	while (parsed < eir_len) {
		u8 field_len = eir[0];

		if (field_len == 0)
			return parsed;

		parsed += field_len + 1;
		eir += field_len + 1;
	}

	return eir_len;
}

static void hci_extended_inquiry_result_evt(struct hci_dev *hdev,
					    struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct inquiry_data data;
	struct extended_inquiry_info *info = (void *) (skb->data + 1);
	int num_rsp = *((__u8 *) skb->data);
<<<<<<< HEAD
=======
	size_t eir_len;
>>>>>>> refs/remotes/origin/master

	BT_DBG("%s num_rsp %d", hdev->name, num_rsp);

	if (!num_rsp)
		return;

<<<<<<< HEAD
	hci_dev_lock(hdev);

	for (; num_rsp; num_rsp--, info++) {
=======
	if (test_bit(HCI_PERIODIC_INQ, &hdev->dev_flags))
		return;

	hci_dev_lock(hdev);

	for (; num_rsp; num_rsp--, info++) {
		bool name_known, ssp;

>>>>>>> refs/remotes/origin/master
		bacpy(&data.bdaddr, &info->bdaddr);
		data.pscan_rep_mode	= info->pscan_rep_mode;
		data.pscan_period_mode	= info->pscan_period_mode;
		data.pscan_mode		= 0x00;
		memcpy(data.dev_class, info->dev_class, 3);
		data.clock_offset	= info->clock_offset;
		data.rssi		= info->rssi;
		data.ssp_mode		= 0x01;
<<<<<<< HEAD
		hci_inquiry_cache_update(hdev, &data);
		mgmt_device_found(hdev->id, &info->bdaddr, 0, 0,
				info->dev_class, info->rssi,
				HCI_MAX_EIR_LENGTH, info->data);
=======

		if (test_bit(HCI_MGMT, &hdev->dev_flags))
			name_known = eir_has_data_type(info->data,
						       sizeof(info->data),
						       EIR_NAME_COMPLETE);
		else
			name_known = true;

		name_known = hci_inquiry_cache_update(hdev, &data, name_known,
						      &ssp);
		eir_len = eir_get_length(info->data, sizeof(info->data));
		mgmt_device_found(hdev, &info->bdaddr, ACL_LINK, 0x00,
				  info->dev_class, info->rssi, !name_known,
				  ssp, info->data, eir_len);
>>>>>>> refs/remotes/origin/master
	}

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline u8 hci_get_auth_req(struct hci_conn *conn)
{
	BT_DBG("%p", conn);

	/* If remote requests dedicated bonding follow that lead */
	if (conn->remote_auth == 0x02 || conn->remote_auth == 0x03) {
		/* If both remote and local IO capabilities allow MITM
		 * protection then require it, otherwise don't */
		if (conn->remote_cap == 0x03 || conn->io_capability == 0x03) {
			return 0x02;
		} else {
			conn->auth_type |= 0x01;
			return 0x03;
		}
	}

	/* If remote requests no-bonding follow that lead */
	if (conn->remote_auth <= 0x01)
		return 0x00;
=======
static void hci_key_refresh_complete_evt(struct hci_dev *hdev,
					 struct sk_buff *skb)
{
	struct hci_ev_key_refresh_complete *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s status 0x%2.2x handle 0x%4.4x", hdev->name, ev->status,
	       __le16_to_cpu(ev->handle));

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(ev->handle));
	if (!conn)
		goto unlock;

	if (!ev->status)
		conn->sec_level = conn->pending_sec_level;

	clear_bit(HCI_CONN_ENCRYPT_PEND, &conn->flags);

	if (ev->status && conn->state == BT_CONNECTED) {
		hci_disconnect(conn, HCI_ERROR_AUTH_FAILURE);
		hci_conn_drop(conn);
		goto unlock;
	}

	if (conn->state == BT_CONFIG) {
		if (!ev->status)
			conn->state = BT_CONNECTED;

		hci_proto_connect_cfm(conn, ev->status);
		hci_conn_drop(conn);
	} else {
		hci_auth_cfm(conn, ev->status);

		hci_conn_hold(conn);
		conn->disc_timeout = HCI_DISCONN_TIMEOUT;
		hci_conn_drop(conn);
	}

unlock:
	hci_dev_unlock(hdev);
}

static u8 hci_get_auth_req(struct hci_conn *conn)
{
	/* If remote requests dedicated bonding follow that lead */
	if (conn->remote_auth == HCI_AT_DEDICATED_BONDING ||
	    conn->remote_auth == HCI_AT_DEDICATED_BONDING_MITM) {
		/* If both remote and local IO capabilities allow MITM
		 * protection then require it, otherwise don't */
		if (conn->remote_cap == HCI_IO_NO_INPUT_OUTPUT ||
		    conn->io_capability == HCI_IO_NO_INPUT_OUTPUT)
			return HCI_AT_DEDICATED_BONDING;
		else
			return HCI_AT_DEDICATED_BONDING_MITM;
	}

	/* If remote requests no-bonding follow that lead */
	if (conn->remote_auth == HCI_AT_NO_BONDING ||
	    conn->remote_auth == HCI_AT_NO_BONDING_MITM)
		return conn->remote_auth | (conn->auth_type & 0x01);
>>>>>>> refs/remotes/origin/master

	return conn->auth_type;
}

<<<<<<< HEAD
static inline void hci_io_capa_request_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_io_capa_request_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_io_capa_request *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &ev->bdaddr);
	if (!conn)
		goto unlock;

	hci_conn_hold(conn);

<<<<<<< HEAD
	if (!test_bit(HCI_MGMT, &hdev->flags))
		goto unlock;

	if (test_bit(HCI_PAIRABLE, &hdev->flags) ||
			(conn->remote_auth & ~0x01) == HCI_AT_NO_BONDING) {
		struct hci_cp_io_capability_reply cp;
		u8 io_cap = conn->io_capability;

		/* ACL-SSP does not support IO CAP 0x04 */
		cp.capability = (io_cap == 0x04) ? 0x01 : io_cap;
		bacpy(&cp.bdaddr, &ev->bdaddr);
		if (conn->auth_initiator)
			cp.authentication = conn->auth_type;
		else
			cp.authentication = hci_get_auth_req(conn);

		if ((conn->out == 0x01 || conn->remote_oob == 0x01) &&
				hci_find_remote_oob_data(hdev, &conn->dst))
=======
	if (!test_bit(HCI_MGMT, &hdev->dev_flags))
		goto unlock;

	if (test_bit(HCI_PAIRABLE, &hdev->dev_flags) ||
	    (conn->remote_auth & ~0x01) == HCI_AT_NO_BONDING) {
		struct hci_cp_io_capability_reply cp;

		bacpy(&cp.bdaddr, &ev->bdaddr);
		/* Change the IO capability from KeyboardDisplay
		 * to DisplayYesNo as it is not supported by BT spec. */
		cp.capability = (conn->io_capability == 0x04) ?
				HCI_IO_DISPLAY_YESNO : conn->io_capability;
		conn->auth_type = hci_get_auth_req(conn);
		cp.authentication = conn->auth_type;

		if (hci_find_remote_oob_data(hdev, &conn->dst) &&
		    (conn->out || test_bit(HCI_CONN_REMOTE_OOB, &conn->flags)))
>>>>>>> refs/remotes/origin/master
			cp.oob_data = 0x01;
		else
			cp.oob_data = 0x00;

		hci_send_cmd(hdev, HCI_OP_IO_CAPABILITY_REPLY,
<<<<<<< HEAD
							sizeof(cp), &cp);
=======
			     sizeof(cp), &cp);
>>>>>>> refs/remotes/origin/master
	} else {
		struct hci_cp_io_capability_neg_reply cp;

		bacpy(&cp.bdaddr, &ev->bdaddr);
<<<<<<< HEAD
		cp.reason = 0x16; /* Pairing not allowed */

		hci_send_cmd(hdev, HCI_OP_IO_CAPABILITY_NEG_REPLY,
							sizeof(cp), &cp);
=======
		cp.reason = HCI_ERROR_PAIRING_NOT_ALLOWED;

		hci_send_cmd(hdev, HCI_OP_IO_CAPABILITY_NEG_REPLY,
			     sizeof(cp), &cp);
>>>>>>> refs/remotes/origin/master
	}

unlock:
	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_io_capa_reply_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_io_capa_reply_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_io_capa_reply *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &ev->bdaddr);
	if (!conn)
		goto unlock;

	conn->remote_cap = ev->capability;
<<<<<<< HEAD
	conn->remote_oob = ev->oob_data;
	conn->remote_auth = ev->authentication;
=======
	conn->remote_auth = ev->authentication;
	if (ev->oob_data)
		set_bit(HCI_CONN_REMOTE_OOB, &conn->flags);
>>>>>>> refs/remotes/origin/master

unlock:
	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_user_ssp_confirmation_evt(struct hci_dev *hdev,
						u8 event, struct sk_buff *skb)
{
	struct hci_ev_user_confirm_req *ev = (void *) skb->data;
=======
static void hci_user_confirm_request_evt(struct hci_dev *hdev,
					 struct sk_buff *skb)
{
	struct hci_ev_user_confirm_req *ev = (void *) skb->data;
	int loc_mitm, rem_mitm, confirm_hint = 0;
	struct hci_conn *conn;
>>>>>>> refs/remotes/origin/master

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

<<<<<<< HEAD
	if (test_bit(HCI_MGMT, &hdev->flags)) {
		if (event == HCI_EV_USER_PASSKEY_REQUEST)
			mgmt_user_confirm_request(hdev->id, event,
							&ev->bdaddr, 0);
		else
			mgmt_user_confirm_request(hdev->id, event,
						&ev->bdaddr, ev->passkey);
	}

	hci_dev_unlock(hdev);
}

static inline void hci_simple_pair_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_simple_pair_complete *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);
=======
	if (!test_bit(HCI_MGMT, &hdev->dev_flags))
		goto unlock;

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &ev->bdaddr);
	if (!conn)
		goto unlock;

	loc_mitm = (conn->auth_type & 0x01);
	rem_mitm = (conn->remote_auth & 0x01);

	/* If we require MITM but the remote device can't provide that
	 * (it has NoInputNoOutput) then reject the confirmation
	 * request. The only exception is when we're dedicated bonding
	 * initiators (connect_cfm_cb set) since then we always have the MITM
	 * bit set. */
	if (!conn->connect_cfm_cb && loc_mitm &&
	    conn->remote_cap == HCI_IO_NO_INPUT_OUTPUT) {
		BT_DBG("Rejecting request: remote device can't provide MITM");
		hci_send_cmd(hdev, HCI_OP_USER_CONFIRM_NEG_REPLY,
			     sizeof(ev->bdaddr), &ev->bdaddr);
		goto unlock;
	}

	/* If no side requires MITM protection; auto-accept */
	if ((!loc_mitm || conn->remote_cap == HCI_IO_NO_INPUT_OUTPUT) &&
	    (!rem_mitm || conn->io_capability == HCI_IO_NO_INPUT_OUTPUT)) {

		/* If we're not the initiators request authorization to
		 * proceed from user space (mgmt_user_confirm with
		 * confirm_hint set to 1). */
		if (!test_bit(HCI_CONN_AUTH_PEND, &conn->flags)) {
			BT_DBG("Confirming auto-accept as acceptor");
			confirm_hint = 1;
			goto confirm;
		}

		BT_DBG("Auto-accept of user confirmation with %ums delay",
		       hdev->auto_accept_delay);

		if (hdev->auto_accept_delay > 0) {
			int delay = msecs_to_jiffies(hdev->auto_accept_delay);
			queue_delayed_work(conn->hdev->workqueue,
					   &conn->auto_accept_work, delay);
			goto unlock;
		}

		hci_send_cmd(hdev, HCI_OP_USER_CONFIRM_REPLY,
			     sizeof(ev->bdaddr), &ev->bdaddr);
		goto unlock;
	}

confirm:
	mgmt_user_confirm_request(hdev, &ev->bdaddr, ACL_LINK, 0, ev->passkey,
				  confirm_hint);

unlock:
	hci_dev_unlock(hdev);
}

static void hci_user_passkey_request_evt(struct hci_dev *hdev,
					 struct sk_buff *skb)
{
	struct hci_ev_user_passkey_req *ev = (void *) skb->data;

	BT_DBG("%s", hdev->name);

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		mgmt_user_passkey_request(hdev, &ev->bdaddr, ACL_LINK, 0);
}

static void hci_user_passkey_notify_evt(struct hci_dev *hdev,
					struct sk_buff *skb)
{
	struct hci_ev_user_passkey_notify *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s", hdev->name);

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &ev->bdaddr);
	if (!conn)
		return;

	conn->passkey_notify = __le32_to_cpu(ev->passkey);
	conn->passkey_entered = 0;

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		mgmt_user_passkey_notify(hdev, &conn->dst, conn->type,
					 conn->dst_type, conn->passkey_notify,
					 conn->passkey_entered);
}

static void hci_keypress_notify_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_keypress_notify *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s", hdev->name);

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &ev->bdaddr);
	if (!conn)
		return;

	switch (ev->type) {
	case HCI_KEYPRESS_STARTED:
		conn->passkey_entered = 0;
		return;

	case HCI_KEYPRESS_ENTERED:
		conn->passkey_entered++;
		break;

	case HCI_KEYPRESS_ERASED:
		conn->passkey_entered--;
		break;

	case HCI_KEYPRESS_CLEARED:
		conn->passkey_entered = 0;
		break;

	case HCI_KEYPRESS_COMPLETED:
		return;
	}

	if (test_bit(HCI_MGMT, &hdev->dev_flags))
		mgmt_user_passkey_notify(hdev, &conn->dst, conn->type,
					 conn->dst_type, conn->passkey_notify,
					 conn->passkey_entered);
}

static void hci_simple_pair_complete_evt(struct hci_dev *hdev,
					 struct sk_buff *skb)
{
	struct hci_ev_simple_pair_complete *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);
>>>>>>> refs/remotes/origin/master

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &ev->bdaddr);
	if (!conn)
		goto unlock;

	/* To avoid duplicate auth_failed events to user space we check
	 * the HCI_CONN_AUTH_PEND flag which will be set if we
	 * initiated the authentication. A traditional auth_complete
	 * event gets always produced as initiator and is also mapped to
	 * the mgmt_auth_failed event */
<<<<<<< HEAD
	if (!test_bit(HCI_CONN_AUTH_PEND, &conn->pend) && ev->status != 0)
		mgmt_auth_failed(hdev->id, &conn->dst, ev->status);

	hci_conn_put(conn);
=======
	if (!test_bit(HCI_CONN_AUTH_PEND, &conn->flags) && ev->status)
		mgmt_auth_failed(hdev, &conn->dst, conn->type, conn->dst_type,
				 ev->status);

	hci_conn_drop(conn);
>>>>>>> refs/remotes/origin/master

unlock:
	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_remote_host_features_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_remote_host_features *ev = (void *) skb->data;
	struct inquiry_entry *ie;
=======
static void hci_remote_host_features_evt(struct hci_dev *hdev,
					 struct sk_buff *skb)
{
	struct hci_ev_remote_host_features *ev = (void *) skb->data;
	struct inquiry_entry *ie;
	struct hci_conn *conn;
>>>>>>> refs/remotes/origin/master

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

<<<<<<< HEAD
	ie = hci_inquiry_cache_lookup(hdev, &ev->bdaddr);
	if (ie)
		ie->data.ssp_mode = (ev->features[0] & 0x01);
=======
	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &ev->bdaddr);
	if (conn)
		memcpy(conn->features[1], ev->features, 8);

	ie = hci_inquiry_cache_lookup(hdev, &ev->bdaddr);
	if (ie)
		ie->data.ssp_mode = (ev->features[0] & LMP_HOST_SSP);
>>>>>>> refs/remotes/origin/master

	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_remote_oob_data_request_evt(struct hci_dev *hdev,
							struct sk_buff *skb)
=======
static void hci_remote_oob_data_request_evt(struct hci_dev *hdev,
					    struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_remote_oob_data_request *ev = (void *) skb->data;
	struct oob_data *data;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

<<<<<<< HEAD
	if (!test_bit(HCI_MGMT, &hdev->flags))
=======
	if (!test_bit(HCI_MGMT, &hdev->dev_flags))
>>>>>>> refs/remotes/origin/master
		goto unlock;

	data = hci_find_remote_oob_data(hdev, &ev->bdaddr);
	if (data) {
		struct hci_cp_remote_oob_data_reply cp;

		bacpy(&cp.bdaddr, &ev->bdaddr);
		memcpy(cp.hash, data->hash, sizeof(cp.hash));
		memcpy(cp.randomizer, data->randomizer, sizeof(cp.randomizer));

		hci_send_cmd(hdev, HCI_OP_REMOTE_OOB_DATA_REPLY, sizeof(cp),
<<<<<<< HEAD
									&cp);
=======
			     &cp);
>>>>>>> refs/remotes/origin/master
	} else {
		struct hci_cp_remote_oob_data_neg_reply cp;

		bacpy(&cp.bdaddr, &ev->bdaddr);
		hci_send_cmd(hdev, HCI_OP_REMOTE_OOB_DATA_NEG_REPLY, sizeof(cp),
<<<<<<< HEAD
									&cp);
=======
			     &cp);
>>>>>>> refs/remotes/origin/master
	}

unlock:
	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_le_conn_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_le_conn_complete *ev = (void *) skb->data;
	struct hci_conn *conn;
<<<<<<< HEAD
	u8 white_list;
=======
>>>>>>> refs/remotes/origin/cm-10.0

	BT_DBG("%s status %d", hdev->name, ev->status);

	hci_dev_lock(hdev);

<<<<<<< HEAD
	/* Ignore event for LE cancel create conn whitelist */
	if (ev->status && !bacmp(&ev->bdaddr, BDADDR_ANY))
		goto unlock;

	if (hci_conn_hash_lookup_ba(hdev, LE_LINK, BDADDR_ANY))
		white_list = 1;
	else
		white_list = 0;

	BT_DBG("w_list %d", white_list);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &ev->bdaddr);
	if (!conn) {
		conn = hci_le_conn_add(hdev, &ev->bdaddr, ev->bdaddr_type);
		if (!conn) {
			BT_ERR("No memory for new connection");
			hci_dev_unlock(hdev);
			return;
=======
static void hci_phy_link_complete_evt(struct hci_dev *hdev,
				      struct sk_buff *skb)
{
	struct hci_ev_phy_link_complete *ev = (void *) skb->data;
	struct hci_conn *hcon, *bredr_hcon;

	BT_DBG("%s handle 0x%2.2x status 0x%2.2x", hdev->name, ev->phy_handle,
	       ev->status);

	hci_dev_lock(hdev);

	hcon = hci_conn_hash_lookup_handle(hdev, ev->phy_handle);
	if (!hcon) {
		hci_dev_unlock(hdev);
		return;
	}

	if (ev->status) {
		hci_conn_del(hcon);
		hci_dev_unlock(hdev);
		return;
	}

	bredr_hcon = hcon->amp_mgr->l2cap_conn->hcon;

	hcon->state = BT_CONNECTED;
	bacpy(&hcon->dst, &bredr_hcon->dst);

	hci_conn_hold(hcon);
	hcon->disc_timeout = HCI_DISCONN_TIMEOUT;
	hci_conn_drop(hcon);

	hci_conn_add_sysfs(hcon);

	amp_physical_cfm(bredr_hcon, hcon);

	hci_dev_unlock(hdev);
}

static void hci_loglink_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_logical_link_complete *ev = (void *) skb->data;
	struct hci_conn *hcon;
	struct hci_chan *hchan;
	struct amp_mgr *mgr;

	BT_DBG("%s log_handle 0x%4.4x phy_handle 0x%2.2x status 0x%2.2x",
	       hdev->name, le16_to_cpu(ev->handle), ev->phy_handle,
	       ev->status);

	hcon = hci_conn_hash_lookup_handle(hdev, ev->phy_handle);
	if (!hcon)
		return;

	/* Create AMP hchan */
	hchan = hci_chan_create(hcon);
	if (!hchan)
		return;

	hchan->handle = le16_to_cpu(ev->handle);

	BT_DBG("hcon %p mgr %p hchan %p", hcon, hcon->amp_mgr, hchan);

	mgr = hcon->amp_mgr;
	if (mgr && mgr->bredr_chan) {
		struct l2cap_chan *bredr_chan = mgr->bredr_chan;

		l2cap_chan_lock(bredr_chan);

		bredr_chan->conn->mtu = hdev->block_mtu;
		l2cap_logical_cfm(bredr_chan, hchan, 0);
		hci_conn_hold(hcon);

		l2cap_chan_unlock(bredr_chan);
	}
}

static void hci_disconn_loglink_complete_evt(struct hci_dev *hdev,
					     struct sk_buff *skb)
{
	struct hci_ev_disconn_logical_link_complete *ev = (void *) skb->data;
	struct hci_chan *hchan;

	BT_DBG("%s log handle 0x%4.4x status 0x%2.2x", hdev->name,
	       le16_to_cpu(ev->handle), ev->status);

	if (ev->status)
		return;

	hci_dev_lock(hdev);

	hchan = hci_chan_lookup_handle(hdev, le16_to_cpu(ev->handle));
	if (!hchan)
		goto unlock;

	amp_destroy_logical_link(hchan, ev->reason);

unlock:
	hci_dev_unlock(hdev);
}

static void hci_disconn_phylink_complete_evt(struct hci_dev *hdev,
					     struct sk_buff *skb)
{
	struct hci_ev_disconn_phy_link_complete *ev = (void *) skb->data;
	struct hci_conn *hcon;

	BT_DBG("%s status 0x%2.2x", hdev->name, ev->status);

	if (ev->status)
		return;

	hci_dev_lock(hdev);

	hcon = hci_conn_hash_lookup_handle(hdev, ev->phy_handle);
	if (hcon) {
		hcon->state = BT_CLOSED;
		hci_conn_del(hcon);
	}

	hci_dev_unlock(hdev);
}

static void hci_le_conn_complete_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_le_conn_complete *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s status 0x%2.2x", hdev->name, ev->status);

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_state(hdev, LE_LINK, BT_CONNECT);
	if (!conn) {
		conn = hci_conn_add(hdev, LE_LINK, &ev->bdaddr);
		if (!conn) {
			BT_ERR("No memory for new connection");
			goto unlock;
		}

		conn->dst_type = ev->bdaddr_type;

		/* The advertising parameters for own address type
		 * define which source address and source address
		 * type this connections has.
		 */
		if (bacmp(&conn->src, BDADDR_ANY)) {
			conn->src_type = ADDR_LE_DEV_PUBLIC;
		} else {
			bacpy(&conn->src, &hdev->static_addr);
			conn->src_type = ADDR_LE_DEV_RANDOM;
		}

		if (ev->role == LE_CONN_ROLE_MASTER) {
			conn->out = true;
			conn->link_mode |= HCI_LM_MASTER;
>>>>>>> refs/remotes/origin/master
		}
	}

	if (ev->status) {
<<<<<<< HEAD
=======
		mgmt_connect_failed(hdev, &conn->dst, conn->type,
				    conn->dst_type, ev->status);
>>>>>>> refs/remotes/origin/master
		hci_proto_connect_cfm(conn, ev->status);
		conn->state = BT_CLOSED;
		hci_conn_del(conn);
		goto unlock;
	}

<<<<<<< HEAD
	conn->sec_level = BT_SECURITY_LOW;
	conn->handle = __le16_to_cpu(ev->handle);
	conn->state = BT_CONNECTED;
	conn->disc_timeout = HCI_DISCONN_TIMEOUT;
	mgmt_connected(hdev->id, &ev->bdaddr, 1);
	mgmt_le_conn_params(hdev->id, &ev->bdaddr,
			__le16_to_cpu(ev->interval),
			__le16_to_cpu(ev->latency),
			__le16_to_cpu(ev->supervision_timeout));

	hci_conn_hold(conn);
	hci_conn_hold_device(conn);
	hci_conn_add_sysfs(conn);

<<<<<<< HEAD
	if (!white_list)
		hci_proto_connect_cfm(conn, ev->status);
=======
	hci_proto_connect_cfm(conn, ev->status);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!test_and_set_bit(HCI_CONN_MGMT_CONNECTED, &conn->flags))
		mgmt_device_connected(hdev, &ev->bdaddr, conn->type,
				      conn->dst_type, 0, NULL, 0, NULL);

	conn->sec_level = BT_SECURITY_LOW;
	conn->handle = __le16_to_cpu(ev->handle);
	conn->state = BT_CONNECTED;

	hci_conn_add_sysfs(conn);

	hci_proto_connect_cfm(conn, ev->status);
>>>>>>> refs/remotes/origin/master

unlock:
	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_le_conn_update_complete_evt(struct hci_dev *hdev,
							struct sk_buff *skb)
{
	struct hci_ev_le_conn_update_complete *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s status %d", hdev->name, ev->status);

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev,
				__le16_to_cpu(ev->handle));
	if (conn == NULL) {
		BT_ERR("Unknown connection update");
		goto unlock;
	}

	if (ev->status) {
		BT_ERR("Connection update unsuccessful");
		goto unlock;
	}

	mgmt_le_conn_params(hdev->id, &conn->dst,
			__le16_to_cpu(ev->interval),
			__le16_to_cpu(ev->latency),
			__le16_to_cpu(ev->supervision_timeout));

unlock:
	hci_dev_unlock(hdev);
}

static inline void hci_le_ltk_request_evt(struct hci_dev *hdev,
						struct sk_buff *skb)
=======
static void hci_le_adv_report_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	u8 num_reports = skb->data[0];
	void *ptr = &skb->data[1];
	s8 rssi;

	while (num_reports--) {
		struct hci_ev_le_advertising_info *ev = ptr;

		rssi = ev->data[ev->length];
		mgmt_device_found(hdev, &ev->bdaddr, LE_LINK, ev->bdaddr_type,
				  NULL, rssi, 0, 1, ev->data, ev->length);

		ptr += sizeof(*ev) + ev->length + 1;
	}
}

static void hci_le_ltk_request_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_le_ltk_req *ev = (void *) skb->data;
	struct hci_cp_le_ltk_reply cp;
	struct hci_cp_le_ltk_neg_reply neg;
	struct hci_conn *conn;
<<<<<<< HEAD
	struct link_key *ltk;

	BT_DBG("%s handle %d", hdev->name, cpu_to_le16(ev->handle));
=======
	struct smp_ltk *ltk;

	BT_DBG("%s handle 0x%4.4x", hdev->name, __le16_to_cpu(ev->handle));
>>>>>>> refs/remotes/origin/master

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, __le16_to_cpu(ev->handle));
	if (conn == NULL)
		goto not_found;

	ltk = hci_find_ltk(hdev, ev->ediv, ev->random);
	if (ltk == NULL)
		goto not_found;

	memcpy(cp.ltk, ltk->val, sizeof(ltk->val));
	cp.handle = cpu_to_le16(conn->handle);
<<<<<<< HEAD
	conn->pin_length = ltk->pin_len;

	hci_send_cmd(hdev, HCI_OP_LE_LTK_REPLY, sizeof(cp), &cp);

=======

	if (ltk->authenticated)
		conn->pending_sec_level = BT_SECURITY_HIGH;
	else
		conn->pending_sec_level = BT_SECURITY_MEDIUM;

	conn->enc_key_size = ltk->enc_size;

	hci_send_cmd(hdev, HCI_OP_LE_LTK_REPLY, sizeof(cp), &cp);

	if (ltk->type & HCI_SMP_STK) {
		list_del(&ltk->list);
		kfree(ltk);
	}

>>>>>>> refs/remotes/origin/master
	hci_dev_unlock(hdev);

	return;

not_found:
	neg.handle = ev->handle;
	hci_send_cmd(hdev, HCI_OP_LE_LTK_NEG_REPLY, sizeof(neg), &neg);
	hci_dev_unlock(hdev);
}

<<<<<<< HEAD
static inline void hci_le_adv_report_evt(struct hci_dev *hdev,
						struct sk_buff *skb)
{
	struct hci_ev_le_advertising_info *ev;
	u8 num_reports;

	num_reports = skb->data[0];
	ev = (void *) &skb->data[1];

	hci_dev_lock(hdev);

	while (num_reports--) {
		mgmt_device_found(hdev->id, &ev->bdaddr, ev->bdaddr_type,
				1, NULL, 0, ev->length, ev->data);
		hci_add_adv_entry(hdev, ev);
		ev = (void *) (ev->data + ev->length + 1);
	}

	hci_dev_unlock(hdev);
}

static inline void hci_le_meta_evt(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_le_meta_evt(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_ev_le_meta *le_ev = (void *) skb->data;

	skb_pull(skb, sizeof(*le_ev));

	switch (le_ev->subevent) {
	case HCI_EV_LE_CONN_COMPLETE:
		hci_le_conn_complete_evt(hdev, skb);
		break;

<<<<<<< HEAD
	case HCI_EV_LE_CONN_UPDATE_COMPLETE:
		hci_le_conn_update_complete_evt(hdev, skb);
=======
	case HCI_EV_LE_ADVERTISING_REPORT:
		hci_le_adv_report_evt(hdev, skb);
>>>>>>> refs/remotes/origin/master
		break;

	case HCI_EV_LE_LTK_REQ:
		hci_le_ltk_request_evt(hdev, skb);
		break;

<<<<<<< HEAD
	case HCI_EV_LE_ADVERTISING_REPORT:
		hci_le_adv_report_evt(hdev, skb);
		break;

=======
>>>>>>> refs/remotes/origin/master
	default:
		break;
	}
}

<<<<<<< HEAD
static inline void hci_phy_link_complete(struct hci_dev *hdev,
					struct sk_buff *skb)
{
	struct hci_ev_phys_link_complete *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s handle %d status %d", hdev->name, ev->phy_handle,
		ev->status);

	hci_dev_lock(hdev);

	if (ev->status == 0) {
		conn = hci_conn_add(hdev, ACL_LINK, 0, BDADDR_ANY);
		if (conn) {
			conn->handle = ev->phy_handle;
			conn->state = BT_CONNECTED;

			hci_conn_hold(conn);
			conn->disc_timeout = HCI_DISCONN_TIMEOUT/2;
			hci_conn_put(conn);

			hci_conn_hold_device(conn);
			hci_conn_add_sysfs(conn);
		} else
			BT_ERR("No memory for new connection");
	}

	hci_dev_unlock(hdev);
}

static inline void hci_log_link_complete(struct hci_dev *hdev,
					struct sk_buff *skb)
{
	struct hci_ev_log_link_complete *ev = (void *) skb->data;
	struct hci_chan *chan;

	BT_DBG("%s handle %d status %d", hdev->name,
		__le16_to_cpu(ev->log_handle), ev->status);

	hci_dev_lock(hdev);

	chan = hci_chan_list_lookup_id(hdev, ev->phy_handle);

	if (chan) {
		if (ev->status == 0) {
			chan->ll_handle = __le16_to_cpu(ev->log_handle);
			chan->state = BT_CONNECTED;
		} else {
			chan->state = BT_CLOSED;
		}

		hci_proto_create_cfm(chan, ev->status);
	}

	hci_dev_unlock(hdev);
}

static inline void hci_flow_spec_modify_complete(struct hci_dev *hdev,
					struct sk_buff *skb)
{
	struct hci_ev_flow_spec_modify_complete *ev = (void *) skb->data;
	struct hci_chan *chan;

	BT_DBG("%s handle %d status %d", hdev->name,
		__le16_to_cpu(ev->log_handle), ev->status);

	hci_dev_lock(hdev);

	chan = hci_chan_list_lookup_handle(hdev, ev->log_handle);
	if (chan)
		hci_proto_modify_cfm(chan, ev->status);

	hci_dev_unlock(hdev);
}

static inline void hci_disconn_log_link_complete_evt(struct hci_dev *hdev,
						struct sk_buff *skb)
{
	struct hci_ev_disconn_log_link_complete *ev = (void *) skb->data;
	struct hci_chan *chan;

	BT_DBG("%s handle %d status %d", hdev->name,
		__le16_to_cpu(ev->log_handle), ev->status);

	if (ev->status)
		return;

	hci_dev_lock(hdev);

	chan = hci_chan_list_lookup_handle(hdev, __le16_to_cpu(ev->log_handle));
	if (chan)
		hci_proto_destroy_cfm(chan, ev->reason);

	hci_dev_unlock(hdev);
}

static inline void hci_disconn_phy_link_complete_evt(struct hci_dev *hdev,
						struct sk_buff *skb)
{
	struct hci_ev_disconn_phys_link_complete *ev = (void *) skb->data;
	struct hci_conn *conn;

	BT_DBG("%s status %d", hdev->name, ev->status);

	if (ev->status)
		return;

	hci_dev_lock(hdev);

	conn = hci_conn_hash_lookup_handle(hdev, ev->phy_handle);
	if (conn) {
		conn->state = BT_CLOSED;

		hci_proto_disconn_cfm(conn, ev->reason, 0);
		hci_conn_del(conn);
	}

	hci_dev_unlock(hdev);
}

void hci_event_packet(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_event_hdr *hdr = (void *) skb->data;
	__u8 event = hdr->evt;

	BT_DBG("");

	skb_pull(skb, HCI_EVENT_HDR_SIZE);
=======
static void hci_chan_selected_evt(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_ev_channel_selected *ev = (void *) skb->data;
	struct hci_conn *hcon;

	BT_DBG("%s handle 0x%2.2x", hdev->name, ev->phy_handle);

	skb_pull(skb, sizeof(*ev));

	hcon = hci_conn_hash_lookup_handle(hdev, ev->phy_handle);
	if (!hcon)
		return;

	amp_read_loc_assoc_final_data(hdev, hcon);
}

void hci_event_packet(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct hci_event_hdr *hdr = (void *) skb->data;
	__u8 event = hdr->evt;

	hci_dev_lock(hdev);

	/* Received events are (currently) only needed when a request is
	 * ongoing so avoid unnecessary memory allocation.
	 */
	if (hdev->req_status == HCI_REQ_PEND) {
		kfree_skb(hdev->recv_evt);
		hdev->recv_evt = skb_clone(skb, GFP_KERNEL);
	}

	hci_dev_unlock(hdev);

	skb_pull(skb, HCI_EVENT_HDR_SIZE);

	if (hdev->sent_cmd && bt_cb(hdev->sent_cmd)->req.event == event) {
		struct hci_command_hdr *cmd_hdr = (void *) hdev->sent_cmd->data;
		u16 opcode = __le16_to_cpu(cmd_hdr->opcode);

		hci_req_cmd_complete(hdev, opcode, 0);
	}
>>>>>>> refs/remotes/origin/master

	switch (event) {
	case HCI_EV_INQUIRY_COMPLETE:
		hci_inquiry_complete_evt(hdev, skb);
		break;

	case HCI_EV_INQUIRY_RESULT:
		hci_inquiry_result_evt(hdev, skb);
		break;

	case HCI_EV_CONN_COMPLETE:
		hci_conn_complete_evt(hdev, skb);
		break;

	case HCI_EV_CONN_REQUEST:
		hci_conn_request_evt(hdev, skb);
		break;

	case HCI_EV_DISCONN_COMPLETE:
		hci_disconn_complete_evt(hdev, skb);
		break;

	case HCI_EV_AUTH_COMPLETE:
		hci_auth_complete_evt(hdev, skb);
		break;

	case HCI_EV_REMOTE_NAME:
		hci_remote_name_evt(hdev, skb);
		break;

	case HCI_EV_ENCRYPT_CHANGE:
		hci_encrypt_change_evt(hdev, skb);
		break;

	case HCI_EV_CHANGE_LINK_KEY_COMPLETE:
		hci_change_link_key_complete_evt(hdev, skb);
		break;

	case HCI_EV_REMOTE_FEATURES:
		hci_remote_features_evt(hdev, skb);
		break;

<<<<<<< HEAD
	case HCI_EV_REMOTE_VERSION:
		hci_remote_version_evt(hdev, skb);
		break;

	case HCI_EV_QOS_SETUP_COMPLETE:
		hci_qos_setup_complete_evt(hdev, skb);
		break;

=======
>>>>>>> refs/remotes/origin/master
	case HCI_EV_CMD_COMPLETE:
		hci_cmd_complete_evt(hdev, skb);
		break;

	case HCI_EV_CMD_STATUS:
		hci_cmd_status_evt(hdev, skb);
		break;

<<<<<<< HEAD
<<<<<<< HEAD
	case HCI_EV_HARDWARE_ERROR:
		hci_hardware_error_evt(hdev, skb);
		break;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	case HCI_EV_ROLE_CHANGE:
		hci_role_change_evt(hdev, skb);
		break;

	case HCI_EV_NUM_COMP_PKTS:
		hci_num_comp_pkts_evt(hdev, skb);
		break;

	case HCI_EV_MODE_CHANGE:
		hci_mode_change_evt(hdev, skb);
		break;

	case HCI_EV_PIN_CODE_REQ:
		hci_pin_code_request_evt(hdev, skb);
		break;

	case HCI_EV_LINK_KEY_REQ:
		hci_link_key_request_evt(hdev, skb);
		break;

	case HCI_EV_LINK_KEY_NOTIFY:
		hci_link_key_notify_evt(hdev, skb);
		break;

	case HCI_EV_CLOCK_OFFSET:
		hci_clock_offset_evt(hdev, skb);
		break;

	case HCI_EV_PKT_TYPE_CHANGE:
		hci_pkt_type_change_evt(hdev, skb);
		break;

	case HCI_EV_PSCAN_REP_MODE:
		hci_pscan_rep_mode_evt(hdev, skb);
		break;

	case HCI_EV_INQUIRY_RESULT_WITH_RSSI:
		hci_inquiry_result_with_rssi_evt(hdev, skb);
		break;

	case HCI_EV_REMOTE_EXT_FEATURES:
		hci_remote_ext_features_evt(hdev, skb);
		break;

	case HCI_EV_SYNC_CONN_COMPLETE:
		hci_sync_conn_complete_evt(hdev, skb);
		break;

<<<<<<< HEAD
	case HCI_EV_SYNC_CONN_CHANGED:
		hci_sync_conn_changed_evt(hdev, skb);
		break;

	case HCI_EV_SNIFF_SUBRATE:
		hci_sniff_subrate_evt(hdev, skb);
		break;

=======
>>>>>>> refs/remotes/origin/master
	case HCI_EV_EXTENDED_INQUIRY_RESULT:
		hci_extended_inquiry_result_evt(hdev, skb);
		break;

<<<<<<< HEAD
=======
	case HCI_EV_KEY_REFRESH_COMPLETE:
		hci_key_refresh_complete_evt(hdev, skb);
		break;

>>>>>>> refs/remotes/origin/master
	case HCI_EV_IO_CAPA_REQUEST:
		hci_io_capa_request_evt(hdev, skb);
		break;

	case HCI_EV_IO_CAPA_REPLY:
		hci_io_capa_reply_evt(hdev, skb);
		break;

<<<<<<< HEAD
	case HCI_EV_USER_PASSKEY_REQUEST:
	case HCI_EV_USER_PASSKEY_NOTIFICATION:
	case HCI_EV_USER_CONFIRM_REQUEST:
		hci_user_ssp_confirmation_evt(hdev, event, skb);
=======
	case HCI_EV_USER_CONFIRM_REQUEST:
		hci_user_confirm_request_evt(hdev, skb);
		break;

	case HCI_EV_USER_PASSKEY_REQUEST:
		hci_user_passkey_request_evt(hdev, skb);
		break;

	case HCI_EV_USER_PASSKEY_NOTIFY:
		hci_user_passkey_notify_evt(hdev, skb);
		break;

	case HCI_EV_KEYPRESS_NOTIFY:
		hci_keypress_notify_evt(hdev, skb);
>>>>>>> refs/remotes/origin/master
		break;

	case HCI_EV_SIMPLE_PAIR_COMPLETE:
		hci_simple_pair_complete_evt(hdev, skb);
		break;

	case HCI_EV_REMOTE_HOST_FEATURES:
		hci_remote_host_features_evt(hdev, skb);
		break;

	case HCI_EV_LE_META:
		hci_le_meta_evt(hdev, skb);
		break;

<<<<<<< HEAD
	case HCI_EV_REMOTE_OOB_DATA_REQUEST:
		hci_remote_oob_data_request_evt(hdev, skb);
		break;

	case HCI_EV_PHYS_LINK_COMPLETE:
		hci_phy_link_complete(hdev, skb);
		hci_amp_event_packet(hdev, event, skb);
		break;

	case HCI_EV_LOG_LINK_COMPLETE:
		hci_log_link_complete(hdev, skb);
		break;

	case HCI_EV_FLOW_SPEC_MODIFY_COMPLETE:
		hci_flow_spec_modify_complete(hdev, skb);
		break;

	case HCI_EV_DISCONN_LOG_LINK_COMPLETE:
		hci_disconn_log_link_complete_evt(hdev, skb);
		break;

	case HCI_EV_DISCONN_PHYS_LINK_COMPLETE:
		hci_disconn_phy_link_complete_evt(hdev, skb);
		hci_amp_event_packet(hdev, event, skb);
=======
	case HCI_EV_CHANNEL_SELECTED:
		hci_chan_selected_evt(hdev, skb);
		break;

	case HCI_EV_REMOTE_OOB_DATA_REQUEST:
		hci_remote_oob_data_request_evt(hdev, skb);
		break;

	case HCI_EV_PHY_LINK_COMPLETE:
		hci_phy_link_complete_evt(hdev, skb);
		break;

	case HCI_EV_LOGICAL_LINK_COMPLETE:
		hci_loglink_complete_evt(hdev, skb);
		break;

	case HCI_EV_DISCONN_LOGICAL_LINK_COMPLETE:
		hci_disconn_loglink_complete_evt(hdev, skb);
		break;

	case HCI_EV_DISCONN_PHY_LINK_COMPLETE:
		hci_disconn_phylink_complete_evt(hdev, skb);
>>>>>>> refs/remotes/origin/master
		break;

	case HCI_EV_NUM_COMP_BLOCKS:
		hci_num_comp_blocks_evt(hdev, skb);
		break;

<<<<<<< HEAD
	case HCI_EV_CHANNEL_SELECTED:
		hci_amp_event_packet(hdev, event, skb);
		break;

	case HCI_EV_AMP_STATUS_CHANGE:
		hci_amp_event_packet(hdev, event, skb);
		break;

	default:
		BT_DBG("%s event 0x%x", hdev->name, event);
=======
	default:
		BT_DBG("%s event 0x%2.2x", hdev->name, event);
>>>>>>> refs/remotes/origin/master
		break;
	}

	kfree_skb(skb);
	hdev->stat.evt_rx++;
}
<<<<<<< HEAD

/* Generate internal stack event */
void hci_si_event(struct hci_dev *hdev, int type, int dlen, void *data)
{
	struct hci_event_hdr *hdr;
	struct hci_ev_stack_internal *ev;
	struct sk_buff *skb;

	skb = bt_skb_alloc(HCI_EVENT_HDR_SIZE + sizeof(*ev) + dlen, GFP_ATOMIC);
	if (!skb)
		return;

	hdr = (void *) skb_put(skb, HCI_EVENT_HDR_SIZE);
	hdr->evt  = HCI_EV_STACK_INTERNAL;
	hdr->plen = sizeof(*ev) + dlen;

	ev  = (void *) skb_put(skb, sizeof(*ev) + dlen);
	ev->type = type;
	memcpy(ev->data, data, dlen);

	bt_cb(skb)->incoming = 1;
	__net_timestamp(skb);

	bt_cb(skb)->pkt_type = HCI_EVENT_PKT;
	skb->dev = (void *) hdev;
	hci_send_to_sock(hdev, skb, NULL);
	kfree_skb(skb);
}
=======
>>>>>>> refs/remotes/origin/master
