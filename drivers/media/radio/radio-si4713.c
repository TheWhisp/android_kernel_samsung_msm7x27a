/*
 * drivers/media/radio/radio-si4713.c
 *
 * Platform Driver for Silicon Labs Si4713 FM Radio Transmitter:
 *
 * Copyright (c) 2008 Instituto Nokia de Tecnologia - INdT
 * Contact: Eduardo Valentin <eduardo.valentin@nokia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/videodev2.h>
#include <linux/slab.h>
#include <media/v4l2-device.h>
#include <media/v4l2-common.h>
#include <media/v4l2-ioctl.h>
<<<<<<< HEAD
=======
#include <media/v4l2-fh.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-event.h>
>>>>>>> refs/remotes/origin/master
#include <media/radio-si4713.h>

/* module parameters */
static int radio_nr = -1;	/* radio device minor (-1 ==> auto assign) */
module_param(radio_nr, int, 0);
MODULE_PARM_DESC(radio_nr,
		 "Minor number for radio device (-1 ==> auto assign)");

<<<<<<< HEAD
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eduardo Valentin <eduardo.valentin@nokia.com>");
MODULE_DESCRIPTION("Platform driver for Si4713 FM Radio Transmitter");
MODULE_VERSION("0.0.1");
=======
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Eduardo Valentin <eduardo.valentin@nokia.com>");
MODULE_DESCRIPTION("Platform driver for Si4713 FM Radio Transmitter");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("platform:radio-si4713");
>>>>>>> refs/remotes/origin/master

/* Driver state struct */
struct radio_si4713_device {
	struct v4l2_device		v4l2_dev;
<<<<<<< HEAD
	struct video_device		*radio_dev;
=======
	struct video_device		radio_dev;
	struct mutex lock;
>>>>>>> refs/remotes/origin/master
};

/* radio_si4713_fops - file operations interface */
static const struct v4l2_file_operations radio_si4713_fops = {
	.owner		= THIS_MODULE,
<<<<<<< HEAD
=======
	.open = v4l2_fh_open,
	.release = v4l2_fh_release,
	.poll = v4l2_ctrl_poll,
>>>>>>> refs/remotes/origin/master
	/* Note: locking is done at the subdev level in the i2c driver. */
	.unlocked_ioctl	= video_ioctl2,
};

/* Video4Linux Interface */
<<<<<<< HEAD
static int radio_si4713_fill_audout(struct v4l2_audioout *vao)
{
	/* TODO: check presence of audio output */
	strlcpy(vao->name, "FM Modulator Audio Out", 32);

	return 0;
}

static int radio_si4713_enumaudout(struct file *file, void *priv,
						struct v4l2_audioout *vao)
{
	return radio_si4713_fill_audout(vao);
}

static int radio_si4713_g_audout(struct file *file, void *priv,
					struct v4l2_audioout *vao)
{
	int rval = radio_si4713_fill_audout(vao);

	vao->index = 0;

	return rval;
}

static int radio_si4713_s_audout(struct file *file, void *priv,
					struct v4l2_audioout *vao)
{
	return vao->index ? -EINVAL : 0;
}
=======
>>>>>>> refs/remotes/origin/master

/* radio_si4713_querycap - query device capabilities */
static int radio_si4713_querycap(struct file *file, void *priv,
					struct v4l2_capability *capability)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct radio_si4713_device *rsdev;

	rsdev = video_get_drvdata(video_devdata(file));

=======
>>>>>>> refs/remotes/origin/cm-10.0
	strlcpy(capability->driver, "radio-si4713", sizeof(capability->driver));
	strlcpy(capability->card, "Silicon Labs Si4713 Modulator",
				sizeof(capability->card));
	capability->capabilities = V4L2_CAP_MODULATOR | V4L2_CAP_RDS_OUTPUT;
=======
	strlcpy(capability->driver, "radio-si4713", sizeof(capability->driver));
	strlcpy(capability->card, "Silicon Labs Si4713 Modulator",
		sizeof(capability->card));
	strlcpy(capability->bus_info, "platform:radio-si4713",
		sizeof(capability->bus_info));
	capability->device_caps = V4L2_CAP_MODULATOR | V4L2_CAP_RDS_OUTPUT;
	capability->capabilities = capability->device_caps | V4L2_CAP_DEVICE_CAPS;
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
/* radio_si4713_queryctrl - enumerate control items */
static int radio_si4713_queryctrl(struct file *file, void *priv,
						struct v4l2_queryctrl *qc)
{
	/* Must be sorted from low to high control ID! */
	static const u32 user_ctrls[] = {
		V4L2_CID_USER_CLASS,
		V4L2_CID_AUDIO_MUTE,
		0
	};

	/* Must be sorted from low to high control ID! */
	static const u32 fmtx_ctrls[] = {
		V4L2_CID_FM_TX_CLASS,
		V4L2_CID_RDS_TX_DEVIATION,
		V4L2_CID_RDS_TX_PI,
		V4L2_CID_RDS_TX_PTY,
		V4L2_CID_RDS_TX_PS_NAME,
		V4L2_CID_RDS_TX_RADIO_TEXT,
		V4L2_CID_AUDIO_LIMITER_ENABLED,
		V4L2_CID_AUDIO_LIMITER_RELEASE_TIME,
		V4L2_CID_AUDIO_LIMITER_DEVIATION,
		V4L2_CID_AUDIO_COMPRESSION_ENABLED,
		V4L2_CID_AUDIO_COMPRESSION_GAIN,
		V4L2_CID_AUDIO_COMPRESSION_THRESHOLD,
		V4L2_CID_AUDIO_COMPRESSION_ATTACK_TIME,
		V4L2_CID_AUDIO_COMPRESSION_RELEASE_TIME,
		V4L2_CID_PILOT_TONE_ENABLED,
		V4L2_CID_PILOT_TONE_DEVIATION,
		V4L2_CID_PILOT_TONE_FREQUENCY,
		V4L2_CID_TUNE_PREEMPHASIS,
		V4L2_CID_TUNE_POWER_LEVEL,
		V4L2_CID_TUNE_ANTENNA_CAPACITOR,
		0
	};
	static const u32 *ctrl_classes[] = {
		user_ctrls,
		fmtx_ctrls,
		NULL
	};
	struct radio_si4713_device *rsdev;

	rsdev = video_get_drvdata(video_devdata(file));

	qc->id = v4l2_ctrl_next(ctrl_classes, qc->id);
	if (qc->id == 0)
		return -EINVAL;

	if (qc->id == V4L2_CID_USER_CLASS || qc->id == V4L2_CID_FM_TX_CLASS)
		return v4l2_ctrl_query_fill(qc, 0, 0, 0, 0);

	return v4l2_device_call_until_err(&rsdev->v4l2_dev, 0, core,
						queryctrl, qc);
}

=======
>>>>>>> refs/remotes/origin/master
/*
 * v4l2 ioctl call backs.
 * we are just a wrapper for v4l2_sub_devs.
 */
static inline struct v4l2_device *get_v4l2_dev(struct file *file)
{
	return &((struct radio_si4713_device *)video_drvdata(file))->v4l2_dev;
}

<<<<<<< HEAD
static int radio_si4713_g_ext_ctrls(struct file *file, void *p,
						struct v4l2_ext_controls *vecs)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, core,
							g_ext_ctrls, vecs);
}

static int radio_si4713_s_ext_ctrls(struct file *file, void *p,
						struct v4l2_ext_controls *vecs)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, core,
							s_ext_ctrls, vecs);
}

static int radio_si4713_g_ctrl(struct file *file, void *p,
						struct v4l2_control *vc)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, core,
							g_ctrl, vc);
}

static int radio_si4713_s_ctrl(struct file *file, void *p,
						struct v4l2_control *vc)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, core,
							s_ctrl, vc);
}

static int radio_si4713_g_modulator(struct file *file, void *p,
						struct v4l2_modulator *vm)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, tuner,
							g_modulator, vm);
}

static int radio_si4713_s_modulator(struct file *file, void *p,
						struct v4l2_modulator *vm)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, tuner,
							s_modulator, vm);
}

static int radio_si4713_g_frequency(struct file *file, void *p,
						struct v4l2_frequency *vf)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, tuner,
							g_frequency, vf);
}

static int radio_si4713_s_frequency(struct file *file, void *p,
						struct v4l2_frequency *vf)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, tuner,
							s_frequency, vf);
}

static long radio_si4713_default(struct file *file, void *p,
				bool valid_prio, int cmd, void *arg)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, core,
							ioctl, cmd, arg);
}

static struct v4l2_ioctl_ops radio_si4713_ioctl_ops = {
	.vidioc_enumaudout	= radio_si4713_enumaudout,
	.vidioc_g_audout	= radio_si4713_g_audout,
	.vidioc_s_audout	= radio_si4713_s_audout,
	.vidioc_querycap	= radio_si4713_querycap,
	.vidioc_queryctrl	= radio_si4713_queryctrl,
	.vidioc_g_ext_ctrls	= radio_si4713_g_ext_ctrls,
	.vidioc_s_ext_ctrls	= radio_si4713_s_ext_ctrls,
	.vidioc_g_ctrl		= radio_si4713_g_ctrl,
	.vidioc_s_ctrl		= radio_si4713_s_ctrl,
=======
static int radio_si4713_g_modulator(struct file *file, void *p,
				    struct v4l2_modulator *vm)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, tuner,
					  g_modulator, vm);
}

static int radio_si4713_s_modulator(struct file *file, void *p,
				    const struct v4l2_modulator *vm)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, tuner,
					  s_modulator, vm);
}

static int radio_si4713_g_frequency(struct file *file, void *p,
				    struct v4l2_frequency *vf)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, tuner,
					  g_frequency, vf);
}

static int radio_si4713_s_frequency(struct file *file, void *p,
				    const struct v4l2_frequency *vf)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, tuner,
					  s_frequency, vf);
}

static long radio_si4713_default(struct file *file, void *p,
				 bool valid_prio, unsigned int cmd, void *arg)
{
	return v4l2_device_call_until_err(get_v4l2_dev(file), 0, core,
					  ioctl, cmd, arg);
}

static struct v4l2_ioctl_ops radio_si4713_ioctl_ops = {
	.vidioc_querycap	= radio_si4713_querycap,
>>>>>>> refs/remotes/origin/master
	.vidioc_g_modulator	= radio_si4713_g_modulator,
	.vidioc_s_modulator	= radio_si4713_s_modulator,
	.vidioc_g_frequency	= radio_si4713_g_frequency,
	.vidioc_s_frequency	= radio_si4713_s_frequency,
<<<<<<< HEAD
=======
	.vidioc_log_status      = v4l2_ctrl_log_status,
	.vidioc_subscribe_event = v4l2_ctrl_subscribe_event,
	.vidioc_unsubscribe_event = v4l2_event_unsubscribe,
>>>>>>> refs/remotes/origin/master
	.vidioc_default		= radio_si4713_default,
};

/* radio_si4713_vdev_template - video device interface */
static struct video_device radio_si4713_vdev_template = {
	.fops			= &radio_si4713_fops,
	.name			= "radio-si4713",
<<<<<<< HEAD
	.release		= video_device_release,
	.ioctl_ops		= &radio_si4713_ioctl_ops,
=======
	.release		= video_device_release_empty,
	.ioctl_ops		= &radio_si4713_ioctl_ops,
	.vfl_dir		= VFL_DIR_TX,
>>>>>>> refs/remotes/origin/master
};

/* Platform driver interface */
/* radio_si4713_pdriver_probe - probe for the device */
static int radio_si4713_pdriver_probe(struct platform_device *pdev)
{
	struct radio_si4713_platform_data *pdata = pdev->dev.platform_data;
	struct radio_si4713_device *rsdev;
	struct i2c_adapter *adapter;
	struct v4l2_subdev *sd;
	int rval = 0;

	if (!pdata) {
		dev_err(&pdev->dev, "Cannot proceed without platform data.\n");
		rval = -EINVAL;
		goto exit;
	}

<<<<<<< HEAD
	rsdev = kzalloc(sizeof *rsdev, GFP_KERNEL);
=======
	rsdev = devm_kzalloc(&pdev->dev, sizeof(*rsdev), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!rsdev) {
		dev_err(&pdev->dev, "Failed to alloc video device.\n");
		rval = -ENOMEM;
		goto exit;
	}
<<<<<<< HEAD
=======
	mutex_init(&rsdev->lock);
>>>>>>> refs/remotes/origin/master

	rval = v4l2_device_register(&pdev->dev, &rsdev->v4l2_dev);
	if (rval) {
		dev_err(&pdev->dev, "Failed to register v4l2 device.\n");
<<<<<<< HEAD
		goto free_rsdev;
=======
		goto exit;
>>>>>>> refs/remotes/origin/master
	}

	adapter = i2c_get_adapter(pdata->i2c_bus);
	if (!adapter) {
		dev_err(&pdev->dev, "Cannot get i2c adapter %d\n",
<<<<<<< HEAD
							pdata->i2c_bus);
=======
			pdata->i2c_bus);
>>>>>>> refs/remotes/origin/master
		rval = -ENODEV;
		goto unregister_v4l2_dev;
	}

	sd = v4l2_i2c_new_subdev_board(&rsdev->v4l2_dev, adapter,
<<<<<<< HEAD
					pdata->subdev_board_info, NULL);
=======
				       pdata->subdev_board_info, NULL);
>>>>>>> refs/remotes/origin/master
	if (!sd) {
		dev_err(&pdev->dev, "Cannot get v4l2 subdevice\n");
		rval = -ENODEV;
		goto put_adapter;
	}

<<<<<<< HEAD
	rsdev->radio_dev = video_device_alloc();
	if (!rsdev->radio_dev) {
		dev_err(&pdev->dev, "Failed to alloc video device.\n");
		rval = -ENOMEM;
		goto put_adapter;
	}

	memcpy(rsdev->radio_dev, &radio_si4713_vdev_template,
			sizeof(radio_si4713_vdev_template));
	video_set_drvdata(rsdev->radio_dev, rsdev);
	if (video_register_device(rsdev->radio_dev, VFL_TYPE_RADIO, radio_nr)) {
		dev_err(&pdev->dev, "Could not register video device.\n");
		rval = -EIO;
		goto free_vdev;
=======
	rsdev->radio_dev = radio_si4713_vdev_template;
	rsdev->radio_dev.v4l2_dev = &rsdev->v4l2_dev;
	rsdev->radio_dev.ctrl_handler = sd->ctrl_handler;
	set_bit(V4L2_FL_USE_FH_PRIO, &rsdev->radio_dev.flags);
	/* Serialize all access to the si4713 */
	rsdev->radio_dev.lock = &rsdev->lock;
	video_set_drvdata(&rsdev->radio_dev, rsdev);
	if (video_register_device(&rsdev->radio_dev, VFL_TYPE_RADIO, radio_nr)) {
		dev_err(&pdev->dev, "Could not register video device.\n");
		rval = -EIO;
		goto put_adapter;
>>>>>>> refs/remotes/origin/master
	}
	dev_info(&pdev->dev, "New device successfully probed\n");

	goto exit;

<<<<<<< HEAD
free_vdev:
	video_device_release(rsdev->radio_dev);
=======
>>>>>>> refs/remotes/origin/master
put_adapter:
	i2c_put_adapter(adapter);
unregister_v4l2_dev:
	v4l2_device_unregister(&rsdev->v4l2_dev);
<<<<<<< HEAD
free_rsdev:
	kfree(rsdev);
=======
>>>>>>> refs/remotes/origin/master
exit:
	return rval;
}

/* radio_si4713_pdriver_remove - remove the device */
<<<<<<< HEAD
static int __exit radio_si4713_pdriver_remove(struct platform_device *pdev)
{
	struct v4l2_device *v4l2_dev = platform_get_drvdata(pdev);
	struct radio_si4713_device *rsdev = container_of(v4l2_dev,
						struct radio_si4713_device,
						v4l2_dev);
	struct v4l2_subdev *sd = list_entry(v4l2_dev->subdevs.next,
					    struct v4l2_subdev, list);
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	video_unregister_device(rsdev->radio_dev);
	i2c_put_adapter(client->adapter);
	v4l2_device_unregister(&rsdev->v4l2_dev);
	kfree(rsdev);
=======
static int radio_si4713_pdriver_remove(struct platform_device *pdev)
{
	struct v4l2_device *v4l2_dev = platform_get_drvdata(pdev);
	struct v4l2_subdev *sd = list_entry(v4l2_dev->subdevs.next,
					    struct v4l2_subdev, list);
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct radio_si4713_device *rsdev;

	rsdev = container_of(v4l2_dev, struct radio_si4713_device, v4l2_dev);
	video_unregister_device(&rsdev->radio_dev);
	i2c_put_adapter(client->adapter);
	v4l2_device_unregister(&rsdev->v4l2_dev);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static struct platform_driver radio_si4713_pdriver = {
	.driver		= {
		.name	= "radio-si4713",
<<<<<<< HEAD
	},
	.probe		= radio_si4713_pdriver_probe,
	.remove         = __exit_p(radio_si4713_pdriver_remove),
};

<<<<<<< HEAD
/* Module Interface */
static int __init radio_si4713_module_init(void)
{
	return platform_driver_register(&radio_si4713_pdriver);
}

static void __exit radio_si4713_module_exit(void)
{
	platform_driver_unregister(&radio_si4713_pdriver);
}

module_init(radio_si4713_module_init);
module_exit(radio_si4713_module_exit);

=======
module_platform_driver(radio_si4713_pdriver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.owner	= THIS_MODULE,
	},
	.probe		= radio_si4713_pdriver_probe,
	.remove         = radio_si4713_pdriver_remove,
};

module_platform_driver(radio_si4713_pdriver);
>>>>>>> refs/remotes/origin/master
