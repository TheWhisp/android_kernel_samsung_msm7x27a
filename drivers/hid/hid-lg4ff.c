/*
<<<<<<< HEAD
 *  Force feedback support for Logitech Speed Force Wireless
 *
 *  http://wiibrew.org/wiki/Logitech_USB_steering_wheel
=======
 *  Force feedback support for Logitech Gaming Wheels
 *
 *  Including G27, G25, DFP, DFGT, FFEX, Momo, Momo2 &
 *  Speed Force Wireless (WiiWheel)
>>>>>>> refs/remotes/origin/master
 *
 *  Copyright (c) 2010 Simon Wood <simon@mungewell.org>
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */


#include <linux/input.h>
#include <linux/usb.h>
#include <linux/hid.h>

#include "usbhid/usbhid.h"
#include "hid-lg.h"
<<<<<<< HEAD
<<<<<<< HEAD

struct lg4ff_device {
	struct hid_report *report;
};

static const signed short ff4_wheel_ac[] = {
=======
=======
>>>>>>> refs/remotes/origin/master
#include "hid-ids.h"

#define DFGT_REV_MAJ 0x13
#define DFGT_REV_MIN 0x22
<<<<<<< HEAD
=======
#define DFGT2_REV_MIN 0x26
>>>>>>> refs/remotes/origin/master
#define DFP_REV_MAJ 0x11
#define DFP_REV_MIN 0x06
#define FFEX_REV_MAJ 0x21
#define FFEX_REV_MIN 0x00
#define G25_REV_MAJ 0x12
#define G25_REV_MIN 0x22
#define G27_REV_MAJ 0x12
#define G27_REV_MIN 0x38

#define to_hid_device(pdev) container_of(pdev, struct hid_device, dev)

static void hid_lg4ff_set_range_dfp(struct hid_device *hid, u16 range);
static void hid_lg4ff_set_range_g25(struct hid_device *hid, u16 range);
static ssize_t lg4ff_range_show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t lg4ff_range_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);

static DEVICE_ATTR(range, S_IRWXU | S_IRWXG | S_IRWXO, lg4ff_range_show, lg4ff_range_store);

<<<<<<< HEAD
static bool list_inited;

struct lg4ff_device_entry {
	char  *device_id;	/* Use name in respective kobject structure's address as the ID */
	__u16 range;
	__u16 min_range;
	__u16 max_range;
	__u8  leds;
=======
struct lg4ff_device_entry {
	__u32 product_id;
	__u16 range;
	__u16 min_range;
	__u16 max_range;
#ifdef CONFIG_LEDS_CLASS
	__u8  led_state;
	struct led_classdev *led[5];
#endif
>>>>>>> refs/remotes/origin/master
	struct list_head list;
	void (*set_range)(struct hid_device *hid, u16 range);
};

<<<<<<< HEAD
static struct lg4ff_device_entry device_list;

static const signed short lg4ff_wheel_effects[] = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const signed short lg4ff_wheel_effects[] = {
>>>>>>> refs/remotes/origin/master
	FF_CONSTANT,
	FF_AUTOCENTER,
	-1
};

<<<<<<< HEAD
<<<<<<< HEAD
static int hid_lg4ff_play(struct input_dev *dev, void *data,
			 struct ff_effect *effect)
=======
=======
>>>>>>> refs/remotes/origin/master
struct lg4ff_wheel {
	const __u32 product_id;
	const signed short *ff_effects;
	const __u16 min_range;
	const __u16 max_range;
	void (*set_range)(struct hid_device *hid, u16 range);
};

static const struct lg4ff_wheel lg4ff_devices[] = {
	{USB_DEVICE_ID_LOGITECH_WHEEL,       lg4ff_wheel_effects, 40, 270, NULL},
	{USB_DEVICE_ID_LOGITECH_MOMO_WHEEL,  lg4ff_wheel_effects, 40, 270, NULL},
	{USB_DEVICE_ID_LOGITECH_DFP_WHEEL,   lg4ff_wheel_effects, 40, 900, hid_lg4ff_set_range_dfp},
	{USB_DEVICE_ID_LOGITECH_G25_WHEEL,   lg4ff_wheel_effects, 40, 900, hid_lg4ff_set_range_g25},
	{USB_DEVICE_ID_LOGITECH_DFGT_WHEEL,  lg4ff_wheel_effects, 40, 900, hid_lg4ff_set_range_g25},
	{USB_DEVICE_ID_LOGITECH_G27_WHEEL,   lg4ff_wheel_effects, 40, 900, hid_lg4ff_set_range_g25},
	{USB_DEVICE_ID_LOGITECH_MOMO_WHEEL2, lg4ff_wheel_effects, 40, 270, NULL},
	{USB_DEVICE_ID_LOGITECH_WII_WHEEL,   lg4ff_wheel_effects, 40, 270, NULL}
};

struct lg4ff_native_cmd {
	const __u8 cmd_num;	/* Number of commands to send */
	const __u8 cmd[];
};

struct lg4ff_usb_revision {
	const __u16 rev_maj;
	const __u16 rev_min;
	const struct lg4ff_native_cmd *command;
};

static const struct lg4ff_native_cmd native_dfp = {
	1,
	{0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}
};

static const struct lg4ff_native_cmd native_dfgt = {
	2,
	{0xf8, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00,	/* 1st command */
	 0xf8, 0x09, 0x03, 0x01, 0x00, 0x00, 0x00}	/* 2nd command */
};

static const struct lg4ff_native_cmd native_g25 = {
	1,
	{0xf8, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00}
};

static const struct lg4ff_native_cmd native_g27 = {
	2,
	{0xf8, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00,	/* 1st command */
	 0xf8, 0x09, 0x04, 0x01, 0x00, 0x00, 0x00}	/* 2nd command */
};

static const struct lg4ff_usb_revision lg4ff_revs[] = {
	{DFGT_REV_MAJ, DFGT_REV_MIN, &native_dfgt},	/* Driving Force GT */
<<<<<<< HEAD
=======
	{DFGT_REV_MAJ, DFGT2_REV_MIN, &native_dfgt},	/* Driving Force GT v2 */
>>>>>>> refs/remotes/origin/master
	{DFP_REV_MAJ,  DFP_REV_MIN,  &native_dfp},	/* Driving Force Pro */
	{G25_REV_MAJ,  G25_REV_MIN,  &native_g25},	/* G25 */
	{G27_REV_MAJ,  G27_REV_MIN,  &native_g27},	/* G27 */
};

<<<<<<< HEAD
static int hid_lg4ff_play(struct input_dev *dev, void *data, struct ff_effect *effect)
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Recalculates X axis value accordingly to currently selected range */
static __s32 lg4ff_adjust_dfp_x_axis(__s32 value, __u16 range)
{
	__u16 max_range;
	__s32 new_value;

	if (range == 900)
		return value;
	else if (range == 200)
		return value;
	else if (range < 200)
		max_range = 200;
	else
		max_range = 900;

	new_value = 8192 + mult_frac(value - 8192, max_range, range);
	if (new_value < 0)
		return 0;
	else if (new_value > 16383)
		return 16383;
	else
		return new_value;
}

int lg4ff_adjust_input_event(struct hid_device *hid, struct hid_field *field,
			     struct hid_usage *usage, __s32 value, struct lg_drv_data *drv_data)
{
	struct lg4ff_device_entry *entry = drv_data->device_props;
	__s32 new_value = 0;

	if (!entry) {
		hid_err(hid, "Device properties not found");
		return 0;
	}

	switch (entry->product_id) {
	case USB_DEVICE_ID_LOGITECH_DFP_WHEEL:
		switch (usage->code) {
		case ABS_X:
			new_value = lg4ff_adjust_dfp_x_axis(value, entry->range);
			input_event(field->hidinput->input, usage->type, usage->code, new_value);
			return 1;
		default:
			return 0;
		}
	default:
		return 0;
	}
}

static int hid_lg4ff_play(struct input_dev *dev, void *data, struct ff_effect *effect)
>>>>>>> refs/remotes/origin/master
{
	struct hid_device *hid = input_get_drvdata(dev);
	struct list_head *report_list = &hid->report_enum[HID_OUTPUT_REPORT].report_list;
	struct hid_report *report = list_entry(report_list->next, struct hid_report, list);
<<<<<<< HEAD
	int x;

#define CLAMP(x) if (x < 0) x = 0; if (x > 0xff) x = 0xff
=======
	__s32 *value = report->field[0]->value;
	int x;

#define CLAMP(x) do { if (x < 0) x = 0; else if (x > 0xff) x = 0xff; } while (0)
>>>>>>> refs/remotes/origin/master

	switch (effect->type) {
	case FF_CONSTANT:
		x = effect->u.ramp.start_level + 0x80;	/* 0x80 is no force */
		CLAMP(x);
<<<<<<< HEAD
		report->field[0]->value[0] = 0x11;	/* Slot 1 */
<<<<<<< HEAD
		report->field[0]->value[1] = 0x10;
		report->field[0]->value[2] = x;
		report->field[0]->value[3] = 0x00;
		report->field[0]->value[4] = 0x00;
		report->field[0]->value[5] = 0x08;
		report->field[0]->value[6] = 0x00;
		dbg_hid("Autocenter, x=0x%02X\n", x);
=======
		report->field[0]->value[1] = 0x08;
		report->field[0]->value[2] = x;
		report->field[0]->value[3] = 0x80;
		report->field[0]->value[4] = 0x00;
		report->field[0]->value[5] = 0x00;
		report->field[0]->value[6] = 0x00;
>>>>>>> refs/remotes/origin/cm-10.0

		usbhid_submit_report(hid, report, USB_DIR_OUT);
=======

		if (x == 0x80) {
			/* De-activate force in slot-1*/
			value[0] = 0x13;
			value[1] = 0x00;
			value[2] = 0x00;
			value[3] = 0x00;
			value[4] = 0x00;
			value[5] = 0x00;
			value[6] = 0x00;

			hid_hw_request(hid, report, HID_REQ_SET_REPORT);
			return 0;
		}

		value[0] = 0x11;	/* Slot 1 */
		value[1] = 0x08;
		value[2] = x;
		value[3] = 0x80;
		value[4] = 0x00;
		value[5] = 0x00;
		value[6] = 0x00;

		hid_hw_request(hid, report, HID_REQ_SET_REPORT);
>>>>>>> refs/remotes/origin/master
		break;
	}
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void hid_lg4ff_set_autocenter(struct input_dev *dev, u16 magnitude)
=======
/* Sends default autocentering command compatible with
 * all wheels except Formula Force EX */
static void hid_lg4ff_set_autocenter_default(struct input_dev *dev, u16 magnitude)
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Sends default autocentering command compatible with
 * all wheels except Formula Force EX */
static void hid_lg4ff_set_autocenter_default(struct input_dev *dev, u16 magnitude)
>>>>>>> refs/remotes/origin/master
{
	struct hid_device *hid = input_get_drvdata(dev);
	struct list_head *report_list = &hid->report_enum[HID_OUTPUT_REPORT].report_list;
	struct hid_report *report = list_entry(report_list->next, struct hid_report, list);
<<<<<<< HEAD
<<<<<<< HEAD
	__s32 *value = report->field[0]->value;

	*value++ = 0xfe;
	*value++ = 0x0d;
	*value++ = 0x07;
	*value++ = 0x07;
	*value++ = (magnitude >> 8) & 0xff;
	*value++ = 0x00;
	*value = 0x00;
=======

	report->field[0]->value[0] = 0xfe;
	report->field[0]->value[1] = 0x0d;
	report->field[0]->value[2] = magnitude >> 13;
	report->field[0]->value[3] = magnitude >> 13;
	report->field[0]->value[4] = magnitude >> 8;
	report->field[0]->value[5] = 0x00;
	report->field[0]->value[6] = 0x00;

	usbhid_submit_report(hid, report, USB_DIR_OUT);
=======
	__s32 *value = report->field[0]->value;
	__u32 expand_a, expand_b;
	struct lg4ff_device_entry *entry;
	struct lg_drv_data *drv_data;

	drv_data = hid_get_drvdata(hid);
	if (!drv_data) {
		hid_err(hid, "Private driver data not found!\n");
		return;
	}

	entry = drv_data->device_props;
	if (!entry) {
		hid_err(hid, "Device properties not found!\n");
		return;
	}

	/* De-activate Auto-Center */
	if (magnitude == 0) {
		value[0] = 0xf5;
		value[1] = 0x00;
		value[2] = 0x00;
		value[3] = 0x00;
		value[4] = 0x00;
		value[5] = 0x00;
		value[6] = 0x00;

		hid_hw_request(hid, report, HID_REQ_SET_REPORT);
		return;
	}

	if (magnitude <= 0xaaaa) {
		expand_a = 0x0c * magnitude;
		expand_b = 0x80 * magnitude;
	} else {
		expand_a = (0x0c * 0xaaaa) + 0x06 * (magnitude - 0xaaaa);
		expand_b = (0x80 * 0xaaaa) + 0xff * (magnitude - 0xaaaa);
	}

	/* Adjust for non-MOMO wheels */
	switch (entry->product_id) {
	case USB_DEVICE_ID_LOGITECH_MOMO_WHEEL:
	case USB_DEVICE_ID_LOGITECH_MOMO_WHEEL2:
		break;
	default:
		expand_a = expand_a >> 1;
		break;
	}

	value[0] = 0xfe;
	value[1] = 0x0d;
	value[2] = expand_a / 0xaaaa;
	value[3] = expand_a / 0xaaaa;
	value[4] = expand_b / 0xaaaa;
	value[5] = 0x00;
	value[6] = 0x00;

	hid_hw_request(hid, report, HID_REQ_SET_REPORT);

	/* Activate Auto-Center */
	value[0] = 0x14;
	value[1] = 0x00;
	value[2] = 0x00;
	value[3] = 0x00;
	value[4] = 0x00;
	value[5] = 0x00;
	value[6] = 0x00;

	hid_hw_request(hid, report, HID_REQ_SET_REPORT);
>>>>>>> refs/remotes/origin/master
}

/* Sends autocentering command compatible with Formula Force EX */
static void hid_lg4ff_set_autocenter_ffex(struct input_dev *dev, u16 magnitude)
{
	struct hid_device *hid = input_get_drvdata(dev);
	struct list_head *report_list = &hid->report_enum[HID_OUTPUT_REPORT].report_list;
	struct hid_report *report = list_entry(report_list->next, struct hid_report, list);
<<<<<<< HEAD
	magnitude = magnitude * 90 / 65535;
	

	report->field[0]->value[0] = 0xfe;
	report->field[0]->value[1] = 0x03;
	report->field[0]->value[2] = magnitude >> 14;
	report->field[0]->value[3] = magnitude >> 14;
	report->field[0]->value[4] = magnitude;
	report->field[0]->value[5] = 0x00;
	report->field[0]->value[6] = 0x00;

	usbhid_submit_report(hid, report, USB_DIR_OUT);
=======
	__s32 *value = report->field[0]->value;
	magnitude = magnitude * 90 / 65535;

	value[0] = 0xfe;
	value[1] = 0x03;
	value[2] = magnitude >> 14;
	value[3] = magnitude >> 14;
	value[4] = magnitude;
	value[5] = 0x00;
	value[6] = 0x00;

	hid_hw_request(hid, report, HID_REQ_SET_REPORT);
>>>>>>> refs/remotes/origin/master
}

/* Sends command to set range compatible with G25/G27/Driving Force GT */
static void hid_lg4ff_set_range_g25(struct hid_device *hid, u16 range)
{
	struct list_head *report_list = &hid->report_enum[HID_OUTPUT_REPORT].report_list;
	struct hid_report *report = list_entry(report_list->next, struct hid_report, list);
<<<<<<< HEAD
	dbg_hid("G25/G27/DFGT: setting range to %u\n", range);

	report->field[0]->value[0] = 0xf8;
	report->field[0]->value[1] = 0x81;
	report->field[0]->value[2] = range & 0x00ff;
	report->field[0]->value[3] = (range & 0xff00) >> 8;
	report->field[0]->value[4] = 0x00;
	report->field[0]->value[5] = 0x00;
	report->field[0]->value[6] = 0x00;

	usbhid_submit_report(hid, report, USB_DIR_OUT);
=======
	__s32 *value = report->field[0]->value;

	dbg_hid("G25/G27/DFGT: setting range to %u\n", range);

	value[0] = 0xf8;
	value[1] = 0x81;
	value[2] = range & 0x00ff;
	value[3] = (range & 0xff00) >> 8;
	value[4] = 0x00;
	value[5] = 0x00;
	value[6] = 0x00;

	hid_hw_request(hid, report, HID_REQ_SET_REPORT);
>>>>>>> refs/remotes/origin/master
}

/* Sends commands to set range compatible with Driving Force Pro wheel */
static void hid_lg4ff_set_range_dfp(struct hid_device *hid, __u16 range)
{
	struct list_head *report_list = &hid->report_enum[HID_OUTPUT_REPORT].report_list;
	struct hid_report *report = list_entry(report_list->next, struct hid_report, list);
	int start_left, start_right, full_range;
<<<<<<< HEAD
	dbg_hid("Driving Force Pro: setting range to %u\n", range);

	/* Prepare "coarse" limit command */
	report->field[0]->value[0] = 0xf8;
	report->field[0]->value[1] = 0x00; 	/* Set later */
	report->field[0]->value[2] = 0x00;
	report->field[0]->value[3] = 0x00;
	report->field[0]->value[4] = 0x00;
	report->field[0]->value[5] = 0x00;
	report->field[0]->value[6] = 0x00;
=======
	__s32 *value = report->field[0]->value;

	dbg_hid("Driving Force Pro: setting range to %u\n", range);

	/* Prepare "coarse" limit command */
	value[0] = 0xf8;
	value[1] = 0x00;	/* Set later */
	value[2] = 0x00;
	value[3] = 0x00;
	value[4] = 0x00;
	value[5] = 0x00;
	value[6] = 0x00;
>>>>>>> refs/remotes/origin/master

	if (range > 200) {
		report->field[0]->value[1] = 0x03;
		full_range = 900;
	} else {
		report->field[0]->value[1] = 0x02;
		full_range = 200;
	}
<<<<<<< HEAD
	usbhid_submit_report(hid, report, USB_DIR_OUT);

	/* Prepare "fine" limit command */
	report->field[0]->value[0] = 0x81;
	report->field[0]->value[1] = 0x0b;
	report->field[0]->value[2] = 0x00;
	report->field[0]->value[3] = 0x00;
	report->field[0]->value[4] = 0x00;
	report->field[0]->value[5] = 0x00;
	report->field[0]->value[6] = 0x00;

	if (range == 200 || range == 900) {	/* Do not apply any fine limit */
		usbhid_submit_report(hid, report, USB_DIR_OUT);
=======
	hid_hw_request(hid, report, HID_REQ_SET_REPORT);

	/* Prepare "fine" limit command */
	value[0] = 0x81;
	value[1] = 0x0b;
	value[2] = 0x00;
	value[3] = 0x00;
	value[4] = 0x00;
	value[5] = 0x00;
	value[6] = 0x00;

	if (range == 200 || range == 900) {	/* Do not apply any fine limit */
		hid_hw_request(hid, report, HID_REQ_SET_REPORT);
>>>>>>> refs/remotes/origin/master
		return;
	}

	/* Construct fine limit command */
	start_left = (((full_range - range + 1) * 2047) / full_range);
	start_right = 0xfff - start_left;

<<<<<<< HEAD
	report->field[0]->value[2] = start_left >> 4;
	report->field[0]->value[3] = start_right >> 4;
	report->field[0]->value[4] = 0xff;
	report->field[0]->value[5] = (start_right & 0xe) << 4 | (start_left & 0xe);
	report->field[0]->value[6] = 0xff;
>>>>>>> refs/remotes/origin/cm-10.0

	usbhid_submit_report(hid, report, USB_DIR_OUT);
}

<<<<<<< HEAD
=======
=======
	value[2] = start_left >> 4;
	value[3] = start_right >> 4;
	value[4] = 0xff;
	value[5] = (start_right & 0xe) << 4 | (start_left & 0xe);
	value[6] = 0xff;

	hid_hw_request(hid, report, HID_REQ_SET_REPORT);
}

>>>>>>> refs/remotes/origin/master
static void hid_lg4ff_switch_native(struct hid_device *hid, const struct lg4ff_native_cmd *cmd)
{
	struct list_head *report_list = &hid->report_enum[HID_OUTPUT_REPORT].report_list;
	struct hid_report *report = list_entry(report_list->next, struct hid_report, list);
	__u8 i, j;

	j = 0;
	while (j < 7*cmd->cmd_num) {
		for (i = 0; i < 7; i++)
			report->field[0]->value[i] = cmd->cmd[j++];

<<<<<<< HEAD
		usbhid_submit_report(hid, report, USB_DIR_OUT);
=======
		hid_hw_request(hid, report, HID_REQ_SET_REPORT);
>>>>>>> refs/remotes/origin/master
	}
}

/* Read current range and display it in terminal */
static ssize_t lg4ff_range_show(struct device *dev, struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct lg4ff_device_entry *uninitialized_var(entry);
	struct list_head *h;
	struct hid_device *hid = to_hid_device(dev);
	size_t count;

	list_for_each(h, &device_list.list) {
		entry = list_entry(h, struct lg4ff_device_entry, list);
		if (strcmp(entry->device_id, (&hid->dev)->kobj.name) == 0)
			break;
	}
	if (h == &device_list.list) {
		dbg_hid("Device not found!");
=======
	struct hid_device *hid = to_hid_device(dev);
	struct lg4ff_device_entry *entry;
	struct lg_drv_data *drv_data;
	size_t count;

	drv_data = hid_get_drvdata(hid);
	if (!drv_data) {
		hid_err(hid, "Private driver data not found!\n");
		return 0;
	}

	entry = drv_data->device_props;
	if (!entry) {
		hid_err(hid, "Device properties not found!\n");
>>>>>>> refs/remotes/origin/master
		return 0;
	}

	count = scnprintf(buf, PAGE_SIZE, "%u\n", entry->range);
	return count;
}

/* Set range to user specified value, call appropriate function
 * according to the type of the wheel */
static ssize_t lg4ff_range_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
<<<<<<< HEAD
	struct lg4ff_device_entry *uninitialized_var(entry);
	struct list_head *h;
	struct hid_device *hid = to_hid_device(dev);
	__u16 range = simple_strtoul(buf, NULL, 10);

	list_for_each(h, &device_list.list) {
		entry = list_entry(h, struct lg4ff_device_entry, list);
		if (strcmp(entry->device_id, (&hid->dev)->kobj.name) == 0)
			break;
	}
	if (h == &device_list.list) {
		dbg_hid("Device not found!");
		return count;
=======
	struct hid_device *hid = to_hid_device(dev);
	struct lg4ff_device_entry *entry;
	struct lg_drv_data *drv_data;
	__u16 range = simple_strtoul(buf, NULL, 10);

	drv_data = hid_get_drvdata(hid);
	if (!drv_data) {
		hid_err(hid, "Private driver data not found!\n");
		return 0;
	}

	entry = drv_data->device_props;
	if (!entry) {
		hid_err(hid, "Device properties not found!\n");
		return 0;
>>>>>>> refs/remotes/origin/master
	}

	if (range == 0)
		range = entry->max_range;

	/* Check if the wheel supports range setting
	 * and that the range is within limits for the wheel */
	if (entry->set_range != NULL && range >= entry->min_range && range <= entry->max_range) {
		entry->set_range(hid, range);
		entry->range = range;
	}

	return count;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

int lg4ff_init(struct hid_device *hid)
{
	struct hid_input *hidinput = list_entry(hid->inputs.next, struct hid_input, list);
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head *report_list = &hid->report_enum[HID_OUTPUT_REPORT].report_list;
	struct input_dev *dev = hidinput->input;
	struct hid_report *report;
	struct hid_field *field;
	const signed short *ff_bits = ff4_wheel_ac;
	int error;
	int i;
=======
	struct input_dev *dev = hidinput->input;
	struct lg4ff_device_entry *entry;
	struct usb_device_descriptor *udesc;
	int error, i, j;
	__u16 bcdDevice, rev_maj, rev_min;
>>>>>>> refs/remotes/origin/cm-11.0

	/* Check that the report looks ok */
	if (!hid_validate_values(hid, HID_OUTPUT_REPORT, 0, 0, 7))
		return -1;

	for (i = 0; ff_bits[i] >= 0; i++)
		set_bit(ff_bits[i], dev->ffbit);
=======
	struct input_dev *dev = hidinput->input;
	struct lg4ff_device_entry *entry;
=======

#ifdef CONFIG_LEDS_CLASS
static void lg4ff_set_leds(struct hid_device *hid, __u8 leds)
{
	struct list_head *report_list = &hid->report_enum[HID_OUTPUT_REPORT].report_list;
	struct hid_report *report = list_entry(report_list->next, struct hid_report, list);
	__s32 *value = report->field[0]->value;

	value[0] = 0xf8;
	value[1] = 0x12;
	value[2] = leds;
	value[3] = 0x00;
	value[4] = 0x00;
	value[5] = 0x00;
	value[6] = 0x00;
	hid_hw_request(hid, report, HID_REQ_SET_REPORT);
}

static void lg4ff_led_set_brightness(struct led_classdev *led_cdev,
			enum led_brightness value)
{
	struct device *dev = led_cdev->dev->parent;
	struct hid_device *hid = container_of(dev, struct hid_device, dev);
	struct lg_drv_data *drv_data = hid_get_drvdata(hid);
	struct lg4ff_device_entry *entry;
	int i, state = 0;

	if (!drv_data) {
		hid_err(hid, "Device data not found.");
		return;
	}

	entry = (struct lg4ff_device_entry *)drv_data->device_props;

	if (!entry) {
		hid_err(hid, "Device properties not found.");
		return;
	}

	for (i = 0; i < 5; i++) {
		if (led_cdev != entry->led[i])
			continue;
		state = (entry->led_state >> i) & 1;
		if (value == LED_OFF && state) {
			entry->led_state &= ~(1 << i);
			lg4ff_set_leds(hid, entry->led_state);
		} else if (value != LED_OFF && !state) {
			entry->led_state |= 1 << i;
			lg4ff_set_leds(hid, entry->led_state);
		}
		break;
	}
}

static enum led_brightness lg4ff_led_get_brightness(struct led_classdev *led_cdev)
{
	struct device *dev = led_cdev->dev->parent;
	struct hid_device *hid = container_of(dev, struct hid_device, dev);
	struct lg_drv_data *drv_data = hid_get_drvdata(hid);
	struct lg4ff_device_entry *entry;
	int i, value = 0;

	if (!drv_data) {
		hid_err(hid, "Device data not found.");
		return LED_OFF;
	}

	entry = (struct lg4ff_device_entry *)drv_data->device_props;

	if (!entry) {
		hid_err(hid, "Device properties not found.");
		return LED_OFF;
	}

	for (i = 0; i < 5; i++)
		if (led_cdev == entry->led[i]) {
			value = (entry->led_state >> i) & 1;
			break;
		}

	return value ? LED_FULL : LED_OFF;
}
#endif

int lg4ff_init(struct hid_device *hid)
{
	struct hid_input *hidinput = list_entry(hid->inputs.next, struct hid_input, list);
	struct input_dev *dev = hidinput->input;
	struct lg4ff_device_entry *entry;
	struct lg_drv_data *drv_data;
>>>>>>> refs/remotes/origin/master
	struct usb_device_descriptor *udesc;
	int error, i, j;
	__u16 bcdDevice, rev_maj, rev_min;

	/* Check that the report looks ok */
	if (!hid_validate_values(hid, HID_OUTPUT_REPORT, 0, 0, 7))
		return -1;

	/* Check what wheel has been connected */
	for (i = 0; i < ARRAY_SIZE(lg4ff_devices); i++) {
		if (hid->product == lg4ff_devices[i].product_id) {
			dbg_hid("Found compatible device, product ID %04X\n", lg4ff_devices[i].product_id);
			break;
		}
	}

	if (i == ARRAY_SIZE(lg4ff_devices)) {
		hid_err(hid, "Device is not supported by lg4ff driver. If you think it should be, consider reporting a bug to"
			     "LKML, Simon Wood <simon@mungewell.org> or Michal Maly <madcatxster@gmail.com>\n");
		return -1;
	}

	/* Attempt to switch wheel to native mode when applicable */
	udesc = &(hid_to_usb_dev(hid)->descriptor);
	if (!udesc) {
		hid_err(hid, "NULL USB device descriptor\n");
		return -1;
	}
	bcdDevice = le16_to_cpu(udesc->bcdDevice);
	rev_maj = bcdDevice >> 8;
	rev_min = bcdDevice & 0xff;

	if (lg4ff_devices[i].product_id == USB_DEVICE_ID_LOGITECH_WHEEL) {
		dbg_hid("Generic wheel detected, can it do native?\n");
		dbg_hid("USB revision: %2x.%02x\n", rev_maj, rev_min);

		for (j = 0; j < ARRAY_SIZE(lg4ff_revs); j++) {
			if (lg4ff_revs[j].rev_maj == rev_maj && lg4ff_revs[j].rev_min == rev_min) {
				hid_lg4ff_switch_native(hid, lg4ff_revs[j].command);
				hid_info(hid, "Switched to native mode\n");
			}
		}
	}

	/* Set supported force feedback capabilities */
	for (j = 0; lg4ff_devices[i].ff_effects[j] >= 0; j++)
		set_bit(lg4ff_devices[i].ff_effects[j], dev->ffbit);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	error = input_ff_create_memless(dev, NULL, hid_lg4ff_play);

	if (error)
		return error;

<<<<<<< HEAD
<<<<<<< HEAD
	if (test_bit(FF_AUTOCENTER, dev->ffbit))
		dev->ff->set_autocenter = hid_lg4ff_set_autocenter;
=======
	/* Check if autocentering is available and
	 * set the centering force to zero by default */
	if (test_bit(FF_AUTOCENTER, dev->ffbit)) {
		if(rev_maj == FFEX_REV_MAJ && rev_min == FFEX_REV_MIN)	/* Formula Force EX expects different autocentering command */
			dev->ff->set_autocenter = hid_lg4ff_set_autocenter_ffex;
		else
			dev->ff->set_autocenter = hid_lg4ff_set_autocenter_default;

		dev->ff->set_autocenter(dev, 0);
	}

		/* Initialize device_list if this is the first device to handle by lg4ff */
	if (!list_inited) {
		INIT_LIST_HEAD(&device_list.list);
		list_inited = 1;
	}

	/* Add the device to device_list */
	entry = kzalloc(sizeof(struct lg4ff_device_entry), GFP_KERNEL);
	if (!entry) {
		hid_err(hid, "Cannot add device, insufficient memory.\n");
		return -ENOMEM;
	}
	entry->device_id = kstrdup((&hid->dev)->kobj.name, GFP_KERNEL);
	if (!entry->device_id) {
		hid_err(hid, "Cannot set device_id, insufficient memory.\n");
		kfree(entry);
		return -ENOMEM;
	}
	entry->min_range = lg4ff_devices[i].min_range;
	entry->max_range = lg4ff_devices[i].max_range;
	entry->set_range = lg4ff_devices[i].set_range;
	list_add(&entry->list, &device_list.list);
=======
	/* Get private driver data */
	drv_data = hid_get_drvdata(hid);
	if (!drv_data) {
		hid_err(hid, "Cannot add device, private driver data not allocated\n");
		return -1;
	}

	/* Initialize device properties */
	entry = kzalloc(sizeof(struct lg4ff_device_entry), GFP_KERNEL);
	if (!entry) {
		hid_err(hid, "Cannot add device, insufficient memory to allocate device properties.\n");
		return -ENOMEM;
	}
	drv_data->device_props = entry;

	entry->product_id = lg4ff_devices[i].product_id;
	entry->min_range = lg4ff_devices[i].min_range;
	entry->max_range = lg4ff_devices[i].max_range;
	entry->set_range = lg4ff_devices[i].set_range;

	/* Check if autocentering is available and
	 * set the centering force to zero by default */
	if (test_bit(FF_AUTOCENTER, dev->ffbit)) {
		if (rev_maj == FFEX_REV_MAJ && rev_min == FFEX_REV_MIN)	/* Formula Force EX expects different autocentering command */
			dev->ff->set_autocenter = hid_lg4ff_set_autocenter_ffex;
		else
			dev->ff->set_autocenter = hid_lg4ff_set_autocenter_default;

		dev->ff->set_autocenter(dev, 0);
	}
>>>>>>> refs/remotes/origin/master

	/* Create sysfs interface */
	error = device_create_file(&hid->dev, &dev_attr_range);
	if (error)
		return error;
	dbg_hid("sysfs interface created\n");

	/* Set the maximum range to start with */
	entry->range = entry->max_range;
	if (entry->set_range != NULL)
		entry->set_range(hid, entry->range);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	hid_info(hid, "Force feedback for Logitech Speed Force Wireless by Simon Wood <simon@mungewell.org>\n");
	return 0;
}

<<<<<<< HEAD
=======
int lg4ff_deinit(struct hid_device *hid)
{
	bool found = 0;
	struct lg4ff_device_entry *entry;
	struct list_head *h, *g;
	list_for_each_safe(h, g, &device_list.list) {
		entry = list_entry(h, struct lg4ff_device_entry, list);
		if (strcmp(entry->device_id, (&hid->dev)->kobj.name) == 0) {
			list_del(h);
			kfree(entry->device_id);
			kfree(entry);
			found = 1;
			break;
		}
	}

	if (!found) {
		dbg_hid("Device entry not found!\n");
		return -1;
	}

	device_remove_file(&hid->dev, &dev_attr_range);
	dbg_hid("Device successfully unregistered\n");
	return 0;
}
>>>>>>> refs/remotes/origin/cm-10.0
=======

#ifdef CONFIG_LEDS_CLASS
	/* register led subsystem - G27 only */
	entry->led_state = 0;
	for (j = 0; j < 5; j++)
		entry->led[j] = NULL;

	if (lg4ff_devices[i].product_id == USB_DEVICE_ID_LOGITECH_G27_WHEEL) {
		struct led_classdev *led;
		size_t name_sz;
		char *name;

		lg4ff_set_leds(hid, 0);

		name_sz = strlen(dev_name(&hid->dev)) + 8;

		for (j = 0; j < 5; j++) {
			led = kzalloc(sizeof(struct led_classdev)+name_sz, GFP_KERNEL);
			if (!led) {
				hid_err(hid, "can't allocate memory for LED %d\n", j);
				goto err;
			}

			name = (void *)(&led[1]);
			snprintf(name, name_sz, "%s::RPM%d", dev_name(&hid->dev), j+1);
			led->name = name;
			led->brightness = 0;
			led->max_brightness = 1;
			led->brightness_get = lg4ff_led_get_brightness;
			led->brightness_set = lg4ff_led_set_brightness;

			entry->led[j] = led;
			error = led_classdev_register(&hid->dev, led);

			if (error) {
				hid_err(hid, "failed to register LED %d. Aborting.\n", j);
err:
				/* Deregister LEDs (if any) */
				for (j = 0; j < 5; j++) {
					led = entry->led[j];
					entry->led[j] = NULL;
					if (!led)
						continue;
					led_classdev_unregister(led);
					kfree(led);
				}
				goto out;	/* Let the driver continue without LEDs */
			}
		}
	}
out:
#endif
	hid_info(hid, "Force feedback support for Logitech Gaming Wheels\n");
	return 0;
}



int lg4ff_deinit(struct hid_device *hid)
{
	struct lg4ff_device_entry *entry;
	struct lg_drv_data *drv_data;

	device_remove_file(&hid->dev, &dev_attr_range);

	drv_data = hid_get_drvdata(hid);
	if (!drv_data) {
		hid_err(hid, "Error while deinitializing device, no private driver data.\n");
		return -1;
	}
	entry = drv_data->device_props;
	if (!entry) {
		hid_err(hid, "Error while deinitializing device, no device properties data.\n");
		return -1;
	}

#ifdef CONFIG_LEDS_CLASS
	{
		int j;
		struct led_classdev *led;

		/* Deregister LEDs (if any) */
		for (j = 0; j < 5; j++) {

			led = entry->led[j];
			entry->led[j] = NULL;
			if (!led)
				continue;
			led_classdev_unregister(led);
			kfree(led);
		}
	}
#endif

	/* Deallocate memory */
	kfree(entry);

	dbg_hid("Device successfully unregistered\n");
	return 0;
}
>>>>>>> refs/remotes/origin/master
