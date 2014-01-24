<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
/* Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
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
<<<<<<< HEAD
<<<<<<< HEAD
=======
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
 */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/iommu.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/slab.h>

<<<<<<< HEAD
#include <mach/iommu_hw-8xxx.h>
#include <mach/iommu.h>
<<<<<<< HEAD
=======
#include "msm_iommu_hw-8xxx.h"
#include "msm_iommu.h"
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

struct iommu_ctx_iter_data {
	/* input */
	const char *name;

	/* output */
	struct device *dev;
};

<<<<<<< HEAD
<<<<<<< HEAD
struct platform_device *msm_iommu_root_dev;
=======
static struct platform_device *msm_iommu_root_dev;
>>>>>>> refs/remotes/origin/master
=======
struct platform_device *msm_iommu_root_dev;
>>>>>>> refs/remotes/origin/cm-11.0

static int each_iommu_ctx(struct device *dev, void *data)
{
	struct iommu_ctx_iter_data *res = data;
<<<<<<< HEAD
<<<<<<< HEAD
	struct msm_iommu_ctx_drvdata *c;

	c = dev_get_drvdata(dev);
=======
	struct msm_iommu_ctx_dev *c = dev->platform_data;

>>>>>>> refs/remotes/origin/master
=======
	struct msm_iommu_ctx_drvdata *c;

	c = dev_get_drvdata(dev);
>>>>>>> refs/remotes/origin/cm-11.0
	if (!res || !c || !c->name || !res->name)
		return -EINVAL;

	if (!strcmp(res->name, c->name)) {
		res->dev = dev;
		return 1;
	}
	return 0;
}

static int each_iommu(struct device *dev, void *data)
{
	return device_for_each_child(dev, data, each_iommu_ctx);
}

struct device *msm_iommu_get_ctx(const char *ctx_name)
{
	struct iommu_ctx_iter_data r;
	int found;

	if (!msm_iommu_root_dev) {
		pr_err("No root IOMMU device.\n");
		goto fail;
	}

	r.name = ctx_name;
	found = device_for_each_child(&msm_iommu_root_dev->dev, &r, each_iommu);

<<<<<<< HEAD
<<<<<<< HEAD
	if (found <= 0 || !dev_get_drvdata(r.dev)) {
=======
	if (!found) {
>>>>>>> refs/remotes/origin/master
=======
	if (found <= 0 || !dev_get_drvdata(r.dev)) {
>>>>>>> refs/remotes/origin/cm-11.0
		pr_err("Could not find context <%s>\n", ctx_name);
		goto fail;
	}

	return r.dev;
fail:
	return NULL;
}
EXPORT_SYMBOL(msm_iommu_get_ctx);

static void msm_iommu_reset(void __iomem *base, int ncb)
{
	int ctx;

	SET_RPUE(base, 0);
	SET_RPUEIE(base, 0);
	SET_ESRRESTORE(base, 0);
	SET_TBE(base, 0);
	SET_CR(base, 0);
	SET_SPDMBE(base, 0);
	SET_TESTBUSCR(base, 0);
	SET_TLBRSW(base, 0);
	SET_GLOBAL_TLBIALL(base, 0);
	SET_RPU_ACR(base, 0);
	SET_TLBLKCRWE(base, 1);

	for (ctx = 0; ctx < ncb; ctx++) {
		SET_BPRCOSH(base, ctx, 0);
		SET_BPRCISH(base, ctx, 0);
		SET_BPRCNSH(base, ctx, 0);
		SET_BPSHCFG(base, ctx, 0);
		SET_BPMTCFG(base, ctx, 0);
		SET_ACTLR(base, ctx, 0);
		SET_SCTLR(base, ctx, 0);
		SET_FSRRESTORE(base, ctx, 0);
		SET_TTBR0(base, ctx, 0);
		SET_TTBR1(base, ctx, 0);
		SET_TTBCR(base, ctx, 0);
		SET_BFBCR(base, ctx, 0);
		SET_PAR(base, ctx, 0);
		SET_FAR(base, ctx, 0);
<<<<<<< HEAD
<<<<<<< HEAD
		SET_TLBFLPTER(base, ctx, 0);
		SET_TLBSLPTER(base, ctx, 0);
		SET_TLBLKCR(base, ctx, 0);
		SET_CTX_TLBIALL(base, ctx, 0);
		SET_TLBIVA(base, ctx, 0);
=======
		SET_CTX_TLBIALL(base, ctx, 0);
		SET_TLBFLPTER(base, ctx, 0);
		SET_TLBSLPTER(base, ctx, 0);
		SET_TLBLKCR(base, ctx, 0);
>>>>>>> refs/remotes/origin/master
=======
		SET_TLBFLPTER(base, ctx, 0);
		SET_TLBSLPTER(base, ctx, 0);
		SET_TLBLKCR(base, ctx, 0);
		SET_CTX_TLBIALL(base, ctx, 0);
		SET_TLBIVA(base, ctx, 0);
>>>>>>> refs/remotes/origin/cm-11.0
		SET_PRRR(base, ctx, 0);
		SET_NMRR(base, ctx, 0);
		SET_CONTEXTIDR(base, ctx, 0);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	mb();
=======
>>>>>>> refs/remotes/origin/master
=======
	mb();
>>>>>>> refs/remotes/origin/cm-11.0
}

static int msm_iommu_probe(struct platform_device *pdev)
{
	struct resource *r, *r2;
<<<<<<< HEAD
<<<<<<< HEAD
	struct clk *iommu_clk = NULL;
	struct clk *iommu_pclk = NULL;
=======
	struct clk *iommu_clk;
	struct clk *iommu_pclk;
>>>>>>> refs/remotes/origin/master
=======
	struct clk *iommu_clk = NULL;
	struct clk *iommu_pclk = NULL;
>>>>>>> refs/remotes/origin/cm-11.0
	struct msm_iommu_drvdata *drvdata;
	struct msm_iommu_dev *iommu_dev = pdev->dev.platform_data;
	void __iomem *regs_base;
	resource_size_t	len;
<<<<<<< HEAD
<<<<<<< HEAD
	int ret, par;
=======
	int ret, irq, par;
>>>>>>> refs/remotes/origin/master
=======
	int ret, par;
>>>>>>> refs/remotes/origin/cm-11.0

	if (pdev->id == -1) {
		msm_iommu_root_dev = pdev;
		return 0;
	}

	drvdata = kzalloc(sizeof(*drvdata), GFP_KERNEL);

	if (!drvdata) {
		ret = -ENOMEM;
		goto fail;
	}

	if (!iommu_dev) {
		ret = -ENODEV;
		goto fail;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	iommu_pclk = clk_get_sys("msm_iommu", "iface_clk");
=======
	iommu_pclk = clk_get(NULL, "smmu_pclk");
>>>>>>> refs/remotes/origin/master
=======
	iommu_pclk = clk_get_sys("msm_iommu", "iface_clk");
>>>>>>> refs/remotes/origin/cm-11.0
	if (IS_ERR(iommu_pclk)) {
		ret = -ENODEV;
		goto fail;
	}

	ret = clk_prepare_enable(iommu_pclk);
	if (ret)
		goto fail_enable;

<<<<<<< HEAD
<<<<<<< HEAD
	iommu_clk = clk_get(&pdev->dev, "core_clk");

	if (!IS_ERR(iommu_clk))	{
		if (clk_get_rate(iommu_clk) == 0) {
			ret = clk_round_rate(iommu_clk, 1);
			clk_set_rate(iommu_clk, ret);
		}
=======
	iommu_clk = clk_get(&pdev->dev, "iommu_clk");

	if (!IS_ERR(iommu_clk))	{
		if (clk_get_rate(iommu_clk) == 0)
			clk_set_rate(iommu_clk, 1);
>>>>>>> refs/remotes/origin/master
=======
	iommu_clk = clk_get(&pdev->dev, "core_clk");

	if (!IS_ERR(iommu_clk))	{
		if (clk_get_rate(iommu_clk) == 0) {
			ret = clk_round_rate(iommu_clk, 1);
			clk_set_rate(iommu_clk, ret);
		}
>>>>>>> refs/remotes/origin/cm-11.0

		ret = clk_prepare_enable(iommu_clk);
		if (ret) {
			clk_put(iommu_clk);
			goto fail_pclk;
		}
	} else
		iommu_clk = NULL;

	r = platform_get_resource_byname(pdev, IORESOURCE_MEM, "physbase");

	if (!r) {
		ret = -ENODEV;
		goto fail_clk;
	}

	len = resource_size(r);

	r2 = request_mem_region(r->start, len, r->name);
	if (!r2) {
		pr_err("Could not request memory region: start=%p, len=%d\n",
							(void *) r->start, len);
		ret = -EBUSY;
		goto fail_clk;
	}

	regs_base = ioremap(r2->start, len);

	if (!regs_base) {
		pr_err("Could not ioremap: start=%p, len=%d\n",
			 (void *) r2->start, len);
		ret = -EBUSY;
		goto fail_mem;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
	irq = platform_get_irq_byname(pdev, "secure_irq");
	if (irq < 0) {
		ret = -ENODEV;
		goto fail_io;
	}

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	msm_iommu_reset(regs_base, iommu_dev->ncb);

	SET_M(regs_base, 0, 1);
	SET_PAR(regs_base, 0, 0);
	SET_V2PCFG(regs_base, 0, 1);
	SET_V2PPR(regs_base, 0, 0);
<<<<<<< HEAD
<<<<<<< HEAD
	mb();
	par = GET_PAR(regs_base, 0);
	SET_V2PCFG(regs_base, 0, 0);
	SET_M(regs_base, 0, 0);
	mb();
=======
	par = GET_PAR(regs_base, 0);
	SET_V2PCFG(regs_base, 0, 0);
	SET_M(regs_base, 0, 0);
>>>>>>> refs/remotes/origin/master
=======
	mb();
	par = GET_PAR(regs_base, 0);
	SET_V2PCFG(regs_base, 0, 0);
	SET_M(regs_base, 0, 0);
	mb();
>>>>>>> refs/remotes/origin/cm-11.0

	if (!par) {
		pr_err("%s: Invalid PAR value detected\n", iommu_dev->name);
		ret = -ENODEV;
		goto fail_io;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	drvdata->pclk = iommu_pclk;
	drvdata->clk = iommu_clk;
	drvdata->base = regs_base;
	drvdata->ncb = iommu_dev->ncb;
	drvdata->ttbr_split = iommu_dev->ttbr_split;
	drvdata->name = iommu_dev->name;

	pr_info("device %s mapped at %p, with %d ctx banks\n",
		iommu_dev->name, regs_base, iommu_dev->ncb);
=======
	ret = request_irq(irq, msm_iommu_fault_handler, 0,
			"msm_iommu_secure_irpt_handler", drvdata);
	if (ret) {
		pr_err("Request IRQ %d failed with ret=%d\n", irq, ret);
		goto fail_io;
	}


=======
>>>>>>> refs/remotes/origin/cm-11.0
	drvdata->pclk = iommu_pclk;
	drvdata->clk = iommu_clk;
	drvdata->base = regs_base;
	drvdata->ncb = iommu_dev->ncb;
	drvdata->ttbr_split = iommu_dev->ttbr_split;
	drvdata->name = iommu_dev->name;

<<<<<<< HEAD
	pr_info("device %s mapped at %p, irq %d with %d ctx banks\n",
		iommu_dev->name, regs_base, irq, iommu_dev->ncb);
>>>>>>> refs/remotes/origin/master
=======
	pr_info("device %s mapped at %p, with %d ctx banks\n",
		iommu_dev->name, regs_base, iommu_dev->ncb);
>>>>>>> refs/remotes/origin/cm-11.0

	platform_set_drvdata(pdev, drvdata);

	if (iommu_clk)
<<<<<<< HEAD
<<<<<<< HEAD
		clk_disable_unprepare(iommu_clk);

	clk_disable_unprepare(iommu_pclk);
=======
		clk_disable(iommu_clk);

	clk_disable(iommu_pclk);
>>>>>>> refs/remotes/origin/master
=======
		clk_disable_unprepare(iommu_clk);

	clk_disable_unprepare(iommu_pclk);
>>>>>>> refs/remotes/origin/cm-11.0

	return 0;
fail_io:
	iounmap(regs_base);
fail_mem:
	release_mem_region(r->start, len);
fail_clk:
	if (iommu_clk) {
<<<<<<< HEAD
<<<<<<< HEAD
		clk_disable_unprepare(iommu_clk);
=======
		clk_disable(iommu_clk);
>>>>>>> refs/remotes/origin/master
=======
		clk_disable_unprepare(iommu_clk);
>>>>>>> refs/remotes/origin/cm-11.0
		clk_put(iommu_clk);
	}
fail_pclk:
	clk_disable_unprepare(iommu_pclk);
fail_enable:
	clk_put(iommu_pclk);
fail:
	kfree(drvdata);
	return ret;
}

static int msm_iommu_remove(struct platform_device *pdev)
{
	struct msm_iommu_drvdata *drv = NULL;

	drv = platform_get_drvdata(pdev);
	if (drv) {
<<<<<<< HEAD
		if (drv->clk)
			clk_put(drv->clk);
		clk_put(drv->pclk);
		memset(drv, 0, sizeof(*drv));
		kfree(drv);
		platform_set_drvdata(pdev, NULL);
=======
		if (drv->clk) {
			clk_unprepare(drv->clk);
			clk_put(drv->clk);
		}
		clk_unprepare(drv->pclk);
		clk_put(drv->pclk);
		memset(drv, 0, sizeof(*drv));
		kfree(drv);
>>>>>>> refs/remotes/origin/master
	}
	return 0;
}

static int msm_iommu_ctx_probe(struct platform_device *pdev)
{
	struct msm_iommu_ctx_dev *c = pdev->dev.platform_data;
	struct msm_iommu_drvdata *drvdata;
<<<<<<< HEAD
	struct msm_iommu_ctx_drvdata *ctx_drvdata = NULL;
	int i, ret, irq;
	if (!c || !pdev->dev.parent) {
		ret = -EINVAL;
		goto fail;
	}

	drvdata = dev_get_drvdata(pdev->dev.parent);

	if (!drvdata) {
		ret = -ENODEV;
		goto fail;
	}

	ctx_drvdata = kzalloc(sizeof(*ctx_drvdata), GFP_KERNEL);
	if (!ctx_drvdata) {
		ret = -ENOMEM;
		goto fail;
	}
	ctx_drvdata->num = c->num;
	ctx_drvdata->pdev = pdev;
	ctx_drvdata->name = c->name;

	irq = platform_get_irq_byname(to_platform_device(pdev->dev.parent),
				      "nonsecure_irq");
	if (irq < 0) {
		ret = -ENODEV;
		goto fail;
	}

	ret = request_threaded_irq(irq, NULL, msm_iommu_fault_handler,
				   IRQF_ONESHOT | IRQF_SHARED,
				   "msm_iommu_nonsecure_irq", ctx_drvdata);

	if (ret) {
		pr_err("request_threaded_irq %d failed: %d\n", irq, ret);
		goto fail;
	}
<<<<<<< HEAD
=======
	struct msm_iommu_ctx_drvdata *ctx_drvdata;
	int i, ret;

	if (!c || !pdev->dev.parent)
		return -EINVAL;

	drvdata = dev_get_drvdata(pdev->dev.parent);
	if (!drvdata)
		return -ENODEV;

	ctx_drvdata = kzalloc(sizeof(*ctx_drvdata), GFP_KERNEL);
	if (!ctx_drvdata)
		return -ENOMEM;

	ctx_drvdata->num = c->num;
	ctx_drvdata->pdev = pdev;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	INIT_LIST_HEAD(&ctx_drvdata->attached_elm);
	platform_set_drvdata(pdev, ctx_drvdata);

	ret = clk_prepare_enable(drvdata->pclk);
	if (ret)
		goto fail;

	if (drvdata->clk) {
		ret = clk_prepare_enable(drvdata->clk);
		if (ret) {
			clk_disable_unprepare(drvdata->pclk);
			goto fail;
		}
	}

	/* Program the M2V tables for this context */
	for (i = 0; i < MAX_NUM_MIDS; i++) {
		int mid = c->mids[i];
		if (mid == -1)
			break;

		SET_M2VCBR_N(drvdata->base, mid, 0);
		SET_CBACR_N(drvdata->base, c->num, 0);

<<<<<<< HEAD
<<<<<<< HEAD
		/* Route page faults to the non-secure interrupt */
		SET_IRPTNDX(drvdata->base, c->num, 1);

=======
>>>>>>> refs/remotes/origin/master
=======
		/* Route page faults to the non-secure interrupt */
		SET_IRPTNDX(drvdata->base, c->num, 1);

>>>>>>> refs/remotes/origin/cm-11.0
		/* Set VMID = 0 */
		SET_VMID(drvdata->base, mid, 0);

		/* Set the context number for that MID to this context */
		SET_CBNDX(drvdata->base, mid, c->num);

<<<<<<< HEAD
<<<<<<< HEAD
		/* Set MID associated with this context bank to 0 */
		SET_CBVMID(drvdata->base, c->num, 0);

		/* Set the ASID for TLB tagging for this context to 0 */
		SET_CONTEXTIDR_ASID(drvdata->base, c->num, 0);
=======
		/* Set MID associated with this context bank to 0*/
		SET_CBVMID(drvdata->base, c->num, 0);

		/* Set the ASID for TLB tagging for this context */
		SET_CONTEXTIDR_ASID(drvdata->base, c->num, c->num);
>>>>>>> refs/remotes/origin/master
=======
		/* Set MID associated with this context bank to 0 */
		SET_CBVMID(drvdata->base, c->num, 0);

		/* Set the ASID for TLB tagging for this context to 0 */
		SET_CONTEXTIDR_ASID(drvdata->base, c->num, 0);
>>>>>>> refs/remotes/origin/cm-11.0

		/* Set security bit override to be Non-secure */
		SET_NSCFG(drvdata->base, mid, 3);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	mb();

	if (drvdata->clk)
		clk_disable_unprepare(drvdata->clk);
	clk_disable_unprepare(drvdata->pclk);
=======

	if (drvdata->clk)
		clk_disable(drvdata->clk);
	clk_disable(drvdata->pclk);
>>>>>>> refs/remotes/origin/master
=======
	mb();

	if (drvdata->clk)
		clk_disable_unprepare(drvdata->clk);
	clk_disable_unprepare(drvdata->pclk);
>>>>>>> refs/remotes/origin/cm-11.0

	dev_info(&pdev->dev, "context %s using bank %d\n", c->name, c->num);
	return 0;
fail:
	kfree(ctx_drvdata);
	return ret;
}

static int msm_iommu_ctx_remove(struct platform_device *pdev)
{
	struct msm_iommu_ctx_drvdata *drv = NULL;
	drv = platform_get_drvdata(pdev);
	if (drv) {
		memset(drv, 0, sizeof(struct msm_iommu_ctx_drvdata));
		kfree(drv);
<<<<<<< HEAD
		platform_set_drvdata(pdev, NULL);
=======
>>>>>>> refs/remotes/origin/master
	}
	return 0;
}

static struct platform_driver msm_iommu_driver = {
	.driver = {
		.name	= "msm_iommu",
	},
	.probe		= msm_iommu_probe,
	.remove		= msm_iommu_remove,
};

static struct platform_driver msm_iommu_ctx_driver = {
	.driver = {
		.name	= "msm_iommu_ctx",
	},
	.probe		= msm_iommu_ctx_probe,
	.remove		= msm_iommu_ctx_remove,
};

static int __init msm_iommu_driver_init(void)
{
	int ret;
	ret = platform_driver_register(&msm_iommu_driver);
	if (ret != 0) {
		pr_err("Failed to register IOMMU driver\n");
		goto error;
	}

	ret = platform_driver_register(&msm_iommu_ctx_driver);
	if (ret != 0) {
<<<<<<< HEAD
=======
		platform_driver_unregister(&msm_iommu_driver);
>>>>>>> refs/remotes/origin/master
		pr_err("Failed to register IOMMU context driver\n");
		goto error;
	}

error:
	return ret;
}

static void __exit msm_iommu_driver_exit(void)
{
	platform_driver_unregister(&msm_iommu_ctx_driver);
	platform_driver_unregister(&msm_iommu_driver);
}

subsys_initcall(msm_iommu_driver_init);
module_exit(msm_iommu_driver_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Stepan Moskovchenko <stepanm@codeaurora.org>");
