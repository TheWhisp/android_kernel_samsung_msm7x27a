<<<<<<< HEAD
<<<<<<< HEAD
/* radiotrack (radioreveal) driver for Linux radio support
 * (c) 1997 M. Kirkwood
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * AimsLab RadioTrack (aka RadioVeveal) driver
 *
 * Copyright 1997 M. Kirkwood
 *
 * Converted to the radio-isa framework by Hans Verkuil <hans.verkuil@cisco.com>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * Converted to V4L2 API by Mauro Carvalho Chehab <mchehab@infradead.org>
 * Converted to new API by Alan Cox <alan@lxorguk.ukuu.org.uk>
 * Various bugfixes and enhancements by Russell Kroll <rkroll@exploits.org>
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * History:
 * 1999-02-24	Russell Kroll <rkroll@exploits.org>
 * 		Fine tuning/VIDEO_TUNER_LOW
 *		Frequency range expanded to start at 87 MHz
 *
 * TODO: Allow for more than one of these foolish entities :-)
 *
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * Notes on the hardware (reverse engineered from other peoples'
 * reverse engineering of AIMS' code :-)
 *
 *  Frequency control is done digitally -- ie out(port,encodefreq(95.8));
 *
 *  The signal strength query is unsurprisingly inaccurate.  And it seems
 *  to indicate that (on my card, at least) the frequency setting isn't
 *  too great.  (I have to tune up .025MHz from what the freq should be
 *  to get a report that the thing is tuned.)
 *
 *  Volume control is (ugh) analogue:
 *   out(port, start_increasing_volume);
 *   wait(a_wee_while);
 *   out(port, stop_changing_the_volume);
 *
<<<<<<< HEAD
<<<<<<< HEAD
=======
 * Fully tested with the Keene USB FM Transmitter and the v4l2-compliance tool.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Fully tested with the Keene USB FM Transmitter and the v4l2-compliance tool.
>>>>>>> refs/remotes/origin/master
 */

#include <linux/module.h>	/* Modules 			*/
#include <linux/init.h>		/* Initdata			*/
#include <linux/ioport.h>	/* request_region		*/
#include <linux/delay.h>	/* msleep			*/
#include <linux/videodev2.h>	/* kernel radio structs		*/
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/version.h>	/* for KERNEL_VERSION MACRO	*/
#include <linux/io.h>		/* outb, outb_p			*/
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>

MODULE_AUTHOR("M.Kirkwood");
MODULE_DESCRIPTION("A driver for the RadioTrack/RadioReveal radio card.");
MODULE_LICENSE("GPL");
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/io.h>		/* outb, outb_p			*/
#include <linux/slab.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-ctrls.h>
#include "radio-isa.h"
<<<<<<< HEAD
=======
#include "lm7000.h"
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("M. Kirkwood");
MODULE_DESCRIPTION("A driver for the RadioTrack/RadioReveal radio card.");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0.0");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#ifndef CONFIG_RADIO_RTRACK_PORT
#define CONFIG_RADIO_RTRACK_PORT -1
#endif

<<<<<<< HEAD
<<<<<<< HEAD
static int io = CONFIG_RADIO_RTRACK_PORT;
static int radio_nr = -1;

module_param(io, int, 0);
MODULE_PARM_DESC(io, "I/O address of the RadioTrack card (0x20f or 0x30f)");
module_param(radio_nr, int, 0);

#define RADIO_VERSION KERNEL_VERSION(0, 0, 2)

struct rtrack
{
	struct v4l2_device v4l2_dev;
	struct video_device vdev;
	int port;
	int curvol;
	unsigned long curfreq;
	int muted;
	int io;
	struct mutex lock;
};

static struct rtrack rtrack_card;

/* local things */

static void rt_decvol(struct rtrack *rt)
{
	outb(0x58, rt->io);		/* volume down + sigstr + on	*/
	msleep(100);
	outb(0xd8, rt->io);		/* volume steady + sigstr + on	*/
}

static void rt_incvol(struct rtrack *rt)
{
	outb(0x98, rt->io);		/* volume up + sigstr + on	*/
	msleep(100);
	outb(0xd8, rt->io);		/* volume steady + sigstr + on	*/
}

static void rt_mute(struct rtrack *rt)
{
	rt->muted = 1;
	mutex_lock(&rt->lock);
	outb(0xd0, rt->io);		/* volume steady, off		*/
	mutex_unlock(&rt->lock);
}

static int rt_setvol(struct rtrack *rt, int vol)
{
	int i;

	mutex_lock(&rt->lock);

	if (vol == rt->curvol) {	/* requested volume = current */
		if (rt->muted) {	/* user is unmuting the card  */
			rt->muted = 0;
			outb(0xd8, rt->io);	/* enable card */
		}
		mutex_unlock(&rt->lock);
		return 0;
	}

	if (vol == 0) {			/* volume = 0 means mute the card */
		outb(0x48, rt->io);	/* volume down but still "on"	*/
		msleep(2000);	/* make sure it's totally down	*/
		outb(0xd0, rt->io);	/* volume steady, off		*/
		rt->curvol = 0;		/* track the volume state!	*/
		mutex_unlock(&rt->lock);
		return 0;
	}

	rt->muted = 0;
	if (vol > rt->curvol)
		for (i = rt->curvol; i < vol; i++)
			rt_incvol(rt);
	else
		for (i = rt->curvol; i > vol; i--)
			rt_decvol(rt);

	rt->curvol = vol;
	mutex_unlock(&rt->lock);
	return 0;
}

/* the 128+64 on these outb's is to keep the volume stable while tuning
 * without them, the volume _will_ creep up with each frequency change
 * and bit 4 (+16) is to keep the signal strength meter enabled
 */

static void send_0_byte(struct rtrack *rt)
{
	if (rt->curvol == 0 || rt->muted) {
		outb_p(128+64+16+  1, rt->io);   /* wr-enable + data low */
		outb_p(128+64+16+2+1, rt->io);   /* clock */
	}
	else {
		outb_p(128+64+16+8+  1, rt->io);  /* on + wr-enable + data low */
		outb_p(128+64+16+8+2+1, rt->io);  /* clock */
	}
	msleep(1);
}

static void send_1_byte(struct rtrack *rt)
{
	if (rt->curvol == 0 || rt->muted) {
		outb_p(128+64+16+4  +1, rt->io);   /* wr-enable+data high */
		outb_p(128+64+16+4+2+1, rt->io);   /* clock */
	}
	else {
		outb_p(128+64+16+8+4  +1, rt->io); /* on+wr-enable+data high */
		outb_p(128+64+16+8+4+2+1, rt->io); /* clock */
	}

	msleep(1);
}

static int rt_setfreq(struct rtrack *rt, unsigned long freq)
{
	int i;

	mutex_lock(&rt->lock);			/* Stop other ops interfering */

	rt->curfreq = freq;

	/* now uses VIDEO_TUNER_LOW for fine tuning */

	freq += 171200;			/* Add 10.7 MHz IF 		*/
	freq /= 800;			/* Convert to 50 kHz units	*/

	send_0_byte(rt);		/*  0: LSB of frequency		*/

	for (i = 0; i < 13; i++)	/*   : frequency bits (1-13)	*/
		if (freq & (1 << i))
			send_1_byte(rt);
		else
			send_0_byte(rt);

	send_0_byte(rt);		/* 14: test bit - always 0    */
	send_0_byte(rt);		/* 15: test bit - always 0    */

	send_0_byte(rt);		/* 16: band data 0 - always 0 */
	send_0_byte(rt);		/* 17: band data 1 - always 0 */
	send_0_byte(rt);		/* 18: band data 2 - always 0 */
	send_0_byte(rt);		/* 19: time base - always 0   */

	send_0_byte(rt);		/* 20: spacing (0 = 25 kHz)   */
	send_1_byte(rt);		/* 21: spacing (1 = 25 kHz)   */
	send_0_byte(rt);		/* 22: spacing (0 = 25 kHz)   */
	send_1_byte(rt);		/* 23: AM/FM (FM = 1, always) */

	if (rt->curvol == 0 || rt->muted)
		outb(0xd0, rt->io);	/* volume steady + sigstr */
	else
		outb(0xd8, rt->io);	/* volume steady + sigstr + on */

	mutex_unlock(&rt->lock);

	return 0;
}

static int rt_getsigstr(struct rtrack *rt)
{
	int sig = 1;

	mutex_lock(&rt->lock);
	if (inb(rt->io) & 2)	/* bit set = no signal present	*/
		sig = 0;
	mutex_unlock(&rt->lock);
	return sig;
}

static int vidioc_querycap(struct file *file, void  *priv,
					struct v4l2_capability *v)
{
	strlcpy(v->driver, "radio-aimslab", sizeof(v->driver));
	strlcpy(v->card, "RadioTrack", sizeof(v->card));
	strlcpy(v->bus_info, "ISA", sizeof(v->bus_info));
	v->version = RADIO_VERSION;
	v->capabilities = V4L2_CAP_TUNER | V4L2_CAP_RADIO;
	return 0;
}

static int vidioc_g_tuner(struct file *file, void *priv,
					struct v4l2_tuner *v)
{
	struct rtrack *rt = video_drvdata(file);

	if (v->index > 0)
		return -EINVAL;

	strlcpy(v->name, "FM", sizeof(v->name));
	v->type = V4L2_TUNER_RADIO;
	v->rangelow = 87 * 16000;
	v->rangehigh = 108 * 16000;
	v->rxsubchans = V4L2_TUNER_SUB_MONO;
	v->capability = V4L2_TUNER_CAP_LOW;
	v->audmode = V4L2_TUNER_MODE_MONO;
	v->signal = 0xffff * rt_getsigstr(rt);
	return 0;
}

static int vidioc_s_tuner(struct file *file, void *priv,
					struct v4l2_tuner *v)
{
	return v->index ? -EINVAL : 0;
}

static int vidioc_s_frequency(struct file *file, void *priv,
					struct v4l2_frequency *f)
{
	struct rtrack *rt = video_drvdata(file);

	if (f->tuner != 0 || f->type != V4L2_TUNER_RADIO)
		return -EINVAL;
	rt_setfreq(rt, f->frequency);
	return 0;
}

static int vidioc_g_frequency(struct file *file, void *priv,
					struct v4l2_frequency *f)
{
	struct rtrack *rt = video_drvdata(file);

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
		return v4l2_ctrl_query_fill(qc, 0, 0xff, 1, 0xff);
	}
	return -EINVAL;
}

static int vidioc_g_ctrl(struct file *file, void *priv,
					struct v4l2_control *ctrl)
{
	struct rtrack *rt = video_drvdata(file);

	switch (ctrl->id) {
	case V4L2_CID_AUDIO_MUTE:
		ctrl->value = rt->muted;
		return 0;
	case V4L2_CID_AUDIO_VOLUME:
		ctrl->value = rt->curvol;
		return 0;
	}
	return -EINVAL;
}

static int vidioc_s_ctrl(struct file *file, void *priv,
					struct v4l2_control *ctrl)
{
	struct rtrack *rt = video_drvdata(file);

	switch (ctrl->id) {
	case V4L2_CID_AUDIO_MUTE:
		if (ctrl->value)
			rt_mute(rt);
		else
			rt_setvol(rt, rt->curvol);
		return 0;
	case V4L2_CID_AUDIO_VOLUME:
		rt_setvol(rt, ctrl->value);
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

static const struct v4l2_file_operations rtrack_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= video_ioctl2,
};

static const struct v4l2_ioctl_ops rtrack_ioctl_ops = {
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
=======
>>>>>>> refs/remotes/origin/master
#define RTRACK_MAX 2

static int io[RTRACK_MAX] = { [0] = CONFIG_RADIO_RTRACK_PORT,
			      [1 ... (RTRACK_MAX - 1)] = -1 };
static int radio_nr[RTRACK_MAX]	= { [0 ... (RTRACK_MAX - 1)] = -1 };

module_param_array(io, int, NULL, 0444);
MODULE_PARM_DESC(io, "I/O addresses of the RadioTrack card (0x20f or 0x30f)");
module_param_array(radio_nr, int, NULL, 0444);
MODULE_PARM_DESC(radio_nr, "Radio device numbers");

struct rtrack {
	struct radio_isa_card isa;
	int curvol;
};

static struct radio_isa_card *rtrack_alloc(void)
{
	struct rtrack *rt = kzalloc(sizeof(struct rtrack), GFP_KERNEL);

	if (rt)
		rt->curvol = 0xff;
	return rt ? &rt->isa : NULL;
}

<<<<<<< HEAD
/* The 128+64 on these outb's is to keep the volume stable while tuning.
 * Without them, the volume _will_ creep up with each frequency change
 * and bit 4 (+16) is to keep the signal strength meter enabled.
 */

static void send_0_byte(struct radio_isa_card *isa, int on)
{
	outb_p(128+64+16+on+1, isa->io);	/* wr-enable + data low */
	outb_p(128+64+16+on+2+1, isa->io);	/* clock */
	msleep(1);
}

static void send_1_byte(struct radio_isa_card *isa, int on)
{
	outb_p(128+64+16+on+4+1, isa->io);	/* wr-enable+data high */
	outb_p(128+64+16+on+4+2+1, isa->io);	/* clock */
	msleep(1);
=======
#define AIMS_BIT_TUN_CE		(1 << 0)
#define AIMS_BIT_TUN_CLK	(1 << 1)
#define AIMS_BIT_TUN_DATA	(1 << 2)
#define AIMS_BIT_VOL_CE		(1 << 3)
#define AIMS_BIT_TUN_STRQ	(1 << 4)
/* bit 5 is not connected */
#define AIMS_BIT_VOL_UP		(1 << 6)	/* active low */
#define AIMS_BIT_VOL_DN		(1 << 7)	/* active low */

static void rtrack_set_pins(void *handle, u8 pins)
{
	struct radio_isa_card *isa = handle;
	struct rtrack *rt = container_of(isa, struct rtrack, isa);
	u8 bits = AIMS_BIT_VOL_DN | AIMS_BIT_VOL_UP | AIMS_BIT_TUN_STRQ;

	if (!v4l2_ctrl_g_ctrl(rt->isa.mute))
		bits |= AIMS_BIT_VOL_CE;

	if (pins & LM7000_DATA)
		bits |= AIMS_BIT_TUN_DATA;
	if (pins & LM7000_CLK)
		bits |= AIMS_BIT_TUN_CLK;
	if (pins & LM7000_CE)
		bits |= AIMS_BIT_TUN_CE;

	outb_p(bits, rt->isa.io);
>>>>>>> refs/remotes/origin/master
}

static int rtrack_s_frequency(struct radio_isa_card *isa, u32 freq)
{
<<<<<<< HEAD
	int on = v4l2_ctrl_g_ctrl(isa->mute) ? 0 : 8;
	int i;

	freq += 171200;			/* Add 10.7 MHz IF 		*/
	freq /= 800;			/* Convert to 50 kHz units	*/

	send_0_byte(isa, on);		/*  0: LSB of frequency		*/

	for (i = 0; i < 13; i++)	/*   : frequency bits (1-13)	*/
		if (freq & (1 << i))
			send_1_byte(isa, on);
		else
			send_0_byte(isa, on);

	send_0_byte(isa, on);		/* 14: test bit - always 0    */
	send_0_byte(isa, on);		/* 15: test bit - always 0    */

	send_0_byte(isa, on);		/* 16: band data 0 - always 0 */
	send_0_byte(isa, on);		/* 17: band data 1 - always 0 */
	send_0_byte(isa, on);		/* 18: band data 2 - always 0 */
	send_0_byte(isa, on);		/* 19: time base - always 0   */

	send_0_byte(isa, on);		/* 20: spacing (0 = 25 kHz)   */
	send_1_byte(isa, on);		/* 21: spacing (1 = 25 kHz)   */
	send_0_byte(isa, on);		/* 22: spacing (0 = 25 kHz)   */
	send_1_byte(isa, on);		/* 23: AM/FM (FM = 1, always) */

	outb(0xd0 + on, isa->io);	/* volume steady + sigstr */
=======
	lm7000_set_freq(freq, isa, rtrack_set_pins);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static u32 rtrack_g_signal(struct radio_isa_card *isa)
{
	/* bit set = no signal present */
	return 0xffff * !(inb(isa->io) & 2);
}

static int rtrack_s_mute_volume(struct radio_isa_card *isa, bool mute, int vol)
{
	struct rtrack *rt = container_of(isa, struct rtrack, isa);
	int curvol = rt->curvol;

	if (mute) {
		outb(0xd0, isa->io);	/* volume steady + sigstr + off	*/
		return 0;
	}
	if (vol == 0) {			/* volume = 0 means mute the card */
		outb(0x48, isa->io);	/* volume down but still "on"	*/
		msleep(curvol * 3);	/* make sure it's totally down	*/
	} else if (curvol < vol) {
		outb(0x98, isa->io);	/* volume up + sigstr + on	*/
		for (; curvol < vol; curvol++)
			udelay(3000);
	} else if (curvol > vol) {
		outb(0x58, isa->io);	/* volume down + sigstr + on	*/
		for (; curvol > vol; curvol--)
			udelay(3000);
	}
	outb(0xd8, isa->io);		/* volume steady + sigstr + on	*/
	rt->curvol = vol;
	return 0;
}

/* Mute card - prevents noisy bootups */
static int rtrack_initialize(struct radio_isa_card *isa)
{
	/* this ensures that the volume is all the way up  */
	outb(0x90, isa->io);	/* volume up but still "on"	*/
	msleep(3000);		/* make sure it's totally up	*/
	outb(0xc0, isa->io);	/* steady volume, mute card	*/
	return 0;
}

static const struct radio_isa_ops rtrack_ops = {
	.alloc = rtrack_alloc,
	.init = rtrack_initialize,
	.s_mute_volume = rtrack_s_mute_volume,
	.s_frequency = rtrack_s_frequency,
	.g_signal = rtrack_g_signal,
};

static const int rtrack_ioports[] = { 0x20f, 0x30f };

static struct radio_isa_driver rtrack_driver = {
	.driver = {
		.match		= radio_isa_match,
		.probe		= radio_isa_probe,
		.remove		= radio_isa_remove,
		.driver		= {
			.name	= "radio-aimslab",
		},
	},
	.io_params = io,
	.radio_nr_params = radio_nr,
	.io_ports = rtrack_ioports,
	.num_of_io_ports = ARRAY_SIZE(rtrack_ioports),
	.region_size = 2,
	.card = "AIMSlab RadioTrack/RadioReveal",
	.ops = &rtrack_ops,
	.has_stereo = true,
	.max_volume = 0xff,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

static int __init rtrack_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct rtrack *rt = &rtrack_card;
	struct v4l2_device *v4l2_dev = &rt->v4l2_dev;
	int res;

	strlcpy(v4l2_dev->name, "rtrack", sizeof(v4l2_dev->name));
	rt->io = io;

	if (rt->io == -1) {
		v4l2_err(v4l2_dev, "you must set an I/O address with io=0x20f or 0x30f\n");
		return -EINVAL;
	}

	if (!request_region(rt->io, 2, "rtrack")) {
		v4l2_err(v4l2_dev, "port 0x%x already in use\n", rt->io);
		return -EBUSY;
	}

	res = v4l2_device_register(NULL, v4l2_dev);
	if (res < 0) {
		release_region(rt->io, 2);
		v4l2_err(v4l2_dev, "could not register v4l2_device\n");
		return res;
	}

	strlcpy(rt->vdev.name, v4l2_dev->name, sizeof(rt->vdev.name));
	rt->vdev.v4l2_dev = v4l2_dev;
	rt->vdev.fops = &rtrack_fops;
	rt->vdev.ioctl_ops = &rtrack_ioctl_ops;
	rt->vdev.release = video_device_release_empty;
	video_set_drvdata(&rt->vdev, rt);

	/* Set up the I/O locking */

	mutex_init(&rt->lock);

	/* mute card - prevents noisy bootups */

	/* this ensures that the volume is all the way down  */
	outb(0x48, rt->io);		/* volume down but still "on"	*/
	msleep(2000);	/* make sure it's totally down	*/
	outb(0xc0, rt->io);		/* steady volume, mute card	*/

	if (video_register_device(&rt->vdev, VFL_TYPE_RADIO, radio_nr) < 0) {
		v4l2_device_unregister(&rt->v4l2_dev);
		release_region(rt->io, 2);
		return -EINVAL;
	}
	v4l2_info(v4l2_dev, "AIMSlab RadioTrack/RadioReveal card driver.\n");

	return 0;
=======
	return isa_register_driver(&rtrack_driver.driver, RTRACK_MAX);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return isa_register_driver(&rtrack_driver.driver, RTRACK_MAX);
>>>>>>> refs/remotes/origin/master
}

static void __exit rtrack_exit(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct rtrack *rt = &rtrack_card;

	video_unregister_device(&rt->vdev);
	v4l2_device_unregister(&rt->v4l2_dev);
	release_region(rt->io, 2);
=======
	isa_unregister_driver(&rtrack_driver.driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	isa_unregister_driver(&rtrack_driver.driver);
>>>>>>> refs/remotes/origin/master
}

module_init(rtrack_init);
module_exit(rtrack_exit);
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
