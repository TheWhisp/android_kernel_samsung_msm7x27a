/*
 *  quickstart.c - ACPI Direct App Launch driver
 *
 *
 *  Copyright (C) 2007-2010 Angelo Arrifano <miknix@gmail.com>
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *  Information gathered from disassebled dsdt and from here:
=======
 *  Information gathered from disassembled dsdt and from here:
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *  Information gathered from disassembled dsdt and from here:
>>>>>>> refs/remotes/origin/master
 *  <http://www.microsoft.com/whdc/system/platform/firmware/DirAppLaunch.mspx>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
#define QUICKSTART_VERSION "1.03"
=======
#define QUICKSTART_VERSION "1.04"

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define QUICKSTART_VERSION "1.04"

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
>>>>>>> refs/remotes/origin/master

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <acpi/acpi_drivers.h>
#include <linux/platform_device.h>
#include <linux/input.h>

MODULE_AUTHOR("Angelo Arrifano");
MODULE_DESCRIPTION("ACPI Direct App Launch driver");
MODULE_LICENSE("GPL");

<<<<<<< HEAD
<<<<<<< HEAD
#define QUICKSTART_ACPI_DEVICE_NAME   "quickstart"
#define QUICKSTART_ACPI_CLASS         "quickstart"
#define QUICKSTART_ACPI_HID           "PNP0C32"

#define QUICKSTART_PF_DRIVER_NAME     "quickstart"
#define QUICKSTART_PF_DEVICE_NAME     "quickstart"
#define QUICKSTART_PF_DEVATTR_NAME    "pressed_button"

#define QUICKSTART_MAX_BTN_NAME_LEN   16

/* There will be two events:
	 * 0x02 - A hot button was pressed while device was off/sleeping.
	 * 0x80 - A hot button was pressed while device was up. */
#define QUICKSTART_EVENT_WAKE         0x02
#define QUICKSTART_EVENT_RUNTIME      0x80

struct quickstart_btn {
	char *name;
	unsigned int id;
	struct quickstart_btn *next;
};

static struct quickstart_driver_data {
	struct quickstart_btn *btn_lst;
	struct quickstart_btn *pressed;
} quickstart_data;

/* ACPI driver Structs */
struct quickstart_acpi {
	struct acpi_device *device;
	struct quickstart_btn *btn;
};
static int quickstart_acpi_add(struct acpi_device *device);
static int quickstart_acpi_remove(struct acpi_device *device, int type);
static const struct acpi_device_id  quickstart_device_ids[] = {
	{QUICKSTART_ACPI_HID, 0},
	{"", 0},
};

static struct acpi_driver quickstart_acpi_driver = {
	.name = "quickstart",
	.class = QUICKSTART_ACPI_CLASS,
	.ids = quickstart_device_ids,
	.ops = {
			.add = quickstart_acpi_add,
			.remove = quickstart_acpi_remove,
		},
};

/* Input device structs */
struct input_dev *quickstart_input;

/* Platform driver structs */
static ssize_t buttons_show(struct device *dev,
					struct device_attribute *attr,
					char *buf);
static ssize_t pressed_button_show(struct device *dev,
					struct device_attribute *attr,
					char *buf);
static ssize_t pressed_button_store(struct device *dev,
					struct device_attribute *attr,
					 const char *buf,
					 size_t count);
static DEVICE_ATTR(pressed_button, 0666, pressed_button_show,
					 pressed_button_store);
static DEVICE_ATTR(buttons, 0444, buttons_show, NULL);
static struct platform_device *pf_device;
static struct platform_driver pf_driver = {
	.driver = {
		.name = QUICKSTART_PF_DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};

/*
 * Platform driver functions
 */
static ssize_t buttons_show(struct device *dev,
					 struct device_attribute *attr,
					 char *buf)
{
	int count = 0;
	struct quickstart_btn *ptr = quickstart_data.btn_lst;

	if (!ptr)
		return snprintf(buf, PAGE_SIZE, "none");

	while (ptr && (count < PAGE_SIZE)) {
		if (ptr->name) {
			count += snprintf(buf + count,
					PAGE_SIZE - count,
					"%d\t%s\n", ptr->id, ptr->name);
		}
		ptr = ptr->next;
=======
=======
>>>>>>> refs/remotes/origin/master
#define QUICKSTART_ACPI_DEVICE_NAME	"quickstart"
#define QUICKSTART_ACPI_CLASS		"quickstart"
#define QUICKSTART_ACPI_HID		"PNP0C32"

#define QUICKSTART_PF_DRIVER_NAME	"quickstart"
#define QUICKSTART_PF_DEVICE_NAME	"quickstart"

/*
 * There will be two events:
 * 0x02 - A hot button was pressed while device was off/sleeping.
 * 0x80 - A hot button was pressed while device was up.
 */
#define QUICKSTART_EVENT_WAKE		0x02
#define QUICKSTART_EVENT_RUNTIME	0x80

struct quickstart_button {
	char *name;
	unsigned int id;
	struct list_head list;
};

struct quickstart_acpi {
	struct acpi_device *device;
	struct quickstart_button *button;
};

static LIST_HEAD(buttons);
static struct quickstart_button *pressed;

static struct input_dev *quickstart_input;

/* Platform driver functions */
<<<<<<< HEAD
static ssize_t quickstart_buttons_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
=======
static ssize_t buttons_show(struct device *dev, struct device_attribute *attr,
			    char *buf)
>>>>>>> refs/remotes/origin/master
{
	int count = 0;
	struct quickstart_button *b;

	if (list_empty(&buttons))
		return snprintf(buf, PAGE_SIZE, "none");

	list_for_each_entry(b, &buttons, list) {
		count += snprintf(buf + count, PAGE_SIZE - count, "%u\t%s\n",
							b->id, b->name);

		if (count >= PAGE_SIZE) {
			count = PAGE_SIZE;
			break;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	return count;
}

<<<<<<< HEAD
<<<<<<< HEAD
static ssize_t pressed_button_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%s\n",
			(quickstart_data.pressed ?
			 quickstart_data.pressed->name : "none"));
}


static ssize_t pressed_button_store(struct device *dev,
					 struct device_attribute *attr,
					 const char *buf, size_t count)
=======
static ssize_t quickstart_pressed_button_show(struct device *dev,
						struct device_attribute *attr,
						char *buf)
=======
static ssize_t pressed_button_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/master
{
	return scnprintf(buf, PAGE_SIZE, "%s\n",
					(pressed ? pressed->name : "none"));
}


<<<<<<< HEAD
static ssize_t quickstart_pressed_button_store(struct device *dev,
						struct device_attribute *attr,
						const char *buf, size_t count)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static ssize_t pressed_button_store(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
>>>>>>> refs/remotes/origin/master
{
	if (count < 2)
		return -EINVAL;

	if (strncasecmp(buf, "none", 4) != 0)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	quickstart_data.pressed = NULL;
	return count;
}

/* Hotstart Helper functions */
static int quickstart_btnlst_add(struct quickstart_btn **data)
{
	struct quickstart_btn **ptr = &quickstart_data.btn_lst;

	while (*ptr)
		ptr = &((*ptr)->next);

	*ptr = kzalloc(sizeof(struct quickstart_btn), GFP_KERNEL);
	if (!*ptr) {
		*data = NULL;
		return -ENOMEM;
	}
	*data = *ptr;

	return 0;
}

static void quickstart_btnlst_del(struct quickstart_btn *data)
{
	struct quickstart_btn **ptr = &quickstart_data.btn_lst;

	if (!data)
		return;

	while (*ptr) {
		if (*ptr == data) {
			*ptr = (*ptr)->next;
			kfree(data);
			return;
		}
		ptr = &((*ptr)->next);
	}

	return;
}

static void quickstart_btnlst_free(void)
{
	struct quickstart_btn *ptr = quickstart_data.btn_lst;
	struct quickstart_btn *lptr = NULL;

	while (ptr) {
		lptr = ptr;
		ptr = ptr->next;
		kfree(lptr->name);
		kfree(lptr);
	}

	return;
=======
=======
>>>>>>> refs/remotes/origin/master
	pressed = NULL;
	return count;
}

/* Helper functions */
static struct quickstart_button *quickstart_buttons_add(void)
{
	struct quickstart_button *b;

	b = kzalloc(sizeof(*b), GFP_KERNEL);
	if (!b)
		return NULL;

	list_add_tail(&b->list, &buttons);

	return b;
}

static void quickstart_button_del(struct quickstart_button *data)
{
	if (!data)
		return;

	list_del(&data->list);
	kfree(data->name);
	kfree(data);
}

static void quickstart_buttons_free(void)
{
	struct quickstart_button *b, *n;

	list_for_each_entry_safe(b, n, &buttons, list)
		quickstart_button_del(b);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/* ACPI Driver functions */
static void quickstart_acpi_notify(acpi_handle handle, u32 event, void *data)
{
	struct quickstart_acpi *quickstart = data;

	if (!quickstart)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	if (event == QUICKSTART_EVENT_WAKE)
		quickstart_data.pressed = quickstart->btn;
	else if (event == QUICKSTART_EVENT_RUNTIME) {
		input_report_key(quickstart_input, quickstart->btn->id, 1);
		input_sync(quickstart_input);
		input_report_key(quickstart_input, quickstart->btn->id, 0);
		input_sync(quickstart_input);
	}
	return;
}

static void quickstart_acpi_ghid(struct quickstart_acpi *quickstart)
{
	acpi_status status;
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
	uint32_t usageid = 0;

	if (!quickstart)
		return;

	/* This returns a buffer telling the button usage ID,
	 * and triggers pending notify events (The ones before booting). */
	status = acpi_evaluate_object(quickstart->device->handle,
					"GHID", NULL, &buffer);
	if (ACPI_FAILURE(status) || !buffer.pointer) {
		printk(KERN_ERR "quickstart: %s GHID method failed.\n",
		       quickstart->btn->name);
		return;
	}

	if (buffer.length < 8)
		return;

	/* <<The GHID method can return a BYTE, WORD, or DWORD.
	 * The value must be encoded in little-endian byte
	 * order (least significant byte first).>> */
	usageid = *((uint32_t *)(buffer.pointer + (buffer.length - 8)));
	quickstart->btn->id = usageid;

	kfree(buffer.pointer);
}

static int quickstart_acpi_config(struct quickstart_acpi *quickstart, char *bid)
{
	int len = strlen(bid);
	int ret;

	/* Add button to list */
	ret = quickstart_btnlst_add(&quickstart->btn);
	if (ret)
		return ret;

	quickstart->btn->name = kzalloc(len + 1, GFP_KERNEL);
	if (!quickstart->btn->name) {
		quickstart_btnlst_free();
		return -ENOMEM;
	}
	strcpy(quickstart->btn->name, bid);
=======
=======
>>>>>>> refs/remotes/origin/master
	switch (event) {
	case QUICKSTART_EVENT_WAKE:
		pressed = quickstart->button;
		break;
	case QUICKSTART_EVENT_RUNTIME:
		input_report_key(quickstart_input, quickstart->button->id, 1);
		input_sync(quickstart_input);
		input_report_key(quickstart_input, quickstart->button->id, 0);
		input_sync(quickstart_input);
		break;
	default:
		pr_err("Unexpected ACPI event notify (%u)\n", event);
		break;
	}
}

static int quickstart_acpi_ghid(struct quickstart_acpi *quickstart)
{
	acpi_status status;
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
	int ret = 0;

	/*
	 * This returns a buffer telling the button usage ID,
	 * and triggers pending notify events (The ones before booting).
	 */
	status = acpi_evaluate_object(quickstart->device->handle, "GHID", NULL,
								&buffer);
	if (ACPI_FAILURE(status)) {
		pr_err("%s GHID method failed\n", quickstart->button->name);
		return -EINVAL;
	}

	/*
	 * <<The GHID method can return a BYTE, WORD, or DWORD.
	 * The value must be encoded in little-endian byte
	 * order (least significant byte first).>>
	 */
	switch (buffer.length) {
	case 1:
		quickstart->button->id = *(uint8_t *)buffer.pointer;
		break;
	case 2:
		quickstart->button->id = *(uint16_t *)buffer.pointer;
		break;
	case 4:
		quickstart->button->id = *(uint32_t *)buffer.pointer;
		break;
	case 8:
		quickstart->button->id = *(uint64_t *)buffer.pointer;
		break;
	default:
		pr_err("%s GHID method returned buffer of unexpected length %lu\n",
				quickstart->button->name,
				(unsigned long)buffer.length);
		ret = -EINVAL;
		break;
	}

	kfree(buffer.pointer);

	return ret;
}

static int quickstart_acpi_config(struct quickstart_acpi *quickstart)
{
	char *bid = acpi_device_bid(quickstart->device);
	char *name;

	name = kmalloc(strlen(bid) + 1, GFP_KERNEL);
	if (!name)
		return -ENOMEM;

	/* Add new button to list */
	quickstart->button = quickstart_buttons_add();
	if (!quickstart->button) {
		kfree(name);
		return -ENOMEM;
	}

	quickstart->button->name = name;
	strcpy(quickstart->button->name, bid);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int quickstart_acpi_add(struct acpi_device *device)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int ret = 0;
	acpi_status status = AE_OK;
	struct quickstart_acpi *quickstart = NULL;
=======
	int ret;
	acpi_status status;
	struct quickstart_acpi *quickstart;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int ret;
	acpi_status status;
	struct quickstart_acpi *quickstart;
>>>>>>> refs/remotes/origin/master

	if (!device)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	quickstart = kzalloc(sizeof(struct quickstart_acpi), GFP_KERNEL);
=======
	quickstart = kzalloc(sizeof(*quickstart), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	quickstart = kzalloc(sizeof(*quickstart), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!quickstart)
		return -ENOMEM;

	quickstart->device = device;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
	strcpy(acpi_device_name(device), QUICKSTART_ACPI_DEVICE_NAME);
	strcpy(acpi_device_class(device), QUICKSTART_ACPI_CLASS);
	device->driver_data = quickstart;

	/* Add button to list and initialize some stuff */
<<<<<<< HEAD
<<<<<<< HEAD
	ret = quickstart_acpi_config(quickstart, acpi_device_bid(device));
	if (ret)
		goto fail_config;

	status = acpi_install_notify_handler(device->handle,
						ACPI_ALL_NOTIFY,
						quickstart_acpi_notify,
						quickstart);
	if (ACPI_FAILURE(status)) {
		printk(KERN_ERR "quickstart: Notify handler install error\n");
=======
=======
>>>>>>> refs/remotes/origin/master
	ret = quickstart_acpi_config(quickstart);
	if (ret < 0)
		goto fail_config;

	status = acpi_install_notify_handler(device->handle, ACPI_ALL_NOTIFY,
						quickstart_acpi_notify,
						quickstart);
	if (ACPI_FAILURE(status)) {
		pr_err("Notify handler install error\n");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		ret = -ENODEV;
		goto fail_installnotify;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	quickstart_acpi_ghid(quickstart);

	return 0;

fail_installnotify:
	quickstart_btnlst_del(quickstart->btn);
=======
=======
>>>>>>> refs/remotes/origin/master
	ret = quickstart_acpi_ghid(quickstart);
	if (ret < 0)
		goto fail_ghid;

	return 0;

fail_ghid:
	acpi_remove_notify_handler(device->handle, ACPI_ALL_NOTIFY,
						quickstart_acpi_notify);

fail_installnotify:
	quickstart_button_del(quickstart->button);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

fail_config:

	kfree(quickstart);

	return ret;
}

<<<<<<< HEAD
static int quickstart_acpi_remove(struct acpi_device *device, int type)
{
<<<<<<< HEAD
	acpi_status status = 0;
	struct quickstart_acpi *quickstart = NULL;

	if (!device || !acpi_driver_data(device))
		return -EINVAL;

	quickstart = acpi_driver_data(device);

	status = acpi_remove_notify_handler(device->handle,
						 ACPI_ALL_NOTIFY,
					    quickstart_acpi_notify);
	if (ACPI_FAILURE(status))
		printk(KERN_ERR "quickstart: Error removing notify handler\n");

=======
=======
static int quickstart_acpi_remove(struct acpi_device *device)
{
>>>>>>> refs/remotes/origin/master
	acpi_status status;
	struct quickstart_acpi *quickstart;

	if (!device)
		return -EINVAL;

	quickstart = acpi_driver_data(device);
	if (!quickstart)
		return -EINVAL;

	status = acpi_remove_notify_handler(device->handle, ACPI_ALL_NOTIFY,
						quickstart_acpi_notify);
	if (ACPI_FAILURE(status))
		pr_err("Error removing notify handler\n");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	kfree(quickstart);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Module functions */

=======
/* Platform driver structs */
static DEVICE_ATTR(pressed_button, 0666, quickstart_pressed_button_show,
					 quickstart_pressed_button_store);
static DEVICE_ATTR(buttons, 0444, quickstart_buttons_show, NULL);
=======
/* Platform driver structs */
static DEVICE_ATTR_RW(pressed_button);
static DEVICE_ATTR_RO(buttons);
>>>>>>> refs/remotes/origin/master
static struct platform_device *pf_device;
static struct platform_driver pf_driver = {
	.driver = {
		.name = QUICKSTART_PF_DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};

static const struct acpi_device_id quickstart_device_ids[] = {
	{QUICKSTART_ACPI_HID, 0},
	{"", 0},
};

static struct acpi_driver quickstart_acpi_driver = {
	.name = "quickstart",
	.class = QUICKSTART_ACPI_CLASS,
	.ids = quickstart_device_ids,
	.ops = {
			.add = quickstart_acpi_add,
			.remove = quickstart_acpi_remove,
		},
};

/* Module functions */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void quickstart_exit(void)
{
	input_unregister_device(quickstart_input);

	device_remove_file(&pf_device->dev, &dev_attr_pressed_button);
	device_remove_file(&pf_device->dev, &dev_attr_buttons);

	platform_device_unregister(pf_device);

	platform_driver_unregister(&pf_driver);

	acpi_bus_unregister_driver(&quickstart_acpi_driver);

<<<<<<< HEAD
<<<<<<< HEAD
	quickstart_btnlst_free();

	return;
=======
	quickstart_buttons_free();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	quickstart_buttons_free();
>>>>>>> refs/remotes/origin/master
}

static int __init quickstart_init_input(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct quickstart_btn **ptr = &quickstart_data.btn_lst;
	int count;
=======
	struct quickstart_button *b;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct quickstart_button *b;
>>>>>>> refs/remotes/origin/master
	int ret;

	quickstart_input = input_allocate_device();

	if (!quickstart_input)
		return -ENOMEM;

	quickstart_input->name = "Quickstart ACPI Buttons";
	quickstart_input->id.bustype = BUS_HOST;

<<<<<<< HEAD
<<<<<<< HEAD
	while (*ptr) {
		count++;
		set_bit(EV_KEY, quickstart_input->evbit);
		set_bit((*ptr)->id, quickstart_input->keybit);
		ptr = &((*ptr)->next);
=======
	list_for_each_entry(b, &buttons, list) {
		set_bit(EV_KEY, quickstart_input->evbit);
		set_bit(b->id, quickstart_input->keybit);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	list_for_each_entry(b, &buttons, list) {
		set_bit(EV_KEY, quickstart_input->evbit);
		set_bit(b->id, quickstart_input->keybit);
>>>>>>> refs/remotes/origin/master
	}

	ret = input_register_device(quickstart_input);
	if (ret) {
		input_free_device(quickstart_input);
		return ret;
	}

	return 0;
}

static int __init quickstart_init(void)
{
	int ret;

<<<<<<< HEAD
	/* ACPI Check */
	if (acpi_disabled)
		return -ENODEV;

=======
>>>>>>> refs/remotes/origin/master
	/* ACPI driver register */
	ret = acpi_bus_register_driver(&quickstart_acpi_driver);
	if (ret)
		return ret;

	/* If existing bus with no devices */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!quickstart_data.btn_lst) {
=======
	if (list_empty(&buttons)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (list_empty(&buttons)) {
>>>>>>> refs/remotes/origin/master
		ret = -ENODEV;
		goto fail_pfdrv_reg;
	}

	/* Platform driver register */
	ret = platform_driver_register(&pf_driver);
	if (ret)
		goto fail_pfdrv_reg;

	/* Platform device register */
	pf_device = platform_device_alloc(QUICKSTART_PF_DEVICE_NAME, -1);
	if (!pf_device) {
		ret = -ENOMEM;
		goto fail_pfdev_alloc;
	}
	ret = platform_device_add(pf_device);
	if (ret)
		goto fail_pfdev_add;

	/* Create device sysfs file */
	ret = device_create_file(&pf_device->dev, &dev_attr_pressed_button);
	if (ret)
		goto fail_dev_file;

	ret = device_create_file(&pf_device->dev, &dev_attr_buttons);
	if (ret)
		goto fail_dev_file2;

<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/* Input device */
	ret = quickstart_init_input();
	if (ret)
		goto fail_input;

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "quickstart: ACPI Direct App Launch ver %s\n",
						QUICKSTART_VERSION);
=======
	pr_info("ACPI Direct App Launch ver %s\n", QUICKSTART_VERSION);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_info("ACPI Direct App Launch ver %s\n", QUICKSTART_VERSION);
>>>>>>> refs/remotes/origin/master

	return 0;
fail_input:
	device_remove_file(&pf_device->dev, &dev_attr_buttons);

fail_dev_file2:
	device_remove_file(&pf_device->dev, &dev_attr_pressed_button);

fail_dev_file:
	platform_device_del(pf_device);

fail_pfdev_add:
	platform_device_put(pf_device);

fail_pfdev_alloc:
	platform_driver_unregister(&pf_driver);

fail_pfdrv_reg:
	acpi_bus_unregister_driver(&quickstart_acpi_driver);

	return ret;
}

module_init(quickstart_init);
module_exit(quickstart_exit);
