/*
 *	Advantech Single Board Computer WDT driver
 *
 *	(c) Copyright 2000-2001 Marek Michalkiewicz <marekm@linux.org.pl>
 *
 *	Based on acquirewdt.c which is based on wdt.c.
 *	Original copyright messages:
 *
 *	(c) Copyright 1996 Alan Cox <alan@lxorguk.ukuu.org.uk>,
 *						All Rights Reserved.
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 *	Neither Alan Cox nor CymruNet Ltd. admit liability nor provide
 *	warranty for any of this software. This material is provided
 *	"AS-IS" and at no charge.
 *
 *	(c) Copyright 1995    Alan Cox <alan@lxorguk.ukuu.org.uk>
 *
 *	14-Dec-2001 Matt Domsch <Matt_Domsch@dell.com>
 *	    Added nowayout module option to override CONFIG_WATCHDOG_NOWAYOUT
 *
 *	16-Oct-2002 Rob Radez <rob@osinvestor.com>
 *	    Clean up ioctls, clean up init + exit, add expect close support,
 *	    add wdt_start and wdt_stop as parameters.
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
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/uaccess.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>

#define DRV_NAME "advantechwdt"
#define PFX DRV_NAME ": "
=======

#define DRV_NAME "advantechwdt"
>>>>>>> refs/remotes/origin/cm-10.0
=======

#define DRV_NAME "advantechwdt"
>>>>>>> refs/remotes/origin/master
#define WATCHDOG_NAME "Advantech WDT"
#define WATCHDOG_TIMEOUT 60		/* 60 sec default timeout */

/* the watchdog platform device */
static struct platform_device *advwdt_platform_device;
static unsigned long advwdt_is_open;
static char adv_expect_close;

/*
 *	You must set these - there is no sane way to probe for this board.
 *
 *	To enable or restart, write the timeout value in seconds (1 to 63)
 *	to I/O port wdt_start.  To disable, read I/O port wdt_stop.
 *	Both are 0x443 for most boards (tested on a PCA-6276VE-00B1), but
 *	check your manual (at least the PCA-6159 seems to be different -
 *	the manual says wdt_stop is 0x43, not 0x443).
 *	(0x43 is also a write-only control register for the 8254 timer!)
 */

static int wdt_stop = 0x443;
module_param(wdt_stop, int, 0);
MODULE_PARM_DESC(wdt_stop, "Advantech WDT 'stop' io port (default 0x443)");

static int wdt_start = 0x443;
module_param(wdt_start, int, 0);
MODULE_PARM_DESC(wdt_start, "Advantech WDT 'start' io port (default 0x443)");

static int timeout = WATCHDOG_TIMEOUT;	/* in seconds */
module_param(timeout, int, 0);
MODULE_PARM_DESC(timeout,
	"Watchdog timeout in seconds. 1<= timeout <=63, default="
		__MODULE_STRING(WATCHDOG_TIMEOUT) ".");

<<<<<<< HEAD
<<<<<<< HEAD
static int nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, int, 0);
=======
static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
>>>>>>> refs/remotes/origin/master
MODULE_PARM_DESC(nowayout,
	"Watchdog cannot be stopped once started (default="
		__MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

/*
 *	Watchdog Operations
 */

static void advwdt_ping(void)
{
	/* Write a watchdog value */
	outb_p(timeout, wdt_start);
}

static void advwdt_disable(void)
{
	inb_p(wdt_stop);
}

static int advwdt_set_heartbeat(int t)
{
	if (t < 1 || t > 63)
		return -EINVAL;
	timeout = t;
	return 0;
}

/*
 *	/dev/watchdog handling
 */

static ssize_t advwdt_write(struct file *file, const char __user *buf,
						size_t count, loff_t *ppos)
{
	if (count) {
		if (!nowayout) {
			size_t i;

			adv_expect_close = 0;

			for (i = 0; i != count; i++) {
				char c;
				if (get_user(c, buf + i))
					return -EFAULT;
				if (c == 'V')
					adv_expect_close = 42;
			}
		}
		advwdt_ping();
	}
	return count;
}

static long advwdt_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int new_timeout;
	void __user *argp = (void __user *)arg;
	int __user *p = argp;
	static const struct watchdog_info ident = {
		.options = WDIOF_KEEPALIVEPING |
			   WDIOF_SETTIMEOUT |
			   WDIOF_MAGICCLOSE,
		.firmware_version = 1,
		.identity = WATCHDOG_NAME,
	};

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		if (copy_to_user(argp, &ident, sizeof(ident)))
			return -EFAULT;
		break;

	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		return put_user(0, p);

	case WDIOC_SETOPTIONS:
	{
		int options, retval = -EINVAL;

		if (get_user(options, p))
			return -EFAULT;
		if (options & WDIOS_DISABLECARD) {
			advwdt_disable();
			retval = 0;
		}
		if (options & WDIOS_ENABLECARD) {
			advwdt_ping();
			retval = 0;
		}
		return retval;
	}
	case WDIOC_KEEPALIVE:
		advwdt_ping();
		break;

	case WDIOC_SETTIMEOUT:
		if (get_user(new_timeout, p))
			return -EFAULT;
		if (advwdt_set_heartbeat(new_timeout))
			return -EINVAL;
		advwdt_ping();
		/* Fall */
	case WDIOC_GETTIMEOUT:
		return put_user(timeout, p);
	default:
		return -ENOTTY;
	}
	return 0;
}

static int advwdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(0, &advwdt_is_open))
		return -EBUSY;
	/*
	 *	Activate
	 */

	advwdt_ping();
	return nonseekable_open(inode, file);
}

static int advwdt_close(struct inode *inode, struct file *file)
{
	if (adv_expect_close == 42) {
		advwdt_disable();
	} else {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_CRIT PFX
				"Unexpected close, not stopping watchdog!\n");
=======
		pr_crit("Unexpected close, not stopping watchdog!\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_crit("Unexpected close, not stopping watchdog!\n");
>>>>>>> refs/remotes/origin/master
		advwdt_ping();
	}
	clear_bit(0, &advwdt_is_open);
	adv_expect_close = 0;
	return 0;
}

/*
 *	Kernel Interfaces
 */

static const struct file_operations advwdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.write		= advwdt_write,
	.unlocked_ioctl	= advwdt_ioctl,
	.open		= advwdt_open,
	.release	= advwdt_close,
};

static struct miscdevice advwdt_miscdev = {
	.minor	= WATCHDOG_MINOR,
	.name	= "watchdog",
	.fops	= &advwdt_fops,
};

/*
 *	Init & exit routines
 */

<<<<<<< HEAD
static int __devinit advwdt_probe(struct platform_device *dev)
=======
static int advwdt_probe(struct platform_device *dev)
>>>>>>> refs/remotes/origin/master
{
	int ret;

	if (wdt_stop != wdt_start) {
		if (!request_region(wdt_stop, 1, WATCHDOG_NAME)) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_ERR PFX
				"I/O address 0x%04x already in use\n",
								wdt_stop);
=======
			pr_err("I/O address 0x%04x already in use\n",
			       wdt_stop);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pr_err("I/O address 0x%04x already in use\n",
			       wdt_stop);
>>>>>>> refs/remotes/origin/master
			ret = -EIO;
			goto out;
		}
	}

	if (!request_region(wdt_start, 1, WATCHDOG_NAME)) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR PFX
				"I/O address 0x%04x already in use\n",
								wdt_start);
=======
		pr_err("I/O address 0x%04x already in use\n", wdt_start);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("I/O address 0x%04x already in use\n", wdt_start);
>>>>>>> refs/remotes/origin/master
		ret = -EIO;
		goto unreg_stop;
	}

	/* Check that the heartbeat value is within it's range ;
	 * if not reset to the default */
	if (advwdt_set_heartbeat(timeout)) {
		advwdt_set_heartbeat(WATCHDOG_TIMEOUT);
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_INFO PFX
			"timeout value must be 1<=x<=63, using %d\n", timeout);
=======
		pr_info("timeout value must be 1<=x<=63, using %d\n", timeout);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_info("timeout value must be 1<=x<=63, using %d\n", timeout);
>>>>>>> refs/remotes/origin/master
	}

	ret = misc_register(&advwdt_miscdev);
	if (ret != 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR PFX
			"cannot register miscdev on minor=%d (err=%d)\n",
							WATCHDOG_MINOR, ret);
		goto unreg_regions;
	}
	printk(KERN_INFO PFX "initialized. timeout=%d sec (nowayout=%d)\n",
=======
=======
>>>>>>> refs/remotes/origin/master
		pr_err("cannot register miscdev on minor=%d (err=%d)\n",
		       WATCHDOG_MINOR, ret);
		goto unreg_regions;
	}
	pr_info("initialized. timeout=%d sec (nowayout=%d)\n",
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		timeout, nowayout);
out:
	return ret;
unreg_regions:
	release_region(wdt_start, 1);
unreg_stop:
	if (wdt_stop != wdt_start)
		release_region(wdt_stop, 1);
	goto out;
}

<<<<<<< HEAD
static int __devexit advwdt_remove(struct platform_device *dev)
=======
static int advwdt_remove(struct platform_device *dev)
>>>>>>> refs/remotes/origin/master
{
	misc_deregister(&advwdt_miscdev);
	release_region(wdt_start, 1);
	if (wdt_stop != wdt_start)
		release_region(wdt_stop, 1);

	return 0;
}

static void advwdt_shutdown(struct platform_device *dev)
{
	/* Turn the WDT off if we have a soft shutdown */
	advwdt_disable();
}

static struct platform_driver advwdt_driver = {
	.probe		= advwdt_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(advwdt_remove),
=======
	.remove		= advwdt_remove,
>>>>>>> refs/remotes/origin/master
	.shutdown	= advwdt_shutdown,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= DRV_NAME,
	},
};

static int __init advwdt_init(void)
{
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO
	     "WDT driver for Advantech single board computer initialising.\n");
=======
	pr_info("WDT driver for Advantech single board computer initialising\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_info("WDT driver for Advantech single board computer initialising\n");
>>>>>>> refs/remotes/origin/master

	err = platform_driver_register(&advwdt_driver);
	if (err)
		return err;

	advwdt_platform_device = platform_device_register_simple(DRV_NAME,
								-1, NULL, 0);
	if (IS_ERR(advwdt_platform_device)) {
		err = PTR_ERR(advwdt_platform_device);
		goto unreg_platform_driver;
	}

	return 0;

unreg_platform_driver:
	platform_driver_unregister(&advwdt_driver);
	return err;
}

static void __exit advwdt_exit(void)
{
	platform_device_unregister(advwdt_platform_device);
	platform_driver_unregister(&advwdt_driver);
<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO PFX "Watchdog Module Unloaded.\n");
=======
	pr_info("Watchdog Module Unloaded\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_info("Watchdog Module Unloaded\n");
>>>>>>> refs/remotes/origin/master
}

module_init(advwdt_init);
module_exit(advwdt_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marek Michalkiewicz <marekm@linux.org.pl>");
MODULE_DESCRIPTION("Advantech Single Board Computer WDT driver");
<<<<<<< HEAD
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
>>>>>>> refs/remotes/origin/master
