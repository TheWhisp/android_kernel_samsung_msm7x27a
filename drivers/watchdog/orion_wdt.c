/*
 * drivers/watchdog/orion_wdt.c
 *
 * Watchdog driver for Orion/Kirkwood processors
 *
 * Author: Sylver Bruneau <sylver.bruneau@googlemail.com>
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> refs/remotes/origin/cm-10.0
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> refs/remotes/origin/master
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/watchdog.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/spinlock.h>
#include <mach/bridge-regs.h>
#include <plat/orion_wdt.h>
=======
#include <linux/platform_device.h>
#include <linux/watchdog.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/spinlock.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/of.h>
#include <mach/bridge-regs.h>
>>>>>>> refs/remotes/origin/master

/*
 * Watchdog timer block registers.
 */
<<<<<<< HEAD
<<<<<<< HEAD
#define TIMER_CTRL		(TIMER_VIRT_BASE + 0x0000)
#define  WDT_EN			0x0010
#define WDT_VAL			(TIMER_VIRT_BASE + 0x0024)
=======
#define TIMER_CTRL		0x0000
#define  WDT_EN			0x0010
#define WDT_VAL			0x0024
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define TIMER_CTRL		0x0000
#define WDT_EN			0x0010
#define WDT_VAL			0x0024
>>>>>>> refs/remotes/origin/master

#define WDT_MAX_CYCLE_COUNT	0xffffffff
#define WDT_IN_USE		0
#define WDT_OK_TO_CLOSE		1

<<<<<<< HEAD
<<<<<<< HEAD
static int nowayout = WATCHDOG_NOWAYOUT;
static int heartbeat = -1;		/* module parameter (seconds) */
static unsigned int wdt_max_duration;	/* (seconds) */
static unsigned int wdt_tclk;
static unsigned long wdt_status;
static spinlock_t wdt_lock;
=======
static bool nowayout = WATCHDOG_NOWAYOUT;
static int heartbeat = -1;		/* module parameter (seconds) */
static unsigned int wdt_max_duration;	/* (seconds) */
static unsigned int wdt_tclk;
static void __iomem *wdt_reg;
static unsigned long wdt_status;
static DEFINE_SPINLOCK(wdt_lock);
>>>>>>> refs/remotes/origin/cm-10.0

static void orion_wdt_ping(void)
=======
#define WDT_RESET_OUT_EN	BIT(1)
#define WDT_INT_REQ		BIT(3)

static bool nowayout = WATCHDOG_NOWAYOUT;
static int heartbeat = -1;		/* module parameter (seconds) */
static unsigned int wdt_max_duration;	/* (seconds) */
static struct clk *clk;
static unsigned int wdt_tclk;
static void __iomem *wdt_reg;
static DEFINE_SPINLOCK(wdt_lock);

static int orion_wdt_ping(struct watchdog_device *wdt_dev)
>>>>>>> refs/remotes/origin/master
{
	spin_lock(&wdt_lock);

	/* Reload watchdog duration */
<<<<<<< HEAD
<<<<<<< HEAD
	writel(wdt_tclk * heartbeat, WDT_VAL);
=======
	writel(wdt_tclk * heartbeat, wdt_reg + WDT_VAL);
>>>>>>> refs/remotes/origin/cm-10.0

	spin_unlock(&wdt_lock);
}

static void orion_wdt_enable(void)
=======
	writel(wdt_tclk * wdt_dev->timeout, wdt_reg + WDT_VAL);

	spin_unlock(&wdt_lock);
	return 0;
}

static int orion_wdt_start(struct watchdog_device *wdt_dev)
>>>>>>> refs/remotes/origin/master
{
	u32 reg;

	spin_lock(&wdt_lock);

	/* Set watchdog duration */
<<<<<<< HEAD
<<<<<<< HEAD
	writel(wdt_tclk * heartbeat, WDT_VAL);
=======
	writel(wdt_tclk * heartbeat, wdt_reg + WDT_VAL);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Clear watchdog timer interrupt */
	reg = readl(BRIDGE_CAUSE);
	reg &= ~WDT_INT_REQ;
	writel(reg, BRIDGE_CAUSE);

	/* Enable watchdog timer */
<<<<<<< HEAD
	reg = readl(TIMER_CTRL);
	reg |= WDT_EN;
	writel(reg, TIMER_CTRL);
=======
	reg = readl(wdt_reg + TIMER_CTRL);
	reg |= WDT_EN;
	writel(reg, wdt_reg + TIMER_CTRL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	writel(wdt_tclk * wdt_dev->timeout, wdt_reg + WDT_VAL);

	/* Clear watchdog timer interrupt */
	writel(~WDT_INT_REQ, BRIDGE_CAUSE);

	/* Enable watchdog timer */
	reg = readl(wdt_reg + TIMER_CTRL);
	reg |= WDT_EN;
	writel(reg, wdt_reg + TIMER_CTRL);
>>>>>>> refs/remotes/origin/master

	/* Enable reset on watchdog */
	reg = readl(RSTOUTn_MASK);
	reg |= WDT_RESET_OUT_EN;
	writel(reg, RSTOUTn_MASK);

	spin_unlock(&wdt_lock);
<<<<<<< HEAD
}

static void orion_wdt_disable(void)
=======
	return 0;
}

static int orion_wdt_stop(struct watchdog_device *wdt_dev)
>>>>>>> refs/remotes/origin/master
{
	u32 reg;

	spin_lock(&wdt_lock);

	/* Disable reset on watchdog */
	reg = readl(RSTOUTn_MASK);
	reg &= ~WDT_RESET_OUT_EN;
	writel(reg, RSTOUTn_MASK);

	/* Disable watchdog timer */
<<<<<<< HEAD
<<<<<<< HEAD
	reg = readl(TIMER_CTRL);
	reg &= ~WDT_EN;
	writel(reg, TIMER_CTRL);
=======
	reg = readl(wdt_reg + TIMER_CTRL);
	reg &= ~WDT_EN;
	writel(reg, wdt_reg + TIMER_CTRL);
>>>>>>> refs/remotes/origin/cm-10.0

	spin_unlock(&wdt_lock);
}

static int orion_wdt_get_timeleft(int *time_left)
{
	spin_lock(&wdt_lock);
<<<<<<< HEAD
	*time_left = readl(WDT_VAL) / wdt_tclk;
=======
	*time_left = readl(wdt_reg + WDT_VAL) / wdt_tclk;
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock(&wdt_lock);
	return 0;
}

static int orion_wdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(WDT_IN_USE, &wdt_status))
		return -EBUSY;
	clear_bit(WDT_OK_TO_CLOSE, &wdt_status);
	orion_wdt_enable();
	return nonseekable_open(inode, file);
}

static ssize_t orion_wdt_write(struct file *file, const char *data,
					size_t len, loff_t *ppos)
{
	if (len) {
		if (!nowayout) {
			size_t i;

			clear_bit(WDT_OK_TO_CLOSE, &wdt_status);
			for (i = 0; i != len; i++) {
				char c;

				if (get_user(c, data + i))
					return -EFAULT;
				if (c == 'V')
					set_bit(WDT_OK_TO_CLOSE, &wdt_status);
			}
		}
		orion_wdt_ping();
	}
	return len;
}

static int orion_wdt_settimeout(int new_time)
{
	if ((new_time <= 0) || (new_time > wdt_max_duration))
		return -EINVAL;

	/* Set new watchdog time to be used when
	 * orion_wdt_enable() or orion_wdt_ping() is called. */
	heartbeat = new_time;
	return 0;
}

static const struct watchdog_info ident = {
	.options	= WDIOF_MAGICCLOSE | WDIOF_SETTIMEOUT |
			  WDIOF_KEEPALIVEPING,
	.identity	= "Orion Watchdog",
};

static long orion_wdt_ioctl(struct file *file, unsigned int cmd,
				unsigned long arg)
{
	int ret = -ENOTTY;
	int time;

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
		orion_wdt_ping();
		ret = 0;
		break;

	case WDIOC_SETTIMEOUT:
		ret = get_user(time, (int *)arg);
		if (ret)
			break;

		if (orion_wdt_settimeout(time)) {
			ret = -EINVAL;
			break;
		}
		orion_wdt_ping();
		/* Fall through */

	case WDIOC_GETTIMEOUT:
		ret = put_user(heartbeat, (int *)arg);
		break;

	case WDIOC_GETTIMELEFT:
		if (orion_wdt_get_timeleft(&time)) {
			ret = -EINVAL;
			break;
		}
		ret = put_user(time, (int *)arg);
		break;
	}
	return ret;
}

static int orion_wdt_release(struct inode *inode, struct file *file)
{
	if (test_bit(WDT_OK_TO_CLOSE, &wdt_status))
		orion_wdt_disable();
	else
<<<<<<< HEAD
		printk(KERN_CRIT "WATCHDOG: Device closed unexpectedly - "
					"timer will not stop\n");
=======
		pr_crit("Device closed unexpectedly - timer will not stop\n");
>>>>>>> refs/remotes/origin/cm-10.0
	clear_bit(WDT_IN_USE, &wdt_status);
	clear_bit(WDT_OK_TO_CLOSE, &wdt_status);

	return 0;
}


static const struct file_operations orion_wdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.write		= orion_wdt_write,
	.unlocked_ioctl	= orion_wdt_ioctl,
	.open		= orion_wdt_open,
	.release	= orion_wdt_release,
};

static struct miscdevice orion_wdt_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &orion_wdt_fops,
};

static int __devinit orion_wdt_probe(struct platform_device *pdev)
{
	struct orion_wdt_platform_data *pdata = pdev->dev.platform_data;
<<<<<<< HEAD
=======
	struct resource *res;
>>>>>>> refs/remotes/origin/cm-10.0
	int ret;

	if (pdata) {
		wdt_tclk = pdata->tclk;
	} else {
<<<<<<< HEAD
		printk(KERN_ERR "Orion Watchdog misses platform data\n");
		return -ENODEV;
	}

=======
		pr_err("misses platform data\n");
		return -ENODEV;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	wdt_reg = ioremap(res->start, resource_size(res));

>>>>>>> refs/remotes/origin/cm-10.0
	if (orion_wdt_miscdev.parent)
		return -EBUSY;
	orion_wdt_miscdev.parent = &pdev->dev;

	wdt_max_duration = WDT_MAX_CYCLE_COUNT / wdt_tclk;
	if (orion_wdt_settimeout(heartbeat))
		heartbeat = wdt_max_duration;

	ret = misc_register(&orion_wdt_miscdev);
	if (ret)
		return ret;

<<<<<<< HEAD
	printk(KERN_INFO "Orion Watchdog Timer: Initial timeout %d sec%s\n",
				heartbeat, nowayout ? ", nowayout" : "");
=======
	pr_info("Initial timeout %d sec%s\n",
		heartbeat, nowayout ? ", nowayout" : "");
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static int __devexit orion_wdt_remove(struct platform_device *pdev)
{
	int ret;

	if (test_bit(WDT_IN_USE, &wdt_status)) {
		orion_wdt_disable();
		clear_bit(WDT_IN_USE, &wdt_status);
	}

	ret = misc_deregister(&orion_wdt_miscdev);
	if (!ret)
		orion_wdt_miscdev.parent = NULL;

	return ret;
=======
	reg = readl(wdt_reg + TIMER_CTRL);
	reg &= ~WDT_EN;
	writel(reg, wdt_reg + TIMER_CTRL);

	spin_unlock(&wdt_lock);
	return 0;
}

static unsigned int orion_wdt_get_timeleft(struct watchdog_device *wdt_dev)
{
	unsigned int time_left;

	spin_lock(&wdt_lock);
	time_left = readl(wdt_reg + WDT_VAL) / wdt_tclk;
	spin_unlock(&wdt_lock);

	return time_left;
}

static int orion_wdt_set_timeout(struct watchdog_device *wdt_dev,
				 unsigned int timeout)
{
	wdt_dev->timeout = timeout;
	return 0;
}

static const struct watchdog_info orion_wdt_info = {
	.options = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
	.identity = "Orion Watchdog",
};

static const struct watchdog_ops orion_wdt_ops = {
	.owner = THIS_MODULE,
	.start = orion_wdt_start,
	.stop = orion_wdt_stop,
	.ping = orion_wdt_ping,
	.set_timeout = orion_wdt_set_timeout,
	.get_timeleft = orion_wdt_get_timeleft,
};

static struct watchdog_device orion_wdt = {
	.info = &orion_wdt_info,
	.ops = &orion_wdt_ops,
	.min_timeout = 1,
};

static int orion_wdt_probe(struct platform_device *pdev)
{
	struct resource *res;
	int ret;

	clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(clk)) {
		dev_err(&pdev->dev, "Orion Watchdog missing clock\n");
		return -ENODEV;
	}
	clk_prepare_enable(clk);
	wdt_tclk = clk_get_rate(clk);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;
	wdt_reg = devm_ioremap(&pdev->dev, res->start, resource_size(res));
	if (!wdt_reg)
		return -ENOMEM;

	wdt_max_duration = WDT_MAX_CYCLE_COUNT / wdt_tclk;

	orion_wdt.timeout = wdt_max_duration;
	orion_wdt.max_timeout = wdt_max_duration;
	watchdog_init_timeout(&orion_wdt, heartbeat, &pdev->dev);

	watchdog_set_nowayout(&orion_wdt, nowayout);
	ret = watchdog_register_device(&orion_wdt);
	if (ret) {
		clk_disable_unprepare(clk);
		return ret;
	}

	pr_info("Initial timeout %d sec%s\n",
		orion_wdt.timeout, nowayout ? ", nowayout" : "");
	return 0;
}

static int orion_wdt_remove(struct platform_device *pdev)
{
	watchdog_unregister_device(&orion_wdt);
	clk_disable_unprepare(clk);
	return 0;
>>>>>>> refs/remotes/origin/master
}

static void orion_wdt_shutdown(struct platform_device *pdev)
{
<<<<<<< HEAD
	if (test_bit(WDT_IN_USE, &wdt_status))
		orion_wdt_disable();
}

static struct platform_driver orion_wdt_driver = {
	.probe		= orion_wdt_probe,
	.remove		= __devexit_p(orion_wdt_remove),
=======
	orion_wdt_stop(&orion_wdt);
}

static const struct of_device_id orion_wdt_of_match_table[] = {
	{ .compatible = "marvell,orion-wdt", },
	{},
};
MODULE_DEVICE_TABLE(of, orion_wdt_of_match_table);

static struct platform_driver orion_wdt_driver = {
	.probe		= orion_wdt_probe,
	.remove		= orion_wdt_remove,
>>>>>>> refs/remotes/origin/master
	.shutdown	= orion_wdt_shutdown,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "orion_wdt",
<<<<<<< HEAD
	},
};

<<<<<<< HEAD
static int __init orion_wdt_init(void)
{
	spin_lock_init(&wdt_lock);
	return platform_driver_register(&orion_wdt_driver);
}

static void __exit orion_wdt_exit(void)
{
	platform_driver_unregister(&orion_wdt_driver);
}

module_init(orion_wdt_init);
module_exit(orion_wdt_exit);
=======
module_platform_driver(orion_wdt_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.of_match_table = orion_wdt_of_match_table,
	},
};

module_platform_driver(orion_wdt_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Sylver Bruneau <sylver.bruneau@googlemail.com>");
MODULE_DESCRIPTION("Orion Processor Watchdog");

module_param(heartbeat, int, 0);
MODULE_PARM_DESC(heartbeat, "Initial watchdog heartbeat in seconds");

<<<<<<< HEAD
<<<<<<< HEAD
module_param(nowayout, int, 0);
=======
module_param(nowayout, bool, 0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_param(nowayout, bool, 0);
>>>>>>> refs/remotes/origin/master
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default="
				__MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
MODULE_ALIAS("platform:orion_wdt");
>>>>>>> refs/remotes/origin/master
