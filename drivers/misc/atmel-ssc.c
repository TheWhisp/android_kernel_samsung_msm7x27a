/*
 * Atmel SSC driver
 *
 * Copyright (C) 2007 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/platform_device.h>
#include <linux/list.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/spinlock.h>
#include <linux/atmel-ssc.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>

#include <linux/of.h>
>>>>>>> refs/remotes/origin/master

/* Serialize access to ssc_list and user count */
static DEFINE_SPINLOCK(user_lock);
static LIST_HEAD(ssc_list);

struct ssc_device *ssc_request(unsigned int ssc_num)
{
	int ssc_valid = 0;
	struct ssc_device *ssc;

	spin_lock(&user_lock);
	list_for_each_entry(ssc, &ssc_list, list) {
<<<<<<< HEAD
		if (ssc->pdev->id == ssc_num) {
=======
		if (ssc->pdev->dev.of_node) {
			if (of_alias_get_id(ssc->pdev->dev.of_node, "ssc")
				== ssc_num) {
				ssc_valid = 1;
				break;
			}
		} else if (ssc->pdev->id == ssc_num) {
>>>>>>> refs/remotes/origin/master
			ssc_valid = 1;
			break;
		}
	}

	if (!ssc_valid) {
		spin_unlock(&user_lock);
		pr_err("ssc: ssc%d platform device is missing\n", ssc_num);
		return ERR_PTR(-ENODEV);
	}

	if (ssc->user) {
		spin_unlock(&user_lock);
		dev_dbg(&ssc->pdev->dev, "module busy\n");
		return ERR_PTR(-EBUSY);
	}
	ssc->user++;
	spin_unlock(&user_lock);

<<<<<<< HEAD
	clk_enable(ssc->clk);
=======
	clk_prepare_enable(ssc->clk);
>>>>>>> refs/remotes/origin/master

	return ssc;
}
EXPORT_SYMBOL(ssc_request);

void ssc_free(struct ssc_device *ssc)
{
<<<<<<< HEAD
	spin_lock(&user_lock);
	if (ssc->user) {
		ssc->user--;
		clk_disable(ssc->clk);
	} else {
		dev_dbg(&ssc->pdev->dev, "device already free\n");
	}
	spin_unlock(&user_lock);
}
EXPORT_SYMBOL(ssc_free);

static int __init ssc_probe(struct platform_device *pdev)
{
	int retval = 0;
	struct resource *regs;
	struct ssc_device *ssc;

	ssc = kzalloc(sizeof(struct ssc_device), GFP_KERNEL);
	if (!ssc) {
		dev_dbg(&pdev->dev, "out of memory\n");
		retval = -ENOMEM;
		goto out;
	}

	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!regs) {
		dev_dbg(&pdev->dev, "no mmio resource defined\n");
		retval = -ENXIO;
		goto out_free;
	}

	ssc->clk = clk_get(&pdev->dev, "pclk");
	if (IS_ERR(ssc->clk)) {
		dev_dbg(&pdev->dev, "no pclk clock defined\n");
		retval = -ENXIO;
		goto out_free;
	}

	ssc->pdev = pdev;
<<<<<<< HEAD
	ssc->regs = ioremap(regs->start, regs->end - regs->start + 1);
=======
	ssc->regs = ioremap(regs->start, resource_size(regs));
>>>>>>> refs/remotes/origin/cm-10.0
	if (!ssc->regs) {
		dev_dbg(&pdev->dev, "ioremap failed\n");
		retval = -EINVAL;
		goto out_clk;
	}

	/* disable all interrupts */
	clk_enable(ssc->clk);
	ssc_writel(ssc->regs, IDR, ~0UL);
	ssc_readl(ssc->regs, SR);
	clk_disable(ssc->clk);
=======
	bool disable_clk = true;

	spin_lock(&user_lock);
	if (ssc->user)
		ssc->user--;
	else {
		disable_clk = false;
		dev_dbg(&ssc->pdev->dev, "device already free\n");
	}
	spin_unlock(&user_lock);

	if (disable_clk)
		clk_disable_unprepare(ssc->clk);
}
EXPORT_SYMBOL(ssc_free);

static struct atmel_ssc_platform_data at91rm9200_config = {
	.use_dma = 0,
};

static struct atmel_ssc_platform_data at91sam9g45_config = {
	.use_dma = 1,
};

static const struct platform_device_id atmel_ssc_devtypes[] = {
	{
		.name = "at91rm9200_ssc",
		.driver_data = (unsigned long) &at91rm9200_config,
	}, {
		.name = "at91sam9g45_ssc",
		.driver_data = (unsigned long) &at91sam9g45_config,
	}, {
		/* sentinel */
	}
};

#ifdef CONFIG_OF
static const struct of_device_id atmel_ssc_dt_ids[] = {
	{
		.compatible = "atmel,at91rm9200-ssc",
		.data = &at91rm9200_config,
	}, {
		.compatible = "atmel,at91sam9g45-ssc",
		.data = &at91sam9g45_config,
	}, {
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(of, atmel_ssc_dt_ids);
#endif

static inline const struct atmel_ssc_platform_data * __init
	atmel_ssc_get_driver_data(struct platform_device *pdev)
{
	if (pdev->dev.of_node) {
		const struct of_device_id *match;
		match = of_match_node(atmel_ssc_dt_ids, pdev->dev.of_node);
		if (match == NULL)
			return NULL;
		return match->data;
	}

	return (struct atmel_ssc_platform_data *)
		platform_get_device_id(pdev)->driver_data;
}

static int ssc_probe(struct platform_device *pdev)
{
	struct resource *regs;
	struct ssc_device *ssc;
	const struct atmel_ssc_platform_data *plat_dat;

	ssc = devm_kzalloc(&pdev->dev, sizeof(struct ssc_device), GFP_KERNEL);
	if (!ssc) {
		dev_dbg(&pdev->dev, "out of memory\n");
		return -ENOMEM;
	}

	ssc->pdev = pdev;

	plat_dat = atmel_ssc_get_driver_data(pdev);
	if (!plat_dat)
		return -ENODEV;
	ssc->pdata = (struct atmel_ssc_platform_data *)plat_dat;

	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	ssc->regs = devm_ioremap_resource(&pdev->dev, regs);
	if (IS_ERR(ssc->regs))
		return PTR_ERR(ssc->regs);

	ssc->phybase = regs->start;

	ssc->clk = devm_clk_get(&pdev->dev, "pclk");
	if (IS_ERR(ssc->clk)) {
		dev_dbg(&pdev->dev, "no pclk clock defined\n");
		return -ENXIO;
	}

	/* disable all interrupts */
	clk_prepare_enable(ssc->clk);
	ssc_writel(ssc->regs, IDR, -1);
	ssc_readl(ssc->regs, SR);
	clk_disable_unprepare(ssc->clk);
>>>>>>> refs/remotes/origin/master

	ssc->irq = platform_get_irq(pdev, 0);
	if (!ssc->irq) {
		dev_dbg(&pdev->dev, "could not get irq\n");
<<<<<<< HEAD
		retval = -ENXIO;
		goto out_unmap;
=======
		return -ENXIO;
>>>>>>> refs/remotes/origin/master
	}

	spin_lock(&user_lock);
	list_add_tail(&ssc->list, &ssc_list);
	spin_unlock(&user_lock);

	platform_set_drvdata(pdev, ssc);

	dev_info(&pdev->dev, "Atmel SSC device at 0x%p (irq %d)\n",
			ssc->regs, ssc->irq);

<<<<<<< HEAD
	goto out;

out_unmap:
	iounmap(ssc->regs);
out_clk:
	clk_put(ssc->clk);
out_free:
	kfree(ssc);
out:
	return retval;
}

static int __devexit ssc_remove(struct platform_device *pdev)
=======
	return 0;
}

static int ssc_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct ssc_device *ssc = platform_get_drvdata(pdev);

	spin_lock(&user_lock);
<<<<<<< HEAD
	iounmap(ssc->regs);
	clk_put(ssc->clk);
	list_del(&ssc->list);
	kfree(ssc);
=======
	list_del(&ssc->list);
>>>>>>> refs/remotes/origin/master
	spin_unlock(&user_lock);

	return 0;
}

static struct platform_driver ssc_driver = {
<<<<<<< HEAD
	.remove		= __devexit_p(ssc_remove),
	.driver		= {
		.name		= "ssc",
		.owner		= THIS_MODULE,
	},
};

static int __init ssc_init(void)
{
	return platform_driver_probe(&ssc_driver, ssc_probe);
}
module_init(ssc_init);

static void __exit ssc_exit(void)
{
	platform_driver_unregister(&ssc_driver);
}
module_exit(ssc_exit);
=======
	.driver		= {
		.name		= "ssc",
		.owner		= THIS_MODULE,
		.of_match_table	= of_match_ptr(atmel_ssc_dt_ids),
	},
	.id_table	= atmel_ssc_devtypes,
	.probe		= ssc_probe,
	.remove		= ssc_remove,
};
module_platform_driver(ssc_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Hans-Christian Egtvedt <hcegtvedt@atmel.com>");
MODULE_DESCRIPTION("SSC driver for Atmel AVR32 and AT91");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:ssc");
