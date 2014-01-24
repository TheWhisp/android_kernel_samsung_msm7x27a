/*
 * omap_wdt.c
 *
 * Watchdog driver for the TI OMAP 16xx & 24xx/34xx 32KHz (non-secure) watchdog
 *
 * Author: MontaVista Software, Inc.
 *	 <gdavis@mvista.com> or <source@mvista.com>
 *
 * 2003 (c) MontaVista Software, Inc. This file is licensed under the
 * terms of the GNU General Public License version 2. This program is
 * licensed "as is" without any warranty of any kind, whether express
 * or implied.
 *
 * History:
 *
 * 20030527: George G. Davis <gdavis@mvista.com>
 *	Initially based on linux-2.4.19-rmk7-pxa1/drivers/char/sa1100_wdt.c
 *	(c) Copyright 2000 Oleg Drokin <green@crimea.edu>
 *	Based on SoftDog driver by Alan Cox <alan@lxorguk.ukuu.org.uk>
 *
 * Copyright (c) 2004 Texas Instruments.
 *	1. Modified to support OMAP1610 32-KHz watchdog timer
 *	2. Ported to 2.6 kernel
 *
 * Copyright (c) 2005 David Brownell
 *	Use the driver model and standard identifiers; handle bigger timeouts.
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
>>>>>>> refs/remotes/origin/master
#include <linux/watchdog.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/moduleparam.h>
<<<<<<< HEAD
#include <linux/bitops.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>
#include <mach/hardware.h>
#include <plat/prcm.h>

#include "omap_wdt.h"

static struct platform_device *omap_wdt_dev;
=======
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>
#include <linux/platform_data/omap-wd-timer.h>

#include "omap_wdt.h"

static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started "
	"(default=" __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");
>>>>>>> refs/remotes/origin/master

static unsigned timer_margin;
module_param(timer_margin, uint, 0);
MODULE_PARM_DESC(timer_margin, "initial watchdog timeout (in seconds)");

<<<<<<< HEAD
static unsigned int wdt_trgr_pattern = 0x1234;
<<<<<<< HEAD
static spinlock_t wdt_lock;
=======
static DEFINE_SPINLOCK(wdt_lock);
>>>>>>> refs/remotes/origin/cm-10.0

struct omap_wdt_dev {
	void __iomem    *base;          /* physical */
	struct device   *dev;
	int             omap_wdt_users;
	struct resource *mem;
	struct miscdevice omap_wdt_miscdev;
};

static void omap_wdt_ping(struct omap_wdt_dev *wdev)
=======
struct omap_wdt_dev {
	void __iomem    *base;          /* physical */
	struct device   *dev;
	bool		omap_wdt_users;
	struct resource *mem;
	int		wdt_trgr_pattern;
	struct mutex	lock;		/* to avoid races with PM */
};

static void omap_wdt_reload(struct omap_wdt_dev *wdev)
>>>>>>> refs/remotes/origin/master
{
	void __iomem    *base = wdev->base;

	/* wait for posted write to complete */
<<<<<<< HEAD
	while ((__raw_readl(base + OMAP_WATCHDOG_WPS)) & 0x08)
		cpu_relax();

	wdt_trgr_pattern = ~wdt_trgr_pattern;
	__raw_writel(wdt_trgr_pattern, (base + OMAP_WATCHDOG_TGR));

	/* wait for posted write to complete */
	while ((__raw_readl(base + OMAP_WATCHDOG_WPS)) & 0x08)
=======
	while ((readl_relaxed(base + OMAP_WATCHDOG_WPS)) & 0x08)
		cpu_relax();

	wdev->wdt_trgr_pattern = ~wdev->wdt_trgr_pattern;
	writel_relaxed(wdev->wdt_trgr_pattern, (base + OMAP_WATCHDOG_TGR));

	/* wait for posted write to complete */
	while ((readl_relaxed(base + OMAP_WATCHDOG_WPS)) & 0x08)
>>>>>>> refs/remotes/origin/master
		cpu_relax();
	/* reloaded WCRR from WLDR */
}

static void omap_wdt_enable(struct omap_wdt_dev *wdev)
{
	void __iomem *base = wdev->base;

	/* Sequence to enable the watchdog */
<<<<<<< HEAD
	__raw_writel(0xBBBB, base + OMAP_WATCHDOG_SPR);
	while ((__raw_readl(base + OMAP_WATCHDOG_WPS)) & 0x10)
		cpu_relax();

	__raw_writel(0x4444, base + OMAP_WATCHDOG_SPR);
	while ((__raw_readl(base + OMAP_WATCHDOG_WPS)) & 0x10)
=======
	writel_relaxed(0xBBBB, base + OMAP_WATCHDOG_SPR);
	while ((readl_relaxed(base + OMAP_WATCHDOG_WPS)) & 0x10)
		cpu_relax();

	writel_relaxed(0x4444, base + OMAP_WATCHDOG_SPR);
	while ((readl_relaxed(base + OMAP_WATCHDOG_WPS)) & 0x10)
>>>>>>> refs/remotes/origin/master
		cpu_relax();
}

static void omap_wdt_disable(struct omap_wdt_dev *wdev)
{
	void __iomem *base = wdev->base;

	/* sequence required to disable watchdog */
<<<<<<< HEAD
	__raw_writel(0xAAAA, base + OMAP_WATCHDOG_SPR);	/* TIMER_MODE */
	while (__raw_readl(base + OMAP_WATCHDOG_WPS) & 0x10)
		cpu_relax();

	__raw_writel(0x5555, base + OMAP_WATCHDOG_SPR);	/* TIMER_MODE */
	while (__raw_readl(base + OMAP_WATCHDOG_WPS) & 0x10)
		cpu_relax();
}

static void omap_wdt_adjust_timeout(unsigned new_timeout)
{
	if (new_timeout < TIMER_MARGIN_MIN)
		new_timeout = TIMER_MARGIN_DEFAULT;
	if (new_timeout > TIMER_MARGIN_MAX)
		new_timeout = TIMER_MARGIN_MAX;
	timer_margin = new_timeout;
}

static void omap_wdt_set_timeout(struct omap_wdt_dev *wdev)
{
	u32 pre_margin = GET_WLDR_VAL(timer_margin);
	void __iomem *base = wdev->base;

	pm_runtime_get_sync(wdev->dev);

	/* just count up at 32 KHz */
	while (__raw_readl(base + OMAP_WATCHDOG_WPS) & 0x04)
		cpu_relax();

	__raw_writel(pre_margin, base + OMAP_WATCHDOG_LDR);
	while (__raw_readl(base + OMAP_WATCHDOG_WPS) & 0x04)
		cpu_relax();

	pm_runtime_put_sync(wdev->dev);
}

/*
 *	Allow only one task to hold it open
 */
static int omap_wdt_open(struct inode *inode, struct file *file)
{
	struct omap_wdt_dev *wdev = platform_get_drvdata(omap_wdt_dev);
	void __iomem *base = wdev->base;

	if (test_and_set_bit(1, (unsigned long *)&(wdev->omap_wdt_users)))
		return -EBUSY;
=======
	writel_relaxed(0xAAAA, base + OMAP_WATCHDOG_SPR);	/* TIMER_MODE */
	while (readl_relaxed(base + OMAP_WATCHDOG_WPS) & 0x10)
		cpu_relax();

	writel_relaxed(0x5555, base + OMAP_WATCHDOG_SPR);	/* TIMER_MODE */
	while (readl_relaxed(base + OMAP_WATCHDOG_WPS) & 0x10)
		cpu_relax();
}

static void omap_wdt_set_timer(struct omap_wdt_dev *wdev,
				   unsigned int timeout)
{
	u32 pre_margin = GET_WLDR_VAL(timeout);
	void __iomem *base = wdev->base;

	/* just count up at 32 KHz */
	while (readl_relaxed(base + OMAP_WATCHDOG_WPS) & 0x04)
		cpu_relax();

	writel_relaxed(pre_margin, base + OMAP_WATCHDOG_LDR);
	while (readl_relaxed(base + OMAP_WATCHDOG_WPS) & 0x04)
		cpu_relax();
}

static int omap_wdt_start(struct watchdog_device *wdog)
{
	struct omap_wdt_dev *wdev = watchdog_get_drvdata(wdog);
	void __iomem *base = wdev->base;

	mutex_lock(&wdev->lock);

	wdev->omap_wdt_users = true;
>>>>>>> refs/remotes/origin/master

	pm_runtime_get_sync(wdev->dev);

	/* initialize prescaler */
<<<<<<< HEAD
	while (__raw_readl(base + OMAP_WATCHDOG_WPS) & 0x01)
		cpu_relax();

	__raw_writel((1 << 5) | (PTV << 2), base + OMAP_WATCHDOG_CNTRL);
	while (__raw_readl(base + OMAP_WATCHDOG_WPS) & 0x01)
		cpu_relax();

	file->private_data = (void *) wdev;

	omap_wdt_set_timeout(wdev);
	omap_wdt_ping(wdev); /* trigger loading of new timeout value */
	omap_wdt_enable(wdev);

	pm_runtime_put_sync(wdev->dev);

	return nonseekable_open(inode, file);
}

static int omap_wdt_release(struct inode *inode, struct file *file)
{
	struct omap_wdt_dev *wdev = file->private_data;

	/*
	 *      Shut off the timer unless NOWAYOUT is defined.
	 */
#ifndef CONFIG_WATCHDOG_NOWAYOUT
	pm_runtime_get_sync(wdev->dev);

	omap_wdt_disable(wdev);

	pm_runtime_put_sync(wdev->dev);
#else
<<<<<<< HEAD
	printk(KERN_CRIT "omap_wdt: Unexpected close, not stopping!\n");
=======
	pr_crit("Unexpected close, not stopping!\n");
>>>>>>> refs/remotes/origin/cm-10.0
#endif
	wdev->omap_wdt_users = 0;

	return 0;
}

static ssize_t omap_wdt_write(struct file *file, const char __user *data,
		size_t len, loff_t *ppos)
{
	struct omap_wdt_dev *wdev = file->private_data;

	/* Refresh LOAD_TIME. */
	if (len) {
		pm_runtime_get_sync(wdev->dev);
		spin_lock(&wdt_lock);
		omap_wdt_ping(wdev);
		spin_unlock(&wdt_lock);
		pm_runtime_put_sync(wdev->dev);
	}
	return len;
}

static long omap_wdt_ioctl(struct file *file, unsigned int cmd,
						unsigned long arg)
{
	struct omap_wdt_dev *wdev;
	int new_margin;
	static const struct watchdog_info ident = {
		.identity = "OMAP Watchdog",
		.options = WDIOF_SETTIMEOUT,
		.firmware_version = 0,
	};

	wdev = file->private_data;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		return copy_to_user((struct watchdog_info __user *)arg, &ident,
				sizeof(ident));
	case WDIOC_GETSTATUS:
		return put_user(0, (int __user *)arg);
	case WDIOC_GETBOOTSTATUS:
		if (cpu_is_omap16xx())
			return put_user(__raw_readw(ARM_SYSST),
					(int __user *)arg);
		if (cpu_is_omap24xx())
			return put_user(omap_prcm_get_reset_sources(),
					(int __user *)arg);
<<<<<<< HEAD
=======
		return put_user(0, (int __user *)arg);
>>>>>>> refs/remotes/origin/cm-10.0
	case WDIOC_KEEPALIVE:
		pm_runtime_get_sync(wdev->dev);
		spin_lock(&wdt_lock);
		omap_wdt_ping(wdev);
		spin_unlock(&wdt_lock);
		pm_runtime_put_sync(wdev->dev);
		return 0;
	case WDIOC_SETTIMEOUT:
		if (get_user(new_margin, (int __user *)arg))
			return -EFAULT;
		omap_wdt_adjust_timeout(new_margin);

		pm_runtime_get_sync(wdev->dev);
		spin_lock(&wdt_lock);
		omap_wdt_disable(wdev);
		omap_wdt_set_timeout(wdev);
		omap_wdt_enable(wdev);

		omap_wdt_ping(wdev);
		spin_unlock(&wdt_lock);
		pm_runtime_put_sync(wdev->dev);
		/* Fall */
	case WDIOC_GETTIMEOUT:
		return put_user(timer_margin, (int __user *)arg);
	default:
		return -ENOTTY;
	}
}

static const struct file_operations omap_wdt_fops = {
	.owner = THIS_MODULE,
	.write = omap_wdt_write,
	.unlocked_ioctl = omap_wdt_ioctl,
	.open = omap_wdt_open,
	.release = omap_wdt_release,
	.llseek = no_llseek,
};

static int __devinit omap_wdt_probe(struct platform_device *pdev)
{
	struct resource *res, *mem;
	struct omap_wdt_dev *wdev;
	int ret;

	/* reserve static register mappings */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		ret = -ENOENT;
		goto err_get_resource;
	}

	if (omap_wdt_dev) {
		ret = -EBUSY;
		goto err_busy;
	}

	mem = request_mem_region(res->start, resource_size(res), pdev->name);
	if (!mem) {
		ret = -EBUSY;
		goto err_busy;
	}

	wdev = kzalloc(sizeof(struct omap_wdt_dev), GFP_KERNEL);
	if (!wdev) {
		ret = -ENOMEM;
		goto err_kzalloc;
	}

	wdev->omap_wdt_users = 0;
	wdev->mem = mem;
	wdev->dev = &pdev->dev;

	wdev->base = ioremap(res->start, resource_size(res));
	if (!wdev->base) {
		ret = -ENOMEM;
		goto err_ioremap;
	}

	platform_set_drvdata(pdev, wdev);
=======
	while (readl_relaxed(base + OMAP_WATCHDOG_WPS) & 0x01)
		cpu_relax();

	writel_relaxed((1 << 5) | (PTV << 2), base + OMAP_WATCHDOG_CNTRL);
	while (readl_relaxed(base + OMAP_WATCHDOG_WPS) & 0x01)
		cpu_relax();

	omap_wdt_set_timer(wdev, wdog->timeout);
	omap_wdt_reload(wdev); /* trigger loading of new timeout value */
	omap_wdt_enable(wdev);

	mutex_unlock(&wdev->lock);

	return 0;
}

static int omap_wdt_stop(struct watchdog_device *wdog)
{
	struct omap_wdt_dev *wdev = watchdog_get_drvdata(wdog);

	mutex_lock(&wdev->lock);
	omap_wdt_disable(wdev);
	pm_runtime_put_sync(wdev->dev);
	wdev->omap_wdt_users = false;
	mutex_unlock(&wdev->lock);
	return 0;
}

static int omap_wdt_ping(struct watchdog_device *wdog)
{
	struct omap_wdt_dev *wdev = watchdog_get_drvdata(wdog);

	mutex_lock(&wdev->lock);
	omap_wdt_reload(wdev);
	mutex_unlock(&wdev->lock);

	return 0;
}

static int omap_wdt_set_timeout(struct watchdog_device *wdog,
				unsigned int timeout)
{
	struct omap_wdt_dev *wdev = watchdog_get_drvdata(wdog);

	mutex_lock(&wdev->lock);
	omap_wdt_disable(wdev);
	omap_wdt_set_timer(wdev, timeout);
	omap_wdt_enable(wdev);
	omap_wdt_reload(wdev);
	wdog->timeout = timeout;
	mutex_unlock(&wdev->lock);

	return 0;
}

static const struct watchdog_info omap_wdt_info = {
	.options = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING,
	.identity = "OMAP Watchdog",
};

static const struct watchdog_ops omap_wdt_ops = {
	.owner		= THIS_MODULE,
	.start		= omap_wdt_start,
	.stop		= omap_wdt_stop,
	.ping		= omap_wdt_ping,
	.set_timeout	= omap_wdt_set_timeout,
};

static int omap_wdt_probe(struct platform_device *pdev)
{
	struct omap_wd_timer_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct watchdog_device *omap_wdt;
	struct resource *res, *mem;
	struct omap_wdt_dev *wdev;
	u32 rs;
	int ret;

	omap_wdt = devm_kzalloc(&pdev->dev, sizeof(*omap_wdt), GFP_KERNEL);
	if (!omap_wdt)
		return -ENOMEM;

	/* reserve static register mappings */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENOENT;

	mem = devm_request_mem_region(&pdev->dev, res->start,
				      resource_size(res), pdev->name);
	if (!mem)
		return -EBUSY;

	wdev = devm_kzalloc(&pdev->dev, sizeof(*wdev), GFP_KERNEL);
	if (!wdev)
		return -ENOMEM;

	wdev->omap_wdt_users	= false;
	wdev->mem		= mem;
	wdev->dev		= &pdev->dev;
	wdev->wdt_trgr_pattern	= 0x1234;
	mutex_init(&wdev->lock);

	wdev->base = devm_ioremap(&pdev->dev, res->start, resource_size(res));
	if (!wdev->base)
		return -ENOMEM;

	omap_wdt->info	      = &omap_wdt_info;
	omap_wdt->ops	      = &omap_wdt_ops;
	omap_wdt->min_timeout = TIMER_MARGIN_MIN;
	omap_wdt->max_timeout = TIMER_MARGIN_MAX;

	if (timer_margin >= TIMER_MARGIN_MIN &&
	    timer_margin <= TIMER_MARGIN_MAX)
		omap_wdt->timeout = timer_margin;
	else
		omap_wdt->timeout = TIMER_MARGIN_DEFAULT;

	watchdog_set_drvdata(omap_wdt, wdev);
	watchdog_set_nowayout(omap_wdt, nowayout);

	platform_set_drvdata(pdev, omap_wdt);
>>>>>>> refs/remotes/origin/master

	pm_runtime_enable(wdev->dev);
	pm_runtime_get_sync(wdev->dev);

<<<<<<< HEAD
	omap_wdt_disable(wdev);
	omap_wdt_adjust_timeout(timer_margin);

	wdev->omap_wdt_miscdev.parent = &pdev->dev;
	wdev->omap_wdt_miscdev.minor = WATCHDOG_MINOR;
	wdev->omap_wdt_miscdev.name = "watchdog";
	wdev->omap_wdt_miscdev.fops = &omap_wdt_fops;

	ret = misc_register(&(wdev->omap_wdt_miscdev));
	if (ret)
		goto err_misc;

	pr_info("OMAP Watchdog Timer Rev 0x%02x: initial timeout %d sec\n",
		__raw_readl(wdev->base + OMAP_WATCHDOG_REV) & 0xFF,
		timer_margin);

	pm_runtime_put_sync(wdev->dev);

	omap_wdt_dev = pdev;

	return 0;

err_misc:
<<<<<<< HEAD
=======
	pm_runtime_disable(wdev->dev);
>>>>>>> refs/remotes/origin/cm-10.0
	platform_set_drvdata(pdev, NULL);
	iounmap(wdev->base);

err_ioremap:
	wdev->base = NULL;
	kfree(wdev);

err_kzalloc:
	release_mem_region(res->start, resource_size(res));

err_busy:
err_get_resource:

	return ret;
=======
	if (pdata && pdata->read_reset_sources)
		rs = pdata->read_reset_sources();
	else
		rs = 0;
	omap_wdt->bootstatus = (rs & (1 << OMAP_MPU_WD_RST_SRC_ID_SHIFT)) ?
				WDIOF_CARDRESET : 0;

	omap_wdt_disable(wdev);

	ret = watchdog_register_device(omap_wdt);
	if (ret) {
		pm_runtime_disable(wdev->dev);
		return ret;
	}

	pr_info("OMAP Watchdog Timer Rev 0x%02x: initial timeout %d sec\n",
		readl_relaxed(wdev->base + OMAP_WATCHDOG_REV) & 0xFF,
		omap_wdt->timeout);

	pm_runtime_put_sync(wdev->dev);

	return 0;
>>>>>>> refs/remotes/origin/master
}

static void omap_wdt_shutdown(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct omap_wdt_dev *wdev = platform_get_drvdata(pdev);

	if (wdev->omap_wdt_users) {
		pm_runtime_get_sync(wdev->dev);
		omap_wdt_disable(wdev);
		pm_runtime_put_sync(wdev->dev);
	}
}

static int __devexit omap_wdt_remove(struct platform_device *pdev)
{
	struct omap_wdt_dev *wdev = platform_get_drvdata(pdev);
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

<<<<<<< HEAD
=======
	pm_runtime_disable(wdev->dev);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!res)
		return -ENOENT;

	misc_deregister(&(wdev->omap_wdt_miscdev));
	release_mem_region(res->start, resource_size(res));
	platform_set_drvdata(pdev, NULL);

	iounmap(wdev->base);

	kfree(wdev);
	omap_wdt_dev = NULL;
=======
	struct watchdog_device *wdog = platform_get_drvdata(pdev);
	struct omap_wdt_dev *wdev = watchdog_get_drvdata(wdog);

	mutex_lock(&wdev->lock);
	if (wdev->omap_wdt_users) {
		omap_wdt_disable(wdev);
		pm_runtime_put_sync(wdev->dev);
	}
	mutex_unlock(&wdev->lock);
}

static int omap_wdt_remove(struct platform_device *pdev)
{
	struct watchdog_device *wdog = platform_get_drvdata(pdev);
	struct omap_wdt_dev *wdev = watchdog_get_drvdata(wdog);

	pm_runtime_disable(wdev->dev);
	watchdog_unregister_device(wdog);
>>>>>>> refs/remotes/origin/master

	return 0;
}

#ifdef	CONFIG_PM

/* REVISIT ... not clear this is the best way to handle system suspend; and
 * it's very inappropriate for selective device suspend (e.g. suspending this
 * through sysfs rather than by stopping the watchdog daemon).  Also, this
 * may not play well enough with NOWAYOUT...
 */

static int omap_wdt_suspend(struct platform_device *pdev, pm_message_t state)
{
<<<<<<< HEAD
	struct omap_wdt_dev *wdev = platform_get_drvdata(pdev);

	if (wdev->omap_wdt_users) {
		pm_runtime_get_sync(wdev->dev);
		omap_wdt_disable(wdev);
		pm_runtime_put_sync(wdev->dev);
	}
=======
	struct watchdog_device *wdog = platform_get_drvdata(pdev);
	struct omap_wdt_dev *wdev = watchdog_get_drvdata(wdog);

	mutex_lock(&wdev->lock);
	if (wdev->omap_wdt_users) {
		omap_wdt_disable(wdev);
		pm_runtime_put_sync(wdev->dev);
	}
	mutex_unlock(&wdev->lock);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int omap_wdt_resume(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct omap_wdt_dev *wdev = platform_get_drvdata(pdev);

	if (wdev->omap_wdt_users) {
		pm_runtime_get_sync(wdev->dev);
		omap_wdt_enable(wdev);
		omap_wdt_ping(wdev);
		pm_runtime_put_sync(wdev->dev);
	}
=======
	struct watchdog_device *wdog = platform_get_drvdata(pdev);
	struct omap_wdt_dev *wdev = watchdog_get_drvdata(wdog);

	mutex_lock(&wdev->lock);
	if (wdev->omap_wdt_users) {
		pm_runtime_get_sync(wdev->dev);
		omap_wdt_enable(wdev);
		omap_wdt_reload(wdev);
	}
	mutex_unlock(&wdev->lock);
>>>>>>> refs/remotes/origin/master

	return 0;
}

#else
#define	omap_wdt_suspend	NULL
#define	omap_wdt_resume		NULL
#endif

<<<<<<< HEAD
static struct platform_driver omap_wdt_driver = {
	.probe		= omap_wdt_probe,
	.remove		= __devexit_p(omap_wdt_remove),
=======
static const struct of_device_id omap_wdt_of_match[] = {
	{ .compatible = "ti,omap3-wdt", },
	{},
};
MODULE_DEVICE_TABLE(of, omap_wdt_of_match);

static struct platform_driver omap_wdt_driver = {
	.probe		= omap_wdt_probe,
	.remove		= omap_wdt_remove,
>>>>>>> refs/remotes/origin/master
	.shutdown	= omap_wdt_shutdown,
	.suspend	= omap_wdt_suspend,
	.resume		= omap_wdt_resume,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "omap_wdt",
<<<<<<< HEAD
	},
};

<<<<<<< HEAD
static int __init omap_wdt_init(void)
{
	spin_lock_init(&wdt_lock);
	return platform_driver_register(&omap_wdt_driver);
}

static void __exit omap_wdt_exit(void)
{
	platform_driver_unregister(&omap_wdt_driver);
}

module_init(omap_wdt_init);
module_exit(omap_wdt_exit);
=======
module_platform_driver(omap_wdt_driver);
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_AUTHOR("George G. Davis");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
		.of_match_table = omap_wdt_of_match,
	},
};

module_platform_driver(omap_wdt_driver);

MODULE_AUTHOR("George G. Davis");
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
MODULE_ALIAS("platform:omap_wdt");
