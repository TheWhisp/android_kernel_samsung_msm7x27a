<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
#include <linux/of_gpio.h>
>>>>>>> refs/remotes/origin/master
#include <linux/platform_device.h>
#include <linux/irq.h>
#include <media/rc-core.h>
#include <media/gpio-ir-recv.h>

#define GPIO_IR_DRIVER_NAME	"gpio-rc-recv"
#define GPIO_IR_DEVICE_NAME	"gpio_ir_recv"

struct gpio_rc_dev {
	struct rc_dev *rcdev;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int gpio_nr;
	bool active_low;
	int can_sleep;
};

static irqreturn_t gpio_ir_recv_irq(int irq, void *dev_id)
{
	struct gpio_rc_dev *gpio_dev = dev_id;
	unsigned int gval;
	int rc = 0;
	enum raw_event_type type = IR_SPACE;

	if (gpio_dev->can_sleep)
		gval = gpio_get_value_cansleep(gpio_dev->gpio_nr);
	else
		gval = gpio_get_value(gpio_dev->gpio_nr);
=======
	int gpio_nr;
=======
	unsigned int gpio_nr;
>>>>>>> refs/remotes/origin/cm-11.0
	bool active_low;
	int can_sleep;
};

#ifdef CONFIG_OF
/*
 * Translate OpenFirmware node properties into platform_data
 */
static int gpio_ir_recv_get_devtree_pdata(struct device *dev,
				  struct gpio_ir_recv_platform_data *pdata)
{
	struct device_node *np = dev->of_node;
	enum of_gpio_flags flags;
	int gpio;

	gpio = of_get_gpio_flags(np, 0, &flags);
	if (gpio < 0) {
		if (gpio != -EPROBE_DEFER)
			dev_err(dev, "Failed to get gpio flags (%d)\n", gpio);
		return gpio;
	}

	pdata->gpio_nr = gpio;
	pdata->active_low = (flags & OF_GPIO_ACTIVE_LOW);
	/* probe() takes care of map_name == NULL or allowed_protos == 0 */
	pdata->map_name = of_get_property(np, "linux,rc-map-name", NULL);
	pdata->allowed_protos = 0;

	return 0;
}

static struct of_device_id gpio_ir_recv_of_match[] = {
	{ .compatible = "gpio-ir-receiver", },
	{ },
};
MODULE_DEVICE_TABLE(of, gpio_ir_recv_of_match);

#else /* !CONFIG_OF */

#define gpio_ir_recv_get_devtree_pdata(dev, pdata)	(-ENOSYS)

#endif

static irqreturn_t gpio_ir_recv_irq(int irq, void *dev_id)
{
	struct gpio_rc_dev *gpio_dev = dev_id;
	unsigned int gval;
	int rc = 0;
	enum raw_event_type type = IR_SPACE;

<<<<<<< HEAD
	gval = gpio_get_value_cansleep(gpio_dev->gpio_nr);
>>>>>>> refs/remotes/origin/master
=======
	if (gpio_dev->can_sleep)
		gval = gpio_get_value_cansleep(gpio_dev->gpio_nr);
	else
		gval = gpio_get_value(gpio_dev->gpio_nr);
>>>>>>> refs/remotes/origin/cm-11.0

	if (gval < 0)
		goto err_get_value;

	if (gpio_dev->active_low)
		gval = !gval;

	if (gval == 1)
		type = IR_PULSE;

	rc = ir_raw_event_store_edge(gpio_dev->rcdev, type);
	if (rc < 0)
		goto err_get_value;

	ir_raw_event_handle(gpio_dev->rcdev);

err_get_value:
	return IRQ_HANDLED;
}

<<<<<<< HEAD
static int __devinit gpio_ir_recv_probe(struct platform_device *pdev)
=======
static int gpio_ir_recv_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct gpio_rc_dev *gpio_dev;
	struct rc_dev *rcdev;
	const struct gpio_ir_recv_platform_data *pdata =
					pdev->dev.platform_data;
	int rc;

<<<<<<< HEAD
=======
	if (pdev->dev.of_node) {
		struct gpio_ir_recv_platform_data *dtpdata =
			devm_kzalloc(&pdev->dev, sizeof(*dtpdata), GFP_KERNEL);
		if (!dtpdata)
			return -ENOMEM;
		rc = gpio_ir_recv_get_devtree_pdata(&pdev->dev, dtpdata);
		if (rc)
			return rc;
		pdata = dtpdata;
	}

>>>>>>> refs/remotes/origin/master
	if (!pdata)
		return -EINVAL;

	if (pdata->gpio_nr < 0)
		return -EINVAL;

	gpio_dev = kzalloc(sizeof(struct gpio_rc_dev), GFP_KERNEL);
	if (!gpio_dev)
		return -ENOMEM;

	rcdev = rc_allocate_device();
	if (!rcdev) {
		rc = -ENOMEM;
		goto err_allocate_device;
	}

<<<<<<< HEAD
	rcdev->driver_type = RC_DRIVER_IR_RAW;
	rcdev->allowed_protos = RC_TYPE_ALL;
	rcdev->input_name = GPIO_IR_DEVICE_NAME;
	rcdev->input_id.bustype = BUS_HOST;
	rcdev->driver_name = GPIO_IR_DRIVER_NAME;
	rcdev->map_name = RC_MAP_SAMSUNG_NECX;
<<<<<<< HEAD
=======
	rcdev->priv = gpio_dev;
	rcdev->driver_type = RC_DRIVER_IR_RAW;
	rcdev->input_name = GPIO_IR_DEVICE_NAME;
	rcdev->input_phys = GPIO_IR_DEVICE_NAME "/input0";
	rcdev->input_id.bustype = BUS_HOST;
	rcdev->input_id.vendor = 0x0001;
	rcdev->input_id.product = 0x0001;
	rcdev->input_id.version = 0x0100;
	rcdev->dev.parent = &pdev->dev;
	rcdev->driver_name = GPIO_IR_DRIVER_NAME;
	if (pdata->allowed_protos)
		rcdev->allowed_protos = pdata->allowed_protos;
	else
		rcdev->allowed_protos = RC_BIT_ALL;
	rcdev->map_name = pdata->map_name ?: RC_MAP_EMPTY;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	gpio_dev->rcdev = rcdev;
	gpio_dev->gpio_nr = pdata->gpio_nr;
	gpio_dev->active_low = pdata->active_low;

	rc = gpio_request(pdata->gpio_nr, "gpio-ir-recv");
	if (rc < 0)
		goto err_gpio_request;
<<<<<<< HEAD
<<<<<<< HEAD

	gpio_dev->can_sleep = gpio_cansleep(pdata->gpio_nr);

=======
>>>>>>> refs/remotes/origin/master
=======

	gpio_dev->can_sleep = gpio_cansleep(pdata->gpio_nr);

>>>>>>> refs/remotes/origin/cm-11.0
	rc  = gpio_direction_input(pdata->gpio_nr);
	if (rc < 0)
		goto err_gpio_direction_input;

	rc = rc_register_device(rcdev);
	if (rc < 0) {
		dev_err(&pdev->dev, "failed to register rc device\n");
		goto err_register_rc_device;
	}

	platform_set_drvdata(pdev, gpio_dev);

	rc = request_any_context_irq(gpio_to_irq(pdata->gpio_nr),
				gpio_ir_recv_irq,
			IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
					"gpio-ir-recv-irq", gpio_dev);
	if (rc < 0)
		goto err_request_irq;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	device_init_wakeup(&pdev->dev, pdata->can_wakeup);

	return 0;

err_request_irq:
	platform_set_drvdata(pdev, NULL);
	rc_unregister_device(rcdev);
=======
	return 0;

err_request_irq:
	rc_unregister_device(rcdev);
	rcdev = NULL;
>>>>>>> refs/remotes/origin/master
err_register_rc_device:
err_gpio_direction_input:
	gpio_free(pdata->gpio_nr);
err_gpio_request:
	rc_free_device(rcdev);
<<<<<<< HEAD
	rcdev = NULL;
=======
>>>>>>> refs/remotes/origin/master
err_allocate_device:
	kfree(gpio_dev);
	return rc;
}

<<<<<<< HEAD
static int __devexit gpio_ir_recv_remove(struct platform_device *pdev)
=======
static int gpio_ir_recv_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct gpio_rc_dev *gpio_dev = platform_get_drvdata(pdev);

	free_irq(gpio_to_irq(gpio_dev->gpio_nr), gpio_dev);
<<<<<<< HEAD
	platform_set_drvdata(pdev, NULL);
	rc_unregister_device(gpio_dev->rcdev);
	gpio_free(gpio_dev->gpio_nr);
	rc_free_device(gpio_dev->rcdev);
=======
	rc_unregister_device(gpio_dev->rcdev);
	gpio_free(gpio_dev->gpio_nr);
>>>>>>> refs/remotes/origin/master
	kfree(gpio_dev);
	return 0;
}

#ifdef CONFIG_PM
static int gpio_ir_recv_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct gpio_rc_dev *gpio_dev = platform_get_drvdata(pdev);

	if (device_may_wakeup(dev))
		enable_irq_wake(gpio_to_irq(gpio_dev->gpio_nr));
	else
		disable_irq(gpio_to_irq(gpio_dev->gpio_nr));

	return 0;
}

static int gpio_ir_recv_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct gpio_rc_dev *gpio_dev = platform_get_drvdata(pdev);

	if (device_may_wakeup(dev))
		disable_irq_wake(gpio_to_irq(gpio_dev->gpio_nr));
	else
		enable_irq(gpio_to_irq(gpio_dev->gpio_nr));

	return 0;
}

static const struct dev_pm_ops gpio_ir_recv_pm_ops = {
	.suspend        = gpio_ir_recv_suspend,
	.resume         = gpio_ir_recv_resume,
};
#endif

static struct platform_driver gpio_ir_recv_driver = {
	.probe  = gpio_ir_recv_probe,
<<<<<<< HEAD
	.remove = __devexit_p(gpio_ir_recv_remove),
	.driver = {
		.name   = GPIO_IR_DRIVER_NAME,
		.owner  = THIS_MODULE,
=======
	.remove = gpio_ir_recv_remove,
	.driver = {
		.name   = GPIO_IR_DRIVER_NAME,
		.owner  = THIS_MODULE,
		.of_match_table = of_match_ptr(gpio_ir_recv_of_match),
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PM
		.pm	= &gpio_ir_recv_pm_ops,
#endif
	},
};
<<<<<<< HEAD

static int __init gpio_ir_recv_init(void)
{
	return platform_driver_register(&gpio_ir_recv_driver);
}
module_init(gpio_ir_recv_init);

static void __exit gpio_ir_recv_exit(void)
{
	platform_driver_unregister(&gpio_ir_recv_driver);
}
module_exit(gpio_ir_recv_exit);
=======
module_platform_driver(gpio_ir_recv_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("GPIO IR Receiver driver");
MODULE_LICENSE("GPL v2");
