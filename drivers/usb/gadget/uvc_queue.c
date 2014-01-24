/*
 *	uvc_queue.c  --  USB Video Class driver - Buffers management
 *
 *	Copyright (C) 2005-2010
 *	    Laurent Pinchart (laurent.pinchart@ideasonboard.com)
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
<<<<<<< HEAD
<<<<<<< HEAD
 *
=======
>>>>>>> refs/remotes/origin/cm-10.0
 */

=======
 */

#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/master
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/videodev2.h>
#include <linux/vmalloc.h>
#include <linux/wait.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======

#include <media/videobuf2-vmalloc.h>
>>>>>>> refs/remotes/origin/master

#include "uvc.h"

/* ------------------------------------------------------------------------
 * Video buffers queue management.
 *
 * Video queues is initialized by uvc_queue_init(). The function performs
 * basic initialization of the uvc_video_queue struct and never fails.
 *
<<<<<<< HEAD
 * Video buffer allocation and freeing are performed by uvc_alloc_buffers and
 * uvc_free_buffers respectively. The former acquires the video queue lock,
 * while the later must be called with the lock held (so that allocation can
 * free previously allocated buffers). Trying to free buffers that are mapped
 * to user space will return -EBUSY.
 *
 * Video buffers are managed using two queues. However, unlike most USB video
 * drivers that use an in queue and an out queue, we use a main queue to hold
 * all queued buffers (both 'empty' and 'done' buffers), and an irq queue to
 * hold empty buffers. This design (copied from video-buf) minimizes locking
 * in interrupt, as only one queue is shared between interrupt and user
 * contexts.
 *
 * Use cases
 * ---------
 *
 * Unless stated otherwise, all operations that modify the irq buffers queue
 * are protected by the irq spinlock.
 *
 * 1. The user queues the buffers, starts streaming and dequeues a buffer.
 *
 *    The buffers are added to the main and irq queues. Both operations are
 *    protected by the queue lock, and the later is protected by the irq
 *    spinlock as well.
 *
 *    The completion handler fetches a buffer from the irq queue and fills it
 *    with video data. If no buffer is available (irq queue empty), the handler
 *    returns immediately.
 *
 *    When the buffer is full, the completion handler removes it from the irq
 *    queue, marks it as ready (UVC_BUF_STATE_DONE) and wakes its wait queue.
 *    At that point, any process waiting on the buffer will be woken up. If a
 *    process tries to dequeue a buffer after it has been marked ready, the
 *    dequeing will succeed immediately.
 *
 * 2. Buffers are queued, user is waiting on a buffer and the device gets
 *    disconnected.
 *
 *    When the device is disconnected, the kernel calls the completion handler
 *    with an appropriate status code. The handler marks all buffers in the
 *    irq queue as being erroneous (UVC_BUF_STATE_ERROR) and wakes them up so
 *    that any process waiting on a buffer gets woken up.
 *
 *    Waking up up the first buffer on the irq list is not enough, as the
 *    process waiting on the buffer might restart the dequeue operation
 *    immediately.
 *
 */

static void
uvc_queue_init(struct uvc_video_queue *queue, enum v4l2_buf_type type)
{
	mutex_init(&queue->mutex);
	spin_lock_init(&queue->irqlock);
	INIT_LIST_HEAD(&queue->mainqueue);
	INIT_LIST_HEAD(&queue->irqqueue);
	queue->type = type;
}

/*
 * Free the video buffers.
 *
 * This function must be called with the queue lock held.
 */
static int uvc_free_buffers(struct uvc_video_queue *queue)
{
	unsigned int i;

	for (i = 0; i < queue->count; ++i) {
		if (queue->buffer[i].vma_use_count != 0)
			return -EBUSY;
	}

	if (queue->count) {
		vfree(queue->mem);
		queue->count = 0;
	}
=======
 * Video buffers are managed by videobuf2. The driver uses a mutex to protect
 * the videobuf2 queue operations by serializing calls to videobuf2 and a
 * spinlock to protect the IRQ queue that holds the buffers to be processed by
 * the driver.
 */

/* -----------------------------------------------------------------------------
 * videobuf2 queue operations
 */

static int uvc_queue_setup(struct vb2_queue *vq, const struct v4l2_format *fmt,
			   unsigned int *nbuffers, unsigned int *nplanes,
			   unsigned int sizes[], void *alloc_ctxs[])
{
	struct uvc_video_queue *queue = vb2_get_drv_priv(vq);
	struct uvc_video *video = container_of(queue, struct uvc_video, queue);

	if (*nbuffers > UVC_MAX_VIDEO_BUFFERS)
		*nbuffers = UVC_MAX_VIDEO_BUFFERS;

	*nplanes = 1;

	sizes[0] = video->imagesize;
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
/*
 * Allocate the video buffers.
 *
 * Pages are reserved to make sure they will not be swapped, as they will be
 * filled in the URB completion handler.
 *
 * Buffers will be individually mapped, so they must all be page aligned.
 */
static int
uvc_alloc_buffers(struct uvc_video_queue *queue, unsigned int nbuffers,
		  unsigned int buflength)
{
	unsigned int bufsize = PAGE_ALIGN(buflength);
	unsigned int i;
	void *mem = NULL;
	int ret;

	if (nbuffers > UVC_MAX_VIDEO_BUFFERS)
		nbuffers = UVC_MAX_VIDEO_BUFFERS;

	mutex_lock(&queue->mutex);

	if ((ret = uvc_free_buffers(queue)) < 0)
		goto done;

	/* Bail out if no buffers should be allocated. */
	if (nbuffers == 0)
		goto done;

	/* Decrement the number of buffers until allocation succeeds. */
	for (; nbuffers > 0; --nbuffers) {
		mem = vmalloc_32(nbuffers * bufsize);
		if (mem != NULL)
			break;
	}

	if (mem == NULL) {
		ret = -ENOMEM;
		goto done;
	}

	for (i = 0; i < nbuffers; ++i) {
		memset(&queue->buffer[i], 0, sizeof queue->buffer[i]);
		queue->buffer[i].buf.index = i;
		queue->buffer[i].buf.m.offset = i * bufsize;
		queue->buffer[i].buf.length = buflength;
		queue->buffer[i].buf.type = queue->type;
		queue->buffer[i].buf.sequence = 0;
		queue->buffer[i].buf.field = V4L2_FIELD_NONE;
		queue->buffer[i].buf.memory = V4L2_MEMORY_MMAP;
		queue->buffer[i].buf.flags = 0;
		init_waitqueue_head(&queue->buffer[i].wait);
	}

	queue->mem = mem;
	queue->count = nbuffers;
	queue->buf_size = bufsize;
	ret = nbuffers;

done:
	mutex_unlock(&queue->mutex);
	return ret;
}

static void __uvc_query_buffer(struct uvc_buffer *buf,
		struct v4l2_buffer *v4l2_buf)
{
	memcpy(v4l2_buf, &buf->buf, sizeof *v4l2_buf);

	if (buf->vma_use_count)
		v4l2_buf->flags |= V4L2_BUF_FLAG_MAPPED;

	switch (buf->state) {
	case UVC_BUF_STATE_ERROR:
	case UVC_BUF_STATE_DONE:
		v4l2_buf->flags |= V4L2_BUF_FLAG_DONE;
		break;
	case UVC_BUF_STATE_QUEUED:
	case UVC_BUF_STATE_ACTIVE:
		v4l2_buf->flags |= V4L2_BUF_FLAG_QUEUED;
		break;
	case UVC_BUF_STATE_IDLE:
	default:
		break;
	}
}

static int
uvc_query_buffer(struct uvc_video_queue *queue, struct v4l2_buffer *v4l2_buf)
{
	int ret = 0;

	mutex_lock(&queue->mutex);
	if (v4l2_buf->index >= queue->count) {
		ret = -EINVAL;
		goto done;
	}

	__uvc_query_buffer(&queue->buffer[v4l2_buf->index], v4l2_buf);

done:
	mutex_unlock(&queue->mutex);
	return ret;
}

/*
 * Queue a video buffer. Attempting to queue a buffer that has already been
 * queued will return -EINVAL.
 */
static int
uvc_queue_buffer(struct uvc_video_queue *queue, struct v4l2_buffer *v4l2_buf)
{
	struct uvc_buffer *buf;
	unsigned long flags;
	int ret = 0;

	uvc_trace(UVC_TRACE_CAPTURE, "Queuing buffer %u.\n", v4l2_buf->index);

	if (v4l2_buf->type != queue->type ||
	    v4l2_buf->memory != V4L2_MEMORY_MMAP) {
		uvc_trace(UVC_TRACE_CAPTURE, "[E] Invalid buffer type (%u) "
			"and/or memory (%u).\n", v4l2_buf->type,
			v4l2_buf->memory);
		return -EINVAL;
	}

	mutex_lock(&queue->mutex);
	if (v4l2_buf->index >= queue->count) {
		uvc_trace(UVC_TRACE_CAPTURE, "[E] Out of range index.\n");
		ret = -EINVAL;
		goto done;
	}

	buf = &queue->buffer[v4l2_buf->index];
	if (buf->state != UVC_BUF_STATE_IDLE) {
		uvc_trace(UVC_TRACE_CAPTURE, "[E] Invalid buffer state "
			"(%u).\n", buf->state);
		ret = -EINVAL;
		goto done;
	}

	if (v4l2_buf->type == V4L2_BUF_TYPE_VIDEO_OUTPUT &&
	    v4l2_buf->bytesused > buf->buf.length) {
		uvc_trace(UVC_TRACE_CAPTURE, "[E] Bytes used out of bounds.\n");
		ret = -EINVAL;
		goto done;
	}

	if (v4l2_buf->type == V4L2_BUF_TYPE_VIDEO_CAPTURE)
		buf->buf.bytesused = 0;
	else
		buf->buf.bytesused = v4l2_buf->bytesused;

	spin_lock_irqsave(&queue->irqlock, flags);
	if (queue->flags & UVC_QUEUE_DISCONNECTED) {
		spin_unlock_irqrestore(&queue->irqlock, flags);
		ret = -ENODEV;
		goto done;
	}
	buf->state = UVC_BUF_STATE_QUEUED;

	ret = (queue->flags & UVC_QUEUE_PAUSED) != 0;
	queue->flags &= ~UVC_QUEUE_PAUSED;

	list_add_tail(&buf->stream, &queue->mainqueue);
	list_add_tail(&buf->queue, &queue->irqqueue);
=======
static int uvc_buffer_prepare(struct vb2_buffer *vb)
{
	struct uvc_video_queue *queue = vb2_get_drv_priv(vb->vb2_queue);
	struct uvc_buffer *buf = container_of(vb, struct uvc_buffer, buf);

	if (vb->v4l2_buf.type == V4L2_BUF_TYPE_VIDEO_OUTPUT &&
	    vb2_get_plane_payload(vb, 0) > vb2_plane_size(vb, 0)) {
		uvc_trace(UVC_TRACE_CAPTURE, "[E] Bytes used out of bounds.\n");
		return -EINVAL;
	}

	if (unlikely(queue->flags & UVC_QUEUE_DISCONNECTED))
		return -ENODEV;

	buf->state = UVC_BUF_STATE_QUEUED;
	buf->mem = vb2_plane_vaddr(vb, 0);
	buf->length = vb2_plane_size(vb, 0);
	if (vb->v4l2_buf.type == V4L2_BUF_TYPE_VIDEO_CAPTURE)
		buf->bytesused = 0;
	else
		buf->bytesused = vb2_get_plane_payload(vb, 0);

	return 0;
}

static void uvc_buffer_queue(struct vb2_buffer *vb)
{
	struct uvc_video_queue *queue = vb2_get_drv_priv(vb->vb2_queue);
	struct uvc_buffer *buf = container_of(vb, struct uvc_buffer, buf);
	unsigned long flags;

	spin_lock_irqsave(&queue->irqlock, flags);

	if (likely(!(queue->flags & UVC_QUEUE_DISCONNECTED))) {
		list_add_tail(&buf->queue, &queue->irqqueue);
	} else {
		/* If the device is disconnected return the buffer to userspace
		 * directly. The next QBUF call will fail with -ENODEV.
		 */
		buf->state = UVC_BUF_STATE_ERROR;
		vb2_buffer_done(&buf->buf, VB2_BUF_STATE_ERROR);
	}

	spin_unlock_irqrestore(&queue->irqlock, flags);
}

static void uvc_wait_prepare(struct vb2_queue *vq)
{
	struct uvc_video_queue *queue = vb2_get_drv_priv(vq);

	mutex_unlock(&queue->mutex);
}

static void uvc_wait_finish(struct vb2_queue *vq)
{
	struct uvc_video_queue *queue = vb2_get_drv_priv(vq);

	mutex_lock(&queue->mutex);
}

static struct vb2_ops uvc_queue_qops = {
	.queue_setup = uvc_queue_setup,
	.buf_prepare = uvc_buffer_prepare,
	.buf_queue = uvc_buffer_queue,
	.wait_prepare = uvc_wait_prepare,
	.wait_finish = uvc_wait_finish,
};

static int uvc_queue_init(struct uvc_video_queue *queue,
			  enum v4l2_buf_type type)
{
	int ret;

	queue->queue.type = type;
	queue->queue.io_modes = VB2_MMAP | VB2_USERPTR;
	queue->queue.drv_priv = queue;
	queue->queue.buf_struct_size = sizeof(struct uvc_buffer);
	queue->queue.ops = &uvc_queue_qops;
	queue->queue.mem_ops = &vb2_vmalloc_memops;
	ret = vb2_queue_init(&queue->queue);
	if (ret)
		return ret;

	mutex_init(&queue->mutex);
	spin_lock_init(&queue->irqlock);
	INIT_LIST_HEAD(&queue->irqqueue);
	queue->flags = 0;

	return 0;
}

/*
 * Free the video buffers.
 */
static void uvc_free_buffers(struct uvc_video_queue *queue)
{
	mutex_lock(&queue->mutex);
	vb2_queue_release(&queue->queue);
	mutex_unlock(&queue->mutex);
}

/*
 * Allocate the video buffers.
 */
static int uvc_alloc_buffers(struct uvc_video_queue *queue,
			     struct v4l2_requestbuffers *rb)
{
	int ret;

	mutex_lock(&queue->mutex);
	ret = vb2_reqbufs(&queue->queue, rb);
	mutex_unlock(&queue->mutex);

	return ret ? ret : rb->count;
}

static int uvc_query_buffer(struct uvc_video_queue *queue,
			    struct v4l2_buffer *buf)
{
	int ret;

	mutex_lock(&queue->mutex);
	ret = vb2_querybuf(&queue->queue, buf);
	mutex_unlock(&queue->mutex);

	return ret;
}

static int uvc_queue_buffer(struct uvc_video_queue *queue,
			    struct v4l2_buffer *buf)
{
	unsigned long flags;
	int ret;

	mutex_lock(&queue->mutex);
	ret = vb2_qbuf(&queue->queue, buf);
	if (ret < 0)
		goto done;

	spin_lock_irqsave(&queue->irqlock, flags);
	ret = (queue->flags & UVC_QUEUE_PAUSED) != 0;
	queue->flags &= ~UVC_QUEUE_PAUSED;
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&queue->irqlock, flags);

done:
	mutex_unlock(&queue->mutex);
	return ret;
}

<<<<<<< HEAD
static int uvc_queue_waiton(struct uvc_buffer *buf, int nonblocking)
{
	if (nonblocking) {
		return (buf->state != UVC_BUF_STATE_QUEUED &&
			buf->state != UVC_BUF_STATE_ACTIVE)
			? 0 : -EAGAIN;
	}

	return wait_event_interruptible(buf->wait,
		buf->state != UVC_BUF_STATE_QUEUED &&
		buf->state != UVC_BUF_STATE_ACTIVE);
}

=======
>>>>>>> refs/remotes/origin/master
/*
 * Dequeue a video buffer. If nonblocking is false, block until a buffer is
 * available.
 */
<<<<<<< HEAD
static int
uvc_dequeue_buffer(struct uvc_video_queue *queue, struct v4l2_buffer *v4l2_buf,
		   int nonblocking)
{
	struct uvc_buffer *buf;
	int ret = 0;

	if (v4l2_buf->type != queue->type ||
	    v4l2_buf->memory != V4L2_MEMORY_MMAP) {
		uvc_trace(UVC_TRACE_CAPTURE, "[E] Invalid buffer type (%u) "
			"and/or memory (%u).\n", v4l2_buf->type,
			v4l2_buf->memory);
		return -EINVAL;
	}

	mutex_lock(&queue->mutex);
	if (list_empty(&queue->mainqueue)) {
		uvc_trace(UVC_TRACE_CAPTURE, "[E] Empty buffer queue.\n");
		ret = -EINVAL;
		goto done;
	}

	buf = list_first_entry(&queue->mainqueue, struct uvc_buffer, stream);
	if ((ret = uvc_queue_waiton(buf, nonblocking)) < 0)
		goto done;

	uvc_trace(UVC_TRACE_CAPTURE, "Dequeuing buffer %u (%u, %u bytes).\n",
		buf->buf.index, buf->state, buf->buf.bytesused);

	switch (buf->state) {
	case UVC_BUF_STATE_ERROR:
		uvc_trace(UVC_TRACE_CAPTURE, "[W] Corrupted data "
			"(transmission error).\n");
		ret = -EIO;
	case UVC_BUF_STATE_DONE:
		buf->state = UVC_BUF_STATE_IDLE;
		break;

	case UVC_BUF_STATE_IDLE:
	case UVC_BUF_STATE_QUEUED:
	case UVC_BUF_STATE_ACTIVE:
	default:
		uvc_trace(UVC_TRACE_CAPTURE, "[E] Invalid buffer state %u "
			"(driver bug?).\n", buf->state);
		ret = -EINVAL;
		goto done;
	}

	list_del(&buf->stream);
	__uvc_query_buffer(buf, v4l2_buf);

done:
	mutex_unlock(&queue->mutex);
=======
static int uvc_dequeue_buffer(struct uvc_video_queue *queue,
			      struct v4l2_buffer *buf, int nonblocking)
{
	int ret;

	mutex_lock(&queue->mutex);
	ret = vb2_dqbuf(&queue->queue, buf, nonblocking);
	mutex_unlock(&queue->mutex);

>>>>>>> refs/remotes/origin/master
	return ret;
}

/*
 * Poll the video queue.
 *
 * This function implements video queue polling and is intended to be used by
 * the device poll handler.
 */
<<<<<<< HEAD
static unsigned int
uvc_queue_poll(struct uvc_video_queue *queue, struct file *file,
	       poll_table *wait)
{
	struct uvc_buffer *buf;
	unsigned int mask = 0;

	mutex_lock(&queue->mutex);
	if (list_empty(&queue->mainqueue))
		goto done;

	buf = list_first_entry(&queue->mainqueue, struct uvc_buffer, stream);

	poll_wait(file, &buf->wait, wait);
	if (buf->state == UVC_BUF_STATE_DONE ||
	    buf->state == UVC_BUF_STATE_ERROR)
		mask |= POLLOUT | POLLWRNORM;

done:
	mutex_unlock(&queue->mutex);
	return mask;
}

/*
 * VMA operations.
 */
static void uvc_vm_open(struct vm_area_struct *vma)
{
	struct uvc_buffer *buffer = vma->vm_private_data;
	buffer->vma_use_count++;
}

static void uvc_vm_close(struct vm_area_struct *vma)
{
	struct uvc_buffer *buffer = vma->vm_private_data;
	buffer->vma_use_count--;
}

static struct vm_operations_struct uvc_vm_ops = {
	.open		= uvc_vm_open,
	.close		= uvc_vm_close,
};

/*
 * Memory-map a buffer.
 *
 * This function implements video buffer memory mapping and is intended to be
 * used by the device mmap handler.
 */
static int
uvc_queue_mmap(struct uvc_video_queue *queue, struct vm_area_struct *vma)
{
	struct uvc_buffer *uninitialized_var(buffer);
	struct page *page;
	unsigned long addr, start, size;
	unsigned int i;
	int ret = 0;

	start = vma->vm_start;
	size = vma->vm_end - vma->vm_start;

	mutex_lock(&queue->mutex);

	for (i = 0; i < queue->count; ++i) {
		buffer = &queue->buffer[i];
		if ((buffer->buf.m.offset >> PAGE_SHIFT) == vma->vm_pgoff)
			break;
	}

	if (i == queue->count || size != queue->buf_size) {
		ret = -EINVAL;
		goto done;
	}

	/*
	 * VM_IO marks the area as being an mmaped region for I/O to a
	 * device. It also prevents the region from being core dumped.
	 */
	vma->vm_flags |= VM_IO;

	addr = (unsigned long)queue->mem + buffer->buf.m.offset;
	while (size > 0) {
		page = vmalloc_to_page((void *)addr);
		if ((ret = vm_insert_page(vma, start, page)) < 0)
			goto done;

		start += PAGE_SIZE;
		addr += PAGE_SIZE;
		size -= PAGE_SIZE;
	}

	vma->vm_ops = &uvc_vm_ops;
	vma->vm_private_data = buffer;
	uvc_vm_open(vma);

done:
	mutex_unlock(&queue->mutex);
	return ret;
}
=======
static unsigned int uvc_queue_poll(struct uvc_video_queue *queue,
				   struct file *file, poll_table *wait)
{
	unsigned int ret;

	mutex_lock(&queue->mutex);
	ret = vb2_poll(&queue->queue, file, wait);
	mutex_unlock(&queue->mutex);

	return ret;
}

static int uvc_queue_mmap(struct uvc_video_queue *queue,
			  struct vm_area_struct *vma)
{
	int ret;

	mutex_lock(&queue->mutex);
	ret = vb2_mmap(&queue->queue, vma);
	mutex_unlock(&queue->mutex);

	return ret;
}

#ifndef CONFIG_MMU
/*
 * Get unmapped area.
 *
 * NO-MMU arch need this function to make mmap() work correctly.
 */
static unsigned long uvc_queue_get_unmapped_area(struct uvc_video_queue *queue,
		unsigned long pgoff)
{
	unsigned long ret;

	mutex_lock(&queue->mutex);
	ret = vb2_get_unmapped_area(&queue->queue, 0, 0, pgoff, 0);
	mutex_unlock(&queue->mutex);
	return ret;
}
#endif
>>>>>>> refs/remotes/origin/master

/*
 * Cancel the video buffers queue.
 *
 * Cancelling the queue marks all buffers on the irq queue as erroneous,
 * wakes them up and removes them from the queue.
 *
 * If the disconnect parameter is set, further calls to uvc_queue_buffer will
 * fail with -ENODEV.
 *
 * This function acquires the irq spinlock and can be called from interrupt
 * context.
 */
static void uvc_queue_cancel(struct uvc_video_queue *queue, int disconnect)
{
	struct uvc_buffer *buf;
	unsigned long flags;

	spin_lock_irqsave(&queue->irqlock, flags);
	while (!list_empty(&queue->irqqueue)) {
		buf = list_first_entry(&queue->irqqueue, struct uvc_buffer,
				       queue);
		list_del(&buf->queue);
		buf->state = UVC_BUF_STATE_ERROR;
<<<<<<< HEAD
		wake_up(&buf->wait);
=======
		vb2_buffer_done(&buf->buf, VB2_BUF_STATE_ERROR);
>>>>>>> refs/remotes/origin/master
	}
	/* This must be protected by the irqlock spinlock to avoid race
	 * conditions between uvc_queue_buffer and the disconnection event that
	 * could result in an interruptible wait in uvc_dequeue_buffer. Do not
	 * blindly replace this logic by checking for the UVC_DEV_DISCONNECTED
	 * state outside the queue code.
	 */
	if (disconnect)
		queue->flags |= UVC_QUEUE_DISCONNECTED;
	spin_unlock_irqrestore(&queue->irqlock, flags);
}

/*
 * Enable or disable the video buffers queue.
 *
 * The queue must be enabled before starting video acquisition and must be
 * disabled after stopping it. This ensures that the video buffers queue
 * state can be properly initialized before buffers are accessed from the
 * interrupt handler.
 *
 * Enabling the video queue initializes parameters (such as sequence number,
 * sync pattern, ...). If the queue is already enabled, return -EBUSY.
 *
 * Disabling the video queue cancels the queue and removes all buffers from
 * the main queue.
 *
 * This function can't be called from interrupt context. Use
 * uvc_queue_cancel() instead.
 */
static int uvc_queue_enable(struct uvc_video_queue *queue, int enable)
{
<<<<<<< HEAD
	unsigned int i;
=======
	unsigned long flags;
>>>>>>> refs/remotes/origin/master
	int ret = 0;

	mutex_lock(&queue->mutex);
	if (enable) {
<<<<<<< HEAD
		if (uvc_queue_streaming(queue)) {
			ret = -EBUSY;
			goto done;
		}
		queue->sequence = 0;
		queue->flags |= UVC_QUEUE_STREAMING;
		queue->buf_used = 0;
	} else {
		uvc_queue_cancel(queue, 0);
		INIT_LIST_HEAD(&queue->mainqueue);

		for (i = 0; i < queue->count; ++i)
			queue->buffer[i].state = UVC_BUF_STATE_IDLE;

		queue->flags &= ~UVC_QUEUE_STREAMING;
=======
		ret = vb2_streamon(&queue->queue, queue->queue.type);
		if (ret < 0)
			goto done;

		queue->sequence = 0;
		queue->buf_used = 0;
	} else {
		ret = vb2_streamoff(&queue->queue, queue->queue.type);
		if (ret < 0)
			goto done;

		spin_lock_irqsave(&queue->irqlock, flags);
		INIT_LIST_HEAD(&queue->irqqueue);

		/*
		 * FIXME: We need to clear the DISCONNECTED flag to ensure that
		 * applications will be able to queue buffers for the next
		 * streaming run. However, clearing it here doesn't guarantee
		 * that the device will be reconnected in the meantime.
		 */
		queue->flags &= ~UVC_QUEUE_DISCONNECTED;
		spin_unlock_irqrestore(&queue->irqlock, flags);
>>>>>>> refs/remotes/origin/master
	}

done:
	mutex_unlock(&queue->mutex);
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
/* called with queue->irqlock held.. */
>>>>>>> refs/remotes/origin/cm-10.0
static struct uvc_buffer *
uvc_queue_next_buffer(struct uvc_video_queue *queue, struct uvc_buffer *buf)
{
	struct uvc_buffer *nextbuf;
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> refs/remotes/origin/cm-10.0

	if ((queue->flags & UVC_QUEUE_DROP_INCOMPLETE) &&
	    buf->buf.length != buf->buf.bytesused) {
		buf->state = UVC_BUF_STATE_QUEUED;
		buf->buf.bytesused = 0;
		return buf;
	}

<<<<<<< HEAD
	spin_lock_irqsave(&queue->irqlock, flags);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* called with &queue_irqlock held.. */
static struct uvc_buffer *uvc_queue_next_buffer(struct uvc_video_queue *queue,
						struct uvc_buffer *buf)
{
	struct uvc_buffer *nextbuf;

	if ((queue->flags & UVC_QUEUE_DROP_INCOMPLETE) &&
	     buf->length != buf->bytesused) {
		buf->state = UVC_BUF_STATE_QUEUED;
		vb2_set_plane_payload(&buf->buf, 0, 0);
		return buf;
	}

>>>>>>> refs/remotes/origin/master
	list_del(&buf->queue);
	if (!list_empty(&queue->irqqueue))
		nextbuf = list_first_entry(&queue->irqqueue, struct uvc_buffer,
					   queue);
	else
		nextbuf = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock_irqrestore(&queue->irqlock, flags);
=======
>>>>>>> refs/remotes/origin/cm-10.0

	buf->buf.sequence = queue->sequence++;
	do_gettimeofday(&buf->buf.timestamp);

	wake_up(&buf->wait);
=======

	/*
	 * FIXME: with videobuf2, the sequence number or timestamp fields
	 * are valid only for video capture devices and the UVC gadget usually
	 * is a video output device. Keeping these until the specs are clear on
	 * this aspect.
	 */
	buf->buf.v4l2_buf.sequence = queue->sequence++;
	do_gettimeofday(&buf->buf.v4l2_buf.timestamp);

	vb2_set_plane_payload(&buf->buf, 0, buf->bytesused);
	vb2_buffer_done(&buf->buf, VB2_BUF_STATE_DONE);

>>>>>>> refs/remotes/origin/master
	return nextbuf;
}

static struct uvc_buffer *uvc_queue_head(struct uvc_video_queue *queue)
{
	struct uvc_buffer *buf = NULL;

	if (!list_empty(&queue->irqqueue))
		buf = list_first_entry(&queue->irqqueue, struct uvc_buffer,
				       queue);
	else
		queue->flags |= UVC_QUEUE_PAUSED;

	return buf;
}

