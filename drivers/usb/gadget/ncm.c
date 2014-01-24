/*
 * ncm.c -- NCM gadget driver
 *
 * Copyright (C) 2010 Nokia Corporation
 * Contact: Yauheni Kaliuta <yauheni.kaliuta@nokia.com>
 *
 * The driver borrows from ether.c which is:
 *
 * Copyright (C) 2003-2005,2008 David Brownell
 * Copyright (C) 2003-2004 Robert Schwebel, Benedikt Spranger
 * Copyright (C) 2008 Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
<<<<<<< HEAD
<<<<<<< HEAD
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */

/* #define DEBUG */
/* #define VERBOSE_DEBUG */

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/utsname.h>


#include "u_ether.h"
=======
#include <linux/module.h>
#include <linux/usb/composite.h>

#include "u_ether.h"
#include "u_ncm.h"
>>>>>>> refs/remotes/origin/master

#define DRIVER_DESC		"NCM Gadget"

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
/*
 * Kbuild is not very cooperative with respect to linking separately
 * compiled library objects into one module.  So for now we won't use
 * separate compilation ... ensuring init/exit sections work to shrink
 * the runtime footprint, and giving us at least some parts of what
 * a "gcc --combine ... part1.c part2.c part3.c ... " build would.
 */
#include "composite.c"
#include "usbstring.c"
#include "config.c"
#include "epautoconf.c"

#include "f_ncm.c"
#include "u_ether.c"

/*-------------------------------------------------------------------------*/

=======
>>>>>>> refs/remotes/origin/master
/* DO NOT REUSE THESE IDs with a protocol-incompatible driver!!  Ever!!
 * Instead:  allocate your own, using normal USB-IF procedures.
 */

/* Thanks to NetChip Technologies for donating this product ID.
 * It's for devices with only CDC Ethernet configurations.
 */
#define CDC_VENDOR_NUM		0x0525	/* NetChip */
#define CDC_PRODUCT_NUM		0xa4a1	/* Linux-USB Ethernet Gadget */

/*-------------------------------------------------------------------------*/
<<<<<<< HEAD
=======
USB_GADGET_COMPOSITE_OPTIONS();

USB_ETHERNET_MODULE_PARAMETERS();
>>>>>>> refs/remotes/origin/master

static struct usb_device_descriptor device_desc = {
	.bLength =		sizeof device_desc,
	.bDescriptorType =	USB_DT_DEVICE,

	.bcdUSB =		cpu_to_le16 (0x0200),

	.bDeviceClass =		USB_CLASS_COMM,
	.bDeviceSubClass =	0,
	.bDeviceProtocol =	0,
	/* .bMaxPacketSize0 = f(hardware) */

	/* Vendor and product id defaults change according to what configs
	 * we support.  (As does bNumConfigurations.)  These values can
	 * also be overridden by module parameters.
	 */
	.idVendor =		cpu_to_le16 (CDC_VENDOR_NUM),
	.idProduct =		cpu_to_le16 (CDC_PRODUCT_NUM),
	/* .bcdDevice = f(hardware) */
	/* .iManufacturer = DYNAMIC */
	/* .iProduct = DYNAMIC */
	/* NO SERIAL NUMBER */
	.bNumConfigurations =	1,
};

static struct usb_otg_descriptor otg_descriptor = {
	.bLength =		sizeof otg_descriptor,
	.bDescriptorType =	USB_DT_OTG,

	/* REVISIT SRP-only hardware is possible, although
	 * it would not be called "OTG" ...
	 */
	.bmAttributes =		USB_OTG_SRP | USB_OTG_HNP,
};

static const struct usb_descriptor_header *otg_desc[] = {
	(struct usb_descriptor_header *) &otg_descriptor,
	NULL,
};

<<<<<<< HEAD

/* string IDs are assigned dynamically */

#define STRING_MANUFACTURER_IDX		0
#define STRING_PRODUCT_IDX		1

static char manufacturer[50];

static struct usb_string strings_dev[] = {
	[STRING_MANUFACTURER_IDX].s = manufacturer,
	[STRING_PRODUCT_IDX].s = DRIVER_DESC,
=======
/* string IDs are assigned dynamically */
static struct usb_string strings_dev[] = {
	[USB_GADGET_MANUFACTURER_IDX].s = "",
	[USB_GADGET_PRODUCT_IDX].s = DRIVER_DESC,
	[USB_GADGET_SERIAL_IDX].s = "",
>>>>>>> refs/remotes/origin/master
	{  } /* end of list */
};

static struct usb_gadget_strings stringtab_dev = {
	.language	= 0x0409,	/* en-us */
	.strings	= strings_dev,
};

static struct usb_gadget_strings *dev_strings[] = {
	&stringtab_dev,
	NULL,
};

<<<<<<< HEAD
static u8 hostaddr[ETH_ALEN];
=======
static struct usb_function_instance *f_ncm_inst;
static struct usb_function *f_ncm;
>>>>>>> refs/remotes/origin/master

/*-------------------------------------------------------------------------*/

static int __init ncm_do_config(struct usb_configuration *c)
{
<<<<<<< HEAD
=======
	int status;

>>>>>>> refs/remotes/origin/master
	/* FIXME alloc iConfiguration string, set it in c->strings */

	if (gadget_is_otg(c->cdev->gadget)) {
		c->descriptors = otg_desc;
		c->bmAttributes |= USB_CONFIG_ATT_WAKEUP;
	}

<<<<<<< HEAD
	return ncm_bind_config(c, hostaddr);
=======
	f_ncm = usb_get_function(f_ncm_inst);
	if (IS_ERR(f_ncm)) {
		status = PTR_ERR(f_ncm);
		return status;
	}

	status = usb_add_function(c, f_ncm);
	if (status < 0) {
		usb_put_function(f_ncm);
		return status;
	}

	return 0;
>>>>>>> refs/remotes/origin/master
}

static struct usb_configuration ncm_config_driver = {
	/* .label = f(hardware) */
	.label			= "CDC Ethernet (NCM)",
	.bConfigurationValue	= 1,
	/* .iConfiguration = DYNAMIC */
	.bmAttributes		= USB_CONFIG_ATT_SELFPOWER,
};

/*-------------------------------------------------------------------------*/

static int __init gncm_bind(struct usb_composite_dev *cdev)
{
<<<<<<< HEAD
	int			gcnum;
	struct usb_gadget	*gadget = cdev->gadget;
	int			status;

	/* set up network link layer */
	status = gether_setup(cdev->gadget, hostaddr);
	if (status < 0)
		return status;

	gcnum = usb_gadget_controller_number(gadget);
	if (gcnum >= 0)
		device_desc.bcdDevice = cpu_to_le16(0x0300 | gcnum);
	else {
		/* We assume that can_support_ecm() tells the truth;
		 * but if the controller isn't recognized at all then
		 * that assumption is a bit more likely to be wrong.
		 */
		dev_warn(&gadget->dev,
			 "controller '%s' not recognized; trying %s\n",
			 gadget->name,
			 ncm_config_driver.label);
		device_desc.bcdDevice =
			cpu_to_le16(0x0300 | 0x0099);
	}

=======
	struct usb_gadget	*gadget = cdev->gadget;
	struct f_ncm_opts	*ncm_opts;
	int			status;

	f_ncm_inst = usb_get_function_instance("ncm");
	if (IS_ERR(f_ncm_inst))
		return PTR_ERR(f_ncm_inst);

	ncm_opts = container_of(f_ncm_inst, struct f_ncm_opts, func_inst);

	gether_set_qmult(ncm_opts->net, qmult);
	if (!gether_set_host_addr(ncm_opts->net, host_addr))
		pr_info("using host ethernet address: %s", host_addr);
	if (!gether_set_dev_addr(ncm_opts->net, dev_addr))
		pr_info("using self ethernet address: %s", dev_addr);
>>>>>>> refs/remotes/origin/master

	/* Allocate string descriptor numbers ... note that string
	 * contents can be overridden by the composite_dev glue.
	 */

<<<<<<< HEAD
	/* device descriptor strings: manufacturer, product */
	snprintf(manufacturer, sizeof manufacturer, "%s %s with %s",
		init_utsname()->sysname, init_utsname()->release,
		gadget->name);
	status = usb_string_id(cdev);
	if (status < 0)
		goto fail;
	strings_dev[STRING_MANUFACTURER_IDX].id = status;
	device_desc.iManufacturer = status;

	status = usb_string_id(cdev);
	if (status < 0)
		goto fail;
	strings_dev[STRING_PRODUCT_IDX].id = status;
	device_desc.iProduct = status;
=======
	status = usb_string_ids_tab(cdev, strings_dev);
	if (status < 0)
		goto fail;
	device_desc.iManufacturer = strings_dev[USB_GADGET_MANUFACTURER_IDX].id;
	device_desc.iProduct = strings_dev[USB_GADGET_PRODUCT_IDX].id;
>>>>>>> refs/remotes/origin/master

	status = usb_add_config(cdev, &ncm_config_driver,
				ncm_do_config);
	if (status < 0)
		goto fail;

<<<<<<< HEAD
=======
	usb_composite_overwrite_options(cdev, &coverwrite);
>>>>>>> refs/remotes/origin/master
	dev_info(&gadget->dev, "%s\n", DRIVER_DESC);

	return 0;

fail:
<<<<<<< HEAD
	gether_cleanup();
=======
	usb_put_function_instance(f_ncm_inst);
>>>>>>> refs/remotes/origin/master
	return status;
}

static int __exit gncm_unbind(struct usb_composite_dev *cdev)
{
<<<<<<< HEAD
	gether_cleanup();
	return 0;
}

static struct usb_composite_driver ncm_driver = {
	.name		= "g_ncm",
	.dev		= &device_desc,
	.strings	= dev_strings,
<<<<<<< HEAD
=======
	.max_speed	= USB_SPEED_HIGH,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!IS_ERR_OR_NULL(f_ncm))
		usb_put_function(f_ncm);
	if (!IS_ERR_OR_NULL(f_ncm_inst))
		usb_put_function_instance(f_ncm_inst);
	return 0;
}

static __refdata struct usb_composite_driver ncm_driver = {
	.name		= "g_ncm",
	.dev		= &device_desc,
	.strings	= dev_strings,
	.max_speed	= USB_SPEED_HIGH,
	.bind		= gncm_bind,
>>>>>>> refs/remotes/origin/master
	.unbind		= __exit_p(gncm_unbind),
};

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Yauheni Kaliuta");
MODULE_LICENSE("GPL");

static int __init init(void)
{
<<<<<<< HEAD
	return usb_composite_probe(&ncm_driver, gncm_bind);
=======
	return usb_composite_probe(&ncm_driver);
>>>>>>> refs/remotes/origin/master
}
module_init(init);

static void __exit cleanup(void)
{
	usb_composite_unregister(&ncm_driver);
}
module_exit(cleanup);
