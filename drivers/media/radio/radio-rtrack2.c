<<<<<<< HEAD
/* RadioTrack II driver for Linux radio support (C) 1998 Ben Pfaff
=======
/*
 * RadioTrack II driver
 * Copyright 1998 Ben Pfaff
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Based on RadioTrack I/RadioReveal (C) 1997 M. Kirkwood
 * Converted to new API by Alan Cox <alan@lxorguk.ukuu.org.uk>
 * Various bugfixes and enhancements by Russell Kroll <rkroll@exploits.org>
 *
<<<<<<< HEAD
 * TODO: Allow for more than one of these foolish entities :-)
 *
=======
 * Converted to the radio-isa framework by Hans Verkuil <hans.verkuil@cisco.com>
>>>>>>> refs/remotes/origin/cm-10.0
 * Converted to V4L2 API by Mauro Carvalho Chehab <mchehab@infradead.org>
 */

#include <linux/module.h>	/* Modules 			*/
#include <linux/init.h>		/* Initdata			*/
#include <linux/ioport.h>	/* request_region		*/
#include <linux/delay.h>	/* udelay			*/
#include <linux/videodev2.h>	/* kernel radio structs		*/
#include <linux/mutex.h>
<<<<<<< HEAD
#include <linux/version.h>      /* for KERNEL_VERSION MACRO     */
#include <linux/io.h>		/* outb, outb_p			*/
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
=======
#include <linux/io.h>		/* outb, outb_p			*/
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include "radio-isa.h"
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_AUTHOR("Ben Pfaff");
MODULE_DESCRIPTION("A driver for the RadioTrack II radio card.");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_VERSION("0.1.99");
>>>>>>> refs/remotes/origin/cm-10.0

#ifndef CONFIG_RADIO_RTRACK2_PORT
#define CONFIG_RADIO_RTRACK2_PORT -1
#endif

<<<<<<< HEAD
static int io = CONFIG_RADIO_RTRACK2_PORT;
static int radio_nr = -1;

module_param(io, int, 0);
MODULE_PARM_DESC(io, "I/O address of the RadioTrack card (0x20c or 0x30c)");
module_param(radio_nr, int, 0);

#define RADIO_VERSION KERNEL_VERSION(0, 0, 2)

struct rtrack2
{
	struct v4l2_device v4l2_dev;
	struct video_device vdev;
	int io;
	unsigned long curfreq;
	int muted;
	struct mutex lock;
};

static struct rtrack2 rtrack2_card;


/* local things */

static void rt_mute(struct rtrack2 *dev)
{
	if (dev->muted)
		return;
	mutex_lock(&dev->lock);
	outb(1, dev->io);
	mutex_unlock(&dev->lock);
	dev->muted = 1;
}

static void rt_unmute(struct rtrack2 *dev)
{
	if(dev->muted == 0)
		return;
	mutex_lock(&dev->lock);
	outb(0, dev->io);
	mutex_unlock(&dev->lock);
	dev->muted = 0;
}

static void zero(struct rtrack2 *dev)
{
	outb_p(1, dev->io);
	outb_p(3, dev->io);
	outb_p(1, dev->io);
}

static void one(struct rtrack2 *dev)
{
	outb_p(5, dev->io);
	outb_p(7, dev->io);
	outb_p(5, dev->io);
}

static int rt_setfreq(struct rtrack2 *dev, unsigned long freq)
{
	int i;

	mutex_lock(&dev->lock);
	dev->curfreq = freq;
	freq = freq / 200 + 856;

	outb_p(0xc8, dev->io);
	outb_p(0xc9, dev->io);
	outb_p(0xc9, dev->io);

	for (i = 0; i < 10; i++)
		zero(dev);

	for (i = 14; i >= 0; i--)
		if (freq & (1 << i))
			one(dev);
		else
			zero(dev);

	outb_p(0xc8, dev->io);
	if (!dev->muted)
		outb_p(0, dev->io);

	mutex_unlock(&dev->lock);
	return 0;
}

static int vidioc_querycap(struct file *file, void *priv,
				struct v4l2_capability *v)
{
	strlcpy(v->driver, "radio-rtrack2", sizeof(v->driver));
	strlcpy(v->card, "RadioTrack II", sizeof(v->card));
	strlcpy(v->bus_info, "ISA", sizeof(v->bus_info));
	v->version = RADIO_VERSION;
	v->capabilities = V4L2_CAP_TUNER | V4L2_CAP_RADIO;
	return 0;
}

static int vidioc_s_tuner(struct file *file, void *priv,
				struct v4l2_tuner *v)
{
	return v->index ? -EINVAL : 0;
}

static int rt_getsigstr(struct rtrack2 *dev)
{
	int sig = 1;

	mutex_lock(&dev->lock);
	if (inb(dev->io) & 2)	/* bit set = no signal present	*/
		sig = 0;
	mutex_unlock(&dev->lock);
	return sig;
}

static int vidioc_g_tuner(struct file *file, void *priv,
				struct v4l2_tuner *v)
{
	struct rtrack2 *rt = video_drvdata(file);

	if (v->index > 0)
		return -EINVAL;

	strlcpy(v->name, "FM", sizeof(v->name));
	v->type = V4L2_TUNER_RADIO;
	v->rangelow = 88 * 16000;
	v->rangehigh = 108 * 16000;
	v->rxsubchans = V4L2_TUNER_SUB_MONO;
	v->capability = V4L2_TUNER_CAP_LOW;
	v->audmode = V4L2_TUNER_MODE_MONO;
	v->signal = 0xFFFF * rt_getsigstr(rt);
	return 0;
}

static int vidioc_s_frequency(struct file *file, void *priv,
				struct v4l2_frequency *f)
{
	struct rtrack2 *rt = video_drvdata(file);

	if (f->tuner != 0 || f->type != V4L2_TUNER_RADIO)
		return -EINVAL;
	rt_setfreq(rt, f->frequency);
	return 0;
}

static int vidioc_g_frequency(struct file *file, void *priv,
				struct v4l2_frequency *f)
{
	struct rtrack2 *rt = video_drvdata(file);

	if (f->tuner != 0)
		return -EINVAL;
	f->type = V4L2_TUNER_RADIO;
	f->frequency = rt->curfreq;
	return 0;
}

static int vidioc_queryctrl(struct file *file, void *priv,
				struct v4l2_queryctrl *qc)
{
	switch (qc->id) {
	case V4L2_CID_AUDIO_MUTE:
		return v4l2_ctrl_query_fill(qc, 0, 1, 1, 1);
	case V4L2_CID_AUDIO_VOLUME:
		return v4l2_ctrl_query_fill(qc, 0, 65535, 65535, 65535);
	}
	return -EINVAL;
}

static int vidioc_g_ctrl(struct file *file, void *priv,
				struct v4l2_control *ctrl)
{
	struct rtrack2 *rt = video_drvdata(file);

	switch (ctrl->id) {
	case V4L2_CID_AUDIO_MUTE:
		ctrl->value = rt->muted;
		return 0;
	case V4L2_CID_AUDIO_VOLUME:
		if (rt->muted)
			ctrl->value = 0;
		else
			ctrl->value = 65535;
		return 0;
	}
	return -EINVAL;
}

static int vidioc_s_ctrl(struct file *file, void *priv,
				struct v4l2_control *ctrl)
{
	struct rtrack2 *rt = video_drvdata(file);

	switch (ctrl->id) {
	case V4L2_CID_AUDIO_MUTE:
		if (ctrl->value)
			rt_mute(rt);
		else
			rt_unmute(rt);
		return 0;
	case V4L2_CID_AUDIO_VOLUME:
		if (ctrl->value)
			rt_unmute(rt);
		else
			rt_mute(rt);
		return 0;
	}
	return -EINVAL;
}

static int vidioc_g_input(struct file *filp, void *priv, unsigned int *i)
{
	*i = 0;
	return 0;
}

static int vidioc_s_input(struct file *filp, void *priv, unsigned int i)
{
	return i ? -EINVAL : 0;
}

static int vidioc_g_audio(struct file *file, void *priv,
				struct v4l2_audio *a)
{
	a->index = 0;
	strlcpy(a->name, "Radio", sizeof(a->name));
	a->capability = V4L2_AUDCAP_STEREO;
	return 0;
}

static int vidioc_s_audio(struct file *file, void *priv,
				struct v4l2_audio *a)
{
	return a->index ? -EINVAL : 0;
}

static const struct v4l2_file_operations rtrack2_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= video_ioctl2,
};

static const struct v4l2_ioctl_ops rtrack2_ioctl_ops = {
	.vidioc_querycap    = vidioc_querycap,
	.vidioc_g_tuner     = vidioc_g_tuner,
	.vidioc_s_tuner     = vidioc_s_tuner,
	.vidioc_g_frequency = vidioc_g_frequency,
	.vidioc_s_frequency = vidioc_s_frequency,
	.vidioc_queryctrl   = vidioc_queryctrl,
	.vidioc_g_ctrl      = vidioc_g_ctrl,
	.vidioc_s_ctrl      = vidioc_s_ctrl,
	.vidioc_g_audio     = vidioc_g_audio,
	.vidioc_s_audio     = vidioc_s_audio,
	.vidioc_g_input     = vidioc_g_input,
	.vidioc_s_input     = vidioc_s_input,
=======
#define RTRACK2_MAX 2

static int io[RTRACK2_MAX] = { [0] = CONFIG_RADIO_RTRACK2_PORT,
			      [1 ... (RTRACK2_MAX - 1)] = -1 };
static int radio_nr[RTRACK2_MAX] = { [0 ... (RTRACK2_MAX - 1)] = -1 };

module_param_array(io, int, NULL, 0444);
MODULE_PARM_DESC(io, "I/O addresses of the RadioTrack card (0x20f or 0x30f)");
module_param_array(radio_nr, int, NULL, 0444);
MODULE_PARM_DESC(radio_nr, "Radio device numbers");

static struct radio_isa_card *rtrack2_alloc(void)
{
	return kzalloc(sizeof(struct radio_isa_card), GFP_KERNEL);
}

static void zero(struct radio_isa_card *isa)
{
	outb_p(1, isa->io);
	outb_p(3, isa->io);
	outb_p(1, isa->io);
}

static void one(struct radio_isa_card *isa)
{
	outb_p(5, isa->io);
	outb_p(7, isa->io);
	outb_p(5, isa->io);
}

static int rtrack2_s_frequency(struct radio_isa_card *isa, u32 freq)
{
	int i;

	freq = freq / 200 + 856;

	outb_p(0xc8, isa->io);
	outb_p(0xc9, isa->io);
	outb_p(0xc9, isa->io);

	for (i = 0; i < 10; i++)
		zero(isa);

	for (i = 14; i >= 0; i--)
		if (freq & (1 << i))
			one(isa);
		else
			zero(isa);

	outb_p(0xc8, isa->io);
	if (!v4l2_ctrl_g_ctrl(isa->mute))
		outb_p(0, isa->io);
	return 0;
}

static u32 rtrack2_g_signal(struct radio_isa_card *isa)
{
	/* bit set = no signal present	*/
	return (inb(isa->io) & 2) ? 0 : 0xffff;
}

static int rtrack2_s_mute_volume(struct radio_isa_card *isa, bool mute, int vol)
{
	outb(mute, isa->io);
	return 0;
}

static const struct radio_isa_ops rtrack2_ops = {
	.alloc = rtrack2_alloc,
	.s_mute_volume = rtrack2_s_mute_volume,
	.s_frequency = rtrack2_s_frequency,
	.g_signal = rtrack2_g_signal,
};

static const int rtrack2_ioports[] = { 0x20f, 0x30f };

static struct radio_isa_driver rtrack2_driver = {
	.driver = {
		.match		= radio_isa_match,
		.probe		= radio_isa_probe,
		.remove		= radio_isa_remove,
		.driver		= {
			.name	= "radio-rtrack2",
		},
	},
	.io_params = io,
	.radio_nr_params = radio_nr,
	.io_ports = rtrack2_ioports,
	.num_of_io_ports = ARRAY_SIZE(rtrack2_ioports),
	.region_size = 4,
	.card = "AIMSlab RadioTrack II",
	.ops = &rtrack2_ops,
	.has_stereo = true,
>>>>>>> refs/remotes/origin/cm-10.0
};

static int __init rtrack2_init(void)
{
<<<<<<< HEAD
	struct rtrack2 *dev = &rtrack2_card;
	struct v4l2_device *v4l2_dev = &dev->v4l2_dev;
	int res;

	strlcpy(v4l2_dev->name, "rtrack2", sizeof(v4l2_dev->name));
	dev->io = io;
	if (dev->io == -1) {
		v4l2_err(v4l2_dev, "You must set an I/O address with io=0x20c or io=0x30c\n");
		return -EINVAL;
	}
	if (!request_region(dev->io, 4, "rtrack2")) {
		v4l2_err(v4l2_dev, "port 0x%x already in use\n", dev->io);
		return -EBUSY;
	}

	res = v4l2_device_register(NULL, v4l2_dev);
	if (res < 0) {
		release_region(dev->io, 4);
		v4l2_err(v4l2_dev, "Could not register v4l2_device\n");
		return res;
	}

	strlcpy(dev->vdev.name, v4l2_dev->name, sizeof(dev->vdev.name));
	dev->vdev.v4l2_dev = v4l2_dev;
	dev->vdev.fops = &rtrack2_fops;
	dev->vdev.ioctl_ops = &rtrack2_ioctl_ops;
	dev->vdev.release = video_device_release_empty;
	video_set_drvdata(&dev->vdev, dev);

	/* mute card - prevents noisy bootups */
	outb(1, dev->io);
	dev->muted = 1;

	mutex_init(&dev->lock);
	if (video_register_device(&dev->vdev, VFL_TYPE_RADIO, radio_nr) < 0) {
		v4l2_device_unregister(v4l2_dev);
		release_region(dev->io, 4);
		return -EINVAL;
	}

	v4l2_info(v4l2_dev, "AIMSlab Radiotrack II card driver.\n");

	return 0;
=======
	return isa_register_driver(&rtrack2_driver.driver, RTRACK2_MAX);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void __exit rtrack2_exit(void)
{
<<<<<<< HEAD
	struct rtrack2 *dev = &rtrack2_card;

	video_unregister_device(&dev->vdev);
	v4l2_device_unregister(&dev->v4l2_dev);
	release_region(dev->io, 4);
=======
	isa_unregister_driver(&rtrack2_driver.driver);
>>>>>>> refs/remotes/origin/cm-10.0
}

module_init(rtrack2_init);
module_exit(rtrack2_exit);
