/****************************************************************************

**

** COPYRIGHT(C) : Samsung Electronics Co.Ltd, 2006-2010 ALL RIGHTS RESERVED

**

****************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/random.h>
#include <linux/if_arp.h>
#include <linux/proc_fs.h>
#include <linux/freezer.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/poll.h>
#include <linux/workqueue.h>
#include <linux/wakelock.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#define	NO_TTY_DPRAM	1
#if (NO_TTY_DPRAM > 0)
#include "dpram.h"
#endif
#define	NO_TTY_RX_BUFF	1
#define	NO_TTY_MUTEX_VNET	1

/* Multiple PDP */
struct pdp_arg {
	unsigned char id;
	char ifname[16];
} __packed;

#define IOC_MZ2_MAGIC		(0xC1)
#define HN_PDP_ACTIVATE		_IOWR(IOC_MZ2_MAGIC, 0xe0, struct pdp_arg)
#define HN_PDP_DEACTIVATE	_IOW(IOC_MZ2_MAGIC, 0xe1, struct pdp_arg)
#define HN_PDP_ADJUST		_IOW(IOC_MZ2_MAGIC, 0xe2, int)
#define HN_PDP_TXSTART		_IO(IOC_MZ2_MAGIC, 0xe3)
#define HN_PDP_TXSTOP		_IO(IOC_MZ2_MAGIC, 0xe4)
#define HN_PDP_CSDSTART		_IO(IOC_MZ2_MAGIC, 0xe5)
#define HN_PDP_CSDSTOP		_IO(IOC_MZ2_MAGIC, 0xe6)

#include <mach/hardware.h>
#include <linux/uaccess.h>

/*
 * Driver macros
 */

#define MULTIPDP_ERROR		/* Define this for error messages */
#undef USE_LOOPBACK_PING	/* Use loopback ping test */

#ifdef USE_LOOPBACK_PING
#include <linux/ip.h>
#include <linux/icmp.h>
#include <net/checksum.h>
#endif

/*	0 : do not print log msg
	1 : error log msg
	2 : low level debug log msg
	3~ : high level debug log msg
*/
#define CONFIG_MULTIPDP_DEBUG 0
#if (CONFIG_MULTIPDP_DEBUG > 0)
#define DPRINTK(N, X...) \
		do { \
			if (N <= CONFIG_MULTIPDP_DEBUG) { \
				printk(KERN_INFO "%s(): ", __func__); \
				printk(KERN_INFO X); \
			} \
		} while (0)
#else
#define DPRINTK(N, X...)	do { } while (0)
#endif

/* Maximum number of PDP context */
#define MAX_PDP_CONTEXT			10

/* Maximum PDP data length */
#define MAX_PDP_DATA_LEN		1550

/* Maximum PDP packet length including header and start/stop bytes */
#define MAX_PDP_PACKET_LEN		(MAX_PDP_DATA_LEN + 4 + 2)

/* Prefix string of virtual network interface */
#define VNET_PREFIX				"pdp"

/* Device node name for application interface */
#define APP_DEVNAME				"multipdp"
#define APP_NAME_MAX_LEN			16

/* DPRAM device node name */
#define DPRAM_DEVNAME			"/dev/dpram1"

/* Device types */
#define DEV_TYPE_NET			0	/* network device for IP data */
#define DEV_TYPE_SERIAL			1	/* serial device for CSD */

/* Device flags */
#define DEV_FLAG_STICKY			0x1	/* Sticky */

/* Device major & minor number */
#define CSD_MAJOR_NUM			235
#define CSD_MINOR_NUM			0

static int pdp_net_activation_count;
static int vnet_start_xmit_flag;

/********** for CPLog Test **************/
static struct sk_buff_head cplog_sk_buf_rx_q;
static struct workqueue_struct *cplog_rx_wq;
static struct work_struct cplog_work;
static void cplog_work_func(struct work_struct *work);
#define APP_LOOPBACK_TEST 1
#ifdef LOOP_BACK_TEST
/*
	Usage
	1. #cd /sys/class/misc/multipdp/
	2. #echo start > loopback
	3. #cat loopback
	4. #echo stop > loopback
 */
#define LOOP_BACK_CHANNEL	31

int loopback_ongoing;
char loopback_data[MAX_PDP_DATA_LEN];
char loopback_value[256];

struct loopback_result {
	int nTransfered;
	int nPacketDataSize;
	struct timeval nStartTime;
	struct timeval nEndTime;
};

static struct loopback_result loopback_res;

static ssize_t show_loopback_value(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	unsigned int nElapsedtime_s, total_size;
	if (!strncmp(loopback_value, "start", 5)) {
		/* show elapsed value */
		do_gettimeofday(&loopback_res.nEndTime);

		nElapsedtime_s =
		    (loopback_res.nEndTime.tv_sec -
		     loopback_res.nStartTime.tv_sec)
		    + (loopback_res.nEndTime.tv_usec -
		       loopback_res.nStartTime.tv_usec) / 1000000;

		total_size =
		    loopback_res.nTransfered * loopback_res.nPacketDataSize;

		/* edit */
		return snprintf(buf, 512,
			       "\n=====	LoopBack Test Result	=====\n\n"
			       "Transfered Items = %d\n"
			       "Packet Data Size = %d\n"
			       "Total transfer size = %d\n"
			       "Elapsed Time = %d (s)\n"
			       "Mean Value = %d (byte/sec)\n"
			       "\n=====================================\n",
			       loopback_res.nTransfered,
			       loopback_res.nPacketDataSize,
			       total_size,
			       nElapsedtime_s, total_size / nElapsedtime_s);
	} else {
		/* edit */
		return snprintf(buf, 32, "loopback test is not on going\n");
	}
}

static struct pdp_info *pdp_get_dev(u8 id);
static int pdp_mux(struct pdp_info *dev, const void *data, size_t len);

static send_loop_back_packet(const char *data, int size)
{
	struct pdp_info *dev = pdp_get_dev(LOOP_BACK_CHANNEL);

	if (loopback_ongoing) {
		pdp_mux(dev, data, size);
		loopback_res.nTransfered++;
	}
}

static ssize_t store_loopback_value(struct device *dev,
				    struct device_attribute *attr, char *buf,
				    size_t count)
{
	int i, data_size = 1500;
	char temp_str[10] = "0123456789";

	if (!strncmp(buf, "start", 5)) {
		sscanf(buf, "%s", loopback_value);

		loopback_res.nTransfered = 0;
		loopback_res.nPacketDataSize = data_size;

		for (i = 0; i < (data_size / 10); i++)
			memcpy((loopback_data + i * 10), temp_str, 10);

		loopback_ongoing = 1;
		do_gettimeofday(&loopback_res.nStartTime);
		send_loop_back_packet(loopback_data, data_size);
	} else if (!strncmp(buf, "stop", 4)) {
		sscanf(buf, "%s", loopback_value);

		loopback_ongoing = 0;
		do_gettimeofday(&loopback_res.nEndTime);
	}
	return strnlen(buf, 256);
}

static DEVICE_ATTR(loopback, S_IRUGO | S_IWUSR, show_loopback_value,
		   store_loopback_value);
#endif

/*
 * Variable types
 */

/* PDP data packet header format */
struct pdp_hdr {
	u16 len;		/* Data length */
	u8 id;			/* Channel ID */
	u8 control;		/* Control field */
} __packed;

/* PDP information type */
struct pdp_info {
	/* PDP context ID */
	u8 id;

	/* Device type */
	unsigned type;

	/* Device flags */
	unsigned flags;

	/* Tx packet buffer */
	u8 *tx_buf;

	/* App device interface */
	union {
		/* Virtual network interface */
		struct {
			struct net_device *net;
			struct net_device_stats stats;
			struct work_struct xmit_task;
		} vnet_u;

		/* Virtual serial interface */
		struct {
			/* CSD, ROUTER, GPS, XGPS, SMD, CPLOG, LOOPBACK */
			struct tty_driver tty_driver[7];
			int refcount;
			struct tty_struct *tty_table[1];
			struct ktermios *termios[1];
			struct ktermios *termios_locked[1];
			char tty_name[16];
			struct tty_struct *tty;
			struct semaphore write_lock;
		} vs_u;
	} dev_u;
#define vn_dev		dev_u.vnet_u
#define vs_dev		dev_u.vs_u
};

/*
 * Global variables
 */

/* PDP information table */
static struct pdp_info *pdp_table[MAX_PDP_CONTEXT];
static DEFINE_SEMAPHORE(pdp_lock);
#ifdef NO_TTY_MUTEX_VNET
static DEFINE_SEMAPHORE(pdp_txlock);
#endif

/* DPRAM-related stuffs */
static struct task_struct *dpram_task;
static struct file *dpram_filp;
static DECLARE_COMPLETION(dpram_complete);

static int g_adjust = 9;
static unsigned long workqueue_data;
#ifndef	NO_TTY_RX_BUFF
static unsigned char pdp_rx_buf[MAX_PDP_DATA_LEN];
#else
#define	MAX_RX_BUFF_LEN		(16*1024)
static unsigned char pdp_rx_buf[MAX_RX_BUFF_LEN];
#endif

static int pdp_tx_flag;
unsigned char *prx_buf;
unsigned int count;
static int pdp_csd_flag;

int fp_vsCSD;
int fp_vsGPS;
int fp_vsEXGPS;
int fp_vsEFS;
int fp_vsSMD;
int fp_vsCPLOG;
int fp_vsLOOPBACK;
/*
 * Function declarations
 */
static int pdp_mux(struct pdp_info *dev, const void *data, size_t len);
static int pdp_demux(void);
static inline struct pdp_info *pdp_get_serdev(const char *name);

static struct tty_driver *get_tty_driver_by_id(struct pdp_info *dev)
{
	int index = 0;

	switch (dev->id) {
	case 1:
		index = 0;
		break;
	case 8:
		index = 1;
		break;
	case 5:
		index = 2;
		break;
	case 6:
		index = 3;
		break;
	case 25:
		index = 4;
		break;
	case 29:
		index = 5;
		break;
	case 31:
		index = 6;
		break;
	default:
		index = 0;
	}

	return &dev->vs_dev.tty_driver[index];
}

static int get_minor_start_index(int id)
{
	int start = 0;

	switch (id) {
	case 1:
		start = 0;
		break;
	case 8:
		start = 1;
		break;
	case 5:
		start = 2;
		break;
	case 6:
		start = 3;
		break;
	case 25:
		start = 4;
		break;
	case 29:
		start = 5;
		break;
	case 31:
		start = 6;
		break;
	default:
		start = 0;
	}

	return start;
}

struct wake_lock pdp_wake_lock;
/* added wake_lock time for RIL to change the waketime
   for implementing Fast Dormancy LCD On/Off Timer */
#define DEFAULT_RAW_WAKE_TIME	(6*HZ)
/* jiffies *//* wake time for not fmt packet */
static long pdp_wake_time;
/* sys fs */
struct class *pdp_class;
EXPORT_SYMBOL(pdp_class);
struct device *pdp_dev;
EXPORT_SYMBOL(pdp_dev);

static ssize_t show_waketime(struct device *d,
			     struct device_attribute *attr, char *buf)
{
	char *p = buf;
	unsigned int msec;
	unsigned long j;
/*
	if (!svnet_dev)
		return 0;

	j = _wake_lock_gettime(svnet_dev);
*/
	j = pdp_wake_time;
	msec = jiffies_to_msecs(j);
	p += snprintf(p, sizeof(msec), "%u\n", msec);

	return p - buf;
}

static ssize_t store_waketime(struct device *d,
			      struct device_attribute *attr, const char *buf,
			      size_t count)
{
	unsigned long msec;
	unsigned long j;
	int r;
/*
	if (!svnet_dev)
		return count;
*/
	r = strict_strtoul(buf, 10, &msec);
	if (r)
		return count;

	j = msecs_to_jiffies(msec);
/*
    _wake_lock_settime(svnet_dev, j);
*/
	pdp_wake_time = j;
	return count;
}

static DEVICE_ATTR(waketime, 0664, show_waketime, store_waketime);

/*
 * DPRAM I/O functions
 */
static inline struct file *dpram_open(void)
{
	int ret;
	struct file *filp;
	struct termios termios;
	mm_segment_t oldfs;

	filp = filp_open(DPRAM_DEVNAME, O_RDWR|O_NONBLOCK, 0);
	if (IS_ERR(filp)) {
		pr_err("[MULTIPDP] filp_open() failed~!: %ld\n", PTR_ERR(filp));
		return NULL;
	}

	oldfs = get_fs();
	set_fs(get_ds());
	ret = filp->f_op->unlocked_ioctl(filp, TCGETA, (unsigned long)&termios);
	set_fs(oldfs);
	if (ret < 0) {
		pr_err("[MULTIPDP] f_op->ioctl() failed: %d\n", ret);
		filp_close(filp, current->files);
		return NULL;
	}

	termios.c_cflag = CS8 | CREAD | HUPCL | CLOCAL | B115200;
	termios.c_iflag = IGNBRK | IGNPAR;
	termios.c_lflag = 0;
	termios.c_oflag = 0;
	termios.c_cc[VMIN] = 1;
	termios.c_cc[VTIME] = 1;

	oldfs = get_fs();
	set_fs(get_ds());
	ret = filp->f_op->unlocked_ioctl(filp, TCSETA, (unsigned long)&termios);
	set_fs(oldfs);
	if (ret < 0) {
		pr_err("[MULTIPDP] f_op->ioctl() failed: %d\n", ret);
		filp_close(filp, current->files);
		return NULL;
	}
	return filp;
}

static inline void dpram_close(struct file *filp)
{
	filp_close(filp, current->files);
}

static inline int dpram_poll(struct file *filp)
{
	int ret;
	unsigned int mask;
	struct poll_wqueues wait_table;
	/*
	poll_table wait_table;
	*/
	mm_segment_t oldfs;

	poll_initwait(&wait_table);
	for (;;) {
		set_current_state(TASK_INTERRUPTIBLE);

		oldfs = get_fs();
		set_fs(get_ds());
		mask = filp->f_op->poll(filp, &wait_table.pt);
		set_fs(oldfs);

		if (mask & POLLIN) {
			/* got data */
			ret = 0;
			break;
		}

		if (wait_table.error) {
			pr_err("[MULTIPDP] error in f_op->poll()\n");
			ret = wait_table.error;
			break;
		}

		if (signal_pending(current)) {
			/* got signal */
			ret = -ERESTARTSYS;
			break;
		}

		schedule();
	}
	set_current_state(TASK_RUNNING);
	poll_freewait(&wait_table);

	return ret;
}

static inline int dpram_write(struct file *filp, const void *buf, size_t count,
			      int nonblock)
{
	int ret, n = 0;
	mm_segment_t oldfs;

	if (pdp_tx_flag) {
		pr_err("[MULTIPDP] Invalid flag\n");
		return -EAGAIN;
	}

	while (count) {
		if (!dpram_filp) {
			pr_err("[MULTIPDP] DPRAM not available\n");
			return -ENODEV;
		}
		dpram_filp->f_flags |= O_NONBLOCK;
		oldfs = get_fs();
		set_fs(get_ds());
		ret = filp->f_op->write(filp, buf + n, count, &filp->f_pos);
		set_fs(oldfs);
		dpram_filp->f_flags &= ~O_NONBLOCK;
		if (ret < 0) {
			if (ret == -EAGAIN && !nonblock)
				continue;

			pr_err("[MULTIPDP] f_op->write() failed: %d\n", ret);
			return ret;
		}
		n += ret;
		count -= ret;
	}
	return n;
}

static inline int dpram_read(struct file *filp, void *buf, size_t count)
{
	int ret, n = 0;
	mm_segment_t oldfs;
	while (count) {
		dpram_filp->f_flags |= O_NONBLOCK;
		oldfs = get_fs();
		set_fs(get_ds());
		ret = filp->f_op->read(filp, buf + n, count, &filp->f_pos);
		set_fs(oldfs);
		dpram_filp->f_flags &= ~O_NONBLOCK;
		if (ret < 0) {
			if (ret == -EAGAIN)
				continue;

			pr_err("[MULTIPDP] f_op->read() failed: %d\n", ret);
			return ret;
		}
		n += ret;
		count -= ret;
	}
	return n;
}

static inline int dpram_flush_rx(struct file *filp, size_t count)
{
	int ret, n = 0;
	char *buf;
	mm_segment_t oldfs;

	buf = vmalloc(count);
	if (buf == NULL) {
		pr_err("[MULTIPDP] Memory alloc failed\n");
		return -ENOMEM;
	}

	while (count) {
		dpram_filp->f_flags |= O_NONBLOCK;
		oldfs = get_fs();
		set_fs(get_ds());
		ret = filp->f_op->read(filp, buf + n, count, &filp->f_pos);
		set_fs(oldfs);
		dpram_filp->f_flags &= ~O_NONBLOCK;
		if (ret < 0) {
			if (ret == -EAGAIN)
				continue;
			pr_err("[MULTIPDP] f_op->read() failed: %d\n", ret);
			vfree(buf);
			return ret;
		}
		n += ret;
		count -= ret;
	}
	vfree(buf);
	return n;
}

#ifdef	NO_TTY_DPRAM
static int multipdp_demux(char *buf, int len);
int multipdp_rx_cback(char *buf, int len)
{
	int i = 0;
	int ret = 0;
	struct pdp_hdr *phdr = NULL;

	DPRINTK(2, "Receive packet size from cp=%d\n", len);

	for (i = 0; i < len; i++) {
		if (buf[i] == 0x7f) {
			phdr = (struct pdp_hdr *)&buf[i + 1];

			if (buf[i + phdr->len + 1] != 0x7E)
				pr_err("[MULTIPDP] NOT 0x7E (Not end byte)\n");

			ret = multipdp_demux((char *)&buf[i + 1], len - i - 1);
			i = i + phdr->len + 1;

			if (ret < 0 || ret == 0)
				break;
		} else
			pr_err("[MULTIPDP] NOT 0x7F (Not start byte)\n");
	}
	return i;
}
#endif

static int dpram_thread(void *data)
{
	int ret = 0;
	int i;
	struct file *filp;
	struct sched_param schedpar;

	dpram_task = current;
	daemonize("dpram_thread");
	strncpy(current->comm,
			APP_DEVNAME,
			APP_NAME_MAX_LEN);

	schedpar.sched_priority = 1;
	sched_setscheduler(current, SCHED_FIFO, &schedpar);

	/* set signals to accept */
	siginitsetinv(&current->blocked, sigmask(SIGUSR1));
	recalc_sigpending();

	for (i = 0; i < 10; i++) {
		filp = dpram_open();
		if (filp == NULL) {
			pr_err("[MULTIPDP] dpram_open failed! retry\n");
			if (i < 2)
				msleep(100);
			else
				msleep(1000);
		} else
			break;
	}
	if (filp == NULL) {
		pr_err("[MULTIPDP] dpram_open failed!\n");
		goto out;
	}

	dpram_filp = filp;

	/* send start signal */
	complete(&dpram_complete);

	while (1) {
		ret = dpram_poll(filp);

		if (ret == -ERESTARTSYS) {
			if (sigismember(&current->pending.signal, SIGUSR1)) {
				sigdelset(&current->pending.signal, SIGUSR1);
				recalc_sigpending();
				ret = 0;
				break;
			}
		}

		else if (ret < 0) {
			pr_err("[MULTIPDP] dpram_poll() failed\n");
			break;
		}

		else {
			char ch;
			ret = dpram_read(dpram_filp, &ch, sizeof(ch));

			if (ret < 0)
				return ret;

			if (ch == 0x7f)
				pdp_demux();
		}
	}

	dpram_close(filp);
	dpram_filp = NULL;

 out:
	dpram_task = NULL;

	/* send finish signal and exit */
	complete_and_exit(&dpram_complete, ret);
}

/*
 * Virtual Network Interface functions
 */

static int vnet_open(struct net_device *net)
{
	struct pdp_info *dev = (struct pdp_info *)net->ml_priv;

	if (pdp_net_activation_count == 0) {
		vnet_start_xmit_flag = 0;
		pr_err("[MULTIPDP] clear xmit_flag, there's no net device\n");
	}
	INIT_WORK(&dev->vn_dev.xmit_task, NULL);
	netif_start_queue(net);

	return 0;
}

static int vnet_stop(struct net_device *net)
{
	struct pdp_info *dev = (struct pdp_info *)net->ml_priv;

	netif_stop_queue(net);
	flush_work(&dev->vn_dev.xmit_task);	/* flush any pending tx tasks */

	return 0;
}

static void vnet_defer_xmit(struct work_struct *data)
{
	struct sk_buff *skb = (struct sk_buff *)workqueue_data;
	struct net_device *net = (struct net_device *)skb->dev;
	struct pdp_info *dev = (struct pdp_info *)net->ml_priv;
	int ret = 0;

	down(&pdp_txlock);
	ret = pdp_mux(dev, skb->data, skb->len);

	if (ret < 0) {
		dev->vn_dev.stats.tx_dropped++;
	} else {
		net->trans_start = jiffies;
		dev->vn_dev.stats.tx_bytes += skb->len;
		dev->vn_dev.stats.tx_packets++;
	}
	dev_kfree_skb_any(skb);
	vnet_start_xmit_flag = 0;

	up(&pdp_txlock);
	if (pdp_tx_flag == 0)
		netif_wake_queue(net);
}

static int vnet_start_xmit(struct sk_buff *skb, struct net_device *net)
{
	struct pdp_info *dev = (struct pdp_info *)net->ml_priv;
#ifdef USE_LOOPBACK_PING
	int ret;
	struct sk_buff *skb2;
	struct icmphdr *icmph;
	struct iphdr *iph;
#endif

#ifdef USE_LOOPBACK_PING
	dev->vn_dev.stats.tx_bytes += skb->len;
	dev->vn_dev.stats.tx_packets++;

	skb2 = alloc_skb(skb->len, GFP_ATOMIC);
	if (skb2 == NULL) {
		pr_err("[MULTIPDP] alloc_skb() failed\n");
		dev_kfree_skb_any(skb);
		return -ENOMEM;
	}

	memcpy(skb2->data, skb->data, skb->len);
	skb_put(skb2, skb->len);
	dev_kfree_skb_any(skb);

	icmph = (struct icmphdr *)(skb2->data + sizeof(struct iphdr));
	iph = (struct iphdr *)skb2->data;

	icmph->type = __constant_htons(ICMP_ECHOREPLY);

	ret = iph->daddr;
	iph->daddr = iph->saddr;
	iph->saddr = ret;
	iph->check = 0;
	iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);

	skb2->dev = net;
	skb2->protocol = __constant_htons(ETH_P_IP);

	netif_rx(skb2);

	dev->vn_dev.stats.rx_packets++;
	dev->vn_dev.stats.rx_bytes += skb->len;
#else
	if (vnet_start_xmit_flag != 0 || pdp_tx_flag == 1)
		return NETDEV_TX_BUSY;

	vnet_start_xmit_flag = 1;
	workqueue_data = (unsigned long)skb;
	PREPARE_WORK(&dev->vn_dev.xmit_task, vnet_defer_xmit);
	schedule_work(&dev->vn_dev.xmit_task);
	netif_stop_queue(net);
#endif

	return 0;
}

#ifdef	NO_TTY_DPRAM
static int multipdp_vnet_recv(struct pdp_info *dev, char *buf, size_t len)
{
	struct sk_buff *skb;
	int ret;

	/* @LDK@ for multiple pdp.. , ex) email & streaming.. by hobac. */
	if (!dev)
		return 0;

	if (dev->vn_dev.net == NULL)
		pr_err("[MULTIPDP] dev->vn_dev.net == NULL!\n");

	if (!netif_running(dev->vn_dev.net)) {
		pr_err("[MULTIPDP] %s(id: %u) is not running\n",
				dev->vn_dev.net->name,
				dev->id);
		return -ENODEV;
	}

	skb = alloc_skb(len, GFP_ATOMIC);

	if (skb == NULL) {
		pr_err("[MULTIPDP] alloc_skb() failed\n");
		return -ENOMEM;
	}

	memcpy(skb->data, buf, len);
	ret = len;

	if (ret < 0) {
		pr_err("[MULTIPDP] dpram_read() failed: %d\n", ret);
		dev_kfree_skb_any(skb);
		return ret;
	}

	skb_put(skb, ret);

	skb->dev = dev->vn_dev.net;
	skb->protocol = __constant_htons(ETH_P_IP);

	netif_rx(skb);

	dev->vn_dev.stats.rx_packets++;
	dev->vn_dev.stats.rx_bytes += skb->len;

	return ret;
}
#endif
static int vnet_recv(struct pdp_info *dev, size_t len)
{
	struct sk_buff *skb;
	int ret;

	/* @LDK@ for multiple pdp.. , ex) email & streaming.. by hobac. */
	if (!dev)
		return 0;

	if (!netif_running(dev->vn_dev.net)) {
		pr_err("[MULTIPDP] %s(id: %u) is not running\n",
				dev->vn_dev.net->name, dev->id);
		return -ENODEV;
	}

	skb = alloc_skb(len, GFP_ATOMIC);

	if (skb == NULL) {
		pr_err("[MULTIPDP] alloc_skb() failed\n");
		return -ENOMEM;
	}
	ret = dpram_read(dpram_filp, skb->data, len);

	if (ret < 0) {
		pr_err("[MULTIPDP] dpram_read() failed: %d\n", ret);
		dev_kfree_skb_any(skb);
		return ret;
	}

	skb_put(skb, ret);

	skb->dev = dev->vn_dev.net;
	skb->protocol = __constant_htons(ETH_P_IP);

	dev->vn_dev.stats.rx_packets++;
	dev->vn_dev.stats.rx_bytes += skb->len;

	netif_rx(skb);

	return 0;
}

static struct net_device_stats *vnet_get_stats(struct net_device *net)
{
	struct pdp_info *dev = (struct pdp_info *)net->ml_priv;
	return &dev->vn_dev.stats;
}

static void vnet_tx_timeout(struct net_device *net)
{
	struct pdp_info *dev = (struct pdp_info *)net->ml_priv;

	net->trans_start = jiffies;
	dev->vn_dev.stats.tx_errors++;
	netif_wake_queue(net);
}

static const struct net_device_ops pdp_netdev_ops = {
	.ndo_open = vnet_open,
	.ndo_stop = vnet_stop,
	.ndo_start_xmit = vnet_start_xmit,
	.ndo_get_stats = vnet_get_stats,
	.ndo_tx_timeout = vnet_tx_timeout,
};

static void vnet_setup(struct net_device *dev)
{
	dev->netdev_ops = &pdp_netdev_ops;
	dev->type = ARPHRD_PPP;
	dev->hard_header_len = 0;
	dev->mtu = MAX_PDP_DATA_LEN;
	dev->addr_len = 0;
	dev->tx_queue_len = 1000;
	dev->flags = IFF_POINTOPOINT | IFF_NOARP | IFF_MULTICAST;
	dev->watchdog_timeo = 40 * HZ;
}

static struct net_device *vnet_add_dev(void *priv)
{
	int ret;
	struct net_device *dev;

	dev = alloc_netdev(0, "pdp%d", vnet_setup);
	if (dev == NULL) {
		pr_err("[MULTIPDP] out of memory\n");
		return NULL;
	}
	dev->ml_priv = priv;
	ret = register_netdev(dev);

	if (ret != 0) {
		pr_err("[MULTIPDP] register_netdevice failed: %d\n",
				ret);
		kfree(dev);
		return NULL;
	}
	return dev;
}

static void vnet_del_dev(struct net_device *net)
{
	DPRINTK(2, "%s network device removed\n",
			net->name);
	unregister_netdev(net);
	kfree(net);
}

/*
 * Virtual Serial Interface functions
 */

static int vs_open(struct tty_struct *tty, struct file *filp)
{
	struct pdp_info *dev;

	DPRINTK(1, "into => tty[%s]\n", tty->name);
	/* 2.6 kernel porting */
	dev = pdp_get_serdev(tty->driver->name);

	if (dev == NULL) {
		pr_err("[MULTIPDP] dev is null\n");
		return -ENODEV;
	}

	switch (dev->id) {
	case 1:
		fp_vsCSD = 1;
		break;

	case 5:
		fp_vsGPS = 1;
		break;

	case 6:
		fp_vsEXGPS = 1;
		break;

	case 8:
		fp_vsEFS = 1;
		break;

	case 25:
		fp_vsSMD = 1;
		break;

	case 29:
		fp_vsCPLOG = 1;
		break;

	case 31:
		fp_vsLOOPBACK = 1;
		break;

	default:
		break;
	}

	tty->driver_data = (void *)dev;
	/* change 1 to 0 */
	tty->low_latency = 0;
	dev->vs_dev.tty = tty;

	return 0;
}

static void vs_close(struct tty_struct *tty, struct file *filp)
{
	struct pdp_info *dev = (struct pdp_info *)tty->driver_data;

	DPRINTK(1, "into => tty[%s]\n", tty->name);

	switch (dev->id) {
	case 1:
		fp_vsCSD = 0;
		break;

	case 5:
		fp_vsGPS = 0;
		break;

	case 6:
		fp_vsEXGPS = 0;
		break;

	case 8:
		fp_vsEFS = 0;
		break;

	case 25:
		fp_vsSMD = 0;
		break;

	case 29:
		fp_vsCPLOG = 0;
		break;

	case 31:
		fp_vsLOOPBACK = 0;
		break;

	default:
		break;
	}

}

static int vs_write(struct tty_struct *tty, const unsigned char *buf, int count)
{
	int ret;
	struct pdp_info *dev;
	/*
    mutex_lock(&pdp_lock);
	*/
	dev = (struct pdp_info *)tty->driver_data;
	ret = pdp_mux(dev, buf, count);

	if (ret == 0)
		ret = count;
	/*
	mutex_unlock(&pdp_lock);
	*/

	return ret;
}

static int vs_write_room(struct tty_struct *tty)
{
	return 8192 * 2;
}

static int vs_chars_in_buffer(struct tty_struct *tty)
{
	return 0;
}

#ifdef	NO_TTY_DPRAM
#define CLUSTER_SEGMENT 1550
static inline int multipdp_tty_insert_data(struct tty_struct *tty,
					   const u8 *psrc, u16 size)
{
	u16 copied_size = 0, real_copied_size = 0;
	int retval = 0;

	if (size > CLUSTER_SEGMENT) {
		while (size) {
			copied_size =
			    (size > CLUSTER_SEGMENT) ? CLUSTER_SEGMENT : size;
			real_copied_size =
			    tty_insert_flip_string(tty, psrc + retval,
						   copied_size);
			size = size - real_copied_size;
			retval += real_copied_size;
		}
		return retval;
	}
	retval = tty_insert_flip_string(tty, psrc, size);
	return retval;
}

static void cplog_work_func(struct work_struct *work)
{
	struct pdp_info *dev = pdp_get_serdev("ttyCPLOG");
	struct sk_buff *skb;
	int ret = 0;
	int reschedule = 0;

	if (fp_vsCPLOG > 0) {
		while ((skb = skb_dequeue(&cplog_sk_buf_rx_q))) {
			ret = multipdp_tty_insert_data(dev->vs_dev.tty,
					skb->data, skb->len);
			if (ret == 0) {
				skb_queue_head(&cplog_sk_buf_rx_q, skb);
				reschedule = 1;
				pr_err("[MULTIPDP] insert_data_size is ZERO!!\n");
				break;
			} else if (ret != skb->len) {
				skb_pull(skb, ret);
				skb_queue_head(&cplog_sk_buf_rx_q, skb);
				tty_flip_buffer_push(dev->vs_dev.tty);
				break;
			} else {
				tty_flip_buffer_push(dev->vs_dev.tty);
				dev_kfree_skb_any(skb);
			}
		}
		if (reschedule == 1)
			queue_work(cplog_rx_wq, &cplog_work);
	} else {
		/* Do Something */
		pr_err("[MULTIPDP] RIL didn't open CPLop node (ch.29)\n");
		return;
	}
}

static int multipdp_vs_read(struct pdp_info *dev, char *buf, size_t len)
{
	int ret = -1;
	struct sk_buff *skb;

	if (!dev) {
		pr_err("[MULTIPDP] dev is null!\n");
		return 0;
	}
#ifndef	NO_TTY_RX_BUFF
	if (len > 1500) {
#else
	if (len > MAX_RX_BUFF_LEN) {
#endif
		unsigned char *prx_buf = kzalloc(len, GFP_ATOMIC);

		if (prx_buf == NULL) {
			pr_err("[MULTIPDP] prx_buf is null!\n");
			return 0;
		}

		memcpy(prx_buf, buf, len);
		ret = len;

		if (ret != len) {
			pr_err("[MULTIPDP] ret != len!\n");
			return ret;
		}

		if (dev->vs_dev.tty == NULL || (dev->id == 1 && !fp_vsCSD)
		    || (dev->id == 5 && !fp_vsGPS) || (dev->id == 8
						       && !fp_vsEFS)
		    || (dev->id == 25 && !fp_vsSMD)
		    || (dev->id == 6 && !fp_vsEXGPS) || (dev->id == 29
							 && !fp_vsCPLOG)
		    || (dev->id == 31 && !fp_vsLOOPBACK)) {
			pr_err("[MULTIPDP] TTY is NULL : (1)~ !!!!\n");
			return 0;
		}

		if (ret > 0 && dev->vs_dev.tty != NULL) {
			if (dev->id == 29) {
				skb = alloc_skb(len, GFP_ATOMIC);
				if (unlikely(!skb))
					pr_err("[MULTIPDP] sk_buff alloc failed!\n");

				memcpy(skb_put(skb, len), prx_buf, len);
				skb_queue_tail(&cplog_sk_buf_rx_q, skb);
				queue_work(cplog_rx_wq, &cplog_work);
			} else {
				ret = multipdp_tty_insert_data(
						dev->vs_dev.tty, prx_buf, ret);
				if (ret > 0) {
					dev->vs_dev.tty->low_latency = 0;
					tty_flip_buffer_push(dev->vs_dev.tty);
				}
			}
		}
		kfree(prx_buf);
	} else {
		/* pdp data length.. */
		memcpy(pdp_rx_buf, buf, len);

		ret = len;
		if (ret != len) {
			pr_err("[MULTIPDP] ret != len!\n");
			return ret;
		}
#ifdef LOOP_BACK_TEST
		if (dev->id == LOOP_BACK_CHANNEL) {
			if (loopback_ongoing) {
				if (strncmp(pdp_rx_buf,
							loopback_data,
							loopback_res.
							nPacketDataSize)) {
					/* DO SOMETHING */
				} else {
					send_loop_back_packet(
							loopback_data,
							loopback_res.
							nPacketDataSize);
				}
			} else {
				/* DO SOMETHING */
			}
		} else if (ret > 0 && dev->vs_dev.tty != NULL) {
			tty_insert_flip_string(
					dev->vs_dev.tty,
					pdp_rx_buf, ret);
			dev->vs_dev.tty->low_latency = 0;
			tty_flip_buffer_push(dev->vs_dev.tty);
		}
#else
		if (dev->vs_dev.tty == NULL)
			pr_err("[MULTIPDP] TTY is NULL : (2)~ !!!!\n");

		if (ret > 0 && dev->vs_dev.tty != NULL) {
			if (dev->id == 29) {
				skb = alloc_skb(len, GFP_ATOMIC);

				if (unlikely(!skb))
					pr_err("[MULTIPDP] Allocation sk_buff error!\n");

				memcpy(skb_put(skb, len), pdp_rx_buf, len);
				skb_queue_tail(&cplog_sk_buf_rx_q, skb);
				queue_work(cplog_rx_wq, &cplog_work);
			} else {
				ret = multipdp_tty_insert_data(
						dev->vs_dev.tty,
						pdp_rx_buf, ret);
				if (ret > 0) {
					dev->vs_dev.tty->low_latency = 0;
					tty_flip_buffer_push(dev->vs_dev.tty);
				}
			}
		}
#endif
	}
	return ret;
}
#endif

static int vs_read(struct pdp_info *dev, size_t len)
{
	int retval = 0;
	u32 size;
	u32 copied_size;
	int insert_size = 0;

	if (dev) {
		/* pdp data length. */

		if (len > MAX_PDP_DATA_LEN) {

			DPRINTK(2, "CAL DATA\n");
			size = dpram_read(dpram_filp, prx_buf, len);
			DPRINTK(2,
				"multipdp_thread request read size : %d readed size %d, count : %d\n",
				len, size, count);

			if ((dev->id == 1 && !fp_vsCSD)
			    || (dev->id == 5 && !fp_vsGPS) || (dev->id == 8
							       && !fp_vsEFS)
			    || (dev->id == 25 && !fp_vsSMD)
			    || (dev->id == 6 && !fp_vsEXGPS) || (dev->id == 29
								 && !fp_vsCPLOG)
			    || (dev->id == 31 && !fp_vsLOOPBACK)) {
				pr_err("[MULTIPDP] %s, discard data.\n",
					dev->vs_dev.tty->name);
			} else {
				while (size) {
					copied_size = (size >
							MAX_PDP_DATA_LEN) ?
						MAX_PDP_DATA_LEN : size;

					if (size > 0
						&& dev->vs_dev.tty != NULL) {
						insert_size =
							tty_insert_flip_string
							(dev->vs_dev.tty,
							 prx_buf + retval,
							 copied_size);
					}

					if (insert_size != copied_size) {
						pr_err("[MULTIPDP] flip buffer full : %s,"
								"insert size : %d,"
								"real size : %d\n",
								dev->vs_dev.
								tty->name,
								copied_size,
								insert_size);
						return -ERANGE;
					}
					size = size - copied_size;
					retval += copied_size;
				}

				DPRINTK(2, "retval : %d\n", retval);
				tty_flip_buffer_push(dev->vs_dev.tty);
				count++;
			}
		} else {
			retval = dpram_read(dpram_filp, pdp_rx_buf, len);

			if (retval != len)
				return retval;

			if (retval > 0) {
				if ((dev->id == 1 && !fp_vsCSD)
				    || (dev->id == 5 && !fp_vsGPS)
				    || (dev->id == 8 && !fp_vsEFS)
				    || (dev->id == 25 && !fp_vsSMD)
				    || (dev->id == 6 && !fp_vsEXGPS)
				    || (dev->id == 29 && !fp_vsCPLOG)
				    || (dev->id == 31 && !fp_vsLOOPBACK)) {
					pr_err("[MULTIPDP] %s, discard data.\n",
						dev->vs_dev.tty->name);
				} else {
					insert_size =
					    tty_insert_flip_string(dev->
								   vs_dev.tty,
								   pdp_rx_buf,
								   retval);

					if (insert_size != retval) {
						pr_err("[MULTIPDP] flip buffer full : %s,"
								"insert size : %d,"
								"real size : %d\n",
								dev->vs_dev.
								tty->name,
								retval,
								insert_size);
						return -ERANGE;
					}

					tty_flip_buffer_push(dev->vs_dev.tty);
				}
			}
		}
	}

	return 0;
}

static int vs_ioctl(struct tty_struct *tty, struct file *file, unsigned int cmd,
		    unsigned long arg)
{
	return -ENOIOCTLCMD;
}

static struct tty_operations multipdp_tty_ops = {
	.open = vs_open,
	.close = vs_close,
	.write = vs_write,
	.write_room = vs_write_room,
	.ioctl = vs_ioctl,
	.chars_in_buffer = vs_chars_in_buffer,
	/* TODO: add more operations */
};

static int vs_add_dev(struct pdp_info *dev)
{
	struct tty_driver *tty_driver;

	tty_driver = get_tty_driver_by_id(dev);

	kref_init(&tty_driver->kref);

	tty_driver->magic = TTY_DRIVER_MAGIC;
	tty_driver->driver_name = "multipdp";
	tty_driver->name = dev->vs_dev.tty_name;
	tty_driver->major = CSD_MAJOR_NUM;
	tty_driver->minor_start = get_minor_start_index(dev->id);
	tty_driver->num = 1;
	tty_driver->type = TTY_DRIVER_TYPE_SERIAL;
	tty_driver->subtype = SERIAL_TYPE_NORMAL;
	tty_driver->flags = TTY_DRIVER_REAL_RAW;
	/*
	tty_driver->refcount = dev->vs_dev.refcount;
	*/
	/* 2.6 kernel porting */
	tty_driver->ttys = dev->vs_dev.tty_table;
	tty_driver->termios = dev->vs_dev.termios;
	tty_driver->termios_locked = dev->vs_dev.termios_locked;

	tty_set_operations(tty_driver, &multipdp_tty_ops);
	return tty_register_driver(tty_driver);
}

static void vs_del_dev(struct pdp_info *dev)
{
	struct tty_driver *tty_driver = NULL;

	tty_driver = get_tty_driver_by_id(dev);
	DPRINTK(2, "%s serial device removed id[%u]\n",
			tty_driver->name, dev->id);
	tty_unregister_driver(tty_driver);
	return;
}

/*
 * PDP context and mux/demux functions
 */

static inline struct pdp_info *pdp_get_dev(u8 id)
{
	int slot;

	for (slot = 0; slot < MAX_PDP_CONTEXT; slot++) {
		if (pdp_table[slot] && pdp_table[slot]->id == id)
			return pdp_table[slot];
	}
	return NULL;
}

static inline struct pdp_info *pdp_get_serdev(const char *name)
{
	int slot;
	struct pdp_info *dev;

	for (slot = 0; slot < MAX_PDP_CONTEXT; slot++) {
		dev = pdp_table[slot];
		if (dev && dev->type == DEV_TYPE_SERIAL &&
				strncmp(name,
					dev->vs_dev.tty_name,
					APP_NAME_MAX_LEN) == 0)
			return dev;
	}
	return NULL;
}

static inline int pdp_add_dev(struct pdp_info *dev)
{
	int slot;

	if (pdp_get_dev(dev->id))
		return -EBUSY;

	for (slot = 0; slot < MAX_PDP_CONTEXT; slot++) {
		if (pdp_table[slot] == NULL) {
			pdp_table[slot] = dev;
			return slot;
		}
	}
	return -ENOSPC;
}

static inline struct pdp_info *pdp_remove_dev(u8 id)
{
	int slot;
	struct pdp_info *dev;

	for (slot = 0; slot < MAX_PDP_CONTEXT; slot++) {
		if (pdp_table[slot] && pdp_table[slot]->id == id) {
			dev = pdp_table[slot];
			pdp_table[slot] = NULL;
			return dev;
		}
	}
	return NULL;
}

static inline struct pdp_info *pdp_remove_slot(int slot)
{
	struct pdp_info *dev;

	dev = pdp_table[slot];
	pdp_table[slot] = NULL;
	return dev;
}

static int pdp_mux(struct pdp_info *dev, const void *data, size_t len)
{
	int ret;
	size_t nbytes;
	u8 *tx_buf;
	struct pdp_hdr *hdr;
	const u8 *buf;

	if (pdp_tx_flag) {
		if (dev->type == DEV_TYPE_NET)
			return -EAGAIN;
	}

	tx_buf = dev->tx_buf;
	hdr = (struct pdp_hdr *)(tx_buf + 1);
	buf = data;

	hdr->id = dev->id;
	hdr->control = 0;

	while (len) {
		if (len > MAX_PDP_DATA_LEN)
			nbytes = MAX_PDP_DATA_LEN;
		else
		    nbytes = len;

		hdr->len = nbytes + sizeof(struct pdp_hdr);
		tx_buf[0] = 0x7f;
		memcpy(tx_buf + 1 + sizeof(struct pdp_hdr), buf, nbytes);
		tx_buf[1 + hdr->len] = 0x7e;
		DPRINTK(2, "hdr->id: %d, hdr->len: %d\n", hdr->id, hdr->len);
		/*
		wake_lock_timeout(&pdp_wake_lock, 12*HZ/2);
		*/
		wake_lock_timeout(&pdp_wake_lock, pdp_wake_time);
#ifdef	NO_TTY_DPRAM
		ret = multipdp_write(tx_buf, hdr->len + 2);
#endif
		if (ret != hdr->len + 2) {
			pr_debug("[MULTIPDP] dpram_write() failed: %d\n", ret);
			return ret;
		}
		buf += nbytes;
		len -= nbytes;
	}

	return 0;
}

#ifdef	NO_TTY_DPRAM
static int multipdp_demux(char *buf, int size)
{
	int ret;
	u8 ch;
	size_t len;
	struct pdp_info *dev = NULL;
	struct pdp_hdr hdr;

	/*
	mutex_lock(&pdp_lock);
	*/
	memcpy((void *)&hdr, (void *)buf, sizeof(struct pdp_hdr));
	len = hdr.len - sizeof(struct pdp_hdr);
	dev = pdp_get_dev(hdr.id);

	if (dev == NULL) {
		pr_err("[MULTIPDP] invalid id: %u, no existing device.\n",
			hdr.id);
		yhexdump((char *)&hdr, sizeof(struct pdp_hdr));

		multipdp_dump();
		ret = -ENODEV;
		goto err;
	}

	if (buf[-1] != 0x7F)
		pr_err("[MULTIPDP] Start byte is not 0x7F\n");

	if (len > size)
		pr_err("[MULTIPDP] len>size : len=%d, size=%d\n", size, len);

	/* read data */
	switch (dev->type) {
	case DEV_TYPE_NET:
		if (len > 1500) {
			pr_err("[MULTIPDP] TYPE_NET len is [%d]\n", len);
			multipdp_dump();
		}
		ret = multipdp_vnet_recv(dev,
				(char *)&buf[sizeof(struct pdp_hdr)],
				len);
		break;
	case DEV_TYPE_SERIAL:
		ret = multipdp_vs_read(dev,
				(char *)&buf[sizeof(struct pdp_hdr)],
				len);
		break;
	default:
		pr_err("[MULTIPDP] Type invalid [%d]\n", dev->type);
		multipdp_dump();
		ret = -1;
	}

	if (ret < 0)
		goto err;

	/* check stop byte */
	ch = buf[hdr.len];

	if (ch != 0x7e) {
		pr_err("[MULTIPDP] End byte is Not 0x7E ... !!!!\n");
		/*
		mutex_unlock(&pdp_lock);
		*/
		return ret;
	}
	/*
	mutex_unlock(&pdp_lock);
	*/
	return ret;

 err:
	/* flush the remaining data including stop byte. */
	/*
	mutex_unlock(&pdp_lock);
	*/
	return ret;
}
#endif

static int pdp_demux(void)
{
	int ret;
	u8 ch;
	size_t len;
	struct pdp_info *dev = NULL;
	struct pdp_hdr hdr;

	/* read header */
	ret = dpram_read(dpram_filp, &hdr, sizeof(hdr));

	if (ret < 0)
		return ret;

	len = hdr.len - sizeof(struct pdp_hdr);

	/* check header */
	down(&pdp_lock);
	dev = pdp_get_dev(hdr.id);

	if (dev == NULL) {
		pr_err("[MULTIPDP] invalid id: %u, no existing device.\n",
			hdr.id);
		ret = -ENODEV;
		goto err;
	}

	/* read data */
	switch (dev->type) {
	case DEV_TYPE_NET:
		ret = vnet_recv(dev, len);
		break;
	case DEV_TYPE_SERIAL:
		ret = vs_read(dev, len);
		break;
	default:
		ret = -1;
	}

	if (ret < 0)
		goto err;

	up(&pdp_lock);

	/* check stop byte */
	ret = dpram_read(dpram_filp, &ch, sizeof(ch));

	if (ret < 0 || ch != 0x7e)
		return ret;

	return 0;
 err:
	up(&pdp_lock);

	/* flush the remaining data including stop byte. */
	dpram_flush_rx(dpram_filp, len + 1);
	return ret;
}

static int pdp_activate(struct pdp_arg *pdp_arg, unsigned type, unsigned flags)
{
	int ret;
	struct pdp_info *dev;
	struct net_device *net;

	dev = vmalloc(sizeof(struct pdp_info) + MAX_PDP_PACKET_LEN);
	if (dev == NULL) {
		pr_err("[MULTIPDP] out of memory\n");
		return -ENOMEM;
	}
	memset(dev, 0, sizeof(struct pdp_info));

	/* @LDK@ added by gykim on 20070203 for adjusting IPC 3.0 spec. */
	if (type == DEV_TYPE_NET)
		dev->id = pdp_arg->id + g_adjust;
	else
	    dev->id = pdp_arg->id;

	/* @LDK@ added by gykim on 20070203 for adjusting IPC 3.0 spec. */
	dev->type = type;
	dev->flags = flags;
	dev->tx_buf = (u8 *) (dev + 1);

	if (type == DEV_TYPE_NET) {
		net = vnet_add_dev((void *)dev);
		if (net == NULL) {
			pr_err("[MULTIPDP] vnet_add_dev is failed");
			vfree(dev);
			return -ENOMEM;
		}

		dev->vn_dev.net = net;

		strncpy(pdp_arg->ifname,
				net->name,
				APP_NAME_MAX_LEN);

		down(&pdp_lock);
		ret = pdp_add_dev(dev);
		if (ret < 0) {
			pr_err("[MULTIPDP] pdp_add_dev() failed\n");
			up(&pdp_lock);
			vnet_del_dev(dev->vn_dev.net);
			vfree(dev);
			return ret;
		}
		pdp_net_activation_count++;
		up(&pdp_lock);

		DPRINTK(2, "%s(id: %u) network device created\n",
			net->name, dev->id);
	} else if (type == DEV_TYPE_SERIAL) {
		init_MUTEX(&dev->vs_dev.write_lock);

		strncpy(dev->vs_dev.tty_name,
				pdp_arg->ifname,
				APP_NAME_MAX_LEN);

		ret = vs_add_dev(dev);
		if (ret < 0) {
			vfree(dev);
			return ret;
		}

		down(&pdp_lock);
		ret = pdp_add_dev(dev);
		if (ret < 0) {
			pr_err("[MULTIPDP] pdp_add_dev() failed\n");
			up(&pdp_lock);
			vs_del_dev(dev);
			vfree(dev);
			return ret;
		}
		up(&pdp_lock);
		DPRINTK(2, "serial device is created id[%u]\n",
				dev->id);
	}

	return 0;
}

static int pdp_deactivate(struct pdp_arg *pdp_arg, int force)
{
	struct pdp_info *dev = NULL;

	down(&pdp_lock);
	pdp_arg->id = pdp_arg->id + g_adjust;
	DPRINTK(2, "ID is adjusted, new ID: %d\n", pdp_arg->id);

	dev = pdp_get_dev(pdp_arg->id);

	if (dev == NULL) {
		pr_err("[MULTIPDP] not found id: %u\n", pdp_arg->id);
		up(&pdp_lock);
		return -EINVAL;
	}
	if (!force && dev->flags & DEV_FLAG_STICKY) {
		pr_err("[MULTIPDP] sticky id: %u\n", pdp_arg->id);
		up(&pdp_lock);
		return -EACCES;
	}

	pdp_remove_dev(pdp_arg->id);
	up(&pdp_lock);

	if (dev->type == DEV_TYPE_NET) {
#ifdef	NO_TTY_MUTEX_VNET
		vnet_stop(dev->vn_dev.net);
		down(&pdp_txlock);
		pdp_net_activation_count--;
#endif
		vnet_del_dev(dev->vn_dev.net);
#ifdef	NO_TTY_MUTEX_VNET
		up(&pdp_txlock);
#endif

	} else if (dev->type == DEV_TYPE_SERIAL) {
		vs_del_dev(dev);
	}

	vfree(dev);
	return 0;
}

static void __exit pdp_cleanup(void)
{
	int slot;
	struct pdp_info *dev;

	down(&pdp_lock);
	for (slot = 0; slot < MAX_PDP_CONTEXT; slot++) {
		dev = pdp_remove_slot(slot);
		if (dev) {
			if (dev->type == DEV_TYPE_NET)
				vnet_del_dev(dev->vn_dev.net);
			else if (dev->type == DEV_TYPE_SERIAL)
				vs_del_dev(dev);
			else
				pr_err("[MULTIPDP] Invalid device type\n");

			vfree(dev);
		}
	}
	up(&pdp_lock);
}

static int pdp_adjust(const int adjust)
{
	g_adjust = adjust;
	DPRINTK(2, "adjusting value: %d\n", adjust);
	return 0;
}

/* App. Interfece Device functions */

/* have to use kernel 2.6.38 or later */
static long multipdp_ioctl(struct file *file, unsigned int cmd,
			   unsigned long arg)
{
	int ret, adjust, slot;
	struct pdp_arg pdp_arg;
	struct pdp_info *dev;

	switch (cmd) {
	case HN_PDP_ACTIVATE:
		if (copy_from_user(&pdp_arg, (void *)arg, sizeof(pdp_arg)))
			return -EFAULT;
		ret = pdp_activate(&pdp_arg, DEV_TYPE_NET, 0);

		if (ret < 0)
			return ret;

		return copy_to_user((void *)arg, &pdp_arg, sizeof(pdp_arg));

	case HN_PDP_DEACTIVATE:
		if (copy_from_user(&pdp_arg, (void *)arg, sizeof(pdp_arg)))
			return -EFAULT;
		return pdp_deactivate(&pdp_arg, 0);

	case HN_PDP_ADJUST:
		if (copy_from_user(&adjust, (void *)arg, sizeof(int)))
			return -EFAULT;
		return pdp_adjust(adjust);

	case HN_PDP_TXSTART:
		for (slot = 0; slot < MAX_PDP_CONTEXT; slot++) {
			dev = pdp_table[slot];
			if (dev != NULL && dev->type == DEV_TYPE_NET) {
				netif_wake_queue(dev->vn_dev.net);
				pr_info("[MULTIPDP] %s %s netif_wake\n",
					__func__, dev->vn_dev.net->name);
			}
		}
		pdp_tx_flag = 0;
		return 0;

	case HN_PDP_TXSTOP:
		for (slot = 0; slot < MAX_PDP_CONTEXT; slot++) {
			dev = pdp_table[slot];
			if (dev != NULL && dev->type == DEV_TYPE_NET) {
				netif_stop_queue(dev->vn_dev.net);
				pr_info("[MULTIPDP] %s %s netif_stop\n",
					__func__, dev->vn_dev.net->name);
			}
		}
		pdp_tx_flag = 1;
		return 0;

	case HN_PDP_CSDSTART:
		pdp_csd_flag = 0;
		return 0;

	case HN_PDP_CSDSTOP:
		pdp_csd_flag = 1;
		return 0;

	}

	return -EINVAL;
}

static struct file_operations multipdp_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = multipdp_ioctl,
	.llseek = no_llseek,
};

static struct miscdevice multipdp_dev = {
	/* MISC_DYNAMIC_MINOR, */
	.minor = 132,
	.name = APP_DEVNAME,
	.fops = &multipdp_fops,
};

/*
 * /proc fs interface
 */

#ifdef CONFIG_PROC_FS
static int multipdp_proc_read(char *page, char **start, off_t off,
			      int count, int *eof, void *data)
{

	char *p = page;
	int len;

	down(&pdp_lock);

	p += snprintf(p, 50, "modified multipdp driver on 20070205");
	for (len = 0; len < MAX_PDP_CONTEXT; len++) {
		struct pdp_info *dev = pdp_table[len];
		if (!dev)
			continue;

		p += snprintf(p, 128,
			"name: %s\t, id: %-3u, type: %-7s, flags: 0x%04x\n",
			dev->type == DEV_TYPE_NET ?
			dev->vn_dev.net->name : dev->vs_dev.tty_name,
			dev->id,
			dev->type == DEV_TYPE_NET ? "network" : "serial",
			dev->flags);
	}
	up(&pdp_lock);

	len = (p - page) - off;
	if (len < 0)
		len = 0;

	*eof = (len <= count) ? 1 : 0;
	*start = page + off;

	return len;
}
#endif

/*
 * Module init/clanup functions
 */
static struct workqueue_struct *multipdp_workq;
static struct work_struct dpram_open_work;


static void dpram_open_work_func(struct work_struct *work)
{
	int ret;
	struct pdp_arg pdp_args[7] = {
		{.id = 1, .ifname = "ttyCSD"},
		{.id = 8, .ifname = "ttyEFS"},
		{.id = 5, .ifname = "ttyGPS"},
		{.id = 6, .ifname = "ttyXTRA"},
		{.id = 25, .ifname = "ttySMD"},
		{.id = 29, .ifname = "ttyCPLOG"},
		{.id = 31, .ifname = "ttyLOOPBACK"},
	};

	msleep(100);

	/* run DPRAM I/O thread */
	ret = kernel_thread(dpram_thread, NULL, CLONE_FS | CLONE_FILES);
	if (ret < 0) {
		pr_err("[MULTIPDP] kernel_thread() failed\n");
		return;
	}
	wait_for_completion(&dpram_complete);
	if (!dpram_task) {
		pr_err("[MULTIPDP] DPRAM I/O thread error\n");
		return;
	}

	/* create serial device for Circuit Switched Data */
	for (ret = 0; ret < 7; ret++) {
		if (pdp_activate(&pdp_args[ret],
					DEV_TYPE_SERIAL,
					DEV_FLAG_STICKY) < 0) {
			pr_err("[MULTIPDP] failed to create a serial device %s\n",
					pdp_args[ret].ifname);
		}
	}
}

static int __init multipdp_init(void)
{
	int ret;

	pdp_net_activation_count = 0;
	vnet_start_xmit_flag = 0;

	wake_lock_init(&pdp_wake_lock, WAKE_LOCK_SUSPEND, "MULTI_PDP");
	pdp_wake_time = DEFAULT_RAW_WAKE_TIME;

	INIT_WORK(&dpram_open_work, dpram_open_work_func);
	multipdp_workq = create_singlethread_workqueue("multipdp_work_queue");

	queue_work(multipdp_workq, &dpram_open_work);

	skb_queue_head_init(&cplog_sk_buf_rx_q);
	INIT_WORK(&cplog_work, cplog_work_func);
	cplog_rx_wq =
		create_singlethread_workqueue("cplog_work_queue");
	if (cplog_rx_wq == NULL) {
		pr_err("[MULTIPDP] create workqueue thread failed\n");
		return -ENOMEM;
	}

	/* create app. interface device */
	ret = misc_register(&multipdp_dev);
	if (ret < 0) {
		pr_err("[MULTIPDP] misc_register() failed\n");
		goto err1;
	}
#ifdef CONFIG_PROC_FS
	create_proc_read_entry(APP_DEVNAME, 0, 0, multipdp_proc_read, NULL);
#endif

#ifdef	NO_TTY_DPRAM
	multipdp_rx_noti_regi(multipdp_rx_cback);
#endif

	/* sys fs - start */
	pdp_class = class_create(THIS_MODULE, "pdp");
	if (IS_ERR(pdp_class))
		pr_err("[MULTIPDP] Failed to create class(pdp)!\n");

	pdp_dev = device_create(pdp_class, NULL, 0, NULL, "pdp");
	if (IS_ERR(pdp_dev))
		pr_err("[MULTIPDP] Failed to create device(pdp)!\n");

	if (device_create_file(pdp_dev, &dev_attr_waketime) < 0)
		pr_err("[MULTIPDP] Failed to create device file(%s)!\n",
			dev_attr_waketime.attr.name);

	return 0;

 err1:
	/* undo serial device for Circuit Switched Data */
	/*
	pdp_deactivate(&pdp_arg, 1);
	*/

	/* kill DPRAM I/O thread */
	if (dpram_task) {
		send_sig(SIGUSR1, dpram_task, 1);
		wait_for_completion(&dpram_complete);
	}
	return ret;
}

static void __exit multipdp_exit(void)
{
	wake_lock_destroy(&pdp_wake_lock);
#ifdef CONFIG_PROC_FS
	remove_proc_entry(APP_DEVNAME, 0);
#endif
	vfree(prx_buf);

	/* remove app. interface device */
	misc_deregister(&multipdp_dev);

	/* clean up PDP context table */
	pdp_cleanup();

	/* remove sys fs */
	class_destroy(pdp_class);

	/* kill DPRAM I/O thread */
	if (dpram_task) {
		send_sig(SIGUSR1, dpram_task, 1);
		wait_for_completion(&dpram_complete);
	}
}

late_initcall(multipdp_init);
module_exit(multipdp_exit);

MODULE_AUTHOR("SAMSUNG ELECTRONICS CO., LTD");
MODULE_DESCRIPTION("Multiple PDP Muxer / Demuxer");
MODULE_LICENSE("GPL");
