/* radio-cadet.c - A video4linux driver for the ADS Cadet AM/FM Radio Card
 *
 * by Fred Gleason <fredg@wava.com>
 * Version 0.3.3
 *
 * (Loosely) based on code for the Aztech radio card by
 *
 * Russell Kroll    (rkroll@exploits.org)
 * Quay Ly
 * Donald Song
 * Jason Lewis      (jlewis@twilight.vtc.vsc.edu)
 * Scott McGrath    (smcgrath@twilight.vtc.vsc.edu)
 * William McGrath  (wmcgrath@twilight.vtc.vsc.edu)
 *
 * History:
 * 2000-04-29	Russell Kroll <rkroll@exploits.org>
 *		Added ISAPnP detection for Linux 2.3/2.4
 *
 * 2001-01-10	Russell Kroll <rkroll@exploits.org>
 *		Removed dead CONFIG_RADIO_CADET_PORT code
 *		PnP detection on load is now default (no args necessary)
 *
 * 2002-01-17	Adam Belay <ambx1@neo.rr.com>
 *		Updated to latest pnp code
 *
 * 2003-01-31	Alan Cox <alan@lxorguk.ukuu.org.uk>
 *		Cleaned up locking, delay code, general odds and ends
 *
 * 2006-07-30	Hans J. Koch <koch@hjk-az.de>
 *		Changed API to V4L2
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/version.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/module.h>	/* Modules 			*/
#include <linux/init.h>		/* Initdata			*/
#include <linux/ioport.h>	/* request_region		*/
#include <linux/delay.h>	/* udelay			*/
#include <linux/videodev2.h>	/* V4L2 API defs		*/
#include <linux/param.h>
#include <linux/pnp.h>
#include <linux/sched.h>
#include <linux/io.h>		/* outb, outb_p			*/
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
<<<<<<< HEAD
=======
#include <media/v4l2-ctrls.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-event.h>
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Fred Gleason, Russell Kroll, Quay Lu, Donald Song, Jason Lewis, Scott McGrath, William McGrath");
MODULE_DESCRIPTION("A driver for the ADS Cadet AM/FM/RDS radio card.");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
<<<<<<< HEAD
=======
MODULE_VERSION("0.3.4");
>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_VERSION("0.3.4");
>>>>>>> refs/remotes/origin/master

static int io = -1;		/* default to isapnp activation */
static int radio_nr = -1;

module_param(io, int, 0);
MODULE_PARM_DESC(io, "I/O address of Cadet card (0x330,0x332,0x334,0x336,0x338,0x33a,0x33c,0x33e)");
module_param(radio_nr, int, 0);

<<<<<<< HEAD
<<<<<<< HEAD
#define CADET_VERSION KERNEL_VERSION(0, 3, 3)

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define RDS_BUFFER 256
#define RDS_RX_FLAG 1
#define MBS_RX_FLAG 2

struct cadet {
	struct v4l2_device v4l2_dev;
	struct video_device vdev;
<<<<<<< HEAD
	int io;
	int users;
	int curtuner;
=======
	struct v4l2_ctrl_handler ctrl_handler;
	int io;
	bool is_fm_band;
	u32 curfreq;
>>>>>>> refs/remotes/origin/master
	int tunestat;
	int sigstrength;
	wait_queue_head_t read_queue;
	struct timer_list readtimer;
<<<<<<< HEAD
	__u8 rdsin, rdsout, rdsstat;
=======
	u8 rdsin, rdsout, rdsstat;
>>>>>>> refs/remotes/origin/master
	unsigned char rdsbuf[RDS_BUFFER];
	struct mutex lock;
	int reading;
};

static struct cadet cadet_card;

/*
 * Signal Strength Threshold Values
 * The V4L API spec does not define any particular unit for the signal
 * strength value.  These values are in microvolts of RF at the tuner's input.
 */
<<<<<<< HEAD
static __u16 sigtable[2][4] = {
	{  5, 10, 30,  150 },
	{ 28, 40, 63, 1000 }
=======
static u16 sigtable[2][4] = {
	{ 1835, 2621,  4128, 65535 },
	{ 2185, 4369, 13107, 65535 },
};

static const struct v4l2_frequency_band bands[] = {
	{
		.index = 0,
		.type = V4L2_TUNER_RADIO,
		.capability = V4L2_TUNER_CAP_LOW | V4L2_TUNER_CAP_FREQ_BANDS,
		.rangelow = 8320,      /* 520 kHz */
		.rangehigh = 26400,    /* 1650 kHz */
		.modulation = V4L2_BAND_MODULATION_AM,
	}, {
		.index = 1,
		.type = V4L2_TUNER_RADIO,
		.capability = V4L2_TUNER_CAP_STEREO | V4L2_TUNER_CAP_RDS |
			V4L2_TUNER_CAP_RDS_BLOCK_IO | V4L2_TUNER_CAP_LOW |
			V4L2_TUNER_CAP_FREQ_BANDS,
		.rangelow = 1400000,   /* 87.5 MHz */
		.rangehigh = 1728000,  /* 108.0 MHz */
		.modulation = V4L2_BAND_MODULATION_FM,
	},
>>>>>>> refs/remotes/origin/master
};


static int cadet_getstereo(struct cadet *dev)
{
	int ret = V4L2_TUNER_SUB_MONO;

<<<<<<< HEAD
	if (dev->curtuner != 0)	/* Only FM has stereo capability! */
		return V4L2_TUNER_SUB_MONO;

	mutex_lock(&dev->lock);
	outb(7, dev->io);          /* Select tuner control */
	if ((inb(dev->io + 1) & 0x40) == 0)
		ret = V4L2_TUNER_SUB_STEREO;
	mutex_unlock(&dev->lock);
=======
	if (!dev->is_fm_band)	/* Only FM has stereo capability! */
		return V4L2_TUNER_SUB_MONO;

	outb(7, dev->io);          /* Select tuner control */
	if ((inb(dev->io + 1) & 0x40) == 0)
		ret = V4L2_TUNER_SUB_STEREO;
>>>>>>> refs/remotes/origin/master
	return ret;
}

static unsigned cadet_gettune(struct cadet *dev)
{
	int curvol, i;
	unsigned fifo = 0;

	/*
	 * Prepare for read
	 */

<<<<<<< HEAD
	mutex_lock(&dev->lock);

=======
>>>>>>> refs/remotes/origin/master
	outb(7, dev->io);       /* Select tuner control */
	curvol = inb(dev->io + 1); /* Save current volume/mute setting */
	outb(0x00, dev->io + 1);  /* Ensure WRITE-ENABLE is LOW */
	dev->tunestat = 0xffff;

	/*
	 * Read the shift register
	 */
	for (i = 0; i < 25; i++) {
		fifo = (fifo << 1) | ((inb(dev->io + 1) >> 7) & 0x01);
		if (i < 24) {
			outb(0x01, dev->io + 1);
			dev->tunestat &= inb(dev->io + 1);
			outb(0x00, dev->io + 1);
		}
	}

	/*
	 * Restore volume/mute setting
	 */
	outb(curvol, dev->io + 1);
<<<<<<< HEAD
	mutex_unlock(&dev->lock);

=======
>>>>>>> refs/remotes/origin/master
	return fifo;
}

static unsigned cadet_getfreq(struct cadet *dev)
{
	int i;
	unsigned freq = 0, test, fifo = 0;

	/*
	 * Read current tuning
	 */
	fifo = cadet_gettune(dev);

	/*
	 * Convert to actual frequency
	 */
<<<<<<< HEAD
	if (dev->curtuner == 0) {    /* FM */
		test = 12500;
		for (i = 0; i < 14; i++) {
			if ((fifo & 0x01) != 0)
				freq += test;
			test = test << 1;
			fifo = fifo >> 1;
		}
		freq -= 10700000;           /* IF frequency is 10.7 MHz */
		freq = (freq * 16) / 1000000;   /* Make it 1/16 MHz */
	}
	if (dev->curtuner == 1)    /* AM */
		freq = ((fifo & 0x7fff) - 2010) * 16;

=======
	if (!dev->is_fm_band)    /* AM */
		return ((fifo & 0x7fff) - 450) * 16;

	test = 12500;
	for (i = 0; i < 14; i++) {
		if ((fifo & 0x01) != 0)
			freq += test;
		test = test << 1;
		fifo = fifo >> 1;
	}
	freq -= 10700000;           /* IF frequency is 10.7 MHz */
	freq = (freq * 16) / 1000;   /* Make it 1/16 kHz */
>>>>>>> refs/remotes/origin/master
	return freq;
}

static void cadet_settune(struct cadet *dev, unsigned fifo)
{
	int i;
	unsigned test;

<<<<<<< HEAD
	mutex_lock(&dev->lock);

=======
>>>>>>> refs/remotes/origin/master
	outb(7, dev->io);                /* Select tuner control */
	/*
	 * Write the shift register
	 */
	test = 0;
	test = (fifo >> 23) & 0x02;      /* Align data for SDO */
	test |= 0x1c;                /* SDM=1, SWE=1, SEN=1, SCK=0 */
	outb(7, dev->io);                /* Select tuner control */
	outb(test, dev->io + 1);           /* Initialize for write */
	for (i = 0; i < 25; i++) {
		test |= 0x01;              /* Toggle SCK High */
		outb(test, dev->io + 1);
		test &= 0xfe;              /* Toggle SCK Low */
		outb(test, dev->io + 1);
		fifo = fifo << 1;            /* Prepare the next bit */
		test = 0x1c | ((fifo >> 23) & 0x02);
		outb(test, dev->io + 1);
	}
<<<<<<< HEAD
	mutex_unlock(&dev->lock);
=======
>>>>>>> refs/remotes/origin/master
}

static void cadet_setfreq(struct cadet *dev, unsigned freq)
{
	unsigned fifo;
	int i, j, test;
	int curvol;

<<<<<<< HEAD
=======
	freq = clamp(freq, bands[dev->is_fm_band].rangelow,
			   bands[dev->is_fm_band].rangehigh);
	dev->curfreq = freq;
>>>>>>> refs/remotes/origin/master
	/*
	 * Formulate a fifo command
	 */
	fifo = 0;
<<<<<<< HEAD
	if (dev->curtuner == 0) {    /* FM */
		test = 102400;
		freq = (freq * 1000) / 16;       /* Make it kHz */
=======
	if (dev->is_fm_band) {    /* FM */
		test = 102400;
		freq = freq / 16;       /* Make it kHz */
>>>>>>> refs/remotes/origin/master
		freq += 10700;               /* IF is 10700 kHz */
		for (i = 0; i < 14; i++) {
			fifo = fifo << 1;
			if (freq >= test) {
				fifo |= 0x01;
				freq -= test;
			}
			test = test >> 1;
		}
<<<<<<< HEAD
	}
	if (dev->curtuner == 1) {    /* AM */
		fifo = (freq / 16) + 2010;            /* Make it kHz */
		fifo |= 0x100000;            /* Select AM Band */
=======
	} else {	/* AM */
		fifo = (freq / 16) + 450;	/* Make it kHz */
		fifo |= 0x100000;		/* Select AM Band */
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Save current volume/mute setting
	 */

<<<<<<< HEAD
	mutex_lock(&dev->lock);
	outb(7, dev->io);                /* Select tuner control */
	curvol = inb(dev->io + 1);
	mutex_unlock(&dev->lock);
=======
	outb(7, dev->io);                /* Select tuner control */
	curvol = inb(dev->io + 1);
>>>>>>> refs/remotes/origin/master

	/*
	 * Tune the card
	 */
	for (j = 3; j > -1; j--) {
		cadet_settune(dev, fifo | (j << 16));

<<<<<<< HEAD
		mutex_lock(&dev->lock);
		outb(7, dev->io);         /* Select tuner control */
		outb(curvol, dev->io + 1);
		mutex_unlock(&dev->lock);
=======
		outb(7, dev->io);         /* Select tuner control */
		outb(curvol, dev->io + 1);
>>>>>>> refs/remotes/origin/master

		msleep(100);

		cadet_gettune(dev);
		if ((dev->tunestat & 0x40) == 0) {   /* Tuned */
<<<<<<< HEAD
			dev->sigstrength = sigtable[dev->curtuner][j];
			return;
		}
	}
	dev->sigstrength = 0;
}


static int cadet_getvol(struct cadet *dev)
{
	int ret = 0;

	mutex_lock(&dev->lock);

	outb(7, dev->io);                /* Select tuner control */
	if ((inb(dev->io + 1) & 0x20) != 0)
		ret = 0xffff;

	mutex_unlock(&dev->lock);
	return ret;
}


static void cadet_setvol(struct cadet *dev, int vol)
{
	mutex_lock(&dev->lock);
	outb(7, dev->io);                /* Select tuner control */
	if (vol > 0)
		outb(0x20, dev->io + 1);
	else
		outb(0x00, dev->io + 1);
	mutex_unlock(&dev->lock);
}

=======
			dev->sigstrength = sigtable[dev->is_fm_band][j];
			goto reset_rds;
		}
	}
	dev->sigstrength = 0;
reset_rds:
	outb(3, dev->io);
	outb(inb(dev->io + 1) & 0x7f, dev->io + 1);
}


>>>>>>> refs/remotes/origin/master
static void cadet_handler(unsigned long data)
{
	struct cadet *dev = (void *)data;

	/* Service the RDS fifo */
	if (mutex_trylock(&dev->lock)) {
		outb(0x3, dev->io);       /* Select RDS Decoder Control */
		if ((inb(dev->io + 1) & 0x20) != 0)
			printk(KERN_CRIT "cadet: RDS fifo overflow\n");
		outb(0x80, dev->io);      /* Select RDS fifo */
		while ((inb(dev->io) & 0x80) != 0) {
			dev->rdsbuf[dev->rdsin] = inb(dev->io + 1);
<<<<<<< HEAD
			if (dev->rdsin == dev->rdsout)
=======
			if (dev->rdsin + 1 == dev->rdsout)
>>>>>>> refs/remotes/origin/master
				printk(KERN_WARNING "cadet: RDS buffer overflow\n");
			else
				dev->rdsin++;
		}
		mutex_unlock(&dev->lock);
	}

	/*
	 * Service pending read
	 */
	if (dev->rdsin != dev->rdsout)
		wake_up_interruptible(&dev->read_queue);

	/*
	 * Clean up and exit
	 */
	init_timer(&dev->readtimer);
	dev->readtimer.function = cadet_handler;
<<<<<<< HEAD
	dev->readtimer.data = (unsigned long)0;
=======
	dev->readtimer.data = data;
>>>>>>> refs/remotes/origin/master
	dev->readtimer.expires = jiffies + msecs_to_jiffies(50);
	add_timer(&dev->readtimer);
}

<<<<<<< HEAD
=======
static void cadet_start_rds(struct cadet *dev)
{
	dev->rdsstat = 1;
	outb(0x80, dev->io);        /* Select RDS fifo */
	init_timer(&dev->readtimer);
	dev->readtimer.function = cadet_handler;
	dev->readtimer.data = (unsigned long)dev;
	dev->readtimer.expires = jiffies + msecs_to_jiffies(50);
	add_timer(&dev->readtimer);
}
>>>>>>> refs/remotes/origin/master

static ssize_t cadet_read(struct file *file, char __user *data, size_t count, loff_t *ppos)
{
	struct cadet *dev = video_drvdata(file);
	unsigned char readbuf[RDS_BUFFER];
	int i = 0;

	mutex_lock(&dev->lock);
<<<<<<< HEAD
	if (dev->rdsstat == 0) {
		dev->rdsstat = 1;
		outb(0x80, dev->io);        /* Select RDS fifo */
		init_timer(&dev->readtimer);
		dev->readtimer.function = cadet_handler;
		dev->readtimer.data = (unsigned long)dev;
		dev->readtimer.expires = jiffies + msecs_to_jiffies(50);
		add_timer(&dev->readtimer);
	}
	if (dev->rdsin == dev->rdsout) {
		mutex_unlock(&dev->lock);
		if (file->f_flags & O_NONBLOCK)
			return -EWOULDBLOCK;
=======
	if (dev->rdsstat == 0)
		cadet_start_rds(dev);
	if (dev->rdsin == dev->rdsout) {
		if (file->f_flags & O_NONBLOCK) {
			i = -EWOULDBLOCK;
			goto unlock;
		}
		mutex_unlock(&dev->lock);
>>>>>>> refs/remotes/origin/master
		interruptible_sleep_on(&dev->read_queue);
		mutex_lock(&dev->lock);
	}
	while (i < count && dev->rdsin != dev->rdsout)
		readbuf[i++] = dev->rdsbuf[dev->rdsout++];
<<<<<<< HEAD
	mutex_unlock(&dev->lock);

	if (copy_to_user(data, readbuf, i))
		return -EFAULT;
=======

	if (i && copy_to_user(data, readbuf, i))
		i = -EFAULT;
unlock:
	mutex_unlock(&dev->lock);
>>>>>>> refs/remotes/origin/master
	return i;
}


static int vidioc_querycap(struct file *file, void *priv,
				struct v4l2_capability *v)
{
	strlcpy(v->driver, "ADS Cadet", sizeof(v->driver));
	strlcpy(v->card, "ADS Cadet", sizeof(v->card));
	strlcpy(v->bus_info, "ISA", sizeof(v->bus_info));
<<<<<<< HEAD
<<<<<<< HEAD
	v->version = CADET_VERSION;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	v->capabilities = V4L2_CAP_TUNER | V4L2_CAP_RADIO |
			  V4L2_CAP_READWRITE | V4L2_CAP_RDS_CAPTURE;
=======
	v->device_caps = V4L2_CAP_TUNER | V4L2_CAP_RADIO |
			  V4L2_CAP_READWRITE | V4L2_CAP_RDS_CAPTURE;
	v->capabilities = v->device_caps | V4L2_CAP_DEVICE_CAPS;
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int vidioc_g_tuner(struct file *file, void *priv,
				struct v4l2_tuner *v)
{
	struct cadet *dev = video_drvdata(file);

<<<<<<< HEAD
	v->type = V4L2_TUNER_RADIO;
	switch (v->index) {
	case 0:
		strlcpy(v->name, "FM", sizeof(v->name));
		v->capability = V4L2_TUNER_CAP_STEREO | V4L2_TUNER_CAP_RDS |
			V4L2_TUNER_CAP_RDS_BLOCK_IO;
		v->rangelow = 1400;     /* 87.5 MHz */
		v->rangehigh = 1728;    /* 108.0 MHz */
		v->rxsubchans = cadet_getstereo(dev);
		switch (v->rxsubchans) {
		case V4L2_TUNER_SUB_MONO:
			v->audmode = V4L2_TUNER_MODE_MONO;
			break;
		case V4L2_TUNER_SUB_STEREO:
			v->audmode = V4L2_TUNER_MODE_STEREO;
			break;
		default:
			break;
		}
		v->rxsubchans |= V4L2_TUNER_SUB_RDS;
		break;
	case 1:
		strlcpy(v->name, "AM", sizeof(v->name));
		v->capability = V4L2_TUNER_CAP_LOW;
		v->rangelow = 8320;      /* 520 kHz */
		v->rangehigh = 26400;    /* 1650 kHz */
		v->rxsubchans = V4L2_TUNER_SUB_MONO;
		v->audmode = V4L2_TUNER_MODE_MONO;
		break;
	default:
		return -EINVAL;
	}
=======
	if (v->index)
		return -EINVAL;
	v->type = V4L2_TUNER_RADIO;
	strlcpy(v->name, "Radio", sizeof(v->name));
	v->capability = bands[0].capability | bands[1].capability;
	v->rangelow = bands[0].rangelow;	   /* 520 kHz (start of AM band) */
	v->rangehigh = bands[1].rangehigh;    /* 108.0 MHz (end of FM band) */
	if (dev->is_fm_band) {
		v->rxsubchans = cadet_getstereo(dev);
		outb(3, dev->io);
		outb(inb(dev->io + 1) & 0x7f, dev->io + 1);
		mdelay(100);
		outb(3, dev->io);
		if (inb(dev->io + 1) & 0x80)
			v->rxsubchans |= V4L2_TUNER_SUB_RDS;
	} else {
		v->rangelow = 8320;      /* 520 kHz */
		v->rangehigh = 26400;    /* 1650 kHz */
		v->rxsubchans = V4L2_TUNER_SUB_MONO;
	}
	v->audmode = V4L2_TUNER_MODE_STEREO;
>>>>>>> refs/remotes/origin/master
	v->signal = dev->sigstrength; /* We might need to modify scaling of this */
	return 0;
}

static int vidioc_s_tuner(struct file *file, void *priv,
<<<<<<< HEAD
				struct v4l2_tuner *v)
{
	struct cadet *dev = video_drvdata(file);

	if (v->index != 0 && v->index != 1)
		return -EINVAL;
	dev->curtuner = v->index;
=======
				const struct v4l2_tuner *v)
{
	return v->index ? -EINVAL : 0;
}

static int vidioc_enum_freq_bands(struct file *file, void *priv,
				struct v4l2_frequency_band *band)
{
	if (band->tuner)
		return -EINVAL;
	if (band->index >= ARRAY_SIZE(bands))
		return -EINVAL;
	*band = bands[band->index];
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int vidioc_g_frequency(struct file *file, void *priv,
				struct v4l2_frequency *f)
{
	struct cadet *dev = video_drvdata(file);

<<<<<<< HEAD
	f->tuner = dev->curtuner;
	f->type = V4L2_TUNER_RADIO;
	f->frequency = cadet_getfreq(dev);
=======
	if (f->tuner)
		return -EINVAL;
	f->type = V4L2_TUNER_RADIO;
	f->frequency = dev->curfreq;
>>>>>>> refs/remotes/origin/master
	return 0;
}


static int vidioc_s_frequency(struct file *file, void *priv,
<<<<<<< HEAD
				struct v4l2_frequency *f)
{
	struct cadet *dev = video_drvdata(file);

	if (f->type != V4L2_TUNER_RADIO)
		return -EINVAL;
	if (dev->curtuner == 0 && (f->frequency < 1400 || f->frequency > 1728))
		return -EINVAL;
	if (dev->curtuner == 1 && (f->frequency < 8320 || f->frequency > 26400))
		return -EINVAL;
=======
				const struct v4l2_frequency *f)
{
	struct cadet *dev = video_drvdata(file);

	if (f->tuner)
		return -EINVAL;
	dev->is_fm_band =
		f->frequency >= (bands[0].rangehigh + bands[1].rangelow) / 2;
>>>>>>> refs/remotes/origin/master
	cadet_setfreq(dev, f->frequency);
	return 0;
}

<<<<<<< HEAD
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
	struct cadet *dev = video_drvdata(file);

	switch (ctrl->id) {
	case V4L2_CID_AUDIO_MUTE: /* TODO: Handle this correctly */
		ctrl->value = (cadet_getvol(dev) == 0);
		break;
	case V4L2_CID_AUDIO_VOLUME:
		ctrl->value = cadet_getvol(dev);
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int vidioc_s_ctrl(struct file *file, void *priv,
				struct v4l2_control *ctrl)
{
	struct cadet *dev = video_drvdata(file);

	switch (ctrl->id){
	case V4L2_CID_AUDIO_MUTE: /* TODO: Handle this correctly */
		if (ctrl->value)
			cadet_setvol(dev, 0);
		else
			cadet_setvol(dev, 0xffff);
		break;
	case V4L2_CID_AUDIO_VOLUME:
		cadet_setvol(dev, ctrl->value);
		break;
	default:
		return -EINVAL;
	}
	return 0;
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
=======
static int cadet_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct cadet *dev = container_of(ctrl->handler, struct cadet, ctrl_handler);

	switch (ctrl->id) {
	case V4L2_CID_AUDIO_MUTE:
		outb(7, dev->io);                /* Select tuner control */
		if (ctrl->val)
			outb(0x00, dev->io + 1);
		else
			outb(0x20, dev->io + 1);
		return 0;
	}
	return -EINVAL;
>>>>>>> refs/remotes/origin/master
}

static int cadet_open(struct file *file)
{
	struct cadet *dev = video_drvdata(file);
<<<<<<< HEAD

	mutex_lock(&dev->lock);
	dev->users++;
	if (1 == dev->users)
		init_waitqueue_head(&dev->read_queue);
	mutex_unlock(&dev->lock);
	return 0;
=======
	int err;

	mutex_lock(&dev->lock);
	err = v4l2_fh_open(file);
	if (err)
		goto fail;
	if (v4l2_fh_is_singular_file(file))
		init_waitqueue_head(&dev->read_queue);
fail:
	mutex_unlock(&dev->lock);
	return err;
>>>>>>> refs/remotes/origin/master
}

static int cadet_release(struct file *file)
{
	struct cadet *dev = video_drvdata(file);

	mutex_lock(&dev->lock);
<<<<<<< HEAD
	dev->users--;
	if (0 == dev->users) {
		del_timer_sync(&dev->readtimer);
		dev->rdsstat = 0;
	}
=======
	if (v4l2_fh_is_singular_file(file) && dev->rdsstat) {
		del_timer_sync(&dev->readtimer);
		dev->rdsstat = 0;
	}
	v4l2_fh_release(file);
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&dev->lock);
	return 0;
}

static unsigned int cadet_poll(struct file *file, struct poll_table_struct *wait)
{
	struct cadet *dev = video_drvdata(file);
<<<<<<< HEAD

	poll_wait(file, &dev->read_queue, wait);
	if (dev->rdsin != dev->rdsout)
		return POLLIN | POLLRDNORM;
	return 0;
=======
	unsigned long req_events = poll_requested_events(wait);
	unsigned int res = v4l2_ctrl_poll(file, wait);

	poll_wait(file, &dev->read_queue, wait);
	if (dev->rdsstat == 0 && (req_events & (POLLIN | POLLRDNORM))) {
		mutex_lock(&dev->lock);
		if (dev->rdsstat == 0)
			cadet_start_rds(dev);
		mutex_unlock(&dev->lock);
	}
	if (dev->rdsin != dev->rdsout)
		res |= POLLIN | POLLRDNORM;
	return res;
>>>>>>> refs/remotes/origin/master
}


static const struct v4l2_file_operations cadet_fops = {
	.owner		= THIS_MODULE,
	.open		= cadet_open,
	.release       	= cadet_release,
	.read		= cadet_read,
	.unlocked_ioctl	= video_ioctl2,
	.poll		= cadet_poll,
};

static const struct v4l2_ioctl_ops cadet_ioctl_ops = {
	.vidioc_querycap    = vidioc_querycap,
	.vidioc_g_tuner     = vidioc_g_tuner,
	.vidioc_s_tuner     = vidioc_s_tuner,
	.vidioc_g_frequency = vidioc_g_frequency,
	.vidioc_s_frequency = vidioc_s_frequency,
<<<<<<< HEAD
	.vidioc_queryctrl   = vidioc_queryctrl,
	.vidioc_g_ctrl      = vidioc_g_ctrl,
	.vidioc_s_ctrl      = vidioc_s_ctrl,
	.vidioc_g_audio     = vidioc_g_audio,
	.vidioc_s_audio     = vidioc_s_audio,
	.vidioc_g_input     = vidioc_g_input,
	.vidioc_s_input     = vidioc_s_input,
=======
	.vidioc_enum_freq_bands = vidioc_enum_freq_bands,
	.vidioc_log_status  = v4l2_ctrl_log_status,
	.vidioc_subscribe_event = v4l2_ctrl_subscribe_event,
	.vidioc_unsubscribe_event = v4l2_event_unsubscribe,
};

static const struct v4l2_ctrl_ops cadet_ctrl_ops = {
	.s_ctrl = cadet_s_ctrl,
>>>>>>> refs/remotes/origin/master
};

#ifdef CONFIG_PNP

static struct pnp_device_id cadet_pnp_devices[] = {
	/* ADS Cadet AM/FM Radio Card */
	{.id = "MSM0c24", .driver_data = 0},
	{.id = ""}
};

MODULE_DEVICE_TABLE(pnp, cadet_pnp_devices);

static int cadet_pnp_probe(struct pnp_dev *dev, const struct pnp_device_id *dev_id)
{
	if (!dev)
		return -ENODEV;
	/* only support one device */
	if (io > 0)
		return -EBUSY;

	if (!pnp_port_valid(dev, 0))
		return -ENODEV;

	io = pnp_port_start(dev, 0);

	printk(KERN_INFO "radio-cadet: PnP reports device at %#x\n", io);

	return io;
}

static struct pnp_driver cadet_pnp_driver = {
	.name		= "radio-cadet",
	.id_table	= cadet_pnp_devices,
	.probe		= cadet_pnp_probe,
	.remove		= NULL,
};

#else
static struct pnp_driver cadet_pnp_driver;
#endif

static void cadet_probe(struct cadet *dev)
{
	static int iovals[8] = { 0x330, 0x332, 0x334, 0x336, 0x338, 0x33a, 0x33c, 0x33e };
	int i;

	for (i = 0; i < 8; i++) {
		dev->io = iovals[i];
		if (request_region(dev->io, 2, "cadet-probe")) {
<<<<<<< HEAD
			cadet_setfreq(dev, 1410);
			if (cadet_getfreq(dev) == 1410) {
=======
			cadet_setfreq(dev, bands[1].rangelow);
			if (cadet_getfreq(dev) == bands[1].rangelow) {
>>>>>>> refs/remotes/origin/master
				release_region(dev->io, 2);
				return;
			}
			release_region(dev->io, 2);
		}
	}
	dev->io = -1;
}

/*
 * io should only be set if the user has used something like
 * isapnp (the userspace program) to initialize this card for us
 */

static int __init cadet_init(void)
{
	struct cadet *dev = &cadet_card;
	struct v4l2_device *v4l2_dev = &dev->v4l2_dev;
<<<<<<< HEAD
	int res;
=======
	struct v4l2_ctrl_handler *hdl;
	int res = -ENODEV;
>>>>>>> refs/remotes/origin/master

	strlcpy(v4l2_dev->name, "cadet", sizeof(v4l2_dev->name));
	mutex_init(&dev->lock);

	/* If a probe was requested then probe ISAPnP first (safest) */
	if (io < 0)
		pnp_register_driver(&cadet_pnp_driver);
	dev->io = io;

	/* If that fails then probe unsafely if probe is requested */
	if (dev->io < 0)
		cadet_probe(dev);

	/* Else we bail out */
	if (dev->io < 0) {
#ifdef MODULE
		v4l2_err(v4l2_dev, "you must set an I/O address with io=0x330, 0x332, 0x334,\n");
		v4l2_err(v4l2_dev, "0x336, 0x338, 0x33a, 0x33c or 0x33e\n");
#endif
		goto fail;
	}
	if (!request_region(dev->io, 2, "cadet"))
		goto fail;

	res = v4l2_device_register(NULL, v4l2_dev);
	if (res < 0) {
		release_region(dev->io, 2);
		v4l2_err(v4l2_dev, "could not register v4l2_device\n");
		goto fail;
	}

<<<<<<< HEAD
=======
	hdl = &dev->ctrl_handler;
	v4l2_ctrl_handler_init(hdl, 2);
	v4l2_ctrl_new_std(hdl, &cadet_ctrl_ops,
			V4L2_CID_AUDIO_MUTE, 0, 1, 1, 1);
	v4l2_dev->ctrl_handler = hdl;
	if (hdl->error) {
		res = hdl->error;
		v4l2_err(v4l2_dev, "Could not register controls\n");
		goto err_hdl;
	}

	dev->is_fm_band = true;
	dev->curfreq = bands[dev->is_fm_band].rangelow;
	cadet_setfreq(dev, dev->curfreq);
>>>>>>> refs/remotes/origin/master
	strlcpy(dev->vdev.name, v4l2_dev->name, sizeof(dev->vdev.name));
	dev->vdev.v4l2_dev = v4l2_dev;
	dev->vdev.fops = &cadet_fops;
	dev->vdev.ioctl_ops = &cadet_ioctl_ops;
	dev->vdev.release = video_device_release_empty;
<<<<<<< HEAD
	video_set_drvdata(&dev->vdev, dev);

	if (video_register_device(&dev->vdev, VFL_TYPE_RADIO, radio_nr) < 0) {
		v4l2_device_unregister(v4l2_dev);
		release_region(dev->io, 2);
		goto fail;
	}
	v4l2_info(v4l2_dev, "ADS Cadet Radio Card at 0x%x\n", dev->io);
	return 0;
fail:
	pnp_unregister_driver(&cadet_pnp_driver);
	return -ENODEV;
=======
	dev->vdev.lock = &dev->lock;
	set_bit(V4L2_FL_USE_FH_PRIO, &dev->vdev.flags);
	video_set_drvdata(&dev->vdev, dev);

	res = video_register_device(&dev->vdev, VFL_TYPE_RADIO, radio_nr);
	if (res < 0)
		goto err_hdl;
	v4l2_info(v4l2_dev, "ADS Cadet Radio Card at 0x%x\n", dev->io);
	return 0;
err_hdl:
	v4l2_ctrl_handler_free(hdl);
	v4l2_device_unregister(v4l2_dev);
	release_region(dev->io, 2);
fail:
	pnp_unregister_driver(&cadet_pnp_driver);
	return res;
>>>>>>> refs/remotes/origin/master
}

static void __exit cadet_exit(void)
{
	struct cadet *dev = &cadet_card;

	video_unregister_device(&dev->vdev);
<<<<<<< HEAD
	v4l2_device_unregister(&dev->v4l2_dev);
=======
	v4l2_ctrl_handler_free(&dev->ctrl_handler);
	v4l2_device_unregister(&dev->v4l2_dev);
	outb(7, dev->io);	/* Mute */
	outb(0x00, dev->io + 1);
>>>>>>> refs/remotes/origin/master
	release_region(dev->io, 2);
	pnp_unregister_driver(&cadet_pnp_driver);
}

module_init(cadet_init);
module_exit(cadet_exit);

