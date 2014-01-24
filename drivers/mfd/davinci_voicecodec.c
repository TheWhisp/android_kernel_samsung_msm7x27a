/*
 * DaVinci Voice Codec Core Interface for TI platforms
 *
 * Copyright (C) 2010 Texas Instruments, Inc
 *
 * Author: Miguel Aguilar <miguel.aguilar@ridgerun.com>
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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/clk.h>
<<<<<<< HEAD
=======
#include <linux/regmap.h>
>>>>>>> refs/remotes/origin/master

#include <sound/pcm.h>

#include <linux/mfd/davinci_voicecodec.h>

<<<<<<< HEAD
u32 davinci_vc_read(struct davinci_vc *davinci_vc, int reg)
{
	return __raw_readl(davinci_vc->base + reg);
}

void davinci_vc_write(struct davinci_vc *davinci_vc,
					   int reg, u32 val)
{
	__raw_writel(val, davinci_vc->base + reg);
}
=======
static struct regmap_config davinci_vc_regmap = {
	.reg_bits = 32,
	.val_bits = 32,
};
>>>>>>> refs/remotes/origin/master

static int __init davinci_vc_probe(struct platform_device *pdev)
{
	struct davinci_vc *davinci_vc;
<<<<<<< HEAD
	struct resource *res, *mem;
	struct mfd_cell *cell = NULL;
	int ret;

	davinci_vc = kzalloc(sizeof(struct davinci_vc), GFP_KERNEL);
=======
	struct resource *res;
	struct mfd_cell *cell = NULL;
	int ret;

	davinci_vc = devm_kzalloc(&pdev->dev,
				  sizeof(struct davinci_vc), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!davinci_vc) {
		dev_dbg(&pdev->dev,
			    "could not allocate memory for private data\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	davinci_vc->clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(davinci_vc->clk)) {
		dev_dbg(&pdev->dev,
			    "could not get the clock for voice codec\n");
		ret = -ENODEV;
		goto fail1;
=======
	davinci_vc->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(davinci_vc->clk)) {
		dev_dbg(&pdev->dev,
			    "could not get the clock for voice codec\n");
		return -ENODEV;
>>>>>>> refs/remotes/origin/master
	}
	clk_enable(davinci_vc->clk);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
<<<<<<< HEAD
	if (!res) {
		dev_err(&pdev->dev, "no mem resource\n");
		ret = -ENODEV;
		goto fail2;
	}

	davinci_vc->pbase = res->start;
	davinci_vc->base_size = resource_size(res);

	mem = request_mem_region(davinci_vc->pbase, davinci_vc->base_size,
				 pdev->name);
	if (!mem) {
		dev_err(&pdev->dev, "VCIF region already claimed\n");
		ret = -EBUSY;
		goto fail2;
	}

	davinci_vc->base = ioremap(davinci_vc->pbase, davinci_vc->base_size);
	if (!davinci_vc->base) {
		dev_err(&pdev->dev, "can't ioremap mem resource.\n");
		ret = -ENOMEM;
		goto fail3;
=======

	davinci_vc->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(davinci_vc->base)) {
		ret = PTR_ERR(davinci_vc->base);
		goto fail;
	}

	davinci_vc->regmap = devm_regmap_init_mmio(&pdev->dev,
						   davinci_vc->base,
						   &davinci_vc_regmap);
	if (IS_ERR(davinci_vc->regmap)) {
		ret = PTR_ERR(davinci_vc->regmap);
		goto fail;
>>>>>>> refs/remotes/origin/master
	}

	res = platform_get_resource(pdev, IORESOURCE_DMA, 0);
	if (!res) {
		dev_err(&pdev->dev, "no DMA resource\n");
		ret = -ENXIO;
<<<<<<< HEAD
		goto fail4;
=======
		goto fail;
>>>>>>> refs/remotes/origin/master
	}

	davinci_vc->davinci_vcif.dma_tx_channel = res->start;
	davinci_vc->davinci_vcif.dma_tx_addr =
		(dma_addr_t)(io_v2p(davinci_vc->base) + DAVINCI_VC_WFIFO);

	res = platform_get_resource(pdev, IORESOURCE_DMA, 1);
	if (!res) {
		dev_err(&pdev->dev, "no DMA resource\n");
		ret = -ENXIO;
<<<<<<< HEAD
		goto fail4;
=======
		goto fail;
>>>>>>> refs/remotes/origin/master
	}

	davinci_vc->davinci_vcif.dma_rx_channel = res->start;
	davinci_vc->davinci_vcif.dma_rx_addr =
		(dma_addr_t)(io_v2p(davinci_vc->base) + DAVINCI_VC_RFIFO);

	davinci_vc->dev = &pdev->dev;
	davinci_vc->pdev = pdev;

	/* Voice codec interface client */
	cell = &davinci_vc->cells[DAVINCI_VC_VCIF_CELL];
	cell->name = "davinci-vcif";
	cell->platform_data = davinci_vc;
	cell->pdata_size = sizeof(*davinci_vc);

	/* Voice codec CQ93VC client */
	cell = &davinci_vc->cells[DAVINCI_VC_CQ93VC_CELL];
	cell->name = "cq93vc-codec";
	cell->platform_data = davinci_vc;
	cell->pdata_size = sizeof(*davinci_vc);

	ret = mfd_add_devices(&pdev->dev, pdev->id, davinci_vc->cells,
<<<<<<< HEAD
			      DAVINCI_VC_CELLS, NULL, 0);
	if (ret != 0) {
		dev_err(&pdev->dev, "fail to register client devices\n");
		goto fail4;
=======
			      DAVINCI_VC_CELLS, NULL, 0, NULL);
	if (ret != 0) {
		dev_err(&pdev->dev, "fail to register client devices\n");
		goto fail;
>>>>>>> refs/remotes/origin/master
	}

	return 0;

<<<<<<< HEAD
fail4:
	iounmap(davinci_vc->base);
fail3:
	release_mem_region(davinci_vc->pbase, davinci_vc->base_size);
fail2:
	clk_disable(davinci_vc->clk);
	clk_put(davinci_vc->clk);
	davinci_vc->clk = NULL;
fail1:
	kfree(davinci_vc);
=======
fail:
	clk_disable(davinci_vc->clk);
>>>>>>> refs/remotes/origin/master

	return ret;
}

<<<<<<< HEAD
static int __devexit davinci_vc_remove(struct platform_device *pdev)
=======
static int davinci_vc_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct davinci_vc *davinci_vc = platform_get_drvdata(pdev);

	mfd_remove_devices(&pdev->dev);

<<<<<<< HEAD
	iounmap(davinci_vc->base);
	release_mem_region(davinci_vc->pbase, davinci_vc->base_size);

	clk_disable(davinci_vc->clk);
	clk_put(davinci_vc->clk);
	davinci_vc->clk = NULL;

	kfree(davinci_vc);
=======
	clk_disable(davinci_vc->clk);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static struct platform_driver davinci_vc_driver = {
	.driver	= {
		.name = "davinci_voicecodec",
		.owner = THIS_MODULE,
	},
<<<<<<< HEAD
	.remove	= __devexit_p(davinci_vc_remove),
};

static int __init davinci_vc_init(void)
{
	return platform_driver_probe(&davinci_vc_driver, davinci_vc_probe);
}
module_init(davinci_vc_init);

static void __exit davinci_vc_exit(void)
{
	platform_driver_unregister(&davinci_vc_driver);
}
module_exit(davinci_vc_exit);
=======
	.remove	= davinci_vc_remove,
};

module_platform_driver_probe(davinci_vc_driver, davinci_vc_probe);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Miguel Aguilar");
MODULE_DESCRIPTION("Texas Instruments DaVinci Voice Codec Core Interface");
MODULE_LICENSE("GPL");
