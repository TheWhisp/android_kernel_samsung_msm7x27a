/*
 * camera image capture (abstract) bus driver
 *
 * Copyright (C) 2008, Guennadi Liakhovetski <kernel@pengutronix.de>
 *
 * This driver provides an interface between platform-specific camera
 * busses and camera devices. It should be used if the camera is
 * connected not over a "proper" bus like PCI or USB, but over a
 * special bus, like, for example, the Quick Capture interface on PXA270
 * SoCs. Later it should also be used for i.MX31 SoCs from Freescale.
 * It can handle multiple cameras and / or multiple busses, which can
 * be used, e.g., in stereo-vision applications.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/device.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>
#include <linux/vmalloc.h>

#include <media/soc_camera.h>
#include <media/v4l2-common.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-dev.h>
#include <media/videobuf-core.h>
#include <media/videobuf2-core.h>
#include <media/soc_mediabus.h>

/* Default to VGA resolution */
#define DEFAULT_WIDTH	640
#define DEFAULT_HEIGHT	480

#define is_streaming(ici, icd)				\
	(((ici)->ops->init_videobuf) ?			\
	 (icd)->vb_vidq.streaming :			\
	 vb2_is_streaming(&(icd)->vb2_vidq))

static LIST_HEAD(hosts);
static LIST_HEAD(devices);
static DEFINE_MUTEX(list_lock);		/* Protects the list of hosts */

<<<<<<< HEAD
static int soc_camera_power_set(struct soc_camera_device *icd,
				struct soc_camera_link *icl,
				int power_on)
{
	int ret;

	if (power_on) {
		ret = regulator_bulk_enable(icl->num_regulators,
					    icl->regulators);
		if (ret < 0) {
			dev_err(&icd->dev, "Cannot enable regulators\n");
			return ret;
		}

		if (icl->power)
			ret = icl->power(icd->pdev, power_on);
		if (ret < 0) {
			dev_err(&icd->dev,
				"Platform failed to power-on the camera.\n");

			regulator_bulk_disable(icl->num_regulators,
					       icl->regulators);
			return ret;
		}
	} else {
		ret = 0;
		if (icl->power)
			ret = icl->power(icd->pdev, 0);
		if (ret < 0) {
			dev_err(&icd->dev,
				"Platform failed to power-off the camera.\n");
			return ret;
		}

		ret = regulator_bulk_disable(icl->num_regulators,
					     icl->regulators);
		if (ret < 0) {
			dev_err(&icd->dev, "Cannot disable regulators\n");
=======
static int soc_camera_power_on(struct soc_camera_device *icd,
			       struct soc_camera_link *icl)
{
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);
	int ret = regulator_bulk_enable(icl->num_regulators,
					icl->regulators);
	if (ret < 0) {
		dev_err(icd->pdev, "Cannot enable regulators\n");
		return ret;
	}

	if (icl->power) {
		ret = icl->power(icd->pdev, 1);
		if (ret < 0) {
			dev_err(icd->pdev,
				"Platform failed to power-on the camera.\n");
			goto elinkpwr;
		}
	}

	ret = v4l2_subdev_call(sd, core, s_power, 1);
	if (ret < 0 && ret != -ENOIOCTLCMD && ret != -ENODEV)
		goto esdpwr;

	return 0;

esdpwr:
	if (icl->power)
		icl->power(icd->pdev, 0);
elinkpwr:
	regulator_bulk_disable(icl->num_regulators,
			       icl->regulators);
	return ret;
}

static int soc_camera_power_off(struct soc_camera_device *icd,
				struct soc_camera_link *icl)
{
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);
	int ret = v4l2_subdev_call(sd, core, s_power, 0);

	if (ret < 0 && ret != -ENOIOCTLCMD && ret != -ENODEV)
		return ret;

	if (icl->power) {
		ret = icl->power(icd->pdev, 0);
		if (ret < 0) {
			dev_err(icd->pdev,
				"Platform failed to power-off the camera.\n");
>>>>>>> refs/remotes/origin/cm-10.0
			return ret;
		}
	}

<<<<<<< HEAD
	return 0;
=======
	ret = regulator_bulk_disable(icl->num_regulators,
				     icl->regulators);
	if (ret < 0)
		dev_err(icd->pdev, "Cannot disable regulators\n");

	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
}

const struct soc_camera_format_xlate *soc_camera_xlate_by_fourcc(
	struct soc_camera_device *icd, unsigned int fourcc)
{
	unsigned int i;

	for (i = 0; i < icd->num_user_formats; i++)
		if (icd->user_formats[i].host_fmt->fourcc == fourcc)
			return icd->user_formats + i;
	return NULL;
}
EXPORT_SYMBOL(soc_camera_xlate_by_fourcc);

/**
<<<<<<< HEAD
 * soc_camera_apply_sensor_flags() - apply platform SOCAM_SENSOR_INVERT_* flags
 * @icl:	camera platform parameters
 * @flags:	flags to be inverted according to platform configuration
 * @return:	resulting flags
 */
unsigned long soc_camera_apply_sensor_flags(struct soc_camera_link *icl,
					    unsigned long flags)
{
	unsigned long f;

	/* If only one of the two polarities is supported, switch to the opposite */
	if (icl->flags & SOCAM_SENSOR_INVERT_HSYNC) {
		f = flags & (SOCAM_HSYNC_ACTIVE_HIGH | SOCAM_HSYNC_ACTIVE_LOW);
		if (f == SOCAM_HSYNC_ACTIVE_HIGH || f == SOCAM_HSYNC_ACTIVE_LOW)
			flags ^= SOCAM_HSYNC_ACTIVE_HIGH | SOCAM_HSYNC_ACTIVE_LOW;
	}

	if (icl->flags & SOCAM_SENSOR_INVERT_VSYNC) {
		f = flags & (SOCAM_VSYNC_ACTIVE_HIGH | SOCAM_VSYNC_ACTIVE_LOW);
		if (f == SOCAM_VSYNC_ACTIVE_HIGH || f == SOCAM_VSYNC_ACTIVE_LOW)
			flags ^= SOCAM_VSYNC_ACTIVE_HIGH | SOCAM_VSYNC_ACTIVE_LOW;
	}

	if (icl->flags & SOCAM_SENSOR_INVERT_PCLK) {
		f = flags & (SOCAM_PCLK_SAMPLE_RISING | SOCAM_PCLK_SAMPLE_FALLING);
		if (f == SOCAM_PCLK_SAMPLE_RISING || f == SOCAM_PCLK_SAMPLE_FALLING)
			flags ^= SOCAM_PCLK_SAMPLE_RISING | SOCAM_PCLK_SAMPLE_FALLING;
=======
 * soc_camera_apply_board_flags() - apply platform SOCAM_SENSOR_INVERT_* flags
 * @icl:	camera platform parameters
 * @cfg:	media bus configuration
 * @return:	resulting flags
 */
unsigned long soc_camera_apply_board_flags(struct soc_camera_link *icl,
					   const struct v4l2_mbus_config *cfg)
{
	unsigned long f, flags = cfg->flags;

	/* If only one of the two polarities is supported, switch to the opposite */
	if (icl->flags & SOCAM_SENSOR_INVERT_HSYNC) {
		f = flags & (V4L2_MBUS_HSYNC_ACTIVE_HIGH | V4L2_MBUS_HSYNC_ACTIVE_LOW);
		if (f == V4L2_MBUS_HSYNC_ACTIVE_HIGH || f == V4L2_MBUS_HSYNC_ACTIVE_LOW)
			flags ^= V4L2_MBUS_HSYNC_ACTIVE_HIGH | V4L2_MBUS_HSYNC_ACTIVE_LOW;
	}

	if (icl->flags & SOCAM_SENSOR_INVERT_VSYNC) {
		f = flags & (V4L2_MBUS_VSYNC_ACTIVE_HIGH | V4L2_MBUS_VSYNC_ACTIVE_LOW);
		if (f == V4L2_MBUS_VSYNC_ACTIVE_HIGH || f == V4L2_MBUS_VSYNC_ACTIVE_LOW)
			flags ^= V4L2_MBUS_VSYNC_ACTIVE_HIGH | V4L2_MBUS_VSYNC_ACTIVE_LOW;
	}

	if (icl->flags & SOCAM_SENSOR_INVERT_PCLK) {
		f = flags & (V4L2_MBUS_PCLK_SAMPLE_RISING | V4L2_MBUS_PCLK_SAMPLE_FALLING);
		if (f == V4L2_MBUS_PCLK_SAMPLE_RISING || f == V4L2_MBUS_PCLK_SAMPLE_FALLING)
			flags ^= V4L2_MBUS_PCLK_SAMPLE_RISING | V4L2_MBUS_PCLK_SAMPLE_FALLING;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	return flags;
}
<<<<<<< HEAD
EXPORT_SYMBOL(soc_camera_apply_sensor_flags);
=======
EXPORT_SYMBOL(soc_camera_apply_board_flags);
>>>>>>> refs/remotes/origin/cm-10.0

#define pixfmtstr(x) (x) & 0xff, ((x) >> 8) & 0xff, ((x) >> 16) & 0xff, \
	((x) >> 24) & 0xff

static int soc_camera_try_fmt(struct soc_camera_device *icd,
			      struct v4l2_format *f)
{
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
	struct v4l2_pix_format *pix = &f->fmt.pix;
	int ret;

	dev_dbg(&icd->dev, "TRY_FMT(%c%c%c%c, %ux%u)\n",
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
	struct v4l2_pix_format *pix = &f->fmt.pix;
	int ret;

	dev_dbg(icd->pdev, "TRY_FMT(%c%c%c%c, %ux%u)\n",
>>>>>>> refs/remotes/origin/cm-10.0
		pixfmtstr(pix->pixelformat), pix->width, pix->height);

	pix->bytesperline = 0;
	pix->sizeimage = 0;

	ret = ici->ops->try_fmt(icd, f);
	if (ret < 0)
		return ret;

	if (!pix->sizeimage) {
		if (!pix->bytesperline) {
			const struct soc_camera_format_xlate *xlate;

			xlate = soc_camera_xlate_by_fourcc(icd, pix->pixelformat);
			if (!xlate)
				return -EINVAL;

			ret = soc_mbus_bytes_per_line(pix->width,
						      xlate->host_fmt);
			if (ret > 0)
				pix->bytesperline = ret;
		}
		if (pix->bytesperline)
			pix->sizeimage = pix->bytesperline * pix->height;
	}

	return 0;
}

static int soc_camera_try_fmt_vid_cap(struct file *file, void *priv,
				      struct v4l2_format *f)
{
	struct soc_camera_device *icd = file->private_data;

	WARN_ON(priv != file->private_data);

	/* Only single-plane capture is supported so far */
	if (f->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	/* limit format to hardware capabilities */
	return soc_camera_try_fmt(icd, f);
}

static int soc_camera_enum_input(struct file *file, void *priv,
				 struct v4l2_input *inp)
{
<<<<<<< HEAD
	struct soc_camera_device *icd = file->private_data;
	int ret = 0;

	if (inp->index != 0)
		return -EINVAL;

	if (icd->ops->enum_input)
		ret = icd->ops->enum_input(icd, inp);
	else {
		/* default is camera */
		inp->type = V4L2_INPUT_TYPE_CAMERA;
		inp->std  = V4L2_STD_UNKNOWN;
		strcpy(inp->name, "Camera");
	}

	return ret;
=======
	if (inp->index != 0)
		return -EINVAL;

	/* default is camera */
	inp->type = V4L2_INPUT_TYPE_CAMERA;
	inp->std  = V4L2_STD_UNKNOWN;
	strcpy(inp->name, "Camera");

	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int soc_camera_g_input(struct file *file, void *priv, unsigned int *i)
{
	*i = 0;

	return 0;
}

static int soc_camera_s_input(struct file *file, void *priv, unsigned int i)
{
	if (i > 0)
		return -EINVAL;

	return 0;
}

static int soc_camera_s_std(struct file *file, void *priv, v4l2_std_id *a)
{
	struct soc_camera_device *icd = file->private_data;
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);

	return v4l2_subdev_call(sd, core, s_std, *a);
}

<<<<<<< HEAD
=======
static int soc_camera_g_std(struct file *file, void *priv, v4l2_std_id *a)
{
	struct soc_camera_device *icd = file->private_data;
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);

	return v4l2_subdev_call(sd, core, g_std, a);
}

>>>>>>> refs/remotes/origin/cm-10.0
static int soc_camera_enum_fsizes(struct file *file, void *fh,
					 struct v4l2_frmsizeenum *fsize)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	return ici->ops->enum_fsizes(icd, fsize);
}

static int soc_camera_reqbufs(struct file *file, void *priv,
			      struct v4l2_requestbuffers *p)
{
	int ret;
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	WARN_ON(priv != file->private_data);

	if (icd->streamer && icd->streamer != file)
		return -EBUSY;

	if (ici->ops->init_videobuf) {
		ret = videobuf_reqbufs(&icd->vb_vidq, p);
		if (ret < 0)
			return ret;

		ret = ici->ops->reqbufs(icd, p);
	} else {
		ret = vb2_reqbufs(&icd->vb2_vidq, p);
	}

	if (!ret && !icd->streamer)
		icd->streamer = file;

	return ret;
}

static int soc_camera_querybuf(struct file *file, void *priv,
			       struct v4l2_buffer *p)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	WARN_ON(priv != file->private_data);

	if (ici->ops->init_videobuf)
		return videobuf_querybuf(&icd->vb_vidq, p);
	else
		return vb2_querybuf(&icd->vb2_vidq, p);
}

static int soc_camera_qbuf(struct file *file, void *priv,
			   struct v4l2_buffer *p)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	WARN_ON(priv != file->private_data);

	if (icd->streamer != file)
		return -EBUSY;

	if (ici->ops->init_videobuf)
		return videobuf_qbuf(&icd->vb_vidq, p);
	else
		return vb2_qbuf(&icd->vb2_vidq, p);
}

static int soc_camera_dqbuf(struct file *file, void *priv,
			    struct v4l2_buffer *p)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	WARN_ON(priv != file->private_data);

	if (icd->streamer != file)
		return -EBUSY;

	if (ici->ops->init_videobuf)
		return videobuf_dqbuf(&icd->vb_vidq, p, file->f_flags & O_NONBLOCK);
	else
		return vb2_dqbuf(&icd->vb2_vidq, p, file->f_flags & O_NONBLOCK);
}

<<<<<<< HEAD
=======
static int soc_camera_create_bufs(struct file *file, void *priv,
			    struct v4l2_create_buffers *create)
{
	struct soc_camera_device *icd = file->private_data;
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);

	/* videobuf2 only */
	if (ici->ops->init_videobuf)
		return -EINVAL;
	else
		return vb2_create_bufs(&icd->vb2_vidq, create);
}

static int soc_camera_prepare_buf(struct file *file, void *priv,
				  struct v4l2_buffer *b)
{
	struct soc_camera_device *icd = file->private_data;
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);

	/* videobuf2 only */
	if (ici->ops->init_videobuf)
		return -EINVAL;
	else
		return vb2_prepare_buf(&icd->vb2_vidq, b);
}

>>>>>>> refs/remotes/origin/cm-10.0
/* Always entered with .video_lock held */
static int soc_camera_init_user_formats(struct soc_camera_device *icd)
{
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned int i, fmts = 0, raw_fmts = 0;
	int ret;
	enum v4l2_mbus_pixelcode code;

	while (!v4l2_subdev_call(sd, video, enum_mbus_fmt, raw_fmts, &code))
		raw_fmts++;

	if (!ici->ops->get_formats)
		/*
		 * Fallback mode - the host will have to serve all
		 * sensor-provided formats one-to-one to the user
		 */
		fmts = raw_fmts;
	else
		/*
		 * First pass - only count formats this host-sensor
		 * configuration can provide
		 */
		for (i = 0; i < raw_fmts; i++) {
			ret = ici->ops->get_formats(icd, i, NULL);
			if (ret < 0)
				return ret;
			fmts += ret;
		}

	if (!fmts)
		return -ENXIO;

	icd->user_formats =
		vmalloc(fmts * sizeof(struct soc_camera_format_xlate));
	if (!icd->user_formats)
		return -ENOMEM;

<<<<<<< HEAD
	dev_dbg(&icd->dev, "Found %d supported formats.\n", fmts);
=======
	dev_dbg(icd->pdev, "Found %d supported formats.\n", fmts);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Second pass - actually fill data formats */
	fmts = 0;
	for (i = 0; i < raw_fmts; i++)
		if (!ici->ops->get_formats) {
			v4l2_subdev_call(sd, video, enum_mbus_fmt, i, &code);
			icd->user_formats[fmts].host_fmt =
				soc_mbus_get_fmtdesc(code);
			if (icd->user_formats[fmts].host_fmt)
				icd->user_formats[fmts++].code = code;
		} else {
			ret = ici->ops->get_formats(icd, i,
						    &icd->user_formats[fmts]);
			if (ret < 0)
				goto egfmt;
			fmts += ret;
		}

	icd->num_user_formats = fmts;
	icd->current_fmt = &icd->user_formats[0];

	return 0;

egfmt:
	vfree(icd->user_formats);
	return ret;
}

/* Always entered with .video_lock held */
static void soc_camera_free_user_formats(struct soc_camera_device *icd)
{
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	if (ici->ops->put_formats)
		ici->ops->put_formats(icd);
	icd->current_fmt = NULL;
	icd->num_user_formats = 0;
	vfree(icd->user_formats);
	icd->user_formats = NULL;
}

/* Called with .vb_lock held, or from the first open(2), see comment there */
static int soc_camera_set_fmt(struct soc_camera_device *icd,
			      struct v4l2_format *f)
{
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
	struct v4l2_pix_format *pix = &f->fmt.pix;
	int ret;

	dev_dbg(&icd->dev, "S_FMT(%c%c%c%c, %ux%u)\n",
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
	struct v4l2_pix_format *pix = &f->fmt.pix;
	int ret;

	dev_dbg(icd->pdev, "S_FMT(%c%c%c%c, %ux%u)\n",
>>>>>>> refs/remotes/origin/cm-10.0
		pixfmtstr(pix->pixelformat), pix->width, pix->height);

	/* We always call try_fmt() before set_fmt() or set_crop() */
	ret = soc_camera_try_fmt(icd, f);
	if (ret < 0)
		return ret;

	ret = ici->ops->set_fmt(icd, f);
	if (ret < 0) {
		return ret;
	} else if (!icd->current_fmt ||
		   icd->current_fmt->host_fmt->fourcc != pix->pixelformat) {
<<<<<<< HEAD
		dev_err(&icd->dev,
=======
		dev_err(icd->pdev,
>>>>>>> refs/remotes/origin/cm-10.0
			"Host driver hasn't set up current format correctly!\n");
		return -EINVAL;
	}

	icd->user_width		= pix->width;
	icd->user_height	= pix->height;
	icd->bytesperline	= pix->bytesperline;
	icd->sizeimage		= pix->sizeimage;
	icd->colorspace		= pix->colorspace;
	icd->field		= pix->field;
	if (ici->ops->init_videobuf)
		icd->vb_vidq.field = pix->field;

<<<<<<< HEAD
	dev_dbg(&icd->dev, "set width: %d height: %d\n",
		icd->user_width, icd->user_height);

	/* set physical bus parameters */
	return ici->ops->set_bus_param(icd, pix->pixelformat);
=======
	dev_dbg(icd->pdev, "set width: %d height: %d\n",
		icd->user_width, icd->user_height);

	/* set physical bus parameters */
	return ici->ops->set_bus_param(icd);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int soc_camera_open(struct file *file)
{
	struct video_device *vdev = video_devdata(file);
<<<<<<< HEAD
	struct soc_camera_device *icd = container_of(vdev->parent,
						     struct soc_camera_device,
						     dev);
=======
	struct soc_camera_device *icd = dev_get_drvdata(vdev->parent);
>>>>>>> refs/remotes/origin/cm-10.0
	struct soc_camera_link *icl = to_soc_camera_link(icd);
	struct soc_camera_host *ici;
	int ret;

<<<<<<< HEAD
	if (!icd->ops)
		/* No device driver attached */
		return -ENODEV;

	ici = to_soc_camera_host(icd->dev.parent);

	if (!try_module_get(ici->ops->owner)) {
		dev_err(&icd->dev, "Couldn't lock capture bus driver.\n");
=======
	if (!to_soc_camera_control(icd))
		/* No device driver attached */
		return -ENODEV;

	ici = to_soc_camera_host(icd->parent);

	if (!try_module_get(ici->ops->owner)) {
		dev_err(icd->pdev, "Couldn't lock capture bus driver.\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;
	}

	icd->use_count++;

	/* Now we really have to activate the camera */
	if (icd->use_count == 1) {
		/* Restore parameters before the last close() per V4L2 API */
		struct v4l2_format f = {
			.type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
			.fmt.pix = {
				.width		= icd->user_width,
				.height		= icd->user_height,
				.field		= icd->field,
				.colorspace	= icd->colorspace,
				.pixelformat	=
					icd->current_fmt->host_fmt->fourcc,
			},
		};

<<<<<<< HEAD
		ret = soc_camera_power_set(icd, icl, 1);
		if (ret < 0)
			goto epower;

=======
>>>>>>> refs/remotes/origin/cm-10.0
		/* The camera could have been already on, try to reset */
		if (icl->reset)
			icl->reset(icd->pdev);

<<<<<<< HEAD
		ret = ici->ops->add(icd);
		if (ret < 0) {
			dev_err(&icd->dev, "Couldn't activate the camera: %d\n", ret);
			goto eiciadd;
		}

=======
		/* Don't mess with the host during probe */
		mutex_lock(&ici->host_lock);
		ret = ici->ops->add(icd);
		mutex_unlock(&ici->host_lock);
		if (ret < 0) {
			dev_err(icd->pdev, "Couldn't activate the camera: %d\n", ret);
			goto eiciadd;
		}

		ret = soc_camera_power_on(icd, icl);
		if (ret < 0)
			goto epower;

>>>>>>> refs/remotes/origin/cm-10.0
		pm_runtime_enable(&icd->vdev->dev);
		ret = pm_runtime_resume(&icd->vdev->dev);
		if (ret < 0 && ret != -ENOSYS)
			goto eresume;

		/*
		 * Try to configure with default parameters. Notice: this is the
		 * very first open, so, we cannot race against other calls,
		 * apart from someone else calling open() simultaneously, but
		 * .video_lock is protecting us against it.
		 */
		ret = soc_camera_set_fmt(icd, &f);
		if (ret < 0)
			goto esfmt;

		if (ici->ops->init_videobuf) {
			ici->ops->init_videobuf(&icd->vb_vidq, icd);
		} else {
			ret = ici->ops->init_videobuf2(&icd->vb2_vidq, icd);
			if (ret < 0)
				goto einitvb;
		}
<<<<<<< HEAD
	}

	file->private_data = icd;
	dev_dbg(&icd->dev, "camera device open\n");
=======
		v4l2_ctrl_handler_setup(&icd->ctrl_handler);
	}

	file->private_data = icd;
	dev_dbg(icd->pdev, "camera device open\n");
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;

	/*
	 * First four errors are entered with the .video_lock held
	 * and use_count == 1
	 */
einitvb:
esfmt:
	pm_runtime_disable(&icd->vdev->dev);
eresume:
<<<<<<< HEAD
	ici->ops->remove(icd);
eiciadd:
	soc_camera_power_set(icd, icl, 0);
epower:
=======
	soc_camera_power_off(icd, icl);
epower:
	ici->ops->remove(icd);
eiciadd:
>>>>>>> refs/remotes/origin/cm-10.0
	icd->use_count--;
	module_put(ici->ops->owner);

	return ret;
}

static int soc_camera_close(struct file *file)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	icd->use_count--;
	if (!icd->use_count) {
		struct soc_camera_link *icl = to_soc_camera_link(icd);

		pm_runtime_suspend(&icd->vdev->dev);
		pm_runtime_disable(&icd->vdev->dev);

<<<<<<< HEAD
		ici->ops->remove(icd);
		if (ici->ops->init_videobuf2)
			vb2_queue_release(&icd->vb2_vidq);

		soc_camera_power_set(icd, icl, 0);
=======
		if (ici->ops->init_videobuf2)
			vb2_queue_release(&icd->vb2_vidq);
		ici->ops->remove(icd);

		soc_camera_power_off(icd, icl);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	if (icd->streamer == file)
		icd->streamer = NULL;

	module_put(ici->ops->owner);

<<<<<<< HEAD
	dev_dbg(&icd->dev, "camera device close\n");
=======
	dev_dbg(icd->pdev, "camera device close\n");
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

static ssize_t soc_camera_read(struct file *file, char __user *buf,
			       size_t count, loff_t *ppos)
{
	struct soc_camera_device *icd = file->private_data;
	int err = -EINVAL;

<<<<<<< HEAD
	dev_err(&icd->dev, "camera device read not implemented\n");
=======
	dev_err(icd->pdev, "camera device read not implemented\n");
>>>>>>> refs/remotes/origin/cm-10.0

	return err;
}

static int soc_camera_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
	int err;

	dev_dbg(&icd->dev, "mmap called, vma=0x%08lx\n", (unsigned long)vma);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
	int err;

	dev_dbg(icd->pdev, "mmap called, vma=0x%08lx\n", (unsigned long)vma);
>>>>>>> refs/remotes/origin/cm-10.0

	if (icd->streamer != file)
		return -EBUSY;

	if (ici->ops->init_videobuf)
		err = videobuf_mmap_mapper(&icd->vb_vidq, vma);
	else
		err = vb2_mmap(&icd->vb2_vidq, vma);

<<<<<<< HEAD
	dev_dbg(&icd->dev, "vma start=0x%08lx, size=%ld, ret=%d\n",
=======
	dev_dbg(icd->pdev, "vma start=0x%08lx, size=%ld, ret=%d\n",
>>>>>>> refs/remotes/origin/cm-10.0
		(unsigned long)vma->vm_start,
		(unsigned long)vma->vm_end - (unsigned long)vma->vm_start,
		err);

	return err;
}

static unsigned int soc_camera_poll(struct file *file, poll_table *pt)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	if (icd->streamer != file)
		return -EBUSY;

	if (ici->ops->init_videobuf && list_empty(&icd->vb_vidq.stream)) {
<<<<<<< HEAD
		dev_err(&icd->dev, "Trying to poll with no queued buffers!\n");
=======
		dev_err(icd->pdev, "Trying to poll with no queued buffers!\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return POLLERR;
	}

	return ici->ops->poll(file, pt);
}

void soc_camera_lock(struct vb2_queue *vq)
{
	struct soc_camera_device *icd = vb2_get_drv_priv(vq);
	mutex_lock(&icd->video_lock);
}
EXPORT_SYMBOL(soc_camera_lock);

void soc_camera_unlock(struct vb2_queue *vq)
{
	struct soc_camera_device *icd = vb2_get_drv_priv(vq);
	mutex_unlock(&icd->video_lock);
}
EXPORT_SYMBOL(soc_camera_unlock);

static struct v4l2_file_operations soc_camera_fops = {
	.owner		= THIS_MODULE,
	.open		= soc_camera_open,
	.release	= soc_camera_close,
	.unlocked_ioctl	= video_ioctl2,
	.read		= soc_camera_read,
	.mmap		= soc_camera_mmap,
	.poll		= soc_camera_poll,
};

static int soc_camera_s_fmt_vid_cap(struct file *file, void *priv,
				    struct v4l2_format *f)
{
	struct soc_camera_device *icd = file->private_data;
	int ret;

	WARN_ON(priv != file->private_data);

	if (f->type != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
<<<<<<< HEAD
		dev_warn(&icd->dev, "Wrong buf-type %d\n", f->type);
=======
		dev_warn(icd->pdev, "Wrong buf-type %d\n", f->type);
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;
	}

	if (icd->streamer && icd->streamer != file)
		return -EBUSY;

<<<<<<< HEAD
	if (is_streaming(to_soc_camera_host(icd->dev.parent), icd)) {
		dev_err(&icd->dev, "S_FMT denied: queue initialised\n");
=======
	if (is_streaming(to_soc_camera_host(icd->parent), icd)) {
		dev_err(icd->pdev, "S_FMT denied: queue initialised\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -EBUSY;
	}

	ret = soc_camera_set_fmt(icd, f);

	if (!ret && !icd->streamer)
		icd->streamer = file;

	return ret;
}

static int soc_camera_enum_fmt_vid_cap(struct file *file, void  *priv,
				       struct v4l2_fmtdesc *f)
{
	struct soc_camera_device *icd = file->private_data;
	const struct soc_mbus_pixelfmt *format;

	WARN_ON(priv != file->private_data);

	if (f->index >= icd->num_user_formats)
		return -EINVAL;

	format = icd->user_formats[f->index].host_fmt;

	if (format->name)
		strlcpy(f->description, format->name, sizeof(f->description));
	f->pixelformat = format->fourcc;
	return 0;
}

static int soc_camera_g_fmt_vid_cap(struct file *file, void *priv,
				    struct v4l2_format *f)
{
	struct soc_camera_device *icd = file->private_data;
	struct v4l2_pix_format *pix = &f->fmt.pix;

	WARN_ON(priv != file->private_data);

	if (f->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	pix->width		= icd->user_width;
	pix->height		= icd->user_height;
	pix->bytesperline	= icd->bytesperline;
	pix->sizeimage		= icd->sizeimage;
	pix->field		= icd->field;
	pix->pixelformat	= icd->current_fmt->host_fmt->fourcc;
	pix->colorspace		= icd->colorspace;
<<<<<<< HEAD
	dev_dbg(&icd->dev, "current_fmt->fourcc: 0x%08x\n",
=======
	dev_dbg(icd->pdev, "current_fmt->fourcc: 0x%08x\n",
>>>>>>> refs/remotes/origin/cm-10.0
		icd->current_fmt->host_fmt->fourcc);
	return 0;
}

static int soc_camera_querycap(struct file *file, void  *priv,
			       struct v4l2_capability *cap)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	WARN_ON(priv != file->private_data);

	strlcpy(cap->driver, ici->drv_name, sizeof(cap->driver));
	return ici->ops->querycap(ici, cap);
}

static int soc_camera_streamon(struct file *file, void *priv,
			       enum v4l2_buf_type i)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);
	int ret;

	WARN_ON(priv != file->private_data);

	if (i != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	if (icd->streamer != file)
		return -EBUSY;

	/* This calls buf_queue from host driver's videobuf_queue_ops */
	if (ici->ops->init_videobuf)
		ret = videobuf_streamon(&icd->vb_vidq);
	else
		ret = vb2_streamon(&icd->vb2_vidq, i);

	if (!ret)
		v4l2_subdev_call(sd, video, s_stream, 1);

	return ret;
}

static int soc_camera_streamoff(struct file *file, void *priv,
				enum v4l2_buf_type i)
{
	struct soc_camera_device *icd = file->private_data;
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	WARN_ON(priv != file->private_data);

	if (i != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	if (icd->streamer != file)
		return -EBUSY;

	/*
	 * This calls buf_release from host driver's videobuf_queue_ops for all
	 * remaining buffers. When the last buffer is freed, stop capture
	 */
	if (ici->ops->init_videobuf)
		videobuf_streamoff(&icd->vb_vidq);
	else
		vb2_streamoff(&icd->vb2_vidq, i);

	v4l2_subdev_call(sd, video, s_stream, 0);

	return 0;
}

<<<<<<< HEAD
static int soc_camera_queryctrl(struct file *file, void *priv,
				struct v4l2_queryctrl *qc)
{
	struct soc_camera_device *icd = file->private_data;
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
	int i;

	WARN_ON(priv != file->private_data);

	if (!qc->id)
		return -EINVAL;

	/* First check host controls */
	for (i = 0; i < ici->ops->num_controls; i++)
		if (qc->id == ici->ops->controls[i].id) {
			memcpy(qc, &(ici->ops->controls[i]),
				sizeof(*qc));
			return 0;
		}

	/* Then device controls */
	for (i = 0; i < icd->ops->num_controls; i++)
		if (qc->id == icd->ops->controls[i].id) {
			memcpy(qc, &(icd->ops->controls[i]),
				sizeof(*qc));
			return 0;
		}

	return -EINVAL;
}

static int soc_camera_g_ctrl(struct file *file, void *priv,
			     struct v4l2_control *ctrl)
{
	struct soc_camera_device *icd = file->private_data;
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);
	int ret;

	WARN_ON(priv != file->private_data);

	if (ici->ops->get_ctrl) {
		ret = ici->ops->get_ctrl(icd, ctrl);
		if (ret != -ENOIOCTLCMD)
			return ret;
	}

	return v4l2_subdev_call(sd, core, g_ctrl, ctrl);
}

static int soc_camera_s_ctrl(struct file *file, void *priv,
			     struct v4l2_control *ctrl)
{
	struct soc_camera_device *icd = file->private_data;
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);
	int ret;

	WARN_ON(priv != file->private_data);

	if (ici->ops->set_ctrl) {
		ret = ici->ops->set_ctrl(icd, ctrl);
		if (ret != -ENOIOCTLCMD)
			return ret;
	}

	return v4l2_subdev_call(sd, core, s_ctrl, ctrl);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static int soc_camera_cropcap(struct file *file, void *fh,
			      struct v4l2_cropcap *a)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	return ici->ops->cropcap(icd, a);
}

static int soc_camera_g_crop(struct file *file, void *fh,
			     struct v4l2_crop *a)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0
	int ret;

	ret = ici->ops->get_crop(icd, a);

	return ret;
}

/*
 * According to the V4L2 API, drivers shall not update the struct v4l2_crop
 * argument with the actual geometry, instead, the user shall use G_CROP to
 * retrieve it.
 */
static int soc_camera_s_crop(struct file *file, void *fh,
			     struct v4l2_crop *a)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0
	struct v4l2_rect *rect = &a->c;
	struct v4l2_crop current_crop;
	int ret;

	if (a->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

<<<<<<< HEAD
	dev_dbg(&icd->dev, "S_CROP(%ux%u@%u:%u)\n",
=======
	dev_dbg(icd->pdev, "S_CROP(%ux%u@%u:%u)\n",
>>>>>>> refs/remotes/origin/cm-10.0
		rect->width, rect->height, rect->left, rect->top);

	/* If get_crop fails, we'll let host and / or client drivers decide */
	ret = ici->ops->get_crop(icd, &current_crop);

	/* Prohibit window size change with initialised buffers */
	if (ret < 0) {
<<<<<<< HEAD
		dev_err(&icd->dev,
=======
		dev_err(icd->pdev,
>>>>>>> refs/remotes/origin/cm-10.0
			"S_CROP denied: getting current crop failed\n");
	} else if ((a->c.width == current_crop.c.width &&
		    a->c.height == current_crop.c.height) ||
		   !is_streaming(ici, icd)) {
		/* same size or not streaming - use .set_crop() */
		ret = ici->ops->set_crop(icd, a);
	} else if (ici->ops->set_livecrop) {
		ret = ici->ops->set_livecrop(icd, a);
	} else {
<<<<<<< HEAD
		dev_err(&icd->dev,
=======
		dev_err(icd->pdev,
>>>>>>> refs/remotes/origin/cm-10.0
			"S_CROP denied: queue initialised and sizes differ\n");
		ret = -EBUSY;
	}

	return ret;
}

static int soc_camera_g_parm(struct file *file, void *fh,
			     struct v4l2_streamparm *a)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	if (ici->ops->get_parm)
		return ici->ops->get_parm(icd, a);

	return -ENOIOCTLCMD;
}

static int soc_camera_s_parm(struct file *file, void *fh,
			     struct v4l2_streamparm *a)
{
	struct soc_camera_device *icd = file->private_data;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	if (ici->ops->set_parm)
		return ici->ops->set_parm(icd, a);

	return -ENOIOCTLCMD;
}

static int soc_camera_g_chip_ident(struct file *file, void *fh,
				   struct v4l2_dbg_chip_ident *id)
{
	struct soc_camera_device *icd = file->private_data;
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);

	return v4l2_subdev_call(sd, core, g_chip_ident, id);
}

#ifdef CONFIG_VIDEO_ADV_DEBUG
static int soc_camera_g_register(struct file *file, void *fh,
				 struct v4l2_dbg_register *reg)
{
	struct soc_camera_device *icd = file->private_data;
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);

	return v4l2_subdev_call(sd, core, g_register, reg);
}

static int soc_camera_s_register(struct file *file, void *fh,
				 struct v4l2_dbg_register *reg)
{
	struct soc_camera_device *icd = file->private_data;
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);

	return v4l2_subdev_call(sd, core, s_register, reg);
}
#endif

<<<<<<< HEAD
=======
static int soc_camera_probe(struct soc_camera_device *icd);

>>>>>>> refs/remotes/origin/cm-10.0
/* So far this function cannot fail */
static void scan_add_host(struct soc_camera_host *ici)
{
	struct soc_camera_device *icd;

<<<<<<< HEAD
	mutex_lock(&list_lock);
=======
	mutex_lock(&ici->host_lock);
>>>>>>> refs/remotes/origin/cm-10.0

	list_for_each_entry(icd, &devices, list) {
		if (icd->iface == ici->nr) {
			int ret;
<<<<<<< HEAD
			icd->dev.parent = ici->v4l2_dev.dev;
			dev_set_name(&icd->dev, "%u-%u", icd->iface,
				     icd->devnum);
			ret = device_register(&icd->dev);
			if (ret < 0) {
				icd->dev.parent = NULL;
				dev_err(&icd->dev,
					"Cannot register device: %d\n", ret);
			}
		}
	}

	mutex_unlock(&list_lock);
=======

			icd->parent = ici->v4l2_dev.dev;
			ret = soc_camera_probe(icd);
		}
	}

	mutex_unlock(&ici->host_lock);
>>>>>>> refs/remotes/origin/cm-10.0
}

#ifdef CONFIG_I2C_BOARDINFO
static int soc_camera_init_i2c(struct soc_camera_device *icd,
			       struct soc_camera_link *icl)
{
	struct i2c_client *client;
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0
	struct i2c_adapter *adap = i2c_get_adapter(icl->i2c_adapter_id);
	struct v4l2_subdev *subdev;

	if (!adap) {
<<<<<<< HEAD
		dev_err(&icd->dev, "Cannot get I2C adapter #%d. No driver?\n",
=======
		dev_err(icd->pdev, "Cannot get I2C adapter #%d. No driver?\n",
>>>>>>> refs/remotes/origin/cm-10.0
			icl->i2c_adapter_id);
		goto ei2cga;
	}

<<<<<<< HEAD
	icl->board_info->platform_data = icd;
=======
	icl->board_info->platform_data = icl;
>>>>>>> refs/remotes/origin/cm-10.0

	subdev = v4l2_i2c_new_subdev_board(&ici->v4l2_dev, adap,
				icl->board_info, NULL);
	if (!subdev)
		goto ei2cnd;

	client = v4l2_get_subdevdata(subdev);

	/* Use to_i2c_client(dev) to recover the i2c client */
<<<<<<< HEAD
	dev_set_drvdata(&icd->dev, &client->dev);
=======
	icd->control = &client->dev;
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
ei2cnd:
	i2c_put_adapter(adap);
ei2cga:
	return -ENODEV;
}

static void soc_camera_free_i2c(struct soc_camera_device *icd)
{
	struct i2c_client *client =
		to_i2c_client(to_soc_camera_control(icd));
	struct i2c_adapter *adap = client->adapter;
<<<<<<< HEAD
	dev_set_drvdata(&icd->dev, NULL);
=======

	icd->control = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
	v4l2_device_unregister_subdev(i2c_get_clientdata(client));
	i2c_unregister_device(client);
	i2c_put_adapter(adap);
}
#else
#define soc_camera_init_i2c(icd, icl)	(-ENODEV)
#define soc_camera_free_i2c(icd)	do {} while (0)
#endif

static int soc_camera_video_start(struct soc_camera_device *icd);
static int video_dev_create(struct soc_camera_device *icd);
/* Called during host-driver probe */
<<<<<<< HEAD
static int soc_camera_probe(struct device *dev)
{
	struct soc_camera_device *icd = to_soc_camera_dev(dev);
	struct soc_camera_host *ici = to_soc_camera_host(dev->parent);
=======
static int soc_camera_probe(struct soc_camera_device *icd)
{
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0
	struct soc_camera_link *icl = to_soc_camera_link(icd);
	struct device *control = NULL;
	struct v4l2_subdev *sd;
	struct v4l2_mbus_framefmt mf;
	int ret;

<<<<<<< HEAD
	dev_info(dev, "Probing %s\n", dev_name(dev));
=======
	dev_info(icd->pdev, "Probing %s\n", dev_name(icd->pdev));

	/*
	 * Currently the subdev with the largest number of controls (13) is
	 * ov6550. So let's pick 16 as a hint for the control handler. Note
	 * that this is a hint only: too large and you waste some memory, too
	 * small and there is a (very) small performance hit when looking up
	 * controls in the internal hash.
	 */
	ret = v4l2_ctrl_handler_init(&icd->ctrl_handler, 16);
	if (ret < 0)
		return ret;
>>>>>>> refs/remotes/origin/cm-10.0

	ret = regulator_bulk_get(icd->pdev, icl->num_regulators,
				 icl->regulators);
	if (ret < 0)
		goto ereg;

<<<<<<< HEAD
	ret = soc_camera_power_set(icd, icl, 1);
	if (ret < 0)
		goto epower;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* The camera could have been already on, try to reset */
	if (icl->reset)
		icl->reset(icd->pdev);

	ret = ici->ops->add(icd);
	if (ret < 0)
		goto eadd;

<<<<<<< HEAD
=======
	/*
	 * This will not yet call v4l2_subdev_core_ops::s_power(1), because the
	 * subdevice has not been initialised yet. We'll have to call it once
	 * again after initialisation, even though it shouldn't be needed, we
	 * don't do any IO here.
	 */
	ret = soc_camera_power_on(icd, icl);
	if (ret < 0)
		goto epower;

>>>>>>> refs/remotes/origin/cm-10.0
	/* Must have icd->vdev before registering the device */
	ret = video_dev_create(icd);
	if (ret < 0)
		goto evdc;

	/* Non-i2c cameras, e.g., soc_camera_platform, have no board_info */
	if (icl->board_info) {
		ret = soc_camera_init_i2c(icd, icl);
		if (ret < 0)
			goto eadddev;
	} else if (!icl->add_device || !icl->del_device) {
		ret = -EINVAL;
		goto eadddev;
	} else {
		if (icl->module_name)
			ret = request_module(icl->module_name);

<<<<<<< HEAD
		ret = icl->add_device(icl, &icd->dev);
=======
		ret = icl->add_device(icd);
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret < 0)
			goto eadddev;

		/*
		 * FIXME: this is racy, have to use driver-binding notification,
		 * when it is available
		 */
		control = to_soc_camera_control(icd);
		if (!control || !control->driver || !dev_get_drvdata(control) ||
		    !try_module_get(control->driver->owner)) {
<<<<<<< HEAD
			icl->del_device(icl);
=======
			icl->del_device(icd);
			ret = -ENODEV;
>>>>>>> refs/remotes/origin/cm-10.0
			goto enodrv;
		}
	}

	sd = soc_camera_to_subdev(icd);
<<<<<<< HEAD
	sd->grp_id = (long)icd;
=======
	sd->grp_id = soc_camera_grp_id(icd);
	v4l2_set_subdev_hostdata(sd, icd);

	if (v4l2_ctrl_add_handler(&icd->ctrl_handler, sd->ctrl_handler))
		goto ectrl;
>>>>>>> refs/remotes/origin/cm-10.0

	/* At this point client .probe() should have run already */
	ret = soc_camera_init_user_formats(icd);
	if (ret < 0)
		goto eiufmt;

	icd->field = V4L2_FIELD_ANY;

<<<<<<< HEAD
	icd->vdev->lock = &icd->video_lock;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * ..._video_start() will create a device node, video_register_device()
	 * itself is protected against concurrent open() calls, but we also have
	 * to protect our data.
	 */
	mutex_lock(&icd->video_lock);

	ret = soc_camera_video_start(icd);
	if (ret < 0)
		goto evidstart;

<<<<<<< HEAD
=======
	ret = v4l2_subdev_call(sd, core, s_power, 1);
	if (ret < 0 && ret != -ENOIOCTLCMD)
		goto esdpwr;

>>>>>>> refs/remotes/origin/cm-10.0
	/* Try to improve our guess of a reasonable window format */
	if (!v4l2_subdev_call(sd, video, g_mbus_fmt, &mf)) {
		icd->user_width		= mf.width;
		icd->user_height	= mf.height;
		icd->colorspace		= mf.colorspace;
		icd->field		= mf.field;
	}

<<<<<<< HEAD
	/* Do we have to sysfs_remove_link() before device_unregister()? */
	if (sysfs_create_link(&icd->dev.kobj, &to_soc_camera_control(icd)->kobj,
			      "control"))
		dev_warn(&icd->dev, "Failed creating the control symlink\n");

	ici->ops->remove(icd);

	soc_camera_power_set(icd, icl, 0);
=======
	ici->ops->remove(icd);

	soc_camera_power_off(icd, icl);
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_unlock(&icd->video_lock);

	return 0;

<<<<<<< HEAD
=======
esdpwr:
	video_unregister_device(icd->vdev);
>>>>>>> refs/remotes/origin/cm-10.0
evidstart:
	mutex_unlock(&icd->video_lock);
	soc_camera_free_user_formats(icd);
eiufmt:
<<<<<<< HEAD
	if (icl->board_info) {
		soc_camera_free_i2c(icd);
	} else {
		icl->del_device(icl);
=======
ectrl:
	if (icl->board_info) {
		soc_camera_free_i2c(icd);
	} else {
		icl->del_device(icd);
>>>>>>> refs/remotes/origin/cm-10.0
		module_put(control->driver->owner);
	}
enodrv:
eadddev:
	video_device_release(icd->vdev);
<<<<<<< HEAD
evdc:
	ici->ops->remove(icd);
eadd:
	soc_camera_power_set(icd, icl, 0);
epower:
	regulator_bulk_free(icl->num_regulators, icl->regulators);
ereg:
=======
	icd->vdev = NULL;
evdc:
	soc_camera_power_off(icd, icl);
epower:
	ici->ops->remove(icd);
eadd:
	regulator_bulk_free(icl->num_regulators, icl->regulators);
ereg:
	v4l2_ctrl_handler_free(&icd->ctrl_handler);
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

/*
 * This is called on device_unregister, which only means we have to disconnect
 * from the host, but not remove ourselves from the device list
 */
<<<<<<< HEAD
static int soc_camera_remove(struct device *dev)
{
	struct soc_camera_device *icd = to_soc_camera_dev(dev);
	struct soc_camera_link *icl = to_soc_camera_link(icd);
	struct video_device *vdev = icd->vdev;

	BUG_ON(!dev->parent);

=======
static int soc_camera_remove(struct soc_camera_device *icd)
{
	struct soc_camera_link *icl = to_soc_camera_link(icd);
	struct video_device *vdev = icd->vdev;

	BUG_ON(!icd->parent);

	v4l2_ctrl_handler_free(&icd->ctrl_handler);
>>>>>>> refs/remotes/origin/cm-10.0
	if (vdev) {
		video_unregister_device(vdev);
		icd->vdev = NULL;
	}

	if (icl->board_info) {
		soc_camera_free_i2c(icd);
	} else {
<<<<<<< HEAD
		struct device_driver *drv = to_soc_camera_control(icd) ?
			to_soc_camera_control(icd)->driver : NULL;
		if (drv) {
			icl->del_device(icl);
=======
		struct device_driver *drv = to_soc_camera_control(icd)->driver;
		if (drv) {
			icl->del_device(icd);
>>>>>>> refs/remotes/origin/cm-10.0
			module_put(drv->owner);
		}
	}
	soc_camera_free_user_formats(icd);

	regulator_bulk_free(icl->num_regulators, icl->regulators);

	return 0;
}

<<<<<<< HEAD
static int soc_camera_suspend(struct device *dev, pm_message_t state)
{
	struct soc_camera_device *icd = to_soc_camera_dev(dev);
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
	int ret = 0;

	if (ici->ops->suspend)
		ret = ici->ops->suspend(icd, state);

	return ret;
}

static int soc_camera_resume(struct device *dev)
{
	struct soc_camera_device *icd = to_soc_camera_dev(dev);
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
	int ret = 0;

	if (ici->ops->resume)
		ret = ici->ops->resume(icd);

	return ret;
}

struct bus_type soc_camera_bus_type = {
	.name		= "soc-camera",
	.probe		= soc_camera_probe,
	.remove		= soc_camera_remove,
	.suspend	= soc_camera_suspend,
	.resume		= soc_camera_resume,
};
EXPORT_SYMBOL_GPL(soc_camera_bus_type);

static struct device_driver ic_drv = {
	.name	= "camera",
	.bus	= &soc_camera_bus_type,
	.owner	= THIS_MODULE,
};

static void dummy_release(struct device *dev)
{
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static int default_cropcap(struct soc_camera_device *icd,
			   struct v4l2_cropcap *a)
{
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);
	return v4l2_subdev_call(sd, video, cropcap, a);
}

static int default_g_crop(struct soc_camera_device *icd, struct v4l2_crop *a)
{
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);
	return v4l2_subdev_call(sd, video, g_crop, a);
}

static int default_s_crop(struct soc_camera_device *icd, struct v4l2_crop *a)
{
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);
	return v4l2_subdev_call(sd, video, s_crop, a);
}

static int default_g_parm(struct soc_camera_device *icd,
			  struct v4l2_streamparm *parm)
{
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);
	return v4l2_subdev_call(sd, video, g_parm, parm);
}

static int default_s_parm(struct soc_camera_device *icd,
			  struct v4l2_streamparm *parm)
{
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);
	return v4l2_subdev_call(sd, video, s_parm, parm);
}

static int default_enum_fsizes(struct soc_camera_device *icd,
			  struct v4l2_frmsizeenum *fsize)
{
	int ret;
	struct v4l2_subdev *sd = soc_camera_to_subdev(icd);
	const struct soc_camera_format_xlate *xlate;
	__u32 pixfmt = fsize->pixel_format;
	struct v4l2_frmsizeenum fsize_mbus = *fsize;

	xlate = soc_camera_xlate_by_fourcc(icd, pixfmt);
	if (!xlate)
		return -EINVAL;
	/* map xlate-code to pixel_format, sensor only handle xlate-code*/
	fsize_mbus.pixel_format = xlate->code;

	ret = v4l2_subdev_call(sd, video, enum_mbus_fsizes, &fsize_mbus);
	if (ret < 0)
		return ret;

	*fsize = fsize_mbus;
	fsize->pixel_format = pixfmt;

	return 0;
}

<<<<<<< HEAD
static void soc_camera_device_init(struct device *dev, void *pdata)
{
	dev->platform_data	= pdata;
	dev->bus		= &soc_camera_bus_type;
	dev->release		= dummy_release;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
int soc_camera_host_register(struct soc_camera_host *ici)
{
	struct soc_camera_host *ix;
	int ret;

	if (!ici || !ici->ops ||
	    !ici->ops->try_fmt ||
	    !ici->ops->set_fmt ||
	    !ici->ops->set_bus_param ||
	    !ici->ops->querycap ||
	    ((!ici->ops->init_videobuf ||
	      !ici->ops->reqbufs) &&
	     !ici->ops->init_videobuf2) ||
	    !ici->ops->add ||
	    !ici->ops->remove ||
	    !ici->ops->poll ||
	    !ici->v4l2_dev.dev)
		return -EINVAL;

	if (!ici->ops->set_crop)
		ici->ops->set_crop = default_s_crop;
	if (!ici->ops->get_crop)
		ici->ops->get_crop = default_g_crop;
	if (!ici->ops->cropcap)
		ici->ops->cropcap = default_cropcap;
	if (!ici->ops->set_parm)
		ici->ops->set_parm = default_s_parm;
	if (!ici->ops->get_parm)
		ici->ops->get_parm = default_g_parm;
	if (!ici->ops->enum_fsizes)
		ici->ops->enum_fsizes = default_enum_fsizes;

	mutex_lock(&list_lock);
	list_for_each_entry(ix, &hosts, list) {
		if (ix->nr == ici->nr) {
			ret = -EBUSY;
			goto edevreg;
		}
	}

	ret = v4l2_device_register(ici->v4l2_dev.dev, &ici->v4l2_dev);
	if (ret < 0)
		goto edevreg;

	list_add_tail(&ici->list, &hosts);
	mutex_unlock(&list_lock);

<<<<<<< HEAD
=======
	mutex_init(&ici->host_lock);
>>>>>>> refs/remotes/origin/cm-10.0
	scan_add_host(ici);

	return 0;

edevreg:
	mutex_unlock(&list_lock);
	return ret;
}
EXPORT_SYMBOL(soc_camera_host_register);

/* Unregister all clients! */
void soc_camera_host_unregister(struct soc_camera_host *ici)
{
	struct soc_camera_device *icd;

	mutex_lock(&list_lock);

	list_del(&ici->list);
<<<<<<< HEAD

	list_for_each_entry(icd, &devices, list) {
		if (icd->iface == ici->nr) {
			void *pdata = icd->dev.platform_data;
			/* The bus->remove will be called */
			device_unregister(&icd->dev);
			/*
			 * Not before device_unregister(), .remove
			 * needs parent to call ici->ops->remove().
			 * If the host module is loaded again, device_register()
			 * would complain "already initialised," since 2.6.32
			 * this is also needed to prevent use-after-free of the
			 * device private data.
			 */
			memset(&icd->dev, 0, sizeof(icd->dev));
			soc_camera_device_init(&icd->dev, pdata);
		}
	}
=======
	list_for_each_entry(icd, &devices, list)
		if (icd->iface == ici->nr && to_soc_camera_control(icd))
			soc_camera_remove(icd);
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_unlock(&list_lock);

	v4l2_device_unregister(&ici->v4l2_dev);
}
EXPORT_SYMBOL(soc_camera_host_unregister);

/* Image capture device */
static int soc_camera_device_register(struct soc_camera_device *icd)
{
	struct soc_camera_device *ix;
	int num = -1, i;

	for (i = 0; i < 256 && num < 0; i++) {
		num = i;
		/* Check if this index is available on this interface */
		list_for_each_entry(ix, &devices, list) {
			if (ix->iface == icd->iface && ix->devnum == i) {
				num = -1;
				break;
			}
		}
	}

	if (num < 0)
		/*
		 * ok, we have 256 cameras on this host...
		 * man, stay reasonable...
		 */
		return -ENOMEM;

	icd->devnum		= num;
	icd->use_count		= 0;
	icd->host_priv		= NULL;
	mutex_init(&icd->video_lock);

	list_add_tail(&icd->list, &devices);

	return 0;
}

<<<<<<< HEAD
static void soc_camera_device_unregister(struct soc_camera_device *icd)
{
	list_del(&icd->list);
}

static const struct v4l2_ioctl_ops soc_camera_ioctl_ops = {
	.vidioc_querycap	 = soc_camera_querycap,
	.vidioc_g_fmt_vid_cap    = soc_camera_g_fmt_vid_cap,
	.vidioc_enum_fmt_vid_cap = soc_camera_enum_fmt_vid_cap,
	.vidioc_s_fmt_vid_cap    = soc_camera_s_fmt_vid_cap,
=======
static const struct v4l2_ioctl_ops soc_camera_ioctl_ops = {
	.vidioc_querycap	 = soc_camera_querycap,
	.vidioc_try_fmt_vid_cap  = soc_camera_try_fmt_vid_cap,
	.vidioc_g_fmt_vid_cap    = soc_camera_g_fmt_vid_cap,
	.vidioc_s_fmt_vid_cap    = soc_camera_s_fmt_vid_cap,
	.vidioc_enum_fmt_vid_cap = soc_camera_enum_fmt_vid_cap,
>>>>>>> refs/remotes/origin/cm-10.0
	.vidioc_enum_input	 = soc_camera_enum_input,
	.vidioc_g_input		 = soc_camera_g_input,
	.vidioc_s_input		 = soc_camera_s_input,
	.vidioc_s_std		 = soc_camera_s_std,
<<<<<<< HEAD
	.vidioc_enum_framesizes  = soc_camera_enum_fsizes,
	.vidioc_reqbufs		 = soc_camera_reqbufs,
	.vidioc_try_fmt_vid_cap  = soc_camera_try_fmt_vid_cap,
	.vidioc_querybuf	 = soc_camera_querybuf,
	.vidioc_qbuf		 = soc_camera_qbuf,
	.vidioc_dqbuf		 = soc_camera_dqbuf,
	.vidioc_streamon	 = soc_camera_streamon,
	.vidioc_streamoff	 = soc_camera_streamoff,
	.vidioc_queryctrl	 = soc_camera_queryctrl,
	.vidioc_g_ctrl		 = soc_camera_g_ctrl,
	.vidioc_s_ctrl		 = soc_camera_s_ctrl,
=======
	.vidioc_g_std		 = soc_camera_g_std,
	.vidioc_enum_framesizes  = soc_camera_enum_fsizes,
	.vidioc_reqbufs		 = soc_camera_reqbufs,
	.vidioc_querybuf	 = soc_camera_querybuf,
	.vidioc_qbuf		 = soc_camera_qbuf,
	.vidioc_dqbuf		 = soc_camera_dqbuf,
	.vidioc_create_bufs	 = soc_camera_create_bufs,
	.vidioc_prepare_buf	 = soc_camera_prepare_buf,
	.vidioc_streamon	 = soc_camera_streamon,
	.vidioc_streamoff	 = soc_camera_streamoff,
>>>>>>> refs/remotes/origin/cm-10.0
	.vidioc_cropcap		 = soc_camera_cropcap,
	.vidioc_g_crop		 = soc_camera_g_crop,
	.vidioc_s_crop		 = soc_camera_s_crop,
	.vidioc_g_parm		 = soc_camera_g_parm,
	.vidioc_s_parm		 = soc_camera_s_parm,
	.vidioc_g_chip_ident     = soc_camera_g_chip_ident,
#ifdef CONFIG_VIDEO_ADV_DEBUG
	.vidioc_g_register	 = soc_camera_g_register,
	.vidioc_s_register	 = soc_camera_s_register,
#endif
};

static int video_dev_create(struct soc_camera_device *icd)
{
<<<<<<< HEAD
	struct soc_camera_host *ici = to_soc_camera_host(icd->dev.parent);
=======
	struct soc_camera_host *ici = to_soc_camera_host(icd->parent);
>>>>>>> refs/remotes/origin/cm-10.0
	struct video_device *vdev = video_device_alloc();

	if (!vdev)
		return -ENOMEM;

	strlcpy(vdev->name, ici->drv_name, sizeof(vdev->name));

<<<<<<< HEAD
	vdev->parent		= &icd->dev;
=======
	vdev->parent		= icd->pdev;
>>>>>>> refs/remotes/origin/cm-10.0
	vdev->current_norm	= V4L2_STD_UNKNOWN;
	vdev->fops		= &soc_camera_fops;
	vdev->ioctl_ops		= &soc_camera_ioctl_ops;
	vdev->release		= video_device_release;
	vdev->tvnorms		= V4L2_STD_UNKNOWN;
<<<<<<< HEAD
=======
	vdev->ctrl_handler	= &icd->ctrl_handler;
	vdev->lock		= &icd->video_lock;
>>>>>>> refs/remotes/origin/cm-10.0

	icd->vdev = vdev;

	return 0;
}

/*
 * Called from soc_camera_probe() above (with .video_lock held???)
 */
static int soc_camera_video_start(struct soc_camera_device *icd)
{
	const struct device_type *type = icd->vdev->dev.type;
	int ret;

<<<<<<< HEAD
	if (!icd->dev.parent)
		return -ENODEV;

	if (!icd->ops ||
	    !icd->ops->query_bus_param ||
	    !icd->ops->set_bus_param)
		return -EINVAL;

	ret = video_register_device(icd->vdev, VFL_TYPE_GRABBER, -1);
	if (ret < 0) {
		dev_err(&icd->dev, "video_register_device failed: %d\n", ret);
=======
	if (!icd->parent)
		return -ENODEV;

	ret = video_register_device(icd->vdev, VFL_TYPE_GRABBER, -1);
	if (ret < 0) {
		dev_err(icd->pdev, "video_register_device failed: %d\n", ret);
>>>>>>> refs/remotes/origin/cm-10.0
		return ret;
	}

	/* Restore device type, possibly set by the subdevice driver */
	icd->vdev->dev.type = type;

	return 0;
}

static int __devinit soc_camera_pdrv_probe(struct platform_device *pdev)
{
	struct soc_camera_link *icl = pdev->dev.platform_data;
	struct soc_camera_device *icd;
	int ret;

	if (!icl)
		return -EINVAL;

	icd = kzalloc(sizeof(*icd), GFP_KERNEL);
	if (!icd)
		return -ENOMEM;

	icd->iface = icl->bus_id;
<<<<<<< HEAD
=======
	icd->link = icl;
>>>>>>> refs/remotes/origin/cm-10.0
	icd->pdev = &pdev->dev;
	platform_set_drvdata(pdev, icd);

	ret = soc_camera_device_register(icd);
	if (ret < 0)
		goto escdevreg;

<<<<<<< HEAD
	soc_camera_device_init(&icd->dev, icl);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	icd->user_width		= DEFAULT_WIDTH;
	icd->user_height	= DEFAULT_HEIGHT;

	return 0;

escdevreg:
	kfree(icd);

	return ret;
}

/*
 * Only called on rmmod for each platform device, since they are not
 * hot-pluggable. Now we know, that all our users - hosts and devices have
 * been unloaded already
 */
static int __devexit soc_camera_pdrv_remove(struct platform_device *pdev)
{
	struct soc_camera_device *icd = platform_get_drvdata(pdev);

	if (!icd)
		return -EINVAL;

<<<<<<< HEAD
	soc_camera_device_unregister(icd);
=======
	list_del(&icd->list);
>>>>>>> refs/remotes/origin/cm-10.0

	kfree(icd);

	return 0;
}

static struct platform_driver __refdata soc_camera_pdrv = {
	.remove  = __devexit_p(soc_camera_pdrv_remove),
	.driver  = {
		.name	= "soc-camera-pdrv",
		.owner	= THIS_MODULE,
	},
};

static int __init soc_camera_init(void)
{
<<<<<<< HEAD
	int ret = bus_register(&soc_camera_bus_type);
	if (ret)
		return ret;
	ret = driver_register(&ic_drv);
	if (ret)
		goto edrvr;

	ret = platform_driver_probe(&soc_camera_pdrv, soc_camera_pdrv_probe);
	if (ret)
		goto epdr;

	return 0;

epdr:
	driver_unregister(&ic_drv);
edrvr:
	bus_unregister(&soc_camera_bus_type);
	return ret;
=======
	return platform_driver_probe(&soc_camera_pdrv, soc_camera_pdrv_probe);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void __exit soc_camera_exit(void)
{
	platform_driver_unregister(&soc_camera_pdrv);
<<<<<<< HEAD
	driver_unregister(&ic_drv);
	bus_unregister(&soc_camera_bus_type);
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

module_init(soc_camera_init);
module_exit(soc_camera_exit);

MODULE_DESCRIPTION("Image capture bus driver");
MODULE_AUTHOR("Guennadi Liakhovetski <kernel@pengutronix.de>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:soc-camera-pdrv");
