/*
 *  Copyright (C) 2010, Paul Cercueil <paul@crapouillou.net>
 *  JZ4740 Watchdog driver
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under  the terms of the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/kernel.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
=======
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/init.h>
#include <linux/platform_device.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/watchdog.h>
#include <linux/init.h>
#include <linux/platform_device.h>
>>>>>>> refs/remotes/origin/master
#include <linux/io.h>
#include <linux/device.h>
#include <linux/clk.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/err.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/err.h>
>>>>>>> refs/remotes/origin/master

#include <asm/mach-jz4740/timer.h>

#define JZ_REG_WDT_TIMER_DATA     0x0
#define JZ_REG_WDT_COUNTER_ENABLE 0x4
#define JZ_REG_WDT_TIMER_COUNTER  0x8
#define JZ_REG_WDT_TIMER_CONTROL  0xC

#define JZ_WDT_CLOCK_PCLK 0x1
#define JZ_WDT_CLOCK_RTC  0x2
#define JZ_WDT_CLOCK_EXT  0x4

<<<<<<< HEAD
<<<<<<< HEAD
#define WDT_IN_USE        0
#define WDT_OK_TO_CLOSE   1

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define JZ_WDT_CLOCK_DIV_SHIFT   3

#define JZ_WDT_CLOCK_DIV_1    (0 << JZ_WDT_CLOCK_DIV_SHIFT)
#define JZ_WDT_CLOCK_DIV_4    (1 << JZ_WDT_CLOCK_DIV_SHIFT)
#define JZ_WDT_CLOCK_DIV_16   (2 << JZ_WDT_CLOCK_DIV_SHIFT)
#define JZ_WDT_CLOCK_DIV_64   (3 << JZ_WDT_CLOCK_DIV_SHIFT)
#define JZ_WDT_CLOCK_DIV_256  (4 << JZ_WDT_CLOCK_DIV_SHIFT)
#define JZ_WDT_CLOCK_DIV_1024 (5 << JZ_WDT_CLOCK_DIV_SHIFT)

#define DEFAULT_HEARTBEAT 5
#define MAX_HEARTBEAT     2048

<<<<<<< HEAD
<<<<<<< HEAD
static struct {
	void __iomem *base;
	struct resource	*mem;
	struct clk *rtc_clk;
	unsigned long status;
} jz4740_wdt;

static int heartbeat = DEFAULT_HEARTBEAT;


static void jz4740_wdt_service(void)
{
	writew(0x0, jz4740_wdt.base + JZ_REG_WDT_TIMER_COUNTER);
}

static void jz4740_wdt_set_heartbeat(int new_heartbeat)
{
=======
=======
>>>>>>> refs/remotes/origin/master
static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout,
		 "Watchdog cannot be stopped once started (default="
		 __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

static unsigned int heartbeat = DEFAULT_HEARTBEAT;
module_param(heartbeat, uint, 0);
MODULE_PARM_DESC(heartbeat,
		"Watchdog heartbeat period in seconds from 1 to "
		__MODULE_STRING(MAX_HEARTBEAT) ", default "
		__MODULE_STRING(DEFAULT_HEARTBEAT));

struct jz4740_wdt_drvdata {
	struct watchdog_device wdt;
	void __iomem *base;
	struct clk *rtc_clk;
};

static int jz4740_wdt_ping(struct watchdog_device *wdt_dev)
{
	struct jz4740_wdt_drvdata *drvdata = watchdog_get_drvdata(wdt_dev);

	writew(0x0, drvdata->base + JZ_REG_WDT_TIMER_COUNTER);
	return 0;
}

static int jz4740_wdt_set_timeout(struct watchdog_device *wdt_dev,
				    unsigned int new_timeout)
{
	struct jz4740_wdt_drvdata *drvdata = watchdog_get_drvdata(wdt_dev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	unsigned int rtc_clk_rate;
	unsigned int timeout_value;
	unsigned short clock_div = JZ_WDT_CLOCK_DIV_1;

<<<<<<< HEAD
<<<<<<< HEAD
	heartbeat = new_heartbeat;

	rtc_clk_rate = clk_get_rate(jz4740_wdt.rtc_clk);

	timeout_value = rtc_clk_rate * heartbeat;
=======
	rtc_clk_rate = clk_get_rate(drvdata->rtc_clk);

	timeout_value = rtc_clk_rate * new_timeout;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rtc_clk_rate = clk_get_rate(drvdata->rtc_clk);

	timeout_value = rtc_clk_rate * new_timeout;
>>>>>>> refs/remotes/origin/master
	while (timeout_value > 0xffff) {
		if (clock_div == JZ_WDT_CLOCK_DIV_1024) {
			/* Requested timeout too high;
			* use highest possible value. */
			timeout_value = 0xffff;
			break;
		}
		timeout_value >>= 2;
		clock_div += (1 << JZ_WDT_CLOCK_DIV_SHIFT);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	writeb(0x0, jz4740_wdt.base + JZ_REG_WDT_COUNTER_ENABLE);
	writew(clock_div, jz4740_wdt.base + JZ_REG_WDT_TIMER_CONTROL);

	writew((u16)timeout_value, jz4740_wdt.base + JZ_REG_WDT_TIMER_DATA);
	writew(0x0, jz4740_wdt.base + JZ_REG_WDT_TIMER_COUNTER);
	writew(clock_div | JZ_WDT_CLOCK_RTC,
		jz4740_wdt.base + JZ_REG_WDT_TIMER_CONTROL);

	writeb(0x1, jz4740_wdt.base + JZ_REG_WDT_COUNTER_ENABLE);
}

static void jz4740_wdt_enable(void)
{
	jz4740_timer_enable_watchdog();
	jz4740_wdt_set_heartbeat(heartbeat);
}

static void jz4740_wdt_disable(void)
{
	jz4740_timer_disable_watchdog();
	writeb(0x0, jz4740_wdt.base + JZ_REG_WDT_COUNTER_ENABLE);
}

static int jz4740_wdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(WDT_IN_USE, &jz4740_wdt.status))
		return -EBUSY;

	jz4740_wdt_enable();

	return nonseekable_open(inode, file);
}

static ssize_t jz4740_wdt_write(struct file *file, const char *data,
		size_t len, loff_t *ppos)
{
	if (len) {
		size_t i;

		clear_bit(WDT_OK_TO_CLOSE, &jz4740_wdt.status);
		for (i = 0; i != len; i++) {
			char c;

			if (get_user(c, data + i))
				return -EFAULT;

			if (c == 'V')
				set_bit(WDT_OK_TO_CLOSE, &jz4740_wdt.status);
		}
		jz4740_wdt_service();
	}

	return len;
}

static const struct watchdog_info ident = {
	.options = WDIOF_KEEPALIVEPING,
	.identity = "jz4740 Watchdog",
};

static long jz4740_wdt_ioctl(struct file *file,
					unsigned int cmd, unsigned long arg)
{
	int ret = -ENOTTY;
	int heartbeat_seconds;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		ret = copy_to_user((struct watchdog_info *)arg, &ident,
				sizeof(ident)) ? -EFAULT : 0;
		break;

	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		ret = put_user(0, (int *)arg);
		break;

	case WDIOC_KEEPALIVE:
		jz4740_wdt_service();
		return 0;

	case WDIOC_SETTIMEOUT:
		if (get_user(heartbeat_seconds, (int __user *)arg))
			return -EFAULT;

		jz4740_wdt_set_heartbeat(heartbeat_seconds);
		return 0;

	case WDIOC_GETTIMEOUT:
		return put_user(heartbeat, (int *)arg);

	default:
		break;
	}

	return ret;
}

static int jz4740_wdt_release(struct inode *inode, struct file *file)
{
	jz4740_wdt_service();

	if (test_and_clear_bit(WDT_OK_TO_CLOSE, &jz4740_wdt.status))
		jz4740_wdt_disable();

	clear_bit(WDT_IN_USE, &jz4740_wdt.status);
	return 0;
}

static const struct file_operations jz4740_wdt_fops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.write = jz4740_wdt_write,
	.unlocked_ioctl = jz4740_wdt_ioctl,
	.open = jz4740_wdt_open,
	.release = jz4740_wdt_release,
};

static struct miscdevice jz4740_wdt_miscdev = {
	.minor = WATCHDOG_MINOR,
	.name = "watchdog",
	.fops = &jz4740_wdt_fops,
=======
=======
>>>>>>> refs/remotes/origin/master
	writeb(0x0, drvdata->base + JZ_REG_WDT_COUNTER_ENABLE);
	writew(clock_div, drvdata->base + JZ_REG_WDT_TIMER_CONTROL);

	writew((u16)timeout_value, drvdata->base + JZ_REG_WDT_TIMER_DATA);
	writew(0x0, drvdata->base + JZ_REG_WDT_TIMER_COUNTER);
	writew(clock_div | JZ_WDT_CLOCK_RTC,
		drvdata->base + JZ_REG_WDT_TIMER_CONTROL);

	writeb(0x1, drvdata->base + JZ_REG_WDT_COUNTER_ENABLE);

	wdt_dev->timeout = new_timeout;
	return 0;
}

static int jz4740_wdt_start(struct watchdog_device *wdt_dev)
{
	jz4740_timer_enable_watchdog();
	jz4740_wdt_set_timeout(wdt_dev, wdt_dev->timeout);

	return 0;
}

static int jz4740_wdt_stop(struct watchdog_device *wdt_dev)
{
	struct jz4740_wdt_drvdata *drvdata = watchdog_get_drvdata(wdt_dev);

	jz4740_timer_disable_watchdog();
	writeb(0x0, drvdata->base + JZ_REG_WDT_COUNTER_ENABLE);

	return 0;
}

static const struct watchdog_info jz4740_wdt_info = {
	.options = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
	.identity = "jz4740 Watchdog",
};

static const struct watchdog_ops jz4740_wdt_ops = {
	.owner = THIS_MODULE,
	.start = jz4740_wdt_start,
	.stop = jz4740_wdt_stop,
	.ping = jz4740_wdt_ping,
	.set_timeout = jz4740_wdt_set_timeout,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
};

static int __devinit jz4740_wdt_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	int ret = 0, size;
	struct resource *res;
	struct device *dev = &pdev->dev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		dev_err(dev, "failed to get memory region resource\n");
		return -ENXIO;
	}

	size = resource_size(res);
	jz4740_wdt.mem = request_mem_region(res->start, size, pdev->name);
	if (jz4740_wdt.mem == NULL) {
		dev_err(dev, "failed to get memory region\n");
		return -EBUSY;
	}

	jz4740_wdt.base = ioremap_nocache(res->start, size);
	if (jz4740_wdt.base == NULL) {
		dev_err(dev, "failed to map memory region\n");
		ret = -EBUSY;
		goto err_release_region;
	}

	jz4740_wdt.rtc_clk = clk_get(NULL, "rtc");
	if (IS_ERR(jz4740_wdt.rtc_clk)) {
		dev_err(dev, "cannot find RTC clock\n");
		ret = PTR_ERR(jz4740_wdt.rtc_clk);
		goto err_iounmap;
	}

	ret = misc_register(&jz4740_wdt_miscdev);
	if (ret < 0) {
		dev_err(dev, "cannot register misc device\n");
		goto err_disable_clk;
	}

	return 0;

err_disable_clk:
	clk_put(jz4740_wdt.rtc_clk);
err_iounmap:
	iounmap(jz4740_wdt.base);
err_release_region:
	release_mem_region(jz4740_wdt.mem->start,
			resource_size(jz4740_wdt.mem));
	return ret;
}


static int __devexit jz4740_wdt_remove(struct platform_device *pdev)
{
	jz4740_wdt_disable();
	misc_deregister(&jz4740_wdt_miscdev);
	clk_put(jz4740_wdt.rtc_clk);

	iounmap(jz4740_wdt.base);
	jz4740_wdt.base = NULL;

	release_mem_region(jz4740_wdt.mem->start,
				resource_size(jz4740_wdt.mem));
	jz4740_wdt.mem = NULL;
=======
=======
};

static int jz4740_wdt_probe(struct platform_device *pdev)
{
>>>>>>> refs/remotes/origin/master
	struct jz4740_wdt_drvdata *drvdata;
	struct watchdog_device *jz4740_wdt;
	struct resource	*res;
	int ret;

	drvdata = devm_kzalloc(&pdev->dev, sizeof(struct jz4740_wdt_drvdata),
			       GFP_KERNEL);
	if (!drvdata) {
		dev_err(&pdev->dev, "Unable to alloacate watchdog device\n");
		return -ENOMEM;
	}

	if (heartbeat < 1 || heartbeat > MAX_HEARTBEAT)
		heartbeat = DEFAULT_HEARTBEAT;

	jz4740_wdt = &drvdata->wdt;
	jz4740_wdt->info = &jz4740_wdt_info;
	jz4740_wdt->ops = &jz4740_wdt_ops;
	jz4740_wdt->timeout = heartbeat;
	jz4740_wdt->min_timeout = 1;
	jz4740_wdt->max_timeout = MAX_HEARTBEAT;
	watchdog_set_nowayout(jz4740_wdt, nowayout);
	watchdog_set_drvdata(jz4740_wdt, drvdata);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
<<<<<<< HEAD
	drvdata->base = devm_request_and_ioremap(&pdev->dev, res);
	if (drvdata->base == NULL) {
		ret = -EBUSY;
		goto err_out;
	}

	drvdata->rtc_clk = clk_get(NULL, "rtc");
=======
	drvdata->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(drvdata->base)) {
		ret = PTR_ERR(drvdata->base);
		goto err_out;
	}

	drvdata->rtc_clk = clk_get(&pdev->dev, "rtc");
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(drvdata->rtc_clk)) {
		dev_err(&pdev->dev, "cannot find RTC clock\n");
		ret = PTR_ERR(drvdata->rtc_clk);
		goto err_out;
	}

	ret = watchdog_register_device(&drvdata->wdt);
	if (ret < 0)
		goto err_disable_clk;

	platform_set_drvdata(pdev, drvdata);
	return 0;

err_disable_clk:
	clk_put(drvdata->rtc_clk);
err_out:
	return ret;
}

<<<<<<< HEAD
static int __devexit jz4740_wdt_remove(struct platform_device *pdev)
=======
static int jz4740_wdt_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct jz4740_wdt_drvdata *drvdata = platform_get_drvdata(pdev);

	jz4740_wdt_stop(&drvdata->wdt);
	watchdog_unregister_device(&drvdata->wdt);
	clk_put(drvdata->rtc_clk);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct platform_driver jz4740_wdt_driver = {
	.probe = jz4740_wdt_probe,
	.remove = __devexit_p(jz4740_wdt_remove),
=======
static struct platform_driver jz4740_wdt_driver = {
	.probe = jz4740_wdt_probe,
	.remove = jz4740_wdt_remove,
>>>>>>> refs/remotes/origin/master
	.driver = {
		.name = "jz4740-wdt",
		.owner	= THIS_MODULE,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD

static int __init jz4740_wdt_init(void)
{
	return platform_driver_register(&jz4740_wdt_driver);
}
module_init(jz4740_wdt_init);

static void __exit jz4740_wdt_exit(void)
{
	platform_driver_unregister(&jz4740_wdt_driver);
}
module_exit(jz4740_wdt_exit);

MODULE_AUTHOR("Paul Cercueil <paul@crapouillou.net>");
MODULE_DESCRIPTION("jz4740 Watchdog Driver");

module_param(heartbeat, int, 0);
MODULE_PARM_DESC(heartbeat,
		"Watchdog heartbeat period in seconds from 1 to "
		__MODULE_STRING(MAX_HEARTBEAT) ", default "
		__MODULE_STRING(DEFAULT_HEARTBEAT));

=======
=======
>>>>>>> refs/remotes/origin/master
module_platform_driver(jz4740_wdt_driver);

MODULE_AUTHOR("Paul Cercueil <paul@crapouillou.net>");
MODULE_DESCRIPTION("jz4740 Watchdog Driver");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
MODULE_ALIAS("platform:jz4740-wdt");
