/*
 * drivers/input/tablet/wacom_sys.c
 *
 *  USB Wacom tablet support - system specific code
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "wacom_wac.h"
#include "wacom.h"

/* defines to get HID report descriptor */
#define HID_DEVICET_HID		(USB_TYPE_CLASS | 0x01)
#define HID_DEVICET_REPORT	(USB_TYPE_CLASS | 0x02)
#define HID_USAGE_UNDEFINED		0x00
#define HID_USAGE_PAGE			0x05
#define HID_USAGE_PAGE_DIGITIZER	0x0d
#define HID_USAGE_PAGE_DESKTOP		0x01
#define HID_USAGE			0x09
#define HID_USAGE_X			0x30
#define HID_USAGE_Y			0x31
#define HID_USAGE_X_TILT		0x3d
#define HID_USAGE_Y_TILT		0x3e
#define HID_USAGE_FINGER		0x22
#define HID_USAGE_STYLUS		0x20
<<<<<<< HEAD
<<<<<<< HEAD
#define HID_COLLECTION			0xc0
=======
#define HID_COLLECTION			0xa1
#define HID_COLLECTION_LOGICAL		0x02
#define HID_COLLECTION_END		0xc0
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define HID_USAGE_CONTACTMAX		0x55
#define HID_COLLECTION			0xa1
#define HID_COLLECTION_LOGICAL		0x02
#define HID_COLLECTION_END		0xc0
>>>>>>> refs/remotes/origin/master

enum {
	WCM_UNDEFINED = 0,
	WCM_DESKTOP,
	WCM_DIGITIZER,
};

struct hid_descriptor {
	struct usb_descriptor_header header;
	__le16   bcdHID;
	u8       bCountryCode;
	u8       bNumDescriptors;
	u8       bDescriptorType;
	__le16   wDescriptorLength;
} __attribute__ ((packed));

/* defines to get/set USB message */
#define USB_REQ_GET_REPORT	0x01
#define USB_REQ_SET_REPORT	0x09
<<<<<<< HEAD
<<<<<<< HEAD
#define WAC_HID_FEATURE_REPORT	0x03

static int usb_get_report(struct usb_interface *intf, unsigned char type,
				unsigned char id, void *buf, int size)
{
	return usb_control_msg(interface_to_usbdev(intf),
		usb_rcvctrlpipe(interface_to_usbdev(intf), 0),
		USB_REQ_GET_REPORT, USB_TYPE_CLASS | USB_RECIP_INTERFACE,
		(type << 8) + id, intf->altsetting[0].desc.bInterfaceNumber,
		buf, size, 100);
}

static int usb_set_report(struct usb_interface *intf, unsigned char type,
				unsigned char id, void *buf, int size)
{
	return usb_control_msg(interface_to_usbdev(intf),
		usb_sndctrlpipe(interface_to_usbdev(intf), 0),
                USB_REQ_SET_REPORT, USB_TYPE_CLASS | USB_RECIP_INTERFACE,
                (type << 8) + id, intf->altsetting[0].desc.bInterfaceNumber,
		buf, size, 1000);
=======
=======
>>>>>>> refs/remotes/origin/master

#define WAC_HID_FEATURE_REPORT	0x03
#define WAC_MSG_RETRIES		5

#define WAC_CMD_LED_CONTROL	0x20
#define WAC_CMD_ICON_START	0x21
#define WAC_CMD_ICON_XFER	0x23
#define WAC_CMD_RETRIES		10

static int wacom_get_report(struct usb_interface *intf, u8 type, u8 id,
			    void *buf, size_t size, unsigned int retries)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	int retval;

	do {
		retval = usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
				USB_REQ_GET_REPORT,
				USB_DIR_IN | USB_TYPE_CLASS |
				USB_RECIP_INTERFACE,
				(type << 8) + id,
				intf->altsetting[0].desc.bInterfaceNumber,
				buf, size, 100);
	} while ((retval == -ETIMEDOUT || retval == -EPIPE) && --retries);

	return retval;
}

static int wacom_set_report(struct usb_interface *intf, u8 type, u8 id,
			    void *buf, size_t size, unsigned int retries)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	int retval;

	do {
		retval = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
				USB_REQ_SET_REPORT,
				USB_TYPE_CLASS | USB_RECIP_INTERFACE,
				(type << 8) + id,
				intf->altsetting[0].desc.bInterfaceNumber,
				buf, size, 1000);
	} while ((retval == -ETIMEDOUT || retval == -EPIPE) && --retries);

	return retval;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void wacom_sys_irq(struct urb *urb)
{
	struct wacom *wacom = urb->context;
<<<<<<< HEAD
=======
	struct device *dev = &wacom->intf->dev;
>>>>>>> refs/remotes/origin/master
	int retval;

	switch (urb->status) {
	case 0:
		/* success */
		break;
	case -ECONNRESET:
	case -ENOENT:
	case -ESHUTDOWN:
		/* this urb is terminated, clean up */
<<<<<<< HEAD
		dbg("%s - urb shutting down with status: %d", __func__, urb->status);
		return;
	default:
		dbg("%s - nonzero urb status received: %d", __func__, urb->status);
=======
		dev_dbg(dev, "%s - urb shutting down with status: %d\n",
			__func__, urb->status);
		return;
	default:
		dev_dbg(dev, "%s - nonzero urb status received: %d\n",
			__func__, urb->status);
>>>>>>> refs/remotes/origin/master
		goto exit;
	}

	wacom_wac_irq(&wacom->wacom_wac, urb->actual_length);

 exit:
	usb_mark_last_busy(wacom->usbdev);
	retval = usb_submit_urb(urb, GFP_ATOMIC);
	if (retval)
<<<<<<< HEAD
		err ("%s - usb_submit_urb failed with result %d",
		     __func__, retval);
=======
		dev_err(dev, "%s - usb_submit_urb failed with result %d\n",
			__func__, retval);
>>>>>>> refs/remotes/origin/master
}

static int wacom_open(struct input_dev *dev)
{
	struct wacom *wacom = input_get_drvdata(dev);
	int retval = 0;

	if (usb_autopm_get_interface(wacom->intf) < 0)
		return -EIO;

	mutex_lock(&wacom->lock);

	if (usb_submit_urb(wacom->irq, GFP_KERNEL)) {
		retval = -EIO;
		goto out;
	}

	wacom->open = true;
	wacom->intf->needs_remote_wakeup = 1;

out:
	mutex_unlock(&wacom->lock);
	usb_autopm_put_interface(wacom->intf);
	return retval;
}

static void wacom_close(struct input_dev *dev)
{
	struct wacom *wacom = input_get_drvdata(dev);
	int autopm_error;

	autopm_error = usb_autopm_get_interface(wacom->intf);

	mutex_lock(&wacom->lock);
	usb_kill_urb(wacom->irq);
	wacom->open = false;
	wacom->intf->needs_remote_wakeup = 0;
	mutex_unlock(&wacom->lock);

	if (!autopm_error)
		usb_autopm_put_interface(wacom->intf);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int wacom_parse_hid(struct usb_interface *intf, struct hid_descriptor *hid_desc,
=======
/*
 * Static values for max X/Y and resolution of Pen interface is stored in
 * features. This mean physical size of active area can be computed.
 * This is useful to do when Pen and Touch have same active area of tablet.
 * This means for Touch device, we only need to find max X/Y value and we
 * have enough information to compute resolution of touch.
 */
static void wacom_set_phy_from_res(struct wacom_features *features)
{
	features->x_phy = (features->x_max * 100) / features->x_resolution;
	features->y_phy = (features->y_max * 100) / features->y_resolution;
=======
/*
 * Calculate the resolution of the X or Y axis, given appropriate HID data.
 * This function is little more than hidinput_calc_abs_res stripped down.
 */
static int wacom_calc_hid_res(int logical_extents, int physical_extents,
                              unsigned char unit, unsigned char exponent)
{
	int prev, unit_exponent;

	/* Check if the extents are sane */
	if (logical_extents <= 0 || physical_extents <= 0)
		return 0;

	/* Get signed value of nybble-sized twos-compliment exponent */
	unit_exponent = exponent;
	if (unit_exponent > 7)
		unit_exponent -= 16;

	/* Convert physical_extents to millimeters */
	if (unit == 0x11) {		/* If centimeters */
		unit_exponent += 1;
	} else if (unit == 0x13) {	/* If inches */
		prev = physical_extents;
		physical_extents *= 254;
		if (physical_extents < prev)
			return 0;
		unit_exponent -= 1;
	} else {
		return 0;
	}

	/* Apply negative unit exponent */
	for (; unit_exponent < 0; unit_exponent++) {
		prev = logical_extents;
		logical_extents *= 10;
		if (logical_extents < prev)
			return 0;
	}
	/* Apply positive unit exponent */
	for (; unit_exponent > 0; unit_exponent--) {
		prev = physical_extents;
		physical_extents *= 10;
		if (physical_extents < prev)
			return 0;
	}

	/* Calculate resolution */
	return logical_extents / physical_extents;
>>>>>>> refs/remotes/origin/master
}

static int wacom_parse_logical_collection(unsigned char *report,
					  struct wacom_features *features)
{
	int length = 0;

	if (features->type == BAMBOO_PT) {

		/* Logical collection is only used by 3rd gen Bamboo Touch */
		features->pktlen = WACOM_PKGLEN_BBTOUCH3;
		features->device_type = BTN_TOOL_FINGER;

<<<<<<< HEAD
		wacom_set_phy_from_res(features);

=======
>>>>>>> refs/remotes/origin/master
		features->x_max = features->y_max =
			get_unaligned_le16(&report[10]);

		length = 11;
	}
	return length;
}

<<<<<<< HEAD
=======
static void wacom_retrieve_report_data(struct usb_interface *intf,
				       struct wacom_features *features)
{
	int result = 0;
	unsigned char *rep_data;

	rep_data = kmalloc(2, GFP_KERNEL);
	if (rep_data) {

		rep_data[0] = 12;
		result = wacom_get_report(intf, WAC_HID_FEATURE_REPORT,
					  rep_data[0], rep_data, 2,
					  WAC_MSG_RETRIES);

		if (result >= 0 && rep_data[1] > 2)
			features->touch_max = rep_data[1];

		kfree(rep_data);
	}
}

>>>>>>> refs/remotes/origin/master
/*
 * Interface Descriptor of wacom devices can be incomplete and
 * inconsistent so wacom_features table is used to store stylus
 * device's packet lengths, various maximum values, and tablet
 * resolution based on product ID's.
 *
 * For devices that contain 2 interfaces, wacom_features table is
 * inaccurate for the touch interface.  Since the Interface Descriptor
 * for touch interfaces has pretty complete data, this function exists
 * to query tablet for this missing information instead of hard coding in
 * an additional table.
 *
 * A typical Interface Descriptor for a stylus will contain a
 * boot mouse application collection that is not of interest and this
 * function will ignore it.
 *
 * It also contains a digitizer application collection that also is not
 * of interest since any information it contains would be duplicate
 * of what is in wacom_features. Usually it defines a report of an array
 * of bytes that could be used as max length of the stylus packet returned.
 * If it happens to define a Digitizer-Stylus Physical Collection then
 * the X and Y logical values contain valid data but it is ignored.
 *
 * A typical Interface Descriptor for a touch interface will contain a
 * Digitizer-Finger Physical Collection which will define both logical
 * X/Y maximum as well as the physical size of tablet. Since touch
 * interfaces haven't supported pressure or distance, this is enough
 * information to override invalid values in the wacom_features table.
 *
 * 3rd gen Bamboo Touch no longer define a Digitizer-Finger Pysical
 * Collection. Instead they define a Logical Collection with a single
 * Logical Maximum for both X and Y.
<<<<<<< HEAD
 */
static int wacom_parse_hid(struct usb_interface *intf,
			   struct hid_descriptor *hid_desc,
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *
 * Intuos5 touch interface does not contain useful data. We deal with
 * this after returning from this function.
 */
static int wacom_parse_hid(struct usb_interface *intf,
			   struct hid_descriptor *hid_desc,
>>>>>>> refs/remotes/origin/master
			   struct wacom_features *features)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	char limit = 0;
	/* result has to be defined as int for some devices */
	int result = 0;
	int i = 0, usage = WCM_UNDEFINED, finger = 0, pen = 0;
	unsigned char *report;

	report = kzalloc(hid_desc->wDescriptorLength, GFP_KERNEL);
	if (!report)
		return -ENOMEM;

	/* retrive report descriptors */
	do {
		result = usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
			USB_REQ_GET_DESCRIPTOR,
			USB_RECIP_INTERFACE | USB_DIR_IN,
			HID_DEVICET_REPORT << 8,
			intf->altsetting[0].desc.bInterfaceNumber, /* interface */
			report,
			hid_desc->wDescriptorLength,
			5000); /* 5 secs */
<<<<<<< HEAD
<<<<<<< HEAD
	} while (result < 0 && limit++ < 5);
=======
	} while (result < 0 && limit++ < WAC_MSG_RETRIES);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	} while (result < 0 && limit++ < WAC_MSG_RETRIES);
>>>>>>> refs/remotes/origin/master

	/* No need to parse the Descriptor. It isn't an error though */
	if (result < 0)
		goto out;

	for (i = 0; i < hid_desc->wDescriptorLength; i++) {

		switch (report[i]) {
		case HID_USAGE_PAGE:
			switch (report[i + 1]) {
			case HID_USAGE_PAGE_DIGITIZER:
				usage = WCM_DIGITIZER;
				i++;
				break;

			case HID_USAGE_PAGE_DESKTOP:
				usage = WCM_DESKTOP;
				i++;
				break;
			}
			break;

		case HID_USAGE:
			switch (report[i + 1]) {
			case HID_USAGE_X:
				if (usage == WCM_DESKTOP) {
					if (finger) {
						features->device_type = BTN_TOOL_FINGER;
<<<<<<< HEAD
						if (features->type == TABLETPC2FG) {
							/* need to reset back */
							features->pktlen = WACOM_PKGLEN_TPC2FG;
<<<<<<< HEAD
							features->device_type = BTN_TOOL_DOUBLETAP;
=======
>>>>>>> refs/remotes/origin/cm-10.0
						}
						if (features->type == BAMBOO_PT) {
							/* need to reset back */
							features->pktlen = WACOM_PKGLEN_BBTOUCH;
<<<<<<< HEAD
							features->device_type = BTN_TOOL_DOUBLETAP;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======

						switch (features->type) {
						case TABLETPC2FG:
							features->pktlen = WACOM_PKGLEN_TPC2FG;
							break;

						case MTSCREEN:
						case WACOM_24HDT:
							features->pktlen = WACOM_PKGLEN_MTOUCH;
							break;

						case MTTPC:
							features->pktlen = WACOM_PKGLEN_MTTPC;
							break;

						case BAMBOO_PT:
							features->pktlen = WACOM_PKGLEN_BBTOUCH;
							break;

						default:
							features->pktlen = WACOM_PKGLEN_GRAPHIRE;
							break;
						}

						switch (features->type) {
						case BAMBOO_PT:
>>>>>>> refs/remotes/origin/master
							features->x_phy =
								get_unaligned_le16(&report[i + 5]);
							features->x_max =
								get_unaligned_le16(&report[i + 8]);
							i += 15;
<<<<<<< HEAD
						} else {
=======
							break;

						case WACOM_24HDT:
							features->x_max =
								get_unaligned_le16(&report[i + 3]);
							features->x_phy =
								get_unaligned_le16(&report[i + 8]);
							features->unit = report[i - 1];
							features->unitExpo = report[i - 3];
							i += 12;
							break;

						default:
>>>>>>> refs/remotes/origin/master
							features->x_max =
								get_unaligned_le16(&report[i + 3]);
							features->x_phy =
								get_unaligned_le16(&report[i + 6]);
							features->unit = report[i + 9];
							features->unitExpo = report[i + 11];
							i += 12;
<<<<<<< HEAD
						}
					} else if (pen) {
						/* penabled only accepts exact bytes of data */
						if (features->type == TABLETPC2FG)
							features->pktlen = WACOM_PKGLEN_GRAPHIRE;
<<<<<<< HEAD
						if (features->type == BAMBOO_PT)
							features->pktlen = WACOM_PKGLEN_BBFUN;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
							break;
						}
					} else if (pen) {
						/* penabled only accepts exact bytes of data */
						if (features->type >= TABLETPC)
							features->pktlen = WACOM_PKGLEN_GRAPHIRE;
>>>>>>> refs/remotes/origin/master
						features->device_type = BTN_TOOL_PEN;
						features->x_max =
							get_unaligned_le16(&report[i + 3]);
						i += 4;
					}
<<<<<<< HEAD
<<<<<<< HEAD
				} else if (usage == WCM_DIGITIZER) {
					/* max pressure isn't reported
					features->pressure_max = (unsigned short)
							(report[i+4] << 8  | report[i + 3]);
					*/
					features->pressure_max = 255;
					i += 4;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				}
				break;

			case HID_USAGE_Y:
				if (usage == WCM_DESKTOP) {
					if (finger) {
<<<<<<< HEAD
						features->device_type = BTN_TOOL_FINGER;
						if (features->type == TABLETPC2FG) {
							/* need to reset back */
							features->pktlen = WACOM_PKGLEN_TPC2FG;
<<<<<<< HEAD
							features->device_type = BTN_TOOL_DOUBLETAP;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
						switch (features->type) {
						case TABLETPC2FG:
						case MTSCREEN:
						case MTTPC:
>>>>>>> refs/remotes/origin/master
							features->y_max =
								get_unaligned_le16(&report[i + 3]);
							features->y_phy =
								get_unaligned_le16(&report[i + 6]);
							i += 7;
<<<<<<< HEAD
						} else if (features->type == BAMBOO_PT) {
							/* need to reset back */
							features->pktlen = WACOM_PKGLEN_BBTOUCH;
<<<<<<< HEAD
							features->device_type = BTN_TOOL_DOUBLETAP;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
							break;

						case WACOM_24HDT:
							features->y_max =
								get_unaligned_le16(&report[i + 3]);
							features->y_phy =
								get_unaligned_le16(&report[i - 2]);
							i += 7;
							break;

						case BAMBOO_PT:
>>>>>>> refs/remotes/origin/master
							features->y_phy =
								get_unaligned_le16(&report[i + 3]);
							features->y_max =
								get_unaligned_le16(&report[i + 6]);
							i += 12;
<<<<<<< HEAD
						} else {
=======
							break;

						default:
>>>>>>> refs/remotes/origin/master
							features->y_max =
								features->x_max;
							features->y_phy =
								get_unaligned_le16(&report[i + 3]);
							i += 4;
<<<<<<< HEAD
						}
					} else if (pen) {
						/* penabled only accepts exact bytes of data */
						if (features->type == TABLETPC2FG)
							features->pktlen = WACOM_PKGLEN_GRAPHIRE;
<<<<<<< HEAD
						if (features->type == BAMBOO_PT)
							features->pktlen = WACOM_PKGLEN_BBFUN;
=======
>>>>>>> refs/remotes/origin/cm-10.0
						features->device_type = BTN_TOOL_PEN;
=======
							break;
						}
					} else if (pen) {
>>>>>>> refs/remotes/origin/master
						features->y_max =
							get_unaligned_le16(&report[i + 3]);
						i += 4;
					}
				}
				break;

			case HID_USAGE_FINGER:
				finger = 1;
				i++;
				break;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
			/*
			 * Requiring Stylus Usage will ignore boot mouse
			 * X/Y values and some cases of invalid Digitizer X/Y
			 * values commonly reported.
			 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			case HID_USAGE_STYLUS:
				pen = 1;
				i++;
				break;
<<<<<<< HEAD
<<<<<<< HEAD

			case HID_USAGE_UNDEFINED:
				if (usage == WCM_DESKTOP && finger) /* capacity */
					features->pressure_max =
						get_unaligned_le16(&report[i + 3]);
				i += 4;
=======

			case HID_USAGE_CONTACTMAX:
				/* leave touch_max as is if predefined */
				if (!features->touch_max)
					wacom_retrieve_report_data(intf, features);
				i++;
>>>>>>> refs/remotes/origin/master
				break;
			}
			break;

<<<<<<< HEAD
		case HID_COLLECTION:
			/* reset UsagePage and Finger */
			finger = usage = 0;
			break;
=======
			}
			break;

=======
>>>>>>> refs/remotes/origin/master
		case HID_COLLECTION_END:
			/* reset UsagePage and Finger */
			finger = usage = 0;
			break;

		case HID_COLLECTION:
			i++;
			switch (report[i]) {
			case HID_COLLECTION_LOGICAL:
				i += wacom_parse_logical_collection(&report[i],
								    features);
				break;
			}
			break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
	}

 out:
	result = 0;
	kfree(report);
	return result;
}

<<<<<<< HEAD
static int wacom_query_tablet_data(struct usb_interface *intf, struct wacom_features *features)
{
	unsigned char *rep_data;
	int limit = 0, report_id = 2;
	int error = -ENOMEM;

<<<<<<< HEAD
	rep_data = kmalloc(2, GFP_KERNEL);
	if (!rep_data)
		return error;

	/* ask to report tablet data if it is 2FGT Tablet PC or
=======
	rep_data = kmalloc(4, GFP_KERNEL);
	if (!rep_data)
		return error;

	/* ask to report tablet data if it is MT Tablet PC or
>>>>>>> refs/remotes/origin/cm-10.0
	 * not a Tablet PC */
	if (features->type == TABLETPC2FG) {
		do {
			rep_data[0] = 3;
			rep_data[1] = 4;
<<<<<<< HEAD
			report_id = 3;
			error = usb_set_report(intf, WAC_HID_FEATURE_REPORT,
				report_id, rep_data, 2);
			if (error >= 0)
				error = usb_get_report(intf,
					WAC_HID_FEATURE_REPORT, report_id,
					rep_data, 3);
		} while ((error < 0 || rep_data[1] != 4) && limit++ < 5);
	} else if (features->type != TABLETPC) {
		do {
			rep_data[0] = 2;
			rep_data[1] = 2;
			error = usb_set_report(intf, WAC_HID_FEATURE_REPORT,
				report_id, rep_data, 2);
			if (error >= 0)
				error = usb_get_report(intf,
					WAC_HID_FEATURE_REPORT, report_id,
					rep_data, 2);
		} while ((error < 0 || rep_data[1] != 2) && limit++ < 5);
=======
			rep_data[2] = 0;
			rep_data[3] = 0;
			report_id = 3;
			error = wacom_set_report(intf, WAC_HID_FEATURE_REPORT,
						 report_id, rep_data, 4, 1);
			if (error >= 0)
				error = wacom_get_report(intf,
						WAC_HID_FEATURE_REPORT,
						report_id, rep_data, 4, 1);
		} while ((error < 0 || rep_data[1] != 4) && limit++ < WAC_MSG_RETRIES);
	} else if (features->type != TABLETPC &&
		   features->type != WIRELESS &&
		   features->device_type == BTN_TOOL_PEN) {
		do {
			rep_data[0] = 2;
			rep_data[1] = 2;
			error = wacom_set_report(intf, WAC_HID_FEATURE_REPORT,
						 report_id, rep_data, 2, 1);
			if (error >= 0)
				error = wacom_get_report(intf,
						WAC_HID_FEATURE_REPORT,
						report_id, rep_data, 2, 1);
		} while ((error < 0 || rep_data[1] != 2) && limit++ < WAC_MSG_RETRIES);
>>>>>>> refs/remotes/origin/cm-10.0
	}
=======
static int wacom_set_device_mode(struct usb_interface *intf, int report_id, int length, int mode)
{
	unsigned char *rep_data;
	int error = -ENOMEM, limit = 0;

	rep_data = kzalloc(length, GFP_KERNEL);
	if (!rep_data)
		return error;

	do {
		rep_data[0] = report_id;
		rep_data[1] = mode;

		error = wacom_set_report(intf, WAC_HID_FEATURE_REPORT,
		                         report_id, rep_data, length, 1);
	} while ((error < 0 || rep_data[1] != mode) && limit++ < WAC_MSG_RETRIES);
>>>>>>> refs/remotes/origin/master

	kfree(rep_data);

	return error < 0 ? error : 0;
}

<<<<<<< HEAD
static int wacom_retrieve_hid_descriptor(struct usb_interface *intf,
		struct wacom_features *features)
=======
/*
 * Switch the tablet into its most-capable mode. Wacom tablets are
 * typically configured to power-up in a mode which sends mouse-like
 * reports to the OS. To get absolute position, pressure data, etc.
 * from the tablet, it is necessary to switch the tablet out of this
 * mode and into one which sends the full range of tablet data.
 */
static int wacom_query_tablet_data(struct usb_interface *intf, struct wacom_features *features)
{
	if (features->device_type == BTN_TOOL_FINGER) {
		if (features->type > TABLETPC) {
			/* MT Tablet PC touch */
			return wacom_set_device_mode(intf, 3, 4, 4);
		}
		else if (features->type == WACOM_24HDT || features->type == CINTIQ_HYBRID) {
			return wacom_set_device_mode(intf, 18, 3, 2);
		}
	} else if (features->device_type == BTN_TOOL_PEN) {
		if (features->type <= BAMBOO_PT && features->type != WIRELESS) {
			return wacom_set_device_mode(intf, 2, 2, 2);
		}
	}

	return 0;
}

static int wacom_retrieve_hid_descriptor(struct usb_interface *intf,
					 struct wacom_features *features)
>>>>>>> refs/remotes/origin/master
{
	int error = 0;
	struct usb_host_interface *interface = intf->cur_altsetting;
	struct hid_descriptor *hid_desc;

	/* default features */
	features->device_type = BTN_TOOL_PEN;
	features->x_fuzz = 4;
	features->y_fuzz = 4;
	features->pressure_fuzz = 0;
	features->distance_fuzz = 0;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * The wireless device HID is basic and layout conflicts with
	 * other tablets (monitor and touch interface can look like pen).
	 * Skip the query for this type and modify defaults based on
	 * interface number.
	 */
	if (features->type == WIRELESS) {
		if (intf->cur_altsetting->desc.bInterfaceNumber == 0) {
			features->device_type = 0;
		} else if (intf->cur_altsetting->desc.bInterfaceNumber == 2) {
<<<<<<< HEAD
			features->device_type = BTN_TOOL_DOUBLETAP;
=======
			features->device_type = BTN_TOOL_FINGER;
>>>>>>> refs/remotes/origin/master
			features->pktlen = WACOM_PKGLEN_BBTOUCH3;
		}
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	/* only Tablet PCs and Bamboo P&T need to retrieve the info */
	if ((features->type != TABLETPC) && (features->type != TABLETPC2FG) &&
	    (features->type != BAMBOO_PT))
		goto out;

	if (usb_get_extra_descriptor(interface, HID_DEVICET_HID, &hid_desc)) {
		if (usb_get_extra_descriptor(&interface->endpoint[0],
				HID_DEVICET_REPORT, &hid_desc)) {
			printk("wacom: can not retrieve extra class descriptor\n");
			error = 1;
=======
	/* only devices that support touch need to retrieve the info */
	if (features->type < BAMBOO_PT) {
		goto out;
	}

	error = usb_get_extra_descriptor(interface, HID_DEVICET_HID, &hid_desc);
	if (error) {
		error = usb_get_extra_descriptor(&interface->endpoint[0],
						 HID_DEVICET_REPORT, &hid_desc);
		if (error) {
			dev_err(&intf->dev,
				"can not retrieve extra class descriptor\n");
>>>>>>> refs/remotes/origin/master
			goto out;
		}
	}
	error = wacom_parse_hid(intf, hid_desc, features);
<<<<<<< HEAD
	if (error)
		goto out;
=======
>>>>>>> refs/remotes/origin/master

 out:
	return error;
}

struct wacom_usbdev_data {
	struct list_head list;
	struct kref kref;
	struct usb_device *dev;
	struct wacom_shared shared;
};

static LIST_HEAD(wacom_udev_list);
static DEFINE_MUTEX(wacom_udev_list_lock);

<<<<<<< HEAD
=======
static struct usb_device *wacom_get_sibling(struct usb_device *dev, int vendor, int product)
{
	int port1;
	struct usb_device *sibling;

	if (vendor == 0 && product == 0)
		return dev;

	if (dev->parent == NULL)
		return NULL;

	usb_hub_for_each_child(dev->parent, port1, sibling) {
		struct usb_device_descriptor *d;
		if (sibling == NULL)
			continue;

		d = &sibling->descriptor;
		if (d->idVendor == vendor && d->idProduct == product)
			return sibling;
	}

	return NULL;
}

>>>>>>> refs/remotes/origin/master
static struct wacom_usbdev_data *wacom_get_usbdev_data(struct usb_device *dev)
{
	struct wacom_usbdev_data *data;

	list_for_each_entry(data, &wacom_udev_list, list) {
		if (data->dev == dev) {
			kref_get(&data->kref);
			return data;
		}
	}

	return NULL;
}

static int wacom_add_shared_data(struct wacom_wac *wacom,
				 struct usb_device *dev)
{
	struct wacom_usbdev_data *data;
	int retval = 0;

	mutex_lock(&wacom_udev_list_lock);

	data = wacom_get_usbdev_data(dev);
	if (!data) {
		data = kzalloc(sizeof(struct wacom_usbdev_data), GFP_KERNEL);
		if (!data) {
			retval = -ENOMEM;
			goto out;
		}

		kref_init(&data->kref);
		data->dev = dev;
		list_add_tail(&data->list, &wacom_udev_list);
	}

	wacom->shared = &data->shared;

out:
	mutex_unlock(&wacom_udev_list_lock);
	return retval;
}

static void wacom_release_shared_data(struct kref *kref)
{
	struct wacom_usbdev_data *data =
		container_of(kref, struct wacom_usbdev_data, kref);

	mutex_lock(&wacom_udev_list_lock);
	list_del(&data->list);
	mutex_unlock(&wacom_udev_list_lock);

	kfree(data);
}

static void wacom_remove_shared_data(struct wacom_wac *wacom)
{
	struct wacom_usbdev_data *data;

	if (wacom->shared) {
		data = container_of(wacom->shared, struct wacom_usbdev_data, shared);
		kref_put(&data->kref, wacom_release_shared_data);
		wacom->shared = NULL;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static int wacom_led_control(struct wacom *wacom)
{
	unsigned char *buf;
	int retval, led = 0;
=======
static int wacom_led_control(struct wacom *wacom)
{
	unsigned char *buf;
	int retval;
>>>>>>> refs/remotes/origin/master

	buf = kzalloc(9, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

<<<<<<< HEAD
	if (wacom->wacom_wac.features.type == WACOM_21UX2 ||
	    wacom->wacom_wac.features.type == WACOM_24HD)
		led = (wacom->led.select[1] << 4) | 0x40;

	led |=  wacom->led.select[0] | 0x4;

	buf[0] = WAC_CMD_LED_CONTROL;
	buf[1] = led;
	buf[2] = wacom->led.llv;
	buf[3] = wacom->led.hlv;
	buf[4] = wacom->led.img_lum;
=======
	if (wacom->wacom_wac.features.type >= INTUOS5S &&
	    wacom->wacom_wac.features.type <= INTUOSPL) {
		/*
		 * Touch Ring and crop mark LED luminance may take on
		 * one of four values:
		 *    0 = Low; 1 = Medium; 2 = High; 3 = Off
		 */
		int ring_led = wacom->led.select[0] & 0x03;
		int ring_lum = (((wacom->led.llv & 0x60) >> 5) - 1) & 0x03;
		int crop_lum = 0;

		buf[0] = WAC_CMD_LED_CONTROL;
		buf[1] = (crop_lum << 4) | (ring_lum << 2) | (ring_led);
	}
	else {
		int led = wacom->led.select[0] | 0x4;

		if (wacom->wacom_wac.features.type == WACOM_21UX2 ||
		    wacom->wacom_wac.features.type == WACOM_24HD)
			led |= (wacom->led.select[1] << 4) | 0x40;

		buf[0] = WAC_CMD_LED_CONTROL;
		buf[1] = led;
		buf[2] = wacom->led.llv;
		buf[3] = wacom->led.hlv;
		buf[4] = wacom->led.img_lum;
	}
>>>>>>> refs/remotes/origin/master

	retval = wacom_set_report(wacom->intf, 0x03, WAC_CMD_LED_CONTROL,
				  buf, 9, WAC_CMD_RETRIES);
	kfree(buf);

	return retval;
}

static int wacom_led_putimage(struct wacom *wacom, int button_id, const void *img)
{
	unsigned char *buf;
	int i, retval;

	buf = kzalloc(259, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	/* Send 'start' command */
	buf[0] = WAC_CMD_ICON_START;
	buf[1] = 1;
	retval = wacom_set_report(wacom->intf, 0x03, WAC_CMD_ICON_START,
				  buf, 2, WAC_CMD_RETRIES);
	if (retval < 0)
		goto out;

	buf[0] = WAC_CMD_ICON_XFER;
	buf[1] = button_id & 0x07;
	for (i = 0; i < 4; i++) {
		buf[2] = i;
		memcpy(buf + 3, img + i * 256, 256);

		retval = wacom_set_report(wacom->intf, 0x03, WAC_CMD_ICON_XFER,
					  buf, 259, WAC_CMD_RETRIES);
		if (retval < 0)
			break;
	}

	/* Send 'stop' */
	buf[0] = WAC_CMD_ICON_START;
	buf[1] = 0;
	wacom_set_report(wacom->intf, 0x03, WAC_CMD_ICON_START,
			 buf, 2, WAC_CMD_RETRIES);

out:
	kfree(buf);
	return retval;
}

static ssize_t wacom_led_select_store(struct device *dev, int set_id,
				      const char *buf, size_t count)
{
	struct wacom *wacom = dev_get_drvdata(dev);
	unsigned int id;
	int err;

	err = kstrtouint(buf, 10, &id);
	if (err)
		return err;

	mutex_lock(&wacom->lock);

	wacom->led.select[set_id] = id & 0x3;
	err = wacom_led_control(wacom);

	mutex_unlock(&wacom->lock);

	return err < 0 ? err : count;
}

#define DEVICE_LED_SELECT_ATTR(SET_ID)					\
static ssize_t wacom_led##SET_ID##_select_store(struct device *dev,	\
	struct device_attribute *attr, const char *buf, size_t count)	\
{									\
	return wacom_led_select_store(dev, SET_ID, buf, count);		\
}									\
static ssize_t wacom_led##SET_ID##_select_show(struct device *dev,	\
	struct device_attribute *attr, char *buf)			\
{									\
	struct wacom *wacom = dev_get_drvdata(dev);			\
	return snprintf(buf, 2, "%d\n", wacom->led.select[SET_ID]);	\
}									\
static DEVICE_ATTR(status_led##SET_ID##_select, S_IWUSR | S_IRUSR,	\
		    wacom_led##SET_ID##_select_show,			\
		    wacom_led##SET_ID##_select_store)

DEVICE_LED_SELECT_ATTR(0);
DEVICE_LED_SELECT_ATTR(1);

static ssize_t wacom_luminance_store(struct wacom *wacom, u8 *dest,
				     const char *buf, size_t count)
{
	unsigned int value;
	int err;

	err = kstrtouint(buf, 10, &value);
	if (err)
		return err;

	mutex_lock(&wacom->lock);

	*dest = value & 0x7f;
	err = wacom_led_control(wacom);

	mutex_unlock(&wacom->lock);

	return err < 0 ? err : count;
}

#define DEVICE_LUMINANCE_ATTR(name, field)				\
static ssize_t wacom_##name##_luminance_store(struct device *dev,	\
	struct device_attribute *attr, const char *buf, size_t count)	\
{									\
	struct wacom *wacom = dev_get_drvdata(dev);			\
									\
	return wacom_luminance_store(wacom, &wacom->led.field,		\
				     buf, count);			\
}									\
static DEVICE_ATTR(name##_luminance, S_IWUSR,				\
		   NULL, wacom_##name##_luminance_store)

DEVICE_LUMINANCE_ATTR(status0, llv);
DEVICE_LUMINANCE_ATTR(status1, hlv);
DEVICE_LUMINANCE_ATTR(buttons, img_lum);

static ssize_t wacom_button_image_store(struct device *dev, int button_id,
					const char *buf, size_t count)
{
	struct wacom *wacom = dev_get_drvdata(dev);
	int err;

	if (count != 1024)
		return -EINVAL;

	mutex_lock(&wacom->lock);

	err = wacom_led_putimage(wacom, button_id, buf);

	mutex_unlock(&wacom->lock);

	return err < 0 ? err : count;
}

#define DEVICE_BTNIMG_ATTR(BUTTON_ID)					\
static ssize_t wacom_btnimg##BUTTON_ID##_store(struct device *dev,	\
	struct device_attribute *attr, const char *buf, size_t count)	\
{									\
	return wacom_button_image_store(dev, BUTTON_ID, buf, count);	\
}									\
static DEVICE_ATTR(button##BUTTON_ID##_rawimg, S_IWUSR,			\
		   NULL, wacom_btnimg##BUTTON_ID##_store)

DEVICE_BTNIMG_ATTR(0);
DEVICE_BTNIMG_ATTR(1);
DEVICE_BTNIMG_ATTR(2);
DEVICE_BTNIMG_ATTR(3);
DEVICE_BTNIMG_ATTR(4);
DEVICE_BTNIMG_ATTR(5);
DEVICE_BTNIMG_ATTR(6);
DEVICE_BTNIMG_ATTR(7);

static struct attribute *cintiq_led_attrs[] = {
	&dev_attr_status_led0_select.attr,
	&dev_attr_status_led1_select.attr,
	NULL
};

static struct attribute_group cintiq_led_attr_group = {
	.name = "wacom_led",
	.attrs = cintiq_led_attrs,
};

static struct attribute *intuos4_led_attrs[] = {
	&dev_attr_status0_luminance.attr,
	&dev_attr_status1_luminance.attr,
	&dev_attr_status_led0_select.attr,
	&dev_attr_buttons_luminance.attr,
	&dev_attr_button0_rawimg.attr,
	&dev_attr_button1_rawimg.attr,
	&dev_attr_button2_rawimg.attr,
	&dev_attr_button3_rawimg.attr,
	&dev_attr_button4_rawimg.attr,
	&dev_attr_button5_rawimg.attr,
	&dev_attr_button6_rawimg.attr,
	&dev_attr_button7_rawimg.attr,
	NULL
};

static struct attribute_group intuos4_led_attr_group = {
	.name = "wacom_led",
	.attrs = intuos4_led_attrs,
};

<<<<<<< HEAD
=======
static struct attribute *intuos5_led_attrs[] = {
	&dev_attr_status0_luminance.attr,
	&dev_attr_status_led0_select.attr,
	NULL
};

static struct attribute_group intuos5_led_attr_group = {
	.name = "wacom_led",
	.attrs = intuos5_led_attrs,
};

>>>>>>> refs/remotes/origin/master
static int wacom_initialize_leds(struct wacom *wacom)
{
	int error;

	/* Initialize default values */
	switch (wacom->wacom_wac.features.type) {
<<<<<<< HEAD
=======
	case INTUOS4S:
>>>>>>> refs/remotes/origin/master
	case INTUOS4:
	case INTUOS4L:
		wacom->led.select[0] = 0;
		wacom->led.select[1] = 0;
		wacom->led.llv = 10;
		wacom->led.hlv = 20;
		wacom->led.img_lum = 10;
		error = sysfs_create_group(&wacom->intf->dev.kobj,
					   &intuos4_led_attr_group);
		break;

	case WACOM_24HD:
	case WACOM_21UX2:
		wacom->led.select[0] = 0;
		wacom->led.select[1] = 0;
		wacom->led.llv = 0;
		wacom->led.hlv = 0;
		wacom->led.img_lum = 0;

		error = sysfs_create_group(&wacom->intf->dev.kobj,
					   &cintiq_led_attr_group);
		break;

<<<<<<< HEAD
=======
	case INTUOS5S:
	case INTUOS5:
	case INTUOS5L:
	case INTUOSPS:
	case INTUOSPM:
	case INTUOSPL:
		if (wacom->wacom_wac.features.device_type == BTN_TOOL_PEN) {
			wacom->led.select[0] = 0;
			wacom->led.select[1] = 0;
			wacom->led.llv = 32;
			wacom->led.hlv = 0;
			wacom->led.img_lum = 0;

			error = sysfs_create_group(&wacom->intf->dev.kobj,
						  &intuos5_led_attr_group);
		} else
			return 0;
		break;

>>>>>>> refs/remotes/origin/master
	default:
		return 0;
	}

	if (error) {
		dev_err(&wacom->intf->dev,
			"cannot create sysfs group err: %d\n", error);
		return error;
	}
	wacom_led_control(wacom);

	return 0;
}

static void wacom_destroy_leds(struct wacom *wacom)
{
	switch (wacom->wacom_wac.features.type) {
<<<<<<< HEAD
=======
	case INTUOS4S:
>>>>>>> refs/remotes/origin/master
	case INTUOS4:
	case INTUOS4L:
		sysfs_remove_group(&wacom->intf->dev.kobj,
				   &intuos4_led_attr_group);
		break;

	case WACOM_24HD:
	case WACOM_21UX2:
		sysfs_remove_group(&wacom->intf->dev.kobj,
				   &cintiq_led_attr_group);
		break;
<<<<<<< HEAD
=======

	case INTUOS5S:
	case INTUOS5:
	case INTUOS5L:
	case INTUOSPS:
	case INTUOSPM:
	case INTUOSPL:
		if (wacom->wacom_wac.features.device_type == BTN_TOOL_PEN)
			sysfs_remove_group(&wacom->intf->dev.kobj,
					   &intuos5_led_attr_group);
		break;
>>>>>>> refs/remotes/origin/master
	}
}

static enum power_supply_property wacom_battery_props[] = {
<<<<<<< HEAD
=======
	POWER_SUPPLY_PROP_SCOPE,
>>>>>>> refs/remotes/origin/master
	POWER_SUPPLY_PROP_CAPACITY
};

static int wacom_battery_get_property(struct power_supply *psy,
				      enum power_supply_property psp,
				      union power_supply_propval *val)
{
	struct wacom *wacom = container_of(psy, struct wacom, battery);
	int ret = 0;

	switch (psp) {
<<<<<<< HEAD
=======
		case POWER_SUPPLY_PROP_SCOPE:
			val->intval = POWER_SUPPLY_SCOPE_DEVICE;
			break;
>>>>>>> refs/remotes/origin/master
		case POWER_SUPPLY_PROP_CAPACITY:
			val->intval =
				wacom->wacom_wac.battery_capacity * 100 / 31;
			break;
		default:
			ret = -EINVAL;
			break;
	}

	return ret;
}

static int wacom_initialize_battery(struct wacom *wacom)
{
	int error = 0;

	if (wacom->wacom_wac.features.quirks & WACOM_QUIRK_MONITOR) {
		wacom->battery.properties = wacom_battery_props;
		wacom->battery.num_properties = ARRAY_SIZE(wacom_battery_props);
		wacom->battery.get_property = wacom_battery_get_property;
		wacom->battery.name = "wacom_battery";
		wacom->battery.type = POWER_SUPPLY_TYPE_BATTERY;
		wacom->battery.use_for_apm = 0;

		error = power_supply_register(&wacom->usbdev->dev,
					      &wacom->battery);
<<<<<<< HEAD
=======

		if (!error)
			power_supply_powers(&wacom->battery,
					    &wacom->usbdev->dev);
>>>>>>> refs/remotes/origin/master
	}

	return error;
}

static void wacom_destroy_battery(struct wacom *wacom)
{
<<<<<<< HEAD
	if (wacom->wacom_wac.features.quirks & WACOM_QUIRK_MONITOR)
		power_supply_unregister(&wacom->battery);
=======
	if (wacom->wacom_wac.features.quirks & WACOM_QUIRK_MONITOR &&
	    wacom->battery.dev) {
		power_supply_unregister(&wacom->battery);
		wacom->battery.dev = NULL;
	}
>>>>>>> refs/remotes/origin/master
}

static int wacom_register_input(struct wacom *wacom)
{
	struct input_dev *input_dev;
	struct usb_interface *intf = wacom->intf;
	struct usb_device *dev = interface_to_usbdev(intf);
	struct wacom_wac *wacom_wac = &(wacom->wacom_wac);
	int error;

	input_dev = input_allocate_device();
<<<<<<< HEAD
	if (!input_dev)
		return -ENOMEM;
=======
	if (!input_dev) {
		error = -ENOMEM;
		goto fail1;
	}
>>>>>>> refs/remotes/origin/master

	input_dev->name = wacom_wac->name;
	input_dev->dev.parent = &intf->dev;
	input_dev->open = wacom_open;
	input_dev->close = wacom_close;
	usb_to_input_id(dev, &input_dev->id);
	input_set_drvdata(input_dev, wacom);

	wacom_wac->input = input_dev;
<<<<<<< HEAD
	wacom_setup_input_capabilities(input_dev, wacom_wac);

	error = input_register_device(input_dev);
	if (error) {
		input_free_device(input_dev);
		wacom_wac->input = NULL;
	}

=======
	error = wacom_setup_input_capabilities(input_dev, wacom_wac);
	if (error)
		goto fail1;

	error = input_register_device(input_dev);
	if (error)
		goto fail2;

	return 0;

fail2:
	input_free_device(input_dev);
	wacom_wac->input = NULL;
fail1:
>>>>>>> refs/remotes/origin/master
	return error;
}

static void wacom_wireless_work(struct work_struct *work)
{
	struct wacom *wacom = container_of(work, struct wacom, work);
	struct usb_device *usbdev = wacom->usbdev;
	struct wacom_wac *wacom_wac = &wacom->wacom_wac;
<<<<<<< HEAD

	/*
	 * Regardless if this is a disconnect or a new tablet,
	 * remove any existing input devices.
	 */

	/* Stylus interface */
	wacom = usb_get_intfdata(usbdev->config->interface[1]);
	if (wacom->wacom_wac.input)
		input_unregister_device(wacom->wacom_wac.input);
	wacom->wacom_wac.input = 0;

	/* Touch interface */
	wacom = usb_get_intfdata(usbdev->config->interface[2]);
	if (wacom->wacom_wac.input)
		input_unregister_device(wacom->wacom_wac.input);
	wacom->wacom_wac.input = 0;

	if (wacom_wac->pid == 0) {
		printk(KERN_INFO "wacom: wireless tablet disconnected\n");
	} else {
		const struct usb_device_id *id = wacom_ids;

		printk(KERN_INFO
		       "wacom: wireless tablet connected with PID %x\n",
		       wacom_wac->pid);
=======
	struct wacom *wacom1, *wacom2;
	struct wacom_wac *wacom_wac1, *wacom_wac2;
	int error;

	/*
	 * Regardless if this is a disconnect or a new tablet,
	 * remove any existing input and battery devices.
	 */

	wacom_destroy_battery(wacom);

	/* Stylus interface */
	wacom1 = usb_get_intfdata(usbdev->config->interface[1]);
	wacom_wac1 = &(wacom1->wacom_wac);
	if (wacom_wac1->input)
		input_unregister_device(wacom_wac1->input);
	wacom_wac1->input = NULL;

	/* Touch interface */
	wacom2 = usb_get_intfdata(usbdev->config->interface[2]);
	wacom_wac2 = &(wacom2->wacom_wac);
	if (wacom_wac2->input)
		input_unregister_device(wacom_wac2->input);
	wacom_wac2->input = NULL;

	if (wacom_wac->pid == 0) {
		dev_info(&wacom->intf->dev, "wireless tablet disconnected\n");
	} else {
		const struct usb_device_id *id = wacom_ids;

		dev_info(&wacom->intf->dev,
			 "wireless tablet connected with PID %x\n",
			 wacom_wac->pid);
>>>>>>> refs/remotes/origin/master

		while (id->match_flags) {
			if (id->idVendor == USB_VENDOR_ID_WACOM &&
			    id->idProduct == wacom_wac->pid)
				break;
			id++;
		}

		if (!id->match_flags) {
<<<<<<< HEAD
			printk(KERN_INFO
			       "wacom: ignorning unknown PID.\n");
=======
			dev_info(&wacom->intf->dev,
				 "ignoring unknown PID.\n");
>>>>>>> refs/remotes/origin/master
			return;
		}

		/* Stylus interface */
<<<<<<< HEAD
		wacom = usb_get_intfdata(usbdev->config->interface[1]);
		wacom_wac = &wacom->wacom_wac;
		wacom_wac->features =
			*((struct wacom_features *)id->driver_info);
		wacom_wac->features.device_type = BTN_TOOL_PEN;
		wacom_register_input(wacom);

		/* Touch interface */
		wacom = usb_get_intfdata(usbdev->config->interface[2]);
		wacom_wac = &wacom->wacom_wac;
		wacom_wac->features =
			*((struct wacom_features *)id->driver_info);
		wacom_wac->features.pktlen = WACOM_PKGLEN_BBTOUCH3;
		wacom_wac->features.device_type = BTN_TOOL_FINGER;
		wacom_set_phy_from_res(&wacom_wac->features);
		wacom_wac->features.x_max = wacom_wac->features.y_max = 4096;
		wacom_register_input(wacom);
	}
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
		wacom_wac1->features =
			*((struct wacom_features *)id->driver_info);
		wacom_wac1->features.device_type = BTN_TOOL_PEN;
		snprintf(wacom_wac1->name, WACOM_NAME_MAX, "%s (WL) Pen",
			 wacom_wac1->features.name);
		error = wacom_register_input(wacom1);
		if (error)
			goto fail;

		/* Touch interface */
		if (wacom_wac1->features.touch_max) {
			wacom_wac2->features =
				*((struct wacom_features *)id->driver_info);
			wacom_wac2->features.pktlen = WACOM_PKGLEN_BBTOUCH3;
			wacom_wac2->features.device_type = BTN_TOOL_FINGER;
			wacom_wac2->features.x_max = wacom_wac2->features.y_max = 4096;
			if (wacom_wac2->features.touch_max)
				snprintf(wacom_wac2->name, WACOM_NAME_MAX,
					 "%s (WL) Finger",wacom_wac2->features.name);
			else
				snprintf(wacom_wac2->name, WACOM_NAME_MAX,
					 "%s (WL) Pad",wacom_wac2->features.name);
			error = wacom_register_input(wacom2);
			if (error)
				goto fail;
		}

		error = wacom_initialize_battery(wacom);
		if (error)
			goto fail;
	}

	return;

fail:
	if (wacom_wac2->input) {
		input_unregister_device(wacom_wac2->input);
		wacom_wac2->input = NULL;
	}

	if (wacom_wac1->input) {
		input_unregister_device(wacom_wac1->input);
		wacom_wac1->input = NULL;
	}
	return;
}

/*
 * Not all devices report physical dimensions from HID.
 * Compute the default from hardcoded logical dimension
 * and resolution before driver overwrites them.
 */
static void wacom_set_default_phy(struct wacom_features *features)
{
	if (features->x_resolution) {
		features->x_phy = (features->x_max * 100) /
					features->x_resolution;
		features->y_phy = (features->y_max * 100) /
					features->y_resolution;
	}
}

static void wacom_calculate_res(struct wacom_features *features)
{
	features->x_resolution = wacom_calc_hid_res(features->x_max,
						    features->x_phy,
						    features->unit,
						    features->unitExpo);
	features->y_resolution = wacom_calc_hid_res(features->y_max,
						    features->y_phy,
						    features->unit,
						    features->unitExpo);
}

>>>>>>> refs/remotes/origin/master
static int wacom_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	struct usb_endpoint_descriptor *endpoint;
	struct wacom *wacom;
	struct wacom_wac *wacom_wac;
	struct wacom_features *features;
<<<<<<< HEAD
<<<<<<< HEAD
	struct input_dev *input_dev;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	int error;

	if (!id->driver_info)
		return -EINVAL;

	wacom = kzalloc(sizeof(struct wacom), GFP_KERNEL);
<<<<<<< HEAD
<<<<<<< HEAD
	input_dev = input_allocate_device();
	if (!wacom || !input_dev) {
		error = -ENOMEM;
		goto fail1;
	}
=======
	if (!wacom)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!wacom)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master

	wacom_wac = &wacom->wacom_wac;
	wacom_wac->features = *((struct wacom_features *)id->driver_info);
	features = &wacom_wac->features;
	if (features->pktlen > WACOM_PKGLEN_MAX) {
		error = -EINVAL;
		goto fail1;
	}

	wacom_wac->data = usb_alloc_coherent(dev, WACOM_PKGLEN_MAX,
					     GFP_KERNEL, &wacom->data_dma);
	if (!wacom_wac->data) {
		error = -ENOMEM;
		goto fail1;
	}

	wacom->irq = usb_alloc_urb(0, GFP_KERNEL);
	if (!wacom->irq) {
		error = -ENOMEM;
		goto fail2;
	}

	wacom->usbdev = dev;
	wacom->intf = intf;
	mutex_init(&wacom->lock);
<<<<<<< HEAD
<<<<<<< HEAD
	usb_make_path(dev, wacom->phys, sizeof(wacom->phys));
	strlcat(wacom->phys, "/input0", sizeof(wacom->phys));

	wacom_wac->input = input_dev;

=======
=======
>>>>>>> refs/remotes/origin/master
	INIT_WORK(&wacom->work, wacom_wireless_work);
	usb_make_path(dev, wacom->phys, sizeof(wacom->phys));
	strlcat(wacom->phys, "/input0", sizeof(wacom->phys));

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	endpoint = &intf->cur_altsetting->endpoint[0].desc;

	/* Retrieve the physical and logical size for OEM devices */
=======
	endpoint = &intf->cur_altsetting->endpoint[0].desc;

	/* set the default size in case we do not get them from hid */
	wacom_set_default_phy(features);

	/* Retrieve the physical and logical size for touch devices */
>>>>>>> refs/remotes/origin/master
	error = wacom_retrieve_hid_descriptor(intf, features);
	if (error)
		goto fail3;

<<<<<<< HEAD
	wacom_setup_device_quirks(features);

	strlcpy(wacom_wac->name, features->name, sizeof(wacom_wac->name));

	if (features->quirks & WACOM_QUIRK_MULTI_INPUT) {
		/* Append the device type to the name */
		strlcat(wacom_wac->name,
			features->device_type == BTN_TOOL_PEN ?
				" Pen" : " Finger",
			sizeof(wacom_wac->name));

		error = wacom_add_shared_data(wacom_wac, dev);
=======
	/*
	 * Intuos5 has no useful data about its touch interface in its
	 * HID descriptor. If this is the touch interface (wMaxPacketSize
	 * of WACOM_PKGLEN_BBTOUCH3), override the table values.
	 */
	if (features->type >= INTUOS5S && features->type <= INTUOSPL) {
		if (endpoint->wMaxPacketSize == WACOM_PKGLEN_BBTOUCH3) {
			features->device_type = BTN_TOOL_FINGER;
			features->pktlen = WACOM_PKGLEN_BBTOUCH3;

			features->x_max = 4096;
			features->y_max = 4096;
		} else {
			features->device_type = BTN_TOOL_PEN;
		}
	}

	wacom_setup_device_quirks(features);

	/* set unit to "100th of a mm" for devices not reported by HID */
	if (!features->unit) {
		features->unit = 0x11;
		features->unitExpo = 16 - 3;
	}
	wacom_calculate_res(features);

	strlcpy(wacom_wac->name, features->name, sizeof(wacom_wac->name));

	if (features->quirks & WACOM_QUIRK_MULTI_INPUT) {
		struct usb_device *other_dev;

		/* Append the device type to the name */
		if (features->device_type != BTN_TOOL_FINGER)
			strlcat(wacom_wac->name, " Pen", WACOM_NAME_MAX);
		else if (features->touch_max)
			strlcat(wacom_wac->name, " Finger", WACOM_NAME_MAX);
		else
			strlcat(wacom_wac->name, " Pad", WACOM_NAME_MAX);

		other_dev = wacom_get_sibling(dev, features->oVid, features->oPid);
		if (other_dev == NULL || wacom_get_usbdev_data(other_dev) == NULL)
			other_dev = dev;
		error = wacom_add_shared_data(wacom_wac, other_dev);
>>>>>>> refs/remotes/origin/master
		if (error)
			goto fail3;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	input_dev->name = wacom_wac->name;
	input_dev->dev.parent = &intf->dev;
	input_dev->open = wacom_open;
	input_dev->close = wacom_close;
	usb_to_input_id(dev, &input_dev->id);
	input_set_drvdata(input_dev, wacom);

	wacom_setup_input_capabilities(input_dev, wacom_wac);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	usb_fill_int_urb(wacom->irq, dev,
			 usb_rcvintpipe(dev, endpoint->bEndpointAddress),
			 wacom_wac->data, features->pktlen,
			 wacom_sys_irq, wacom, endpoint->bInterval);
	wacom->irq->transfer_dma = wacom->data_dma;
	wacom->irq->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

<<<<<<< HEAD
<<<<<<< HEAD
	error = input_register_device(input_dev);
	if (error)
		goto fail4;

=======
=======
>>>>>>> refs/remotes/origin/master
	error = wacom_initialize_leds(wacom);
	if (error)
		goto fail4;

<<<<<<< HEAD
	error = wacom_initialize_battery(wacom);
	if (error)
		goto fail5;

	if (!(features->quirks & WACOM_QUIRK_NO_INPUT)) {
		error = wacom_register_input(wacom);
		if (error)
			goto fail6;
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!(features->quirks & WACOM_QUIRK_NO_INPUT)) {
		error = wacom_register_input(wacom);
		if (error)
			goto fail5;
	}

>>>>>>> refs/remotes/origin/master
	/* Note that if query fails it is not a hard failure */
	wacom_query_tablet_data(intf, features);

	usb_set_intfdata(intf, wacom);
<<<<<<< HEAD
<<<<<<< HEAD
	return 0;

 fail4:	wacom_remove_shared_data(wacom_wac);
 fail3:	usb_free_urb(wacom->irq);
 fail2:	usb_free_coherent(dev, WACOM_PKGLEN_MAX, wacom_wac->data, wacom->data_dma);
 fail1:	input_free_device(input_dev);
	kfree(wacom);
=======

	if (features->quirks & WACOM_QUIRK_MONITOR) {
		if (usb_submit_urb(wacom->irq, GFP_KERNEL))
			goto fail5;
=======

	if (features->quirks & WACOM_QUIRK_MONITOR) {
		if (usb_submit_urb(wacom->irq, GFP_KERNEL)) {
			error = -EIO;
			goto fail5;
		}
>>>>>>> refs/remotes/origin/master
	}

	return 0;

<<<<<<< HEAD
 fail6: wacom_destroy_battery(wacom);
=======
>>>>>>> refs/remotes/origin/master
 fail5: wacom_destroy_leds(wacom);
 fail4:	wacom_remove_shared_data(wacom_wac);
 fail3:	usb_free_urb(wacom->irq);
 fail2:	usb_free_coherent(dev, WACOM_PKGLEN_MAX, wacom_wac->data, wacom->data_dma);
 fail1:	kfree(wacom);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return error;
}

static void wacom_disconnect(struct usb_interface *intf)
{
	struct wacom *wacom = usb_get_intfdata(intf);

	usb_set_intfdata(intf, NULL);

	usb_kill_urb(wacom->irq);
<<<<<<< HEAD
<<<<<<< HEAD
	input_unregister_device(wacom->wacom_wac.input);
=======
=======
>>>>>>> refs/remotes/origin/master
	cancel_work_sync(&wacom->work);
	if (wacom->wacom_wac.input)
		input_unregister_device(wacom->wacom_wac.input);
	wacom_destroy_battery(wacom);
	wacom_destroy_leds(wacom);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	usb_free_urb(wacom->irq);
	usb_free_coherent(interface_to_usbdev(intf), WACOM_PKGLEN_MAX,
			wacom->wacom_wac.data, wacom->data_dma);
	wacom_remove_shared_data(&wacom->wacom_wac);
	kfree(wacom);
}

static int wacom_suspend(struct usb_interface *intf, pm_message_t message)
{
	struct wacom *wacom = usb_get_intfdata(intf);

	mutex_lock(&wacom->lock);
	usb_kill_urb(wacom->irq);
	mutex_unlock(&wacom->lock);

	return 0;
}

static int wacom_resume(struct usb_interface *intf)
{
	struct wacom *wacom = usb_get_intfdata(intf);
	struct wacom_features *features = &wacom->wacom_wac.features;
<<<<<<< HEAD
<<<<<<< HEAD
	int rv;
=======
	int rv = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int rv = 0;
>>>>>>> refs/remotes/origin/master

	mutex_lock(&wacom->lock);

	/* switch to wacom mode first */
	wacom_query_tablet_data(intf, features);
<<<<<<< HEAD
<<<<<<< HEAD

	if (wacom->open)
		rv = usb_submit_urb(wacom->irq, GFP_NOIO);
	else
		rv = 0;
=======
	wacom_led_control(wacom);

	if ((wacom->open || features->quirks & WACOM_QUIRK_MONITOR)
	     && usb_submit_urb(wacom->irq, GFP_NOIO) < 0)
		rv = -EIO;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	wacom_led_control(wacom);

	if ((wacom->open || (features->quirks & WACOM_QUIRK_MONITOR)) &&
	    usb_submit_urb(wacom->irq, GFP_NOIO) < 0)
		rv = -EIO;
>>>>>>> refs/remotes/origin/master

	mutex_unlock(&wacom->lock);

	return rv;
}

static int wacom_reset_resume(struct usb_interface *intf)
{
	return wacom_resume(intf);
}

static struct usb_driver wacom_driver = {
	.name =		"wacom",
	.id_table =	wacom_ids,
	.probe =	wacom_probe,
	.disconnect =	wacom_disconnect,
	.suspend =	wacom_suspend,
	.resume =	wacom_resume,
	.reset_resume =	wacom_reset_resume,
	.supports_autosuspend = 1,
};

<<<<<<< HEAD
<<<<<<< HEAD
static int __init wacom_init(void)
{
	int result;

	result = usb_register(&wacom_driver);
	if (result == 0)
		printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_VERSION ":"
		       DRIVER_DESC "\n");
	return result;
}

static void __exit wacom_exit(void)
{
	usb_deregister(&wacom_driver);
}

module_init(wacom_init);
module_exit(wacom_exit);
=======
module_usb_driver(wacom_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_usb_driver(wacom_driver);
>>>>>>> refs/remotes/origin/master
