/* Typhoon Radio Card driver for radio support
 * (c) 1999 Dr. Henrik Seidel <Henrik.Seidel@gmx.de>
 *
 * Notes on the hardware
 *
 * This card has two output sockets, one for speakers and one for line.
 * The speaker output has volume control, but only in four discrete
 * steps. The line output has neither volume control nor mute.
 *
 * The card has auto-stereo according to its manual, although it all
 * sounds mono to me (even with the Win/DOS drivers). Maybe it's my
 * antenna - I really don't know for sure.
 *
 * Frequency control is done digitally.
 *
 * Volume control is done digitally, but there are only four different
 * possible values. So you should better always turn the volume up and
 * use line control. I got the best results by connecting line output
 * to the sound card microphone input. For such a configuration the
 * volume control has no effect, since volume control only influences
 * the speaker output.
 *
 * There is no explicit mute/unmute. So I set the radio frequency to a
 * value where I do expect just noise and turn the speaker volume down.
 * The frequency change is necessary since the card never seems to be
 * completely silent.
 *
 * Converted to V4L2 API by Mauro Carvalho Chehab <mchehab@infradead.org>
 */

#include <linux/module.h>	/* Modules                        */
#include <linux/init.h>		/* Initdata                       */
#include <linux/ioport.h>	/* request_region		  */
<<<<<<< HEAD
#include <linux/version.h>      /* for KERNEL_VERSION MACRO     */
#include <linux/videodev2.h>	/* kernel radio structs           */
#include <linux/io.h>		/* outb, outb_p                   */
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
=======
#include <linux/videodev2.h>	/* kernel radio structs           */
#include <linux/io.h>		/* outb, outb_p                   */
#include <linux/slab.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include "radio-isa.h"

#define DRIVER_VERSION "0.1.2"
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_AUTHOR("Dr. Henrik Seidel");
MODULE_DESCRIPTION("A driver for the Typhoon radio card (a.k.a. EcoRadio).");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_VERSION("0.1.99");
>>>>>>> refs/remotes/origin/cm-10.0

#ifndef CONFIG_RADIO_TYPHOON_PORT
#define CONFIG_RADIO_TYPHOON_PORT -1
#endif

#ifndef CONFIG_RADIO_TYPHOON_MUTEFREQ
<<<<<<< HEAD
#define CONFIG_RADIO_TYPHOON_MUTEFREQ 0
#endif

static int io = CONFIG_RADIO_TYPHOON_PORT;
static int radio_nr = -1;

module_param(io, int, 0);
MODULE_PARM_DESC(io, "I/O address of the Typhoon card (0x316 or 0x336)");

module_param(radio_nr, int, 0);

static unsigned long mutefreq = CONFIG_RADIO_TYPHOON_MUTEFREQ;
module_param(mutefreq, ulong, 0);
MODULE_PARM_DESC(mutefreq, "Frequency used when muting the card (in kHz)");

#define RADIO_VERSION KERNEL_VERSION(0, 1, 1)

#define BANNER "Typhoon Radio Card driver v0.1.1\n"

struct typhoon {
	struct v4l2_device v4l2_dev;
	struct video_device vdev;
	int io;
	int curvol;
	int muted;
	unsigned long curfreq;
	unsigned long mutefreq;
	struct mutex lock;
};

static struct typhoon typhoon_card;

static void typhoon_setvol_generic(struct typhoon *dev, int vol)
{
	mutex_lock(&dev->lock);
	vol >>= 14;				/* Map 16 bit to 2 bit */
	vol &= 3;
	outb_p(vol / 2, dev->io);		/* Set the volume, high bit. */
	outb_p(vol % 2, dev->io + 2);	/* Set the volume, low bit. */
	mutex_unlock(&dev->lock);
}

static int typhoon_setfreq_generic(struct typhoon *dev,
				   unsigned long frequency)
=======
#define CONFIG_RADIO_TYPHOON_MUTEFREQ 87000
#endif

#define TYPHOON_MAX 2

static int io[TYPHOON_MAX] = { [0] = CONFIG_RADIO_TYPHOON_PORT,
			      [1 ... (TYPHOON_MAX - 1)] = -1 };
static int radio_nr[TYPHOON_MAX]	= { [0 ... (TYPHOON_MAX - 1)] = -1 };
static unsigned long mutefreq = CONFIG_RADIO_TYPHOON_MUTEFREQ;

module_param_array(io, int, NULL, 0444);
MODULE_PARM_DESC(io, "I/O addresses of the Typhoon card (0x316 or 0x336)");
module_param_array(radio_nr, int, NULL, 0444);
MODULE_PARM_DESC(radio_nr, "Radio device numbers");
module_param(mutefreq, ulong, 0);
MODULE_PARM_DESC(mutefreq, "Frequency used when muting the card (in kHz)");

struct typhoon {
	struct radio_isa_card isa;
	int muted;
};

static struct radio_isa_card *typhoon_alloc(void)
{
	struct typhoon *ty = kzalloc(sizeof(*ty), GFP_KERNEL);

	return ty ? &ty->isa : NULL;
}

static int typhoon_s_frequency(struct radio_isa_card *isa, u32 freq)
>>>>>>> refs/remotes/origin/cm-10.0
{
	unsigned long outval;
	unsigned long x;

	/*
	 * The frequency transfer curve is not linear. The best fit I could
	 * get is
	 *
	 * outval = -155 + exp((f + 15.55) * 0.057))
	 *
	 * where frequency f is in MHz. Since we don't have exp in the kernel,
	 * I approximate this function by a third order polynomial.
	 *
	 */

<<<<<<< HEAD
	mutex_lock(&dev->lock);
	x = frequency / 160;
=======
	x = freq / 160;
>>>>>>> refs/remotes/origin/cm-10.0
	outval = (x * x + 2500) / 5000;
	outval = (outval * x + 5000) / 10000;
	outval -= (10 * x * x + 10433) / 20866;
	outval += 4 * x - 11505;

<<<<<<< HEAD
	outb_p((outval >> 8) & 0x01, dev->io + 4);
	outb_p(outval >> 9, dev->io + 6);
	outb_p(outval & 0xff, dev->io + 8);
	mutex_unlock(&dev->lock);

	return 0;
}

static int typhoon_setfreq(struct typhoon *dev, unsigned long frequency)
{
	typhoon_setfreq_generic(dev, frequency);
	dev->curfreq = frequency;
	return 0;
}

static void typhoon_mute(struct typhoon *dev)
{
	if (dev->muted == 1)
		return;
	typhoon_setvol_generic(dev, 0);
	typhoon_setfreq_generic(dev, dev->mutefreq);
	dev->muted = 1;
}

static void typhoon_unmute(struct typhoon *dev)
{
	if (dev->muted == 0)
		return;
	typhoon_setfreq_generic(dev, dev->curfreq);
	typhoon_setvol_generic(dev, dev->curvol);
	dev->muted = 0;
}

static int typhoon_setvol(struct typhoon *dev, int vol)
{
	if (dev->muted && vol != 0) {	/* user is unmuting the card */
		dev->curvol = vol;
		typhoon_unmute(dev);
		return 0;
	}
	if (vol == dev->curvol)		/* requested volume == current */
		return 0;

	if (vol == 0) {			/* volume == 0 means mute the card */
		typhoon_mute(dev);
		dev->curvol = vol;
		return 0;
	}
	typhoon_setvol_generic(dev, vol);
	dev->curvol = vol;
	return 0;
}

static int vidioc_querycap(struct file *file, void  *priv,
					struct v4l2_capability *v)
{
	strlcpy(v->driver, "radio-typhoon", sizeof(v->driver));
	strlcpy(v->card, "Typhoon Radio", sizeof(v->card));
	strlcpy(v->bus_info, "ISA", sizeof(v->bus_info));
	v->version = RADIO_VERSION;
	v->capabilities = V4L2_CAP_TUNER | V4L2_CAP_RADIO;
	return 0;
}

static int vidioc_g_tuner(struct file *file, void *priv,
					struct v4l2_tuner *v)
{
	if (v->index > 0)
		return -EINVAL;

	strlcpy(v->name, "FM", sizeof(v->name));
	v->type = V4L2_TUNER_RADIO;
	v->rangelow = 87.5 * 16000;
	v->rangehigh = 108 * 16000;
	v->rxsubchans = V4L2_TUNER_SUB_MONO;
	v->capability = V4L2_TUNER_CAP_LOW;
	v->audmode = V4L2_TUNER_MODE_MONO;
	v->signal = 0xFFFF;     /* We can't get the signal strength */
	return 0;
}

static int vidioc_s_tuner(struct file *file, void *priv,
					struct v4l2_tuner *v)
{
	return v->index ? -EINVAL : 0;
}

static int vidioc_g_frequency(struct file *file, void *priv,
					struct v4l2_frequency *f)
{
	struct typhoon *dev = video_drvdata(file);

	if (f->tuner != 0)
		return -EINVAL;
	f->type = V4L2_TUNER_RADIO;
	f->frequency = dev->curfreq;
	return 0;
}

static int vidioc_s_frequency(struct file *file, void *priv,
					struct v4l2_frequency *f)
{
	struct typhoon *dev = video_drvdata(file);

	if (f->tuner != 0 || f->type != V4L2_TUNER_RADIO)
		return -EINVAL;
	dev->curfreq = f->frequency;
	typhoon_setfreq(dev, dev->curfreq);
	return 0;
}

static int vidioc_queryctrl(struct file *file, void *priv,
					struct v4l2_queryctrl *qc)
{
	switch (qc->id) {
	case V4L2_CID_AUDIO_MUTE:
		return v4l2_ctrl_query_fill(qc, 0, 1, 1, 1);
	case V4L2_CID_AUDIO_VOLUME:
		return v4l2_ctrl_query_fill(qc, 0, 65535, 16384, 65535);
	}
	return -EINVAL;
}

static int vidioc_g_ctrl(struct file *file, void *priv,
					struct v4l2_control *ctrl)
{
	struct typhoon *dev = video_drvdata(file);

	switch (ctrl->id) {
	case V4L2_CID_AUDIO_MUTE:
		ctrl->value = dev->muted;
		return 0;
	case V4L2_CID_AUDIO_VOLUME:
		ctrl->value = dev->curvol;
		return 0;
	}
	return -EINVAL;
}

static int vidioc_s_ctrl (struct file *file, void *priv,
					struct v4l2_control *ctrl)
{
	struct typhoon *dev = video_drvdata(file);

	switch (ctrl->id) {
	case V4L2_CID_AUDIO_MUTE:
		if (ctrl->value)
			typhoon_mute(dev);
		else
			typhoon_unmute(dev);
		return 0;
	case V4L2_CID_AUDIO_VOLUME:
		typhoon_setvol(dev, ctrl->value);
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

static int vidioc_log_status(struct file *file, void *priv)
{
	struct typhoon *dev = video_drvdata(file);
	struct v4l2_device *v4l2_dev = &dev->v4l2_dev;

	v4l2_info(v4l2_dev, BANNER);
#ifdef MODULE
	v4l2_info(v4l2_dev, "Load type: Driver loaded as a module\n\n");
#else
	v4l2_info(v4l2_dev, "Load type: Driver compiled into kernel\n\n");
#endif
	v4l2_info(v4l2_dev, "frequency = %lu kHz\n", dev->curfreq >> 4);
	v4l2_info(v4l2_dev, "volume = %d\n", dev->curvol);
	v4l2_info(v4l2_dev, "mute = %s\n", dev->muted ?  "on" : "off");
	v4l2_info(v4l2_dev, "io = 0x%x\n", dev->io);
	v4l2_info(v4l2_dev, "mute frequency = %lu kHz\n", dev->mutefreq >> 4);
	return 0;
}

static const struct v4l2_file_operations typhoon_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= video_ioctl2,
};

static const struct v4l2_ioctl_ops typhoon_ioctl_ops = {
	.vidioc_log_status  = vidioc_log_status,
	.vidioc_querycap    = vidioc_querycap,
	.vidioc_g_tuner     = vidioc_g_tuner,
	.vidioc_s_tuner     = vidioc_s_tuner,
	.vidioc_g_audio     = vidioc_g_audio,
	.vidioc_s_audio     = vidioc_s_audio,
	.vidioc_g_input     = vidioc_g_input,
	.vidioc_s_input     = vidioc_s_input,
	.vidioc_g_frequency = vidioc_g_frequency,
	.vidioc_s_frequency = vidioc_s_frequency,
	.vidioc_queryctrl   = vidioc_queryctrl,
	.vidioc_g_ctrl      = vidioc_g_ctrl,
	.vidioc_s_ctrl      = vidioc_s_ctrl,
=======
	outb_p((outval >> 8) & 0x01, isa->io + 4);
	outb_p(outval >> 9, isa->io + 6);
	outb_p(outval & 0xff, isa->io + 8);
	return 0;
}

static int typhoon_s_mute_volume(struct radio_isa_card *isa, bool mute, int vol)
{
	struct typhoon *ty = container_of(isa, struct typhoon, isa);

	if (mute)
		vol = 0;
	vol >>= 14;			/* Map 16 bit to 2 bit */
	vol &= 3;
	outb_p(vol / 2, isa->io);	/* Set the volume, high bit. */
	outb_p(vol % 2, isa->io + 2);	/* Set the volume, low bit. */

	if (vol == 0 && !ty->muted) {
		ty->muted = true;
		return typhoon_s_frequency(isa, mutefreq << 4);
	}
	if (vol && ty->muted) {
		ty->muted = false;
		return typhoon_s_frequency(isa, isa->freq);
	}
	return 0;
}

static const struct radio_isa_ops typhoon_ops = {
	.alloc = typhoon_alloc,
	.s_mute_volume = typhoon_s_mute_volume,
	.s_frequency = typhoon_s_frequency,
};

static const int typhoon_ioports[] = { 0x316, 0x336 };

static struct radio_isa_driver typhoon_driver = {
	.driver = {
		.match		= radio_isa_match,
		.probe		= radio_isa_probe,
		.remove		= radio_isa_remove,
		.driver		= {
			.name	= "radio-typhoon",
		},
	},
	.io_params = io,
	.radio_nr_params = radio_nr,
	.io_ports = typhoon_ioports,
	.num_of_io_ports = ARRAY_SIZE(typhoon_ioports),
	.region_size = 8,
	.card = "Typhoon Radio",
	.ops = &typhoon_ops,
	.has_stereo = true,
	.max_volume = 3,
>>>>>>> refs/remotes/origin/cm-10.0
};

static int __init typhoon_init(void)
{
<<<<<<< HEAD
	struct typhoon *dev = &typhoon_card;
	struct v4l2_device *v4l2_dev = &dev->v4l2_dev;
	int res;

	strlcpy(v4l2_dev->name, "typhoon", sizeof(v4l2_dev->name));
	dev->io = io;

	if (dev->io == -1) {
		v4l2_err(v4l2_dev, "You must set an I/O address with io=0x316 or io=0x336\n");
		return -EINVAL;
	}

	if (mutefreq < 87000 || mutefreq > 108500) {
		v4l2_err(v4l2_dev, "You must set a frequency (in kHz) used when muting the card,\n");
		v4l2_err(v4l2_dev, "e.g. with \"mutefreq=87500\" (87000 <= mutefreq <= 108500)\n");
		return -EINVAL;
	}
	dev->curfreq = dev->mutefreq = mutefreq << 4;

	mutex_init(&dev->lock);
	if (!request_region(dev->io, 8, "typhoon")) {
		v4l2_err(v4l2_dev, "port 0x%x already in use\n",
		       dev->io);
		return -EBUSY;
	}

	res = v4l2_device_register(NULL, v4l2_dev);
	if (res < 0) {
		release_region(dev->io, 8);
		v4l2_err(v4l2_dev, "Could not register v4l2_device\n");
		return res;
	}
	v4l2_info(v4l2_dev, BANNER);

	strlcpy(dev->vdev.name, v4l2_dev->name, sizeof(dev->vdev.name));
	dev->vdev.v4l2_dev = v4l2_dev;
	dev->vdev.fops = &typhoon_fops;
	dev->vdev.ioctl_ops = &typhoon_ioctl_ops;
	dev->vdev.release = video_device_release_empty;
	video_set_drvdata(&dev->vdev, dev);

	/* mute card - prevents noisy bootups */
	typhoon_mute(dev);

	if (video_register_device(&dev->vdev, VFL_TYPE_RADIO, radio_nr) < 0) {
		v4l2_device_unregister(&dev->v4l2_dev);
		release_region(dev->io, 8);
		return -EINVAL;
	}
	v4l2_info(v4l2_dev, "port 0x%x.\n", dev->io);
	v4l2_info(v4l2_dev, "mute frequency is %lu kHz.\n", mutefreq);

	return 0;
=======
	if (mutefreq < 87000 || mutefreq > 108000) {
		printk(KERN_ERR "%s: You must set a frequency (in kHz) used when muting the card,\n",
				typhoon_driver.driver.driver.name);
		printk(KERN_ERR "%s: e.g. with \"mutefreq=87500\" (87000 <= mutefreq <= 108000)\n",
				typhoon_driver.driver.driver.name);
		return -ENODEV;
	}
	return isa_register_driver(&typhoon_driver.driver, TYPHOON_MAX);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void __exit typhoon_exit(void)
{
<<<<<<< HEAD
	struct typhoon *dev = &typhoon_card;

	video_unregister_device(&dev->vdev);
	v4l2_device_unregister(&dev->v4l2_dev);
	release_region(dev->io, 8);
}

=======
	isa_unregister_driver(&typhoon_driver.driver);
}


>>>>>>> refs/remotes/origin/cm-10.0
module_init(typhoon_init);
module_exit(typhoon_exit);

