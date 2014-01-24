/*
 * Cryptographic API.
 *
 * Support for OMAP AES HW acceleration.
 *
 * Copyright (c) 2010 Nokia Corporation
 * Author: Dmitry Kasatkin <dmitry.kasatkin@nokia.com>
<<<<<<< HEAD
=======
 * Copyright (c) 2011 Texas Instruments Incorporated
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 */

<<<<<<< HEAD
#define pr_fmt(fmt) "%s: " fmt, __func__
=======
#define pr_fmt(fmt) "%20s: " fmt, __func__
#define prn(num) pr_debug(#num "=%d\n", num)
#define prx(num) pr_debug(#num "=%x\n", num)
>>>>>>> refs/remotes/origin/master

#include <linux/err.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <linux/dma-mapping.h>
=======
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/omap-dma.h>
#include <linux/pm_runtime.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
>>>>>>> refs/remotes/origin/master
#include <linux/io.h>
#include <linux/crypto.h>
#include <linux/interrupt.h>
#include <crypto/scatterwalk.h>
#include <crypto/aes.h>

<<<<<<< HEAD
#include <plat/cpu.h>
#include <plat/dma.h>
=======
#define DST_MAXBURST			4
#define DMA_MIN				(DST_MAXBURST * sizeof(u32))

#define _calc_walked(inout) (dd->inout##_walk.offset - dd->inout##_sg->offset)
>>>>>>> refs/remotes/origin/master

/* OMAP TRM gives bitfields as start:end, where start is the higher bit
   number. For example 7:0 */
#define FLD_MASK(start, end)	(((1 << ((start) - (end) + 1)) - 1) << (end))
#define FLD_VAL(val, start, end) (((val) << (end)) & FLD_MASK(start, end))

<<<<<<< HEAD
#define AES_REG_KEY(x)			(0x1C - ((x ^ 0x01) * 0x04))
#define AES_REG_IV(x)			(0x20 + ((x) * 0x04))

#define AES_REG_CTRL			0x30
#define AES_REG_CTRL_CTR_WIDTH		(1 << 7)
=======
#define AES_REG_KEY(dd, x)		((dd)->pdata->key_ofs - \
						((x ^ 0x01) * 0x04))
#define AES_REG_IV(dd, x)		((dd)->pdata->iv_ofs + ((x) * 0x04))

#define AES_REG_CTRL(dd)		((dd)->pdata->ctrl_ofs)
#define AES_REG_CTRL_CTR_WIDTH_MASK	(3 << 7)
#define AES_REG_CTRL_CTR_WIDTH_32		(0 << 7)
#define AES_REG_CTRL_CTR_WIDTH_64		(1 << 7)
#define AES_REG_CTRL_CTR_WIDTH_96		(2 << 7)
#define AES_REG_CTRL_CTR_WIDTH_128		(3 << 7)
>>>>>>> refs/remotes/origin/master
#define AES_REG_CTRL_CTR		(1 << 6)
#define AES_REG_CTRL_CBC		(1 << 5)
#define AES_REG_CTRL_KEY_SIZE		(3 << 3)
#define AES_REG_CTRL_DIRECTION		(1 << 2)
#define AES_REG_CTRL_INPUT_READY	(1 << 1)
#define AES_REG_CTRL_OUTPUT_READY	(1 << 0)

<<<<<<< HEAD
#define AES_REG_DATA			0x34
#define AES_REG_DATA_N(x)		(0x34 + ((x) * 0x04))

#define AES_REG_REV			0x44
#define AES_REG_REV_MAJOR		0xF0
#define AES_REG_REV_MINOR		0x0F

#define AES_REG_MASK			0x48
=======
#define AES_REG_DATA_N(dd, x)		((dd)->pdata->data_ofs + ((x) * 0x04))

#define AES_REG_REV(dd)			((dd)->pdata->rev_ofs)

#define AES_REG_MASK(dd)		((dd)->pdata->mask_ofs)
>>>>>>> refs/remotes/origin/master
#define AES_REG_MASK_SIDLE		(1 << 6)
#define AES_REG_MASK_START		(1 << 5)
#define AES_REG_MASK_DMA_OUT_EN		(1 << 3)
#define AES_REG_MASK_DMA_IN_EN		(1 << 2)
#define AES_REG_MASK_SOFTRESET		(1 << 1)
#define AES_REG_AUTOIDLE		(1 << 0)

<<<<<<< HEAD
#define AES_REG_SYSSTATUS		0x4C
#define AES_REG_SYSSTATUS_RESETDONE	(1 << 0)

=======
#define AES_REG_LENGTH_N(x)		(0x54 + ((x) * 0x04))

#define AES_REG_IRQ_STATUS(dd)         ((dd)->pdata->irq_status_ofs)
#define AES_REG_IRQ_ENABLE(dd)         ((dd)->pdata->irq_enable_ofs)
#define AES_REG_IRQ_DATA_IN            BIT(1)
#define AES_REG_IRQ_DATA_OUT           BIT(2)
>>>>>>> refs/remotes/origin/master
#define DEFAULT_TIMEOUT		(5*HZ)

#define FLAGS_MODE_MASK		0x000f
#define FLAGS_ENCRYPT		BIT(0)
#define FLAGS_CBC		BIT(1)
#define FLAGS_GIV		BIT(2)
<<<<<<< HEAD
=======
#define FLAGS_CTR		BIT(3)
>>>>>>> refs/remotes/origin/master

#define FLAGS_INIT		BIT(4)
#define FLAGS_FAST		BIT(5)
#define FLAGS_BUSY		BIT(6)

<<<<<<< HEAD
=======
#define AES_BLOCK_WORDS		(AES_BLOCK_SIZE >> 2)

>>>>>>> refs/remotes/origin/master
struct omap_aes_ctx {
	struct omap_aes_dev *dd;

	int		keylen;
	u32		key[AES_KEYSIZE_256 / sizeof(u32)];
	unsigned long	flags;
};

struct omap_aes_reqctx {
	unsigned long mode;
};

#define OMAP_AES_QUEUE_LENGTH	1
#define OMAP_AES_CACHE_SIZE	0

<<<<<<< HEAD
=======
struct omap_aes_algs_info {
	struct crypto_alg	*algs_list;
	unsigned int		size;
	unsigned int		registered;
};

struct omap_aes_pdata {
	struct omap_aes_algs_info	*algs_info;
	unsigned int	algs_info_size;

	void		(*trigger)(struct omap_aes_dev *dd, int length);

	u32		key_ofs;
	u32		iv_ofs;
	u32		ctrl_ofs;
	u32		data_ofs;
	u32		rev_ofs;
	u32		mask_ofs;
	u32             irq_enable_ofs;
	u32             irq_status_ofs;

	u32		dma_enable_in;
	u32		dma_enable_out;
	u32		dma_start;

	u32		major_mask;
	u32		major_shift;
	u32		minor_mask;
	u32		minor_shift;
};

>>>>>>> refs/remotes/origin/master
struct omap_aes_dev {
	struct list_head	list;
	unsigned long		phys_base;
	void __iomem		*io_base;
<<<<<<< HEAD
	struct clk		*iclk;
=======
>>>>>>> refs/remotes/origin/master
	struct omap_aes_ctx	*ctx;
	struct device		*dev;
	unsigned long		flags;
	int			err;

	spinlock_t		lock;
	struct crypto_queue	queue;

	struct tasklet_struct	done_task;
	struct tasklet_struct	queue_task;

	struct ablkcipher_request	*req;
<<<<<<< HEAD
	size_t				total;
	struct scatterlist		*in_sg;
	size_t				in_offset;
	struct scatterlist		*out_sg;
	size_t				out_offset;

	size_t			buflen;
	void			*buf_in;
	size_t			dma_size;
	int			dma_in;
	int			dma_lch_in;
	dma_addr_t		dma_addr_in;
	void			*buf_out;
	int			dma_out;
	int			dma_lch_out;
	dma_addr_t		dma_addr_out;
=======

	/*
	 * total is used by PIO mode for book keeping so introduce
	 * variable total_save as need it to calc page_order
	 */
	size_t				total;
	size_t				total_save;

	struct scatterlist		*in_sg;
	struct scatterlist		*out_sg;

	/* Buffers for copying for unaligned cases */
	struct scatterlist		in_sgl;
	struct scatterlist		out_sgl;
	struct scatterlist		*orig_out;
	int				sgs_copied;

	struct scatter_walk		in_walk;
	struct scatter_walk		out_walk;
	int			dma_in;
	struct dma_chan		*dma_lch_in;
	int			dma_out;
	struct dma_chan		*dma_lch_out;
	int			in_sg_len;
	int			out_sg_len;
	int			pio_only;
	const struct omap_aes_pdata	*pdata;
>>>>>>> refs/remotes/origin/master
};

/* keep registered devices data here */
static LIST_HEAD(dev_list);
static DEFINE_SPINLOCK(list_lock);

<<<<<<< HEAD
=======
#ifdef DEBUG
#define omap_aes_read(dd, offset)				\
({								\
	int _read_ret;						\
	_read_ret = __raw_readl(dd->io_base + offset);		\
	pr_debug("omap_aes_read(" #offset "=%#x)= %#x\n",	\
		 offset, _read_ret);				\
	_read_ret;						\
})
#else
>>>>>>> refs/remotes/origin/master
static inline u32 omap_aes_read(struct omap_aes_dev *dd, u32 offset)
{
	return __raw_readl(dd->io_base + offset);
}
<<<<<<< HEAD

=======
#endif

#ifdef DEBUG
#define omap_aes_write(dd, offset, value)				\
	do {								\
		pr_debug("omap_aes_write(" #offset "=%#x) value=%#x\n",	\
			 offset, value);				\
		__raw_writel(value, dd->io_base + offset);		\
	} while (0)
#else
>>>>>>> refs/remotes/origin/master
static inline void omap_aes_write(struct omap_aes_dev *dd, u32 offset,
				  u32 value)
{
	__raw_writel(value, dd->io_base + offset);
}
<<<<<<< HEAD
=======
#endif
>>>>>>> refs/remotes/origin/master

static inline void omap_aes_write_mask(struct omap_aes_dev *dd, u32 offset,
					u32 value, u32 mask)
{
	u32 val;

	val = omap_aes_read(dd, offset);
	val &= ~mask;
	val |= value;
	omap_aes_write(dd, offset, val);
}

static void omap_aes_write_n(struct omap_aes_dev *dd, u32 offset,
					u32 *value, int count)
{
	for (; count--; value++, offset += 4)
		omap_aes_write(dd, offset, *value);
}

<<<<<<< HEAD
static int omap_aes_wait(struct omap_aes_dev *dd, u32 offset, u32 bit)
{
	unsigned long timeout = jiffies + DEFAULT_TIMEOUT;

	while (!(omap_aes_read(dd, offset) & bit)) {
		if (time_is_before_jiffies(timeout)) {
			dev_err(dd->dev, "omap-aes timeout\n");
			return -ETIMEDOUT;
		}
	}
	return 0;
}

static int omap_aes_hw_init(struct omap_aes_dev *dd)
{
	/*
	 * clocks are enabled when request starts and disabled when finished.
	 * It may be long delays between requests.
	 * Device might go to off mode to save power.
	 */
	clk_enable(dd->iclk);

	if (!(dd->flags & FLAGS_INIT)) {
		/* is it necessary to reset before every operation? */
		omap_aes_write_mask(dd, AES_REG_MASK, AES_REG_MASK_SOFTRESET,
					AES_REG_MASK_SOFTRESET);
		/*
		 * prevent OCP bus error (SRESP) in case an access to the module
		 * is performed while the module is coming out of soft reset
		 */
		__asm__ __volatile__("nop");
		__asm__ __volatile__("nop");

		if (omap_aes_wait(dd, AES_REG_SYSSTATUS,
				AES_REG_SYSSTATUS_RESETDONE))
			return -ETIMEDOUT;

=======
static int omap_aes_hw_init(struct omap_aes_dev *dd)
{
	if (!(dd->flags & FLAGS_INIT)) {
>>>>>>> refs/remotes/origin/master
		dd->flags |= FLAGS_INIT;
		dd->err = 0;
	}

	return 0;
}

static int omap_aes_write_ctrl(struct omap_aes_dev *dd)
{
	unsigned int key32;
	int i, err;
<<<<<<< HEAD
	u32 val, mask;
=======
	u32 val, mask = 0;
>>>>>>> refs/remotes/origin/master

	err = omap_aes_hw_init(dd);
	if (err)
		return err;

<<<<<<< HEAD
	val = 0;
	if (dd->dma_lch_out >= 0)
		val |= AES_REG_MASK_DMA_OUT_EN;
	if (dd->dma_lch_in >= 0)
		val |= AES_REG_MASK_DMA_IN_EN;

	mask = AES_REG_MASK_DMA_IN_EN | AES_REG_MASK_DMA_OUT_EN;

	omap_aes_write_mask(dd, AES_REG_MASK, val, mask);

=======
>>>>>>> refs/remotes/origin/master
	key32 = dd->ctx->keylen / sizeof(u32);

	/* it seems a key should always be set even if it has not changed */
	for (i = 0; i < key32; i++) {
<<<<<<< HEAD
		omap_aes_write(dd, AES_REG_KEY(i),
			__le32_to_cpu(dd->ctx->key[i]));
	}

	if ((dd->flags & FLAGS_CBC) && dd->req->info)
		omap_aes_write_n(dd, AES_REG_IV(0), dd->req->info, 4);
=======
		omap_aes_write(dd, AES_REG_KEY(dd, i),
			__le32_to_cpu(dd->ctx->key[i]));
	}

	if ((dd->flags & (FLAGS_CBC | FLAGS_CTR)) && dd->req->info)
		omap_aes_write_n(dd, AES_REG_IV(dd, 0), dd->req->info, 4);
>>>>>>> refs/remotes/origin/master

	val = FLD_VAL(((dd->ctx->keylen >> 3) - 1), 4, 3);
	if (dd->flags & FLAGS_CBC)
		val |= AES_REG_CTRL_CBC;
<<<<<<< HEAD
	if (dd->flags & FLAGS_ENCRYPT)
		val |= AES_REG_CTRL_DIRECTION;

	mask = AES_REG_CTRL_CBC | AES_REG_CTRL_DIRECTION |
			AES_REG_CTRL_KEY_SIZE;

	omap_aes_write_mask(dd, AES_REG_CTRL, val, mask);

	/* IN */
	omap_set_dma_dest_params(dd->dma_lch_in, 0, OMAP_DMA_AMODE_CONSTANT,
				 dd->phys_base + AES_REG_DATA, 0, 4);

	omap_set_dma_dest_burst_mode(dd->dma_lch_in, OMAP_DMA_DATA_BURST_4);
	omap_set_dma_src_burst_mode(dd->dma_lch_in, OMAP_DMA_DATA_BURST_4);

	/* OUT */
	omap_set_dma_src_params(dd->dma_lch_out, 0, OMAP_DMA_AMODE_CONSTANT,
				dd->phys_base + AES_REG_DATA, 0, 4);

	omap_set_dma_src_burst_mode(dd->dma_lch_out, OMAP_DMA_DATA_BURST_4);
	omap_set_dma_dest_burst_mode(dd->dma_lch_out, OMAP_DMA_DATA_BURST_4);

	return 0;
=======
	if (dd->flags & FLAGS_CTR) {
		val |= AES_REG_CTRL_CTR | AES_REG_CTRL_CTR_WIDTH_128;
		mask = AES_REG_CTRL_CTR | AES_REG_CTRL_CTR_WIDTH_MASK;
	}
	if (dd->flags & FLAGS_ENCRYPT)
		val |= AES_REG_CTRL_DIRECTION;

	mask |= AES_REG_CTRL_CBC | AES_REG_CTRL_DIRECTION |
			AES_REG_CTRL_KEY_SIZE;

	omap_aes_write_mask(dd, AES_REG_CTRL(dd), val, mask);

	return 0;
}

static void omap_aes_dma_trigger_omap2(struct omap_aes_dev *dd, int length)
{
	u32 mask, val;

	val = dd->pdata->dma_start;

	if (dd->dma_lch_out != NULL)
		val |= dd->pdata->dma_enable_out;
	if (dd->dma_lch_in != NULL)
		val |= dd->pdata->dma_enable_in;

	mask = dd->pdata->dma_enable_out | dd->pdata->dma_enable_in |
	       dd->pdata->dma_start;

	omap_aes_write_mask(dd, AES_REG_MASK(dd), val, mask);

}

static void omap_aes_dma_trigger_omap4(struct omap_aes_dev *dd, int length)
{
	omap_aes_write(dd, AES_REG_LENGTH_N(0), length);
	omap_aes_write(dd, AES_REG_LENGTH_N(1), 0);

	omap_aes_dma_trigger_omap2(dd, length);
}

static void omap_aes_dma_stop(struct omap_aes_dev *dd)
{
	u32 mask;

	mask = dd->pdata->dma_enable_out | dd->pdata->dma_enable_in |
	       dd->pdata->dma_start;

	omap_aes_write_mask(dd, AES_REG_MASK(dd), 0, mask);
>>>>>>> refs/remotes/origin/master
}

static struct omap_aes_dev *omap_aes_find_dev(struct omap_aes_ctx *ctx)
{
	struct omap_aes_dev *dd = NULL, *tmp;

	spin_lock_bh(&list_lock);
	if (!ctx->dd) {
		list_for_each_entry(tmp, &dev_list, list) {
			/* FIXME: take fist available aes core */
			dd = tmp;
			break;
		}
		ctx->dd = dd;
	} else {
		/* already found before */
		dd = ctx->dd;
	}
	spin_unlock_bh(&list_lock);

	return dd;
}

<<<<<<< HEAD
static void omap_aes_dma_callback(int lch, u16 ch_status, void *data)
{
	struct omap_aes_dev *dd = data;

	if (ch_status != OMAP_DMA_BLOCK_IRQ) {
		pr_err("omap-aes DMA error status: 0x%hx\n", ch_status);
		dd->err = -EIO;
		dd->flags &= ~FLAGS_INIT; /* request to re-initialize */
	} else if (lch == dd->dma_lch_in) {
		return;
	}

=======
static void omap_aes_dma_out_callback(void *data)
{
	struct omap_aes_dev *dd = data;

>>>>>>> refs/remotes/origin/master
	/* dma_lch_out - completed */
	tasklet_schedule(&dd->done_task);
}

static int omap_aes_dma_init(struct omap_aes_dev *dd)
{
	int err = -ENOMEM;
<<<<<<< HEAD

	dd->dma_lch_out = -1;
	dd->dma_lch_in = -1;

	dd->buf_in = (void *)__get_free_pages(GFP_KERNEL, OMAP_AES_CACHE_SIZE);
	dd->buf_out = (void *)__get_free_pages(GFP_KERNEL, OMAP_AES_CACHE_SIZE);
	dd->buflen = PAGE_SIZE << OMAP_AES_CACHE_SIZE;
	dd->buflen &= ~(AES_BLOCK_SIZE - 1);

	if (!dd->buf_in || !dd->buf_out) {
		dev_err(dd->dev, "unable to alloc pages.\n");
		goto err_alloc;
	}

	/* MAP here */
	dd->dma_addr_in = dma_map_single(dd->dev, dd->buf_in, dd->buflen,
					 DMA_TO_DEVICE);
	if (dma_mapping_error(dd->dev, dd->dma_addr_in)) {
		dev_err(dd->dev, "dma %d bytes error\n", dd->buflen);
		err = -EINVAL;
		goto err_map_in;
	}

	dd->dma_addr_out = dma_map_single(dd->dev, dd->buf_out, dd->buflen,
					  DMA_FROM_DEVICE);
	if (dma_mapping_error(dd->dev, dd->dma_addr_out)) {
		dev_err(dd->dev, "dma %d bytes error\n", dd->buflen);
		err = -EINVAL;
		goto err_map_out;
	}

	err = omap_request_dma(dd->dma_in, "omap-aes-rx",
			       omap_aes_dma_callback, dd, &dd->dma_lch_in);
	if (err) {
		dev_err(dd->dev, "Unable to request DMA channel\n");
		goto err_dma_in;
	}
	err = omap_request_dma(dd->dma_out, "omap-aes-tx",
			       omap_aes_dma_callback, dd, &dd->dma_lch_out);
	if (err) {
		dev_err(dd->dev, "Unable to request DMA channel\n");
=======
	dma_cap_mask_t mask;

	dd->dma_lch_out = NULL;
	dd->dma_lch_in = NULL;

	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

	dd->dma_lch_in = dma_request_slave_channel_compat(mask,
							  omap_dma_filter_fn,
							  &dd->dma_in,
							  dd->dev, "rx");
	if (!dd->dma_lch_in) {
		dev_err(dd->dev, "Unable to request in DMA channel\n");
		goto err_dma_in;
	}

	dd->dma_lch_out = dma_request_slave_channel_compat(mask,
							   omap_dma_filter_fn,
							   &dd->dma_out,
							   dd->dev, "tx");
	if (!dd->dma_lch_out) {
		dev_err(dd->dev, "Unable to request out DMA channel\n");
>>>>>>> refs/remotes/origin/master
		goto err_dma_out;
	}

	return 0;

err_dma_out:
<<<<<<< HEAD
	omap_free_dma(dd->dma_lch_in);
err_dma_in:
	dma_unmap_single(dd->dev, dd->dma_addr_out, dd->buflen,
			 DMA_FROM_DEVICE);
err_map_out:
	dma_unmap_single(dd->dev, dd->dma_addr_in, dd->buflen, DMA_TO_DEVICE);
err_map_in:
	free_pages((unsigned long)dd->buf_out, OMAP_AES_CACHE_SIZE);
	free_pages((unsigned long)dd->buf_in, OMAP_AES_CACHE_SIZE);
err_alloc:
=======
	dma_release_channel(dd->dma_lch_in);
err_dma_in:
>>>>>>> refs/remotes/origin/master
	if (err)
		pr_err("error: %d\n", err);
	return err;
}

static void omap_aes_dma_cleanup(struct omap_aes_dev *dd)
{
<<<<<<< HEAD
	omap_free_dma(dd->dma_lch_out);
	omap_free_dma(dd->dma_lch_in);
	dma_unmap_single(dd->dev, dd->dma_addr_out, dd->buflen,
			 DMA_FROM_DEVICE);
	dma_unmap_single(dd->dev, dd->dma_addr_in, dd->buflen, DMA_TO_DEVICE);
	free_pages((unsigned long)dd->buf_out, OMAP_AES_CACHE_SIZE);
	free_pages((unsigned long)dd->buf_in, OMAP_AES_CACHE_SIZE);
=======
	dma_release_channel(dd->dma_lch_out);
	dma_release_channel(dd->dma_lch_in);
>>>>>>> refs/remotes/origin/master
}

static void sg_copy_buf(void *buf, struct scatterlist *sg,
			      unsigned int start, unsigned int nbytes, int out)
{
	struct scatter_walk walk;

	if (!nbytes)
		return;

	scatterwalk_start(&walk, sg);
	scatterwalk_advance(&walk, start);
	scatterwalk_copychunks(buf, &walk, nbytes, out);
	scatterwalk_done(&walk, out, 0);
}

<<<<<<< HEAD
static int sg_copy(struct scatterlist **sg, size_t *offset, void *buf,
		   size_t buflen, size_t total, int out)
{
	unsigned int count, off = 0;

	while (buflen && total) {
		count = min((*sg)->length - *offset, total);
		count = min(count, buflen);

		if (!count)
			return off;

		/*
		 * buflen and total are AES_BLOCK_SIZE size aligned,
		 * so count should be also aligned
		 */

		sg_copy_buf(buf + off, *sg, *offset, count, out);

		off += count;
		buflen -= count;
		*offset += count;
		total -= count;

		if (*offset == (*sg)->length) {
			*sg = sg_next(*sg);
			if (*sg)
				*offset = 0;
			else
				total = 0;
		}
	}

	return off;
}

static int omap_aes_crypt_dma(struct crypto_tfm *tfm, dma_addr_t dma_addr_in,
			       dma_addr_t dma_addr_out, int length)
{
	struct omap_aes_ctx *ctx = crypto_tfm_ctx(tfm);
	struct omap_aes_dev *dd = ctx->dd;
	int len32;

	pr_debug("len: %d\n", length);

	dd->dma_size = length;

	if (!(dd->flags & FLAGS_FAST))
		dma_sync_single_for_device(dd->dev, dma_addr_in, length,
					   DMA_TO_DEVICE);

	len32 = DIV_ROUND_UP(length, sizeof(u32));

	/* IN */
	omap_set_dma_transfer_params(dd->dma_lch_in, OMAP_DMA_DATA_TYPE_S32,
				     len32, 1, OMAP_DMA_SYNC_PACKET, dd->dma_in,
					OMAP_DMA_DST_SYNC);

	omap_set_dma_src_params(dd->dma_lch_in, 0, OMAP_DMA_AMODE_POST_INC,
				dma_addr_in, 0, 0);

	/* OUT */
	omap_set_dma_transfer_params(dd->dma_lch_out, OMAP_DMA_DATA_TYPE_S32,
				     len32, 1, OMAP_DMA_SYNC_PACKET,
					dd->dma_out, OMAP_DMA_SRC_SYNC);

	omap_set_dma_dest_params(dd->dma_lch_out, 0, OMAP_DMA_AMODE_POST_INC,
				 dma_addr_out, 0, 0);

	omap_start_dma(dd->dma_lch_in);
	omap_start_dma(dd->dma_lch_out);

	/* start DMA or disable idle mode */
	omap_aes_write_mask(dd, AES_REG_MASK, AES_REG_MASK_START,
			    AES_REG_MASK_START);
=======
static int omap_aes_crypt_dma(struct crypto_tfm *tfm,
		struct scatterlist *in_sg, struct scatterlist *out_sg,
		int in_sg_len, int out_sg_len)
{
	struct omap_aes_ctx *ctx = crypto_tfm_ctx(tfm);
	struct omap_aes_dev *dd = ctx->dd;
	struct dma_async_tx_descriptor *tx_in, *tx_out;
	struct dma_slave_config cfg;
	int ret;

	if (dd->pio_only) {
		scatterwalk_start(&dd->in_walk, dd->in_sg);
		scatterwalk_start(&dd->out_walk, dd->out_sg);

		/* Enable DATAIN interrupt and let it take
		   care of the rest */
		omap_aes_write(dd, AES_REG_IRQ_ENABLE(dd), 0x2);
		return 0;
	}

	dma_sync_sg_for_device(dd->dev, dd->in_sg, in_sg_len, DMA_TO_DEVICE);

	memset(&cfg, 0, sizeof(cfg));

	cfg.src_addr = dd->phys_base + AES_REG_DATA_N(dd, 0);
	cfg.dst_addr = dd->phys_base + AES_REG_DATA_N(dd, 0);
	cfg.src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	cfg.dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	cfg.src_maxburst = DST_MAXBURST;
	cfg.dst_maxburst = DST_MAXBURST;

	/* IN */
	ret = dmaengine_slave_config(dd->dma_lch_in, &cfg);
	if (ret) {
		dev_err(dd->dev, "can't configure IN dmaengine slave: %d\n",
			ret);
		return ret;
	}

	tx_in = dmaengine_prep_slave_sg(dd->dma_lch_in, in_sg, in_sg_len,
					DMA_MEM_TO_DEV,
					DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	if (!tx_in) {
		dev_err(dd->dev, "IN prep_slave_sg() failed\n");
		return -EINVAL;
	}

	/* No callback necessary */
	tx_in->callback_param = dd;

	/* OUT */
	ret = dmaengine_slave_config(dd->dma_lch_out, &cfg);
	if (ret) {
		dev_err(dd->dev, "can't configure OUT dmaengine slave: %d\n",
			ret);
		return ret;
	}

	tx_out = dmaengine_prep_slave_sg(dd->dma_lch_out, out_sg, out_sg_len,
					DMA_DEV_TO_MEM,
					DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	if (!tx_out) {
		dev_err(dd->dev, "OUT prep_slave_sg() failed\n");
		return -EINVAL;
	}

	tx_out->callback = omap_aes_dma_out_callback;
	tx_out->callback_param = dd;

	dmaengine_submit(tx_in);
	dmaengine_submit(tx_out);

	dma_async_issue_pending(dd->dma_lch_in);
	dma_async_issue_pending(dd->dma_lch_out);

	/* start DMA */
	dd->pdata->trigger(dd, dd->total);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int omap_aes_crypt_dma_start(struct omap_aes_dev *dd)
{
	struct crypto_tfm *tfm = crypto_ablkcipher_tfm(
					crypto_ablkcipher_reqtfm(dd->req));
<<<<<<< HEAD
	int err, fast = 0, in, out;
	size_t count;
	dma_addr_t addr_in, addr_out;

	pr_debug("total: %d\n", dd->total);

	if (sg_is_last(dd->in_sg) && sg_is_last(dd->out_sg)) {
		/* check for alignment */
		in = IS_ALIGNED((u32)dd->in_sg->offset, sizeof(u32));
		out = IS_ALIGNED((u32)dd->out_sg->offset, sizeof(u32));

		fast = in && out;
	}

	if (fast)  {
		count = min(dd->total, sg_dma_len(dd->in_sg));
		count = min(count, sg_dma_len(dd->out_sg));

		if (count != dd->total) {
			pr_err("request length != buffer length\n");
			return -EINVAL;
		}

		pr_debug("fast\n");

		err = dma_map_sg(dd->dev, dd->in_sg, 1, DMA_TO_DEVICE);
=======
	int err;

	pr_debug("total: %d\n", dd->total);

	if (!dd->pio_only) {
		err = dma_map_sg(dd->dev, dd->in_sg, dd->in_sg_len,
				 DMA_TO_DEVICE);
>>>>>>> refs/remotes/origin/master
		if (!err) {
			dev_err(dd->dev, "dma_map_sg() error\n");
			return -EINVAL;
		}

<<<<<<< HEAD
		err = dma_map_sg(dd->dev, dd->out_sg, 1, DMA_FROM_DEVICE);
		if (!err) {
			dev_err(dd->dev, "dma_map_sg() error\n");
			dma_unmap_sg(dd->dev, dd->in_sg, 1, DMA_TO_DEVICE);
			return -EINVAL;
		}

		addr_in = sg_dma_address(dd->in_sg);
		addr_out = sg_dma_address(dd->out_sg);

		dd->flags |= FLAGS_FAST;

	} else {
		/* use cache buffers */
		count = sg_copy(&dd->in_sg, &dd->in_offset, dd->buf_in,
				 dd->buflen, dd->total, 0);

		addr_in = dd->dma_addr_in;
		addr_out = dd->dma_addr_out;

		dd->flags &= ~FLAGS_FAST;

	}

	dd->total -= count;

	err = omap_aes_crypt_dma(tfm, addr_in, addr_out, count);
	if (err) {
		dma_unmap_sg(dd->dev, dd->in_sg, 1, DMA_TO_DEVICE);
		dma_unmap_sg(dd->dev, dd->out_sg, 1, DMA_TO_DEVICE);
=======
		err = dma_map_sg(dd->dev, dd->out_sg, dd->out_sg_len,
				 DMA_FROM_DEVICE);
		if (!err) {
			dev_err(dd->dev, "dma_map_sg() error\n");
			return -EINVAL;
		}
	}

	err = omap_aes_crypt_dma(tfm, dd->in_sg, dd->out_sg, dd->in_sg_len,
				 dd->out_sg_len);
	if (err && !dd->pio_only) {
		dma_unmap_sg(dd->dev, dd->in_sg, dd->in_sg_len, DMA_TO_DEVICE);
		dma_unmap_sg(dd->dev, dd->out_sg, dd->out_sg_len,
			     DMA_FROM_DEVICE);
>>>>>>> refs/remotes/origin/master
	}

	return err;
}

static void omap_aes_finish_req(struct omap_aes_dev *dd, int err)
{
	struct ablkcipher_request *req = dd->req;

	pr_debug("err: %d\n", err);

<<<<<<< HEAD
	clk_disable(dd->iclk);
=======
>>>>>>> refs/remotes/origin/master
	dd->flags &= ~FLAGS_BUSY;

	req->base.complete(&req->base, err);
}

static int omap_aes_crypt_dma_stop(struct omap_aes_dev *dd)
{
	int err = 0;
<<<<<<< HEAD
	size_t count;

	pr_debug("total: %d\n", dd->total);

	omap_aes_write_mask(dd, AES_REG_MASK, 0, AES_REG_MASK_START);

	omap_stop_dma(dd->dma_lch_in);
	omap_stop_dma(dd->dma_lch_out);

	if (dd->flags & FLAGS_FAST) {
		dma_unmap_sg(dd->dev, dd->out_sg, 1, DMA_FROM_DEVICE);
		dma_unmap_sg(dd->dev, dd->in_sg, 1, DMA_TO_DEVICE);
	} else {
		dma_sync_single_for_device(dd->dev, dd->dma_addr_out,
					   dd->dma_size, DMA_FROM_DEVICE);

		/* copy data */
		count = sg_copy(&dd->out_sg, &dd->out_offset, dd->buf_out,
				 dd->buflen, dd->dma_size, 1);
		if (count != dd->dma_size) {
			err = -EINVAL;
			pr_err("not all data converted: %u\n", count);
		}
	}

	return err;
=======

	pr_debug("total: %d\n", dd->total);

	omap_aes_dma_stop(dd);

	dmaengine_terminate_all(dd->dma_lch_in);
	dmaengine_terminate_all(dd->dma_lch_out);

	return err;
}

static int omap_aes_check_aligned(struct scatterlist *sg)
{
	while (sg) {
		if (!IS_ALIGNED(sg->offset, 4))
			return -1;
		if (!IS_ALIGNED(sg->length, AES_BLOCK_SIZE))
			return -1;
		sg = sg_next(sg);
	}
	return 0;
}

static int omap_aes_copy_sgs(struct omap_aes_dev *dd)
{
	void *buf_in, *buf_out;
	int pages;

	pages = get_order(dd->total);

	buf_in = (void *)__get_free_pages(GFP_ATOMIC, pages);
	buf_out = (void *)__get_free_pages(GFP_ATOMIC, pages);

	if (!buf_in || !buf_out) {
		pr_err("Couldn't allocated pages for unaligned cases.\n");
		return -1;
	}

	dd->orig_out = dd->out_sg;

	sg_copy_buf(buf_in, dd->in_sg, 0, dd->total, 0);

	sg_init_table(&dd->in_sgl, 1);
	sg_set_buf(&dd->in_sgl, buf_in, dd->total);
	dd->in_sg = &dd->in_sgl;

	sg_init_table(&dd->out_sgl, 1);
	sg_set_buf(&dd->out_sgl, buf_out, dd->total);
	dd->out_sg = &dd->out_sgl;

	return 0;
>>>>>>> refs/remotes/origin/master
}

static int omap_aes_handle_queue(struct omap_aes_dev *dd,
			       struct ablkcipher_request *req)
{
	struct crypto_async_request *async_req, *backlog;
	struct omap_aes_ctx *ctx;
	struct omap_aes_reqctx *rctx;
	unsigned long flags;
	int err, ret = 0;

	spin_lock_irqsave(&dd->lock, flags);
	if (req)
		ret = ablkcipher_enqueue_request(&dd->queue, req);
	if (dd->flags & FLAGS_BUSY) {
		spin_unlock_irqrestore(&dd->lock, flags);
		return ret;
	}
	backlog = crypto_get_backlog(&dd->queue);
	async_req = crypto_dequeue_request(&dd->queue);
	if (async_req)
		dd->flags |= FLAGS_BUSY;
	spin_unlock_irqrestore(&dd->lock, flags);

	if (!async_req)
		return ret;

	if (backlog)
		backlog->complete(backlog, -EINPROGRESS);

	req = ablkcipher_request_cast(async_req);

	/* assign new request to device */
	dd->req = req;
	dd->total = req->nbytes;
<<<<<<< HEAD
	dd->in_offset = 0;
	dd->in_sg = req->src;
	dd->out_offset = 0;
	dd->out_sg = req->dst;

=======
	dd->total_save = req->nbytes;
	dd->in_sg = req->src;
	dd->out_sg = req->dst;

	if (omap_aes_check_aligned(dd->in_sg) ||
	    omap_aes_check_aligned(dd->out_sg)) {
		if (omap_aes_copy_sgs(dd))
			pr_err("Failed to copy SGs for unaligned cases\n");
		dd->sgs_copied = 1;
	} else {
		dd->sgs_copied = 0;
	}

	dd->in_sg_len = scatterwalk_bytes_sglen(dd->in_sg, dd->total);
	dd->out_sg_len = scatterwalk_bytes_sglen(dd->out_sg, dd->total);
	BUG_ON(dd->in_sg_len < 0 || dd->out_sg_len < 0);

>>>>>>> refs/remotes/origin/master
	rctx = ablkcipher_request_ctx(req);
	ctx = crypto_ablkcipher_ctx(crypto_ablkcipher_reqtfm(req));
	rctx->mode &= FLAGS_MODE_MASK;
	dd->flags = (dd->flags & ~FLAGS_MODE_MASK) | rctx->mode;

	dd->ctx = ctx;
	ctx->dd = dd;

	err = omap_aes_write_ctrl(dd);
	if (!err)
		err = omap_aes_crypt_dma_start(dd);
	if (err) {
		/* aes_task will not finish it, so do it here */
		omap_aes_finish_req(dd, err);
		tasklet_schedule(&dd->queue_task);
	}

	return ret; /* return ret, which is enqueue return value */
}

static void omap_aes_done_task(unsigned long data)
{
	struct omap_aes_dev *dd = (struct omap_aes_dev *)data;
<<<<<<< HEAD
	int err;

	pr_debug("enter\n");

	err = omap_aes_crypt_dma_stop(dd);

	err = dd->err ? : err;

	if (dd->total && !err) {
		err = omap_aes_crypt_dma_start(dd);
		if (!err)
			return; /* DMA started. Not fininishing. */
	}

	omap_aes_finish_req(dd, err);
=======
	void *buf_in, *buf_out;
	int pages;

	pr_debug("enter done_task\n");

	if (!dd->pio_only) {
		dma_sync_sg_for_device(dd->dev, dd->out_sg, dd->out_sg_len,
				       DMA_FROM_DEVICE);
		dma_unmap_sg(dd->dev, dd->in_sg, dd->in_sg_len, DMA_TO_DEVICE);
		dma_unmap_sg(dd->dev, dd->out_sg, dd->out_sg_len,
			     DMA_FROM_DEVICE);
		omap_aes_crypt_dma_stop(dd);
	}

	if (dd->sgs_copied) {
		buf_in = sg_virt(&dd->in_sgl);
		buf_out = sg_virt(&dd->out_sgl);

		sg_copy_buf(buf_out, dd->orig_out, 0, dd->total_save, 1);

		pages = get_order(dd->total_save);
		free_pages((unsigned long)buf_in, pages);
		free_pages((unsigned long)buf_out, pages);
	}

	omap_aes_finish_req(dd, 0);
>>>>>>> refs/remotes/origin/master
	omap_aes_handle_queue(dd, NULL);

	pr_debug("exit\n");
}

static void omap_aes_queue_task(unsigned long data)
{
	struct omap_aes_dev *dd = (struct omap_aes_dev *)data;

	omap_aes_handle_queue(dd, NULL);
}

static int omap_aes_crypt(struct ablkcipher_request *req, unsigned long mode)
{
	struct omap_aes_ctx *ctx = crypto_ablkcipher_ctx(
			crypto_ablkcipher_reqtfm(req));
	struct omap_aes_reqctx *rctx = ablkcipher_request_ctx(req);
	struct omap_aes_dev *dd;

	pr_debug("nbytes: %d, enc: %d, cbc: %d\n", req->nbytes,
		  !!(mode & FLAGS_ENCRYPT),
		  !!(mode & FLAGS_CBC));

	if (!IS_ALIGNED(req->nbytes, AES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of AES blocks\n");
		return -EINVAL;
	}

	dd = omap_aes_find_dev(ctx);
	if (!dd)
		return -ENODEV;

	rctx->mode = mode;

	return omap_aes_handle_queue(dd, req);
}

/* ********************** ALG API ************************************ */

static int omap_aes_setkey(struct crypto_ablkcipher *tfm, const u8 *key,
			   unsigned int keylen)
{
	struct omap_aes_ctx *ctx = crypto_ablkcipher_ctx(tfm);

	if (keylen != AES_KEYSIZE_128 && keylen != AES_KEYSIZE_192 &&
		   keylen != AES_KEYSIZE_256)
		return -EINVAL;

	pr_debug("enter, keylen: %d\n", keylen);

	memcpy(ctx->key, key, keylen);
	ctx->keylen = keylen;

	return 0;
}

static int omap_aes_ecb_encrypt(struct ablkcipher_request *req)
{
	return omap_aes_crypt(req, FLAGS_ENCRYPT);
}

static int omap_aes_ecb_decrypt(struct ablkcipher_request *req)
{
	return omap_aes_crypt(req, 0);
}

static int omap_aes_cbc_encrypt(struct ablkcipher_request *req)
{
	return omap_aes_crypt(req, FLAGS_ENCRYPT | FLAGS_CBC);
}

static int omap_aes_cbc_decrypt(struct ablkcipher_request *req)
{
	return omap_aes_crypt(req, FLAGS_CBC);
}

<<<<<<< HEAD
static int omap_aes_cra_init(struct crypto_tfm *tfm)
{
	pr_debug("enter\n");

=======
static int omap_aes_ctr_encrypt(struct ablkcipher_request *req)
{
	return omap_aes_crypt(req, FLAGS_ENCRYPT | FLAGS_CTR);
}

static int omap_aes_ctr_decrypt(struct ablkcipher_request *req)
{
	return omap_aes_crypt(req, FLAGS_CTR);
}

static int omap_aes_cra_init(struct crypto_tfm *tfm)
{
	struct omap_aes_dev *dd = NULL;

	/* Find AES device, currently picks the first device */
	spin_lock_bh(&list_lock);
	list_for_each_entry(dd, &dev_list, list) {
		break;
	}
	spin_unlock_bh(&list_lock);

	pm_runtime_get_sync(dd->dev);
>>>>>>> refs/remotes/origin/master
	tfm->crt_ablkcipher.reqsize = sizeof(struct omap_aes_reqctx);

	return 0;
}

static void omap_aes_cra_exit(struct crypto_tfm *tfm)
{
<<<<<<< HEAD
	pr_debug("enter\n");
=======
	struct omap_aes_dev *dd = NULL;

	/* Find AES device, currently picks the first device */
	spin_lock_bh(&list_lock);
	list_for_each_entry(dd, &dev_list, list) {
		break;
	}
	spin_unlock_bh(&list_lock);

	pm_runtime_put_sync(dd->dev);
>>>>>>> refs/remotes/origin/master
}

/* ********************** ALGS ************************************ */

<<<<<<< HEAD
static struct crypto_alg algs[] = {
=======
static struct crypto_alg algs_ecb_cbc[] = {
>>>>>>> refs/remotes/origin/master
{
	.cra_name		= "ecb(aes)",
	.cra_driver_name	= "ecb-aes-omap",
	.cra_priority		= 100,
<<<<<<< HEAD
<<<<<<< HEAD
	.cra_flags		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
=======
	.cra_flags		= CRYPTO_ALG_TYPE_ABLKCIPHER |
				  CRYPTO_ALG_KERN_DRIVER_ONLY |
				  CRYPTO_ALG_ASYNC,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.cra_flags		= CRYPTO_ALG_TYPE_ABLKCIPHER |
				  CRYPTO_ALG_KERN_DRIVER_ONLY |
				  CRYPTO_ALG_ASYNC,
>>>>>>> refs/remotes/origin/master
	.cra_blocksize		= AES_BLOCK_SIZE,
	.cra_ctxsize		= sizeof(struct omap_aes_ctx),
	.cra_alignmask		= 0,
	.cra_type		= &crypto_ablkcipher_type,
	.cra_module		= THIS_MODULE,
	.cra_init		= omap_aes_cra_init,
	.cra_exit		= omap_aes_cra_exit,
	.cra_u.ablkcipher = {
		.min_keysize	= AES_MIN_KEY_SIZE,
		.max_keysize	= AES_MAX_KEY_SIZE,
		.setkey		= omap_aes_setkey,
		.encrypt	= omap_aes_ecb_encrypt,
		.decrypt	= omap_aes_ecb_decrypt,
	}
},
{
	.cra_name		= "cbc(aes)",
	.cra_driver_name	= "cbc-aes-omap",
	.cra_priority		= 100,
<<<<<<< HEAD
<<<<<<< HEAD
	.cra_flags		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
=======
	.cra_flags		= CRYPTO_ALG_TYPE_ABLKCIPHER |
				  CRYPTO_ALG_KERN_DRIVER_ONLY |
				  CRYPTO_ALG_ASYNC,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.cra_flags		= CRYPTO_ALG_TYPE_ABLKCIPHER |
				  CRYPTO_ALG_KERN_DRIVER_ONLY |
				  CRYPTO_ALG_ASYNC,
>>>>>>> refs/remotes/origin/master
	.cra_blocksize		= AES_BLOCK_SIZE,
	.cra_ctxsize		= sizeof(struct omap_aes_ctx),
	.cra_alignmask		= 0,
	.cra_type		= &crypto_ablkcipher_type,
	.cra_module		= THIS_MODULE,
	.cra_init		= omap_aes_cra_init,
	.cra_exit		= omap_aes_cra_exit,
	.cra_u.ablkcipher = {
		.min_keysize	= AES_MIN_KEY_SIZE,
		.max_keysize	= AES_MAX_KEY_SIZE,
		.ivsize		= AES_BLOCK_SIZE,
		.setkey		= omap_aes_setkey,
		.encrypt	= omap_aes_cbc_encrypt,
		.decrypt	= omap_aes_cbc_decrypt,
	}
}
};

<<<<<<< HEAD
=======
static struct crypto_alg algs_ctr[] = {
{
	.cra_name		= "ctr(aes)",
	.cra_driver_name	= "ctr-aes-omap",
	.cra_priority		= 100,
	.cra_flags		= CRYPTO_ALG_TYPE_ABLKCIPHER |
				  CRYPTO_ALG_KERN_DRIVER_ONLY |
				  CRYPTO_ALG_ASYNC,
	.cra_blocksize		= AES_BLOCK_SIZE,
	.cra_ctxsize		= sizeof(struct omap_aes_ctx),
	.cra_alignmask		= 0,
	.cra_type		= &crypto_ablkcipher_type,
	.cra_module		= THIS_MODULE,
	.cra_init		= omap_aes_cra_init,
	.cra_exit		= omap_aes_cra_exit,
	.cra_u.ablkcipher = {
		.min_keysize	= AES_MIN_KEY_SIZE,
		.max_keysize	= AES_MAX_KEY_SIZE,
		.geniv		= "eseqiv",
		.ivsize		= AES_BLOCK_SIZE,
		.setkey		= omap_aes_setkey,
		.encrypt	= omap_aes_ctr_encrypt,
		.decrypt	= omap_aes_ctr_decrypt,
	}
} ,
};

static struct omap_aes_algs_info omap_aes_algs_info_ecb_cbc[] = {
	{
		.algs_list	= algs_ecb_cbc,
		.size		= ARRAY_SIZE(algs_ecb_cbc),
	},
};

static const struct omap_aes_pdata omap_aes_pdata_omap2 = {
	.algs_info	= omap_aes_algs_info_ecb_cbc,
	.algs_info_size	= ARRAY_SIZE(omap_aes_algs_info_ecb_cbc),
	.trigger	= omap_aes_dma_trigger_omap2,
	.key_ofs	= 0x1c,
	.iv_ofs		= 0x20,
	.ctrl_ofs	= 0x30,
	.data_ofs	= 0x34,
	.rev_ofs	= 0x44,
	.mask_ofs	= 0x48,
	.dma_enable_in	= BIT(2),
	.dma_enable_out	= BIT(3),
	.dma_start	= BIT(5),
	.major_mask	= 0xf0,
	.major_shift	= 4,
	.minor_mask	= 0x0f,
	.minor_shift	= 0,
};

#ifdef CONFIG_OF
static struct omap_aes_algs_info omap_aes_algs_info_ecb_cbc_ctr[] = {
	{
		.algs_list	= algs_ecb_cbc,
		.size		= ARRAY_SIZE(algs_ecb_cbc),
	},
	{
		.algs_list	= algs_ctr,
		.size		= ARRAY_SIZE(algs_ctr),
	},
};

static const struct omap_aes_pdata omap_aes_pdata_omap3 = {
	.algs_info	= omap_aes_algs_info_ecb_cbc_ctr,
	.algs_info_size	= ARRAY_SIZE(omap_aes_algs_info_ecb_cbc_ctr),
	.trigger	= omap_aes_dma_trigger_omap2,
	.key_ofs	= 0x1c,
	.iv_ofs		= 0x20,
	.ctrl_ofs	= 0x30,
	.data_ofs	= 0x34,
	.rev_ofs	= 0x44,
	.mask_ofs	= 0x48,
	.dma_enable_in	= BIT(2),
	.dma_enable_out	= BIT(3),
	.dma_start	= BIT(5),
	.major_mask	= 0xf0,
	.major_shift	= 4,
	.minor_mask	= 0x0f,
	.minor_shift	= 0,
};

static const struct omap_aes_pdata omap_aes_pdata_omap4 = {
	.algs_info	= omap_aes_algs_info_ecb_cbc_ctr,
	.algs_info_size	= ARRAY_SIZE(omap_aes_algs_info_ecb_cbc_ctr),
	.trigger	= omap_aes_dma_trigger_omap4,
	.key_ofs	= 0x3c,
	.iv_ofs		= 0x40,
	.ctrl_ofs	= 0x50,
	.data_ofs	= 0x60,
	.rev_ofs	= 0x80,
	.mask_ofs	= 0x84,
	.irq_status_ofs = 0x8c,
	.irq_enable_ofs = 0x90,
	.dma_enable_in	= BIT(5),
	.dma_enable_out	= BIT(6),
	.major_mask	= 0x0700,
	.major_shift	= 8,
	.minor_mask	= 0x003f,
	.minor_shift	= 0,
};

static irqreturn_t omap_aes_irq(int irq, void *dev_id)
{
	struct omap_aes_dev *dd = dev_id;
	u32 status, i;
	u32 *src, *dst;

	status = omap_aes_read(dd, AES_REG_IRQ_STATUS(dd));
	if (status & AES_REG_IRQ_DATA_IN) {
		omap_aes_write(dd, AES_REG_IRQ_ENABLE(dd), 0x0);

		BUG_ON(!dd->in_sg);

		BUG_ON(_calc_walked(in) > dd->in_sg->length);

		src = sg_virt(dd->in_sg) + _calc_walked(in);

		for (i = 0; i < AES_BLOCK_WORDS; i++) {
			omap_aes_write(dd, AES_REG_DATA_N(dd, i), *src);

			scatterwalk_advance(&dd->in_walk, 4);
			if (dd->in_sg->length == _calc_walked(in)) {
				dd->in_sg = scatterwalk_sg_next(dd->in_sg);
				if (dd->in_sg) {
					scatterwalk_start(&dd->in_walk,
							  dd->in_sg);
					src = sg_virt(dd->in_sg) +
					      _calc_walked(in);
				}
			} else {
				src++;
			}
		}

		/* Clear IRQ status */
		status &= ~AES_REG_IRQ_DATA_IN;
		omap_aes_write(dd, AES_REG_IRQ_STATUS(dd), status);

		/* Enable DATA_OUT interrupt */
		omap_aes_write(dd, AES_REG_IRQ_ENABLE(dd), 0x4);

	} else if (status & AES_REG_IRQ_DATA_OUT) {
		omap_aes_write(dd, AES_REG_IRQ_ENABLE(dd), 0x0);

		BUG_ON(!dd->out_sg);

		BUG_ON(_calc_walked(out) > dd->out_sg->length);

		dst = sg_virt(dd->out_sg) + _calc_walked(out);

		for (i = 0; i < AES_BLOCK_WORDS; i++) {
			*dst = omap_aes_read(dd, AES_REG_DATA_N(dd, i));
			scatterwalk_advance(&dd->out_walk, 4);
			if (dd->out_sg->length == _calc_walked(out)) {
				dd->out_sg = scatterwalk_sg_next(dd->out_sg);
				if (dd->out_sg) {
					scatterwalk_start(&dd->out_walk,
							  dd->out_sg);
					dst = sg_virt(dd->out_sg) +
					      _calc_walked(out);
				}
			} else {
				dst++;
			}
		}

		dd->total -= AES_BLOCK_SIZE;

		BUG_ON(dd->total < 0);

		/* Clear IRQ status */
		status &= ~AES_REG_IRQ_DATA_OUT;
		omap_aes_write(dd, AES_REG_IRQ_STATUS(dd), status);

		if (!dd->total)
			/* All bytes read! */
			tasklet_schedule(&dd->done_task);
		else
			/* Enable DATA_IN interrupt for next block */
			omap_aes_write(dd, AES_REG_IRQ_ENABLE(dd), 0x2);
	}

	return IRQ_HANDLED;
}

static const struct of_device_id omap_aes_of_match[] = {
	{
		.compatible	= "ti,omap2-aes",
		.data		= &omap_aes_pdata_omap2,
	},
	{
		.compatible	= "ti,omap3-aes",
		.data		= &omap_aes_pdata_omap3,
	},
	{
		.compatible	= "ti,omap4-aes",
		.data		= &omap_aes_pdata_omap4,
	},
	{},
};
MODULE_DEVICE_TABLE(of, omap_aes_of_match);

static int omap_aes_get_res_of(struct omap_aes_dev *dd,
		struct device *dev, struct resource *res)
{
	struct device_node *node = dev->of_node;
	const struct of_device_id *match;
	int err = 0;

	match = of_match_device(of_match_ptr(omap_aes_of_match), dev);
	if (!match) {
		dev_err(dev, "no compatible OF match\n");
		err = -EINVAL;
		goto err;
	}

	err = of_address_to_resource(node, 0, res);
	if (err < 0) {
		dev_err(dev, "can't translate OF node address\n");
		err = -EINVAL;
		goto err;
	}

	dd->dma_out = -1; /* Dummy value that's unused */
	dd->dma_in = -1; /* Dummy value that's unused */

	dd->pdata = match->data;

err:
	return err;
}
#else
static const struct of_device_id omap_aes_of_match[] = {
	{},
};

static int omap_aes_get_res_of(struct omap_aes_dev *dd,
		struct device *dev, struct resource *res)
{
	return -EINVAL;
}
#endif

static int omap_aes_get_res_pdev(struct omap_aes_dev *dd,
		struct platform_device *pdev, struct resource *res)
{
	struct device *dev = &pdev->dev;
	struct resource *r;
	int err = 0;

	/* Get the base address */
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r) {
		dev_err(dev, "no MEM resource info\n");
		err = -ENODEV;
		goto err;
	}
	memcpy(res, r, sizeof(*res));

	/* Get the DMA out channel */
	r = platform_get_resource(pdev, IORESOURCE_DMA, 0);
	if (!r) {
		dev_err(dev, "no DMA out resource info\n");
		err = -ENODEV;
		goto err;
	}
	dd->dma_out = r->start;

	/* Get the DMA in channel */
	r = platform_get_resource(pdev, IORESOURCE_DMA, 1);
	if (!r) {
		dev_err(dev, "no DMA in resource info\n");
		err = -ENODEV;
		goto err;
	}
	dd->dma_in = r->start;

	/* Only OMAP2/3 can be non-DT */
	dd->pdata = &omap_aes_pdata_omap2;

err:
	return err;
}

>>>>>>> refs/remotes/origin/master
static int omap_aes_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct omap_aes_dev *dd;
<<<<<<< HEAD
	struct resource *res;
	int err = -ENOMEM, i, j;
	u32 reg;

	dd = kzalloc(sizeof(struct omap_aes_dev), GFP_KERNEL);
=======
	struct crypto_alg *algp;
	struct resource res;
	int err = -ENOMEM, i, j, irq = -1;
	u32 reg;

	dd = devm_kzalloc(dev, sizeof(struct omap_aes_dev), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (dd == NULL) {
		dev_err(dev, "unable to alloc data struct.\n");
		goto err_data;
	}
	dd->dev = dev;
	platform_set_drvdata(pdev, dd);

	spin_lock_init(&dd->lock);
	crypto_init_queue(&dd->queue, OMAP_AES_QUEUE_LENGTH);

<<<<<<< HEAD
	/* Get the base address */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "invalid resource type\n");
		err = -ENODEV;
		goto err_res;
	}
	dd->phys_base = res->start;

	/* Get the DMA */
	res = platform_get_resource(pdev, IORESOURCE_DMA, 0);
	if (!res)
		dev_info(dev, "no DMA info\n");
	else
		dd->dma_out = res->start;

	/* Get the DMA */
	res = platform_get_resource(pdev, IORESOURCE_DMA, 1);
	if (!res)
		dev_info(dev, "no DMA info\n");
	else
		dd->dma_in = res->start;

	/* Initializing the clock */
	dd->iclk = clk_get(dev, "ick");
	if (IS_ERR(dd->iclk)) {
		dev_err(dev, "clock intialization failed.\n");
		err = PTR_ERR(dd->iclk);
		goto err_res;
	}

	dd->io_base = ioremap(dd->phys_base, SZ_4K);
	if (!dd->io_base) {
		dev_err(dev, "can't ioremap\n");
		err = -ENOMEM;
		goto err_io;
	}

	clk_enable(dd->iclk);
	reg = omap_aes_read(dd, AES_REG_REV);
	dev_info(dev, "OMAP AES hw accel rev: %u.%u\n",
		 (reg & AES_REG_REV_MAJOR) >> 4, reg & AES_REG_REV_MINOR);
	clk_disable(dd->iclk);
=======
	err = (dev->of_node) ? omap_aes_get_res_of(dd, dev, &res) :
			       omap_aes_get_res_pdev(dd, pdev, &res);
	if (err)
		goto err_res;

	dd->io_base = devm_ioremap_resource(dev, &res);
	if (IS_ERR(dd->io_base)) {
		err = PTR_ERR(dd->io_base);
		goto err_res;
	}
	dd->phys_base = res.start;

	pm_runtime_enable(dev);
	pm_runtime_get_sync(dev);

	omap_aes_dma_stop(dd);

	reg = omap_aes_read(dd, AES_REG_REV(dd));

	pm_runtime_put_sync(dev);

	dev_info(dev, "OMAP AES hw accel rev: %u.%u\n",
		 (reg & dd->pdata->major_mask) >> dd->pdata->major_shift,
		 (reg & dd->pdata->minor_mask) >> dd->pdata->minor_shift);
>>>>>>> refs/remotes/origin/master

	tasklet_init(&dd->done_task, omap_aes_done_task, (unsigned long)dd);
	tasklet_init(&dd->queue_task, omap_aes_queue_task, (unsigned long)dd);

	err = omap_aes_dma_init(dd);
<<<<<<< HEAD
	if (err)
		goto err_dma;
=======
	if (err && AES_REG_IRQ_STATUS(dd) && AES_REG_IRQ_ENABLE(dd)) {
		dd->pio_only = 1;

		irq = platform_get_irq(pdev, 0);
		if (irq < 0) {
			dev_err(dev, "can't get IRQ resource\n");
			goto err_irq;
		}

		err = devm_request_irq(dev, irq, omap_aes_irq, 0,
				dev_name(dev), dd);
		if (err) {
			dev_err(dev, "Unable to grab omap-aes IRQ\n");
			goto err_irq;
		}
	}

>>>>>>> refs/remotes/origin/master

	INIT_LIST_HEAD(&dd->list);
	spin_lock(&list_lock);
	list_add_tail(&dd->list, &dev_list);
	spin_unlock(&list_lock);

<<<<<<< HEAD
	for (i = 0; i < ARRAY_SIZE(algs); i++) {
		pr_debug("i: %d\n", i);
		INIT_LIST_HEAD(&algs[i].cra_list);
		err = crypto_register_alg(&algs[i]);
		if (err)
			goto err_algs;
	}

	pr_info("probe() done\n");

	return 0;
err_algs:
	for (j = 0; j < i; j++)
		crypto_unregister_alg(&algs[j]);
	omap_aes_dma_cleanup(dd);
err_dma:
	tasklet_kill(&dd->done_task);
	tasklet_kill(&dd->queue_task);
	iounmap(dd->io_base);
err_io:
	clk_put(dd->iclk);
err_res:
	kfree(dd);
=======
	for (i = 0; i < dd->pdata->algs_info_size; i++) {
		for (j = 0; j < dd->pdata->algs_info[i].size; j++) {
			algp = &dd->pdata->algs_info[i].algs_list[j];

			pr_debug("reg alg: %s\n", algp->cra_name);
			INIT_LIST_HEAD(&algp->cra_list);

			err = crypto_register_alg(algp);
			if (err)
				goto err_algs;

			dd->pdata->algs_info[i].registered++;
		}
	}

	return 0;
err_algs:
	for (i = dd->pdata->algs_info_size - 1; i >= 0; i--)
		for (j = dd->pdata->algs_info[i].registered - 1; j >= 0; j--)
			crypto_unregister_alg(
					&dd->pdata->algs_info[i].algs_list[j]);
	if (!dd->pio_only)
		omap_aes_dma_cleanup(dd);
err_irq:
	tasklet_kill(&dd->done_task);
	tasklet_kill(&dd->queue_task);
	pm_runtime_disable(dev);
err_res:
>>>>>>> refs/remotes/origin/master
	dd = NULL;
err_data:
	dev_err(dev, "initialization failed.\n");
	return err;
}

static int omap_aes_remove(struct platform_device *pdev)
{
	struct omap_aes_dev *dd = platform_get_drvdata(pdev);
<<<<<<< HEAD
	int i;
=======
	int i, j;
>>>>>>> refs/remotes/origin/master

	if (!dd)
		return -ENODEV;

	spin_lock(&list_lock);
	list_del(&dd->list);
	spin_unlock(&list_lock);

<<<<<<< HEAD
	for (i = 0; i < ARRAY_SIZE(algs); i++)
		crypto_unregister_alg(&algs[i]);
=======
	for (i = dd->pdata->algs_info_size - 1; i >= 0; i--)
		for (j = dd->pdata->algs_info[i].registered - 1; j >= 0; j--)
			crypto_unregister_alg(
					&dd->pdata->algs_info[i].algs_list[j]);
>>>>>>> refs/remotes/origin/master

	tasklet_kill(&dd->done_task);
	tasklet_kill(&dd->queue_task);
	omap_aes_dma_cleanup(dd);
<<<<<<< HEAD
	iounmap(dd->io_base);
	clk_put(dd->iclk);
	kfree(dd);
=======
	pm_runtime_disable(dd->dev);
>>>>>>> refs/remotes/origin/master
	dd = NULL;

	return 0;
}

<<<<<<< HEAD
=======
#ifdef CONFIG_PM_SLEEP
static int omap_aes_suspend(struct device *dev)
{
	pm_runtime_put_sync(dev);
	return 0;
}

static int omap_aes_resume(struct device *dev)
{
	pm_runtime_get_sync(dev);
	return 0;
}
#endif

static const struct dev_pm_ops omap_aes_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(omap_aes_suspend, omap_aes_resume)
};

>>>>>>> refs/remotes/origin/master
static struct platform_driver omap_aes_driver = {
	.probe	= omap_aes_probe,
	.remove	= omap_aes_remove,
	.driver	= {
		.name	= "omap-aes",
		.owner	= THIS_MODULE,
<<<<<<< HEAD
	},
};

static int __init omap_aes_mod_init(void)
{
	pr_info("loading %s driver\n", "omap-aes");

	if (!cpu_class_is_omap2() || omap_type() != OMAP2_DEVICE_TYPE_SEC) {
		pr_err("Unsupported cpu\n");
		return -ENODEV;
	}

	return  platform_driver_register(&omap_aes_driver);
}

static void __exit omap_aes_mod_exit(void)
{
	platform_driver_unregister(&omap_aes_driver);
}

module_init(omap_aes_mod_init);
module_exit(omap_aes_mod_exit);
=======
		.pm	= &omap_aes_pm_ops,
		.of_match_table	= omap_aes_of_match,
	},
};

module_platform_driver(omap_aes_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("OMAP AES hw acceleration support.");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Dmitry Kasatkin");

