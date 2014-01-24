/*
 * SuperH Mobile SDHI
 *
 * Copyright (C) 2009 Magnus Damm
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Based on "Compaq ASIC3 support":
 *
 * Copyright 2001 Compaq Computer Corporation.
 * Copyright 2004-2005 Phil Blundell
 * Copyright 2007-2008 OpenedHand Ltd.
 *
 * Authors: Phil Blundell <pb@handhelds.org>,
 *	    Samuel Ortiz <sameo@openedhand.com>
 *
 */

#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/of_device.h>
>>>>>>> refs/remotes/origin/master
#include <linux/platform_device.h>
#include <linux/mmc/host.h>
#include <linux/mmc/sh_mobile_sdhi.h>
#include <linux/mfd/tmio.h>
#include <linux/sh_dma.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/delay.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include "tmio_mmc.h"

struct sh_mobile_sdhi {
	struct clk *clk;
	struct tmio_mmc_data mmc_data;
	struct sh_dmae_slave param_tx;
	struct sh_dmae_slave param_rx;
	struct tmio_mmc_dma dma_priv;
};

static void sh_mobile_sdhi_set_pwr(struct platform_device *pdev, int state)
{
	struct sh_mobile_sdhi_info *p = pdev->dev.platform_data;

	if (p && p->set_pwr)
		p->set_pwr(pdev, state);
}

static int sh_mobile_sdhi_get_cd(struct platform_device *pdev)
{
	struct sh_mobile_sdhi_info *p = pdev->dev.platform_data;

	if (p && p->get_cd)
		return p->get_cd(pdev);
	else
		return -ENOSYS;
}

<<<<<<< HEAD
=======
=======
#include <linux/delay.h>

#include "tmio_mmc.h"

struct sh_mobile_sdhi_of_data {
	unsigned long tmio_flags;
};

static const struct sh_mobile_sdhi_of_data sh_mobile_sdhi_of_cfg[] = {
	{
		.tmio_flags = TMIO_MMC_HAS_IDLE_WAIT,
	},
};

struct sh_mobile_sdhi {
	struct clk *clk;
	struct tmio_mmc_data mmc_data;
	struct tmio_mmc_dma dma_priv;
};

static int sh_mobile_sdhi_clk_enable(struct platform_device *pdev, unsigned int *f)
{
	struct mmc_host *mmc = platform_get_drvdata(pdev);
	struct tmio_mmc_host *host = mmc_priv(mmc);
	struct sh_mobile_sdhi *priv = container_of(host->pdata, struct sh_mobile_sdhi, mmc_data);
	int ret = clk_enable(priv->clk);
	if (ret < 0)
		return ret;

	*f = clk_get_rate(priv->clk);
	return 0;
}

static void sh_mobile_sdhi_clk_disable(struct platform_device *pdev)
{
	struct mmc_host *mmc = platform_get_drvdata(pdev);
	struct tmio_mmc_host *host = mmc_priv(mmc);
	struct sh_mobile_sdhi *priv = container_of(host->pdata, struct sh_mobile_sdhi, mmc_data);
	clk_disable(priv->clk);
}

>>>>>>> refs/remotes/origin/master
static int sh_mobile_sdhi_wait_idle(struct tmio_mmc_host *host)
{
	int timeout = 1000;

	while (--timeout && !(sd_ctrl_read16(host, CTL_STATUS2) & (1 << 13)))
		udelay(1);

	if (!timeout) {
		dev_warn(host->pdata->dev, "timeout waiting for SD bus idle\n");
		return -EBUSY;
	}

	return 0;
}

static int sh_mobile_sdhi_write16_hook(struct tmio_mmc_host *host, int addr)
{
	switch (addr)
	{
	case CTL_SD_CMD:
	case CTL_STOP_INTERNAL_ACTION:
	case CTL_XFER_BLK_COUNT:
	case CTL_SD_CARD_CLK_CTL:
	case CTL_SD_XFER_LEN:
	case CTL_SD_MEM_CARD_OPT:
	case CTL_TRANSACTION_CTL:
	case CTL_DMA_ENABLE:
		return sh_mobile_sdhi_wait_idle(host);
	}

	return 0;
}

static void sh_mobile_sdhi_cd_wakeup(const struct platform_device *pdev)
{
<<<<<<< HEAD
	mmc_detect_change(dev_get_drvdata(&pdev->dev), msecs_to_jiffies(100));
=======
	mmc_detect_change(platform_get_drvdata(pdev), msecs_to_jiffies(100));
>>>>>>> refs/remotes/origin/master
}

static const struct sh_mobile_sdhi_ops sdhi_ops = {
	.cd_wakeup = sh_mobile_sdhi_cd_wakeup,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static int __devinit sh_mobile_sdhi_probe(struct platform_device *pdev)
{
=======
static const struct of_device_id sh_mobile_sdhi_of_match[] = {
	{ .compatible = "renesas,sdhi-shmobile" },
	{ .compatible = "renesas,sdhi-sh7372" },
	{ .compatible = "renesas,sdhi-sh73a0", .data = &sh_mobile_sdhi_of_cfg[0], },
	{ .compatible = "renesas,sdhi-r8a73a4", .data = &sh_mobile_sdhi_of_cfg[0], },
	{ .compatible = "renesas,sdhi-r8a7740", .data = &sh_mobile_sdhi_of_cfg[0], },
	{ .compatible = "renesas,sdhi-r8a7778", .data = &sh_mobile_sdhi_of_cfg[0], },
	{ .compatible = "renesas,sdhi-r8a7779", .data = &sh_mobile_sdhi_of_cfg[0], },
	{ .compatible = "renesas,sdhi-r8a7790", .data = &sh_mobile_sdhi_of_cfg[0], },
	{},
};
MODULE_DEVICE_TABLE(of, sh_mobile_sdhi_of_match);

static int sh_mobile_sdhi_probe(struct platform_device *pdev)
{
	const struct of_device_id *of_id =
		of_match_device(sh_mobile_sdhi_of_match, &pdev->dev);
>>>>>>> refs/remotes/origin/master
	struct sh_mobile_sdhi *priv;
	struct tmio_mmc_data *mmc_data;
	struct sh_mobile_sdhi_info *p = pdev->dev.platform_data;
	struct tmio_mmc_host *host;
<<<<<<< HEAD
	char clk_name[8];
<<<<<<< HEAD
	int i, irq, ret;
=======
	int irq, ret, i = 0;
	bool multiplexed_isr = true;
>>>>>>> refs/remotes/origin/cm-10.0

	priv = kzalloc(sizeof(struct sh_mobile_sdhi), GFP_KERNEL);
=======
	int irq, ret, i = 0;
	bool multiplexed_isr = true;
	struct tmio_mmc_dma *dma_priv;

	priv = devm_kzalloc(&pdev->dev, sizeof(struct sh_mobile_sdhi), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (priv == NULL) {
		dev_err(&pdev->dev, "kzalloc failed\n");
		return -ENOMEM;
	}

	mmc_data = &priv->mmc_data;
<<<<<<< HEAD
	p->pdata = mmc_data;

<<<<<<< HEAD
=======
	if (p->init) {
		ret = p->init(pdev, &sdhi_ops);
		if (ret)
			goto einit;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	snprintf(clk_name, sizeof(clk_name), "sdhi%d", pdev->id);
	priv->clk = clk_get(&pdev->dev, clk_name);
	if (IS_ERR(priv->clk)) {
		dev_err(&pdev->dev, "cannot get clock \"%s\"\n", clk_name);
		ret = PTR_ERR(priv->clk);
		goto eclkget;
	}

<<<<<<< HEAD
	clk_enable(priv->clk);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	mmc_data->hclk = clk_get_rate(priv->clk);
	mmc_data->set_pwr = sh_mobile_sdhi_set_pwr;
	mmc_data->get_cd = sh_mobile_sdhi_get_cd;
	mmc_data->capabilities = MMC_CAP_MMC_HIGHSPEED;
	if (p) {
		mmc_data->flags = p->tmio_flags;
<<<<<<< HEAD
		mmc_data->ocr_mask = p->tmio_ocr_mask;
		mmc_data->capabilities |= p->tmio_caps;
=======
		if (mmc_data->flags & TMIO_MMC_HAS_IDLE_WAIT)
			mmc_data->write16_hook = sh_mobile_sdhi_write16_hook;
		mmc_data->ocr_mask = p->tmio_ocr_mask;
		mmc_data->capabilities |= p->tmio_caps;
		mmc_data->cd_gpio = p->cd_gpio;
>>>>>>> refs/remotes/origin/cm-10.0

		if (p->dma_slave_tx > 0 && p->dma_slave_rx > 0) {
			priv->param_tx.slave_id = p->dma_slave_tx;
			priv->param_rx.slave_id = p->dma_slave_rx;
			priv->dma_priv.chan_priv_tx = &priv->param_tx;
			priv->dma_priv.chan_priv_rx = &priv->param_rx;
			priv->dma_priv.alignment_shift = 1; /* 2-byte alignment */
			mmc_data->dma = &priv->dma_priv;
		}
	}

=======
	dma_priv = &priv->dma_priv;

	if (p) {
		if (p->init) {
			ret = p->init(pdev, &sdhi_ops);
			if (ret)
				return ret;
		}
	}

	priv->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(priv->clk)) {
		ret = PTR_ERR(priv->clk);
		dev_err(&pdev->dev, "cannot get clock: %d\n", ret);
		goto eclkget;
	}

	mmc_data->clk_enable = sh_mobile_sdhi_clk_enable;
	mmc_data->clk_disable = sh_mobile_sdhi_clk_disable;
	mmc_data->capabilities = MMC_CAP_MMC_HIGHSPEED;
	mmc_data->write16_hook = sh_mobile_sdhi_write16_hook;
	if (p) {
		mmc_data->flags = p->tmio_flags;
		mmc_data->ocr_mask = p->tmio_ocr_mask;
		mmc_data->capabilities |= p->tmio_caps;
		mmc_data->capabilities2 |= p->tmio_caps2;
		mmc_data->cd_gpio = p->cd_gpio;

		if (p->dma_slave_tx > 0 && p->dma_slave_rx > 0) {
			/*
			 * Yes, we have to provide slave IDs twice to TMIO:
			 * once as a filter parameter and once for channel
			 * configuration as an explicit slave ID
			 */
			dma_priv->chan_priv_tx = (void *)p->dma_slave_tx;
			dma_priv->chan_priv_rx = (void *)p->dma_slave_rx;
			dma_priv->slave_id_tx = p->dma_slave_tx;
			dma_priv->slave_id_rx = p->dma_slave_rx;
		}
	}

	dma_priv->alignment_shift = 1; /* 2-byte alignment */
	dma_priv->filter = shdma_chan_filter;

	mmc_data->dma = dma_priv;

>>>>>>> refs/remotes/origin/master
	/*
	 * All SDHI blocks support 2-byte and larger block sizes in 4-bit
	 * bus width mode.
	 */
	mmc_data->flags |= TMIO_MMC_BLKSZ_2BYTES;

	/*
	 * All SDHI blocks support SDIO IRQ signalling.
	 */
	mmc_data->flags |= TMIO_MMC_SDIO_IRQ;

<<<<<<< HEAD
=======
	if (of_id && of_id->data) {
		const struct sh_mobile_sdhi_of_data *of_data = of_id->data;
		mmc_data->flags |= of_data->tmio_flags;
	}

>>>>>>> refs/remotes/origin/master
	ret = tmio_mmc_host_probe(&host, pdev, mmc_data);
	if (ret < 0)
		goto eprobe;

<<<<<<< HEAD
<<<<<<< HEAD
	for (i = 0; i < 3; i++) {
		irq = platform_get_irq(pdev, i);
		if (irq < 0) {
			if (i) {
				continue;
			} else {
				ret = irq;
				goto eirq;
			}
		}
		ret = request_irq(irq, tmio_mmc_irq, 0,
				  dev_name(&pdev->dev), host);
		if (ret) {
			while (i--) {
				irq = platform_get_irq(pdev, i);
				if (irq >= 0)
					free_irq(irq, host);
			}
			goto eirq;
		}
	}
	dev_info(&pdev->dev, "%s base at 0x%08lx clock rate %u MHz\n",
		 mmc_hostname(host->mmc), (unsigned long)
		 (platform_get_resource(pdev,IORESOURCE_MEM, 0)->start),
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Allow one or more specific (named) ISRs or
	 * one or more multiplexed (un-named) ISRs.
	 */

	irq = platform_get_irq_byname(pdev, SH_MOBILE_SDHI_IRQ_CARD_DETECT);
	if (irq >= 0) {
		multiplexed_isr = false;
<<<<<<< HEAD
		ret = request_irq(irq, tmio_mmc_card_detect_irq, 0,
				  dev_name(&pdev->dev), host);
		if (ret)
			goto eirq_card_detect;
=======
		ret = devm_request_irq(&pdev->dev, irq, tmio_mmc_card_detect_irq, 0,
				  dev_name(&pdev->dev), host);
		if (ret)
			goto eirq;
>>>>>>> refs/remotes/origin/master
	}

	irq = platform_get_irq_byname(pdev, SH_MOBILE_SDHI_IRQ_SDIO);
	if (irq >= 0) {
		multiplexed_isr = false;
<<<<<<< HEAD
		ret = request_irq(irq, tmio_mmc_sdio_irq, 0,
				  dev_name(&pdev->dev), host);
		if (ret)
			goto eirq_sdio;
=======
		ret = devm_request_irq(&pdev->dev, irq, tmio_mmc_sdio_irq, 0,
				  dev_name(&pdev->dev), host);
		if (ret)
			goto eirq;
>>>>>>> refs/remotes/origin/master
	}

	irq = platform_get_irq_byname(pdev, SH_MOBILE_SDHI_IRQ_SDCARD);
	if (irq >= 0) {
		multiplexed_isr = false;
<<<<<<< HEAD
		ret = request_irq(irq, tmio_mmc_sdcard_irq, 0,
				  dev_name(&pdev->dev), host);
		if (ret)
			goto eirq_sdcard;
=======
		ret = devm_request_irq(&pdev->dev, irq, tmio_mmc_sdcard_irq, 0,
				  dev_name(&pdev->dev), host);
		if (ret)
			goto eirq;
>>>>>>> refs/remotes/origin/master
	} else if (!multiplexed_isr) {
		dev_err(&pdev->dev,
			"Principal SD-card IRQ is missing among named interrupts\n");
		ret = irq;
<<<<<<< HEAD
		goto eirq_sdcard;
=======
		goto eirq;
>>>>>>> refs/remotes/origin/master
	}

	if (multiplexed_isr) {
		while (1) {
			irq = platform_get_irq(pdev, i);
			if (irq < 0)
				break;
			i++;
<<<<<<< HEAD
			ret = request_irq(irq, tmio_mmc_irq, 0,
					  dev_name(&pdev->dev), host);
			if (ret)
				goto eirq_multiplexed;
		}

		/* There must be at least one IRQ source */
		if (!i)
			goto eirq_multiplexed;
=======
			ret = devm_request_irq(&pdev->dev, irq, tmio_mmc_irq, 0,
					  dev_name(&pdev->dev), host);
			if (ret)
				goto eirq;
		}

		/* There must be at least one IRQ source */
		if (!i) {
			ret = irq;
			goto eirq;
		}
>>>>>>> refs/remotes/origin/master
	}

	dev_info(&pdev->dev, "%s base at 0x%08lx clock rate %u MHz\n",
		 mmc_hostname(host->mmc), (unsigned long)
		 (platform_get_resource(pdev, IORESOURCE_MEM, 0)->start),
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		 mmc_data->hclk / 1000000);

	return ret;

<<<<<<< HEAD
eirq:
	tmio_mmc_host_remove(host);
eprobe:
	clk_disable(priv->clk);
	clk_put(priv->clk);
eclkget:
=======
eirq_multiplexed:
	while (i--) {
		irq = platform_get_irq(pdev, i);
		free_irq(irq, host);
	}
eirq_sdcard:
	irq = platform_get_irq_byname(pdev, SH_MOBILE_SDHI_IRQ_SDIO);
	if (irq >= 0)
		free_irq(irq, host);
eirq_sdio:
	irq = platform_get_irq_byname(pdev, SH_MOBILE_SDHI_IRQ_CARD_DETECT);
	if (irq >= 0)
		free_irq(irq, host);
eirq_card_detect:
	tmio_mmc_host_remove(host);
eprobe:
	clk_put(priv->clk);
eclkget:
	if (p->cleanup)
		p->cleanup(pdev);
einit:
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(priv);
=======
		 host->mmc->f_max / 1000000);

	return ret;

eirq:
	tmio_mmc_host_remove(host);
eprobe:
eclkget:
	if (p && p->cleanup)
		p->cleanup(pdev);
>>>>>>> refs/remotes/origin/master
	return ret;
}

static int sh_mobile_sdhi_remove(struct platform_device *pdev)
{
	struct mmc_host *mmc = platform_get_drvdata(pdev);
	struct tmio_mmc_host *host = mmc_priv(mmc);
<<<<<<< HEAD
	struct sh_mobile_sdhi *priv = container_of(host->pdata, struct sh_mobile_sdhi, mmc_data);
	struct sh_mobile_sdhi_info *p = pdev->dev.platform_data;
<<<<<<< HEAD
	int i, irq;
=======
	int i = 0, irq;
>>>>>>> refs/remotes/origin/cm-10.0

	p->pdata = NULL;

	tmio_mmc_host_remove(host);

<<<<<<< HEAD
	for (i = 0; i < 3; i++) {
		irq = platform_get_irq(pdev, i);
		if (irq >= 0)
			free_irq(irq, host);
	}

	clk_disable(priv->clk);
	clk_put(priv->clk);
=======
	while (1) {
		irq = platform_get_irq(pdev, i++);
		if (irq < 0)
			break;
		free_irq(irq, host);
	}

	clk_put(priv->clk);

	if (p->cleanup)
		p->cleanup(pdev);

>>>>>>> refs/remotes/origin/cm-10.0
	kfree(priv);

=======
	struct sh_mobile_sdhi_info *p = pdev->dev.platform_data;

	tmio_mmc_host_remove(host);

	if (p && p->cleanup)
		p->cleanup(pdev);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static const struct dev_pm_ops tmio_mmc_dev_pm_ops = {
	.suspend = tmio_mmc_host_suspend,
	.resume = tmio_mmc_host_resume,
	.runtime_suspend = tmio_mmc_host_runtime_suspend,
	.runtime_resume = tmio_mmc_host_runtime_resume,
};

static struct platform_driver sh_mobile_sdhi_driver = {
	.driver		= {
		.name	= "sh_mobile_sdhi",
		.owner	= THIS_MODULE,
		.pm	= &tmio_mmc_dev_pm_ops,
<<<<<<< HEAD
	},
	.probe		= sh_mobile_sdhi_probe,
	.remove		= __devexit_p(sh_mobile_sdhi_remove),
};

<<<<<<< HEAD
static int __init sh_mobile_sdhi_init(void)
{
	return platform_driver_register(&sh_mobile_sdhi_driver);
}

static void __exit sh_mobile_sdhi_exit(void)
{
	platform_driver_unregister(&sh_mobile_sdhi_driver);
}

module_init(sh_mobile_sdhi_init);
module_exit(sh_mobile_sdhi_exit);
=======
module_platform_driver(sh_mobile_sdhi_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.of_match_table = sh_mobile_sdhi_of_match,
	},
	.probe		= sh_mobile_sdhi_probe,
	.remove		= sh_mobile_sdhi_remove,
};

module_platform_driver(sh_mobile_sdhi_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("SuperH Mobile SDHI driver");
MODULE_AUTHOR("Magnus Damm");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:sh_mobile_sdhi");
