/*
 * Copyright 2010 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: Ben Skeggs
 */

#include "drmP.h"
#include "nouveau_drv.h"
#include "nouveau_bios.h"
#include "nouveau_pm.h"

<<<<<<< HEAD
/* This is actually a lot more complex than it appears here, but hopefully
 * this should be able to deal with what the VBIOS leaves for us..
 *
 * If not, well, I'll jump off that bridge when I come to it.
 */

struct nva3_pm_state {
	enum pll_types type;
	u32 src0;
	u32 src1;
	u32 ctrl;
	u32 coef;
	u32 old_pnm;
	u32 new_pnm;
	u32 new_div;
};

static int
nva3_pm_pll_offset(u32 id)
{
	static const u32 pll_map[] = {
		0x00, PLL_CORE,
		0x01, PLL_SHADER,
		0x02, PLL_MEMORY,
		0x00, 0x00
	};
	const u32 *map = pll_map;

	while (map[1]) {
		if (id == map[1])
			return map[0];
		map += 2;
	}

	return -ENOENT;
}

int
nva3_pm_clock_get(struct drm_device *dev, u32 id)
{
	u32 src0, src1, ctrl, coef;
	struct pll_lims pll;
	int ret, off;
	int P, N, M;

	ret = get_pll_limits(dev, id, &pll);
	if (ret)
		return ret;

	off = nva3_pm_pll_offset(id);
	if (off < 0)
		return off;

	src0 = nv_rd32(dev, 0x4120 + (off * 4));
	src1 = nv_rd32(dev, 0x4160 + (off * 4));
	ctrl = nv_rd32(dev, pll.reg + 0);
	coef = nv_rd32(dev, pll.reg + 4);
	NV_DEBUG(dev, "PLL %02x: 0x%08x 0x%08x 0x%08x 0x%08x\n",
		      id, src0, src1, ctrl, coef);

	if (ctrl & 0x00000008) {
		u32 div = ((src1 & 0x003c0000) >> 18) + 1;
		return (pll.refclk * 2) / div;
	}

	P = (coef & 0x003f0000) >> 16;
	N = (coef & 0x0000ff00) >> 8;
	M = (coef & 0x000000ff);
	return pll.refclk * N / M / P;
}

void *
nva3_pm_clock_pre(struct drm_device *dev, struct nouveau_pm_level *perflvl,
		  u32 id, int khz)
{
	struct nva3_pm_state *pll;
	struct pll_lims limits;
	int N, M, P, diff;
	int ret, off;

	ret = get_pll_limits(dev, id, &limits);
	if (ret < 0)
		return (ret == -ENOENT) ? NULL : ERR_PTR(ret);

	off = nva3_pm_pll_offset(id);
	if (id < 0)
		return ERR_PTR(-EINVAL);


	pll = kzalloc(sizeof(*pll), GFP_KERNEL);
	if (!pll)
		return ERR_PTR(-ENOMEM);
	pll->type = id;
	pll->src0 = 0x004120 + (off * 4);
	pll->src1 = 0x004160 + (off * 4);
	pll->ctrl = limits.reg + 0;
	pll->coef = limits.reg + 4;

	/* If target clock is within [-2, 3) MHz of a divisor, we'll
	 * use that instead of calculating MNP values
	 */
	pll->new_div = min((limits.refclk * 2) / (khz - 2999), 16);
	if (pll->new_div) {
		diff = khz - ((limits.refclk * 2) / pll->new_div);
		if (diff < -2000 || diff >= 3000)
			pll->new_div = 0;
	}

	if (!pll->new_div) {
		ret = nva3_calc_pll(dev, &limits, khz, &N, NULL, &M, &P);
		if (ret < 0)
			return ERR_PTR(ret);

		pll->new_pnm = (P << 16) | (N << 8) | M;
		pll->new_div = 2 - 1;
	} else {
		pll->new_pnm = 0;
		pll->new_div--;
	}

	if ((nv_rd32(dev, pll->src1) & 0x00000101) != 0x00000101)
		pll->old_pnm = nv_rd32(dev, pll->coef);
	return pll;
}

void
nva3_pm_clock_set(struct drm_device *dev, void *pre_state)
{
	struct nva3_pm_state *pll = pre_state;
	u32 ctrl = 0;

	/* For the memory clock, NVIDIA will build a "script" describing
	 * the reclocking process and ask PDAEMON to execute it.
	 */
	if (pll->type == PLL_MEMORY) {
		nv_wr32(dev, 0x100210, 0);
		nv_wr32(dev, 0x1002dc, 1);
		nv_wr32(dev, 0x004018, 0x00001000);
		ctrl = 0x18000100;
	}

	if (pll->old_pnm || !pll->new_pnm) {
		nv_mask(dev, pll->src1, 0x003c0101, 0x00000101 |
						    (pll->new_div << 18));
		nv_wr32(dev, pll->ctrl, 0x0001001d | ctrl);
		nv_mask(dev, pll->ctrl, 0x00000001, 0x00000000);
	}

	if (pll->new_pnm) {
		nv_mask(dev, pll->src0, 0x00000101, 0x00000101);
		nv_wr32(dev, pll->coef, pll->new_pnm);
		nv_wr32(dev, pll->ctrl, 0x0001001d | ctrl);
		nv_mask(dev, pll->ctrl, 0x00000010, 0x00000000);
		nv_mask(dev, pll->ctrl, 0x00020010, 0x00020010);
		nv_wr32(dev, pll->ctrl, 0x00010015 | ctrl);
		nv_mask(dev, pll->src1, 0x00000100, 0x00000000);
		nv_mask(dev, pll->src1, 0x00000001, 0x00000000);
		if (pll->type == PLL_MEMORY)
			nv_wr32(dev, 0x4018, 0x10005000);
	} else {
		nv_mask(dev, pll->ctrl, 0x00000001, 0x00000000);
		nv_mask(dev, pll->src0, 0x00000100, 0x00000000);
		nv_mask(dev, pll->src0, 0x00000001, 0x00000000);
		if (pll->type == PLL_MEMORY)
			nv_wr32(dev, 0x4018, 0x1000d000);
	}

	if (pll->type == PLL_MEMORY) {
=======
static u32 read_clk(struct drm_device *, int, bool);
static u32 read_pll(struct drm_device *, int, u32);

static u32
read_vco(struct drm_device *dev, int clk)
{
	u32 sctl = nv_rd32(dev, 0x4120 + (clk * 4));
	if ((sctl & 0x00000030) != 0x00000030)
		return read_pll(dev, 0x41, 0x00e820);
	return read_pll(dev, 0x42, 0x00e8a0);
}

static u32
read_clk(struct drm_device *dev, int clk, bool ignore_en)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	u32 sctl, sdiv, sclk;

	/* refclk for the 0xe8xx plls is a fixed frequency */
	if (clk >= 0x40) {
		if (dev_priv->chipset == 0xaf) {
			/* no joke.. seriously.. sigh.. */
			return nv_rd32(dev, 0x00471c) * 1000;
		}

		return dev_priv->crystal;
	}

	sctl = nv_rd32(dev, 0x4120 + (clk * 4));
	if (!ignore_en && !(sctl & 0x00000100))
		return 0;

	switch (sctl & 0x00003000) {
	case 0x00000000:
		return dev_priv->crystal;
	case 0x00002000:
		if (sctl & 0x00000040)
			return 108000;
		return 100000;
	case 0x00003000:
		sclk = read_vco(dev, clk);
		sdiv = ((sctl & 0x003f0000) >> 16) + 2;
		return (sclk * 2) / sdiv;
	default:
		return 0;
	}
}

static u32
read_pll(struct drm_device *dev, int clk, u32 pll)
{
	u32 ctrl = nv_rd32(dev, pll + 0);
	u32 sclk = 0, P = 1, N = 1, M = 1;

	if (!(ctrl & 0x00000008)) {
		if (ctrl & 0x00000001) {
			u32 coef = nv_rd32(dev, pll + 4);
			M = (coef & 0x000000ff) >> 0;
			N = (coef & 0x0000ff00) >> 8;
			P = (coef & 0x003f0000) >> 16;

			/* no post-divider on these.. */
			if ((pll & 0x00ff00) == 0x00e800)
				P = 1;

			sclk = read_clk(dev, 0x00 + clk, false);
		}
	} else {
		sclk = read_clk(dev, 0x10 + clk, false);
	}

	return sclk * N / (M * P);
}

struct creg {
	u32 clk;
	u32 pll;
};

static int
calc_clk(struct drm_device *dev, int clk, u32 pll, u32 khz, struct creg *reg)
{
	struct pll_lims limits;
	u32 oclk, sclk, sdiv;
	int P, N, M, diff;
	int ret;

	reg->pll = 0;
	reg->clk = 0;
	if (!khz) {
		NV_DEBUG(dev, "no clock for 0x%04x/0x%02x\n", pll, clk);
		return 0;
	}

	switch (khz) {
	case 27000:
		reg->clk = 0x00000100;
		return khz;
	case 100000:
		reg->clk = 0x00002100;
		return khz;
	case 108000:
		reg->clk = 0x00002140;
		return khz;
	default:
		sclk = read_vco(dev, clk);
		sdiv = min((sclk * 2) / (khz - 2999), (u32)65);
		/* if the clock has a PLL attached, and we can get a within
		 * [-2, 3) MHz of a divider, we'll disable the PLL and use
		 * the divider instead.
		 *
		 * divider can go as low as 2, limited here because NVIDIA
		 * and the VBIOS on my NVA8 seem to prefer using the PLL
		 * for 810MHz - is there a good reason?
		 */
		if (sdiv > 4) {
			oclk = (sclk * 2) / sdiv;
			diff = khz - oclk;
			if (!pll || (diff >= -2000 && diff < 3000)) {
				reg->clk = (((sdiv - 2) << 16) | 0x00003100);
				return oclk;
			}
		}

		if (!pll) {
			NV_ERROR(dev, "bad freq %02x: %d %d\n", clk, khz, sclk);
			return -ERANGE;
		}

		break;
	}

	ret = get_pll_limits(dev, pll, &limits);
	if (ret)
		return ret;

	limits.refclk = read_clk(dev, clk - 0x10, true);
	if (!limits.refclk)
		return -EINVAL;

	ret = nva3_calc_pll(dev, &limits, khz, &N, NULL, &M, &P);
	if (ret >= 0) {
		reg->clk = nv_rd32(dev, 0x4120 + (clk * 4));
		reg->pll = (P << 16) | (N << 8) | M;
	}
	return ret;
}

static void
prog_pll(struct drm_device *dev, int clk, u32 pll, struct creg *reg)
{
	const u32 src0 = 0x004120 + (clk * 4);
	const u32 src1 = 0x004160 + (clk * 4);
	const u32 ctrl = pll + 0;
	const u32 coef = pll + 4;
	u32 cntl;

	if (!reg->clk && !reg->pll) {
		NV_DEBUG(dev, "no clock for %02x\n", clk);
		return;
	}

	cntl = nv_rd32(dev, ctrl) & 0xfffffff2;
	if (reg->pll) {
		nv_mask(dev, src0, 0x00000101, 0x00000101);
		nv_wr32(dev, coef, reg->pll);
		nv_wr32(dev, ctrl, cntl | 0x00000015);
		nv_mask(dev, src1, 0x00000100, 0x00000000);
		nv_mask(dev, src1, 0x00000001, 0x00000000);
	} else {
		nv_mask(dev, src1, 0x003f3141, 0x00000101 | reg->clk);
		nv_wr32(dev, ctrl, cntl | 0x0000001d);
		nv_mask(dev, ctrl, 0x00000001, 0x00000000);
		nv_mask(dev, src0, 0x00000100, 0x00000000);
		nv_mask(dev, src0, 0x00000001, 0x00000000);
	}
}

static void
prog_clk(struct drm_device *dev, int clk, struct creg *reg)
{
	if (!reg->clk) {
		NV_DEBUG(dev, "no clock for %02x\n", clk);
		return;
	}

	nv_mask(dev, 0x004120 + (clk * 4), 0x003f3141, 0x00000101 | reg->clk);
}

int
nva3_pm_clocks_get(struct drm_device *dev, struct nouveau_pm_level *perflvl)
{
	perflvl->core   = read_pll(dev, 0x00, 0x4200);
	perflvl->shader = read_pll(dev, 0x01, 0x4220);
	perflvl->memory = read_pll(dev, 0x02, 0x4000);
	perflvl->unka0  = read_clk(dev, 0x20, false);
	perflvl->vdec   = read_clk(dev, 0x21, false);
	perflvl->daemon = read_clk(dev, 0x25, false);
	perflvl->copy   = perflvl->core;
	return 0;
}

struct nva3_pm_state {
	struct creg nclk;
	struct creg sclk;
	struct creg mclk;
	struct creg vdec;
	struct creg unka0;
};

void *
nva3_pm_clocks_pre(struct drm_device *dev, struct nouveau_pm_level *perflvl)
{
	struct nva3_pm_state *info;
	int ret;

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		return ERR_PTR(-ENOMEM);

	ret = calc_clk(dev, 0x10, 0x4200, perflvl->core, &info->nclk);
	if (ret < 0)
		goto out;

	ret = calc_clk(dev, 0x11, 0x4220, perflvl->shader, &info->sclk);
	if (ret < 0)
		goto out;

	ret = calc_clk(dev, 0x12, 0x4000, perflvl->memory, &info->mclk);
	if (ret < 0)
		goto out;

	ret = calc_clk(dev, 0x20, 0x0000, perflvl->unka0, &info->unka0);
	if (ret < 0)
		goto out;

	ret = calc_clk(dev, 0x21, 0x0000, perflvl->vdec, &info->vdec);
	if (ret < 0)
		goto out;

out:
	if (ret < 0) {
		kfree(info);
		info = ERR_PTR(ret);
	}
	return info;
}

static bool
nva3_pm_grcp_idle(void *data)
{
	struct drm_device *dev = data;

	if (!(nv_rd32(dev, 0x400304) & 0x00000001))
		return true;
	if (nv_rd32(dev, 0x400308) == 0x0050001c)
		return true;
	return false;
}

int
nva3_pm_clocks_set(struct drm_device *dev, void *pre_state)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nva3_pm_state *info = pre_state;
	unsigned long flags;
	int ret = -EAGAIN;

	/* prevent any new grctx switches from starting */
	spin_lock_irqsave(&dev_priv->context_switch_lock, flags);
	nv_wr32(dev, 0x400324, 0x00000000);
	nv_wr32(dev, 0x400328, 0x0050001c); /* wait flag 0x1c */
	/* wait for any pending grctx switches to complete */
	if (!nv_wait_cb(dev, nva3_pm_grcp_idle, dev)) {
		NV_ERROR(dev, "pm: ctxprog didn't go idle\n");
		goto cleanup;
	}
	/* freeze PFIFO */
	nv_mask(dev, 0x002504, 0x00000001, 0x00000001);
	if (!nv_wait(dev, 0x002504, 0x00000010, 0x00000010)) {
		NV_ERROR(dev, "pm: fifo didn't go idle\n");
		goto cleanup;
	}

	prog_pll(dev, 0x00, 0x004200, &info->nclk);
	prog_pll(dev, 0x01, 0x004220, &info->sclk);
	prog_clk(dev, 0x20, &info->unka0);
	prog_clk(dev, 0x21, &info->vdec);

	if (info->mclk.clk || info->mclk.pll) {
		nv_wr32(dev, 0x100210, 0);
		nv_wr32(dev, 0x1002dc, 1);
		nv_wr32(dev, 0x004018, 0x00001000);
		prog_pll(dev, 0x02, 0x004000, &info->mclk);
		if (nv_rd32(dev, 0x4000) & 0x00000008)
			nv_wr32(dev, 0x004018, 0x1000d000);
		else
			nv_wr32(dev, 0x004018, 0x10005000);
>>>>>>> refs/remotes/origin/cm-10.0
		nv_wr32(dev, 0x1002dc, 0);
		nv_wr32(dev, 0x100210, 0x80000000);
	}

<<<<<<< HEAD
	kfree(pll);
}

=======
	ret = 0;

cleanup:
	/* unfreeze PFIFO */
	nv_mask(dev, 0x002504, 0x00000001, 0x00000000);
	/* restore ctxprog to normal */
	nv_wr32(dev, 0x400324, 0x00000000);
	nv_wr32(dev, 0x400328, 0x0070009c); /* set flag 0x1c */
	/* unblock it if necessary */
	if (nv_rd32(dev, 0x400308) == 0x0050001c)
		nv_mask(dev, 0x400824, 0x10000000, 0x10000000);
	spin_unlock_irqrestore(&dev_priv->context_switch_lock, flags);
	kfree(info);
	return ret;
}
>>>>>>> refs/remotes/origin/cm-10.0
