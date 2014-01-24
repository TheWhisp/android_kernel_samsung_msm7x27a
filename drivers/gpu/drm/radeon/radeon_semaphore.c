/*
 * Copyright 2011 Christian König.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 */
/*
 * Authors:
 *    Christian König <deathsimple@vodafone.de>
 */
<<<<<<< HEAD
#include "drmP.h"
#include "drm.h"
#include "radeon.h"

static int radeon_semaphore_add_bo(struct radeon_device *rdev)
{
	struct radeon_semaphore_bo *bo;
	unsigned long irq_flags;
	uint64_t gpu_addr;
	uint32_t *cpu_ptr;
	int r, i;


	bo = kmalloc(sizeof(struct radeon_semaphore_bo), GFP_KERNEL);
	if (bo == NULL) {
		return -ENOMEM;
	}
	INIT_LIST_HEAD(&bo->free);
	INIT_LIST_HEAD(&bo->list);
	bo->nused = 0;

	r = radeon_ib_get(rdev, 0, &bo->ib, RADEON_SEMAPHORE_BO_SIZE);
	if (r) {
		dev_err(rdev->dev, "failed to get a bo after 5 retry\n");
		kfree(bo);
		return r;
	}
	gpu_addr = rdev->ib_pool.sa_manager.gpu_addr;
	gpu_addr += bo->ib->sa_bo.offset;
	cpu_ptr = rdev->ib_pool.sa_manager.cpu_ptr;
	cpu_ptr += (bo->ib->sa_bo.offset >> 2);
	for (i = 0; i < (RADEON_SEMAPHORE_BO_SIZE/8); i++) {
		bo->semaphores[i].gpu_addr = gpu_addr;
		bo->semaphores[i].cpu_ptr = cpu_ptr;
		bo->semaphores[i].bo = bo;
		list_add_tail(&bo->semaphores[i].list, &bo->free);
		gpu_addr += 8;
		cpu_ptr += 2;
	}
	write_lock_irqsave(&rdev->semaphore_drv.lock, irq_flags);
	list_add_tail(&bo->list, &rdev->semaphore_drv.bo);
	write_unlock_irqrestore(&rdev->semaphore_drv.lock, irq_flags);
	return 0;
}

static void radeon_semaphore_del_bo_locked(struct radeon_device *rdev,
					   struct radeon_semaphore_bo *bo)
{
	radeon_sa_bo_free(rdev, &bo->ib->sa_bo);
	radeon_fence_unref(&bo->ib->fence);
	list_del(&bo->list);
	kfree(bo);
}

void radeon_semaphore_shrink_locked(struct radeon_device *rdev)
{
	struct radeon_semaphore_bo *bo, *n;

	if (list_empty(&rdev->semaphore_drv.bo)) {
		return;
	}
	/* only shrink if first bo has free semaphore */
	bo = list_first_entry(&rdev->semaphore_drv.bo, struct radeon_semaphore_bo, list);
	if (list_empty(&bo->free)) {
		return;
	}
	list_for_each_entry_safe_continue(bo, n, &rdev->semaphore_drv.bo, list) {
		if (bo->nused)
			continue;
		radeon_semaphore_del_bo_locked(rdev, bo);
	}
}

int radeon_semaphore_create(struct radeon_device *rdev,
			    struct radeon_semaphore **semaphore)
{
	struct radeon_semaphore_bo *bo;
	unsigned long irq_flags;
	bool do_retry = true;
	int r;

retry:
	*semaphore = NULL;
	write_lock_irqsave(&rdev->semaphore_drv.lock, irq_flags);
	list_for_each_entry(bo, &rdev->semaphore_drv.bo, list) {
		if (list_empty(&bo->free))
			continue;
		*semaphore = list_first_entry(&bo->free, struct radeon_semaphore, list);
		(*semaphore)->cpu_ptr[0] = 0;
		(*semaphore)->cpu_ptr[1] = 0;
		list_del(&(*semaphore)->list);
		bo->nused++;
		break;
	}
	write_unlock_irqrestore(&rdev->semaphore_drv.lock, irq_flags);

	if (*semaphore == NULL) {
		if (do_retry) {
			do_retry = false;
			r = radeon_semaphore_add_bo(rdev);
			if (r)
				return r;
			goto retry;
		}
		return -ENOMEM;
	}

	return 0;
}

void radeon_semaphore_emit_signal(struct radeon_device *rdev, int ring,
			          struct radeon_semaphore *semaphore)
{
	radeon_semaphore_ring_emit(rdev, ring, &rdev->ring[ring], semaphore, false);
}

void radeon_semaphore_emit_wait(struct radeon_device *rdev, int ring,
			        struct radeon_semaphore *semaphore)
{
	radeon_semaphore_ring_emit(rdev, ring, &rdev->ring[ring], semaphore, true);
}

void radeon_semaphore_free(struct radeon_device *rdev,
			   struct radeon_semaphore *semaphore)
{
	unsigned long irq_flags;

	write_lock_irqsave(&rdev->semaphore_drv.lock, irq_flags);
	semaphore->bo->nused--;
	list_add_tail(&semaphore->list, &semaphore->bo->free);
	radeon_semaphore_shrink_locked(rdev);
	write_unlock_irqrestore(&rdev->semaphore_drv.lock, irq_flags);
}

void radeon_semaphore_driver_fini(struct radeon_device *rdev)
{
	struct radeon_semaphore_bo *bo, *n;
	unsigned long irq_flags;

	write_lock_irqsave(&rdev->semaphore_drv.lock, irq_flags);
	/* we force to free everything */
	list_for_each_entry_safe(bo, n, &rdev->semaphore_drv.bo, list) {
		if (!list_empty(&bo->free)) {
			dev_err(rdev->dev, "still in use semaphore\n");
		}
		radeon_semaphore_del_bo_locked(rdev, bo);
	}
	write_unlock_irqrestore(&rdev->semaphore_drv.lock, irq_flags);
=======
#include <drm/drmP.h>
#include "radeon.h"
#include "radeon_trace.h"

int radeon_semaphore_create(struct radeon_device *rdev,
			    struct radeon_semaphore **semaphore)
{
	int i, r;

	*semaphore = kmalloc(sizeof(struct radeon_semaphore), GFP_KERNEL);
	if (*semaphore == NULL) {
		return -ENOMEM;
	}
	r = radeon_sa_bo_new(rdev, &rdev->ring_tmp_bo,
			     &(*semaphore)->sa_bo, 8, 8, true);
	if (r) {
		kfree(*semaphore);
		*semaphore = NULL;
		return r;
	}
	(*semaphore)->waiters = 0;
	(*semaphore)->gpu_addr = radeon_sa_bo_gpu_addr((*semaphore)->sa_bo);
	*((uint64_t*)radeon_sa_bo_cpu_addr((*semaphore)->sa_bo)) = 0;

	for (i = 0; i < RADEON_NUM_RINGS; ++i)
		(*semaphore)->sync_to[i] = NULL;

	return 0;
}

bool radeon_semaphore_emit_signal(struct radeon_device *rdev, int ridx,
			          struct radeon_semaphore *semaphore)
{
	struct radeon_ring *ring = &rdev->ring[ridx];

	trace_radeon_semaphore_signale(ridx, semaphore);

	if (radeon_semaphore_ring_emit(rdev, ridx, ring, semaphore, false)) {
		--semaphore->waiters;

		/* for debugging lockup only, used by sysfs debug files */
		ring->last_semaphore_signal_addr = semaphore->gpu_addr;
		return true;
	}
	return false;
}

bool radeon_semaphore_emit_wait(struct radeon_device *rdev, int ridx,
			        struct radeon_semaphore *semaphore)
{
	struct radeon_ring *ring = &rdev->ring[ridx];

	trace_radeon_semaphore_wait(ridx, semaphore);

	if (radeon_semaphore_ring_emit(rdev, ridx, ring, semaphore, true)) {
		++semaphore->waiters;

		/* for debugging lockup only, used by sysfs debug files */
		ring->last_semaphore_wait_addr = semaphore->gpu_addr;
		return true;
	}
	return false;
}

/**
 * radeon_semaphore_sync_to - use the semaphore to sync to a fence
 *
 * @semaphore: semaphore object to add fence to
 * @fence: fence to sync to
 *
 * Sync to the fence using this semaphore object
 */
void radeon_semaphore_sync_to(struct radeon_semaphore *semaphore,
			      struct radeon_fence *fence)
{
        struct radeon_fence *other;

        if (!fence)
                return;

        other = semaphore->sync_to[fence->ring];
        semaphore->sync_to[fence->ring] = radeon_fence_later(fence, other);
}

/**
 * radeon_semaphore_sync_rings - sync ring to all registered fences
 *
 * @rdev: radeon_device pointer
 * @semaphore: semaphore object to use for sync
 * @ring: ring that needs sync
 *
 * Ensure that all registered fences are signaled before letting
 * the ring continue. The caller must hold the ring lock.
 */
int radeon_semaphore_sync_rings(struct radeon_device *rdev,
				struct radeon_semaphore *semaphore,
				int ring)
{
	int i, r;

        for (i = 0; i < RADEON_NUM_RINGS; ++i) {
		struct radeon_fence *fence = semaphore->sync_to[i];

		/* check if we really need to sync */
                if (!radeon_fence_need_sync(fence, ring))
			continue;

		/* prevent GPU deadlocks */
		if (!rdev->ring[i].ready) {
			dev_err(rdev->dev, "Syncing to a disabled ring!");
			return -EINVAL;
		}

		/* allocate enough space for sync command */
		r = radeon_ring_alloc(rdev, &rdev->ring[i], 16);
		if (r) {
			return r;
		}

		/* emit the signal semaphore */
		if (!radeon_semaphore_emit_signal(rdev, i, semaphore)) {
			/* signaling wasn't successful wait manually */
			radeon_ring_undo(&rdev->ring[i]);
			radeon_fence_wait_locked(fence);
			continue;
		}

		/* we assume caller has already allocated space on waiters ring */
		if (!radeon_semaphore_emit_wait(rdev, ring, semaphore)) {
			/* waiting wasn't successful wait manually */
			radeon_ring_undo(&rdev->ring[i]);
			radeon_fence_wait_locked(fence);
			continue;
		}

		radeon_ring_commit(rdev, &rdev->ring[i]);
		radeon_fence_note_sync(fence, ring);
	}

	return 0;
}

void radeon_semaphore_free(struct radeon_device *rdev,
			   struct radeon_semaphore **semaphore,
			   struct radeon_fence *fence)
{
	if (semaphore == NULL || *semaphore == NULL) {
		return;
	}
	if ((*semaphore)->waiters > 0) {
		dev_err(rdev->dev, "semaphore %p has more waiters than signalers,"
			" hardware lockup imminent!\n", *semaphore);
	}
	radeon_sa_bo_free(rdev, &(*semaphore)->sa_bo, fence);
	kfree(*semaphore);
	*semaphore = NULL;
>>>>>>> refs/remotes/origin/master
}
