/*
 * Samsung S5P/EXYNOS4 SoC series camera interface (camera capture) driver
 *
 * Copyright (C) 2010 - 2011 Samsung Electronics Co., Ltd.
 * Author: Sylwester Nawrocki, <s.nawrocki@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/version.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/bug.h>
#include <linux/interrupt.h>
#include <linux/device.h>
<<<<<<< HEAD
#include <linux/platform_device.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/i2c.h>
=======
#include <linux/pm_runtime.h>
#include <linux/list.h>
#include <linux/slab.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <linux/videodev2.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-mem2mem.h>
#include <media/videobuf2-core.h>
#include <media/videobuf2-dma-contig.h>

<<<<<<< HEAD
#include "fimc-core.h"

static struct v4l2_subdev *fimc_subdev_register(struct fimc_dev *fimc,
					    struct s5p_fimc_isp_info *isp_info)
{
	struct i2c_adapter *i2c_adap;
	struct fimc_vid_cap *vid_cap = &fimc->vid_cap;
	struct v4l2_subdev *sd = NULL;

	i2c_adap = i2c_get_adapter(isp_info->i2c_bus_num);
	if (!i2c_adap)
		return ERR_PTR(-ENOMEM);

	sd = v4l2_i2c_new_subdev_board(&vid_cap->v4l2_dev, i2c_adap,
				       isp_info->board_info, NULL);
	if (!sd) {
		v4l2_err(&vid_cap->v4l2_dev, "failed to acquire subdev\n");
		return NULL;
	}

	v4l2_info(&vid_cap->v4l2_dev, "subdevice %s registered successfuly\n",
		isp_info->board_info->type);

	return sd;
}

static void fimc_subdev_unregister(struct fimc_dev *fimc)
{
	struct fimc_vid_cap *vid_cap = &fimc->vid_cap;
	struct i2c_client *client;

	if (vid_cap->input_index < 0)
		return;	/* Subdevice already released or not registered. */

	if (vid_cap->sd) {
		v4l2_device_unregister_subdev(vid_cap->sd);
		client = v4l2_get_subdevdata(vid_cap->sd);
		i2c_unregister_device(client);
		i2c_put_adapter(client->adapter);
		vid_cap->sd = NULL;
	}

	vid_cap->input_index = -1;
}

/**
 * fimc_subdev_attach - attach v4l2_subdev to camera host interface
 *
 * @fimc: FIMC device information
 * @index: index to the array of available subdevices,
 *	   -1 for full array search or non negative value
 *	   to select specific subdevice
 */
static int fimc_subdev_attach(struct fimc_dev *fimc, int index)
{
	struct fimc_vid_cap *vid_cap = &fimc->vid_cap;
	struct s5p_platform_fimc *pdata = fimc->pdata;
	struct s5p_fimc_isp_info *isp_info;
	struct v4l2_subdev *sd;
	int i;

	for (i = 0; i < pdata->num_clients; ++i) {
		isp_info = &pdata->isp_info[i];

		if (index >= 0 && i != index)
			continue;

		sd = fimc_subdev_register(fimc, isp_info);
		if (!IS_ERR_OR_NULL(sd)) {
			vid_cap->sd = sd;
			vid_cap->input_index = i;

			return 0;
		}
	}

	vid_cap->input_index = -1;
	vid_cap->sd = NULL;
	v4l2_err(&vid_cap->v4l2_dev, "fimc%d: sensor attach failed\n",
		 fimc->id);
	return -ENODEV;
}

static int fimc_isp_subdev_init(struct fimc_dev *fimc, unsigned int index)
{
	struct s5p_fimc_isp_info *isp_info;
	struct s5p_platform_fimc *pdata = fimc->pdata;
	int ret;

	if (index >= pdata->num_clients)
		return -EINVAL;

	isp_info = &pdata->isp_info[index];

	if (isp_info->clk_frequency)
		clk_set_rate(fimc->clock[CLK_CAM], isp_info->clk_frequency);

	ret = clk_enable(fimc->clock[CLK_CAM]);
	if (ret)
		return ret;

	ret = fimc_subdev_attach(fimc, index);
	if (ret)
		return ret;

	ret = fimc_hw_set_camera_polarity(fimc, isp_info);
	if (ret)
		return ret;

	ret = v4l2_subdev_call(fimc->vid_cap.sd, core, s_power, 1);
	if (!ret)
		return ret;

	/* enabling power failed so unregister subdev */
	fimc_subdev_unregister(fimc);

	v4l2_err(&fimc->vid_cap.v4l2_dev, "ISP initialization failed: %d\n",
		 ret);

	return ret;
}

static int fimc_stop_capture(struct fimc_dev *fimc)
{
	unsigned long flags;
	struct fimc_vid_cap *cap;
	struct fimc_vid_buffer *buf;

	cap = &fimc->vid_cap;
=======
#include "fimc-mdevice.h"
#include "fimc-core.h"

static int fimc_init_capture(struct fimc_dev *fimc)
{
	struct fimc_ctx *ctx = fimc->vid_cap.ctx;
	struct fimc_sensor_info *sensor;
	unsigned long flags;
	int ret = 0;

	if (fimc->pipeline.sensor == NULL || ctx == NULL)
		return -ENXIO;
	if (ctx->s_frame.fmt == NULL)
		return -EINVAL;

	sensor = v4l2_get_subdev_hostdata(fimc->pipeline.sensor);

	spin_lock_irqsave(&fimc->slock, flags);
	fimc_prepare_dma_offset(ctx, &ctx->d_frame);
	fimc_set_yuv_order(ctx);

	fimc_hw_set_camera_polarity(fimc, sensor->pdata);
	fimc_hw_set_camera_type(fimc, sensor->pdata);
	fimc_hw_set_camera_source(fimc, sensor->pdata);
	fimc_hw_set_camera_offset(fimc, &ctx->s_frame);

	ret = fimc_set_scaler_info(ctx);
	if (!ret) {
		fimc_hw_set_input_path(ctx);
		fimc_hw_set_prescaler(ctx);
		fimc_hw_set_mainscaler(ctx);
		fimc_hw_set_target_format(ctx);
		fimc_hw_set_rotation(ctx);
		fimc_hw_set_effect(ctx, false);
		fimc_hw_set_output_path(ctx);
		fimc_hw_set_out_dma(ctx);
		if (fimc->variant->has_alpha)
			fimc_hw_set_rgb_alpha(ctx);
		clear_bit(ST_CAPT_APPLY_CFG, &fimc->state);
	}
	spin_unlock_irqrestore(&fimc->slock, flags);
	return ret;
}

static int fimc_capture_state_cleanup(struct fimc_dev *fimc, bool suspend)
{
	struct fimc_vid_cap *cap = &fimc->vid_cap;
	struct fimc_vid_buffer *buf;
	unsigned long flags;
	bool streaming;

	spin_lock_irqsave(&fimc->slock, flags);
	streaming = fimc->state & (1 << ST_CAPT_ISP_STREAM);

	fimc->state &= ~(1 << ST_CAPT_RUN | 1 << ST_CAPT_SHUT |
			 1 << ST_CAPT_STREAM | 1 << ST_CAPT_ISP_STREAM);
	if (!suspend)
		fimc->state &= ~(1 << ST_CAPT_PEND | 1 << ST_CAPT_SUSPENDED);

	/* Release unused buffers */
	while (!suspend && !list_empty(&cap->pending_buf_q)) {
		buf = fimc_pending_queue_pop(cap);
		vb2_buffer_done(&buf->vb, VB2_BUF_STATE_ERROR);
	}
	/* If suspending put unused buffers onto pending queue */
	while (!list_empty(&cap->active_buf_q)) {
		buf = fimc_active_queue_pop(cap);
		if (suspend)
			fimc_pending_queue_add(cap, buf);
		else
			vb2_buffer_done(&buf->vb, VB2_BUF_STATE_ERROR);
	}
	set_bit(ST_CAPT_SUSPENDED, &fimc->state);

	fimc_hw_reset(fimc);
	cap->buf_index = 0;

	spin_unlock_irqrestore(&fimc->slock, flags);

	if (streaming)
		return fimc_pipeline_s_stream(fimc, 0);
	else
		return 0;
}

static int fimc_stop_capture(struct fimc_dev *fimc, bool suspend)
{
	unsigned long flags;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!fimc_capture_active(fimc))
		return 0;

	spin_lock_irqsave(&fimc->slock, flags);
	set_bit(ST_CAPT_SHUT, &fimc->state);
	fimc_deactivate_capture(fimc);
	spin_unlock_irqrestore(&fimc->slock, flags);

	wait_event_timeout(fimc->irq_queue,
			   !test_bit(ST_CAPT_SHUT, &fimc->state),
<<<<<<< HEAD
			   FIMC_SHUTDOWN_TIMEOUT);

	v4l2_subdev_call(cap->sd, video, s_stream, 0);

	spin_lock_irqsave(&fimc->slock, flags);
	fimc->state &= ~(1 << ST_CAPT_RUN | 1 << ST_CAPT_PEND |
			 1 << ST_CAPT_SHUT | 1 << ST_CAPT_STREAM);

	fimc->vid_cap.active_buf_cnt = 0;

	/* Release buffers that were enqueued in the driver by videobuf2. */
	while (!list_empty(&cap->pending_buf_q)) {
		buf = pending_queue_pop(cap);
		vb2_buffer_done(&buf->vb, VB2_BUF_STATE_ERROR);
	}

	while (!list_empty(&cap->active_buf_q)) {
		buf = active_queue_pop(cap);
		vb2_buffer_done(&buf->vb, VB2_BUF_STATE_ERROR);
	}

	spin_unlock_irqrestore(&fimc->slock, flags);

	dbg("state: 0x%lx", fimc->state);
	return 0;
}

static int start_streaming(struct vb2_queue *q)
{
	struct fimc_ctx *ctx = q->drv_priv;
	struct fimc_dev *fimc = ctx->fimc_dev;
	struct s5p_fimc_isp_info *isp_info;
	int ret;

	fimc_hw_reset(fimc);

	ret = v4l2_subdev_call(fimc->vid_cap.sd, video, s_stream, 1);
	if (ret && ret != -ENOIOCTLCMD)
		return ret;

	ret = fimc_prepare_config(ctx, ctx->state);
	if (ret)
		return ret;

	isp_info = &fimc->pdata->isp_info[fimc->vid_cap.input_index];
	fimc_hw_set_camera_type(fimc, isp_info);
	fimc_hw_set_camera_source(fimc, isp_info);
	fimc_hw_set_camera_offset(fimc, &ctx->s_frame);

	if (ctx->state & FIMC_PARAMS) {
		ret = fimc_set_scaler_info(ctx);
		if (ret) {
			err("Scaler setup error");
			return ret;
		}
		fimc_hw_set_input_path(ctx);
=======
			   (2*HZ/10)); /* 200 ms */

	return fimc_capture_state_cleanup(fimc, suspend);
}

/**
 * fimc_capture_config_update - apply the camera interface configuration
 *
 * To be called from within the interrupt handler with fimc.slock
 * spinlock held. It updates the camera pixel crop, rotation and
 * image flip in H/W.
 */
int fimc_capture_config_update(struct fimc_ctx *ctx)
{
	struct fimc_dev *fimc = ctx->fimc_dev;
	int ret;

	if (!test_bit(ST_CAPT_APPLY_CFG, &fimc->state))
		return 0;

	spin_lock(&ctx->slock);
	fimc_hw_set_camera_offset(fimc, &ctx->s_frame);
	ret = fimc_set_scaler_info(ctx);
	if (ret == 0) {
>>>>>>> refs/remotes/origin/cm-10.0
		fimc_hw_set_prescaler(ctx);
		fimc_hw_set_mainscaler(ctx);
		fimc_hw_set_target_format(ctx);
		fimc_hw_set_rotation(ctx);
<<<<<<< HEAD
		fimc_hw_set_effect(ctx);
	}

	fimc_hw_set_output_path(ctx);
	fimc_hw_set_out_dma(ctx);

	INIT_LIST_HEAD(&fimc->vid_cap.pending_buf_q);
	INIT_LIST_HEAD(&fimc->vid_cap.active_buf_q);
	fimc->vid_cap.active_buf_cnt = 0;
	fimc->vid_cap.frame_count = 0;
	fimc->vid_cap.buf_index = 0;

	set_bit(ST_CAPT_PEND, &fimc->state);

	return 0;
=======
		fimc_prepare_dma_offset(ctx, &ctx->d_frame);
		fimc_hw_set_out_dma(ctx);
		if (fimc->variant->has_alpha)
			fimc_hw_set_rgb_alpha(ctx);
		clear_bit(ST_CAPT_APPLY_CFG, &fimc->state);
	}
	spin_unlock(&ctx->slock);
	return ret;
}

static int start_streaming(struct vb2_queue *q, unsigned int count)
{
	struct fimc_ctx *ctx = q->drv_priv;
	struct fimc_dev *fimc = ctx->fimc_dev;
	struct fimc_vid_cap *vid_cap = &fimc->vid_cap;
	int min_bufs;
	int ret;

	vid_cap->frame_count = 0;

	ret = fimc_init_capture(fimc);
	if (ret)
		goto error;

	set_bit(ST_CAPT_PEND, &fimc->state);

	min_bufs = fimc->vid_cap.reqbufs_count > 1 ? 2 : 1;

	if (vid_cap->active_buf_cnt >= min_bufs &&
	    !test_and_set_bit(ST_CAPT_STREAM, &fimc->state)) {
		fimc_activate_capture(ctx);

		if (!test_and_set_bit(ST_CAPT_ISP_STREAM, &fimc->state))
			fimc_pipeline_s_stream(fimc, 1);
	}

	return 0;
error:
	fimc_capture_state_cleanup(fimc, false);
	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int stop_streaming(struct vb2_queue *q)
{
	struct fimc_ctx *ctx = q->drv_priv;
	struct fimc_dev *fimc = ctx->fimc_dev;

	if (!fimc_capture_active(fimc))
		return -EINVAL;

<<<<<<< HEAD
	return fimc_stop_capture(fimc);
}

static unsigned int get_plane_size(struct fimc_frame *fr, unsigned int plane)
{
	if (!fr || plane >= fr->fmt->memplanes)
		return 0;
	return fr->f_width * fr->f_height * fr->fmt->depth[plane] / 8;
}

static int queue_setup(struct vb2_queue *vq, unsigned int *num_buffers,
		       unsigned int *num_planes, unsigned long sizes[],
		       void *allocators[])
{
	struct fimc_ctx *ctx = vq->drv_priv;
	struct fimc_fmt *fmt = ctx->d_frame.fmt;
	int i;

	if (!fmt)
=======
	return fimc_stop_capture(fimc, false);
}

int fimc_capture_suspend(struct fimc_dev *fimc)
{
	bool suspend = fimc_capture_busy(fimc);

	int ret = fimc_stop_capture(fimc, suspend);
	if (ret)
		return ret;
	return fimc_pipeline_shutdown(fimc);
}

static void buffer_queue(struct vb2_buffer *vb);

int fimc_capture_resume(struct fimc_dev *fimc)
{
	struct fimc_vid_cap *vid_cap = &fimc->vid_cap;
	struct fimc_vid_buffer *buf;
	int i;

	if (!test_and_clear_bit(ST_CAPT_SUSPENDED, &fimc->state))
		return 0;

	INIT_LIST_HEAD(&fimc->vid_cap.active_buf_q);
	vid_cap->buf_index = 0;
	fimc_pipeline_initialize(fimc, &fimc->vid_cap.vfd->entity,
				 false);
	fimc_init_capture(fimc);

	clear_bit(ST_CAPT_SUSPENDED, &fimc->state);

	for (i = 0; i < vid_cap->reqbufs_count; i++) {
		if (list_empty(&vid_cap->pending_buf_q))
			break;
		buf = fimc_pending_queue_pop(vid_cap);
		buffer_queue(&buf->vb);
	}
	return 0;

}

static int queue_setup(struct vb2_queue *vq, const struct v4l2_format *pfmt,
		       unsigned int *num_buffers, unsigned int *num_planes,
		       unsigned int sizes[], void *allocators[])
{
	const struct v4l2_pix_format_mplane *pixm = NULL;
	struct fimc_ctx *ctx = vq->drv_priv;
	struct fimc_frame *frame = &ctx->d_frame;
	struct fimc_fmt *fmt = frame->fmt;
	unsigned long wh;
	int i;

	if (pfmt) {
		pixm = &pfmt->fmt.pix_mp;
		fmt = fimc_find_format(&pixm->pixelformat, NULL,
				       FMT_FLAGS_CAM | FMT_FLAGS_M2M, -1);
		wh = pixm->width * pixm->height;
	} else {
		wh = frame->f_width * frame->f_height;
	}

	if (fmt == NULL)
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;

	*num_planes = fmt->memplanes;

	for (i = 0; i < fmt->memplanes; i++) {
<<<<<<< HEAD
		sizes[i] = get_plane_size(&ctx->d_frame, i);
=======
		unsigned int size = (wh * fmt->depth[i]) / 8;
		if (pixm)
			sizes[i] = max(size, pixm->plane_fmt[i].sizeimage);
		else
			sizes[i] = size;
>>>>>>> refs/remotes/origin/cm-10.0
		allocators[i] = ctx->fimc_dev->alloc_ctx;
	}

	return 0;
}

static int buffer_prepare(struct vb2_buffer *vb)
{
	struct vb2_queue *vq = vb->vb2_queue;
	struct fimc_ctx *ctx = vq->drv_priv;
<<<<<<< HEAD
	struct v4l2_device *v4l2_dev = &ctx->fimc_dev->m2m.v4l2_dev;
	int i;

	if (!ctx->d_frame.fmt || vq->type != V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE)
		return -EINVAL;

	for (i = 0; i < ctx->d_frame.fmt->memplanes; i++) {
		unsigned long size = get_plane_size(&ctx->d_frame, i);

		if (vb2_plane_size(vb, i) < size) {
			v4l2_err(v4l2_dev, "User buffer too small (%ld < %ld)\n",
				 vb2_plane_size(vb, i), size);
			return -EINVAL;
		}

=======
	int i;

	if (ctx->d_frame.fmt == NULL)
		return -EINVAL;

	for (i = 0; i < ctx->d_frame.fmt->memplanes; i++) {
		unsigned long size = ctx->d_frame.payload[i];

		if (vb2_plane_size(vb, i) < size) {
			v4l2_err(ctx->fimc_dev->vid_cap.vfd,
				 "User buffer too small (%ld < %ld)\n",
				 vb2_plane_size(vb, i), size);
			return -EINVAL;
		}
>>>>>>> refs/remotes/origin/cm-10.0
		vb2_set_plane_payload(vb, i, size);
	}

	return 0;
}

static void buffer_queue(struct vb2_buffer *vb)
{
<<<<<<< HEAD
	struct fimc_ctx *ctx = vb2_get_drv_priv(vb->vb2_queue);
	struct fimc_dev *fimc = ctx->fimc_dev;
	struct fimc_vid_buffer *buf
		= container_of(vb, struct fimc_vid_buffer, vb);
=======
	struct fimc_vid_buffer *buf
		= container_of(vb, struct fimc_vid_buffer, vb);
	struct fimc_ctx *ctx = vb2_get_drv_priv(vb->vb2_queue);
	struct fimc_dev *fimc = ctx->fimc_dev;
>>>>>>> refs/remotes/origin/cm-10.0
	struct fimc_vid_cap *vid_cap = &fimc->vid_cap;
	unsigned long flags;
	int min_bufs;

	spin_lock_irqsave(&fimc->slock, flags);
	fimc_prepare_addr(ctx, &buf->vb, &ctx->d_frame, &buf->paddr);

<<<<<<< HEAD
	if (!test_bit(ST_CAPT_STREAM, &fimc->state)
	     && vid_cap->active_buf_cnt < FIMC_MAX_OUT_BUFS) {
=======
	if (!test_bit(ST_CAPT_SUSPENDED, &fimc->state) &&
	    !test_bit(ST_CAPT_STREAM, &fimc->state) &&
	    vid_cap->active_buf_cnt < FIMC_MAX_OUT_BUFS) {
>>>>>>> refs/remotes/origin/cm-10.0
		/* Setup the buffer directly for processing. */
		int buf_id = (vid_cap->reqbufs_count == 1) ? -1 :
				vid_cap->buf_index;

		fimc_hw_set_output_addr(fimc, &buf->paddr, buf_id);
		buf->index = vid_cap->buf_index;
<<<<<<< HEAD
		active_queue_add(vid_cap, buf);
=======
		fimc_active_queue_add(vid_cap, buf);
>>>>>>> refs/remotes/origin/cm-10.0

		if (++vid_cap->buf_index >= FIMC_MAX_OUT_BUFS)
			vid_cap->buf_index = 0;
	} else {
		fimc_pending_queue_add(vid_cap, buf);
	}

	min_bufs = vid_cap->reqbufs_count > 1 ? 2 : 1;

<<<<<<< HEAD
	if (vid_cap->active_buf_cnt >= min_bufs &&
	    !test_and_set_bit(ST_CAPT_STREAM, &fimc->state))
		fimc_activate_capture(ctx);

=======

	if (vb2_is_streaming(&vid_cap->vbq) &&
	    vid_cap->active_buf_cnt >= min_bufs &&
	    !test_and_set_bit(ST_CAPT_STREAM, &fimc->state)) {
		fimc_activate_capture(ctx);
		spin_unlock_irqrestore(&fimc->slock, flags);

		if (!test_and_set_bit(ST_CAPT_ISP_STREAM, &fimc->state))
			fimc_pipeline_s_stream(fimc, 1);
		return;
	}
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock_irqrestore(&fimc->slock, flags);
}

static void fimc_lock(struct vb2_queue *vq)
{
	struct fimc_ctx *ctx = vb2_get_drv_priv(vq);
	mutex_lock(&ctx->fimc_dev->lock);
}

static void fimc_unlock(struct vb2_queue *vq)
{
	struct fimc_ctx *ctx = vb2_get_drv_priv(vq);
	mutex_unlock(&ctx->fimc_dev->lock);
}

static struct vb2_ops fimc_capture_qops = {
	.queue_setup		= queue_setup,
	.buf_prepare		= buffer_prepare,
	.buf_queue		= buffer_queue,
	.wait_prepare		= fimc_unlock,
	.wait_finish		= fimc_lock,
	.start_streaming	= start_streaming,
	.stop_streaming		= stop_streaming,
};

<<<<<<< HEAD
static int fimc_capture_open(struct file *file)
{
	struct fimc_dev *fimc = video_drvdata(file);
	int ret = 0;
=======
/**
 * fimc_capture_ctrls_create - initialize the control handler
 * Initialize the capture video node control handler and fill it
 * with the FIMC controls. Inherit any sensor's controls if the
 * 'user_subdev_api' flag is false (default behaviour).
 * This function need to be called with the graph mutex held.
 */
int fimc_capture_ctrls_create(struct fimc_dev *fimc)
{
	struct fimc_vid_cap *vid_cap = &fimc->vid_cap;
	int ret;

	if (WARN_ON(vid_cap->ctx == NULL))
		return -ENXIO;
	if (vid_cap->ctx->ctrls_rdy)
		return 0;

	ret = fimc_ctrls_create(vid_cap->ctx);
	if (ret || vid_cap->user_subdev_api)
		return ret;

	return v4l2_ctrl_add_handler(&vid_cap->ctx->ctrl_handler,
				    fimc->pipeline.sensor->ctrl_handler);
}

static int fimc_capture_set_default_format(struct fimc_dev *fimc);

static int fimc_capture_open(struct file *file)
{
	struct fimc_dev *fimc = video_drvdata(file);
	int ret = v4l2_fh_open(file);

	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/cm-10.0

	dbg("pid: %d, state: 0x%lx", task_pid_nr(current), fimc->state);

	/* Return if the corresponding video mem2mem node is already opened. */
	if (fimc_m2m_active(fimc))
		return -EBUSY;

<<<<<<< HEAD
	if (++fimc->vid_cap.refcnt == 1) {
		ret = fimc_isp_subdev_init(fimc, 0);
		if (ret) {
			fimc->vid_cap.refcnt--;
			return -EIO;
		}
	}

	file->private_data = fimc->vid_cap.ctx;

	return 0;
=======
	set_bit(ST_CAPT_BUSY, &fimc->state);
	pm_runtime_get_sync(&fimc->pdev->dev);

	if (++fimc->vid_cap.refcnt == 1) {
		ret = fimc_pipeline_initialize(fimc,
			       &fimc->vid_cap.vfd->entity, true);
		if (ret < 0) {
			dev_err(&fimc->pdev->dev,
				"Video pipeline initialization failed\n");
			pm_runtime_put_sync(&fimc->pdev->dev);
			fimc->vid_cap.refcnt--;
			v4l2_fh_release(file);
			clear_bit(ST_CAPT_BUSY, &fimc->state);
			return ret;
		}
		ret = fimc_capture_ctrls_create(fimc);

		if (!ret && !fimc->vid_cap.user_subdev_api)
			ret = fimc_capture_set_default_format(fimc);
	}
	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int fimc_capture_close(struct file *file)
{
	struct fimc_dev *fimc = video_drvdata(file);

	dbg("pid: %d, state: 0x%lx", task_pid_nr(current), fimc->state);

	if (--fimc->vid_cap.refcnt == 0) {
<<<<<<< HEAD
		fimc_stop_capture(fimc);
		vb2_queue_release(&fimc->vid_cap.vbq);

		v4l2_err(&fimc->vid_cap.v4l2_dev, "releasing ISP\n");

		v4l2_subdev_call(fimc->vid_cap.sd, core, s_power, 0);
		clk_disable(fimc->clock[CLK_CAM]);
		fimc_subdev_unregister(fimc);
	}

	return 0;
=======
		clear_bit(ST_CAPT_BUSY, &fimc->state);
		fimc_stop_capture(fimc, false);
		fimc_pipeline_shutdown(fimc);
		clear_bit(ST_CAPT_SUSPENDED, &fimc->state);
	}

	pm_runtime_put(&fimc->pdev->dev);

	if (fimc->vid_cap.refcnt == 0) {
		vb2_queue_release(&fimc->vid_cap.vbq);
		fimc_ctrls_delete(fimc->vid_cap.ctx);
	}
	return v4l2_fh_release(file);
>>>>>>> refs/remotes/origin/cm-10.0
}

static unsigned int fimc_capture_poll(struct file *file,
				      struct poll_table_struct *wait)
{
<<<<<<< HEAD
	struct fimc_ctx *ctx = file->private_data;
	struct fimc_dev *fimc = ctx->fimc_dev;
=======
	struct fimc_dev *fimc = video_drvdata(file);
>>>>>>> refs/remotes/origin/cm-10.0

	return vb2_poll(&fimc->vid_cap.vbq, file, wait);
}

static int fimc_capture_mmap(struct file *file, struct vm_area_struct *vma)
{
<<<<<<< HEAD
	struct fimc_ctx *ctx = file->private_data;
	struct fimc_dev *fimc = ctx->fimc_dev;
=======
	struct fimc_dev *fimc = video_drvdata(file);
>>>>>>> refs/remotes/origin/cm-10.0

	return vb2_mmap(&fimc->vid_cap.vbq, vma);
}

<<<<<<< HEAD
/* video device file operations */
=======
>>>>>>> refs/remotes/origin/cm-10.0
static const struct v4l2_file_operations fimc_capture_fops = {
	.owner		= THIS_MODULE,
	.open		= fimc_capture_open,
	.release	= fimc_capture_close,
	.poll		= fimc_capture_poll,
	.unlocked_ioctl	= video_ioctl2,
	.mmap		= fimc_capture_mmap,
};

<<<<<<< HEAD
static int fimc_vidioc_querycap_capture(struct file *file, void *priv,
					struct v4l2_capability *cap)
{
	struct fimc_ctx *ctx = file->private_data;
	struct fimc_dev *fimc = ctx->fimc_dev;
=======
/*
 * Format and crop negotiation helpers
 */

static struct fimc_fmt *fimc_capture_try_format(struct fimc_ctx *ctx,
						u32 *width, u32 *height,
						u32 *code, u32 *fourcc, int pad)
{
	bool rotation = ctx->rotation == 90 || ctx->rotation == 270;
	struct fimc_dev *fimc = ctx->fimc_dev;
	struct samsung_fimc_variant *var = fimc->variant;
	struct fimc_pix_limit *pl = var->pix_limit;
	struct fimc_frame *dst = &ctx->d_frame;
	u32 depth, min_w, max_w, min_h, align_h = 3;
	u32 mask = FMT_FLAGS_CAM;
	struct fimc_fmt *ffmt;

	/* Color conversion from/to JPEG is not supported */
	if (code && ctx->s_frame.fmt && pad == FIMC_SD_PAD_SOURCE &&
	    fimc_fmt_is_jpeg(ctx->s_frame.fmt->color))
		*code = V4L2_MBUS_FMT_JPEG_1X8;

	if (fourcc && *fourcc != V4L2_PIX_FMT_JPEG && pad != FIMC_SD_PAD_SINK)
		mask |= FMT_FLAGS_M2M;

	ffmt = fimc_find_format(fourcc, code, mask, 0);
	if (WARN_ON(!ffmt))
		return NULL;
	if (code)
		*code = ffmt->mbus_code;
	if (fourcc)
		*fourcc = ffmt->fourcc;

	if (pad == FIMC_SD_PAD_SINK) {
		max_w = fimc_fmt_is_jpeg(ffmt->color) ?
			pl->scaler_dis_w : pl->scaler_en_w;
		/* Apply the camera input interface pixel constraints */
		v4l_bound_align_image(width, max_t(u32, *width, 32), max_w, 4,
				      height, max_t(u32, *height, 32),
				      FIMC_CAMIF_MAX_HEIGHT,
				      fimc_fmt_is_jpeg(ffmt->color) ? 3 : 1,
				      0);
		return ffmt;
	}
	/* Can't scale or crop in transparent (JPEG) transfer mode */
	if (fimc_fmt_is_jpeg(ffmt->color)) {
		*width  = ctx->s_frame.f_width;
		*height = ctx->s_frame.f_height;
		return ffmt;
	}
	/* Apply the scaler and the output DMA constraints */
	max_w = rotation ? pl->out_rot_en_w : pl->out_rot_dis_w;
	min_w = ctx->state & FIMC_DST_CROP ? dst->width : var->min_out_pixsize;
	min_h = ctx->state & FIMC_DST_CROP ? dst->height : var->min_out_pixsize;
	if (var->min_vsize_align == 1 && !rotation)
		align_h = fimc_fmt_is_rgb(ffmt->color) ? 0 : 1;

	depth = fimc_get_format_depth(ffmt);
	v4l_bound_align_image(width, min_w, max_w,
			      ffs(var->min_out_pixsize) - 1,
			      height, min_h, FIMC_CAMIF_MAX_HEIGHT,
			      align_h,
			      64/(ALIGN(depth, 8)));

	dbg("pad%d: code: 0x%x, %dx%d. dst fmt: %dx%d",
	    pad, code ? *code : 0, *width, *height,
	    dst->f_width, dst->f_height);

	return ffmt;
}

static void fimc_capture_try_crop(struct fimc_ctx *ctx, struct v4l2_rect *r,
				  int pad)
{
	bool rotate = ctx->rotation == 90 || ctx->rotation == 270;
	struct fimc_dev *fimc = ctx->fimc_dev;
	struct samsung_fimc_variant *var = fimc->variant;
	struct fimc_pix_limit *pl = var->pix_limit;
	struct fimc_frame *sink = &ctx->s_frame;
	u32 max_w, max_h, min_w = 0, min_h = 0, min_sz;
	u32 align_sz = 0, align_h = 4;
	u32 max_sc_h, max_sc_v;

	/* In JPEG transparent transfer mode cropping is not supported */
	if (fimc_fmt_is_jpeg(ctx->d_frame.fmt->color)) {
		r->width  = sink->f_width;
		r->height = sink->f_height;
		r->left   = r->top = 0;
		return;
	}
	if (pad == FIMC_SD_PAD_SOURCE) {
		if (ctx->rotation != 90 && ctx->rotation != 270)
			align_h = 1;
		max_sc_h = min(SCALER_MAX_HRATIO, 1 << (ffs(sink->width) - 3));
		max_sc_v = min(SCALER_MAX_VRATIO, 1 << (ffs(sink->height) - 1));
		min_sz = var->min_out_pixsize;
	} else {
		u32 depth = fimc_get_format_depth(sink->fmt);
		align_sz = 64/ALIGN(depth, 8);
		min_sz = var->min_inp_pixsize;
		min_w = min_h = min_sz;
		max_sc_h = max_sc_v = 1;
	}
	/*
	 * For the crop rectangle at source pad the following constraints
	 * must be met:
	 * - it must fit in the sink pad format rectangle (f_width/f_height);
	 * - maximum downscaling ratio is 64;
	 * - maximum crop size depends if the rotator is used or not;
	 * - the sink pad format width/height must be 4 multiple of the
	 *   prescaler ratios determined by sink pad size and source pad crop,
	 *   the prescaler ratio is returned by fimc_get_scaler_factor().
	 */
	max_w = min_t(u32,
		      rotate ? pl->out_rot_en_w : pl->out_rot_dis_w,
		      rotate ? sink->f_height : sink->f_width);
	max_h = min_t(u32, FIMC_CAMIF_MAX_HEIGHT, sink->f_height);
	if (pad == FIMC_SD_PAD_SOURCE) {
		min_w = min_t(u32, max_w, sink->f_width / max_sc_h);
		min_h = min_t(u32, max_h, sink->f_height / max_sc_v);
		if (rotate) {
			swap(max_sc_h, max_sc_v);
			swap(min_w, min_h);
		}
	}
	v4l_bound_align_image(&r->width, min_w, max_w, ffs(min_sz) - 1,
			      &r->height, min_h, max_h, align_h,
			      align_sz);
	/* Adjust left/top if cropping rectangle is out of bounds */
	r->left = clamp_t(u32, r->left, 0, sink->f_width - r->width);
	r->top  = clamp_t(u32, r->top, 0, sink->f_height - r->height);
	r->left = round_down(r->left, var->hor_offs_align);

	dbg("pad%d: (%d,%d)/%dx%d, sink fmt: %dx%d",
	    pad, r->left, r->top, r->width, r->height,
	    sink->f_width, sink->f_height);
}

/*
 * The video node ioctl operations
 */
static int fimc_vidioc_querycap_capture(struct file *file, void *priv,
					struct v4l2_capability *cap)
{
	struct fimc_dev *fimc = video_drvdata(file);
>>>>>>> refs/remotes/origin/cm-10.0

	strncpy(cap->driver, fimc->pdev->name, sizeof(cap->driver) - 1);
	strncpy(cap->card, fimc->pdev->name, sizeof(cap->card) - 1);
	cap->bus_info[0] = 0;
<<<<<<< HEAD
	cap->version = KERNEL_VERSION(1, 0, 0);
	cap->capabilities = V4L2_CAP_STREAMING | V4L2_CAP_VIDEO_CAPTURE |
			    V4L2_CAP_VIDEO_CAPTURE_MPLANE;

	return 0;
}

/* Synchronize formats of the camera interface input and attached  sensor. */
static int sync_capture_fmt(struct fimc_ctx *ctx)
{
	struct fimc_frame *frame = &ctx->s_frame;
	struct fimc_dev *fimc = ctx->fimc_dev;
	struct v4l2_mbus_framefmt *fmt = &fimc->vid_cap.fmt;
	int ret;

	fmt->width  = ctx->d_frame.o_width;
	fmt->height = ctx->d_frame.o_height;

	ret = v4l2_subdev_call(fimc->vid_cap.sd, video, s_mbus_fmt, fmt);
	if (ret == -ENOIOCTLCMD) {
		err("s_mbus_fmt failed");
		return ret;
	}
	dbg("w: %d, h: %d, code= %d", fmt->width, fmt->height, fmt->code);

	frame->fmt = find_mbus_format(fmt, FMT_FLAGS_CAM);
	if (!frame->fmt) {
		err("fimc source format not found\n");
		return -EINVAL;
	}

	frame->f_width	= fmt->width;
	frame->f_height = fmt->height;
	frame->width	= fmt->width;
	frame->height	= fmt->height;
	frame->o_width	= fmt->width;
	frame->o_height = fmt->height;
	frame->offs_h	= 0;
	frame->offs_v	= 0;

	return 0;
}

static int fimc_cap_s_fmt_mplane(struct file *file, void *priv,
				 struct v4l2_format *f)
{
	struct fimc_ctx *ctx = priv;
	struct fimc_dev *fimc = ctx->fimc_dev;
	struct fimc_frame *frame;
	struct v4l2_pix_format_mplane *pix;
	int ret;
	int i;
=======
	cap->capabilities = V4L2_CAP_STREAMING | V4L2_CAP_VIDEO_CAPTURE_MPLANE;

	return 0;
}

static int fimc_cap_enum_fmt_mplane(struct file *file, void *priv,
				    struct v4l2_fmtdesc *f)
{
	struct fimc_fmt *fmt;

	fmt = fimc_find_format(NULL, NULL, FMT_FLAGS_CAM | FMT_FLAGS_M2M,
			       f->index);
	if (!fmt)
		return -EINVAL;
	strncpy(f->description, fmt->name, sizeof(f->description) - 1);
	f->pixelformat = fmt->fourcc;
	if (fmt->fourcc == V4L2_MBUS_FMT_JPEG_1X8)
		f->flags |= V4L2_FMT_FLAG_COMPRESSED;
	return 0;
}

/**
 * fimc_pipeline_try_format - negotiate and/or set formats at pipeline
 *                            elements
 * @ctx: FIMC capture context
 * @tfmt: media bus format to try/set on subdevs
 * @fmt_id: fimc pixel format id corresponding to returned @tfmt (output)
 * @set: true to set format on subdevs, false to try only
 */
static int fimc_pipeline_try_format(struct fimc_ctx *ctx,
				    struct v4l2_mbus_framefmt *tfmt,
				    struct fimc_fmt **fmt_id,
				    bool set)
{
	struct fimc_dev *fimc = ctx->fimc_dev;
	struct v4l2_subdev *sd = fimc->pipeline.sensor;
	struct v4l2_subdev *csis = fimc->pipeline.csis;
	struct v4l2_subdev_format sfmt;
	struct v4l2_mbus_framefmt *mf = &sfmt.format;
	struct fimc_fmt *ffmt = NULL;
	int ret, i = 0;

	if (WARN_ON(!sd || !tfmt))
		return -EINVAL;

	memset(&sfmt, 0, sizeof(sfmt));
	sfmt.format = *tfmt;

	sfmt.which = set ? V4L2_SUBDEV_FORMAT_ACTIVE : V4L2_SUBDEV_FORMAT_TRY;
	while (1) {
		ffmt = fimc_find_format(NULL, mf->code != 0 ? &mf->code : NULL,
					FMT_FLAGS_CAM, i++);
		if (ffmt == NULL) {
			/*
			 * Notify user-space if common pixel code for
			 * host and sensor does not exist.
			 */
			return -EINVAL;
		}
		mf->code = tfmt->code = ffmt->mbus_code;

		ret = v4l2_subdev_call(sd, pad, set_fmt, NULL, &sfmt);
		if (ret)
			return ret;
		if (mf->code != tfmt->code) {
			mf->code = 0;
			continue;
		}
		if (mf->width != tfmt->width || mf->height != tfmt->height) {
			u32 fcc = ffmt->fourcc;
			tfmt->width  = mf->width;
			tfmt->height = mf->height;
			ffmt = fimc_capture_try_format(ctx,
					       &tfmt->width, &tfmt->height,
					       NULL, &fcc, FIMC_SD_PAD_SOURCE);
			if (ffmt && ffmt->mbus_code)
				mf->code = ffmt->mbus_code;
			if (mf->width != tfmt->width ||
			    mf->height != tfmt->height)
				continue;
			tfmt->code = mf->code;
		}
		if (csis)
			ret = v4l2_subdev_call(csis, pad, set_fmt, NULL, &sfmt);

		if (mf->code == tfmt->code &&
		    mf->width == tfmt->width && mf->height == tfmt->height)
			break;
	}

	if (fmt_id && ffmt)
		*fmt_id = ffmt;
	*tfmt = *mf;

	dbg("code: 0x%x, %dx%d, %p", mf->code, mf->width, mf->height, ffmt);
	return 0;
}

static int fimc_cap_g_fmt_mplane(struct file *file, void *fh,
				 struct v4l2_format *f)
{
	struct fimc_dev *fimc = video_drvdata(file);
	struct fimc_ctx *ctx = fimc->vid_cap.ctx;
>>>>>>> refs/remotes/origin/cm-10.0

	if (f->type != V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE)
		return -EINVAL;

<<<<<<< HEAD
	ret = fimc_vidioc_try_fmt_mplane(file, priv, f);
	if (ret)
		return ret;

	if (vb2_is_busy(&fimc->vid_cap.vbq) || fimc_capture_active(fimc))
		return -EBUSY;

	frame = &ctx->d_frame;

	pix = &f->fmt.pix_mp;
	frame->fmt = find_format(f, FMT_FLAGS_M2M | FMT_FLAGS_CAM);
	if (!frame->fmt) {
		err("fimc target format not found\n");
		return -EINVAL;
	}

	for (i = 0; i < frame->fmt->colplanes; i++) {
		frame->payload[i] =
			(pix->width * pix->height * frame->fmt->depth[i]) >> 3;
	}

	/* Output DMA frame pixel size and offsets. */
	frame->f_width = pix->plane_fmt[0].bytesperline * 8
			/ frame->fmt->depth[0];
	frame->f_height = pix->height;
	frame->width	= pix->width;
	frame->height	= pix->height;
	frame->o_width	= pix->width;
	frame->o_height = pix->height;
	frame->offs_h	= 0;
	frame->offs_v	= 0;

	ctx->state |= (FIMC_PARAMS | FIMC_DST_FMT);

	ret = sync_capture_fmt(ctx);
	return ret;
}

static int fimc_cap_enum_input(struct file *file, void *priv,
				     struct v4l2_input *i)
{
	struct fimc_ctx *ctx = priv;
	struct s5p_platform_fimc *pldata = ctx->fimc_dev->pdata;
	struct s5p_fimc_isp_info *isp_info;

	if (i->index >= pldata->num_clients)
		return -EINVAL;

	isp_info = &pldata->isp_info[i->index];

	i->type = V4L2_INPUT_TYPE_CAMERA;
	strncpy(i->name, isp_info->board_info->type, 32);
	return 0;
}

static int fimc_cap_s_input(struct file *file, void *priv,
				  unsigned int i)
{
	struct fimc_ctx *ctx = priv;
	struct fimc_dev *fimc = ctx->fimc_dev;
	struct s5p_platform_fimc *pdata = fimc->pdata;

	if (fimc_capture_active(ctx->fimc_dev))
		return -EBUSY;

	if (i >= pdata->num_clients)
		return -EINVAL;


	if (fimc->vid_cap.sd) {
		int ret = v4l2_subdev_call(fimc->vid_cap.sd, core, s_power, 0);
		if (ret)
			err("s_power failed: %d", ret);

		clk_disable(fimc->clock[CLK_CAM]);
	}

	/* Release the attached sensor subdevice. */
	fimc_subdev_unregister(fimc);

	return fimc_isp_subdev_init(fimc, i);
}

static int fimc_cap_g_input(struct file *file, void *priv,
				       unsigned int *i)
{
	struct fimc_ctx *ctx = priv;
	struct fimc_vid_cap *cap = &ctx->fimc_dev->vid_cap;

	*i = cap->input_index;
=======
	return fimc_fill_format(&ctx->d_frame, f);
}

static int fimc_cap_try_fmt_mplane(struct file *file, void *fh,
				   struct v4l2_format *f)
{
	struct v4l2_pix_format_mplane *pix = &f->fmt.pix_mp;
	struct fimc_dev *fimc = video_drvdata(file);
	struct fimc_ctx *ctx = fimc->vid_cap.ctx;
	struct v4l2_mbus_framefmt mf;
	struct fimc_fmt *ffmt = NULL;

	if (f->type != V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE)
		return -EINVAL;

	if (pix->pixelformat == V4L2_PIX_FMT_JPEG) {
		fimc_capture_try_format(ctx, &pix->width, &pix->height,
					NULL, &pix->pixelformat,
					FIMC_SD_PAD_SINK);
		ctx->s_frame.f_width  = pix->width;
		ctx->s_frame.f_height = pix->height;
	}
	ffmt = fimc_capture_try_format(ctx, &pix->width, &pix->height,
				       NULL, &pix->pixelformat,
				       FIMC_SD_PAD_SOURCE);
	if (!ffmt)
		return -EINVAL;

	if (!fimc->vid_cap.user_subdev_api) {
		mf.width  = pix->width;
		mf.height = pix->height;
		mf.code   = ffmt->mbus_code;
		fimc_md_graph_lock(fimc);
		fimc_pipeline_try_format(ctx, &mf, &ffmt, false);
		fimc_md_graph_unlock(fimc);

		pix->width	 = mf.width;
		pix->height	 = mf.height;
		if (ffmt)
			pix->pixelformat = ffmt->fourcc;
	}

	fimc_adjust_mplane_format(ffmt, pix->width, pix->height, pix);
	return 0;
}

static void fimc_capture_mark_jpeg_xfer(struct fimc_ctx *ctx, bool jpeg)
{
	ctx->scaler.enabled = !jpeg;
	fimc_ctrls_activate(ctx, !jpeg);

	if (jpeg)
		set_bit(ST_CAPT_JPEG, &ctx->fimc_dev->state);
	else
		clear_bit(ST_CAPT_JPEG, &ctx->fimc_dev->state);
}

static int fimc_capture_set_format(struct fimc_dev *fimc, struct v4l2_format *f)
{
	struct fimc_ctx *ctx = fimc->vid_cap.ctx;
	struct v4l2_pix_format_mplane *pix = &f->fmt.pix_mp;
	struct v4l2_mbus_framefmt *mf = &fimc->vid_cap.mf;
	struct fimc_frame *ff = &ctx->d_frame;
	struct fimc_fmt *s_fmt = NULL;
	int ret, i;

	if (f->type != V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE)
		return -EINVAL;
	if (vb2_is_busy(&fimc->vid_cap.vbq))
		return -EBUSY;

	/* Pre-configure format at camera interface input, for JPEG only */
	if (pix->pixelformat == V4L2_PIX_FMT_JPEG) {
		fimc_capture_try_format(ctx, &pix->width, &pix->height,
					NULL, &pix->pixelformat,
					FIMC_SD_PAD_SINK);
		ctx->s_frame.f_width  = pix->width;
		ctx->s_frame.f_height = pix->height;
	}
	/* Try the format at the scaler and the DMA output */
	ff->fmt = fimc_capture_try_format(ctx, &pix->width, &pix->height,
					  NULL, &pix->pixelformat,
					  FIMC_SD_PAD_SOURCE);
	if (!ff->fmt)
		return -EINVAL;

	/* Update RGB Alpha control state and value range */
	fimc_alpha_ctrl_update(ctx);

	/* Try to match format at the host and the sensor */
	if (!fimc->vid_cap.user_subdev_api) {
		mf->code   = ff->fmt->mbus_code;
		mf->width  = pix->width;
		mf->height = pix->height;

		fimc_md_graph_lock(fimc);
		ret = fimc_pipeline_try_format(ctx, mf, &s_fmt, true);
		fimc_md_graph_unlock(fimc);
		if (ret)
			return ret;
		pix->width  = mf->width;
		pix->height = mf->height;
	}
	fimc_adjust_mplane_format(ff->fmt, pix->width, pix->height, pix);
	for (i = 0; i < ff->fmt->colplanes; i++)
		ff->payload[i] =
			(pix->width * pix->height * ff->fmt->depth[i]) / 8;

	set_frame_bounds(ff, pix->width, pix->height);
	/* Reset the composition rectangle if not yet configured */
	if (!(ctx->state & FIMC_DST_CROP))
		set_frame_crop(ff, 0, 0, pix->width, pix->height);

	fimc_capture_mark_jpeg_xfer(ctx, fimc_fmt_is_jpeg(ff->fmt->color));

	/* Reset cropping and set format at the camera interface input */
	if (!fimc->vid_cap.user_subdev_api) {
		ctx->s_frame.fmt = s_fmt;
		set_frame_bounds(&ctx->s_frame, pix->width, pix->height);
		set_frame_crop(&ctx->s_frame, 0, 0, pix->width, pix->height);
	}

	return ret;
}

static int fimc_cap_s_fmt_mplane(struct file *file, void *priv,
				 struct v4l2_format *f)
{
	struct fimc_dev *fimc = video_drvdata(file);

	return fimc_capture_set_format(fimc, f);
}

static int fimc_cap_enum_input(struct file *file, void *priv,
			       struct v4l2_input *i)
{
	struct fimc_dev *fimc = video_drvdata(file);
	struct v4l2_subdev *sd = fimc->pipeline.sensor;

	if (i->index != 0)
		return -EINVAL;

	i->type = V4L2_INPUT_TYPE_CAMERA;
	if (sd)
		strlcpy(i->name, sd->name, sizeof(i->name));
	return 0;
}

static int fimc_cap_s_input(struct file *file, void *priv, unsigned int i)
{
	return i == 0 ? i : -EINVAL;
}

static int fimc_cap_g_input(struct file *file, void *priv, unsigned int *i)
{
	*i = 0;
	return 0;
}

/**
 * fimc_pipeline_validate - check for formats inconsistencies
 *                          between source and sink pad of each link
 *
 * Return 0 if all formats match or -EPIPE otherwise.
 */
static int fimc_pipeline_validate(struct fimc_dev *fimc)
{
	struct v4l2_subdev_format sink_fmt, src_fmt;
	struct fimc_vid_cap *vid_cap = &fimc->vid_cap;
	struct v4l2_subdev *sd;
	struct media_pad *pad;
	int ret;

	/* Start with the video capture node pad */
	pad = media_entity_remote_source(&vid_cap->vd_pad);
	if (pad == NULL)
		return -EPIPE;
	/* FIMC.{N} subdevice */
	sd = media_entity_to_v4l2_subdev(pad->entity);

	while (1) {
		/* Retrieve format at the sink pad */
		pad = &sd->entity.pads[0];
		if (!(pad->flags & MEDIA_PAD_FL_SINK))
			break;
		/* Don't call FIMC subdev operation to avoid nested locking */
		if (sd == fimc->vid_cap.subdev) {
			struct fimc_frame *ff = &vid_cap->ctx->s_frame;
			sink_fmt.format.width = ff->f_width;
			sink_fmt.format.height = ff->f_height;
			sink_fmt.format.code = ff->fmt ? ff->fmt->mbus_code : 0;
		} else {
			sink_fmt.pad = pad->index;
			sink_fmt.which = V4L2_SUBDEV_FORMAT_ACTIVE;
			ret = v4l2_subdev_call(sd, pad, get_fmt, NULL, &sink_fmt);
			if (ret < 0 && ret != -ENOIOCTLCMD)
				return -EPIPE;
		}
		/* Retrieve format at the source pad */
		pad = media_entity_remote_source(pad);
		if (pad == NULL ||
		    media_entity_type(pad->entity) != MEDIA_ENT_T_V4L2_SUBDEV)
			break;

		sd = media_entity_to_v4l2_subdev(pad->entity);
		src_fmt.pad = pad->index;
		src_fmt.which = V4L2_SUBDEV_FORMAT_ACTIVE;
		ret = v4l2_subdev_call(sd, pad, get_fmt, NULL, &src_fmt);
		if (ret < 0 && ret != -ENOIOCTLCMD)
			return -EPIPE;

		if (src_fmt.format.width != sink_fmt.format.width ||
		    src_fmt.format.height != sink_fmt.format.height ||
		    src_fmt.format.code != sink_fmt.format.code)
			return -EPIPE;
	}
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static int fimc_cap_streamon(struct file *file, void *priv,
			     enum v4l2_buf_type type)
{
<<<<<<< HEAD
	struct fimc_ctx *ctx = priv;
	struct fimc_dev *fimc = ctx->fimc_dev;

	if (fimc_capture_active(fimc) || !fimc->vid_cap.sd)
		return -EBUSY;

	if (!(ctx->state & FIMC_DST_FMT)) {
		v4l2_err(&fimc->vid_cap.v4l2_dev, "Format is not set\n");
		return -EINVAL;
	}

=======
	struct fimc_dev *fimc = video_drvdata(file);
	struct fimc_pipeline *p = &fimc->pipeline;
	int ret;

	if (fimc_capture_active(fimc))
		return -EBUSY;

	media_entity_pipeline_start(&p->sensor->entity, p->pipe);

	if (fimc->vid_cap.user_subdev_api) {
		ret = fimc_pipeline_validate(fimc);
		if (ret)
			return ret;
	}
>>>>>>> refs/remotes/origin/cm-10.0
	return vb2_streamon(&fimc->vid_cap.vbq, type);
}

static int fimc_cap_streamoff(struct file *file, void *priv,
			    enum v4l2_buf_type type)
{
<<<<<<< HEAD
	struct fimc_ctx *ctx = priv;
	struct fimc_dev *fimc = ctx->fimc_dev;

	return vb2_streamoff(&fimc->vid_cap.vbq, type);
=======
	struct fimc_dev *fimc = video_drvdata(file);
	struct v4l2_subdev *sd = fimc->pipeline.sensor;
	int ret;

	ret = vb2_streamoff(&fimc->vid_cap.vbq, type);
	if (ret == 0)
		media_entity_pipeline_stop(&sd->entity);
	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int fimc_cap_reqbufs(struct file *file, void *priv,
			    struct v4l2_requestbuffers *reqbufs)
{
<<<<<<< HEAD
	struct fimc_ctx *ctx = priv;
	struct fimc_vid_cap *cap = &ctx->fimc_dev->vid_cap;
	int ret;


	ret = vb2_reqbufs(&cap->vbq, reqbufs);
	if (!ret)
		cap->reqbufs_count = reqbufs->count;

=======
	struct fimc_dev *fimc = video_drvdata(file);
	int ret = vb2_reqbufs(&fimc->vid_cap.vbq, reqbufs);

	if (!ret)
		fimc->vid_cap.reqbufs_count = reqbufs->count;
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static int fimc_cap_querybuf(struct file *file, void *priv,
			   struct v4l2_buffer *buf)
{
<<<<<<< HEAD
	struct fimc_ctx *ctx = priv;
	struct fimc_vid_cap *cap = &ctx->fimc_dev->vid_cap;

	return vb2_querybuf(&cap->vbq, buf);
=======
	struct fimc_dev *fimc = video_drvdata(file);

	return vb2_querybuf(&fimc->vid_cap.vbq, buf);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int fimc_cap_qbuf(struct file *file, void *priv,
			  struct v4l2_buffer *buf)
{
<<<<<<< HEAD
	struct fimc_ctx *ctx = priv;
	struct fimc_vid_cap *cap = &ctx->fimc_dev->vid_cap;
	return vb2_qbuf(&cap->vbq, buf);
=======
	struct fimc_dev *fimc = video_drvdata(file);

	return vb2_qbuf(&fimc->vid_cap.vbq, buf);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int fimc_cap_dqbuf(struct file *file, void *priv,
			   struct v4l2_buffer *buf)
{
<<<<<<< HEAD
	struct fimc_ctx *ctx = priv;
	return vb2_dqbuf(&ctx->fimc_dev->vid_cap.vbq, buf,
		file->f_flags & O_NONBLOCK);
}

static int fimc_cap_s_ctrl(struct file *file, void *priv,
			 struct v4l2_control *ctrl)
{
	struct fimc_ctx *ctx = priv;
	int ret = -EINVAL;

	/* Allow any controls but 90/270 rotation while streaming */
	if (!fimc_capture_active(ctx->fimc_dev) ||
	    ctrl->id != V4L2_CID_ROTATE ||
	    (ctrl->value != 90 && ctrl->value != 270)) {
		ret = check_ctrl_val(ctx, ctrl);
		if (!ret) {
			ret = fimc_s_ctrl(ctx, ctrl);
			if (!ret)
				ctx->state |= FIMC_PARAMS;
		}
	}
	if (ret == -EINVAL)
		ret = v4l2_subdev_call(ctx->fimc_dev->vid_cap.sd,
				       core, s_ctrl, ctrl);
	return ret;
}

static int fimc_cap_cropcap(struct file *file, void *fh,
			    struct v4l2_cropcap *cr)
{
	struct fimc_frame *f;
	struct fimc_ctx *ctx = fh;

	if (cr->type != V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE)
		return -EINVAL;

	f = &ctx->s_frame;

	cr->bounds.left		= 0;
	cr->bounds.top		= 0;
	cr->bounds.width	= f->o_width;
	cr->bounds.height	= f->o_height;
	cr->defrect		= cr->bounds;

	return 0;
}

static int fimc_cap_g_crop(struct file *file, void *fh, struct v4l2_crop *cr)
{
	struct fimc_frame *f;
	struct fimc_ctx *ctx = file->private_data;

	f = &ctx->s_frame;

	cr->c.left	= f->offs_h;
	cr->c.top	= f->offs_v;
	cr->c.width	= f->width;
	cr->c.height	= f->height;

	return 0;
}

static int fimc_cap_s_crop(struct file *file, void *fh,
			       struct v4l2_crop *cr)
{
	struct fimc_frame *f;
	struct fimc_ctx *ctx = file->private_data;
	struct fimc_dev *fimc = ctx->fimc_dev;
	int ret = -EINVAL;

	if (fimc_capture_active(fimc))
		return -EBUSY;

	ret = fimc_try_crop(ctx, cr);
	if (ret)
		return ret;

	if (!(ctx->state & FIMC_DST_FMT)) {
		v4l2_err(&fimc->vid_cap.v4l2_dev,
			 "Capture color format not set\n");
		return -EINVAL; /* TODO: make sure this is the right value */
	}

	f = &ctx->s_frame;
	/* Check for the pixel scaling ratio when cropping input image. */
	ret = fimc_check_scaler_ratio(cr->c.width, cr->c.height,
				      ctx->d_frame.width, ctx->d_frame.height,
				      ctx->rotation);
	if (ret) {
		v4l2_err(&fimc->vid_cap.v4l2_dev, "Out of the scaler range\n");
		return ret;
	}

	f->offs_h = cr->c.left;
	f->offs_v = cr->c.top;
	f->width  = cr->c.width;
	f->height = cr->c.height;

	return 0;
}


static const struct v4l2_ioctl_ops fimc_capture_ioctl_ops = {
	.vidioc_querycap		= fimc_vidioc_querycap_capture,

	.vidioc_enum_fmt_vid_cap_mplane	= fimc_vidioc_enum_fmt_mplane,
	.vidioc_try_fmt_vid_cap_mplane	= fimc_vidioc_try_fmt_mplane,
	.vidioc_s_fmt_vid_cap_mplane	= fimc_cap_s_fmt_mplane,
	.vidioc_g_fmt_vid_cap_mplane	= fimc_vidioc_g_fmt_mplane,
=======
	struct fimc_dev *fimc = video_drvdata(file);

	return vb2_dqbuf(&fimc->vid_cap.vbq, buf, file->f_flags & O_NONBLOCK);
}

static int fimc_cap_create_bufs(struct file *file, void *priv,
				struct v4l2_create_buffers *create)
{
	struct fimc_dev *fimc = video_drvdata(file);

	return vb2_create_bufs(&fimc->vid_cap.vbq, create);
}

static int fimc_cap_prepare_buf(struct file *file, void *priv,
				struct v4l2_buffer *b)
{
	struct fimc_dev *fimc = video_drvdata(file);

	return vb2_prepare_buf(&fimc->vid_cap.vbq, b);
}

static int fimc_cap_g_selection(struct file *file, void *fh,
				struct v4l2_selection *s)
{
	struct fimc_dev *fimc = video_drvdata(file);
	struct fimc_ctx *ctx = fimc->vid_cap.ctx;
	struct fimc_frame *f = &ctx->s_frame;

	if (s->type != V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE)
		return -EINVAL;

	switch (s->target) {
	case V4L2_SEL_TGT_COMPOSE_DEFAULT:
	case V4L2_SEL_TGT_COMPOSE_BOUNDS:
		f = &ctx->d_frame;
	case V4L2_SEL_TGT_CROP_BOUNDS:
	case V4L2_SEL_TGT_CROP_DEFAULT:
		s->r.left = 0;
		s->r.top = 0;
		s->r.width = f->o_width;
		s->r.height = f->o_height;
		return 0;

	case V4L2_SEL_TGT_COMPOSE_ACTIVE:
		f = &ctx->d_frame;
	case V4L2_SEL_TGT_CROP_ACTIVE:
		s->r.left = f->offs_h;
		s->r.top = f->offs_v;
		s->r.width = f->width;
		s->r.height = f->height;
		return 0;
	}

	return -EINVAL;
}

/* Return 1 if rectangle a is enclosed in rectangle b, or 0 otherwise. */
int enclosed_rectangle(struct v4l2_rect *a, struct v4l2_rect *b)
{
	if (a->left < b->left || a->top < b->top)
		return 0;
	if (a->left + a->width > b->left + b->width)
		return 0;
	if (a->top + a->height > b->top + b->height)
		return 0;

	return 1;
}

static int fimc_cap_s_selection(struct file *file, void *fh,
				struct v4l2_selection *s)
{
	struct fimc_dev *fimc = video_drvdata(file);
	struct fimc_ctx *ctx = fimc->vid_cap.ctx;
	struct v4l2_rect rect = s->r;
	struct fimc_frame *f;
	unsigned long flags;
	unsigned int pad;

	if (s->type != V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE)
		return -EINVAL;

	switch (s->target) {
	case V4L2_SEL_TGT_COMPOSE_DEFAULT:
	case V4L2_SEL_TGT_COMPOSE_BOUNDS:
	case V4L2_SEL_TGT_COMPOSE_ACTIVE:
		f = &ctx->d_frame;
		pad = FIMC_SD_PAD_SOURCE;
		break;
	case V4L2_SEL_TGT_CROP_BOUNDS:
	case V4L2_SEL_TGT_CROP_DEFAULT:
	case V4L2_SEL_TGT_CROP_ACTIVE:
		f = &ctx->s_frame;
		pad = FIMC_SD_PAD_SINK;
		break;
	default:
		return -EINVAL;
	}

	fimc_capture_try_crop(ctx, &rect, pad);

	if (s->flags & V4L2_SEL_FLAG_LE &&
	    !enclosed_rectangle(&rect, &s->r))
		return -ERANGE;

	if (s->flags & V4L2_SEL_FLAG_GE &&
	    !enclosed_rectangle(&s->r, &rect))
		return -ERANGE;

	s->r = rect;
	spin_lock_irqsave(&fimc->slock, flags);
	set_frame_crop(f, s->r.left, s->r.top, s->r.width,
		       s->r.height);
	spin_unlock_irqrestore(&fimc->slock, flags);

	set_bit(ST_CAPT_APPLY_CFG, &fimc->state);
	return 0;
}

static const struct v4l2_ioctl_ops fimc_capture_ioctl_ops = {
	.vidioc_querycap		= fimc_vidioc_querycap_capture,

	.vidioc_enum_fmt_vid_cap_mplane	= fimc_cap_enum_fmt_mplane,
	.vidioc_try_fmt_vid_cap_mplane	= fimc_cap_try_fmt_mplane,
	.vidioc_s_fmt_vid_cap_mplane	= fimc_cap_s_fmt_mplane,
	.vidioc_g_fmt_vid_cap_mplane	= fimc_cap_g_fmt_mplane,
>>>>>>> refs/remotes/origin/cm-10.0

	.vidioc_reqbufs			= fimc_cap_reqbufs,
	.vidioc_querybuf		= fimc_cap_querybuf,

	.vidioc_qbuf			= fimc_cap_qbuf,
	.vidioc_dqbuf			= fimc_cap_dqbuf,

<<<<<<< HEAD
	.vidioc_streamon		= fimc_cap_streamon,
	.vidioc_streamoff		= fimc_cap_streamoff,

	.vidioc_queryctrl		= fimc_vidioc_queryctrl,
	.vidioc_g_ctrl			= fimc_vidioc_g_ctrl,
	.vidioc_s_ctrl			= fimc_cap_s_ctrl,

	.vidioc_g_crop			= fimc_cap_g_crop,
	.vidioc_s_crop			= fimc_cap_s_crop,
	.vidioc_cropcap			= fimc_cap_cropcap,
=======
	.vidioc_prepare_buf		= fimc_cap_prepare_buf,
	.vidioc_create_bufs		= fimc_cap_create_bufs,

	.vidioc_streamon		= fimc_cap_streamon,
	.vidioc_streamoff		= fimc_cap_streamoff,

	.vidioc_g_selection		= fimc_cap_g_selection,
	.vidioc_s_selection		= fimc_cap_s_selection,
>>>>>>> refs/remotes/origin/cm-10.0

	.vidioc_enum_input		= fimc_cap_enum_input,
	.vidioc_s_input			= fimc_cap_s_input,
	.vidioc_g_input			= fimc_cap_g_input,
};

<<<<<<< HEAD
/* fimc->lock must be already initialized */
int fimc_register_capture_device(struct fimc_dev *fimc)
{
	struct v4l2_device *v4l2_dev = &fimc->vid_cap.v4l2_dev;
	struct video_device *vfd;
	struct fimc_vid_cap *vid_cap;
	struct fimc_ctx *ctx;
	struct v4l2_format f;
	struct fimc_frame *fr;
	struct vb2_queue *q;
	int ret;
=======
/* Capture subdev media entity operations */
static int fimc_link_setup(struct media_entity *entity,
			   const struct media_pad *local,
			   const struct media_pad *remote, u32 flags)
{
	struct v4l2_subdev *sd = media_entity_to_v4l2_subdev(entity);
	struct fimc_dev *fimc = v4l2_get_subdevdata(sd);

	if (media_entity_type(remote->entity) != MEDIA_ENT_T_V4L2_SUBDEV)
		return -EINVAL;

	if (WARN_ON(fimc == NULL))
		return 0;

	dbg("%s --> %s, flags: 0x%x. input: 0x%x",
	    local->entity->name, remote->entity->name, flags,
	    fimc->vid_cap.input);

	if (flags & MEDIA_LNK_FL_ENABLED) {
		if (fimc->vid_cap.input != 0)
			return -EBUSY;
		fimc->vid_cap.input = sd->grp_id;
		return 0;
	}

	fimc->vid_cap.input = 0;
	return 0;
}

static const struct media_entity_operations fimc_sd_media_ops = {
	.link_setup = fimc_link_setup,
};

/**
 * fimc_sensor_notify - v4l2_device notification from a sensor subdev
 * @sd: pointer to a subdev generating the notification
 * @notification: the notification type, must be S5P_FIMC_TX_END_NOTIFY
 * @arg: pointer to an u32 type integer that stores the frame payload value
 *
 * The End Of Frame notification sent by sensor subdev in its still capture
 * mode. If there is only a single VSYNC generated by the sensor at the
 * beginning of a frame transmission, FIMC does not issue the LastIrq
 * (end of frame) interrupt. And this notification is used to complete the
 * frame capture and returning a buffer to user-space. Subdev drivers should
 * call this notification from their last 'End of frame capture' interrupt.
 */
void fimc_sensor_notify(struct v4l2_subdev *sd, unsigned int notification,
			void *arg)
{
	struct fimc_sensor_info	*sensor;
	struct fimc_vid_buffer *buf;
	struct fimc_md *fmd;
	struct fimc_dev *fimc;
	unsigned long flags;

	if (sd == NULL)
		return;

	sensor = v4l2_get_subdev_hostdata(sd);
	fmd = entity_to_fimc_mdev(&sd->entity);

	spin_lock_irqsave(&fmd->slock, flags);
	fimc = sensor ? sensor->host : NULL;

	if (fimc && arg && notification == S5P_FIMC_TX_END_NOTIFY &&
	    test_bit(ST_CAPT_PEND, &fimc->state)) {
		unsigned long irq_flags;
		spin_lock_irqsave(&fimc->slock, irq_flags);
		if (!list_empty(&fimc->vid_cap.active_buf_q)) {
			buf = list_entry(fimc->vid_cap.active_buf_q.next,
					 struct fimc_vid_buffer, list);
			vb2_set_plane_payload(&buf->vb, 0, *((u32 *)arg));
		}
		fimc_capture_irq_handler(fimc, true);
		fimc_deactivate_capture(fimc);
		spin_unlock_irqrestore(&fimc->slock, irq_flags);
	}
	spin_unlock_irqrestore(&fmd->slock, flags);
}

static int fimc_subdev_enum_mbus_code(struct v4l2_subdev *sd,
				      struct v4l2_subdev_fh *fh,
				      struct v4l2_subdev_mbus_code_enum *code)
{
	struct fimc_fmt *fmt;

	fmt = fimc_find_format(NULL, NULL, FMT_FLAGS_CAM, code->index);
	if (!fmt)
		return -EINVAL;
	code->code = fmt->mbus_code;
	return 0;
}

static int fimc_subdev_get_fmt(struct v4l2_subdev *sd,
			       struct v4l2_subdev_fh *fh,
			       struct v4l2_subdev_format *fmt)
{
	struct fimc_dev *fimc = v4l2_get_subdevdata(sd);
	struct fimc_ctx *ctx = fimc->vid_cap.ctx;
	struct v4l2_mbus_framefmt *mf;
	struct fimc_frame *ff;

	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY) {
		mf = v4l2_subdev_get_try_format(fh, fmt->pad);
		fmt->format = *mf;
		return 0;
	}
	mf = &fmt->format;
	mf->colorspace = V4L2_COLORSPACE_JPEG;
	ff = fmt->pad == FIMC_SD_PAD_SINK ? &ctx->s_frame : &ctx->d_frame;

	mutex_lock(&fimc->lock);
	/* The pixel code is same on both input and output pad */
	if (!WARN_ON(ctx->s_frame.fmt == NULL))
		mf->code = ctx->s_frame.fmt->mbus_code;
	mf->width  = ff->f_width;
	mf->height = ff->f_height;
	mutex_unlock(&fimc->lock);

	return 0;
}

static int fimc_subdev_set_fmt(struct v4l2_subdev *sd,
			       struct v4l2_subdev_fh *fh,
			       struct v4l2_subdev_format *fmt)
{
	struct fimc_dev *fimc = v4l2_get_subdevdata(sd);
	struct v4l2_mbus_framefmt *mf = &fmt->format;
	struct fimc_ctx *ctx = fimc->vid_cap.ctx;
	struct fimc_frame *ff;
	struct fimc_fmt *ffmt;

	dbg("pad%d: code: 0x%x, %dx%d",
	    fmt->pad, mf->code, mf->width, mf->height);

	if (fmt->pad == FIMC_SD_PAD_SOURCE &&
	    vb2_is_busy(&fimc->vid_cap.vbq))
		return -EBUSY;

	mutex_lock(&fimc->lock);
	ffmt = fimc_capture_try_format(ctx, &mf->width, &mf->height,
				       &mf->code, NULL, fmt->pad);
	mutex_unlock(&fimc->lock);
	mf->colorspace = V4L2_COLORSPACE_JPEG;

	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY) {
		mf = v4l2_subdev_get_try_format(fh, fmt->pad);
		*mf = fmt->format;
		return 0;
	}
	/* Update RGB Alpha control state and value range */
	fimc_alpha_ctrl_update(ctx);

	fimc_capture_mark_jpeg_xfer(ctx, fimc_fmt_is_jpeg(ffmt->color));

	ff = fmt->pad == FIMC_SD_PAD_SINK ?
		&ctx->s_frame : &ctx->d_frame;

	mutex_lock(&fimc->lock);
	set_frame_bounds(ff, mf->width, mf->height);
	fimc->vid_cap.mf = *mf;
	ff->fmt = ffmt;

	/* Reset the crop rectangle if required. */
	if (!(fmt->pad == FIMC_SD_PAD_SOURCE && (ctx->state & FIMC_DST_CROP)))
		set_frame_crop(ff, 0, 0, mf->width, mf->height);

	if (fmt->pad == FIMC_SD_PAD_SINK)
		ctx->state &= ~FIMC_DST_CROP;
	mutex_unlock(&fimc->lock);
	return 0;
}

static int fimc_subdev_get_crop(struct v4l2_subdev *sd,
				struct v4l2_subdev_fh *fh,
				struct v4l2_subdev_crop *crop)
{
	struct fimc_dev *fimc = v4l2_get_subdevdata(sd);
	struct fimc_ctx *ctx = fimc->vid_cap.ctx;
	struct v4l2_rect *r = &crop->rect;
	struct fimc_frame *ff;

	if (crop->which == V4L2_SUBDEV_FORMAT_TRY) {
		crop->rect = *v4l2_subdev_get_try_crop(fh, crop->pad);
		return 0;
	}
	ff = crop->pad == FIMC_SD_PAD_SINK ?
		&ctx->s_frame : &ctx->d_frame;

	mutex_lock(&fimc->lock);
	r->left	  = ff->offs_h;
	r->top	  = ff->offs_v;
	r->width  = ff->width;
	r->height = ff->height;
	mutex_unlock(&fimc->lock);

	dbg("ff:%p, pad%d: l:%d, t:%d, %dx%d, f_w: %d, f_h: %d",
	    ff, crop->pad, r->left, r->top, r->width, r->height,
	    ff->f_width, ff->f_height);

	return 0;
}

static int fimc_subdev_set_crop(struct v4l2_subdev *sd,
				struct v4l2_subdev_fh *fh,
				struct v4l2_subdev_crop *crop)
{
	struct fimc_dev *fimc = v4l2_get_subdevdata(sd);
	struct fimc_ctx *ctx = fimc->vid_cap.ctx;
	struct v4l2_rect *r = &crop->rect;
	struct fimc_frame *ff;
	unsigned long flags;

	dbg("(%d,%d)/%dx%d", r->left, r->top, r->width, r->height);

	ff = crop->pad == FIMC_SD_PAD_SOURCE ?
		&ctx->d_frame : &ctx->s_frame;

	mutex_lock(&fimc->lock);
	fimc_capture_try_crop(ctx, r, crop->pad);

	if (crop->which == V4L2_SUBDEV_FORMAT_TRY) {
		mutex_unlock(&fimc->lock);
		*v4l2_subdev_get_try_crop(fh, crop->pad) = *r;
		return 0;
	}
	spin_lock_irqsave(&fimc->slock, flags);
	set_frame_crop(ff, r->left, r->top, r->width, r->height);
	if (crop->pad == FIMC_SD_PAD_SOURCE)
		ctx->state |= FIMC_DST_CROP;

	set_bit(ST_CAPT_APPLY_CFG, &fimc->state);
	spin_unlock_irqrestore(&fimc->slock, flags);

	dbg("pad%d: (%d,%d)/%dx%d", crop->pad, r->left, r->top,
	    r->width, r->height);

	mutex_unlock(&fimc->lock);
	return 0;
}

static struct v4l2_subdev_pad_ops fimc_subdev_pad_ops = {
	.enum_mbus_code = fimc_subdev_enum_mbus_code,
	.get_fmt = fimc_subdev_get_fmt,
	.set_fmt = fimc_subdev_set_fmt,
	.get_crop = fimc_subdev_get_crop,
	.set_crop = fimc_subdev_set_crop,
};

static struct v4l2_subdev_ops fimc_subdev_ops = {
	.pad = &fimc_subdev_pad_ops,
};

static int fimc_create_capture_subdev(struct fimc_dev *fimc,
				      struct v4l2_device *v4l2_dev)
{
	struct v4l2_subdev *sd;
	int ret;

	sd = kzalloc(sizeof(*sd), GFP_KERNEL);
	if (!sd)
		return -ENOMEM;

	v4l2_subdev_init(sd, &fimc_subdev_ops);
	sd->flags = V4L2_SUBDEV_FL_HAS_DEVNODE;
	snprintf(sd->name, sizeof(sd->name), "FIMC.%d", fimc->pdev->id);

	fimc->vid_cap.sd_pads[FIMC_SD_PAD_SINK].flags = MEDIA_PAD_FL_SINK;
	fimc->vid_cap.sd_pads[FIMC_SD_PAD_SOURCE].flags = MEDIA_PAD_FL_SOURCE;
	ret = media_entity_init(&sd->entity, FIMC_SD_PADS_NUM,
				fimc->vid_cap.sd_pads, 0);
	if (ret)
		goto me_err;
	ret = v4l2_device_register_subdev(v4l2_dev, sd);
	if (ret)
		goto sd_err;

	fimc->vid_cap.subdev = sd;
	v4l2_set_subdevdata(sd, fimc);
	sd->entity.ops = &fimc_sd_media_ops;
	return 0;
sd_err:
	media_entity_cleanup(&sd->entity);
me_err:
	kfree(sd);
	return ret;
}

static void fimc_destroy_capture_subdev(struct fimc_dev *fimc)
{
	struct v4l2_subdev *sd = fimc->vid_cap.subdev;

	if (!sd)
		return;
	media_entity_cleanup(&sd->entity);
	v4l2_device_unregister_subdev(sd);
	kfree(sd);
	fimc->vid_cap.subdev = NULL;
}

/* Set default format at the sensor and host interface */
static int fimc_capture_set_default_format(struct fimc_dev *fimc)
{
	struct v4l2_format fmt = {
		.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE,
		.fmt.pix_mp = {
			.width		= 640,
			.height		= 480,
			.pixelformat	= V4L2_PIX_FMT_YUYV,
			.field		= V4L2_FIELD_NONE,
			.colorspace	= V4L2_COLORSPACE_JPEG,
		},
	};

	return fimc_capture_set_format(fimc, &fmt);
}

/* fimc->lock must be already initialized */
int fimc_register_capture_device(struct fimc_dev *fimc,
				 struct v4l2_device *v4l2_dev)
{
	struct video_device *vfd;
	struct fimc_vid_cap *vid_cap;
	struct fimc_ctx *ctx;
	struct vb2_queue *q;
	int ret = -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0

	ctx = kzalloc(sizeof *ctx, GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->fimc_dev	 = fimc;
	ctx->in_path	 = FIMC_CAMERA;
	ctx->out_path	 = FIMC_DMA;
	ctx->state	 = FIMC_CTX_CAP;
<<<<<<< HEAD

	/* Default format of the output frames */
	f.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB32;
	fr = &ctx->d_frame;
	fr->fmt = find_format(&f, FMT_FLAGS_M2M);
	fr->width = fr->f_width = fr->o_width = 640;
	fr->height = fr->f_height = fr->o_height = 480;

	if (!v4l2_dev->name[0])
		snprintf(v4l2_dev->name, sizeof(v4l2_dev->name),
			 "%s.capture", dev_name(&fimc->pdev->dev));

	ret = v4l2_device_register(NULL, v4l2_dev);
	if (ret)
		goto err_info;
=======
	ctx->s_frame.fmt = fimc_find_format(NULL, NULL, FMT_FLAGS_CAM, 0);
	ctx->d_frame.fmt = fimc_find_format(NULL, NULL, FMT_FLAGS_CAM, 0);
>>>>>>> refs/remotes/origin/cm-10.0

	vfd = video_device_alloc();
	if (!vfd) {
		v4l2_err(v4l2_dev, "Failed to allocate video device\n");
<<<<<<< HEAD
		goto err_v4l2_reg;
	}

	snprintf(vfd->name, sizeof(vfd->name), "%s:cap",
=======
		goto err_vd_alloc;
	}

	snprintf(vfd->name, sizeof(vfd->name), "%s.capture",
>>>>>>> refs/remotes/origin/cm-10.0
		 dev_name(&fimc->pdev->dev));

	vfd->fops	= &fimc_capture_fops;
	vfd->ioctl_ops	= &fimc_capture_ioctl_ops;
<<<<<<< HEAD
=======
	vfd->v4l2_dev	= v4l2_dev;
>>>>>>> refs/remotes/origin/cm-10.0
	vfd->minor	= -1;
	vfd->release	= video_device_release;
	vfd->lock	= &fimc->lock;
	video_set_drvdata(vfd, fimc);

	vid_cap = &fimc->vid_cap;
	vid_cap->vfd = vfd;
	vid_cap->active_buf_cnt = 0;
	vid_cap->reqbufs_count  = 0;
	vid_cap->refcnt = 0;
<<<<<<< HEAD
	/* Default color format for image sensor */
	vid_cap->fmt.code = V4L2_MBUS_FMT_YUYV8_2X8;
=======
>>>>>>> refs/remotes/origin/cm-10.0

	INIT_LIST_HEAD(&vid_cap->pending_buf_q);
	INIT_LIST_HEAD(&vid_cap->active_buf_q);
	spin_lock_init(&ctx->slock);
	vid_cap->ctx = ctx;

	q = &fimc->vid_cap.vbq;
	memset(q, 0, sizeof(*q));
	q->type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	q->io_modes = VB2_MMAP | VB2_USERPTR;
	q->drv_priv = fimc->vid_cap.ctx;
	q->ops = &fimc_capture_qops;
	q->mem_ops = &vb2_dma_contig_memops;
	q->buf_struct_size = sizeof(struct fimc_vid_buffer);

	vb2_queue_init(q);

<<<<<<< HEAD
	ret = video_register_device(vfd, VFL_TYPE_GRABBER, -1);
	if (ret) {
		v4l2_err(v4l2_dev, "Failed to register video device\n");
		goto err_vd_reg;
	}

	v4l2_info(v4l2_dev,
		  "FIMC capture driver registered as /dev/video%d\n",
		  vfd->num);

	return 0;

err_vd_reg:
	video_device_release(vfd);
err_v4l2_reg:
	v4l2_device_unregister(v4l2_dev);
err_info:
	kfree(ctx);
	dev_err(&fimc->pdev->dev, "failed to install\n");
=======
	fimc->vid_cap.vd_pad.flags = MEDIA_PAD_FL_SINK;
	ret = media_entity_init(&vfd->entity, 1, &fimc->vid_cap.vd_pad, 0);
	if (ret)
		goto err_ent;
	ret = fimc_create_capture_subdev(fimc, v4l2_dev);
	if (ret)
		goto err_sd_reg;

	vfd->ctrl_handler = &ctx->ctrl_handler;
	return 0;

err_sd_reg:
	media_entity_cleanup(&vfd->entity);
err_ent:
	video_device_release(vfd);
err_vd_alloc:
	kfree(ctx);
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

void fimc_unregister_capture_device(struct fimc_dev *fimc)
{
<<<<<<< HEAD
	struct fimc_vid_cap *capture = &fimc->vid_cap;

	if (capture->vfd)
		video_unregister_device(capture->vfd);

	kfree(capture->ctx);
=======
	struct video_device *vfd = fimc->vid_cap.vfd;

	if (vfd) {
		media_entity_cleanup(&vfd->entity);
		/* Can also be called if video device was
		   not registered */
		video_unregister_device(vfd);
	}
	fimc_destroy_capture_subdev(fimc);
	kfree(fimc->vid_cap.ctx);
	fimc->vid_cap.ctx = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
}
