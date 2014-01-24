/*
 * drivers/usb/musb/ux500_dma.c
 *
<<<<<<< HEAD
 * U8500 and U5500 DMA support code
=======
 * U8500 DMA support code
>>>>>>> refs/remotes/origin/master
 *
 * Copyright (C) 2009 STMicroelectronics
 * Copyright (C) 2011 ST-Ericsson SA
 * Authors:
 *	Mian Yousaf Kaukab <mian.yousaf.kaukab@stericsson.com>
 *	Praveena Nadahally <praveen.nadahally@stericsson.com>
 *	Rajaram Regupathy <ragupathy.rajaram@stericsson.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/pfn.h>
<<<<<<< HEAD
#include <mach/usb.h>
#include "musb_core.h"

=======
#include <linux/sizes.h>
#include <linux/platform_data/usb-musb-ux500.h>
#include "musb_core.h"

static const char *iep_chan_names[] = { "iep_1_9", "iep_2_10", "iep_3_11", "iep_4_12",
					"iep_5_13", "iep_6_14", "iep_7_15", "iep_8" };
static const char *oep_chan_names[] = { "oep_1_9", "oep_2_10", "oep_3_11", "oep_4_12",
					"oep_5_13", "oep_6_14", "oep_7_15", "oep_8" };

>>>>>>> refs/remotes/origin/master
struct ux500_dma_channel {
	struct dma_channel channel;
	struct ux500_dma_controller *controller;
	struct musb_hw_ep *hw_ep;
<<<<<<< HEAD
<<<<<<< HEAD
	struct work_struct channel_work;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct dma_chan *dma_chan;
	unsigned int cur_len;
	dma_cookie_t cookie;
	u8 ch_num;
	u8 is_tx;
	u8 is_allocated;
};

struct ux500_dma_controller {
	struct dma_controller controller;
<<<<<<< HEAD
	struct ux500_dma_channel rx_channel[UX500_MUSB_DMA_NUM_RX_CHANNELS];
	struct ux500_dma_channel tx_channel[UX500_MUSB_DMA_NUM_TX_CHANNELS];
	u32	num_rx_channels;
	u32	num_tx_channels;
=======
	struct ux500_dma_channel rx_channel[UX500_MUSB_DMA_NUM_RX_TX_CHANNELS];
	struct ux500_dma_channel tx_channel[UX500_MUSB_DMA_NUM_RX_TX_CHANNELS];
>>>>>>> refs/remotes/origin/master
	void *private_data;
	dma_addr_t phy_base;
};

<<<<<<< HEAD
<<<<<<< HEAD
/* Work function invoked from DMA callback to handle tx transfers. */
static void ux500_tx_work(struct work_struct *data)
{
	struct ux500_dma_channel *ux500_channel = container_of(data,
		struct ux500_dma_channel, channel_work);
	struct musb_hw_ep       *hw_ep = ux500_channel->hw_ep;
	struct musb *musb = hw_ep->musb;
	unsigned long flags;

	DBG(4, "DMA tx transfer done on hw_ep=%d\n", hw_ep->epnum);

	spin_lock_irqsave(&musb->lock, flags);
	ux500_channel->channel.actual_len = ux500_channel->cur_len;
	ux500_channel->channel.status = MUSB_DMA_STATUS_FREE;
	musb_dma_completion(musb, hw_ep->epnum,
				ux500_channel->is_tx);
	spin_unlock_irqrestore(&musb->lock, flags);
}

/* Work function invoked from DMA callback to handle rx transfers. */
static void ux500_rx_work(struct work_struct *data)
{
	struct ux500_dma_channel *ux500_channel = container_of(data,
		struct ux500_dma_channel, channel_work);
=======
/* Work function invoked from DMA callback to handle rx transfers. */
void ux500_dma_callback(void *private_data)
{
	struct dma_channel *channel = private_data;
	struct ux500_dma_channel *ux500_channel = channel->private_data;
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Work function invoked from DMA callback to handle rx transfers. */
static void ux500_dma_callback(void *private_data)
{
	struct dma_channel *channel = private_data;
	struct ux500_dma_channel *ux500_channel = channel->private_data;
>>>>>>> refs/remotes/origin/master
	struct musb_hw_ep       *hw_ep = ux500_channel->hw_ep;
	struct musb *musb = hw_ep->musb;
	unsigned long flags;

<<<<<<< HEAD
<<<<<<< HEAD
	DBG(4, "DMA rx transfer done on hw_ep=%d\n", hw_ep->epnum);
=======
	dev_dbg(musb->controller, "DMA rx transfer done on hw_ep=%d\n",
		hw_ep->epnum);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev_dbg(musb->controller, "DMA rx transfer done on hw_ep=%d\n",
		hw_ep->epnum);
>>>>>>> refs/remotes/origin/master

	spin_lock_irqsave(&musb->lock, flags);
	ux500_channel->channel.actual_len = ux500_channel->cur_len;
	ux500_channel->channel.status = MUSB_DMA_STATUS_FREE;
<<<<<<< HEAD
	musb_dma_completion(musb, hw_ep->epnum,
		ux500_channel->is_tx);
	spin_unlock_irqrestore(&musb->lock, flags);
<<<<<<< HEAD
}

void ux500_dma_callback(void *private_data)
{
	struct dma_channel *channel = (struct dma_channel *)private_data;
	struct ux500_dma_channel *ux500_channel = channel->private_data;

	schedule_work(&ux500_channel->channel_work);
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======
	musb_dma_completion(musb, hw_ep->epnum, ux500_channel->is_tx);
	spin_unlock_irqrestore(&musb->lock, flags);

>>>>>>> refs/remotes/origin/master
}

static bool ux500_configure_channel(struct dma_channel *channel,
				u16 packet_sz, u8 mode,
				dma_addr_t dma_addr, u32 len)
{
	struct ux500_dma_channel *ux500_channel = channel->private_data;
	struct musb_hw_ep *hw_ep = ux500_channel->hw_ep;
	struct dma_chan *dma_chan = ux500_channel->dma_chan;
	struct dma_async_tx_descriptor *dma_desc;
<<<<<<< HEAD
<<<<<<< HEAD
	enum dma_data_direction direction;
=======
	enum dma_transfer_direction direction;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	enum dma_transfer_direction direction;
>>>>>>> refs/remotes/origin/master
	struct scatterlist sg;
	struct dma_slave_config slave_conf;
	enum dma_slave_buswidth addr_width;
	dma_addr_t usb_fifo_addr = (MUSB_FIFO_OFFSET(hw_ep->epnum) +
					ux500_channel->controller->phy_base);
<<<<<<< HEAD
<<<<<<< HEAD

	DBG(4, "packet_sz=%d, mode=%d, dma_addr=0x%x, len=%d is_tx=%d\n",
			packet_sz, mode, dma_addr, len, ux500_channel->is_tx);
=======
	struct musb *musb = ux500_channel->controller->private_data;

	dev_dbg(musb->controller,
		"packet_sz=%d, mode=%d, dma_addr=0x%x, len=%d is_tx=%d\n",
		packet_sz, mode, dma_addr, len, ux500_channel->is_tx);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct musb *musb = ux500_channel->controller->private_data;

	dev_dbg(musb->controller,
		"packet_sz=%d, mode=%d, dma_addr=0x%llu, len=%d is_tx=%d\n",
		packet_sz, mode, (unsigned long long) dma_addr,
		len, ux500_channel->is_tx);
>>>>>>> refs/remotes/origin/master

	ux500_channel->cur_len = len;

	sg_init_table(&sg, 1);
	sg_set_page(&sg, pfn_to_page(PFN_DOWN(dma_addr)), len,
					    offset_in_page(dma_addr));
	sg_dma_address(&sg) = dma_addr;
	sg_dma_len(&sg) = len;

<<<<<<< HEAD
<<<<<<< HEAD
	direction = ux500_channel->is_tx ? DMA_TO_DEVICE : DMA_FROM_DEVICE;
=======
	direction = ux500_channel->is_tx ? DMA_MEM_TO_DEV : DMA_DEV_TO_MEM;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	direction = ux500_channel->is_tx ? DMA_MEM_TO_DEV : DMA_DEV_TO_MEM;
>>>>>>> refs/remotes/origin/master
	addr_width = (len & 0x3) ? DMA_SLAVE_BUSWIDTH_1_BYTE :
					DMA_SLAVE_BUSWIDTH_4_BYTES;

	slave_conf.direction = direction;
<<<<<<< HEAD
<<<<<<< HEAD
	if (direction == DMA_FROM_DEVICE) {
		slave_conf.src_addr = usb_fifo_addr;
		slave_conf.src_addr_width = addr_width;
		slave_conf.src_maxburst = 16;
	} else {
		slave_conf.dst_addr = usb_fifo_addr;
		slave_conf.dst_addr_width = addr_width;
		slave_conf.dst_maxburst = 16;
	}
	dma_chan->device->device_control(dma_chan, DMA_SLAVE_CONFIG,
					     (unsigned long) &slave_conf);

	dma_desc = dma_chan->device->
			device_prep_slave_sg(dma_chan, &sg, 1, direction,
=======
=======
>>>>>>> refs/remotes/origin/master
	slave_conf.src_addr = usb_fifo_addr;
	slave_conf.src_addr_width = addr_width;
	slave_conf.src_maxburst = 16;
	slave_conf.dst_addr = usb_fifo_addr;
	slave_conf.dst_addr_width = addr_width;
	slave_conf.dst_maxburst = 16;
	slave_conf.device_fc = false;

	dma_chan->device->device_control(dma_chan, DMA_SLAVE_CONFIG,
					     (unsigned long) &slave_conf);

	dma_desc = dmaengine_prep_slave_sg(dma_chan, &sg, 1, direction,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
					     DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	if (!dma_desc)
		return false;

	dma_desc->callback = ux500_dma_callback;
	dma_desc->callback_param = channel;
	ux500_channel->cookie = dma_desc->tx_submit(dma_desc);

	dma_async_issue_pending(dma_chan);

	return true;
}

static struct dma_channel *ux500_dma_channel_allocate(struct dma_controller *c,
				struct musb_hw_ep *hw_ep, u8 is_tx)
{
	struct ux500_dma_controller *controller = container_of(c,
			struct ux500_dma_controller, controller);
	struct ux500_dma_channel *ux500_channel = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct musb *musb = controller->private_data;
>>>>>>> refs/remotes/origin/cm-10.0
	u8 ch_num = hw_ep->epnum - 1;
	u32 max_ch;

	/* Max 8 DMA channels (0 - 7). Each DMA channel can only be allocated
=======
	struct musb *musb = controller->private_data;
	u8 ch_num = hw_ep->epnum - 1;

	/* 8 DMA channels (0 - 7). Each DMA channel can only be allocated
>>>>>>> refs/remotes/origin/master
	 * to specified hw_ep. For example DMA channel 0 can only be allocated
	 * to hw_ep 1 and 9.
	 */
	if (ch_num > 7)
		ch_num -= 8;

<<<<<<< HEAD
	max_ch = is_tx ? controller->num_tx_channels :
			controller->num_rx_channels;

	if (ch_num >= max_ch)
=======
	if (ch_num >= UX500_MUSB_DMA_NUM_RX_TX_CHANNELS)
>>>>>>> refs/remotes/origin/master
		return NULL;

	ux500_channel = is_tx ? &(controller->tx_channel[ch_num]) :
				&(controller->rx_channel[ch_num]) ;

	/* Check if channel is already used. */
	if (ux500_channel->is_allocated)
		return NULL;

	ux500_channel->hw_ep = hw_ep;
	ux500_channel->is_allocated = 1;

<<<<<<< HEAD
<<<<<<< HEAD
	DBG(7, "hw_ep=%d, is_tx=0x%x, channel=%d\n",
=======
	dev_dbg(musb->controller, "hw_ep=%d, is_tx=0x%x, channel=%d\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev_dbg(musb->controller, "hw_ep=%d, is_tx=0x%x, channel=%d\n",
>>>>>>> refs/remotes/origin/master
		hw_ep->epnum, is_tx, ch_num);

	return &(ux500_channel->channel);
}

static void ux500_dma_channel_release(struct dma_channel *channel)
{
	struct ux500_dma_channel *ux500_channel = channel->private_data;
<<<<<<< HEAD
<<<<<<< HEAD

	DBG(7, "channel=%d\n", ux500_channel->ch_num);
=======
	struct musb *musb = ux500_channel->controller->private_data;

	dev_dbg(musb->controller, "channel=%d\n", ux500_channel->ch_num);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct musb *musb = ux500_channel->controller->private_data;

	dev_dbg(musb->controller, "channel=%d\n", ux500_channel->ch_num);
>>>>>>> refs/remotes/origin/master

	if (ux500_channel->is_allocated) {
		ux500_channel->is_allocated = 0;
		channel->status = MUSB_DMA_STATUS_FREE;
		channel->actual_len = 0;
	}
}

static int ux500_dma_is_compatible(struct dma_channel *channel,
		u16 maxpacket, void *buf, u32 length)
{
	if ((maxpacket & 0x3)		||
<<<<<<< HEAD
		((int)buf & 0x3)	||
=======
		((unsigned long int) buf & 0x3)	||
>>>>>>> refs/remotes/origin/master
		(length < 512)		||
		(length & 0x3))
		return false;
	else
		return true;
}

static int ux500_dma_channel_program(struct dma_channel *channel,
				u16 packet_sz, u8 mode,
				dma_addr_t dma_addr, u32 len)
{
	int ret;

	BUG_ON(channel->status == MUSB_DMA_STATUS_UNKNOWN ||
		channel->status == MUSB_DMA_STATUS_BUSY);

	if (!ux500_dma_is_compatible(channel, packet_sz, (void *)dma_addr, len))
		return false;

	channel->status = MUSB_DMA_STATUS_BUSY;
	channel->actual_len = 0;
	ret = ux500_configure_channel(channel, packet_sz, mode, dma_addr, len);
	if (!ret)
		channel->status = MUSB_DMA_STATUS_FREE;

	return ret;
}

static int ux500_dma_channel_abort(struct dma_channel *channel)
{
	struct ux500_dma_channel *ux500_channel = channel->private_data;
	struct ux500_dma_controller *controller = ux500_channel->controller;
	struct musb *musb = controller->private_data;
	void __iomem *epio = musb->endpoints[ux500_channel->hw_ep->epnum].regs;
	u16 csr;

<<<<<<< HEAD
<<<<<<< HEAD
	DBG(4, "channel=%d, is_tx=%d\n", ux500_channel->ch_num,
						ux500_channel->is_tx);
=======
	dev_dbg(musb->controller, "channel=%d, is_tx=%d\n",
		ux500_channel->ch_num, ux500_channel->is_tx);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev_dbg(musb->controller, "channel=%d, is_tx=%d\n",
		ux500_channel->ch_num, ux500_channel->is_tx);
>>>>>>> refs/remotes/origin/master

	if (channel->status == MUSB_DMA_STATUS_BUSY) {
		if (ux500_channel->is_tx) {
			csr = musb_readw(epio, MUSB_TXCSR);
			csr &= ~(MUSB_TXCSR_AUTOSET |
				 MUSB_TXCSR_DMAENAB |
				 MUSB_TXCSR_DMAMODE);
			musb_writew(epio, MUSB_TXCSR, csr);
		} else {
			csr = musb_readw(epio, MUSB_RXCSR);
			csr &= ~(MUSB_RXCSR_AUTOCLEAR |
				 MUSB_RXCSR_DMAENAB |
				 MUSB_RXCSR_DMAMODE);
			musb_writew(epio, MUSB_RXCSR, csr);
		}

		ux500_channel->dma_chan->device->
				device_control(ux500_channel->dma_chan,
					DMA_TERMINATE_ALL, 0);
		channel->status = MUSB_DMA_STATUS_FREE;
	}
	return 0;
}

<<<<<<< HEAD
static int ux500_dma_controller_stop(struct dma_controller *c)
{
	struct ux500_dma_controller *controller = container_of(c,
			struct ux500_dma_controller, controller);
=======
static void ux500_dma_controller_stop(struct ux500_dma_controller *controller)
{
>>>>>>> refs/remotes/origin/master
	struct ux500_dma_channel *ux500_channel;
	struct dma_channel *channel;
	u8 ch_num;

<<<<<<< HEAD
	for (ch_num = 0; ch_num < controller->num_rx_channels; ch_num++) {
=======
	for (ch_num = 0; ch_num < UX500_MUSB_DMA_NUM_RX_TX_CHANNELS; ch_num++) {
>>>>>>> refs/remotes/origin/master
		channel = &controller->rx_channel[ch_num].channel;
		ux500_channel = channel->private_data;

		ux500_dma_channel_release(channel);

		if (ux500_channel->dma_chan)
			dma_release_channel(ux500_channel->dma_chan);
	}

<<<<<<< HEAD
	for (ch_num = 0; ch_num < controller->num_tx_channels; ch_num++) {
=======
	for (ch_num = 0; ch_num < UX500_MUSB_DMA_NUM_RX_TX_CHANNELS; ch_num++) {
>>>>>>> refs/remotes/origin/master
		channel = &controller->tx_channel[ch_num].channel;
		ux500_channel = channel->private_data;

		ux500_dma_channel_release(channel);

		if (ux500_channel->dma_chan)
			dma_release_channel(ux500_channel->dma_chan);
	}
<<<<<<< HEAD

	return 0;
}

static int ux500_dma_controller_start(struct dma_controller *c)
{
	struct ux500_dma_controller *controller = container_of(c,
			struct ux500_dma_controller, controller);
	struct ux500_dma_channel *ux500_channel = NULL;
	struct musb *musb = controller->private_data;
	struct device *dev = musb->controller;
	struct musb_hdrc_platform_data *plat = dev->platform_data;
	struct ux500_musb_board_data *data = plat->board_data;
	struct dma_channel *dma_channel = NULL;
=======
}

static int ux500_dma_controller_start(struct ux500_dma_controller *controller)
{
	struct ux500_dma_channel *ux500_channel = NULL;
	struct musb *musb = controller->private_data;
	struct device *dev = musb->controller;
	struct musb_hdrc_platform_data *plat = dev_get_platdata(dev);
	struct ux500_musb_board_data *data;
	struct dma_channel *dma_channel = NULL;
	char **chan_names;
>>>>>>> refs/remotes/origin/master
	u32 ch_num;
	u8 dir;
	u8 is_tx = 0;

	void **param_array;
	struct ux500_dma_channel *channel_array;
<<<<<<< HEAD
	u32 ch_count;
<<<<<<< HEAD
	void (*musb_channel_work)(struct work_struct *);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	dma_cap_mask_t mask;

	if ((data->num_rx_channels > UX500_MUSB_DMA_NUM_RX_CHANNELS) ||
		(data->num_tx_channels > UX500_MUSB_DMA_NUM_TX_CHANNELS))
		return -EINVAL;

	controller->num_rx_channels = data->num_rx_channels;
	controller->num_tx_channels = data->num_tx_channels;
=======
	dma_cap_mask_t mask;

	if (!plat) {
		dev_err(musb->controller, "No platform data\n");
		return -EINVAL;
	}

	data = plat->board_data;
>>>>>>> refs/remotes/origin/master

	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

	/* Prepare the loop for RX channels */
	channel_array = controller->rx_channel;
<<<<<<< HEAD
	ch_count = data->num_rx_channels;
	param_array = data->dma_rx_param_array;
<<<<<<< HEAD
	musb_channel_work = ux500_rx_work;
=======
>>>>>>> refs/remotes/origin/cm-10.0

	for (dir = 0; dir < 2; dir++) {
		for (ch_num = 0; ch_num < ch_count; ch_num++) {
=======
	param_array = data ? data->dma_rx_param_array : NULL;
	chan_names = (char **)iep_chan_names;

	for (dir = 0; dir < 2; dir++) {
		for (ch_num = 0;
		     ch_num < UX500_MUSB_DMA_NUM_RX_TX_CHANNELS;
		     ch_num++) {
>>>>>>> refs/remotes/origin/master
			ux500_channel = &channel_array[ch_num];
			ux500_channel->controller = controller;
			ux500_channel->ch_num = ch_num;
			ux500_channel->is_tx = is_tx;

			dma_channel = &(ux500_channel->channel);
			dma_channel->private_data = ux500_channel;
			dma_channel->status = MUSB_DMA_STATUS_FREE;
			dma_channel->max_len = SZ_16M;

<<<<<<< HEAD
			ux500_channel->dma_chan = dma_request_channel(mask,
							data->dma_filter,
							param_array[ch_num]);
=======
			ux500_channel->dma_chan =
				dma_request_slave_channel(dev, chan_names[ch_num]);

			if (!ux500_channel->dma_chan)
				ux500_channel->dma_chan =
					dma_request_channel(mask,
							    data ?
							    data->dma_filter :
							    NULL,
							    param_array ?
							    param_array[ch_num] :
							    NULL);

>>>>>>> refs/remotes/origin/master
			if (!ux500_channel->dma_chan) {
				ERR("Dma pipe allocation error dir=%d ch=%d\n",
					dir, ch_num);

				/* Release already allocated channels */
<<<<<<< HEAD
				ux500_dma_controller_stop(c);
=======
				ux500_dma_controller_stop(controller);
>>>>>>> refs/remotes/origin/master

				return -EBUSY;
			}

<<<<<<< HEAD
<<<<<<< HEAD
			INIT_WORK(&ux500_channel->channel_work,
				musb_channel_work);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}

		/* Prepare the loop for TX channels */
		channel_array = controller->tx_channel;
<<<<<<< HEAD
		ch_count = data->num_tx_channels;
		param_array = data->dma_tx_param_array;
<<<<<<< HEAD
		musb_channel_work = ux500_tx_work;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
		param_array = data ? data->dma_tx_param_array : NULL;
		chan_names = (char **)oep_chan_names;
>>>>>>> refs/remotes/origin/master
		is_tx = 1;
	}

	return 0;
}

void dma_controller_destroy(struct dma_controller *c)
{
	struct ux500_dma_controller *controller = container_of(c,
			struct ux500_dma_controller, controller);

<<<<<<< HEAD
	kfree(controller);
}

struct dma_controller *__init
dma_controller_create(struct musb *musb, void __iomem *base)
=======
	ux500_dma_controller_stop(controller);
	kfree(controller);
}

struct dma_controller *dma_controller_create(struct musb *musb,
					void __iomem *base)
>>>>>>> refs/remotes/origin/master
{
	struct ux500_dma_controller *controller;
	struct platform_device *pdev = to_platform_device(musb->controller);
	struct resource	*iomem;
<<<<<<< HEAD

	controller = kzalloc(sizeof(*controller), GFP_KERNEL);
	if (!controller)
		return NULL;
=======
	int ret;

	controller = kzalloc(sizeof(*controller), GFP_KERNEL);
	if (!controller)
		goto kzalloc_fail;
>>>>>>> refs/remotes/origin/master

	controller->private_data = musb;

	/* Save physical address for DMA controller. */
	iomem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
<<<<<<< HEAD
	controller->phy_base = (dma_addr_t) iomem->start;

	controller->controller.start = ux500_dma_controller_start;
	controller->controller.stop = ux500_dma_controller_stop;
=======
	if (!iomem) {
		dev_err(musb->controller, "no memory resource defined\n");
		goto plat_get_fail;
	}

	controller->phy_base = (dma_addr_t) iomem->start;

>>>>>>> refs/remotes/origin/master
	controller->controller.channel_alloc = ux500_dma_channel_allocate;
	controller->controller.channel_release = ux500_dma_channel_release;
	controller->controller.channel_program = ux500_dma_channel_program;
	controller->controller.channel_abort = ux500_dma_channel_abort;
	controller->controller.is_compatible = ux500_dma_is_compatible;

<<<<<<< HEAD
	return &controller->controller;
=======
	ret = ux500_dma_controller_start(controller);
	if (ret)
		goto plat_get_fail;
	return &controller->controller;

plat_get_fail:
	kfree(controller);
kzalloc_fail:
	return NULL;
>>>>>>> refs/remotes/origin/master
}
