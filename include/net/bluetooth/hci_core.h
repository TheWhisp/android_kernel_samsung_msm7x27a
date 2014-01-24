/*
   BlueZ - Bluetooth protocol stack for Linux
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
   Copyright (c) 2000-2001, The Linux Foundation. All rights reserved.
   Copyright (c) 2010-2013, The Linux Foundation. All rights reserved.
=======
   Copyright (c) 2000-2001, 2010-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
   Copyright (c) 2000-2001, 2010, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
   Copyright (c) 2000-2001, 2010-2012, The Linux Foundation. All rights reserved.
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

#ifndef __HCI_CORE_H
#define __HCI_CORE_H

#include <net/bluetooth/hci.h>
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/wakelock.h>
>>>>>>> refs/remotes/origin/cm-10.0
/* HCI upper protocols */
#define HCI_PROTO_L2CAP	0
#define HCI_PROTO_SCO	1
=======

/* HCI priority */
#define HCI_PRIO_MAX	7
>>>>>>> refs/remotes/origin/master
=======
#include <linux/wakelock.h>
/* HCI upper protocols */
#define HCI_PROTO_L2CAP	0
#define HCI_PROTO_SCO	1
>>>>>>> refs/remotes/origin/cm-11.0

/* HCI Core structures */
struct inquiry_data {
	bdaddr_t	bdaddr;
	__u8		pscan_rep_mode;
	__u8		pscan_period_mode;
	__u8		pscan_mode;
	__u8		dev_class[3];
	__le16		clock_offset;
	__s8		rssi;
	__u8		ssp_mode;
};

struct inquiry_entry {
<<<<<<< HEAD
<<<<<<< HEAD
	struct inquiry_entry	*next;
=======
	struct list_head	all;		/* inq_cache.all */
	struct list_head	list;		/* unknown or resolve */
	enum {
		NAME_NOT_KNOWN,
		NAME_NEEDED,
		NAME_PENDING,
		NAME_KNOWN,
	} name_state;
>>>>>>> refs/remotes/origin/master
=======
	struct inquiry_entry	*next;
>>>>>>> refs/remotes/origin/cm-11.0
	__u32			timestamp;
	struct inquiry_data	data;
};

<<<<<<< HEAD
<<<<<<< HEAD
struct inquiry_cache {
	spinlock_t		lock;
	__u32			timestamp;
	struct inquiry_entry	*list;
=======
struct discovery_state {
	int			type;
	enum {
		DISCOVERY_STOPPED,
		DISCOVERY_STARTING,
		DISCOVERY_FINDING,
		DISCOVERY_RESOLVING,
		DISCOVERY_STOPPING,
	} state;
	struct list_head	all;	/* All devices found during inquiry */
	struct list_head	unknown;	/* Name state not known */
	struct list_head	resolve;	/* Name needs to be resolved */
	__u32			timestamp;
>>>>>>> refs/remotes/origin/master
=======
struct inquiry_cache {
	spinlock_t		lock;
	__u32			timestamp;
	struct inquiry_entry	*list;
>>>>>>> refs/remotes/origin/cm-11.0
};

struct hci_conn_hash {
	struct list_head list;
<<<<<<< HEAD
<<<<<<< HEAD
	spinlock_t       lock;
	unsigned int     acl_num;
=======
=======
	spinlock_t       lock;
>>>>>>> refs/remotes/origin/cm-11.0
	unsigned int     acl_num;
	unsigned int     amp_num;
>>>>>>> refs/remotes/origin/master
	unsigned int     sco_num;
	unsigned int     le_num;
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
struct hci_chan_list {
	struct list_head list;
	spinlock_t       lock;
};

<<<<<<< HEAD
struct bdaddr_list {
	struct list_head list;
	bdaddr_t bdaddr;
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
struct bdaddr_list {
	struct list_head list;
	bdaddr_t bdaddr;
	u8 bdaddr_type;
>>>>>>> refs/remotes/origin/master
};

struct bt_uuid {
	struct list_head list;
	u8 uuid[16];
<<<<<<< HEAD
	u8 svc_hint;
};

struct key_master_id {
	__le16 ediv;
	u8 rand[8];
} __packed;

#define KEY_TYPE_LE_BASE	0x11
#define KEY_TYPE_LTK		0x11
#define KEY_TYPE_IRK		0x12
#define KEY_TYPE_CSRK		0x13

struct link_key_data {
	bdaddr_t bdaddr;
	u8 addr_type;
	u8 key_type;
	u8 val[16];
	u8 pin_len;
	u8 auth;
	u8 dlen;
	u8 data[0];
=======
	u8 size;
	u8 svc_hint;
};

struct key_master_id {
	__le16 ediv;
	u8 rand[8];
} __packed;

#define KEY_TYPE_LE_BASE	0x11
#define KEY_TYPE_LTK		0x11
#define KEY_TYPE_IRK		0x12
#define KEY_TYPE_CSRK		0x13

struct link_key_data {
	bdaddr_t bdaddr;
	u8 addr_type;
	u8 key_type;
	u8 val[16];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
	u8 pin_len;
	u8 auth;
	u8 dlen;
	u8 data[0];
>>>>>>> refs/remotes/origin/cm-11.0
} __packed;

struct link_key {
	struct list_head list;
	bdaddr_t bdaddr;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	u8 addr_type;
	u8 key_type;
	u8 val[16];
	u8 pin_len;
	u8 auth;
	u8 dlen;
	u8 data[0];
<<<<<<< HEAD
=======
	u8 type;
	u8 val[HCI_LINK_KEY_SIZE];
	u8 pin_len;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
};

struct oob_data {
	struct list_head list;
	bdaddr_t bdaddr;
	u8 hash[16];
	u8 randomizer[16];
};

<<<<<<< HEAD
struct adv_entry {
	struct list_head list;
	bdaddr_t bdaddr;
	u8 bdaddr_type;
	u8 flags;
};

<<<<<<< HEAD
#define NUM_REASSEMBLY 4
struct hci_dev {
	struct list_head list;
	spinlock_t	lock;
	atomic_t	refcnt;
=======
#define HCI_MAX_SHORT_NAME_LENGTH	10

struct amp_assoc {
	__u16	len;
	__u16	offset;
	__u16	rem_len;
	__u16	len_so_far;
	__u8	data[HCI_MAX_AMP_ASSOC_SIZE];
};

#define HCI_MAX_PAGES	3

#define NUM_REASSEMBLY 4
struct hci_dev {
	struct list_head list;
	struct mutex	lock;
>>>>>>> refs/remotes/origin/master
=======
#define NUM_REASSEMBLY 4
struct hci_dev {
	struct list_head list;
	spinlock_t	lock;
	atomic_t	refcnt;
>>>>>>> refs/remotes/origin/cm-11.0

	char		name[8];
	unsigned long	flags;
	__u16		id;
	__u8		bus;
	__u8		dev_type;
	bdaddr_t	bdaddr;
<<<<<<< HEAD
	__u8		dev_name[HCI_MAX_NAME_LENGTH];
=======
	bdaddr_t	static_addr;
	__u8		own_addr_type;
	__u8		dev_name[HCI_MAX_NAME_LENGTH];
<<<<<<< HEAD
	__u8		short_name[HCI_MAX_SHORT_NAME_LENGTH];
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	__u8		eir[HCI_MAX_EIR_LENGTH];
	__u8		dev_class[3];
	__u8		major_class;
	__u8		minor_class;
<<<<<<< HEAD
	__u8		features[8];
	__u8		commands[64];
	__u8		ssp_mode;
<<<<<<< HEAD
=======
	__u8		max_page;
	__u8		features[HCI_MAX_PAGES][8];
	__u8		le_features[8];
	__u8		le_white_list_size;
	__u8		le_states[8];
	__u8		commands[64];
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	__u8		hci_ver;
	__u16		hci_rev;
	__u8		lmp_ver;
	__u16		manufacturer;
<<<<<<< HEAD
	__le16		lmp_subver;
	__u16		voice_setting;
	__u8		io_capability;
=======
	__u16		lmp_subver;
	__u16		voice_setting;
	__u8		num_iac;
	__u8		io_capability;
	__s8		inq_tx_power;
	__u16		page_scan_interval;
	__u16		page_scan_window;
	__u8		page_scan_type;
	__u16		le_scan_interval;
	__u16		le_scan_window;
	__u16		le_conn_min_interval;
	__u16		le_conn_max_interval;
	__u8		ssp_debug_mode;

	__u16		devid_source;
	__u16		devid_vendor;
	__u16		devid_product;
	__u16		devid_version;
>>>>>>> refs/remotes/origin/master

	__u16		pkt_type;
	__u16		esco_type;
	__u16		link_policy;
	__u16		link_mode;

	__u32		idle_timeout;
	__u16		sniff_min_interval;
	__u16		sniff_max_interval;

	__u8		amp_status;
	__u32		amp_total_bw;
	__u32		amp_max_bw;
	__u32		amp_min_latency;
	__u32		amp_max_pdu;
	__u8		amp_type;
	__u16		amp_pal_cap;
	__u16		amp_assoc_size;
	__u32		amp_max_flush_to;
	__u32		amp_be_flush_to;

<<<<<<< HEAD
<<<<<<< HEAD
	__s8		is_wbs;
=======
	struct amp_assoc	loc_assoc;

	__u8		flow_ctl_mode;

	unsigned int	auto_accept_delay;
>>>>>>> refs/remotes/origin/master
=======
	__s8		is_wbs;
>>>>>>> refs/remotes/origin/cm-11.0

	unsigned long	quirks;

	atomic_t	cmd_cnt;
	unsigned int	acl_cnt;
	unsigned int	sco_cnt;
	unsigned int	le_cnt;

<<<<<<< HEAD
<<<<<<< HEAD
	__u8	flow_ctl_mode;

=======
>>>>>>> refs/remotes/origin/master
=======
	__u8	flow_ctl_mode;

>>>>>>> refs/remotes/origin/cm-11.0
	unsigned int	acl_mtu;
	unsigned int	sco_mtu;
	unsigned int	le_mtu;
	unsigned int	acl_pkts;
	unsigned int	sco_pkts;
	unsigned int	le_pkts;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int	le_white_list_size;
=======
>>>>>>> refs/remotes/origin/cm-10.0

	unsigned int	data_block_len;
=======

	__u16		block_len;
	__u16		block_mtu;
	__u16		num_blocks;
	__u16		block_cnt;
>>>>>>> refs/remotes/origin/master
=======
	unsigned int	le_white_list_size;

	unsigned int	data_block_len;
>>>>>>> refs/remotes/origin/cm-11.0

	unsigned long	acl_last_tx;
	unsigned long	sco_last_tx;
	unsigned long	le_last_tx;

	struct workqueue_struct	*workqueue;
<<<<<<< HEAD

	struct work_struct	power_on;
	struct work_struct	power_off;
	struct timer_list	off_timer;

	struct timer_list	cmd_timer;
	struct tasklet_struct	cmd_task;
	struct tasklet_struct	rx_task;
	struct tasklet_struct	tx_task;
=======
	struct workqueue_struct	*req_workqueue;

	struct work_struct	power_on;
	struct work_struct	power_off;
	struct timer_list	off_timer;

	struct timer_list	cmd_timer;
<<<<<<< HEAD

	struct work_struct	rx_work;
	struct work_struct	cmd_work;
	struct work_struct	tx_work;
>>>>>>> refs/remotes/origin/master
=======
	struct tasklet_struct	cmd_task;
	struct tasklet_struct	rx_task;
	struct tasklet_struct	tx_task;
>>>>>>> refs/remotes/origin/cm-11.0

	struct sk_buff_head	rx_q;
	struct sk_buff_head	raw_q;
	struct sk_buff_head	cmd_q;

<<<<<<< HEAD
=======
	struct sk_buff		*recv_evt;
>>>>>>> refs/remotes/origin/master
	struct sk_buff		*sent_cmd;
	struct sk_buff		*reassembly[NUM_REASSEMBLY];

	struct mutex		req_lock;
	wait_queue_head_t	req_wait_q;
	__u32			req_status;
	__u32			req_result;

<<<<<<< HEAD
	__u16			init_last_cmd;

	struct crypto_blkcipher	*tfm;
<<<<<<< HEAD

	struct inquiry_cache	inq_cache;
	struct hci_conn_hash	conn_hash;
	struct hci_chan_list	chan_list;
=======
	struct list_head	mgmt_pending;
=======
>>>>>>> refs/remotes/origin/cm-11.0

	struct inquiry_cache	inq_cache;
	struct hci_conn_hash	conn_hash;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
	struct hci_chan_list	chan_list;
>>>>>>> refs/remotes/origin/cm-11.0
	struct list_head	blacklist;

	struct list_head	uuids;

	struct list_head	link_keys;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct list_head	remote_oob_data;

	struct list_head	adv_entries;
	rwlock_t		adv_entries_lock;
	struct timer_list	adv_timer;

	struct timer_list	disco_timer;
	struct timer_list	disco_le_timer;
	__u8			disco_state;
	int			disco_int_phase;
	int			disco_int_count;

	struct hci_dev_stats	stat;

	struct sk_buff_head	driver_init;

	void			*driver_data;
	void			*core_data;

=======
	struct list_head	long_term_keys;

	struct list_head	remote_oob_data;

	struct hci_dev_stats	stat;

>>>>>>> refs/remotes/origin/master
	atomic_t		promisc;

	struct dentry		*debugfs;

<<<<<<< HEAD
	struct device		*parent;
=======
>>>>>>> refs/remotes/origin/master
	struct device		dev;

	struct rfkill		*rfkill;

<<<<<<< HEAD
<<<<<<< HEAD
	struct module		*owner;
=======
	unsigned long		dev_flags;

	struct delayed_work	le_scan_disable;

	__s8			adv_tx_power;
	__u8			adv_data[HCI_MAX_AD_LENGTH];
	__u8			adv_data_len;
	__u8			scan_rsp_data[HCI_MAX_AD_LENGTH];
	__u8			scan_rsp_data_len;
>>>>>>> refs/remotes/origin/master
=======
	struct module		*owner;
>>>>>>> refs/remotes/origin/cm-11.0

	int (*open)(struct hci_dev *hdev);
	int (*close)(struct hci_dev *hdev);
	int (*flush)(struct hci_dev *hdev);
<<<<<<< HEAD
	int (*send)(struct sk_buff *skb);
	void (*destruct)(struct hci_dev *hdev);
	void (*notify)(struct hci_dev *hdev, unsigned int evt);
	int (*ioctl)(struct hci_dev *hdev, unsigned int cmd, unsigned long arg);
};

=======
	int (*setup)(struct hci_dev *hdev);
	int (*send)(struct hci_dev *hdev, struct sk_buff *skb);
	void (*notify)(struct hci_dev *hdev, unsigned int evt);
};

#define HCI_PHY_HANDLE(handle)	(handle & 0xff)

>>>>>>> refs/remotes/origin/master
struct hci_conn {
	struct list_head list;

	atomic_t	refcnt;
<<<<<<< HEAD
<<<<<<< HEAD
	spinlock_t	lock;

	bdaddr_t	dst;
	__u8		 dst_id;
	__u8            dst_type;
=======

	bdaddr_t	dst;
	__u8		dst_type;
	bdaddr_t	src;
	__u8		src_type;
>>>>>>> refs/remotes/origin/master
=======
	spinlock_t	lock;

	bdaddr_t	dst;
	__u8		 dst_id;
	__u8            dst_type;
>>>>>>> refs/remotes/origin/cm-11.0
	__u16		handle;
	__u16		state;
	__u8		mode;
	__u8		type;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	__u8		out;
	__u8		attempt;
	__u8		dev_class[3];
	__u8		features[8];
	__u8		ssp_mode;
	__u16		interval;
	__u16		pkt_type;
	__u16		link_policy;
	__u32		link_mode;
<<<<<<< HEAD
=======
	bool		out;
	__u8		attempt;
	__u8		dev_class[3];
	__u8		features[HCI_MAX_PAGES][8];
	__u16		pkt_type;
	__u16		link_policy;
	__u32		link_mode;
	__u8		key_type;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	__u8		auth_type;
	__u8		sec_level;
	__u8		pending_sec_level;
	__u8		pin_length;
	__u8		enc_key_size;
	__u8		io_capability;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	__u8		auth_initiator;
	__u8		power_save;
	__u16		disc_timeout;
	__u16		conn_timeout;
	unsigned long	pend;
<<<<<<< HEAD

	__u8		remote_cap;
	__u8		remote_oob;
	__u8		remote_auth;

	__s8	rssi_threshold;
	__u16	rssi_update_interval;
	__u8	rssi_update_thresh_exceed;
=======
	__u32		passkey_notify;
	__u8		passkey_entered;
	__u16		disc_timeout;
	__u16		setting;
	unsigned long	flags;
=======
>>>>>>> refs/remotes/origin/cm-11.0

	__u8		remote_cap;
	__u8		remote_oob;
	__u8		remote_auth;
<<<<<<< HEAD
	__u8		remote_id;
	bool		flush_key;
>>>>>>> refs/remotes/origin/master
=======

	__s8	rssi_threshold;
	__u16	rssi_update_interval;
	__u8	rssi_update_thresh_exceed;
>>>>>>> refs/remotes/origin/cm-11.0

	unsigned int	sent;

	struct sk_buff_head data_q;
<<<<<<< HEAD
<<<<<<< HEAD

	struct timer_list disc_timer;
	struct timer_list idle_timer;
	struct delayed_work	rssi_update_work;
	struct timer_list encrypt_pause_timer;

	struct work_struct work_add;
	struct work_struct work_del;
<<<<<<< HEAD
=======
	struct wake_lock idle_lock;
>>>>>>> refs/remotes/origin/cm-10.0
	struct device	dev;
	atomic_t	devref;
=======
	struct list_head chan_list;

	struct delayed_work disc_work;
	struct delayed_work auto_accept_work;
	struct delayed_work idle_work;
=======

	struct timer_list disc_timer;
	struct timer_list idle_timer;
	struct delayed_work	rssi_update_work;
	struct timer_list encrypt_pause_timer;
>>>>>>> refs/remotes/origin/cm-11.0

	struct work_struct work_add;
	struct work_struct work_del;
	struct wake_lock idle_lock;
	struct device	dev;
>>>>>>> refs/remotes/origin/master

	struct hci_dev	*hdev;
	void		*l2cap_data;
	void		*sco_data;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	void		*priv;

	__u8             link_key[16];
	__u8             key_type;
<<<<<<< HEAD

	struct hci_conn	*link;

	/* Low Energy SMP pairing data */
	__u8		oob; /* OOB pairing supported */
	__u8		tk_valid; /* TK value is valid */
	__u8		cfm_pending; /* CONFIRM cmd may be sent */
	__u8		preq[7]; /* Pairing Request */
	__u8		prsp[7]; /* Pairing Response */
	__u8		prnd[16]; /* Pairing Random */
	__u8		pcnf[16]; /* Pairing Confirm */
	__u8		tk[16]; /* Temporary Key */
	__u8		smp_key_size;
	__u8		sec_req;
	__u8		auth;
	void		*smp_conn;
	struct timer_list smp_timer;
	__u8		conn_valid;
<<<<<<< HEAD
	__u8		hidp_session_valid;
=======
>>>>>>> refs/remotes/origin/cm-10.0

=======
	void		*smp_conn;
	struct amp_mgr	*amp_mgr;
=======
>>>>>>> refs/remotes/origin/cm-11.0

	struct hci_conn	*link;
>>>>>>> refs/remotes/origin/master

	/* Low Energy SMP pairing data */
	__u8		oob; /* OOB pairing supported */
	__u8		tk_valid; /* TK value is valid */
	__u8		cfm_pending; /* CONFIRM cmd may be sent */
	__u8		preq[7]; /* Pairing Request */
	__u8		prsp[7]; /* Pairing Response */
	__u8		prnd[16]; /* Pairing Random */
	__u8		pcnf[16]; /* Pairing Confirm */
	__u8		tk[16]; /* Temporary Key */
	__u8		smp_key_size;
	__u8		sec_req;
	__u8		auth;
	void		*smp_conn;
	struct timer_list smp_timer;
	__u8		conn_valid;


	void (*connect_cfm_cb)	(struct hci_conn *conn, u8 status);
	void (*security_cfm_cb)	(struct hci_conn *conn, u8 status);
	void (*disconn_cfm_cb)	(struct hci_conn *conn, u8 reason);
};

struct hci_chan {
	struct list_head list;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct hci_dev	*hdev;
	__u16		state;
	atomic_t	refcnt;
	__u16		ll_handle;
	struct hci_ext_fs	tx_fs;
	struct hci_ext_fs	rx_fs;
	struct hci_conn	*conn;
	void		*l2cap_sk;
<<<<<<< HEAD
};

extern struct hci_proto *hci_proto[];
=======
	__u16 handle;
	struct hci_conn *conn;
	struct sk_buff_head data_q;
	unsigned int	sent;
	__u8		state;
};

>>>>>>> refs/remotes/origin/master
=======
};

extern struct hci_proto *hci_proto[];
>>>>>>> refs/remotes/origin/cm-11.0
extern struct list_head hci_dev_list;
extern struct list_head hci_cb_list;
extern rwlock_t hci_dev_list_lock;
extern rwlock_t hci_cb_list_lock;

<<<<<<< HEAD
<<<<<<< HEAD
/* ----- Inquiry cache ----- */
#define INQUIRY_CACHE_AGE_MAX   (HZ*30)   /* 30 seconds */
<<<<<<< HEAD
#define INQUIRY_ENTRY_AGE_MAX   (HZ*60*60)   /* 1 Hour */
=======
#define INQUIRY_ENTRY_AGE_MAX   (HZ*60)   /* 60 seconds */
>>>>>>> refs/remotes/origin/cm-10.0

#define inquiry_cache_lock(c)		spin_lock(&c->lock)
#define inquiry_cache_unlock(c)		spin_unlock(&c->lock)
#define inquiry_cache_lock_bh(c)	spin_lock_bh(&c->lock)
#define inquiry_cache_unlock_bh(c)	spin_unlock_bh(&c->lock)

static inline void inquiry_cache_init(struct hci_dev *hdev)
{
	struct inquiry_cache *c = &hdev->inq_cache;
	spin_lock_init(&c->lock);
	c->list = NULL;
}

static inline int inquiry_cache_empty(struct hci_dev *hdev)
{
	struct inquiry_cache *c = &hdev->inq_cache;
	return c->list == NULL;
=======
/* ----- HCI interface to upper protocols ----- */
int l2cap_connect_ind(struct hci_dev *hdev, bdaddr_t *bdaddr);
void l2cap_connect_cfm(struct hci_conn *hcon, u8 status);
int l2cap_disconn_ind(struct hci_conn *hcon);
void l2cap_disconn_cfm(struct hci_conn *hcon, u8 reason);
int l2cap_security_cfm(struct hci_conn *hcon, u8 status, u8 encrypt);
int l2cap_recv_acldata(struct hci_conn *hcon, struct sk_buff *skb, u16 flags);

int sco_connect_ind(struct hci_dev *hdev, bdaddr_t *bdaddr, __u8 *flags);
void sco_connect_cfm(struct hci_conn *hcon, __u8 status);
void sco_disconn_cfm(struct hci_conn *hcon, __u8 reason);
int sco_recv_scodata(struct hci_conn *hcon, struct sk_buff *skb);

=======
>>>>>>> refs/remotes/origin/cm-11.0
/* ----- Inquiry cache ----- */
#define INQUIRY_CACHE_AGE_MAX   (HZ*30)   /* 30 seconds */
#define INQUIRY_ENTRY_AGE_MAX   (HZ*60*60)   /* 1 Hour */

#define inquiry_cache_lock(c)		spin_lock(&c->lock)
#define inquiry_cache_unlock(c)		spin_unlock(&c->lock)
#define inquiry_cache_lock_bh(c)	spin_lock_bh(&c->lock)
#define inquiry_cache_unlock_bh(c)	spin_unlock_bh(&c->lock)

static inline void inquiry_cache_init(struct hci_dev *hdev)
{
	struct inquiry_cache *c = &hdev->inq_cache;
	spin_lock_init(&c->lock);
	c->list = NULL;
}

static inline int inquiry_cache_empty(struct hci_dev *hdev)
{
<<<<<<< HEAD
	return list_empty(&hdev->discovery.all);
>>>>>>> refs/remotes/origin/master
=======
	struct inquiry_cache *c = &hdev->inq_cache;
	return c->list == NULL;
>>>>>>> refs/remotes/origin/cm-11.0
}

static inline long inquiry_cache_age(struct hci_dev *hdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct inquiry_cache *c = &hdev->inq_cache;
=======
	struct discovery_state *c = &hdev->discovery;
>>>>>>> refs/remotes/origin/master
=======
	struct inquiry_cache *c = &hdev->inq_cache;
>>>>>>> refs/remotes/origin/cm-11.0
	return jiffies - c->timestamp;
}

static inline long inquiry_entry_age(struct inquiry_entry *e)
{
	return jiffies - e->timestamp;
}

<<<<<<< HEAD
<<<<<<< HEAD
struct inquiry_entry *hci_inquiry_cache_lookup(struct hci_dev *hdev, bdaddr_t *bdaddr);
void hci_inquiry_cache_update(struct hci_dev *hdev, struct inquiry_data *data);
=======
struct inquiry_entry *hci_inquiry_cache_lookup(struct hci_dev *hdev,
					       bdaddr_t *bdaddr);
struct inquiry_entry *hci_inquiry_cache_lookup_unknown(struct hci_dev *hdev,
						       bdaddr_t *bdaddr);
struct inquiry_entry *hci_inquiry_cache_lookup_resolve(struct hci_dev *hdev,
						       bdaddr_t *bdaddr,
						       int state);
void hci_inquiry_cache_update_resolve(struct hci_dev *hdev,
				      struct inquiry_entry *ie);
bool hci_inquiry_cache_update(struct hci_dev *hdev, struct inquiry_data *data,
			      bool name_known, bool *ssp);
void hci_inquiry_cache_flush(struct hci_dev *hdev);
>>>>>>> refs/remotes/origin/master
=======
struct inquiry_entry *hci_inquiry_cache_lookup(struct hci_dev *hdev, bdaddr_t *bdaddr);
void hci_inquiry_cache_update(struct hci_dev *hdev, struct inquiry_data *data);
>>>>>>> refs/remotes/origin/cm-11.0

/* ----- HCI Connections ----- */
enum {
	HCI_CONN_AUTH_PEND,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	HCI_CONN_REAUTH_PEND,
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	HCI_CONN_ENCRYPT_PEND,
	HCI_CONN_RSWITCH_PEND,
	HCI_CONN_MODE_CHANGE_PEND,
	HCI_CONN_SCO_SETUP_PEND,
<<<<<<< HEAD
<<<<<<< HEAD
};

static inline void hci_conn_hash_init(struct hci_dev *hdev)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	INIT_LIST_HEAD(&h->list);
	spin_lock_init(&h->lock);
	h->acl_num = 0;
	h->sco_num = 0;
=======
	HCI_CONN_LE_SMP_PEND,
	HCI_CONN_MGMT_CONNECTED,
	HCI_CONN_SSP_ENABLED,
	HCI_CONN_POWER_SAVE,
	HCI_CONN_REMOTE_OOB,
};

static inline bool hci_conn_ssp_enabled(struct hci_conn *conn)
{
	struct hci_dev *hdev = conn->hdev;
	return test_bit(HCI_SSP_ENABLED, &hdev->dev_flags) &&
	       test_bit(HCI_CONN_SSP_ENABLED, &conn->flags);
>>>>>>> refs/remotes/origin/master
=======
};

static inline void hci_conn_hash_init(struct hci_dev *hdev)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	INIT_LIST_HEAD(&h->list);
	spin_lock_init(&h->lock);
	h->acl_num = 0;
	h->sco_num = 0;
>>>>>>> refs/remotes/origin/cm-11.0
}

static inline void hci_conn_hash_add(struct hci_dev *hdev, struct hci_conn *c)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
<<<<<<< HEAD
<<<<<<< HEAD
	list_add(&c->list, &h->list);
=======
	list_add_rcu(&c->list, &h->list);
>>>>>>> refs/remotes/origin/master
=======
	list_add(&c->list, &h->list);
>>>>>>> refs/remotes/origin/cm-11.0
	switch (c->type) {
	case ACL_LINK:
		h->acl_num++;
		break;
<<<<<<< HEAD
=======
	case AMP_LINK:
		h->amp_num++;
		break;
>>>>>>> refs/remotes/origin/master
	case LE_LINK:
		h->le_num++;
		break;
	case SCO_LINK:
	case ESCO_LINK:
		h->sco_num++;
		break;
	}
}

static inline void hci_conn_hash_del(struct hci_dev *hdev, struct hci_conn *c)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
<<<<<<< HEAD
<<<<<<< HEAD
	list_del(&c->list);
=======

	list_del_rcu(&c->list);
	synchronize_rcu();

>>>>>>> refs/remotes/origin/master
=======
	list_del(&c->list);
>>>>>>> refs/remotes/origin/cm-11.0
	switch (c->type) {
	case ACL_LINK:
		h->acl_num--;
		break;
<<<<<<< HEAD
=======
	case AMP_LINK:
		h->amp_num--;
		break;
>>>>>>> refs/remotes/origin/master
	case LE_LINK:
		h->le_num--;
		break;
	case SCO_LINK:
	case ESCO_LINK:
		h->sco_num--;
		break;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static inline unsigned int hci_conn_num(struct hci_dev *hdev, __u8 type)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	switch (type) {
	case ACL_LINK:
		return h->acl_num;
	case AMP_LINK:
		return h->amp_num;
	case LE_LINK:
		return h->le_num;
	case SCO_LINK:
	case ESCO_LINK:
		return h->sco_num;
	default:
		return 0;
	}
}

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static inline struct hci_conn *hci_conn_hash_lookup_handle(struct hci_dev *hdev,
								__u16 handle)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head *p;
	struct hci_conn  *c;

	list_for_each(p, &h->list) {
		c = list_entry(p, struct hci_conn, list);
		if (c->handle == handle)
			return c;
	}
	return NULL;
}

static inline void hci_chan_list_init(struct hci_dev *hdev)
{
	struct hci_chan_list *h = &hdev->chan_list;
	INIT_LIST_HEAD(&h->list);
	spin_lock_init(&h->lock);
=======
=======
	struct list_head *p;
>>>>>>> refs/remotes/origin/cm-11.0
	struct hci_conn  *c;

	list_for_each(p, &h->list) {
		c = list_entry(p, struct hci_conn, list);
		if (c->handle == handle)
			return c;
	}
	return NULL;
>>>>>>> refs/remotes/origin/master
}

static inline void hci_chan_list_init(struct hci_dev *hdev)
{
	struct hci_chan_list *h = &hdev->chan_list;
	INIT_LIST_HEAD(&h->list);
	spin_lock_init(&h->lock);
}

static inline struct hci_conn *hci_conn_hash_lookup_ba(struct hci_dev *hdev,
							__u8 type, bdaddr_t *ba)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head *p;
	struct hci_conn  *c;

	list_for_each(p, &h->list) {
		c = list_entry(p, struct hci_conn, list);
		if (c->type == type && !bacmp(&c->dst, ba))
			return c;
	}
	return NULL;
}

static inline struct hci_conn *hci_conn_hash_lookup_id(struct hci_dev *hdev,
					bdaddr_t *ba, __u8 id)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	struct list_head *p;
	struct hci_conn  *c;

	list_for_each(p, &h->list) {
		c = list_entry(p, struct hci_conn, list);
		if (!bacmp(&c->dst, ba) && (c->dst_id == id))
			return c;
	}
=======
=======
	struct list_head *p;
>>>>>>> refs/remotes/origin/cm-11.0
	struct hci_conn  *c;

	list_for_each(p, &h->list) {
		c = list_entry(p, struct hci_conn, list);
		if (c->type == type && !bacmp(&c->dst, ba))
			return c;
	}
	return NULL;
}

static inline struct hci_conn *hci_conn_hash_lookup_id(struct hci_dev *hdev,
					bdaddr_t *ba, __u8 id)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
	struct list_head *p;
	struct hci_conn  *c;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
	list_for_each(p, &h->list) {
		c = list_entry(p, struct hci_conn, list);
		if (!bacmp(&c->dst, ba) && (c->dst_id == id))
			return c;
	}
>>>>>>> refs/remotes/origin/cm-11.0
	return NULL;
}

static inline struct hci_conn *hci_conn_hash_lookup_state(struct hci_dev *hdev,
							__u8 type, __u16 state)
{
	struct hci_conn_hash *h = &hdev->conn_hash;
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head *p;
	struct hci_conn  *c;

	list_for_each(p, &h->list) {
		c = list_entry(p, struct hci_conn, list);
		if (c->type == type && c->state == state)
			return c;
	}
	return NULL;
}

static inline struct hci_chan *hci_chan_list_lookup_handle(struct hci_dev *hdev,
					__u16 handle)
{
	struct hci_chan_list *l = &hdev->chan_list;
	struct list_head *p;
	struct hci_chan  *c;

	list_for_each(p, &l->list) {
		c = list_entry(p, struct hci_chan, list);
		if (c->ll_handle == handle)
			return c;
	}
	return NULL;
}

static inline struct hci_chan *hci_chan_list_lookup_id(struct hci_dev *hdev,
					__u8 handle)
{
	struct hci_chan_list *l = &hdev->chan_list;
	struct list_head *p;
	struct hci_chan  *c;

	list_for_each(p, &l->list) {
		c = list_entry(p, struct hci_chan, list);
		if (c->conn->handle == handle)
			return c;
	}
	return NULL;
}

void hci_acl_connect(struct hci_conn *conn);
void hci_acl_disconn(struct hci_conn *conn, __u8 reason);
void hci_add_sco(struct hci_conn *conn, __u16 handle);
void hci_setup_sync(struct hci_conn *conn, __u16 handle);
void hci_sco_setup(struct hci_conn *conn, __u8 status);

struct hci_conn *hci_conn_add(struct hci_dev *hdev, int type,
					__u16 pkt_type, bdaddr_t *dst);
struct hci_conn *hci_le_conn_add(struct hci_dev *hdev, bdaddr_t *dst,
							__u8 addr_type);
int hci_conn_del(struct hci_conn *conn);
void hci_conn_hash_flush(struct hci_dev *hdev, u8 is_process);
void hci_conn_check_pending(struct hci_dev *hdev);

struct hci_chan *hci_chan_add(struct hci_dev *hdev);
int hci_chan_del(struct hci_chan *chan);
static inline void hci_chan_hold(struct hci_chan *chan)
{
	atomic_inc(&chan->refcnt);
}
int hci_chan_put(struct hci_chan *chan);

struct hci_chan *hci_chan_create(struct hci_chan *chan,
				struct hci_ext_fs *tx_fs,
				struct hci_ext_fs *rx_fs);
void hci_chan_modify(struct hci_chan *chan,
				struct hci_ext_fs *tx_fs,
				struct hci_ext_fs *rx_fs);

struct hci_conn *hci_connect(struct hci_dev *hdev, int type,
					__u16 pkt_type, bdaddr_t *dst,
					__u8 sec_level, __u8 auth_type);
struct hci_conn *hci_le_connect(struct hci_dev *hdev, __u16 pkt_type,
					bdaddr_t *dst, __u8 sec_level,
					__u8 auth_type,
					struct bt_le_params *le_params);
<<<<<<< HEAD
void hci_le_add_dev_white_list(struct hci_dev *hdev, bdaddr_t *dst);
void hci_le_remove_dev_white_list(struct hci_dev *hdev, bdaddr_t *dst);
void hci_le_cancel_create_connect(struct hci_dev *hdev, bdaddr_t *dst);
=======
>>>>>>> refs/remotes/origin/cm-10.0
int hci_conn_check_link_mode(struct hci_conn *conn);
int hci_conn_security(struct hci_conn *conn, __u8 sec_level, __u8 auth_type);
int hci_conn_change_link_key(struct hci_conn *conn);
int hci_conn_switch_role(struct hci_conn *conn, __u8 role);
void hci_disconnect(struct hci_conn *conn, __u8 reason);
void hci_disconnect_amp(struct hci_conn *conn, __u8 reason);

void hci_conn_enter_active_mode(struct hci_conn *conn, __u8 force_active);
void hci_conn_enter_sniff_mode(struct hci_conn *conn);

void hci_conn_hold_device(struct hci_conn *conn);
void hci_conn_put_device(struct hci_conn *conn);

void hci_conn_set_rssi_reporter(struct hci_conn *conn,
		s8 rssi_threshold, u16 interval, u8 updateOnThreshExceed);
void hci_conn_unset_rssi_reporter(struct hci_conn *conn);

static inline void hci_conn_hold(struct hci_conn *conn)
{
	atomic_inc(&conn->refcnt);
	del_timer(&conn->disc_timer);
}

static inline void hci_conn_put(struct hci_conn *conn)
{
	if (atomic_dec_and_test(&conn->refcnt)) {
		unsigned long timeo;
		if (conn->type == ACL_LINK || conn->type == LE_LINK) {
			del_timer(&conn->idle_timer);
			if (conn->state == BT_CONNECTED) {
				timeo = msecs_to_jiffies(conn->disc_timeout);
				if (!conn->out)
					timeo *= 4;
			} else
				timeo = msecs_to_jiffies(10);
		} else
			timeo = msecs_to_jiffies(10);
		mod_timer(&conn->disc_timer, jiffies + timeo);
=======
=======
	struct list_head *p;
>>>>>>> refs/remotes/origin/cm-11.0
	struct hci_conn  *c;

	list_for_each(p, &h->list) {
		c = list_entry(p, struct hci_conn, list);
		if (c->type == type && c->state == state)
			return c;
	}
	return NULL;
}

static inline struct hci_chan *hci_chan_list_lookup_handle(struct hci_dev *hdev,
					__u16 handle)
{
	struct hci_chan_list *l = &hdev->chan_list;
	struct list_head *p;
	struct hci_chan  *c;

	list_for_each(p, &l->list) {
		c = list_entry(p, struct hci_chan, list);
		if (c->ll_handle == handle)
			return c;
	}
	return NULL;
}

static inline struct hci_chan *hci_chan_list_lookup_id(struct hci_dev *hdev,
					__u8 handle)
{
	struct hci_chan_list *l = &hdev->chan_list;
	struct list_head *p;
	struct hci_chan  *c;

	list_for_each(p, &l->list) {
		c = list_entry(p, struct hci_chan, list);
		if (c->conn->handle == handle)
			return c;
	}
	return NULL;
}

void hci_disconnect(struct hci_conn *conn, __u8 reason);
bool hci_setup_sync(struct hci_conn *conn, __u16 handle);
void hci_sco_setup(struct hci_conn *conn, __u8 status);

struct hci_conn *hci_conn_add(struct hci_dev *hdev, int type,
					__u16 pkt_type, bdaddr_t *dst);
struct hci_conn *hci_le_conn_add(struct hci_dev *hdev, bdaddr_t *dst,
							__u8 addr_type);
int hci_conn_del(struct hci_conn *conn);
void hci_conn_hash_flush(struct hci_dev *hdev, u8 is_process);
void hci_conn_check_pending(struct hci_dev *hdev);

<<<<<<< HEAD
struct hci_chan *hci_chan_create(struct hci_conn *conn);
void hci_chan_del(struct hci_chan *chan);
void hci_chan_list_flush(struct hci_conn *conn);
struct hci_chan *hci_chan_lookup_handle(struct hci_dev *hdev, __u16 handle);

struct hci_conn *hci_connect(struct hci_dev *hdev, int type, bdaddr_t *dst,
			     __u8 dst_type, __u8 sec_level, __u8 auth_type);
struct hci_conn *hci_connect_sco(struct hci_dev *hdev, int type, bdaddr_t *dst,
				 __u16 setting);
=======
struct hci_chan *hci_chan_add(struct hci_dev *hdev);
int hci_chan_del(struct hci_chan *chan);
static inline void hci_chan_hold(struct hci_chan *chan)
{
	atomic_inc(&chan->refcnt);
}
int hci_chan_put(struct hci_chan *chan);

struct hci_chan *hci_chan_create(struct hci_chan *chan,
				struct hci_ext_fs *tx_fs,
				struct hci_ext_fs *rx_fs);
void hci_chan_modify(struct hci_chan *chan,
				struct hci_ext_fs *tx_fs,
				struct hci_ext_fs *rx_fs);

struct hci_conn *hci_connect(struct hci_dev *hdev, int type,
					__u16 pkt_type, bdaddr_t *dst,
					__u8 sec_level, __u8 auth_type);
struct hci_conn *hci_le_connect(struct hci_dev *hdev, __u16 pkt_type,
					bdaddr_t *dst, __u8 sec_level,
					__u8 auth_type,
					struct bt_le_params *le_params);
void hci_le_add_dev_white_list(struct hci_dev *hdev, bdaddr_t *dst);
void hci_le_remove_dev_white_list(struct hci_dev *hdev, bdaddr_t *dst);
void hci_le_cancel_create_connect(struct hci_dev *hdev, bdaddr_t *dst);
>>>>>>> refs/remotes/origin/cm-11.0
int hci_conn_check_link_mode(struct hci_conn *conn);
int hci_conn_security(struct hci_conn *conn, __u8 sec_level, __u8 auth_type);
int hci_conn_change_link_key(struct hci_conn *conn);
int hci_conn_switch_role(struct hci_conn *conn, __u8 role);
void hci_disconnect(struct hci_conn *conn, __u8 reason);
void hci_disconnect_amp(struct hci_conn *conn, __u8 reason);

void hci_conn_enter_active_mode(struct hci_conn *conn, __u8 force_active);
void hci_conn_enter_sniff_mode(struct hci_conn *conn);

/*
 * hci_conn_get() and hci_conn_put() are used to control the life-time of an
 * "hci_conn" object. They do not guarantee that the hci_conn object is running,
 * working or anything else. They just guarantee that the object is available
 * and can be dereferenced. So you can use its locks, local variables and any
 * other constant data.
 * Before accessing runtime data, you _must_ lock the object and then check that
 * it is still running. As soon as you release the locks, the connection might
 * get dropped, though.
 *
 * On the other hand, hci_conn_hold() and hci_conn_drop() are used to control
 * how long the underlying connection is held. So every channel that runs on the
 * hci_conn object calls this to prevent the connection from disappearing. As
 * long as you hold a device, you must also guarantee that you have a valid
 * reference to the device via hci_conn_get() (or the initial reference from
 * hci_conn_add()).
 * The hold()/drop() ref-count is known to drop below 0 sometimes, which doesn't
 * break because nobody cares for that. But this means, we cannot use
 * _get()/_drop() in it, but require the caller to have a valid ref (FIXME).
 */

static inline void hci_conn_get(struct hci_conn *conn)
{
	get_device(&conn->dev);
}

static inline void hci_conn_put(struct hci_conn *conn)
{
	put_device(&conn->dev);
}

void hci_conn_set_rssi_reporter(struct hci_conn *conn,
		s8 rssi_threshold, u16 interval, u8 updateOnThreshExceed);
void hci_conn_unset_rssi_reporter(struct hci_conn *conn);

static inline void hci_conn_hold(struct hci_conn *conn)
{
	BT_DBG("hcon %p orig refcnt %d", conn, atomic_read(&conn->refcnt));

	atomic_inc(&conn->refcnt);
	del_timer(&conn->disc_timer);
}

static inline void hci_conn_drop(struct hci_conn *conn)
{
	BT_DBG("hcon %p orig refcnt %d", conn, atomic_read(&conn->refcnt));

	if (atomic_dec_and_test(&conn->refcnt)) {
		unsigned long timeo;

		switch (conn->type) {
		case ACL_LINK:
		case LE_LINK:
			cancel_delayed_work(&conn->idle_work);
			if (conn->state == BT_CONNECTED) {
				timeo = conn->disc_timeout;
				if (!conn->out)
					timeo *= 4;
			} else
				timeo = msecs_to_jiffies(10);
<<<<<<< HEAD
			}
			break;

		case AMP_LINK:
			timeo = conn->disc_timeout;
			break;

		default:
			timeo = msecs_to_jiffies(10);
			break;
		}

		cancel_delayed_work(&conn->disc_work);
		queue_delayed_work(conn->hdev->workqueue,
				   &conn->disc_work, timeo);
>>>>>>> refs/remotes/origin/master
=======
		} else
			timeo = msecs_to_jiffies(10);
		mod_timer(&conn->disc_timer, jiffies + timeo);
>>>>>>> refs/remotes/origin/cm-11.0
	}
}

/* ----- HCI Devices ----- */
<<<<<<< HEAD
<<<<<<< HEAD
static inline void __hci_dev_put(struct hci_dev *d)
{
	if (atomic_dec_and_test(&d->refcnt))
		d->destruct(d);
}

static inline void hci_dev_put(struct hci_dev *d)
{
	__hci_dev_put(d);
	module_put(d->owner);
}

static inline struct hci_dev *__hci_dev_hold(struct hci_dev *d)
{
	atomic_inc(&d->refcnt);
	return d;
}

static inline struct hci_dev *hci_dev_hold(struct hci_dev *d)
{
	if (try_module_get(d->owner))
		return __hci_dev_hold(d);
	return NULL;
}

#define hci_dev_lock(d)		spin_lock(&d->lock)
#define hci_dev_unlock(d)	spin_unlock(&d->lock)
#define hci_dev_lock_bh(d)	spin_lock_bh(&d->lock)
#define hci_dev_unlock_bh(d)	spin_unlock_bh(&d->lock)

struct hci_dev *hci_dev_get(int index);
struct hci_dev *hci_get_route(bdaddr_t *src, bdaddr_t *dst);
struct hci_dev *hci_dev_get_type(__u8 amp_type);
=======
static inline void hci_dev_put(struct hci_dev *d)
{
	BT_DBG("%s orig refcnt %d", d->name,
	       atomic_read(&d->dev.kobj.kref.refcount));

	put_device(&d->dev);
=======
static inline void __hci_dev_put(struct hci_dev *d)
{
	if (atomic_dec_and_test(&d->refcnt))
		d->destruct(d);
>>>>>>> refs/remotes/origin/cm-11.0
}

static inline void hci_dev_put(struct hci_dev *d)
{
<<<<<<< HEAD
	BT_DBG("%s orig refcnt %d", d->name,
	       atomic_read(&d->dev.kobj.kref.refcount));

	get_device(&d->dev);
	return d;
=======
	__hci_dev_put(d);
	module_put(d->owner);
>>>>>>> refs/remotes/origin/cm-11.0
}

static inline struct hci_dev *__hci_dev_hold(struct hci_dev *d)
{
	atomic_inc(&d->refcnt);
	return d;
}

static inline struct hci_dev *hci_dev_hold(struct hci_dev *d)
{
	if (try_module_get(d->owner))
		return __hci_dev_hold(d);
	return NULL;
}

#define hci_dev_lock(d)		spin_lock(&d->lock)
#define hci_dev_unlock(d)	spin_unlock(&d->lock)
#define hci_dev_lock_bh(d)	spin_lock_bh(&d->lock)
#define hci_dev_unlock_bh(d)	spin_unlock_bh(&d->lock)

struct hci_dev *hci_dev_get(int index);
<<<<<<< HEAD
struct hci_dev *hci_get_route(bdaddr_t *dst, bdaddr_t *src);
>>>>>>> refs/remotes/origin/master
=======
struct hci_dev *hci_get_route(bdaddr_t *src, bdaddr_t *dst);
struct hci_dev *hci_dev_get_type(__u8 amp_type);
>>>>>>> refs/remotes/origin/cm-11.0

struct hci_dev *hci_alloc_dev(void);
void hci_free_dev(struct hci_dev *hdev);
int hci_register_dev(struct hci_dev *hdev);
<<<<<<< HEAD
<<<<<<< HEAD
int hci_unregister_dev(struct hci_dev *hdev);
=======
void hci_unregister_dev(struct hci_dev *hdev);
>>>>>>> refs/remotes/origin/master
=======
int hci_unregister_dev(struct hci_dev *hdev);
>>>>>>> refs/remotes/origin/cm-11.0
int hci_suspend_dev(struct hci_dev *hdev);
int hci_resume_dev(struct hci_dev *hdev);
int hci_dev_open(__u16 dev);
int hci_dev_close(__u16 dev);
int hci_dev_reset(__u16 dev);
int hci_dev_reset_stat(__u16 dev);
int hci_dev_cmd(unsigned int cmd, void __user *arg);
int hci_get_dev_list(void __user *arg);
int hci_get_dev_info(void __user *arg);
int hci_get_conn_list(void __user *arg);
int hci_get_conn_info(struct hci_dev *hdev, void __user *arg);
int hci_get_auth_info(struct hci_dev *hdev, void __user *arg);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
int hci_set_auth_info(struct hci_dev *hdev, void __user *arg);
int hci_inquiry(void __user *arg);

struct bdaddr_list *hci_blacklist_lookup(struct hci_dev *hdev, bdaddr_t *bdaddr);
int hci_blacklist_clear(struct hci_dev *hdev);
<<<<<<< HEAD
=======
int hci_inquiry(void __user *arg);

struct bdaddr_list *hci_blacklist_lookup(struct hci_dev *hdev,
					 bdaddr_t *bdaddr, u8 type);
int hci_blacklist_clear(struct hci_dev *hdev);
int hci_blacklist_add(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 type);
int hci_blacklist_del(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 type);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

int hci_uuids_clear(struct hci_dev *hdev);

int hci_link_keys_clear(struct hci_dev *hdev);
struct link_key *hci_find_link_key(struct hci_dev *hdev, bdaddr_t *bdaddr);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
int hci_add_link_key(struct hci_dev *hdev, int new_key, bdaddr_t *bdaddr,
						u8 *key, u8 type, u8 pin_len);
struct link_key *hci_find_ltk(struct hci_dev *hdev, __le16 ediv, u8 rand[8]);
struct link_key *hci_find_link_key_type(struct hci_dev *hdev,
					bdaddr_t *bdaddr, u8 type);
int hci_add_ltk(struct hci_dev *hdev, int new_key, bdaddr_t *bdaddr, u8 type,
		u8 auth, u8 key_size, __le16 ediv, u8 rand[8], u8 ltk[16]);
<<<<<<< HEAD
=======
int hci_add_link_key(struct hci_dev *hdev, struct hci_conn *conn, int new_key,
		     bdaddr_t *bdaddr, u8 *val, u8 type, u8 pin_len);
struct smp_ltk *hci_find_ltk(struct hci_dev *hdev, __le16 ediv, u8 rand[8]);
int hci_add_ltk(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 addr_type, u8 type,
		int new_key, u8 authenticated, u8 tk[16], u8 enc_size,
		__le16 ediv, u8 rand[8]);
struct smp_ltk *hci_find_ltk_by_addr(struct hci_dev *hdev, bdaddr_t *bdaddr,
				     u8 addr_type);
int hci_remove_ltk(struct hci_dev *hdev, bdaddr_t *bdaddr);
int hci_smp_ltks_clear(struct hci_dev *hdev);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
int hci_remove_link_key(struct hci_dev *hdev, bdaddr_t *bdaddr);

int hci_remote_oob_data_clear(struct hci_dev *hdev);
struct oob_data *hci_find_remote_oob_data(struct hci_dev *hdev,
							bdaddr_t *bdaddr);
int hci_add_remote_oob_data(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 *hash,
								u8 *randomizer);
int hci_remove_remote_oob_data(struct hci_dev *hdev, bdaddr_t *bdaddr);

<<<<<<< HEAD
#define ADV_CLEAR_TIMEOUT (3*60*HZ) /* Three minutes */
int hci_adv_entries_clear(struct hci_dev *hdev);
struct adv_entry *hci_find_adv_entry(struct hci_dev *hdev, bdaddr_t *bdaddr);
int hci_add_adv_entry(struct hci_dev *hdev,
					struct hci_ev_le_advertising_info *ev);

void hci_del_off_timer(struct hci_dev *hdev);

void hci_event_packet(struct hci_dev *hdev, struct sk_buff *skb);

int hci_recv_frame(struct sk_buff *skb);
int hci_recv_fragment(struct hci_dev *hdev, int type, void *data, int count);
int hci_recv_stream_fragment(struct hci_dev *hdev, void *data, int count);

int hci_register_sysfs(struct hci_dev *hdev);
void hci_unregister_sysfs(struct hci_dev *hdev);
<<<<<<< HEAD
=======
void hci_event_packet(struct hci_dev *hdev, struct sk_buff *skb);

int hci_recv_frame(struct hci_dev *hdev, struct sk_buff *skb);
int hci_recv_fragment(struct hci_dev *hdev, int type, void *data, int count);
int hci_recv_stream_fragment(struct hci_dev *hdev, void *data, int count);

void hci_init_sysfs(struct hci_dev *hdev);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
void hci_conn_init_sysfs(struct hci_conn *conn);
void hci_conn_add_sysfs(struct hci_conn *conn);
void hci_conn_del_sysfs(struct hci_conn *conn);

<<<<<<< HEAD
#define SET_HCIDEV_DEV(hdev, pdev) ((hdev)->parent = (pdev))

/* ----- LMP capabilities ----- */
#define lmp_rswitch_capable(dev)   ((dev)->features[0] & LMP_RSWITCH)
#define lmp_encrypt_capable(dev)   ((dev)->features[0] & LMP_ENCRYPT)
#define lmp_sniff_capable(dev)     ((dev)->features[0] & LMP_SNIFF)
#define lmp_sniffsubr_capable(dev) ((dev)->features[5] & LMP_SNIFF_SUBR)
#define lmp_esco_capable(dev)      ((dev)->features[3] & LMP_ESCO)
#define lmp_ssp_capable(dev)       ((dev)->features[6] & LMP_SIMPLE_PAIR)
#define lmp_no_flush_capable(dev)  ((dev)->features[6] & LMP_NO_FLUSH)
#define lmp_le_capable(dev)        ((dev)->features[4] & LMP_LE)
<<<<<<< HEAD

/* ----- HCI protocols ----- */
struct hci_proto {
	char		*name;
	unsigned int	id;
	unsigned long	flags;

	void		*priv;

	int (*connect_ind)	(struct hci_dev *hdev, bdaddr_t *bdaddr, __u8 type);
	int (*connect_cfm)	(struct hci_conn *conn, __u8 status);
	int (*disconn_ind)	(struct hci_conn *conn);
	int (*disconn_cfm)	(struct hci_conn *conn, __u8 reason,
							__u8 is_process);
	int (*recv_acldata)	(struct hci_conn *conn, struct sk_buff *skb, __u16 flags);
	int (*recv_scodata)	(struct hci_conn *conn, struct sk_buff *skb);
	int (*security_cfm)	(struct hci_conn *conn, __u8 status, __u8 encrypt);
	int (*create_cfm)	(struct hci_chan *chan, __u8 status);
	int (*modify_cfm)	(struct hci_chan *chan, __u8 status);
	int (*destroy_cfm)	(struct hci_chan *chan, __u8 status);
};

static inline int hci_proto_connect_ind(struct hci_dev *hdev, bdaddr_t *bdaddr, __u8 type)
{
	register struct hci_proto *hp;
	int mask = 0;

	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->connect_ind)
		mask |= hp->connect_ind(hdev, bdaddr, type);

	hp = hci_proto[HCI_PROTO_SCO];
	if (hp && hp->connect_ind)
		mask |= hp->connect_ind(hdev, bdaddr, type);

	return mask;
=======
#define SET_HCIDEV_DEV(hdev, pdev) ((hdev)->dev.parent = (pdev))

/* ----- LMP capabilities ----- */
#define lmp_encrypt_capable(dev)   ((dev)->features[0][0] & LMP_ENCRYPT)
#define lmp_rswitch_capable(dev)   ((dev)->features[0][0] & LMP_RSWITCH)
#define lmp_hold_capable(dev)      ((dev)->features[0][0] & LMP_HOLD)
#define lmp_sniff_capable(dev)     ((dev)->features[0][0] & LMP_SNIFF)
#define lmp_park_capable(dev)      ((dev)->features[0][1] & LMP_PARK)
#define lmp_inq_rssi_capable(dev)  ((dev)->features[0][3] & LMP_RSSI_INQ)
#define lmp_esco_capable(dev)      ((dev)->features[0][3] & LMP_ESCO)
#define lmp_bredr_capable(dev)     (!((dev)->features[0][4] & LMP_NO_BREDR))
#define lmp_le_capable(dev)        ((dev)->features[0][4] & LMP_LE)
#define lmp_sniffsubr_capable(dev) ((dev)->features[0][5] & LMP_SNIFF_SUBR)
#define lmp_pause_enc_capable(dev) ((dev)->features[0][5] & LMP_PAUSE_ENC)
#define lmp_ext_inq_capable(dev)   ((dev)->features[0][6] & LMP_EXT_INQ)
#define lmp_le_br_capable(dev)     (!!((dev)->features[0][6] & LMP_SIMUL_LE_BR))
#define lmp_ssp_capable(dev)       ((dev)->features[0][6] & LMP_SIMPLE_PAIR)
#define lmp_no_flush_capable(dev)  ((dev)->features[0][6] & LMP_NO_FLUSH)
#define lmp_lsto_capable(dev)      ((dev)->features[0][7] & LMP_LSTO)
#define lmp_inq_tx_pwr_capable(dev) ((dev)->features[0][7] & LMP_INQ_TX_PWR)
#define lmp_ext_feat_capable(dev)  ((dev)->features[0][7] & LMP_EXTFEATURES)
#define lmp_transp_capable(dev)    ((dev)->features[0][2] & LMP_TRANSPARENT)

/* ----- Extended LMP capabilities ----- */
#define lmp_host_ssp_capable(dev)  ((dev)->features[1][0] & LMP_HOST_SSP)
#define lmp_host_le_capable(dev)   (!!((dev)->features[1][0] & LMP_HOST_LE))
#define lmp_host_le_br_capable(dev) (!!((dev)->features[1][0] & LMP_HOST_LE_BREDR))

/* ----- HCI protocols ----- */
#define HCI_PROTO_DEFER             0x01

static inline int hci_proto_connect_ind(struct hci_dev *hdev, bdaddr_t *bdaddr,
					__u8 type, __u8 *flags)
=======

/* ----- HCI protocols ----- */
struct hci_proto {
	char		*name;
	unsigned int	id;
	unsigned long	flags;

	void		*priv;

	int (*connect_ind)	(struct hci_dev *hdev, bdaddr_t *bdaddr, __u8 type);
	int (*connect_cfm)	(struct hci_conn *conn, __u8 status);
	int (*disconn_ind)	(struct hci_conn *conn);
	int (*disconn_cfm)	(struct hci_conn *conn, __u8 reason,
							__u8 is_process);
	int (*recv_acldata)	(struct hci_conn *conn, struct sk_buff *skb, __u16 flags);
	int (*recv_scodata)	(struct hci_conn *conn, struct sk_buff *skb);
	int (*security_cfm)	(struct hci_conn *conn, __u8 status, __u8 encrypt);
	int (*create_cfm)	(struct hci_chan *chan, __u8 status);
	int (*modify_cfm)	(struct hci_chan *chan, __u8 status);
	int (*destroy_cfm)	(struct hci_chan *chan, __u8 status);
};

static inline int hci_proto_connect_ind(struct hci_dev *hdev, bdaddr_t *bdaddr, __u8 type)
>>>>>>> refs/remotes/origin/cm-11.0
{
	register struct hci_proto *hp;
	int mask = 0;

<<<<<<< HEAD
	case SCO_LINK:
	case ESCO_LINK:
		return sco_connect_ind(hdev, bdaddr, flags);

	default:
		BT_ERR("unknown link type %d", type);
		return -EINVAL;
	}
>>>>>>> refs/remotes/origin/master
=======
	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->connect_ind)
		mask |= hp->connect_ind(hdev, bdaddr, type);

	hp = hci_proto[HCI_PROTO_SCO];
	if (hp && hp->connect_ind)
		mask |= hp->connect_ind(hdev, bdaddr, type);

	return mask;
>>>>>>> refs/remotes/origin/cm-11.0
}

static inline void hci_proto_connect_cfm(struct hci_conn *conn, __u8 status)
{
<<<<<<< HEAD
<<<<<<< HEAD
	register struct hci_proto *hp;

	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->connect_cfm)
		hp->connect_cfm(conn, status);

	hp = hci_proto[HCI_PROTO_SCO];
	if (hp && hp->connect_cfm)
		hp->connect_cfm(conn, status);
=======
	switch (conn->type) {
	case ACL_LINK:
	case LE_LINK:
		l2cap_connect_cfm(conn, status);
		break;
=======
	register struct hci_proto *hp;
>>>>>>> refs/remotes/origin/cm-11.0

	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->connect_cfm)
		hp->connect_cfm(conn, status);

<<<<<<< HEAD
	default:
		BT_ERR("unknown link type %d", conn->type);
		break;
	}
>>>>>>> refs/remotes/origin/master
=======
	hp = hci_proto[HCI_PROTO_SCO];
	if (hp && hp->connect_cfm)
		hp->connect_cfm(conn, status);
>>>>>>> refs/remotes/origin/cm-11.0

	if (conn->connect_cfm_cb)
		conn->connect_cfm_cb(conn, status);
}

static inline int hci_proto_disconn_ind(struct hci_conn *conn)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	register struct hci_proto *hp;
	int reason = 0x13;

	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->disconn_ind)
		reason = hp->disconn_ind(conn);

	hp = hci_proto[HCI_PROTO_SCO];
	if (hp && hp->disconn_ind)
		reason = hp->disconn_ind(conn);
<<<<<<< HEAD

	return reason;
}

static inline void hci_proto_disconn_cfm(struct hci_conn *conn, __u8 reason,
							__u8 is_process)
{
	register struct hci_proto *hp;

	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->disconn_cfm)
		hp->disconn_cfm(conn, reason, is_process);

	hp = hci_proto[HCI_PROTO_SCO];
	if (hp && hp->disconn_cfm)
		hp->disconn_cfm(conn, reason, is_process);
=======
	if (conn->type != ACL_LINK && conn->type != LE_LINK)
		return HCI_ERROR_REMOTE_USER_TERM;
=======
>>>>>>> refs/remotes/origin/cm-11.0

	return reason;
}

static inline void hci_proto_disconn_cfm(struct hci_conn *conn, __u8 reason,
							__u8 is_process)
{
	register struct hci_proto *hp;

	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->disconn_cfm)
		hp->disconn_cfm(conn, reason, is_process);

<<<<<<< HEAD
	/* L2CAP would be handled for BREDR chan */
	case AMP_LINK:
		break;

	default:
		BT_ERR("unknown link type %d", conn->type);
		break;
	}
>>>>>>> refs/remotes/origin/master
=======
	hp = hci_proto[HCI_PROTO_SCO];
	if (hp && hp->disconn_cfm)
		hp->disconn_cfm(conn, reason, is_process);
>>>>>>> refs/remotes/origin/cm-11.0

	if (conn->disconn_cfm_cb)
		conn->disconn_cfm_cb(conn, reason);
}

static inline void hci_proto_auth_cfm(struct hci_conn *conn, __u8 status)
{
<<<<<<< HEAD
<<<<<<< HEAD
	register struct hci_proto *hp;
	__u8 encrypt;

	if (test_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend))
		return;

	encrypt = (conn->link_mode & HCI_LM_ENCRYPT) ? 0x01 : 0x00;

	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->security_cfm)
		hp->security_cfm(conn, status, encrypt);

	hp = hci_proto[HCI_PROTO_SCO];
	if (hp && hp->security_cfm)
		hp->security_cfm(conn, status, encrypt);
=======
=======
	register struct hci_proto *hp;
>>>>>>> refs/remotes/origin/cm-11.0
	__u8 encrypt;

	if (test_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend))
		return;

	encrypt = (conn->link_mode & HCI_LM_ENCRYPT) ? 0x01 : 0x00;
<<<<<<< HEAD
	l2cap_security_cfm(conn, status, encrypt);
>>>>>>> refs/remotes/origin/master
=======

	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->security_cfm)
		hp->security_cfm(conn, status, encrypt);

	hp = hci_proto[HCI_PROTO_SCO];
	if (hp && hp->security_cfm)
		hp->security_cfm(conn, status, encrypt);
>>>>>>> refs/remotes/origin/cm-11.0

	if (conn->security_cfm_cb)
		conn->security_cfm_cb(conn, status);
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void hci_proto_encrypt_cfm(struct hci_conn *conn, __u8 status, __u8 encrypt)
{
	register struct hci_proto *hp;

	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->security_cfm)
		hp->security_cfm(conn, status, encrypt);

	hp = hci_proto[HCI_PROTO_SCO];
	if (hp && hp->security_cfm)
		hp->security_cfm(conn, status, encrypt);
=======
static inline void hci_proto_encrypt_cfm(struct hci_conn *conn, __u8 status,
								__u8 encrypt)
=======
static inline void hci_proto_encrypt_cfm(struct hci_conn *conn, __u8 status, __u8 encrypt)
>>>>>>> refs/remotes/origin/cm-11.0
{
	register struct hci_proto *hp;

	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->security_cfm)
		hp->security_cfm(conn, status, encrypt);

<<<<<<< HEAD
	l2cap_security_cfm(conn, status, encrypt);
>>>>>>> refs/remotes/origin/master
=======
	hp = hci_proto[HCI_PROTO_SCO];
	if (hp && hp->security_cfm)
		hp->security_cfm(conn, status, encrypt);
>>>>>>> refs/remotes/origin/cm-11.0

	if (conn->security_cfm_cb)
		conn->security_cfm_cb(conn, status);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
static inline void hci_proto_create_cfm(struct hci_chan *chan, __u8 status)
{
	register struct hci_proto *hp;

	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->create_cfm)
		hp->create_cfm(chan, status);
}

static inline void hci_proto_modify_cfm(struct hci_chan *chan, __u8 status)
{
	register struct hci_proto *hp;

	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->modify_cfm)
		hp->modify_cfm(chan, status);
}

static inline void hci_proto_destroy_cfm(struct hci_chan *chan, __u8 status)
{
	register struct hci_proto *hp;

	hp = hci_proto[HCI_PROTO_L2CAP];
	if (hp && hp->destroy_cfm)
		hp->destroy_cfm(chan, status);
}

int hci_register_proto(struct hci_proto *hproto);
int hci_unregister_proto(struct hci_proto *hproto);

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
/* ----- HCI callbacks ----- */
struct hci_cb {
	struct list_head list;

	char *name;

<<<<<<< HEAD
<<<<<<< HEAD
	void (*security_cfm)	(struct hci_conn *conn, __u8 status, __u8 encrypt);
=======
	void (*security_cfm)	(struct hci_conn *conn, __u8 status,
								__u8 encrypt);
>>>>>>> refs/remotes/origin/master
=======
	void (*security_cfm)	(struct hci_conn *conn, __u8 status, __u8 encrypt);
>>>>>>> refs/remotes/origin/cm-11.0
	void (*key_change_cfm)	(struct hci_conn *conn, __u8 status);
	void (*role_switch_cfm)	(struct hci_conn *conn, __u8 status, __u8 role);
};

static inline void hci_auth_cfm(struct hci_conn *conn, __u8 status)
{
<<<<<<< HEAD
	struct list_head *p;
=======
	struct hci_cb *cb;
>>>>>>> refs/remotes/origin/master
	__u8 encrypt;

	hci_proto_auth_cfm(conn, status);

<<<<<<< HEAD
<<<<<<< HEAD
	if (test_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend))
=======
	if (test_bit(HCI_CONN_ENCRYPT_PEND, &conn->flags))
>>>>>>> refs/remotes/origin/master
=======
	if (test_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend))
>>>>>>> refs/remotes/origin/cm-11.0
		return;

	encrypt = (conn->link_mode & HCI_LM_ENCRYPT) ? 0x01 : 0x00;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	read_lock_bh(&hci_cb_list_lock);
	list_for_each(p, &hci_cb_list) {
		struct hci_cb *cb = list_entry(p, struct hci_cb, list);
		if (cb->security_cfm)
			cb->security_cfm(conn, status, encrypt);
	}
	read_unlock_bh(&hci_cb_list_lock);
<<<<<<< HEAD
}

static inline void hci_encrypt_cfm(struct hci_conn *conn, __u8 status, __u8 encrypt)
{
	struct list_head *p;
=======
	read_lock(&hci_cb_list_lock);
	list_for_each_entry(cb, &hci_cb_list, list) {
		if (cb->security_cfm)
			cb->security_cfm(conn, status, encrypt);
	}
	read_unlock(&hci_cb_list_lock);
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static inline void hci_encrypt_cfm(struct hci_conn *conn, __u8 status, __u8 encrypt)
{
	struct hci_cb *cb;
>>>>>>> refs/remotes/origin/master

	if (conn->sec_level == BT_SECURITY_SDP)
		conn->sec_level = BT_SECURITY_LOW;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!status && encrypt && conn->pending_sec_level > conn->sec_level)
=======
	if (conn->pending_sec_level > conn->sec_level)
>>>>>>> refs/remotes/origin/master
=======
	if (!status && encrypt && conn->pending_sec_level > conn->sec_level)
>>>>>>> refs/remotes/origin/cm-11.0
		conn->sec_level = conn->pending_sec_level;

	hci_proto_encrypt_cfm(conn, status, encrypt);

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	read_lock_bh(&hci_cb_list_lock);
	list_for_each(p, &hci_cb_list) {
		struct hci_cb *cb = list_entry(p, struct hci_cb, list);
		if (cb->security_cfm)
			cb->security_cfm(conn, status, encrypt);
	}
	read_unlock_bh(&hci_cb_list_lock);
<<<<<<< HEAD
=======
	read_lock(&hci_cb_list_lock);
	list_for_each_entry(cb, &hci_cb_list, list) {
		if (cb->security_cfm)
			cb->security_cfm(conn, status, encrypt);
	}
	read_unlock(&hci_cb_list_lock);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static inline void hci_key_change_cfm(struct hci_conn *conn, __u8 status)
{
<<<<<<< HEAD
	struct list_head *p;

	read_lock_bh(&hci_cb_list_lock);
	list_for_each(p, &hci_cb_list) {
		struct hci_cb *cb = list_entry(p, struct hci_cb, list);
		if (cb->key_change_cfm)
			cb->key_change_cfm(conn, status);
	}
	read_unlock_bh(&hci_cb_list_lock);
<<<<<<< HEAD
}

static inline void hci_role_switch_cfm(struct hci_conn *conn, __u8 status, __u8 role)
{
	struct list_head *p;

	read_lock_bh(&hci_cb_list_lock);
	list_for_each(p, &hci_cb_list) {
		struct hci_cb *cb = list_entry(p, struct hci_cb, list);
		if (cb->role_switch_cfm)
			cb->role_switch_cfm(conn, status, role);
	}
	read_unlock_bh(&hci_cb_list_lock);
=======
	struct hci_cb *cb;

	read_lock(&hci_cb_list_lock);
	list_for_each_entry(cb, &hci_cb_list, list) {
		if (cb->key_change_cfm)
			cb->key_change_cfm(conn, status);
	}
	read_unlock(&hci_cb_list_lock);
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static inline void hci_role_switch_cfm(struct hci_conn *conn, __u8 status, __u8 role)
{
	struct hci_cb *cb;

<<<<<<< HEAD
	read_lock(&hci_cb_list_lock);
	list_for_each_entry(cb, &hci_cb_list, list) {
=======
	read_lock_bh(&hci_cb_list_lock);
	list_for_each(p, &hci_cb_list) {
		struct hci_cb *cb = list_entry(p, struct hci_cb, list);
>>>>>>> refs/remotes/origin/cm-11.0
		if (cb->role_switch_cfm)
			cb->role_switch_cfm(conn, status, role);
	}
	read_unlock_bh(&hci_cb_list_lock);
}

int hci_register_cb(struct hci_cb *hcb);
int hci_unregister_cb(struct hci_cb *hcb);

int hci_register_notifier(struct notifier_block *nb);
int hci_unregister_notifier(struct notifier_block *nb);

<<<<<<< HEAD
		data += field_len + 1;
	}

	return false;
>>>>>>> refs/remotes/origin/master
}
=======
/* AMP Manager event callbacks */
struct amp_mgr_cb {
	struct list_head list;
	void (*amp_cmd_complete_event) (struct hci_dev *hdev, __u16 opcode,
					struct sk_buff *skb);
	void (*amp_cmd_status_event) (struct hci_dev *hdev, __u16 opcode,
					__u8 status);
	void (*amp_event) (struct hci_dev *hdev, __u8 ev_code,
					struct sk_buff *skb);
};

void hci_amp_cmd_complete(struct hci_dev *hdev, __u16 opcode,
			struct sk_buff *skb);
void hci_amp_cmd_status(struct hci_dev *hdev, __u16 opcode, __u8 status);
void hci_amp_event_packet(struct hci_dev *hdev, __u8 ev_code,
			struct sk_buff *skb);
>>>>>>> refs/remotes/origin/cm-11.0

int hci_register_amp(struct amp_mgr_cb *acb);
int hci_unregister_amp(struct amp_mgr_cb *acb);

<<<<<<< HEAD
int hci_register_notifier(struct notifier_block *nb);
int hci_unregister_notifier(struct notifier_block *nb);

/* AMP Manager event callbacks */
struct amp_mgr_cb {
	struct list_head list;
	void (*amp_cmd_complete_event) (struct hci_dev *hdev, __u16 opcode,
					struct sk_buff *skb);
	void (*amp_cmd_status_event) (struct hci_dev *hdev, __u16 opcode,
					__u8 status);
	void (*amp_event) (struct hci_dev *hdev, __u8 ev_code,
					struct sk_buff *skb);
};

void hci_amp_cmd_complete(struct hci_dev *hdev, __u16 opcode,
			struct sk_buff *skb);
void hci_amp_cmd_status(struct hci_dev *hdev, __u16 opcode, __u8 status);
void hci_amp_event_packet(struct hci_dev *hdev, __u8 ev_code,
			struct sk_buff *skb);

int hci_register_amp(struct amp_mgr_cb *acb);
int hci_unregister_amp(struct amp_mgr_cb *acb);

int hci_send_cmd(struct hci_dev *hdev, __u16 opcode, __u32 plen, void *param);
void hci_send_acl(struct hci_conn *conn, struct hci_chan *chan,
		struct sk_buff *skb, __u16 flags);
<<<<<<< HEAD
=======
struct hci_request {
	struct hci_dev		*hdev;
	struct sk_buff_head	cmd_q;

	/* If something goes wrong when building the HCI request, the error
	 * value is stored in this field.
	 */
	int			err;
};

void hci_req_init(struct hci_request *req, struct hci_dev *hdev);
int hci_req_run(struct hci_request *req, hci_req_complete_t complete);
void hci_req_add(struct hci_request *req, u16 opcode, u32 plen,
		 const void *param);
void hci_req_add_ev(struct hci_request *req, u16 opcode, u32 plen,
		    const void *param, u8 event);
void hci_req_cmd_complete(struct hci_dev *hdev, u16 opcode, u8 status);

struct sk_buff *__hci_cmd_sync(struct hci_dev *hdev, u16 opcode, u32 plen,
			       const void *param, u32 timeout);
struct sk_buff *__hci_cmd_sync_ev(struct hci_dev *hdev, u16 opcode, u32 plen,
				  const void *param, u8 event, u32 timeout);

int hci_send_cmd(struct hci_dev *hdev, __u16 opcode, __u32 plen,
		 const void *param);
void hci_send_acl(struct hci_chan *chan, struct sk_buff *skb, __u16 flags);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
void hci_send_sco(struct hci_conn *conn, struct sk_buff *skb);

void *hci_sent_cmd_data(struct hci_dev *hdev, __u16 opcode);

<<<<<<< HEAD
<<<<<<< HEAD
void hci_si_event(struct hci_dev *hdev, int type, int dlen, void *data);

/* ----- HCI Sockets ----- */
void hci_send_to_sock(struct hci_dev *hdev, struct sk_buff *skb,
							struct sock *skip_sk);

/* Management interface */
int mgmt_control(struct sock *sk, struct msghdr *msg, size_t len);
int mgmt_index_added(u16 index);
int mgmt_index_removed(u16 index);
int mgmt_powered(u16 index, u8 powered);
int mgmt_discoverable(u16 index, u8 discoverable);
int mgmt_connectable(u16 index, u8 connectable);
int mgmt_new_key(u16 index, struct link_key *key, u8 bonded);
int mgmt_connected(u16 index, bdaddr_t *bdaddr, u8 le);
int mgmt_le_conn_params(u16 index, bdaddr_t *bdaddr, u16 interval,
						u16 latency, u16 timeout);
<<<<<<< HEAD
int mgmt_disconnected(u16 index, bdaddr_t *bdaddr, u8 reason);
=======
int mgmt_disconnected(u16 index, bdaddr_t *bdaddr);
>>>>>>> refs/remotes/origin/cm-10.0
int mgmt_disconnect_failed(u16 index);
int mgmt_connect_failed(u16 index, bdaddr_t *bdaddr, u8 status);
int mgmt_pin_code_request(u16 index, bdaddr_t *bdaddr);
int mgmt_pin_code_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status);
int mgmt_pin_code_neg_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status);
int mgmt_user_confirm_request(u16 index, u8 event, bdaddr_t *bdaddr,
							__le32 value);
int mgmt_user_oob_request(u16 index, bdaddr_t *bdaddr);
int mgmt_user_confirm_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status);
int mgmt_user_confirm_neg_reply_complete(u16 index, bdaddr_t *bdaddr,
								u8 status);
int mgmt_auth_failed(u16 index, bdaddr_t *bdaddr, u8 status);
int mgmt_set_local_name_complete(u16 index, u8 *name, u8 status);
int mgmt_read_local_oob_data_reply_complete(u16 index, u8 *hash, u8 *randomizer,
								u8 status);
int mgmt_device_found(u16 index, bdaddr_t *bdaddr, u8 type, u8 le,
				u8 *dev_class, s8 rssi, u8 eir_len, u8 *eir);
void mgmt_read_rssi_complete(u16 index, s8 rssi, bdaddr_t *bdaddr,
				u16 handle, u8 status);
int mgmt_remote_name(u16 index, bdaddr_t *bdaddr, u8 status, u8 *name);
void mgmt_inquiry_started(u16 index);
void mgmt_inquiry_complete_evt(u16 index, u8 status);
void mgmt_disco_timeout(unsigned long data);
void mgmt_disco_le_timeout(unsigned long data);
int mgmt_encrypt_change(u16 index, bdaddr_t *bdaddr, u8 status);

/* LE SMP Management interface */
int le_user_confirm_reply(struct hci_conn *conn, u16 mgmt_op, void *cp);
int mgmt_remote_class(u16 index, bdaddr_t *bdaddr, u8 dev_class[3]);
int mgmt_remote_version(u16 index, bdaddr_t *bdaddr, u8 ver, u16 mnf,
							u16 sub_ver);
int mgmt_remote_features(u16 index, bdaddr_t *bdaddr, u8 features[8]);
=======
/* ----- HCI Sockets ----- */
void hci_send_to_sock(struct hci_dev *hdev, struct sk_buff *skb);
void hci_send_to_control(struct sk_buff *skb, struct sock *skip_sk);
void hci_send_to_monitor(struct hci_dev *hdev, struct sk_buff *skb);
=======
void hci_si_event(struct hci_dev *hdev, int type, int dlen, void *data);
>>>>>>> refs/remotes/origin/cm-11.0

/* ----- HCI Sockets ----- */
void hci_send_to_sock(struct hci_dev *hdev, struct sk_buff *skb,
							struct sock *skip_sk);

/* Management interface */
<<<<<<< HEAD
#define DISCOV_TYPE_BREDR		(BIT(BDADDR_BREDR))
#define DISCOV_TYPE_LE			(BIT(BDADDR_LE_PUBLIC) | \
					 BIT(BDADDR_LE_RANDOM))
#define DISCOV_TYPE_INTERLEAVED		(BIT(BDADDR_BREDR) | \
					 BIT(BDADDR_LE_PUBLIC) | \
					 BIT(BDADDR_LE_RANDOM))

/* These LE scan and inquiry parameters were chosen according to LE General
 * Discovery Procedure specification.
 */
#define DISCOV_LE_SCAN_WIN		0x12
#define DISCOV_LE_SCAN_INT		0x12
#define DISCOV_LE_TIMEOUT		msecs_to_jiffies(10240)
#define DISCOV_INTERLEAVED_TIMEOUT	msecs_to_jiffies(5120)
#define DISCOV_INTERLEAVED_INQUIRY_LEN	0x04
#define DISCOV_BREDR_INQUIRY_LEN	0x08

int mgmt_control(struct sock *sk, struct msghdr *msg, size_t len);
void mgmt_index_added(struct hci_dev *hdev);
void mgmt_index_removed(struct hci_dev *hdev);
void mgmt_set_powered_failed(struct hci_dev *hdev, int err);
int mgmt_powered(struct hci_dev *hdev, u8 powered);
void mgmt_discoverable_timeout(struct hci_dev *hdev);
void mgmt_discoverable(struct hci_dev *hdev, u8 discoverable);
void mgmt_connectable(struct hci_dev *hdev, u8 connectable);
void mgmt_write_scan_failed(struct hci_dev *hdev, u8 scan, u8 status);
void mgmt_new_link_key(struct hci_dev *hdev, struct link_key *key,
		       bool persistent);
void mgmt_device_connected(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 link_type,
			   u8 addr_type, u32 flags, u8 *name, u8 name_len,
			   u8 *dev_class);
void mgmt_device_disconnected(struct hci_dev *hdev, bdaddr_t *bdaddr,
			      u8 link_type, u8 addr_type, u8 reason);
void mgmt_disconnect_failed(struct hci_dev *hdev, bdaddr_t *bdaddr,
			    u8 link_type, u8 addr_type, u8 status);
void mgmt_connect_failed(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 link_type,
			 u8 addr_type, u8 status);
void mgmt_pin_code_request(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 secure);
void mgmt_pin_code_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
				  u8 status);
void mgmt_pin_code_neg_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
				      u8 status);
int mgmt_user_confirm_request(struct hci_dev *hdev, bdaddr_t *bdaddr,
			      u8 link_type, u8 addr_type, __le32 value,
			      u8 confirm_hint);
int mgmt_user_confirm_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
				     u8 link_type, u8 addr_type, u8 status);
int mgmt_user_confirm_neg_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
					 u8 link_type, u8 addr_type, u8 status);
int mgmt_user_passkey_request(struct hci_dev *hdev, bdaddr_t *bdaddr,
			      u8 link_type, u8 addr_type);
int mgmt_user_passkey_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
				     u8 link_type, u8 addr_type, u8 status);
int mgmt_user_passkey_neg_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
					 u8 link_type, u8 addr_type, u8 status);
int mgmt_user_passkey_notify(struct hci_dev *hdev, bdaddr_t *bdaddr,
			     u8 link_type, u8 addr_type, u32 passkey,
			     u8 entered);
void mgmt_auth_failed(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 link_type,
		      u8 addr_type, u8 status);
void mgmt_auth_enable_complete(struct hci_dev *hdev, u8 status);
void mgmt_ssp_enable_complete(struct hci_dev *hdev, u8 enable, u8 status);
void mgmt_set_class_of_dev_complete(struct hci_dev *hdev, u8 *dev_class,
				    u8 status);
void mgmt_set_local_name_complete(struct hci_dev *hdev, u8 *name, u8 status);
void mgmt_read_local_oob_data_reply_complete(struct hci_dev *hdev, u8 *hash,
					     u8 *randomizer, u8 status);
void mgmt_device_found(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 link_type,
		       u8 addr_type, u8 *dev_class, s8 rssi, u8 cfm_name,
		       u8 ssp, u8 *eir, u16 eir_len);
void mgmt_remote_name(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 link_type,
		      u8 addr_type, s8 rssi, u8 *name, u8 name_len);
void mgmt_discovering(struct hci_dev *hdev, u8 discovering);
int mgmt_device_blocked(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 type);
int mgmt_device_unblocked(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 type);
void mgmt_new_ltk(struct hci_dev *hdev, struct smp_ltk *key, u8 persistent);
void mgmt_reenable_advertising(struct hci_dev *hdev);
>>>>>>> refs/remotes/origin/master
=======
int mgmt_control(struct sock *sk, struct msghdr *msg, size_t len);
int mgmt_index_added(u16 index);
int mgmt_index_removed(u16 index);
int mgmt_powered(u16 index, u8 powered);
int mgmt_discoverable(u16 index, u8 discoverable);
int mgmt_connectable(u16 index, u8 connectable);
int mgmt_new_key(u16 index, struct link_key *key, u8 bonded);
int mgmt_connected(u16 index, bdaddr_t *bdaddr, u8 le);
int mgmt_le_conn_params(u16 index, bdaddr_t *bdaddr, u16 interval,
						u16 latency, u16 timeout);
int mgmt_disconnected(u16 index, bdaddr_t *bdaddr, u8 reason);
int mgmt_disconnect_failed(u16 index);
int mgmt_connect_failed(u16 index, bdaddr_t *bdaddr, u8 status);
int mgmt_pin_code_request(u16 index, bdaddr_t *bdaddr);
int mgmt_pin_code_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status);
int mgmt_pin_code_neg_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status);
int mgmt_user_confirm_request(u16 index, u8 event, bdaddr_t *bdaddr,
							__le32 value);
int mgmt_user_oob_request(u16 index, bdaddr_t *bdaddr);
int mgmt_user_confirm_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status);
int mgmt_user_confirm_neg_reply_complete(u16 index, bdaddr_t *bdaddr,
								u8 status);
int mgmt_auth_failed(u16 index, bdaddr_t *bdaddr, u8 status);
int mgmt_set_local_name_complete(u16 index, u8 *name, u8 status);
int mgmt_read_local_oob_data_reply_complete(u16 index, u8 *hash, u8 *randomizer,
								u8 status);
int mgmt_device_found(u16 index, bdaddr_t *bdaddr, u8 type, u8 le,
				u8 *dev_class, s8 rssi, u8 eir_len, u8 *eir);
void mgmt_read_rssi_complete(u16 index, s8 rssi, bdaddr_t *bdaddr,
				u16 handle, u8 status);
int mgmt_remote_name(u16 index, bdaddr_t *bdaddr, u8 status, u8 *name);
void mgmt_inquiry_started(u16 index);
void mgmt_inquiry_complete_evt(u16 index, u8 status);
void mgmt_disco_timeout(unsigned long data);
void mgmt_disco_le_timeout(unsigned long data);
int mgmt_encrypt_change(u16 index, bdaddr_t *bdaddr, u8 status);

/* LE SMP Management interface */
int le_user_confirm_reply(struct hci_conn *conn, u16 mgmt_op, void *cp);
int mgmt_remote_class(u16 index, bdaddr_t *bdaddr, u8 dev_class[3]);
int mgmt_remote_version(u16 index, bdaddr_t *bdaddr, u8 ver, u16 mnf,
							u16 sub_ver);
int mgmt_remote_features(u16 index, bdaddr_t *bdaddr, u8 features[8]);
>>>>>>> refs/remotes/origin/cm-11.0

/* HCI info for socket */
#define hci_pi(sk) ((struct hci_pinfo *) sk)

struct hci_pinfo {
	struct bt_sock    bt;
	struct hci_dev    *hdev;
	struct hci_filter filter;
	__u32             cmsg_mask;
	unsigned short   channel;
};

/* HCI security filter */
#define HCI_SFLT_MAX_OGF  5

struct hci_sec_filter {
	__u32 type_mask;
	__u32 event_mask[2];
	__u32 ocf_mask[HCI_SFLT_MAX_OGF + 1][4];
};

/* ----- HCI requests ----- */
#define HCI_REQ_DONE	  0
#define HCI_REQ_PEND	  1
#define HCI_REQ_CANCELED  2

#define hci_req_lock(d)		mutex_lock(&d->req_lock)
#define hci_req_unlock(d)	mutex_unlock(&d->req_lock)

<<<<<<< HEAD
void hci_req_complete(struct hci_dev *hdev, __u16 cmd, int result);

=======
>>>>>>> refs/remotes/origin/master
void hci_le_conn_update(struct hci_conn *conn, u16 min, u16 max,
					u16 latency, u16 to_multiplier);
void hci_le_start_enc(struct hci_conn *conn, __le16 ediv, __u8 rand[8],
							__u8 ltk[16]);
<<<<<<< HEAD
void hci_le_ltk_reply(struct hci_conn *conn, u8 ltk[16]);
void hci_le_ltk_neg_reply(struct hci_conn *conn);

void hci_read_rssi(struct hci_conn *conn);
<<<<<<< HEAD
=======

#define SCO_AIRMODE_MASK       0x0003
#define SCO_AIRMODE_CVSD       0x0000
#define SCO_AIRMODE_TRANSP     0x0003
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

#endif /* __HCI_CORE_H */
