/*
<<<<<<< HEAD
<<<<<<< HEAD
 *	SoftDog	0.07:	A Software Watchdog Device
=======
 *	SoftDog:	A Software Watchdog Device
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *	SoftDog:	A Software Watchdog Device
>>>>>>> refs/remotes/origin/master
 *
 *	(c) Copyright 1996 Alan Cox <alan@lxorguk.ukuu.org.uk>,
 *							All Rights Reserved.
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
 *	Software only watchdog driver. Unlike its big brother the WDT501P
 *	driver this won't always recover a failed machine.
 *
 *  03/96: Angelo Haritsis <ah@doc.ic.ac.uk> :
 *	Modularised.
 *	Added soft_margin; use upon insmod to change the timer delay.
 *	NB: uses same minor as wdt (WATCHDOG_MINOR); we could use separate
 *	    minors.
 *
 *  19980911 Alan Cox
 *	Made SMP safe for 2.3.x
 *
 *  20011127 Joel Becker (jlbec@evilplan.org>
 *	Added soft_noboot; Allows testing the softdog trigger without
 *	requiring a recompile.
 *	Added WDIOC_GETTIMEOUT and WDIOC_SETTIMOUT.
 *
 *  20020530 Joel Becker <joel.becker@oracle.com>
 *	Added Matt Domsch's nowayout module option.
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
#include <linux/timer.h>
<<<<<<< HEAD
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
<<<<<<< HEAD
#include <linux/fs.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/watchdog.h>
>>>>>>> refs/remotes/origin/master
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <linux/jiffies.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/uaccess.h>
#include <linux/kernel.h>

#define PFX "SoftDog: "

#define TIMER_MARGIN	60		/* Default is 60 seconds */
static int soft_margin = TIMER_MARGIN;	/* in seconds */
module_param(soft_margin, int, 0);
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/kernel.h>

#define TIMER_MARGIN	60		/* Default is 60 seconds */
static unsigned int soft_margin = TIMER_MARGIN;	/* in seconds */
module_param(soft_margin, uint, 0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
MODULE_PARM_DESC(soft_margin,
	"Watchdog soft_margin in seconds. (0 < soft_margin < 65536, default="
					__MODULE_STRING(TIMER_MARGIN) ")");

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

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef ONLY_TESTING
static int soft_noboot = 1;
#else
static int soft_noboot = 0;
#endif  /* ONLY_TESTING */

module_param(soft_noboot, int, 0);
MODULE_PARM_DESC(soft_noboot,
	"Softdog action, set to 1 to ignore reboots, 0 to reboot "
					"(default depends on ONLY_TESTING)");
=======
=======
>>>>>>> refs/remotes/origin/master
static int soft_noboot = 0;
module_param(soft_noboot, int, 0);
MODULE_PARM_DESC(soft_noboot,
	"Softdog action, set to 1 to ignore reboots, 0 to reboot (default=0)");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static int soft_panic;
module_param(soft_panic, int, 0);
MODULE_PARM_DESC(soft_panic,
	"Softdog action, set to 1 to panic, 0 to reboot (default=0)");

/*
 *	Our timer
 */

static void watchdog_fire(unsigned long);

static struct timer_list watchdog_ticktock =
		TIMER_INITIALIZER(watchdog_fire, 0, 0);
<<<<<<< HEAD
<<<<<<< HEAD
static unsigned long driver_open, orphan_timer;
static char expect_close;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/*
 *	If the timer expires..
 */

static void watchdog_fire(unsigned long data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (test_and_clear_bit(0, &orphan_timer))
		module_put(THIS_MODULE);

	if (soft_noboot)
		printk(KERN_CRIT PFX "Triggered - Reboot ignored.\n");
	else if (soft_panic) {
		printk(KERN_CRIT PFX "Initiating panic.\n");
		panic("Software Watchdog Timer expired.");
	} else {
		printk(KERN_CRIT PFX "Initiating system reboot.\n");
		emergency_restart();
		printk(KERN_CRIT PFX "Reboot didn't ?????\n");
=======
=======
>>>>>>> refs/remotes/origin/master
	if (soft_noboot)
		pr_crit("Triggered - Reboot ignored\n");
	else if (soft_panic) {
		pr_crit("Initiating panic\n");
		panic("Software Watchdog Timer expired");
	} else {
		pr_crit("Initiating system reboot\n");
		emergency_restart();
		pr_crit("Reboot didn't ?????\n");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
}

/*
 *	Softdog operations
 */

<<<<<<< HEAD
<<<<<<< HEAD
static int softdog_keepalive(void)
{
	mod_timer(&watchdog_ticktock, jiffies+(soft_margin*HZ));
	return 0;
}

static int softdog_stop(void)
=======
=======
>>>>>>> refs/remotes/origin/master
static int softdog_ping(struct watchdog_device *w)
{
	mod_timer(&watchdog_ticktock, jiffies+(w->timeout*HZ));
	return 0;
}

static int softdog_stop(struct watchdog_device *w)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	del_timer(&watchdog_ticktock);
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int softdog_set_heartbeat(int t)
{
	if ((t < 0x0001) || (t > 0xFFFF))
		return -EINVAL;

	soft_margin = t;
	return 0;
}

/*
 *	/dev/watchdog handling
 */

static int softdog_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(0, &driver_open))
		return -EBUSY;
	if (!test_and_clear_bit(0, &orphan_timer))
		__module_get(THIS_MODULE);
	/*
	 *	Activate timer
	 */
	softdog_keepalive();
	return nonseekable_open(inode, file);
}

static int softdog_release(struct inode *inode, struct file *file)
{
	/*
	 *	Shut off the timer.
	 *	Lock it in if it's a module and we set nowayout
	 */
	if (expect_close == 42) {
		softdog_stop();
		module_put(THIS_MODULE);
	} else {
		printk(KERN_CRIT PFX
			"Unexpected close, not stopping watchdog!\n");
		set_bit(0, &orphan_timer);
		softdog_keepalive();
	}
	clear_bit(0, &driver_open);
	expect_close = 0;
	return 0;
}

static ssize_t softdog_write(struct file *file, const char __user *data,
						size_t len, loff_t *ppos)
{
	/*
	 *	Refresh the timer.
	 */
	if (len) {
		if (!nowayout) {
			size_t i;

			/* In case it was set long ago */
			expect_close = 0;

			for (i = 0; i != len; i++) {
				char c;

				if (get_user(c, data + i))
					return -EFAULT;
				if (c == 'V')
					expect_close = 42;
			}
		}
		softdog_keepalive();
	}
	return len;
}

static long softdog_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int __user *p = argp;
	int new_margin;
	static const struct watchdog_info ident = {
		.options =		WDIOF_SETTIMEOUT |
					WDIOF_KEEPALIVEPING |
					WDIOF_MAGICCLOSE,
		.firmware_version =	0,
		.identity =		"Software Watchdog",
	};
	switch (cmd) {
	case WDIOC_GETSUPPORT:
		return copy_to_user(argp, &ident, sizeof(ident)) ? -EFAULT : 0;
	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		return put_user(0, p);
	case WDIOC_KEEPALIVE:
		softdog_keepalive();
		return 0;
	case WDIOC_SETTIMEOUT:
		if (get_user(new_margin, p))
			return -EFAULT;
		if (softdog_set_heartbeat(new_margin))
			return -EINVAL;
		softdog_keepalive();
		/* Fall */
	case WDIOC_GETTIMEOUT:
		return put_user(soft_margin, p);
	default:
		return -ENOTTY;
	}
}

=======
=======
>>>>>>> refs/remotes/origin/master
static int softdog_set_timeout(struct watchdog_device *w, unsigned int t)
{
	w->timeout = t;
	return 0;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 *	Notifier for system down
 */

static int softdog_notify_sys(struct notifier_block *this, unsigned long code,
	void *unused)
{
	if (code == SYS_DOWN || code == SYS_HALT)
		/* Turn the WDT off */
<<<<<<< HEAD
<<<<<<< HEAD
		softdog_stop();
=======
		softdog_stop(NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		softdog_stop(NULL);
>>>>>>> refs/remotes/origin/master
	return NOTIFY_DONE;
}

/*
 *	Kernel Interfaces
 */

<<<<<<< HEAD
<<<<<<< HEAD
static const struct file_operations softdog_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.write		= softdog_write,
	.unlocked_ioctl	= softdog_ioctl,
	.open		= softdog_open,
	.release	= softdog_release,
};

static struct miscdevice softdog_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &softdog_fops,
};

static struct notifier_block softdog_notifier = {
	.notifier_call	= softdog_notify_sys,
};

static char banner[] __initdata = KERN_INFO "Software Watchdog Timer: 0.07 "
	"initialized. soft_noboot=%d soft_margin=%d sec soft_panic=%d "
	"(nowayout= %d)\n";
=======
=======
>>>>>>> refs/remotes/origin/master
static struct notifier_block softdog_notifier = {
	.notifier_call	= softdog_notify_sys,
};

static struct watchdog_info softdog_info = {
	.identity = "Software Watchdog",
	.options = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
};

static struct watchdog_ops softdog_ops = {
	.owner = THIS_MODULE,
	.start = softdog_ping,
	.stop = softdog_stop,
<<<<<<< HEAD
	.ping = softdog_ping,
=======
>>>>>>> refs/remotes/origin/master
	.set_timeout = softdog_set_timeout,
};

static struct watchdog_device softdog_dev = {
	.info = &softdog_info,
	.ops = &softdog_ops,
	.min_timeout = 1,
	.max_timeout = 0xFFFF
};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static int __init watchdog_init(void)
{
	int ret;

	/* Check that the soft_margin value is within it's range;
	   if not reset to the default */
<<<<<<< HEAD
<<<<<<< HEAD
	if (softdog_set_heartbeat(soft_margin)) {
		softdog_set_heartbeat(TIMER_MARGIN);
		printk(KERN_INFO PFX
		    "soft_margin must be 0 < soft_margin < 65536, using %d\n",
			TIMER_MARGIN);
	}

	ret = register_reboot_notifier(&softdog_notifier);
	if (ret) {
		printk(KERN_ERR PFX
			"cannot register reboot notifier (err=%d)\n", ret);
		return ret;
	}

	ret = misc_register(&softdog_miscdev);
	if (ret) {
		printk(KERN_ERR PFX
			"cannot register miscdev on minor=%d (err=%d)\n",
						WATCHDOG_MINOR, ret);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (soft_margin < 1 || soft_margin > 65535) {
		pr_info("soft_margin must be 0 < soft_margin < 65536, using %d\n",
			TIMER_MARGIN);
		return -EINVAL;
	}
	softdog_dev.timeout = soft_margin;

	watchdog_set_nowayout(&softdog_dev, nowayout);

	ret = register_reboot_notifier(&softdog_notifier);
	if (ret) {
		pr_err("cannot register reboot notifier (err=%d)\n", ret);
		return ret;
	}

	ret = watchdog_register_device(&softdog_dev);
	if (ret) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		unregister_reboot_notifier(&softdog_notifier);
		return ret;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	printk(banner, soft_noboot, soft_margin, soft_panic, nowayout);
=======
	pr_info("Software Watchdog Timer: 0.08 initialized. soft_noboot=%d soft_margin=%d sec soft_panic=%d (nowayout=%d)\n",
		soft_noboot, soft_margin, soft_panic, nowayout);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_info("Software Watchdog Timer: 0.08 initialized. soft_noboot=%d soft_margin=%d sec soft_panic=%d (nowayout=%d)\n",
		soft_noboot, soft_margin, soft_panic, nowayout);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static void __exit watchdog_exit(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	misc_deregister(&softdog_miscdev);
=======
	watchdog_unregister_device(&softdog_dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	watchdog_unregister_device(&softdog_dev);
>>>>>>> refs/remotes/origin/master
	unregister_reboot_notifier(&softdog_notifier);
}

module_init(watchdog_init);
module_exit(watchdog_exit);

MODULE_AUTHOR("Alan Cox");
MODULE_DESCRIPTION("Software Watchdog Device Driver");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
>>>>>>> refs/remotes/origin/master
