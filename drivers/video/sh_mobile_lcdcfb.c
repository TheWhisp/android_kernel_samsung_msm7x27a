/*
 * SuperH Mobile LCDC Framebuffer
 *
 * Copyright (c) 2008 Magnus Damm
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/clk.h>
#include <linux/pm_runtime.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/ioctl.h>
#include <linux/slab.h>
#include <linux/console.h>
#include <linux/backlight.h>
#include <linux/gpio.h>
#include <video/sh_mobile_lcdc.h>
#include <asm/atomic.h>

#include "sh_mobile_lcdcfb.h"
#include "sh_mobile_meram.h"
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/atomic.h>
#include <linux/backlight.h>
#include <linux/clk.h>
#include <linux/console.h>
<<<<<<< HEAD
=======
#include <linux/ctype.h>
>>>>>>> refs/remotes/origin/master
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/videodev2.h>
#include <linux/vmalloc.h>

#include <video/sh_mobile_lcdc.h>
#include <video/sh_mobile_meram.h>

#include "sh_mobile_lcdcfb.h"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

/* ----------------------------------------------------------------------------
 * Overlay register definitions
 */

#define LDBCR			0xb00
#define LDBCR_UPC(n)		(1 << ((n) + 16))
#define LDBCR_UPF(n)		(1 << ((n) + 8))
#define LDBCR_UPD(n)		(1 << ((n) + 0))
#define LDBnBSIFR(n)		(0xb20 + (n) * 0x20 + 0x00)
#define LDBBSIFR_EN		(1 << 31)
#define LDBBSIFR_VS		(1 << 29)
#define LDBBSIFR_BRSEL		(1 << 28)
#define LDBBSIFR_MX		(1 << 27)
#define LDBBSIFR_MY		(1 << 26)
#define LDBBSIFR_CV3		(3 << 24)
#define LDBBSIFR_CV2		(2 << 24)
#define LDBBSIFR_CV1		(1 << 24)
#define LDBBSIFR_CV0		(0 << 24)
#define LDBBSIFR_CV_MASK	(3 << 24)
#define LDBBSIFR_LAY_MASK	(0xff << 16)
#define LDBBSIFR_LAY_SHIFT	16
#define LDBBSIFR_ROP3_MASK	(0xff << 16)
#define LDBBSIFR_ROP3_SHIFT	16
#define LDBBSIFR_AL_PL8		(3 << 14)
#define LDBBSIFR_AL_PL1		(2 << 14)
#define LDBBSIFR_AL_PK		(1 << 14)
#define LDBBSIFR_AL_1		(0 << 14)
#define LDBBSIFR_AL_MASK	(3 << 14)
#define LDBBSIFR_SWPL		(1 << 10)
#define LDBBSIFR_SWPW		(1 << 9)
#define LDBBSIFR_SWPB		(1 << 8)
#define LDBBSIFR_RY		(1 << 7)
#define LDBBSIFR_CHRR_420	(2 << 0)
#define LDBBSIFR_CHRR_422	(1 << 0)
#define LDBBSIFR_CHRR_444	(0 << 0)
#define LDBBSIFR_RPKF_ARGB32	(0x00 << 0)
#define LDBBSIFR_RPKF_RGB16	(0x03 << 0)
#define LDBBSIFR_RPKF_RGB24	(0x0b << 0)
#define LDBBSIFR_RPKF_MASK	(0x1f << 0)
#define LDBnBSSZR(n)		(0xb20 + (n) * 0x20 + 0x04)
#define LDBBSSZR_BVSS_MASK	(0xfff << 16)
#define LDBBSSZR_BVSS_SHIFT	16
#define LDBBSSZR_BHSS_MASK	(0xfff << 0)
#define LDBBSSZR_BHSS_SHIFT	0
#define LDBnBLOCR(n)		(0xb20 + (n) * 0x20 + 0x08)
#define LDBBLOCR_CVLC_MASK	(0xfff << 16)
#define LDBBLOCR_CVLC_SHIFT	16
#define LDBBLOCR_CHLC_MASK	(0xfff << 0)
#define LDBBLOCR_CHLC_SHIFT	0
#define LDBnBSMWR(n)		(0xb20 + (n) * 0x20 + 0x0c)
#define LDBBSMWR_BSMWA_MASK	(0xffff << 16)
#define LDBBSMWR_BSMWA_SHIFT	16
#define LDBBSMWR_BSMW_MASK	(0xffff << 0)
#define LDBBSMWR_BSMW_SHIFT	0
#define LDBnBSAYR(n)		(0xb20 + (n) * 0x20 + 0x10)
#define LDBBSAYR_FG1A_MASK	(0xff << 24)
#define LDBBSAYR_FG1A_SHIFT	24
#define LDBBSAYR_FG1R_MASK	(0xff << 16)
#define LDBBSAYR_FG1R_SHIFT	16
#define LDBBSAYR_FG1G_MASK	(0xff << 8)
#define LDBBSAYR_FG1G_SHIFT	8
#define LDBBSAYR_FG1B_MASK	(0xff << 0)
#define LDBBSAYR_FG1B_SHIFT	0
#define LDBnBSACR(n)		(0xb20 + (n) * 0x20 + 0x14)
#define LDBBSACR_FG2A_MASK	(0xff << 24)
#define LDBBSACR_FG2A_SHIFT	24
#define LDBBSACR_FG2R_MASK	(0xff << 16)
#define LDBBSACR_FG2R_SHIFT	16
#define LDBBSACR_FG2G_MASK	(0xff << 8)
#define LDBBSACR_FG2G_SHIFT	8
#define LDBBSACR_FG2B_MASK	(0xff << 0)
#define LDBBSACR_FG2B_SHIFT	0
#define LDBnBSAAR(n)		(0xb20 + (n) * 0x20 + 0x18)
#define LDBBSAAR_AP_MASK	(0xff << 24)
#define LDBBSAAR_AP_SHIFT	24
#define LDBBSAAR_R_MASK		(0xff << 16)
#define LDBBSAAR_R_SHIFT	16
#define LDBBSAAR_GY_MASK	(0xff << 8)
#define LDBBSAAR_GY_SHIFT	8
#define LDBBSAAR_B_MASK		(0xff << 0)
#define LDBBSAAR_B_SHIFT	0
#define LDBnBPPCR(n)		(0xb20 + (n) * 0x20 + 0x1c)
#define LDBBPPCR_AP_MASK	(0xff << 24)
#define LDBBPPCR_AP_SHIFT	24
#define LDBBPPCR_R_MASK		(0xff << 16)
#define LDBBPPCR_R_SHIFT	16
#define LDBBPPCR_GY_MASK	(0xff << 8)
#define LDBBPPCR_GY_SHIFT	8
#define LDBBPPCR_B_MASK		(0xff << 0)
#define LDBBPPCR_B_SHIFT	0
#define LDBnBBGCL(n)		(0xb10 + (n) * 0x04)
#define LDBBBGCL_BGA_MASK	(0xff << 24)
#define LDBBBGCL_BGA_SHIFT	24
#define LDBBBGCL_BGR_MASK	(0xff << 16)
#define LDBBBGCL_BGR_SHIFT	16
#define LDBBBGCL_BGG_MASK	(0xff << 8)
#define LDBBBGCL_BGG_SHIFT	8
#define LDBBBGCL_BGB_MASK	(0xff << 0)
#define LDBBBGCL_BGB_SHIFT	0
>>>>>>> refs/remotes/origin/master

#define SIDE_B_OFFSET 0x1000
#define MIRROR_OFFSET 0x2000

<<<<<<< HEAD
<<<<<<< HEAD
/* shared registers */
#define _LDDCKR 0x410
#define _LDDCKSTPR 0x414
#define _LDINTR 0x468
#define _LDSR 0x46c
#define _LDCNT1R 0x470
#define _LDCNT2R 0x474
#define _LDRCNTR 0x478
#define _LDDDSR 0x47c
#define _LDDWD0R 0x800
#define _LDDRDR 0x840
#define _LDDWAR 0x900
#define _LDDRAR 0x904

/* shared registers and their order for context save/restore */
static int lcdc_shared_regs[] = {
	_LDDCKR,
	_LDDCKSTPR,
	_LDINTR,
	_LDDDSR,
	_LDCNT1R,
	_LDCNT2R,
};
#define NR_SHARED_REGS ARRAY_SIZE(lcdc_shared_regs)

#define MAX_XRES 1920
#define MAX_YRES 1080

=======
#define MAX_XRES 1920
#define MAX_YRES 1080
=======
#define MAX_XRES 1920
#define MAX_YRES 1080

enum sh_mobile_lcdc_overlay_mode {
	LCDC_OVERLAY_BLEND,
	LCDC_OVERLAY_ROP3,
};

/*
 * struct sh_mobile_lcdc_overlay - LCDC display overlay
 *
 * @channel: LCDC channel this overlay belongs to
 * @cfg: Overlay configuration
 * @info: Frame buffer device
 * @index: Overlay index (0-3)
 * @base: Overlay registers base address
 * @enabled: True if the overlay is enabled
 * @mode: Overlay blending mode (alpha blend or ROP3)
 * @alpha: Global alpha blending value (0-255, for alpha blending mode)
 * @rop3: Raster operation (for ROP3 mode)
 * @fb_mem: Frame buffer virtual memory address
 * @fb_size: Frame buffer size in bytes
 * @dma_handle: Frame buffer DMA address
 * @base_addr_y: Overlay base address (RGB or luma component)
 * @base_addr_c: Overlay base address (chroma component)
 * @pan_y_offset: Panning linear offset in bytes (luma component)
 * @format: Current pixelf format
 * @xres: Horizontal visible resolution
 * @xres_virtual: Horizontal total resolution
 * @yres: Vertical visible resolution
 * @yres_virtual: Vertical total resolution
 * @pitch: Overlay line pitch
 * @pos_x: Horizontal overlay position
 * @pos_y: Vertical overlay position
 */
struct sh_mobile_lcdc_overlay {
	struct sh_mobile_lcdc_chan *channel;

	const struct sh_mobile_lcdc_overlay_cfg *cfg;
	struct fb_info *info;

	unsigned int index;
	unsigned long base;

	bool enabled;
	enum sh_mobile_lcdc_overlay_mode mode;
	unsigned int alpha;
	unsigned int rop3;

	void *fb_mem;
	unsigned long fb_size;

	dma_addr_t dma_handle;
	unsigned long base_addr_y;
	unsigned long base_addr_c;
	unsigned long pan_y_offset;

	const struct sh_mobile_lcdc_format_info *format;
	unsigned int xres;
	unsigned int xres_virtual;
	unsigned int yres;
	unsigned int yres_virtual;
	unsigned int pitch;
	int pos_x;
	int pos_y;
};
>>>>>>> refs/remotes/origin/master

struct sh_mobile_lcdc_priv {
	void __iomem *base;
	int irq;
	atomic_t hw_usecnt;
	struct device *dev;
	struct clk *dot_clk;
	unsigned long lddckr;
<<<<<<< HEAD
	struct sh_mobile_lcdc_chan ch[2];
=======

	struct sh_mobile_lcdc_chan ch[2];
	struct sh_mobile_lcdc_overlay overlays[4];

>>>>>>> refs/remotes/origin/master
	struct notifier_block notifier;
	int started;
	int forced_fourcc; /* 2 channel LCDC must share fourcc setting */
	struct sh_mobile_meram_info *meram_dev;
};

/* -----------------------------------------------------------------------------
 * Registers access
 */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static unsigned long lcdc_offs_mainlcd[NR_CH_REGS] = {
	[LDDCKPAT1R] = 0x400,
	[LDDCKPAT2R] = 0x404,
	[LDMT1R] = 0x418,
	[LDMT2R] = 0x41c,
	[LDMT3R] = 0x420,
	[LDDFR] = 0x424,
	[LDSM1R] = 0x428,
	[LDSM2R] = 0x42c,
	[LDSA1R] = 0x430,
	[LDSA2R] = 0x434,
	[LDMLSR] = 0x438,
	[LDHCNR] = 0x448,
	[LDHSYNR] = 0x44c,
	[LDVLNR] = 0x450,
	[LDVSYNR] = 0x454,
	[LDPMR] = 0x460,
	[LDHAJR] = 0x4a0,
};

static unsigned long lcdc_offs_sublcd[NR_CH_REGS] = {
	[LDDCKPAT1R] = 0x408,
	[LDDCKPAT2R] = 0x40c,
	[LDMT1R] = 0x600,
	[LDMT2R] = 0x604,
	[LDMT3R] = 0x608,
	[LDDFR] = 0x60c,
	[LDSM1R] = 0x610,
	[LDSM2R] = 0x614,
	[LDSA1R] = 0x618,
	[LDMLSR] = 0x620,
	[LDHCNR] = 0x624,
	[LDHSYNR] = 0x628,
	[LDVLNR] = 0x62c,
	[LDVSYNR] = 0x630,
	[LDPMR] = 0x63c,
};

<<<<<<< HEAD
<<<<<<< HEAD
#define START_LCDC	0x00000001
#define LCDC_RESET	0x00000100
#define DISPLAY_BEU	0x00000008
#define LCDC_ENABLE	0x00000001
#define LDINTR_FE	0x00000400
#define LDINTR_VSE	0x00000200
#define LDINTR_VEE	0x00000100
#define LDINTR_FS	0x00000004
#define LDINTR_VSS	0x00000002
#define LDINTR_VES	0x00000001
#define LDRCNTR_SRS	0x00020000
#define LDRCNTR_SRC	0x00010000
#define LDRCNTR_MRS	0x00000002
#define LDRCNTR_MRC	0x00000001
#define LDSR_MRS	0x00000100

static const struct fb_videomode default_720p = {
	.name = "HDMI 720p",
	.xres = 1280,
	.yres = 720,

	.left_margin = 220,
	.right_margin = 110,
	.hsync_len = 40,

	.upper_margin = 20,
	.lower_margin = 5,
	.vsync_len = 5,

	.pixclock = 13468,
	.refresh = 60,
	.sync = FB_SYNC_VERT_HIGH_ACT | FB_SYNC_HOR_HIGH_ACT,
};

struct sh_mobile_lcdc_priv {
	void __iomem *base;
	int irq;
	atomic_t hw_usecnt;
	struct device *dev;
	struct clk *dot_clk;
	unsigned long lddckr;
	struct sh_mobile_lcdc_chan ch[2];
	struct notifier_block notifier;
	unsigned long saved_shared_regs[NR_SHARED_REGS];
	int started;
	int forced_bpp; /* 2 channel LCDC must share bpp setting */
	struct sh_mobile_meram_info *meram_dev;
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static bool banked(int reg_nr)
{
	switch (reg_nr) {
	case LDMT1R:
	case LDMT2R:
	case LDMT3R:
	case LDDFR:
	case LDSM1R:
	case LDSA1R:
	case LDSA2R:
	case LDMLSR:
	case LDHCNR:
	case LDHSYNR:
	case LDVLNR:
	case LDVSYNR:
		return true;
	}
	return false;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static int lcdc_chan_is_sublcd(struct sh_mobile_lcdc_chan *chan)
{
	return chan->cfg->chan == LCDC_CHAN_SUBLCD;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void lcdc_write_chan(struct sh_mobile_lcdc_chan *chan,
			    int reg_nr, unsigned long data)
{
	iowrite32(data, chan->lcdc->base + chan->reg_offs[reg_nr]);
	if (banked(reg_nr))
		iowrite32(data, chan->lcdc->base + chan->reg_offs[reg_nr] +
			  SIDE_B_OFFSET);
}

static void lcdc_write_chan_mirror(struct sh_mobile_lcdc_chan *chan,
			    int reg_nr, unsigned long data)
{
	iowrite32(data, chan->lcdc->base + chan->reg_offs[reg_nr] +
		  MIRROR_OFFSET);
}

static unsigned long lcdc_read_chan(struct sh_mobile_lcdc_chan *chan,
				    int reg_nr)
{
	return ioread32(chan->lcdc->base + chan->reg_offs[reg_nr]);
}

<<<<<<< HEAD
=======
static void lcdc_write_overlay(struct sh_mobile_lcdc_overlay *ovl,
			       int reg, unsigned long data)
{
	iowrite32(data, ovl->channel->lcdc->base + reg);
	iowrite32(data, ovl->channel->lcdc->base + reg + SIDE_B_OFFSET);
}

>>>>>>> refs/remotes/origin/master
static void lcdc_write(struct sh_mobile_lcdc_priv *priv,
		       unsigned long reg_offs, unsigned long data)
{
	iowrite32(data, priv->base + reg_offs);
}

static unsigned long lcdc_read(struct sh_mobile_lcdc_priv *priv,
			       unsigned long reg_offs)
{
	return ioread32(priv->base + reg_offs);
}

static void lcdc_wait_bit(struct sh_mobile_lcdc_priv *priv,
			  unsigned long reg_offs,
			  unsigned long mask, unsigned long until)
{
	while ((lcdc_read(priv, reg_offs) & mask) != until)
		cpu_relax();
}

<<<<<<< HEAD
<<<<<<< HEAD
static int lcdc_chan_is_sublcd(struct sh_mobile_lcdc_chan *chan)
{
	return chan->cfg.chan == LCDC_CHAN_SUBLCD;
}

=======
=======
>>>>>>> refs/remotes/origin/master
/* -----------------------------------------------------------------------------
 * Clock management
 */

static void sh_mobile_lcdc_clk_on(struct sh_mobile_lcdc_priv *priv)
{
	if (atomic_inc_and_test(&priv->hw_usecnt)) {
		if (priv->dot_clk)
<<<<<<< HEAD
			clk_enable(priv->dot_clk);
=======
			clk_prepare_enable(priv->dot_clk);
>>>>>>> refs/remotes/origin/master
		pm_runtime_get_sync(priv->dev);
		if (priv->meram_dev && priv->meram_dev->pdev)
			pm_runtime_get_sync(&priv->meram_dev->pdev->dev);
	}
}

static void sh_mobile_lcdc_clk_off(struct sh_mobile_lcdc_priv *priv)
{
	if (atomic_sub_return(1, &priv->hw_usecnt) == -1) {
		if (priv->meram_dev && priv->meram_dev->pdev)
			pm_runtime_put_sync(&priv->meram_dev->pdev->dev);
		pm_runtime_put(priv->dev);
		if (priv->dot_clk)
<<<<<<< HEAD
			clk_disable(priv->dot_clk);
=======
			clk_disable_unprepare(priv->dot_clk);
>>>>>>> refs/remotes/origin/master
	}
}

static int sh_mobile_lcdc_setup_clocks(struct sh_mobile_lcdc_priv *priv,
				       int clock_source)
{
	struct clk *clk;
	char *str;

	switch (clock_source) {
	case LCDC_CLK_BUS:
		str = "bus_clk";
		priv->lddckr = LDDCKR_ICKSEL_BUS;
		break;
	case LCDC_CLK_PERIPHERAL:
		str = "peripheral_clk";
		priv->lddckr = LDDCKR_ICKSEL_MIPI;
		break;
	case LCDC_CLK_EXTERNAL:
		str = NULL;
		priv->lddckr = LDDCKR_ICKSEL_HDMI;
		break;
	default:
		return -EINVAL;
	}

	if (str == NULL)
		return 0;

	clk = clk_get(priv->dev, str);
	if (IS_ERR(clk)) {
		dev_err(priv->dev, "cannot get dot clock %s\n", str);
		return PTR_ERR(clk);
	}

	priv->dot_clk = clk;
	return 0;
}

/* -----------------------------------------------------------------------------
 * Display, panel and deferred I/O
 */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void lcdc_sys_write_index(void *handle, unsigned long data)
{
	struct sh_mobile_lcdc_chan *ch = handle;

<<<<<<< HEAD
<<<<<<< HEAD
	lcdc_write(ch->lcdc, _LDDWD0R, data | 0x10000000);
	lcdc_wait_bit(ch->lcdc, _LDSR, 2, 0);
	lcdc_write(ch->lcdc, _LDDWAR, 1 | (lcdc_chan_is_sublcd(ch) ? 2 : 0));
	lcdc_wait_bit(ch->lcdc, _LDSR, 2, 0);
=======
=======
>>>>>>> refs/remotes/origin/master
	lcdc_write(ch->lcdc, _LDDWD0R, data | LDDWDxR_WDACT);
	lcdc_wait_bit(ch->lcdc, _LDSR, LDSR_AS, 0);
	lcdc_write(ch->lcdc, _LDDWAR, LDDWAR_WA |
		   (lcdc_chan_is_sublcd(ch) ? 2 : 0));
	lcdc_wait_bit(ch->lcdc, _LDSR, LDSR_AS, 0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void lcdc_sys_write_data(void *handle, unsigned long data)
{
	struct sh_mobile_lcdc_chan *ch = handle;

<<<<<<< HEAD
<<<<<<< HEAD
	lcdc_write(ch->lcdc, _LDDWD0R, data | 0x11000000);
	lcdc_wait_bit(ch->lcdc, _LDSR, 2, 0);
	lcdc_write(ch->lcdc, _LDDWAR, 1 | (lcdc_chan_is_sublcd(ch) ? 2 : 0));
	lcdc_wait_bit(ch->lcdc, _LDSR, 2, 0);
=======
=======
>>>>>>> refs/remotes/origin/master
	lcdc_write(ch->lcdc, _LDDWD0R, data | LDDWDxR_WDACT | LDDWDxR_RSW);
	lcdc_wait_bit(ch->lcdc, _LDSR, LDSR_AS, 0);
	lcdc_write(ch->lcdc, _LDDWAR, LDDWAR_WA |
		   (lcdc_chan_is_sublcd(ch) ? 2 : 0));
	lcdc_wait_bit(ch->lcdc, _LDSR, LDSR_AS, 0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static unsigned long lcdc_sys_read_data(void *handle)
{
	struct sh_mobile_lcdc_chan *ch = handle;

<<<<<<< HEAD
<<<<<<< HEAD
	lcdc_write(ch->lcdc, _LDDRDR, 0x01000000);
	lcdc_wait_bit(ch->lcdc, _LDSR, 2, 0);
	lcdc_write(ch->lcdc, _LDDRAR, 1 | (lcdc_chan_is_sublcd(ch) ? 2 : 0));
	udelay(1);
	lcdc_wait_bit(ch->lcdc, _LDSR, 2, 0);

	return lcdc_read(ch->lcdc, _LDDRDR) & 0x3ffff;
=======
=======
>>>>>>> refs/remotes/origin/master
	lcdc_write(ch->lcdc, _LDDRDR, LDDRDR_RSR);
	lcdc_wait_bit(ch->lcdc, _LDSR, LDSR_AS, 0);
	lcdc_write(ch->lcdc, _LDDRAR, LDDRAR_RA |
		   (lcdc_chan_is_sublcd(ch) ? 2 : 0));
	udelay(1);
	lcdc_wait_bit(ch->lcdc, _LDSR, LDSR_AS, 0);

	return lcdc_read(ch->lcdc, _LDDRDR) & LDDRDR_DRD_MASK;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

struct sh_mobile_lcdc_sys_bus_ops sh_mobile_lcdc_sys_bus_ops = {
=======
}

static struct sh_mobile_lcdc_sys_bus_ops sh_mobile_lcdc_sys_bus_ops = {
>>>>>>> refs/remotes/origin/master
	lcdc_sys_write_index,
	lcdc_sys_write_data,
	lcdc_sys_read_data,
};

<<<<<<< HEAD
<<<<<<< HEAD
static void sh_mobile_lcdc_clk_on(struct sh_mobile_lcdc_priv *priv)
{
	if (atomic_inc_and_test(&priv->hw_usecnt)) {
		pm_runtime_get_sync(priv->dev);
		if (priv->dot_clk)
			clk_enable(priv->dot_clk);
	}
}

static void sh_mobile_lcdc_clk_off(struct sh_mobile_lcdc_priv *priv)
{
	if (atomic_sub_return(1, &priv->hw_usecnt) == -1) {
		if (priv->dot_clk)
			clk_disable(priv->dot_clk);
		pm_runtime_put(priv->dev);
	}
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int sh_mobile_lcdc_sginit(struct fb_info *info,
				  struct list_head *pagelist)
{
	struct sh_mobile_lcdc_chan *ch = info->par;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int nr_pages_max = info->fix.smem_len >> PAGE_SHIFT;
=======
	unsigned int nr_pages_max = ch->fb_size >> PAGE_SHIFT;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned int nr_pages_max = ch->fb_size >> PAGE_SHIFT;
>>>>>>> refs/remotes/origin/master
	struct page *page;
	int nr_pages = 0;

	sg_init_table(ch->sglist, nr_pages_max);

	list_for_each_entry(page, pagelist, lru)
		sg_set_page(&ch->sglist[nr_pages++], page, PAGE_SIZE, 0);

	return nr_pages;
}

static void sh_mobile_lcdc_deferred_io(struct fb_info *info,
				       struct list_head *pagelist)
{
	struct sh_mobile_lcdc_chan *ch = info->par;
<<<<<<< HEAD
<<<<<<< HEAD
	struct sh_mobile_lcdc_board_cfg	*bcfg = &ch->cfg.board_cfg;
=======
	const struct sh_mobile_lcdc_panel_cfg *panel = &ch->cfg->panel_cfg;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const struct sh_mobile_lcdc_panel_cfg *panel = &ch->cfg->panel_cfg;
>>>>>>> refs/remotes/origin/master

	/* enable clocks before accessing hardware */
	sh_mobile_lcdc_clk_on(ch->lcdc);

	/*
	 * It's possible to get here without anything on the pagelist via
	 * sh_mobile_lcdc_deferred_io_touch() or via a userspace fsync()
	 * invocation. In the former case, the acceleration routines are
	 * stepped in to when using the framebuffer console causing the
	 * workqueue to be scheduled without any dirty pages on the list.
	 *
	 * Despite this, a panel update is still needed given that the
	 * acceleration routines have their own methods for writing in
	 * that still need to be updated.
	 *
	 * The fsync() and empty pagelist case could be optimized for,
	 * but we don't bother, as any application exhibiting such
	 * behaviour is fundamentally broken anyways.
	 */
	if (!list_empty(pagelist)) {
		unsigned int nr_pages = sh_mobile_lcdc_sginit(info, pagelist);

		/* trigger panel update */
<<<<<<< HEAD
<<<<<<< HEAD
		dma_map_sg(info->dev, ch->sglist, nr_pages, DMA_TO_DEVICE);
		if (bcfg->start_transfer)
			bcfg->start_transfer(bcfg->board_data, ch,
					     &sh_mobile_lcdc_sys_bus_ops);
		lcdc_write_chan(ch, LDSM2R, 1);
		dma_unmap_sg(info->dev, ch->sglist, nr_pages, DMA_TO_DEVICE);
	} else {
		if (bcfg->start_transfer)
			bcfg->start_transfer(bcfg->board_data, ch,
					     &sh_mobile_lcdc_sys_bus_ops);
		lcdc_write_chan(ch, LDSM2R, 1);
=======
=======
>>>>>>> refs/remotes/origin/master
		dma_map_sg(ch->lcdc->dev, ch->sglist, nr_pages, DMA_TO_DEVICE);
		if (panel->start_transfer)
			panel->start_transfer(ch, &sh_mobile_lcdc_sys_bus_ops);
		lcdc_write_chan(ch, LDSM2R, LDSM2R_OSTRG);
		dma_unmap_sg(ch->lcdc->dev, ch->sglist, nr_pages,
			     DMA_TO_DEVICE);
	} else {
		if (panel->start_transfer)
			panel->start_transfer(ch, &sh_mobile_lcdc_sys_bus_ops);
		lcdc_write_chan(ch, LDSM2R, LDSM2R_OSTRG);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
}

static void sh_mobile_lcdc_deferred_io_touch(struct fb_info *info)
{
	struct fb_deferred_io *fbdefio = info->fbdefio;

	if (fbdefio)
		schedule_delayed_work(&info->deferred_work, fbdefio->delay);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void sh_mobile_lcdc_display_on(struct sh_mobile_lcdc_chan *ch)
{
	const struct sh_mobile_lcdc_panel_cfg *panel = &ch->cfg->panel_cfg;

	if (ch->tx_dev) {
		int ret;

		ret = ch->tx_dev->ops->display_on(ch->tx_dev);
		if (ret < 0)
			return;

		if (ret == SH_MOBILE_LCDC_DISPLAY_DISCONNECTED)
			ch->info->state = FBINFO_STATE_SUSPENDED;
	}

	/* HDMI must be enabled before LCDC configuration */
	if (panel->display_on)
		panel->display_on();
}

static void sh_mobile_lcdc_display_off(struct sh_mobile_lcdc_chan *ch)
{
	const struct sh_mobile_lcdc_panel_cfg *panel = &ch->cfg->panel_cfg;

	if (panel->display_off)
		panel->display_off();

	if (ch->tx_dev)
		ch->tx_dev->ops->display_off(ch->tx_dev);
}

static bool
sh_mobile_lcdc_must_reconfigure(struct sh_mobile_lcdc_chan *ch,
				const struct fb_videomode *new_mode)
{
	dev_dbg(ch->info->dev, "Old %ux%u, new %ux%u\n",
		ch->display.mode.xres, ch->display.mode.yres,
		new_mode->xres, new_mode->yres);

	/* It can be a different monitor with an equal video-mode */
	if (fb_mode_is_equal(&ch->display.mode, new_mode))
		return false;

	dev_dbg(ch->info->dev, "Switching %u -> %u lines\n",
		ch->display.mode.yres, new_mode->yres);
	ch->display.mode = *new_mode;

	return true;
}

<<<<<<< HEAD
static int sh_mobile_check_var(struct fb_var_screeninfo *var,
			       struct fb_info *info);
=======
static int sh_mobile_lcdc_check_var(struct fb_var_screeninfo *var,
				    struct fb_info *info);
>>>>>>> refs/remotes/origin/master

static int sh_mobile_lcdc_display_notify(struct sh_mobile_lcdc_chan *ch,
					 enum sh_mobile_lcdc_entity_event event,
					 const struct fb_videomode *mode,
					 const struct fb_monspecs *monspec)
{
	struct fb_info *info = ch->info;
	struct fb_var_screeninfo var;
	int ret = 0;

	switch (event) {
	case SH_MOBILE_LCDC_EVENT_DISPLAY_CONNECT:
		/* HDMI plug in */
<<<<<<< HEAD
		if (lock_fb_info(info)) {
			console_lock();
=======
		console_lock();
		if (lock_fb_info(info)) {

>>>>>>> refs/remotes/origin/master

			ch->display.width = monspec->max_x * 10;
			ch->display.height = monspec->max_y * 10;

			if (!sh_mobile_lcdc_must_reconfigure(ch, mode) &&
			    info->state == FBINFO_STATE_RUNNING) {
				/* First activation with the default monitor.
				 * Just turn on, if we run a resume here, the
				 * logo disappears.
				 */
<<<<<<< HEAD
				info->var.width = monspec->max_x * 10;
				info->var.height = monspec->max_y * 10;
=======
				info->var.width = ch->display.width;
				info->var.height = ch->display.height;
>>>>>>> refs/remotes/origin/master
				sh_mobile_lcdc_display_on(ch);
			} else {
				/* New monitor or have to wake up */
				fb_set_suspend(info, 0);
			}

<<<<<<< HEAD
			console_unlock();
			unlock_fb_info(info);
		}
=======

			unlock_fb_info(info);
		}
		console_unlock();
>>>>>>> refs/remotes/origin/master
		break;

	case SH_MOBILE_LCDC_EVENT_DISPLAY_DISCONNECT:
		/* HDMI disconnect */
<<<<<<< HEAD
		if (lock_fb_info(info)) {
			console_lock();
			fb_set_suspend(info, 1);
			console_unlock();
			unlock_fb_info(info);
		}
=======
		console_lock();
		if (lock_fb_info(info)) {
			fb_set_suspend(info, 1);
			unlock_fb_info(info);
		}
		console_unlock();
>>>>>>> refs/remotes/origin/master
		break;

	case SH_MOBILE_LCDC_EVENT_DISPLAY_MODE:
		/* Validate a proposed new mode */
		fb_videomode_to_var(&var, mode);
		var.bits_per_pixel = info->var.bits_per_pixel;
		var.grayscale = info->var.grayscale;
<<<<<<< HEAD
		ret = sh_mobile_check_var(&var, info);
=======
		ret = sh_mobile_lcdc_check_var(&var, info);
>>>>>>> refs/remotes/origin/master
		break;
	}

	return ret;
}

/* -----------------------------------------------------------------------------
 * Format helpers
 */

struct sh_mobile_lcdc_format_info {
	u32 fourcc;
	unsigned int bpp;
	bool yuv;
	u32 lddfr;
};

static const struct sh_mobile_lcdc_format_info sh_mobile_format_infos[] = {
	{
		.fourcc = V4L2_PIX_FMT_RGB565,
		.bpp = 16,
		.yuv = false,
		.lddfr = LDDFR_PKF_RGB16,
	}, {
		.fourcc = V4L2_PIX_FMT_BGR24,
		.bpp = 24,
		.yuv = false,
		.lddfr = LDDFR_PKF_RGB24,
	}, {
		.fourcc = V4L2_PIX_FMT_BGR32,
		.bpp = 32,
		.yuv = false,
		.lddfr = LDDFR_PKF_ARGB32,
	}, {
		.fourcc = V4L2_PIX_FMT_NV12,
		.bpp = 12,
		.yuv = true,
		.lddfr = LDDFR_CC | LDDFR_YF_420,
	}, {
		.fourcc = V4L2_PIX_FMT_NV21,
		.bpp = 12,
		.yuv = true,
		.lddfr = LDDFR_CC | LDDFR_YF_420,
	}, {
		.fourcc = V4L2_PIX_FMT_NV16,
		.bpp = 16,
		.yuv = true,
		.lddfr = LDDFR_CC | LDDFR_YF_422,
	}, {
		.fourcc = V4L2_PIX_FMT_NV61,
		.bpp = 16,
		.yuv = true,
		.lddfr = LDDFR_CC | LDDFR_YF_422,
	}, {
		.fourcc = V4L2_PIX_FMT_NV24,
		.bpp = 24,
		.yuv = true,
		.lddfr = LDDFR_CC | LDDFR_YF_444,
	}, {
		.fourcc = V4L2_PIX_FMT_NV42,
		.bpp = 24,
		.yuv = true,
		.lddfr = LDDFR_CC | LDDFR_YF_444,
	},
};

static const struct sh_mobile_lcdc_format_info *
sh_mobile_format_info(u32 fourcc)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(sh_mobile_format_infos); ++i) {
		if (sh_mobile_format_infos[i].fourcc == fourcc)
			return &sh_mobile_format_infos[i];
	}

	return NULL;
}

static int sh_mobile_format_fourcc(const struct fb_var_screeninfo *var)
{
	if (var->grayscale > 1)
		return var->grayscale;

	switch (var->bits_per_pixel) {
	case 16:
		return V4L2_PIX_FMT_RGB565;
	case 24:
		return V4L2_PIX_FMT_BGR24;
	case 32:
		return V4L2_PIX_FMT_BGR32;
	default:
		return 0;
	}
}

static int sh_mobile_format_is_fourcc(const struct fb_var_screeninfo *var)
{
	return var->grayscale > 1;
}

/* -----------------------------------------------------------------------------
 * Start, stop and IRQ
 */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static irqreturn_t sh_mobile_lcdc_irq(int irq, void *data)
{
	struct sh_mobile_lcdc_priv *priv = data;
	struct sh_mobile_lcdc_chan *ch;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long tmp;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	unsigned long ldintr;
	int is_sub;
	int k;

<<<<<<< HEAD
<<<<<<< HEAD
	/* acknowledge interrupt */
	ldintr = tmp = lcdc_read(priv, _LDINTR);
	/*
	 * disable further VSYNC End IRQs, preserve all other enabled IRQs,
	 * write 0 to bits 0-6 to ack all triggered IRQs.
	 */
	tmp &= 0xffffff00 & ~LDINTR_VEE;
	lcdc_write(priv, _LDINTR, tmp);

	/* figure out if this interrupt is for main or sub lcd */
	is_sub = (lcdc_read(priv, _LDSR) & (1 << 10)) ? 1 : 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	/* Acknowledge interrupts and disable further VSYNC End IRQs. */
	ldintr = lcdc_read(priv, _LDINTR);
	lcdc_write(priv, _LDINTR, (ldintr ^ LDINTR_STATUS_MASK) & ~LDINTR_VEE);

	/* figure out if this interrupt is for main or sub lcd */
	is_sub = (lcdc_read(priv, _LDSR) & LDSR_MSS) ? 1 : 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* wake up channel and disable clocks */
	for (k = 0; k < ARRAY_SIZE(priv->ch); k++) {
		ch = &priv->ch[k];

		if (!ch->enabled)
			continue;

<<<<<<< HEAD
<<<<<<< HEAD
		/* Frame Start */
=======
		/* Frame End */
>>>>>>> refs/remotes/origin/cm-10.0
=======
		/* Frame End */
>>>>>>> refs/remotes/origin/master
		if (ldintr & LDINTR_FS) {
			if (is_sub == lcdc_chan_is_sublcd(ch)) {
				ch->frame_end = 1;
				wake_up(&ch->frame_end_wait);

				sh_mobile_lcdc_clk_off(priv);
			}
		}

		/* VSYNC End */
		if (ldintr & LDINTR_VES)
			complete(&ch->vsync_completion);
	}

	return IRQ_HANDLED;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static int sh_mobile_wait_for_vsync(struct sh_mobile_lcdc_chan *ch)
=======
static int sh_mobile_lcdc_wait_for_vsync(struct sh_mobile_lcdc_chan *ch)
>>>>>>> refs/remotes/origin/master
{
	unsigned long ldintr;
	int ret;

	/* Enable VSync End interrupt and be careful not to acknowledge any
	 * pending interrupt.
	 */
	ldintr = lcdc_read(ch->lcdc, _LDINTR);
	ldintr |= LDINTR_VEE | LDINTR_STATUS_MASK;
	lcdc_write(ch->lcdc, _LDINTR, ldintr);

	ret = wait_for_completion_interruptible_timeout(&ch->vsync_completion,
							msecs_to_jiffies(100));
	if (!ret)
		return -ETIMEDOUT;

	return 0;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void sh_mobile_lcdc_start_stop(struct sh_mobile_lcdc_priv *priv,
				      int start)
{
	unsigned long tmp = lcdc_read(priv, _LDCNT2R);
	int k;

	/* start or stop the lcdc */
	if (start)
<<<<<<< HEAD
<<<<<<< HEAD
		lcdc_write(priv, _LDCNT2R, tmp | START_LCDC);
	else
		lcdc_write(priv, _LDCNT2R, tmp & ~START_LCDC);
=======
		lcdc_write(priv, _LDCNT2R, tmp | LDCNT2R_DO);
	else
		lcdc_write(priv, _LDCNT2R, tmp & ~LDCNT2R_DO);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		lcdc_write(priv, _LDCNT2R, tmp | LDCNT2R_DO);
	else
		lcdc_write(priv, _LDCNT2R, tmp & ~LDCNT2R_DO);
>>>>>>> refs/remotes/origin/master

	/* wait until power is applied/stopped on all channels */
	for (k = 0; k < ARRAY_SIZE(priv->ch); k++)
		if (lcdc_read(priv, _LDCNT2R) & priv->ch[k].enabled)
			while (1) {
<<<<<<< HEAD
<<<<<<< HEAD
				tmp = lcdc_read_chan(&priv->ch[k], LDPMR) & 3;
				if (start && tmp == 3)
=======
				tmp = lcdc_read_chan(&priv->ch[k], LDPMR)
				    & LDPMR_LPS;
				if (start && tmp == LDPMR_LPS)
>>>>>>> refs/remotes/origin/cm-10.0
=======
				tmp = lcdc_read_chan(&priv->ch[k], LDPMR)
				    & LDPMR_LPS;
				if (start && tmp == LDPMR_LPS)
>>>>>>> refs/remotes/origin/master
					break;
				if (!start && tmp == 0)
					break;
				cpu_relax();
			}

	if (!start)
		lcdc_write(priv, _LDDCKSTPR, 1); /* stop dotclock */
}

static void sh_mobile_lcdc_geometry(struct sh_mobile_lcdc_chan *ch)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct fb_var_screeninfo *var = &ch->info->var, *display_var = &ch->display_var;
=======
	const struct fb_var_screeninfo *var = &ch->info->var;
	const struct fb_videomode *mode = &ch->display.mode;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const struct fb_var_screeninfo *var = &ch->info->var;
	const struct fb_videomode *mode = &ch->display.mode;
>>>>>>> refs/remotes/origin/master
	unsigned long h_total, hsync_pos, display_h_total;
	u32 tmp;

	tmp = ch->ldmt1r_value;
<<<<<<< HEAD
<<<<<<< HEAD
	tmp |= (var->sync & FB_SYNC_VERT_HIGH_ACT) ? 0 : 1 << 28;
	tmp |= (var->sync & FB_SYNC_HOR_HIGH_ACT) ? 0 : 1 << 27;
	tmp |= (ch->cfg.flags & LCDC_FLAGS_DWPOL) ? 1 << 26 : 0;
	tmp |= (ch->cfg.flags & LCDC_FLAGS_DIPOL) ? 1 << 25 : 0;
	tmp |= (ch->cfg.flags & LCDC_FLAGS_DAPOL) ? 1 << 24 : 0;
	tmp |= (ch->cfg.flags & LCDC_FLAGS_HSCNT) ? 1 << 17 : 0;
	tmp |= (ch->cfg.flags & LCDC_FLAGS_DWCNT) ? 1 << 16 : 0;
	lcdc_write_chan(ch, LDMT1R, tmp);

	/* setup SYS bus */
	lcdc_write_chan(ch, LDMT2R, ch->cfg.sys_bus_cfg.ldmt2r);
	lcdc_write_chan(ch, LDMT3R, ch->cfg.sys_bus_cfg.ldmt3r);

	/* horizontal configuration */
	h_total = display_var->xres + display_var->hsync_len +
		display_var->left_margin + display_var->right_margin;
	tmp = h_total / 8; /* HTCN */
	tmp |= (min(display_var->xres, var->xres) / 8) << 16; /* HDCN */
	lcdc_write_chan(ch, LDHCNR, tmp);

	hsync_pos = display_var->xres + display_var->right_margin;
	tmp = hsync_pos / 8; /* HSYNP */
	tmp |= (display_var->hsync_len / 8) << 16; /* HSYNW */
	lcdc_write_chan(ch, LDHSYNR, tmp);

	/* vertical configuration */
	tmp = display_var->yres + display_var->vsync_len +
		display_var->upper_margin + display_var->lower_margin; /* VTLN */
	tmp |= min(display_var->yres, var->yres) << 16; /* VDLN */
	lcdc_write_chan(ch, LDVLNR, tmp);

	tmp = display_var->yres + display_var->lower_margin; /* VSYNP */
	tmp |= display_var->vsync_len << 16; /* VSYNW */
	lcdc_write_chan(ch, LDVSYNR, tmp);

	/* Adjust horizontal synchronisation for HDMI */
	display_h_total = display_var->xres + display_var->hsync_len +
		display_var->left_margin + display_var->right_margin;
	tmp = ((display_var->xres & 7) << 24) |
		((display_h_total & 7) << 16) |
		((display_var->hsync_len & 7) << 8) |
		hsync_pos;
	lcdc_write_chan(ch, LDHAJR, tmp);
}

static int sh_mobile_lcdc_start(struct sh_mobile_lcdc_priv *priv)
{
	struct sh_mobile_lcdc_chan *ch;
	struct sh_mobile_lcdc_board_cfg	*board_cfg;
	unsigned long tmp;
	int bpp = 0;
	unsigned long ldddsr;
	int k, m, ret;

	/* enable clocks before accessing the hardware */
	for (k = 0; k < ARRAY_SIZE(priv->ch); k++) {
		if (priv->ch[k].enabled) {
			sh_mobile_lcdc_clk_on(priv);
			if (!bpp)
				bpp = priv->ch[k].info->var.bits_per_pixel;
		}
	}

	/* reset */
	lcdc_write(priv, _LDCNT2R, lcdc_read(priv, _LDCNT2R) | LCDC_RESET);
	lcdc_wait_bit(priv, _LDCNT2R, LCDC_RESET, 0);

	/* enable LCDC channels */
	tmp = lcdc_read(priv, _LDCNT2R);
	tmp |= priv->ch[0].enabled;
	tmp |= priv->ch[1].enabled;
	lcdc_write(priv, _LDCNT2R, tmp);

	/* read data from external memory, avoid using the BEU for now */
	lcdc_write(priv, _LDCNT2R, lcdc_read(priv, _LDCNT2R) & ~DISPLAY_BEU);

	/* stop the lcdc first */
	sh_mobile_lcdc_start_stop(priv, 0);

	/* configure clocks */
	tmp = priv->lddckr;
	for (k = 0; k < ARRAY_SIZE(priv->ch); k++) {
		ch = &priv->ch[k];

		if (!priv->ch[k].enabled)
			continue;

		m = ch->cfg.clock_divider;
		if (!m)
			continue;

		if (m == 1)
			m = 1 << 6;
		tmp |= m << (lcdc_chan_is_sublcd(ch) ? 8 : 0);

		/* FIXME: sh7724 can only use 42, 48, 54 and 60 for the divider denominator */
		lcdc_write_chan(ch, LDDCKPAT1R, 0);
		lcdc_write_chan(ch, LDDCKPAT2R, (1 << (m/2)) - 1);
	}

	lcdc_write(priv, _LDDCKR, tmp);

	/* start dotclock again */
	lcdc_write(priv, _LDDCKSTPR, 0);
	lcdc_wait_bit(priv, _LDDCKSTPR, ~0, 0);

	/* interrupts are disabled to begin with */
	lcdc_write(priv, _LDINTR, 0);

	for (k = 0; k < ARRAY_SIZE(priv->ch); k++) {
		ch = &priv->ch[k];

=======
=======
>>>>>>> refs/remotes/origin/master
	tmp |= (var->sync & FB_SYNC_VERT_HIGH_ACT) ? 0 : LDMT1R_VPOL;
	tmp |= (var->sync & FB_SYNC_HOR_HIGH_ACT) ? 0 : LDMT1R_HPOL;
	tmp |= (ch->cfg->flags & LCDC_FLAGS_DWPOL) ? LDMT1R_DWPOL : 0;
	tmp |= (ch->cfg->flags & LCDC_FLAGS_DIPOL) ? LDMT1R_DIPOL : 0;
	tmp |= (ch->cfg->flags & LCDC_FLAGS_DAPOL) ? LDMT1R_DAPOL : 0;
	tmp |= (ch->cfg->flags & LCDC_FLAGS_HSCNT) ? LDMT1R_HSCNT : 0;
	tmp |= (ch->cfg->flags & LCDC_FLAGS_DWCNT) ? LDMT1R_DWCNT : 0;
	lcdc_write_chan(ch, LDMT1R, tmp);

	/* setup SYS bus */
	lcdc_write_chan(ch, LDMT2R, ch->cfg->sys_bus_cfg.ldmt2r);
	lcdc_write_chan(ch, LDMT3R, ch->cfg->sys_bus_cfg.ldmt3r);

	/* horizontal configuration */
	h_total = mode->xres + mode->hsync_len + mode->left_margin
		+ mode->right_margin;
	tmp = h_total / 8; /* HTCN */
	tmp |= (min(mode->xres, ch->xres) / 8) << 16; /* HDCN */
	lcdc_write_chan(ch, LDHCNR, tmp);

	hsync_pos = mode->xres + mode->right_margin;
	tmp = hsync_pos / 8; /* HSYNP */
	tmp |= (mode->hsync_len / 8) << 16; /* HSYNW */
	lcdc_write_chan(ch, LDHSYNR, tmp);

	/* vertical configuration */
	tmp = mode->yres + mode->vsync_len + mode->upper_margin
	    + mode->lower_margin; /* VTLN */
	tmp |= min(mode->yres, ch->yres) << 16; /* VDLN */
	lcdc_write_chan(ch, LDVLNR, tmp);

	tmp = mode->yres + mode->lower_margin; /* VSYNP */
	tmp |= mode->vsync_len << 16; /* VSYNW */
	lcdc_write_chan(ch, LDVSYNR, tmp);

	/* Adjust horizontal synchronisation for HDMI */
	display_h_total = mode->xres + mode->hsync_len + mode->left_margin
			+ mode->right_margin;
	tmp = ((mode->xres & 7) << 24) | ((display_h_total & 7) << 16)
	    | ((mode->hsync_len & 7) << 8) | (hsync_pos & 7);
	lcdc_write_chan(ch, LDHAJR, tmp);
<<<<<<< HEAD
}

/*
 * __sh_mobile_lcdc_start - Configure and tart the LCDC
=======
	lcdc_write_chan_mirror(ch, LDHAJR, tmp);
}

static void sh_mobile_lcdc_overlay_setup(struct sh_mobile_lcdc_overlay *ovl)
{
	u32 format = 0;

	if (!ovl->enabled) {
		lcdc_write(ovl->channel->lcdc, LDBCR, LDBCR_UPC(ovl->index));
		lcdc_write_overlay(ovl, LDBnBSIFR(ovl->index), 0);
		lcdc_write(ovl->channel->lcdc, LDBCR,
			   LDBCR_UPF(ovl->index) | LDBCR_UPD(ovl->index));
		return;
	}

	ovl->base_addr_y = ovl->dma_handle;
	ovl->base_addr_c = ovl->dma_handle
			 + ovl->xres_virtual * ovl->yres_virtual;

	switch (ovl->mode) {
	case LCDC_OVERLAY_BLEND:
		format = LDBBSIFR_EN | (ovl->alpha << LDBBSIFR_LAY_SHIFT);
		break;

	case LCDC_OVERLAY_ROP3:
		format = LDBBSIFR_EN | LDBBSIFR_BRSEL
		       | (ovl->rop3 << LDBBSIFR_ROP3_SHIFT);
		break;
	}

	switch (ovl->format->fourcc) {
	case V4L2_PIX_FMT_RGB565:
	case V4L2_PIX_FMT_NV21:
	case V4L2_PIX_FMT_NV61:
	case V4L2_PIX_FMT_NV42:
		format |= LDBBSIFR_SWPL | LDBBSIFR_SWPW;
		break;
	case V4L2_PIX_FMT_BGR24:
	case V4L2_PIX_FMT_NV12:
	case V4L2_PIX_FMT_NV16:
	case V4L2_PIX_FMT_NV24:
		format |= LDBBSIFR_SWPL | LDBBSIFR_SWPW | LDBBSIFR_SWPB;
		break;
	case V4L2_PIX_FMT_BGR32:
	default:
		format |= LDBBSIFR_SWPL;
		break;
	}

	switch (ovl->format->fourcc) {
	case V4L2_PIX_FMT_RGB565:
		format |= LDBBSIFR_AL_1 | LDBBSIFR_RY | LDBBSIFR_RPKF_RGB16;
		break;
	case V4L2_PIX_FMT_BGR24:
		format |= LDBBSIFR_AL_1 | LDBBSIFR_RY | LDBBSIFR_RPKF_RGB24;
		break;
	case V4L2_PIX_FMT_BGR32:
		format |= LDBBSIFR_AL_PK | LDBBSIFR_RY | LDDFR_PKF_ARGB32;
		break;
	case V4L2_PIX_FMT_NV12:
	case V4L2_PIX_FMT_NV21:
		format |= LDBBSIFR_AL_1 | LDBBSIFR_CHRR_420;
		break;
	case V4L2_PIX_FMT_NV16:
	case V4L2_PIX_FMT_NV61:
		format |= LDBBSIFR_AL_1 | LDBBSIFR_CHRR_422;
		break;
	case V4L2_PIX_FMT_NV24:
	case V4L2_PIX_FMT_NV42:
		format |= LDBBSIFR_AL_1 | LDBBSIFR_CHRR_444;
		break;
	}

	lcdc_write(ovl->channel->lcdc, LDBCR, LDBCR_UPC(ovl->index));

	lcdc_write_overlay(ovl, LDBnBSIFR(ovl->index), format);

	lcdc_write_overlay(ovl, LDBnBSSZR(ovl->index),
		(ovl->yres << LDBBSSZR_BVSS_SHIFT) |
		(ovl->xres << LDBBSSZR_BHSS_SHIFT));
	lcdc_write_overlay(ovl, LDBnBLOCR(ovl->index),
		(ovl->pos_y << LDBBLOCR_CVLC_SHIFT) |
		(ovl->pos_x << LDBBLOCR_CHLC_SHIFT));
	lcdc_write_overlay(ovl, LDBnBSMWR(ovl->index),
		ovl->pitch << LDBBSMWR_BSMW_SHIFT);

	lcdc_write_overlay(ovl, LDBnBSAYR(ovl->index), ovl->base_addr_y);
	lcdc_write_overlay(ovl, LDBnBSACR(ovl->index), ovl->base_addr_c);

	lcdc_write(ovl->channel->lcdc, LDBCR,
		   LDBCR_UPF(ovl->index) | LDBCR_UPD(ovl->index));
}

/*
 * __sh_mobile_lcdc_start - Configure and start the LCDC
>>>>>>> refs/remotes/origin/master
 * @priv: LCDC device
 *
 * Configure all enabled channels and start the LCDC device. All external
 * devices (clocks, MERAM, panels, ...) are not touched by this function.
 */
static void __sh_mobile_lcdc_start(struct sh_mobile_lcdc_priv *priv)
{
	struct sh_mobile_lcdc_chan *ch;
	unsigned long tmp;
	int k, m;

	/* Enable LCDC channels. Read data from external memory, avoid using the
	 * BEU for now.
	 */
	lcdc_write(priv, _LDCNT2R, priv->ch[0].enabled | priv->ch[1].enabled);

	/* Stop the LCDC first and disable all interrupts. */
	sh_mobile_lcdc_start_stop(priv, 0);
	lcdc_write(priv, _LDINTR, 0);

	/* Configure power supply, dot clocks and start them. */
	tmp = priv->lddckr;
	for (k = 0; k < ARRAY_SIZE(priv->ch); k++) {
		ch = &priv->ch[k];
		if (!ch->enabled)
			continue;

		/* Power supply */
		lcdc_write_chan(ch, LDPMR, 0);

		m = ch->cfg->clock_divider;
		if (!m)
			continue;

		/* FIXME: sh7724 can only use 42, 48, 54 and 60 for the divider
		 * denominator.
		 */
		lcdc_write_chan(ch, LDDCKPAT1R, 0);
		lcdc_write_chan(ch, LDDCKPAT2R, (1 << (m/2)) - 1);

		if (m == 1)
			m = LDDCKR_MOSEL;
		tmp |= m << (lcdc_chan_is_sublcd(ch) ? 8 : 0);
	}

	lcdc_write(priv, _LDDCKR, tmp);
	lcdc_write(priv, _LDDCKSTPR, 0);
	lcdc_wait_bit(priv, _LDDCKSTPR, ~0, 0);

	/* Setup geometry, format, frame buffer memory and operation mode. */
	for (k = 0; k < ARRAY_SIZE(priv->ch); k++) {
		ch = &priv->ch[k];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (!ch->enabled)
			continue;

		sh_mobile_lcdc_geometry(ch);

<<<<<<< HEAD
<<<<<<< HEAD
		/* power supply */
		lcdc_write_chan(ch, LDPMR, 0);

		board_cfg = &ch->cfg.board_cfg;
		if (board_cfg->setup_sys) {
			ret = board_cfg->setup_sys(board_cfg->board_data,
						ch, &sh_mobile_lcdc_sys_bus_ops);
			if (ret)
				return ret;
		}
	}

	/* word and long word swap */
	ldddsr = lcdc_read(priv, _LDDDSR);
	if  (priv->ch[0].info->var.nonstd)
		lcdc_write(priv, _LDDDSR, ldddsr | 7);
	else {
		switch (bpp) {
		case 16:
			lcdc_write(priv, _LDDDSR, ldddsr | 6);
			break;
		case 24:
			lcdc_write(priv, _LDDDSR, ldddsr | 7);
			break;
		case 32:
			lcdc_write(priv, _LDDDSR, ldddsr | 4);
			break;
		}
	}

	for (k = 0; k < ARRAY_SIZE(priv->ch); k++) {
		unsigned long base_addr_y;
		unsigned long base_addr_c = 0;
		int pitch;
		ch = &priv->ch[k];

		if (!priv->ch[k].enabled)
			continue;

		/* set bpp format in PKF[4:0] */
		tmp = lcdc_read_chan(ch, LDDFR);
		tmp &= ~0x0003031f;
		if (ch->info->var.nonstd) {
			tmp |= (ch->info->var.nonstd << 16);
			switch (ch->info->var.bits_per_pixel) {
			case 12:
				break;
			case 16:
				tmp |= (0x1 << 8);
				break;
			case 24:
				tmp |= (0x2 << 8);
				break;
			}
		} else {
			switch (ch->info->var.bits_per_pixel) {
			case 16:
				tmp |= 0x03;
				break;
			case 24:
				tmp |= 0x0b;
				break;
			case 32:
				break;
			}
		}
		lcdc_write_chan(ch, LDDFR, tmp);

		base_addr_y = ch->info->fix.smem_start;
		base_addr_c = base_addr_y +
				ch->info->var.xres *
				ch->info->var.yres_virtual;
		pitch = ch->info->fix.line_length;

		/* test if we can enable meram */
		if (ch->cfg.meram_cfg && priv->meram_dev &&
				priv->meram_dev->ops) {
			struct sh_mobile_meram_cfg *cfg;
			struct sh_mobile_meram_info *mdev;
			unsigned long icb_addr_y, icb_addr_c;
			int icb_pitch;
			int pf;

			cfg = ch->cfg.meram_cfg;
			mdev = priv->meram_dev;
			/* we need to de-init configured ICBs before we
			 * we can re-initialize them.
			 */
			if (ch->meram_enabled)
				mdev->ops->meram_unregister(mdev, cfg);

			ch->meram_enabled = 0;

			if (ch->info->var.nonstd) {
				if (ch->info->var.bits_per_pixel == 24)
					pf = SH_MOBILE_MERAM_PF_NV24;
				else
					pf = SH_MOBILE_MERAM_PF_NV;
			} else {
				pf = SH_MOBILE_MERAM_PF_RGB;
			}

			ret = mdev->ops->meram_register(mdev, cfg, pitch,
						ch->info->var.yres,
						pf,
						base_addr_y,
						base_addr_c,
						&icb_addr_y,
						&icb_addr_c,
						&icb_pitch);
			if (!ret)  {
				/* set LDSA1R value */
				base_addr_y = icb_addr_y;
				pitch = icb_pitch;

				/* set LDSA2R value if required */
				if (base_addr_c)
					base_addr_c = icb_addr_c;

				ch->meram_enabled = 1;
			}
		}

		/* point out our frame buffer */
		lcdc_write_chan(ch, LDSA1R, base_addr_y);
		if (ch->info->var.nonstd)
			lcdc_write_chan(ch, LDSA2R, base_addr_c);

		/* set line size */
		lcdc_write_chan(ch, LDMLSR, pitch);

		/* setup deferred io if SYS bus */
		tmp = ch->cfg.sys_bus_cfg.deferred_io_msec;
		if (ch->ldmt1r_value & (1 << 12) && tmp) {
			ch->defio.deferred_io = sh_mobile_lcdc_deferred_io;
			ch->defio.delay = msecs_to_jiffies(tmp);
			ch->info->fbdefio = &ch->defio;
			fb_deferred_io_init(ch->info);

			/* one-shot mode */
			lcdc_write_chan(ch, LDSM1R, 1);

			/* enable "Frame End Interrupt Enable" bit */
			lcdc_write(priv, _LDINTR, LDINTR_FE);

		} else {
			/* continuous read mode */
			lcdc_write_chan(ch, LDSM1R, 0);
		}
	}

	/* display output */
	lcdc_write(priv, _LDCNT1R, LCDC_ENABLE);

	/* start the lcdc */
	sh_mobile_lcdc_start_stop(priv, 1);
	priv->started = 1;

	/* tell the board code to enable the panel */
=======
		tmp = ch->format->lddfr;

		if (ch->format->yuv) {
			switch (ch->colorspace) {
			case V4L2_COLORSPACE_REC709:
				tmp |= LDDFR_CF1;
				break;
			case V4L2_COLORSPACE_JPEG:
				tmp |= LDDFR_CF0;
=======
		tmp = ch->format->lddfr;

		if (ch->format->yuv) {
			switch (ch->colorspace) {
			case V4L2_COLORSPACE_REC709:
				tmp |= LDDFR_CF1;
				break;
			case V4L2_COLORSPACE_JPEG:
				tmp |= LDDFR_CF0;
>>>>>>> refs/remotes/origin/master
				break;
			}
		}

		lcdc_write_chan(ch, LDDFR, tmp);
<<<<<<< HEAD
		lcdc_write_chan(ch, LDMLSR, ch->pitch);
=======
		lcdc_write_chan(ch, LDMLSR, ch->line_size);
>>>>>>> refs/remotes/origin/master
		lcdc_write_chan(ch, LDSA1R, ch->base_addr_y);
		if (ch->format->yuv)
			lcdc_write_chan(ch, LDSA2R, ch->base_addr_c);

		/* When using deferred I/O mode, configure the LCDC for one-shot
		 * operation and enable the frame end interrupt. Otherwise use
		 * continuous read mode.
		 */
		if (ch->ldmt1r_value & LDMT1R_IFM &&
		    ch->cfg->sys_bus_cfg.deferred_io_msec) {
			lcdc_write_chan(ch, LDSM1R, LDSM1R_OS);
			lcdc_write(priv, _LDINTR, LDINTR_FE);
		} else {
			lcdc_write_chan(ch, LDSM1R, 0);
		}
	}

	/* Word and long word swap. */
	switch (priv->ch[0].format->fourcc) {
	case V4L2_PIX_FMT_RGB565:
	case V4L2_PIX_FMT_NV21:
	case V4L2_PIX_FMT_NV61:
	case V4L2_PIX_FMT_NV42:
		tmp = LDDDSR_LS | LDDDSR_WS;
		break;
	case V4L2_PIX_FMT_BGR24:
	case V4L2_PIX_FMT_NV12:
	case V4L2_PIX_FMT_NV16:
	case V4L2_PIX_FMT_NV24:
		tmp = LDDDSR_LS | LDDDSR_WS | LDDDSR_BS;
		break;
	case V4L2_PIX_FMT_BGR32:
	default:
		tmp = LDDDSR_LS;
		break;
	}
	lcdc_write(priv, _LDDDSR, tmp);

	/* Enable the display output. */
	lcdc_write(priv, _LDCNT1R, LDCNT1R_DE);
	sh_mobile_lcdc_start_stop(priv, 1);
	priv->started = 1;
}

static int sh_mobile_lcdc_start(struct sh_mobile_lcdc_priv *priv)
{
	struct sh_mobile_meram_info *mdev = priv->meram_dev;
	struct sh_mobile_lcdc_chan *ch;
	unsigned long tmp;
	int ret;
	int k;

	/* enable clocks before accessing the hardware */
	for (k = 0; k < ARRAY_SIZE(priv->ch); k++) {
		if (priv->ch[k].enabled)
			sh_mobile_lcdc_clk_on(priv);
	}

	/* reset */
	lcdc_write(priv, _LDCNT2R, lcdc_read(priv, _LDCNT2R) | LDCNT2R_BR);
	lcdc_wait_bit(priv, _LDCNT2R, LDCNT2R_BR, 0);

	for (k = 0; k < ARRAY_SIZE(priv->ch); k++) {
		const struct sh_mobile_lcdc_panel_cfg *panel;

		ch = &priv->ch[k];
		if (!ch->enabled)
			continue;

		panel = &ch->cfg->panel_cfg;
		if (panel->setup_sys) {
			ret = panel->setup_sys(ch, &sh_mobile_lcdc_sys_bus_ops);
			if (ret)
				return ret;
		}
	}

	/* Compute frame buffer base address and pitch for each channel. */
	for (k = 0; k < ARRAY_SIZE(priv->ch); k++) {
		int pixelformat;
<<<<<<< HEAD
		void *meram;
=======
		void *cache;
>>>>>>> refs/remotes/origin/master

		ch = &priv->ch[k];
		if (!ch->enabled)
			continue;

		ch->base_addr_y = ch->dma_handle;
<<<<<<< HEAD
		ch->base_addr_c = ch->base_addr_y + ch->xres * ch->yres_virtual;

		/* Enable MERAM if possible. */
		if (mdev == NULL || mdev->ops == NULL ||
		    ch->cfg->meram_cfg == NULL)
			continue;

		/* we need to de-init configured ICBs before we can
		 * re-initialize them.
		 */
		if (ch->meram) {
			mdev->ops->meram_unregister(mdev, ch->meram);
			ch->meram = NULL;
=======
		ch->base_addr_c = ch->dma_handle
				+ ch->xres_virtual * ch->yres_virtual;
		ch->line_size = ch->pitch;

		/* Enable MERAM if possible. */
		if (mdev == NULL || ch->cfg->meram_cfg == NULL)
			continue;

		/* Free the allocated MERAM cache. */
		if (ch->cache) {
			sh_mobile_meram_cache_free(mdev, ch->cache);
			ch->cache = NULL;
>>>>>>> refs/remotes/origin/master
		}

		switch (ch->format->fourcc) {
		case V4L2_PIX_FMT_NV12:
		case V4L2_PIX_FMT_NV21:
		case V4L2_PIX_FMT_NV16:
		case V4L2_PIX_FMT_NV61:
			pixelformat = SH_MOBILE_MERAM_PF_NV;
			break;
		case V4L2_PIX_FMT_NV24:
		case V4L2_PIX_FMT_NV42:
			pixelformat = SH_MOBILE_MERAM_PF_NV24;
			break;
		case V4L2_PIX_FMT_RGB565:
		case V4L2_PIX_FMT_BGR24:
		case V4L2_PIX_FMT_BGR32:
		default:
			pixelformat = SH_MOBILE_MERAM_PF_RGB;
			break;
		}

<<<<<<< HEAD
		meram = mdev->ops->meram_register(mdev, ch->cfg->meram_cfg,
					ch->pitch, ch->yres, pixelformat,
					&ch->pitch);
		if (!IS_ERR(meram)) {
			mdev->ops->meram_update(mdev, meram,
					ch->base_addr_y, ch->base_addr_c,
					&ch->base_addr_y, &ch->base_addr_c);
			ch->meram = meram;
		}
	}

=======
		cache = sh_mobile_meram_cache_alloc(mdev, ch->cfg->meram_cfg,
					ch->pitch, ch->yres, pixelformat,
					&ch->line_size);
		if (!IS_ERR(cache)) {
			sh_mobile_meram_cache_update(mdev, cache,
					ch->base_addr_y, ch->base_addr_c,
					&ch->base_addr_y, &ch->base_addr_c);
			ch->cache = cache;
		}
	}

	for (k = 0; k < ARRAY_SIZE(priv->overlays); ++k) {
		struct sh_mobile_lcdc_overlay *ovl = &priv->overlays[k];
		sh_mobile_lcdc_overlay_setup(ovl);
	}

>>>>>>> refs/remotes/origin/master
	/* Start the LCDC. */
	__sh_mobile_lcdc_start(priv);

	/* Setup deferred I/O, tell the board code to enable the panels, and
	 * turn backlight on.
	 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	for (k = 0; k < ARRAY_SIZE(priv->ch); k++) {
		ch = &priv->ch[k];
		if (!ch->enabled)
			continue;

<<<<<<< HEAD
<<<<<<< HEAD
		board_cfg = &ch->cfg.board_cfg;
		if (board_cfg->display_on && try_module_get(board_cfg->owner)) {
			board_cfg->display_on(board_cfg->board_data, ch->info);
			module_put(board_cfg->owner);
		}

=======
=======
>>>>>>> refs/remotes/origin/master
		tmp = ch->cfg->sys_bus_cfg.deferred_io_msec;
		if (ch->ldmt1r_value & LDMT1R_IFM && tmp) {
			ch->defio.deferred_io = sh_mobile_lcdc_deferred_io;
			ch->defio.delay = msecs_to_jiffies(tmp);
			ch->info->fbdefio = &ch->defio;
			fb_deferred_io_init(ch->info);
		}

		sh_mobile_lcdc_display_on(ch);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (ch->bl) {
			ch->bl->props.power = FB_BLANK_UNBLANK;
			backlight_update_status(ch->bl);
		}
	}

	return 0;
}

static void sh_mobile_lcdc_stop(struct sh_mobile_lcdc_priv *priv)
{
	struct sh_mobile_lcdc_chan *ch;
<<<<<<< HEAD
<<<<<<< HEAD
	struct sh_mobile_lcdc_board_cfg	*board_cfg;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	int k;

	/* clean up deferred io and ask board code to disable panel */
	for (k = 0; k < ARRAY_SIZE(priv->ch); k++) {
		ch = &priv->ch[k];
		if (!ch->enabled)
			continue;

		/* deferred io mode:
		 * flush frame, and wait for frame end interrupt
		 * clean up deferred io and enable clock
		 */
		if (ch->info && ch->info->fbdefio) {
			ch->frame_end = 0;
			schedule_delayed_work(&ch->info->deferred_work, 0);
			wait_event(ch->frame_end_wait, ch->frame_end);
			fb_deferred_io_cleanup(ch->info);
			ch->info->fbdefio = NULL;
			sh_mobile_lcdc_clk_on(priv);
		}

		if (ch->bl) {
			ch->bl->props.power = FB_BLANK_POWERDOWN;
			backlight_update_status(ch->bl);
		}

<<<<<<< HEAD
<<<<<<< HEAD
		board_cfg = &ch->cfg.board_cfg;
		if (board_cfg->display_off && try_module_get(board_cfg->owner)) {
			board_cfg->display_off(board_cfg->board_data);
			module_put(board_cfg->owner);
		}

		/* disable the meram */
		if (ch->meram_enabled) {
			struct sh_mobile_meram_cfg *cfg;
			struct sh_mobile_meram_info *mdev;
			cfg = ch->cfg.meram_cfg;
			mdev = priv->meram_dev;
			mdev->ops->meram_unregister(mdev, cfg);
			ch->meram_enabled = 0;
=======
		sh_mobile_lcdc_display_off(ch);

		/* disable the meram */
		if (ch->meram) {
			struct sh_mobile_meram_info *mdev;
			mdev = priv->meram_dev;
			mdev->ops->meram_unregister(mdev, ch->meram);
			ch->meram = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		sh_mobile_lcdc_display_off(ch);

		/* Free the MERAM cache. */
		if (ch->cache) {
			sh_mobile_meram_cache_free(priv->meram_dev, ch->cache);
			ch->cache = 0;
>>>>>>> refs/remotes/origin/master
		}

	}

	/* stop the lcdc */
	if (priv->started) {
		sh_mobile_lcdc_start_stop(priv, 0);
		priv->started = 0;
	}

	/* stop clocks */
	for (k = 0; k < ARRAY_SIZE(priv->ch); k++)
		if (priv->ch[k].enabled)
			sh_mobile_lcdc_clk_off(priv);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int sh_mobile_lcdc_check_interface(struct sh_mobile_lcdc_chan *ch)
{
	int ifm, miftyp;

	switch (ch->cfg.interface_type) {
	case RGB8: ifm = 0; miftyp = 0; break;
	case RGB9: ifm = 0; miftyp = 4; break;
	case RGB12A: ifm = 0; miftyp = 5; break;
	case RGB12B: ifm = 0; miftyp = 6; break;
	case RGB16: ifm = 0; miftyp = 7; break;
	case RGB18: ifm = 0; miftyp = 10; break;
	case RGB24: ifm = 0; miftyp = 11; break;
	case SYS8A: ifm = 1; miftyp = 0; break;
	case SYS8B: ifm = 1; miftyp = 1; break;
	case SYS8C: ifm = 1; miftyp = 2; break;
	case SYS8D: ifm = 1; miftyp = 3; break;
	case SYS9: ifm = 1; miftyp = 4; break;
	case SYS12: ifm = 1; miftyp = 5; break;
	case SYS16A: ifm = 1; miftyp = 7; break;
	case SYS16B: ifm = 1; miftyp = 8; break;
	case SYS16C: ifm = 1; miftyp = 9; break;
	case SYS18: ifm = 1; miftyp = 10; break;
	case SYS24: ifm = 1; miftyp = 11; break;
	default: goto bad;
	}

	/* SUBLCD only supports SYS interface */
	if (lcdc_chan_is_sublcd(ch)) {
		if (ifm == 0)
			goto bad;
		else
			ifm = 0;
	}

	ch->ldmt1r_value = (ifm << 12) | miftyp;
	return 0;
 bad:
	return -EINVAL;
}

static int sh_mobile_lcdc_setup_clocks(struct platform_device *pdev,
				       int clock_source,
				       struct sh_mobile_lcdc_priv *priv)
{
	char *str;
	int icksel;

	switch (clock_source) {
	case LCDC_CLK_BUS: str = "bus_clk"; icksel = 0; break;
	case LCDC_CLK_PERIPHERAL: str = "peripheral_clk"; icksel = 1; break;
	case LCDC_CLK_EXTERNAL: str = NULL; icksel = 2; break;
	default:
		return -EINVAL;
	}

	priv->lddckr = icksel << 16;

	if (str) {
		priv->dot_clk = clk_get(&pdev->dev, str);
		if (IS_ERR(priv->dot_clk)) {
			dev_err(&pdev->dev, "cannot get dot clock %s\n", str);
			return PTR_ERR(priv->dot_clk);
		}
	}

	/* Runtime PM support involves two step for this driver:
	 * 1) Enable Runtime PM
	 * 2) Force Runtime PM Resume since hardware is accessed from probe()
	 */
	priv->dev = &pdev->dev;
	pm_runtime_enable(priv->dev);
	pm_runtime_resume(priv->dev);
	return 0;
}
=======
/* -----------------------------------------------------------------------------
 * Frame buffer operations
 */
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int __sh_mobile_lcdc_check_var(struct fb_var_screeninfo *var,
				      struct fb_info *info)
{
	if (var->xres > MAX_XRES || var->yres > MAX_YRES)
		return -EINVAL;

	/* Make sure the virtual resolution is at least as big as the visible
	 * resolution.
	 */
	if (var->xres_virtual < var->xres)
		var->xres_virtual = var->xres;
	if (var->yres_virtual < var->yres)
		var->yres_virtual = var->yres;

	if (sh_mobile_format_is_fourcc(var)) {
		const struct sh_mobile_lcdc_format_info *format;

		format = sh_mobile_format_info(var->grayscale);
		if (format == NULL)
			return -EINVAL;
		var->bits_per_pixel = format->bpp;

		/* Default to RGB and JPEG color-spaces for RGB and YUV formats
		 * respectively.
		 */
		if (!format->yuv)
			var->colorspace = V4L2_COLORSPACE_SRGB;
		else if (var->colorspace != V4L2_COLORSPACE_REC709)
			var->colorspace = V4L2_COLORSPACE_JPEG;
	} else {
		if (var->bits_per_pixel <= 16) {		/* RGB 565 */
			var->bits_per_pixel = 16;
			var->red.offset = 11;
			var->red.length = 5;
			var->green.offset = 5;
			var->green.length = 6;
			var->blue.offset = 0;
			var->blue.length = 5;
			var->transp.offset = 0;
			var->transp.length = 0;
		} else if (var->bits_per_pixel <= 24) {		/* RGB 888 */
			var->bits_per_pixel = 24;
			var->red.offset = 16;
			var->red.length = 8;
			var->green.offset = 8;
			var->green.length = 8;
			var->blue.offset = 0;
			var->blue.length = 8;
			var->transp.offset = 0;
			var->transp.length = 0;
		} else if (var->bits_per_pixel <= 32) {		/* RGBA 888 */
			var->bits_per_pixel = 32;
			var->red.offset = 16;
			var->red.length = 8;
			var->green.offset = 8;
			var->green.length = 8;
			var->blue.offset = 0;
			var->blue.length = 8;
			var->transp.offset = 24;
			var->transp.length = 8;
		} else
			return -EINVAL;

		var->red.msb_right = 0;
		var->green.msb_right = 0;
		var->blue.msb_right = 0;
		var->transp.msb_right = 0;
	}

	/* Make sure we don't exceed our allocated memory. */
	if (var->xres_virtual * var->yres_virtual * var->bits_per_pixel / 8 >
	    info->fix.smem_len)
		return -EINVAL;

	return 0;
}

/* -----------------------------------------------------------------------------
 * Frame buffer operations - Overlays
 */

static ssize_t
overlay_alpha_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct fb_info *info = dev_get_drvdata(dev);
	struct sh_mobile_lcdc_overlay *ovl = info->par;

	return scnprintf(buf, PAGE_SIZE, "%u\n", ovl->alpha);
}

static ssize_t
overlay_alpha_store(struct device *dev, struct device_attribute *attr,
		    const char *buf, size_t count)
{
	struct fb_info *info = dev_get_drvdata(dev);
	struct sh_mobile_lcdc_overlay *ovl = info->par;
	unsigned int alpha;
	char *endp;

	alpha = simple_strtoul(buf, &endp, 10);
	if (isspace(*endp))
		endp++;

	if (endp - buf != count)
		return -EINVAL;

	if (alpha > 255)
		return -EINVAL;

	if (ovl->alpha != alpha) {
		ovl->alpha = alpha;

		if (ovl->mode == LCDC_OVERLAY_BLEND && ovl->enabled)
			sh_mobile_lcdc_overlay_setup(ovl);
	}

	return count;
}

static ssize_t
overlay_mode_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct fb_info *info = dev_get_drvdata(dev);
	struct sh_mobile_lcdc_overlay *ovl = info->par;

	return scnprintf(buf, PAGE_SIZE, "%u\n", ovl->mode);
}

static ssize_t
overlay_mode_store(struct device *dev, struct device_attribute *attr,
		   const char *buf, size_t count)
{
	struct fb_info *info = dev_get_drvdata(dev);
	struct sh_mobile_lcdc_overlay *ovl = info->par;
	unsigned int mode;
	char *endp;

	mode = simple_strtoul(buf, &endp, 10);
	if (isspace(*endp))
		endp++;

	if (endp - buf != count)
		return -EINVAL;

	if (mode != LCDC_OVERLAY_BLEND && mode != LCDC_OVERLAY_ROP3)
		return -EINVAL;

	if (ovl->mode != mode) {
		ovl->mode = mode;

		if (ovl->enabled)
			sh_mobile_lcdc_overlay_setup(ovl);
	}

	return count;
}

static ssize_t
overlay_position_show(struct device *dev, struct device_attribute *attr,
		      char *buf)
{
	struct fb_info *info = dev_get_drvdata(dev);
	struct sh_mobile_lcdc_overlay *ovl = info->par;

	return scnprintf(buf, PAGE_SIZE, "%d,%d\n", ovl->pos_x, ovl->pos_y);
}

static ssize_t
overlay_position_store(struct device *dev, struct device_attribute *attr,
		       const char *buf, size_t count)
{
	struct fb_info *info = dev_get_drvdata(dev);
	struct sh_mobile_lcdc_overlay *ovl = info->par;
	char *endp;
	int pos_x;
	int pos_y;

	pos_x = simple_strtol(buf, &endp, 10);
	if (*endp != ',')
		return -EINVAL;

	pos_y = simple_strtol(endp + 1, &endp, 10);
	if (isspace(*endp))
		endp++;

	if (endp - buf != count)
		return -EINVAL;

	if (ovl->pos_x != pos_x || ovl->pos_y != pos_y) {
		ovl->pos_x = pos_x;
		ovl->pos_y = pos_y;

		if (ovl->enabled)
			sh_mobile_lcdc_overlay_setup(ovl);
	}

	return count;
}

static ssize_t
overlay_rop3_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct fb_info *info = dev_get_drvdata(dev);
	struct sh_mobile_lcdc_overlay *ovl = info->par;

	return scnprintf(buf, PAGE_SIZE, "%u\n", ovl->rop3);
}

static ssize_t
overlay_rop3_store(struct device *dev, struct device_attribute *attr,
		    const char *buf, size_t count)
{
	struct fb_info *info = dev_get_drvdata(dev);
	struct sh_mobile_lcdc_overlay *ovl = info->par;
	unsigned int rop3;
	char *endp;

	rop3 = !!simple_strtoul(buf, &endp, 10);
	if (isspace(*endp))
		endp++;

	if (endp - buf != count)
		return -EINVAL;

	if (rop3 > 255)
		return -EINVAL;

	if (ovl->rop3 != rop3) {
		ovl->rop3 = rop3;

		if (ovl->mode == LCDC_OVERLAY_ROP3 && ovl->enabled)
			sh_mobile_lcdc_overlay_setup(ovl);
	}

	return count;
}

static const struct device_attribute overlay_sysfs_attrs[] = {
	__ATTR(ovl_alpha, S_IRUGO|S_IWUSR,
	       overlay_alpha_show, overlay_alpha_store),
	__ATTR(ovl_mode, S_IRUGO|S_IWUSR,
	       overlay_mode_show, overlay_mode_store),
	__ATTR(ovl_position, S_IRUGO|S_IWUSR,
	       overlay_position_show, overlay_position_store),
	__ATTR(ovl_rop3, S_IRUGO|S_IWUSR,
	       overlay_rop3_show, overlay_rop3_store),
};

static const struct fb_fix_screeninfo sh_mobile_lcdc_overlay_fix  = {
	.id =		"SH Mobile LCDC",
	.type =		FB_TYPE_PACKED_PIXELS,
	.visual =	FB_VISUAL_TRUECOLOR,
	.accel =	FB_ACCEL_NONE,
	.xpanstep =	1,
	.ypanstep =	1,
	.ywrapstep =	0,
	.capabilities =	FB_CAP_FOURCC,
};

static int sh_mobile_lcdc_overlay_pan(struct fb_var_screeninfo *var,
				    struct fb_info *info)
{
	struct sh_mobile_lcdc_overlay *ovl = info->par;
	unsigned long base_addr_y;
	unsigned long base_addr_c;
	unsigned long y_offset;
	unsigned long c_offset;

	if (!ovl->format->yuv) {
		y_offset = (var->yoffset * ovl->xres_virtual + var->xoffset)
			 * ovl->format->bpp / 8;
		c_offset = 0;
	} else {
		unsigned int xsub = ovl->format->bpp < 24 ? 2 : 1;
		unsigned int ysub = ovl->format->bpp < 16 ? 2 : 1;

		y_offset = var->yoffset * ovl->xres_virtual + var->xoffset;
		c_offset = var->yoffset / ysub * ovl->xres_virtual * 2 / xsub
			 + var->xoffset * 2 / xsub;
	}

	/* If the Y offset hasn't changed, the C offset hasn't either. There's
	 * nothing to do in that case.
	 */
	if (y_offset == ovl->pan_y_offset)
		return 0;

	/* Set the source address for the next refresh */
	base_addr_y = ovl->dma_handle + y_offset;
	base_addr_c = ovl->dma_handle + ovl->xres_virtual * ovl->yres_virtual
		    + c_offset;

	ovl->base_addr_y = base_addr_y;
	ovl->base_addr_c = base_addr_c;
	ovl->pan_y_offset = y_offset;

	lcdc_write(ovl->channel->lcdc, LDBCR, LDBCR_UPC(ovl->index));

	lcdc_write_overlay(ovl, LDBnBSAYR(ovl->index), ovl->base_addr_y);
	lcdc_write_overlay(ovl, LDBnBSACR(ovl->index), ovl->base_addr_c);

	lcdc_write(ovl->channel->lcdc, LDBCR,
		   LDBCR_UPF(ovl->index) | LDBCR_UPD(ovl->index));

	return 0;
}

static int sh_mobile_lcdc_overlay_ioctl(struct fb_info *info, unsigned int cmd,
				      unsigned long arg)
{
	struct sh_mobile_lcdc_overlay *ovl = info->par;

	switch (cmd) {
	case FBIO_WAITFORVSYNC:
		return sh_mobile_lcdc_wait_for_vsync(ovl->channel);

	default:
		return -ENOIOCTLCMD;
	}
}

static int sh_mobile_lcdc_overlay_check_var(struct fb_var_screeninfo *var,
					  struct fb_info *info)
{
	return __sh_mobile_lcdc_check_var(var, info);
}

static int sh_mobile_lcdc_overlay_set_par(struct fb_info *info)
{
	struct sh_mobile_lcdc_overlay *ovl = info->par;

	ovl->format =
		sh_mobile_format_info(sh_mobile_format_fourcc(&info->var));

	ovl->xres = info->var.xres;
	ovl->xres_virtual = info->var.xres_virtual;
	ovl->yres = info->var.yres;
	ovl->yres_virtual = info->var.yres_virtual;

	if (ovl->format->yuv)
		ovl->pitch = info->var.xres_virtual;
	else
		ovl->pitch = info->var.xres_virtual * ovl->format->bpp / 8;

	sh_mobile_lcdc_overlay_setup(ovl);

	info->fix.line_length = ovl->pitch;

	if (sh_mobile_format_is_fourcc(&info->var)) {
		info->fix.type = FB_TYPE_FOURCC;
		info->fix.visual = FB_VISUAL_FOURCC;
	} else {
		info->fix.type = FB_TYPE_PACKED_PIXELS;
		info->fix.visual = FB_VISUAL_TRUECOLOR;
	}

	return 0;
}

/* Overlay blanking. Disable the overlay when blanked. */
static int sh_mobile_lcdc_overlay_blank(int blank, struct fb_info *info)
{
	struct sh_mobile_lcdc_overlay *ovl = info->par;

	ovl->enabled = !blank;
	sh_mobile_lcdc_overlay_setup(ovl);

	/* Prevent the backlight from receiving a blanking event by returning
	 * a non-zero value.
	 */
	return 1;
}

static int
sh_mobile_lcdc_overlay_mmap(struct fb_info *info, struct vm_area_struct *vma)
{
	struct sh_mobile_lcdc_overlay *ovl = info->par;

	return dma_mmap_coherent(ovl->channel->lcdc->dev, vma, ovl->fb_mem,
				 ovl->dma_handle, ovl->fb_size);
}

static struct fb_ops sh_mobile_lcdc_overlay_ops = {
	.owner          = THIS_MODULE,
	.fb_read        = fb_sys_read,
	.fb_write       = fb_sys_write,
	.fb_fillrect	= sys_fillrect,
	.fb_copyarea	= sys_copyarea,
	.fb_imageblit	= sys_imageblit,
	.fb_blank	= sh_mobile_lcdc_overlay_blank,
	.fb_pan_display = sh_mobile_lcdc_overlay_pan,
	.fb_ioctl       = sh_mobile_lcdc_overlay_ioctl,
	.fb_check_var	= sh_mobile_lcdc_overlay_check_var,
	.fb_set_par	= sh_mobile_lcdc_overlay_set_par,
	.fb_mmap	= sh_mobile_lcdc_overlay_mmap,
};

static void
sh_mobile_lcdc_overlay_fb_unregister(struct sh_mobile_lcdc_overlay *ovl)
{
	struct fb_info *info = ovl->info;

	if (info == NULL || info->dev == NULL)
		return;

	unregister_framebuffer(ovl->info);
}

static int
sh_mobile_lcdc_overlay_fb_register(struct sh_mobile_lcdc_overlay *ovl)
{
	struct sh_mobile_lcdc_priv *lcdc = ovl->channel->lcdc;
	struct fb_info *info = ovl->info;
	unsigned int i;
	int ret;

	if (info == NULL)
		return 0;

	ret = register_framebuffer(info);
	if (ret < 0)
		return ret;

	dev_info(lcdc->dev, "registered %s/overlay %u as %dx%d %dbpp.\n",
		 dev_name(lcdc->dev), ovl->index, info->var.xres,
		 info->var.yres, info->var.bits_per_pixel);

	for (i = 0; i < ARRAY_SIZE(overlay_sysfs_attrs); ++i) {
		ret = device_create_file(info->dev, &overlay_sysfs_attrs[i]);
		if (ret < 0)
			return ret;
	}

	return 0;
}

static void
sh_mobile_lcdc_overlay_fb_cleanup(struct sh_mobile_lcdc_overlay *ovl)
{
	struct fb_info *info = ovl->info;

	if (info == NULL || info->device == NULL)
		return;

	framebuffer_release(info);
}

static int
sh_mobile_lcdc_overlay_fb_init(struct sh_mobile_lcdc_overlay *ovl)
{
	struct sh_mobile_lcdc_priv *priv = ovl->channel->lcdc;
	struct fb_var_screeninfo *var;
	struct fb_info *info;

	/* Allocate and initialize the frame buffer device. */
	info = framebuffer_alloc(0, priv->dev);
	if (info == NULL) {
		dev_err(priv->dev, "unable to allocate fb_info\n");
		return -ENOMEM;
	}

	ovl->info = info;

	info->flags = FBINFO_FLAG_DEFAULT;
	info->fbops = &sh_mobile_lcdc_overlay_ops;
	info->device = priv->dev;
	info->screen_base = ovl->fb_mem;
	info->par = ovl;

	/* Initialize fixed screen information. Restrict pan to 2 lines steps
	 * for NV12 and NV21.
	 */
	info->fix = sh_mobile_lcdc_overlay_fix;
	snprintf(info->fix.id, sizeof(info->fix.id),
		 "SH Mobile LCDC Overlay %u", ovl->index);
	info->fix.smem_start = ovl->dma_handle;
	info->fix.smem_len = ovl->fb_size;
	info->fix.line_length = ovl->pitch;

	if (ovl->format->yuv)
		info->fix.visual = FB_VISUAL_FOURCC;
	else
		info->fix.visual = FB_VISUAL_TRUECOLOR;

	switch (ovl->format->fourcc) {
	case V4L2_PIX_FMT_NV12:
	case V4L2_PIX_FMT_NV21:
		info->fix.ypanstep = 2;
	case V4L2_PIX_FMT_NV16:
	case V4L2_PIX_FMT_NV61:
		info->fix.xpanstep = 2;
	}

	/* Initialize variable screen information. */
	var = &info->var;
	memset(var, 0, sizeof(*var));
	var->xres = ovl->xres;
	var->yres = ovl->yres;
	var->xres_virtual = ovl->xres_virtual;
	var->yres_virtual = ovl->yres_virtual;
	var->activate = FB_ACTIVATE_NOW;

	/* Use the legacy API by default for RGB formats, and the FOURCC API
	 * for YUV formats.
	 */
	if (!ovl->format->yuv)
		var->bits_per_pixel = ovl->format->bpp;
	else
		var->grayscale = ovl->format->fourcc;

	return sh_mobile_lcdc_overlay_check_var(var, info);
}

/* -----------------------------------------------------------------------------
 * Frame buffer operations - main frame buffer
 */
>>>>>>> refs/remotes/origin/master

static int sh_mobile_lcdc_setcolreg(u_int regno,
				    u_int red, u_int green, u_int blue,
				    u_int transp, struct fb_info *info)
{
	u32 *palette = info->pseudo_palette;

	if (regno >= PALETTE_NR)
		return -EINVAL;

	/* only FB_VISUAL_TRUECOLOR supported */

	red >>= 16 - info->var.red.length;
	green >>= 16 - info->var.green.length;
	blue >>= 16 - info->var.blue.length;
	transp >>= 16 - info->var.transp.length;

	palette[regno] = (red << info->var.red.offset) |
	  (green << info->var.green.offset) |
	  (blue << info->var.blue.offset) |
	  (transp << info->var.transp.offset);

	return 0;
}

<<<<<<< HEAD
static struct fb_fix_screeninfo sh_mobile_lcdc_fix  = {
=======
static const struct fb_fix_screeninfo sh_mobile_lcdc_fix  = {
>>>>>>> refs/remotes/origin/master
	.id =		"SH Mobile LCDC",
	.type =		FB_TYPE_PACKED_PIXELS,
	.visual =	FB_VISUAL_TRUECOLOR,
	.accel =	FB_ACCEL_NONE,
<<<<<<< HEAD
	.xpanstep =	0,
	.ypanstep =	1,
	.ywrapstep =	0,
<<<<<<< HEAD
=======
	.capabilities =	FB_CAP_FOURCC,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.xpanstep =	1,
	.ypanstep =	1,
	.ywrapstep =	0,
	.capabilities =	FB_CAP_FOURCC,
>>>>>>> refs/remotes/origin/master
};

static void sh_mobile_lcdc_fillrect(struct fb_info *info,
				    const struct fb_fillrect *rect)
{
	sys_fillrect(info, rect);
	sh_mobile_lcdc_deferred_io_touch(info);
}

static void sh_mobile_lcdc_copyarea(struct fb_info *info,
				    const struct fb_copyarea *area)
{
	sys_copyarea(info, area);
	sh_mobile_lcdc_deferred_io_touch(info);
}

static void sh_mobile_lcdc_imageblit(struct fb_info *info,
				     const struct fb_image *image)
{
	sys_imageblit(info, image);
	sh_mobile_lcdc_deferred_io_touch(info);
}

<<<<<<< HEAD
static int sh_mobile_fb_pan_display(struct fb_var_screeninfo *var,
				     struct fb_info *info)
=======
static int sh_mobile_lcdc_pan(struct fb_var_screeninfo *var,
			      struct fb_info *info)
>>>>>>> refs/remotes/origin/master
{
	struct sh_mobile_lcdc_chan *ch = info->par;
	struct sh_mobile_lcdc_priv *priv = ch->lcdc;
	unsigned long ldrcntr;
<<<<<<< HEAD
	unsigned long new_pan_offset;
	unsigned long base_addr_y, base_addr_c;
	unsigned long c_offset;

<<<<<<< HEAD
	if (!var->nonstd)
		new_pan_offset = (var->yoffset * info->fix.line_length) +
			(var->xoffset * (info->var.bits_per_pixel / 8));
	else
		new_pan_offset = (var->yoffset * info->fix.line_length) +
			(var->xoffset);
=======
	if (!ch->format->yuv)
		new_pan_offset = var->yoffset * ch->pitch
			       + var->xoffset * (ch->format->bpp / 8);
	else
		new_pan_offset = var->yoffset * ch->pitch + var->xoffset;
>>>>>>> refs/remotes/origin/cm-10.0

	if (new_pan_offset == ch->pan_offset)
		return 0;	/* No change, do nothing */

	ldrcntr = lcdc_read(priv, _LDRCNTR);

	/* Set the source address for the next refresh */
	base_addr_y = ch->dma_handle + new_pan_offset;
<<<<<<< HEAD
	if (var->nonstd) {
		/* Set y offset */
		c_offset = (var->yoffset *
			info->fix.line_length *
			(info->var.bits_per_pixel - 8)) / 8;
		base_addr_c = ch->dma_handle + var->xres * var->yres_virtual +
			c_offset;
		/* Set x offset */
		if (info->var.bits_per_pixel == 24)
			base_addr_c += 2 * var->xoffset;
		else
			base_addr_c += var->xoffset;
	} else
		base_addr_c = 0;

	if (!ch->meram_enabled) {
		lcdc_write_chan_mirror(ch, LDSA1R, base_addr_y);
		if (base_addr_c)
			lcdc_write_chan_mirror(ch, LDSA2R, base_addr_c);
	} else {
		struct sh_mobile_meram_cfg *cfg;
		struct sh_mobile_meram_info *mdev;
		unsigned long icb_addr_y, icb_addr_c;
		int ret;

		cfg = ch->cfg.meram_cfg;
		mdev = priv->meram_dev;
		ret = mdev->ops->meram_update(mdev, cfg,
					base_addr_y, base_addr_c,
					&icb_addr_y, &icb_addr_c);
		if (ret)
			return ret;

		lcdc_write_chan_mirror(ch, LDSA1R, icb_addr_y);
		if (icb_addr_c)
			lcdc_write_chan_mirror(ch, LDSA2R, icb_addr_c);

	}
=======
	if (ch->format->yuv) {
		/* Set y offset */
		c_offset = var->yoffset * ch->pitch
			 * (ch->format->bpp - 8) / 8;
		base_addr_c = ch->dma_handle + ch->xres * ch->yres_virtual
			    + c_offset;
		/* Set x offset */
		if (ch->format->fourcc == V4L2_PIX_FMT_NV24)
			base_addr_c += 2 * var->xoffset;
		else
			base_addr_c += var->xoffset;
	}

	if (ch->meram) {
		struct sh_mobile_meram_info *mdev;

		mdev = priv->meram_dev;
		mdev->ops->meram_update(mdev, ch->meram,
					base_addr_y, base_addr_c,
					&base_addr_y, &base_addr_c);
	}

	ch->base_addr_y = base_addr_y;
	ch->base_addr_c = base_addr_c;
=======
	unsigned long base_addr_y, base_addr_c;
	unsigned long y_offset;
	unsigned long c_offset;

	if (!ch->format->yuv) {
		y_offset = (var->yoffset * ch->xres_virtual + var->xoffset)
			 * ch->format->bpp / 8;
		c_offset = 0;
	} else {
		unsigned int xsub = ch->format->bpp < 24 ? 2 : 1;
		unsigned int ysub = ch->format->bpp < 16 ? 2 : 1;

		y_offset = var->yoffset * ch->xres_virtual + var->xoffset;
		c_offset = var->yoffset / ysub * ch->xres_virtual * 2 / xsub
			 + var->xoffset * 2 / xsub;
	}

	/* If the Y offset hasn't changed, the C offset hasn't either. There's
	 * nothing to do in that case.
	 */
	if (y_offset == ch->pan_y_offset)
		return 0;

	/* Set the source address for the next refresh */
	base_addr_y = ch->dma_handle + y_offset;
	base_addr_c = ch->dma_handle + ch->xres_virtual * ch->yres_virtual
		    + c_offset;

	if (ch->cache)
		sh_mobile_meram_cache_update(priv->meram_dev, ch->cache,
					     base_addr_y, base_addr_c,
					     &base_addr_y, &base_addr_c);

	ch->base_addr_y = base_addr_y;
	ch->base_addr_c = base_addr_c;
	ch->pan_y_offset = y_offset;
>>>>>>> refs/remotes/origin/master

	lcdc_write_chan_mirror(ch, LDSA1R, base_addr_y);
	if (ch->format->yuv)
		lcdc_write_chan_mirror(ch, LDSA2R, base_addr_c);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

=======

	ldrcntr = lcdc_read(priv, _LDRCNTR);
>>>>>>> refs/remotes/origin/master
	if (lcdc_chan_is_sublcd(ch))
		lcdc_write(ch->lcdc, _LDRCNTR, ldrcntr ^ LDRCNTR_SRS);
	else
		lcdc_write(ch->lcdc, _LDRCNTR, ldrcntr ^ LDRCNTR_MRS);

<<<<<<< HEAD
	ch->pan_offset = new_pan_offset;
=======
>>>>>>> refs/remotes/origin/master

	sh_mobile_lcdc_deferred_io_touch(info);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int sh_mobile_wait_for_vsync(struct fb_info *info)
{
	struct sh_mobile_lcdc_chan *ch = info->par;
	unsigned long ldintr;
	int ret;

	/* Enable VSync End interrupt */
	ldintr = lcdc_read(ch->lcdc, _LDINTR);
	ldintr |= LDINTR_VEE;
	lcdc_write(ch->lcdc, _LDINTR, ldintr);

	ret = wait_for_completion_interruptible_timeout(&ch->vsync_completion,
							msecs_to_jiffies(100));
	if (!ret)
		return -ETIMEDOUT;

	return 0;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static int sh_mobile_ioctl(struct fb_info *info, unsigned int cmd,
		       unsigned long arg)
{
=======
static int sh_mobile_lcdc_ioctl(struct fb_info *info, unsigned int cmd,
				unsigned long arg)
{
	struct sh_mobile_lcdc_chan *ch = info->par;
>>>>>>> refs/remotes/origin/master
	int retval;

	switch (cmd) {
	case FBIO_WAITFORVSYNC:
<<<<<<< HEAD
<<<<<<< HEAD
		retval = sh_mobile_wait_for_vsync(info);
=======
		retval = sh_mobile_wait_for_vsync(info->par);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		retval = sh_mobile_lcdc_wait_for_vsync(ch);
>>>>>>> refs/remotes/origin/master
		break;

	default:
		retval = -ENOIOCTLCMD;
		break;
	}
	return retval;
}

static void sh_mobile_fb_reconfig(struct fb_info *info)
{
	struct sh_mobile_lcdc_chan *ch = info->par;
<<<<<<< HEAD
<<<<<<< HEAD
	struct fb_videomode mode1, mode2;
=======
	struct fb_var_screeninfo var;
	struct fb_videomode mode;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct fb_var_screeninfo var;
	struct fb_videomode mode;
>>>>>>> refs/remotes/origin/master
	struct fb_event event;
	int evnt = FB_EVENT_MODE_CHANGE_ALL;

	if (ch->use_count > 1 || (ch->use_count == 1 && !info->fbcon_par))
		/* More framebuffer users are active */
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	fb_var_to_videomode(&mode1, &ch->display_var);
	fb_var_to_videomode(&mode2, &info->var);

	if (fb_mode_is_equal(&mode1, &mode2))
		return;

	/* Display has been re-plugged, framebuffer is free now, reconfigure */
	if (fb_set_var(info, &ch->display_var) < 0)
		/* Couldn't reconfigure, hopefully, can continue as before */
		return;

	if (info->var.nonstd)
		info->fix.line_length = mode1.xres;
	else
		info->fix.line_length = mode1.xres * (ch->cfg.bpp / 8);

=======
=======
>>>>>>> refs/remotes/origin/master
	fb_var_to_videomode(&mode, &info->var);

	if (fb_mode_is_equal(&ch->display.mode, &mode))
		return;

	/* Display has been re-plugged, framebuffer is free now, reconfigure */
	var = info->var;
	fb_videomode_to_var(&var, &ch->display.mode);
	var.width = ch->display.width;
	var.height = ch->display.height;
	var.activate = FB_ACTIVATE_NOW;

	if (fb_set_var(info, &var) < 0)
		/* Couldn't reconfigure, hopefully, can continue as before */
		return;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * fb_set_var() calls the notifier change internally, only if
	 * FBINFO_MISC_USEREVENT flag is set. Since we do not want to fake a
	 * user event, we have to call the chain ourselves.
	 */
	event.info = info;
<<<<<<< HEAD
<<<<<<< HEAD
	event.data = &mode1;
=======
	event.data = &ch->display.mode;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	event.data = &ch->display.mode;
>>>>>>> refs/remotes/origin/master
	fb_notifier_call_chain(evnt, &event);
}

/*
 * Locking: both .fb_release() and .fb_open() are called with info->lock held if
 * user == 1, or with console sem held, if user == 0.
 */
<<<<<<< HEAD
static int sh_mobile_release(struct fb_info *info, int user)
=======
static int sh_mobile_lcdc_release(struct fb_info *info, int user)
>>>>>>> refs/remotes/origin/master
{
	struct sh_mobile_lcdc_chan *ch = info->par;

	mutex_lock(&ch->open_lock);
	dev_dbg(info->dev, "%s(): %d users\n", __func__, ch->use_count);

	ch->use_count--;

	/* Nothing to reconfigure, when called from fbcon */
	if (user) {
		console_lock();
		sh_mobile_fb_reconfig(info);
		console_unlock();
	}

	mutex_unlock(&ch->open_lock);

	return 0;
}

<<<<<<< HEAD
static int sh_mobile_open(struct fb_info *info, int user)
=======
static int sh_mobile_lcdc_open(struct fb_info *info, int user)
>>>>>>> refs/remotes/origin/master
{
	struct sh_mobile_lcdc_chan *ch = info->par;

	mutex_lock(&ch->open_lock);
	ch->use_count++;

	dev_dbg(info->dev, "%s(): %d users\n", __func__, ch->use_count);
	mutex_unlock(&ch->open_lock);

	return 0;
}

<<<<<<< HEAD
static int sh_mobile_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
	struct sh_mobile_lcdc_chan *ch = info->par;
	struct sh_mobile_lcdc_priv *p = ch->lcdc;
<<<<<<< HEAD

	if (var->xres > MAX_XRES || var->yres > MAX_YRES ||
	    var->xres * var->yres * (ch->cfg.bpp / 8) * 2 > info->fix.smem_len) {
		dev_warn(info->dev, "Invalid info: %u-%u-%u-%u x %u-%u-%u-%u @ %lukHz!\n",
			 var->left_margin, var->xres, var->right_margin, var->hsync_len,
			 var->upper_margin, var->yres, var->lower_margin, var->vsync_len,
			 PICOS2KHZ(var->pixclock));
		return -EINVAL;
	}

	/* only accept the forced_bpp for dual channel configurations */
	if (p->forced_bpp && p->forced_bpp != var->bits_per_pixel)
		return -EINVAL;

	switch (var->bits_per_pixel) {
	case 16: /* PKF[4:0] = 00011 - RGB 565 */
	case 24: /* PKF[4:0] = 01011 - RGB 888 */
	case 32: /* PKF[4:0] = 00000 - RGBA 888 */
		break;
	default:
		return -EINVAL;
	}
=======
=======
static int sh_mobile_lcdc_check_var(struct fb_var_screeninfo *var,
				    struct fb_info *info)
{
	struct sh_mobile_lcdc_chan *ch = info->par;
	struct sh_mobile_lcdc_priv *p = ch->lcdc;
>>>>>>> refs/remotes/origin/master
	unsigned int best_dist = (unsigned int)-1;
	unsigned int best_xres = 0;
	unsigned int best_yres = 0;
	unsigned int i;
<<<<<<< HEAD

	if (var->xres > MAX_XRES || var->yres > MAX_YRES)
		return -EINVAL;
=======
	int ret;
>>>>>>> refs/remotes/origin/master

	/* If board code provides us with a list of available modes, make sure
	 * we use one of them. Find the mode closest to the requested one. The
	 * distance between two modes is defined as the size of the
	 * non-overlapping parts of the two rectangles.
	 */
	for (i = 0; i < ch->cfg->num_modes; ++i) {
		const struct fb_videomode *mode = &ch->cfg->lcd_modes[i];
		unsigned int dist;

		/* We can only round up. */
		if (var->xres > mode->xres || var->yres > mode->yres)
			continue;

		dist = var->xres * var->yres + mode->xres * mode->yres
		     - 2 * min(var->xres, mode->xres)
			 * min(var->yres, mode->yres);

<<<<<<< HEAD
		if (dist < best_dist) {
			best_xres = mode->xres;
			best_yres = mode->yres;
			best_dist = dist;
		}
	}

	/* If no available mode can be used, return an error. */
	if (ch->cfg->num_modes != 0) {
		if (best_dist == (unsigned int)-1)
			return -EINVAL;

		var->xres = best_xres;
		var->yres = best_yres;
	}

	/* Make sure the virtual resolution is at least as big as the visible
	 * resolution.
	 */
	if (var->xres_virtual < var->xres)
		var->xres_virtual = var->xres;
	if (var->yres_virtual < var->yres)
		var->yres_virtual = var->yres;

	if (sh_mobile_format_is_fourcc(var)) {
		const struct sh_mobile_lcdc_format_info *format;

		format = sh_mobile_format_info(var->grayscale);
		if (format == NULL)
			return -EINVAL;
		var->bits_per_pixel = format->bpp;

		/* Default to RGB and JPEG color-spaces for RGB and YUV formats
		 * respectively.
		 */
		if (!format->yuv)
			var->colorspace = V4L2_COLORSPACE_SRGB;
		else if (var->colorspace != V4L2_COLORSPACE_REC709)
			var->colorspace = V4L2_COLORSPACE_JPEG;
	} else {
		if (var->bits_per_pixel <= 16) {		/* RGB 565 */
			var->bits_per_pixel = 16;
			var->red.offset = 11;
			var->red.length = 5;
			var->green.offset = 5;
			var->green.length = 6;
			var->blue.offset = 0;
			var->blue.length = 5;
			var->transp.offset = 0;
			var->transp.length = 0;
		} else if (var->bits_per_pixel <= 24) {		/* RGB 888 */
			var->bits_per_pixel = 24;
			var->red.offset = 16;
			var->red.length = 8;
			var->green.offset = 8;
			var->green.length = 8;
			var->blue.offset = 0;
			var->blue.length = 8;
			var->transp.offset = 0;
			var->transp.length = 0;
		} else if (var->bits_per_pixel <= 32) {		/* RGBA 888 */
			var->bits_per_pixel = 32;
			var->red.offset = 16;
			var->red.length = 8;
			var->green.offset = 8;
			var->green.length = 8;
			var->blue.offset = 0;
			var->blue.length = 8;
			var->transp.offset = 24;
			var->transp.length = 8;
		} else
			return -EINVAL;

		var->red.msb_right = 0;
		var->green.msb_right = 0;
		var->blue.msb_right = 0;
		var->transp.msb_right = 0;
	}

	/* Make sure we don't exceed our allocated memory. */
	if (var->xres_virtual * var->yres_virtual * var->bits_per_pixel / 8 >
	    info->fix.smem_len)
		return -EINVAL;
=======
		if (dist < best_dist) {
			best_xres = mode->xres;
			best_yres = mode->yres;
			best_dist = dist;
		}
	}

	/* If no available mode can be used, return an error. */
	if (ch->cfg->num_modes != 0) {
		if (best_dist == (unsigned int)-1)
			return -EINVAL;

		var->xres = best_xres;
		var->yres = best_yres;
	}

	ret = __sh_mobile_lcdc_check_var(var, info);
	if (ret < 0)
		return ret;
>>>>>>> refs/remotes/origin/master

	/* only accept the forced_fourcc for dual channel configurations */
	if (p->forced_fourcc &&
	    p->forced_fourcc != sh_mobile_format_fourcc(var))
		return -EINVAL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static int sh_mobile_set_par(struct fb_info *info)
=======
static int sh_mobile_lcdc_set_par(struct fb_info *info)
>>>>>>> refs/remotes/origin/master
{
	struct sh_mobile_lcdc_chan *ch = info->par;
	int ret;

	sh_mobile_lcdc_stop(ch->lcdc);

	ch->format = sh_mobile_format_info(sh_mobile_format_fourcc(&info->var));
	ch->colorspace = info->var.colorspace;

	ch->xres = info->var.xres;
	ch->xres_virtual = info->var.xres_virtual;
	ch->yres = info->var.yres;
	ch->yres_virtual = info->var.yres_virtual;

	if (ch->format->yuv)
<<<<<<< HEAD
		ch->pitch = info->var.xres;
	else
		ch->pitch = info->var.xres * ch->format->bpp / 8;
=======
		ch->pitch = info->var.xres_virtual;
	else
		ch->pitch = info->var.xres_virtual * ch->format->bpp / 8;
>>>>>>> refs/remotes/origin/master

	ret = sh_mobile_lcdc_start(ch->lcdc);
	if (ret < 0)
		dev_err(info->dev, "%s: unable to restart LCDC\n", __func__);

	info->fix.line_length = ch->pitch;

	if (sh_mobile_format_is_fourcc(&info->var)) {
		info->fix.type = FB_TYPE_FOURCC;
		info->fix.visual = FB_VISUAL_FOURCC;
	} else {
		info->fix.type = FB_TYPE_PACKED_PIXELS;
		info->fix.visual = FB_VISUAL_TRUECOLOR;
	}

	return ret;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Screen blanking. Behavior is as follows:
 * FB_BLANK_UNBLANK: screen unblanked, clocks enabled
 * FB_BLANK_NORMAL: screen blanked, clocks enabled
 * FB_BLANK_VSYNC,
 * FB_BLANK_HSYNC,
 * FB_BLANK_POWEROFF: screen blanked, clocks disabled
 */
static int sh_mobile_lcdc_blank(int blank, struct fb_info *info)
{
	struct sh_mobile_lcdc_chan *ch = info->par;
	struct sh_mobile_lcdc_priv *p = ch->lcdc;

	/* blank the screen? */
	if (blank > FB_BLANK_UNBLANK && ch->blank_status == FB_BLANK_UNBLANK) {
		struct fb_fillrect rect = {
<<<<<<< HEAD
<<<<<<< HEAD
			.width = info->var.xres,
			.height = info->var.yres,
=======
			.width = ch->xres,
			.height = ch->yres,
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.width = ch->xres,
			.height = ch->yres,
>>>>>>> refs/remotes/origin/master
		};
		sh_mobile_lcdc_fillrect(info, &rect);
	}
	/* turn clocks on? */
	if (blank <= FB_BLANK_NORMAL && ch->blank_status > FB_BLANK_NORMAL) {
		sh_mobile_lcdc_clk_on(p);
	}
	/* turn clocks off? */
	if (blank > FB_BLANK_NORMAL && ch->blank_status <= FB_BLANK_NORMAL) {
		/* make sure the screen is updated with the black fill before
		 * switching the clocks off. one vsync is not enough since
		 * blanking may occur in the middle of a refresh. deferred io
		 * mode will reenable the clocks and update the screen in time,
		 * so it does not need this. */
		if (!info->fbdefio) {
<<<<<<< HEAD
<<<<<<< HEAD
			sh_mobile_wait_for_vsync(info);
			sh_mobile_wait_for_vsync(info);
=======
			sh_mobile_wait_for_vsync(ch);
			sh_mobile_wait_for_vsync(ch);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			sh_mobile_lcdc_wait_for_vsync(ch);
			sh_mobile_lcdc_wait_for_vsync(ch);
>>>>>>> refs/remotes/origin/master
		}
		sh_mobile_lcdc_clk_off(p);
	}

	ch->blank_status = blank;
	return 0;
}

<<<<<<< HEAD
=======
static int
sh_mobile_lcdc_mmap(struct fb_info *info, struct vm_area_struct *vma)
{
	struct sh_mobile_lcdc_chan *ch = info->par;

	return dma_mmap_coherent(ch->lcdc->dev, vma, ch->fb_mem,
				 ch->dma_handle, ch->fb_size);
}

>>>>>>> refs/remotes/origin/master
static struct fb_ops sh_mobile_lcdc_ops = {
	.owner          = THIS_MODULE,
	.fb_setcolreg	= sh_mobile_lcdc_setcolreg,
	.fb_read        = fb_sys_read,
	.fb_write       = fb_sys_write,
	.fb_fillrect	= sh_mobile_lcdc_fillrect,
	.fb_copyarea	= sh_mobile_lcdc_copyarea,
	.fb_imageblit	= sh_mobile_lcdc_imageblit,
	.fb_blank	= sh_mobile_lcdc_blank,
<<<<<<< HEAD
	.fb_pan_display = sh_mobile_fb_pan_display,
	.fb_ioctl       = sh_mobile_ioctl,
	.fb_open	= sh_mobile_open,
	.fb_release	= sh_mobile_release,
	.fb_check_var	= sh_mobile_check_var,
<<<<<<< HEAD
};

static int sh_mobile_lcdc_update_bl(struct backlight_device *bdev)
{
	struct sh_mobile_lcdc_chan *ch = bl_get_data(bdev);
	struct sh_mobile_lcdc_board_cfg *cfg = &ch->cfg.board_cfg;
=======
	.fb_set_par	= sh_mobile_set_par,
=======
	.fb_pan_display = sh_mobile_lcdc_pan,
	.fb_ioctl       = sh_mobile_lcdc_ioctl,
	.fb_open	= sh_mobile_lcdc_open,
	.fb_release	= sh_mobile_lcdc_release,
	.fb_check_var	= sh_mobile_lcdc_check_var,
	.fb_set_par	= sh_mobile_lcdc_set_par,
	.fb_mmap	= sh_mobile_lcdc_mmap,
>>>>>>> refs/remotes/origin/master
};

static void
sh_mobile_lcdc_channel_fb_unregister(struct sh_mobile_lcdc_chan *ch)
{
	if (ch->info && ch->info->dev)
		unregister_framebuffer(ch->info);
}

<<<<<<< HEAD
static int __devinit
=======
static int
>>>>>>> refs/remotes/origin/master
sh_mobile_lcdc_channel_fb_register(struct sh_mobile_lcdc_chan *ch)
{
	struct fb_info *info = ch->info;
	int ret;

	if (info->fbdefio) {
		ch->sglist = vmalloc(sizeof(struct scatterlist) *
				     ch->fb_size >> PAGE_SHIFT);
		if (!ch->sglist) {
			dev_err(ch->lcdc->dev, "cannot allocate sglist\n");
			return -ENOMEM;
		}
	}

	info->bl_dev = ch->bl;

	ret = register_framebuffer(info);
	if (ret < 0)
		return ret;

	dev_info(ch->lcdc->dev, "registered %s/%s as %dx%d %dbpp.\n",
		 dev_name(ch->lcdc->dev), (ch->cfg->chan == LCDC_CHAN_MAINLCD) ?
		 "mainlcd" : "sublcd", info->var.xres, info->var.yres,
		 info->var.bits_per_pixel);

	/* deferred io mode: disable clock to save power */
	if (info->fbdefio || info->state == FBINFO_STATE_SUSPENDED)
		sh_mobile_lcdc_clk_off(ch->lcdc);

	return ret;
}

static void
sh_mobile_lcdc_channel_fb_cleanup(struct sh_mobile_lcdc_chan *ch)
{
	struct fb_info *info = ch->info;

	if (!info || !info->device)
		return;

	if (ch->sglist)
		vfree(ch->sglist);

	fb_dealloc_cmap(&info->cmap);
	framebuffer_release(info);
}

<<<<<<< HEAD
static int __devinit
sh_mobile_lcdc_channel_fb_init(struct sh_mobile_lcdc_chan *ch,
			       const struct fb_videomode *mode,
=======
static int
sh_mobile_lcdc_channel_fb_init(struct sh_mobile_lcdc_chan *ch,
			       const struct fb_videomode *modes,
>>>>>>> refs/remotes/origin/master
			       unsigned int num_modes)
{
	struct sh_mobile_lcdc_priv *priv = ch->lcdc;
	struct fb_var_screeninfo *var;
	struct fb_info *info;
	int ret;

	/* Allocate and initialize the frame buffer device. Create the modes
	 * list and allocate the color map.
	 */
	info = framebuffer_alloc(0, priv->dev);
	if (info == NULL) {
		dev_err(priv->dev, "unable to allocate fb_info\n");
		return -ENOMEM;
	}

	ch->info = info;

	info->flags = FBINFO_FLAG_DEFAULT;
	info->fbops = &sh_mobile_lcdc_ops;
	info->device = priv->dev;
	info->screen_base = ch->fb_mem;
	info->pseudo_palette = &ch->pseudo_palette;
	info->par = ch;

<<<<<<< HEAD
	fb_videomode_to_modelist(mode, num_modes, &info->modelist);
=======
	fb_videomode_to_modelist(modes, num_modes, &info->modelist);
>>>>>>> refs/remotes/origin/master

	ret = fb_alloc_cmap(&info->cmap, PALETTE_NR, 0);
	if (ret < 0) {
		dev_err(priv->dev, "unable to allocate cmap\n");
		return ret;
	}

	/* Initialize fixed screen information. Restrict pan to 2 lines steps
	 * for NV12 and NV21.
	 */
	info->fix = sh_mobile_lcdc_fix;
	info->fix.smem_start = ch->dma_handle;
	info->fix.smem_len = ch->fb_size;
	info->fix.line_length = ch->pitch;

	if (ch->format->yuv)
		info->fix.visual = FB_VISUAL_FOURCC;
	else
		info->fix.visual = FB_VISUAL_TRUECOLOR;

<<<<<<< HEAD
	if (ch->format->fourcc == V4L2_PIX_FMT_NV12 ||
	    ch->format->fourcc == V4L2_PIX_FMT_NV21)
		info->fix.ypanstep = 2;

	/* Initialize variable screen information using the first mode as
	 * default. The default Y virtual resolution is twice the panel size to
	 * allow for double-buffering.
	 */
	var = &info->var;
	fb_videomode_to_var(var, mode);
	var->width = ch->cfg->panel_cfg.width;
	var->height = ch->cfg->panel_cfg.height;
	var->yres_virtual = var->yres * 2;
=======
	switch (ch->format->fourcc) {
	case V4L2_PIX_FMT_NV12:
	case V4L2_PIX_FMT_NV21:
		info->fix.ypanstep = 2;
	case V4L2_PIX_FMT_NV16:
	case V4L2_PIX_FMT_NV61:
		info->fix.xpanstep = 2;
	}

	/* Initialize variable screen information using the first mode as
	 * default.
	 */
	var = &info->var;
	fb_videomode_to_var(var, modes);
	var->width = ch->display.width;
	var->height = ch->display.height;
	var->xres_virtual = ch->xres_virtual;
	var->yres_virtual = ch->yres_virtual;
>>>>>>> refs/remotes/origin/master
	var->activate = FB_ACTIVATE_NOW;

	/* Use the legacy API by default for RGB formats, and the FOURCC API
	 * for YUV formats.
	 */
	if (!ch->format->yuv)
		var->bits_per_pixel = ch->format->bpp;
	else
		var->grayscale = ch->format->fourcc;

<<<<<<< HEAD
	ret = sh_mobile_check_var(var, info);
=======
	ret = sh_mobile_lcdc_check_var(var, info);
>>>>>>> refs/remotes/origin/master
	if (ret)
		return ret;

	return 0;
}

/* -----------------------------------------------------------------------------
 * Backlight
 */

static int sh_mobile_lcdc_update_bl(struct backlight_device *bdev)
{
	struct sh_mobile_lcdc_chan *ch = bl_get_data(bdev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	int brightness = bdev->props.brightness;

	if (bdev->props.power != FB_BLANK_UNBLANK ||
	    bdev->props.state & (BL_CORE_SUSPENDED | BL_CORE_FBBLANK))
		brightness = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	return cfg->set_brightness(cfg->board_data, brightness);
=======
	return ch->cfg->bl_info.set_brightness(brightness);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ch->bl_brightness = brightness;
	return ch->cfg->bl_info.set_brightness(brightness);
>>>>>>> refs/remotes/origin/master
}

static int sh_mobile_lcdc_get_brightness(struct backlight_device *bdev)
{
	struct sh_mobile_lcdc_chan *ch = bl_get_data(bdev);
<<<<<<< HEAD
<<<<<<< HEAD
	struct sh_mobile_lcdc_board_cfg *cfg = &ch->cfg.board_cfg;

	return cfg->get_brightness(cfg->board_data);
=======

	return ch->cfg->bl_info.get_brightness();
>>>>>>> refs/remotes/origin/cm-10.0
=======

	return ch->bl_brightness;
>>>>>>> refs/remotes/origin/master
}

static int sh_mobile_lcdc_check_fb(struct backlight_device *bdev,
				   struct fb_info *info)
{
	return (info->bl_dev == bdev);
}

static struct backlight_ops sh_mobile_lcdc_bl_ops = {
	.options	= BL_CORE_SUSPENDRESUME,
	.update_status	= sh_mobile_lcdc_update_bl,
	.get_brightness	= sh_mobile_lcdc_get_brightness,
	.check_fb	= sh_mobile_lcdc_check_fb,
};

static struct backlight_device *sh_mobile_lcdc_bl_probe(struct device *parent,
					       struct sh_mobile_lcdc_chan *ch)
{
	struct backlight_device *bl;

<<<<<<< HEAD
<<<<<<< HEAD
	bl = backlight_device_register(ch->cfg.bl_info.name, parent, ch,
=======
	bl = backlight_device_register(ch->cfg->bl_info.name, parent, ch,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bl = backlight_device_register(ch->cfg->bl_info.name, parent, ch,
>>>>>>> refs/remotes/origin/master
				       &sh_mobile_lcdc_bl_ops, NULL);
	if (IS_ERR(bl)) {
		dev_err(parent, "unable to register backlight device: %ld\n",
			PTR_ERR(bl));
		return NULL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	bl->props.max_brightness = ch->cfg.bl_info.max_brightness;
=======
	bl->props.max_brightness = ch->cfg->bl_info.max_brightness;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bl->props.max_brightness = ch->cfg->bl_info.max_brightness;
>>>>>>> refs/remotes/origin/master
	bl->props.brightness = bl->props.max_brightness;
	backlight_update_status(bl);

	return bl;
}

static void sh_mobile_lcdc_bl_remove(struct backlight_device *bdev)
{
	backlight_device_unregister(bdev);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int sh_mobile_lcdc_set_bpp(struct fb_var_screeninfo *var, int bpp,
				   int nonstd)
{
	if (nonstd) {
		switch (bpp) {
		case 12:
		case 16:
		case 24:
			var->bits_per_pixel = bpp;
			var->nonstd = nonstd;
			return 0;
		default:
			return -EINVAL;
		}
	}

	switch (bpp) {
	case 16: /* PKF[4:0] = 00011 - RGB 565 */
		var->red.offset = 11;
		var->red.length = 5;
		var->green.offset = 5;
		var->green.length = 6;
		var->blue.offset = 0;
		var->blue.length = 5;
		var->transp.offset = 0;
		var->transp.length = 0;
		break;

	case 24: /* PKF[4:0] = 01011 - RGB 888 */
		var->red.offset = 16;
		var->red.length = 8;
		var->green.offset = 8;
		var->green.length = 8;
		var->blue.offset = 0;
		var->blue.length = 8;
		var->transp.offset = 0;
		var->transp.length = 0;
		break;

	case 32: /* PKF[4:0] = 00000 - RGBA 888 */
		var->red.offset = 16;
		var->red.length = 8;
		var->green.offset = 8;
		var->green.length = 8;
		var->blue.offset = 0;
		var->blue.length = 8;
		var->transp.offset = 24;
		var->transp.length = 8;
		break;
	default:
		return -EINVAL;
	}
	var->bits_per_pixel = bpp;
	var->red.msb_right = 0;
	var->green.msb_right = 0;
	var->blue.msb_right = 0;
	var->transp.msb_right = 0;
	return 0;
}
=======
/* -----------------------------------------------------------------------------
 * Power management
 */
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* -----------------------------------------------------------------------------
 * Power management
 */
>>>>>>> refs/remotes/origin/master

static int sh_mobile_lcdc_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);

	sh_mobile_lcdc_stop(platform_get_drvdata(pdev));
	return 0;
}

static int sh_mobile_lcdc_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);

	return sh_mobile_lcdc_start(platform_get_drvdata(pdev));
}

static int sh_mobile_lcdc_runtime_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	struct sh_mobile_lcdc_priv *p = platform_get_drvdata(pdev);
	struct sh_mobile_lcdc_chan *ch;
	int k, n;

	/* save per-channel registers */
	for (k = 0; k < ARRAY_SIZE(p->ch); k++) {
		ch = &p->ch[k];
		if (!ch->enabled)
			continue;
		for (n = 0; n < NR_CH_REGS; n++)
			ch->saved_ch_regs[n] = lcdc_read_chan(ch, n);
	}

	/* save shared registers */
	for (n = 0; n < NR_SHARED_REGS; n++)
		p->saved_shared_regs[n] = lcdc_read(p, lcdc_shared_regs[n]);

	/* turn off LCDC hardware */
	lcdc_write(p, _LDCNT1R, 0);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct sh_mobile_lcdc_priv *priv = platform_get_drvdata(pdev);

	/* turn off LCDC hardware */
	lcdc_write(priv, _LDCNT1R, 0);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int sh_mobile_lcdc_runtime_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	struct sh_mobile_lcdc_priv *p = platform_get_drvdata(pdev);
	struct sh_mobile_lcdc_chan *ch;
	int k, n;

	/* restore per-channel registers */
	for (k = 0; k < ARRAY_SIZE(p->ch); k++) {
		ch = &p->ch[k];
		if (!ch->enabled)
			continue;
		for (n = 0; n < NR_CH_REGS; n++)
			lcdc_write_chan(ch, n, ch->saved_ch_regs[n]);
	}

	/* restore shared registers */
	for (n = 0; n < NR_SHARED_REGS; n++)
		lcdc_write(p, lcdc_shared_regs[n], p->saved_shared_regs[n]);
=======
	struct sh_mobile_lcdc_priv *priv = platform_get_drvdata(pdev);

	__sh_mobile_lcdc_start(priv);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct sh_mobile_lcdc_priv *priv = platform_get_drvdata(pdev);

	__sh_mobile_lcdc_start(priv);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static const struct dev_pm_ops sh_mobile_lcdc_dev_pm_ops = {
	.suspend = sh_mobile_lcdc_suspend,
	.resume = sh_mobile_lcdc_resume,
	.runtime_suspend = sh_mobile_lcdc_runtime_suspend,
	.runtime_resume = sh_mobile_lcdc_runtime_resume,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/* -----------------------------------------------------------------------------
 * Framebuffer notifier
 */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* locking: called with info->lock held */
static int sh_mobile_lcdc_notify(struct notifier_block *nb,
				 unsigned long action, void *data)
{
	struct fb_event *event = data;
	struct fb_info *info = event->info;
	struct sh_mobile_lcdc_chan *ch = info->par;
<<<<<<< HEAD
<<<<<<< HEAD
	struct sh_mobile_lcdc_board_cfg	*board_cfg = &ch->cfg.board_cfg;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (&ch->lcdc->notifier != nb)
		return NOTIFY_DONE;

	dev_dbg(info->dev, "%s(): action = %lu, data = %p\n",
		__func__, action, event->data);

	switch(action) {
	case FB_EVENT_SUSPEND:
<<<<<<< HEAD
<<<<<<< HEAD
		if (board_cfg->display_off && try_module_get(board_cfg->owner)) {
			board_cfg->display_off(board_cfg->board_data);
			module_put(board_cfg->owner);
		}
=======
		sh_mobile_lcdc_display_off(ch);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		sh_mobile_lcdc_display_off(ch);
>>>>>>> refs/remotes/origin/master
		sh_mobile_lcdc_stop(ch->lcdc);
		break;
	case FB_EVENT_RESUME:
		mutex_lock(&ch->open_lock);
		sh_mobile_fb_reconfig(info);
		mutex_unlock(&ch->open_lock);

<<<<<<< HEAD
<<<<<<< HEAD
		/* HDMI must be enabled before LCDC configuration */
		if (board_cfg->display_on && try_module_get(board_cfg->owner)) {
			board_cfg->display_on(board_cfg->board_data, info);
			module_put(board_cfg->owner);
		}

=======
		sh_mobile_lcdc_display_on(ch);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		sh_mobile_lcdc_display_on(ch);
>>>>>>> refs/remotes/origin/master
		sh_mobile_lcdc_start(ch->lcdc);
	}

	return NOTIFY_OK;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int sh_mobile_lcdc_remove(struct platform_device *pdev);

static int __devinit sh_mobile_lcdc_probe(struct platform_device *pdev)
{
	struct fb_info *info;
	struct sh_mobile_lcdc_priv *priv;
	struct sh_mobile_lcdc_info *pdata = pdev->dev.platform_data;
	struct resource *res;
	int error;
	void *buf;
	int i, j;
=======
=======
>>>>>>> refs/remotes/origin/master
/* -----------------------------------------------------------------------------
 * Probe/remove and driver init/exit
 */

<<<<<<< HEAD
static const struct fb_videomode default_720p __devinitconst = {
=======
static const struct fb_videomode default_720p = {
>>>>>>> refs/remotes/origin/master
	.name = "HDMI 720p",
	.xres = 1280,
	.yres = 720,

	.left_margin = 220,
	.right_margin = 110,
	.hsync_len = 40,

	.upper_margin = 20,
	.lower_margin = 5,
	.vsync_len = 5,

	.pixclock = 13468,
	.refresh = 60,
	.sync = FB_SYNC_VERT_HIGH_ACT | FB_SYNC_HOR_HIGH_ACT,
};

static int sh_mobile_lcdc_remove(struct platform_device *pdev)
{
	struct sh_mobile_lcdc_priv *priv = platform_get_drvdata(pdev);
<<<<<<< HEAD
	int i;

	fb_unregister_client(&priv->notifier);

=======
	unsigned int i;

	fb_unregister_client(&priv->notifier);

	for (i = 0; i < ARRAY_SIZE(priv->overlays); i++)
		sh_mobile_lcdc_overlay_fb_unregister(&priv->overlays[i]);
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < ARRAY_SIZE(priv->ch); i++)
		sh_mobile_lcdc_channel_fb_unregister(&priv->ch[i]);

	sh_mobile_lcdc_stop(priv);

<<<<<<< HEAD
=======
	for (i = 0; i < ARRAY_SIZE(priv->overlays); i++) {
		struct sh_mobile_lcdc_overlay *ovl = &priv->overlays[i];

		sh_mobile_lcdc_overlay_fb_cleanup(ovl);

		if (ovl->fb_mem)
			dma_free_coherent(&pdev->dev, ovl->fb_size,
					  ovl->fb_mem, ovl->dma_handle);
	}

>>>>>>> refs/remotes/origin/master
	for (i = 0; i < ARRAY_SIZE(priv->ch); i++) {
		struct sh_mobile_lcdc_chan *ch = &priv->ch[i];

		if (ch->tx_dev) {
			ch->tx_dev->lcdc = NULL;
			module_put(ch->cfg->tx_dev->dev.driver->owner);
		}

		sh_mobile_lcdc_channel_fb_cleanup(ch);

		if (ch->fb_mem)
			dma_free_coherent(&pdev->dev, ch->fb_size,
					  ch->fb_mem, ch->dma_handle);
	}

	for (i = 0; i < ARRAY_SIZE(priv->ch); i++) {
<<<<<<< HEAD
		if (priv->ch[i].bl)
			sh_mobile_lcdc_bl_remove(priv->ch[i].bl);
=======
		struct sh_mobile_lcdc_chan *ch = &priv->ch[i];

		if (ch->bl)
			sh_mobile_lcdc_bl_remove(ch->bl);
		mutex_destroy(&ch->open_lock);
>>>>>>> refs/remotes/origin/master
	}

	if (priv->dot_clk) {
		pm_runtime_disable(&pdev->dev);
		clk_put(priv->dot_clk);
	}

	if (priv->base)
		iounmap(priv->base);

	if (priv->irq)
		free_irq(priv->irq, priv);
	kfree(priv);
	return 0;
}

<<<<<<< HEAD
static int __devinit sh_mobile_lcdc_check_interface(struct sh_mobile_lcdc_chan *ch)
=======
static int sh_mobile_lcdc_check_interface(struct sh_mobile_lcdc_chan *ch)
>>>>>>> refs/remotes/origin/master
{
	int interface_type = ch->cfg->interface_type;

	switch (interface_type) {
	case RGB8:
	case RGB9:
	case RGB12A:
	case RGB12B:
	case RGB16:
	case RGB18:
	case RGB24:
	case SYS8A:
	case SYS8B:
	case SYS8C:
	case SYS8D:
	case SYS9:
	case SYS12:
	case SYS16A:
	case SYS16B:
	case SYS16C:
	case SYS18:
	case SYS24:
		break;
	default:
		return -EINVAL;
	}

	/* SUBLCD only supports SYS interface */
	if (lcdc_chan_is_sublcd(ch)) {
		if (!(interface_type & LDMT1R_IFM))
			return -EINVAL;

		interface_type &= ~LDMT1R_IFM;
	}

	ch->ldmt1r_value = interface_type;
	return 0;
}

<<<<<<< HEAD
static int __devinit
sh_mobile_lcdc_channel_init(struct sh_mobile_lcdc_priv *priv,
			    struct sh_mobile_lcdc_chan *ch)
{
	const struct sh_mobile_lcdc_format_info *format;
	const struct sh_mobile_lcdc_chan_cfg *cfg = ch->cfg;
=======
static int
sh_mobile_lcdc_overlay_init(struct sh_mobile_lcdc_overlay *ovl)
{
	const struct sh_mobile_lcdc_format_info *format;
	struct device *dev = ovl->channel->lcdc->dev;
	int ret;

	if (ovl->cfg->fourcc == 0)
		return 0;

	/* Validate the format. */
	format = sh_mobile_format_info(ovl->cfg->fourcc);
	if (format == NULL) {
		dev_err(dev, "Invalid FOURCC %08x\n", ovl->cfg->fourcc);
		return -EINVAL;
	}

	ovl->enabled = false;
	ovl->mode = LCDC_OVERLAY_BLEND;
	ovl->alpha = 255;
	ovl->rop3 = 0;
	ovl->pos_x = 0;
	ovl->pos_y = 0;

	/* The default Y virtual resolution is twice the panel size to allow for
	 * double-buffering.
	 */
	ovl->format = format;
	ovl->xres = ovl->cfg->max_xres;
	ovl->xres_virtual = ovl->xres;
	ovl->yres = ovl->cfg->max_yres;
	ovl->yres_virtual = ovl->yres * 2;

	if (!format->yuv)
		ovl->pitch = ovl->xres_virtual * format->bpp / 8;
	else
		ovl->pitch = ovl->xres_virtual;

	/* Allocate frame buffer memory. */
	ovl->fb_size = ovl->cfg->max_xres * ovl->cfg->max_yres
		       * format->bpp / 8 * 2;
	ovl->fb_mem = dma_alloc_coherent(dev, ovl->fb_size, &ovl->dma_handle,
					 GFP_KERNEL);
	if (!ovl->fb_mem) {
		dev_err(dev, "unable to allocate buffer\n");
		return -ENOMEM;
	}

	ret = sh_mobile_lcdc_overlay_fb_init(ovl);
	if (ret < 0)
		return ret;

	return 0;
}

static int
sh_mobile_lcdc_channel_init(struct sh_mobile_lcdc_chan *ch)
{
	const struct sh_mobile_lcdc_format_info *format;
	const struct sh_mobile_lcdc_chan_cfg *cfg = ch->cfg;
	struct device *dev = ch->lcdc->dev;
>>>>>>> refs/remotes/origin/master
	const struct fb_videomode *max_mode;
	const struct fb_videomode *mode;
	unsigned int num_modes;
	unsigned int max_size;
	unsigned int i;

	mutex_init(&ch->open_lock);
	ch->notify = sh_mobile_lcdc_display_notify;

	/* Validate the format. */
	format = sh_mobile_format_info(cfg->fourcc);
	if (format == NULL) {
<<<<<<< HEAD
		dev_err(priv->dev, "Invalid FOURCC %08x.\n", cfg->fourcc);
=======
		dev_err(dev, "Invalid FOURCC %08x.\n", cfg->fourcc);
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}

	/* Iterate through the modes to validate them and find the highest
	 * resolution.
	 */
	max_mode = NULL;
	max_size = 0;

	for (i = 0, mode = cfg->lcd_modes; i < cfg->num_modes; i++, mode++) {
		unsigned int size = mode->yres * mode->xres;

		/* NV12/NV21 buffers must have even number of lines */
		if ((cfg->fourcc == V4L2_PIX_FMT_NV12 ||
		     cfg->fourcc == V4L2_PIX_FMT_NV21) && (mode->yres & 0x1)) {
<<<<<<< HEAD
			dev_err(priv->dev, "yres must be multiple of 2 for "
=======
			dev_err(dev, "yres must be multiple of 2 for "
>>>>>>> refs/remotes/origin/master
				"YCbCr420 mode.\n");
			return -EINVAL;
		}

		if (size > max_size) {
			max_mode = mode;
			max_size = size;
		}
	}

	if (!max_size)
		max_size = MAX_XRES * MAX_YRES;
	else
<<<<<<< HEAD
		dev_dbg(priv->dev, "Found largest videomode %ux%u\n",
=======
		dev_dbg(dev, "Found largest videomode %ux%u\n",
>>>>>>> refs/remotes/origin/master
			max_mode->xres, max_mode->yres);

	if (cfg->lcd_modes == NULL) {
		mode = &default_720p;
		num_modes = 1;
	} else {
		mode = cfg->lcd_modes;
		num_modes = cfg->num_modes;
	}

<<<<<<< HEAD
	/* Use the first mode as default. */
=======
	/* Use the first mode as default. The default Y virtual resolution is
	 * twice the panel size to allow for double-buffering.
	 */
>>>>>>> refs/remotes/origin/master
	ch->format = format;
	ch->xres = mode->xres;
	ch->xres_virtual = mode->xres;
	ch->yres = mode->yres;
	ch->yres_virtual = mode->yres * 2;

	if (!format->yuv) {
		ch->colorspace = V4L2_COLORSPACE_SRGB;
<<<<<<< HEAD
		ch->pitch = ch->xres * format->bpp / 8;
	} else {
		ch->colorspace = V4L2_COLORSPACE_REC709;
		ch->pitch = ch->xres;
=======
		ch->pitch = ch->xres_virtual * format->bpp / 8;
	} else {
		ch->colorspace = V4L2_COLORSPACE_REC709;
		ch->pitch = ch->xres_virtual;
>>>>>>> refs/remotes/origin/master
	}

	ch->display.width = cfg->panel_cfg.width;
	ch->display.height = cfg->panel_cfg.height;
	ch->display.mode = *mode;

	/* Allocate frame buffer memory. */
	ch->fb_size = max_size * format->bpp / 8 * 2;
<<<<<<< HEAD
	ch->fb_mem = dma_alloc_coherent(priv->dev, ch->fb_size, &ch->dma_handle,
					GFP_KERNEL);
	if (ch->fb_mem == NULL) {
		dev_err(priv->dev, "unable to allocate buffer\n");
=======
	ch->fb_mem = dma_alloc_coherent(dev, ch->fb_size, &ch->dma_handle,
					GFP_KERNEL);
	if (ch->fb_mem == NULL) {
		dev_err(dev, "unable to allocate buffer\n");
>>>>>>> refs/remotes/origin/master
		return -ENOMEM;
	}

	/* Initialize the transmitter device if present. */
	if (cfg->tx_dev) {
		if (!cfg->tx_dev->dev.driver ||
		    !try_module_get(cfg->tx_dev->dev.driver->owner)) {
<<<<<<< HEAD
			dev_warn(priv->dev,
				 "unable to get transmitter device\n");
=======
			dev_warn(dev, "unable to get transmitter device\n");
>>>>>>> refs/remotes/origin/master
			return -EINVAL;
		}
		ch->tx_dev = platform_get_drvdata(cfg->tx_dev);
		ch->tx_dev->lcdc = ch;
		ch->tx_dev->def_mode = *mode;
	}

	return sh_mobile_lcdc_channel_fb_init(ch, mode, num_modes);
}

<<<<<<< HEAD
static int __devinit sh_mobile_lcdc_probe(struct platform_device *pdev)
=======
static int sh_mobile_lcdc_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct sh_mobile_lcdc_info *pdata = pdev->dev.platform_data;
	struct sh_mobile_lcdc_priv *priv;
	struct resource *res;
	int num_channels;
	int error;
	int i;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (!pdata) {
		dev_err(&pdev->dev, "no platform data defined\n");
		return -EINVAL;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	i = platform_get_irq(pdev, 0);
	if (!res || i < 0) {
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

	error = request_irq(i, sh_mobile_lcdc_irq, IRQF_DISABLED,
=======
=======
>>>>>>> refs/remotes/origin/master
	priv->dev = &pdev->dev;
	priv->meram_dev = pdata->meram_dev;
	platform_set_drvdata(pdev, priv);

	error = request_irq(i, sh_mobile_lcdc_irq, 0,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			    dev_name(&pdev->dev), priv);
	if (error) {
		dev_err(&pdev->dev, "unable to request irq\n");
		goto err1;
	}

	priv->irq = i;
	atomic_set(&priv->hw_usecnt, -1);

<<<<<<< HEAD
<<<<<<< HEAD
	j = 0;
	for (i = 0; i < ARRAY_SIZE(pdata->ch); i++) {
		struct sh_mobile_lcdc_chan *ch = priv->ch + j;

		ch->lcdc = priv;
		memcpy(&ch->cfg, &pdata->ch[i], sizeof(pdata->ch[i]));
=======
=======
>>>>>>> refs/remotes/origin/master
	for (i = 0, num_channels = 0; i < ARRAY_SIZE(pdata->ch); i++) {
		struct sh_mobile_lcdc_chan *ch = priv->ch + num_channels;

		ch->lcdc = priv;
		ch->cfg = &pdata->ch[i];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		error = sh_mobile_lcdc_check_interface(ch);
		if (error) {
			dev_err(&pdev->dev, "unsupported interface type\n");
			goto err1;
		}
		init_waitqueue_head(&ch->frame_end_wait);
		init_completion(&ch->vsync_completion);
<<<<<<< HEAD
		ch->pan_offset = 0;

		/* probe the backlight is there is one defined */
<<<<<<< HEAD
		if (ch->cfg.bl_info.max_brightness)
=======
		if (ch->cfg->bl_info.max_brightness)
>>>>>>> refs/remotes/origin/cm-10.0
=======

		/* probe the backlight is there is one defined */
		if (ch->cfg->bl_info.max_brightness)
>>>>>>> refs/remotes/origin/master
			ch->bl = sh_mobile_lcdc_bl_probe(&pdev->dev, ch);

		switch (pdata->ch[i].chan) {
		case LCDC_CHAN_MAINLCD:
<<<<<<< HEAD
<<<<<<< HEAD
			ch->enabled = 1 << 1;
			ch->reg_offs = lcdc_offs_mainlcd;
			j++;
			break;
		case LCDC_CHAN_SUBLCD:
			ch->enabled = 1 << 2;
			ch->reg_offs = lcdc_offs_sublcd;
			j++;
=======
=======
>>>>>>> refs/remotes/origin/master
			ch->enabled = LDCNT2R_ME;
			ch->reg_offs = lcdc_offs_mainlcd;
			num_channels++;
			break;
		case LCDC_CHAN_SUBLCD:
			ch->enabled = LDCNT2R_SE;
			ch->reg_offs = lcdc_offs_sublcd;
			num_channels++;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			break;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (!j) {
=======
	if (!num_channels) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!num_channels) {
>>>>>>> refs/remotes/origin/master
		dev_err(&pdev->dev, "no channels defined\n");
		error = -EINVAL;
		goto err1;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/* for dual channel LCDC (MAIN + SUB) force shared bpp setting */
	if (j == 2)
		priv->forced_bpp = pdata->ch[0].bpp;
=======
	/* for dual channel LCDC (MAIN + SUB) force shared format setting */
	if (num_channels == 2)
		priv->forced_fourcc = pdata->ch[0].fourcc;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* for dual channel LCDC (MAIN + SUB) force shared format setting */
	if (num_channels == 2)
		priv->forced_fourcc = pdata->ch[0].fourcc;
>>>>>>> refs/remotes/origin/master

	priv->base = ioremap_nocache(res->start, resource_size(res));
	if (!priv->base)
		goto err1;

<<<<<<< HEAD
<<<<<<< HEAD
	error = sh_mobile_lcdc_setup_clocks(pdev, pdata->clock_source, priv);
=======
	error = sh_mobile_lcdc_setup_clocks(priv, pdata->clock_source);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	error = sh_mobile_lcdc_setup_clocks(priv, pdata->clock_source);
>>>>>>> refs/remotes/origin/master
	if (error) {
		dev_err(&pdev->dev, "unable to setup clocks\n");
		goto err1;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	priv->meram_dev = pdata->meram_dev;

	for (i = 0; i < j; i++) {
		struct fb_var_screeninfo *var;
		const struct fb_videomode *lcd_cfg, *max_cfg = NULL;
		struct sh_mobile_lcdc_chan *ch = priv->ch + i;
		struct sh_mobile_lcdc_chan_cfg *cfg = &ch->cfg;
		const struct fb_videomode *mode = cfg->lcd_cfg;
		unsigned long max_size = 0;
		int k;
		int num_cfg;

		ch->info = framebuffer_alloc(0, &pdev->dev);
		if (!ch->info) {
			dev_err(&pdev->dev, "unable to allocate fb_info\n");
			error = -ENOMEM;
			break;
		}

		info = ch->info;
		var = &info->var;
		info->fbops = &sh_mobile_lcdc_ops;
		info->par = ch;

		mutex_init(&ch->open_lock);

		for (k = 0, lcd_cfg = mode;
		     k < cfg->num_cfg && lcd_cfg;
		     k++, lcd_cfg++) {
			unsigned long size = lcd_cfg->yres * lcd_cfg->xres;
			/* NV12 buffers must have even number of lines */
			if ((cfg->nonstd) && cfg->bpp == 12 &&
					(lcd_cfg->yres & 0x1)) {
				dev_err(&pdev->dev, "yres must be multiple of 2"
						" for YCbCr420 mode.\n");
				error = -EINVAL;
				goto err1;
			}

			if (size > max_size) {
				max_cfg = lcd_cfg;
				max_size = size;
			}
		}

		if (!mode)
			max_size = MAX_XRES * MAX_YRES;
		else if (max_cfg)
			dev_dbg(&pdev->dev, "Found largest videomode %ux%u\n",
				max_cfg->xres, max_cfg->yres);

		info->fix = sh_mobile_lcdc_fix;
		info->fix.smem_len = max_size * 2 * cfg->bpp / 8;

		 /* Only pan in 2 line steps for NV12 */
		if (cfg->nonstd && cfg->bpp == 12)
			info->fix.ypanstep = 2;

		if (!mode) {
			mode = &default_720p;
			num_cfg = 1;
		} else {
			num_cfg = cfg->num_cfg;
		}

		fb_videomode_to_modelist(mode, num_cfg, &info->modelist);

		fb_videomode_to_var(var, mode);
		var->width = cfg->lcd_size_cfg.width;
		var->height = cfg->lcd_size_cfg.height;
		/* Default Y virtual resolution is 2x panel size */
		var->yres_virtual = var->yres * 2;
		var->activate = FB_ACTIVATE_NOW;

		error = sh_mobile_lcdc_set_bpp(var, cfg->bpp, cfg->nonstd);
		if (error)
			break;

		buf = dma_alloc_coherent(&pdev->dev, info->fix.smem_len,
					 &ch->dma_handle, GFP_KERNEL);
		if (!buf) {
			dev_err(&pdev->dev, "unable to allocate buffer\n");
			error = -ENOMEM;
			break;
		}

		info->pseudo_palette = &ch->pseudo_palette;
		info->flags = FBINFO_FLAG_DEFAULT;

		error = fb_alloc_cmap(&info->cmap, PALETTE_NR, 0);
		if (error < 0) {
			dev_err(&pdev->dev, "unable to allocate cmap\n");
			dma_free_coherent(&pdev->dev, info->fix.smem_len,
					  buf, ch->dma_handle);
			break;
		}

		info->fix.smem_start = ch->dma_handle;
		if (var->nonstd)
			info->fix.line_length = var->xres;
		else
			info->fix.line_length = var->xres * (cfg->bpp / 8);

		info->screen_base = buf;
		info->device = &pdev->dev;
		ch->display_var = *var;
	}

	if (error)
		goto err1;

=======
	/* Enable runtime PM. */
	pm_runtime_enable(&pdev->dev);

	for (i = 0; i < num_channels; i++) {
		struct sh_mobile_lcdc_chan *ch = priv->ch + i;

		error = sh_mobile_lcdc_channel_init(priv, ch);
=======
	/* Enable runtime PM. */
	pm_runtime_enable(&pdev->dev);

	for (i = 0; i < num_channels; i++) {
		struct sh_mobile_lcdc_chan *ch = &priv->ch[i];

		error = sh_mobile_lcdc_channel_init(ch);
		if (error)
			goto err1;
	}

	for (i = 0; i < ARRAY_SIZE(pdata->overlays); i++) {
		struct sh_mobile_lcdc_overlay *ovl = &priv->overlays[i];

		ovl->cfg = &pdata->overlays[i];
		ovl->channel = &priv->ch[0];

		error = sh_mobile_lcdc_overlay_init(ovl);
>>>>>>> refs/remotes/origin/master
		if (error)
			goto err1;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	error = sh_mobile_lcdc_start(priv);
	if (error) {
		dev_err(&pdev->dev, "unable to start hardware\n");
		goto err1;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	for (i = 0; i < j; i++) {
		struct sh_mobile_lcdc_chan *ch = priv->ch + i;

		info = ch->info;

		if (info->fbdefio) {
			ch->sglist = vmalloc(sizeof(struct scatterlist) *
					info->fix.smem_len >> PAGE_SHIFT);
			if (!ch->sglist) {
				dev_err(&pdev->dev, "cannot allocate sglist\n");
				goto err1;
			}
		}

		info->bl_dev = ch->bl;

		error = register_framebuffer(info);
		if (error < 0)
			goto err1;

		dev_info(info->dev,
			 "registered %s/%s as %dx%d %dbpp.\n",
			 pdev->name,
			 (ch->cfg.chan == LCDC_CHAN_MAINLCD) ?
			 "mainlcd" : "sublcd",
			 info->var.xres, info->var.yres,
			 ch->cfg.bpp);

		/* deferred io mode: disable clock to save power */
		if (info->fbdefio || info->state == FBINFO_STATE_SUSPENDED)
			sh_mobile_lcdc_clk_off(priv);
=======
	for (i = 0; i < num_channels; i++) {
		struct sh_mobile_lcdc_chan *ch = priv->ch + i;

		error = sh_mobile_lcdc_channel_fb_register(ch);
		if (error)
			goto err1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (i = 0; i < num_channels; i++) {
		struct sh_mobile_lcdc_chan *ch = priv->ch + i;

		error = sh_mobile_lcdc_channel_fb_register(ch);
		if (error)
			goto err1;
	}

	for (i = 0; i < ARRAY_SIZE(pdata->overlays); i++) {
		struct sh_mobile_lcdc_overlay *ovl = &priv->overlays[i];

		error = sh_mobile_lcdc_overlay_fb_register(ovl);
		if (error)
			goto err1;
>>>>>>> refs/remotes/origin/master
	}

	/* Failure ignored */
	priv->notifier.notifier_call = sh_mobile_lcdc_notify;
	fb_register_client(&priv->notifier);

	return 0;
err1:
	sh_mobile_lcdc_remove(pdev);

	return error;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int sh_mobile_lcdc_remove(struct platform_device *pdev)
{
	struct sh_mobile_lcdc_priv *priv = platform_get_drvdata(pdev);
	struct fb_info *info;
	int i;

	fb_unregister_client(&priv->notifier);

	for (i = 0; i < ARRAY_SIZE(priv->ch); i++)
		if (priv->ch[i].info && priv->ch[i].info->dev)
			unregister_framebuffer(priv->ch[i].info);

	sh_mobile_lcdc_stop(priv);

	for (i = 0; i < ARRAY_SIZE(priv->ch); i++) {
		info = priv->ch[i].info;

		if (!info || !info->device)
			continue;

		if (priv->ch[i].sglist)
			vfree(priv->ch[i].sglist);

		if (info->screen_base)
			dma_free_coherent(&pdev->dev, info->fix.smem_len,
					  info->screen_base,
					  priv->ch[i].dma_handle);
		fb_dealloc_cmap(&info->cmap);
		framebuffer_release(info);
	}

	for (i = 0; i < ARRAY_SIZE(priv->ch); i++) {
		if (priv->ch[i].bl)
			sh_mobile_lcdc_bl_remove(priv->ch[i].bl);
	}

	if (priv->dot_clk)
		clk_put(priv->dot_clk);

	if (priv->dev)
		pm_runtime_disable(priv->dev);

	if (priv->base)
		iounmap(priv->base);

	if (priv->irq)
		free_irq(priv->irq, priv);
	kfree(priv);
	return 0;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct platform_driver sh_mobile_lcdc_driver = {
	.driver		= {
		.name		= "sh_mobile_lcdc_fb",
		.owner		= THIS_MODULE,
		.pm		= &sh_mobile_lcdc_dev_pm_ops,
	},
	.probe		= sh_mobile_lcdc_probe,
	.remove		= sh_mobile_lcdc_remove,
};

<<<<<<< HEAD
<<<<<<< HEAD
static int __init sh_mobile_lcdc_init(void)
{
	return platform_driver_register(&sh_mobile_lcdc_driver);
}

static void __exit sh_mobile_lcdc_exit(void)
{
	platform_driver_unregister(&sh_mobile_lcdc_driver);
}

module_init(sh_mobile_lcdc_init);
module_exit(sh_mobile_lcdc_exit);
=======
module_platform_driver(sh_mobile_lcdc_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_platform_driver(sh_mobile_lcdc_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("SuperH Mobile LCDC Framebuffer driver");
MODULE_AUTHOR("Magnus Damm <damm@opensource.se>");
MODULE_LICENSE("GPL v2");
