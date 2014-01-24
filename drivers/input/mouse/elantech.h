/*
 * Elantech Touchpad driver (v6)
 *
 * Copyright (C) 2007-2009 Arjan Opmeer <arjan@opmeer.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Trademarks are the property of their respective owners.
 */

#ifndef _ELANTECH_H
#define _ELANTECH_H

/*
 * Command values for Synaptics style queries
 */
<<<<<<< HEAD
<<<<<<< HEAD
#define ETP_FW_VERSION_QUERY		0x01
#define ETP_CAPABILITIES_QUERY		0x02
=======
=======
>>>>>>> refs/remotes/origin/master
#define ETP_FW_ID_QUERY			0x00
#define ETP_FW_VERSION_QUERY		0x01
#define ETP_CAPABILITIES_QUERY		0x02
#define ETP_SAMPLE_QUERY		0x03
#define ETP_RESOLUTION_QUERY		0x04
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/*
 * Command values for register reading or writing
 */
#define ETP_REGISTER_READ		0x10
#define ETP_REGISTER_WRITE		0x11
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define ETP_REGISTER_READWRITE		0x00
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define ETP_REGISTER_READWRITE		0x00
>>>>>>> refs/remotes/origin/master

/*
 * Hardware version 2 custom PS/2 command value
 */
#define ETP_PS2_CUSTOM_COMMAND		0xf8

/*
 * Times to retry a ps2_command and millisecond delay between tries
 */
#define ETP_PS2_COMMAND_TRIES		3
#define ETP_PS2_COMMAND_DELAY		500

/*
 * Times to try to read back a register and millisecond delay between tries
 */
#define ETP_READ_BACK_TRIES		5
#define ETP_READ_BACK_DELAY		2000

/*
 * Register bitmasks for hardware version 1
 */
#define ETP_R10_ABSOLUTE_MODE		0x04
#define ETP_R11_4_BYTE_MODE		0x02

/*
 * Capability bitmasks
 */
#define ETP_CAP_HAS_ROCKER		0x04

/*
 * One hard to find application note states that X axis range is 0 to 576
 * and Y axis range is 0 to 384 for harware version 1.
 * Edge fuzz might be necessary because of bezel around the touchpad
 */
#define ETP_EDGE_FUZZ_V1		32

#define ETP_XMIN_V1			(  0 + ETP_EDGE_FUZZ_V1)
#define ETP_XMAX_V1			(576 - ETP_EDGE_FUZZ_V1)
#define ETP_YMIN_V1			(  0 + ETP_EDGE_FUZZ_V1)
#define ETP_YMAX_V1			(384 - ETP_EDGE_FUZZ_V1)

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * It seems the resolution for hardware version 2 doubled.
 * Hence the X and Y ranges are doubled too.
 * The bezel around the pad also appears to be smaller
 */
#define ETP_EDGE_FUZZ_V2		8

#define ETP_XMIN_V2			(   0 + ETP_EDGE_FUZZ_V2)
#define ETP_XMAX_V2			(1152 - ETP_EDGE_FUZZ_V2)
#define ETP_YMIN_V2			(   0 + ETP_EDGE_FUZZ_V2)
#define ETP_YMAX_V2			( 768 - ETP_EDGE_FUZZ_V2)
=======
=======
>>>>>>> refs/remotes/origin/master
 * The resolution for older v2 hardware doubled.
 * (newer v2's firmware provides command so we can query)
 */
#define ETP_XMIN_V2			0
#define ETP_XMAX_V2			1152
#define ETP_YMIN_V2			0
#define ETP_YMAX_V2			768
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#define ETP_PMIN_V2			0
#define ETP_PMAX_V2			255
#define ETP_WMIN_V2			0
#define ETP_WMAX_V2			15

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * For two finger touches the coordinate of each finger gets reported
 * separately but with reduced resolution.
 */
#define ETP_2FT_FUZZ			4

#define ETP_2FT_XMIN			(  0 + ETP_2FT_FUZZ)
#define ETP_2FT_XMAX			(288 - ETP_2FT_FUZZ)
#define ETP_2FT_YMIN			(  0 + ETP_2FT_FUZZ)
#define ETP_2FT_YMAX			(192 - ETP_2FT_FUZZ)

struct elantech_data {
=======
=======
>>>>>>> refs/remotes/origin/master
 * v3 hardware has 2 kinds of packet types,
 * v4 hardware has 3.
 */
#define PACKET_UNKNOWN			0x01
#define PACKET_DEBOUNCE			0x02
#define PACKET_V3_HEAD			0x03
#define PACKET_V3_TAIL			0x04
#define PACKET_V4_HEAD			0x05
#define PACKET_V4_MOTION		0x06
#define PACKET_V4_STATUS		0x07

/*
 * track up to 5 fingers for v4 hardware
 */
#define ETP_MAX_FINGERS			5

/*
 * weight value for v4 hardware
 */
#define ETP_WEIGHT_VALUE		5

/*
 * The base position for one finger, v4 hardware
 */
struct finger_pos {
	unsigned int x;
	unsigned int y;
};

struct elantech_data {
	unsigned char reg_07;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	unsigned char reg_10;
	unsigned char reg_11;
	unsigned char reg_20;
	unsigned char reg_21;
	unsigned char reg_22;
	unsigned char reg_23;
	unsigned char reg_24;
	unsigned char reg_25;
	unsigned char reg_26;
	unsigned char debug;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned char capabilities;
=======
	unsigned char capabilities[3];
>>>>>>> refs/remotes/origin/cm-10.0
	bool paritycheck;
	bool jumpy_cursor;
	bool reports_pressure;
	unsigned char hw_version;
	unsigned int fw_version;
	unsigned int single_finger_reports;
<<<<<<< HEAD
	unsigned char parity[256];
=======
=======
	unsigned char capabilities[3];
	bool paritycheck;
	bool jumpy_cursor;
	bool reports_pressure;
	bool crc_enabled;
	unsigned char hw_version;
	unsigned int fw_version;
	unsigned int single_finger_reports;
>>>>>>> refs/remotes/origin/master
	unsigned int y_max;
	unsigned int width;
	struct finger_pos mt[ETP_MAX_FINGERS];
	unsigned char parity[256];
	int (*send_cmd)(struct psmouse *psmouse, unsigned char c, unsigned char *param);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

#ifdef CONFIG_MOUSE_PS2_ELANTECH
int elantech_detect(struct psmouse *psmouse, bool set_properties);
int elantech_init(struct psmouse *psmouse);
#else
static inline int elantech_detect(struct psmouse *psmouse, bool set_properties)
{
	return -ENOSYS;
}
static inline int elantech_init(struct psmouse *psmouse)
{
	return -ENOSYS;
}
#endif /* CONFIG_MOUSE_PS2_ELANTECH */

#endif
