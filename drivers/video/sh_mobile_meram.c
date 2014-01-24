/*
 * SuperH Mobile MERAM Driver for SuperH Mobile LCDC Driver
 *
 * Copyright (c) 2011	Damian Hobson-Garcia <dhobsong@igel.co.jp>
 *                      Takanari Hayama <taki@igel.co.jp>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/platform_device.h>

#include "sh_mobile_meram.h"

/* meram registers */
#define MExxCTL 0x0
#define MExxBSIZE 0x4
#define MExxMNCF 0x8
#define MExxSARA 0x10
#define MExxSARB 0x14
#define MExxSBSIZE 0x18

#define MERAM_MExxCTL_VAL(ctl, next_icb, addr)	\
	((ctl) | (((next_icb) & 0x1f) << 11) | (((addr) & 0x7ff) << 16))
#define	MERAM_MExxBSIZE_VAL(a, b, c) \
	(((a) << 28) | ((b) << 16) | (c))

#define MEVCR1 0x4
#define MEACTS 0x10
#define MEQSEL1 0x40
#define MEQSEL2 0x44

/* settings */
#define MERAM_SEC_LINE 15
#define MERAM_LINE_WIDTH 2048

/*
 * MERAM/ICB access functions
 */

#define MERAM_ICB_OFFSET(base, idx, off)	\
	((base) + (0x400 + ((idx) * 0x20) + (off)))

static inline void meram_write_icb(void __iomem *base, int idx, int off,
	unsigned long val)
=======
#include <linux/device.h>
#include <linux/err.h>
=======
#include <linux/device.h>
#include <linux/err.h>
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/genalloc.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>

#include <video/sh_mobile_meram.h>

/* -----------------------------------------------------------------------------
 * MERAM registers
 */

#define MEVCR1			0x4
#define MEVCR1_RST		(1 << 31)
#define MEVCR1_WD		(1 << 30)
#define MEVCR1_AMD1		(1 << 29)
#define MEVCR1_AMD0		(1 << 28)
#define MEQSEL1			0x40
#define MEQSEL2			0x44

#define MExxCTL			0x400
#define MExxCTL_BV		(1 << 31)
#define MExxCTL_BSZ_SHIFT	28
#define MExxCTL_MSAR_MASK	(0x7ff << MExxCTL_MSAR_SHIFT)
#define MExxCTL_MSAR_SHIFT	16
#define MExxCTL_NXT_MASK	(0x1f << MExxCTL_NXT_SHIFT)
#define MExxCTL_NXT_SHIFT	11
#define MExxCTL_WD1		(1 << 10)
#define MExxCTL_WD0		(1 << 9)
#define MExxCTL_WS		(1 << 8)
#define MExxCTL_CB		(1 << 7)
#define MExxCTL_WBF		(1 << 6)
#define MExxCTL_WF		(1 << 5)
#define MExxCTL_RF		(1 << 4)
#define MExxCTL_CM		(1 << 3)
#define MExxCTL_MD_READ		(1 << 0)
#define MExxCTL_MD_WRITE	(2 << 0)
#define MExxCTL_MD_ICB_WB	(3 << 0)
#define MExxCTL_MD_ICB		(4 << 0)
#define MExxCTL_MD_FB		(7 << 0)
#define MExxCTL_MD_MASK		(7 << 0)
#define MExxBSIZE		0x404
#define MExxBSIZE_RCNT_SHIFT	28
#define MExxBSIZE_YSZM1_SHIFT	16
#define MExxBSIZE_XSZM1_SHIFT	0
#define MExxMNCF		0x408
#define MExxMNCF_KWBNM_SHIFT	28
#define MExxMNCF_KRBNM_SHIFT	24
#define MExxMNCF_BNM_SHIFT	16
#define MExxMNCF_XBV		(1 << 15)
#define MExxMNCF_CPL_YCBCR444	(1 << 12)
#define MExxMNCF_CPL_YCBCR420	(2 << 12)
#define MExxMNCF_CPL_YCBCR422	(3 << 12)
#define MExxMNCF_CPL_MSK	(3 << 12)
#define MExxMNCF_BL		(1 << 2)
#define MExxMNCF_LNM_SHIFT	0
#define MExxSARA		0x410
#define MExxSARB		0x414
#define MExxSBSIZE		0x418
#define MExxSBSIZE_HDV		(1 << 31)
#define MExxSBSIZE_HSZ16	(0 << 28)
#define MExxSBSIZE_HSZ32	(1 << 28)
#define MExxSBSIZE_HSZ64	(2 << 28)
#define MExxSBSIZE_HSZ128	(3 << 28)
#define MExxSBSIZE_SBSIZZ_SHIFT	0

#define MERAM_MExxCTL_VAL(next, addr)	\
	((((next) << MExxCTL_NXT_SHIFT) & MExxCTL_NXT_MASK) | \
	 (((addr) << MExxCTL_MSAR_SHIFT) & MExxCTL_MSAR_MASK))
#define	MERAM_MExxBSIZE_VAL(rcnt, yszm1, xszm1) \
	(((rcnt) << MExxBSIZE_RCNT_SHIFT) | \
	 ((yszm1) << MExxBSIZE_YSZM1_SHIFT) | \
	 ((xszm1) << MExxBSIZE_XSZM1_SHIFT))

static const unsigned long common_regs[] = {
	MEVCR1,
	MEQSEL1,
	MEQSEL2,
};
#define MERAM_REGS_SIZE ARRAY_SIZE(common_regs)

static const unsigned long icb_regs[] = {
	MExxCTL,
	MExxBSIZE,
	MExxMNCF,
	MExxSARA,
	MExxSARB,
	MExxSBSIZE,
};
#define ICB_REGS_SIZE ARRAY_SIZE(icb_regs)

/*
 * sh_mobile_meram_icb - MERAM ICB information
 * @regs: Registers cache
 * @index: ICB index
 * @offset: MERAM block offset
 * @size: MERAM block size in KiB
 * @cache_unit: Bytes to cache per ICB
 * @pixelformat: Video pixel format of the data stored in the ICB
 * @current_reg: Which of Start Address Register A (0) or B (1) is in use
 */
struct sh_mobile_meram_icb {
	unsigned long regs[ICB_REGS_SIZE];
	unsigned int index;
	unsigned long offset;
	unsigned int size;

	unsigned int cache_unit;
	unsigned int pixelformat;
	unsigned int current_reg;
};

#define MERAM_ICB_NUM			32

struct sh_mobile_meram_fb_plane {
	struct sh_mobile_meram_icb *marker;
	struct sh_mobile_meram_icb *cache;
};

struct sh_mobile_meram_fb_cache {
	unsigned int nplanes;
	struct sh_mobile_meram_fb_plane planes[2];
};

/*
 * sh_mobile_meram_priv - MERAM device
 * @base: Registers base address
 * @meram: MERAM physical address
 * @regs: Registers cache
 * @lock: Protects used_icb and icbs
 * @used_icb: Bitmask of used ICBs
 * @icbs: ICBs
 * @pool: Allocation pool to manage the MERAM
 */
struct sh_mobile_meram_priv {
	void __iomem *base;
	unsigned long meram;
	unsigned long regs[MERAM_REGS_SIZE];

	struct mutex lock;
	unsigned long used_icb;
	struct sh_mobile_meram_icb icbs[MERAM_ICB_NUM];

	struct gen_pool *pool;
};

/* settings */
#define MERAM_GRANULARITY		1024
#define MERAM_SEC_LINE			15
#define MERAM_LINE_WIDTH		2048

/* -----------------------------------------------------------------------------
 * Registers access
 */

#define MERAM_ICB_OFFSET(base, idx, off)	((base) + (off) + (idx) * 0x20)

static inline void meram_write_icb(void __iomem *base, unsigned int idx,
				   unsigned int off, unsigned long val)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	iowrite32(val, MERAM_ICB_OFFSET(base, idx, off));
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline unsigned long meram_read_icb(void __iomem *base, int idx, int off)
=======
static inline unsigned long meram_read_icb(void __iomem *base, unsigned int idx,
					   unsigned int off)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline unsigned long meram_read_icb(void __iomem *base, unsigned int idx,
					   unsigned int off)
>>>>>>> refs/remotes/origin/master
{
	return ioread32(MERAM_ICB_OFFSET(base, idx, off));
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void meram_write_reg(void __iomem *base, int off,
		unsigned long val)
=======
static inline void meram_write_reg(void __iomem *base, unsigned int off,
				   unsigned long val)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline void meram_write_reg(void __iomem *base, unsigned int off,
				   unsigned long val)
>>>>>>> refs/remotes/origin/master
{
	iowrite32(val, base + off);
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline unsigned long meram_read_reg(void __iomem *base, int off)
=======
static inline unsigned long meram_read_reg(void __iomem *base, unsigned int off)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline unsigned long meram_read_reg(void __iomem *base, unsigned int off)
>>>>>>> refs/remotes/origin/master
{
	return ioread32(base + off);
}

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * register ICB
 */

#define MERAM_CACHE_START(p)	 ((p) >> 16)
#define MERAM_CACHE_END(p)	 ((p) & 0xffff)
#define MERAM_CACHE_SET(o, s)	 ((((o) & 0xffff) << 16) | \
				  (((o) + (s) - 1) & 0xffff))

/*
 * check if there's no overlaps in MERAM allocation.
 */

static inline int meram_check_overlap(struct sh_mobile_meram_priv *priv,
				      struct sh_mobile_meram_icb *new)
{
	int i;
	int used_start, used_end, meram_start, meram_end;

	/* valid ICB? */
	if (new->marker_icb & ~0x1f || new->cache_icb & ~0x1f)
		return 1;

	if (test_bit(new->marker_icb, &priv->used_icb) ||
			test_bit(new->cache_icb,  &priv->used_icb))
		return  1;

	for (i = 0; i < priv->used_meram_cache_regions; i++) {
		used_start = MERAM_CACHE_START(priv->used_meram_cache[i]);
		used_end   = MERAM_CACHE_END(priv->used_meram_cache[i]);
		meram_start = new->meram_offset;
		meram_end   = new->meram_offset + new->meram_size;

		if ((meram_start >= used_start && meram_start < used_end) ||
			(meram_end > used_start && meram_end < used_end))
			return 1;
	}
=======
/* -----------------------------------------------------------------------------
 * Allocation
 */

/* Allocate ICBs and MERAM for a plane. */
static int __meram_alloc(struct sh_mobile_meram_priv *priv,
			 struct sh_mobile_meram_fb_plane *plane,
			 size_t size)
=======
/* -----------------------------------------------------------------------------
 * MERAM allocation and free
 */

static unsigned long meram_alloc(struct sh_mobile_meram_priv *priv, size_t size)
{
	return gen_pool_alloc(priv->pool, size);
}

static void meram_free(struct sh_mobile_meram_priv *priv, unsigned long mem,
		       size_t size)
{
	gen_pool_free(priv->pool, mem, size);
}

/* -----------------------------------------------------------------------------
 * LCDC cache planes allocation, init, cleanup and free
 */

/* Allocate ICBs and MERAM for a plane. */
static int meram_plane_alloc(struct sh_mobile_meram_priv *priv,
			     struct sh_mobile_meram_fb_plane *plane,
			     size_t size)
>>>>>>> refs/remotes/origin/master
{
	unsigned long mem;
	unsigned long idx;

	idx = find_first_zero_bit(&priv->used_icb, 28);
	if (idx == 28)
		return -ENOMEM;
	plane->cache = &priv->icbs[idx];

	idx = find_next_zero_bit(&priv->used_icb, 32, 28);
	if (idx == 32)
		return -ENOMEM;
	plane->marker = &priv->icbs[idx];

<<<<<<< HEAD
	mem = gen_pool_alloc(priv->pool, size * 1024);
=======
	mem = meram_alloc(priv, size * 1024);
>>>>>>> refs/remotes/origin/master
	if (mem == 0)
		return -ENOMEM;

	__set_bit(plane->marker->index, &priv->used_icb);
	__set_bit(plane->cache->index, &priv->used_icb);

	plane->marker->offset = mem - priv->meram;
	plane->marker->size = size;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

<<<<<<< HEAD
/*
 * mark the specified ICB as used
 */

static inline void meram_mark(struct sh_mobile_meram_priv *priv,
			      struct sh_mobile_meram_icb *new)
{
	int n;

	if (new->marker_icb < 0 || new->cache_icb < 0)
		return;

	__set_bit(new->marker_icb, &priv->used_icb);
	__set_bit(new->cache_icb, &priv->used_icb);

	n = priv->used_meram_cache_regions;

	priv->used_meram_cache[n] = MERAM_CACHE_SET(new->meram_offset,
						    new->meram_size);

	priv->used_meram_cache_regions++;
}

/*
 * unmark the specified ICB as used
 */

static inline void meram_unmark(struct sh_mobile_meram_priv *priv,
				struct sh_mobile_meram_icb *icb)
{
	int i;
	unsigned long pattern;

	if (icb->marker_icb < 0 || icb->cache_icb < 0)
		return;

	__clear_bit(icb->marker_icb, &priv->used_icb);
	__clear_bit(icb->cache_icb, &priv->used_icb);

	pattern = MERAM_CACHE_SET(icb->meram_offset, icb->meram_size);
	for (i = 0; i < priv->used_meram_cache_regions; i++) {
		if (priv->used_meram_cache[i] == pattern) {
			while (i < priv->used_meram_cache_regions - 1) {
				priv->used_meram_cache[i] =
					priv->used_meram_cache[i + 1] ;
				i++;
			}
			priv->used_meram_cache[i] = 0;
			priv->used_meram_cache_regions--;
			break;
		}
	}
}

/*
 * is this a YCbCr(NV12, NV16 or NV24) colorspace
 */
static inline int is_nvcolor(int cspace)
{
	if (cspace == SH_MOBILE_MERAM_PF_NV ||
			cspace == SH_MOBILE_MERAM_PF_NV24)
		return 1;
	return 0;
}

/*
 * set the next address to fetch
 */
static inline void meram_set_next_addr(struct sh_mobile_meram_priv *priv,
				       struct sh_mobile_meram_cfg *cfg,
				       unsigned long base_addr_y,
				       unsigned long base_addr_c)
{
	unsigned long target;

	target = (cfg->current_reg) ? MExxSARA : MExxSARB;
	cfg->current_reg ^= 1;

	/* set the next address to fetch */
	meram_write_icb(priv->base, cfg->icb[0].cache_icb,  target,
			base_addr_y);
	meram_write_icb(priv->base, cfg->icb[0].marker_icb, target,
			base_addr_y + cfg->icb[0].cache_unit);

	if (is_nvcolor(cfg->pixelformat)) {
		meram_write_icb(priv->base, cfg->icb[1].cache_icb,  target,
				base_addr_c);
		meram_write_icb(priv->base, cfg->icb[1].marker_icb, target,
				base_addr_c + cfg->icb[1].cache_unit);
	}
}

/*
 * get the next ICB address
 */
static inline void meram_get_next_icb_addr(struct sh_mobile_meram_info *pdata,
					   struct sh_mobile_meram_cfg *cfg,
					   unsigned long *icb_addr_y,
					   unsigned long *icb_addr_c)
{
	unsigned long icb_offset;

	if (pdata->addr_mode == SH_MOBILE_MERAM_MODE0)
		icb_offset = 0x80000000 | (cfg->current_reg << 29);
	else
		icb_offset = 0xc0000000 | (cfg->current_reg << 23);

	*icb_addr_y = icb_offset | (cfg->icb[0].marker_icb << 24);
	if (is_nvcolor(cfg->pixelformat))
		*icb_addr_c = icb_offset | (cfg->icb[1].marker_icb << 24);
=======
/* Free ICBs and MERAM for a plane. */
static void __meram_free(struct sh_mobile_meram_priv *priv,
			 struct sh_mobile_meram_fb_plane *plane)
{
	gen_pool_free(priv->pool, priv->meram + plane->marker->offset,
		      plane->marker->size * 1024);
=======

	return 0;
}

/* Free ICBs and MERAM for a plane. */
static void meram_plane_free(struct sh_mobile_meram_priv *priv,
			     struct sh_mobile_meram_fb_plane *plane)
{
	meram_free(priv, priv->meram + plane->marker->offset,
		   plane->marker->size * 1024);
>>>>>>> refs/remotes/origin/master

	__clear_bit(plane->marker->index, &priv->used_icb);
	__clear_bit(plane->cache->index, &priv->used_icb);
}

/* Is this a YCbCr(NV12, NV16 or NV24) colorspace? */
static int is_nvcolor(int cspace)
{
	if (cspace == SH_MOBILE_MERAM_PF_NV ||
	    cspace == SH_MOBILE_MERAM_PF_NV24)
		return 1;
	return 0;
}

<<<<<<< HEAD
/* Allocate memory for the ICBs and mark them as used. */
static struct sh_mobile_meram_fb_cache *
meram_alloc(struct sh_mobile_meram_priv *priv,
	    const struct sh_mobile_meram_cfg *cfg,
	    int pixelformat)
{
	struct sh_mobile_meram_fb_cache *cache;
	unsigned int nplanes = is_nvcolor(pixelformat) ? 2 : 1;
	int ret;

	if (cfg->icb[0].meram_size == 0)
		return ERR_PTR(-EINVAL);

	if (nplanes == 2 && cfg->icb[1].meram_size == 0)
		return ERR_PTR(-EINVAL);

	cache = kzalloc(sizeof(*cache), GFP_KERNEL);
	if (cache == NULL)
		return ERR_PTR(-ENOMEM);

	cache->nplanes = nplanes;

	ret = __meram_alloc(priv, &cache->planes[0], cfg->icb[0].meram_size);
	if (ret < 0)
		goto error;

	cache->planes[0].marker->current_reg = 1;
	cache->planes[0].marker->pixelformat = pixelformat;

	if (cache->nplanes == 1)
		return cache;

	ret = __meram_alloc(priv, &cache->planes[1], cfg->icb[1].meram_size);
	if (ret < 0) {
		__meram_free(priv, &cache->planes[0]);
		goto error;
	}

	return cache;

error:
	kfree(cache);
	return ERR_PTR(-ENOMEM);
}

/* Unmark the specified ICB as used. */
static void meram_free(struct sh_mobile_meram_priv *priv,
		       struct sh_mobile_meram_fb_cache *cache)
{
	__meram_free(priv, &cache->planes[0]);
	if (cache->nplanes == 2)
		__meram_free(priv, &cache->planes[1]);

	kfree(cache);
}

=======
>>>>>>> refs/remotes/origin/master
/* Set the next address to fetch. */
static void meram_set_next_addr(struct sh_mobile_meram_priv *priv,
				struct sh_mobile_meram_fb_cache *cache,
				unsigned long base_addr_y,
				unsigned long base_addr_c)
{
	struct sh_mobile_meram_icb *icb = cache->planes[0].marker;
	unsigned long target;

	icb->current_reg ^= 1;
	target = icb->current_reg ? MExxSARB : MExxSARA;

	/* set the next address to fetch */
	meram_write_icb(priv->base, cache->planes[0].cache->index, target,
			base_addr_y);
	meram_write_icb(priv->base, cache->planes[0].marker->index, target,
			base_addr_y + cache->planes[0].marker->cache_unit);

	if (cache->nplanes == 2) {
		meram_write_icb(priv->base, cache->planes[1].cache->index,
				target, base_addr_c);
		meram_write_icb(priv->base, cache->planes[1].marker->index,
				target, base_addr_c +
				cache->planes[1].marker->cache_unit);
	}
}

/* Get the next ICB address. */
static void
meram_get_next_icb_addr(struct sh_mobile_meram_info *pdata,
			struct sh_mobile_meram_fb_cache *cache,
			unsigned long *icb_addr_y, unsigned long *icb_addr_c)
{
	struct sh_mobile_meram_icb *icb = cache->planes[0].marker;
	unsigned long icb_offset;

	if (pdata->addr_mode == SH_MOBILE_MERAM_MODE0)
		icb_offset = 0x80000000 | (icb->current_reg << 29);
	else
		icb_offset = 0xc0000000 | (icb->current_reg << 23);

	*icb_addr_y = icb_offset | (cache->planes[0].marker->index << 24);
	if (cache->nplanes == 2)
		*icb_addr_c = icb_offset
			    | (cache->planes[1].marker->index << 24);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

#define MERAM_CALC_BYTECOUNT(x, y) \
	(((x) * (y) + (MERAM_LINE_WIDTH - 1)) & ~(MERAM_LINE_WIDTH - 1))

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * initialize MERAM
 */

static int meram_init(struct sh_mobile_meram_priv *priv,
		      struct sh_mobile_meram_icb *icb,
		      int xres, int yres, int *out_pitch)
{
	unsigned long total_byte_count = MERAM_CALC_BYTECOUNT(xres, yres);
	unsigned long bnm;
	int lcdc_pitch, xpitch, line_cnt;
	int save_lines;
=======
/* Initialize MERAM. */
static int meram_init(struct sh_mobile_meram_priv *priv,
		      struct sh_mobile_meram_fb_plane *plane,
		      unsigned int xres, unsigned int yres,
		      unsigned int *out_pitch)
=======
/* Initialize MERAM. */
static int meram_plane_init(struct sh_mobile_meram_priv *priv,
			    struct sh_mobile_meram_fb_plane *plane,
			    unsigned int xres, unsigned int yres,
			    unsigned int *out_pitch)
>>>>>>> refs/remotes/origin/master
{
	struct sh_mobile_meram_icb *marker = plane->marker;
	unsigned long total_byte_count = MERAM_CALC_BYTECOUNT(xres, yres);
	unsigned long bnm;
	unsigned int lcdc_pitch;
	unsigned int xpitch;
	unsigned int line_cnt;
	unsigned int save_lines;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* adjust pitch to 1024, 2048, 4096 or 8192 */
	lcdc_pitch = (xres - 1) | 1023;
	lcdc_pitch = lcdc_pitch | (lcdc_pitch >> 1);
	lcdc_pitch = lcdc_pitch | (lcdc_pitch >> 2);
	lcdc_pitch += 1;

	/* derive settings */
	if (lcdc_pitch == 8192 && yres >= 1024) {
		lcdc_pitch = xpitch = MERAM_LINE_WIDTH;
		line_cnt = total_byte_count >> 11;
		*out_pitch = xres;
<<<<<<< HEAD
<<<<<<< HEAD
		save_lines = (icb->meram_size / 16 / MERAM_SEC_LINE);
=======
		save_lines = plane->marker->size / 16 / MERAM_SEC_LINE;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		save_lines = plane->marker->size / 16 / MERAM_SEC_LINE;
>>>>>>> refs/remotes/origin/master
		save_lines *= MERAM_SEC_LINE;
	} else {
		xpitch = xres;
		line_cnt = yres;
		*out_pitch = lcdc_pitch;
<<<<<<< HEAD
<<<<<<< HEAD
		save_lines = icb->meram_size / (lcdc_pitch >> 10) / 2;
=======
		save_lines = plane->marker->size / (lcdc_pitch >> 10) / 2;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		save_lines = plane->marker->size / (lcdc_pitch >> 10) / 2;
>>>>>>> refs/remotes/origin/master
		save_lines &= 0xff;
	}
	bnm = (save_lines - 1) << 16;

	/* TODO: we better to check if we have enough MERAM buffer size */

	/* set up ICB */
<<<<<<< HEAD
<<<<<<< HEAD
	meram_write_icb(priv->base, icb->cache_icb,  MExxBSIZE,
			MERAM_MExxBSIZE_VAL(0x0, line_cnt - 1, xpitch - 1));
	meram_write_icb(priv->base, icb->marker_icb, MExxBSIZE,
			MERAM_MExxBSIZE_VAL(0xf, line_cnt - 1, xpitch - 1));

	meram_write_icb(priv->base, icb->cache_icb,  MExxMNCF, bnm);
	meram_write_icb(priv->base, icb->marker_icb, MExxMNCF, bnm);

	meram_write_icb(priv->base, icb->cache_icb,  MExxSBSIZE, xpitch);
	meram_write_icb(priv->base, icb->marker_icb, MExxSBSIZE, xpitch);

	/* save a cache unit size */
	icb->cache_unit = xres * save_lines;
=======
=======
>>>>>>> refs/remotes/origin/master
	meram_write_icb(priv->base, plane->cache->index,  MExxBSIZE,
			MERAM_MExxBSIZE_VAL(0x0, line_cnt - 1, xpitch - 1));
	meram_write_icb(priv->base, plane->marker->index, MExxBSIZE,
			MERAM_MExxBSIZE_VAL(0xf, line_cnt - 1, xpitch - 1));

	meram_write_icb(priv->base, plane->cache->index,  MExxMNCF, bnm);
	meram_write_icb(priv->base, plane->marker->index, MExxMNCF, bnm);

	meram_write_icb(priv->base, plane->cache->index,  MExxSBSIZE, xpitch);
	meram_write_icb(priv->base, plane->marker->index, MExxSBSIZE, xpitch);

	/* save a cache unit size */
	plane->cache->cache_unit = xres * save_lines;
	plane->marker->cache_unit = xres * save_lines;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * Set MERAM for framebuffer
	 *
<<<<<<< HEAD
<<<<<<< HEAD
	 * 0x70f:  WD = 0x3, WS=0x1, CM=0x1, MD=FB mode
	 * we also chain the cache_icb and the marker_icb.
	 * we also split the allocated MERAM buffer between two ICBs.
	 */
	meram_write_icb(priv->base, icb->cache_icb, MExxCTL,
			MERAM_MExxCTL_VAL(0x70f, icb->marker_icb,
					  icb->meram_offset));
	meram_write_icb(priv->base, icb->marker_icb, MExxCTL,
			MERAM_MExxCTL_VAL(0x70f, icb->cache_icb,
					  icb->meram_offset +
					  icb->meram_size / 2));
=======
=======
>>>>>>> refs/remotes/origin/master
	 * we also chain the cache_icb and the marker_icb.
	 * we also split the allocated MERAM buffer between two ICBs.
	 */
	meram_write_icb(priv->base, plane->cache->index, MExxCTL,
			MERAM_MExxCTL_VAL(plane->marker->index, marker->offset)
			| MExxCTL_WD1 | MExxCTL_WD0 | MExxCTL_WS | MExxCTL_CM |
			MExxCTL_MD_FB);
	meram_write_icb(priv->base, plane->marker->index, MExxCTL,
			MERAM_MExxCTL_VAL(plane->cache->index, marker->offset +
					  plane->marker->size / 2) |
			MExxCTL_WD1 | MExxCTL_WD0 | MExxCTL_WS | MExxCTL_CM |
			MExxCTL_MD_FB);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static void meram_deinit(struct sh_mobile_meram_priv *priv,
<<<<<<< HEAD
			struct sh_mobile_meram_icb *icb)
{
	/* disable ICB */
	meram_write_icb(priv->base, icb->cache_icb,  MExxCTL, 0);
	meram_write_icb(priv->base, icb->marker_icb, MExxCTL, 0);
	icb->cache_unit = 0;
}

/*
 * register the ICB
 */

static int sh_mobile_meram_register(struct sh_mobile_meram_info *pdata,
				    struct sh_mobile_meram_cfg *cfg,
				    int xres, int yres, int pixelformat,
				    unsigned long base_addr_y,
				    unsigned long base_addr_c,
				    unsigned long *icb_addr_y,
				    unsigned long *icb_addr_c,
				    int *pitch)
{
	struct platform_device *pdev;
	struct sh_mobile_meram_priv *priv;
	int n, out_pitch;
	int error = 0;

	if (!pdata || !pdata->priv || !pdata->pdev || !cfg)
		return -EINVAL;
=======
			 struct sh_mobile_meram_fb_plane *plane)
=======
static void meram_plane_cleanup(struct sh_mobile_meram_priv *priv,
				struct sh_mobile_meram_fb_plane *plane)
>>>>>>> refs/remotes/origin/master
{
	/* disable ICB */
	meram_write_icb(priv->base, plane->cache->index,  MExxCTL,
			MExxCTL_WBF | MExxCTL_WF | MExxCTL_RF);
	meram_write_icb(priv->base, plane->marker->index, MExxCTL,
			MExxCTL_WBF | MExxCTL_WF | MExxCTL_RF);

	plane->cache->cache_unit = 0;
	plane->marker->cache_unit = 0;
}

/* -----------------------------------------------------------------------------
<<<<<<< HEAD
 * Registration/unregistration
 */

static void *sh_mobile_meram_register(struct sh_mobile_meram_info *pdata,
				      const struct sh_mobile_meram_cfg *cfg,
				      unsigned int xres, unsigned int yres,
				      unsigned int pixelformat,
				      unsigned int *pitch)
{
	struct sh_mobile_meram_fb_cache *cache;
	struct sh_mobile_meram_priv *priv = pdata->priv;
	struct platform_device *pdev = pdata->pdev;
	unsigned int out_pitch;
>>>>>>> refs/remotes/origin/cm-10.0

	if (pixelformat != SH_MOBILE_MERAM_PF_NV &&
	    pixelformat != SH_MOBILE_MERAM_PF_NV24 &&
	    pixelformat != SH_MOBILE_MERAM_PF_RGB)
<<<<<<< HEAD
		return -EINVAL;

	priv = pdata->priv;
	pdev = pdata->pdev;

	dev_dbg(&pdev->dev, "registering %dx%d (%s) (y=%08lx, c=%08lx)",
		xres, yres, (!pixelformat) ? "yuv" : "rgb",
		base_addr_y, base_addr_c);

	mutex_lock(&priv->lock);
=======
		return ERR_PTR(-EINVAL);

	dev_dbg(&pdev->dev, "registering %dx%d (%s)", xres, yres,
		!pixelformat ? "yuv" : "rgb");
>>>>>>> refs/remotes/origin/cm-10.0

	/* we can't handle wider than 8192px */
	if (xres > 8192) {
		dev_err(&pdev->dev, "width exceeding the limit (> 8192).");
<<<<<<< HEAD
		error = -EINVAL;
		goto err;
	}

	if (priv->used_meram_cache_regions + 2 > SH_MOBILE_MERAM_ICB_NUM) {
		dev_err(&pdev->dev, "no more ICB available.");
		error = -EINVAL;
		goto err;
	}

	/* do we have at least one ICB config? */
	if (cfg->icb[0].marker_icb < 0 || cfg->icb[0].cache_icb < 0) {
		dev_err(&pdev->dev, "at least one ICB is required.");
		error = -EINVAL;
		goto err;
	}

	/* make sure that there's no overlaps */
	if (meram_check_overlap(priv, &cfg->icb[0])) {
		dev_err(&pdev->dev, "conflicting config detected.");
		error = -EINVAL;
		goto err;
	}
	n = 1;

	/* do the same if we have the second ICB set */
	if (cfg->icb[1].marker_icb >= 0 && cfg->icb[1].cache_icb >= 0) {
		if (meram_check_overlap(priv, &cfg->icb[1])) {
			dev_err(&pdev->dev, "conflicting config detected.");
			error = -EINVAL;
			goto err;
		}
		n = 2;
	}

	if (is_nvcolor(pixelformat) && n != 2) {
		dev_err(&pdev->dev, "requires two ICB sets for planar Y/C.");
		error =  -EINVAL;
		goto err;
	}

	/* we now register the ICB */
	cfg->pixelformat = pixelformat;
	meram_mark(priv, &cfg->icb[0]);
	if (is_nvcolor(pixelformat))
		meram_mark(priv, &cfg->icb[1]);

	/* initialize MERAM */
	meram_init(priv, &cfg->icb[0], xres, yres, &out_pitch);
	*pitch = out_pitch;
	if (pixelformat == SH_MOBILE_MERAM_PF_NV)
		meram_init(priv, &cfg->icb[1], xres, (yres + 1) / 2,
			&out_pitch);
	else if (pixelformat == SH_MOBILE_MERAM_PF_NV24)
		meram_init(priv, &cfg->icb[1], 2 * xres, (yres + 1) / 2,
			&out_pitch);

	cfg->current_reg = 1;
	meram_set_next_addr(priv, cfg, base_addr_y, base_addr_c);
	meram_get_next_icb_addr(pdata, cfg, icb_addr_y, icb_addr_c);

	dev_dbg(&pdev->dev, "registered - can access via y=%08lx, c=%08lx",
		*icb_addr_y, *icb_addr_c);

err:
	mutex_unlock(&priv->lock);
	return error;
}

static int sh_mobile_meram_unregister(struct sh_mobile_meram_info *pdata,
				      struct sh_mobile_meram_cfg *cfg)
{
	struct sh_mobile_meram_priv *priv;

	if (!pdata || !pdata->priv || !cfg)
		return -EINVAL;

	priv = pdata->priv;

	mutex_lock(&priv->lock);

	/* deinit & unmark */
	if (is_nvcolor(cfg->pixelformat)) {
		meram_deinit(priv, &cfg->icb[1]);
		meram_unmark(priv, &cfg->icb[1]);
	}
	meram_deinit(priv, &cfg->icb[0]);
	meram_unmark(priv, &cfg->icb[0]);

	mutex_unlock(&priv->lock);

	return 0;
}

static int sh_mobile_meram_update(struct sh_mobile_meram_info *pdata,
				  struct sh_mobile_meram_cfg *cfg,
				  unsigned long base_addr_y,
				  unsigned long base_addr_c,
				  unsigned long *icb_addr_y,
				  unsigned long *icb_addr_c)
{
	struct sh_mobile_meram_priv *priv;

	if (!pdata || !pdata->priv || !cfg)
		return -EINVAL;

	priv = pdata->priv;

	mutex_lock(&priv->lock);

	meram_set_next_addr(priv, cfg, base_addr_y, base_addr_c);
	meram_get_next_icb_addr(pdata, cfg, icb_addr_y, icb_addr_c);

	mutex_unlock(&priv->lock);

	return 0;
=======
		return ERR_PTR(-EINVAL);
	}
=======
 * MERAM operations
 */

unsigned long sh_mobile_meram_alloc(struct sh_mobile_meram_info *pdata,
				    size_t size)
{
	struct sh_mobile_meram_priv *priv = pdata->priv;

	return meram_alloc(priv, size);
}
EXPORT_SYMBOL_GPL(sh_mobile_meram_alloc);

void sh_mobile_meram_free(struct sh_mobile_meram_info *pdata, unsigned long mem,
			  size_t size)
{
	struct sh_mobile_meram_priv *priv = pdata->priv;

	meram_free(priv, mem, size);
}
EXPORT_SYMBOL_GPL(sh_mobile_meram_free);

/* Allocate memory for the ICBs and mark them as used. */
static struct sh_mobile_meram_fb_cache *
meram_cache_alloc(struct sh_mobile_meram_priv *priv,
		  const struct sh_mobile_meram_cfg *cfg,
		  int pixelformat)
{
	unsigned int nplanes = is_nvcolor(pixelformat) ? 2 : 1;
	struct sh_mobile_meram_fb_cache *cache;
	int ret;

	cache = kzalloc(sizeof(*cache), GFP_KERNEL);
	if (cache == NULL)
		return ERR_PTR(-ENOMEM);

	cache->nplanes = nplanes;

	ret = meram_plane_alloc(priv, &cache->planes[0],
				cfg->icb[0].meram_size);
	if (ret < 0)
		goto error;

	cache->planes[0].marker->current_reg = 1;
	cache->planes[0].marker->pixelformat = pixelformat;

	if (cache->nplanes == 1)
		return cache;

	ret = meram_plane_alloc(priv, &cache->planes[1],
				cfg->icb[1].meram_size);
	if (ret < 0) {
		meram_plane_free(priv, &cache->planes[0]);
		goto error;
	}

	return cache;

error:
	kfree(cache);
	return ERR_PTR(-ENOMEM);
}

void *sh_mobile_meram_cache_alloc(struct sh_mobile_meram_info *pdata,
				  const struct sh_mobile_meram_cfg *cfg,
				  unsigned int xres, unsigned int yres,
				  unsigned int pixelformat, unsigned int *pitch)
{
	struct sh_mobile_meram_fb_cache *cache;
	struct sh_mobile_meram_priv *priv = pdata->priv;
	struct platform_device *pdev = pdata->pdev;
	unsigned int nplanes = is_nvcolor(pixelformat) ? 2 : 1;
	unsigned int out_pitch;

	if (priv == NULL)
		return ERR_PTR(-ENODEV);

	if (pixelformat != SH_MOBILE_MERAM_PF_NV &&
	    pixelformat != SH_MOBILE_MERAM_PF_NV24 &&
	    pixelformat != SH_MOBILE_MERAM_PF_RGB)
		return ERR_PTR(-EINVAL);

	dev_dbg(&pdev->dev, "registering %dx%d (%s)", xres, yres,
		!pixelformat ? "yuv" : "rgb");

	/* we can't handle wider than 8192px */
	if (xres > 8192) {
		dev_err(&pdev->dev, "width exceeding the limit (> 8192).");
		return ERR_PTR(-EINVAL);
	}

	if (cfg->icb[0].meram_size == 0)
		return ERR_PTR(-EINVAL);

	if (nplanes == 2 && cfg->icb[1].meram_size == 0)
		return ERR_PTR(-EINVAL);
>>>>>>> refs/remotes/origin/master

	mutex_lock(&priv->lock);

	/* We now register the ICBs and allocate the MERAM regions. */
<<<<<<< HEAD
	cache = meram_alloc(priv, cfg, pixelformat);
=======
	cache = meram_cache_alloc(priv, cfg, pixelformat);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(cache)) {
		dev_err(&pdev->dev, "MERAM allocation failed (%ld).",
			PTR_ERR(cache));
		goto err;
	}

	/* initialize MERAM */
<<<<<<< HEAD
	meram_init(priv, &cache->planes[0], xres, yres, &out_pitch);
	*pitch = out_pitch;
	if (pixelformat == SH_MOBILE_MERAM_PF_NV)
		meram_init(priv, &cache->planes[1], xres, (yres + 1) / 2,
			&out_pitch);
	else if (pixelformat == SH_MOBILE_MERAM_PF_NV24)
		meram_init(priv, &cache->planes[1], 2 * xres, (yres + 1) / 2,
			&out_pitch);
=======
	meram_plane_init(priv, &cache->planes[0], xres, yres, &out_pitch);
	*pitch = out_pitch;
	if (pixelformat == SH_MOBILE_MERAM_PF_NV)
		meram_plane_init(priv, &cache->planes[1],
				 xres, (yres + 1) / 2, &out_pitch);
	else if (pixelformat == SH_MOBILE_MERAM_PF_NV24)
		meram_plane_init(priv, &cache->planes[1],
				 2 * xres, (yres + 1) / 2, &out_pitch);
>>>>>>> refs/remotes/origin/master

err:
	mutex_unlock(&priv->lock);
	return cache;
}
<<<<<<< HEAD

static void
sh_mobile_meram_unregister(struct sh_mobile_meram_info *pdata, void *data)
=======
EXPORT_SYMBOL_GPL(sh_mobile_meram_cache_alloc);

void
sh_mobile_meram_cache_free(struct sh_mobile_meram_info *pdata, void *data)
>>>>>>> refs/remotes/origin/master
{
	struct sh_mobile_meram_fb_cache *cache = data;
	struct sh_mobile_meram_priv *priv = pdata->priv;

	mutex_lock(&priv->lock);

<<<<<<< HEAD
	/* deinit & free */
	meram_deinit(priv, &cache->planes[0]);
	if (cache->nplanes == 2)
		meram_deinit(priv, &cache->planes[1]);

	meram_free(priv, cache);

	mutex_unlock(&priv->lock);
}

static void
sh_mobile_meram_update(struct sh_mobile_meram_info *pdata, void *data,
		       unsigned long base_addr_y, unsigned long base_addr_c,
		       unsigned long *icb_addr_y, unsigned long *icb_addr_c)
=======
	/* Cleanup and free. */
	meram_plane_cleanup(priv, &cache->planes[0]);
	meram_plane_free(priv, &cache->planes[0]);

	if (cache->nplanes == 2) {
		meram_plane_cleanup(priv, &cache->planes[1]);
		meram_plane_free(priv, &cache->planes[1]);
	}

	kfree(cache);

	mutex_unlock(&priv->lock);
}
EXPORT_SYMBOL_GPL(sh_mobile_meram_cache_free);

void
sh_mobile_meram_cache_update(struct sh_mobile_meram_info *pdata, void *data,
			     unsigned long base_addr_y,
			     unsigned long base_addr_c,
			     unsigned long *icb_addr_y,
			     unsigned long *icb_addr_c)
>>>>>>> refs/remotes/origin/master
{
	struct sh_mobile_meram_fb_cache *cache = data;
	struct sh_mobile_meram_priv *priv = pdata->priv;

	mutex_lock(&priv->lock);

	meram_set_next_addr(priv, cache, base_addr_y, base_addr_c);
	meram_get_next_icb_addr(pdata, cache, icb_addr_y, icb_addr_c);

	mutex_unlock(&priv->lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static struct sh_mobile_meram_ops sh_mobile_meram_ops = {
	.module			= THIS_MODULE,
	.meram_register		= sh_mobile_meram_register,
	.meram_unregister	= sh_mobile_meram_unregister,
	.meram_update		= sh_mobile_meram_update,
};

<<<<<<< HEAD
/*
 * initialize MERAM
 */

static int sh_mobile_meram_remove(struct platform_device *pdev);
=======
=======
}
EXPORT_SYMBOL_GPL(sh_mobile_meram_cache_update);

>>>>>>> refs/remotes/origin/master
/* -----------------------------------------------------------------------------
 * Power management
 */

<<<<<<< HEAD
=======
#if defined(CONFIG_PM_SLEEP) || defined(CONFIG_PM_RUNTIME)
>>>>>>> refs/remotes/origin/master
static int sh_mobile_meram_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct sh_mobile_meram_priv *priv = platform_get_drvdata(pdev);
	unsigned int i, j;

	for (i = 0; i < MERAM_REGS_SIZE; i++)
		priv->regs[i] = meram_read_reg(priv->base, common_regs[i]);

	for (i = 0; i < 32; i++) {
		if (!test_bit(i, &priv->used_icb))
			continue;
		for (j = 0; j < ICB_REGS_SIZE; j++) {
			priv->icbs[i].regs[j] =
				meram_read_icb(priv->base, i, icb_regs[j]);
			/* Reset ICB on resume */
			if (icb_regs[j] == MExxCTL)
				priv->icbs[i].regs[j] |=
					MExxCTL_WBF | MExxCTL_WF | MExxCTL_RF;
		}
	}
	return 0;
}

static int sh_mobile_meram_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct sh_mobile_meram_priv *priv = platform_get_drvdata(pdev);
	unsigned int i, j;

	for (i = 0; i < 32; i++) {
		if (!test_bit(i, &priv->used_icb))
			continue;
		for (j = 0; j < ICB_REGS_SIZE; j++)
			meram_write_icb(priv->base, i, icb_regs[j],
					priv->icbs[i].regs[j]);
	}

	for (i = 0; i < MERAM_REGS_SIZE; i++)
		meram_write_reg(priv->base, common_regs[i], priv->regs[i]);
	return 0;
}
<<<<<<< HEAD
=======
#endif /* CONFIG_PM_SLEEP || CONFIG_PM_RUNTIME */
>>>>>>> refs/remotes/origin/master

static UNIVERSAL_DEV_PM_OPS(sh_mobile_meram_dev_pm_ops,
			    sh_mobile_meram_suspend,
			    sh_mobile_meram_resume, NULL);

/* -----------------------------------------------------------------------------
 * Probe/remove and driver init/exit
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

static int __devinit sh_mobile_meram_probe(struct platform_device *pdev)
{
	struct sh_mobile_meram_priv *priv;
	struct sh_mobile_meram_info *pdata = pdev->dev.platform_data;
<<<<<<< HEAD
	struct resource *res;
=======
	struct resource *regs;
	struct resource *meram;
	unsigned int i;
>>>>>>> refs/remotes/origin/cm-10.0
=======

static int sh_mobile_meram_probe(struct platform_device *pdev)
{
	struct sh_mobile_meram_priv *priv;
	struct sh_mobile_meram_info *pdata = pdev->dev.platform_data;
	struct resource *regs;
	struct resource *meram;
	unsigned int i;
>>>>>>> refs/remotes/origin/master
	int error;

	if (!pdata) {
		dev_err(&pdev->dev, "no platform data defined\n");
		return -EINVAL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
=======
	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	meram = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (regs == NULL || meram == NULL) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	meram = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (regs == NULL || meram == NULL) {
>>>>>>> refs/remotes/origin/master
		dev_err(&pdev->dev, "cannot get platform resources\n");
		return -ENOENT;
	}

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		dev_err(&pdev->dev, "cannot allocate device data\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	platform_set_drvdata(pdev, priv);

	/* initialize private data */
	mutex_init(&priv->lock);
	priv->base = ioremap_nocache(res->start, resource_size(res));
	if (!priv->base) {
		dev_err(&pdev->dev, "ioremap failed\n");
		error = -EFAULT;
		goto err;
	}
	pdata->ops = &sh_mobile_meram_ops;
	pdata->priv = priv;
	pdata->pdev = pdev;

	/* initialize ICB addressing mode */
	if (pdata->addr_mode == SH_MOBILE_MERAM_MODE1)
		meram_write_reg(priv->base, MEVCR1, 1 << 29);
=======
=======
>>>>>>> refs/remotes/origin/master
	/* Initialize private data. */
	mutex_init(&priv->lock);
	priv->used_icb = pdata->reserved_icbs;

	for (i = 0; i < MERAM_ICB_NUM; ++i)
		priv->icbs[i].index = i;

<<<<<<< HEAD
	pdata->ops = &sh_mobile_meram_ops;
=======
>>>>>>> refs/remotes/origin/master
	pdata->priv = priv;
	pdata->pdev = pdev;

	/* Request memory regions and remap the registers. */
	if (!request_mem_region(regs->start, resource_size(regs), pdev->name)) {
		dev_err(&pdev->dev, "MERAM registers region already claimed\n");
		error = -EBUSY;
		goto err_req_regs;
	}

	if (!request_mem_region(meram->start, resource_size(meram),
				pdev->name)) {
		dev_err(&pdev->dev, "MERAM memory region already claimed\n");
		error = -EBUSY;
		goto err_req_meram;
	}

	priv->base = ioremap_nocache(regs->start, resource_size(regs));
	if (!priv->base) {
		dev_err(&pdev->dev, "ioremap failed\n");
		error = -EFAULT;
		goto err_ioremap;
	}

	priv->meram = meram->start;

	/* Create and initialize the MERAM memory pool. */
	priv->pool = gen_pool_create(ilog2(MERAM_GRANULARITY), -1);
	if (priv->pool == NULL) {
		error = -ENOMEM;
		goto err_genpool;
	}

	error = gen_pool_add(priv->pool, meram->start, resource_size(meram),
			     -1);
	if (error < 0)
		goto err_genpool;

	/* initialize ICB addressing mode */
	if (pdata->addr_mode == SH_MOBILE_MERAM_MODE1)
		meram_write_reg(priv->base, MEVCR1, MEVCR1_AMD1);

	platform_set_drvdata(pdev, priv);
	pm_runtime_enable(&pdev->dev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	dev_info(&pdev->dev, "sh_mobile_meram initialized.");

	return 0;

<<<<<<< HEAD
<<<<<<< HEAD
err:
	sh_mobile_meram_remove(pdev);
=======
=======
>>>>>>> refs/remotes/origin/master
err_genpool:
	if (priv->pool)
		gen_pool_destroy(priv->pool);
	iounmap(priv->base);
err_ioremap:
	release_mem_region(meram->start, resource_size(meram));
err_req_meram:
	release_mem_region(regs->start, resource_size(regs));
err_req_regs:
	mutex_destroy(&priv->lock);
	kfree(priv);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return error;
}


static int sh_mobile_meram_remove(struct platform_device *pdev)
{
	struct sh_mobile_meram_priv *priv = platform_get_drvdata(pdev);
<<<<<<< HEAD
<<<<<<< HEAD

	if (priv->base)
		iounmap(priv->base);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct resource *regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	struct resource *meram = platform_get_resource(pdev, IORESOURCE_MEM, 1);

	pm_runtime_disable(&pdev->dev);

	gen_pool_destroy(priv->pool);

	iounmap(priv->base);
	release_mem_region(meram->start, resource_size(meram));
	release_mem_region(regs->start, resource_size(regs));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	mutex_destroy(&priv->lock);

	kfree(priv);

	return 0;
}

static struct platform_driver sh_mobile_meram_driver = {
	.driver	= {
		.name		= "sh_mobile_meram",
		.owner		= THIS_MODULE,
<<<<<<< HEAD
<<<<<<< HEAD
=======
		.pm		= &sh_mobile_meram_dev_pm_ops,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.pm		= &sh_mobile_meram_dev_pm_ops,
>>>>>>> refs/remotes/origin/master
	},
	.probe		= sh_mobile_meram_probe,
	.remove		= sh_mobile_meram_remove,
};

<<<<<<< HEAD
<<<<<<< HEAD
static int __init sh_mobile_meram_init(void)
{
	return platform_driver_register(&sh_mobile_meram_driver);
}

static void __exit sh_mobile_meram_exit(void)
{
	platform_driver_unregister(&sh_mobile_meram_driver);
}

module_init(sh_mobile_meram_init);
module_exit(sh_mobile_meram_exit);
=======
module_platform_driver(sh_mobile_meram_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_platform_driver(sh_mobile_meram_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("SuperH Mobile MERAM driver");
MODULE_AUTHOR("Damian Hobson-Garcia / Takanari Hayama");
MODULE_LICENSE("GPL v2");
