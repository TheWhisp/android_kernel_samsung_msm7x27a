/*
 * Copyright © 2006-2007 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *	Eric Anholt <eric@anholt.net>
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/cpufreq.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/dmi.h>
>>>>>>> refs/remotes/origin/master
#include <linux/module.h>
#include <linux/input.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/vgaarb.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <drm/drm_edid.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include "drmP.h"
#include "intel_drv.h"
#include "i915_drm.h"
#include "i915_drv.h"
#include "i915_trace.h"
#include "drm_dp_helper.h"
<<<<<<< HEAD

#include "drm_crtc_helper.h"

#define HAS_eDP (intel_pipe_has_type(crtc, INTEL_OUTPUT_EDP))

bool intel_pipe_has_type (struct drm_crtc *crtc, int type);
=======
#include "drm_crtc_helper.h"
#include <linux/dma_remapping.h>

#define HAS_eDP (intel_pipe_has_type(crtc, INTEL_OUTPUT_EDP))

bool intel_pipe_has_type(struct drm_crtc *crtc, int type);
>>>>>>> refs/remotes/origin/cm-10.0
static void intel_update_watermarks(struct drm_device *dev);
static void intel_increase_pllclock(struct drm_crtc *crtc);
static void intel_crtc_update_cursor(struct drm_crtc *crtc, bool on);

typedef struct {
<<<<<<< HEAD
    /* given values */
    int n;
    int m1, m2;
    int p1, p2;
    /* derived values */
    int	dot;
    int	vco;
    int	m;
    int	p;
} intel_clock_t;

typedef struct {
    int	min, max;
} intel_range_t;

typedef struct {
    int	dot_limit;
    int	p2_slow, p2_fast;
=======
	/* given values */
	int n;
	int m1, m2;
	int p1, p2;
	/* derived values */
	int	dot;
	int	vco;
	int	m;
	int	p;
} intel_clock_t;
=======
#include <drm/drm_edid.h>
#include <drm/drmP.h>
#include "intel_drv.h"
#include <drm/i915_drm.h>
#include "i915_drv.h"
#include "i915_trace.h"
#include <drm/drm_dp_helper.h>
#include <drm/drm_crtc_helper.h>
#include <linux/dma_remapping.h>

static void intel_increase_pllclock(struct drm_crtc *crtc);
static void intel_crtc_update_cursor(struct drm_crtc *crtc, bool on);

static void i9xx_crtc_clock_get(struct intel_crtc *crtc,
				struct intel_crtc_config *pipe_config);
static void ironlake_pch_clock_get(struct intel_crtc *crtc,
				   struct intel_crtc_config *pipe_config);

static int intel_set_mode(struct drm_crtc *crtc, struct drm_display_mode *mode,
			  int x, int y, struct drm_framebuffer *old_fb);

>>>>>>> refs/remotes/origin/master

typedef struct {
	int	min, max;
} intel_range_t;

typedef struct {
	int	dot_limit;
	int	p2_slow, p2_fast;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
} intel_p2_t;

#define INTEL_P2_NUM		      2
typedef struct intel_limit intel_limit_t;
struct intel_limit {
<<<<<<< HEAD
    intel_range_t   dot, vco, n, m, m1, m2, p, p1;
    intel_p2_t	    p2;
    bool (* find_pll)(const intel_limit_t *, struct drm_crtc *,
		      int, int, intel_clock_t *);
=======
	intel_range_t   dot, vco, n, m, m1, m2, p, p1;
	intel_p2_t	    p2;
	bool (* find_pll)(const intel_limit_t *, struct drm_crtc *,
			int, int, intel_clock_t *, intel_clock_t *);
>>>>>>> refs/remotes/origin/cm-10.0
};

/* FDI */
#define IRONLAKE_FDI_FREQ		2700000 /* in kHz for mode->clock */

static bool
intel_find_best_PLL(const intel_limit_t *limit, struct drm_crtc *crtc,
<<<<<<< HEAD
		    int target, int refclk, intel_clock_t *best_clock);
static bool
intel_g4x_find_best_PLL(const intel_limit_t *limit, struct drm_crtc *crtc,
			int target, int refclk, intel_clock_t *best_clock);

static bool
intel_find_pll_g4x_dp(const intel_limit_t *, struct drm_crtc *crtc,
		      int target, int refclk, intel_clock_t *best_clock);
static bool
intel_find_pll_ironlake_dp(const intel_limit_t *, struct drm_crtc *crtc,
			   int target, int refclk, intel_clock_t *best_clock);
=======
		    int target, int refclk, intel_clock_t *match_clock,
		    intel_clock_t *best_clock);
static bool
intel_g4x_find_best_PLL(const intel_limit_t *limit, struct drm_crtc *crtc,
			int target, int refclk, intel_clock_t *match_clock,
			intel_clock_t *best_clock);

static bool
intel_find_pll_g4x_dp(const intel_limit_t *, struct drm_crtc *crtc,
		      int target, int refclk, intel_clock_t *match_clock,
		      intel_clock_t *best_clock);
static bool
intel_find_pll_ironlake_dp(const intel_limit_t *, struct drm_crtc *crtc,
			   int target, int refclk, intel_clock_t *match_clock,
			   intel_clock_t *best_clock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
} intel_p2_t;

typedef struct intel_limit intel_limit_t;
struct intel_limit {
	intel_range_t   dot, vco, n, m, m1, m2, p, p1;
	intel_p2_t	    p2;
};

int
intel_pch_rawclk(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	WARN_ON(!HAS_PCH_SPLIT(dev));

	return I915_READ(PCH_RAWCLK_FREQ) & RAWCLK_FREQ_MASK;
}
>>>>>>> refs/remotes/origin/master

static inline u32 /* units of 100MHz */
intel_fdi_link_freq(struct drm_device *dev)
{
	if (IS_GEN5(dev)) {
		struct drm_i915_private *dev_priv = dev->dev_private;
		return (I915_READ(FDI_PLL_BIOS_0) & FDI_PLL_FB_CLOCK_MASK) + 2;
	} else
		return 27;
}

<<<<<<< HEAD
static const intel_limit_t intel_limits_i8xx_dvo = {
<<<<<<< HEAD
        .dot = { .min = 25000, .max = 350000 },
        .vco = { .min = 930000, .max = 1400000 },
        .n = { .min = 3, .max = 16 },
        .m = { .min = 96, .max = 140 },
        .m1 = { .min = 18, .max = 26 },
        .m2 = { .min = 6, .max = 16 },
        .p = { .min = 4, .max = 128 },
        .p1 = { .min = 2, .max = 33 },
=======
=======
static const intel_limit_t intel_limits_i8xx_dac = {
>>>>>>> refs/remotes/origin/master
	.dot = { .min = 25000, .max = 350000 },
	.vco = { .min = 930000, .max = 1400000 },
	.n = { .min = 3, .max = 16 },
	.m = { .min = 96, .max = 140 },
	.m1 = { .min = 18, .max = 26 },
	.m2 = { .min = 6, .max = 16 },
	.p = { .min = 4, .max = 128 },
	.p1 = { .min = 2, .max = 33 },
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	.p2 = { .dot_limit = 165000,
		.p2_slow = 4, .p2_fast = 2 },
	.find_pll = intel_find_best_PLL,
};

static const intel_limit_t intel_limits_i8xx_lvds = {
<<<<<<< HEAD
        .dot = { .min = 25000, .max = 350000 },
        .vco = { .min = 930000, .max = 1400000 },
        .n = { .min = 3, .max = 16 },
        .m = { .min = 96, .max = 140 },
        .m1 = { .min = 18, .max = 26 },
        .m2 = { .min = 6, .max = 16 },
        .p = { .min = 4, .max = 128 },
        .p1 = { .min = 1, .max = 6 },
=======
=======
	.p2 = { .dot_limit = 165000,
		.p2_slow = 4, .p2_fast = 2 },
};

static const intel_limit_t intel_limits_i8xx_dvo = {
	.dot = { .min = 25000, .max = 350000 },
	.vco = { .min = 930000, .max = 1400000 },
	.n = { .min = 3, .max = 16 },
	.m = { .min = 96, .max = 140 },
	.m1 = { .min = 18, .max = 26 },
	.m2 = { .min = 6, .max = 16 },
	.p = { .min = 4, .max = 128 },
	.p1 = { .min = 2, .max = 33 },
	.p2 = { .dot_limit = 165000,
		.p2_slow = 4, .p2_fast = 4 },
};

static const intel_limit_t intel_limits_i8xx_lvds = {
>>>>>>> refs/remotes/origin/master
	.dot = { .min = 25000, .max = 350000 },
	.vco = { .min = 930000, .max = 1400000 },
	.n = { .min = 3, .max = 16 },
	.m = { .min = 96, .max = 140 },
	.m1 = { .min = 18, .max = 26 },
	.m2 = { .min = 6, .max = 16 },
	.p = { .min = 4, .max = 128 },
	.p1 = { .min = 1, .max = 6 },
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	.p2 = { .dot_limit = 165000,
		.p2_slow = 14, .p2_fast = 7 },
	.find_pll = intel_find_best_PLL,
};

static const intel_limit_t intel_limits_i9xx_sdvo = {
<<<<<<< HEAD
        .dot = { .min = 20000, .max = 400000 },
        .vco = { .min = 1400000, .max = 2800000 },
        .n = { .min = 1, .max = 6 },
        .m = { .min = 70, .max = 120 },
        .m1 = { .min = 10, .max = 22 },
        .m2 = { .min = 5, .max = 9 },
        .p = { .min = 5, .max = 80 },
        .p1 = { .min = 1, .max = 8 },
=======
=======
	.p2 = { .dot_limit = 165000,
		.p2_slow = 14, .p2_fast = 7 },
};

static const intel_limit_t intel_limits_i9xx_sdvo = {
>>>>>>> refs/remotes/origin/master
	.dot = { .min = 20000, .max = 400000 },
	.vco = { .min = 1400000, .max = 2800000 },
	.n = { .min = 1, .max = 6 },
	.m = { .min = 70, .max = 120 },
	.m1 = { .min = 8, .max = 18 },
	.m2 = { .min = 3, .max = 7 },
	.p = { .min = 5, .max = 80 },
	.p1 = { .min = 1, .max = 8 },
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	.p2 = { .dot_limit = 200000,
		.p2_slow = 10, .p2_fast = 5 },
	.find_pll = intel_find_best_PLL,
};

static const intel_limit_t intel_limits_i9xx_lvds = {
<<<<<<< HEAD
        .dot = { .min = 20000, .max = 400000 },
        .vco = { .min = 1400000, .max = 2800000 },
        .n = { .min = 1, .max = 6 },
        .m = { .min = 70, .max = 120 },
        .m1 = { .min = 10, .max = 22 },
        .m2 = { .min = 5, .max = 9 },
        .p = { .min = 7, .max = 98 },
        .p1 = { .min = 1, .max = 8 },
=======
=======
	.p2 = { .dot_limit = 200000,
		.p2_slow = 10, .p2_fast = 5 },
};

static const intel_limit_t intel_limits_i9xx_lvds = {
>>>>>>> refs/remotes/origin/master
	.dot = { .min = 20000, .max = 400000 },
	.vco = { .min = 1400000, .max = 2800000 },
	.n = { .min = 1, .max = 6 },
	.m = { .min = 70, .max = 120 },
<<<<<<< HEAD
	.m1 = { .min = 10, .max = 22 },
	.m2 = { .min = 5, .max = 9 },
	.p = { .min = 7, .max = 98 },
	.p1 = { .min = 1, .max = 8 },
>>>>>>> refs/remotes/origin/cm-10.0
	.p2 = { .dot_limit = 112000,
		.p2_slow = 14, .p2_fast = 7 },
	.find_pll = intel_find_best_PLL,
=======
	.m1 = { .min = 8, .max = 18 },
	.m2 = { .min = 3, .max = 7 },
	.p = { .min = 7, .max = 98 },
	.p1 = { .min = 1, .max = 8 },
	.p2 = { .dot_limit = 112000,
		.p2_slow = 14, .p2_fast = 7 },
>>>>>>> refs/remotes/origin/master
};


static const intel_limit_t intel_limits_g4x_sdvo = {
	.dot = { .min = 25000, .max = 270000 },
	.vco = { .min = 1750000, .max = 3500000},
	.n = { .min = 1, .max = 4 },
	.m = { .min = 104, .max = 138 },
	.m1 = { .min = 17, .max = 23 },
	.m2 = { .min = 5, .max = 11 },
	.p = { .min = 10, .max = 30 },
	.p1 = { .min = 1, .max = 3},
	.p2 = { .dot_limit = 270000,
		.p2_slow = 10,
		.p2_fast = 10
	},
<<<<<<< HEAD
	.find_pll = intel_g4x_find_best_PLL,
=======
>>>>>>> refs/remotes/origin/master
};

static const intel_limit_t intel_limits_g4x_hdmi = {
	.dot = { .min = 22000, .max = 400000 },
	.vco = { .min = 1750000, .max = 3500000},
	.n = { .min = 1, .max = 4 },
	.m = { .min = 104, .max = 138 },
	.m1 = { .min = 16, .max = 23 },
	.m2 = { .min = 5, .max = 11 },
	.p = { .min = 5, .max = 80 },
	.p1 = { .min = 1, .max = 8},
	.p2 = { .dot_limit = 165000,
		.p2_slow = 10, .p2_fast = 5 },
<<<<<<< HEAD
	.find_pll = intel_g4x_find_best_PLL,
=======
>>>>>>> refs/remotes/origin/master
};

static const intel_limit_t intel_limits_g4x_single_channel_lvds = {
	.dot = { .min = 20000, .max = 115000 },
	.vco = { .min = 1750000, .max = 3500000 },
	.n = { .min = 1, .max = 3 },
	.m = { .min = 104, .max = 138 },
	.m1 = { .min = 17, .max = 23 },
	.m2 = { .min = 5, .max = 11 },
	.p = { .min = 28, .max = 112 },
	.p1 = { .min = 2, .max = 8 },
	.p2 = { .dot_limit = 0,
		.p2_slow = 14, .p2_fast = 14
	},
<<<<<<< HEAD
	.find_pll = intel_g4x_find_best_PLL,
=======
>>>>>>> refs/remotes/origin/master
};

static const intel_limit_t intel_limits_g4x_dual_channel_lvds = {
	.dot = { .min = 80000, .max = 224000 },
	.vco = { .min = 1750000, .max = 3500000 },
	.n = { .min = 1, .max = 3 },
	.m = { .min = 104, .max = 138 },
	.m1 = { .min = 17, .max = 23 },
	.m2 = { .min = 5, .max = 11 },
	.p = { .min = 14, .max = 42 },
	.p1 = { .min = 2, .max = 6 },
	.p2 = { .dot_limit = 0,
		.p2_slow = 7, .p2_fast = 7
	},
<<<<<<< HEAD
	.find_pll = intel_g4x_find_best_PLL,
};

static const intel_limit_t intel_limits_g4x_display_port = {
<<<<<<< HEAD
        .dot = { .min = 161670, .max = 227000 },
        .vco = { .min = 1750000, .max = 3500000},
        .n = { .min = 1, .max = 2 },
        .m = { .min = 97, .max = 108 },
        .m1 = { .min = 0x10, .max = 0x12 },
        .m2 = { .min = 0x05, .max = 0x06 },
        .p = { .min = 10, .max = 20 },
        .p1 = { .min = 1, .max = 2},
        .p2 = { .dot_limit = 0,
		.p2_slow = 10, .p2_fast = 10 },
        .find_pll = intel_find_pll_g4x_dp,
};

static const intel_limit_t intel_limits_pineview_sdvo = {
        .dot = { .min = 20000, .max = 400000},
        .vco = { .min = 1700000, .max = 3500000 },
	/* Pineview's Ncounter is a ring counter */
        .n = { .min = 3, .max = 6 },
        .m = { .min = 2, .max = 256 },
	/* Pineview only has one combined m divider, which we treat as m2. */
        .m1 = { .min = 0, .max = 0 },
        .m2 = { .min = 0, .max = 254 },
        .p = { .min = 5, .max = 80 },
        .p1 = { .min = 1, .max = 8 },
=======
	.dot = { .min = 161670, .max = 227000 },
	.vco = { .min = 1750000, .max = 3500000},
	.n = { .min = 1, .max = 2 },
	.m = { .min = 97, .max = 108 },
	.m1 = { .min = 0x10, .max = 0x12 },
	.m2 = { .min = 0x05, .max = 0x06 },
	.p = { .min = 10, .max = 20 },
	.p1 = { .min = 1, .max = 2},
	.p2 = { .dot_limit = 0,
		.p2_slow = 10, .p2_fast = 10 },
	.find_pll = intel_find_pll_g4x_dp,
=======
>>>>>>> refs/remotes/origin/master
};

static const intel_limit_t intel_limits_pineview_sdvo = {
	.dot = { .min = 20000, .max = 400000},
	.vco = { .min = 1700000, .max = 3500000 },
	/* Pineview's Ncounter is a ring counter */
	.n = { .min = 3, .max = 6 },
	.m = { .min = 2, .max = 256 },
	/* Pineview only has one combined m divider, which we treat as m2. */
	.m1 = { .min = 0, .max = 0 },
	.m2 = { .min = 0, .max = 254 },
	.p = { .min = 5, .max = 80 },
	.p1 = { .min = 1, .max = 8 },
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	.p2 = { .dot_limit = 200000,
		.p2_slow = 10, .p2_fast = 5 },
	.find_pll = intel_find_best_PLL,
};

static const intel_limit_t intel_limits_pineview_lvds = {
<<<<<<< HEAD
        .dot = { .min = 20000, .max = 400000 },
        .vco = { .min = 1700000, .max = 3500000 },
        .n = { .min = 3, .max = 6 },
        .m = { .min = 2, .max = 256 },
        .m1 = { .min = 0, .max = 0 },
        .m2 = { .min = 0, .max = 254 },
        .p = { .min = 7, .max = 112 },
        .p1 = { .min = 1, .max = 8 },
=======
=======
	.p2 = { .dot_limit = 200000,
		.p2_slow = 10, .p2_fast = 5 },
};

static const intel_limit_t intel_limits_pineview_lvds = {
>>>>>>> refs/remotes/origin/master
	.dot = { .min = 20000, .max = 400000 },
	.vco = { .min = 1700000, .max = 3500000 },
	.n = { .min = 3, .max = 6 },
	.m = { .min = 2, .max = 256 },
	.m1 = { .min = 0, .max = 0 },
	.m2 = { .min = 0, .max = 254 },
	.p = { .min = 7, .max = 112 },
	.p1 = { .min = 1, .max = 8 },
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	.p2 = { .dot_limit = 112000,
		.p2_slow = 14, .p2_fast = 14 },
	.find_pll = intel_find_best_PLL,
=======
	.p2 = { .dot_limit = 112000,
		.p2_slow = 14, .p2_fast = 14 },
>>>>>>> refs/remotes/origin/master
};

/* Ironlake / Sandybridge
 *
 * We calculate clock using (register_value + 2) for N/M1/M2, so here
 * the range value for them is (actual_value - 2).
 */
static const intel_limit_t intel_limits_ironlake_dac = {
	.dot = { .min = 25000, .max = 350000 },
	.vco = { .min = 1760000, .max = 3510000 },
	.n = { .min = 1, .max = 5 },
	.m = { .min = 79, .max = 127 },
	.m1 = { .min = 12, .max = 22 },
	.m2 = { .min = 5, .max = 9 },
	.p = { .min = 5, .max = 80 },
	.p1 = { .min = 1, .max = 8 },
	.p2 = { .dot_limit = 225000,
		.p2_slow = 10, .p2_fast = 5 },
<<<<<<< HEAD
	.find_pll = intel_g4x_find_best_PLL,
=======
>>>>>>> refs/remotes/origin/master
};

static const intel_limit_t intel_limits_ironlake_single_lvds = {
	.dot = { .min = 25000, .max = 350000 },
	.vco = { .min = 1760000, .max = 3510000 },
	.n = { .min = 1, .max = 3 },
	.m = { .min = 79, .max = 118 },
	.m1 = { .min = 12, .max = 22 },
	.m2 = { .min = 5, .max = 9 },
	.p = { .min = 28, .max = 112 },
	.p1 = { .min = 2, .max = 8 },
	.p2 = { .dot_limit = 225000,
		.p2_slow = 14, .p2_fast = 14 },
<<<<<<< HEAD
	.find_pll = intel_g4x_find_best_PLL,
=======
>>>>>>> refs/remotes/origin/master
};

static const intel_limit_t intel_limits_ironlake_dual_lvds = {
	.dot = { .min = 25000, .max = 350000 },
	.vco = { .min = 1760000, .max = 3510000 },
	.n = { .min = 1, .max = 3 },
	.m = { .min = 79, .max = 127 },
	.m1 = { .min = 12, .max = 22 },
	.m2 = { .min = 5, .max = 9 },
	.p = { .min = 14, .max = 56 },
	.p1 = { .min = 2, .max = 8 },
	.p2 = { .dot_limit = 225000,
		.p2_slow = 7, .p2_fast = 7 },
<<<<<<< HEAD
	.find_pll = intel_g4x_find_best_PLL,
=======
>>>>>>> refs/remotes/origin/master
};

/* LVDS 100mhz refclk limits. */
static const intel_limit_t intel_limits_ironlake_single_lvds_100m = {
	.dot = { .min = 25000, .max = 350000 },
	.vco = { .min = 1760000, .max = 3510000 },
	.n = { .min = 1, .max = 2 },
	.m = { .min = 79, .max = 126 },
	.m1 = { .min = 12, .max = 22 },
	.m2 = { .min = 5, .max = 9 },
	.p = { .min = 28, .max = 112 },
<<<<<<< HEAD
<<<<<<< HEAD
	.p1 = { .min = 2,.max = 8 },
=======
	.p1 = { .min = 2, .max = 8 },
>>>>>>> refs/remotes/origin/cm-10.0
	.p2 = { .dot_limit = 225000,
		.p2_slow = 14, .p2_fast = 14 },
	.find_pll = intel_g4x_find_best_PLL,
=======
	.p1 = { .min = 2, .max = 8 },
	.p2 = { .dot_limit = 225000,
		.p2_slow = 14, .p2_fast = 14 },
>>>>>>> refs/remotes/origin/master
};

static const intel_limit_t intel_limits_ironlake_dual_lvds_100m = {
	.dot = { .min = 25000, .max = 350000 },
	.vco = { .min = 1760000, .max = 3510000 },
	.n = { .min = 1, .max = 3 },
	.m = { .min = 79, .max = 126 },
	.m1 = { .min = 12, .max = 22 },
	.m2 = { .min = 5, .max = 9 },
	.p = { .min = 14, .max = 42 },
<<<<<<< HEAD
<<<<<<< HEAD
	.p1 = { .min = 2,.max = 6 },
=======
	.p1 = { .min = 2, .max = 6 },
>>>>>>> refs/remotes/origin/cm-10.0
	.p2 = { .dot_limit = 225000,
		.p2_slow = 7, .p2_fast = 7 },
	.find_pll = intel_g4x_find_best_PLL,
};

static const intel_limit_t intel_limits_ironlake_display_port = {
<<<<<<< HEAD
        .dot = { .min = 25000, .max = 350000 },
        .vco = { .min = 1760000, .max = 3510000},
        .n = { .min = 1, .max = 2 },
        .m = { .min = 81, .max = 90 },
        .m1 = { .min = 12, .max = 22 },
        .m2 = { .min = 5, .max = 9 },
        .p = { .min = 10, .max = 20 },
        .p1 = { .min = 1, .max = 2},
        .p2 = { .dot_limit = 0,
		.p2_slow = 10, .p2_fast = 10 },
        .find_pll = intel_find_pll_ironlake_dp,
};

=======
	.dot = { .min = 25000, .max = 350000 },
	.vco = { .min = 1760000, .max = 3510000},
	.n = { .min = 1, .max = 2 },
	.m = { .min = 81, .max = 90 },
	.m1 = { .min = 12, .max = 22 },
	.m2 = { .min = 5, .max = 9 },
	.p = { .min = 10, .max = 20 },
	.p1 = { .min = 1, .max = 2},
	.p2 = { .dot_limit = 0,
		.p2_slow = 10, .p2_fast = 10 },
	.find_pll = intel_find_pll_ironlake_dp,
};

static bool is_dual_link_lvds(struct drm_i915_private *dev_priv,
			      unsigned int reg)
{
	unsigned int val;

	if (dev_priv->lvds_val)
		val = dev_priv->lvds_val;
	else {
		/* BIOS should set the proper LVDS register value at boot, but
		 * in reality, it doesn't set the value when the lid is closed;
		 * we need to check "the value to be set" in VBT when LVDS
		 * register is uninitialized.
		 */
		val = I915_READ(reg);
		if (!(val & ~LVDS_DETECTED))
			val = dev_priv->bios_lvds_val;
		dev_priv->lvds_val = val;
	}
	return (val & LVDS_CLKB_POWER_MASK) == LVDS_CLKB_POWER_UP;
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	.p1 = { .min = 2, .max = 6 },
	.p2 = { .dot_limit = 225000,
		.p2_slow = 7, .p2_fast = 7 },
};

static const intel_limit_t intel_limits_vlv = {
	 /*
	  * These are the data rate limits (measured in fast clocks)
	  * since those are the strictest limits we have. The fast
	  * clock and actual rate limits are more relaxed, so checking
	  * them would make no difference.
	  */
	.dot = { .min = 25000 * 5, .max = 270000 * 5 },
	.vco = { .min = 4000000, .max = 6000000 },
	.n = { .min = 1, .max = 7 },
	.m1 = { .min = 2, .max = 3 },
	.m2 = { .min = 11, .max = 156 },
	.p1 = { .min = 2, .max = 3 },
	.p2 = { .p2_slow = 2, .p2_fast = 20 }, /* slow=min, fast=max */
};

static void vlv_clock(int refclk, intel_clock_t *clock)
{
	clock->m = clock->m1 * clock->m2;
	clock->p = clock->p1 * clock->p2;
	clock->vco = DIV_ROUND_CLOSEST(refclk * clock->m, clock->n);
	clock->dot = DIV_ROUND_CLOSEST(clock->vco, clock->p);
}

/**
 * Returns whether any output on the specified pipe is of the specified type
 */
static bool intel_pipe_has_type(struct drm_crtc *crtc, int type)
{
	struct drm_device *dev = crtc->dev;
	struct intel_encoder *encoder;

	for_each_encoder_on_crtc(dev, crtc, encoder)
		if (encoder->type == type)
			return true;

	return false;
}

>>>>>>> refs/remotes/origin/master
static const intel_limit_t *intel_ironlake_limit(struct drm_crtc *crtc,
						int refclk)
{
	struct drm_device *dev = crtc->dev;
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = dev->dev_private;
	const intel_limit_t *limit;

	if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS)) {
<<<<<<< HEAD
		if ((I915_READ(PCH_LVDS) & LVDS_CLKB_POWER_MASK) ==
		    LVDS_CLKB_POWER_UP) {
=======
		if (is_dual_link_lvds(dev_priv, PCH_LVDS)) {
>>>>>>> refs/remotes/origin/cm-10.0
			/* LVDS dual channel */
=======
	const intel_limit_t *limit;

	if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS)) {
		if (intel_is_dual_link_lvds(dev)) {
>>>>>>> refs/remotes/origin/master
			if (refclk == 100000)
				limit = &intel_limits_ironlake_dual_lvds_100m;
			else
				limit = &intel_limits_ironlake_dual_lvds;
		} else {
			if (refclk == 100000)
				limit = &intel_limits_ironlake_single_lvds_100m;
			else
				limit = &intel_limits_ironlake_single_lvds;
		}
<<<<<<< HEAD
	} else if (intel_pipe_has_type(crtc, INTEL_OUTPUT_DISPLAYPORT) ||
			HAS_eDP)
		limit = &intel_limits_ironlake_display_port;
	else
=======
	} else
>>>>>>> refs/remotes/origin/master
		limit = &intel_limits_ironlake_dac;

	return limit;
}

static const intel_limit_t *intel_g4x_limit(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = dev->dev_private;
	const intel_limit_t *limit;

	if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS)) {
<<<<<<< HEAD
		if ((I915_READ(LVDS) & LVDS_CLKB_POWER_MASK) ==
		    LVDS_CLKB_POWER_UP)
=======
		if (is_dual_link_lvds(dev_priv, LVDS))
>>>>>>> refs/remotes/origin/cm-10.0
			/* LVDS with dual channel */
			limit = &intel_limits_g4x_dual_channel_lvds;
		else
			/* LVDS with dual channel */
=======
	const intel_limit_t *limit;

	if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS)) {
		if (intel_is_dual_link_lvds(dev))
			limit = &intel_limits_g4x_dual_channel_lvds;
		else
>>>>>>> refs/remotes/origin/master
			limit = &intel_limits_g4x_single_channel_lvds;
	} else if (intel_pipe_has_type(crtc, INTEL_OUTPUT_HDMI) ||
		   intel_pipe_has_type(crtc, INTEL_OUTPUT_ANALOG)) {
		limit = &intel_limits_g4x_hdmi;
	} else if (intel_pipe_has_type(crtc, INTEL_OUTPUT_SDVO)) {
		limit = &intel_limits_g4x_sdvo;
<<<<<<< HEAD
<<<<<<< HEAD
	} else if (intel_pipe_has_type (crtc, INTEL_OUTPUT_DISPLAYPORT)) {
=======
	} else if (intel_pipe_has_type(crtc, INTEL_OUTPUT_DISPLAYPORT)) {
>>>>>>> refs/remotes/origin/cm-10.0
		limit = &intel_limits_g4x_display_port;
=======
>>>>>>> refs/remotes/origin/master
	} else /* The option is for other outputs */
		limit = &intel_limits_i9xx_sdvo;

	return limit;
}

static const intel_limit_t *intel_limit(struct drm_crtc *crtc, int refclk)
{
	struct drm_device *dev = crtc->dev;
	const intel_limit_t *limit;

	if (HAS_PCH_SPLIT(dev))
		limit = intel_ironlake_limit(crtc, refclk);
	else if (IS_G4X(dev)) {
		limit = intel_g4x_limit(crtc);
	} else if (IS_PINEVIEW(dev)) {
		if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS))
			limit = &intel_limits_pineview_lvds;
		else
			limit = &intel_limits_pineview_sdvo;
<<<<<<< HEAD
=======
	} else if (IS_VALLEYVIEW(dev)) {
		limit = &intel_limits_vlv;
>>>>>>> refs/remotes/origin/master
	} else if (!IS_GEN2(dev)) {
		if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS))
			limit = &intel_limits_i9xx_lvds;
		else
			limit = &intel_limits_i9xx_sdvo;
	} else {
		if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS))
			limit = &intel_limits_i8xx_lvds;
<<<<<<< HEAD
		else
			limit = &intel_limits_i8xx_dvo;
=======
		else if (intel_pipe_has_type(crtc, INTEL_OUTPUT_DVO))
			limit = &intel_limits_i8xx_dvo;
		else
			limit = &intel_limits_i8xx_dac;
>>>>>>> refs/remotes/origin/master
	}
	return limit;
}

/* m1 is reserved as 0 in Pineview, n is a ring counter */
static void pineview_clock(int refclk, intel_clock_t *clock)
{
	clock->m = clock->m2 + 2;
	clock->p = clock->p1 * clock->p2;
<<<<<<< HEAD
	clock->vco = refclk * clock->m / clock->n;
	clock->dot = clock->vco / clock->p;
}

static void intel_clock(struct drm_device *dev, int refclk, intel_clock_t *clock)
{
	if (IS_PINEVIEW(dev)) {
		pineview_clock(refclk, clock);
		return;
	}
	clock->m = 5 * (clock->m1 + 2) + (clock->m2 + 2);
	clock->p = clock->p1 * clock->p2;
	clock->vco = refclk * clock->m / (clock->n + 2);
	clock->dot = clock->vco / clock->p;
}

/**
 * Returns whether any output on the specified pipe is of the specified type
 */
bool intel_pipe_has_type(struct drm_crtc *crtc, int type)
{
	struct drm_device *dev = crtc->dev;
	struct drm_mode_config *mode_config = &dev->mode_config;
	struct intel_encoder *encoder;

	list_for_each_entry(encoder, &mode_config->encoder_list, base.head)
		if (encoder->base.crtc == crtc && encoder->type == type)
			return true;

	return false;
=======
	clock->vco = DIV_ROUND_CLOSEST(refclk * clock->m, clock->n);
	clock->dot = DIV_ROUND_CLOSEST(clock->vco, clock->p);
}

static uint32_t i9xx_dpll_compute_m(struct dpll *dpll)
{
	return 5 * (dpll->m1 + 2) + (dpll->m2 + 2);
}

static void i9xx_clock(int refclk, intel_clock_t *clock)
{
	clock->m = i9xx_dpll_compute_m(clock);
	clock->p = clock->p1 * clock->p2;
	clock->vco = DIV_ROUND_CLOSEST(refclk * clock->m, clock->n + 2);
	clock->dot = DIV_ROUND_CLOSEST(clock->vco, clock->p);
>>>>>>> refs/remotes/origin/master
}

#define INTELPllInvalid(s)   do { /* DRM_DEBUG(s); */ return false; } while (0)
/**
 * Returns whether the given set of divisors are valid for a given refclk with
 * the given connectors.
 */

static bool intel_PLL_is_valid(struct drm_device *dev,
			       const intel_limit_t *limit,
			       const intel_clock_t *clock)
{
<<<<<<< HEAD
	if (clock->p1  < limit->p1.min  || limit->p1.max  < clock->p1)
<<<<<<< HEAD
		INTELPllInvalid ("p1 out of range\n");
	if (clock->p   < limit->p.min   || limit->p.max   < clock->p)
		INTELPllInvalid ("p out of range\n");
	if (clock->m2  < limit->m2.min  || limit->m2.max  < clock->m2)
		INTELPllInvalid ("m2 out of range\n");
	if (clock->m1  < limit->m1.min  || limit->m1.max  < clock->m1)
		INTELPllInvalid ("m1 out of range\n");
	if (clock->m1 <= clock->m2 && !IS_PINEVIEW(dev))
		INTELPllInvalid ("m1 <= m2\n");
	if (clock->m   < limit->m.min   || limit->m.max   < clock->m)
		INTELPllInvalid ("m out of range\n");
	if (clock->n   < limit->n.min   || limit->n.max   < clock->n)
		INTELPllInvalid ("n out of range\n");
	if (clock->vco < limit->vco.min || limit->vco.max < clock->vco)
		INTELPllInvalid ("vco out of range\n");
=======
		INTELPllInvalid("p1 out of range\n");
	if (clock->p   < limit->p.min   || limit->p.max   < clock->p)
		INTELPllInvalid("p out of range\n");
=======
	if (clock->n   < limit->n.min   || limit->n.max   < clock->n)
		INTELPllInvalid("n out of range\n");
	if (clock->p1  < limit->p1.min  || limit->p1.max  < clock->p1)
		INTELPllInvalid("p1 out of range\n");
>>>>>>> refs/remotes/origin/master
	if (clock->m2  < limit->m2.min  || limit->m2.max  < clock->m2)
		INTELPllInvalid("m2 out of range\n");
	if (clock->m1  < limit->m1.min  || limit->m1.max  < clock->m1)
		INTELPllInvalid("m1 out of range\n");
<<<<<<< HEAD
	if (clock->m1 <= clock->m2 && !IS_PINEVIEW(dev))
		INTELPllInvalid("m1 <= m2\n");
	if (clock->m   < limit->m.min   || limit->m.max   < clock->m)
		INTELPllInvalid("m out of range\n");
	if (clock->n   < limit->n.min   || limit->n.max   < clock->n)
		INTELPllInvalid("n out of range\n");
	if (clock->vco < limit->vco.min || limit->vco.max < clock->vco)
		INTELPllInvalid("vco out of range\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======

	if (!IS_PINEVIEW(dev) && !IS_VALLEYVIEW(dev))
		if (clock->m1 <= clock->m2)
			INTELPllInvalid("m1 <= m2\n");

	if (!IS_VALLEYVIEW(dev)) {
		if (clock->p < limit->p.min || limit->p.max < clock->p)
			INTELPllInvalid("p out of range\n");
		if (clock->m < limit->m.min || limit->m.max < clock->m)
			INTELPllInvalid("m out of range\n");
	}

	if (clock->vco < limit->vco.min || limit->vco.max < clock->vco)
		INTELPllInvalid("vco out of range\n");
>>>>>>> refs/remotes/origin/master
	/* XXX: We may need to be checking "Dot clock" depending on the multiplier,
	 * connector, etc., rather than just a single range.
	 */
	if (clock->dot < limit->dot.min || limit->dot.max < clock->dot)
<<<<<<< HEAD
<<<<<<< HEAD
		INTELPllInvalid ("dot out of range\n");
=======
		INTELPllInvalid("dot out of range\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		INTELPllInvalid("dot out of range\n");
>>>>>>> refs/remotes/origin/master

	return true;
}

static bool
<<<<<<< HEAD
intel_find_best_PLL(const intel_limit_t *limit, struct drm_crtc *crtc,
<<<<<<< HEAD
		    int target, int refclk, intel_clock_t *best_clock)
=======
		    int target, int refclk, intel_clock_t *match_clock,
		    intel_clock_t *best_clock)
>>>>>>> refs/remotes/origin/cm-10.0

{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	intel_clock_t clock;
	int err = target;

	if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS) &&
	    (I915_READ(LVDS)) != 0) {
		/*
		 * For LVDS, if the panel is on, just rely on its current
		 * settings for dual-channel.  We haven't figured out how to
		 * reliably set up different single/dual channel state, if we
		 * even can.
		 */
<<<<<<< HEAD
		if ((I915_READ(LVDS) & LVDS_CLKB_POWER_MASK) ==
		    LVDS_CLKB_POWER_UP)
=======
		if (is_dual_link_lvds(dev_priv, LVDS))
>>>>>>> refs/remotes/origin/cm-10.0
=======
i9xx_find_best_dpll(const intel_limit_t *limit, struct drm_crtc *crtc,
		    int target, int refclk, intel_clock_t *match_clock,
		    intel_clock_t *best_clock)
{
	struct drm_device *dev = crtc->dev;
	intel_clock_t clock;
	int err = target;

	if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS)) {
		/*
		 * For LVDS just rely on its current settings for dual-channel.
		 * We haven't figured out how to reliably set up different
		 * single/dual channel state, if we even can.
		 */
		if (intel_is_dual_link_lvds(dev))
>>>>>>> refs/remotes/origin/master
			clock.p2 = limit->p2.p2_fast;
		else
			clock.p2 = limit->p2.p2_slow;
	} else {
		if (target < limit->p2.dot_limit)
			clock.p2 = limit->p2.p2_slow;
		else
			clock.p2 = limit->p2.p2_fast;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	memset (best_clock, 0, sizeof (*best_clock));
=======
	memset(best_clock, 0, sizeof(*best_clock));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	memset(best_clock, 0, sizeof(*best_clock));
>>>>>>> refs/remotes/origin/master

	for (clock.m1 = limit->m1.min; clock.m1 <= limit->m1.max;
	     clock.m1++) {
		for (clock.m2 = limit->m2.min;
		     clock.m2 <= limit->m2.max; clock.m2++) {
<<<<<<< HEAD
			/* m1 is always 0 in Pineview */
			if (clock.m2 >= clock.m1 && !IS_PINEVIEW(dev))
=======
			if (clock.m2 >= clock.m1)
>>>>>>> refs/remotes/origin/master
				break;
			for (clock.n = limit->n.min;
			     clock.n <= limit->n.max; clock.n++) {
				for (clock.p1 = limit->p1.min;
					clock.p1 <= limit->p1.max; clock.p1++) {
					int this_err;

<<<<<<< HEAD
					intel_clock(dev, refclk, &clock);
					if (!intel_PLL_is_valid(dev, limit,
								&clock))
						continue;
<<<<<<< HEAD
=======
					if (match_clock &&
					    clock.p != match_clock->p)
						continue;
>>>>>>> refs/remotes/origin/cm-10.0
=======
					i9xx_clock(refclk, &clock);
					if (!intel_PLL_is_valid(dev, limit,
								&clock))
						continue;
					if (match_clock &&
					    clock.p != match_clock->p)
						continue;
>>>>>>> refs/remotes/origin/master

					this_err = abs(clock.dot - target);
					if (this_err < err) {
						*best_clock = clock;
						err = this_err;
					}
				}
			}
		}
	}

	return (err != target);
}

static bool
<<<<<<< HEAD
intel_g4x_find_best_PLL(const intel_limit_t *limit, struct drm_crtc *crtc,
<<<<<<< HEAD
			int target, int refclk, intel_clock_t *best_clock)
=======
			int target, int refclk, intel_clock_t *match_clock,
			intel_clock_t *best_clock)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
=======
pnv_find_best_dpll(const intel_limit_t *limit, struct drm_crtc *crtc,
		   int target, int refclk, intel_clock_t *match_clock,
		   intel_clock_t *best_clock)
{
	struct drm_device *dev = crtc->dev;
	intel_clock_t clock;
	int err = target;

	if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS)) {
		/*
		 * For LVDS just rely on its current settings for dual-channel.
		 * We haven't figured out how to reliably set up different
		 * single/dual channel state, if we even can.
		 */
		if (intel_is_dual_link_lvds(dev))
			clock.p2 = limit->p2.p2_fast;
		else
			clock.p2 = limit->p2.p2_slow;
	} else {
		if (target < limit->p2.dot_limit)
			clock.p2 = limit->p2.p2_slow;
		else
			clock.p2 = limit->p2.p2_fast;
	}

	memset(best_clock, 0, sizeof(*best_clock));

	for (clock.m1 = limit->m1.min; clock.m1 <= limit->m1.max;
	     clock.m1++) {
		for (clock.m2 = limit->m2.min;
		     clock.m2 <= limit->m2.max; clock.m2++) {
			for (clock.n = limit->n.min;
			     clock.n <= limit->n.max; clock.n++) {
				for (clock.p1 = limit->p1.min;
					clock.p1 <= limit->p1.max; clock.p1++) {
					int this_err;

					pineview_clock(refclk, &clock);
					if (!intel_PLL_is_valid(dev, limit,
								&clock))
						continue;
					if (match_clock &&
					    clock.p != match_clock->p)
						continue;

					this_err = abs(clock.dot - target);
					if (this_err < err) {
						*best_clock = clock;
						err = this_err;
					}
				}
			}
		}
	}

	return (err != target);
}

static bool
g4x_find_best_dpll(const intel_limit_t *limit, struct drm_crtc *crtc,
		   int target, int refclk, intel_clock_t *match_clock,
		   intel_clock_t *best_clock)
{
	struct drm_device *dev = crtc->dev;
>>>>>>> refs/remotes/origin/master
	intel_clock_t clock;
	int max_n;
	bool found;
	/* approximately equals target * 0.00585 */
	int err_most = (target >> 8) + (target >> 9);
	found = false;

	if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS)) {
<<<<<<< HEAD
		int lvds_reg;

		if (HAS_PCH_SPLIT(dev))
			lvds_reg = PCH_LVDS;
		else
			lvds_reg = LVDS;
		if ((I915_READ(lvds_reg) & LVDS_CLKB_POWER_MASK) ==
		    LVDS_CLKB_POWER_UP)
=======
		if (intel_is_dual_link_lvds(dev))
>>>>>>> refs/remotes/origin/master
			clock.p2 = limit->p2.p2_fast;
		else
			clock.p2 = limit->p2.p2_slow;
	} else {
		if (target < limit->p2.dot_limit)
			clock.p2 = limit->p2.p2_slow;
		else
			clock.p2 = limit->p2.p2_fast;
	}

	memset(best_clock, 0, sizeof(*best_clock));
	max_n = limit->n.max;
	/* based on hardware requirement, prefer smaller n to precision */
	for (clock.n = limit->n.min; clock.n <= max_n; clock.n++) {
		/* based on hardware requirement, prefere larger m1,m2 */
		for (clock.m1 = limit->m1.max;
		     clock.m1 >= limit->m1.min; clock.m1--) {
			for (clock.m2 = limit->m2.max;
			     clock.m2 >= limit->m2.min; clock.m2--) {
				for (clock.p1 = limit->p1.max;
				     clock.p1 >= limit->p1.min; clock.p1--) {
					int this_err;

<<<<<<< HEAD
					intel_clock(dev, refclk, &clock);
					if (!intel_PLL_is_valid(dev, limit,
								&clock))
						continue;
<<<<<<< HEAD
=======
					if (match_clock &&
					    clock.p != match_clock->p)
						continue;
>>>>>>> refs/remotes/origin/cm-10.0
=======
					i9xx_clock(refclk, &clock);
					if (!intel_PLL_is_valid(dev, limit,
								&clock))
						continue;
>>>>>>> refs/remotes/origin/master

					this_err = abs(clock.dot - target);
					if (this_err < err_most) {
						*best_clock = clock;
						err_most = this_err;
						max_n = clock.n;
						found = true;
					}
				}
			}
		}
	}
	return found;
}

static bool
<<<<<<< HEAD
intel_find_pll_ironlake_dp(const intel_limit_t *limit, struct drm_crtc *crtc,
<<<<<<< HEAD
			   int target, int refclk, intel_clock_t *best_clock)
=======
			   int target, int refclk, intel_clock_t *match_clock,
			   intel_clock_t *best_clock)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct drm_device *dev = crtc->dev;
	intel_clock_t clock;

	if (target < 200000) {
		clock.n = 1;
		clock.p1 = 2;
		clock.p2 = 10;
		clock.m1 = 12;
		clock.m2 = 9;
	} else {
		clock.n = 2;
		clock.p1 = 1;
		clock.p2 = 10;
		clock.m1 = 14;
		clock.m2 = 8;
	}
	intel_clock(dev, refclk, &clock);
	memcpy(best_clock, &clock, sizeof(intel_clock_t));
	return true;
}

/* DisplayPort has only two frequencies, 162MHz and 270MHz */
static bool
intel_find_pll_g4x_dp(const intel_limit_t *limit, struct drm_crtc *crtc,
<<<<<<< HEAD
		      int target, int refclk, intel_clock_t *best_clock)
=======
		      int target, int refclk, intel_clock_t *match_clock,
		      intel_clock_t *best_clock)
>>>>>>> refs/remotes/origin/cm-10.0
{
	intel_clock_t clock;
	if (target < 200000) {
		clock.p1 = 2;
		clock.p2 = 10;
		clock.n = 2;
		clock.m1 = 23;
		clock.m2 = 8;
	} else {
		clock.p1 = 1;
		clock.p2 = 10;
		clock.n = 1;
		clock.m1 = 14;
		clock.m2 = 2;
	}
	clock.m = 5 * (clock.m1 + 2) + (clock.m2 + 2);
	clock.p = (clock.p1 * clock.p2);
	clock.dot = 96000 * clock.m / (clock.n + 2) / clock.p;
	clock.vco = 0;
	memcpy(best_clock, &clock, sizeof(intel_clock_t));
	return true;
=======
vlv_find_best_dpll(const intel_limit_t *limit, struct drm_crtc *crtc,
		   int target, int refclk, intel_clock_t *match_clock,
		   intel_clock_t *best_clock)
{
	struct drm_device *dev = crtc->dev;
	intel_clock_t clock;
	unsigned int bestppm = 1000000;
	/* min update 19.2 MHz */
	int max_n = min(limit->n.max, refclk / 19200);
	bool found = false;

	target *= 5; /* fast clock */

	memset(best_clock, 0, sizeof(*best_clock));

	/* based on hardware requirement, prefer smaller n to precision */
	for (clock.n = limit->n.min; clock.n <= max_n; clock.n++) {
		for (clock.p1 = limit->p1.max; clock.p1 >= limit->p1.min; clock.p1--) {
			for (clock.p2 = limit->p2.p2_fast; clock.p2 >= limit->p2.p2_slow;
			     clock.p2 -= clock.p2 > 10 ? 2 : 1) {
				clock.p = clock.p1 * clock.p2;
				/* based on hardware requirement, prefer bigger m1,m2 values */
				for (clock.m1 = limit->m1.min; clock.m1 <= limit->m1.max; clock.m1++) {
					unsigned int ppm, diff;

					clock.m2 = DIV_ROUND_CLOSEST(target * clock.p * clock.n,
								     refclk * clock.m1);

					vlv_clock(refclk, &clock);

					if (!intel_PLL_is_valid(dev, limit,
								&clock))
						continue;

					diff = abs(clock.dot - target);
					ppm = div_u64(1000000ULL * diff, target);

					if (ppm < 100 && clock.p > best_clock->p) {
						bestppm = 0;
						*best_clock = clock;
						found = true;
					}

					if (bestppm >= 10 && ppm < bestppm - 10) {
						bestppm = ppm;
						*best_clock = clock;
						found = true;
					}
				}
			}
		}
	}

	return found;
}

bool intel_crtc_active(struct drm_crtc *crtc)
{
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);

	/* Be paranoid as we can arrive here with only partial
	 * state retrieved from the hardware during setup.
	 *
	 * We can ditch the adjusted_mode.crtc_clock check as soon
	 * as Haswell has gained clock readout/fastboot support.
	 *
	 * We can ditch the crtc->fb check as soon as we can
	 * properly reconstruct framebuffers.
	 */
	return intel_crtc->active && crtc->fb &&
		intel_crtc->config.adjusted_mode.crtc_clock;
}

enum transcoder intel_pipe_to_cpu_transcoder(struct drm_i915_private *dev_priv,
					     enum pipe pipe)
{
	struct drm_crtc *crtc = dev_priv->pipe_to_crtc_mapping[pipe];
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);

	return intel_crtc->config.cpu_transcoder;
}

static void ironlake_wait_for_vblank(struct drm_device *dev, int pipe)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 frame, frame_reg = PIPEFRAME(pipe);

	frame = I915_READ(frame_reg);

	if (wait_for(I915_READ_NOTRACE(frame_reg) != frame, 50))
		DRM_DEBUG_KMS("vblank wait timed out\n");
>>>>>>> refs/remotes/origin/master
}

/**
 * intel_wait_for_vblank - wait for vblank on a given pipe
 * @dev: drm device
 * @pipe: pipe to wait for
 *
 * Wait for vblank to occur on a given pipe.  Needed for various bits of
 * mode setting code.
 */
void intel_wait_for_vblank(struct drm_device *dev, int pipe)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipestat_reg = PIPESTAT(pipe);

<<<<<<< HEAD
=======
	if (INTEL_INFO(dev)->gen >= 5) {
		ironlake_wait_for_vblank(dev, pipe);
		return;
	}

>>>>>>> refs/remotes/origin/master
	/* Clear existing vblank status. Note this will clear any other
	 * sticky status fields as well.
	 *
	 * This races with i915_driver_irq_handler() with the result
	 * that either function could miss a vblank event.  Here it is not
	 * fatal, as we will either wait upon the next vblank interrupt or
	 * timeout.  Generally speaking intel_wait_for_vblank() is only
	 * called during modeset at which time the GPU should be idle and
	 * should *not* be performing page flips and thus not waiting on
	 * vblanks...
	 * Currently, the result of us stealing a vblank from the irq
	 * handler is that a single frame will be skipped during swapbuffers.
	 */
	I915_WRITE(pipestat_reg,
		   I915_READ(pipestat_reg) | PIPE_VBLANK_INTERRUPT_STATUS);

	/* Wait for vblank interrupt bit to set */
	if (wait_for(I915_READ(pipestat_reg) &
		     PIPE_VBLANK_INTERRUPT_STATUS,
		     50))
		DRM_DEBUG_KMS("vblank wait timed out\n");
}

<<<<<<< HEAD
=======
static bool pipe_dsl_stopped(struct drm_device *dev, enum pipe pipe)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 reg = PIPEDSL(pipe);
	u32 line1, line2;
	u32 line_mask;

	if (IS_GEN2(dev))
		line_mask = DSL_LINEMASK_GEN2;
	else
		line_mask = DSL_LINEMASK_GEN3;

	line1 = I915_READ(reg) & line_mask;
	mdelay(5);
	line2 = I915_READ(reg) & line_mask;

	return line1 == line2;
}

>>>>>>> refs/remotes/origin/master
/*
 * intel_wait_for_pipe_off - wait for pipe to turn off
 * @dev: drm device
 * @pipe: pipe to wait for
 *
 * After disabling a pipe, we can't wait for vblank in the usual way,
 * spinning on the vblank interrupt status bit, since we won't actually
 * see an interrupt when the pipe is disabled.
 *
 * On Gen4 and above:
 *   wait for the pipe register state bit to turn off
 *
 * Otherwise:
 *   wait for the display line value to settle (it usually
 *   ends up stopping at the start of the next frame).
 *
 */
void intel_wait_for_pipe_off(struct drm_device *dev, int pipe)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
<<<<<<< HEAD

	if (INTEL_INFO(dev)->gen >= 4) {
		int reg = PIPECONF(pipe);
=======
	enum transcoder cpu_transcoder = intel_pipe_to_cpu_transcoder(dev_priv,
								      pipe);

	if (INTEL_INFO(dev)->gen >= 4) {
		int reg = PIPECONF(cpu_transcoder);
>>>>>>> refs/remotes/origin/master

		/* Wait for the Pipe State to go off */
		if (wait_for((I915_READ(reg) & I965_PIPECONF_ACTIVE) == 0,
			     100))
<<<<<<< HEAD
			DRM_DEBUG_KMS("pipe_off wait timed out\n");
	} else {
		u32 last_line;
		int reg = PIPEDSL(pipe);
		unsigned long timeout = jiffies + msecs_to_jiffies(100);

		/* Wait for the display line to settle */
		do {
			last_line = I915_READ(reg) & DSL_LINEMASK;
			mdelay(5);
		} while (((I915_READ(reg) & DSL_LINEMASK) != last_line) &&
			 time_after(timeout, jiffies));
		if (time_after(jiffies, timeout))
			DRM_DEBUG_KMS("pipe_off wait timed out\n");
	}
}

=======
			WARN(1, "pipe_off wait timed out\n");
	} else {
		/* Wait for the display line to settle */
		if (wait_for(pipe_dsl_stopped(dev, pipe), 100))
			WARN(1, "pipe_off wait timed out\n");
	}
}

/*
 * ibx_digital_port_connected - is the specified port connected?
 * @dev_priv: i915 private structure
 * @port: the port to test
 *
 * Returns true if @port is connected, false otherwise.
 */
bool ibx_digital_port_connected(struct drm_i915_private *dev_priv,
				struct intel_digital_port *port)
{
	u32 bit;

	if (HAS_PCH_IBX(dev_priv->dev)) {
		switch(port->port) {
		case PORT_B:
			bit = SDE_PORTB_HOTPLUG;
			break;
		case PORT_C:
			bit = SDE_PORTC_HOTPLUG;
			break;
		case PORT_D:
			bit = SDE_PORTD_HOTPLUG;
			break;
		default:
			return true;
		}
	} else {
		switch(port->port) {
		case PORT_B:
			bit = SDE_PORTB_HOTPLUG_CPT;
			break;
		case PORT_C:
			bit = SDE_PORTC_HOTPLUG_CPT;
			break;
		case PORT_D:
			bit = SDE_PORTD_HOTPLUG_CPT;
			break;
		default:
			return true;
		}
	}

	return I915_READ(SDEISR) & bit;
}

>>>>>>> refs/remotes/origin/master
static const char *state_string(bool enabled)
{
	return enabled ? "on" : "off";
}

/* Only for pre-ILK configs */
<<<<<<< HEAD
static void assert_pll(struct drm_i915_private *dev_priv,
		       enum pipe pipe, bool state)
=======
void assert_pll(struct drm_i915_private *dev_priv,
		enum pipe pipe, bool state)
>>>>>>> refs/remotes/origin/master
{
	int reg;
	u32 val;
	bool cur_state;

	reg = DPLL(pipe);
	val = I915_READ(reg);
	cur_state = !!(val & DPLL_VCO_ENABLE);
	WARN(cur_state != state,
	     "PLL state assertion failure (expected %s, current %s)\n",
	     state_string(state), state_string(cur_state));
}
<<<<<<< HEAD
#define assert_pll_enabled(d, p) assert_pll(d, p, true)
#define assert_pll_disabled(d, p) assert_pll(d, p, false)

/* For ILK+ */
static void assert_pch_pll(struct drm_i915_private *dev_priv,
			   enum pipe pipe, bool state)
{
	int reg;
	u32 val;
	bool cur_state;

<<<<<<< HEAD
=======
	if (HAS_PCH_CPT(dev_priv->dev)) {
		u32 pch_dpll;

		pch_dpll = I915_READ(PCH_DPLL_SEL);

		/* Make sure the selected PLL is enabled to the transcoder */
		WARN(!((pch_dpll >> (4 * pipe)) & 8),
		     "transcoder %d PLL not enabled\n", pipe);

		/* Convert the transcoder pipe number to a pll pipe number */
		pipe = (pch_dpll >> (4 * pipe)) & 1;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	reg = PCH_DPLL(pipe);
	val = I915_READ(reg);
	cur_state = !!(val & DPLL_VCO_ENABLE);
	WARN(cur_state != state,
	     "PCH PLL state assertion failure (expected %s, current %s)\n",
	     state_string(state), state_string(cur_state));
}
#define assert_pch_pll_enabled(d, p) assert_pch_pll(d, p, true)
#define assert_pch_pll_disabled(d, p) assert_pch_pll(d, p, false)
=======

/* XXX: the dsi pll is shared between MIPI DSI ports */
static void assert_dsi_pll(struct drm_i915_private *dev_priv, bool state)
{
	u32 val;
	bool cur_state;

	mutex_lock(&dev_priv->dpio_lock);
	val = vlv_cck_read(dev_priv, CCK_REG_DSI_PLL_CONTROL);
	mutex_unlock(&dev_priv->dpio_lock);

	cur_state = val & DSI_PLL_VCO_EN;
	WARN(cur_state != state,
	     "DSI PLL state assertion failure (expected %s, current %s)\n",
	     state_string(state), state_string(cur_state));
}
#define assert_dsi_pll_enabled(d) assert_dsi_pll(d, true)
#define assert_dsi_pll_disabled(d) assert_dsi_pll(d, false)

struct intel_shared_dpll *
intel_crtc_to_shared_dpll(struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = crtc->base.dev->dev_private;

	if (crtc->config.shared_dpll < 0)
		return NULL;

	return &dev_priv->shared_dplls[crtc->config.shared_dpll];
}

/* For ILK+ */
void assert_shared_dpll(struct drm_i915_private *dev_priv,
			struct intel_shared_dpll *pll,
			bool state)
{
	bool cur_state;
	struct intel_dpll_hw_state hw_state;

	if (HAS_PCH_LPT(dev_priv->dev)) {
		DRM_DEBUG_DRIVER("LPT detected: skipping PCH PLL test\n");
		return;
	}

	if (WARN (!pll,
		  "asserting DPLL %s with no DPLL\n", state_string(state)))
		return;

	cur_state = pll->get_hw_state(dev_priv, pll, &hw_state);
	WARN(cur_state != state,
	     "%s assertion failure (expected %s, current %s)\n",
	     pll->name, state_string(state), state_string(cur_state));
}
>>>>>>> refs/remotes/origin/master

static void assert_fdi_tx(struct drm_i915_private *dev_priv,
			  enum pipe pipe, bool state)
{
	int reg;
	u32 val;
	bool cur_state;
<<<<<<< HEAD

	reg = FDI_TX_CTL(pipe);
	val = I915_READ(reg);
	cur_state = !!(val & FDI_TX_ENABLE);
=======
	enum transcoder cpu_transcoder = intel_pipe_to_cpu_transcoder(dev_priv,
								      pipe);

	if (HAS_DDI(dev_priv->dev)) {
		/* DDI does not have a specific FDI_TX register */
		reg = TRANS_DDI_FUNC_CTL(cpu_transcoder);
		val = I915_READ(reg);
		cur_state = !!(val & TRANS_DDI_FUNC_ENABLE);
	} else {
		reg = FDI_TX_CTL(pipe);
		val = I915_READ(reg);
		cur_state = !!(val & FDI_TX_ENABLE);
	}
>>>>>>> refs/remotes/origin/master
	WARN(cur_state != state,
	     "FDI TX state assertion failure (expected %s, current %s)\n",
	     state_string(state), state_string(cur_state));
}
#define assert_fdi_tx_enabled(d, p) assert_fdi_tx(d, p, true)
#define assert_fdi_tx_disabled(d, p) assert_fdi_tx(d, p, false)

static void assert_fdi_rx(struct drm_i915_private *dev_priv,
			  enum pipe pipe, bool state)
{
	int reg;
	u32 val;
	bool cur_state;

	reg = FDI_RX_CTL(pipe);
	val = I915_READ(reg);
	cur_state = !!(val & FDI_RX_ENABLE);
	WARN(cur_state != state,
	     "FDI RX state assertion failure (expected %s, current %s)\n",
	     state_string(state), state_string(cur_state));
}
#define assert_fdi_rx_enabled(d, p) assert_fdi_rx(d, p, true)
#define assert_fdi_rx_disabled(d, p) assert_fdi_rx(d, p, false)

static void assert_fdi_tx_pll_enabled(struct drm_i915_private *dev_priv,
				      enum pipe pipe)
{
	int reg;
	u32 val;

	/* ILK FDI PLL is always enabled */
	if (dev_priv->info->gen == 5)
		return;

<<<<<<< HEAD
=======
	/* On Haswell, DDI ports are responsible for the FDI PLL setup */
	if (HAS_DDI(dev_priv->dev))
		return;

>>>>>>> refs/remotes/origin/master
	reg = FDI_TX_CTL(pipe);
	val = I915_READ(reg);
	WARN(!(val & FDI_TX_PLL_ENABLE), "FDI TX PLL assertion failure, should be active but is disabled\n");
}

<<<<<<< HEAD
static void assert_fdi_rx_pll_enabled(struct drm_i915_private *dev_priv,
				      enum pipe pipe)
{
	int reg;
	u32 val;

	reg = FDI_RX_CTL(pipe);
	val = I915_READ(reg);
	WARN(!(val & FDI_RX_PLL_ENABLE), "FDI RX PLL assertion failure, should be active but is disabled\n");
=======
void assert_fdi_rx_pll(struct drm_i915_private *dev_priv,
		       enum pipe pipe, bool state)
{
	int reg;
	u32 val;
	bool cur_state;

	reg = FDI_RX_CTL(pipe);
	val = I915_READ(reg);
	cur_state = !!(val & FDI_RX_PLL_ENABLE);
	WARN(cur_state != state,
	     "FDI RX PLL assertion failure (expected %s, current %s)\n",
	     state_string(state), state_string(cur_state));
>>>>>>> refs/remotes/origin/master
}

static void assert_panel_unlocked(struct drm_i915_private *dev_priv,
				  enum pipe pipe)
{
	int pp_reg, lvds_reg;
	u32 val;
	enum pipe panel_pipe = PIPE_A;
<<<<<<< HEAD
<<<<<<< HEAD
	bool locked = locked;
=======
	bool locked = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bool locked = true;
>>>>>>> refs/remotes/origin/master

	if (HAS_PCH_SPLIT(dev_priv->dev)) {
		pp_reg = PCH_PP_CONTROL;
		lvds_reg = PCH_LVDS;
	} else {
		pp_reg = PP_CONTROL;
		lvds_reg = LVDS;
	}

	val = I915_READ(pp_reg);
	if (!(val & PANEL_POWER_ON) ||
	    ((val & PANEL_UNLOCK_REGS) == PANEL_UNLOCK_REGS))
		locked = false;

	if (I915_READ(lvds_reg) & LVDS_PIPEB_SELECT)
		panel_pipe = PIPE_B;

	WARN(panel_pipe == pipe && locked,
	     "panel assertion failure, pipe %c regs locked\n",
	     pipe_name(pipe));
}

<<<<<<< HEAD
<<<<<<< HEAD
static void assert_pipe(struct drm_i915_private *dev_priv,
			enum pipe pipe, bool state)
=======
void assert_pipe(struct drm_i915_private *dev_priv,
		 enum pipe pipe, bool state)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void assert_cursor(struct drm_i915_private *dev_priv,
			  enum pipe pipe, bool state)
{
	struct drm_device *dev = dev_priv->dev;
	bool cur_state;

	if (IS_IVYBRIDGE(dev) || IS_HASWELL(dev))
		cur_state = I915_READ(CURCNTR_IVB(pipe)) & CURSOR_MODE;
	else if (IS_845G(dev) || IS_I865G(dev))
		cur_state = I915_READ(_CURACNTR) & CURSOR_ENABLE;
	else
		cur_state = I915_READ(CURCNTR(pipe)) & CURSOR_MODE;

	WARN(cur_state != state,
	     "cursor on pipe %c assertion failure (expected %s, current %s)\n",
	     pipe_name(pipe), state_string(state), state_string(cur_state));
}
#define assert_cursor_enabled(d, p) assert_cursor(d, p, true)
#define assert_cursor_disabled(d, p) assert_cursor(d, p, false)

void assert_pipe(struct drm_i915_private *dev_priv,
		 enum pipe pipe, bool state)
>>>>>>> refs/remotes/origin/master
{
	int reg;
	u32 val;
	bool cur_state;
<<<<<<< HEAD

<<<<<<< HEAD
=======
=======
	enum transcoder cpu_transcoder = intel_pipe_to_cpu_transcoder(dev_priv,
								      pipe);

>>>>>>> refs/remotes/origin/master
	/* if we need the pipe A quirk it must be always on */
	if (pipe == PIPE_A && dev_priv->quirks & QUIRK_PIPEA_FORCE)
		state = true;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	reg = PIPECONF(pipe);
	val = I915_READ(reg);
	cur_state = !!(val & PIPECONF_ENABLE);
=======
	if (!intel_display_power_enabled(dev_priv->dev,
				POWER_DOMAIN_TRANSCODER(cpu_transcoder))) {
		cur_state = false;
	} else {
		reg = PIPECONF(cpu_transcoder);
		val = I915_READ(reg);
		cur_state = !!(val & PIPECONF_ENABLE);
	}

>>>>>>> refs/remotes/origin/master
	WARN(cur_state != state,
	     "pipe %c assertion failure (expected %s, current %s)\n",
	     pipe_name(pipe), state_string(state), state_string(cur_state));
}
<<<<<<< HEAD
<<<<<<< HEAD
#define assert_pipe_enabled(d, p) assert_pipe(d, p, true)
#define assert_pipe_disabled(d, p) assert_pipe(d, p, false)

static void assert_plane_enabled(struct drm_i915_private *dev_priv,
				 enum plane plane)
{
	int reg;
	u32 val;

	reg = DSPCNTR(plane);
	val = I915_READ(reg);
	WARN(!(val & DISPLAY_PLANE_ENABLE),
	     "plane %c assertion failure, should be active but is disabled\n",
	     plane_name(plane));
}

=======
=======
>>>>>>> refs/remotes/origin/master

static void assert_plane(struct drm_i915_private *dev_priv,
			 enum plane plane, bool state)
{
	int reg;
	u32 val;
	bool cur_state;

	reg = DSPCNTR(plane);
	val = I915_READ(reg);
	cur_state = !!(val & DISPLAY_PLANE_ENABLE);
	WARN(cur_state != state,
	     "plane %c assertion failure (expected %s, current %s)\n",
	     plane_name(plane), state_string(state), state_string(cur_state));
}

#define assert_plane_enabled(d, p) assert_plane(d, p, true)
#define assert_plane_disabled(d, p) assert_plane(d, p, false)

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static void assert_planes_disabled(struct drm_i915_private *dev_priv,
				   enum pipe pipe)
{
=======
static void assert_planes_disabled(struct drm_i915_private *dev_priv,
				   enum pipe pipe)
{
	struct drm_device *dev = dev_priv->dev;
>>>>>>> refs/remotes/origin/master
	int reg, i;
	u32 val;
	int cur_pipe;

<<<<<<< HEAD
	/* Planes are fixed to pipes on ILK+ */
<<<<<<< HEAD
	if (HAS_PCH_SPLIT(dev_priv->dev))
		return;
=======
	if (HAS_PCH_SPLIT(dev_priv->dev)) {
=======
	/* Primary planes are fixed to pipes on gen4+ */
	if (INTEL_INFO(dev)->gen >= 4) {
>>>>>>> refs/remotes/origin/master
		reg = DSPCNTR(pipe);
		val = I915_READ(reg);
		WARN((val & DISPLAY_PLANE_ENABLE),
		     "plane %c assertion failure, should be disabled but not\n",
		     plane_name(pipe));
		return;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	/* Need to check both planes against the pipe */
	for (i = 0; i < 2; i++) {
=======

	/* Need to check both planes against the pipe */
	for_each_pipe(i) {
>>>>>>> refs/remotes/origin/master
		reg = DSPCNTR(i);
		val = I915_READ(reg);
		cur_pipe = (val & DISPPLANE_SEL_PIPE_MASK) >>
			DISPPLANE_SEL_PIPE_SHIFT;
		WARN((val & DISPLAY_PLANE_ENABLE) && pipe == cur_pipe,
		     "plane %c assertion failure, should be off on pipe %c but is still active\n",
		     plane_name(i), pipe_name(pipe));
	}
}

<<<<<<< HEAD
=======
static void assert_sprites_disabled(struct drm_i915_private *dev_priv,
				    enum pipe pipe)
{
	struct drm_device *dev = dev_priv->dev;
	int reg, i;
	u32 val;

	if (IS_VALLEYVIEW(dev)) {
		for (i = 0; i < dev_priv->num_plane; i++) {
			reg = SPCNTR(pipe, i);
			val = I915_READ(reg);
			WARN((val & SP_ENABLE),
			     "sprite %c assertion failure, should be off on pipe %c but is still active\n",
			     sprite_name(pipe, i), pipe_name(pipe));
		}
	} else if (INTEL_INFO(dev)->gen >= 7) {
		reg = SPRCTL(pipe);
		val = I915_READ(reg);
		WARN((val & SPRITE_ENABLE),
		     "sprite %c assertion failure, should be off on pipe %c but is still active\n",
		     plane_name(pipe), pipe_name(pipe));
	} else if (INTEL_INFO(dev)->gen >= 5) {
		reg = DVSCNTR(pipe);
		val = I915_READ(reg);
		WARN((val & DVS_ENABLE),
		     "sprite %c assertion failure, should be off on pipe %c but is still active\n",
		     plane_name(pipe), pipe_name(pipe));
	}
}

>>>>>>> refs/remotes/origin/master
static void assert_pch_refclk_enabled(struct drm_i915_private *dev_priv)
{
	u32 val;
	bool enabled;

<<<<<<< HEAD
=======
	if (HAS_PCH_LPT(dev_priv->dev)) {
		DRM_DEBUG_DRIVER("LPT does not has PCH refclk, skipping check\n");
		return;
	}

>>>>>>> refs/remotes/origin/master
	val = I915_READ(PCH_DREF_CONTROL);
	enabled = !!(val & (DREF_SSC_SOURCE_MASK | DREF_NONSPREAD_SOURCE_MASK |
			    DREF_SUPERSPREAD_SOURCE_MASK));
	WARN(!enabled, "PCH refclk assertion failure, should be active but is disabled\n");
}

<<<<<<< HEAD
static void assert_transcoder_disabled(struct drm_i915_private *dev_priv,
				       enum pipe pipe)
=======
static void assert_pch_transcoder_disabled(struct drm_i915_private *dev_priv,
					   enum pipe pipe)
>>>>>>> refs/remotes/origin/master
{
	int reg;
	u32 val;
	bool enabled;

<<<<<<< HEAD
	reg = TRANSCONF(pipe);
=======
	reg = PCH_TRANSCONF(pipe);
>>>>>>> refs/remotes/origin/master
	val = I915_READ(reg);
	enabled = !!(val & TRANS_ENABLE);
	WARN(enabled,
	     "transcoder assertion failed, should be off on pipe %c but is still active\n",
	     pipe_name(pipe));
}

<<<<<<< HEAD
<<<<<<< HEAD
static void assert_pch_dp_disabled(struct drm_i915_private *dev_priv,
				   enum pipe pipe, int reg)
{
	u32 val = I915_READ(reg);
	WARN(DP_PIPE_ENABLED(val, pipe),
=======
=======
>>>>>>> refs/remotes/origin/master
static bool dp_pipe_enabled(struct drm_i915_private *dev_priv,
			    enum pipe pipe, u32 port_sel, u32 val)
{
	if ((val & DP_PORT_EN) == 0)
		return false;

	if (HAS_PCH_CPT(dev_priv->dev)) {
		u32	trans_dp_ctl_reg = TRANS_DP_CTL(pipe);
		u32	trans_dp_ctl = I915_READ(trans_dp_ctl_reg);
		if ((trans_dp_ctl & TRANS_DP_PORT_SEL_MASK) != port_sel)
			return false;
	} else {
		if ((val & DP_PIPE_MASK) != (pipe << 30))
			return false;
	}
	return true;
}

static bool hdmi_pipe_enabled(struct drm_i915_private *dev_priv,
			      enum pipe pipe, u32 val)
{
<<<<<<< HEAD
	if ((val & PORT_ENABLE) == 0)
		return false;

	if (HAS_PCH_CPT(dev_priv->dev)) {
		if ((val & PORT_TRANS_SEL_MASK) != PORT_TRANS_SEL_CPT(pipe))
			return false;
	} else {
		if ((val & TRANSCODER_MASK) != TRANSCODER(pipe))
=======
	if ((val & SDVO_ENABLE) == 0)
		return false;

	if (HAS_PCH_CPT(dev_priv->dev)) {
		if ((val & SDVO_PIPE_SEL_MASK_CPT) != SDVO_PIPE_SEL_CPT(pipe))
			return false;
	} else {
		if ((val & SDVO_PIPE_SEL_MASK) != SDVO_PIPE_SEL(pipe))
>>>>>>> refs/remotes/origin/master
			return false;
	}
	return true;
}

static bool lvds_pipe_enabled(struct drm_i915_private *dev_priv,
			      enum pipe pipe, u32 val)
{
	if ((val & LVDS_PORT_EN) == 0)
		return false;

	if (HAS_PCH_CPT(dev_priv->dev)) {
		if ((val & PORT_TRANS_SEL_MASK) != PORT_TRANS_SEL_CPT(pipe))
			return false;
	} else {
		if ((val & LVDS_PIPE_MASK) != LVDS_PIPE(pipe))
			return false;
	}
	return true;
}

static bool adpa_pipe_enabled(struct drm_i915_private *dev_priv,
			      enum pipe pipe, u32 val)
{
	if ((val & ADPA_DAC_ENABLE) == 0)
		return false;
	if (HAS_PCH_CPT(dev_priv->dev)) {
		if ((val & PORT_TRANS_SEL_MASK) != PORT_TRANS_SEL_CPT(pipe))
			return false;
	} else {
		if ((val & ADPA_PIPE_SELECT_MASK) != ADPA_PIPE_SELECT(pipe))
			return false;
	}
	return true;
}

static void assert_pch_dp_disabled(struct drm_i915_private *dev_priv,
				   enum pipe pipe, int reg, u32 port_sel)
{
	u32 val = I915_READ(reg);
	WARN(dp_pipe_enabled(dev_priv, pipe, port_sel, val),
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	     "PCH DP (0x%08x) enabled on transcoder %c, should be disabled\n",
	     reg, pipe_name(pipe));
=======
	     "PCH DP (0x%08x) enabled on transcoder %c, should be disabled\n",
	     reg, pipe_name(pipe));

	WARN(HAS_PCH_IBX(dev_priv->dev) && (val & DP_PORT_EN) == 0
	     && (val & DP_PIPEB_SELECT),
	     "IBX PCH dp port still using transcoder B\n");
>>>>>>> refs/remotes/origin/master
}

static void assert_pch_hdmi_disabled(struct drm_i915_private *dev_priv,
				     enum pipe pipe, int reg)
{
	u32 val = I915_READ(reg);
<<<<<<< HEAD
<<<<<<< HEAD
	WARN(HDMI_PIPE_ENABLED(val, pipe),
	     "PCH DP (0x%08x) enabled on transcoder %c, should be disabled\n",
=======
	WARN(hdmi_pipe_enabled(dev_priv, pipe, val),
	     "PCH HDMI (0x%08x) enabled on transcoder %c, should be disabled\n",
>>>>>>> refs/remotes/origin/cm-10.0
	     reg, pipe_name(pipe));
=======
	WARN(hdmi_pipe_enabled(dev_priv, pipe, val),
	     "PCH HDMI (0x%08x) enabled on transcoder %c, should be disabled\n",
	     reg, pipe_name(pipe));

	WARN(HAS_PCH_IBX(dev_priv->dev) && (val & SDVO_ENABLE) == 0
	     && (val & SDVO_PIPE_B_SELECT),
	     "IBX PCH hdmi port still using transcoder B\n");
>>>>>>> refs/remotes/origin/master
}

static void assert_pch_ports_disabled(struct drm_i915_private *dev_priv,
				      enum pipe pipe)
{
	int reg;
	u32 val;

<<<<<<< HEAD
<<<<<<< HEAD
	assert_pch_dp_disabled(dev_priv, pipe, PCH_DP_B);
	assert_pch_dp_disabled(dev_priv, pipe, PCH_DP_C);
	assert_pch_dp_disabled(dev_priv, pipe, PCH_DP_D);

	reg = PCH_ADPA;
	val = I915_READ(reg);
	WARN(ADPA_PIPE_ENABLED(val, pipe),
=======
=======
>>>>>>> refs/remotes/origin/master
	assert_pch_dp_disabled(dev_priv, pipe, PCH_DP_B, TRANS_DP_PORT_SEL_B);
	assert_pch_dp_disabled(dev_priv, pipe, PCH_DP_C, TRANS_DP_PORT_SEL_C);
	assert_pch_dp_disabled(dev_priv, pipe, PCH_DP_D, TRANS_DP_PORT_SEL_D);

	reg = PCH_ADPA;
	val = I915_READ(reg);
	WARN(adpa_pipe_enabled(dev_priv, pipe, val),
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	     "PCH VGA enabled on transcoder %c, should be disabled\n",
	     pipe_name(pipe));

	reg = PCH_LVDS;
	val = I915_READ(reg);
<<<<<<< HEAD
<<<<<<< HEAD
	WARN(LVDS_PIPE_ENABLED(val, pipe),
=======
	WARN(lvds_pipe_enabled(dev_priv, pipe, val),
>>>>>>> refs/remotes/origin/cm-10.0
	     "PCH LVDS enabled on transcoder %c, should be disabled\n",
	     pipe_name(pipe));

	assert_pch_hdmi_disabled(dev_priv, pipe, HDMIB);
	assert_pch_hdmi_disabled(dev_priv, pipe, HDMIC);
	assert_pch_hdmi_disabled(dev_priv, pipe, HDMID);
}

/**
 * intel_enable_pll - enable a PLL
 * @dev_priv: i915 private structure
 * @pipe: pipe PLL to enable
 *
 * Enable @pipe's PLL so we can start pumping pixels from a plane.  Check to
 * make sure the PLL reg is writable first though, since the panel write
 * protect mechanism may be enabled.
 *
 * Note!  This is for pre-ILK only.
 */
static void intel_enable_pll(struct drm_i915_private *dev_priv, enum pipe pipe)
{
	int reg;
	u32 val;

	/* No really, not for ILK+ */
	BUG_ON(dev_priv->info->gen >= 5);

	/* PLL is protected by panel, make sure we can write it */
	if (IS_MOBILE(dev_priv->dev) && !IS_I830(dev_priv->dev))
		assert_panel_unlocked(dev_priv, pipe);

	reg = DPLL(pipe);
	val = I915_READ(reg);
	val |= DPLL_VCO_ENABLE;

	/* We do this three times for luck */
	I915_WRITE(reg, val);
	POSTING_READ(reg);
	udelay(150); /* wait for warmup */
	I915_WRITE(reg, val);
	POSTING_READ(reg);
	udelay(150); /* wait for warmup */
	I915_WRITE(reg, val);
=======
	WARN(lvds_pipe_enabled(dev_priv, pipe, val),
	     "PCH LVDS enabled on transcoder %c, should be disabled\n",
	     pipe_name(pipe));

	assert_pch_hdmi_disabled(dev_priv, pipe, PCH_HDMIB);
	assert_pch_hdmi_disabled(dev_priv, pipe, PCH_HDMIC);
	assert_pch_hdmi_disabled(dev_priv, pipe, PCH_HDMID);
}

static void intel_init_dpio(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (!IS_VALLEYVIEW(dev))
		return;

	/*
	 * From VLV2A0_DP_eDP_DPIO_driver_vbios_notes_10.docx -
	 *  6.	De-assert cmn_reset/side_reset. Same as VLV X0.
	 *   a.	GUnit 0x2110 bit[0] set to 1 (def 0)
	 *   b.	The other bits such as sfr settings / modesel may all be set
	 *      to 0.
	 *
	 * This should only be done on init and resume from S3 with both
	 * PLLs disabled, or we risk losing DPIO and PLL synchronization.
	 */
	I915_WRITE(DPIO_CTL, I915_READ(DPIO_CTL) | DPIO_CMNRST);
}

static void vlv_enable_pll(struct intel_crtc *crtc)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int reg = DPLL(crtc->pipe);
	u32 dpll = crtc->config.dpll_hw_state.dpll;

	assert_pipe_disabled(dev_priv, crtc->pipe);

	/* No really, not for ILK+ */
	BUG_ON(!IS_VALLEYVIEW(dev_priv->dev));

	/* PLL is protected by panel, make sure we can write it */
	if (IS_MOBILE(dev_priv->dev) && !IS_I830(dev_priv->dev))
		assert_panel_unlocked(dev_priv, crtc->pipe);

	I915_WRITE(reg, dpll);
	POSTING_READ(reg);
	udelay(150);

	if (wait_for(((I915_READ(reg) & DPLL_LOCK_VLV) == DPLL_LOCK_VLV), 1))
		DRM_ERROR("DPLL %d failed to lock\n", crtc->pipe);

	I915_WRITE(DPLL_MD(crtc->pipe), crtc->config.dpll_hw_state.dpll_md);
	POSTING_READ(DPLL_MD(crtc->pipe));

	/* We do this three times for luck */
	I915_WRITE(reg, dpll);
	POSTING_READ(reg);
	udelay(150); /* wait for warmup */
	I915_WRITE(reg, dpll);
	POSTING_READ(reg);
	udelay(150); /* wait for warmup */
	I915_WRITE(reg, dpll);
	POSTING_READ(reg);
	udelay(150); /* wait for warmup */
}

static void i9xx_enable_pll(struct intel_crtc *crtc)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int reg = DPLL(crtc->pipe);
	u32 dpll = crtc->config.dpll_hw_state.dpll;

	assert_pipe_disabled(dev_priv, crtc->pipe);

	/* No really, not for ILK+ */
	BUG_ON(dev_priv->info->gen >= 5);

	/* PLL is protected by panel, make sure we can write it */
	if (IS_MOBILE(dev) && !IS_I830(dev))
		assert_panel_unlocked(dev_priv, crtc->pipe);

	I915_WRITE(reg, dpll);

	/* Wait for the clocks to stabilize. */
	POSTING_READ(reg);
	udelay(150);

	if (INTEL_INFO(dev)->gen >= 4) {
		I915_WRITE(DPLL_MD(crtc->pipe),
			   crtc->config.dpll_hw_state.dpll_md);
	} else {
		/* The pixel multiplier can only be updated once the
		 * DPLL is enabled and the clocks are stable.
		 *
		 * So write it again.
		 */
		I915_WRITE(reg, dpll);
	}

	/* We do this three times for luck */
	I915_WRITE(reg, dpll);
	POSTING_READ(reg);
	udelay(150); /* wait for warmup */
	I915_WRITE(reg, dpll);
	POSTING_READ(reg);
	udelay(150); /* wait for warmup */
	I915_WRITE(reg, dpll);
>>>>>>> refs/remotes/origin/master
	POSTING_READ(reg);
	udelay(150); /* wait for warmup */
}

/**
<<<<<<< HEAD
 * intel_disable_pll - disable a PLL
=======
 * i9xx_disable_pll - disable a PLL
>>>>>>> refs/remotes/origin/master
 * @dev_priv: i915 private structure
 * @pipe: pipe PLL to disable
 *
 * Disable the PLL for @pipe, making sure the pipe is off first.
 *
 * Note!  This is for pre-ILK only.
 */
<<<<<<< HEAD
static void intel_disable_pll(struct drm_i915_private *dev_priv, enum pipe pipe)
{
	int reg;
	u32 val;

=======
static void i9xx_disable_pll(struct drm_i915_private *dev_priv, enum pipe pipe)
{
>>>>>>> refs/remotes/origin/master
	/* Don't disable pipe A or pipe A PLLs if needed */
	if (pipe == PIPE_A && (dev_priv->quirks & QUIRK_PIPEA_FORCE))
		return;

	/* Make sure the pipe isn't still relying on us */
	assert_pipe_disabled(dev_priv, pipe);

<<<<<<< HEAD
	reg = DPLL(pipe);
	val = I915_READ(reg);
	val &= ~DPLL_VCO_ENABLE;
	I915_WRITE(reg, val);
	POSTING_READ(reg);
}

/**
 * intel_enable_pch_pll - enable PCH PLL
=======
	I915_WRITE(DPLL(pipe), 0);
	POSTING_READ(DPLL(pipe));
}

static void vlv_disable_pll(struct drm_i915_private *dev_priv, enum pipe pipe)
{
	u32 val = 0;

	/* Make sure the pipe isn't still relying on us */
	assert_pipe_disabled(dev_priv, pipe);

	/* Leave integrated clock source enabled */
	if (pipe == PIPE_B)
		val = DPLL_INTEGRATED_CRI_CLK_VLV;
	I915_WRITE(DPLL(pipe), val);
	POSTING_READ(DPLL(pipe));
}

void vlv_wait_port_ready(struct drm_i915_private *dev_priv, int port)
{
	u32 port_mask;

	if (!port)
		port_mask = DPLL_PORTB_READY_MASK;
	else
		port_mask = DPLL_PORTC_READY_MASK;

	if (wait_for((I915_READ(DPLL(0)) & port_mask) == 0, 1000))
		WARN(1, "timed out waiting for port %c ready: 0x%08x\n",
		     'B' + port, I915_READ(DPLL(0)));
}

/**
 * ironlake_enable_shared_dpll - enable PCH PLL
>>>>>>> refs/remotes/origin/master
 * @dev_priv: i915 private structure
 * @pipe: pipe PLL to enable
 *
 * The PCH PLL needs to be enabled before the PCH transcoder, since it
 * drives the transcoder clock.
 */
<<<<<<< HEAD
static void intel_enable_pch_pll(struct drm_i915_private *dev_priv,
				 enum pipe pipe)
{
	int reg;
	u32 val;

<<<<<<< HEAD
=======
	if (pipe > 1)
		return;

>>>>>>> refs/remotes/origin/cm-10.0
	/* PCH only available on ILK+ */
	BUG_ON(dev_priv->info->gen < 5);

	/* PCH refclock must be enabled first */
	assert_pch_refclk_enabled(dev_priv);

	reg = PCH_DPLL(pipe);
	val = I915_READ(reg);
	val |= DPLL_VCO_ENABLE;
	I915_WRITE(reg, val);
	POSTING_READ(reg);
	udelay(200);
}

static void intel_disable_pch_pll(struct drm_i915_private *dev_priv,
				  enum pipe pipe)
{
	int reg;
<<<<<<< HEAD
	u32 val;
=======
	u32 val, pll_mask = TRANSC_DPLL_ENABLE | TRANSC_DPLLB_SEL,
		pll_sel = TRANSC_DPLL_ENABLE;

	if (pipe > 1)
		return;
>>>>>>> refs/remotes/origin/cm-10.0

	/* PCH only available on ILK+ */
	BUG_ON(dev_priv->info->gen < 5);

	/* Make sure transcoder isn't still depending on us */
	assert_transcoder_disabled(dev_priv, pipe);

<<<<<<< HEAD
=======
	if (pipe == 0)
		pll_sel |= TRANSC_DPLLA_SEL;
	else if (pipe == 1)
		pll_sel |= TRANSC_DPLLB_SEL;


	if ((I915_READ(PCH_DPLL_SEL) & pll_mask) == pll_sel)
		return;

>>>>>>> refs/remotes/origin/cm-10.0
	reg = PCH_DPLL(pipe);
	val = I915_READ(reg);
	val &= ~DPLL_VCO_ENABLE;
	I915_WRITE(reg, val);
	POSTING_READ(reg);
	udelay(200);
}

static void intel_enable_transcoder(struct drm_i915_private *dev_priv,
				    enum pipe pipe)
{
	int reg;
<<<<<<< HEAD
	u32 val;
=======
	u32 val, pipeconf_val;
	struct drm_crtc *crtc = dev_priv->pipe_to_crtc_mapping[pipe];
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void ironlake_enable_shared_dpll(struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = crtc->base.dev->dev_private;
	struct intel_shared_dpll *pll = intel_crtc_to_shared_dpll(crtc);

	/* PCH PLLs only available on ILK, SNB and IVB */
	BUG_ON(dev_priv->info->gen < 5);
	if (WARN_ON(pll == NULL))
		return;

	if (WARN_ON(pll->refcount == 0))
		return;

	DRM_DEBUG_KMS("enable %s (active %d, on? %d)for crtc %d\n",
		      pll->name, pll->active, pll->on,
		      crtc->base.base.id);

	if (pll->active++) {
		WARN_ON(!pll->on);
		assert_shared_dpll_enabled(dev_priv, pll);
		return;
	}
	WARN_ON(pll->on);

	DRM_DEBUG_KMS("enabling %s\n", pll->name);
	pll->enable(dev_priv, pll);
	pll->on = true;
}

static void intel_disable_shared_dpll(struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = crtc->base.dev->dev_private;
	struct intel_shared_dpll *pll = intel_crtc_to_shared_dpll(crtc);

	/* PCH only available on ILK+ */
	BUG_ON(dev_priv->info->gen < 5);
	if (WARN_ON(pll == NULL))
	       return;

	if (WARN_ON(pll->refcount == 0))
		return;

	DRM_DEBUG_KMS("disable %s (active %d, on? %d) for crtc %d\n",
		      pll->name, pll->active, pll->on,
		      crtc->base.base.id);

	if (WARN_ON(pll->active == 0)) {
		assert_shared_dpll_disabled(dev_priv, pll);
		return;
	}

	assert_shared_dpll_enabled(dev_priv, pll);
	WARN_ON(!pll->on);
	if (--pll->active)
		return;

	DRM_DEBUG_KMS("disabling %s\n", pll->name);
	pll->disable(dev_priv, pll);
	pll->on = false;
}

static void ironlake_enable_pch_transcoder(struct drm_i915_private *dev_priv,
					   enum pipe pipe)
{
	struct drm_device *dev = dev_priv->dev;
	struct drm_crtc *crtc = dev_priv->pipe_to_crtc_mapping[pipe];
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	uint32_t reg, val, pipeconf_val;
>>>>>>> refs/remotes/origin/master

	/* PCH only available on ILK+ */
	BUG_ON(dev_priv->info->gen < 5);

	/* Make sure PCH DPLL is enabled */
<<<<<<< HEAD
	assert_pch_pll_enabled(dev_priv, pipe);
=======
	assert_shared_dpll_enabled(dev_priv,
				   intel_crtc_to_shared_dpll(intel_crtc));
>>>>>>> refs/remotes/origin/master

	/* FDI must be feeding us bits for PCH ports */
	assert_fdi_tx_enabled(dev_priv, pipe);
	assert_fdi_rx_enabled(dev_priv, pipe);

<<<<<<< HEAD
	reg = TRANSCONF(pipe);
	val = I915_READ(reg);
<<<<<<< HEAD
	/*
	 * make the BPC in transcoder be consistent with
	 * that in pipeconf reg.
	 */
	val &= ~PIPE_BPC_MASK;
	val |= I915_READ(PIPECONF(pipe)) & PIPE_BPC_MASK;
=======
=======
	if (HAS_PCH_CPT(dev)) {
		/* Workaround: Set the timing override bit before enabling the
		 * pch transcoder. */
		reg = TRANS_CHICKEN2(pipe);
		val = I915_READ(reg);
		val |= TRANS_CHICKEN2_TIMING_OVERRIDE;
		I915_WRITE(reg, val);
	}

	reg = PCH_TRANSCONF(pipe);
	val = I915_READ(reg);
>>>>>>> refs/remotes/origin/master
	pipeconf_val = I915_READ(PIPECONF(pipe));

	if (HAS_PCH_IBX(dev_priv->dev)) {
		/*
		 * make the BPC in transcoder be consistent with
		 * that in pipeconf reg.
		 */
<<<<<<< HEAD
		val &= ~PIPE_BPC_MASK;
		val |= pipeconf_val & PIPE_BPC_MASK;
=======
		val &= ~PIPECONF_BPC_MASK;
		val |= pipeconf_val & PIPECONF_BPC_MASK;
>>>>>>> refs/remotes/origin/master
	}

	val &= ~TRANS_INTERLACE_MASK;
	if ((pipeconf_val & PIPECONF_INTERLACE_MASK) == PIPECONF_INTERLACED_ILK)
		if (HAS_PCH_IBX(dev_priv->dev) &&
		    intel_pipe_has_type(crtc, INTEL_OUTPUT_SDVO))
			val |= TRANS_LEGACY_INTERLACED_ILK;
		else
			val |= TRANS_INTERLACED;
	else
		val |= TRANS_PROGRESSIVE;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	I915_WRITE(reg, val | TRANS_ENABLE);
	if (wait_for(I915_READ(reg) & TRANS_STATE_ENABLE, 100))
		DRM_ERROR("failed to enable transcoder %d\n", pipe);
}

static void intel_disable_transcoder(struct drm_i915_private *dev_priv,
				     enum pipe pipe)
{
	int reg;
	u32 val;
=======
	I915_WRITE(reg, val | TRANS_ENABLE);
	if (wait_for(I915_READ(reg) & TRANS_STATE_ENABLE, 100))
		DRM_ERROR("failed to enable transcoder %c\n", pipe_name(pipe));
}

static void lpt_enable_pch_transcoder(struct drm_i915_private *dev_priv,
				      enum transcoder cpu_transcoder)
{
	u32 val, pipeconf_val;

	/* PCH only available on ILK+ */
	BUG_ON(dev_priv->info->gen < 5);

	/* FDI must be feeding us bits for PCH ports */
	assert_fdi_tx_enabled(dev_priv, (enum pipe) cpu_transcoder);
	assert_fdi_rx_enabled(dev_priv, TRANSCODER_A);

	/* Workaround: set timing override bit. */
	val = I915_READ(_TRANSA_CHICKEN2);
	val |= TRANS_CHICKEN2_TIMING_OVERRIDE;
	I915_WRITE(_TRANSA_CHICKEN2, val);

	val = TRANS_ENABLE;
	pipeconf_val = I915_READ(PIPECONF(cpu_transcoder));

	if ((pipeconf_val & PIPECONF_INTERLACE_MASK_HSW) ==
	    PIPECONF_INTERLACED_ILK)
		val |= TRANS_INTERLACED;
	else
		val |= TRANS_PROGRESSIVE;

	I915_WRITE(LPT_TRANSCONF, val);
	if (wait_for(I915_READ(LPT_TRANSCONF) & TRANS_STATE_ENABLE, 100))
		DRM_ERROR("Failed to enable PCH transcoder\n");
}

static void ironlake_disable_pch_transcoder(struct drm_i915_private *dev_priv,
					    enum pipe pipe)
{
	struct drm_device *dev = dev_priv->dev;
	uint32_t reg, val;
>>>>>>> refs/remotes/origin/master

	/* FDI relies on the transcoder */
	assert_fdi_tx_disabled(dev_priv, pipe);
	assert_fdi_rx_disabled(dev_priv, pipe);

	/* Ports must be off as well */
	assert_pch_ports_disabled(dev_priv, pipe);

<<<<<<< HEAD
	reg = TRANSCONF(pipe);
=======
	reg = PCH_TRANSCONF(pipe);
>>>>>>> refs/remotes/origin/master
	val = I915_READ(reg);
	val &= ~TRANS_ENABLE;
	I915_WRITE(reg, val);
	/* wait for PCH transcoder off, transcoder state */
	if (wait_for((I915_READ(reg) & TRANS_STATE_ENABLE) == 0, 50))
<<<<<<< HEAD
<<<<<<< HEAD
		DRM_ERROR("failed to disable transcoder\n");
=======
		DRM_ERROR("failed to disable transcoder %d\n", pipe);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		DRM_ERROR("failed to disable transcoder %c\n", pipe_name(pipe));

	if (!HAS_PCH_IBX(dev)) {
		/* Workaround: Clear the timing override chicken bit again. */
		reg = TRANS_CHICKEN2(pipe);
		val = I915_READ(reg);
		val &= ~TRANS_CHICKEN2_TIMING_OVERRIDE;
		I915_WRITE(reg, val);
	}
}

static void lpt_disable_pch_transcoder(struct drm_i915_private *dev_priv)
{
	u32 val;

	val = I915_READ(LPT_TRANSCONF);
	val &= ~TRANS_ENABLE;
	I915_WRITE(LPT_TRANSCONF, val);
	/* wait for PCH transcoder off, transcoder state */
	if (wait_for((I915_READ(LPT_TRANSCONF) & TRANS_STATE_ENABLE) == 0, 50))
		DRM_ERROR("Failed to disable PCH transcoder\n");

	/* Workaround: clear timing override bit. */
	val = I915_READ(_TRANSA_CHICKEN2);
	val &= ~TRANS_CHICKEN2_TIMING_OVERRIDE;
	I915_WRITE(_TRANSA_CHICKEN2, val);
>>>>>>> refs/remotes/origin/master
}

/**
 * intel_enable_pipe - enable a pipe, asserting requirements
 * @dev_priv: i915 private structure
 * @pipe: pipe to enable
 * @pch_port: on ILK+, is this pipe driving a PCH port or not
 *
 * Enable @pipe, making sure that various hardware specific requirements
 * are met, if applicable, e.g. PLL enabled, LVDS pairs enabled, etc.
 *
 * @pipe should be %PIPE_A or %PIPE_B.
 *
 * Will wait until the pipe is actually running (i.e. first vblank) before
 * returning.
 */
static void intel_enable_pipe(struct drm_i915_private *dev_priv, enum pipe pipe,
<<<<<<< HEAD
			      bool pch_port)
{
	int reg;
	u32 val;

=======
			      bool pch_port, bool dsi)
{
	enum transcoder cpu_transcoder = intel_pipe_to_cpu_transcoder(dev_priv,
								      pipe);
	enum pipe pch_transcoder;
	int reg;
	u32 val;

	assert_planes_disabled(dev_priv, pipe);
	assert_cursor_disabled(dev_priv, pipe);
	assert_sprites_disabled(dev_priv, pipe);

	if (HAS_PCH_LPT(dev_priv->dev))
		pch_transcoder = TRANSCODER_A;
	else
		pch_transcoder = pipe;

>>>>>>> refs/remotes/origin/master
	/*
	 * A pipe without a PLL won't actually be able to drive bits from
	 * a plane.  On ILK+ the pipe PLLs are integrated, so we don't
	 * need the check.
	 */
	if (!HAS_PCH_SPLIT(dev_priv->dev))
<<<<<<< HEAD
		assert_pll_enabled(dev_priv, pipe);
	else {
		if (pch_port) {
			/* if driving the PCH, we need FDI enabled */
			assert_fdi_rx_pll_enabled(dev_priv, pipe);
			assert_fdi_tx_pll_enabled(dev_priv, pipe);
=======
		if (dsi)
			assert_dsi_pll_enabled(dev_priv);
		else
			assert_pll_enabled(dev_priv, pipe);
	else {
		if (pch_port) {
			/* if driving the PCH, we need FDI enabled */
			assert_fdi_rx_pll_enabled(dev_priv, pch_transcoder);
			assert_fdi_tx_pll_enabled(dev_priv,
						  (enum pipe) cpu_transcoder);
>>>>>>> refs/remotes/origin/master
		}
		/* FIXME: assert CPU port conditions for SNB+ */
	}

<<<<<<< HEAD
	reg = PIPECONF(pipe);
=======
	reg = PIPECONF(cpu_transcoder);
>>>>>>> refs/remotes/origin/master
	val = I915_READ(reg);
	if (val & PIPECONF_ENABLE)
		return;

	I915_WRITE(reg, val | PIPECONF_ENABLE);
	intel_wait_for_vblank(dev_priv->dev, pipe);
}

/**
 * intel_disable_pipe - disable a pipe, asserting requirements
 * @dev_priv: i915 private structure
 * @pipe: pipe to disable
 *
 * Disable @pipe, making sure that various hardware specific requirements
 * are met, if applicable, e.g. plane disabled, panel fitter off, etc.
 *
 * @pipe should be %PIPE_A or %PIPE_B.
 *
 * Will wait until the pipe has shut down before returning.
 */
static void intel_disable_pipe(struct drm_i915_private *dev_priv,
			       enum pipe pipe)
{
<<<<<<< HEAD
=======
	enum transcoder cpu_transcoder = intel_pipe_to_cpu_transcoder(dev_priv,
								      pipe);
>>>>>>> refs/remotes/origin/master
	int reg;
	u32 val;

	/*
	 * Make sure planes won't keep trying to pump pixels to us,
	 * or we might hang the display.
	 */
	assert_planes_disabled(dev_priv, pipe);
<<<<<<< HEAD
=======
	assert_cursor_disabled(dev_priv, pipe);
	assert_sprites_disabled(dev_priv, pipe);
>>>>>>> refs/remotes/origin/master

	/* Don't disable pipe A or pipe A PLLs if needed */
	if (pipe == PIPE_A && (dev_priv->quirks & QUIRK_PIPEA_FORCE))
		return;

<<<<<<< HEAD
	reg = PIPECONF(pipe);
=======
	reg = PIPECONF(cpu_transcoder);
>>>>>>> refs/remotes/origin/master
	val = I915_READ(reg);
	if ((val & PIPECONF_ENABLE) == 0)
		return;

	I915_WRITE(reg, val & ~PIPECONF_ENABLE);
	intel_wait_for_pipe_off(dev_priv->dev, pipe);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * Plane regs are double buffered, going from enabled->disabled needs a
 * trigger in order to latch.  The display address reg provides this.
 */
<<<<<<< HEAD
static void intel_flush_display_plane(struct drm_i915_private *dev_priv,
				      enum plane plane)
{
	I915_WRITE(DSPADDR(plane), I915_READ(DSPADDR(plane)));
	I915_WRITE(DSPSURF(plane), I915_READ(DSPSURF(plane)));
}

>>>>>>> refs/remotes/origin/cm-10.0
/**
 * intel_enable_plane - enable a display plane on a given pipe
=======
void intel_flush_primary_plane(struct drm_i915_private *dev_priv,
			       enum plane plane)
{
	u32 reg = dev_priv->info->gen >= 4 ? DSPSURF(plane) : DSPADDR(plane);

	I915_WRITE(reg, I915_READ(reg));
	POSTING_READ(reg);
}

/**
 * intel_enable_primary_plane - enable the primary plane on a given pipe
>>>>>>> refs/remotes/origin/master
 * @dev_priv: i915 private structure
 * @plane: plane to enable
 * @pipe: pipe being fed
 *
 * Enable @plane on @pipe, making sure that @pipe is running first.
 */
<<<<<<< HEAD
static void intel_enable_plane(struct drm_i915_private *dev_priv,
			       enum plane plane, enum pipe pipe)
{
=======
static void intel_enable_primary_plane(struct drm_i915_private *dev_priv,
				       enum plane plane, enum pipe pipe)
{
	struct intel_crtc *intel_crtc =
		to_intel_crtc(dev_priv->pipe_to_crtc_mapping[pipe]);
>>>>>>> refs/remotes/origin/master
	int reg;
	u32 val;

	/* If the pipe isn't enabled, we can't pump pixels and may hang */
	assert_pipe_enabled(dev_priv, pipe);

<<<<<<< HEAD
=======
	WARN(intel_crtc->primary_enabled, "Primary plane already enabled\n");

	intel_crtc->primary_enabled = true;

>>>>>>> refs/remotes/origin/master
	reg = DSPCNTR(plane);
	val = I915_READ(reg);
	if (val & DISPLAY_PLANE_ENABLE)
		return;

	I915_WRITE(reg, val | DISPLAY_PLANE_ENABLE);
<<<<<<< HEAD
<<<<<<< HEAD
	intel_wait_for_vblank(dev_priv->dev, pipe);
}

/*
 * Plane regs are double buffered, going from enabled->disabled needs a
 * trigger in order to latch.  The display address reg provides this.
 */
static void intel_flush_display_plane(struct drm_i915_private *dev_priv,
				      enum plane plane)
{
	u32 reg = DSPADDR(plane);
	I915_WRITE(reg, I915_READ(reg));
}

=======
	intel_flush_display_plane(dev_priv, plane);
	intel_wait_for_vblank(dev_priv->dev, pipe);
}

>>>>>>> refs/remotes/origin/cm-10.0
/**
 * intel_disable_plane - disable a display plane
=======
	intel_flush_primary_plane(dev_priv, plane);
	intel_wait_for_vblank(dev_priv->dev, pipe);
}

/**
 * intel_disable_primary_plane - disable the primary plane
>>>>>>> refs/remotes/origin/master
 * @dev_priv: i915 private structure
 * @plane: plane to disable
 * @pipe: pipe consuming the data
 *
 * Disable @plane; should be an independent operation.
 */
<<<<<<< HEAD
static void intel_disable_plane(struct drm_i915_private *dev_priv,
				enum plane plane, enum pipe pipe)
{
	int reg;
	u32 val;

=======
static void intel_disable_primary_plane(struct drm_i915_private *dev_priv,
					enum plane plane, enum pipe pipe)
{
	struct intel_crtc *intel_crtc =
		to_intel_crtc(dev_priv->pipe_to_crtc_mapping[pipe]);
	int reg;
	u32 val;

	WARN(!intel_crtc->primary_enabled, "Primary plane already disabled\n");

	intel_crtc->primary_enabled = false;

>>>>>>> refs/remotes/origin/master
	reg = DSPCNTR(plane);
	val = I915_READ(reg);
	if ((val & DISPLAY_PLANE_ENABLE) == 0)
		return;

	I915_WRITE(reg, val & ~DISPLAY_PLANE_ENABLE);
<<<<<<< HEAD
	intel_flush_display_plane(dev_priv, plane);
	intel_wait_for_vblank(dev_priv->dev, pipe);
}

static void disable_pch_dp(struct drm_i915_private *dev_priv,
<<<<<<< HEAD
			   enum pipe pipe, int reg)
{
	u32 val = I915_READ(reg);
	if (DP_PIPE_ENABLED(val, pipe))
		I915_WRITE(reg, val & ~DP_PORT_EN);
=======
			   enum pipe pipe, int reg, u32 port_sel)
{
	u32 val = I915_READ(reg);
	if (dp_pipe_enabled(dev_priv, pipe, port_sel, val)) {
		DRM_DEBUG_KMS("Disabling pch dp %x on pipe %d\n", reg, pipe);
		I915_WRITE(reg, val & ~DP_PORT_EN);
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

static void disable_pch_hdmi(struct drm_i915_private *dev_priv,
			     enum pipe pipe, int reg)
{
	u32 val = I915_READ(reg);
<<<<<<< HEAD
	if (HDMI_PIPE_ENABLED(val, pipe))
		I915_WRITE(reg, val & ~PORT_ENABLE);
=======
	if (hdmi_pipe_enabled(dev_priv, pipe, val)) {
		DRM_DEBUG_KMS("Disabling pch HDMI %x on pipe %d\n",
			      reg, pipe);
		I915_WRITE(reg, val & ~PORT_ENABLE);
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

/* Disable any ports connected to this transcoder */
static void intel_disable_pch_ports(struct drm_i915_private *dev_priv,
				    enum pipe pipe)
{
	u32 reg, val;

	val = I915_READ(PCH_PP_CONTROL);
	I915_WRITE(PCH_PP_CONTROL, val | PANEL_UNLOCK_REGS);

<<<<<<< HEAD
	disable_pch_dp(dev_priv, pipe, PCH_DP_B);
	disable_pch_dp(dev_priv, pipe, PCH_DP_C);
	disable_pch_dp(dev_priv, pipe, PCH_DP_D);

	reg = PCH_ADPA;
	val = I915_READ(reg);
	if (ADPA_PIPE_ENABLED(val, pipe))
=======
	disable_pch_dp(dev_priv, pipe, PCH_DP_B, TRANS_DP_PORT_SEL_B);
	disable_pch_dp(dev_priv, pipe, PCH_DP_C, TRANS_DP_PORT_SEL_C);
	disable_pch_dp(dev_priv, pipe, PCH_DP_D, TRANS_DP_PORT_SEL_D);

	reg = PCH_ADPA;
	val = I915_READ(reg);
	if (adpa_pipe_enabled(dev_priv, pipe, val))
>>>>>>> refs/remotes/origin/cm-10.0
		I915_WRITE(reg, val & ~ADPA_DAC_ENABLE);

	reg = PCH_LVDS;
	val = I915_READ(reg);
<<<<<<< HEAD
	if (LVDS_PIPE_ENABLED(val, pipe)) {
=======
	if (lvds_pipe_enabled(dev_priv, pipe, val)) {
		DRM_DEBUG_KMS("disable lvds on pipe %d val 0x%08x\n", pipe, val);
>>>>>>> refs/remotes/origin/cm-10.0
		I915_WRITE(reg, val & ~LVDS_PORT_EN);
		POSTING_READ(reg);
		udelay(100);
	}

	disable_pch_hdmi(dev_priv, pipe, HDMIB);
	disable_pch_hdmi(dev_priv, pipe, HDMIC);
	disable_pch_hdmi(dev_priv, pipe, HDMID);
}

<<<<<<< HEAD
=======
static void i8xx_disable_fbc(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 fbc_ctl;

	/* Disable compression */
	fbc_ctl = I915_READ(FBC_CONTROL);
	if ((fbc_ctl & FBC_CTL_EN) == 0)
		return;

	fbc_ctl &= ~FBC_CTL_EN;
	I915_WRITE(FBC_CONTROL, fbc_ctl);

	/* Wait for compressing bit to clear */
	if (wait_for((I915_READ(FBC_STATUS) & FBC_STAT_COMPRESSING) == 0, 10)) {
		DRM_DEBUG_KMS("FBC idle timed out\n");
		return;
	}

	DRM_DEBUG_KMS("disabled FBC\n");
}

>>>>>>> refs/remotes/origin/cm-10.0
static void i8xx_enable_fbc(struct drm_crtc *crtc, unsigned long interval)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_framebuffer *fb = crtc->fb;
	struct intel_framebuffer *intel_fb = to_intel_framebuffer(fb);
	struct drm_i915_gem_object *obj = intel_fb->obj;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
<<<<<<< HEAD
	int plane, i;
	u32 fbc_ctl, fbc_ctl2;

	if (fb->pitch == dev_priv->cfb_pitch &&
	    obj->fence_reg == dev_priv->cfb_fence &&
	    intel_crtc->plane == dev_priv->cfb_plane &&
	    I915_READ(FBC_CONTROL) & FBC_CTL_EN)
		return;

	i8xx_disable_fbc(dev);

	dev_priv->cfb_pitch = dev_priv->cfb_size / FBC_LL_SIZE;

	if (fb->pitch < dev_priv->cfb_pitch)
		dev_priv->cfb_pitch = fb->pitch;

	/* FBC_CTL wants 64B units */
	dev_priv->cfb_pitch = (dev_priv->cfb_pitch / 64) - 1;
	dev_priv->cfb_fence = obj->fence_reg;
	dev_priv->cfb_plane = intel_crtc->plane;
	plane = dev_priv->cfb_plane == 0 ? FBC_CTL_PLANEA : FBC_CTL_PLANEB;
=======
	int cfb_pitch;
	int plane, i;
	u32 fbc_ctl, fbc_ctl2;

	cfb_pitch = dev_priv->cfb_size / FBC_LL_SIZE;
	if (fb->pitches[0] < cfb_pitch)
		cfb_pitch = fb->pitches[0];

	/* FBC_CTL wants 64B units */
	cfb_pitch = (cfb_pitch / 64) - 1;
	plane = intel_crtc->plane == 0 ? FBC_CTL_PLANEA : FBC_CTL_PLANEB;
>>>>>>> refs/remotes/origin/cm-10.0

	/* Clear old tags */
	for (i = 0; i < (FBC_LL_SIZE / 32) + 1; i++)
		I915_WRITE(FBC_TAG + (i * 4), 0);

	/* Set it up... */
<<<<<<< HEAD
	fbc_ctl2 = FBC_CTL_FENCE_DBL | FBC_CTL_IDLE_IMM | plane;
	if (obj->tiling_mode != I915_TILING_NONE)
		fbc_ctl2 |= FBC_CTL_CPU_FENCE;
=======
	fbc_ctl2 = FBC_CTL_FENCE_DBL | FBC_CTL_IDLE_IMM | FBC_CTL_CPU_FENCE;
	fbc_ctl2 |= plane;
>>>>>>> refs/remotes/origin/cm-10.0
	I915_WRITE(FBC_CONTROL2, fbc_ctl2);
	I915_WRITE(FBC_FENCE_OFF, crtc->y);

	/* enable it... */
	fbc_ctl = FBC_CTL_EN | FBC_CTL_PERIODIC;
	if (IS_I945GM(dev))
		fbc_ctl |= FBC_CTL_C3_IDLE; /* 945 needs special SR handling */
<<<<<<< HEAD
	fbc_ctl |= (dev_priv->cfb_pitch & 0xff) << FBC_CTL_STRIDE_SHIFT;
	fbc_ctl |= (interval & 0x2fff) << FBC_CTL_INTERVAL_SHIFT;
	if (obj->tiling_mode != I915_TILING_NONE)
		fbc_ctl |= dev_priv->cfb_fence;
	I915_WRITE(FBC_CONTROL, fbc_ctl);

	DRM_DEBUG_KMS("enabled FBC, pitch %ld, yoff %d, plane %d, ",
		      dev_priv->cfb_pitch, crtc->y, dev_priv->cfb_plane);
}

void i8xx_disable_fbc(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 fbc_ctl;

	/* Disable compression */
	fbc_ctl = I915_READ(FBC_CONTROL);
	if ((fbc_ctl & FBC_CTL_EN) == 0)
		return;

	fbc_ctl &= ~FBC_CTL_EN;
	I915_WRITE(FBC_CONTROL, fbc_ctl);

	/* Wait for compressing bit to clear */
	if (wait_for((I915_READ(FBC_STATUS) & FBC_STAT_COMPRESSING) == 0, 10)) {
		DRM_DEBUG_KMS("FBC idle timed out\n");
		return;
	}

	DRM_DEBUG_KMS("disabled FBC\n");
=======
	fbc_ctl |= (cfb_pitch & 0xff) << FBC_CTL_STRIDE_SHIFT;
	fbc_ctl |= (interval & 0x2fff) << FBC_CTL_INTERVAL_SHIFT;
	fbc_ctl |= obj->fence_reg;
	I915_WRITE(FBC_CONTROL, fbc_ctl);

	DRM_DEBUG_KMS("enabled FBC, pitch %d, yoff %d, plane %d, ",
		      cfb_pitch, crtc->y, intel_crtc->plane);
>>>>>>> refs/remotes/origin/cm-10.0
}

static bool i8xx_fbc_enabled(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	return I915_READ(FBC_CONTROL) & FBC_CTL_EN;
}

static void g4x_enable_fbc(struct drm_crtc *crtc, unsigned long interval)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_framebuffer *fb = crtc->fb;
	struct intel_framebuffer *intel_fb = to_intel_framebuffer(fb);
	struct drm_i915_gem_object *obj = intel_fb->obj;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int plane = intel_crtc->plane == 0 ? DPFC_CTL_PLANEA : DPFC_CTL_PLANEB;
	unsigned long stall_watermark = 200;
	u32 dpfc_ctl;

<<<<<<< HEAD
	dpfc_ctl = I915_READ(DPFC_CONTROL);
	if (dpfc_ctl & DPFC_CTL_EN) {
		if (dev_priv->cfb_pitch == dev_priv->cfb_pitch / 64 - 1 &&
		    dev_priv->cfb_fence == obj->fence_reg &&
		    dev_priv->cfb_plane == intel_crtc->plane &&
		    dev_priv->cfb_y == crtc->y)
			return;

		I915_WRITE(DPFC_CONTROL, dpfc_ctl & ~DPFC_CTL_EN);
		intel_wait_for_vblank(dev, intel_crtc->pipe);
	}

	dev_priv->cfb_pitch = (dev_priv->cfb_pitch / 64) - 1;
	dev_priv->cfb_fence = obj->fence_reg;
	dev_priv->cfb_plane = intel_crtc->plane;
	dev_priv->cfb_y = crtc->y;

	dpfc_ctl = plane | DPFC_SR_EN | DPFC_CTL_LIMIT_1X;
	if (obj->tiling_mode != I915_TILING_NONE) {
		dpfc_ctl |= DPFC_CTL_FENCE_EN | dev_priv->cfb_fence;
		I915_WRITE(DPFC_CHICKEN, DPFC_HT_MODIFY);
	} else {
		I915_WRITE(DPFC_CHICKEN, ~DPFC_HT_MODIFY);
	}
=======
	dpfc_ctl = plane | DPFC_SR_EN | DPFC_CTL_LIMIT_1X;
	dpfc_ctl |= DPFC_CTL_FENCE_EN | obj->fence_reg;
	I915_WRITE(DPFC_CHICKEN, DPFC_HT_MODIFY);
>>>>>>> refs/remotes/origin/cm-10.0

	I915_WRITE(DPFC_RECOMP_CTL, DPFC_RECOMP_STALL_EN |
		   (stall_watermark << DPFC_RECOMP_STALL_WM_SHIFT) |
		   (interval << DPFC_RECOMP_TIMER_COUNT_SHIFT));
	I915_WRITE(DPFC_FENCE_YOFF, crtc->y);

	/* enable it... */
	I915_WRITE(DPFC_CONTROL, I915_READ(DPFC_CONTROL) | DPFC_CTL_EN);

	DRM_DEBUG_KMS("enabled fbc on plane %d\n", intel_crtc->plane);
}

<<<<<<< HEAD
void g4x_disable_fbc(struct drm_device *dev)
=======
static void g4x_disable_fbc(struct drm_device *dev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 dpfc_ctl;

	/* Disable compression */
	dpfc_ctl = I915_READ(DPFC_CONTROL);
	if (dpfc_ctl & DPFC_CTL_EN) {
		dpfc_ctl &= ~DPFC_CTL_EN;
		I915_WRITE(DPFC_CONTROL, dpfc_ctl);

		DRM_DEBUG_KMS("disabled FBC\n");
	}
}

static bool g4x_fbc_enabled(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	return I915_READ(DPFC_CONTROL) & DPFC_CTL_EN;
}

static void sandybridge_blit_fbc_update(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 blt_ecoskpd;

	/* Make sure blitter notifies FBC of writes */
	gen6_gt_force_wake_get(dev_priv);
	blt_ecoskpd = I915_READ(GEN6_BLITTER_ECOSKPD);
	blt_ecoskpd |= GEN6_BLITTER_FBC_NOTIFY <<
		GEN6_BLITTER_LOCK_SHIFT;
	I915_WRITE(GEN6_BLITTER_ECOSKPD, blt_ecoskpd);
	blt_ecoskpd |= GEN6_BLITTER_FBC_NOTIFY;
	I915_WRITE(GEN6_BLITTER_ECOSKPD, blt_ecoskpd);
	blt_ecoskpd &= ~(GEN6_BLITTER_FBC_NOTIFY <<
			 GEN6_BLITTER_LOCK_SHIFT);
	I915_WRITE(GEN6_BLITTER_ECOSKPD, blt_ecoskpd);
	POSTING_READ(GEN6_BLITTER_ECOSKPD);
	gen6_gt_force_wake_put(dev_priv);
}

static void ironlake_enable_fbc(struct drm_crtc *crtc, unsigned long interval)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_framebuffer *fb = crtc->fb;
	struct intel_framebuffer *intel_fb = to_intel_framebuffer(fb);
	struct drm_i915_gem_object *obj = intel_fb->obj;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int plane = intel_crtc->plane == 0 ? DPFC_CTL_PLANEA : DPFC_CTL_PLANEB;
	unsigned long stall_watermark = 200;
	u32 dpfc_ctl;

	dpfc_ctl = I915_READ(ILK_DPFC_CONTROL);
<<<<<<< HEAD
	if (dpfc_ctl & DPFC_CTL_EN) {
		if (dev_priv->cfb_pitch == dev_priv->cfb_pitch / 64 - 1 &&
		    dev_priv->cfb_fence == obj->fence_reg &&
		    dev_priv->cfb_plane == intel_crtc->plane &&
		    dev_priv->cfb_offset == obj->gtt_offset &&
		    dev_priv->cfb_y == crtc->y)
			return;

		I915_WRITE(ILK_DPFC_CONTROL, dpfc_ctl & ~DPFC_CTL_EN);
		intel_wait_for_vblank(dev, intel_crtc->pipe);
	}

	dev_priv->cfb_pitch = (dev_priv->cfb_pitch / 64) - 1;
	dev_priv->cfb_fence = obj->fence_reg;
	dev_priv->cfb_plane = intel_crtc->plane;
	dev_priv->cfb_offset = obj->gtt_offset;
	dev_priv->cfb_y = crtc->y;

	dpfc_ctl &= DPFC_RESERVED;
	dpfc_ctl |= (plane | DPFC_CTL_LIMIT_1X);
	if (obj->tiling_mode != I915_TILING_NONE) {
		dpfc_ctl |= (DPFC_CTL_FENCE_EN | dev_priv->cfb_fence);
		I915_WRITE(ILK_DPFC_CHICKEN, DPFC_HT_MODIFY);
	} else {
		I915_WRITE(ILK_DPFC_CHICKEN, ~DPFC_HT_MODIFY);
	}
=======
	dpfc_ctl &= DPFC_RESERVED;
	dpfc_ctl |= (plane | DPFC_CTL_LIMIT_1X);
	/* Set persistent mode for front-buffer rendering, ala X. */
	dpfc_ctl |= DPFC_CTL_PERSISTENT_MODE;
	dpfc_ctl |= (DPFC_CTL_FENCE_EN | obj->fence_reg);
	I915_WRITE(ILK_DPFC_CHICKEN, DPFC_HT_MODIFY);
>>>>>>> refs/remotes/origin/cm-10.0

	I915_WRITE(ILK_DPFC_RECOMP_CTL, DPFC_RECOMP_STALL_EN |
		   (stall_watermark << DPFC_RECOMP_STALL_WM_SHIFT) |
		   (interval << DPFC_RECOMP_TIMER_COUNT_SHIFT));
	I915_WRITE(ILK_DPFC_FENCE_YOFF, crtc->y);
	I915_WRITE(ILK_FBC_RT_BASE, obj->gtt_offset | ILK_FBC_RT_VALID);
	/* enable it... */
	I915_WRITE(ILK_DPFC_CONTROL, dpfc_ctl | DPFC_CTL_EN);

	if (IS_GEN6(dev)) {
		I915_WRITE(SNB_DPFC_CTL_SA,
<<<<<<< HEAD
			   SNB_CPU_FENCE_ENABLE | dev_priv->cfb_fence);
=======
			   SNB_CPU_FENCE_ENABLE | obj->fence_reg);
>>>>>>> refs/remotes/origin/cm-10.0
		I915_WRITE(DPFC_CPU_FENCE_OFFSET, crtc->y);
		sandybridge_blit_fbc_update(dev);
	}

	DRM_DEBUG_KMS("enabled fbc on plane %d\n", intel_crtc->plane);
}

<<<<<<< HEAD
void ironlake_disable_fbc(struct drm_device *dev)
=======
static void ironlake_disable_fbc(struct drm_device *dev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 dpfc_ctl;

	/* Disable compression */
	dpfc_ctl = I915_READ(ILK_DPFC_CONTROL);
	if (dpfc_ctl & DPFC_CTL_EN) {
		dpfc_ctl &= ~DPFC_CTL_EN;
		I915_WRITE(ILK_DPFC_CONTROL, dpfc_ctl);

		DRM_DEBUG_KMS("disabled FBC\n");
	}
}

static bool ironlake_fbc_enabled(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	return I915_READ(ILK_DPFC_CONTROL) & DPFC_CTL_EN;
}

bool intel_fbc_enabled(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (!dev_priv->display.fbc_enabled)
		return false;

	return dev_priv->display.fbc_enabled(dev);
}

<<<<<<< HEAD
void intel_enable_fbc(struct drm_crtc *crtc, unsigned long interval)
{
	struct drm_i915_private *dev_priv = crtc->dev->dev_private;
=======
static void intel_fbc_work_fn(struct work_struct *__work)
{
	struct intel_fbc_work *work =
		container_of(to_delayed_work(__work),
			     struct intel_fbc_work, work);
	struct drm_device *dev = work->crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	mutex_lock(&dev->struct_mutex);
	if (work == dev_priv->fbc_work) {
		/* Double check that we haven't switched fb without cancelling
		 * the prior work.
		 */
		if (work->crtc->fb == work->fb) {
			dev_priv->display.enable_fbc(work->crtc,
						     work->interval);

			dev_priv->cfb_plane = to_intel_crtc(work->crtc)->plane;
			dev_priv->cfb_fb = work->crtc->fb->base.id;
			dev_priv->cfb_y = work->crtc->y;
		}

		dev_priv->fbc_work = NULL;
	}
	mutex_unlock(&dev->struct_mutex);

	kfree(work);
}

static void intel_cancel_fbc_work(struct drm_i915_private *dev_priv)
{
	if (dev_priv->fbc_work == NULL)
		return;

	DRM_DEBUG_KMS("cancelling pending FBC enable\n");

	/* Synchronisation is provided by struct_mutex and checking of
	 * dev_priv->fbc_work, so we can perform the cancellation
	 * entirely asynchronously.
	 */
	if (cancel_delayed_work(&dev_priv->fbc_work->work))
		/* tasklet was killed before being run, clean up */
		kfree(dev_priv->fbc_work);

	/* Mark the work as no longer wanted so that if it does
	 * wake-up (because the work was already running and waiting
	 * for our mutex), it will discover that is no longer
	 * necessary to run.
	 */
	dev_priv->fbc_work = NULL;
}

static void intel_enable_fbc(struct drm_crtc *crtc, unsigned long interval)
{
	struct intel_fbc_work *work;
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!dev_priv->display.enable_fbc)
		return;

<<<<<<< HEAD
	dev_priv->display.enable_fbc(crtc, interval);
=======
	intel_cancel_fbc_work(dev_priv);

	work = kzalloc(sizeof *work, GFP_KERNEL);
	if (work == NULL) {
		dev_priv->display.enable_fbc(crtc, interval);
		return;
	}

	work->crtc = crtc;
	work->fb = crtc->fb;
	work->interval = interval;
	INIT_DELAYED_WORK(&work->work, intel_fbc_work_fn);

	dev_priv->fbc_work = work;

	DRM_DEBUG_KMS("scheduling delayed FBC enable\n");

	/* Delay the actual enabling to let pageflipping cease and the
	 * display to settle before starting the compression. Note that
	 * this delay also serves a second purpose: it allows for a
	 * vblank to pass after disabling the FBC before we attempt
	 * to modify the control registers.
	 *
	 * A more complicated solution would involve tracking vblanks
	 * following the termination of the page-flipping sequence
	 * and indeed performing the enable as a co-routine and not
	 * waiting synchronously upon the vblank.
	 */
	schedule_delayed_work(&work->work, msecs_to_jiffies(50));
>>>>>>> refs/remotes/origin/cm-10.0
}

void intel_disable_fbc(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

<<<<<<< HEAD
=======
	intel_cancel_fbc_work(dev_priv);

>>>>>>> refs/remotes/origin/cm-10.0
	if (!dev_priv->display.disable_fbc)
		return;

	dev_priv->display.disable_fbc(dev);
<<<<<<< HEAD
=======
	dev_priv->cfb_plane = -1;
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * intel_update_fbc - enable/disable FBC as needed
 * @dev: the drm_device
 *
 * Set up the framebuffer compression hardware at mode set time.  We
 * enable it if possible:
 *   - plane A only (on pre-965)
 *   - no pixel mulitply/line duplication
 *   - no alpha buffer discard
 *   - no dual wide
 *   - framebuffer <= 2048 in width, 1536 in height
 *
 * We can't assume that any compression will take place (worst case),
 * so the compressed buffer has to be the same size as the uncompressed
 * one.  It also must reside (along with the line length buffer) in
 * stolen memory.
 *
 * We need to enable/disable FBC on a global basis.
 */
static void intel_update_fbc(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_crtc *crtc = NULL, *tmp_crtc;
	struct intel_crtc *intel_crtc;
	struct drm_framebuffer *fb;
	struct intel_framebuffer *intel_fb;
	struct drm_i915_gem_object *obj;
<<<<<<< HEAD
=======
	int enable_fbc;
>>>>>>> refs/remotes/origin/cm-10.0

	DRM_DEBUG_KMS("\n");

	if (!i915_powersave)
		return;

	if (!I915_HAS_FBC(dev))
		return;

	/*
	 * If FBC is already on, we just have to verify that we can
	 * keep it that way...
	 * Need to disable if:
	 *   - more than one pipe is active
	 *   - changing FBC params (stride, fence, mode)
	 *   - new fb is too large to fit in compressed buffer
	 *   - going to an unsupported config (interlace, pixel multiply, etc.)
	 */
	list_for_each_entry(tmp_crtc, &dev->mode_config.crtc_list, head) {
		if (tmp_crtc->enabled && tmp_crtc->fb) {
			if (crtc) {
				DRM_DEBUG_KMS("more than one pipe active, disabling compression\n");
				dev_priv->no_fbc_reason = FBC_MULTIPLE_PIPES;
				goto out_disable;
			}
			crtc = tmp_crtc;
		}
	}

	if (!crtc || crtc->fb == NULL) {
		DRM_DEBUG_KMS("no output, disabling\n");
		dev_priv->no_fbc_reason = FBC_NO_OUTPUT;
		goto out_disable;
	}

	intel_crtc = to_intel_crtc(crtc);
	fb = crtc->fb;
	intel_fb = to_intel_framebuffer(fb);
	obj = intel_fb->obj;

<<<<<<< HEAD
	if (!i915_enable_fbc) {
		DRM_DEBUG_KMS("fbc disabled per module param (default off)\n");
=======
	enable_fbc = i915_enable_fbc;
	if (enable_fbc < 0) {
		DRM_DEBUG_KMS("fbc set to per-chip default\n");
		enable_fbc = 1;
		if (INTEL_INFO(dev)->gen <= 6)
			enable_fbc = 0;
	}
	if (!enable_fbc) {
		DRM_DEBUG_KMS("fbc disabled per module param\n");
>>>>>>> refs/remotes/origin/cm-10.0
		dev_priv->no_fbc_reason = FBC_MODULE_PARAM;
		goto out_disable;
	}
	if (intel_fb->obj->base.size > dev_priv->cfb_size) {
		DRM_DEBUG_KMS("framebuffer too large, disabling "
			      "compression\n");
		dev_priv->no_fbc_reason = FBC_STOLEN_TOO_SMALL;
		goto out_disable;
	}
	if ((crtc->mode.flags & DRM_MODE_FLAG_INTERLACE) ||
	    (crtc->mode.flags & DRM_MODE_FLAG_DBLSCAN)) {
		DRM_DEBUG_KMS("mode incompatible with compression, "
			      "disabling\n");
		dev_priv->no_fbc_reason = FBC_UNSUPPORTED_MODE;
		goto out_disable;
	}
	if ((crtc->mode.hdisplay > 2048) ||
	    (crtc->mode.vdisplay > 1536)) {
		DRM_DEBUG_KMS("mode too large for compression, disabling\n");
		dev_priv->no_fbc_reason = FBC_MODE_TOO_LARGE;
		goto out_disable;
	}
	if ((IS_I915GM(dev) || IS_I945GM(dev)) && intel_crtc->plane != 0) {
		DRM_DEBUG_KMS("plane not 0, disabling compression\n");
		dev_priv->no_fbc_reason = FBC_BAD_PLANE;
		goto out_disable;
	}
<<<<<<< HEAD
	if (obj->tiling_mode != I915_TILING_X) {
		DRM_DEBUG_KMS("framebuffer not tiled, disabling compression\n");
=======

	/* The use of a CPU fence is mandatory in order to detect writes
	 * by the CPU to the scanout and trigger updates to the FBC.
	 */
	if (obj->tiling_mode != I915_TILING_X ||
	    obj->fence_reg == I915_FENCE_REG_NONE) {
		DRM_DEBUG_KMS("framebuffer not tiled or fenced, disabling compression\n");
>>>>>>> refs/remotes/origin/cm-10.0
		dev_priv->no_fbc_reason = FBC_NOT_TILED;
		goto out_disable;
	}

	/* If the kernel debugger is active, always disable compression */
	if (in_dbg_master())
		goto out_disable;

<<<<<<< HEAD
=======
	/* If the scanout has not changed, don't modify the FBC settings.
	 * Note that we make the fundamental assumption that the fb->obj
	 * cannot be unpinned (and have its GTT offset and fence revoked)
	 * without first being decoupled from the scanout and FBC disabled.
	 */
	if (dev_priv->cfb_plane == intel_crtc->plane &&
	    dev_priv->cfb_fb == fb->base.id &&
	    dev_priv->cfb_y == crtc->y)
		return;

	if (intel_fbc_enabled(dev)) {
		/* We update FBC along two paths, after changing fb/crtc
		 * configuration (modeswitching) and after page-flipping
		 * finishes. For the latter, we know that not only did
		 * we disable the FBC at the start of the page-flip
		 * sequence, but also more than one vblank has passed.
		 *
		 * For the former case of modeswitching, it is possible
		 * to switch between two FBC valid configurations
		 * instantaneously so we do need to disable the FBC
		 * before we can modify its control registers. We also
		 * have to wait for the next vblank for that to take
		 * effect. However, since we delay enabling FBC we can
		 * assume that a vblank has passed since disabling and
		 * that we can safely alter the registers in the deferred
		 * callback.
		 *
		 * In the scenario that we go from a valid to invalid
		 * and then back to valid FBC configuration we have
		 * no strict enforcement that a vblank occurred since
		 * disabling the FBC. However, along all current pipe
		 * disabling paths we do need to wait for a vblank at
		 * some point. And we wait before enabling FBC anyway.
		 */
		DRM_DEBUG_KMS("disabling active FBC for update\n");
		intel_disable_fbc(dev);
	}

>>>>>>> refs/remotes/origin/cm-10.0
	intel_enable_fbc(crtc, 500);
	return;

out_disable:
	/* Multiple disables should be harmless */
	if (intel_fbc_enabled(dev)) {
		DRM_DEBUG_KMS("unsupported config, disabling FBC\n");
		intel_disable_fbc(dev);
	}
}

int
intel_pin_and_fence_fb_obj(struct drm_device *dev,
			   struct drm_i915_gem_object *obj,
			   struct intel_ring_buffer *pipelined)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 alignment;
	int ret;

	switch (obj->tiling_mode) {
	case I915_TILING_NONE:
		if (IS_BROADWATER(dev) || IS_CRESTLINE(dev))
			alignment = 128 * 1024;
		else if (INTEL_INFO(dev)->gen >= 4)
			alignment = 4 * 1024;
		else
			alignment = 64 * 1024;
		break;
	case I915_TILING_X:
		/* pin() will align the object as required by fence */
		alignment = 0;
		break;
	case I915_TILING_Y:
		/* FIXME: Is this true? */
		DRM_ERROR("Y tiled not allowed for scan out buffers\n");
		return -EINVAL;
	default:
		BUG();
	}

	dev_priv->mm.interruptible = false;
<<<<<<< HEAD
	ret = i915_gem_object_pin(obj, alignment, true);
	if (ret)
		goto err_interruptible;

	ret = i915_gem_object_set_to_display_plane(obj, pipelined);
	if (ret)
		goto err_unpin;

=======
	ret = i915_gem_object_pin_to_display_plane(obj, alignment, pipelined);
	if (ret)
		goto err_interruptible;

>>>>>>> refs/remotes/origin/cm-10.0
	/* Install a fence for tiled scan-out. Pre-i965 always needs a
	 * fence, whereas 965+ only requires a fence if using
	 * framebuffer compression.  For simplicity, we always install
	 * a fence as the cost is not that onerous.
	 */
	if (obj->tiling_mode != I915_TILING_NONE) {
		ret = i915_gem_object_get_fence(obj, pipelined);
		if (ret)
			goto err_unpin;
<<<<<<< HEAD
=======

		i915_gem_object_pin_fence(obj);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	dev_priv->mm.interruptible = true;
	return 0;

err_unpin:
	i915_gem_object_unpin(obj);
err_interruptible:
	dev_priv->mm.interruptible = true;
	return ret;
}

<<<<<<< HEAD
/* Assume fb object is pinned & idle & fenced and just update base pointers */
static int
intel_pipe_set_base_atomic(struct drm_crtc *crtc, struct drm_framebuffer *fb,
			   int x, int y, enum mode_set_atomic state)
=======
void intel_unpin_fb_obj(struct drm_i915_gem_object *obj)
{
	i915_gem_object_unpin_fence(obj);
	i915_gem_object_unpin(obj);
}

static int i9xx_update_plane(struct drm_crtc *crtc, struct drm_framebuffer *fb,
			     int x, int y)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_framebuffer *intel_fb;
	struct drm_i915_gem_object *obj;
	int plane = intel_crtc->plane;
	unsigned long Start, Offset;
	u32 dspcntr;
	u32 reg;

	switch (plane) {
	case 0:
	case 1:
		break;
	default:
		DRM_ERROR("Can't update plane %d in SAREA\n", plane);
		return -EINVAL;
	}

	intel_fb = to_intel_framebuffer(fb);
	obj = intel_fb->obj;

	reg = DSPCNTR(plane);
	dspcntr = I915_READ(reg);
	/* Mask out pixel format bits in case we change it */
	dspcntr &= ~DISPPLANE_PIXFORMAT_MASK;
	switch (fb->bits_per_pixel) {
	case 8:
		dspcntr |= DISPPLANE_8BPP;
		break;
	case 16:
		if (fb->depth == 15)
			dspcntr |= DISPPLANE_15_16BPP;
		else
			dspcntr |= DISPPLANE_16BPP;
		break;
	case 24:
	case 32:
		dspcntr |= DISPPLANE_32BPP_NO_ALPHA;
		break;
	default:
<<<<<<< HEAD
		DRM_ERROR("Unknown color depth\n");
=======
		DRM_ERROR("Unknown color depth %d\n", fb->bits_per_pixel);
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;
	}
	if (INTEL_INFO(dev)->gen >= 4) {
		if (obj->tiling_mode != I915_TILING_NONE)
			dspcntr |= DISPPLANE_TILED;
		else
			dspcntr &= ~DISPPLANE_TILED;
	}

<<<<<<< HEAD
	if (HAS_PCH_SPLIT(dev))
		/* must disable */
		dspcntr |= DISPPLANE_TRICKLE_FEED_DISABLE;

	I915_WRITE(reg, dspcntr);

	Start = obj->gtt_offset;
	Offset = y * fb->pitch + x * (fb->bits_per_pixel / 8);

	DRM_DEBUG_KMS("Writing base %08lX %08lX %d %d %d\n",
		      Start, Offset, x, y, fb->pitch);
	I915_WRITE(DSPSTRIDE(plane), fb->pitch);
=======
	I915_WRITE(reg, dspcntr);

	Start = obj->gtt_offset;
	Offset = y * fb->pitches[0] + x * (fb->bits_per_pixel / 8);

	DRM_DEBUG_KMS("Writing base %08lX %08lX %d %d %d\n",
		      Start, Offset, x, y, fb->pitches[0]);
	I915_WRITE(DSPSTRIDE(plane), fb->pitches[0]);
>>>>>>> refs/remotes/origin/cm-10.0
	if (INTEL_INFO(dev)->gen >= 4) {
		I915_WRITE(DSPSURF(plane), Start);
		I915_WRITE(DSPTILEOFF(plane), (y << 16) | x);
		I915_WRITE(DSPADDR(plane), Offset);
	} else
		I915_WRITE(DSPADDR(plane), Start + Offset);
	POSTING_READ(reg);

<<<<<<< HEAD
=======
	return 0;
}

static int ironlake_update_plane(struct drm_crtc *crtc,
				 struct drm_framebuffer *fb, int x, int y)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_framebuffer *intel_fb;
	struct drm_i915_gem_object *obj;
	int plane = intel_crtc->plane;
	unsigned long Start, Offset;
	u32 dspcntr;
	u32 reg;

	switch (plane) {
	case 0:
	case 1:
	case 2:
		break;
	default:
		DRM_ERROR("Can't update plane %d in SAREA\n", plane);
		return -EINVAL;
	}

	intel_fb = to_intel_framebuffer(fb);
	obj = intel_fb->obj;

	reg = DSPCNTR(plane);
	dspcntr = I915_READ(reg);
	/* Mask out pixel format bits in case we change it */
	dspcntr &= ~DISPPLANE_PIXFORMAT_MASK;
	switch (fb->bits_per_pixel) {
	case 8:
		dspcntr |= DISPPLANE_8BPP;
		break;
	case 16:
		if (fb->depth != 16)
			return -EINVAL;

		dspcntr |= DISPPLANE_16BPP;
		break;
	case 24:
	case 32:
		if (fb->depth == 24)
			dspcntr |= DISPPLANE_32BPP_NO_ALPHA;
		else if (fb->depth == 30)
			dspcntr |= DISPPLANE_32BPP_30BIT_NO_ALPHA;
		else
			return -EINVAL;
		break;
	default:
		DRM_ERROR("Unknown color depth %d\n", fb->bits_per_pixel);
		return -EINVAL;
	}

	if (obj->tiling_mode != I915_TILING_NONE)
		dspcntr |= DISPPLANE_TILED;
	else
		dspcntr &= ~DISPPLANE_TILED;

	/* must disable */
	dspcntr |= DISPPLANE_TRICKLE_FEED_DISABLE;

	I915_WRITE(reg, dspcntr);

	Start = obj->gtt_offset;
	Offset = y * fb->pitches[0] + x * (fb->bits_per_pixel / 8);

	DRM_DEBUG_KMS("Writing base %08lX %08lX %d %d %d\n",
		      Start, Offset, x, y, fb->pitches[0]);
	I915_WRITE(DSPSTRIDE(plane), fb->pitches[0]);
	I915_WRITE(DSPSURF(plane), Start);
	I915_WRITE(DSPTILEOFF(plane), (y << 16) | x);
	I915_WRITE(DSPADDR(plane), Offset);
	POSTING_READ(reg);

	return 0;
}

/* Assume fb object is pinned & idle & fenced and just update base pointers */
static int
intel_pipe_set_base_atomic(struct drm_crtc *crtc, struct drm_framebuffer *fb,
			   int x, int y, enum mode_set_atomic state)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret;

	ret = dev_priv->display.update_plane(crtc, fb, x, y);
	if (ret)
		return ret;

>>>>>>> refs/remotes/origin/cm-10.0
	intel_update_fbc(dev);
	intel_increase_pllclock(crtc);

	return 0;
}

static int
<<<<<<< HEAD
=======
intel_finish_fb(struct drm_framebuffer *old_fb)
{
	struct drm_i915_gem_object *obj = to_intel_framebuffer(old_fb)->obj;
	struct drm_i915_private *dev_priv = obj->base.dev->dev_private;
	bool was_interruptible = dev_priv->mm.interruptible;
	int ret;

	wait_event(dev_priv->pending_flip_queue,
		   atomic_read(&dev_priv->mm.wedged) ||
		   atomic_read(&obj->pending_flip) == 0);

	/* Big Hammer, we also need to ensure that any pending
	 * MI_WAIT_FOR_EVENT inside a user batch buffer on the
	 * current scanout is retired before unpinning the old
	 * framebuffer.
	 *
	 * This should only fail upon a hung GPU, in which case we
	 * can safely continue.
	 */
	dev_priv->mm.interruptible = false;
	ret = i915_gem_object_finish_gpu(obj);
	dev_priv->mm.interruptible = was_interruptible;

	return ret;
}

static int
>>>>>>> refs/remotes/origin/cm-10.0
intel_pipe_set_base(struct drm_crtc *crtc, int x, int y,
		    struct drm_framebuffer *old_fb)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_master_private *master_priv;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int ret;

	/* no fb bound */
	if (!crtc->fb) {
<<<<<<< HEAD
		DRM_DEBUG_KMS("No FB bound\n");
=======
		DRM_ERROR("No FB bound\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return 0;
	}

	switch (intel_crtc->plane) {
	case 0:
	case 1:
		break;
<<<<<<< HEAD
	default:
=======
	case 2:
		if (IS_IVYBRIDGE(dev))
			break;
		/* fall through otherwise */
	default:
		DRM_ERROR("no plane for crtc\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	intel_flush_primary_plane(dev_priv, plane);
	intel_wait_for_vblank(dev_priv->dev, pipe);
}

static bool need_vtd_wa(struct drm_device *dev)
{
#ifdef CONFIG_INTEL_IOMMU
	if (INTEL_INFO(dev)->gen >= 6 && intel_iommu_gfx_mapped)
		return true;
#endif
	return false;
}

int
intel_pin_and_fence_fb_obj(struct drm_device *dev,
			   struct drm_i915_gem_object *obj,
			   struct intel_ring_buffer *pipelined)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 alignment;
	int ret;

	switch (obj->tiling_mode) {
	case I915_TILING_NONE:
		if (IS_BROADWATER(dev) || IS_CRESTLINE(dev))
			alignment = 128 * 1024;
		else if (INTEL_INFO(dev)->gen >= 4)
			alignment = 4 * 1024;
		else
			alignment = 64 * 1024;
		break;
	case I915_TILING_X:
		/* pin() will align the object as required by fence */
		alignment = 0;
		break;
	case I915_TILING_Y:
		WARN(1, "Y tiled bo slipped through, driver bug!\n");
		return -EINVAL;
	default:
		BUG();
	}

	/* Note that the w/a also requires 64 PTE of padding following the
	 * bo. We currently fill all unused PTE with the shadow page and so
	 * we should always have valid PTE following the scanout preventing
	 * the VT-d warning.
	 */
	if (need_vtd_wa(dev) && alignment < 256 * 1024)
		alignment = 256 * 1024;

	dev_priv->mm.interruptible = false;
	ret = i915_gem_object_pin_to_display_plane(obj, alignment, pipelined);
	if (ret)
		goto err_interruptible;

	/* Install a fence for tiled scan-out. Pre-i965 always needs a
	 * fence, whereas 965+ only requires a fence if using
	 * framebuffer compression.  For simplicity, we always install
	 * a fence as the cost is not that onerous.
	 */
	ret = i915_gem_object_get_fence(obj);
	if (ret)
		goto err_unpin;

	i915_gem_object_pin_fence(obj);

	dev_priv->mm.interruptible = true;
	return 0;

err_unpin:
	i915_gem_object_unpin_from_display_plane(obj);
err_interruptible:
	dev_priv->mm.interruptible = true;
	return ret;
}

void intel_unpin_fb_obj(struct drm_i915_gem_object *obj)
{
	i915_gem_object_unpin_fence(obj);
	i915_gem_object_unpin_from_display_plane(obj);
}

/* Computes the linear offset to the base tile and adjusts x, y. bytes per pixel
 * is assumed to be a power-of-two. */
unsigned long intel_gen4_compute_page_offset(int *x, int *y,
					     unsigned int tiling_mode,
					     unsigned int cpp,
					     unsigned int pitch)
{
	if (tiling_mode != I915_TILING_NONE) {
		unsigned int tile_rows, tiles;

		tile_rows = *y / 8;
		*y %= 8;

		tiles = *x / (512/cpp);
		*x %= 512/cpp;

		return tile_rows * pitch * 8 + tiles * 4096;
	} else {
		unsigned int offset;

		offset = *y * pitch + *x * cpp;
		*y = 0;
		*x = (offset & 4095) / cpp;
		return offset & -4096;
	}
}

static int i9xx_update_plane(struct drm_crtc *crtc, struct drm_framebuffer *fb,
			     int x, int y)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_framebuffer *intel_fb;
	struct drm_i915_gem_object *obj;
	int plane = intel_crtc->plane;
	unsigned long linear_offset;
	u32 dspcntr;
	u32 reg;

	switch (plane) {
	case 0:
	case 1:
		break;
	default:
		DRM_ERROR("Can't update plane %c in SAREA\n", plane_name(plane));
		return -EINVAL;
	}

	intel_fb = to_intel_framebuffer(fb);
	obj = intel_fb->obj;

	reg = DSPCNTR(plane);
	dspcntr = I915_READ(reg);
	/* Mask out pixel format bits in case we change it */
	dspcntr &= ~DISPPLANE_PIXFORMAT_MASK;
	switch (fb->pixel_format) {
	case DRM_FORMAT_C8:
		dspcntr |= DISPPLANE_8BPP;
		break;
	case DRM_FORMAT_XRGB1555:
	case DRM_FORMAT_ARGB1555:
		dspcntr |= DISPPLANE_BGRX555;
		break;
	case DRM_FORMAT_RGB565:
		dspcntr |= DISPPLANE_BGRX565;
		break;
	case DRM_FORMAT_XRGB8888:
	case DRM_FORMAT_ARGB8888:
		dspcntr |= DISPPLANE_BGRX888;
		break;
	case DRM_FORMAT_XBGR8888:
	case DRM_FORMAT_ABGR8888:
		dspcntr |= DISPPLANE_RGBX888;
		break;
	case DRM_FORMAT_XRGB2101010:
	case DRM_FORMAT_ARGB2101010:
		dspcntr |= DISPPLANE_BGRX101010;
		break;
	case DRM_FORMAT_XBGR2101010:
	case DRM_FORMAT_ABGR2101010:
		dspcntr |= DISPPLANE_RGBX101010;
		break;
	default:
		BUG();
	}

	if (INTEL_INFO(dev)->gen >= 4) {
		if (obj->tiling_mode != I915_TILING_NONE)
			dspcntr |= DISPPLANE_TILED;
		else
			dspcntr &= ~DISPPLANE_TILED;
	}

	if (IS_G4X(dev))
		dspcntr |= DISPPLANE_TRICKLE_FEED_DISABLE;

	I915_WRITE(reg, dspcntr);

	linear_offset = y * fb->pitches[0] + x * (fb->bits_per_pixel / 8);

	if (INTEL_INFO(dev)->gen >= 4) {
		intel_crtc->dspaddr_offset =
			intel_gen4_compute_page_offset(&x, &y, obj->tiling_mode,
						       fb->bits_per_pixel / 8,
						       fb->pitches[0]);
		linear_offset -= intel_crtc->dspaddr_offset;
	} else {
		intel_crtc->dspaddr_offset = linear_offset;
	}

	DRM_DEBUG_KMS("Writing base %08lX %08lX %d %d %d\n",
		      i915_gem_obj_ggtt_offset(obj), linear_offset, x, y,
		      fb->pitches[0]);
	I915_WRITE(DSPSTRIDE(plane), fb->pitches[0]);
	if (INTEL_INFO(dev)->gen >= 4) {
		I915_MODIFY_DISPBASE(DSPSURF(plane),
				     i915_gem_obj_ggtt_offset(obj) + intel_crtc->dspaddr_offset);
		I915_WRITE(DSPTILEOFF(plane), (y << 16) | x);
		I915_WRITE(DSPLINOFF(plane), linear_offset);
	} else
		I915_WRITE(DSPADDR(plane), i915_gem_obj_ggtt_offset(obj) + linear_offset);
	POSTING_READ(reg);

	return 0;
}

static int ironlake_update_plane(struct drm_crtc *crtc,
				 struct drm_framebuffer *fb, int x, int y)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_framebuffer *intel_fb;
	struct drm_i915_gem_object *obj;
	int plane = intel_crtc->plane;
	unsigned long linear_offset;
	u32 dspcntr;
	u32 reg;

	switch (plane) {
	case 0:
	case 1:
	case 2:
		break;
	default:
		DRM_ERROR("Can't update plane %c in SAREA\n", plane_name(plane));
		return -EINVAL;
	}

	intel_fb = to_intel_framebuffer(fb);
	obj = intel_fb->obj;

	reg = DSPCNTR(plane);
	dspcntr = I915_READ(reg);
	/* Mask out pixel format bits in case we change it */
	dspcntr &= ~DISPPLANE_PIXFORMAT_MASK;
	switch (fb->pixel_format) {
	case DRM_FORMAT_C8:
		dspcntr |= DISPPLANE_8BPP;
		break;
	case DRM_FORMAT_RGB565:
		dspcntr |= DISPPLANE_BGRX565;
		break;
	case DRM_FORMAT_XRGB8888:
	case DRM_FORMAT_ARGB8888:
		dspcntr |= DISPPLANE_BGRX888;
		break;
	case DRM_FORMAT_XBGR8888:
	case DRM_FORMAT_ABGR8888:
		dspcntr |= DISPPLANE_RGBX888;
		break;
	case DRM_FORMAT_XRGB2101010:
	case DRM_FORMAT_ARGB2101010:
		dspcntr |= DISPPLANE_BGRX101010;
		break;
	case DRM_FORMAT_XBGR2101010:
	case DRM_FORMAT_ABGR2101010:
		dspcntr |= DISPPLANE_RGBX101010;
		break;
	default:
		BUG();
	}

	if (obj->tiling_mode != I915_TILING_NONE)
		dspcntr |= DISPPLANE_TILED;
	else
		dspcntr &= ~DISPPLANE_TILED;

	if (IS_HASWELL(dev) || IS_BROADWELL(dev))
		dspcntr &= ~DISPPLANE_TRICKLE_FEED_DISABLE;
	else
		dspcntr |= DISPPLANE_TRICKLE_FEED_DISABLE;

	I915_WRITE(reg, dspcntr);

	linear_offset = y * fb->pitches[0] + x * (fb->bits_per_pixel / 8);
	intel_crtc->dspaddr_offset =
		intel_gen4_compute_page_offset(&x, &y, obj->tiling_mode,
					       fb->bits_per_pixel / 8,
					       fb->pitches[0]);
	linear_offset -= intel_crtc->dspaddr_offset;

	DRM_DEBUG_KMS("Writing base %08lX %08lX %d %d %d\n",
		      i915_gem_obj_ggtt_offset(obj), linear_offset, x, y,
		      fb->pitches[0]);
	I915_WRITE(DSPSTRIDE(plane), fb->pitches[0]);
	I915_MODIFY_DISPBASE(DSPSURF(plane),
			     i915_gem_obj_ggtt_offset(obj) + intel_crtc->dspaddr_offset);
	if (IS_HASWELL(dev) || IS_BROADWELL(dev)) {
		I915_WRITE(DSPOFFSET(plane), (y << 16) | x);
	} else {
		I915_WRITE(DSPTILEOFF(plane), (y << 16) | x);
		I915_WRITE(DSPLINOFF(plane), linear_offset);
	}
	POSTING_READ(reg);

	return 0;
}

/* Assume fb object is pinned & idle & fenced and just update base pointers */
static int
intel_pipe_set_base_atomic(struct drm_crtc *crtc, struct drm_framebuffer *fb,
			   int x, int y, enum mode_set_atomic state)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (dev_priv->display.disable_fbc)
		dev_priv->display.disable_fbc(dev);
	intel_increase_pllclock(crtc);

	return dev_priv->display.update_plane(crtc, fb, x, y);
}

void intel_display_handle_reset(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_crtc *crtc;

	/*
	 * Flips in the rings have been nuked by the reset,
	 * so complete all pending flips so that user space
	 * will get its events and not get stuck.
	 *
	 * Also update the base address of all primary
	 * planes to the the last fb to make sure we're
	 * showing the correct fb after a reset.
	 *
	 * Need to make two loops over the crtcs so that we
	 * don't try to grab a crtc mutex before the
	 * pending_flip_queue really got woken up.
	 */

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
		enum plane plane = intel_crtc->plane;

		intel_prepare_page_flip(dev, plane);
		intel_finish_page_flip_plane(dev, plane);
	}

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct intel_crtc *intel_crtc = to_intel_crtc(crtc);

		mutex_lock(&crtc->mutex);
		if (intel_crtc->active)
			dev_priv->display.update_plane(crtc, crtc->fb,
						       crtc->x, crtc->y);
		mutex_unlock(&crtc->mutex);
	}
}

static int
intel_finish_fb(struct drm_framebuffer *old_fb)
{
	struct drm_i915_gem_object *obj = to_intel_framebuffer(old_fb)->obj;
	struct drm_i915_private *dev_priv = obj->base.dev->dev_private;
	bool was_interruptible = dev_priv->mm.interruptible;
	int ret;

	/* Big Hammer, we also need to ensure that any pending
	 * MI_WAIT_FOR_EVENT inside a user batch buffer on the
	 * current scanout is retired before unpinning the old
	 * framebuffer.
	 *
	 * This should only fail upon a hung GPU, in which case we
	 * can safely continue.
	 */
	dev_priv->mm.interruptible = false;
	ret = i915_gem_object_finish_gpu(obj);
	dev_priv->mm.interruptible = was_interruptible;

	return ret;
}

static void intel_crtc_update_sarea_pos(struct drm_crtc *crtc, int x, int y)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_master_private *master_priv;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);

	if (!dev->primary->master)
		return;

	master_priv = dev->primary->master->driver_priv;
	if (!master_priv->sarea_priv)
		return;

	switch (intel_crtc->pipe) {
	case 0:
		master_priv->sarea_priv->pipeA_x = x;
		master_priv->sarea_priv->pipeA_y = y;
		break;
	case 1:
		master_priv->sarea_priv->pipeB_x = x;
		master_priv->sarea_priv->pipeB_y = y;
		break;
	default:
		break;
	}
}

static int
intel_pipe_set_base(struct drm_crtc *crtc, int x, int y,
		    struct drm_framebuffer *fb)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct drm_framebuffer *old_fb;
	int ret;

	/* no fb bound */
	if (!fb) {
		DRM_ERROR("No FB bound\n");
		return 0;
	}

	if (intel_crtc->plane > INTEL_INFO(dev)->num_pipes) {
		DRM_ERROR("no plane for crtc: plane %c, num_pipes %d\n",
			  plane_name(intel_crtc->plane),
			  INTEL_INFO(dev)->num_pipes);
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}

	mutex_lock(&dev->struct_mutex);
	ret = intel_pin_and_fence_fb_obj(dev,
<<<<<<< HEAD
					 to_intel_framebuffer(crtc->fb)->obj,
					 NULL);
	if (ret != 0) {
		mutex_unlock(&dev->struct_mutex);
<<<<<<< HEAD
		return ret;
	}

	if (old_fb) {
		struct drm_i915_private *dev_priv = dev->dev_private;
		struct drm_i915_gem_object *obj = to_intel_framebuffer(old_fb)->obj;

		wait_event(dev_priv->pending_flip_queue,
			   atomic_read(&dev_priv->mm.wedged) ||
			   atomic_read(&obj->pending_flip) == 0);

		/* Big Hammer, we also need to ensure that any pending
		 * MI_WAIT_FOR_EVENT inside a user batch buffer on the
		 * current scanout is retired before unpinning the old
		 * framebuffer.
		 *
		 * This should only fail upon a hung GPU, in which case we
		 * can safely continue.
		 */
		ret = i915_gem_object_flush_gpu(obj);
		(void) ret;
	}
=======
=======
					 to_intel_framebuffer(fb)->obj,
					 NULL);
	if (ret != 0) {
		mutex_unlock(&dev->struct_mutex);
>>>>>>> refs/remotes/origin/master
		DRM_ERROR("pin & fence failed\n");
		return ret;
	}

<<<<<<< HEAD
	if (old_fb)
		intel_finish_fb(old_fb);
>>>>>>> refs/remotes/origin/cm-10.0

	ret = intel_pipe_set_base_atomic(crtc, crtc->fb, x, y,
					 LEAVE_ATOMIC_MODE_SET);
	if (ret) {
<<<<<<< HEAD
		i915_gem_object_unpin(to_intel_framebuffer(crtc->fb)->obj);
		mutex_unlock(&dev->struct_mutex);
=======
		intel_unpin_fb_obj(to_intel_framebuffer(crtc->fb)->obj);
		mutex_unlock(&dev->struct_mutex);
		DRM_ERROR("failed to update base address\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return ret;
	}

	if (old_fb) {
		intel_wait_for_vblank(dev, intel_crtc->pipe);
<<<<<<< HEAD
		i915_gem_object_unpin(to_intel_framebuffer(old_fb)->obj);
=======
		intel_unpin_fb_obj(to_intel_framebuffer(old_fb)->obj);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	mutex_unlock(&dev->struct_mutex);

	if (!dev->primary->master)
		return 0;

	master_priv = dev->primary->master->driver_priv;
	if (!master_priv->sarea_priv)
		return 0;

	if (intel_crtc->pipe) {
		master_priv->sarea_priv->pipeB_x = x;
		master_priv->sarea_priv->pipeB_y = y;
	} else {
		master_priv->sarea_priv->pipeA_x = x;
		master_priv->sarea_priv->pipeA_y = y;
	}
=======
	/*
	 * Update pipe size and adjust fitter if needed: the reason for this is
	 * that in compute_mode_changes we check the native mode (not the pfit
	 * mode) to see if we can flip rather than do a full mode set. In the
	 * fastboot case, we'll flip, but if we don't update the pipesrc and
	 * pfit state, we'll end up with a big fb scanned out into the wrong
	 * sized surface.
	 *
	 * To fix this properly, we need to hoist the checks up into
	 * compute_mode_changes (or above), check the actual pfit state and
	 * whether the platform allows pfit disable with pipe active, and only
	 * then update the pipesrc and pfit state, even on the flip path.
	 */
	if (i915_fastboot) {
		const struct drm_display_mode *adjusted_mode =
			&intel_crtc->config.adjusted_mode;

		I915_WRITE(PIPESRC(intel_crtc->pipe),
			   ((adjusted_mode->crtc_hdisplay - 1) << 16) |
			   (adjusted_mode->crtc_vdisplay - 1));
		if (!intel_crtc->config.pch_pfit.enabled &&
		    (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS) ||
		     intel_pipe_has_type(crtc, INTEL_OUTPUT_EDP))) {
			I915_WRITE(PF_CTL(intel_crtc->pipe), 0);
			I915_WRITE(PF_WIN_POS(intel_crtc->pipe), 0);
			I915_WRITE(PF_WIN_SZ(intel_crtc->pipe), 0);
		}
	}

	ret = dev_priv->display.update_plane(crtc, fb, x, y);
	if (ret) {
		intel_unpin_fb_obj(to_intel_framebuffer(fb)->obj);
		mutex_unlock(&dev->struct_mutex);
		DRM_ERROR("failed to update base address\n");
		return ret;
	}

	old_fb = crtc->fb;
	crtc->fb = fb;
	crtc->x = x;
	crtc->y = y;

	if (old_fb) {
		if (intel_crtc->active && old_fb != fb)
			intel_wait_for_vblank(dev, intel_crtc->pipe);
		intel_unpin_fb_obj(to_intel_framebuffer(old_fb)->obj);
	}

	intel_update_fbc(dev);
	intel_edp_psr_update(dev);
	mutex_unlock(&dev->struct_mutex);

	intel_crtc_update_sarea_pos(crtc, x, y);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static void ironlake_set_pll_edp(struct drm_crtc *crtc, int clock)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 dpa_ctl;

	DRM_DEBUG_KMS("eDP PLL enable for clock %d\n", clock);
	dpa_ctl = I915_READ(DP_A);
	dpa_ctl &= ~DP_PLL_FREQ_MASK;

	if (clock < 200000) {
		u32 temp;
		dpa_ctl |= DP_PLL_FREQ_160MHZ;
		/* workaround for 160Mhz:
		   1) program 0x4600c bits 15:0 = 0x8124
		   2) program 0x46010 bit 0 = 1
		   3) program 0x46034 bit 24 = 1
		   4) program 0x64000 bit 14 = 1
		   */
		temp = I915_READ(0x4600c);
		temp &= 0xffff0000;
		I915_WRITE(0x4600c, temp | 0x8124);

		temp = I915_READ(0x46010);
		I915_WRITE(0x46010, temp | 1);

		temp = I915_READ(0x46034);
		I915_WRITE(0x46034, temp | (1 << 24));
	} else {
		dpa_ctl |= DP_PLL_FREQ_270MHZ;
	}
	I915_WRITE(DP_A, dpa_ctl);

	POSTING_READ(DP_A);
	udelay(500);
}

=======
>>>>>>> refs/remotes/origin/master
static void intel_fdi_normal_train(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	u32 reg, temp;

	/* enable normal train */
	reg = FDI_TX_CTL(pipe);
	temp = I915_READ(reg);
	if (IS_IVYBRIDGE(dev)) {
		temp &= ~FDI_LINK_TRAIN_NONE_IVB;
		temp |= FDI_LINK_TRAIN_NONE_IVB | FDI_TX_ENHANCE_FRAME_ENABLE;
	} else {
		temp &= ~FDI_LINK_TRAIN_NONE;
		temp |= FDI_LINK_TRAIN_NONE | FDI_TX_ENHANCE_FRAME_ENABLE;
	}
	I915_WRITE(reg, temp);

	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
	if (HAS_PCH_CPT(dev)) {
		temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
		temp |= FDI_LINK_TRAIN_NORMAL_CPT;
	} else {
		temp &= ~FDI_LINK_TRAIN_NONE;
		temp |= FDI_LINK_TRAIN_NONE;
	}
	I915_WRITE(reg, temp | FDI_RX_ENHANCE_FRAME_ENABLE);

	/* wait one idle pattern time */
	POSTING_READ(reg);
	udelay(1000);

	/* IVB wants error correction enabled */
	if (IS_IVYBRIDGE(dev))
		I915_WRITE(reg, I915_READ(reg) | FDI_FS_ERRC_ENABLE |
			   FDI_FE_ERRC_ENABLE);
}

<<<<<<< HEAD
=======
static bool pipe_has_enabled_pch(struct intel_crtc *crtc)
{
	return crtc->base.enabled && crtc->active &&
		crtc->config.has_pch_encoder;
}

static void ivb_modeset_global_resources(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *pipe_B_crtc =
		to_intel_crtc(dev_priv->pipe_to_crtc_mapping[PIPE_B]);
	struct intel_crtc *pipe_C_crtc =
		to_intel_crtc(dev_priv->pipe_to_crtc_mapping[PIPE_C]);
	uint32_t temp;

	/*
	 * When everything is off disable fdi C so that we could enable fdi B
	 * with all lanes. Note that we don't care about enabled pipes without
	 * an enabled pch encoder.
	 */
	if (!pipe_has_enabled_pch(pipe_B_crtc) &&
	    !pipe_has_enabled_pch(pipe_C_crtc)) {
		WARN_ON(I915_READ(FDI_RX_CTL(PIPE_B)) & FDI_RX_ENABLE);
		WARN_ON(I915_READ(FDI_RX_CTL(PIPE_C)) & FDI_RX_ENABLE);

		temp = I915_READ(SOUTH_CHICKEN1);
		temp &= ~FDI_BC_BIFURCATION_SELECT;
		DRM_DEBUG_KMS("disabling fdi C rx\n");
		I915_WRITE(SOUTH_CHICKEN1, temp);
	}
}

>>>>>>> refs/remotes/origin/master
/* The FDI link training functions for ILK/Ibexpeak. */
static void ironlake_fdi_link_train(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;
	u32 reg, temp, tries;

	/* FDI needs bits from pipe & plane first */
	assert_pipe_enabled(dev_priv, pipe);
	assert_plane_enabled(dev_priv, plane);

	/* Train 1: umask FDI RX Interrupt symbol_lock and bit_lock bit
	   for train result */
	reg = FDI_RX_IMR(pipe);
	temp = I915_READ(reg);
	temp &= ~FDI_RX_SYMBOL_LOCK;
	temp &= ~FDI_RX_BIT_LOCK;
	I915_WRITE(reg, temp);
	I915_READ(reg);
	udelay(150);

	/* enable CPU FDI TX and PCH FDI RX */
	reg = FDI_TX_CTL(pipe);
	temp = I915_READ(reg);
<<<<<<< HEAD
	temp &= ~(7 << 19);
	temp |= (intel_crtc->fdi_lanes - 1) << 19;
=======
	temp &= ~FDI_DP_PORT_WIDTH_MASK;
	temp |= FDI_DP_PORT_WIDTH(intel_crtc->config.fdi_lanes);
>>>>>>> refs/remotes/origin/master
	temp &= ~FDI_LINK_TRAIN_NONE;
	temp |= FDI_LINK_TRAIN_PATTERN_1;
	I915_WRITE(reg, temp | FDI_TX_ENABLE);

	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
	temp &= ~FDI_LINK_TRAIN_NONE;
	temp |= FDI_LINK_TRAIN_PATTERN_1;
	I915_WRITE(reg, temp | FDI_RX_ENABLE);

	POSTING_READ(reg);
	udelay(150);

	/* Ironlake workaround, enable clock pointer after FDI enable*/
<<<<<<< HEAD
	if (HAS_PCH_IBX(dev)) {
		I915_WRITE(FDI_RX_CHICKEN(pipe), FDI_RX_PHASE_SYNC_POINTER_OVR);
		I915_WRITE(FDI_RX_CHICKEN(pipe), FDI_RX_PHASE_SYNC_POINTER_OVR |
			   FDI_RX_PHASE_SYNC_POINTER_EN);
	}
=======
	I915_WRITE(FDI_RX_CHICKEN(pipe), FDI_RX_PHASE_SYNC_POINTER_OVR);
	I915_WRITE(FDI_RX_CHICKEN(pipe), FDI_RX_PHASE_SYNC_POINTER_OVR |
		   FDI_RX_PHASE_SYNC_POINTER_EN);
>>>>>>> refs/remotes/origin/master

	reg = FDI_RX_IIR(pipe);
	for (tries = 0; tries < 5; tries++) {
		temp = I915_READ(reg);
		DRM_DEBUG_KMS("FDI_RX_IIR 0x%x\n", temp);

		if ((temp & FDI_RX_BIT_LOCK)) {
			DRM_DEBUG_KMS("FDI train 1 done.\n");
			I915_WRITE(reg, temp | FDI_RX_BIT_LOCK);
			break;
		}
	}
	if (tries == 5)
		DRM_ERROR("FDI train 1 fail!\n");

	/* Train 2 */
	reg = FDI_TX_CTL(pipe);
	temp = I915_READ(reg);
	temp &= ~FDI_LINK_TRAIN_NONE;
	temp |= FDI_LINK_TRAIN_PATTERN_2;
	I915_WRITE(reg, temp);

	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
	temp &= ~FDI_LINK_TRAIN_NONE;
	temp |= FDI_LINK_TRAIN_PATTERN_2;
	I915_WRITE(reg, temp);

	POSTING_READ(reg);
	udelay(150);

	reg = FDI_RX_IIR(pipe);
	for (tries = 0; tries < 5; tries++) {
		temp = I915_READ(reg);
		DRM_DEBUG_KMS("FDI_RX_IIR 0x%x\n", temp);

		if (temp & FDI_RX_SYMBOL_LOCK) {
			I915_WRITE(reg, temp | FDI_RX_SYMBOL_LOCK);
			DRM_DEBUG_KMS("FDI train 2 done.\n");
			break;
		}
	}
	if (tries == 5)
		DRM_ERROR("FDI train 2 fail!\n");

	DRM_DEBUG_KMS("FDI train done\n");

}

<<<<<<< HEAD
<<<<<<< HEAD
static const int snb_b_fdi_train_param [] = {
=======
static const int snb_b_fdi_train_param[] = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const int snb_b_fdi_train_param[] = {
>>>>>>> refs/remotes/origin/master
	FDI_LINK_TRAIN_400MV_0DB_SNB_B,
	FDI_LINK_TRAIN_400MV_6DB_SNB_B,
	FDI_LINK_TRAIN_600MV_3_5DB_SNB_B,
	FDI_LINK_TRAIN_800MV_0DB_SNB_B,
};

/* The FDI link training functions for SNB/Cougarpoint. */
static void gen6_fdi_link_train(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
<<<<<<< HEAD
	u32 reg, temp, i;
=======
	u32 reg, temp, i, retry;
>>>>>>> refs/remotes/origin/master

	/* Train 1: umask FDI RX Interrupt symbol_lock and bit_lock bit
	   for train result */
	reg = FDI_RX_IMR(pipe);
	temp = I915_READ(reg);
	temp &= ~FDI_RX_SYMBOL_LOCK;
	temp &= ~FDI_RX_BIT_LOCK;
	I915_WRITE(reg, temp);

	POSTING_READ(reg);
	udelay(150);

	/* enable CPU FDI TX and PCH FDI RX */
	reg = FDI_TX_CTL(pipe);
	temp = I915_READ(reg);
<<<<<<< HEAD
	temp &= ~(7 << 19);
	temp |= (intel_crtc->fdi_lanes - 1) << 19;
=======
	temp &= ~FDI_DP_PORT_WIDTH_MASK;
	temp |= FDI_DP_PORT_WIDTH(intel_crtc->config.fdi_lanes);
>>>>>>> refs/remotes/origin/master
	temp &= ~FDI_LINK_TRAIN_NONE;
	temp |= FDI_LINK_TRAIN_PATTERN_1;
	temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
	/* SNB-B */
	temp |= FDI_LINK_TRAIN_400MV_0DB_SNB_B;
	I915_WRITE(reg, temp | FDI_TX_ENABLE);

<<<<<<< HEAD
=======
	I915_WRITE(FDI_RX_MISC(pipe),
		   FDI_RX_TP1_TO_TP2_48 | FDI_RX_FDI_DELAY_90);

>>>>>>> refs/remotes/origin/master
	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
	if (HAS_PCH_CPT(dev)) {
		temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
		temp |= FDI_LINK_TRAIN_PATTERN_1_CPT;
	} else {
		temp &= ~FDI_LINK_TRAIN_NONE;
		temp |= FDI_LINK_TRAIN_PATTERN_1;
	}
	I915_WRITE(reg, temp | FDI_RX_ENABLE);

	POSTING_READ(reg);
	udelay(150);

<<<<<<< HEAD
<<<<<<< HEAD
	for (i = 0; i < 4; i++ ) {
=======
	for (i = 0; i < 4; i++) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (i = 0; i < 4; i++) {
>>>>>>> refs/remotes/origin/master
		reg = FDI_TX_CTL(pipe);
		temp = I915_READ(reg);
		temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
		temp |= snb_b_fdi_train_param[i];
		I915_WRITE(reg, temp);

		POSTING_READ(reg);
		udelay(500);

<<<<<<< HEAD
		reg = FDI_RX_IIR(pipe);
		temp = I915_READ(reg);
		DRM_DEBUG_KMS("FDI_RX_IIR 0x%x\n", temp);

		if (temp & FDI_RX_BIT_LOCK) {
			I915_WRITE(reg, temp | FDI_RX_BIT_LOCK);
			DRM_DEBUG_KMS("FDI train 1 done.\n");
			break;
		}
=======
		for (retry = 0; retry < 5; retry++) {
			reg = FDI_RX_IIR(pipe);
			temp = I915_READ(reg);
			DRM_DEBUG_KMS("FDI_RX_IIR 0x%x\n", temp);
			if (temp & FDI_RX_BIT_LOCK) {
				I915_WRITE(reg, temp | FDI_RX_BIT_LOCK);
				DRM_DEBUG_KMS("FDI train 1 done.\n");
				break;
			}
			udelay(50);
		}
		if (retry < 5)
			break;
>>>>>>> refs/remotes/origin/master
	}
	if (i == 4)
		DRM_ERROR("FDI train 1 fail!\n");

	/* Train 2 */
	reg = FDI_TX_CTL(pipe);
	temp = I915_READ(reg);
	temp &= ~FDI_LINK_TRAIN_NONE;
	temp |= FDI_LINK_TRAIN_PATTERN_2;
	if (IS_GEN6(dev)) {
		temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
		/* SNB-B */
		temp |= FDI_LINK_TRAIN_400MV_0DB_SNB_B;
	}
	I915_WRITE(reg, temp);

	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
	if (HAS_PCH_CPT(dev)) {
		temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
		temp |= FDI_LINK_TRAIN_PATTERN_2_CPT;
	} else {
		temp &= ~FDI_LINK_TRAIN_NONE;
		temp |= FDI_LINK_TRAIN_PATTERN_2;
	}
	I915_WRITE(reg, temp);

	POSTING_READ(reg);
	udelay(150);

<<<<<<< HEAD
<<<<<<< HEAD
	for (i = 0; i < 4; i++ ) {
=======
	for (i = 0; i < 4; i++) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (i = 0; i < 4; i++) {
>>>>>>> refs/remotes/origin/master
		reg = FDI_TX_CTL(pipe);
		temp = I915_READ(reg);
		temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
		temp |= snb_b_fdi_train_param[i];
		I915_WRITE(reg, temp);

		POSTING_READ(reg);
		udelay(500);

<<<<<<< HEAD
		reg = FDI_RX_IIR(pipe);
		temp = I915_READ(reg);
		DRM_DEBUG_KMS("FDI_RX_IIR 0x%x\n", temp);

		if (temp & FDI_RX_SYMBOL_LOCK) {
			I915_WRITE(reg, temp | FDI_RX_SYMBOL_LOCK);
			DRM_DEBUG_KMS("FDI train 2 done.\n");
			break;
		}
=======
		for (retry = 0; retry < 5; retry++) {
			reg = FDI_RX_IIR(pipe);
			temp = I915_READ(reg);
			DRM_DEBUG_KMS("FDI_RX_IIR 0x%x\n", temp);
			if (temp & FDI_RX_SYMBOL_LOCK) {
				I915_WRITE(reg, temp | FDI_RX_SYMBOL_LOCK);
				DRM_DEBUG_KMS("FDI train 2 done.\n");
				break;
			}
			udelay(50);
		}
		if (retry < 5)
			break;
>>>>>>> refs/remotes/origin/master
	}
	if (i == 4)
		DRM_ERROR("FDI train 2 fail!\n");

	DRM_DEBUG_KMS("FDI train done.\n");
}

/* Manual link training for Ivy Bridge A0 parts */
static void ivb_manual_fdi_link_train(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
<<<<<<< HEAD
	u32 reg, temp, i;
=======
	u32 reg, temp, i, j;
>>>>>>> refs/remotes/origin/master

	/* Train 1: umask FDI RX Interrupt symbol_lock and bit_lock bit
	   for train result */
	reg = FDI_RX_IMR(pipe);
	temp = I915_READ(reg);
	temp &= ~FDI_RX_SYMBOL_LOCK;
	temp &= ~FDI_RX_BIT_LOCK;
	I915_WRITE(reg, temp);

	POSTING_READ(reg);
	udelay(150);

<<<<<<< HEAD
	/* enable CPU FDI TX and PCH FDI RX */
	reg = FDI_TX_CTL(pipe);
	temp = I915_READ(reg);
	temp &= ~(7 << 19);
	temp |= (intel_crtc->fdi_lanes - 1) << 19;
	temp &= ~(FDI_LINK_TRAIN_AUTO | FDI_LINK_TRAIN_NONE_IVB);
	temp |= FDI_LINK_TRAIN_PATTERN_1_IVB;
	temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
	temp |= FDI_LINK_TRAIN_400MV_0DB_SNB_B;
	temp |= FDI_COMPOSITE_SYNC;
	I915_WRITE(reg, temp | FDI_TX_ENABLE);

	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
	temp &= ~FDI_LINK_TRAIN_AUTO;
	temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
	temp |= FDI_LINK_TRAIN_PATTERN_1_CPT;
	temp |= FDI_COMPOSITE_SYNC;
	I915_WRITE(reg, temp | FDI_RX_ENABLE);

	POSTING_READ(reg);
	udelay(150);

<<<<<<< HEAD
	for (i = 0; i < 4; i++ ) {
=======
	for (i = 0; i < 4; i++) {
>>>>>>> refs/remotes/origin/cm-10.0
		reg = FDI_TX_CTL(pipe);
		temp = I915_READ(reg);
		temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
		temp |= snb_b_fdi_train_param[i];
		I915_WRITE(reg, temp);

		POSTING_READ(reg);
		udelay(500);

		reg = FDI_RX_IIR(pipe);
		temp = I915_READ(reg);
		DRM_DEBUG_KMS("FDI_RX_IIR 0x%x\n", temp);

		if (temp & FDI_RX_BIT_LOCK ||
		    (I915_READ(reg) & FDI_RX_BIT_LOCK)) {
			I915_WRITE(reg, temp | FDI_RX_BIT_LOCK);
			DRM_DEBUG_KMS("FDI train 1 done.\n");
			break;
		}
	}
	if (i == 4)
		DRM_ERROR("FDI train 1 fail!\n");

	/* Train 2 */
	reg = FDI_TX_CTL(pipe);
	temp = I915_READ(reg);
	temp &= ~FDI_LINK_TRAIN_NONE_IVB;
	temp |= FDI_LINK_TRAIN_PATTERN_2_IVB;
	temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
	temp |= FDI_LINK_TRAIN_400MV_0DB_SNB_B;
	I915_WRITE(reg, temp);

	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
	temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
	temp |= FDI_LINK_TRAIN_PATTERN_2_CPT;
	I915_WRITE(reg, temp);

	POSTING_READ(reg);
	udelay(150);

<<<<<<< HEAD
	for (i = 0; i < 4; i++ ) {
=======
	for (i = 0; i < 4; i++) {
>>>>>>> refs/remotes/origin/cm-10.0
		reg = FDI_TX_CTL(pipe);
		temp = I915_READ(reg);
		temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
		temp |= snb_b_fdi_train_param[i];
		I915_WRITE(reg, temp);

		POSTING_READ(reg);
		udelay(500);

		reg = FDI_RX_IIR(pipe);
		temp = I915_READ(reg);
		DRM_DEBUG_KMS("FDI_RX_IIR 0x%x\n", temp);

		if (temp & FDI_RX_SYMBOL_LOCK) {
			I915_WRITE(reg, temp | FDI_RX_SYMBOL_LOCK);
			DRM_DEBUG_KMS("FDI train 2 done.\n");
			break;
		}
	}
	if (i == 4)
		DRM_ERROR("FDI train 2 fail!\n");

	DRM_DEBUG_KMS("FDI train done.\n");
}

static void ironlake_fdi_pll_enable(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	u32 reg, temp;

	/* Write the TU size bits so error detection works */
	I915_WRITE(FDI_RX_TUSIZE1(pipe),
		   I915_READ(PIPE_DATA_M1(pipe)) & TU_SIZE_MASK);
=======
	DRM_DEBUG_KMS("FDI_RX_IIR before link train 0x%x\n",
		      I915_READ(FDI_RX_IIR(pipe)));

	/* Try each vswing and preemphasis setting twice before moving on */
	for (j = 0; j < ARRAY_SIZE(snb_b_fdi_train_param) * 2; j++) {
		/* disable first in case we need to retry */
		reg = FDI_TX_CTL(pipe);
		temp = I915_READ(reg);
		temp &= ~(FDI_LINK_TRAIN_AUTO | FDI_LINK_TRAIN_NONE_IVB);
		temp &= ~FDI_TX_ENABLE;
		I915_WRITE(reg, temp);

		reg = FDI_RX_CTL(pipe);
		temp = I915_READ(reg);
		temp &= ~FDI_LINK_TRAIN_AUTO;
		temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
		temp &= ~FDI_RX_ENABLE;
		I915_WRITE(reg, temp);

		/* enable CPU FDI TX and PCH FDI RX */
		reg = FDI_TX_CTL(pipe);
		temp = I915_READ(reg);
		temp &= ~FDI_DP_PORT_WIDTH_MASK;
		temp |= FDI_DP_PORT_WIDTH(intel_crtc->config.fdi_lanes);
		temp |= FDI_LINK_TRAIN_PATTERN_1_IVB;
		temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
		temp |= snb_b_fdi_train_param[j/2];
		temp |= FDI_COMPOSITE_SYNC;
		I915_WRITE(reg, temp | FDI_TX_ENABLE);

		I915_WRITE(FDI_RX_MISC(pipe),
			   FDI_RX_TP1_TO_TP2_48 | FDI_RX_FDI_DELAY_90);

		reg = FDI_RX_CTL(pipe);
		temp = I915_READ(reg);
		temp |= FDI_LINK_TRAIN_PATTERN_1_CPT;
		temp |= FDI_COMPOSITE_SYNC;
		I915_WRITE(reg, temp | FDI_RX_ENABLE);

		POSTING_READ(reg);
		udelay(1); /* should be 0.5us */

		for (i = 0; i < 4; i++) {
			reg = FDI_RX_IIR(pipe);
			temp = I915_READ(reg);
			DRM_DEBUG_KMS("FDI_RX_IIR 0x%x\n", temp);

			if (temp & FDI_RX_BIT_LOCK ||
			    (I915_READ(reg) & FDI_RX_BIT_LOCK)) {
				I915_WRITE(reg, temp | FDI_RX_BIT_LOCK);
				DRM_DEBUG_KMS("FDI train 1 done, level %i.\n",
					      i);
				break;
			}
			udelay(1); /* should be 0.5us */
		}
		if (i == 4) {
			DRM_DEBUG_KMS("FDI train 1 fail on vswing %d\n", j / 2);
			continue;
		}

		/* Train 2 */
		reg = FDI_TX_CTL(pipe);
		temp = I915_READ(reg);
		temp &= ~FDI_LINK_TRAIN_NONE_IVB;
		temp |= FDI_LINK_TRAIN_PATTERN_2_IVB;
		I915_WRITE(reg, temp);

		reg = FDI_RX_CTL(pipe);
		temp = I915_READ(reg);
		temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
		temp |= FDI_LINK_TRAIN_PATTERN_2_CPT;
		I915_WRITE(reg, temp);

		POSTING_READ(reg);
		udelay(2); /* should be 1.5us */

		for (i = 0; i < 4; i++) {
			reg = FDI_RX_IIR(pipe);
			temp = I915_READ(reg);
			DRM_DEBUG_KMS("FDI_RX_IIR 0x%x\n", temp);

			if (temp & FDI_RX_SYMBOL_LOCK ||
			    (I915_READ(reg) & FDI_RX_SYMBOL_LOCK)) {
				I915_WRITE(reg, temp | FDI_RX_SYMBOL_LOCK);
				DRM_DEBUG_KMS("FDI train 2 done, level %i.\n",
					      i);
				goto train_done;
			}
			udelay(2); /* should be 1.5us */
		}
		if (i == 4)
			DRM_DEBUG_KMS("FDI train 2 fail on vswing %d\n", j / 2);
	}

train_done:
	DRM_DEBUG_KMS("FDI train done.\n");
}

static void ironlake_fdi_pll_enable(struct intel_crtc *intel_crtc)
{
	struct drm_device *dev = intel_crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipe = intel_crtc->pipe;
	u32 reg, temp;

>>>>>>> refs/remotes/origin/master

	/* enable PCH FDI RX PLL, wait warmup plus DMI latency */
	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
<<<<<<< HEAD
	temp &= ~((0x7 << 19) | (0x7 << 16));
	temp |= (intel_crtc->fdi_lanes - 1) << 19;
	temp |= (I915_READ(PIPECONF(pipe)) & PIPE_BPC_MASK) << 11;
=======
	temp &= ~(FDI_DP_PORT_WIDTH_MASK | (0x7 << 16));
	temp |= FDI_DP_PORT_WIDTH(intel_crtc->config.fdi_lanes);
	temp |= (I915_READ(PIPECONF(pipe)) & PIPECONF_BPC_MASK) << 11;
>>>>>>> refs/remotes/origin/master
	I915_WRITE(reg, temp | FDI_RX_PLL_ENABLE);

	POSTING_READ(reg);
	udelay(200);

	/* Switch from Rawclk to PCDclk */
	temp = I915_READ(reg);
	I915_WRITE(reg, temp | FDI_PCDCLK);

	POSTING_READ(reg);
	udelay(200);

	/* Enable CPU FDI TX PLL, always on for Ironlake */
	reg = FDI_TX_CTL(pipe);
	temp = I915_READ(reg);
	if ((temp & FDI_TX_PLL_ENABLE) == 0) {
		I915_WRITE(reg, temp | FDI_TX_PLL_ENABLE);

		POSTING_READ(reg);
		udelay(100);
	}
}

<<<<<<< HEAD
=======
static void ironlake_fdi_pll_disable(struct intel_crtc *intel_crtc)
{
	struct drm_device *dev = intel_crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipe = intel_crtc->pipe;
	u32 reg, temp;

	/* Switch from PCDclk to Rawclk */
	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
	I915_WRITE(reg, temp & ~FDI_PCDCLK);

	/* Disable CPU FDI TX PLL */
	reg = FDI_TX_CTL(pipe);
	temp = I915_READ(reg);
	I915_WRITE(reg, temp & ~FDI_TX_PLL_ENABLE);

	POSTING_READ(reg);
	udelay(100);

	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
	I915_WRITE(reg, temp & ~FDI_RX_PLL_ENABLE);

	/* Wait for the clocks to turn off. */
	POSTING_READ(reg);
	udelay(100);
}

>>>>>>> refs/remotes/origin/master
static void ironlake_fdi_disable(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	u32 reg, temp;

	/* disable CPU FDI tx and PCH FDI rx */
	reg = FDI_TX_CTL(pipe);
	temp = I915_READ(reg);
	I915_WRITE(reg, temp & ~FDI_TX_ENABLE);
	POSTING_READ(reg);

	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
	temp &= ~(0x7 << 16);
<<<<<<< HEAD
	temp |= (I915_READ(PIPECONF(pipe)) & PIPE_BPC_MASK) << 11;
=======
	temp |= (I915_READ(PIPECONF(pipe)) & PIPECONF_BPC_MASK) << 11;
>>>>>>> refs/remotes/origin/master
	I915_WRITE(reg, temp & ~FDI_RX_ENABLE);

	POSTING_READ(reg);
	udelay(100);

	/* Ironlake workaround, disable clock pointer after downing FDI */
	if (HAS_PCH_IBX(dev)) {
		I915_WRITE(FDI_RX_CHICKEN(pipe), FDI_RX_PHASE_SYNC_POINTER_OVR);
<<<<<<< HEAD
		I915_WRITE(FDI_RX_CHICKEN(pipe),
			   I915_READ(FDI_RX_CHICKEN(pipe) &
				     ~FDI_RX_PHASE_SYNC_POINTER_EN));
=======
>>>>>>> refs/remotes/origin/master
	}

	/* still set train pattern 1 */
	reg = FDI_TX_CTL(pipe);
	temp = I915_READ(reg);
	temp &= ~FDI_LINK_TRAIN_NONE;
	temp |= FDI_LINK_TRAIN_PATTERN_1;
	I915_WRITE(reg, temp);

	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
	if (HAS_PCH_CPT(dev)) {
		temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
		temp |= FDI_LINK_TRAIN_PATTERN_1_CPT;
	} else {
		temp &= ~FDI_LINK_TRAIN_NONE;
		temp |= FDI_LINK_TRAIN_PATTERN_1;
	}
	/* BPC in FDI rx is consistent with that in PIPECONF */
	temp &= ~(0x07 << 16);
<<<<<<< HEAD
	temp |= (I915_READ(PIPECONF(pipe)) & PIPE_BPC_MASK) << 11;
=======
	temp |= (I915_READ(PIPECONF(pipe)) & PIPECONF_BPC_MASK) << 11;
>>>>>>> refs/remotes/origin/master
	I915_WRITE(reg, temp);

	POSTING_READ(reg);
	udelay(100);
}

<<<<<<< HEAD
/*
 * When we disable a pipe, we need to clear any pending scanline wait events
 * to avoid hanging the ring, which we assume we are waiting on.
 */
static void intel_clear_scanline_wait(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_ring_buffer *ring;
	u32 tmp;

	if (IS_GEN2(dev))
		/* Can't break the hang on i8xx */
		return;

	ring = LP_RING(dev_priv);
	tmp = I915_READ_CTL(ring);
	if (tmp & RING_WAIT)
		I915_WRITE_CTL(ring, tmp);
}

<<<<<<< HEAD
static void intel_crtc_wait_for_pending_flips(struct drm_crtc *crtc)
{
	struct drm_i915_gem_object *obj;
	struct drm_i915_private *dev_priv;
=======
static bool intel_crtc_has_pending_flip(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned long flags;
	bool pending;

	if (atomic_read(&dev_priv->mm.wedged))
		return false;

	spin_lock_irqsave(&dev->event_lock, flags);
	pending = to_intel_crtc(crtc)->unpin_work != NULL;
	spin_unlock_irqrestore(&dev->event_lock, flags);

	return pending;
}

static void intel_crtc_wait_for_pending_flips(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
>>>>>>> refs/remotes/origin/cm-10.0

	if (crtc->fb == NULL)
		return;

<<<<<<< HEAD
	obj = to_intel_framebuffer(crtc->fb)->obj;
	dev_priv = crtc->dev->dev_private;
	wait_event(dev_priv->pending_flip_queue,
		   atomic_read(&obj->pending_flip) == 0);
=======
	wait_event(dev_priv->pending_flip_queue,
		   !intel_crtc_has_pending_flip(crtc));

	mutex_lock(&dev->struct_mutex);
	intel_finish_fb(crtc->fb);
	mutex_unlock(&dev->struct_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
}

static bool intel_crtc_driving_pch(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_mode_config *mode_config = &dev->mode_config;
	struct intel_encoder *encoder;

	/*
	 * If there's a non-PCH eDP on this crtc, it must be DP_A, and that
	 * must be driven by its own crtc; no sharing is possible.
	 */
	list_for_each_entry(encoder, &mode_config->encoder_list, base.head) {
		if (encoder->base.crtc != crtc)
			continue;

		switch (encoder->type) {
		case INTEL_OUTPUT_EDP:
			if (!intel_encoder_is_pch_edp(&encoder->base))
				return false;
			continue;
		}
	}

	return true;
=======
static bool intel_crtc_has_pending_flip(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	unsigned long flags;
	bool pending;

	if (i915_reset_in_progress(&dev_priv->gpu_error) ||
	    intel_crtc->reset_counter != atomic_read(&dev_priv->gpu_error.reset_counter))
		return false;

	spin_lock_irqsave(&dev->event_lock, flags);
	pending = to_intel_crtc(crtc)->unpin_work != NULL;
	spin_unlock_irqrestore(&dev->event_lock, flags);

	return pending;
}

static void intel_crtc_wait_for_pending_flips(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (crtc->fb == NULL)
		return;

	WARN_ON(waitqueue_active(&dev_priv->pending_flip_queue));

	wait_event(dev_priv->pending_flip_queue,
		   !intel_crtc_has_pending_flip(crtc));

	mutex_lock(&dev->struct_mutex);
	intel_finish_fb(crtc->fb);
	mutex_unlock(&dev->struct_mutex);
}

/* Program iCLKIP clock to the desired frequency */
static void lpt_program_iclkip(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int clock = to_intel_crtc(crtc)->config.adjusted_mode.crtc_clock;
	u32 divsel, phaseinc, auxdiv, phasedir = 0;
	u32 temp;

	mutex_lock(&dev_priv->dpio_lock);

	/* It is necessary to ungate the pixclk gate prior to programming
	 * the divisors, and gate it back when it is done.
	 */
	I915_WRITE(PIXCLK_GATE, PIXCLK_GATE_GATE);

	/* Disable SSCCTL */
	intel_sbi_write(dev_priv, SBI_SSCCTL6,
			intel_sbi_read(dev_priv, SBI_SSCCTL6, SBI_ICLK) |
				SBI_SSCCTL_DISABLE,
			SBI_ICLK);

	/* 20MHz is a corner case which is out of range for the 7-bit divisor */
	if (clock == 20000) {
		auxdiv = 1;
		divsel = 0x41;
		phaseinc = 0x20;
	} else {
		/* The iCLK virtual clock root frequency is in MHz,
		 * but the adjusted_mode->crtc_clock in in KHz. To get the
		 * divisors, it is necessary to divide one by another, so we
		 * convert the virtual clock precision to KHz here for higher
		 * precision.
		 */
		u32 iclk_virtual_root_freq = 172800 * 1000;
		u32 iclk_pi_range = 64;
		u32 desired_divisor, msb_divisor_value, pi_value;

		desired_divisor = (iclk_virtual_root_freq / clock);
		msb_divisor_value = desired_divisor / iclk_pi_range;
		pi_value = desired_divisor % iclk_pi_range;

		auxdiv = 0;
		divsel = msb_divisor_value - 2;
		phaseinc = pi_value;
	}

	/* This should not happen with any sane values */
	WARN_ON(SBI_SSCDIVINTPHASE_DIVSEL(divsel) &
		~SBI_SSCDIVINTPHASE_DIVSEL_MASK);
	WARN_ON(SBI_SSCDIVINTPHASE_DIR(phasedir) &
		~SBI_SSCDIVINTPHASE_INCVAL_MASK);

	DRM_DEBUG_KMS("iCLKIP clock: found settings for %dKHz refresh rate: auxdiv=%x, divsel=%x, phasedir=%x, phaseinc=%x\n",
			clock,
			auxdiv,
			divsel,
			phasedir,
			phaseinc);

	/* Program SSCDIVINTPHASE6 */
	temp = intel_sbi_read(dev_priv, SBI_SSCDIVINTPHASE6, SBI_ICLK);
	temp &= ~SBI_SSCDIVINTPHASE_DIVSEL_MASK;
	temp |= SBI_SSCDIVINTPHASE_DIVSEL(divsel);
	temp &= ~SBI_SSCDIVINTPHASE_INCVAL_MASK;
	temp |= SBI_SSCDIVINTPHASE_INCVAL(phaseinc);
	temp |= SBI_SSCDIVINTPHASE_DIR(phasedir);
	temp |= SBI_SSCDIVINTPHASE_PROPAGATE;
	intel_sbi_write(dev_priv, SBI_SSCDIVINTPHASE6, temp, SBI_ICLK);

	/* Program SSCAUXDIV */
	temp = intel_sbi_read(dev_priv, SBI_SSCAUXDIV6, SBI_ICLK);
	temp &= ~SBI_SSCAUXDIV_FINALDIV2SEL(1);
	temp |= SBI_SSCAUXDIV_FINALDIV2SEL(auxdiv);
	intel_sbi_write(dev_priv, SBI_SSCAUXDIV6, temp, SBI_ICLK);

	/* Enable modulator and associated divider */
	temp = intel_sbi_read(dev_priv, SBI_SSCCTL6, SBI_ICLK);
	temp &= ~SBI_SSCCTL_DISABLE;
	intel_sbi_write(dev_priv, SBI_SSCCTL6, temp, SBI_ICLK);

	/* Wait for initialization time */
	udelay(24);

	I915_WRITE(PIXCLK_GATE, PIXCLK_GATE_UNGATE);

	mutex_unlock(&dev_priv->dpio_lock);
}

static void ironlake_pch_transcoder_set_timings(struct intel_crtc *crtc,
						enum pipe pch_transcoder)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	enum transcoder cpu_transcoder = crtc->config.cpu_transcoder;

	I915_WRITE(PCH_TRANS_HTOTAL(pch_transcoder),
		   I915_READ(HTOTAL(cpu_transcoder)));
	I915_WRITE(PCH_TRANS_HBLANK(pch_transcoder),
		   I915_READ(HBLANK(cpu_transcoder)));
	I915_WRITE(PCH_TRANS_HSYNC(pch_transcoder),
		   I915_READ(HSYNC(cpu_transcoder)));

	I915_WRITE(PCH_TRANS_VTOTAL(pch_transcoder),
		   I915_READ(VTOTAL(cpu_transcoder)));
	I915_WRITE(PCH_TRANS_VBLANK(pch_transcoder),
		   I915_READ(VBLANK(cpu_transcoder)));
	I915_WRITE(PCH_TRANS_VSYNC(pch_transcoder),
		   I915_READ(VSYNC(cpu_transcoder)));
	I915_WRITE(PCH_TRANS_VSYNCSHIFT(pch_transcoder),
		   I915_READ(VSYNCSHIFT(cpu_transcoder)));
}

static void cpt_enable_fdi_bc_bifurcation(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t temp;

	temp = I915_READ(SOUTH_CHICKEN1);
	if (temp & FDI_BC_BIFURCATION_SELECT)
		return;

	WARN_ON(I915_READ(FDI_RX_CTL(PIPE_B)) & FDI_RX_ENABLE);
	WARN_ON(I915_READ(FDI_RX_CTL(PIPE_C)) & FDI_RX_ENABLE);

	temp |= FDI_BC_BIFURCATION_SELECT;
	DRM_DEBUG_KMS("enabling fdi C rx\n");
	I915_WRITE(SOUTH_CHICKEN1, temp);
	POSTING_READ(SOUTH_CHICKEN1);
}

static void ivybridge_update_fdi_bc_bifurcation(struct intel_crtc *intel_crtc)
{
	struct drm_device *dev = intel_crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	switch (intel_crtc->pipe) {
	case PIPE_A:
		break;
	case PIPE_B:
		if (intel_crtc->config.fdi_lanes > 2)
			WARN_ON(I915_READ(SOUTH_CHICKEN1) & FDI_BC_BIFURCATION_SELECT);
		else
			cpt_enable_fdi_bc_bifurcation(dev);

		break;
	case PIPE_C:
		cpt_enable_fdi_bc_bifurcation(dev);

		break;
	default:
		BUG();
	}
>>>>>>> refs/remotes/origin/master
}

/*
 * Enable PCH resources required for PCH ports:
 *   - PCH PLLs
 *   - FDI training & RX/TX
 *   - update transcoder timings
 *   - DP transcoding bits
 *   - transcoder
 */
static void ironlake_pch_enable(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
<<<<<<< HEAD
<<<<<<< HEAD
	u32 reg, temp;
=======
	u32 reg, temp, transc_sel;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 reg, temp;

	assert_pch_transcoder_disabled(dev_priv, pipe);

	if (IS_IVYBRIDGE(dev))
		ivybridge_update_fdi_bc_bifurcation(intel_crtc);

	/* Write the TU size bits before fdi link training, so that error
	 * detection works. */
	I915_WRITE(FDI_RX_TUSIZE1(pipe),
		   I915_READ(PIPE_DATA_M1(pipe)) & TU_SIZE_MASK);
>>>>>>> refs/remotes/origin/master

	/* For PCH output, training FDI link */
	dev_priv->display.fdi_link_train(crtc);

<<<<<<< HEAD
	intel_enable_pch_pll(dev_priv, pipe);

	if (HAS_PCH_CPT(dev)) {
<<<<<<< HEAD
		/* Be sure PCH DPLL SEL is set */
		temp = I915_READ(PCH_DPLL_SEL);
		if (pipe == 0 && (temp & TRANSA_DPLL_ENABLE) == 0)
			temp |= (TRANSA_DPLL_ENABLE | TRANSA_DPLLA_SEL);
		else if (pipe == 1 && (temp & TRANSB_DPLL_ENABLE) == 0)
			temp |= (TRANSB_DPLL_ENABLE | TRANSB_DPLLB_SEL);
=======
		transc_sel = intel_crtc->use_pll_a ? TRANSC_DPLLA_SEL :
			TRANSC_DPLLB_SEL;

		/* Be sure PCH DPLL SEL is set */
		temp = I915_READ(PCH_DPLL_SEL);
		if (pipe == 0) {
			temp &= ~(TRANSA_DPLLB_SEL);
			temp |= (TRANSA_DPLL_ENABLE | TRANSA_DPLLA_SEL);
		} else if (pipe == 1) {
			temp &= ~(TRANSB_DPLLB_SEL);
			temp |= (TRANSB_DPLL_ENABLE | TRANSB_DPLLB_SEL);
		} else if (pipe == 2) {
			temp &= ~(TRANSC_DPLLB_SEL);
			temp |= (TRANSC_DPLL_ENABLE | transc_sel);
		}
>>>>>>> refs/remotes/origin/cm-10.0
		I915_WRITE(PCH_DPLL_SEL, temp);
	}

	/* set transcoder timing, panel must allow it */
	assert_panel_unlocked(dev_priv, pipe);
	I915_WRITE(TRANS_HTOTAL(pipe), I915_READ(HTOTAL(pipe)));
	I915_WRITE(TRANS_HBLANK(pipe), I915_READ(HBLANK(pipe)));
	I915_WRITE(TRANS_HSYNC(pipe),  I915_READ(HSYNC(pipe)));

	I915_WRITE(TRANS_VTOTAL(pipe), I915_READ(VTOTAL(pipe)));
	I915_WRITE(TRANS_VBLANK(pipe), I915_READ(VBLANK(pipe)));
	I915_WRITE(TRANS_VSYNC(pipe),  I915_READ(VSYNC(pipe)));
<<<<<<< HEAD
=======
	I915_WRITE(TRANS_VSYNCSHIFT(pipe),  I915_READ(VSYNCSHIFT(pipe)));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* We need to program the right clock selection before writing the pixel
	 * mutliplier into the DPLL. */
	if (HAS_PCH_CPT(dev)) {
		u32 sel;

		temp = I915_READ(PCH_DPLL_SEL);
		temp |= TRANS_DPLL_ENABLE(pipe);
		sel = TRANS_DPLLB_SEL(pipe);
		if (intel_crtc->config.shared_dpll == DPLL_ID_PCH_PLL_B)
			temp |= sel;
		else
			temp &= ~sel;
		I915_WRITE(PCH_DPLL_SEL, temp);
	}

	/* XXX: pch pll's can be enabled any time before we enable the PCH
	 * transcoder, and we actually should do this to not upset any PCH
	 * transcoder that already use the clock when we share it.
	 *
	 * Note that enable_shared_dpll tries to do the right thing, but
	 * get_shared_dpll unconditionally resets the pll - we need that to have
	 * the right LVDS enable sequence. */
	ironlake_enable_shared_dpll(intel_crtc);

	/* set transcoder timing, panel must allow it */
	assert_panel_unlocked(dev_priv, pipe);
	ironlake_pch_transcoder_set_timings(intel_crtc, pipe);
>>>>>>> refs/remotes/origin/master

	intel_fdi_normal_train(crtc);

	/* For PCH DP, enable TRANS_DP_CTL */
	if (HAS_PCH_CPT(dev) &&
<<<<<<< HEAD
<<<<<<< HEAD
	    intel_pipe_has_type(crtc, INTEL_OUTPUT_DISPLAYPORT)) {
=======
	    (intel_pipe_has_type(crtc, INTEL_OUTPUT_DISPLAYPORT) ||
	     intel_pipe_has_type(crtc, INTEL_OUTPUT_EDP))) {
		u32 bpc = (I915_READ(PIPECONF(pipe)) & PIPE_BPC_MASK) >> 5;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	    (intel_pipe_has_type(crtc, INTEL_OUTPUT_DISPLAYPORT) ||
	     intel_pipe_has_type(crtc, INTEL_OUTPUT_EDP))) {
		u32 bpc = (I915_READ(PIPECONF(pipe)) & PIPECONF_BPC_MASK) >> 5;
>>>>>>> refs/remotes/origin/master
		reg = TRANS_DP_CTL(pipe);
		temp = I915_READ(reg);
		temp &= ~(TRANS_DP_PORT_SEL_MASK |
			  TRANS_DP_SYNC_MASK |
			  TRANS_DP_BPC_MASK);
		temp |= (TRANS_DP_OUTPUT_ENABLE |
			 TRANS_DP_ENH_FRAMING);
<<<<<<< HEAD
<<<<<<< HEAD
		temp |= TRANS_DP_8BPC;
=======
		temp |= bpc << 9; /* same format but at 11:9 */
>>>>>>> refs/remotes/origin/cm-10.0
=======
		temp |= bpc << 9; /* same format but at 11:9 */
>>>>>>> refs/remotes/origin/master

		if (crtc->mode.flags & DRM_MODE_FLAG_PHSYNC)
			temp |= TRANS_DP_HSYNC_ACTIVE_HIGH;
		if (crtc->mode.flags & DRM_MODE_FLAG_PVSYNC)
			temp |= TRANS_DP_VSYNC_ACTIVE_HIGH;

		switch (intel_trans_dp_port_sel(crtc)) {
		case PCH_DP_B:
			temp |= TRANS_DP_PORT_SEL_B;
			break;
		case PCH_DP_C:
			temp |= TRANS_DP_PORT_SEL_C;
			break;
		case PCH_DP_D:
			temp |= TRANS_DP_PORT_SEL_D;
			break;
		default:
<<<<<<< HEAD
			DRM_DEBUG_KMS("Wrong PCH DP port return. Guess port B\n");
			temp |= TRANS_DP_PORT_SEL_B;
			break;
		}

		I915_WRITE(reg, temp);
	}

	intel_enable_transcoder(dev_priv, pipe);
}

<<<<<<< HEAD
=======
void intel_cpt_verify_modeset(struct drm_device *dev, int pipe)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int dslreg = PIPEDSL(pipe), tc2reg = TRANS_CHICKEN2(pipe);
	u32 temp;

	temp = I915_READ(dslreg);
	udelay(500);
	if (wait_for(I915_READ(dslreg) != temp, 5)) {
		/* Without this, mode sets may fail silently on FDI */
		I915_WRITE(tc2reg, TRANS_AUTOTRAIN_GEN_STALL_DIS);
		udelay(250);
		I915_WRITE(tc2reg, 0);
		if (wait_for(I915_READ(dslreg) != temp, 5))
			DRM_ERROR("mode set failed: pipe %d stuck\n", pipe);
	}
}

>>>>>>> refs/remotes/origin/cm-10.0
static void ironlake_crtc_enable(struct drm_crtc *crtc)
=======
			BUG();
		}

		I915_WRITE(reg, temp);
	}

	ironlake_enable_pch_transcoder(dev_priv, pipe);
}

static void lpt_pch_enable(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	enum transcoder cpu_transcoder = intel_crtc->config.cpu_transcoder;

	assert_pch_transcoder_disabled(dev_priv, TRANSCODER_A);

	lpt_program_iclkip(crtc);

	/* Set transcoder timing. */
	ironlake_pch_transcoder_set_timings(intel_crtc, PIPE_A);

	lpt_enable_pch_transcoder(dev_priv, cpu_transcoder);
}

static void intel_put_shared_dpll(struct intel_crtc *crtc)
{
	struct intel_shared_dpll *pll = intel_crtc_to_shared_dpll(crtc);

	if (pll == NULL)
		return;

	if (pll->refcount == 0) {
		WARN(1, "bad %s refcount\n", pll->name);
		return;
	}

	if (--pll->refcount == 0) {
		WARN_ON(pll->on);
		WARN_ON(pll->active);
	}

	crtc->config.shared_dpll = DPLL_ID_PRIVATE;
}

static struct intel_shared_dpll *intel_get_shared_dpll(struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = crtc->base.dev->dev_private;
	struct intel_shared_dpll *pll = intel_crtc_to_shared_dpll(crtc);
	enum intel_dpll_id i;

	if (pll) {
		DRM_DEBUG_KMS("CRTC:%d dropping existing %s\n",
			      crtc->base.base.id, pll->name);
		intel_put_shared_dpll(crtc);
	}

	if (HAS_PCH_IBX(dev_priv->dev)) {
		/* Ironlake PCH has a fixed PLL->PCH pipe mapping. */
		i = (enum intel_dpll_id) crtc->pipe;
		pll = &dev_priv->shared_dplls[i];

		DRM_DEBUG_KMS("CRTC:%d using pre-allocated %s\n",
			      crtc->base.base.id, pll->name);

		goto found;
	}

	for (i = 0; i < dev_priv->num_shared_dpll; i++) {
		pll = &dev_priv->shared_dplls[i];

		/* Only want to check enabled timings first */
		if (pll->refcount == 0)
			continue;

		if (memcmp(&crtc->config.dpll_hw_state, &pll->hw_state,
			   sizeof(pll->hw_state)) == 0) {
			DRM_DEBUG_KMS("CRTC:%d sharing existing %s (refcount %d, ative %d)\n",
				      crtc->base.base.id,
				      pll->name, pll->refcount, pll->active);

			goto found;
		}
	}

	/* Ok no matching timings, maybe there's a free one? */
	for (i = 0; i < dev_priv->num_shared_dpll; i++) {
		pll = &dev_priv->shared_dplls[i];
		if (pll->refcount == 0) {
			DRM_DEBUG_KMS("CRTC:%d allocated %s\n",
				      crtc->base.base.id, pll->name);
			goto found;
		}
	}

	return NULL;

found:
	crtc->config.shared_dpll = i;
	DRM_DEBUG_DRIVER("using %s for pipe %c\n", pll->name,
			 pipe_name(crtc->pipe));

	if (pll->active == 0) {
		memcpy(&pll->hw_state, &crtc->config.dpll_hw_state,
		       sizeof(pll->hw_state));

		DRM_DEBUG_DRIVER("setting up %s\n", pll->name);
		WARN_ON(pll->on);
		assert_shared_dpll_disabled(dev_priv, pll);

		pll->mode_set(dev_priv, pll);
	}
	pll->refcount++;

	return pll;
}

static void cpt_verify_modeset(struct drm_device *dev, int pipe)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int dslreg = PIPEDSL(pipe);
	u32 temp;

	temp = I915_READ(dslreg);
	udelay(500);
	if (wait_for(I915_READ(dslreg) != temp, 5)) {
		if (wait_for(I915_READ(dslreg) != temp, 5))
			DRM_ERROR("mode set failed: pipe %c stuck\n", pipe_name(pipe));
	}
}

static void ironlake_pfit_enable(struct intel_crtc *crtc)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipe = crtc->pipe;

	if (crtc->config.pch_pfit.enabled) {
		/* Force use of hard-coded filter coefficients
		 * as some pre-programmed values are broken,
		 * e.g. x201.
		 */
		if (IS_IVYBRIDGE(dev) || IS_HASWELL(dev))
			I915_WRITE(PF_CTL(pipe), PF_ENABLE | PF_FILTER_MED_3x3 |
						 PF_PIPE_SEL_IVB(pipe));
		else
			I915_WRITE(PF_CTL(pipe), PF_ENABLE | PF_FILTER_MED_3x3);
		I915_WRITE(PF_WIN_POS(pipe), crtc->config.pch_pfit.pos);
		I915_WRITE(PF_WIN_SZ(pipe), crtc->config.pch_pfit.size);
	}
}

static void intel_enable_planes(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	enum pipe pipe = to_intel_crtc(crtc)->pipe;
	struct intel_plane *intel_plane;

	list_for_each_entry(intel_plane, &dev->mode_config.plane_list, base.head)
		if (intel_plane->pipe == pipe)
			intel_plane_restore(&intel_plane->base);
}

static void intel_disable_planes(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	enum pipe pipe = to_intel_crtc(crtc)->pipe;
	struct intel_plane *intel_plane;

	list_for_each_entry(intel_plane, &dev->mode_config.plane_list, base.head)
		if (intel_plane->pipe == pipe)
			intel_plane_disable(&intel_plane->base);
}

void hsw_enable_ips(struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = crtc->base.dev->dev_private;

	if (!crtc->config.ips_enabled)
		return;

	/* We can only enable IPS after we enable a plane and wait for a vblank.
	 * We guarantee that the plane is enabled by calling intel_enable_ips
	 * only after intel_enable_plane. And intel_enable_plane already waits
	 * for a vblank, so all we need to do here is to enable the IPS bit. */
	assert_plane_enabled(dev_priv, crtc->plane);
	if (IS_BROADWELL(crtc->base.dev)) {
		mutex_lock(&dev_priv->rps.hw_lock);
		WARN_ON(sandybridge_pcode_write(dev_priv, DISPLAY_IPS_CONTROL, 0xc0000000));
		mutex_unlock(&dev_priv->rps.hw_lock);
		/* Quoting Art Runyan: "its not safe to expect any particular
		 * value in IPS_CTL bit 31 after enabling IPS through the
		 * mailbox." Therefore we need to defer waiting on the state
		 * change.
		 * TODO: need to fix this for state checker
		 */
	} else {
		I915_WRITE(IPS_CTL, IPS_ENABLE);
		/* The bit only becomes 1 in the next vblank, so this wait here
		 * is essentially intel_wait_for_vblank. If we don't have this
		 * and don't wait for vblanks until the end of crtc_enable, then
		 * the HW state readout code will complain that the expected
		 * IPS_CTL value is not the one we read. */
		if (wait_for(I915_READ_NOTRACE(IPS_CTL) & IPS_ENABLE, 50))
			DRM_ERROR("Timed out waiting for IPS enable\n");
	}
}

void hsw_disable_ips(struct intel_crtc *crtc)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (!crtc->config.ips_enabled)
		return;

	assert_plane_enabled(dev_priv, crtc->plane);
	if (IS_BROADWELL(crtc->base.dev)) {
		mutex_lock(&dev_priv->rps.hw_lock);
		WARN_ON(sandybridge_pcode_write(dev_priv, DISPLAY_IPS_CONTROL, 0));
		mutex_unlock(&dev_priv->rps.hw_lock);
	} else
		I915_WRITE(IPS_CTL, 0);
	POSTING_READ(IPS_CTL);

	/* We need to wait for a vblank before we can disable the plane. */
	intel_wait_for_vblank(dev, crtc->pipe);
}

/** Loads the palette/gamma unit for the CRTC with the prepared values */
static void intel_crtc_load_lut(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	enum pipe pipe = intel_crtc->pipe;
	int palreg = PALETTE(pipe);
	int i;
	bool reenable_ips = false;

	/* The clocks have to be on to load the palette. */
	if (!crtc->enabled || !intel_crtc->active)
		return;

	if (!HAS_PCH_SPLIT(dev_priv->dev)) {
		if (intel_pipe_has_type(crtc, INTEL_OUTPUT_DSI))
			assert_dsi_pll_enabled(dev_priv);
		else
			assert_pll_enabled(dev_priv, pipe);
	}

	/* use legacy palette for Ironlake */
	if (HAS_PCH_SPLIT(dev))
		palreg = LGC_PALETTE(pipe);

	/* Workaround : Do not read or write the pipe palette/gamma data while
	 * GAMMA_MODE is configured for split gamma and IPS_CTL has IPS enabled.
	 */
	if (intel_crtc->config.ips_enabled &&
	    ((I915_READ(GAMMA_MODE(pipe)) & GAMMA_MODE_MODE_MASK) ==
	     GAMMA_MODE_MODE_SPLIT)) {
		hsw_disable_ips(intel_crtc);
		reenable_ips = true;
	}

	for (i = 0; i < 256; i++) {
		I915_WRITE(palreg + 4 * i,
			   (intel_crtc->lut_r[i] << 16) |
			   (intel_crtc->lut_g[i] << 8) |
			   intel_crtc->lut_b[i]);
	}

	if (reenable_ips)
		hsw_enable_ips(intel_crtc);
}

static void ironlake_crtc_enable(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_encoder *encoder;
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;

	WARN_ON(!crtc->enabled);

	if (intel_crtc->active)
		return;

	intel_crtc->active = true;

	intel_set_cpu_fifo_underrun_reporting(dev, pipe, true);
	intel_set_pch_fifo_underrun_reporting(dev, pipe, true);

	for_each_encoder_on_crtc(dev, crtc, encoder)
		if (encoder->pre_enable)
			encoder->pre_enable(encoder);

	if (intel_crtc->config.has_pch_encoder) {
		/* Note: FDI PLL enabling _must_ be done before we enable the
		 * cpu pipes, hence this is separate from all the other fdi/pch
		 * enabling. */
		ironlake_fdi_pll_enable(intel_crtc);
	} else {
		assert_fdi_tx_disabled(dev_priv, pipe);
		assert_fdi_rx_disabled(dev_priv, pipe);
	}

	ironlake_pfit_enable(intel_crtc);

	/*
	 * On ILK+ LUT must be loaded before the pipe is running but with
	 * clocks enabled
	 */
	intel_crtc_load_lut(crtc);

	intel_update_watermarks(crtc);
	intel_enable_pipe(dev_priv, pipe,
			  intel_crtc->config.has_pch_encoder, false);
	intel_enable_primary_plane(dev_priv, plane, pipe);
	intel_enable_planes(crtc);
	intel_crtc_update_cursor(crtc, true);

	if (intel_crtc->config.has_pch_encoder)
		ironlake_pch_enable(crtc);

	mutex_lock(&dev->struct_mutex);
	intel_update_fbc(dev);
	mutex_unlock(&dev->struct_mutex);

	for_each_encoder_on_crtc(dev, crtc, encoder)
		encoder->enable(encoder);

	if (HAS_PCH_CPT(dev))
		cpt_verify_modeset(dev, intel_crtc->pipe);

	/*
	 * There seems to be a race in PCH platform hw (at least on some
	 * outputs) where an enabled pipe still completes any pageflip right
	 * away (as if the pipe is off) instead of waiting for vblank. As soon
	 * as the first vblank happend, everything works as expected. Hence just
	 * wait for one vblank before returning to avoid strange things
	 * happening.
	 */
	intel_wait_for_vblank(dev, intel_crtc->pipe);
}

/* IPS only exists on ULT machines and is tied to pipe A. */
static bool hsw_crtc_supports_ips(struct intel_crtc *crtc)
{
	return HAS_IPS(crtc->base.dev) && crtc->pipe == PIPE_A;
}

static void haswell_crtc_enable_planes(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;

	intel_enable_primary_plane(dev_priv, plane, pipe);
	intel_enable_planes(crtc);
	intel_crtc_update_cursor(crtc, true);

	hsw_enable_ips(intel_crtc);

	mutex_lock(&dev->struct_mutex);
	intel_update_fbc(dev);
	mutex_unlock(&dev->struct_mutex);
}

static void haswell_crtc_disable_planes(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;

	intel_crtc_wait_for_pending_flips(crtc);
	drm_vblank_off(dev, pipe);

	/* FBC must be disabled before disabling the plane on HSW. */
	if (dev_priv->fbc.plane == plane)
		intel_disable_fbc(dev);

	hsw_disable_ips(intel_crtc);

	intel_crtc_update_cursor(crtc, false);
	intel_disable_planes(crtc);
	intel_disable_primary_plane(dev_priv, plane, pipe);
}

/*
 * This implements the workaround described in the "notes" section of the mode
 * set sequence documentation. When going from no pipes or single pipe to
 * multiple pipes, and planes are enabled after the pipe, we need to wait at
 * least 2 vblanks on the first pipe before enabling planes on the second pipe.
 */
static void haswell_mode_set_planes_workaround(struct intel_crtc *crtc)
{
	struct drm_device *dev = crtc->base.dev;
	struct intel_crtc *crtc_it, *other_active_crtc = NULL;

	/* We want to get the other_active_crtc only if there's only 1 other
	 * active crtc. */
	list_for_each_entry(crtc_it, &dev->mode_config.crtc_list, base.head) {
		if (!crtc_it->active || crtc_it == crtc)
			continue;

		if (other_active_crtc)
			return;

		other_active_crtc = crtc_it;
	}
	if (!other_active_crtc)
		return;

	intel_wait_for_vblank(dev, other_active_crtc->pipe);
	intel_wait_for_vblank(dev, other_active_crtc->pipe);
}

static void haswell_crtc_enable(struct drm_crtc *crtc)
>>>>>>> refs/remotes/origin/master
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
<<<<<<< HEAD
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;
	u32 temp;
	bool is_pch_port;
=======
	struct intel_encoder *encoder;
	int pipe = intel_crtc->pipe;

	WARN_ON(!crtc->enabled);
>>>>>>> refs/remotes/origin/master

	if (intel_crtc->active)
		return;

	intel_crtc->active = true;
<<<<<<< HEAD
	intel_update_watermarks(dev);

	if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS)) {
		temp = I915_READ(PCH_LVDS);
		if ((temp & LVDS_PORT_EN) == 0)
			I915_WRITE(PCH_LVDS, temp | LVDS_PORT_EN);
	}

	is_pch_port = intel_crtc_driving_pch(crtc);

	if (is_pch_port)
		ironlake_fdi_pll_enable(crtc);
	else
		ironlake_fdi_disable(crtc);

	/* Enable panel fitting for LVDS */
	if (dev_priv->pch_pf_size &&
	    (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS) || HAS_eDP)) {
		/* Force use of hard-coded filter coefficients
		 * as some pre-programmed values are broken,
		 * e.g. x201.
		 */
		if (IS_IVYBRIDGE(dev))
			I915_WRITE(PF_CTL(pipe), PF_ENABLE | PF_FILTER_MED_3x3 |
						 PF_PIPE_SEL_IVB(pipe));
		else
			I915_WRITE(PF_CTL(pipe), PF_ENABLE | PF_FILTER_MED_3x3);
		I915_WRITE(PF_WIN_POS(pipe), dev_priv->pch_pf_pos);
		I915_WRITE(PF_WIN_SZ(pipe), dev_priv->pch_pf_size);
	}
=======

	intel_set_cpu_fifo_underrun_reporting(dev, pipe, true);
	if (intel_crtc->config.has_pch_encoder)
		intel_set_pch_fifo_underrun_reporting(dev, TRANSCODER_A, true);

	if (intel_crtc->config.has_pch_encoder)
		dev_priv->display.fdi_link_train(crtc);

	for_each_encoder_on_crtc(dev, crtc, encoder)
		if (encoder->pre_enable)
			encoder->pre_enable(encoder);

	intel_ddi_enable_pipe_clock(intel_crtc);

	ironlake_pfit_enable(intel_crtc);
>>>>>>> refs/remotes/origin/master

	/*
	 * On ILK+ LUT must be loaded before the pipe is running but with
	 * clocks enabled
	 */
	intel_crtc_load_lut(crtc);

<<<<<<< HEAD
	intel_enable_pipe(dev_priv, pipe, is_pch_port);
	intel_enable_plane(dev_priv, plane, pipe);

	if (is_pch_port)
		ironlake_pch_enable(crtc);

	mutex_lock(&dev->struct_mutex);
	intel_update_fbc(dev);
	mutex_unlock(&dev->struct_mutex);

	intel_crtc_update_cursor(crtc, true);
=======
	intel_ddi_set_pipe_settings(crtc);
	intel_ddi_enable_transcoder_func(crtc);

	intel_update_watermarks(crtc);
	intel_enable_pipe(dev_priv, pipe,
			  intel_crtc->config.has_pch_encoder, false);

	if (intel_crtc->config.has_pch_encoder)
		lpt_pch_enable(crtc);

	for_each_encoder_on_crtc(dev, crtc, encoder) {
		encoder->enable(encoder);
		intel_opregion_notify_encoder(encoder, true);
	}

	/* If we change the relative order between pipe/planes enabling, we need
	 * to change the workaround. */
	haswell_mode_set_planes_workaround(intel_crtc);
	haswell_crtc_enable_planes(crtc);

	/*
	 * There seems to be a race in PCH platform hw (at least on some
	 * outputs) where an enabled pipe still completes any pageflip right
	 * away (as if the pipe is off) instead of waiting for vblank. As soon
	 * as the first vblank happend, everything works as expected. Hence just
	 * wait for one vblank before returning to avoid strange things
	 * happening.
	 */
	intel_wait_for_vblank(dev, intel_crtc->pipe);
}

static void ironlake_pfit_disable(struct intel_crtc *crtc)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipe = crtc->pipe;

	/* To avoid upsetting the power well on haswell only disable the pfit if
	 * it's in use. The hw state code will make sure we get this right. */
	if (crtc->config.pch_pfit.enabled) {
		I915_WRITE(PF_CTL(pipe), 0);
		I915_WRITE(PF_WIN_POS(pipe), 0);
		I915_WRITE(PF_WIN_SZ(pipe), 0);
	}
>>>>>>> refs/remotes/origin/master
}

static void ironlake_crtc_disable(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
<<<<<<< HEAD
=======
	struct intel_encoder *encoder;
>>>>>>> refs/remotes/origin/master
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;
	u32 reg, temp;

<<<<<<< HEAD
	if (!intel_crtc->active)
		return;

	intel_crtc_wait_for_pending_flips(crtc);
	drm_vblank_off(dev, pipe);
	intel_crtc_update_cursor(crtc, false);

	intel_disable_plane(dev_priv, plane, pipe);

<<<<<<< HEAD
	if (dev_priv->cfb_plane == plane &&
	    dev_priv->display.disable_fbc)
		dev_priv->display.disable_fbc(dev);
=======
	if (dev_priv->cfb_plane == plane)
		intel_disable_fbc(dev);
>>>>>>> refs/remotes/origin/cm-10.0

	intel_disable_pipe(dev_priv, pipe);

	/* Disable PF */
	I915_WRITE(PF_CTL(pipe), 0);
	I915_WRITE(PF_WIN_SZ(pipe), 0);

	ironlake_fdi_disable(crtc);

	/* This is a horrible layering violation; we should be doing this in
	 * the connector/encoder ->prepare instead, but we don't always have
	 * enough information there about the config to know whether it will
	 * actually be necessary or just cause undesired flicker.
	 */
	intel_disable_pch_ports(dev_priv, pipe);

	intel_disable_transcoder(dev_priv, pipe);

	if (HAS_PCH_CPT(dev)) {
		/* disable TRANS_DP_CTL */
		reg = TRANS_DP_CTL(pipe);
		temp = I915_READ(reg);
		temp &= ~(TRANS_DP_OUTPUT_ENABLE | TRANS_DP_PORT_SEL_MASK);
		temp |= TRANS_DP_PORT_SEL_NONE;
		I915_WRITE(reg, temp);

		/* disable DPLL_SEL */
		temp = I915_READ(PCH_DPLL_SEL);
		switch (pipe) {
		case 0:
<<<<<<< HEAD
			temp &= ~(TRANSA_DPLL_ENABLE | TRANSA_DPLLA_SEL);
=======
			temp &= ~(TRANSA_DPLL_ENABLE | TRANSA_DPLLB_SEL);
>>>>>>> refs/remotes/origin/cm-10.0
			break;
		case 1:
			temp &= ~(TRANSB_DPLL_ENABLE | TRANSB_DPLLB_SEL);
			break;
		case 2:
<<<<<<< HEAD
			/* FIXME: manage transcoder PLLs? */
=======
			/* C shares PLL A or B */
>>>>>>> refs/remotes/origin/cm-10.0
			temp &= ~(TRANSC_DPLL_ENABLE | TRANSC_DPLLB_SEL);
			break;
		default:
			BUG(); /* wtf */
		}
		I915_WRITE(PCH_DPLL_SEL, temp);
	}

	/* disable PCH DPLL */
<<<<<<< HEAD
	intel_disable_pch_pll(dev_priv, pipe);
=======
	if (!intel_crtc->no_pll)
		intel_disable_pch_pll(dev_priv, pipe);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Switch from PCDclk to Rawclk */
	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
	I915_WRITE(reg, temp & ~FDI_PCDCLK);

	/* Disable CPU FDI TX PLL */
	reg = FDI_TX_CTL(pipe);
	temp = I915_READ(reg);
	I915_WRITE(reg, temp & ~FDI_TX_PLL_ENABLE);

	POSTING_READ(reg);
	udelay(100);

	reg = FDI_RX_CTL(pipe);
	temp = I915_READ(reg);
	I915_WRITE(reg, temp & ~FDI_RX_PLL_ENABLE);

	/* Wait for the clocks to turn off. */
	POSTING_READ(reg);
	udelay(100);

	intel_crtc->active = false;
	intel_update_watermarks(dev);

	mutex_lock(&dev->struct_mutex);
	intel_update_fbc(dev);
	intel_clear_scanline_wait(dev);
	mutex_unlock(&dev->struct_mutex);
}

static void ironlake_crtc_dpms(struct drm_crtc *crtc, int mode)
{
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;

	/* XXX: When our outputs are all unaware of DPMS modes other than off
	 * and on, we should map those modes to DRM_MODE_DPMS_OFF in the CRTC.
	 */
	switch (mode) {
	case DRM_MODE_DPMS_ON:
	case DRM_MODE_DPMS_STANDBY:
	case DRM_MODE_DPMS_SUSPEND:
		DRM_DEBUG_KMS("crtc %d/%d dpms on\n", pipe, plane);
		ironlake_crtc_enable(crtc);
		break;

	case DRM_MODE_DPMS_OFF:
		DRM_DEBUG_KMS("crtc %d/%d dpms off\n", pipe, plane);
		ironlake_crtc_disable(crtc);
		break;
	}
=======

	if (!intel_crtc->active)
		return;

	for_each_encoder_on_crtc(dev, crtc, encoder)
		encoder->disable(encoder);

	intel_crtc_wait_for_pending_flips(crtc);
	drm_vblank_off(dev, pipe);

	if (dev_priv->fbc.plane == plane)
		intel_disable_fbc(dev);

	intel_crtc_update_cursor(crtc, false);
	intel_disable_planes(crtc);
	intel_disable_primary_plane(dev_priv, plane, pipe);

	if (intel_crtc->config.has_pch_encoder)
		intel_set_pch_fifo_underrun_reporting(dev, pipe, false);

	intel_disable_pipe(dev_priv, pipe);

	ironlake_pfit_disable(intel_crtc);

	for_each_encoder_on_crtc(dev, crtc, encoder)
		if (encoder->post_disable)
			encoder->post_disable(encoder);

	if (intel_crtc->config.has_pch_encoder) {
		ironlake_fdi_disable(crtc);

		ironlake_disable_pch_transcoder(dev_priv, pipe);
		intel_set_pch_fifo_underrun_reporting(dev, pipe, true);

		if (HAS_PCH_CPT(dev)) {
			/* disable TRANS_DP_CTL */
			reg = TRANS_DP_CTL(pipe);
			temp = I915_READ(reg);
			temp &= ~(TRANS_DP_OUTPUT_ENABLE |
				  TRANS_DP_PORT_SEL_MASK);
			temp |= TRANS_DP_PORT_SEL_NONE;
			I915_WRITE(reg, temp);

			/* disable DPLL_SEL */
			temp = I915_READ(PCH_DPLL_SEL);
			temp &= ~(TRANS_DPLL_ENABLE(pipe) | TRANS_DPLLB_SEL(pipe));
			I915_WRITE(PCH_DPLL_SEL, temp);
		}

		/* disable PCH DPLL */
		intel_disable_shared_dpll(intel_crtc);

		ironlake_fdi_pll_disable(intel_crtc);
	}

	intel_crtc->active = false;
	intel_update_watermarks(crtc);

	mutex_lock(&dev->struct_mutex);
	intel_update_fbc(dev);
	mutex_unlock(&dev->struct_mutex);
}

static void haswell_crtc_disable(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_encoder *encoder;
	int pipe = intel_crtc->pipe;
	enum transcoder cpu_transcoder = intel_crtc->config.cpu_transcoder;

	if (!intel_crtc->active)
		return;

	haswell_crtc_disable_planes(crtc);

	for_each_encoder_on_crtc(dev, crtc, encoder) {
		intel_opregion_notify_encoder(encoder, false);
		encoder->disable(encoder);
	}

	if (intel_crtc->config.has_pch_encoder)
		intel_set_pch_fifo_underrun_reporting(dev, TRANSCODER_A, false);
	intel_disable_pipe(dev_priv, pipe);

	intel_ddi_disable_transcoder_func(dev_priv, cpu_transcoder);

	ironlake_pfit_disable(intel_crtc);

	intel_ddi_disable_pipe_clock(intel_crtc);

	for_each_encoder_on_crtc(dev, crtc, encoder)
		if (encoder->post_disable)
			encoder->post_disable(encoder);

	if (intel_crtc->config.has_pch_encoder) {
		lpt_disable_pch_transcoder(dev_priv);
		intel_set_pch_fifo_underrun_reporting(dev, TRANSCODER_A, true);
		intel_ddi_fdi_disable(crtc);
	}

	intel_crtc->active = false;
	intel_update_watermarks(crtc);

	mutex_lock(&dev->struct_mutex);
	intel_update_fbc(dev);
	mutex_unlock(&dev->struct_mutex);
}

static void ironlake_crtc_off(struct drm_crtc *crtc)
{
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	intel_put_shared_dpll(intel_crtc);
}

static void haswell_crtc_off(struct drm_crtc *crtc)
{
	intel_ddi_put_crtc_pll(crtc);
>>>>>>> refs/remotes/origin/master
}

static void intel_crtc_dpms_overlay(struct intel_crtc *intel_crtc, bool enable)
{
	if (!enable && intel_crtc->overlay) {
		struct drm_device *dev = intel_crtc->base.dev;
		struct drm_i915_private *dev_priv = dev->dev_private;

		mutex_lock(&dev->struct_mutex);
		dev_priv->mm.interruptible = false;
		(void) intel_overlay_switch_off(intel_crtc->overlay);
		dev_priv->mm.interruptible = true;
		mutex_unlock(&dev->struct_mutex);
	}

	/* Let userspace switch the overlay on again. In most cases userspace
	 * has to recompute where to put it anyway.
	 */
}

<<<<<<< HEAD
static void i9xx_crtc_enable(struct drm_crtc *crtc)
=======
/**
 * i9xx_fixup_plane - ugly workaround for G45 to fire up the hardware
 * cursor plane briefly if not already running after enabling the display
 * plane.
 * This workaround avoids occasional blank screens when self refresh is
 * enabled.
 */
static void
g4x_fixup_plane(struct drm_i915_private *dev_priv, enum pipe pipe)
{
	u32 cntl = I915_READ(CURCNTR(pipe));

	if ((cntl & CURSOR_MODE) == 0) {
		u32 fw_bcl_self = I915_READ(FW_BLC_SELF);

		I915_WRITE(FW_BLC_SELF, fw_bcl_self & ~FW_BLC_SELF_EN);
		I915_WRITE(CURCNTR(pipe), CURSOR_MODE_64_ARGB_AX);
		intel_wait_for_vblank(dev_priv->dev, pipe);
		I915_WRITE(CURCNTR(pipe), cntl);
		I915_WRITE(CURBASE(pipe), I915_READ(CURBASE(pipe)));
		I915_WRITE(FW_BLC_SELF, fw_bcl_self);
	}
}

static void i9xx_pfit_enable(struct intel_crtc *crtc)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc_config *pipe_config = &crtc->config;

	if (!crtc->config.gmch_pfit.control)
		return;

	/*
	 * The panel fitter should only be adjusted whilst the pipe is disabled,
	 * according to register description and PRM.
	 */
	WARN_ON(I915_READ(PFIT_CONTROL) & PFIT_ENABLE);
	assert_pipe_disabled(dev_priv, crtc->pipe);

	I915_WRITE(PFIT_PGM_RATIOS, pipe_config->gmch_pfit.pgm_ratios);
	I915_WRITE(PFIT_CONTROL, pipe_config->gmch_pfit.control);

	/* Border color in case we don't scale up to the full screen. Black by
	 * default, change to something else for debugging. */
	I915_WRITE(BCLRPAT(crtc->pipe), 0);
}

static void valleyview_crtc_enable(struct drm_crtc *crtc)
>>>>>>> refs/remotes/origin/master
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
<<<<<<< HEAD
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;
=======
	struct intel_encoder *encoder;
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;
	bool is_dsi;

	WARN_ON(!crtc->enabled);
>>>>>>> refs/remotes/origin/master

	if (intel_crtc->active)
		return;

	intel_crtc->active = true;
<<<<<<< HEAD
	intel_update_watermarks(dev);

	intel_enable_pll(dev_priv, pipe);
	intel_enable_pipe(dev_priv, pipe, false);
	intel_enable_plane(dev_priv, plane, pipe);

	intel_crtc_load_lut(crtc);
	intel_update_fbc(dev);

	/* Give the overlay scaler a chance to enable if it's on this pipe */
	intel_crtc_dpms_overlay(intel_crtc, true);
	intel_crtc_update_cursor(crtc, true);
=======

	for_each_encoder_on_crtc(dev, crtc, encoder)
		if (encoder->pre_pll_enable)
			encoder->pre_pll_enable(encoder);

	is_dsi = intel_pipe_has_type(crtc, INTEL_OUTPUT_DSI);

	if (!is_dsi)
		vlv_enable_pll(intel_crtc);

	for_each_encoder_on_crtc(dev, crtc, encoder)
		if (encoder->pre_enable)
			encoder->pre_enable(encoder);

	i9xx_pfit_enable(intel_crtc);

	intel_crtc_load_lut(crtc);

	intel_update_watermarks(crtc);
	intel_enable_pipe(dev_priv, pipe, false, is_dsi);
	intel_enable_primary_plane(dev_priv, plane, pipe);
	intel_enable_planes(crtc);
	intel_crtc_update_cursor(crtc, true);

	intel_update_fbc(dev);

	for_each_encoder_on_crtc(dev, crtc, encoder)
		encoder->enable(encoder);
}

static void i9xx_crtc_enable(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_encoder *encoder;
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;

	WARN_ON(!crtc->enabled);

	if (intel_crtc->active)
		return;

	intel_crtc->active = true;

	for_each_encoder_on_crtc(dev, crtc, encoder)
		if (encoder->pre_enable)
			encoder->pre_enable(encoder);

	i9xx_enable_pll(intel_crtc);

	i9xx_pfit_enable(intel_crtc);

	intel_crtc_load_lut(crtc);

	intel_update_watermarks(crtc);
	intel_enable_pipe(dev_priv, pipe, false, false);
	intel_enable_primary_plane(dev_priv, plane, pipe);
	intel_enable_planes(crtc);
	/* The fixup needs to happen before cursor is enabled */
	if (IS_G4X(dev))
		g4x_fixup_plane(dev_priv, pipe);
	intel_crtc_update_cursor(crtc, true);

	/* Give the overlay scaler a chance to enable if it's on this pipe */
	intel_crtc_dpms_overlay(intel_crtc, true);

	intel_update_fbc(dev);

	for_each_encoder_on_crtc(dev, crtc, encoder)
		encoder->enable(encoder);
}

static void i9xx_pfit_disable(struct intel_crtc *crtc)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (!crtc->config.gmch_pfit.control)
		return;

	assert_pipe_disabled(dev_priv, crtc->pipe);

	DRM_DEBUG_DRIVER("disabling pfit, current: 0x%08x\n",
			 I915_READ(PFIT_CONTROL));
	I915_WRITE(PFIT_CONTROL, 0);
>>>>>>> refs/remotes/origin/master
}

static void i9xx_crtc_disable(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
<<<<<<< HEAD
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;
	u32 pctl;
=======
	struct intel_encoder *encoder;
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;
>>>>>>> refs/remotes/origin/master

	if (!intel_crtc->active)
		return;

<<<<<<< HEAD
	/* Give the overlay scaler a chance to disable if it's on this pipe */
	intel_crtc_wait_for_pending_flips(crtc);
	drm_vblank_off(dev, pipe);
	intel_crtc_dpms_overlay(intel_crtc, false);
	intel_crtc_update_cursor(crtc, false);

<<<<<<< HEAD
	if (dev_priv->cfb_plane == plane &&
	    dev_priv->display.disable_fbc)
		dev_priv->display.disable_fbc(dev);
=======
	if (dev_priv->cfb_plane == plane)
		intel_disable_fbc(dev);
>>>>>>> refs/remotes/origin/cm-10.0

	intel_disable_plane(dev_priv, plane, pipe);
	intel_disable_pipe(dev_priv, pipe);

	/* Disable pannel fitter if it is on this pipe. */
	pctl = I915_READ(PFIT_CONTROL);
	if ((pctl & PFIT_ENABLE) &&
	    ((pctl & PFIT_PIPE_MASK) >> PFIT_PIPE_SHIFT) == pipe)
		I915_WRITE(PFIT_CONTROL, 0);

	intel_disable_pll(dev_priv, pipe);

	intel_crtc->active = false;
	intel_update_fbc(dev);
	intel_update_watermarks(dev);
	intel_clear_scanline_wait(dev);
}

static void i9xx_crtc_dpms(struct drm_crtc *crtc, int mode)
{
	/* XXX: When our outputs are all unaware of DPMS modes other than off
	 * and on, we should map those modes to DRM_MODE_DPMS_OFF in the CRTC.
	 */
	switch (mode) {
	case DRM_MODE_DPMS_ON:
	case DRM_MODE_DPMS_STANDBY:
	case DRM_MODE_DPMS_SUSPEND:
		i9xx_crtc_enable(crtc);
		break;
	case DRM_MODE_DPMS_OFF:
		i9xx_crtc_disable(crtc);
		break;
	}
}

/**
 * Sets the power management mode of the pipe and plane.
 */
static void intel_crtc_dpms(struct drm_crtc *crtc, int mode)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_i915_master_private *master_priv;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	bool enabled;

	if (intel_crtc->dpms_mode == mode)
		return;

	intel_crtc->dpms_mode = mode;

	dev_priv->display.dpms(crtc, mode);
=======
	for_each_encoder_on_crtc(dev, crtc, encoder)
		encoder->disable(encoder);

	/* Give the overlay scaler a chance to disable if it's on this pipe */
	intel_crtc_wait_for_pending_flips(crtc);
	drm_vblank_off(dev, pipe);

	if (dev_priv->fbc.plane == plane)
		intel_disable_fbc(dev);

	intel_crtc_dpms_overlay(intel_crtc, false);
	intel_crtc_update_cursor(crtc, false);
	intel_disable_planes(crtc);
	intel_disable_primary_plane(dev_priv, plane, pipe);

	intel_disable_pipe(dev_priv, pipe);

	i9xx_pfit_disable(intel_crtc);

	for_each_encoder_on_crtc(dev, crtc, encoder)
		if (encoder->post_disable)
			encoder->post_disable(encoder);

	if (IS_VALLEYVIEW(dev) && !intel_pipe_has_type(crtc, INTEL_OUTPUT_DSI))
		vlv_disable_pll(dev_priv, pipe);
	else if (!IS_VALLEYVIEW(dev))
		i9xx_disable_pll(dev_priv, pipe);

	intel_crtc->active = false;
	intel_update_watermarks(crtc);

	intel_update_fbc(dev);
}

static void i9xx_crtc_off(struct drm_crtc *crtc)
{
}

static void intel_crtc_update_sarea(struct drm_crtc *crtc,
				    bool enabled)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_master_private *master_priv;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
>>>>>>> refs/remotes/origin/master

	if (!dev->primary->master)
		return;

	master_priv = dev->primary->master->driver_priv;
	if (!master_priv->sarea_priv)
		return;

<<<<<<< HEAD
	enabled = crtc->enabled && mode != DRM_MODE_DPMS_OFF;

=======
>>>>>>> refs/remotes/origin/master
	switch (pipe) {
	case 0:
		master_priv->sarea_priv->pipeA_w = enabled ? crtc->mode.hdisplay : 0;
		master_priv->sarea_priv->pipeA_h = enabled ? crtc->mode.vdisplay : 0;
		break;
	case 1:
		master_priv->sarea_priv->pipeB_w = enabled ? crtc->mode.hdisplay : 0;
		master_priv->sarea_priv->pipeB_h = enabled ? crtc->mode.vdisplay : 0;
		break;
	default:
		DRM_ERROR("Can't update pipe %c in SAREA\n", pipe_name(pipe));
		break;
	}
}

<<<<<<< HEAD
static void intel_crtc_disable(struct drm_crtc *crtc)
{
	struct drm_crtc_helper_funcs *crtc_funcs = crtc->helper_private;
	struct drm_device *dev = crtc->dev;

	crtc_funcs->dpms(crtc, DRM_MODE_DPMS_OFF);
<<<<<<< HEAD

	if (crtc->fb) {
		mutex_lock(&dev->struct_mutex);
		i915_gem_object_unpin(to_intel_framebuffer(crtc->fb)->obj);
=======
	assert_plane_disabled(dev->dev_private, to_intel_crtc(crtc)->plane);
=======
/**
 * Sets the power management mode of the pipe and plane.
 */
void intel_crtc_update_dpms(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_encoder *intel_encoder;
	bool enable = false;

	for_each_encoder_on_crtc(dev, crtc, intel_encoder)
		enable |= intel_encoder->connectors_active;

	if (enable)
		dev_priv->display.crtc_enable(crtc);
	else
		dev_priv->display.crtc_disable(crtc);

	intel_crtc_update_sarea(crtc, enable);
}

static void intel_crtc_disable(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_connector *connector;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);

	/* crtc should still be enabled when we disable it. */
	WARN_ON(!crtc->enabled);

	dev_priv->display.crtc_disable(crtc);
	intel_crtc->eld_vld = false;
	intel_crtc_update_sarea(crtc, false);
	dev_priv->display.off(crtc);

	assert_plane_disabled(dev->dev_private, to_intel_crtc(crtc)->plane);
	assert_cursor_disabled(dev_priv, to_intel_crtc(crtc)->pipe);
>>>>>>> refs/remotes/origin/master
	assert_pipe_disabled(dev->dev_private, to_intel_crtc(crtc)->pipe);

	if (crtc->fb) {
		mutex_lock(&dev->struct_mutex);
		intel_unpin_fb_obj(to_intel_framebuffer(crtc->fb)->obj);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		mutex_unlock(&dev->struct_mutex);
	}
}

/* Prepare for a mode set.
 *
 * Note we could be a lot smarter here.  We need to figure out which outputs
 * will be enabled, which disabled (in short, how the config will changes)
 * and perform the minimum necessary steps to accomplish that, e.g. updating
 * watermarks, FBC configuration, making sure PLLs are programmed correctly,
 * panel fitting is in the proper state, etc.
 */
static void i9xx_crtc_prepare(struct drm_crtc *crtc)
{
	i9xx_crtc_disable(crtc);
}

static void i9xx_crtc_commit(struct drm_crtc *crtc)
{
	i9xx_crtc_enable(crtc);
}

static void ironlake_crtc_prepare(struct drm_crtc *crtc)
{
	ironlake_crtc_disable(crtc);
}

static void ironlake_crtc_commit(struct drm_crtc *crtc)
{
	ironlake_crtc_enable(crtc);
}

<<<<<<< HEAD
void intel_encoder_prepare (struct drm_encoder *encoder)
=======
void intel_encoder_prepare(struct drm_encoder *encoder)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct drm_encoder_helper_funcs *encoder_funcs = encoder->helper_private;
	/* lvds has its own version of prepare see intel_lvds_prepare */
	encoder_funcs->dpms(encoder, DRM_MODE_DPMS_OFF);
}

<<<<<<< HEAD
void intel_encoder_commit (struct drm_encoder *encoder)
{
	struct drm_encoder_helper_funcs *encoder_funcs = encoder->helper_private;
	/* lvds has its own version of commit see intel_lvds_commit */
	encoder_funcs->dpms(encoder, DRM_MODE_DPMS_ON);
=======
void intel_encoder_commit(struct drm_encoder *encoder)
{
	struct drm_encoder_helper_funcs *encoder_funcs = encoder->helper_private;
	struct drm_device *dev = encoder->dev;
	struct intel_encoder *intel_encoder = to_intel_encoder(encoder);
	struct intel_crtc *intel_crtc = to_intel_crtc(intel_encoder->base.crtc);

	/* lvds has its own version of commit see intel_lvds_commit */
	encoder_funcs->dpms(encoder, DRM_MODE_DPMS_ON);

	if (HAS_PCH_CPT(dev))
		intel_cpt_verify_modeset(dev, intel_crtc->pipe);
>>>>>>> refs/remotes/origin/cm-10.0
}

void intel_encoder_destroy(struct drm_encoder *encoder)
{
	struct intel_encoder *intel_encoder = to_intel_encoder(encoder);

	drm_encoder_cleanup(encoder);
	kfree(intel_encoder);
}

static bool intel_crtc_mode_fixup(struct drm_crtc *crtc,
				  struct drm_display_mode *mode,
				  struct drm_display_mode *adjusted_mode)
{
	struct drm_device *dev = crtc->dev;

	if (HAS_PCH_SPLIT(dev)) {
		/* FDI link clock is fixed at 2.7G */
		if (mode->clock * 3 > IRONLAKE_FDI_FREQ * 4)
			return false;
	}

<<<<<<< HEAD
	/* XXX some encoders set the crtcinfo, others don't.
	 * Obviously we need some form of conflict resolution here...
	 */
	if (adjusted_mode->crtc_htotal == 0)
=======
	/* All interlaced capable intel hw wants timings in frames. Note though
	 * that intel_lvds_mode_fixup does some funny tricks with the crtc
	 * timings, so we need to be careful not to clobber these.*/
	if (!(adjusted_mode->private_flags & INTEL_MODE_CRTC_TIMINGS_SET))
>>>>>>> refs/remotes/origin/cm-10.0
		drm_mode_set_crtcinfo(adjusted_mode, 0);

	return true;
=======
		mutex_unlock(&dev->struct_mutex);
		crtc->fb = NULL;
	}

	/* Update computed state. */
	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		if (!connector->encoder || !connector->encoder->crtc)
			continue;

		if (connector->encoder->crtc != crtc)
			continue;

		connector->dpms = DRM_MODE_DPMS_OFF;
		to_intel_encoder(connector->encoder)->connectors_active = false;
	}
}

void intel_encoder_destroy(struct drm_encoder *encoder)
{
	struct intel_encoder *intel_encoder = to_intel_encoder(encoder);

	drm_encoder_cleanup(encoder);
	kfree(intel_encoder);
}

/* Simple dpms helper for encoders with just one connector, no cloning and only
 * one kind of off state. It clamps all !ON modes to fully OFF and changes the
 * state of the entire output pipe. */
static void intel_encoder_dpms(struct intel_encoder *encoder, int mode)
{
	if (mode == DRM_MODE_DPMS_ON) {
		encoder->connectors_active = true;

		intel_crtc_update_dpms(encoder->base.crtc);
	} else {
		encoder->connectors_active = false;

		intel_crtc_update_dpms(encoder->base.crtc);
	}
}

/* Cross check the actual hw state with our own modeset state tracking (and it's
 * internal consistency). */
static void intel_connector_check_state(struct intel_connector *connector)
{
	if (connector->get_hw_state(connector)) {
		struct intel_encoder *encoder = connector->encoder;
		struct drm_crtc *crtc;
		bool encoder_enabled;
		enum pipe pipe;

		DRM_DEBUG_KMS("[CONNECTOR:%d:%s]\n",
			      connector->base.base.id,
			      drm_get_connector_name(&connector->base));

		WARN(connector->base.dpms == DRM_MODE_DPMS_OFF,
		     "wrong connector dpms state\n");
		WARN(connector->base.encoder != &encoder->base,
		     "active connector not linked to encoder\n");
		WARN(!encoder->connectors_active,
		     "encoder->connectors_active not set\n");

		encoder_enabled = encoder->get_hw_state(encoder, &pipe);
		WARN(!encoder_enabled, "encoder not enabled\n");
		if (WARN_ON(!encoder->base.crtc))
			return;

		crtc = encoder->base.crtc;

		WARN(!crtc->enabled, "crtc not enabled\n");
		WARN(!to_intel_crtc(crtc)->active, "crtc not active\n");
		WARN(pipe != to_intel_crtc(crtc)->pipe,
		     "encoder active on the wrong pipe\n");
	}
}

/* Even simpler default implementation, if there's really no special case to
 * consider. */
void intel_connector_dpms(struct drm_connector *connector, int mode)
{
	/* All the simple cases only support two dpms states. */
	if (mode != DRM_MODE_DPMS_ON)
		mode = DRM_MODE_DPMS_OFF;

	if (mode == connector->dpms)
		return;

	connector->dpms = mode;

	/* Only need to change hw state when actually enabled */
	if (connector->encoder)
		intel_encoder_dpms(to_intel_encoder(connector->encoder), mode);

	intel_modeset_check_state(connector->dev);
}

/* Simple connector->get_hw_state implementation for encoders that support only
 * one connector and no cloning and hence the encoder state determines the state
 * of the connector. */
bool intel_connector_get_hw_state(struct intel_connector *connector)
{
	enum pipe pipe = 0;
	struct intel_encoder *encoder = connector->encoder;

	return encoder->get_hw_state(encoder, &pipe);
}

static bool ironlake_check_fdi_lanes(struct drm_device *dev, enum pipe pipe,
				     struct intel_crtc_config *pipe_config)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *pipe_B_crtc =
		to_intel_crtc(dev_priv->pipe_to_crtc_mapping[PIPE_B]);

	DRM_DEBUG_KMS("checking fdi config on pipe %c, lanes %i\n",
		      pipe_name(pipe), pipe_config->fdi_lanes);
	if (pipe_config->fdi_lanes > 4) {
		DRM_DEBUG_KMS("invalid fdi lane config on pipe %c: %i lanes\n",
			      pipe_name(pipe), pipe_config->fdi_lanes);
		return false;
	}

	if (IS_HASWELL(dev) || IS_BROADWELL(dev)) {
		if (pipe_config->fdi_lanes > 2) {
			DRM_DEBUG_KMS("only 2 lanes on haswell, required: %i lanes\n",
				      pipe_config->fdi_lanes);
			return false;
		} else {
			return true;
		}
	}

	if (INTEL_INFO(dev)->num_pipes == 2)
		return true;

	/* Ivybridge 3 pipe is really complicated */
	switch (pipe) {
	case PIPE_A:
		return true;
	case PIPE_B:
		if (dev_priv->pipe_to_crtc_mapping[PIPE_C]->enabled &&
		    pipe_config->fdi_lanes > 2) {
			DRM_DEBUG_KMS("invalid shared fdi lane config on pipe %c: %i lanes\n",
				      pipe_name(pipe), pipe_config->fdi_lanes);
			return false;
		}
		return true;
	case PIPE_C:
		if (!pipe_has_enabled_pch(pipe_B_crtc) ||
		    pipe_B_crtc->config.fdi_lanes <= 2) {
			if (pipe_config->fdi_lanes > 2) {
				DRM_DEBUG_KMS("invalid shared fdi lane config on pipe %c: %i lanes\n",
					      pipe_name(pipe), pipe_config->fdi_lanes);
				return false;
			}
		} else {
			DRM_DEBUG_KMS("fdi link B uses too many lanes to enable link C\n");
			return false;
		}
		return true;
	default:
		BUG();
	}
}

#define RETRY 1
static int ironlake_fdi_compute_config(struct intel_crtc *intel_crtc,
				       struct intel_crtc_config *pipe_config)
{
	struct drm_device *dev = intel_crtc->base.dev;
	struct drm_display_mode *adjusted_mode = &pipe_config->adjusted_mode;
	int lane, link_bw, fdi_dotclock;
	bool setup_ok, needs_recompute = false;

retry:
	/* FDI is a binary signal running at ~2.7GHz, encoding
	 * each output octet as 10 bits. The actual frequency
	 * is stored as a divider into a 100MHz clock, and the
	 * mode pixel clock is stored in units of 1KHz.
	 * Hence the bw of each lane in terms of the mode signal
	 * is:
	 */
	link_bw = intel_fdi_link_freq(dev) * MHz(100)/KHz(1)/10;

	fdi_dotclock = adjusted_mode->crtc_clock;

	lane = ironlake_get_lanes_required(fdi_dotclock, link_bw,
					   pipe_config->pipe_bpp);

	pipe_config->fdi_lanes = lane;

	intel_link_compute_m_n(pipe_config->pipe_bpp, lane, fdi_dotclock,
			       link_bw, &pipe_config->fdi_m_n);

	setup_ok = ironlake_check_fdi_lanes(intel_crtc->base.dev,
					    intel_crtc->pipe, pipe_config);
	if (!setup_ok && pipe_config->pipe_bpp > 6*3) {
		pipe_config->pipe_bpp -= 2*3;
		DRM_DEBUG_KMS("fdi link bw constraint, reducing pipe bpp to %i\n",
			      pipe_config->pipe_bpp);
		needs_recompute = true;
		pipe_config->bw_constrained = true;

		goto retry;
	}

	if (needs_recompute)
		return RETRY;

	return setup_ok ? 0 : -EINVAL;
}

static void hsw_compute_ips_config(struct intel_crtc *crtc,
				   struct intel_crtc_config *pipe_config)
{
	pipe_config->ips_enabled = i915_enable_ips &&
				   hsw_crtc_supports_ips(crtc) &&
				   pipe_config->pipe_bpp <= 24;
}

static int intel_crtc_compute_config(struct intel_crtc *crtc,
				     struct intel_crtc_config *pipe_config)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_display_mode *adjusted_mode = &pipe_config->adjusted_mode;

	/* FIXME should check pixel clock limits on all platforms */
	if (INTEL_INFO(dev)->gen < 4) {
		struct drm_i915_private *dev_priv = dev->dev_private;
		int clock_limit =
			dev_priv->display.get_display_clock_speed(dev);

		/*
		 * Enable pixel doubling when the dot clock
		 * is > 90% of the (display) core speed.
		 *
		 * GDG double wide on either pipe,
		 * otherwise pipe A only.
		 */
		if ((crtc->pipe == PIPE_A || IS_I915G(dev)) &&
		    adjusted_mode->crtc_clock > clock_limit * 9 / 10) {
			clock_limit *= 2;
			pipe_config->double_wide = true;
		}

		if (adjusted_mode->crtc_clock > clock_limit * 9 / 10)
			return -EINVAL;
	}

	/*
	 * Pipe horizontal size must be even in:
	 * - DVO ganged mode
	 * - LVDS dual channel mode
	 * - Double wide pipe
	 */
	if ((intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_LVDS) &&
	     intel_is_dual_link_lvds(dev)) || pipe_config->double_wide)
		pipe_config->pipe_src_w &= ~1;

	/* Cantiga+ cannot handle modes with a hsync front porch of 0.
	 * WaPruneModeWithIncorrectHsyncOffset:ctg,elk,ilk,snb,ivb,vlv,hsw.
	 */
	if ((INTEL_INFO(dev)->gen > 4 || IS_G4X(dev)) &&
		adjusted_mode->hsync_start == adjusted_mode->hdisplay)
		return -EINVAL;

	if ((IS_G4X(dev) || IS_VALLEYVIEW(dev)) && pipe_config->pipe_bpp > 10*3) {
		pipe_config->pipe_bpp = 10*3; /* 12bpc is gen5+ */
	} else if (INTEL_INFO(dev)->gen <= 4 && pipe_config->pipe_bpp > 8*3) {
		/* only a 8bpc pipe, with 6bpc dither through the panel fitter
		 * for lvds. */
		pipe_config->pipe_bpp = 8*3;
	}

	if (HAS_IPS(dev))
		hsw_compute_ips_config(crtc, pipe_config);

	/* XXX: PCH clock sharing is done in ->mode_set, so make sure the old
	 * clock survives for now. */
	if (HAS_PCH_IBX(dev) || HAS_PCH_CPT(dev))
		pipe_config->shared_dpll = crtc->config.shared_dpll;

	if (pipe_config->has_pch_encoder)
		return ironlake_fdi_compute_config(crtc, pipe_config);

	return 0;
}

static int valleyview_get_display_clock_speed(struct drm_device *dev)
{
	return 400000; /* FIXME */
>>>>>>> refs/remotes/origin/master
}

static int i945_get_display_clock_speed(struct drm_device *dev)
{
	return 400000;
}

static int i915_get_display_clock_speed(struct drm_device *dev)
{
	return 333000;
}

static int i9xx_misc_get_display_clock_speed(struct drm_device *dev)
{
	return 200000;
}

<<<<<<< HEAD
=======
static int pnv_get_display_clock_speed(struct drm_device *dev)
{
	u16 gcfgc = 0;

	pci_read_config_word(dev->pdev, GCFGC, &gcfgc);

	switch (gcfgc & GC_DISPLAY_CLOCK_MASK) {
	case GC_DISPLAY_CLOCK_267_MHZ_PNV:
		return 267000;
	case GC_DISPLAY_CLOCK_333_MHZ_PNV:
		return 333000;
	case GC_DISPLAY_CLOCK_444_MHZ_PNV:
		return 444000;
	case GC_DISPLAY_CLOCK_200_MHZ_PNV:
		return 200000;
	default:
		DRM_ERROR("Unknown pnv display core clock 0x%04x\n", gcfgc);
	case GC_DISPLAY_CLOCK_133_MHZ_PNV:
		return 133000;
	case GC_DISPLAY_CLOCK_167_MHZ_PNV:
		return 167000;
	}
}

>>>>>>> refs/remotes/origin/master
static int i915gm_get_display_clock_speed(struct drm_device *dev)
{
	u16 gcfgc = 0;

	pci_read_config_word(dev->pdev, GCFGC, &gcfgc);

	if (gcfgc & GC_LOW_FREQUENCY_ENABLE)
		return 133000;
	else {
		switch (gcfgc & GC_DISPLAY_CLOCK_MASK) {
		case GC_DISPLAY_CLOCK_333_MHZ:
			return 333000;
		default:
		case GC_DISPLAY_CLOCK_190_200_MHZ:
			return 190000;
		}
	}
}

static int i865_get_display_clock_speed(struct drm_device *dev)
{
	return 266000;
}

static int i855_get_display_clock_speed(struct drm_device *dev)
{
	u16 hpllcc = 0;
	/* Assume that the hardware is in the high speed state.  This
	 * should be the default.
	 */
	switch (hpllcc & GC_CLOCK_CONTROL_MASK) {
	case GC_CLOCK_133_200:
	case GC_CLOCK_100_200:
		return 200000;
	case GC_CLOCK_166_250:
		return 250000;
	case GC_CLOCK_100_133:
		return 133000;
	}

<<<<<<< HEAD
	/* Shouldn't happen */
	return 0;
}

static int i830_get_display_clock_speed(struct drm_device *dev)
{
	return 133000;
}

struct fdi_m_n {
	u32        tu;
	u32        gmch_m;
	u32        gmch_n;
	u32        link_m;
	u32        link_n;
};

static void
fdi_reduce_ratio(u32 *num, u32 *den)
{
	while (*num > 0xffffff || *den > 0xffffff) {
		*num >>= 1;
		*den >>= 1;
	}
}

static void
ironlake_compute_m_n(int bits_per_pixel, int nlanes, int pixel_clock,
		     int link_clock, struct fdi_m_n *m_n)
{
	m_n->tu = 64; /* default size */

	/* BUG_ON(pixel_clock > INT_MAX / 36); */
	m_n->gmch_m = bits_per_pixel * pixel_clock;
	m_n->gmch_n = link_clock * nlanes * 8;
	fdi_reduce_ratio(&m_n->gmch_m, &m_n->gmch_n);

	m_n->link_m = pixel_clock;
	m_n->link_n = link_clock;
	fdi_reduce_ratio(&m_n->link_m, &m_n->link_n);
}


struct intel_watermark_params {
	unsigned long fifo_size;
	unsigned long max_wm;
	unsigned long default_wm;
	unsigned long guard_size;
	unsigned long cacheline_size;
};

/* Pineview has different values for various configs */
static const struct intel_watermark_params pineview_display_wm = {
	PINEVIEW_DISPLAY_FIFO,
	PINEVIEW_MAX_WM,
	PINEVIEW_DFT_WM,
	PINEVIEW_GUARD_WM,
	PINEVIEW_FIFO_LINE_SIZE
};
static const struct intel_watermark_params pineview_display_hplloff_wm = {
	PINEVIEW_DISPLAY_FIFO,
	PINEVIEW_MAX_WM,
	PINEVIEW_DFT_HPLLOFF_WM,
	PINEVIEW_GUARD_WM,
	PINEVIEW_FIFO_LINE_SIZE
};
static const struct intel_watermark_params pineview_cursor_wm = {
	PINEVIEW_CURSOR_FIFO,
	PINEVIEW_CURSOR_MAX_WM,
	PINEVIEW_CURSOR_DFT_WM,
	PINEVIEW_CURSOR_GUARD_WM,
	PINEVIEW_FIFO_LINE_SIZE,
};
static const struct intel_watermark_params pineview_cursor_hplloff_wm = {
	PINEVIEW_CURSOR_FIFO,
	PINEVIEW_CURSOR_MAX_WM,
	PINEVIEW_CURSOR_DFT_WM,
	PINEVIEW_CURSOR_GUARD_WM,
	PINEVIEW_FIFO_LINE_SIZE
};
static const struct intel_watermark_params g4x_wm_info = {
	G4X_FIFO_SIZE,
	G4X_MAX_WM,
	G4X_MAX_WM,
	2,
	G4X_FIFO_LINE_SIZE,
};
static const struct intel_watermark_params g4x_cursor_wm_info = {
	I965_CURSOR_FIFO,
	I965_CURSOR_MAX_WM,
	I965_CURSOR_DFT_WM,
	2,
	G4X_FIFO_LINE_SIZE,
};
static const struct intel_watermark_params i965_cursor_wm_info = {
	I965_CURSOR_FIFO,
	I965_CURSOR_MAX_WM,
	I965_CURSOR_DFT_WM,
	2,
	I915_FIFO_LINE_SIZE,
};
static const struct intel_watermark_params i945_wm_info = {
	I945_FIFO_SIZE,
	I915_MAX_WM,
	1,
	2,
	I915_FIFO_LINE_SIZE
};
static const struct intel_watermark_params i915_wm_info = {
	I915_FIFO_SIZE,
	I915_MAX_WM,
	1,
	2,
	I915_FIFO_LINE_SIZE
};
static const struct intel_watermark_params i855_wm_info = {
	I855GM_FIFO_SIZE,
	I915_MAX_WM,
	1,
	2,
	I830_FIFO_LINE_SIZE
};
static const struct intel_watermark_params i830_wm_info = {
	I830_FIFO_SIZE,
	I915_MAX_WM,
	1,
	2,
	I830_FIFO_LINE_SIZE
};

static const struct intel_watermark_params ironlake_display_wm_info = {
	ILK_DISPLAY_FIFO,
	ILK_DISPLAY_MAXWM,
	ILK_DISPLAY_DFTWM,
	2,
	ILK_FIFO_LINE_SIZE
};
static const struct intel_watermark_params ironlake_cursor_wm_info = {
	ILK_CURSOR_FIFO,
	ILK_CURSOR_MAXWM,
	ILK_CURSOR_DFTWM,
	2,
	ILK_FIFO_LINE_SIZE
};
static const struct intel_watermark_params ironlake_display_srwm_info = {
	ILK_DISPLAY_SR_FIFO,
	ILK_DISPLAY_MAX_SRWM,
	ILK_DISPLAY_DFT_SRWM,
	2,
	ILK_FIFO_LINE_SIZE
};
static const struct intel_watermark_params ironlake_cursor_srwm_info = {
	ILK_CURSOR_SR_FIFO,
	ILK_CURSOR_MAX_SRWM,
	ILK_CURSOR_DFT_SRWM,
	2,
	ILK_FIFO_LINE_SIZE
};

static const struct intel_watermark_params sandybridge_display_wm_info = {
	SNB_DISPLAY_FIFO,
	SNB_DISPLAY_MAXWM,
	SNB_DISPLAY_DFTWM,
	2,
	SNB_FIFO_LINE_SIZE
};
static const struct intel_watermark_params sandybridge_cursor_wm_info = {
	SNB_CURSOR_FIFO,
	SNB_CURSOR_MAXWM,
	SNB_CURSOR_DFTWM,
	2,
	SNB_FIFO_LINE_SIZE
};
static const struct intel_watermark_params sandybridge_display_srwm_info = {
	SNB_DISPLAY_SR_FIFO,
	SNB_DISPLAY_MAX_SRWM,
	SNB_DISPLAY_DFT_SRWM,
	2,
	SNB_FIFO_LINE_SIZE
};
static const struct intel_watermark_params sandybridge_cursor_srwm_info = {
	SNB_CURSOR_SR_FIFO,
	SNB_CURSOR_MAX_SRWM,
	SNB_CURSOR_DFT_SRWM,
	2,
	SNB_FIFO_LINE_SIZE
};


/**
 * intel_calculate_wm - calculate watermark level
 * @clock_in_khz: pixel clock
 * @wm: chip FIFO params
 * @pixel_size: display pixel size
 * @latency_ns: memory latency for the platform
 *
 * Calculate the watermark level (the level at which the display plane will
 * start fetching from memory again).  Each chip has a different display
 * FIFO size and allocation, so the caller needs to figure that out and pass
 * in the correct intel_watermark_params structure.
 *
 * As the pixel clock runs, the FIFO will be drained at a rate that depends
 * on the pixel size.  When it reaches the watermark level, it'll start
 * fetching FIFO line sized based chunks from memory until the FIFO fills
 * past the watermark point.  If the FIFO drains completely, a FIFO underrun
 * will occur, and a display engine hang could result.
 */
static unsigned long intel_calculate_wm(unsigned long clock_in_khz,
					const struct intel_watermark_params *wm,
					int fifo_size,
					int pixel_size,
					unsigned long latency_ns)
{
	long entries_required, wm_size;

	/*
	 * Note: we need to make sure we don't overflow for various clock &
	 * latency values.
	 * clocks go from a few thousand to several hundred thousand.
	 * latency is usually a few thousand
	 */
	entries_required = ((clock_in_khz / 1000) * pixel_size * latency_ns) /
		1000;
	entries_required = DIV_ROUND_UP(entries_required, wm->cacheline_size);

	DRM_DEBUG_KMS("FIFO entries required for mode: %ld\n", entries_required);

	wm_size = fifo_size - (entries_required + wm->guard_size);

	DRM_DEBUG_KMS("FIFO watermark level: %ld\n", wm_size);

	/* Don't promote wm_size to unsigned... */
	if (wm_size > (long)wm->max_wm)
		wm_size = wm->max_wm;
	if (wm_size <= 0)
		wm_size = wm->default_wm;
	return wm_size;
}

struct cxsr_latency {
	int is_desktop;
	int is_ddr3;
	unsigned long fsb_freq;
	unsigned long mem_freq;
	unsigned long display_sr;
	unsigned long display_hpll_disable;
	unsigned long cursor_sr;
	unsigned long cursor_hpll_disable;
};

static const struct cxsr_latency cxsr_latency_table[] = {
	{1, 0, 800, 400, 3382, 33382, 3983, 33983},    /* DDR2-400 SC */
	{1, 0, 800, 667, 3354, 33354, 3807, 33807},    /* DDR2-667 SC */
	{1, 0, 800, 800, 3347, 33347, 3763, 33763},    /* DDR2-800 SC */
	{1, 1, 800, 667, 6420, 36420, 6873, 36873},    /* DDR3-667 SC */
	{1, 1, 800, 800, 5902, 35902, 6318, 36318},    /* DDR3-800 SC */

	{1, 0, 667, 400, 3400, 33400, 4021, 34021},    /* DDR2-400 SC */
	{1, 0, 667, 667, 3372, 33372, 3845, 33845},    /* DDR2-667 SC */
	{1, 0, 667, 800, 3386, 33386, 3822, 33822},    /* DDR2-800 SC */
	{1, 1, 667, 667, 6438, 36438, 6911, 36911},    /* DDR3-667 SC */
	{1, 1, 667, 800, 5941, 35941, 6377, 36377},    /* DDR3-800 SC */

	{1, 0, 400, 400, 3472, 33472, 4173, 34173},    /* DDR2-400 SC */
	{1, 0, 400, 667, 3443, 33443, 3996, 33996},    /* DDR2-667 SC */
	{1, 0, 400, 800, 3430, 33430, 3946, 33946},    /* DDR2-800 SC */
	{1, 1, 400, 667, 6509, 36509, 7062, 37062},    /* DDR3-667 SC */
	{1, 1, 400, 800, 5985, 35985, 6501, 36501},    /* DDR3-800 SC */

	{0, 0, 800, 400, 3438, 33438, 4065, 34065},    /* DDR2-400 SC */
	{0, 0, 800, 667, 3410, 33410, 3889, 33889},    /* DDR2-667 SC */
	{0, 0, 800, 800, 3403, 33403, 3845, 33845},    /* DDR2-800 SC */
	{0, 1, 800, 667, 6476, 36476, 6955, 36955},    /* DDR3-667 SC */
	{0, 1, 800, 800, 5958, 35958, 6400, 36400},    /* DDR3-800 SC */

	{0, 0, 667, 400, 3456, 33456, 4103, 34106},    /* DDR2-400 SC */
	{0, 0, 667, 667, 3428, 33428, 3927, 33927},    /* DDR2-667 SC */
	{0, 0, 667, 800, 3443, 33443, 3905, 33905},    /* DDR2-800 SC */
	{0, 1, 667, 667, 6494, 36494, 6993, 36993},    /* DDR3-667 SC */
	{0, 1, 667, 800, 5998, 35998, 6460, 36460},    /* DDR3-800 SC */

	{0, 0, 400, 400, 3528, 33528, 4255, 34255},    /* DDR2-400 SC */
	{0, 0, 400, 667, 3500, 33500, 4079, 34079},    /* DDR2-667 SC */
	{0, 0, 400, 800, 3487, 33487, 4029, 34029},    /* DDR2-800 SC */
	{0, 1, 400, 667, 6566, 36566, 7145, 37145},    /* DDR3-667 SC */
	{0, 1, 400, 800, 6042, 36042, 6584, 36584},    /* DDR3-800 SC */
};

static const struct cxsr_latency *intel_get_cxsr_latency(int is_desktop,
							 int is_ddr3,
							 int fsb,
							 int mem)
{
	const struct cxsr_latency *latency;
	int i;

	if (fsb == 0 || mem == 0)
		return NULL;

	for (i = 0; i < ARRAY_SIZE(cxsr_latency_table); i++) {
		latency = &cxsr_latency_table[i];
		if (is_desktop == latency->is_desktop &&
		    is_ddr3 == latency->is_ddr3 &&
		    fsb == latency->fsb_freq && mem == latency->mem_freq)
			return latency;
	}

	DRM_DEBUG_KMS("Unknown FSB/MEM found, disable CxSR\n");

	return NULL;
}

static void pineview_disable_cxsr(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	/* deactivate cxsr */
	I915_WRITE(DSPFW3, I915_READ(DSPFW3) & ~PINEVIEW_SELF_REFRESH_EN);
}

/*
 * Latency for FIFO fetches is dependent on several factors:
 *   - memory configuration (speed, channels)
 *   - chipset
 *   - current MCH state
 * It can be fairly high in some situations, so here we assume a fairly
 * pessimal value.  It's a tradeoff between extra memory fetches (if we
 * set this value too high, the FIFO will fetch frequently to stay full)
 * and power consumption (set it too low to save power and we might see
 * FIFO underruns and display "flicker").
 *
 * A value of 5us seems to be a good balance; safe for very low end
 * platforms but not overly aggressive on lower latency configs.
 */
static const int latency_ns = 5000;

static int i9xx_get_fifo_size(struct drm_device *dev, int plane)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t dsparb = I915_READ(DSPARB);
	int size;

	size = dsparb & 0x7f;
	if (plane)
		size = ((dsparb >> DSPARB_CSTART_SHIFT) & 0x7f) - size;

	DRM_DEBUG_KMS("FIFO size - (0x%08x) %s: %d\n", dsparb,
		      plane ? "B" : "A", size);

	return size;
}

static int i85x_get_fifo_size(struct drm_device *dev, int plane)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t dsparb = I915_READ(DSPARB);
	int size;

	size = dsparb & 0x1ff;
	if (plane)
		size = ((dsparb >> DSPARB_BEND_SHIFT) & 0x1ff) - size;
	size >>= 1; /* Convert to cachelines */

	DRM_DEBUG_KMS("FIFO size - (0x%08x) %s: %d\n", dsparb,
		      plane ? "B" : "A", size);

	return size;
}

static int i845_get_fifo_size(struct drm_device *dev, int plane)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t dsparb = I915_READ(DSPARB);
	int size;

	size = dsparb & 0x7f;
	size >>= 2; /* Convert to cachelines */

	DRM_DEBUG_KMS("FIFO size - (0x%08x) %s: %d\n", dsparb,
		      plane ? "B" : "A",
		      size);

	return size;
}

static int i830_get_fifo_size(struct drm_device *dev, int plane)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t dsparb = I915_READ(DSPARB);
	int size;

	size = dsparb & 0x7f;
	size >>= 1; /* Convert to cachelines */

	DRM_DEBUG_KMS("FIFO size - (0x%08x) %s: %d\n", dsparb,
		      plane ? "B" : "A", size);

	return size;
}

static struct drm_crtc *single_enabled_crtc(struct drm_device *dev)
{
	struct drm_crtc *crtc, *enabled = NULL;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		if (crtc->enabled && crtc->fb) {
			if (enabled)
				return NULL;
			enabled = crtc;
		}
	}

	return enabled;
}

static void pineview_update_wm(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_crtc *crtc;
	const struct cxsr_latency *latency;
	u32 reg;
	unsigned long wm;

	latency = intel_get_cxsr_latency(IS_PINEVIEW_G(dev), dev_priv->is_ddr3,
					 dev_priv->fsb_freq, dev_priv->mem_freq);
	if (!latency) {
		DRM_DEBUG_KMS("Unknown FSB/MEM found, disable CxSR\n");
		pineview_disable_cxsr(dev);
		return;
	}

	crtc = single_enabled_crtc(dev);
	if (crtc) {
		int clock = crtc->mode.clock;
		int pixel_size = crtc->fb->bits_per_pixel / 8;

		/* Display SR */
		wm = intel_calculate_wm(clock, &pineview_display_wm,
					pineview_display_wm.fifo_size,
					pixel_size, latency->display_sr);
		reg = I915_READ(DSPFW1);
		reg &= ~DSPFW_SR_MASK;
		reg |= wm << DSPFW_SR_SHIFT;
		I915_WRITE(DSPFW1, reg);
		DRM_DEBUG_KMS("DSPFW1 register is %x\n", reg);

		/* cursor SR */
		wm = intel_calculate_wm(clock, &pineview_cursor_wm,
					pineview_display_wm.fifo_size,
					pixel_size, latency->cursor_sr);
		reg = I915_READ(DSPFW3);
		reg &= ~DSPFW_CURSOR_SR_MASK;
		reg |= (wm & 0x3f) << DSPFW_CURSOR_SR_SHIFT;
		I915_WRITE(DSPFW3, reg);

		/* Display HPLL off SR */
		wm = intel_calculate_wm(clock, &pineview_display_hplloff_wm,
					pineview_display_hplloff_wm.fifo_size,
					pixel_size, latency->display_hpll_disable);
		reg = I915_READ(DSPFW3);
		reg &= ~DSPFW_HPLL_SR_MASK;
		reg |= wm & DSPFW_HPLL_SR_MASK;
		I915_WRITE(DSPFW3, reg);

		/* cursor HPLL off SR */
		wm = intel_calculate_wm(clock, &pineview_cursor_hplloff_wm,
					pineview_display_hplloff_wm.fifo_size,
					pixel_size, latency->cursor_hpll_disable);
		reg = I915_READ(DSPFW3);
		reg &= ~DSPFW_HPLL_CURSOR_MASK;
		reg |= (wm & 0x3f) << DSPFW_HPLL_CURSOR_SHIFT;
		I915_WRITE(DSPFW3, reg);
		DRM_DEBUG_KMS("DSPFW3 register is %x\n", reg);

		/* activate cxsr */
		I915_WRITE(DSPFW3,
			   I915_READ(DSPFW3) | PINEVIEW_SELF_REFRESH_EN);
		DRM_DEBUG_KMS("Self-refresh is enabled\n");
	} else {
		pineview_disable_cxsr(dev);
		DRM_DEBUG_KMS("Self-refresh is disabled\n");
	}
}

static bool g4x_compute_wm0(struct drm_device *dev,
			    int plane,
			    const struct intel_watermark_params *display,
			    int display_latency_ns,
			    const struct intel_watermark_params *cursor,
			    int cursor_latency_ns,
			    int *plane_wm,
			    int *cursor_wm)
{
	struct drm_crtc *crtc;
	int htotal, hdisplay, clock, pixel_size;
	int line_time_us, line_count;
	int entries, tlb_miss;

	crtc = intel_get_crtc_for_plane(dev, plane);
	if (crtc->fb == NULL || !crtc->enabled) {
		*cursor_wm = cursor->guard_size;
		*plane_wm = display->guard_size;
		return false;
	}

	htotal = crtc->mode.htotal;
	hdisplay = crtc->mode.hdisplay;
	clock = crtc->mode.clock;
	pixel_size = crtc->fb->bits_per_pixel / 8;

	/* Use the small buffer method to calculate plane watermark */
	entries = ((clock * pixel_size / 1000) * display_latency_ns) / 1000;
	tlb_miss = display->fifo_size*display->cacheline_size - hdisplay * 8;
	if (tlb_miss > 0)
		entries += tlb_miss;
	entries = DIV_ROUND_UP(entries, display->cacheline_size);
	*plane_wm = entries + display->guard_size;
	if (*plane_wm > (int)display->max_wm)
		*plane_wm = display->max_wm;

	/* Use the large buffer method to calculate cursor watermark */
	line_time_us = ((htotal * 1000) / clock);
	line_count = (cursor_latency_ns / line_time_us + 1000) / 1000;
	entries = line_count * 64 * pixel_size;
	tlb_miss = cursor->fifo_size*cursor->cacheline_size - hdisplay * 8;
	if (tlb_miss > 0)
		entries += tlb_miss;
	entries = DIV_ROUND_UP(entries, cursor->cacheline_size);
	*cursor_wm = entries + cursor->guard_size;
	if (*cursor_wm > (int)cursor->max_wm)
		*cursor_wm = (int)cursor->max_wm;
=======
	/* Shouldn't happen */
	return 0;
}

static int i830_get_display_clock_speed(struct drm_device *dev)
{
	return 133000;
}

static void
intel_reduce_m_n_ratio(uint32_t *num, uint32_t *den)
{
	while (*num > DATA_LINK_M_N_MASK ||
	       *den > DATA_LINK_M_N_MASK) {
		*num >>= 1;
		*den >>= 1;
	}
}

static void compute_m_n(unsigned int m, unsigned int n,
			uint32_t *ret_m, uint32_t *ret_n)
{
	*ret_n = min_t(unsigned int, roundup_pow_of_two(n), DATA_LINK_N_MAX);
	*ret_m = div_u64((uint64_t) m * *ret_n, n);
	intel_reduce_m_n_ratio(ret_m, ret_n);
}

void
intel_link_compute_m_n(int bits_per_pixel, int nlanes,
		       int pixel_clock, int link_clock,
		       struct intel_link_m_n *m_n)
{
	m_n->tu = 64;

	compute_m_n(bits_per_pixel * pixel_clock,
		    link_clock * nlanes * 8,
		    &m_n->gmch_m, &m_n->gmch_n);

	compute_m_n(pixel_clock, link_clock,
		    &m_n->link_m, &m_n->link_n);
}

static inline bool intel_panel_use_ssc(struct drm_i915_private *dev_priv)
{
	if (i915_panel_use_ssc >= 0)
		return i915_panel_use_ssc != 0;
	return dev_priv->vbt.lvds_use_ssc
		&& !(dev_priv->quirks & QUIRK_LVDS_SSC_DISABLE);
}

static int i9xx_get_refclk(struct drm_crtc *crtc, int num_connectors)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int refclk;

	if (IS_VALLEYVIEW(dev)) {
		refclk = 100000;
	} else if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS) &&
	    intel_panel_use_ssc(dev_priv) && num_connectors < 2) {
		refclk = dev_priv->vbt.lvds_ssc_freq * 1000;
		DRM_DEBUG_KMS("using SSC reference clock of %d MHz\n",
			      refclk / 1000);
	} else if (!IS_GEN2(dev)) {
		refclk = 96000;
	} else {
		refclk = 48000;
	}

	return refclk;
}

static uint32_t pnv_dpll_compute_fp(struct dpll *dpll)
{
	return (1 << dpll->n) << 16 | dpll->m2;
}

static uint32_t i9xx_dpll_compute_fp(struct dpll *dpll)
{
	return dpll->n << 16 | dpll->m1 << 8 | dpll->m2;
}

static void i9xx_update_pll_dividers(struct intel_crtc *crtc,
				     intel_clock_t *reduced_clock)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipe = crtc->pipe;
	u32 fp, fp2 = 0;

	if (IS_PINEVIEW(dev)) {
		fp = pnv_dpll_compute_fp(&crtc->config.dpll);
		if (reduced_clock)
			fp2 = pnv_dpll_compute_fp(reduced_clock);
	} else {
		fp = i9xx_dpll_compute_fp(&crtc->config.dpll);
		if (reduced_clock)
			fp2 = i9xx_dpll_compute_fp(reduced_clock);
	}

	I915_WRITE(FP0(pipe), fp);
	crtc->config.dpll_hw_state.fp0 = fp;

	crtc->lowfreq_avail = false;
	if (intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_LVDS) &&
	    reduced_clock && i915_powersave) {
		I915_WRITE(FP1(pipe), fp2);
		crtc->config.dpll_hw_state.fp1 = fp2;
		crtc->lowfreq_avail = true;
	} else {
		I915_WRITE(FP1(pipe), fp);
		crtc->config.dpll_hw_state.fp1 = fp;
	}
}

static void vlv_pllb_recal_opamp(struct drm_i915_private *dev_priv, enum pipe
		pipe)
{
	u32 reg_val;

	/*
	 * PLLB opamp always calibrates to max value of 0x3f, force enable it
	 * and set it to a reasonable value instead.
	 */
	reg_val = vlv_dpio_read(dev_priv, pipe, DPIO_IREF(1));
	reg_val &= 0xffffff00;
	reg_val |= 0x00000030;
	vlv_dpio_write(dev_priv, pipe, DPIO_IREF(1), reg_val);

	reg_val = vlv_dpio_read(dev_priv, pipe, DPIO_CALIBRATION);
	reg_val &= 0x8cffffff;
	reg_val = 0x8c000000;
	vlv_dpio_write(dev_priv, pipe, DPIO_CALIBRATION, reg_val);

	reg_val = vlv_dpio_read(dev_priv, pipe, DPIO_IREF(1));
	reg_val &= 0xffffff00;
	vlv_dpio_write(dev_priv, pipe, DPIO_IREF(1), reg_val);

	reg_val = vlv_dpio_read(dev_priv, pipe, DPIO_CALIBRATION);
	reg_val &= 0x00ffffff;
	reg_val |= 0xb0000000;
	vlv_dpio_write(dev_priv, pipe, DPIO_CALIBRATION, reg_val);
}

static void intel_pch_transcoder_set_m_n(struct intel_crtc *crtc,
					 struct intel_link_m_n *m_n)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipe = crtc->pipe;

	I915_WRITE(PCH_TRANS_DATA_M1(pipe), TU_SIZE(m_n->tu) | m_n->gmch_m);
	I915_WRITE(PCH_TRANS_DATA_N1(pipe), m_n->gmch_n);
	I915_WRITE(PCH_TRANS_LINK_M1(pipe), m_n->link_m);
	I915_WRITE(PCH_TRANS_LINK_N1(pipe), m_n->link_n);
}

static void intel_cpu_transcoder_set_m_n(struct intel_crtc *crtc,
					 struct intel_link_m_n *m_n)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipe = crtc->pipe;
	enum transcoder transcoder = crtc->config.cpu_transcoder;

	if (INTEL_INFO(dev)->gen >= 5) {
		I915_WRITE(PIPE_DATA_M1(transcoder), TU_SIZE(m_n->tu) | m_n->gmch_m);
		I915_WRITE(PIPE_DATA_N1(transcoder), m_n->gmch_n);
		I915_WRITE(PIPE_LINK_M1(transcoder), m_n->link_m);
		I915_WRITE(PIPE_LINK_N1(transcoder), m_n->link_n);
	} else {
		I915_WRITE(PIPE_DATA_M_G4X(pipe), TU_SIZE(m_n->tu) | m_n->gmch_m);
		I915_WRITE(PIPE_DATA_N_G4X(pipe), m_n->gmch_n);
		I915_WRITE(PIPE_LINK_M_G4X(pipe), m_n->link_m);
		I915_WRITE(PIPE_LINK_N_G4X(pipe), m_n->link_n);
	}
}

static void intel_dp_set_m_n(struct intel_crtc *crtc)
{
	if (crtc->config.has_pch_encoder)
		intel_pch_transcoder_set_m_n(crtc, &crtc->config.dp_m_n);
	else
		intel_cpu_transcoder_set_m_n(crtc, &crtc->config.dp_m_n);
}

static void vlv_update_pll(struct intel_crtc *crtc)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipe = crtc->pipe;
	u32 dpll, mdiv;
	u32 bestn, bestm1, bestm2, bestp1, bestp2;
	u32 coreclk, reg_val, dpll_md;

	mutex_lock(&dev_priv->dpio_lock);

	bestn = crtc->config.dpll.n;
	bestm1 = crtc->config.dpll.m1;
	bestm2 = crtc->config.dpll.m2;
	bestp1 = crtc->config.dpll.p1;
	bestp2 = crtc->config.dpll.p2;

	/* See eDP HDMI DPIO driver vbios notes doc */

	/* PLL B needs special handling */
	if (pipe)
		vlv_pllb_recal_opamp(dev_priv, pipe);

	/* Set up Tx target for periodic Rcomp update */
	vlv_dpio_write(dev_priv, pipe, DPIO_IREF_BCAST, 0x0100000f);

	/* Disable target IRef on PLL */
	reg_val = vlv_dpio_read(dev_priv, pipe, DPIO_IREF_CTL(pipe));
	reg_val &= 0x00ffffff;
	vlv_dpio_write(dev_priv, pipe, DPIO_IREF_CTL(pipe), reg_val);

	/* Disable fast lock */
	vlv_dpio_write(dev_priv, pipe, DPIO_FASTCLK_DISABLE, 0x610);

	/* Set idtafcrecal before PLL is enabled */
	mdiv = ((bestm1 << DPIO_M1DIV_SHIFT) | (bestm2 & DPIO_M2DIV_MASK));
	mdiv |= ((bestp1 << DPIO_P1_SHIFT) | (bestp2 << DPIO_P2_SHIFT));
	mdiv |= ((bestn << DPIO_N_SHIFT));
	mdiv |= (1 << DPIO_K_SHIFT);

	/*
	 * Post divider depends on pixel clock rate, DAC vs digital (and LVDS,
	 * but we don't support that).
	 * Note: don't use the DAC post divider as it seems unstable.
	 */
	mdiv |= (DPIO_POST_DIV_HDMIDP << DPIO_POST_DIV_SHIFT);
	vlv_dpio_write(dev_priv, pipe, DPIO_DIV(pipe), mdiv);

	mdiv |= DPIO_ENABLE_CALIBRATION;
	vlv_dpio_write(dev_priv, pipe, DPIO_DIV(pipe), mdiv);

	/* Set HBR and RBR LPF coefficients */
	if (crtc->config.port_clock == 162000 ||
	    intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_ANALOG) ||
	    intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_HDMI))
		vlv_dpio_write(dev_priv, pipe, DPIO_LPF_COEFF(pipe),
				 0x009f0003);
	else
		vlv_dpio_write(dev_priv, pipe, DPIO_LPF_COEFF(pipe),
				 0x00d0000f);

	if (intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_EDP) ||
	    intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_DISPLAYPORT)) {
		/* Use SSC source */
		if (!pipe)
			vlv_dpio_write(dev_priv, pipe, DPIO_REFSFR(pipe),
					 0x0df40000);
		else
			vlv_dpio_write(dev_priv, pipe, DPIO_REFSFR(pipe),
					 0x0df70000);
	} else { /* HDMI or VGA */
		/* Use bend source */
		if (!pipe)
			vlv_dpio_write(dev_priv, pipe, DPIO_REFSFR(pipe),
					 0x0df70000);
		else
			vlv_dpio_write(dev_priv, pipe, DPIO_REFSFR(pipe),
					 0x0df40000);
	}

	coreclk = vlv_dpio_read(dev_priv, pipe, DPIO_CORE_CLK(pipe));
	coreclk = (coreclk & 0x0000ff00) | 0x01c00000;
	if (intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_DISPLAYPORT) ||
	    intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_EDP))
		coreclk |= 0x01000000;
	vlv_dpio_write(dev_priv, pipe, DPIO_CORE_CLK(pipe), coreclk);

	vlv_dpio_write(dev_priv, pipe, DPIO_PLL_CML(pipe), 0x87871000);

	/* Enable DPIO clock input */
	dpll = DPLL_EXT_BUFFER_ENABLE_VLV | DPLL_REFA_CLK_ENABLE_VLV |
		DPLL_VGA_MODE_DIS | DPLL_INTEGRATED_CLOCK_VLV;
	/* We should never disable this, set it here for state tracking */
	if (pipe == PIPE_B)
		dpll |= DPLL_INTEGRATED_CRI_CLK_VLV;
	dpll |= DPLL_VCO_ENABLE;
	crtc->config.dpll_hw_state.dpll = dpll;

	dpll_md = (crtc->config.pixel_multiplier - 1)
		<< DPLL_MD_UDI_MULTIPLIER_SHIFT;
	crtc->config.dpll_hw_state.dpll_md = dpll_md;

	if (crtc->config.has_dp_encoder)
		intel_dp_set_m_n(crtc);

	mutex_unlock(&dev_priv->dpio_lock);
}

static void i9xx_update_pll(struct intel_crtc *crtc,
			    intel_clock_t *reduced_clock,
			    int num_connectors)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 dpll;
	bool is_sdvo;
	struct dpll *clock = &crtc->config.dpll;

	i9xx_update_pll_dividers(crtc, reduced_clock);

	is_sdvo = intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_SDVO) ||
		intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_HDMI);

	dpll = DPLL_VGA_MODE_DIS;

	if (intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_LVDS))
		dpll |= DPLLB_MODE_LVDS;
	else
		dpll |= DPLLB_MODE_DAC_SERIAL;

	if (IS_I945G(dev) || IS_I945GM(dev) || IS_G33(dev)) {
		dpll |= (crtc->config.pixel_multiplier - 1)
			<< SDVO_MULTIPLIER_SHIFT_HIRES;
	}

	if (is_sdvo)
		dpll |= DPLL_SDVO_HIGH_SPEED;

	if (intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_DISPLAYPORT))
		dpll |= DPLL_SDVO_HIGH_SPEED;

	/* compute bitmask from p1 value */
	if (IS_PINEVIEW(dev))
		dpll |= (1 << (clock->p1 - 1)) << DPLL_FPA01_P1_POST_DIV_SHIFT_PINEVIEW;
	else {
		dpll |= (1 << (clock->p1 - 1)) << DPLL_FPA01_P1_POST_DIV_SHIFT;
		if (IS_G4X(dev) && reduced_clock)
			dpll |= (1 << (reduced_clock->p1 - 1)) << DPLL_FPA1_P1_POST_DIV_SHIFT;
	}
	switch (clock->p2) {
	case 5:
		dpll |= DPLL_DAC_SERIAL_P2_CLOCK_DIV_5;
		break;
	case 7:
		dpll |= DPLLB_LVDS_P2_CLOCK_DIV_7;
		break;
	case 10:
		dpll |= DPLL_DAC_SERIAL_P2_CLOCK_DIV_10;
		break;
	case 14:
		dpll |= DPLLB_LVDS_P2_CLOCK_DIV_14;
		break;
	}
	if (INTEL_INFO(dev)->gen >= 4)
		dpll |= (6 << PLL_LOAD_PULSE_PHASE_SHIFT);

	if (crtc->config.sdvo_tv_clock)
		dpll |= PLL_REF_INPUT_TVCLKINBC;
	else if (intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_LVDS) &&
		 intel_panel_use_ssc(dev_priv) && num_connectors < 2)
		dpll |= PLLB_REF_INPUT_SPREADSPECTRUMIN;
	else
		dpll |= PLL_REF_INPUT_DREFCLK;

	dpll |= DPLL_VCO_ENABLE;
	crtc->config.dpll_hw_state.dpll = dpll;

	if (INTEL_INFO(dev)->gen >= 4) {
		u32 dpll_md = (crtc->config.pixel_multiplier - 1)
			<< DPLL_MD_UDI_MULTIPLIER_SHIFT;
		crtc->config.dpll_hw_state.dpll_md = dpll_md;
	}

	if (crtc->config.has_dp_encoder)
		intel_dp_set_m_n(crtc);
}

static void i8xx_update_pll(struct intel_crtc *crtc,
			    intel_clock_t *reduced_clock,
			    int num_connectors)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 dpll;
	struct dpll *clock = &crtc->config.dpll;

	i9xx_update_pll_dividers(crtc, reduced_clock);

	dpll = DPLL_VGA_MODE_DIS;

	if (intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_LVDS)) {
		dpll |= (1 << (clock->p1 - 1)) << DPLL_FPA01_P1_POST_DIV_SHIFT;
	} else {
		if (clock->p1 == 2)
			dpll |= PLL_P1_DIVIDE_BY_TWO;
		else
			dpll |= (clock->p1 - 2) << DPLL_FPA01_P1_POST_DIV_SHIFT;
		if (clock->p2 == 4)
			dpll |= PLL_P2_DIVIDE_BY_4;
	}

	if (intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_DVO))
		dpll |= DPLL_DVO_2X_MODE;

	if (intel_pipe_has_type(&crtc->base, INTEL_OUTPUT_LVDS) &&
		 intel_panel_use_ssc(dev_priv) && num_connectors < 2)
		dpll |= PLLB_REF_INPUT_SPREADSPECTRUMIN;
	else
		dpll |= PLL_REF_INPUT_DREFCLK;

	dpll |= DPLL_VCO_ENABLE;
	crtc->config.dpll_hw_state.dpll = dpll;
}

static void intel_set_pipe_timings(struct intel_crtc *intel_crtc)
{
	struct drm_device *dev = intel_crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	enum pipe pipe = intel_crtc->pipe;
	enum transcoder cpu_transcoder = intel_crtc->config.cpu_transcoder;
	struct drm_display_mode *adjusted_mode =
		&intel_crtc->config.adjusted_mode;
	uint32_t vsyncshift, crtc_vtotal, crtc_vblank_end;

	/* We need to be careful not to changed the adjusted mode, for otherwise
	 * the hw state checker will get angry at the mismatch. */
	crtc_vtotal = adjusted_mode->crtc_vtotal;
	crtc_vblank_end = adjusted_mode->crtc_vblank_end;

	if (!IS_GEN2(dev) && adjusted_mode->flags & DRM_MODE_FLAG_INTERLACE) {
		/* the chip adds 2 halflines automatically */
		crtc_vtotal -= 1;
		crtc_vblank_end -= 1;
		vsyncshift = adjusted_mode->crtc_hsync_start
			     - adjusted_mode->crtc_htotal / 2;
	} else {
		vsyncshift = 0;
	}

	if (INTEL_INFO(dev)->gen > 3)
		I915_WRITE(VSYNCSHIFT(cpu_transcoder), vsyncshift);

	I915_WRITE(HTOTAL(cpu_transcoder),
		   (adjusted_mode->crtc_hdisplay - 1) |
		   ((adjusted_mode->crtc_htotal - 1) << 16));
	I915_WRITE(HBLANK(cpu_transcoder),
		   (adjusted_mode->crtc_hblank_start - 1) |
		   ((adjusted_mode->crtc_hblank_end - 1) << 16));
	I915_WRITE(HSYNC(cpu_transcoder),
		   (adjusted_mode->crtc_hsync_start - 1) |
		   ((adjusted_mode->crtc_hsync_end - 1) << 16));

	I915_WRITE(VTOTAL(cpu_transcoder),
		   (adjusted_mode->crtc_vdisplay - 1) |
		   ((crtc_vtotal - 1) << 16));
	I915_WRITE(VBLANK(cpu_transcoder),
		   (adjusted_mode->crtc_vblank_start - 1) |
		   ((crtc_vblank_end - 1) << 16));
	I915_WRITE(VSYNC(cpu_transcoder),
		   (adjusted_mode->crtc_vsync_start - 1) |
		   ((adjusted_mode->crtc_vsync_end - 1) << 16));

	/* Workaround: when the EDP input selection is B, the VTOTAL_B must be
	 * programmed with the VTOTAL_EDP value. Same for VTOTAL_C. This is
	 * documented on the DDI_FUNC_CTL register description, EDP Input Select
	 * bits. */
	if (IS_HASWELL(dev) && cpu_transcoder == TRANSCODER_EDP &&
	    (pipe == PIPE_B || pipe == PIPE_C))
		I915_WRITE(VTOTAL(pipe), I915_READ(VTOTAL(cpu_transcoder)));

	/* pipesrc controls the size that is scaled from, which should
	 * always be the user's requested size.
	 */
	I915_WRITE(PIPESRC(pipe),
		   ((intel_crtc->config.pipe_src_w - 1) << 16) |
		   (intel_crtc->config.pipe_src_h - 1));
}

static void intel_get_pipe_timings(struct intel_crtc *crtc,
				   struct intel_crtc_config *pipe_config)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	enum transcoder cpu_transcoder = pipe_config->cpu_transcoder;
	uint32_t tmp;

	tmp = I915_READ(HTOTAL(cpu_transcoder));
	pipe_config->adjusted_mode.crtc_hdisplay = (tmp & 0xffff) + 1;
	pipe_config->adjusted_mode.crtc_htotal = ((tmp >> 16) & 0xffff) + 1;
	tmp = I915_READ(HBLANK(cpu_transcoder));
	pipe_config->adjusted_mode.crtc_hblank_start = (tmp & 0xffff) + 1;
	pipe_config->adjusted_mode.crtc_hblank_end = ((tmp >> 16) & 0xffff) + 1;
	tmp = I915_READ(HSYNC(cpu_transcoder));
	pipe_config->adjusted_mode.crtc_hsync_start = (tmp & 0xffff) + 1;
	pipe_config->adjusted_mode.crtc_hsync_end = ((tmp >> 16) & 0xffff) + 1;

	tmp = I915_READ(VTOTAL(cpu_transcoder));
	pipe_config->adjusted_mode.crtc_vdisplay = (tmp & 0xffff) + 1;
	pipe_config->adjusted_mode.crtc_vtotal = ((tmp >> 16) & 0xffff) + 1;
	tmp = I915_READ(VBLANK(cpu_transcoder));
	pipe_config->adjusted_mode.crtc_vblank_start = (tmp & 0xffff) + 1;
	pipe_config->adjusted_mode.crtc_vblank_end = ((tmp >> 16) & 0xffff) + 1;
	tmp = I915_READ(VSYNC(cpu_transcoder));
	pipe_config->adjusted_mode.crtc_vsync_start = (tmp & 0xffff) + 1;
	pipe_config->adjusted_mode.crtc_vsync_end = ((tmp >> 16) & 0xffff) + 1;

	if (I915_READ(PIPECONF(cpu_transcoder)) & PIPECONF_INTERLACE_MASK) {
		pipe_config->adjusted_mode.flags |= DRM_MODE_FLAG_INTERLACE;
		pipe_config->adjusted_mode.crtc_vtotal += 1;
		pipe_config->adjusted_mode.crtc_vblank_end += 1;
	}

	tmp = I915_READ(PIPESRC(crtc->pipe));
	pipe_config->pipe_src_h = (tmp & 0xffff) + 1;
	pipe_config->pipe_src_w = ((tmp >> 16) & 0xffff) + 1;

	pipe_config->requested_mode.vdisplay = pipe_config->pipe_src_h;
	pipe_config->requested_mode.hdisplay = pipe_config->pipe_src_w;
}

static void intel_crtc_mode_from_pipe_config(struct intel_crtc *intel_crtc,
					     struct intel_crtc_config *pipe_config)
{
	struct drm_crtc *crtc = &intel_crtc->base;

	crtc->mode.hdisplay = pipe_config->adjusted_mode.crtc_hdisplay;
	crtc->mode.htotal = pipe_config->adjusted_mode.crtc_htotal;
	crtc->mode.hsync_start = pipe_config->adjusted_mode.crtc_hsync_start;
	crtc->mode.hsync_end = pipe_config->adjusted_mode.crtc_hsync_end;

	crtc->mode.vdisplay = pipe_config->adjusted_mode.crtc_vdisplay;
	crtc->mode.vtotal = pipe_config->adjusted_mode.crtc_vtotal;
	crtc->mode.vsync_start = pipe_config->adjusted_mode.crtc_vsync_start;
	crtc->mode.vsync_end = pipe_config->adjusted_mode.crtc_vsync_end;

	crtc->mode.flags = pipe_config->adjusted_mode.flags;

	crtc->mode.clock = pipe_config->adjusted_mode.crtc_clock;
	crtc->mode.flags |= pipe_config->adjusted_mode.flags;
}

static void i9xx_set_pipeconf(struct intel_crtc *intel_crtc)
{
	struct drm_device *dev = intel_crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t pipeconf;

	pipeconf = 0;

	if (dev_priv->quirks & QUIRK_PIPEA_FORCE &&
	    I915_READ(PIPECONF(intel_crtc->pipe)) & PIPECONF_ENABLE)
		pipeconf |= PIPECONF_ENABLE;

	if (intel_crtc->config.double_wide)
		pipeconf |= PIPECONF_DOUBLE_WIDE;

	/* only g4x and later have fancy bpc/dither controls */
	if (IS_G4X(dev) || IS_VALLEYVIEW(dev)) {
		/* Bspec claims that we can't use dithering for 30bpp pipes. */
		if (intel_crtc->config.dither && intel_crtc->config.pipe_bpp != 30)
			pipeconf |= PIPECONF_DITHER_EN |
				    PIPECONF_DITHER_TYPE_SP;

		switch (intel_crtc->config.pipe_bpp) {
		case 18:
			pipeconf |= PIPECONF_6BPC;
			break;
		case 24:
			pipeconf |= PIPECONF_8BPC;
			break;
		case 30:
			pipeconf |= PIPECONF_10BPC;
			break;
		default:
			/* Case prevented by intel_choose_pipe_bpp_dither. */
			BUG();
		}
	}

	if (HAS_PIPE_CXSR(dev)) {
		if (intel_crtc->lowfreq_avail) {
			DRM_DEBUG_KMS("enabling CxSR downclocking\n");
			pipeconf |= PIPECONF_CXSR_DOWNCLOCK;
		} else {
			DRM_DEBUG_KMS("disabling CxSR downclocking\n");
		}
	}

	if (!IS_GEN2(dev) &&
	    intel_crtc->config.adjusted_mode.flags & DRM_MODE_FLAG_INTERLACE)
		pipeconf |= PIPECONF_INTERLACE_W_FIELD_INDICATION;
	else
		pipeconf |= PIPECONF_PROGRESSIVE;

	if (IS_VALLEYVIEW(dev) && intel_crtc->config.limited_color_range)
		pipeconf |= PIPECONF_COLOR_RANGE_SELECT;

	I915_WRITE(PIPECONF(intel_crtc->pipe), pipeconf);
	POSTING_READ(PIPECONF(intel_crtc->pipe));
}

static int i9xx_crtc_mode_set(struct drm_crtc *crtc,
			      int x, int y,
			      struct drm_framebuffer *fb)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;
	int refclk, num_connectors = 0;
	intel_clock_t clock, reduced_clock;
	u32 dspcntr;
	bool ok, has_reduced_clock = false;
	bool is_lvds = false, is_dsi = false;
	struct intel_encoder *encoder;
	const intel_limit_t *limit;
	int ret;

	for_each_encoder_on_crtc(dev, crtc, encoder) {
		switch (encoder->type) {
		case INTEL_OUTPUT_LVDS:
			is_lvds = true;
			break;
		case INTEL_OUTPUT_DSI:
			is_dsi = true;
			break;
		}

		num_connectors++;
	}

	if (is_dsi)
		goto skip_dpll;

	if (!intel_crtc->config.clock_set) {
		refclk = i9xx_get_refclk(crtc, num_connectors);

		/*
		 * Returns a set of divisors for the desired target clock with
		 * the given refclk, or FALSE.  The returned values represent
		 * the clock equation: reflck * (5 * (m1 + 2) + (m2 + 2)) / (n +
		 * 2) / p1 / p2.
		 */
		limit = intel_limit(crtc, refclk);
		ok = dev_priv->display.find_dpll(limit, crtc,
						 intel_crtc->config.port_clock,
						 refclk, NULL, &clock);
		if (!ok) {
			DRM_ERROR("Couldn't find PLL settings for mode!\n");
			return -EINVAL;
		}

		if (is_lvds && dev_priv->lvds_downclock_avail) {
			/*
			 * Ensure we match the reduced clock's P to the target
			 * clock.  If the clocks don't match, we can't switch
			 * the display clock by using the FP0/FP1. In such case
			 * we will disable the LVDS downclock feature.
			 */
			has_reduced_clock =
				dev_priv->display.find_dpll(limit, crtc,
							    dev_priv->lvds_downclock,
							    refclk, &clock,
							    &reduced_clock);
		}
		/* Compat-code for transition, will disappear. */
		intel_crtc->config.dpll.n = clock.n;
		intel_crtc->config.dpll.m1 = clock.m1;
		intel_crtc->config.dpll.m2 = clock.m2;
		intel_crtc->config.dpll.p1 = clock.p1;
		intel_crtc->config.dpll.p2 = clock.p2;
	}

	if (IS_GEN2(dev)) {
		i8xx_update_pll(intel_crtc,
				has_reduced_clock ? &reduced_clock : NULL,
				num_connectors);
	} else if (IS_VALLEYVIEW(dev)) {
		vlv_update_pll(intel_crtc);
	} else {
		i9xx_update_pll(intel_crtc,
				has_reduced_clock ? &reduced_clock : NULL,
                                num_connectors);
	}

skip_dpll:
	/* Set up the display plane register */
	dspcntr = DISPPLANE_GAMMA_ENABLE;

	if (!IS_VALLEYVIEW(dev)) {
		if (pipe == 0)
			dspcntr &= ~DISPPLANE_SEL_PIPE_MASK;
		else
			dspcntr |= DISPPLANE_SEL_PIPE_B;
	}

	intel_set_pipe_timings(intel_crtc);

	/* pipesrc and dspsize control the size that is scaled from,
	 * which should always be the user's requested size.
	 */
	I915_WRITE(DSPSIZE(plane),
		   ((intel_crtc->config.pipe_src_h - 1) << 16) |
		   (intel_crtc->config.pipe_src_w - 1));
	I915_WRITE(DSPPOS(plane), 0);

	i9xx_set_pipeconf(intel_crtc);

	I915_WRITE(DSPCNTR(plane), dspcntr);
	POSTING_READ(DSPCNTR(plane));

	ret = intel_pipe_set_base(crtc, x, y, fb);

	return ret;
}

static void i9xx_get_pfit_config(struct intel_crtc *crtc,
				 struct intel_crtc_config *pipe_config)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t tmp;

	tmp = I915_READ(PFIT_CONTROL);
	if (!(tmp & PFIT_ENABLE))
		return;

	/* Check whether the pfit is attached to our pipe. */
	if (INTEL_INFO(dev)->gen < 4) {
		if (crtc->pipe != PIPE_B)
			return;
	} else {
		if ((tmp & PFIT_PIPE_MASK) != (crtc->pipe << PFIT_PIPE_SHIFT))
			return;
	}

	pipe_config->gmch_pfit.control = tmp;
	pipe_config->gmch_pfit.pgm_ratios = I915_READ(PFIT_PGM_RATIOS);
	if (INTEL_INFO(dev)->gen < 5)
		pipe_config->gmch_pfit.lvds_border_bits =
			I915_READ(LVDS) & LVDS_BORDER_ENABLE;
}

static void vlv_crtc_clock_get(struct intel_crtc *crtc,
			       struct intel_crtc_config *pipe_config)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipe = pipe_config->cpu_transcoder;
	intel_clock_t clock;
	u32 mdiv;
	int refclk = 100000;

	mutex_lock(&dev_priv->dpio_lock);
	mdiv = vlv_dpio_read(dev_priv, pipe, DPIO_DIV(pipe));
	mutex_unlock(&dev_priv->dpio_lock);

	clock.m1 = (mdiv >> DPIO_M1DIV_SHIFT) & 7;
	clock.m2 = mdiv & DPIO_M2DIV_MASK;
	clock.n = (mdiv >> DPIO_N_SHIFT) & 0xf;
	clock.p1 = (mdiv >> DPIO_P1_SHIFT) & 7;
	clock.p2 = (mdiv >> DPIO_P2_SHIFT) & 0x1f;

	vlv_clock(refclk, &clock);

	/* clock.dot is the fast clock */
	pipe_config->port_clock = clock.dot / 5;
}

static bool i9xx_get_pipe_config(struct intel_crtc *crtc,
				 struct intel_crtc_config *pipe_config)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t tmp;

	pipe_config->cpu_transcoder = (enum transcoder) crtc->pipe;
	pipe_config->shared_dpll = DPLL_ID_PRIVATE;

	tmp = I915_READ(PIPECONF(crtc->pipe));
	if (!(tmp & PIPECONF_ENABLE))
		return false;

	if (IS_G4X(dev) || IS_VALLEYVIEW(dev)) {
		switch (tmp & PIPECONF_BPC_MASK) {
		case PIPECONF_6BPC:
			pipe_config->pipe_bpp = 18;
			break;
		case PIPECONF_8BPC:
			pipe_config->pipe_bpp = 24;
			break;
		case PIPECONF_10BPC:
			pipe_config->pipe_bpp = 30;
			break;
		default:
			break;
		}
	}

	if (INTEL_INFO(dev)->gen < 4)
		pipe_config->double_wide = tmp & PIPECONF_DOUBLE_WIDE;

	intel_get_pipe_timings(crtc, pipe_config);

	i9xx_get_pfit_config(crtc, pipe_config);

	if (INTEL_INFO(dev)->gen >= 4) {
		tmp = I915_READ(DPLL_MD(crtc->pipe));
		pipe_config->pixel_multiplier =
			((tmp & DPLL_MD_UDI_MULTIPLIER_MASK)
			 >> DPLL_MD_UDI_MULTIPLIER_SHIFT) + 1;
		pipe_config->dpll_hw_state.dpll_md = tmp;
	} else if (IS_I945G(dev) || IS_I945GM(dev) || IS_G33(dev)) {
		tmp = I915_READ(DPLL(crtc->pipe));
		pipe_config->pixel_multiplier =
			((tmp & SDVO_MULTIPLIER_MASK)
			 >> SDVO_MULTIPLIER_SHIFT_HIRES) + 1;
	} else {
		/* Note that on i915G/GM the pixel multiplier is in the sdvo
		 * port and will be fixed up in the encoder->get_config
		 * function. */
		pipe_config->pixel_multiplier = 1;
	}
	pipe_config->dpll_hw_state.dpll = I915_READ(DPLL(crtc->pipe));
	if (!IS_VALLEYVIEW(dev)) {
		pipe_config->dpll_hw_state.fp0 = I915_READ(FP0(crtc->pipe));
		pipe_config->dpll_hw_state.fp1 = I915_READ(FP1(crtc->pipe));
	} else {
		/* Mask out read-only status bits. */
		pipe_config->dpll_hw_state.dpll &= ~(DPLL_LOCK_VLV |
						     DPLL_PORTC_READY_MASK |
						     DPLL_PORTB_READY_MASK);
	}

	if (IS_VALLEYVIEW(dev))
		vlv_crtc_clock_get(crtc, pipe_config);
	else
		i9xx_crtc_clock_get(crtc, pipe_config);
>>>>>>> refs/remotes/origin/master

	return true;
}

<<<<<<< HEAD
/*
 * Check the wm result.
 *
 * If any calculated watermark values is larger than the maximum value that
 * can be programmed into the associated watermark register, that watermark
 * must be disabled.
 */
static bool g4x_check_srwm(struct drm_device *dev,
			   int display_wm, int cursor_wm,
			   const struct intel_watermark_params *display,
			   const struct intel_watermark_params *cursor)
{
	DRM_DEBUG_KMS("SR watermark: display plane %d, cursor %d\n",
		      display_wm, cursor_wm);

	if (display_wm > display->max_wm) {
		DRM_DEBUG_KMS("display watermark is too large(%d/%ld), disabling\n",
			      display_wm, display->max_wm);
		return false;
	}

	if (cursor_wm > cursor->max_wm) {
		DRM_DEBUG_KMS("cursor watermark is too large(%d/%ld), disabling\n",
			      cursor_wm, cursor->max_wm);
		return false;
	}

	if (!(display_wm || cursor_wm)) {
		DRM_DEBUG_KMS("SR latency is 0, disabling\n");
		return false;
	}

	return true;
}

static bool g4x_compute_srwm(struct drm_device *dev,
			     int plane,
			     int latency_ns,
			     const struct intel_watermark_params *display,
			     const struct intel_watermark_params *cursor,
			     int *display_wm, int *cursor_wm)
{
	struct drm_crtc *crtc;
	int hdisplay, htotal, pixel_size, clock;
	unsigned long line_time_us;
	int line_count, line_size;
	int small, large;
	int entries;

	if (!latency_ns) {
		*display_wm = *cursor_wm = 0;
		return false;
	}

	crtc = intel_get_crtc_for_plane(dev, plane);
	hdisplay = crtc->mode.hdisplay;
	htotal = crtc->mode.htotal;
	clock = crtc->mode.clock;
	pixel_size = crtc->fb->bits_per_pixel / 8;

	line_time_us = (htotal * 1000) / clock;
	line_count = (latency_ns / line_time_us + 1000) / 1000;
	line_size = hdisplay * pixel_size;

	/* Use the minimum of the small and large buffer method for primary */
	small = ((clock * pixel_size / 1000) * latency_ns) / 1000;
	large = line_count * line_size;

	entries = DIV_ROUND_UP(min(small, large), display->cacheline_size);
	*display_wm = entries + display->guard_size;

	/* calculate the self-refresh watermark for display cursor */
	entries = line_count * pixel_size * 64;
	entries = DIV_ROUND_UP(entries, cursor->cacheline_size);
	*cursor_wm = entries + cursor->guard_size;

	return g4x_check_srwm(dev,
			      *display_wm, *cursor_wm,
			      display, cursor);
}

#define single_plane_enabled(mask) is_power_of_2(mask)

static void g4x_update_wm(struct drm_device *dev)
{
	static const int sr_latency_ns = 12000;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int planea_wm, planeb_wm, cursora_wm, cursorb_wm;
	int plane_sr, cursor_sr;
	unsigned int enabled = 0;

	if (g4x_compute_wm0(dev, 0,
			    &g4x_wm_info, latency_ns,
			    &g4x_cursor_wm_info, latency_ns,
			    &planea_wm, &cursora_wm))
		enabled |= 1;

	if (g4x_compute_wm0(dev, 1,
			    &g4x_wm_info, latency_ns,
			    &g4x_cursor_wm_info, latency_ns,
			    &planeb_wm, &cursorb_wm))
		enabled |= 2;

	plane_sr = cursor_sr = 0;
	if (single_plane_enabled(enabled) &&
	    g4x_compute_srwm(dev, ffs(enabled) - 1,
			     sr_latency_ns,
			     &g4x_wm_info,
			     &g4x_cursor_wm_info,
			     &plane_sr, &cursor_sr))
		I915_WRITE(FW_BLC_SELF, FW_BLC_SELF_EN);
	else
		I915_WRITE(FW_BLC_SELF,
			   I915_READ(FW_BLC_SELF) & ~FW_BLC_SELF_EN);

	DRM_DEBUG_KMS("Setting FIFO watermarks - A: plane=%d, cursor=%d, B: plane=%d, cursor=%d, SR: plane=%d, cursor=%d\n",
		      planea_wm, cursora_wm,
		      planeb_wm, cursorb_wm,
		      plane_sr, cursor_sr);

	I915_WRITE(DSPFW1,
		   (plane_sr << DSPFW_SR_SHIFT) |
		   (cursorb_wm << DSPFW_CURSORB_SHIFT) |
		   (planeb_wm << DSPFW_PLANEB_SHIFT) |
		   planea_wm);
	I915_WRITE(DSPFW2,
		   (I915_READ(DSPFW2) & DSPFW_CURSORA_MASK) |
		   (cursora_wm << DSPFW_CURSORA_SHIFT));
	/* HPLL off in SR has some issues on G4x... disable it */
	I915_WRITE(DSPFW3,
		   (I915_READ(DSPFW3) & ~DSPFW_HPLL_SR_EN) |
		   (cursor_sr << DSPFW_CURSOR_SR_SHIFT));
}

static void i965_update_wm(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_crtc *crtc;
	int srwm = 1;
	int cursor_sr = 16;

	/* Calc sr entries for one plane configs */
	crtc = single_enabled_crtc(dev);
	if (crtc) {
		/* self-refresh has much higher latency */
		static const int sr_latency_ns = 12000;
		int clock = crtc->mode.clock;
		int htotal = crtc->mode.htotal;
		int hdisplay = crtc->mode.hdisplay;
		int pixel_size = crtc->fb->bits_per_pixel / 8;
		unsigned long line_time_us;
		int entries;

		line_time_us = ((htotal * 1000) / clock);

		/* Use ns/us then divide to preserve precision */
		entries = (((sr_latency_ns / line_time_us) + 1000) / 1000) *
			pixel_size * hdisplay;
		entries = DIV_ROUND_UP(entries, I915_FIFO_LINE_SIZE);
		srwm = I965_FIFO_SIZE - entries;
		if (srwm < 0)
			srwm = 1;
		srwm &= 0x1ff;
		DRM_DEBUG_KMS("self-refresh entries: %d, wm: %d\n",
			      entries, srwm);

		entries = (((sr_latency_ns / line_time_us) + 1000) / 1000) *
			pixel_size * 64;
		entries = DIV_ROUND_UP(entries,
					  i965_cursor_wm_info.cacheline_size);
		cursor_sr = i965_cursor_wm_info.fifo_size -
			(entries + i965_cursor_wm_info.guard_size);

		if (cursor_sr > i965_cursor_wm_info.max_wm)
			cursor_sr = i965_cursor_wm_info.max_wm;

		DRM_DEBUG_KMS("self-refresh watermark: display plane %d "
			      "cursor %d\n", srwm, cursor_sr);

		if (IS_CRESTLINE(dev))
			I915_WRITE(FW_BLC_SELF, FW_BLC_SELF_EN);
	} else {
		/* Turn off self refresh if both pipes are enabled */
		if (IS_CRESTLINE(dev))
			I915_WRITE(FW_BLC_SELF, I915_READ(FW_BLC_SELF)
				   & ~FW_BLC_SELF_EN);
	}

	DRM_DEBUG_KMS("Setting FIFO watermarks - A: 8, B: 8, C: 8, SR %d\n",
		      srwm);

	/* 965 has limitations... */
	I915_WRITE(DSPFW1, (srwm << DSPFW_SR_SHIFT) |
		   (8 << 16) | (8 << 8) | (8 << 0));
	I915_WRITE(DSPFW2, (8 << 8) | (8 << 0));
	/* update cursor SR watermark */
	I915_WRITE(DSPFW3, (cursor_sr << DSPFW_CURSOR_SR_SHIFT));
}

static void i9xx_update_wm(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	const struct intel_watermark_params *wm_info;
	uint32_t fwater_lo;
	uint32_t fwater_hi;
	int cwm, srwm = 1;
	int fifo_size;
	int planea_wm, planeb_wm;
	struct drm_crtc *crtc, *enabled = NULL;

	if (IS_I945GM(dev))
		wm_info = &i945_wm_info;
	else if (!IS_GEN2(dev))
		wm_info = &i915_wm_info;
	else
		wm_info = &i855_wm_info;

	fifo_size = dev_priv->display.get_fifo_size(dev, 0);
	crtc = intel_get_crtc_for_plane(dev, 0);
	if (crtc->enabled && crtc->fb) {
		planea_wm = intel_calculate_wm(crtc->mode.clock,
					       wm_info, fifo_size,
					       crtc->fb->bits_per_pixel / 8,
					       latency_ns);
		enabled = crtc;
	} else
		planea_wm = fifo_size - wm_info->guard_size;

	fifo_size = dev_priv->display.get_fifo_size(dev, 1);
	crtc = intel_get_crtc_for_plane(dev, 1);
	if (crtc->enabled && crtc->fb) {
		planeb_wm = intel_calculate_wm(crtc->mode.clock,
					       wm_info, fifo_size,
					       crtc->fb->bits_per_pixel / 8,
					       latency_ns);
		if (enabled == NULL)
			enabled = crtc;
		else
			enabled = NULL;
	} else
		planeb_wm = fifo_size - wm_info->guard_size;

	DRM_DEBUG_KMS("FIFO watermarks - A: %d, B: %d\n", planea_wm, planeb_wm);

	/*
	 * Overlay gets an aggressive default since video jitter is bad.
	 */
	cwm = 2;

	/* Play safe and disable self-refresh before adjusting watermarks. */
	if (IS_I945G(dev) || IS_I945GM(dev))
		I915_WRITE(FW_BLC_SELF, FW_BLC_SELF_EN_MASK | 0);
	else if (IS_I915GM(dev))
		I915_WRITE(INSTPM, I915_READ(INSTPM) & ~INSTPM_SELF_EN);

	/* Calc sr entries for one plane configs */
	if (HAS_FW_BLC(dev) && enabled) {
		/* self-refresh has much higher latency */
		static const int sr_latency_ns = 6000;
		int clock = enabled->mode.clock;
		int htotal = enabled->mode.htotal;
		int hdisplay = enabled->mode.hdisplay;
		int pixel_size = enabled->fb->bits_per_pixel / 8;
		unsigned long line_time_us;
		int entries;

		line_time_us = (htotal * 1000) / clock;

		/* Use ns/us then divide to preserve precision */
		entries = (((sr_latency_ns / line_time_us) + 1000) / 1000) *
			pixel_size * hdisplay;
		entries = DIV_ROUND_UP(entries, wm_info->cacheline_size);
		DRM_DEBUG_KMS("self-refresh entries: %d\n", entries);
		srwm = wm_info->fifo_size - entries;
		if (srwm < 0)
			srwm = 1;

		if (IS_I945G(dev) || IS_I945GM(dev))
			I915_WRITE(FW_BLC_SELF,
				   FW_BLC_SELF_FIFO_MASK | (srwm & 0xff));
		else if (IS_I915GM(dev))
			I915_WRITE(FW_BLC_SELF, srwm & 0x3f);
	}

	DRM_DEBUG_KMS("Setting FIFO watermarks - A: %d, B: %d, C: %d, SR %d\n",
		      planea_wm, planeb_wm, cwm, srwm);

	fwater_lo = ((planeb_wm & 0x3f) << 16) | (planea_wm & 0x3f);
	fwater_hi = (cwm & 0x1f);

	/* Set request length to 8 cachelines per fetch */
	fwater_lo = fwater_lo | (1 << 24) | (1 << 8);
	fwater_hi = fwater_hi | (1 << 8);

	I915_WRITE(FW_BLC, fwater_lo);
	I915_WRITE(FW_BLC2, fwater_hi);

	if (HAS_FW_BLC(dev)) {
		if (enabled) {
			if (IS_I945G(dev) || IS_I945GM(dev))
				I915_WRITE(FW_BLC_SELF,
					   FW_BLC_SELF_EN_MASK | FW_BLC_SELF_EN);
			else if (IS_I915GM(dev))
				I915_WRITE(INSTPM, I915_READ(INSTPM) | INSTPM_SELF_EN);
			DRM_DEBUG_KMS("memory self refresh enabled\n");
		} else
			DRM_DEBUG_KMS("memory self refresh disabled\n");
	}
}

static void i830_update_wm(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_crtc *crtc;
	uint32_t fwater_lo;
	int planea_wm;

	crtc = single_enabled_crtc(dev);
	if (crtc == NULL)
		return;

	planea_wm = intel_calculate_wm(crtc->mode.clock, &i830_wm_info,
				       dev_priv->display.get_fifo_size(dev, 0),
				       crtc->fb->bits_per_pixel / 8,
				       latency_ns);
	fwater_lo = I915_READ(FW_BLC) & ~0xfff;
	fwater_lo |= (3<<8) | planea_wm;

	DRM_DEBUG_KMS("Setting FIFO watermarks - A: %d\n", planea_wm);

	I915_WRITE(FW_BLC, fwater_lo);
}

#define ILK_LP0_PLANE_LATENCY		700
#define ILK_LP0_CURSOR_LATENCY		1300

/*
 * Check the wm result.
 *
 * If any calculated watermark values is larger than the maximum value that
 * can be programmed into the associated watermark register, that watermark
 * must be disabled.
 */
static bool ironlake_check_srwm(struct drm_device *dev, int level,
				int fbc_wm, int display_wm, int cursor_wm,
				const struct intel_watermark_params *display,
				const struct intel_watermark_params *cursor)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	DRM_DEBUG_KMS("watermark %d: display plane %d, fbc lines %d,"
		      " cursor %d\n", level, display_wm, fbc_wm, cursor_wm);

	if (fbc_wm > SNB_FBC_MAX_SRWM) {
		DRM_DEBUG_KMS("fbc watermark(%d) is too large(%d), disabling wm%d+\n",
			      fbc_wm, SNB_FBC_MAX_SRWM, level);

		/* fbc has it's own way to disable FBC WM */
		I915_WRITE(DISP_ARB_CTL,
			   I915_READ(DISP_ARB_CTL) | DISP_FBC_WM_DIS);
		return false;
	}

	if (display_wm > display->max_wm) {
		DRM_DEBUG_KMS("display watermark(%d) is too large(%d), disabling wm%d+\n",
			      display_wm, SNB_DISPLAY_MAX_SRWM, level);
		return false;
	}

	if (cursor_wm > cursor->max_wm) {
		DRM_DEBUG_KMS("cursor watermark(%d) is too large(%d), disabling wm%d+\n",
			      cursor_wm, SNB_CURSOR_MAX_SRWM, level);
		return false;
	}

	if (!(fbc_wm || display_wm || cursor_wm)) {
		DRM_DEBUG_KMS("latency %d is 0, disabling wm%d+\n", level, level);
		return false;
	}

	return true;
}

/*
 * Compute watermark values of WM[1-3],
 */
static bool ironlake_compute_srwm(struct drm_device *dev, int level, int plane,
				  int latency_ns,
				  const struct intel_watermark_params *display,
				  const struct intel_watermark_params *cursor,
				  int *fbc_wm, int *display_wm, int *cursor_wm)
{
	struct drm_crtc *crtc;
	unsigned long line_time_us;
	int hdisplay, htotal, pixel_size, clock;
	int line_count, line_size;
	int small, large;
	int entries;

	if (!latency_ns) {
		*fbc_wm = *display_wm = *cursor_wm = 0;
		return false;
	}

	crtc = intel_get_crtc_for_plane(dev, plane);
	hdisplay = crtc->mode.hdisplay;
	htotal = crtc->mode.htotal;
	clock = crtc->mode.clock;
	pixel_size = crtc->fb->bits_per_pixel / 8;

	line_time_us = (htotal * 1000) / clock;
	line_count = (latency_ns / line_time_us + 1000) / 1000;
	line_size = hdisplay * pixel_size;

	/* Use the minimum of the small and large buffer method for primary */
	small = ((clock * pixel_size / 1000) * latency_ns) / 1000;
	large = line_count * line_size;

	entries = DIV_ROUND_UP(min(small, large), display->cacheline_size);
	*display_wm = entries + display->guard_size;

	/*
	 * Spec says:
	 * FBC WM = ((Final Primary WM * 64) / number of bytes per line) + 2
	 */
	*fbc_wm = DIV_ROUND_UP(*display_wm * 64, line_size) + 2;

	/* calculate the self-refresh watermark for display cursor */
	entries = line_count * pixel_size * 64;
	entries = DIV_ROUND_UP(entries, cursor->cacheline_size);
	*cursor_wm = entries + cursor->guard_size;

	return ironlake_check_srwm(dev, level,
				   *fbc_wm, *display_wm, *cursor_wm,
				   display, cursor);
}

static void ironlake_update_wm(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int fbc_wm, plane_wm, cursor_wm;
	unsigned int enabled;

	enabled = 0;
	if (g4x_compute_wm0(dev, 0,
			    &ironlake_display_wm_info,
			    ILK_LP0_PLANE_LATENCY,
			    &ironlake_cursor_wm_info,
			    ILK_LP0_CURSOR_LATENCY,
			    &plane_wm, &cursor_wm)) {
		I915_WRITE(WM0_PIPEA_ILK,
			   (plane_wm << WM0_PIPE_PLANE_SHIFT) | cursor_wm);
		DRM_DEBUG_KMS("FIFO watermarks For pipe A -"
			      " plane %d, " "cursor: %d\n",
			      plane_wm, cursor_wm);
		enabled |= 1;
	}

	if (g4x_compute_wm0(dev, 1,
			    &ironlake_display_wm_info,
			    ILK_LP0_PLANE_LATENCY,
			    &ironlake_cursor_wm_info,
			    ILK_LP0_CURSOR_LATENCY,
			    &plane_wm, &cursor_wm)) {
		I915_WRITE(WM0_PIPEB_ILK,
			   (plane_wm << WM0_PIPE_PLANE_SHIFT) | cursor_wm);
		DRM_DEBUG_KMS("FIFO watermarks For pipe B -"
			      " plane %d, cursor: %d\n",
			      plane_wm, cursor_wm);
		enabled |= 2;
	}

	/*
	 * Calculate and update the self-refresh watermark only when one
	 * display plane is used.
	 */
	I915_WRITE(WM3_LP_ILK, 0);
	I915_WRITE(WM2_LP_ILK, 0);
	I915_WRITE(WM1_LP_ILK, 0);

	if (!single_plane_enabled(enabled))
		return;
	enabled = ffs(enabled) - 1;

	/* WM1 */
	if (!ironlake_compute_srwm(dev, 1, enabled,
				   ILK_READ_WM1_LATENCY() * 500,
				   &ironlake_display_srwm_info,
				   &ironlake_cursor_srwm_info,
				   &fbc_wm, &plane_wm, &cursor_wm))
		return;

	I915_WRITE(WM1_LP_ILK,
		   WM1_LP_SR_EN |
		   (ILK_READ_WM1_LATENCY() << WM1_LP_LATENCY_SHIFT) |
		   (fbc_wm << WM1_LP_FBC_SHIFT) |
		   (plane_wm << WM1_LP_SR_SHIFT) |
		   cursor_wm);

	/* WM2 */
	if (!ironlake_compute_srwm(dev, 2, enabled,
				   ILK_READ_WM2_LATENCY() * 500,
				   &ironlake_display_srwm_info,
				   &ironlake_cursor_srwm_info,
				   &fbc_wm, &plane_wm, &cursor_wm))
		return;

	I915_WRITE(WM2_LP_ILK,
		   WM2_LP_EN |
		   (ILK_READ_WM2_LATENCY() << WM1_LP_LATENCY_SHIFT) |
		   (fbc_wm << WM1_LP_FBC_SHIFT) |
		   (plane_wm << WM1_LP_SR_SHIFT) |
		   cursor_wm);

	/*
	 * WM3 is unsupported on ILK, probably because we don't have latency
	 * data for that power state
	 */
}

<<<<<<< HEAD
static void sandybridge_update_wm(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int latency = SNB_READ_WM0_LATENCY() * 100;	/* In unit 0.1us */
=======
void sandybridge_update_wm(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int latency = SNB_READ_WM0_LATENCY() * 100;	/* In unit 0.1us */
	u32 val;
>>>>>>> refs/remotes/origin/cm-10.0
	int fbc_wm, plane_wm, cursor_wm;
	unsigned int enabled;

	enabled = 0;
	if (g4x_compute_wm0(dev, 0,
			    &sandybridge_display_wm_info, latency,
			    &sandybridge_cursor_wm_info, latency,
			    &plane_wm, &cursor_wm)) {
<<<<<<< HEAD
		I915_WRITE(WM0_PIPEA_ILK,
			   (plane_wm << WM0_PIPE_PLANE_SHIFT) | cursor_wm);
=======
		val = I915_READ(WM0_PIPEA_ILK);
		val &= ~(WM0_PIPE_PLANE_MASK | WM0_PIPE_CURSOR_MASK);
		I915_WRITE(WM0_PIPEA_ILK, val |
			   ((plane_wm << WM0_PIPE_PLANE_SHIFT) | cursor_wm));
>>>>>>> refs/remotes/origin/cm-10.0
		DRM_DEBUG_KMS("FIFO watermarks For pipe A -"
			      " plane %d, " "cursor: %d\n",
			      plane_wm, cursor_wm);
		enabled |= 1;
	}

	if (g4x_compute_wm0(dev, 1,
			    &sandybridge_display_wm_info, latency,
			    &sandybridge_cursor_wm_info, latency,
			    &plane_wm, &cursor_wm)) {
<<<<<<< HEAD
		I915_WRITE(WM0_PIPEB_ILK,
			   (plane_wm << WM0_PIPE_PLANE_SHIFT) | cursor_wm);
=======
		val = I915_READ(WM0_PIPEB_ILK);
		val &= ~(WM0_PIPE_PLANE_MASK | WM0_PIPE_CURSOR_MASK);
		I915_WRITE(WM0_PIPEB_ILK, val |
			   ((plane_wm << WM0_PIPE_PLANE_SHIFT) | cursor_wm));
>>>>>>> refs/remotes/origin/cm-10.0
		DRM_DEBUG_KMS("FIFO watermarks For pipe B -"
			      " plane %d, cursor: %d\n",
			      plane_wm, cursor_wm);
		enabled |= 2;
	}

<<<<<<< HEAD
=======
	/* IVB has 3 pipes */
	if (IS_IVYBRIDGE(dev) &&
	    g4x_compute_wm0(dev, 2,
			    &sandybridge_display_wm_info, latency,
			    &sandybridge_cursor_wm_info, latency,
			    &plane_wm, &cursor_wm)) {
		val = I915_READ(WM0_PIPEC_IVB);
		val &= ~(WM0_PIPE_PLANE_MASK | WM0_PIPE_CURSOR_MASK);
		I915_WRITE(WM0_PIPEC_IVB, val |
			   ((plane_wm << WM0_PIPE_PLANE_SHIFT) | cursor_wm));
		DRM_DEBUG_KMS("FIFO watermarks For pipe C -"
			      " plane %d, cursor: %d\n",
			      plane_wm, cursor_wm);
		enabled |= 3;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Calculate and update the self-refresh watermark only when one
	 * display plane is used.
	 *
	 * SNB support 3 levels of watermark.
	 *
	 * WM1/WM2/WM2 watermarks have to be enabled in the ascending order,
	 * and disabled in the descending order
	 *
	 */
	I915_WRITE(WM3_LP_ILK, 0);
	I915_WRITE(WM2_LP_ILK, 0);
	I915_WRITE(WM1_LP_ILK, 0);

<<<<<<< HEAD
	if (!single_plane_enabled(enabled))
=======
	if (!single_plane_enabled(enabled) ||
	    dev_priv->sprite_scaling_enabled)
>>>>>>> refs/remotes/origin/cm-10.0
		return;
	enabled = ffs(enabled) - 1;

	/* WM1 */
	if (!ironlake_compute_srwm(dev, 1, enabled,
				   SNB_READ_WM1_LATENCY() * 500,
				   &sandybridge_display_srwm_info,
				   &sandybridge_cursor_srwm_info,
				   &fbc_wm, &plane_wm, &cursor_wm))
		return;

	I915_WRITE(WM1_LP_ILK,
		   WM1_LP_SR_EN |
		   (SNB_READ_WM1_LATENCY() << WM1_LP_LATENCY_SHIFT) |
		   (fbc_wm << WM1_LP_FBC_SHIFT) |
		   (plane_wm << WM1_LP_SR_SHIFT) |
		   cursor_wm);

	/* WM2 */
	if (!ironlake_compute_srwm(dev, 2, enabled,
				   SNB_READ_WM2_LATENCY() * 500,
				   &sandybridge_display_srwm_info,
				   &sandybridge_cursor_srwm_info,
				   &fbc_wm, &plane_wm, &cursor_wm))
		return;

	I915_WRITE(WM2_LP_ILK,
		   WM2_LP_EN |
		   (SNB_READ_WM2_LATENCY() << WM1_LP_LATENCY_SHIFT) |
		   (fbc_wm << WM1_LP_FBC_SHIFT) |
		   (plane_wm << WM1_LP_SR_SHIFT) |
		   cursor_wm);

	/* WM3 */
	if (!ironlake_compute_srwm(dev, 3, enabled,
				   SNB_READ_WM3_LATENCY() * 500,
				   &sandybridge_display_srwm_info,
				   &sandybridge_cursor_srwm_info,
				   &fbc_wm, &plane_wm, &cursor_wm))
		return;

	I915_WRITE(WM3_LP_ILK,
		   WM3_LP_EN |
		   (SNB_READ_WM3_LATENCY() << WM1_LP_LATENCY_SHIFT) |
		   (fbc_wm << WM1_LP_FBC_SHIFT) |
		   (plane_wm << WM1_LP_SR_SHIFT) |
		   cursor_wm);
}

<<<<<<< HEAD
=======
static bool
sandybridge_compute_sprite_wm(struct drm_device *dev, int plane,
			      uint32_t sprite_width, int pixel_size,
			      const struct intel_watermark_params *display,
			      int display_latency_ns, int *sprite_wm)
{
	struct drm_crtc *crtc;
	int clock;
	int entries, tlb_miss;

	crtc = intel_get_crtc_for_plane(dev, plane);
	if (crtc->fb == NULL || !crtc->enabled) {
		*sprite_wm = display->guard_size;
		return false;
	}

	clock = crtc->mode.clock;

	/* Use the small buffer method to calculate the sprite watermark */
	entries = ((clock * pixel_size / 1000) * display_latency_ns) / 1000;
	tlb_miss = display->fifo_size*display->cacheline_size -
		sprite_width * 8;
	if (tlb_miss > 0)
		entries += tlb_miss;
	entries = DIV_ROUND_UP(entries, display->cacheline_size);
	*sprite_wm = entries + display->guard_size;
	if (*sprite_wm > (int)display->max_wm)
		*sprite_wm = display->max_wm;

	return true;
}

static bool
sandybridge_compute_sprite_srwm(struct drm_device *dev, int plane,
				uint32_t sprite_width, int pixel_size,
				const struct intel_watermark_params *display,
				int latency_ns, int *sprite_wm)
{
	struct drm_crtc *crtc;
	unsigned long line_time_us;
	int clock;
	int line_count, line_size;
	int small, large;
	int entries;

	if (!latency_ns) {
		*sprite_wm = 0;
		return false;
	}

	crtc = intel_get_crtc_for_plane(dev, plane);
	clock = crtc->mode.clock;
	if (!clock) {
		*sprite_wm = 0;
		return false;
	}

	line_time_us = (sprite_width * 1000) / clock;
	if (!line_time_us) {
		*sprite_wm = 0;
		return false;
	}

	line_count = (latency_ns / line_time_us + 1000) / 1000;
	line_size = sprite_width * pixel_size;

	/* Use the minimum of the small and large buffer method for primary */
	small = ((clock * pixel_size / 1000) * latency_ns) / 1000;
	large = line_count * line_size;

	entries = DIV_ROUND_UP(min(small, large), display->cacheline_size);
	*sprite_wm = entries + display->guard_size;

	return *sprite_wm > 0x3ff ? false : true;
}

static void sandybridge_update_sprite_wm(struct drm_device *dev, int pipe,
					 uint32_t sprite_width, int pixel_size)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int latency = SNB_READ_WM0_LATENCY() * 100;	/* In unit 0.1us */
	u32 val;
	int sprite_wm, reg;
	int ret;

	switch (pipe) {
	case 0:
		reg = WM0_PIPEA_ILK;
		break;
	case 1:
		reg = WM0_PIPEB_ILK;
		break;
	case 2:
		reg = WM0_PIPEC_IVB;
		break;
	default:
		return; /* bad pipe */
	}

	ret = sandybridge_compute_sprite_wm(dev, pipe, sprite_width, pixel_size,
					    &sandybridge_display_wm_info,
					    latency, &sprite_wm);
	if (!ret) {
		DRM_DEBUG_KMS("failed to compute sprite wm for pipe %d\n",
			      pipe);
		return;
	}

	val = I915_READ(reg);
	val &= ~WM0_PIPE_SPRITE_MASK;
	I915_WRITE(reg, val | (sprite_wm << WM0_PIPE_SPRITE_SHIFT));
	DRM_DEBUG_KMS("sprite watermarks For pipe %d - %d\n", pipe, sprite_wm);


	ret = sandybridge_compute_sprite_srwm(dev, pipe, sprite_width,
					      pixel_size,
					      &sandybridge_display_srwm_info,
					      SNB_READ_WM1_LATENCY() * 500,
					      &sprite_wm);
	if (!ret) {
		DRM_DEBUG_KMS("failed to compute sprite lp1 wm on pipe %d\n",
			      pipe);
		return;
	}
	I915_WRITE(WM1S_LP_ILK, sprite_wm);

	/* Only IVB has two more LP watermarks for sprite */
	if (!IS_IVYBRIDGE(dev))
		return;

	ret = sandybridge_compute_sprite_srwm(dev, pipe, sprite_width,
					      pixel_size,
					      &sandybridge_display_srwm_info,
					      SNB_READ_WM2_LATENCY() * 500,
					      &sprite_wm);
	if (!ret) {
		DRM_DEBUG_KMS("failed to compute sprite lp2 wm on pipe %d\n",
			      pipe);
		return;
	}
	I915_WRITE(WM2S_LP_IVB, sprite_wm);

	ret = sandybridge_compute_sprite_srwm(dev, pipe, sprite_width,
					      pixel_size,
					      &sandybridge_display_srwm_info,
					      SNB_READ_WM3_LATENCY() * 500,
					      &sprite_wm);
	if (!ret) {
		DRM_DEBUG_KMS("failed to compute sprite lp3 wm on pipe %d\n",
			      pipe);
		return;
	}
	I915_WRITE(WM3S_LP_IVB, sprite_wm);
}

>>>>>>> refs/remotes/origin/cm-10.0
/**
 * intel_update_watermarks - update FIFO watermark values based on current modes
 *
 * Calculate watermark values for the various WM regs based on current mode
 * and plane configuration.
 *
 * There are several cases to deal with here:
 *   - normal (i.e. non-self-refresh)
 *   - self-refresh (SR) mode
 *   - lines are large relative to FIFO size (buffer can hold up to 2)
 *   - lines are small relative to FIFO size (buffer can hold more than 2
 *     lines), so need to account for TLB latency
 *
 *   The normal calculation is:
 *     watermark = dotclock * bytes per pixel * latency
 *   where latency is platform & configuration dependent (we assume pessimal
 *   values here).
 *
 *   The SR calculation is:
 *     watermark = (trunc(latency/line time)+1) * surface width *
 *       bytes per pixel
 *   where
 *     line time = htotal / dotclock
 *     surface width = hdisplay for normal plane and 64 for cursor
 *   and latency is assumed to be high, as above.
 *
 * The final value programmed to the register should always be rounded up,
 * and include an extra 2 entries to account for clock crossings.
 *
 * We don't use the sprite, so we can ignore that.  And on Crestline we have
 * to set the non-SR watermarks to 8.
 */
static void intel_update_watermarks(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (dev_priv->display.update_wm)
		dev_priv->display.update_wm(dev);
}

<<<<<<< HEAD
static inline bool intel_panel_use_ssc(struct drm_i915_private *dev_priv)
{
	return dev_priv->lvds_use_ssc && i915_panel_use_ssc
		&& !(dev_priv->quirks & QUIRK_LVDS_SSC_DISABLE);
}

=======
void intel_update_sprite_watermarks(struct drm_device *dev, int pipe,
				    uint32_t sprite_width, int pixel_size)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (dev_priv->display.update_sprite_wm)
		dev_priv->display.update_sprite_wm(dev, pipe, sprite_width,
						   pixel_size);
}

static inline bool intel_panel_use_ssc(struct drm_i915_private *dev_priv)
{
	if (i915_panel_use_ssc >= 0)
		return i915_panel_use_ssc != 0;
	return dev_priv->lvds_use_ssc
		&& !(dev_priv->quirks & QUIRK_LVDS_SSC_DISABLE);
}

/**
 * intel_choose_pipe_bpp_dither - figure out what color depth the pipe should send
 * @crtc: CRTC structure
 * @mode: requested mode
 *
 * A pipe may be connected to one or more outputs.  Based on the depth of the
 * attached framebuffer, choose a good color depth to use on the pipe.
 *
 * If possible, match the pipe depth to the fb depth.  In some cases, this
 * isn't ideal, because the connected output supports a lesser or restricted
 * set of depths.  Resolve that here:
 *    LVDS typically supports only 6bpc, so clamp down in that case
 *    HDMI supports only 8bpc or 12bpc, so clamp to 8bpc with dither for 10bpc
 *    Displays may support a restricted set as well, check EDID and clamp as
 *      appropriate.
 *    DP may want to dither down to 6bpc to fit larger modes
 *
 * RETURNS:
 * Dithering requirement (i.e. false if display bpc and pipe bpc match,
 * true if they don't match).
 */
static bool intel_choose_pipe_bpp_dither(struct drm_crtc *crtc,
					 unsigned int *pipe_bpp,
					 struct drm_display_mode *mode)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_encoder *encoder;
	struct drm_connector *connector;
	unsigned int display_bpc = UINT_MAX, bpc;

	/* Walk the encoders & connectors on this crtc, get min bpc */
	list_for_each_entry(encoder, &dev->mode_config.encoder_list, head) {
		struct intel_encoder *intel_encoder = to_intel_encoder(encoder);

		if (encoder->crtc != crtc)
			continue;

		if (intel_encoder->type == INTEL_OUTPUT_LVDS) {
			unsigned int lvds_bpc;

			if ((I915_READ(PCH_LVDS) & LVDS_A3_POWER_MASK) ==
			    LVDS_A3_POWER_UP)
				lvds_bpc = 8;
			else
				lvds_bpc = 6;

			if (lvds_bpc < display_bpc) {
				DRM_DEBUG_KMS("clamping display bpc (was %d) to LVDS (%d)\n", display_bpc, lvds_bpc);
				display_bpc = lvds_bpc;
			}
			continue;
		}

		/* Not one of the known troublemakers, check the EDID */
		list_for_each_entry(connector, &dev->mode_config.connector_list,
				    head) {
			if (connector->encoder != encoder)
				continue;

			/* Don't use an invalid EDID bpc value */
			if (connector->display_info.bpc &&
			    connector->display_info.bpc < display_bpc) {
				DRM_DEBUG_KMS("clamping display bpc (was %d) to EDID reported max of %d\n", display_bpc, connector->display_info.bpc);
				display_bpc = connector->display_info.bpc;
			}
		}

		if (intel_encoder->type == INTEL_OUTPUT_EDP) {
			/* Use VBT settings if we have an eDP panel */
			unsigned int edp_bpc = dev_priv->edp.bpp / 3;

			if (edp_bpc && edp_bpc < display_bpc) {
				DRM_DEBUG_KMS("clamping display bpc (was %d) to eDP (%d)\n", display_bpc, edp_bpc);
				display_bpc = edp_bpc;
			}
			continue;
		}

		/*
		 * HDMI is either 12 or 8, so if the display lets 10bpc sneak
		 * through, clamp it down.  (Note: >12bpc will be caught below.)
		 */
		if (intel_encoder->type == INTEL_OUTPUT_HDMI) {
			if (display_bpc > 8 && display_bpc < 12) {
				DRM_DEBUG_KMS("forcing bpc to 12 for HDMI\n");
				display_bpc = 12;
			} else {
				DRM_DEBUG_KMS("forcing bpc to 8 for HDMI\n");
				display_bpc = 8;
			}
		}
	}

	if (mode->private_flags & INTEL_MODE_DP_FORCE_6BPC) {
		DRM_DEBUG_KMS("Dithering DP to 6bpc\n");
		display_bpc = 6;
	}

	/*
	 * We could just drive the pipe at the highest bpc all the time and
	 * enable dithering as needed, but that costs bandwidth.  So choose
	 * the minimum value that expresses the full color range of the fb but
	 * also stays within the max display bpc discovered above.
	 */

	switch (crtc->fb->depth) {
	case 8:
		bpc = 8; /* since we go through a colormap */
		break;
	case 15:
	case 16:
		bpc = 6; /* min is 18bpp */
		break;
	case 24:
		bpc = 8;
		break;
	case 30:
		bpc = 10;
		break;
	case 48:
		bpc = 12;
		break;
	default:
		DRM_DEBUG("unsupported depth, assuming 24 bits\n");
		bpc = min((unsigned int)8, display_bpc);
		break;
	}

	display_bpc = min(display_bpc, bpc);

	DRM_DEBUG_KMS("setting pipe bpc to %d (max display bpc %d)\n",
		      bpc, display_bpc);

	*pipe_bpp = display_bpc * 3;

	return display_bpc != bpc;
}

static int i9xx_get_refclk(struct drm_crtc *crtc, int num_connectors)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int refclk;

	if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS) &&
	    intel_panel_use_ssc(dev_priv) && num_connectors < 2) {
		refclk = dev_priv->lvds_ssc_freq * 1000;
		DRM_DEBUG_KMS("using SSC reference clock of %d MHz\n",
			      refclk / 1000);
	} else if (!IS_GEN2(dev)) {
		refclk = 96000;
	} else {
		refclk = 48000;
	}

	return refclk;
}

static void i9xx_adjust_sdvo_tv_clock(struct drm_display_mode *adjusted_mode,
				      intel_clock_t *clock)
{
	/* SDVO TV has fixed PLL values depend on its clock range,
	   this mirrors vbios setting. */
	if (adjusted_mode->clock >= 100000
	    && adjusted_mode->clock < 140500) {
		clock->p1 = 2;
		clock->p2 = 10;
		clock->n = 3;
		clock->m1 = 16;
		clock->m2 = 8;
	} else if (adjusted_mode->clock >= 140500
		   && adjusted_mode->clock <= 200000) {
		clock->p1 = 1;
		clock->p2 = 10;
		clock->n = 6;
		clock->m1 = 12;
		clock->m2 = 8;
	}
}

static void i9xx_update_pll_dividers(struct drm_crtc *crtc,
				     intel_clock_t *clock,
				     intel_clock_t *reduced_clock)
=======
static void ironlake_init_pch_refclk(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_mode_config *mode_config = &dev->mode_config;
	struct intel_encoder *encoder;
	u32 val, final;
	bool has_lvds = false;
	bool has_cpu_edp = false;
	bool has_panel = false;
	bool has_ck505 = false;
	bool can_ssc = false;

	/* We need to take the global config into account */
	list_for_each_entry(encoder, &mode_config->encoder_list,
			    base.head) {
		switch (encoder->type) {
		case INTEL_OUTPUT_LVDS:
			has_panel = true;
			has_lvds = true;
			break;
		case INTEL_OUTPUT_EDP:
			has_panel = true;
			if (enc_to_dig_port(&encoder->base)->port == PORT_A)
				has_cpu_edp = true;
			break;
		}
	}

	if (HAS_PCH_IBX(dev)) {
		has_ck505 = dev_priv->vbt.display_clock_mode;
		can_ssc = has_ck505;
	} else {
		has_ck505 = false;
		can_ssc = true;
	}

	DRM_DEBUG_KMS("has_panel %d has_lvds %d has_ck505 %d\n",
		      has_panel, has_lvds, has_ck505);

	/* Ironlake: try to setup display ref clock before DPLL
	 * enabling. This is only under driver's control after
	 * PCH B stepping, previous chipset stepping should be
	 * ignoring this setting.
	 */
	val = I915_READ(PCH_DREF_CONTROL);

	/* As we must carefully and slowly disable/enable each source in turn,
	 * compute the final state we want first and check if we need to
	 * make any changes at all.
	 */
	final = val;
	final &= ~DREF_NONSPREAD_SOURCE_MASK;
	if (has_ck505)
		final |= DREF_NONSPREAD_CK505_ENABLE;
	else
		final |= DREF_NONSPREAD_SOURCE_ENABLE;

	final &= ~DREF_SSC_SOURCE_MASK;
	final &= ~DREF_CPU_SOURCE_OUTPUT_MASK;
	final &= ~DREF_SSC1_ENABLE;

	if (has_panel) {
		final |= DREF_SSC_SOURCE_ENABLE;

		if (intel_panel_use_ssc(dev_priv) && can_ssc)
			final |= DREF_SSC1_ENABLE;

		if (has_cpu_edp) {
			if (intel_panel_use_ssc(dev_priv) && can_ssc)
				final |= DREF_CPU_SOURCE_OUTPUT_DOWNSPREAD;
			else
				final |= DREF_CPU_SOURCE_OUTPUT_NONSPREAD;
		} else
			final |= DREF_CPU_SOURCE_OUTPUT_DISABLE;
	} else {
		final |= DREF_SSC_SOURCE_DISABLE;
		final |= DREF_CPU_SOURCE_OUTPUT_DISABLE;
	}

	if (final == val)
		return;

	/* Always enable nonspread source */
	val &= ~DREF_NONSPREAD_SOURCE_MASK;

	if (has_ck505)
		val |= DREF_NONSPREAD_CK505_ENABLE;
	else
		val |= DREF_NONSPREAD_SOURCE_ENABLE;

	if (has_panel) {
		val &= ~DREF_SSC_SOURCE_MASK;
		val |= DREF_SSC_SOURCE_ENABLE;

		/* SSC must be turned on before enabling the CPU output  */
		if (intel_panel_use_ssc(dev_priv) && can_ssc) {
			DRM_DEBUG_KMS("Using SSC on panel\n");
			val |= DREF_SSC1_ENABLE;
		} else
			val &= ~DREF_SSC1_ENABLE;

		/* Get SSC going before enabling the outputs */
		I915_WRITE(PCH_DREF_CONTROL, val);
		POSTING_READ(PCH_DREF_CONTROL);
		udelay(200);

		val &= ~DREF_CPU_SOURCE_OUTPUT_MASK;

		/* Enable CPU source on CPU attached eDP */
		if (has_cpu_edp) {
			if (intel_panel_use_ssc(dev_priv) && can_ssc) {
				DRM_DEBUG_KMS("Using SSC on eDP\n");
				val |= DREF_CPU_SOURCE_OUTPUT_DOWNSPREAD;
			}
			else
				val |= DREF_CPU_SOURCE_OUTPUT_NONSPREAD;
		} else
			val |= DREF_CPU_SOURCE_OUTPUT_DISABLE;

		I915_WRITE(PCH_DREF_CONTROL, val);
		POSTING_READ(PCH_DREF_CONTROL);
		udelay(200);
	} else {
		DRM_DEBUG_KMS("Disabling SSC entirely\n");

		val &= ~DREF_CPU_SOURCE_OUTPUT_MASK;

		/* Turn off CPU output */
		val |= DREF_CPU_SOURCE_OUTPUT_DISABLE;

		I915_WRITE(PCH_DREF_CONTROL, val);
		POSTING_READ(PCH_DREF_CONTROL);
		udelay(200);

		/* Turn off the SSC source */
		val &= ~DREF_SSC_SOURCE_MASK;
		val |= DREF_SSC_SOURCE_DISABLE;

		/* Turn off SSC1 */
		val &= ~DREF_SSC1_ENABLE;

		I915_WRITE(PCH_DREF_CONTROL, val);
		POSTING_READ(PCH_DREF_CONTROL);
		udelay(200);
	}

	BUG_ON(val != final);
}

static void lpt_reset_fdi_mphy(struct drm_i915_private *dev_priv)
{
	uint32_t tmp;

	tmp = I915_READ(SOUTH_CHICKEN2);
	tmp |= FDI_MPHY_IOSFSB_RESET_CTL;
	I915_WRITE(SOUTH_CHICKEN2, tmp);

	if (wait_for_atomic_us(I915_READ(SOUTH_CHICKEN2) &
			       FDI_MPHY_IOSFSB_RESET_STATUS, 100))
		DRM_ERROR("FDI mPHY reset assert timeout\n");

	tmp = I915_READ(SOUTH_CHICKEN2);
	tmp &= ~FDI_MPHY_IOSFSB_RESET_CTL;
	I915_WRITE(SOUTH_CHICKEN2, tmp);

	if (wait_for_atomic_us((I915_READ(SOUTH_CHICKEN2) &
				FDI_MPHY_IOSFSB_RESET_STATUS) == 0, 100))
		DRM_ERROR("FDI mPHY reset de-assert timeout\n");
}

/* WaMPhyProgramming:hsw */
static void lpt_program_fdi_mphy(struct drm_i915_private *dev_priv)
{
	uint32_t tmp;

	tmp = intel_sbi_read(dev_priv, 0x8008, SBI_MPHY);
	tmp &= ~(0xFF << 24);
	tmp |= (0x12 << 24);
	intel_sbi_write(dev_priv, 0x8008, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x2008, SBI_MPHY);
	tmp |= (1 << 11);
	intel_sbi_write(dev_priv, 0x2008, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x2108, SBI_MPHY);
	tmp |= (1 << 11);
	intel_sbi_write(dev_priv, 0x2108, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x206C, SBI_MPHY);
	tmp |= (1 << 24) | (1 << 21) | (1 << 18);
	intel_sbi_write(dev_priv, 0x206C, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x216C, SBI_MPHY);
	tmp |= (1 << 24) | (1 << 21) | (1 << 18);
	intel_sbi_write(dev_priv, 0x216C, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x2080, SBI_MPHY);
	tmp &= ~(7 << 13);
	tmp |= (5 << 13);
	intel_sbi_write(dev_priv, 0x2080, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x2180, SBI_MPHY);
	tmp &= ~(7 << 13);
	tmp |= (5 << 13);
	intel_sbi_write(dev_priv, 0x2180, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x208C, SBI_MPHY);
	tmp &= ~0xFF;
	tmp |= 0x1C;
	intel_sbi_write(dev_priv, 0x208C, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x218C, SBI_MPHY);
	tmp &= ~0xFF;
	tmp |= 0x1C;
	intel_sbi_write(dev_priv, 0x218C, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x2098, SBI_MPHY);
	tmp &= ~(0xFF << 16);
	tmp |= (0x1C << 16);
	intel_sbi_write(dev_priv, 0x2098, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x2198, SBI_MPHY);
	tmp &= ~(0xFF << 16);
	tmp |= (0x1C << 16);
	intel_sbi_write(dev_priv, 0x2198, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x20C4, SBI_MPHY);
	tmp |= (1 << 27);
	intel_sbi_write(dev_priv, 0x20C4, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x21C4, SBI_MPHY);
	tmp |= (1 << 27);
	intel_sbi_write(dev_priv, 0x21C4, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x20EC, SBI_MPHY);
	tmp &= ~(0xF << 28);
	tmp |= (4 << 28);
	intel_sbi_write(dev_priv, 0x20EC, tmp, SBI_MPHY);

	tmp = intel_sbi_read(dev_priv, 0x21EC, SBI_MPHY);
	tmp &= ~(0xF << 28);
	tmp |= (4 << 28);
	intel_sbi_write(dev_priv, 0x21EC, tmp, SBI_MPHY);
}

/* Implements 3 different sequences from BSpec chapter "Display iCLK
 * Programming" based on the parameters passed:
 * - Sequence to enable CLKOUT_DP
 * - Sequence to enable CLKOUT_DP without spread
 * - Sequence to enable CLKOUT_DP for FDI usage and configure PCH FDI I/O
 */
static void lpt_enable_clkout_dp(struct drm_device *dev, bool with_spread,
				 bool with_fdi)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t reg, tmp;

	if (WARN(with_fdi && !with_spread, "FDI requires downspread\n"))
		with_spread = true;
	if (WARN(dev_priv->pch_id == INTEL_PCH_LPT_LP_DEVICE_ID_TYPE &&
		 with_fdi, "LP PCH doesn't have FDI\n"))
		with_fdi = false;

	mutex_lock(&dev_priv->dpio_lock);

	tmp = intel_sbi_read(dev_priv, SBI_SSCCTL, SBI_ICLK);
	tmp &= ~SBI_SSCCTL_DISABLE;
	tmp |= SBI_SSCCTL_PATHALT;
	intel_sbi_write(dev_priv, SBI_SSCCTL, tmp, SBI_ICLK);

	udelay(24);

	if (with_spread) {
		tmp = intel_sbi_read(dev_priv, SBI_SSCCTL, SBI_ICLK);
		tmp &= ~SBI_SSCCTL_PATHALT;
		intel_sbi_write(dev_priv, SBI_SSCCTL, tmp, SBI_ICLK);

		if (with_fdi) {
			lpt_reset_fdi_mphy(dev_priv);
			lpt_program_fdi_mphy(dev_priv);
		}
	}

	reg = (dev_priv->pch_id == INTEL_PCH_LPT_LP_DEVICE_ID_TYPE) ?
	       SBI_GEN0 : SBI_DBUFF0;
	tmp = intel_sbi_read(dev_priv, reg, SBI_ICLK);
	tmp |= SBI_GEN0_CFG_BUFFENABLE_DISABLE;
	intel_sbi_write(dev_priv, reg, tmp, SBI_ICLK);

	mutex_unlock(&dev_priv->dpio_lock);
}

/* Sequence to disable CLKOUT_DP */
static void lpt_disable_clkout_dp(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t reg, tmp;

	mutex_lock(&dev_priv->dpio_lock);

	reg = (dev_priv->pch_id == INTEL_PCH_LPT_LP_DEVICE_ID_TYPE) ?
	       SBI_GEN0 : SBI_DBUFF0;
	tmp = intel_sbi_read(dev_priv, reg, SBI_ICLK);
	tmp &= ~SBI_GEN0_CFG_BUFFENABLE_DISABLE;
	intel_sbi_write(dev_priv, reg, tmp, SBI_ICLK);

	tmp = intel_sbi_read(dev_priv, SBI_SSCCTL, SBI_ICLK);
	if (!(tmp & SBI_SSCCTL_DISABLE)) {
		if (!(tmp & SBI_SSCCTL_PATHALT)) {
			tmp |= SBI_SSCCTL_PATHALT;
			intel_sbi_write(dev_priv, SBI_SSCCTL, tmp, SBI_ICLK);
			udelay(32);
		}
		tmp |= SBI_SSCCTL_DISABLE;
		intel_sbi_write(dev_priv, SBI_SSCCTL, tmp, SBI_ICLK);
	}

	mutex_unlock(&dev_priv->dpio_lock);
}

static void lpt_init_pch_refclk(struct drm_device *dev)
{
	struct drm_mode_config *mode_config = &dev->mode_config;
	struct intel_encoder *encoder;
	bool has_vga = false;

	list_for_each_entry(encoder, &mode_config->encoder_list, base.head) {
		switch (encoder->type) {
		case INTEL_OUTPUT_ANALOG:
			has_vga = true;
			break;
		}
	}

	if (has_vga)
		lpt_enable_clkout_dp(dev, true, true);
	else
		lpt_disable_clkout_dp(dev);
}

/*
 * Initialize reference clocks when the driver loads
 */
void intel_init_pch_refclk(struct drm_device *dev)
{
	if (HAS_PCH_IBX(dev) || HAS_PCH_CPT(dev))
		ironlake_init_pch_refclk(dev);
	else if (HAS_PCH_LPT(dev))
		lpt_init_pch_refclk(dev);
}

static int ironlake_get_refclk(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_encoder *encoder;
	int num_connectors = 0;
	bool is_lvds = false;

	for_each_encoder_on_crtc(dev, crtc, encoder) {
		switch (encoder->type) {
		case INTEL_OUTPUT_LVDS:
			is_lvds = true;
			break;
		}
		num_connectors++;
	}

	if (is_lvds && intel_panel_use_ssc(dev_priv) && num_connectors < 2) {
		DRM_DEBUG_KMS("using SSC reference clock of %d MHz\n",
			      dev_priv->vbt.lvds_ssc_freq);
		return dev_priv->vbt.lvds_ssc_freq * 1000;
	}

	return 120000;
}

static void ironlake_set_pipeconf(struct drm_crtc *crtc)
{
	struct drm_i915_private *dev_priv = crtc->dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	uint32_t val;

	val = 0;

	switch (intel_crtc->config.pipe_bpp) {
	case 18:
		val |= PIPECONF_6BPC;
		break;
	case 24:
		val |= PIPECONF_8BPC;
		break;
	case 30:
		val |= PIPECONF_10BPC;
		break;
	case 36:
		val |= PIPECONF_12BPC;
		break;
	default:
		/* Case prevented by intel_choose_pipe_bpp_dither. */
		BUG();
	}

	if (intel_crtc->config.dither)
		val |= (PIPECONF_DITHER_EN | PIPECONF_DITHER_TYPE_SP);

	if (intel_crtc->config.adjusted_mode.flags & DRM_MODE_FLAG_INTERLACE)
		val |= PIPECONF_INTERLACED_ILK;
	else
		val |= PIPECONF_PROGRESSIVE;

	if (intel_crtc->config.limited_color_range)
		val |= PIPECONF_COLOR_RANGE_SELECT;

	I915_WRITE(PIPECONF(pipe), val);
	POSTING_READ(PIPECONF(pipe));
}

/*
 * Set up the pipe CSC unit.
 *
 * Currently only full range RGB to limited range RGB conversion
 * is supported, but eventually this should handle various
 * RGB<->YCbCr scenarios as well.
 */
static void intel_set_pipe_csc(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	uint16_t coeff = 0x7800; /* 1.0 */

	/*
	 * TODO: Check what kind of values actually come out of the pipe
	 * with these coeff/postoff values and adjust to get the best
	 * accuracy. Perhaps we even need to take the bpc value into
	 * consideration.
	 */

	if (intel_crtc->config.limited_color_range)
		coeff = ((235 - 16) * (1 << 12) / 255) & 0xff8; /* 0.xxx... */

	/*
	 * GY/GU and RY/RU should be the other way around according
	 * to BSpec, but reality doesn't agree. Just set them up in
	 * a way that results in the correct picture.
	 */
	I915_WRITE(PIPE_CSC_COEFF_RY_GY(pipe), coeff << 16);
	I915_WRITE(PIPE_CSC_COEFF_BY(pipe), 0);

	I915_WRITE(PIPE_CSC_COEFF_RU_GU(pipe), coeff);
	I915_WRITE(PIPE_CSC_COEFF_BU(pipe), 0);

	I915_WRITE(PIPE_CSC_COEFF_RV_GV(pipe), 0);
	I915_WRITE(PIPE_CSC_COEFF_BV(pipe), coeff << 16);

	I915_WRITE(PIPE_CSC_PREOFF_HI(pipe), 0);
	I915_WRITE(PIPE_CSC_PREOFF_ME(pipe), 0);
	I915_WRITE(PIPE_CSC_PREOFF_LO(pipe), 0);

	if (INTEL_INFO(dev)->gen > 6) {
		uint16_t postoff = 0;

		if (intel_crtc->config.limited_color_range)
			postoff = (16 * (1 << 12) / 255) & 0x1fff;

		I915_WRITE(PIPE_CSC_POSTOFF_HI(pipe), postoff);
		I915_WRITE(PIPE_CSC_POSTOFF_ME(pipe), postoff);
		I915_WRITE(PIPE_CSC_POSTOFF_LO(pipe), postoff);

		I915_WRITE(PIPE_CSC_MODE(pipe), 0);
	} else {
		uint32_t mode = CSC_MODE_YUV_TO_RGB;

		if (intel_crtc->config.limited_color_range)
			mode |= CSC_BLACK_SCREEN_OFFSET;

		I915_WRITE(PIPE_CSC_MODE(pipe), mode);
	}
}

static void haswell_set_pipeconf(struct drm_crtc *crtc)
>>>>>>> refs/remotes/origin/master
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
<<<<<<< HEAD
	int pipe = intel_crtc->pipe;
	u32 fp, fp2 = 0;

	if (IS_PINEVIEW(dev)) {
		fp = (1 << clock->n) << 16 | clock->m1 << 8 | clock->m2;
		if (reduced_clock)
			fp2 = (1 << reduced_clock->n) << 16 |
				reduced_clock->m1 << 8 | reduced_clock->m2;
	} else {
		fp = clock->n << 16 | clock->m1 << 8 | clock->m2;
		if (reduced_clock)
			fp2 = reduced_clock->n << 16 | reduced_clock->m1 << 8 |
				reduced_clock->m2;
	}

	I915_WRITE(FP0(pipe), fp);

	intel_crtc->lowfreq_avail = false;
	if (intel_pipe_has_type(crtc, INTEL_OUTPUT_LVDS) &&
	    reduced_clock && i915_powersave) {
		I915_WRITE(FP1(pipe), fp2);
		intel_crtc->lowfreq_avail = true;
	} else {
		I915_WRITE(FP1(pipe), fp);
	}
}

>>>>>>> refs/remotes/origin/cm-10.0
static int i9xx_crtc_mode_set(struct drm_crtc *crtc,
			      struct drm_display_mode *mode,
			      struct drm_display_mode *adjusted_mode,
			      int x, int y,
			      struct drm_framebuffer *old_fb)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;
	int refclk, num_connectors = 0;
	intel_clock_t clock, reduced_clock;
<<<<<<< HEAD
	u32 dpll, fp = 0, fp2 = 0, dspcntr, pipeconf;
=======
	u32 dpll, dspcntr, pipeconf, vsyncshift;
>>>>>>> refs/remotes/origin/cm-10.0
	bool ok, has_reduced_clock = false, is_sdvo = false, is_dvo = false;
	bool is_crt = false, is_lvds = false, is_tv = false, is_dp = false;
	struct drm_mode_config *mode_config = &dev->mode_config;
	struct intel_encoder *encoder;
	const intel_limit_t *limit;
	int ret;
	u32 temp;
	u32 lvds_sync = 0;

	list_for_each_entry(encoder, &mode_config->encoder_list, base.head) {
		if (encoder->base.crtc != crtc)
			continue;

		switch (encoder->type) {
		case INTEL_OUTPUT_LVDS:
			is_lvds = true;
			break;
		case INTEL_OUTPUT_SDVO:
		case INTEL_OUTPUT_HDMI:
			is_sdvo = true;
			if (encoder->needs_tv_clock)
				is_tv = true;
			break;
		case INTEL_OUTPUT_DVO:
			is_dvo = true;
			break;
		case INTEL_OUTPUT_TVOUT:
			is_tv = true;
			break;
		case INTEL_OUTPUT_ANALOG:
			is_crt = true;
			break;
		case INTEL_OUTPUT_DISPLAYPORT:
			is_dp = true;
			break;
		}

		num_connectors++;
	}

<<<<<<< HEAD
	if (is_lvds && intel_panel_use_ssc(dev_priv) && num_connectors < 2) {
		refclk = dev_priv->lvds_ssc_freq * 1000;
		DRM_DEBUG_KMS("using SSC reference clock of %d MHz\n",
			      refclk / 1000);
	} else if (!IS_GEN2(dev)) {
		refclk = 96000;
	} else {
		refclk = 48000;
	}
=======
	refclk = i9xx_get_refclk(crtc, num_connectors);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	enum pipe pipe = intel_crtc->pipe;
	enum transcoder cpu_transcoder = intel_crtc->config.cpu_transcoder;
	uint32_t val;

	val = 0;

	if (IS_HASWELL(dev) && intel_crtc->config.dither)
		val |= (PIPECONF_DITHER_EN | PIPECONF_DITHER_TYPE_SP);

	if (intel_crtc->config.adjusted_mode.flags & DRM_MODE_FLAG_INTERLACE)
		val |= PIPECONF_INTERLACED_ILK;
	else
		val |= PIPECONF_PROGRESSIVE;

	I915_WRITE(PIPECONF(cpu_transcoder), val);
	POSTING_READ(PIPECONF(cpu_transcoder));

	I915_WRITE(GAMMA_MODE(intel_crtc->pipe), GAMMA_MODE_MODE_8BIT);
	POSTING_READ(GAMMA_MODE(intel_crtc->pipe));

	if (IS_BROADWELL(dev)) {
		val = 0;

		switch (intel_crtc->config.pipe_bpp) {
		case 18:
			val |= PIPEMISC_DITHER_6_BPC;
			break;
		case 24:
			val |= PIPEMISC_DITHER_8_BPC;
			break;
		case 30:
			val |= PIPEMISC_DITHER_10_BPC;
			break;
		case 36:
			val |= PIPEMISC_DITHER_12_BPC;
			break;
		default:
			/* Case prevented by pipe_config_set_bpp. */
			BUG();
		}

		if (intel_crtc->config.dither)
			val |= PIPEMISC_DITHER_ENABLE | PIPEMISC_DITHER_TYPE_SP;

		I915_WRITE(PIPEMISC(pipe), val);
	}
}

static bool ironlake_compute_clocks(struct drm_crtc *crtc,
				    intel_clock_t *clock,
				    bool *has_reduced_clock,
				    intel_clock_t *reduced_clock)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_encoder *intel_encoder;
	int refclk;
	const intel_limit_t *limit;
	bool ret, is_lvds = false;

	for_each_encoder_on_crtc(dev, crtc, intel_encoder) {
		switch (intel_encoder->type) {
		case INTEL_OUTPUT_LVDS:
			is_lvds = true;
			break;
		}
	}

	refclk = ironlake_get_refclk(crtc);
>>>>>>> refs/remotes/origin/master

	/*
	 * Returns a set of divisors for the desired target clock with the given
	 * refclk, or FALSE.  The returned values represent the clock equation:
	 * reflck * (5 * (m1 + 2) + (m2 + 2)) / (n + 2) / p1 / p2.
	 */
	limit = intel_limit(crtc, refclk);
<<<<<<< HEAD
<<<<<<< HEAD
	ok = limit->find_pll(limit, crtc, adjusted_mode->clock, refclk, &clock);
=======
	ok = limit->find_pll(limit, crtc, adjusted_mode->clock, refclk, NULL,
			     &clock);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!ok) {
		DRM_ERROR("Couldn't find PLL settings for mode!\n");
		return -EINVAL;
	}

	/* Ensure that the cursor is valid for the new mode before changing... */
	intel_crtc_update_cursor(crtc, true);

	if (is_lvds && dev_priv->lvds_downclock_avail) {
<<<<<<< HEAD
		has_reduced_clock = limit->find_pll(limit, crtc,
						    dev_priv->lvds_downclock,
						    refclk,
						    &reduced_clock);
		if (has_reduced_clock && (clock.p != reduced_clock.p)) {
			/*
			 * If the different P is found, it means that we can't
			 * switch the display clock by using the FP0/FP1.
			 * In such case we will disable the LVDS downclock
			 * feature.
			 */
			DRM_DEBUG_KMS("Different P is found for "
				      "LVDS clock/downclock\n");
			has_reduced_clock = 0;
		}
	}
	/* SDVO TV has fixed PLL values depend on its clock range,
	   this mirrors vbios setting. */
	if (is_sdvo && is_tv) {
		if (adjusted_mode->clock >= 100000
		    && adjusted_mode->clock < 140500) {
			clock.p1 = 2;
			clock.p2 = 10;
			clock.n = 3;
			clock.m1 = 16;
			clock.m2 = 8;
		} else if (adjusted_mode->clock >= 140500
			   && adjusted_mode->clock <= 200000) {
			clock.p1 = 1;
			clock.p2 = 10;
			clock.n = 6;
			clock.m1 = 12;
			clock.m2 = 8;
		}
	}

	if (IS_PINEVIEW(dev)) {
		fp = (1 << clock.n) << 16 | clock.m1 << 8 | clock.m2;
		if (has_reduced_clock)
			fp2 = (1 << reduced_clock.n) << 16 |
				reduced_clock.m1 << 8 | reduced_clock.m2;
	} else {
		fp = clock.n << 16 | clock.m1 << 8 | clock.m2;
		if (has_reduced_clock)
			fp2 = reduced_clock.n << 16 | reduced_clock.m1 << 8 |
				reduced_clock.m2;
	}
=======
=======
	ret = dev_priv->display.find_dpll(limit, crtc,
					  to_intel_crtc(crtc)->config.port_clock,
					  refclk, NULL, clock);
	if (!ret)
		return false;

	if (is_lvds && dev_priv->lvds_downclock_avail) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Ensure we match the reduced clock's P to the target clock.
		 * If the clocks don't match, we can't switch the display clock
		 * by using the FP0/FP1. In such case we will disable the LVDS
		 * downclock feature.
		*/
<<<<<<< HEAD
		has_reduced_clock = limit->find_pll(limit, crtc,
						    dev_priv->lvds_downclock,
						    refclk,
						    &clock,
						    &reduced_clock);
	}

	if (is_sdvo && is_tv)
		i9xx_adjust_sdvo_tv_clock(adjusted_mode, &clock);

	i9xx_update_pll_dividers(crtc, &clock, has_reduced_clock ?
				 &reduced_clock : NULL);
>>>>>>> refs/remotes/origin/cm-10.0

	dpll = DPLL_VGA_MODE_DIS;

	if (!IS_GEN2(dev)) {
		if (is_lvds)
			dpll |= DPLLB_MODE_LVDS;
		else
			dpll |= DPLLB_MODE_DAC_SERIAL;
		if (is_sdvo) {
			int pixel_multiplier = intel_mode_get_pixel_multiplier(adjusted_mode);
			if (pixel_multiplier > 1) {
				if (IS_I945G(dev) || IS_I945GM(dev) || IS_G33(dev))
					dpll |= (pixel_multiplier - 1) << SDVO_MULTIPLIER_SHIFT_HIRES;
			}
			dpll |= DPLL_DVO_HIGH_SPEED;
		}
		if (is_dp)
			dpll |= DPLL_DVO_HIGH_SPEED;

		/* compute bitmask from p1 value */
		if (IS_PINEVIEW(dev))
			dpll |= (1 << (clock.p1 - 1)) << DPLL_FPA01_P1_POST_DIV_SHIFT_PINEVIEW;
		else {
			dpll |= (1 << (clock.p1 - 1)) << DPLL_FPA01_P1_POST_DIV_SHIFT;
			if (IS_G4X(dev) && has_reduced_clock)
				dpll |= (1 << (reduced_clock.p1 - 1)) << DPLL_FPA1_P1_POST_DIV_SHIFT;
		}
		switch (clock.p2) {
		case 5:
			dpll |= DPLL_DAC_SERIAL_P2_CLOCK_DIV_5;
			break;
		case 7:
			dpll |= DPLLB_LVDS_P2_CLOCK_DIV_7;
			break;
		case 10:
			dpll |= DPLL_DAC_SERIAL_P2_CLOCK_DIV_10;
			break;
		case 14:
			dpll |= DPLLB_LVDS_P2_CLOCK_DIV_14;
			break;
		}
		if (INTEL_INFO(dev)->gen >= 4)
			dpll |= (6 << PLL_LOAD_PULSE_PHASE_SHIFT);
	} else {
		if (is_lvds) {
			dpll |= (1 << (clock.p1 - 1)) << DPLL_FPA01_P1_POST_DIV_SHIFT;
		} else {
			if (clock.p1 == 2)
				dpll |= PLL_P1_DIVIDE_BY_TWO;
			else
				dpll |= (clock.p1 - 2) << DPLL_FPA01_P1_POST_DIV_SHIFT;
			if (clock.p2 == 4)
				dpll |= PLL_P2_DIVIDE_BY_4;
		}
	}

	if (is_sdvo && is_tv)
		dpll |= PLL_REF_INPUT_TVCLKINBC;
	else if (is_tv)
		/* XXX: just matching BIOS for now */
		/*	dpll |= PLL_REF_INPUT_TVCLKINBC; */
		dpll |= 3;
	else if (is_lvds && intel_panel_use_ssc(dev_priv) && num_connectors < 2)
		dpll |= PLLB_REF_INPUT_SPREADSPECTRUMIN;
	else
		dpll |= PLL_REF_INPUT_DREFCLK;

	/* setup pipeconf */
	pipeconf = I915_READ(PIPECONF(pipe));

	/* Set up the display plane register */
	dspcntr = DISPPLANE_GAMMA_ENABLE;

<<<<<<< HEAD
	/* Ironlake's plane is forced to pipe, bit 24 is to
	   enable color space conversion */
=======
>>>>>>> refs/remotes/origin/cm-10.0
	if (pipe == 0)
		dspcntr &= ~DISPPLANE_SEL_PIPE_MASK;
	else
		dspcntr |= DISPPLANE_SEL_PIPE_B;

	if (pipe == 0 && INTEL_INFO(dev)->gen < 4) {
		/* Enable pixel doubling when the dot clock is > 90% of the (display)
		 * core speed.
		 *
		 * XXX: No double-wide on 915GM pipe B. Is that the only reason for the
		 * pipe == 0 check?
		 */
		if (mode->clock >
		    dev_priv->display.get_display_clock_speed(dev) * 9 / 10)
			pipeconf |= PIPECONF_DOUBLE_WIDE;
		else
			pipeconf &= ~PIPECONF_DOUBLE_WIDE;
	}

<<<<<<< HEAD
=======
	/* default to 8bpc */
	pipeconf &= ~(PIPECONF_BPP_MASK | PIPECONF_DITHER_EN);
	if (is_dp) {
		if (adjusted_mode->private_flags & INTEL_MODE_DP_FORCE_6BPC) {
			pipeconf |= PIPECONF_BPP_6 |
				    PIPECONF_DITHER_EN |
				    PIPECONF_DITHER_TYPE_SP;
		}
	}

>>>>>>> refs/remotes/origin/cm-10.0
	dpll |= DPLL_VCO_ENABLE;

	DRM_DEBUG_KMS("Mode for pipe %c:\n", pipe == 0 ? 'A' : 'B');
	drm_mode_debug_printmodeline(mode);

<<<<<<< HEAD
	I915_WRITE(FP0(pipe), fp);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	I915_WRITE(DPLL(pipe), dpll & ~DPLL_VCO_ENABLE);

	POSTING_READ(DPLL(pipe));
	udelay(150);

	/* The LVDS pin pair needs to be on before the DPLLs are enabled.
	 * This is an exception to the general rule that mode_set doesn't turn
	 * things on.
	 */
	if (is_lvds) {
		temp = I915_READ(LVDS);
		temp |= LVDS_PORT_EN | LVDS_A0A2_CLKA_POWER_UP;
		if (pipe == 1) {
			temp |= LVDS_PIPEB_SELECT;
		} else {
			temp &= ~LVDS_PIPEB_SELECT;
		}
		/* set the corresponsding LVDS_BORDER bit */
		temp |= dev_priv->lvds_border_bits;
		/* Set the B0-B3 data pairs corresponding to whether we're going to
		 * set the DPLLs for dual-channel mode or not.
		 */
		if (clock.p2 == 7)
			temp |= LVDS_B0B3_POWER_UP | LVDS_CLKB_POWER_UP;
		else
			temp &= ~(LVDS_B0B3_POWER_UP | LVDS_CLKB_POWER_UP);

		/* It would be nice to set 24 vs 18-bit mode (LVDS_A3_POWER_UP)
		 * appropriately here, but we need to look more thoroughly into how
		 * panels behave in the two modes.
		 */
		/* set the dithering flag on LVDS as needed */
		if (INTEL_INFO(dev)->gen >= 4) {
			if (dev_priv->lvds_dither)
				temp |= LVDS_ENABLE_DITHER;
			else
				temp &= ~LVDS_ENABLE_DITHER;
		}
		if (adjusted_mode->flags & DRM_MODE_FLAG_NHSYNC)
			lvds_sync |= LVDS_HSYNC_POLARITY;
		if (adjusted_mode->flags & DRM_MODE_FLAG_NVSYNC)
			lvds_sync |= LVDS_VSYNC_POLARITY;
		if ((temp & (LVDS_HSYNC_POLARITY | LVDS_VSYNC_POLARITY))
		    != lvds_sync) {
			char flags[2] = "-+";
			DRM_INFO("Changing LVDS panel from "
				 "(%chsync, %cvsync) to (%chsync, %cvsync)\n",
				 flags[!(temp & LVDS_HSYNC_POLARITY)],
				 flags[!(temp & LVDS_VSYNC_POLARITY)],
				 flags[!(lvds_sync & LVDS_HSYNC_POLARITY)],
				 flags[!(lvds_sync & LVDS_VSYNC_POLARITY)]);
			temp &= ~(LVDS_HSYNC_POLARITY | LVDS_VSYNC_POLARITY);
			temp |= lvds_sync;
		}
		I915_WRITE(LVDS, temp);
	}

	if (is_dp) {
		intel_dp_set_m_n(crtc, mode, adjusted_mode);
	}

	I915_WRITE(DPLL(pipe), dpll);

	/* Wait for the clocks to stabilize. */
	POSTING_READ(DPLL(pipe));
	udelay(150);

	if (INTEL_INFO(dev)->gen >= 4) {
		temp = 0;
		if (is_sdvo) {
			temp = intel_mode_get_pixel_multiplier(adjusted_mode);
			if (temp > 1)
				temp = (temp - 1) << DPLL_MD_UDI_MULTIPLIER_SHIFT;
			else
				temp = 0;
		}
		I915_WRITE(DPLL_MD(pipe), temp);
	} else {
		/* The pixel multiplier can only be updated once the
		 * DPLL is enabled and the clocks are stable.
		 *
		 * So write it again.
		 */
		I915_WRITE(DPLL(pipe), dpll);
	}

<<<<<<< HEAD
	intel_crtc->lowfreq_avail = false;
	if (is_lvds && has_reduced_clock && i915_powersave) {
		I915_WRITE(FP1(pipe), fp2);
		intel_crtc->lowfreq_avail = true;
		if (HAS_PIPE_CXSR(dev)) {
			DRM_DEBUG_KMS("enabling CxSR downclocking\n");
			pipeconf |= PIPECONF_CXSR_DOWNCLOCK;
		}
	} else {
		I915_WRITE(FP1(pipe), fp);
		if (HAS_PIPE_CXSR(dev)) {
=======
	if (HAS_PIPE_CXSR(dev)) {
		if (intel_crtc->lowfreq_avail) {
			DRM_DEBUG_KMS("enabling CxSR downclocking\n");
			pipeconf |= PIPECONF_CXSR_DOWNCLOCK;
		} else {
>>>>>>> refs/remotes/origin/cm-10.0
			DRM_DEBUG_KMS("disabling CxSR downclocking\n");
			pipeconf &= ~PIPECONF_CXSR_DOWNCLOCK;
		}
	}

<<<<<<< HEAD
	if (adjusted_mode->flags & DRM_MODE_FLAG_INTERLACE) {
		pipeconf |= PIPECONF_INTERLACE_W_FIELD_INDICATION;
		/* the chip adds 2 halflines automatically */
		adjusted_mode->crtc_vdisplay -= 1;
		adjusted_mode->crtc_vtotal -= 1;
		adjusted_mode->crtc_vblank_start -= 1;
		adjusted_mode->crtc_vblank_end -= 1;
		adjusted_mode->crtc_vsync_end -= 1;
		adjusted_mode->crtc_vsync_start -= 1;
	} else
		pipeconf &= ~PIPECONF_INTERLACE_W_FIELD_INDICATION; /* progressive */
=======
	pipeconf &= ~PIPECONF_INTERLACE_MASK;
	if (!IS_GEN2(dev) &&
	    adjusted_mode->flags & DRM_MODE_FLAG_INTERLACE) {
		pipeconf |= PIPECONF_INTERLACE_W_FIELD_INDICATION;
		/* the chip adds 2 halflines automatically */
		adjusted_mode->crtc_vtotal -= 1;
		adjusted_mode->crtc_vblank_end -= 1;
		vsyncshift = adjusted_mode->crtc_hsync_start
			     - adjusted_mode->crtc_htotal/2;
	} else {
		pipeconf |= PIPECONF_PROGRESSIVE;
		vsyncshift = 0;
	}

	if (!IS_GEN3(dev))
		I915_WRITE(VSYNCSHIFT(pipe), vsyncshift);
>>>>>>> refs/remotes/origin/cm-10.0

	I915_WRITE(HTOTAL(pipe),
		   (adjusted_mode->crtc_hdisplay - 1) |
		   ((adjusted_mode->crtc_htotal - 1) << 16));
	I915_WRITE(HBLANK(pipe),
		   (adjusted_mode->crtc_hblank_start - 1) |
		   ((adjusted_mode->crtc_hblank_end - 1) << 16));
	I915_WRITE(HSYNC(pipe),
		   (adjusted_mode->crtc_hsync_start - 1) |
		   ((adjusted_mode->crtc_hsync_end - 1) << 16));

	I915_WRITE(VTOTAL(pipe),
		   (adjusted_mode->crtc_vdisplay - 1) |
		   ((adjusted_mode->crtc_vtotal - 1) << 16));
	I915_WRITE(VBLANK(pipe),
		   (adjusted_mode->crtc_vblank_start - 1) |
		   ((adjusted_mode->crtc_vblank_end - 1) << 16));
	I915_WRITE(VSYNC(pipe),
		   (adjusted_mode->crtc_vsync_start - 1) |
		   ((adjusted_mode->crtc_vsync_end - 1) << 16));

	/* pipesrc and dspsize control the size that is scaled from,
	 * which should always be the user's requested size.
	 */
	I915_WRITE(DSPSIZE(plane),
		   ((mode->vdisplay - 1) << 16) |
		   (mode->hdisplay - 1));
	I915_WRITE(DSPPOS(plane), 0);
	I915_WRITE(PIPESRC(pipe),
		   ((mode->hdisplay - 1) << 16) | (mode->vdisplay - 1));

	I915_WRITE(PIPECONF(pipe), pipeconf);
	POSTING_READ(PIPECONF(pipe));
	intel_enable_pipe(dev_priv, pipe, false);

	intel_wait_for_vblank(dev, pipe);

	I915_WRITE(DSPCNTR(plane), dspcntr);
	POSTING_READ(DSPCNTR(plane));
	intel_enable_plane(dev_priv, plane, pipe);

	ret = intel_pipe_set_base(crtc, x, y, old_fb);

	intel_update_watermarks(dev);

	return ret;
}

<<<<<<< HEAD
=======
/*
 * Initialize reference clocks when the driver loads
 */
void ironlake_init_pch_refclk(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_mode_config *mode_config = &dev->mode_config;
	struct intel_encoder *encoder;
	u32 temp;
	bool has_lvds = false;
	bool has_cpu_edp = false;
	bool has_pch_edp = false;
	bool has_panel = false;
	bool has_ck505 = false;
	bool can_ssc = false;

	/* We need to take the global config into account */
	list_for_each_entry(encoder, &mode_config->encoder_list,
			    base.head) {
		switch (encoder->type) {
		case INTEL_OUTPUT_LVDS:
			has_panel = true;
			has_lvds = true;
			break;
		case INTEL_OUTPUT_EDP:
			has_panel = true;
			if (intel_encoder_is_pch_edp(&encoder->base))
				has_pch_edp = true;
			else
				has_cpu_edp = true;
			break;
		}
	}

	if (HAS_PCH_IBX(dev)) {
		has_ck505 = dev_priv->display_clock_mode;
		can_ssc = has_ck505;
	} else {
		has_ck505 = false;
		can_ssc = true;
	}

	DRM_DEBUG_KMS("has_panel %d has_lvds %d has_pch_edp %d has_cpu_edp %d has_ck505 %d\n",
		      has_panel, has_lvds, has_pch_edp, has_cpu_edp,
		      has_ck505);

	/* Ironlake: try to setup display ref clock before DPLL
	 * enabling. This is only under driver's control after
	 * PCH B stepping, previous chipset stepping should be
	 * ignoring this setting.
	 */
	temp = I915_READ(PCH_DREF_CONTROL);
	/* Always enable nonspread source */
	temp &= ~DREF_NONSPREAD_SOURCE_MASK;

	if (has_ck505)
		temp |= DREF_NONSPREAD_CK505_ENABLE;
	else
		temp |= DREF_NONSPREAD_SOURCE_ENABLE;

	if (has_panel) {
		temp &= ~DREF_SSC_SOURCE_MASK;
		temp |= DREF_SSC_SOURCE_ENABLE;

		/* SSC must be turned on before enabling the CPU output  */
		if (intel_panel_use_ssc(dev_priv) && can_ssc) {
			DRM_DEBUG_KMS("Using SSC on panel\n");
			temp |= DREF_SSC1_ENABLE;
		} else
			temp &= ~DREF_SSC1_ENABLE;

		/* Get SSC going before enabling the outputs */
		I915_WRITE(PCH_DREF_CONTROL, temp);
		POSTING_READ(PCH_DREF_CONTROL);
		udelay(200);

		temp &= ~DREF_CPU_SOURCE_OUTPUT_MASK;

		/* Enable CPU source on CPU attached eDP */
		if (has_cpu_edp) {
			if (intel_panel_use_ssc(dev_priv) && can_ssc) {
				DRM_DEBUG_KMS("Using SSC on eDP\n");
				temp |= DREF_CPU_SOURCE_OUTPUT_DOWNSPREAD;
			}
			else
				temp |= DREF_CPU_SOURCE_OUTPUT_NONSPREAD;
		} else
			temp |= DREF_CPU_SOURCE_OUTPUT_DISABLE;

		I915_WRITE(PCH_DREF_CONTROL, temp);
		POSTING_READ(PCH_DREF_CONTROL);
		udelay(200);
	} else {
		DRM_DEBUG_KMS("Disabling SSC entirely\n");

		temp &= ~DREF_CPU_SOURCE_OUTPUT_MASK;

		/* Turn off CPU output */
		temp |= DREF_CPU_SOURCE_OUTPUT_DISABLE;

		I915_WRITE(PCH_DREF_CONTROL, temp);
		POSTING_READ(PCH_DREF_CONTROL);
		udelay(200);

		/* Turn off the SSC source */
		temp &= ~DREF_SSC_SOURCE_MASK;
		temp |= DREF_SSC_SOURCE_DISABLE;

		/* Turn off SSC1 */
		temp &= ~ DREF_SSC1_ENABLE;

		I915_WRITE(PCH_DREF_CONTROL, temp);
		POSTING_READ(PCH_DREF_CONTROL);
		udelay(200);
	}
}

static int ironlake_get_refclk(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_encoder *encoder;
	struct drm_mode_config *mode_config = &dev->mode_config;
	struct intel_encoder *edp_encoder = NULL;
	int num_connectors = 0;
	bool is_lvds = false;

	list_for_each_entry(encoder, &mode_config->encoder_list, base.head) {
		if (encoder->base.crtc != crtc)
			continue;

		switch (encoder->type) {
		case INTEL_OUTPUT_LVDS:
			is_lvds = true;
			break;
		case INTEL_OUTPUT_EDP:
			edp_encoder = encoder;
			break;
		}
		num_connectors++;
	}

	if (is_lvds && intel_panel_use_ssc(dev_priv) && num_connectors < 2) {
		DRM_DEBUG_KMS("using SSC reference clock of %d MHz\n",
			      dev_priv->lvds_ssc_freq);
		return dev_priv->lvds_ssc_freq * 1000;
	}

	return 120000;
}

>>>>>>> refs/remotes/origin/cm-10.0
static int ironlake_crtc_mode_set(struct drm_crtc *crtc,
				  struct drm_display_mode *mode,
				  struct drm_display_mode *adjusted_mode,
				  int x, int y,
				  struct drm_framebuffer *old_fb)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;
	int refclk, num_connectors = 0;
	intel_clock_t clock, reduced_clock;
	u32 dpll, fp = 0, fp2 = 0, dspcntr, pipeconf;
	bool ok, has_reduced_clock = false, is_sdvo = false;
	bool is_crt = false, is_lvds = false, is_tv = false, is_dp = false;
	struct intel_encoder *has_edp_encoder = NULL;
	struct drm_mode_config *mode_config = &dev->mode_config;
	struct intel_encoder *encoder;
	const intel_limit_t *limit;
	int ret;
	struct fdi_m_n m_n = {0};
	u32 temp;
	u32 lvds_sync = 0;
<<<<<<< HEAD
	int target_clock, pixel_multiplier, lane, link_bw, bpp, factor;
=======
	int target_clock, pixel_multiplier, lane, link_bw, factor;
	unsigned int pipe_bpp;
	bool dither;
>>>>>>> refs/remotes/origin/cm-10.0

	list_for_each_entry(encoder, &mode_config->encoder_list, base.head) {
		if (encoder->base.crtc != crtc)
			continue;

		switch (encoder->type) {
		case INTEL_OUTPUT_LVDS:
			is_lvds = true;
			break;
		case INTEL_OUTPUT_SDVO:
		case INTEL_OUTPUT_HDMI:
			is_sdvo = true;
			if (encoder->needs_tv_clock)
				is_tv = true;
			break;
		case INTEL_OUTPUT_TVOUT:
			is_tv = true;
			break;
		case INTEL_OUTPUT_ANALOG:
			is_crt = true;
			break;
		case INTEL_OUTPUT_DISPLAYPORT:
			is_dp = true;
			break;
		case INTEL_OUTPUT_EDP:
			has_edp_encoder = encoder;
			break;
		}

		num_connectors++;
	}

<<<<<<< HEAD
	if (is_lvds && intel_panel_use_ssc(dev_priv) && num_connectors < 2) {
		refclk = dev_priv->lvds_ssc_freq * 1000;
		DRM_DEBUG_KMS("using SSC reference clock of %d MHz\n",
			      refclk / 1000);
	} else {
		refclk = 96000;
		if (!has_edp_encoder ||
		    intel_encoder_is_pch_edp(&has_edp_encoder->base))
			refclk = 120000; /* 120Mhz refclk */
	}
=======
	refclk = ironlake_get_refclk(crtc);
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * Returns a set of divisors for the desired target clock with the given
	 * refclk, or FALSE.  The returned values represent the clock equation:
	 * reflck * (5 * (m1 + 2) + (m2 + 2)) / (n + 2) / p1 / p2.
	 */
	limit = intel_limit(crtc, refclk);
<<<<<<< HEAD
	ok = limit->find_pll(limit, crtc, adjusted_mode->clock, refclk, &clock);
=======
	ok = limit->find_pll(limit, crtc, adjusted_mode->clock, refclk, NULL,
			     &clock);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!ok) {
		DRM_ERROR("Couldn't find PLL settings for mode!\n");
		return -EINVAL;
	}

	/* Ensure that the cursor is valid for the new mode before changing... */
	intel_crtc_update_cursor(crtc, true);

	if (is_lvds && dev_priv->lvds_downclock_avail) {
<<<<<<< HEAD
		has_reduced_clock = limit->find_pll(limit, crtc,
						    dev_priv->lvds_downclock,
						    refclk,
						    &reduced_clock);
		if (has_reduced_clock && (clock.p != reduced_clock.p)) {
			/*
			 * If the different P is found, it means that we can't
			 * switch the display clock by using the FP0/FP1.
			 * In such case we will disable the LVDS downclock
			 * feature.
			 */
			DRM_DEBUG_KMS("Different P is found for "
				      "LVDS clock/downclock\n");
			has_reduced_clock = 0;
		}
=======
		/*
		 * Ensure we match the reduced clock's P to the target clock.
		 * If the clocks don't match, we can't switch the display clock
		 * by using the FP0/FP1. In such case we will disable the LVDS
		 * downclock feature.
		*/
		has_reduced_clock = limit->find_pll(limit, crtc,
						    dev_priv->lvds_downclock,
						    refclk,
						    &clock,
						    &reduced_clock);
>>>>>>> refs/remotes/origin/cm-10.0
	}
	/* SDVO TV has fixed PLL values depend on its clock range,
	   this mirrors vbios setting. */
	if (is_sdvo && is_tv) {
		if (adjusted_mode->clock >= 100000
		    && adjusted_mode->clock < 140500) {
			clock.p1 = 2;
			clock.p2 = 10;
			clock.n = 3;
			clock.m1 = 16;
			clock.m2 = 8;
		} else if (adjusted_mode->clock >= 140500
			   && adjusted_mode->clock <= 200000) {
			clock.p1 = 1;
			clock.p2 = 10;
			clock.n = 6;
			clock.m1 = 12;
			clock.m2 = 8;
		}
	}

	/* FDI link */
	pixel_multiplier = intel_mode_get_pixel_multiplier(adjusted_mode);
	lane = 0;
	/* CPU eDP doesn't require FDI link, so just set DP M/N
	   according to current link config */
	if (has_edp_encoder &&
	    !intel_encoder_is_pch_edp(&has_edp_encoder->base)) {
		target_clock = mode->clock;
		intel_edp_link_config(has_edp_encoder,
				      &lane, &link_bw);
	} else {
		/* [e]DP over FDI requires target mode clock
		   instead of link clock */
		if (is_dp || intel_encoder_is_pch_edp(&has_edp_encoder->base))
			target_clock = mode->clock;
		else
			target_clock = adjusted_mode->clock;

		/* FDI is a binary signal running at ~2.7GHz, encoding
		 * each output octet as 10 bits. The actual frequency
		 * is stored as a divider into a 100MHz clock, and the
		 * mode pixel clock is stored in units of 1KHz.
		 * Hence the bw of each lane in terms of the mode signal
		 * is:
		 */
		link_bw = intel_fdi_link_freq(dev) * MHz(100)/KHz(1)/10;
	}

	/* determine panel color depth */
	temp = I915_READ(PIPECONF(pipe));
	temp &= ~PIPE_BPC_MASK;
<<<<<<< HEAD
	if (is_lvds) {
		/* the BPC will be 6 if it is 18-bit LVDS panel */
		if ((I915_READ(PCH_LVDS) & LVDS_A3_POWER_MASK) == LVDS_A3_POWER_UP)
			temp |= PIPE_8BPC;
		else
			temp |= PIPE_6BPC;
	} else if (has_edp_encoder) {
		switch (dev_priv->edp.bpp/3) {
		case 8:
			temp |= PIPE_8BPC;
			break;
		case 10:
			temp |= PIPE_10BPC;
			break;
		case 6:
			temp |= PIPE_6BPC;
			break;
		case 12:
			temp |= PIPE_12BPC;
			break;
		}
	} else
		temp |= PIPE_8BPC;
	I915_WRITE(PIPECONF(pipe), temp);

	switch (temp & PIPE_BPC_MASK) {
	case PIPE_8BPC:
		bpp = 24;
		break;
	case PIPE_10BPC:
		bpp = 30;
		break;
	case PIPE_6BPC:
		bpp = 18;
		break;
	case PIPE_12BPC:
		bpp = 36;
		break;
	default:
		DRM_ERROR("unknown pipe bpc value\n");
		bpp = 24;
	}

=======
	dither = intel_choose_pipe_bpp_dither(crtc, &pipe_bpp, adjusted_mode);
	switch (pipe_bpp) {
	case 18:
		temp |= PIPE_6BPC;
		break;
	case 24:
		temp |= PIPE_8BPC;
		break;
	case 30:
		temp |= PIPE_10BPC;
		break;
	case 36:
		temp |= PIPE_12BPC;
		break;
	default:
		WARN(1, "intel_choose_pipe_bpp returned invalid value %d\n",
			pipe_bpp);
		temp |= PIPE_8BPC;
		pipe_bpp = 24;
		break;
	}

	intel_crtc->bpp = pipe_bpp;
	I915_WRITE(PIPECONF(pipe), temp);

>>>>>>> refs/remotes/origin/cm-10.0
	if (!lane) {
		/*
		 * Account for spread spectrum to avoid
		 * oversubscribing the link. Max center spread
		 * is 2.5%; use 5% for safety's sake.
		 */
<<<<<<< HEAD
		u32 bps = target_clock * bpp * 21 / 20;
=======
		u32 bps = target_clock * intel_crtc->bpp * 21 / 20;
>>>>>>> refs/remotes/origin/cm-10.0
		lane = bps / (link_bw * 8) + 1;
	}

	intel_crtc->fdi_lanes = lane;

	if (pixel_multiplier > 1)
		link_bw *= pixel_multiplier;
<<<<<<< HEAD
	ironlake_compute_m_n(bpp, lane, target_clock, link_bw, &m_n);

	/* Ironlake: try to setup display ref clock before DPLL
	 * enabling. This is only under driver's control after
	 * PCH B stepping, previous chipset stepping should be
	 * ignoring this setting.
	 */
	temp = I915_READ(PCH_DREF_CONTROL);
	/* Always enable nonspread source */
	temp &= ~DREF_NONSPREAD_SOURCE_MASK;
	temp |= DREF_NONSPREAD_SOURCE_ENABLE;
	temp &= ~DREF_SSC_SOURCE_MASK;
	temp |= DREF_SSC_SOURCE_ENABLE;
	I915_WRITE(PCH_DREF_CONTROL, temp);

	POSTING_READ(PCH_DREF_CONTROL);
	udelay(200);

	if (has_edp_encoder) {
		if (intel_panel_use_ssc(dev_priv)) {
			temp |= DREF_SSC1_ENABLE;
			I915_WRITE(PCH_DREF_CONTROL, temp);

			POSTING_READ(PCH_DREF_CONTROL);
			udelay(200);
		}
		temp &= ~DREF_CPU_SOURCE_OUTPUT_MASK;

		/* Enable CPU source on CPU attached eDP */
		if (!intel_encoder_is_pch_edp(&has_edp_encoder->base)) {
			if (intel_panel_use_ssc(dev_priv))
				temp |= DREF_CPU_SOURCE_OUTPUT_DOWNSPREAD;
			else
				temp |= DREF_CPU_SOURCE_OUTPUT_NONSPREAD;
		} else {
			/* Enable SSC on PCH eDP if needed */
			if (intel_panel_use_ssc(dev_priv)) {
				DRM_ERROR("enabling SSC on PCH\n");
				temp |= DREF_SUPERSPREAD_SOURCE_ENABLE;
			}
		}
		I915_WRITE(PCH_DREF_CONTROL, temp);
		POSTING_READ(PCH_DREF_CONTROL);
		udelay(200);
	}
=======
	ironlake_compute_m_n(intel_crtc->bpp, lane, target_clock, link_bw,
			     &m_n);
>>>>>>> refs/remotes/origin/cm-10.0

	fp = clock.n << 16 | clock.m1 << 8 | clock.m2;
	if (has_reduced_clock)
		fp2 = reduced_clock.n << 16 | reduced_clock.m1 << 8 |
			reduced_clock.m2;

	/* Enable autotuning of the PLL clock (if permissible) */
	factor = 21;
	if (is_lvds) {
		if ((intel_panel_use_ssc(dev_priv) &&
		     dev_priv->lvds_ssc_freq == 100) ||
		    (I915_READ(PCH_LVDS) & LVDS_CLKB_POWER_MASK) == LVDS_CLKB_POWER_UP)
			factor = 25;
	} else if (is_sdvo && is_tv)
		factor = 20;

	if (clock.m < factor * clock.n)
		fp |= FP_CB_TUNE;

	dpll = 0;

	if (is_lvds)
		dpll |= DPLLB_MODE_LVDS;
	else
		dpll |= DPLLB_MODE_DAC_SERIAL;
	if (is_sdvo) {
		int pixel_multiplier = intel_mode_get_pixel_multiplier(adjusted_mode);
		if (pixel_multiplier > 1) {
			dpll |= (pixel_multiplier - 1) << PLL_REF_SDVO_HDMI_MULTIPLIER_SHIFT;
		}
		dpll |= DPLL_DVO_HIGH_SPEED;
	}
	if (is_dp || intel_encoder_is_pch_edp(&has_edp_encoder->base))
		dpll |= DPLL_DVO_HIGH_SPEED;

	/* compute bitmask from p1 value */
	dpll |= (1 << (clock.p1 - 1)) << DPLL_FPA01_P1_POST_DIV_SHIFT;
	/* also FPA1 */
	dpll |= (1 << (clock.p1 - 1)) << DPLL_FPA1_P1_POST_DIV_SHIFT;

	switch (clock.p2) {
	case 5:
		dpll |= DPLL_DAC_SERIAL_P2_CLOCK_DIV_5;
		break;
	case 7:
		dpll |= DPLLB_LVDS_P2_CLOCK_DIV_7;
		break;
	case 10:
		dpll |= DPLL_DAC_SERIAL_P2_CLOCK_DIV_10;
		break;
	case 14:
		dpll |= DPLLB_LVDS_P2_CLOCK_DIV_14;
		break;
	}

	if (is_sdvo && is_tv)
		dpll |= PLL_REF_INPUT_TVCLKINBC;
	else if (is_tv)
		/* XXX: just matching BIOS for now */
		/*	dpll |= PLL_REF_INPUT_TVCLKINBC; */
		dpll |= 3;
	else if (is_lvds && intel_panel_use_ssc(dev_priv) && num_connectors < 2)
		dpll |= PLLB_REF_INPUT_SPREADSPECTRUMIN;
	else
		dpll |= PLL_REF_INPUT_DREFCLK;

	/* setup pipeconf */
	pipeconf = I915_READ(PIPECONF(pipe));

	/* Set up the display plane register */
	dspcntr = DISPPLANE_GAMMA_ENABLE;

<<<<<<< HEAD
	DRM_DEBUG_KMS("Mode for pipe %c:\n", pipe == 0 ? 'A' : 'B');
	drm_mode_debug_printmodeline(mode);

	/* PCH eDP needs FDI, but CPU eDP does not */
	if (!has_edp_encoder || intel_encoder_is_pch_edp(&has_edp_encoder->base)) {
		I915_WRITE(PCH_FP0(pipe), fp);
		I915_WRITE(PCH_DPLL(pipe), dpll & ~DPLL_VCO_ENABLE);

		POSTING_READ(PCH_DPLL(pipe));
		udelay(150);
	}

	/* enable transcoder DPLL */
	if (HAS_PCH_CPT(dev)) {
		temp = I915_READ(PCH_DPLL_SEL);
		switch (pipe) {
		case 0:
			temp |= TRANSA_DPLL_ENABLE | TRANSA_DPLLA_SEL;
			break;
		case 1:
			temp |=	TRANSB_DPLL_ENABLE | TRANSB_DPLLB_SEL;
			break;
		case 2:
			/* FIXME: manage transcoder PLLs? */
			temp |= TRANSC_DPLL_ENABLE | TRANSC_DPLLB_SEL;
			break;
		default:
			BUG();
		}
		I915_WRITE(PCH_DPLL_SEL, temp);

		POSTING_READ(PCH_DPLL_SEL);
		udelay(150);
=======
	DRM_DEBUG_KMS("Mode for pipe %d:\n", pipe);
	drm_mode_debug_printmodeline(mode);

	/* PCH eDP needs FDI, but CPU eDP does not */
	if (!intel_crtc->no_pll) {
		if (!has_edp_encoder ||
		    intel_encoder_is_pch_edp(&has_edp_encoder->base)) {
			I915_WRITE(PCH_FP0(pipe), fp);
			I915_WRITE(PCH_DPLL(pipe), dpll & ~DPLL_VCO_ENABLE);

			POSTING_READ(PCH_DPLL(pipe));
			udelay(150);
		}
	} else {
		if (dpll == (I915_READ(PCH_DPLL(0)) & 0x7fffffff) &&
		    fp == I915_READ(PCH_FP0(0))) {
			intel_crtc->use_pll_a = true;
			DRM_DEBUG_KMS("using pipe a dpll\n");
		} else if (dpll == (I915_READ(PCH_DPLL(1)) & 0x7fffffff) &&
			   fp == I915_READ(PCH_FP0(1))) {
			intel_crtc->use_pll_a = false;
			DRM_DEBUG_KMS("using pipe b dpll\n");
		} else {
			DRM_DEBUG_KMS("no matching PLL configuration for pipe 2\n");
			return -EINVAL;
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/* The LVDS pin pair needs to be on before the DPLLs are enabled.
	 * This is an exception to the general rule that mode_set doesn't turn
	 * things on.
	 */
	if (is_lvds) {
		temp = I915_READ(PCH_LVDS);
		temp |= LVDS_PORT_EN | LVDS_A0A2_CLKA_POWER_UP;
<<<<<<< HEAD
		if (pipe == 1) {
			if (HAS_PCH_CPT(dev))
				temp |= PORT_TRANS_B_SEL_CPT;
			else
				temp |= LVDS_PIPEB_SELECT;
		} else {
			if (HAS_PCH_CPT(dev))
				temp &= ~PORT_TRANS_SEL_MASK;
			else
				temp &= ~LVDS_PIPEB_SELECT;
		}
=======
		if (HAS_PCH_CPT(dev)) {
			temp &= ~PORT_TRANS_SEL_MASK;
			temp |= PORT_TRANS_SEL_CPT(pipe);
		} else {
			if (pipe == 1)
				temp |= LVDS_PIPEB_SELECT;
			else
				temp &= ~LVDS_PIPEB_SELECT;
		}

>>>>>>> refs/remotes/origin/cm-10.0
		/* set the corresponsding LVDS_BORDER bit */
		temp |= dev_priv->lvds_border_bits;
		/* Set the B0-B3 data pairs corresponding to whether we're going to
		 * set the DPLLs for dual-channel mode or not.
		 */
		if (clock.p2 == 7)
			temp |= LVDS_B0B3_POWER_UP | LVDS_CLKB_POWER_UP;
		else
			temp &= ~(LVDS_B0B3_POWER_UP | LVDS_CLKB_POWER_UP);

		/* It would be nice to set 24 vs 18-bit mode (LVDS_A3_POWER_UP)
		 * appropriately here, but we need to look more thoroughly into how
		 * panels behave in the two modes.
		 */
		if (adjusted_mode->flags & DRM_MODE_FLAG_NHSYNC)
			lvds_sync |= LVDS_HSYNC_POLARITY;
		if (adjusted_mode->flags & DRM_MODE_FLAG_NVSYNC)
			lvds_sync |= LVDS_VSYNC_POLARITY;
		if ((temp & (LVDS_HSYNC_POLARITY | LVDS_VSYNC_POLARITY))
		    != lvds_sync) {
			char flags[2] = "-+";
			DRM_INFO("Changing LVDS panel from "
				 "(%chsync, %cvsync) to (%chsync, %cvsync)\n",
				 flags[!(temp & LVDS_HSYNC_POLARITY)],
				 flags[!(temp & LVDS_VSYNC_POLARITY)],
				 flags[!(lvds_sync & LVDS_HSYNC_POLARITY)],
				 flags[!(lvds_sync & LVDS_VSYNC_POLARITY)]);
			temp &= ~(LVDS_HSYNC_POLARITY | LVDS_VSYNC_POLARITY);
			temp |= lvds_sync;
		}
		I915_WRITE(PCH_LVDS, temp);
	}

<<<<<<< HEAD
	/* set the dithering flag and clear for anything other than a panel. */
	pipeconf &= ~PIPECONF_DITHER_EN;
	pipeconf &= ~PIPECONF_DITHER_TYPE_MASK;
	if (dev_priv->lvds_dither && (is_lvds || has_edp_encoder)) {
		pipeconf |= PIPECONF_DITHER_EN;
		pipeconf |= PIPECONF_DITHER_TYPE_ST1;
	}

=======
	pipeconf &= ~PIPECONF_DITHER_EN;
	pipeconf &= ~PIPECONF_DITHER_TYPE_MASK;
	if ((is_lvds && dev_priv->lvds_dither) || dither) {
		pipeconf |= PIPECONF_DITHER_EN;
		pipeconf |= PIPECONF_DITHER_TYPE_SP;
	}
>>>>>>> refs/remotes/origin/cm-10.0
	if (is_dp || intel_encoder_is_pch_edp(&has_edp_encoder->base)) {
		intel_dp_set_m_n(crtc, mode, adjusted_mode);
	} else {
		/* For non-DP output, clear any trans DP clock recovery setting.*/
		I915_WRITE(TRANSDATA_M1(pipe), 0);
		I915_WRITE(TRANSDATA_N1(pipe), 0);
		I915_WRITE(TRANSDPLINK_M1(pipe), 0);
		I915_WRITE(TRANSDPLINK_N1(pipe), 0);
	}

<<<<<<< HEAD
	if (!has_edp_encoder ||
	    intel_encoder_is_pch_edp(&has_edp_encoder->base)) {
=======
	if (!intel_crtc->no_pll &&
	    (!has_edp_encoder ||
	     intel_encoder_is_pch_edp(&has_edp_encoder->base))) {
>>>>>>> refs/remotes/origin/cm-10.0
		I915_WRITE(PCH_DPLL(pipe), dpll);

		/* Wait for the clocks to stabilize. */
		POSTING_READ(PCH_DPLL(pipe));
		udelay(150);

		/* The pixel multiplier can only be updated once the
		 * DPLL is enabled and the clocks are stable.
		 *
		 * So write it again.
		 */
		I915_WRITE(PCH_DPLL(pipe), dpll);
	}

	intel_crtc->lowfreq_avail = false;
<<<<<<< HEAD
	if (is_lvds && has_reduced_clock && i915_powersave) {
		I915_WRITE(PCH_FP1(pipe), fp2);
		intel_crtc->lowfreq_avail = true;
		if (HAS_PIPE_CXSR(dev)) {
			DRM_DEBUG_KMS("enabling CxSR downclocking\n");
			pipeconf |= PIPECONF_CXSR_DOWNCLOCK;
		}
	} else {
		I915_WRITE(PCH_FP1(pipe), fp);
		if (HAS_PIPE_CXSR(dev)) {
			DRM_DEBUG_KMS("disabling CxSR downclocking\n");
			pipeconf &= ~PIPECONF_CXSR_DOWNCLOCK;
		}
	}

	if (adjusted_mode->flags & DRM_MODE_FLAG_INTERLACE) {
		pipeconf |= PIPECONF_INTERLACE_W_FIELD_INDICATION;
		/* the chip adds 2 halflines automatically */
		adjusted_mode->crtc_vdisplay -= 1;
		adjusted_mode->crtc_vtotal -= 1;
		adjusted_mode->crtc_vblank_start -= 1;
		adjusted_mode->crtc_vblank_end -= 1;
		adjusted_mode->crtc_vsync_end -= 1;
		adjusted_mode->crtc_vsync_start -= 1;
	} else
		pipeconf &= ~PIPECONF_INTERLACE_W_FIELD_INDICATION; /* progressive */
=======
	if (!intel_crtc->no_pll) {
		if (is_lvds && has_reduced_clock && i915_powersave) {
			I915_WRITE(PCH_FP1(pipe), fp2);
			intel_crtc->lowfreq_avail = true;
			if (HAS_PIPE_CXSR(dev)) {
				DRM_DEBUG_KMS("enabling CxSR downclocking\n");
				pipeconf |= PIPECONF_CXSR_DOWNCLOCK;
			}
		} else {
			I915_WRITE(PCH_FP1(pipe), fp);
			if (HAS_PIPE_CXSR(dev)) {
				DRM_DEBUG_KMS("disabling CxSR downclocking\n");
				pipeconf &= ~PIPECONF_CXSR_DOWNCLOCK;
			}
		}
	}

	pipeconf &= ~PIPECONF_INTERLACE_MASK;
	if (adjusted_mode->flags & DRM_MODE_FLAG_INTERLACE) {
		pipeconf |= PIPECONF_INTERLACED_ILK;
		/* the chip adds 2 halflines automatically */
		adjusted_mode->crtc_vtotal -= 1;
		adjusted_mode->crtc_vblank_end -= 1;
		I915_WRITE(VSYNCSHIFT(pipe),
			   adjusted_mode->crtc_hsync_start
			   - adjusted_mode->crtc_htotal/2);
	} else {
		pipeconf |= PIPECONF_PROGRESSIVE;
		I915_WRITE(VSYNCSHIFT(pipe), 0);
	}
>>>>>>> refs/remotes/origin/cm-10.0

	I915_WRITE(HTOTAL(pipe),
		   (adjusted_mode->crtc_hdisplay - 1) |
		   ((adjusted_mode->crtc_htotal - 1) << 16));
	I915_WRITE(HBLANK(pipe),
		   (adjusted_mode->crtc_hblank_start - 1) |
		   ((adjusted_mode->crtc_hblank_end - 1) << 16));
	I915_WRITE(HSYNC(pipe),
		   (adjusted_mode->crtc_hsync_start - 1) |
		   ((adjusted_mode->crtc_hsync_end - 1) << 16));

	I915_WRITE(VTOTAL(pipe),
		   (adjusted_mode->crtc_vdisplay - 1) |
		   ((adjusted_mode->crtc_vtotal - 1) << 16));
	I915_WRITE(VBLANK(pipe),
		   (adjusted_mode->crtc_vblank_start - 1) |
		   ((adjusted_mode->crtc_vblank_end - 1) << 16));
	I915_WRITE(VSYNC(pipe),
		   (adjusted_mode->crtc_vsync_start - 1) |
		   ((adjusted_mode->crtc_vsync_end - 1) << 16));

	/* pipesrc controls the size that is scaled from, which should
	 * always be the user's requested size.
	 */
	I915_WRITE(PIPESRC(pipe),
		   ((mode->hdisplay - 1) << 16) | (mode->vdisplay - 1));

	I915_WRITE(PIPE_DATA_M1(pipe), TU_SIZE(m_n.tu) | m_n.gmch_m);
	I915_WRITE(PIPE_DATA_N1(pipe), m_n.gmch_n);
	I915_WRITE(PIPE_LINK_M1(pipe), m_n.link_m);
	I915_WRITE(PIPE_LINK_N1(pipe), m_n.link_n);

	if (has_edp_encoder &&
	    !intel_encoder_is_pch_edp(&has_edp_encoder->base)) {
		ironlake_set_pll_edp(crtc, adjusted_mode->clock);
	}

	I915_WRITE(PIPECONF(pipe), pipeconf);
	POSTING_READ(PIPECONF(pipe));

	intel_wait_for_vblank(dev, pipe);

<<<<<<< HEAD
	if (IS_GEN5(dev)) {
		/* enable address swizzle for tiling buffer */
		temp = I915_READ(DISP_ARB_CTL);
		I915_WRITE(DISP_ARB_CTL, temp | DISP_TILE_SURFACE_SWIZZLING);
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
	I915_WRITE(DSPCNTR(plane), dspcntr);
	POSTING_READ(DSPCNTR(plane));

	ret = intel_pipe_set_base(crtc, x, y, old_fb);

	intel_update_watermarks(dev);

	return ret;
}

static int intel_crtc_mode_set(struct drm_crtc *crtc,
			       struct drm_display_mode *mode,
			       struct drm_display_mode *adjusted_mode,
			       int x, int y,
			       struct drm_framebuffer *old_fb)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
=======
		*has_reduced_clock =
			dev_priv->display.find_dpll(limit, crtc,
						    dev_priv->lvds_downclock,
						    refclk, clock,
						    reduced_clock);
	}

	return true;
}

int ironlake_get_lanes_required(int target_clock, int link_bw, int bpp)
{
	/*
	 * Account for spread spectrum to avoid
	 * oversubscribing the link. Max center spread
	 * is 2.5%; use 5% for safety's sake.
	 */
	u32 bps = target_clock * bpp * 21 / 20;
	return bps / (link_bw * 8) + 1;
}

static bool ironlake_needs_fb_cb_tune(struct dpll *dpll, int factor)
{
	return i9xx_dpll_compute_m(dpll) < factor * dpll->n;
}

static uint32_t ironlake_compute_dpll(struct intel_crtc *intel_crtc,
				      u32 *fp,
				      intel_clock_t *reduced_clock, u32 *fp2)
{
	struct drm_crtc *crtc = &intel_crtc->base;
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_encoder *intel_encoder;
	uint32_t dpll;
	int factor, num_connectors = 0;
	bool is_lvds = false, is_sdvo = false;

	for_each_encoder_on_crtc(dev, crtc, intel_encoder) {
		switch (intel_encoder->type) {
		case INTEL_OUTPUT_LVDS:
			is_lvds = true;
			break;
		case INTEL_OUTPUT_SDVO:
		case INTEL_OUTPUT_HDMI:
			is_sdvo = true;
			break;
		}

		num_connectors++;
	}

	/* Enable autotuning of the PLL clock (if permissible) */
	factor = 21;
	if (is_lvds) {
		if ((intel_panel_use_ssc(dev_priv) &&
		     dev_priv->vbt.lvds_ssc_freq == 100) ||
		    (HAS_PCH_IBX(dev) && intel_is_dual_link_lvds(dev)))
			factor = 25;
	} else if (intel_crtc->config.sdvo_tv_clock)
		factor = 20;

	if (ironlake_needs_fb_cb_tune(&intel_crtc->config.dpll, factor))
		*fp |= FP_CB_TUNE;

	if (fp2 && (reduced_clock->m < factor * reduced_clock->n))
		*fp2 |= FP_CB_TUNE;

	dpll = 0;

	if (is_lvds)
		dpll |= DPLLB_MODE_LVDS;
	else
		dpll |= DPLLB_MODE_DAC_SERIAL;

	dpll |= (intel_crtc->config.pixel_multiplier - 1)
		<< PLL_REF_SDVO_HDMI_MULTIPLIER_SHIFT;

	if (is_sdvo)
		dpll |= DPLL_SDVO_HIGH_SPEED;
	if (intel_crtc->config.has_dp_encoder)
		dpll |= DPLL_SDVO_HIGH_SPEED;

	/* compute bitmask from p1 value */
	dpll |= (1 << (intel_crtc->config.dpll.p1 - 1)) << DPLL_FPA01_P1_POST_DIV_SHIFT;
	/* also FPA1 */
	dpll |= (1 << (intel_crtc->config.dpll.p1 - 1)) << DPLL_FPA1_P1_POST_DIV_SHIFT;

	switch (intel_crtc->config.dpll.p2) {
	case 5:
		dpll |= DPLL_DAC_SERIAL_P2_CLOCK_DIV_5;
		break;
	case 7:
		dpll |= DPLLB_LVDS_P2_CLOCK_DIV_7;
		break;
	case 10:
		dpll |= DPLL_DAC_SERIAL_P2_CLOCK_DIV_10;
		break;
	case 14:
		dpll |= DPLLB_LVDS_P2_CLOCK_DIV_14;
		break;
	}

	if (is_lvds && intel_panel_use_ssc(dev_priv) && num_connectors < 2)
		dpll |= PLLB_REF_INPUT_SPREADSPECTRUMIN;
	else
		dpll |= PLL_REF_INPUT_DREFCLK;

	return dpll | DPLL_VCO_ENABLE;
}

static int ironlake_crtc_mode_set(struct drm_crtc *crtc,
				  int x, int y,
				  struct drm_framebuffer *fb)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;
	int num_connectors = 0;
	intel_clock_t clock, reduced_clock;
	u32 dpll = 0, fp = 0, fp2 = 0;
	bool ok, has_reduced_clock = false;
	bool is_lvds = false;
	struct intel_encoder *encoder;
	struct intel_shared_dpll *pll;
	int ret;

	for_each_encoder_on_crtc(dev, crtc, encoder) {
		switch (encoder->type) {
		case INTEL_OUTPUT_LVDS:
			is_lvds = true;
			break;
		}

		num_connectors++;
	}

	WARN(!(HAS_PCH_IBX(dev) || HAS_PCH_CPT(dev)),
	     "Unexpected PCH type %d\n", INTEL_PCH_TYPE(dev));

	ok = ironlake_compute_clocks(crtc, &clock,
				     &has_reduced_clock, &reduced_clock);
	if (!ok && !intel_crtc->config.clock_set) {
		DRM_ERROR("Couldn't find PLL settings for mode!\n");
		return -EINVAL;
	}
	/* Compat-code for transition, will disappear. */
	if (!intel_crtc->config.clock_set) {
		intel_crtc->config.dpll.n = clock.n;
		intel_crtc->config.dpll.m1 = clock.m1;
		intel_crtc->config.dpll.m2 = clock.m2;
		intel_crtc->config.dpll.p1 = clock.p1;
		intel_crtc->config.dpll.p2 = clock.p2;
	}

	/* CPU eDP is the only output that doesn't need a PCH PLL of its own. */
	if (intel_crtc->config.has_pch_encoder) {
		fp = i9xx_dpll_compute_fp(&intel_crtc->config.dpll);
		if (has_reduced_clock)
			fp2 = i9xx_dpll_compute_fp(&reduced_clock);

		dpll = ironlake_compute_dpll(intel_crtc,
					     &fp, &reduced_clock,
					     has_reduced_clock ? &fp2 : NULL);

		intel_crtc->config.dpll_hw_state.dpll = dpll;
		intel_crtc->config.dpll_hw_state.fp0 = fp;
		if (has_reduced_clock)
			intel_crtc->config.dpll_hw_state.fp1 = fp2;
		else
			intel_crtc->config.dpll_hw_state.fp1 = fp;

		pll = intel_get_shared_dpll(intel_crtc);
		if (pll == NULL) {
			DRM_DEBUG_DRIVER("failed to find PLL for pipe %c\n",
					 pipe_name(pipe));
			return -EINVAL;
		}
	} else
		intel_put_shared_dpll(intel_crtc);

	if (intel_crtc->config.has_dp_encoder)
		intel_dp_set_m_n(intel_crtc);

	if (is_lvds && has_reduced_clock && i915_powersave)
		intel_crtc->lowfreq_avail = true;
	else
		intel_crtc->lowfreq_avail = false;

	intel_set_pipe_timings(intel_crtc);

	if (intel_crtc->config.has_pch_encoder) {
		intel_cpu_transcoder_set_m_n(intel_crtc,
					     &intel_crtc->config.fdi_m_n);
	}

	ironlake_set_pipeconf(crtc);

	/* Set up the display plane register */
	I915_WRITE(DSPCNTR(plane), DISPPLANE_GAMMA_ENABLE);
	POSTING_READ(DSPCNTR(plane));

	ret = intel_pipe_set_base(crtc, x, y, fb);

	return ret;
}

static void intel_pch_transcoder_get_m_n(struct intel_crtc *crtc,
					 struct intel_link_m_n *m_n)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	enum pipe pipe = crtc->pipe;

	m_n->link_m = I915_READ(PCH_TRANS_LINK_M1(pipe));
	m_n->link_n = I915_READ(PCH_TRANS_LINK_N1(pipe));
	m_n->gmch_m = I915_READ(PCH_TRANS_DATA_M1(pipe))
		& ~TU_SIZE_MASK;
	m_n->gmch_n = I915_READ(PCH_TRANS_DATA_N1(pipe));
	m_n->tu = ((I915_READ(PCH_TRANS_DATA_M1(pipe))
		    & TU_SIZE_MASK) >> TU_SIZE_SHIFT) + 1;
}

static void intel_cpu_transcoder_get_m_n(struct intel_crtc *crtc,
					 enum transcoder transcoder,
					 struct intel_link_m_n *m_n)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	enum pipe pipe = crtc->pipe;

	if (INTEL_INFO(dev)->gen >= 5) {
		m_n->link_m = I915_READ(PIPE_LINK_M1(transcoder));
		m_n->link_n = I915_READ(PIPE_LINK_N1(transcoder));
		m_n->gmch_m = I915_READ(PIPE_DATA_M1(transcoder))
			& ~TU_SIZE_MASK;
		m_n->gmch_n = I915_READ(PIPE_DATA_N1(transcoder));
		m_n->tu = ((I915_READ(PIPE_DATA_M1(transcoder))
			    & TU_SIZE_MASK) >> TU_SIZE_SHIFT) + 1;
	} else {
		m_n->link_m = I915_READ(PIPE_LINK_M_G4X(pipe));
		m_n->link_n = I915_READ(PIPE_LINK_N_G4X(pipe));
		m_n->gmch_m = I915_READ(PIPE_DATA_M_G4X(pipe))
			& ~TU_SIZE_MASK;
		m_n->gmch_n = I915_READ(PIPE_DATA_N_G4X(pipe));
		m_n->tu = ((I915_READ(PIPE_DATA_M_G4X(pipe))
			    & TU_SIZE_MASK) >> TU_SIZE_SHIFT) + 1;
	}
}

void intel_dp_get_m_n(struct intel_crtc *crtc,
		      struct intel_crtc_config *pipe_config)
{
	if (crtc->config.has_pch_encoder)
		intel_pch_transcoder_get_m_n(crtc, &pipe_config->dp_m_n);
	else
		intel_cpu_transcoder_get_m_n(crtc, pipe_config->cpu_transcoder,
					     &pipe_config->dp_m_n);
}

static void ironlake_get_fdi_m_n_config(struct intel_crtc *crtc,
					struct intel_crtc_config *pipe_config)
{
	intel_cpu_transcoder_get_m_n(crtc, pipe_config->cpu_transcoder,
				     &pipe_config->fdi_m_n);
}

static void ironlake_get_pfit_config(struct intel_crtc *crtc,
				     struct intel_crtc_config *pipe_config)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t tmp;

	tmp = I915_READ(PF_CTL(crtc->pipe));

	if (tmp & PF_ENABLE) {
		pipe_config->pch_pfit.enabled = true;
		pipe_config->pch_pfit.pos = I915_READ(PF_WIN_POS(crtc->pipe));
		pipe_config->pch_pfit.size = I915_READ(PF_WIN_SZ(crtc->pipe));

		/* We currently do not free assignements of panel fitters on
		 * ivb/hsw (since we don't use the higher upscaling modes which
		 * differentiates them) so just WARN about this case for now. */
		if (IS_GEN7(dev)) {
			WARN_ON((tmp & PF_PIPE_SEL_MASK_IVB) !=
				PF_PIPE_SEL_IVB(crtc->pipe));
		}
	}
}

static bool ironlake_get_pipe_config(struct intel_crtc *crtc,
				     struct intel_crtc_config *pipe_config)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t tmp;

	pipe_config->cpu_transcoder = (enum transcoder) crtc->pipe;
	pipe_config->shared_dpll = DPLL_ID_PRIVATE;

	tmp = I915_READ(PIPECONF(crtc->pipe));
	if (!(tmp & PIPECONF_ENABLE))
		return false;

	switch (tmp & PIPECONF_BPC_MASK) {
	case PIPECONF_6BPC:
		pipe_config->pipe_bpp = 18;
		break;
	case PIPECONF_8BPC:
		pipe_config->pipe_bpp = 24;
		break;
	case PIPECONF_10BPC:
		pipe_config->pipe_bpp = 30;
		break;
	case PIPECONF_12BPC:
		pipe_config->pipe_bpp = 36;
		break;
	default:
		break;
	}

	if (I915_READ(PCH_TRANSCONF(crtc->pipe)) & TRANS_ENABLE) {
		struct intel_shared_dpll *pll;

		pipe_config->has_pch_encoder = true;

		tmp = I915_READ(FDI_RX_CTL(crtc->pipe));
		pipe_config->fdi_lanes = ((FDI_DP_PORT_WIDTH_MASK & tmp) >>
					  FDI_DP_PORT_WIDTH_SHIFT) + 1;

		ironlake_get_fdi_m_n_config(crtc, pipe_config);

		if (HAS_PCH_IBX(dev_priv->dev)) {
			pipe_config->shared_dpll =
				(enum intel_dpll_id) crtc->pipe;
		} else {
			tmp = I915_READ(PCH_DPLL_SEL);
			if (tmp & TRANS_DPLLB_SEL(crtc->pipe))
				pipe_config->shared_dpll = DPLL_ID_PCH_PLL_B;
			else
				pipe_config->shared_dpll = DPLL_ID_PCH_PLL_A;
		}

		pll = &dev_priv->shared_dplls[pipe_config->shared_dpll];

		WARN_ON(!pll->get_hw_state(dev_priv, pll,
					   &pipe_config->dpll_hw_state));

		tmp = pipe_config->dpll_hw_state.dpll;
		pipe_config->pixel_multiplier =
			((tmp & PLL_REF_SDVO_HDMI_MULTIPLIER_MASK)
			 >> PLL_REF_SDVO_HDMI_MULTIPLIER_SHIFT) + 1;

		ironlake_pch_clock_get(crtc, pipe_config);
	} else {
		pipe_config->pixel_multiplier = 1;
	}

	intel_get_pipe_timings(crtc, pipe_config);

	ironlake_get_pfit_config(crtc, pipe_config);

	return true;
}

static void assert_can_disable_lcpll(struct drm_i915_private *dev_priv)
{
	struct drm_device *dev = dev_priv->dev;
	struct intel_ddi_plls *plls = &dev_priv->ddi_plls;
	struct intel_crtc *crtc;
	unsigned long irqflags;
	uint32_t val;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, base.head)
		WARN(crtc->active, "CRTC for pipe %c enabled\n",
		     pipe_name(crtc->pipe));

	WARN(I915_READ(HSW_PWR_WELL_DRIVER), "Power well on\n");
	WARN(plls->spll_refcount, "SPLL enabled\n");
	WARN(plls->wrpll1_refcount, "WRPLL1 enabled\n");
	WARN(plls->wrpll2_refcount, "WRPLL2 enabled\n");
	WARN(I915_READ(PCH_PP_STATUS) & PP_ON, "Panel power on\n");
	WARN(I915_READ(BLC_PWM_CPU_CTL2) & BLM_PWM_ENABLE,
	     "CPU PWM1 enabled\n");
	WARN(I915_READ(HSW_BLC_PWM2_CTL) & BLM_PWM_ENABLE,
	     "CPU PWM2 enabled\n");
	WARN(I915_READ(BLC_PWM_PCH_CTL1) & BLM_PCH_PWM_ENABLE,
	     "PCH PWM1 enabled\n");
	WARN(I915_READ(UTIL_PIN_CTL) & UTIL_PIN_ENABLE,
	     "Utility pin enabled\n");
	WARN(I915_READ(PCH_GTC_CTL) & PCH_GTC_ENABLE, "PCH GTC enabled\n");

	spin_lock_irqsave(&dev_priv->irq_lock, irqflags);
	val = I915_READ(DEIMR);
	WARN((val & ~DE_PCH_EVENT_IVB) != val,
	     "Unexpected DEIMR bits enabled: 0x%x\n", val);
	val = I915_READ(SDEIMR);
	WARN((val | SDE_HOTPLUG_MASK_CPT) != 0xffffffff,
	     "Unexpected SDEIMR bits enabled: 0x%x\n", val);
	spin_unlock_irqrestore(&dev_priv->irq_lock, irqflags);
}

/*
 * This function implements pieces of two sequences from BSpec:
 * - Sequence for display software to disable LCPLL
 * - Sequence for display software to allow package C8+
 * The steps implemented here are just the steps that actually touch the LCPLL
 * register. Callers should take care of disabling all the display engine
 * functions, doing the mode unset, fixing interrupts, etc.
 */
static void hsw_disable_lcpll(struct drm_i915_private *dev_priv,
			      bool switch_to_fclk, bool allow_power_down)
{
	uint32_t val;

	assert_can_disable_lcpll(dev_priv);

	val = I915_READ(LCPLL_CTL);

	if (switch_to_fclk) {
		val |= LCPLL_CD_SOURCE_FCLK;
		I915_WRITE(LCPLL_CTL, val);

		if (wait_for_atomic_us(I915_READ(LCPLL_CTL) &
				       LCPLL_CD_SOURCE_FCLK_DONE, 1))
			DRM_ERROR("Switching to FCLK failed\n");

		val = I915_READ(LCPLL_CTL);
	}

	val |= LCPLL_PLL_DISABLE;
	I915_WRITE(LCPLL_CTL, val);
	POSTING_READ(LCPLL_CTL);

	if (wait_for((I915_READ(LCPLL_CTL) & LCPLL_PLL_LOCK) == 0, 1))
		DRM_ERROR("LCPLL still locked\n");

	val = I915_READ(D_COMP);
	val |= D_COMP_COMP_DISABLE;
	mutex_lock(&dev_priv->rps.hw_lock);
	if (sandybridge_pcode_write(dev_priv, GEN6_PCODE_WRITE_D_COMP, val))
		DRM_ERROR("Failed to disable D_COMP\n");
	mutex_unlock(&dev_priv->rps.hw_lock);
	POSTING_READ(D_COMP);
	ndelay(100);

	if (wait_for((I915_READ(D_COMP) & D_COMP_RCOMP_IN_PROGRESS) == 0, 1))
		DRM_ERROR("D_COMP RCOMP still in progress\n");

	if (allow_power_down) {
		val = I915_READ(LCPLL_CTL);
		val |= LCPLL_POWER_DOWN_ALLOW;
		I915_WRITE(LCPLL_CTL, val);
		POSTING_READ(LCPLL_CTL);
	}
}

/*
 * Fully restores LCPLL, disallowing power down and switching back to LCPLL
 * source.
 */
static void hsw_restore_lcpll(struct drm_i915_private *dev_priv)
{
	uint32_t val;

	val = I915_READ(LCPLL_CTL);

	if ((val & (LCPLL_PLL_LOCK | LCPLL_PLL_DISABLE | LCPLL_CD_SOURCE_FCLK |
		    LCPLL_POWER_DOWN_ALLOW)) == LCPLL_PLL_LOCK)
		return;

	/* Make sure we're not on PC8 state before disabling PC8, otherwise
	 * we'll hang the machine! */
	gen6_gt_force_wake_get(dev_priv);

	if (val & LCPLL_POWER_DOWN_ALLOW) {
		val &= ~LCPLL_POWER_DOWN_ALLOW;
		I915_WRITE(LCPLL_CTL, val);
		POSTING_READ(LCPLL_CTL);
	}

	val = I915_READ(D_COMP);
	val |= D_COMP_COMP_FORCE;
	val &= ~D_COMP_COMP_DISABLE;
	mutex_lock(&dev_priv->rps.hw_lock);
	if (sandybridge_pcode_write(dev_priv, GEN6_PCODE_WRITE_D_COMP, val))
		DRM_ERROR("Failed to enable D_COMP\n");
	mutex_unlock(&dev_priv->rps.hw_lock);
	POSTING_READ(D_COMP);

	val = I915_READ(LCPLL_CTL);
	val &= ~LCPLL_PLL_DISABLE;
	I915_WRITE(LCPLL_CTL, val);

	if (wait_for(I915_READ(LCPLL_CTL) & LCPLL_PLL_LOCK, 5))
		DRM_ERROR("LCPLL not locked yet\n");

	if (val & LCPLL_CD_SOURCE_FCLK) {
		val = I915_READ(LCPLL_CTL);
		val &= ~LCPLL_CD_SOURCE_FCLK;
		I915_WRITE(LCPLL_CTL, val);

		if (wait_for_atomic_us((I915_READ(LCPLL_CTL) &
					LCPLL_CD_SOURCE_FCLK_DONE) == 0, 1))
			DRM_ERROR("Switching back to LCPLL failed\n");
	}

	gen6_gt_force_wake_put(dev_priv);
}

void hsw_enable_pc8_work(struct work_struct *__work)
{
	struct drm_i915_private *dev_priv =
		container_of(to_delayed_work(__work), struct drm_i915_private,
			     pc8.enable_work);
	struct drm_device *dev = dev_priv->dev;
	uint32_t val;

	if (dev_priv->pc8.enabled)
		return;

	DRM_DEBUG_KMS("Enabling package C8+\n");

	dev_priv->pc8.enabled = true;

	if (dev_priv->pch_id == INTEL_PCH_LPT_LP_DEVICE_ID_TYPE) {
		val = I915_READ(SOUTH_DSPCLK_GATE_D);
		val &= ~PCH_LP_PARTITION_LEVEL_DISABLE;
		I915_WRITE(SOUTH_DSPCLK_GATE_D, val);
	}

	lpt_disable_clkout_dp(dev);
	hsw_pc8_disable_interrupts(dev);
	hsw_disable_lcpll(dev_priv, true, true);
}

static void __hsw_enable_package_c8(struct drm_i915_private *dev_priv)
{
	WARN_ON(!mutex_is_locked(&dev_priv->pc8.lock));
	WARN(dev_priv->pc8.disable_count < 1,
	     "pc8.disable_count: %d\n", dev_priv->pc8.disable_count);

	dev_priv->pc8.disable_count--;
	if (dev_priv->pc8.disable_count != 0)
		return;

	schedule_delayed_work(&dev_priv->pc8.enable_work,
			      msecs_to_jiffies(i915_pc8_timeout));
}

static void __hsw_disable_package_c8(struct drm_i915_private *dev_priv)
{
	struct drm_device *dev = dev_priv->dev;
	uint32_t val;

	WARN_ON(!mutex_is_locked(&dev_priv->pc8.lock));
	WARN(dev_priv->pc8.disable_count < 0,
	     "pc8.disable_count: %d\n", dev_priv->pc8.disable_count);

	dev_priv->pc8.disable_count++;
	if (dev_priv->pc8.disable_count != 1)
		return;

	cancel_delayed_work_sync(&dev_priv->pc8.enable_work);
	if (!dev_priv->pc8.enabled)
		return;

	DRM_DEBUG_KMS("Disabling package C8+\n");

	hsw_restore_lcpll(dev_priv);
	hsw_pc8_restore_interrupts(dev);
	lpt_init_pch_refclk(dev);

	if (dev_priv->pch_id == INTEL_PCH_LPT_LP_DEVICE_ID_TYPE) {
		val = I915_READ(SOUTH_DSPCLK_GATE_D);
		val |= PCH_LP_PARTITION_LEVEL_DISABLE;
		I915_WRITE(SOUTH_DSPCLK_GATE_D, val);
	}

	intel_prepare_ddi(dev);
	i915_gem_init_swizzling(dev);
	mutex_lock(&dev_priv->rps.hw_lock);
	gen6_update_ring_freq(dev);
	mutex_unlock(&dev_priv->rps.hw_lock);
	dev_priv->pc8.enabled = false;
}

void hsw_enable_package_c8(struct drm_i915_private *dev_priv)
{
	if (!HAS_PC8(dev_priv->dev))
		return;

	mutex_lock(&dev_priv->pc8.lock);
	__hsw_enable_package_c8(dev_priv);
	mutex_unlock(&dev_priv->pc8.lock);
}

void hsw_disable_package_c8(struct drm_i915_private *dev_priv)
{
	if (!HAS_PC8(dev_priv->dev))
		return;

	mutex_lock(&dev_priv->pc8.lock);
	__hsw_disable_package_c8(dev_priv);
	mutex_unlock(&dev_priv->pc8.lock);
}

static bool hsw_can_enable_package_c8(struct drm_i915_private *dev_priv)
{
	struct drm_device *dev = dev_priv->dev;
	struct intel_crtc *crtc;
	uint32_t val;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, base.head)
		if (crtc->base.enabled)
			return false;

	/* This case is still possible since we have the i915.disable_power_well
	 * parameter and also the KVMr or something else might be requesting the
	 * power well. */
	val = I915_READ(HSW_PWR_WELL_DRIVER);
	if (val != 0) {
		DRM_DEBUG_KMS("Not enabling PC8: power well on\n");
		return false;
	}

	return true;
}

/* Since we're called from modeset_global_resources there's no way to
 * symmetrically increase and decrease the refcount, so we use
 * dev_priv->pc8.requirements_met to track whether we already have the refcount
 * or not.
 */
static void hsw_update_package_c8(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	bool allow;

	if (!HAS_PC8(dev_priv->dev))
		return;

	if (!i915_enable_pc8)
		return;

	mutex_lock(&dev_priv->pc8.lock);

	allow = hsw_can_enable_package_c8(dev_priv);

	if (allow == dev_priv->pc8.requirements_met)
		goto done;

	dev_priv->pc8.requirements_met = allow;

	if (allow)
		__hsw_enable_package_c8(dev_priv);
	else
		__hsw_disable_package_c8(dev_priv);

done:
	mutex_unlock(&dev_priv->pc8.lock);
}

static void hsw_package_c8_gpu_idle(struct drm_i915_private *dev_priv)
{
	if (!HAS_PC8(dev_priv->dev))
		return;

	mutex_lock(&dev_priv->pc8.lock);
	if (!dev_priv->pc8.gpu_idle) {
		dev_priv->pc8.gpu_idle = true;
		__hsw_enable_package_c8(dev_priv);
	}
	mutex_unlock(&dev_priv->pc8.lock);
}

static void hsw_package_c8_gpu_busy(struct drm_i915_private *dev_priv)
{
	if (!HAS_PC8(dev_priv->dev))
		return;

	mutex_lock(&dev_priv->pc8.lock);
	if (dev_priv->pc8.gpu_idle) {
		dev_priv->pc8.gpu_idle = false;
		__hsw_disable_package_c8(dev_priv);
	}
	mutex_unlock(&dev_priv->pc8.lock);
}

#define for_each_power_domain(domain, mask)				\
	for ((domain) = 0; (domain) < POWER_DOMAIN_NUM; (domain)++)	\
		if ((1 << (domain)) & (mask))

static unsigned long get_pipe_power_domains(struct drm_device *dev,
					    enum pipe pipe, bool pfit_enabled)
{
	unsigned long mask;
	enum transcoder transcoder;

	transcoder = intel_pipe_to_cpu_transcoder(dev->dev_private, pipe);

	mask = BIT(POWER_DOMAIN_PIPE(pipe));
	mask |= BIT(POWER_DOMAIN_TRANSCODER(transcoder));
	if (pfit_enabled)
		mask |= BIT(POWER_DOMAIN_PIPE_PANEL_FITTER(pipe));

	return mask;
}

void intel_display_set_init_power(struct drm_device *dev, bool enable)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (dev_priv->power_domains.init_power_on == enable)
		return;

	if (enable)
		intel_display_power_get(dev, POWER_DOMAIN_INIT);
	else
		intel_display_power_put(dev, POWER_DOMAIN_INIT);

	dev_priv->power_domains.init_power_on = enable;
}

static void modeset_update_power_wells(struct drm_device *dev)
{
	unsigned long pipe_domains[I915_MAX_PIPES] = { 0, };
	struct intel_crtc *crtc;

	/*
	 * First get all needed power domains, then put all unneeded, to avoid
	 * any unnecessary toggling of the power wells.
	 */
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, base.head) {
		enum intel_display_power_domain domain;

		if (!crtc->base.enabled)
			continue;

		pipe_domains[crtc->pipe] = get_pipe_power_domains(dev,
						crtc->pipe,
						crtc->config.pch_pfit.enabled);

		for_each_power_domain(domain, pipe_domains[crtc->pipe])
			intel_display_power_get(dev, domain);
	}

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, base.head) {
		enum intel_display_power_domain domain;

		for_each_power_domain(domain, crtc->enabled_power_domains)
			intel_display_power_put(dev, domain);

		crtc->enabled_power_domains = pipe_domains[crtc->pipe];
	}

	intel_display_set_init_power(dev, false);
}

static void haswell_modeset_global_resources(struct drm_device *dev)
{
	modeset_update_power_wells(dev);
	hsw_update_package_c8(dev);
}

static int haswell_crtc_mode_set(struct drm_crtc *crtc,
				 int x, int y,
				 struct drm_framebuffer *fb)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int plane = intel_crtc->plane;
	int ret;

	if (!intel_ddi_pll_mode_set(crtc))
		return -EINVAL;

	if (intel_crtc->config.has_dp_encoder)
		intel_dp_set_m_n(intel_crtc);

	intel_crtc->lowfreq_avail = false;

	intel_set_pipe_timings(intel_crtc);

	if (intel_crtc->config.has_pch_encoder) {
		intel_cpu_transcoder_set_m_n(intel_crtc,
					     &intel_crtc->config.fdi_m_n);
	}

	haswell_set_pipeconf(crtc);

	intel_set_pipe_csc(crtc);

	/* Set up the display plane register */
	I915_WRITE(DSPCNTR(plane), DISPPLANE_GAMMA_ENABLE | DISPPLANE_PIPE_CSC_ENABLE);
	POSTING_READ(DSPCNTR(plane));

	ret = intel_pipe_set_base(crtc, x, y, fb);

	return ret;
}

static bool haswell_get_pipe_config(struct intel_crtc *crtc,
				    struct intel_crtc_config *pipe_config)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	enum intel_display_power_domain pfit_domain;
	uint32_t tmp;

	pipe_config->cpu_transcoder = (enum transcoder) crtc->pipe;
	pipe_config->shared_dpll = DPLL_ID_PRIVATE;

	tmp = I915_READ(TRANS_DDI_FUNC_CTL(TRANSCODER_EDP));
	if (tmp & TRANS_DDI_FUNC_ENABLE) {
		enum pipe trans_edp_pipe;
		switch (tmp & TRANS_DDI_EDP_INPUT_MASK) {
		default:
			WARN(1, "unknown pipe linked to edp transcoder\n");
		case TRANS_DDI_EDP_INPUT_A_ONOFF:
		case TRANS_DDI_EDP_INPUT_A_ON:
			trans_edp_pipe = PIPE_A;
			break;
		case TRANS_DDI_EDP_INPUT_B_ONOFF:
			trans_edp_pipe = PIPE_B;
			break;
		case TRANS_DDI_EDP_INPUT_C_ONOFF:
			trans_edp_pipe = PIPE_C;
			break;
		}

		if (trans_edp_pipe == crtc->pipe)
			pipe_config->cpu_transcoder = TRANSCODER_EDP;
	}

	if (!intel_display_power_enabled(dev,
			POWER_DOMAIN_TRANSCODER(pipe_config->cpu_transcoder)))
		return false;

	tmp = I915_READ(PIPECONF(pipe_config->cpu_transcoder));
	if (!(tmp & PIPECONF_ENABLE))
		return false;

	/*
	 * Haswell has only FDI/PCH transcoder A. It is which is connected to
	 * DDI E. So just check whether this pipe is wired to DDI E and whether
	 * the PCH transcoder is on.
	 */
	tmp = I915_READ(TRANS_DDI_FUNC_CTL(pipe_config->cpu_transcoder));
	if ((tmp & TRANS_DDI_PORT_MASK) == TRANS_DDI_SELECT_PORT(PORT_E) &&
	    I915_READ(LPT_TRANSCONF) & TRANS_ENABLE) {
		pipe_config->has_pch_encoder = true;

		tmp = I915_READ(FDI_RX_CTL(PIPE_A));
		pipe_config->fdi_lanes = ((FDI_DP_PORT_WIDTH_MASK & tmp) >>
					  FDI_DP_PORT_WIDTH_SHIFT) + 1;

		ironlake_get_fdi_m_n_config(crtc, pipe_config);
	}

	intel_get_pipe_timings(crtc, pipe_config);

	pfit_domain = POWER_DOMAIN_PIPE_PANEL_FITTER(crtc->pipe);
	if (intel_display_power_enabled(dev, pfit_domain))
		ironlake_get_pfit_config(crtc, pipe_config);

	pipe_config->ips_enabled = hsw_crtc_supports_ips(crtc) &&
				   (I915_READ(IPS_CTL) & IPS_ENABLE);

	pipe_config->pixel_multiplier = 1;

	return true;
}

static int intel_crtc_mode_set(struct drm_crtc *crtc,
			       int x, int y,
			       struct drm_framebuffer *fb)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_encoder *encoder;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct drm_display_mode *mode = &intel_crtc->config.requested_mode;
>>>>>>> refs/remotes/origin/master
	int pipe = intel_crtc->pipe;
	int ret;

	drm_vblank_pre_modeset(dev, pipe);

<<<<<<< HEAD
	ret = dev_priv->display.crtc_mode_set(crtc, mode, adjusted_mode,
					      x, y, old_fb);
<<<<<<< HEAD

	drm_vblank_post_modeset(dev, pipe);

	return ret;
}

=======
	drm_vblank_post_modeset(dev, pipe);

	if (ret)
		intel_crtc->dpms_mode = DRM_MODE_DPMS_OFF;
	else
		intel_crtc->dpms_mode = DRM_MODE_DPMS_ON;

	return ret;
=======
	ret = dev_priv->display.crtc_mode_set(crtc, x, y, fb);

	drm_vblank_post_modeset(dev, pipe);

	if (ret != 0)
		return ret;

	for_each_encoder_on_crtc(dev, crtc, encoder) {
		DRM_DEBUG_KMS("[ENCODER:%d:%s] set [MODE:%d:%s]\n",
			encoder->base.base.id,
			drm_get_encoder_name(&encoder->base),
			mode->base.id, mode->name);
		encoder->mode_set(encoder);
	}

	return 0;
}

static struct {
	int clock;
	u32 config;
} hdmi_audio_clock[] = {
	{ DIV_ROUND_UP(25200 * 1000, 1001), AUD_CONFIG_PIXEL_CLOCK_HDMI_25175 },
	{ 25200, AUD_CONFIG_PIXEL_CLOCK_HDMI_25200 }, /* default per bspec */
	{ 27000, AUD_CONFIG_PIXEL_CLOCK_HDMI_27000 },
	{ 27000 * 1001 / 1000, AUD_CONFIG_PIXEL_CLOCK_HDMI_27027 },
	{ 54000, AUD_CONFIG_PIXEL_CLOCK_HDMI_54000 },
	{ 54000 * 1001 / 1000, AUD_CONFIG_PIXEL_CLOCK_HDMI_54054 },
	{ DIV_ROUND_UP(74250 * 1000, 1001), AUD_CONFIG_PIXEL_CLOCK_HDMI_74176 },
	{ 74250, AUD_CONFIG_PIXEL_CLOCK_HDMI_74250 },
	{ DIV_ROUND_UP(148500 * 1000, 1001), AUD_CONFIG_PIXEL_CLOCK_HDMI_148352 },
	{ 148500, AUD_CONFIG_PIXEL_CLOCK_HDMI_148500 },
};

/* get AUD_CONFIG_PIXEL_CLOCK_HDMI_* value for mode */
static u32 audio_config_hdmi_pixel_clock(struct drm_display_mode *mode)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(hdmi_audio_clock); i++) {
		if (mode->clock == hdmi_audio_clock[i].clock)
			break;
	}

	if (i == ARRAY_SIZE(hdmi_audio_clock)) {
		DRM_DEBUG_KMS("HDMI audio pixel clock setting for %d not found, falling back to defaults\n", mode->clock);
		i = 1;
	}

	DRM_DEBUG_KMS("Configuring HDMI audio for pixel clock %d (0x%08x)\n",
		      hdmi_audio_clock[i].clock,
		      hdmi_audio_clock[i].config);

	return hdmi_audio_clock[i].config;
>>>>>>> refs/remotes/origin/master
}

static bool intel_eld_uptodate(struct drm_connector *connector,
			       int reg_eldv, uint32_t bits_eldv,
			       int reg_elda, uint32_t bits_elda,
			       int reg_edid)
{
	struct drm_i915_private *dev_priv = connector->dev->dev_private;
	uint8_t *eld = connector->eld;
	uint32_t i;

	i = I915_READ(reg_eldv);
	i &= bits_eldv;

	if (!eld[0])
		return !i;

	if (!i)
		return false;

	i = I915_READ(reg_elda);
	i &= ~bits_elda;
	I915_WRITE(reg_elda, i);

	for (i = 0; i < eld[2]; i++)
		if (I915_READ(reg_edid) != *((uint32_t *)eld + i))
			return false;

	return true;
}

static void g4x_write_eld(struct drm_connector *connector,
<<<<<<< HEAD
			  struct drm_crtc *crtc)
=======
			  struct drm_crtc *crtc,
			  struct drm_display_mode *mode)
>>>>>>> refs/remotes/origin/master
{
	struct drm_i915_private *dev_priv = connector->dev->dev_private;
	uint8_t *eld = connector->eld;
	uint32_t eldv;
	uint32_t len;
	uint32_t i;

	i = I915_READ(G4X_AUD_VID_DID);

	if (i == INTEL_AUDIO_DEVBLC || i == INTEL_AUDIO_DEVCL)
		eldv = G4X_ELDV_DEVCL_DEVBLC;
	else
		eldv = G4X_ELDV_DEVCTG;

	if (intel_eld_uptodate(connector,
			       G4X_AUD_CNTL_ST, eldv,
			       G4X_AUD_CNTL_ST, G4X_ELD_ADDR,
			       G4X_HDMIW_HDMIEDID))
		return;

	i = I915_READ(G4X_AUD_CNTL_ST);
	i &= ~(eldv | G4X_ELD_ADDR);
	len = (i >> 9) & 0x1f;		/* ELD buffer size */
	I915_WRITE(G4X_AUD_CNTL_ST, i);

	if (!eld[0])
		return;

	len = min_t(uint8_t, eld[2], len);
	DRM_DEBUG_DRIVER("ELD size %d\n", len);
	for (i = 0; i < len; i++)
		I915_WRITE(G4X_HDMIW_HDMIEDID, *((uint32_t *)eld + i));

	i = I915_READ(G4X_AUD_CNTL_ST);
	i |= eldv;
	I915_WRITE(G4X_AUD_CNTL_ST, i);
}

<<<<<<< HEAD
static void ironlake_write_eld(struct drm_connector *connector,
				     struct drm_crtc *crtc)
=======
static void haswell_write_eld(struct drm_connector *connector,
			      struct drm_crtc *crtc,
			      struct drm_display_mode *mode)
{
	struct drm_i915_private *dev_priv = connector->dev->dev_private;
	uint8_t *eld = connector->eld;
	struct drm_device *dev = crtc->dev;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	uint32_t eldv;
	uint32_t i;
	int len;
	int pipe = to_intel_crtc(crtc)->pipe;
	int tmp;

	int hdmiw_hdmiedid = HSW_AUD_EDID_DATA(pipe);
	int aud_cntl_st = HSW_AUD_DIP_ELD_CTRL(pipe);
	int aud_config = HSW_AUD_CFG(pipe);
	int aud_cntrl_st2 = HSW_AUD_PIN_ELD_CP_VLD;


	DRM_DEBUG_DRIVER("HDMI: Haswell Audio initialize....\n");

	/* Audio output enable */
	DRM_DEBUG_DRIVER("HDMI audio: enable codec\n");
	tmp = I915_READ(aud_cntrl_st2);
	tmp |= (AUDIO_OUTPUT_ENABLE_A << (pipe * 4));
	I915_WRITE(aud_cntrl_st2, tmp);

	/* Wait for 1 vertical blank */
	intel_wait_for_vblank(dev, pipe);

	/* Set ELD valid state */
	tmp = I915_READ(aud_cntrl_st2);
	DRM_DEBUG_DRIVER("HDMI audio: pin eld vld status=0x%08x\n", tmp);
	tmp |= (AUDIO_ELD_VALID_A << (pipe * 4));
	I915_WRITE(aud_cntrl_st2, tmp);
	tmp = I915_READ(aud_cntrl_st2);
	DRM_DEBUG_DRIVER("HDMI audio: eld vld status=0x%08x\n", tmp);

	/* Enable HDMI mode */
	tmp = I915_READ(aud_config);
	DRM_DEBUG_DRIVER("HDMI audio: audio conf: 0x%08x\n", tmp);
	/* clear N_programing_enable and N_value_index */
	tmp &= ~(AUD_CONFIG_N_VALUE_INDEX | AUD_CONFIG_N_PROG_ENABLE);
	I915_WRITE(aud_config, tmp);

	DRM_DEBUG_DRIVER("ELD on pipe %c\n", pipe_name(pipe));

	eldv = AUDIO_ELD_VALID_A << (pipe * 4);
	intel_crtc->eld_vld = true;

	if (intel_pipe_has_type(crtc, INTEL_OUTPUT_DISPLAYPORT)) {
		DRM_DEBUG_DRIVER("ELD: DisplayPort detected\n");
		eld[5] |= (1 << 2);	/* Conn_Type, 0x1 = DisplayPort */
		I915_WRITE(aud_config, AUD_CONFIG_N_VALUE_INDEX); /* 0x1 = DP */
	} else {
		I915_WRITE(aud_config, audio_config_hdmi_pixel_clock(mode));
	}

	if (intel_eld_uptodate(connector,
			       aud_cntrl_st2, eldv,
			       aud_cntl_st, IBX_ELD_ADDRESS,
			       hdmiw_hdmiedid))
		return;

	i = I915_READ(aud_cntrl_st2);
	i &= ~eldv;
	I915_WRITE(aud_cntrl_st2, i);

	if (!eld[0])
		return;

	i = I915_READ(aud_cntl_st);
	i &= ~IBX_ELD_ADDRESS;
	I915_WRITE(aud_cntl_st, i);
	i = (i >> 29) & DIP_PORT_SEL_MASK;		/* DIP_Port_Select, 0x1 = PortB */
	DRM_DEBUG_DRIVER("port num:%d\n", i);

	len = min_t(uint8_t, eld[2], 21);	/* 84 bytes of hw ELD buffer */
	DRM_DEBUG_DRIVER("ELD size %d\n", len);
	for (i = 0; i < len; i++)
		I915_WRITE(hdmiw_hdmiedid, *((uint32_t *)eld + i));

	i = I915_READ(aud_cntrl_st2);
	i |= eldv;
	I915_WRITE(aud_cntrl_st2, i);

}

static void ironlake_write_eld(struct drm_connector *connector,
			       struct drm_crtc *crtc,
			       struct drm_display_mode *mode)
>>>>>>> refs/remotes/origin/master
{
	struct drm_i915_private *dev_priv = connector->dev->dev_private;
	uint8_t *eld = connector->eld;
	uint32_t eldv;
	uint32_t i;
	int len;
	int hdmiw_hdmiedid;
	int aud_config;
	int aud_cntl_st;
	int aud_cntrl_st2;
<<<<<<< HEAD

	if (HAS_PCH_IBX(connector->dev)) {
		hdmiw_hdmiedid = IBX_HDMIW_HDMIEDID_A;
		aud_config = IBX_AUD_CONFIG_A;
		aud_cntl_st = IBX_AUD_CNTL_ST_A;
		aud_cntrl_st2 = IBX_AUD_CNTL_ST2;
	} else {
		hdmiw_hdmiedid = CPT_HDMIW_HDMIEDID_A;
		aud_config = CPT_AUD_CONFIG_A;
		aud_cntl_st = CPT_AUD_CNTL_ST_A;
		aud_cntrl_st2 = CPT_AUD_CNTRL_ST2;
	}

	i = to_intel_crtc(crtc)->pipe;
	hdmiw_hdmiedid += i * 0x100;
	aud_cntl_st += i * 0x100;
	aud_config += i * 0x100;

	DRM_DEBUG_DRIVER("ELD on pipe %c\n", pipe_name(i));

	i = I915_READ(aud_cntl_st);
	i = (i >> 29) & 0x3;		/* DIP_Port_Select, 0x1 = PortB */
=======
	int pipe = to_intel_crtc(crtc)->pipe;

	if (HAS_PCH_IBX(connector->dev)) {
		hdmiw_hdmiedid = IBX_HDMIW_HDMIEDID(pipe);
		aud_config = IBX_AUD_CFG(pipe);
		aud_cntl_st = IBX_AUD_CNTL_ST(pipe);
		aud_cntrl_st2 = IBX_AUD_CNTL_ST2;
	} else if (IS_VALLEYVIEW(connector->dev)) {
		hdmiw_hdmiedid = VLV_HDMIW_HDMIEDID(pipe);
		aud_config = VLV_AUD_CFG(pipe);
		aud_cntl_st = VLV_AUD_CNTL_ST(pipe);
		aud_cntrl_st2 = VLV_AUD_CNTL_ST2;
	} else {
		hdmiw_hdmiedid = CPT_HDMIW_HDMIEDID(pipe);
		aud_config = CPT_AUD_CFG(pipe);
		aud_cntl_st = CPT_AUD_CNTL_ST(pipe);
		aud_cntrl_st2 = CPT_AUD_CNTRL_ST2;
	}

	DRM_DEBUG_DRIVER("ELD on pipe %c\n", pipe_name(pipe));

	if (IS_VALLEYVIEW(connector->dev))  {
		struct intel_encoder *intel_encoder;
		struct intel_digital_port *intel_dig_port;

		intel_encoder = intel_attached_encoder(connector);
		intel_dig_port = enc_to_dig_port(&intel_encoder->base);
		i = intel_dig_port->port;
	} else {
		i = I915_READ(aud_cntl_st);
		i = (i >> 29) & DIP_PORT_SEL_MASK;
		/* DIP_Port_Select, 0x1 = PortB */
	}

>>>>>>> refs/remotes/origin/master
	if (!i) {
		DRM_DEBUG_DRIVER("Audio directed to unknown port\n");
		/* operate blindly on all ports */
		eldv = IBX_ELD_VALIDB;
		eldv |= IBX_ELD_VALIDB << 4;
		eldv |= IBX_ELD_VALIDB << 8;
	} else {
<<<<<<< HEAD
		DRM_DEBUG_DRIVER("ELD on port %c\n", 'A' + i);
=======
		DRM_DEBUG_DRIVER("ELD on port %c\n", port_name(i));
>>>>>>> refs/remotes/origin/master
		eldv = IBX_ELD_VALIDB << ((i - 1) * 4);
	}

	if (intel_pipe_has_type(crtc, INTEL_OUTPUT_DISPLAYPORT)) {
		DRM_DEBUG_DRIVER("ELD: DisplayPort detected\n");
		eld[5] |= (1 << 2);	/* Conn_Type, 0x1 = DisplayPort */
		I915_WRITE(aud_config, AUD_CONFIG_N_VALUE_INDEX); /* 0x1 = DP */
<<<<<<< HEAD
	} else
		I915_WRITE(aud_config, 0);
=======
	} else {
		I915_WRITE(aud_config, audio_config_hdmi_pixel_clock(mode));
	}
>>>>>>> refs/remotes/origin/master

	if (intel_eld_uptodate(connector,
			       aud_cntrl_st2, eldv,
			       aud_cntl_st, IBX_ELD_ADDRESS,
			       hdmiw_hdmiedid))
		return;

	i = I915_READ(aud_cntrl_st2);
	i &= ~eldv;
	I915_WRITE(aud_cntrl_st2, i);

	if (!eld[0])
		return;

	i = I915_READ(aud_cntl_st);
	i &= ~IBX_ELD_ADDRESS;
	I915_WRITE(aud_cntl_st, i);

	len = min_t(uint8_t, eld[2], 21);	/* 84 bytes of hw ELD buffer */
	DRM_DEBUG_DRIVER("ELD size %d\n", len);
	for (i = 0; i < len; i++)
		I915_WRITE(hdmiw_hdmiedid, *((uint32_t *)eld + i));

	i = I915_READ(aud_cntrl_st2);
	i |= eldv;
	I915_WRITE(aud_cntrl_st2, i);
}

void intel_write_eld(struct drm_encoder *encoder,
		     struct drm_display_mode *mode)
{
	struct drm_crtc *crtc = encoder->crtc;
	struct drm_connector *connector;
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	connector = drm_select_eld(encoder, mode);
	if (!connector)
		return;

	DRM_DEBUG_DRIVER("ELD on [CONNECTOR:%d:%s], [ENCODER:%d:%s]\n",
			 connector->base.id,
			 drm_get_connector_name(connector),
			 connector->encoder->base.id,
			 drm_get_encoder_name(connector->encoder));

	connector->eld[6] = drm_av_sync_delay(connector, mode) / 2;

	if (dev_priv->display.write_eld)
<<<<<<< HEAD
		dev_priv->display.write_eld(connector, crtc);
}

>>>>>>> refs/remotes/origin/cm-10.0
/** Loads the palette/gamma unit for the CRTC with the prepared values */
void intel_crtc_load_lut(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int palreg = PALETTE(intel_crtc->pipe);
	int i;

	/* The clocks have to be on to load the palette. */
	if (!crtc->enabled || !intel_crtc->active)
		return;

	/* use legacy palette for Ironlake */
	if (HAS_PCH_SPLIT(dev))
		palreg = LGC_PALETTE(intel_crtc->pipe);

	for (i = 0; i < 256; i++) {
		I915_WRITE(palreg + 4 * i,
			   (intel_crtc->lut_r[i] << 16) |
			   (intel_crtc->lut_g[i] << 8) |
			   intel_crtc->lut_b[i]);
	}
=======
		dev_priv->display.write_eld(connector, crtc, mode);
>>>>>>> refs/remotes/origin/master
}

static void i845_update_cursor(struct drm_crtc *crtc, u32 base)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	bool visible = base != 0;
	u32 cntl;

	if (intel_crtc->cursor_visible == visible)
		return;

	cntl = I915_READ(_CURACNTR);
	if (visible) {
		/* On these chipsets we can only modify the base whilst
		 * the cursor is disabled.
		 */
		I915_WRITE(_CURABASE, base);

		cntl &= ~(CURSOR_FORMAT_MASK);
		/* XXX width must be 64, stride 256 => 0x00 << 28 */
		cntl |= CURSOR_ENABLE |
			CURSOR_GAMMA_ENABLE |
			CURSOR_FORMAT_ARGB;
	} else
		cntl &= ~(CURSOR_ENABLE | CURSOR_GAMMA_ENABLE);
	I915_WRITE(_CURACNTR, cntl);

	intel_crtc->cursor_visible = visible;
}

static void i9xx_update_cursor(struct drm_crtc *crtc, u32 base)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	bool visible = base != 0;

	if (intel_crtc->cursor_visible != visible) {
		uint32_t cntl = I915_READ(CURCNTR(pipe));
		if (base) {
			cntl &= ~(CURSOR_MODE | MCURSOR_PIPE_SELECT);
			cntl |= CURSOR_MODE_64_ARGB_AX | MCURSOR_GAMMA_ENABLE;
			cntl |= pipe << 28; /* Connect to correct pipe */
		} else {
			cntl &= ~(CURSOR_MODE | MCURSOR_GAMMA_ENABLE);
			cntl |= CURSOR_MODE_DISABLE;
		}
		I915_WRITE(CURCNTR(pipe), cntl);

		intel_crtc->cursor_visible = visible;
	}
	/* and commit changes on next vblank */
<<<<<<< HEAD
	I915_WRITE(CURBASE(pipe), base);
=======
	POSTING_READ(CURCNTR(pipe));
	I915_WRITE(CURBASE(pipe), base);
	POSTING_READ(CURBASE(pipe));
>>>>>>> refs/remotes/origin/master
}

static void ivb_update_cursor(struct drm_crtc *crtc, u32 base)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	bool visible = base != 0;

	if (intel_crtc->cursor_visible != visible) {
		uint32_t cntl = I915_READ(CURCNTR_IVB(pipe));
		if (base) {
			cntl &= ~CURSOR_MODE;
			cntl |= CURSOR_MODE_64_ARGB_AX | MCURSOR_GAMMA_ENABLE;
		} else {
			cntl &= ~(CURSOR_MODE | MCURSOR_GAMMA_ENABLE);
			cntl |= CURSOR_MODE_DISABLE;
		}
<<<<<<< HEAD
=======
		if (IS_HASWELL(dev) || IS_BROADWELL(dev)) {
			cntl |= CURSOR_PIPE_CSC_ENABLE;
			cntl &= ~CURSOR_TRICKLE_FEED_DISABLE;
		}
>>>>>>> refs/remotes/origin/master
		I915_WRITE(CURCNTR_IVB(pipe), cntl);

		intel_crtc->cursor_visible = visible;
	}
	/* and commit changes on next vblank */
<<<<<<< HEAD
	I915_WRITE(CURBASE_IVB(pipe), base);
=======
	POSTING_READ(CURCNTR_IVB(pipe));
	I915_WRITE(CURBASE_IVB(pipe), base);
	POSTING_READ(CURBASE_IVB(pipe));
>>>>>>> refs/remotes/origin/master
}

/* If no-part of the cursor is visible on the framebuffer, then the GPU may hang... */
static void intel_crtc_update_cursor(struct drm_crtc *crtc,
				     bool on)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	int x = intel_crtc->cursor_x;
	int y = intel_crtc->cursor_y;
<<<<<<< HEAD
	u32 base, pos;
	bool visible;

	pos = 0;

	if (on && crtc->enabled && crtc->fb) {
		base = intel_crtc->cursor_addr;
		if (x > (int) crtc->fb->width)
			base = 0;

		if (y > (int) crtc->fb->height)
			base = 0;
	} else
		base = 0;

	if (x < 0) {
		if (x + intel_crtc->cursor_width < 0)
=======
	u32 base = 0, pos = 0;
	bool visible;

	if (on)
		base = intel_crtc->cursor_addr;

	if (x >= intel_crtc->config.pipe_src_w)
		base = 0;

	if (y >= intel_crtc->config.pipe_src_h)
		base = 0;

	if (x < 0) {
		if (x + intel_crtc->cursor_width <= 0)
>>>>>>> refs/remotes/origin/master
			base = 0;

		pos |= CURSOR_POS_SIGN << CURSOR_X_SHIFT;
		x = -x;
	}
	pos |= x << CURSOR_X_SHIFT;

	if (y < 0) {
<<<<<<< HEAD
		if (y + intel_crtc->cursor_height < 0)
=======
		if (y + intel_crtc->cursor_height <= 0)
>>>>>>> refs/remotes/origin/master
			base = 0;

		pos |= CURSOR_POS_SIGN << CURSOR_Y_SHIFT;
		y = -y;
	}
	pos |= y << CURSOR_Y_SHIFT;

	visible = base != 0;
	if (!visible && !intel_crtc->cursor_visible)
		return;

<<<<<<< HEAD
	if (IS_IVYBRIDGE(dev)) {
=======
	if (IS_IVYBRIDGE(dev) || IS_HASWELL(dev) || IS_BROADWELL(dev)) {
>>>>>>> refs/remotes/origin/master
		I915_WRITE(CURPOS_IVB(pipe), pos);
		ivb_update_cursor(crtc, base);
	} else {
		I915_WRITE(CURPOS(pipe), pos);
		if (IS_845G(dev) || IS_I865G(dev))
			i845_update_cursor(crtc, base);
		else
			i9xx_update_cursor(crtc, base);
	}
<<<<<<< HEAD

	if (visible)
		intel_mark_busy(dev, to_intel_framebuffer(crtc->fb)->obj);
=======
>>>>>>> refs/remotes/origin/master
}

static int intel_crtc_cursor_set(struct drm_crtc *crtc,
				 struct drm_file *file,
				 uint32_t handle,
				 uint32_t width, uint32_t height)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct drm_i915_gem_object *obj;
	uint32_t addr;
	int ret;

<<<<<<< HEAD
	DRM_DEBUG_KMS("\n");

=======
>>>>>>> refs/remotes/origin/master
	/* if we want to turn off the cursor ignore width and height */
	if (!handle) {
		DRM_DEBUG_KMS("cursor off\n");
		addr = 0;
		obj = NULL;
		mutex_lock(&dev->struct_mutex);
		goto finish;
	}

	/* Currently we only support 64x64 cursors */
	if (width != 64 || height != 64) {
		DRM_ERROR("we currently only support 64x64 cursors\n");
		return -EINVAL;
	}

	obj = to_intel_bo(drm_gem_object_lookup(dev, file, handle));
	if (&obj->base == NULL)
		return -ENOENT;

	if (obj->base.size < width * height * 4) {
		DRM_ERROR("buffer is to small\n");
		ret = -ENOMEM;
		goto fail;
	}

	/* we only need to pin inside GTT if cursor is non-phy */
	mutex_lock(&dev->struct_mutex);
	if (!dev_priv->info->cursor_needs_physical) {
<<<<<<< HEAD
=======
		unsigned alignment;

>>>>>>> refs/remotes/origin/master
		if (obj->tiling_mode) {
			DRM_ERROR("cursor cannot be tiled\n");
			ret = -EINVAL;
			goto fail_locked;
		}

<<<<<<< HEAD
<<<<<<< HEAD
		ret = i915_gem_object_pin(obj, PAGE_SIZE, true);
		if (ret) {
			DRM_ERROR("failed to pin cursor bo\n");
			goto fail_locked;
		}

		ret = i915_gem_object_set_to_gtt_domain(obj, 0);
		if (ret) {
			DRM_ERROR("failed to move cursor bo into the GTT\n");
			goto fail_unpin;
=======
		ret = i915_gem_object_pin_to_display_plane(obj, 0, NULL);
		if (ret) {
			DRM_ERROR("failed to move cursor bo into the GTT\n");
			goto fail_locked;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		/* Note that the w/a also requires 2 PTE of padding following
		 * the bo. We currently fill all unused PTE with the shadow
		 * page and so we should always have valid PTE following the
		 * cursor preventing the VT-d warning.
		 */
		alignment = 0;
		if (need_vtd_wa(dev))
			alignment = 64*1024;

		ret = i915_gem_object_pin_to_display_plane(obj, alignment, NULL);
		if (ret) {
			DRM_ERROR("failed to move cursor bo into the GTT\n");
			goto fail_locked;
>>>>>>> refs/remotes/origin/master
		}

		ret = i915_gem_object_put_fence(obj);
		if (ret) {
<<<<<<< HEAD
<<<<<<< HEAD
			DRM_ERROR("failed to move cursor bo into the GTT\n");
=======
			DRM_ERROR("failed to release fence for cursor");
>>>>>>> refs/remotes/origin/cm-10.0
			goto fail_unpin;
		}

		addr = obj->gtt_offset;
=======
			DRM_ERROR("failed to release fence for cursor");
			goto fail_unpin;
		}

		addr = i915_gem_obj_ggtt_offset(obj);
>>>>>>> refs/remotes/origin/master
	} else {
		int align = IS_I830(dev) ? 16 * 1024 : 256;
		ret = i915_gem_attach_phys_object(dev, obj,
						  (intel_crtc->pipe == 0) ? I915_GEM_PHYS_CURSOR_0 : I915_GEM_PHYS_CURSOR_1,
						  align);
		if (ret) {
			DRM_ERROR("failed to attach phys object\n");
			goto fail_locked;
		}
		addr = obj->phys_obj->handle->busaddr;
	}

	if (IS_GEN2(dev))
		I915_WRITE(CURSIZE, (height << 12) | width);

 finish:
	if (intel_crtc->cursor_bo) {
		if (dev_priv->info->cursor_needs_physical) {
			if (intel_crtc->cursor_bo != obj)
				i915_gem_detach_phys_object(dev, intel_crtc->cursor_bo);
		} else
<<<<<<< HEAD
			i915_gem_object_unpin(intel_crtc->cursor_bo);
=======
			i915_gem_object_unpin_from_display_plane(intel_crtc->cursor_bo);
>>>>>>> refs/remotes/origin/master
		drm_gem_object_unreference(&intel_crtc->cursor_bo->base);
	}

	mutex_unlock(&dev->struct_mutex);

	intel_crtc->cursor_addr = addr;
	intel_crtc->cursor_bo = obj;
	intel_crtc->cursor_width = width;
	intel_crtc->cursor_height = height;

<<<<<<< HEAD
	intel_crtc_update_cursor(crtc, true);

	return 0;
fail_unpin:
	i915_gem_object_unpin(obj);
=======
	if (intel_crtc->active)
		intel_crtc_update_cursor(crtc, intel_crtc->cursor_bo != NULL);

	return 0;
fail_unpin:
	i915_gem_object_unpin_from_display_plane(obj);
>>>>>>> refs/remotes/origin/master
fail_locked:
	mutex_unlock(&dev->struct_mutex);
fail:
	drm_gem_object_unreference_unlocked(&obj->base);
	return ret;
}

static int intel_crtc_cursor_move(struct drm_crtc *crtc, int x, int y)
{
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);

<<<<<<< HEAD
	intel_crtc->cursor_x = x;
	intel_crtc->cursor_y = y;

	intel_crtc_update_cursor(crtc, true);
=======
	intel_crtc->cursor_x = clamp_t(int, x, SHRT_MIN, SHRT_MAX);
	intel_crtc->cursor_y = clamp_t(int, y, SHRT_MIN, SHRT_MAX);

	if (intel_crtc->active)
		intel_crtc_update_cursor(crtc, intel_crtc->cursor_bo != NULL);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
/** Sets the color ramps on behalf of RandR */
void intel_crtc_fb_gamma_set(struct drm_crtc *crtc, u16 red, u16 green,
				 u16 blue, int regno)
{
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);

	intel_crtc->lut_r[regno] = red >> 8;
	intel_crtc->lut_g[regno] = green >> 8;
	intel_crtc->lut_b[regno] = blue >> 8;
}

void intel_crtc_fb_gamma_get(struct drm_crtc *crtc, u16 *red, u16 *green,
			     u16 *blue, int regno)
{
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);

	*red = intel_crtc->lut_r[regno] << 8;
	*green = intel_crtc->lut_g[regno] << 8;
	*blue = intel_crtc->lut_b[regno] << 8;
}

=======
>>>>>>> refs/remotes/origin/master
static void intel_crtc_gamma_set(struct drm_crtc *crtc, u16 *red, u16 *green,
				 u16 *blue, uint32_t start, uint32_t size)
{
	int end = (start + size > 256) ? 256 : start + size, i;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);

	for (i = start; i < end; i++) {
		intel_crtc->lut_r[i] = red[i] >> 8;
		intel_crtc->lut_g[i] = green[i] >> 8;
		intel_crtc->lut_b[i] = blue[i] >> 8;
	}

	intel_crtc_load_lut(crtc);
}

<<<<<<< HEAD
/**
 * Get a pipe with a simple mode set on it for doing load-based monitor
 * detection.
 *
 * It will be up to the load-detect code to adjust the pipe as appropriate for
 * its requirements.  The pipe will be connected to no other encoders.
 *
 * Currently this code will only succeed if there is a pipe with no encoders
 * configured for it.  In the future, it could choose to temporarily disable
 * some outputs to free up a pipe for its use.
 *
 * \return crtc, or NULL if no pipes are available.
 */

=======
>>>>>>> refs/remotes/origin/master
/* VESA 640x480x72Hz mode to set on the pipe */
static struct drm_display_mode load_detect_mode = {
	DRM_MODE("640x480", DRM_MODE_TYPE_DEFAULT, 31500, 640, 664,
		 704, 832, 0, 480, 489, 491, 520, 0, DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
};

static struct drm_framebuffer *
intel_framebuffer_create(struct drm_device *dev,
<<<<<<< HEAD
<<<<<<< HEAD
			 struct drm_mode_fb_cmd *mode_cmd,
=======
			 struct drm_mode_fb_cmd2 *mode_cmd,
>>>>>>> refs/remotes/origin/cm-10.0
=======
			 struct drm_mode_fb_cmd2 *mode_cmd,
>>>>>>> refs/remotes/origin/master
			 struct drm_i915_gem_object *obj)
{
	struct intel_framebuffer *intel_fb;
	int ret;

	intel_fb = kzalloc(sizeof(*intel_fb), GFP_KERNEL);
	if (!intel_fb) {
		drm_gem_object_unreference_unlocked(&obj->base);
		return ERR_PTR(-ENOMEM);
	}

<<<<<<< HEAD
	ret = intel_framebuffer_init(dev, intel_fb, mode_cmd, obj);
	if (ret) {
		drm_gem_object_unreference_unlocked(&obj->base);
		kfree(intel_fb);
		return ERR_PTR(ret);
	}

	return &intel_fb->base;
=======
	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		goto err;

	ret = intel_framebuffer_init(dev, intel_fb, mode_cmd, obj);
	mutex_unlock(&dev->struct_mutex);
	if (ret)
		goto err;

	return &intel_fb->base;
err:
	drm_gem_object_unreference_unlocked(&obj->base);
	kfree(intel_fb);

	return ERR_PTR(ret);
>>>>>>> refs/remotes/origin/master
}

static u32
intel_framebuffer_pitch_for_width(int width, int bpp)
{
	u32 pitch = DIV_ROUND_UP(width * bpp, 8);
	return ALIGN(pitch, 64);
}

static u32
intel_framebuffer_size_for_mode(struct drm_display_mode *mode, int bpp)
{
	u32 pitch = intel_framebuffer_pitch_for_width(mode->hdisplay, bpp);
	return ALIGN(pitch * mode->vdisplay, PAGE_SIZE);
}

static struct drm_framebuffer *
intel_framebuffer_create_for_mode(struct drm_device *dev,
				  struct drm_display_mode *mode,
				  int depth, int bpp)
{
	struct drm_i915_gem_object *obj;
<<<<<<< HEAD
<<<<<<< HEAD
	struct drm_mode_fb_cmd mode_cmd;
=======
	struct drm_mode_fb_cmd2 mode_cmd;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct drm_mode_fb_cmd2 mode_cmd = { 0 };
>>>>>>> refs/remotes/origin/master

	obj = i915_gem_alloc_object(dev,
				    intel_framebuffer_size_for_mode(mode, bpp));
	if (obj == NULL)
		return ERR_PTR(-ENOMEM);

	mode_cmd.width = mode->hdisplay;
	mode_cmd.height = mode->vdisplay;
<<<<<<< HEAD
<<<<<<< HEAD
	mode_cmd.depth = depth;
	mode_cmd.bpp = bpp;
	mode_cmd.pitch = intel_framebuffer_pitch_for_width(mode_cmd.width, bpp);
=======
	mode_cmd.pitches[0] = intel_framebuffer_pitch_for_width(mode_cmd.width,
								bpp);
	mode_cmd.pixel_format = drm_mode_legacy_fb_format(bpp, depth);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mode_cmd.pitches[0] = intel_framebuffer_pitch_for_width(mode_cmd.width,
								bpp);
	mode_cmd.pixel_format = drm_mode_legacy_fb_format(bpp, depth);
>>>>>>> refs/remotes/origin/master

	return intel_framebuffer_create(dev, &mode_cmd, obj);
}

static struct drm_framebuffer *
mode_fits_in_fbdev(struct drm_device *dev,
		   struct drm_display_mode *mode)
{
<<<<<<< HEAD
=======
#ifdef CONFIG_DRM_I915_FBDEV
>>>>>>> refs/remotes/origin/master
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_i915_gem_object *obj;
	struct drm_framebuffer *fb;

	if (dev_priv->fbdev == NULL)
		return NULL;

	obj = dev_priv->fbdev->ifb.obj;
	if (obj == NULL)
		return NULL;

	fb = &dev_priv->fbdev->ifb.base;
<<<<<<< HEAD
<<<<<<< HEAD
	if (fb->pitch < intel_framebuffer_pitch_for_width(mode->hdisplay,
							  fb->bits_per_pixel))
		return NULL;

	if (obj->base.size < mode->vdisplay * fb->pitch)
=======
=======
>>>>>>> refs/remotes/origin/master
	if (fb->pitches[0] < intel_framebuffer_pitch_for_width(mode->hdisplay,
							       fb->bits_per_pixel))
		return NULL;

	if (obj->base.size < mode->vdisplay * fb->pitches[0])
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		return NULL;

	return fb;
}

bool intel_get_load_detect_pipe(struct intel_encoder *intel_encoder,
				struct drm_connector *connector,
=======
		return NULL;

	return fb;
#else
	return NULL;
#endif
}

bool intel_get_load_detect_pipe(struct drm_connector *connector,
>>>>>>> refs/remotes/origin/master
				struct drm_display_mode *mode,
				struct intel_load_detect_pipe *old)
{
	struct intel_crtc *intel_crtc;
<<<<<<< HEAD
=======
	struct intel_encoder *intel_encoder =
		intel_attached_encoder(connector);
>>>>>>> refs/remotes/origin/master
	struct drm_crtc *possible_crtc;
	struct drm_encoder *encoder = &intel_encoder->base;
	struct drm_crtc *crtc = NULL;
	struct drm_device *dev = encoder->dev;
<<<<<<< HEAD
	struct drm_framebuffer *old_fb;
=======
	struct drm_framebuffer *fb;
>>>>>>> refs/remotes/origin/master
	int i = -1;

	DRM_DEBUG_KMS("[CONNECTOR:%d:%s], [ENCODER:%d:%s]\n",
		      connector->base.id, drm_get_connector_name(connector),
		      encoder->base.id, drm_get_encoder_name(encoder));

	/*
	 * Algorithm gets a little messy:
	 *
	 *   - if the connector already has an assigned crtc, use it (but make
	 *     sure it's on first)
	 *
	 *   - try to find the first unused crtc that can drive this connector,
	 *     and use that if we find one
	 */

	/* See if we already have a CRTC for this connector */
	if (encoder->crtc) {
		crtc = encoder->crtc;

<<<<<<< HEAD
		intel_crtc = to_intel_crtc(crtc);
		old->dpms_mode = intel_crtc->dpms_mode;
		old->load_detect_temp = false;

		/* Make sure the crtc and connector are running */
		if (intel_crtc->dpms_mode != DRM_MODE_DPMS_ON) {
			struct drm_encoder_helper_funcs *encoder_funcs;
			struct drm_crtc_helper_funcs *crtc_funcs;

			crtc_funcs = crtc->helper_private;
			crtc_funcs->dpms(crtc, DRM_MODE_DPMS_ON);

			encoder_funcs = encoder->helper_private;
			encoder_funcs->dpms(encoder, DRM_MODE_DPMS_ON);
		}
=======
		mutex_lock(&crtc->mutex);

		old->dpms_mode = connector->dpms;
		old->load_detect_temp = false;

		/* Make sure the crtc and connector are running */
		if (connector->dpms != DRM_MODE_DPMS_ON)
			connector->funcs->dpms(connector, DRM_MODE_DPMS_ON);
>>>>>>> refs/remotes/origin/master

		return true;
	}

	/* Find an unused one (if possible) */
	list_for_each_entry(possible_crtc, &dev->mode_config.crtc_list, head) {
		i++;
		if (!(encoder->possible_crtcs & (1 << i)))
			continue;
		if (!possible_crtc->enabled) {
			crtc = possible_crtc;
			break;
		}
	}

	/*
	 * If we didn't find an unused CRTC, don't use any.
	 */
	if (!crtc) {
		DRM_DEBUG_KMS("no pipe available for load-detect\n");
		return false;
	}

<<<<<<< HEAD
	encoder->crtc = crtc;
	connector->encoder = encoder;

	intel_crtc = to_intel_crtc(crtc);
	old->dpms_mode = intel_crtc->dpms_mode;
=======
	mutex_lock(&crtc->mutex);
	intel_encoder->new_crtc = to_intel_crtc(crtc);
	to_intel_connector(connector)->new_encoder = intel_encoder;

	intel_crtc = to_intel_crtc(crtc);
	old->dpms_mode = connector->dpms;
>>>>>>> refs/remotes/origin/master
	old->load_detect_temp = true;
	old->release_fb = NULL;

	if (!mode)
		mode = &load_detect_mode;

<<<<<<< HEAD
	old_fb = crtc->fb;

=======
>>>>>>> refs/remotes/origin/master
	/* We need a framebuffer large enough to accommodate all accesses
	 * that the plane may generate whilst we perform load detection.
	 * We can not rely on the fbcon either being present (we get called
	 * during its initialisation to detect all boot displays, or it may
	 * not even exist) or that it is large enough to satisfy the
	 * requested mode.
	 */
<<<<<<< HEAD
	crtc->fb = mode_fits_in_fbdev(dev, mode);
	if (crtc->fb == NULL) {
		DRM_DEBUG_KMS("creating tmp fb for load-detection\n");
		crtc->fb = intel_framebuffer_create_for_mode(dev, mode, 24, 32);
		old->release_fb = crtc->fb;
	} else
		DRM_DEBUG_KMS("reusing fbdev for load-detection framebuffer\n");
	if (IS_ERR(crtc->fb)) {
		DRM_DEBUG_KMS("failed to allocate framebuffer for load-detection\n");
		crtc->fb = old_fb;
		return false;
	}

	if (!drm_crtc_helper_set_mode(crtc, mode, 0, 0, old_fb)) {
		DRM_DEBUG_KMS("failed to set mode on load-detect pipe\n");
		if (old->release_fb)
			old->release_fb->funcs->destroy(old->release_fb);
		crtc->fb = old_fb;
=======
	fb = mode_fits_in_fbdev(dev, mode);
	if (fb == NULL) {
		DRM_DEBUG_KMS("creating tmp fb for load-detection\n");
		fb = intel_framebuffer_create_for_mode(dev, mode, 24, 32);
		old->release_fb = fb;
	} else
		DRM_DEBUG_KMS("reusing fbdev for load-detection framebuffer\n");
	if (IS_ERR(fb)) {
		DRM_DEBUG_KMS("failed to allocate framebuffer for load-detection\n");
		mutex_unlock(&crtc->mutex);
		return false;
	}

	if (intel_set_mode(crtc, mode, 0, 0, fb)) {
		DRM_DEBUG_KMS("failed to set mode on load-detect pipe\n");
		if (old->release_fb)
			old->release_fb->funcs->destroy(old->release_fb);
		mutex_unlock(&crtc->mutex);
>>>>>>> refs/remotes/origin/master
		return false;
	}

	/* let the connector get through one full cycle before testing */
	intel_wait_for_vblank(dev, intel_crtc->pipe);
<<<<<<< HEAD

	return true;
}

void intel_release_load_detect_pipe(struct intel_encoder *intel_encoder,
				    struct drm_connector *connector,
				    struct intel_load_detect_pipe *old)
{
	struct drm_encoder *encoder = &intel_encoder->base;
	struct drm_device *dev = encoder->dev;
	struct drm_crtc *crtc = encoder->crtc;
	struct drm_encoder_helper_funcs *encoder_funcs = encoder->helper_private;
	struct drm_crtc_helper_funcs *crtc_funcs = crtc->helper_private;
=======
	return true;
}

void intel_release_load_detect_pipe(struct drm_connector *connector,
				    struct intel_load_detect_pipe *old)
{
	struct intel_encoder *intel_encoder =
		intel_attached_encoder(connector);
	struct drm_encoder *encoder = &intel_encoder->base;
	struct drm_crtc *crtc = encoder->crtc;
>>>>>>> refs/remotes/origin/master

	DRM_DEBUG_KMS("[CONNECTOR:%d:%s], [ENCODER:%d:%s]\n",
		      connector->base.id, drm_get_connector_name(connector),
		      encoder->base.id, drm_get_encoder_name(encoder));

	if (old->load_detect_temp) {
<<<<<<< HEAD
		connector->encoder = NULL;
		drm_helper_disable_unused_functions(dev);

		if (old->release_fb)
			old->release_fb->funcs->destroy(old->release_fb);

=======
		to_intel_connector(connector)->new_encoder = NULL;
		intel_encoder->new_crtc = NULL;
		intel_set_mode(crtc, NULL, 0, 0, NULL);

		if (old->release_fb) {
			drm_framebuffer_unregister_private(old->release_fb);
			drm_framebuffer_unreference(old->release_fb);
		}

		mutex_unlock(&crtc->mutex);
>>>>>>> refs/remotes/origin/master
		return;
	}

	/* Switch crtc and encoder back off if necessary */
<<<<<<< HEAD
	if (old->dpms_mode != DRM_MODE_DPMS_ON) {
		encoder_funcs->dpms(encoder, old->dpms_mode);
		crtc_funcs->dpms(crtc, old->dpms_mode);
	}
}

/* Returns the clock of the currently programmed mode of the given pipe. */
static int intel_crtc_clock_get(struct drm_device *dev, struct drm_crtc *crtc)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	u32 dpll = I915_READ(DPLL(pipe));
	u32 fp;
	intel_clock_t clock;

	if ((dpll & DISPLAY_RATE_SELECT_FPA1) == 0)
		fp = I915_READ(FP0(pipe));
	else
		fp = I915_READ(FP1(pipe));
=======
	if (old->dpms_mode != DRM_MODE_DPMS_ON)
		connector->funcs->dpms(connector, old->dpms_mode);

	mutex_unlock(&crtc->mutex);
}

static int i9xx_pll_refclk(struct drm_device *dev,
			   const struct intel_crtc_config *pipe_config)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 dpll = pipe_config->dpll_hw_state.dpll;

	if ((dpll & PLL_REF_INPUT_MASK) == PLLB_REF_INPUT_SPREADSPECTRUMIN)
		return dev_priv->vbt.lvds_ssc_freq * 1000;
	else if (HAS_PCH_SPLIT(dev))
		return 120000;
	else if (!IS_GEN2(dev))
		return 96000;
	else
		return 48000;
}

/* Returns the clock of the currently programmed mode of the given pipe. */
static void i9xx_crtc_clock_get(struct intel_crtc *crtc,
				struct intel_crtc_config *pipe_config)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipe = pipe_config->cpu_transcoder;
	u32 dpll = pipe_config->dpll_hw_state.dpll;
	u32 fp;
	intel_clock_t clock;
	int refclk = i9xx_pll_refclk(dev, pipe_config);

	if ((dpll & DISPLAY_RATE_SELECT_FPA1) == 0)
		fp = pipe_config->dpll_hw_state.fp0;
	else
		fp = pipe_config->dpll_hw_state.fp1;
>>>>>>> refs/remotes/origin/master

	clock.m1 = (fp & FP_M1_DIV_MASK) >> FP_M1_DIV_SHIFT;
	if (IS_PINEVIEW(dev)) {
		clock.n = ffs((fp & FP_N_PINEVIEW_DIV_MASK) >> FP_N_DIV_SHIFT) - 1;
		clock.m2 = (fp & FP_M2_PINEVIEW_DIV_MASK) >> FP_M2_DIV_SHIFT;
	} else {
		clock.n = (fp & FP_N_DIV_MASK) >> FP_N_DIV_SHIFT;
		clock.m2 = (fp & FP_M2_DIV_MASK) >> FP_M2_DIV_SHIFT;
	}

	if (!IS_GEN2(dev)) {
		if (IS_PINEVIEW(dev))
			clock.p1 = ffs((dpll & DPLL_FPA01_P1_POST_DIV_MASK_PINEVIEW) >>
				DPLL_FPA01_P1_POST_DIV_SHIFT_PINEVIEW);
		else
			clock.p1 = ffs((dpll & DPLL_FPA01_P1_POST_DIV_MASK) >>
			       DPLL_FPA01_P1_POST_DIV_SHIFT);

		switch (dpll & DPLL_MODE_MASK) {
		case DPLLB_MODE_DAC_SERIAL:
			clock.p2 = dpll & DPLL_DAC_SERIAL_P2_CLOCK_DIV_5 ?
				5 : 10;
			break;
		case DPLLB_MODE_LVDS:
			clock.p2 = dpll & DPLLB_LVDS_P2_CLOCK_DIV_7 ?
				7 : 14;
			break;
		default:
			DRM_DEBUG_KMS("Unknown DPLL mode %08x in programmed "
				  "mode\n", (int)(dpll & DPLL_MODE_MASK));
<<<<<<< HEAD
			return 0;
		}

		/* XXX: Handle the 100Mhz refclk */
		intel_clock(dev, 96000, &clock);
=======
			return;
		}

		if (IS_PINEVIEW(dev))
			pineview_clock(refclk, &clock);
		else
			i9xx_clock(refclk, &clock);
>>>>>>> refs/remotes/origin/master
	} else {
		bool is_lvds = (pipe == 1) && (I915_READ(LVDS) & LVDS_PORT_EN);

		if (is_lvds) {
			clock.p1 = ffs((dpll & DPLL_FPA01_P1_POST_DIV_MASK_I830_LVDS) >>
				       DPLL_FPA01_P1_POST_DIV_SHIFT);
			clock.p2 = 14;
<<<<<<< HEAD

			if ((dpll & PLL_REF_INPUT_MASK) ==
			    PLLB_REF_INPUT_SPREADSPECTRUMIN) {
				/* XXX: might not be 66MHz */
				intel_clock(dev, 66000, &clock);
			} else
				intel_clock(dev, 48000, &clock);
=======
>>>>>>> refs/remotes/origin/master
		} else {
			if (dpll & PLL_P1_DIVIDE_BY_TWO)
				clock.p1 = 2;
			else {
				clock.p1 = ((dpll & DPLL_FPA01_P1_POST_DIV_MASK_I830) >>
					    DPLL_FPA01_P1_POST_DIV_SHIFT) + 2;
			}
			if (dpll & PLL_P2_DIVIDE_BY_4)
				clock.p2 = 4;
			else
				clock.p2 = 2;
<<<<<<< HEAD

			intel_clock(dev, 48000, &clock);
		}
	}

	/* XXX: It would be nice to validate the clocks, but we can't reuse
	 * i830PllIsValid() because it relies on the xf86_config connector
	 * configuration being accurate, which it isn't necessarily.
	 */

	return clock.dot;
=======
		}

		i9xx_clock(refclk, &clock);
	}

	/*
	 * This value includes pixel_multiplier. We will use
	 * port_clock to compute adjusted_mode.crtc_clock in the
	 * encoder's get_config() function.
	 */
	pipe_config->port_clock = clock.dot;
}

int intel_dotclock_calculate(int link_freq,
			     const struct intel_link_m_n *m_n)
{
	/*
	 * The calculation for the data clock is:
	 * pixel_clock = ((m/n)*(link_clock * nr_lanes))/bpp
	 * But we want to avoid losing precison if possible, so:
	 * pixel_clock = ((m * link_clock * nr_lanes)/(n*bpp))
	 *
	 * and the link clock is simpler:
	 * link_clock = (m * link_clock) / n
	 */

	if (!m_n->link_n)
		return 0;

	return div_u64((u64)m_n->link_m * link_freq, m_n->link_n);
}

static void ironlake_pch_clock_get(struct intel_crtc *crtc,
				   struct intel_crtc_config *pipe_config)
{
	struct drm_device *dev = crtc->base.dev;

	/* read out port_clock from the DPLL */
	i9xx_crtc_clock_get(crtc, pipe_config);

	/*
	 * This value does not include pixel_multiplier.
	 * We will check that port_clock and adjusted_mode.crtc_clock
	 * agree once we know their relationship in the encoder's
	 * get_config() function.
	 */
	pipe_config->adjusted_mode.crtc_clock =
		intel_dotclock_calculate(intel_fdi_link_freq(dev) * 10000,
					 &pipe_config->fdi_m_n);
>>>>>>> refs/remotes/origin/master
}

/** Returns the currently programmed mode of the given pipe. */
struct drm_display_mode *intel_crtc_mode_get(struct drm_device *dev,
					     struct drm_crtc *crtc)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
<<<<<<< HEAD
	int pipe = intel_crtc->pipe;
	struct drm_display_mode *mode;
	int htot = I915_READ(HTOTAL(pipe));
	int hsync = I915_READ(HSYNC(pipe));
	int vtot = I915_READ(VTOTAL(pipe));
	int vsync = I915_READ(VSYNC(pipe));
=======
	enum transcoder cpu_transcoder = intel_crtc->config.cpu_transcoder;
	struct drm_display_mode *mode;
	struct intel_crtc_config pipe_config;
	int htot = I915_READ(HTOTAL(cpu_transcoder));
	int hsync = I915_READ(HSYNC(cpu_transcoder));
	int vtot = I915_READ(VTOTAL(cpu_transcoder));
	int vsync = I915_READ(VSYNC(cpu_transcoder));
	enum pipe pipe = intel_crtc->pipe;
>>>>>>> refs/remotes/origin/master

	mode = kzalloc(sizeof(*mode), GFP_KERNEL);
	if (!mode)
		return NULL;

<<<<<<< HEAD
	mode->clock = intel_crtc_clock_get(dev, crtc);
=======
	/*
	 * Construct a pipe_config sufficient for getting the clock info
	 * back out of crtc_clock_get.
	 *
	 * Note, if LVDS ever uses a non-1 pixel multiplier, we'll need
	 * to use a real value here instead.
	 */
	pipe_config.cpu_transcoder = (enum transcoder) pipe;
	pipe_config.pixel_multiplier = 1;
	pipe_config.dpll_hw_state.dpll = I915_READ(DPLL(pipe));
	pipe_config.dpll_hw_state.fp0 = I915_READ(FP0(pipe));
	pipe_config.dpll_hw_state.fp1 = I915_READ(FP1(pipe));
	i9xx_crtc_clock_get(intel_crtc, &pipe_config);

	mode->clock = pipe_config.port_clock / pipe_config.pixel_multiplier;
>>>>>>> refs/remotes/origin/master
	mode->hdisplay = (htot & 0xffff) + 1;
	mode->htotal = ((htot & 0xffff0000) >> 16) + 1;
	mode->hsync_start = (hsync & 0xffff) + 1;
	mode->hsync_end = ((hsync & 0xffff0000) >> 16) + 1;
	mode->vdisplay = (vtot & 0xffff) + 1;
	mode->vtotal = ((vtot & 0xffff0000) >> 16) + 1;
	mode->vsync_start = (vsync & 0xffff) + 1;
	mode->vsync_end = ((vsync & 0xffff0000) >> 16) + 1;

	drm_mode_set_name(mode);
<<<<<<< HEAD
	drm_mode_set_crtcinfo(mode, 0);
=======
>>>>>>> refs/remotes/origin/master

	return mode;
}

<<<<<<< HEAD
#define GPU_IDLE_TIMEOUT 500 /* ms */

/* When this timer fires, we've been idle for awhile */
static void intel_gpu_idle_timer(unsigned long arg)
{
	struct drm_device *dev = (struct drm_device *)arg;
	drm_i915_private_t *dev_priv = dev->dev_private;

	if (!list_empty(&dev_priv->mm.active_list)) {
		/* Still processing requests, so just re-arm the timer. */
		mod_timer(&dev_priv->idle_timer, jiffies +
			  msecs_to_jiffies(GPU_IDLE_TIMEOUT));
		return;
	}

	dev_priv->busy = false;
	queue_work(dev_priv->wq, &dev_priv->idle_work);
}

#define CRTC_IDLE_TIMEOUT 1000 /* ms */

static void intel_crtc_idle_timer(unsigned long arg)
{
	struct intel_crtc *intel_crtc = (struct intel_crtc *)arg;
	struct drm_crtc *crtc = &intel_crtc->base;
	drm_i915_private_t *dev_priv = crtc->dev->dev_private;
	struct intel_framebuffer *intel_fb;

	intel_fb = to_intel_framebuffer(crtc->fb);
	if (intel_fb && intel_fb->obj->active) {
		/* The framebuffer is still being accessed by the GPU. */
		mod_timer(&intel_crtc->idle_timer, jiffies +
			  msecs_to_jiffies(CRTC_IDLE_TIMEOUT));
		return;
	}

	intel_crtc->busy = false;
	queue_work(dev_priv->wq, &dev_priv->idle_work);
}

=======
>>>>>>> refs/remotes/origin/master
static void intel_increase_pllclock(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	int dpll_reg = DPLL(pipe);
	int dpll;

	if (HAS_PCH_SPLIT(dev))
		return;

	if (!dev_priv->lvds_downclock_avail)
		return;

	dpll = I915_READ(dpll_reg);
	if (!HAS_PIPE_CXSR(dev) && (dpll & DISPLAY_RATE_SELECT_FPA1)) {
		DRM_DEBUG_DRIVER("upclocking LVDS\n");

<<<<<<< HEAD
<<<<<<< HEAD
		/* Unlock panel regs */
		I915_WRITE(PP_CONTROL,
			   I915_READ(PP_CONTROL) | PANEL_UNLOCK_REGS);
=======
		assert_panel_unlocked(dev_priv, pipe);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		assert_panel_unlocked(dev_priv, pipe);
>>>>>>> refs/remotes/origin/master

		dpll &= ~DISPLAY_RATE_SELECT_FPA1;
		I915_WRITE(dpll_reg, dpll);
		intel_wait_for_vblank(dev, pipe);

		dpll = I915_READ(dpll_reg);
		if (dpll & DISPLAY_RATE_SELECT_FPA1)
			DRM_DEBUG_DRIVER("failed to upclock LVDS!\n");
<<<<<<< HEAD
<<<<<<< HEAD

		/* ...and lock them again */
		I915_WRITE(PP_CONTROL, I915_READ(PP_CONTROL) & 0x3);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/* Schedule downclock */
	mod_timer(&intel_crtc->idle_timer, jiffies +
		  msecs_to_jiffies(CRTC_IDLE_TIMEOUT));
=======
	}
>>>>>>> refs/remotes/origin/master
}

static void intel_decrease_pllclock(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
<<<<<<< HEAD
<<<<<<< HEAD
	int pipe = intel_crtc->pipe;
	int dpll_reg = DPLL(pipe);
	int dpll = I915_READ(dpll_reg);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (HAS_PCH_SPLIT(dev))
		return;

	if (!dev_priv->lvds_downclock_avail)
		return;

	/*
	 * Since this is called by a timer, we should never get here in
	 * the manual case.
	 */
	if (!HAS_PIPE_CXSR(dev) && intel_crtc->lowfreq_avail) {
<<<<<<< HEAD
<<<<<<< HEAD
		DRM_DEBUG_DRIVER("downclocking LVDS\n");

		/* Unlock panel regs */
		I915_WRITE(PP_CONTROL, I915_READ(PP_CONTROL) |
			   PANEL_UNLOCK_REGS);

=======
		int pipe = intel_crtc->pipe;
		int dpll_reg = DPLL(pipe);
		u32 dpll;
=======
		int pipe = intel_crtc->pipe;
		int dpll_reg = DPLL(pipe);
		int dpll;
>>>>>>> refs/remotes/origin/master

		DRM_DEBUG_DRIVER("downclocking LVDS\n");

		assert_panel_unlocked(dev_priv, pipe);

		dpll = I915_READ(dpll_reg);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		dpll |= DISPLAY_RATE_SELECT_FPA1;
		I915_WRITE(dpll_reg, dpll);
		intel_wait_for_vblank(dev, pipe);
		dpll = I915_READ(dpll_reg);
		if (!(dpll & DISPLAY_RATE_SELECT_FPA1))
			DRM_DEBUG_DRIVER("failed to downclock LVDS!\n");
<<<<<<< HEAD
<<<<<<< HEAD

		/* ...and lock them again */
		I915_WRITE(PP_CONTROL, I915_READ(PP_CONTROL) & 0x3);
	}

=======
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * intel_idle_update - adjust clocks for idleness
 * @work: work struct
 *
 * Either the GPU or display (or both) went idle.  Check the busy status
 * here and adjust the CRTC and GPU clocks as necessary.
 */
static void intel_idle_update(struct work_struct *work)
{
	drm_i915_private_t *dev_priv = container_of(work, drm_i915_private_t,
						    idle_work);
	struct drm_device *dev = dev_priv->dev;
	struct drm_crtc *crtc;
	struct intel_crtc *intel_crtc;
=======
	}

}

void intel_mark_busy(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	hsw_package_c8_gpu_busy(dev_priv);
	i915_update_gfx_val(dev_priv);
}

void intel_mark_idle(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_crtc *crtc;

	hsw_package_c8_gpu_idle(dev_priv);
>>>>>>> refs/remotes/origin/master

	if (!i915_powersave)
		return;

<<<<<<< HEAD
	mutex_lock(&dev->struct_mutex);

	i915_update_gfx_val(dev_priv);

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		/* Skip inactive CRTCs */
		if (!crtc->fb)
			continue;

		intel_crtc = to_intel_crtc(crtc);
		if (!intel_crtc->busy)
			intel_decrease_pllclock(crtc);
	}


	mutex_unlock(&dev->struct_mutex);
}

/**
 * intel_mark_busy - mark the GPU and possibly the display busy
 * @dev: drm device
 * @obj: object we're operating on
 *
 * Callers can use this function to indicate that the GPU is busy processing
 * commands.  If @obj matches one of the CRTC objects (i.e. it's a scanout
 * buffer), we'll also mark the display as busy, so we know to increase its
 * clock frequency.
 */
void intel_mark_busy(struct drm_device *dev, struct drm_i915_gem_object *obj)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_crtc *crtc = NULL;
	struct intel_framebuffer *intel_fb;
	struct intel_crtc *intel_crtc;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return;

	if (!dev_priv->busy)
		dev_priv->busy = true;
	else
		mod_timer(&dev_priv->idle_timer, jiffies +
			  msecs_to_jiffies(GPU_IDLE_TIMEOUT));

=======
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		if (!crtc->fb)
			continue;

		intel_decrease_pllclock(crtc);
	}

	if (dev_priv->info->gen >= 6)
		gen6_rps_idle(dev->dev_private);
}

void intel_mark_fb_busy(struct drm_i915_gem_object *obj,
			struct intel_ring_buffer *ring)
{
	struct drm_device *dev = obj->base.dev;
	struct drm_crtc *crtc;

	if (!i915_powersave)
		return;

>>>>>>> refs/remotes/origin/master
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		if (!crtc->fb)
			continue;

<<<<<<< HEAD
		intel_crtc = to_intel_crtc(crtc);
		intel_fb = to_intel_framebuffer(crtc->fb);
		if (intel_fb->obj == obj) {
			if (!intel_crtc->busy) {
				/* Non-busy -> busy, upclock */
				intel_increase_pllclock(crtc);
				intel_crtc->busy = true;
			} else {
				/* Busy -> busy, put off timer */
				mod_timer(&intel_crtc->idle_timer, jiffies +
					  msecs_to_jiffies(CRTC_IDLE_TIMEOUT));
			}
		}
=======
		if (to_intel_framebuffer(crtc->fb)->obj != obj)
			continue;

		intel_increase_pllclock(crtc);
		if (ring && intel_fbc_enabled(dev))
			ring->fbc_dirty = true;
>>>>>>> refs/remotes/origin/master
	}
}

static void intel_crtc_destroy(struct drm_crtc *crtc)
{
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct drm_device *dev = crtc->dev;
	struct intel_unpin_work *work;
	unsigned long flags;

	spin_lock_irqsave(&dev->event_lock, flags);
	work = intel_crtc->unpin_work;
	intel_crtc->unpin_work = NULL;
	spin_unlock_irqrestore(&dev->event_lock, flags);

	if (work) {
		cancel_work_sync(&work->work);
		kfree(work);
	}

<<<<<<< HEAD
=======
	intel_crtc_cursor_set(crtc, NULL, 0, 0, 0);

>>>>>>> refs/remotes/origin/master
	drm_crtc_cleanup(crtc);

	kfree(intel_crtc);
}

static void intel_unpin_work_fn(struct work_struct *__work)
{
	struct intel_unpin_work *work =
		container_of(__work, struct intel_unpin_work, work);
<<<<<<< HEAD

	mutex_lock(&work->dev->struct_mutex);
<<<<<<< HEAD
	i915_gem_object_unpin(work->old_fb_obj);
	drm_gem_object_unreference(&work->pending_flip_obj->base);
	drm_gem_object_unreference(&work->old_fb_obj->base);

=======
=======
	struct drm_device *dev = work->crtc->dev;

	mutex_lock(&dev->struct_mutex);
>>>>>>> refs/remotes/origin/master
	intel_unpin_fb_obj(work->old_fb_obj);
	drm_gem_object_unreference(&work->pending_flip_obj->base);
	drm_gem_object_unreference(&work->old_fb_obj->base);

<<<<<<< HEAD
	intel_update_fbc(work->dev);
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_unlock(&work->dev->struct_mutex);
=======
	intel_update_fbc(dev);
	mutex_unlock(&dev->struct_mutex);

	BUG_ON(atomic_read(&to_intel_crtc(work->crtc)->unpin_work_count) == 0);
	atomic_dec(&to_intel_crtc(work->crtc)->unpin_work_count);

>>>>>>> refs/remotes/origin/master
	kfree(work);
}

static void do_intel_finish_page_flip(struct drm_device *dev,
				      struct drm_crtc *crtc)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_unpin_work *work;
<<<<<<< HEAD
	struct drm_i915_gem_object *obj;
	struct drm_pending_vblank_event *e;
	struct timeval tnow, tvbl;
=======
>>>>>>> refs/remotes/origin/master
	unsigned long flags;

	/* Ignore early vblank irqs */
	if (intel_crtc == NULL)
		return;

<<<<<<< HEAD
	do_gettimeofday(&tnow);

	spin_lock_irqsave(&dev->event_lock, flags);
	work = intel_crtc->unpin_work;
	if (work == NULL || !work->pending) {
=======
	spin_lock_irqsave(&dev->event_lock, flags);
	work = intel_crtc->unpin_work;

	/* Ensure we don't miss a work->pending update ... */
	smp_rmb();

	if (work == NULL || atomic_read(&work->pending) < INTEL_FLIP_COMPLETE) {
>>>>>>> refs/remotes/origin/master
		spin_unlock_irqrestore(&dev->event_lock, flags);
		return;
	}

<<<<<<< HEAD
	intel_crtc->unpin_work = NULL;

	if (work->event) {
		e = work->event;
		e->event.sequence = drm_vblank_count_and_time(dev, intel_crtc->pipe, &tvbl);

		/* Called before vblank count and timestamps have
		 * been updated for the vblank interval of flip
		 * completion? Need to increment vblank count and
		 * add one videorefresh duration to returned timestamp
		 * to account for this. We assume this happened if we
		 * get called over 0.9 frame durations after the last
		 * timestamped vblank.
		 *
		 * This calculation can not be used with vrefresh rates
		 * below 5Hz (10Hz to be on the safe side) without
		 * promoting to 64 integers.
		 */
		if (10 * (timeval_to_ns(&tnow) - timeval_to_ns(&tvbl)) >
		    9 * crtc->framedur_ns) {
			e->event.sequence++;
			tvbl = ns_to_timeval(timeval_to_ns(&tvbl) +
					     crtc->framedur_ns);
		}

		e->event.tv_sec = tvbl.tv_sec;
		e->event.tv_usec = tvbl.tv_usec;

		list_add_tail(&e->base.link,
			      &e->base.file_priv->event_list);
		wake_up_interruptible(&e->base.file_priv->event_wait);
	}
=======
	/* and that the unpin work is consistent wrt ->pending. */
	smp_rmb();

	intel_crtc->unpin_work = NULL;

	if (work->event)
		drm_send_vblank_event(dev, intel_crtc->pipe, work->event);
>>>>>>> refs/remotes/origin/master

	drm_vblank_put(dev, intel_crtc->pipe);

	spin_unlock_irqrestore(&dev->event_lock, flags);

<<<<<<< HEAD
	obj = work->old_fb_obj;

	atomic_clear_mask(1 << intel_crtc->plane,
			  &obj->pending_flip.counter);
<<<<<<< HEAD
	if (atomic_read(&obj->pending_flip) == 0)
		wake_up(&dev_priv->pending_flip_queue);

=======

	wake_up(&dev_priv->pending_flip_queue);
>>>>>>> refs/remotes/origin/cm-10.0
	schedule_work(&work->work);
=======
	wake_up_all(&dev_priv->pending_flip_queue);

	queue_work(dev_priv->wq, &work->work);
>>>>>>> refs/remotes/origin/master

	trace_i915_flip_complete(intel_crtc->plane, work->pending_flip_obj);
}

void intel_finish_page_flip(struct drm_device *dev, int pipe)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_crtc *crtc = dev_priv->pipe_to_crtc_mapping[pipe];

	do_intel_finish_page_flip(dev, crtc);
}

void intel_finish_page_flip_plane(struct drm_device *dev, int plane)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_crtc *crtc = dev_priv->plane_to_crtc_mapping[plane];

	do_intel_finish_page_flip(dev, crtc);
}

void intel_prepare_page_flip(struct drm_device *dev, int plane)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc =
		to_intel_crtc(dev_priv->plane_to_crtc_mapping[plane]);
	unsigned long flags;

<<<<<<< HEAD
	spin_lock_irqsave(&dev->event_lock, flags);
	if (intel_crtc->unpin_work) {
		if ((++intel_crtc->unpin_work->pending) > 1)
			DRM_ERROR("Prepared flip multiple times\n");
	} else {
		DRM_DEBUG_DRIVER("preparing flip with no unpin work?\n");
	}
	spin_unlock_irqrestore(&dev->event_lock, flags);
}

static int intel_gen2_queue_flip(struct drm_device *dev,
				 struct drm_crtc *crtc,
				 struct drm_framebuffer *fb,
				 struct drm_i915_gem_object *obj)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	unsigned long offset;
	u32 flip_mask;
	int ret;

	ret = intel_pin_and_fence_fb_obj(dev, obj, LP_RING(dev_priv));
	if (ret)
<<<<<<< HEAD
		goto out;

	/* Offset into the new buffer for cases of shared fbs between CRTCs */
	offset = crtc->y * fb->pitch + crtc->x * fb->bits_per_pixel/8;

	ret = BEGIN_LP_RING(6);
	if (ret)
		goto out;
=======
		goto err;

	/* Offset into the new buffer for cases of shared fbs between CRTCs */
	offset = crtc->y * fb->pitches[0] + crtc->x * fb->bits_per_pixel/8;

	ret = BEGIN_LP_RING(6);
	if (ret)
		goto err_unpin;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* NB: An MMIO update of the plane base pointer will also
	 * generate a page-flip completion irq, i.e. every modeset
	 * is also accompanied by a spurious intel_prepare_page_flip().
	 */
	spin_lock_irqsave(&dev->event_lock, flags);
	if (intel_crtc->unpin_work)
		atomic_inc_not_zero(&intel_crtc->unpin_work->pending);
	spin_unlock_irqrestore(&dev->event_lock, flags);
}

inline static void intel_mark_page_flip_active(struct intel_crtc *intel_crtc)
{
	/* Ensure that the work item is consistent when activating it ... */
	smp_wmb();
	atomic_set(&intel_crtc->unpin_work->pending, INTEL_FLIP_PENDING);
	/* and that it is marked active as soon as the irq could fire. */
	smp_wmb();
}

static int intel_gen2_queue_flip(struct drm_device *dev,
				 struct drm_crtc *crtc,
				 struct drm_framebuffer *fb,
				 struct drm_i915_gem_object *obj,
				 uint32_t flags)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	u32 flip_mask;
	struct intel_ring_buffer *ring = &dev_priv->ring[RCS];
	int ret;

	ret = intel_pin_and_fence_fb_obj(dev, obj, ring);
	if (ret)
		goto err;

	ret = intel_ring_begin(ring, 6);
	if (ret)
		goto err_unpin;
>>>>>>> refs/remotes/origin/master

	/* Can't queue multiple flips, so wait for the previous
	 * one to finish before executing the next.
	 */
	if (intel_crtc->plane)
		flip_mask = MI_WAIT_FOR_PLANE_B_FLIP;
	else
		flip_mask = MI_WAIT_FOR_PLANE_A_FLIP;
<<<<<<< HEAD
	OUT_RING(MI_WAIT_FOR_EVENT | flip_mask);
	OUT_RING(MI_NOOP);
	OUT_RING(MI_DISPLAY_FLIP |
		 MI_DISPLAY_FLIP_PLANE(intel_crtc->plane));
<<<<<<< HEAD
	OUT_RING(fb->pitch);
	OUT_RING(obj->gtt_offset + offset);
	OUT_RING(MI_NOOP);
	ADVANCE_LP_RING();
out:
=======
	OUT_RING(fb->pitches[0]);
	OUT_RING(obj->gtt_offset + offset);
	OUT_RING(0); /* aux display base address, unused */
	ADVANCE_LP_RING();
=======
	intel_ring_emit(ring, MI_WAIT_FOR_EVENT | flip_mask);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_emit(ring, MI_DISPLAY_FLIP |
			MI_DISPLAY_FLIP_PLANE(intel_crtc->plane));
	intel_ring_emit(ring, fb->pitches[0]);
	intel_ring_emit(ring, i915_gem_obj_ggtt_offset(obj) + intel_crtc->dspaddr_offset);
	intel_ring_emit(ring, 0); /* aux display base address, unused */

	intel_mark_page_flip_active(intel_crtc);
	__intel_ring_advance(ring);
>>>>>>> refs/remotes/origin/master
	return 0;

err_unpin:
	intel_unpin_fb_obj(obj);
err:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return ret;
}

static int intel_gen3_queue_flip(struct drm_device *dev,
				 struct drm_crtc *crtc,
				 struct drm_framebuffer *fb,
<<<<<<< HEAD
				 struct drm_i915_gem_object *obj)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	unsigned long offset;
	u32 flip_mask;
	int ret;

	ret = intel_pin_and_fence_fb_obj(dev, obj, LP_RING(dev_priv));
	if (ret)
<<<<<<< HEAD
		goto out;

	/* Offset into the new buffer for cases of shared fbs between CRTCs */
	offset = crtc->y * fb->pitch + crtc->x * fb->bits_per_pixel/8;

	ret = BEGIN_LP_RING(6);
	if (ret)
		goto out;
=======
		goto err;

	/* Offset into the new buffer for cases of shared fbs between CRTCs */
	offset = crtc->y * fb->pitches[0] + crtc->x * fb->bits_per_pixel/8;

	ret = BEGIN_LP_RING(6);
	if (ret)
		goto err_unpin;
>>>>>>> refs/remotes/origin/cm-10.0
=======
				 struct drm_i915_gem_object *obj,
				 uint32_t flags)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	u32 flip_mask;
	struct intel_ring_buffer *ring = &dev_priv->ring[RCS];
	int ret;

	ret = intel_pin_and_fence_fb_obj(dev, obj, ring);
	if (ret)
		goto err;

	ret = intel_ring_begin(ring, 6);
	if (ret)
		goto err_unpin;
>>>>>>> refs/remotes/origin/master

	if (intel_crtc->plane)
		flip_mask = MI_WAIT_FOR_PLANE_B_FLIP;
	else
		flip_mask = MI_WAIT_FOR_PLANE_A_FLIP;
<<<<<<< HEAD
	OUT_RING(MI_WAIT_FOR_EVENT | flip_mask);
	OUT_RING(MI_NOOP);
	OUT_RING(MI_DISPLAY_FLIP_I915 |
		 MI_DISPLAY_FLIP_PLANE(intel_crtc->plane));
<<<<<<< HEAD
	OUT_RING(fb->pitch);
=======
	OUT_RING(fb->pitches[0]);
>>>>>>> refs/remotes/origin/cm-10.0
	OUT_RING(obj->gtt_offset + offset);
	OUT_RING(MI_NOOP);

	ADVANCE_LP_RING();
<<<<<<< HEAD
out:
=======
=======
	intel_ring_emit(ring, MI_WAIT_FOR_EVENT | flip_mask);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_emit(ring, MI_DISPLAY_FLIP_I915 |
			MI_DISPLAY_FLIP_PLANE(intel_crtc->plane));
	intel_ring_emit(ring, fb->pitches[0]);
	intel_ring_emit(ring, i915_gem_obj_ggtt_offset(obj) + intel_crtc->dspaddr_offset);
	intel_ring_emit(ring, MI_NOOP);

	intel_mark_page_flip_active(intel_crtc);
	__intel_ring_advance(ring);
>>>>>>> refs/remotes/origin/master
	return 0;

err_unpin:
	intel_unpin_fb_obj(obj);
err:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return ret;
}

static int intel_gen4_queue_flip(struct drm_device *dev,
				 struct drm_crtc *crtc,
				 struct drm_framebuffer *fb,
<<<<<<< HEAD
				 struct drm_i915_gem_object *obj)
=======
				 struct drm_i915_gem_object *obj,
				 uint32_t flags)
>>>>>>> refs/remotes/origin/master
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	uint32_t pf, pipesrc;
<<<<<<< HEAD
	int ret;

	ret = intel_pin_and_fence_fb_obj(dev, obj, LP_RING(dev_priv));
	if (ret)
<<<<<<< HEAD
		goto out;

	ret = BEGIN_LP_RING(4);
	if (ret)
		goto out;
=======
		goto err;

	ret = BEGIN_LP_RING(4);
	if (ret)
		goto err_unpin;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct intel_ring_buffer *ring = &dev_priv->ring[RCS];
	int ret;

	ret = intel_pin_and_fence_fb_obj(dev, obj, ring);
	if (ret)
		goto err;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		goto err_unpin;
>>>>>>> refs/remotes/origin/master

	/* i965+ uses the linear or tiled offsets from the
	 * Display Registers (which do not change across a page-flip)
	 * so we need only reprogram the base address.
	 */
<<<<<<< HEAD
	OUT_RING(MI_DISPLAY_FLIP |
		 MI_DISPLAY_FLIP_PLANE(intel_crtc->plane));
<<<<<<< HEAD
	OUT_RING(fb->pitch);
=======
	OUT_RING(fb->pitches[0]);
>>>>>>> refs/remotes/origin/cm-10.0
	OUT_RING(obj->gtt_offset | obj->tiling_mode);
=======
	intel_ring_emit(ring, MI_DISPLAY_FLIP |
			MI_DISPLAY_FLIP_PLANE(intel_crtc->plane));
	intel_ring_emit(ring, fb->pitches[0]);
	intel_ring_emit(ring,
			(i915_gem_obj_ggtt_offset(obj) + intel_crtc->dspaddr_offset) |
			obj->tiling_mode);
>>>>>>> refs/remotes/origin/master

	/* XXX Enabling the panel-fitter across page-flip is so far
	 * untested on non-native modes, so ignore it for now.
	 * pf = I915_READ(pipe == 0 ? PFA_CTL_1 : PFB_CTL_1) & PF_ENABLE;
	 */
	pf = 0;
	pipesrc = I915_READ(PIPESRC(intel_crtc->pipe)) & 0x0fff0fff;
<<<<<<< HEAD
	OUT_RING(pf | pipesrc);
	ADVANCE_LP_RING();
<<<<<<< HEAD
out:
=======
=======
	intel_ring_emit(ring, pf | pipesrc);

	intel_mark_page_flip_active(intel_crtc);
	__intel_ring_advance(ring);
>>>>>>> refs/remotes/origin/master
	return 0;

err_unpin:
	intel_unpin_fb_obj(obj);
err:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return ret;
}

static int intel_gen6_queue_flip(struct drm_device *dev,
				 struct drm_crtc *crtc,
				 struct drm_framebuffer *fb,
<<<<<<< HEAD
				 struct drm_i915_gem_object *obj)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	uint32_t pf, pipesrc;
	int ret;

	ret = intel_pin_and_fence_fb_obj(dev, obj, LP_RING(dev_priv));
	if (ret)
<<<<<<< HEAD
		goto out;

	ret = BEGIN_LP_RING(4);
	if (ret)
		goto out;

	OUT_RING(MI_DISPLAY_FLIP |
		 MI_DISPLAY_FLIP_PLANE(intel_crtc->plane));
	OUT_RING(fb->pitch | obj->tiling_mode);
	OUT_RING(obj->gtt_offset);

	pf = I915_READ(PF_CTL(intel_crtc->pipe)) & PF_ENABLE;
	pipesrc = I915_READ(PIPESRC(intel_crtc->pipe)) & 0x0fff0fff;
	OUT_RING(pf | pipesrc);
	ADVANCE_LP_RING();
out:
=======
		goto err;

	ret = BEGIN_LP_RING(4);
	if (ret)
		goto err_unpin;

	OUT_RING(MI_DISPLAY_FLIP |
		 MI_DISPLAY_FLIP_PLANE(intel_crtc->plane));
	OUT_RING(fb->pitches[0] | obj->tiling_mode);
	OUT_RING(obj->gtt_offset);
=======
				 struct drm_i915_gem_object *obj,
				 uint32_t flags)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_ring_buffer *ring = &dev_priv->ring[RCS];
	uint32_t pf, pipesrc;
	int ret;

	ret = intel_pin_and_fence_fb_obj(dev, obj, ring);
	if (ret)
		goto err;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		goto err_unpin;

	intel_ring_emit(ring, MI_DISPLAY_FLIP |
			MI_DISPLAY_FLIP_PLANE(intel_crtc->plane));
	intel_ring_emit(ring, fb->pitches[0] | obj->tiling_mode);
	intel_ring_emit(ring, i915_gem_obj_ggtt_offset(obj) + intel_crtc->dspaddr_offset);
>>>>>>> refs/remotes/origin/master

	/* Contrary to the suggestions in the documentation,
	 * "Enable Panel Fitter" does not seem to be required when page
	 * flipping with a non-native mode, and worse causes a normal
	 * modeset to fail.
	 * pf = I915_READ(PF_CTL(intel_crtc->pipe)) & PF_ENABLE;
	 */
	pf = 0;
	pipesrc = I915_READ(PIPESRC(intel_crtc->pipe)) & 0x0fff0fff;
<<<<<<< HEAD
	OUT_RING(pf | pipesrc);
	ADVANCE_LP_RING();
=======
	intel_ring_emit(ring, pf | pipesrc);

	intel_mark_page_flip_active(intel_crtc);
	__intel_ring_advance(ring);
>>>>>>> refs/remotes/origin/master
	return 0;

err_unpin:
	intel_unpin_fb_obj(obj);
err:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

/*
 * On gen7 we currently use the blit ring because (in early silicon at least)
 * the render ring doesn't give us interrpts for page flip completion, which
 * means clients will hang after the first flip is queued.  Fortunately the
 * blit ring generates interrupts properly, so use it instead.
 */
static int intel_gen7_queue_flip(struct drm_device *dev,
				 struct drm_crtc *crtc,
				 struct drm_framebuffer *fb,
				 struct drm_i915_gem_object *obj)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_ring_buffer *ring = &dev_priv->ring[BCS];
<<<<<<< HEAD
=======
	uint32_t plane_bit = 0;
>>>>>>> refs/remotes/origin/cm-10.0
	int ret;

	ret = intel_pin_and_fence_fb_obj(dev, obj, ring);
	if (ret)
<<<<<<< HEAD
		goto out;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		goto out;

	intel_ring_emit(ring, MI_DISPLAY_FLIP_I915 | (intel_crtc->plane << 19));
	intel_ring_emit(ring, (fb->pitch | obj->tiling_mode));
	intel_ring_emit(ring, (obj->gtt_offset));
	intel_ring_emit(ring, (MI_NOOP));
	intel_ring_advance(ring);
out:
=======
=======
	return ret;
}

static int intel_gen7_queue_flip(struct drm_device *dev,
				 struct drm_crtc *crtc,
				 struct drm_framebuffer *fb,
				 struct drm_i915_gem_object *obj,
				 uint32_t flags)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_ring_buffer *ring;
	uint32_t plane_bit = 0;
	int len, ret;

	ring = obj->ring;
	if (IS_VALLEYVIEW(dev) || ring == NULL || ring->id != RCS)
		ring = &dev_priv->ring[BCS];

	ret = intel_pin_and_fence_fb_obj(dev, obj, ring);
	if (ret)
>>>>>>> refs/remotes/origin/master
		goto err;

	switch(intel_crtc->plane) {
	case PLANE_A:
		plane_bit = MI_DISPLAY_FLIP_IVB_PLANE_A;
		break;
	case PLANE_B:
		plane_bit = MI_DISPLAY_FLIP_IVB_PLANE_B;
		break;
	case PLANE_C:
		plane_bit = MI_DISPLAY_FLIP_IVB_PLANE_C;
		break;
	default:
		WARN_ONCE(1, "unknown plane in flip command\n");
		ret = -ENODEV;
		goto err_unpin;
	}

<<<<<<< HEAD
	ret = intel_ring_begin(ring, 4);
	if (ret)
		goto err_unpin;

	intel_ring_emit(ring, MI_DISPLAY_FLIP_I915 | plane_bit);
	intel_ring_emit(ring, (fb->pitches[0] | obj->tiling_mode));
	intel_ring_emit(ring, (obj->gtt_offset));
	intel_ring_emit(ring, (MI_NOOP));
	intel_ring_advance(ring);
=======
	len = 4;
	if (ring->id == RCS)
		len += 6;

	ret = intel_ring_begin(ring, len);
	if (ret)
		goto err_unpin;

	/* Unmask the flip-done completion message. Note that the bspec says that
	 * we should do this for both the BCS and RCS, and that we must not unmask
	 * more than one flip event at any time (or ensure that one flip message
	 * can be sent by waiting for flip-done prior to queueing new flips).
	 * Experimentation says that BCS works despite DERRMR masking all
	 * flip-done completion events and that unmasking all planes at once
	 * for the RCS also doesn't appear to drop events. Setting the DERRMR
	 * to zero does lead to lockups within MI_DISPLAY_FLIP.
	 */
	if (ring->id == RCS) {
		intel_ring_emit(ring, MI_LOAD_REGISTER_IMM(1));
		intel_ring_emit(ring, DERRMR);
		intel_ring_emit(ring, ~(DERRMR_PIPEA_PRI_FLIP_DONE |
					DERRMR_PIPEB_PRI_FLIP_DONE |
					DERRMR_PIPEC_PRI_FLIP_DONE));
		intel_ring_emit(ring, MI_STORE_REGISTER_MEM(1) |
				MI_SRM_LRM_GLOBAL_GTT);
		intel_ring_emit(ring, DERRMR);
		intel_ring_emit(ring, ring->scratch.gtt_offset + 256);
	}

	intel_ring_emit(ring, MI_DISPLAY_FLIP_I915 | plane_bit);
	intel_ring_emit(ring, (fb->pitches[0] | obj->tiling_mode));
	intel_ring_emit(ring, i915_gem_obj_ggtt_offset(obj) + intel_crtc->dspaddr_offset);
	intel_ring_emit(ring, (MI_NOOP));

	intel_mark_page_flip_active(intel_crtc);
	__intel_ring_advance(ring);
>>>>>>> refs/remotes/origin/master
	return 0;

err_unpin:
	intel_unpin_fb_obj(obj);
err:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return ret;
}

static int intel_default_queue_flip(struct drm_device *dev,
				    struct drm_crtc *crtc,
				    struct drm_framebuffer *fb,
<<<<<<< HEAD
				    struct drm_i915_gem_object *obj)
=======
				    struct drm_i915_gem_object *obj,
				    uint32_t flags)
>>>>>>> refs/remotes/origin/master
{
	return -ENODEV;
}

static int intel_crtc_page_flip(struct drm_crtc *crtc,
				struct drm_framebuffer *fb,
<<<<<<< HEAD
				struct drm_pending_vblank_event *event)
=======
				struct drm_pending_vblank_event *event,
				uint32_t page_flip_flags)
>>>>>>> refs/remotes/origin/master
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_framebuffer *old_fb = crtc->fb;
	struct drm_i915_gem_object *obj = to_intel_framebuffer(fb)->obj;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_unpin_work *work;
	unsigned long flags;
	int ret;

<<<<<<< HEAD
	work = kzalloc(sizeof *work, GFP_KERNEL);
=======
	/* Can't change pixel format via MI display flips. */
	if (fb->pixel_format != crtc->fb->pixel_format)
		return -EINVAL;

	/*
	 * TILEOFF/LINOFF registers can't be changed via MI display flips.
	 * Note that pitch changes could also affect these register.
	 */
	if (INTEL_INFO(dev)->gen > 3 &&
	    (fb->offsets[0] != crtc->fb->offsets[0] ||
	     fb->pitches[0] != crtc->fb->pitches[0]))
		return -EINVAL;

	work = kzalloc(sizeof(*work), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (work == NULL)
		return -ENOMEM;

	work->event = event;
<<<<<<< HEAD
	work->dev = crtc->dev;
=======
	work->crtc = crtc;
>>>>>>> refs/remotes/origin/master
	work->old_fb_obj = to_intel_framebuffer(old_fb)->obj;
	INIT_WORK(&work->work, intel_unpin_work_fn);

	ret = drm_vblank_get(dev, intel_crtc->pipe);
	if (ret)
		goto free_work;

	/* We borrow the event spin lock for protecting unpin_work */
	spin_lock_irqsave(&dev->event_lock, flags);
	if (intel_crtc->unpin_work) {
		spin_unlock_irqrestore(&dev->event_lock, flags);
		kfree(work);
		drm_vblank_put(dev, intel_crtc->pipe);

		DRM_DEBUG_DRIVER("flip queue: crtc already busy\n");
		return -EBUSY;
	}
	intel_crtc->unpin_work = work;
	spin_unlock_irqrestore(&dev->event_lock, flags);

<<<<<<< HEAD
	mutex_lock(&dev->struct_mutex);

	/* Reference the objects for the scheduled work. */
	drm_gem_object_reference(&work->old_fb_obj->base);
	drm_gem_object_reference(&obj->base);

	crtc->fb = fb;

	work->pending_flip_obj = obj;

	work->enable_stall_check = true;

	/* Block clients from rendering to the new back buffer until
	 * the flip occurs and the object is no longer visible.
	 */
	atomic_add(1 << intel_crtc->plane, &work->old_fb_obj->pending_flip);

	ret = dev_priv->display.queue_flip(dev, crtc, fb, obj);
	if (ret)
		goto cleanup_pending;

<<<<<<< HEAD
=======
	intel_disable_fbc(dev);
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_unlock(&dev->struct_mutex);

	trace_i915_flip_request(intel_crtc->plane, obj);

	return 0;

cleanup_pending:
	atomic_sub(1 << intel_crtc->plane, &work->old_fb_obj->pending_flip);
	crtc->fb = old_fb;
	drm_gem_object_unreference(&work->old_fb_obj->base);
	drm_gem_object_unreference(&obj->base);
	mutex_unlock(&dev->struct_mutex);

	spin_lock_irqsave(&dev->event_lock, flags);
	intel_crtc->unpin_work = NULL;
	spin_unlock_irqrestore(&dev->event_lock, flags);

	drm_vblank_put(dev, intel_crtc->pipe);
free_work:
	kfree(work);

	return ret;
}

static void intel_sanitize_modesetting(struct drm_device *dev,
				       int pipe, int plane)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 reg, val;
<<<<<<< HEAD
	int i;

	/* Clear any frame start delays used for debugging left by the BIOS */
	for_each_pipe(i) {
		reg = PIPECONF(i);
=======

	/* Clear any frame start delays used for debugging left by the BIOS */
	for_each_pipe(pipe) {
		reg = PIPECONF(pipe);
>>>>>>> refs/remotes/origin/cm-10.0
		I915_WRITE(reg, I915_READ(reg) & ~PIPECONF_FRAME_START_DELAY_MASK);
	}

	if (HAS_PCH_SPLIT(dev))
		return;

	/* Who knows what state these registers were left in by the BIOS or
	 * grub?
	 *
	 * If we leave the registers in a conflicting state (e.g. with the
	 * display plane reading from the other pipe than the one we intend
	 * to use) then when we attempt to teardown the active mode, we will
	 * not disable the pipes and planes in the correct order -- leaving
	 * a plane reading from a disabled pipe and possibly leading to
	 * undefined behaviour.
	 */

	reg = DSPCNTR(plane);
	val = I915_READ(reg);

	if ((val & DISPLAY_PLANE_ENABLE) == 0)
		return;
	if (!!(val & DISPPLANE_SEL_PIPE_MASK) == pipe)
		return;

	/* This display plane is active and attached to the other CPU pipe. */
	pipe = !pipe;

	/* Disable the plane and wait for it to stop reading from the pipe. */
	intel_disable_plane(dev_priv, plane, pipe);
	intel_disable_pipe(dev_priv, pipe);
}

static void intel_crtc_reset(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);

	/* Reset flags back to the 'unknown' status so that they
	 * will be correctly set on the initial modeset.
	 */
	intel_crtc->dpms_mode = -1;

	/* We need to fix up any BIOS configuration that conflicts with
	 * our expectations.
	 */
	intel_sanitize_modesetting(dev, intel_crtc->pipe, intel_crtc->plane);
}

static struct drm_crtc_helper_funcs intel_helper_funcs = {
	.dpms = intel_crtc_dpms,
	.mode_fixup = intel_crtc_mode_fixup,
	.mode_set = intel_crtc_mode_set,
	.mode_set_base = intel_pipe_set_base,
	.mode_set_base_atomic = intel_pipe_set_base_atomic,
	.load_lut = intel_crtc_load_lut,
	.disable = intel_crtc_disable,
};

static const struct drm_crtc_funcs intel_crtc_funcs = {
	.reset = intel_crtc_reset,
	.cursor_set = intel_crtc_cursor_set,
	.cursor_move = intel_crtc_cursor_move,
	.gamma_set = intel_crtc_gamma_set,
	.set_config = drm_crtc_helper_set_config,
	.destroy = intel_crtc_destroy,
	.page_flip = intel_crtc_page_flip,
};

static void intel_crtc_init(struct drm_device *dev, int pipe)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc;
	int i;

	intel_crtc = kzalloc(sizeof(struct intel_crtc) + (INTELFB_CONN_LIMIT * sizeof(struct drm_connector *)), GFP_KERNEL);
	if (intel_crtc == NULL)
		return;

	drm_crtc_init(dev, &intel_crtc->base, &intel_crtc_funcs);

	drm_mode_crtc_set_gamma_size(&intel_crtc->base, 256);
	for (i = 0; i < 256; i++) {
		intel_crtc->lut_r[i] = i;
		intel_crtc->lut_g[i] = i;
		intel_crtc->lut_b[i] = i;
	}

	/* Swap pipes & planes for FBC on pre-965 */
	intel_crtc->pipe = pipe;
	intel_crtc->plane = pipe;
	if (IS_MOBILE(dev) && IS_GEN3(dev)) {
		DRM_DEBUG_KMS("swapping pipes & planes for FBC\n");
		intel_crtc->plane = !pipe;
	}

	BUG_ON(pipe >= ARRAY_SIZE(dev_priv->plane_to_crtc_mapping) ||
	       dev_priv->plane_to_crtc_mapping[intel_crtc->plane] != NULL);
	dev_priv->plane_to_crtc_mapping[intel_crtc->plane] = &intel_crtc->base;
	dev_priv->pipe_to_crtc_mapping[intel_crtc->pipe] = &intel_crtc->base;

	intel_crtc_reset(&intel_crtc->base);
	intel_crtc->active = true; /* force the pipe off on setup_init_config */
<<<<<<< HEAD

	if (HAS_PCH_SPLIT(dev)) {
=======
	intel_crtc->bpp = 24; /* default for pre-Ironlake */

	if (HAS_PCH_SPLIT(dev)) {
		if (pipe == 2 && IS_IVYBRIDGE(dev))
			intel_crtc->no_pll = true;
>>>>>>> refs/remotes/origin/cm-10.0
		intel_helper_funcs.prepare = ironlake_crtc_prepare;
		intel_helper_funcs.commit = ironlake_crtc_commit;
	} else {
		intel_helper_funcs.prepare = i9xx_crtc_prepare;
		intel_helper_funcs.commit = i9xx_crtc_commit;
	}

	drm_crtc_helper_add(&intel_crtc->base, &intel_helper_funcs);

	intel_crtc->busy = false;

	setup_timer(&intel_crtc->idle_timer, intel_crtc_idle_timer,
		    (unsigned long)intel_crtc);
}

int intel_get_pipe_from_crtc_id(struct drm_device *dev, void *data,
				struct drm_file *file)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_i915_get_pipe_from_crtc_id *pipe_from_crtc_id = data;
	struct drm_mode_object *drmmode_obj;
	struct intel_crtc *crtc;

	if (!dev_priv) {
		DRM_ERROR("called with no initialization\n");
		return -EINVAL;
	}

	drmmode_obj = drm_mode_object_find(dev, pipe_from_crtc_id->crtc_id,
			DRM_MODE_OBJECT_CRTC);

	if (!drmmode_obj) {
		DRM_ERROR("no such CRTC id\n");
		return -EINVAL;
	}

	crtc = to_intel_crtc(obj_to_crtc(drmmode_obj));
	pipe_from_crtc_id->pipe = crtc->pipe;

	return 0;
}

static int intel_encoder_clones(struct drm_device *dev, int type_mask)
{
	struct intel_encoder *encoder;
	int index_mask = 0;
	int entry = 0;

	list_for_each_entry(encoder, &dev->mode_config.encoder_list, base.head) {
		if (type_mask & encoder->clone_mask)
			index_mask |= (1 << entry);
		entry++;
	}

	return index_mask;
}

static bool has_edp_a(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (!IS_MOBILE(dev))
		return false;

	if ((I915_READ(DP_A) & DP_DETECTED) == 0)
		return false;

	if (IS_GEN5(dev) &&
	    (I915_READ(ILK_DISPLAY_CHICKEN_FUSES) & ILK_eDP_A_DISABLE))
		return false;

	return true;
}

static void intel_setup_outputs(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_encoder *encoder;
	bool dpd_is_edp = false;
<<<<<<< HEAD
	bool has_lvds = false;

	if (IS_MOBILE(dev) && !IS_I830(dev))
		has_lvds = intel_lvds_init(dev);
=======
	bool has_lvds;

	has_lvds = intel_lvds_init(dev);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!has_lvds && !HAS_PCH_SPLIT(dev)) {
		/* disable the panel fitter on everything but LVDS */
		I915_WRITE(PFIT_CONTROL, 0);
	}

	if (HAS_PCH_SPLIT(dev)) {
		dpd_is_edp = intel_dpd_is_edp(dev);

		if (has_edp_a(dev))
			intel_dp_init(dev, DP_A);

		if (dpd_is_edp && (I915_READ(PCH_DP_D) & DP_DETECTED))
			intel_dp_init(dev, PCH_DP_D);
	}

	intel_crt_init(dev);

	if (HAS_PCH_SPLIT(dev)) {
		int found;

		if (I915_READ(HDMIB) & PORT_DETECTED) {
			/* PCH SDVOB multiplex with HDMIB */
			found = intel_sdvo_init(dev, PCH_SDVOB);
			if (!found)
				intel_hdmi_init(dev, HDMIB);
			if (!found && (I915_READ(PCH_DP_B) & DP_DETECTED))
				intel_dp_init(dev, PCH_DP_B);
		}

		if (I915_READ(HDMIC) & PORT_DETECTED)
			intel_hdmi_init(dev, HDMIC);

		if (I915_READ(HDMID) & PORT_DETECTED)
			intel_hdmi_init(dev, HDMID);

		if (I915_READ(PCH_DP_C) & DP_DETECTED)
			intel_dp_init(dev, PCH_DP_C);

		if (!dpd_is_edp && (I915_READ(PCH_DP_D) & DP_DETECTED))
			intel_dp_init(dev, PCH_DP_D);

	} else if (SUPPORTS_DIGITAL_OUTPUTS(dev)) {
		bool found = false;

		if (I915_READ(SDVOB) & SDVO_DETECTED) {
			DRM_DEBUG_KMS("probing SDVOB\n");
			found = intel_sdvo_init(dev, SDVOB);
			if (!found && SUPPORTS_INTEGRATED_HDMI(dev)) {
				DRM_DEBUG_KMS("probing HDMI on SDVOB\n");
				intel_hdmi_init(dev, SDVOB);
			}

			if (!found && SUPPORTS_INTEGRATED_DP(dev)) {
				DRM_DEBUG_KMS("probing DP_B\n");
				intel_dp_init(dev, DP_B);
			}
		}

		/* Before G4X SDVOC doesn't have its own detect register */

		if (I915_READ(SDVOB) & SDVO_DETECTED) {
			DRM_DEBUG_KMS("probing SDVOC\n");
			found = intel_sdvo_init(dev, SDVOC);
		}

		if (!found && (I915_READ(SDVOC) & SDVO_DETECTED)) {

			if (SUPPORTS_INTEGRATED_HDMI(dev)) {
				DRM_DEBUG_KMS("probing HDMI on SDVOC\n");
				intel_hdmi_init(dev, SDVOC);
			}
			if (SUPPORTS_INTEGRATED_DP(dev)) {
				DRM_DEBUG_KMS("probing DP_C\n");
				intel_dp_init(dev, DP_C);
			}
		}

		if (SUPPORTS_INTEGRATED_DP(dev) &&
		    (I915_READ(DP_D) & DP_DETECTED)) {
			DRM_DEBUG_KMS("probing DP_D\n");
			intel_dp_init(dev, DP_D);
		}
	} else if (IS_GEN2(dev))
		intel_dvo_init(dev);

	if (SUPPORTS_TV(dev))
		intel_tv_init(dev);

	list_for_each_entry(encoder, &dev->mode_config.encoder_list, base.head) {
		encoder->base.possible_crtcs = encoder->crtc_mask;
		encoder->base.possible_clones =
			intel_encoder_clones(dev, encoder->clone_mask);
	}

<<<<<<< HEAD
	intel_panel_setup_backlight(dev);

	/* disable all the possible outputs/crtcs before entering KMS mode */
	drm_helper_disable_unused_functions(dev);
=======
	/* disable all the possible outputs/crtcs before entering KMS mode */
	drm_helper_disable_unused_functions(dev);

	if (HAS_PCH_SPLIT(dev))
		ironlake_init_pch_refclk(dev);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void intel_user_framebuffer_destroy(struct drm_framebuffer *fb)
{
	struct intel_framebuffer *intel_fb = to_intel_framebuffer(fb);

	drm_framebuffer_cleanup(fb);
	drm_gem_object_unreference_unlocked(&intel_fb->obj->base);

	kfree(intel_fb);
}

static int intel_user_framebuffer_create_handle(struct drm_framebuffer *fb,
						struct drm_file *file,
						unsigned int *handle)
{
	struct intel_framebuffer *intel_fb = to_intel_framebuffer(fb);
	struct drm_i915_gem_object *obj = intel_fb->obj;

	return drm_gem_handle_create(file, &obj->base, handle);
}

static const struct drm_framebuffer_funcs intel_fb_funcs = {
	.destroy = intel_user_framebuffer_destroy,
	.create_handle = intel_user_framebuffer_create_handle,
};

int intel_framebuffer_init(struct drm_device *dev,
			   struct intel_framebuffer *intel_fb,
<<<<<<< HEAD
			   struct drm_mode_fb_cmd *mode_cmd,
=======
			   struct drm_mode_fb_cmd2 *mode_cmd,
>>>>>>> refs/remotes/origin/cm-10.0
			   struct drm_i915_gem_object *obj)
{
	int ret;

	if (obj->tiling_mode == I915_TILING_Y)
		return -EINVAL;

<<<<<<< HEAD
	if (mode_cmd->pitch & 63)
		return -EINVAL;

	switch (mode_cmd->bpp) {
	case 8:
	case 16:
	case 24:
	case 32:
		break;
	default:
=======
	if (mode_cmd->pitches[0] & 63)
		return -EINVAL;

	switch (mode_cmd->pixel_format) {
	case DRM_FORMAT_RGB332:
	case DRM_FORMAT_RGB565:
	case DRM_FORMAT_XRGB8888:
	case DRM_FORMAT_XBGR8888:
	case DRM_FORMAT_ARGB8888:
	case DRM_FORMAT_XRGB2101010:
	case DRM_FORMAT_ARGB2101010:
		/* RGB formats are common across chipsets */
		break;
	case DRM_FORMAT_YUYV:
	case DRM_FORMAT_UYVY:
	case DRM_FORMAT_YVYU:
	case DRM_FORMAT_VYUY:
		break;
	default:
		DRM_DEBUG_KMS("unsupported pixel format %u\n",
				mode_cmd->pixel_format);
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;
	}

	ret = drm_framebuffer_init(dev, &intel_fb->base, &intel_fb_funcs);
	if (ret) {
		DRM_ERROR("framebuffer init failed %d\n", ret);
		return ret;
	}

	drm_helper_mode_fill_fb_struct(&intel_fb->base, mode_cmd);
	intel_fb->obj = obj;
	return 0;
}

static struct drm_framebuffer *
intel_user_framebuffer_create(struct drm_device *dev,
			      struct drm_file *filp,
<<<<<<< HEAD
			      struct drm_mode_fb_cmd *mode_cmd)
{
	struct drm_i915_gem_object *obj;

	obj = to_intel_bo(drm_gem_object_lookup(dev, filp, mode_cmd->handle));
=======
			      struct drm_mode_fb_cmd2 *mode_cmd)
{
	struct drm_i915_gem_object *obj;

	obj = to_intel_bo(drm_gem_object_lookup(dev, filp,
						mode_cmd->handles[0]));
>>>>>>> refs/remotes/origin/cm-10.0
	if (&obj->base == NULL)
		return ERR_PTR(-ENOENT);

	return intel_framebuffer_create(dev, mode_cmd, obj);
}

static const struct drm_mode_config_funcs intel_mode_funcs = {
	.fb_create = intel_user_framebuffer_create,
	.output_poll_changed = intel_fb_output_poll_changed,
};

static struct drm_i915_gem_object *
intel_alloc_context_page(struct drm_device *dev)
{
	struct drm_i915_gem_object *ctx;
	int ret;

	WARN_ON(!mutex_is_locked(&dev->struct_mutex));

	ctx = i915_gem_alloc_object(dev, 4096);
	if (!ctx) {
		DRM_DEBUG("failed to alloc power context, RC6 disabled\n");
		return NULL;
	}

	ret = i915_gem_object_pin(ctx, 4096, true);
	if (ret) {
		DRM_ERROR("failed to pin power context: %d\n", ret);
		goto err_unref;
	}

	ret = i915_gem_object_set_to_gtt_domain(ctx, 1);
	if (ret) {
		DRM_ERROR("failed to set-domain on power context: %d\n", ret);
		goto err_unpin;
	}

	return ctx;

err_unpin:
	i915_gem_object_unpin(ctx);
err_unref:
	drm_gem_object_unreference(&ctx->base);
	mutex_unlock(&dev->struct_mutex);
	return NULL;
}

bool ironlake_set_drps(struct drm_device *dev, u8 val)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u16 rgvswctl;

	rgvswctl = I915_READ16(MEMSWCTL);
	if (rgvswctl & MEMCTL_CMD_STS) {
		DRM_DEBUG("gpu busy, RCS change rejected\n");
		return false; /* still busy with another command */
	}

	rgvswctl = (MEMCTL_CMD_CHFREQ << MEMCTL_CMD_SHIFT) |
		(val << MEMCTL_FREQ_SHIFT) | MEMCTL_SFCAVM;
	I915_WRITE16(MEMSWCTL, rgvswctl);
	POSTING_READ16(MEMSWCTL);

	rgvswctl |= MEMCTL_CMD_STS;
	I915_WRITE16(MEMSWCTL, rgvswctl);

	return true;
}

void ironlake_enable_drps(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 rgvmodectl = I915_READ(MEMMODECTL);
	u8 fmax, fmin, fstart, vstart;

	/* Enable temp reporting */
	I915_WRITE16(PMMISC, I915_READ(PMMISC) | MCPPCE_EN);
	I915_WRITE16(TSC1, I915_READ(TSC1) | TSE);

	/* 100ms RC evaluation intervals */
	I915_WRITE(RCUPEI, 100000);
	I915_WRITE(RCDNEI, 100000);

	/* Set max/min thresholds to 90ms and 80ms respectively */
	I915_WRITE(RCBMAXAVG, 90000);
	I915_WRITE(RCBMINAVG, 80000);

	I915_WRITE(MEMIHYST, 1);

	/* Set up min, max, and cur for interrupt handling */
	fmax = (rgvmodectl & MEMMODE_FMAX_MASK) >> MEMMODE_FMAX_SHIFT;
	fmin = (rgvmodectl & MEMMODE_FMIN_MASK);
	fstart = (rgvmodectl & MEMMODE_FSTART_MASK) >>
		MEMMODE_FSTART_SHIFT;

	vstart = (I915_READ(PXVFREQ_BASE + (fstart * 4)) & PXVFREQ_PX_MASK) >>
		PXVFREQ_PX_SHIFT;

	dev_priv->fmax = fmax; /* IPS callback will increase this */
	dev_priv->fstart = fstart;

	dev_priv->max_delay = fstart;
	dev_priv->min_delay = fmin;
	dev_priv->cur_delay = fstart;

	DRM_DEBUG_DRIVER("fmax: %d, fmin: %d, fstart: %d\n",
			 fmax, fmin, fstart);

	I915_WRITE(MEMINTREN, MEMINT_CX_SUPR_EN | MEMINT_EVAL_CHG_EN);

	/*
	 * Interrupts will be enabled in ironlake_irq_postinstall
	 */

	I915_WRITE(VIDSTART, vstart);
	POSTING_READ(VIDSTART);

	rgvmodectl |= MEMMODE_SWMODE_EN;
	I915_WRITE(MEMMODECTL, rgvmodectl);

	if (wait_for((I915_READ(MEMSWCTL) & MEMCTL_CMD_STS) == 0, 10))
		DRM_ERROR("stuck trying to change perf mode\n");
	msleep(1);

	ironlake_set_drps(dev, fstart);

	dev_priv->last_count1 = I915_READ(0x112e4) + I915_READ(0x112e8) +
		I915_READ(0x112e0);
	dev_priv->last_time1 = jiffies_to_msecs(jiffies);
	dev_priv->last_count2 = I915_READ(0x112f4);
	getrawmonotonic(&dev_priv->last_time2);
}

void ironlake_disable_drps(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u16 rgvswctl = I915_READ16(MEMSWCTL);

	/* Ack interrupts, disable EFC interrupt */
	I915_WRITE(MEMINTREN, I915_READ(MEMINTREN) & ~MEMINT_EVAL_CHG_EN);
	I915_WRITE(MEMINTRSTS, MEMINT_EVAL_CHG);
	I915_WRITE(DEIER, I915_READ(DEIER) & ~DE_PCU_EVENT);
	I915_WRITE(DEIIR, DE_PCU_EVENT);
	I915_WRITE(DEIMR, I915_READ(DEIMR) | DE_PCU_EVENT);

	/* Go back to the starting frequency */
	ironlake_set_drps(dev, dev_priv->fstart);
	msleep(1);
	rgvswctl |= MEMCTL_CMD_STS;
	I915_WRITE(MEMSWCTL, rgvswctl);
	msleep(1);

}

void gen6_set_rps(struct drm_device *dev, u8 val)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 swreq;

	swreq = (val & 0x3ff) << 25;
	I915_WRITE(GEN6_RPNSWREQ, swreq);
}

void gen6_disable_rps(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	I915_WRITE(GEN6_RPNSWREQ, 1 << 31);
	I915_WRITE(GEN6_PMINTRMSK, 0xffffffff);
	I915_WRITE(GEN6_PMIER, 0);
<<<<<<< HEAD
=======
	/* Complete PM interrupt masking here doesn't race with the rps work
	 * item again unmasking PM interrupts because that is using a different
	 * register (PMIMR) to mask PM interrupts. The only risk is in leaving
	 * stale bits in PMIIR and PMIMR which gen6_enable_rps will clean up. */
>>>>>>> refs/remotes/origin/cm-10.0

	spin_lock_irq(&dev_priv->rps_lock);
	dev_priv->pm_iir = 0;
	spin_unlock_irq(&dev_priv->rps_lock);

	I915_WRITE(GEN6_PMIIR, I915_READ(GEN6_PMIIR));
}

static unsigned long intel_pxfreq(u32 vidfreq)
{
	unsigned long freq;
	int div = (vidfreq & 0x3f0000) >> 16;
	int post = (vidfreq & 0x3000) >> 12;
	int pre = (vidfreq & 0x7);

	if (!pre)
		return 0;

	freq = ((div * 133333) / ((1<<post) * pre));

	return freq;
}

void intel_init_emon(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 lcfuse;
	u8 pxw[16];
	int i;

	/* Disable to program */
	I915_WRITE(ECR, 0);
	POSTING_READ(ECR);

	/* Program energy weights for various events */
	I915_WRITE(SDEW, 0x15040d00);
	I915_WRITE(CSIEW0, 0x007f0000);
	I915_WRITE(CSIEW1, 0x1e220004);
	I915_WRITE(CSIEW2, 0x04000004);

	for (i = 0; i < 5; i++)
		I915_WRITE(PEW + (i * 4), 0);
	for (i = 0; i < 3; i++)
		I915_WRITE(DEW + (i * 4), 0);

	/* Program P-state weights to account for frequency power adjustment */
	for (i = 0; i < 16; i++) {
		u32 pxvidfreq = I915_READ(PXVFREQ_BASE + (i * 4));
		unsigned long freq = intel_pxfreq(pxvidfreq);
		unsigned long vid = (pxvidfreq & PXVFREQ_PX_MASK) >>
			PXVFREQ_PX_SHIFT;
		unsigned long val;

		val = vid * vid;
		val *= (freq / 1000);
		val *= 255;
		val /= (127*127*900);
		if (val > 0xff)
			DRM_ERROR("bad pxval: %ld\n", val);
		pxw[i] = val;
	}
	/* Render standby states get 0 weight */
	pxw[14] = 0;
	pxw[15] = 0;

	for (i = 0; i < 4; i++) {
		u32 val = (pxw[i*4] << 24) | (pxw[(i*4)+1] << 16) |
			(pxw[(i*4)+2] << 8) | (pxw[(i*4)+3]);
		I915_WRITE(PXW + (i * 4), val);
	}

	/* Adjust magic regs to magic values (more experimental results) */
	I915_WRITE(OGW0, 0);
	I915_WRITE(OGW1, 0);
	I915_WRITE(EG0, 0x00007f00);
	I915_WRITE(EG1, 0x0000000e);
	I915_WRITE(EG2, 0x000e0000);
	I915_WRITE(EG3, 0x68000300);
	I915_WRITE(EG4, 0x42000000);
	I915_WRITE(EG5, 0x00140031);
	I915_WRITE(EG6, 0);
	I915_WRITE(EG7, 0);

	for (i = 0; i < 8; i++)
		I915_WRITE(PXWL + (i * 4), 0);

	/* Enable PMON + select events */
	I915_WRITE(ECR, 0x80000019);

	lcfuse = I915_READ(LCFUSE02);

	dev_priv->corr = (lcfuse & LCFUSE_HIV_MASK);
}

<<<<<<< HEAD
=======
static int intel_enable_rc6(struct drm_device *dev)
{
	/*
	 * Respect the kernel parameter if it is set
	 */
	if (i915_enable_rc6 >= 0)
		return i915_enable_rc6;

	/*
	 * Disable RC6 on Ironlake
	 */
	if (INTEL_INFO(dev)->gen == 5)
		return 0;

	/*
	 * Disable rc6 on Sandybridge
	 */
	if (INTEL_INFO(dev)->gen == 6) {
		DRM_DEBUG_DRIVER("Sandybridge: deep RC6 disabled\n");
		return INTEL_RC6_ENABLE;
	}
	DRM_DEBUG_DRIVER("RC6 and deep RC6 enabled\n");
	return (INTEL_RC6_ENABLE | INTEL_RC6p_ENABLE);
}

>>>>>>> refs/remotes/origin/cm-10.0
void gen6_enable_rps(struct drm_i915_private *dev_priv)
{
	u32 rp_state_cap = I915_READ(GEN6_RP_STATE_CAP);
	u32 gt_perf_status = I915_READ(GEN6_GT_PERF_STATUS);
	u32 pcu_mbox, rc6_mask = 0;
<<<<<<< HEAD
	int cur_freq, min_freq, max_freq;
=======
	u32 gtfifodbg;
	int cur_freq, min_freq, max_freq;
	int rc6_mode;
>>>>>>> refs/remotes/origin/cm-10.0
	int i;

	/* Here begins a magic sequence of register writes to enable
	 * auto-downclocking.
	 *
	 * Perhaps there might be some value in exposing these to
	 * userspace...
	 */
	I915_WRITE(GEN6_RC_STATE, 0);
	mutex_lock(&dev_priv->dev->struct_mutex);
<<<<<<< HEAD
=======

	/* Clear the DBG now so we don't confuse earlier errors */
	if ((gtfifodbg = I915_READ(GTFIFODBG))) {
		DRM_ERROR("GT fifo had a previous error %x\n", gtfifodbg);
		I915_WRITE(GTFIFODBG, gtfifodbg);
	}

>>>>>>> refs/remotes/origin/cm-10.0
	gen6_gt_force_wake_get(dev_priv);

	/* disable the counters and set deterministic thresholds */
	I915_WRITE(GEN6_RC_CONTROL, 0);

	I915_WRITE(GEN6_RC1_WAKE_RATE_LIMIT, 1000 << 16);
	I915_WRITE(GEN6_RC6_WAKE_RATE_LIMIT, 40 << 16 | 30);
	I915_WRITE(GEN6_RC6pp_WAKE_RATE_LIMIT, 30);
	I915_WRITE(GEN6_RC_EVALUATION_INTERVAL, 125000);
	I915_WRITE(GEN6_RC_IDLE_HYSTERSIS, 25);

	for (i = 0; i < I915_NUM_RINGS; i++)
		I915_WRITE(RING_MAX_IDLE(dev_priv->ring[i].mmio_base), 10);

	I915_WRITE(GEN6_RC_SLEEP, 0);
	I915_WRITE(GEN6_RC1e_THRESHOLD, 1000);
	I915_WRITE(GEN6_RC6_THRESHOLD, 50000);
<<<<<<< HEAD
	I915_WRITE(GEN6_RC6p_THRESHOLD, 100000);
	I915_WRITE(GEN6_RC6pp_THRESHOLD, 64000); /* unused */

	if (i915_enable_rc6)
		rc6_mask = GEN6_RC_CTL_RC6p_ENABLE |
			GEN6_RC_CTL_RC6_ENABLE;
=======
	I915_WRITE(GEN6_RC6p_THRESHOLD, 150000);
	I915_WRITE(GEN6_RC6pp_THRESHOLD, 64000); /* unused */

	rc6_mode = intel_enable_rc6(dev_priv->dev);
	if (rc6_mode & INTEL_RC6_ENABLE)
		rc6_mask |= GEN6_RC_CTL_RC6_ENABLE;

	if (rc6_mode & INTEL_RC6p_ENABLE)
		rc6_mask |= GEN6_RC_CTL_RC6p_ENABLE;

	if (rc6_mode & INTEL_RC6pp_ENABLE)
		rc6_mask |= GEN6_RC_CTL_RC6pp_ENABLE;

	DRM_INFO("Enabling RC6 states: RC6 %s, RC6p %s, RC6pp %s\n",
			(rc6_mode & INTEL_RC6_ENABLE) ? "on" : "off",
			(rc6_mode & INTEL_RC6p_ENABLE) ? "on" : "off",
			(rc6_mode & INTEL_RC6pp_ENABLE) ? "on" : "off");
>>>>>>> refs/remotes/origin/cm-10.0

	I915_WRITE(GEN6_RC_CONTROL,
		   rc6_mask |
		   GEN6_RC_CTL_EI_MODE(1) |
		   GEN6_RC_CTL_HW_ENABLE);

	I915_WRITE(GEN6_RPNSWREQ,
		   GEN6_FREQUENCY(10) |
		   GEN6_OFFSET(0) |
		   GEN6_AGGRESSIVE_TURBO);
	I915_WRITE(GEN6_RC_VIDEO_FREQ,
		   GEN6_FREQUENCY(12));

	I915_WRITE(GEN6_RP_DOWN_TIMEOUT, 1000000);
	I915_WRITE(GEN6_RP_INTERRUPT_LIMITS,
		   18 << 24 |
		   6 << 16);
	I915_WRITE(GEN6_RP_UP_THRESHOLD, 10000);
	I915_WRITE(GEN6_RP_DOWN_THRESHOLD, 1000000);
	I915_WRITE(GEN6_RP_UP_EI, 100000);
	I915_WRITE(GEN6_RP_DOWN_EI, 5000000);
	I915_WRITE(GEN6_RP_IDLE_HYSTERSIS, 10);
	I915_WRITE(GEN6_RP_CONTROL,
		   GEN6_RP_MEDIA_TURBO |
<<<<<<< HEAD
		   GEN6_RP_USE_NORMAL_FREQ |
=======
		   GEN6_RP_MEDIA_HW_NORMAL_MODE |
>>>>>>> refs/remotes/origin/cm-10.0
		   GEN6_RP_MEDIA_IS_GFX |
		   GEN6_RP_ENABLE |
		   GEN6_RP_UP_BUSY_AVG |
		   GEN6_RP_DOWN_IDLE_CONT);

	if (wait_for((I915_READ(GEN6_PCODE_MAILBOX) & GEN6_PCODE_READY) == 0,
		     500))
		DRM_ERROR("timeout waiting for pcode mailbox to become idle\n");

	I915_WRITE(GEN6_PCODE_DATA, 0);
	I915_WRITE(GEN6_PCODE_MAILBOX,
		   GEN6_PCODE_READY |
		   GEN6_PCODE_WRITE_MIN_FREQ_TABLE);
	if (wait_for((I915_READ(GEN6_PCODE_MAILBOX) & GEN6_PCODE_READY) == 0,
		     500))
		DRM_ERROR("timeout waiting for pcode mailbox to finish\n");

	min_freq = (rp_state_cap & 0xff0000) >> 16;
	max_freq = rp_state_cap & 0xff;
	cur_freq = (gt_perf_status & 0xff00) >> 8;

	/* Check for overclock support */
	if (wait_for((I915_READ(GEN6_PCODE_MAILBOX) & GEN6_PCODE_READY) == 0,
		     500))
		DRM_ERROR("timeout waiting for pcode mailbox to become idle\n");
	I915_WRITE(GEN6_PCODE_MAILBOX, GEN6_READ_OC_PARAMS);
	pcu_mbox = I915_READ(GEN6_PCODE_DATA);
	if (wait_for((I915_READ(GEN6_PCODE_MAILBOX) & GEN6_PCODE_READY) == 0,
		     500))
		DRM_ERROR("timeout waiting for pcode mailbox to finish\n");
	if (pcu_mbox & (1<<31)) { /* OC supported */
		max_freq = pcu_mbox & 0xff;
		DRM_DEBUG_DRIVER("overclocking supported, adjusting frequency max to %dMHz\n", pcu_mbox * 50);
	}

	/* In units of 100MHz */
	dev_priv->max_delay = max_freq;
	dev_priv->min_delay = min_freq;
	dev_priv->cur_delay = cur_freq;

	/* requires MSI enabled */
	I915_WRITE(GEN6_PMIER,
		   GEN6_PM_MBOX_EVENT |
		   GEN6_PM_THERMAL_EVENT |
		   GEN6_PM_RP_DOWN_TIMEOUT |
		   GEN6_PM_RP_UP_THRESHOLD |
		   GEN6_PM_RP_DOWN_THRESHOLD |
		   GEN6_PM_RP_UP_EI_EXPIRED |
		   GEN6_PM_RP_DOWN_EI_EXPIRED);
	spin_lock_irq(&dev_priv->rps_lock);
	WARN_ON(dev_priv->pm_iir != 0);
	I915_WRITE(GEN6_PMIMR, 0);
	spin_unlock_irq(&dev_priv->rps_lock);
	/* enable all PM interrupts */
	I915_WRITE(GEN6_PMINTRMSK, 0);

	gen6_gt_force_wake_put(dev_priv);
	mutex_unlock(&dev_priv->dev->struct_mutex);
}

<<<<<<< HEAD
=======
void gen6_update_ring_freq(struct drm_i915_private *dev_priv)
{
	int min_freq = 15;
	int gpu_freq, ia_freq, max_ia_freq;
	int scaling_factor = 180;

	max_ia_freq = cpufreq_quick_get_max(0);
	/*
	 * Default to measured freq if none found, PCU will ensure we don't go
	 * over
	 */
	if (!max_ia_freq)
		max_ia_freq = tsc_khz;

	/* Convert from kHz to MHz */
	max_ia_freq /= 1000;

	mutex_lock(&dev_priv->dev->struct_mutex);

	/*
	 * For each potential GPU frequency, load a ring frequency we'd like
	 * to use for memory access.  We do this by specifying the IA frequency
	 * the PCU should use as a reference to determine the ring frequency.
	 */
	for (gpu_freq = dev_priv->max_delay; gpu_freq >= dev_priv->min_delay;
	     gpu_freq--) {
		int diff = dev_priv->max_delay - gpu_freq;

		/*
		 * For GPU frequencies less than 750MHz, just use the lowest
		 * ring freq.
		 */
		if (gpu_freq < min_freq)
			ia_freq = 800;
		else
			ia_freq = max_ia_freq - ((diff * scaling_factor) / 2);
		ia_freq = DIV_ROUND_CLOSEST(ia_freq, 100);

		I915_WRITE(GEN6_PCODE_DATA,
			   (ia_freq << GEN6_PCODE_FREQ_IA_RATIO_SHIFT) |
			   gpu_freq);
		I915_WRITE(GEN6_PCODE_MAILBOX, GEN6_PCODE_READY |
			   GEN6_PCODE_WRITE_MIN_FREQ_TABLE);
		if (wait_for((I915_READ(GEN6_PCODE_MAILBOX) &
			      GEN6_PCODE_READY) == 0, 10)) {
			DRM_ERROR("pcode write of freq table timed out\n");
			continue;
		}
	}

	mutex_unlock(&dev_priv->dev->struct_mutex);
}

>>>>>>> refs/remotes/origin/cm-10.0
static void ironlake_init_clock_gating(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t dspclk_gate = VRHUNIT_CLOCK_GATE_DISABLE;

	/* Required for FBC */
	dspclk_gate |= DPFCUNIT_CLOCK_GATE_DISABLE |
		DPFCRUNIT_CLOCK_GATE_DISABLE |
		DPFDUNIT_CLOCK_GATE_DISABLE;
	/* Required for CxSR */
	dspclk_gate |= DPARBUNIT_CLOCK_GATE_DISABLE;

	I915_WRITE(PCH_3DCGDIS0,
		   MARIUNIT_CLOCK_GATE_DISABLE |
		   SVSMUNIT_CLOCK_GATE_DISABLE);
	I915_WRITE(PCH_3DCGDIS1,
		   VFMUNIT_CLOCK_GATE_DISABLE);

	I915_WRITE(PCH_DSPCLK_GATE_D, dspclk_gate);

	/*
	 * According to the spec the following bits should be set in
	 * order to enable memory self-refresh
	 * The bit 22/21 of 0x42004
	 * The bit 5 of 0x42020
	 * The bit 15 of 0x45000
	 */
	I915_WRITE(ILK_DISPLAY_CHICKEN2,
		   (I915_READ(ILK_DISPLAY_CHICKEN2) |
		    ILK_DPARB_GATE | ILK_VSDPFD_FULL));
	I915_WRITE(ILK_DSPCLK_GATE,
		   (I915_READ(ILK_DSPCLK_GATE) |
		    ILK_DPARB_CLK_GATE));
	I915_WRITE(DISP_ARB_CTL,
		   (I915_READ(DISP_ARB_CTL) |
		    DISP_FBC_WM_DIS));
	I915_WRITE(WM3_LP_ILK, 0);
	I915_WRITE(WM2_LP_ILK, 0);
	I915_WRITE(WM1_LP_ILK, 0);

	/*
	 * Based on the document from hardware guys the following bits
	 * should be set unconditionally in order to enable FBC.
	 * The bit 22 of 0x42000
	 * The bit 22 of 0x42004
	 * The bit 7,8,9 of 0x42020.
	 */
	if (IS_IRONLAKE_M(dev)) {
		I915_WRITE(ILK_DISPLAY_CHICKEN1,
			   I915_READ(ILK_DISPLAY_CHICKEN1) |
			   ILK_FBCQ_DIS);
		I915_WRITE(ILK_DISPLAY_CHICKEN2,
			   I915_READ(ILK_DISPLAY_CHICKEN2) |
			   ILK_DPARB_GATE);
		I915_WRITE(ILK_DSPCLK_GATE,
			   I915_READ(ILK_DSPCLK_GATE) |
			   ILK_DPFC_DIS1 |
			   ILK_DPFC_DIS2 |
			   ILK_CLK_FBC);
	}

	I915_WRITE(ILK_DISPLAY_CHICKEN2,
		   I915_READ(ILK_DISPLAY_CHICKEN2) |
		   ILK_ELPIN_409_SELECT);
	I915_WRITE(_3D_CHICKEN2,
		   _3D_CHICKEN2_WM_READ_PIPELINED << 16 |
		   _3D_CHICKEN2_WM_READ_PIPELINED);
}

static void gen6_init_clock_gating(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipe;
	uint32_t dspclk_gate = VRHUNIT_CLOCK_GATE_DISABLE;

	I915_WRITE(PCH_DSPCLK_GATE_D, dspclk_gate);

	I915_WRITE(ILK_DISPLAY_CHICKEN2,
		   I915_READ(ILK_DISPLAY_CHICKEN2) |
		   ILK_ELPIN_409_SELECT);

	/* WaDisableHiZPlanesWhenMSAAEnabled */
	I915_WRITE(_3D_CHICKEN,
		   _MASKED_BIT_ENABLE(_3D_CHICKEN_HIZ_PLANE_DISABLE_MSAA_4X_SNB));

	I915_WRITE(WM3_LP_ILK, 0);
	I915_WRITE(WM2_LP_ILK, 0);
	I915_WRITE(WM1_LP_ILK, 0);

<<<<<<< HEAD
=======
	I915_WRITE(GEN6_UCGCTL1,
		   I915_READ(GEN6_UCGCTL1) |
		   GEN6_BLBUNIT_CLOCK_GATE_DISABLE);

>>>>>>> refs/remotes/origin/cm-10.0
	/* According to the BSpec vol1g, bit 12 (RCPBUNIT) clock
	 * gating disable must be set.  Failure to set it results in
	 * flickering pixels due to Z write ordering failures after
	 * some amount of runtime in the Mesa "fire" demo, and Unigine
	 * Sanctuary and Tropics, and apparently anything else with
	 * alpha test or pixel discard.
	 *
	 * According to the spec, bit 11 (RCCUNIT) must also be set,
	 * but we didn't debug actual testcases to find it out.
	 */
	I915_WRITE(GEN6_UCGCTL2,
		   GEN6_RCPBUNIT_CLOCK_GATE_DISABLE |
		   GEN6_RCCUNIT_CLOCK_GATE_DISABLE);

	/*
	 * According to the spec the following bits should be
	 * set in order to enable memory self-refresh and fbc:
	 * The bit21 and bit22 of 0x42000
	 * The bit21 and bit22 of 0x42004
	 * The bit5 and bit7 of 0x42020
	 * The bit14 of 0x70180
	 * The bit14 of 0x71180
	 */
	I915_WRITE(ILK_DISPLAY_CHICKEN1,
		   I915_READ(ILK_DISPLAY_CHICKEN1) |
		   ILK_FBCQ_DIS | ILK_PABSTRETCH_DIS);
	I915_WRITE(ILK_DISPLAY_CHICKEN2,
		   I915_READ(ILK_DISPLAY_CHICKEN2) |
		   ILK_DPARB_GATE | ILK_VSDPFD_FULL);
	I915_WRITE(ILK_DSPCLK_GATE,
		   I915_READ(ILK_DSPCLK_GATE) |
		   ILK_DPARB_CLK_GATE  |
		   ILK_DPFD_CLK_GATE);

<<<<<<< HEAD
	for_each_pipe(pipe)
		I915_WRITE(DSPCNTR(pipe),
			   I915_READ(DSPCNTR(pipe)) |
			   DISPPLANE_TRICKLE_FEED_DISABLE);
}

static void gen7_setup_fixed_func_scheduler(struct drm_i915_private *dev_priv)
{
	uint32_t reg = I915_READ(GEN7_FF_THREAD_MODE);

	reg &= ~GEN7_FF_SCHED_MASK;
	reg |= GEN7_FF_TS_SCHED_HW;
	reg |= GEN7_FF_VS_SCHED_HW;
	reg |= GEN7_FF_DS_SCHED_HW;

	I915_WRITE(GEN7_FF_THREAD_MODE, reg);
=======
	for_each_pipe(pipe) {
		I915_WRITE(DSPCNTR(pipe),
			   I915_READ(DSPCNTR(pipe)) |
			   DISPPLANE_TRICKLE_FEED_DISABLE);
		intel_flush_display_plane(dev_priv, pipe);
	}

	/* The default value should be 0x200 according to docs, but the two
	 * platforms I checked have a 0 for this. (Maybe BIOS overrides?) */
	I915_WRITE(GEN6_GT_MODE, 0xffff << 16);
	I915_WRITE(GEN6_GT_MODE, GEN6_GT_MODE_HI << 16 | GEN6_GT_MODE_HI);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void ivybridge_init_clock_gating(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipe;
	uint32_t dspclk_gate = VRHUNIT_CLOCK_GATE_DISABLE;

	I915_WRITE(PCH_DSPCLK_GATE_D, dspclk_gate);

	I915_WRITE(WM3_LP_ILK, 0);
	I915_WRITE(WM2_LP_ILK, 0);
	I915_WRITE(WM1_LP_ILK, 0);

	/* According to the spec, bit 13 (RCZUNIT) must be set on IVB.
	 * This implements the WaDisableRCZUnitClockGating workaround.
	 */
	I915_WRITE(GEN6_UCGCTL2, GEN6_RCZUNIT_CLOCK_GATE_DISABLE);

	I915_WRITE(ILK_DSPCLK_GATE, IVB_VRHUNIT_CLK_GATE);

<<<<<<< HEAD
=======
	I915_WRITE(IVB_CHICKEN3,
		   CHICKEN3_DGMG_REQ_OUT_FIX_DISABLE |
		   CHICKEN3_DGMG_DONE_FIX_DISABLE);

>>>>>>> refs/remotes/origin/cm-10.0
	/* Apply the WaDisableRHWOOptimizationForRenderHang workaround. */
	I915_WRITE(GEN7_COMMON_SLICE_CHICKEN1,
		   GEN7_CSC1_RHWO_OPT_DISABLE_IN_RCC);

	/* WaApplyL3ControlAndL3ChickenMode requires those two on Ivy Bridge */
	I915_WRITE(GEN7_L3CNTLREG1,
			GEN7_WA_FOR_GEN7_L3_CONTROL);
	I915_WRITE(GEN7_L3_CHICKEN_MODE_REGISTER,
			GEN7_WA_L3_CHICKEN_MODE);

	/* This is required by WaCatErrorRejectionIssue */
	I915_WRITE(GEN7_SQ_CHICKEN_MBCUNIT_CONFIG,
			I915_READ(GEN7_SQ_CHICKEN_MBCUNIT_CONFIG) |
			GEN7_SQ_CHICKEN_MBCUNIT_SQINTMOB);

<<<<<<< HEAD
	for_each_pipe(pipe)
		I915_WRITE(DSPCNTR(pipe),
			   I915_READ(DSPCNTR(pipe)) |
			   DISPPLANE_TRICKLE_FEED_DISABLE);
=======
	for_each_pipe(pipe) {
		I915_WRITE(DSPCNTR(pipe),
			   I915_READ(DSPCNTR(pipe)) |
			   DISPPLANE_TRICKLE_FEED_DISABLE);
		intel_flush_display_plane(dev_priv, pipe);
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

static void g4x_init_clock_gating(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t dspclk_gate;

	I915_WRITE(RENCLK_GATE_D1, 0);
	I915_WRITE(RENCLK_GATE_D2, VF_UNIT_CLOCK_GATE_DISABLE |
		   GS_UNIT_CLOCK_GATE_DISABLE |
		   CL_UNIT_CLOCK_GATE_DISABLE);
	I915_WRITE(RAMCLK_GATE_D, 0);
	dspclk_gate = VRHUNIT_CLOCK_GATE_DISABLE |
		OVRUNIT_CLOCK_GATE_DISABLE |
		OVCUNIT_CLOCK_GATE_DISABLE;
	if (IS_GM45(dev))
		dspclk_gate |= DSSUNIT_CLOCK_GATE_DISABLE;
	I915_WRITE(DSPCLK_GATE_D, dspclk_gate);
}

static void crestline_init_clock_gating(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	I915_WRITE(RENCLK_GATE_D1, I965_RCC_CLOCK_GATE_DISABLE);
	I915_WRITE(RENCLK_GATE_D2, 0);
	I915_WRITE(DSPCLK_GATE_D, 0);
	I915_WRITE(RAMCLK_GATE_D, 0);
	I915_WRITE16(DEUC, 0);
}

static void broadwater_init_clock_gating(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	I915_WRITE(RENCLK_GATE_D1, I965_RCZ_CLOCK_GATE_DISABLE |
		   I965_RCC_CLOCK_GATE_DISABLE |
		   I965_RCPB_CLOCK_GATE_DISABLE |
		   I965_ISC_CLOCK_GATE_DISABLE |
		   I965_FBC_CLOCK_GATE_DISABLE);
	I915_WRITE(RENCLK_GATE_D2, 0);
}

static void gen3_init_clock_gating(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 dstate = I915_READ(D_STATE);

	dstate |= DSTATE_PLL_D3_OFF | DSTATE_GFX_CLOCK_GATING |
		DSTATE_DOT_CLOCK_GATING;
	I915_WRITE(D_STATE, dstate);
}

static void i85x_init_clock_gating(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	I915_WRITE(RENCLK_GATE_D1, SV_CLOCK_GATE_DISABLE);
}

static void i830_init_clock_gating(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	I915_WRITE(DSPCLK_GATE_D, OVRUNIT_CLOCK_GATE_DISABLE);
}

static void ibx_init_clock_gating(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	/*
	 * On Ibex Peak and Cougar Point, we need to disable clock
	 * gating for the panel power sequencer or it will fail to
	 * start up when no ports are active.
	 */
	I915_WRITE(SOUTH_DSPCLK_GATE_D, PCH_DPLSUNIT_CLOCK_GATE_DISABLE);
}

static void cpt_init_clock_gating(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int pipe;

	/*
	 * On Ibex Peak and Cougar Point, we need to disable clock
	 * gating for the panel power sequencer or it will fail to
	 * start up when no ports are active.
	 */
	I915_WRITE(SOUTH_DSPCLK_GATE_D, PCH_DPLSUNIT_CLOCK_GATE_DISABLE);
	I915_WRITE(SOUTH_CHICKEN2, I915_READ(SOUTH_CHICKEN2) |
		   DPLS_EDP_PPS_FIX_DIS);
	/* Without this, mode sets may fail silently on FDI */
	for_each_pipe(pipe)
		I915_WRITE(TRANS_CHICKEN2(pipe), TRANS_AUTOTRAIN_GEN_STALL_DIS);
}

static void ironlake_teardown_rc6(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (dev_priv->renderctx) {
		i915_gem_object_unpin(dev_priv->renderctx);
		drm_gem_object_unreference(&dev_priv->renderctx->base);
		dev_priv->renderctx = NULL;
	}

	if (dev_priv->pwrctx) {
		i915_gem_object_unpin(dev_priv->pwrctx);
		drm_gem_object_unreference(&dev_priv->pwrctx->base);
		dev_priv->pwrctx = NULL;
	}
<<<<<<< HEAD

	gen7_setup_fixed_func_scheduler(dev_priv);
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

static void ironlake_disable_rc6(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (I915_READ(PWRCTXA)) {
		/* Wake the GPU, prevent RC6, then restore RSTDBYCTL */
		I915_WRITE(RSTDBYCTL, I915_READ(RSTDBYCTL) | RCX_SW_EXIT);
		wait_for(((I915_READ(RSTDBYCTL) & RSX_STATUS_MASK) == RSX_STATUS_ON),
			 50);

		I915_WRITE(PWRCTXA, 0);
		POSTING_READ(PWRCTXA);

		I915_WRITE(RSTDBYCTL, I915_READ(RSTDBYCTL) & ~RCX_SW_EXIT);
		POSTING_READ(RSTDBYCTL);
	}

	ironlake_teardown_rc6(dev);
}

static int ironlake_setup_rc6(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (dev_priv->renderctx == NULL)
		dev_priv->renderctx = intel_alloc_context_page(dev);
	if (!dev_priv->renderctx)
		return -ENOMEM;

	if (dev_priv->pwrctx == NULL)
		dev_priv->pwrctx = intel_alloc_context_page(dev);
	if (!dev_priv->pwrctx) {
		ironlake_teardown_rc6(dev);
		return -ENOMEM;
	}

	return 0;
}

void ironlake_enable_rc6(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret;

	/* rc6 disabled by default due to repeated reports of hanging during
	 * boot and resume.
	 */
<<<<<<< HEAD
	if (!i915_enable_rc6)
=======
	if (!intel_enable_rc6(dev))
>>>>>>> refs/remotes/origin/cm-10.0
		return;

	mutex_lock(&dev->struct_mutex);
	ret = ironlake_setup_rc6(dev);
	if (ret) {
		mutex_unlock(&dev->struct_mutex);
		return;
	}

	/*
	 * GPU can automatically power down the render unit if given a page
	 * to save state.
	 */
	ret = BEGIN_LP_RING(6);
	if (ret) {
		ironlake_teardown_rc6(dev);
		mutex_unlock(&dev->struct_mutex);
		return;
	}

	OUT_RING(MI_SUSPEND_FLUSH | MI_SUSPEND_FLUSH_EN);
	OUT_RING(MI_SET_CONTEXT);
	OUT_RING(dev_priv->renderctx->gtt_offset |
		 MI_MM_SPACE_GTT |
		 MI_SAVE_EXT_STATE_EN |
		 MI_RESTORE_EXT_STATE_EN |
		 MI_RESTORE_INHIBIT);
	OUT_RING(MI_SUSPEND_FLUSH);
	OUT_RING(MI_NOOP);
	OUT_RING(MI_FLUSH);
	ADVANCE_LP_RING();

	/*
	 * Wait for the command parser to advance past MI_SET_CONTEXT. The HW
	 * does an implicit flush, combined with MI_FLUSH above, it should be
	 * safe to assume that renderctx is valid
	 */
	ret = intel_wait_ring_idle(LP_RING(dev_priv));
	if (ret) {
		DRM_ERROR("failed to enable ironlake power power savings\n");
		ironlake_teardown_rc6(dev);
		mutex_unlock(&dev->struct_mutex);
		return;
	}

	I915_WRITE(PWRCTXA, dev_priv->pwrctx->gtt_offset | PWRCTX_EN);
	I915_WRITE(RSTDBYCTL, I915_READ(RSTDBYCTL) & ~RCX_SW_EXIT);
	mutex_unlock(&dev->struct_mutex);
}

void intel_init_clock_gating(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	dev_priv->display.init_clock_gating(dev);

	if (dev_priv->display.init_pch_clock_gating)
		dev_priv->display.init_pch_clock_gating(dev);
}
=======
	if (atomic_read(&intel_crtc->unpin_work_count) >= 2)
		flush_workqueue(dev_priv->wq);

	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		goto cleanup;

	/* Reference the objects for the scheduled work. */
	drm_gem_object_reference(&work->old_fb_obj->base);
	drm_gem_object_reference(&obj->base);

	crtc->fb = fb;

	work->pending_flip_obj = obj;

	work->enable_stall_check = true;

	atomic_inc(&intel_crtc->unpin_work_count);
	intel_crtc->reset_counter = atomic_read(&dev_priv->gpu_error.reset_counter);

	ret = dev_priv->display.queue_flip(dev, crtc, fb, obj, page_flip_flags);
	if (ret)
		goto cleanup_pending;

	intel_disable_fbc(dev);
	intel_mark_fb_busy(obj, NULL);
	mutex_unlock(&dev->struct_mutex);

	trace_i915_flip_request(intel_crtc->plane, obj);

	return 0;

cleanup_pending:
	atomic_dec(&intel_crtc->unpin_work_count);
	crtc->fb = old_fb;
	drm_gem_object_unreference(&work->old_fb_obj->base);
	drm_gem_object_unreference(&obj->base);
	mutex_unlock(&dev->struct_mutex);

cleanup:
	spin_lock_irqsave(&dev->event_lock, flags);
	intel_crtc->unpin_work = NULL;
	spin_unlock_irqrestore(&dev->event_lock, flags);

	drm_vblank_put(dev, intel_crtc->pipe);
free_work:
	kfree(work);

	return ret;
}

static struct drm_crtc_helper_funcs intel_helper_funcs = {
	.mode_set_base_atomic = intel_pipe_set_base_atomic,
	.load_lut = intel_crtc_load_lut,
};

static bool intel_encoder_crtc_ok(struct drm_encoder *encoder,
				  struct drm_crtc *crtc)
{
	struct drm_device *dev;
	struct drm_crtc *tmp;
	int crtc_mask = 1;

	WARN(!crtc, "checking null crtc?\n");

	dev = crtc->dev;

	list_for_each_entry(tmp, &dev->mode_config.crtc_list, head) {
		if (tmp == crtc)
			break;
		crtc_mask <<= 1;
	}

	if (encoder->possible_crtcs & crtc_mask)
		return true;
	return false;
}

/**
 * intel_modeset_update_staged_output_state
 *
 * Updates the staged output configuration state, e.g. after we've read out the
 * current hw state.
 */
static void intel_modeset_update_staged_output_state(struct drm_device *dev)
{
	struct intel_encoder *encoder;
	struct intel_connector *connector;

	list_for_each_entry(connector, &dev->mode_config.connector_list,
			    base.head) {
		connector->new_encoder =
			to_intel_encoder(connector->base.encoder);
	}

	list_for_each_entry(encoder, &dev->mode_config.encoder_list,
			    base.head) {
		encoder->new_crtc =
			to_intel_crtc(encoder->base.crtc);
	}
}

/**
 * intel_modeset_commit_output_state
 *
 * This function copies the stage display pipe configuration to the real one.
 */
static void intel_modeset_commit_output_state(struct drm_device *dev)
{
	struct intel_encoder *encoder;
	struct intel_connector *connector;

	list_for_each_entry(connector, &dev->mode_config.connector_list,
			    base.head) {
		connector->base.encoder = &connector->new_encoder->base;
	}

	list_for_each_entry(encoder, &dev->mode_config.encoder_list,
			    base.head) {
		encoder->base.crtc = &encoder->new_crtc->base;
	}
}

static void
connected_sink_compute_bpp(struct intel_connector * connector,
			   struct intel_crtc_config *pipe_config)
{
	int bpp = pipe_config->pipe_bpp;

	DRM_DEBUG_KMS("[CONNECTOR:%d:%s] checking for sink bpp constrains\n",
		connector->base.base.id,
		drm_get_connector_name(&connector->base));

	/* Don't use an invalid EDID bpc value */
	if (connector->base.display_info.bpc &&
	    connector->base.display_info.bpc * 3 < bpp) {
		DRM_DEBUG_KMS("clamping display bpp (was %d) to EDID reported max of %d\n",
			      bpp, connector->base.display_info.bpc*3);
		pipe_config->pipe_bpp = connector->base.display_info.bpc*3;
	}

	/* Clamp bpp to 8 on screens without EDID 1.4 */
	if (connector->base.display_info.bpc == 0 && bpp > 24) {
		DRM_DEBUG_KMS("clamping display bpp (was %d) to default limit of 24\n",
			      bpp);
		pipe_config->pipe_bpp = 24;
	}
}

static int
compute_baseline_pipe_bpp(struct intel_crtc *crtc,
			  struct drm_framebuffer *fb,
			  struct intel_crtc_config *pipe_config)
{
	struct drm_device *dev = crtc->base.dev;
	struct intel_connector *connector;
	int bpp;

	switch (fb->pixel_format) {
	case DRM_FORMAT_C8:
		bpp = 8*3; /* since we go through a colormap */
		break;
	case DRM_FORMAT_XRGB1555:
	case DRM_FORMAT_ARGB1555:
		/* checked in intel_framebuffer_init already */
		if (WARN_ON(INTEL_INFO(dev)->gen > 3))
			return -EINVAL;
	case DRM_FORMAT_RGB565:
		bpp = 6*3; /* min is 18bpp */
		break;
	case DRM_FORMAT_XBGR8888:
	case DRM_FORMAT_ABGR8888:
		/* checked in intel_framebuffer_init already */
		if (WARN_ON(INTEL_INFO(dev)->gen < 4))
			return -EINVAL;
	case DRM_FORMAT_XRGB8888:
	case DRM_FORMAT_ARGB8888:
		bpp = 8*3;
		break;
	case DRM_FORMAT_XRGB2101010:
	case DRM_FORMAT_ARGB2101010:
	case DRM_FORMAT_XBGR2101010:
	case DRM_FORMAT_ABGR2101010:
		/* checked in intel_framebuffer_init already */
		if (WARN_ON(INTEL_INFO(dev)->gen < 4))
			return -EINVAL;
		bpp = 10*3;
		break;
	/* TODO: gen4+ supports 16 bpc floating point, too. */
	default:
		DRM_DEBUG_KMS("unsupported depth\n");
		return -EINVAL;
	}

	pipe_config->pipe_bpp = bpp;

	/* Clamp display bpp to EDID value */
	list_for_each_entry(connector, &dev->mode_config.connector_list,
			    base.head) {
		if (!connector->new_encoder ||
		    connector->new_encoder->new_crtc != crtc)
			continue;

		connected_sink_compute_bpp(connector, pipe_config);
	}

	return bpp;
}

static void intel_dump_crtc_timings(const struct drm_display_mode *mode)
{
	DRM_DEBUG_KMS("crtc timings: %d %d %d %d %d %d %d %d %d, "
			"type: 0x%x flags: 0x%x\n",
		mode->crtc_clock,
		mode->crtc_hdisplay, mode->crtc_hsync_start,
		mode->crtc_hsync_end, mode->crtc_htotal,
		mode->crtc_vdisplay, mode->crtc_vsync_start,
		mode->crtc_vsync_end, mode->crtc_vtotal, mode->type, mode->flags);
}

static void intel_dump_pipe_config(struct intel_crtc *crtc,
				   struct intel_crtc_config *pipe_config,
				   const char *context)
{
	DRM_DEBUG_KMS("[CRTC:%d]%s config for pipe %c\n", crtc->base.base.id,
		      context, pipe_name(crtc->pipe));

	DRM_DEBUG_KMS("cpu_transcoder: %c\n", transcoder_name(pipe_config->cpu_transcoder));
	DRM_DEBUG_KMS("pipe bpp: %i, dithering: %i\n",
		      pipe_config->pipe_bpp, pipe_config->dither);
	DRM_DEBUG_KMS("fdi/pch: %i, lanes: %i, gmch_m: %u, gmch_n: %u, link_m: %u, link_n: %u, tu: %u\n",
		      pipe_config->has_pch_encoder,
		      pipe_config->fdi_lanes,
		      pipe_config->fdi_m_n.gmch_m, pipe_config->fdi_m_n.gmch_n,
		      pipe_config->fdi_m_n.link_m, pipe_config->fdi_m_n.link_n,
		      pipe_config->fdi_m_n.tu);
	DRM_DEBUG_KMS("dp: %i, gmch_m: %u, gmch_n: %u, link_m: %u, link_n: %u, tu: %u\n",
		      pipe_config->has_dp_encoder,
		      pipe_config->dp_m_n.gmch_m, pipe_config->dp_m_n.gmch_n,
		      pipe_config->dp_m_n.link_m, pipe_config->dp_m_n.link_n,
		      pipe_config->dp_m_n.tu);
	DRM_DEBUG_KMS("requested mode:\n");
	drm_mode_debug_printmodeline(&pipe_config->requested_mode);
	DRM_DEBUG_KMS("adjusted mode:\n");
	drm_mode_debug_printmodeline(&pipe_config->adjusted_mode);
	intel_dump_crtc_timings(&pipe_config->adjusted_mode);
	DRM_DEBUG_KMS("port clock: %d\n", pipe_config->port_clock);
	DRM_DEBUG_KMS("pipe src size: %dx%d\n",
		      pipe_config->pipe_src_w, pipe_config->pipe_src_h);
	DRM_DEBUG_KMS("gmch pfit: control: 0x%08x, ratios: 0x%08x, lvds border: 0x%08x\n",
		      pipe_config->gmch_pfit.control,
		      pipe_config->gmch_pfit.pgm_ratios,
		      pipe_config->gmch_pfit.lvds_border_bits);
	DRM_DEBUG_KMS("pch pfit: pos: 0x%08x, size: 0x%08x, %s\n",
		      pipe_config->pch_pfit.pos,
		      pipe_config->pch_pfit.size,
		      pipe_config->pch_pfit.enabled ? "enabled" : "disabled");
	DRM_DEBUG_KMS("ips: %i\n", pipe_config->ips_enabled);
	DRM_DEBUG_KMS("double wide: %i\n", pipe_config->double_wide);
}

static bool check_encoder_cloning(struct drm_crtc *crtc)
{
	int num_encoders = 0;
	bool uncloneable_encoders = false;
	struct intel_encoder *encoder;

	list_for_each_entry(encoder, &crtc->dev->mode_config.encoder_list,
			    base.head) {
		if (&encoder->new_crtc->base != crtc)
			continue;

		num_encoders++;
		if (!encoder->cloneable)
			uncloneable_encoders = true;
	}

	return !(num_encoders > 1 && uncloneable_encoders);
}

static struct intel_crtc_config *
intel_modeset_pipe_config(struct drm_crtc *crtc,
			  struct drm_framebuffer *fb,
			  struct drm_display_mode *mode)
{
	struct drm_device *dev = crtc->dev;
	struct intel_encoder *encoder;
	struct intel_crtc_config *pipe_config;
	int plane_bpp, ret = -EINVAL;
	bool retry = true;

	if (!check_encoder_cloning(crtc)) {
		DRM_DEBUG_KMS("rejecting invalid cloning configuration\n");
		return ERR_PTR(-EINVAL);
	}

	pipe_config = kzalloc(sizeof(*pipe_config), GFP_KERNEL);
	if (!pipe_config)
		return ERR_PTR(-ENOMEM);

	drm_mode_copy(&pipe_config->adjusted_mode, mode);
	drm_mode_copy(&pipe_config->requested_mode, mode);

	pipe_config->cpu_transcoder =
		(enum transcoder) to_intel_crtc(crtc)->pipe;
	pipe_config->shared_dpll = DPLL_ID_PRIVATE;

	/*
	 * Sanitize sync polarity flags based on requested ones. If neither
	 * positive or negative polarity is requested, treat this as meaning
	 * negative polarity.
	 */
	if (!(pipe_config->adjusted_mode.flags &
	      (DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_NHSYNC)))
		pipe_config->adjusted_mode.flags |= DRM_MODE_FLAG_NHSYNC;

	if (!(pipe_config->adjusted_mode.flags &
	      (DRM_MODE_FLAG_PVSYNC | DRM_MODE_FLAG_NVSYNC)))
		pipe_config->adjusted_mode.flags |= DRM_MODE_FLAG_NVSYNC;

	/* Compute a starting value for pipe_config->pipe_bpp taking the source
	 * plane pixel format and any sink constraints into account. Returns the
	 * source plane bpp so that dithering can be selected on mismatches
	 * after encoders and crtc also have had their say. */
	plane_bpp = compute_baseline_pipe_bpp(to_intel_crtc(crtc),
					      fb, pipe_config);
	if (plane_bpp < 0)
		goto fail;

	/*
	 * Determine the real pipe dimensions. Note that stereo modes can
	 * increase the actual pipe size due to the frame doubling and
	 * insertion of additional space for blanks between the frame. This
	 * is stored in the crtc timings. We use the requested mode to do this
	 * computation to clearly distinguish it from the adjusted mode, which
	 * can be changed by the connectors in the below retry loop.
	 */
	drm_mode_set_crtcinfo(&pipe_config->requested_mode, CRTC_STEREO_DOUBLE);
	pipe_config->pipe_src_w = pipe_config->requested_mode.crtc_hdisplay;
	pipe_config->pipe_src_h = pipe_config->requested_mode.crtc_vdisplay;

encoder_retry:
	/* Ensure the port clock defaults are reset when retrying. */
	pipe_config->port_clock = 0;
	pipe_config->pixel_multiplier = 1;

	/* Fill in default crtc timings, allow encoders to overwrite them. */
	drm_mode_set_crtcinfo(&pipe_config->adjusted_mode, CRTC_STEREO_DOUBLE);

	/* Pass our mode to the connectors and the CRTC to give them a chance to
	 * adjust it according to limitations or connector properties, and also
	 * a chance to reject the mode entirely.
	 */
	list_for_each_entry(encoder, &dev->mode_config.encoder_list,
			    base.head) {

		if (&encoder->new_crtc->base != crtc)
			continue;

		if (!(encoder->compute_config(encoder, pipe_config))) {
			DRM_DEBUG_KMS("Encoder config failure\n");
			goto fail;
		}
	}

	/* Set default port clock if not overwritten by the encoder. Needs to be
	 * done afterwards in case the encoder adjusts the mode. */
	if (!pipe_config->port_clock)
		pipe_config->port_clock = pipe_config->adjusted_mode.crtc_clock
			* pipe_config->pixel_multiplier;

	ret = intel_crtc_compute_config(to_intel_crtc(crtc), pipe_config);
	if (ret < 0) {
		DRM_DEBUG_KMS("CRTC fixup failed\n");
		goto fail;
	}

	if (ret == RETRY) {
		if (WARN(!retry, "loop in pipe configuration computation\n")) {
			ret = -EINVAL;
			goto fail;
		}

		DRM_DEBUG_KMS("CRTC bw constrained, retrying\n");
		retry = false;
		goto encoder_retry;
	}

	pipe_config->dither = pipe_config->pipe_bpp != plane_bpp;
	DRM_DEBUG_KMS("plane bpp: %i, pipe bpp: %i, dithering: %i\n",
		      plane_bpp, pipe_config->pipe_bpp, pipe_config->dither);

	return pipe_config;
fail:
	kfree(pipe_config);
	return ERR_PTR(ret);
}

/* Computes which crtcs are affected and sets the relevant bits in the mask. For
 * simplicity we use the crtc's pipe number (because it's easier to obtain). */
static void
intel_modeset_affected_pipes(struct drm_crtc *crtc, unsigned *modeset_pipes,
			     unsigned *prepare_pipes, unsigned *disable_pipes)
{
	struct intel_crtc *intel_crtc;
	struct drm_device *dev = crtc->dev;
	struct intel_encoder *encoder;
	struct intel_connector *connector;
	struct drm_crtc *tmp_crtc;

	*disable_pipes = *modeset_pipes = *prepare_pipes = 0;

	/* Check which crtcs have changed outputs connected to them, these need
	 * to be part of the prepare_pipes mask. We don't (yet) support global
	 * modeset across multiple crtcs, so modeset_pipes will only have one
	 * bit set at most. */
	list_for_each_entry(connector, &dev->mode_config.connector_list,
			    base.head) {
		if (connector->base.encoder == &connector->new_encoder->base)
			continue;

		if (connector->base.encoder) {
			tmp_crtc = connector->base.encoder->crtc;

			*prepare_pipes |= 1 << to_intel_crtc(tmp_crtc)->pipe;
		}

		if (connector->new_encoder)
			*prepare_pipes |=
				1 << connector->new_encoder->new_crtc->pipe;
	}

	list_for_each_entry(encoder, &dev->mode_config.encoder_list,
			    base.head) {
		if (encoder->base.crtc == &encoder->new_crtc->base)
			continue;

		if (encoder->base.crtc) {
			tmp_crtc = encoder->base.crtc;

			*prepare_pipes |= 1 << to_intel_crtc(tmp_crtc)->pipe;
		}

		if (encoder->new_crtc)
			*prepare_pipes |= 1 << encoder->new_crtc->pipe;
	}

	/* Check for any pipes that will be fully disabled ... */
	list_for_each_entry(intel_crtc, &dev->mode_config.crtc_list,
			    base.head) {
		bool used = false;

		/* Don't try to disable disabled crtcs. */
		if (!intel_crtc->base.enabled)
			continue;

		list_for_each_entry(encoder, &dev->mode_config.encoder_list,
				    base.head) {
			if (encoder->new_crtc == intel_crtc)
				used = true;
		}

		if (!used)
			*disable_pipes |= 1 << intel_crtc->pipe;
	}


	/* set_mode is also used to update properties on life display pipes. */
	intel_crtc = to_intel_crtc(crtc);
	if (crtc->enabled)
		*prepare_pipes |= 1 << intel_crtc->pipe;

	/*
	 * For simplicity do a full modeset on any pipe where the output routing
	 * changed. We could be more clever, but that would require us to be
	 * more careful with calling the relevant encoder->mode_set functions.
	 */
	if (*prepare_pipes)
		*modeset_pipes = *prepare_pipes;

	/* ... and mask these out. */
	*modeset_pipes &= ~(*disable_pipes);
	*prepare_pipes &= ~(*disable_pipes);

	/*
	 * HACK: We don't (yet) fully support global modesets. intel_set_config
	 * obies this rule, but the modeset restore mode of
	 * intel_modeset_setup_hw_state does not.
	 */
	*modeset_pipes &= 1 << intel_crtc->pipe;
	*prepare_pipes &= 1 << intel_crtc->pipe;

	DRM_DEBUG_KMS("set mode pipe masks: modeset: %x, prepare: %x, disable: %x\n",
		      *modeset_pipes, *prepare_pipes, *disable_pipes);
}

static bool intel_crtc_in_use(struct drm_crtc *crtc)
{
	struct drm_encoder *encoder;
	struct drm_device *dev = crtc->dev;

	list_for_each_entry(encoder, &dev->mode_config.encoder_list, head)
		if (encoder->crtc == crtc)
			return true;

	return false;
}

static void
intel_modeset_update_state(struct drm_device *dev, unsigned prepare_pipes)
{
	struct intel_encoder *intel_encoder;
	struct intel_crtc *intel_crtc;
	struct drm_connector *connector;

	list_for_each_entry(intel_encoder, &dev->mode_config.encoder_list,
			    base.head) {
		if (!intel_encoder->base.crtc)
			continue;

		intel_crtc = to_intel_crtc(intel_encoder->base.crtc);

		if (prepare_pipes & (1 << intel_crtc->pipe))
			intel_encoder->connectors_active = false;
	}

	intel_modeset_commit_output_state(dev);

	/* Update computed state. */
	list_for_each_entry(intel_crtc, &dev->mode_config.crtc_list,
			    base.head) {
		intel_crtc->base.enabled = intel_crtc_in_use(&intel_crtc->base);
	}

	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		if (!connector->encoder || !connector->encoder->crtc)
			continue;

		intel_crtc = to_intel_crtc(connector->encoder->crtc);

		if (prepare_pipes & (1 << intel_crtc->pipe)) {
			struct drm_property *dpms_property =
				dev->mode_config.dpms_property;

			connector->dpms = DRM_MODE_DPMS_ON;
			drm_object_property_set_value(&connector->base,
							 dpms_property,
							 DRM_MODE_DPMS_ON);

			intel_encoder = to_intel_encoder(connector->encoder);
			intel_encoder->connectors_active = true;
		}
	}

}

static bool intel_fuzzy_clock_check(int clock1, int clock2)
{
	int diff;

	if (clock1 == clock2)
		return true;

	if (!clock1 || !clock2)
		return false;

	diff = abs(clock1 - clock2);

	if (((((diff + clock1 + clock2) * 100)) / (clock1 + clock2)) < 105)
		return true;

	return false;
}

#define for_each_intel_crtc_masked(dev, mask, intel_crtc) \
	list_for_each_entry((intel_crtc), \
			    &(dev)->mode_config.crtc_list, \
			    base.head) \
		if (mask & (1 <<(intel_crtc)->pipe))

static bool
intel_pipe_config_compare(struct drm_device *dev,
			  struct intel_crtc_config *current_config,
			  struct intel_crtc_config *pipe_config)
{
#define PIPE_CONF_CHECK_X(name)	\
	if (current_config->name != pipe_config->name) { \
		DRM_ERROR("mismatch in " #name " " \
			  "(expected 0x%08x, found 0x%08x)\n", \
			  current_config->name, \
			  pipe_config->name); \
		return false; \
	}

#define PIPE_CONF_CHECK_I(name)	\
	if (current_config->name != pipe_config->name) { \
		DRM_ERROR("mismatch in " #name " " \
			  "(expected %i, found %i)\n", \
			  current_config->name, \
			  pipe_config->name); \
		return false; \
	}

#define PIPE_CONF_CHECK_FLAGS(name, mask)	\
	if ((current_config->name ^ pipe_config->name) & (mask)) { \
		DRM_ERROR("mismatch in " #name "(" #mask ") "	   \
			  "(expected %i, found %i)\n", \
			  current_config->name & (mask), \
			  pipe_config->name & (mask)); \
		return false; \
	}

#define PIPE_CONF_CHECK_CLOCK_FUZZY(name) \
	if (!intel_fuzzy_clock_check(current_config->name, pipe_config->name)) { \
		DRM_ERROR("mismatch in " #name " " \
			  "(expected %i, found %i)\n", \
			  current_config->name, \
			  pipe_config->name); \
		return false; \
	}

#define PIPE_CONF_QUIRK(quirk)	\
	((current_config->quirks | pipe_config->quirks) & (quirk))

	PIPE_CONF_CHECK_I(cpu_transcoder);

	PIPE_CONF_CHECK_I(has_pch_encoder);
	PIPE_CONF_CHECK_I(fdi_lanes);
	PIPE_CONF_CHECK_I(fdi_m_n.gmch_m);
	PIPE_CONF_CHECK_I(fdi_m_n.gmch_n);
	PIPE_CONF_CHECK_I(fdi_m_n.link_m);
	PIPE_CONF_CHECK_I(fdi_m_n.link_n);
	PIPE_CONF_CHECK_I(fdi_m_n.tu);

	PIPE_CONF_CHECK_I(has_dp_encoder);
	PIPE_CONF_CHECK_I(dp_m_n.gmch_m);
	PIPE_CONF_CHECK_I(dp_m_n.gmch_n);
	PIPE_CONF_CHECK_I(dp_m_n.link_m);
	PIPE_CONF_CHECK_I(dp_m_n.link_n);
	PIPE_CONF_CHECK_I(dp_m_n.tu);

	PIPE_CONF_CHECK_I(adjusted_mode.crtc_hdisplay);
	PIPE_CONF_CHECK_I(adjusted_mode.crtc_htotal);
	PIPE_CONF_CHECK_I(adjusted_mode.crtc_hblank_start);
	PIPE_CONF_CHECK_I(adjusted_mode.crtc_hblank_end);
	PIPE_CONF_CHECK_I(adjusted_mode.crtc_hsync_start);
	PIPE_CONF_CHECK_I(adjusted_mode.crtc_hsync_end);

	PIPE_CONF_CHECK_I(adjusted_mode.crtc_vdisplay);
	PIPE_CONF_CHECK_I(adjusted_mode.crtc_vtotal);
	PIPE_CONF_CHECK_I(adjusted_mode.crtc_vblank_start);
	PIPE_CONF_CHECK_I(adjusted_mode.crtc_vblank_end);
	PIPE_CONF_CHECK_I(adjusted_mode.crtc_vsync_start);
	PIPE_CONF_CHECK_I(adjusted_mode.crtc_vsync_end);

	PIPE_CONF_CHECK_I(pixel_multiplier);

	PIPE_CONF_CHECK_FLAGS(adjusted_mode.flags,
			      DRM_MODE_FLAG_INTERLACE);

	if (!PIPE_CONF_QUIRK(PIPE_CONFIG_QUIRK_MODE_SYNC_FLAGS)) {
		PIPE_CONF_CHECK_FLAGS(adjusted_mode.flags,
				      DRM_MODE_FLAG_PHSYNC);
		PIPE_CONF_CHECK_FLAGS(adjusted_mode.flags,
				      DRM_MODE_FLAG_NHSYNC);
		PIPE_CONF_CHECK_FLAGS(adjusted_mode.flags,
				      DRM_MODE_FLAG_PVSYNC);
		PIPE_CONF_CHECK_FLAGS(adjusted_mode.flags,
				      DRM_MODE_FLAG_NVSYNC);
	}

	PIPE_CONF_CHECK_I(pipe_src_w);
	PIPE_CONF_CHECK_I(pipe_src_h);

	PIPE_CONF_CHECK_I(gmch_pfit.control);
	/* pfit ratios are autocomputed by the hw on gen4+ */
	if (INTEL_INFO(dev)->gen < 4)
		PIPE_CONF_CHECK_I(gmch_pfit.pgm_ratios);
	PIPE_CONF_CHECK_I(gmch_pfit.lvds_border_bits);
	PIPE_CONF_CHECK_I(pch_pfit.enabled);
	if (current_config->pch_pfit.enabled) {
		PIPE_CONF_CHECK_I(pch_pfit.pos);
		PIPE_CONF_CHECK_I(pch_pfit.size);
	}

	PIPE_CONF_CHECK_I(ips_enabled);

	PIPE_CONF_CHECK_I(double_wide);

	PIPE_CONF_CHECK_I(shared_dpll);
	PIPE_CONF_CHECK_X(dpll_hw_state.dpll);
	PIPE_CONF_CHECK_X(dpll_hw_state.dpll_md);
	PIPE_CONF_CHECK_X(dpll_hw_state.fp0);
	PIPE_CONF_CHECK_X(dpll_hw_state.fp1);

	if (IS_G4X(dev) || INTEL_INFO(dev)->gen >= 5)
		PIPE_CONF_CHECK_I(pipe_bpp);

	if (!HAS_DDI(dev)) {
		PIPE_CONF_CHECK_CLOCK_FUZZY(adjusted_mode.crtc_clock);
		PIPE_CONF_CHECK_CLOCK_FUZZY(port_clock);
	}

#undef PIPE_CONF_CHECK_X
#undef PIPE_CONF_CHECK_I
#undef PIPE_CONF_CHECK_FLAGS
#undef PIPE_CONF_CHECK_CLOCK_FUZZY
#undef PIPE_CONF_QUIRK

	return true;
}

static void
check_connector_state(struct drm_device *dev)
{
	struct intel_connector *connector;

	list_for_each_entry(connector, &dev->mode_config.connector_list,
			    base.head) {
		/* This also checks the encoder/connector hw state with the
		 * ->get_hw_state callbacks. */
		intel_connector_check_state(connector);

		WARN(&connector->new_encoder->base != connector->base.encoder,
		     "connector's staged encoder doesn't match current encoder\n");
	}
}

static void
check_encoder_state(struct drm_device *dev)
{
	struct intel_encoder *encoder;
	struct intel_connector *connector;

	list_for_each_entry(encoder, &dev->mode_config.encoder_list,
			    base.head) {
		bool enabled = false;
		bool active = false;
		enum pipe pipe, tracked_pipe;

		DRM_DEBUG_KMS("[ENCODER:%d:%s]\n",
			      encoder->base.base.id,
			      drm_get_encoder_name(&encoder->base));

		WARN(&encoder->new_crtc->base != encoder->base.crtc,
		     "encoder's stage crtc doesn't match current crtc\n");
		WARN(encoder->connectors_active && !encoder->base.crtc,
		     "encoder's active_connectors set, but no crtc\n");

		list_for_each_entry(connector, &dev->mode_config.connector_list,
				    base.head) {
			if (connector->base.encoder != &encoder->base)
				continue;
			enabled = true;
			if (connector->base.dpms != DRM_MODE_DPMS_OFF)
				active = true;
		}
		WARN(!!encoder->base.crtc != enabled,
		     "encoder's enabled state mismatch "
		     "(expected %i, found %i)\n",
		     !!encoder->base.crtc, enabled);
		WARN(active && !encoder->base.crtc,
		     "active encoder with no crtc\n");

		WARN(encoder->connectors_active != active,
		     "encoder's computed active state doesn't match tracked active state "
		     "(expected %i, found %i)\n", active, encoder->connectors_active);

		active = encoder->get_hw_state(encoder, &pipe);
		WARN(active != encoder->connectors_active,
		     "encoder's hw state doesn't match sw tracking "
		     "(expected %i, found %i)\n",
		     encoder->connectors_active, active);

		if (!encoder->base.crtc)
			continue;

		tracked_pipe = to_intel_crtc(encoder->base.crtc)->pipe;
		WARN(active && pipe != tracked_pipe,
		     "active encoder's pipe doesn't match"
		     "(expected %i, found %i)\n",
		     tracked_pipe, pipe);

	}
}

static void
check_crtc_state(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_crtc *crtc;
	struct intel_encoder *encoder;
	struct intel_crtc_config pipe_config;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list,
			    base.head) {
		bool enabled = false;
		bool active = false;

		memset(&pipe_config, 0, sizeof(pipe_config));

		DRM_DEBUG_KMS("[CRTC:%d]\n",
			      crtc->base.base.id);

		WARN(crtc->active && !crtc->base.enabled,
		     "active crtc, but not enabled in sw tracking\n");

		list_for_each_entry(encoder, &dev->mode_config.encoder_list,
				    base.head) {
			if (encoder->base.crtc != &crtc->base)
				continue;
			enabled = true;
			if (encoder->connectors_active)
				active = true;
		}

		WARN(active != crtc->active,
		     "crtc's computed active state doesn't match tracked active state "
		     "(expected %i, found %i)\n", active, crtc->active);
		WARN(enabled != crtc->base.enabled,
		     "crtc's computed enabled state doesn't match tracked enabled state "
		     "(expected %i, found %i)\n", enabled, crtc->base.enabled);

		active = dev_priv->display.get_pipe_config(crtc,
							   &pipe_config);

		/* hw state is inconsistent with the pipe A quirk */
		if (crtc->pipe == PIPE_A && dev_priv->quirks & QUIRK_PIPEA_FORCE)
			active = crtc->active;

		list_for_each_entry(encoder, &dev->mode_config.encoder_list,
				    base.head) {
			enum pipe pipe;
			if (encoder->base.crtc != &crtc->base)
				continue;
			if (encoder->get_hw_state(encoder, &pipe))
				encoder->get_config(encoder, &pipe_config);
		}

		WARN(crtc->active != active,
		     "crtc active state doesn't match with hw state "
		     "(expected %i, found %i)\n", crtc->active, active);

		if (active &&
		    !intel_pipe_config_compare(dev, &crtc->config, &pipe_config)) {
			WARN(1, "pipe state doesn't match!\n");
			intel_dump_pipe_config(crtc, &pipe_config,
					       "[hw state]");
			intel_dump_pipe_config(crtc, &crtc->config,
					       "[sw state]");
		}
	}
}

static void
check_shared_dpll_state(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_crtc *crtc;
	struct intel_dpll_hw_state dpll_hw_state;
	int i;

	for (i = 0; i < dev_priv->num_shared_dpll; i++) {
		struct intel_shared_dpll *pll = &dev_priv->shared_dplls[i];
		int enabled_crtcs = 0, active_crtcs = 0;
		bool active;

		memset(&dpll_hw_state, 0, sizeof(dpll_hw_state));

		DRM_DEBUG_KMS("%s\n", pll->name);

		active = pll->get_hw_state(dev_priv, pll, &dpll_hw_state);

		WARN(pll->active > pll->refcount,
		     "more active pll users than references: %i vs %i\n",
		     pll->active, pll->refcount);
		WARN(pll->active && !pll->on,
		     "pll in active use but not on in sw tracking\n");
		WARN(pll->on && !pll->active,
		     "pll in on but not on in use in sw tracking\n");
		WARN(pll->on != active,
		     "pll on state mismatch (expected %i, found %i)\n",
		     pll->on, active);

		list_for_each_entry(crtc, &dev->mode_config.crtc_list,
				    base.head) {
			if (crtc->base.enabled && intel_crtc_to_shared_dpll(crtc) == pll)
				enabled_crtcs++;
			if (crtc->active && intel_crtc_to_shared_dpll(crtc) == pll)
				active_crtcs++;
		}
		WARN(pll->active != active_crtcs,
		     "pll active crtcs mismatch (expected %i, found %i)\n",
		     pll->active, active_crtcs);
		WARN(pll->refcount != enabled_crtcs,
		     "pll enabled crtcs mismatch (expected %i, found %i)\n",
		     pll->refcount, enabled_crtcs);

		WARN(pll->on && memcmp(&pll->hw_state, &dpll_hw_state,
				       sizeof(dpll_hw_state)),
		     "pll hw state mismatch\n");
	}
}

void
intel_modeset_check_state(struct drm_device *dev)
{
	check_connector_state(dev);
	check_encoder_state(dev);
	check_crtc_state(dev);
	check_shared_dpll_state(dev);
}

void ironlake_check_encoder_dotclock(const struct intel_crtc_config *pipe_config,
				     int dotclock)
{
	/*
	 * FDI already provided one idea for the dotclock.
	 * Yell if the encoder disagrees.
	 */
	WARN(!intel_fuzzy_clock_check(pipe_config->adjusted_mode.crtc_clock, dotclock),
	     "FDI dotclock and encoder dotclock mismatch, fdi: %i, encoder: %i\n",
	     pipe_config->adjusted_mode.crtc_clock, dotclock);
}

static int __intel_set_mode(struct drm_crtc *crtc,
			    struct drm_display_mode *mode,
			    int x, int y, struct drm_framebuffer *fb)
{
	struct drm_device *dev = crtc->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_display_mode *saved_mode, *saved_hwmode;
	struct intel_crtc_config *pipe_config = NULL;
	struct intel_crtc *intel_crtc;
	unsigned disable_pipes, prepare_pipes, modeset_pipes;
	int ret = 0;

	saved_mode = kcalloc(2, sizeof(*saved_mode), GFP_KERNEL);
	if (!saved_mode)
		return -ENOMEM;
	saved_hwmode = saved_mode + 1;

	intel_modeset_affected_pipes(crtc, &modeset_pipes,
				     &prepare_pipes, &disable_pipes);

	*saved_hwmode = crtc->hwmode;
	*saved_mode = crtc->mode;

	/* Hack: Because we don't (yet) support global modeset on multiple
	 * crtcs, we don't keep track of the new mode for more than one crtc.
	 * Hence simply check whether any bit is set in modeset_pipes in all the
	 * pieces of code that are not yet converted to deal with mutliple crtcs
	 * changing their mode at the same time. */
	if (modeset_pipes) {
		pipe_config = intel_modeset_pipe_config(crtc, fb, mode);
		if (IS_ERR(pipe_config)) {
			ret = PTR_ERR(pipe_config);
			pipe_config = NULL;

			goto out;
		}
		intel_dump_pipe_config(to_intel_crtc(crtc), pipe_config,
				       "[modeset]");
	}

	for_each_intel_crtc_masked(dev, disable_pipes, intel_crtc)
		intel_crtc_disable(&intel_crtc->base);

	for_each_intel_crtc_masked(dev, prepare_pipes, intel_crtc) {
		if (intel_crtc->base.enabled)
			dev_priv->display.crtc_disable(&intel_crtc->base);
	}

	/* crtc->mode is already used by the ->mode_set callbacks, hence we need
	 * to set it here already despite that we pass it down the callchain.
	 */
	if (modeset_pipes) {
		crtc->mode = *mode;
		/* mode_set/enable/disable functions rely on a correct pipe
		 * config. */
		to_intel_crtc(crtc)->config = *pipe_config;
	}

	/* Only after disabling all output pipelines that will be changed can we
	 * update the the output configuration. */
	intel_modeset_update_state(dev, prepare_pipes);

	if (dev_priv->display.modeset_global_resources)
		dev_priv->display.modeset_global_resources(dev);

	/* Set up the DPLL and any encoders state that needs to adjust or depend
	 * on the DPLL.
	 */
	for_each_intel_crtc_masked(dev, modeset_pipes, intel_crtc) {
		ret = intel_crtc_mode_set(&intel_crtc->base,
					  x, y, fb);
		if (ret)
			goto done;
	}

	/* Now enable the clocks, plane, pipe, and connectors that we set up. */
	for_each_intel_crtc_masked(dev, prepare_pipes, intel_crtc)
		dev_priv->display.crtc_enable(&intel_crtc->base);

	if (modeset_pipes) {
		/* Store real post-adjustment hardware mode. */
		crtc->hwmode = pipe_config->adjusted_mode;

		/* Calculate and store various constants which
		 * are later needed by vblank and swap-completion
		 * timestamping. They are derived from true hwmode.
		 */
		drm_calc_timestamping_constants(crtc);
	}

	/* FIXME: add subpixel order */
done:
	if (ret && crtc->enabled) {
		crtc->hwmode = *saved_hwmode;
		crtc->mode = *saved_mode;
	}

out:
	kfree(pipe_config);
	kfree(saved_mode);
	return ret;
}

static int intel_set_mode(struct drm_crtc *crtc,
			  struct drm_display_mode *mode,
			  int x, int y, struct drm_framebuffer *fb)
{
	int ret;

	ret = __intel_set_mode(crtc, mode, x, y, fb);

	if (ret == 0)
		intel_modeset_check_state(crtc->dev);

	return ret;
}

void intel_crtc_restore_mode(struct drm_crtc *crtc)
{
	intel_set_mode(crtc, &crtc->mode, crtc->x, crtc->y, crtc->fb);
}

#undef for_each_intel_crtc_masked

static void intel_set_config_free(struct intel_set_config *config)
{
	if (!config)
		return;

	kfree(config->save_connector_encoders);
	kfree(config->save_encoder_crtcs);
	kfree(config);
}

static int intel_set_config_save_state(struct drm_device *dev,
				       struct intel_set_config *config)
{
	struct drm_encoder *encoder;
	struct drm_connector *connector;
	int count;

	config->save_encoder_crtcs =
		kcalloc(dev->mode_config.num_encoder,
			sizeof(struct drm_crtc *), GFP_KERNEL);
	if (!config->save_encoder_crtcs)
		return -ENOMEM;

	config->save_connector_encoders =
		kcalloc(dev->mode_config.num_connector,
			sizeof(struct drm_encoder *), GFP_KERNEL);
	if (!config->save_connector_encoders)
		return -ENOMEM;

	/* Copy data. Note that driver private data is not affected.
	 * Should anything bad happen only the expected state is
	 * restored, not the drivers personal bookkeeping.
	 */
	count = 0;
	list_for_each_entry(encoder, &dev->mode_config.encoder_list, head) {
		config->save_encoder_crtcs[count++] = encoder->crtc;
	}

	count = 0;
	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		config->save_connector_encoders[count++] = connector->encoder;
	}

	return 0;
}

static void intel_set_config_restore_state(struct drm_device *dev,
					   struct intel_set_config *config)
{
	struct intel_encoder *encoder;
	struct intel_connector *connector;
	int count;

	count = 0;
	list_for_each_entry(encoder, &dev->mode_config.encoder_list, base.head) {
		encoder->new_crtc =
			to_intel_crtc(config->save_encoder_crtcs[count++]);
	}

	count = 0;
	list_for_each_entry(connector, &dev->mode_config.connector_list, base.head) {
		connector->new_encoder =
			to_intel_encoder(config->save_connector_encoders[count++]);
	}
}

static bool
is_crtc_connector_off(struct drm_mode_set *set)
{
	int i;

	if (set->num_connectors == 0)
		return false;

	if (WARN_ON(set->connectors == NULL))
		return false;

	for (i = 0; i < set->num_connectors; i++)
		if (set->connectors[i]->encoder &&
		    set->connectors[i]->encoder->crtc == set->crtc &&
		    set->connectors[i]->dpms != DRM_MODE_DPMS_ON)
			return true;

	return false;
}

static void
intel_set_config_compute_mode_changes(struct drm_mode_set *set,
				      struct intel_set_config *config)
{

	/* We should be able to check here if the fb has the same properties
	 * and then just flip_or_move it */
	if (is_crtc_connector_off(set)) {
		config->mode_changed = true;
	} else if (set->crtc->fb != set->fb) {
		/* If we have no fb then treat it as a full mode set */
		if (set->crtc->fb == NULL) {
			struct intel_crtc *intel_crtc =
				to_intel_crtc(set->crtc);

			if (intel_crtc->active && i915_fastboot) {
				DRM_DEBUG_KMS("crtc has no fb, will flip\n");
				config->fb_changed = true;
			} else {
				DRM_DEBUG_KMS("inactive crtc, full mode set\n");
				config->mode_changed = true;
			}
		} else if (set->fb == NULL) {
			config->mode_changed = true;
		} else if (set->fb->pixel_format !=
			   set->crtc->fb->pixel_format) {
			config->mode_changed = true;
		} else {
			config->fb_changed = true;
		}
	}

	if (set->fb && (set->x != set->crtc->x || set->y != set->crtc->y))
		config->fb_changed = true;

	if (set->mode && !drm_mode_equal(set->mode, &set->crtc->mode)) {
		DRM_DEBUG_KMS("modes are different, full mode set\n");
		drm_mode_debug_printmodeline(&set->crtc->mode);
		drm_mode_debug_printmodeline(set->mode);
		config->mode_changed = true;
	}

	DRM_DEBUG_KMS("computed changes for [CRTC:%d], mode_changed=%d, fb_changed=%d\n",
			set->crtc->base.id, config->mode_changed, config->fb_changed);
}

static int
intel_modeset_stage_output_state(struct drm_device *dev,
				 struct drm_mode_set *set,
				 struct intel_set_config *config)
{
	struct drm_crtc *new_crtc;
	struct intel_connector *connector;
	struct intel_encoder *encoder;
	int ro;

	/* The upper layers ensure that we either disable a crtc or have a list
	 * of connectors. For paranoia, double-check this. */
	WARN_ON(!set->fb && (set->num_connectors != 0));
	WARN_ON(set->fb && (set->num_connectors == 0));

	list_for_each_entry(connector, &dev->mode_config.connector_list,
			    base.head) {
		/* Otherwise traverse passed in connector list and get encoders
		 * for them. */
		for (ro = 0; ro < set->num_connectors; ro++) {
			if (set->connectors[ro] == &connector->base) {
				connector->new_encoder = connector->encoder;
				break;
			}
		}

		/* If we disable the crtc, disable all its connectors. Also, if
		 * the connector is on the changing crtc but not on the new
		 * connector list, disable it. */
		if ((!set->fb || ro == set->num_connectors) &&
		    connector->base.encoder &&
		    connector->base.encoder->crtc == set->crtc) {
			connector->new_encoder = NULL;

			DRM_DEBUG_KMS("[CONNECTOR:%d:%s] to [NOCRTC]\n",
				connector->base.base.id,
				drm_get_connector_name(&connector->base));
		}


		if (&connector->new_encoder->base != connector->base.encoder) {
			DRM_DEBUG_KMS("encoder changed, full mode switch\n");
			config->mode_changed = true;
		}
	}
	/* connector->new_encoder is now updated for all connectors. */

	/* Update crtc of enabled connectors. */
	list_for_each_entry(connector, &dev->mode_config.connector_list,
			    base.head) {
		if (!connector->new_encoder)
			continue;

		new_crtc = connector->new_encoder->base.crtc;

		for (ro = 0; ro < set->num_connectors; ro++) {
			if (set->connectors[ro] == &connector->base)
				new_crtc = set->crtc;
		}

		/* Make sure the new CRTC will work with the encoder */
		if (!intel_encoder_crtc_ok(&connector->new_encoder->base,
					   new_crtc)) {
			return -EINVAL;
		}
		connector->encoder->new_crtc = to_intel_crtc(new_crtc);

		DRM_DEBUG_KMS("[CONNECTOR:%d:%s] to [CRTC:%d]\n",
			connector->base.base.id,
			drm_get_connector_name(&connector->base),
			new_crtc->base.id);
	}

	/* Check for any encoders that needs to be disabled. */
	list_for_each_entry(encoder, &dev->mode_config.encoder_list,
			    base.head) {
		list_for_each_entry(connector,
				    &dev->mode_config.connector_list,
				    base.head) {
			if (connector->new_encoder == encoder) {
				WARN_ON(!connector->new_encoder->new_crtc);

				goto next_encoder;
			}
		}
		encoder->new_crtc = NULL;
next_encoder:
		/* Only now check for crtc changes so we don't miss encoders
		 * that will be disabled. */
		if (&encoder->new_crtc->base != encoder->base.crtc) {
			DRM_DEBUG_KMS("crtc changed, full mode switch\n");
			config->mode_changed = true;
		}
	}
	/* Now we've also updated encoder->new_crtc for all encoders. */

	return 0;
}

static int intel_crtc_set_config(struct drm_mode_set *set)
{
	struct drm_device *dev;
	struct drm_mode_set save_set;
	struct intel_set_config *config;
	int ret;

	BUG_ON(!set);
	BUG_ON(!set->crtc);
	BUG_ON(!set->crtc->helper_private);

	/* Enforce sane interface api - has been abused by the fb helper. */
	BUG_ON(!set->mode && set->fb);
	BUG_ON(set->fb && set->num_connectors == 0);

	if (set->fb) {
		DRM_DEBUG_KMS("[CRTC:%d] [FB:%d] #connectors=%d (x y) (%i %i)\n",
				set->crtc->base.id, set->fb->base.id,
				(int)set->num_connectors, set->x, set->y);
	} else {
		DRM_DEBUG_KMS("[CRTC:%d] [NOFB]\n", set->crtc->base.id);
	}

	dev = set->crtc->dev;

	ret = -ENOMEM;
	config = kzalloc(sizeof(*config), GFP_KERNEL);
	if (!config)
		goto out_config;

	ret = intel_set_config_save_state(dev, config);
	if (ret)
		goto out_config;

	save_set.crtc = set->crtc;
	save_set.mode = &set->crtc->mode;
	save_set.x = set->crtc->x;
	save_set.y = set->crtc->y;
	save_set.fb = set->crtc->fb;

	/* Compute whether we need a full modeset, only an fb base update or no
	 * change at all. In the future we might also check whether only the
	 * mode changed, e.g. for LVDS where we only change the panel fitter in
	 * such cases. */
	intel_set_config_compute_mode_changes(set, config);

	ret = intel_modeset_stage_output_state(dev, set, config);
	if (ret)
		goto fail;

	if (config->mode_changed) {
		ret = intel_set_mode(set->crtc, set->mode,
				     set->x, set->y, set->fb);
	} else if (config->fb_changed) {
		intel_crtc_wait_for_pending_flips(set->crtc);

		ret = intel_pipe_set_base(set->crtc,
					  set->x, set->y, set->fb);
	}

	if (ret) {
		DRM_DEBUG_KMS("failed to set mode on [CRTC:%d], err = %d\n",
			      set->crtc->base.id, ret);
fail:
		intel_set_config_restore_state(dev, config);

		/* Try to restore the config */
		if (config->mode_changed &&
		    intel_set_mode(save_set.crtc, save_set.mode,
				   save_set.x, save_set.y, save_set.fb))
			DRM_ERROR("failed to restore config after modeset failure\n");
	}

out_config:
	intel_set_config_free(config);
	return ret;
}

static const struct drm_crtc_funcs intel_crtc_funcs = {
	.cursor_set = intel_crtc_cursor_set,
	.cursor_move = intel_crtc_cursor_move,
	.gamma_set = intel_crtc_gamma_set,
	.set_config = intel_crtc_set_config,
	.destroy = intel_crtc_destroy,
	.page_flip = intel_crtc_page_flip,
};

static void intel_cpu_pll_init(struct drm_device *dev)
{
	if (HAS_DDI(dev))
		intel_ddi_pll_init(dev);
}

static bool ibx_pch_dpll_get_hw_state(struct drm_i915_private *dev_priv,
				      struct intel_shared_dpll *pll,
				      struct intel_dpll_hw_state *hw_state)
{
	uint32_t val;

	val = I915_READ(PCH_DPLL(pll->id));
	hw_state->dpll = val;
	hw_state->fp0 = I915_READ(PCH_FP0(pll->id));
	hw_state->fp1 = I915_READ(PCH_FP1(pll->id));

	return val & DPLL_VCO_ENABLE;
}

static void ibx_pch_dpll_mode_set(struct drm_i915_private *dev_priv,
				  struct intel_shared_dpll *pll)
{
	I915_WRITE(PCH_FP0(pll->id), pll->hw_state.fp0);
	I915_WRITE(PCH_FP1(pll->id), pll->hw_state.fp1);
}

static void ibx_pch_dpll_enable(struct drm_i915_private *dev_priv,
				struct intel_shared_dpll *pll)
{
	/* PCH refclock must be enabled first */
	assert_pch_refclk_enabled(dev_priv);

	I915_WRITE(PCH_DPLL(pll->id), pll->hw_state.dpll);

	/* Wait for the clocks to stabilize. */
	POSTING_READ(PCH_DPLL(pll->id));
	udelay(150);

	/* The pixel multiplier can only be updated once the
	 * DPLL is enabled and the clocks are stable.
	 *
	 * So write it again.
	 */
	I915_WRITE(PCH_DPLL(pll->id), pll->hw_state.dpll);
	POSTING_READ(PCH_DPLL(pll->id));
	udelay(200);
}

static void ibx_pch_dpll_disable(struct drm_i915_private *dev_priv,
				 struct intel_shared_dpll *pll)
{
	struct drm_device *dev = dev_priv->dev;
	struct intel_crtc *crtc;

	/* Make sure no transcoder isn't still depending on us. */
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, base.head) {
		if (intel_crtc_to_shared_dpll(crtc) == pll)
			assert_pch_transcoder_disabled(dev_priv, crtc->pipe);
	}

	I915_WRITE(PCH_DPLL(pll->id), 0);
	POSTING_READ(PCH_DPLL(pll->id));
	udelay(200);
}

static char *ibx_pch_dpll_names[] = {
	"PCH DPLL A",
	"PCH DPLL B",
};

static void ibx_pch_dpll_init(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int i;

	dev_priv->num_shared_dpll = 2;

	for (i = 0; i < dev_priv->num_shared_dpll; i++) {
		dev_priv->shared_dplls[i].id = i;
		dev_priv->shared_dplls[i].name = ibx_pch_dpll_names[i];
		dev_priv->shared_dplls[i].mode_set = ibx_pch_dpll_mode_set;
		dev_priv->shared_dplls[i].enable = ibx_pch_dpll_enable;
		dev_priv->shared_dplls[i].disable = ibx_pch_dpll_disable;
		dev_priv->shared_dplls[i].get_hw_state =
			ibx_pch_dpll_get_hw_state;
	}
}

static void intel_shared_dpll_init(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (HAS_PCH_IBX(dev) || HAS_PCH_CPT(dev))
		ibx_pch_dpll_init(dev);
	else
		dev_priv->num_shared_dpll = 0;

	BUG_ON(dev_priv->num_shared_dpll > I915_NUM_PLLS);
	DRM_DEBUG_KMS("%i shared PLLs initialized\n",
		      dev_priv->num_shared_dpll);
}

static void intel_crtc_init(struct drm_device *dev, int pipe)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc;
	int i;

	intel_crtc = kzalloc(sizeof(*intel_crtc), GFP_KERNEL);
	if (intel_crtc == NULL)
		return;

	drm_crtc_init(dev, &intel_crtc->base, &intel_crtc_funcs);

	drm_mode_crtc_set_gamma_size(&intel_crtc->base, 256);
	for (i = 0; i < 256; i++) {
		intel_crtc->lut_r[i] = i;
		intel_crtc->lut_g[i] = i;
		intel_crtc->lut_b[i] = i;
	}

	/* Swap pipes & planes for FBC on pre-965 */
	intel_crtc->pipe = pipe;
	intel_crtc->plane = pipe;
	if (IS_MOBILE(dev) && IS_GEN3(dev)) {
		DRM_DEBUG_KMS("swapping pipes & planes for FBC\n");
		intel_crtc->plane = !pipe;
	}

	BUG_ON(pipe >= ARRAY_SIZE(dev_priv->plane_to_crtc_mapping) ||
	       dev_priv->plane_to_crtc_mapping[intel_crtc->plane] != NULL);
	dev_priv->plane_to_crtc_mapping[intel_crtc->plane] = &intel_crtc->base;
	dev_priv->pipe_to_crtc_mapping[intel_crtc->pipe] = &intel_crtc->base;

	drm_crtc_helper_add(&intel_crtc->base, &intel_helper_funcs);
}

enum pipe intel_get_pipe_from_connector(struct intel_connector *connector)
{
	struct drm_encoder *encoder = connector->base.encoder;

	WARN_ON(!mutex_is_locked(&connector->base.dev->mode_config.mutex));

	if (!encoder)
		return INVALID_PIPE;

	return to_intel_crtc(encoder->crtc)->pipe;
}

int intel_get_pipe_from_crtc_id(struct drm_device *dev, void *data,
				struct drm_file *file)
{
	struct drm_i915_get_pipe_from_crtc_id *pipe_from_crtc_id = data;
	struct drm_mode_object *drmmode_obj;
	struct intel_crtc *crtc;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -ENODEV;

	drmmode_obj = drm_mode_object_find(dev, pipe_from_crtc_id->crtc_id,
			DRM_MODE_OBJECT_CRTC);

	if (!drmmode_obj) {
		DRM_ERROR("no such CRTC id\n");
		return -ENOENT;
	}

	crtc = to_intel_crtc(obj_to_crtc(drmmode_obj));
	pipe_from_crtc_id->pipe = crtc->pipe;

	return 0;
}

static int intel_encoder_clones(struct intel_encoder *encoder)
{
	struct drm_device *dev = encoder->base.dev;
	struct intel_encoder *source_encoder;
	int index_mask = 0;
	int entry = 0;

	list_for_each_entry(source_encoder,
			    &dev->mode_config.encoder_list, base.head) {

		if (encoder == source_encoder)
			index_mask |= (1 << entry);

		/* Intel hw has only one MUX where enocoders could be cloned. */
		if (encoder->cloneable && source_encoder->cloneable)
			index_mask |= (1 << entry);

		entry++;
	}

	return index_mask;
}

static bool has_edp_a(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (!IS_MOBILE(dev))
		return false;

	if ((I915_READ(DP_A) & DP_DETECTED) == 0)
		return false;

	if (IS_GEN5(dev) &&
	    (I915_READ(ILK_DISPLAY_CHICKEN_FUSES) & ILK_eDP_A_DISABLE))
		return false;

	return true;
}

static void intel_setup_outputs(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_encoder *encoder;
	bool dpd_is_edp = false;

	intel_lvds_init(dev);

	if (!IS_ULT(dev))
		intel_crt_init(dev);

	if (HAS_DDI(dev)) {
		int found;

		/* Haswell uses DDI functions to detect digital outputs */
		found = I915_READ(DDI_BUF_CTL_A) & DDI_INIT_DISPLAY_DETECTED;
		/* DDI A only supports eDP */
		if (found)
			intel_ddi_init(dev, PORT_A);

		/* DDI B, C and D detection is indicated by the SFUSE_STRAP
		 * register */
		found = I915_READ(SFUSE_STRAP);

		if (found & SFUSE_STRAP_DDIB_DETECTED)
			intel_ddi_init(dev, PORT_B);
		if (found & SFUSE_STRAP_DDIC_DETECTED)
			intel_ddi_init(dev, PORT_C);
		if (found & SFUSE_STRAP_DDID_DETECTED)
			intel_ddi_init(dev, PORT_D);
	} else if (HAS_PCH_SPLIT(dev)) {
		int found;
		dpd_is_edp = intel_dp_is_edp(dev, PORT_D);

		if (has_edp_a(dev))
			intel_dp_init(dev, DP_A, PORT_A);

		if (I915_READ(PCH_HDMIB) & SDVO_DETECTED) {
			/* PCH SDVOB multiplex with HDMIB */
			found = intel_sdvo_init(dev, PCH_SDVOB, true);
			if (!found)
				intel_hdmi_init(dev, PCH_HDMIB, PORT_B);
			if (!found && (I915_READ(PCH_DP_B) & DP_DETECTED))
				intel_dp_init(dev, PCH_DP_B, PORT_B);
		}

		if (I915_READ(PCH_HDMIC) & SDVO_DETECTED)
			intel_hdmi_init(dev, PCH_HDMIC, PORT_C);

		if (!dpd_is_edp && I915_READ(PCH_HDMID) & SDVO_DETECTED)
			intel_hdmi_init(dev, PCH_HDMID, PORT_D);

		if (I915_READ(PCH_DP_C) & DP_DETECTED)
			intel_dp_init(dev, PCH_DP_C, PORT_C);

		if (I915_READ(PCH_DP_D) & DP_DETECTED)
			intel_dp_init(dev, PCH_DP_D, PORT_D);
	} else if (IS_VALLEYVIEW(dev)) {
		if (I915_READ(VLV_DISPLAY_BASE + GEN4_HDMIB) & SDVO_DETECTED) {
			intel_hdmi_init(dev, VLV_DISPLAY_BASE + GEN4_HDMIB,
					PORT_B);
			if (I915_READ(VLV_DISPLAY_BASE + DP_B) & DP_DETECTED)
				intel_dp_init(dev, VLV_DISPLAY_BASE + DP_B, PORT_B);
		}

		if (I915_READ(VLV_DISPLAY_BASE + GEN4_HDMIC) & SDVO_DETECTED) {
			intel_hdmi_init(dev, VLV_DISPLAY_BASE + GEN4_HDMIC,
					PORT_C);
			if (I915_READ(VLV_DISPLAY_BASE + DP_C) & DP_DETECTED)
				intel_dp_init(dev, VLV_DISPLAY_BASE + DP_C, PORT_C);
		}

		intel_dsi_init(dev);
	} else if (SUPPORTS_DIGITAL_OUTPUTS(dev)) {
		bool found = false;

		if (I915_READ(GEN3_SDVOB) & SDVO_DETECTED) {
			DRM_DEBUG_KMS("probing SDVOB\n");
			found = intel_sdvo_init(dev, GEN3_SDVOB, true);
			if (!found && SUPPORTS_INTEGRATED_HDMI(dev)) {
				DRM_DEBUG_KMS("probing HDMI on SDVOB\n");
				intel_hdmi_init(dev, GEN4_HDMIB, PORT_B);
			}

			if (!found && SUPPORTS_INTEGRATED_DP(dev))
				intel_dp_init(dev, DP_B, PORT_B);
		}

		/* Before G4X SDVOC doesn't have its own detect register */

		if (I915_READ(GEN3_SDVOB) & SDVO_DETECTED) {
			DRM_DEBUG_KMS("probing SDVOC\n");
			found = intel_sdvo_init(dev, GEN3_SDVOC, false);
		}

		if (!found && (I915_READ(GEN3_SDVOC) & SDVO_DETECTED)) {

			if (SUPPORTS_INTEGRATED_HDMI(dev)) {
				DRM_DEBUG_KMS("probing HDMI on SDVOC\n");
				intel_hdmi_init(dev, GEN4_HDMIC, PORT_C);
			}
			if (SUPPORTS_INTEGRATED_DP(dev))
				intel_dp_init(dev, DP_C, PORT_C);
		}

		if (SUPPORTS_INTEGRATED_DP(dev) &&
		    (I915_READ(DP_D) & DP_DETECTED))
			intel_dp_init(dev, DP_D, PORT_D);
	} else if (IS_GEN2(dev))
		intel_dvo_init(dev);

	if (SUPPORTS_TV(dev))
		intel_tv_init(dev);

	list_for_each_entry(encoder, &dev->mode_config.encoder_list, base.head) {
		encoder->base.possible_crtcs = encoder->crtc_mask;
		encoder->base.possible_clones =
			intel_encoder_clones(encoder);
	}

	intel_init_pch_refclk(dev);

	drm_helper_move_panel_connectors_to_head(dev);
}

void intel_framebuffer_fini(struct intel_framebuffer *fb)
{
	drm_framebuffer_cleanup(&fb->base);
	WARN_ON(!fb->obj->framebuffer_references--);
	drm_gem_object_unreference_unlocked(&fb->obj->base);
}

static void intel_user_framebuffer_destroy(struct drm_framebuffer *fb)
{
	struct intel_framebuffer *intel_fb = to_intel_framebuffer(fb);

	intel_framebuffer_fini(intel_fb);
	kfree(intel_fb);
}

static int intel_user_framebuffer_create_handle(struct drm_framebuffer *fb,
						struct drm_file *file,
						unsigned int *handle)
{
	struct intel_framebuffer *intel_fb = to_intel_framebuffer(fb);
	struct drm_i915_gem_object *obj = intel_fb->obj;

	return drm_gem_handle_create(file, &obj->base, handle);
}

static const struct drm_framebuffer_funcs intel_fb_funcs = {
	.destroy = intel_user_framebuffer_destroy,
	.create_handle = intel_user_framebuffer_create_handle,
};

int intel_framebuffer_init(struct drm_device *dev,
			   struct intel_framebuffer *intel_fb,
			   struct drm_mode_fb_cmd2 *mode_cmd,
			   struct drm_i915_gem_object *obj)
{
	int aligned_height, tile_height;
	int pitch_limit;
	int ret;

	WARN_ON(!mutex_is_locked(&dev->struct_mutex));

	if (obj->tiling_mode == I915_TILING_Y) {
		DRM_DEBUG("hardware does not support tiling Y\n");
		return -EINVAL;
	}

	if (mode_cmd->pitches[0] & 63) {
		DRM_DEBUG("pitch (%d) must be at least 64 byte aligned\n",
			  mode_cmd->pitches[0]);
		return -EINVAL;
	}

	if (INTEL_INFO(dev)->gen >= 5 && !IS_VALLEYVIEW(dev)) {
		pitch_limit = 32*1024;
	} else if (INTEL_INFO(dev)->gen >= 4) {
		if (obj->tiling_mode)
			pitch_limit = 16*1024;
		else
			pitch_limit = 32*1024;
	} else if (INTEL_INFO(dev)->gen >= 3) {
		if (obj->tiling_mode)
			pitch_limit = 8*1024;
		else
			pitch_limit = 16*1024;
	} else
		/* XXX DSPC is limited to 4k tiled */
		pitch_limit = 8*1024;

	if (mode_cmd->pitches[0] > pitch_limit) {
		DRM_DEBUG("%s pitch (%d) must be at less than %d\n",
			  obj->tiling_mode ? "tiled" : "linear",
			  mode_cmd->pitches[0], pitch_limit);
		return -EINVAL;
	}

	if (obj->tiling_mode != I915_TILING_NONE &&
	    mode_cmd->pitches[0] != obj->stride) {
		DRM_DEBUG("pitch (%d) must match tiling stride (%d)\n",
			  mode_cmd->pitches[0], obj->stride);
		return -EINVAL;
	}

	/* Reject formats not supported by any plane early. */
	switch (mode_cmd->pixel_format) {
	case DRM_FORMAT_C8:
	case DRM_FORMAT_RGB565:
	case DRM_FORMAT_XRGB8888:
	case DRM_FORMAT_ARGB8888:
		break;
	case DRM_FORMAT_XRGB1555:
	case DRM_FORMAT_ARGB1555:
		if (INTEL_INFO(dev)->gen > 3) {
			DRM_DEBUG("unsupported pixel format: %s\n",
				  drm_get_format_name(mode_cmd->pixel_format));
			return -EINVAL;
		}
		break;
	case DRM_FORMAT_XBGR8888:
	case DRM_FORMAT_ABGR8888:
	case DRM_FORMAT_XRGB2101010:
	case DRM_FORMAT_ARGB2101010:
	case DRM_FORMAT_XBGR2101010:
	case DRM_FORMAT_ABGR2101010:
		if (INTEL_INFO(dev)->gen < 4) {
			DRM_DEBUG("unsupported pixel format: %s\n",
				  drm_get_format_name(mode_cmd->pixel_format));
			return -EINVAL;
		}
		break;
	case DRM_FORMAT_YUYV:
	case DRM_FORMAT_UYVY:
	case DRM_FORMAT_YVYU:
	case DRM_FORMAT_VYUY:
		if (INTEL_INFO(dev)->gen < 5) {
			DRM_DEBUG("unsupported pixel format: %s\n",
				  drm_get_format_name(mode_cmd->pixel_format));
			return -EINVAL;
		}
		break;
	default:
		DRM_DEBUG("unsupported pixel format: %s\n",
			  drm_get_format_name(mode_cmd->pixel_format));
		return -EINVAL;
	}

	/* FIXME need to adjust LINOFF/TILEOFF accordingly. */
	if (mode_cmd->offsets[0] != 0)
		return -EINVAL;

	tile_height = IS_GEN2(dev) ? 16 : 8;
	aligned_height = ALIGN(mode_cmd->height,
			       obj->tiling_mode ? tile_height : 1);
	/* FIXME drm helper for size checks (especially planar formats)? */
	if (obj->base.size < aligned_height * mode_cmd->pitches[0])
		return -EINVAL;

	drm_helper_mode_fill_fb_struct(&intel_fb->base, mode_cmd);
	intel_fb->obj = obj;
	intel_fb->obj->framebuffer_references++;

	ret = drm_framebuffer_init(dev, &intel_fb->base, &intel_fb_funcs);
	if (ret) {
		DRM_ERROR("framebuffer init failed %d\n", ret);
		return ret;
	}

	return 0;
}

static struct drm_framebuffer *
intel_user_framebuffer_create(struct drm_device *dev,
			      struct drm_file *filp,
			      struct drm_mode_fb_cmd2 *mode_cmd)
{
	struct drm_i915_gem_object *obj;

	obj = to_intel_bo(drm_gem_object_lookup(dev, filp,
						mode_cmd->handles[0]));
	if (&obj->base == NULL)
		return ERR_PTR(-ENOENT);

	return intel_framebuffer_create(dev, mode_cmd, obj);
}

#ifndef CONFIG_DRM_I915_FBDEV
static inline void intel_fbdev_output_poll_changed(struct drm_device *dev)
{
}
#endif

static const struct drm_mode_config_funcs intel_mode_funcs = {
	.fb_create = intel_user_framebuffer_create,
	.output_poll_changed = intel_fbdev_output_poll_changed,
};
>>>>>>> refs/remotes/origin/master

/* Set up chip specific display functions */
static void intel_init_display(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

<<<<<<< HEAD
	/* We always want a DPMS function */
	if (HAS_PCH_SPLIT(dev)) {
		dev_priv->display.dpms = ironlake_crtc_dpms;
		dev_priv->display.crtc_mode_set = ironlake_crtc_mode_set;
<<<<<<< HEAD
	} else {
		dev_priv->display.dpms = i9xx_crtc_dpms;
		dev_priv->display.crtc_mode_set = i9xx_crtc_mode_set;
=======
		dev_priv->display.update_plane = ironlake_update_plane;
	} else {
		dev_priv->display.dpms = i9xx_crtc_dpms;
		dev_priv->display.crtc_mode_set = i9xx_crtc_mode_set;
		dev_priv->display.update_plane = i9xx_update_plane;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	if (I915_HAS_FBC(dev)) {
		if (HAS_PCH_SPLIT(dev)) {
			dev_priv->display.fbc_enabled = ironlake_fbc_enabled;
			dev_priv->display.enable_fbc = ironlake_enable_fbc;
			dev_priv->display.disable_fbc = ironlake_disable_fbc;
		} else if (IS_GM45(dev)) {
			dev_priv->display.fbc_enabled = g4x_fbc_enabled;
			dev_priv->display.enable_fbc = g4x_enable_fbc;
			dev_priv->display.disable_fbc = g4x_disable_fbc;
		} else if (IS_CRESTLINE(dev)) {
			dev_priv->display.fbc_enabled = i8xx_fbc_enabled;
			dev_priv->display.enable_fbc = i8xx_enable_fbc;
			dev_priv->display.disable_fbc = i8xx_disable_fbc;
		}
		/* 855GM needs testing */
	}

	/* Returns the core display clock speed */
<<<<<<< HEAD
	if (IS_I945G(dev) || (IS_G33(dev) && ! IS_PINEVIEW_M(dev)))
=======
	if (IS_I945G(dev) || (IS_G33(dev) && !IS_PINEVIEW_M(dev)))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (HAS_PCH_SPLIT(dev) || IS_G4X(dev))
		dev_priv->display.find_dpll = g4x_find_best_dpll;
	else if (IS_VALLEYVIEW(dev))
		dev_priv->display.find_dpll = vlv_find_best_dpll;
	else if (IS_PINEVIEW(dev))
		dev_priv->display.find_dpll = pnv_find_best_dpll;
	else
		dev_priv->display.find_dpll = i9xx_find_best_dpll;

	if (HAS_DDI(dev)) {
		dev_priv->display.get_pipe_config = haswell_get_pipe_config;
		dev_priv->display.crtc_mode_set = haswell_crtc_mode_set;
		dev_priv->display.crtc_enable = haswell_crtc_enable;
		dev_priv->display.crtc_disable = haswell_crtc_disable;
		dev_priv->display.off = haswell_crtc_off;
		dev_priv->display.update_plane = ironlake_update_plane;
	} else if (HAS_PCH_SPLIT(dev)) {
		dev_priv->display.get_pipe_config = ironlake_get_pipe_config;
		dev_priv->display.crtc_mode_set = ironlake_crtc_mode_set;
		dev_priv->display.crtc_enable = ironlake_crtc_enable;
		dev_priv->display.crtc_disable = ironlake_crtc_disable;
		dev_priv->display.off = ironlake_crtc_off;
		dev_priv->display.update_plane = ironlake_update_plane;
	} else if (IS_VALLEYVIEW(dev)) {
		dev_priv->display.get_pipe_config = i9xx_get_pipe_config;
		dev_priv->display.crtc_mode_set = i9xx_crtc_mode_set;
		dev_priv->display.crtc_enable = valleyview_crtc_enable;
		dev_priv->display.crtc_disable = i9xx_crtc_disable;
		dev_priv->display.off = i9xx_crtc_off;
		dev_priv->display.update_plane = i9xx_update_plane;
	} else {
		dev_priv->display.get_pipe_config = i9xx_get_pipe_config;
		dev_priv->display.crtc_mode_set = i9xx_crtc_mode_set;
		dev_priv->display.crtc_enable = i9xx_crtc_enable;
		dev_priv->display.crtc_disable = i9xx_crtc_disable;
		dev_priv->display.off = i9xx_crtc_off;
		dev_priv->display.update_plane = i9xx_update_plane;
	}

	/* Returns the core display clock speed */
	if (IS_VALLEYVIEW(dev))
		dev_priv->display.get_display_clock_speed =
			valleyview_get_display_clock_speed;
	else if (IS_I945G(dev) || (IS_G33(dev) && !IS_PINEVIEW_M(dev)))
>>>>>>> refs/remotes/origin/master
		dev_priv->display.get_display_clock_speed =
			i945_get_display_clock_speed;
	else if (IS_I915G(dev))
		dev_priv->display.get_display_clock_speed =
			i915_get_display_clock_speed;
<<<<<<< HEAD
	else if (IS_I945GM(dev) || IS_845G(dev) || IS_PINEVIEW_M(dev))
		dev_priv->display.get_display_clock_speed =
			i9xx_misc_get_display_clock_speed;
=======
	else if (IS_I945GM(dev) || IS_845G(dev))
		dev_priv->display.get_display_clock_speed =
			i9xx_misc_get_display_clock_speed;
	else if (IS_PINEVIEW(dev))
		dev_priv->display.get_display_clock_speed =
			pnv_get_display_clock_speed;
>>>>>>> refs/remotes/origin/master
	else if (IS_I915GM(dev))
		dev_priv->display.get_display_clock_speed =
			i915gm_get_display_clock_speed;
	else if (IS_I865G(dev))
		dev_priv->display.get_display_clock_speed =
			i865_get_display_clock_speed;
	else if (IS_I85X(dev))
		dev_priv->display.get_display_clock_speed =
			i855_get_display_clock_speed;
	else /* 852, 830 */
		dev_priv->display.get_display_clock_speed =
			i830_get_display_clock_speed;

<<<<<<< HEAD
	/* For FIFO watermark updates */
	if (HAS_PCH_SPLIT(dev)) {
<<<<<<< HEAD
=======
		dev_priv->display.force_wake_get = __gen6_gt_force_wake_get;
		dev_priv->display.force_wake_put = __gen6_gt_force_wake_put;

		/* IVB configs may use multi-threaded forcewake */
		if (IS_IVYBRIDGE(dev)) {
			u32	ecobus;

			/* A small trick here - if the bios hasn't configured MT forcewake,
			 * and if the device is in RC6, then force_wake_mt_get will not wake
			 * the device and the ECOBUS read will return zero. Which will be
			 * (correctly) interpreted by the test below as MT forcewake being
			 * disabled.
			 */
			mutex_lock(&dev->struct_mutex);
			__gen6_gt_force_wake_mt_get(dev_priv);
			ecobus = I915_READ_NOTRACE(ECOBUS);
			__gen6_gt_force_wake_mt_put(dev_priv);
			mutex_unlock(&dev->struct_mutex);

			if (ecobus & FORCEWAKE_MT_ENABLE) {
				DRM_DEBUG_KMS("Using MT version of forcewake\n");
				dev_priv->display.force_wake_get =
					__gen6_gt_force_wake_mt_get;
				dev_priv->display.force_wake_put =
					__gen6_gt_force_wake_mt_put;
			}
		}

>>>>>>> refs/remotes/origin/cm-10.0
		if (HAS_PCH_IBX(dev))
			dev_priv->display.init_pch_clock_gating = ibx_init_clock_gating;
		else if (HAS_PCH_CPT(dev))
			dev_priv->display.init_pch_clock_gating = cpt_init_clock_gating;

		if (IS_GEN5(dev)) {
			if (I915_READ(MLTR_ILK) & ILK_SRLT_MASK)
				dev_priv->display.update_wm = ironlake_update_wm;
			else {
				DRM_DEBUG_KMS("Failed to get proper latency. "
					      "Disable CxSR\n");
				dev_priv->display.update_wm = NULL;
			}
			dev_priv->display.fdi_link_train = ironlake_fdi_link_train;
			dev_priv->display.init_clock_gating = ironlake_init_clock_gating;
<<<<<<< HEAD
		} else if (IS_GEN6(dev)) {
			if (SNB_READ_WM0_LATENCY()) {
				dev_priv->display.update_wm = sandybridge_update_wm;
=======
			dev_priv->display.write_eld = ironlake_write_eld;
		} else if (IS_GEN6(dev)) {
			if (SNB_READ_WM0_LATENCY()) {
				dev_priv->display.update_wm = sandybridge_update_wm;
				dev_priv->display.update_sprite_wm = sandybridge_update_sprite_wm;
>>>>>>> refs/remotes/origin/cm-10.0
			} else {
				DRM_DEBUG_KMS("Failed to read display plane latency. "
					      "Disable CxSR\n");
				dev_priv->display.update_wm = NULL;
			}
			dev_priv->display.fdi_link_train = gen6_fdi_link_train;
			dev_priv->display.init_clock_gating = gen6_init_clock_gating;
<<<<<<< HEAD
=======
			dev_priv->display.write_eld = ironlake_write_eld;
>>>>>>> refs/remotes/origin/cm-10.0
		} else if (IS_IVYBRIDGE(dev)) {
			/* FIXME: detect B0+ stepping and use auto training */
			dev_priv->display.fdi_link_train = ivb_manual_fdi_link_train;
			if (SNB_READ_WM0_LATENCY()) {
				dev_priv->display.update_wm = sandybridge_update_wm;
<<<<<<< HEAD
=======
				dev_priv->display.update_sprite_wm = sandybridge_update_sprite_wm;
>>>>>>> refs/remotes/origin/cm-10.0
			} else {
				DRM_DEBUG_KMS("Failed to read display plane latency. "
					      "Disable CxSR\n");
				dev_priv->display.update_wm = NULL;
			}
			dev_priv->display.init_clock_gating = ivybridge_init_clock_gating;
<<<<<<< HEAD

=======
			dev_priv->display.write_eld = ironlake_write_eld;
>>>>>>> refs/remotes/origin/cm-10.0
		} else
			dev_priv->display.update_wm = NULL;
	} else if (IS_PINEVIEW(dev)) {
		if (!intel_get_cxsr_latency(IS_PINEVIEW_G(dev),
					    dev_priv->is_ddr3,
					    dev_priv->fsb_freq,
					    dev_priv->mem_freq)) {
			DRM_INFO("failed to find known CxSR latency "
				 "(found ddr%s fsb freq %d, mem freq %d), "
				 "disabling CxSR\n",
<<<<<<< HEAD
				 (dev_priv->is_ddr3 == 1) ? "3": "2",
=======
				 (dev_priv->is_ddr3 == 1) ? "3" : "2",
>>>>>>> refs/remotes/origin/cm-10.0
				 dev_priv->fsb_freq, dev_priv->mem_freq);
			/* Disable CxSR and never update its watermark again */
			pineview_disable_cxsr(dev);
			dev_priv->display.update_wm = NULL;
		} else
			dev_priv->display.update_wm = pineview_update_wm;
		dev_priv->display.init_clock_gating = gen3_init_clock_gating;
	} else if (IS_G4X(dev)) {
<<<<<<< HEAD
=======
		dev_priv->display.write_eld = g4x_write_eld;
>>>>>>> refs/remotes/origin/cm-10.0
		dev_priv->display.update_wm = g4x_update_wm;
		dev_priv->display.init_clock_gating = g4x_init_clock_gating;
	} else if (IS_GEN4(dev)) {
		dev_priv->display.update_wm = i965_update_wm;
		if (IS_CRESTLINE(dev))
			dev_priv->display.init_clock_gating = crestline_init_clock_gating;
		else if (IS_BROADWATER(dev))
			dev_priv->display.init_clock_gating = broadwater_init_clock_gating;
	} else if (IS_GEN3(dev)) {
		dev_priv->display.update_wm = i9xx_update_wm;
		dev_priv->display.get_fifo_size = i9xx_get_fifo_size;
		dev_priv->display.init_clock_gating = gen3_init_clock_gating;
	} else if (IS_I865G(dev)) {
		dev_priv->display.update_wm = i830_update_wm;
		dev_priv->display.init_clock_gating = i85x_init_clock_gating;
		dev_priv->display.get_fifo_size = i830_get_fifo_size;
	} else if (IS_I85X(dev)) {
		dev_priv->display.update_wm = i9xx_update_wm;
		dev_priv->display.get_fifo_size = i85x_get_fifo_size;
		dev_priv->display.init_clock_gating = i85x_init_clock_gating;
	} else {
		dev_priv->display.update_wm = i830_update_wm;
		dev_priv->display.init_clock_gating = i830_init_clock_gating;
		if (IS_845G(dev))
			dev_priv->display.get_fifo_size = i845_get_fifo_size;
		else
			dev_priv->display.get_fifo_size = i830_get_fifo_size;
	}
=======
	if (HAS_PCH_SPLIT(dev)) {
		if (IS_GEN5(dev)) {
			dev_priv->display.fdi_link_train = ironlake_fdi_link_train;
			dev_priv->display.write_eld = ironlake_write_eld;
		} else if (IS_GEN6(dev)) {
			dev_priv->display.fdi_link_train = gen6_fdi_link_train;
			dev_priv->display.write_eld = ironlake_write_eld;
		} else if (IS_IVYBRIDGE(dev)) {
			/* FIXME: detect B0+ stepping and use auto training */
			dev_priv->display.fdi_link_train = ivb_manual_fdi_link_train;
			dev_priv->display.write_eld = ironlake_write_eld;
			dev_priv->display.modeset_global_resources =
				ivb_modeset_global_resources;
		} else if (IS_HASWELL(dev) || IS_GEN8(dev)) {
			dev_priv->display.fdi_link_train = hsw_fdi_link_train;
			dev_priv->display.write_eld = haswell_write_eld;
			dev_priv->display.modeset_global_resources =
				haswell_modeset_global_resources;
		}
	} else if (IS_G4X(dev)) {
		dev_priv->display.write_eld = g4x_write_eld;
	} else if (IS_VALLEYVIEW(dev))
		dev_priv->display.write_eld = ironlake_write_eld;
>>>>>>> refs/remotes/origin/master

	/* Default just returns -ENODEV to indicate unsupported */
	dev_priv->display.queue_flip = intel_default_queue_flip;

	switch (INTEL_INFO(dev)->gen) {
	case 2:
		dev_priv->display.queue_flip = intel_gen2_queue_flip;
		break;

	case 3:
		dev_priv->display.queue_flip = intel_gen3_queue_flip;
		break;

	case 4:
	case 5:
		dev_priv->display.queue_flip = intel_gen4_queue_flip;
		break;

	case 6:
		dev_priv->display.queue_flip = intel_gen6_queue_flip;
		break;
	case 7:
<<<<<<< HEAD
=======
	case 8: /* FIXME(BDW): Check that the gen8 RCS flip works. */
>>>>>>> refs/remotes/origin/master
		dev_priv->display.queue_flip = intel_gen7_queue_flip;
		break;
	}
}

/*
 * Some BIOSes insist on assuming the GPU's pipe A is enabled at suspend,
 * resume, or other times.  This quirk makes sure that's the case for
 * affected systems.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void quirk_pipea_force (struct drm_device *dev)
=======
static void quirk_pipea_force(struct drm_device *dev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void quirk_pipea_force(struct drm_device *dev)
>>>>>>> refs/remotes/origin/master
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	dev_priv->quirks |= QUIRK_PIPEA_FORCE;
<<<<<<< HEAD
	DRM_DEBUG_DRIVER("applying pipe a force quirk\n");
=======
	DRM_INFO("applying pipe a force quirk\n");
>>>>>>> refs/remotes/origin/master
}

/*
 * Some machines (Lenovo U160) do not work with SSC on LVDS for some reason
 */
static void quirk_ssc_force_disable(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	dev_priv->quirks |= QUIRK_LVDS_SSC_DISABLE;
<<<<<<< HEAD
}

<<<<<<< HEAD
=======
=======
	DRM_INFO("applying lvds SSC disable quirk\n");
}

/*
 * A machine (e.g. Acer Aspire 5734Z) may need to invert the panel backlight
 * brightness value
 */
static void quirk_invert_brightness(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	dev_priv->quirks |= QUIRK_INVERT_BRIGHTNESS;
	DRM_INFO("applying inverted panel brightness quirk\n");
}

>>>>>>> refs/remotes/origin/master
/*
 * Some machines (Dell XPS13) suffer broken backlight controls if
 * BLM_PCH_PWM_ENABLE is set.
 */
static void quirk_no_pcm_pwm_enable(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	dev_priv->quirks |= QUIRK_NO_PCH_PWM_ENABLE;
	DRM_INFO("applying no-PCH_PWM_ENABLE quirk\n");
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct intel_quirk {
	int device;
	int subsystem_vendor;
	int subsystem_device;
	void (*hook)(struct drm_device *dev);
};

<<<<<<< HEAD
struct intel_quirk intel_quirks[] = {
<<<<<<< HEAD
	/* HP Compaq 2730p needs pipe A force quirk (LP: #291555) */
	{ 0x2a42, 0x103c, 0x30eb, quirk_pipea_force },
	/* HP Mini needs pipe A force quirk (LP: #322104) */
	{ 0x27ae,0x103c, 0x361a, quirk_pipea_force },
=======
	/* HP Mini needs pipe A force quirk (LP: #322104) */
	{ 0x27ae, 0x103c, 0x361a, quirk_pipea_force },
>>>>>>> refs/remotes/origin/cm-10.0

	/* Thinkpad R31 needs pipe A force quirk */
	{ 0x3577, 0x1014, 0x0505, quirk_pipea_force },
	/* Toshiba Protege R-205, S-209 needs pipe A force quirk */
	{ 0x2592, 0x1179, 0x0001, quirk_pipea_force },

	/* ThinkPad X30 needs pipe A force quirk (LP: #304614) */
	{ 0x3577,  0x1014, 0x0513, quirk_pipea_force },
	/* ThinkPad X40 needs pipe A force quirk */

	/* ThinkPad T60 needs pipe A force quirk (bug #16494) */
	{ 0x2782, 0x17aa, 0x201a, quirk_pipea_force },

	/* 855 & before need to leave pipe A & dpll A up */
	{ 0x3582, PCI_ANY_ID, PCI_ANY_ID, quirk_pipea_force },
	{ 0x2562, PCI_ANY_ID, PCI_ANY_ID, quirk_pipea_force },

	/* Lenovo U160 cannot use SSC on LVDS */
	{ 0x0046, 0x17aa, 0x3920, quirk_ssc_force_disable },
<<<<<<< HEAD
=======
=======
/* For systems that don't have a meaningful PCI subdevice/subvendor ID */
struct intel_dmi_quirk {
	void (*hook)(struct drm_device *dev);
	const struct dmi_system_id (*dmi_id_list)[];
};

static int intel_dmi_reverse_brightness(const struct dmi_system_id *id)
{
	DRM_INFO("Backlight polarity reversed on %s\n", id->ident);
	return 1;
}

static const struct intel_dmi_quirk intel_dmi_quirks[] = {
	{
		.dmi_id_list = &(const struct dmi_system_id[]) {
			{
				.callback = intel_dmi_reverse_brightness,
				.ident = "NCR Corporation",
				.matches = {DMI_MATCH(DMI_SYS_VENDOR, "NCR Corporation"),
					    DMI_MATCH(DMI_PRODUCT_NAME, ""),
				},
			},
			{ }  /* terminating entry */
		},
		.hook = quirk_invert_brightness,
	},
};

static struct intel_quirk intel_quirks[] = {
	/* HP Mini needs pipe A force quirk (LP: #322104) */
	{ 0x27ae, 0x103c, 0x361a, quirk_pipea_force },

	/* Toshiba Protege R-205, S-209 needs pipe A force quirk */
	{ 0x2592, 0x1179, 0x0001, quirk_pipea_force },

	/* ThinkPad T60 needs pipe A force quirk (bug #16494) */
	{ 0x2782, 0x17aa, 0x201a, quirk_pipea_force },

	/* 830 needs to leave pipe A & dpll A up */
	{ 0x3577, PCI_ANY_ID, PCI_ANY_ID, quirk_pipea_force },

	/* Lenovo U160 cannot use SSC on LVDS */
	{ 0x0046, 0x17aa, 0x3920, quirk_ssc_force_disable },
>>>>>>> refs/remotes/origin/master

	/* Sony Vaio Y cannot use SSC on LVDS */
	{ 0x0046, 0x104d, 0x9076, quirk_ssc_force_disable },

<<<<<<< HEAD
=======
	/* Acer Aspire 5734Z must invert backlight brightness */
	{ 0x2a42, 0x1025, 0x0459, quirk_invert_brightness },

	/* Acer/eMachines G725 */
	{ 0x2a42, 0x1025, 0x0210, quirk_invert_brightness },

	/* Acer/eMachines e725 */
	{ 0x2a42, 0x1025, 0x0212, quirk_invert_brightness },

	/* Acer/Packard Bell NCL20 */
	{ 0x2a42, 0x1025, 0x034b, quirk_invert_brightness },

	/* Acer Aspire 4736Z */
	{ 0x2a42, 0x1025, 0x0260, quirk_invert_brightness },

>>>>>>> refs/remotes/origin/master
	/* Dell XPS13 HD Sandy Bridge */
	{ 0x0116, 0x1028, 0x052e, quirk_no_pcm_pwm_enable },
	/* Dell XPS13 HD and XPS13 FHD Ivy Bridge */
	{ 0x0166, 0x1028, 0x058b, quirk_no_pcm_pwm_enable },
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

static void intel_init_quirks(struct drm_device *dev)
{
	struct pci_dev *d = dev->pdev;
	int i;

	for (i = 0; i < ARRAY_SIZE(intel_quirks); i++) {
		struct intel_quirk *q = &intel_quirks[i];

		if (d->device == q->device &&
		    (d->subsystem_vendor == q->subsystem_vendor ||
		     q->subsystem_vendor == PCI_ANY_ID) &&
		    (d->subsystem_device == q->subsystem_device ||
		     q->subsystem_device == PCI_ANY_ID))
			q->hook(dev);
	}
<<<<<<< HEAD
=======
	for (i = 0; i < ARRAY_SIZE(intel_dmi_quirks); i++) {
		if (dmi_check_system(*intel_dmi_quirks[i].dmi_id_list) != 0)
			intel_dmi_quirks[i].hook(dev);
	}
>>>>>>> refs/remotes/origin/master
}

/* Disable the VGA plane that we never use */
static void i915_disable_vga(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u8 sr1;
<<<<<<< HEAD
	u32 vga_reg;

	if (HAS_PCH_SPLIT(dev))
		vga_reg = CPU_VGACNTRL;
	else
		vga_reg = VGACNTRL;

	vga_get_uninterruptible(dev->pdev, VGA_RSRC_LEGACY_IO);
	outb(1, VGA_SR_INDEX);
=======
	u32 vga_reg = i915_vgacntrl_reg(dev);

	vga_get_uninterruptible(dev->pdev, VGA_RSRC_LEGACY_IO);
	outb(SR01, VGA_SR_INDEX);
>>>>>>> refs/remotes/origin/master
	sr1 = inb(VGA_SR_DATA);
	outb(sr1 | 1<<5, VGA_SR_DATA);
	vga_put(dev->pdev, VGA_RSRC_LEGACY_IO);
	udelay(300);

	I915_WRITE(vga_reg, VGA_DISP_DISABLE);
	POSTING_READ(vga_reg);
}

<<<<<<< HEAD
void intel_modeset_init(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
<<<<<<< HEAD
	int i;
=======
	int i, ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
void intel_modeset_init_hw(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	intel_prepare_ddi(dev);

	intel_init_clock_gating(dev);

	/* Enable the CRI clock source so we can get at the display */
	if (IS_VALLEYVIEW(dev))
		I915_WRITE(DPLL(PIPE_B), I915_READ(DPLL(PIPE_B)) |
			   DPLL_INTEGRATED_CRI_CLK_VLV);

	intel_init_dpio(dev);

	mutex_lock(&dev->struct_mutex);
	intel_enable_gt_powersave(dev);
	mutex_unlock(&dev->struct_mutex);
}

void intel_modeset_suspend_hw(struct drm_device *dev)
{
	intel_suspend_hw(dev);
}

void intel_modeset_init(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int i, j, ret;
>>>>>>> refs/remotes/origin/master

	drm_mode_config_init(dev);

	dev->mode_config.min_width = 0;
	dev->mode_config.min_height = 0;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	dev->mode_config.preferred_depth = 24;
	dev->mode_config.prefer_shadow = 1;

>>>>>>> refs/remotes/origin/cm-10.0
	dev->mode_config.funcs = (void *)&intel_mode_funcs;

	intel_init_quirks(dev);

=======
	dev->mode_config.preferred_depth = 24;
	dev->mode_config.prefer_shadow = 1;

	dev->mode_config.funcs = &intel_mode_funcs;

	intel_init_quirks(dev);

	intel_init_pm(dev);

	if (INTEL_INFO(dev)->num_pipes == 0)
		return;

>>>>>>> refs/remotes/origin/master
	intel_init_display(dev);

	if (IS_GEN2(dev)) {
		dev->mode_config.max_width = 2048;
		dev->mode_config.max_height = 2048;
	} else if (IS_GEN3(dev)) {
		dev->mode_config.max_width = 4096;
		dev->mode_config.max_height = 4096;
	} else {
		dev->mode_config.max_width = 8192;
		dev->mode_config.max_height = 8192;
	}
<<<<<<< HEAD
	dev->mode_config.fb_base = dev->agp->base;

	DRM_DEBUG_KMS("%d display pipe%s available.\n",
		      dev_priv->num_pipe, dev_priv->num_pipe > 1 ? "s" : "");

	for (i = 0; i < dev_priv->num_pipe; i++) {
		intel_crtc_init(dev, i);
<<<<<<< HEAD
=======
		ret = intel_plane_init(dev, i);
		if (ret)
			DRM_DEBUG_KMS("plane %d init failed: %d\n", i, ret);
>>>>>>> refs/remotes/origin/cm-10.0
	}

=======
	dev->mode_config.fb_base = dev_priv->gtt.mappable_base;

	DRM_DEBUG_KMS("%d display pipe%s available.\n",
		      INTEL_INFO(dev)->num_pipes,
		      INTEL_INFO(dev)->num_pipes > 1 ? "s" : "");

	for_each_pipe(i) {
		intel_crtc_init(dev, i);
		for (j = 0; j < dev_priv->num_plane; j++) {
			ret = intel_plane_init(dev, i, j);
			if (ret)
				DRM_DEBUG_KMS("pipe %c sprite %c init failed: %d\n",
					      pipe_name(i), sprite_name(i, j), ret);
		}
	}

	intel_cpu_pll_init(dev);
	intel_shared_dpll_init(dev);

>>>>>>> refs/remotes/origin/master
	/* Just disable it once at startup */
	i915_disable_vga(dev);
	intel_setup_outputs(dev);

<<<<<<< HEAD
	intel_init_clock_gating(dev);

	if (IS_IRONLAKE_M(dev)) {
		ironlake_enable_drps(dev);
		intel_init_emon(dev);
	}

<<<<<<< HEAD
	if (IS_GEN6(dev) || IS_GEN7(dev))
		gen6_enable_rps(dev_priv);
=======
	if (IS_GEN6(dev) || IS_GEN7(dev)) {
		gen6_enable_rps(dev_priv);
		gen6_update_ring_freq(dev_priv);
	}
>>>>>>> refs/remotes/origin/cm-10.0

	INIT_WORK(&dev_priv->idle_work, intel_idle_update);
	setup_timer(&dev_priv->idle_timer, intel_gpu_idle_timer,
		    (unsigned long)dev);
=======
	/* Just in case the BIOS is doing something questionable. */
	intel_disable_fbc(dev);
}

static void
intel_connector_break_all_links(struct intel_connector *connector)
{
	connector->base.dpms = DRM_MODE_DPMS_OFF;
	connector->base.encoder = NULL;
	connector->encoder->connectors_active = false;
	connector->encoder->base.crtc = NULL;
}

static void intel_enable_pipe_a(struct drm_device *dev)
{
	struct intel_connector *connector;
	struct drm_connector *crt = NULL;
	struct intel_load_detect_pipe load_detect_temp;

	/* We can't just switch on the pipe A, we need to set things up with a
	 * proper mode and output configuration. As a gross hack, enable pipe A
	 * by enabling the load detect pipe once. */
	list_for_each_entry(connector,
			    &dev->mode_config.connector_list,
			    base.head) {
		if (connector->encoder->type == INTEL_OUTPUT_ANALOG) {
			crt = &connector->base;
			break;
		}
	}

	if (!crt)
		return;

	if (intel_get_load_detect_pipe(crt, NULL, &load_detect_temp))
		intel_release_load_detect_pipe(crt, &load_detect_temp);


}

static bool
intel_check_plane_mapping(struct intel_crtc *crtc)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 reg, val;

	if (INTEL_INFO(dev)->num_pipes == 1)
		return true;

	reg = DSPCNTR(!crtc->plane);
	val = I915_READ(reg);

	if ((val & DISPLAY_PLANE_ENABLE) &&
	    (!!(val & DISPPLANE_SEL_PIPE_MASK) == crtc->pipe))
		return false;

	return true;
}

static void intel_sanitize_crtc(struct intel_crtc *crtc)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 reg;

	/* Clear any frame start delays used for debugging left by the BIOS */
	reg = PIPECONF(crtc->config.cpu_transcoder);
	I915_WRITE(reg, I915_READ(reg) & ~PIPECONF_FRAME_START_DELAY_MASK);

	/* We need to sanitize the plane -> pipe mapping first because this will
	 * disable the crtc (and hence change the state) if it is wrong. Note
	 * that gen4+ has a fixed plane -> pipe mapping.  */
	if (INTEL_INFO(dev)->gen < 4 && !intel_check_plane_mapping(crtc)) {
		struct intel_connector *connector;
		bool plane;

		DRM_DEBUG_KMS("[CRTC:%d] wrong plane connection detected!\n",
			      crtc->base.base.id);

		/* Pipe has the wrong plane attached and the plane is active.
		 * Temporarily change the plane mapping and disable everything
		 * ...  */
		plane = crtc->plane;
		crtc->plane = !plane;
		dev_priv->display.crtc_disable(&crtc->base);
		crtc->plane = plane;

		/* ... and break all links. */
		list_for_each_entry(connector, &dev->mode_config.connector_list,
				    base.head) {
			if (connector->encoder->base.crtc != &crtc->base)
				continue;

			intel_connector_break_all_links(connector);
		}

		WARN_ON(crtc->active);
		crtc->base.enabled = false;
	}

	if (dev_priv->quirks & QUIRK_PIPEA_FORCE &&
	    crtc->pipe == PIPE_A && !crtc->active) {
		/* BIOS forgot to enable pipe A, this mostly happens after
		 * resume. Force-enable the pipe to fix this, the update_dpms
		 * call below we restore the pipe to the right state, but leave
		 * the required bits on. */
		intel_enable_pipe_a(dev);
	}

	/* Adjust the state of the output pipe according to whether we
	 * have active connectors/encoders. */
	intel_crtc_update_dpms(&crtc->base);

	if (crtc->active != crtc->base.enabled) {
		struct intel_encoder *encoder;

		/* This can happen either due to bugs in the get_hw_state
		 * functions or because the pipe is force-enabled due to the
		 * pipe A quirk. */
		DRM_DEBUG_KMS("[CRTC:%d] hw state adjusted, was %s, now %s\n",
			      crtc->base.base.id,
			      crtc->base.enabled ? "enabled" : "disabled",
			      crtc->active ? "enabled" : "disabled");

		crtc->base.enabled = crtc->active;

		/* Because we only establish the connector -> encoder ->
		 * crtc links if something is active, this means the
		 * crtc is now deactivated. Break the links. connector
		 * -> encoder links are only establish when things are
		 *  actually up, hence no need to break them. */
		WARN_ON(crtc->active);

		for_each_encoder_on_crtc(dev, &crtc->base, encoder) {
			WARN_ON(encoder->connectors_active);
			encoder->base.crtc = NULL;
		}
	}
}

static void intel_sanitize_encoder(struct intel_encoder *encoder)
{
	struct intel_connector *connector;
	struct drm_device *dev = encoder->base.dev;

	/* We need to check both for a crtc link (meaning that the
	 * encoder is active and trying to read from a pipe) and the
	 * pipe itself being active. */
	bool has_active_crtc = encoder->base.crtc &&
		to_intel_crtc(encoder->base.crtc)->active;

	if (encoder->connectors_active && !has_active_crtc) {
		DRM_DEBUG_KMS("[ENCODER:%d:%s] has active connectors but no active pipe!\n",
			      encoder->base.base.id,
			      drm_get_encoder_name(&encoder->base));

		/* Connector is active, but has no active pipe. This is
		 * fallout from our resume register restoring. Disable
		 * the encoder manually again. */
		if (encoder->base.crtc) {
			DRM_DEBUG_KMS("[ENCODER:%d:%s] manually disabled\n",
				      encoder->base.base.id,
				      drm_get_encoder_name(&encoder->base));
			encoder->disable(encoder);
		}

		/* Inconsistent output/port/pipe state happens presumably due to
		 * a bug in one of the get_hw_state functions. Or someplace else
		 * in our code, like the register restore mess on resume. Clamp
		 * things to off as a safer default. */
		list_for_each_entry(connector,
				    &dev->mode_config.connector_list,
				    base.head) {
			if (connector->encoder != encoder)
				continue;

			intel_connector_break_all_links(connector);
		}
	}
	/* Enabled encoders without active connectors will be fixed in
	 * the crtc fixup. */
}

void i915_redisable_vga(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 vga_reg = i915_vgacntrl_reg(dev);

	/* This function can be called both from intel_modeset_setup_hw_state or
	 * at a very early point in our resume sequence, where the power well
	 * structures are not yet restored. Since this function is at a very
	 * paranoid "someone might have enabled VGA while we were not looking"
	 * level, just check if the power well is enabled instead of trying to
	 * follow the "don't touch the power well if we don't need it" policy
	 * the rest of the driver uses. */
	if (HAS_POWER_WELL(dev) &&
	    (I915_READ(HSW_PWR_WELL_DRIVER) & HSW_PWR_WELL_STATE_ENABLED) == 0)
		return;

	if (!(I915_READ(vga_reg) & VGA_DISP_DISABLE)) {
		DRM_DEBUG_KMS("Something enabled VGA plane, disabling it\n");
		i915_disable_vga(dev);
	}
}

static void intel_modeset_readout_hw_state(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	enum pipe pipe;
	struct intel_crtc *crtc;
	struct intel_encoder *encoder;
	struct intel_connector *connector;
	int i;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list,
			    base.head) {
		memset(&crtc->config, 0, sizeof(crtc->config));

		crtc->active = dev_priv->display.get_pipe_config(crtc,
								 &crtc->config);

		crtc->base.enabled = crtc->active;
		crtc->primary_enabled = crtc->active;

		DRM_DEBUG_KMS("[CRTC:%d] hw state readout: %s\n",
			      crtc->base.base.id,
			      crtc->active ? "enabled" : "disabled");
	}

	/* FIXME: Smash this into the new shared dpll infrastructure. */
	if (HAS_DDI(dev))
		intel_ddi_setup_hw_pll_state(dev);

	for (i = 0; i < dev_priv->num_shared_dpll; i++) {
		struct intel_shared_dpll *pll = &dev_priv->shared_dplls[i];

		pll->on = pll->get_hw_state(dev_priv, pll, &pll->hw_state);
		pll->active = 0;
		list_for_each_entry(crtc, &dev->mode_config.crtc_list,
				    base.head) {
			if (crtc->active && intel_crtc_to_shared_dpll(crtc) == pll)
				pll->active++;
		}
		pll->refcount = pll->active;

		DRM_DEBUG_KMS("%s hw state readout: refcount %i, on %i\n",
			      pll->name, pll->refcount, pll->on);
	}

	list_for_each_entry(encoder, &dev->mode_config.encoder_list,
			    base.head) {
		pipe = 0;

		if (encoder->get_hw_state(encoder, &pipe)) {
			crtc = to_intel_crtc(dev_priv->pipe_to_crtc_mapping[pipe]);
			encoder->base.crtc = &crtc->base;
			encoder->get_config(encoder, &crtc->config);
		} else {
			encoder->base.crtc = NULL;
		}

		encoder->connectors_active = false;
		DRM_DEBUG_KMS("[ENCODER:%d:%s] hw state readout: %s, pipe %c\n",
			      encoder->base.base.id,
			      drm_get_encoder_name(&encoder->base),
			      encoder->base.crtc ? "enabled" : "disabled",
			      pipe_name(pipe));
	}

	list_for_each_entry(connector, &dev->mode_config.connector_list,
			    base.head) {
		if (connector->get_hw_state(connector)) {
			connector->base.dpms = DRM_MODE_DPMS_ON;
			connector->encoder->connectors_active = true;
			connector->base.encoder = &connector->encoder->base;
		} else {
			connector->base.dpms = DRM_MODE_DPMS_OFF;
			connector->base.encoder = NULL;
		}
		DRM_DEBUG_KMS("[CONNECTOR:%d:%s] hw state readout: %s\n",
			      connector->base.base.id,
			      drm_get_connector_name(&connector->base),
			      connector->base.encoder ? "enabled" : "disabled");
	}
}

/* Scan out the current hw modeset state, sanitizes it and maps it into the drm
 * and i915 state tracking structures. */
void intel_modeset_setup_hw_state(struct drm_device *dev,
				  bool force_restore)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	enum pipe pipe;
	struct intel_crtc *crtc;
	struct intel_encoder *encoder;
	int i;

	intel_modeset_readout_hw_state(dev);

	/*
	 * Now that we have the config, copy it to each CRTC struct
	 * Note that this could go away if we move to using crtc_config
	 * checking everywhere.
	 */
	list_for_each_entry(crtc, &dev->mode_config.crtc_list,
			    base.head) {
		if (crtc->active && i915_fastboot) {
			intel_crtc_mode_from_pipe_config(crtc, &crtc->config);

			DRM_DEBUG_KMS("[CRTC:%d] found active mode: ",
				      crtc->base.base.id);
			drm_mode_debug_printmodeline(&crtc->base.mode);
		}
	}

	/* HW state is read out, now we need to sanitize this mess. */
	list_for_each_entry(encoder, &dev->mode_config.encoder_list,
			    base.head) {
		intel_sanitize_encoder(encoder);
	}

	for_each_pipe(pipe) {
		crtc = to_intel_crtc(dev_priv->pipe_to_crtc_mapping[pipe]);
		intel_sanitize_crtc(crtc);
		intel_dump_pipe_config(crtc, &crtc->config, "[setup_hw_state]");
	}

	for (i = 0; i < dev_priv->num_shared_dpll; i++) {
		struct intel_shared_dpll *pll = &dev_priv->shared_dplls[i];

		if (!pll->on || pll->active)
			continue;

		DRM_DEBUG_KMS("%s enabled but not in use, disabling\n", pll->name);

		pll->disable(dev_priv, pll);
		pll->on = false;
	}

	if (IS_HASWELL(dev))
		ilk_wm_get_hw_state(dev);

	if (force_restore) {
		i915_redisable_vga(dev);

		/*
		 * We need to use raw interfaces for restoring state to avoid
		 * checking (bogus) intermediate states.
		 */
		for_each_pipe(pipe) {
			struct drm_crtc *crtc =
				dev_priv->pipe_to_crtc_mapping[pipe];

			__intel_set_mode(crtc, &crtc->mode, crtc->x, crtc->y,
					 crtc->fb);
		}
	} else {
		intel_modeset_update_staged_output_state(dev);
	}

	intel_modeset_check_state(dev);
>>>>>>> refs/remotes/origin/master
}

void intel_modeset_gem_init(struct drm_device *dev)
{
<<<<<<< HEAD
	if (IS_IRONLAKE_M(dev))
		ironlake_enable_rc6(dev);

	intel_setup_overlay(dev);
=======
	intel_modeset_init_hw(dev);

	intel_setup_overlay(dev);

	mutex_lock(&dev->mode_config.mutex);
	drm_mode_config_reset(dev);
	intel_modeset_setup_hw_state(dev, false);
	mutex_unlock(&dev->mode_config.mutex);
>>>>>>> refs/remotes/origin/master
}

void intel_modeset_cleanup(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_crtc *crtc;
<<<<<<< HEAD
	struct intel_crtc *intel_crtc;

	drm_kms_helper_poll_fini(dev);
=======
	struct drm_connector *connector;

	/*
	 * Interrupts and polling as the first thing to avoid creating havoc.
	 * Too much stuff here (turning of rps, connectors, ...) would
	 * experience fancy races otherwise.
	 */
	drm_irq_uninstall(dev);
	cancel_work_sync(&dev_priv->hotplug_work);
	/*
	 * Due to the hpd irq storm handling the hotplug work can re-arm the
	 * poll handlers. Hence disable polling after hpd handling is shut down.
	 */
	drm_kms_helper_poll_fini(dev);

>>>>>>> refs/remotes/origin/master
	mutex_lock(&dev->struct_mutex);

	intel_unregister_dsm_handler();

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		/* Skip inactive CRTCs */
		if (!crtc->fb)
			continue;

<<<<<<< HEAD
		intel_crtc = to_intel_crtc(crtc);
		intel_increase_pllclock(crtc);
	}

<<<<<<< HEAD
	if (dev_priv->display.disable_fbc)
		dev_priv->display.disable_fbc(dev);
=======
	intel_disable_fbc(dev);
>>>>>>> refs/remotes/origin/cm-10.0

	if (IS_IRONLAKE_M(dev))
		ironlake_disable_drps(dev);
	if (IS_GEN6(dev) || IS_GEN7(dev))
		gen6_disable_rps(dev);

	if (IS_IRONLAKE_M(dev))
		ironlake_disable_rc6(dev);

	mutex_unlock(&dev->struct_mutex);

	/* Disable the irq before mode object teardown, for the irq might
	 * enqueue unpin/hotplug work. */
	drm_irq_uninstall(dev);
	cancel_work_sync(&dev_priv->hotplug_work);
<<<<<<< HEAD
=======
	cancel_work_sync(&dev_priv->rps_work);

	/* flush any delayed tasks or pending work */
	flush_scheduled_work();
>>>>>>> refs/remotes/origin/cm-10.0

	/* Shut off idle work before the crtcs get freed. */
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		intel_crtc = to_intel_crtc(crtc);
		del_timer_sync(&intel_crtc->idle_timer);
	}
	del_timer_sync(&dev_priv->idle_timer);
	cancel_work_sync(&dev_priv->idle_work);

	drm_mode_config_cleanup(dev);
=======
		intel_increase_pllclock(crtc);
	}

	intel_disable_fbc(dev);

	intel_disable_gt_powersave(dev);

	ironlake_teardown_rc6(dev);

	mutex_unlock(&dev->struct_mutex);

	/* flush any delayed tasks or pending work */
	flush_scheduled_work();

	/* destroy backlight, if any, before the connectors */
	intel_panel_destroy_backlight(dev);

	/* destroy the sysfs files before encoders/connectors */
	list_for_each_entry(connector, &dev->mode_config.connector_list, head)
		drm_sysfs_connector_remove(connector);

	drm_mode_config_cleanup(dev);

	intel_cleanup_overlay(dev);
>>>>>>> refs/remotes/origin/master
}

/*
 * Return which encoder is currently attached for connector.
 */
struct drm_encoder *intel_best_encoder(struct drm_connector *connector)
{
	return &intel_attached_encoder(connector)->base;
}

void intel_connector_attach_encoder(struct intel_connector *connector,
				    struct intel_encoder *encoder)
{
	connector->encoder = encoder;
	drm_mode_connector_attach_encoder(&connector->base,
					  &encoder->base);
}

/*
 * set vga decode state - true == enable VGA decode
 */
int intel_modeset_vga_set_state(struct drm_device *dev, bool state)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
<<<<<<< HEAD
	u16 gmch_ctrl;

	pci_read_config_word(dev_priv->bridge_dev, INTEL_GMCH_CTRL, &gmch_ctrl);
=======
	unsigned reg = INTEL_INFO(dev)->gen >= 6 ? SNB_GMCH_CTRL : INTEL_GMCH_CTRL;
	u16 gmch_ctrl;

	pci_read_config_word(dev_priv->bridge_dev, reg, &gmch_ctrl);
>>>>>>> refs/remotes/origin/master
	if (state)
		gmch_ctrl &= ~INTEL_GMCH_VGA_DISABLE;
	else
		gmch_ctrl |= INTEL_GMCH_VGA_DISABLE;
<<<<<<< HEAD
	pci_write_config_word(dev_priv->bridge_dev, INTEL_GMCH_CTRL, gmch_ctrl);
	return 0;
}

#ifdef CONFIG_DEBUG_FS
#include <linux/seq_file.h>

struct intel_display_error_state {
=======
	pci_write_config_word(dev_priv->bridge_dev, reg, gmch_ctrl);
	return 0;
}

struct intel_display_error_state {

	u32 power_well_driver;

	int num_transcoders;

>>>>>>> refs/remotes/origin/master
	struct intel_cursor_error_state {
		u32 control;
		u32 position;
		u32 base;
		u32 size;
<<<<<<< HEAD
	} cursor[2];

	struct intel_pipe_error_state {
		u32 conf;
		u32 source;

		u32 htotal;
		u32 hblank;
		u32 hsync;
		u32 vtotal;
		u32 vblank;
		u32 vsync;
	} pipe[2];
=======
	} cursor[I915_MAX_PIPES];

	struct intel_pipe_error_state {
		u32 source;
	} pipe[I915_MAX_PIPES];
>>>>>>> refs/remotes/origin/master

	struct intel_plane_error_state {
		u32 control;
		u32 stride;
		u32 size;
		u32 pos;
		u32 addr;
		u32 surface;
		u32 tile_offset;
<<<<<<< HEAD
	} plane[2];
=======
	} plane[I915_MAX_PIPES];

	struct intel_transcoder_error_state {
		enum transcoder cpu_transcoder;

		u32 conf;

		u32 htotal;
		u32 hblank;
		u32 hsync;
		u32 vtotal;
		u32 vblank;
		u32 vsync;
	} transcoder[4];
>>>>>>> refs/remotes/origin/master
};

struct intel_display_error_state *
intel_display_capture_error_state(struct drm_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
        drm_i915_private_t *dev_priv = dev->dev_private;
=======
	drm_i915_private_t *dev_priv = dev->dev_private;
>>>>>>> refs/remotes/origin/cm-10.0
	struct intel_display_error_state *error;
	int i;

	error = kmalloc(sizeof(*error), GFP_ATOMIC);
	if (error == NULL)
		return NULL;

	for (i = 0; i < 2; i++) {
		error->cursor[i].control = I915_READ(CURCNTR(i));
		error->cursor[i].position = I915_READ(CURPOS(i));
		error->cursor[i].base = I915_READ(CURBASE(i));

		error->plane[i].control = I915_READ(DSPCNTR(i));
		error->plane[i].stride = I915_READ(DSPSTRIDE(i));
		error->plane[i].size = I915_READ(DSPSIZE(i));
<<<<<<< HEAD
		error->plane[i].pos= I915_READ(DSPPOS(i));
=======
		error->plane[i].pos = I915_READ(DSPPOS(i));
>>>>>>> refs/remotes/origin/cm-10.0
		error->plane[i].addr = I915_READ(DSPADDR(i));
=======
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_display_error_state *error;
	int transcoders[] = {
		TRANSCODER_A,
		TRANSCODER_B,
		TRANSCODER_C,
		TRANSCODER_EDP,
	};
	int i;

	if (INTEL_INFO(dev)->num_pipes == 0)
		return NULL;

	error = kzalloc(sizeof(*error), GFP_ATOMIC);
	if (error == NULL)
		return NULL;

	if (HAS_POWER_WELL(dev))
		error->power_well_driver = I915_READ(HSW_PWR_WELL_DRIVER);

	for_each_pipe(i) {
		if (!intel_display_power_enabled(dev, POWER_DOMAIN_PIPE(i)))
			continue;

		if (INTEL_INFO(dev)->gen <= 6 || IS_VALLEYVIEW(dev)) {
			error->cursor[i].control = I915_READ(CURCNTR(i));
			error->cursor[i].position = I915_READ(CURPOS(i));
			error->cursor[i].base = I915_READ(CURBASE(i));
		} else {
			error->cursor[i].control = I915_READ(CURCNTR_IVB(i));
			error->cursor[i].position = I915_READ(CURPOS_IVB(i));
			error->cursor[i].base = I915_READ(CURBASE_IVB(i));
		}

		error->plane[i].control = I915_READ(DSPCNTR(i));
		error->plane[i].stride = I915_READ(DSPSTRIDE(i));
		if (INTEL_INFO(dev)->gen <= 3) {
			error->plane[i].size = I915_READ(DSPSIZE(i));
			error->plane[i].pos = I915_READ(DSPPOS(i));
		}
		if (INTEL_INFO(dev)->gen <= 7 && !IS_HASWELL(dev))
			error->plane[i].addr = I915_READ(DSPADDR(i));
>>>>>>> refs/remotes/origin/master
		if (INTEL_INFO(dev)->gen >= 4) {
			error->plane[i].surface = I915_READ(DSPSURF(i));
			error->plane[i].tile_offset = I915_READ(DSPTILEOFF(i));
		}

<<<<<<< HEAD
		error->pipe[i].conf = I915_READ(PIPECONF(i));
		error->pipe[i].source = I915_READ(PIPESRC(i));
		error->pipe[i].htotal = I915_READ(HTOTAL(i));
		error->pipe[i].hblank = I915_READ(HBLANK(i));
		error->pipe[i].hsync = I915_READ(HSYNC(i));
		error->pipe[i].vtotal = I915_READ(VTOTAL(i));
		error->pipe[i].vblank = I915_READ(VBLANK(i));
		error->pipe[i].vsync = I915_READ(VSYNC(i));
=======
		error->pipe[i].source = I915_READ(PIPESRC(i));
	}

	error->num_transcoders = INTEL_INFO(dev)->num_pipes;
	if (HAS_DDI(dev_priv->dev))
		error->num_transcoders++; /* Account for eDP. */

	for (i = 0; i < error->num_transcoders; i++) {
		enum transcoder cpu_transcoder = transcoders[i];

		if (!intel_display_power_enabled(dev,
				POWER_DOMAIN_TRANSCODER(cpu_transcoder)))
			continue;

		error->transcoder[i].cpu_transcoder = cpu_transcoder;

		error->transcoder[i].conf = I915_READ(PIPECONF(cpu_transcoder));
		error->transcoder[i].htotal = I915_READ(HTOTAL(cpu_transcoder));
		error->transcoder[i].hblank = I915_READ(HBLANK(cpu_transcoder));
		error->transcoder[i].hsync = I915_READ(HSYNC(cpu_transcoder));
		error->transcoder[i].vtotal = I915_READ(VTOTAL(cpu_transcoder));
		error->transcoder[i].vblank = I915_READ(VBLANK(cpu_transcoder));
		error->transcoder[i].vsync = I915_READ(VSYNC(cpu_transcoder));
>>>>>>> refs/remotes/origin/master
	}

	return error;
}

<<<<<<< HEAD
void
intel_display_print_error_state(struct seq_file *m,
=======
#define err_printf(e, ...) i915_error_printf(e, __VA_ARGS__)

void
intel_display_print_error_state(struct drm_i915_error_state_buf *m,
>>>>>>> refs/remotes/origin/master
				struct drm_device *dev,
				struct intel_display_error_state *error)
{
	int i;

<<<<<<< HEAD
	for (i = 0; i < 2; i++) {
		seq_printf(m, "Pipe [%d]:\n", i);
		seq_printf(m, "  CONF: %08x\n", error->pipe[i].conf);
		seq_printf(m, "  SRC: %08x\n", error->pipe[i].source);
		seq_printf(m, "  HTOTAL: %08x\n", error->pipe[i].htotal);
		seq_printf(m, "  HBLANK: %08x\n", error->pipe[i].hblank);
		seq_printf(m, "  HSYNC: %08x\n", error->pipe[i].hsync);
		seq_printf(m, "  VTOTAL: %08x\n", error->pipe[i].vtotal);
		seq_printf(m, "  VBLANK: %08x\n", error->pipe[i].vblank);
		seq_printf(m, "  VSYNC: %08x\n", error->pipe[i].vsync);

		seq_printf(m, "Plane [%d]:\n", i);
		seq_printf(m, "  CNTR: %08x\n", error->plane[i].control);
		seq_printf(m, "  STRIDE: %08x\n", error->plane[i].stride);
		seq_printf(m, "  SIZE: %08x\n", error->plane[i].size);
		seq_printf(m, "  POS: %08x\n", error->plane[i].pos);
		seq_printf(m, "  ADDR: %08x\n", error->plane[i].addr);
		if (INTEL_INFO(dev)->gen >= 4) {
			seq_printf(m, "  SURF: %08x\n", error->plane[i].surface);
			seq_printf(m, "  TILEOFF: %08x\n", error->plane[i].tile_offset);
		}

		seq_printf(m, "Cursor [%d]:\n", i);
		seq_printf(m, "  CNTR: %08x\n", error->cursor[i].control);
		seq_printf(m, "  POS: %08x\n", error->cursor[i].position);
		seq_printf(m, "  BASE: %08x\n", error->cursor[i].base);
	}
}
#endif
=======
	if (!error)
		return;

	err_printf(m, "Num Pipes: %d\n", INTEL_INFO(dev)->num_pipes);
	if (HAS_POWER_WELL(dev))
		err_printf(m, "PWR_WELL_CTL2: %08x\n",
			   error->power_well_driver);
	for_each_pipe(i) {
		err_printf(m, "Pipe [%d]:\n", i);
		err_printf(m, "  SRC: %08x\n", error->pipe[i].source);

		err_printf(m, "Plane [%d]:\n", i);
		err_printf(m, "  CNTR: %08x\n", error->plane[i].control);
		err_printf(m, "  STRIDE: %08x\n", error->plane[i].stride);
		if (INTEL_INFO(dev)->gen <= 3) {
			err_printf(m, "  SIZE: %08x\n", error->plane[i].size);
			err_printf(m, "  POS: %08x\n", error->plane[i].pos);
		}
		if (INTEL_INFO(dev)->gen <= 7 && !IS_HASWELL(dev))
			err_printf(m, "  ADDR: %08x\n", error->plane[i].addr);
		if (INTEL_INFO(dev)->gen >= 4) {
			err_printf(m, "  SURF: %08x\n", error->plane[i].surface);
			err_printf(m, "  TILEOFF: %08x\n", error->plane[i].tile_offset);
		}

		err_printf(m, "Cursor [%d]:\n", i);
		err_printf(m, "  CNTR: %08x\n", error->cursor[i].control);
		err_printf(m, "  POS: %08x\n", error->cursor[i].position);
		err_printf(m, "  BASE: %08x\n", error->cursor[i].base);
	}

	for (i = 0; i < error->num_transcoders; i++) {
		err_printf(m, "CPU transcoder: %c\n",
			   transcoder_name(error->transcoder[i].cpu_transcoder));
		err_printf(m, "  CONF: %08x\n", error->transcoder[i].conf);
		err_printf(m, "  HTOTAL: %08x\n", error->transcoder[i].htotal);
		err_printf(m, "  HBLANK: %08x\n", error->transcoder[i].hblank);
		err_printf(m, "  HSYNC: %08x\n", error->transcoder[i].hsync);
		err_printf(m, "  VTOTAL: %08x\n", error->transcoder[i].vtotal);
		err_printf(m, "  VBLANK: %08x\n", error->transcoder[i].vblank);
		err_printf(m, "  VSYNC: %08x\n", error->transcoder[i].vsync);
	}
}
>>>>>>> refs/remotes/origin/master
