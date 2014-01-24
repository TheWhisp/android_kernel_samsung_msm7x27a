/*
   BlueZ - Bluetooth protocol stack for Linux
<<<<<<< HEAD
<<<<<<< HEAD
   Copyright (c) 2000-2001, 2010-2012 The Linux Foundation.  All rights reserved.
=======
   Copyright (C) 2000-2001 Qualcomm Incorporated
   Copyright (C) 2011 ProFUSION Embedded Systems
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

/* Bluetooth HCI core. */

<<<<<<< HEAD
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/kmod.h>

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/notifier.h>
#include <linux/rfkill.h>
#include <linux/timer.h>
#include <linux/crypto.h>
#include <net/sock.h>

#include <asm/system.h>
#include <linux/uaccess.h>
=======
#include <linux/export.h>
#include <linux/idr.h>
#include <linux/rfkill.h>
#include <linux/debugfs.h>
>>>>>>> refs/remotes/origin/master
#include <asm/unaligned.h>

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>

<<<<<<< HEAD
#define AUTO_OFF_TIMEOUT 2000

static void hci_cmd_task(unsigned long arg);
static void hci_rx_task(unsigned long arg);
static void hci_tx_task(unsigned long arg);

static DEFINE_RWLOCK(hci_task_lock);

static bool enable_smp = 1;
<<<<<<< HEAD
=======
static void hci_rx_work(struct work_struct *work);
static void hci_cmd_work(struct work_struct *work);
static void hci_tx_work(struct work_struct *work);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

/* HCI device list */
LIST_HEAD(hci_dev_list);
DEFINE_RWLOCK(hci_dev_list_lock);

/* HCI callback list */
LIST_HEAD(hci_cb_list);
DEFINE_RWLOCK(hci_cb_list_lock);

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
/* AMP Manager event callbacks */
LIST_HEAD(amp_mgr_cb_list);
DEFINE_RWLOCK(amp_mgr_cb_list_lock);

/* HCI protocols */
#define HCI_MAX_PROTO	2
struct hci_proto *hci_proto[HCI_MAX_PROTO];

/* HCI notifiers list */
static ATOMIC_NOTIFIER_HEAD(hci_notifier);

/* ---- HCI notifications ---- */

int hci_register_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_register(&hci_notifier, nb);
}

int hci_unregister_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_unregister(&hci_notifier, nb);
}

static void hci_notify(struct hci_dev *hdev, int event)
{
	atomic_notifier_call_chain(&hci_notifier, event, hdev);
}

/* ---- HCI requests ---- */

void hci_req_complete(struct hci_dev *hdev, __u16 cmd, int result)
{
	BT_DBG("%s command 0x%04x result 0x%2.2x", hdev->name, cmd, result);

	/* If this is the init phase check if the completed command matches
	 * the last init command, and if not just return.
	 */
	if (test_bit(HCI_INIT, &hdev->flags) && hdev->init_last_cmd != cmd)
		return;

	if (hdev->req_status == HCI_REQ_PEND) {
		hdev->req_result = result;
		hdev->req_status = HCI_REQ_DONE;
		wake_up_interruptible(&hdev->req_wait_q);
	}
}

static void hci_req_cancel(struct hci_dev *hdev, int err)
{
	BT_DBG("%s err 0x%2.2x", hdev->name, err);

	if (hdev->req_status == HCI_REQ_PEND) {
		hdev->req_result = err;
		hdev->req_status = HCI_REQ_CANCELED;
		wake_up_interruptible(&hdev->req_wait_q);
	}
}

/* Execute request and wait for completion. */
static int __hci_request(struct hci_dev *hdev, void (*req)(struct hci_dev *hdev, unsigned long opt),
					unsigned long opt, __u32 timeout)
{
=======
/* HCI ID Numbering */
static DEFINE_IDA(hci_index_ida);

/* ---- HCI notifications ---- */

static void hci_notify(struct hci_dev *hdev, int event)
{
	hci_sock_dev_event(hdev, event);
}

/* ---- HCI debugfs entries ---- */

static ssize_t dut_mode_read(struct file *file, char __user *user_buf,
			     size_t count, loff_t *ppos)
{
	struct hci_dev *hdev = file->private_data;
	char buf[3];

	buf[0] = test_bit(HCI_DUT_MODE, &hdev->dev_flags) ? 'Y': 'N';
	buf[1] = '\n';
	buf[2] = '\0';
	return simple_read_from_buffer(user_buf, count, ppos, buf, 2);
}

static ssize_t dut_mode_write(struct file *file, const char __user *user_buf,
			      size_t count, loff_t *ppos)
{
	struct hci_dev *hdev = file->private_data;
	struct sk_buff *skb;
	char buf[32];
	size_t buf_size = min(count, (sizeof(buf)-1));
	bool enable;
	int err;

<<<<<<< HEAD
	if (!test_bit(HCI_UP, &hdev->flags))
		return -ENETDOWN;
=======
	switch (hdev->req_status) {
	case HCI_REQ_DONE:
		err = -bt_err(hdev->req_result);
		break;
>>>>>>> refs/remotes/origin/cm-11.0

	if (copy_from_user(buf, user_buf, buf_size))
		return -EFAULT;

	buf[buf_size] = '\0';
	if (strtobool(buf, &enable))
		return -EINVAL;

	if (enable == test_bit(HCI_DUT_MODE, &hdev->dev_flags))
		return -EALREADY;

	hci_req_lock(hdev);
	if (enable)
		skb = __hci_cmd_sync(hdev, HCI_OP_ENABLE_DUT_MODE, 0, NULL,
				     HCI_CMD_TIMEOUT);
	else
		skb = __hci_cmd_sync(hdev, HCI_OP_RESET, 0, NULL,
				     HCI_CMD_TIMEOUT);
	hci_req_unlock(hdev);

	if (IS_ERR(skb))
		return PTR_ERR(skb);

	err = -bt_to_errno(skb->data[0]);
	kfree_skb(skb);

	if (err < 0)
		return err;

	change_bit(HCI_DUT_MODE, &hdev->dev_flags);

	return count;
}

static const struct file_operations dut_mode_fops = {
	.open		= simple_open,
	.read		= dut_mode_read,
	.write		= dut_mode_write,
	.llseek		= default_llseek,
};

static int features_show(struct seq_file *f, void *ptr)
{
	struct hci_dev *hdev = f->private;
	u8 p;

	hci_dev_lock(hdev);
	for (p = 0; p < HCI_MAX_PAGES && p <= hdev->max_page; p++) {
		seq_printf(f, "%2u: 0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x "
			   "0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x\n", p,
			   hdev->features[p][0], hdev->features[p][1],
			   hdev->features[p][2], hdev->features[p][3],
			   hdev->features[p][4], hdev->features[p][5],
			   hdev->features[p][6], hdev->features[p][7]);
	}
	if (lmp_le_capable(hdev))
		seq_printf(f, "LE: 0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x "
			   "0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x\n",
			   hdev->le_features[0], hdev->le_features[1],
			   hdev->le_features[2], hdev->le_features[3],
			   hdev->le_features[4], hdev->le_features[5],
			   hdev->le_features[6], hdev->le_features[7]);
	hci_dev_unlock(hdev);

	return 0;
}

static int features_open(struct inode *inode, struct file *file)
{
	return single_open(file, features_show, inode->i_private);
}

static const struct file_operations features_fops = {
	.open		= features_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int blacklist_show(struct seq_file *f, void *p)
{
	struct hci_dev *hdev = f->private;
	struct bdaddr_list *b;

	hci_dev_lock(hdev);
	list_for_each_entry(b, &hdev->blacklist, list)
		seq_printf(f, "%pMR (type %u)\n", &b->bdaddr, b->bdaddr_type);
	hci_dev_unlock(hdev);

	return 0;
}

static int blacklist_open(struct inode *inode, struct file *file)
{
	return single_open(file, blacklist_show, inode->i_private);
}

static const struct file_operations blacklist_fops = {
	.open		= blacklist_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int uuids_show(struct seq_file *f, void *p)
{
	struct hci_dev *hdev = f->private;
	struct bt_uuid *uuid;

	hci_dev_lock(hdev);
	list_for_each_entry(uuid, &hdev->uuids, list) {
		u8 i, val[16];

		/* The Bluetooth UUID values are stored in big endian,
		 * but with reversed byte order. So convert them into
		 * the right order for the %pUb modifier.
		 */
		for (i = 0; i < 16; i++)
			val[i] = uuid->uuid[15 - i];

		seq_printf(f, "%pUb\n", val);
	}
	hci_dev_unlock(hdev);

	return 0;
}

static int uuids_open(struct inode *inode, struct file *file)
{
	return single_open(file, uuids_show, inode->i_private);
}

static const struct file_operations uuids_fops = {
	.open		= uuids_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int inquiry_cache_show(struct seq_file *f, void *p)
{
	struct hci_dev *hdev = f->private;
	struct discovery_state *cache = &hdev->discovery;
	struct inquiry_entry *e;

	hci_dev_lock(hdev);

	list_for_each_entry(e, &cache->all, all) {
		struct inquiry_data *data = &e->data;
		seq_printf(f, "%pMR %d %d %d 0x%.2x%.2x%.2x 0x%.4x %d %d %u\n",
			   &data->bdaddr,
			   data->pscan_rep_mode, data->pscan_period_mode,
			   data->pscan_mode, data->dev_class[2],
			   data->dev_class[1], data->dev_class[0],
			   __le16_to_cpu(data->clock_offset),
			   data->rssi, data->ssp_mode, e->timestamp);
	}

	hci_dev_unlock(hdev);

	return 0;
}

static int inquiry_cache_open(struct inode *inode, struct file *file)
{
	return single_open(file, inquiry_cache_show, inode->i_private);
}

static const struct file_operations inquiry_cache_fops = {
	.open		= inquiry_cache_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int link_keys_show(struct seq_file *f, void *ptr)
{
	struct hci_dev *hdev = f->private;
	struct list_head *p, *n;

	hci_dev_lock(hdev);
	list_for_each_safe(p, n, &hdev->link_keys) {
		struct link_key *key = list_entry(p, struct link_key, list);
		seq_printf(f, "%pMR %u %*phN %u\n", &key->bdaddr, key->type,
			   HCI_LINK_KEY_SIZE, key->val, key->pin_len);
	}
	hci_dev_unlock(hdev);

	return 0;
}

static int link_keys_open(struct inode *inode, struct file *file)
{
	return single_open(file, link_keys_show, inode->i_private);
}

static const struct file_operations link_keys_fops = {
	.open		= link_keys_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static ssize_t use_debug_keys_read(struct file *file, char __user *user_buf,
				   size_t count, loff_t *ppos)
{
	struct hci_dev *hdev = file->private_data;
	char buf[3];

	buf[0] = test_bit(HCI_DEBUG_KEYS, &hdev->dev_flags) ? 'Y': 'N';
	buf[1] = '\n';
	buf[2] = '\0';
	return simple_read_from_buffer(user_buf, count, ppos, buf, 2);
}

static const struct file_operations use_debug_keys_fops = {
	.open		= simple_open,
	.read		= use_debug_keys_read,
	.llseek		= default_llseek,
};

static int dev_class_show(struct seq_file *f, void *ptr)
{
	struct hci_dev *hdev = f->private;

	hci_dev_lock(hdev);
	seq_printf(f, "0x%.2x%.2x%.2x\n", hdev->dev_class[2],
		   hdev->dev_class[1], hdev->dev_class[0]);
	hci_dev_unlock(hdev);

	return 0;
}

static int dev_class_open(struct inode *inode, struct file *file)
{
	return single_open(file, dev_class_show, inode->i_private);
}

static const struct file_operations dev_class_fops = {
	.open		= dev_class_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int voice_setting_get(void *data, u64 *val)
{
	struct hci_dev *hdev = data;

	hci_dev_lock(hdev);
	*val = hdev->voice_setting;
	hci_dev_unlock(hdev);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(voice_setting_fops, voice_setting_get,
			NULL, "0x%4.4llx\n");

static int auto_accept_delay_set(void *data, u64 val)
{
	struct hci_dev *hdev = data;

	hci_dev_lock(hdev);
	hdev->auto_accept_delay = val;
	hci_dev_unlock(hdev);

	return 0;
}

static int auto_accept_delay_get(void *data, u64 *val)
{
	struct hci_dev *hdev = data;

	hci_dev_lock(hdev);
	*val = hdev->auto_accept_delay;
	hci_dev_unlock(hdev);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(auto_accept_delay_fops, auto_accept_delay_get,
			auto_accept_delay_set, "%llu\n");

static int ssp_debug_mode_set(void *data, u64 val)
{
	struct hci_dev *hdev = data;
	struct sk_buff *skb;
	__u8 mode;
	int err;

	if (val != 0 && val != 1)
		return -EINVAL;

	if (!test_bit(HCI_UP, &hdev->flags))
		return -ENETDOWN;

	hci_req_lock(hdev);
	mode = val;
	skb = __hci_cmd_sync(hdev, HCI_OP_WRITE_SSP_DEBUG_MODE, sizeof(mode),
			     &mode, HCI_CMD_TIMEOUT);
	hci_req_unlock(hdev);

	if (IS_ERR(skb))
		return PTR_ERR(skb);

	err = -bt_to_errno(skb->data[0]);
	kfree_skb(skb);

	if (err < 0)
		return err;

	hci_dev_lock(hdev);
	hdev->ssp_debug_mode = val;
	hci_dev_unlock(hdev);

	return 0;
}

static int ssp_debug_mode_get(void *data, u64 *val)
{
	struct hci_dev *hdev = data;

<<<<<<< HEAD
	hci_dev_lock(hdev);
	*val = hdev->ssp_debug_mode;
	hci_dev_unlock(hdev);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(ssp_debug_mode_fops, ssp_debug_mode_get,
			ssp_debug_mode_set, "%llu\n");

static int idle_timeout_set(void *data, u64 val)
{
	struct hci_dev *hdev = data;

	if (val != 0 && (val < 500 || val > 3600000))
		return -EINVAL;
=======
	/* Reset device */
	set_bit(HCI_RESET, &hdev->flags);
	memset(&hdev->features, 0, sizeof(hdev->features));
	hci_send_cmd(hdev, HCI_OP_RESET, 0, NULL);
}

static void hci_init_req(struct hci_dev *hdev, unsigned long opt)
{
	struct hci_cp_delete_stored_link_key cp;
	struct sk_buff *skb;
	__le16 param;
	__u8 flt_type;

	BT_DBG("%s %ld", hdev->name, opt);

	/* Driver initialization */

	/* Special commands */
	while ((skb = skb_dequeue(&hdev->driver_init))) {
		bt_cb(skb)->pkt_type = HCI_COMMAND_PKT;
		skb->dev = (void *) hdev;

		skb_queue_tail(&hdev->cmd_q, skb);
		tasklet_schedule(&hdev->cmd_task);
	}
	skb_queue_purge(&hdev->driver_init);
>>>>>>> refs/remotes/origin/cm-11.0

	hci_dev_lock(hdev);
	hdev->idle_timeout = val;
	hci_dev_unlock(hdev);

<<<<<<< HEAD
	return 0;
}

static int idle_timeout_get(void *data, u64 *val)
{
	struct hci_dev *hdev = data;

	hci_dev_lock(hdev);
	*val = hdev->idle_timeout;
	hci_dev_unlock(hdev);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(idle_timeout_fops, idle_timeout_get,
			idle_timeout_set, "%llu\n");

static int sniff_min_interval_set(void *data, u64 val)
{
	struct hci_dev *hdev = data;

	if (val == 0 || val % 2 || val > hdev->sniff_max_interval)
		return -EINVAL;

	hci_dev_lock(hdev);
	hdev->sniff_min_interval = val;
	hci_dev_unlock(hdev);

	return 0;
}

static int sniff_min_interval_get(void *data, u64 *val)
{
	struct hci_dev *hdev = data;

	hci_dev_lock(hdev);
	*val = hdev->sniff_min_interval;
	hci_dev_unlock(hdev);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(sniff_min_interval_fops, sniff_min_interval_get,
			sniff_min_interval_set, "%llu\n");

static int sniff_max_interval_set(void *data, u64 val)
{
	struct hci_dev *hdev = data;

	if (val == 0 || val % 2 || val < hdev->sniff_min_interval)
		return -EINVAL;

	hci_dev_lock(hdev);
	hdev->sniff_max_interval = val;
	hci_dev_unlock(hdev);

	return 0;
}

static int sniff_max_interval_get(void *data, u64 *val)
{
	struct hci_dev *hdev = data;

	hci_dev_lock(hdev);
	*val = hdev->sniff_max_interval;
	hci_dev_unlock(hdev);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(sniff_max_interval_fops, sniff_max_interval_get,
			sniff_max_interval_set, "%llu\n");

static int static_address_show(struct seq_file *f, void *p)
{
	struct hci_dev *hdev = f->private;

	hci_dev_lock(hdev);
	seq_printf(f, "%pMR\n", &hdev->static_addr);
	hci_dev_unlock(hdev);

	return 0;
}

static int static_address_open(struct inode *inode, struct file *file)
{
	return single_open(file, static_address_show, inode->i_private);
}

static const struct file_operations static_address_fops = {
	.open		= static_address_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int own_address_type_set(void *data, u64 val)
{
	struct hci_dev *hdev = data;

	if (val != 0 && val != 1)
		return -EINVAL;

	hci_dev_lock(hdev);
	hdev->own_addr_type = val;
	hci_dev_unlock(hdev);

	return 0;
}

static int own_address_type_get(void *data, u64 *val)
{
	struct hci_dev *hdev = data;

	hci_dev_lock(hdev);
	*val = hdev->own_addr_type;
	hci_dev_unlock(hdev);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(own_address_type_fops, own_address_type_get,
			own_address_type_set, "%llu\n");

static int long_term_keys_show(struct seq_file *f, void *ptr)
{
	struct hci_dev *hdev = f->private;
	struct list_head *p, *n;

	hci_dev_lock(hdev);
	list_for_each_safe(p, n, &hdev->link_keys) {
		struct smp_ltk *ltk = list_entry(p, struct smp_ltk, list);
		seq_printf(f, "%pMR (type %u) %u %u %u %.4x %*phN %*phN\\n",
			   &ltk->bdaddr, ltk->bdaddr_type, ltk->authenticated,
			   ltk->type, ltk->enc_size, __le16_to_cpu(ltk->ediv),
			   8, ltk->rand, 16, ltk->val);
	}
	hci_dev_unlock(hdev);

	return 0;
}

static int long_term_keys_open(struct inode *inode, struct file *file)
{
	return single_open(file, long_term_keys_show, inode->i_private);
}

static const struct file_operations long_term_keys_fops = {
	.open		= long_term_keys_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int conn_min_interval_set(void *data, u64 val)
{
	struct hci_dev *hdev = data;

	if (val < 0x0006 || val > 0x0c80 || val > hdev->le_conn_max_interval)
		return -EINVAL;

	hci_dev_lock(hdev);
	hdev->le_conn_min_interval = val;
	hci_dev_unlock(hdev);

	return 0;
}

static int conn_min_interval_get(void *data, u64 *val)
{
	struct hci_dev *hdev = data;

	hci_dev_lock(hdev);
	*val = hdev->le_conn_min_interval;
	hci_dev_unlock(hdev);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(conn_min_interval_fops, conn_min_interval_get,
			conn_min_interval_set, "%llu\n");

static int conn_max_interval_set(void *data, u64 val)
{
	struct hci_dev *hdev = data;

	if (val < 0x0006 || val > 0x0c80 || val < hdev->le_conn_min_interval)
		return -EINVAL;

	hci_dev_lock(hdev);
	hdev->le_conn_max_interval = val;
	hci_dev_unlock(hdev);

	return 0;
}

static int conn_max_interval_get(void *data, u64 *val)
{
	struct hci_dev *hdev = data;

	hci_dev_lock(hdev);
	*val = hdev->le_conn_max_interval;
	hci_dev_unlock(hdev);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(conn_max_interval_fops, conn_max_interval_get,
			conn_max_interval_set, "%llu\n");

/* ---- HCI requests ---- */

static void hci_req_sync_complete(struct hci_dev *hdev, u8 result)
{
	BT_DBG("%s result 0x%2.2x", hdev->name, result);

	if (hdev->req_status == HCI_REQ_PEND) {
		hdev->req_result = result;
		hdev->req_status = HCI_REQ_DONE;
		wake_up_interruptible(&hdev->req_wait_q);
	}
}

static void hci_req_cancel(struct hci_dev *hdev, int err)
{
	BT_DBG("%s err 0x%2.2x", hdev->name, err);

	if (hdev->req_status == HCI_REQ_PEND) {
		hdev->req_result = err;
		hdev->req_status = HCI_REQ_CANCELED;
		wake_up_interruptible(&hdev->req_wait_q);
	}
}

static struct sk_buff *hci_get_cmd_complete(struct hci_dev *hdev, u16 opcode,
					    u8 event)
{
	struct hci_ev_cmd_complete *ev;
	struct hci_event_hdr *hdr;
	struct sk_buff *skb;

	hci_dev_lock(hdev);

	skb = hdev->recv_evt;
	hdev->recv_evt = NULL;

	hci_dev_unlock(hdev);

	if (!skb)
		return ERR_PTR(-ENODATA);

	if (skb->len < sizeof(*hdr)) {
		BT_ERR("Too short HCI event");
		goto failed;
	}

	hdr = (void *) skb->data;
	skb_pull(skb, HCI_EVENT_HDR_SIZE);

	if (event) {
		if (hdr->evt != event)
			goto failed;
		return skb;
	}

	if (hdr->evt != HCI_EV_CMD_COMPLETE) {
		BT_DBG("Last event is not cmd complete (0x%2.2x)", hdr->evt);
		goto failed;
	}

	if (skb->len < sizeof(*ev)) {
		BT_ERR("Too short cmd_complete event");
		goto failed;
	}

	ev = (void *) skb->data;
	skb_pull(skb, sizeof(*ev));

	if (opcode == __le16_to_cpu(ev->opcode))
		return skb;

	BT_DBG("opcode doesn't match (0x%2.2x != 0x%2.2x)", opcode,
	       __le16_to_cpu(ev->opcode));

failed:
	kfree_skb(skb);
	return ERR_PTR(-ENODATA);
}

struct sk_buff *__hci_cmd_sync_ev(struct hci_dev *hdev, u16 opcode, u32 plen,
				  const void *param, u8 event, u32 timeout)
{
	DECLARE_WAITQUEUE(wait, current);
	struct hci_request req;
	int err = 0;

	BT_DBG("%s", hdev->name);

	hci_req_init(&req, hdev);

	hci_req_add_ev(&req, opcode, plen, param, event);

	hdev->req_status = HCI_REQ_PEND;

	err = hci_req_run(&req, hci_req_sync_complete);
	if (err < 0)
		return ERR_PTR(err);

	add_wait_queue(&hdev->req_wait_q, &wait);
	set_current_state(TASK_INTERRUPTIBLE);

	schedule_timeout(timeout);

	remove_wait_queue(&hdev->req_wait_q, &wait);

	if (signal_pending(current))
		return ERR_PTR(-EINTR);

	switch (hdev->req_status) {
	case HCI_REQ_DONE:
		err = -bt_to_errno(hdev->req_result);
		break;

	case HCI_REQ_CANCELED:
		err = -hdev->req_result;
		break;

	default:
		err = -ETIMEDOUT;
		break;
	}

	hdev->req_status = hdev->req_result = 0;

	BT_DBG("%s end: err %d", hdev->name, err);

	if (err < 0)
		return ERR_PTR(err);

	return hci_get_cmd_complete(hdev, opcode, event);
}
EXPORT_SYMBOL(__hci_cmd_sync_ev);

struct sk_buff *__hci_cmd_sync(struct hci_dev *hdev, u16 opcode, u32 plen,
			       const void *param, u32 timeout)
{
	return __hci_cmd_sync_ev(hdev, opcode, plen, param, 0, timeout);
}
EXPORT_SYMBOL(__hci_cmd_sync);

/* Execute request and wait for completion. */
static int __hci_req_sync(struct hci_dev *hdev,
			  void (*func)(struct hci_request *req,
				      unsigned long opt),
			  unsigned long opt, __u32 timeout)
{
	struct hci_request req;
>>>>>>> refs/remotes/origin/master
	DECLARE_WAITQUEUE(wait, current);
	int err = 0;

	BT_DBG("%s start", hdev->name);

<<<<<<< HEAD
	hdev->req_status = HCI_REQ_PEND;

	add_wait_queue(&hdev->req_wait_q, &wait);
	set_current_state(TASK_INTERRUPTIBLE);

	req(hdev, opt);
=======
	hci_req_init(&req, hdev);

	hdev->req_status = HCI_REQ_PEND;

	func(&req, opt);

	err = hci_req_run(&req, hci_req_sync_complete);
	if (err < 0) {
		hdev->req_status = 0;

		/* ENODATA means the HCI request command queue is empty.
		 * This can happen when a request with conditionals doesn't
		 * trigger any commands to be sent. This is normal behavior
		 * and should not trigger an error return.
		 */
		if (err == -ENODATA)
			return 0;

		return err;
	}

	add_wait_queue(&hdev->req_wait_q, &wait);
	set_current_state(TASK_INTERRUPTIBLE);

>>>>>>> refs/remotes/origin/master
	schedule_timeout(timeout);

	remove_wait_queue(&hdev->req_wait_q, &wait);

	if (signal_pending(current))
		return -EINTR;

	switch (hdev->req_status) {
	case HCI_REQ_DONE:
<<<<<<< HEAD
		err = -bt_err(hdev->req_result);
=======
		err = -bt_to_errno(hdev->req_result);
>>>>>>> refs/remotes/origin/master
		break;

	case HCI_REQ_CANCELED:
		err = -hdev->req_result;
		break;

	default:
		err = -ETIMEDOUT;
		break;
	}

	hdev->req_status = hdev->req_result = 0;

	BT_DBG("%s end: err %d", hdev->name, err);

	return err;
}

<<<<<<< HEAD
static inline int hci_request(struct hci_dev *hdev, void (*req)(struct hci_dev *hdev, unsigned long opt),
					unsigned long opt, __u32 timeout)
=======
static int hci_req_sync(struct hci_dev *hdev,
			void (*req)(struct hci_request *req,
				    unsigned long opt),
			unsigned long opt, __u32 timeout)
>>>>>>> refs/remotes/origin/master
{
	int ret;

	if (!test_bit(HCI_UP, &hdev->flags))
		return -ENETDOWN;

	/* Serialize all requests */
	hci_req_lock(hdev);
<<<<<<< HEAD
	ret = __hci_request(hdev, req, opt, timeout);
=======
	ret = __hci_req_sync(hdev, req, opt, timeout);
>>>>>>> refs/remotes/origin/master
	hci_req_unlock(hdev);

	return ret;
}

<<<<<<< HEAD
static void hci_reset_req(struct hci_dev *hdev, unsigned long opt)
{
	BT_DBG("%s %ld", hdev->name, opt);

	/* Reset device */
	set_bit(HCI_RESET, &hdev->flags);
	memset(&hdev->features, 0, sizeof(hdev->features));
	hci_send_cmd(hdev, HCI_OP_RESET, 0, NULL);
}

static void hci_init_req(struct hci_dev *hdev, unsigned long opt)
{
	struct hci_cp_delete_stored_link_key cp;
	struct sk_buff *skb;
	__le16 param;
	__u8 flt_type;

	BT_DBG("%s %ld", hdev->name, opt);

	/* Driver initialization */

	/* Special commands */
	while ((skb = skb_dequeue(&hdev->driver_init))) {
		bt_cb(skb)->pkt_type = HCI_COMMAND_PKT;
		skb->dev = (void *) hdev;

		skb_queue_tail(&hdev->cmd_q, skb);
		tasklet_schedule(&hdev->cmd_task);
	}
	skb_queue_purge(&hdev->driver_init);

	/* Mandatory initialization */

	/* Reset */
	if (!test_bit(HCI_QUIRK_NO_RESET, &hdev->quirks)) {
			set_bit(HCI_RESET, &hdev->flags);
			hci_send_cmd(hdev, HCI_OP_RESET, 0, NULL);
	}

	/* Read Local Version */
	hci_send_cmd(hdev, HCI_OP_READ_LOCAL_VERSION, 0, NULL);


	/* Set default HCI Flow Control Mode */
	if (hdev->dev_type == HCI_BREDR)
		hdev->flow_ctl_mode = HCI_PACKET_BASED_FLOW_CTL_MODE;
	else
		hdev->flow_ctl_mode = HCI_BLOCK_BASED_FLOW_CTL_MODE;

	/* Read HCI Flow Control Mode */
	hci_send_cmd(hdev, HCI_OP_READ_FLOW_CONTROL_MODE, 0, NULL);

	/* Read Buffer Size (ACL mtu, max pkt, etc.) */
	hci_send_cmd(hdev, HCI_OP_READ_BUFFER_SIZE, 0, NULL);

	/* Read Data Block Size (ACL mtu, max pkt, etc.) */
	hci_send_cmd(hdev, HCI_OP_READ_DATA_BLOCK_SIZE, 0, NULL);

#if 0
	/* Host buffer size */
	{
		struct hci_cp_host_buffer_size cp;
		cp.acl_mtu = cpu_to_le16(HCI_MAX_ACL_SIZE);
		cp.sco_mtu = HCI_MAX_SCO_SIZE;
		cp.acl_max_pkt = cpu_to_le16(0xffff);
		cp.sco_max_pkt = cpu_to_le16(0xffff);
		hci_send_cmd(hdev, HCI_OP_HOST_BUFFER_SIZE, sizeof(cp), &cp);
	}
#endif

	if (hdev->dev_type == HCI_BREDR) {
		/* BR-EDR initialization */

		/* Read Local Supported Features */
		hci_send_cmd(hdev, HCI_OP_READ_LOCAL_FEATURES, 0, NULL);

		/* Read BD Address */
		hci_send_cmd(hdev, HCI_OP_READ_BD_ADDR, 0, NULL);

		/* Read Class of Device */
		hci_send_cmd(hdev, HCI_OP_READ_CLASS_OF_DEV, 0, NULL);

		/* Read Local Name */
		hci_send_cmd(hdev, HCI_OP_READ_LOCAL_NAME, 0, NULL);

		/* Read Voice Setting */
		hci_send_cmd(hdev, HCI_OP_READ_VOICE_SETTING, 0, NULL);

		/* Optional initialization */
		/* Clear Event Filters */
		flt_type = HCI_FLT_CLEAR_ALL;
		hci_send_cmd(hdev, HCI_OP_SET_EVENT_FLT, 1, &flt_type);

		/* Connection accept timeout ~20 secs */
		param = cpu_to_le16(0x7d00);
		hci_send_cmd(hdev, HCI_OP_WRITE_CA_TIMEOUT, 2, &param);

		bacpy(&cp.bdaddr, BDADDR_ANY);
		cp.delete_all = 1;
		hci_send_cmd(hdev, HCI_OP_DELETE_STORED_LINK_KEY,
				sizeof(cp), &cp);
	} else {
		/* AMP initialization */
		/* Connection accept timeout ~5 secs */
		param = cpu_to_le16(0x1f40);
		hci_send_cmd(hdev, HCI_OP_WRITE_CA_TIMEOUT, 2, &param);

		/* Read AMP Info */
		hci_send_cmd(hdev, HCI_OP_READ_LOCAL_AMP_INFO, 0, NULL);
	}
}

static void hci_le_init_req(struct hci_dev *hdev, unsigned long opt)
{
	BT_DBG("%s", hdev->name);

	/* Read LE buffer size */
	hci_send_cmd(hdev, HCI_OP_LE_READ_BUFFER_SIZE, 0, NULL);
<<<<<<< HEAD

	/* Read LE clear white list */
	hci_send_cmd(hdev, HCI_OP_LE_CLEAR_WHITE_LIST, 0, NULL);

	/* Read LE white list size */
	hci_send_cmd(hdev, HCI_OP_LE_READ_WHITE_LIST_SIZE, 0, NULL);
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

static void hci_scan_req(struct hci_dev *hdev, unsigned long opt)
{
	__u8 scan = opt;

	BT_DBG("%s %x", hdev->name, scan);

	/* Inquiry and Page scans */
	hci_send_cmd(hdev, HCI_OP_WRITE_SCAN_ENABLE, 1, &scan);
}

static void hci_auth_req(struct hci_dev *hdev, unsigned long opt)
{
	__u8 auth = opt;

	BT_DBG("%s %x", hdev->name, auth);

	/* Authentication */
	hci_send_cmd(hdev, HCI_OP_WRITE_AUTH_ENABLE, 1, &auth);
}

static void hci_encrypt_req(struct hci_dev *hdev, unsigned long opt)
{
	__u8 encrypt = opt;

	BT_DBG("%s %x", hdev->name, encrypt);

	/* Encryption */
	hci_send_cmd(hdev, HCI_OP_WRITE_ENCRYPT_MODE, 1, &encrypt);
}

static void hci_linkpol_req(struct hci_dev *hdev, unsigned long opt)
{
	__le16 policy = cpu_to_le16(opt);

	BT_DBG("%s %x", hdev->name, policy);

	/* Default link policy */
	hci_send_cmd(hdev, HCI_OP_WRITE_DEF_LINK_POLICY, 2, &policy);
}

/* Get HCI device by index.
 * Device is held on return. */
struct hci_dev *hci_dev_get(int index)
{
	struct hci_dev *hdev = NULL;
	struct list_head *p;

	BT_DBG("%d", index);

	if (index < 0)
		return NULL;

	read_lock(&hci_dev_list_lock);
	list_for_each(p, &hci_dev_list) {
		struct hci_dev *d = list_entry(p, struct hci_dev, list);
		if (d->id == index) {
			hdev = hci_dev_hold(d);
			break;
		}
	}
	read_unlock(&hci_dev_list_lock);
	return hdev;
}
EXPORT_SYMBOL(hci_dev_get);

/* ---- Inquiry support ---- */
static void inquiry_cache_flush(struct hci_dev *hdev)
{
	struct inquiry_cache *cache = &hdev->inq_cache;
	struct inquiry_entry *next  = cache->list, *e;

	BT_DBG("cache %p", cache);

	cache->list = NULL;
	while ((e = next)) {
		next = e->next;
		kfree(e);
	}
}

struct inquiry_entry *hci_inquiry_cache_lookup(struct hci_dev *hdev, bdaddr_t *bdaddr)
{
	struct inquiry_cache *cache = &hdev->inq_cache;
	struct inquiry_entry *e;

	BT_DBG("cache %p, %s", cache, batostr(bdaddr));

	for (e = cache->list; e; e = e->next)
		if (!bacmp(&e->data.bdaddr, bdaddr))
			break;
	return e;
}

void hci_inquiry_cache_update(struct hci_dev *hdev, struct inquiry_data *data)
{
	struct inquiry_cache *cache = &hdev->inq_cache;
	struct inquiry_entry *ie;

	BT_DBG("cache %p, %s", cache, batostr(&data->bdaddr));

	ie = hci_inquiry_cache_lookup(hdev, &data->bdaddr);
	if (!ie) {
		/* Entry not in the cache. Add new one. */
		ie = kzalloc(sizeof(struct inquiry_entry), GFP_ATOMIC);
		if (!ie)
			return;

		ie->next = cache->list;
		cache->list = ie;
=======
static void hci_reset_req(struct hci_request *req, unsigned long opt)
{
	BT_DBG("%s %ld", req->hdev->name, opt);

	/* Reset device */
	set_bit(HCI_RESET, &req->hdev->flags);
	hci_req_add(req, HCI_OP_RESET, 0, NULL);
}

static void bredr_init(struct hci_request *req)
{
	req->hdev->flow_ctl_mode = HCI_FLOW_CTL_MODE_PACKET_BASED;

	/* Read Local Supported Features */
	hci_req_add(req, HCI_OP_READ_LOCAL_FEATURES, 0, NULL);

=======
	/* Reset */
	if (!test_bit(HCI_QUIRK_NO_RESET, &hdev->quirks)) {
			set_bit(HCI_RESET, &hdev->flags);
			hci_send_cmd(hdev, HCI_OP_RESET, 0, NULL);
	}

>>>>>>> refs/remotes/origin/cm-11.0
	/* Read Local Version */
	hci_req_add(req, HCI_OP_READ_LOCAL_VERSION, 0, NULL);

	/* Read BD Address */
	hci_req_add(req, HCI_OP_READ_BD_ADDR, 0, NULL);
}

static void amp_init(struct hci_request *req)
{
	req->hdev->flow_ctl_mode = HCI_FLOW_CTL_MODE_BLOCK_BASED;

	/* Read Local Version */
	hci_req_add(req, HCI_OP_READ_LOCAL_VERSION, 0, NULL);

	/* Read Local Supported Commands */
	hci_req_add(req, HCI_OP_READ_LOCAL_COMMANDS, 0, NULL);

	/* Read Local Supported Features */
	hci_req_add(req, HCI_OP_READ_LOCAL_FEATURES, 0, NULL);

	/* Read Local AMP Info */
	hci_req_add(req, HCI_OP_READ_LOCAL_AMP_INFO, 0, NULL);

	/* Read Data Blk size */
	hci_req_add(req, HCI_OP_READ_DATA_BLOCK_SIZE, 0, NULL);

	/* Read Flow Control Mode */
	hci_req_add(req, HCI_OP_READ_FLOW_CONTROL_MODE, 0, NULL);

	/* Read Location Data */
	hci_req_add(req, HCI_OP_READ_LOCATION_DATA, 0, NULL);
}

static void hci_init1_req(struct hci_request *req, unsigned long opt)
{
	struct hci_dev *hdev = req->hdev;

	BT_DBG("%s %ld", hdev->name, opt);

	/* Reset */
	if (!test_bit(HCI_QUIRK_RESET_ON_CLOSE, &hdev->quirks))
		hci_reset_req(req, 0);

	switch (hdev->dev_type) {
	case HCI_BREDR:
		bredr_init(req);
		break;

	case HCI_AMP:
		amp_init(req);
		break;

	default:
		BT_ERR("Unknown device type %d", hdev->dev_type);
		break;
	}
}

static void bredr_setup(struct hci_request *req)
{
	struct hci_dev *hdev = req->hdev;

	__le16 param;
	__u8 flt_type;

<<<<<<< HEAD
	/* Read Buffer Size (ACL mtu, max pkt, etc.) */
	hci_req_add(req, HCI_OP_READ_BUFFER_SIZE, 0, NULL);

	/* Read Class of Device */
	hci_req_add(req, HCI_OP_READ_CLASS_OF_DEV, 0, NULL);

	/* Read Local Name */
	hci_req_add(req, HCI_OP_READ_LOCAL_NAME, 0, NULL);

	/* Read Voice Setting */
	hci_req_add(req, HCI_OP_READ_VOICE_SETTING, 0, NULL);

	/* Read Number of Supported IAC */
	hci_req_add(req, HCI_OP_READ_NUM_SUPPORTED_IAC, 0, NULL);

	/* Read Current IAC LAP */
	hci_req_add(req, HCI_OP_READ_CURRENT_IAC_LAP, 0, NULL);

	/* Clear Event Filters */
	flt_type = HCI_FLT_CLEAR_ALL;
	hci_req_add(req, HCI_OP_SET_EVENT_FLT, 1, &flt_type);

	/* Connection accept timeout ~20 secs */
	param = __constant_cpu_to_le16(0x7d00);
	hci_req_add(req, HCI_OP_WRITE_CA_TIMEOUT, 2, &param);

	/* AVM Berlin (31), aka "BlueFRITZ!", reports version 1.2,
	 * but it does not support page scan related HCI commands.
	 */
	if (hdev->manufacturer != 31 && hdev->hci_ver > BLUETOOTH_VER_1_1) {
		hci_req_add(req, HCI_OP_READ_PAGE_SCAN_ACTIVITY, 0, NULL);
		hci_req_add(req, HCI_OP_READ_PAGE_SCAN_TYPE, 0, NULL);
	}
}

static void le_setup(struct hci_request *req)
{
	struct hci_dev *hdev = req->hdev;

	/* Read LE Buffer Size */
	hci_req_add(req, HCI_OP_LE_READ_BUFFER_SIZE, 0, NULL);

	/* Read LE Local Supported Features */
	hci_req_add(req, HCI_OP_LE_READ_LOCAL_FEATURES, 0, NULL);

	/* Read LE Advertising Channel TX Power */
	hci_req_add(req, HCI_OP_LE_READ_ADV_TX_POWER, 0, NULL);

	/* Read LE White List Size */
	hci_req_add(req, HCI_OP_LE_READ_WHITE_LIST_SIZE, 0, NULL);

	/* Read LE Supported States */
	hci_req_add(req, HCI_OP_LE_READ_SUPPORTED_STATES, 0, NULL);

	/* LE-only controllers have LE implicitly enabled */
	if (!lmp_bredr_capable(hdev))
		set_bit(HCI_LE_ENABLED, &hdev->dev_flags);
}

static u8 hci_get_inquiry_mode(struct hci_dev *hdev)
{
	if (lmp_ext_inq_capable(hdev))
		return 0x02;

	if (lmp_inq_rssi_capable(hdev))
		return 0x01;

	if (hdev->manufacturer == 11 && hdev->hci_rev == 0x00 &&
	    hdev->lmp_subver == 0x0757)
		return 0x01;

	if (hdev->manufacturer == 15) {
		if (hdev->hci_rev == 0x03 && hdev->lmp_subver == 0x6963)
			return 0x01;
		if (hdev->hci_rev == 0x09 && hdev->lmp_subver == 0x6963)
			return 0x01;
		if (hdev->hci_rev == 0x00 && hdev->lmp_subver == 0x6965)
			return 0x01;
	}

	if (hdev->manufacturer == 31 && hdev->hci_rev == 0x2005 &&
	    hdev->lmp_subver == 0x1805)
		return 0x01;

	return 0x00;
}

static void hci_setup_inquiry_mode(struct hci_request *req)
{
	u8 mode;

	mode = hci_get_inquiry_mode(req->hdev);

	hci_req_add(req, HCI_OP_WRITE_INQUIRY_MODE, 1, &mode);
}

static void hci_setup_event_mask(struct hci_request *req)
{
	struct hci_dev *hdev = req->hdev;

	/* The second byte is 0xff instead of 0x9f (two reserved bits
	 * disabled) since a Broadcom 1.2 dongle doesn't respond to the
	 * command otherwise.
	 */
	u8 events[8] = { 0xff, 0xff, 0xfb, 0xff, 0x00, 0x00, 0x00, 0x00 };

	/* CSR 1.1 dongles does not accept any bitfield so don't try to set
	 * any event mask for pre 1.2 devices.
	 */
	if (hdev->hci_ver < BLUETOOTH_VER_1_2)
		return;

	if (lmp_bredr_capable(hdev)) {
		events[4] |= 0x01; /* Flow Specification Complete */
		events[4] |= 0x02; /* Inquiry Result with RSSI */
		events[4] |= 0x04; /* Read Remote Extended Features Complete */
		events[5] |= 0x08; /* Synchronous Connection Complete */
		events[5] |= 0x10; /* Synchronous Connection Changed */
	} else {
		/* Use a different default for LE-only devices */
		memset(events, 0, sizeof(events));
		events[0] |= 0x10; /* Disconnection Complete */
		events[0] |= 0x80; /* Encryption Change */
		events[1] |= 0x08; /* Read Remote Version Information Complete */
		events[1] |= 0x20; /* Command Complete */
		events[1] |= 0x40; /* Command Status */
		events[1] |= 0x80; /* Hardware Error */
		events[2] |= 0x04; /* Number of Completed Packets */
		events[3] |= 0x02; /* Data Buffer Overflow */
		events[5] |= 0x80; /* Encryption Key Refresh Complete */
	}

	if (lmp_inq_rssi_capable(hdev))
		events[4] |= 0x02; /* Inquiry Result with RSSI */

	if (lmp_sniffsubr_capable(hdev))
		events[5] |= 0x20; /* Sniff Subrating */

	if (lmp_pause_enc_capable(hdev))
		events[5] |= 0x80; /* Encryption Key Refresh Complete */

	if (lmp_ext_inq_capable(hdev))
		events[5] |= 0x40; /* Extended Inquiry Result */

	if (lmp_no_flush_capable(hdev))
		events[7] |= 0x01; /* Enhanced Flush Complete */

	if (lmp_lsto_capable(hdev))
		events[6] |= 0x80; /* Link Supervision Timeout Changed */

	if (lmp_ssp_capable(hdev)) {
		events[6] |= 0x01;	/* IO Capability Request */
		events[6] |= 0x02;	/* IO Capability Response */
		events[6] |= 0x04;	/* User Confirmation Request */
		events[6] |= 0x08;	/* User Passkey Request */
		events[6] |= 0x10;	/* Remote OOB Data Request */
		events[6] |= 0x20;	/* Simple Pairing Complete */
		events[7] |= 0x04;	/* User Passkey Notification */
		events[7] |= 0x08;	/* Keypress Notification */
		events[7] |= 0x10;	/* Remote Host Supported
					 * Features Notification
					 */
	}

	if (lmp_le_capable(hdev))
		events[7] |= 0x20;	/* LE Meta-Event */

	hci_req_add(req, HCI_OP_SET_EVENT_MASK, sizeof(events), events);

	if (lmp_le_capable(hdev)) {
		memset(events, 0, sizeof(events));
		events[0] = 0x1f;
		hci_req_add(req, HCI_OP_LE_SET_EVENT_MASK,
			    sizeof(events), events);
	}
}

static void hci_init2_req(struct hci_request *req, unsigned long opt)
{
	struct hci_dev *hdev = req->hdev;

	if (lmp_bredr_capable(hdev))
		bredr_setup(req);
	else
		clear_bit(HCI_BREDR_ENABLED, &hdev->dev_flags);

	if (lmp_le_capable(hdev))
		le_setup(req);

	hci_setup_event_mask(req);

	/* AVM Berlin (31), aka "BlueFRITZ!", doesn't support the read
	 * local supported commands HCI command.
	 */
	if (hdev->manufacturer != 31 && hdev->hci_ver > BLUETOOTH_VER_1_1)
		hci_req_add(req, HCI_OP_READ_LOCAL_COMMANDS, 0, NULL);

	if (lmp_ssp_capable(hdev)) {
		/* When SSP is available, then the host features page
		 * should also be available as well. However some
		 * controllers list the max_page as 0 as long as SSP
		 * has not been enabled. To achieve proper debugging
		 * output, force the minimum max_page to 1 at least.
		 */
		hdev->max_page = 0x01;

		if (test_bit(HCI_SSP_ENABLED, &hdev->dev_flags)) {
			u8 mode = 0x01;
			hci_req_add(req, HCI_OP_WRITE_SSP_MODE,
				    sizeof(mode), &mode);
		} else {
			struct hci_cp_write_eir cp;

			memset(hdev->eir, 0, sizeof(hdev->eir));
			memset(&cp, 0, sizeof(cp));

			hci_req_add(req, HCI_OP_WRITE_EIR, sizeof(cp), &cp);
		}
	}

	if (lmp_inq_rssi_capable(hdev))
		hci_setup_inquiry_mode(req);

	if (lmp_inq_tx_pwr_capable(hdev))
		hci_req_add(req, HCI_OP_READ_INQ_RSP_TX_POWER, 0, NULL);

	if (lmp_ext_feat_capable(hdev)) {
		struct hci_cp_read_local_ext_features cp;

		cp.page = 0x01;
		hci_req_add(req, HCI_OP_READ_LOCAL_EXT_FEATURES,
			    sizeof(cp), &cp);
	}

	if (test_bit(HCI_LINK_SECURITY, &hdev->dev_flags)) {
		u8 enable = 1;
		hci_req_add(req, HCI_OP_WRITE_AUTH_ENABLE, sizeof(enable),
			    &enable);
	}
}

static void hci_setup_link_policy(struct hci_request *req)
{
	struct hci_dev *hdev = req->hdev;
	struct hci_cp_write_def_link_policy cp;
	u16 link_policy = 0;

	if (lmp_rswitch_capable(hdev))
		link_policy |= HCI_LP_RSWITCH;
	if (lmp_hold_capable(hdev))
		link_policy |= HCI_LP_HOLD;
	if (lmp_sniff_capable(hdev))
		link_policy |= HCI_LP_SNIFF;
	if (lmp_park_capable(hdev))
		link_policy |= HCI_LP_PARK;

	cp.policy = cpu_to_le16(link_policy);
	hci_req_add(req, HCI_OP_WRITE_DEF_LINK_POLICY, sizeof(cp), &cp);
}

static void hci_set_le_support(struct hci_request *req)
{
	struct hci_dev *hdev = req->hdev;
	struct hci_cp_write_le_host_supported cp;

	/* LE-only devices do not support explicit enablement */
	if (!lmp_bredr_capable(hdev))
		return;

	memset(&cp, 0, sizeof(cp));

	if (test_bit(HCI_LE_ENABLED, &hdev->dev_flags)) {
		cp.le = 0x01;
		cp.simul = lmp_le_br_capable(hdev);
	}

	if (cp.le != lmp_host_le_capable(hdev))
		hci_req_add(req, HCI_OP_WRITE_LE_HOST_SUPPORTED, sizeof(cp),
			    &cp);
}

static void hci_set_event_mask_page_2(struct hci_request *req)
{
	struct hci_dev *hdev = req->hdev;
	u8 events[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	/* If Connectionless Slave Broadcast master role is supported
	 * enable all necessary events for it.
	 */
	if (hdev->features[2][0] & 0x01) {
		events[1] |= 0x40;	/* Triggered Clock Capture */
		events[1] |= 0x80;	/* Synchronization Train Complete */
		events[2] |= 0x10;	/* Slave Page Response Timeout */
		events[2] |= 0x20;	/* CSB Channel Map Change */
	}

	/* If Connectionless Slave Broadcast slave role is supported
	 * enable all necessary events for it.
	 */
	if (hdev->features[2][0] & 0x02) {
		events[2] |= 0x01;	/* Synchronization Train Received */
		events[2] |= 0x02;	/* CSB Receive */
		events[2] |= 0x04;	/* CSB Timeout */
		events[2] |= 0x08;	/* Truncated Page Complete */
	}

	hci_req_add(req, HCI_OP_SET_EVENT_MASK_PAGE_2, sizeof(events), events);
}

static void hci_init3_req(struct hci_request *req, unsigned long opt)
{
	struct hci_dev *hdev = req->hdev;
	u8 p;

	/* Some Broadcom based Bluetooth controllers do not support the
	 * Delete Stored Link Key command. They are clearly indicating its
	 * absence in the bit mask of supported commands.
	 *
	 * Check the supported commands and only if the the command is marked
	 * as supported send it. If not supported assume that the controller
	 * does not have actual support for stored link keys which makes this
	 * command redundant anyway.
	 */
	if (hdev->commands[6] & 0x80) {
		struct hci_cp_delete_stored_link_key cp;

		bacpy(&cp.bdaddr, BDADDR_ANY);
		cp.delete_all = 0x01;
		hci_req_add(req, HCI_OP_DELETE_STORED_LINK_KEY,
			    sizeof(cp), &cp);
	}

	if (hdev->commands[5] & 0x10)
		hci_setup_link_policy(req);

	if (lmp_le_capable(hdev)) {
		/* If the controller has a public BD_ADDR, then by
		 * default use that one. If this is a LE only
		 * controller without one, default to the random
		 * address.
		 */
		if (bacmp(&hdev->bdaddr, BDADDR_ANY))
			hdev->own_addr_type = ADDR_LE_DEV_PUBLIC;
		else
			hdev->own_addr_type = ADDR_LE_DEV_RANDOM;

		hci_set_le_support(req);
	}

	/* Read features beyond page 1 if available */
	for (p = 2; p < HCI_MAX_PAGES && p <= hdev->max_page; p++) {
		struct hci_cp_read_local_ext_features cp;

		cp.page = p;
		hci_req_add(req, HCI_OP_READ_LOCAL_EXT_FEATURES,
			    sizeof(cp), &cp);
	}
}

static void hci_init4_req(struct hci_request *req, unsigned long opt)
{
	struct hci_dev *hdev = req->hdev;

	/* Set event mask page 2 if the HCI command for it is supported */
	if (hdev->commands[22] & 0x04)
		hci_set_event_mask_page_2(req);

	/* Check for Synchronization Train support */
	if (hdev->features[2][0] & 0x04)
		hci_req_add(req, HCI_OP_READ_SYNC_TRAIN_PARAMS, 0, NULL);
}

static int __hci_init(struct hci_dev *hdev)
{
	int err;

	err = __hci_req_sync(hdev, hci_init1_req, 0, HCI_INIT_TIMEOUT);
	if (err < 0)
		return err;

	/* The Device Under Test (DUT) mode is special and available for
	 * all controller types. So just create it early on.
	 */
	if (test_bit(HCI_SETUP, &hdev->dev_flags)) {
		debugfs_create_file("dut_mode", 0644, hdev->debugfs, hdev,
				    &dut_mode_fops);
	}

	/* HCI_BREDR covers both single-mode LE, BR/EDR and dual-mode
	 * BR/EDR/LE type controllers. AMP controllers only need the
	 * first stage init.
	 */
	if (hdev->dev_type != HCI_BREDR)
		return 0;

	err = __hci_req_sync(hdev, hci_init2_req, 0, HCI_INIT_TIMEOUT);
	if (err < 0)
		return err;

	err = __hci_req_sync(hdev, hci_init3_req, 0, HCI_INIT_TIMEOUT);
	if (err < 0)
		return err;

	err = __hci_req_sync(hdev, hci_init4_req, 0, HCI_INIT_TIMEOUT);
	if (err < 0)
		return err;

	/* Only create debugfs entries during the initial setup
	 * phase and not every time the controller gets powered on.
	 */
	if (!test_bit(HCI_SETUP, &hdev->dev_flags))
		return 0;

	debugfs_create_file("features", 0444, hdev->debugfs, hdev,
			    &features_fops);
	debugfs_create_u16("manufacturer", 0444, hdev->debugfs,
			   &hdev->manufacturer);
	debugfs_create_u8("hci_version", 0444, hdev->debugfs, &hdev->hci_ver);
	debugfs_create_u16("hci_revision", 0444, hdev->debugfs, &hdev->hci_rev);
	debugfs_create_file("blacklist", 0444, hdev->debugfs, hdev,
			    &blacklist_fops);
	debugfs_create_file("uuids", 0444, hdev->debugfs, hdev, &uuids_fops);

	if (lmp_bredr_capable(hdev)) {
		debugfs_create_file("inquiry_cache", 0444, hdev->debugfs,
				    hdev, &inquiry_cache_fops);
		debugfs_create_file("link_keys", 0400, hdev->debugfs,
				    hdev, &link_keys_fops);
		debugfs_create_file("use_debug_keys", 0444, hdev->debugfs,
				    hdev, &use_debug_keys_fops);
		debugfs_create_file("dev_class", 0444, hdev->debugfs,
				    hdev, &dev_class_fops);
		debugfs_create_file("voice_setting", 0444, hdev->debugfs,
				    hdev, &voice_setting_fops);
	}

	if (lmp_ssp_capable(hdev)) {
		debugfs_create_file("auto_accept_delay", 0644, hdev->debugfs,
				    hdev, &auto_accept_delay_fops);
		debugfs_create_file("ssp_debug_mode", 0644, hdev->debugfs,
				    hdev, &ssp_debug_mode_fops);
	}
=======

	/* Set default HCI Flow Control Mode */
	if (hdev->dev_type == HCI_BREDR)
		hdev->flow_ctl_mode = HCI_PACKET_BASED_FLOW_CTL_MODE;
	else
		hdev->flow_ctl_mode = HCI_BLOCK_BASED_FLOW_CTL_MODE;

	/* Read HCI Flow Control Mode */
	hci_send_cmd(hdev, HCI_OP_READ_FLOW_CONTROL_MODE, 0, NULL);

	/* Read Buffer Size (ACL mtu, max pkt, etc.) */
	hci_send_cmd(hdev, HCI_OP_READ_BUFFER_SIZE, 0, NULL);

	/* Read Data Block Size (ACL mtu, max pkt, etc.) */
	hci_send_cmd(hdev, HCI_OP_READ_DATA_BLOCK_SIZE, 0, NULL);

#if 0
	/* Host buffer size */
	{
		struct hci_cp_host_buffer_size cp;
		cp.acl_mtu = cpu_to_le16(HCI_MAX_ACL_SIZE);
		cp.sco_mtu = HCI_MAX_SCO_SIZE;
		cp.acl_max_pkt = cpu_to_le16(0xffff);
		cp.sco_max_pkt = cpu_to_le16(0xffff);
		hci_send_cmd(hdev, HCI_OP_HOST_BUFFER_SIZE, sizeof(cp), &cp);
	}
#endif

	if (hdev->dev_type == HCI_BREDR) {
		/* BR-EDR initialization */

		/* Read Local Supported Features */
		hci_send_cmd(hdev, HCI_OP_READ_LOCAL_FEATURES, 0, NULL);

		/* Read BD Address */
		hci_send_cmd(hdev, HCI_OP_READ_BD_ADDR, 0, NULL);

		/* Read Class of Device */
		hci_send_cmd(hdev, HCI_OP_READ_CLASS_OF_DEV, 0, NULL);

		/* Read Local Name */
		hci_send_cmd(hdev, HCI_OP_READ_LOCAL_NAME, 0, NULL);

		/* Read Voice Setting */
		hci_send_cmd(hdev, HCI_OP_READ_VOICE_SETTING, 0, NULL);

		/* Optional initialization */
		/* Clear Event Filters */
		flt_type = HCI_FLT_CLEAR_ALL;
		hci_send_cmd(hdev, HCI_OP_SET_EVENT_FLT, 1, &flt_type);

		/* Connection accept timeout ~20 secs */
		param = cpu_to_le16(0x7d00);
		hci_send_cmd(hdev, HCI_OP_WRITE_CA_TIMEOUT, 2, &param);

		bacpy(&cp.bdaddr, BDADDR_ANY);
		cp.delete_all = 1;
		hci_send_cmd(hdev, HCI_OP_DELETE_STORED_LINK_KEY,
				sizeof(cp), &cp);
	} else {
		/* AMP initialization */
		/* Connection accept timeout ~5 secs */
		param = cpu_to_le16(0x1f40);
		hci_send_cmd(hdev, HCI_OP_WRITE_CA_TIMEOUT, 2, &param);

		/* Read AMP Info */
		hci_send_cmd(hdev, HCI_OP_READ_LOCAL_AMP_INFO, 0, NULL);
	}
}
>>>>>>> refs/remotes/origin/cm-11.0

	if (lmp_sniff_capable(hdev)) {
		debugfs_create_file("idle_timeout", 0644, hdev->debugfs,
				    hdev, &idle_timeout_fops);
		debugfs_create_file("sniff_min_interval", 0644, hdev->debugfs,
				    hdev, &sniff_min_interval_fops);
		debugfs_create_file("sniff_max_interval", 0644, hdev->debugfs,
				    hdev, &sniff_max_interval_fops);
	}

<<<<<<< HEAD
	if (lmp_le_capable(hdev)) {
		debugfs_create_u8("white_list_size", 0444, hdev->debugfs,
				  &hdev->le_white_list_size);
		debugfs_create_file("static_address", 0444, hdev->debugfs,
				   hdev, &static_address_fops);
		debugfs_create_file("own_address_type", 0644, hdev->debugfs,
				    hdev, &own_address_type_fops);
		debugfs_create_file("long_term_keys", 0400, hdev->debugfs,
				    hdev, &long_term_keys_fops);
		debugfs_create_file("conn_min_interval", 0644, hdev->debugfs,
				    hdev, &conn_min_interval_fops);
		debugfs_create_file("conn_max_interval", 0644, hdev->debugfs,
				    hdev, &conn_max_interval_fops);
	}

	return 0;
=======
	/* Read LE buffer size */
	hci_send_cmd(hdev, HCI_OP_LE_READ_BUFFER_SIZE, 0, NULL);

	/* Read LE clear white list */
	hci_send_cmd(hdev, HCI_OP_LE_CLEAR_WHITE_LIST, 0, NULL);

	/* Read LE white list size */
	hci_send_cmd(hdev, HCI_OP_LE_READ_WHITE_LIST_SIZE, 0, NULL);
>>>>>>> refs/remotes/origin/cm-11.0
}

static void hci_scan_req(struct hci_request *req, unsigned long opt)
{
	__u8 scan = opt;

	BT_DBG("%s %x", req->hdev->name, scan);

	/* Inquiry and Page scans */
	hci_req_add(req, HCI_OP_WRITE_SCAN_ENABLE, 1, &scan);
}

static void hci_auth_req(struct hci_request *req, unsigned long opt)
{
	__u8 auth = opt;

	BT_DBG("%s %x", req->hdev->name, auth);

	/* Authentication */
	hci_req_add(req, HCI_OP_WRITE_AUTH_ENABLE, 1, &auth);
}

static void hci_encrypt_req(struct hci_request *req, unsigned long opt)
{
	__u8 encrypt = opt;

	BT_DBG("%s %x", req->hdev->name, encrypt);

	/* Encryption */
	hci_req_add(req, HCI_OP_WRITE_ENCRYPT_MODE, 1, &encrypt);
}

static void hci_linkpol_req(struct hci_request *req, unsigned long opt)
{
	__le16 policy = cpu_to_le16(opt);

	BT_DBG("%s %x", req->hdev->name, policy);

	/* Default link policy */
	hci_req_add(req, HCI_OP_WRITE_DEF_LINK_POLICY, 2, &policy);
}

/* Get HCI device by index.
 * Device is held on return. */
struct hci_dev *hci_dev_get(int index)
{
	struct hci_dev *hdev = NULL;
	struct list_head *p;

	BT_DBG("%d", index);

	if (index < 0)
		return NULL;

	read_lock(&hci_dev_list_lock);
	list_for_each(p, &hci_dev_list) {
		struct hci_dev *d = list_entry(p, struct hci_dev, list);
		if (d->id == index) {
			hdev = hci_dev_hold(d);
			break;
		}
	}
	read_unlock(&hci_dev_list_lock);
	return hdev;
}
EXPORT_SYMBOL(hci_dev_get);

/* ---- Inquiry support ---- */
<<<<<<< HEAD

bool hci_discovery_active(struct hci_dev *hdev)
{
	struct discovery_state *discov = &hdev->discovery;

	switch (discov->state) {
	case DISCOVERY_FINDING:
	case DISCOVERY_RESOLVING:
		return true;

	default:
		return false;
	}
}

void hci_discovery_set_state(struct hci_dev *hdev, int state)
{
	BT_DBG("%s state %u -> %u", hdev->name, hdev->discovery.state, state);

	if (hdev->discovery.state == state)
		return;

	switch (state) {
	case DISCOVERY_STOPPED:
		if (hdev->discovery.state != DISCOVERY_STARTING)
			mgmt_discovering(hdev, 0);
		break;
	case DISCOVERY_STARTING:
		break;
	case DISCOVERY_FINDING:
		mgmt_discovering(hdev, 1);
		break;
	case DISCOVERY_RESOLVING:
		break;
	case DISCOVERY_STOPPING:
		break;
	}

	hdev->discovery.state = state;
}

void hci_inquiry_cache_flush(struct hci_dev *hdev)
{
	struct discovery_state *cache = &hdev->discovery;
	struct inquiry_entry *p, *n;

	list_for_each_entry_safe(p, n, &cache->all, all) {
		list_del(&p->all);
		kfree(p);
	}

	INIT_LIST_HEAD(&cache->unknown);
	INIT_LIST_HEAD(&cache->resolve);
}

struct inquiry_entry *hci_inquiry_cache_lookup(struct hci_dev *hdev,
					       bdaddr_t *bdaddr)
{
	struct discovery_state *cache = &hdev->discovery;
	struct inquiry_entry *e;

	BT_DBG("cache %p, %pMR", cache, bdaddr);
=======
static void inquiry_cache_flush(struct hci_dev *hdev)
{
	struct inquiry_cache *cache = &hdev->inq_cache;
	struct inquiry_entry *next  = cache->list, *e;

	BT_DBG("cache %p", cache);
>>>>>>> refs/remotes/origin/cm-11.0

	cache->list = NULL;
	while ((e = next)) {
		next = e->next;
		kfree(e);
	}
}

struct inquiry_entry *hci_inquiry_cache_lookup(struct hci_dev *hdev, bdaddr_t *bdaddr)
{
	struct inquiry_cache *cache = &hdev->inq_cache;
	struct inquiry_entry *e;

	BT_DBG("cache %p, %pMR", cache, bdaddr);

<<<<<<< HEAD
	list_for_each_entry(e, &cache->unknown, list) {
		if (!bacmp(&e->data.bdaddr, bdaddr))
			return e;
	}

	return NULL;
}

struct inquiry_entry *hci_inquiry_cache_lookup_resolve(struct hci_dev *hdev,
						       bdaddr_t *bdaddr,
						       int state)
{
	struct discovery_state *cache = &hdev->discovery;
	struct inquiry_entry *e;

	BT_DBG("cache %p bdaddr %pMR state %d", cache, bdaddr, state);

	list_for_each_entry(e, &cache->resolve, list) {
		if (!bacmp(bdaddr, BDADDR_ANY) && e->name_state == state)
			return e;
		if (!bacmp(&e->data.bdaddr, bdaddr))
			return e;
	}

	return NULL;
}

void hci_inquiry_cache_update_resolve(struct hci_dev *hdev,
				      struct inquiry_entry *ie)
{
	struct discovery_state *cache = &hdev->discovery;
	struct list_head *pos = &cache->resolve;
	struct inquiry_entry *p;

	list_del(&ie->list);

	list_for_each_entry(p, &cache->resolve, list) {
		if (p->name_state != NAME_PENDING &&
		    abs(p->data.rssi) >= abs(ie->data.rssi))
=======
	for (e = cache->list; e; e = e->next)
		if (!bacmp(&e->data.bdaddr, bdaddr))
>>>>>>> refs/remotes/origin/cm-11.0
			break;
	return e;
}

void hci_inquiry_cache_update(struct hci_dev *hdev, struct inquiry_data *data)
{
	struct inquiry_cache *cache = &hdev->inq_cache;
	struct inquiry_entry *ie;

	BT_DBG("cache %p, %pMR", cache, &data->bdaddr);

	hci_remove_remote_oob_data(hdev, &data->bdaddr);

	ie = hci_inquiry_cache_lookup(hdev, &data->bdaddr);
<<<<<<< HEAD
	if (ie) {
		if (ie->data.ssp_mode && ssp)
			*ssp = true;

		if (ie->name_state == NAME_NEEDED &&
		    data->rssi != ie->data.rssi) {
			ie->data.rssi = data->rssi;
			hci_inquiry_cache_update_resolve(hdev, ie);
		}

		goto update;
	}

	/* Entry not in the cache. Add new one. */
	ie = kzalloc(sizeof(struct inquiry_entry), GFP_ATOMIC);
	if (!ie)
		return false;

	list_add(&ie->all, &cache->all);

	if (name_known) {
		ie->name_state = NAME_KNOWN;
	} else {
		ie->name_state = NAME_NOT_KNOWN;
		list_add(&ie->list, &cache->unknown);
	}

update:
	if (name_known && ie->name_state != NAME_KNOWN &&
	    ie->name_state != NAME_PENDING) {
		ie->name_state = NAME_KNOWN;
		list_del(&ie->list);
>>>>>>> refs/remotes/origin/master
=======
	if (!ie) {
		/* Entry not in the cache. Add new one. */
		ie = kzalloc(sizeof(struct inquiry_entry), GFP_ATOMIC);
		if (!ie)
			return;

		ie->next = cache->list;
		cache->list = ie;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	memcpy(&ie->data, data, sizeof(*data));
	ie->timestamp = jiffies;
	cache->timestamp = jiffies;
<<<<<<< HEAD
<<<<<<< HEAD
=======

	if (ie->name_state == NAME_NOT_KNOWN)
		return false;

	return true;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static int inquiry_cache_dump(struct hci_dev *hdev, int num, __u8 *buf)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct inquiry_cache *cache = &hdev->inq_cache;
=======
	struct discovery_state *cache = &hdev->discovery;
>>>>>>> refs/remotes/origin/master
=======
	struct inquiry_cache *cache = &hdev->inq_cache;
>>>>>>> refs/remotes/origin/cm-11.0
	struct inquiry_info *info = (struct inquiry_info *) buf;
	struct inquiry_entry *e;
	int copied = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	for (e = cache->list; e && copied < num; e = e->next, copied++) {
		struct inquiry_data *data = &e->data;
=======
	list_for_each_entry(e, &cache->all, all) {
		struct inquiry_data *data = &e->data;

		if (copied >= num)
			break;

>>>>>>> refs/remotes/origin/master
=======
	for (e = cache->list; e && copied < num; e = e->next, copied++) {
		struct inquiry_data *data = &e->data;
>>>>>>> refs/remotes/origin/cm-11.0
		bacpy(&info->bdaddr, &data->bdaddr);
		info->pscan_rep_mode	= data->pscan_rep_mode;
		info->pscan_period_mode	= data->pscan_period_mode;
		info->pscan_mode	= data->pscan_mode;
		memcpy(info->dev_class, data->dev_class, 3);
		info->clock_offset	= data->clock_offset;
<<<<<<< HEAD
<<<<<<< HEAD
		info++;
=======

		info++;
		copied++;
>>>>>>> refs/remotes/origin/master
=======
		info++;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	BT_DBG("cache %p, copied %d", cache, copied);
	return copied;
}

<<<<<<< HEAD
static void hci_inq_req(struct hci_dev *hdev, unsigned long opt)
{
	struct hci_inquiry_req *ir = (struct hci_inquiry_req *) opt;
=======
static void hci_inq_req(struct hci_request *req, unsigned long opt)
{
	struct hci_inquiry_req *ir = (struct hci_inquiry_req *) opt;
	struct hci_dev *hdev = req->hdev;
>>>>>>> refs/remotes/origin/master
	struct hci_cp_inquiry cp;

	BT_DBG("%s", hdev->name);

	if (test_bit(HCI_INQUIRY, &hdev->flags))
		return;

	/* Start Inquiry */
	memcpy(&cp.lap, &ir->lap, 3);
	cp.length  = ir->length;
	cp.num_rsp = ir->num_rsp;
<<<<<<< HEAD
	hci_send_cmd(hdev, HCI_OP_INQUIRY, sizeof(cp), &cp);
=======
	hci_req_add(req, HCI_OP_INQUIRY, sizeof(cp), &cp);
}

static int wait_inquiry(void *word)
{
	schedule();
	return signal_pending(current);
>>>>>>> refs/remotes/origin/master
}

int hci_inquiry(void __user *arg)
{
	__u8 __user *ptr = arg;
	struct hci_inquiry_req ir;
	struct hci_dev *hdev;
	int err = 0, do_inquiry = 0, max_rsp;
	long timeo;
	__u8 *buf;

	if (copy_from_user(&ir, ptr, sizeof(ir)))
		return -EFAULT;

	hdev = hci_dev_get(ir.dev_id);
	if (!hdev)
		return -ENODEV;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	hci_dev_lock_bh(hdev);
	if (inquiry_cache_age(hdev) > INQUIRY_CACHE_AGE_MAX ||
				inquiry_cache_empty(hdev) ||
				ir.flags & IREQ_CACHE_FLUSH) {
		inquiry_cache_flush(hdev);
		do_inquiry = 1;
	}
	hci_dev_unlock_bh(hdev);
<<<<<<< HEAD
=======
	if (test_bit(HCI_USER_CHANNEL, &hdev->dev_flags)) {
		err = -EBUSY;
		goto done;
	}

	if (hdev->dev_type != HCI_BREDR) {
		err = -EOPNOTSUPP;
		goto done;
	}

	if (!test_bit(HCI_BREDR_ENABLED, &hdev->dev_flags)) {
		err = -EOPNOTSUPP;
		goto done;
	}

	hci_dev_lock(hdev);
	if (inquiry_cache_age(hdev) > INQUIRY_CACHE_AGE_MAX ||
	    inquiry_cache_empty(hdev) || ir.flags & IREQ_CACHE_FLUSH) {
		hci_inquiry_cache_flush(hdev);
		do_inquiry = 1;
	}
	hci_dev_unlock(hdev);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	timeo = ir.length * msecs_to_jiffies(2000);

	if (do_inquiry) {
<<<<<<< HEAD
		err = hci_request(hdev, hci_inq_req, (unsigned long)&ir, timeo);
		if (err < 0)
			goto done;
	}

	/* for unlimited number of responses we will use buffer with 255 entries */
=======
		err = hci_req_sync(hdev, hci_inq_req, (unsigned long) &ir,
				   timeo);
		if (err < 0)
			goto done;

		/* Wait until Inquiry procedure finishes (HCI_INQUIRY flag is
		 * cleared). If it is interrupted by a signal, return -EINTR.
		 */
		if (wait_on_bit(&hdev->flags, HCI_INQUIRY, wait_inquiry,
				TASK_INTERRUPTIBLE))
			return -EINTR;
	}

	/* for unlimited number of responses we will use buffer with
	 * 255 entries
	 */
>>>>>>> refs/remotes/origin/master
	max_rsp = (ir.num_rsp == 0) ? 255 : ir.num_rsp;

	/* cache_dump can't sleep. Therefore we allocate temp buffer and then
	 * copy it to the user space.
	 */
	buf = kmalloc(sizeof(struct inquiry_info) * max_rsp, GFP_KERNEL);
	if (!buf) {
		err = -ENOMEM;
		goto done;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	hci_dev_lock_bh(hdev);
	ir.num_rsp = inquiry_cache_dump(hdev, max_rsp, buf);
	hci_dev_unlock_bh(hdev);
=======
	hci_dev_lock(hdev);
	ir.num_rsp = inquiry_cache_dump(hdev, max_rsp, buf);
	hci_dev_unlock(hdev);
>>>>>>> refs/remotes/origin/master
=======
	hci_dev_lock_bh(hdev);
	ir.num_rsp = inquiry_cache_dump(hdev, max_rsp, buf);
	hci_dev_unlock_bh(hdev);
>>>>>>> refs/remotes/origin/cm-11.0

	BT_DBG("num_rsp %d", ir.num_rsp);

	if (!copy_to_user(ptr, &ir, sizeof(ir))) {
		ptr += sizeof(ir);
		if (copy_to_user(ptr, buf, sizeof(struct inquiry_info) *
<<<<<<< HEAD
					ir.num_rsp))
=======
				 ir.num_rsp))
>>>>>>> refs/remotes/origin/master
			err = -EFAULT;
	} else
		err = -EFAULT;

	kfree(buf);

done:
	hci_dev_put(hdev);
	return err;
}

<<<<<<< HEAD
/* ---- HCI ioctl helpers ---- */

int hci_dev_open(__u16 dev)
{
	struct hci_dev *hdev;
	int ret = 0;

	hdev = hci_dev_get(dev);
	if (!hdev)
		return -ENODEV;

=======
static int hci_dev_do_open(struct hci_dev *hdev)
{
	int ret = 0;

>>>>>>> refs/remotes/origin/master
	BT_DBG("%s %p", hdev->name, hdev);

	hci_req_lock(hdev);

<<<<<<< HEAD
<<<<<<< HEAD
	if (hdev->rfkill && rfkill_blocked(hdev->rfkill)) {
		ret = -ERFKILL;
		goto done;
	}

=======
	if (test_bit(HCI_UNREGISTER, &hdev->dev_flags)) {
		ret = -ENODEV;
		goto done;
	}

	if (!test_bit(HCI_SETUP, &hdev->dev_flags)) {
		/* Check for rfkill but allow the HCI setup stage to
		 * proceed (which in itself doesn't cause any RF activity).
		 */
		if (test_bit(HCI_RFKILLED, &hdev->dev_flags)) {
			ret = -ERFKILL;
			goto done;
		}

		/* Check for valid public address or a configured static
		 * random adddress, but let the HCI setup proceed to
		 * be able to determine if there is a public address
		 * or not.
		 *
		 * This check is only valid for BR/EDR controllers
		 * since AMP controllers do not have an address.
		 */
		if (hdev->dev_type == HCI_BREDR &&
		    !bacmp(&hdev->bdaddr, BDADDR_ANY) &&
		    !bacmp(&hdev->static_addr, BDADDR_ANY)) {
			ret = -EADDRNOTAVAIL;
			goto done;
		}
=======
	if (hdev->rfkill && rfkill_blocked(hdev->rfkill)) {
		ret = -ERFKILL;
		goto done;
>>>>>>> refs/remotes/origin/cm-11.0
	}

>>>>>>> refs/remotes/origin/master
	if (test_bit(HCI_UP, &hdev->flags)) {
		ret = -EALREADY;
		goto done;
	}

<<<<<<< HEAD
	if (test_bit(HCI_QUIRK_RAW_DEVICE, &hdev->quirks))
		set_bit(HCI_RAW, &hdev->flags);

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (hdev->open(hdev)) {
		ret = -EIO;
		goto done;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (!skb_queue_empty(&hdev->cmd_q)) {
		BT_ERR("command queue is not empty, purging");
		skb_queue_purge(&hdev->cmd_q);
	}
	if (!skb_queue_empty(&hdev->rx_q)) {
		BT_ERR("rx queue is not empty, purging");
		skb_queue_purge(&hdev->rx_q);
	}
	if (!skb_queue_empty(&hdev->raw_q)) {
		BT_ERR("raw queue is not empty, purging");
		skb_queue_purge(&hdev->raw_q);
	}

	if (!test_bit(HCI_RAW, &hdev->flags)) {
		atomic_set(&hdev->cmd_cnt, 1);
		set_bit(HCI_INIT, &hdev->flags);
		hdev->init_last_cmd = 0;

		ret = __hci_request(hdev, hci_init_req, 0,
					msecs_to_jiffies(HCI_INIT_TIMEOUT));

		if (lmp_le_capable(hdev))
			ret = __hci_request(hdev, hci_le_init_req, 0,
					msecs_to_jiffies(HCI_INIT_TIMEOUT));

		clear_bit(HCI_INIT, &hdev->flags);
	}

=======
	atomic_set(&hdev->cmd_cnt, 1);
	set_bit(HCI_INIT, &hdev->flags);

	if (hdev->setup && test_bit(HCI_SETUP, &hdev->dev_flags))
		ret = hdev->setup(hdev);

	if (!ret) {
		if (test_bit(HCI_QUIRK_RAW_DEVICE, &hdev->quirks))
			set_bit(HCI_RAW, &hdev->flags);

		if (!test_bit(HCI_RAW, &hdev->flags) &&
		    !test_bit(HCI_USER_CHANNEL, &hdev->dev_flags))
			ret = __hci_init(hdev);
	}

	clear_bit(HCI_INIT, &hdev->flags);

>>>>>>> refs/remotes/origin/master
	if (!ret) {
		hci_dev_hold(hdev);
		set_bit(HCI_UP, &hdev->flags);
		hci_notify(hdev, HCI_DEV_UP);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		if (!test_bit(HCI_SETUP, &hdev->flags) &&
				hdev->dev_type == HCI_BREDR) {
			hci_dev_lock_bh(hdev);
			mgmt_powered(hdev->id, 1);
			hci_dev_unlock_bh(hdev);
<<<<<<< HEAD
		}
	} else {
		/* Init failed, cleanup */
		tasklet_kill(&hdev->rx_task);
		tasklet_kill(&hdev->tx_task);
		tasklet_kill(&hdev->cmd_task);
=======
		if (!test_bit(HCI_SETUP, &hdev->dev_flags) &&
		    !test_bit(HCI_USER_CHANNEL, &hdev->dev_flags) &&
		    hdev->dev_type == HCI_BREDR) {
			hci_dev_lock(hdev);
			mgmt_powered(hdev, 1);
			hci_dev_unlock(hdev);
		}
	} else {
		/* Init failed, cleanup */
		flush_work(&hdev->tx_work);
		flush_work(&hdev->cmd_work);
		flush_work(&hdev->rx_work);
>>>>>>> refs/remotes/origin/master
=======
		}
	} else {
		/* Init failed, cleanup */
		tasklet_kill(&hdev->rx_task);
		tasklet_kill(&hdev->tx_task);
		tasklet_kill(&hdev->cmd_task);
>>>>>>> refs/remotes/origin/cm-11.0

		skb_queue_purge(&hdev->cmd_q);
		skb_queue_purge(&hdev->rx_q);

		if (hdev->flush)
			hdev->flush(hdev);

		if (hdev->sent_cmd) {
			kfree_skb(hdev->sent_cmd);
			hdev->sent_cmd = NULL;
		}

		hdev->close(hdev);
		hdev->flags = 0;
	}

done:
	hci_req_unlock(hdev);
<<<<<<< HEAD
	hci_dev_put(hdev);
	return ret;
}

static int hci_dev_do_close(struct hci_dev *hdev, u8 is_process)
<<<<<<< HEAD
{
	unsigned long keepflags = 0;

	BT_DBG("%s %p", hdev->name, hdev);

=======
	return ret;
}

/* ---- HCI ioctl helpers ---- */

int hci_dev_open(__u16 dev)
{
	struct hci_dev *hdev;
	int err;

	hdev = hci_dev_get(dev);
	if (!hdev)
		return -ENODEV;

	/* We need to ensure that no other power on/off work is pending
	 * before proceeding to call hci_dev_do_open. This is
	 * particularly important if the setup procedure has not yet
	 * completed.
	 */
	if (test_and_clear_bit(HCI_AUTO_OFF, &hdev->dev_flags))
		cancel_delayed_work(&hdev->power_off);

	/* After this call it is guaranteed that the setup procedure
	 * has finished. This means that error conditions like RFKILL
	 * or no valid public or static random address apply.
	 */
	flush_workqueue(hdev->req_workqueue);

	err = hci_dev_do_open(hdev);

	hci_dev_put(hdev);

	return err;
}

static int hci_dev_do_close(struct hci_dev *hdev)
=======
>>>>>>> refs/remotes/origin/cm-11.0
{
	unsigned long keepflags = 0;

<<<<<<< HEAD
	cancel_delayed_work(&hdev->power_off);
=======
	BT_DBG("%s %p", hdev->name, hdev);
>>>>>>> refs/remotes/origin/cm-11.0

>>>>>>> refs/remotes/origin/master
	hci_req_cancel(hdev, ENODEV);
	hci_req_lock(hdev);

	if (!test_and_clear_bit(HCI_UP, &hdev->flags)) {
		del_timer_sync(&hdev->cmd_timer);
		hci_req_unlock(hdev);
		return 0;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/* Kill RX and TX tasks */
	tasklet_kill(&hdev->rx_task);
	tasklet_kill(&hdev->tx_task);

	hci_dev_lock_bh(hdev);
	inquiry_cache_flush(hdev);
	hci_conn_hash_flush(hdev, is_process);
	hci_dev_unlock_bh(hdev);

	hci_notify(hdev, HCI_DEV_DOWN);

	if (hdev->dev_type == HCI_BREDR) {
		hci_dev_lock_bh(hdev);
		mgmt_powered(hdev->id, 0);
		hci_dev_unlock_bh(hdev);
	}
=======
	/* Flush RX and TX works */
	flush_work(&hdev->tx_work);
	flush_work(&hdev->rx_work);

	if (hdev->discov_timeout > 0) {
		cancel_delayed_work(&hdev->discov_off);
		hdev->discov_timeout = 0;
		clear_bit(HCI_DISCOVERABLE, &hdev->dev_flags);
		clear_bit(HCI_LIMITED_DISCOVERABLE, &hdev->dev_flags);
	}

	if (test_and_clear_bit(HCI_SERVICE_CACHE, &hdev->dev_flags))
		cancel_delayed_work(&hdev->service_cache);

	cancel_delayed_work_sync(&hdev->le_scan_disable);

	hci_dev_lock(hdev);
	hci_inquiry_cache_flush(hdev);
	hci_conn_hash_flush(hdev);
	hci_dev_unlock(hdev);
=======
	/* Kill RX and TX tasks */
	tasklet_kill(&hdev->rx_task);
	tasklet_kill(&hdev->tx_task);

	hci_dev_lock_bh(hdev);
	inquiry_cache_flush(hdev);
	hci_conn_hash_flush(hdev, is_process);
	hci_dev_unlock_bh(hdev);
>>>>>>> refs/remotes/origin/cm-11.0

	hci_notify(hdev, HCI_DEV_DOWN);
>>>>>>> refs/remotes/origin/master

	if (hdev->dev_type == HCI_BREDR) {
		hci_dev_lock_bh(hdev);
		mgmt_powered(hdev->id, 0);
		hci_dev_unlock_bh(hdev);
	}

	if (hdev->flush)
		hdev->flush(hdev);

	/* Reset device */
	skb_queue_purge(&hdev->cmd_q);
	atomic_set(&hdev->cmd_cnt, 1);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (!test_bit(HCI_RAW, &hdev->flags)) {
		set_bit(HCI_INIT, &hdev->flags);
		__hci_request(hdev, hci_reset_req, 0,
					msecs_to_jiffies(250));
		clear_bit(HCI_INIT, &hdev->flags);
	}

	/* Kill cmd task */
	tasklet_kill(&hdev->cmd_task);
<<<<<<< HEAD
=======
	if (!test_bit(HCI_RAW, &hdev->flags) &&
	    !test_bit(HCI_AUTO_OFF, &hdev->dev_flags) &&
	    test_bit(HCI_QUIRK_RESET_ON_CLOSE, &hdev->quirks)) {
		set_bit(HCI_INIT, &hdev->flags);
		__hci_req_sync(hdev, hci_reset_req, 0, HCI_CMD_TIMEOUT);
		clear_bit(HCI_INIT, &hdev->flags);
	}

	/* flush cmd  work */
	flush_work(&hdev->cmd_work);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	/* Drop queues */
	skb_queue_purge(&hdev->rx_q);
	skb_queue_purge(&hdev->cmd_q);
	skb_queue_purge(&hdev->raw_q);

	/* Drop last sent command */
	if (hdev->sent_cmd) {
		del_timer_sync(&hdev->cmd_timer);
		kfree_skb(hdev->sent_cmd);
		hdev->sent_cmd = NULL;
	}

<<<<<<< HEAD
=======
	kfree_skb(hdev->recv_evt);
	hdev->recv_evt = NULL;

>>>>>>> refs/remotes/origin/master
	/* After this point our queues are empty
	 * and no tasks are scheduled. */
	hdev->close(hdev);

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* Clear only non-persistent flags */
	if (test_bit(HCI_MGMT, &hdev->flags))
		set_bit(HCI_MGMT, &keepflags);
	if (test_bit(HCI_LINK_KEYS, &hdev->flags))
		set_bit(HCI_LINK_KEYS, &keepflags);
	if (test_bit(HCI_DEBUG_KEYS, &hdev->flags))
		set_bit(HCI_DEBUG_KEYS, &keepflags);
<<<<<<< HEAD

	hdev->flags = keepflags;
=======
	/* Clear flags */
	hdev->flags = 0;
	hdev->dev_flags &= ~HCI_PERSISTENT_MASK;

	if (!test_and_clear_bit(HCI_AUTO_OFF, &hdev->dev_flags)) {
		if (hdev->dev_type == HCI_BREDR) {
			hci_dev_lock(hdev);
			mgmt_powered(hdev, 0);
			hci_dev_unlock(hdev);
		}
	}

	/* Controller radio is available but is currently powered down */
	hdev->amp_status = AMP_STATUS_POWERED_DOWN;

	memset(hdev->eir, 0, sizeof(hdev->eir));
	memset(hdev->dev_class, 0, sizeof(hdev->dev_class));
>>>>>>> refs/remotes/origin/master
=======

	hdev->flags = keepflags;
>>>>>>> refs/remotes/origin/cm-11.0

	hci_req_unlock(hdev);

	hci_dev_put(hdev);
	return 0;
}

int hci_dev_close(__u16 dev)
{
	struct hci_dev *hdev;
	int err;

	hdev = hci_dev_get(dev);
	if (!hdev)
		return -ENODEV;
<<<<<<< HEAD
<<<<<<< HEAD
	err = hci_dev_do_close(hdev, 1);
=======

	if (test_bit(HCI_USER_CHANNEL, &hdev->dev_flags)) {
		err = -EBUSY;
		goto done;
	}

	if (test_and_clear_bit(HCI_AUTO_OFF, &hdev->dev_flags))
		cancel_delayed_work(&hdev->power_off);

	err = hci_dev_do_close(hdev);

done:
>>>>>>> refs/remotes/origin/master
=======
	err = hci_dev_do_close(hdev, 1);
>>>>>>> refs/remotes/origin/cm-11.0
	hci_dev_put(hdev);
	return err;
}

int hci_dev_reset(__u16 dev)
{
	struct hci_dev *hdev;
	int ret = 0;

	hdev = hci_dev_get(dev);
	if (!hdev)
		return -ENODEV;

	hci_req_lock(hdev);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	tasklet_disable(&hdev->tx_task);

	if (!test_bit(HCI_UP, &hdev->flags))
		goto done;
=======

	if (!test_bit(HCI_UP, &hdev->flags)) {
		ret = -ENETDOWN;
		goto done;
	}

	if (test_bit(HCI_USER_CHANNEL, &hdev->dev_flags)) {
		ret = -EBUSY;
		goto done;
	}
>>>>>>> refs/remotes/origin/master

	/* Drop queues */
	skb_queue_purge(&hdev->rx_q);
	skb_queue_purge(&hdev->cmd_q);

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	hci_dev_lock_bh(hdev);
	inquiry_cache_flush(hdev);
	hci_conn_hash_flush(hdev, 0);
	hci_dev_unlock_bh(hdev);
<<<<<<< HEAD
=======
	hci_dev_lock(hdev);
	hci_inquiry_cache_flush(hdev);
	hci_conn_hash_flush(hdev);
	hci_dev_unlock(hdev);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	if (hdev->flush)
		hdev->flush(hdev);

	atomic_set(&hdev->cmd_cnt, 1);
	hdev->acl_cnt = 0; hdev->sco_cnt = 0; hdev->le_cnt = 0;

	if (!test_bit(HCI_RAW, &hdev->flags))
<<<<<<< HEAD
		ret = __hci_request(hdev, hci_reset_req, 0,
					msecs_to_jiffies(HCI_INIT_TIMEOUT));

done:
	tasklet_enable(&hdev->tx_task);
<<<<<<< HEAD
=======
		ret = __hci_req_sync(hdev, hci_reset_req, 0, HCI_INIT_TIMEOUT);

done:
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	hci_req_unlock(hdev);
	hci_dev_put(hdev);
	return ret;
}

int hci_dev_reset_stat(__u16 dev)
{
	struct hci_dev *hdev;
	int ret = 0;

	hdev = hci_dev_get(dev);
	if (!hdev)
		return -ENODEV;

<<<<<<< HEAD
	memset(&hdev->stat, 0, sizeof(struct hci_dev_stats));

	hci_dev_put(hdev);

=======
	if (test_bit(HCI_USER_CHANNEL, &hdev->dev_flags)) {
		ret = -EBUSY;
		goto done;
	}

	memset(&hdev->stat, 0, sizeof(struct hci_dev_stats));

done:
	hci_dev_put(hdev);
>>>>>>> refs/remotes/origin/master
	return ret;
}

int hci_dev_cmd(unsigned int cmd, void __user *arg)
{
	struct hci_dev *hdev;
	struct hci_dev_req dr;
	int err = 0;

	if (copy_from_user(&dr, arg, sizeof(dr)))
		return -EFAULT;

	hdev = hci_dev_get(dr.dev_id);
	if (!hdev)
		return -ENODEV;

<<<<<<< HEAD
	switch (cmd) {
	case HCISETAUTH:
		err = hci_request(hdev, hci_auth_req, dr.dev_opt,
					msecs_to_jiffies(HCI_INIT_TIMEOUT));
=======
	if (test_bit(HCI_USER_CHANNEL, &hdev->dev_flags)) {
		err = -EBUSY;
		goto done;
	}

	if (hdev->dev_type != HCI_BREDR) {
		err = -EOPNOTSUPP;
		goto done;
	}

	if (!test_bit(HCI_BREDR_ENABLED, &hdev->dev_flags)) {
		err = -EOPNOTSUPP;
		goto done;
	}

	switch (cmd) {
	case HCISETAUTH:
		err = hci_req_sync(hdev, hci_auth_req, dr.dev_opt,
				   HCI_INIT_TIMEOUT);
>>>>>>> refs/remotes/origin/master
		break;

	case HCISETENCRYPT:
		if (!lmp_encrypt_capable(hdev)) {
			err = -EOPNOTSUPP;
			break;
		}

		if (!test_bit(HCI_AUTH, &hdev->flags)) {
			/* Auth must be enabled first */
<<<<<<< HEAD
			err = hci_request(hdev, hci_auth_req, dr.dev_opt,
					msecs_to_jiffies(HCI_INIT_TIMEOUT));
=======
			err = hci_req_sync(hdev, hci_auth_req, dr.dev_opt,
					   HCI_INIT_TIMEOUT);
>>>>>>> refs/remotes/origin/master
			if (err)
				break;
		}

<<<<<<< HEAD
		err = hci_request(hdev, hci_encrypt_req, dr.dev_opt,
					msecs_to_jiffies(HCI_INIT_TIMEOUT));
		break;

	case HCISETSCAN:
		err = hci_request(hdev, hci_scan_req, dr.dev_opt,
					msecs_to_jiffies(HCI_INIT_TIMEOUT));
		break;

	case HCISETLINKPOL:
		err = hci_request(hdev, hci_linkpol_req, dr.dev_opt,
					msecs_to_jiffies(HCI_INIT_TIMEOUT));
=======
		err = hci_req_sync(hdev, hci_encrypt_req, dr.dev_opt,
				   HCI_INIT_TIMEOUT);
		break;

	case HCISETSCAN:
		err = hci_req_sync(hdev, hci_scan_req, dr.dev_opt,
				   HCI_INIT_TIMEOUT);
		break;

	case HCISETLINKPOL:
		err = hci_req_sync(hdev, hci_linkpol_req, dr.dev_opt,
				   HCI_INIT_TIMEOUT);
>>>>>>> refs/remotes/origin/master
		break;

	case HCISETLINKMODE:
		hdev->link_mode = ((__u16) dr.dev_opt) &
					(HCI_LM_MASTER | HCI_LM_ACCEPT);
		break;

	case HCISETPTYPE:
		hdev->pkt_type = (__u16) dr.dev_opt;
		break;

	case HCISETACLMTU:
		hdev->acl_mtu  = *((__u16 *) &dr.dev_opt + 1);
		hdev->acl_pkts = *((__u16 *) &dr.dev_opt + 0);
		break;

	case HCISETSCOMTU:
		hdev->sco_mtu  = *((__u16 *) &dr.dev_opt + 1);
		hdev->sco_pkts = *((__u16 *) &dr.dev_opt + 0);
		break;

	default:
		err = -EINVAL;
		break;
	}

<<<<<<< HEAD
=======
done:
>>>>>>> refs/remotes/origin/master
	hci_dev_put(hdev);
	return err;
}

int hci_get_dev_list(void __user *arg)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct hci_dev_list_req *dl;
	struct hci_dev_req *dr;
	struct list_head *p;
=======
	struct hci_dev *hdev;
	struct hci_dev_list_req *dl;
	struct hci_dev_req *dr;
>>>>>>> refs/remotes/origin/master
=======
	struct hci_dev_list_req *dl;
	struct hci_dev_req *dr;
	struct list_head *p;
>>>>>>> refs/remotes/origin/cm-11.0
	int n = 0, size, err;
	__u16 dev_num;

	if (get_user(dev_num, (__u16 __user *) arg))
		return -EFAULT;

	if (!dev_num || dev_num > (PAGE_SIZE * 2) / sizeof(*dr))
		return -EINVAL;

	size = sizeof(*dl) + dev_num * sizeof(*dr);

	dl = kzalloc(size, GFP_KERNEL);
	if (!dl)
		return -ENOMEM;

	dr = dl->dev_req;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	read_lock_bh(&hci_dev_list_lock);
	list_for_each(p, &hci_dev_list) {
		struct hci_dev *hdev;

		hdev = list_entry(p, struct hci_dev, list);
<<<<<<< HEAD

		hci_del_off_timer(hdev);

		if (!test_bit(HCI_MGMT, &hdev->flags))
			set_bit(HCI_PAIRABLE, &hdev->flags);
=======
	read_lock(&hci_dev_list_lock);
	list_for_each_entry(hdev, &hci_dev_list, list) {
		if (test_and_clear_bit(HCI_AUTO_OFF, &hdev->dev_flags))
			cancel_delayed_work(&hdev->power_off);

		if (!test_bit(HCI_MGMT, &hdev->dev_flags))
			set_bit(HCI_PAIRABLE, &hdev->dev_flags);
>>>>>>> refs/remotes/origin/master
=======

		hci_del_off_timer(hdev);

		if (!test_bit(HCI_MGMT, &hdev->flags))
			set_bit(HCI_PAIRABLE, &hdev->flags);
>>>>>>> refs/remotes/origin/cm-11.0

		(dr + n)->dev_id  = hdev->id;
		(dr + n)->dev_opt = hdev->flags;

		if (++n >= dev_num)
			break;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	read_unlock_bh(&hci_dev_list_lock);
=======
	read_unlock(&hci_dev_list_lock);
>>>>>>> refs/remotes/origin/master
=======
	read_unlock_bh(&hci_dev_list_lock);
>>>>>>> refs/remotes/origin/cm-11.0

	dl->dev_num = n;
	size = sizeof(*dl) + n * sizeof(*dr);

	err = copy_to_user(arg, dl, size);
	kfree(dl);

	return err ? -EFAULT : 0;
}

int hci_get_dev_info(void __user *arg)
{
	struct hci_dev *hdev;
	struct hci_dev_info di;
	int err = 0;

	if (copy_from_user(&di, arg, sizeof(di)))
		return -EFAULT;

	hdev = hci_dev_get(di.dev_id);
	if (!hdev)
		return -ENODEV;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	hci_del_off_timer(hdev);

	if (!test_bit(HCI_MGMT, &hdev->flags))
		set_bit(HCI_PAIRABLE, &hdev->flags);

	strcpy(di.name, hdev->name);
	di.bdaddr   = hdev->bdaddr;
	di.type     = (hdev->bus & 0x0f) | (hdev->dev_type << 4);
	di.flags    = hdev->flags;
	di.pkt_type = hdev->pkt_type;
	di.acl_mtu  = hdev->acl_mtu;
	di.acl_pkts = hdev->acl_pkts;
	di.sco_mtu  = hdev->sco_mtu;
	di.sco_pkts = hdev->sco_pkts;
=======
	if (test_and_clear_bit(HCI_AUTO_OFF, &hdev->dev_flags))
		cancel_delayed_work_sync(&hdev->power_off);

	if (!test_bit(HCI_MGMT, &hdev->dev_flags))
		set_bit(HCI_PAIRABLE, &hdev->dev_flags);

	strcpy(di.name, hdev->name);
	di.bdaddr   = hdev->bdaddr;
	di.type     = (hdev->bus & 0x0f) | ((hdev->dev_type & 0x03) << 4);
	di.flags    = hdev->flags;
	di.pkt_type = hdev->pkt_type;
	if (lmp_bredr_capable(hdev)) {
		di.acl_mtu  = hdev->acl_mtu;
		di.acl_pkts = hdev->acl_pkts;
		di.sco_mtu  = hdev->sco_mtu;
		di.sco_pkts = hdev->sco_pkts;
	} else {
		di.acl_mtu  = hdev->le_mtu;
		di.acl_pkts = hdev->le_pkts;
		di.sco_mtu  = 0;
		di.sco_pkts = 0;
	}
>>>>>>> refs/remotes/origin/master
	di.link_policy = hdev->link_policy;
	di.link_mode   = hdev->link_mode;

	memcpy(&di.stat, &hdev->stat, sizeof(di.stat));
	memcpy(&di.features, &hdev->features, sizeof(di.features));

	if (copy_to_user(arg, &di, sizeof(di)))
		err = -EFAULT;

	hci_dev_put(hdev);

	return err;
}

/* ---- Interface to HCI drivers ---- */

static int hci_rfkill_set_block(void *data, bool blocked)
{
	struct hci_dev *hdev = data;

	BT_DBG("%p name %s blocked %d", hdev, hdev->name, blocked);

<<<<<<< HEAD
	if (!blocked)
		return 0;

	hci_dev_do_close(hdev, 0);
<<<<<<< HEAD
=======
	if (test_bit(HCI_USER_CHANNEL, &hdev->dev_flags))
		return -EBUSY;

	if (blocked) {
		set_bit(HCI_RFKILLED, &hdev->dev_flags);
		if (!test_bit(HCI_SETUP, &hdev->dev_flags))
			hci_dev_do_close(hdev);
	} else {
		clear_bit(HCI_RFKILLED, &hdev->dev_flags);
	}
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	return 0;
}

static const struct rfkill_ops hci_rfkill_ops = {
	.set_block = hci_rfkill_set_block,
};

<<<<<<< HEAD
/* Alloc HCI device */
struct hci_dev *hci_alloc_dev(void)
{
	struct hci_dev *hdev;

	hdev = kzalloc(sizeof(struct hci_dev), GFP_KERNEL);
	if (!hdev)
		return NULL;

	skb_queue_head_init(&hdev->driver_init);

	return hdev;
}
EXPORT_SYMBOL(hci_alloc_dev);

/* Free HCI device */
void hci_free_dev(struct hci_dev *hdev)
{
	skb_queue_purge(&hdev->driver_init);

	/* will free via device release */
	put_device(&hdev->dev);
}
EXPORT_SYMBOL(hci_free_dev);

=======
>>>>>>> refs/remotes/origin/master
static void hci_power_on(struct work_struct *work)
{
	struct hci_dev *hdev = container_of(work, struct hci_dev, power_on);
	int err;

	BT_DBG("%s", hdev->name);

<<<<<<< HEAD
<<<<<<< HEAD
	err = hci_dev_open(hdev->id);
	if (err && err != -EALREADY)
		return;

	if (test_bit(HCI_AUTO_OFF, &hdev->flags) &&
				hdev->dev_type == HCI_BREDR)
		mod_timer(&hdev->off_timer,
				jiffies + msecs_to_jiffies(AUTO_OFF_TIMEOUT));

	if (test_and_clear_bit(HCI_SETUP, &hdev->flags) &&
				hdev->dev_type == HCI_BREDR)
		mgmt_index_added(hdev->id);
=======
	err = hci_dev_do_open(hdev);
	if (err < 0) {
		mgmt_set_powered_failed(hdev, err);
=======
	err = hci_dev_open(hdev->id);
	if (err && err != -EALREADY)
>>>>>>> refs/remotes/origin/cm-11.0
		return;
	}

<<<<<<< HEAD
	/* During the HCI setup phase, a few error conditions are
	 * ignored and they need to be checked now. If they are still
	 * valid, it is important to turn the device back off.
	 */
	if (test_bit(HCI_RFKILLED, &hdev->dev_flags) ||
	    (hdev->dev_type == HCI_BREDR &&
	     !bacmp(&hdev->bdaddr, BDADDR_ANY) &&
	     !bacmp(&hdev->static_addr, BDADDR_ANY))) {
		clear_bit(HCI_AUTO_OFF, &hdev->dev_flags);
		hci_dev_do_close(hdev);
	} else if (test_bit(HCI_AUTO_OFF, &hdev->dev_flags)) {
		queue_delayed_work(hdev->req_workqueue, &hdev->power_off,
				   HCI_AUTO_OFF_TIMEOUT);
	}

	if (test_and_clear_bit(HCI_SETUP, &hdev->dev_flags))
		mgmt_index_added(hdev);
>>>>>>> refs/remotes/origin/master
=======
	if (test_bit(HCI_AUTO_OFF, &hdev->flags) &&
				hdev->dev_type == HCI_BREDR)
		mod_timer(&hdev->off_timer,
				jiffies + msecs_to_jiffies(AUTO_OFF_TIMEOUT));

	if (test_and_clear_bit(HCI_SETUP, &hdev->flags) &&
				hdev->dev_type == HCI_BREDR)
		mgmt_index_added(hdev->id);
>>>>>>> refs/remotes/origin/cm-11.0
}

static void hci_power_off(struct work_struct *work)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct hci_dev *hdev = container_of(work, struct hci_dev, power_off);

	BT_DBG("%s", hdev->name);

	hci_dev_close(hdev->id);
}

static void hci_auto_off(unsigned long data)
{
	struct hci_dev *hdev = (struct hci_dev *) data;

	BT_DBG("%s", hdev->name);

	clear_bit(HCI_AUTO_OFF, &hdev->flags);

	queue_work(hdev->workqueue, &hdev->power_off);
}

void hci_del_off_timer(struct hci_dev *hdev)
{
	BT_DBG("%s", hdev->name);

	clear_bit(HCI_AUTO_OFF, &hdev->flags);
	del_timer(&hdev->off_timer);
=======
	struct hci_dev *hdev = container_of(work, struct hci_dev,
					    power_off.work);
=======
	struct hci_dev *hdev = container_of(work, struct hci_dev, power_off);
>>>>>>> refs/remotes/origin/cm-11.0

	BT_DBG("%s", hdev->name);

	hci_dev_close(hdev->id);
}

static void hci_auto_off(unsigned long data)
{
<<<<<<< HEAD
	struct hci_dev *hdev;

	hdev = container_of(work, struct hci_dev, discov_off.work);

	BT_DBG("%s", hdev->name);

	mgmt_discoverable_timeout(hdev);
>>>>>>> refs/remotes/origin/master
=======
	struct hci_dev *hdev = (struct hci_dev *) data;

	BT_DBG("%s", hdev->name);

	clear_bit(HCI_AUTO_OFF, &hdev->flags);

	queue_work(hdev->workqueue, &hdev->power_off);
}

void hci_del_off_timer(struct hci_dev *hdev)
{
	BT_DBG("%s", hdev->name);

	clear_bit(HCI_AUTO_OFF, &hdev->flags);
	del_timer(&hdev->off_timer);
>>>>>>> refs/remotes/origin/cm-11.0
}

int hci_uuids_clear(struct hci_dev *hdev)
{
<<<<<<< HEAD
	struct list_head *p, *n;

	list_for_each_safe(p, n, &hdev->uuids) {
		struct bt_uuid *uuid;

		uuid = list_entry(p, struct bt_uuid, list);

		list_del(p);
=======
	struct bt_uuid *uuid, *tmp;

	list_for_each_entry_safe(uuid, tmp, &hdev->uuids, list) {
		list_del(&uuid->list);
>>>>>>> refs/remotes/origin/master
		kfree(uuid);
	}

	return 0;
}

int hci_link_keys_clear(struct hci_dev *hdev)
{
	struct list_head *p, *n;

	list_for_each_safe(p, n, &hdev->link_keys) {
		struct link_key *key;

		key = list_entry(p, struct link_key, list);

		list_del(p);
		kfree(key);
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
struct link_key *hci_find_link_key(struct hci_dev *hdev, bdaddr_t *bdaddr)
{
	struct list_head *p;

	list_for_each(p, &hdev->link_keys) {
		struct link_key *k;

		k = list_entry(p, struct link_key, list);

		if (bacmp(bdaddr, &k->bdaddr) == 0)
			return k;
	}
=======
int hci_smp_ltks_clear(struct hci_dev *hdev)
=======
struct link_key *hci_find_link_key(struct hci_dev *hdev, bdaddr_t *bdaddr)
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct list_head *p;

	list_for_each(p, &hdev->link_keys) {
		struct link_key *k;

		k = list_entry(p, struct link_key, list);

		if (bacmp(bdaddr, &k->bdaddr) == 0)
			return k;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
	}
>>>>>>> refs/remotes/origin/cm-11.0

	return NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
struct link_key *hci_find_ltk(struct hci_dev *hdev, __le16 ediv, u8 rand[8])
{
	struct list_head *p;

	list_for_each(p, &hdev->link_keys) {
		struct link_key *k;
		struct key_master_id *id;

		k = list_entry(p, struct link_key, list);

		if (k->key_type != KEY_TYPE_LTK)
			continue;

		if (k->dlen != sizeof(*id))
			continue;

		id = (void *) &k->data;
		if (id->ediv == ediv &&
				(memcmp(rand, id->rand, sizeof(id->rand)) == 0))
			return k;
=======
static bool hci_persistent_key(struct hci_dev *hdev, struct hci_conn *conn,
			       u8 key_type, u8 old_key_type)
=======
struct link_key *hci_find_ltk(struct hci_dev *hdev, __le16 ediv, u8 rand[8])
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct list_head *p;

	list_for_each(p, &hdev->link_keys) {
		struct link_key *k;
		struct key_master_id *id;

		k = list_entry(p, struct link_key, list);

		if (k->key_type != KEY_TYPE_LTK)
			continue;

<<<<<<< HEAD
	list_for_each_entry(k, &hdev->long_term_keys, list) {
		if (k->ediv != ediv ||
		    memcmp(rand, k->rand, sizeof(k->rand)))
			continue;

		return k;
>>>>>>> refs/remotes/origin/master
=======
		if (k->dlen != sizeof(*id))
			continue;

		id = (void *) &k->data;
		if (id->ediv == ediv &&
				(memcmp(rand, id->rand, sizeof(id->rand)) == 0))
			return k;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	return NULL;
}
<<<<<<< HEAD
EXPORT_SYMBOL(hci_find_ltk);

struct link_key *hci_find_link_key_type(struct hci_dev *hdev,
					bdaddr_t *bdaddr, u8 type)
<<<<<<< HEAD
{
	struct list_head *p;

	list_for_each(p, &hdev->link_keys) {
		struct link_key *k;

		k = list_entry(p, struct link_key, list);

		if ((k->key_type == type) && (bacmp(bdaddr, &k->bdaddr) == 0))
			return k;
	}

	return NULL;
}
EXPORT_SYMBOL(hci_find_link_key_type);

int hci_add_link_key(struct hci_dev *hdev, int new_key, bdaddr_t *bdaddr,
						u8 *val, u8 type, u8 pin_len)
{
	struct link_key *key, *old_key;
	struct hci_conn *conn;
	u8 old_key_type;
	u8 bonded = 0;

	old_key = hci_find_link_key(hdev, bdaddr);
	if (old_key) {
		old_key_type = old_key->key_type;
		key = old_key;
	} else {
		old_key_type = 0xff;
=======

struct smp_ltk *hci_find_ltk_by_addr(struct hci_dev *hdev, bdaddr_t *bdaddr,
				     u8 addr_type)
=======
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct list_head *p;

	list_for_each(p, &hdev->link_keys) {
		struct link_key *k;

<<<<<<< HEAD
	list_for_each_entry(k, &hdev->long_term_keys, list)
		if (addr_type == k->bdaddr_type &&
		    bacmp(bdaddr, &k->bdaddr) == 0)
=======
		k = list_entry(p, struct link_key, list);

		if ((k->key_type == type) && (bacmp(bdaddr, &k->bdaddr) == 0))
>>>>>>> refs/remotes/origin/cm-11.0
			return k;
	}

	return NULL;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(hci_find_link_key_type);
>>>>>>> refs/remotes/origin/cm-11.0

int hci_add_link_key(struct hci_dev *hdev, int new_key, bdaddr_t *bdaddr,
						u8 *val, u8 type, u8 pin_len)
{
	struct link_key *key, *old_key;
	struct hci_conn *conn;
	u8 old_key_type;
	u8 bonded = 0;

	old_key = hci_find_link_key(hdev, bdaddr);
	if (old_key) {
		old_key_type = old_key->key_type;
		key = old_key;
	} else {
<<<<<<< HEAD
		old_key_type = conn ? conn->key_type : 0xff;
>>>>>>> refs/remotes/origin/master
=======
		old_key_type = 0xff;
>>>>>>> refs/remotes/origin/cm-11.0
		key = kzalloc(sizeof(*key), GFP_ATOMIC);
		if (!key)
			return -ENOMEM;
		list_add(&key->list, &hdev->link_keys);
	}

<<<<<<< HEAD
	BT_DBG("%s key for %s type %u", hdev->name, batostr(bdaddr), type);

<<<<<<< HEAD
	bacpy(&key->bdaddr, bdaddr);
	memcpy(key->val, val, 16);
	key->auth = 0x01;
	key->key_type = type;
	key->pin_len = pin_len;

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, bdaddr);
	/* Store the link key persistently if one of the following is true:
	 * 1. the remote side is using dedicated bonding since in that case
	 *    also the local requirements are set to dedicated bonding
	 * 2. the local side had dedicated bonding as a requirement
	 * 3. this is a legacy link key
	 * 4. this is a changed combination key and there was a previously
	 *    stored one
	 * If none of the above match only keep the link key around for
	 * this connection and set the temporary flag for the device.
	*/

	if (conn) {
		if ((conn->remote_auth > 0x01) ||
			(conn->auth_initiator && conn->auth_type > 0x01) ||
			(key->key_type < 0x03) ||
			(key->key_type == 0x06 && old_key_type != 0xff))
			bonded = 1;
	}

	if (new_key)
		mgmt_new_key(hdev->id, key, bonded);

	if (type == 0x06)
		key->key_type = old_key_type;
=======
	BT_DBG("%s key for %pMR type %u", hdev->name, bdaddr, type);

	/* Some buggy controller combinations generate a changed
	 * combination key for legacy pairing even when there's no
	 * previous key */
	if (type == HCI_LK_CHANGED_COMBINATION &&
	    (!conn || conn->remote_auth == 0xff) && old_key_type == 0xff) {
		type = HCI_LK_COMBINATION;
		if (conn)
			conn->key_type = type;
	}

	bacpy(&key->bdaddr, bdaddr);
	memcpy(key->val, val, HCI_LINK_KEY_SIZE);
=======
	bacpy(&key->bdaddr, bdaddr);
	memcpy(key->val, val, 16);
	key->auth = 0x01;
	key->key_type = type;
>>>>>>> refs/remotes/origin/cm-11.0
	key->pin_len = pin_len;

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, bdaddr);
	/* Store the link key persistently if one of the following is true:
	 * 1. the remote side is using dedicated bonding since in that case
	 *    also the local requirements are set to dedicated bonding
	 * 2. the local side had dedicated bonding as a requirement
	 * 3. this is a legacy link key
	 * 4. this is a changed combination key and there was a previously
	 *    stored one
	 * If none of the above match only keep the link key around for
	 * this connection and set the temporary flag for the device.
	*/

	if (conn) {
		if ((conn->remote_auth > 0x01) ||
			(conn->auth_initiator && conn->auth_type > 0x01) ||
			(key->key_type < 0x03) ||
			(key->key_type == 0x06 && old_key_type != 0xff))
			bonded = 1;
	}

	if (new_key)
		mgmt_new_key(hdev->id, key, bonded);

<<<<<<< HEAD
	if (conn)
		conn->flush_key = !persistent;
>>>>>>> refs/remotes/origin/master
=======
	if (type == 0x06)
		key->key_type = old_key_type;
>>>>>>> refs/remotes/origin/cm-11.0

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
int hci_add_ltk(struct hci_dev *hdev, int new_key, bdaddr_t *bdaddr,
			u8 addr_type, u8 key_size, u8 auth,
			__le16 ediv, u8 rand[8], u8 ltk[16])
{
	struct link_key *key, *old_key;
	struct key_master_id *id;

	BT_DBG("%s Auth: %2.2X addr %s type: %d", hdev->name, auth,
						batostr(bdaddr), addr_type);

	old_key = hci_find_link_key_type(hdev, bdaddr, KEY_TYPE_LTK);
	if (old_key) {
		key = old_key;
	} else {
		key = kzalloc(sizeof(*key) + sizeof(*id), GFP_ATOMIC);
		if (!key)
			return -ENOMEM;
		list_add(&key->list, &hdev->link_keys);
	}

	key->dlen = sizeof(*id);

	bacpy(&key->bdaddr, bdaddr);
	key->addr_type = addr_type;
	memcpy(key->val, ltk, sizeof(key->val));
	key->key_type = KEY_TYPE_LTK;
	key->pin_len = key_size;
	key->auth = auth;

	id = (void *) &key->data;
	id->ediv = ediv;
	memcpy(id->rand, rand, sizeof(id->rand));

	if (new_key)
		mgmt_new_key(hdev->id, key, auth & 0x01);
=======
int hci_add_ltk(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 addr_type, u8 type,
		int new_key, u8 authenticated, u8 tk[16], u8 enc_size, __le16
		ediv, u8 rand[8])
=======
int hci_add_ltk(struct hci_dev *hdev, int new_key, bdaddr_t *bdaddr,
			u8 addr_type, u8 key_size, u8 auth,
			__le16 ediv, u8 rand[8], u8 ltk[16])
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct link_key *key, *old_key;
	struct key_master_id *id;

	BT_DBG("%s Auth: %2.2X addr %s type: %d", hdev->name, auth,
						batostr(bdaddr), addr_type);

	old_key = hci_find_link_key_type(hdev, bdaddr, KEY_TYPE_LTK);
	if (old_key) {
		key = old_key;
	} else {
		key = kzalloc(sizeof(*key) + sizeof(*id), GFP_ATOMIC);
		if (!key)
			return -ENOMEM;
		list_add(&key->list, &hdev->link_keys);
	}

	key->dlen = sizeof(*id);

	bacpy(&key->bdaddr, bdaddr);
	key->addr_type = addr_type;
	memcpy(key->val, ltk, sizeof(key->val));
	key->key_type = KEY_TYPE_LTK;
	key->pin_len = key_size;
	key->auth = auth;

	id = (void *) &key->data;
	id->ediv = ediv;
	memcpy(id->rand, rand, sizeof(id->rand));

	if (new_key)
		mgmt_new_key(hdev->id, key, auth & 0x01);

	return 0;
}

int hci_remove_link_key(struct hci_dev *hdev, bdaddr_t *bdaddr)
{
	struct link_key *key;

	key = hci_find_link_key(hdev, bdaddr);
	if (!key)
		return -ENOENT;

	BT_DBG("%s removing %pMR", hdev->name, bdaddr);

	list_del(&key->list);
	kfree(key);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
int hci_remove_link_key(struct hci_dev *hdev, bdaddr_t *bdaddr)
{
	struct link_key *key;

	key = hci_find_link_key(hdev, bdaddr);
	if (!key)
		return -ENOENT;

	BT_DBG("%s removing %s", hdev->name, batostr(bdaddr));

	list_del(&key->list);
	kfree(key);
=======
int hci_remove_ltk(struct hci_dev *hdev, bdaddr_t *bdaddr)
{
	struct smp_ltk *k, *tmp;

	list_for_each_entry_safe(k, tmp, &hdev->long_term_keys, list) {
		if (bacmp(bdaddr, &k->bdaddr))
			continue;

		BT_DBG("%s removing %pMR", hdev->name, bdaddr);

		list_del(&k->list);
		kfree(k);
	}
>>>>>>> refs/remotes/origin/master

	return 0;
}

=======
>>>>>>> refs/remotes/origin/cm-11.0
/* HCI command timer function */
<<<<<<< HEAD
static void hci_cmd_timer(unsigned long arg)
{
	struct hci_dev *hdev = (void *) arg;

	BT_ERR("%s command tx timeout", hdev->name);
	atomic_set(&hdev->cmd_cnt, 1);
	clear_bit(HCI_RESET, &hdev->flags);
	tasklet_schedule(&hdev->cmd_task);
<<<<<<< HEAD
}

struct oob_data *hci_find_remote_oob_data(struct hci_dev *hdev,
							bdaddr_t *bdaddr)
=======
static void hci_cmd_timeout(unsigned long arg)
{
	struct hci_dev *hdev = (void *) arg;

	if (hdev->sent_cmd) {
		struct hci_command_hdr *sent = (void *) hdev->sent_cmd->data;
		u16 opcode = __le16_to_cpu(sent->opcode);

		BT_ERR("%s command 0x%4.4x tx timeout", hdev->name, opcode);
	} else {
		BT_ERR("%s command tx timeout", hdev->name);
	}

	atomic_set(&hdev->cmd_cnt, 1);
	queue_work(hdev->workqueue, &hdev->cmd_work);
}

struct oob_data *hci_find_remote_oob_data(struct hci_dev *hdev,
					  bdaddr_t *bdaddr)
>>>>>>> refs/remotes/origin/master
=======
}

struct oob_data *hci_find_remote_oob_data(struct hci_dev *hdev,
							bdaddr_t *bdaddr)
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct oob_data *data;

	list_for_each_entry(data, &hdev->remote_oob_data, list)
		if (bacmp(bdaddr, &data->bdaddr) == 0)
			return data;

	return NULL;
}

int hci_remove_remote_oob_data(struct hci_dev *hdev, bdaddr_t *bdaddr)
{
	struct oob_data *data;

	data = hci_find_remote_oob_data(hdev, bdaddr);
	if (!data)
		return -ENOENT;

<<<<<<< HEAD
	BT_DBG("%s removing %s", hdev->name, batostr(bdaddr));
=======
	BT_DBG("%s removing %pMR", hdev->name, bdaddr);
>>>>>>> refs/remotes/origin/master

	list_del(&data->list);
	kfree(data);

	return 0;
}

int hci_remote_oob_data_clear(struct hci_dev *hdev)
{
	struct oob_data *data, *n;

	list_for_each_entry_safe(data, n, &hdev->remote_oob_data, list) {
		list_del(&data->list);
		kfree(data);
	}

	return 0;
}

<<<<<<< HEAD
static void hci_adv_clear(unsigned long arg)
{
	struct hci_dev *hdev = (void *) arg;

	hci_adv_entries_clear(hdev);
}

int hci_adv_entries_clear(struct hci_dev *hdev)
{
	struct list_head *p, *n;

	BT_DBG("");
	write_lock_bh(&hdev->adv_entries_lock);

	list_for_each_safe(p, n, &hdev->adv_entries) {
		struct adv_entry *entry;

		entry = list_entry(p, struct adv_entry, list);

		list_del(p);
		kfree(entry);
	}

	write_unlock_bh(&hdev->adv_entries_lock);

	return 0;
}

struct adv_entry *hci_find_adv_entry(struct hci_dev *hdev, bdaddr_t *bdaddr)
{
	struct list_head *p;
	struct adv_entry *res = NULL;

	BT_DBG("");
	read_lock_bh(&hdev->adv_entries_lock);

	list_for_each(p, &hdev->adv_entries) {
		struct adv_entry *entry;

		entry = list_entry(p, struct adv_entry, list);

		if (bacmp(bdaddr, &entry->bdaddr) == 0) {
			res = entry;
			goto out;
		}
	}
out:
	read_unlock_bh(&hdev->adv_entries_lock);
	return res;
}

static inline int is_connectable_adv(u8 evt_type)
{
	if (evt_type == ADV_IND || evt_type == ADV_DIRECT_IND)
		return 1;

	return 0;
}

int hci_add_remote_oob_data(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 *hash,
								u8 *randomizer)
{
	struct oob_data *data;

	data = hci_find_remote_oob_data(hdev, bdaddr);

	if (!data) {
		data = kmalloc(sizeof(*data), GFP_ATOMIC);
		if (!data)
			return -ENOMEM;

		bacpy(&data->bdaddr, bdaddr);
		list_add(&data->list, &hdev->remote_oob_data);
	}

	memcpy(data->hash, hash, sizeof(data->hash));
	memcpy(data->randomizer, randomizer, sizeof(data->randomizer));

	BT_DBG("%s for %s", hdev->name, batostr(bdaddr));

	return 0;
}

int hci_add_adv_entry(struct hci_dev *hdev,
					struct hci_ev_le_advertising_info *ev)
{
	struct adv_entry *entry;
	u8 flags = 0;
	int i;

	BT_DBG("");

	if (!is_connectable_adv(ev->evt_type))
		return -EINVAL;

	if (ev->data && ev->length) {
		for (i = 0; (i + 2) < ev->length; i++)
			if (ev->data[i+1] == 0x01) {
				flags = ev->data[i+2];
				BT_DBG("flags: %2.2x", flags);
				break;
			} else {
				i += ev->data[i];
			}
	}

	entry = hci_find_adv_entry(hdev, &ev->bdaddr);
	/* Only new entries should be added to adv_entries. So, if
	 * bdaddr was found, don't add it. */
	if (entry) {
		entry->flags = flags;
		return 0;
	}

	entry = kzalloc(sizeof(*entry), GFP_ATOMIC);
	if (!entry)
		return -ENOMEM;

	bacpy(&entry->bdaddr, &ev->bdaddr);
	entry->bdaddr_type = ev->bdaddr_type;
	entry->flags = flags;

	write_lock(&hdev->adv_entries_lock);
	list_add(&entry->list, &hdev->adv_entries);
	write_unlock(&hdev->adv_entries_lock);

	return 0;
}

static struct crypto_blkcipher *alloc_cypher(void)
{
	if (enable_smp)
		return crypto_alloc_blkcipher("ecb(aes)", 0, CRYPTO_ALG_ASYNC);

	return ERR_PTR(-ENOTSUPP);
}

/* Register HCI device */
int hci_register_dev(struct hci_dev *hdev)
{
	struct list_head *head = &hci_dev_list, *p;
	int i, id;

	BT_DBG("%p name %s bus %d owner %p", hdev, hdev->name,
						hdev->bus, hdev->owner);

	if (!hdev->open || !hdev->close || !hdev->destruct)
		return -EINVAL;

	id = (hdev->dev_type == HCI_BREDR) ? 0 : 1;

	write_lock_bh(&hci_dev_list_lock);

	/* Find first available device id */
	list_for_each(p, &hci_dev_list) {
		if (list_entry(p, struct hci_dev, list)->id != id)
			break;
		head = p; id++;
	}

	sprintf(hdev->name, "hci%d", id);
	hdev->id = id;
	list_add(&hdev->list, head);

	atomic_set(&hdev->refcnt, 1);
	spin_lock_init(&hdev->lock);

	hdev->flags = 0;
	hdev->pkt_type  = (HCI_DM1 | HCI_DH1 | HCI_HV1);
	hdev->esco_type = (ESCO_HV1);
	hdev->link_mode = (HCI_LM_ACCEPT);
	hdev->io_capability = 0x03; /* No Input No Output */

	hdev->idle_timeout = 0;
	hdev->sniff_max_interval = 800;
	hdev->sniff_min_interval = 80;

<<<<<<< HEAD
	set_bit(HCI_SETUP, &hdev->flags);

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-10.0
	tasklet_init(&hdev->cmd_task, hci_cmd_task, (unsigned long) hdev);
	tasklet_init(&hdev->rx_task, hci_rx_task, (unsigned long) hdev);
	tasklet_init(&hdev->tx_task, hci_tx_task, (unsigned long) hdev);
=======
int hci_add_remote_oob_data(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 *hash,
			    u8 *randomizer)
=======
static void hci_adv_clear(unsigned long arg)
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct hci_dev *hdev = (void *) arg;

	hci_adv_entries_clear(hdev);
}

int hci_adv_entries_clear(struct hci_dev *hdev)
{
	struct list_head *p, *n;

	BT_DBG("");
	write_lock_bh(&hdev->adv_entries_lock);

	list_for_each_safe(p, n, &hdev->adv_entries) {
		struct adv_entry *entry;

<<<<<<< HEAD
	BT_DBG("%s for %pMR", hdev->name, bdaddr);

	return 0;
}

struct bdaddr_list *hci_blacklist_lookup(struct hci_dev *hdev,
					 bdaddr_t *bdaddr, u8 type)
{
	struct bdaddr_list *b;

	list_for_each_entry(b, &hdev->blacklist, list) {
		if (!bacmp(&b->bdaddr, bdaddr) && b->bdaddr_type == type)
			return b;
	}

	return NULL;
}

int hci_blacklist_clear(struct hci_dev *hdev)
{
	struct list_head *p, *n;

	list_for_each_safe(p, n, &hdev->blacklist) {
		struct bdaddr_list *b = list_entry(p, struct bdaddr_list, list);
=======
		entry = list_entry(p, struct adv_entry, list);
>>>>>>> refs/remotes/origin/cm-11.0

		list_del(p);
		kfree(entry);
	}

	write_unlock_bh(&hdev->adv_entries_lock);

	return 0;
}

struct adv_entry *hci_find_adv_entry(struct hci_dev *hdev, bdaddr_t *bdaddr)
{
	struct list_head *p;
	struct adv_entry *res = NULL;

<<<<<<< HEAD
	if (!bacmp(bdaddr, BDADDR_ANY))
		return -EBADF;

	if (hci_blacklist_lookup(hdev, bdaddr, type))
		return -EEXIST;
=======
	BT_DBG("");
	read_lock_bh(&hdev->adv_entries_lock);

	list_for_each(p, &hdev->adv_entries) {
		struct adv_entry *entry;
>>>>>>> refs/remotes/origin/cm-11.0

		entry = list_entry(p, struct adv_entry, list);

<<<<<<< HEAD
	bacpy(&entry->bdaddr, bdaddr);
	entry->bdaddr_type = type;

	list_add(&entry->list, &hdev->blacklist);

	return mgmt_device_blocked(hdev, bdaddr, type);
=======
		if (bacmp(bdaddr, &entry->bdaddr) == 0) {
			res = entry;
			goto out;
		}
	}
out:
	read_unlock_bh(&hdev->adv_entries_lock);
	return res;
>>>>>>> refs/remotes/origin/cm-11.0
}

static inline int is_connectable_adv(u8 evt_type)
{
<<<<<<< HEAD
	struct bdaddr_list *entry;

	if (!bacmp(bdaddr, BDADDR_ANY))
		return hci_blacklist_clear(hdev);

	entry = hci_blacklist_lookup(hdev, bdaddr, type);
	if (!entry)
		return -ENOENT;

	list_del(&entry->list);
	kfree(entry);
=======
	if (evt_type == ADV_IND || evt_type == ADV_DIRECT_IND)
		return 1;
>>>>>>> refs/remotes/origin/cm-11.0

	return 0;
}

<<<<<<< HEAD
static void inquiry_complete(struct hci_dev *hdev, u8 status)
{
	if (status) {
		BT_ERR("Failed to start inquiry: status %d", status);

		hci_dev_lock(hdev);
		hci_discovery_set_state(hdev, DISCOVERY_STOPPED);
		hci_dev_unlock(hdev);
		return;
	}
}

static void le_scan_disable_work_complete(struct hci_dev *hdev, u8 status)
{
	/* General inquiry access code (GIAC) */
	u8 lap[3] = { 0x33, 0x8b, 0x9e };
	struct hci_request req;
	struct hci_cp_inquiry cp;
	int err;

	if (status) {
		BT_ERR("Failed to disable LE scanning: status %d", status);
		return;
	}

	switch (hdev->discovery.type) {
	case DISCOV_TYPE_LE:
		hci_dev_lock(hdev);
		hci_discovery_set_state(hdev, DISCOVERY_STOPPED);
		hci_dev_unlock(hdev);
		break;

	case DISCOV_TYPE_INTERLEAVED:
		hci_req_init(&req, hdev);

		memset(&cp, 0, sizeof(cp));
		memcpy(&cp.lap, lap, sizeof(cp.lap));
		cp.length = DISCOV_INTERLEAVED_INQUIRY_LEN;
		hci_req_add(&req, HCI_OP_INQUIRY, sizeof(cp), &cp);

		hci_dev_lock(hdev);

		hci_inquiry_cache_flush(hdev);

		err = hci_req_run(&req, inquiry_complete);
		if (err) {
			BT_ERR("Inquiry request failed: err %d", err);
			hci_discovery_set_state(hdev, DISCOVERY_STOPPED);
		}
=======
int hci_add_remote_oob_data(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 *hash,
								u8 *randomizer)
{
	struct oob_data *data;

	data = hci_find_remote_oob_data(hdev, bdaddr);

	if (!data) {
		data = kmalloc(sizeof(*data), GFP_ATOMIC);
		if (!data)
			return -ENOMEM;

		bacpy(&data->bdaddr, bdaddr);
		list_add(&data->list, &hdev->remote_oob_data);
	}

	memcpy(data->hash, hash, sizeof(data->hash));
	memcpy(data->randomizer, randomizer, sizeof(data->randomizer));

	BT_DBG("%s for %s", hdev->name, batostr(bdaddr));

	return 0;
}

int hci_add_adv_entry(struct hci_dev *hdev,
					struct hci_ev_le_advertising_info *ev)
{
	struct adv_entry *entry;
	u8 flags = 0;
	int i;

	BT_DBG("");

	if (!is_connectable_adv(ev->evt_type))
		return -EINVAL;

	if (ev->data && ev->length) {
		for (i = 0; (i + 2) < ev->length; i++)
			if (ev->data[i+1] == 0x01) {
				flags = ev->data[i+2];
				BT_DBG("flags: %2.2x", flags);
				break;
			} else {
				i += ev->data[i];
			}
	}

	entry = hci_find_adv_entry(hdev, &ev->bdaddr);
	/* Only new entries should be added to adv_entries. So, if
	 * bdaddr was found, don't add it. */
	if (entry) {
		entry->flags = flags;
		return 0;
	}

	entry = kzalloc(sizeof(*entry), GFP_ATOMIC);
	if (!entry)
		return -ENOMEM;

	bacpy(&entry->bdaddr, &ev->bdaddr);
	entry->bdaddr_type = ev->bdaddr_type;
	entry->flags = flags;

	write_lock(&hdev->adv_entries_lock);
	list_add(&entry->list, &hdev->adv_entries);
	write_unlock(&hdev->adv_entries_lock);
>>>>>>> refs/remotes/origin/cm-11.0

		hci_dev_unlock(hdev);
		break;
	}
}

<<<<<<< HEAD
static void le_scan_disable_work(struct work_struct *work)
{
	struct hci_dev *hdev = container_of(work, struct hci_dev,
					    le_scan_disable.work);
	struct hci_cp_le_set_scan_enable cp;
	struct hci_request req;
	int err;

	BT_DBG("%s", hdev->name);

	hci_req_init(&req, hdev);

	memset(&cp, 0, sizeof(cp));
	cp.enable = LE_SCAN_DISABLE;
	hci_req_add(&req, HCI_OP_LE_SET_SCAN_ENABLE, sizeof(cp), &cp);

	err = hci_req_run(&req, le_scan_disable_work_complete);
	if (err)
		BT_ERR("Disable LE scanning request failed: err %d", err);
=======
static struct crypto_blkcipher *alloc_cypher(void)
{
	if (enable_smp)
		return crypto_alloc_blkcipher("ecb(aes)", 0, CRYPTO_ALG_ASYNC);

	return ERR_PTR(-ENOTSUPP);
>>>>>>> refs/remotes/origin/cm-11.0
}

/* Alloc HCI device */
struct hci_dev *hci_alloc_dev(void)
{
<<<<<<< HEAD
	struct hci_dev *hdev;

	hdev = kzalloc(sizeof(struct hci_dev), GFP_KERNEL);
	if (!hdev)
		return NULL;

=======
	struct list_head *head = &hci_dev_list, *p;
	int i, id;

	BT_DBG("%p name %s bus %d owner %p", hdev, hdev->name,
						hdev->bus, hdev->owner);

	if (!hdev->open || !hdev->close || !hdev->destruct)
		return -EINVAL;

	id = (hdev->dev_type == HCI_BREDR) ? 0 : 1;

	write_lock_bh(&hci_dev_list_lock);

	/* Find first available device id */
	list_for_each(p, &hci_dev_list) {
		if (list_entry(p, struct hci_dev, list)->id != id)
			break;
		head = p; id++;
	}

	sprintf(hdev->name, "hci%d", id);
	hdev->id = id;
	list_add(&hdev->list, head);

	atomic_set(&hdev->refcnt, 1);
	spin_lock_init(&hdev->lock);

	hdev->flags = 0;
>>>>>>> refs/remotes/origin/cm-11.0
	hdev->pkt_type  = (HCI_DM1 | HCI_DH1 | HCI_HV1);
	hdev->esco_type = (ESCO_HV1);
	hdev->link_mode = (HCI_LM_ACCEPT);
	hdev->num_iac = 0x01;		/* One IAC support is mandatory */
	hdev->io_capability = 0x03;	/* No Input No Output */
	hdev->inq_tx_power = HCI_TX_POWER_INVALID;
	hdev->adv_tx_power = HCI_TX_POWER_INVALID;

	hdev->sniff_max_interval = 800;
	hdev->sniff_min_interval = 80;

<<<<<<< HEAD
	hdev->le_scan_interval = 0x0060;
	hdev->le_scan_window = 0x0030;
	hdev->le_conn_min_interval = 0x0028;
	hdev->le_conn_max_interval = 0x0038;

	mutex_init(&hdev->lock);
	mutex_init(&hdev->req_lock);

	INIT_LIST_HEAD(&hdev->mgmt_pending);
	INIT_LIST_HEAD(&hdev->blacklist);
	INIT_LIST_HEAD(&hdev->uuids);
	INIT_LIST_HEAD(&hdev->link_keys);
	INIT_LIST_HEAD(&hdev->long_term_keys);
	INIT_LIST_HEAD(&hdev->remote_oob_data);
	INIT_LIST_HEAD(&hdev->conn_hash.list);

	INIT_WORK(&hdev->rx_work, hci_rx_work);
	INIT_WORK(&hdev->cmd_work, hci_cmd_work);
	INIT_WORK(&hdev->tx_work, hci_tx_work);
	INIT_WORK(&hdev->power_on, hci_power_on);

	INIT_DELAYED_WORK(&hdev->power_off, hci_power_off);
	INIT_DELAYED_WORK(&hdev->discov_off, hci_discov_off);
	INIT_DELAYED_WORK(&hdev->le_scan_disable, le_scan_disable_work);
>>>>>>> refs/remotes/origin/master
=======
	set_bit(HCI_SETUP, &hdev->flags);

	tasklet_init(&hdev->cmd_task, hci_cmd_task, (unsigned long) hdev);
	tasklet_init(&hdev->rx_task, hci_rx_task, (unsigned long) hdev);
	tasklet_init(&hdev->tx_task, hci_tx_task, (unsigned long) hdev);
>>>>>>> refs/remotes/origin/cm-11.0

	skb_queue_head_init(&hdev->rx_q);
	skb_queue_head_init(&hdev->cmd_q);
	skb_queue_head_init(&hdev->raw_q);

<<<<<<< HEAD
	setup_timer(&hdev->cmd_timer, hci_cmd_timer, (unsigned long) hdev);
	setup_timer(&hdev->disco_timer, mgmt_disco_timeout,
						(unsigned long) hdev);
	setup_timer(&hdev->disco_le_timer, mgmt_disco_le_timeout,
						(unsigned long) hdev);

	for (i = 0; i < NUM_REASSEMBLY; i++)
		hdev->reassembly[i] = NULL;

	init_waitqueue_head(&hdev->req_wait_q);
	mutex_init(&hdev->req_lock);

	inquiry_cache_init(hdev);

	hci_conn_hash_init(hdev);
	hci_chan_list_init(hdev);

	INIT_LIST_HEAD(&hdev->blacklist);

	INIT_LIST_HEAD(&hdev->uuids);

	INIT_LIST_HEAD(&hdev->link_keys);

	INIT_LIST_HEAD(&hdev->remote_oob_data);

	INIT_LIST_HEAD(&hdev->adv_entries);
	rwlock_init(&hdev->adv_entries_lock);
	setup_timer(&hdev->adv_timer, hci_adv_clear, (unsigned long) hdev);

	INIT_WORK(&hdev->power_on, hci_power_on);
	INIT_WORK(&hdev->power_off, hci_power_off);
	setup_timer(&hdev->off_timer, hci_auto_off, (unsigned long) hdev);

	memset(&hdev->stat, 0, sizeof(struct hci_dev_stats));

	atomic_set(&hdev->promisc, 0);

	write_unlock_bh(&hci_dev_list_lock);
<<<<<<< HEAD

	hdev->workqueue = create_singlethread_workqueue(hdev->name);
	if (!hdev->workqueue)
		goto nomem;

	hdev->tfm = alloc_cypher();
	if (IS_ERR(hdev->tfm))
		BT_INFO("Failed to load transform for ecb(aes): %ld",
							PTR_ERR(hdev->tfm));

	hci_register_sysfs(hdev);

	hdev->rfkill = rfkill_alloc(hdev->name, &hdev->dev,
				RFKILL_TYPE_BLUETOOTH, &hci_rfkill_ops, hdev);
=======
	init_waitqueue_head(&hdev->req_wait_q);

	setup_timer(&hdev->cmd_timer, hci_cmd_timeout, (unsigned long) hdev);

	hci_init_sysfs(hdev);
	discovery_init(hdev);

	return hdev;
}
EXPORT_SYMBOL(hci_alloc_dev);

/* Free HCI device */
void hci_free_dev(struct hci_dev *hdev)
{
	/* will free via device release */
	put_device(&hdev->dev);
}
EXPORT_SYMBOL(hci_free_dev);

/* Register HCI device */
int hci_register_dev(struct hci_dev *hdev)
{
	int id, error;

	if (!hdev->open || !hdev->close)
		return -EINVAL;

	/* Do not allow HCI_AMP devices to register at index 0,
	 * so the index can be used as the AMP controller ID.
	 */
	switch (hdev->dev_type) {
	case HCI_BREDR:
		id = ida_simple_get(&hci_index_ida, 0, 0, GFP_KERNEL);
		break;
	case HCI_AMP:
		id = ida_simple_get(&hci_index_ida, 1, 0, GFP_KERNEL);
		break;
	default:
		return -EINVAL;
	}

	if (id < 0)
		return id;

	sprintf(hdev->name, "hci%d", id);
	hdev->id = id;

	BT_DBG("%p name %s bus %d", hdev, hdev->name, hdev->bus);

	hdev->workqueue = alloc_workqueue("%s", WQ_HIGHPRI | WQ_UNBOUND |
					  WQ_MEM_RECLAIM, 1, hdev->name);
	if (!hdev->workqueue) {
		error = -ENOMEM;
		goto err;
	}

	hdev->req_workqueue = alloc_workqueue("%s", WQ_HIGHPRI | WQ_UNBOUND |
					      WQ_MEM_RECLAIM, 1, hdev->name);
	if (!hdev->req_workqueue) {
		destroy_workqueue(hdev->workqueue);
		error = -ENOMEM;
		goto err;
	}

	if (!IS_ERR_OR_NULL(bt_debugfs))
		hdev->debugfs = debugfs_create_dir(hdev->name, bt_debugfs);

	dev_set_name(&hdev->dev, "%s", hdev->name);

	error = device_add(&hdev->dev);
	if (error < 0)
		goto err_wqueue;
=======

	hdev->workqueue = create_singlethread_workqueue(hdev->name);
	if (!hdev->workqueue)
		goto nomem;

	hdev->tfm = alloc_cypher();
	if (IS_ERR(hdev->tfm))
		BT_INFO("Failed to load transform for ecb(aes): %ld",
							PTR_ERR(hdev->tfm));

	hci_register_sysfs(hdev);
>>>>>>> refs/remotes/origin/cm-11.0

	hdev->rfkill = rfkill_alloc(hdev->name, &hdev->dev,
				    RFKILL_TYPE_BLUETOOTH, &hci_rfkill_ops,
				    hdev);
>>>>>>> refs/remotes/origin/master
	if (hdev->rfkill) {
		if (rfkill_register(hdev->rfkill) < 0) {
			rfkill_destroy(hdev->rfkill);
			hdev->rfkill = NULL;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	set_bit(HCI_AUTO_OFF, &hdev->flags);
<<<<<<< HEAD
=======
	set_bit(HCI_SETUP, &hdev->flags);
>>>>>>> refs/remotes/origin/cm-10.0
	queue_work(hdev->workqueue, &hdev->power_on);

	hci_notify(hdev, HCI_DEV_REG);

	return id;

nomem:
	write_lock_bh(&hci_dev_list_lock);
	list_del(&hdev->list);
	write_unlock_bh(&hci_dev_list_lock);

	return -ENOMEM;
=======
	if (hdev->rfkill && rfkill_blocked(hdev->rfkill))
		set_bit(HCI_RFKILLED, &hdev->dev_flags);

	set_bit(HCI_SETUP, &hdev->dev_flags);
	set_bit(HCI_AUTO_OFF, &hdev->dev_flags);

	if (hdev->dev_type == HCI_BREDR) {
		/* Assume BR/EDR support until proven otherwise (such as
		 * through reading supported features during init.
		 */
		set_bit(HCI_BREDR_ENABLED, &hdev->dev_flags);
	}

	write_lock(&hci_dev_list_lock);
	list_add(&hdev->list, &hci_dev_list);
	write_unlock(&hci_dev_list_lock);
=======
	set_bit(HCI_AUTO_OFF, &hdev->flags);
	queue_work(hdev->workqueue, &hdev->power_on);
>>>>>>> refs/remotes/origin/cm-11.0

	hci_notify(hdev, HCI_DEV_REG);

	queue_work(hdev->req_workqueue, &hdev->power_on);

	return id;

<<<<<<< HEAD
err_wqueue:
	destroy_workqueue(hdev->workqueue);
	destroy_workqueue(hdev->req_workqueue);
err:
	ida_simple_remove(&hci_index_ida, hdev->id);

	return error;
>>>>>>> refs/remotes/origin/master
=======
nomem:
	write_lock_bh(&hci_dev_list_lock);
	list_del(&hdev->list);
	write_unlock_bh(&hci_dev_list_lock);

	return -ENOMEM;
>>>>>>> refs/remotes/origin/cm-11.0
}
EXPORT_SYMBOL(hci_register_dev);

/* Unregister HCI device */
<<<<<<< HEAD
<<<<<<< HEAD
int hci_unregister_dev(struct hci_dev *hdev)
{
	int i;

	BT_DBG("%p name %s bus %d", hdev, hdev->name, hdev->bus);

	write_lock_bh(&hci_dev_list_lock);
	list_del(&hdev->list);
	write_unlock_bh(&hci_dev_list_lock);

	hci_dev_do_close(hdev, hdev->bus == HCI_SMD);
=======
void hci_unregister_dev(struct hci_dev *hdev)
=======
int hci_unregister_dev(struct hci_dev *hdev)
>>>>>>> refs/remotes/origin/cm-11.0
{
	int i, id;

	BT_DBG("%p name %s bus %d", hdev, hdev->name, hdev->bus);

<<<<<<< HEAD
	set_bit(HCI_UNREGISTER, &hdev->dev_flags);

	id = hdev->id;

	write_lock(&hci_dev_list_lock);
=======
	write_lock_bh(&hci_dev_list_lock);
>>>>>>> refs/remotes/origin/cm-11.0
	list_del(&hdev->list);
	write_unlock_bh(&hci_dev_list_lock);

<<<<<<< HEAD
	hci_dev_do_close(hdev);
>>>>>>> refs/remotes/origin/master
=======
	hci_dev_do_close(hdev, hdev->bus == HCI_SMD);
>>>>>>> refs/remotes/origin/cm-11.0

	for (i = 0; i < NUM_REASSEMBLY; i++)
		kfree_skb(hdev->reassembly[i]);

<<<<<<< HEAD
	if (!test_bit(HCI_INIT, &hdev->flags) &&
				!test_bit(HCI_SETUP, &hdev->flags) &&
				hdev->dev_type == HCI_BREDR) {
		hci_dev_lock_bh(hdev);
		mgmt_index_removed(hdev->id);
		hci_dev_unlock_bh(hdev);
	}

	if (!IS_ERR(hdev->tfm))
		crypto_free_blkcipher(hdev->tfm);
=======
	cancel_work_sync(&hdev->power_on);

	if (!test_bit(HCI_INIT, &hdev->flags) &&
<<<<<<< HEAD
	    !test_bit(HCI_SETUP, &hdev->dev_flags)) {
		hci_dev_lock(hdev);
		mgmt_index_removed(hdev);
		hci_dev_unlock(hdev);
	}

	/* mgmt_index_removed should take care of emptying the
	 * pending list */
	BUG_ON(!list_empty(&hdev->mgmt_pending));
>>>>>>> refs/remotes/origin/master
=======
				!test_bit(HCI_SETUP, &hdev->flags) &&
				hdev->dev_type == HCI_BREDR) {
		hci_dev_lock_bh(hdev);
		mgmt_index_removed(hdev->id);
		hci_dev_unlock_bh(hdev);
	}

	if (!IS_ERR(hdev->tfm))
		crypto_free_blkcipher(hdev->tfm);
>>>>>>> refs/remotes/origin/cm-11.0

	hci_notify(hdev, HCI_DEV_UNREG);

	if (hdev->rfkill) {
		rfkill_unregister(hdev->rfkill);
		rfkill_destroy(hdev->rfkill);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	hci_unregister_sysfs(hdev);

	/* Disable all timers */
	hci_del_off_timer(hdev);
	del_timer(&hdev->adv_timer);
	del_timer(&hdev->cmd_timer);
	del_timer(&hdev->disco_timer);
	del_timer(&hdev->disco_le_timer);

	destroy_workqueue(hdev->workqueue);

	hci_dev_lock_bh(hdev);
	hci_blacklist_clear(hdev);
	hci_uuids_clear(hdev);
	hci_link_keys_clear(hdev);
	hci_remote_oob_data_clear(hdev);
	hci_adv_entries_clear(hdev);
	hci_dev_unlock_bh(hdev);

	__hci_dev_put(hdev);

	return 0;
=======
	device_del(&hdev->dev);

	debugfs_remove_recursive(hdev->debugfs);
=======
	hci_unregister_sysfs(hdev);

	/* Disable all timers */
	hci_del_off_timer(hdev);
	del_timer(&hdev->adv_timer);
	del_timer(&hdev->cmd_timer);
	del_timer(&hdev->disco_timer);
	del_timer(&hdev->disco_le_timer);
>>>>>>> refs/remotes/origin/cm-11.0

	destroy_workqueue(hdev->workqueue);
	destroy_workqueue(hdev->req_workqueue);

	hci_dev_lock_bh(hdev);
	hci_blacklist_clear(hdev);
	hci_uuids_clear(hdev);
	hci_link_keys_clear(hdev);
	hci_remote_oob_data_clear(hdev);
<<<<<<< HEAD
	hci_dev_unlock(hdev);

	hci_dev_put(hdev);

	ida_simple_remove(&hci_index_ida, id);
>>>>>>> refs/remotes/origin/master
=======
	hci_adv_entries_clear(hdev);
	hci_dev_unlock_bh(hdev);

	__hci_dev_put(hdev);

	return 0;
>>>>>>> refs/remotes/origin/cm-11.0
}
EXPORT_SYMBOL(hci_unregister_dev);

/* Suspend HCI device */
int hci_suspend_dev(struct hci_dev *hdev)
{
	hci_notify(hdev, HCI_DEV_SUSPEND);
	return 0;
}
EXPORT_SYMBOL(hci_suspend_dev);

/* Resume HCI device */
int hci_resume_dev(struct hci_dev *hdev)
{
	hci_notify(hdev, HCI_DEV_RESUME);
	return 0;
}
EXPORT_SYMBOL(hci_resume_dev);

/* Receive frame from HCI drivers */
<<<<<<< HEAD
int hci_recv_frame(struct sk_buff *skb)
{
	struct hci_dev *hdev = (struct hci_dev *) skb->dev;
	if (!hdev || (!test_bit(HCI_UP, &hdev->flags)
				&& !test_bit(HCI_INIT, &hdev->flags))) {
=======
int hci_recv_frame(struct hci_dev *hdev, struct sk_buff *skb)
{
	if (!hdev || (!test_bit(HCI_UP, &hdev->flags)
		      && !test_bit(HCI_INIT, &hdev->flags))) {
>>>>>>> refs/remotes/origin/master
		kfree_skb(skb);
		return -ENXIO;
	}

<<<<<<< HEAD
	/* Incomming skb */
=======
	/* Incoming skb */
>>>>>>> refs/remotes/origin/master
	bt_cb(skb)->incoming = 1;

	/* Time stamp */
	__net_timestamp(skb);

<<<<<<< HEAD
<<<<<<< HEAD
	/* Queue frame for rx task */
	skb_queue_tail(&hdev->rx_q, skb);
	tasklet_schedule(&hdev->rx_task);
=======
	skb_queue_tail(&hdev->rx_q, skb);
	queue_work(hdev->workqueue, &hdev->rx_work);
>>>>>>> refs/remotes/origin/master
=======
	/* Queue frame for rx task */
	skb_queue_tail(&hdev->rx_q, skb);
	tasklet_schedule(&hdev->rx_task);
>>>>>>> refs/remotes/origin/cm-11.0

	return 0;
}
EXPORT_SYMBOL(hci_recv_frame);

static int hci_reassembly(struct hci_dev *hdev, int type, void *data,
<<<<<<< HEAD
						  int count, __u8 index)
=======
			  int count, __u8 index)
>>>>>>> refs/remotes/origin/master
{
	int len = 0;
	int hlen = 0;
	int remain = count;
	struct sk_buff *skb;
	struct bt_skb_cb *scb;

	if ((type < HCI_ACLDATA_PKT || type > HCI_EVENT_PKT) ||
<<<<<<< HEAD
				index >= NUM_REASSEMBLY)
=======
	    index >= NUM_REASSEMBLY)
>>>>>>> refs/remotes/origin/master
		return -EILSEQ;

	skb = hdev->reassembly[index];

	if (!skb) {
		switch (type) {
		case HCI_ACLDATA_PKT:
			len = HCI_MAX_FRAME_SIZE;
			hlen = HCI_ACL_HDR_SIZE;
			break;
		case HCI_EVENT_PKT:
			len = HCI_MAX_EVENT_SIZE;
			hlen = HCI_EVENT_HDR_SIZE;
			break;
		case HCI_SCODATA_PKT:
			len = HCI_MAX_SCO_SIZE;
			hlen = HCI_SCO_HDR_SIZE;
			break;
		}

		skb = bt_skb_alloc(len, GFP_ATOMIC);
		if (!skb)
			return -ENOMEM;

		scb = (void *) skb->cb;
		scb->expect = hlen;
		scb->pkt_type = type;

<<<<<<< HEAD
		skb->dev = (void *) hdev;
=======
>>>>>>> refs/remotes/origin/master
		hdev->reassembly[index] = skb;
	}

	while (count) {
		scb = (void *) skb->cb;
<<<<<<< HEAD
<<<<<<< HEAD
		len = min(scb->expect, (__u16)count);
=======
		len = min_t(uint, scb->expect, count);
>>>>>>> refs/remotes/origin/master
=======
		len = min(scb->expect, (__u16)count);
>>>>>>> refs/remotes/origin/cm-11.0

		memcpy(skb_put(skb, len), data, len);

		count -= len;
		data += len;
		scb->expect -= len;
		remain = count;

		switch (type) {
		case HCI_EVENT_PKT:
			if (skb->len == HCI_EVENT_HDR_SIZE) {
				struct hci_event_hdr *h = hci_event_hdr(skb);
				scb->expect = h->plen;

				if (skb_tailroom(skb) < scb->expect) {
					kfree_skb(skb);
					hdev->reassembly[index] = NULL;
					return -ENOMEM;
				}
			}
			break;

		case HCI_ACLDATA_PKT:
			if (skb->len  == HCI_ACL_HDR_SIZE) {
				struct hci_acl_hdr *h = hci_acl_hdr(skb);
				scb->expect = __le16_to_cpu(h->dlen);

				if (skb_tailroom(skb) < scb->expect) {
					kfree_skb(skb);
					hdev->reassembly[index] = NULL;
					return -ENOMEM;
				}
			}
			break;

		case HCI_SCODATA_PKT:
			if (skb->len == HCI_SCO_HDR_SIZE) {
				struct hci_sco_hdr *h = hci_sco_hdr(skb);
				scb->expect = h->dlen;

				if (skb_tailroom(skb) < scb->expect) {
					kfree_skb(skb);
					hdev->reassembly[index] = NULL;
					return -ENOMEM;
				}
			}
			break;
		}

		if (scb->expect == 0) {
			/* Complete frame */

			bt_cb(skb)->pkt_type = type;
<<<<<<< HEAD
			hci_recv_frame(skb);
=======
			hci_recv_frame(hdev, skb);
>>>>>>> refs/remotes/origin/master

			hdev->reassembly[index] = NULL;
			return remain;
		}
	}

	return remain;
}

int hci_recv_fragment(struct hci_dev *hdev, int type, void *data, int count)
{
	int rem = 0;

	if (type < HCI_ACLDATA_PKT || type > HCI_EVENT_PKT)
		return -EILSEQ;

	while (count) {
		rem = hci_reassembly(hdev, type, data, count, type - 1);
		if (rem < 0)
			return rem;

		data += (count - rem);
		count = rem;
<<<<<<< HEAD
<<<<<<< HEAD
	};
=======
	}
>>>>>>> refs/remotes/origin/master
=======
	};
>>>>>>> refs/remotes/origin/cm-11.0

	return rem;
}
EXPORT_SYMBOL(hci_recv_fragment);

#define STREAM_REASSEMBLY 0

int hci_recv_stream_fragment(struct hci_dev *hdev, void *data, int count)
{
	int type;
	int rem = 0;

	while (count) {
		struct sk_buff *skb = hdev->reassembly[STREAM_REASSEMBLY];

		if (!skb) {
			struct { char type; } *pkt;

			/* Start of the frame */
			pkt = data;
			type = pkt->type;

			data++;
			count--;
		} else
			type = bt_cb(skb)->pkt_type;

		rem = hci_reassembly(hdev, type, data, count,
<<<<<<< HEAD
							STREAM_REASSEMBLY);
=======
				     STREAM_REASSEMBLY);
>>>>>>> refs/remotes/origin/master
		if (rem < 0)
			return rem;

		data += (count - rem);
		count = rem;
<<<<<<< HEAD
<<<<<<< HEAD
	};
=======
	}
>>>>>>> refs/remotes/origin/master
=======
	};
>>>>>>> refs/remotes/origin/cm-11.0

	return rem;
}
EXPORT_SYMBOL(hci_recv_stream_fragment);

/* ---- Interface to upper protocols ---- */

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
/* Register/Unregister protocols.
 * hci_task_lock is used to ensure that no tasks are running. */
int hci_register_proto(struct hci_proto *hp)
{
	int err = 0;

	BT_DBG("%p name %s id %d", hp, hp->name, hp->id);

	if (hp->id >= HCI_MAX_PROTO)
		return -EINVAL;

	write_lock_bh(&hci_task_lock);

	if (!hci_proto[hp->id])
		hci_proto[hp->id] = hp;
	else
		err = -EEXIST;

	write_unlock_bh(&hci_task_lock);

	return err;
}
EXPORT_SYMBOL(hci_register_proto);

int hci_unregister_proto(struct hci_proto *hp)
{
	int err = 0;

	BT_DBG("%p name %s id %d", hp, hp->name, hp->id);

	if (hp->id >= HCI_MAX_PROTO)
		return -EINVAL;

	write_lock_bh(&hci_task_lock);

	if (hci_proto[hp->id])
		hci_proto[hp->id] = NULL;
	else
		err = -ENOENT;

	write_unlock_bh(&hci_task_lock);

	return err;
}
EXPORT_SYMBOL(hci_unregister_proto);

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
int hci_register_cb(struct hci_cb *cb)
{
	BT_DBG("%p name %s", cb, cb->name);

<<<<<<< HEAD
<<<<<<< HEAD
	write_lock_bh(&hci_cb_list_lock);
	list_add(&cb->list, &hci_cb_list);
	write_unlock_bh(&hci_cb_list_lock);
=======
	write_lock(&hci_cb_list_lock);
	list_add(&cb->list, &hci_cb_list);
	write_unlock(&hci_cb_list_lock);
>>>>>>> refs/remotes/origin/master
=======
	write_lock_bh(&hci_cb_list_lock);
	list_add(&cb->list, &hci_cb_list);
	write_unlock_bh(&hci_cb_list_lock);
>>>>>>> refs/remotes/origin/cm-11.0

	return 0;
}
EXPORT_SYMBOL(hci_register_cb);

int hci_unregister_cb(struct hci_cb *cb)
{
	BT_DBG("%p name %s", cb, cb->name);

<<<<<<< HEAD
<<<<<<< HEAD
	write_lock_bh(&hci_cb_list_lock);
	list_del(&cb->list);
	write_unlock_bh(&hci_cb_list_lock);
=======
	write_lock(&hci_cb_list_lock);
	list_del(&cb->list);
	write_unlock(&hci_cb_list_lock);
>>>>>>> refs/remotes/origin/master
=======
	write_lock_bh(&hci_cb_list_lock);
	list_del(&cb->list);
	write_unlock_bh(&hci_cb_list_lock);
>>>>>>> refs/remotes/origin/cm-11.0

	return 0;
}
EXPORT_SYMBOL(hci_unregister_cb);

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
int hci_register_amp(struct amp_mgr_cb *cb)
{
	BT_DBG("%p", cb);

	write_lock_bh(&amp_mgr_cb_list_lock);
	list_add(&cb->list, &amp_mgr_cb_list);
	write_unlock_bh(&amp_mgr_cb_list_lock);

	return 0;
}
EXPORT_SYMBOL(hci_register_amp);

int hci_unregister_amp(struct amp_mgr_cb *cb)
{
	BT_DBG("%p", cb);

	write_lock_bh(&amp_mgr_cb_list_lock);
	list_del(&cb->list);
	write_unlock_bh(&amp_mgr_cb_list_lock);

	return 0;
}
EXPORT_SYMBOL(hci_unregister_amp);

void hci_amp_cmd_complete(struct hci_dev *hdev, __u16 opcode,
			struct sk_buff *skb)
{
	struct amp_mgr_cb *cb;

	BT_DBG("opcode 0x%x", opcode);

	read_lock_bh(&amp_mgr_cb_list_lock);
	list_for_each_entry(cb, &amp_mgr_cb_list, list) {
		if (cb->amp_cmd_complete_event)
			cb->amp_cmd_complete_event(hdev, opcode, skb);
	}
	read_unlock_bh(&amp_mgr_cb_list_lock);
}

void hci_amp_cmd_status(struct hci_dev *hdev, __u16 opcode, __u8 status)
{
	struct amp_mgr_cb *cb;

	BT_DBG("opcode 0x%x, status %d", opcode, status);

	read_lock_bh(&amp_mgr_cb_list_lock);
	list_for_each_entry(cb, &amp_mgr_cb_list, list) {
		if (cb->amp_cmd_status_event)
			cb->amp_cmd_status_event(hdev, opcode, status);
	}
	read_unlock_bh(&amp_mgr_cb_list_lock);
}

void hci_amp_event_packet(struct hci_dev *hdev, __u8 ev_code,
			struct sk_buff *skb)
{
	struct amp_mgr_cb *cb;

	BT_DBG("ev_code 0x%x", ev_code);

	read_lock_bh(&amp_mgr_cb_list_lock);
	list_for_each_entry(cb, &amp_mgr_cb_list, list) {
		if (cb->amp_event)
			cb->amp_event(hdev, ev_code, skb);
	}
	read_unlock_bh(&amp_mgr_cb_list_lock);
}

static int hci_send_frame(struct sk_buff *skb)
{
	struct hci_dev *hdev = (struct hci_dev *) skb->dev;

	if (!hdev) {
		kfree_skb(skb);
		return -ENODEV;
	}

	BT_DBG("%s type %d len %d", hdev->name, bt_cb(skb)->pkt_type, skb->len);

<<<<<<< HEAD
	if (atomic_read(&hdev->promisc)) {
		/* Time stamp */
		__net_timestamp(skb);

		hci_send_to_sock(hdev, skb, NULL);
	}

	/* Get rid of skb owner, prior to sending to the driver. */
	skb_orphan(skb);

	hci_notify(hdev, HCI_DEV_WRITE);
	return hdev->send(skb);
}

/* Send HCI command */
int hci_send_cmd(struct hci_dev *hdev, __u16 opcode, __u32 plen, void *param)
{
	int len = HCI_COMMAND_HDR_SIZE + plen;
	struct hci_command_hdr *hdr;
	struct sk_buff *skb;

	BT_DBG("%s opcode 0x%x plen %d", hdev->name, opcode, plen);

	skb = bt_skb_alloc(len, GFP_ATOMIC);
=======
static void hci_send_frame(struct hci_dev *hdev, struct sk_buff *skb)
{
	BT_DBG("%s type %d len %d", hdev->name, bt_cb(skb)->pkt_type, skb->len);

	/* Time stamp */
	__net_timestamp(skb);

	/* Send copy to monitor */
	hci_send_to_monitor(hdev, skb);

=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (atomic_read(&hdev->promisc)) {
		/* Time stamp */
		__net_timestamp(skb);

		hci_send_to_sock(hdev, skb, NULL);
	}

	/* Get rid of skb owner, prior to sending to the driver. */
	skb_orphan(skb);

<<<<<<< HEAD
	if (hdev->send(hdev, skb) < 0)
		BT_ERR("%s sending frame failed", hdev->name);
=======
	hci_notify(hdev, HCI_DEV_WRITE);
	return hdev->send(skb);
>>>>>>> refs/remotes/origin/cm-11.0
}

void hci_req_init(struct hci_request *req, struct hci_dev *hdev)
{
	skb_queue_head_init(&req->cmd_q);
	req->hdev = hdev;
	req->err = 0;
}

int hci_req_run(struct hci_request *req, hci_req_complete_t complete)
{
	struct hci_dev *hdev = req->hdev;
	struct sk_buff *skb;
	unsigned long flags;

	BT_DBG("length %u", skb_queue_len(&req->cmd_q));

	/* If an error occured during request building, remove all HCI
	 * commands queued on the HCI request queue.
	 */
	if (req->err) {
		skb_queue_purge(&req->cmd_q);
		return req->err;
	}

	/* Do not allow empty requests */
	if (skb_queue_empty(&req->cmd_q))
		return -ENODATA;

	skb = skb_peek_tail(&req->cmd_q);
	bt_cb(skb)->req.complete = complete;

	spin_lock_irqsave(&hdev->cmd_q.lock, flags);
	skb_queue_splice_tail(&req->cmd_q, &hdev->cmd_q);
	spin_unlock_irqrestore(&hdev->cmd_q.lock, flags);

	queue_work(hdev->workqueue, &hdev->cmd_work);

	return 0;
}

static struct sk_buff *hci_prepare_cmd(struct hci_dev *hdev, u16 opcode,
				       u32 plen, const void *param)
{
	int len = HCI_COMMAND_HDR_SIZE + plen;
	struct hci_command_hdr *hdr;
	struct sk_buff *skb;

	skb = bt_skb_alloc(len, GFP_ATOMIC);
	if (!skb)
		return NULL;

	hdr = (struct hci_command_hdr *) skb_put(skb, HCI_COMMAND_HDR_SIZE);
	hdr->opcode = cpu_to_le16(opcode);
	hdr->plen   = plen;

	if (plen)
		memcpy(skb_put(skb, plen), param, plen);

	BT_DBG("skb len %d", skb->len);

	bt_cb(skb)->pkt_type = HCI_COMMAND_PKT;

	return skb;
}

/* Send HCI command */
int hci_send_cmd(struct hci_dev *hdev, __u16 opcode, __u32 plen,
		 const void *param)
{
	struct sk_buff *skb;

	BT_DBG("%s opcode 0x%4.4x plen %d", hdev->name, opcode, plen);

	skb = hci_prepare_cmd(hdev, opcode, plen, param);
>>>>>>> refs/remotes/origin/master
	if (!skb) {
		BT_ERR("%s no memory for command", hdev->name);
		return -ENOMEM;
	}

<<<<<<< HEAD
	hdr = (struct hci_command_hdr *) skb_put(skb, HCI_COMMAND_HDR_SIZE);
	hdr->opcode = cpu_to_le16(opcode);
	hdr->plen   = plen;

	if (plen)
		memcpy(skb_put(skb, plen), param, plen);

	BT_DBG("skb len %d", skb->len);

	bt_cb(skb)->pkt_type = HCI_COMMAND_PKT;
	skb->dev = (void *) hdev;

	if (test_bit(HCI_INIT, &hdev->flags))
		hdev->init_last_cmd = opcode;

	skb_queue_tail(&hdev->cmd_q, skb);
	tasklet_schedule(&hdev->cmd_task);
<<<<<<< HEAD

	return 0;
}
EXPORT_SYMBOL(hci_send_cmd);
=======
	/* Stand-alone HCI commands must be flaged as
	 * single-command requests.
	 */
	bt_cb(skb)->req.start = true;

	skb_queue_tail(&hdev->cmd_q, skb);
	queue_work(hdev->workqueue, &hdev->cmd_work);
=======
>>>>>>> refs/remotes/origin/cm-11.0

	return 0;
}
EXPORT_SYMBOL(hci_send_cmd);

/* Queue a command to an asynchronous HCI request */
void hci_req_add_ev(struct hci_request *req, u16 opcode, u32 plen,
		    const void *param, u8 event)
{
	struct hci_dev *hdev = req->hdev;
	struct sk_buff *skb;

	BT_DBG("%s opcode 0x%4.4x plen %d", hdev->name, opcode, plen);

	/* If an error occured during request building, there is no point in
	 * queueing the HCI command. We can simply return.
	 */
	if (req->err)
		return;

	skb = hci_prepare_cmd(hdev, opcode, plen, param);
	if (!skb) {
		BT_ERR("%s no memory for command (opcode 0x%4.4x)",
		       hdev->name, opcode);
		req->err = -ENOMEM;
		return;
	}

	if (skb_queue_empty(&req->cmd_q))
		bt_cb(skb)->req.start = true;

	bt_cb(skb)->req.event = event;

	skb_queue_tail(&req->cmd_q, skb);
}

void hci_req_add(struct hci_request *req, u16 opcode, u32 plen,
		 const void *param)
{
	hci_req_add_ev(req, opcode, plen, param, 0);
}
>>>>>>> refs/remotes/origin/master

/* Get data from the previously sent command */
void *hci_sent_cmd_data(struct hci_dev *hdev, __u16 opcode)
{
	struct hci_command_hdr *hdr;

	if (!hdev->sent_cmd)
		return NULL;

	hdr = (void *) hdev->sent_cmd->data;

	if (hdr->opcode != cpu_to_le16(opcode))
		return NULL;

<<<<<<< HEAD
	BT_DBG("%s opcode 0x%x", hdev->name, opcode);
=======
	BT_DBG("%s opcode 0x%4.4x", hdev->name, opcode);
>>>>>>> refs/remotes/origin/master

	return hdev->sent_cmd->data + HCI_COMMAND_HDR_SIZE;
}

/* Send ACL data */
static void hci_add_acl_hdr(struct sk_buff *skb, __u16 handle, __u16 flags)
{
	struct hci_acl_hdr *hdr;
	int len = skb->len;

	skb_push(skb, HCI_ACL_HDR_SIZE);
	skb_reset_transport_header(skb);
	hdr = (struct hci_acl_hdr *)skb_transport_header(skb);
	hdr->handle = cpu_to_le16(hci_handle_pack(handle, flags));
	hdr->dlen   = cpu_to_le16(len);
}

<<<<<<< HEAD
<<<<<<< HEAD
void hci_send_acl(struct hci_conn *conn, struct hci_chan *chan,
		struct sk_buff *skb, __u16 flags)
{
	struct hci_dev *hdev = conn->hdev;
	struct sk_buff *list;

	BT_DBG("%s conn %p chan %p flags 0x%x", hdev->name, conn, chan, flags);

	skb->dev = (void *) hdev;
	bt_cb(skb)->pkt_type = HCI_ACLDATA_PKT;
	if (hdev->dev_type == HCI_BREDR)
		hci_add_acl_hdr(skb, conn->handle, flags);
	else
		hci_add_acl_hdr(skb, chan->ll_handle, flags);
=======
static void hci_queue_acl(struct hci_chan *chan, struct sk_buff_head *queue,
			  struct sk_buff *skb, __u16 flags)
=======
void hci_send_acl(struct hci_conn *conn, struct hci_chan *chan,
		struct sk_buff *skb, __u16 flags)
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct hci_conn *conn = chan->conn;
	struct hci_dev *hdev = conn->hdev;
	struct sk_buff *list;

<<<<<<< HEAD
	skb->len = skb_headlen(skb);
	skb->data_len = 0;

	bt_cb(skb)->pkt_type = HCI_ACLDATA_PKT;

	switch (hdev->dev_type) {
	case HCI_BREDR:
		hci_add_acl_hdr(skb, conn->handle, flags);
		break;
	case HCI_AMP:
		hci_add_acl_hdr(skb, chan->handle, flags);
		break;
	default:
		BT_ERR("%s unknown dev_type %d", hdev->name, hdev->dev_type);
		return;
	}
>>>>>>> refs/remotes/origin/master
=======
	BT_DBG("%s conn %p chan %p flags 0x%x", hdev->name, conn, chan, flags);

	skb->dev = (void *) hdev;
	bt_cb(skb)->pkt_type = HCI_ACLDATA_PKT;
	if (hdev->dev_type == HCI_BREDR)
		hci_add_acl_hdr(skb, conn->handle, flags);
	else
		hci_add_acl_hdr(skb, chan->ll_handle, flags);
>>>>>>> refs/remotes/origin/cm-11.0

	list = skb_shinfo(skb)->frag_list;
	if (!list) {
		/* Non fragmented */
		BT_DBG("%s nonfrag skb %p len %d", hdev->name, skb, skb->len);

<<<<<<< HEAD
<<<<<<< HEAD
		skb_queue_tail(&conn->data_q, skb);
=======
		skb_queue_tail(queue, skb);
>>>>>>> refs/remotes/origin/master
=======
		skb_queue_tail(&conn->data_q, skb);
>>>>>>> refs/remotes/origin/cm-11.0
	} else {
		/* Fragmented */
		BT_DBG("%s frag %p len %d", hdev->name, skb, skb->len);

		skb_shinfo(skb)->frag_list = NULL;

		/* Queue all fragments atomically */
<<<<<<< HEAD
<<<<<<< HEAD
		spin_lock_bh(&conn->data_q.lock);

		__skb_queue_tail(&conn->data_q, skb);
		flags &= ~ACL_PB_MASK;
=======
		spin_lock(&queue->lock);

		__skb_queue_tail(queue, skb);

		flags &= ~ACL_START;
>>>>>>> refs/remotes/origin/master
=======
		spin_lock_bh(&conn->data_q.lock);

		__skb_queue_tail(&conn->data_q, skb);
		flags &= ~ACL_PB_MASK;
>>>>>>> refs/remotes/origin/cm-11.0
		flags |= ACL_CONT;
		do {
			skb = list; list = list->next;

<<<<<<< HEAD
			skb->dev = (void *) hdev;
=======
>>>>>>> refs/remotes/origin/master
			bt_cb(skb)->pkt_type = HCI_ACLDATA_PKT;
			hci_add_acl_hdr(skb, conn->handle, flags);

			BT_DBG("%s frag %p len %d", hdev->name, skb, skb->len);

<<<<<<< HEAD
<<<<<<< HEAD
			__skb_queue_tail(&conn->data_q, skb);
		} while (list);

		spin_unlock_bh(&conn->data_q.lock);
	}

	tasklet_schedule(&hdev->tx_task);
}
EXPORT_SYMBOL(hci_send_acl);
=======
			__skb_queue_tail(queue, skb);
=======
			__skb_queue_tail(&conn->data_q, skb);
>>>>>>> refs/remotes/origin/cm-11.0
		} while (list);

		spin_unlock_bh(&conn->data_q.lock);
	}
<<<<<<< HEAD
}

void hci_send_acl(struct hci_chan *chan, struct sk_buff *skb, __u16 flags)
{
	struct hci_dev *hdev = chan->conn->hdev;

	BT_DBG("%s chan %p flags 0x%4.4x", hdev->name, chan, flags);

	hci_queue_acl(chan, &chan->data_q, skb, flags);

	queue_work(hdev->workqueue, &hdev->tx_work);
=======

	tasklet_schedule(&hdev->tx_task);
>>>>>>> refs/remotes/origin/cm-11.0
}
>>>>>>> refs/remotes/origin/master

/* Send SCO data */
void hci_send_sco(struct hci_conn *conn, struct sk_buff *skb)
{
	struct hci_dev *hdev = conn->hdev;
	struct hci_sco_hdr hdr;

	BT_DBG("%s len %d", hdev->name, skb->len);

	hdr.handle = cpu_to_le16(conn->handle);
	hdr.dlen   = skb->len;

	skb_push(skb, HCI_SCO_HDR_SIZE);
	skb_reset_transport_header(skb);
	memcpy(skb_transport_header(skb), &hdr, HCI_SCO_HDR_SIZE);

<<<<<<< HEAD
	skb->dev = (void *) hdev;
	bt_cb(skb)->pkt_type = HCI_SCODATA_PKT;

	skb_queue_tail(&conn->data_q, skb);
	tasklet_schedule(&hdev->tx_task);
}
EXPORT_SYMBOL(hci_send_sco);

/* ---- HCI TX task (outgoing data) ---- */
<<<<<<< HEAD
/* HCI ACL Connection scheduler */
static inline struct hci_conn *hci_low_sent_acl(struct hci_dev *hdev,
								int *quote)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	struct hci_conn *conn = NULL;
	int num = 0, min = ~0, conn_num = 0;
	struct list_head *p;

	/* We don't have to lock device here. Connections are always
	 * added and removed with TX task disabled. */
	list_for_each(p, &h->list) {
		struct hci_conn *c;
		c = list_entry(p, struct hci_conn, list);
		if (c->type == ACL_LINK)
			conn_num++;

		if (skb_queue_empty(&c->data_q))
			continue;

		if (c->state != BT_CONNECTED && c->state != BT_CONFIG)
			continue;

		num++;

		if (c->sent < min) {
			min  = c->sent;
			conn = c;
		}
	}

	if (conn) {
		int cnt, q;
		cnt = hdev->acl_cnt;
		q = cnt / num;
		*quote = q ? q : 1;
	} else
		*quote = 0;

	if ((*quote == hdev->acl_cnt) &&
		(conn->sent == (hdev->acl_pkts - 1)) &&
		(conn_num > 1)) {
			*quote = 0;
			conn = NULL;
	}

	BT_DBG("conn %p quote %d", conn, *quote);
	return conn;
}
=======
>>>>>>> refs/remotes/origin/cm-10.0

/* HCI Connection scheduler */
static inline struct hci_conn *hci_low_sent(struct hci_dev *hdev, __u8 type, int *quote)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	struct hci_conn *conn = NULL;
	int num = 0, min = ~0;
	struct list_head *p;

	/* We don't have to lock device here. Connections are always
	 * added and removed with TX task disabled. */
	list_for_each(p, &h->list) {
		struct hci_conn *c;
		c = list_entry(p, struct hci_conn, list);

=======
	bt_cb(skb)->pkt_type = HCI_SCODATA_PKT;

	skb_queue_tail(&conn->data_q, skb);
	tasklet_schedule(&hdev->tx_task);
}

/* ---- HCI TX task (outgoing data) ---- */
/* HCI ACL Connection scheduler */
static inline struct hci_conn *hci_low_sent_acl(struct hci_dev *hdev,
								int *quote)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	struct hci_conn *conn = NULL;
	int num = 0, min = ~0, conn_num = 0;
	struct list_head *p;

	/* We don't have to lock device here. Connections are always
	 * added and removed with TX task disabled. */
	list_for_each(p, &h->list) {
		struct hci_conn *c;
		c = list_entry(p, struct hci_conn, list);
		if (c->type == ACL_LINK)
			conn_num++;

		if (skb_queue_empty(&c->data_q))
			continue;

		if (c->state != BT_CONNECTED && c->state != BT_CONFIG)
			continue;

		num++;

		if (c->sent < min) {
			min  = c->sent;
			conn = c;
		}
	}

	if (conn) {
		int cnt, q;
		cnt = hdev->acl_cnt;
		q = cnt / num;
		*quote = q ? q : 1;
	} else
		*quote = 0;

	if ((*quote == hdev->acl_cnt) &&
		(conn->sent == (hdev->acl_pkts - 1)) &&
		(conn_num > 1)) {
			*quote = 0;
			conn = NULL;
	}

	BT_DBG("conn %p quote %d", conn, *quote);
	return conn;
}

/* HCI Connection scheduler */
static struct hci_conn *hci_low_sent(struct hci_dev *hdev, __u8 type,
				     int *quote)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
<<<<<<< HEAD
	struct hci_conn *conn = NULL, *c;
	unsigned int num = 0, min = ~0;
=======
	struct hci_conn *conn = NULL;
	int num = 0, min = ~0;
	struct list_head *p;
>>>>>>> refs/remotes/origin/cm-11.0

	/* We don't have to lock device here. Connections are always
	 * added and removed with TX task disabled. */
	list_for_each(p, &h->list) {
		struct hci_conn *c;
		c = list_entry(p, struct hci_conn, list);

<<<<<<< HEAD
	rcu_read_lock();

	list_for_each_entry_rcu(c, &h->list, list) {
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		if (c->type != type || skb_queue_empty(&c->data_q))
			continue;

		if (c->state != BT_CONNECTED && c->state != BT_CONFIG)
			continue;

		num++;

		if (c->sent < min) {
			min  = c->sent;
			conn = c;
		}
<<<<<<< HEAD
<<<<<<< HEAD
	}

=======

		if (hci_conn_num(hdev, type) == num)
			break;
	}

	rcu_read_unlock();

>>>>>>> refs/remotes/origin/master
=======
	}

>>>>>>> refs/remotes/origin/cm-11.0
	if (conn) {
		int cnt, q;

		switch (conn->type) {
		case ACL_LINK:
			cnt = hdev->acl_cnt;
			break;
		case SCO_LINK:
		case ESCO_LINK:
			cnt = hdev->sco_cnt;
			break;
		case LE_LINK:
			cnt = hdev->le_mtu ? hdev->le_cnt : hdev->acl_cnt;
			break;
		default:
			cnt = 0;
			BT_ERR("Unknown link type");
		}

		q = cnt / num;
		*quote = q ? q : 1;
	} else
		*quote = 0;

	BT_DBG("conn %p quote %d", conn, *quote);
	return conn;
}

<<<<<<< HEAD
static inline void hci_link_tx_to(struct hci_dev *hdev, __u8 type)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	struct list_head *p;
	struct hci_conn  *c;
<<<<<<< HEAD

	BT_ERR("%s link tx timeout", hdev->name);

	/* Kill stalled connections */
	list_for_each(p, &h->list) {
		c = list_entry(p, struct hci_conn, list);
		if (c->type == type && c->sent) {
			BT_ERR("%s killing stalled connection %s",
				hdev->name, batostr(&c->dst));
			hci_acl_disconn(c, 0x13);
		}
	}
}

static inline void hci_sched_acl(struct hci_dev *hdev)
{
	struct hci_conn *conn;
	struct sk_buff *skb;
	int quote;

	BT_DBG("%s", hdev->name);

	if (!test_bit(HCI_RAW, &hdev->flags)) {
		/* ACL tx timeout must be longer than maximum
		 * link supervision timeout (40.9 seconds) */
		if (hdev->acl_cnt <= 0 &&
			time_after(jiffies, hdev->acl_last_tx + HZ * 45))
			hci_link_tx_to(hdev, ACL_LINK);
	}

	while (hdev->acl_cnt > 0 &&
<<<<<<< HEAD
		((conn = hci_low_sent_acl(hdev, &quote)) != NULL)) {

		while (quote > 0 &&
			  (skb = skb_dequeue(&conn->data_q))) {
=======
		(conn = hci_low_sent(hdev, ACL_LINK, &quote))) {
		while (quote > 0 && (skb = skb_dequeue(&conn->data_q))) {
>>>>>>> refs/remotes/origin/cm-10.0
			int count = 1;

			BT_DBG("skb %p len %d", skb, skb->len);

			if (hdev->flow_ctl_mode ==
				HCI_BLOCK_BASED_FLOW_CTL_MODE)
				/* Calculate count of blocks used by
				 * this packet
				 */
				count = ((skb->len - HCI_ACL_HDR_SIZE - 1) /
					hdev->data_block_len) + 1;

			if (count > hdev->acl_cnt)
				return;

			hci_conn_enter_active_mode(conn, bt_cb(skb)->force_active);

			hci_send_frame(skb);
			hdev->acl_last_tx = jiffies;

			hdev->acl_cnt -= count;
			quote -= count;

			conn->sent += count;
		}
	}
}

/* Schedule SCO */
static inline void hci_sched_sco(struct hci_dev *hdev)
=======
static void hci_link_tx_to(struct hci_dev *hdev, __u8 type)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	struct hci_conn *c;
=======
>>>>>>> refs/remotes/origin/cm-11.0

	BT_ERR("%s link tx timeout", hdev->name);

	/* Kill stalled connections */
	list_for_each(p, &h->list) {
		c = list_entry(p, struct hci_conn, list);
		if (c->type == type && c->sent) {
			BT_ERR("%s killing stalled connection %pMR",
			       hdev->name, &c->dst);
			hci_disconnect(c, HCI_ERROR_REMOTE_USER_TERM);
		}
	}
}

<<<<<<< HEAD
static struct hci_chan *hci_chan_sent(struct hci_dev *hdev, __u8 type,
				      int *quote)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	struct hci_chan *chan = NULL;
	unsigned int num = 0, min = ~0, cur_prio = 0;
	struct hci_conn *conn;
	int cnt, q, conn_num = 0;

	BT_DBG("%s", hdev->name);

	rcu_read_lock();

	list_for_each_entry_rcu(conn, &h->list, list) {
		struct hci_chan *tmp;

		if (conn->type != type)
			continue;

		if (conn->state != BT_CONNECTED && conn->state != BT_CONFIG)
			continue;

		conn_num++;

		list_for_each_entry_rcu(tmp, &conn->chan_list, list) {
			struct sk_buff *skb;

			if (skb_queue_empty(&tmp->data_q))
				continue;

			skb = skb_peek(&tmp->data_q);
			if (skb->priority < cur_prio)
				continue;

			if (skb->priority > cur_prio) {
				num = 0;
				min = ~0;
				cur_prio = skb->priority;
			}

			num++;

			if (conn->sent < min) {
				min  = conn->sent;
				chan = tmp;
			}
		}

		if (hci_conn_num(hdev, type) == conn_num)
			break;
	}

	rcu_read_unlock();

	if (!chan)
		return NULL;

	switch (chan->conn->type) {
	case ACL_LINK:
		cnt = hdev->acl_cnt;
		break;
	case AMP_LINK:
		cnt = hdev->block_cnt;
		break;
	case SCO_LINK:
	case ESCO_LINK:
		cnt = hdev->sco_cnt;
		break;
	case LE_LINK:
		cnt = hdev->le_mtu ? hdev->le_cnt : hdev->acl_cnt;
		break;
	default:
		cnt = 0;
		BT_ERR("Unknown link type");
	}

	q = cnt / num;
	*quote = q ? q : 1;
	BT_DBG("chan %p quote %d", chan, *quote);
	return chan;
}

static void hci_prio_recalculate(struct hci_dev *hdev, __u8 type)
=======
static inline void hci_sched_acl(struct hci_dev *hdev)
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct hci_conn *conn;
	struct sk_buff *skb;
	int quote;

	BT_DBG("%s", hdev->name);

<<<<<<< HEAD
	rcu_read_lock();

	list_for_each_entry_rcu(conn, &h->list, list) {
		struct hci_chan *chan;

		if (conn->type != type)
			continue;

		if (conn->state != BT_CONNECTED && conn->state != BT_CONFIG)
			continue;

		num++;

		list_for_each_entry_rcu(chan, &conn->chan_list, list) {
			struct sk_buff *skb;

			if (chan->sent) {
				chan->sent = 0;
				continue;
			}

			if (skb_queue_empty(&chan->data_q))
				continue;

			skb = skb_peek(&chan->data_q);
			if (skb->priority >= HCI_PRIO_MAX - 1)
				continue;

			skb->priority = HCI_PRIO_MAX - 1;

			BT_DBG("chan %p skb %p promoted to %d", chan, skb,
			       skb->priority);
		}

		if (hci_conn_num(hdev, type) == num)
			break;
	}

	rcu_read_unlock();

}

static inline int __get_blocks(struct hci_dev *hdev, struct sk_buff *skb)
{
	/* Calculate count of blocks used by this packet */
	return DIV_ROUND_UP(skb->len - HCI_ACL_HDR_SIZE, hdev->block_len);
}

static void __check_timeout(struct hci_dev *hdev, unsigned int cnt)
{
	if (!test_bit(HCI_RAW, &hdev->flags)) {
		/* ACL tx timeout must be longer than maximum
		 * link supervision timeout (40.9 seconds) */
		if (!cnt && time_after(jiffies, hdev->acl_last_tx +
				       HCI_ACL_TX_TIMEOUT))
			hci_link_tx_to(hdev, ACL_LINK);
	}
}

static void hci_sched_acl_pkt(struct hci_dev *hdev)
{
	unsigned int cnt = hdev->acl_cnt;
	struct hci_chan *chan;
	struct sk_buff *skb;
	int quote;

	__check_timeout(hdev, cnt);

	while (hdev->acl_cnt &&
	       (chan = hci_chan_sent(hdev, ACL_LINK, &quote))) {
		u32 priority = (skb_peek(&chan->data_q))->priority;
		while (quote-- && (skb = skb_peek(&chan->data_q))) {
			BT_DBG("chan %p skb %p len %d priority %u", chan, skb,
			       skb->len, skb->priority);

			/* Stop if priority has changed */
			if (skb->priority < priority)
				break;

			skb = skb_dequeue(&chan->data_q);

			hci_conn_enter_active_mode(chan->conn,
						   bt_cb(skb)->force_active);

			hci_send_frame(hdev, skb);
			hdev->acl_last_tx = jiffies;

			hdev->acl_cnt--;
			chan->sent++;
			chan->conn->sent++;
		}
	}
=======
	if (!test_bit(HCI_RAW, &hdev->flags)) {
		/* ACL tx timeout must be longer than maximum
		 * link supervision timeout (40.9 seconds) */
		if (hdev->acl_cnt <= 0 &&
			time_after(jiffies, hdev->acl_last_tx + HZ * 45))
			hci_link_tx_to(hdev, ACL_LINK);
	}
>>>>>>> refs/remotes/origin/cm-11.0

	while (hdev->acl_cnt > 0 &&
		((conn = hci_low_sent_acl(hdev, &quote)) != NULL)) {

<<<<<<< HEAD
static void hci_sched_acl_blk(struct hci_dev *hdev)
{
	unsigned int cnt = hdev->block_cnt;
	struct hci_chan *chan;
	struct sk_buff *skb;
	int quote;
	u8 type;

	__check_timeout(hdev, cnt);

	BT_DBG("%s", hdev->name);

	if (hdev->dev_type == HCI_AMP)
		type = AMP_LINK;
	else
		type = ACL_LINK;

	while (hdev->block_cnt > 0 &&
	       (chan = hci_chan_sent(hdev, type, &quote))) {
		u32 priority = (skb_peek(&chan->data_q))->priority;
		while (quote > 0 && (skb = skb_peek(&chan->data_q))) {
			int blocks;

			BT_DBG("chan %p skb %p len %d priority %u", chan, skb,
			       skb->len, skb->priority);

			/* Stop if priority has changed */
			if (skb->priority < priority)
				break;
=======
		while (quote > 0 &&
			  (skb = skb_dequeue(&conn->data_q))) {
			int count = 1;

			BT_DBG("skb %p len %d", skb, skb->len);
>>>>>>> refs/remotes/origin/cm-11.0

			if (hdev->flow_ctl_mode ==
				HCI_BLOCK_BASED_FLOW_CTL_MODE)
				/* Calculate count of blocks used by
				 * this packet
				 */
				count = ((skb->len - HCI_ACL_HDR_SIZE - 1) /
					hdev->data_block_len) + 1;

			if (count > hdev->acl_cnt)
				return;

<<<<<<< HEAD
			hci_conn_enter_active_mode(chan->conn,
						   bt_cb(skb)->force_active);
=======
			hci_conn_enter_active_mode(conn, bt_cb(skb)->force_active);
>>>>>>> refs/remotes/origin/cm-11.0

			hci_send_frame(hdev, skb);
			hdev->acl_last_tx = jiffies;

			hdev->acl_cnt -= count;
			quote -= count;

			conn->sent += count;
		}
	}
<<<<<<< HEAD

	if (cnt != hdev->block_cnt)
		hci_prio_recalculate(hdev, type);
}

static void hci_sched_acl(struct hci_dev *hdev)
{
	BT_DBG("%s", hdev->name);

	/* No ACL link over BR/EDR controller */
	if (!hci_conn_num(hdev, ACL_LINK) && hdev->dev_type == HCI_BREDR)
		return;

	/* No AMP link over AMP controller */
	if (!hci_conn_num(hdev, AMP_LINK) && hdev->dev_type == HCI_AMP)
		return;

	switch (hdev->flow_ctl_mode) {
	case HCI_FLOW_CTL_MODE_PACKET_BASED:
		hci_sched_acl_pkt(hdev);
		break;

	case HCI_FLOW_CTL_MODE_BLOCK_BASED:
		hci_sched_acl_blk(hdev);
		break;
	}
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

/* Schedule SCO */
static void hci_sched_sco(struct hci_dev *hdev)
>>>>>>> refs/remotes/origin/master
{
	struct hci_conn *conn;
	struct sk_buff *skb;
	int quote;

	BT_DBG("%s", hdev->name);

<<<<<<< HEAD
<<<<<<< HEAD
	while (hdev->sco_cnt && (conn = hci_low_sent(hdev, SCO_LINK, &quote))) {
		while (quote-- && (skb = skb_dequeue(&conn->data_q))) {
			BT_DBG("skb %p len %d", skb, skb->len);
			hci_send_frame(skb);
=======
	if (!hci_conn_num(hdev, SCO_LINK))
		return;

=======
>>>>>>> refs/remotes/origin/cm-11.0
	while (hdev->sco_cnt && (conn = hci_low_sent(hdev, SCO_LINK, &quote))) {
		while (quote-- && (skb = skb_dequeue(&conn->data_q))) {
			BT_DBG("skb %p len %d", skb, skb->len);
			hci_send_frame(hdev, skb);
>>>>>>> refs/remotes/origin/master

			conn->sent++;
			if (conn->sent == ~0)
				conn->sent = 0;
		}
	}
}

<<<<<<< HEAD
static inline void hci_sched_esco(struct hci_dev *hdev)
=======
static void hci_sched_esco(struct hci_dev *hdev)
>>>>>>> refs/remotes/origin/master
{
	struct hci_conn *conn;
	struct sk_buff *skb;
	int quote;

	BT_DBG("%s", hdev->name);

<<<<<<< HEAD
<<<<<<< HEAD
	while (hdev->sco_cnt && (conn = hci_low_sent(hdev, ESCO_LINK, &quote))) {
		while (quote-- && (skb = skb_dequeue(&conn->data_q))) {
			BT_DBG("skb %p len %d", skb, skb->len);
			hci_send_frame(skb);
=======
	if (!hci_conn_num(hdev, ESCO_LINK))
		return;

	while (hdev->sco_cnt && (conn = hci_low_sent(hdev, ESCO_LINK,
						     &quote))) {
=======
	while (hdev->sco_cnt && (conn = hci_low_sent(hdev, ESCO_LINK, &quote))) {
>>>>>>> refs/remotes/origin/cm-11.0
		while (quote-- && (skb = skb_dequeue(&conn->data_q))) {
			BT_DBG("skb %p len %d", skb, skb->len);
			hci_send_frame(hdev, skb);
>>>>>>> refs/remotes/origin/master

			conn->sent++;
			if (conn->sent == ~0)
				conn->sent = 0;
		}
	}
}

<<<<<<< HEAD
static inline void hci_sched_le(struct hci_dev *hdev)
{
	struct hci_conn *conn;
<<<<<<< HEAD
	struct sk_buff *skb;
	int quote, cnt;

	BT_DBG("%s", hdev->name);

=======
static void hci_sched_le(struct hci_dev *hdev)
{
	struct hci_chan *chan;
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct sk_buff *skb;
	int quote, cnt;

	BT_DBG("%s", hdev->name);

<<<<<<< HEAD
	if (!hci_conn_num(hdev, LE_LINK))
		return;

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (!test_bit(HCI_RAW, &hdev->flags)) {
		/* LE tx timeout must be longer than maximum
		 * link supervision timeout (40.9 seconds) */
		if (!hdev->le_cnt && hdev->le_pkts &&
<<<<<<< HEAD
				time_after(jiffies, hdev->le_last_tx + HZ * 45))
=======
		    time_after(jiffies, hdev->le_last_tx + HZ * 45))
>>>>>>> refs/remotes/origin/master
			hci_link_tx_to(hdev, LE_LINK);
	}

	cnt = hdev->le_pkts ? hdev->le_cnt : hdev->acl_cnt;
<<<<<<< HEAD
<<<<<<< HEAD
	while (cnt && (conn = hci_low_sent(hdev, LE_LINK, &quote))) {
		while (quote-- && (skb = skb_dequeue(&conn->data_q))) {
			BT_DBG("skb %p len %d", skb, skb->len);

			hci_send_frame(skb);
			hdev->le_last_tx = jiffies;

			cnt--;
			conn->sent++;
		}
	}
=======
	tmp = cnt;
	while (cnt && (chan = hci_chan_sent(hdev, LE_LINK, &quote))) {
		u32 priority = (skb_peek(&chan->data_q))->priority;
		while (quote-- && (skb = skb_peek(&chan->data_q))) {
			BT_DBG("chan %p skb %p len %d priority %u", chan, skb,
			       skb->len, skb->priority);

			/* Stop if priority has changed */
			if (skb->priority < priority)
				break;

			skb = skb_dequeue(&chan->data_q);
=======
	while (cnt && (conn = hci_low_sent(hdev, LE_LINK, &quote))) {
		while (quote-- && (skb = skb_dequeue(&conn->data_q))) {
			BT_DBG("skb %p len %d", skb, skb->len);
>>>>>>> refs/remotes/origin/cm-11.0

			hci_send_frame(hdev, skb);
			hdev->le_last_tx = jiffies;

			cnt--;
			conn->sent++;
		}
	}
<<<<<<< HEAD

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (hdev->le_pkts)
		hdev->le_cnt = cnt;
	else
		hdev->acl_cnt = cnt;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static void hci_tx_task(unsigned long arg)
{
	struct hci_dev *hdev = (struct hci_dev *) arg;
	struct sk_buff *skb;

	read_lock(&hci_task_lock);

	BT_DBG("%s acl %d sco %d le %d", hdev->name, hdev->acl_cnt,
		hdev->sco_cnt, hdev->le_cnt);

	/* Schedule queues and send stuff to HCI driver */

	hci_sched_acl(hdev);

	hci_sched_sco(hdev);

	hci_sched_esco(hdev);

	hci_sched_le(hdev);

	/* Send next queued raw (unknown type) packet */
	while ((skb = skb_dequeue(&hdev->raw_q)))
		hci_send_frame(skb);

	read_unlock(&hci_task_lock);
}

/* ----- HCI RX task (incoming data proccessing) ----- */

/* ACL data packet */
static inline void hci_acldata_packet(struct hci_dev *hdev, struct sk_buff *skb)
=======

	if (cnt != tmp)
		hci_prio_recalculate(hdev, LE_LINK);
}

static void hci_tx_work(struct work_struct *work)
{
	struct hci_dev *hdev = container_of(work, struct hci_dev, tx_work);
	struct sk_buff *skb;

	BT_DBG("%s acl %d sco %d le %d", hdev->name, hdev->acl_cnt,
	       hdev->sco_cnt, hdev->le_cnt);

	if (!test_bit(HCI_USER_CHANNEL, &hdev->dev_flags)) {
		/* Schedule queues and send stuff to HCI driver */
		hci_sched_acl(hdev);
		hci_sched_sco(hdev);
		hci_sched_esco(hdev);
		hci_sched_le(hdev);
	}

	/* Send next queued raw (unknown type) packet */
	while ((skb = skb_dequeue(&hdev->raw_q)))
		hci_send_frame(hdev, skb);
}

/* ----- HCI RX task (incoming data processing) ----- */

/* ACL data packet */
static void hci_acldata_packet(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_acl_hdr *hdr = (void *) skb->data;
	struct hci_conn *conn;
	__u16 handle, flags;

	skb_pull(skb, HCI_ACL_HDR_SIZE);

	handle = __le16_to_cpu(hdr->handle);
	flags  = hci_flags(handle);
	handle = hci_handle(handle);

<<<<<<< HEAD
	BT_DBG("%s len %d handle 0x%x flags 0x%x", hdev->name, skb->len, handle, flags);
=======
	BT_DBG("%s len %d handle 0x%4.4x flags 0x%4.4x", hdev->name, skb->len,
	       handle, flags);
>>>>>>> refs/remotes/origin/master

	hdev->stat.acl_rx++;

	hci_dev_lock(hdev);
	conn = hci_conn_hash_lookup_handle(hdev, handle);
	hci_dev_unlock(hdev);

	if (conn) {
<<<<<<< HEAD
<<<<<<< HEAD
		register struct hci_proto *hp;

		hci_conn_enter_active_mode(conn, bt_cb(skb)->force_active);

		/* Send to upper protocol */
		hp = hci_proto[HCI_PROTO_L2CAP];
		if (hp && hp->recv_acldata) {
			hp->recv_acldata(conn, skb, flags);
			return;
		}
	} else {
		BT_ERR("%s ACL packet for unknown connection handle %d",
			hdev->name, handle);
=======
		hci_conn_enter_active_mode(conn, BT_POWER_FORCE_ACTIVE_OFF);
=======
		register struct hci_proto *hp;

		hci_conn_enter_active_mode(conn, bt_cb(skb)->force_active);
>>>>>>> refs/remotes/origin/cm-11.0

		/* Send to upper protocol */
		hp = hci_proto[HCI_PROTO_L2CAP];
		if (hp && hp->recv_acldata) {
			hp->recv_acldata(conn, skb, flags);
			return;
		}
	} else {
		BT_ERR("%s ACL packet for unknown connection handle %d",
		       hdev->name, handle);
>>>>>>> refs/remotes/origin/master
	}

	kfree_skb(skb);
}

/* SCO data packet */
<<<<<<< HEAD
static inline void hci_scodata_packet(struct hci_dev *hdev, struct sk_buff *skb)
=======
static void hci_scodata_packet(struct hci_dev *hdev, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct hci_sco_hdr *hdr = (void *) skb->data;
	struct hci_conn *conn;
	__u16 handle;

	skb_pull(skb, HCI_SCO_HDR_SIZE);

	handle = __le16_to_cpu(hdr->handle);

<<<<<<< HEAD
	BT_DBG("%s len %d handle 0x%x", hdev->name, skb->len, handle);
=======
	BT_DBG("%s len %d handle 0x%4.4x", hdev->name, skb->len, handle);
>>>>>>> refs/remotes/origin/master

	hdev->stat.sco_rx++;

	hci_dev_lock(hdev);
	conn = hci_conn_hash_lookup_handle(hdev, handle);
	hci_dev_unlock(hdev);

	if (conn) {
<<<<<<< HEAD
<<<<<<< HEAD
		register struct hci_proto *hp;

		/* Send to upper protocol */
		hp = hci_proto[HCI_PROTO_SCO];
		if (hp && hp->recv_scodata) {
			hp->recv_scodata(conn, skb);
			return;
		}
	} else {
		BT_ERR("%s SCO packet for unknown connection handle %d",
			hdev->name, handle);
=======
=======
		register struct hci_proto *hp;

>>>>>>> refs/remotes/origin/cm-11.0
		/* Send to upper protocol */
		hp = hci_proto[HCI_PROTO_SCO];
		if (hp && hp->recv_scodata) {
			hp->recv_scodata(conn, skb);
			return;
		}
	} else {
		BT_ERR("%s SCO packet for unknown connection handle %d",
		       hdev->name, handle);
>>>>>>> refs/remotes/origin/master
	}

	kfree_skb(skb);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void hci_rx_task(unsigned long arg)
{
	struct hci_dev *hdev = (struct hci_dev *) arg;
	struct sk_buff *skb;

	BT_DBG("%s", hdev->name);

	read_lock(&hci_task_lock);

	while ((skb = skb_dequeue(&hdev->rx_q))) {
		if (atomic_read(&hdev->promisc)) {
			/* Send copy to the sockets */
			hci_send_to_sock(hdev, skb, NULL);
		}

		if (test_bit(HCI_RAW, &hdev->flags)) {
=======
static bool hci_req_is_complete(struct hci_dev *hdev)
{
	struct sk_buff *skb;

	skb = skb_peek(&hdev->cmd_q);
	if (!skb)
		return true;

	return bt_cb(skb)->req.start;
}

static void hci_resend_last(struct hci_dev *hdev)
{
	struct hci_command_hdr *sent;
	struct sk_buff *skb;
	u16 opcode;

	if (!hdev->sent_cmd)
		return;

	sent = (void *) hdev->sent_cmd->data;
	opcode = __le16_to_cpu(sent->opcode);
	if (opcode == HCI_OP_RESET)
		return;

	skb = skb_clone(hdev->sent_cmd, GFP_KERNEL);
	if (!skb)
		return;

	skb_queue_head(&hdev->cmd_q, skb);
	queue_work(hdev->workqueue, &hdev->cmd_work);
}

void hci_req_cmd_complete(struct hci_dev *hdev, u16 opcode, u8 status)
{
	hci_req_complete_t req_complete = NULL;
	struct sk_buff *skb;
	unsigned long flags;

	BT_DBG("opcode 0x%04x status 0x%02x", opcode, status);

	/* If the completed command doesn't match the last one that was
	 * sent we need to do special handling of it.
	 */
	if (!hci_sent_cmd_data(hdev, opcode)) {
		/* Some CSR based controllers generate a spontaneous
		 * reset complete event during init and any pending
		 * command will never be completed. In such a case we
		 * need to resend whatever was the last sent
		 * command.
		 */
		if (test_bit(HCI_INIT, &hdev->flags) && opcode == HCI_OP_RESET)
			hci_resend_last(hdev);

		return;
	}

	/* If the command succeeded and there's still more commands in
	 * this request the request is not yet complete.
	 */
	if (!status && !hci_req_is_complete(hdev))
		return;

	/* If this was the last command in a request the complete
	 * callback would be found in hdev->sent_cmd instead of the
	 * command queue (hdev->cmd_q).
	 */
	if (hdev->sent_cmd) {
		req_complete = bt_cb(hdev->sent_cmd)->req.complete;

		if (req_complete) {
			/* We must set the complete callback to NULL to
			 * avoid calling the callback more than once if
			 * this function gets called again.
			 */
			bt_cb(hdev->sent_cmd)->req.complete = NULL;

			goto call_complete;
		}
	}

	/* Remove all pending commands belonging to this request */
	spin_lock_irqsave(&hdev->cmd_q.lock, flags);
	while ((skb = __skb_dequeue(&hdev->cmd_q))) {
		if (bt_cb(skb)->req.start) {
			__skb_queue_head(&hdev->cmd_q, skb);
			break;
		}

		req_complete = bt_cb(skb)->req.complete;
		kfree_skb(skb);
	}
	spin_unlock_irqrestore(&hdev->cmd_q.lock, flags);

call_complete:
	if (req_complete)
		req_complete(hdev, status);
}

static void hci_rx_work(struct work_struct *work)
=======
static void hci_rx_task(unsigned long arg)
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct hci_dev *hdev = (struct hci_dev *) arg;
	struct sk_buff *skb;

	BT_DBG("%s", hdev->name);

	read_lock(&hci_task_lock);

	while ((skb = skb_dequeue(&hdev->rx_q))) {
		if (atomic_read(&hdev->promisc)) {
			/* Send copy to the sockets */
			hci_send_to_sock(hdev, skb, NULL);
		}

		if (test_bit(HCI_RAW, &hdev->flags) ||
		    test_bit(HCI_USER_CHANNEL, &hdev->dev_flags)) {
>>>>>>> refs/remotes/origin/master
			kfree_skb(skb);
			continue;
		}

		if (test_bit(HCI_INIT, &hdev->flags)) {
			/* Don't process data packets in this states. */
			switch (bt_cb(skb)->pkt_type) {
			case HCI_ACLDATA_PKT:
			case HCI_SCODATA_PKT:
				kfree_skb(skb);
				continue;
			}
		}

		/* Process frame */
		switch (bt_cb(skb)->pkt_type) {
		case HCI_EVENT_PKT:
<<<<<<< HEAD
<<<<<<< HEAD
=======
			BT_DBG("%s Event packet", hdev->name);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
			hci_event_packet(hdev, skb);
			break;

		case HCI_ACLDATA_PKT:
			BT_DBG("%s ACL data packet", hdev->name);
			hci_acldata_packet(hdev, skb);
			break;

		case HCI_SCODATA_PKT:
			BT_DBG("%s SCO data packet", hdev->name);
			hci_scodata_packet(hdev, skb);
			break;

		default:
			kfree_skb(skb);
			break;
		}
	}
<<<<<<< HEAD
<<<<<<< HEAD

	read_unlock(&hci_task_lock);
}

static void hci_cmd_task(unsigned long arg)
{
	struct hci_dev *hdev = (struct hci_dev *) arg;
	struct sk_buff *skb;

	BT_DBG("%s cmd %d", hdev->name, atomic_read(&hdev->cmd_cnt));
=======
=======

	read_unlock(&hci_task_lock);
>>>>>>> refs/remotes/origin/cm-11.0
}

static void hci_cmd_task(unsigned long arg)
{
	struct hci_dev *hdev = (struct hci_dev *) arg;
	struct sk_buff *skb;

	BT_DBG("%s cmd_cnt %d cmd queued %d", hdev->name,
	       atomic_read(&hdev->cmd_cnt), skb_queue_len(&hdev->cmd_q));
>>>>>>> refs/remotes/origin/master

	/* Send queued commands */
	if (atomic_read(&hdev->cmd_cnt)) {
		skb = skb_dequeue(&hdev->cmd_q);
		if (!skb)
			return;

		kfree_skb(hdev->sent_cmd);

<<<<<<< HEAD
		hdev->sent_cmd = skb_clone(skb, GFP_ATOMIC);
		if (hdev->sent_cmd) {
			atomic_dec(&hdev->cmd_cnt);
			hci_send_frame(skb);
			mod_timer(&hdev->cmd_timer,
				  jiffies + msecs_to_jiffies(HCI_CMD_TIMEOUT));
<<<<<<< HEAD
		} else {
			skb_queue_head(&hdev->cmd_q, skb);
			tasklet_schedule(&hdev->cmd_task);
		}
	}
}

module_param(enable_smp, bool, 0644);
MODULE_PARM_DESC(enable_smp, "Enable SMP support (LE only)");
=======
		hdev->sent_cmd = skb_clone(skb, GFP_KERNEL);
		if (hdev->sent_cmd) {
			atomic_dec(&hdev->cmd_cnt);
			hci_send_frame(hdev, skb);
			if (test_bit(HCI_RESET, &hdev->flags))
				del_timer(&hdev->cmd_timer);
			else
				mod_timer(&hdev->cmd_timer,
					  jiffies + HCI_CMD_TIMEOUT);
=======
>>>>>>> refs/remotes/origin/cm-11.0
		} else {
			skb_queue_head(&hdev->cmd_q, skb);
			tasklet_schedule(&hdev->cmd_task);
		}
	}
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======

module_param(enable_smp, bool, 0644);
MODULE_PARM_DESC(enable_smp, "Enable SMP support (LE only)");
>>>>>>> refs/remotes/origin/cm-11.0
