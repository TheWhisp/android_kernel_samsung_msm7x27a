/*
 *  Copyright (c) 2000-2001 Vojtech Pavlik
 *  Copyright (c) 2002 Russell King
 */

/*
 * Acorn RiscPC PS/2 keyboard controller driver for Linux/ARM
 */

/*
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
 *
 * Should you need to contact me, the author, you can do so either by
 * e-mail - mail your message to <vojtech@ucw.cz>, or by paper mail:
 * Vojtech Pavlik, Simunkova 1594, Prague 8, 182 00 Czech Republic
 */

#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/serio.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/slab.h>

<<<<<<< HEAD
#include <asm/irq.h>
#include <mach/hardware.h>
#include <asm/hardware/iomd.h>
#include <asm/system.h>
=======
#include <mach/hardware.h>
#include <asm/hardware/iomd.h>
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_AUTHOR("Vojtech Pavlik, Russell King");
MODULE_DESCRIPTION("Acorn RiscPC PS/2 keyboard controller driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:kart");

<<<<<<< HEAD
=======
struct rpckbd_data {
	int tx_irq;
	int rx_irq;
};

>>>>>>> refs/remotes/origin/cm-10.0
static int rpckbd_write(struct serio *port, unsigned char val)
{
	while (!(iomd_readb(IOMD_KCTRL) & (1 << 7)))
		cpu_relax();

	iomd_writeb(val, IOMD_KARTTX);

	return 0;
}

static irqreturn_t rpckbd_rx(int irq, void *dev_id)
{
	struct serio *port = dev_id;
	unsigned int byte;
	int handled = IRQ_NONE;

	while (iomd_readb(IOMD_KCTRL) & (1 << 5)) {
		byte = iomd_readb(IOMD_KARTRX);

		serio_interrupt(port, byte, 0);
		handled = IRQ_HANDLED;
	}
	return handled;
}

static irqreturn_t rpckbd_tx(int irq, void *dev_id)
{
	return IRQ_HANDLED;
}

static int rpckbd_open(struct serio *port)
{
<<<<<<< HEAD
=======
	struct rpckbd_data *rpckbd = port->port_data;

>>>>>>> refs/remotes/origin/cm-10.0
	/* Reset the keyboard state machine. */
	iomd_writeb(0, IOMD_KCTRL);
	iomd_writeb(8, IOMD_KCTRL);
	iomd_readb(IOMD_KARTRX);

<<<<<<< HEAD
	if (request_irq(IRQ_KEYBOARDRX, rpckbd_rx, 0, "rpckbd", port) != 0) {
=======
	if (request_irq(rpckbd->rx_irq, rpckbd_rx, 0, "rpckbd", port) != 0) {
>>>>>>> refs/remotes/origin/cm-10.0
		printk(KERN_ERR "rpckbd.c: Could not allocate keyboard receive IRQ\n");
		return -EBUSY;
	}

<<<<<<< HEAD
	if (request_irq(IRQ_KEYBOARDTX, rpckbd_tx, 0, "rpckbd", port) != 0) {
		printk(KERN_ERR "rpckbd.c: Could not allocate keyboard transmit IRQ\n");
		free_irq(IRQ_KEYBOARDRX, port);
=======
	if (request_irq(rpckbd->tx_irq, rpckbd_tx, 0, "rpckbd", port) != 0) {
		printk(KERN_ERR "rpckbd.c: Could not allocate keyboard transmit IRQ\n");
		free_irq(rpckbd->rx_irq, port);
>>>>>>> refs/remotes/origin/cm-10.0
		return -EBUSY;
	}

	return 0;
}

static void rpckbd_close(struct serio *port)
{
<<<<<<< HEAD
	free_irq(IRQ_KEYBOARDRX, port);
	free_irq(IRQ_KEYBOARDTX, port);
=======
	struct rpckbd_data *rpckbd = port->port_data;

	free_irq(rpckbd->rx_irq, port);
	free_irq(rpckbd->tx_irq, port);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Allocate and initialize serio structure for subsequent registration
 * with serio core.
 */
static int __devinit rpckbd_probe(struct platform_device *dev)
{
<<<<<<< HEAD
	struct serio *serio;

	serio = kzalloc(sizeof(struct serio), GFP_KERNEL);
	if (!serio)
		return -ENOMEM;
=======
	struct rpckbd_data *rpckbd;
	struct serio *serio;
	int tx_irq, rx_irq;

	rx_irq = platform_get_irq(dev, 0);
	if (rx_irq <= 0)
		return rx_irq < 0 ? rx_irq : -ENXIO;

	tx_irq = platform_get_irq(dev, 1);
	if (tx_irq <= 0)
		return tx_irq < 0 ? tx_irq : -ENXIO;

	serio = kzalloc(sizeof(struct serio), GFP_KERNEL);
	rpckbd = kzalloc(sizeof(*rpckbd), GFP_KERNEL);
	if (!serio || !rpckbd) {
		kfree(rpckbd);
		kfree(serio);
		return -ENOMEM;
	}

	rpckbd->rx_irq = rx_irq;
	rpckbd->tx_irq = tx_irq;
>>>>>>> refs/remotes/origin/cm-10.0

	serio->id.type		= SERIO_8042;
	serio->write		= rpckbd_write;
	serio->open		= rpckbd_open;
	serio->close		= rpckbd_close;
	serio->dev.parent	= &dev->dev;
<<<<<<< HEAD
=======
	serio->port_data	= rpckbd;
>>>>>>> refs/remotes/origin/cm-10.0
	strlcpy(serio->name, "RiscPC PS/2 kbd port", sizeof(serio->name));
	strlcpy(serio->phys, "rpckbd/serio0", sizeof(serio->phys));

	platform_set_drvdata(dev, serio);
	serio_register_port(serio);
	return 0;
}

static int __devexit rpckbd_remove(struct platform_device *dev)
{
	struct serio *serio = platform_get_drvdata(dev);
<<<<<<< HEAD
	serio_unregister_port(serio);
=======
	struct rpckbd_data *rpckbd = serio->port_data;

	serio_unregister_port(serio);
	kfree(rpckbd);

>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static struct platform_driver rpckbd_driver = {
	.probe		= rpckbd_probe,
	.remove		= __devexit_p(rpckbd_remove),
	.driver		= {
		.name	= "kart",
		.owner	= THIS_MODULE,
	},
};
<<<<<<< HEAD

static int __init rpckbd_init(void)
{
	return platform_driver_register(&rpckbd_driver);
}

static void __exit rpckbd_exit(void)
{
	platform_driver_unregister(&rpckbd_driver);
}

module_init(rpckbd_init);
module_exit(rpckbd_exit);
=======
module_platform_driver(rpckbd_driver);
>>>>>>> refs/remotes/origin/cm-10.0
