/*
 * Guillemot Maxi Radio FM 2000 PCI radio card driver for Linux
 * (C) 2001 Dimitromanolakis Apostolos <apdim@grecian.net>
 *
 * Based in the radio Maestro PCI driver. Actually it uses the same chip
 * for radio but different pci controller.
 *
 * I didn't have any specs I reversed engineered the protocol from
 * the windows driver (radio.dll).
 *
 * The card uses the TEA5757 chip that includes a search function but it
 * is useless as I haven't found any way to read back the frequency. If
 * anybody does please mail me.
 *
 * For the pdf file see:
 * http://www.nxp.com/acrobat_download2/expired_datasheets/TEA5757_5759_3.pdf 
 *
 *
 * CHANGES:
 *   0.75b
 *     - better pci interface thanks to Francois Romieu <romieu@cogenit.fr>
 *
 *   0.75      Sun Feb  4 22:51:27 EET 2001
 *     - tiding up
 *     - removed support for multiple devices as it didn't work anyway
 *
 * BUGS:
 *   - card unmutes if you change frequency
 *
 * (c) 2006, 2007 by Mauro Carvalho Chehab <mchehab@infradead.org>:
 *	- Conversion to V4L2 API
 *      - Uses video_ioctl2 for parsing and to add debug support
 */


#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/pci.h>
#include <linux/videodev2.h>
<<<<<<< HEAD
#include <linux/version.h>      /* for KERNEL_VERSION MACRO     */
#include <linux/io.h>
#include <linux/slab.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>

MODULE_AUTHOR("Dimitromanolakis Apostolos, apdim@grecian.net");
MODULE_DESCRIPTION("Radio driver for the Guillemot Maxi Radio FM2000 radio.");
MODULE_LICENSE("GPL");

static int radio_nr = -1;
module_param(radio_nr, int, 0);

static int debug;

module_param(debug, int, 0644);
MODULE_PARM_DESC(debug, "activates debug info");

#define DRIVER_VERSION	"0.77"

#define RADIO_VERSION KERNEL_VERSION(0, 7, 7)

#define dprintk(dev, num, fmt, arg...) \
	v4l2_dbg(num, debug, &dev->v4l2_dev, fmt, ## arg)

#ifndef PCI_VENDOR_ID_GUILLEMOT
#define PCI_VENDOR_ID_GUILLEMOT 0x5046
#endif

#ifndef PCI_DEVICE_ID_GUILLEMOT
#define PCI_DEVICE_ID_GUILLEMOT_MAXIRADIO 0x1001
#endif

=======
#include <linux/io.h>
#include <linux/slab.h>
#include <sound/tea575x-tuner.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-event.h>

MODULE_AUTHOR("Dimitromanolakis Apostolos, apdim@grecian.net");
MODULE_DESCRIPTION("Radio driver for the Guillemot Maxi Radio FM2000.");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0.0");

static int radio_nr = -1;
module_param(radio_nr, int, 0644);
MODULE_PARM_DESC(radio_nr, "Radio device number");
>>>>>>> refs/remotes/origin/cm-10.0

/* TEA5757 pin mappings */
static const int clk = 1, data = 2, wren = 4, mo_st = 8, power = 16;

<<<<<<< HEAD
#define FREQ_LO		(87 * 16000)
#define FREQ_HI		(108 * 16000)

#define FREQ_IF         171200 /* 10.7*16000   */
#define FREQ_STEP       200    /* 12.5*16      */

/* (x==fmhz*16*1000) -> bits */
#define FREQ2BITS(x) \
  ((((unsigned int)(x) + FREQ_IF + (FREQ_STEP << 1)) / (FREQ_STEP << 2)) << 2)

#define BITS2FREQ(x)	((x) * FREQ_STEP - FREQ_IF)


struct maxiradio
{
	struct v4l2_device v4l2_dev;
	struct video_device vdev;
	struct pci_dev *pdev;

	u16	io;	/* base of radio io */
	u16	muted;	/* VIDEO_AUDIO_MUTE */
	u16	stereo;	/* VIDEO_TUNER_STEREO_ON */
	u16	tuned;	/* signal strength (0 or 0xffff) */

	unsigned long freq;

	struct mutex lock;
=======
static atomic_t maxiradio_instance = ATOMIC_INIT(0);

#define PCI_VENDOR_ID_GUILLEMOT 0x5046
#define PCI_DEVICE_ID_GUILLEMOT_MAXIRADIO 0x1001

struct maxiradio
{
	struct snd_tea575x tea;
	struct v4l2_device v4l2_dev;
	struct pci_dev *pdev;

	u16	io;	/* base of radio io */
>>>>>>> refs/remotes/origin/cm-10.0
};

static inline struct maxiradio *to_maxiradio(struct v4l2_device *v4l2_dev)
{
	return container_of(v4l2_dev, struct maxiradio, v4l2_dev);
}

<<<<<<< HEAD
static void outbit(unsigned long bit, u16 io)
{
	int val = power | wren | (bit ? data : 0);

	outb(val, io);
	udelay(4);
	outb(val | clk, io);
	udelay(4);
	outb(val, io);
	udelay(4);
}

static void turn_power(struct maxiradio *dev, int p)
{
	if (p != 0) {
		dprintk(dev, 1, "Radio powered on\n");
		outb(power, dev->io);
	} else {
		dprintk(dev, 1, "Radio powered off\n");
		outb(0, dev->io);
	}
}

static void set_freq(struct maxiradio *dev, u32 freq)
{
	unsigned long int si;
	int bl;
	int io = dev->io;
	int val = FREQ2BITS(freq);

	/* TEA5757 shift register bits (see pdf) */

	outbit(0, io); /* 24  search */
	outbit(1, io); /* 23  search up/down */

	outbit(0, io); /* 22  stereo/mono */

	outbit(0, io); /* 21  band */
	outbit(0, io); /* 20  band (only 00=FM works I think) */

	outbit(0, io); /* 19  port ? */
	outbit(0, io); /* 18  port ? */

	outbit(0, io); /* 17  search level */
	outbit(0, io); /* 16  search level */

	si = 0x8000;
	for (bl = 1; bl <= 16; bl++) {
		outbit(val & si, io);
		si >>= 1;
	}

	dprintk(dev, 1, "Radio freq set to %d.%02d MHz\n",
				freq / 16000,
				freq % 16000 * 100 / 16000);

	turn_power(dev, 1);
}

static int get_stereo(u16 io)
{
	outb(power,io);
	udelay(4);

	return !(inb(io) & mo_st);
}

static int get_tune(u16 io)
{
	outb(power+clk,io);
	udelay(4);

	return !(inb(io) & mo_st);
}


static int vidioc_querycap(struct file *file, void  *priv,
			    struct v4l2_capability *v)
{
	struct maxiradio *dev = video_drvdata(file);

	strlcpy(v->driver, "radio-maxiradio", sizeof(v->driver));
	strlcpy(v->card, "Maxi Radio FM2000 radio", sizeof(v->card));
	snprintf(v->bus_info, sizeof(v->bus_info), "PCI:%s", pci_name(dev->pdev));
	v->version = RADIO_VERSION;
	v->capabilities = V4L2_CAP_TUNER | V4L2_CAP_RADIO;
	return 0;
}

static int vidioc_g_tuner(struct file *file, void *priv,
			   struct v4l2_tuner *v)
{
	struct maxiradio *dev = video_drvdata(file);

	if (v->index > 0)
		return -EINVAL;

	mutex_lock(&dev->lock);
	strlcpy(v->name, "FM", sizeof(v->name));
	v->type = V4L2_TUNER_RADIO;
	v->rangelow = FREQ_LO;
	v->rangehigh = FREQ_HI;
	v->rxsubchans = V4L2_TUNER_SUB_MONO | V4L2_TUNER_SUB_STEREO;
	v->capability = V4L2_TUNER_CAP_LOW;
	if (get_stereo(dev->io))
		v->audmode = V4L2_TUNER_MODE_STEREO;
	else
		v->audmode = V4L2_TUNER_MODE_MONO;
	v->signal = 0xffff * get_tune(dev->io);
	mutex_unlock(&dev->lock);

	return 0;
}

static int vidioc_s_tuner(struct file *file, void *priv,
			   struct v4l2_tuner *v)
{
	return v->index ? -EINVAL : 0;
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

static int vidioc_s_frequency(struct file *file, void *priv,
			       struct v4l2_frequency *f)
{
	struct maxiradio *dev = video_drvdata(file);

	if (f->tuner != 0 || f->type != V4L2_TUNER_RADIO)
		return -EINVAL;
	if (f->frequency < FREQ_LO || f->frequency > FREQ_HI) {
		dprintk(dev, 1, "radio freq (%d.%02d MHz) out of range (%d-%d)\n",
					f->frequency / 16000,
					f->frequency % 16000 * 100 / 16000,
					FREQ_LO / 16000, FREQ_HI / 16000);

		return -EINVAL;
	}

	mutex_lock(&dev->lock);
	dev->freq = f->frequency;
	set_freq(dev, dev->freq);
	msleep(125);
	mutex_unlock(&dev->lock);

	return 0;
}

static int vidioc_g_frequency(struct file *file, void *priv,
			       struct v4l2_frequency *f)
{
	struct maxiradio *dev = video_drvdata(file);

	if (f->tuner != 0)
		return -EINVAL;
	f->type = V4L2_TUNER_RADIO;
	f->frequency = dev->freq;

	dprintk(dev, 4, "radio freq is %d.%02d MHz",
				f->frequency / 16000,
				f->frequency % 16000 * 100 / 16000);

	return 0;
}

static int vidioc_queryctrl(struct file *file, void *priv,
			     struct v4l2_queryctrl *qc)
{
	switch (qc->id) {
	case V4L2_CID_AUDIO_MUTE:
		return v4l2_ctrl_query_fill(qc, 0, 1, 1, 1);
	}
	return -EINVAL;
}

static int vidioc_g_ctrl(struct file *file, void *priv,
		struct v4l2_control *ctrl)
{
	struct maxiradio *dev = video_drvdata(file);

	switch (ctrl->id) {
	case V4L2_CID_AUDIO_MUTE:
		ctrl->value = dev->muted;
		return 0;
	}

	return -EINVAL;
}

static int vidioc_s_ctrl(struct file *file, void *priv,
		struct v4l2_control *ctrl)
{
	struct maxiradio *dev = video_drvdata(file);

	switch (ctrl->id) {
	case V4L2_CID_AUDIO_MUTE:
		mutex_lock(&dev->lock);
		dev->muted = ctrl->value;
		if (dev->muted)
			turn_power(dev, 0);
		else
			set_freq(dev, dev->freq);
		mutex_unlock(&dev->lock);
		return 0;
	}

	return -EINVAL;
}

static const struct v4l2_file_operations maxiradio_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl = video_ioctl2,
};

static const struct v4l2_ioctl_ops maxiradio_ioctl_ops = {
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
};

static int __devinit maxiradio_init_one(struct pci_dev *pdev, const struct pci_device_id *ent)
=======
static void maxiradio_tea575x_set_pins(struct snd_tea575x *tea, u8 pins)
{
	struct maxiradio *dev = tea->private_data;
	u8 bits = 0;

	bits |= (pins & TEA575X_DATA) ? data : 0;
	bits |= (pins & TEA575X_CLK)  ? clk  : 0;
	bits |= (pins & TEA575X_WREN) ? wren : 0;
	bits |= power;

	outb(bits, dev->io);
}

/* Note: this card cannot read out the data of the shift registers,
   only the mono/stereo pin works. */
static u8 maxiradio_tea575x_get_pins(struct snd_tea575x *tea)
{
	struct maxiradio *dev = tea->private_data;
	u8 bits = inb(dev->io);

	return  ((bits & data) ? TEA575X_DATA : 0) |
		((bits & mo_st) ? TEA575X_MOST : 0);
}

static void maxiradio_tea575x_set_direction(struct snd_tea575x *tea, bool output)
{
}

static struct snd_tea575x_ops maxiradio_tea_ops = {
	.set_pins = maxiradio_tea575x_set_pins,
	.get_pins = maxiradio_tea575x_get_pins,
	.set_direction = maxiradio_tea575x_set_direction,
};

static int __devinit maxiradio_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct maxiradio *dev;
	struct v4l2_device *v4l2_dev;
	int retval = -ENOMEM;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (dev == NULL) {
		dev_err(&pdev->dev, "not enough memory\n");
		return -ENOMEM;
	}

	v4l2_dev = &dev->v4l2_dev;
<<<<<<< HEAD
	mutex_init(&dev->lock);
	dev->pdev = pdev;
	dev->muted = 1;
	dev->freq = FREQ_LO;

	strlcpy(v4l2_dev->name, "maxiradio", sizeof(v4l2_dev->name));
=======
	v4l2_device_set_name(v4l2_dev, "maxiradio", &maxiradio_instance);
>>>>>>> refs/remotes/origin/cm-10.0

	retval = v4l2_device_register(&pdev->dev, v4l2_dev);
	if (retval < 0) {
		v4l2_err(v4l2_dev, "Could not register v4l2_device\n");
		goto errfr;
	}
<<<<<<< HEAD

	if (!request_region(pci_resource_start(pdev, 0),
			   pci_resource_len(pdev, 0), "Maxi Radio FM 2000")) {
		v4l2_err(v4l2_dev, "can't reserve I/O ports\n");
		goto err_out;
=======
	dev->tea.private_data = dev;
	dev->tea.ops = &maxiradio_tea_ops;
	/* The data pin cannot be read. This may be a hardware limitation, or
	   we just don't know how to read it. */
	dev->tea.cannot_read_data = true;
	dev->tea.v4l2_dev = v4l2_dev;
	dev->tea.radio_nr = radio_nr;
	strlcpy(dev->tea.card, "Maxi Radio FM2000", sizeof(dev->tea.card));
	snprintf(dev->tea.bus_info, sizeof(dev->tea.bus_info),
			"PCI:%s", pci_name(pdev));

	retval = -ENODEV;

	if (!request_region(pci_resource_start(pdev, 0),
			   pci_resource_len(pdev, 0), v4l2_dev->name)) {
		dev_err(&pdev->dev, "can't reserve I/O ports\n");
		goto err_hdl;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	if (pci_enable_device(pdev))
		goto err_out_free_region;

	dev->io = pci_resource_start(pdev, 0);
<<<<<<< HEAD
	strlcpy(dev->vdev.name, v4l2_dev->name, sizeof(dev->vdev.name));
	dev->vdev.v4l2_dev = v4l2_dev;
	dev->vdev.fops = &maxiradio_fops;
	dev->vdev.ioctl_ops = &maxiradio_ioctl_ops;
	dev->vdev.release = video_device_release_empty;
	video_set_drvdata(&dev->vdev, dev);

	if (video_register_device(&dev->vdev, VFL_TYPE_RADIO, radio_nr) < 0) {
		v4l2_err(v4l2_dev, "can't register device!");
		goto err_out_free_region;
	}

	v4l2_info(v4l2_dev, "version " DRIVER_VERSION "\n");

	v4l2_info(v4l2_dev, "found Guillemot MAXI Radio device (io = 0x%x)\n",
	       dev->io);
=======
	if (snd_tea575x_init(&dev->tea)) {
		printk(KERN_ERR "radio-maxiradio: Unable to detect TEA575x tuner\n");
		goto err_out_free_region;
	}
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;

err_out_free_region:
	release_region(pci_resource_start(pdev, 0), pci_resource_len(pdev, 0));
<<<<<<< HEAD
err_out:
	v4l2_device_unregister(v4l2_dev);
errfr:
	kfree(dev);
	return -ENODEV;
}

static void __devexit maxiradio_remove_one(struct pci_dev *pdev)
=======
err_hdl:
	v4l2_device_unregister(v4l2_dev);
errfr:
	kfree(dev);
	return retval;
}

static void __devexit maxiradio_remove(struct pci_dev *pdev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct v4l2_device *v4l2_dev = dev_get_drvdata(&pdev->dev);
	struct maxiradio *dev = to_maxiradio(v4l2_dev);

<<<<<<< HEAD
	video_unregister_device(&dev->vdev);
	v4l2_device_unregister(&dev->v4l2_dev);
=======
	snd_tea575x_exit(&dev->tea);
	/* Turn off power */
	outb(0, dev->io);
	v4l2_device_unregister(v4l2_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	release_region(pci_resource_start(pdev, 0), pci_resource_len(pdev, 0));
}

static struct pci_device_id maxiradio_pci_tbl[] = {
	{ PCI_VENDOR_ID_GUILLEMOT, PCI_DEVICE_ID_GUILLEMOT_MAXIRADIO,
		PCI_ANY_ID, PCI_ANY_ID, },
	{ 0 }
};

MODULE_DEVICE_TABLE(pci, maxiradio_pci_tbl);

static struct pci_driver maxiradio_driver = {
	.name		= "radio-maxiradio",
	.id_table	= maxiradio_pci_tbl,
<<<<<<< HEAD
	.probe		= maxiradio_init_one,
	.remove		= __devexit_p(maxiradio_remove_one),
};

static int __init maxiradio_radio_init(void)
=======
	.probe		= maxiradio_probe,
	.remove		= __devexit_p(maxiradio_remove),
};

static int __init maxiradio_init(void)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return pci_register_driver(&maxiradio_driver);
}

<<<<<<< HEAD
static void __exit maxiradio_radio_exit(void)
=======
static void __exit maxiradio_exit(void)
>>>>>>> refs/remotes/origin/cm-10.0
{
	pci_unregister_driver(&maxiradio_driver);
}

<<<<<<< HEAD
module_init(maxiradio_radio_init);
module_exit(maxiradio_radio_exit);
=======
module_init(maxiradio_init);
module_exit(maxiradio_exit);
>>>>>>> refs/remotes/origin/cm-10.0
