/* -----------------------------------------------------------------------------
 * Copyright (c) 2011 Ozmo Inc
 * Released under the GNU General Public License Version 2 (GPLv2).
 * -----------------------------------------------------------------------------
 */
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/netdevice.h>
#include <linux/errno.h>
#include <linux/ieee80211.h>
<<<<<<< HEAD
#include "ozconfig.h"
#include "ozpd.h"
#include "ozproto.h"
#include "ozcdev.h"
#include "oztrace.h"
#include "ozevent.h"
/*------------------------------------------------------------------------------
=======
#include "ozdbg.h"
#include "ozpd.h"
#include "ozproto.h"
#include "ozcdev.h"

unsigned int oz_dbg_mask = OZ_DEFAULT_DBG_MASK;

/*
>>>>>>> refs/remotes/origin/master
 * The name of the 802.11 mac device. Empty string is the default value but a
 * value can be supplied as a parameter to the module. An empty string means
 * bind to nothing. '*' means bind to all netcards - this includes non-802.11
 * netcards. Bindings can be added later using an IOCTL.
 */
<<<<<<< HEAD
char *g_net_dev = "";
/*------------------------------------------------------------------------------
=======
static char *g_net_dev = "";

/*
>>>>>>> refs/remotes/origin/master
 * Context: process
 */
static int __init ozwpan_init(void)
{
<<<<<<< HEAD
	oz_event_init();
=======
>>>>>>> refs/remotes/origin/master
	oz_cdev_register();
	oz_protocol_init(g_net_dev);
	oz_app_enable(OZ_APPID_USB, 1);
	oz_apps_init();
	return 0;
}
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> refs/remotes/origin/master
 * Context: process
 */
static void __exit ozwpan_exit(void)
{
	oz_protocol_term();
	oz_apps_term();
	oz_cdev_deregister();
<<<<<<< HEAD
	oz_event_term();
}
/*------------------------------------------------------------------------------
 */
=======
}

>>>>>>> refs/remotes/origin/master
module_param(g_net_dev, charp, S_IRUGO);
module_init(ozwpan_init);
module_exit(ozwpan_exit);

MODULE_AUTHOR("Chris Kelly");
MODULE_DESCRIPTION("Ozmo Devices USB over WiFi hcd driver");
<<<<<<< HEAD
MODULE_VERSION("1.0.8");
=======
MODULE_VERSION("1.0.13");
>>>>>>> refs/remotes/origin/master
MODULE_LICENSE("GPL");

