/*
 * hid.c -- HID Composite driver
 *
 * Based on multi.c
 *
 * Copyright (C) 2010 Fabien Chouteau <fabien.chouteau@barco.com>
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


#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/list.h>
<<<<<<< HEAD

=======
#include <linux/module.h>
#include <linux/usb/composite.h>

#include "gadget_chips.h"
>>>>>>> refs/remotes/origin/master
#define DRIVER_DESC		"HID Gadget"
#define DRIVER_VERSION		"2010/03/16"

/*-------------------------------------------------------------------------*/

#define HIDG_VENDOR_NUM		0x0525	/* XXX NetChip */
#define HIDG_PRODUCT_NUM	0xa4ac	/* Linux-USB HID gadget */

/*-------------------------------------------------------------------------*/

/*
 * kbuild is not very cooperative with respect to linking separately
 * compiled library objects into one module.  So for now we won't use
 * separate compilation ... ensuring init/exit sections work to shrink
 * the runtime footprint, and giving us at least some parts of what
 * a "gcc --combine ... part1.c part2.c part3.c ... " build would.
 */
<<<<<<< HEAD

#include "composite.c"
#include "usbstring.c"
#include "config.c"
#include "epautoconf.c"

=======
>>>>>>> refs/remotes/origin/master
#include "f_hid.c"


struct hidg_func_node {
	struct list_head node;
	struct hidg_func_descriptor *func;
};

static LIST_HEAD(hidg_func_list);

/*-------------------------------------------------------------------------*/
<<<<<<< HEAD
=======
USB_GADGET_COMPOSITE_OPTIONS();
>>>>>>> refs/remotes/origin/master

static struct usb_device_descriptor device_desc = {
	.bLength =		sizeof device_desc,
	.bDescriptorType =	USB_DT_DEVICE,

	.bcdUSB =		cpu_to_le16(0x0200),

	/* .bDeviceClass =		USB_CLASS_COMM, */
	/* .bDeviceSubClass =	0, */
	/* .bDeviceProtocol =	0, */
	.bDeviceClass =		USB_CLASS_PER_INTERFACE,
	.bDeviceSubClass =	0,
	.bDeviceProtocol =	0,
	/* .bMaxPacketSize0 = f(hardware) */

	/* Vendor and product id can be overridden by module parameters.  */
	.idVendor =		cpu_to_le16(HIDG_VENDOR_NUM),
	.idProduct =		cpu_to_le16(HIDG_PRODUCT_NUM),
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


/* string IDs are assigned dynamically */
<<<<<<< HEAD

#define STRING_MANUFACTURER_IDX		0
#define STRING_PRODUCT_IDX		1

static char manufacturer[50];

static struct usb_string strings_dev[] = {
	[STRING_MANUFACTURER_IDX].s = manufacturer,
	[STRING_PRODUCT_IDX].s = DRIVER_DESC,
=======
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



/****************************** Configurations ******************************/

static int __init do_config(struct usb_configuration *c)
{
	struct hidg_func_node *e;
	int func = 0, status = 0;

	if (gadget_is_otg(c->cdev->gadget)) {
		c->descriptors = otg_desc;
		c->bmAttributes |= USB_CONFIG_ATT_WAKEUP;
	}

	list_for_each_entry(e, &hidg_func_list, node) {
		status = hidg_bind_config(c, e->func, func++);
		if (status)
			break;
	}

	return status;
}

static struct usb_configuration config_driver = {
	.label			= "HID Gadget",
	.bConfigurationValue	= 1,
	/* .iConfiguration = DYNAMIC */
	.bmAttributes		= USB_CONFIG_ATT_SELFPOWER,
};

/****************************** Gadget Bind ******************************/

static int __init hid_bind(struct usb_composite_dev *cdev)
{
	struct usb_gadget *gadget = cdev->gadget;
	struct list_head *tmp;
<<<<<<< HEAD
	int status, gcnum, funcs = 0;
=======
	int status, funcs = 0;
>>>>>>> refs/remotes/origin/master

	list_for_each(tmp, &hidg_func_list)
		funcs++;

	if (!funcs)
		return -ENODEV;

	/* set up HID */
	status = ghid_setup(cdev->gadget, funcs);
	if (status < 0)
		return status;

<<<<<<< HEAD
	gcnum = usb_gadget_controller_number(gadget);
	if (gcnum >= 0)
		device_desc.bcdDevice = cpu_to_le16(0x0300 | gcnum);
	else
		device_desc.bcdDevice = cpu_to_le16(0x0300 | 0x0099);


=======
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
		return status;
	strings_dev[STRING_MANUFACTURER_IDX].id = status;
	device_desc.iManufacturer = status;

	status = usb_string_id(cdev);
	if (status < 0)
		return status;
	strings_dev[STRING_PRODUCT_IDX].id = status;
	device_desc.iProduct = status;
=======
	status = usb_string_ids_tab(cdev, strings_dev);
	if (status < 0)
		return status;
	device_desc.iManufacturer = strings_dev[USB_GADGET_MANUFACTURER_IDX].id;
	device_desc.iProduct = strings_dev[USB_GADGET_PRODUCT_IDX].id;
>>>>>>> refs/remotes/origin/master

	/* register our configuration */
	status = usb_add_config(cdev, &config_driver, do_config);
	if (status < 0)
		return status;

<<<<<<< HEAD
=======
	usb_composite_overwrite_options(cdev, &coverwrite);
>>>>>>> refs/remotes/origin/master
	dev_info(&gadget->dev, DRIVER_DESC ", version: " DRIVER_VERSION "\n");

	return 0;
}

static int __exit hid_unbind(struct usb_composite_dev *cdev)
{
	ghid_cleanup();
	return 0;
}

static int __init hidg_plat_driver_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct hidg_func_descriptor *func = pdev->dev.platform_data;
=======
	struct hidg_func_descriptor *func = dev_get_platdata(&pdev->dev);
>>>>>>> refs/remotes/origin/master
	struct hidg_func_node *entry;

	if (!func) {
		dev_err(&pdev->dev, "Platform data missing\n");
		return -ENODEV;
	}

	entry = kzalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return -ENOMEM;

	entry->func = func;
	list_add_tail(&entry->node, &hidg_func_list);

	return 0;
}

<<<<<<< HEAD
static int __devexit hidg_plat_driver_remove(struct platform_device *pdev)
=======
static int hidg_plat_driver_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct hidg_func_node *e, *n;

	list_for_each_entry_safe(e, n, &hidg_func_list, node) {
		list_del(&e->node);
		kfree(e);
	}

	return 0;
}


/****************************** Some noise ******************************/


<<<<<<< HEAD
static struct usb_composite_driver hidg_driver = {
	.name		= "g_hid",
	.dev		= &device_desc,
	.strings	= dev_strings,
<<<<<<< HEAD
=======
	.max_speed	= USB_SPEED_HIGH,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static __refdata struct usb_composite_driver hidg_driver = {
	.name		= "g_hid",
	.dev		= &device_desc,
	.strings	= dev_strings,
	.max_speed	= USB_SPEED_HIGH,
	.bind		= hid_bind,
>>>>>>> refs/remotes/origin/master
	.unbind		= __exit_p(hid_unbind),
};

static struct platform_driver hidg_plat_driver = {
<<<<<<< HEAD
	.remove		= __devexit_p(hidg_plat_driver_remove),
=======
	.remove		= hidg_plat_driver_remove,
>>>>>>> refs/remotes/origin/master
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "hidg",
	},
};


MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Fabien Chouteau, Peter Korsgaard");
MODULE_LICENSE("GPL");

static int __init hidg_init(void)
{
	int status;

	status = platform_driver_probe(&hidg_plat_driver,
				hidg_plat_driver_probe);
	if (status < 0)
		return status;

<<<<<<< HEAD
	status = usb_composite_probe(&hidg_driver, hid_bind);
=======
	status = usb_composite_probe(&hidg_driver);
>>>>>>> refs/remotes/origin/master
	if (status < 0)
		platform_driver_unregister(&hidg_plat_driver);

	return status;
}
module_init(hidg_init);

static void __exit hidg_cleanup(void)
{
	platform_driver_unregister(&hidg_plat_driver);
	usb_composite_unregister(&hidg_driver);
}
module_exit(hidg_cleanup);
