/*
 * audio.c -- Audio gadget driver
 *
 * Copyright (C) 2008 Bryan Wu <cooloney@kernel.org>
 * Copyright (C) 2008 Analog Devices, Inc
 *
 * Enter bugs at http://blackfin.uclinux.org/
 *
 * Licensed under the GPL-2 or later.
 */

/* #define VERBOSE_DEBUG */

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/utsname.h>

<<<<<<< HEAD
#include "u_audio.h"

#define DRIVER_DESC		"Linux USB Audio Gadget"
#define DRIVER_VERSION		"Dec 18, 2008"
=======
#define DRIVER_DESC		"Linux USB Audio Gadget"
#define DRIVER_VERSION		"Feb 2, 2012"
>>>>>>> refs/remotes/origin/cm-10.0

/*-------------------------------------------------------------------------*/

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

<<<<<<< HEAD
#include "u_audio.c"
#include "f_audio.c"
=======
/* string IDs are assigned dynamically */

#define STRING_MANUFACTURER_IDX		0
#define STRING_PRODUCT_IDX		1

static char manufacturer[50];

static struct usb_string strings_dev[] = {
	[STRING_MANUFACTURER_IDX].s = manufacturer,
	[STRING_PRODUCT_IDX].s = DRIVER_DESC,
=======
#include <linux/module.h>
#include <linux/usb/composite.h>

#include "gadget_chips.h"
#define DRIVER_DESC		"Linux USB Audio Gadget"
#define DRIVER_VERSION		"Feb 2, 2012"

USB_GADGET_COMPOSITE_OPTIONS();

/* string IDs are assigned dynamically */

static struct usb_string strings_dev[] = {
	[USB_GADGET_MANUFACTURER_IDX].s = "",
	[USB_GADGET_PRODUCT_IDX].s = DRIVER_DESC,
	[USB_GADGET_SERIAL_IDX].s = "",
>>>>>>> refs/remotes/origin/master
	{  } /* end of list */
};

static struct usb_gadget_strings stringtab_dev = {
	.language = 0x0409,	/* en-us */
	.strings = strings_dev,
};

static struct usb_gadget_strings *audio_strings[] = {
	&stringtab_dev,
	NULL,
};

#ifdef CONFIG_GADGET_UAC1
#include "u_uac1.h"
#include "u_uac1.c"
#include "f_uac1.c"
#else
#include "f_uac2.c"
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/*-------------------------------------------------------------------------*/

/* DO NOT REUSE THESE IDs with a protocol-incompatible driver!!  Ever!!
 * Instead:  allocate your own, using normal USB-IF procedures.
 */

/* Thanks to Linux Foundation for donating this product ID. */
#define AUDIO_VENDOR_NUM		0x1d6b	/* Linux Foundation */
#define AUDIO_PRODUCT_NUM		0x0101	/* Linux-USB Audio Gadget */

/*-------------------------------------------------------------------------*/

static struct usb_device_descriptor device_desc = {
	.bLength =		sizeof device_desc,
	.bDescriptorType =	USB_DT_DEVICE,

	.bcdUSB =		__constant_cpu_to_le16(0x200),

<<<<<<< HEAD
<<<<<<< HEAD
	.bDeviceClass =		USB_CLASS_PER_INTERFACE,
	.bDeviceSubClass =	0,
	.bDeviceProtocol =	0,
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_GADGET_UAC1
	.bDeviceClass =		USB_CLASS_PER_INTERFACE,
	.bDeviceSubClass =	0,
	.bDeviceProtocol =	0,
#else
	.bDeviceClass =		USB_CLASS_MISC,
	.bDeviceSubClass =	0x02,
	.bDeviceProtocol =	0x01,
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/* .bMaxPacketSize0 = f(hardware) */

	/* Vendor and product id defaults change according to what configs
	 * we support.  (As does bNumConfigurations.)  These values can
	 * also be overridden by module parameters.
	 */
	.idVendor =		__constant_cpu_to_le16(AUDIO_VENDOR_NUM),
	.idProduct =		__constant_cpu_to_le16(AUDIO_PRODUCT_NUM),
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

/*-------------------------------------------------------------------------*/

static int __init audio_do_config(struct usb_configuration *c)
{
	/* FIXME alloc iConfiguration string, set it in c->strings */

	if (gadget_is_otg(c->cdev->gadget)) {
		c->descriptors = otg_desc;
		c->bmAttributes |= USB_CONFIG_ATT_WAKEUP;
	}

	audio_bind_config(c);

	return 0;
}

static struct usb_configuration audio_config_driver = {
	.label			= DRIVER_DESC,
	.bConfigurationValue	= 1,
	/* .iConfiguration = DYNAMIC */
	.bmAttributes		= USB_CONFIG_ATT_SELFPOWER,
<<<<<<< HEAD
<<<<<<< HEAD
=======
#ifndef CONFIG_GADGET_UAC1
	.unbind			= uac2_unbind_config,
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifndef CONFIG_GADGET_UAC1
	.unbind			= uac2_unbind_config,
#endif
>>>>>>> refs/remotes/origin/master
};

/*-------------------------------------------------------------------------*/

static int __init audio_bind(struct usb_composite_dev *cdev)
{
<<<<<<< HEAD
	int			gcnum;
	int			status;

	gcnum = usb_gadget_controller_number(cdev->gadget);
	if (gcnum >= 0)
		device_desc.bcdDevice = cpu_to_le16(0x0300 | gcnum);
	else {
		ERROR(cdev, "controller '%s' not recognized; trying %s\n",
			cdev->gadget->name,
			audio_config_driver.label);
		device_desc.bcdDevice =
			__constant_cpu_to_le16(0x0300 | 0x0099);
	}

	/* device descriptor strings: manufacturer, product */
	snprintf(manufacturer, sizeof manufacturer, "%s %s with %s",
		init_utsname()->sysname, init_utsname()->release,
		cdev->gadget->name);
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
	int			status;

	status = usb_string_ids_tab(cdev, strings_dev);
	if (status < 0)
		goto fail;
	device_desc.iManufacturer = strings_dev[USB_GADGET_MANUFACTURER_IDX].id;
	device_desc.iProduct = strings_dev[USB_GADGET_PRODUCT_IDX].id;
>>>>>>> refs/remotes/origin/master

	status = usb_add_config(cdev, &audio_config_driver, audio_do_config);
	if (status < 0)
		goto fail;
<<<<<<< HEAD
=======
	usb_composite_overwrite_options(cdev, &coverwrite);
>>>>>>> refs/remotes/origin/master

	INFO(cdev, "%s, version: %s\n", DRIVER_DESC, DRIVER_VERSION);
	return 0;

fail:
	return status;
}

static int __exit audio_unbind(struct usb_composite_dev *cdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	gaudio_cleanup();
=======
#ifdef CONFIG_GADGET_UAC1
	gaudio_cleanup();
#endif
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static struct usb_composite_driver audio_driver = {
	.name		= "g_audio",
	.dev		= &device_desc,
	.strings	= audio_strings,
<<<<<<< HEAD
=======
	.max_speed	= USB_SPEED_HIGH,
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_GADGET_UAC1
	gaudio_cleanup();
#endif
	return 0;
}

static __refdata struct usb_composite_driver audio_driver = {
	.name		= "g_audio",
	.dev		= &device_desc,
	.strings	= audio_strings,
	.max_speed	= USB_SPEED_HIGH,
	.bind		= audio_bind,
>>>>>>> refs/remotes/origin/master
	.unbind		= __exit_p(audio_unbind),
};

static int __init init(void)
{
<<<<<<< HEAD
	return usb_composite_probe(&audio_driver, audio_bind);
=======
	return usb_composite_probe(&audio_driver);
>>>>>>> refs/remotes/origin/master
}
module_init(init);

static void __exit cleanup(void)
{
	usb_composite_unregister(&audio_driver);
}
module_exit(cleanup);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Bryan Wu <cooloney@kernel.org>");
MODULE_LICENSE("GPL");

