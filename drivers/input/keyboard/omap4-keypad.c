/*
 * OMAP4 Keypad Driver
 *
 * Copyright (C) 2010 Texas Instruments
 *
 * Author: Abraham Arce <x0066660@ti.com>
 * Initial Code: Syed Rafiuddin <rafiuddin.syed@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/errno.h>
#include <linux/io.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> refs/remotes/origin/master
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <plat/omap4-keypad.h>
=======
#include <linux/platform_data/omap4-keypad.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/platform_data/omap4-keypad.h>
>>>>>>> refs/remotes/origin/master

/* OMAP4 registers */
#define OMAP4_KBD_REVISION		0x00
#define OMAP4_KBD_SYSCONFIG		0x10
#define OMAP4_KBD_SYSSTATUS		0x14
#define OMAP4_KBD_IRQSTATUS		0x18
#define OMAP4_KBD_IRQENABLE		0x1C
#define OMAP4_KBD_WAKEUPENABLE		0x20
#define OMAP4_KBD_PENDING		0x24
#define OMAP4_KBD_CTRL			0x28
#define OMAP4_KBD_DEBOUNCINGTIME	0x2C
#define OMAP4_KBD_LONGKEYTIME		0x30
#define OMAP4_KBD_TIMEOUT		0x34
#define OMAP4_KBD_STATEMACHINE		0x38
#define OMAP4_KBD_ROWINPUTS		0x3C
#define OMAP4_KBD_COLUMNOUTPUTS		0x40
#define OMAP4_KBD_FULLCODE31_0		0x44
#define OMAP4_KBD_FULLCODE63_32		0x48

/* OMAP4 bit definitions */
<<<<<<< HEAD
#define OMAP4_DEF_IRQENABLE_EVENTEN	(1 << 0)
#define OMAP4_DEF_IRQENABLE_LONGKEY	(1 << 1)
#define OMAP4_DEF_IRQENABLE_TIMEOUTEN	(1 << 2)
#define OMAP4_DEF_WUP_EVENT_ENA		(1 << 0)
#define OMAP4_DEF_WUP_LONG_KEY_ENA	(1 << 1)
#define OMAP4_DEF_CTRL_NOSOFTMODE	(1 << 1)
#define OMAP4_DEF_CTRLPTVVALUE		(1 << 2)
#define OMAP4_DEF_CTRLPTV		(1 << 1)

/* OMAP4 values */
#define OMAP4_VAL_IRQDISABLE		0x00
#define OMAP4_VAL_DEBOUNCINGTIME	0x07
#define OMAP4_VAL_FUNCTIONALCFG		0x1E

#define OMAP4_MASK_IRQSTATUSDISABLE	0xFFFF
=======
#define OMAP4_DEF_IRQENABLE_EVENTEN	BIT(0)
#define OMAP4_DEF_IRQENABLE_LONGKEY	BIT(1)
#define OMAP4_DEF_WUP_EVENT_ENA		BIT(0)
#define OMAP4_DEF_WUP_LONG_KEY_ENA	BIT(1)
#define OMAP4_DEF_CTRL_NOSOFTMODE	BIT(1)
#define OMAP4_DEF_CTRL_PTV_SHIFT	2

/* OMAP4 values */
#define OMAP4_VAL_IRQDISABLE		0x0
#define OMAP4_VAL_DEBOUNCINGTIME	0x7
#define OMAP4_VAL_PVT			0x7

enum {
	KBD_REVISION_OMAP4 = 0,
	KBD_REVISION_OMAP5,
};
>>>>>>> refs/remotes/origin/master

struct omap4_keypad {
	struct input_dev *input;

	void __iomem *base;
<<<<<<< HEAD
	int irq;

	unsigned int rows;
	unsigned int cols;
	unsigned int row_shift;
	unsigned char key_state[8];
	unsigned short keymap[];
};

/* Interrupt handler */
static irqreturn_t omap4_keypad_interrupt(int irq, void *dev_id)
=======
	bool irq_wake_enabled;
	unsigned int irq;

	unsigned int rows;
	unsigned int cols;
	u32 reg_offset;
	u32 irqreg_offset;
	unsigned int row_shift;
	bool no_autorepeat;
	unsigned char key_state[8];
	unsigned short *keymap;
};

static int kbd_readl(struct omap4_keypad *keypad_data, u32 offset)
{
	return __raw_readl(keypad_data->base +
				keypad_data->reg_offset + offset);
}

static void kbd_writel(struct omap4_keypad *keypad_data, u32 offset, u32 value)
{
	__raw_writel(value,
		     keypad_data->base + keypad_data->reg_offset + offset);
}

static int kbd_read_irqreg(struct omap4_keypad *keypad_data, u32 offset)
{
	return __raw_readl(keypad_data->base +
				keypad_data->irqreg_offset + offset);
}

static void kbd_write_irqreg(struct omap4_keypad *keypad_data,
			     u32 offset, u32 value)
{
	__raw_writel(value,
		     keypad_data->base + keypad_data->irqreg_offset + offset);
}


/* Interrupt handlers */
static irqreturn_t omap4_keypad_irq_handler(int irq, void *dev_id)
{
	struct omap4_keypad *keypad_data = dev_id;

	if (kbd_read_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS)) {
		/* Disable interrupts */
		kbd_write_irqreg(keypad_data, OMAP4_KBD_IRQENABLE,
				 OMAP4_VAL_IRQDISABLE);
		return IRQ_WAKE_THREAD;
	}

	return IRQ_NONE;
}

static irqreturn_t omap4_keypad_irq_thread_fn(int irq, void *dev_id)
>>>>>>> refs/remotes/origin/master
{
	struct omap4_keypad *keypad_data = dev_id;
	struct input_dev *input_dev = keypad_data->input;
	unsigned char key_state[ARRAY_SIZE(keypad_data->key_state)];
	unsigned int col, row, code, changed;
	u32 *new_state = (u32 *) key_state;

<<<<<<< HEAD
	/* Disable interrupts */
	__raw_writel(OMAP4_VAL_IRQDISABLE,
		     keypad_data->base + OMAP4_KBD_IRQENABLE);

	*new_state = __raw_readl(keypad_data->base + OMAP4_KBD_FULLCODE31_0);
	*(new_state + 1) = __raw_readl(keypad_data->base
						+ OMAP4_KBD_FULLCODE63_32);
=======
	*new_state = kbd_readl(keypad_data, OMAP4_KBD_FULLCODE31_0);
	*(new_state + 1) = kbd_readl(keypad_data, OMAP4_KBD_FULLCODE63_32);
>>>>>>> refs/remotes/origin/master

	for (row = 0; row < keypad_data->rows; row++) {
		changed = key_state[row] ^ keypad_data->key_state[row];
		if (!changed)
			continue;

		for (col = 0; col < keypad_data->cols; col++) {
			if (changed & (1 << col)) {
				code = MATRIX_SCAN_CODE(row, col,
						keypad_data->row_shift);
				input_event(input_dev, EV_MSC, MSC_SCAN, code);
				input_report_key(input_dev,
						 keypad_data->keymap[code],
						 key_state[row] & (1 << col));
			}
		}
	}

	input_sync(input_dev);

	memcpy(keypad_data->key_state, key_state,
		sizeof(keypad_data->key_state));

	/* clear pending interrupts */
<<<<<<< HEAD
	__raw_writel(__raw_readl(keypad_data->base + OMAP4_KBD_IRQSTATUS),
			keypad_data->base + OMAP4_KBD_IRQSTATUS);

	/* enable interrupts */
	__raw_writel(OMAP4_DEF_IRQENABLE_EVENTEN | OMAP4_DEF_IRQENABLE_LONGKEY,
			keypad_data->base + OMAP4_KBD_IRQENABLE);
=======
	kbd_write_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS,
			 kbd_read_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS));

	/* enable interrupts */
	kbd_write_irqreg(keypad_data, OMAP4_KBD_IRQENABLE,
		OMAP4_DEF_IRQENABLE_EVENTEN |
				OMAP4_DEF_IRQENABLE_LONGKEY);
>>>>>>> refs/remotes/origin/master

	return IRQ_HANDLED;
}

static int omap4_keypad_open(struct input_dev *input)
{
	struct omap4_keypad *keypad_data = input_get_drvdata(input);

	pm_runtime_get_sync(input->dev.parent);

	disable_irq(keypad_data->irq);

<<<<<<< HEAD
	__raw_writel(OMAP4_VAL_FUNCTIONALCFG,
			keypad_data->base + OMAP4_KBD_CTRL);
	__raw_writel(OMAP4_VAL_DEBOUNCINGTIME,
			keypad_data->base + OMAP4_KBD_DEBOUNCINGTIME);
	__raw_writel(OMAP4_VAL_IRQDISABLE,
			keypad_data->base + OMAP4_KBD_IRQSTATUS);
	__raw_writel(OMAP4_DEF_IRQENABLE_EVENTEN | OMAP4_DEF_IRQENABLE_LONGKEY,
			keypad_data->base + OMAP4_KBD_IRQENABLE);
	__raw_writel(OMAP4_DEF_WUP_EVENT_ENA | OMAP4_DEF_WUP_LONG_KEY_ENA,
			keypad_data->base + OMAP4_KBD_WAKEUPENABLE);
=======
	kbd_writel(keypad_data, OMAP4_KBD_CTRL,
			OMAP4_DEF_CTRL_NOSOFTMODE |
			(OMAP4_VAL_PVT << OMAP4_DEF_CTRL_PTV_SHIFT));
	kbd_writel(keypad_data, OMAP4_KBD_DEBOUNCINGTIME,
			OMAP4_VAL_DEBOUNCINGTIME);
	/* clear pending interrupts */
	kbd_write_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS,
			 kbd_read_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS));
	kbd_write_irqreg(keypad_data, OMAP4_KBD_IRQENABLE,
			OMAP4_DEF_IRQENABLE_EVENTEN |
				OMAP4_DEF_IRQENABLE_LONGKEY);
	kbd_writel(keypad_data, OMAP4_KBD_WAKEUPENABLE,
			OMAP4_DEF_WUP_EVENT_ENA | OMAP4_DEF_WUP_LONG_KEY_ENA);
>>>>>>> refs/remotes/origin/master

	enable_irq(keypad_data->irq);

	return 0;
}

static void omap4_keypad_close(struct input_dev *input)
{
	struct omap4_keypad *keypad_data = input_get_drvdata(input);

	disable_irq(keypad_data->irq);

	/* Disable interrupts */
<<<<<<< HEAD
	__raw_writel(OMAP4_VAL_IRQDISABLE,
		     keypad_data->base + OMAP4_KBD_IRQENABLE);

	/* clear pending interrupts */
	__raw_writel(__raw_readl(keypad_data->base + OMAP4_KBD_IRQSTATUS),
			keypad_data->base + OMAP4_KBD_IRQSTATUS);
=======
	kbd_write_irqreg(keypad_data, OMAP4_KBD_IRQENABLE,
			 OMAP4_VAL_IRQDISABLE);

	/* clear pending interrupts */
	kbd_write_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS,
			 kbd_read_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS));
>>>>>>> refs/remotes/origin/master

	enable_irq(keypad_data->irq);

	pm_runtime_put_sync(input->dev.parent);
}

<<<<<<< HEAD
static int __devinit omap4_keypad_probe(struct platform_device *pdev)
{
	const struct omap4_keypad_platform_data *pdata;
	struct omap4_keypad *keypad_data;
	struct input_dev *input_dev;
	struct resource *res;
	resource_size_t size;
	unsigned int row_shift, max_keys;
	int irq;
	int error;

	/* platform data */
	pdata = pdev->dev.platform_data;
	if (!pdata) {
		dev_err(&pdev->dev, "no platform data defined\n");
		return -EINVAL;
	}

=======
#ifdef CONFIG_OF
static int omap4_keypad_parse_dt(struct device *dev,
				 struct omap4_keypad *keypad_data)
{
	struct device_node *np = dev->of_node;
	int err;

	err = matrix_keypad_parse_of_params(dev, &keypad_data->rows,
					    &keypad_data->cols);
	if (err)
		return err;

	if (of_get_property(np, "linux,input-no-autorepeat", NULL))
		keypad_data->no_autorepeat = true;

	return 0;
}
#else
static inline int omap4_keypad_parse_dt(struct device *dev,
					struct omap4_keypad *keypad_data)
{
	return -ENOSYS;
}
#endif

static int omap4_keypad_probe(struct platform_device *pdev)
{
	const struct omap4_keypad_platform_data *pdata =
				dev_get_platdata(&pdev->dev);
	const struct matrix_keymap_data *keymap_data =
				pdata ? pdata->keymap_data : NULL;
	struct omap4_keypad *keypad_data;
	struct input_dev *input_dev;
	struct resource *res;
	unsigned int max_keys;
	int rev;
	int irq;
	int error;

>>>>>>> refs/remotes/origin/master
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "no base address specified\n");
		return -EINVAL;
	}

	irq = platform_get_irq(pdev, 0);
	if (!irq) {
		dev_err(&pdev->dev, "no keyboard irq assigned\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	if (!pdata->keymap_data) {
		dev_err(&pdev->dev, "no keymap data defined\n");
		return -EINVAL;
	}

	row_shift = get_count_order(pdata->cols);
	max_keys = pdata->rows << row_shift;

	keypad_data = kzalloc(sizeof(struct omap4_keypad) +
				max_keys * sizeof(keypad_data->keymap[0]),
			      GFP_KERNEL);
=======
	keypad_data = kzalloc(sizeof(struct omap4_keypad), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!keypad_data) {
		dev_err(&pdev->dev, "keypad_data memory allocation failed\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	size = resource_size(res);

	res = request_mem_region(res->start, size, pdev->name);
=======
	keypad_data->irq = irq;

	if (pdata) {
		keypad_data->rows = pdata->rows;
		keypad_data->cols = pdata->cols;
	} else {
		error = omap4_keypad_parse_dt(&pdev->dev, keypad_data);
		if (error)
			return error;
	}

	res = request_mem_region(res->start, resource_size(res), pdev->name);
>>>>>>> refs/remotes/origin/master
	if (!res) {
		dev_err(&pdev->dev, "can't request mem region\n");
		error = -EBUSY;
		goto err_free_keypad;
	}

	keypad_data->base = ioremap(res->start, resource_size(res));
	if (!keypad_data->base) {
		dev_err(&pdev->dev, "can't ioremap mem resource\n");
		error = -ENOMEM;
		goto err_release_mem;
	}

<<<<<<< HEAD
	keypad_data->irq = irq;
	keypad_data->row_shift = row_shift;
	keypad_data->rows = pdata->rows;
	keypad_data->cols = pdata->cols;
=======

	/*
	 * Enable clocks for the keypad module so that we can read
	 * revision register.
	 */
	pm_runtime_enable(&pdev->dev);
	error = pm_runtime_get_sync(&pdev->dev);
	if (error) {
		dev_err(&pdev->dev, "pm_runtime_get_sync() failed\n");
		goto err_unmap;
	}
	rev = __raw_readl(keypad_data->base + OMAP4_KBD_REVISION);
	rev &= 0x03 << 30;
	rev >>= 30;
	switch (rev) {
	case KBD_REVISION_OMAP4:
		keypad_data->reg_offset = 0x00;
		keypad_data->irqreg_offset = 0x00;
		break;
	case KBD_REVISION_OMAP5:
		keypad_data->reg_offset = 0x10;
		keypad_data->irqreg_offset = 0x0c;
		break;
	default:
		dev_err(&pdev->dev,
			"Keypad reports unsupported revision %d", rev);
		error = -EINVAL;
		goto err_pm_put_sync;
	}
>>>>>>> refs/remotes/origin/master

	/* input device allocation */
	keypad_data->input = input_dev = input_allocate_device();
	if (!input_dev) {
		error = -ENOMEM;
<<<<<<< HEAD
		goto err_unmap;
=======
		goto err_pm_put_sync;
>>>>>>> refs/remotes/origin/master
	}

	input_dev->name = pdev->name;
	input_dev->dev.parent = &pdev->dev;
	input_dev->id.bustype = BUS_HOST;
	input_dev->id.vendor = 0x0001;
	input_dev->id.product = 0x0001;
	input_dev->id.version = 0x0001;

	input_dev->open = omap4_keypad_open;
	input_dev->close = omap4_keypad_close;

<<<<<<< HEAD
	input_dev->keycode	= keypad_data->keymap;
	input_dev->keycodesize	= sizeof(keypad_data->keymap[0]);
	input_dev->keycodemax	= max_keys;

	__set_bit(EV_KEY, input_dev->evbit);
	__set_bit(EV_REP, input_dev->evbit);

	input_set_capability(input_dev, EV_MSC, MSC_SCAN);

	input_set_drvdata(input_dev, keypad_data);

	matrix_keypad_build_keymap(pdata->keymap_data, row_shift,
			input_dev->keycode, input_dev->keybit);

	error = request_irq(keypad_data->irq, omap4_keypad_interrupt,
			     IRQF_TRIGGER_RISING,
			     "omap4-keypad", keypad_data);
=======
	input_set_capability(input_dev, EV_MSC, MSC_SCAN);
	if (!keypad_data->no_autorepeat)
		__set_bit(EV_REP, input_dev->evbit);

	input_set_drvdata(input_dev, keypad_data);

	keypad_data->row_shift = get_count_order(keypad_data->cols);
	max_keys = keypad_data->rows << keypad_data->row_shift;
	keypad_data->keymap = kzalloc(max_keys * sizeof(keypad_data->keymap[0]),
				      GFP_KERNEL);
	if (!keypad_data->keymap) {
		dev_err(&pdev->dev, "Not enough memory for keymap\n");
		error = -ENOMEM;
		goto err_free_input;
	}

	error = matrix_keypad_build_keymap(keymap_data, NULL,
					   keypad_data->rows, keypad_data->cols,
					   keypad_data->keymap, input_dev);
	if (error) {
		dev_err(&pdev->dev, "failed to build keymap\n");
		goto err_free_keymap;
	}

	error = request_threaded_irq(keypad_data->irq, omap4_keypad_irq_handler,
				     omap4_keypad_irq_thread_fn, 0,
				     "omap4-keypad", keypad_data);
>>>>>>> refs/remotes/origin/master
	if (error) {
		dev_err(&pdev->dev, "failed to register interrupt\n");
		goto err_free_input;
	}

<<<<<<< HEAD
	pm_runtime_enable(&pdev->dev);
=======
	device_init_wakeup(&pdev->dev, true);
	pm_runtime_put_sync(&pdev->dev);
>>>>>>> refs/remotes/origin/master

	error = input_register_device(keypad_data->input);
	if (error < 0) {
		dev_err(&pdev->dev, "failed to register input device\n");
		goto err_pm_disable;
	}

	platform_set_drvdata(pdev, keypad_data);
	return 0;

err_pm_disable:
	pm_runtime_disable(&pdev->dev);
<<<<<<< HEAD
	free_irq(keypad_data->irq, keypad_data);
err_free_input:
	input_free_device(input_dev);
err_unmap:
	iounmap(keypad_data->base);
err_release_mem:
	release_mem_region(res->start, size);
=======
	device_init_wakeup(&pdev->dev, false);
	free_irq(keypad_data->irq, keypad_data);
err_free_keymap:
	kfree(keypad_data->keymap);
err_free_input:
	input_free_device(input_dev);
err_pm_put_sync:
	pm_runtime_put_sync(&pdev->dev);
err_unmap:
	iounmap(keypad_data->base);
err_release_mem:
	release_mem_region(res->start, resource_size(res));
>>>>>>> refs/remotes/origin/master
err_free_keypad:
	kfree(keypad_data);
	return error;
}

<<<<<<< HEAD
static int __devexit omap4_keypad_remove(struct platform_device *pdev)
=======
static int omap4_keypad_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct omap4_keypad *keypad_data = platform_get_drvdata(pdev);
	struct resource *res;

	free_irq(keypad_data->irq, keypad_data);

	pm_runtime_disable(&pdev->dev);

<<<<<<< HEAD
=======
	device_init_wakeup(&pdev->dev, false);

>>>>>>> refs/remotes/origin/master
	input_unregister_device(keypad_data->input);

	iounmap(keypad_data->base);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	release_mem_region(res->start, resource_size(res));

<<<<<<< HEAD
	kfree(keypad_data);
	platform_set_drvdata(pdev, NULL);
=======
	kfree(keypad_data->keymap);
	kfree(keypad_data);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static struct platform_driver omap4_keypad_driver = {
	.probe		= omap4_keypad_probe,
	.remove		= __devexit_p(omap4_keypad_remove),
	.driver		= {
		.name	= "omap4-keypad",
		.owner	= THIS_MODULE,
	},
};
<<<<<<< HEAD

static int __init omap4_keypad_init(void)
{
	return platform_driver_register(&omap4_keypad_driver);
}
module_init(omap4_keypad_init);

static void __exit omap4_keypad_exit(void)
{
	platform_driver_unregister(&omap4_keypad_driver);
}
module_exit(omap4_keypad_exit);
=======
module_platform_driver(omap4_keypad_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_OF
static const struct of_device_id omap_keypad_dt_match[] = {
	{ .compatible = "ti,omap4-keypad" },
	{},
};
MODULE_DEVICE_TABLE(of, omap_keypad_dt_match);
#endif

#ifdef CONFIG_PM_SLEEP
static int omap4_keypad_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct omap4_keypad *keypad_data = platform_get_drvdata(pdev);
	int error;

	if (device_may_wakeup(&pdev->dev)) {
		error = enable_irq_wake(keypad_data->irq);
		if (!error)
			keypad_data->irq_wake_enabled = true;
	}

	return 0;
}

static int omap4_keypad_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct omap4_keypad *keypad_data = platform_get_drvdata(pdev);

	if (device_may_wakeup(&pdev->dev) && keypad_data->irq_wake_enabled) {
		disable_irq_wake(keypad_data->irq);
		keypad_data->irq_wake_enabled = false;
	}

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(omap4_keypad_pm_ops,
			 omap4_keypad_suspend, omap4_keypad_resume);

static struct platform_driver omap4_keypad_driver = {
	.probe		= omap4_keypad_probe,
	.remove		= omap4_keypad_remove,
	.driver		= {
		.name	= "omap4-keypad",
		.owner	= THIS_MODULE,
		.pm	= &omap4_keypad_pm_ops,
		.of_match_table = of_match_ptr(omap_keypad_dt_match),
	},
};
module_platform_driver(omap4_keypad_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("OMAP4 Keypad Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:omap4-keypad");
