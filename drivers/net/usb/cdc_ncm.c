/*
 * cdc_ncm.c
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (C) ST-Ericsson 2010-2011
=======
 * Copyright (C) ST-Ericsson 2010-2012
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (C) ST-Ericsson 2010-2012
>>>>>>> refs/remotes/origin/master
 * Contact: Alexey Orishko <alexey.orishko@stericsson.com>
 * Original author: Hans Petter Selasky <hans.petter.selasky@stericsson.com>
 *
 * USB Host Driver for Network Control Model (NCM)
 * http://www.usb.org/developers/devclass_docs/NCM10.zip
 *
 * The NCM encoding, decoding and initialization logic
 * derives from FreeBSD 8.x. if_cdce.c and if_cdcereg.h
 *
 * This software is available to you under a choice of one of two
 * licenses. You may choose this file to be licensed under the terms
 * of the GNU General Public License (GPL) Version 2 or the 2-clause
 * BSD license listed below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/ctype.h>
#include <linux/ethtool.h>
#include <linux/workqueue.h>
#include <linux/mii.h>
#include <linux/crc32.h>
#include <linux/usb.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/version.h>
#include <linux/timer.h>
#include <linux/spinlock.h>
=======
#include <linux/hrtimer.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/atomic.h>
#include <linux/usb/usbnet.h>
#include <linux/usb/cdc.h>

<<<<<<< HEAD
#define	DRIVER_VERSION				"04-Aug-2011"
=======
#define	DRIVER_VERSION				"14-Mar-2012"
>>>>>>> refs/remotes/origin/cm-10.0

/* CDC NCM subclass 3.2.1 */
#define USB_CDC_NCM_NDP16_LENGTH_MIN		0x10

/* Maximum NTB length */
<<<<<<< HEAD
#define	CDC_NCM_NTB_MAX_SIZE_TX			16384	/* bytes */
#define	CDC_NCM_NTB_MAX_SIZE_RX			16384	/* bytes */
=======
#define	CDC_NCM_NTB_MAX_SIZE_TX			32768	/* bytes */
#define	CDC_NCM_NTB_MAX_SIZE_RX			32768	/* bytes */
>>>>>>> refs/remotes/origin/cm-10.0

/* Minimum value for MaxDatagramSize, ch. 6.2.9 */
#define	CDC_NCM_MIN_DATAGRAM_SIZE		1514	/* bytes */

#define	CDC_NCM_MIN_TX_PKT			512	/* bytes */

/* Default value for MaxDatagramSize */
<<<<<<< HEAD
#define	CDC_NCM_MAX_DATAGRAM_SIZE		2048	/* bytes */

/*
 * Maximum amount of datagrams in NCM Datagram Pointer Table, not counting
 * the last NULL entry. Any additional datagrams in NTB would be discarded.
 */
#define	CDC_NCM_DPT_DATAGRAMS_MAX		32

/* Maximum amount of IN datagrams in NTB */
#define	CDC_NCM_DPT_DATAGRAMS_IN_MAX		0 /* unlimited */

/* Restart the timer, if amount of datagrams is less than given value */
#define	CDC_NCM_RESTART_TIMER_DATAGRAM_CNT	3
=======
#define	CDC_NCM_MAX_DATAGRAM_SIZE		8192	/* bytes */

/*
 * Maximum amount of datagrams in NCM Datagram Pointer Table, not counting
 * the last NULL entry.
 */
#define	CDC_NCM_DPT_DATAGRAMS_MAX		40

/* Restart the timer, if amount of datagrams is less than given value */
#define	CDC_NCM_RESTART_TIMER_DATAGRAM_CNT	3
#define	CDC_NCM_TIMER_PENDING_CNT		2
#define CDC_NCM_TIMER_INTERVAL			(400UL * NSEC_PER_USEC)
>>>>>>> refs/remotes/origin/cm-10.0

/* The following macro defines the minimum header space */
#define	CDC_NCM_MIN_HDR_SIZE \
	(sizeof(struct usb_cdc_ncm_nth16) + sizeof(struct usb_cdc_ncm_ndp16) + \
	(CDC_NCM_DPT_DATAGRAMS_MAX + 1) * sizeof(struct usb_cdc_ncm_dpe16))

struct cdc_ncm_data {
	struct usb_cdc_ncm_nth16 nth16;
	struct usb_cdc_ncm_ndp16 ndp16;
	struct usb_cdc_ncm_dpe16 dpe16[CDC_NCM_DPT_DATAGRAMS_MAX + 1];
};

struct cdc_ncm_ctx {
<<<<<<< HEAD
	struct cdc_ncm_data rx_ncm;
	struct cdc_ncm_data tx_ncm;
	struct usb_cdc_ncm_ntb_parameters ncm_parm;
	struct timer_list tx_timer;
=======
	struct cdc_ncm_data tx_ncm;
	struct usb_cdc_ncm_ntb_parameters ncm_parm;
	struct hrtimer tx_timer;
	struct tasklet_struct bh;
>>>>>>> refs/remotes/origin/cm-10.0

	const struct usb_cdc_ncm_desc *func_desc;
	const struct usb_cdc_header_desc *header_desc;
	const struct usb_cdc_union_desc *union_desc;
	const struct usb_cdc_ether_desc *ether_desc;

	struct net_device *netdev;
	struct usb_device *udev;
	struct usb_host_endpoint *in_ep;
	struct usb_host_endpoint *out_ep;
	struct usb_host_endpoint *status_ep;
	struct usb_interface *intf;
	struct usb_interface *control;
	struct usb_interface *data;

	struct sk_buff *tx_curr_skb;
	struct sk_buff *tx_rem_skb;

	spinlock_t mtx;
<<<<<<< HEAD
=======
	atomic_t stop;
>>>>>>> refs/remotes/origin/cm-10.0

	u32 tx_timer_pending;
	u32 tx_curr_offset;
	u32 tx_curr_last_offset;
	u32 tx_curr_frame_num;
	u32 rx_speed;
	u32 tx_speed;
	u32 rx_max;
	u32 tx_max;
	u32 max_datagram_size;
	u16 tx_max_datagrams;
	u16 tx_remainder;
	u16 tx_modulus;
	u16 tx_ndp_modulus;
	u16 tx_seq;
<<<<<<< HEAD
	u16 connected;
};

static void cdc_ncm_tx_timeout(unsigned long arg);
static const struct driver_info cdc_ncm_info;
static struct usb_driver cdc_ncm_driver;
static struct ethtool_ops cdc_ncm_ethtool_ops;
=======
	u16 rx_seq;
	u16 connected;
};
=======
#include <linux/hrtimer.h>
#include <linux/atomic.h>
#include <linux/usb/usbnet.h>
#include <linux/usb/cdc.h>
#include <linux/usb/cdc_ncm.h>

#if IS_ENABLED(CONFIG_USB_NET_CDC_MBIM)
static bool prefer_mbim = true;
#else
static bool prefer_mbim;
#endif
module_param(prefer_mbim, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(prefer_mbim, "Prefer MBIM setting on dual NCM/MBIM functions");
>>>>>>> refs/remotes/origin/master

static void cdc_ncm_txpath_bh(unsigned long param);
static void cdc_ncm_tx_timeout_start(struct cdc_ncm_ctx *ctx);
static enum hrtimer_restart cdc_ncm_tx_timer_cb(struct hrtimer *hr_timer);
<<<<<<< HEAD
static const struct driver_info cdc_ncm_info;
static struct usb_driver cdc_ncm_driver;
static const struct ethtool_ops cdc_ncm_ethtool_ops;
>>>>>>> refs/remotes/origin/cm-10.0

static const struct usb_device_id cdc_devs[] = {
	{ USB_INTERFACE_INFO(USB_CLASS_COMM,
		USB_CDC_SUBCLASS_NCM, USB_CDC_PROTO_NONE),
		.driver_info = (unsigned long)&cdc_ncm_info,
	},
	{
	},
};

MODULE_DEVICE_TABLE(usb, cdc_devs);

static void
cdc_ncm_get_drvinfo(struct net_device *net, struct ethtool_drvinfo *info)
{
	struct usbnet *dev = netdev_priv(net);

	strncpy(info->driver, dev->driver_name, sizeof(info->driver));
	strncpy(info->version, DRIVER_VERSION, sizeof(info->version));
	strncpy(info->fw_version, dev->driver_info->description,
		sizeof(info->fw_version));
	usb_make_path(dev->udev, info->bus_info, sizeof(info->bus_info));
}

static u8 cdc_ncm_setup(struct cdc_ncm_ctx *ctx)
{
=======
static struct usb_driver cdc_ncm_driver;

static int cdc_ncm_setup(struct usbnet *dev)
{
	struct cdc_ncm_ctx *ctx = (struct cdc_ncm_ctx *)dev->data[0];
	struct usb_cdc_ncm_ntb_parameters ncm_parm;
>>>>>>> refs/remotes/origin/master
	u32 val;
	u8 flags;
	u8 iface_no;
	int err;
<<<<<<< HEAD
	u16 ntb_fmt_supported;

	iface_no = ctx->control->cur_altsetting->desc.bInterfaceNumber;

	err = usb_control_msg(ctx->udev,
				usb_rcvctrlpipe(ctx->udev, 0),
				USB_CDC_GET_NTB_PARAMETERS,
				USB_TYPE_CLASS | USB_DIR_IN
				 | USB_RECIP_INTERFACE,
				0, iface_no, &ctx->ncm_parm,
				sizeof(ctx->ncm_parm), 10000);
	if (err < 0) {
		pr_debug("failed GET_NTB_PARAMETERS\n");
		return 1;
	}

	/* read correct set of parameters according to device mode */
	ctx->rx_max = le32_to_cpu(ctx->ncm_parm.dwNtbInMaxSize);
	ctx->tx_max = le32_to_cpu(ctx->ncm_parm.dwNtbOutMaxSize);
	ctx->tx_remainder = le16_to_cpu(ctx->ncm_parm.wNdpOutPayloadRemainder);
	ctx->tx_modulus = le16_to_cpu(ctx->ncm_parm.wNdpOutDivisor);
	ctx->tx_ndp_modulus = le16_to_cpu(ctx->ncm_parm.wNdpOutAlignment);
	/* devices prior to NCM Errata shall set this field to zero */
	ctx->tx_max_datagrams = le16_to_cpu(ctx->ncm_parm.wNtbOutMaxDatagrams);
	ntb_fmt_supported = le16_to_cpu(ctx->ncm_parm.bmNtbFormatsSupported);

	if (ctx->func_desc != NULL)
		flags = ctx->func_desc->bmNetworkCapabilities;
	else
		flags = 0;

	pr_debug("dwNtbInMaxSize=%u dwNtbOutMaxSize=%u "
		 "wNdpOutPayloadRemainder=%u wNdpOutDivisor=%u "
		 "wNdpOutAlignment=%u wNtbOutMaxDatagrams=%u flags=0x%x\n",
		 ctx->rx_max, ctx->tx_max, ctx->tx_remainder, ctx->tx_modulus,
		 ctx->tx_ndp_modulus, ctx->tx_max_datagrams, flags);
=======
	int eth_hlen;
	u16 ntb_fmt_supported;
	__le16 max_datagram_size;

	iface_no = ctx->control->cur_altsetting->desc.bInterfaceNumber;

	err = usbnet_read_cmd(dev, USB_CDC_GET_NTB_PARAMETERS,
			      USB_TYPE_CLASS | USB_DIR_IN
			      |USB_RECIP_INTERFACE,
			      0, iface_no, &ncm_parm,
			      sizeof(ncm_parm));
	if (err < 0) {
		dev_err(&dev->intf->dev, "failed GET_NTB_PARAMETERS\n");
		return err; /* GET_NTB_PARAMETERS is required */
	}

	/* read correct set of parameters according to device mode */
	ctx->rx_max = le32_to_cpu(ncm_parm.dwNtbInMaxSize);
	ctx->tx_max = le32_to_cpu(ncm_parm.dwNtbOutMaxSize);
	ctx->tx_remainder = le16_to_cpu(ncm_parm.wNdpOutPayloadRemainder);
	ctx->tx_modulus = le16_to_cpu(ncm_parm.wNdpOutDivisor);
	ctx->tx_ndp_modulus = le16_to_cpu(ncm_parm.wNdpOutAlignment);
	/* devices prior to NCM Errata shall set this field to zero */
	ctx->tx_max_datagrams = le16_to_cpu(ncm_parm.wNtbOutMaxDatagrams);
	ntb_fmt_supported = le16_to_cpu(ncm_parm.bmNtbFormatsSupported);

	/* there are some minor differences in NCM and MBIM defaults */
	if (cdc_ncm_comm_intf_is_mbim(ctx->control->cur_altsetting)) {
		if (!ctx->mbim_desc)
			return -EINVAL;
		eth_hlen = 0;
		flags = ctx->mbim_desc->bmNetworkCapabilities;
		ctx->max_datagram_size = le16_to_cpu(ctx->mbim_desc->wMaxSegmentSize);
		if (ctx->max_datagram_size < CDC_MBIM_MIN_DATAGRAM_SIZE)
			ctx->max_datagram_size = CDC_MBIM_MIN_DATAGRAM_SIZE;
	} else {
		if (!ctx->func_desc)
			return -EINVAL;
		eth_hlen = ETH_HLEN;
		flags = ctx->func_desc->bmNetworkCapabilities;
		ctx->max_datagram_size = le16_to_cpu(ctx->ether_desc->wMaxSegmentSize);
		if (ctx->max_datagram_size < CDC_NCM_MIN_DATAGRAM_SIZE)
			ctx->max_datagram_size = CDC_NCM_MIN_DATAGRAM_SIZE;
	}

	/* common absolute max for NCM and MBIM */
	if (ctx->max_datagram_size > CDC_NCM_MAX_DATAGRAM_SIZE)
		ctx->max_datagram_size = CDC_NCM_MAX_DATAGRAM_SIZE;

	dev_dbg(&dev->intf->dev,
		"dwNtbInMaxSize=%u dwNtbOutMaxSize=%u wNdpOutPayloadRemainder=%u wNdpOutDivisor=%u wNdpOutAlignment=%u wNtbOutMaxDatagrams=%u flags=0x%x\n",
		ctx->rx_max, ctx->tx_max, ctx->tx_remainder, ctx->tx_modulus,
		ctx->tx_ndp_modulus, ctx->tx_max_datagrams, flags);
>>>>>>> refs/remotes/origin/master

	/* max count of tx datagrams */
	if ((ctx->tx_max_datagrams == 0) ||
			(ctx->tx_max_datagrams > CDC_NCM_DPT_DATAGRAMS_MAX))
		ctx->tx_max_datagrams = CDC_NCM_DPT_DATAGRAMS_MAX;

	/* verify maximum size of received NTB in bytes */
	if (ctx->rx_max < USB_CDC_NCM_NTB_MIN_IN_SIZE) {
<<<<<<< HEAD
		pr_debug("Using min receive length=%d\n",
						USB_CDC_NCM_NTB_MIN_IN_SIZE);
=======
		dev_dbg(&dev->intf->dev, "Using min receive length=%d\n",
			USB_CDC_NCM_NTB_MIN_IN_SIZE);
>>>>>>> refs/remotes/origin/master
		ctx->rx_max = USB_CDC_NCM_NTB_MIN_IN_SIZE;
	}

	if (ctx->rx_max > CDC_NCM_NTB_MAX_SIZE_RX) {
<<<<<<< HEAD
		pr_debug("Using default maximum receive length=%d\n",
						CDC_NCM_NTB_MAX_SIZE_RX);
=======
		dev_dbg(&dev->intf->dev, "Using default maximum receive length=%d\n",
			CDC_NCM_NTB_MAX_SIZE_RX);
>>>>>>> refs/remotes/origin/master
		ctx->rx_max = CDC_NCM_NTB_MAX_SIZE_RX;
	}

	/* inform device about NTB input size changes */
<<<<<<< HEAD
	if (ctx->rx_max != le32_to_cpu(ctx->ncm_parm.dwNtbInMaxSize)) {

		if (flags & USB_CDC_NCM_NCAP_NTB_INPUT_SIZE) {
			struct usb_cdc_ncm_ndp_input_size *ndp_in_sz;

			ndp_in_sz = kzalloc(sizeof(*ndp_in_sz), GFP_KERNEL);
			if (!ndp_in_sz) {
				err = -ENOMEM;
				goto size_err;
			}

			err = usb_control_msg(ctx->udev,
					usb_sndctrlpipe(ctx->udev, 0),
					USB_CDC_SET_NTB_INPUT_SIZE,
					USB_TYPE_CLASS | USB_DIR_OUT
					 | USB_RECIP_INTERFACE,
					0, iface_no, ndp_in_sz, 8, 1000);
			kfree(ndp_in_sz);
		} else {
			__le32 *dwNtbInMaxSize;
			dwNtbInMaxSize = kzalloc(sizeof(*dwNtbInMaxSize),
					GFP_KERNEL);
			if (!dwNtbInMaxSize) {
				err = -ENOMEM;
				goto size_err;
			}
			*dwNtbInMaxSize = cpu_to_le32(ctx->rx_max);

			err = usb_control_msg(ctx->udev,
					usb_sndctrlpipe(ctx->udev, 0),
					USB_CDC_SET_NTB_INPUT_SIZE,
					USB_TYPE_CLASS | USB_DIR_OUT
					 | USB_RECIP_INTERFACE,
					0, iface_no, dwNtbInMaxSize, 4, 1000);
			kfree(dwNtbInMaxSize);
		}
size_err:
		if (err < 0)
			pr_debug("Setting NTB Input Size failed\n");
	}

	/* verify maximum size of transmitted NTB in bytes */
	if ((ctx->tx_max <
	    (CDC_NCM_MIN_HDR_SIZE + CDC_NCM_MIN_DATAGRAM_SIZE)) ||
	    (ctx->tx_max > CDC_NCM_NTB_MAX_SIZE_TX)) {
		pr_debug("Using default maximum transmit length=%d\n",
						CDC_NCM_NTB_MAX_SIZE_TX);
		ctx->tx_max = CDC_NCM_NTB_MAX_SIZE_TX;
=======
	if (ctx->rx_max != le32_to_cpu(ncm_parm.dwNtbInMaxSize)) {
		__le32 dwNtbInMaxSize = cpu_to_le32(ctx->rx_max);

		err = usbnet_write_cmd(dev, USB_CDC_SET_NTB_INPUT_SIZE,
				       USB_TYPE_CLASS | USB_DIR_OUT
				       | USB_RECIP_INTERFACE,
				       0, iface_no, &dwNtbInMaxSize, 4);
		if (err < 0)
			dev_dbg(&dev->intf->dev, "Setting NTB Input Size failed\n");
	}

	/* verify maximum size of transmitted NTB in bytes */
	if (ctx->tx_max > CDC_NCM_NTB_MAX_SIZE_TX) {
		dev_dbg(&dev->intf->dev, "Using default maximum transmit length=%d\n",
			CDC_NCM_NTB_MAX_SIZE_TX);
		ctx->tx_max = CDC_NCM_NTB_MAX_SIZE_TX;

		/* Adding a pad byte here simplifies the handling in
		 * cdc_ncm_fill_tx_frame, by making tx_max always
		 * represent the real skb max size.
		 */
		if (ctx->tx_max % usb_maxpacket(dev->udev, dev->out, 1) == 0)
			ctx->tx_max++;

>>>>>>> refs/remotes/origin/master
	}

	/*
	 * verify that the structure alignment is:
	 * - power of two
	 * - not greater than the maximum transmit length
	 * - not less than four bytes
	 */
	val = ctx->tx_ndp_modulus;

	if ((val < USB_CDC_NCM_NDP_ALIGN_MIN_SIZE) ||
	    (val != ((-val) & val)) || (val >= ctx->tx_max)) {
<<<<<<< HEAD
		pr_debug("Using default alignment: 4 bytes\n");
=======
		dev_dbg(&dev->intf->dev, "Using default alignment: 4 bytes\n");
>>>>>>> refs/remotes/origin/master
		ctx->tx_ndp_modulus = USB_CDC_NCM_NDP_ALIGN_MIN_SIZE;
	}

	/*
	 * verify that the payload alignment is:
	 * - power of two
	 * - not greater than the maximum transmit length
	 * - not less than four bytes
	 */
	val = ctx->tx_modulus;

	if ((val < USB_CDC_NCM_NDP_ALIGN_MIN_SIZE) ||
	    (val != ((-val) & val)) || (val >= ctx->tx_max)) {
<<<<<<< HEAD
		pr_debug("Using default transmit modulus: 4 bytes\n");
=======
		dev_dbg(&dev->intf->dev, "Using default transmit modulus: 4 bytes\n");
>>>>>>> refs/remotes/origin/master
		ctx->tx_modulus = USB_CDC_NCM_NDP_ALIGN_MIN_SIZE;
	}

	/* verify the payload remainder */
	if (ctx->tx_remainder >= ctx->tx_modulus) {
<<<<<<< HEAD
		pr_debug("Using default transmit remainder: 0 bytes\n");
=======
		dev_dbg(&dev->intf->dev, "Using default transmit remainder: 0 bytes\n");
>>>>>>> refs/remotes/origin/master
		ctx->tx_remainder = 0;
	}

	/* adjust TX-remainder according to NCM specification. */
<<<<<<< HEAD
	ctx->tx_remainder = ((ctx->tx_remainder - ETH_HLEN) &
						(ctx->tx_modulus - 1));
=======
	ctx->tx_remainder = ((ctx->tx_remainder - eth_hlen) &
			     (ctx->tx_modulus - 1));
>>>>>>> refs/remotes/origin/master

	/* additional configuration */

	/* set CRC Mode */
	if (flags & USB_CDC_NCM_NCAP_CRC_MODE) {
<<<<<<< HEAD
		err = usb_control_msg(ctx->udev, usb_sndctrlpipe(ctx->udev, 0),
				USB_CDC_SET_CRC_MODE,
				USB_TYPE_CLASS | USB_DIR_OUT
				 | USB_RECIP_INTERFACE,
				USB_CDC_NCM_CRC_NOT_APPENDED,
				iface_no, NULL, 0, 1000);
		if (err < 0)
			pr_debug("Setting CRC mode off failed\n");
=======
		err = usbnet_write_cmd(dev, USB_CDC_SET_CRC_MODE,
				       USB_TYPE_CLASS | USB_DIR_OUT
				       | USB_RECIP_INTERFACE,
				       USB_CDC_NCM_CRC_NOT_APPENDED,
				       iface_no, NULL, 0);
		if (err < 0)
			dev_dbg(&dev->intf->dev, "Setting CRC mode off failed\n");
>>>>>>> refs/remotes/origin/master
	}

	/* set NTB format, if both formats are supported */
	if (ntb_fmt_supported & USB_CDC_NCM_NTH32_SIGN) {
<<<<<<< HEAD
		err = usb_control_msg(ctx->udev, usb_sndctrlpipe(ctx->udev, 0),
				USB_CDC_SET_NTB_FORMAT, USB_TYPE_CLASS
				 | USB_DIR_OUT | USB_RECIP_INTERFACE,
				USB_CDC_NCM_NTB16_FORMAT,
				iface_no, NULL, 0, 1000);
		if (err < 0)
			pr_debug("Setting NTB format to 16-bit failed\n");
	}

	ctx->max_datagram_size = CDC_NCM_MIN_DATAGRAM_SIZE;

	/* set Max Datagram Size (MTU) */
	if (flags & USB_CDC_NCM_NCAP_MAX_DATAGRAM_SIZE) {
		__le16 *max_datagram_size;
		u16 eth_max_sz = le16_to_cpu(ctx->ether_desc->wMaxSegmentSize);

		max_datagram_size = kzalloc(sizeof(*max_datagram_size),
				GFP_KERNEL);
		if (!max_datagram_size) {
			err = -ENOMEM;
			goto max_dgram_err;
		}

		err = usb_control_msg(ctx->udev, usb_rcvctrlpipe(ctx->udev, 0),
				USB_CDC_GET_MAX_DATAGRAM_SIZE,
				USB_TYPE_CLASS | USB_DIR_IN
				 | USB_RECIP_INTERFACE,
				0, iface_no, max_datagram_size,
				2, 1000);
		if (err < 0) {
			pr_debug("GET_MAX_DATAGRAM_SIZE failed, use size=%u\n",
						CDC_NCM_MIN_DATAGRAM_SIZE);
<<<<<<< HEAD
			kfree(max_datagram_size);
=======
>>>>>>> refs/remotes/origin/cm-10.0
		} else {
			ctx->max_datagram_size =
				le16_to_cpu(*max_datagram_size);
			/* Check Eth descriptor value */
<<<<<<< HEAD
			if (eth_max_sz < CDC_NCM_MAX_DATAGRAM_SIZE) {
				if (ctx->max_datagram_size > eth_max_sz)
					ctx->max_datagram_size = eth_max_sz;
			} else {
				if (ctx->max_datagram_size >
						CDC_NCM_MAX_DATAGRAM_SIZE)
					ctx->max_datagram_size =
						CDC_NCM_MAX_DATAGRAM_SIZE;
			}
=======
			if (ctx->max_datagram_size > eth_max_sz)
					ctx->max_datagram_size = eth_max_sz;

			if (ctx->max_datagram_size > CDC_NCM_MAX_DATAGRAM_SIZE)
				ctx->max_datagram_size =
						CDC_NCM_MAX_DATAGRAM_SIZE;
>>>>>>> refs/remotes/origin/cm-10.0

			if (ctx->max_datagram_size < CDC_NCM_MIN_DATAGRAM_SIZE)
				ctx->max_datagram_size =
					CDC_NCM_MIN_DATAGRAM_SIZE;

			/* if value changed, update device */
<<<<<<< HEAD
			err = usb_control_msg(ctx->udev,
=======
			if (ctx->max_datagram_size !=
					le16_to_cpu(*max_datagram_size)) {
				err = usb_control_msg(ctx->udev,
>>>>>>> refs/remotes/origin/cm-10.0
						usb_sndctrlpipe(ctx->udev, 0),
						USB_CDC_SET_MAX_DATAGRAM_SIZE,
						USB_TYPE_CLASS | USB_DIR_OUT
						 | USB_RECIP_INTERFACE,
						0,
						iface_no, max_datagram_size,
						2, 1000);
<<<<<<< HEAD
			kfree(max_datagram_size);
max_dgram_err:
			if (err < 0)
				pr_debug("SET_MAX_DATAGRAM_SIZE failed\n");
		}

	}

=======
				if (err < 0)
					pr_debug("SET_MAX_DGRAM_SIZE failed\n");
			}
		}
		kfree(max_datagram_size);
	}

max_dgram_err:
>>>>>>> refs/remotes/origin/cm-10.0
	if (ctx->netdev->mtu != (ctx->max_datagram_size - ETH_HLEN))
		ctx->netdev->mtu = ctx->max_datagram_size - ETH_HLEN;

=======
		err = usbnet_write_cmd(dev, USB_CDC_SET_NTB_FORMAT,
				       USB_TYPE_CLASS | USB_DIR_OUT
				       | USB_RECIP_INTERFACE,
				       USB_CDC_NCM_NTB16_FORMAT,
				       iface_no, NULL, 0);
		if (err < 0)
			dev_dbg(&dev->intf->dev, "Setting NTB format to 16-bit failed\n");
	}

	/* inform the device about the selected Max Datagram Size */
	if (!(flags & USB_CDC_NCM_NCAP_MAX_DATAGRAM_SIZE))
		goto out;

	/* read current mtu value from device */
	err = usbnet_read_cmd(dev, USB_CDC_GET_MAX_DATAGRAM_SIZE,
			      USB_TYPE_CLASS | USB_DIR_IN | USB_RECIP_INTERFACE,
			      0, iface_no, &max_datagram_size, 2);
	if (err < 0) {
		dev_dbg(&dev->intf->dev, "GET_MAX_DATAGRAM_SIZE failed\n");
		goto out;
	}

	if (le16_to_cpu(max_datagram_size) == ctx->max_datagram_size)
		goto out;

	max_datagram_size = cpu_to_le16(ctx->max_datagram_size);
	err = usbnet_write_cmd(dev, USB_CDC_SET_MAX_DATAGRAM_SIZE,
			       USB_TYPE_CLASS | USB_DIR_OUT | USB_RECIP_INTERFACE,
			       0, iface_no, &max_datagram_size, 2);
	if (err < 0)
		dev_dbg(&dev->intf->dev, "SET_MAX_DATAGRAM_SIZE failed\n");

out:
	/* set MTU to max supported by the device if necessary */
	if (dev->net->mtu > ctx->max_datagram_size - eth_hlen)
		dev->net->mtu = ctx->max_datagram_size - eth_hlen;
>>>>>>> refs/remotes/origin/master
	return 0;
}

static void
<<<<<<< HEAD
cdc_ncm_find_endpoints(struct cdc_ncm_ctx *ctx, struct usb_interface *intf)
{
	struct usb_host_endpoint *e;
=======
cdc_ncm_find_endpoints(struct usbnet *dev, struct usb_interface *intf)
{
	struct usb_host_endpoint *e, *in = NULL, *out = NULL;
>>>>>>> refs/remotes/origin/master
	u8 ep;

	for (ep = 0; ep < intf->cur_altsetting->desc.bNumEndpoints; ep++) {

		e = intf->cur_altsetting->endpoint + ep;
		switch (e->desc.bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) {
		case USB_ENDPOINT_XFER_INT:
			if (usb_endpoint_dir_in(&e->desc)) {
<<<<<<< HEAD
				if (ctx->status_ep == NULL)
					ctx->status_ep = e;
=======
				if (!dev->status)
					dev->status = e;
>>>>>>> refs/remotes/origin/master
			}
			break;

		case USB_ENDPOINT_XFER_BULK:
			if (usb_endpoint_dir_in(&e->desc)) {
<<<<<<< HEAD
				if (ctx->in_ep == NULL)
					ctx->in_ep = e;
			} else {
				if (ctx->out_ep == NULL)
					ctx->out_ep = e;
=======
				if (!in)
					in = e;
			} else {
				if (!out)
					out = e;
>>>>>>> refs/remotes/origin/master
			}
			break;

		default:
			break;
		}
	}
<<<<<<< HEAD
=======
	if (in && !dev->in)
		dev->in = usb_rcvbulkpipe(dev->udev,
					  in->desc.bEndpointAddress &
					  USB_ENDPOINT_NUMBER_MASK);
	if (out && !dev->out)
		dev->out = usb_sndbulkpipe(dev->udev,
					   out->desc.bEndpointAddress &
					   USB_ENDPOINT_NUMBER_MASK);
>>>>>>> refs/remotes/origin/master
}

static void cdc_ncm_free(struct cdc_ncm_ctx *ctx)
{
	if (ctx == NULL)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	del_timer_sync(&ctx->tx_timer);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (ctx->tx_rem_skb != NULL) {
		dev_kfree_skb_any(ctx->tx_rem_skb);
		ctx->tx_rem_skb = NULL;
	}

	if (ctx->tx_curr_skb != NULL) {
		dev_kfree_skb_any(ctx->tx_curr_skb);
		ctx->tx_curr_skb = NULL;
	}

	kfree(ctx);
}

<<<<<<< HEAD
static int cdc_ncm_bind(struct usbnet *dev, struct usb_interface *intf)
{
=======
int cdc_ncm_bind_common(struct usbnet *dev, struct usb_interface *intf, u8 data_altsetting)
{
	const struct usb_cdc_union_desc *union_desc = NULL;
>>>>>>> refs/remotes/origin/master
	struct cdc_ncm_ctx *ctx;
	struct usb_driver *driver;
	u8 *buf;
	int len;
	int temp;
	u8 iface_no;

<<<<<<< HEAD
<<<<<<< HEAD
	ctx = kmalloc(sizeof(*ctx), GFP_KERNEL);
	if (ctx == NULL)
		return -ENODEV;

	memset(ctx, 0, sizeof(*ctx));

	init_timer(&ctx->tx_timer);
=======
	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (ctx == NULL)
		return -ENODEV;

	hrtimer_init(&ctx->tx_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	ctx->tx_timer.function = &cdc_ncm_tx_timer_cb;
	ctx->bh.data = (unsigned long)ctx;
	ctx->bh.func = cdc_ncm_txpath_bh;
	atomic_set(&ctx->stop, 0);
>>>>>>> refs/remotes/origin/cm-10.0
	spin_lock_init(&ctx->mtx);
	ctx->netdev = dev->net;
=======
	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	hrtimer_init(&ctx->tx_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	ctx->tx_timer.function = &cdc_ncm_tx_timer_cb;
	ctx->bh.data = (unsigned long)dev;
	ctx->bh.func = cdc_ncm_txpath_bh;
	atomic_set(&ctx->stop, 0);
	spin_lock_init(&ctx->mtx);
>>>>>>> refs/remotes/origin/master

	/* store ctx pointer in device data field */
	dev->data[0] = (unsigned long)ctx;

<<<<<<< HEAD
=======
	/* only the control interface can be successfully probed */
	ctx->control = intf;

>>>>>>> refs/remotes/origin/master
	/* get some pointers */
	driver = driver_of(intf);
	buf = intf->cur_altsetting->extra;
	len = intf->cur_altsetting->extralen;

<<<<<<< HEAD
	ctx->udev = dev->udev;
	ctx->intf = intf;

=======
>>>>>>> refs/remotes/origin/master
	/* parse through descriptors associated with control interface */
	while ((len > 0) && (buf[0] > 2) && (buf[0] <= len)) {

		if (buf[1] != USB_DT_CS_INTERFACE)
			goto advance;

		switch (buf[2]) {
		case USB_CDC_UNION_TYPE:
<<<<<<< HEAD
			if (buf[0] < sizeof(*(ctx->union_desc)))
				break;

			ctx->union_desc =
					(const struct usb_cdc_union_desc *)buf;

			ctx->control = usb_ifnum_to_if(dev->udev,
					ctx->union_desc->bMasterInterface0);
			ctx->data = usb_ifnum_to_if(dev->udev,
					ctx->union_desc->bSlaveInterface0);
=======
			if (buf[0] < sizeof(*union_desc))
				break;

			union_desc = (const struct usb_cdc_union_desc *)buf;
			/* the master must be the interface we are probing */
			if (intf->cur_altsetting->desc.bInterfaceNumber !=
			    union_desc->bMasterInterface0) {
				dev_dbg(&intf->dev, "bogus CDC Union\n");
				goto error;
			}
			ctx->data = usb_ifnum_to_if(dev->udev,
						    union_desc->bSlaveInterface0);
>>>>>>> refs/remotes/origin/master
			break;

		case USB_CDC_ETHERNET_TYPE:
			if (buf[0] < sizeof(*(ctx->ether_desc)))
				break;

			ctx->ether_desc =
					(const struct usb_cdc_ether_desc *)buf;
<<<<<<< HEAD
			dev->hard_mtu =
				le16_to_cpu(ctx->ether_desc->wMaxSegmentSize);

			if (dev->hard_mtu < CDC_NCM_MIN_DATAGRAM_SIZE)
				dev->hard_mtu =	CDC_NCM_MIN_DATAGRAM_SIZE;
			else if (dev->hard_mtu > CDC_NCM_MAX_DATAGRAM_SIZE)
				dev->hard_mtu =	CDC_NCM_MAX_DATAGRAM_SIZE;
=======
>>>>>>> refs/remotes/origin/master
			break;

		case USB_CDC_NCM_TYPE:
			if (buf[0] < sizeof(*(ctx->func_desc)))
				break;

			ctx->func_desc = (const struct usb_cdc_ncm_desc *)buf;
			break;

<<<<<<< HEAD
=======
		case USB_CDC_MBIM_TYPE:
			if (buf[0] < sizeof(*(ctx->mbim_desc)))
				break;

			ctx->mbim_desc = (const struct usb_cdc_mbim_desc *)buf;
			break;

>>>>>>> refs/remotes/origin/master
		default:
			break;
		}
advance:
		/* advance to next descriptor */
		temp = buf[0];
		buf += temp;
		len -= temp;
	}

<<<<<<< HEAD
	/* check if we got everything */
	if ((ctx->control == NULL) || (ctx->data == NULL) ||
	    (ctx->ether_desc == NULL) || (ctx->control != intf))
		goto error;

	/* claim interfaces, if any */
	temp = usb_driver_claim_interface(driver, ctx->data, dev);
	if (temp)
		goto error;
=======
	/* some buggy devices have an IAD but no CDC Union */
	if (!union_desc && intf->intf_assoc && intf->intf_assoc->bInterfaceCount == 2) {
		ctx->data = usb_ifnum_to_if(dev->udev, intf->cur_altsetting->desc.bInterfaceNumber + 1);
		dev_dbg(&intf->dev, "CDC Union missing - got slave from IAD\n");
	}

	/* check if we got everything */
	if (!ctx->data || (!ctx->mbim_desc && !ctx->ether_desc)) {
		dev_dbg(&intf->dev, "CDC descriptors missing\n");
		goto error;
	}

	/* claim data interface, if different from control */
	if (ctx->data != ctx->control) {
		temp = usb_driver_claim_interface(driver, ctx->data, dev);
		if (temp) {
			dev_dbg(&intf->dev, "failed to claim data intf\n");
			goto error;
		}
	}
>>>>>>> refs/remotes/origin/master

	iface_no = ctx->data->cur_altsetting->desc.bInterfaceNumber;

	/* reset data interface */
	temp = usb_set_interface(dev->udev, iface_no, 0);
<<<<<<< HEAD
	if (temp)
		goto error2;

	/* initialize data interface */
	if (cdc_ncm_setup(ctx))
		goto error2;

	/* configure data interface */
	temp = usb_set_interface(dev->udev, iface_no, 1);
	if (temp)
		goto error2;

	cdc_ncm_find_endpoints(ctx, ctx->data);
	cdc_ncm_find_endpoints(ctx, ctx->control);

	if ((ctx->in_ep == NULL) || (ctx->out_ep == NULL) ||
	    (ctx->status_ep == NULL))
		goto error2;

	dev->net->ethtool_ops = &cdc_ncm_ethtool_ops;

	usb_set_intfdata(ctx->data, dev);
	usb_set_intfdata(ctx->control, dev);
	usb_set_intfdata(ctx->intf, dev);

	temp = usbnet_get_ethernet_addr(dev, ctx->ether_desc->iMACAddress);
	if (temp)
		goto error2;

<<<<<<< HEAD
	dev_info(&dev->udev->dev, "MAC-Address: "
				"0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n",
				dev->net->dev_addr[0], dev->net->dev_addr[1],
				dev->net->dev_addr[2], dev->net->dev_addr[3],
				dev->net->dev_addr[4], dev->net->dev_addr[5]);
=======
	dev_info(&dev->udev->dev, "MAC-Address: %pM\n", dev->net->dev_addr);
>>>>>>> refs/remotes/origin/cm-10.0

	dev->in = usb_rcvbulkpipe(dev->udev,
		ctx->in_ep->desc.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
	dev->out = usb_sndbulkpipe(dev->udev,
		ctx->out_ep->desc.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
	dev->status = ctx->status_ep;
	dev->rx_urb_size = ctx->rx_max;

	/*
	 * We should get an event when network connection is "connected" or
	 * "disconnected". Set network connection in "disconnected" state
	 * (carrier is OFF) during attach, so the IP network stack does not
	 * start IPv6 negotiation and more.
	 */
	netif_carrier_off(dev->net);
	ctx->tx_speed = ctx->rx_speed = 0;
=======
	if (temp) {
		dev_dbg(&intf->dev, "set interface failed\n");
		goto error2;
	}

	/* configure data interface */
	temp = usb_set_interface(dev->udev, iface_no, data_altsetting);
	if (temp) {
		dev_dbg(&intf->dev, "set interface failed\n");
		goto error2;
	}

	cdc_ncm_find_endpoints(dev, ctx->data);
	cdc_ncm_find_endpoints(dev, ctx->control);
	if (!dev->in || !dev->out || !dev->status) {
		dev_dbg(&intf->dev, "failed to collect endpoints\n");
		goto error2;
	}

	/* initialize data interface */
	if (cdc_ncm_setup(dev))	{
		dev_dbg(&intf->dev, "cdc_ncm_setup() failed\n");
		goto error2;
	}

	usb_set_intfdata(ctx->data, dev);
	usb_set_intfdata(ctx->control, dev);

	if (ctx->ether_desc) {
		temp = usbnet_get_ethernet_addr(dev, ctx->ether_desc->iMACAddress);
		if (temp) {
			dev_dbg(&intf->dev, "failed to get mac address\n");
			goto error2;
		}
		dev_info(&intf->dev, "MAC-Address: %pM\n", dev->net->dev_addr);
	}

	/* usbnet use these values for sizing tx/rx queues */
	dev->hard_mtu = ctx->tx_max;
	dev->rx_urb_size = ctx->rx_max;

>>>>>>> refs/remotes/origin/master
	return 0;

error2:
	usb_set_intfdata(ctx->control, NULL);
	usb_set_intfdata(ctx->data, NULL);
<<<<<<< HEAD
	usb_driver_release_interface(driver, ctx->data);
error:
	cdc_ncm_free((struct cdc_ncm_ctx *)dev->data[0]);
	dev->data[0] = 0;
	dev_info(&dev->udev->dev, "bind() failure\n");
	return -ENODEV;
}

static void cdc_ncm_unbind(struct usbnet *dev, struct usb_interface *intf)
=======
	if (ctx->data != ctx->control)
		usb_driver_release_interface(driver, ctx->data);
error:
	cdc_ncm_free((struct cdc_ncm_ctx *)dev->data[0]);
	dev->data[0] = 0;
	dev_info(&intf->dev, "bind() failure\n");
	return -ENODEV;
}
EXPORT_SYMBOL_GPL(cdc_ncm_bind_common);

void cdc_ncm_unbind(struct usbnet *dev, struct usb_interface *intf)
>>>>>>> refs/remotes/origin/master
{
	struct cdc_ncm_ctx *ctx = (struct cdc_ncm_ctx *)dev->data[0];
	struct usb_driver *driver = driver_of(intf);

	if (ctx == NULL)
		return;		/* no setup */

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	atomic_set(&ctx->stop, 1);

	if (hrtimer_active(&ctx->tx_timer))
		hrtimer_cancel(&ctx->tx_timer);

	tasklet_kill(&ctx->bh);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* handle devices with combined control and data interface */
	if (ctx->control == ctx->data)
		ctx->data = NULL;

>>>>>>> refs/remotes/origin/master
	/* disconnect master --> disconnect slave */
	if (intf == ctx->control && ctx->data) {
		usb_set_intfdata(ctx->data, NULL);
		usb_driver_release_interface(driver, ctx->data);
		ctx->data = NULL;

	} else if (intf == ctx->data && ctx->control) {
		usb_set_intfdata(ctx->control, NULL);
		usb_driver_release_interface(driver, ctx->control);
		ctx->control = NULL;
	}

<<<<<<< HEAD
	usb_set_intfdata(ctx->intf, NULL);
	cdc_ncm_free(ctx);
}

static void cdc_ncm_zero_fill(u8 *ptr, u32 first, u32 end, u32 max)
{
	if (first >= max)
		return;
	if (first >= end)
		return;
	if (end > max)
		end = max;
	memset(ptr + first, 0, end - first);
}

static struct sk_buff *
cdc_ncm_fill_tx_frame(struct cdc_ncm_ctx *ctx, struct sk_buff *skb)
{
	struct sk_buff *skb_out;
	u32 rem;
	u32 offset;
	u32 last_offset;
	u16 n = 0, index;
	u8 ready2send = 0;

	/* if there is a remaining skb, it gets priority */
	if (skb != NULL)
		swap(skb, ctx->tx_rem_skb);
	else
		ready2send = 1;

	/*
	 * +----------------+
	 * | skb_out        |
	 * +----------------+
	 *           ^ offset
	 *        ^ last_offset
	 */

	/* check if we are resuming an OUT skb */
	if (ctx->tx_curr_skb != NULL) {
		/* pop variables */
		skb_out = ctx->tx_curr_skb;
		offset = ctx->tx_curr_offset;
		last_offset = ctx->tx_curr_last_offset;
		n = ctx->tx_curr_frame_num;

	} else {
		/* reset variables */
		skb_out = alloc_skb((ctx->tx_max + 1), GFP_ATOMIC);
		if (skb_out == NULL) {
			if (skb != NULL) {
				dev_kfree_skb_any(skb);
				ctx->netdev->stats.tx_dropped++;
			}
			goto exit_no_skb;
		}

		/* make room for NTH and NDP */
		offset = ALIGN(sizeof(struct usb_cdc_ncm_nth16),
					ctx->tx_ndp_modulus) +
					sizeof(struct usb_cdc_ncm_ndp16) +
					(ctx->tx_max_datagrams + 1) *
					sizeof(struct usb_cdc_ncm_dpe16);

		/* store last valid offset before alignment */
		last_offset = offset;
		/* align first Datagram offset correctly */
		offset = ALIGN(offset, ctx->tx_modulus) + ctx->tx_remainder;
		/* zero buffer till the first IP datagram */
		cdc_ncm_zero_fill(skb_out->data, 0, offset, offset);
		n = 0;
		ctx->tx_curr_frame_num = 0;
	}

	for (; n < ctx->tx_max_datagrams; n++) {
		/* check if end of transmit buffer is reached */
		if (offset >= ctx->tx_max) {
			ready2send = 1;
			break;
		}
		/* compute maximum buffer size */
		rem = ctx->tx_max - offset;

		if (skb == NULL) {
			skb = ctx->tx_rem_skb;
=======
	usb_set_intfdata(intf, NULL);
	cdc_ncm_free(ctx);
}
EXPORT_SYMBOL_GPL(cdc_ncm_unbind);

/* Select the MBIM altsetting iff it is preferred and available,
 * returning the number of the corresponding data interface altsetting
 */
u8 cdc_ncm_select_altsetting(struct usbnet *dev, struct usb_interface *intf)
{
	struct usb_host_interface *alt;

	/* The MBIM spec defines a NCM compatible default altsetting,
	 * which we may have matched:
	 *
	 *  "Functions that implement both NCM 1.0 and MBIM (an
	 *   “NCM/MBIM function”) according to this recommendation
	 *   shall provide two alternate settings for the
	 *   Communication Interface.  Alternate setting 0, and the
	 *   associated class and endpoint descriptors, shall be
	 *   constructed according to the rules given for the
	 *   Communication Interface in section 5 of [USBNCM10].
	 *   Alternate setting 1, and the associated class and
	 *   endpoint descriptors, shall be constructed according to
	 *   the rules given in section 6 (USB Device Model) of this
	 *   specification."
	 */
	if (prefer_mbim && intf->num_altsetting == 2) {
		alt = usb_altnum_to_altsetting(intf, CDC_NCM_COMM_ALTSETTING_MBIM);
		if (alt && cdc_ncm_comm_intf_is_mbim(alt) &&
		    !usb_set_interface(dev->udev,
				       intf->cur_altsetting->desc.bInterfaceNumber,
				       CDC_NCM_COMM_ALTSETTING_MBIM))
			return CDC_NCM_DATA_ALTSETTING_MBIM;
	}
	return CDC_NCM_DATA_ALTSETTING_NCM;
}
EXPORT_SYMBOL_GPL(cdc_ncm_select_altsetting);

static int cdc_ncm_bind(struct usbnet *dev, struct usb_interface *intf)
{
	int ret;

	/* MBIM backwards compatible function? */
	cdc_ncm_select_altsetting(dev, intf);
	if (cdc_ncm_comm_intf_is_mbim(intf->cur_altsetting))
		return -ENODEV;

	/* NCM data altsetting is always 1 */
	ret = cdc_ncm_bind_common(dev, intf, 1);

	/*
	 * We should get an event when network connection is "connected" or
	 * "disconnected". Set network connection in "disconnected" state
	 * (carrier is OFF) during attach, so the IP network stack does not
	 * start IPv6 negotiation and more.
	 */
	usbnet_link_change(dev, 0, 0);
	return ret;
}

static void cdc_ncm_align_tail(struct sk_buff *skb, size_t modulus, size_t remainder, size_t max)
{
	size_t align = ALIGN(skb->len, modulus) - skb->len + remainder;

	if (skb->len + align > max)
		align = max - skb->len;
	if (align && skb_tailroom(skb) >= align)
		memset(skb_put(skb, align), 0, align);
}

/* return a pointer to a valid struct usb_cdc_ncm_ndp16 of type sign, possibly
 * allocating a new one within skb
 */
static struct usb_cdc_ncm_ndp16 *cdc_ncm_ndp(struct cdc_ncm_ctx *ctx, struct sk_buff *skb, __le32 sign, size_t reserve)
{
	struct usb_cdc_ncm_ndp16 *ndp16 = NULL;
	struct usb_cdc_ncm_nth16 *nth16 = (void *)skb->data;
	size_t ndpoffset = le16_to_cpu(nth16->wNdpIndex);

	/* follow the chain of NDPs, looking for a match */
	while (ndpoffset) {
		ndp16 = (struct usb_cdc_ncm_ndp16 *)(skb->data + ndpoffset);
		if  (ndp16->dwSignature == sign)
			return ndp16;
		ndpoffset = le16_to_cpu(ndp16->wNextNdpIndex);
	}

	/* align new NDP */
	cdc_ncm_align_tail(skb, ctx->tx_ndp_modulus, 0, ctx->tx_max);

	/* verify that there is room for the NDP and the datagram (reserve) */
	if ((ctx->tx_max - skb->len - reserve) < CDC_NCM_NDP_SIZE)
		return NULL;

	/* link to it */
	if (ndp16)
		ndp16->wNextNdpIndex = cpu_to_le16(skb->len);
	else
		nth16->wNdpIndex = cpu_to_le16(skb->len);

	/* push a new empty NDP */
	ndp16 = (struct usb_cdc_ncm_ndp16 *)memset(skb_put(skb, CDC_NCM_NDP_SIZE), 0, CDC_NCM_NDP_SIZE);
	ndp16->dwSignature = sign;
	ndp16->wLength = cpu_to_le16(sizeof(struct usb_cdc_ncm_ndp16) + sizeof(struct usb_cdc_ncm_dpe16));
	return ndp16;
}

struct sk_buff *
cdc_ncm_fill_tx_frame(struct usbnet *dev, struct sk_buff *skb, __le32 sign)
{
	struct cdc_ncm_ctx *ctx = (struct cdc_ncm_ctx *)dev->data[0];
	struct usb_cdc_ncm_nth16 *nth16;
	struct usb_cdc_ncm_ndp16 *ndp16;
	struct sk_buff *skb_out;
	u16 n = 0, index, ndplen;
	u8 ready2send = 0;

	/* if there is a remaining skb, it gets priority */
	if (skb != NULL) {
		swap(skb, ctx->tx_rem_skb);
		swap(sign, ctx->tx_rem_sign);
	} else {
		ready2send = 1;
	}

	/* check if we are resuming an OUT skb */
	skb_out = ctx->tx_curr_skb;

	/* allocate a new OUT skb */
	if (!skb_out) {
		skb_out = alloc_skb(ctx->tx_max, GFP_ATOMIC);
		if (skb_out == NULL) {
			if (skb != NULL) {
				dev_kfree_skb_any(skb);
				dev->net->stats.tx_dropped++;
			}
			goto exit_no_skb;
		}
		/* fill out the initial 16-bit NTB header */
		nth16 = (struct usb_cdc_ncm_nth16 *)memset(skb_put(skb_out, sizeof(struct usb_cdc_ncm_nth16)), 0, sizeof(struct usb_cdc_ncm_nth16));
		nth16->dwSignature = cpu_to_le32(USB_CDC_NCM_NTH16_SIGN);
		nth16->wHeaderLength = cpu_to_le16(sizeof(struct usb_cdc_ncm_nth16));
		nth16->wSequence = cpu_to_le16(ctx->tx_seq++);

		/* count total number of frames in this NTB */
		ctx->tx_curr_frame_num = 0;
	}

	for (n = ctx->tx_curr_frame_num; n < ctx->tx_max_datagrams; n++) {
		/* send any remaining skb first */
		if (skb == NULL) {
			skb = ctx->tx_rem_skb;
			sign = ctx->tx_rem_sign;
>>>>>>> refs/remotes/origin/master
			ctx->tx_rem_skb = NULL;

			/* check for end of skb */
			if (skb == NULL)
				break;
		}

<<<<<<< HEAD
		if (skb->len > rem) {
=======
		/* get the appropriate NDP for this skb */
		ndp16 = cdc_ncm_ndp(ctx, skb_out, sign, skb->len + ctx->tx_modulus + ctx->tx_remainder);

		/* align beginning of next frame */
		cdc_ncm_align_tail(skb_out,  ctx->tx_modulus, ctx->tx_remainder, ctx->tx_max);

		/* check if we had enough room left for both NDP and frame */
		if (!ndp16 || skb_out->len + skb->len > ctx->tx_max) {
>>>>>>> refs/remotes/origin/master
			if (n == 0) {
				/* won't fit, MTU problem? */
				dev_kfree_skb_any(skb);
				skb = NULL;
<<<<<<< HEAD
				ctx->netdev->stats.tx_dropped++;
=======
				dev->net->stats.tx_dropped++;
>>>>>>> refs/remotes/origin/master
			} else {
				/* no room for skb - store for later */
				if (ctx->tx_rem_skb != NULL) {
					dev_kfree_skb_any(ctx->tx_rem_skb);
<<<<<<< HEAD
					ctx->netdev->stats.tx_dropped++;
				}
				ctx->tx_rem_skb = skb;
=======
					dev->net->stats.tx_dropped++;
				}
				ctx->tx_rem_skb = skb;
				ctx->tx_rem_sign = sign;
>>>>>>> refs/remotes/origin/master
				skb = NULL;
				ready2send = 1;
			}
			break;
		}

<<<<<<< HEAD
		memcpy(((u8 *)skb_out->data) + offset, skb->data, skb->len);

		ctx->tx_ncm.dpe16[n].wDatagramLength = cpu_to_le16(skb->len);
		ctx->tx_ncm.dpe16[n].wDatagramIndex = cpu_to_le16(offset);

		/* update offset */
		offset += skb->len;

		/* store last valid offset before alignment */
		last_offset = offset;

		/* align offset correctly */
		offset = ALIGN(offset, ctx->tx_modulus) + ctx->tx_remainder;

		/* zero padding */
		cdc_ncm_zero_fill(skb_out->data, last_offset, offset,
								ctx->tx_max);
		dev_kfree_skb_any(skb);
		skb = NULL;
=======
		/* calculate frame number withing this NDP */
		ndplen = le16_to_cpu(ndp16->wLength);
		index = (ndplen - sizeof(struct usb_cdc_ncm_ndp16)) / sizeof(struct usb_cdc_ncm_dpe16) - 1;

		/* OK, add this skb */
		ndp16->dpe16[index].wDatagramLength = cpu_to_le16(skb->len);
		ndp16->dpe16[index].wDatagramIndex = cpu_to_le16(skb_out->len);
		ndp16->wLength = cpu_to_le16(ndplen + sizeof(struct usb_cdc_ncm_dpe16));
		memcpy(skb_put(skb_out, skb->len), skb->data, skb->len);
		dev_kfree_skb_any(skb);
		skb = NULL;

		/* send now if this NDP is full */
		if (index >= CDC_NCM_DPT_DATAGRAMS_MAX) {
			ready2send = 1;
			break;
		}
>>>>>>> refs/remotes/origin/master
	}

	/* free up any dangling skb */
	if (skb != NULL) {
		dev_kfree_skb_any(skb);
		skb = NULL;
<<<<<<< HEAD
		ctx->netdev->stats.tx_dropped++;
=======
		dev->net->stats.tx_dropped++;
>>>>>>> refs/remotes/origin/master
	}

	ctx->tx_curr_frame_num = n;

	if (n == 0) {
		/* wait for more frames */
		/* push variables */
		ctx->tx_curr_skb = skb_out;
<<<<<<< HEAD
		ctx->tx_curr_offset = offset;
		ctx->tx_curr_last_offset = last_offset;
=======
>>>>>>> refs/remotes/origin/master
		goto exit_no_skb;

	} else if ((n < ctx->tx_max_datagrams) && (ready2send == 0)) {
		/* wait for more frames */
		/* push variables */
		ctx->tx_curr_skb = skb_out;
<<<<<<< HEAD
		ctx->tx_curr_offset = offset;
		ctx->tx_curr_last_offset = last_offset;
		/* set the pending count */
		if (n < CDC_NCM_RESTART_TIMER_DATAGRAM_CNT)
<<<<<<< HEAD
			ctx->tx_timer_pending = 2;
=======
			ctx->tx_timer_pending = CDC_NCM_TIMER_PENDING_CNT;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		/* set the pending count */
		if (n < CDC_NCM_RESTART_TIMER_DATAGRAM_CNT)
			ctx->tx_timer_pending = CDC_NCM_TIMER_PENDING_CNT;
>>>>>>> refs/remotes/origin/master
		goto exit_no_skb;

	} else {
		/* frame goes out */
		/* variables will be reset at next call */
	}

<<<<<<< HEAD
	/* check for overflow */
	if (last_offset > ctx->tx_max)
		last_offset = ctx->tx_max;

	/* revert offset */
	offset = last_offset;

	/*
	 * If collected data size is less or equal CDC_NCM_MIN_TX_PKT bytes,
	 * we send buffers as it is. If we get more data, it would be more
	 * efficient for USB HS mobile device with DMA engine to receive a full
	 * size NTB, than canceling DMA transfer and receiving a short packet.
	 */
	if (offset > CDC_NCM_MIN_TX_PKT)
		offset = ctx->tx_max;

	/* final zero padding */
	cdc_ncm_zero_fill(skb_out->data, last_offset, offset, ctx->tx_max);

	/* store last offset */
	last_offset = offset;

	if (((last_offset < ctx->tx_max) && ((last_offset %
			le16_to_cpu(ctx->out_ep->desc.wMaxPacketSize)) == 0)) ||
	    (((last_offset == ctx->tx_max) && ((ctx->tx_max %
		le16_to_cpu(ctx->out_ep->desc.wMaxPacketSize)) == 0)) &&
		(ctx->tx_max < le32_to_cpu(ctx->ncm_parm.dwNtbOutMaxSize)))) {
		/* force short packet */
		*(((u8 *)skb_out->data) + last_offset) = 0;
		last_offset++;
	}

	/* zero the rest of the DPEs plus the last NULL entry */
	for (; n <= CDC_NCM_DPT_DATAGRAMS_MAX; n++) {
		ctx->tx_ncm.dpe16[n].wDatagramLength = 0;
		ctx->tx_ncm.dpe16[n].wDatagramIndex = 0;
	}

	/* fill out 16-bit NTB header */
	ctx->tx_ncm.nth16.dwSignature = cpu_to_le32(USB_CDC_NCM_NTH16_SIGN);
	ctx->tx_ncm.nth16.wHeaderLength =
					cpu_to_le16(sizeof(ctx->tx_ncm.nth16));
	ctx->tx_ncm.nth16.wSequence = cpu_to_le16(ctx->tx_seq);
	ctx->tx_ncm.nth16.wBlockLength = cpu_to_le16(last_offset);
	index = ALIGN(sizeof(struct usb_cdc_ncm_nth16), ctx->tx_ndp_modulus);
	ctx->tx_ncm.nth16.wNdpIndex = cpu_to_le16(index);

	memcpy(skb_out->data, &(ctx->tx_ncm.nth16), sizeof(ctx->tx_ncm.nth16));
	ctx->tx_seq++;

	/* fill out 16-bit NDP table */
	ctx->tx_ncm.ndp16.dwSignature =
				cpu_to_le32(USB_CDC_NCM_NDP16_NOCRC_SIGN);
	rem = sizeof(ctx->tx_ncm.ndp16) + ((ctx->tx_curr_frame_num + 1) *
					sizeof(struct usb_cdc_ncm_dpe16));
	ctx->tx_ncm.ndp16.wLength = cpu_to_le16(rem);
	ctx->tx_ncm.ndp16.wNextNdpIndex = 0; /* reserved */

	memcpy(((u8 *)skb_out->data) + index,
						&(ctx->tx_ncm.ndp16),
						sizeof(ctx->tx_ncm.ndp16));

	memcpy(((u8 *)skb_out->data) + index + sizeof(ctx->tx_ncm.ndp16),
					&(ctx->tx_ncm.dpe16),
					(ctx->tx_curr_frame_num + 1) *
					sizeof(struct usb_cdc_ncm_dpe16));

	/* set frame length */
	skb_put(skb_out, last_offset);

	/* return skb */
	ctx->tx_curr_skb = NULL;
<<<<<<< HEAD
	return skb_out;

exit_no_skb:
=======
	ctx->netdev->stats.tx_packets += ctx->tx_curr_frame_num;
=======
	/* If collected data size is less or equal CDC_NCM_MIN_TX_PKT
	 * bytes, we send buffers as it is. If we get more data, it
	 * would be more efficient for USB HS mobile device with DMA
	 * engine to receive a full size NTB, than canceling DMA
	 * transfer and receiving a short packet.
	 *
	 * This optimization support is pointless if we end up sending
	 * a ZLP after full sized NTBs.
	 */
	if (!(dev->driver_info->flags & FLAG_SEND_ZLP) &&
	    skb_out->len > CDC_NCM_MIN_TX_PKT)
		memset(skb_put(skb_out, ctx->tx_max - skb_out->len), 0,
		       ctx->tx_max - skb_out->len);
	else if ((skb_out->len % dev->maxpacket) == 0)
		*skb_put(skb_out, 1) = 0;	/* force short packet */

	/* set final frame length */
	nth16 = (struct usb_cdc_ncm_nth16 *)skb_out->data;
	nth16->wBlockLength = cpu_to_le16(skb_out->len);

	/* return skb */
	ctx->tx_curr_skb = NULL;
	dev->net->stats.tx_packets += ctx->tx_curr_frame_num;
>>>>>>> refs/remotes/origin/master
	return skb_out;

exit_no_skb:
	/* Start timer, if there is a remaining skb */
	if (ctx->tx_curr_skb != NULL)
		cdc_ncm_tx_timeout_start(ctx);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	return NULL;
}
=======
	return NULL;
}
EXPORT_SYMBOL_GPL(cdc_ncm_fill_tx_frame);
>>>>>>> refs/remotes/origin/master

static void cdc_ncm_tx_timeout_start(struct cdc_ncm_ctx *ctx)
{
	/* start timer, if not already started */
<<<<<<< HEAD
<<<<<<< HEAD
	if (timer_pending(&ctx->tx_timer) == 0) {
		ctx->tx_timer.function = &cdc_ncm_tx_timeout;
		ctx->tx_timer.data = (unsigned long)ctx;
		ctx->tx_timer.expires = jiffies + ((HZ + 999) / 1000);
		add_timer(&ctx->tx_timer);
	}
}

static void cdc_ncm_tx_timeout(unsigned long arg)
{
	struct cdc_ncm_ctx *ctx = (struct cdc_ncm_ctx *)arg;
	u8 restart;

	spin_lock(&ctx->mtx);
	if (ctx->tx_timer_pending != 0) {
		ctx->tx_timer_pending--;
		restart = 1;
	} else {
		restart = 0;
	}

	spin_unlock(&ctx->mtx);

	if (restart) {
		spin_lock(&ctx->mtx);
		cdc_ncm_tx_timeout_start(ctx);
		spin_unlock(&ctx->mtx);
	} else if (ctx->netdev != NULL) {
		usbnet_start_xmit(NULL, ctx->netdev);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!(hrtimer_active(&ctx->tx_timer) || atomic_read(&ctx->stop)))
		hrtimer_start(&ctx->tx_timer,
				ktime_set(0, CDC_NCM_TIMER_INTERVAL),
				HRTIMER_MODE_REL);
}

static enum hrtimer_restart cdc_ncm_tx_timer_cb(struct hrtimer *timer)
{
	struct cdc_ncm_ctx *ctx =
			container_of(timer, struct cdc_ncm_ctx, tx_timer);

	if (!atomic_read(&ctx->stop))
		tasklet_schedule(&ctx->bh);
	return HRTIMER_NORESTART;
}

static void cdc_ncm_txpath_bh(unsigned long param)
{
<<<<<<< HEAD
	struct cdc_ncm_ctx *ctx = (struct cdc_ncm_ctx *)param;
=======
	struct usbnet *dev = (struct usbnet *)param;
	struct cdc_ncm_ctx *ctx = (struct cdc_ncm_ctx *)dev->data[0];
>>>>>>> refs/remotes/origin/master

	spin_lock_bh(&ctx->mtx);
	if (ctx->tx_timer_pending != 0) {
		ctx->tx_timer_pending--;
		cdc_ncm_tx_timeout_start(ctx);
		spin_unlock_bh(&ctx->mtx);
<<<<<<< HEAD
	} else if (ctx->netdev != NULL) {
		spin_unlock_bh(&ctx->mtx);
		netif_tx_lock_bh(ctx->netdev);
		usbnet_start_xmit(NULL, ctx->netdev);
		netif_tx_unlock_bh(ctx->netdev);
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

static struct sk_buff *
=======
	} else if (dev->net != NULL) {
		spin_unlock_bh(&ctx->mtx);
		netif_tx_lock_bh(dev->net);
		usbnet_start_xmit(NULL, dev->net);
		netif_tx_unlock_bh(dev->net);
	} else {
		spin_unlock_bh(&ctx->mtx);
	}
}

struct sk_buff *
>>>>>>> refs/remotes/origin/master
cdc_ncm_tx_fixup(struct usbnet *dev, struct sk_buff *skb, gfp_t flags)
{
	struct sk_buff *skb_out;
	struct cdc_ncm_ctx *ctx = (struct cdc_ncm_ctx *)dev->data[0];
<<<<<<< HEAD
<<<<<<< HEAD
	u8 need_timer = 0;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * The Ethernet API we are using does not support transmitting
	 * multiple Ethernet frames in a single call. This driver will
	 * accumulate multiple Ethernet frames and send out a larger
	 * USB frame when the USB buffer is full or when a single jiffies
	 * timeout happens.
	 */
	if (ctx == NULL)
		goto error;

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock(&ctx->mtx);
	skb_out = cdc_ncm_fill_tx_frame(ctx, skb);
	if (ctx->tx_curr_skb != NULL)
		need_timer = 1;

	/* Start timer, if there is a remaining skb */
	if (need_timer)
		cdc_ncm_tx_timeout_start(ctx);

	if (skb_out)
		dev->net->stats.tx_packets += ctx->tx_curr_frame_num;

	spin_unlock(&ctx->mtx);
=======
	spin_lock_bh(&ctx->mtx);
	skb_out = cdc_ncm_fill_tx_frame(ctx, skb);
	spin_unlock_bh(&ctx->mtx);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_lock_bh(&ctx->mtx);
	skb_out = cdc_ncm_fill_tx_frame(dev, skb, cpu_to_le32(USB_CDC_NCM_NDP16_NOCRC_SIGN));
	spin_unlock_bh(&ctx->mtx);
>>>>>>> refs/remotes/origin/master
	return skb_out;

error:
	if (skb != NULL)
		dev_kfree_skb_any(skb);

	return NULL;
}
<<<<<<< HEAD

static int cdc_ncm_rx_fixup(struct usbnet *dev, struct sk_buff *skb_in)
{
	struct sk_buff *skb;
<<<<<<< HEAD
	struct cdc_ncm_ctx *ctx;
	int sumlen;
	int actlen;
	int temp;
	int nframes;
	int x;
	int offset;

	ctx = (struct cdc_ncm_ctx *)dev->data[0];
	if (ctx == NULL)
		goto error;

	actlen = skb_in->len;
	sumlen = CDC_NCM_NTB_MAX_SIZE_RX;

	if (actlen < (sizeof(ctx->rx_ncm.nth16) + sizeof(ctx->rx_ncm.ndp16))) {
=======
	struct cdc_ncm_ctx *ctx = (struct cdc_ncm_ctx *)dev->data[0];
	int len;
	int nframes;
	int x;
	int offset;
	struct usb_cdc_ncm_nth16 *nth16;
	struct usb_cdc_ncm_ndp16 *ndp16;
	struct usb_cdc_ncm_dpe16 *dpe16;
=======
EXPORT_SYMBOL_GPL(cdc_ncm_tx_fixup);

/* verify NTB header and return offset of first NDP, or negative error */
int cdc_ncm_rx_verify_nth16(struct cdc_ncm_ctx *ctx, struct sk_buff *skb_in)
{
	struct usbnet *dev = netdev_priv(skb_in->dev);
	struct usb_cdc_ncm_nth16 *nth16;
	int len;
	int ret = -EINVAL;
>>>>>>> refs/remotes/origin/master

	if (ctx == NULL)
		goto error;

	if (skb_in->len < (sizeof(struct usb_cdc_ncm_nth16) +
					sizeof(struct usb_cdc_ncm_ndp16))) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		pr_debug("frame too short\n");
		goto error;
	}

<<<<<<< HEAD
	memcpy(&(ctx->rx_ncm.nth16), ((u8 *)skb_in->data),
						sizeof(ctx->rx_ncm.nth16));

	if (le32_to_cpu(ctx->rx_ncm.nth16.dwSignature) !=
	    USB_CDC_NCM_NTH16_SIGN) {
		pr_debug("invalid NTH16 signature <%u>\n",
			 le32_to_cpu(ctx->rx_ncm.nth16.dwSignature));
		goto error;
	}

	temp = le16_to_cpu(ctx->rx_ncm.nth16.wBlockLength);
	if (temp > sumlen) {
		pr_debug("unsupported NTB block length %u/%u\n", temp, sumlen);
		goto error;
	}

	temp = le16_to_cpu(ctx->rx_ncm.nth16.wNdpIndex);
	if ((temp + sizeof(ctx->rx_ncm.ndp16)) > actlen) {
		pr_debug("invalid DPT16 index\n");
		goto error;
	}

	memcpy(&(ctx->rx_ncm.ndp16), ((u8 *)skb_in->data) + temp,
						sizeof(ctx->rx_ncm.ndp16));

	if (le32_to_cpu(ctx->rx_ncm.ndp16.dwSignature) !=
	    USB_CDC_NCM_NDP16_NOCRC_SIGN) {
		pr_debug("invalid DPT16 signature <%u>\n",
			 le32_to_cpu(ctx->rx_ncm.ndp16.dwSignature));
		goto error;
	}

	if (le16_to_cpu(ctx->rx_ncm.ndp16.wLength) <
	    USB_CDC_NCM_NDP16_LENGTH_MIN) {
		pr_debug("invalid DPT16 length <%u>\n",
			 le32_to_cpu(ctx->rx_ncm.ndp16.dwSignature));
		goto error;
	}

	nframes = ((le16_to_cpu(ctx->rx_ncm.ndp16.wLength) -
=======
	nth16 = (struct usb_cdc_ncm_nth16 *)skb_in->data;

	if (le32_to_cpu(nth16->dwSignature) != USB_CDC_NCM_NTH16_SIGN) {
		pr_debug("invalid NTH16 signature <%u>\n",
					le32_to_cpu(nth16->dwSignature));
=======
		netif_dbg(dev, rx_err, dev->net, "frame too short\n");
		goto error;
	}

	nth16 = (struct usb_cdc_ncm_nth16 *)skb_in->data;

	if (nth16->dwSignature != cpu_to_le32(USB_CDC_NCM_NTH16_SIGN)) {
		netif_dbg(dev, rx_err, dev->net,
			  "invalid NTH16 signature <%#010x>\n",
			  le32_to_cpu(nth16->dwSignature));
>>>>>>> refs/remotes/origin/master
		goto error;
	}

	len = le16_to_cpu(nth16->wBlockLength);
	if (len > ctx->rx_max) {
<<<<<<< HEAD
		pr_debug("unsupported NTB block length %u/%u\n", len,
								ctx->rx_max);
=======
		netif_dbg(dev, rx_err, dev->net,
			  "unsupported NTB block length %u/%u\n", len,
			  ctx->rx_max);
>>>>>>> refs/remotes/origin/master
		goto error;
	}

	if ((ctx->rx_seq + 1) != le16_to_cpu(nth16->wSequence) &&
<<<<<<< HEAD
		(ctx->rx_seq || le16_to_cpu(nth16->wSequence)) &&
		!((ctx->rx_seq == 0xffff) && !le16_to_cpu(nth16->wSequence))) {
		pr_debug("sequence number glitch prev=%d curr=%d\n",
				ctx->rx_seq, le16_to_cpu(nth16->wSequence));
	}
	ctx->rx_seq = le16_to_cpu(nth16->wSequence);

	len = le16_to_cpu(nth16->wNdpIndex);
	if ((len + sizeof(struct usb_cdc_ncm_ndp16)) > skb_in->len) {
		pr_debug("invalid DPT16 index <%u>\n",
					le16_to_cpu(nth16->wNdpIndex));
		goto error;
	}

	ndp16 = (struct usb_cdc_ncm_ndp16 *)(((u8 *)skb_in->data) + len);

	if (le32_to_cpu(ndp16->dwSignature) != USB_CDC_NCM_NDP16_NOCRC_SIGN) {
		pr_debug("invalid DPT16 signature <%u>\n",
					le32_to_cpu(ndp16->dwSignature));
		goto error;
	}

	if (le16_to_cpu(ndp16->wLength) < USB_CDC_NCM_NDP16_LENGTH_MIN) {
		pr_debug("invalid DPT16 length <%u>\n",
					le32_to_cpu(ndp16->dwSignature));
		goto error;
	}

	nframes = ((le16_to_cpu(ndp16->wLength) -
>>>>>>> refs/remotes/origin/cm-10.0
					sizeof(struct usb_cdc_ncm_ndp16)) /
					sizeof(struct usb_cdc_ncm_dpe16));
	nframes--; /* we process NDP entries except for the last one */

<<<<<<< HEAD
	pr_debug("nframes = %u\n", nframes);

	temp += sizeof(ctx->rx_ncm.ndp16);

	if ((temp + nframes * (sizeof(struct usb_cdc_ncm_dpe16))) > actlen) {
=======
	len += sizeof(struct usb_cdc_ncm_ndp16);

	if ((len + nframes * (sizeof(struct usb_cdc_ncm_dpe16))) >
								skb_in->len) {
>>>>>>> refs/remotes/origin/cm-10.0
		pr_debug("Invalid nframes = %d\n", nframes);
		goto error;
	}

<<<<<<< HEAD
	if (nframes > CDC_NCM_DPT_DATAGRAMS_MAX) {
		pr_debug("Truncating number of frames from %u to %u\n",
					nframes, CDC_NCM_DPT_DATAGRAMS_MAX);
		nframes = CDC_NCM_DPT_DATAGRAMS_MAX;
	}

	memcpy(&(ctx->rx_ncm.dpe16), ((u8 *)skb_in->data) + temp,
				nframes * (sizeof(struct usb_cdc_ncm_dpe16)));

	for (x = 0; x < nframes; x++) {
		offset = le16_to_cpu(ctx->rx_ncm.dpe16[x].wDatagramIndex);
		temp = le16_to_cpu(ctx->rx_ncm.dpe16[x].wDatagramLength);
=======
	dpe16 = (struct usb_cdc_ncm_dpe16 *)(((u8 *)skb_in->data) + len);
=======
	    (ctx->rx_seq || le16_to_cpu(nth16->wSequence)) &&
	    !((ctx->rx_seq == 0xffff) && !le16_to_cpu(nth16->wSequence))) {
		netif_dbg(dev, rx_err, dev->net,
			  "sequence number glitch prev=%d curr=%d\n",
			  ctx->rx_seq, le16_to_cpu(nth16->wSequence));
	}
	ctx->rx_seq = le16_to_cpu(nth16->wSequence);

	ret = le16_to_cpu(nth16->wNdpIndex);
error:
	return ret;
}
EXPORT_SYMBOL_GPL(cdc_ncm_rx_verify_nth16);

/* verify NDP header and return number of datagrams, or negative error */
int cdc_ncm_rx_verify_ndp16(struct sk_buff *skb_in, int ndpoffset)
{
	struct usbnet *dev = netdev_priv(skb_in->dev);
	struct usb_cdc_ncm_ndp16 *ndp16;
	int ret = -EINVAL;

	if ((ndpoffset + sizeof(struct usb_cdc_ncm_ndp16)) > skb_in->len) {
		netif_dbg(dev, rx_err, dev->net, "invalid NDP offset  <%u>\n",
			  ndpoffset);
		goto error;
	}
	ndp16 = (struct usb_cdc_ncm_ndp16 *)(skb_in->data + ndpoffset);

	if (le16_to_cpu(ndp16->wLength) < USB_CDC_NCM_NDP16_LENGTH_MIN) {
		netif_dbg(dev, rx_err, dev->net, "invalid DPT16 length <%u>\n",
			  le16_to_cpu(ndp16->wLength));
		goto error;
	}

	ret = ((le16_to_cpu(ndp16->wLength) -
					sizeof(struct usb_cdc_ncm_ndp16)) /
					sizeof(struct usb_cdc_ncm_dpe16));
	ret--; /* we process NDP entries except for the last one */

	if ((sizeof(struct usb_cdc_ncm_ndp16) +
	     ret * (sizeof(struct usb_cdc_ncm_dpe16))) > skb_in->len) {
		netif_dbg(dev, rx_err, dev->net, "Invalid nframes = %d\n", ret);
		ret = -EINVAL;
	}

error:
	return ret;
}
EXPORT_SYMBOL_GPL(cdc_ncm_rx_verify_ndp16);

int cdc_ncm_rx_fixup(struct usbnet *dev, struct sk_buff *skb_in)
{
	struct sk_buff *skb;
	struct cdc_ncm_ctx *ctx = (struct cdc_ncm_ctx *)dev->data[0];
	int len;
	int nframes;
	int x;
	int offset;
	struct usb_cdc_ncm_ndp16 *ndp16;
	struct usb_cdc_ncm_dpe16 *dpe16;
	int ndpoffset;
	int loopcount = 50; /* arbitrary max preventing infinite loop */

	ndpoffset = cdc_ncm_rx_verify_nth16(ctx, skb_in);
	if (ndpoffset < 0)
		goto error;

next_ndp:
	nframes = cdc_ncm_rx_verify_ndp16(skb_in, ndpoffset);
	if (nframes < 0)
		goto error;

	ndp16 = (struct usb_cdc_ncm_ndp16 *)(skb_in->data + ndpoffset);

	if (ndp16->dwSignature != cpu_to_le32(USB_CDC_NCM_NDP16_NOCRC_SIGN)) {
		netif_dbg(dev, rx_err, dev->net,
			  "invalid DPT16 signature <%#010x>\n",
			  le32_to_cpu(ndp16->dwSignature));
		goto err_ndp;
	}
	dpe16 = ndp16->dpe16;
>>>>>>> refs/remotes/origin/master

	for (x = 0; x < nframes; x++, dpe16++) {
		offset = le16_to_cpu(dpe16->wDatagramIndex);
		len = le16_to_cpu(dpe16->wDatagramLength);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		/*
		 * CDC NCM ch. 3.7
		 * All entries after first NULL entry are to be ignored
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		if ((offset == 0) || (temp == 0)) {
=======
		if ((offset == 0) || (len == 0)) {
>>>>>>> refs/remotes/origin/cm-10.0
			if (!x)
				goto error; /* empty NTB */
=======
		if ((offset == 0) || (len == 0)) {
			if (!x)
				goto err_ndp; /* empty NTB */
>>>>>>> refs/remotes/origin/master
			break;
		}

		/* sanity checking */
<<<<<<< HEAD
<<<<<<< HEAD
		if (((offset + temp) > actlen) ||
		    (temp > CDC_NCM_MAX_DATAGRAM_SIZE) || (temp < ETH_HLEN)) {
			pr_debug("invalid frame detected (ignored)"
					"offset[%u]=%u, length=%u, skb=%p\n",
					x, offset, temp, skb_in);
=======
		if (((offset + len) > skb_in->len) ||
				(len > ctx->rx_max) || (len < ETH_HLEN)) {
			pr_debug("invalid frame detected (ignored)"
					"offset[%u]=%u, length=%u, skb=%p\n",
					x, offset, len, skb_in);
>>>>>>> refs/remotes/origin/cm-10.0
			if (!x)
				goto error;
=======
		if (((offset + len) > skb_in->len) ||
				(len > ctx->rx_max) || (len < ETH_HLEN)) {
			netif_dbg(dev, rx_err, dev->net,
				  "invalid frame detected (ignored) offset[%u]=%u, length=%u, skb=%p\n",
				  x, offset, len, skb_in);
			if (!x)
				goto err_ndp;
>>>>>>> refs/remotes/origin/master
			break;

		} else {
			skb = skb_clone(skb_in, GFP_ATOMIC);
			if (!skb)
				goto error;
<<<<<<< HEAD
<<<<<<< HEAD
			skb->len = temp;
			skb->data = ((u8 *)skb_in->data) + offset;
			skb_set_tail_pointer(skb, temp);
=======
			skb->len = len;
			skb->data = ((u8 *)skb_in->data) + offset;
			skb_set_tail_pointer(skb, len);
>>>>>>> refs/remotes/origin/cm-10.0
			usbnet_skb_return(dev, skb);
		}
	}
=======
			skb->len = len;
			skb->data = ((u8 *)skb_in->data) + offset;
			skb_set_tail_pointer(skb, len);
			usbnet_skb_return(dev, skb);
		}
	}
err_ndp:
	/* are there more NDPs to process? */
	ndpoffset = le16_to_cpu(ndp16->wNextNdpIndex);
	if (ndpoffset && loopcount--)
		goto next_ndp;

>>>>>>> refs/remotes/origin/master
	return 1;
error:
	return 0;
}
<<<<<<< HEAD

static void
cdc_ncm_speed_change(struct cdc_ncm_ctx *ctx,
=======
EXPORT_SYMBOL_GPL(cdc_ncm_rx_fixup);

static void
cdc_ncm_speed_change(struct usbnet *dev,
>>>>>>> refs/remotes/origin/master
		     struct usb_cdc_speed_change *data)
{
	uint32_t rx_speed = le32_to_cpu(data->DLBitRRate);
	uint32_t tx_speed = le32_to_cpu(data->ULBitRate);

	/*
	 * Currently the USB-NET API does not support reporting the actual
	 * device speed. Do print it instead.
	 */
<<<<<<< HEAD
	if ((tx_speed != ctx->tx_speed) || (rx_speed != ctx->rx_speed)) {
		ctx->tx_speed = tx_speed;
		ctx->rx_speed = rx_speed;

		if ((tx_speed > 1000000) && (rx_speed > 1000000)) {
			printk(KERN_INFO KBUILD_MODNAME
				": %s: %u mbit/s downlink "
				"%u mbit/s uplink\n",
				ctx->netdev->name,
				(unsigned int)(rx_speed / 1000000U),
				(unsigned int)(tx_speed / 1000000U));
		} else {
			printk(KERN_INFO KBUILD_MODNAME
				": %s: %u kbit/s downlink "
				"%u kbit/s uplink\n",
				ctx->netdev->name,
				(unsigned int)(rx_speed / 1000U),
				(unsigned int)(tx_speed / 1000U));
		}
=======
	if ((tx_speed > 1000000) && (rx_speed > 1000000)) {
		netif_info(dev, link, dev->net,
		       "%u mbit/s downlink %u mbit/s uplink\n",
		       (unsigned int)(rx_speed / 1000000U),
		       (unsigned int)(tx_speed / 1000000U));
	} else {
		netif_info(dev, link, dev->net,
		       "%u kbit/s downlink %u kbit/s uplink\n",
		       (unsigned int)(rx_speed / 1000U),
		       (unsigned int)(tx_speed / 1000U));
>>>>>>> refs/remotes/origin/master
	}
}

static void cdc_ncm_status(struct usbnet *dev, struct urb *urb)
{
	struct cdc_ncm_ctx *ctx;
	struct usb_cdc_notification *event;

	ctx = (struct cdc_ncm_ctx *)dev->data[0];

	if (urb->actual_length < sizeof(*event))
		return;

	/* test for split data in 8-byte chunks */
	if (test_and_clear_bit(EVENT_STS_SPLIT, &dev->flags)) {
<<<<<<< HEAD
		cdc_ncm_speed_change(ctx,
=======
		cdc_ncm_speed_change(dev,
>>>>>>> refs/remotes/origin/master
		      (struct usb_cdc_speed_change *)urb->transfer_buffer);
		return;
	}

	event = urb->transfer_buffer;

	switch (event->bNotificationType) {
	case USB_CDC_NOTIFY_NETWORK_CONNECTION:
		/*
		 * According to the CDC NCM specification ch.7.1
		 * USB_CDC_NOTIFY_NETWORK_CONNECTION notification shall be
		 * sent by device after USB_CDC_NOTIFY_SPEED_CHANGE.
		 */
<<<<<<< HEAD
		ctx->connected = event->wValue;

		printk(KERN_INFO KBUILD_MODNAME ": %s: network connection:"
			" %sconnected\n",
			ctx->netdev->name, ctx->connected ? "" : "dis");

		if (ctx->connected)
			netif_carrier_on(dev->net);
		else {
			netif_carrier_off(dev->net);
			ctx->tx_speed = ctx->rx_speed = 0;
		}
=======
		ctx->connected = le16_to_cpu(event->wValue);
		netif_info(dev, link, dev->net,
			   "network connection: %sconnected\n",
			   ctx->connected ? "" : "dis");
		usbnet_link_change(dev, ctx->connected, 0);
>>>>>>> refs/remotes/origin/master
		break;

	case USB_CDC_NOTIFY_SPEED_CHANGE:
		if (urb->actual_length < (sizeof(*event) +
					sizeof(struct usb_cdc_speed_change)))
			set_bit(EVENT_STS_SPLIT, &dev->flags);
		else
<<<<<<< HEAD
			cdc_ncm_speed_change(ctx,
				(struct usb_cdc_speed_change *) &event[1]);
		break;

	default:
		dev_err(&dev->udev->dev, "NCM: unexpected "
			"notification 0x%02x!\n", event->bNotificationType);
=======
			cdc_ncm_speed_change(dev,
					     (struct usb_cdc_speed_change *)&event[1]);
		break;

	default:
		dev_dbg(&dev->udev->dev,
			"NCM: unexpected notification 0x%02x!\n",
			event->bNotificationType);
>>>>>>> refs/remotes/origin/master
		break;
	}
}

static int cdc_ncm_check_connect(struct usbnet *dev)
{
	struct cdc_ncm_ctx *ctx;

	ctx = (struct cdc_ncm_ctx *)dev->data[0];
	if (ctx == NULL)
		return 1;	/* disconnected */

	return !ctx->connected;
}

<<<<<<< HEAD
static int
cdc_ncm_probe(struct usb_interface *udev, const struct usb_device_id *prod)
{
	return usbnet_probe(udev, prod);
}

static void cdc_ncm_disconnect(struct usb_interface *intf)
{
	struct usbnet *dev = usb_get_intfdata(intf);

	if (dev == NULL)
		return;		/* already disconnected */

	usbnet_disconnect(intf);
}

static int cdc_ncm_manage_power(struct usbnet *dev, int status)
{
	dev->intf->needs_remote_wakeup = status;
	return 0;
}

=======
>>>>>>> refs/remotes/origin/master
static const struct driver_info cdc_ncm_info = {
	.description = "CDC NCM",
	.flags = FLAG_POINTTOPOINT | FLAG_NO_SETINT | FLAG_MULTI_PACKET,
	.bind = cdc_ncm_bind,
	.unbind = cdc_ncm_unbind,
	.check_connect = cdc_ncm_check_connect,
<<<<<<< HEAD
	.manage_power = cdc_ncm_manage_power,
=======
	.manage_power = usbnet_manage_power,
>>>>>>> refs/remotes/origin/master
	.status = cdc_ncm_status,
	.rx_fixup = cdc_ncm_rx_fixup,
	.tx_fixup = cdc_ncm_tx_fixup,
};

<<<<<<< HEAD
static struct usb_driver cdc_ncm_driver = {
	.name = "cdc_ncm",
	.id_table = cdc_devs,
	.probe = cdc_ncm_probe,
	.disconnect = cdc_ncm_disconnect,
=======
/* Same as cdc_ncm_info, but with FLAG_WWAN */
static const struct driver_info wwan_info = {
	.description = "Mobile Broadband Network Device",
	.flags = FLAG_POINTTOPOINT | FLAG_NO_SETINT | FLAG_MULTI_PACKET
			| FLAG_WWAN,
	.bind = cdc_ncm_bind,
	.unbind = cdc_ncm_unbind,
	.check_connect = cdc_ncm_check_connect,
	.manage_power = usbnet_manage_power,
	.status = cdc_ncm_status,
	.rx_fixup = cdc_ncm_rx_fixup,
	.tx_fixup = cdc_ncm_tx_fixup,
};

/* Same as wwan_info, but with FLAG_NOARP  */
static const struct driver_info wwan_noarp_info = {
	.description = "Mobile Broadband Network Device (NO ARP)",
	.flags = FLAG_POINTTOPOINT | FLAG_NO_SETINT | FLAG_MULTI_PACKET
			| FLAG_WWAN | FLAG_NOARP,
	.bind = cdc_ncm_bind,
	.unbind = cdc_ncm_unbind,
	.check_connect = cdc_ncm_check_connect,
	.manage_power = usbnet_manage_power,
	.status = cdc_ncm_status,
	.rx_fixup = cdc_ncm_rx_fixup,
	.tx_fixup = cdc_ncm_tx_fixup,
};

static const struct usb_device_id cdc_devs[] = {
	/* Ericsson MBM devices like F5521gw */
	{ .match_flags = USB_DEVICE_ID_MATCH_INT_INFO
		| USB_DEVICE_ID_MATCH_VENDOR,
	  .idVendor = 0x0bdb,
	  .bInterfaceClass = USB_CLASS_COMM,
	  .bInterfaceSubClass = USB_CDC_SUBCLASS_NCM,
	  .bInterfaceProtocol = USB_CDC_PROTO_NONE,
	  .driver_info = (unsigned long) &wwan_info,
	},

	/* Dell branded MBM devices like DW5550 */
	{ .match_flags = USB_DEVICE_ID_MATCH_INT_INFO
		| USB_DEVICE_ID_MATCH_VENDOR,
	  .idVendor = 0x413c,
	  .bInterfaceClass = USB_CLASS_COMM,
	  .bInterfaceSubClass = USB_CDC_SUBCLASS_NCM,
	  .bInterfaceProtocol = USB_CDC_PROTO_NONE,
	  .driver_info = (unsigned long) &wwan_info,
	},

	/* Toshiba branded MBM devices */
	{ .match_flags = USB_DEVICE_ID_MATCH_INT_INFO
		| USB_DEVICE_ID_MATCH_VENDOR,
	  .idVendor = 0x0930,
	  .bInterfaceClass = USB_CLASS_COMM,
	  .bInterfaceSubClass = USB_CDC_SUBCLASS_NCM,
	  .bInterfaceProtocol = USB_CDC_PROTO_NONE,
	  .driver_info = (unsigned long) &wwan_info,
	},

	/* tag Huawei devices as wwan */
	{ USB_VENDOR_AND_INTERFACE_INFO(0x12d1,
					USB_CLASS_COMM,
					USB_CDC_SUBCLASS_NCM,
					USB_CDC_PROTO_NONE),
	  .driver_info = (unsigned long)&wwan_info,
	},

	/* Infineon(now Intel) HSPA Modem platform */
	{ USB_DEVICE_AND_INTERFACE_INFO(0x1519, 0x0443,
		USB_CLASS_COMM,
		USB_CDC_SUBCLASS_NCM, USB_CDC_PROTO_NONE),
	  .driver_info = (unsigned long)&wwan_noarp_info,
	},

	/* Generic CDC-NCM devices */
	{ USB_INTERFACE_INFO(USB_CLASS_COMM,
		USB_CDC_SUBCLASS_NCM, USB_CDC_PROTO_NONE),
		.driver_info = (unsigned long)&cdc_ncm_info,
	},
	{
	},
};
MODULE_DEVICE_TABLE(usb, cdc_devs);

static struct usb_driver cdc_ncm_driver = {
	.name = "cdc_ncm",
	.id_table = cdc_devs,
	.probe = usbnet_probe,
	.disconnect = usbnet_disconnect,
>>>>>>> refs/remotes/origin/master
	.suspend = usbnet_suspend,
	.resume = usbnet_resume,
	.reset_resume =	usbnet_resume,
	.supports_autosuspend = 1,
<<<<<<< HEAD
};

<<<<<<< HEAD
static struct ethtool_ops cdc_ncm_ethtool_ops = {
=======
static const struct ethtool_ops cdc_ncm_ethtool_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
	.get_drvinfo = cdc_ncm_get_drvinfo,
	.get_link = usbnet_get_link,
	.get_msglevel = usbnet_get_msglevel,
	.set_msglevel = usbnet_set_msglevel,
	.get_settings = usbnet_get_settings,
	.set_settings = usbnet_set_settings,
	.nway_reset = usbnet_nway_reset,
};

<<<<<<< HEAD
static int __init cdc_ncm_init(void)
{
	printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_VERSION "\n");
	return usb_register(&cdc_ncm_driver);
}

module_init(cdc_ncm_init);

static void __exit cdc_ncm_exit(void)
{
	usb_deregister(&cdc_ncm_driver);
}

module_exit(cdc_ncm_exit);
=======
module_usb_driver(cdc_ncm_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.disable_hub_initiated_lpm = 1,
};

module_usb_driver(cdc_ncm_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Hans Petter Selasky");
MODULE_DESCRIPTION("USB CDC NCM host driver");
MODULE_LICENSE("Dual BSD/GPL");
