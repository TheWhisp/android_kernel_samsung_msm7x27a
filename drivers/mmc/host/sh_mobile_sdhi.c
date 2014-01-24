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
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/platform_device.h>
#include <linux/mmc/host.h>
#include <linux/mmc/sh_mobile_sdhi.h>
#include <linux/mfd/tmio.h>
#include <linux/sh_dma.h>
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
	mmc_detect_change(dev_get_drvdata(&pdev->dev), msecs_to_jiffies(100));
}

static const struct sh_mobile_sdhi_ops sdhi_ops = {
	.cd_wakeup = sh_mobile_sdhi_cd_wakeup,
};

>>>>>>> refs/remotes/origin/cm-10.0
static int __devinit sh_mobile_sdhi_probe(struct platform_device *pdev)
{
	struct sh_mobile_sdhi *priv;
	struct tmio_mmc_data *mmc_data;
	struct sh_mobile_sdhi_info *p = pdev->dev.platform_data;
	struct tmio_mmc_host *host;
	char clk_name[8];
<<<<<<< HEAD
	int i, irq, ret;
=======
	int irq, ret, i = 0;
	bool multiplexed_isr = true;
>>>>>>> refs/remotes/origin/cm-10.0

	priv = kzalloc(sizeof(struct sh_mobile_sdhi), GFP_KERNEL);
	if (priv == NULL) {
		dev_err(&pdev->dev, "kzalloc failed\n");
		return -ENOMEM;
	}

	mmc_data = &priv->mmc_data;
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

	/*
	 * All SDHI blocks support 2-byte and larger block sizes in 4-bit
	 * bus width mode.
	 */
	mmc_data->flags |= TMIO_MMC_BLKSZ_2BYTES;

	/*
	 * All SDHI blocks support SDIO IRQ signalling.
	 */
	mmc_data->flags |= TMIO_MMC_SDIO_IRQ;

	ret = tmio_mmc_host_probe(&host, pdev, mmc_data);
	if (ret < 0)
		goto eprobe;

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
	/*
	 * Allow one or more specific (named) ISRs or
	 * one or more multiplexed (un-named) ISRs.
	 */

	irq = platform_get_irq_byname(pdev, SH_MOBILE_SDHI_IRQ_CARD_DETECT);
	if (irq >= 0) {
		multiplexed_isr = false;
		ret = request_irq(irq, tmio_mmc_card_detect_irq, 0,
				  dev_name(&pdev->dev), host);
		if (ret)
			goto eirq_card_detect;
	}

	irq = platform_get_irq_byname(pdev, SH_MOBILE_SDHI_IRQ_SDIO);
	if (irq >= 0) {
		multiplexed_isr = false;
		ret = request_irq(irq, tmio_mmc_sdio_irq, 0,
				  dev_name(&pdev->dev), host);
		if (ret)
			goto eirq_sdio;
	}

	irq = platform_get_irq_byname(pdev, SH_MOBILE_SDHI_IRQ_SDCARD);
	if (irq >= 0) {
		multiplexed_isr = false;
		ret = request_irq(irq, tmio_mmc_sdcard_irq, 0,
				  dev_name(&pdev->dev), host);
		if (ret)
			goto eirq_sdcard;
	} else if (!multiplexed_isr) {
		dev_err(&pdev->dev,
			"Principal SD-card IRQ is missing among named interrupts\n");
		ret = irq;
		goto eirq_sdcard;
	}

	if (multiplexed_isr) {
		while (1) {
			irq = platform_get_irq(pdev, i);
			if (irq < 0)
				break;
			i++;
			ret = request_irq(irq, tmio_mmc_irq, 0,
					  dev_name(&pdev->dev), host);
			if (ret)
				goto eirq_multiplexed;
		}

		/* There must be at least one IRQ source */
		if (!i)
			goto eirq_multiplexed;
	}

	dev_info(&pdev->dev, "%s base at 0x%08lx clock rate %u MHz\n",
		 mmc_hostname(host->mmc), (unsigned long)
		 (platform_get_resource(pdev, IORESOURCE_MEM, 0)->start),
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
	return ret;
}

static int sh_mobile_sdhi_remove(struct platform_device *pdev)
{
	struct mmc_host *mmc = platform_get_drvdata(pdev);
	struct tmio_mmc_host *host = mmc_priv(mmc);
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

MODULE_DESCRIPTION("SuperH Mobile SDHI driver");
MODULE_AUTHOR("Magnus Damm");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:sh_mobile_sdhi");
