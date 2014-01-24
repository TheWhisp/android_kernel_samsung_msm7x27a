/* /proc routines for Host AP driver */

#include <linux/types.h>
#include <linux/proc_fs.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <net/lib80211.h>

#include "hostap_wlan.h"
#include "hostap.h"

#define PROC_LIMIT (PAGE_SIZE - 80)


#ifndef PRISM2_NO_PROCFS_DEBUG
<<<<<<< HEAD
static int prism2_debug_proc_read(char *page, char **start, off_t off,
				  int count, int *eof, void *data)
{
	char *p = page;
	local_info_t *local = (local_info_t *) data;
	int i;

	if (off != 0) {
		*eof = 1;
		return 0;
	}

	p += sprintf(p, "next_txfid=%d next_alloc=%d\n",
		     local->next_txfid, local->next_alloc);
	for (i = 0; i < PRISM2_TXFID_COUNT; i++)
		p += sprintf(p, "FID: tx=%04X intransmit=%04X\n",
			     local->txfid[i], local->intransmitfid[i]);
	p += sprintf(p, "FW TX rate control: %d\n", local->fw_tx_rate_control);
	p += sprintf(p, "beacon_int=%d\n", local->beacon_int);
	p += sprintf(p, "dtim_period=%d\n", local->dtim_period);
	p += sprintf(p, "wds_max_connections=%d\n",
		     local->wds_max_connections);
	p += sprintf(p, "dev_enabled=%d\n", local->dev_enabled);
	p += sprintf(p, "sw_tick_stuck=%d\n", local->sw_tick_stuck);
	for (i = 0; i < WEP_KEYS; i++) {
		if (local->crypt_info.crypt[i] &&
		    local->crypt_info.crypt[i]->ops) {
			p += sprintf(p, "crypt[%d]=%s\n", i,
				     local->crypt_info.crypt[i]->ops->name);
		}
	}
	p += sprintf(p, "pri_only=%d\n", local->pri_only);
	p += sprintf(p, "pci=%d\n", local->func->hw_type == HOSTAP_HW_PCI);
	p += sprintf(p, "sram_type=%d\n", local->sram_type);
	p += sprintf(p, "no_pri=%d\n", local->no_pri);

	return (p - page);
}
#endif /* PRISM2_NO_PROCFS_DEBUG */


static int prism2_stats_proc_read(char *page, char **start, off_t off,
				  int count, int *eof, void *data)
{
	char *p = page;
	local_info_t *local = (local_info_t *) data;
	struct comm_tallies_sums *sums = (struct comm_tallies_sums *)
		&local->comm_tallies;

	if (off != 0) {
		*eof = 1;
		return 0;
	}

	p += sprintf(p, "TxUnicastFrames=%u\n", sums->tx_unicast_frames);
	p += sprintf(p, "TxMulticastframes=%u\n", sums->tx_multicast_frames);
	p += sprintf(p, "TxFragments=%u\n", sums->tx_fragments);
	p += sprintf(p, "TxUnicastOctets=%u\n", sums->tx_unicast_octets);
	p += sprintf(p, "TxMulticastOctets=%u\n", sums->tx_multicast_octets);
	p += sprintf(p, "TxDeferredTransmissions=%u\n",
		     sums->tx_deferred_transmissions);
	p += sprintf(p, "TxSingleRetryFrames=%u\n",
		     sums->tx_single_retry_frames);
	p += sprintf(p, "TxMultipleRetryFrames=%u\n",
		     sums->tx_multiple_retry_frames);
	p += sprintf(p, "TxRetryLimitExceeded=%u\n",
		     sums->tx_retry_limit_exceeded);
	p += sprintf(p, "TxDiscards=%u\n", sums->tx_discards);
	p += sprintf(p, "RxUnicastFrames=%u\n", sums->rx_unicast_frames);
	p += sprintf(p, "RxMulticastFrames=%u\n", sums->rx_multicast_frames);
	p += sprintf(p, "RxFragments=%u\n", sums->rx_fragments);
	p += sprintf(p, "RxUnicastOctets=%u\n", sums->rx_unicast_octets);
	p += sprintf(p, "RxMulticastOctets=%u\n", sums->rx_multicast_octets);
	p += sprintf(p, "RxFCSErrors=%u\n", sums->rx_fcs_errors);
	p += sprintf(p, "RxDiscardsNoBuffer=%u\n",
		     sums->rx_discards_no_buffer);
	p += sprintf(p, "TxDiscardsWrongSA=%u\n", sums->tx_discards_wrong_sa);
	p += sprintf(p, "RxDiscardsWEPUndecryptable=%u\n",
		     sums->rx_discards_wep_undecryptable);
	p += sprintf(p, "RxMessageInMsgFragments=%u\n",
		     sums->rx_message_in_msg_fragments);
	p += sprintf(p, "RxMessageInBadMsgFragments=%u\n",
		     sums->rx_message_in_bad_msg_fragments);
	/* FIX: this may grow too long for one page(?) */

	return (p - page);
}


static int prism2_wds_proc_read(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	char *p = page;
	local_info_t *local = (local_info_t *) data;
	struct list_head *ptr;
	struct hostap_interface *iface;

	if (off > PROC_LIMIT) {
		*eof = 1;
		return 0;
	}

	read_lock_bh(&local->iface_lock);
	list_for_each(ptr, &local->hostap_interfaces) {
		iface = list_entry(ptr, struct hostap_interface, list);
		if (iface->type != HOSTAP_INTERFACE_WDS)
			continue;
		p += sprintf(p, "%s\t%pM\n",
			     iface->dev->name,
			     iface->u.wds.remote_addr);
		if ((p - page) > PROC_LIMIT) {
			printk(KERN_DEBUG "%s: wds proc did not fit\n",
			       local->dev->name);
			break;
		}
	}
	read_unlock_bh(&local->iface_lock);

	if ((p - page) <= off) {
		*eof = 1;
		return 0;
	}

	*start = page + off;

	return (p - page - off);
}


static int prism2_bss_list_proc_read(char *page, char **start, off_t off,
				     int count, int *eof, void *data)
{
	char *p = page;
	local_info_t *local = (local_info_t *) data;
	struct list_head *ptr;
	struct hostap_bss_info *bss;
	int i;

	if (off > PROC_LIMIT) {
		*eof = 1;
		return 0;
	}

	p += sprintf(p, "#BSSID\tlast_update\tcount\tcapab_info\tSSID(txt)\t"
		     "SSID(hex)\tWPA IE\n");
	spin_lock_bh(&local->lock);
	list_for_each(ptr, &local->bss_list) {
		bss = list_entry(ptr, struct hostap_bss_info, list);
		p += sprintf(p, "%pM\t%lu\t%u\t0x%x\t",
			     bss->bssid, bss->last_update,
			     bss->count, bss->capab_info);
		for (i = 0; i < bss->ssid_len; i++) {
			p += sprintf(p, "%c",
				     bss->ssid[i] >= 32 && bss->ssid[i] < 127 ?
				     bss->ssid[i] : '_');
		}
		p += sprintf(p, "\t");
		for (i = 0; i < bss->ssid_len; i++) {
			p += sprintf(p, "%02x", bss->ssid[i]);
		}
		p += sprintf(p, "\t");
		for (i = 0; i < bss->wpa_ie_len; i++) {
			p += sprintf(p, "%02x", bss->wpa_ie[i]);
		}
		p += sprintf(p, "\n");
		if ((p - page) > PROC_LIMIT) {
			printk(KERN_DEBUG "%s: BSS proc did not fit\n",
			       local->dev->name);
			break;
		}
	}
	spin_unlock_bh(&local->lock);

	if ((p - page) <= off) {
		*eof = 1;
		return 0;
	}

	*start = page + off;

	return (p - page - off);
}


static int prism2_crypt_proc_read(char *page, char **start, off_t off,
				  int count, int *eof, void *data)
{
	char *p = page;
	local_info_t *local = (local_info_t *) data;
	int i;

	if (off > PROC_LIMIT) {
		*eof = 1;
		return 0;
	}

	p += sprintf(p, "tx_keyidx=%d\n", local->crypt_info.tx_keyidx);
=======
static int prism2_debug_proc_show(struct seq_file *m, void *v)
{
	local_info_t *local = m->private;
	int i;

	seq_printf(m, "next_txfid=%d next_alloc=%d\n",
		   local->next_txfid, local->next_alloc);
	for (i = 0; i < PRISM2_TXFID_COUNT; i++)
		seq_printf(m, "FID: tx=%04X intransmit=%04X\n",
			   local->txfid[i], local->intransmitfid[i]);
	seq_printf(m, "FW TX rate control: %d\n", local->fw_tx_rate_control);
	seq_printf(m, "beacon_int=%d\n", local->beacon_int);
	seq_printf(m, "dtim_period=%d\n", local->dtim_period);
	seq_printf(m, "wds_max_connections=%d\n", local->wds_max_connections);
	seq_printf(m, "dev_enabled=%d\n", local->dev_enabled);
	seq_printf(m, "sw_tick_stuck=%d\n", local->sw_tick_stuck);
	for (i = 0; i < WEP_KEYS; i++) {
		if (local->crypt_info.crypt[i] &&
		    local->crypt_info.crypt[i]->ops) {
			seq_printf(m, "crypt[%d]=%s\n", i,
				   local->crypt_info.crypt[i]->ops->name);
		}
	}
	seq_printf(m, "pri_only=%d\n", local->pri_only);
	seq_printf(m, "pci=%d\n", local->func->hw_type == HOSTAP_HW_PCI);
	seq_printf(m, "sram_type=%d\n", local->sram_type);
	seq_printf(m, "no_pri=%d\n", local->no_pri);

	return 0;
}

static int prism2_debug_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, prism2_debug_proc_show, PDE_DATA(inode));
}

static const struct file_operations prism2_debug_proc_fops = {
	.open		= prism2_debug_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#endif /* PRISM2_NO_PROCFS_DEBUG */


static int prism2_stats_proc_show(struct seq_file *m, void *v)
{
	local_info_t *local = m->private;
	struct comm_tallies_sums *sums = &local->comm_tallies;

	seq_printf(m, "TxUnicastFrames=%u\n", sums->tx_unicast_frames);
	seq_printf(m, "TxMulticastframes=%u\n", sums->tx_multicast_frames);
	seq_printf(m, "TxFragments=%u\n", sums->tx_fragments);
	seq_printf(m, "TxUnicastOctets=%u\n", sums->tx_unicast_octets);
	seq_printf(m, "TxMulticastOctets=%u\n", sums->tx_multicast_octets);
	seq_printf(m, "TxDeferredTransmissions=%u\n",
		   sums->tx_deferred_transmissions);
	seq_printf(m, "TxSingleRetryFrames=%u\n", sums->tx_single_retry_frames);
	seq_printf(m, "TxMultipleRetryFrames=%u\n",
		   sums->tx_multiple_retry_frames);
	seq_printf(m, "TxRetryLimitExceeded=%u\n",
		   sums->tx_retry_limit_exceeded);
	seq_printf(m, "TxDiscards=%u\n", sums->tx_discards);
	seq_printf(m, "RxUnicastFrames=%u\n", sums->rx_unicast_frames);
	seq_printf(m, "RxMulticastFrames=%u\n", sums->rx_multicast_frames);
	seq_printf(m, "RxFragments=%u\n", sums->rx_fragments);
	seq_printf(m, "RxUnicastOctets=%u\n", sums->rx_unicast_octets);
	seq_printf(m, "RxMulticastOctets=%u\n", sums->rx_multicast_octets);
	seq_printf(m, "RxFCSErrors=%u\n", sums->rx_fcs_errors);
	seq_printf(m, "RxDiscardsNoBuffer=%u\n", sums->rx_discards_no_buffer);
	seq_printf(m, "TxDiscardsWrongSA=%u\n", sums->tx_discards_wrong_sa);
	seq_printf(m, "RxDiscardsWEPUndecryptable=%u\n",
		   sums->rx_discards_wep_undecryptable);
	seq_printf(m, "RxMessageInMsgFragments=%u\n",
		   sums->rx_message_in_msg_fragments);
	seq_printf(m, "RxMessageInBadMsgFragments=%u\n",
		   sums->rx_message_in_bad_msg_fragments);
	/* FIX: this may grow too long for one page(?) */

	return 0;
}

static int prism2_stats_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, prism2_stats_proc_show, PDE_DATA(inode));
}

static const struct file_operations prism2_stats_proc_fops = {
	.open		= prism2_stats_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};


static int prism2_wds_proc_show(struct seq_file *m, void *v)
{
	struct list_head *ptr = v;
	struct hostap_interface *iface;

	iface = list_entry(ptr, struct hostap_interface, list);
	if (iface->type == HOSTAP_INTERFACE_WDS)
		seq_printf(m, "%s\t%pM\n",
			   iface->dev->name, iface->u.wds.remote_addr);
	return 0;
}

static void *prism2_wds_proc_start(struct seq_file *m, loff_t *_pos)
{
	local_info_t *local = m->private;
	read_lock_bh(&local->iface_lock);
	return seq_list_start(&local->hostap_interfaces, *_pos);
}

static void *prism2_wds_proc_next(struct seq_file *m, void *v, loff_t *_pos)
{
	local_info_t *local = m->private;
	return seq_list_next(v, &local->hostap_interfaces, _pos);
}

static void prism2_wds_proc_stop(struct seq_file *m, void *v)
{
	local_info_t *local = m->private;
	read_unlock_bh(&local->iface_lock);
}

static const struct seq_operations prism2_wds_proc_seqops = {
	.start	= prism2_wds_proc_start,
	.next	= prism2_wds_proc_next,
	.stop	= prism2_wds_proc_stop,
	.show	= prism2_wds_proc_show,
};

static int prism2_wds_proc_open(struct inode *inode, struct file *file)
{
	int ret = seq_open(file, &prism2_wds_proc_seqops);
	if (ret == 0) {
		struct seq_file *m = file->private_data;
		m->private = PDE_DATA(inode);
	}
	return ret;
}

static const struct file_operations prism2_wds_proc_fops = {
	.open		= prism2_wds_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};


static int prism2_bss_list_proc_show(struct seq_file *m, void *v)
{
	local_info_t *local = m->private;
	struct list_head *ptr = v;
	struct hostap_bss_info *bss;
	int i;

	if (ptr == &local->bss_list) {
		seq_printf(m, "#BSSID\tlast_update\tcount\tcapab_info\tSSID(txt)\t"
			   "SSID(hex)\tWPA IE\n");
		return 0;
	}

	bss = list_entry(ptr, struct hostap_bss_info, list);
	seq_printf(m, "%pM\t%lu\t%u\t0x%x\t",
		   bss->bssid, bss->last_update,
		   bss->count, bss->capab_info);

	for (i = 0; i < bss->ssid_len; i++)
		seq_putc(m,bss->ssid[i] >= 32 && bss->ssid[i] < 127 ?
			   bss->ssid[i] : '_');

	seq_putc(m, '\t');
	for (i = 0; i < bss->ssid_len; i++)
		seq_printf(m, "%02x", bss->ssid[i]);
	seq_putc(m, '\t');
	for (i = 0; i < bss->wpa_ie_len; i++)
		seq_printf(m, "%02x", bss->wpa_ie[i]);
	seq_putc(m, '\n');
	return 0;
}

static void *prism2_bss_list_proc_start(struct seq_file *m, loff_t *_pos)
{
	local_info_t *local = m->private;
	spin_lock_bh(&local->lock);
	return seq_list_start_head(&local->bss_list, *_pos);
}

static void *prism2_bss_list_proc_next(struct seq_file *m, void *v, loff_t *_pos)
{
	local_info_t *local = m->private;
	return seq_list_next(v, &local->bss_list, _pos);
}

static void prism2_bss_list_proc_stop(struct seq_file *m, void *v)
{
	local_info_t *local = m->private;
	spin_unlock_bh(&local->lock);
}

static const struct seq_operations prism2_bss_list_proc_seqops = {
	.start	= prism2_bss_list_proc_start,
	.next	= prism2_bss_list_proc_next,
	.stop	= prism2_bss_list_proc_stop,
	.show	= prism2_bss_list_proc_show,
};

static int prism2_bss_list_proc_open(struct inode *inode, struct file *file)
{
	int ret = seq_open(file, &prism2_bss_list_proc_seqops);
	if (ret == 0) {
		struct seq_file *m = file->private_data;
		m->private = PDE_DATA(inode);
	}
	return ret;
}

static const struct file_operations prism2_bss_list_proc_fops = {
	.open		= prism2_bss_list_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};


static int prism2_crypt_proc_show(struct seq_file *m, void *v)
{
	local_info_t *local = m->private;
	int i;

	seq_printf(m, "tx_keyidx=%d\n", local->crypt_info.tx_keyidx);
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < WEP_KEYS; i++) {
		if (local->crypt_info.crypt[i] &&
		    local->crypt_info.crypt[i]->ops &&
		    local->crypt_info.crypt[i]->ops->print_stats) {
<<<<<<< HEAD
			p = local->crypt_info.crypt[i]->ops->print_stats(
				p, local->crypt_info.crypt[i]->priv);
		}
	}

	if ((p - page) <= off) {
		*eof = 1;
		return 0;
	}

	*start = page + off;

	return (p - page - off);
}


static int prism2_pda_proc_read(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	local_info_t *local = (local_info_t *) data;

	if (local->pda == NULL || off >= PRISM2_PDA_SIZE) {
		*eof = 1;
		return 0;
	}

	if (off + count > PRISM2_PDA_SIZE)
		count = PRISM2_PDA_SIZE - off;

	memcpy(page, local->pda + off, count);
	return count;
}


static int prism2_aux_dump_proc_read(char *page, char **start, off_t off,
				     int count, int *eof, void *data)
{
	local_info_t *local = (local_info_t *) data;

	if (local->func->read_aux == NULL) {
		*eof = 1;
		return 0;
	}

	if (local->func->read_aux(local->dev, off, count, page)) {
		*eof = 1;
		return 0;
	}
	*start = page;

	return count;
}

=======
			local->crypt_info.crypt[i]->ops->print_stats(
				m, local->crypt_info.crypt[i]->priv);
		}
	}
	return 0;
}

static int prism2_crypt_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, prism2_crypt_proc_show, PDE_DATA(inode));
}

static const struct file_operations prism2_crypt_proc_fops = {
	.open		= prism2_crypt_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};


static ssize_t prism2_pda_proc_read(struct file *file, char __user *buf,
				    size_t count, loff_t *_pos)
{
	local_info_t *local = PDE_DATA(file_inode(file));
	size_t off;

	if (local->pda == NULL || *_pos >= PRISM2_PDA_SIZE)
		return 0;

	off = *_pos;
	if (count > PRISM2_PDA_SIZE - off)
		count = PRISM2_PDA_SIZE - off;
	if (copy_to_user(buf, local->pda + off, count) != 0)
		return -EFAULT;
	*_pos += count;
	return count;
}

static const struct file_operations prism2_pda_proc_fops = {
	.read		= prism2_pda_proc_read,
	.llseek		= generic_file_llseek,
};


static ssize_t prism2_aux_dump_proc_no_read(struct file *file, char __user *buf,
					    size_t bufsize, loff_t *_pos)
{
	return 0;
}

static const struct file_operations prism2_aux_dump_proc_fops = {
	.read		= prism2_aux_dump_proc_no_read,
};

>>>>>>> refs/remotes/origin/master

#ifdef PRISM2_IO_DEBUG
static int prism2_io_debug_proc_read(char *page, char **start, off_t off,
				     int count, int *eof, void *data)
{
	local_info_t *local = (local_info_t *) data;
	int head = local->io_debug_head;
	int start_bytes, left, copy, copied;

	if (off + count > PRISM2_IO_DEBUG_SIZE * 4) {
		*eof = 1;
		if (off >= PRISM2_IO_DEBUG_SIZE * 4)
			return 0;
		count = PRISM2_IO_DEBUG_SIZE * 4 - off;
	}

	copied = 0;
	start_bytes = (PRISM2_IO_DEBUG_SIZE - head) * 4;
	left = count;

	if (off < start_bytes) {
		copy = start_bytes - off;
		if (copy > count)
			copy = count;
		memcpy(page, ((u8 *) &local->io_debug[head]) + off, copy);
		left -= copy;
		if (left > 0)
			memcpy(&page[copy], local->io_debug, left);
	} else {
		memcpy(page, ((u8 *) local->io_debug) + (off - start_bytes),
		       left);
	}

	*start = page;

	return count;
}
#endif /* PRISM2_IO_DEBUG */


#ifndef PRISM2_NO_STATION_MODES
<<<<<<< HEAD
static int prism2_scan_results_proc_read(char *page, char **start, off_t off,
					 int count, int *eof, void *data)
{
	char *p = page;
	local_info_t *local = (local_info_t *) data;
	int entry, i, len, total = 0;
	struct hfa384x_hostscan_result *scanres;
	u8 *pos;

	p += sprintf(p, "CHID ANL SL BcnInt Capab Rate BSSID ATIM SupRates "
		     "SSID\n");

	spin_lock_bh(&local->lock);
	for (entry = 0; entry < local->last_scan_results_count; entry++) {
		scanres = &local->last_scan_results[entry];

		if (total + (p - page) <= off) {
			total += p - page;
			p = page;
		}
		if (total + (p - page) > off + count)
			break;
		if ((p - page) > (PAGE_SIZE - 200))
			break;

		p += sprintf(p, "%d %d %d %d 0x%02x %d %pM %d ",
			     le16_to_cpu(scanres->chid),
			     (s16) le16_to_cpu(scanres->anl),
			     (s16) le16_to_cpu(scanres->sl),
			     le16_to_cpu(scanres->beacon_interval),
			     le16_to_cpu(scanres->capability),
			     le16_to_cpu(scanres->rate),
			     scanres->bssid,
			     le16_to_cpu(scanres->atim));

		pos = scanres->sup_rates;
		for (i = 0; i < sizeof(scanres->sup_rates); i++) {
			if (pos[i] == 0)
				break;
			p += sprintf(p, "<%02x>", pos[i]);
		}
		p += sprintf(p, " ");

		pos = scanres->ssid;
		len = le16_to_cpu(scanres->ssid_len);
		if (len > 32)
			len = 32;
		for (i = 0; i < len; i++) {
			unsigned char c = pos[i];
			if (c >= 32 && c < 127)
				p += sprintf(p, "%c", c);
			else
				p += sprintf(p, "<%02x>", c);
		}
		p += sprintf(p, "\n");
	}
	spin_unlock_bh(&local->lock);

	total += (p - page);
	if (total >= off + count)
		*eof = 1;

	if (total < off) {
		*eof = 1;
		return 0;
	}

	len = total - off;
	if (len > (p - page))
		len = p - page;
	*start = p - len;
	if (len > count)
		len = count;

	return len;
}
=======
static int prism2_scan_results_proc_show(struct seq_file *m, void *v)
{
	local_info_t *local = m->private;
	unsigned long entry;
	int i, len;
	struct hfa384x_hostscan_result *scanres;
	u8 *p;

	if (v == SEQ_START_TOKEN) {
		seq_printf(m,
			   "CHID ANL SL BcnInt Capab Rate BSSID ATIM SupRates SSID\n");
		return 0;
	}

	entry = (unsigned long)v - 2;
	scanres = &local->last_scan_results[entry];

	seq_printf(m, "%d %d %d %d 0x%02x %d %pM %d ",
		   le16_to_cpu(scanres->chid),
		   (s16) le16_to_cpu(scanres->anl),
		   (s16) le16_to_cpu(scanres->sl),
		   le16_to_cpu(scanres->beacon_interval),
		   le16_to_cpu(scanres->capability),
		   le16_to_cpu(scanres->rate),
		   scanres->bssid,
		   le16_to_cpu(scanres->atim));

	p = scanres->sup_rates;
	for (i = 0; i < sizeof(scanres->sup_rates); i++) {
		if (p[i] == 0)
			break;
		seq_printf(m, "<%02x>", p[i]);
	}
	seq_putc(m, ' ');

	p = scanres->ssid;
	len = le16_to_cpu(scanres->ssid_len);
	if (len > 32)
		len = 32;
	for (i = 0; i < len; i++) {
		unsigned char c = p[i];
		if (c >= 32 && c < 127)
			seq_putc(m, c);
		else
			seq_printf(m, "<%02x>", c);
	}
	seq_putc(m, '\n');
	return 0;
}

static void *prism2_scan_results_proc_start(struct seq_file *m, loff_t *_pos)
{
	local_info_t *local = m->private;
	spin_lock_bh(&local->lock);

	/* We have a header (pos 0) + N results to show (pos 1...N) */
	if (*_pos > local->last_scan_results_count)
		return NULL;
	return (void *)(unsigned long)(*_pos + 1); /* 0 would be EOF */
}

static void *prism2_scan_results_proc_next(struct seq_file *m, void *v, loff_t *_pos)
{
	local_info_t *local = m->private;

	++*_pos;
	if (*_pos > local->last_scan_results_count)
		return NULL;
	return (void *)(unsigned long)(*_pos + 1); /* 0 would be EOF */
}

static void prism2_scan_results_proc_stop(struct seq_file *m, void *v)
{
	local_info_t *local = m->private;
	spin_unlock_bh(&local->lock);
}

static const struct seq_operations prism2_scan_results_proc_seqops = {
	.start	= prism2_scan_results_proc_start,
	.next	= prism2_scan_results_proc_next,
	.stop	= prism2_scan_results_proc_stop,
	.show	= prism2_scan_results_proc_show,
};

static int prism2_scan_results_proc_open(struct inode *inode, struct file *file)
{
	int ret = seq_open(file, &prism2_scan_results_proc_seqops);
	if (ret == 0) {
		struct seq_file *m = file->private_data;
		m->private = PDE_DATA(inode);
	}
	return ret;
}

static const struct file_operations prism2_scan_results_proc_fops = {
	.open		= prism2_scan_results_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};


>>>>>>> refs/remotes/origin/master
#endif /* PRISM2_NO_STATION_MODES */


void hostap_init_proc(local_info_t *local)
{
	local->proc = NULL;

	if (hostap_proc == NULL) {
		printk(KERN_WARNING "%s: hostap proc directory not created\n",
		       local->dev->name);
		return;
	}

	local->proc = proc_mkdir(local->ddev->name, hostap_proc);
	if (local->proc == NULL) {
		printk(KERN_INFO "/proc/net/hostap/%s creation failed\n",
		       local->ddev->name);
		return;
	}

#ifndef PRISM2_NO_PROCFS_DEBUG
<<<<<<< HEAD
	create_proc_read_entry("debug", 0, local->proc,
			       prism2_debug_proc_read, local);
#endif /* PRISM2_NO_PROCFS_DEBUG */
	create_proc_read_entry("stats", 0, local->proc,
			       prism2_stats_proc_read, local);
	create_proc_read_entry("wds", 0, local->proc,
			       prism2_wds_proc_read, local);
	create_proc_read_entry("pda", 0, local->proc,
			       prism2_pda_proc_read, local);
	create_proc_read_entry("aux_dump", 0, local->proc,
			       prism2_aux_dump_proc_read, local);
	create_proc_read_entry("bss_list", 0, local->proc,
			       prism2_bss_list_proc_read, local);
	create_proc_read_entry("crypt", 0, local->proc,
			       prism2_crypt_proc_read, local);
#ifdef PRISM2_IO_DEBUG
	create_proc_read_entry("io_debug", 0, local->proc,
			       prism2_io_debug_proc_read, local);
#endif /* PRISM2_IO_DEBUG */
#ifndef PRISM2_NO_STATION_MODES
	create_proc_read_entry("scan_results", 0, local->proc,
			       prism2_scan_results_proc_read, local);
=======
	proc_create_data("debug", 0, local->proc,
			 &prism2_debug_proc_fops, local);
#endif /* PRISM2_NO_PROCFS_DEBUG */
	proc_create_data("stats", 0, local->proc,
			 &prism2_stats_proc_fops, local);
	proc_create_data("wds", 0, local->proc,
			 &prism2_wds_proc_fops, local);
	proc_create_data("pda", 0, local->proc,
			 &prism2_pda_proc_fops, local);
	proc_create_data("aux_dump", 0, local->proc,
			 local->func->read_aux_fops ?: &prism2_aux_dump_proc_fops,
			 local);
	proc_create_data("bss_list", 0, local->proc,
			 &prism2_bss_list_proc_fops, local);
	proc_create_data("crypt", 0, local->proc,
			 &prism2_crypt_proc_fops, local);
#ifdef PRISM2_IO_DEBUG
	proc_create_data("io_debug", 0, local->proc,
			 &prism2_io_debug_proc_fops, local);
#endif /* PRISM2_IO_DEBUG */
#ifndef PRISM2_NO_STATION_MODES
	proc_create_data("scan_results", 0, local->proc,
			 &prism2_scan_results_proc_fops, local);
>>>>>>> refs/remotes/origin/master
#endif /* PRISM2_NO_STATION_MODES */
}


void hostap_remove_proc(local_info_t *local)
{
<<<<<<< HEAD
	if (local->proc != NULL) {
#ifndef PRISM2_NO_STATION_MODES
		remove_proc_entry("scan_results", local->proc);
#endif /* PRISM2_NO_STATION_MODES */
#ifdef PRISM2_IO_DEBUG
		remove_proc_entry("io_debug", local->proc);
#endif /* PRISM2_IO_DEBUG */
		remove_proc_entry("pda", local->proc);
		remove_proc_entry("aux_dump", local->proc);
		remove_proc_entry("wds", local->proc);
		remove_proc_entry("stats", local->proc);
		remove_proc_entry("bss_list", local->proc);
		remove_proc_entry("crypt", local->proc);
#ifndef PRISM2_NO_PROCFS_DEBUG
		remove_proc_entry("debug", local->proc);
#endif /* PRISM2_NO_PROCFS_DEBUG */
		if (hostap_proc != NULL)
			remove_proc_entry(local->proc->name, hostap_proc);
	}
=======
	remove_proc_subtree(local->ddev->name, hostap_proc);
>>>>>>> refs/remotes/origin/master
}


EXPORT_SYMBOL(hostap_init_proc);
EXPORT_SYMBOL(hostap_remove_proc);
