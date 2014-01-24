/*
 * Copyright © 2008-2010 Intel Corporation
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *    Zou Nan hai <nanhai.zou@intel.com>
 *    Xiang Hai hao<haihao.xiang@intel.com>
 *
 */

<<<<<<< HEAD
#include "drmP.h"
#include "drm.h"
#include "i915_drv.h"
#include "i915_drm.h"
#include "i915_trace.h"
#include "intel_drv.h"

<<<<<<< HEAD
=======
/*
 * 965+ support PIPE_CONTROL commands, which provide finer grained control
 * over cache flushing.
 */
struct pipe_control {
	struct drm_i915_gem_object *obj;
	volatile u32 *cpu_page;
	u32 gtt_offset;
};

>>>>>>> refs/remotes/origin/cm-10.0
static inline int ring_space(struct intel_ring_buffer *ring)
{
	int space = (ring->head & HEAD_ADDR) - (ring->tail + 8);
=======
#include <drm/drmP.h>
#include "i915_drv.h"
#include <drm/i915_drm.h>
#include "i915_trace.h"
#include "intel_drv.h"

static inline int ring_space(struct intel_ring_buffer *ring)
{
	int space = (ring->head & HEAD_ADDR) - (ring->tail + I915_RING_FREE_SPACE);
>>>>>>> refs/remotes/origin/master
	if (space < 0)
		space += ring->size;
	return space;
}

<<<<<<< HEAD
<<<<<<< HEAD
static u32 i915_gem_get_seqno(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	u32 seqno;

	seqno = dev_priv->next_seqno;

	/* reserve 0 for non-seqno */
	if (++dev_priv->next_seqno == 0)
		dev_priv->next_seqno = 1;

	return seqno;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static int
render_ring_flush(struct intel_ring_buffer *ring,
		  u32	invalidate_domains,
		  u32	flush_domains)
=======
void __intel_ring_advance(struct intel_ring_buffer *ring)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;

	ring->tail &= ring->size - 1;
	if (dev_priv->gpu_error.stop_rings & intel_ring_flag(ring))
		return;
	ring->write_tail(ring, ring->tail);
}

static int
gen2_render_ring_flush(struct intel_ring_buffer *ring,
		       u32	invalidate_domains,
		       u32	flush_domains)
{
	u32 cmd;
	int ret;

	cmd = MI_FLUSH;
	if (((invalidate_domains|flush_domains) & I915_GEM_DOMAIN_RENDER) == 0)
		cmd |= MI_NO_WRITE_FLUSH;

	if (invalidate_domains & I915_GEM_DOMAIN_SAMPLER)
		cmd |= MI_READ_FLUSH;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring, cmd);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);

	return 0;
}

static int
gen4_render_ring_flush(struct intel_ring_buffer *ring,
		       u32	invalidate_domains,
		       u32	flush_domains)
>>>>>>> refs/remotes/origin/master
{
	struct drm_device *dev = ring->dev;
	u32 cmd;
	int ret;

	/*
	 * read/write caches:
	 *
	 * I915_GEM_DOMAIN_RENDER is always invalidated, but is
	 * only flushed if MI_NO_WRITE_FLUSH is unset.  On 965, it is
	 * also flushed at 2d versus 3d pipeline switches.
	 *
	 * read-only caches:
	 *
	 * I915_GEM_DOMAIN_SAMPLER is flushed on pre-965 if
	 * MI_READ_FLUSH is set, and is always flushed on 965.
	 *
	 * I915_GEM_DOMAIN_COMMAND may not exist?
	 *
	 * I915_GEM_DOMAIN_INSTRUCTION, which exists on 965, is
	 * invalidated when MI_EXE_FLUSH is set.
	 *
	 * I915_GEM_DOMAIN_VERTEX, which exists on 965, is
	 * invalidated with every MI_FLUSH.
	 *
	 * TLBs:
	 *
	 * On 965, TLBs associated with I915_GEM_DOMAIN_COMMAND
	 * and I915_GEM_DOMAIN_CPU in are invalidated at PTE write and
	 * I915_GEM_DOMAIN_RENDER and I915_GEM_DOMAIN_SAMPLER
	 * are flushed at any MI_FLUSH.
	 */

	cmd = MI_FLUSH | MI_NO_WRITE_FLUSH;
<<<<<<< HEAD
	if ((invalidate_domains|flush_domains) &
	    I915_GEM_DOMAIN_RENDER)
		cmd &= ~MI_NO_WRITE_FLUSH;
	if (INTEL_INFO(dev)->gen < 4) {
		/*
		 * On the 965, the sampler cache always gets flushed
		 * and this bit is reserved.
		 */
		if (invalidate_domains & I915_GEM_DOMAIN_SAMPLER)
			cmd |= MI_READ_FLUSH;
	}
=======
	if ((invalidate_domains|flush_domains) & I915_GEM_DOMAIN_RENDER)
		cmd &= ~MI_NO_WRITE_FLUSH;
>>>>>>> refs/remotes/origin/master
	if (invalidate_domains & I915_GEM_DOMAIN_INSTRUCTION)
		cmd |= MI_EXE_FLUSH;

	if (invalidate_domains & I915_GEM_DOMAIN_COMMAND &&
	    (IS_G4X(dev) || IS_GEN5(dev)))
		cmd |= MI_INVALIDATE_ISP;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring, cmd);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/**
 * Emits a PIPE_CONTROL with a non-zero post-sync operation, for
 * implementing two workarounds on gen6.  From section 1.4.7.1
 * "PIPE_CONTROL" of the Sandy Bridge PRM volume 2 part 1:
 *
 * [DevSNB-C+{W/A}] Before any depth stall flush (including those
 * produced by non-pipelined state commands), software needs to first
 * send a PIPE_CONTROL with no bits set except Post-Sync Operation !=
 * 0.
 *
 * [Dev-SNB{W/A}]: Before a PIPE_CONTROL with Write Cache Flush Enable
 * =1, a PIPE_CONTROL with any non-zero post-sync-op is required.
 *
 * And the workaround for these two requires this workaround first:
 *
 * [Dev-SNB{W/A}]: Pipe-control with CS-stall bit set must be sent
 * BEFORE the pipe-control with a post-sync op and no write-cache
 * flushes.
 *
 * And this last workaround is tricky because of the requirements on
 * that bit.  From section 1.4.7.2.3 "Stall" of the Sandy Bridge PRM
 * volume 2 part 1:
 *
 *     "1 of the following must also be set:
 *      - Render Target Cache Flush Enable ([12] of DW1)
 *      - Depth Cache Flush Enable ([0] of DW1)
 *      - Stall at Pixel Scoreboard ([1] of DW1)
 *      - Depth Stall ([13] of DW1)
 *      - Post-Sync Operation ([13] of DW1)
 *      - Notify Enable ([8] of DW1)"
 *
 * The cache flushes require the workaround flush that triggered this
 * one, so we can't use it.  Depth stall would trigger the same.
 * Post-sync nonzero is what triggered this second workaround, so we
 * can't use that one either.  Notify enable is IRQs, which aren't
 * really our business.  That leaves only stall at scoreboard.
 */
static int
intel_emit_post_sync_nonzero_flush(struct intel_ring_buffer *ring)
{
<<<<<<< HEAD
	struct pipe_control *pc = ring->private;
	u32 scratch_addr = pc->gtt_offset + 128;
=======
	u32 scratch_addr = ring->scratch.gtt_offset + 128;
>>>>>>> refs/remotes/origin/master
	int ret;


	ret = intel_ring_begin(ring, 6);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(5));
	intel_ring_emit(ring, PIPE_CONTROL_CS_STALL |
			PIPE_CONTROL_STALL_AT_SCOREBOARD);
	intel_ring_emit(ring, scratch_addr | PIPE_CONTROL_GLOBAL_GTT); /* address */
	intel_ring_emit(ring, 0); /* low dword */
	intel_ring_emit(ring, 0); /* high dword */
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);

	ret = intel_ring_begin(ring, 6);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(5));
	intel_ring_emit(ring, PIPE_CONTROL_QW_WRITE);
	intel_ring_emit(ring, scratch_addr | PIPE_CONTROL_GLOBAL_GTT); /* address */
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);

	return 0;
}

static int
gen6_render_ring_flush(struct intel_ring_buffer *ring,
                         u32 invalidate_domains, u32 flush_domains)
{
	u32 flags = 0;
<<<<<<< HEAD
	struct pipe_control *pc = ring->private;
	u32 scratch_addr = pc->gtt_offset + 128;
	int ret;

	/* Force SNB workarounds for PIPE_CONTROL flushes */
	intel_emit_post_sync_nonzero_flush(ring);
=======
	u32 scratch_addr = ring->scratch.gtt_offset + 128;
	int ret;

	/* Force SNB workarounds for PIPE_CONTROL flushes */
	ret = intel_emit_post_sync_nonzero_flush(ring);
	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/master

	/* Just flush everything.  Experiments have shown that reducing the
	 * number of bits based on the write domains has little performance
	 * impact.
	 */
<<<<<<< HEAD
	flags |= PIPE_CONTROL_RENDER_TARGET_CACHE_FLUSH;
	flags |= PIPE_CONTROL_INSTRUCTION_CACHE_INVALIDATE;
	flags |= PIPE_CONTROL_TEXTURE_CACHE_INVALIDATE;
	flags |= PIPE_CONTROL_DEPTH_CACHE_FLUSH;
	flags |= PIPE_CONTROL_VF_CACHE_INVALIDATE;
	flags |= PIPE_CONTROL_CONST_CACHE_INVALIDATE;
	flags |= PIPE_CONTROL_STATE_CACHE_INVALIDATE;

	ret = intel_ring_begin(ring, 6);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(5));
	intel_ring_emit(ring, flags);
	intel_ring_emit(ring, scratch_addr | PIPE_CONTROL_GLOBAL_GTT);
	intel_ring_emit(ring, 0); /* lower dword */
	intel_ring_emit(ring, 0); /* uppwer dword */
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);

	return 0;
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (flush_domains) {
		flags |= PIPE_CONTROL_RENDER_TARGET_CACHE_FLUSH;
		flags |= PIPE_CONTROL_DEPTH_CACHE_FLUSH;
		/*
		 * Ensure that any following seqno writes only happen
		 * when the render cache is indeed flushed.
		 */
		flags |= PIPE_CONTROL_CS_STALL;
	}
	if (invalidate_domains) {
		flags |= PIPE_CONTROL_TLB_INVALIDATE;
		flags |= PIPE_CONTROL_INSTRUCTION_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_TEXTURE_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_VF_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_CONST_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_STATE_CACHE_INVALIDATE;
		/*
		 * TLB invalidate requires a post-sync write.
		 */
		flags |= PIPE_CONTROL_QW_WRITE | PIPE_CONTROL_CS_STALL;
	}

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(4));
	intel_ring_emit(ring, flags);
	intel_ring_emit(ring, scratch_addr | PIPE_CONTROL_GLOBAL_GTT);
	intel_ring_emit(ring, 0);
	intel_ring_advance(ring);

	return 0;
}

static int
gen7_render_ring_cs_stall_wa(struct intel_ring_buffer *ring)
{
	int ret;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(4));
	intel_ring_emit(ring, PIPE_CONTROL_CS_STALL |
			      PIPE_CONTROL_STALL_AT_SCOREBOARD);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, 0);
	intel_ring_advance(ring);

	return 0;
}

static int gen7_ring_fbc_flush(struct intel_ring_buffer *ring, u32 value)
{
	int ret;

	if (!ring->fbc_dirty)
		return 0;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;
	intel_ring_emit(ring, MI_NOOP);
	/* WaFbcNukeOn3DBlt:ivb/hsw */
	intel_ring_emit(ring, MI_LOAD_REGISTER_IMM(1));
	intel_ring_emit(ring, MSG_FBC_REND_STATE);
	intel_ring_emit(ring, value);
	intel_ring_advance(ring);

	ring->fbc_dirty = false;
	return 0;
}

static int
gen7_render_ring_flush(struct intel_ring_buffer *ring,
		       u32 invalidate_domains, u32 flush_domains)
{
	u32 flags = 0;
	u32 scratch_addr = ring->scratch.gtt_offset + 128;
	int ret;

	/*
	 * Ensure that any following seqno writes only happen when the render
	 * cache is indeed flushed.
	 *
	 * Workaround: 4th PIPE_CONTROL command (except the ones with only
	 * read-cache invalidate bits set) must have the CS_STALL bit set. We
	 * don't try to be clever and just set it unconditionally.
	 */
	flags |= PIPE_CONTROL_CS_STALL;

	/* Just flush everything.  Experiments have shown that reducing the
	 * number of bits based on the write domains has little performance
	 * impact.
	 */
	if (flush_domains) {
		flags |= PIPE_CONTROL_RENDER_TARGET_CACHE_FLUSH;
		flags |= PIPE_CONTROL_DEPTH_CACHE_FLUSH;
	}
	if (invalidate_domains) {
		flags |= PIPE_CONTROL_TLB_INVALIDATE;
		flags |= PIPE_CONTROL_INSTRUCTION_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_TEXTURE_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_VF_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_CONST_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_STATE_CACHE_INVALIDATE;
		/*
		 * TLB invalidate requires a post-sync write.
		 */
		flags |= PIPE_CONTROL_QW_WRITE;
		flags |= PIPE_CONTROL_GLOBAL_GTT_IVB;

		/* Workaround: we must issue a pipe_control with CS-stall bit
		 * set before a pipe_control command that has the state cache
		 * invalidate bit set. */
		gen7_render_ring_cs_stall_wa(ring);
	}

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(4));
	intel_ring_emit(ring, flags);
	intel_ring_emit(ring, scratch_addr);
	intel_ring_emit(ring, 0);
	intel_ring_advance(ring);

	if (flush_domains)
		return gen7_ring_fbc_flush(ring, FBC_REND_NUKE);

	return 0;
}

static int
gen8_render_ring_flush(struct intel_ring_buffer *ring,
		       u32 invalidate_domains, u32 flush_domains)
{
	u32 flags = 0;
	u32 scratch_addr = ring->scratch.gtt_offset + 128;
	int ret;

	flags |= PIPE_CONTROL_CS_STALL;

	if (flush_domains) {
		flags |= PIPE_CONTROL_RENDER_TARGET_CACHE_FLUSH;
		flags |= PIPE_CONTROL_DEPTH_CACHE_FLUSH;
	}
	if (invalidate_domains) {
		flags |= PIPE_CONTROL_TLB_INVALIDATE;
		flags |= PIPE_CONTROL_INSTRUCTION_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_TEXTURE_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_VF_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_CONST_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_STATE_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_QW_WRITE;
		flags |= PIPE_CONTROL_GLOBAL_GTT_IVB;
	}

	ret = intel_ring_begin(ring, 6);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(6));
	intel_ring_emit(ring, flags);
	intel_ring_emit(ring, scratch_addr);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, 0);
	intel_ring_advance(ring);

	return 0;

}

>>>>>>> refs/remotes/origin/master
static void ring_write_tail(struct intel_ring_buffer *ring,
			    u32 value)
{
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
	I915_WRITE_TAIL(ring, value);
}

u32 intel_ring_get_active_head(struct intel_ring_buffer *ring)
{
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
	u32 acthd_reg = INTEL_INFO(ring->dev)->gen >= 4 ?
			RING_ACTHD(ring->mmio_base) : ACTHD;

	return I915_READ(acthd_reg);
}

<<<<<<< HEAD
static int init_ring_common(struct intel_ring_buffer *ring)
{
<<<<<<< HEAD
<<<<<<< HEAD
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
=======
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
>>>>>>> refs/remotes/origin/cm-11.0
	struct drm_i915_gem_object *obj = ring->obj;
	int ret = 0;
	u32 head;

<<<<<<< HEAD
=======
=======
static void ring_setup_phys_status_page(struct intel_ring_buffer *ring)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;
	u32 addr;

	addr = dev_priv->status_page_dmah->busaddr;
	if (INTEL_INFO(ring->dev)->gen >= 4)
		addr |= (dev_priv->status_page_dmah->busaddr >> 28) & 0xf0;
	I915_WRITE(HWS_PGA, addr);
}

static int init_ring_common(struct intel_ring_buffer *ring)
{
>>>>>>> refs/remotes/origin/master
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_i915_gem_object *obj = ring->obj;
	int ret = 0;
	u32 head;

<<<<<<< HEAD
	if (HAS_FORCE_WAKE(dev))
		gen6_gt_force_wake_get(dev_priv);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	gen6_gt_force_wake_get(dev_priv);

	if (I915_NEED_GFX_HWS(dev))
		intel_ring_setup_status_page(ring);
	else
		ring_setup_phys_status_page(ring);

>>>>>>> refs/remotes/origin/master
=======
	if (HAS_FORCE_WAKE(dev))
		gen6_gt_force_wake_get(dev_priv);

>>>>>>> refs/remotes/origin/cm-11.0
	/* Stop the ring if it's running. */
	I915_WRITE_CTL(ring, 0);
	I915_WRITE_HEAD(ring, 0);
	ring->write_tail(ring, 0);

<<<<<<< HEAD
	/* Initialize the ring. */
	I915_WRITE_START(ring, obj->gtt_offset);
=======
>>>>>>> refs/remotes/origin/master
	head = I915_READ_HEAD(ring) & HEAD_ADDR;

	/* G45 ring initialization fails to reset head to zero */
	if (head != 0) {
		DRM_DEBUG_KMS("%s head not reset to zero "
			      "ctl %08x head %08x tail %08x start %08x\n",
			      ring->name,
			      I915_READ_CTL(ring),
			      I915_READ_HEAD(ring),
			      I915_READ_TAIL(ring),
			      I915_READ_START(ring));

		I915_WRITE_HEAD(ring, 0);

		if (I915_READ_HEAD(ring) & HEAD_ADDR) {
			DRM_ERROR("failed to set %s head to zero "
				  "ctl %08x head %08x tail %08x start %08x\n",
				  ring->name,
				  I915_READ_CTL(ring),
				  I915_READ_HEAD(ring),
				  I915_READ_TAIL(ring),
				  I915_READ_START(ring));
		}
	}

<<<<<<< HEAD
	I915_WRITE_CTL(ring,
			((ring->size - PAGE_SIZE) & RING_NR_PAGES)
<<<<<<< HEAD
			| RING_REPORT_64K | RING_VALID);

	/* If the head is still not zero, the ring is dead */
<<<<<<< HEAD
	if ((I915_READ_CTL(ring) & RING_VALID) == 0 ||
	    I915_READ_START(ring) != obj->gtt_offset ||
	    (I915_READ_HEAD(ring) & HEAD_ADDR) != 0) {
=======
=======
	/* Initialize the ring. This must happen _after_ we've cleared the ring
	 * registers with the above sequence (the readback of the HEAD registers
	 * also enforces ordering), otherwise the hw might lose the new ring
	 * register values. */
	I915_WRITE_START(ring, i915_gem_obj_ggtt_offset(obj));
	I915_WRITE_CTL(ring,
			((ring->size - PAGE_SIZE) & RING_NR_PAGES)
>>>>>>> refs/remotes/origin/master
			| RING_VALID);

	/* If the head is still not zero, the ring is dead */
	if (wait_for((I915_READ_CTL(ring) & RING_VALID) != 0 &&
<<<<<<< HEAD
		     I915_READ_START(ring) == obj->gtt_offset &&
		     (I915_READ_HEAD(ring) & HEAD_ADDR) == 0, 50)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		     I915_READ_START(ring) == i915_gem_obj_ggtt_offset(obj) &&
		     (I915_READ_HEAD(ring) & HEAD_ADDR) == 0, 50)) {
>>>>>>> refs/remotes/origin/master
=======
	if (wait_for((I915_READ_CTL(ring) & RING_VALID) != 0 &&
		     I915_READ_START(ring) == obj->gtt_offset &&
		     (I915_READ_HEAD(ring) & HEAD_ADDR) == 0, 50)) {
>>>>>>> refs/remotes/origin/cm-11.0
		DRM_ERROR("%s initialization failed "
				"ctl %08x head %08x tail %08x start %08x\n",
				ring->name,
				I915_READ_CTL(ring),
				I915_READ_HEAD(ring),
				I915_READ_TAIL(ring),
				I915_READ_START(ring));
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		return -EIO;
=======
		ret = -EIO;
		goto out;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ret = -EIO;
		goto out;
>>>>>>> refs/remotes/origin/master
=======
		ret = -EIO;
		goto out;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	if (!drm_core_check_feature(ring->dev, DRIVER_MODESET))
		i915_kernel_lost_context(ring->dev);
	else {
		ring->head = I915_READ_HEAD(ring);
		ring->tail = I915_READ_TAIL(ring) & TAIL_ADDR;
		ring->space = ring_space(ring);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
		ring->last_retired_head = -1;
>>>>>>> refs/remotes/origin/cm-11.0
	}

out:
	if (HAS_FORCE_WAKE(dev))
		gen6_gt_force_wake_put(dev_priv);

	return ret;
}

/*
 * 965+ support PIPE_CONTROL commands, which provide finer grained control
 * over cache flushing.
 */
struct pipe_control {
	struct drm_i915_gem_object *obj;
	volatile u32 *cpu_page;
	u32 gtt_offset;
};
=======
		ring->last_retired_head = -1;
	}

out:
	if (HAS_FORCE_WAKE(dev))
		gen6_gt_force_wake_put(dev_priv);

	return ret;
}
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ring->last_retired_head = -1;
	}

	memset(&ring->hangcheck, 0, sizeof(ring->hangcheck));

out:
	gen6_gt_force_wake_put(dev_priv);

	return ret;
}
>>>>>>> refs/remotes/origin/master

static int
init_pipe_control(struct intel_ring_buffer *ring)
{
<<<<<<< HEAD
	struct pipe_control *pc;
	struct drm_i915_gem_object *obj;
	int ret;

	if (ring->private)
		return 0;

	pc = kmalloc(sizeof(*pc), GFP_KERNEL);
	if (!pc)
		return -ENOMEM;

	obj = i915_gem_alloc_object(ring->dev, 4096);
	if (obj == NULL) {
=======
	int ret;

	if (ring->scratch.obj)
		return 0;

	ring->scratch.obj = i915_gem_alloc_object(ring->dev, 4096);
	if (ring->scratch.obj == NULL) {
>>>>>>> refs/remotes/origin/master
		DRM_ERROR("Failed to allocate seqno page\n");
		ret = -ENOMEM;
		goto err;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	obj->cache_level = I915_CACHE_LLC;
=======

	i915_gem_object_set_cache_level(obj, I915_CACHE_LLC);
>>>>>>> refs/remotes/origin/cm-10.0

	ret = i915_gem_object_pin(obj, 4096, true);
	if (ret)
		goto err_unref;

	pc->gtt_offset = obj->gtt_offset;
	pc->cpu_page =  kmap(obj->pages[0]);
	if (pc->cpu_page == NULL)
		goto err_unpin;

	pc->obj = obj;
	ring->private = pc;
	return 0;

err_unpin:
	i915_gem_object_unpin(obj);
err_unref:
	drm_gem_object_unreference(&obj->base);
err:
	kfree(pc);
	return ret;
}

static void
cleanup_pipe_control(struct intel_ring_buffer *ring)
{
	struct pipe_control *pc = ring->private;
	struct drm_i915_gem_object *obj;

	if (!ring->private)
		return;

	obj = pc->obj;
	kunmap(obj->pages[0]);
	i915_gem_object_unpin(obj);
	drm_gem_object_unreference(&obj->base);

	kfree(pc);
	ring->private = NULL;
}

=======

	i915_gem_object_set_cache_level(ring->scratch.obj, I915_CACHE_LLC);

	ret = i915_gem_obj_ggtt_pin(ring->scratch.obj, 4096, true, false);
	if (ret)
		goto err_unref;

	ring->scratch.gtt_offset = i915_gem_obj_ggtt_offset(ring->scratch.obj);
	ring->scratch.cpu_page = kmap(sg_page(ring->scratch.obj->pages->sgl));
	if (ring->scratch.cpu_page == NULL) {
		ret = -ENOMEM;
		goto err_unpin;
	}

	DRM_DEBUG_DRIVER("%s pipe control offset: 0x%08x\n",
			 ring->name, ring->scratch.gtt_offset);
	return 0;

err_unpin:
	i915_gem_object_unpin(ring->scratch.obj);
err_unref:
	drm_gem_object_unreference(&ring->scratch.obj->base);
err:
	return ret;
}

>>>>>>> refs/remotes/origin/master
static int init_render_ring(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret = init_ring_common(ring);

<<<<<<< HEAD
	if (INTEL_INFO(dev)->gen > 3) {
		int mode = VS_TIMER_DISPATCH << 16 | VS_TIMER_DISPATCH;
<<<<<<< HEAD
		if (IS_GEN6(dev) || IS_GEN7(dev))
			mode |= MI_FLUSH_ENABLE << 16 | MI_FLUSH_ENABLE;
		I915_WRITE(MI_MODE, mode);
	}

	if (INTEL_INFO(dev)->gen >= 6) {
	} else if (IS_GEN5(dev)) {
=======
		I915_WRITE(MI_MODE, mode);
		if (IS_GEN7(dev))
			I915_WRITE(GFX_MODE_GEN7,
				   GFX_MODE_DISABLE(GFX_TLB_INVALIDATE_ALWAYS) |
				   GFX_MODE_ENABLE(GFX_REPLAY_MODE));
	}

	if (INTEL_INFO(dev)->gen >= 5) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (INTEL_INFO(dev)->gen > 3)
		I915_WRITE(MI_MODE, _MASKED_BIT_ENABLE(VS_TIMER_DISPATCH));

	/* We need to disable the AsyncFlip performance optimisations in order
	 * to use MI_WAIT_FOR_EVENT within the CS. It should already be
	 * programmed to '1' on all products.
	 *
	 * WaDisableAsyncFlipPerfMode:snb,ivb,hsw,vlv
	 */
	if (INTEL_INFO(dev)->gen >= 6)
		I915_WRITE(MI_MODE, _MASKED_BIT_ENABLE(ASYNC_FLIP_PERF_DISABLE));

	/* Required for the hardware to program scanline values for waiting */
	if (INTEL_INFO(dev)->gen == 6)
		I915_WRITE(GFX_MODE,
			   _MASKED_BIT_ENABLE(GFX_TLB_INVALIDATE_ALWAYS));

	if (IS_GEN7(dev))
		I915_WRITE(GFX_MODE_GEN7,
			   _MASKED_BIT_DISABLE(GFX_TLB_INVALIDATE_ALWAYS) |
			   _MASKED_BIT_ENABLE(GFX_REPLAY_MODE));

	if (INTEL_INFO(dev)->gen >= 5) {
>>>>>>> refs/remotes/origin/master
		ret = init_pipe_control(ring);
		if (ret)
			return ret;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======

=======
>>>>>>> refs/remotes/origin/master
	if (IS_GEN6(dev)) {
		/* From the Sandybridge PRM, volume 1 part 3, page 24:
		 * "If this bit is set, STCunit will have LRA as replacement
		 *  policy. [...] This bit must be reset.  LRA replacement
		 *  policy is not supported."
		 */
		I915_WRITE(CACHE_MODE_0,
<<<<<<< HEAD
			   CM0_STC_EVICT_DISABLE_LRA_SNB << CM0_MASK_SHIFT);
	}

	if (INTEL_INFO(dev)->gen >= 6) {
		I915_WRITE(INSTPM,
			   INSTPM_FORCE_ORDERING << 16 | INSTPM_FORCE_ORDERING);
	}

>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static void render_ring_cleanup(struct intel_ring_buffer *ring)
{
	if (!ring->private)
		return;

	cleanup_pipe_control(ring);
}

static void
<<<<<<< HEAD
update_semaphore(struct intel_ring_buffer *ring, int i, u32 seqno)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int id;

	/*
	 * cs -> 1 = vcs, 0 = bcs
	 * vcs -> 1 = bcs, 0 = cs,
	 * bcs -> 1 = cs, 0 = vcs.
	 */
	id = ring - dev_priv->ring;
	id += 2 - i;
	id %= 3;

	intel_ring_emit(ring,
			MI_SEMAPHORE_MBOX |
			MI_SEMAPHORE_REGISTER |
			MI_SEMAPHORE_UPDATE);
	intel_ring_emit(ring, seqno);
	intel_ring_emit(ring,
			RING_SYNC_0(dev_priv->ring[id].mmio_base) + 4*i);
}

static int
gen6_add_request(struct intel_ring_buffer *ring,
		 u32 *result)
{
	u32 seqno;
=======
update_mboxes(struct intel_ring_buffer *ring,
	    u32 seqno,
	    u32 mmio_offset)
{
	intel_ring_emit(ring, MI_SEMAPHORE_MBOX |
			      MI_SEMAPHORE_GLOBAL_GTT |
			      MI_SEMAPHORE_REGISTER |
			      MI_SEMAPHORE_UPDATE);
	intel_ring_emit(ring, seqno);
	intel_ring_emit(ring, mmio_offset);
=======
			   _MASKED_BIT_DISABLE(CM0_STC_EVICT_DISABLE_LRA_SNB));

		/* This is not explicitly set for GEN6, so read the register.
		 * see intel_ring_mi_set_context() for why we care.
		 * TODO: consider explicitly setting the bit for GEN5
		 */
		ring->itlb_before_ctx_switch =
			!!(I915_READ(GFX_MODE) & GFX_TLB_INVALIDATE_ALWAYS);
	}

	if (INTEL_INFO(dev)->gen >= 6)
		I915_WRITE(INSTPM, _MASKED_BIT_ENABLE(INSTPM_FORCE_ORDERING));

	if (HAS_L3_DPF(dev))
		I915_WRITE_IMR(ring, ~GT_PARITY_ERROR(dev));

	return ret;
}

static void render_ring_cleanup(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;

	if (ring->scratch.obj == NULL)
		return;

	if (INTEL_INFO(dev)->gen >= 5) {
		kunmap(sg_page(ring->scratch.obj->pages->sgl));
		i915_gem_object_unpin(ring->scratch.obj);
	}

	drm_gem_object_unreference(&ring->scratch.obj->base);
	ring->scratch.obj = NULL;
}

static void
update_mboxes(struct intel_ring_buffer *ring,
	      u32 mmio_offset)
{
/* NB: In order to be able to do semaphore MBOX updates for varying number
 * of rings, it's easiest if we round up each individual update to a
 * multiple of 2 (since ring updates must always be a multiple of 2)
 * even though the actual update only requires 3 dwords.
 */
#define MBOX_UPDATE_DWORDS 4
	intel_ring_emit(ring, MI_LOAD_REGISTER_IMM(1));
	intel_ring_emit(ring, mmio_offset);
	intel_ring_emit(ring, ring->outstanding_lazy_seqno);
	intel_ring_emit(ring, MI_NOOP);
>>>>>>> refs/remotes/origin/master
}

/**
 * gen6_add_request - Update the semaphore mailbox registers
 * 
 * @ring - ring that is adding a request
 * @seqno - return seqno stuck into the ring
 *
 * Update the mailbox registers in the *other* rings with the current seqno.
 * This acts like a signal in the canonical semaphore.
 */
static int
<<<<<<< HEAD
gen6_add_request(struct intel_ring_buffer *ring,
		 u32 *seqno)
{
	u32 mbox1_reg;
	u32 mbox2_reg;
>>>>>>> refs/remotes/origin/cm-10.0
	int ret;

	ret = intel_ring_begin(ring, 10);
	if (ret)
		return ret;

<<<<<<< HEAD
	seqno = i915_gem_get_seqno(ring->dev);
	update_semaphore(ring, 0, seqno);
	update_semaphore(ring, 1, seqno);

	intel_ring_emit(ring, MI_STORE_DWORD_INDEX);
	intel_ring_emit(ring, I915_GEM_HWS_INDEX << MI_STORE_DWORD_INDEX_SHIFT);
	intel_ring_emit(ring, seqno);
	intel_ring_emit(ring, MI_USER_INTERRUPT);
	intel_ring_advance(ring);

	*result = seqno;
	return 0;
}

int
intel_ring_sync(struct intel_ring_buffer *ring,
		struct intel_ring_buffer *to,
		u32 seqno)
{
	int ret;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	intel_ring_emit(ring,
			MI_SEMAPHORE_MBOX |
			MI_SEMAPHORE_REGISTER |
			intel_ring_sync_index(ring, to) << 17 |
			MI_SEMAPHORE_COMPARE);
	intel_ring_emit(ring, seqno);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);
=======
	mbox1_reg = ring->signal_mbox[0];
	mbox2_reg = ring->signal_mbox[1];

	*seqno = i915_gem_next_request_seqno(ring);

	update_mboxes(ring, *seqno, mbox1_reg);
	update_mboxes(ring, *seqno, mbox2_reg);
	intel_ring_emit(ring, MI_STORE_DWORD_INDEX);
	intel_ring_emit(ring, I915_GEM_HWS_INDEX << MI_STORE_DWORD_INDEX_SHIFT);
	intel_ring_emit(ring, *seqno);
	intel_ring_emit(ring, MI_USER_INTERRUPT);
	intel_ring_advance(ring);

	return 0;
=======
gen6_add_request(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_ring_buffer *useless;
	int i, ret;

	ret = intel_ring_begin(ring, ((I915_NUM_RINGS-1) *
				      MBOX_UPDATE_DWORDS) +
				      4);
	if (ret)
		return ret;
#undef MBOX_UPDATE_DWORDS

	for_each_ring(useless, dev_priv, i) {
		u32 mbox_reg = ring->signal_mbox[i];
		if (mbox_reg != GEN6_NOSYNC)
			update_mboxes(ring, mbox_reg);
	}

	intel_ring_emit(ring, MI_STORE_DWORD_INDEX);
	intel_ring_emit(ring, I915_GEM_HWS_INDEX << MI_STORE_DWORD_INDEX_SHIFT);
	intel_ring_emit(ring, ring->outstanding_lazy_seqno);
	intel_ring_emit(ring, MI_USER_INTERRUPT);
	__intel_ring_advance(ring);

	return 0;
}

static inline bool i915_gem_has_seqno_wrapped(struct drm_device *dev,
					      u32 seqno)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	return dev_priv->last_seqno < seqno;
>>>>>>> refs/remotes/origin/master
}

/**
 * intel_ring_sync - sync the waiter to the signaller on seqno
 *
 * @waiter - ring that is waiting
 * @signaller - ring which has, or will signal
 * @seqno - seqno which the waiter will block on
 */
static int
<<<<<<< HEAD
intel_ring_sync(struct intel_ring_buffer *waiter,
		struct intel_ring_buffer *signaller,
		int ring,
		u32 seqno)
=======
gen6_ring_sync(struct intel_ring_buffer *waiter,
	       struct intel_ring_buffer *signaller,
	       u32 seqno)
>>>>>>> refs/remotes/origin/master
{
	int ret;
	u32 dw1 = MI_SEMAPHORE_MBOX |
		  MI_SEMAPHORE_COMPARE |
		  MI_SEMAPHORE_REGISTER;

<<<<<<< HEAD
=======
	/* Throughout all of the GEM code, seqno passed implies our current
	 * seqno is >= the last seqno executed. However for hardware the
	 * comparison is strictly greater than.
	 */
	seqno -= 1;

	WARN_ON(signaller->semaphore_register[waiter->id] ==
		MI_SEMAPHORE_SYNC_INVALID);

>>>>>>> refs/remotes/origin/master
	ret = intel_ring_begin(waiter, 4);
	if (ret)
		return ret;

<<<<<<< HEAD
	intel_ring_emit(waiter, dw1 | signaller->semaphore_register[ring]);
	intel_ring_emit(waiter, seqno);
	intel_ring_emit(waiter, 0);
	intel_ring_emit(waiter, MI_NOOP);
	intel_ring_advance(waiter);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* If seqno wrap happened, omit the wait with no-ops */
	if (likely(!i915_gem_has_seqno_wrapped(waiter->dev, seqno))) {
		intel_ring_emit(waiter,
				dw1 |
				signaller->semaphore_register[waiter->id]);
		intel_ring_emit(waiter, seqno);
		intel_ring_emit(waiter, 0);
		intel_ring_emit(waiter, MI_NOOP);
	} else {
		intel_ring_emit(waiter, MI_NOOP);
		intel_ring_emit(waiter, MI_NOOP);
		intel_ring_emit(waiter, MI_NOOP);
		intel_ring_emit(waiter, MI_NOOP);
	}
	intel_ring_advance(waiter);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
#define PIPE_CONTROL_FLUSH(ring__, addr__)					\
do {									\
	intel_ring_emit(ring__, GFX_OP_PIPE_CONTROL | PIPE_CONTROL_QW_WRITE |		\
		 PIPE_CONTROL_DEPTH_STALL | 2);				\
=======
/* VCS->RCS (RVSYNC) or BCS->RCS (RBSYNC) */
int
render_ring_sync_to(struct intel_ring_buffer *waiter,
		    struct intel_ring_buffer *signaller,
		    u32 seqno)
{
	WARN_ON(signaller->semaphore_register[RCS] == MI_SEMAPHORE_SYNC_INVALID);
	return intel_ring_sync(waiter,
			       signaller,
			       RCS,
			       seqno);
}

/* RCS->VCS (VRSYNC) or BCS->VCS (VBSYNC) */
int
gen6_bsd_ring_sync_to(struct intel_ring_buffer *waiter,
		      struct intel_ring_buffer *signaller,
		      u32 seqno)
{
	WARN_ON(signaller->semaphore_register[VCS] == MI_SEMAPHORE_SYNC_INVALID);
	return intel_ring_sync(waiter,
			       signaller,
			       VCS,
			       seqno);
}

/* RCS->BCS (BRSYNC) or VCS->BCS (BVSYNC) */
int
gen6_blt_ring_sync_to(struct intel_ring_buffer *waiter,
		      struct intel_ring_buffer *signaller,
		      u32 seqno)
{
	WARN_ON(signaller->semaphore_register[BCS] == MI_SEMAPHORE_SYNC_INVALID);
	return intel_ring_sync(waiter,
			       signaller,
			       BCS,
			       seqno);
}



=======
>>>>>>> refs/remotes/origin/master
#define PIPE_CONTROL_FLUSH(ring__, addr__)					\
do {									\
	intel_ring_emit(ring__, GFX_OP_PIPE_CONTROL(4) | PIPE_CONTROL_QW_WRITE |		\
		 PIPE_CONTROL_DEPTH_STALL);				\
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	intel_ring_emit(ring__, (addr__) | PIPE_CONTROL_GLOBAL_GTT);			\
	intel_ring_emit(ring__, 0);							\
	intel_ring_emit(ring__, 0);							\
} while (0)

static int
<<<<<<< HEAD
pc_render_add_request(struct intel_ring_buffer *ring,
		      u32 *result)
{
<<<<<<< HEAD
	struct drm_device *dev = ring->dev;
	u32 seqno = i915_gem_get_seqno(dev);
=======
	u32 seqno = i915_gem_next_request_seqno(ring);
>>>>>>> refs/remotes/origin/cm-10.0
	struct pipe_control *pc = ring->private;
	u32 scratch_addr = pc->gtt_offset + 128;
=======
pc_render_add_request(struct intel_ring_buffer *ring)
{
	u32 scratch_addr = ring->scratch.gtt_offset + 128;
>>>>>>> refs/remotes/origin/master
	int ret;

	/* For Ironlake, MI_USER_INTERRUPT was deprecated and apparently
	 * incoherent with writes to memory, i.e. completely fubar,
	 * so we need to use PIPE_NOTIFY instead.
	 *
	 * However, we also need to workaround the qword write
	 * incoherence by flushing the 6 PIPE_NOTIFY buffers out to
	 * memory before requesting an interrupt.
	 */
	ret = intel_ring_begin(ring, 32);
	if (ret)
		return ret;

<<<<<<< HEAD
<<<<<<< HEAD
	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL | PIPE_CONTROL_QW_WRITE |
			PIPE_CONTROL_WC_FLUSH | PIPE_CONTROL_TC_FLUSH);
=======
	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(4) | PIPE_CONTROL_QW_WRITE |
			PIPE_CONTROL_WRITE_FLUSH |
			PIPE_CONTROL_TEXTURE_CACHE_INVALIDATE);
>>>>>>> refs/remotes/origin/cm-10.0
	intel_ring_emit(ring, pc->gtt_offset | PIPE_CONTROL_GLOBAL_GTT);
	intel_ring_emit(ring, seqno);
=======
	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(4) | PIPE_CONTROL_QW_WRITE |
			PIPE_CONTROL_WRITE_FLUSH |
			PIPE_CONTROL_TEXTURE_CACHE_INVALIDATE);
	intel_ring_emit(ring, ring->scratch.gtt_offset | PIPE_CONTROL_GLOBAL_GTT);
	intel_ring_emit(ring, ring->outstanding_lazy_seqno);
>>>>>>> refs/remotes/origin/master
	intel_ring_emit(ring, 0);
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 128; /* write to separate cachelines */
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 128;
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 128;
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 128;
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 128;
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
<<<<<<< HEAD
<<<<<<< HEAD
	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL | PIPE_CONTROL_QW_WRITE |
			PIPE_CONTROL_WC_FLUSH | PIPE_CONTROL_TC_FLUSH |
=======
=======
>>>>>>> refs/remotes/origin/master

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(4) | PIPE_CONTROL_QW_WRITE |
			PIPE_CONTROL_WRITE_FLUSH |
			PIPE_CONTROL_TEXTURE_CACHE_INVALIDATE |
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			PIPE_CONTROL_NOTIFY);
	intel_ring_emit(ring, pc->gtt_offset | PIPE_CONTROL_GLOBAL_GTT);
	intel_ring_emit(ring, seqno);
	intel_ring_emit(ring, 0);
	intel_ring_advance(ring);

	*result = seqno;
	return 0;
}

static int
render_ring_add_request(struct intel_ring_buffer *ring,
			u32 *result)
{
<<<<<<< HEAD
	struct drm_device *dev = ring->dev;
	u32 seqno = i915_gem_get_seqno(dev);
=======
	u32 seqno = i915_gem_next_request_seqno(ring);
>>>>>>> refs/remotes/origin/cm-10.0
	int ret;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	intel_ring_emit(ring, MI_STORE_DWORD_INDEX);
	intel_ring_emit(ring, I915_GEM_HWS_INDEX << MI_STORE_DWORD_INDEX_SHIFT);
	intel_ring_emit(ring, seqno);
	intel_ring_emit(ring, MI_USER_INTERRUPT);
	intel_ring_advance(ring);

	*result = seqno;
	return 0;
}

static u32
<<<<<<< HEAD
=======
gen6_ring_get_seqno(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;

	/* Workaround to force correct ordering between irq and seqno writes on
	 * ivb (and maybe also on snb) by reading from a CS register (like
	 * ACTHD) before reading the status page. */
	if (IS_GEN6(dev) || IS_GEN7(dev))
=======
			PIPE_CONTROL_NOTIFY);
	intel_ring_emit(ring, ring->scratch.gtt_offset | PIPE_CONTROL_GLOBAL_GTT);
	intel_ring_emit(ring, ring->outstanding_lazy_seqno);
	intel_ring_emit(ring, 0);
	__intel_ring_advance(ring);

	return 0;
}

static u32
gen6_ring_get_seqno(struct intel_ring_buffer *ring, bool lazy_coherency)
{
	/* Workaround to force correct ordering between irq and seqno writes on
	 * ivb (and maybe also on snb) by reading from a CS register (like
	 * ACTHD) before reading the status page. */
	if (!lazy_coherency)
>>>>>>> refs/remotes/origin/master
		intel_ring_get_active_head(ring);
	return intel_read_status_page(ring, I915_GEM_HWS_INDEX);
}

static u32
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
ring_get_seqno(struct intel_ring_buffer *ring)
=======
ring_get_seqno(struct intel_ring_buffer *ring, bool lazy_coherency)
>>>>>>> refs/remotes/origin/master
{
	return intel_read_status_page(ring, I915_GEM_HWS_INDEX);
}

<<<<<<< HEAD
static u32
pc_render_get_seqno(struct intel_ring_buffer *ring)
{
	struct pipe_control *pc = ring->private;
	return pc->cpu_page[0];
}

static void
ironlake_enable_irq(drm_i915_private_t *dev_priv, u32 mask)
{
	dev_priv->gt_irq_mask &= ~mask;
	I915_WRITE(GTIMR, dev_priv->gt_irq_mask);
	POSTING_READ(GTIMR);
}

static void
ironlake_disable_irq(drm_i915_private_t *dev_priv, u32 mask)
{
	dev_priv->gt_irq_mask |= mask;
	I915_WRITE(GTIMR, dev_priv->gt_irq_mask);
	POSTING_READ(GTIMR);
}

static void
i915_enable_irq(drm_i915_private_t *dev_priv, u32 mask)
{
	dev_priv->irq_mask &= ~mask;
	I915_WRITE(IMR, dev_priv->irq_mask);
	POSTING_READ(IMR);
}

static void
i915_disable_irq(drm_i915_private_t *dev_priv, u32 mask)
{
	dev_priv->irq_mask |= mask;
	I915_WRITE(IMR, dev_priv->irq_mask);
	POSTING_READ(IMR);
}

static bool
render_ring_get_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
=======
static void
ring_set_seqno(struct intel_ring_buffer *ring, u32 seqno)
{
	intel_write_status_page(ring, I915_GEM_HWS_INDEX, seqno);
}

static u32
pc_render_get_seqno(struct intel_ring_buffer *ring, bool lazy_coherency)
{
	return ring->scratch.cpu_page[0];
}

static void
pc_render_set_seqno(struct intel_ring_buffer *ring, u32 seqno)
{
	ring->scratch.cpu_page[0] = seqno;
}

static bool
gen5_ring_get_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	unsigned long flags;

	if (!dev->irq_enabled)
		return false;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (ring->irq_refcount++ == 0)
		ilk_enable_gt_irq(dev_priv, ring->irq_enable_mask);
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);

	return true;
}

static void
gen5_ring_put_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	unsigned long flags;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (--ring->irq_refcount == 0)
		ilk_disable_gt_irq(dev_priv, ring->irq_enable_mask);
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);
}

static bool
i9xx_ring_get_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	unsigned long flags;

	if (!dev->irq_enabled)
		return false;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (ring->irq_refcount++ == 0) {
		dev_priv->irq_mask &= ~ring->irq_enable_mask;
		I915_WRITE(IMR, dev_priv->irq_mask);
		POSTING_READ(IMR);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);

	return true;
}

static void
i9xx_ring_put_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	unsigned long flags;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (--ring->irq_refcount == 0) {
		dev_priv->irq_mask |= ring->irq_enable_mask;
		I915_WRITE(IMR, dev_priv->irq_mask);
		POSTING_READ(IMR);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);
}

static bool
i8xx_ring_get_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	unsigned long flags;
>>>>>>> refs/remotes/origin/master

	if (!dev->irq_enabled)
		return false;

<<<<<<< HEAD
	spin_lock(&ring->irq_lock);
	if (ring->irq_refcount++ == 0) {
		if (HAS_PCH_SPLIT(dev))
			ironlake_enable_irq(dev_priv,
					    GT_PIPE_NOTIFY | GT_USER_INTERRUPT);
		else
			i915_enable_irq(dev_priv, I915_USER_INTERRUPT);
	}
	spin_unlock(&ring->irq_lock);
=======
	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (ring->irq_refcount++ == 0) {
		dev_priv->irq_mask &= ~ring->irq_enable_mask;
		I915_WRITE16(IMR, dev_priv->irq_mask);
		POSTING_READ16(IMR);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);
>>>>>>> refs/remotes/origin/master

	return true;
}

static void
<<<<<<< HEAD
render_ring_put_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;

	spin_lock(&ring->irq_lock);
	if (--ring->irq_refcount == 0) {
		if (HAS_PCH_SPLIT(dev))
			ironlake_disable_irq(dev_priv,
					     GT_USER_INTERRUPT |
					     GT_PIPE_NOTIFY);
		else
			i915_disable_irq(dev_priv, I915_USER_INTERRUPT);
	}
	spin_unlock(&ring->irq_lock);
=======
i8xx_ring_put_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	unsigned long flags;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (--ring->irq_refcount == 0) {
		dev_priv->irq_mask |= ring->irq_enable_mask;
		I915_WRITE16(IMR, dev_priv->irq_mask);
		POSTING_READ16(IMR);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);
>>>>>>> refs/remotes/origin/master
}

void intel_ring_setup_status_page(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
	u32 mmio = 0;

	/* The ring status page addresses are no longer next to the rest of
	 * the ring registers as of gen7.
	 */
	if (IS_GEN7(dev)) {
		switch (ring->id) {
<<<<<<< HEAD
<<<<<<< HEAD
		case RING_RENDER:
			mmio = RENDER_HWS_PGA_GEN7;
			break;
		case RING_BLT:
			mmio = BLT_HWS_PGA_GEN7;
			break;
		case RING_BSD:
=======
=======
>>>>>>> refs/remotes/origin/master
		case RCS:
			mmio = RENDER_HWS_PGA_GEN7;
			break;
		case BCS:
			mmio = BLT_HWS_PGA_GEN7;
			break;
		case VCS:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			mmio = BSD_HWS_PGA_GEN7;
			break;
=======
			mmio = BSD_HWS_PGA_GEN7;
			break;
		case VECS:
			mmio = VEBOX_HWS_PGA_GEN7;
			break;
>>>>>>> refs/remotes/origin/master
		}
	} else if (IS_GEN6(ring->dev)) {
		mmio = RING_HWS_PGA_GEN6(ring->mmio_base);
	} else {
<<<<<<< HEAD
=======
		/* XXX: gen8 returns to sanity */
>>>>>>> refs/remotes/origin/master
		mmio = RING_HWS_PGA(ring->mmio_base);
	}

	I915_WRITE(mmio, (u32)ring->status_page.gfx_addr);
	POSTING_READ(mmio);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	/* Flush the TLB for this page */
	if (INTEL_INFO(dev)->gen >= 6) {
		u32 reg = RING_INSTPM(ring->mmio_base);
		I915_WRITE(reg,
			   _MASKED_BIT_ENABLE(INSTPM_TLB_INVALIDATE |
					      INSTPM_SYNC_FLUSH));
		if (wait_for((I915_READ(reg) & INSTPM_SYNC_FLUSH) == 0,
			     1000))
			DRM_ERROR("%s: wait for SyncFlush to complete for TLB invalidation timed out\n",
				  ring->name);
	}
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static int
bsd_ring_flush(struct intel_ring_buffer *ring,
	       u32     invalidate_domains,
	       u32     flush_domains)
{
	int ret;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring, MI_FLUSH);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);
	return 0;
}

static int
<<<<<<< HEAD
ring_add_request(struct intel_ring_buffer *ring,
		 u32 *result)
{
	u32 seqno;
=======
i9xx_add_request(struct intel_ring_buffer *ring)
{
>>>>>>> refs/remotes/origin/master
	int ret;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

<<<<<<< HEAD
<<<<<<< HEAD
	seqno = i915_gem_get_seqno(ring->dev);
=======
	seqno = i915_gem_next_request_seqno(ring);
>>>>>>> refs/remotes/origin/cm-10.0

	intel_ring_emit(ring, MI_STORE_DWORD_INDEX);
	intel_ring_emit(ring, I915_GEM_HWS_INDEX << MI_STORE_DWORD_INDEX_SHIFT);
	intel_ring_emit(ring, seqno);
	intel_ring_emit(ring, MI_USER_INTERRUPT);
	intel_ring_advance(ring);

	*result = seqno;
=======
	intel_ring_emit(ring, MI_STORE_DWORD_INDEX);
	intel_ring_emit(ring, I915_GEM_HWS_INDEX << MI_STORE_DWORD_INDEX_SHIFT);
	intel_ring_emit(ring, ring->outstanding_lazy_seqno);
	intel_ring_emit(ring, MI_USER_INTERRUPT);
	__intel_ring_advance(ring);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static bool
<<<<<<< HEAD
gen6_ring_get_irq(struct intel_ring_buffer *ring, u32 gflag, u32 rflag)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
=======
gen6_ring_get_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	unsigned long flags;
>>>>>>> refs/remotes/origin/master

	if (!dev->irq_enabled)
	       return false;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	/* It looks like we need to prevent the gt from suspending while waiting
	 * for an notifiy irq, otherwise irqs seem to get lost on at least the
	 * blt/bsd rings on ivb. */
	gen6_gt_force_wake_get(dev_priv);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	spin_lock(&ring->irq_lock);
	if (ring->irq_refcount++ == 0) {
		ring->irq_mask &= ~rflag;
		I915_WRITE_IMR(ring, ring->irq_mask);
		ironlake_enable_irq(dev_priv, gflag);
	}
	spin_unlock(&ring->irq_lock);
=======
	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (ring->irq_refcount++ == 0) {
		if (HAS_L3_DPF(dev) && ring->id == RCS)
			I915_WRITE_IMR(ring,
				       ~(ring->irq_enable_mask |
					 GT_PARITY_ERROR(dev)));
		else
			I915_WRITE_IMR(ring, ~ring->irq_enable_mask);
		ilk_enable_gt_irq(dev_priv, ring->irq_enable_mask);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);
>>>>>>> refs/remotes/origin/master

	return true;
}

static void
<<<<<<< HEAD
gen6_ring_put_irq(struct intel_ring_buffer *ring, u32 gflag, u32 rflag)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;

	spin_lock(&ring->irq_lock);
	if (--ring->irq_refcount == 0) {
		ring->irq_mask |= rflag;
		I915_WRITE_IMR(ring, ring->irq_mask);
		ironlake_disable_irq(dev_priv, gflag);
	}
	spin_unlock(&ring->irq_lock);
<<<<<<< HEAD
=======

	gen6_gt_force_wake_put(dev_priv);
>>>>>>> refs/remotes/origin/cm-10.0
}

static bool
bsd_ring_get_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
=======
gen6_ring_put_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	unsigned long flags;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (--ring->irq_refcount == 0) {
		if (HAS_L3_DPF(dev) && ring->id == RCS)
			I915_WRITE_IMR(ring, ~GT_PARITY_ERROR(dev));
		else
			I915_WRITE_IMR(ring, ~0);
		ilk_disable_gt_irq(dev_priv, ring->irq_enable_mask);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);

	gen6_gt_force_wake_put(dev_priv);
}

static bool
hsw_vebox_get_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned long flags;
>>>>>>> refs/remotes/origin/master

	if (!dev->irq_enabled)
		return false;

<<<<<<< HEAD
	spin_lock(&ring->irq_lock);
	if (ring->irq_refcount++ == 0) {
		if (IS_G4X(dev))
			i915_enable_irq(dev_priv, I915_BSD_USER_INTERRUPT);
		else
			ironlake_enable_irq(dev_priv, GT_BSD_USER_INTERRUPT);
	}
	spin_unlock(&ring->irq_lock);

	return true;
}
static void
bsd_ring_put_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;

	spin_lock(&ring->irq_lock);
	if (--ring->irq_refcount == 0) {
		if (IS_G4X(dev))
			i915_disable_irq(dev_priv, I915_BSD_USER_INTERRUPT);
		else
			ironlake_disable_irq(dev_priv, GT_BSD_USER_INTERRUPT);
	}
	spin_unlock(&ring->irq_lock);
}

static int
ring_dispatch_execbuffer(struct intel_ring_buffer *ring, u32 offset, u32 length)
=======
	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (ring->irq_refcount++ == 0) {
		I915_WRITE_IMR(ring, ~ring->irq_enable_mask);
		snb_enable_pm_irq(dev_priv, ring->irq_enable_mask);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);

	return true;
}

static void
hsw_vebox_put_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned long flags;

	if (!dev->irq_enabled)
		return;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (--ring->irq_refcount == 0) {
		I915_WRITE_IMR(ring, ~0);
		snb_disable_pm_irq(dev_priv, ring->irq_enable_mask);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);
}

static bool
gen8_ring_get_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned long flags;

	if (!dev->irq_enabled)
		return false;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (ring->irq_refcount++ == 0) {
		if (HAS_L3_DPF(dev) && ring->id == RCS) {
			I915_WRITE_IMR(ring,
				       ~(ring->irq_enable_mask |
					 GT_RENDER_L3_PARITY_ERROR_INTERRUPT));
		} else {
			I915_WRITE_IMR(ring, ~ring->irq_enable_mask);
		}
		POSTING_READ(RING_IMR(ring->mmio_base));
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);

	return true;
}

static void
gen8_ring_put_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned long flags;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (--ring->irq_refcount == 0) {
		if (HAS_L3_DPF(dev) && ring->id == RCS) {
			I915_WRITE_IMR(ring,
				       ~GT_RENDER_L3_PARITY_ERROR_INTERRUPT);
		} else {
			I915_WRITE_IMR(ring, ~0);
		}
		POSTING_READ(RING_IMR(ring->mmio_base));
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);
}

static int
i965_dispatch_execbuffer(struct intel_ring_buffer *ring,
			 u32 offset, u32 length,
			 unsigned flags)
>>>>>>> refs/remotes/origin/master
{
	int ret;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring,
<<<<<<< HEAD
			MI_BATCH_BUFFER_START | (2 << 6) |
			MI_BATCH_NON_SECURE_I965);
=======
			MI_BATCH_BUFFER_START |
			MI_BATCH_GTT |
			(flags & I915_DISPATCH_SECURE ? 0 : MI_BATCH_NON_SECURE_I965));
>>>>>>> refs/remotes/origin/master
	intel_ring_emit(ring, offset);
	intel_ring_advance(ring);

	return 0;
}

<<<<<<< HEAD
static int
render_ring_dispatch_execbuffer(struct intel_ring_buffer *ring,
				u32 offset, u32 len)
{
	struct drm_device *dev = ring->dev;
	int ret;

	if (IS_I830(dev) || IS_845G(dev)) {
=======
/* Just userspace ABI convention to limit the wa batch bo to a resonable size */
#define I830_BATCH_LIMIT (256*1024)
static int
i830_dispatch_execbuffer(struct intel_ring_buffer *ring,
				u32 offset, u32 len,
				unsigned flags)
{
	int ret;

	if (flags & I915_DISPATCH_PINNED) {
>>>>>>> refs/remotes/origin/master
		ret = intel_ring_begin(ring, 4);
		if (ret)
			return ret;

		intel_ring_emit(ring, MI_BATCH_BUFFER);
<<<<<<< HEAD
		intel_ring_emit(ring, offset | MI_BATCH_NON_SECURE);
		intel_ring_emit(ring, offset + len - 8);
		intel_ring_emit(ring, 0);
	} else {
		ret = intel_ring_begin(ring, 2);
		if (ret)
			return ret;

		if (INTEL_INFO(dev)->gen >= 4) {
			intel_ring_emit(ring,
					MI_BATCH_BUFFER_START | (2 << 6) |
					MI_BATCH_NON_SECURE_I965);
			intel_ring_emit(ring, offset);
		} else {
			intel_ring_emit(ring,
					MI_BATCH_BUFFER_START | (2 << 6));
			intel_ring_emit(ring, offset | MI_BATCH_NON_SECURE);
		}
	}
=======
		intel_ring_emit(ring, offset | (flags & I915_DISPATCH_SECURE ? 0 : MI_BATCH_NON_SECURE));
		intel_ring_emit(ring, offset + len - 8);
		intel_ring_emit(ring, MI_NOOP);
		intel_ring_advance(ring);
	} else {
		u32 cs_offset = ring->scratch.gtt_offset;

		if (len > I830_BATCH_LIMIT)
			return -ENOSPC;

		ret = intel_ring_begin(ring, 9+3);
		if (ret)
			return ret;
		/* Blit the batch (which has now all relocs applied) to the stable batch
		 * scratch bo area (so that the CS never stumbles over its tlb
		 * invalidation bug) ... */
		intel_ring_emit(ring, XY_SRC_COPY_BLT_CMD |
				XY_SRC_COPY_BLT_WRITE_ALPHA |
				XY_SRC_COPY_BLT_WRITE_RGB);
		intel_ring_emit(ring, BLT_DEPTH_32 | BLT_ROP_GXCOPY | 4096);
		intel_ring_emit(ring, 0);
		intel_ring_emit(ring, (DIV_ROUND_UP(len, 4096) << 16) | 1024);
		intel_ring_emit(ring, cs_offset);
		intel_ring_emit(ring, 0);
		intel_ring_emit(ring, 4096);
		intel_ring_emit(ring, offset);
		intel_ring_emit(ring, MI_FLUSH);

		/* ... and execute it. */
		intel_ring_emit(ring, MI_BATCH_BUFFER);
		intel_ring_emit(ring, cs_offset | (flags & I915_DISPATCH_SECURE ? 0 : MI_BATCH_NON_SECURE));
		intel_ring_emit(ring, cs_offset + len - 8);
		intel_ring_advance(ring);
	}

	return 0;
}

static int
i915_dispatch_execbuffer(struct intel_ring_buffer *ring,
			 u32 offset, u32 len,
			 unsigned flags)
{
	int ret;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring, MI_BATCH_BUFFER_START | MI_BATCH_GTT);
	intel_ring_emit(ring, offset | (flags & I915_DISPATCH_SECURE ? 0 : MI_BATCH_NON_SECURE));
>>>>>>> refs/remotes/origin/master
	intel_ring_advance(ring);

	return 0;
}

static void cleanup_status_page(struct intel_ring_buffer *ring)
{
<<<<<<< HEAD
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
=======
>>>>>>> refs/remotes/origin/master
	struct drm_i915_gem_object *obj;

	obj = ring->status_page.obj;
	if (obj == NULL)
		return;

<<<<<<< HEAD
	kunmap(obj->pages[0]);
	i915_gem_object_unpin(obj);
	drm_gem_object_unreference(&obj->base);
	ring->status_page.obj = NULL;

	memset(&dev_priv->hws_map, 0, sizeof(dev_priv->hws_map));
=======
	kunmap(sg_page(obj->pages->sgl));
	i915_gem_object_unpin(obj);
	drm_gem_object_unreference(&obj->base);
	ring->status_page.obj = NULL;
>>>>>>> refs/remotes/origin/master
}

static int init_status_page(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
<<<<<<< HEAD
	drm_i915_private_t *dev_priv = dev->dev_private;
=======
>>>>>>> refs/remotes/origin/master
	struct drm_i915_gem_object *obj;
	int ret;

	obj = i915_gem_alloc_object(dev, 4096);
	if (obj == NULL) {
		DRM_ERROR("Failed to allocate status page\n");
		ret = -ENOMEM;
		goto err;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	obj->cache_level = I915_CACHE_LLC;
=======

	i915_gem_object_set_cache_level(obj, I915_CACHE_LLC);
>>>>>>> refs/remotes/origin/cm-10.0

	ret = i915_gem_object_pin(obj, 4096, true);
=======

	i915_gem_object_set_cache_level(obj, I915_CACHE_LLC);

	ret = i915_gem_obj_ggtt_pin(obj, 4096, true, false);
>>>>>>> refs/remotes/origin/master
	if (ret != 0) {
		goto err_unref;
	}

<<<<<<< HEAD
	ring->status_page.gfx_addr = obj->gtt_offset;
	ring->status_page.page_addr = kmap(obj->pages[0]);
	if (ring->status_page.page_addr == NULL) {
		memset(&dev_priv->hws_map, 0, sizeof(dev_priv->hws_map));
=======
	ring->status_page.gfx_addr = i915_gem_obj_ggtt_offset(obj);
	ring->status_page.page_addr = kmap(sg_page(obj->pages->sgl));
	if (ring->status_page.page_addr == NULL) {
		ret = -ENOMEM;
>>>>>>> refs/remotes/origin/master
		goto err_unpin;
	}
	ring->status_page.obj = obj;
	memset(ring->status_page.page_addr, 0, PAGE_SIZE);

<<<<<<< HEAD
	intel_ring_setup_status_page(ring);
=======
>>>>>>> refs/remotes/origin/master
	DRM_DEBUG_DRIVER("%s hws offset: 0x%08x\n",
			ring->name, ring->status_page.gfx_addr);

	return 0;

err_unpin:
	i915_gem_object_unpin(obj);
err_unref:
	drm_gem_object_unreference(&obj->base);
err:
	return ret;
}

<<<<<<< HEAD
int intel_init_ring_buffer(struct drm_device *dev,
			   struct intel_ring_buffer *ring)
{
	struct drm_i915_gem_object *obj;
=======
static int init_phys_status_page(struct intel_ring_buffer *ring)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;

	if (!dev_priv->status_page_dmah) {
		dev_priv->status_page_dmah =
			drm_pci_alloc(ring->dev, PAGE_SIZE, PAGE_SIZE);
		if (!dev_priv->status_page_dmah)
			return -ENOMEM;
	}

	ring->status_page.page_addr = dev_priv->status_page_dmah->vaddr;
	memset(ring->status_page.page_addr, 0, PAGE_SIZE);

	return 0;
}

static int intel_init_ring_buffer(struct drm_device *dev,
				  struct intel_ring_buffer *ring)
{
	struct drm_i915_gem_object *obj;
	struct drm_i915_private *dev_priv = dev->dev_private;
>>>>>>> refs/remotes/origin/master
	int ret;

	ring->dev = dev;
	INIT_LIST_HEAD(&ring->active_list);
	INIT_LIST_HEAD(&ring->request_list);
<<<<<<< HEAD
	INIT_LIST_HEAD(&ring->gpu_write_list);

	init_waitqueue_head(&ring->irq_queue);
	spin_lock_init(&ring->irq_lock);
	ring->irq_mask = ~0;
=======
	ring->size = 32 * PAGE_SIZE;
	memset(ring->sync_seqno, 0, sizeof(ring->sync_seqno));

	init_waitqueue_head(&ring->irq_queue);
>>>>>>> refs/remotes/origin/master

	if (I915_NEED_GFX_HWS(dev)) {
		ret = init_status_page(ring);
		if (ret)
			return ret;
<<<<<<< HEAD
	}

	obj = i915_gem_alloc_object(dev, ring->size);
=======
	} else {
		BUG_ON(ring->id != RCS);
		ret = init_phys_status_page(ring);
		if (ret)
			return ret;
	}

	obj = NULL;
	if (!HAS_LLC(dev))
		obj = i915_gem_object_create_stolen(dev, ring->size);
	if (obj == NULL)
		obj = i915_gem_alloc_object(dev, ring->size);
>>>>>>> refs/remotes/origin/master
	if (obj == NULL) {
		DRM_ERROR("Failed to allocate ringbuffer\n");
		ret = -ENOMEM;
		goto err_hws;
	}

	ring->obj = obj;

<<<<<<< HEAD
	ret = i915_gem_object_pin(obj, PAGE_SIZE, true);
	if (ret)
		goto err_unref;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
	ret = i915_gem_obj_ggtt_pin(obj, PAGE_SIZE, true, false);
	if (ret)
		goto err_unref;

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	ret = i915_gem_object_set_to_gtt_domain(obj, true);
	if (ret)
		goto err_unpin;

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	ring->map.size = ring->size;
	ring->map.offset = dev->agp->base + obj->gtt_offset;
	ring->map.type = 0;
	ring->map.flags = 0;
	ring->map.mtrr = 0;

	drm_core_ioremap_wc(&ring->map, dev);
	if (ring->map.handle == NULL) {
=======
	ring->virtual_start =
		ioremap_wc(dev_priv->gtt.mappable_base + i915_gem_obj_ggtt_offset(obj),
			   ring->size);
	if (ring->virtual_start == NULL) {
>>>>>>> refs/remotes/origin/master
		DRM_ERROR("Failed to map ringbuffer.\n");
		ret = -EINVAL;
		goto err_unpin;
	}

<<<<<<< HEAD
	ring->virtual_start = ring->map.handle;
=======
>>>>>>> refs/remotes/origin/master
	ret = ring->init(ring);
	if (ret)
		goto err_unmap;

	/* Workaround an erratum on the i830 which causes a hang if
	 * the TAIL pointer points to within the last 2 cachelines
	 * of the buffer.
	 */
	ring->effective_size = ring->size;
	if (IS_I830(ring->dev) || IS_845G(ring->dev))
		ring->effective_size -= 128;

	return 0;

err_unmap:
<<<<<<< HEAD
	drm_core_ioremapfree(&ring->map, dev);
=======
	iounmap(ring->virtual_start);
>>>>>>> refs/remotes/origin/master
err_unpin:
	i915_gem_object_unpin(obj);
err_unref:
	drm_gem_object_unreference(&obj->base);
	ring->obj = NULL;
err_hws:
	cleanup_status_page(ring);
	return ret;
}

void intel_cleanup_ring_buffer(struct intel_ring_buffer *ring)
{
	struct drm_i915_private *dev_priv;
	int ret;

	if (ring->obj == NULL)
		return;

	/* Disable the ring buffer. The ring must be idle at this point */
	dev_priv = ring->dev->dev_private;
<<<<<<< HEAD
	ret = intel_wait_ring_idle(ring);
	if (ret)
=======
	ret = intel_ring_idle(ring);
	if (ret && !i915_reset_in_progress(&dev_priv->gpu_error))
>>>>>>> refs/remotes/origin/master
		DRM_ERROR("failed to quiesce %s whilst cleaning up: %d\n",
			  ring->name, ret);

	I915_WRITE_CTL(ring, 0);

<<<<<<< HEAD
	drm_core_ioremapfree(&ring->map, ring->dev);
=======
	iounmap(ring->virtual_start);
>>>>>>> refs/remotes/origin/master

	i915_gem_object_unpin(ring->obj);
	drm_gem_object_unreference(&ring->obj->base);
	ring->obj = NULL;
<<<<<<< HEAD
=======
	ring->preallocated_lazy_request = NULL;
	ring->outstanding_lazy_seqno = 0;
>>>>>>> refs/remotes/origin/master

	if (ring->cleanup)
		ring->cleanup(ring);

	cleanup_status_page(ring);
}

<<<<<<< HEAD
static int intel_wrap_ring_buffer(struct intel_ring_buffer *ring)
{
	unsigned int *virt;
	int rem = ring->size - ring->tail;

	if (ring->space < rem) {
		int ret = intel_wait_ring_buffer(ring, rem);
		if (ret)
			return ret;
	}

	virt = (unsigned int *)(ring->virtual_start + ring->tail);
	rem /= 8;
	while (rem--) {
		*virt++ = MI_NOOP;
		*virt++ = MI_NOOP;
	}

	ring->tail = 0;
	ring->space = ring_space(ring);

	return 0;
}

<<<<<<< HEAD
int intel_wait_ring_buffer(struct intel_ring_buffer *ring, int n)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned long end;
	u32 head;

	/* If the reported head position has wrapped or hasn't advanced,
	 * fallback to the slow and accurate path.
	 */
	head = intel_read_status_page(ring, 4);
	if (head > ring->head) {
		ring->head = head;
=======
static int intel_ring_wait_seqno(struct intel_ring_buffer *ring, u32 seqno)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;
	bool was_interruptible;
	int ret;

	/* XXX As we have not yet audited all the paths to check that
	 * they are ready for ERESTARTSYS from intel_ring_begin, do not
	 * allow us to be interruptible by a signal.
	 */
	was_interruptible = dev_priv->mm.interruptible;
	dev_priv->mm.interruptible = false;

	ret = i915_wait_request(ring, seqno, true);

	dev_priv->mm.interruptible = was_interruptible;
=======
static int intel_ring_wait_seqno(struct intel_ring_buffer *ring, u32 seqno)
{
	int ret;

	ret = i915_wait_seqno(ring, seqno);
	if (!ret)
		i915_gem_retire_requests_ring(ring);
>>>>>>> refs/remotes/origin/master

	return ret;
}

static int intel_ring_wait_request(struct intel_ring_buffer *ring, int n)
{
	struct drm_i915_gem_request *request;
	u32 seqno = 0;
	int ret;

	i915_gem_retire_requests_ring(ring);

	if (ring->last_retired_head != -1) {
		ring->head = ring->last_retired_head;
		ring->last_retired_head = -1;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		ring->space = ring_space(ring);
		if (ring->space >= n)
			return 0;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	trace_i915_ring_wait_begin(ring);
	end = jiffies + 3 * HZ;
=======
=======
>>>>>>> refs/remotes/origin/master
	list_for_each_entry(request, &ring->request_list, list) {
		int space;

		if (request->tail == -1)
			continue;

<<<<<<< HEAD
		space = request->tail - (ring->tail + 8);
=======
		space = request->tail - (ring->tail + I915_RING_FREE_SPACE);
>>>>>>> refs/remotes/origin/master
		if (space < 0)
			space += ring->size;
		if (space >= n) {
			seqno = request->seqno;
			break;
		}

		/* Consume this request in case we need more space than
		 * is available and so need to prevent a race between
		 * updating last_retired_head and direct reads of
		 * I915_RING_HEAD. It also provides a nice sanity check.
		 */
		request->tail = -1;
	}

	if (seqno == 0)
		return -ENOSPC;

	ret = intel_ring_wait_seqno(ring, seqno);
	if (ret)
		return ret;

	if (WARN_ON(ring->last_retired_head == -1))
		return -ENOSPC;

	ring->head = ring->last_retired_head;
	ring->last_retired_head = -1;
	ring->space = ring_space(ring);
	if (WARN_ON(ring->space < n))
		return -ENOSPC;

	return 0;
}

<<<<<<< HEAD
int intel_wait_ring_buffer(struct intel_ring_buffer *ring, int n)
=======
static int ring_wait_for_space(struct intel_ring_buffer *ring, int n)
>>>>>>> refs/remotes/origin/master
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned long end;
	int ret;

	ret = intel_ring_wait_request(ring, n);
	if (ret != -ENOSPC)
		return ret;

<<<<<<< HEAD
	trace_i915_ring_wait_begin(ring);
	if (drm_core_check_feature(dev, DRIVER_GEM))
		/* With GEM the hangcheck timer should kick us out of the loop,
		 * leaving it early runs the risk of corrupting GEM state (due
		 * to running on almost untested codepaths). But on resume
		 * timers don't work yet, so prevent a complete hang in that
		 * case by choosing an insanely large timeout. */
		end = jiffies + 60 * HZ;
	else
		end = jiffies + 3 * HZ;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* force the tail write in case we have been skipping them */
	__intel_ring_advance(ring);

	trace_i915_ring_wait_begin(ring);
	/* With GEM the hangcheck timer should kick us out of the loop,
	 * leaving it early runs the risk of corrupting GEM state (due
	 * to running on almost untested codepaths). But on resume
	 * timers don't work yet, so prevent a complete hang in that
	 * case by choosing an insanely large timeout. */
	end = jiffies + 60 * HZ;

>>>>>>> refs/remotes/origin/master
	do {
		ring->head = I915_READ_HEAD(ring);
		ring->space = ring_space(ring);
		if (ring->space >= n) {
			trace_i915_ring_wait_end(ring);
			return 0;
		}

		if (dev->primary->master) {
			struct drm_i915_master_private *master_priv = dev->primary->master->driver_priv;
			if (master_priv->sarea_priv)
				master_priv->sarea_priv->perf_boxes |= I915_BOX_WAIT;
		}

		msleep(1);
<<<<<<< HEAD
		if (atomic_read(&dev_priv->mm.wedged))
			return -EAGAIN;
=======

		ret = i915_gem_check_wedge(&dev_priv->gpu_error,
					   dev_priv->mm.interruptible);
		if (ret)
			return ret;
>>>>>>> refs/remotes/origin/master
	} while (!time_after(jiffies, end));
	trace_i915_ring_wait_end(ring);
	return -EBUSY;
}

<<<<<<< HEAD
int intel_ring_begin(struct intel_ring_buffer *ring,
		     int num_dwords)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;
	int n = 4*num_dwords;
	int ret;

	if (unlikely(atomic_read(&dev_priv->mm.wedged)))
		return -EIO;

	if (unlikely(ring->tail + n > ring->effective_size)) {
=======
static int intel_wrap_ring_buffer(struct intel_ring_buffer *ring)
{
	uint32_t __iomem *virt;
	int rem = ring->size - ring->tail;

	if (ring->space < rem) {
		int ret = ring_wait_for_space(ring, rem);
		if (ret)
			return ret;
	}

	virt = ring->virtual_start + ring->tail;
	rem /= 4;
	while (rem--)
		iowrite32(MI_NOOP, virt++);

	ring->tail = 0;
	ring->space = ring_space(ring);

	return 0;
}

int intel_ring_idle(struct intel_ring_buffer *ring)
{
	u32 seqno;
	int ret;

	/* We need to add any requests required to flush the objects and ring */
	if (ring->outstanding_lazy_seqno) {
		ret = i915_add_request(ring, NULL);
		if (ret)
			return ret;
	}

	/* Wait upon the last request to be completed */
	if (list_empty(&ring->request_list))
		return 0;

	seqno = list_entry(ring->request_list.prev,
			   struct drm_i915_gem_request,
			   list)->seqno;

	return i915_wait_seqno(ring, seqno);
}

static int
intel_ring_alloc_seqno(struct intel_ring_buffer *ring)
{
	if (ring->outstanding_lazy_seqno)
		return 0;

	if (ring->preallocated_lazy_request == NULL) {
		struct drm_i915_gem_request *request;

		request = kmalloc(sizeof(*request), GFP_KERNEL);
		if (request == NULL)
			return -ENOMEM;

		ring->preallocated_lazy_request = request;
	}

	return i915_gem_get_seqno(ring->dev, &ring->outstanding_lazy_seqno);
}

static int __intel_ring_begin(struct intel_ring_buffer *ring,
			      int bytes)
{
	int ret;

	if (unlikely(ring->tail + bytes > ring->effective_size)) {
>>>>>>> refs/remotes/origin/master
		ret = intel_wrap_ring_buffer(ring);
		if (unlikely(ret))
			return ret;
	}

<<<<<<< HEAD
	if (unlikely(ring->space < n)) {
		ret = intel_wait_ring_buffer(ring, n);
=======
	if (unlikely(ring->space < bytes)) {
		ret = ring_wait_for_space(ring, bytes);
>>>>>>> refs/remotes/origin/master
		if (unlikely(ret))
			return ret;
	}

<<<<<<< HEAD
	ring->space -= n;
	return 0;
}

void intel_ring_advance(struct intel_ring_buffer *ring)
{
	ring->tail &= ring->size - 1;
	ring->write_tail(ring, ring->tail);
}

static const struct intel_ring_buffer render_ring = {
	.name			= "render ring",
<<<<<<< HEAD
	.id			= RING_RENDER,
=======
	.id			= RCS,
>>>>>>> refs/remotes/origin/cm-10.0
	.mmio_base		= RENDER_RING_BASE,
	.size			= 32 * PAGE_SIZE,
	.init			= init_render_ring,
	.write_tail		= ring_write_tail,
	.flush			= render_ring_flush,
	.add_request		= render_ring_add_request,
	.get_seqno		= ring_get_seqno,
	.irq_get		= render_ring_get_irq,
	.irq_put		= render_ring_put_irq,
	.dispatch_execbuffer	= render_ring_dispatch_execbuffer,
<<<<<<< HEAD
       .cleanup			= render_ring_cleanup,
=======
	.cleanup		= render_ring_cleanup,
	.sync_to		= render_ring_sync_to,
	.semaphore_register	= {MI_SEMAPHORE_SYNC_INVALID,
				   MI_SEMAPHORE_SYNC_RV,
				   MI_SEMAPHORE_SYNC_RB},
	.signal_mbox		= {GEN6_VRSYNC, GEN6_BRSYNC},
>>>>>>> refs/remotes/origin/cm-10.0
};

/* ring buffer for bit-stream decoder */

static const struct intel_ring_buffer bsd_ring = {
	.name                   = "bsd ring",
<<<<<<< HEAD
	.id			= RING_BSD,
=======
	.id			= VCS,
>>>>>>> refs/remotes/origin/cm-10.0
	.mmio_base		= BSD_RING_BASE,
	.size			= 32 * PAGE_SIZE,
	.init			= init_ring_common,
	.write_tail		= ring_write_tail,
	.flush			= bsd_ring_flush,
	.add_request		= ring_add_request,
	.get_seqno		= ring_get_seqno,
	.irq_get		= bsd_ring_get_irq,
	.irq_put		= bsd_ring_put_irq,
	.dispatch_execbuffer	= ring_dispatch_execbuffer,
};

=======
	ring->space -= bytes;
	return 0;
}

int intel_ring_begin(struct intel_ring_buffer *ring,
		     int num_dwords)
{
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
	int ret;

	ret = i915_gem_check_wedge(&dev_priv->gpu_error,
				   dev_priv->mm.interruptible);
	if (ret)
		return ret;

	/* Preallocate the olr before touching the ring */
	ret = intel_ring_alloc_seqno(ring);
	if (ret)
		return ret;

	return __intel_ring_begin(ring, num_dwords * sizeof(uint32_t));
}

void intel_ring_init_seqno(struct intel_ring_buffer *ring, u32 seqno)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;

	BUG_ON(ring->outstanding_lazy_seqno);

	if (INTEL_INFO(ring->dev)->gen >= 6) {
		I915_WRITE(RING_SYNC_0(ring->mmio_base), 0);
		I915_WRITE(RING_SYNC_1(ring->mmio_base), 0);
		if (HAS_VEBOX(ring->dev))
			I915_WRITE(RING_SYNC_2(ring->mmio_base), 0);
	}

	ring->set_seqno(ring, seqno);
	ring->hangcheck.seqno = seqno;
}
>>>>>>> refs/remotes/origin/master

static void gen6_bsd_ring_write_tail(struct intel_ring_buffer *ring,
				     u32 value)
{
<<<<<<< HEAD
<<<<<<< HEAD
       drm_i915_private_t *dev_priv = ring->dev->dev_private;

       /* Every tail move must follow the sequence below */
       I915_WRITE(GEN6_BSD_SLEEP_PSMI_CONTROL,
	       GEN6_BSD_SLEEP_PSMI_CONTROL_RC_ILDL_MESSAGE_MODIFY_MASK |
	       GEN6_BSD_SLEEP_PSMI_CONTROL_RC_ILDL_MESSAGE_DISABLE);
       I915_WRITE(GEN6_BSD_RNCID, 0x0);

       if (wait_for((I915_READ(GEN6_BSD_SLEEP_PSMI_CONTROL) &
                               GEN6_BSD_SLEEP_PSMI_CONTROL_IDLE_INDICATOR) == 0,
                       50))
               DRM_ERROR("timed out waiting for IDLE Indicator\n");

       I915_WRITE_TAIL(ring, value);
       I915_WRITE(GEN6_BSD_SLEEP_PSMI_CONTROL,
	       GEN6_BSD_SLEEP_PSMI_CONTROL_RC_ILDL_MESSAGE_MODIFY_MASK |
	       GEN6_BSD_SLEEP_PSMI_CONTROL_RC_ILDL_MESSAGE_ENABLE);
=======
	drm_i915_private_t *dev_priv = ring->dev->dev_private;

       /* Every tail move must follow the sequence below */
	I915_WRITE(GEN6_BSD_SLEEP_PSMI_CONTROL,
		GEN6_BSD_SLEEP_PSMI_CONTROL_RC_ILDL_MESSAGE_MODIFY_MASK |
		GEN6_BSD_SLEEP_PSMI_CONTROL_RC_ILDL_MESSAGE_DISABLE);
	I915_WRITE(GEN6_BSD_RNCID, 0x0);

	if (wait_for((I915_READ(GEN6_BSD_SLEEP_PSMI_CONTROL) &
		GEN6_BSD_SLEEP_PSMI_CONTROL_IDLE_INDICATOR) == 0,
		50))
	DRM_ERROR("timed out waiting for IDLE Indicator\n");

	I915_WRITE_TAIL(ring, value);
	I915_WRITE(GEN6_BSD_SLEEP_PSMI_CONTROL,
		GEN6_BSD_SLEEP_PSMI_CONTROL_RC_ILDL_MESSAGE_MODIFY_MASK |
		GEN6_BSD_SLEEP_PSMI_CONTROL_RC_ILDL_MESSAGE_ENABLE);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int gen6_ring_flush(struct intel_ring_buffer *ring,
			   u32 invalidate, u32 flush)
=======
	drm_i915_private_t *dev_priv = ring->dev->dev_private;

       /* Every tail move must follow the sequence below */

	/* Disable notification that the ring is IDLE. The GT
	 * will then assume that it is busy and bring it out of rc6.
	 */
	I915_WRITE(GEN6_BSD_SLEEP_PSMI_CONTROL,
		   _MASKED_BIT_ENABLE(GEN6_BSD_SLEEP_MSG_DISABLE));

	/* Clear the context id. Here be magic! */
	I915_WRITE64(GEN6_BSD_RNCID, 0x0);

	/* Wait for the ring not to be idle, i.e. for it to wake up. */
	if (wait_for((I915_READ(GEN6_BSD_SLEEP_PSMI_CONTROL) &
		      GEN6_BSD_SLEEP_INDICATOR) == 0,
		     50))
		DRM_ERROR("timed out waiting for the BSD ring to wake up\n");

	/* Now that the ring is fully powered up, update the tail */
	I915_WRITE_TAIL(ring, value);
	POSTING_READ(RING_TAIL(ring->mmio_base));

	/* Let the ring send IDLE messages to the GT again,
	 * and so let it sleep to conserve power when idle.
	 */
	I915_WRITE(GEN6_BSD_SLEEP_PSMI_CONTROL,
		   _MASKED_BIT_DISABLE(GEN6_BSD_SLEEP_MSG_DISABLE));
}

static int gen6_bsd_ring_flush(struct intel_ring_buffer *ring,
			       u32 invalidate, u32 flush)
>>>>>>> refs/remotes/origin/master
{
	uint32_t cmd;
	int ret;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	cmd = MI_FLUSH_DW;
<<<<<<< HEAD
	if (invalidate & I915_GEM_GPU_DOMAINS)
		cmd |= MI_INVALIDATE_TLB | MI_INVALIDATE_BSD;
	intel_ring_emit(ring, cmd);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, MI_NOOP);
=======
	if (INTEL_INFO(ring->dev)->gen >= 8)
		cmd += 1;
	/*
	 * Bspec vol 1c.5 - video engine command streamer:
	 * "If ENABLED, all TLBs will be invalidated once the flush
	 * operation is complete. This bit is only valid when the
	 * Post-Sync Operation field is a value of 1h or 3h."
	 */
	if (invalidate & I915_GEM_GPU_DOMAINS)
		cmd |= MI_INVALIDATE_TLB | MI_INVALIDATE_BSD |
			MI_FLUSH_DW_STORE_INDEX | MI_FLUSH_DW_OP_STOREDW;
	intel_ring_emit(ring, cmd);
	intel_ring_emit(ring, I915_GEM_HWS_SCRATCH_ADDR | MI_FLUSH_DW_USE_GTT);
	if (INTEL_INFO(ring->dev)->gen >= 8) {
		intel_ring_emit(ring, 0); /* upper addr */
		intel_ring_emit(ring, 0); /* value */
	} else  {
		intel_ring_emit(ring, 0);
		intel_ring_emit(ring, MI_NOOP);
	}
>>>>>>> refs/remotes/origin/master
	intel_ring_advance(ring);
	return 0;
}

static int
<<<<<<< HEAD
gen6_ring_dispatch_execbuffer(struct intel_ring_buffer *ring,
			      u32 offset, u32 len)
{
<<<<<<< HEAD
       int ret;

       ret = intel_ring_begin(ring, 2);
       if (ret)
	       return ret;

       intel_ring_emit(ring, MI_BATCH_BUFFER_START | MI_BATCH_NON_SECURE_I965);
       /* bit0-7 is the length on GEN6+ */
       intel_ring_emit(ring, offset);
       intel_ring_advance(ring);

       return 0;
=======
	int ret;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring, MI_BATCH_BUFFER_START | MI_BATCH_NON_SECURE_I965);
	/* bit0-7 is the length on GEN6+ */
	intel_ring_emit(ring, offset);
	intel_ring_advance(ring);

	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

static bool
gen6_render_ring_get_irq(struct intel_ring_buffer *ring)
{
	return gen6_ring_get_irq(ring,
				 GT_USER_INTERRUPT,
				 GEN6_RENDER_USER_INTERRUPT);
}

static void
gen6_render_ring_put_irq(struct intel_ring_buffer *ring)
{
	return gen6_ring_put_irq(ring,
				 GT_USER_INTERRUPT,
				 GEN6_RENDER_USER_INTERRUPT);
}

static bool
gen6_bsd_ring_get_irq(struct intel_ring_buffer *ring)
{
	return gen6_ring_get_irq(ring,
				 GT_GEN6_BSD_USER_INTERRUPT,
				 GEN6_BSD_USER_INTERRUPT);
}

static void
gen6_bsd_ring_put_irq(struct intel_ring_buffer *ring)
{
	return gen6_ring_put_irq(ring,
				 GT_GEN6_BSD_USER_INTERRUPT,
				 GEN6_BSD_USER_INTERRUPT);
}

/* ring buffer for Video Codec for Gen6+ */
static const struct intel_ring_buffer gen6_bsd_ring = {
	.name			= "gen6 bsd ring",
<<<<<<< HEAD
	.id			= RING_BSD,
=======
	.id			= VCS,
>>>>>>> refs/remotes/origin/cm-10.0
	.mmio_base		= GEN6_BSD_RING_BASE,
	.size			= 32 * PAGE_SIZE,
	.init			= init_ring_common,
	.write_tail		= gen6_bsd_ring_write_tail,
	.flush			= gen6_ring_flush,
	.add_request		= gen6_add_request,
<<<<<<< HEAD
	.get_seqno		= ring_get_seqno,
	.irq_get		= gen6_bsd_ring_get_irq,
	.irq_put		= gen6_bsd_ring_put_irq,
	.dispatch_execbuffer	= gen6_ring_dispatch_execbuffer,
=======
	.get_seqno		= gen6_ring_get_seqno,
	.irq_get		= gen6_bsd_ring_get_irq,
	.irq_put		= gen6_bsd_ring_put_irq,
	.dispatch_execbuffer	= gen6_ring_dispatch_execbuffer,
	.sync_to		= gen6_bsd_ring_sync_to,
	.semaphore_register	= {MI_SEMAPHORE_SYNC_VR,
				   MI_SEMAPHORE_SYNC_INVALID,
				   MI_SEMAPHORE_SYNC_VB},
	.signal_mbox		= {GEN6_RVSYNC, GEN6_BVSYNC},
>>>>>>> refs/remotes/origin/cm-10.0
};

/* Blitter support (SandyBridge+) */

static bool
blt_ring_get_irq(struct intel_ring_buffer *ring)
{
	return gen6_ring_get_irq(ring,
				 GT_BLT_USER_INTERRUPT,
				 GEN6_BLITTER_USER_INTERRUPT);
}

static void
blt_ring_put_irq(struct intel_ring_buffer *ring)
{
	gen6_ring_put_irq(ring,
			  GT_BLT_USER_INTERRUPT,
			  GEN6_BLITTER_USER_INTERRUPT);
}

<<<<<<< HEAD

/* Workaround for some stepping of SNB,
 * each time when BLT engine ring tail moved,
 * the first command in the ring to be parsed
 * should be MI_BATCH_BUFFER_START
 */
#define NEED_BLT_WORKAROUND(dev) \
	(IS_GEN6(dev) && (dev->pdev->revision < 8))

static inline struct drm_i915_gem_object *
to_blt_workaround(struct intel_ring_buffer *ring)
{
	return ring->private;
}

static int blt_ring_init(struct intel_ring_buffer *ring)
{
	if (NEED_BLT_WORKAROUND(ring->dev)) {
		struct drm_i915_gem_object *obj;
		u32 *ptr;
		int ret;

		obj = i915_gem_alloc_object(ring->dev, 4096);
		if (obj == NULL)
			return -ENOMEM;

		ret = i915_gem_object_pin(obj, 4096, true);
		if (ret) {
			drm_gem_object_unreference(&obj->base);
			return ret;
		}

		ptr = kmap(obj->pages[0]);
		*ptr++ = MI_BATCH_BUFFER_END;
		*ptr++ = MI_NOOP;
		kunmap(obj->pages[0]);

		ret = i915_gem_object_set_to_gtt_domain(obj, false);
		if (ret) {
			i915_gem_object_unpin(obj);
			drm_gem_object_unreference(&obj->base);
			return ret;
		}

		ring->private = obj;
	}

	return init_ring_common(ring);
}

static int blt_ring_begin(struct intel_ring_buffer *ring,
			  int num_dwords)
{
	if (ring->private) {
		int ret = intel_ring_begin(ring, num_dwords+2);
		if (ret)
			return ret;

		intel_ring_emit(ring, MI_BATCH_BUFFER_START);
		intel_ring_emit(ring, to_blt_workaround(ring)->gtt_offset);

		return 0;
	} else
		return intel_ring_begin(ring, 4);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static int blt_ring_flush(struct intel_ring_buffer *ring,
			  u32 invalidate, u32 flush)
{
	uint32_t cmd;
	int ret;

<<<<<<< HEAD
	ret = blt_ring_begin(ring, 4);
=======
	ret = intel_ring_begin(ring, 4);
>>>>>>> refs/remotes/origin/cm-10.0
=======
gen8_ring_dispatch_execbuffer(struct intel_ring_buffer *ring,
			      u32 offset, u32 len,
			      unsigned flags)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;
	bool ppgtt = dev_priv->mm.aliasing_ppgtt != NULL &&
		!(flags & I915_DISPATCH_SECURE);
	int ret;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	/* FIXME(BDW): Address space and security selectors. */
	intel_ring_emit(ring, MI_BATCH_BUFFER_START_GEN8 | (ppgtt<<8));
	intel_ring_emit(ring, offset);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);

	return 0;
}

static int
hsw_ring_dispatch_execbuffer(struct intel_ring_buffer *ring,
			      u32 offset, u32 len,
			      unsigned flags)
{
	int ret;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring,
			MI_BATCH_BUFFER_START | MI_BATCH_PPGTT_HSW |
			(flags & I915_DISPATCH_SECURE ? 0 : MI_BATCH_NON_SECURE_HSW));
	/* bit0-7 is the length on GEN6+ */
	intel_ring_emit(ring, offset);
	intel_ring_advance(ring);

	return 0;
}

static int
gen6_ring_dispatch_execbuffer(struct intel_ring_buffer *ring,
			      u32 offset, u32 len,
			      unsigned flags)
{
	int ret;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring,
			MI_BATCH_BUFFER_START |
			(flags & I915_DISPATCH_SECURE ? 0 : MI_BATCH_NON_SECURE_I965));
	/* bit0-7 is the length on GEN6+ */
	intel_ring_emit(ring, offset);
	intel_ring_advance(ring);

	return 0;
}

/* Blitter support (SandyBridge+) */

static int gen6_ring_flush(struct intel_ring_buffer *ring,
			   u32 invalidate, u32 flush)
{
	struct drm_device *dev = ring->dev;
	uint32_t cmd;
	int ret;

	ret = intel_ring_begin(ring, 4);
>>>>>>> refs/remotes/origin/master
	if (ret)
		return ret;

	cmd = MI_FLUSH_DW;
<<<<<<< HEAD
	if (invalidate & I915_GEM_DOMAIN_RENDER)
		cmd |= MI_INVALIDATE_TLB;
	intel_ring_emit(ring, cmd);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);
	return 0;
}

<<<<<<< HEAD
static void blt_ring_cleanup(struct intel_ring_buffer *ring)
{
	if (!ring->private)
		return;

	i915_gem_object_unpin(ring->private);
	drm_gem_object_unreference(ring->private);
	ring->private = NULL;
}

static const struct intel_ring_buffer gen6_blt_ring = {
       .name			= "blt ring",
       .id			= RING_BLT,
       .mmio_base		= BLT_RING_BASE,
       .size			= 32 * PAGE_SIZE,
       .init			= blt_ring_init,
       .write_tail		= ring_write_tail,
       .flush			= blt_ring_flush,
       .add_request		= gen6_add_request,
       .get_seqno		= ring_get_seqno,
       .irq_get			= blt_ring_get_irq,
       .irq_put			= blt_ring_put_irq,
       .dispatch_execbuffer	= gen6_ring_dispatch_execbuffer,
       .cleanup			= blt_ring_cleanup,
=======
static const struct intel_ring_buffer gen6_blt_ring = {
	.name			= "blt ring",
	.id			= BCS,
	.mmio_base		= BLT_RING_BASE,
	.size			= 32 * PAGE_SIZE,
	.init			= init_ring_common,
	.write_tail		= ring_write_tail,
	.flush			= blt_ring_flush,
	.add_request		= gen6_add_request,
	.get_seqno		= gen6_ring_get_seqno,
	.irq_get		= blt_ring_get_irq,
	.irq_put		= blt_ring_put_irq,
	.dispatch_execbuffer	= gen6_ring_dispatch_execbuffer,
	.sync_to		= gen6_blt_ring_sync_to,
	.semaphore_register	= {MI_SEMAPHORE_SYNC_BR,
				   MI_SEMAPHORE_SYNC_BV,
				   MI_SEMAPHORE_SYNC_INVALID},
	.signal_mbox		= {GEN6_RBSYNC, GEN6_VBSYNC},
>>>>>>> refs/remotes/origin/cm-10.0
};
=======
	if (INTEL_INFO(ring->dev)->gen >= 8)
		cmd += 1;
	/*
	 * Bspec vol 1c.3 - blitter engine command streamer:
	 * "If ENABLED, all TLBs will be invalidated once the flush
	 * operation is complete. This bit is only valid when the
	 * Post-Sync Operation field is a value of 1h or 3h."
	 */
	if (invalidate & I915_GEM_DOMAIN_RENDER)
		cmd |= MI_INVALIDATE_TLB | MI_FLUSH_DW_STORE_INDEX |
			MI_FLUSH_DW_OP_STOREDW;
	intel_ring_emit(ring, cmd);
	intel_ring_emit(ring, I915_GEM_HWS_SCRATCH_ADDR | MI_FLUSH_DW_USE_GTT);
	if (INTEL_INFO(ring->dev)->gen >= 8) {
		intel_ring_emit(ring, 0); /* upper addr */
		intel_ring_emit(ring, 0); /* value */
	} else  {
		intel_ring_emit(ring, 0);
		intel_ring_emit(ring, MI_NOOP);
	}
	intel_ring_advance(ring);

	if (IS_GEN7(dev) && flush)
		return gen7_ring_fbc_flush(ring, FBC_REND_CACHE_CLEAN);

	return 0;
}
>>>>>>> refs/remotes/origin/master

int intel_init_render_ring_buffer(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_ring_buffer *ring = &dev_priv->ring[RCS];

<<<<<<< HEAD
	*ring = render_ring;
	if (INTEL_INFO(dev)->gen >= 6) {
		ring->add_request = gen6_add_request;
<<<<<<< HEAD
		ring->irq_get = gen6_render_ring_get_irq;
		ring->irq_put = gen6_render_ring_put_irq;
=======
		ring->flush = gen6_render_ring_flush;
		ring->irq_get = gen6_render_ring_get_irq;
		ring->irq_put = gen6_render_ring_put_irq;
		ring->get_seqno = gen6_ring_get_seqno;
>>>>>>> refs/remotes/origin/cm-10.0
	} else if (IS_GEN5(dev)) {
		ring->add_request = pc_render_add_request;
		ring->get_seqno = pc_render_get_seqno;
	}

	if (!I915_NEED_GFX_HWS(dev)) {
		ring->status_page.page_addr = dev_priv->status_page_dmah->vaddr;
		memset(ring->status_page.page_addr, 0, PAGE_SIZE);
=======
	ring->name = "render ring";
	ring->id = RCS;
	ring->mmio_base = RENDER_RING_BASE;

	if (INTEL_INFO(dev)->gen >= 6) {
		ring->add_request = gen6_add_request;
		ring->flush = gen7_render_ring_flush;
		if (INTEL_INFO(dev)->gen == 6)
			ring->flush = gen6_render_ring_flush;
		if (INTEL_INFO(dev)->gen >= 8) {
			ring->flush = gen8_render_ring_flush;
			ring->irq_get = gen8_ring_get_irq;
			ring->irq_put = gen8_ring_put_irq;
		} else {
			ring->irq_get = gen6_ring_get_irq;
			ring->irq_put = gen6_ring_put_irq;
		}
		ring->irq_enable_mask = GT_RENDER_USER_INTERRUPT;
		ring->get_seqno = gen6_ring_get_seqno;
		ring->set_seqno = ring_set_seqno;
		ring->sync_to = gen6_ring_sync;
		ring->semaphore_register[RCS] = MI_SEMAPHORE_SYNC_INVALID;
		ring->semaphore_register[VCS] = MI_SEMAPHORE_SYNC_RV;
		ring->semaphore_register[BCS] = MI_SEMAPHORE_SYNC_RB;
		ring->semaphore_register[VECS] = MI_SEMAPHORE_SYNC_RVE;
		ring->signal_mbox[RCS] = GEN6_NOSYNC;
		ring->signal_mbox[VCS] = GEN6_VRSYNC;
		ring->signal_mbox[BCS] = GEN6_BRSYNC;
		ring->signal_mbox[VECS] = GEN6_VERSYNC;
	} else if (IS_GEN5(dev)) {
		ring->add_request = pc_render_add_request;
		ring->flush = gen4_render_ring_flush;
		ring->get_seqno = pc_render_get_seqno;
		ring->set_seqno = pc_render_set_seqno;
		ring->irq_get = gen5_ring_get_irq;
		ring->irq_put = gen5_ring_put_irq;
		ring->irq_enable_mask = GT_RENDER_USER_INTERRUPT |
					GT_RENDER_PIPECTL_NOTIFY_INTERRUPT;
	} else {
		ring->add_request = i9xx_add_request;
		if (INTEL_INFO(dev)->gen < 4)
			ring->flush = gen2_render_ring_flush;
		else
			ring->flush = gen4_render_ring_flush;
		ring->get_seqno = ring_get_seqno;
		ring->set_seqno = ring_set_seqno;
		if (IS_GEN2(dev)) {
			ring->irq_get = i8xx_ring_get_irq;
			ring->irq_put = i8xx_ring_put_irq;
		} else {
			ring->irq_get = i9xx_ring_get_irq;
			ring->irq_put = i9xx_ring_put_irq;
		}
		ring->irq_enable_mask = I915_USER_INTERRUPT;
	}
	ring->write_tail = ring_write_tail;
	if (IS_HASWELL(dev))
		ring->dispatch_execbuffer = hsw_ring_dispatch_execbuffer;
	else if (IS_GEN8(dev))
		ring->dispatch_execbuffer = gen8_ring_dispatch_execbuffer;
	else if (INTEL_INFO(dev)->gen >= 6)
		ring->dispatch_execbuffer = gen6_ring_dispatch_execbuffer;
	else if (INTEL_INFO(dev)->gen >= 4)
		ring->dispatch_execbuffer = i965_dispatch_execbuffer;
	else if (IS_I830(dev) || IS_845G(dev))
		ring->dispatch_execbuffer = i830_dispatch_execbuffer;
	else
		ring->dispatch_execbuffer = i915_dispatch_execbuffer;
	ring->init = init_render_ring;
	ring->cleanup = render_ring_cleanup;

	/* Workaround batchbuffer to combat CS tlb bug. */
	if (HAS_BROKEN_CS_TLB(dev)) {
		struct drm_i915_gem_object *obj;
		int ret;

		obj = i915_gem_alloc_object(dev, I830_BATCH_LIMIT);
		if (obj == NULL) {
			DRM_ERROR("Failed to allocate batch bo\n");
			return -ENOMEM;
		}

		ret = i915_gem_obj_ggtt_pin(obj, 0, true, false);
		if (ret != 0) {
			drm_gem_object_unreference(&obj->base);
			DRM_ERROR("Failed to ping batch bo\n");
			return ret;
		}

		ring->scratch.obj = obj;
		ring->scratch.gtt_offset = i915_gem_obj_ggtt_offset(obj);
>>>>>>> refs/remotes/origin/master
	}

	return intel_init_ring_buffer(dev, ring);
}

int intel_render_ring_init_dri(struct drm_device *dev, u64 start, u32 size)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_ring_buffer *ring = &dev_priv->ring[RCS];
<<<<<<< HEAD

	*ring = render_ring;
	if (INTEL_INFO(dev)->gen >= 6) {
		ring->add_request = gen6_add_request;
		ring->irq_get = gen6_render_ring_get_irq;
		ring->irq_put = gen6_render_ring_put_irq;
	} else if (IS_GEN5(dev)) {
		ring->add_request = pc_render_add_request;
		ring->get_seqno = pc_render_get_seqno;
	}

	if (!I915_NEED_GFX_HWS(dev))
		ring->status_page.page_addr = dev_priv->status_page_dmah->vaddr;
=======
	int ret;

	ring->name = "render ring";
	ring->id = RCS;
	ring->mmio_base = RENDER_RING_BASE;

	if (INTEL_INFO(dev)->gen >= 6) {
		/* non-kms not supported on gen6+ */
		return -ENODEV;
	}

	/* Note: gem is not supported on gen5/ilk without kms (the corresponding
	 * gem_init ioctl returns with -ENODEV). Hence we do not need to set up
	 * the special gen5 functions. */
	ring->add_request = i9xx_add_request;
	if (INTEL_INFO(dev)->gen < 4)
		ring->flush = gen2_render_ring_flush;
	else
		ring->flush = gen4_render_ring_flush;
	ring->get_seqno = ring_get_seqno;
	ring->set_seqno = ring_set_seqno;
	if (IS_GEN2(dev)) {
		ring->irq_get = i8xx_ring_get_irq;
		ring->irq_put = i8xx_ring_put_irq;
	} else {
		ring->irq_get = i9xx_ring_get_irq;
		ring->irq_put = i9xx_ring_put_irq;
	}
	ring->irq_enable_mask = I915_USER_INTERRUPT;
	ring->write_tail = ring_write_tail;
	if (INTEL_INFO(dev)->gen >= 4)
		ring->dispatch_execbuffer = i965_dispatch_execbuffer;
	else if (IS_I830(dev) || IS_845G(dev))
		ring->dispatch_execbuffer = i830_dispatch_execbuffer;
	else
		ring->dispatch_execbuffer = i915_dispatch_execbuffer;
	ring->init = init_render_ring;
	ring->cleanup = render_ring_cleanup;
>>>>>>> refs/remotes/origin/master

	ring->dev = dev;
	INIT_LIST_HEAD(&ring->active_list);
	INIT_LIST_HEAD(&ring->request_list);
<<<<<<< HEAD
	INIT_LIST_HEAD(&ring->gpu_write_list);

	ring->size = size;
	ring->effective_size = ring->size;
	if (IS_I830(ring->dev))
		ring->effective_size -= 128;

	ring->map.offset = start;
	ring->map.size = size;
	ring->map.type = 0;
	ring->map.flags = 0;
	ring->map.mtrr = 0;

	drm_core_ioremap_wc(&ring->map, dev);
	if (ring->map.handle == NULL) {
=======

	ring->size = size;
	ring->effective_size = ring->size;
	if (IS_I830(ring->dev) || IS_845G(ring->dev))
		ring->effective_size -= 128;

	ring->virtual_start = ioremap_wc(start, size);
	if (ring->virtual_start == NULL) {
>>>>>>> refs/remotes/origin/master
		DRM_ERROR("can not ioremap virtual address for"
			  " ring buffer\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	ring->virtual_start = (void __force __iomem *)ring->map.handle;
=======
	if (!I915_NEED_GFX_HWS(dev)) {
		ret = init_phys_status_page(ring);
		if (ret)
			return ret;
	}

>>>>>>> refs/remotes/origin/master
	return 0;
}

int intel_init_bsd_ring_buffer(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_ring_buffer *ring = &dev_priv->ring[VCS];

<<<<<<< HEAD
	if (IS_GEN6(dev) || IS_GEN7(dev))
		*ring = gen6_bsd_ring;
	else
		*ring = bsd_ring;
=======
	ring->name = "bsd ring";
	ring->id = VCS;

	ring->write_tail = ring_write_tail;
	if (INTEL_INFO(dev)->gen >= 6) {
		ring->mmio_base = GEN6_BSD_RING_BASE;
		/* gen6 bsd needs a special wa for tail updates */
		if (IS_GEN6(dev))
			ring->write_tail = gen6_bsd_ring_write_tail;
		ring->flush = gen6_bsd_ring_flush;
		ring->add_request = gen6_add_request;
		ring->get_seqno = gen6_ring_get_seqno;
		ring->set_seqno = ring_set_seqno;
		if (INTEL_INFO(dev)->gen >= 8) {
			ring->irq_enable_mask =
				GT_RENDER_USER_INTERRUPT << GEN8_VCS1_IRQ_SHIFT;
			ring->irq_get = gen8_ring_get_irq;
			ring->irq_put = gen8_ring_put_irq;
			ring->dispatch_execbuffer =
				gen8_ring_dispatch_execbuffer;
		} else {
			ring->irq_enable_mask = GT_BSD_USER_INTERRUPT;
			ring->irq_get = gen6_ring_get_irq;
			ring->irq_put = gen6_ring_put_irq;
			ring->dispatch_execbuffer =
				gen6_ring_dispatch_execbuffer;
		}
		ring->sync_to = gen6_ring_sync;
		ring->semaphore_register[RCS] = MI_SEMAPHORE_SYNC_VR;
		ring->semaphore_register[VCS] = MI_SEMAPHORE_SYNC_INVALID;
		ring->semaphore_register[BCS] = MI_SEMAPHORE_SYNC_VB;
		ring->semaphore_register[VECS] = MI_SEMAPHORE_SYNC_VVE;
		ring->signal_mbox[RCS] = GEN6_RVSYNC;
		ring->signal_mbox[VCS] = GEN6_NOSYNC;
		ring->signal_mbox[BCS] = GEN6_BVSYNC;
		ring->signal_mbox[VECS] = GEN6_VEVSYNC;
	} else {
		ring->mmio_base = BSD_RING_BASE;
		ring->flush = bsd_ring_flush;
		ring->add_request = i9xx_add_request;
		ring->get_seqno = ring_get_seqno;
		ring->set_seqno = ring_set_seqno;
		if (IS_GEN5(dev)) {
			ring->irq_enable_mask = ILK_BSD_USER_INTERRUPT;
			ring->irq_get = gen5_ring_get_irq;
			ring->irq_put = gen5_ring_put_irq;
		} else {
			ring->irq_enable_mask = I915_BSD_USER_INTERRUPT;
			ring->irq_get = i9xx_ring_get_irq;
			ring->irq_put = i9xx_ring_put_irq;
		}
		ring->dispatch_execbuffer = i965_dispatch_execbuffer;
	}
	ring->init = init_ring_common;
>>>>>>> refs/remotes/origin/master

	return intel_init_ring_buffer(dev, ring);
}

int intel_init_blt_ring_buffer(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_ring_buffer *ring = &dev_priv->ring[BCS];

<<<<<<< HEAD
	*ring = gen6_blt_ring;

	return intel_init_ring_buffer(dev, ring);
}
=======
	ring->name = "blitter ring";
	ring->id = BCS;

	ring->mmio_base = BLT_RING_BASE;
	ring->write_tail = ring_write_tail;
	ring->flush = gen6_ring_flush;
	ring->add_request = gen6_add_request;
	ring->get_seqno = gen6_ring_get_seqno;
	ring->set_seqno = ring_set_seqno;
	if (INTEL_INFO(dev)->gen >= 8) {
		ring->irq_enable_mask =
			GT_RENDER_USER_INTERRUPT << GEN8_BCS_IRQ_SHIFT;
		ring->irq_get = gen8_ring_get_irq;
		ring->irq_put = gen8_ring_put_irq;
		ring->dispatch_execbuffer = gen8_ring_dispatch_execbuffer;
	} else {
		ring->irq_enable_mask = GT_BLT_USER_INTERRUPT;
		ring->irq_get = gen6_ring_get_irq;
		ring->irq_put = gen6_ring_put_irq;
		ring->dispatch_execbuffer = gen6_ring_dispatch_execbuffer;
	}
	ring->sync_to = gen6_ring_sync;
	ring->semaphore_register[RCS] = MI_SEMAPHORE_SYNC_BR;
	ring->semaphore_register[VCS] = MI_SEMAPHORE_SYNC_BV;
	ring->semaphore_register[BCS] = MI_SEMAPHORE_SYNC_INVALID;
	ring->semaphore_register[VECS] = MI_SEMAPHORE_SYNC_BVE;
	ring->signal_mbox[RCS] = GEN6_RBSYNC;
	ring->signal_mbox[VCS] = GEN6_VBSYNC;
	ring->signal_mbox[BCS] = GEN6_NOSYNC;
	ring->signal_mbox[VECS] = GEN6_VEBSYNC;
	ring->init = init_ring_common;

	return intel_init_ring_buffer(dev, ring);
}

int intel_init_vebox_ring_buffer(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_ring_buffer *ring = &dev_priv->ring[VECS];

	ring->name = "video enhancement ring";
	ring->id = VECS;

	ring->mmio_base = VEBOX_RING_BASE;
	ring->write_tail = ring_write_tail;
	ring->flush = gen6_ring_flush;
	ring->add_request = gen6_add_request;
	ring->get_seqno = gen6_ring_get_seqno;
	ring->set_seqno = ring_set_seqno;

	if (INTEL_INFO(dev)->gen >= 8) {
		ring->irq_enable_mask =
			GT_RENDER_USER_INTERRUPT << GEN8_VECS_IRQ_SHIFT;
		ring->irq_get = gen8_ring_get_irq;
		ring->irq_put = gen8_ring_put_irq;
		ring->dispatch_execbuffer = gen8_ring_dispatch_execbuffer;
	} else {
		ring->irq_enable_mask = PM_VEBOX_USER_INTERRUPT;
		ring->irq_get = hsw_vebox_get_irq;
		ring->irq_put = hsw_vebox_put_irq;
		ring->dispatch_execbuffer = gen6_ring_dispatch_execbuffer;
	}
	ring->sync_to = gen6_ring_sync;
	ring->semaphore_register[RCS] = MI_SEMAPHORE_SYNC_VER;
	ring->semaphore_register[VCS] = MI_SEMAPHORE_SYNC_VEV;
	ring->semaphore_register[BCS] = MI_SEMAPHORE_SYNC_VEB;
	ring->semaphore_register[VECS] = MI_SEMAPHORE_SYNC_INVALID;
	ring->signal_mbox[RCS] = GEN6_RVESYNC;
	ring->signal_mbox[VCS] = GEN6_VVESYNC;
	ring->signal_mbox[BCS] = GEN6_BVESYNC;
	ring->signal_mbox[VECS] = GEN6_NOSYNC;
	ring->init = init_ring_common;

	return intel_init_ring_buffer(dev, ring);
}

int
intel_ring_flush_all_caches(struct intel_ring_buffer *ring)
{
	int ret;

	if (!ring->gpu_caches_dirty)
		return 0;

	ret = ring->flush(ring, 0, I915_GEM_GPU_DOMAINS);
	if (ret)
		return ret;

	trace_i915_gem_ring_flush(ring, 0, I915_GEM_GPU_DOMAINS);

	ring->gpu_caches_dirty = false;
	return 0;
}

int
intel_ring_invalidate_all_caches(struct intel_ring_buffer *ring)
{
	uint32_t flush_domains;
	int ret;

	flush_domains = 0;
	if (ring->gpu_caches_dirty)
		flush_domains = I915_GEM_GPU_DOMAINS;

	ret = ring->flush(ring, I915_GEM_GPU_DOMAINS, flush_domains);
	if (ret)
		return ret;

	trace_i915_gem_ring_flush(ring, I915_GEM_GPU_DOMAINS, flush_domains);

	ring->gpu_caches_dirty = false;
	return 0;
}
>>>>>>> refs/remotes/origin/master
