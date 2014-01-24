/*
 * Fifo-attached Serial Interface (FSI) support for SH7724
 *
 * Copyright (C) 2009 Renesas Solutions Corp.
 * Kuninori Morimoto <morimoto.kuninori@renesas.com>
 *
 * Based on ssi.c
 * Copyright (c) 2007 Manuel Lauss <mano@roarinelk.homelinux.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/pm_runtime.h>
#include <linux/io.h>
#include <linux/slab.h>
=======
#include <linux/dma-mapping.h>
#include <linux/pm_runtime.h>
#include <linux/io.h>
#include <linux/scatterlist.h>
#include <linux/sh_dma.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/workqueue.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <sound/soc.h>
#include <sound/sh_fsi.h>

/* PortA/PortB register */
#define REG_DO_FMT	0x0000
#define REG_DOFF_CTL	0x0004
#define REG_DOFF_ST	0x0008
#define REG_DI_FMT	0x000C
#define REG_DIFF_CTL	0x0010
#define REG_DIFF_ST	0x0014
#define REG_CKG1	0x0018
#define REG_CKG2	0x001C
#define REG_DIDT	0x0020
#define REG_DODT	0x0024
#define REG_MUTE_ST	0x0028
<<<<<<< HEAD
#define REG_OUT_SEL	0x0030
=======
#define REG_OUT_DMAC	0x002C
#define REG_OUT_SEL	0x0030
#define REG_IN_DMAC	0x0038
>>>>>>> refs/remotes/origin/cm-10.0

/* master register */
#define MST_CLK_RST	0x0210
#define MST_SOFT_RST	0x0214
#define MST_FIFO_SZ	0x0218

/* core register (depend on FSI version) */
#define A_MST_CTLR	0x0180
#define B_MST_CTLR	0x01A0
#define CPU_INT_ST	0x01F4
#define CPU_IEMSK	0x01F8
#define CPU_IMSK	0x01FC
#define INT_ST		0x0200
#define IEMSK		0x0204
#define IMSK		0x0208

/* DO_FMT */
/* DI_FMT */
<<<<<<< HEAD
=======
#define CR_BWS_MASK	(0x3 << 20) /* FSI2 */
>>>>>>> refs/remotes/origin/cm-10.0
#define CR_BWS_24	(0x0 << 20) /* FSI2 */
#define CR_BWS_16	(0x1 << 20) /* FSI2 */
#define CR_BWS_20	(0x2 << 20) /* FSI2 */

#define CR_DTMD_PCM		(0x0 << 8) /* FSI2 */
#define CR_DTMD_SPDIF_PCM	(0x1 << 8) /* FSI2 */
#define CR_DTMD_SPDIF_STREAM	(0x2 << 8) /* FSI2 */

#define CR_MONO		(0x0 << 4)
#define CR_MONO_D	(0x1 << 4)
#define CR_PCM		(0x2 << 4)
#define CR_I2S		(0x3 << 4)
#define CR_TDM		(0x4 << 4)
#define CR_TDM_D	(0x5 << 4)

<<<<<<< HEAD
=======
/* OUT_DMAC */
/* IN_DMAC */
#define VDMD_MASK	(0x3 << 4)
#define VDMD_FRONT	(0x0 << 4) /* Package in front */
#define VDMD_BACK	(0x1 << 4) /* Package in back */
#define VDMD_STREAM	(0x2 << 4) /* Stream mode(16bit * 2) */

#define DMA_ON		(0x1 << 0)

>>>>>>> refs/remotes/origin/cm-10.0
/* DOFF_CTL */
/* DIFF_CTL */
#define IRQ_HALF	0x00100000
#define FIFO_CLR	0x00000001

/* DOFF_ST */
#define ERR_OVER	0x00000010
#define ERR_UNDER	0x00000001
#define ST_ERR		(ERR_OVER | ERR_UNDER)

/* CKG1 */
#define ACKMD_MASK	0x00007000
#define BPFMD_MASK	0x00000700
#define DIMD		(1 << 4)
#define DOMD		(1 << 0)

/* A/B MST_CTLR */
#define BP	(1 << 4)	/* Fix the signal of Biphase output */
#define SE	(1 << 0)	/* Fix the master clock */

/* CLK_RST */
#define CRB	(1 << 4)
#define CRA	(1 << 0)

/* IO SHIFT / MACRO */
#define BI_SHIFT	12
#define BO_SHIFT	8
#define AI_SHIFT	4
#define AO_SHIFT	0
#define AB_IO(param, shift)	(param << shift)

/* SOFT_RST */
#define PBSR		(1 << 12) /* Port B Software Reset */
#define PASR		(1 <<  8) /* Port A Software Reset */
#define IR		(1 <<  4) /* Interrupt Reset */
#define FSISR		(1 <<  0) /* Software Reset */

/* OUT_SEL (FSI2) */
#define DMMD		(1 << 4) /* SPDIF output timing 0: Biphase only */
				 /*			1: Biphase and serial */

/* FIFO_SZ */
#define FIFO_SZ_MASK	0x7

#define FSI_RATES SNDRV_PCM_RATE_8000_96000

#define FSI_FMTS (SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S16_LE)

<<<<<<< HEAD
typedef int (*set_rate_func)(struct device *dev, int is_porta, int rate, int enable);
=======
typedef int (*set_rate_func)(struct device *dev, int rate, int enable);
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * FSI driver use below type name for variable
 *
<<<<<<< HEAD
 * xxx_len	: data length
 * xxx_width	: data width
 * xxx_offset	: data offset
 * xxx_num	: number of data
 */

/*
 *		struct
 */

struct fsi_stream {
	struct snd_pcm_substream *substream;

	int fifo_max_num;

	int buff_offset;
	int buff_len;
	int period_len;
	int period_num;

	int uerr_num;
	int oerr_num;
=======
 * xxx_num	: number of data
 * xxx_pos	: position of data
 * xxx_capa	: capacity of data
 */

/*
 *	period/frame/sample image
 *
 * ex) PCM (2ch)
 *
 * period pos					   period pos
 *   [n]					     [n + 1]
 *   |<-------------------- period--------------------->|
 * ==|============================================ ... =|==
 *   |							|
 *   ||<-----  frame ----->|<------ frame ----->|  ...	|
 *   |+--------------------+--------------------+- ...	|
 *   ||[ sample ][ sample ]|[ sample ][ sample ]|  ...	|
 *   |+--------------------+--------------------+- ...	|
 * ==|============================================ ... =|==
 */

/*
 *	FSI FIFO image
 *
 *	|	     |
 *	|	     |
 *	| [ sample ] |
 *	| [ sample ] |
 *	| [ sample ] |
 *	| [ sample ] |
 *		--> go to codecs
 */

/*
 *		struct
 */

struct fsi_stream_handler;
struct fsi_stream {

	/*
	 * these are initialized by fsi_stream_init()
	 */
	struct snd_pcm_substream *substream;
	int fifo_sample_capa;	/* sample capacity of FSI FIFO */
	int buff_sample_capa;	/* sample capacity of ALSA buffer */
	int buff_sample_pos;	/* sample position of ALSA buffer */
	int period_samples;	/* sample number / 1 period */
	int period_pos;		/* current period position */
	int sample_width;	/* sample width */
	int uerr_num;
	int oerr_num;

	/*
	 * thse are initialized by fsi_handler_init()
	 */
	struct fsi_stream_handler *handler;
	struct fsi_priv		*priv;

	/*
	 * these are for DMAEngine
	 */
	struct dma_chan		*chan;
	struct sh_dmae_slave	slave; /* see fsi_handler_init() */
	struct work_struct	work;
	dma_addr_t		dma;
>>>>>>> refs/remotes/origin/cm-10.0
};

struct fsi_priv {
	void __iomem *base;
	struct fsi_master *master;
<<<<<<< HEAD
=======
	struct sh_fsi_port_info *info;
>>>>>>> refs/remotes/origin/cm-10.0

	struct fsi_stream playback;
	struct fsi_stream capture;

<<<<<<< HEAD
	int chan_num:16;
	int clk_master:1;

	long rate;

	/* for suspend/resume */
	u32 saved_do_fmt;
	u32 saved_di_fmt;
	u32 saved_ckg1;
	u32 saved_ckg2;
	u32 saved_out_sel;
};
=======
	u32 do_fmt;
	u32 di_fmt;

	int chan_num:16;
	int clk_master:1;
	int spdif:1;

	long rate;
};

struct fsi_stream_handler {
	int (*init)(struct fsi_priv *fsi, struct fsi_stream *io);
	int (*quit)(struct fsi_priv *fsi, struct fsi_stream *io);
	int (*probe)(struct fsi_priv *fsi, struct fsi_stream *io);
	int (*transfer)(struct fsi_priv *fsi, struct fsi_stream *io);
	int (*remove)(struct fsi_priv *fsi, struct fsi_stream *io);
	void (*start_stop)(struct fsi_priv *fsi, struct fsi_stream *io,
			   int enable);
};
#define fsi_stream_handler_call(io, func, args...)	\
	(!(io) ? -ENODEV :				\
	 !((io)->handler->func) ? 0 :			\
	 (io)->handler->func(args))
>>>>>>> refs/remotes/origin/cm-10.0

struct fsi_core {
	int ver;

	u32 int_st;
	u32 iemsk;
	u32 imsk;
	u32 a_mclk;
	u32 b_mclk;
};

struct fsi_master {
	void __iomem *base;
	int irq;
	struct fsi_priv fsia;
	struct fsi_priv fsib;
	struct fsi_core *core;
<<<<<<< HEAD
	struct sh_fsi_platform_info *info;
	spinlock_t lock;

	/* for suspend/resume */
	u32 saved_a_mclk;
	u32 saved_b_mclk;
	u32 saved_iemsk;
	u32 saved_imsk;
	u32 saved_clk_rst;
	u32 saved_soft_rst;
};

=======
	spinlock_t lock;
};

static int fsi_stream_is_play(struct fsi_priv *fsi, struct fsi_stream *io);

>>>>>>> refs/remotes/origin/cm-10.0
/*
 *		basic read write function
 */

<<<<<<< HEAD
static void __fsi_reg_write(u32 reg, u32 data)
=======
static void __fsi_reg_write(u32 __iomem *reg, u32 data)
>>>>>>> refs/remotes/origin/cm-10.0
{
	/* valid data area is 24bit */
	data &= 0x00ffffff;

	__raw_writel(data, reg);
}

<<<<<<< HEAD
static u32 __fsi_reg_read(u32 reg)
=======
static u32 __fsi_reg_read(u32 __iomem *reg)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return __raw_readl(reg);
}

<<<<<<< HEAD
static void __fsi_reg_mask_set(u32 reg, u32 mask, u32 data)
=======
static void __fsi_reg_mask_set(u32 __iomem *reg, u32 mask, u32 data)
>>>>>>> refs/remotes/origin/cm-10.0
{
	u32 val = __fsi_reg_read(reg);

	val &= ~mask;
	val |= data & mask;

	__fsi_reg_write(reg, val);
}

#define fsi_reg_write(p, r, d)\
<<<<<<< HEAD
	__fsi_reg_write((u32)(p->base + REG_##r), d)

#define fsi_reg_read(p, r)\
	__fsi_reg_read((u32)(p->base + REG_##r))

#define fsi_reg_mask_set(p, r, m, d)\
	__fsi_reg_mask_set((u32)(p->base + REG_##r), m, d)
=======
	__fsi_reg_write((p->base + REG_##r), d)

#define fsi_reg_read(p, r)\
	__fsi_reg_read((p->base + REG_##r))

#define fsi_reg_mask_set(p, r, m, d)\
	__fsi_reg_mask_set((p->base + REG_##r), m, d)
>>>>>>> refs/remotes/origin/cm-10.0

#define fsi_master_read(p, r) _fsi_master_read(p, MST_##r)
#define fsi_core_read(p, r)   _fsi_master_read(p, p->core->r)
static u32 _fsi_master_read(struct fsi_master *master, u32 reg)
{
	u32 ret;
	unsigned long flags;

	spin_lock_irqsave(&master->lock, flags);
<<<<<<< HEAD
	ret = __fsi_reg_read((u32)(master->base + reg));
=======
	ret = __fsi_reg_read(master->base + reg);
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock_irqrestore(&master->lock, flags);

	return ret;
}

#define fsi_master_mask_set(p, r, m, d) _fsi_master_mask_set(p, MST_##r, m, d)
#define fsi_core_mask_set(p, r, m, d)  _fsi_master_mask_set(p, p->core->r, m, d)
static void _fsi_master_mask_set(struct fsi_master *master,
			       u32 reg, u32 mask, u32 data)
{
	unsigned long flags;

	spin_lock_irqsave(&master->lock, flags);
<<<<<<< HEAD
	__fsi_reg_mask_set((u32)(master->base + reg), mask, data);
=======
	__fsi_reg_mask_set(master->base + reg, mask, data);
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock_irqrestore(&master->lock, flags);
}

/*
 *		basic function
 */

static struct fsi_master *fsi_get_master(struct fsi_priv *fsi)
{
	return fsi->master;
}

static int fsi_is_clk_master(struct fsi_priv *fsi)
{
	return fsi->clk_master;
}

static int fsi_is_port_a(struct fsi_priv *fsi)
{
	return fsi->master->base == fsi->base;
}

<<<<<<< HEAD
=======
static int fsi_is_spdif(struct fsi_priv *fsi)
{
	return fsi->spdif;
}

static int fsi_is_play(struct snd_pcm_substream *substream)
{
	return substream->stream == SNDRV_PCM_STREAM_PLAYBACK;
}

>>>>>>> refs/remotes/origin/cm-10.0
static struct snd_soc_dai *fsi_get_dai(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;

	return  rtd->cpu_dai;
}

static struct fsi_priv *fsi_get_priv_frm_dai(struct snd_soc_dai *dai)
{
	struct fsi_master *master = snd_soc_dai_get_drvdata(dai);

	if (dai->id == 0)
		return &master->fsia;
	else
		return &master->fsib;
}

static struct fsi_priv *fsi_get_priv(struct snd_pcm_substream *substream)
{
	return fsi_get_priv_frm_dai(fsi_get_dai(substream));
}

<<<<<<< HEAD
static set_rate_func fsi_get_info_set_rate(struct fsi_master *master)
{
	if (!master->info)
		return NULL;

	return master->info->set_rate;
=======
static set_rate_func fsi_get_info_set_rate(struct fsi_priv *fsi)
{
	if (!fsi->info)
		return NULL;

	return fsi->info->set_rate;
>>>>>>> refs/remotes/origin/cm-10.0
}

static u32 fsi_get_info_flags(struct fsi_priv *fsi)
{
<<<<<<< HEAD
	int is_porta = fsi_is_port_a(fsi);
	struct fsi_master *master = fsi_get_master(fsi);

	if (!master->info)
		return 0;

	return is_porta ? master->info->porta_flags :
		master->info->portb_flags;
}

static inline int fsi_stream_is_play(int stream)
{
	return stream == SNDRV_PCM_STREAM_PLAYBACK;
}

static inline int fsi_is_play(struct snd_pcm_substream *substream)
{
	return fsi_stream_is_play(substream->stream);
}

static inline struct fsi_stream *fsi_get_stream(struct fsi_priv *fsi,
						int is_play)
{
	return is_play ? &fsi->playback : &fsi->capture;
}

static u32 fsi_get_port_shift(struct fsi_priv *fsi, int is_play)
{
=======
	if (!fsi->info)
		return 0;

	return fsi->info->flags;
}

static u32 fsi_get_port_shift(struct fsi_priv *fsi, struct fsi_stream *io)
{
	int is_play = fsi_stream_is_play(fsi, io);
>>>>>>> refs/remotes/origin/cm-10.0
	int is_porta = fsi_is_port_a(fsi);
	u32 shift;

	if (is_porta)
		shift = is_play ? AO_SHIFT : AI_SHIFT;
	else
		shift = is_play ? BO_SHIFT : BI_SHIFT;

	return shift;
}

<<<<<<< HEAD
static void fsi_stream_push(struct fsi_priv *fsi,
			    int is_play,
			    struct snd_pcm_substream *substream,
			    u32 buffer_len,
			    u32 period_len)
{
	struct fsi_stream *io = fsi_get_stream(fsi, is_play);

	io->substream	= substream;
	io->buff_len	= buffer_len;
	io->buff_offset	= 0;
	io->period_len	= period_len;
	io->period_num	= 0;
	io->oerr_num	= -1; /* ignore 1st err */
	io->uerr_num	= -1; /* ignore 1st err */
}

static void fsi_stream_pop(struct fsi_priv *fsi, int is_play)
{
	struct fsi_stream *io = fsi_get_stream(fsi, is_play);
	struct snd_soc_dai *dai = fsi_get_dai(io->substream);


	if (io->oerr_num > 0)
		dev_err(dai->dev, "over_run = %d\n", io->oerr_num);

	if (io->uerr_num > 0)
		dev_err(dai->dev, "under_run = %d\n", io->uerr_num);

	io->substream	= NULL;
	io->buff_len	= 0;
	io->buff_offset	= 0;
	io->period_len	= 0;
	io->period_num	= 0;
	io->oerr_num	= 0;
	io->uerr_num	= 0;
}

static int fsi_get_fifo_data_num(struct fsi_priv *fsi, int is_play)
{
	u32 status;
	int data_num;
=======
static int fsi_frame2sample(struct fsi_priv *fsi, int frames)
{
	return frames * fsi->chan_num;
}

static int fsi_sample2frame(struct fsi_priv *fsi, int samples)
{
	return samples / fsi->chan_num;
}

static int fsi_get_current_fifo_samples(struct fsi_priv *fsi,
					struct fsi_stream *io)
{
	int is_play = fsi_stream_is_play(fsi, io);
	u32 status;
	int frames;
>>>>>>> refs/remotes/origin/cm-10.0

	status = is_play ?
		fsi_reg_read(fsi, DOFF_ST) :
		fsi_reg_read(fsi, DIFF_ST);

<<<<<<< HEAD
	data_num = 0x1ff & (status >> 8);
	data_num *= fsi->chan_num;

	return data_num;
}

static int fsi_len2num(int len, int width)
{
	return len / width;
}

#define fsi_num2offset(a, b) fsi_num2len(a, b)
static int fsi_num2len(int num, int width)
{
	return num * width;
}

static int fsi_get_frame_width(struct fsi_priv *fsi, int is_play)
{
	struct fsi_stream *io = fsi_get_stream(fsi, is_play);
	struct snd_pcm_substream *substream = io->substream;
	struct snd_pcm_runtime *runtime = substream->runtime;

	return frames_to_bytes(runtime, 1) / fsi->chan_num;
=======
	frames = 0x1ff & (status >> 8);

	return fsi_frame2sample(fsi, frames);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void fsi_count_fifo_err(struct fsi_priv *fsi)
{
	u32 ostatus = fsi_reg_read(fsi, DOFF_ST);
	u32 istatus = fsi_reg_read(fsi, DIFF_ST);

	if (ostatus & ERR_OVER)
		fsi->playback.oerr_num++;

	if (ostatus & ERR_UNDER)
		fsi->playback.uerr_num++;

	if (istatus & ERR_OVER)
		fsi->capture.oerr_num++;

	if (istatus & ERR_UNDER)
		fsi->capture.uerr_num++;

	fsi_reg_write(fsi, DOFF_ST, 0);
	fsi_reg_write(fsi, DIFF_ST, 0);
}

/*
<<<<<<< HEAD
 *		dma function
 */

static u8 *fsi_dma_get_area(struct fsi_priv *fsi, int stream)
{
	int is_play = fsi_stream_is_play(stream);
	struct fsi_stream *io = fsi_get_stream(fsi, is_play);

	return io->substream->runtime->dma_area + io->buff_offset;
}

static void fsi_dma_soft_push16(struct fsi_priv *fsi, int num)
{
	u16 *start;
	int i;

	start  = (u16 *)fsi_dma_get_area(fsi, SNDRV_PCM_STREAM_PLAYBACK);

	for (i = 0; i < num; i++)
		fsi_reg_write(fsi, DODT, ((u32)*(start + i) << 8));
}

static void fsi_dma_soft_pop16(struct fsi_priv *fsi, int num)
{
	u16 *start;
	int i;

	start  = (u16 *)fsi_dma_get_area(fsi, SNDRV_PCM_STREAM_CAPTURE);


	for (i = 0; i < num; i++)
		*(start + i) = (u16)(fsi_reg_read(fsi, DIDT) >> 8);
}

static void fsi_dma_soft_push32(struct fsi_priv *fsi, int num)
{
	u32 *start;
	int i;

	start  = (u32 *)fsi_dma_get_area(fsi, SNDRV_PCM_STREAM_PLAYBACK);


	for (i = 0; i < num; i++)
		fsi_reg_write(fsi, DODT, *(start + i));
}

static void fsi_dma_soft_pop32(struct fsi_priv *fsi, int num)
{
	u32 *start;
	int i;

	start  = (u32 *)fsi_dma_get_area(fsi, SNDRV_PCM_STREAM_CAPTURE);

	for (i = 0; i < num; i++)
		*(start + i) = fsi_reg_read(fsi, DIDT);
=======
 *		fsi_stream_xx() function
 */
static inline int fsi_stream_is_play(struct fsi_priv *fsi,
				     struct fsi_stream *io)
{
	return &fsi->playback == io;
}

static inline struct fsi_stream *fsi_stream_get(struct fsi_priv *fsi,
					struct snd_pcm_substream *substream)
{
	return fsi_is_play(substream) ? &fsi->playback : &fsi->capture;
}

static int fsi_stream_is_working(struct fsi_priv *fsi,
				 struct fsi_stream *io)
{
	struct fsi_master *master = fsi_get_master(fsi);
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&master->lock, flags);
	ret = !!(io->substream && io->substream->runtime);
	spin_unlock_irqrestore(&master->lock, flags);

	return ret;
}

static struct fsi_priv *fsi_stream_to_priv(struct fsi_stream *io)
{
	return io->priv;
}

static void fsi_stream_init(struct fsi_priv *fsi,
			    struct fsi_stream *io,
			    struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct fsi_master *master = fsi_get_master(fsi);
	unsigned long flags;

	spin_lock_irqsave(&master->lock, flags);
	io->substream	= substream;
	io->buff_sample_capa	= fsi_frame2sample(fsi, runtime->buffer_size);
	io->buff_sample_pos	= 0;
	io->period_samples	= fsi_frame2sample(fsi, runtime->period_size);
	io->period_pos		= 0;
	io->sample_width	= samples_to_bytes(runtime, 1);
	io->oerr_num	= -1; /* ignore 1st err */
	io->uerr_num	= -1; /* ignore 1st err */
	fsi_stream_handler_call(io, init, fsi, io);
	spin_unlock_irqrestore(&master->lock, flags);
}

static void fsi_stream_quit(struct fsi_priv *fsi, struct fsi_stream *io)
{
	struct snd_soc_dai *dai = fsi_get_dai(io->substream);
	struct fsi_master *master = fsi_get_master(fsi);
	unsigned long flags;

	spin_lock_irqsave(&master->lock, flags);

	if (io->oerr_num > 0)
		dev_err(dai->dev, "over_run = %d\n", io->oerr_num);

	if (io->uerr_num > 0)
		dev_err(dai->dev, "under_run = %d\n", io->uerr_num);

	fsi_stream_handler_call(io, quit, fsi, io);
	io->substream	= NULL;
	io->buff_sample_capa	= 0;
	io->buff_sample_pos	= 0;
	io->period_samples	= 0;
	io->period_pos		= 0;
	io->sample_width	= 0;
	io->oerr_num	= 0;
	io->uerr_num	= 0;
	spin_unlock_irqrestore(&master->lock, flags);
}

static int fsi_stream_transfer(struct fsi_stream *io)
{
	struct fsi_priv *fsi = fsi_stream_to_priv(io);
	if (!fsi)
		return -EIO;

	return fsi_stream_handler_call(io, transfer, fsi, io);
}

#define fsi_stream_start(fsi, io)\
	fsi_stream_handler_call(io, start_stop, fsi, io, 1)

#define fsi_stream_stop(fsi, io)\
	fsi_stream_handler_call(io, start_stop, fsi, io, 0)

static int fsi_stream_probe(struct fsi_priv *fsi)
{
	struct fsi_stream *io;
	int ret1, ret2;

	io = &fsi->playback;
	ret1 = fsi_stream_handler_call(io, probe, fsi, io);

	io = &fsi->capture;
	ret2 = fsi_stream_handler_call(io, probe, fsi, io);

	if (ret1 < 0)
		return ret1;
	if (ret2 < 0)
		return ret2;

	return 0;
}

static int fsi_stream_remove(struct fsi_priv *fsi)
{
	struct fsi_stream *io;
	int ret1, ret2;

	io = &fsi->playback;
	ret1 = fsi_stream_handler_call(io, remove, fsi, io);

	io = &fsi->capture;
	ret2 = fsi_stream_handler_call(io, remove, fsi, io);

	if (ret1 < 0)
		return ret1;
	if (ret2 < 0)
		return ret2;

	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 *		irq function
 */

<<<<<<< HEAD
static void fsi_irq_enable(struct fsi_priv *fsi, int is_play)
{
	u32 data = AB_IO(1, fsi_get_port_shift(fsi, is_play));
=======
static void fsi_irq_enable(struct fsi_priv *fsi, struct fsi_stream *io)
{
	u32 data = AB_IO(1, fsi_get_port_shift(fsi, io));
>>>>>>> refs/remotes/origin/cm-10.0
	struct fsi_master *master = fsi_get_master(fsi);

	fsi_core_mask_set(master, imsk,  data, data);
	fsi_core_mask_set(master, iemsk, data, data);
}

<<<<<<< HEAD
static void fsi_irq_disable(struct fsi_priv *fsi, int is_play)
{
	u32 data = AB_IO(1, fsi_get_port_shift(fsi, is_play));
=======
static void fsi_irq_disable(struct fsi_priv *fsi, struct fsi_stream *io)
{
	u32 data = AB_IO(1, fsi_get_port_shift(fsi, io));
>>>>>>> refs/remotes/origin/cm-10.0
	struct fsi_master *master = fsi_get_master(fsi);

	fsi_core_mask_set(master, imsk,  data, 0);
	fsi_core_mask_set(master, iemsk, data, 0);
}

static u32 fsi_irq_get_status(struct fsi_master *master)
{
	return fsi_core_read(master, int_st);
}

static void fsi_irq_clear_status(struct fsi_priv *fsi)
{
	u32 data = 0;
	struct fsi_master *master = fsi_get_master(fsi);

<<<<<<< HEAD
	data |= AB_IO(1, fsi_get_port_shift(fsi, 0));
	data |= AB_IO(1, fsi_get_port_shift(fsi, 1));
=======
	data |= AB_IO(1, fsi_get_port_shift(fsi, &fsi->playback));
	data |= AB_IO(1, fsi_get_port_shift(fsi, &fsi->capture));
>>>>>>> refs/remotes/origin/cm-10.0

	/* clear interrupt factor */
	fsi_core_mask_set(master, int_st, data, 0);
}

<<<<<<< HEAD
/*
 *		SPDIF master clock function
 *
 * These functions are used later FSI2
 */
static void fsi_spdif_clk_ctrl(struct fsi_priv *fsi, int enable)
{
	struct fsi_master *master = fsi_get_master(fsi);
	u32 mask, val;

	if (master->core->ver < 2) {
		pr_err("fsi: register access err (%s)\n", __func__);
		return;
	}

	mask = BP | SE;
	val = enable ? mask : 0;

	fsi_is_port_a(fsi) ?
		fsi_core_mask_set(master, a_mclk, mask, val) :
		fsi_core_mask_set(master, b_mclk, mask, val);
}

/*
 *		clock function
 */
#define fsi_module_init(m, d)	__fsi_module_clk_ctrl(m, d, 1)
#define fsi_module_kill(m, d)	__fsi_module_clk_ctrl(m, d, 0)
static void __fsi_module_clk_ctrl(struct fsi_master *master,
				  struct device *dev,
				  int enable)
{
	pm_runtime_get_sync(dev);

	if (enable) {
		/* enable only SR */
		fsi_master_mask_set(master, SOFT_RST, FSISR, FSISR);
		fsi_master_mask_set(master, SOFT_RST, PASR | PBSR, 0);
	} else {
		/* clear all registers */
		fsi_master_mask_set(master, SOFT_RST, FSISR, 0);
	}

	pm_runtime_put_sync(dev);
}

#define fsi_port_start(f)	__fsi_port_clk_ctrl(f, 1)
#define fsi_port_stop(f)	__fsi_port_clk_ctrl(f, 0)
static void __fsi_port_clk_ctrl(struct fsi_priv *fsi, int enable)
{
	struct fsi_master *master = fsi_get_master(fsi);
	u32 soft = fsi_is_port_a(fsi) ? PASR : PBSR;
	u32 clk  = fsi_is_port_a(fsi) ? CRA  : CRB;
	int is_master = fsi_is_clk_master(fsi);

	fsi_master_mask_set(master, SOFT_RST, soft, (enable) ? soft : 0);
	if (is_master)
		fsi_master_mask_set(master, CLK_RST, clk, (enable) ? clk : 0);
}

/*
 *		ctrl function
 */
static void fsi_fifo_init(struct fsi_priv *fsi,
			  int is_play,
			  struct snd_soc_dai *dai)
{
	struct fsi_master *master = fsi_get_master(fsi);
	struct fsi_stream *io = fsi_get_stream(fsi, is_play);
	u32 shift, i;

	/* get on-chip RAM capacity */
	shift = fsi_master_read(master, FIFO_SZ);
	shift >>= fsi_get_port_shift(fsi, is_play);
	shift &= FIFO_SZ_MASK;
	io->fifo_max_num = 256 << shift;
	dev_dbg(dai->dev, "fifo = %d words\n", io->fifo_max_num);
=======
/*
 *		SPDIF master clock function
 *
 * These functions are used later FSI2
 */
static void fsi_spdif_clk_ctrl(struct fsi_priv *fsi, int enable)
{
	struct fsi_master *master = fsi_get_master(fsi);
	u32 mask, val;

	if (master->core->ver < 2) {
		pr_err("fsi: register access err (%s)\n", __func__);
		return;
	}

	mask = BP | SE;
	val = enable ? mask : 0;

	fsi_is_port_a(fsi) ?
		fsi_core_mask_set(master, a_mclk, mask, val) :
		fsi_core_mask_set(master, b_mclk, mask, val);
}

/*
 *		clock function
 */
static int fsi_set_master_clk(struct device *dev, struct fsi_priv *fsi,
			      long rate, int enable)
{
	struct fsi_master *master = fsi_get_master(fsi);
	set_rate_func set_rate = fsi_get_info_set_rate(fsi);
	int fsi_ver = master->core->ver;
	int ret;

	if (!set_rate)
		return 0;

	ret = set_rate(dev, rate, enable);
	if (ret < 0) /* error */
		return ret;

	if (!enable)
		return 0;

	if (ret > 0) {
		u32 data = 0;

		switch (ret & SH_FSI_ACKMD_MASK) {
		default:
			/* FALL THROUGH */
		case SH_FSI_ACKMD_512:
			data |= (0x0 << 12);
			break;
		case SH_FSI_ACKMD_256:
			data |= (0x1 << 12);
			break;
		case SH_FSI_ACKMD_128:
			data |= (0x2 << 12);
			break;
		case SH_FSI_ACKMD_64:
			data |= (0x3 << 12);
			break;
		case SH_FSI_ACKMD_32:
			if (fsi_ver < 2)
				dev_err(dev, "unsupported ACKMD\n");
			else
				data |= (0x4 << 12);
			break;
		}

		switch (ret & SH_FSI_BPFMD_MASK) {
		default:
			/* FALL THROUGH */
		case SH_FSI_BPFMD_32:
			data |= (0x0 << 8);
			break;
		case SH_FSI_BPFMD_64:
			data |= (0x1 << 8);
			break;
		case SH_FSI_BPFMD_128:
			data |= (0x2 << 8);
			break;
		case SH_FSI_BPFMD_256:
			data |= (0x3 << 8);
			break;
		case SH_FSI_BPFMD_512:
			data |= (0x4 << 8);
			break;
		case SH_FSI_BPFMD_16:
			if (fsi_ver < 2)
				dev_err(dev, "unsupported ACKMD\n");
			else
				data |= (0x7 << 8);
			break;
		}

		fsi_reg_mask_set(fsi, CKG1, (ACKMD_MASK | BPFMD_MASK) , data);
		udelay(10);
		ret = 0;
	}

	return ret;
}

/*
 *		pio data transfer handler
 */
static void fsi_pio_push16(struct fsi_priv *fsi, u8 *_buf, int samples)
{
	u16 *buf = (u16 *)_buf;
	int i;

	for (i = 0; i < samples; i++)
		fsi_reg_write(fsi, DODT, ((u32)*(buf + i) << 8));
}

static void fsi_pio_pop16(struct fsi_priv *fsi, u8 *_buf, int samples)
{
	u16 *buf = (u16 *)_buf;
	int i;

	for (i = 0; i < samples; i++)
		*(buf + i) = (u16)(fsi_reg_read(fsi, DIDT) >> 8);
}

static void fsi_pio_push32(struct fsi_priv *fsi, u8 *_buf, int samples)
{
	u32 *buf = (u32 *)_buf;
	int i;

	for (i = 0; i < samples; i++)
		fsi_reg_write(fsi, DODT, *(buf + i));
}

static void fsi_pio_pop32(struct fsi_priv *fsi, u8 *_buf, int samples)
{
	u32 *buf = (u32 *)_buf;
	int i;

	for (i = 0; i < samples; i++)
		*(buf + i) = fsi_reg_read(fsi, DIDT);
}

static u8 *fsi_pio_get_area(struct fsi_priv *fsi, struct fsi_stream *io)
{
	struct snd_pcm_runtime *runtime = io->substream->runtime;

	return runtime->dma_area +
		samples_to_bytes(runtime, io->buff_sample_pos);
}

static int fsi_pio_transfer(struct fsi_priv *fsi, struct fsi_stream *io,
		void (*run16)(struct fsi_priv *fsi, u8 *buf, int samples),
		void (*run32)(struct fsi_priv *fsi, u8 *buf, int samples),
		int samples)
{
	struct snd_pcm_runtime *runtime;
	struct snd_pcm_substream *substream;
	u8 *buf;
	int over_period;

	if (!fsi_stream_is_working(fsi, io))
		return -EINVAL;

	over_period	= 0;
	substream	= io->substream;
	runtime		= substream->runtime;

	/* FSI FIFO has limit.
	 * So, this driver can not send periods data at a time
	 */
	if (io->buff_sample_pos >=
	    io->period_samples * (io->period_pos + 1)) {

		over_period = 1;
		io->period_pos = (io->period_pos + 1) % runtime->periods;

		if (0 == io->period_pos)
			io->buff_sample_pos = 0;
	}

	buf = fsi_pio_get_area(fsi, io);

	switch (io->sample_width) {
	case 2:
		run16(fsi, buf, samples);
		break;
	case 4:
		run32(fsi, buf, samples);
		break;
	default:
		return -EINVAL;
	}

	/* update buff_sample_pos */
	io->buff_sample_pos += samples;

	if (over_period)
		snd_pcm_period_elapsed(substream);

	return 0;
}

static int fsi_pio_pop(struct fsi_priv *fsi, struct fsi_stream *io)
{
	int sample_residues;	/* samples in FSI fifo */
	int sample_space;	/* ALSA free samples space */
	int samples;

	sample_residues	= fsi_get_current_fifo_samples(fsi, io);
	sample_space	= io->buff_sample_capa - io->buff_sample_pos;

	samples = min(sample_residues, sample_space);

	return fsi_pio_transfer(fsi, io,
				  fsi_pio_pop16,
				  fsi_pio_pop32,
				  samples);
}

static int fsi_pio_push(struct fsi_priv *fsi, struct fsi_stream *io)
{
	int sample_residues;	/* ALSA residue samples */
	int sample_space;	/* FSI fifo free samples space */
	int samples;

	sample_residues	= io->buff_sample_capa - io->buff_sample_pos;
	sample_space	= io->fifo_sample_capa -
		fsi_get_current_fifo_samples(fsi, io);

	samples = min(sample_residues, sample_space);

	return fsi_pio_transfer(fsi, io,
				  fsi_pio_push16,
				  fsi_pio_push32,
				  samples);
}

static void fsi_pio_start_stop(struct fsi_priv *fsi, struct fsi_stream *io,
			       int enable)
{
	struct fsi_master *master = fsi_get_master(fsi);
	u32 clk  = fsi_is_port_a(fsi) ? CRA  : CRB;

	if (enable)
		fsi_irq_enable(fsi, io);
	else
		fsi_irq_disable(fsi, io);

	if (fsi_is_clk_master(fsi))
		fsi_master_mask_set(master, CLK_RST, clk, (enable) ? clk : 0);
}

static struct fsi_stream_handler fsi_pio_push_handler = {
	.transfer	= fsi_pio_push,
	.start_stop	= fsi_pio_start_stop,
};

static struct fsi_stream_handler fsi_pio_pop_handler = {
	.transfer	= fsi_pio_pop,
	.start_stop	= fsi_pio_start_stop,
};

static irqreturn_t fsi_interrupt(int irq, void *data)
{
	struct fsi_master *master = data;
	u32 int_st = fsi_irq_get_status(master);

	/* clear irq status */
	fsi_master_mask_set(master, SOFT_RST, IR, 0);
	fsi_master_mask_set(master, SOFT_RST, IR, IR);

	if (int_st & AB_IO(1, AO_SHIFT))
		fsi_stream_transfer(&master->fsia.playback);
	if (int_st & AB_IO(1, BO_SHIFT))
		fsi_stream_transfer(&master->fsib.playback);
	if (int_st & AB_IO(1, AI_SHIFT))
		fsi_stream_transfer(&master->fsia.capture);
	if (int_st & AB_IO(1, BI_SHIFT))
		fsi_stream_transfer(&master->fsib.capture);

	fsi_count_fifo_err(&master->fsia);
	fsi_count_fifo_err(&master->fsib);

	fsi_irq_clear_status(&master->fsia);
	fsi_irq_clear_status(&master->fsib);

	return IRQ_HANDLED;
}

/*
 *		dma data transfer handler
 */
static int fsi_dma_init(struct fsi_priv *fsi, struct fsi_stream *io)
{
	struct snd_pcm_runtime *runtime = io->substream->runtime;
	struct snd_soc_dai *dai = fsi_get_dai(io->substream);
	enum dma_data_direction dir = fsi_stream_is_play(fsi, io) ?
				DMA_TO_DEVICE : DMA_FROM_DEVICE;

	io->dma = dma_map_single(dai->dev, runtime->dma_area,
				 snd_pcm_lib_buffer_bytes(io->substream), dir);
	return 0;
}

static int fsi_dma_quit(struct fsi_priv *fsi, struct fsi_stream *io)
{
	struct snd_soc_dai *dai = fsi_get_dai(io->substream);
	enum dma_data_direction dir = fsi_stream_is_play(fsi, io) ?
		DMA_TO_DEVICE : DMA_FROM_DEVICE;

	dma_unmap_single(dai->dev, io->dma,
			 snd_pcm_lib_buffer_bytes(io->substream), dir);
	return 0;
}

static void fsi_dma_complete(void *data)
{
	struct fsi_stream *io = (struct fsi_stream *)data;
	struct fsi_priv *fsi = fsi_stream_to_priv(io);
	struct snd_pcm_runtime *runtime = io->substream->runtime;
	struct snd_soc_dai *dai = fsi_get_dai(io->substream);
	enum dma_data_direction dir = fsi_stream_is_play(fsi, io) ?
		DMA_TO_DEVICE : DMA_FROM_DEVICE;

	dma_sync_single_for_cpu(dai->dev, io->dma,
			samples_to_bytes(runtime, io->period_samples), dir);

	io->buff_sample_pos += io->period_samples;
	io->period_pos++;

	if (io->period_pos >= runtime->periods) {
		io->period_pos = 0;
		io->buff_sample_pos = 0;
	}

	fsi_count_fifo_err(fsi);
	fsi_stream_transfer(io);

	snd_pcm_period_elapsed(io->substream);
}

static dma_addr_t fsi_dma_get_area(struct fsi_stream *io)
{
	struct snd_pcm_runtime *runtime = io->substream->runtime;

	return io->dma + samples_to_bytes(runtime, io->buff_sample_pos);
}

static void fsi_dma_do_work(struct work_struct *work)
{
	struct fsi_stream *io = container_of(work, struct fsi_stream, work);
	struct fsi_priv *fsi = fsi_stream_to_priv(io);
	struct dma_chan *chan;
	struct snd_soc_dai *dai;
	struct dma_async_tx_descriptor *desc;
	struct scatterlist sg;
	struct snd_pcm_runtime *runtime;
	enum dma_data_direction dir;
	dma_cookie_t cookie;
	int is_play = fsi_stream_is_play(fsi, io);
	int len;
	dma_addr_t buf;

	if (!fsi_stream_is_working(fsi, io))
		return;

	dai	= fsi_get_dai(io->substream);
	chan	= io->chan;
	runtime	= io->substream->runtime;
	dir	= is_play ? DMA_TO_DEVICE : DMA_FROM_DEVICE;
	len	= samples_to_bytes(runtime, io->period_samples);
	buf	= fsi_dma_get_area(io);

	dma_sync_single_for_device(dai->dev, io->dma, len, dir);

	sg_init_table(&sg, 1);
	sg_set_page(&sg, pfn_to_page(PFN_DOWN(buf)),
		    len , offset_in_page(buf));
	sg_dma_address(&sg) = buf;
	sg_dma_len(&sg) = len;

	desc = dmaengine_prep_slave_sg(chan, &sg, 1, dir,
				       DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	if (!desc) {
		dev_err(dai->dev, "dmaengine_prep_slave_sg() fail\n");
		return;
	}

	desc->callback		= fsi_dma_complete;
	desc->callback_param	= io;

	cookie = desc->tx_submit(desc);
	if (cookie < 0) {
		dev_err(dai->dev, "tx_submit() fail\n");
		return;
	}

	dma_async_issue_pending(chan);

	/*
	 * FIXME
	 *
	 * In DMAEngine case, codec and FSI cannot be started simultaneously
	 * since FSI is using the scheduler work queue.
	 * Therefore, in capture case, probably FSI FIFO will have got
	 * overflow error in this point.
	 * in that case, DMA cannot start transfer until error was cleared.
	 */
	if (!is_play) {
		if (ERR_OVER & fsi_reg_read(fsi, DIFF_ST)) {
			fsi_reg_mask_set(fsi, DIFF_CTL, FIFO_CLR, FIFO_CLR);
			fsi_reg_write(fsi, DIFF_ST, 0);
		}
	}
}

static bool fsi_dma_filter(struct dma_chan *chan, void *param)
{
	struct sh_dmae_slave *slave = param;

	chan->private = slave;

	return true;
}

static int fsi_dma_transfer(struct fsi_priv *fsi, struct fsi_stream *io)
{
	schedule_work(&io->work);

	return 0;
}

static void fsi_dma_push_start_stop(struct fsi_priv *fsi, struct fsi_stream *io,
				 int start)
{
	u32 bws;
	u32 dma;

	switch (io->sample_width * start) {
	case 2:
		bws = CR_BWS_16;
		dma = VDMD_STREAM | DMA_ON;
		break;
	case 4:
		bws = CR_BWS_24;
		dma = VDMD_BACK | DMA_ON;
		break;
	default:
		bws = 0;
		dma = 0;
	}

	fsi_reg_mask_set(fsi, DO_FMT, CR_BWS_MASK, bws);
	fsi_reg_write(fsi, OUT_DMAC, dma);
}

static int fsi_dma_probe(struct fsi_priv *fsi, struct fsi_stream *io)
{
	dma_cap_mask_t mask;

	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

	io->chan = dma_request_channel(mask, fsi_dma_filter, &io->slave);
	if (!io->chan)
		return -EIO;

	INIT_WORK(&io->work, fsi_dma_do_work);

	return 0;
}

static int fsi_dma_remove(struct fsi_priv *fsi, struct fsi_stream *io)
{
	cancel_work_sync(&io->work);

	fsi_stream_stop(fsi, io);

	if (io->chan)
		dma_release_channel(io->chan);

	io->chan = NULL;
	return 0;
}

static struct fsi_stream_handler fsi_dma_push_handler = {
	.init		= fsi_dma_init,
	.quit		= fsi_dma_quit,
	.probe		= fsi_dma_probe,
	.transfer	= fsi_dma_transfer,
	.remove		= fsi_dma_remove,
	.start_stop	= fsi_dma_push_start_stop,
};

/*
 *		dai ops
 */
static void fsi_fifo_init(struct fsi_priv *fsi,
			  struct fsi_stream *io,
			  struct device *dev)
{
	struct fsi_master *master = fsi_get_master(fsi);
	int is_play = fsi_stream_is_play(fsi, io);
	u32 shift, i;
	int frame_capa;

	/* get on-chip RAM capacity */
	shift = fsi_master_read(master, FIFO_SZ);
	shift >>= fsi_get_port_shift(fsi, io);
	shift &= FIFO_SZ_MASK;
	frame_capa = 256 << shift;
	dev_dbg(dev, "fifo = %d words\n", frame_capa);
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * The maximum number of sample data varies depending
	 * on the number of channels selected for the format.
	 *
	 * FIFOs are used in 4-channel units in 3-channel mode
	 * and in 8-channel units in 5- to 7-channel mode
	 * meaning that more FIFOs than the required size of DPRAM
	 * are used.
	 *
	 * ex) if 256 words of DP-RAM is connected
	 * 1 channel:  256 (256 x 1 = 256)
	 * 2 channels: 128 (128 x 2 = 256)
	 * 3 channels:  64 ( 64 x 3 = 192)
	 * 4 channels:  64 ( 64 x 4 = 256)
	 * 5 channels:  32 ( 32 x 5 = 160)
	 * 6 channels:  32 ( 32 x 6 = 192)
	 * 7 channels:  32 ( 32 x 7 = 224)
	 * 8 channels:  32 ( 32 x 8 = 256)
	 */
	for (i = 1; i < fsi->chan_num; i <<= 1)
<<<<<<< HEAD
		io->fifo_max_num >>= 1;
	dev_dbg(dai->dev, "%d channel %d store\n",
		fsi->chan_num, io->fifo_max_num);
=======
		frame_capa >>= 1;
	dev_dbg(dev, "%d channel %d store\n",
		fsi->chan_num, frame_capa);

	io->fifo_sample_capa = fsi_frame2sample(fsi, frame_capa);
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * set interrupt generation factor
	 * clear FIFO
	 */
	if (is_play) {
		fsi_reg_write(fsi,	DOFF_CTL, IRQ_HALF);
		fsi_reg_mask_set(fsi,	DOFF_CTL, FIFO_CLR, FIFO_CLR);
	} else {
		fsi_reg_write(fsi,	DIFF_CTL, IRQ_HALF);
		fsi_reg_mask_set(fsi,	DIFF_CTL, FIFO_CLR, FIFO_CLR);
	}
}

<<<<<<< HEAD
static int fsi_fifo_data_ctrl(struct fsi_priv *fsi, int stream)
{
	struct snd_pcm_runtime *runtime;
	struct snd_pcm_substream *substream = NULL;
	int is_play = fsi_stream_is_play(stream);
	struct fsi_stream *io = fsi_get_stream(fsi, is_play);
	int data_residue_num;
	int data_num;
	int data_num_max;
	int ch_width;
	int over_period;
	void (*fn)(struct fsi_priv *fsi, int size);

	if (!fsi			||
	    !io->substream		||
	    !io->substream->runtime)
		return -EINVAL;

	over_period	= 0;
	substream	= io->substream;
	runtime		= substream->runtime;

	/* FSI FIFO has limit.
	 * So, this driver can not send periods data at a time
	 */
	if (io->buff_offset >=
	    fsi_num2offset(io->period_num + 1, io->period_len)) {

		over_period = 1;
		io->period_num = (io->period_num + 1) % runtime->periods;

		if (0 == io->period_num)
			io->buff_offset = 0;
	}

	/* get 1 channel data width */
	ch_width = fsi_get_frame_width(fsi, is_play);

	/* get residue data number of alsa */
	data_residue_num = fsi_len2num(io->buff_len - io->buff_offset,
				       ch_width);

	if (is_play) {
		/*
		 * for play-back
		 *
		 * data_num_max	: number of FSI fifo free space
		 * data_num	: number of ALSA residue data
		 */
		data_num_max  = io->fifo_max_num * fsi->chan_num;
		data_num_max -= fsi_get_fifo_data_num(fsi, is_play);

		data_num = data_residue_num;

		switch (ch_width) {
		case 2:
			fn = fsi_dma_soft_push16;
			break;
		case 4:
			fn = fsi_dma_soft_push32;
			break;
		default:
			return -EINVAL;
		}
	} else {
		/*
		 * for capture
		 *
		 * data_num_max	: number of ALSA free space
		 * data_num	: number of data in FSI fifo
		 */
		data_num_max = data_residue_num;
		data_num     = fsi_get_fifo_data_num(fsi, is_play);

		switch (ch_width) {
		case 2:
			fn = fsi_dma_soft_pop16;
			break;
		case 4:
			fn = fsi_dma_soft_pop32;
			break;
		default:
			return -EINVAL;
		}
	}

	data_num = min(data_num, data_num_max);

	fn(fsi, data_num);

	/* update buff_offset */
	io->buff_offset += fsi_num2offset(data_num, ch_width);

	if (over_period)
		snd_pcm_period_elapsed(substream);

	return 0;
}

static int fsi_data_pop(struct fsi_priv *fsi)
{
	return fsi_fifo_data_ctrl(fsi, SNDRV_PCM_STREAM_CAPTURE);
}

static int fsi_data_push(struct fsi_priv *fsi)
{
	return fsi_fifo_data_ctrl(fsi, SNDRV_PCM_STREAM_PLAYBACK);
}

static irqreturn_t fsi_interrupt(int irq, void *data)
{
	struct fsi_master *master = data;
	u32 int_st = fsi_irq_get_status(master);

	/* clear irq status */
	fsi_master_mask_set(master, SOFT_RST, IR, 0);
	fsi_master_mask_set(master, SOFT_RST, IR, IR);

	if (int_st & AB_IO(1, AO_SHIFT))
		fsi_data_push(&master->fsia);
	if (int_st & AB_IO(1, BO_SHIFT))
		fsi_data_push(&master->fsib);
	if (int_st & AB_IO(1, AI_SHIFT))
		fsi_data_pop(&master->fsia);
	if (int_st & AB_IO(1, BI_SHIFT))
		fsi_data_pop(&master->fsib);

	fsi_count_fifo_err(&master->fsia);
	fsi_count_fifo_err(&master->fsib);

	fsi_irq_clear_status(&master->fsia);
	fsi_irq_clear_status(&master->fsib);

	return IRQ_HANDLED;
}

/*
 *		dai ops
 */

static int fsi_dai_startup(struct snd_pcm_substream *substream,
			   struct snd_soc_dai *dai)
{
	struct fsi_priv *fsi = fsi_get_priv(substream);
	u32 flags = fsi_get_info_flags(fsi);
	u32 data;
	int is_play = fsi_is_play(substream);

	pm_runtime_get_sync(dai->dev);

=======
static int fsi_hw_startup(struct fsi_priv *fsi,
			  struct fsi_stream *io,
			  struct device *dev)
{
	struct fsi_master *master = fsi_get_master(fsi);
	int fsi_ver = master->core->ver;
	u32 flags = fsi_get_info_flags(fsi);
	u32 data = 0;

	/* clock setting */
	if (fsi_is_clk_master(fsi))
		data = DIMD | DOMD;

	fsi_reg_mask_set(fsi, CKG1, (DIMD | DOMD), data);
>>>>>>> refs/remotes/origin/cm-10.0

	/* clock inversion (CKG2) */
	data = 0;
	if (SH_FSI_LRM_INV & flags)
		data |= 1 << 12;
	if (SH_FSI_BRM_INV & flags)
		data |= 1 << 8;
	if (SH_FSI_LRS_INV & flags)
		data |= 1 << 4;
	if (SH_FSI_BRS_INV & flags)
		data |= 1 << 0;

	fsi_reg_write(fsi, CKG2, data);

<<<<<<< HEAD
	/* irq clear */
	fsi_irq_disable(fsi, is_play);
	fsi_irq_clear_status(fsi);

	/* fifo init */
	fsi_fifo_init(fsi, is_play, dai);
=======
	/* set format */
	fsi_reg_write(fsi, DO_FMT, fsi->do_fmt);
	fsi_reg_write(fsi, DI_FMT, fsi->di_fmt);

	/* spdif ? */
	if (fsi_is_spdif(fsi)) {
		fsi_spdif_clk_ctrl(fsi, 1);
		fsi_reg_mask_set(fsi, OUT_SEL, DMMD, DMMD);
	}

	/*
	 * FIXME
	 *
	 * FSI driver assumed that data package is in-back.
	 * FSI2 chip can select it.
	 */
	if (fsi_ver >= 2) {
		fsi_reg_write(fsi, OUT_DMAC,	(1 << 4));
		fsi_reg_write(fsi, IN_DMAC,	(1 << 4));
	}

	/* irq clear */
	fsi_irq_disable(fsi, io);
	fsi_irq_clear_status(fsi);

	/* fifo init */
	fsi_fifo_init(fsi, io, dev);
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

<<<<<<< HEAD
static void fsi_dai_shutdown(struct snd_pcm_substream *substream,
			     struct snd_soc_dai *dai)
{
	struct fsi_priv *fsi = fsi_get_priv(substream);
	int is_play = fsi_is_play(substream);
	struct fsi_master *master = fsi_get_master(fsi);
	set_rate_func set_rate = fsi_get_info_set_rate(master);

	fsi_irq_disable(fsi, is_play);

	if (fsi_is_clk_master(fsi))
		set_rate(dai->dev, fsi_is_port_a(fsi), fsi->rate, 0);

	fsi->rate = 0;

	pm_runtime_put_sync(dai->dev);
=======
static void fsi_hw_shutdown(struct fsi_priv *fsi,
			    struct device *dev)
{
	if (fsi_is_clk_master(fsi))
		fsi_set_master_clk(dev, fsi, fsi->rate, 0);
}

static int fsi_dai_startup(struct snd_pcm_substream *substream,
			   struct snd_soc_dai *dai)
{
	struct fsi_priv *fsi = fsi_get_priv(substream);

	return fsi_hw_startup(fsi, fsi_stream_get(fsi, substream), dai->dev);
}

static void fsi_dai_shutdown(struct snd_pcm_substream *substream,
			     struct snd_soc_dai *dai)
{
	struct fsi_priv *fsi = fsi_get_priv(substream);

	fsi_hw_shutdown(fsi, dai->dev);
	fsi->rate = 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int fsi_dai_trigger(struct snd_pcm_substream *substream, int cmd,
			   struct snd_soc_dai *dai)
{
	struct fsi_priv *fsi = fsi_get_priv(substream);
<<<<<<< HEAD
	struct snd_pcm_runtime *runtime = substream->runtime;
	int is_play = fsi_is_play(substream);
=======
	struct fsi_stream *io = fsi_stream_get(fsi, substream);
>>>>>>> refs/remotes/origin/cm-10.0
	int ret = 0;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
<<<<<<< HEAD
		fsi_stream_push(fsi, is_play, substream,
				frames_to_bytes(runtime, runtime->buffer_size),
				frames_to_bytes(runtime, runtime->period_size));
		ret = is_play ? fsi_data_push(fsi) : fsi_data_pop(fsi);
		fsi_irq_enable(fsi, is_play);
		fsi_port_start(fsi);
		break;
	case SNDRV_PCM_TRIGGER_STOP:
		fsi_port_stop(fsi);
		fsi_irq_disable(fsi, is_play);
		fsi_stream_pop(fsi, is_play);
=======
		fsi_stream_init(fsi, io, substream);
		ret = fsi_stream_transfer(io);
		if (0 == ret)
			fsi_stream_start(fsi, io);
		break;
	case SNDRV_PCM_TRIGGER_STOP:
		fsi_stream_stop(fsi, io);
		fsi_stream_quit(fsi, io);
>>>>>>> refs/remotes/origin/cm-10.0
		break;
	}

	return ret;
}

static int fsi_set_fmt_dai(struct fsi_priv *fsi, unsigned int fmt)
{
	u32 data = 0;

	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		data = CR_I2S;
		fsi->chan_num = 2;
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		data = CR_PCM;
		fsi->chan_num = 2;
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	fsi_reg_write(fsi, DO_FMT, data);
	fsi_reg_write(fsi, DI_FMT, data);
=======
	fsi->do_fmt = data;
	fsi->di_fmt = data;
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

static int fsi_set_fmt_spdif(struct fsi_priv *fsi)
{
	struct fsi_master *master = fsi_get_master(fsi);
	u32 data = 0;

	if (master->core->ver < 2)
		return -EINVAL;

	data = CR_BWS_16 | CR_DTMD_SPDIF_PCM | CR_PCM;
	fsi->chan_num = 2;
<<<<<<< HEAD
	fsi_spdif_clk_ctrl(fsi, 1);
	fsi_reg_mask_set(fsi, OUT_SEL, DMMD, DMMD);

	fsi_reg_write(fsi, DO_FMT, data);
	fsi_reg_write(fsi, DI_FMT, data);
=======
	fsi->spdif = 1;

	fsi->do_fmt = data;
	fsi->di_fmt = data;
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

static int fsi_dai_set_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	struct fsi_priv *fsi = fsi_get_priv_frm_dai(dai);
<<<<<<< HEAD
	struct fsi_master *master = fsi_get_master(fsi);
	set_rate_func set_rate = fsi_get_info_set_rate(master);
	u32 flags = fsi_get_info_flags(fsi);
	u32 data = 0;
	int ret;

	pm_runtime_get_sync(dai->dev);

	/* set master/slave audio interface */
	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:
		data = DIMD | DOMD;
=======
	set_rate_func set_rate = fsi_get_info_set_rate(fsi);
	u32 flags = fsi_get_info_flags(fsi);
	int ret;

	/* set master/slave audio interface */
	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:
>>>>>>> refs/remotes/origin/cm-10.0
		fsi->clk_master = 1;
		break;
	case SND_SOC_DAIFMT_CBS_CFS:
		break;
	default:
<<<<<<< HEAD
		ret = -EINVAL;
		goto set_fmt_exit;
=======
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	if (fsi_is_clk_master(fsi) && !set_rate) {
		dev_err(dai->dev, "platform doesn't have set_rate\n");
<<<<<<< HEAD
		ret = -EINVAL;
		goto set_fmt_exit;
	}

	fsi_reg_mask_set(fsi, CKG1, (DIMD | DOMD), data);

=======
		return -EINVAL;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	/* set format */
	switch (flags & SH_FSI_FMT_MASK) {
	case SH_FSI_FMT_DAI:
		ret = fsi_set_fmt_dai(fsi, fmt & SND_SOC_DAIFMT_FORMAT_MASK);
		break;
	case SH_FSI_FMT_SPDIF:
		ret = fsi_set_fmt_spdif(fsi);
		break;
	default:
		ret = -EINVAL;
	}

<<<<<<< HEAD
set_fmt_exit:
	pm_runtime_put_sync(dai->dev);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static int fsi_dai_hw_params(struct snd_pcm_substream *substream,
			     struct snd_pcm_hw_params *params,
			     struct snd_soc_dai *dai)
{
	struct fsi_priv *fsi = fsi_get_priv(substream);
<<<<<<< HEAD
	struct fsi_master *master = fsi_get_master(fsi);
	set_rate_func set_rate = fsi_get_info_set_rate(master);
	int fsi_ver = master->core->ver;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	long rate = params_rate(params);
	int ret;

	if (!fsi_is_clk_master(fsi))
		return 0;

<<<<<<< HEAD
	ret = set_rate(dai->dev, fsi_is_port_a(fsi), rate, 1);
	if (ret < 0) /* error */
		return ret;

	fsi->rate = rate;
	if (ret > 0) {
		u32 data = 0;

		switch (ret & SH_FSI_ACKMD_MASK) {
		default:
			/* FALL THROUGH */
		case SH_FSI_ACKMD_512:
			data |= (0x0 << 12);
			break;
		case SH_FSI_ACKMD_256:
			data |= (0x1 << 12);
			break;
		case SH_FSI_ACKMD_128:
			data |= (0x2 << 12);
			break;
		case SH_FSI_ACKMD_64:
			data |= (0x3 << 12);
			break;
		case SH_FSI_ACKMD_32:
			if (fsi_ver < 2)
				dev_err(dai->dev, "unsupported ACKMD\n");
			else
				data |= (0x4 << 12);
			break;
		}

		switch (ret & SH_FSI_BPFMD_MASK) {
		default:
			/* FALL THROUGH */
		case SH_FSI_BPFMD_32:
			data |= (0x0 << 8);
			break;
		case SH_FSI_BPFMD_64:
			data |= (0x1 << 8);
			break;
		case SH_FSI_BPFMD_128:
			data |= (0x2 << 8);
			break;
		case SH_FSI_BPFMD_256:
			data |= (0x3 << 8);
			break;
		case SH_FSI_BPFMD_512:
			data |= (0x4 << 8);
			break;
		case SH_FSI_BPFMD_16:
			if (fsi_ver < 2)
				dev_err(dai->dev, "unsupported ACKMD\n");
			else
				data |= (0x7 << 8);
			break;
		}

		fsi_reg_mask_set(fsi, CKG1, (ACKMD_MASK | BPFMD_MASK) , data);
		udelay(10);
		ret = 0;
	}

	return ret;

}

static struct snd_soc_dai_ops fsi_dai_ops = {
=======
	ret = fsi_set_master_clk(dai->dev, fsi, rate, 1);
	if (ret < 0)
		return ret;

	fsi->rate = rate;

	return ret;
}

static const struct snd_soc_dai_ops fsi_dai_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
	.startup	= fsi_dai_startup,
	.shutdown	= fsi_dai_shutdown,
	.trigger	= fsi_dai_trigger,
	.set_fmt	= fsi_dai_set_fmt,
	.hw_params	= fsi_dai_hw_params,
};

/*
 *		pcm ops
 */

static struct snd_pcm_hardware fsi_pcm_hardware = {
	.info =		SNDRV_PCM_INFO_INTERLEAVED	|
			SNDRV_PCM_INFO_MMAP		|
			SNDRV_PCM_INFO_MMAP_VALID	|
			SNDRV_PCM_INFO_PAUSE,
	.formats		= FSI_FMTS,
	.rates			= FSI_RATES,
	.rate_min		= 8000,
	.rate_max		= 192000,
	.channels_min		= 1,
	.channels_max		= 2,
	.buffer_bytes_max	= 64 * 1024,
	.period_bytes_min	= 32,
	.period_bytes_max	= 8192,
	.periods_min		= 1,
	.periods_max		= 32,
	.fifo_size		= 256,
};

static int fsi_pcm_open(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	int ret = 0;

	snd_soc_set_runtime_hwparams(substream, &fsi_pcm_hardware);

	ret = snd_pcm_hw_constraint_integer(runtime,
					    SNDRV_PCM_HW_PARAM_PERIODS);

	return ret;
}

static int fsi_hw_params(struct snd_pcm_substream *substream,
			 struct snd_pcm_hw_params *hw_params)
{
	return snd_pcm_lib_malloc_pages(substream,
					params_buffer_bytes(hw_params));
}

static int fsi_hw_free(struct snd_pcm_substream *substream)
{
	return snd_pcm_lib_free_pages(substream);
}

static snd_pcm_uframes_t fsi_pointer(struct snd_pcm_substream *substream)
{
<<<<<<< HEAD
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct fsi_priv *fsi = fsi_get_priv(substream);
	struct fsi_stream *io = fsi_get_stream(fsi, fsi_is_play(substream));
	long location;

	location = (io->buff_offset - 1);
	if (location < 0)
		location = 0;

	return bytes_to_frames(runtime, location);
=======
	struct fsi_priv *fsi = fsi_get_priv(substream);
	struct fsi_stream *io = fsi_stream_get(fsi, substream);

	return fsi_sample2frame(fsi, io->buff_sample_pos);
>>>>>>> refs/remotes/origin/cm-10.0
}

static struct snd_pcm_ops fsi_pcm_ops = {
	.open		= fsi_pcm_open,
	.ioctl		= snd_pcm_lib_ioctl,
	.hw_params	= fsi_hw_params,
	.hw_free	= fsi_hw_free,
	.pointer	= fsi_pointer,
};

/*
 *		snd_soc_platform
 */

#define PREALLOC_BUFFER		(32 * 1024)
#define PREALLOC_BUFFER_MAX	(32 * 1024)

static void fsi_pcm_free(struct snd_pcm *pcm)
{
	snd_pcm_lib_preallocate_free_for_all(pcm);
}

static int fsi_pcm_new(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_pcm *pcm = rtd->pcm;

	/*
	 * dont use SNDRV_DMA_TYPE_DEV, since it will oops the SH kernel
	 * in MMAP mode (i.e. aplay -M)
	 */
	return snd_pcm_lib_preallocate_pages_for_all(
		pcm,
		SNDRV_DMA_TYPE_CONTINUOUS,
		snd_dma_continuous_data(GFP_KERNEL),
		PREALLOC_BUFFER, PREALLOC_BUFFER_MAX);
}

/*
 *		alsa struct
 */

static struct snd_soc_dai_driver fsi_soc_dai[] = {
	{
		.name			= "fsia-dai",
		.playback = {
			.rates		= FSI_RATES,
			.formats	= FSI_FMTS,
			.channels_min	= 1,
			.channels_max	= 8,
		},
		.capture = {
			.rates		= FSI_RATES,
			.formats	= FSI_FMTS,
			.channels_min	= 1,
			.channels_max	= 8,
		},
		.ops = &fsi_dai_ops,
	},
	{
		.name			= "fsib-dai",
		.playback = {
			.rates		= FSI_RATES,
			.formats	= FSI_FMTS,
			.channels_min	= 1,
			.channels_max	= 8,
		},
		.capture = {
			.rates		= FSI_RATES,
			.formats	= FSI_FMTS,
			.channels_min	= 1,
			.channels_max	= 8,
		},
		.ops = &fsi_dai_ops,
	},
};

static struct snd_soc_platform_driver fsi_soc_platform = {
	.ops		= &fsi_pcm_ops,
	.pcm_new	= fsi_pcm_new,
	.pcm_free	= fsi_pcm_free,
};

/*
 *		platform function
 */
<<<<<<< HEAD
=======
static void fsi_handler_init(struct fsi_priv *fsi)
{
	fsi->playback.handler	= &fsi_pio_push_handler; /* default PIO */
	fsi->playback.priv	= fsi;
	fsi->capture.handler	= &fsi_pio_pop_handler;  /* default PIO */
	fsi->capture.priv	= fsi;

	if (fsi->info->tx_id) {
		fsi->playback.slave.slave_id	= fsi->info->tx_id;
		fsi->playback.handler		= &fsi_dma_push_handler;
	}
}
>>>>>>> refs/remotes/origin/cm-10.0

static int fsi_probe(struct platform_device *pdev)
{
	struct fsi_master *master;
	const struct platform_device_id	*id_entry;
<<<<<<< HEAD
=======
	struct sh_fsi_platform_info *info = pdev->dev.platform_data;
>>>>>>> refs/remotes/origin/cm-10.0
	struct resource *res;
	unsigned int irq;
	int ret;

	id_entry = pdev->id_entry;
	if (!id_entry) {
		dev_err(&pdev->dev, "unknown fsi device\n");
		return -ENODEV;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	irq = platform_get_irq(pdev, 0);
	if (!res || (int)irq <= 0) {
		dev_err(&pdev->dev, "Not enough FSI platform resources.\n");
		ret = -ENODEV;
		goto exit;
	}

	master = kzalloc(sizeof(*master), GFP_KERNEL);
	if (!master) {
		dev_err(&pdev->dev, "Could not allocate master\n");
		ret = -ENOMEM;
		goto exit;
	}

	master->base = ioremap_nocache(res->start, resource_size(res));
	if (!master->base) {
		ret = -ENXIO;
		dev_err(&pdev->dev, "Unable to ioremap FSI registers.\n");
		goto exit_kfree;
	}

	/* master setting */
	master->irq		= irq;
<<<<<<< HEAD
	master->info		= pdev->dev.platform_data;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	master->core		= (struct fsi_core *)id_entry->driver_data;
	spin_lock_init(&master->lock);

	/* FSI A setting */
	master->fsia.base	= master->base;
	master->fsia.master	= master;
<<<<<<< HEAD
=======
	master->fsia.info	= &info->port_a;
	fsi_handler_init(&master->fsia);
	ret = fsi_stream_probe(&master->fsia);
	if (ret < 0) {
		dev_err(&pdev->dev, "FSIA stream probe failed\n");
		goto exit_iounmap;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	/* FSI B setting */
	master->fsib.base	= master->base + 0x40;
	master->fsib.master	= master;
<<<<<<< HEAD
=======
	master->fsib.info	= &info->port_b;
	fsi_handler_init(&master->fsib);
	ret = fsi_stream_probe(&master->fsib);
	if (ret < 0) {
		dev_err(&pdev->dev, "FSIB stream probe failed\n");
		goto exit_fsia;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	pm_runtime_enable(&pdev->dev);
	dev_set_drvdata(&pdev->dev, master);

<<<<<<< HEAD
	fsi_module_init(master, &pdev->dev);

	ret = request_irq(irq, &fsi_interrupt, IRQF_DISABLED,
			  id_entry->name, master);
	if (ret) {
		dev_err(&pdev->dev, "irq request err\n");
		goto exit_iounmap;
=======
	ret = request_irq(irq, &fsi_interrupt, 0,
			  id_entry->name, master);
	if (ret) {
		dev_err(&pdev->dev, "irq request err\n");
		goto exit_fsib;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	ret = snd_soc_register_platform(&pdev->dev, &fsi_soc_platform);
	if (ret < 0) {
		dev_err(&pdev->dev, "cannot snd soc register\n");
		goto exit_free_irq;
	}

	ret = snd_soc_register_dais(&pdev->dev, fsi_soc_dai,
				    ARRAY_SIZE(fsi_soc_dai));
	if (ret < 0) {
		dev_err(&pdev->dev, "cannot snd dai register\n");
		goto exit_snd_soc;
	}

	return ret;

exit_snd_soc:
	snd_soc_unregister_platform(&pdev->dev);
exit_free_irq:
	free_irq(irq, master);
<<<<<<< HEAD
=======
exit_fsib:
	fsi_stream_remove(&master->fsib);
exit_fsia:
	fsi_stream_remove(&master->fsia);
>>>>>>> refs/remotes/origin/cm-10.0
exit_iounmap:
	iounmap(master->base);
	pm_runtime_disable(&pdev->dev);
exit_kfree:
	kfree(master);
	master = NULL;
exit:
	return ret;
}

static int fsi_remove(struct platform_device *pdev)
{
	struct fsi_master *master;

	master = dev_get_drvdata(&pdev->dev);

<<<<<<< HEAD
	fsi_module_kill(master, &pdev->dev);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	free_irq(master->irq, master);
	pm_runtime_disable(&pdev->dev);

	snd_soc_unregister_dais(&pdev->dev, ARRAY_SIZE(fsi_soc_dai));
	snd_soc_unregister_platform(&pdev->dev);

<<<<<<< HEAD
=======
	fsi_stream_remove(&master->fsia);
	fsi_stream_remove(&master->fsib);

>>>>>>> refs/remotes/origin/cm-10.0
	iounmap(master->base);
	kfree(master);

	return 0;
}

static void __fsi_suspend(struct fsi_priv *fsi,
<<<<<<< HEAD
			  struct device *dev,
			  set_rate_func set_rate)
{
	fsi->saved_do_fmt	= fsi_reg_read(fsi, DO_FMT);
	fsi->saved_di_fmt	= fsi_reg_read(fsi, DI_FMT);
	fsi->saved_ckg1		= fsi_reg_read(fsi, CKG1);
	fsi->saved_ckg2		= fsi_reg_read(fsi, CKG2);
	fsi->saved_out_sel	= fsi_reg_read(fsi, OUT_SEL);

	if (fsi_is_clk_master(fsi))
		set_rate(dev, fsi_is_port_a(fsi), fsi->rate, 0);
}

static void __fsi_resume(struct fsi_priv *fsi,
			 struct device *dev,
			 set_rate_func set_rate)
{
	fsi_reg_write(fsi, DO_FMT,	fsi->saved_do_fmt);
	fsi_reg_write(fsi, DI_FMT,	fsi->saved_di_fmt);
	fsi_reg_write(fsi, CKG1,	fsi->saved_ckg1);
	fsi_reg_write(fsi, CKG2,	fsi->saved_ckg2);
	fsi_reg_write(fsi, OUT_SEL,	fsi->saved_out_sel);

	if (fsi_is_clk_master(fsi))
		set_rate(dev, fsi_is_port_a(fsi), fsi->rate, 1);
=======
			  struct fsi_stream *io,
			  struct device *dev)
{
	if (!fsi_stream_is_working(fsi, io))
		return;

	fsi_stream_stop(fsi, io);
	fsi_hw_shutdown(fsi, dev);
}

static void __fsi_resume(struct fsi_priv *fsi,
			 struct fsi_stream *io,
			 struct device *dev)
{
	if (!fsi_stream_is_working(fsi, io))
		return;

	fsi_hw_startup(fsi, io, dev);

	if (fsi_is_clk_master(fsi) && fsi->rate)
		fsi_set_master_clk(dev, fsi, fsi->rate, 1);

	fsi_stream_start(fsi, io);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int fsi_suspend(struct device *dev)
{
	struct fsi_master *master = dev_get_drvdata(dev);
<<<<<<< HEAD
	set_rate_func set_rate = fsi_get_info_set_rate(master);

	pm_runtime_get_sync(dev);

	__fsi_suspend(&master->fsia, dev, set_rate);
	__fsi_suspend(&master->fsib, dev, set_rate);

	master->saved_a_mclk	= fsi_core_read(master, a_mclk);
	master->saved_b_mclk	= fsi_core_read(master, b_mclk);
	master->saved_iemsk	= fsi_core_read(master, iemsk);
	master->saved_imsk	= fsi_core_read(master, imsk);
	master->saved_clk_rst	= fsi_master_read(master, CLK_RST);
	master->saved_soft_rst	= fsi_master_read(master, SOFT_RST);

	fsi_module_kill(master, dev);

	pm_runtime_put_sync(dev);
=======
	struct fsi_priv *fsia = &master->fsia;
	struct fsi_priv *fsib = &master->fsib;

	__fsi_suspend(fsia, &fsia->playback, dev);
	__fsi_suspend(fsia, &fsia->capture, dev);

	__fsi_suspend(fsib, &fsib->playback, dev);
	__fsi_suspend(fsib, &fsib->capture, dev);
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

static int fsi_resume(struct device *dev)
{
	struct fsi_master *master = dev_get_drvdata(dev);
<<<<<<< HEAD
	set_rate_func set_rate = fsi_get_info_set_rate(master);

	pm_runtime_get_sync(dev);

	fsi_module_init(master, dev);

	fsi_master_mask_set(master, SOFT_RST, 0xffff, master->saved_soft_rst);
	fsi_master_mask_set(master, CLK_RST, 0xffff, master->saved_clk_rst);
	fsi_core_mask_set(master, a_mclk, 0xffff, master->saved_a_mclk);
	fsi_core_mask_set(master, b_mclk, 0xffff, master->saved_b_mclk);
	fsi_core_mask_set(master, iemsk, 0xffff, master->saved_iemsk);
	fsi_core_mask_set(master, imsk, 0xffff, master->saved_imsk);

	__fsi_resume(&master->fsia, dev, set_rate);
	__fsi_resume(&master->fsib, dev, set_rate);

	pm_runtime_put_sync(dev);
=======
	struct fsi_priv *fsia = &master->fsia;
	struct fsi_priv *fsib = &master->fsib;

	__fsi_resume(fsia, &fsia->playback, dev);
	__fsi_resume(fsia, &fsia->capture, dev);

	__fsi_resume(fsib, &fsib->playback, dev);
	__fsi_resume(fsib, &fsib->capture, dev);
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

<<<<<<< HEAD
static int fsi_runtime_nop(struct device *dev)
{
	/* Runtime PM callback shared between ->runtime_suspend()
	 * and ->runtime_resume(). Simply returns success.
	 *
	 * This driver re-initializes all registers after
	 * pm_runtime_get_sync() anyway so there is no need
	 * to save and restore registers here.
	 */
	return 0;
}

static struct dev_pm_ops fsi_pm_ops = {
	.suspend		= fsi_suspend,
	.resume			= fsi_resume,
	.runtime_suspend	= fsi_runtime_nop,
	.runtime_resume		= fsi_runtime_nop,
=======
static struct dev_pm_ops fsi_pm_ops = {
	.suspend		= fsi_suspend,
	.resume			= fsi_resume,
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct fsi_core fsi1_core = {
	.ver	= 1,

	/* Interrupt */
	.int_st	= INT_ST,
	.iemsk	= IEMSK,
	.imsk	= IMSK,
};

static struct fsi_core fsi2_core = {
	.ver	= 2,

	/* Interrupt */
	.int_st	= CPU_INT_ST,
	.iemsk	= CPU_IEMSK,
	.imsk	= CPU_IMSK,
	.a_mclk	= A_MST_CTLR,
	.b_mclk	= B_MST_CTLR,
};

static struct platform_device_id fsi_id_table[] = {
	{ "sh_fsi",	(kernel_ulong_t)&fsi1_core },
	{ "sh_fsi2",	(kernel_ulong_t)&fsi2_core },
	{},
};
MODULE_DEVICE_TABLE(platform, fsi_id_table);

static struct platform_driver fsi_driver = {
	.driver 	= {
		.name	= "fsi-pcm-audio",
		.pm	= &fsi_pm_ops,
	},
	.probe		= fsi_probe,
	.remove		= fsi_remove,
	.id_table	= fsi_id_table,
};

<<<<<<< HEAD
static int __init fsi_mobile_init(void)
{
	return platform_driver_register(&fsi_driver);
}

static void __exit fsi_mobile_exit(void)
{
	platform_driver_unregister(&fsi_driver);
}

module_init(fsi_mobile_init);
module_exit(fsi_mobile_exit);
=======
module_platform_driver(fsi_driver);
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SuperH onchip FSI audio driver");
MODULE_AUTHOR("Kuninori Morimoto <morimoto.kuninori@renesas.com>");
MODULE_ALIAS("platform:fsi-pcm-audio");
