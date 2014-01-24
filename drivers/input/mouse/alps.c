/*
 * ALPS touchpad PS/2 mouse driver
 *
 * Copyright (c) 2003 Neil Brown <neilb@cse.unsw.edu.au>
 * Copyright (c) 2003-2005 Peter Osterlund <petero2@telia.com>
 * Copyright (c) 2004 Dmitry Torokhov <dtor@mail.ru>
 * Copyright (c) 2005 Vojtech Pavlik <vojtech@suse.cz>
 * Copyright (c) 2009 Sebastian Kapfer <sebastian_kapfer@gmx.net>
 *
 * ALPS detection, tap switching and status querying info is taken from
 * tpconfig utility (by C. Scott Ananian and Bruce Kall).
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

#include <linux/slab.h>
#include <linux/input.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/input/mt.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/input/mt.h>
>>>>>>> refs/remotes/origin/master
#include <linux/serio.h>
#include <linux/libps2.h>

#include "psmouse.h"
#include "alps.h"

<<<<<<< HEAD
<<<<<<< HEAD
#undef DEBUG
#ifdef DEBUG
#define dbg(format, arg...) printk(KERN_INFO "alps.c: " format "\n", ## arg)
#else
#define dbg(format, arg...) do {} while (0)
#endif

#define ALPS_OLDPROTO		0x01	/* old style input */
=======
/*
 * Definitions for ALPS version 3 and 4 command mode protocol
 */
#define ALPS_V3_X_MAX	2000
#define ALPS_V3_Y_MAX	1400

#define ALPS_BITMAP_X_BITS	15
#define ALPS_BITMAP_Y_BITS	11

#define ALPS_CMD_NIBBLE_10	0x01f2

=======
/*
 * Definitions for ALPS version 3 and 4 command mode protocol
 */
#define ALPS_CMD_NIBBLE_10	0x01f2

#define ALPS_REG_BASE_RUSHMORE	0xc2c0
#define ALPS_REG_BASE_PINNACLE	0x0000

>>>>>>> refs/remotes/origin/master
static const struct alps_nibble_commands alps_v3_nibble_commands[] = {
	{ PSMOUSE_CMD_SETPOLL,		0x00 }, /* 0 */
	{ PSMOUSE_CMD_RESET_DIS,	0x00 }, /* 1 */
	{ PSMOUSE_CMD_SETSCALE21,	0x00 }, /* 2 */
	{ PSMOUSE_CMD_SETRATE,		0x0a }, /* 3 */
	{ PSMOUSE_CMD_SETRATE,		0x14 }, /* 4 */
	{ PSMOUSE_CMD_SETRATE,		0x28 }, /* 5 */
	{ PSMOUSE_CMD_SETRATE,		0x3c }, /* 6 */
	{ PSMOUSE_CMD_SETRATE,		0x50 }, /* 7 */
	{ PSMOUSE_CMD_SETRATE,		0x64 }, /* 8 */
	{ PSMOUSE_CMD_SETRATE,		0xc8 }, /* 9 */
	{ ALPS_CMD_NIBBLE_10,		0x00 }, /* a */
	{ PSMOUSE_CMD_SETRES,		0x00 }, /* b */
	{ PSMOUSE_CMD_SETRES,		0x01 }, /* c */
	{ PSMOUSE_CMD_SETRES,		0x02 }, /* d */
	{ PSMOUSE_CMD_SETRES,		0x03 }, /* e */
	{ PSMOUSE_CMD_SETSCALE11,	0x00 }, /* f */
};

static const struct alps_nibble_commands alps_v4_nibble_commands[] = {
	{ PSMOUSE_CMD_ENABLE,		0x00 }, /* 0 */
	{ PSMOUSE_CMD_RESET_DIS,	0x00 }, /* 1 */
	{ PSMOUSE_CMD_SETSCALE21,	0x00 }, /* 2 */
	{ PSMOUSE_CMD_SETRATE,		0x0a }, /* 3 */
	{ PSMOUSE_CMD_SETRATE,		0x14 }, /* 4 */
	{ PSMOUSE_CMD_SETRATE,		0x28 }, /* 5 */
	{ PSMOUSE_CMD_SETRATE,		0x3c }, /* 6 */
	{ PSMOUSE_CMD_SETRATE,		0x50 }, /* 7 */
	{ PSMOUSE_CMD_SETRATE,		0x64 }, /* 8 */
	{ PSMOUSE_CMD_SETRATE,		0xc8 }, /* 9 */
	{ ALPS_CMD_NIBBLE_10,		0x00 }, /* a */
	{ PSMOUSE_CMD_SETRES,		0x00 }, /* b */
	{ PSMOUSE_CMD_SETRES,		0x01 }, /* c */
	{ PSMOUSE_CMD_SETRES,		0x02 }, /* d */
	{ PSMOUSE_CMD_SETRES,		0x03 }, /* e */
	{ PSMOUSE_CMD_SETSCALE11,	0x00 }, /* f */
};

<<<<<<< HEAD

>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct alps_nibble_commands alps_v6_nibble_commands[] = {
	{ PSMOUSE_CMD_ENABLE,		0x00 }, /* 0 */
	{ PSMOUSE_CMD_SETRATE,		0x0a }, /* 1 */
	{ PSMOUSE_CMD_SETRATE,		0x14 }, /* 2 */
	{ PSMOUSE_CMD_SETRATE,		0x28 }, /* 3 */
	{ PSMOUSE_CMD_SETRATE,		0x3c }, /* 4 */
	{ PSMOUSE_CMD_SETRATE,		0x50 }, /* 5 */
	{ PSMOUSE_CMD_SETRATE,		0x64 }, /* 6 */
	{ PSMOUSE_CMD_SETRATE,		0xc8 }, /* 7 */
	{ PSMOUSE_CMD_GETID,		0x00 }, /* 8 */
	{ PSMOUSE_CMD_GETINFO,		0x00 }, /* 9 */
	{ PSMOUSE_CMD_SETRES,		0x00 }, /* a */
	{ PSMOUSE_CMD_SETRES,		0x01 }, /* b */
	{ PSMOUSE_CMD_SETRES,		0x02 }, /* c */
	{ PSMOUSE_CMD_SETRES,		0x03 }, /* d */
	{ PSMOUSE_CMD_SETSCALE21,	0x00 }, /* e */
	{ PSMOUSE_CMD_SETSCALE11,	0x00 }, /* f */
};


>>>>>>> refs/remotes/origin/master
#define ALPS_DUALPOINT		0x02	/* touchpad has trackstick */
#define ALPS_PASS		0x04	/* device has a pass-through port */

#define ALPS_WHEEL		0x08	/* hardware wheel present */
#define ALPS_FW_BK_1		0x10	/* front & back buttons present */
#define ALPS_FW_BK_2		0x20	/* front & back buttons present */
#define ALPS_FOUR_BUTTONS	0x40	/* 4 direction button present */
#define ALPS_PS2_INTERLEAVED	0x80	/* 3-byte PS/2 packet interleaved with
					   6-byte ALPS packet */

static const struct alps_model_info alps_model_data[] = {
<<<<<<< HEAD
<<<<<<< HEAD
	{ { 0x32, 0x02, 0x14 },	0xf8, 0xf8, ALPS_PASS | ALPS_DUALPOINT }, /* Toshiba Salellite Pro M10 */
	{ { 0x33, 0x02, 0x0a },	0x88, 0xf8, ALPS_OLDPROTO },		  /* UMAX-530T */
	{ { 0x53, 0x02, 0x0a },	0xf8, 0xf8, 0 },
	{ { 0x53, 0x02, 0x14 },	0xf8, 0xf8, 0 },
	{ { 0x60, 0x03, 0xc8 }, 0xf8, 0xf8, 0 },			  /* HP ze1115 */
	{ { 0x63, 0x02, 0x0a },	0xf8, 0xf8, 0 },
	{ { 0x63, 0x02, 0x14 },	0xf8, 0xf8, 0 },
	{ { 0x63, 0x02, 0x28 },	0xf8, 0xf8, ALPS_FW_BK_2 },		  /* Fujitsu Siemens S6010 */
	{ { 0x63, 0x02, 0x3c },	0x8f, 0x8f, ALPS_WHEEL },		  /* Toshiba Satellite S2400-103 */
	{ { 0x63, 0x02, 0x50 },	0xef, 0xef, ALPS_FW_BK_1 },		  /* NEC Versa L320 */
	{ { 0x63, 0x02, 0x64 },	0xf8, 0xf8, 0 },
	{ { 0x63, 0x03, 0xc8 }, 0xf8, 0xf8, ALPS_PASS | ALPS_DUALPOINT }, /* Dell Latitude D800 */
	{ { 0x73, 0x00, 0x0a },	0xf8, 0xf8, ALPS_DUALPOINT },		  /* ThinkPad R61 8918-5QG */
	{ { 0x73, 0x02, 0x0a },	0xf8, 0xf8, 0 },
	{ { 0x73, 0x02, 0x14 },	0xf8, 0xf8, ALPS_FW_BK_2 },		  /* Ahtec Laptop */
	{ { 0x20, 0x02, 0x0e },	0xf8, 0xf8, ALPS_PASS | ALPS_DUALPOINT }, /* XXX */
	{ { 0x22, 0x02, 0x0a },	0xf8, 0xf8, ALPS_PASS | ALPS_DUALPOINT },
	{ { 0x22, 0x02, 0x14 }, 0xff, 0xff, ALPS_PASS | ALPS_DUALPOINT }, /* Dell Latitude D600 */
	/* Dell Latitude E5500, E6400, E6500, Precision M4400 */
	{ { 0x62, 0x02, 0x14 }, 0xcf, 0xcf,
		ALPS_PASS | ALPS_DUALPOINT | ALPS_PS2_INTERLEAVED },
	{ { 0x73, 0x02, 0x50 }, 0xcf, 0xcf, ALPS_FOUR_BUTTONS },	  /* Dell Vostro 1400 */
	{ { 0x52, 0x01, 0x14 }, 0xff, 0xff,
		ALPS_PASS | ALPS_DUALPOINT | ALPS_PS2_INTERLEAVED },	  /* Toshiba Tecra A11-11L */
=======
=======
>>>>>>> refs/remotes/origin/master
	{ { 0x32, 0x02, 0x14 },	0x00, ALPS_PROTO_V2, 0xf8, 0xf8, ALPS_PASS | ALPS_DUALPOINT },	/* Toshiba Salellite Pro M10 */
	{ { 0x33, 0x02, 0x0a },	0x00, ALPS_PROTO_V1, 0x88, 0xf8, 0 },				/* UMAX-530T */
	{ { 0x53, 0x02, 0x0a },	0x00, ALPS_PROTO_V2, 0xf8, 0xf8, 0 },
	{ { 0x53, 0x02, 0x14 },	0x00, ALPS_PROTO_V2, 0xf8, 0xf8, 0 },
	{ { 0x60, 0x03, 0xc8 }, 0x00, ALPS_PROTO_V2, 0xf8, 0xf8, 0 },				/* HP ze1115 */
	{ { 0x63, 0x02, 0x0a },	0x00, ALPS_PROTO_V2, 0xf8, 0xf8, 0 },
	{ { 0x63, 0x02, 0x14 },	0x00, ALPS_PROTO_V2, 0xf8, 0xf8, 0 },
	{ { 0x63, 0x02, 0x28 },	0x00, ALPS_PROTO_V2, 0xf8, 0xf8, ALPS_FW_BK_2 },		/* Fujitsu Siemens S6010 */
	{ { 0x63, 0x02, 0x3c },	0x00, ALPS_PROTO_V2, 0x8f, 0x8f, ALPS_WHEEL },			/* Toshiba Satellite S2400-103 */
	{ { 0x63, 0x02, 0x50 },	0x00, ALPS_PROTO_V2, 0xef, 0xef, ALPS_FW_BK_1 },		/* NEC Versa L320 */
	{ { 0x63, 0x02, 0x64 },	0x00, ALPS_PROTO_V2, 0xf8, 0xf8, 0 },
	{ { 0x63, 0x03, 0xc8 }, 0x00, ALPS_PROTO_V2, 0xf8, 0xf8, ALPS_PASS | ALPS_DUALPOINT },	/* Dell Latitude D800 */
	{ { 0x73, 0x00, 0x0a },	0x00, ALPS_PROTO_V2, 0xf8, 0xf8, ALPS_DUALPOINT },		/* ThinkPad R61 8918-5QG */
	{ { 0x73, 0x02, 0x0a },	0x00, ALPS_PROTO_V2, 0xf8, 0xf8, 0 },
	{ { 0x73, 0x02, 0x14 },	0x00, ALPS_PROTO_V2, 0xf8, 0xf8, ALPS_FW_BK_2 },		/* Ahtec Laptop */
	{ { 0x20, 0x02, 0x0e },	0x00, ALPS_PROTO_V2, 0xf8, 0xf8, ALPS_PASS | ALPS_DUALPOINT },	/* XXX */
	{ { 0x22, 0x02, 0x0a },	0x00, ALPS_PROTO_V2, 0xf8, 0xf8, ALPS_PASS | ALPS_DUALPOINT },
	{ { 0x22, 0x02, 0x14 }, 0x00, ALPS_PROTO_V2, 0xff, 0xff, ALPS_PASS | ALPS_DUALPOINT },	/* Dell Latitude D600 */
	/* Dell Latitude E5500, E6400, E6500, Precision M4400 */
	{ { 0x62, 0x02, 0x14 }, 0x00, ALPS_PROTO_V2, 0xcf, 0xcf,
		ALPS_PASS | ALPS_DUALPOINT | ALPS_PS2_INTERLEAVED },
<<<<<<< HEAD
	{ { 0x73, 0x02, 0x50 }, 0x00, ALPS_PROTO_V2, 0xcf, 0xcf, ALPS_FOUR_BUTTONS },		/* Dell Vostro 1400 */
	{ { 0x52, 0x01, 0x14 }, 0x00, ALPS_PROTO_V2, 0xff, 0xff,
		ALPS_PASS | ALPS_DUALPOINT | ALPS_PS2_INTERLEAVED },				/* Toshiba Tecra A11-11L */
	{ { 0x73, 0x02, 0x64 },	0x9b, ALPS_PROTO_V3, 0x8f, 0x8f, ALPS_DUALPOINT },
	{ { 0x73, 0x02, 0x64 },	0x9d, ALPS_PROTO_V3, 0x8f, 0x8f, ALPS_DUALPOINT },
	{ { 0x73, 0x02, 0x64 },	0x8a, ALPS_PROTO_V4, 0x8f, 0x8f, 0 },
>>>>>>> refs/remotes/origin/cm-10.0
};

=======
	{ { 0x73, 0x00, 0x14 }, 0x00, ALPS_PROTO_V6, 0xff, 0xff, ALPS_DUALPOINT },		/* Dell XT2 */
	{ { 0x73, 0x02, 0x50 }, 0x00, ALPS_PROTO_V2, 0xcf, 0xcf, ALPS_FOUR_BUTTONS },		/* Dell Vostro 1400 */
	{ { 0x52, 0x01, 0x14 }, 0x00, ALPS_PROTO_V2, 0xff, 0xff,
		ALPS_PASS | ALPS_DUALPOINT | ALPS_PS2_INTERLEAVED },				/* Toshiba Tecra A11-11L */
	{ { 0x73, 0x02, 0x64 },	0x8a, ALPS_PROTO_V4, 0x8f, 0x8f, 0 },
};

static void alps_set_abs_params_st(struct alps_data *priv,
				   struct input_dev *dev1);
static void alps_set_abs_params_mt(struct alps_data *priv,
				   struct input_dev *dev1);

>>>>>>> refs/remotes/origin/master
/*
 * XXX - this entry is suspicious. First byte has zero lower nibble,
 * which is what a normal mouse would report. Also, the value 0x0e
 * isn't valid per PS/2 spec.
 */

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * PS/2 packet format
 *
 * byte 0:  0    0 YSGN XSGN    1    M    R    L
 * byte 1: X7   X6   X5   X4   X3   X2   X1   X0
 * byte 2: Y7   Y6   Y5   Y4   Y3   Y2   Y1   Y0
 *
 * Note that the device never signals overflow condition.
 *
 * ALPS absolute Mode - new format
 *
 * byte 0:  1    ?    ?    ?    1    ?    ?    ?
 * byte 1:  0   x6   x5   x4   x3   x2   x1   x0
 * byte 2:  0  x10   x9   x8   x7    ?  fin  ges
 * byte 3:  0   y9   y8   y7    1    M    R    L
 * byte 4:  0   y6   y5   y4   y3   y2   y1   y0
 * byte 5:  0   z6   z5   z4   z3   z2   z1   z0
 *
 * Dualpoint device -- interleaved packet format
 *
 * byte 0:    1    1    0    0    1    1    1    1
 * byte 1:    0   x6   x5   x4   x3   x2   x1   x0
 * byte 2:    0  x10   x9   x8   x7    0  fin  ges
 * byte 3:    0    0 YSGN XSGN    1    1    1    1
 * byte 4:   X7   X6   X5   X4   X3   X2   X1   X0
 * byte 5:   Y7   Y6   Y5   Y4   Y3   Y2   Y1   Y0
 * byte 6:    0   y9   y8   y7    1    m    r    l
 * byte 7:    0   y6   y5   y4   y3   y2   y1   y0
 * byte 8:    0   z6   z5   z4   z3   z2   z1   z0
 *
 * CAPITALS = stick, miniscules = touchpad
 *
 * ?'s can have different meanings on different models,
 * such as wheel rotation, extra buttons, stick buttons
 * on a dualpoint, etc.
 */
=======
/* Packet formats are described in Documentation/input/alps.txt */
>>>>>>> refs/remotes/origin/cm-10.0

static bool alps_is_valid_first_byte(const struct alps_model_info *model,
				     unsigned char data)
{
	return (data & model->mask0) == model->byte0;
=======
/* Packet formats are described in Documentation/input/alps.txt */

static bool alps_is_valid_first_byte(struct alps_data *priv,
				     unsigned char data)
{
	return (data & priv->mask0) == priv->byte0;
>>>>>>> refs/remotes/origin/master
}

static void alps_report_buttons(struct psmouse *psmouse,
				struct input_dev *dev1, struct input_dev *dev2,
				int left, int right, int middle)
{
	struct input_dev *dev;

	/*
	 * If shared button has already been reported on the
	 * other device (dev2) then this event should be also
	 * sent through that device.
	 */
	dev = test_bit(BTN_LEFT, dev2->key) ? dev2 : dev1;
	input_report_key(dev, BTN_LEFT, left);

	dev = test_bit(BTN_RIGHT, dev2->key) ? dev2 : dev1;
	input_report_key(dev, BTN_RIGHT, right);

	dev = test_bit(BTN_MIDDLE, dev2->key) ? dev2 : dev1;
	input_report_key(dev, BTN_MIDDLE, middle);

	/*
	 * Sync the _other_ device now, we'll do the first
	 * device later once we report the rest of the events.
	 */
	input_sync(dev2);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void alps_process_packet(struct psmouse *psmouse)
=======
static void alps_process_packet_v1_v2(struct psmouse *psmouse)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct alps_data *priv = psmouse->private;
	const struct alps_model_info *model = priv->i;
=======
static void alps_process_packet_v1_v2(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;
>>>>>>> refs/remotes/origin/master
	unsigned char *packet = psmouse->packet;
	struct input_dev *dev = psmouse->dev;
	struct input_dev *dev2 = priv->dev2;
	int x, y, z, ges, fin, left, right, middle;
	int back = 0, forward = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	if (model->flags & ALPS_OLDPROTO) {
=======
	if (model->proto_version == ALPS_PROTO_V1) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (priv->proto_version == ALPS_PROTO_V1) {
>>>>>>> refs/remotes/origin/master
		left = packet[2] & 0x10;
		right = packet[2] & 0x08;
		middle = 0;
		x = packet[1] | ((packet[0] & 0x07) << 7);
		y = packet[4] | ((packet[3] & 0x07) << 7);
		z = packet[5];
	} else {
		left = packet[3] & 1;
		right = packet[3] & 2;
		middle = packet[3] & 4;
		x = packet[1] | ((packet[2] & 0x78) << (7 - 3));
		y = packet[4] | ((packet[3] & 0x70) << (7 - 4));
		z = packet[5];
	}

<<<<<<< HEAD
	if (model->flags & ALPS_FW_BK_1) {
=======
	if (priv->flags & ALPS_FW_BK_1) {
>>>>>>> refs/remotes/origin/master
		back = packet[0] & 0x10;
		forward = packet[2] & 4;
	}

<<<<<<< HEAD
	if (model->flags & ALPS_FW_BK_2) {
=======
	if (priv->flags & ALPS_FW_BK_2) {
>>>>>>> refs/remotes/origin/master
		back = packet[3] & 4;
		forward = packet[2] & 4;
		if ((middle = forward && back))
			forward = back = 0;
	}

	ges = packet[2] & 1;
	fin = packet[2] & 2;

<<<<<<< HEAD
	if ((model->flags & ALPS_DUALPOINT) && z == 127) {
=======
	if ((priv->flags & ALPS_DUALPOINT) && z == 127) {
>>>>>>> refs/remotes/origin/master
		input_report_rel(dev2, REL_X,  (x > 383 ? (x - 768) : x));
		input_report_rel(dev2, REL_Y, -(y > 255 ? (y - 512) : y));

		alps_report_buttons(psmouse, dev2, dev, left, right, middle);

		input_sync(dev2);
		return;
	}

	alps_report_buttons(psmouse, dev, dev2, left, right, middle);

	/* Convert hardware tap to a reasonable Z value */
	if (ges && !fin)
		z = 40;

	/*
	 * A "tap and drag" operation is reported by the hardware as a transition
	 * from (!fin && ges) to (fin && ges). This should be translated to the
	 * sequence Z>0, Z==0, Z>0, so the Z==0 event has to be generated manually.
	 */
	if (ges && fin && !priv->prev_fin) {
		input_report_abs(dev, ABS_X, x);
		input_report_abs(dev, ABS_Y, y);
		input_report_abs(dev, ABS_PRESSURE, 0);
		input_report_key(dev, BTN_TOOL_FINGER, 0);
		input_sync(dev);
	}
	priv->prev_fin = fin;

	if (z > 30)
		input_report_key(dev, BTN_TOUCH, 1);
	if (z < 25)
		input_report_key(dev, BTN_TOUCH, 0);

	if (z > 0) {
		input_report_abs(dev, ABS_X, x);
		input_report_abs(dev, ABS_Y, y);
	}

	input_report_abs(dev, ABS_PRESSURE, z);
	input_report_key(dev, BTN_TOOL_FINGER, z > 0);

<<<<<<< HEAD
	if (model->flags & ALPS_WHEEL)
		input_report_rel(dev, REL_WHEEL, ((packet[2] << 1) & 0x08) - ((packet[0] >> 4) & 0x07));

	if (model->flags & (ALPS_FW_BK_1 | ALPS_FW_BK_2)) {
=======
	if (priv->flags & ALPS_WHEEL)
		input_report_rel(dev, REL_WHEEL, ((packet[2] << 1) & 0x08) - ((packet[0] >> 4) & 0x07));

	if (priv->flags & (ALPS_FW_BK_1 | ALPS_FW_BK_2)) {
>>>>>>> refs/remotes/origin/master
		input_report_key(dev, BTN_FORWARD, forward);
		input_report_key(dev, BTN_BACK, back);
	}

<<<<<<< HEAD
	if (model->flags & ALPS_FOUR_BUTTONS) {
=======
	if (priv->flags & ALPS_FOUR_BUTTONS) {
>>>>>>> refs/remotes/origin/master
		input_report_key(dev, BTN_0, packet[2] & 4);
		input_report_key(dev, BTN_1, packet[0] & 0x10);
		input_report_key(dev, BTN_2, packet[3] & 4);
		input_report_key(dev, BTN_3, packet[0] & 0x20);
	}

	input_sync(dev);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * Process bitmap data from v3 and v4 protocols. Returns the number of
 * fingers detected. A return value of 0 means at least one of the
 * bitmaps was empty.
 *
 * The bitmaps don't have enough data to track fingers, so this function
 * only generates points representing a bounding box of all contacts.
 * These points are returned in x1, y1, x2, and y2 when the return value
 * is greater than 0.
 */
<<<<<<< HEAD
static int alps_process_bitmap(unsigned int x_map, unsigned int y_map,
=======
static int alps_process_bitmap(struct alps_data *priv,
			       unsigned int x_map, unsigned int y_map,
>>>>>>> refs/remotes/origin/master
			       int *x1, int *y1, int *x2, int *y2)
{
	struct alps_bitmap_point {
		int start_bit;
		int num_bits;
	};

	int fingers_x = 0, fingers_y = 0, fingers;
	int i, bit, prev_bit;
	struct alps_bitmap_point x_low = {0,}, x_high = {0,};
	struct alps_bitmap_point y_low = {0,}, y_high = {0,};
	struct alps_bitmap_point *point;

	if (!x_map || !y_map)
		return 0;

	*x1 = *y1 = *x2 = *y2 = 0;

	prev_bit = 0;
	point = &x_low;
	for (i = 0; x_map != 0; i++, x_map >>= 1) {
		bit = x_map & 1;
		if (bit) {
			if (!prev_bit) {
				point->start_bit = i;
				fingers_x++;
			}
			point->num_bits++;
		} else {
			if (prev_bit)
				point = &x_high;
			else
				point->num_bits = 0;
		}
		prev_bit = bit;
	}

	/*
	 * y bitmap is reversed for what we need (lower positions are in
	 * higher bits), so we process from the top end.
	 */
<<<<<<< HEAD
	y_map = y_map << (sizeof(y_map) * BITS_PER_BYTE - ALPS_BITMAP_Y_BITS);
=======
	y_map = y_map << (sizeof(y_map) * BITS_PER_BYTE - priv->y_bits);
>>>>>>> refs/remotes/origin/master
	prev_bit = 0;
	point = &y_low;
	for (i = 0; y_map != 0; i++, y_map <<= 1) {
		bit = y_map & (1 << (sizeof(y_map) * BITS_PER_BYTE - 1));
		if (bit) {
			if (!prev_bit) {
				point->start_bit = i;
				fingers_y++;
			}
			point->num_bits++;
		} else {
			if (prev_bit)
				point = &y_high;
			else
				point->num_bits = 0;
		}
		prev_bit = bit;
	}

	/*
	 * Fingers can overlap, so we use the maximum count of fingers
	 * on either axis as the finger count.
	 */
	fingers = max(fingers_x, fingers_y);

	/*
	 * If total fingers is > 1 but either axis reports only a single
	 * contact, we have overlapping or adjacent fingers. For the
	 * purposes of creating a bounding box, divide the single contact
	 * (roughly) equally between the two points.
	 */
	if (fingers > 1) {
		if (fingers_x == 1) {
			i = x_low.num_bits / 2;
			x_low.num_bits = x_low.num_bits - i;
			x_high.start_bit = x_low.start_bit + i;
			x_high.num_bits = max(i, 1);
		} else if (fingers_y == 1) {
			i = y_low.num_bits / 2;
			y_low.num_bits = y_low.num_bits - i;
			y_high.start_bit = y_low.start_bit + i;
			y_high.num_bits = max(i, 1);
		}
	}

<<<<<<< HEAD
	*x1 = (ALPS_V3_X_MAX * (2 * x_low.start_bit + x_low.num_bits - 1)) /
	      (2 * (ALPS_BITMAP_X_BITS - 1));
	*y1 = (ALPS_V3_Y_MAX * (2 * y_low.start_bit + y_low.num_bits - 1)) /
	      (2 * (ALPS_BITMAP_Y_BITS - 1));

	if (fingers > 1) {
		*x2 = (ALPS_V3_X_MAX * (2 * x_high.start_bit + x_high.num_bits - 1)) /
		      (2 * (ALPS_BITMAP_X_BITS - 1));
		*y2 = (ALPS_V3_Y_MAX * (2 * y_high.start_bit + y_high.num_bits - 1)) /
		      (2 * (ALPS_BITMAP_Y_BITS - 1));
=======
	*x1 = (priv->x_max * (2 * x_low.start_bit + x_low.num_bits - 1)) /
	      (2 * (priv->x_bits - 1));
	*y1 = (priv->y_max * (2 * y_low.start_bit + y_low.num_bits - 1)) /
	      (2 * (priv->y_bits - 1));

	if (fingers > 1) {
		*x2 = (priv->x_max *
		       (2 * x_high.start_bit + x_high.num_bits - 1)) /
		      (2 * (priv->x_bits - 1));
		*y2 = (priv->y_max *
		       (2 * y_high.start_bit + y_high.num_bits - 1)) /
		      (2 * (priv->y_bits - 1));
>>>>>>> refs/remotes/origin/master
	}

	return fingers;
}

static void alps_set_slot(struct input_dev *dev, int slot, bool active,
			  int x, int y)
{
	input_mt_slot(dev, slot);
	input_mt_report_slot_state(dev, MT_TOOL_FINGER, active);
	if (active) {
		input_report_abs(dev, ABS_MT_POSITION_X, x);
		input_report_abs(dev, ABS_MT_POSITION_Y, y);
	}
}

static void alps_report_semi_mt_data(struct input_dev *dev, int num_fingers,
				     int x1, int y1, int x2, int y2)
{
	alps_set_slot(dev, 0, num_fingers != 0, x1, y1);
	alps_set_slot(dev, 1, num_fingers == 2, x2, y2);
}

static void alps_process_trackstick_packet_v3(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;
	unsigned char *packet = psmouse->packet;
	struct input_dev *dev = priv->dev2;
	int x, y, z, left, right, middle;

	/* Sanity check packet */
	if (!(packet[0] & 0x40)) {
		psmouse_dbg(psmouse, "Bad trackstick packet, discarding\n");
		return;
	}

	/*
	 * There's a special packet that seems to indicate the end
	 * of a stream of trackstick data. Filter these out.
	 */
	if (packet[1] == 0x7f && packet[2] == 0x7f && packet[4] == 0x7f)
		return;

	x = (s8)(((packet[0] & 0x20) << 2) | (packet[1] & 0x7f));
	y = (s8)(((packet[0] & 0x10) << 3) | (packet[2] & 0x7f));
	z = (packet[4] & 0x7c) >> 2;

	/*
	 * The x and y values tend to be quite large, and when used
	 * alone the trackstick is difficult to use. Scale them down
	 * to compensate.
	 */
	x /= 8;
	y /= 8;

	input_report_rel(dev, REL_X, x);
	input_report_rel(dev, REL_Y, -y);

	/*
	 * Most ALPS models report the trackstick buttons in the touchpad
	 * packets, but a few report them here. No reliable way has been
	 * found to differentiate between the models upfront, so we enable
	 * the quirk in response to seeing a button press in the trackstick
	 * packet.
	 */
	left = packet[3] & 0x01;
	right = packet[3] & 0x02;
	middle = packet[3] & 0x04;

	if (!(priv->quirks & ALPS_QUIRK_TRACKSTICK_BUTTONS) &&
	    (left || right || middle))
		priv->quirks |= ALPS_QUIRK_TRACKSTICK_BUTTONS;

	if (priv->quirks & ALPS_QUIRK_TRACKSTICK_BUTTONS) {
		input_report_key(dev, BTN_LEFT, left);
		input_report_key(dev, BTN_RIGHT, right);
		input_report_key(dev, BTN_MIDDLE, middle);
	}

	input_sync(dev);
	return;
}

<<<<<<< HEAD
=======
static void alps_decode_buttons_v3(struct alps_fields *f, unsigned char *p)
{
	f->left = !!(p[3] & 0x01);
	f->right = !!(p[3] & 0x02);
	f->middle = !!(p[3] & 0x04);

	f->ts_left = !!(p[3] & 0x10);
	f->ts_right = !!(p[3] & 0x20);
	f->ts_middle = !!(p[3] & 0x40);
}

static void alps_decode_pinnacle(struct alps_fields *f, unsigned char *p)
{
	f->first_mp = !!(p[4] & 0x40);
	f->is_mp = !!(p[0] & 0x40);

	f->fingers = (p[5] & 0x3) + 1;
	f->x_map = ((p[4] & 0x7e) << 8) |
		   ((p[1] & 0x7f) << 2) |
		   ((p[0] & 0x30) >> 4);
	f->y_map = ((p[3] & 0x70) << 4) |
		   ((p[2] & 0x7f) << 1) |
		   (p[4] & 0x01);

	f->x = ((p[1] & 0x7f) << 4) | ((p[4] & 0x30) >> 2) |
	       ((p[0] & 0x30) >> 4);
	f->y = ((p[2] & 0x7f) << 4) | (p[4] & 0x0f);
	f->z = p[5] & 0x7f;

	alps_decode_buttons_v3(f, p);
}

static void alps_decode_rushmore(struct alps_fields *f, unsigned char *p)
{
	alps_decode_pinnacle(f, p);

	f->x_map |= (p[5] & 0x10) << 11;
	f->y_map |= (p[5] & 0x20) << 6;
}

static void alps_decode_dolphin(struct alps_fields *f, unsigned char *p)
{
	f->first_mp = !!(p[0] & 0x02);
	f->is_mp = !!(p[0] & 0x20);

	f->fingers = ((p[0] & 0x6) >> 1 |
		     (p[0] & 0x10) >> 2);
	f->x_map = ((p[2] & 0x60) >> 5) |
		   ((p[4] & 0x7f) << 2) |
		   ((p[5] & 0x7f) << 9) |
		   ((p[3] & 0x07) << 16) |
		   ((p[3] & 0x70) << 15) |
		   ((p[0] & 0x01) << 22);
	f->y_map = (p[1] & 0x7f) |
		   ((p[2] & 0x1f) << 7);

	f->x = ((p[1] & 0x7f) | ((p[4] & 0x0f) << 7));
	f->y = ((p[2] & 0x7f) | ((p[4] & 0xf0) << 3));
	f->z = (p[0] & 4) ? 0 : p[5] & 0x7f;

	alps_decode_buttons_v3(f, p);
}

>>>>>>> refs/remotes/origin/master
static void alps_process_touchpad_packet_v3(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;
	unsigned char *packet = psmouse->packet;
	struct input_dev *dev = psmouse->dev;
	struct input_dev *dev2 = priv->dev2;
<<<<<<< HEAD
	int x, y, z;
	int left, right, middle;
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	int fingers = 0, bmap_fingers;
	unsigned int x_bitmap, y_bitmap;
=======
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	int fingers = 0, bmap_fingers;
	struct alps_fields f;

	priv->decode_fields(&f, packet);
>>>>>>> refs/remotes/origin/master

	/*
	 * There's no single feature of touchpad position and bitmap packets
	 * that can be used to distinguish between them. We rely on the fact
	 * that a bitmap packet should always follow a position packet with
	 * bit 6 of packet[4] set.
	 */
	if (priv->multi_packet) {
		/*
		 * Sometimes a position packet will indicate a multi-packet
		 * sequence, but then what follows is another position
		 * packet. Check for this, and when it happens process the
		 * position packet as usual.
		 */
<<<<<<< HEAD
		if (packet[0] & 0x40) {
			fingers = (packet[5] & 0x3) + 1;
			x_bitmap = ((packet[4] & 0x7e) << 8) |
				   ((packet[1] & 0x7f) << 2) |
				   ((packet[0] & 0x30) >> 4);
			y_bitmap = ((packet[3] & 0x70) << 4) |
				   ((packet[2] & 0x7f) << 1) |
				   (packet[4] & 0x01);

			bmap_fingers = alps_process_bitmap(x_bitmap, y_bitmap,
=======
		if (f.is_mp) {
			fingers = f.fingers;
			bmap_fingers = alps_process_bitmap(priv,
							   f.x_map, f.y_map,
>>>>>>> refs/remotes/origin/master
							   &x1, &y1, &x2, &y2);

			/*
			 * We shouldn't report more than one finger if
			 * we don't have two coordinates.
			 */
			if (fingers > 1 && bmap_fingers < 2)
				fingers = bmap_fingers;

			/* Now process position packet */
<<<<<<< HEAD
			packet = priv->multi_data;
=======
			priv->decode_fields(&f, priv->multi_data);
>>>>>>> refs/remotes/origin/master
		} else {
			priv->multi_packet = 0;
		}
	}

	/*
	 * Bit 6 of byte 0 is not usually set in position packets. The only
	 * times it seems to be set is in situations where the data is
	 * suspect anyway, e.g. a palm resting flat on the touchpad. Given
	 * this combined with the fact that this bit is useful for filtering
	 * out misidentified bitmap packets, we reject anything with this
	 * bit set.
	 */
<<<<<<< HEAD
	if (packet[0] & 0x40)
		return;

	if (!priv->multi_packet && (packet[4] & 0x40)) {
=======
	if (f.is_mp)
		return;

	if (!priv->multi_packet && f.first_mp) {
>>>>>>> refs/remotes/origin/master
		priv->multi_packet = 1;
		memcpy(priv->multi_data, packet, sizeof(priv->multi_data));
		return;
	}

	priv->multi_packet = 0;

<<<<<<< HEAD
	left = packet[3] & 0x01;
	right = packet[3] & 0x02;
	middle = packet[3] & 0x04;

	x = ((packet[1] & 0x7f) << 4) | ((packet[4] & 0x30) >> 2) |
	    ((packet[0] & 0x30) >> 4);
	y = ((packet[2] & 0x7f) << 4) | (packet[4] & 0x0f);
	z = packet[5] & 0x7f;

=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Sometimes the hardware sends a single packet with z = 0
	 * in the middle of a stream. Real releases generate packets
	 * with x, y, and z all zero, so these seem to be flukes.
	 * Ignore them.
	 */
<<<<<<< HEAD
	if (x && y && !z)
=======
	if (f.x && f.y && !f.z)
>>>>>>> refs/remotes/origin/master
		return;

	/*
	 * If we don't have MT data or the bitmaps were empty, we have
	 * to rely on ST data.
	 */
	if (!fingers) {
<<<<<<< HEAD
		x1 = x;
		y1 = y;
		fingers = z > 0 ? 1 : 0;
	}

	if (z >= 64)
=======
		x1 = f.x;
		y1 = f.y;
		fingers = f.z > 0 ? 1 : 0;
	}

	if (f.z >= 64)
>>>>>>> refs/remotes/origin/master
		input_report_key(dev, BTN_TOUCH, 1);
	else
		input_report_key(dev, BTN_TOUCH, 0);

	alps_report_semi_mt_data(dev, fingers, x1, y1, x2, y2);

<<<<<<< HEAD
	input_report_key(dev, BTN_TOOL_FINGER, fingers == 1);
	input_report_key(dev, BTN_TOOL_DOUBLETAP, fingers == 2);
	input_report_key(dev, BTN_TOOL_TRIPLETAP, fingers == 3);
	input_report_key(dev, BTN_TOOL_QUADTAP, fingers == 4);

	input_report_key(dev, BTN_LEFT, left);
	input_report_key(dev, BTN_RIGHT, right);
	input_report_key(dev, BTN_MIDDLE, middle);

	if (z > 0) {
		input_report_abs(dev, ABS_X, x);
		input_report_abs(dev, ABS_Y, y);
	}
	input_report_abs(dev, ABS_PRESSURE, z);
=======
	input_mt_report_finger_count(dev, fingers);

	input_report_key(dev, BTN_LEFT, f.left);
	input_report_key(dev, BTN_RIGHT, f.right);
	input_report_key(dev, BTN_MIDDLE, f.middle);

	if (f.z > 0) {
		input_report_abs(dev, ABS_X, f.x);
		input_report_abs(dev, ABS_Y, f.y);
	}
	input_report_abs(dev, ABS_PRESSURE, f.z);
>>>>>>> refs/remotes/origin/master

	input_sync(dev);

	if (!(priv->quirks & ALPS_QUIRK_TRACKSTICK_BUTTONS)) {
<<<<<<< HEAD
		left = packet[3] & 0x10;
		right = packet[3] & 0x20;
		middle = packet[3] & 0x40;

		input_report_key(dev2, BTN_LEFT, left);
		input_report_key(dev2, BTN_RIGHT, right);
		input_report_key(dev2, BTN_MIDDLE, middle);
=======
		input_report_key(dev2, BTN_LEFT, f.ts_left);
		input_report_key(dev2, BTN_RIGHT, f.ts_right);
		input_report_key(dev2, BTN_MIDDLE, f.ts_middle);
>>>>>>> refs/remotes/origin/master
		input_sync(dev2);
	}
}

static void alps_process_packet_v3(struct psmouse *psmouse)
{
	unsigned char *packet = psmouse->packet;

	/*
	 * v3 protocol packets come in three types, two representing
	 * touchpad data and one representing trackstick data.
	 * Trackstick packets seem to be distinguished by always
	 * having 0x3f in the last byte. This value has never been
	 * observed in the last byte of either of the other types
	 * of packets.
	 */
	if (packet[5] == 0x3f) {
		alps_process_trackstick_packet_v3(psmouse);
		return;
	}

	alps_process_touchpad_packet_v3(psmouse);
}

<<<<<<< HEAD
static void alps_process_packet_v4(struct psmouse *psmouse)
{
	unsigned char *packet = psmouse->packet;
	struct input_dev *dev = psmouse->dev;
	int x, y, z;
	int left, right;
=======
static void alps_process_packet_v6(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;
	unsigned char *packet = psmouse->packet;
	struct input_dev *dev = psmouse->dev;
	struct input_dev *dev2 = priv->dev2;
	int x, y, z, left, right, middle;

	/*
	 * We can use Byte5 to distinguish if the packet is from Touchpad
	 * or Trackpoint.
	 * Touchpad:	0 - 0x7E
	 * Trackpoint:	0x7F
	 */
	if (packet[5] == 0x7F) {
		/* It should be a DualPoint when received Trackpoint packet */
		if (!(priv->flags & ALPS_DUALPOINT))
			return;

		/* Trackpoint packet */
		x = packet[1] | ((packet[3] & 0x20) << 2);
		y = packet[2] | ((packet[3] & 0x40) << 1);
		z = packet[4];
		left = packet[3] & 0x01;
		right = packet[3] & 0x02;
		middle = packet[3] & 0x04;

		/* To prevent the cursor jump when finger lifted */
		if (x == 0x7F && y == 0x7F && z == 0x7F)
			x = y = z = 0;

		/* Divide 4 since trackpoint's speed is too fast */
		input_report_rel(dev2, REL_X, (char)x / 4);
		input_report_rel(dev2, REL_Y, -((char)y / 4));

		input_report_key(dev2, BTN_LEFT, left);
		input_report_key(dev2, BTN_RIGHT, right);
		input_report_key(dev2, BTN_MIDDLE, middle);

		input_sync(dev2);
		return;
	}

	/* Touchpad packet */
	x = packet[1] | ((packet[3] & 0x78) << 4);
	y = packet[2] | ((packet[4] & 0x78) << 4);
	z = packet[5];
	left = packet[3] & 0x01;
	right = packet[3] & 0x02;

	if (z > 30)
		input_report_key(dev, BTN_TOUCH, 1);
	if (z < 25)
		input_report_key(dev, BTN_TOUCH, 0);

	if (z > 0) {
		input_report_abs(dev, ABS_X, x);
		input_report_abs(dev, ABS_Y, y);
	}

	input_report_abs(dev, ABS_PRESSURE, z);
	input_report_key(dev, BTN_TOOL_FINGER, z > 0);

	/* v6 touchpad does not have middle button */
	input_report_key(dev, BTN_LEFT, left);
	input_report_key(dev, BTN_RIGHT, right);

	input_sync(dev);
}

static void alps_process_packet_v4(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;
	unsigned char *packet = psmouse->packet;
	struct input_dev *dev = psmouse->dev;
	int offset;
	int x, y, z;
	int left, right;
	int x1, y1, x2, y2;
	int fingers = 0;
	unsigned int x_bitmap, y_bitmap;

	/*
	 * v4 has a 6-byte encoding for bitmap data, but this data is
	 * broken up between 3 normal packets. Use priv->multi_packet to
	 * track our position in the bitmap packet.
	 */
	if (packet[6] & 0x40) {
		/* sync, reset position */
		priv->multi_packet = 0;
	}

	if (WARN_ON_ONCE(priv->multi_packet > 2))
		return;

	offset = 2 * priv->multi_packet;
	priv->multi_data[offset] = packet[6];
	priv->multi_data[offset + 1] = packet[7];

	if (++priv->multi_packet > 2) {
		priv->multi_packet = 0;

		x_bitmap = ((priv->multi_data[2] & 0x1f) << 10) |
			   ((priv->multi_data[3] & 0x60) << 3) |
			   ((priv->multi_data[0] & 0x3f) << 2) |
			   ((priv->multi_data[1] & 0x60) >> 5);
		y_bitmap = ((priv->multi_data[5] & 0x01) << 10) |
			   ((priv->multi_data[3] & 0x1f) << 5) |
			    (priv->multi_data[1] & 0x1f);

		fingers = alps_process_bitmap(priv, x_bitmap, y_bitmap,
					      &x1, &y1, &x2, &y2);

		/* Store MT data.*/
		priv->fingers = fingers;
		priv->x1 = x1;
		priv->x2 = x2;
		priv->y1 = y1;
		priv->y2 = y2;
	}
>>>>>>> refs/remotes/origin/master

	left = packet[4] & 0x01;
	right = packet[4] & 0x02;

	x = ((packet[1] & 0x7f) << 4) | ((packet[3] & 0x30) >> 2) |
	    ((packet[0] & 0x30) >> 4);
	y = ((packet[2] & 0x7f) << 4) | (packet[3] & 0x0f);
	z = packet[5] & 0x7f;

<<<<<<< HEAD
=======
	/*
	 * If there were no contacts in the bitmap, use ST
	 * points in MT reports.
	 * If there were two contacts or more, report MT data.
	 */
	if (priv->fingers < 2) {
		x1 = x;
		y1 = y;
		fingers = z > 0 ? 1 : 0;
	} else {
		fingers = priv->fingers;
		x1 = priv->x1;
		x2 = priv->x2;
		y1 = priv->y1;
		y2 = priv->y2;
	}

>>>>>>> refs/remotes/origin/master
	if (z >= 64)
		input_report_key(dev, BTN_TOUCH, 1);
	else
		input_report_key(dev, BTN_TOUCH, 0);

<<<<<<< HEAD
=======
	alps_report_semi_mt_data(dev, fingers, x1, y1, x2, y2);

	input_mt_report_finger_count(dev, fingers);

	input_report_key(dev, BTN_LEFT, left);
	input_report_key(dev, BTN_RIGHT, right);

>>>>>>> refs/remotes/origin/master
	if (z > 0) {
		input_report_abs(dev, ABS_X, x);
		input_report_abs(dev, ABS_Y, y);
	}
	input_report_abs(dev, ABS_PRESSURE, z);

<<<<<<< HEAD
	input_report_key(dev, BTN_TOOL_FINGER, z > 0);
	input_report_key(dev, BTN_LEFT, left);
	input_report_key(dev, BTN_RIGHT, right);

	input_sync(dev);
}

static void alps_process_packet(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;
	const struct alps_model_info *model = priv->i;

	switch (model->proto_version) {
	case ALPS_PROTO_V1:
	case ALPS_PROTO_V2:
		alps_process_packet_v1_v2(psmouse);
		break;
	case ALPS_PROTO_V3:
		alps_process_packet_v3(psmouse);
		break;
	case ALPS_PROTO_V4:
		alps_process_packet_v4(psmouse);
		break;
	}
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	input_sync(dev);
}

>>>>>>> refs/remotes/origin/master
static void alps_report_bare_ps2_packet(struct psmouse *psmouse,
					unsigned char packet[],
					bool report_buttons)
{
	struct alps_data *priv = psmouse->private;
	struct input_dev *dev2 = priv->dev2;

	if (report_buttons)
		alps_report_buttons(psmouse, dev2, psmouse->dev,
				packet[0] & 1, packet[0] & 2, packet[0] & 4);

	input_report_rel(dev2, REL_X,
		packet[1] ? packet[1] - ((packet[0] << 4) & 0x100) : 0);
	input_report_rel(dev2, REL_Y,
		packet[2] ? ((packet[0] << 3) & 0x100) - packet[2] : 0);

	input_sync(dev2);
}

static psmouse_ret_t alps_handle_interleaved_ps2(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;

	if (psmouse->pktcnt < 6)
		return PSMOUSE_GOOD_DATA;

	if (psmouse->pktcnt == 6) {
		/*
		 * Start a timer to flush the packet if it ends up last
		 * 6-byte packet in the stream. Timer needs to fire
		 * psmouse core times out itself. 20 ms should be enough
		 * to decide if we are getting more data or not.
		 */
		mod_timer(&priv->timer, jiffies + msecs_to_jiffies(20));
		return PSMOUSE_GOOD_DATA;
	}

	del_timer(&priv->timer);

	if (psmouse->packet[6] & 0x80) {

		/*
		 * Highest bit is set - that means we either had
		 * complete ALPS packet and this is start of the
		 * next packet or we got garbage.
		 */

		if (((psmouse->packet[3] |
		      psmouse->packet[4] |
		      psmouse->packet[5]) & 0x80) ||
<<<<<<< HEAD
		    (!alps_is_valid_first_byte(priv->i, psmouse->packet[6]))) {
<<<<<<< HEAD
			dbg("refusing packet %x %x %x %x "
			    "(suspected interleaved ps/2)\n",
			    psmouse->packet[3], psmouse->packet[4],
			    psmouse->packet[5], psmouse->packet[6]);
=======
			psmouse_dbg(psmouse,
				    "refusing packet %x %x %x %x (suspected interleaved ps/2)\n",
				    psmouse->packet[3], psmouse->packet[4],
				    psmouse->packet[5], psmouse->packet[6]);
>>>>>>> refs/remotes/origin/cm-10.0
			return PSMOUSE_BAD_DATA;
		}

		alps_process_packet(psmouse);
=======
		    (!alps_is_valid_first_byte(priv, psmouse->packet[6]))) {
			psmouse_dbg(psmouse,
				    "refusing packet %4ph (suspected interleaved ps/2)\n",
				    psmouse->packet + 3);
			return PSMOUSE_BAD_DATA;
		}

		priv->process_packet(psmouse);
>>>>>>> refs/remotes/origin/master

		/* Continue with the next packet */
		psmouse->packet[0] = psmouse->packet[6];
		psmouse->pktcnt = 1;

	} else {

		/*
		 * High bit is 0 - that means that we indeed got a PS/2
		 * packet in the middle of ALPS packet.
		 *
		 * There is also possibility that we got 6-byte ALPS
		 * packet followed  by 3-byte packet from trackpoint. We
		 * can not distinguish between these 2 scenarios but
<<<<<<< HEAD
<<<<<<< HEAD
		 * becase the latter is unlikely to happen in course of
=======
		 * because the latter is unlikely to happen in course of
>>>>>>> refs/remotes/origin/cm-10.0
=======
		 * because the latter is unlikely to happen in course of
>>>>>>> refs/remotes/origin/master
		 * normal operation (user would need to press all
		 * buttons on the pad and start moving trackpoint
		 * without touching the pad surface) we assume former.
		 * Even if we are wrong the wost thing that would happen
		 * the cursor would jump but we should not get protocol
<<<<<<< HEAD
<<<<<<< HEAD
		 * desynchronization.
=======
		 * de-synchronization.
>>>>>>> refs/remotes/origin/cm-10.0
=======
		 * de-synchronization.
>>>>>>> refs/remotes/origin/master
		 */

		alps_report_bare_ps2_packet(psmouse, &psmouse->packet[3],
					    false);

		/*
		 * Continue with the standard ALPS protocol handling,
		 * but make sure we won't process it as an interleaved
		 * packet again, which may happen if all buttons are
		 * pressed. To avoid this let's reset the 4th bit which
		 * is normally 1.
		 */
		psmouse->packet[3] = psmouse->packet[6] & 0xf7;
		psmouse->pktcnt = 4;
	}

	return PSMOUSE_GOOD_DATA;
}

static void alps_flush_packet(unsigned long data)
{
	struct psmouse *psmouse = (struct psmouse *)data;
<<<<<<< HEAD

	serio_pause_rx(psmouse->ps2dev.serio);

<<<<<<< HEAD
	if (psmouse->pktcnt == 6) {
=======
	if (psmouse->pktcnt == psmouse->pktsize) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct alps_data *priv = psmouse->private;

	serio_pause_rx(psmouse->ps2dev.serio);

	if (psmouse->pktcnt == psmouse->pktsize) {
>>>>>>> refs/remotes/origin/master

		/*
		 * We did not any more data in reasonable amount of time.
		 * Validate the last 3 bytes and process as a standard
		 * ALPS packet.
		 */
		if ((psmouse->packet[3] |
		     psmouse->packet[4] |
		     psmouse->packet[5]) & 0x80) {
<<<<<<< HEAD
<<<<<<< HEAD
			dbg("refusing packet %x %x %x "
			    "(suspected interleaved ps/2)\n",
			    psmouse->packet[3], psmouse->packet[4],
			    psmouse->packet[5]);
=======
			psmouse_dbg(psmouse,
				    "refusing packet %x %x %x (suspected interleaved ps/2)\n",
				    psmouse->packet[3], psmouse->packet[4],
				    psmouse->packet[5]);
>>>>>>> refs/remotes/origin/cm-10.0
		} else {
			alps_process_packet(psmouse);
=======
			psmouse_dbg(psmouse,
				    "refusing packet %3ph (suspected interleaved ps/2)\n",
				    psmouse->packet + 3);
		} else {
			priv->process_packet(psmouse);
>>>>>>> refs/remotes/origin/master
		}
		psmouse->pktcnt = 0;
	}

	serio_continue_rx(psmouse->ps2dev.serio);
}

static psmouse_ret_t alps_process_byte(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;
<<<<<<< HEAD
	const struct alps_model_info *model = priv->i;
=======
>>>>>>> refs/remotes/origin/master

	if ((psmouse->packet[0] & 0xc8) == 0x08) { /* PS/2 packet */
		if (psmouse->pktcnt == 3) {
			alps_report_bare_ps2_packet(psmouse, psmouse->packet,
						    true);
			return PSMOUSE_FULL_PACKET;
		}
		return PSMOUSE_GOOD_DATA;
	}

	/* Check for PS/2 packet stuffed in the middle of ALPS packet. */

<<<<<<< HEAD
	if ((model->flags & ALPS_PS2_INTERLEAVED) &&
=======
	if ((priv->flags & ALPS_PS2_INTERLEAVED) &&
>>>>>>> refs/remotes/origin/master
	    psmouse->pktcnt >= 4 && (psmouse->packet[3] & 0x0f) == 0x0f) {
		return alps_handle_interleaved_ps2(psmouse);
	}

<<<<<<< HEAD
	if (!alps_is_valid_first_byte(model, psmouse->packet[0])) {
<<<<<<< HEAD
		dbg("refusing packet[0] = %x (mask0 = %x, byte0 = %x)\n",
		    psmouse->packet[0], model->mask0, model->byte0);
		return PSMOUSE_BAD_DATA;
	}

	/* Bytes 2 - 6 should have 0 in the highest bit */
	if (psmouse->pktcnt >= 2 && psmouse->pktcnt <= 6 &&
	    (psmouse->packet[psmouse->pktcnt - 1] & 0x80)) {
		dbg("refusing packet[%i] = %x\n",
		    psmouse->pktcnt - 1, psmouse->packet[psmouse->pktcnt - 1]);
		return PSMOUSE_BAD_DATA;
	}

	if (psmouse->pktcnt == 6) {
=======
		psmouse_dbg(psmouse,
			    "refusing packet[0] = %x (mask0 = %x, byte0 = %x)\n",
			    psmouse->packet[0], model->mask0, model->byte0);
=======
	if (!alps_is_valid_first_byte(priv, psmouse->packet[0])) {
		psmouse_dbg(psmouse,
			    "refusing packet[0] = %x (mask0 = %x, byte0 = %x)\n",
			    psmouse->packet[0], priv->mask0, priv->byte0);
>>>>>>> refs/remotes/origin/master
		return PSMOUSE_BAD_DATA;
	}

	/* Bytes 2 - pktsize should have 0 in the highest bit */
<<<<<<< HEAD
	if (psmouse->pktcnt >= 2 && psmouse->pktcnt <= psmouse->pktsize &&
=======
	if ((priv->proto_version < ALPS_PROTO_V5) &&
	    psmouse->pktcnt >= 2 && psmouse->pktcnt <= psmouse->pktsize &&
>>>>>>> refs/remotes/origin/master
	    (psmouse->packet[psmouse->pktcnt - 1] & 0x80)) {
		psmouse_dbg(psmouse, "refusing packet[%i] = %x\n",
			    psmouse->pktcnt - 1,
			    psmouse->packet[psmouse->pktcnt - 1]);
		return PSMOUSE_BAD_DATA;
	}

	if (psmouse->pktcnt == psmouse->pktsize) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		alps_process_packet(psmouse);
=======
		priv->process_packet(psmouse);
>>>>>>> refs/remotes/origin/master
		return PSMOUSE_FULL_PACKET;
	}

	return PSMOUSE_GOOD_DATA;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static int alps_command_mode_send_nibble(struct psmouse *psmouse, int nibble)
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	struct alps_data *priv = psmouse->private;
	int command;
	unsigned char *param;
	unsigned char dummy[4];

	BUG_ON(nibble > 0xf);

	command = priv->nibble_commands[nibble].command;
	param = (command & 0x0f00) ?
		dummy : (unsigned char *)&priv->nibble_commands[nibble].data;

	if (ps2_command(ps2dev, param, command))
		return -1;

	return 0;
}

static int alps_command_mode_set_addr(struct psmouse *psmouse, int addr)
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	struct alps_data *priv = psmouse->private;
	int i, nibble;

	if (ps2_command(ps2dev, NULL, priv->addr_command))
		return -1;

	for (i = 12; i >= 0; i -= 4) {
		nibble = (addr >> i) & 0xf;
		if (alps_command_mode_send_nibble(psmouse, nibble))
			return -1;
	}

	return 0;
}

static int __alps_command_mode_read_reg(struct psmouse *psmouse, int addr)
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	unsigned char param[4];

	if (ps2_command(ps2dev, param, PSMOUSE_CMD_GETINFO))
		return -1;

	/*
	 * The address being read is returned in the first two bytes
	 * of the result. Check that this address matches the expected
	 * address.
	 */
	if (addr != ((param[0] << 8) | param[1]))
		return -1;

	return param[2];
}

static int alps_command_mode_read_reg(struct psmouse *psmouse, int addr)
{
	if (alps_command_mode_set_addr(psmouse, addr))
		return -1;
	return __alps_command_mode_read_reg(psmouse, addr);
}

static int __alps_command_mode_write_reg(struct psmouse *psmouse, u8 value)
{
	if (alps_command_mode_send_nibble(psmouse, (value >> 4) & 0xf))
		return -1;
	if (alps_command_mode_send_nibble(psmouse, value & 0xf))
		return -1;
	return 0;
}

static int alps_command_mode_write_reg(struct psmouse *psmouse, int addr,
				       u8 value)
{
	if (alps_command_mode_set_addr(psmouse, addr))
		return -1;
	return __alps_command_mode_write_reg(psmouse, value);
}

<<<<<<< HEAD
static int alps_enter_command_mode(struct psmouse *psmouse,
				   unsigned char *resp)
{
	unsigned char param[4];
	struct ps2dev *ps2dev = &psmouse->ps2dev;

	if (ps2_command(ps2dev, NULL, PSMOUSE_CMD_RESET_WRAP) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_RESET_WRAP) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_RESET_WRAP) ||
	    ps2_command(ps2dev, param, PSMOUSE_CMD_GETINFO)) {
=======
static int alps_rpt_cmd(struct psmouse *psmouse, int init_command,
			int repeated_command, unsigned char *param)
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;

	param[0] = 0;
	if (init_command && ps2_command(ps2dev, param, init_command))
		return -EIO;

	if (ps2_command(ps2dev,  NULL, repeated_command) ||
	    ps2_command(ps2dev,  NULL, repeated_command) ||
	    ps2_command(ps2dev,  NULL, repeated_command))
		return -EIO;

	param[0] = param[1] = param[2] = 0xff;
	if (ps2_command(ps2dev, param, PSMOUSE_CMD_GETINFO))
		return -EIO;

	psmouse_dbg(psmouse, "%2.2X report: %3ph\n",
		    repeated_command, param);
	return 0;
}

static int alps_enter_command_mode(struct psmouse *psmouse)
{
	unsigned char param[4];

	if (alps_rpt_cmd(psmouse, 0, PSMOUSE_CMD_RESET_WRAP, param)) {
>>>>>>> refs/remotes/origin/master
		psmouse_err(psmouse, "failed to enter command mode\n");
		return -1;
	}

<<<<<<< HEAD
	if (param[0] != 0x88 && param[1] != 0x07) {
		psmouse_dbg(psmouse,
			    "unknown response while entering command mode: %2.2x %2.2x %2.2x\n",
			    param[0], param[1], param[2]);
		return -1;
	}

	if (resp)
		*resp = param[2];
=======
	if ((param[0] != 0x88 || (param[1] != 0x07 && param[1] != 0x08)) &&
	    param[0] != 0x73) {
		psmouse_dbg(psmouse,
			    "unknown response while entering command mode\n");
		return -1;
	}
>>>>>>> refs/remotes/origin/master
	return 0;
}

static inline int alps_exit_command_mode(struct psmouse *psmouse)
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	if (ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSTREAM))
		return -1;
	return 0;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static const struct alps_model_info *alps_get_model(struct psmouse *psmouse, int *version)
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	static const unsigned char rates[] = { 0, 10, 20, 40, 60, 80, 100, 200 };
	unsigned char param[4];
<<<<<<< HEAD
=======
	const struct alps_model_info *model = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
	int i;

	/*
	 * First try "E6 report".
	 * ALPS should return 0,0,10 or 0,0,100 if no buttons are pressed.
	 * The bits 0-2 of the first byte will be 1s if some buttons are
	 * pressed.
	 */
	param[0] = 0;
	if (ps2_command(ps2dev, param, PSMOUSE_CMD_SETRES) ||
	    ps2_command(ps2dev,  NULL, PSMOUSE_CMD_SETSCALE11) ||
	    ps2_command(ps2dev,  NULL, PSMOUSE_CMD_SETSCALE11) ||
	    ps2_command(ps2dev,  NULL, PSMOUSE_CMD_SETSCALE11))
		return NULL;

	param[0] = param[1] = param[2] = 0xff;
	if (ps2_command(ps2dev, param, PSMOUSE_CMD_GETINFO))
		return NULL;

<<<<<<< HEAD
	dbg("E6 report: %2.2x %2.2x %2.2x", param[0], param[1], param[2]);
=======
	psmouse_dbg(psmouse, "E6 report: %2.2x %2.2x %2.2x",
		    param[0], param[1], param[2]);
>>>>>>> refs/remotes/origin/cm-10.0

	if ((param[0] & 0xf8) != 0 || param[1] != 0 ||
	    (param[2] != 10 && param[2] != 100))
		return NULL;

	/*
	 * Now try "E7 report". Allowed responses are in
	 * alps_model_data[].signature
	 */
	param[0] = 0;
	if (ps2_command(ps2dev, param, PSMOUSE_CMD_SETRES) ||
	    ps2_command(ps2dev,  NULL, PSMOUSE_CMD_SETSCALE21) ||
	    ps2_command(ps2dev,  NULL, PSMOUSE_CMD_SETSCALE21) ||
	    ps2_command(ps2dev,  NULL, PSMOUSE_CMD_SETSCALE21))
		return NULL;

	param[0] = param[1] = param[2] = 0xff;
	if (ps2_command(ps2dev, param, PSMOUSE_CMD_GETINFO))
		return NULL;

<<<<<<< HEAD
	dbg("E7 report: %2.2x %2.2x %2.2x", param[0], param[1], param[2]);
=======
	psmouse_dbg(psmouse, "E7 report: %2.2x %2.2x %2.2x",
		    param[0], param[1], param[2]);
>>>>>>> refs/remotes/origin/cm-10.0

	if (version) {
		for (i = 0; i < ARRAY_SIZE(rates) && param[2] != rates[i]; i++)
			/* empty */;
		*version = (param[0] << 8) | (param[1] << 4) | i;
	}

<<<<<<< HEAD
	for (i = 0; i < ARRAY_SIZE(alps_model_data); i++)
		if (!memcmp(param, alps_model_data[i].signature,
			    sizeof(alps_model_data[i].signature)))
			return alps_model_data + i;

	return NULL;
=======
	for (i = 0; i < ARRAY_SIZE(alps_model_data); i++) {
		if (!memcmp(param, alps_model_data[i].signature,
			    sizeof(alps_model_data[i].signature))) {
			model = alps_model_data + i;
			break;
		}
	}

	if (model && model->proto_version > ALPS_PROTO_V2) {
		/*
		 * Need to check command mode response to identify
		 * model
		 */
		model = NULL;
		if (alps_enter_command_mode(psmouse, param)) {
			psmouse_warn(psmouse,
				     "touchpad failed to enter command mode\n");
		} else {
			for (i = 0; i < ARRAY_SIZE(alps_model_data); i++) {
				if (alps_model_data[i].proto_version > ALPS_PROTO_V2 &&
				    alps_model_data[i].command_mode_resp == param[0]) {
					model = alps_model_data + i;
					break;
				}
			}
			alps_exit_command_mode(psmouse);

			if (!model)
				psmouse_dbg(psmouse,
					    "Unknown command mode response %2.2x\n",
					    param[0]);
		}
	}

	return model;
>>>>>>> refs/remotes/origin/cm-10.0
}

=======
>>>>>>> refs/remotes/origin/master
/*
 * For DualPoint devices select the device that should respond to
 * subsequent commands. It looks like glidepad is behind stickpointer,
 * I'd thought it would be other way around...
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int alps_passthrough_mode(struct psmouse *psmouse, bool enable)
=======
static int alps_passthrough_mode_v2(struct psmouse *psmouse, bool enable)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int alps_passthrough_mode_v2(struct psmouse *psmouse, bool enable)
>>>>>>> refs/remotes/origin/master
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	int cmd = enable ? PSMOUSE_CMD_SETSCALE21 : PSMOUSE_CMD_SETSCALE11;

	if (ps2_command(ps2dev, NULL, cmd) ||
	    ps2_command(ps2dev, NULL, cmd) ||
	    ps2_command(ps2dev, NULL, cmd) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_DISABLE))
		return -1;

	/* we may get 3 more bytes, just ignore them */
	ps2_drain(ps2dev, 3, 100);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int alps_absolute_mode(struct psmouse *psmouse)
=======
static int alps_absolute_mode_v1_v2(struct psmouse *psmouse)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int alps_absolute_mode_v1_v2(struct psmouse *psmouse)
>>>>>>> refs/remotes/origin/master
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;

	/* Try ALPS magic knock - 4 disable before enable */
	if (ps2_command(ps2dev, NULL, PSMOUSE_CMD_DISABLE) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_DISABLE) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_DISABLE) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_DISABLE) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_ENABLE))
		return -1;

	/*
	 * Switch mouse to poll (remote) mode so motion data will not
	 * get in our way
	 */
	return ps2_command(&psmouse->ps2dev, NULL, PSMOUSE_CMD_SETPOLL);
}

<<<<<<< HEAD
static int alps_get_status(struct psmouse *psmouse, char *param)
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;

	/* Get status: 0xF5 0xF5 0xF5 0xE9 */
	if (ps2_command(ps2dev, NULL, PSMOUSE_CMD_DISABLE) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_DISABLE) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_DISABLE) ||
	    ps2_command(ps2dev, param, PSMOUSE_CMD_GETINFO))
		return -1;

<<<<<<< HEAD
	dbg("Status: %2.2x %2.2x %2.2x", param[0], param[1], param[2]);
=======
	psmouse_dbg(psmouse, "Status: %2.2x %2.2x %2.2x",
		    param[0], param[1], param[2]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int alps_monitor_mode_send_word(struct psmouse *psmouse, u16 word)
{
	int i, nibble;

	/*
	 * b0-b11 are valid bits, send sequence is inverse.
	 * e.g. when word = 0x0123, nibble send sequence is 3, 2, 1
	 */
	for (i = 0; i <= 8; i += 4) {
		nibble = (word >> i) & 0xf;
		if (alps_command_mode_send_nibble(psmouse, nibble))
			return -1;
	}

	return 0;
}

static int alps_monitor_mode_write_reg(struct psmouse *psmouse,
				       u16 addr, u16 value)
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;

	/* 0x0A0 is the command to write the word */
	if (ps2_command(ps2dev, NULL, PSMOUSE_CMD_ENABLE) ||
	    alps_monitor_mode_send_word(psmouse, 0x0A0) ||
	    alps_monitor_mode_send_word(psmouse, addr) ||
	    alps_monitor_mode_send_word(psmouse, value) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_DISABLE))
		return -1;

	return 0;
}

static int alps_monitor_mode(struct psmouse *psmouse, bool enable)
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;

	if (enable) {
		/* EC E9 F5 F5 E7 E6 E7 E9 to enter monitor mode */
		if (ps2_command(ps2dev, NULL, PSMOUSE_CMD_RESET_WRAP) ||
		    ps2_command(ps2dev, NULL, PSMOUSE_CMD_GETINFO) ||
		    ps2_command(ps2dev, NULL, PSMOUSE_CMD_DISABLE) ||
		    ps2_command(ps2dev, NULL, PSMOUSE_CMD_DISABLE) ||
		    ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSCALE21) ||
		    ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSCALE11) ||
		    ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSCALE21) ||
		    ps2_command(ps2dev, NULL, PSMOUSE_CMD_GETINFO))
			return -1;
	} else {
		/* EC to exit monitor mode */
		if (ps2_command(ps2dev, NULL, PSMOUSE_CMD_RESET_WRAP))
			return -1;
	}

	return 0;
}

static int alps_absolute_mode_v6(struct psmouse *psmouse)
{
	u16 reg_val = 0x181;
	int ret = -1;

	/* enter monitor mode, to write the register */
	if (alps_monitor_mode(psmouse, true))
		return -1;

	ret = alps_monitor_mode_write_reg(psmouse, 0x000, reg_val);

	if (alps_monitor_mode(psmouse, false))
		ret = -1;

	return ret;
}

static int alps_get_status(struct psmouse *psmouse, char *param)
{
	/* Get status: 0xF5 0xF5 0xF5 0xE9 */
	if (alps_rpt_cmd(psmouse, 0, PSMOUSE_CMD_DISABLE, param))
		return -1;
>>>>>>> refs/remotes/origin/master

	return 0;
}

/*
 * Turn touchpad tapping on or off. The sequences are:
 * 0xE9 0xF5 0xF5 0xF3 0x0A to enable,
 * 0xE9 0xF5 0xF5 0xE8 0x00 to disable.
 * My guess that 0xE9 (GetInfo) is here as a sync point.
 * For models that also have stickpointer (DualPoints) its tapping
 * is controlled separately (0xE6 0xE6 0xE6 0xF3 0x14|0x0A) but
 * we don't fiddle with it.
 */
static int alps_tap_mode(struct psmouse *psmouse, int enable)
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	int cmd = enable ? PSMOUSE_CMD_SETRATE : PSMOUSE_CMD_SETRES;
	unsigned char tap_arg = enable ? 0x0A : 0x00;
	unsigned char param[4];

	if (ps2_command(ps2dev, param, PSMOUSE_CMD_GETINFO) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_DISABLE) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_DISABLE) ||
	    ps2_command(ps2dev, &tap_arg, cmd))
		return -1;

	if (alps_get_status(psmouse, param))
		return -1;

	return 0;
}

/*
 * alps_poll() - poll the touchpad for current motion packet.
 * Used in resync.
 */
static int alps_poll(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned char buf[6];
	bool poll_failed;

	if (priv->i->flags & ALPS_PASS)
		alps_passthrough_mode(psmouse, true);
=======
	unsigned char buf[sizeof(psmouse->packet)];
	bool poll_failed;

	if (priv->i->flags & ALPS_PASS)
		alps_passthrough_mode_v2(psmouse, true);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned char buf[sizeof(psmouse->packet)];
	bool poll_failed;

	if (priv->flags & ALPS_PASS)
		alps_passthrough_mode_v2(psmouse, true);
>>>>>>> refs/remotes/origin/master

	poll_failed = ps2_command(&psmouse->ps2dev, buf,
				  PSMOUSE_CMD_POLL | (psmouse->pktsize << 8)) < 0;

<<<<<<< HEAD
	if (priv->i->flags & ALPS_PASS)
<<<<<<< HEAD
		alps_passthrough_mode(psmouse, false);
=======
		alps_passthrough_mode_v2(psmouse, false);
>>>>>>> refs/remotes/origin/cm-10.0

	if (poll_failed || (buf[0] & priv->i->mask0) != priv->i->byte0)
=======
	if (priv->flags & ALPS_PASS)
		alps_passthrough_mode_v2(psmouse, false);

	if (poll_failed || (buf[0] & priv->mask0) != priv->byte0)
>>>>>>> refs/remotes/origin/master
		return -1;

	if ((psmouse->badbyte & 0xc8) == 0x08) {
/*
 * Poll the track stick ...
 */
		if (ps2_command(&psmouse->ps2dev, buf, PSMOUSE_CMD_POLL | (3 << 8)))
			return -1;
	}

	memcpy(psmouse->packet, buf, sizeof(buf));
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int alps_hw_init(struct psmouse *psmouse)
=======
static int alps_hw_init_v1_v2(struct psmouse *psmouse)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct alps_data *priv = psmouse->private;
	const struct alps_model_info *model = priv->i;

	if ((model->flags & ALPS_PASS) &&
<<<<<<< HEAD
	    alps_passthrough_mode(psmouse, true)) {
=======
	    alps_passthrough_mode_v2(psmouse, true)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int alps_hw_init_v1_v2(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;

	if ((priv->flags & ALPS_PASS) &&
	    alps_passthrough_mode_v2(psmouse, true)) {
>>>>>>> refs/remotes/origin/master
		return -1;
	}

	if (alps_tap_mode(psmouse, true)) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_WARNING "alps.c: Failed to enable hardware tapping\n");
		return -1;
	}

	if (alps_absolute_mode(psmouse)) {
		printk(KERN_ERR "alps.c: Failed to enable absolute mode\n");
=======
=======
>>>>>>> refs/remotes/origin/master
		psmouse_warn(psmouse, "Failed to enable hardware tapping\n");
		return -1;
	}

	if (alps_absolute_mode_v1_v2(psmouse)) {
		psmouse_err(psmouse, "Failed to enable absolute mode\n");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		return -1;
	}

	if ((model->flags & ALPS_PASS) &&
<<<<<<< HEAD
	    alps_passthrough_mode(psmouse, false)) {
=======
	    alps_passthrough_mode_v2(psmouse, false)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return -1;
	}

	if ((priv->flags & ALPS_PASS) &&
	    alps_passthrough_mode_v2(psmouse, false)) {
>>>>>>> refs/remotes/origin/master
		return -1;
	}

	/* ALPS needs stream mode, otherwise it won't report any data */
	if (ps2_command(&psmouse->ps2dev, NULL, PSMOUSE_CMD_SETSTREAM)) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "alps.c: Failed to enable stream mode\n");
=======
		psmouse_err(psmouse, "Failed to enable stream mode\n");
>>>>>>> refs/remotes/origin/master
		return -1;
	}

	return 0;
}

<<<<<<< HEAD
=======
		psmouse_err(psmouse, "Failed to enable stream mode\n");
=======
static int alps_hw_init_v6(struct psmouse *psmouse)
{
	unsigned char param[2] = {0xC8, 0x14};

	/* Enter passthrough mode to let trackpoint enter 6byte raw mode */
	if (alps_passthrough_mode_v2(psmouse, true))
		return -1;

	if (ps2_command(&psmouse->ps2dev, NULL, PSMOUSE_CMD_SETSCALE11) ||
	    ps2_command(&psmouse->ps2dev, NULL, PSMOUSE_CMD_SETSCALE11) ||
	    ps2_command(&psmouse->ps2dev, NULL, PSMOUSE_CMD_SETSCALE11) ||
	    ps2_command(&psmouse->ps2dev, &param[0], PSMOUSE_CMD_SETRATE) ||
	    ps2_command(&psmouse->ps2dev, &param[1], PSMOUSE_CMD_SETRATE))
		return -1;

	if (alps_passthrough_mode_v2(psmouse, false))
		return -1;

	if (alps_absolute_mode_v6(psmouse)) {
		psmouse_err(psmouse, "Failed to enable absolute mode\n");
>>>>>>> refs/remotes/origin/master
		return -1;
	}

	return 0;
}

/*
<<<<<<< HEAD
 * Enable or disable passthrough mode to the trackstick. Must be in
 * command mode when calling this function.
 */
static int alps_passthrough_mode_v3(struct psmouse *psmouse, bool enable)
{
	int reg_val;

	reg_val = alps_command_mode_read_reg(psmouse, 0x0008);
	if (reg_val == -1)
		return -1;

=======
 * Enable or disable passthrough mode to the trackstick.
 */
static int alps_passthrough_mode_v3(struct psmouse *psmouse,
				    int reg_base, bool enable)
{
	int reg_val, ret = -1;

	if (alps_enter_command_mode(psmouse))
		return -1;

	reg_val = alps_command_mode_read_reg(psmouse, reg_base + 0x0008);
	if (reg_val == -1)
		goto error;

>>>>>>> refs/remotes/origin/master
	if (enable)
		reg_val |= 0x01;
	else
		reg_val &= ~0x01;

<<<<<<< HEAD
	if (__alps_command_mode_write_reg(psmouse, reg_val))
		return -1;

	return 0;
=======
	ret = __alps_command_mode_write_reg(psmouse, reg_val);

error:
	if (alps_exit_command_mode(psmouse))
		ret = -1;
	return ret;
>>>>>>> refs/remotes/origin/master
}

/* Must be in command mode when calling this function */
static int alps_absolute_mode_v3(struct psmouse *psmouse)
{
	int reg_val;

	reg_val = alps_command_mode_read_reg(psmouse, 0x0004);
	if (reg_val == -1)
		return -1;

	reg_val |= 0x06;
	if (__alps_command_mode_write_reg(psmouse, reg_val))
		return -1;

	return 0;
}

<<<<<<< HEAD
static int alps_hw_init_v3(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	int reg_val;
	unsigned char param[4];

	priv->nibble_commands = alps_v3_nibble_commands;
	priv->addr_command = PSMOUSE_CMD_RESET_WRAP;

	if (alps_enter_command_mode(psmouse, NULL))
		goto error;

	/* Check for trackstick */
	reg_val = alps_command_mode_read_reg(psmouse, 0x0008);
	if (reg_val == -1)
		goto error;
	if (reg_val & 0x80) {
		if (alps_passthrough_mode_v3(psmouse, true))
			goto error;
		if (alps_exit_command_mode(psmouse))
			goto error;

		/*
		 * E7 report for the trackstick
		 *
		 * There have been reports of failures to seem to trace back
		 * to the above trackstick check failing. When these occur
		 * this E7 report fails, so when that happens we continue
		 * with the assumption that there isn't a trackstick after
		 * all.
		 */
		param[0] = 0x64;
		if (ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSCALE21) ||
		    ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSCALE21) ||
		    ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSCALE21) ||
		    ps2_command(ps2dev, param, PSMOUSE_CMD_GETINFO)) {
			psmouse_warn(psmouse, "trackstick E7 report failed\n");
		} else {
			psmouse_dbg(psmouse,
				    "trackstick E7 report: %2.2x %2.2x %2.2x\n",
				    param[0], param[1], param[2]);

			/*
			 * Not sure what this does, but it is absolutely
			 * essential. Without it, the touchpad does not
			 * work at all and the trackstick just emits normal
			 * PS/2 packets.
			 */
			if (ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSCALE11) ||
			    ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSCALE11) ||
			    ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSCALE11) ||
			    alps_command_mode_send_nibble(psmouse, 0x9) ||
			    alps_command_mode_send_nibble(psmouse, 0x4)) {
				psmouse_err(psmouse,
					    "Error sending magic E6 sequence\n");
				goto error_passthrough;
			}
		}

		if (alps_enter_command_mode(psmouse, NULL))
			goto error_passthrough;
		if (alps_passthrough_mode_v3(psmouse, false))
			goto error;
	}

	if (alps_absolute_mode_v3(psmouse)) {
=======
static int alps_probe_trackstick_v3(struct psmouse *psmouse, int reg_base)
{
	int ret = -EIO, reg_val;

	if (alps_enter_command_mode(psmouse))
		goto error;

	reg_val = alps_command_mode_read_reg(psmouse, reg_base + 0x08);
	if (reg_val == -1)
		goto error;

	/* bit 7: trackstick is present */
	ret = reg_val & 0x80 ? 0 : -ENODEV;

error:
	alps_exit_command_mode(psmouse);
	return ret;
}

static int alps_setup_trackstick_v3(struct psmouse *psmouse, int reg_base)
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	int ret = 0;
	unsigned char param[4];

	if (alps_passthrough_mode_v3(psmouse, reg_base, true))
		return -EIO;

	/*
	 * E7 report for the trackstick
	 *
	 * There have been reports of failures to seem to trace back
	 * to the above trackstick check failing. When these occur
	 * this E7 report fails, so when that happens we continue
	 * with the assumption that there isn't a trackstick after
	 * all.
	 */
	if (alps_rpt_cmd(psmouse, 0, PSMOUSE_CMD_SETSCALE21, param)) {
		psmouse_warn(psmouse, "trackstick E7 report failed\n");
		ret = -ENODEV;
	} else {
		psmouse_dbg(psmouse, "trackstick E7 report: %3ph\n", param);

		/*
		 * Not sure what this does, but it is absolutely
		 * essential. Without it, the touchpad does not
		 * work at all and the trackstick just emits normal
		 * PS/2 packets.
		 */
		if (ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSCALE11) ||
		    ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSCALE11) ||
		    ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSCALE11) ||
		    alps_command_mode_send_nibble(psmouse, 0x9) ||
		    alps_command_mode_send_nibble(psmouse, 0x4)) {
			psmouse_err(psmouse,
				    "Error sending magic E6 sequence\n");
			ret = -EIO;
			goto error;
		}

		/*
		 * This ensures the trackstick packets are in the format
		 * supported by this driver. If bit 1 isn't set the packet
		 * format is different.
		 */
		if (alps_enter_command_mode(psmouse) ||
		    alps_command_mode_write_reg(psmouse,
						reg_base + 0x08, 0x82) ||
		    alps_exit_command_mode(psmouse))
			ret = -EIO;
	}

error:
	if (alps_passthrough_mode_v3(psmouse, reg_base, false))
		ret = -EIO;

	return ret;
}

static int alps_hw_init_v3(struct psmouse *psmouse)
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	int reg_val;
	unsigned char param[4];

	reg_val = alps_probe_trackstick_v3(psmouse, ALPS_REG_BASE_PINNACLE);
	if (reg_val == -EIO)
		goto error;

	if (reg_val == 0 &&
	    alps_setup_trackstick_v3(psmouse, ALPS_REG_BASE_PINNACLE) == -EIO)
		goto error;

	if (alps_enter_command_mode(psmouse) ||
	    alps_absolute_mode_v3(psmouse)) {
>>>>>>> refs/remotes/origin/master
		psmouse_err(psmouse, "Failed to enter absolute mode\n");
		goto error;
	}

	reg_val = alps_command_mode_read_reg(psmouse, 0x0006);
	if (reg_val == -1)
		goto error;
	if (__alps_command_mode_write_reg(psmouse, reg_val | 0x01))
		goto error;

	reg_val = alps_command_mode_read_reg(psmouse, 0x0007);
	if (reg_val == -1)
		goto error;
	if (__alps_command_mode_write_reg(psmouse, reg_val | 0x01))
		goto error;

	if (alps_command_mode_read_reg(psmouse, 0x0144) == -1)
		goto error;
	if (__alps_command_mode_write_reg(psmouse, 0x04))
		goto error;

	if (alps_command_mode_read_reg(psmouse, 0x0159) == -1)
		goto error;
	if (__alps_command_mode_write_reg(psmouse, 0x03))
		goto error;

	if (alps_command_mode_read_reg(psmouse, 0x0163) == -1)
		goto error;
	if (alps_command_mode_write_reg(psmouse, 0x0163, 0x03))
		goto error;

	if (alps_command_mode_read_reg(psmouse, 0x0162) == -1)
		goto error;
	if (alps_command_mode_write_reg(psmouse, 0x0162, 0x04))
		goto error;

<<<<<<< HEAD
	/*
	 * This ensures the trackstick packets are in the format
	 * supported by this driver. If bit 1 isn't set the packet
	 * format is different.
	 */
	if (alps_command_mode_write_reg(psmouse, 0x0008, 0x82))
		goto error;

=======
>>>>>>> refs/remotes/origin/master
	alps_exit_command_mode(psmouse);

	/* Set rate and enable data reporting */
	param[0] = 0x64;
	if (ps2_command(ps2dev, param, PSMOUSE_CMD_SETRATE) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_ENABLE)) {
		psmouse_err(psmouse, "Failed to enable data reporting\n");
		return -1;
	}

	return 0;

<<<<<<< HEAD
error_passthrough:
	/* Something failed while in passthrough mode, so try to get out */
	if (!alps_enter_command_mode(psmouse, NULL))
		alps_passthrough_mode_v3(psmouse, false);
=======
>>>>>>> refs/remotes/origin/master
error:
	/*
	 * Leaving the touchpad in command mode will essentially render
	 * it unusable until the machine reboots, so exit it here just
	 * to be safe
	 */
	alps_exit_command_mode(psmouse);
	return -1;
}

<<<<<<< HEAD
=======
static int alps_hw_init_rushmore_v3(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	int reg_val, ret = -1;

	if (priv->flags & ALPS_DUALPOINT) {
		reg_val = alps_setup_trackstick_v3(psmouse,
						   ALPS_REG_BASE_RUSHMORE);
		if (reg_val == -EIO)
			goto error;
		if (reg_val == -ENODEV)
			priv->flags &= ~ALPS_DUALPOINT;
	}

	if (alps_enter_command_mode(psmouse) ||
	    alps_command_mode_read_reg(psmouse, 0xc2d9) == -1 ||
	    alps_command_mode_write_reg(psmouse, 0xc2cb, 0x00))
		goto error;

	reg_val = alps_command_mode_read_reg(psmouse, 0xc2c6);
	if (reg_val == -1)
		goto error;
	if (__alps_command_mode_write_reg(psmouse, reg_val & 0xfd))
		goto error;

	if (alps_command_mode_write_reg(psmouse, 0xc2c9, 0x64))
		goto error;

	/* enter absolute mode */
	reg_val = alps_command_mode_read_reg(psmouse, 0xc2c4);
	if (reg_val == -1)
		goto error;
	if (__alps_command_mode_write_reg(psmouse, reg_val | 0x02))
		goto error;

	alps_exit_command_mode(psmouse);
	return ps2_command(ps2dev, NULL, PSMOUSE_CMD_ENABLE);

error:
	alps_exit_command_mode(psmouse);
	return ret;
}

>>>>>>> refs/remotes/origin/master
/* Must be in command mode when calling this function */
static int alps_absolute_mode_v4(struct psmouse *psmouse)
{
	int reg_val;

	reg_val = alps_command_mode_read_reg(psmouse, 0x0004);
	if (reg_val == -1)
		return -1;

	reg_val |= 0x02;
	if (__alps_command_mode_write_reg(psmouse, reg_val))
		return -1;

	return 0;
}

static int alps_hw_init_v4(struct psmouse *psmouse)
{
<<<<<<< HEAD
	struct alps_data *priv = psmouse->private;
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	unsigned char param[4];

	priv->nibble_commands = alps_v4_nibble_commands;
	priv->addr_command = PSMOUSE_CMD_DISABLE;

	if (alps_enter_command_mode(psmouse, NULL))
=======
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	unsigned char param[4];

	if (alps_enter_command_mode(psmouse))
>>>>>>> refs/remotes/origin/master
		goto error;

	if (alps_absolute_mode_v4(psmouse)) {
		psmouse_err(psmouse, "Failed to enter absolute mode\n");
		goto error;
	}

	if (alps_command_mode_write_reg(psmouse, 0x0007, 0x8c))
		goto error;

	if (alps_command_mode_write_reg(psmouse, 0x0149, 0x03))
		goto error;

	if (alps_command_mode_write_reg(psmouse, 0x0160, 0x03))
		goto error;

	if (alps_command_mode_write_reg(psmouse, 0x017f, 0x15))
		goto error;

	if (alps_command_mode_write_reg(psmouse, 0x0151, 0x01))
		goto error;

	if (alps_command_mode_write_reg(psmouse, 0x0168, 0x03))
		goto error;

	if (alps_command_mode_write_reg(psmouse, 0x014a, 0x03))
		goto error;

	if (alps_command_mode_write_reg(psmouse, 0x0161, 0x03))
		goto error;

	alps_exit_command_mode(psmouse);

	/*
	 * This sequence changes the output from a 9-byte to an
	 * 8-byte format. All the same data seems to be present,
	 * just in a more compact format.
	 */
	param[0] = 0xc8;
	param[1] = 0x64;
	param[2] = 0x50;
	if (ps2_command(ps2dev, &param[0], PSMOUSE_CMD_SETRATE) ||
	    ps2_command(ps2dev, &param[1], PSMOUSE_CMD_SETRATE) ||
	    ps2_command(ps2dev, &param[2], PSMOUSE_CMD_SETRATE) ||
	    ps2_command(ps2dev, param, PSMOUSE_CMD_GETID))
		return -1;

	/* Set rate and enable data reporting */
	param[0] = 0x64;
	if (ps2_command(ps2dev, param, PSMOUSE_CMD_SETRATE) ||
	    ps2_command(ps2dev, NULL, PSMOUSE_CMD_ENABLE)) {
		psmouse_err(psmouse, "Failed to enable data reporting\n");
		return -1;
	}

	return 0;

error:
	/*
	 * Leaving the touchpad in command mode will essentially render
	 * it unusable until the machine reboots, so exit it here just
	 * to be safe
	 */
	alps_exit_command_mode(psmouse);
	return -1;
}

<<<<<<< HEAD
static int alps_hw_init(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;
	const struct alps_model_info *model = priv->i;
	int ret = -1;

	switch (model->proto_version) {
	case ALPS_PROTO_V1:
	case ALPS_PROTO_V2:
		ret = alps_hw_init_v1_v2(psmouse);
		break;
	case ALPS_PROTO_V3:
		ret = alps_hw_init_v3(psmouse);
		break;
	case ALPS_PROTO_V4:
		ret = alps_hw_init_v4(psmouse);
		break;
	}

	return ret;
}

>>>>>>> refs/remotes/origin/cm-10.0
static int alps_reconnect(struct psmouse *psmouse)
{
	const struct alps_model_info *model;

	psmouse_reset(psmouse);

	model = alps_get_model(psmouse, NULL);
	if (!model)
		return -1;

	return alps_hw_init(psmouse);
=======
static int alps_hw_init_dolphin_v1(struct psmouse *psmouse)
{
	struct ps2dev *ps2dev = &psmouse->ps2dev;
	unsigned char param[2];

	/* This is dolphin "v1" as empirically defined by florin9doi */
	param[0] = 0x64;
	param[1] = 0x28;

	if (ps2_command(ps2dev, NULL, PSMOUSE_CMD_SETSTREAM) ||
	    ps2_command(ps2dev, &param[0], PSMOUSE_CMD_SETRATE) ||
	    ps2_command(ps2dev, &param[1], PSMOUSE_CMD_SETRATE))
		return -1;

	return 0;
}

static void alps_set_defaults(struct alps_data *priv)
{
	priv->byte0 = 0x8f;
	priv->mask0 = 0x8f;
	priv->flags = ALPS_DUALPOINT;

	priv->x_max = 2000;
	priv->y_max = 1400;
	priv->x_bits = 15;
	priv->y_bits = 11;

	switch (priv->proto_version) {
	case ALPS_PROTO_V1:
	case ALPS_PROTO_V2:
		priv->hw_init = alps_hw_init_v1_v2;
		priv->process_packet = alps_process_packet_v1_v2;
		priv->set_abs_params = alps_set_abs_params_st;
		priv->x_max = 1023;
		priv->y_max = 767;
		break;
	case ALPS_PROTO_V3:
		priv->hw_init = alps_hw_init_v3;
		priv->process_packet = alps_process_packet_v3;
		priv->set_abs_params = alps_set_abs_params_mt;
		priv->decode_fields = alps_decode_pinnacle;
		priv->nibble_commands = alps_v3_nibble_commands;
		priv->addr_command = PSMOUSE_CMD_RESET_WRAP;
		break;
	case ALPS_PROTO_V4:
		priv->hw_init = alps_hw_init_v4;
		priv->process_packet = alps_process_packet_v4;
		priv->set_abs_params = alps_set_abs_params_mt;
		priv->nibble_commands = alps_v4_nibble_commands;
		priv->addr_command = PSMOUSE_CMD_DISABLE;
		break;
	case ALPS_PROTO_V5:
		priv->hw_init = alps_hw_init_dolphin_v1;
		priv->process_packet = alps_process_packet_v3;
		priv->decode_fields = alps_decode_dolphin;
		priv->set_abs_params = alps_set_abs_params_mt;
		priv->nibble_commands = alps_v3_nibble_commands;
		priv->addr_command = PSMOUSE_CMD_RESET_WRAP;
		priv->byte0 = 0xc8;
		priv->mask0 = 0xc8;
		priv->flags = 0;
		priv->x_max = 1360;
		priv->y_max = 660;
		priv->x_bits = 23;
		priv->y_bits = 12;
		break;
	case ALPS_PROTO_V6:
		priv->hw_init = alps_hw_init_v6;
		priv->process_packet = alps_process_packet_v6;
		priv->set_abs_params = alps_set_abs_params_st;
		priv->nibble_commands = alps_v6_nibble_commands;
		priv->x_max = 2047;
		priv->y_max = 1535;
		break;
	}
}

static int alps_match_table(struct psmouse *psmouse, struct alps_data *priv,
			    unsigned char *e7, unsigned char *ec)
{
	const struct alps_model_info *model;
	int i;

	for (i = 0; i < ARRAY_SIZE(alps_model_data); i++) {
		model = &alps_model_data[i];

		if (!memcmp(e7, model->signature, sizeof(model->signature)) &&
		    (!model->command_mode_resp ||
		     model->command_mode_resp == ec[2])) {

			priv->proto_version = model->proto_version;
			alps_set_defaults(priv);

			priv->flags = model->flags;
			priv->byte0 = model->byte0;
			priv->mask0 = model->mask0;

			return 0;
		}
	}

	return -EINVAL;
}

static int alps_identify(struct psmouse *psmouse, struct alps_data *priv)
{
	unsigned char e6[4], e7[4], ec[4];

	/*
	 * First try "E6 report".
	 * ALPS should return 0,0,10 or 0,0,100 if no buttons are pressed.
	 * The bits 0-2 of the first byte will be 1s if some buttons are
	 * pressed.
	 */
	if (alps_rpt_cmd(psmouse, PSMOUSE_CMD_SETRES,
			 PSMOUSE_CMD_SETSCALE11, e6))
		return -EIO;

	if ((e6[0] & 0xf8) != 0 || e6[1] != 0 || (e6[2] != 10 && e6[2] != 100))
		return -EINVAL;

	/*
	 * Now get the "E7" and "EC" reports.  These will uniquely identify
	 * most ALPS touchpads.
	 */
	if (alps_rpt_cmd(psmouse, PSMOUSE_CMD_SETRES,
			 PSMOUSE_CMD_SETSCALE21, e7) ||
	    alps_rpt_cmd(psmouse, PSMOUSE_CMD_SETRES,
			 PSMOUSE_CMD_RESET_WRAP, ec) ||
	    alps_exit_command_mode(psmouse))
		return -EIO;

	if (alps_match_table(psmouse, priv, e7, ec) == 0) {
		return 0;
	} else if (e7[0] == 0x73 && e7[1] == 0x03 && e7[2] == 0x50 &&
		   ec[0] == 0x73 && ec[1] == 0x01) {
		priv->proto_version = ALPS_PROTO_V5;
		alps_set_defaults(priv);

		return 0;
	} else if (ec[0] == 0x88 && ec[1] == 0x08) {
		priv->proto_version = ALPS_PROTO_V3;
		alps_set_defaults(priv);

		priv->hw_init = alps_hw_init_rushmore_v3;
		priv->decode_fields = alps_decode_rushmore;
		priv->x_bits = 16;
		priv->y_bits = 12;

		/* hack to make addr_command, nibble_command available */
		psmouse->private = priv;

		if (alps_probe_trackstick_v3(psmouse, ALPS_REG_BASE_RUSHMORE))
			priv->flags &= ~ALPS_DUALPOINT;

		return 0;
	} else if (ec[0] == 0x88 && ec[1] == 0x07 &&
		   ec[2] >= 0x90 && ec[2] <= 0x9d) {
		priv->proto_version = ALPS_PROTO_V3;
		alps_set_defaults(priv);

		return 0;
	}

	psmouse_info(psmouse,
		     "Unknown ALPS touchpad: E7=%3ph, EC=%3ph\n", e7, ec);

	return -EINVAL;
}

static int alps_reconnect(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;

	psmouse_reset(psmouse);

	if (alps_identify(psmouse, priv) < 0)
		return -1;

	return priv->hw_init(psmouse);
>>>>>>> refs/remotes/origin/master
}

static void alps_disconnect(struct psmouse *psmouse)
{
	struct alps_data *priv = psmouse->private;

	psmouse_reset(psmouse);
	del_timer_sync(&priv->timer);
	input_unregister_device(priv->dev2);
	kfree(priv);
}

<<<<<<< HEAD
int alps_init(struct psmouse *psmouse)
{
	struct alps_data *priv;
	const struct alps_model_info *model;
	struct input_dev *dev1 = psmouse->dev, *dev2;
	int version;
=======
static void alps_set_abs_params_st(struct alps_data *priv,
				   struct input_dev *dev1)
{
	input_set_abs_params(dev1, ABS_X, 0, priv->x_max, 0, 0);
	input_set_abs_params(dev1, ABS_Y, 0, priv->y_max, 0, 0);
}

static void alps_set_abs_params_mt(struct alps_data *priv,
				   struct input_dev *dev1)
{
	set_bit(INPUT_PROP_SEMI_MT, dev1->propbit);
	input_mt_init_slots(dev1, 2, 0);
	input_set_abs_params(dev1, ABS_MT_POSITION_X, 0, priv->x_max, 0, 0);
	input_set_abs_params(dev1, ABS_MT_POSITION_Y, 0, priv->y_max, 0, 0);

	set_bit(BTN_TOOL_DOUBLETAP, dev1->keybit);
	set_bit(BTN_TOOL_TRIPLETAP, dev1->keybit);
	set_bit(BTN_TOOL_QUADTAP, dev1->keybit);

	input_set_abs_params(dev1, ABS_X, 0, priv->x_max, 0, 0);
	input_set_abs_params(dev1, ABS_Y, 0, priv->y_max, 0, 0);
}

int alps_init(struct psmouse *psmouse)
{
	struct alps_data *priv;
	struct input_dev *dev1 = psmouse->dev, *dev2;
>>>>>>> refs/remotes/origin/master

	priv = kzalloc(sizeof(struct alps_data), GFP_KERNEL);
	dev2 = input_allocate_device();
	if (!priv || !dev2)
		goto init_fail;

	priv->dev2 = dev2;
	setup_timer(&priv->timer, alps_flush_packet, (unsigned long)psmouse);

	psmouse->private = priv;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	psmouse_reset(psmouse);

>>>>>>> refs/remotes/origin/cm-10.0
	model = alps_get_model(psmouse, &version);
	if (!model)
		goto init_fail;

	priv->i = model;

	if (alps_hw_init(psmouse))
=======
	psmouse_reset(psmouse);

	if (alps_identify(psmouse, priv) < 0)
		goto init_fail;

	if (priv->hw_init(psmouse))
>>>>>>> refs/remotes/origin/master
		goto init_fail;

	/*
	 * Undo part of setup done for us by psmouse core since touchpad
	 * is not a relative device.
	 */
	__clear_bit(EV_REL, dev1->evbit);
	__clear_bit(REL_X, dev1->relbit);
	__clear_bit(REL_Y, dev1->relbit);

	/*
	 * Now set up our capabilities.
	 */
	dev1->evbit[BIT_WORD(EV_KEY)] |= BIT_MASK(EV_KEY);
	dev1->keybit[BIT_WORD(BTN_TOUCH)] |= BIT_MASK(BTN_TOUCH);
	dev1->keybit[BIT_WORD(BTN_TOOL_FINGER)] |= BIT_MASK(BTN_TOOL_FINGER);
	dev1->keybit[BIT_WORD(BTN_LEFT)] |=
		BIT_MASK(BTN_LEFT) | BIT_MASK(BTN_RIGHT);

	dev1->evbit[BIT_WORD(EV_ABS)] |= BIT_MASK(EV_ABS);
<<<<<<< HEAD
<<<<<<< HEAD
	input_set_abs_params(dev1, ABS_X, 0, 1023, 0, 0);
	input_set_abs_params(dev1, ABS_Y, 0, 767, 0, 0);
=======

	switch (model->proto_version) {
	case ALPS_PROTO_V1:
	case ALPS_PROTO_V2:
		input_set_abs_params(dev1, ABS_X, 0, 1023, 0, 0);
		input_set_abs_params(dev1, ABS_Y, 0, 767, 0, 0);
		break;
	case ALPS_PROTO_V3:
		set_bit(INPUT_PROP_SEMI_MT, dev1->propbit);
		input_mt_init_slots(dev1, 2);
		input_set_abs_params(dev1, ABS_MT_POSITION_X, 0, ALPS_V3_X_MAX, 0, 0);
		input_set_abs_params(dev1, ABS_MT_POSITION_Y, 0, ALPS_V3_Y_MAX, 0, 0);

		set_bit(BTN_TOOL_DOUBLETAP, dev1->keybit);
		set_bit(BTN_TOOL_TRIPLETAP, dev1->keybit);
		set_bit(BTN_TOOL_QUADTAP, dev1->keybit);
		/* fall through */
	case ALPS_PROTO_V4:
		input_set_abs_params(dev1, ABS_X, 0, ALPS_V3_X_MAX, 0, 0);
		input_set_abs_params(dev1, ABS_Y, 0, ALPS_V3_Y_MAX, 0, 0);
		break;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	input_set_abs_params(dev1, ABS_PRESSURE, 0, 127, 0, 0);

	if (model->flags & ALPS_WHEEL) {
=======

	priv->set_abs_params(priv, dev1);
	input_set_abs_params(dev1, ABS_PRESSURE, 0, 127, 0, 0);

	if (priv->flags & ALPS_WHEEL) {
>>>>>>> refs/remotes/origin/master
		dev1->evbit[BIT_WORD(EV_REL)] |= BIT_MASK(EV_REL);
		dev1->relbit[BIT_WORD(REL_WHEEL)] |= BIT_MASK(REL_WHEEL);
	}

<<<<<<< HEAD
	if (model->flags & (ALPS_FW_BK_1 | ALPS_FW_BK_2)) {
=======
	if (priv->flags & (ALPS_FW_BK_1 | ALPS_FW_BK_2)) {
>>>>>>> refs/remotes/origin/master
		dev1->keybit[BIT_WORD(BTN_FORWARD)] |= BIT_MASK(BTN_FORWARD);
		dev1->keybit[BIT_WORD(BTN_BACK)] |= BIT_MASK(BTN_BACK);
	}

<<<<<<< HEAD
	if (model->flags & ALPS_FOUR_BUTTONS) {
=======
	if (priv->flags & ALPS_FOUR_BUTTONS) {
>>>>>>> refs/remotes/origin/master
		dev1->keybit[BIT_WORD(BTN_0)] |= BIT_MASK(BTN_0);
		dev1->keybit[BIT_WORD(BTN_1)] |= BIT_MASK(BTN_1);
		dev1->keybit[BIT_WORD(BTN_2)] |= BIT_MASK(BTN_2);
		dev1->keybit[BIT_WORD(BTN_3)] |= BIT_MASK(BTN_3);
	} else {
		dev1->keybit[BIT_WORD(BTN_MIDDLE)] |= BIT_MASK(BTN_MIDDLE);
	}

	snprintf(priv->phys, sizeof(priv->phys), "%s/input1", psmouse->ps2dev.serio->phys);
	dev2->phys = priv->phys;
<<<<<<< HEAD
	dev2->name = (model->flags & ALPS_DUALPOINT) ? "DualPoint Stick" : "PS/2 Mouse";
=======
	dev2->name = (priv->flags & ALPS_DUALPOINT) ?
		     "DualPoint Stick" : "ALPS PS/2 Device";
>>>>>>> refs/remotes/origin/master
	dev2->id.bustype = BUS_I8042;
	dev2->id.vendor  = 0x0002;
	dev2->id.product = PSMOUSE_ALPS;
	dev2->id.version = 0x0000;
	dev2->dev.parent = &psmouse->ps2dev.serio->dev;

	dev2->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
	dev2->relbit[BIT_WORD(REL_X)] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);
	dev2->keybit[BIT_WORD(BTN_LEFT)] =
		BIT_MASK(BTN_LEFT) | BIT_MASK(BTN_MIDDLE) | BIT_MASK(BTN_RIGHT);

	if (input_register_device(priv->dev2))
		goto init_fail;

	psmouse->protocol_handler = alps_process_byte;
	psmouse->poll = alps_poll;
	psmouse->disconnect = alps_disconnect;
	psmouse->reconnect = alps_reconnect;
<<<<<<< HEAD
<<<<<<< HEAD
	psmouse->pktsize = 6;
=======
	psmouse->pktsize = model->proto_version == ALPS_PROTO_V4 ? 8 : 6;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	psmouse->pktsize = priv->proto_version == ALPS_PROTO_V4 ? 8 : 6;
>>>>>>> refs/remotes/origin/master

	/* We are having trouble resyncing ALPS touchpads so disable it for now */
	psmouse->resync_time = 0;

	return 0;

init_fail:
	psmouse_reset(psmouse);
	input_free_device(dev2);
	kfree(priv);
	psmouse->private = NULL;
	return -1;
}

int alps_detect(struct psmouse *psmouse, bool set_properties)
{
<<<<<<< HEAD
	int version;
	const struct alps_model_info *model;

	model = alps_get_model(psmouse, &version);
	if (!model)
=======
	struct alps_data dummy;

	if (alps_identify(psmouse, &dummy) < 0)
>>>>>>> refs/remotes/origin/master
		return -1;

	if (set_properties) {
		psmouse->vendor = "ALPS";
<<<<<<< HEAD
		psmouse->name = model->flags & ALPS_DUALPOINT ?
				"DualPoint TouchPad" : "GlidePoint";
		psmouse->model = version;
=======
		psmouse->name = dummy.flags & ALPS_DUALPOINT ?
				"DualPoint TouchPad" : "GlidePoint";
		psmouse->model = dummy.proto_version << 8;
>>>>>>> refs/remotes/origin/master
	}
	return 0;
}

