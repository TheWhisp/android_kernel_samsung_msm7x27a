/*
 * Copyright (c) 2008-2009 Nuvoton technology corporation.
 *
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/slab.h>

<<<<<<< HEAD
#include <mach/w90p910_keypad.h>
=======
#include <linux/platform_data/keypad-w90p910.h>
>>>>>>> refs/remotes/origin/master

/* Keypad Interface Control Registers */
#define KPI_CONF		0x00
#define KPI_3KCONF		0x04
#define KPI_LPCONF		0x08
#define KPI_STATUS		0x0C

#define IS1KEY			(0x01 << 16)
#define INTTR			(0x01 << 21)
#define KEY0R			(0x0f << 3)
#define KEY0C			0x07
#define DEBOUNCE_BIT		0x08
#define KSIZE0			(0x01 << 16)
#define KSIZE1			(0x01 << 17)
#define KPSEL			(0x01 << 19)
#define ENKP			(0x01 << 18)

#define KGET_RAW(n)		(((n) & KEY0R) >> 3)
#define KGET_COLUMN(n)		((n) & KEY0C)

<<<<<<< HEAD
#define W90P910_MAX_KEY_NUM	(8 * 8)
=======
#define W90P910_NUM_ROWS	8
#define W90P910_NUM_COLS	8
>>>>>>> refs/remotes/origin/master
#define W90P910_ROW_SHIFT	3

struct w90p910_keypad {
	const struct w90p910_keypad_platform_data *pdata;
	struct clk *clk;
	struct input_dev *input_dev;
	void __iomem *mmio_base;
	int irq;
<<<<<<< HEAD
	unsigned short keymap[W90P910_MAX_KEY_NUM];
=======
	unsigned short keymap[W90P910_NUM_ROWS * W90P910_NUM_COLS];
>>>>>>> refs/remotes/origin/master
};

static void w90p910_keypad_scan_matrix(struct w90p910_keypad *keypad,
							unsigned int status)
{
	struct input_dev *input_dev = keypad->input_dev;
	unsigned int row = KGET_RAW(status);
	unsigned int col = KGET_COLUMN(status);
	unsigned int code = MATRIX_SCAN_CODE(row, col, W90P910_ROW_SHIFT);
	unsigned int key = keypad->keymap[code];

	input_event(input_dev, EV_MSC, MSC_SCAN, code);
	input_report_key(input_dev, key, 1);
	input_sync(input_dev);

	input_event(input_dev, EV_MSC, MSC_SCAN, code);
	input_report_key(input_dev, key, 0);
	input_sync(input_dev);
}

static irqreturn_t w90p910_keypad_irq_handler(int irq, void *dev_id)
{
	struct w90p910_keypad *keypad = dev_id;
	unsigned int  kstatus, val;

	kstatus = __raw_readl(keypad->mmio_base + KPI_STATUS);

	val = INTTR | IS1KEY;

	if (kstatus & val)
		w90p910_keypad_scan_matrix(keypad, kstatus);

	return IRQ_HANDLED;
}

static int w90p910_keypad_open(struct input_dev *dev)
{
	struct w90p910_keypad *keypad = input_get_drvdata(dev);
	const struct w90p910_keypad_platform_data *pdata = keypad->pdata;
	unsigned int val, config;

	/* Enable unit clock */
	clk_enable(keypad->clk);

	val = __raw_readl(keypad->mmio_base + KPI_CONF);
	val |= (KPSEL | ENKP);
	val &= ~(KSIZE0 | KSIZE1);

	config = pdata->prescale | (pdata->debounce << DEBOUNCE_BIT);

	val |= config;

	__raw_writel(val, keypad->mmio_base + KPI_CONF);

	return 0;
}

static void w90p910_keypad_close(struct input_dev *dev)
{
	struct w90p910_keypad *keypad = input_get_drvdata(dev);

	/* Disable clock unit */
	clk_disable(keypad->clk);
}

<<<<<<< HEAD
static int __devinit w90p910_keypad_probe(struct platform_device *pdev)
=======
static int w90p910_keypad_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	const struct w90p910_keypad_platform_data *pdata =
						pdev->dev.platform_data;
	const struct matrix_keymap_data *keymap_data;
	struct w90p910_keypad *keypad;
	struct input_dev *input_dev;
	struct resource *res;
	int irq;
	int error;

	if (!pdata) {
		dev_err(&pdev->dev, "no platform data defined\n");
		return -EINVAL;
	}

	keymap_data = pdata->keymap_data;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(&pdev->dev, "failed to get keypad irq\n");
		return -ENXIO;
	}

	keypad = kzalloc(sizeof(struct w90p910_keypad), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!keypad || !input_dev) {
		dev_err(&pdev->dev, "failed to allocate driver data\n");
		error = -ENOMEM;
		goto failed_free;
	}

	keypad->pdata = pdata;
	keypad->input_dev = input_dev;
	keypad->irq = irq;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		dev_err(&pdev->dev, "failed to get I/O memory\n");
		error = -ENXIO;
		goto failed_free;
	}

	res = request_mem_region(res->start, resource_size(res), pdev->name);
	if (res == NULL) {
		dev_err(&pdev->dev, "failed to request I/O memory\n");
		error = -EBUSY;
		goto failed_free;
	}

	keypad->mmio_base = ioremap(res->start, resource_size(res));
	if (keypad->mmio_base == NULL) {
		dev_err(&pdev->dev, "failed to remap I/O memory\n");
		error = -ENXIO;
		goto failed_free_res;
	}

	keypad->clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(keypad->clk)) {
		dev_err(&pdev->dev, "failed to get keypad clock\n");
		error = PTR_ERR(keypad->clk);
		goto failed_free_io;
	}

	/* set multi-function pin for w90p910 kpi. */
	mfp_set_groupi(&pdev->dev);

	input_dev->name = pdev->name;
	input_dev->id.bustype = BUS_HOST;
	input_dev->open = w90p910_keypad_open;
	input_dev->close = w90p910_keypad_close;
	input_dev->dev.parent = &pdev->dev;

<<<<<<< HEAD
	input_dev->keycode = keypad->keymap;
	input_dev->keycodesize = sizeof(keypad->keymap[0]);
	input_dev->keycodemax = ARRAY_SIZE(keypad->keymap);

	input_set_drvdata(input_dev, keypad);

	input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP);
	input_set_capability(input_dev, EV_MSC, MSC_SCAN);

	matrix_keypad_build_keymap(keymap_data, W90P910_ROW_SHIFT,
				   input_dev->keycode, input_dev->keybit);

	error = request_irq(keypad->irq, w90p910_keypad_irq_handler,
<<<<<<< HEAD
			    IRQF_DISABLED, pdev->name, keypad);
=======
			    0, pdev->name, keypad);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	error = matrix_keypad_build_keymap(keymap_data, NULL,
					   W90P910_NUM_ROWS, W90P910_NUM_COLS,
					   keypad->keymap, input_dev);
	if (error) {
		dev_err(&pdev->dev, "failed to build keymap\n");
		goto failed_put_clk;
	}

	error = request_irq(keypad->irq, w90p910_keypad_irq_handler,
			    0, pdev->name, keypad);
>>>>>>> refs/remotes/origin/master
	if (error) {
		dev_err(&pdev->dev, "failed to request IRQ\n");
		goto failed_put_clk;
	}

<<<<<<< HEAD
=======
	__set_bit(EV_REP, input_dev->evbit);
	input_set_capability(input_dev, EV_MSC, MSC_SCAN);
	input_set_drvdata(input_dev, keypad);

>>>>>>> refs/remotes/origin/master
	/* Register the input device */
	error = input_register_device(input_dev);
	if (error) {
		dev_err(&pdev->dev, "failed to register input device\n");
		goto failed_free_irq;
	}

	platform_set_drvdata(pdev, keypad);
	return 0;

failed_free_irq:
<<<<<<< HEAD
	free_irq(irq, pdev);
=======
	free_irq(irq, keypad);
>>>>>>> refs/remotes/origin/master
failed_put_clk:
	clk_put(keypad->clk);
failed_free_io:
	iounmap(keypad->mmio_base);
failed_free_res:
	release_mem_region(res->start, resource_size(res));
failed_free:
	input_free_device(input_dev);
	kfree(keypad);
	return error;
}

<<<<<<< HEAD
static int __devexit w90p910_keypad_remove(struct platform_device *pdev)
=======
static int w90p910_keypad_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct w90p910_keypad *keypad = platform_get_drvdata(pdev);
	struct resource *res;

<<<<<<< HEAD
	free_irq(keypad->irq, pdev);
=======
	free_irq(keypad->irq, keypad);
>>>>>>> refs/remotes/origin/master

	clk_put(keypad->clk);

	input_unregister_device(keypad->input_dev);

	iounmap(keypad->mmio_base);
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	release_mem_region(res->start, resource_size(res));

<<<<<<< HEAD
	platform_set_drvdata(pdev, NULL);
=======
>>>>>>> refs/remotes/origin/master
	kfree(keypad);

	return 0;
}

static struct platform_driver w90p910_keypad_driver = {
	.probe		= w90p910_keypad_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(w90p910_keypad_remove),
=======
	.remove		= w90p910_keypad_remove,
>>>>>>> refs/remotes/origin/master
	.driver		= {
		.name	= "nuc900-kpi",
		.owner	= THIS_MODULE,
	},
};
<<<<<<< HEAD
<<<<<<< HEAD

static int __init w90p910_keypad_init(void)
{
	return platform_driver_register(&w90p910_keypad_driver);
}

static void __exit w90p910_keypad_exit(void)
{
	platform_driver_unregister(&w90p910_keypad_driver);
}

module_init(w90p910_keypad_init);
module_exit(w90p910_keypad_exit);
=======
module_platform_driver(w90p910_keypad_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_platform_driver(w90p910_keypad_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Wan ZongShun <mcuos.com@gmail.com>");
MODULE_DESCRIPTION("w90p910 keypad driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:nuc900-keypad");
