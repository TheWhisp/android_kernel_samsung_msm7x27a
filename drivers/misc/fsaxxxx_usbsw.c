/*
 * fsa_microusb.c - FSA9480, FSA88x micro USB switch device driver
 *
 * Copyright (C) 2011 Samsung Electronics
 *
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/fsaxxxx_usbsw.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio.h>

/* FSA9480 I2C registers */
#define FSA9480_REG_DEVID		0x01
#define FSA9480_REG_CTRL		0x02
#define FSA9480_REG_INT1		0x03
#define FSA9480_REG_INT2		0x04
#define FSA9480_REG_INT1_MASK	0x05
#define FSA9480_REG_INT2_MASK	0x06
#define FSA9480_REG_ADC			0x07
#define FSA9480_REG_TIMING1		0x08
#define FSA9480_REG_TIMING2		0x09
#define FSA9480_REG_DEV_T1		0x0a
#define FSA9480_REG_DEV_T2		0x0b
#define FSA9480_REG_BTN1		0x0c
#define FSA9480_REG_BTN2		0x0d
#define FSA9480_REG_CK			0x0e
#define FSA9480_REG_CK_INT1		0x0f
#define FSA9480_REG_CK_INT2		0x10
#define FSA9480_REG_CK_INTMASK1	0x11
#define FSA9480_REG_CK_INTMASK2	0x12
#define FSA9480_REG_MANSW1		0x13
#define FSA9480_REG_MANSW2		0x14
#define FSA9480_REG_RESERVED	        0x1d

/* FSA88x I2C registers */
#define FSA880_REG_DEVID		FSA9480_REG_DEVID
#define FSA880_REG_CTRL			FSA9480_REG_CTRL
#define FSA880_REG_INT1			FSA9480_REG_INT1
#define FSA880_REG_ADC			FSA9480_REG_ADC
#define FSA880_REG_DEV_T1		FSA9480_REG_DEV_T1
#define FSA880_REG_DEV_T2		FSA9480_REG_DEV_T2
#define FSA880_REG_MANSW1		FSA9480_REG_MANSW1
#define FSA880_REG_MANSW2		FSA9480_REG_MANSW2
#define FSA880_REG_RESET		0x1b

/* FSAxxx I2C registers */
#define FSA_REG_DEVID		FSA9480_REG_DEVID
#define FSA_REG_CTRL		FSA9480_REG_CTRL
#define FSA_REG_INT1		FSA9480_REG_INT1
#define FSA_REG_INT2		FSA9480_REG_INT2
#define FSA_REG_ADC		FSA9480_REG_ADC
#define FSA_REG_DEV_T1		FSA9480_REG_DEV_T1
#define FSA_REG_DEV_T2		FSA9480_REG_DEV_T2
#define FSA_REG_MANSW1		FSA9480_REG_MANSW1
#define FSA_REG_MANSW2		FSA9480_REG_MANSW2
#define FSA_REG_RESET		FSA880_REG_RESET
#define FSA_REG_RESERVED	FSA9480_REG_RESERVED

/* MANSW1 */
#define VAUDIO	0x90
#define UART	0x6c
#define AUDIO	0x48
#define DHOST	0x24
#define AUTO	0x0

#define FSA880_REG_MANSW1_MASK	0x6c

/* Control */
#define SWITCH_OPEN		(1 << 4)
#define RAW_DATA		(1 << 3)
#define MANUAL_SWITCH	(1 << 2)
#define WAIT			(1 << 1)
#define INT_MASK		(1 << 0)
#define CTRL_MASK		(SWITCH_OPEN | RAW_DATA | MANUAL_SWITCH | \
		WAIT | INT_MASK)
#define FSA880_REG_DEV_CTRL_MASK	(SWITCH_OPEN | MANUAL_SWITCH | \
		INT_MASK)

/* Device Type 1 */
#define DEV_USB_OTG			(1 << 7)
#define DEV_DEDICATED_CHG	(1 << 6)
#define DEV_USB_CHG			(1 << 5)
#define DEV_CAR_KIT			(1 << 4)
#define DEV_UART			(1 << 3)
#define DEV_USB				(1 << 2)
#define DEV_AUDIO_2			(1 << 1)
#define DEV_AUDIO_1			(1 << 0)

#define FSA9480_DEV_T1_USB_MASK		(DEV_USB_OTG | DEV_USB)
#define FSA9480_DEV_T1_UART_MASK	(DEV_UART)
#define FSA9480_DEV_T1_CHARGER_MASK	(DEV_DEDICATED_CHG | DEV_USB_CHG |\
					DEV_CAR_KIT)
#define FSA880_REG_DEV_T1_MASK	(DEV_DEDICATED_CHG | DEV_USB_CHG | \
		DEV_CAR_KIT | DEV_USB)

#define FSA_DEV_T1_USB_MASK		FSA9480_DEV_T1_USB_MASK
#define FSA_DEV_T1_CHARGER_MASK	FSA9480_DEV_T1_CHARGER_MASK

/* Device Type 2 */
#define DEV_UNKNOWN			(1 << 7)
#define DEV_AV				(1 << 6)
#define DEV_TTY				(1 << 5)
#define DEV_PPD				(1 << 4)
#define DEV_JIG_UART_OFF	(1 << 3)
#define DEV_JIG_UART_ON		(1 << 2)
#define DEV_JIG_USB_OFF		(1 << 1)
#define DEV_JIG_USB_ON		(1 << 0)

#define FSA9480_DEV_T2_USB_MASK		(DEV_JIG_USB_OFF | DEV_JIG_USB_ON)
#define FSA9480_DEV_T2_UART_MASK	(DEV_JIG_UART_OFF | DEV_JIG_UART_ON)
#define FSA9480_DEV_T2_JIG_MASK		(DEV_JIG_USB_OFF | DEV_JIG_USB_ON | \
		DEV_JIG_UART_OFF | DEV_JIG_UART_ON)
#define FSA880_REG_DEV_T2_MASK		(DEV_UNKNOWN | DEV_JIG_UART_OFF | \
		DEV_JIG_UART_ON | DEV_JIG_USB_OFF | DEV_JIG_USB_ON)

#define FSA_DEV_T2_USB_MASK		FSA9480_DEV_T2_USB_MASK
#define FSA_DEV_T2_UART_MASK	FSA9480_DEV_T2_UART_MASK
#define FSA_DEV_T2_JIG_MASK		FSA9480_DEV_T2_JIG_MASK

/* RESERVED REG 0x1D */
#define FSA_RESERVED_VBUS_MASK		0x02

/* Device ID */
#define FSA9480_ID	0
#define FSA9280_ID	1
#define FSA880_ID	2

struct fsausb_info {
	struct i2c_client *client;
	struct fsausb_platform_data *pdata;
	struct work_struct work;
	int dev1;
	int dev2;
	int mansw;
	unsigned int id;
	const char *name;
};

static struct fsausb_info *chip;
static struct fsausb_ops ops;
static int charger_cb_flag;
static int ovp_cb_flag;
u8 jig_with_vbus;

void fsa_default_detach_handler(void)
{
	return;
};

void fsa_default_attach_handler(int d)
{
	return;
};

int fsa_microusb_connection_handler_register(
		void (*detach), void (*attach))
{
	if (detach)
		ops.detach_handler = detach;
	if (attach)
		ops.attach_handler = attach;

	if (!(detach || attach))
		return -EINVAL;

	return 0;
}

static int fsausb_write_reg(struct i2c_client *client, u8 reg, u8 data)
{
	int ret;
	u8 buf[2];
	struct i2c_msg msg[] = {
		{
			.addr = client->addr,
			.flags = 0,
			.len = 2,
			.buf = buf,
		},
	};

	buf[0] = reg;
	buf[1] = data;

	ret = i2c_transfer(client->adapter, msg, 1);
	if (ret != 1) {
		dev_err(&client->dev,
				"[%s] Failed (ret=%d)\n", __func__, ret);
		return -EREMOTEIO;
	}

	pr_debug("[%s] success\n", __func__);
	return 0;
}

static int fsausb_read_reg(struct i2c_client *client, u8 reg, u8 *data)
{
	int ret;
	struct i2c_msg msg[] = {
		{
			.addr = client->addr,
			.flags = 0,
			.len = 1,
			.buf = &reg,
		},
		{
			.addr = client->addr,
			.flags = I2C_M_RD,
			.len = 1,
			.buf = data,
		}
	};

	ret = i2c_transfer(client->adapter, msg, 2);
	if (ret != 2) {
		dev_err(&client->dev,
				"[%s] Failed (ret=%d)\n", __func__, ret);
		return -EREMOTEIO;
	}

	pr_debug("[%s] success\n", __func__);
	return 0;
}

void fsausb_set_switch(const char *buf)
{
	struct fsausb_info *usbsw = chip;
	struct i2c_client *client = usbsw->client;
	unsigned int path;
	u8 value;

	fsausb_read_reg(client, FSA_REG_CTRL, &value);

	if (!strncmp(buf, "VAUDIO", 6)) {
		path = VAUDIO;
		value &= ~MANUAL_SWITCH;
	} else if (!strncmp(buf, "UART", 4)) {
		path = UART;
		value &= ~MANUAL_SWITCH;
	} else if (!strncmp(buf, "AUDIO", 5)) {
		path = AUDIO;
		value &= ~MANUAL_SWITCH;
	} else if (!strncmp(buf, "DHOST", 5)) {
		path = DHOST;
		value &= ~MANUAL_SWITCH;
	} else if (!strncmp(buf, "AUTO", 4)) {
		path = AUTO;
		value |= MANUAL_SWITCH;
	} else {
		dev_err(&client->dev, "Wrong command\n");
		return;
	}

	if (usbsw->id == FSA880_ID)
		path &= FSA880_REG_MANSW1_MASK;

	usbsw->mansw = path;
	fsausb_write_reg(client, FSA_REG_MANSW1, path);
	fsausb_write_reg(client, FSA_REG_CTRL, value);
}
EXPORT_SYMBOL_GPL(fsausb_set_switch);

ssize_t fsausb_get_switch(char *buf)
{
	struct fsausb_info *usbsw = chip;
	struct i2c_client *client = usbsw->client;
	u8 value;

	fsausb_read_reg(client, FSA_REG_MANSW1, &value);

	if (usbsw->id == FSA880_ID)
		value &= FSA880_REG_MANSW1_MASK;

	if (value == VAUDIO)
		return sprintf(buf, "VAUDIO\n");
	else if (value == UART)
		return sprintf(buf, "UART\n");
	else if (value == AUDIO)
		return sprintf(buf, "AUDIO\n");
	else if (value == DHOST)
		return sprintf(buf, "DHOST\n");
	else if (value == AUTO)
		return sprintf(buf, "AUTO\n");
	else
		return sprintf(buf, "%x", value);
}
EXPORT_SYMBOL_GPL(fsausb_get_switch);

static ssize_t fsausb_show_status(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct fsausb_info *usbsw = dev_get_drvdata(dev);
	struct i2c_client *client = usbsw->client;
	u8 devid, ctrl, adc, dev1, dev2, intr;
	u8 intmask1, intmask2, time1, time2, mansw1;

	fsausb_read_reg(client, FSA_REG_DEVID, &devid);
	fsausb_read_reg(client, FSA_REG_CTRL, &ctrl);
	fsausb_read_reg(client, FSA_REG_ADC, &adc);
	fsausb_read_reg(client, FSA_REG_DEV_T1, &dev1);
	fsausb_read_reg(client, FSA_REG_DEV_T2, &dev2);
	fsausb_read_reg(client, FSA_REG_MANSW1, &mansw1);

	if (usbsw->id != FSA880_ID) {
		fsausb_read_reg(client, FSA9480_REG_INT1_MASK, &intmask1);
		fsausb_read_reg(client, FSA9480_REG_INT2_MASK, &intmask2);
		fsausb_read_reg(client, FSA9480_REG_TIMING1, &time1);
		fsausb_read_reg(client, FSA9480_REG_TIMING2, &time2);
	}

	fsausb_read_reg(client, FSA_REG_INT1, &intr);
	intr &= 0xffff;

	return sprintf(buf, "%s: Device ID(%02x), CTRL(%02x)\n"
			"ADC(%02x), DEV_T1(%02x), DEV_T2(%02x)\n"
			"INT(%04x), INTMASK(%02x, %02x)\n"
			"TIMING(%02x, %02x), MANSW1(%02x)\n", usbsw->name,
			devid, ctrl, adc, dev1, dev2, intr,
			intmask1, intmask2, time1, time2, mansw1);
}

static ssize_t fsausb_show_manualsw(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return fsausb_get_switch(buf);
}

static ssize_t fsausb_set_manualsw(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count)
{
	fsausb_set_switch(buf);
	return count;
}

static DEVICE_ATTR(status, S_IRUGO, fsausb_show_status, NULL);
static DEVICE_ATTR(switch, 0644,
		fsausb_show_manualsw, fsausb_set_manualsw);

static struct attribute *fsausb_attributes[] = {
	&dev_attr_status.attr,
	&dev_attr_switch.attr,
	NULL
};

static const struct attribute_group fsausb_group = {
	.attrs = fsausb_attributes,
};

static irqreturn_t fsausb_irq_handler(int irq, void *data)
{
	struct fsausb_info *usbsw = data;
	if (!work_pending(&usbsw->work)) {
		disable_irq_nosync(irq);
		schedule_work(&usbsw->work);
	}
	return IRQ_HANDLED;
}

static void fsausb_detect_dev(struct fsausb_info *usbsw, u8 intr)
{
	u8 val1, val2, ctrl, val_vbus;
	struct fsausb_platform_data *pdata = usbsw->pdata;
	struct i2c_client *client = usbsw->client;

	fsausb_read_reg(client, FSA_REG_DEV_T1, &val1);
	fsausb_read_reg(client, FSA_REG_DEV_T2, &val2);
	fsausb_read_reg(client, FSA_REG_CTRL, &ctrl);
	fsausb_read_reg(client, FSA_REG_RESERVED, &val_vbus);

	dev_info(&client->dev, "intr: 0x%x, dev1: 0x%x, dev2: 0x%x\n",
			intr, val1, val2);

	if (usbsw->id == FSA880_ID) {
		val1 &= FSA880_REG_DEV_T1_MASK;
		val2 &= FSA880_REG_DEV_T2_MASK;
		ctrl &= FSA880_REG_DEV_CTRL_MASK;
	}
	val_vbus &= FSA_RESERVED_VBUS_MASK;

	/* Attached */
	if (intr & (1 << 0)) {
		if (val1 & FSA_DEV_T1_USB_MASK ||
				val2 & FSA_DEV_T2_USB_MASK) {
			if (pdata->usb_cb)
				pdata->usb_cb(FSA_ATTACHED, &ops);
			if (usbsw->mansw)
				fsausb_write_reg(client, FSA_REG_MANSW1,
					   usbsw->mansw);
			charger_cb_flag = 1;
		}
		if (val1 & FSA9480_DEV_T1_UART_MASK ||
				val2 & FSA_DEV_T2_UART_MASK) {
			if (pdata->uart_cb)
				pdata->uart_cb(FSA_ATTACHED, &ops);
			if (!(ctrl & (1 << 2)))
				fsausb_write_reg(client,
						FSA_REG_MANSW1, UART);
		}
		if (val1 & FSA_DEV_T1_CHARGER_MASK) {
			if (pdata->charger_cb)
				pdata->charger_cb(FSA_ATTACHED, &ops);
			charger_cb_flag = 1;
		}
		if (val2 & FSA_DEV_T2_JIG_MASK) {
			if (pdata->jig_cb) {
				if (val_vbus == FSA_RESERVED_VBUS_MASK) {
					pdata->charger_cb(FSA_ATTACHED, &ops);
					pr_err("[fsa] JIG ATTACHED with VBUS DETECTED\n");
					jig_with_vbus  = 1;
				} else
					pdata->jig_cb(FSA_ATTACHED, &ops);
			}
		}
		if (val1 == 0 && val2 == 0) {
			if (usbsw->dev1 & FSA_DEV_T1_USB_MASK ||
					usbsw->dev2 & FSA_DEV_T2_USB_MASK) {
				if (pdata->usb_cb) {
					/*detach event of USB*/
					pdata->usb_cb(FSA_DETACHED, &ops);
						dev_info(&client->dev, "USB detached[WORKAROUND]\n");
					}
				}
			if (usbsw->dev1 & FSA_DEV_T1_CHARGER_MASK) {
				if (pdata->charger_cb) {
					/*detach event of CHARGER*/
					pdata->charger_cb(FSA_DETACHED, &ops);
					dev_info(&client->dev, "charger detached[WORKAROUND]\n");
					}
			}
			/* reset USB switch */
			fsausb_write_reg(client, FSA_REG_CTRL, 0x1E);
		} else if (intr & (1 << 5)) {
			if (pdata->ovp_cb)
				pdata->ovp_cb(FSA_ATTACHED, &ops);
			pr_info("[BATT] OVP occur connect charger!%d\n",
					ovp_cb_flag);
			ovp_cb_flag = 1;
		}
	} else if (intr & (1 << 1)) {
		if (usbsw->dev1 & FSA_DEV_T1_USB_MASK ||
				usbsw->dev2 & FSA_DEV_T2_USB_MASK) {
			if (pdata->usb_cb)
				pdata->usb_cb(FSA_DETACHED, &ops);
			charger_cb_flag = 0;
		}
		if (usbsw->dev1 & FSA9480_DEV_T1_UART_MASK ||
				usbsw->dev2 & FSA_DEV_T2_UART_MASK) {
			if (pdata->uart_cb)
				pdata->uart_cb(FSA_DETACHED, &ops);
		}
		if (usbsw->dev1 & FSA_DEV_T1_CHARGER_MASK) {
			if (pdata->charger_cb)
				pdata->charger_cb(FSA_DETACHED, &ops);
			charger_cb_flag = 0;
		}
		if (usbsw->dev2 & FSA_DEV_T2_JIG_MASK) {
			if (pdata->jig_cb) {
				if (jig_with_vbus == 1) {
					pdata->charger_cb(FSA_DETACHED, &ops);
					pr_err("[fsa] JIG DETACHED with VBUS DETECTED\n");
					jig_with_vbus = 0;
				} else
					pdata->jig_cb(FSA_DETACHED, &ops);
			}
		}
		if (usbsw->dev1 == 0x00 && usbsw->dev2 == 0x00) {
			if (pdata->charger_cb)
				pdata->charger_cb(FSA_DETACHED, &ops);
				charger_cb_flag = 0;
			fsausb_write_reg(client, FSA_REG_CTRL, 0x1E);
		}
		if ((intr & (1 << 7)) && ovp_cb_flag) {
			pr_info("[BATT] OVP is not!%d\n", ovp_cb_flag);
			if (pdata->ovp_cb)
				pdata->ovp_cb(FSA_DETACHED, &ops);
			ovp_cb_flag = 0;
		}
	} else if (intr == 0) {
		if (val1 == 0 && val2 == 0) {
			if (usbsw->dev1 & FSA_DEV_T1_USB_MASK ||
					usbsw->dev2 & FSA_DEV_T2_USB_MASK) {
				if (pdata->usb_cb) {
					/*detach event of USB*/
					pdata->usb_cb(FSA_DETACHED, &ops);
						dev_info(&client->dev, "USB detached[WORKAROUND]\n");
					}
				}
			if (usbsw->dev1 & FSA_DEV_T1_CHARGER_MASK) {
				if (pdata->charger_cb) {
					/*detach event of CHARGER*/
					pdata->charger_cb(FSA_DETACHED, &ops);
					dev_info(&client->dev, "charger detached[WORKAROUND]\n");
					}
			}
			/* reset USB switch */
			fsausb_write_reg(client, FSA_REG_CTRL, 0x1E);
		}
	} else if (intr & (1 << 5)) {
		if (pdata->ovp_cb)
			pdata->ovp_cb(FSA_ATTACHED, &ops);
		pr_info("[BATT] OVP is occur!%d\n", ovp_cb_flag);
		ovp_cb_flag = 1;
	} else if ((intr & (1 << 7)) && ovp_cb_flag) {
		pr_info("[BATT] OVP is not!%d\n", ovp_cb_flag);
		if (pdata->ovp_cb)
			pdata->ovp_cb(FSA_DETACHED, &ops);
		ovp_cb_flag = 0;
	}

	usbsw->dev1 = val1;
	usbsw->dev2 = val2;
	chip->dev1 = val1;
	chip->dev2 = val2;
	ctrl &= ~INT_MASK;

	fsausb_write_reg(client, FSA_REG_CTRL, ctrl);
}

static void fsausb_work_cb(struct work_struct *work)
{
	struct fsausb_info *usbsw =
		container_of(work, struct fsausb_info, work);
	struct i2c_client *client = usbsw->client;

	struct fsausb_platform_data *pdata = usbsw->pdata;
	u8 intr, intr2;

	/* clear interrupt */
/*	if (usbsw->id != FSA880_ID)
		fsausb_read_reg(client, FSA9480_REG_INT2, &intr);
*/
	fsausb_read_reg(client, FSA_REG_INT1, &intr);
	fsausb_read_reg(client, FSA_REG_INT2, &intr2);

	dev_info(&client->dev, "%s: intr: 0x%x intr2: 0x%x\n",
					__func__, intr, intr2);
	intr &= 0xff;

	/* device detection */
	fsausb_detect_dev(usbsw, intr);
	enable_irq(client->irq);
}

static int fsausb_irq_init(struct fsausb_info *usbsw)
{
	struct i2c_client *client = usbsw->client;
	unsigned int ctrl = CTRL_MASK;
	int ret, irq = -1;
	u8 intr;
	u8 mansw1;

	/* clear interrupt */
	if (usbsw->id != FSA880_ID)
		fsausb_read_reg(client, FSA9480_REG_INT2, &intr);
	fsausb_read_reg(client, FSA_REG_INT1, &intr);

	if (usbsw->id != FSA880_ID) {
		/* unmask interrupt (attach/detach only) */
		ret = fsausb_write_reg(client, FSA9480_REG_INT1_MASK, 0x00);
		if (ret < 0)
			return ret;

		ret = fsausb_write_reg(client, FSA9480_REG_INT2_MASK, 0x00);
		if (ret < 0)
			return ret;
	}

	fsausb_read_reg(client, FSA_REG_MANSW1, &mansw1);
	usbsw->mansw = mansw1;
	ctrl &= ~INT_MASK;              /* Unmask Interrupt */

	/* XXX: should check android spec */
	if (usbsw->mansw)
		ctrl &= ~MANUAL_SWITCH; /* Manual Switching Mode */
	fsausb_write_reg(client, FSA_REG_CTRL, ctrl);
	dev_info(&client->dev, "[%s] set ctrl=0x%02x\n", __func__, ctrl);

	INIT_WORK(&usbsw->work, fsausb_work_cb);

	switch (usbsw->id) {
	case FSA9480_ID:
		ret = request_irq(client->irq, fsausb_irq_handler,
				IRQF_TRIGGER_LOW | IRQF_DISABLED,
				"fsa9480 micro USB", usbsw);
		break;
	case FSA9280_ID:
		ret = request_irq(client->irq, fsausb_irq_handler,
				IRQF_TRIGGER_LOW | IRQF_DISABLED,
				"fsa9280 micro USB", usbsw);
		break;
	case FSA880_ID:
		ret = request_irq(client->irq, fsausb_irq_handler,
				IRQF_TRIGGER_LOW | IRQF_DISABLED,
				"fsa880 micro USB", usbsw);
		break;
	}

	if (ret) {
		dev_err(&client->dev,
				"%s micro USB: Unable to get IRQ %d\n",
				usbsw->name, irq);
		goto out;
	}

	return 0;

out:
	return ret;
}

static int __devinit fsausb_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	struct fsausb_info *usbsw;
	int ret = 0;

	dev_info(&client->dev, "[FSAxxxx micro usb] PROBE ...\n");

	usbsw = kzalloc(sizeof(struct fsausb_info), GFP_KERNEL);
	if (!usbsw) {
		dev_err(&client->dev, "failed to allocate driver data\n");
		return -ENOMEM;
	}
	usbsw->client = client;
	usbsw->pdata = client->dev.platform_data;

	chip = usbsw;
	ops.detach_handler = fsa_default_detach_handler;
	ops.attach_handler = fsa_default_attach_handler;
	charger_cb_flag = 0;

	if (id->driver_data > FSA880_ID)
		return -EINVAL;

	usbsw->name = id->name;
	usbsw->id = id->driver_data;

	i2c_set_clientdata(client, usbsw);

	ret = fsausb_irq_init(usbsw);
	if (ret)
		goto fsausb_probe_fail;

	ret = sysfs_create_group(&client->dev.kobj, &fsausb_group);
	if (ret) {
		dev_err(&client->dev,
				"Creating fsa attribute group failed");
		goto fsausb_probe_fail2;
	}
	dev_info(&client->dev, "[%s] %d\n", __func__, __LINE__);

	if (usbsw->id != FSA880_ID)
		fsausb_write_reg(client, FSA9480_REG_TIMING1, 0x6);

	if (chip->pdata->reset_cb)
		chip->pdata->reset_cb();

	/* device detection */
	fsausb_detect_dev(usbsw, 1);

	dev_info(&client->dev, "[%s] PROBE Done.\n", usbsw->name);

	return 0;

fsausb_probe_fail2:
	if (client->irq)
		free_irq(client->irq, NULL);

fsausb_probe_fail:
	i2c_set_clientdata(client, NULL);
	kfree(usbsw);
	return ret;
}

static int __devexit fsausb_remove(struct i2c_client *client)
{
	struct fsausb_info *usbsw = i2c_get_clientdata(client);

	if (client->irq)
		free_irq(client->irq, NULL);

	i2c_set_clientdata(client, NULL);
	sysfs_remove_group(&client->dev.kobj, &fsausb_group);
	kfree(usbsw);

	return 0;
}

#ifdef CONFIG_PM
static int fsausb_resume(struct i2c_client *client)
{
	struct fsausb_info *usbsw = i2c_get_clientdata(client);
	u8 intr;
	u8 val1, val2;

	/* for hibernation */
	fsausb_read_reg(client, FSA_REG_DEV_T1, &val1);
	fsausb_read_reg(client, FSA_REG_DEV_T2, &val2);

	if (val1 || val2)
		intr = 1 << 0;
	else
		intr = 1 << 1;

	/* device detection */
	fsausb_detect_dev(usbsw, intr);
	return 0;
}
#else
#define fsausb_resume         NULL
#endif

static const struct i2c_device_id fsausb_id[] = {
	{"FSA9480", FSA9480_ID},
	{"FSA9280", FSA9280_ID},
	{"FSA880", FSA880_ID},
	{}
};
MODULE_DEVICE_TABLE(i2c, fsausb_id);

static struct i2c_driver fsausb_i2c_driver = {
	.driver = {
		.name = "fsa_microusb",
	},
	.probe = fsausb_probe,
	.remove = __devexit_p(fsausb_remove),
	.resume = fsausb_resume,
	.id_table = fsausb_id,
};

static int __init fsausb_init(void)
{
	return i2c_add_driver(&fsausb_i2c_driver);
}

static void __exit fsausb_exit(void)
{
	i2c_del_driver(&fsausb_i2c_driver);
}

module_init(fsausb_init);
module_exit(fsausb_exit);

MODULE_AUTHOR("handohee");
MODULE_DESCRIPTION("FSAxxxx USB Switch driver");
MODULE_LICENSE("GPL");
