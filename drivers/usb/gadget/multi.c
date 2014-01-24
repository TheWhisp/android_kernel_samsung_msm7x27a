/*
 * multi.c -- Multifunction Composite driver
 *
 * Copyright (C) 2008 David Brownell
 * Copyright (C) 2008 Nokia Corporation
 * Copyright (C) 2009 Samsung Electronics
<<<<<<< HEAD
<<<<<<< HEAD
 * Author: Michal Nazarewicz (m.nazarewicz@samsung.com)
=======
 * Author: Michal Nazarewicz (mina86@mina86.com)
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Author: Michal Nazarewicz (mina86@mina86.com)
>>>>>>> refs/remotes/origin/master
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
<<<<<<< HEAD
#include <linux/utsname.h>
#include <linux/module.h>


=======
#include <linux/module.h>
#include <linux/netdevice.h>

#include "u_serial.h"
>>>>>>> refs/remotes/origin/master
#if defined USB_ETH_RNDIS
#  undef USB_ETH_RNDIS
#endif
#ifdef CONFIG_USB_G_MULTI_RNDIS
#  define USB_ETH_RNDIS y
#endif


#define DRIVER_DESC		"Multifunction Composite Gadget"

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Michal Nazarewicz");
MODULE_LICENSE("GPL");


<<<<<<< HEAD
/***************************** All the files... *****************************/

/*
 * kbuild is not very cooperative with respect to linking separately
 * compiled library objects into one module.  So for now we won't use
 * separate compilation ... ensuring init/exit sections work to shrink
 * the runtime footprint, and giving us at least some parts of what
 * a "gcc --combine ... part1.c part2.c part3.c ... " build would.
 */

#include "composite.c"
#include "usbstring.c"
#include "config.c"
#include "epautoconf.c"

#include "f_mass_storage.c"

#include "u_serial.c"
#include "f_acm.c"

#include "f_ecm.c"
#include "f_subset.c"
#ifdef USB_ETH_RNDIS
#  include "f_rndis.c"
#  include "rndis.c"
#endif
#include "u_ether.c"


=======
#include "f_mass_storage.h"

#include "u_ecm.h"
#ifdef USB_ETH_RNDIS
#  include "u_rndis.h"
#  include "rndis.h"
#endif
#include "u_ether.h"

USB_GADGET_COMPOSITE_OPTIONS();

USB_ETHERNET_MODULE_PARAMETERS();
>>>>>>> refs/remotes/origin/master

/***************************** Device Descriptor ****************************/

#define MULTI_VENDOR_NUM	0x1d6b	/* Linux Foundation */
#define MULTI_PRODUCT_NUM	0x0104	/* Multifunction Composite Gadget */


enum {
	__MULTI_NO_CONFIG,
#ifdef CONFIG_USB_G_MULTI_RNDIS
	MULTI_RNDIS_CONFIG_NUM,
#endif
#ifdef CONFIG_USB_G_MULTI_CDC
	MULTI_CDC_CONFIG_NUM,
#endif
};


static struct usb_device_descriptor device_desc = {
	.bLength =		sizeof device_desc,
	.bDescriptorType =	USB_DT_DEVICE,

	.bcdUSB =		cpu_to_le16(0x0200),

	.bDeviceClass =		USB_CLASS_MISC /* 0xEF */,
	.bDeviceSubClass =	2,
	.bDeviceProtocol =	1,

	/* Vendor and product id can be overridden by module parameters.  */
	.idVendor =		cpu_to_le16(MULTI_VENDOR_NUM),
	.idProduct =		cpu_to_le16(MULTI_PRODUCT_NUM),
};


static const struct usb_descriptor_header *otg_desc[] = {
	(struct usb_descriptor_header *) &(struct usb_otg_descriptor){
		.bLength =		sizeof(struct usb_otg_descriptor),
		.bDescriptorType =	USB_DT_OTG,

		/*
		 * REVISIT SRP-only hardware is possible, although
		 * it would not be called "OTG" ...
		 */
		.bmAttributes =		USB_OTG_SRP | USB_OTG_HNP,
	},
	NULL,
};


enum {
<<<<<<< HEAD
#ifdef CONFIG_USB_G_MULTI_RNDIS
	MULTI_STRING_RNDIS_CONFIG_IDX,
#endif
#ifdef CONFIG_USB_G_MULTI_CDC
	MULTI_STRING_CDC_CONFIG_IDX,
#endif
};

static struct usb_string strings_dev[] = {
#ifdef CONFIG_USB_G_MULTI_RNDIS
	[MULTI_STRING_RNDIS_CONFIG_IDX].s = "Multifunction with RNDIS",
#endif
#ifdef CONFIG_USB_G_MULTI_CDC
	[MULTI_STRING_CDC_CONFIG_IDX].s   = "Multifunction with CDC ECM",
#endif
=======
	MULTI_STRING_RNDIS_CONFIG_IDX = USB_GADGET_FIRST_AVAIL_IDX,
	MULTI_STRING_CDC_CONFIG_IDX,
};

static struct usb_string strings_dev[] = {
	[USB_GADGET_MANUFACTURER_IDX].s = "",
	[USB_GADGET_PRODUCT_IDX].s = DRIVER_DESC,
	[USB_GADGET_SERIAL_IDX].s = "",
	[MULTI_STRING_RNDIS_CONFIG_IDX].s = "Multifunction with RNDIS",
	[MULTI_STRING_CDC_CONFIG_IDX].s   = "Multifunction with CDC ECM",
>>>>>>> refs/remotes/origin/master
	{  } /* end of list */
};

static struct usb_gadget_strings *dev_strings[] = {
	&(struct usb_gadget_strings){
		.language	= 0x0409,	/* en-us */
		.strings	= strings_dev,
	},
	NULL,
};




/****************************** Configurations ******************************/

static struct fsg_module_parameters fsg_mod_data = { .stall = 1 };
<<<<<<< HEAD
FSG_MODULE_PARAMETERS(/* no prefix */, fsg_mod_data);

static struct fsg_common fsg_common;

static u8 hostaddr[ETH_ALEN];

=======
#ifdef CONFIG_USB_GADGET_DEBUG_FILES

static unsigned int fsg_num_buffers = CONFIG_USB_GADGET_STORAGE_NUM_BUFFERS;

#else

/*
 * Number of buffers we will use.
 * 2 is usually enough for good buffering pipeline
 */
#define fsg_num_buffers	CONFIG_USB_GADGET_STORAGE_NUM_BUFFERS

#endif /* CONFIG_USB_GADGET_DEBUG_FILES */

FSG_MODULE_PARAMETERS(/* no prefix */, fsg_mod_data);

static struct usb_function_instance *fi_acm;
static struct usb_function_instance *fi_msg;
>>>>>>> refs/remotes/origin/master

/********** RNDIS **********/

#ifdef USB_ETH_RNDIS
<<<<<<< HEAD

static __init int rndis_do_config(struct usb_configuration *c)
{
=======
static struct usb_function_instance *fi_rndis;
static struct usb_function *f_acm_rndis;
static struct usb_function *f_rndis;
static struct usb_function *f_msg_rndis;

static __init int rndis_do_config(struct usb_configuration *c)
{
	struct fsg_opts *fsg_opts;
>>>>>>> refs/remotes/origin/master
	int ret;

	if (gadget_is_otg(c->cdev->gadget)) {
		c->descriptors = otg_desc;
		c->bmAttributes |= USB_CONFIG_ATT_WAKEUP;
	}

<<<<<<< HEAD
	ret = rndis_bind_config(c, hostaddr);
	if (ret < 0)
		return ret;

	ret = acm_bind_config(c, 0);
	if (ret < 0)
		return ret;

	ret = fsg_bind_config(c->cdev, c, &fsg_common);
	if (ret < 0)
		return ret;

	return 0;
}

static int rndis_config_register(struct usb_composite_dev *cdev)
=======
	f_rndis = usb_get_function(fi_rndis);
	if (IS_ERR(f_rndis))
		return PTR_ERR(f_rndis);

	ret = usb_add_function(c, f_rndis);
	if (ret < 0)
		goto err_func_rndis;

	f_acm_rndis = usb_get_function(fi_acm);
	if (IS_ERR(f_acm_rndis)) {
		ret = PTR_ERR(f_acm_rndis);
		goto err_func_acm;
	}

	ret = usb_add_function(c, f_acm_rndis);
	if (ret)
		goto err_conf;

	f_msg_rndis = usb_get_function(fi_msg);
	if (IS_ERR(f_msg_rndis)) {
		ret = PTR_ERR(f_msg_rndis);
		goto err_fsg;
	}

	fsg_opts = fsg_opts_from_func_inst(fi_msg);
	ret = fsg_common_run_thread(fsg_opts->common);
	if (ret)
		goto err_run;

	ret = usb_add_function(c, f_msg_rndis);
	if (ret)
		goto err_run;

	return 0;
err_run:
	usb_put_function(f_msg_rndis);
err_fsg:
	usb_remove_function(c, f_acm_rndis);
err_conf:
	usb_put_function(f_acm_rndis);
err_func_acm:
	usb_remove_function(c, f_rndis);
err_func_rndis:
	usb_put_function(f_rndis);
	return ret;
}

static __ref int rndis_config_register(struct usb_composite_dev *cdev)
>>>>>>> refs/remotes/origin/master
{
	static struct usb_configuration config = {
		.bConfigurationValue	= MULTI_RNDIS_CONFIG_NUM,
		.bmAttributes		= USB_CONFIG_ATT_SELFPOWER,
	};

	config.label          = strings_dev[MULTI_STRING_RNDIS_CONFIG_IDX].s;
	config.iConfiguration = strings_dev[MULTI_STRING_RNDIS_CONFIG_IDX].id;

	return usb_add_config(cdev, &config, rndis_do_config);
}

#else

<<<<<<< HEAD
static int rndis_config_register(struct usb_composite_dev *cdev)
=======
static __ref int rndis_config_register(struct usb_composite_dev *cdev)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

#endif


/********** CDC ECM **********/

#ifdef CONFIG_USB_G_MULTI_CDC
<<<<<<< HEAD

static __init int cdc_do_config(struct usb_configuration *c)
{
=======
static struct usb_function_instance *fi_ecm;
static struct usb_function *f_acm_multi;
static struct usb_function *f_ecm;
static struct usb_function *f_msg_multi;

static __init int cdc_do_config(struct usb_configuration *c)
{
	struct fsg_opts *fsg_opts;
>>>>>>> refs/remotes/origin/master
	int ret;

	if (gadget_is_otg(c->cdev->gadget)) {
		c->descriptors = otg_desc;
		c->bmAttributes |= USB_CONFIG_ATT_WAKEUP;
	}

<<<<<<< HEAD
	ret = ecm_bind_config(c, hostaddr);
	if (ret < 0)
		return ret;

	ret = acm_bind_config(c, 0);
	if (ret < 0)
		return ret;

	ret = fsg_bind_config(c->cdev, c, &fsg_common);
	if (ret < 0)
		return ret;

	return 0;
}

static int cdc_config_register(struct usb_composite_dev *cdev)
=======
	f_ecm = usb_get_function(fi_ecm);
	if (IS_ERR(f_ecm))
		return PTR_ERR(f_ecm);

	ret = usb_add_function(c, f_ecm);
	if (ret < 0)
		goto err_func_ecm;

	/* implicit port_num is zero */
	f_acm_multi = usb_get_function(fi_acm);
	if (IS_ERR(f_acm_multi)) {
		ret = PTR_ERR(f_acm_multi);
		goto err_func_acm;
	}

	ret = usb_add_function(c, f_acm_multi);
	if (ret)
		goto err_conf;

	f_msg_multi = usb_get_function(fi_msg);
	if (IS_ERR(f_msg_multi)) {
		ret = PTR_ERR(f_msg_multi);
		goto err_fsg;
	}

	fsg_opts = fsg_opts_from_func_inst(fi_msg);
	ret = fsg_common_run_thread(fsg_opts->common);
	if (ret)
		goto err_run;

	ret = usb_add_function(c, f_msg_multi);
	if (ret)
		goto err_run;

	return 0;
err_run:
	usb_put_function(f_msg_multi);
err_fsg:
	usb_remove_function(c, f_acm_multi);
err_conf:
	usb_put_function(f_acm_multi);
err_func_acm:
	usb_remove_function(c, f_ecm);
err_func_ecm:
	usb_put_function(f_ecm);
	return ret;
}

static __ref int cdc_config_register(struct usb_composite_dev *cdev)
>>>>>>> refs/remotes/origin/master
{
	static struct usb_configuration config = {
		.bConfigurationValue	= MULTI_CDC_CONFIG_NUM,
		.bmAttributes		= USB_CONFIG_ATT_SELFPOWER,
	};

	config.label          = strings_dev[MULTI_STRING_CDC_CONFIG_IDX].s;
	config.iConfiguration = strings_dev[MULTI_STRING_CDC_CONFIG_IDX].id;

	return usb_add_config(cdev, &config, cdc_do_config);
}

#else

<<<<<<< HEAD
static int cdc_config_register(struct usb_composite_dev *cdev)
=======
static __ref int cdc_config_register(struct usb_composite_dev *cdev)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

#endif



/****************************** Gadget Bind ******************************/

<<<<<<< HEAD

static int __ref multi_bind(struct usb_composite_dev *cdev)
{
	struct usb_gadget *gadget = cdev->gadget;
	int status, gcnum;

	if (!can_support_ecm(cdev->gadget)) {
		dev_err(&gadget->dev, "controller '%s' not usable\n",
		        gadget->name);
		return -EINVAL;
	}

	/* set up network link layer */
	status = gether_setup(cdev->gadget, hostaddr);
	if (status < 0)
		return status;

	/* set up serial link layer */
	status = gserial_setup(cdev->gadget, 1);
	if (status < 0)
		goto fail0;

	/* set up mass storage function */
	{
		void *retp;
		retp = fsg_common_from_params(&fsg_common, cdev, &fsg_mod_data);
		if (IS_ERR(retp)) {
			status = PTR_ERR(retp);
			goto fail1;
		}
	}

	/* set bcdDevice */
	gcnum = usb_gadget_controller_number(gadget);
	if (gcnum >= 0) {
		device_desc.bcdDevice = cpu_to_le16(0x0300 | gcnum);
	} else {
		WARNING(cdev, "controller '%s' not recognized\n", gadget->name);
		device_desc.bcdDevice = cpu_to_le16(0x0300 | 0x0099);
	}
=======
static int __ref multi_bind(struct usb_composite_dev *cdev)
{
	struct usb_gadget *gadget = cdev->gadget;
#ifdef CONFIG_USB_G_MULTI_CDC
	struct f_ecm_opts *ecm_opts;
#endif
#ifdef USB_ETH_RNDIS
	struct f_rndis_opts *rndis_opts;
#endif
	struct fsg_opts *fsg_opts;
	struct fsg_config config;
	int status;

	if (!can_support_ecm(cdev->gadget)) {
		dev_err(&gadget->dev, "controller '%s' not usable\n",
			gadget->name);
		return -EINVAL;
	}

#ifdef CONFIG_USB_G_MULTI_CDC
	fi_ecm = usb_get_function_instance("ecm");
	if (IS_ERR(fi_ecm))
		return PTR_ERR(fi_ecm);

	ecm_opts = container_of(fi_ecm, struct f_ecm_opts, func_inst);

	gether_set_qmult(ecm_opts->net, qmult);
	if (!gether_set_host_addr(ecm_opts->net, host_addr))
		pr_info("using host ethernet address: %s", host_addr);
	if (!gether_set_dev_addr(ecm_opts->net, dev_addr))
		pr_info("using self ethernet address: %s", dev_addr);
#endif

#ifdef USB_ETH_RNDIS
	fi_rndis = usb_get_function_instance("rndis");
	if (IS_ERR(fi_rndis)) {
		status = PTR_ERR(fi_rndis);
		goto fail;
	}

	rndis_opts = container_of(fi_rndis, struct f_rndis_opts, func_inst);

	gether_set_qmult(rndis_opts->net, qmult);
	if (!gether_set_host_addr(rndis_opts->net, host_addr))
		pr_info("using host ethernet address: %s", host_addr);
	if (!gether_set_dev_addr(rndis_opts->net, dev_addr))
		pr_info("using self ethernet address: %s", dev_addr);
#endif

#if (defined CONFIG_USB_G_MULTI_CDC && defined USB_ETH_RNDIS)
	/*
	 * If both ecm and rndis are selected then:
	 *	1) rndis borrows the net interface from ecm
	 *	2) since the interface is shared it must not be bound
	 *	twice - in ecm's _and_ rndis' binds, so do it here.
	 */
	gether_set_gadget(ecm_opts->net, cdev->gadget);
	status = gether_register_netdev(ecm_opts->net);
	if (status)
		goto fail0;

	rndis_borrow_net(fi_rndis, ecm_opts->net);
	ecm_opts->bound = true;
#endif

	/* set up serial link layer */
	fi_acm = usb_get_function_instance("acm");
	if (IS_ERR(fi_acm)) {
		status = PTR_ERR(fi_acm);
		goto fail0;
	}

	/* set up mass storage function */
	fi_msg = usb_get_function_instance("mass_storage");
	if (IS_ERR(fi_msg)) {
		status = PTR_ERR(fi_msg);
		goto fail1;
	}
	fsg_config_from_params(&config, &fsg_mod_data, fsg_num_buffers);
	fsg_opts = fsg_opts_from_func_inst(fi_msg);

	fsg_opts->no_configfs = true;
	status = fsg_common_set_num_buffers(fsg_opts->common, fsg_num_buffers);
	if (status)
		goto fail2;

	status = fsg_common_set_nluns(fsg_opts->common, config.nluns);
	if (status)
		goto fail_set_nluns;

	status = fsg_common_set_cdev(fsg_opts->common, cdev, config.can_stall);
	if (status)
		goto fail_set_cdev;

	fsg_common_set_sysfs(fsg_opts->common, true);
	status = fsg_common_create_luns(fsg_opts->common, &config);
	if (status)
		goto fail_set_cdev;

	fsg_common_set_inquiry_string(fsg_opts->common, config.vendor_name,
				      config.product_name);
>>>>>>> refs/remotes/origin/master

	/* allocate string IDs */
	status = usb_string_ids_tab(cdev, strings_dev);
	if (unlikely(status < 0))
<<<<<<< HEAD
		goto fail2;
=======
		goto fail_string_ids;
	device_desc.iProduct = strings_dev[USB_GADGET_PRODUCT_IDX].id;
>>>>>>> refs/remotes/origin/master

	/* register configurations */
	status = rndis_config_register(cdev);
	if (unlikely(status < 0))
<<<<<<< HEAD
		goto fail2;

	status = cdc_config_register(cdev);
	if (unlikely(status < 0))
		goto fail2;

	/* we're done */
	dev_info(&gadget->dev, DRIVER_DESC "\n");
	fsg_common_put(&fsg_common);
=======
		goto fail_string_ids;

	status = cdc_config_register(cdev);
	if (unlikely(status < 0))
		goto fail_string_ids;
	usb_composite_overwrite_options(cdev, &coverwrite);

	/* we're done */
	dev_info(&gadget->dev, DRIVER_DESC "\n");
>>>>>>> refs/remotes/origin/master
	return 0;


	/* error recovery */
<<<<<<< HEAD
fail2:
	fsg_common_put(&fsg_common);
fail1:
	gserial_cleanup();
fail0:
	gether_cleanup();
=======
fail_string_ids:
	fsg_common_remove_luns(fsg_opts->common);
fail_set_cdev:
	fsg_common_free_luns(fsg_opts->common);
fail_set_nluns:
	fsg_common_free_buffers(fsg_opts->common);
fail2:
	usb_put_function_instance(fi_msg);
fail1:
	usb_put_function_instance(fi_acm);
fail0:
#ifdef USB_ETH_RNDIS
	usb_put_function_instance(fi_rndis);
fail:
#endif
#ifdef CONFIG_USB_G_MULTI_CDC
	usb_put_function_instance(fi_ecm);
#endif
>>>>>>> refs/remotes/origin/master
	return status;
}

static int __exit multi_unbind(struct usb_composite_dev *cdev)
{
<<<<<<< HEAD
	gserial_cleanup();
	gether_cleanup();
=======
#ifdef CONFIG_USB_G_MULTI_CDC
	usb_put_function(f_msg_multi);
#endif
#ifdef USB_ETH_RNDIS
	usb_put_function(f_msg_rndis);
#endif
	usb_put_function_instance(fi_msg);
#ifdef CONFIG_USB_G_MULTI_CDC
	usb_put_function(f_acm_multi);
#endif
#ifdef USB_ETH_RNDIS
	usb_put_function(f_acm_rndis);
#endif
	usb_put_function_instance(fi_acm);
#ifdef USB_ETH_RNDIS
	usb_put_function(f_rndis);
	usb_put_function_instance(fi_rndis);
#endif
#ifdef CONFIG_USB_G_MULTI_CDC
	usb_put_function(f_ecm);
	usb_put_function_instance(fi_ecm);
#endif
>>>>>>> refs/remotes/origin/master
	return 0;
}


/****************************** Some noise ******************************/


<<<<<<< HEAD
static struct usb_composite_driver multi_driver = {
	.name		= "g_multi",
	.dev		= &device_desc,
	.strings	= dev_strings,
<<<<<<< HEAD
=======
	.max_speed	= USB_SPEED_HIGH,
>>>>>>> refs/remotes/origin/cm-10.0
	.unbind		= __exit_p(multi_unbind),
	.iProduct	= DRIVER_DESC,
=======
static __refdata struct usb_composite_driver multi_driver = {
	.name		= "g_multi",
	.dev		= &device_desc,
	.strings	= dev_strings,
	.max_speed	= USB_SPEED_HIGH,
	.bind		= multi_bind,
	.unbind		= __exit_p(multi_unbind),
>>>>>>> refs/remotes/origin/master
	.needs_serial	= 1,
};


static int __init multi_init(void)
{
<<<<<<< HEAD
	return usb_composite_probe(&multi_driver, multi_bind);
=======
	return usb_composite_probe(&multi_driver);
>>>>>>> refs/remotes/origin/master
}
module_init(multi_init);

static void __exit multi_exit(void)
{
	usb_composite_unregister(&multi_driver);
}
module_exit(multi_exit);
