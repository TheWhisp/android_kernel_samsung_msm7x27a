/*
 * radio-timb.c Timberdale FPGA Radio driver
 * Copyright (c) 2009 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/version.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/io.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-device.h>
=======
#include <linux/io.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-event.h>
>>>>>>> refs/remotes/origin/master
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/i2c.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master
#include <media/timb_radio.h>

#define DRIVER_NAME "timb-radio"

struct timbradio {
	struct timb_radio_platform_data	pdata;
	struct v4l2_subdev	*sd_tuner;
	struct v4l2_subdev	*sd_dsp;
	struct video_device	video_dev;
	struct v4l2_device	v4l2_dev;
	struct mutex		lock;
};


static int timbradio_vidioc_querycap(struct file *file, void  *priv,
	struct v4l2_capability *v)
{
	strlcpy(v->driver, DRIVER_NAME, sizeof(v->driver));
	strlcpy(v->card, "Timberdale Radio", sizeof(v->card));
	snprintf(v->bus_info, sizeof(v->bus_info), "platform:"DRIVER_NAME);
<<<<<<< HEAD
<<<<<<< HEAD
	v->version = KERNEL_VERSION(0, 0, 1);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	v->capabilities = V4L2_CAP_TUNER | V4L2_CAP_RADIO;
=======
	v->device_caps = V4L2_CAP_TUNER | V4L2_CAP_RADIO;
	v->capabilities = v->device_caps | V4L2_CAP_DEVICE_CAPS;
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int timbradio_vidioc_g_tuner(struct file *file, void *priv,
	struct v4l2_tuner *v)
{
	struct timbradio *tr = video_drvdata(file);
	return v4l2_subdev_call(tr->sd_tuner, tuner, g_tuner, v);
}

static int timbradio_vidioc_s_tuner(struct file *file, void *priv,
<<<<<<< HEAD
	struct v4l2_tuner *v)
=======
	const struct v4l2_tuner *v)
>>>>>>> refs/remotes/origin/master
{
	struct timbradio *tr = video_drvdata(file);
	return v4l2_subdev_call(tr->sd_tuner, tuner, s_tuner, v);
}

<<<<<<< HEAD
static int timbradio_vidioc_g_input(struct file *filp, void *priv,
	unsigned int *i)
{
	*i = 0;
	return 0;
}

static int timbradio_vidioc_s_input(struct file *filp, void *priv,
	unsigned int i)
{
	return i ? -EINVAL : 0;
}

static int timbradio_vidioc_g_audio(struct file *file, void *priv,
	struct v4l2_audio *a)
{
	a->index = 0;
	strlcpy(a->name, "Radio", sizeof(a->name));
	a->capability = V4L2_AUDCAP_STEREO;
	return 0;
}

static int timbradio_vidioc_s_audio(struct file *file, void *priv,
	struct v4l2_audio *a)
{
	return a->index ? -EINVAL : 0;
}

static int timbradio_vidioc_s_frequency(struct file *file, void *priv,
	struct v4l2_frequency *f)
=======
static int timbradio_vidioc_s_frequency(struct file *file, void *priv,
	const struct v4l2_frequency *f)
>>>>>>> refs/remotes/origin/master
{
	struct timbradio *tr = video_drvdata(file);
	return v4l2_subdev_call(tr->sd_tuner, tuner, s_frequency, f);
}

static int timbradio_vidioc_g_frequency(struct file *file, void *priv,
	struct v4l2_frequency *f)
{
	struct timbradio *tr = video_drvdata(file);
	return v4l2_subdev_call(tr->sd_tuner, tuner, g_frequency, f);
}

<<<<<<< HEAD
static int timbradio_vidioc_queryctrl(struct file *file, void *priv,
	struct v4l2_queryctrl *qc)
{
	struct timbradio *tr = video_drvdata(file);
	return v4l2_subdev_call(tr->sd_dsp, core, queryctrl, qc);
}

static int timbradio_vidioc_g_ctrl(struct file *file, void *priv,
	struct v4l2_control *ctrl)
{
	struct timbradio *tr = video_drvdata(file);
	return v4l2_subdev_call(tr->sd_dsp, core, g_ctrl, ctrl);
}

static int timbradio_vidioc_s_ctrl(struct file *file, void *priv,
	struct v4l2_control *ctrl)
{
	struct timbradio *tr = video_drvdata(file);
	return v4l2_subdev_call(tr->sd_dsp, core, s_ctrl, ctrl);
}

=======
>>>>>>> refs/remotes/origin/master
static const struct v4l2_ioctl_ops timbradio_ioctl_ops = {
	.vidioc_querycap	= timbradio_vidioc_querycap,
	.vidioc_g_tuner		= timbradio_vidioc_g_tuner,
	.vidioc_s_tuner		= timbradio_vidioc_s_tuner,
	.vidioc_g_frequency	= timbradio_vidioc_g_frequency,
	.vidioc_s_frequency	= timbradio_vidioc_s_frequency,
<<<<<<< HEAD
	.vidioc_g_input		= timbradio_vidioc_g_input,
	.vidioc_s_input		= timbradio_vidioc_s_input,
	.vidioc_g_audio		= timbradio_vidioc_g_audio,
	.vidioc_s_audio		= timbradio_vidioc_s_audio,
	.vidioc_queryctrl	= timbradio_vidioc_queryctrl,
	.vidioc_g_ctrl		= timbradio_vidioc_g_ctrl,
	.vidioc_s_ctrl		= timbradio_vidioc_s_ctrl
=======
	.vidioc_log_status      = v4l2_ctrl_log_status,
	.vidioc_subscribe_event = v4l2_ctrl_subscribe_event,
	.vidioc_unsubscribe_event = v4l2_event_unsubscribe,
>>>>>>> refs/remotes/origin/master
};

static const struct v4l2_file_operations timbradio_fops = {
	.owner		= THIS_MODULE,
<<<<<<< HEAD
	.unlocked_ioctl	= video_ioctl2,
};

static int __devinit timbradio_probe(struct platform_device *pdev)
=======
	.open		= v4l2_fh_open,
	.release	= v4l2_fh_release,
	.poll		= v4l2_ctrl_poll,
	.unlocked_ioctl	= video_ioctl2,
};

static int timbradio_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct timb_radio_platform_data *pdata = pdev->dev.platform_data;
	struct timbradio *tr;
	int err;

	if (!pdata) {
		dev_err(&pdev->dev, "Platform data missing\n");
		err = -EINVAL;
		goto err;
	}

<<<<<<< HEAD
	tr = kzalloc(sizeof(*tr), GFP_KERNEL);
=======
	tr = devm_kzalloc(&pdev->dev, sizeof(*tr), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!tr) {
		err = -ENOMEM;
		goto err;
	}

	tr->pdata = *pdata;
	mutex_init(&tr->lock);

	strlcpy(tr->video_dev.name, "Timberdale Radio",
		sizeof(tr->video_dev.name));
	tr->video_dev.fops = &timbradio_fops;
	tr->video_dev.ioctl_ops = &timbradio_ioctl_ops;
	tr->video_dev.release = video_device_release_empty;
	tr->video_dev.minor = -1;
	tr->video_dev.lock = &tr->lock;
<<<<<<< HEAD
=======
	set_bit(V4L2_FL_USE_FH_PRIO, &tr->video_dev.flags);
>>>>>>> refs/remotes/origin/master

	strlcpy(tr->v4l2_dev.name, DRIVER_NAME, sizeof(tr->v4l2_dev.name));
	err = v4l2_device_register(NULL, &tr->v4l2_dev);
	if (err)
<<<<<<< HEAD
		goto err_v4l2_dev;

	tr->video_dev.v4l2_dev = &tr->v4l2_dev;

=======
		goto err;

	tr->video_dev.v4l2_dev = &tr->v4l2_dev;

	tr->sd_tuner = v4l2_i2c_new_subdev_board(&tr->v4l2_dev,
		i2c_get_adapter(pdata->i2c_adapter), pdata->tuner, NULL);
	tr->sd_dsp = v4l2_i2c_new_subdev_board(&tr->v4l2_dev,
		i2c_get_adapter(pdata->i2c_adapter), pdata->dsp, NULL);
	if (tr->sd_tuner == NULL || tr->sd_dsp == NULL)
		goto err_video_req;

	tr->v4l2_dev.ctrl_handler = tr->sd_dsp->ctrl_handler;

>>>>>>> refs/remotes/origin/master
	err = video_register_device(&tr->video_dev, VFL_TYPE_RADIO, -1);
	if (err) {
		dev_err(&pdev->dev, "Error reg video\n");
		goto err_video_req;
	}

	video_set_drvdata(&tr->video_dev, tr);

	platform_set_drvdata(pdev, tr);
	return 0;

err_video_req:
<<<<<<< HEAD
	video_device_release_empty(&tr->video_dev);
	v4l2_device_unregister(&tr->v4l2_dev);
err_v4l2_dev:
	kfree(tr);
=======
	v4l2_device_unregister(&tr->v4l2_dev);
>>>>>>> refs/remotes/origin/master
err:
	dev_err(&pdev->dev, "Failed to register: %d\n", err);

	return err;
}

<<<<<<< HEAD
static int __devexit timbradio_remove(struct platform_device *pdev)
=======
static int timbradio_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct timbradio *tr = platform_get_drvdata(pdev);

	video_unregister_device(&tr->video_dev);
<<<<<<< HEAD
	video_device_release_empty(&tr->video_dev);

	v4l2_device_unregister(&tr->v4l2_dev);

	kfree(tr);

=======
	v4l2_device_unregister(&tr->v4l2_dev);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct platform_driver timbradio_platform_driver = {
	.driver = {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= timbradio_probe,
	.remove		= timbradio_remove,
};

<<<<<<< HEAD
<<<<<<< HEAD
/*--------------------------------------------------------------------------*/

static int __init timbradio_init(void)
{
	return platform_driver_register(&timbradio_platform_driver);
}

static void __exit timbradio_exit(void)
{
	platform_driver_unregister(&timbradio_platform_driver);
}

module_init(timbradio_init);
module_exit(timbradio_exit);
=======
module_platform_driver(timbradio_platform_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_platform_driver(timbradio_platform_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("Timberdale Radio driver");
MODULE_AUTHOR("Mocean Laboratories <info@mocean-labs.com>");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
<<<<<<< HEAD
=======
MODULE_VERSION("0.0.2");
>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_VERSION("0.0.2");
>>>>>>> refs/remotes/origin/master
MODULE_ALIAS("platform:"DRIVER_NAME);
