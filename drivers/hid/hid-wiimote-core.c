/*
<<<<<<< HEAD
 * HID driver for Nintendo Wiimote devices
 * Copyright (c) 2011 David Herrmann
=======
 * HID driver for Nintendo Wii / Wii U peripherals
 * Copyright (c) 2011-2013 David Herrmann <dh.herrmann@gmail.com>
>>>>>>> refs/remotes/origin/master
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */

#include <linux/completion.h>
#include <linux/device.h>
#include <linux/hid.h>
#include <linux/input.h>
<<<<<<< HEAD
#include <linux/leds.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/power_supply.h>
=======
#include <linux/module.h>
#include <linux/mutex.h>
>>>>>>> refs/remotes/origin/master
#include <linux/spinlock.h>
#include "hid-ids.h"
#include "hid-wiimote.h"

<<<<<<< HEAD
enum wiiproto_keys {
	WIIPROTO_KEY_LEFT,
	WIIPROTO_KEY_RIGHT,
	WIIPROTO_KEY_UP,
	WIIPROTO_KEY_DOWN,
	WIIPROTO_KEY_PLUS,
	WIIPROTO_KEY_MINUS,
	WIIPROTO_KEY_ONE,
	WIIPROTO_KEY_TWO,
	WIIPROTO_KEY_A,
	WIIPROTO_KEY_B,
	WIIPROTO_KEY_HOME,
	WIIPROTO_KEY_COUNT
};

static __u16 wiiproto_keymap[] = {
	KEY_LEFT,	/* WIIPROTO_KEY_LEFT */
	KEY_RIGHT,	/* WIIPROTO_KEY_RIGHT */
	KEY_UP,		/* WIIPROTO_KEY_UP */
	KEY_DOWN,	/* WIIPROTO_KEY_DOWN */
	KEY_NEXT,	/* WIIPROTO_KEY_PLUS */
	KEY_PREVIOUS,	/* WIIPROTO_KEY_MINUS */
	BTN_1,		/* WIIPROTO_KEY_ONE */
	BTN_2,		/* WIIPROTO_KEY_TWO */
	BTN_A,		/* WIIPROTO_KEY_A */
	BTN_B,		/* WIIPROTO_KEY_B */
	BTN_MODE,	/* WIIPROTO_KEY_HOME */
};

static enum power_supply_property wiimote_battery_props[] = {
	POWER_SUPPLY_PROP_CAPACITY,
	POWER_SUPPLY_PROP_SCOPE,
};

static ssize_t wiimote_hid_send(struct hid_device *hdev, __u8 *buffer,
								size_t count)
{
	__u8 *buf;
	ssize_t ret;
=======
/* output queue handling */

static int wiimote_hid_send(struct hid_device *hdev, __u8 *buffer,
			    size_t count)
{
	__u8 *buf;
	int ret;
>>>>>>> refs/remotes/origin/master

	if (!hdev->hid_output_raw_report)
		return -ENODEV;

	buf = kmemdup(buffer, count, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	ret = hdev->hid_output_raw_report(hdev, buf, count, HID_OUTPUT_REPORT);

	kfree(buf);
	return ret;
}

<<<<<<< HEAD
static void wiimote_worker(struct work_struct *work)
{
	struct wiimote_data *wdata = container_of(work, struct wiimote_data,
									worker);
	unsigned long flags;

	spin_lock_irqsave(&wdata->qlock, flags);

	while (wdata->head != wdata->tail) {
		spin_unlock_irqrestore(&wdata->qlock, flags);
		wiimote_hid_send(wdata->hdev, wdata->outq[wdata->tail].data,
						wdata->outq[wdata->tail].size);
		spin_lock_irqsave(&wdata->qlock, flags);

		wdata->tail = (wdata->tail + 1) % WIIMOTE_BUFSIZE;
	}

	spin_unlock_irqrestore(&wdata->qlock, flags);
=======
static void wiimote_queue_worker(struct work_struct *work)
{
	struct wiimote_queue *queue = container_of(work, struct wiimote_queue,
						   worker);
	struct wiimote_data *wdata = container_of(queue, struct wiimote_data,
						  queue);
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&wdata->queue.lock, flags);

	while (wdata->queue.head != wdata->queue.tail) {
		spin_unlock_irqrestore(&wdata->queue.lock, flags);
		ret = wiimote_hid_send(wdata->hdev,
				 wdata->queue.outq[wdata->queue.tail].data,
				 wdata->queue.outq[wdata->queue.tail].size);
		if (ret < 0) {
			spin_lock_irqsave(&wdata->state.lock, flags);
			wiimote_cmd_abort(wdata);
			spin_unlock_irqrestore(&wdata->state.lock, flags);
		}
		spin_lock_irqsave(&wdata->queue.lock, flags);

		wdata->queue.tail = (wdata->queue.tail + 1) % WIIMOTE_BUFSIZE;
	}

	spin_unlock_irqrestore(&wdata->queue.lock, flags);
>>>>>>> refs/remotes/origin/master
}

static void wiimote_queue(struct wiimote_data *wdata, const __u8 *buffer,
								size_t count)
{
	unsigned long flags;
	__u8 newhead;

	if (count > HID_MAX_BUFFER_SIZE) {
		hid_warn(wdata->hdev, "Sending too large output report\n");
<<<<<<< HEAD
		return;
=======

		spin_lock_irqsave(&wdata->queue.lock, flags);
		goto out_error;
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Copy new request into our output queue and check whether the
	 * queue is full. If it is full, discard this request.
	 * If it is empty we need to start a new worker that will
	 * send out the buffer to the hid device.
	 * If the queue is not empty, then there must be a worker
	 * that is currently sending out our buffer and this worker
	 * will reschedule itself until the queue is empty.
	 */

<<<<<<< HEAD
	spin_lock_irqsave(&wdata->qlock, flags);

	memcpy(wdata->outq[wdata->head].data, buffer, count);
	wdata->outq[wdata->head].size = count;
	newhead = (wdata->head + 1) % WIIMOTE_BUFSIZE;

	if (wdata->head == wdata->tail) {
		wdata->head = newhead;
		schedule_work(&wdata->worker);
	} else if (newhead != wdata->tail) {
		wdata->head = newhead;
	} else {
		hid_warn(wdata->hdev, "Output queue is full");
	}

	spin_unlock_irqrestore(&wdata->qlock, flags);
=======
	spin_lock_irqsave(&wdata->queue.lock, flags);

	memcpy(wdata->queue.outq[wdata->queue.head].data, buffer, count);
	wdata->queue.outq[wdata->queue.head].size = count;
	newhead = (wdata->queue.head + 1) % WIIMOTE_BUFSIZE;

	if (wdata->queue.head == wdata->queue.tail) {
		wdata->queue.head = newhead;
		schedule_work(&wdata->queue.worker);
	} else if (newhead != wdata->queue.tail) {
		wdata->queue.head = newhead;
	} else {
		hid_warn(wdata->hdev, "Output queue is full");
		goto out_error;
	}

	goto out_unlock;

out_error:
	wiimote_cmd_abort(wdata);
out_unlock:
	spin_unlock_irqrestore(&wdata->queue.lock, flags);
>>>>>>> refs/remotes/origin/master
}

/*
 * This sets the rumble bit on the given output report if rumble is
 * currently enabled.
 * \cmd1 must point to the second byte in the output report => &cmd[1]
 * This must be called on nearly every output report before passing it
 * into the output queue!
 */
static inline void wiiproto_keep_rumble(struct wiimote_data *wdata, __u8 *cmd1)
{
	if (wdata->state.flags & WIIPROTO_FLAG_RUMBLE)
		*cmd1 |= 0x01;
}

<<<<<<< HEAD
static void wiiproto_req_rumble(struct wiimote_data *wdata, __u8 rumble)
=======
void wiiproto_req_rumble(struct wiimote_data *wdata, __u8 rumble)
>>>>>>> refs/remotes/origin/master
{
	__u8 cmd[2];

	rumble = !!rumble;
	if (rumble == !!(wdata->state.flags & WIIPROTO_FLAG_RUMBLE))
		return;

	if (rumble)
		wdata->state.flags |= WIIPROTO_FLAG_RUMBLE;
	else
		wdata->state.flags &= ~WIIPROTO_FLAG_RUMBLE;

	cmd[0] = WIIPROTO_REQ_RUMBLE;
	cmd[1] = 0;

	wiiproto_keep_rumble(wdata, &cmd[1]);
	wiimote_queue(wdata, cmd, sizeof(cmd));
}

<<<<<<< HEAD
static void wiiproto_req_leds(struct wiimote_data *wdata, int leds)
=======
void wiiproto_req_leds(struct wiimote_data *wdata, int leds)
>>>>>>> refs/remotes/origin/master
{
	__u8 cmd[2];

	leds &= WIIPROTO_FLAGS_LEDS;
	if ((wdata->state.flags & WIIPROTO_FLAGS_LEDS) == leds)
		return;
	wdata->state.flags = (wdata->state.flags & ~WIIPROTO_FLAGS_LEDS) | leds;

	cmd[0] = WIIPROTO_REQ_LED;
	cmd[1] = 0;

	if (leds & WIIPROTO_FLAG_LED1)
		cmd[1] |= 0x10;
	if (leds & WIIPROTO_FLAG_LED2)
		cmd[1] |= 0x20;
	if (leds & WIIPROTO_FLAG_LED3)
		cmd[1] |= 0x40;
	if (leds & WIIPROTO_FLAG_LED4)
		cmd[1] |= 0x80;

	wiiproto_keep_rumble(wdata, &cmd[1]);
	wiimote_queue(wdata, cmd, sizeof(cmd));
}

/*
 * Check what peripherals of the wiimote are currently
 * active and select a proper DRM that supports all of
 * the requested data inputs.
<<<<<<< HEAD
=======
 *
 * Not all combinations are actually supported. The following
 * combinations work only with limitations:
 *  - IR cam in extended or full mode disables any data transmission
 *    of extension controllers. There is no DRM mode that supports
 *    extension bytes plus extended/full IR.
 *  - IR cam with accelerometer and extension *_EXT8 is not supported.
 *    However, all extensions that need *_EXT8 are devices that don't
 *    support IR cameras. Hence, this shouldn't happen under normal
 *    operation.
 *  - *_EXT16 is only supported in combination with buttons and
 *    accelerometer. No IR or similar can be active simultaneously. As
 *    above, all modules that require it are mutually exclusive with
 *    IR/etc. so this doesn't matter.
>>>>>>> refs/remotes/origin/master
 */
static __u8 select_drm(struct wiimote_data *wdata)
{
	__u8 ir = wdata->state.flags & WIIPROTO_FLAGS_IR;
<<<<<<< HEAD
	bool ext = wiiext_active(wdata);

	if (ir == WIIPROTO_FLAG_IR_BASIC) {
		if (wdata->state.flags & WIIPROTO_FLAG_ACCEL)
			return WIIPROTO_REQ_DRM_KAIE;
		else
			return WIIPROTO_REQ_DRM_KIE;
=======
	bool ext;

	ext = (wdata->state.flags & WIIPROTO_FLAG_EXT_USED) ||
	      (wdata->state.flags & WIIPROTO_FLAG_MP_USED);

	/* some 3rd-party balance-boards are hard-coded to KEE, *sigh* */
	if (wdata->state.devtype == WIIMOTE_DEV_BALANCE_BOARD) {
		if (ext)
			return WIIPROTO_REQ_DRM_KEE;
		else
			return WIIPROTO_REQ_DRM_K;
	}

	if (ir == WIIPROTO_FLAG_IR_BASIC) {
		if (wdata->state.flags & WIIPROTO_FLAG_ACCEL) {
			/* GEN10 and ealier devices bind IR formats to DRMs.
			 * Hence, we cannot use DRM_KAI here as it might be
			 * bound to IR_EXT. Use DRM_KAIE unconditionally so we
			 * work with all devices and our parsers can use the
			 * fixed formats, too. */
			return WIIPROTO_REQ_DRM_KAIE;
		} else {
			return WIIPROTO_REQ_DRM_KIE;
		}
>>>>>>> refs/remotes/origin/master
	} else if (ir == WIIPROTO_FLAG_IR_EXT) {
		return WIIPROTO_REQ_DRM_KAI;
	} else if (ir == WIIPROTO_FLAG_IR_FULL) {
		return WIIPROTO_REQ_DRM_SKAI1;
	} else {
		if (wdata->state.flags & WIIPROTO_FLAG_ACCEL) {
			if (ext)
				return WIIPROTO_REQ_DRM_KAE;
			else
				return WIIPROTO_REQ_DRM_KA;
		} else {
			if (ext)
<<<<<<< HEAD
				return WIIPROTO_REQ_DRM_KE;
=======
				return WIIPROTO_REQ_DRM_KEE;
>>>>>>> refs/remotes/origin/master
			else
				return WIIPROTO_REQ_DRM_K;
		}
	}
}

void wiiproto_req_drm(struct wiimote_data *wdata, __u8 drm)
{
	__u8 cmd[3];

<<<<<<< HEAD
	if (drm == WIIPROTO_REQ_NULL)
=======
	if (wdata->state.flags & WIIPROTO_FLAG_DRM_LOCKED)
		drm = wdata->state.drm;
	else if (drm == WIIPROTO_REQ_NULL)
>>>>>>> refs/remotes/origin/master
		drm = select_drm(wdata);

	cmd[0] = WIIPROTO_REQ_DRM;
	cmd[1] = 0;
	cmd[2] = drm;

	wdata->state.drm = drm;
	wiiproto_keep_rumble(wdata, &cmd[1]);
	wiimote_queue(wdata, cmd, sizeof(cmd));
}

<<<<<<< HEAD
static void wiiproto_req_status(struct wiimote_data *wdata)
=======
void wiiproto_req_status(struct wiimote_data *wdata)
>>>>>>> refs/remotes/origin/master
{
	__u8 cmd[2];

	cmd[0] = WIIPROTO_REQ_SREQ;
	cmd[1] = 0;

	wiiproto_keep_rumble(wdata, &cmd[1]);
	wiimote_queue(wdata, cmd, sizeof(cmd));
}

<<<<<<< HEAD
static void wiiproto_req_accel(struct wiimote_data *wdata, __u8 accel)
=======
void wiiproto_req_accel(struct wiimote_data *wdata, __u8 accel)
>>>>>>> refs/remotes/origin/master
{
	accel = !!accel;
	if (accel == !!(wdata->state.flags & WIIPROTO_FLAG_ACCEL))
		return;

	if (accel)
		wdata->state.flags |= WIIPROTO_FLAG_ACCEL;
	else
		wdata->state.flags &= ~WIIPROTO_FLAG_ACCEL;

	wiiproto_req_drm(wdata, WIIPROTO_REQ_NULL);
}

<<<<<<< HEAD
static void wiiproto_req_ir1(struct wiimote_data *wdata, __u8 flags)
=======
void wiiproto_req_ir1(struct wiimote_data *wdata, __u8 flags)
>>>>>>> refs/remotes/origin/master
{
	__u8 cmd[2];

	cmd[0] = WIIPROTO_REQ_IR1;
	cmd[1] = flags;

	wiiproto_keep_rumble(wdata, &cmd[1]);
	wiimote_queue(wdata, cmd, sizeof(cmd));
}

<<<<<<< HEAD
static void wiiproto_req_ir2(struct wiimote_data *wdata, __u8 flags)
=======
void wiiproto_req_ir2(struct wiimote_data *wdata, __u8 flags)
>>>>>>> refs/remotes/origin/master
{
	__u8 cmd[2];

	cmd[0] = WIIPROTO_REQ_IR2;
	cmd[1] = flags;

	wiiproto_keep_rumble(wdata, &cmd[1]);
	wiimote_queue(wdata, cmd, sizeof(cmd));
}

#define wiiproto_req_wreg(wdata, os, buf, sz) \
			wiiproto_req_wmem((wdata), false, (os), (buf), (sz))

#define wiiproto_req_weeprom(wdata, os, buf, sz) \
			wiiproto_req_wmem((wdata), true, (os), (buf), (sz))

static void wiiproto_req_wmem(struct wiimote_data *wdata, bool eeprom,
				__u32 offset, const __u8 *buf, __u8 size)
{
	__u8 cmd[22];

	if (size > 16 || size == 0) {
		hid_warn(wdata->hdev, "Invalid length %d wmem request\n", size);
		return;
	}

	memset(cmd, 0, sizeof(cmd));
	cmd[0] = WIIPROTO_REQ_WMEM;
	cmd[2] = (offset >> 16) & 0xff;
	cmd[3] = (offset >> 8) & 0xff;
	cmd[4] = offset & 0xff;
	cmd[5] = size;
	memcpy(&cmd[6], buf, size);

	if (!eeprom)
		cmd[1] |= 0x04;

	wiiproto_keep_rumble(wdata, &cmd[1]);
	wiimote_queue(wdata, cmd, sizeof(cmd));
}

void wiiproto_req_rmem(struct wiimote_data *wdata, bool eeprom, __u32 offset,
								__u16 size)
{
	__u8 cmd[7];

	if (size == 0) {
		hid_warn(wdata->hdev, "Invalid length %d rmem request\n", size);
		return;
	}

	cmd[0] = WIIPROTO_REQ_RMEM;
	cmd[1] = 0;
	cmd[2] = (offset >> 16) & 0xff;
	cmd[3] = (offset >> 8) & 0xff;
	cmd[4] = offset & 0xff;
	cmd[5] = (size >> 8) & 0xff;
	cmd[6] = size & 0xff;

	if (!eeprom)
		cmd[1] |= 0x04;

	wiiproto_keep_rumble(wdata, &cmd[1]);
	wiimote_queue(wdata, cmd, sizeof(cmd));
}

/* requries the cmd-mutex to be held */
int wiimote_cmd_write(struct wiimote_data *wdata, __u32 offset,
						const __u8 *wmem, __u8 size)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&wdata->state.lock, flags);
	wiimote_cmd_set(wdata, WIIPROTO_REQ_WMEM, 0);
	wiiproto_req_wreg(wdata, offset, wmem, size);
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	ret = wiimote_cmd_wait(wdata);
	if (!ret && wdata->state.cmd_err)
		ret = -EIO;

	return ret;
}

/* requries the cmd-mutex to be held */
ssize_t wiimote_cmd_read(struct wiimote_data *wdata, __u32 offset, __u8 *rmem,
								__u8 size)
{
	unsigned long flags;
	ssize_t ret;

	spin_lock_irqsave(&wdata->state.lock, flags);
	wdata->state.cmd_read_size = size;
	wdata->state.cmd_read_buf = rmem;
	wiimote_cmd_set(wdata, WIIPROTO_REQ_RMEM, offset & 0xffff);
	wiiproto_req_rreg(wdata, offset, size);
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	ret = wiimote_cmd_wait(wdata);

	spin_lock_irqsave(&wdata->state.lock, flags);
	wdata->state.cmd_read_buf = NULL;
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	if (!ret) {
		if (wdata->state.cmd_read_size == 0)
			ret = -EIO;
		else
			ret = wdata->state.cmd_read_size;
	}

	return ret;
}

<<<<<<< HEAD
static int wiimote_battery_get_property(struct power_supply *psy,
						enum power_supply_property psp,
						union power_supply_propval *val)
{
	struct wiimote_data *wdata = container_of(psy,
						struct wiimote_data, battery);
	int ret = 0, state;
	unsigned long flags;

	if (psp == POWER_SUPPLY_PROP_SCOPE) {
		val->intval = POWER_SUPPLY_SCOPE_DEVICE;
		return 0;
	}

	ret = wiimote_cmd_acquire(wdata);
	if (ret)
		return ret;

	spin_lock_irqsave(&wdata->state.lock, flags);
	wiimote_cmd_set(wdata, WIIPROTO_REQ_SREQ, 0);
	wiiproto_req_status(wdata);
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	ret = wiimote_cmd_wait(wdata);
	state = wdata->state.cmd_battery;
	wiimote_cmd_release(wdata);

	if (ret)
		return ret;

	switch (psp) {
		case POWER_SUPPLY_PROP_CAPACITY:
			val->intval = state * 100 / 255;
			break;
		default:
			ret = -EINVAL;
			break;
	}

	return ret;
}

static int wiimote_init_ir(struct wiimote_data *wdata, __u16 mode)
{
	int ret;
	unsigned long flags;
	__u8 format = 0;
	static const __u8 data_enable[] = { 0x01 };
	static const __u8 data_sens1[] = { 0x02, 0x00, 0x00, 0x71, 0x01,
						0x00, 0xaa, 0x00, 0x64 };
	static const __u8 data_sens2[] = { 0x63, 0x03 };
	static const __u8 data_fin[] = { 0x08 };

	spin_lock_irqsave(&wdata->state.lock, flags);

	if (mode == (wdata->state.flags & WIIPROTO_FLAGS_IR)) {
		spin_unlock_irqrestore(&wdata->state.lock, flags);
		return 0;
	}

	if (mode == 0) {
		wdata->state.flags &= ~WIIPROTO_FLAGS_IR;
		wiiproto_req_ir1(wdata, 0);
		wiiproto_req_ir2(wdata, 0);
		wiiproto_req_drm(wdata, WIIPROTO_REQ_NULL);
		spin_unlock_irqrestore(&wdata->state.lock, flags);
		return 0;
	}

	spin_unlock_irqrestore(&wdata->state.lock, flags);

	ret = wiimote_cmd_acquire(wdata);
	if (ret)
		return ret;

	/* send PIXEL CLOCK ENABLE cmd first */
	spin_lock_irqsave(&wdata->state.lock, flags);
	wiimote_cmd_set(wdata, WIIPROTO_REQ_IR1, 0);
	wiiproto_req_ir1(wdata, 0x06);
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	ret = wiimote_cmd_wait(wdata);
	if (ret)
		goto unlock;
	if (wdata->state.cmd_err) {
		ret = -EIO;
		goto unlock;
	}

	/* enable IR LOGIC */
	spin_lock_irqsave(&wdata->state.lock, flags);
	wiimote_cmd_set(wdata, WIIPROTO_REQ_IR2, 0);
	wiiproto_req_ir2(wdata, 0x06);
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	ret = wiimote_cmd_wait(wdata);
	if (ret)
		goto unlock;
	if (wdata->state.cmd_err) {
		ret = -EIO;
		goto unlock;
	}

	/* enable IR cam but do not make it send data, yet */
	ret = wiimote_cmd_write(wdata, 0xb00030, data_enable,
							sizeof(data_enable));
	if (ret)
		goto unlock;

	/* write first sensitivity block */
	ret = wiimote_cmd_write(wdata, 0xb00000, data_sens1,
							sizeof(data_sens1));
	if (ret)
		goto unlock;

	/* write second sensitivity block */
	ret = wiimote_cmd_write(wdata, 0xb0001a, data_sens2,
							sizeof(data_sens2));
	if (ret)
		goto unlock;

	/* put IR cam into desired state */
	switch (mode) {
		case WIIPROTO_FLAG_IR_FULL:
			format = 5;
			break;
		case WIIPROTO_FLAG_IR_EXT:
			format = 3;
			break;
		case WIIPROTO_FLAG_IR_BASIC:
			format = 1;
			break;
	}
	ret = wiimote_cmd_write(wdata, 0xb00033, &format, sizeof(format));
	if (ret)
		goto unlock;

	/* make IR cam send data */
	ret = wiimote_cmd_write(wdata, 0xb00030, data_fin, sizeof(data_fin));
	if (ret)
		goto unlock;

	/* request new DRM mode compatible to IR mode */
	spin_lock_irqsave(&wdata->state.lock, flags);
	wdata->state.flags &= ~WIIPROTO_FLAGS_IR;
	wdata->state.flags |= mode & WIIPROTO_FLAGS_IR;
	wiiproto_req_drm(wdata, WIIPROTO_REQ_NULL);
	spin_unlock_irqrestore(&wdata->state.lock, flags);

unlock:
	wiimote_cmd_release(wdata);
	return ret;
}

static enum led_brightness wiimote_leds_get(struct led_classdev *led_dev)
{
	struct wiimote_data *wdata;
	struct device *dev = led_dev->dev->parent;
	int i;
	unsigned long flags;
	bool value = false;

	wdata = hid_get_drvdata(container_of(dev, struct hid_device, dev));

	for (i = 0; i < 4; ++i) {
		if (wdata->leds[i] == led_dev) {
			spin_lock_irqsave(&wdata->state.lock, flags);
			value = wdata->state.flags & WIIPROTO_FLAG_LED(i + 1);
			spin_unlock_irqrestore(&wdata->state.lock, flags);
			break;
		}
	}

	return value ? LED_FULL : LED_OFF;
}

static void wiimote_leds_set(struct led_classdev *led_dev,
						enum led_brightness value)
{
	struct wiimote_data *wdata;
	struct device *dev = led_dev->dev->parent;
	int i;
	unsigned long flags;
	__u8 state, flag;

	wdata = hid_get_drvdata(container_of(dev, struct hid_device, dev));

	for (i = 0; i < 4; ++i) {
		if (wdata->leds[i] == led_dev) {
			flag = WIIPROTO_FLAG_LED(i + 1);
			spin_lock_irqsave(&wdata->state.lock, flags);
			state = wdata->state.flags;
			if (value == LED_OFF)
				wiiproto_req_leds(wdata, state & ~flag);
			else
				wiiproto_req_leds(wdata, state | flag);
			spin_unlock_irqrestore(&wdata->state.lock, flags);
			break;
		}
	}
}

static int wiimote_ff_play(struct input_dev *dev, void *data,
							struct ff_effect *eff)
{
	struct wiimote_data *wdata = input_get_drvdata(dev);
	__u8 value;
	unsigned long flags;

	/*
	 * The wiimote supports only a single rumble motor so if any magnitude
	 * is set to non-zero then we start the rumble motor. If both are set to
	 * zero, we stop the rumble motor.
	 */

	if (eff->u.rumble.strong_magnitude || eff->u.rumble.weak_magnitude)
		value = 1;
	else
		value = 0;

	spin_lock_irqsave(&wdata->state.lock, flags);
	wiiproto_req_rumble(wdata, value);
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	return 0;
}

static int wiimote_input_open(struct input_dev *dev)
{
	struct wiimote_data *wdata = input_get_drvdata(dev);

	return hid_hw_open(wdata->hdev);
}

static void wiimote_input_close(struct input_dev *dev)
{
	struct wiimote_data *wdata = input_get_drvdata(dev);

	hid_hw_close(wdata->hdev);
}

static int wiimote_accel_open(struct input_dev *dev)
{
	struct wiimote_data *wdata = input_get_drvdata(dev);
	int ret;
	unsigned long flags;

	ret = hid_hw_open(wdata->hdev);
	if (ret)
		return ret;

	spin_lock_irqsave(&wdata->state.lock, flags);
	wiiproto_req_accel(wdata, true);
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	return 0;
}

static void wiimote_accel_close(struct input_dev *dev)
{
	struct wiimote_data *wdata = input_get_drvdata(dev);
	unsigned long flags;

	spin_lock_irqsave(&wdata->state.lock, flags);
	wiiproto_req_accel(wdata, false);
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	hid_hw_close(wdata->hdev);
}

static int wiimote_ir_open(struct input_dev *dev)
{
	struct wiimote_data *wdata = input_get_drvdata(dev);
	int ret;

	ret = hid_hw_open(wdata->hdev);
	if (ret)
		return ret;

	ret = wiimote_init_ir(wdata, WIIPROTO_FLAG_IR_BASIC);
	if (ret) {
		hid_hw_close(wdata->hdev);
		return ret;
	}

	return 0;
}

static void wiimote_ir_close(struct input_dev *dev)
{
	struct wiimote_data *wdata = input_get_drvdata(dev);

	wiimote_init_ir(wdata, 0);
	hid_hw_close(wdata->hdev);
}

static void handler_keys(struct wiimote_data *wdata, const __u8 *payload)
{
	input_report_key(wdata->input, wiiproto_keymap[WIIPROTO_KEY_LEFT],
							!!(payload[0] & 0x01));
	input_report_key(wdata->input, wiiproto_keymap[WIIPROTO_KEY_RIGHT],
							!!(payload[0] & 0x02));
	input_report_key(wdata->input, wiiproto_keymap[WIIPROTO_KEY_DOWN],
							!!(payload[0] & 0x04));
	input_report_key(wdata->input, wiiproto_keymap[WIIPROTO_KEY_UP],
							!!(payload[0] & 0x08));
	input_report_key(wdata->input, wiiproto_keymap[WIIPROTO_KEY_PLUS],
							!!(payload[0] & 0x10));
	input_report_key(wdata->input, wiiproto_keymap[WIIPROTO_KEY_TWO],
							!!(payload[1] & 0x01));
	input_report_key(wdata->input, wiiproto_keymap[WIIPROTO_KEY_ONE],
							!!(payload[1] & 0x02));
	input_report_key(wdata->input, wiiproto_keymap[WIIPROTO_KEY_B],
							!!(payload[1] & 0x04));
	input_report_key(wdata->input, wiiproto_keymap[WIIPROTO_KEY_A],
							!!(payload[1] & 0x08));
	input_report_key(wdata->input, wiiproto_keymap[WIIPROTO_KEY_MINUS],
							!!(payload[1] & 0x10));
	input_report_key(wdata->input, wiiproto_keymap[WIIPROTO_KEY_HOME],
							!!(payload[1] & 0x80));
	input_sync(wdata->input);
}

static void handler_accel(struct wiimote_data *wdata, const __u8 *payload)
{
	__u16 x, y, z;

	if (!(wdata->state.flags & WIIPROTO_FLAG_ACCEL))
		return;

	/*
	 * payload is: BB BB XX YY ZZ
	 * Accelerometer data is encoded into 3 10bit values. XX, YY and ZZ
	 * contain the upper 8 bits of each value. The lower 2 bits are
	 * contained in the buttons data BB BB.
	 * Bits 6 and 7 of the first buttons byte BB is the lower 2 bits of the
	 * X accel value. Bit 5 of the second buttons byte is the 2nd bit of Y
	 * accel value and bit 6 is the second bit of the Z value.
	 * The first bit of Y and Z values is not available and always set to 0.
	 * 0x200 is returned on no movement.
	 */

	x = payload[2] << 2;
	y = payload[3] << 2;
	z = payload[4] << 2;

	x |= (payload[0] >> 5) & 0x3;
	y |= (payload[1] >> 4) & 0x2;
	z |= (payload[1] >> 5) & 0x2;

	input_report_abs(wdata->accel, ABS_RX, x - 0x200);
	input_report_abs(wdata->accel, ABS_RY, y - 0x200);
	input_report_abs(wdata->accel, ABS_RZ, z - 0x200);
	input_sync(wdata->accel);
}

#define ir_to_input0(wdata, ir, packed) __ir_to_input((wdata), (ir), (packed), \
							ABS_HAT0X, ABS_HAT0Y)
#define ir_to_input1(wdata, ir, packed) __ir_to_input((wdata), (ir), (packed), \
							ABS_HAT1X, ABS_HAT1Y)
#define ir_to_input2(wdata, ir, packed) __ir_to_input((wdata), (ir), (packed), \
							ABS_HAT2X, ABS_HAT2Y)
#define ir_to_input3(wdata, ir, packed) __ir_to_input((wdata), (ir), (packed), \
							ABS_HAT3X, ABS_HAT3Y)

static void __ir_to_input(struct wiimote_data *wdata, const __u8 *ir,
						bool packed, __u8 xid, __u8 yid)
{
	__u16 x, y;

	if (!(wdata->state.flags & WIIPROTO_FLAGS_IR))
		return;

	/*
	 * Basic IR data is encoded into 3 bytes. The first two bytes are the
	 * upper 8 bit of the X/Y data, the 3rd byte contains the lower 2 bits
	 * of both.
	 * If data is packed, then the 3rd byte is put first and slightly
	 * reordered. This allows to interleave packed and non-packed data to
	 * have two IR sets in 5 bytes instead of 6.
	 * The resulting 10bit X/Y values are passed to the ABS_HATXY input dev.
	 */

	if (packed) {
		x = ir[1] << 2;
		y = ir[2] << 2;

		x |= ir[0] & 0x3;
		y |= (ir[0] >> 2) & 0x3;
	} else {
		x = ir[0] << 2;
		y = ir[1] << 2;

		x |= (ir[2] >> 4) & 0x3;
		y |= (ir[2] >> 6) & 0x3;
	}

	input_report_abs(wdata->ir, xid, x);
	input_report_abs(wdata->ir, yid, y);
}

static void handler_status(struct wiimote_data *wdata, const __u8 *payload)
=======
/* requires the cmd-mutex to be held */
static int wiimote_cmd_init_ext(struct wiimote_data *wdata)
{
	__u8 wmem;
	int ret;

	/* initialize extension */
	wmem = 0x55;
	ret = wiimote_cmd_write(wdata, 0xa400f0, &wmem, sizeof(wmem));
	if (ret)
		return ret;

	/* disable default encryption */
	wmem = 0x0;
	ret = wiimote_cmd_write(wdata, 0xa400fb, &wmem, sizeof(wmem));
	if (ret)
		return ret;

	return 0;
}

/* requires the cmd-mutex to be held */
static __u8 wiimote_cmd_read_ext(struct wiimote_data *wdata, __u8 *rmem)
{
	int ret;

	/* read extension ID */
	ret = wiimote_cmd_read(wdata, 0xa400fa, rmem, 6);
	if (ret != 6)
		return WIIMOTE_EXT_NONE;

	hid_dbg(wdata->hdev, "extension ID: %6phC\n", rmem);

	if (rmem[0] == 0xff && rmem[1] == 0xff && rmem[2] == 0xff &&
	    rmem[3] == 0xff && rmem[4] == 0xff && rmem[5] == 0xff)
		return WIIMOTE_EXT_NONE;

	if (rmem[4] == 0x00 && rmem[5] == 0x00)
		return WIIMOTE_EXT_NUNCHUK;
	if (rmem[4] == 0x01 && rmem[5] == 0x01)
		return WIIMOTE_EXT_CLASSIC_CONTROLLER;
	if (rmem[4] == 0x04 && rmem[5] == 0x02)
		return WIIMOTE_EXT_BALANCE_BOARD;
	if (rmem[4] == 0x01 && rmem[5] == 0x20)
		return WIIMOTE_EXT_PRO_CONTROLLER;

	return WIIMOTE_EXT_UNKNOWN;
}

/* requires the cmd-mutex to be held */
static int wiimote_cmd_init_mp(struct wiimote_data *wdata)
{
	__u8 wmem;
	int ret;

	/* initialize MP */
	wmem = 0x55;
	ret = wiimote_cmd_write(wdata, 0xa600f0, &wmem, sizeof(wmem));
	if (ret)
		return ret;

	/* disable default encryption */
	wmem = 0x0;
	ret = wiimote_cmd_write(wdata, 0xa600fb, &wmem, sizeof(wmem));
	if (ret)
		return ret;

	return 0;
}

/* requires the cmd-mutex to be held */
static bool wiimote_cmd_map_mp(struct wiimote_data *wdata, __u8 exttype)
{
	__u8 wmem;

	/* map MP with correct pass-through mode */
	switch (exttype) {
	case WIIMOTE_EXT_CLASSIC_CONTROLLER:
		wmem = 0x07;
		break;
	case WIIMOTE_EXT_NUNCHUK:
		wmem = 0x05;
		break;
	default:
		wmem = 0x04;
		break;
	}

	return wiimote_cmd_write(wdata, 0xa600fe, &wmem, sizeof(wmem));
}

/* requires the cmd-mutex to be held */
static bool wiimote_cmd_read_mp(struct wiimote_data *wdata, __u8 *rmem)
{
	int ret;

	/* read motion plus ID */
	ret = wiimote_cmd_read(wdata, 0xa600fa, rmem, 6);
	if (ret != 6)
		return false;

	hid_dbg(wdata->hdev, "motion plus ID: %6phC\n", rmem);

	if (rmem[5] == 0x05)
		return true;

	hid_info(wdata->hdev, "unknown motion plus ID: %6phC\n", rmem);

	return false;
}

/* requires the cmd-mutex to be held */
static __u8 wiimote_cmd_read_mp_mapped(struct wiimote_data *wdata)
{
	int ret;
	__u8 rmem[6];

	/* read motion plus ID */
	ret = wiimote_cmd_read(wdata, 0xa400fa, rmem, 6);
	if (ret != 6)
		return WIIMOTE_MP_NONE;

	hid_dbg(wdata->hdev, "mapped motion plus ID: %6phC\n", rmem);

	if (rmem[0] == 0xff && rmem[1] == 0xff && rmem[2] == 0xff &&
	    rmem[3] == 0xff && rmem[4] == 0xff && rmem[5] == 0xff)
		return WIIMOTE_MP_NONE;

	if (rmem[4] == 0x04 && rmem[5] == 0x05)
		return WIIMOTE_MP_SINGLE;
	else if (rmem[4] == 0x05 && rmem[5] == 0x05)
		return WIIMOTE_MP_PASSTHROUGH_NUNCHUK;
	else if (rmem[4] == 0x07 && rmem[5] == 0x05)
		return WIIMOTE_MP_PASSTHROUGH_CLASSIC;

	return WIIMOTE_MP_UNKNOWN;
}

/* device module handling */

static const __u8 * const wiimote_devtype_mods[WIIMOTE_DEV_NUM] = {
	[WIIMOTE_DEV_PENDING] = (const __u8[]){
		WIIMOD_NULL,
	},
	[WIIMOTE_DEV_UNKNOWN] = (const __u8[]){
		WIIMOD_NO_MP,
		WIIMOD_NULL,
	},
	[WIIMOTE_DEV_GENERIC] = (const __u8[]){
		WIIMOD_KEYS,
		WIIMOD_RUMBLE,
		WIIMOD_BATTERY,
		WIIMOD_LED1,
		WIIMOD_LED2,
		WIIMOD_LED3,
		WIIMOD_LED4,
		WIIMOD_ACCEL,
		WIIMOD_IR,
		WIIMOD_NULL,
	},
	[WIIMOTE_DEV_GEN10] = (const __u8[]){
		WIIMOD_KEYS,
		WIIMOD_RUMBLE,
		WIIMOD_BATTERY,
		WIIMOD_LED1,
		WIIMOD_LED2,
		WIIMOD_LED3,
		WIIMOD_LED4,
		WIIMOD_ACCEL,
		WIIMOD_IR,
		WIIMOD_NULL,
	},
	[WIIMOTE_DEV_GEN20] = (const __u8[]){
		WIIMOD_KEYS,
		WIIMOD_RUMBLE,
		WIIMOD_BATTERY,
		WIIMOD_LED1,
		WIIMOD_LED2,
		WIIMOD_LED3,
		WIIMOD_LED4,
		WIIMOD_ACCEL,
		WIIMOD_IR,
		WIIMOD_BUILTIN_MP,
		WIIMOD_NULL,
	},
	[WIIMOTE_DEV_BALANCE_BOARD] = (const __u8[]) {
		WIIMOD_BATTERY,
		WIIMOD_LED1,
		WIIMOD_NO_MP,
		WIIMOD_NULL,
	},
	[WIIMOTE_DEV_PRO_CONTROLLER] = (const __u8[]) {
		WIIMOD_BATTERY,
		WIIMOD_LED1,
		WIIMOD_LED2,
		WIIMOD_LED3,
		WIIMOD_LED4,
		WIIMOD_NO_MP,
		WIIMOD_NULL,
	},
};

static void wiimote_modules_load(struct wiimote_data *wdata,
				 unsigned int devtype)
{
	bool need_input = false;
	const __u8 *mods, *iter;
	const struct wiimod_ops *ops;
	int ret;

	mods = wiimote_devtype_mods[devtype];

	for (iter = mods; *iter != WIIMOD_NULL; ++iter) {
		if (wiimod_table[*iter]->flags & WIIMOD_FLAG_INPUT) {
			need_input = true;
			break;
		}
	}

	if (need_input) {
		wdata->input = input_allocate_device();
		if (!wdata->input)
			return;

		input_set_drvdata(wdata->input, wdata);
		wdata->input->dev.parent = &wdata->hdev->dev;
		wdata->input->id.bustype = wdata->hdev->bus;
		wdata->input->id.vendor = wdata->hdev->vendor;
		wdata->input->id.product = wdata->hdev->product;
		wdata->input->id.version = wdata->hdev->version;
		wdata->input->name = WIIMOTE_NAME;
	}

	for (iter = mods; *iter != WIIMOD_NULL; ++iter) {
		ops = wiimod_table[*iter];
		if (!ops->probe)
			continue;

		ret = ops->probe(ops, wdata);
		if (ret)
			goto error;
	}

	if (wdata->input) {
		ret = input_register_device(wdata->input);
		if (ret)
			goto error;
	}

	spin_lock_irq(&wdata->state.lock);
	wdata->state.devtype = devtype;
	spin_unlock_irq(&wdata->state.lock);
	return;

error:
	for ( ; iter-- != mods; ) {
		ops = wiimod_table[*iter];
		if (ops->remove)
			ops->remove(ops, wdata);
	}

	if (wdata->input) {
		input_free_device(wdata->input);
		wdata->input = NULL;
	}
}

static void wiimote_modules_unload(struct wiimote_data *wdata)
{
	const __u8 *mods, *iter;
	const struct wiimod_ops *ops;
	unsigned long flags;

	mods = wiimote_devtype_mods[wdata->state.devtype];

	spin_lock_irqsave(&wdata->state.lock, flags);
	wdata->state.devtype = WIIMOTE_DEV_UNKNOWN;
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	/* find end of list */
	for (iter = mods; *iter != WIIMOD_NULL; ++iter)
		/* empty */ ;

	if (wdata->input) {
		input_get_device(wdata->input);
		input_unregister_device(wdata->input);
	}

	for ( ; iter-- != mods; ) {
		ops = wiimod_table[*iter];
		if (ops->remove)
			ops->remove(ops, wdata);
	}

	if (wdata->input) {
		input_put_device(wdata->input);
		wdata->input = NULL;
	}
}

/* device extension handling */

static void wiimote_ext_load(struct wiimote_data *wdata, unsigned int ext)
{
	unsigned long flags;
	const struct wiimod_ops *ops;
	int ret;

	ops = wiimod_ext_table[ext];

	if (ops->probe) {
		ret = ops->probe(ops, wdata);
		if (ret)
			ext = WIIMOTE_EXT_UNKNOWN;
	}

	spin_lock_irqsave(&wdata->state.lock, flags);
	wdata->state.exttype = ext;
	spin_unlock_irqrestore(&wdata->state.lock, flags);
}

static void wiimote_ext_unload(struct wiimote_data *wdata)
{
	unsigned long flags;
	const struct wiimod_ops *ops;

	ops = wiimod_ext_table[wdata->state.exttype];

	spin_lock_irqsave(&wdata->state.lock, flags);
	wdata->state.exttype = WIIMOTE_EXT_UNKNOWN;
	wdata->state.flags &= ~WIIPROTO_FLAG_EXT_USED;
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	if (ops->remove)
		ops->remove(ops, wdata);
}

static void wiimote_mp_load(struct wiimote_data *wdata)
{
	unsigned long flags;
	const struct wiimod_ops *ops;
	int ret;
	__u8 mode = 2;

	ops = &wiimod_mp;
	if (ops->probe) {
		ret = ops->probe(ops, wdata);
		if (ret)
			mode = 1;
	}

	spin_lock_irqsave(&wdata->state.lock, flags);
	wdata->state.mp = mode;
	spin_unlock_irqrestore(&wdata->state.lock, flags);
}

static void wiimote_mp_unload(struct wiimote_data *wdata)
{
	unsigned long flags;
	const struct wiimod_ops *ops;

	if (wdata->state.mp < 2)
		return;

	ops = &wiimod_mp;

	spin_lock_irqsave(&wdata->state.lock, flags);
	wdata->state.mp = 0;
	wdata->state.flags &= ~WIIPROTO_FLAG_MP_USED;
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	if (ops->remove)
		ops->remove(ops, wdata);
}

/* device (re-)initialization and detection */

static const char *wiimote_devtype_names[WIIMOTE_DEV_NUM] = {
	[WIIMOTE_DEV_PENDING] = "Pending",
	[WIIMOTE_DEV_UNKNOWN] = "Unknown",
	[WIIMOTE_DEV_GENERIC] = "Generic",
	[WIIMOTE_DEV_GEN10] = "Nintendo Wii Remote (Gen 1)",
	[WIIMOTE_DEV_GEN20] = "Nintendo Wii Remote Plus (Gen 2)",
	[WIIMOTE_DEV_BALANCE_BOARD] = "Nintendo Wii Balance Board",
	[WIIMOTE_DEV_PRO_CONTROLLER] = "Nintendo Wii U Pro Controller",
};

/* Try to guess the device type based on all collected information. We
 * first try to detect by static extension types, then VID/PID and the
 * device name. If we cannot detect the device, we use
 * WIIMOTE_DEV_GENERIC so all modules will get probed on the device. */
static void wiimote_init_set_type(struct wiimote_data *wdata,
				  __u8 exttype)
{
	__u8 devtype = WIIMOTE_DEV_GENERIC;
	__u16 vendor, product;
	const char *name;

	vendor = wdata->hdev->vendor;
	product = wdata->hdev->product;
	name = wdata->hdev->name;

	if (exttype == WIIMOTE_EXT_BALANCE_BOARD) {
		devtype = WIIMOTE_DEV_BALANCE_BOARD;
		goto done;
	} else if (exttype == WIIMOTE_EXT_PRO_CONTROLLER) {
		devtype = WIIMOTE_DEV_PRO_CONTROLLER;
		goto done;
	}

	if (!strcmp(name, "Nintendo RVL-CNT-01")) {
		devtype = WIIMOTE_DEV_GEN10;
		goto done;
	} else if (!strcmp(name, "Nintendo RVL-CNT-01-TR")) {
		devtype = WIIMOTE_DEV_GEN20;
		goto done;
	} else if (!strcmp(name, "Nintendo RVL-WBC-01")) {
		devtype = WIIMOTE_DEV_BALANCE_BOARD;
		goto done;
	} else if (!strcmp(name, "Nintendo RVL-CNT-01-UC")) {
		devtype = WIIMOTE_DEV_PRO_CONTROLLER;
		goto done;
	}

	if (vendor == USB_VENDOR_ID_NINTENDO) {
		if (product == USB_DEVICE_ID_NINTENDO_WIIMOTE) {
			devtype = WIIMOTE_DEV_GEN10;
			goto done;
		} else if (product == USB_DEVICE_ID_NINTENDO_WIIMOTE2) {
			devtype = WIIMOTE_DEV_GEN20;
			goto done;
		}
	}

done:
	if (devtype == WIIMOTE_DEV_GENERIC)
		hid_info(wdata->hdev, "cannot detect device; NAME: %s VID: %04x PID: %04x EXT: %04x\n",
			name, vendor, product, exttype);
	else
		hid_info(wdata->hdev, "detected device: %s\n",
			 wiimote_devtype_names[devtype]);

	wiimote_modules_load(wdata, devtype);
}

static void wiimote_init_detect(struct wiimote_data *wdata)
{
	__u8 exttype = WIIMOTE_EXT_NONE, extdata[6];
	bool ext;
	int ret;

	wiimote_cmd_acquire_noint(wdata);

	spin_lock_irq(&wdata->state.lock);
	wdata->state.devtype = WIIMOTE_DEV_UNKNOWN;
	wiimote_cmd_set(wdata, WIIPROTO_REQ_SREQ, 0);
	wiiproto_req_status(wdata);
	spin_unlock_irq(&wdata->state.lock);

	ret = wiimote_cmd_wait_noint(wdata);
	if (ret)
		goto out_release;

	spin_lock_irq(&wdata->state.lock);
	ext = wdata->state.flags & WIIPROTO_FLAG_EXT_PLUGGED;
	spin_unlock_irq(&wdata->state.lock);

	if (!ext)
		goto out_release;

	wiimote_cmd_init_ext(wdata);
	exttype = wiimote_cmd_read_ext(wdata, extdata);

out_release:
	wiimote_cmd_release(wdata);
	wiimote_init_set_type(wdata, exttype);

	/* schedule MP timer */
	spin_lock_irq(&wdata->state.lock);
	if (!(wdata->state.flags & WIIPROTO_FLAG_BUILTIN_MP) &&
	    !(wdata->state.flags & WIIPROTO_FLAG_NO_MP))
		mod_timer(&wdata->timer, jiffies + HZ * 4);
	spin_unlock_irq(&wdata->state.lock);
}

/*
 * MP hotplug events are not generated by the wiimote. Therefore, we need
 * polling to detect it. We use a 4s interval for polling MP registers. This
 * seems reasonable considering applications can trigger it manually via
 * sysfs requests.
 */
static void wiimote_init_poll_mp(struct wiimote_data *wdata)
{
	bool mp;
	__u8 mpdata[6];

	wiimote_cmd_acquire_noint(wdata);
	wiimote_cmd_init_mp(wdata);
	mp = wiimote_cmd_read_mp(wdata, mpdata);
	wiimote_cmd_release(wdata);

	/* load/unload MP module if it changed */
	if (mp) {
		if (!wdata->state.mp) {
			hid_info(wdata->hdev, "detected extension: Nintendo Wii Motion Plus\n");
			wiimote_mp_load(wdata);
		}
	} else if (wdata->state.mp) {
		wiimote_mp_unload(wdata);
	}

	mod_timer(&wdata->timer, jiffies + HZ * 4);
}

/*
 * Check whether the wiimote is in the expected state. The extension registers
 * may change during hotplug and initialization so we might get hotplug events
 * that we caused by remapping some memory.
 * We use some heuristics here to check known states. If the wiimote is in the
 * expected state, we can ignore the hotplug event.
 *
 * Returns "true" if the device is in expected state, "false" if we should
 * redo hotplug handling and extension initialization.
 */
static bool wiimote_init_check(struct wiimote_data *wdata)
{
	__u32 flags;
	__u8 type, data[6];
	bool ret, poll_mp;

	spin_lock_irq(&wdata->state.lock);
	flags = wdata->state.flags;
	spin_unlock_irq(&wdata->state.lock);

	wiimote_cmd_acquire_noint(wdata);

	/* If MP is used and active, but the extension is not, we expect:
	 *   read_mp_mapped() == WIIMOTE_MP_SINGLE
	 *   state.flags == !EXT_ACTIVE && !MP_PLUGGED && MP_ACTIVE
	 * We do not check EXT_PLUGGED because it might change during
	 * initialization of MP without extensions.
	 *  - If MP is unplugged/replugged, read_mp_mapped() fails
	 *  - If EXT is plugged, MP_PLUGGED will get set */
	if (wdata->state.exttype == WIIMOTE_EXT_NONE &&
	    wdata->state.mp > 0 && (flags & WIIPROTO_FLAG_MP_USED)) {
		type = wiimote_cmd_read_mp_mapped(wdata);
		ret = type == WIIMOTE_MP_SINGLE;

		spin_lock_irq(&wdata->state.lock);
		ret = ret && !(wdata->state.flags & WIIPROTO_FLAG_EXT_ACTIVE);
		ret = ret && !(wdata->state.flags & WIIPROTO_FLAG_MP_PLUGGED);
		ret = ret && (wdata->state.flags & WIIPROTO_FLAG_MP_ACTIVE);
		spin_unlock_irq(&wdata->state.lock);

		if (!ret)
			hid_dbg(wdata->hdev, "state left: !EXT && MP\n");

		/* while MP is mapped, we get EXT_PLUGGED events */
		poll_mp = false;

		goto out_release;
	}

	/* If MP is unused, but the extension port is used, we expect:
	 *   read_ext == state.exttype
	 *   state.flags == !MP_ACTIVE && EXT_ACTIVE
	 * - If MP is plugged/unplugged, our timer detects it
	 * - If EXT is unplugged/replugged, EXT_ACTIVE will become unset */
	if (!(flags & WIIPROTO_FLAG_MP_USED) &&
	    wdata->state.exttype != WIIMOTE_EXT_NONE) {
		type = wiimote_cmd_read_ext(wdata, data);
		ret = type == wdata->state.exttype;

		spin_lock_irq(&wdata->state.lock);
		ret = ret && !(wdata->state.flags & WIIPROTO_FLAG_MP_ACTIVE);
		ret = ret && (wdata->state.flags & WIIPROTO_FLAG_EXT_ACTIVE);
		spin_unlock_irq(&wdata->state.lock);

		if (!ret)
			hid_dbg(wdata->hdev, "state left: EXT && !MP\n");

		/* poll MP for hotplug events */
		poll_mp = true;

		goto out_release;
	}

	/* If neither MP nor an extension are used, we expect:
	 *   read_ext() == WIIMOTE_EXT_NONE
	 *   state.flags == !MP_ACTIVE && !EXT_ACTIVE && !EXT_PLUGGED
	 * No need to perform any action in this case as everything is
	 * disabled already.
	 * - If MP is plugged/unplugged, our timer detects it
	 * - If EXT is plugged, EXT_PLUGGED will be set */
	if (!(flags & WIIPROTO_FLAG_MP_USED) &&
	    wdata->state.exttype == WIIMOTE_EXT_NONE) {
		type = wiimote_cmd_read_ext(wdata, data);
		ret = type == wdata->state.exttype;

		spin_lock_irq(&wdata->state.lock);
		ret = ret && !(wdata->state.flags & WIIPROTO_FLAG_EXT_ACTIVE);
		ret = ret && !(wdata->state.flags & WIIPROTO_FLAG_MP_ACTIVE);
		ret = ret && !(wdata->state.flags & WIIPROTO_FLAG_EXT_PLUGGED);
		spin_unlock_irq(&wdata->state.lock);

		if (!ret)
			hid_dbg(wdata->hdev, "state left: !EXT && !MP\n");

		/* poll MP for hotplug events */
		poll_mp = true;

		goto out_release;
	}

	/* The trickiest part is if both EXT and MP are active. We cannot read
	 * the EXT ID, anymore, because MP is mapped over it. However, we use
	 * a handy trick here:
	 *   - EXT_ACTIVE is unset whenever !MP_PLUGGED is sent
	 * MP_PLUGGED might be re-sent again before we are scheduled, but
	 * EXT_ACTIVE will stay unset.
	 * So it is enough to check for mp_mapped() and MP_ACTIVE and
	 * EXT_ACTIVE. EXT_PLUGGED is a sanity check. */
	if (wdata->state.exttype != WIIMOTE_EXT_NONE &&
	    wdata->state.mp > 0 && (flags & WIIPROTO_FLAG_MP_USED)) {
		type = wiimote_cmd_read_mp_mapped(wdata);
		ret = type != WIIMOTE_MP_NONE;
		ret = ret && type != WIIMOTE_MP_UNKNOWN;
		ret = ret && type != WIIMOTE_MP_SINGLE;

		spin_lock_irq(&wdata->state.lock);
		ret = ret && (wdata->state.flags & WIIPROTO_FLAG_EXT_PLUGGED);
		ret = ret && (wdata->state.flags & WIIPROTO_FLAG_EXT_ACTIVE);
		ret = ret && (wdata->state.flags & WIIPROTO_FLAG_MP_ACTIVE);
		spin_unlock_irq(&wdata->state.lock);

		if (!ret)
			hid_dbg(wdata->hdev, "state left: EXT && MP\n");

		/* while MP is mapped, we get EXT_PLUGGED events */
		poll_mp = false;

		goto out_release;
	}

	/* unknown state */
	ret = false;

out_release:
	wiimote_cmd_release(wdata);

	/* only poll for MP if requested and if state didn't change */
	if (ret && poll_mp && !(flags & WIIPROTO_FLAG_BUILTIN_MP) &&
	    !(flags & WIIPROTO_FLAG_NO_MP))
		wiimote_init_poll_mp(wdata);

	return ret;
}

static const char *wiimote_exttype_names[WIIMOTE_EXT_NUM] = {
	[WIIMOTE_EXT_NONE] = "None",
	[WIIMOTE_EXT_UNKNOWN] = "Unknown",
	[WIIMOTE_EXT_NUNCHUK] = "Nintendo Wii Nunchuk",
	[WIIMOTE_EXT_CLASSIC_CONTROLLER] = "Nintendo Wii Classic Controller",
	[WIIMOTE_EXT_BALANCE_BOARD] = "Nintendo Wii Balance Board",
	[WIIMOTE_EXT_PRO_CONTROLLER] = "Nintendo Wii U Pro Controller",
};

/*
 * Handle hotplug events
 * If we receive an hotplug event and the device-check failed, we deinitialize
 * the extension ports, re-read all extension IDs and set the device into
 * the desired state. This involves mapping MP into the main extension
 * registers, setting up extension passthrough modes and initializing the
 * requested extensions.
 */
static void wiimote_init_hotplug(struct wiimote_data *wdata)
{
	__u8 exttype, extdata[6], mpdata[6];
	__u32 flags;
	bool mp;

	hid_dbg(wdata->hdev, "detect extensions..\n");

	wiimote_cmd_acquire_noint(wdata);

	spin_lock_irq(&wdata->state.lock);

	/* get state snapshot that we will then work on */
	flags = wdata->state.flags;

	/* disable event forwarding temporarily */
	wdata->state.flags &= ~WIIPROTO_FLAG_EXT_ACTIVE;
	wdata->state.flags &= ~WIIPROTO_FLAG_MP_ACTIVE;

	spin_unlock_irq(&wdata->state.lock);

	/* init extension and MP (deactivates current extension or MP) */
	wiimote_cmd_init_ext(wdata);
	if (flags & WIIPROTO_FLAG_NO_MP) {
		mp = false;
	} else {
		wiimote_cmd_init_mp(wdata);
		mp = wiimote_cmd_read_mp(wdata, mpdata);
	}
	exttype = wiimote_cmd_read_ext(wdata, extdata);

	wiimote_cmd_release(wdata);

	/* load/unload extension module if it changed */
	if (exttype != wdata->state.exttype) {
		/* unload previous extension */
		wiimote_ext_unload(wdata);

		if (exttype == WIIMOTE_EXT_UNKNOWN) {
			hid_info(wdata->hdev, "cannot detect extension; %6phC\n",
				 extdata);
		} else if (exttype == WIIMOTE_EXT_NONE) {
			spin_lock_irq(&wdata->state.lock);
			wdata->state.exttype = WIIMOTE_EXT_NONE;
			spin_unlock_irq(&wdata->state.lock);
		} else {
			hid_info(wdata->hdev, "detected extension: %s\n",
				 wiimote_exttype_names[exttype]);
			/* try loading new extension */
			wiimote_ext_load(wdata, exttype);
		}
	}

	/* load/unload MP module if it changed */
	if (mp) {
		if (!wdata->state.mp) {
			hid_info(wdata->hdev, "detected extension: Nintendo Wii Motion Plus\n");
			wiimote_mp_load(wdata);
		}
	} else if (wdata->state.mp) {
		wiimote_mp_unload(wdata);
	}

	/* if MP is not used, do not map or activate it */
	if (!(flags & WIIPROTO_FLAG_MP_USED))
		mp = false;

	/* map MP into main extension registers if used */
	if (mp) {
		wiimote_cmd_acquire_noint(wdata);
		wiimote_cmd_map_mp(wdata, exttype);
		wiimote_cmd_release(wdata);

		/* delete MP hotplug timer */
		del_timer_sync(&wdata->timer);
	} else {
		/* reschedule MP hotplug timer */
		if (!(flags & WIIPROTO_FLAG_BUILTIN_MP) &&
		    !(flags & WIIPROTO_FLAG_NO_MP))
			mod_timer(&wdata->timer, jiffies + HZ * 4);
	}

	spin_lock_irq(&wdata->state.lock);

	/* enable data forwarding again and set expected hotplug state */
	if (mp) {
		wdata->state.flags |= WIIPROTO_FLAG_MP_ACTIVE;
		if (wdata->state.exttype == WIIMOTE_EXT_NONE) {
			wdata->state.flags &= ~WIIPROTO_FLAG_EXT_PLUGGED;
			wdata->state.flags &= ~WIIPROTO_FLAG_MP_PLUGGED;
		} else {
			wdata->state.flags &= ~WIIPROTO_FLAG_EXT_PLUGGED;
			wdata->state.flags |= WIIPROTO_FLAG_MP_PLUGGED;
			wdata->state.flags |= WIIPROTO_FLAG_EXT_ACTIVE;
		}
	} else if (wdata->state.exttype != WIIMOTE_EXT_NONE) {
		wdata->state.flags |= WIIPROTO_FLAG_EXT_ACTIVE;
	}

	/* request status report for hotplug state updates */
	wiiproto_req_status(wdata);

	spin_unlock_irq(&wdata->state.lock);

	hid_dbg(wdata->hdev, "detected extensions: MP: %d EXT: %d\n",
		wdata->state.mp, wdata->state.exttype);
}

static void wiimote_init_worker(struct work_struct *work)
{
	struct wiimote_data *wdata = container_of(work, struct wiimote_data,
						  init_worker);
	bool changed = false;

	if (wdata->state.devtype == WIIMOTE_DEV_PENDING) {
		wiimote_init_detect(wdata);
		changed = true;
	}

	if (changed || !wiimote_init_check(wdata))
		wiimote_init_hotplug(wdata);

	if (changed)
		kobject_uevent(&wdata->hdev->dev.kobj, KOBJ_CHANGE);
}

void __wiimote_schedule(struct wiimote_data *wdata)
{
	if (!(wdata->state.flags & WIIPROTO_FLAG_EXITING))
		schedule_work(&wdata->init_worker);
}

static void wiimote_schedule(struct wiimote_data *wdata)
{
	unsigned long flags;

	spin_lock_irqsave(&wdata->state.lock, flags);
	__wiimote_schedule(wdata);
	spin_unlock_irqrestore(&wdata->state.lock, flags);
}

static void wiimote_init_timeout(unsigned long arg)
{
	struct wiimote_data *wdata = (void*)arg;

	wiimote_schedule(wdata);
}

/* protocol handlers */

static void handler_keys(struct wiimote_data *wdata, const __u8 *payload)
{
	const __u8 *iter, *mods;
	const struct wiimod_ops *ops;

	ops = wiimod_ext_table[wdata->state.exttype];
	if (ops->in_keys) {
		ops->in_keys(wdata, payload);
		return;
	}

	mods = wiimote_devtype_mods[wdata->state.devtype];
	for (iter = mods; *iter != WIIMOD_NULL; ++iter) {
		ops = wiimod_table[*iter];
		if (ops->in_keys) {
			ops->in_keys(wdata, payload);
			break;
		}
	}
}

static void handler_accel(struct wiimote_data *wdata, const __u8 *payload)
{
	const __u8 *iter, *mods;
	const struct wiimod_ops *ops;

	ops = wiimod_ext_table[wdata->state.exttype];
	if (ops->in_accel) {
		ops->in_accel(wdata, payload);
		return;
	}

	mods = wiimote_devtype_mods[wdata->state.devtype];
	for (iter = mods; *iter != WIIMOD_NULL; ++iter) {
		ops = wiimod_table[*iter];
		if (ops->in_accel) {
			ops->in_accel(wdata, payload);
			break;
		}
	}
}

static bool valid_ext_handler(const struct wiimod_ops *ops, size_t len)
{
	if (!ops->in_ext)
		return false;
	if ((ops->flags & WIIMOD_FLAG_EXT8) && len < 8)
		return false;
	if ((ops->flags & WIIMOD_FLAG_EXT16) && len < 16)
		return false;

	return true;
}

static void handler_ext(struct wiimote_data *wdata, const __u8 *payload,
			size_t len)
{
	static const __u8 invalid[21] = { 0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff };
	const __u8 *iter, *mods;
	const struct wiimod_ops *ops;
	bool is_mp;

	if (len > 21)
		len = 21;
	if (len < 6 || !memcmp(payload, invalid, len))
		return;

	/* if MP is active, track MP slot hotplugging */
	if (wdata->state.flags & WIIPROTO_FLAG_MP_ACTIVE) {
		/* this bit is set for invalid events (eg. during hotplug) */
		if (payload[5] & 0x01)
			return;

		if (payload[4] & 0x01) {
			if (!(wdata->state.flags & WIIPROTO_FLAG_MP_PLUGGED)) {
				hid_dbg(wdata->hdev, "MP hotplug: 1\n");
				wdata->state.flags |= WIIPROTO_FLAG_MP_PLUGGED;
				__wiimote_schedule(wdata);
			}
		} else {
			if (wdata->state.flags & WIIPROTO_FLAG_MP_PLUGGED) {
				hid_dbg(wdata->hdev, "MP hotplug: 0\n");
				wdata->state.flags &= ~WIIPROTO_FLAG_MP_PLUGGED;
				wdata->state.flags &= ~WIIPROTO_FLAG_EXT_ACTIVE;
				__wiimote_schedule(wdata);
			}
		}

		/* detect MP data that is sent interleaved with EXT data */
		is_mp = payload[5] & 0x02;
	} else {
		is_mp = false;
	}

	/* ignore EXT events if no extension is active */
	if (!(wdata->state.flags & WIIPROTO_FLAG_EXT_ACTIVE) && !is_mp)
		return;

	/* try forwarding to extension handler, first */
	ops = wiimod_ext_table[wdata->state.exttype];
	if (is_mp && ops->in_mp) {
		ops->in_mp(wdata, payload);
		return;
	} else if (!is_mp && valid_ext_handler(ops, len)) {
		ops->in_ext(wdata, payload);
		return;
	}

	/* try forwarding to MP handler */
	ops = &wiimod_mp;
	if (is_mp && ops->in_mp) {
		ops->in_mp(wdata, payload);
		return;
	} else if (!is_mp && valid_ext_handler(ops, len)) {
		ops->in_ext(wdata, payload);
		return;
	}

	/* try forwarding to loaded modules */
	mods = wiimote_devtype_mods[wdata->state.devtype];
	for (iter = mods; *iter != WIIMOD_NULL; ++iter) {
		ops = wiimod_table[*iter];
		if (is_mp && ops->in_mp) {
			ops->in_mp(wdata, payload);
			return;
		} else if (!is_mp && valid_ext_handler(ops, len)) {
			ops->in_ext(wdata, payload);
			return;
		}
	}
}

#define ir_to_input0(wdata, ir, packed) handler_ir((wdata), (ir), (packed), 0)
#define ir_to_input1(wdata, ir, packed) handler_ir((wdata), (ir), (packed), 1)
#define ir_to_input2(wdata, ir, packed) handler_ir((wdata), (ir), (packed), 2)
#define ir_to_input3(wdata, ir, packed) handler_ir((wdata), (ir), (packed), 3)

static void handler_ir(struct wiimote_data *wdata, const __u8 *payload,
		       bool packed, unsigned int id)
{
	const __u8 *iter, *mods;
	const struct wiimod_ops *ops;

	ops = wiimod_ext_table[wdata->state.exttype];
	if (ops->in_ir) {
		ops->in_ir(wdata, payload, packed, id);
		return;
	}

	mods = wiimote_devtype_mods[wdata->state.devtype];
	for (iter = mods; *iter != WIIMOD_NULL; ++iter) {
		ops = wiimod_table[*iter];
		if (ops->in_ir) {
			ops->in_ir(wdata, payload, packed, id);
			break;
		}
	}
}

/* reduced status report with "BB BB" key data only */
static void handler_status_K(struct wiimote_data *wdata,
			     const __u8 *payload)
>>>>>>> refs/remotes/origin/master
{
	handler_keys(wdata, payload);

	/* on status reports the drm is reset so we need to resend the drm */
	wiiproto_req_drm(wdata, WIIPROTO_REQ_NULL);
<<<<<<< HEAD

	wiiext_event(wdata, payload[2] & 0x02);

	if (wiimote_cmd_pending(wdata, WIIPROTO_REQ_SREQ, 0)) {
		wdata->state.cmd_battery = payload[5];
		wiimote_cmd_complete(wdata);
	}
=======
}

/* extended status report with "BB BB LF 00 00 VV" data */
static void handler_status(struct wiimote_data *wdata, const __u8 *payload)
{
	handler_status_K(wdata, payload);

	/* update extension status */
	if (payload[2] & 0x02) {
		if (!(wdata->state.flags & WIIPROTO_FLAG_EXT_PLUGGED)) {
			hid_dbg(wdata->hdev, "EXT hotplug: 1\n");
			wdata->state.flags |= WIIPROTO_FLAG_EXT_PLUGGED;
			__wiimote_schedule(wdata);
		}
	} else {
		if (wdata->state.flags & WIIPROTO_FLAG_EXT_PLUGGED) {
			hid_dbg(wdata->hdev, "EXT hotplug: 0\n");
			wdata->state.flags &= ~WIIPROTO_FLAG_EXT_PLUGGED;
			wdata->state.flags &= ~WIIPROTO_FLAG_MP_PLUGGED;
			wdata->state.flags &= ~WIIPROTO_FLAG_EXT_ACTIVE;
			wdata->state.flags &= ~WIIPROTO_FLAG_MP_ACTIVE;
			__wiimote_schedule(wdata);
		}
	}

	wdata->state.cmd_battery = payload[5];
	if (wiimote_cmd_pending(wdata, WIIPROTO_REQ_SREQ, 0))
		wiimote_cmd_complete(wdata);
}

/* reduced generic report with "BB BB" key data only */
static void handler_generic_K(struct wiimote_data *wdata, const __u8 *payload)
{
	handler_keys(wdata, payload);
>>>>>>> refs/remotes/origin/master
}

static void handler_data(struct wiimote_data *wdata, const __u8 *payload)
{
	__u16 offset = payload[3] << 8 | payload[4];
	__u8 size = (payload[2] >> 4) + 1;
	__u8 err = payload[2] & 0x0f;

	handler_keys(wdata, payload);

	if (wiimote_cmd_pending(wdata, WIIPROTO_REQ_RMEM, offset)) {
		if (err)
			size = 0;
		else if (size > wdata->state.cmd_read_size)
			size = wdata->state.cmd_read_size;

		wdata->state.cmd_read_size = size;
		if (wdata->state.cmd_read_buf)
			memcpy(wdata->state.cmd_read_buf, &payload[5], size);
		wiimote_cmd_complete(wdata);
	}
}

static void handler_return(struct wiimote_data *wdata, const __u8 *payload)
{
	__u8 err = payload[3];
	__u8 cmd = payload[2];

	handler_keys(wdata, payload);

	if (wiimote_cmd_pending(wdata, cmd, 0)) {
		wdata->state.cmd_err = err;
		wiimote_cmd_complete(wdata);
	} else if (err) {
		hid_warn(wdata->hdev, "Remote error %hhu on req %hhu\n", err,
									cmd);
	}
}

static void handler_drm_KA(struct wiimote_data *wdata, const __u8 *payload)
{
	handler_keys(wdata, payload);
	handler_accel(wdata, payload);
}

static void handler_drm_KE(struct wiimote_data *wdata, const __u8 *payload)
{
	handler_keys(wdata, payload);
<<<<<<< HEAD
	wiiext_handle(wdata, &payload[2]);
=======
	handler_ext(wdata, &payload[2], 8);
>>>>>>> refs/remotes/origin/master
}

static void handler_drm_KAI(struct wiimote_data *wdata, const __u8 *payload)
{
	handler_keys(wdata, payload);
	handler_accel(wdata, payload);
	ir_to_input0(wdata, &payload[5], false);
	ir_to_input1(wdata, &payload[8], false);
	ir_to_input2(wdata, &payload[11], false);
	ir_to_input3(wdata, &payload[14], false);
<<<<<<< HEAD
	input_sync(wdata->ir);
=======
>>>>>>> refs/remotes/origin/master
}

static void handler_drm_KEE(struct wiimote_data *wdata, const __u8 *payload)
{
	handler_keys(wdata, payload);
<<<<<<< HEAD
	wiiext_handle(wdata, &payload[2]);
=======
	handler_ext(wdata, &payload[2], 19);
>>>>>>> refs/remotes/origin/master
}

static void handler_drm_KIE(struct wiimote_data *wdata, const __u8 *payload)
{
	handler_keys(wdata, payload);
	ir_to_input0(wdata, &payload[2], false);
	ir_to_input1(wdata, &payload[4], true);
	ir_to_input2(wdata, &payload[7], false);
	ir_to_input3(wdata, &payload[9], true);
<<<<<<< HEAD
	input_sync(wdata->ir);
	wiiext_handle(wdata, &payload[12]);
=======
	handler_ext(wdata, &payload[12], 9);
>>>>>>> refs/remotes/origin/master
}

static void handler_drm_KAE(struct wiimote_data *wdata, const __u8 *payload)
{
	handler_keys(wdata, payload);
	handler_accel(wdata, payload);
<<<<<<< HEAD
	wiiext_handle(wdata, &payload[5]);
=======
	handler_ext(wdata, &payload[5], 16);
>>>>>>> refs/remotes/origin/master
}

static void handler_drm_KAIE(struct wiimote_data *wdata, const __u8 *payload)
{
	handler_keys(wdata, payload);
	handler_accel(wdata, payload);
	ir_to_input0(wdata, &payload[5], false);
	ir_to_input1(wdata, &payload[7], true);
	ir_to_input2(wdata, &payload[10], false);
	ir_to_input3(wdata, &payload[12], true);
<<<<<<< HEAD
	input_sync(wdata->ir);
	wiiext_handle(wdata, &payload[15]);
=======
	handler_ext(wdata, &payload[15], 6);
>>>>>>> refs/remotes/origin/master
}

static void handler_drm_E(struct wiimote_data *wdata, const __u8 *payload)
{
<<<<<<< HEAD
	wiiext_handle(wdata, payload);
=======
	handler_ext(wdata, payload, 21);
>>>>>>> refs/remotes/origin/master
}

static void handler_drm_SKAI1(struct wiimote_data *wdata, const __u8 *payload)
{
	handler_keys(wdata, payload);

	wdata->state.accel_split[0] = payload[2];
	wdata->state.accel_split[1] = (payload[0] >> 1) & (0x10 | 0x20);
	wdata->state.accel_split[1] |= (payload[1] << 1) & (0x40 | 0x80);

	ir_to_input0(wdata, &payload[3], false);
	ir_to_input1(wdata, &payload[12], false);
<<<<<<< HEAD
	input_sync(wdata->ir);
=======
>>>>>>> refs/remotes/origin/master
}

static void handler_drm_SKAI2(struct wiimote_data *wdata, const __u8 *payload)
{
	__u8 buf[5];

	handler_keys(wdata, payload);

	wdata->state.accel_split[1] |= (payload[0] >> 5) & (0x01 | 0x02);
	wdata->state.accel_split[1] |= (payload[1] >> 3) & (0x04 | 0x08);

	buf[0] = 0;
	buf[1] = 0;
	buf[2] = wdata->state.accel_split[0];
	buf[3] = payload[2];
	buf[4] = wdata->state.accel_split[1];
	handler_accel(wdata, buf);

	ir_to_input2(wdata, &payload[3], false);
	ir_to_input3(wdata, &payload[12], false);
<<<<<<< HEAD
	input_sync(wdata->ir);
=======
>>>>>>> refs/remotes/origin/master
}

struct wiiproto_handler {
	__u8 id;
	size_t size;
	void (*func)(struct wiimote_data *wdata, const __u8 *payload);
};

static struct wiiproto_handler handlers[] = {
	{ .id = WIIPROTO_REQ_STATUS, .size = 6, .func = handler_status },
<<<<<<< HEAD
	{ .id = WIIPROTO_REQ_DATA, .size = 21, .func = handler_data },
	{ .id = WIIPROTO_REQ_RETURN, .size = 4, .func = handler_return },
	{ .id = WIIPROTO_REQ_DRM_K, .size = 2, .func = handler_keys },
	{ .id = WIIPROTO_REQ_DRM_KA, .size = 5, .func = handler_drm_KA },
	{ .id = WIIPROTO_REQ_DRM_KE, .size = 10, .func = handler_drm_KE },
	{ .id = WIIPROTO_REQ_DRM_KAI, .size = 17, .func = handler_drm_KAI },
	{ .id = WIIPROTO_REQ_DRM_KEE, .size = 21, .func = handler_drm_KEE },
	{ .id = WIIPROTO_REQ_DRM_KAE, .size = 21, .func = handler_drm_KAE },
	{ .id = WIIPROTO_REQ_DRM_KIE, .size = 21, .func = handler_drm_KIE },
	{ .id = WIIPROTO_REQ_DRM_KAIE, .size = 21, .func = handler_drm_KAIE },
=======
	{ .id = WIIPROTO_REQ_STATUS, .size = 2, .func = handler_status_K },
	{ .id = WIIPROTO_REQ_DATA, .size = 21, .func = handler_data },
	{ .id = WIIPROTO_REQ_DATA, .size = 2, .func = handler_generic_K },
	{ .id = WIIPROTO_REQ_RETURN, .size = 4, .func = handler_return },
	{ .id = WIIPROTO_REQ_RETURN, .size = 2, .func = handler_generic_K },
	{ .id = WIIPROTO_REQ_DRM_K, .size = 2, .func = handler_keys },
	{ .id = WIIPROTO_REQ_DRM_KA, .size = 5, .func = handler_drm_KA },
	{ .id = WIIPROTO_REQ_DRM_KA, .size = 2, .func = handler_generic_K },
	{ .id = WIIPROTO_REQ_DRM_KE, .size = 10, .func = handler_drm_KE },
	{ .id = WIIPROTO_REQ_DRM_KE, .size = 2, .func = handler_generic_K },
	{ .id = WIIPROTO_REQ_DRM_KAI, .size = 17, .func = handler_drm_KAI },
	{ .id = WIIPROTO_REQ_DRM_KAI, .size = 2, .func = handler_generic_K },
	{ .id = WIIPROTO_REQ_DRM_KEE, .size = 21, .func = handler_drm_KEE },
	{ .id = WIIPROTO_REQ_DRM_KEE, .size = 2, .func = handler_generic_K },
	{ .id = WIIPROTO_REQ_DRM_KAE, .size = 21, .func = handler_drm_KAE },
	{ .id = WIIPROTO_REQ_DRM_KAE, .size = 2, .func = handler_generic_K },
	{ .id = WIIPROTO_REQ_DRM_KIE, .size = 21, .func = handler_drm_KIE },
	{ .id = WIIPROTO_REQ_DRM_KIE, .size = 2, .func = handler_generic_K },
	{ .id = WIIPROTO_REQ_DRM_KAIE, .size = 21, .func = handler_drm_KAIE },
	{ .id = WIIPROTO_REQ_DRM_KAIE, .size = 2, .func = handler_generic_K },
>>>>>>> refs/remotes/origin/master
	{ .id = WIIPROTO_REQ_DRM_E, .size = 21, .func = handler_drm_E },
	{ .id = WIIPROTO_REQ_DRM_SKAI1, .size = 21, .func = handler_drm_SKAI1 },
	{ .id = WIIPROTO_REQ_DRM_SKAI2, .size = 21, .func = handler_drm_SKAI2 },
	{ .id = 0 }
};

static int wiimote_hid_event(struct hid_device *hdev, struct hid_report *report,
							u8 *raw_data, int size)
{
	struct wiimote_data *wdata = hid_get_drvdata(hdev);
	struct wiiproto_handler *h;
	int i;
	unsigned long flags;
<<<<<<< HEAD
	bool handled = false;
=======
>>>>>>> refs/remotes/origin/master

	if (size < 1)
		return -EINVAL;

	spin_lock_irqsave(&wdata->state.lock, flags);

	for (i = 0; handlers[i].id; ++i) {
		h = &handlers[i];
		if (h->id == raw_data[0] && h->size < size) {
			h->func(wdata, &raw_data[1]);
<<<<<<< HEAD
			handled = true;
		}
	}

	if (!handled)
=======
			break;
		}
	}

	if (!handlers[i].id)
>>>>>>> refs/remotes/origin/master
		hid_warn(hdev, "Unhandled report %hhu size %d\n", raw_data[0],
									size);

	spin_unlock_irqrestore(&wdata->state.lock, flags);

	return 0;
}

<<<<<<< HEAD
static void wiimote_leds_destroy(struct wiimote_data *wdata)
{
	int i;
	struct led_classdev *led;

	for (i = 0; i < 4; ++i) {
		if (wdata->leds[i]) {
			led = wdata->leds[i];
			wdata->leds[i] = NULL;
			led_classdev_unregister(led);
			kfree(led);
		}
	}
}

static int wiimote_leds_create(struct wiimote_data *wdata)
{
	int i, ret;
	struct device *dev = &wdata->hdev->dev;
	size_t namesz = strlen(dev_name(dev)) + 9;
	struct led_classdev *led;
	char *name;

	for (i = 0; i < 4; ++i) {
		led = kzalloc(sizeof(struct led_classdev) + namesz, GFP_KERNEL);
		if (!led) {
			ret = -ENOMEM;
			goto err;
		}
		name = (void*)&led[1];
		snprintf(name, namesz, "%s:blue:p%d", dev_name(dev), i);
		led->name = name;
		led->brightness = 0;
		led->max_brightness = 1;
		led->brightness_get = wiimote_leds_get;
		led->brightness_set = wiimote_leds_set;

		ret = led_classdev_register(dev, led);
		if (ret) {
			kfree(led);
			goto err;
		}
		wdata->leds[i] = led;
	}

	return 0;

err:
	wiimote_leds_destroy(wdata);
	return ret;
}

static struct wiimote_data *wiimote_create(struct hid_device *hdev)
{
	struct wiimote_data *wdata;
	int i;
=======
static ssize_t wiimote_ext_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	struct wiimote_data *wdata = dev_to_wii(dev);
	__u8 type;
	unsigned long flags;

	spin_lock_irqsave(&wdata->state.lock, flags);
	type = wdata->state.exttype;
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	switch (type) {
	case WIIMOTE_EXT_NONE:
		return sprintf(buf, "none\n");
	case WIIMOTE_EXT_NUNCHUK:
		return sprintf(buf, "nunchuk\n");
	case WIIMOTE_EXT_CLASSIC_CONTROLLER:
		return sprintf(buf, "classic\n");
	case WIIMOTE_EXT_BALANCE_BOARD:
		return sprintf(buf, "balanceboard\n");
	case WIIMOTE_EXT_PRO_CONTROLLER:
		return sprintf(buf, "procontroller\n");
	case WIIMOTE_EXT_UNKNOWN:
		/* fallthrough */
	default:
		return sprintf(buf, "unknown\n");
	}
}

static ssize_t wiimote_ext_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
{
	struct wiimote_data *wdata = dev_to_wii(dev);

	if (!strcmp(buf, "scan")) {
		wiimote_schedule(wdata);
	} else {
		return -EINVAL;
	}

	return strnlen(buf, PAGE_SIZE);
}

static DEVICE_ATTR(extension, S_IRUGO | S_IWUSR | S_IWGRP, wiimote_ext_show,
		   wiimote_ext_store);

static ssize_t wiimote_dev_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	struct wiimote_data *wdata = dev_to_wii(dev);
	__u8 type;
	unsigned long flags;

	spin_lock_irqsave(&wdata->state.lock, flags);
	type = wdata->state.devtype;
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	switch (type) {
	case WIIMOTE_DEV_GENERIC:
		return sprintf(buf, "generic\n");
	case WIIMOTE_DEV_GEN10:
		return sprintf(buf, "gen10\n");
	case WIIMOTE_DEV_GEN20:
		return sprintf(buf, "gen20\n");
	case WIIMOTE_DEV_BALANCE_BOARD:
		return sprintf(buf, "balanceboard\n");
	case WIIMOTE_DEV_PRO_CONTROLLER:
		return sprintf(buf, "procontroller\n");
	case WIIMOTE_DEV_PENDING:
		return sprintf(buf, "pending\n");
	case WIIMOTE_DEV_UNKNOWN:
		/* fallthrough */
	default:
		return sprintf(buf, "unknown\n");
	}
}

static DEVICE_ATTR(devtype, S_IRUGO, wiimote_dev_show, NULL);

static struct wiimote_data *wiimote_create(struct hid_device *hdev)
{
	struct wiimote_data *wdata;
>>>>>>> refs/remotes/origin/master

	wdata = kzalloc(sizeof(*wdata), GFP_KERNEL);
	if (!wdata)
		return NULL;

<<<<<<< HEAD
	wdata->input = input_allocate_device();
	if (!wdata->input)
		goto err;

	wdata->hdev = hdev;
	hid_set_drvdata(hdev, wdata);

	input_set_drvdata(wdata->input, wdata);
	wdata->input->open = wiimote_input_open;
	wdata->input->close = wiimote_input_close;
	wdata->input->dev.parent = &wdata->hdev->dev;
	wdata->input->id.bustype = wdata->hdev->bus;
	wdata->input->id.vendor = wdata->hdev->vendor;
	wdata->input->id.product = wdata->hdev->product;
	wdata->input->id.version = wdata->hdev->version;
	wdata->input->name = WIIMOTE_NAME;

	set_bit(EV_KEY, wdata->input->evbit);
	for (i = 0; i < WIIPROTO_KEY_COUNT; ++i)
		set_bit(wiiproto_keymap[i], wdata->input->keybit);

	set_bit(FF_RUMBLE, wdata->input->ffbit);
	if (input_ff_create_memless(wdata->input, NULL, wiimote_ff_play))
		goto err_input;

	wdata->accel = input_allocate_device();
	if (!wdata->accel)
		goto err_input;

	input_set_drvdata(wdata->accel, wdata);
	wdata->accel->open = wiimote_accel_open;
	wdata->accel->close = wiimote_accel_close;
	wdata->accel->dev.parent = &wdata->hdev->dev;
	wdata->accel->id.bustype = wdata->hdev->bus;
	wdata->accel->id.vendor = wdata->hdev->vendor;
	wdata->accel->id.product = wdata->hdev->product;
	wdata->accel->id.version = wdata->hdev->version;
	wdata->accel->name = WIIMOTE_NAME " Accelerometer";

	set_bit(EV_ABS, wdata->accel->evbit);
	set_bit(ABS_RX, wdata->accel->absbit);
	set_bit(ABS_RY, wdata->accel->absbit);
	set_bit(ABS_RZ, wdata->accel->absbit);
	input_set_abs_params(wdata->accel, ABS_RX, -500, 500, 2, 4);
	input_set_abs_params(wdata->accel, ABS_RY, -500, 500, 2, 4);
	input_set_abs_params(wdata->accel, ABS_RZ, -500, 500, 2, 4);

	wdata->ir = input_allocate_device();
	if (!wdata->ir)
		goto err_ir;

	input_set_drvdata(wdata->ir, wdata);
	wdata->ir->open = wiimote_ir_open;
	wdata->ir->close = wiimote_ir_close;
	wdata->ir->dev.parent = &wdata->hdev->dev;
	wdata->ir->id.bustype = wdata->hdev->bus;
	wdata->ir->id.vendor = wdata->hdev->vendor;
	wdata->ir->id.product = wdata->hdev->product;
	wdata->ir->id.version = wdata->hdev->version;
	wdata->ir->name = WIIMOTE_NAME " IR";

	set_bit(EV_ABS, wdata->ir->evbit);
	set_bit(ABS_HAT0X, wdata->ir->absbit);
	set_bit(ABS_HAT0Y, wdata->ir->absbit);
	set_bit(ABS_HAT1X, wdata->ir->absbit);
	set_bit(ABS_HAT1Y, wdata->ir->absbit);
	set_bit(ABS_HAT2X, wdata->ir->absbit);
	set_bit(ABS_HAT2Y, wdata->ir->absbit);
	set_bit(ABS_HAT3X, wdata->ir->absbit);
	set_bit(ABS_HAT3Y, wdata->ir->absbit);
	input_set_abs_params(wdata->ir, ABS_HAT0X, 0, 1023, 2, 4);
	input_set_abs_params(wdata->ir, ABS_HAT0Y, 0, 767, 2, 4);
	input_set_abs_params(wdata->ir, ABS_HAT1X, 0, 1023, 2, 4);
	input_set_abs_params(wdata->ir, ABS_HAT1Y, 0, 767, 2, 4);
	input_set_abs_params(wdata->ir, ABS_HAT2X, 0, 1023, 2, 4);
	input_set_abs_params(wdata->ir, ABS_HAT2Y, 0, 767, 2, 4);
	input_set_abs_params(wdata->ir, ABS_HAT3X, 0, 1023, 2, 4);
	input_set_abs_params(wdata->ir, ABS_HAT3Y, 0, 767, 2, 4);

	spin_lock_init(&wdata->qlock);
	INIT_WORK(&wdata->worker, wiimote_worker);
=======
	wdata->hdev = hdev;
	hid_set_drvdata(hdev, wdata);

	spin_lock_init(&wdata->queue.lock);
	INIT_WORK(&wdata->queue.worker, wiimote_queue_worker);
>>>>>>> refs/remotes/origin/master

	spin_lock_init(&wdata->state.lock);
	init_completion(&wdata->state.ready);
	mutex_init(&wdata->state.sync);
	wdata->state.drm = WIIPROTO_REQ_DRM_K;
<<<<<<< HEAD

	return wdata;

err_ir:
	input_free_device(wdata->accel);
err_input:
	input_free_device(wdata->input);
err:
	kfree(wdata);
	return NULL;
=======
	wdata->state.cmd_battery = 0xff;

	INIT_WORK(&wdata->init_worker, wiimote_init_worker);
	setup_timer(&wdata->timer, wiimote_init_timeout, (long)wdata);

	return wdata;
>>>>>>> refs/remotes/origin/master
}

static void wiimote_destroy(struct wiimote_data *wdata)
{
<<<<<<< HEAD
	wiidebug_deinit(wdata);
	wiiext_deinit(wdata);
	wiimote_leds_destroy(wdata);

	power_supply_unregister(&wdata->battery);
	input_unregister_device(wdata->accel);
	input_unregister_device(wdata->ir);
	input_unregister_device(wdata->input);
	cancel_work_sync(&wdata->worker);
=======
	unsigned long flags;

	wiidebug_deinit(wdata);

	/* prevent init_worker from being scheduled again */
	spin_lock_irqsave(&wdata->state.lock, flags);
	wdata->state.flags |= WIIPROTO_FLAG_EXITING;
	spin_unlock_irqrestore(&wdata->state.lock, flags);

	cancel_work_sync(&wdata->init_worker);
	del_timer_sync(&wdata->timer);

	device_remove_file(&wdata->hdev->dev, &dev_attr_devtype);
	device_remove_file(&wdata->hdev->dev, &dev_attr_extension);

	wiimote_mp_unload(wdata);
	wiimote_ext_unload(wdata);
	wiimote_modules_unload(wdata);
	cancel_work_sync(&wdata->queue.worker);
	hid_hw_close(wdata->hdev);
>>>>>>> refs/remotes/origin/master
	hid_hw_stop(wdata->hdev);

	kfree(wdata);
}

static int wiimote_hid_probe(struct hid_device *hdev,
				const struct hid_device_id *id)
{
	struct wiimote_data *wdata;
	int ret;

	hdev->quirks |= HID_QUIRK_NO_INIT_REPORTS;

	wdata = wiimote_create(hdev);
	if (!wdata) {
		hid_err(hdev, "Can't alloc device\n");
		return -ENOMEM;
	}

	ret = hid_parse(hdev);
	if (ret) {
		hid_err(hdev, "HID parse failed\n");
		goto err;
	}

	ret = hid_hw_start(hdev, HID_CONNECT_HIDRAW);
	if (ret) {
		hid_err(hdev, "HW start failed\n");
		goto err;
	}

<<<<<<< HEAD
	ret = input_register_device(wdata->accel);
	if (ret) {
		hid_err(hdev, "Cannot register input device\n");
		goto err_stop;
	}

	ret = input_register_device(wdata->ir);
	if (ret) {
		hid_err(hdev, "Cannot register input device\n");
		goto err_ir;
	}

	ret = input_register_device(wdata->input);
	if (ret) {
		hid_err(hdev, "Cannot register input device\n");
		goto err_input;
	}

	wdata->battery.properties = wiimote_battery_props;
	wdata->battery.num_properties = ARRAY_SIZE(wiimote_battery_props);
	wdata->battery.get_property = wiimote_battery_get_property;
	wdata->battery.name = "wiimote_battery";
	wdata->battery.type = POWER_SUPPLY_TYPE_BATTERY;
	wdata->battery.use_for_apm = 0;

	ret = power_supply_register(&wdata->hdev->dev, &wdata->battery);
	if (ret) {
		hid_err(hdev, "Cannot register battery device\n");
		goto err_battery;
	}

	power_supply_powers(&wdata->battery, &hdev->dev);

	ret = wiimote_leds_create(wdata);
	if (ret)
		goto err_free;

	ret = wiiext_init(wdata);
	if (ret)
		goto err_free;

=======
	ret = hid_hw_open(hdev);
	if (ret) {
		hid_err(hdev, "cannot start hardware I/O\n");
		goto err_stop;
	}

	ret = device_create_file(&hdev->dev, &dev_attr_extension);
	if (ret) {
		hid_err(hdev, "cannot create sysfs attribute\n");
		goto err_close;
	}

	ret = device_create_file(&hdev->dev, &dev_attr_devtype);
	if (ret) {
		hid_err(hdev, "cannot create sysfs attribute\n");
		goto err_ext;
	}

>>>>>>> refs/remotes/origin/master
	ret = wiidebug_init(wdata);
	if (ret)
		goto err_free;

	hid_info(hdev, "New device registered\n");

<<<<<<< HEAD
	/* by default set led1 after device initialization */
	spin_lock_irq(&wdata->state.lock);
	wiiproto_req_leds(wdata, WIIPROTO_FLAG_LED1);
	spin_unlock_irq(&wdata->state.lock);
=======
	/* schedule device detection */
	wiimote_schedule(wdata);
>>>>>>> refs/remotes/origin/master

	return 0;

err_free:
	wiimote_destroy(wdata);
	return ret;

<<<<<<< HEAD
err_battery:
	input_unregister_device(wdata->input);
	wdata->input = NULL;
err_input:
	input_unregister_device(wdata->ir);
	wdata->ir = NULL;
err_ir:
	input_unregister_device(wdata->accel);
	wdata->accel = NULL;
=======
err_ext:
	device_remove_file(&wdata->hdev->dev, &dev_attr_extension);
err_close:
	hid_hw_close(hdev);
>>>>>>> refs/remotes/origin/master
err_stop:
	hid_hw_stop(hdev);
err:
	input_free_device(wdata->ir);
	input_free_device(wdata->accel);
<<<<<<< HEAD
	input_free_device(wdata->input);
=======
>>>>>>> refs/remotes/origin/master
	kfree(wdata);
	return ret;
}

static void wiimote_hid_remove(struct hid_device *hdev)
{
	struct wiimote_data *wdata = hid_get_drvdata(hdev);

	hid_info(hdev, "Device removed\n");
	wiimote_destroy(wdata);
}

static const struct hid_device_id wiimote_hid_devices[] = {
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_NINTENDO,
				USB_DEVICE_ID_NINTENDO_WIIMOTE) },
<<<<<<< HEAD
=======
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_NINTENDO,
				USB_DEVICE_ID_NINTENDO_WIIMOTE2) },
>>>>>>> refs/remotes/origin/master
	{ }
};
MODULE_DEVICE_TABLE(hid, wiimote_hid_devices);

static struct hid_driver wiimote_hid_driver = {
	.name = "wiimote",
	.id_table = wiimote_hid_devices,
	.probe = wiimote_hid_probe,
	.remove = wiimote_hid_remove,
	.raw_event = wiimote_hid_event,
};
<<<<<<< HEAD

static int __init wiimote_init(void)
{
	int ret;

	ret = hid_register_driver(&wiimote_hid_driver);
	if (ret)
		pr_err("Can't register wiimote hid driver\n");

	return ret;
}

static void __exit wiimote_exit(void)
{
	hid_unregister_driver(&wiimote_hid_driver);
}

module_init(wiimote_init);
module_exit(wiimote_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Herrmann <dh.herrmann@gmail.com>");
MODULE_DESCRIPTION(WIIMOTE_NAME " Device Driver");
=======
module_hid_driver(wiimote_hid_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Herrmann <dh.herrmann@gmail.com>");
MODULE_DESCRIPTION("Driver for Nintendo Wii / Wii U peripherals");
>>>>>>> refs/remotes/origin/master
