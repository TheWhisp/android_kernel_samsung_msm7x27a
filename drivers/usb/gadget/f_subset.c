/*
 * f_subset.c -- "CDC Subset" Ethernet link function driver
 *
 * Copyright (C) 2003-2005,2008 David Brownell
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

#include <linux/slab.h>
#include <linux/kernel.h>
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master
#include <linux/device.h>
#include <linux/etherdevice.h>

#include "u_ether.h"
<<<<<<< HEAD

=======
#include "u_ether_configfs.h"
#include "u_gether.h"
>>>>>>> refs/remotes/origin/master

/*
 * This function packages a simple "CDC Subset" Ethernet port with no real
 * control mechanisms; just raw data transfer over two bulk endpoints.
 * The data transfer model is exactly that of CDC Ethernet, which is
 * why we call it the "CDC Subset".
 *
 * Because it's not standardized, this has some interoperability issues.
 * They mostly relate to driver binding, since the data transfer model is
 * so simple (CDC Ethernet).  The original versions of this protocol used
 * specific product/vendor IDs:  byteswapped IDs for Digital Equipment's
 * SA-1100 "Itsy" board, which could run Linux 2.4 kernels and supported
 * daughtercards with USB peripheral connectors.  (It was used more often
 * with other boards, using the Itsy identifiers.)  Linux hosts recognized
 * this with CONFIG_USB_ARMLINUX; these devices have only one configuration
 * and one interface.
 *
 * At some point, MCCI defined a (nonconformant) CDC MDLM variant called
 * "SAFE", which happens to have a mode which is identical to the "CDC
 * Subset" in terms of data transfer and lack of control model.  This was
 * adopted by later Sharp Zaurus models, and by some other software which
 * Linux hosts recognize with CONFIG_USB_NET_ZAURUS.
 *
 * Because Microsoft's RNDIS drivers are far from robust, we added a few
 * descriptors to the CDC Subset code, making this code look like a SAFE
 * implementation.  This lets you use MCCI's host side MS-Windows drivers
 * if you get fed up with RNDIS.  It also makes it easier for composite
 * drivers to work, since they can use class based binding instead of
 * caring about specific product and vendor IDs.
 */

<<<<<<< HEAD
<<<<<<< HEAD
struct geth_descs {
	struct usb_endpoint_descriptor	*in;
	struct usb_endpoint_descriptor	*out;
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct f_gether {
	struct gether			port;

	char				ethaddr[14];
<<<<<<< HEAD
<<<<<<< HEAD

	struct geth_descs		fs;
	struct geth_descs		hs;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

static inline struct f_gether *func_to_geth(struct usb_function *f)
{
	return container_of(f, struct f_gether, port.func);
}

/*-------------------------------------------------------------------------*/

/*
 * "Simple" CDC-subset option is a simple vendor-neutral model that most
 * full speed controllers can handle:  one interface, two bulk endpoints.
 * To assist host side drivers, we fancy it up a bit, and add descriptors so
 * some host side drivers will understand it as a "SAFE" variant.
 *
 * "SAFE" loosely follows CDC WMC MDLM, violating the spec in various ways.
 * Data endpoints live in the control interface, there's no data interface.
 * And it's not used to talk to a cell phone radio.
 */

/* interface descriptor: */

<<<<<<< HEAD
<<<<<<< HEAD
static struct usb_interface_descriptor subset_data_intf __initdata = {
=======
static struct usb_interface_descriptor subset_data_intf = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct usb_interface_descriptor subset_data_intf = {
>>>>>>> refs/remotes/origin/master
	.bLength =		sizeof subset_data_intf,
	.bDescriptorType =	USB_DT_INTERFACE,

	/* .bInterfaceNumber = DYNAMIC */
	.bAlternateSetting =	0,
	.bNumEndpoints =	2,
	.bInterfaceClass =      USB_CLASS_COMM,
	.bInterfaceSubClass =	USB_CDC_SUBCLASS_MDLM,
	.bInterfaceProtocol =	0,
	/* .iInterface = DYNAMIC */
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct usb_cdc_header_desc mdlm_header_desc __initdata = {
=======
static struct usb_cdc_header_desc mdlm_header_desc = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct usb_cdc_header_desc mdlm_header_desc = {
>>>>>>> refs/remotes/origin/master
	.bLength =		sizeof mdlm_header_desc,
	.bDescriptorType =	USB_DT_CS_INTERFACE,
	.bDescriptorSubType =	USB_CDC_HEADER_TYPE,

	.bcdCDC =		cpu_to_le16(0x0110),
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct usb_cdc_mdlm_desc mdlm_desc __initdata = {
=======
static struct usb_cdc_mdlm_desc mdlm_desc = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct usb_cdc_mdlm_desc mdlm_desc = {
>>>>>>> refs/remotes/origin/master
	.bLength =		sizeof mdlm_desc,
	.bDescriptorType =	USB_DT_CS_INTERFACE,
	.bDescriptorSubType =	USB_CDC_MDLM_TYPE,

	.bcdVersion =		cpu_to_le16(0x0100),
	.bGUID = {
		0x5d, 0x34, 0xcf, 0x66, 0x11, 0x18, 0x11, 0xd6,
		0xa2, 0x1a, 0x00, 0x01, 0x02, 0xca, 0x9a, 0x7f,
	},
};

/* since "usb_cdc_mdlm_detail_desc" is a variable length structure, we
 * can't really use its struct.  All we do here is say that we're using
 * the submode of "SAFE" which directly matches the CDC Subset.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static u8 mdlm_detail_desc[] __initdata = {
=======
static u8 mdlm_detail_desc[] = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static u8 mdlm_detail_desc[] = {
>>>>>>> refs/remotes/origin/master
	6,
	USB_DT_CS_INTERFACE,
	USB_CDC_MDLM_DETAIL_TYPE,

	0,	/* "SAFE" */
	0,	/* network control capabilities (none) */
	0,	/* network data capabilities ("raw" encapsulation) */
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct usb_cdc_ether_desc ether_desc __initdata = {
=======
static struct usb_cdc_ether_desc ether_desc = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct usb_cdc_ether_desc ether_desc = {
>>>>>>> refs/remotes/origin/master
	.bLength =		sizeof ether_desc,
	.bDescriptorType =	USB_DT_CS_INTERFACE,
	.bDescriptorSubType =	USB_CDC_ETHERNET_TYPE,

	/* this descriptor actually adds value, surprise! */
	/* .iMACAddress = DYNAMIC */
	.bmEthernetStatistics =	cpu_to_le32(0), /* no statistics */
	.wMaxSegmentSize =	cpu_to_le16(ETH_FRAME_LEN),
	.wNumberMCFilters =	cpu_to_le16(0),
	.bNumberPowerFilters =	0,
};

/* full speed support: */

<<<<<<< HEAD
<<<<<<< HEAD
static struct usb_endpoint_descriptor fs_subset_in_desc __initdata = {
=======
static struct usb_endpoint_descriptor fs_subset_in_desc = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct usb_endpoint_descriptor fs_subset_in_desc = {
>>>>>>> refs/remotes/origin/master
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct usb_endpoint_descriptor fs_subset_out_desc __initdata = {
=======
static struct usb_endpoint_descriptor fs_subset_out_desc = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct usb_endpoint_descriptor fs_subset_out_desc = {
>>>>>>> refs/remotes/origin/master
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_OUT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct usb_descriptor_header *fs_eth_function[] __initdata = {
=======
static struct usb_descriptor_header *fs_eth_function[] = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct usb_descriptor_header *fs_eth_function[] = {
>>>>>>> refs/remotes/origin/master
	(struct usb_descriptor_header *) &subset_data_intf,
	(struct usb_descriptor_header *) &mdlm_header_desc,
	(struct usb_descriptor_header *) &mdlm_desc,
	(struct usb_descriptor_header *) &mdlm_detail_desc,
	(struct usb_descriptor_header *) &ether_desc,
	(struct usb_descriptor_header *) &fs_subset_in_desc,
	(struct usb_descriptor_header *) &fs_subset_out_desc,
	NULL,
};

/* high speed support: */

<<<<<<< HEAD
<<<<<<< HEAD
static struct usb_endpoint_descriptor hs_subset_in_desc __initdata = {
=======
static struct usb_endpoint_descriptor hs_subset_in_desc = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct usb_endpoint_descriptor hs_subset_in_desc = {
>>>>>>> refs/remotes/origin/master
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(512),
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct usb_endpoint_descriptor hs_subset_out_desc __initdata = {
=======
static struct usb_endpoint_descriptor hs_subset_out_desc = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct usb_endpoint_descriptor hs_subset_out_desc = {
>>>>>>> refs/remotes/origin/master
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(512),
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct usb_descriptor_header *hs_eth_function[] __initdata = {
=======
static struct usb_descriptor_header *hs_eth_function[] = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct usb_descriptor_header *hs_eth_function[] = {
>>>>>>> refs/remotes/origin/master
	(struct usb_descriptor_header *) &subset_data_intf,
	(struct usb_descriptor_header *) &mdlm_header_desc,
	(struct usb_descriptor_header *) &mdlm_desc,
	(struct usb_descriptor_header *) &mdlm_detail_desc,
	(struct usb_descriptor_header *) &ether_desc,
	(struct usb_descriptor_header *) &hs_subset_in_desc,
	(struct usb_descriptor_header *) &hs_subset_out_desc,
	NULL,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/* super speed support: */

static struct usb_endpoint_descriptor ss_subset_in_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(1024),
};

static struct usb_endpoint_descriptor ss_subset_out_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(1024),
};

static struct usb_ss_ep_comp_descriptor ss_subset_bulk_comp_desc = {
	.bLength =		sizeof ss_subset_bulk_comp_desc,
	.bDescriptorType =	USB_DT_SS_ENDPOINT_COMP,

	/* the following 2 values can be tweaked if necessary */
	/* .bMaxBurst =		0, */
	/* .bmAttributes =	0, */
};

static struct usb_descriptor_header *ss_eth_function[] = {
	(struct usb_descriptor_header *) &subset_data_intf,
	(struct usb_descriptor_header *) &mdlm_header_desc,
	(struct usb_descriptor_header *) &mdlm_desc,
	(struct usb_descriptor_header *) &mdlm_detail_desc,
	(struct usb_descriptor_header *) &ether_desc,
	(struct usb_descriptor_header *) &ss_subset_in_desc,
	(struct usb_descriptor_header *) &ss_subset_bulk_comp_desc,
	(struct usb_descriptor_header *) &ss_subset_out_desc,
	(struct usb_descriptor_header *) &ss_subset_bulk_comp_desc,
	NULL,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* string descriptors: */

static struct usb_string geth_string_defs[] = {
	[0].s = "CDC Ethernet Subset/SAFE",
<<<<<<< HEAD
	[1].s = NULL /* DYNAMIC */,
=======
	[1].s = "",
>>>>>>> refs/remotes/origin/master
	{  } /* end of list */
};

static struct usb_gadget_strings geth_string_table = {
	.language =		0x0409,	/* en-us */
	.strings =		geth_string_defs,
};

static struct usb_gadget_strings *geth_strings[] = {
	&geth_string_table,
	NULL,
};

/*-------------------------------------------------------------------------*/

static int geth_set_alt(struct usb_function *f, unsigned intf, unsigned alt)
{
	struct f_gether		*geth = func_to_geth(f);
	struct usb_composite_dev *cdev = f->config->cdev;
	struct net_device	*net;

	/* we know alt == 0, so this is an activation or a reset */

	if (geth->port.in_ep->driver_data) {
		DBG(cdev, "reset cdc subset\n");
		gether_disconnect(&geth->port);
	}

	DBG(cdev, "init + activate cdc subset\n");
<<<<<<< HEAD
<<<<<<< HEAD
	geth->port.in = ep_choose(cdev->gadget,
			geth->hs.in, geth->fs.in);
	geth->port.out = ep_choose(cdev->gadget,
			geth->hs.out, geth->fs.out);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (config_ep_by_speed(cdev->gadget, f, geth->port.in_ep) ||
	    config_ep_by_speed(cdev->gadget, f, geth->port.out_ep)) {
		geth->port.in_ep->desc = NULL;
		geth->port.out_ep->desc = NULL;
		return -EINVAL;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	net = gether_connect(&geth->port);
	return IS_ERR(net) ? PTR_ERR(net) : 0;
}

static void geth_disable(struct usb_function *f)
{
	struct f_gether	*geth = func_to_geth(f);
	struct usb_composite_dev *cdev = f->config->cdev;

	DBG(cdev, "net deactivated\n");
	gether_disconnect(&geth->port);
}

/*-------------------------------------------------------------------------*/

/* serial function driver setup/binding */

<<<<<<< HEAD
<<<<<<< HEAD
static int __init
=======
static int
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int
>>>>>>> refs/remotes/origin/master
geth_bind(struct usb_configuration *c, struct usb_function *f)
{
	struct usb_composite_dev *cdev = c->cdev;
	struct f_gether		*geth = func_to_geth(f);
<<<<<<< HEAD
	int			status;
	struct usb_ep		*ep;

=======
	struct usb_string	*us;
	int			status;
	struct usb_ep		*ep;

	struct f_gether_opts	*gether_opts;

	gether_opts = container_of(f->fi, struct f_gether_opts, func_inst);

	/*
	 * in drivers/usb/gadget/configfs.c:configfs_composite_bind()
	 * configurations are bound in sequence with list_for_each_entry,
	 * in each configuration its functions are bound in sequence
	 * with list_for_each_entry, so we assume no race condition
	 * with regard to gether_opts->bound access
	 */
	if (!gether_opts->bound) {
		mutex_lock(&gether_opts->lock);
		gether_set_gadget(gether_opts->net, cdev->gadget);
		status = gether_register_netdev(gether_opts->net);
		mutex_unlock(&gether_opts->lock);
		if (status)
			return status;
		gether_opts->bound = true;
	}

	us = usb_gstrings_attach(cdev, geth_strings,
				 ARRAY_SIZE(geth_string_defs));
	if (IS_ERR(us))
		return PTR_ERR(us);

	subset_data_intf.iInterface = us[0].id;
	ether_desc.iMACAddress = us[1].id;

>>>>>>> refs/remotes/origin/master
	/* allocate instance-specific interface IDs */
	status = usb_interface_id(c, f);
	if (status < 0)
		goto fail;
	subset_data_intf.bInterfaceNumber = status;

	status = -ENODEV;

	/* allocate instance-specific endpoints */
	ep = usb_ep_autoconfig(cdev->gadget, &fs_subset_in_desc);
	if (!ep)
		goto fail;
	geth->port.in_ep = ep;
	ep->driver_data = cdev;	/* claim */

	ep = usb_ep_autoconfig(cdev->gadget, &fs_subset_out_desc);
	if (!ep)
		goto fail;
	geth->port.out_ep = ep;
	ep->driver_data = cdev;	/* claim */

<<<<<<< HEAD
	/* copy descriptors, and track endpoint copies */
	f->descriptors = usb_copy_descriptors(fs_eth_function);
<<<<<<< HEAD

	geth->fs.in = usb_find_endpoint(fs_eth_function,
			f->descriptors, &fs_subset_in_desc);
	geth->fs.out = usb_find_endpoint(fs_eth_function,
			f->descriptors, &fs_subset_out_desc);

=======
	if (!f->descriptors)
		goto fail;
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
	/* support all relevant hardware speeds... we expect that when
	 * hardware is dual speed, all bulk-capable endpoints work at
	 * both speeds
	 */
<<<<<<< HEAD
	if (gadget_is_dualspeed(c->cdev->gadget)) {
		hs_subset_in_desc.bEndpointAddress =
				fs_subset_in_desc.bEndpointAddress;
		hs_subset_out_desc.bEndpointAddress =
				fs_subset_out_desc.bEndpointAddress;

		/* copy descriptors, and track endpoint copies */
		f->hs_descriptors = usb_copy_descriptors(hs_eth_function);
<<<<<<< HEAD

		geth->hs.in = usb_find_endpoint(hs_eth_function,
				f->hs_descriptors, &hs_subset_in_desc);
		geth->hs.out = usb_find_endpoint(hs_eth_function,
				f->hs_descriptors, &hs_subset_out_desc);
=======
		if (!f->hs_descriptors)
			goto fail;
	}

	if (gadget_is_superspeed(c->cdev->gadget)) {
		ss_subset_in_desc.bEndpointAddress =
				fs_subset_in_desc.bEndpointAddress;
		ss_subset_out_desc.bEndpointAddress =
				fs_subset_out_desc.bEndpointAddress;

		/* copy descriptors, and track endpoint copies */
		f->ss_descriptors = usb_copy_descriptors(ss_eth_function);
		if (!f->ss_descriptors)
			goto fail;
>>>>>>> refs/remotes/origin/cm-10.0
	}
=======
	hs_subset_in_desc.bEndpointAddress = fs_subset_in_desc.bEndpointAddress;
	hs_subset_out_desc.bEndpointAddress =
		fs_subset_out_desc.bEndpointAddress;

	ss_subset_in_desc.bEndpointAddress = fs_subset_in_desc.bEndpointAddress;
	ss_subset_out_desc.bEndpointAddress =
		fs_subset_out_desc.bEndpointAddress;

	status = usb_assign_descriptors(f, fs_eth_function, hs_eth_function,
			ss_eth_function);
	if (status)
		goto fail;
>>>>>>> refs/remotes/origin/master

	/* NOTE:  all that is done without knowing or caring about
	 * the network link ... which is unavailable to this code
	 * until we're activated via set_alt().
	 */

	DBG(cdev, "CDC Subset: %s speed IN/%s OUT/%s\n",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			gadget_is_superspeed(c->cdev->gadget) ? "super" :
>>>>>>> refs/remotes/origin/cm-10.0
=======
			gadget_is_superspeed(c->cdev->gadget) ? "super" :
>>>>>>> refs/remotes/origin/master
			gadget_is_dualspeed(c->cdev->gadget) ? "dual" : "full",
			geth->port.in_ep->name, geth->port.out_ep->name);
	return 0;

fail:
<<<<<<< HEAD
<<<<<<< HEAD
	/* we might as well release our claims on endpoints */
	if (geth->port.out)
		geth->port.out_ep->driver_data = NULL;
	if (geth->port.in)
=======
	if (f->descriptors)
		usb_free_descriptors(f->descriptors);
	if (f->hs_descriptors)
		usb_free_descriptors(f->hs_descriptors);

=======
	usb_free_all_descriptors(f);
>>>>>>> refs/remotes/origin/master
	/* we might as well release our claims on endpoints */
	if (geth->port.out_ep)
		geth->port.out_ep->driver_data = NULL;
	if (geth->port.in_ep)
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		geth->port.in_ep->driver_data = NULL;

	ERROR(cdev, "%s: can't bind, err %d\n", f->name, status);

	return status;
}

<<<<<<< HEAD
static void
geth_unbind(struct usb_configuration *c, struct usb_function *f)
{
<<<<<<< HEAD
=======
	if (gadget_is_superspeed(c->cdev->gadget))
		usb_free_descriptors(f->ss_descriptors);
>>>>>>> refs/remotes/origin/cm-10.0
	if (gadget_is_dualspeed(c->cdev->gadget))
		usb_free_descriptors(f->hs_descriptors);
	usb_free_descriptors(f->descriptors);
	geth_string_defs[1].s = NULL;
	kfree(func_to_geth(f));
}

/**
 * geth_bind_config - add CDC Subset network link to a configuration
 * @c: the configuration to support the network link
 * @ethaddr: a buffer in which the ethernet address of the host side
 *	side of the link was recorded
 * Context: single threaded during gadget setup
 *
 * Returns zero on success, else negative errno.
 *
 * Caller must have called @gether_setup().  Caller is also responsible
 * for calling @gether_cleanup() before module unload.
 */
<<<<<<< HEAD
int __init geth_bind_config(struct usb_configuration *c, u8 ethaddr[ETH_ALEN])
=======
int geth_bind_config(struct usb_configuration *c, u8 ethaddr[ETH_ALEN])
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct f_gether	*geth;
	int		status;

	if (!ethaddr)
		return -EINVAL;

	/* maybe allocate device-global string IDs */
	if (geth_string_defs[0].id == 0) {

		/* interface label */
		status = usb_string_id(c->cdev);
		if (status < 0)
			return status;
		geth_string_defs[0].id = status;
		subset_data_intf.iInterface = status;

		/* MAC address */
		status = usb_string_id(c->cdev);
		if (status < 0)
			return status;
		geth_string_defs[1].id = status;
		ether_desc.iMACAddress = status;
	}

	/* allocate and initialize one new instance */
	geth = kzalloc(sizeof *geth, GFP_KERNEL);
	if (!geth)
		return -ENOMEM;

	/* export host's Ethernet address in CDC format */
	snprintf(geth->ethaddr, sizeof geth->ethaddr,
		"%02X%02X%02X%02X%02X%02X",
		ethaddr[0], ethaddr[1], ethaddr[2],
		ethaddr[3], ethaddr[4], ethaddr[5]);
	geth_string_defs[1].s = geth->ethaddr;

	geth->port.cdc_filter = DEFAULT_FILTER;

	geth->port.func.name = "cdc_subset";
	geth->port.func.strings = geth_strings;
=======
static inline struct f_gether_opts *to_f_gether_opts(struct config_item *item)
{
	return container_of(to_config_group(item), struct f_gether_opts,
			    func_inst.group);
}

/* f_gether_item_ops */
USB_ETHERNET_CONFIGFS_ITEM(gether);

/* f_gether_opts_dev_addr */
USB_ETHERNET_CONFIGFS_ITEM_ATTR_DEV_ADDR(gether);

/* f_gether_opts_host_addr */
USB_ETHERNET_CONFIGFS_ITEM_ATTR_HOST_ADDR(gether);

/* f_gether_opts_qmult */
USB_ETHERNET_CONFIGFS_ITEM_ATTR_QMULT(gether);

/* f_gether_opts_ifname */
USB_ETHERNET_CONFIGFS_ITEM_ATTR_IFNAME(gether);

static struct configfs_attribute *gether_attrs[] = {
	&f_gether_opts_dev_addr.attr,
	&f_gether_opts_host_addr.attr,
	&f_gether_opts_qmult.attr,
	&f_gether_opts_ifname.attr,
	NULL,
};

static struct config_item_type gether_func_type = {
	.ct_item_ops	= &gether_item_ops,
	.ct_attrs	= gether_attrs,
	.ct_owner	= THIS_MODULE,
};

static void geth_free_inst(struct usb_function_instance *f)
{
	struct f_gether_opts *opts;

	opts = container_of(f, struct f_gether_opts, func_inst);
	if (opts->bound)
		gether_cleanup(netdev_priv(opts->net));
	else
		free_netdev(opts->net);
	kfree(opts);
}

static struct usb_function_instance *geth_alloc_inst(void)
{
	struct f_gether_opts *opts;

	opts = kzalloc(sizeof(*opts), GFP_KERNEL);
	if (!opts)
		return ERR_PTR(-ENOMEM);
	mutex_init(&opts->lock);
	opts->func_inst.free_func_inst = geth_free_inst;
	opts->net = gether_setup_default();
	if (IS_ERR(opts->net)) {
		struct net_device *net = opts->net;
		kfree(opts);
		return ERR_CAST(net);
	}

	config_group_init_type_name(&opts->func_inst.group, "",
				    &gether_func_type);

	return &opts->func_inst;
}

static void geth_free(struct usb_function *f)
{
	struct f_gether *eth;

	eth = func_to_geth(f);
	kfree(eth);
}

static void geth_unbind(struct usb_configuration *c, struct usb_function *f)
{
	geth_string_defs[0].id = 0;
	usb_free_all_descriptors(f);
}

static struct usb_function *geth_alloc(struct usb_function_instance *fi)
{
	struct f_gether	*geth;
	struct f_gether_opts *opts;
	int status;

	/* allocate and initialize one new instance */
	geth = kzalloc(sizeof(*geth), GFP_KERNEL);
	if (!geth)
		return ERR_PTR(-ENOMEM);

	opts = container_of(fi, struct f_gether_opts, func_inst);

	mutex_lock(&opts->lock);
	opts->refcnt++;
	/* export host's Ethernet address in CDC format */
	status = gether_get_host_addr_cdc(opts->net, geth->ethaddr,
					  sizeof(geth->ethaddr));
	if (status < 12) {
		kfree(geth);
		mutex_unlock(&opts->lock);
		return ERR_PTR(-EINVAL);
	}
	geth_string_defs[1].s = geth->ethaddr;

	geth->port.ioport = netdev_priv(opts->net);
	mutex_unlock(&opts->lock);
	geth->port.cdc_filter = DEFAULT_FILTER;

	geth->port.func.name = "cdc_subset";
>>>>>>> refs/remotes/origin/master
	geth->port.func.bind = geth_bind;
	geth->port.func.unbind = geth_unbind;
	geth->port.func.set_alt = geth_set_alt;
	geth->port.func.disable = geth_disable;
<<<<<<< HEAD

	status = usb_add_function(c, &geth->port.func);
	if (status) {
		geth_string_defs[1].s = NULL;
		kfree(geth);
	}
	return status;
}
=======
	geth->port.func.free_func = geth_free;

	return &geth->port.func;
}

DECLARE_USB_FUNCTION_INIT(geth, geth_alloc_inst, geth_alloc);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Brownell");
>>>>>>> refs/remotes/origin/master
