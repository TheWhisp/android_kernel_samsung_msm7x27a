<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/slab.h>
#include <linux/serio.h>
#include <linux/delay.h>
#include "nvec.h"

#define START_STREAMING	{'\x06','\x03','\x01'}
#define STOP_STREAMING	{'\x06','\x04'}
#define SEND_COMMAND	{'\x06','\x01','\xf4','\x01'}

struct nvec_ps2
{
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * nvec_ps2: mouse driver for a NVIDIA compliant embedded controller
 *
 * Copyright (C) 2011 The AC100 Kernel Team <ac100@lists.launchpad.net>
 *
 * Authors:  Pierre-Hugues Husson <phhusson@free.fr>
 *           Ilya Petrov <ilya.muromec@gmail.com>
 *           Marc Dietrich <marvin24@gmx.de>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/serio.h>
#include <linux/delay.h>
#include <linux/platform_device.h>

#include "nvec.h"

<<<<<<< HEAD
#define START_STREAMING	{'\x06', '\x03', '\x06'}
#define STOP_STREAMING	{'\x06', '\x04'}
#define SEND_COMMAND	{'\x06', '\x01', '\xf4', '\x01'}
=======
#define PACKET_SIZE	6

#define ENABLE_MOUSE	0xf4
#define DISABLE_MOUSE	0xf5
#define PSMOUSE_RST	0xff
>>>>>>> refs/remotes/origin/master

#ifdef NVEC_PS2_DEBUG
#define NVEC_PHD(str, buf, len) \
	print_hex_dump(KERN_DEBUG, str, DUMP_PREFIX_NONE, \
			16, 1, buf, len, false)
#else
#define NVEC_PHD(str, buf, len)
#endif

<<<<<<< HEAD
static const unsigned char MOUSE_RESET[] = {'\x06', '\x01', '\xff', '\x03'};

struct nvec_ps2 {
>>>>>>> refs/remotes/origin/cm-10.0
=======
enum ps2_subcmds {
	SEND_COMMAND = 1,
	RECEIVE_N,
	AUTO_RECEIVE_N,
	CANCEL_AUTO_RECEIVE,
};

struct nvec_ps2 {
>>>>>>> refs/remotes/origin/master
	struct serio *ser_dev;
	struct notifier_block notifier;
	struct nvec_chip *nvec;
};

static struct nvec_ps2 ps2_dev;

static int ps2_startstreaming(struct serio *ser_dev)
{
<<<<<<< HEAD
	unsigned char buf[] = START_STREAMING;
<<<<<<< HEAD
	nvec_write_async(ps2_dev.nvec, buf, sizeof(buf));
	return 0;
=======
	return nvec_write_async(ps2_dev.nvec, buf, sizeof(buf));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned char buf[] = { NVEC_PS2, AUTO_RECEIVE_N, PACKET_SIZE };
	return nvec_write_async(ps2_dev.nvec, buf, sizeof(buf));
>>>>>>> refs/remotes/origin/master
}

static void ps2_stopstreaming(struct serio *ser_dev)
{
<<<<<<< HEAD
	unsigned char buf[] = STOP_STREAMING;
	nvec_write_async(ps2_dev.nvec, buf, sizeof(buf));
}

<<<<<<< HEAD
/* is this really needed?
static void nvec_resp_handler(unsigned char *data) {
	serio_interrupt(ser_dev, data[4], 0);
}
*/

=======
>>>>>>> refs/remotes/origin/cm-10.0
static int ps2_sendcommand(struct serio *ser_dev, unsigned char cmd)
{
	unsigned char buf[] = SEND_COMMAND;
=======
	unsigned char buf[] = { NVEC_PS2, CANCEL_AUTO_RECEIVE };
	nvec_write_async(ps2_dev.nvec, buf, sizeof(buf));
}

static int ps2_sendcommand(struct serio *ser_dev, unsigned char cmd)
{
	unsigned char buf[] = { NVEC_PS2, SEND_COMMAND, ENABLE_MOUSE, 1 };
>>>>>>> refs/remotes/origin/master

	buf[2] = cmd & 0xff;

	dev_dbg(&ser_dev->dev, "Sending ps2 cmd %02x\n", cmd);
<<<<<<< HEAD
<<<<<<< HEAD
	nvec_write_async(ps2_dev.nvec, buf, sizeof(buf));

	return 0;
}

static int nvec_ps2_notifier(struct notifier_block *nb,
				unsigned long event_type, void *data)
=======
=======
>>>>>>> refs/remotes/origin/master
	return nvec_write_async(ps2_dev.nvec, buf, sizeof(buf));
}

static int nvec_ps2_notifier(struct notifier_block *nb,
			     unsigned long event_type, void *data)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	int i;
	unsigned char *msg = (unsigned char *)data;

	switch (event_type) {
<<<<<<< HEAD
<<<<<<< HEAD
		case NVEC_PS2_EVT:
			serio_interrupt(ps2_dev.ser_dev, msg[2], 0);
			return NOTIFY_STOP;

		case NVEC_PS2:
			if (msg[2] == 1)
				for(i = 0; i < (msg[1] - 2); i++)
					serio_interrupt(ps2_dev.ser_dev, msg[i+4], 0);
			else if (msg[1] != 2) /* !ack */
			{
				printk("nvec_ps2: unhandled mouse event ");
				for(i = 0; i <= (msg[1]+1); i++)
					printk("%02x ", msg[i]);
				printk(".\n");
			}

			return NOTIFY_STOP;
=======
=======
>>>>>>> refs/remotes/origin/master
	case NVEC_PS2_EVT:
		for (i = 0; i < msg[1]; i++)
			serio_interrupt(ps2_dev.ser_dev, msg[2 + i], 0);
		NVEC_PHD("ps/2 mouse event: ", &msg[2], msg[1]);
		return NOTIFY_STOP;

	case NVEC_PS2:
		if (msg[2] == 1) {
			for (i = 0; i < (msg[1] - 2); i++)
				serio_interrupt(ps2_dev.ser_dev, msg[i + 4], 0);
			NVEC_PHD("ps/2 mouse reply: ", &msg[4], msg[1] - 2);
		}

		else if (msg[1] != 2) /* !ack */
			NVEC_PHD("unhandled mouse event: ", msg, msg[1] + 2);
		return NOTIFY_STOP;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	return NOTIFY_DONE;
}

<<<<<<< HEAD
<<<<<<< HEAD

int __init nvec_ps2(struct nvec_chip *nvec)
{
	struct serio *ser_dev = kzalloc(sizeof(struct serio), GFP_KERNEL);

	ser_dev->id.type=SERIO_8042;
	ser_dev->write=ps2_sendcommand;
	ser_dev->open=ps2_startstreaming;
	ser_dev->close=ps2_stopstreaming;

	strlcpy(ser_dev->name, "NVEC PS2", sizeof(ser_dev->name));
	strlcpy(ser_dev->phys, "NVEC I2C slave", sizeof(ser_dev->phys));
=======
static int __devinit nvec_mouse_probe(struct platform_device *pdev)
{
	struct nvec_chip *nvec = dev_get_drvdata(pdev->dev.parent);
	struct serio *ser_dev = kzalloc(sizeof(struct serio), GFP_KERNEL);
=======
static int nvec_mouse_probe(struct platform_device *pdev)
{
	struct nvec_chip *nvec = dev_get_drvdata(pdev->dev.parent);
	struct serio *ser_dev;
	char mouse_reset[] = { NVEC_PS2, SEND_COMMAND, PSMOUSE_RST, 3 };

	ser_dev = kzalloc(sizeof(struct serio), GFP_KERNEL);
	if (ser_dev == NULL)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master

	ser_dev->id.type = SERIO_PS_PSTHRU;
	ser_dev->write = ps2_sendcommand;
	ser_dev->start = ps2_startstreaming;
	ser_dev->stop = ps2_stopstreaming;

	strlcpy(ser_dev->name, "nvec mouse", sizeof(ser_dev->name));
	strlcpy(ser_dev->phys, "nvec", sizeof(ser_dev->phys));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	ps2_dev.ser_dev = ser_dev;
	ps2_dev.notifier.notifier_call = nvec_ps2_notifier;
	ps2_dev.nvec = nvec;
	nvec_register_notifier(nvec, &ps2_dev.notifier, 0);

	serio_register_port(ser_dev);

	/* mouse reset */
<<<<<<< HEAD
<<<<<<< HEAD
	nvec_write_async(nvec, "\x06\x01\xff\x03", 4);

	return 0;
}
=======
	nvec_write_async(nvec, MOUSE_RESET, 4);
=======
	nvec_write_async(nvec, mouse_reset, sizeof(mouse_reset));

	return 0;
}

static int nvec_mouse_remove(struct platform_device *pdev)
{
	struct nvec_chip *nvec = dev_get_drvdata(pdev->dev.parent);

	ps2_sendcommand(ps2_dev.ser_dev, DISABLE_MOUSE);
	ps2_stopstreaming(ps2_dev.ser_dev);
	nvec_unregister_notifier(nvec, &ps2_dev.notifier);
	serio_unregister_port(ps2_dev.ser_dev);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static int nvec_mouse_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct nvec_chip *nvec = dev_get_drvdata(pdev->dev.parent);

	/* disable mouse */
	nvec_write_async(nvec, "\x06\xf4", 2);

	/* send cancel autoreceive */
	nvec_write_async(nvec, "\x06\x04", 2);
=======
#ifdef CONFIG_PM_SLEEP
static int nvec_mouse_suspend(struct device *dev)
{
	/* disable mouse */
	ps2_sendcommand(ps2_dev.ser_dev, DISABLE_MOUSE);

	/* send cancel autoreceive */
	ps2_stopstreaming(ps2_dev.ser_dev);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static int nvec_mouse_resume(struct platform_device *pdev)
{
	struct nvec_chip *nvec = dev_get_drvdata(pdev->dev.parent);

	ps2_startstreaming(ps2_dev.ser_dev);

	/* enable mouse */
	nvec_write_async(nvec, "\x06\xf5", 2);

	return 0;
}

static struct platform_driver nvec_mouse_driver = {
	.probe  = nvec_mouse_probe,
	.suspend = nvec_mouse_suspend,
	.resume = nvec_mouse_resume,
	.driver = {
		.name = "nvec-mouse",
		.owner = THIS_MODULE,
	},
};

static int __init nvec_mouse_init(void)
{
	return platform_driver_register(&nvec_mouse_driver);
}

module_init(nvec_mouse_init);

MODULE_DESCRIPTION("NVEC mouse driver");
MODULE_AUTHOR("Marc Dietrich <marvin24@gmx.de>");
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int nvec_mouse_resume(struct device *dev)
{
	/* start streaming */
	ps2_startstreaming(ps2_dev.ser_dev);

	/* enable mouse */
	ps2_sendcommand(ps2_dev.ser_dev, ENABLE_MOUSE);

	return 0;
}
#endif

static const SIMPLE_DEV_PM_OPS(nvec_mouse_pm_ops, nvec_mouse_suspend,
				nvec_mouse_resume);

static struct platform_driver nvec_mouse_driver = {
	.probe  = nvec_mouse_probe,
	.remove = nvec_mouse_remove,
	.driver = {
		.name = "nvec-mouse",
		.owner = THIS_MODULE,
		.pm = &nvec_mouse_pm_ops,
	},
};

module_platform_driver(nvec_mouse_driver);

MODULE_DESCRIPTION("NVEC mouse driver");
MODULE_AUTHOR("Marc Dietrich <marvin24@gmx.de>");
MODULE_ALIAS("platform:nvec-mouse");
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
