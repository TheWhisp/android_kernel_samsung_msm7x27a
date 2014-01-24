/*
 * Watchdog driver for Atmel AT91SAM9x processors.
 *
 * Copyright (C) 2008 Renaud CERRATO r.cerrato@til-technologies.fr
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

/*
 * The Watchdog Timer Mode Register can be only written to once. If the
 * timeout need to be set from Linux, be sure that the bootstrap or the
 * bootloader doesn't write to this register.
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
>>>>>>> refs/remotes/origin/master
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/watchdog.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/bitops.h>
#include <linux/uaccess.h>
<<<<<<< HEAD

<<<<<<< HEAD
#include <mach/at91_wdt.h>

#define DRV_NAME "AT91SAM9 Watchdog"

=======
=======
#include <linux/of.h>

>>>>>>> refs/remotes/origin/master
#include "at91sam9_wdt.h"

#define DRV_NAME "AT91SAM9 Watchdog"

#define wdt_read(field) \
	__raw_readl(at91wdt_private.base + field)
#define wdt_write(field, val) \
	__raw_writel((val), at91wdt_private.base + field)

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* AT91SAM9 watchdog runs a 12bit counter @ 256Hz,
 * use this to convert a watchdog
 * value from/to milliseconds.
 */
#define ms_to_ticks(t)	(((t << 8) / 1000) - 1)
#define ticks_to_ms(t)	(((t + 1) * 1000) >> 8)

/* Hardware timeout in seconds */
#define WDT_HW_TIMEOUT 2

/* Timer heartbeat (500ms) */
#define WDT_TIMEOUT	(HZ/2)

/* User land timeout */
#define WDT_HEARTBEAT 15
<<<<<<< HEAD
static int heartbeat = WDT_HEARTBEAT;
=======
static int heartbeat;
>>>>>>> refs/remotes/origin/master
module_param(heartbeat, int, 0);
MODULE_PARM_DESC(heartbeat, "Watchdog heartbeats in seconds. "
	"(default = " __MODULE_STRING(WDT_HEARTBEAT) ")");

<<<<<<< HEAD
<<<<<<< HEAD
static int nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, int, 0);
=======
static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
>>>>>>> refs/remotes/origin/cm-10.0
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started "
	"(default=" __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

static void at91_ping(unsigned long data);

static struct {
<<<<<<< HEAD
=======
	void __iomem *base;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long next_heartbeat;	/* the next_heartbeat for the timer */
	unsigned long open;
	char expect_close;
=======
static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started "
	"(default=" __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

static struct watchdog_device at91_wdt_dev;
static void at91_ping(unsigned long data);

static struct {
	void __iomem *base;
	unsigned long next_heartbeat;	/* the next_heartbeat for the timer */
>>>>>>> refs/remotes/origin/master
	struct timer_list timer;	/* The timer that pings the watchdog */
} at91wdt_private;

/* ......................................................................... */

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
/*
 * Reload the watchdog timer.  (ie, pat the watchdog)
 */
static inline void at91_wdt_reset(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	at91_sys_write(AT91_WDT_CR, AT91_WDT_KEY | AT91_WDT_WDRSTT);
=======
	wdt_write(AT91_WDT_CR, AT91_WDT_KEY | AT91_WDT_WDRSTT);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	wdt_write(AT91_WDT_CR, AT91_WDT_KEY | AT91_WDT_WDRSTT);
>>>>>>> refs/remotes/origin/master
}

/*
 * Timer tick
 */
static void at91_ping(unsigned long data)
{
	if (time_before(jiffies, at91wdt_private.next_heartbeat) ||
<<<<<<< HEAD
			(!nowayout && !at91wdt_private.open)) {
		at91_wdt_reset();
		mod_timer(&at91wdt_private.timer, jiffies + WDT_TIMEOUT);
	} else
<<<<<<< HEAD
		printk(KERN_CRIT DRV_NAME": I will reset your machine !\n");
=======
		pr_crit("I will reset your machine !\n");
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Watchdog device is opened, and watchdog starts running.
 */
static int at91_wdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(0, &at91wdt_private.open))
		return -EBUSY;

	at91wdt_private.next_heartbeat = jiffies + heartbeat * HZ;
	mod_timer(&at91wdt_private.timer, jiffies + WDT_TIMEOUT);

	return nonseekable_open(inode, file);
}

/*
 * Close the watchdog device.
 */
static int at91_wdt_close(struct inode *inode, struct file *file)
{
	clear_bit(0, &at91wdt_private.open);

	/* stop internal ping */
	if (!at91wdt_private.expect_close)
		del_timer(&at91wdt_private.timer);

	at91wdt_private.expect_close = 0;
=======
	    (!watchdog_active(&at91_wdt_dev))) {
		at91_wdt_reset();
		mod_timer(&at91wdt_private.timer, jiffies + WDT_TIMEOUT);
	} else
		pr_crit("I will reset your machine !\n");
}

static int at91_wdt_ping(struct watchdog_device *wdd)
{
	/* calculate when the next userspace timeout will be */
	at91wdt_private.next_heartbeat = jiffies + wdd->timeout * HZ;
	return 0;
}

static int at91_wdt_start(struct watchdog_device *wdd)
{
	/* calculate the next userspace timeout and modify the timer */
	at91_wdt_ping(wdd);
	mod_timer(&at91wdt_private.timer, jiffies + WDT_TIMEOUT);
	return 0;
}

static int at91_wdt_stop(struct watchdog_device *wdd)
{
	/* The watchdog timer hardware can not be stopped... */
	return 0;
}

static int at91_wdt_set_timeout(struct watchdog_device *wdd, unsigned int new_timeout)
{
	wdd->timeout = new_timeout;
>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
 * Set the watchdog time interval in 1/256Hz (write-once)
 * Counter is 12 bit.
 */
static int at91_wdt_settimeout(unsigned int timeout)
{
	unsigned int reg;
	unsigned int mr;

	/* Check if disabled */
<<<<<<< HEAD
<<<<<<< HEAD
	mr = at91_sys_read(AT91_WDT_MR);
	if (mr & AT91_WDT_WDDIS) {
		printk(KERN_ERR DRV_NAME": sorry, watchdog is disabled\n");
=======
	mr = wdt_read(AT91_WDT_MR);
	if (mr & AT91_WDT_WDDIS) {
		pr_err("sorry, watchdog is disabled\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mr = wdt_read(AT91_WDT_MR);
	if (mr & AT91_WDT_WDDIS) {
		pr_err("sorry, watchdog is disabled\n");
>>>>>>> refs/remotes/origin/master
		return -EIO;
	}

	/*
	 * All counting occurs at SLOW_CLOCK / 128 = 256 Hz
	 *
	 * Since WDV is a 12-bit counter, the maximum period is
	 * 4096 / 256 = 16 seconds.
	 */
	reg = AT91_WDT_WDRSTEN	/* causes watchdog reset */
		/* | AT91_WDT_WDRPROC	causes processor reset only */
		| AT91_WDT_WDDBGHLT	/* disabled in debug mode */
		| AT91_WDT_WDD		/* restart at any time */
		| (timeout & AT91_WDT_WDV);  /* timer value */
<<<<<<< HEAD
<<<<<<< HEAD
	at91_sys_write(AT91_WDT_MR, reg);
=======
	wdt_write(AT91_WDT_MR, reg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	wdt_write(AT91_WDT_MR, reg);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
=======
/* ......................................................................... */

>>>>>>> refs/remotes/origin/master
static const struct watchdog_info at91_wdt_info = {
	.identity	= DRV_NAME,
	.options	= WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING |
						WDIOF_MAGICCLOSE,
};

<<<<<<< HEAD
/*
 * Handle commands from user-space.
 */
static long at91_wdt_ioctl(struct file *file,
		unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int __user *p = argp;
	int new_value;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		return copy_to_user(argp, &at91_wdt_info,
				    sizeof(at91_wdt_info)) ? -EFAULT : 0;

	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		return put_user(0, p);

	case WDIOC_KEEPALIVE:
		at91wdt_private.next_heartbeat = jiffies + heartbeat * HZ;
		return 0;

	case WDIOC_SETTIMEOUT:
		if (get_user(new_value, p))
			return -EFAULT;

		heartbeat = new_value;
		at91wdt_private.next_heartbeat = jiffies + heartbeat * HZ;

		return put_user(new_value, p);  /* return current value */

	case WDIOC_GETTIMEOUT:
		return put_user(heartbeat, p);
	}
	return -ENOTTY;
}

/*
 * Pat the watchdog whenever device is written to.
 */
static ssize_t at91_wdt_write(struct file *file, const char *data, size_t len,
								loff_t *ppos)
{
	if (!len)
		return 0;

	/* Scan for magic character */
	if (!nowayout) {
		size_t i;

		at91wdt_private.expect_close = 0;

		for (i = 0; i < len; i++) {
			char c;
			if (get_user(c, data + i))
				return -EFAULT;
			if (c == 'V') {
				at91wdt_private.expect_close = 42;
				break;
			}
		}
	}

	at91wdt_private.next_heartbeat = jiffies + heartbeat * HZ;

	return len;
}

/* ......................................................................... */

static const struct file_operations at91wdt_fops = {
	.owner			= THIS_MODULE,
	.llseek			= no_llseek,
	.unlocked_ioctl	= at91_wdt_ioctl,
	.open			= at91_wdt_open,
	.release		= at91_wdt_close,
	.write			= at91_wdt_write,
};

static struct miscdevice at91wdt_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &at91wdt_fops,
=======
static const struct watchdog_ops at91_wdt_ops = {
	.owner =	THIS_MODULE,
	.start =	at91_wdt_start,
	.stop =		at91_wdt_stop,
	.ping =		at91_wdt_ping,
	.set_timeout =	at91_wdt_set_timeout,
};

static struct watchdog_device at91_wdt_dev = {
	.info =		&at91_wdt_info,
	.ops =		&at91_wdt_ops,
	.timeout =	WDT_HEARTBEAT,
	.min_timeout =	1,
	.max_timeout =	0xFFFF,
>>>>>>> refs/remotes/origin/master
};

static int __init at91wdt_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct resource	*r;
>>>>>>> refs/remotes/origin/cm-10.0
	int res;

	if (at91wdt_miscdev.parent)
		return -EBUSY;
	at91wdt_miscdev.parent = &pdev->dev;

<<<<<<< HEAD
=======
=======
	struct resource	*r;
	int res;

>>>>>>> refs/remotes/origin/master
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r)
		return -ENODEV;
	at91wdt_private.base = ioremap(r->start, resource_size(r));
	if (!at91wdt_private.base) {
		dev_err(&pdev->dev, "failed to map registers, aborting.\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	at91_wdt_dev.parent = &pdev->dev;
	watchdog_init_timeout(&at91_wdt_dev, heartbeat, &pdev->dev);
	watchdog_set_nowayout(&at91_wdt_dev, nowayout);

>>>>>>> refs/remotes/origin/master
	/* Set watchdog */
	res = at91_wdt_settimeout(ms_to_ticks(WDT_HW_TIMEOUT * 1000));
	if (res)
		return res;

<<<<<<< HEAD
	res = misc_register(&at91wdt_miscdev);
	if (res)
		return res;

	at91wdt_private.next_heartbeat = jiffies + heartbeat * HZ;
	setup_timer(&at91wdt_private.timer, at91_ping, 0);
	mod_timer(&at91wdt_private.timer, jiffies + WDT_TIMEOUT);

<<<<<<< HEAD
	printk(KERN_INFO DRV_NAME " enabled (heartbeat=%d sec, nowayout=%d)\n",
=======
	pr_info("enabled (heartbeat=%d sec, nowayout=%d)\n",
>>>>>>> refs/remotes/origin/cm-10.0
		heartbeat, nowayout);
=======
	res = watchdog_register_device(&at91_wdt_dev);
	if (res)
		return res;

	at91wdt_private.next_heartbeat = jiffies + at91_wdt_dev.timeout * HZ;
	setup_timer(&at91wdt_private.timer, at91_ping, 0);
	mod_timer(&at91wdt_private.timer, jiffies + WDT_TIMEOUT);

	pr_info("enabled (heartbeat=%d sec, nowayout=%d)\n",
		at91_wdt_dev.timeout, nowayout);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int __exit at91wdt_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	int res;

	res = misc_deregister(&at91wdt_miscdev);
	if (!res)
		at91wdt_miscdev.parent = NULL;

	return res;
}

<<<<<<< HEAD
#ifdef CONFIG_PM

static int at91wdt_suspend(struct platform_device *pdev, pm_message_t message)
{
	return 0;
}

static int at91wdt_resume(struct platform_device *pdev)
{
	return 0;
}

#else
#define at91wdt_suspend	NULL
#define at91wdt_resume	NULL
=======
	watchdog_unregister_device(&at91_wdt_dev);

	pr_warn("I quit now, hardware will probably reboot!\n");
	del_timer(&at91wdt_private.timer);

	return 0;
}

#if defined(CONFIG_OF)
static const struct of_device_id at91_wdt_dt_ids[] = {
	{ .compatible = "atmel,at91sam9260-wdt" },
	{ /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, at91_wdt_dt_ids);
>>>>>>> refs/remotes/origin/master
#endif

static struct platform_driver at91wdt_driver = {
	.remove		= __exit_p(at91wdt_remove),
<<<<<<< HEAD
	.suspend	= at91wdt_suspend,
	.resume		= at91wdt_resume,
=======
static struct platform_driver at91wdt_driver = {
	.remove		= __exit_p(at91wdt_remove),
>>>>>>> refs/remotes/origin/cm-10.0
	.driver		= {
		.name	= "at91_wdt",
		.owner	= THIS_MODULE,
	},
};

static int __init at91sam_wdt_init(void)
{
	return platform_driver_probe(&at91wdt_driver, at91wdt_probe);
}

static void __exit at91sam_wdt_exit(void)
{
	platform_driver_unregister(&at91wdt_driver);
}

module_init(at91sam_wdt_init);
module_exit(at91sam_wdt_exit);
=======
	.driver		= {
		.name	= "at91_wdt",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(at91_wdt_dt_ids),
	},
};

module_platform_driver_probe(at91wdt_driver, at91wdt_probe);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Renaud CERRATO <r.cerrato@til-technologies.fr>");
MODULE_DESCRIPTION("Watchdog driver for Atmel AT91SAM9x processors");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
>>>>>>> refs/remotes/origin/master
