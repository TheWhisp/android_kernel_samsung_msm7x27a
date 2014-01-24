/*
 * Copyright (C) 2005-2006 Micronas USA Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (Version 2) as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */

#include <linux/module.h>
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/version.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/unistd.h>
#include <linux/time.h>
#include <linux/vmalloc.h>
#include <linux/pagemap.h>
<<<<<<< HEAD
=======
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
>>>>>>> refs/remotes/origin/master
#include <linux/videodev2.h>
#include <media/v4l2-common.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-subdev.h>
<<<<<<< HEAD
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>

#include "go7007.h"
#include "go7007-priv.h"
#include "wis-i2c.h"

/* Temporary defines until accepted in v4l-dvb */
#ifndef V4L2_MPEG_STREAM_TYPE_MPEG_ELEM
#define	V4L2_MPEG_STREAM_TYPE_MPEG_ELEM   6 /* MPEG elementary stream */
#endif
#ifndef V4L2_MPEG_VIDEO_ENCODING_MPEG_4
#define	V4L2_MPEG_VIDEO_ENCODING_MPEG_4   3
#endif
=======
#include <media/v4l2-event.h>
#include <media/videobuf2-vmalloc.h>
#include <media/saa7115.h>

#include "go7007.h"
#include "go7007-priv.h"
>>>>>>> refs/remotes/origin/master

#define call_all(dev, o, f, args...) \
	v4l2_device_call_until_err(dev, 0, o, f, ##args)

<<<<<<< HEAD
static void deactivate_buffer(struct go7007_buffer *gobuf)
{
	int i;

	if (gobuf->state != BUF_STATE_IDLE) {
		list_del(&gobuf->stream);
		gobuf->state = BUF_STATE_IDLE;
	}
	if (gobuf->page_count > 0) {
		for (i = 0; i < gobuf->page_count; ++i)
			page_cache_release(gobuf->pages[i]);
		gobuf->page_count = 0;
	}
}

static void abort_queued(struct go7007 *go)
{
	struct go7007_buffer *gobuf, *next;

	list_for_each_entry_safe(gobuf, next, &go->stream, stream) {
		deactivate_buffer(gobuf);
	}
}

static int go7007_streamoff(struct go7007 *go)
{
	int retval = -EINVAL;
	unsigned long flags;

	mutex_lock(&go->hw_lock);
	if (go->streaming) {
		go->streaming = 0;
		go7007_stream_stop(go);
		spin_lock_irqsave(&go->spinlock, flags);
		abort_queued(go);
		spin_unlock_irqrestore(&go->spinlock, flags);
		go7007_reset_encoder(go);
		retval = 0;
	}
	mutex_unlock(&go->hw_lock);
	return 0;
}

static int go7007_open(struct file *file)
{
	struct go7007 *go = video_get_drvdata(video_devdata(file));
	struct go7007_file *gofh;

	if (go->status != STATUS_ONLINE)
		return -EBUSY;
	gofh = kmalloc(sizeof(struct go7007_file), GFP_KERNEL);
	if (gofh == NULL)
		return -ENOMEM;
	++go->ref_count;
	gofh->go = go;
	mutex_init(&gofh->lock);
	gofh->buf_count = 0;
	file->private_data = gofh;
	return 0;
}

static int go7007_release(struct file *file)
{
	struct go7007_file *gofh = file->private_data;
	struct go7007 *go = gofh->go;

	if (gofh->buf_count > 0) {
		go7007_streamoff(go);
		go->in_use = 0;
		kfree(gofh->bufs);
		gofh->buf_count = 0;
	}
	kfree(gofh);
	if (--go->ref_count == 0)
		kfree(go);
	file->private_data = NULL;
	return 0;
}

static u32 get_frame_type_flag(struct go7007_buffer *gobuf, int format)
{
	u8 *f = page_address(gobuf->pages[0]);

	switch (format) {
	case GO7007_FORMAT_MJPEG:
		return V4L2_BUF_FLAG_KEYFRAME;
	case GO7007_FORMAT_MPEG4:
		switch ((f[gobuf->frame_offset + 4] >> 6) & 0x3) {
=======
static bool valid_pixelformat(u32 pixelformat)
{
	switch (pixelformat) {
	case V4L2_PIX_FMT_MJPEG:
	case V4L2_PIX_FMT_MPEG1:
	case V4L2_PIX_FMT_MPEG2:
	case V4L2_PIX_FMT_MPEG4:
		return true;
	default:
		return false;
	}
}

static u32 get_frame_type_flag(struct go7007_buffer *vb, int format)
{
	u8 *ptr = vb2_plane_vaddr(&vb->vb, 0);

	switch (format) {
	case V4L2_PIX_FMT_MJPEG:
		return V4L2_BUF_FLAG_KEYFRAME;
	case V4L2_PIX_FMT_MPEG4:
		switch ((ptr[vb->frame_offset + 4] >> 6) & 0x3) {
>>>>>>> refs/remotes/origin/master
		case 0:
			return V4L2_BUF_FLAG_KEYFRAME;
		case 1:
			return V4L2_BUF_FLAG_PFRAME;
		case 2:
			return V4L2_BUF_FLAG_BFRAME;
		default:
			return 0;
		}
<<<<<<< HEAD
	case GO7007_FORMAT_MPEG1:
	case GO7007_FORMAT_MPEG2:
		switch ((f[gobuf->frame_offset + 5] >> 3) & 0x7) {
=======
	case V4L2_PIX_FMT_MPEG1:
	case V4L2_PIX_FMT_MPEG2:
		switch ((ptr[vb->frame_offset + 5] >> 3) & 0x7) {
>>>>>>> refs/remotes/origin/master
		case 1:
			return V4L2_BUF_FLAG_KEYFRAME;
		case 2:
			return V4L2_BUF_FLAG_PFRAME;
		case 3:
			return V4L2_BUF_FLAG_BFRAME;
		default:
			return 0;
		}
	}

	return 0;
}

<<<<<<< HEAD
static int set_capture_size(struct go7007 *go, struct v4l2_format *fmt, int try)
{
	int sensor_height = 0, sensor_width = 0;
	int width, height, i;

	if (fmt != NULL && fmt->fmt.pix.pixelformat != V4L2_PIX_FMT_MJPEG &&
			fmt->fmt.pix.pixelformat != V4L2_PIX_FMT_MPEG &&
			fmt->fmt.pix.pixelformat != V4L2_PIX_FMT_MPEG4)
		return -EINVAL;

	switch (go->standard) {
	case GO7007_STD_NTSC:
		sensor_width = 720;
		sensor_height = 480;
		break;
	case GO7007_STD_PAL:
		sensor_width = 720;
		sensor_height = 576;
		break;
	case GO7007_STD_OTHER:
		sensor_width = go->board_info->sensor_width;
		sensor_height = go->board_info->sensor_height;
		break;
	}
=======
static void get_resolution(struct go7007 *go, int *width, int *height)
{
	switch (go->standard) {
	case GO7007_STD_NTSC:
		*width = 720;
		*height = 480;
		break;
	case GO7007_STD_PAL:
		*width = 720;
		*height = 576;
		break;
	case GO7007_STD_OTHER:
	default:
		*width = go->board_info->sensor_width;
		*height = go->board_info->sensor_height;
		break;
	}
}

static void set_formatting(struct go7007 *go)
{
	if (go->format == V4L2_PIX_FMT_MJPEG) {
		go->pali = 0;
		go->aspect_ratio = GO7007_RATIO_1_1;
		go->gop_size = 0;
		go->ipb = 0;
		go->closed_gop = 0;
		go->repeat_seqhead = 0;
		go->seq_header_enable = 0;
		go->gop_header_enable = 0;
		go->dvd_mode = 0;
		return;
	}

	switch (go->format) {
	case V4L2_PIX_FMT_MPEG1:
		go->pali = 0;
		break;
	default:
	case V4L2_PIX_FMT_MPEG2:
		go->pali = 0x48;
		break;
	case V4L2_PIX_FMT_MPEG4:
		/* For future reference: this is the list of MPEG4
		 * profiles that are available, although they are
		 * untested:
		 *
		 * Profile		pali
		 * --------------	----
		 * PROFILE_S_L0		0x08
		 * PROFILE_S_L1		0x01
		 * PROFILE_S_L2		0x02
		 * PROFILE_S_L3		0x03
		 * PROFILE_ARTS_L1	0x91
		 * PROFILE_ARTS_L2	0x92
		 * PROFILE_ARTS_L3	0x93
		 * PROFILE_ARTS_L4	0x94
		 * PROFILE_AS_L0	0xf0
		 * PROFILE_AS_L1	0xf1
		 * PROFILE_AS_L2	0xf2
		 * PROFILE_AS_L3	0xf3
		 * PROFILE_AS_L4	0xf4
		 * PROFILE_AS_L5	0xf5
		 */
		go->pali = 0xf5;
		break;
	}
	go->gop_size = v4l2_ctrl_g_ctrl(go->mpeg_video_gop_size);
	go->closed_gop = v4l2_ctrl_g_ctrl(go->mpeg_video_gop_closure);
	go->ipb = v4l2_ctrl_g_ctrl(go->mpeg_video_b_frames) != 0;
	go->bitrate = v4l2_ctrl_g_ctrl(go->mpeg_video_bitrate);
	go->repeat_seqhead = v4l2_ctrl_g_ctrl(go->mpeg_video_rep_seqheader);
	go->gop_header_enable = 1;
	go->dvd_mode = 0;
	if (go->format == V4L2_PIX_FMT_MPEG2)
		go->dvd_mode =
			go->bitrate == 9800000 &&
			go->gop_size == 15 &&
			go->ipb == 0 &&
			go->repeat_seqhead == 1 &&
			go->closed_gop;

	switch (v4l2_ctrl_g_ctrl(go->mpeg_video_aspect_ratio)) {
	default:
	case V4L2_MPEG_VIDEO_ASPECT_1x1:
		go->aspect_ratio = GO7007_RATIO_1_1;
		break;
	case V4L2_MPEG_VIDEO_ASPECT_4x3:
		go->aspect_ratio = GO7007_RATIO_4_3;
		break;
	case V4L2_MPEG_VIDEO_ASPECT_16x9:
		go->aspect_ratio = GO7007_RATIO_16_9;
		break;
	}
}

static int set_capture_size(struct go7007 *go, struct v4l2_format *fmt, int try)
{
	int sensor_height = 0, sensor_width = 0;
	int width, height, i;

	if (fmt != NULL && !valid_pixelformat(fmt->fmt.pix.pixelformat))
		return -EINVAL;

	get_resolution(go, &sensor_width, &sensor_height);
>>>>>>> refs/remotes/origin/master

	if (fmt == NULL) {
		width = sensor_width;
		height = sensor_height;
	} else if (go->board_info->sensor_flags & GO7007_SENSOR_SCALING) {
		if (fmt->fmt.pix.width > sensor_width)
			width = sensor_width;
		else if (fmt->fmt.pix.width < 144)
			width = 144;
		else
			width = fmt->fmt.pix.width & ~0x0f;

		if (fmt->fmt.pix.height > sensor_height)
			height = sensor_height;
		else if (fmt->fmt.pix.height < 96)
			height = 96;
		else
			height = fmt->fmt.pix.height & ~0x0f;
	} else {
<<<<<<< HEAD
		int requested_size = fmt->fmt.pix.width * fmt->fmt.pix.height;
		int sensor_size = sensor_width * sensor_height;

		if (64 * requested_size < 9 * sensor_size) {
			width = sensor_width / 4;
			height = sensor_height / 4;
		} else if (64 * requested_size < 36 * sensor_size) {
=======
		width = fmt->fmt.pix.width;

		if (width <= sensor_width / 4) {
			width = sensor_width / 4;
			height = sensor_height / 4;
		} else if (width <= sensor_width / 2) {
>>>>>>> refs/remotes/origin/master
			width = sensor_width / 2;
			height = sensor_height / 2;
		} else {
			width = sensor_width;
			height = sensor_height;
		}
		width &= ~0xf;
		height &= ~0xf;
	}

	if (fmt != NULL) {
		u32 pixelformat = fmt->fmt.pix.pixelformat;

		memset(fmt, 0, sizeof(*fmt));
		fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		fmt->fmt.pix.width = width;
		fmt->fmt.pix.height = height;
		fmt->fmt.pix.pixelformat = pixelformat;
		fmt->fmt.pix.field = V4L2_FIELD_NONE;
		fmt->fmt.pix.bytesperline = 0;
		fmt->fmt.pix.sizeimage = GO7007_BUF_SIZE;
<<<<<<< HEAD
		fmt->fmt.pix.colorspace = V4L2_COLORSPACE_SMPTE170M; /* ?? */
=======
		fmt->fmt.pix.colorspace = V4L2_COLORSPACE_SMPTE170M;
>>>>>>> refs/remotes/origin/master
	}

	if (try)
		return 0;

<<<<<<< HEAD
=======
	if (fmt)
		go->format = fmt->fmt.pix.pixelformat;
>>>>>>> refs/remotes/origin/master
	go->width = width;
	go->height = height;
	go->encoder_h_offset = go->board_info->sensor_h_offset;
	go->encoder_v_offset = go->board_info->sensor_v_offset;
	for (i = 0; i < 4; ++i)
		go->modet[i].enable = 0;
	for (i = 0; i < 1624; ++i)
		go->modet_map[i] = 0;

	if (go->board_info->sensor_flags & GO7007_SENSOR_SCALING) {
		struct v4l2_mbus_framefmt mbus_fmt;

		mbus_fmt.code = V4L2_MBUS_FMT_FIXED;
<<<<<<< HEAD
		if (fmt != NULL)
			mbus_fmt.width = fmt->fmt.pix.width;
		else
			mbus_fmt.width = width;

		if (height > sensor_height / 2) {
			mbus_fmt.height = height / 2;
			go->encoder_v_halve = 0;
		} else {
			mbus_fmt.height = height;
			go->encoder_v_halve = 1;
		}
=======
		mbus_fmt.width = fmt ? fmt->fmt.pix.width : width;
		mbus_fmt.height = height;
		go->encoder_h_halve = 0;
		go->encoder_v_halve = 0;
		go->encoder_subsample = 0;
>>>>>>> refs/remotes/origin/master
		call_all(&go->v4l2_dev, video, s_mbus_fmt, &mbus_fmt);
	} else {
		if (width <= sensor_width / 4) {
			go->encoder_h_halve = 1;
			go->encoder_v_halve = 1;
			go->encoder_subsample = 1;
		} else if (width <= sensor_width / 2) {
			go->encoder_h_halve = 1;
			go->encoder_v_halve = 1;
			go->encoder_subsample = 0;
		} else {
			go->encoder_h_halve = 0;
			go->encoder_v_halve = 0;
			go->encoder_subsample = 0;
		}
	}
<<<<<<< HEAD

	if (fmt == NULL)
		return 0;

	switch (fmt->fmt.pix.pixelformat) {
	case V4L2_PIX_FMT_MPEG:
		if (go->format == GO7007_FORMAT_MPEG1 ||
				go->format == GO7007_FORMAT_MPEG2 ||
				go->format == GO7007_FORMAT_MPEG4)
			break;
		go->format = GO7007_FORMAT_MPEG1;
		go->pali = 0;
		go->aspect_ratio = GO7007_RATIO_1_1;
		go->gop_size = go->sensor_framerate / 1000;
		go->ipb = 0;
		go->closed_gop = 1;
		go->repeat_seqhead = 1;
		go->seq_header_enable = 1;
		go->gop_header_enable = 1;
		go->dvd_mode = 0;
		break;
	/* Backwards compatibility only! */
	case V4L2_PIX_FMT_MPEG4:
		if (go->format == GO7007_FORMAT_MPEG4)
			break;
		go->format = GO7007_FORMAT_MPEG4;
		go->pali = 0xf5;
		go->aspect_ratio = GO7007_RATIO_1_1;
		go->gop_size = go->sensor_framerate / 1000;
		go->ipb = 0;
		go->closed_gop = 1;
		go->repeat_seqhead = 1;
		go->seq_header_enable = 1;
		go->gop_header_enable = 1;
		go->dvd_mode = 0;
		break;
	case V4L2_PIX_FMT_MJPEG:
		go->format = GO7007_FORMAT_MJPEG;
		go->pali = 0;
		go->aspect_ratio = GO7007_RATIO_1_1;
		go->gop_size = 0;
		go->ipb = 0;
		go->closed_gop = 0;
		go->repeat_seqhead = 0;
		go->seq_header_enable = 0;
		go->gop_header_enable = 0;
		go->dvd_mode = 0;
		break;
	}
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

#if 0
static int clip_to_modet_map(struct go7007 *go, int region,
		struct v4l2_clip *clip_list)
{
	struct v4l2_clip clip, *clip_ptr;
	int x, y, mbnum;

	/* Check if coordinates are OK and if any macroblocks are already
	 * used by other regions (besides 0) */
	clip_ptr = clip_list;
	while (clip_ptr) {
		if (copy_from_user(&clip, clip_ptr, sizeof(clip)))
			return -EFAULT;
		if (clip.c.left < 0 || (clip.c.left & 0xF) ||
				clip.c.width <= 0 || (clip.c.width & 0xF))
			return -EINVAL;
		if (clip.c.left + clip.c.width > go->width)
			return -EINVAL;
		if (clip.c.top < 0 || (clip.c.top & 0xF) ||
				clip.c.height <= 0 || (clip.c.height & 0xF))
			return -EINVAL;
		if (clip.c.top + clip.c.height > go->height)
			return -EINVAL;
		for (y = 0; y < clip.c.height; y += 16)
			for (x = 0; x < clip.c.width; x += 16) {
				mbnum = (go->width >> 4) *
						((clip.c.top + y) >> 4) +
					((clip.c.left + x) >> 4);
				if (go->modet_map[mbnum] != 0 &&
						go->modet_map[mbnum] != region)
					return -EBUSY;
			}
		clip_ptr = clip.next;
	}

	/* Clear old region macroblocks */
	for (mbnum = 0; mbnum < 1624; ++mbnum)
		if (go->modet_map[mbnum] == region)
			go->modet_map[mbnum] = 0;

	/* Claim macroblocks in this list */
	clip_ptr = clip_list;
	while (clip_ptr) {
		if (copy_from_user(&clip, clip_ptr, sizeof(clip)))
			return -EFAULT;
		for (y = 0; y < clip.c.height; y += 16)
			for (x = 0; x < clip.c.width; x += 16) {
				mbnum = (go->width >> 4) *
						((clip.c.top + y) >> 4) +
					((clip.c.left + x) >> 4);
				go->modet_map[mbnum] = region;
			}
		clip_ptr = clip.next;
	}
	return 0;
}
#endif

<<<<<<< HEAD
static int mpeg_query_ctrl(struct v4l2_queryctrl *ctrl)
{
	static const u32 mpeg_ctrls[] = {
		V4L2_CID_MPEG_CLASS,
		V4L2_CID_MPEG_STREAM_TYPE,
		V4L2_CID_MPEG_VIDEO_ENCODING,
		V4L2_CID_MPEG_VIDEO_ASPECT,
		V4L2_CID_MPEG_VIDEO_GOP_SIZE,
		V4L2_CID_MPEG_VIDEO_GOP_CLOSURE,
		V4L2_CID_MPEG_VIDEO_BITRATE,
		0
	};
	static const u32 *ctrl_classes[] = {
		mpeg_ctrls,
		NULL
	};

	ctrl->id = v4l2_ctrl_next(ctrl_classes, ctrl->id);

	switch (ctrl->id) {
	case V4L2_CID_MPEG_CLASS:
		return v4l2_ctrl_query_fill(ctrl, 0, 0, 0, 0);
	case V4L2_CID_MPEG_STREAM_TYPE:
		return v4l2_ctrl_query_fill(ctrl,
				V4L2_MPEG_STREAM_TYPE_MPEG2_DVD,
				V4L2_MPEG_STREAM_TYPE_MPEG_ELEM, 1,
				V4L2_MPEG_STREAM_TYPE_MPEG_ELEM);
	case V4L2_CID_MPEG_VIDEO_ENCODING:
		return v4l2_ctrl_query_fill(ctrl,
				V4L2_MPEG_VIDEO_ENCODING_MPEG_1,
				V4L2_MPEG_VIDEO_ENCODING_MPEG_4, 1,
				V4L2_MPEG_VIDEO_ENCODING_MPEG_2);
	case V4L2_CID_MPEG_VIDEO_ASPECT:
		return v4l2_ctrl_query_fill(ctrl,
				V4L2_MPEG_VIDEO_ASPECT_1x1,
				V4L2_MPEG_VIDEO_ASPECT_16x9, 1,
				V4L2_MPEG_VIDEO_ASPECT_1x1);
	case V4L2_CID_MPEG_VIDEO_GOP_SIZE:
		return v4l2_ctrl_query_fill(ctrl, 0, 34, 1, 15);
	case V4L2_CID_MPEG_VIDEO_GOP_CLOSURE:
		return v4l2_ctrl_query_fill(ctrl, 0, 1, 1, 0);
	case V4L2_CID_MPEG_VIDEO_BITRATE:
		return v4l2_ctrl_query_fill(ctrl,
				64000,
				10000000, 1,
				1500000);
	default:
		return -EINVAL;
	}
	return 0;
}

static int mpeg_s_ctrl(struct v4l2_control *ctrl, struct go7007 *go)
{
	/* pretty sure we can't change any of these while streaming */
	if (go->streaming)
		return -EBUSY;

	switch (ctrl->id) {
	case V4L2_CID_MPEG_STREAM_TYPE:
		switch (ctrl->value) {
		case V4L2_MPEG_STREAM_TYPE_MPEG2_DVD:
			go->format = GO7007_FORMAT_MPEG2;
			go->bitrate = 9800000;
			go->gop_size = 15;
			go->pali = 0x48;
			go->closed_gop = 1;
			go->repeat_seqhead = 0;
			go->seq_header_enable = 1;
			go->gop_header_enable = 1;
			go->dvd_mode = 1;
			break;
		case V4L2_MPEG_STREAM_TYPE_MPEG_ELEM:
			/* todo: */
			break;
		default:
			return -EINVAL;
		}
		break;
	case V4L2_CID_MPEG_VIDEO_ENCODING:
		switch (ctrl->value) {
		case V4L2_MPEG_VIDEO_ENCODING_MPEG_1:
			go->format = GO7007_FORMAT_MPEG1;
			go->pali = 0;
			break;
		case V4L2_MPEG_VIDEO_ENCODING_MPEG_2:
			go->format = GO7007_FORMAT_MPEG2;
			/*if (mpeg->pali >> 24 == 2)
				go->pali = mpeg->pali & 0xff;
			else*/
				go->pali = 0x48;
			break;
		case V4L2_MPEG_VIDEO_ENCODING_MPEG_4:
			go->format = GO7007_FORMAT_MPEG4;
			/*if (mpeg->pali >> 24 == 4)
				go->pali = mpeg->pali & 0xff;
			else*/
				go->pali = 0xf5;
			break;
		default:
			return -EINVAL;
		}
		go->gop_header_enable =
			/*mpeg->flags & GO7007_MPEG_OMIT_GOP_HEADER
			? 0 :*/ 1;
		/*if (mpeg->flags & GO7007_MPEG_REPEAT_SEQHEADER)
			go->repeat_seqhead = 1;
		else*/
			go->repeat_seqhead = 0;
		go->dvd_mode = 0;
		break;
	case V4L2_CID_MPEG_VIDEO_ASPECT:
		if (go->format == GO7007_FORMAT_MJPEG)
			return -EINVAL;
		switch (ctrl->value) {
		case V4L2_MPEG_VIDEO_ASPECT_1x1:
			go->aspect_ratio = GO7007_RATIO_1_1;
			break;
		case V4L2_MPEG_VIDEO_ASPECT_4x3:
			go->aspect_ratio = GO7007_RATIO_4_3;
			break;
		case V4L2_MPEG_VIDEO_ASPECT_16x9:
			go->aspect_ratio = GO7007_RATIO_16_9;
			break;
		case V4L2_MPEG_VIDEO_ASPECT_221x100:
		default:
			return -EINVAL;
		}
		break;
	case V4L2_CID_MPEG_VIDEO_GOP_SIZE:
		if (ctrl->value < 0 || ctrl->value > 34)
			return -EINVAL;
		go->gop_size = ctrl->value;
		break;
	case V4L2_CID_MPEG_VIDEO_GOP_CLOSURE:
		if (ctrl->value != 0 && ctrl->value != 1)
			return -EINVAL;
		go->closed_gop = ctrl->value;
		break;
	case V4L2_CID_MPEG_VIDEO_BITRATE:
		/* Upper bound is kind of arbitrary here */
		if (ctrl->value < 64000 || ctrl->value > 10000000)
			return -EINVAL;
		go->bitrate = ctrl->value;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int mpeg_g_ctrl(struct v4l2_control *ctrl, struct go7007 *go)
{
	switch (ctrl->id) {
	case V4L2_CID_MPEG_STREAM_TYPE:
		if (go->dvd_mode)
			ctrl->value = V4L2_MPEG_STREAM_TYPE_MPEG2_DVD;
		else
			ctrl->value = V4L2_MPEG_STREAM_TYPE_MPEG_ELEM;
		break;
	case V4L2_CID_MPEG_VIDEO_ENCODING:
		switch (go->format) {
		case GO7007_FORMAT_MPEG1:
			ctrl->value = V4L2_MPEG_VIDEO_ENCODING_MPEG_1;
			break;
		case GO7007_FORMAT_MPEG2:
			ctrl->value = V4L2_MPEG_VIDEO_ENCODING_MPEG_2;
			break;
		case GO7007_FORMAT_MPEG4:
			ctrl->value = V4L2_MPEG_VIDEO_ENCODING_MPEG_4;
			break;
		default:
			return -EINVAL;
		}
		break;
	case V4L2_CID_MPEG_VIDEO_ASPECT:
		switch (go->aspect_ratio) {
		case GO7007_RATIO_1_1:
			ctrl->value = V4L2_MPEG_VIDEO_ASPECT_1x1;
			break;
		case GO7007_RATIO_4_3:
			ctrl->value = V4L2_MPEG_VIDEO_ASPECT_4x3;
			break;
		case GO7007_RATIO_16_9:
			ctrl->value = V4L2_MPEG_VIDEO_ASPECT_16x9;
			break;
		default:
			return -EINVAL;
		}
		break;
	case V4L2_CID_MPEG_VIDEO_GOP_SIZE:
		ctrl->value = go->gop_size;
		break;
	case V4L2_CID_MPEG_VIDEO_GOP_CLOSURE:
		ctrl->value = go->closed_gop;
		break;
	case V4L2_CID_MPEG_VIDEO_BITRATE:
		ctrl->value = go->bitrate;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int vidioc_querycap(struct file *file, void  *priv,
					struct v4l2_capability *cap)
{
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	strlcpy(cap->driver, "go7007", sizeof(cap->driver));
	strlcpy(cap->card, go->name, sizeof(cap->card));
#if 0
	strlcpy(cap->bus_info, dev_name(&dev->udev->dev), sizeof(cap->bus_info));
#endif

	cap->version = KERNEL_VERSION(0, 9, 8);

	cap->capabilities = V4L2_CAP_VIDEO_CAPTURE |
			    V4L2_CAP_STREAMING; /* | V4L2_CAP_AUDIO; */

	if (go->board_info->flags & GO7007_BOARD_HAS_TUNER)
		cap->capabilities |= V4L2_CAP_TUNER;

=======
static int vidioc_querycap(struct file *file, void  *priv,
					struct v4l2_capability *cap)
{
	struct go7007 *go = video_drvdata(file);

	strlcpy(cap->driver, "go7007", sizeof(cap->driver));
	strlcpy(cap->card, go->name, sizeof(cap->card));
	strlcpy(cap->bus_info, go->bus_info, sizeof(cap->bus_info));

	cap->device_caps = V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_READWRITE |
				V4L2_CAP_STREAMING;

	if (go->board_info->num_aud_inputs)
		cap->device_caps |= V4L2_CAP_AUDIO;
	if (go->board_info->flags & GO7007_BOARD_HAS_TUNER)
		cap->device_caps |= V4L2_CAP_TUNER;
	cap->capabilities = cap->device_caps | V4L2_CAP_DEVICE_CAPS;
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int vidioc_enum_fmt_vid_cap(struct file *file, void  *priv,
					struct v4l2_fmtdesc *fmt)
{
	char *desc = NULL;

	switch (fmt->index) {
	case 0:
		fmt->pixelformat = V4L2_PIX_FMT_MJPEG;
<<<<<<< HEAD
		desc = "Motion-JPEG";
		break;
	case 1:
		fmt->pixelformat = V4L2_PIX_FMT_MPEG;
		desc = "MPEG1/MPEG2/MPEG4";
=======
		desc = "Motion JPEG";
		break;
	case 1:
		fmt->pixelformat = V4L2_PIX_FMT_MPEG1;
		desc = "MPEG-1 ES";
		break;
	case 2:
		fmt->pixelformat = V4L2_PIX_FMT_MPEG2;
		desc = "MPEG-2 ES";
		break;
	case 3:
		fmt->pixelformat = V4L2_PIX_FMT_MPEG4;
		desc = "MPEG-4 ES";
>>>>>>> refs/remotes/origin/master
		break;
	default:
		return -EINVAL;
	}
	fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt->flags = V4L2_FMT_FLAG_COMPRESSED;

	strncpy(fmt->description, desc, sizeof(fmt->description));

	return 0;
}

static int vidioc_g_fmt_vid_cap(struct file *file, void *priv,
					struct v4l2_format *fmt)
{
<<<<<<< HEAD
	struct go7007 *go = ((struct go7007_file *) priv)->go;
=======
	struct go7007 *go = video_drvdata(file);
>>>>>>> refs/remotes/origin/master

	fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt->fmt.pix.width = go->width;
	fmt->fmt.pix.height = go->height;
<<<<<<< HEAD
	fmt->fmt.pix.pixelformat = (go->format == GO7007_FORMAT_MJPEG) ?
				   V4L2_PIX_FMT_MJPEG : V4L2_PIX_FMT_MPEG;
=======
	fmt->fmt.pix.pixelformat = go->format;
>>>>>>> refs/remotes/origin/master
	fmt->fmt.pix.field = V4L2_FIELD_NONE;
	fmt->fmt.pix.bytesperline = 0;
	fmt->fmt.pix.sizeimage = GO7007_BUF_SIZE;
	fmt->fmt.pix.colorspace = V4L2_COLORSPACE_SMPTE170M;

	return 0;
}

static int vidioc_try_fmt_vid_cap(struct file *file, void *priv,
			struct v4l2_format *fmt)
{
<<<<<<< HEAD
	struct go7007 *go = ((struct go7007_file *) priv)->go;
=======
	struct go7007 *go = video_drvdata(file);
>>>>>>> refs/remotes/origin/master

	return set_capture_size(go, fmt, 1);
}

static int vidioc_s_fmt_vid_cap(struct file *file, void *priv,
			struct v4l2_format *fmt)
{
<<<<<<< HEAD
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	if (go->streaming)
=======
	struct go7007 *go = video_drvdata(file);

	if (vb2_is_busy(&go->vidq))
>>>>>>> refs/remotes/origin/master
		return -EBUSY;

	return set_capture_size(go, fmt, 0);
}

<<<<<<< HEAD
static int vidioc_reqbufs(struct file *file, void *priv,
			  struct v4l2_requestbuffers *req)
{
	struct go7007_file *gofh = priv;
	struct go7007 *go = gofh->go;
	int retval = -EBUSY;
	unsigned int count, i;

	if (go->streaming)
		return retval;

	if (req->type != V4L2_BUF_TYPE_VIDEO_CAPTURE ||
			req->memory != V4L2_MEMORY_MMAP)
		return -EINVAL;

	mutex_lock(&gofh->lock);
	for (i = 0; i < gofh->buf_count; ++i)
		if (gofh->bufs[i].mapped > 0)
			goto unlock_and_return;

	mutex_lock(&go->hw_lock);
	if (go->in_use > 0 && gofh->buf_count == 0) {
		mutex_unlock(&go->hw_lock);
		goto unlock_and_return;
	}

	if (gofh->buf_count > 0)
		kfree(gofh->bufs);

	retval = -ENOMEM;
	count = req->count;
	if (count > 0) {
		if (count < 2)
			count = 2;
		if (count > 32)
			count = 32;

		gofh->bufs = kcalloc(count, sizeof(struct go7007_buffer),
				     GFP_KERNEL);

		if (!gofh->bufs) {
			mutex_unlock(&go->hw_lock);
			goto unlock_and_return;
		}

		for (i = 0; i < count; ++i) {
			gofh->bufs[i].go = go;
			gofh->bufs[i].index = i;
			gofh->bufs[i].state = BUF_STATE_IDLE;
			gofh->bufs[i].mapped = 0;
		}

		go->in_use = 1;
	} else {
		go->in_use = 0;
	}

	gofh->buf_count = count;
	mutex_unlock(&go->hw_lock);
	mutex_unlock(&gofh->lock);

	memset(req, 0, sizeof(*req));

	req->count = count;
	req->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req->memory = V4L2_MEMORY_MMAP;

	return 0;

unlock_and_return:
	mutex_unlock(&gofh->lock);
	return retval;
}

static int vidioc_querybuf(struct file *file, void *priv,
			   struct v4l2_buffer *buf)
{
	struct go7007_file *gofh = priv;
	int retval = -EINVAL;
	unsigned int index;

	if (buf->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return retval;

	index = buf->index;

	mutex_lock(&gofh->lock);
	if (index >= gofh->buf_count)
		goto unlock_and_return;

	memset(buf, 0, sizeof(*buf));
	buf->index = index;
	buf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	switch (gofh->bufs[index].state) {
	case BUF_STATE_QUEUED:
		buf->flags = V4L2_BUF_FLAG_QUEUED;
		break;
	case BUF_STATE_DONE:
		buf->flags = V4L2_BUF_FLAG_DONE;
		break;
	default:
		buf->flags = 0;
	}

	if (gofh->bufs[index].mapped)
		buf->flags |= V4L2_BUF_FLAG_MAPPED;
	buf->memory = V4L2_MEMORY_MMAP;
	buf->m.offset = index * GO7007_BUF_SIZE;
	buf->length = GO7007_BUF_SIZE;
	mutex_unlock(&gofh->lock);

	return 0;

unlock_and_return:
	mutex_unlock(&gofh->lock);
	return retval;
}

static int vidioc_qbuf(struct file *file, void *priv, struct v4l2_buffer *buf)
{
	struct go7007_file *gofh = priv;
	struct go7007 *go = gofh->go;
	struct go7007_buffer *gobuf;
	unsigned long flags;
	int retval = -EINVAL;
	int ret;

	if (buf->type != V4L2_BUF_TYPE_VIDEO_CAPTURE ||
			buf->memory != V4L2_MEMORY_MMAP)
		return retval;

	mutex_lock(&gofh->lock);
	if (buf->index < 0 || buf->index >= gofh->buf_count)
		goto unlock_and_return;

	gobuf = &gofh->bufs[buf->index];
	if (!gobuf->mapped)
		goto unlock_and_return;

	retval = -EBUSY;
	if (gobuf->state != BUF_STATE_IDLE)
		goto unlock_and_return;

	/* offset will be 0 until we really support USERPTR streaming */
	gobuf->offset = gobuf->user_addr & ~PAGE_MASK;
	gobuf->bytesused = 0;
	gobuf->frame_offset = 0;
	gobuf->modet_active = 0;
	if (gobuf->offset > 0)
		gobuf->page_count = GO7007_BUF_PAGES + 1;
	else
		gobuf->page_count = GO7007_BUF_PAGES;

	retval = -ENOMEM;
	down_read(&current->mm->mmap_sem);
	ret = get_user_pages(current, current->mm,
			gobuf->user_addr & PAGE_MASK, gobuf->page_count,
			1, 1, gobuf->pages, NULL);
	up_read(&current->mm->mmap_sem);

	if (ret != gobuf->page_count) {
		int i;
		for (i = 0; i < ret; ++i)
			page_cache_release(gobuf->pages[i]);
		gobuf->page_count = 0;
		goto unlock_and_return;
	}

	gobuf->state = BUF_STATE_QUEUED;
	spin_lock_irqsave(&go->spinlock, flags);
	list_add_tail(&gobuf->stream, &go->stream);
	spin_unlock_irqrestore(&go->spinlock, flags);
	mutex_unlock(&gofh->lock);

	return 0;

unlock_and_return:
	mutex_unlock(&gofh->lock);
	return retval;
}


static int vidioc_dqbuf(struct file *file, void *priv, struct v4l2_buffer *buf)
{
	struct go7007_file *gofh = priv;
	struct go7007 *go = gofh->go;
	struct go7007_buffer *gobuf;
	int retval = -EINVAL;
	unsigned long flags;
	u32 frame_type_flag;
	DEFINE_WAIT(wait);

	if (buf->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return retval;
	if (buf->memory != V4L2_MEMORY_MMAP)
		return retval;

	mutex_lock(&gofh->lock);
	if (list_empty(&go->stream))
		goto unlock_and_return;
	gobuf = list_entry(go->stream.next,
			struct go7007_buffer, stream);

	retval = -EAGAIN;
	if (gobuf->state != BUF_STATE_DONE &&
			!(file->f_flags & O_NONBLOCK)) {
		for (;;) {
			prepare_to_wait(&go->frame_waitq, &wait,
					TASK_INTERRUPTIBLE);
			if (gobuf->state == BUF_STATE_DONE)
				break;
			if (signal_pending(current)) {
				retval = -ERESTARTSYS;
				break;
			}
			schedule();
		}
		finish_wait(&go->frame_waitq, &wait);
	}
	if (gobuf->state != BUF_STATE_DONE)
		goto unlock_and_return;

	spin_lock_irqsave(&go->spinlock, flags);
	deactivate_buffer(gobuf);
	spin_unlock_irqrestore(&go->spinlock, flags);
	frame_type_flag = get_frame_type_flag(gobuf, go->format);
	gobuf->state = BUF_STATE_IDLE;

	memset(buf, 0, sizeof(*buf));
	buf->index = gobuf->index;
	buf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf->bytesused = gobuf->bytesused;
	buf->flags = V4L2_BUF_FLAG_MAPPED | frame_type_flag;
	buf->field = V4L2_FIELD_NONE;
	buf->timestamp = gobuf->timestamp;
	buf->sequence = gobuf->seq;
	buf->memory = V4L2_MEMORY_MMAP;
	buf->m.offset = gobuf->index * GO7007_BUF_SIZE;
	buf->length = GO7007_BUF_SIZE;
	buf->reserved = gobuf->modet_active;

	mutex_unlock(&gofh->lock);
	return 0;

unlock_and_return:
	mutex_unlock(&gofh->lock);
	return retval;
}

static int vidioc_streamon(struct file *file, void *priv,
					enum v4l2_buf_type type)
{
	struct go7007_file *gofh = priv;
	struct go7007 *go = gofh->go;
	int retval = 0;

	if (type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	mutex_lock(&gofh->lock);
	mutex_lock(&go->hw_lock);

	if (!go->streaming) {
		go->streaming = 1;
		go->next_seq = 0;
		go->active_buf = NULL;
		if (go7007_start_encoder(go) < 0)
			retval = -EIO;
		else
			retval = 0;
	}
	mutex_unlock(&go->hw_lock);
	mutex_unlock(&gofh->lock);

	return retval;
}

static int vidioc_streamoff(struct file *file, void *priv,
					enum v4l2_buf_type type)
{
	struct go7007_file *gofh = priv;
	struct go7007 *go = gofh->go;

	if (type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;
	mutex_lock(&gofh->lock);
	go7007_streamoff(go);
	mutex_unlock(&gofh->lock);

	return 0;
}

static int vidioc_queryctrl(struct file *file, void *priv,
			   struct v4l2_queryctrl *query)
{
	struct go7007 *go = ((struct go7007_file *) priv)->go;
	int id = query->id;

	if (0 == call_all(&go->v4l2_dev, core, queryctrl, query))
		return 0;

	query->id = id;
	return mpeg_query_ctrl(query);
}

static int vidioc_g_ctrl(struct file *file, void *priv,
				struct v4l2_control *ctrl)
{
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	if (0 == call_all(&go->v4l2_dev, core, g_ctrl, ctrl))
		return 0;

	return mpeg_g_ctrl(ctrl, go);
}

static int vidioc_s_ctrl(struct file *file, void *priv,
				struct v4l2_control *ctrl)
{
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	if (0 == call_all(&go->v4l2_dev, core, s_ctrl, ctrl))
		return 0;

	return mpeg_s_ctrl(ctrl, go);
}
=======
static int go7007_queue_setup(struct vb2_queue *q, const struct v4l2_format *fmt,
		unsigned int *num_buffers, unsigned int *num_planes,
		unsigned int sizes[], void *alloc_ctxs[])
{
	sizes[0] = GO7007_BUF_SIZE;
	*num_planes = 1;

	if (*num_buffers < 2)
		*num_buffers = 2;

	return 0;
}

static void go7007_buf_queue(struct vb2_buffer *vb)
{
	struct vb2_queue *vq = vb->vb2_queue;
	struct go7007 *go = vb2_get_drv_priv(vq);
	struct go7007_buffer *go7007_vb =
		container_of(vb, struct go7007_buffer, vb);
	unsigned long flags;

	spin_lock_irqsave(&go->spinlock, flags);
	list_add_tail(&go7007_vb->list, &go->vidq_active);
	spin_unlock_irqrestore(&go->spinlock, flags);
}

static int go7007_buf_prepare(struct vb2_buffer *vb)
{
	struct go7007_buffer *go7007_vb =
		container_of(vb, struct go7007_buffer, vb);

	go7007_vb->modet_active = 0;
	go7007_vb->frame_offset = 0;
	vb->v4l2_planes[0].bytesused = 0;
	return 0;
}

static int go7007_buf_finish(struct vb2_buffer *vb)
{
	struct vb2_queue *vq = vb->vb2_queue;
	struct go7007 *go = vb2_get_drv_priv(vq);
	struct go7007_buffer *go7007_vb =
		container_of(vb, struct go7007_buffer, vb);
	u32 frame_type_flag = get_frame_type_flag(go7007_vb, go->format);
	struct v4l2_buffer *buf = &vb->v4l2_buf;

	buf->flags &= ~(V4L2_BUF_FLAG_KEYFRAME | V4L2_BUF_FLAG_BFRAME |
			V4L2_BUF_FLAG_PFRAME);
	buf->flags |= frame_type_flag;
	buf->field = V4L2_FIELD_NONE;
	return 0;
}

static int go7007_start_streaming(struct vb2_queue *q, unsigned int count)
{
	struct go7007 *go = vb2_get_drv_priv(q);
	int ret;

	set_formatting(go);
	mutex_lock(&go->hw_lock);
	go->next_seq = 0;
	go->active_buf = NULL;
	q->streaming = 1;
	if (go7007_start_encoder(go) < 0)
		ret = -EIO;
	else
		ret = 0;
	mutex_unlock(&go->hw_lock);
	if (ret) {
		q->streaming = 0;
		return ret;
	}
	call_all(&go->v4l2_dev, video, s_stream, 1);
	v4l2_ctrl_grab(go->mpeg_video_gop_size, true);
	v4l2_ctrl_grab(go->mpeg_video_gop_closure, true);
	v4l2_ctrl_grab(go->mpeg_video_bitrate, true);
	v4l2_ctrl_grab(go->mpeg_video_aspect_ratio, true);
	/* Turn on Capture LED */
	if (go->board_id == GO7007_BOARDID_ADS_USBAV_709)
		go7007_write_addr(go, 0x3c82, 0x0005);
	return ret;
}

static int go7007_stop_streaming(struct vb2_queue *q)
{
	struct go7007 *go = vb2_get_drv_priv(q);
	unsigned long flags;

	q->streaming = 0;
	go7007_stream_stop(go);
	mutex_lock(&go->hw_lock);
	go7007_reset_encoder(go);
	mutex_unlock(&go->hw_lock);
	call_all(&go->v4l2_dev, video, s_stream, 0);

	spin_lock_irqsave(&go->spinlock, flags);
	INIT_LIST_HEAD(&go->vidq_active);
	spin_unlock_irqrestore(&go->spinlock, flags);
	v4l2_ctrl_grab(go->mpeg_video_gop_size, false);
	v4l2_ctrl_grab(go->mpeg_video_gop_closure, false);
	v4l2_ctrl_grab(go->mpeg_video_bitrate, false);
	v4l2_ctrl_grab(go->mpeg_video_aspect_ratio, false);
	/* Turn on Capture LED */
	if (go->board_id == GO7007_BOARDID_ADS_USBAV_709)
		go7007_write_addr(go, 0x3c82, 0x000d);
	return 0;
}

static struct vb2_ops go7007_video_qops = {
	.queue_setup    = go7007_queue_setup,
	.buf_queue      = go7007_buf_queue,
	.buf_prepare    = go7007_buf_prepare,
	.buf_finish     = go7007_buf_finish,
	.start_streaming = go7007_start_streaming,
	.stop_streaming = go7007_stop_streaming,
	.wait_prepare   = vb2_ops_wait_prepare,
	.wait_finish    = vb2_ops_wait_finish,
};
>>>>>>> refs/remotes/origin/master

static int vidioc_g_parm(struct file *filp, void *priv,
		struct v4l2_streamparm *parm)
{
<<<<<<< HEAD
	struct go7007 *go = ((struct go7007_file *) priv)->go;
=======
	struct go7007 *go = video_drvdata(filp);
>>>>>>> refs/remotes/origin/master
	struct v4l2_fract timeperframe = {
		.numerator = 1001 *  go->fps_scale,
		.denominator = go->sensor_framerate,
	};

	if (parm->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

<<<<<<< HEAD
	parm->parm.capture.capability |= V4L2_CAP_TIMEPERFRAME;
=======
	parm->parm.capture.readbuffers = 2;
	parm->parm.capture.capability = V4L2_CAP_TIMEPERFRAME;
>>>>>>> refs/remotes/origin/master
	parm->parm.capture.timeperframe = timeperframe;

	return 0;
}

static int vidioc_s_parm(struct file *filp, void *priv,
		struct v4l2_streamparm *parm)
{
<<<<<<< HEAD
	struct go7007 *go = ((struct go7007_file *) priv)->go;
=======
	struct go7007 *go = video_drvdata(filp);
>>>>>>> refs/remotes/origin/master
	unsigned int n, d;

	if (parm->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;
<<<<<<< HEAD
	if (parm->parm.capture.capturemode != 0)
		return -EINVAL;
=======
>>>>>>> refs/remotes/origin/master

	n = go->sensor_framerate *
		parm->parm.capture.timeperframe.numerator;
	d = 1001 * parm->parm.capture.timeperframe.denominator;
	if (n != 0 && d != 0 && n > d)
		go->fps_scale = (n + d/2) / d;
	else
		go->fps_scale = 1;

<<<<<<< HEAD
	return 0;
=======
	return vidioc_g_parm(filp, priv, parm);
>>>>>>> refs/remotes/origin/master
}

/* VIDIOC_ENUMSTD on go7007 were used for enumerating the supported fps and
   its resolution, when the device is not connected to TV.
   This is were an API abuse, probably used by the lack of specific IOCTL's to
   enumerate it, by the time the driver was written.

   However, since kernel 2.6.19, two new ioctls (VIDIOC_ENUM_FRAMEINTERVALS
   and VIDIOC_ENUM_FRAMESIZES) were added for this purpose.

   The two functions below implement the newer ioctls
*/
static int vidioc_enum_framesizes(struct file *filp, void *priv,
				  struct v4l2_frmsizeenum *fsize)
{
<<<<<<< HEAD
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	/* Return -EINVAL, if it is a TV board */
	if ((go->board_info->flags & GO7007_BOARD_HAS_TUNER) ||
	    (go->board_info->sensor_flags & GO7007_SENSOR_TV))
		return -EINVAL;

	if (fsize->index > 0)
		return -EINVAL;

	fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
	fsize->discrete.width = go->board_info->sensor_width;
	fsize->discrete.height = go->board_info->sensor_height;

=======
	struct go7007 *go = video_drvdata(filp);
	int width, height;

	if (fsize->index > 2)
		return -EINVAL;

	if (!valid_pixelformat(fsize->pixel_format))
		return -EINVAL;

	get_resolution(go, &width, &height);
	fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
	fsize->discrete.width = (width >> fsize->index) & ~0xf;
	fsize->discrete.height = (height >> fsize->index) & ~0xf;
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int vidioc_enum_frameintervals(struct file *filp, void *priv,
				      struct v4l2_frmivalenum *fival)
{
<<<<<<< HEAD
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	/* Return -EINVAL, if it is a TV board */
	if ((go->board_info->flags & GO7007_BOARD_HAS_TUNER) ||
	    (go->board_info->sensor_flags & GO7007_SENSOR_TV))
		return -EINVAL;

	if (fival->index > 0)
		return -EINVAL;

	fival->type = V4L2_FRMIVAL_TYPE_DISCRETE;
	fival->discrete.numerator = 1001;
	fival->discrete.denominator = go->board_info->sensor_framerate;

=======
	struct go7007 *go = video_drvdata(filp);
	int width, height;
	int i;

	if (fival->index > 4)
		return -EINVAL;

	if (!valid_pixelformat(fival->pixel_format))
		return -EINVAL;

	if (!(go->board_info->sensor_flags & GO7007_SENSOR_SCALING)) {
		get_resolution(go, &width, &height);
		for (i = 0; i <= 2; i++)
			if (fival->width == ((width >> i) & ~0xf) &&
			    fival->height == ((height >> i) & ~0xf))
				break;
		if (i > 2)
			return -EINVAL;
	}
	fival->type = V4L2_FRMIVAL_TYPE_DISCRETE;
	fival->discrete.numerator = 1001 * (fival->index + 1);
	fival->discrete.denominator = go->sensor_framerate;
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int vidioc_g_std(struct file *file, void *priv, v4l2_std_id *std)
{
<<<<<<< HEAD
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	switch (go->standard) {
	case GO7007_STD_NTSC:
		*std = V4L2_STD_NTSC;
		break;
	case GO7007_STD_PAL:
		*std = V4L2_STD_PAL;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int vidioc_s_std(struct file *file, void *priv, v4l2_std_id *std)
{
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	if (go->streaming)
		return -EBUSY;

	if (!(go->board_info->sensor_flags & GO7007_SENSOR_TV) && *std != 0)
		return -EINVAL;

	if (*std == 0)
		return -EINVAL;

	if ((go->board_info->flags & GO7007_BOARD_HAS_TUNER) &&
			go->input == go->board_info->num_inputs - 1) {
		if (!go->i2c_adapter_online)
			return -EIO;
		if (call_all(&go->v4l2_dev, core, s_std, *std) < 0)
			return -EINVAL;
	}

	if (*std & V4L2_STD_NTSC) {
		go->standard = GO7007_STD_NTSC;
		go->sensor_framerate = 30000;
	} else if (*std & V4L2_STD_PAL) {
		go->standard = GO7007_STD_PAL;
		go->sensor_framerate = 25025;
	} else if (*std & V4L2_STD_SECAM) {
		go->standard = GO7007_STD_PAL;
		go->sensor_framerate = 25025;
	} else
		return -EINVAL;

	call_all(&go->v4l2_dev, core, s_std, *std);
	set_capture_size(go, NULL, 0);

	return 0;
=======
	struct go7007 *go = video_drvdata(file);

	*std = go->std;
	return 0;
}

static int go7007_s_std(struct go7007 *go)
{
	if (go->std & V4L2_STD_625_50) {
		go->standard = GO7007_STD_PAL;
		go->sensor_framerate = 25025;
	} else {
		go->standard = GO7007_STD_NTSC;
		go->sensor_framerate = 30000;
	}

	call_all(&go->v4l2_dev, core, s_std, go->std);
	set_capture_size(go, NULL, 0);
	return 0;
}

static int vidioc_s_std(struct file *file, void *priv, v4l2_std_id std)
{
	struct go7007 *go = video_drvdata(file);

	if (vb2_is_busy(&go->vidq))
		return -EBUSY;

	go->std = std;

	return go7007_s_std(go);
>>>>>>> refs/remotes/origin/master
}

static int vidioc_querystd(struct file *file, void *priv, v4l2_std_id *std)
{
<<<<<<< HEAD
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	if ((go->board_info->flags & GO7007_BOARD_HAS_TUNER) &&
			go->input == go->board_info->num_inputs - 1) {
		if (!go->i2c_adapter_online)
			return -EIO;
		return call_all(&go->v4l2_dev, video, querystd, std);
	} else if (go->board_info->sensor_flags & GO7007_SENSOR_TV)
		*std = V4L2_STD_NTSC | V4L2_STD_PAL | V4L2_STD_SECAM;
	else
		*std = 0;

	return 0;
=======
	struct go7007 *go = video_drvdata(file);

	return call_all(&go->v4l2_dev, video, querystd, std);
>>>>>>> refs/remotes/origin/master
}

static int vidioc_enum_input(struct file *file, void *priv,
				struct v4l2_input *inp)
{
<<<<<<< HEAD
	struct go7007 *go = ((struct go7007_file *) priv)->go;
=======
	struct go7007 *go = video_drvdata(file);
>>>>>>> refs/remotes/origin/master

	if (inp->index >= go->board_info->num_inputs)
		return -EINVAL;

	strncpy(inp->name, go->board_info->inputs[inp->index].name,
			sizeof(inp->name));

<<<<<<< HEAD
	/* If this board has a tuner, it will be the last input */
	if ((go->board_info->flags & GO7007_BOARD_HAS_TUNER) &&
			inp->index == go->board_info->num_inputs - 1)
=======
	/* If this board has a tuner, it will be the first input */
	if ((go->board_info->flags & GO7007_BOARD_HAS_TUNER) &&
			inp->index == 0)
>>>>>>> refs/remotes/origin/master
		inp->type = V4L2_INPUT_TYPE_TUNER;
	else
		inp->type = V4L2_INPUT_TYPE_CAMERA;

<<<<<<< HEAD
	inp->audioset = 0;
	inp->tuner = 0;
	if (go->board_info->sensor_flags & GO7007_SENSOR_TV)
		inp->std = V4L2_STD_NTSC | V4L2_STD_PAL |
						V4L2_STD_SECAM;
=======
	if (go->board_info->num_aud_inputs)
		inp->audioset = (1 << go->board_info->num_aud_inputs) - 1;
	else
		inp->audioset = 0;
	inp->tuner = 0;
	if (go->board_info->sensor_flags & GO7007_SENSOR_TV)
		inp->std = video_devdata(file)->tvnorms;
>>>>>>> refs/remotes/origin/master
	else
		inp->std = 0;

	return 0;
}


static int vidioc_g_input(struct file *file, void *priv, unsigned int *input)
{
<<<<<<< HEAD
	struct go7007 *go = ((struct go7007_file *) priv)->go;
=======
	struct go7007 *go = video_drvdata(file);
>>>>>>> refs/remotes/origin/master

	*input = go->input;

	return 0;
}

<<<<<<< HEAD
static int vidioc_s_input(struct file *file, void *priv, unsigned int input)
{
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	if (input >= go->board_info->num_inputs)
		return -EINVAL;
	if (go->streaming)
		return -EBUSY;

	go->input = input;

	return call_all(&go->v4l2_dev, video, s_routing, input, 0, 0);
}

static int vidioc_g_tuner(struct file *file, void *priv,
				struct v4l2_tuner *t)
{
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	if (!(go->board_info->flags & GO7007_BOARD_HAS_TUNER))
		return -EINVAL;
	if (t->index != 0)
		return -EINVAL;
	if (!go->i2c_adapter_online)
		return -EIO;

	return call_all(&go->v4l2_dev, tuner, g_tuner, t);
}

static int vidioc_s_tuner(struct file *file, void *priv,
				struct v4l2_tuner *t)
{
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	if (!(go->board_info->flags & GO7007_BOARD_HAS_TUNER))
		return -EINVAL;
	if (t->index != 0)
		return -EINVAL;
	if (!go->i2c_adapter_online)
		return -EIO;

	switch (go->board_id) {
	case GO7007_BOARDID_PX_TV402U_NA:
	case GO7007_BOARDID_PX_TV402U_JP:
		/* No selectable options currently */
		if (t->audmode != V4L2_TUNER_MODE_STEREO)
			return -EINVAL;
		break;
	}

	return call_all(&go->v4l2_dev, tuner, s_tuner, t);
}

static int vidioc_g_frequency(struct file *file, void *priv,
				struct v4l2_frequency *f)
{
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	if (!(go->board_info->flags & GO7007_BOARD_HAS_TUNER))
		return -EINVAL;
	if (!go->i2c_adapter_online)
		return -EIO;

	f->type = V4L2_TUNER_ANALOG_TV;

	return call_all(&go->v4l2_dev, tuner, g_frequency, f);
}

static int vidioc_s_frequency(struct file *file, void *priv,
				struct v4l2_frequency *f)
{
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	if (!(go->board_info->flags & GO7007_BOARD_HAS_TUNER))
		return -EINVAL;
	if (!go->i2c_adapter_online)
		return -EIO;

	return call_all(&go->v4l2_dev, tuner, s_frequency, f);
}

static int vidioc_cropcap(struct file *file, void *priv,
					struct v4l2_cropcap *cropcap)
{
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	if (cropcap->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	/* These specify the raw input of the sensor */
	switch (go->standard) {
	case GO7007_STD_NTSC:
		cropcap->bounds.top = 0;
		cropcap->bounds.left = 0;
		cropcap->bounds.width = 720;
		cropcap->bounds.height = 480;
		cropcap->defrect.top = 0;
		cropcap->defrect.left = 0;
		cropcap->defrect.width = 720;
		cropcap->defrect.height = 480;
		break;
	case GO7007_STD_PAL:
		cropcap->bounds.top = 0;
		cropcap->bounds.left = 0;
		cropcap->bounds.width = 720;
		cropcap->bounds.height = 576;
		cropcap->defrect.top = 0;
		cropcap->defrect.left = 0;
		cropcap->defrect.width = 720;
		cropcap->defrect.height = 576;
		break;
	case GO7007_STD_OTHER:
		cropcap->bounds.top = 0;
		cropcap->bounds.left = 0;
		cropcap->bounds.width = go->board_info->sensor_width;
		cropcap->bounds.height = go->board_info->sensor_height;
		cropcap->defrect.top = 0;
		cropcap->defrect.left = 0;
		cropcap->defrect.width = go->board_info->sensor_width;
		cropcap->defrect.height = go->board_info->sensor_height;
		break;
	}

	return 0;
}

static int vidioc_g_crop(struct file *file, void *priv, struct v4l2_crop *crop)
{
	struct go7007 *go = ((struct go7007_file *) priv)->go;

	if (crop->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	crop->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	/* These specify the raw input of the sensor */
	switch (go->standard) {
	case GO7007_STD_NTSC:
		crop->c.top = 0;
		crop->c.left = 0;
		crop->c.width = 720;
		crop->c.height = 480;
		break;
	case GO7007_STD_PAL:
		crop->c.top = 0;
		crop->c.left = 0;
		crop->c.width = 720;
		crop->c.height = 576;
		break;
	case GO7007_STD_OTHER:
		crop->c.top = 0;
		crop->c.left = 0;
		crop->c.width = go->board_info->sensor_width;
		crop->c.height = go->board_info->sensor_height;
		break;
	}

	return 0;
}

/* FIXME: vidioc_s_crop is not really implemented!!!
 */
static int vidioc_s_crop(struct file *file, void *priv, struct v4l2_crop *crop)
{
	if (crop->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	return 0;
}

static int vidioc_g_jpegcomp(struct file *file, void *priv,
			 struct v4l2_jpegcompression *params)
{
	memset(params, 0, sizeof(*params));
	params->quality = 50; /* ?? */
	params->jpeg_markers = V4L2_JPEG_MARKER_DHT |
				V4L2_JPEG_MARKER_DQT;

	return 0;
}

static int vidioc_s_jpegcomp(struct file *file, void *priv,
			 struct v4l2_jpegcompression *params)
{
	if (params->quality != 50 ||
			params->jpeg_markers != (V4L2_JPEG_MARKER_DHT |
						V4L2_JPEG_MARKER_DQT))
		return -EINVAL;

	return 0;
=======
static int vidioc_enumaudio(struct file *file, void *fh, struct v4l2_audio *a)
{
	struct go7007 *go = video_drvdata(file);

	if (a->index >= go->board_info->num_aud_inputs)
		return -EINVAL;
	strlcpy(a->name, go->board_info->aud_inputs[a->index].name, sizeof(a->name));
	a->capability = V4L2_AUDCAP_STEREO;
	return 0;
}

static int vidioc_g_audio(struct file *file, void *fh, struct v4l2_audio *a)
{
	struct go7007 *go = video_drvdata(file);

	a->index = go->aud_input;
	strlcpy(a->name, go->board_info->aud_inputs[go->aud_input].name, sizeof(a->name));
	a->capability = V4L2_AUDCAP_STEREO;
	return 0;
}

static int vidioc_s_audio(struct file *file, void *fh, const struct v4l2_audio *a)
{
	struct go7007 *go = video_drvdata(file);

	if (a->index >= go->board_info->num_aud_inputs)
		return -EINVAL;
	go->aud_input = a->index;
	v4l2_subdev_call(go->sd_audio, audio, s_routing,
			go->board_info->aud_inputs[go->aud_input].audio_input, 0, 0);
	return 0;
}

static void go7007_s_input(struct go7007 *go)
{
	unsigned int input = go->input;

	v4l2_subdev_call(go->sd_video, video, s_routing,
			go->board_info->inputs[input].video_input, 0,
			go->board_info->video_config);
	if (go->board_info->num_aud_inputs) {
		int aud_input = go->board_info->inputs[input].audio_index;

		v4l2_subdev_call(go->sd_audio, audio, s_routing,
			go->board_info->aud_inputs[aud_input].audio_input, 0, 0);
		go->aud_input = aud_input;
	}
}

static int vidioc_s_input(struct file *file, void *priv, unsigned int input)
{
	struct go7007 *go = video_drvdata(file);

	if (input >= go->board_info->num_inputs)
		return -EINVAL;
	if (vb2_is_busy(&go->vidq))
		return -EBUSY;

	go->input = input;
	go7007_s_input(go);

	return 0;
}

static int vidioc_g_tuner(struct file *file, void *priv,
				struct v4l2_tuner *t)
{
	struct go7007 *go = video_drvdata(file);

	if (t->index != 0)
		return -EINVAL;

	strlcpy(t->name, "Tuner", sizeof(t->name));
	return call_all(&go->v4l2_dev, tuner, g_tuner, t);
}

static int vidioc_s_tuner(struct file *file, void *priv,
				const struct v4l2_tuner *t)
{
	struct go7007 *go = video_drvdata(file);

	if (t->index != 0)
		return -EINVAL;

	return call_all(&go->v4l2_dev, tuner, s_tuner, t);
}

static int vidioc_g_frequency(struct file *file, void *priv,
				struct v4l2_frequency *f)
{
	struct go7007 *go = video_drvdata(file);

	if (f->tuner)
		return -EINVAL;

	return call_all(&go->v4l2_dev, tuner, g_frequency, f);
}

static int vidioc_s_frequency(struct file *file, void *priv,
				const struct v4l2_frequency *f)
{
	struct go7007 *go = video_drvdata(file);

	if (f->tuner)
		return -EINVAL;

	return call_all(&go->v4l2_dev, tuner, s_frequency, f);
}

static int vidioc_log_status(struct file *file, void *priv)
{
	struct go7007 *go = video_drvdata(file);

	v4l2_ctrl_log_status(file, priv);
	return call_all(&go->v4l2_dev, core, log_status);
>>>>>>> refs/remotes/origin/master
}

/* FIXME:
	Those ioctls are private, and not needed, since several standard
	extended controls already provide streaming control.
	So, those ioctls should be converted into vidioc_g_ext_ctrls()
	and vidioc_s_ext_ctrls()
 */

#if 0
<<<<<<< HEAD
	/* Temporary ioctls for controlling compression characteristics */
	case GO7007IOC_S_BITRATE:
	{
		int *bitrate = arg;

		if (go->streaming)
			return -EINVAL;
		/* Upper bound is kind of arbitrary here */
		if (*bitrate < 64000 || *bitrate > 10000000)
			return -EINVAL;
		go->bitrate = *bitrate;
		return 0;
	}
	case GO7007IOC_G_BITRATE:
	{
		int *bitrate = arg;

		*bitrate = go->bitrate;
		return 0;
	}
	case GO7007IOC_S_COMP_PARAMS:
	{
		struct go7007_comp_params *comp = arg;

		if (go->format == GO7007_FORMAT_MJPEG)
			return -EINVAL;
		if (comp->gop_size > 0)
			go->gop_size = comp->gop_size;
		else
			go->gop_size = go->sensor_framerate / 1000;
		if (go->gop_size != 15)
			go->dvd_mode = 0;
		/*go->ipb = comp->max_b_frames > 0;*/ /* completely untested */
		if (go->board_info->sensor_flags & GO7007_SENSOR_TV) {
			switch (comp->aspect_ratio) {
			case GO7007_ASPECT_RATIO_4_3_NTSC:
			case GO7007_ASPECT_RATIO_4_3_PAL:
				go->aspect_ratio = GO7007_RATIO_4_3;
				break;
			case GO7007_ASPECT_RATIO_16_9_NTSC:
			case GO7007_ASPECT_RATIO_16_9_PAL:
				go->aspect_ratio = GO7007_RATIO_16_9;
				break;
			default:
				go->aspect_ratio = GO7007_RATIO_1_1;
				break;
			}
		}
		if (comp->flags & GO7007_COMP_OMIT_SEQ_HEADER) {
			go->dvd_mode = 0;
			go->seq_header_enable = 0;
		} else {
			go->seq_header_enable = 1;
		}
		/* fall-through */
	}
	case GO7007IOC_G_COMP_PARAMS:
	{
		struct go7007_comp_params *comp = arg;

		if (go->format == GO7007_FORMAT_MJPEG)
			return -EINVAL;
		memset(comp, 0, sizeof(*comp));
		comp->gop_size = go->gop_size;
		comp->max_b_frames = go->ipb ? 2 : 0;
		switch (go->aspect_ratio) {
		case GO7007_RATIO_4_3:
			if (go->standard == GO7007_STD_NTSC)
				comp->aspect_ratio =
					GO7007_ASPECT_RATIO_4_3_NTSC;
			else
				comp->aspect_ratio =
					GO7007_ASPECT_RATIO_4_3_PAL;
			break;
		case GO7007_RATIO_16_9:
			if (go->standard == GO7007_STD_NTSC)
				comp->aspect_ratio =
					GO7007_ASPECT_RATIO_16_9_NTSC;
			else
				comp->aspect_ratio =
					GO7007_ASPECT_RATIO_16_9_PAL;
			break;
		default:
			comp->aspect_ratio = GO7007_ASPECT_RATIO_1_1;
			break;
		}
		if (go->closed_gop)
			comp->flags |= GO7007_COMP_CLOSED_GOP;
		if (!go->seq_header_enable)
			comp->flags |= GO7007_COMP_OMIT_SEQ_HEADER;
		return 0;
	}
	case GO7007IOC_S_MPEG_PARAMS:
	{
		struct go7007_mpeg_params *mpeg = arg;

		if (go->format != GO7007_FORMAT_MPEG1 &&
				go->format != GO7007_FORMAT_MPEG2 &&
				go->format != GO7007_FORMAT_MPEG4)
			return -EINVAL;

		if (mpeg->flags & GO7007_MPEG_FORCE_DVD_MODE) {
			go->format = GO7007_FORMAT_MPEG2;
			go->bitrate = 9800000;
			go->gop_size = 15;
			go->pali = 0x48;
			go->closed_gop = 1;
			go->repeat_seqhead = 0;
			go->seq_header_enable = 1;
			go->gop_header_enable = 1;
			go->dvd_mode = 1;
		} else {
			switch (mpeg->mpeg_video_standard) {
			case GO7007_MPEG_VIDEO_MPEG1:
				go->format = GO7007_FORMAT_MPEG1;
				go->pali = 0;
				break;
			case GO7007_MPEG_VIDEO_MPEG2:
				go->format = GO7007_FORMAT_MPEG2;
				if (mpeg->pali >> 24 == 2)
					go->pali = mpeg->pali & 0xff;
				else
					go->pali = 0x48;
				break;
			case GO7007_MPEG_VIDEO_MPEG4:
				go->format = GO7007_FORMAT_MPEG4;
				if (mpeg->pali >> 24 == 4)
					go->pali = mpeg->pali & 0xff;
				else
					go->pali = 0xf5;
				break;
			default:
				return -EINVAL;
			}
			go->gop_header_enable =
				mpeg->flags & GO7007_MPEG_OMIT_GOP_HEADER
				? 0 : 1;
			if (mpeg->flags & GO7007_MPEG_REPEAT_SEQHEADER)
				go->repeat_seqhead = 1;
			else
				go->repeat_seqhead = 0;
			go->dvd_mode = 0;
		}
		/* fall-through */
	}
	case GO7007IOC_G_MPEG_PARAMS:
	{
		struct go7007_mpeg_params *mpeg = arg;

		memset(mpeg, 0, sizeof(*mpeg));
		switch (go->format) {
		case GO7007_FORMAT_MPEG1:
			mpeg->mpeg_video_standard = GO7007_MPEG_VIDEO_MPEG1;
			mpeg->pali = 0;
			break;
		case GO7007_FORMAT_MPEG2:
			mpeg->mpeg_video_standard = GO7007_MPEG_VIDEO_MPEG2;
			mpeg->pali = GO7007_MPEG_PROFILE(2, go->pali);
			break;
		case GO7007_FORMAT_MPEG4:
			mpeg->mpeg_video_standard = GO7007_MPEG_VIDEO_MPEG4;
			mpeg->pali = GO7007_MPEG_PROFILE(4, go->pali);
			break;
		default:
			return -EINVAL;
		}
		if (!go->gop_header_enable)
			mpeg->flags |= GO7007_MPEG_OMIT_GOP_HEADER;
		if (go->repeat_seqhead)
			mpeg->flags |= GO7007_MPEG_REPEAT_SEQHEADER;
		if (go->dvd_mode)
			mpeg->flags |= GO7007_MPEG_FORCE_DVD_MODE;
		return 0;
	}
=======
>>>>>>> refs/remotes/origin/master
	case GO7007IOC_S_MD_PARAMS:
	{
		struct go7007_md_params *mdp = arg;

		if (mdp->region > 3)
			return -EINVAL;
		if (mdp->trigger > 0) {
			go->modet[mdp->region].pixel_threshold =
					mdp->pixel_threshold >> 1;
			go->modet[mdp->region].motion_threshold =
					mdp->motion_threshold >> 1;
			go->modet[mdp->region].mb_threshold =
					mdp->trigger >> 1;
			go->modet[mdp->region].enable = 1;
		} else
			go->modet[mdp->region].enable = 0;
		/* fall-through */
	}
<<<<<<< HEAD
	case GO7007IOC_G_MD_PARAMS:
	{
		struct go7007_md_params *mdp = arg;
		int region = mdp->region;

		if (mdp->region > 3)
			return -EINVAL;
		memset(mdp, 0, sizeof(struct go7007_md_params));
		mdp->region = region;
		if (!go->modet[region].enable)
			return 0;
		mdp->pixel_threshold =
			(go->modet[region].pixel_threshold << 1) + 1;
		mdp->motion_threshold =
			(go->modet[region].motion_threshold << 1) + 1;
		mdp->trigger =
			(go->modet[region].mb_threshold << 1) + 1;
		return 0;
	}
=======
>>>>>>> refs/remotes/origin/master
	case GO7007IOC_S_MD_REGION:
	{
		struct go7007_md_region *region = arg;

		if (region->region < 1 || region->region > 3)
			return -EINVAL;
		return clip_to_modet_map(go, region->region, region->clips);
	}
#endif

<<<<<<< HEAD
static ssize_t go7007_read(struct file *file, char __user *data,
		size_t count, loff_t *ppos)
{
	return -EINVAL;
}

static void go7007_vm_open(struct vm_area_struct *vma)
{
	struct go7007_buffer *gobuf = vma->vm_private_data;

	++gobuf->mapped;
}

static void go7007_vm_close(struct vm_area_struct *vma)
{
	struct go7007_buffer *gobuf = vma->vm_private_data;
	unsigned long flags;

	if (--gobuf->mapped == 0) {
		spin_lock_irqsave(&gobuf->go->spinlock, flags);
		deactivate_buffer(gobuf);
		spin_unlock_irqrestore(&gobuf->go->spinlock, flags);
	}
}

/* Copied from videobuf-dma-sg.c */
static int go7007_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct page *page;

	page = alloc_page(GFP_USER | __GFP_DMA32);
	if (!page)
		return VM_FAULT_OOM;
	clear_user_highpage(page, (unsigned long)vmf->virtual_address);
	vmf->page = page;
	return 0;
}

static struct vm_operations_struct go7007_vm_ops = {
	.open	= go7007_vm_open,
	.close	= go7007_vm_close,
	.fault	= go7007_vm_fault,
};

static int go7007_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct go7007_file *gofh = file->private_data;
	unsigned int index;

	if (gofh->go->status != STATUS_ONLINE)
		return -EIO;
	if (!(vma->vm_flags & VM_SHARED))
		return -EINVAL; /* only support VM_SHARED mapping */
	if (vma->vm_end - vma->vm_start != GO7007_BUF_SIZE)
		return -EINVAL; /* must map exactly one full buffer */
	mutex_lock(&gofh->lock);
	index = vma->vm_pgoff / GO7007_BUF_PAGES;
	if (index >= gofh->buf_count) {
		mutex_unlock(&gofh->lock);
		return -EINVAL; /* trying to map beyond requested buffers */
	}
	if (index * GO7007_BUF_PAGES != vma->vm_pgoff) {
		mutex_unlock(&gofh->lock);
		return -EINVAL; /* offset is not aligned on buffer boundary */
	}
	if (gofh->bufs[index].mapped > 0) {
		mutex_unlock(&gofh->lock);
		return -EBUSY;
	}
	gofh->bufs[index].mapped = 1;
	gofh->bufs[index].user_addr = vma->vm_start;
	vma->vm_ops = &go7007_vm_ops;
	vma->vm_flags |= VM_DONTEXPAND;
	vma->vm_flags &= ~VM_IO;
	vma->vm_private_data = &gofh->bufs[index];
	mutex_unlock(&gofh->lock);
	return 0;
}

static unsigned int go7007_poll(struct file *file, poll_table *wait)
{
	struct go7007_file *gofh = file->private_data;
	struct go7007_buffer *gobuf;

	if (list_empty(&gofh->go->stream))
		return POLLERR;
	gobuf = list_entry(gofh->go->stream.next, struct go7007_buffer, stream);
	poll_wait(file, &gofh->go->frame_waitq, wait);
	if (gobuf->state == BUF_STATE_DONE)
		return POLLIN | POLLRDNORM;
	return 0;
}

static void go7007_vfl_release(struct video_device *vfd)
{
	struct go7007 *go = video_get_drvdata(vfd);

	video_device_release(vfd);
	if (--go->ref_count == 0)
		kfree(go);
}

static struct v4l2_file_operations go7007_fops = {
	.owner		= THIS_MODULE,
	.open		= go7007_open,
	.release	= go7007_release,
	.ioctl		= video_ioctl2,
	.read		= go7007_read,
	.mmap		= go7007_mmap,
	.poll		= go7007_poll,
=======
static struct v4l2_file_operations go7007_fops = {
	.owner		= THIS_MODULE,
	.open		= v4l2_fh_open,
	.release	= vb2_fop_release,
	.unlocked_ioctl	= video_ioctl2,
	.read		= vb2_fop_read,
	.mmap		= vb2_fop_mmap,
	.poll		= vb2_fop_poll,
>>>>>>> refs/remotes/origin/master
};

static const struct v4l2_ioctl_ops video_ioctl_ops = {
	.vidioc_querycap          = vidioc_querycap,
	.vidioc_enum_fmt_vid_cap  = vidioc_enum_fmt_vid_cap,
	.vidioc_g_fmt_vid_cap     = vidioc_g_fmt_vid_cap,
	.vidioc_try_fmt_vid_cap   = vidioc_try_fmt_vid_cap,
	.vidioc_s_fmt_vid_cap     = vidioc_s_fmt_vid_cap,
<<<<<<< HEAD
	.vidioc_reqbufs           = vidioc_reqbufs,
	.vidioc_querybuf          = vidioc_querybuf,
	.vidioc_qbuf              = vidioc_qbuf,
	.vidioc_dqbuf             = vidioc_dqbuf,
=======
	.vidioc_reqbufs           = vb2_ioctl_reqbufs,
	.vidioc_querybuf          = vb2_ioctl_querybuf,
	.vidioc_qbuf              = vb2_ioctl_qbuf,
	.vidioc_dqbuf             = vb2_ioctl_dqbuf,
>>>>>>> refs/remotes/origin/master
	.vidioc_g_std             = vidioc_g_std,
	.vidioc_s_std             = vidioc_s_std,
	.vidioc_querystd          = vidioc_querystd,
	.vidioc_enum_input        = vidioc_enum_input,
	.vidioc_g_input           = vidioc_g_input,
	.vidioc_s_input           = vidioc_s_input,
<<<<<<< HEAD
	.vidioc_queryctrl         = vidioc_queryctrl,
	.vidioc_g_ctrl            = vidioc_g_ctrl,
	.vidioc_s_ctrl            = vidioc_s_ctrl,
	.vidioc_streamon          = vidioc_streamon,
	.vidioc_streamoff         = vidioc_streamoff,
=======
	.vidioc_enumaudio         = vidioc_enumaudio,
	.vidioc_g_audio           = vidioc_g_audio,
	.vidioc_s_audio           = vidioc_s_audio,
	.vidioc_streamon          = vb2_ioctl_streamon,
	.vidioc_streamoff         = vb2_ioctl_streamoff,
>>>>>>> refs/remotes/origin/master
	.vidioc_g_tuner           = vidioc_g_tuner,
	.vidioc_s_tuner           = vidioc_s_tuner,
	.vidioc_g_frequency       = vidioc_g_frequency,
	.vidioc_s_frequency       = vidioc_s_frequency,
	.vidioc_g_parm            = vidioc_g_parm,
	.vidioc_s_parm            = vidioc_s_parm,
	.vidioc_enum_framesizes   = vidioc_enum_framesizes,
	.vidioc_enum_frameintervals = vidioc_enum_frameintervals,
<<<<<<< HEAD
	.vidioc_cropcap           = vidioc_cropcap,
	.vidioc_g_crop            = vidioc_g_crop,
	.vidioc_s_crop            = vidioc_s_crop,
	.vidioc_g_jpegcomp        = vidioc_g_jpegcomp,
	.vidioc_s_jpegcomp        = vidioc_s_jpegcomp,
=======
	.vidioc_log_status        = vidioc_log_status,
	.vidioc_subscribe_event   = v4l2_ctrl_subscribe_event,
	.vidioc_unsubscribe_event = v4l2_event_unsubscribe,
>>>>>>> refs/remotes/origin/master
};

static struct video_device go7007_template = {
	.name		= "go7007",
	.fops		= &go7007_fops,
<<<<<<< HEAD
	.release	= go7007_vfl_release,
	.ioctl_ops	= &video_ioctl_ops,
	.tvnorms	= V4L2_STD_ALL,
	.current_norm	= V4L2_STD_NTSC,
};

int go7007_v4l2_init(struct go7007 *go)
{
	int rv;

	go->video_dev = video_device_alloc();
	if (go->video_dev == NULL)
		return -ENOMEM;
	*go->video_dev = go7007_template;
	go->video_dev->parent = go->dev;
	rv = video_register_device(go->video_dev, VFL_TYPE_GRABBER, -1);
	if (rv < 0) {
		video_device_release(go->video_dev);
		go->video_dev = NULL;
		return rv;
	}
	rv = v4l2_device_register(go->dev, &go->v4l2_dev);
	if (rv < 0) {
		video_device_release(go->video_dev);
		go->video_dev = NULL;
		return rv;
	}
	video_set_drvdata(go->video_dev, go);
	++go->ref_count;
	printk(KERN_INFO "%s: registered device %s [v4l2]\n",
	       go->video_dev->name, video_device_node_name(go->video_dev));
=======
	.release	= video_device_release_empty,
	.ioctl_ops	= &video_ioctl_ops,
	.tvnorms	= V4L2_STD_ALL,
};

int go7007_v4l2_ctrl_init(struct go7007 *go)
{
	struct v4l2_ctrl_handler *hdl = &go->hdl;
	struct v4l2_ctrl *ctrl;

	v4l2_ctrl_handler_init(hdl, 13);
	go->mpeg_video_gop_size = v4l2_ctrl_new_std(hdl, NULL,
			V4L2_CID_MPEG_VIDEO_GOP_SIZE, 0, 34, 1, 15);
	go->mpeg_video_gop_closure = v4l2_ctrl_new_std(hdl, NULL,
			V4L2_CID_MPEG_VIDEO_GOP_CLOSURE, 0, 1, 1, 1);
	go->mpeg_video_bitrate = v4l2_ctrl_new_std(hdl, NULL,
			V4L2_CID_MPEG_VIDEO_BITRATE,
			64000, 10000000, 1, 9800000);
	go->mpeg_video_b_frames = v4l2_ctrl_new_std(hdl, NULL,
			V4L2_CID_MPEG_VIDEO_B_FRAMES, 0, 2, 2, 0);
	go->mpeg_video_rep_seqheader = v4l2_ctrl_new_std(hdl, NULL,
			V4L2_CID_MPEG_VIDEO_REPEAT_SEQ_HEADER, 0, 1, 1, 1);

	go->mpeg_video_aspect_ratio = v4l2_ctrl_new_std_menu(hdl, NULL,
			V4L2_CID_MPEG_VIDEO_ASPECT,
			V4L2_MPEG_VIDEO_ASPECT_16x9, 0,
			V4L2_MPEG_VIDEO_ASPECT_1x1);
	ctrl = v4l2_ctrl_new_std(hdl, NULL,
			V4L2_CID_JPEG_ACTIVE_MARKER, 0,
			V4L2_JPEG_ACTIVE_MARKER_DQT | V4L2_JPEG_ACTIVE_MARKER_DHT, 0,
			V4L2_JPEG_ACTIVE_MARKER_DQT | V4L2_JPEG_ACTIVE_MARKER_DHT);
	if (ctrl)
		ctrl->flags |= V4L2_CTRL_FLAG_READ_ONLY;
	if (hdl->error) {
		int rv = hdl->error;

		v4l2_err(&go->v4l2_dev, "Could not register controls\n");
		return rv;
	}
	go->v4l2_dev.ctrl_handler = hdl;
	return 0;
}

int go7007_v4l2_init(struct go7007 *go)
{
	struct video_device *vdev = &go->vdev;
	int rv;

	mutex_init(&go->serialize_lock);
	mutex_init(&go->queue_lock);

	INIT_LIST_HEAD(&go->vidq_active);
	go->vidq.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	go->vidq.io_modes = VB2_MMAP | VB2_USERPTR | VB2_READ;
	go->vidq.ops = &go7007_video_qops;
	go->vidq.mem_ops = &vb2_vmalloc_memops;
	go->vidq.drv_priv = go;
	go->vidq.buf_struct_size = sizeof(struct go7007_buffer);
	go->vidq.timestamp_type = V4L2_BUF_FLAG_TIMESTAMP_MONOTONIC;
	go->vidq.lock = &go->queue_lock;
	rv = vb2_queue_init(&go->vidq);
	if (rv)
		return rv;
	*vdev = go7007_template;
	vdev->lock = &go->serialize_lock;
	vdev->queue = &go->vidq;
	set_bit(V4L2_FL_USE_FH_PRIO, &vdev->flags);
	video_set_drvdata(vdev, go);
	vdev->v4l2_dev = &go->v4l2_dev;
	if (!v4l2_device_has_op(&go->v4l2_dev, video, querystd))
		v4l2_disable_ioctl(vdev, VIDIOC_QUERYSTD);
	if (!(go->board_info->flags & GO7007_BOARD_HAS_TUNER)) {
		v4l2_disable_ioctl(vdev, VIDIOC_S_FREQUENCY);
		v4l2_disable_ioctl(vdev, VIDIOC_G_FREQUENCY);
		v4l2_disable_ioctl(vdev, VIDIOC_S_TUNER);
		v4l2_disable_ioctl(vdev, VIDIOC_G_TUNER);
	} else {
		struct v4l2_frequency f = {
			.type = V4L2_TUNER_ANALOG_TV,
			.frequency = 980,
		};

		call_all(&go->v4l2_dev, tuner, s_frequency, &f);
	}
	if (!(go->board_info->sensor_flags & GO7007_SENSOR_TV)) {
		v4l2_disable_ioctl(vdev, VIDIOC_G_STD);
		v4l2_disable_ioctl(vdev, VIDIOC_S_STD);
		vdev->tvnorms = 0;
	}
	if (go->board_info->sensor_flags & GO7007_SENSOR_SCALING)
		v4l2_disable_ioctl(vdev, VIDIOC_ENUM_FRAMESIZES);
	if (go->board_info->num_aud_inputs == 0) {
		v4l2_disable_ioctl(vdev, VIDIOC_G_AUDIO);
		v4l2_disable_ioctl(vdev, VIDIOC_S_AUDIO);
		v4l2_disable_ioctl(vdev, VIDIOC_ENUMAUDIO);
	}
	/* Setup correct crystal frequency on this board */
	if (go->board_info->sensor_flags & GO7007_SENSOR_SAA7115)
		v4l2_subdev_call(go->sd_video, video, s_crystal_freq,
				SAA7115_FREQ_24_576_MHZ,
				SAA7115_FREQ_FL_APLL | SAA7115_FREQ_FL_UCGC |
				SAA7115_FREQ_FL_DOUBLE_ASCLK);
	go7007_s_input(go);
	if (go->board_info->sensor_flags & GO7007_SENSOR_TV)
		go7007_s_std(go);
	rv = video_register_device(vdev, VFL_TYPE_GRABBER, -1);
	if (rv < 0)
		return rv;
	dev_info(go->dev, "registered device %s [v4l2]\n",
		 video_device_node_name(vdev));
>>>>>>> refs/remotes/origin/master

	return 0;
}

void go7007_v4l2_remove(struct go7007 *go)
{
<<<<<<< HEAD
	unsigned long flags;

	mutex_lock(&go->hw_lock);
	if (go->streaming) {
		go->streaming = 0;
		go7007_stream_stop(go);
		spin_lock_irqsave(&go->spinlock, flags);
		abort_queued(go);
		spin_unlock_irqrestore(&go->spinlock, flags);
	}
	mutex_unlock(&go->hw_lock);
	if (go->video_dev)
		video_unregister_device(go->video_dev);
	v4l2_device_unregister(&go->v4l2_dev);
=======
	v4l2_ctrl_handler_free(&go->hdl);
>>>>>>> refs/remotes/origin/master
}
