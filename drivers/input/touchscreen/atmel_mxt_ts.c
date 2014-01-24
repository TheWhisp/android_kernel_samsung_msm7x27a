/*
 * Atmel maXTouch Touchscreen driver
 *
 * Copyright (C) 2010 Samsung Electronics Co.Ltd
 * Author: Joonyoung Shim <jy0922.shim@samsung.com>
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
=======
>>>>>>> refs/remotes/origin/master
=======
 * Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/firmware.h>
#include <linux/i2c.h>
#include <linux/i2c/atmel_mxt_ts.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>

=======
#include <linux/input/mt.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/regulator/consumer.h>
#include <linux/string.h>
#include <linux/of_gpio.h>
>>>>>>> refs/remotes/origin/cm-10.0
#if defined(CONFIG_HAS_EARLYSUSPEND)
#include <linux/earlysuspend.h>
/* Early-suspend level */
#define MXT_SUSPEND_LEVEL 1
#endif

/* Family ID */
#define MXT224_ID	0x80
#define MXT224E_ID	0x81
#define MXT1386_ID	0xA0
=======
#include <linux/input/mt.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
#include <linux/gpio.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/regulator/consumer.h>
#include <linux/string.h>
#include <linux/of_gpio.h>
#if defined(CONFIG_HAS_EARLYSUSPEND)
#include <linux/earlysuspend.h>
/* Early-suspend level */
#define MXT_SUSPEND_LEVEL 1
#endif

/* Family ID */
#define MXT224_ID	0x80
#define MXT224E_ID	0x81
#define MXT1386_ID	0xA0
>>>>>>> refs/remotes/origin/cm-11.0

/* Version */
#define MXT_VER_20		20
#define MXT_VER_21		21
#define MXT_VER_22		22

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
/* Slave addresses */
#define MXT_APP_LOW		0x4a
#define MXT_APP_HIGH		0x4b
#define MXT_BOOT_LOW		0x24
#define MXT_BOOT_HIGH		0x25
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
/* I2C slave address pairs */
struct mxt_address_pair {
	int bootloader;
	int application;
};

static const struct mxt_address_pair mxt_slave_addresses[] = {
	{ 0x24, 0x4a },
	{ 0x25, 0x4b },
	{ 0x25, 0x4b },
	{ 0x26, 0x4c },
	{ 0x27, 0x4d },
	{ 0x34, 0x5a },
	{ 0x35, 0x5b },
	{ 0 },
};

enum mxt_device_state { INIT, APPMODE, BOOTLOADER };
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

/* Firmware */
#define MXT_FW_NAME		"maxtouch.fw"

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
/* Firmware frame size including frame data and CRC */
#define MXT_SINGLE_FW_MAX_FRAME_SIZE	278
#define MXT_CHIPSET_FW_MAX_FRAME_SIZE	534

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
/* Registers */
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
/* Registers */
#define MXT_INFO		0x00
>>>>>>> refs/remotes/origin/master
#define MXT_FAMILY_ID		0x00
#define MXT_VARIANT_ID		0x01
#define MXT_VERSION		0x02
#define MXT_BUILD		0x03
#define MXT_MATRIX_X_SIZE	0x04
#define MXT_MATRIX_Y_SIZE	0x05
#define MXT_OBJECT_NUM		0x06
#define MXT_OBJECT_START	0x07

#define MXT_OBJECT_SIZE		6

/* Object types */
#define MXT_DEBUG_DIAGNOSTIC_T37	37
#define MXT_GEN_MESSAGE_T5		5
#define MXT_GEN_COMMAND_T6		6
#define MXT_GEN_POWER_T7		7
#define MXT_GEN_ACQUIRE_T8		8
#define MXT_GEN_DATASOURCE_T53		53
#define MXT_TOUCH_MULTI_T9		9
#define MXT_TOUCH_KEYARRAY_T15		15
#define MXT_TOUCH_PROXIMITY_T23		23
#define MXT_TOUCH_PROXKEY_T52		52
#define MXT_PROCI_GRIPFACE_T20		20
#define MXT_PROCG_NOISE_T22		22
#define MXT_PROCI_ONETOUCH_T24		24
#define MXT_PROCI_TWOTOUCH_T27		27
#define MXT_PROCI_GRIP_T40		40
#define MXT_PROCI_PALM_T41		41
#define MXT_PROCI_TOUCHSUPPRESSION_T42	42
#define MXT_PROCI_STYLUS_T47		47
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define MXT_PROCI_ADAPTIVETHRESHOLD_T55 55
#define MXT_PROCI_SHIELDLESS_T56	56
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
#define MXT_PROCI_ADAPTIVETHRESHOLD_T55 55
#define MXT_PROCI_SHIELDLESS_T56	56
>>>>>>> refs/remotes/origin/cm-11.0
#define MXT_PROCG_NOISESUPPRESSION_T48	48
#define MXT_SPT_COMMSCONFIG_T18		18
#define MXT_SPT_GPIOPWM_T19		19
#define MXT_SPT_SELFTEST_T25		25
#define MXT_SPT_CTECONFIG_T28		28
#define MXT_SPT_USERDATA_T38		38
#define MXT_SPT_DIGITIZER_T43		43
#define MXT_SPT_MESSAGECOUNT_T44	44
#define MXT_SPT_CTECONFIG_T46		46

/* MXT_GEN_COMMAND_T6 field */
#define MXT_COMMAND_RESET	0
#define MXT_COMMAND_BACKUPNV	1
#define MXT_COMMAND_CALIBRATE	2
#define MXT_COMMAND_REPORTALL	3
#define MXT_COMMAND_DIAGNOSTIC	5

/* MXT_GEN_POWER_T7 field */
#define MXT_POWER_IDLEACQINT	0
#define MXT_POWER_ACTVACQINT	1
#define MXT_POWER_ACTV2IDLETO	2

/* MXT_GEN_ACQUIRE_T8 field */
#define MXT_ACQUIRE_CHRGTIME	0
#define MXT_ACQUIRE_TCHDRIFT	2
#define MXT_ACQUIRE_DRIFTST	3
#define MXT_ACQUIRE_TCHAUTOCAL	4
#define MXT_ACQUIRE_SYNC	5
#define MXT_ACQUIRE_ATCHCALST	6
#define MXT_ACQUIRE_ATCHCALSTHR	7

<<<<<<< HEAD
<<<<<<< HEAD
/* MXT_TOUCH_MULT_T9 field */
=======
/* MXT_TOUCH_MULTI_T9 field */
>>>>>>> refs/remotes/origin/master
=======
/* MXT_TOUCH_MULT_T9 field */
>>>>>>> refs/remotes/origin/cm-11.0
#define MXT_TOUCH_CTRL		0
#define MXT_TOUCH_XORIGIN	1
#define MXT_TOUCH_YORIGIN	2
#define MXT_TOUCH_XSIZE		3
#define MXT_TOUCH_YSIZE		4
#define MXT_TOUCH_BLEN		6
#define MXT_TOUCH_TCHTHR	7
#define MXT_TOUCH_TCHDI		8
#define MXT_TOUCH_ORIENT	9
#define MXT_TOUCH_MOVHYSTI	11
#define MXT_TOUCH_MOVHYSTN	12
#define MXT_TOUCH_NUMTOUCH	14
#define MXT_TOUCH_MRGHYST	15
#define MXT_TOUCH_MRGTHR	16
#define MXT_TOUCH_AMPHYST	17
#define MXT_TOUCH_XRANGE_LSB	18
#define MXT_TOUCH_XRANGE_MSB	19
#define MXT_TOUCH_YRANGE_LSB	20
#define MXT_TOUCH_YRANGE_MSB	21
#define MXT_TOUCH_XLOCLIP	22
#define MXT_TOUCH_XHICLIP	23
#define MXT_TOUCH_YLOCLIP	24
#define MXT_TOUCH_YHICLIP	25
#define MXT_TOUCH_XEDGECTRL	26
#define MXT_TOUCH_XEDGEDIST	27
#define MXT_TOUCH_YEDGECTRL	28
#define MXT_TOUCH_YEDGEDIST	29
#define MXT_TOUCH_JUMPLIMIT	30

/* MXT_PROCI_GRIPFACE_T20 field */
#define MXT_GRIPFACE_CTRL	0
#define MXT_GRIPFACE_XLOGRIP	1
#define MXT_GRIPFACE_XHIGRIP	2
#define MXT_GRIPFACE_YLOGRIP	3
#define MXT_GRIPFACE_YHIGRIP	4
#define MXT_GRIPFACE_MAXTCHS	5
#define MXT_GRIPFACE_SZTHR1	7
#define MXT_GRIPFACE_SZTHR2	8
#define MXT_GRIPFACE_SHPTHR1	9
#define MXT_GRIPFACE_SHPTHR2	10
#define MXT_GRIPFACE_SUPEXTTO	11

/* MXT_PROCI_NOISE field */
#define MXT_NOISE_CTRL		0
#define MXT_NOISE_OUTFLEN	1
#define MXT_NOISE_GCAFUL_LSB	3
#define MXT_NOISE_GCAFUL_MSB	4
#define MXT_NOISE_GCAFLL_LSB	5
#define MXT_NOISE_GCAFLL_MSB	6
#define MXT_NOISE_ACTVGCAFVALID	7
#define MXT_NOISE_NOISETHR	8
#define MXT_NOISE_FREQHOPSCALE	10
#define MXT_NOISE_FREQ0		11
#define MXT_NOISE_FREQ1		12
#define MXT_NOISE_FREQ2		13
#define MXT_NOISE_FREQ3		14
#define MXT_NOISE_FREQ4		15
#define MXT_NOISE_IDLEGCAFVALID	16

/* MXT_SPT_COMMSCONFIG_T18 */
#define MXT_COMMS_CTRL		0
#define MXT_COMMS_CMD		1

/* MXT_SPT_CTECONFIG_T28 field */
#define MXT_CTE_CTRL		0
#define MXT_CTE_CMD		1
#define MXT_CTE_MODE		2
#define MXT_CTE_IDLEGCAFDEPTH	3
#define MXT_CTE_ACTVGCAFDEPTH	4
#define MXT_CTE_VOLTAGE		5

#define MXT_VOLTAGE_DEFAULT	2700000
#define MXT_VOLTAGE_STEP	10000

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
/* Analog voltage @2.7 V */
#define MXT_VTG_MIN_UV		2700000
#define MXT_VTG_MAX_UV		3300000
#define MXT_ACTIVE_LOAD_UA	15000
#define MXT_LPM_LOAD_UA		10
/* Digital voltage @1.8 V */
#define MXT_VTG_DIG_MIN_UV	1800000
#define MXT_VTG_DIG_MAX_UV	1800000
#define MXT_ACTIVE_LOAD_DIG_UA	10000
#define MXT_LPM_LOAD_DIG_UA	10

#define MXT_I2C_VTG_MIN_UV	1800000
#define MXT_I2C_VTG_MAX_UV	1800000
#define MXT_I2C_LOAD_UA		10000
#define MXT_I2C_LPM_LOAD_UA	10

/* Define for MXT_GEN_COMMAND_T6 */
#define MXT_BOOT_VALUE		0xa5
#define MXT_BACKUP_VALUE	0x55
#define MXT_BACKUP_TIME		25	/* msec */
<<<<<<< HEAD
<<<<<<< HEAD
#define MXT224_RESET_TIME		65	/* msec */
#define MXT224E_RESET_TIME		22	/* msec */
#define MXT1386_RESET_TIME		250	/* msec */
#define MXT_RESET_TIME		250	/* msec */
#define MXT_RESET_NOCHGREAD		400	/* msec */

#define MXT_FWRESET_TIME	175	/* msec */
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define MXT224_RESET_TIME	65	/* msec */
#define MXT224E_RESET_TIME	150	/* msec */
#define MXT1386_RESET_TIME	250	/* msec */
#define MXT_RESET_TIME		250	/* msec */
#define MXT_RESET_NOCHGREAD	400	/* msec */
<<<<<<< HEAD

#define MXT_FWRESET_TIME	1000	/* msec */
>>>>>>> refs/remotes/origin/cm-10.0

#define MXT_WAKE_TIME		25
=======
/* Define for MXT_GEN_COMMAND_T6 */
#define MXT_BOOT_VALUE		0xa5
#define MXT_BACKUP_VALUE	0x55
#define MXT_BACKUP_TIME		50	/* msec */
#define MXT_RESET_TIME		200	/* msec */
=======
>>>>>>> refs/remotes/origin/cm-11.0

#define MXT_FWRESET_TIME	1000	/* msec */

#define MXT_WAKE_TIME		25

/* MXT_SPT_GPIOPWM_T19 field */
#define MXT_GPIO0_MASK		0x04
#define MXT_GPIO1_MASK		0x08
#define MXT_GPIO2_MASK		0x10
#define MXT_GPIO3_MASK		0x20
>>>>>>> refs/remotes/origin/master

/* Command to unlock bootloader */
#define MXT_UNLOCK_CMD_MSB	0xaa
#define MXT_UNLOCK_CMD_LSB	0xdc

/* Bootloader mode status */
#define MXT_WAITING_BOOTLOAD_CMD	0xc0	/* valid 7 6 bit only */
#define MXT_WAITING_FRAME_DATA	0x80	/* valid 7 6 bit only */
#define MXT_FRAME_CRC_CHECK	0x02
#define MXT_FRAME_CRC_FAIL	0x03
#define MXT_FRAME_CRC_PASS	0x04
#define MXT_APP_CRC_FAIL	0x40	/* valid 7 8 bit only */
#define MXT_BOOT_STATUS_MASK	0x3f
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define MXT_BOOT_EXTENDED_ID	(1 << 5)
#define MXT_BOOT_ID_MASK	0x1f
>>>>>>> refs/remotes/origin/cm-10.0

/* Touch status */
=======
=======
#define MXT_BOOT_EXTENDED_ID	(1 << 5)
#define MXT_BOOT_ID_MASK	0x1f
>>>>>>> refs/remotes/origin/cm-11.0

/* Touch status */
#define MXT_UNGRIP		(1 << 0)
>>>>>>> refs/remotes/origin/master
#define MXT_SUPPRESS		(1 << 1)
#define MXT_AMP			(1 << 2)
#define MXT_VECTOR		(1 << 3)
#define MXT_MOVE		(1 << 4)
#define MXT_RELEASE		(1 << 5)
#define MXT_PRESS		(1 << 6)
#define MXT_DETECT		(1 << 7)

/* Touch orient bits */
#define MXT_XY_SWITCH		(1 << 0)
#define MXT_X_INVERT		(1 << 1)
#define MXT_Y_INVERT		(1 << 2)

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
/* Touch suppression */
#define MXT_TCHSUP_ACTIVE      (1 << 0)

>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Touch suppression */
#define MXT_TCHSUP_ACTIVE      (1 << 0)

>>>>>>> refs/remotes/origin/cm-11.0
/* Touchscreen absolute values */
#define MXT_MAX_AREA		0xff

#define MXT_MAX_FINGER		10

#define T7_DATA_SIZE		3
#define MXT_MAX_RW_TRIES	3
#define MXT_BLOCK_SIZE		256
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define MXT_CFG_VERSION_LEN	3
#define MXT_CFG_VERSION_EQUAL	0
#define MXT_CFG_VERSION_LESS	1
#define MXT_CFG_VERSION_GREATER	2

#define MXT_COORDS_ARR_SIZE	4

#define MXT_DEBUGFS_DIR		"atmel_mxt_ts"
#define MXT_DEBUGFS_FILE	"object"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Touchscreen absolute values */
#define MXT_MAX_AREA		0xff

#define MXT_PIXELS_PER_MM	20
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

struct mxt_info {
	u8 family_id;
	u8 variant_id;
	u8 version;
	u8 build;
	u8 matrix_xsize;
	u8 matrix_ysize;
	u8 object_num;
};

struct mxt_object {
	u8 type;
	u16 start_address;
<<<<<<< HEAD
	u8 size;
	u8 instances;
	u8 num_report_ids;

	/* to map object and message */
	u8 max_reportid;
};
=======
	u8 size;		/* Size of each instance - 1 */
	u8 instances;		/* Number of instances - 1 */
	u8 num_report_ids;
} __packed;
>>>>>>> refs/remotes/origin/master

struct mxt_message {
	u8 reportid;
	u8 message[7];
<<<<<<< HEAD
	u8 checksum;
};

struct mxt_finger {
	int status;
	int x;
	int y;
	int area;
	int pressure;
=======
>>>>>>> refs/remotes/origin/master
};

/* Each client has this additional data */
struct mxt_data {
	struct i2c_client *client;
	struct input_dev *input_dev;
<<<<<<< HEAD
	const struct mxt_platform_data *pdata;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	const struct mxt_config_info *config_info;
	enum mxt_device_state state;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const struct mxt_config_info *config_info;
	enum mxt_device_state state;
>>>>>>> refs/remotes/origin/cm-11.0
	struct mxt_object *object_table;
	struct mxt_info info;
	struct mxt_finger finger[MXT_MAX_FINGER];
	unsigned int irq;
	struct regulator *vcc_ana;
	struct regulator *vcc_dig;
	struct regulator *vcc_i2c;
#if defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend early_suspend;
#endif

	u8 t7_data[T7_DATA_SIZE];
	u16 t7_start_addr;
<<<<<<< HEAD
<<<<<<< HEAD
	u8 t9_ctrl;
};

=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	u32 keyarray_old;
	u32 keyarray_new;
	u8 t9_max_reportid;
	u8 t9_min_reportid;
	u8 t15_max_reportid;
	u8 t15_min_reportid;
	u8 t42_max_reportid;
	u8 t42_min_reportid;
	u8 cfg_version[MXT_CFG_VERSION_LEN];
	int cfg_version_idx;
	int t38_start_addr;
	bool update_cfg;
	const char *fw_name;
<<<<<<< HEAD
};

static struct dentry *debug_base;

>>>>>>> refs/remotes/origin/cm-10.0
static bool mxt_object_readable(unsigned int type)
{
	switch (type) {
	case MXT_GEN_MESSAGE_T5:
=======
	char phys[64];		/* device physical location */
	const struct mxt_platform_data *pdata;
	struct mxt_object *object_table;
	struct mxt_info info;
	bool is_tp;

	unsigned int irq;
	unsigned int max_x;
	unsigned int max_y;

	/* Cached parameters from object table */
	u8 T6_reportid;
	u8 T9_reportid_min;
	u8 T9_reportid_max;
	u8 T19_reportid;
=======
>>>>>>> refs/remotes/origin/cm-11.0
};

static struct dentry *debug_base;

static bool mxt_object_readable(unsigned int type)
{
	switch (type) {
>>>>>>> refs/remotes/origin/master
	case MXT_GEN_COMMAND_T6:
	case MXT_GEN_POWER_T7:
	case MXT_GEN_ACQUIRE_T8:
	case MXT_GEN_DATASOURCE_T53:
	case MXT_TOUCH_MULTI_T9:
	case MXT_TOUCH_KEYARRAY_T15:
	case MXT_TOUCH_PROXIMITY_T23:
	case MXT_TOUCH_PROXKEY_T52:
	case MXT_PROCI_GRIPFACE_T20:
	case MXT_PROCG_NOISE_T22:
	case MXT_PROCI_ONETOUCH_T24:
	case MXT_PROCI_TWOTOUCH_T27:
	case MXT_PROCI_GRIP_T40:
	case MXT_PROCI_PALM_T41:
	case MXT_PROCI_TOUCHSUPPRESSION_T42:
	case MXT_PROCI_STYLUS_T47:
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	case MXT_PROCI_SHIELDLESS_T56:
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	case MXT_PROCI_SHIELDLESS_T56:
>>>>>>> refs/remotes/origin/cm-11.0
	case MXT_PROCG_NOISESUPPRESSION_T48:
	case MXT_SPT_COMMSCONFIG_T18:
	case MXT_SPT_GPIOPWM_T19:
	case MXT_SPT_SELFTEST_T25:
	case MXT_SPT_CTECONFIG_T28:
	case MXT_SPT_USERDATA_T38:
	case MXT_SPT_DIGITIZER_T43:
	case MXT_SPT_CTECONFIG_T46:
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	case MXT_PROCI_ADAPTIVETHRESHOLD_T55:
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	case MXT_PROCI_ADAPTIVETHRESHOLD_T55:
>>>>>>> refs/remotes/origin/cm-11.0
		return true;
	default:
		return false;
	}
}

static bool mxt_object_writable(unsigned int type)
{
	switch (type) {
	case MXT_GEN_COMMAND_T6:
	case MXT_GEN_POWER_T7:
	case MXT_GEN_ACQUIRE_T8:
	case MXT_TOUCH_MULTI_T9:
	case MXT_TOUCH_KEYARRAY_T15:
	case MXT_TOUCH_PROXIMITY_T23:
	case MXT_TOUCH_PROXKEY_T52:
	case MXT_PROCI_GRIPFACE_T20:
	case MXT_PROCG_NOISE_T22:
	case MXT_PROCI_ONETOUCH_T24:
	case MXT_PROCI_TWOTOUCH_T27:
	case MXT_PROCI_GRIP_T40:
	case MXT_PROCI_PALM_T41:
	case MXT_PROCI_TOUCHSUPPRESSION_T42:
	case MXT_PROCI_STYLUS_T47:
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	case MXT_PROCI_SHIELDLESS_T56:
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	case MXT_PROCI_SHIELDLESS_T56:
>>>>>>> refs/remotes/origin/cm-11.0
	case MXT_PROCG_NOISESUPPRESSION_T48:
	case MXT_SPT_COMMSCONFIG_T18:
	case MXT_SPT_GPIOPWM_T19:
	case MXT_SPT_SELFTEST_T25:
	case MXT_SPT_CTECONFIG_T28:
<<<<<<< HEAD
<<<<<<< HEAD
	case MXT_SPT_USERDATA_T38:
	case MXT_SPT_DIGITIZER_T43:
	case MXT_SPT_CTECONFIG_T46:
<<<<<<< HEAD
=======
	case MXT_PROCI_ADAPTIVETHRESHOLD_T55:
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case MXT_SPT_DIGITIZER_T43:
	case MXT_SPT_CTECONFIG_T46:
>>>>>>> refs/remotes/origin/master
=======
	case MXT_SPT_USERDATA_T38:
	case MXT_SPT_DIGITIZER_T43:
	case MXT_SPT_CTECONFIG_T46:
	case MXT_PROCI_ADAPTIVETHRESHOLD_T55:
>>>>>>> refs/remotes/origin/cm-11.0
		return true;
	default:
		return false;
	}
}

static void mxt_dump_message(struct device *dev,
<<<<<<< HEAD
				  struct mxt_message *message)
{
	dev_dbg(dev, "reportid:\t0x%x\n", message->reportid);
	dev_dbg(dev, "message1:\t0x%x\n", message->message[0]);
	dev_dbg(dev, "message2:\t0x%x\n", message->message[1]);
	dev_dbg(dev, "message3:\t0x%x\n", message->message[2]);
	dev_dbg(dev, "message4:\t0x%x\n", message->message[3]);
	dev_dbg(dev, "message5:\t0x%x\n", message->message[4]);
	dev_dbg(dev, "message6:\t0x%x\n", message->message[5]);
	dev_dbg(dev, "message7:\t0x%x\n", message->message[6]);
	dev_dbg(dev, "checksum:\t0x%x\n", message->checksum);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int mxt_check_bootloader(struct i2c_client *client,
				     unsigned int state)
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int mxt_switch_to_bootloader_address(struct mxt_data *data)
{
	int i;
	struct i2c_client *client = data->client;

	if (data->state == BOOTLOADER) {
		dev_err(&client->dev, "Already in BOOTLOADER state\n");
		return -EINVAL;
	}

	for (i = 0; mxt_slave_addresses[i].application != 0;  i++) {
		if (mxt_slave_addresses[i].application == client->addr) {
			dev_info(&client->dev, "Changing to bootloader address: "
				"%02x -> %02x",
				client->addr,
				mxt_slave_addresses[i].bootloader);

			client->addr = mxt_slave_addresses[i].bootloader;
			data->state = BOOTLOADER;
			return 0;
		}
	}

	dev_err(&client->dev, "Address 0x%02x not found in address table",
								client->addr);
	return -EINVAL;
}

static int mxt_switch_to_appmode_address(struct mxt_data *data)
{
	int i;
	struct i2c_client *client = data->client;

	if (data->state == APPMODE) {
		dev_err(&client->dev, "Already in APPMODE state\n");
		return -EINVAL;
	}

	for (i = 0; mxt_slave_addresses[i].application != 0;  i++) {
		if (mxt_slave_addresses[i].bootloader == client->addr) {
			dev_info(&client->dev,
				"Changing to application mode address: "
							"0x%02x -> 0x%02x",
				client->addr,
				mxt_slave_addresses[i].application);

			client->addr = mxt_slave_addresses[i].application;
			data->state = APPMODE;
			return 0;
		}
	}

	dev_err(&client->dev, "Address 0x%02x not found in address table",
								client->addr);
	return -EINVAL;
}

static int mxt_get_bootloader_version(struct i2c_client *client, u8 val)
{
	u8 buf[3];

	if (val | MXT_BOOT_EXTENDED_ID)	{
		dev_dbg(&client->dev,
				"Retrieving extended mode ID information");

		if (i2c_master_recv(client, &buf[0], 3) != 3) {
			dev_err(&client->dev, "%s: i2c recv failed\n",
								__func__);
			return -EIO;
		}

		dev_info(&client->dev, "Bootloader ID:%d Version:%d",
			buf[1], buf[2]);

		return buf[0];
	} else {
		dev_info(&client->dev, "Bootloader ID:%d",
			val & MXT_BOOT_ID_MASK);

		return val;
	}
}

static int mxt_get_bootloader_id(struct i2c_client *client)
{
	u8 val;
	u8 buf[3];

	if (i2c_master_recv(client, &val, 1) != 1) {
		dev_err(&client->dev, "%s: i2c recv failed\n", __func__);
		return -EIO;
	}

	if (val | MXT_BOOT_EXTENDED_ID)	{
		if (i2c_master_recv(client, &buf[0], 3) != 3) {
			dev_err(&client->dev, "%s: i2c recv failed\n",
								__func__);
			return -EIO;
		}
		return buf[1];
	} else {
		dev_info(&client->dev, "Bootloader ID:%d",
			val & MXT_BOOT_ID_MASK);

		return val & MXT_BOOT_ID_MASK;
	}
}

<<<<<<< HEAD
static int mxt_check_bootloader(struct i2c_client *client,
				unsigned int state)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			     struct mxt_message *message)
{
	dev_dbg(dev, "reportid: %u\tmessage: %*ph\n",
		message->reportid, 7, message->message);
}

static int mxt_check_bootloader(struct i2c_client *client,
				     unsigned int state)
>>>>>>> refs/remotes/origin/master
=======
static int mxt_check_bootloader(struct i2c_client *client,
				unsigned int state)
>>>>>>> refs/remotes/origin/cm-11.0
{
	u8 val;

recheck:
	if (i2c_master_recv(client, &val, 1) != 1) {
		dev_err(&client->dev, "%s: i2c recv failed\n", __func__);
		return -EIO;
	}

	switch (state) {
	case MXT_WAITING_BOOTLOAD_CMD:
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	case MXT_WAITING_FRAME_DATA:
=======
		val = mxt_get_bootloader_version(client, val);
		val &= ~MXT_BOOT_STATUS_MASK;
		break;
	case MXT_WAITING_FRAME_DATA:
	case MXT_APP_CRC_FAIL:
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case MXT_WAITING_FRAME_DATA:
>>>>>>> refs/remotes/origin/master
=======
		val = mxt_get_bootloader_version(client, val);
		val &= ~MXT_BOOT_STATUS_MASK;
		break;
	case MXT_WAITING_FRAME_DATA:
	case MXT_APP_CRC_FAIL:
>>>>>>> refs/remotes/origin/cm-11.0
		val &= ~MXT_BOOT_STATUS_MASK;
		break;
	case MXT_FRAME_CRC_PASS:
		if (val == MXT_FRAME_CRC_CHECK)
			goto recheck;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
		if (val == MXT_FRAME_CRC_FAIL) {
			dev_err(&client->dev, "Bootloader CRC fail\n");
			return -EINVAL;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		break;
	default:
		return -EINVAL;
	}

	if (val != state) {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		dev_err(&client->dev, "Unvalid bootloader mode state\n");
=======
		dev_err(&client->dev, "Invalid bootloader mode state %X\n",
			val);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		dev_err(&client->dev, "Unvalid bootloader mode state\n");
>>>>>>> refs/remotes/origin/master
=======
		dev_err(&client->dev, "Invalid bootloader mode state %X\n",
			val);
>>>>>>> refs/remotes/origin/cm-11.0
		return -EINVAL;
	}

	return 0;
}

static int mxt_unlock_bootloader(struct i2c_client *client)
{
	u8 buf[2];

	buf[0] = MXT_UNLOCK_CMD_LSB;
	buf[1] = MXT_UNLOCK_CMD_MSB;

	if (i2c_master_send(client, buf, 2) != 2) {
		dev_err(&client->dev, "%s: i2c send failed\n", __func__);
		return -EIO;
	}

	return 0;
}

static int mxt_fw_write(struct i2c_client *client,
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
			     const u8 *data, unsigned int frame_size)
=======
			const u8 *data, unsigned int frame_size)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			     const u8 *data, unsigned int frame_size)
>>>>>>> refs/remotes/origin/master
=======
			const u8 *data, unsigned int frame_size)
>>>>>>> refs/remotes/origin/cm-11.0
{
	if (i2c_master_send(client, data, frame_size) != frame_size) {
		dev_err(&client->dev, "%s: i2c send failed\n", __func__);
		return -EIO;
	}

	return 0;
}

static int __mxt_read_reg(struct i2c_client *client,
			       u16 reg, u16 len, void *val)
{
	struct i2c_msg xfer[2];
	u8 buf[2];
<<<<<<< HEAD
<<<<<<< HEAD
	int i = 0;
=======
	int ret;
>>>>>>> refs/remotes/origin/master
=======
	int i = 0;
>>>>>>> refs/remotes/origin/cm-11.0

	buf[0] = reg & 0xff;
	buf[1] = (reg >> 8) & 0xff;

	/* Write register */
	xfer[0].addr = client->addr;
	xfer[0].flags = 0;
	xfer[0].len = 2;
	xfer[0].buf = buf;

	/* Read data */
	xfer[1].addr = client->addr;
	xfer[1].flags = I2C_M_RD;
	xfer[1].len = len;
	xfer[1].buf = val;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	do {
		if (i2c_transfer(client->adapter, xfer, 2) == 2)
			return 0;
		msleep(MXT_WAKE_TIME);
	} while (++i < MXT_MAX_RW_TRIES);
<<<<<<< HEAD

	dev_err(&client->dev, "%s: i2c transfer failed\n", __func__);
	return -EIO;
=======
	ret = i2c_transfer(client->adapter, xfer, 2);
	if (ret == 2) {
		ret = 0;
	} else {
		if (ret >= 0)
			ret = -EIO;
		dev_err(&client->dev, "%s: i2c transfer failed (%d)\n",
			__func__, ret);
	}

	return ret;
>>>>>>> refs/remotes/origin/master
=======

	dev_err(&client->dev, "%s: i2c transfer failed\n", __func__);
	return -EIO;
>>>>>>> refs/remotes/origin/cm-11.0
}

static int mxt_read_reg(struct i2c_client *client, u16 reg, u8 *val)
{
	return __mxt_read_reg(client, reg, 1, val);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int __mxt_write_reg(struct i2c_client *client,
		    u16 addr, u16 length, u8 *value)
{
	u8 buf[MXT_BLOCK_SIZE + 2];
	int i, tries = 0;
<<<<<<< HEAD

	if (length > MXT_BLOCK_SIZE)
		return -EINVAL;

	buf[0] = addr & 0xff;
	buf[1] = (addr >> 8) & 0xff;
	for (i = 0; i < length; i++)
		buf[i + 2] = *value++;

	do {
		if (i2c_master_send(client, buf, length + 2) == (length + 2))
			return 0;
		msleep(MXT_WAKE_TIME);
	} while (++tries < MXT_MAX_RW_TRIES);

	dev_err(&client->dev, "%s: i2c send failed\n", __func__);
	return -EIO;
=======
static int __mxt_write_reg(struct i2c_client *client, u16 reg, u16 len,
			   const void *val)
{
	u8 *buf;
	size_t count;
	int ret;

	count = len + 2;
	buf = kmalloc(count, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	buf[0] = reg & 0xff;
	buf[1] = (reg >> 8) & 0xff;
	memcpy(&buf[2], val, len);

	ret = i2c_master_send(client, buf, count);
	if (ret == count) {
		ret = 0;
	} else {
		if (ret >= 0)
			ret = -EIO;
		dev_err(&client->dev, "%s: i2c send failed (%d)\n",
			__func__, ret);
	}

	kfree(buf);
	return ret;
>>>>>>> refs/remotes/origin/master
=======

	if (length > MXT_BLOCK_SIZE)
		return -EINVAL;

	buf[0] = addr & 0xff;
	buf[1] = (addr >> 8) & 0xff;
	for (i = 0; i < length; i++)
		buf[i + 2] = *value++;

	do {
		if (i2c_master_send(client, buf, length + 2) == (length + 2))
			return 0;
		msleep(MXT_WAKE_TIME);
	} while (++tries < MXT_MAX_RW_TRIES);

	dev_err(&client->dev, "%s: i2c send failed\n", __func__);
	return -EIO;
>>>>>>> refs/remotes/origin/cm-11.0
}

static int mxt_write_reg(struct i2c_client *client, u16 reg, u8 val)
{
	return __mxt_write_reg(client, reg, 1, &val);
}

<<<<<<< HEAD
static int mxt_read_object_table(struct i2c_client *client,
				      u16 reg, u8 *object_buf)
{
	return __mxt_read_reg(client, reg, MXT_OBJECT_SIZE,
				   object_buf);
}

=======
>>>>>>> refs/remotes/origin/master
static struct mxt_object *
mxt_get_object(struct mxt_data *data, u8 type)
{
	struct mxt_object *object;
	int i;

	for (i = 0; i < data->info.object_num; i++) {
		object = data->object_table + i;
		if (object->type == type)
			return object;
	}

	dev_err(&data->client->dev, "Invalid object type\n");
	return NULL;
}

static int mxt_read_message(struct mxt_data *data,
				 struct mxt_message *message)
{
	struct mxt_object *object;
	u16 reg;

	object = mxt_get_object(data, MXT_GEN_MESSAGE_T5);
	if (!object)
		return -EINVAL;

	reg = object->start_address;
	return __mxt_read_reg(data->client, reg,
			sizeof(struct mxt_message), message);
}

<<<<<<< HEAD
static int mxt_read_object(struct mxt_data *data,
				u8 type, u8 offset, u8 *val)
{
	struct mxt_object *object;
	u16 reg;

	object = mxt_get_object(data, type);
	if (!object)
		return -EINVAL;

	reg = object->start_address;
	return __mxt_read_reg(data->client, reg + offset, 1, val);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int mxt_get_object_address(struct device *dev, u8 type)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	u8 obj_num, obj_buf[MXT_OBJECT_SIZE];
	u16 reg;
	int i, error;

	error = mxt_read_reg(data->client, MXT_OBJECT_NUM, &obj_num);

	if (error) {
		dev_err(dev, "reading number of objects failed\n");
		return -EINVAL;
	}

	for (i = 0; i < obj_num; i++) {
		reg = MXT_OBJECT_START + MXT_OBJECT_SIZE * i;
		error = mxt_read_object_table(data->client,
						reg, obj_buf);
		if (error)
			return error;

		if (obj_buf[0] == type)
			return obj_buf[2] << 8 | obj_buf[1];
	}
	/* If control reaches here, i = obj_num and object not found */
	dev_err(dev, "Requested object %d not found.\n", type);
	return -EINVAL;

}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int mxt_write_object(struct mxt_data *data,
				 u8 type, u8 offset, u8 val)
{
	struct mxt_object *object;
	u16 reg;

	object = mxt_get_object(data, type);
<<<<<<< HEAD
	if (!object)
=======
	if (!object || offset >= object->size + 1)
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	reg = object->start_address;
	return mxt_write_reg(data->client, reg + offset, val);
}

<<<<<<< HEAD
static void mxt_input_report(struct mxt_data *data, int single_id)
{
	struct mxt_finger *finger = data->finger;
	struct input_dev *input_dev = data->input_dev;
	int status = finger[single_id].status;
	int finger_num = 0;
	int id;

	for (id = 0; id < MXT_MAX_FINGER; id++) {
		if (!finger[id].status)
			continue;

<<<<<<< HEAD
		input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR,
				finger[id].status != MXT_RELEASE ?
				finger[id].area : 0);
		input_report_abs(input_dev, ABS_MT_POSITION_X,
				finger[id].x);
		input_report_abs(input_dev, ABS_MT_POSITION_Y,
				finger[id].y);
		input_report_abs(input_dev, ABS_MT_PRESSURE,
				finger[id].status != MXT_RELEASE ?
				finger[id].pressure : 0);
		input_mt_sync(input_dev);

		if (finger[id].status == MXT_RELEASE)
			finger[id].status = 0;
		else
			finger_num++;
=======
		input_mt_slot(input_dev, id);
		/* Firmware reports min/max values when the touch is
		 * outside screen area. Send a release event in
		 * such cases to avoid unwanted touches.
		 */
		if (finger[id].x <= data->pdata->panel_minx ||
				finger[id].x >= data->pdata->panel_maxx ||
				finger[id].y <= data->pdata->panel_miny ||
				finger[id].y >= data->pdata->panel_maxy) {
			finger[id].status = MXT_RELEASE;
		}

		input_mt_report_slot_state(input_dev, MT_TOOL_FINGER,
				finger[id].status != MXT_RELEASE);

		if (finger[id].status != MXT_RELEASE) {
			finger_num++;
			input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR,
					finger[id].area);
			input_report_abs(input_dev, ABS_MT_POSITION_X,
					finger[id].x);
			input_report_abs(input_dev, ABS_MT_POSITION_Y,
					finger[id].y);
			input_report_abs(input_dev, ABS_MT_PRESSURE,
					 finger[id].pressure);
		} else {
			finger[id].status = 0;
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	input_report_key(input_dev, BTN_TOUCH, finger_num > 0);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (finger[single_id].x <= data->pdata->panel_minx ||
		finger[single_id].x >= data->pdata->panel_maxx ||
		finger[single_id].y <= data->pdata->panel_miny ||
		finger[single_id].y >= data->pdata->panel_maxy) {
		status = MXT_RELEASE;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (status != MXT_RELEASE) {
		input_report_abs(input_dev, ABS_X, finger[single_id].x);
		input_report_abs(input_dev, ABS_Y, finger[single_id].y);
		input_report_abs(input_dev,
			ABS_PRESSURE, finger[single_id].pressure);
	}

	input_sync(input_dev);
=======
static void mxt_input_button(struct mxt_data *data, struct mxt_message *message)
{
	struct input_dev *input = data->input_dev;
	bool button;
	int i;

	/* Active-low switch */
	for (i = 0; i < MXT_NUM_GPIO; i++) {
		if (data->pdata->key_map[i] == KEY_RESERVED)
			continue;
		button = !(message->message[0] & MXT_GPIO0_MASK << i);
		input_report_key(input, data->pdata->key_map[i], button);
	}
>>>>>>> refs/remotes/origin/master
}

static void mxt_input_touchevent(struct mxt_data *data,
				      struct mxt_message *message, int id)
{
<<<<<<< HEAD
	struct mxt_finger *finger = data->finger;
	struct device *dev = &data->client->dev;
	u8 status = message->message[0];
=======
	struct device *dev = &data->client->dev;
	u8 status = message->message[0];
	struct input_dev *input_dev = data->input_dev;
>>>>>>> refs/remotes/origin/master
	int x;
	int y;
	int area;
	int pressure;

<<<<<<< HEAD
	/* Check the touch is present on the screen */
	if (!(status & MXT_DETECT)) {
		if (status & MXT_RELEASE) {
			dev_dbg(dev, "[%d] released\n", id);

			finger[id].status = MXT_RELEASE;
			mxt_input_report(data, id);
		}
		return;
	}

	/* Check only AMP detection */
	if (!(status & (MXT_PRESS | MXT_MOVE)))
		return;

	x = (message->message[1] << 4) | ((message->message[3] >> 4) & 0xf);
	y = (message->message[2] << 4) | ((message->message[3] & 0xf));
<<<<<<< HEAD
<<<<<<< HEAD
	if (data->pdata->x_size < 1024)
		x = x >> 2;
	if (data->pdata->y_size < 1024)
=======
	if (data->pdata->panel_maxx < 1024)
		x = x >> 2;
	if (data->pdata->panel_maxy < 1024)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	x = (message->message[1] << 4) | ((message->message[3] >> 4) & 0xf);
	y = (message->message[2] << 4) | ((message->message[3] & 0xf));
	if (data->max_x < 1024)
		x = x >> 2;
	if (data->max_y < 1024)
>>>>>>> refs/remotes/origin/master
=======
	if (data->pdata->panel_maxx < 1024)
		x = x >> 2;
	if (data->pdata->panel_maxy < 1024)
>>>>>>> refs/remotes/origin/cm-11.0
		y = y >> 2;

	area = message->message[4];
	pressure = message->message[5];

<<<<<<< HEAD
	dev_dbg(dev, "[%d] %s x: %d, y: %d, area: %d\n", id,
		status & MXT_MOVE ? "moved" : "pressed",
		x, y, area);

	finger[id].status = status & MXT_MOVE ?
				MXT_MOVE : MXT_PRESS;
	finger[id].x = x;
	finger[id].y = y;
	finger[id].area = area;
	finger[id].pressure = pressure;

	mxt_input_report(data, id);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static void mxt_handle_key_array(struct mxt_data *data,
				struct mxt_message *message)
{
	u32 keys_changed;
	int i;

	if (!data->pdata->key_codes) {
		dev_err(&data->client->dev, "keyarray is not supported\n");
		return;
	}

	data->keyarray_new = message->message[1] |
				(message->message[2] << 8) |
				(message->message[3] << 16) |
				(message->message[4] << 24);

	keys_changed = data->keyarray_old ^ data->keyarray_new;

	if (!keys_changed) {
		dev_dbg(&data->client->dev, "no keys changed\n");
		return;
	}

	for (i = 0; i < MXT_KEYARRAY_MAX_KEYS; i++) {
		if (!(keys_changed & (1 << i)))
			continue;

		input_report_key(data->input_dev, data->pdata->key_codes[i],
					(data->keyarray_new & (1 << i)));
		input_sync(data->input_dev);
	}

	data->keyarray_old = data->keyarray_new;
}

static void mxt_release_all(struct mxt_data *data)
{
	int id;

	for (id = 0; id < MXT_MAX_FINGER; id++)
		if (data->finger[id].status)
			data->finger[id].status = MXT_RELEASE;

	mxt_input_report(data, 0);
}

static void mxt_handle_touch_supression(struct mxt_data *data, u8 status)
{
	dev_dbg(&data->client->dev, "touch suppression\n");
	/* release all touches */
	if (status & MXT_TCHSUP_ACTIVE)
		mxt_release_all(data);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev_dbg(dev,
		"[%u] %c%c%c%c%c%c%c%c x: %5u y: %5u area: %3u amp: %3u\n",
		id,
		(status & MXT_DETECT) ? 'D' : '.',
		(status & MXT_PRESS) ? 'P' : '.',
		(status & MXT_RELEASE) ? 'R' : '.',
		(status & MXT_MOVE) ? 'M' : '.',
		(status & MXT_VECTOR) ? 'V' : '.',
		(status & MXT_AMP) ? 'A' : '.',
		(status & MXT_SUPPRESS) ? 'S' : '.',
		(status & MXT_UNGRIP) ? 'U' : '.',
		x, y, area, pressure);

	input_mt_slot(input_dev, id);
	input_mt_report_slot_state(input_dev, MT_TOOL_FINGER,
				   status & MXT_DETECT);

	if (status & MXT_DETECT) {
		input_report_abs(input_dev, ABS_MT_POSITION_X, x);
		input_report_abs(input_dev, ABS_MT_POSITION_Y, y);
		input_report_abs(input_dev, ABS_MT_PRESSURE, pressure);
		input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR, area);
	}
}

static unsigned mxt_extract_T6_csum(const u8 *csum)
{
	return csum[0] | (csum[1] << 8) | (csum[2] << 16);
}

static bool mxt_is_T9_message(struct mxt_data *data, struct mxt_message *msg)
{
	u8 id = msg->reportid;
	return (id >= data->T9_reportid_min && id <= data->T9_reportid_max);
}

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static irqreturn_t mxt_interrupt(int irq, void *dev_id)
{
	struct mxt_data *data = dev_id;
	struct mxt_message message;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	struct mxt_object *object;
	struct device *dev = &data->client->dev;
	int id;
	u8 reportid;
	u8 max_reportid;
	u8 min_reportid;
=======
	struct device *dev = &data->client->dev;
	int id;
	u8 reportid;
=======
	struct device *dev = &data->client->dev;
	int id;
	u8 reportid;
>>>>>>> refs/remotes/origin/cm-11.0

	if (data->state != APPMODE) {
		dev_err(dev, "Ignoring IRQ - not in APPMODE state\n");
		return IRQ_HANDLED;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const u8 *payload = &message.message[0];
	struct device *dev = &data->client->dev;
	u8 reportid;
	bool update_input = false;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	do {
		if (mxt_read_message(data, &message)) {
			dev_err(dev, "Failed to read message\n");
			goto end;
		}
<<<<<<< HEAD
<<<<<<< HEAD
		reportid = message.reportid;

<<<<<<< HEAD
		/* whether reportid is thing of MXT_TOUCH_MULTI_T9 */
		object = mxt_get_object(data, MXT_TOUCH_MULTI_T9);
		if (!object)
			goto end;
		max_reportid = object->max_reportid;
		min_reportid = max_reportid - object->num_report_ids + 1;
		id = reportid - min_reportid;
=======
		reportid = message.reportid;

		if (!reportid) {
			dev_dbg(dev, "Report id 0 is reserved\n");
			continue;
		}

		/* check whether report id is part of T9 or T15 */
		id = reportid - data->t9_min_reportid;
>>>>>>> refs/remotes/origin/cm-11.0

		if (reportid >= data->t9_min_reportid &&
					reportid <= data->t9_max_reportid)
			mxt_input_touchevent(data, &message, id);
<<<<<<< HEAD
=======
		if (!reportid) {
			dev_dbg(dev, "Report id 0 is reserved\n");
			continue;
		}

		/* check whether report id is part of T9 or T15 */
		id = reportid - data->t9_min_reportid;

		if (reportid >= data->t9_min_reportid &&
					reportid <= data->t9_max_reportid)
			mxt_input_touchevent(data, &message, id);
=======
>>>>>>> refs/remotes/origin/cm-11.0
		else if (reportid >= data->t15_min_reportid &&
					reportid <= data->t15_max_reportid)
			mxt_handle_key_array(data, &message);
		else if (reportid >= data->t42_min_reportid &&
					reportid <= data->t42_max_reportid)
			mxt_handle_touch_supression(data, message.message[0]);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
		else
			mxt_dump_message(dev, &message);
	} while (reportid != 0xff);

=======

		reportid = message.reportid;

		if (reportid == data->T6_reportid) {
			u8 status = payload[0];
			unsigned csum = mxt_extract_T6_csum(&payload[1]);
			dev_dbg(dev, "Status: %02x Config Checksum: %06x\n",
				status, csum);
		} else if (mxt_is_T9_message(data, &message)) {
			int id = reportid - data->T9_reportid_min;
			mxt_input_touchevent(data, &message, id);
			update_input = true;
		} else if (message.reportid == data->T19_reportid) {
			mxt_input_button(data, &message);
			update_input = true;
		} else {
			mxt_dump_message(dev, &message);
		}
	} while (reportid != 0xff);

	if (update_input) {
		input_mt_report_pointer_emulation(data->input_dev, false);
		input_sync(data->input_dev);
	}

>>>>>>> refs/remotes/origin/master
end:
	return IRQ_HANDLED;
}

static int mxt_check_reg_init(struct mxt_data *data)
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	const struct mxt_platform_data *pdata = data->pdata;
=======
	const struct mxt_config_info *config_info = data->config_info;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const struct mxt_config_info *config_info = data->config_info;
>>>>>>> refs/remotes/origin/cm-11.0
	struct mxt_object *object;
	struct device *dev = &data->client->dev;
	int index = 0;
	int i, j, config_offset;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!pdata->config) {
=======
	if (!config_info) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const struct mxt_platform_data *pdata = data->pdata;
	struct mxt_object *object;
	struct device *dev = &data->client->dev;
	int index = 0;
	int i, size;
	int ret;

	if (!pdata->config) {
>>>>>>> refs/remotes/origin/master
=======
	if (!config_info) {
>>>>>>> refs/remotes/origin/cm-11.0
		dev_dbg(dev, "No cfg data defined, skipping reg init\n");
		return 0;
	}

	for (i = 0; i < data->info.object_num; i++) {
		object = data->object_table + i;

		if (!mxt_object_writable(object->type))
			continue;

<<<<<<< HEAD
<<<<<<< HEAD
		for (j = 0; j < object->size + 1; j++) {
			config_offset = index + j;
<<<<<<< HEAD
			if (config_offset > pdata->config_length) {
=======
			if (config_offset > config_info->config_length) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		for (j = 0; j < object->size + 1; j++) {
			config_offset = index + j;
			if (config_offset > config_info->config_length) {
>>>>>>> refs/remotes/origin/cm-11.0
				dev_err(dev, "Not enough config data!\n");
				return -EINVAL;
			}
			mxt_write_object(data, object->type, j,
<<<<<<< HEAD
<<<<<<< HEAD
					 pdata->config[config_offset]);
=======
					 config_info->config[config_offset]);
>>>>>>> refs/remotes/origin/cm-10.0
		}
		index += object->size + 1;
=======
		size = (object->size + 1) * (object->instances + 1);
		if (index + size > pdata->config_length) {
			dev_err(dev, "Not enough config data!\n");
			return -EINVAL;
		}

		ret = __mxt_write_reg(data->client, object->start_address,
				size, &pdata->config[index]);
		if (ret)
			return ret;
		index += size;
>>>>>>> refs/remotes/origin/master
=======
					 config_info->config[config_offset]);
		}
		index += object->size + 1;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	return 0;
}

static int mxt_make_highchg(struct mxt_data *data)
{
	struct device *dev = &data->client->dev;
	struct mxt_message message;
	int count = 10;
	int error;

	/* Read dummy message to make high CHG pin */
	do {
		error = mxt_read_message(data, &message);
		if (error)
			return error;
	} while (message.reportid != 0xff && --count);

	if (!count) {
		dev_err(dev, "CHG pin isn't cleared\n");
		return -EBUSY;
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static void mxt_handle_pdata(struct mxt_data *data)
{
	const struct mxt_platform_data *pdata = data->pdata;
	u8 voltage;

	/* Set touchscreen lines */
	mxt_write_object(data, MXT_TOUCH_MULTI_T9, MXT_TOUCH_XSIZE,
			pdata->x_line);
	mxt_write_object(data, MXT_TOUCH_MULTI_T9, MXT_TOUCH_YSIZE,
			pdata->y_line);

	/* Set touchscreen orient */
	mxt_write_object(data, MXT_TOUCH_MULTI_T9, MXT_TOUCH_ORIENT,
			pdata->orient);

	/* Set touchscreen burst length */
	mxt_write_object(data, MXT_TOUCH_MULTI_T9,
			MXT_TOUCH_BLEN, pdata->blen);

	/* Set touchscreen threshold */
	mxt_write_object(data, MXT_TOUCH_MULTI_T9,
			MXT_TOUCH_TCHTHR, pdata->threshold);

	/* Set touchscreen resolution */
	mxt_write_object(data, MXT_TOUCH_MULTI_T9,
			MXT_TOUCH_XRANGE_LSB, (pdata->x_size - 1) & 0xff);
	mxt_write_object(data, MXT_TOUCH_MULTI_T9,
			MXT_TOUCH_XRANGE_MSB, (pdata->x_size - 1) >> 8);
	mxt_write_object(data, MXT_TOUCH_MULTI_T9,
			MXT_TOUCH_YRANGE_LSB, (pdata->y_size - 1) & 0xff);
	mxt_write_object(data, MXT_TOUCH_MULTI_T9,
			MXT_TOUCH_YRANGE_MSB, (pdata->y_size - 1) >> 8);

	/* Set touchscreen voltage */
	if (pdata->voltage) {
		if (pdata->voltage < MXT_VOLTAGE_DEFAULT) {
			voltage = (MXT_VOLTAGE_DEFAULT - pdata->voltage) /
				MXT_VOLTAGE_STEP;
			voltage = 0xff - voltage + 1;
		} else
			voltage = (pdata->voltage - MXT_VOLTAGE_DEFAULT) /
				MXT_VOLTAGE_STEP;

		mxt_write_object(data, MXT_SPT_CTECONFIG_T28,
				MXT_CTE_VOLTAGE, voltage);
	}
}

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int mxt_get_info(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	struct mxt_info *info = &data->info;
	int error;
<<<<<<< HEAD
	u8 val;

	error = mxt_read_reg(client, MXT_FAMILY_ID, &val);
	if (error)
		return error;
	info->family_id = val;

	error = mxt_read_reg(client, MXT_VARIANT_ID, &val);
	if (error)
		return error;
	info->variant_id = val;

	error = mxt_read_reg(client, MXT_VERSION, &val);
	if (error)
		return error;
	info->version = val;

	error = mxt_read_reg(client, MXT_BUILD, &val);
	if (error)
		return error;
	info->build = val;

	error = mxt_read_reg(client, MXT_OBJECT_NUM, &val);
	if (error)
		return error;
	info->object_num = val;
=======

	/* Read 7-byte info block starting at address 0 */
	error = __mxt_read_reg(client, MXT_INFO, sizeof(*info), info);
	if (error)
		return error;
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int mxt_get_object_table(struct mxt_data *data)
{
<<<<<<< HEAD
	int error;
	int i;
	u16 reg;
	u8 reportid = 0;
	u8 buf[MXT_OBJECT_SIZE];
<<<<<<< HEAD
<<<<<<< HEAD
=======
	bool found_t38 = false;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bool found_t38 = false;
>>>>>>> refs/remotes/origin/cm-11.0

	for (i = 0; i < data->info.object_num; i++) {
		struct mxt_object *object = data->object_table + i;

		reg = MXT_OBJECT_START + MXT_OBJECT_SIZE * i;
		error = mxt_read_object_table(data->client, reg, buf);
		if (error)
			return error;

		object->type = buf[0];
		object->start_address = (buf[2] << 8) | buf[1];
		object->size = buf[3];
		object->instances = buf[4];
		object->num_report_ids = buf[5];

		if (object->num_report_ids) {
			reportid += object->num_report_ids *
					(object->instances + 1);
			object->max_reportid = reportid;
		}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======

		/* Calculate index for config major version in config array.
		 * Major version is the first byte in object T38.
		 */
		if (object->type == MXT_SPT_USERDATA_T38) {
			data->t38_start_addr = object->start_address;
			found_t38 = true;
		}
		if (!found_t38 && mxt_object_writable(object->type))
			data->cfg_version_idx += object->size + 1;
	}
>>>>>>> refs/remotes/origin/cm-11.0

		/* Calculate index for config major version in config array.
		 * Major version is the first byte in object T38.
		 */
		if (object->type == MXT_SPT_USERDATA_T38) {
			data->t38_start_addr = object->start_address;
			found_t38 = true;
		}
		if (!found_t38 && mxt_object_writable(object->type))
			data->cfg_version_idx += object->size + 1;
=======
	struct i2c_client *client = data->client;
	size_t table_size;
	int error;
	int i;
	u8 reportid;

	table_size = data->info.object_num * sizeof(struct mxt_object);
	error = __mxt_read_reg(client, MXT_OBJECT_START, table_size,
			data->object_table);
	if (error)
		return error;

	/* Valid Report IDs start counting from 1 */
	reportid = 1;
	for (i = 0; i < data->info.object_num; i++) {
		struct mxt_object *object = data->object_table + i;
		u8 min_id, max_id;

		le16_to_cpus(&object->start_address);

		if (object->num_report_ids) {
			min_id = reportid;
			reportid += object->num_report_ids *
					(object->instances + 1);
			max_id = reportid - 1;
		} else {
			min_id = 0;
			max_id = 0;
		}

		dev_dbg(&data->client->dev,
			"Type %2d Start %3d Size %3d Instances %2d ReportIDs %3u : %3u\n",
			object->type, object->start_address, object->size + 1,
			object->instances + 1, min_id, max_id);

		switch (object->type) {
		case MXT_GEN_COMMAND_T6:
			data->T6_reportid = min_id;
			break;
		case MXT_TOUCH_MULTI_T9:
			data->T9_reportid_min = min_id;
			data->T9_reportid_max = max_id;
			break;
		case MXT_SPT_GPIOPWM_T19:
			data->T19_reportid = min_id;
			break;
		}
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

<<<<<<< HEAD
static int compare_versions(const u8 *v1, const u8 *v2)
{
	int i;

	if (!v1 || !v2)
		return -EINVAL;

	/* The major version number stays the same across different versions for
	 * a particular controller on a target. The minor and sub-minor version
	 * numbers indicate which version is newer.
	 */
	if (v1[0] != v2[0])
		return -EINVAL;

	for (i = 1; i < MXT_CFG_VERSION_LEN; i++) {
		if (v1[i] > v2[i])
			return MXT_CFG_VERSION_LESS;	/* v2 is older */

		if (v1[i] < v2[i])
			return MXT_CFG_VERSION_GREATER;	/* v2 is newer */
	}

	return MXT_CFG_VERSION_EQUAL;	/* v1 and v2 are equal */
}

static void mxt_check_config_version(struct mxt_data *data,
			const struct mxt_config_info *cfg_info,
			bool match_major,
			const u8 **cfg_version_found,
			bool *found_cfg_major_match)
{
	const u8 *cfg_version;
	int result = -EINVAL;

	cfg_version = cfg_info->config + data->cfg_version_idx;

	if (*cfg_version_found)
		result = compare_versions(*cfg_version_found, cfg_version);

	if (match_major) {
		if (result >= MXT_CFG_VERSION_EQUAL)
			*found_cfg_major_match = true;

		if (result == MXT_CFG_VERSION_EQUAL ||
			result == MXT_CFG_VERSION_GREATER) {
			data->config_info = cfg_info;
			data->fw_name = cfg_info->fw_name;
			*cfg_version_found = cfg_version;
		}

		if (result == MXT_CFG_VERSION_GREATER)
			data->update_cfg = true;
	} else if (!*cfg_version_found || result == MXT_CFG_VERSION_GREATER) {
		data->config_info = cfg_info;
		data->fw_name = cfg_info->fw_name;
		data->update_cfg = true;
		*cfg_version_found = cfg_version;
	}
}

/* If the controller's config version has a non-zero major number, call this
 * function with match_major = true to look for the latest config present in
 * the pdata based on matching family id, variant id, f/w version, build, and
 * config major number. If the controller is programmed with wrong config data
 * previously, call this function with match_major = false to look for latest
 * config based on based on matching family id, variant id, f/w version and
 * build only.
 */
static int mxt_search_config_array(struct mxt_data *data, bool match_major)
{

	const struct mxt_platform_data *pdata = data->pdata;
	const struct mxt_config_info *cfg_info;
	const struct mxt_info *info = &data->info;
	const u8 *cfg_version_found;
	bool found_cfg_major_match = false;
	int i;

	cfg_version_found = match_major ? data->cfg_version : NULL;

	for (i = 0; i < pdata->config_array_size; i++) {

		cfg_info = &pdata->config_array[i];

		if (!cfg_info->config || !cfg_info->config_length)
			continue;

		if (info->family_id == cfg_info->family_id &&
			info->variant_id == cfg_info->variant_id &&
			info->version == cfg_info->version &&
			info->build == cfg_info->build) {

			mxt_check_config_version(data, cfg_info, match_major,
				&cfg_version_found, &found_cfg_major_match);
		}
	}

	if (data->config_info || found_cfg_major_match)
		return 0;

	data->config_info = NULL;
	data->fw_name = NULL;

	return -EINVAL;
}

static int mxt_get_config(struct mxt_data *data)
{
	const struct mxt_platform_data *pdata = data->pdata;
	struct device *dev = &data->client->dev;
	struct mxt_object *object;
	int error;

	if (!pdata->config_array || !pdata->config_array_size) {
		dev_dbg(dev, "No cfg data provided by platform data\n");
		return 0;
	}

	/* Get current config version */
	object = mxt_get_object(data, MXT_SPT_USERDATA_T38);
	if (!object) {
		dev_err(dev, "Unable to obtain USERDATA object\n");
		return -EINVAL;
	}

	error = __mxt_read_reg(data->client, object->start_address,
				sizeof(data->cfg_version), data->cfg_version);
	if (error) {
		dev_err(dev, "Unable to read config version\n");
		return error;
	}
	dev_info(dev, "Current config version on the controller is %d.%d.%d\n",
			data->cfg_version[0], data->cfg_version[1],
			data->cfg_version[2]);

	/* It is possible that the config data on the controller is not
	 * versioned and the version number returns 0. In this case,
	 * find a match without the config version checking.
	 */
	error = mxt_search_config_array(data,
				data->cfg_version[0] != 0 ? true : false);
	if (error) {
		/* If a match wasn't found for a non-zero config version,
		 * it means the controller has the wrong config data. Search
		 * for a best match based on controller and firmware version,
		 * but not config version.
		 */
		if (data->cfg_version[0])
			error = mxt_search_config_array(data, false);
		if (error) {
			dev_err(dev,
				"Unable to find matching config in pdata\n");
			return error;
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	return 0;
}

static void mxt_reset_delay(struct mxt_data *data)
{
	struct mxt_info *info = &data->info;

	switch (info->family_id) {
	case MXT224_ID:
		msleep(MXT224_RESET_TIME);
		break;
	case MXT224E_ID:
		msleep(MXT224E_RESET_TIME);
		break;
	case MXT1386_ID:
		msleep(MXT1386_RESET_TIME);
		break;
	default:
		msleep(MXT_RESET_TIME);
	}
}

<<<<<<< HEAD
static int mxt_initialize(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	struct mxt_info *info = &data->info;
	int error;
	int timeout_counter = 0;
	u8 val;
	u8 command_register;
	struct mxt_object *t7_object;

	error = mxt_get_info(data);
	if (error)
		return error;

	data->object_table = kcalloc(info->object_num,
				     sizeof(struct mxt_object),
				     GFP_KERNEL);
	if (!data->object_table) {
		dev_err(&client->dev, "Failed to allocate memory\n");
		return -ENOMEM;
	}

	/* Get object table information */
	error = mxt_get_object_table(data);
	if (error)
		goto free_object_table;

	/* Check register init values */
	error = mxt_check_reg_init(data);
	if (error)
		goto free_object_table;
=======
static int mxt_backup_nv(struct mxt_data *data)
{
	int error;
	u8 command_register;
	int timeout_counter = 0;

	/* Backup to memory */
	mxt_write_object(data, MXT_GEN_COMMAND_T6,
			MXT_COMMAND_BACKUPNV,
			MXT_BACKUP_VALUE);
	msleep(MXT_BACKUP_TIME);

	do {
		error = mxt_read_object(data, MXT_GEN_COMMAND_T6,
					MXT_COMMAND_BACKUPNV,
					&command_register);
		if (error)
			return error;

		usleep_range(1000, 2000);

	} while ((command_register != 0) && (++timeout_counter <= 100));

	if (timeout_counter > 100) {
		dev_err(&data->client->dev, "No response after backup!\n");
		return -EIO;
	}

	/* Soft reset */
	mxt_write_object(data, MXT_GEN_COMMAND_T6, MXT_COMMAND_RESET, 1);

	mxt_reset_delay(data);

	return 0;
}

static int mxt_save_objects(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	struct mxt_object *t7_object;
	struct mxt_object *t9_object;
	struct mxt_object *t15_object;
	struct mxt_object *t42_object;
	int error;
>>>>>>> refs/remotes/origin/cm-10.0

	/* Store T7 and T9 locally, used in suspend/resume operations */
	t7_object = mxt_get_object(data, MXT_GEN_POWER_T7);
	if (!t7_object) {
		dev_err(&client->dev, "Failed to get T7 object\n");
<<<<<<< HEAD
		error = -EINVAL;
		goto free_object_table;
=======
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	data->t7_start_addr = t7_object->start_address;
	error = __mxt_read_reg(client, data->t7_start_addr,
				T7_DATA_SIZE, data->t7_data);
	if (error < 0) {
		dev_err(&client->dev,
			"Failed to save current power state\n");
<<<<<<< HEAD
		goto free_object_table;
	}
	error = mxt_read_object(data, MXT_TOUCH_MULTI_T9, MXT_TOUCH_CTRL,
			&data->t9_ctrl);
	if (error < 0) {
		dev_err(&client->dev, "Failed to save current touch object\n");
		goto free_object_table;
	}
=======
static void mxt_free_object_table(struct mxt_data *data)
{
	kfree(data->object_table);
	data->object_table = NULL;
	data->T6_reportid = 0;
	data->T9_reportid_min = 0;
	data->T9_reportid_max = 0;
	data->T19_reportid = 0;
}

static int mxt_initialize(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	struct mxt_info *info = &data->info;
	int error;
	u8 val;

	error = mxt_get_info(data);
	if (error)
		return error;

	data->object_table = kcalloc(info->object_num,
				     sizeof(struct mxt_object),
				     GFP_KERNEL);
	if (!data->object_table) {
		dev_err(&client->dev, "Failed to allocate memory\n");
		return -ENOMEM;
	}

	/* Get object table information */
	error = mxt_get_object_table(data);
	if (error)
		goto err_free_object_table;

	/* Check register init values */
	error = mxt_check_reg_init(data);
	if (error)
		goto err_free_object_table;

	mxt_handle_pdata(data);
>>>>>>> refs/remotes/origin/master

	/* Backup to memory */
	mxt_write_object(data, MXT_GEN_COMMAND_T6,
			MXT_COMMAND_BACKUPNV,
			MXT_BACKUP_VALUE);
	msleep(MXT_BACKUP_TIME);
<<<<<<< HEAD
	do {
		error =  mxt_read_object(data, MXT_GEN_COMMAND_T6,
					MXT_COMMAND_BACKUPNV,
					&command_register);
		if (error)
			goto free_object_table;
		usleep_range(1000, 2000);
	} while ((command_register != 0) && (++timeout_counter <= 100));
	if (timeout_counter > 100) {
		dev_err(&client->dev, "No response after backup!\n");
		error = -EIO;
		goto free_object_table;
	}

=======
>>>>>>> refs/remotes/origin/master

	/* Soft reset */
	mxt_write_object(data, MXT_GEN_COMMAND_T6,
			MXT_COMMAND_RESET, 1);
<<<<<<< HEAD

	mxt_reset_delay(data);
=======
		return error;
	}

	/* Store T9, T15's min and max report ids */
	t9_object = mxt_get_object(data, MXT_TOUCH_MULTI_T9);
	if (!t9_object) {
		dev_err(&client->dev, "Failed to get T9 object\n");
		return -EINVAL;
	}
	data->t9_max_reportid = t9_object->max_reportid;
	data->t9_min_reportid = t9_object->max_reportid -
					t9_object->num_report_ids + 1;

	if (data->pdata->key_codes) {
		t15_object = mxt_get_object(data, MXT_TOUCH_KEYARRAY_T15);
		if (!t15_object)
			dev_dbg(&client->dev, "T15 object is not available\n");
		else {
			data->t15_max_reportid = t15_object->max_reportid;
			data->t15_min_reportid = t15_object->max_reportid -
						t15_object->num_report_ids + 1;
		}
	}

	/* Store T42 min and max report ids */
	t42_object = mxt_get_object(data, MXT_PROCI_TOUCHSUPPRESSION_T42);
	if (!t42_object)
		dev_dbg(&client->dev, "T42 object is not available\n");
	else {
		data->t42_max_reportid = t42_object->max_reportid;
		data->t42_min_reportid = t42_object->max_reportid -
					t42_object->num_report_ids + 1;
	}

	return 0;
}

static int mxt_initialize(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	struct mxt_info *info = &data->info;
	int error;
	u8 val;
	const u8 *cfg_ver;

	error = mxt_get_info(data);
	if (error) {
		/* Try bootloader mode */
		error = mxt_switch_to_bootloader_address(data);
		if (error)
			return error;

		error = mxt_check_bootloader(client, MXT_APP_CRC_FAIL);
		if (error)
			return error;

		dev_err(&client->dev, "Application CRC failure\n");
		data->state = BOOTLOADER;

		return 0;
	}

	dev_info(&client->dev,
			"Family ID: %d Variant ID: %d Version: %d.%d "
			"Build: 0x%02X Object Num: %d\n",
			info->family_id, info->variant_id,
			info->version >> 4, info->version & 0xf,
			info->build, info->object_num);

	data->state = APPMODE;

	data->object_table = kcalloc(info->object_num,
				     sizeof(struct mxt_object),
				     GFP_KERNEL);
	if (!data->object_table) {
		dev_err(&client->dev, "Failed to allocate memory\n");
		return -ENOMEM;
	}

	/* Get object table information */
	error = mxt_get_object_table(data);
	if (error)
		goto free_object_table;

	/* Get config data from platform data */
	error = mxt_get_config(data);
	if (error)
		dev_dbg(&client->dev, "Config info not found.\n");

	/* Check register init values */
	if (data->config_info && data->config_info->config) {
		if (data->update_cfg) {
			error = mxt_check_reg_init(data);
			if (error) {
				dev_err(&client->dev,
					"Failed to check reg init value\n");
				goto free_object_table;
			}

			error = mxt_backup_nv(data);
			if (error) {
				dev_err(&client->dev, "Failed to back up NV\n");
				goto free_object_table;
			}

			cfg_ver = data->config_info->config +
							data->cfg_version_idx;
			dev_info(&client->dev,
				"Config updated from %d.%d.%d to %d.%d.%d\n",
				data->cfg_version[0], data->cfg_version[1],
				data->cfg_version[2],
				cfg_ver[0], cfg_ver[1], cfg_ver[2]);

			memcpy(data->cfg_version, cfg_ver, MXT_CFG_VERSION_LEN);
		}
	} else {
		dev_info(&client->dev,
			"No cfg data defined, skipping check reg init\n");
	}

	error = mxt_save_objects(data);
	if (error)
		goto free_object_table;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	msleep(MXT_RESET_TIME);
>>>>>>> refs/remotes/origin/master

	/* Update matrix size at info struct */
	error = mxt_read_reg(client, MXT_MATRIX_X_SIZE, &val);
	if (error)
<<<<<<< HEAD
		goto free_object_table;
=======
		goto err_free_object_table;
>>>>>>> refs/remotes/origin/master
	info->matrix_xsize = val;

	error = mxt_read_reg(client, MXT_MATRIX_Y_SIZE, &val);
	if (error)
<<<<<<< HEAD
		goto free_object_table;
	info->matrix_ysize = val;

	dev_info(&client->dev,
<<<<<<< HEAD
			"Family ID: %d Variant ID: %d Version: %d Build: %d\n",
			info->family_id, info->variant_id, info->version,
			info->build);

	dev_info(&client->dev,
			"Matrix X Size: %d Matrix Y Size: %d Object Num: %d\n",
			info->matrix_xsize, info->matrix_ysize,
			info->object_num);
=======
			"Matrix X Size: %d Matrix Y Size: %d\n",
			info->matrix_xsize, info->matrix_ysize);
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;

free_object_table:
	kfree(data->object_table);
	return error;
}

static ssize_t mxt_object_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	struct mxt_object *object;
	int count = 0;
	int i, j;
	int error;
	u8 val;

	for (i = 0; i < data->info.object_num; i++) {
		object = data->object_table + i;

		count += snprintf(buf + count, PAGE_SIZE - count,
				"Object[%d] (Type %d)\n",
				i + 1, object->type);
		if (count >= PAGE_SIZE)
			return PAGE_SIZE - 1;

		if (!mxt_object_readable(object->type)) {
			count += snprintf(buf + count, PAGE_SIZE - count,
					"\n");
			if (count >= PAGE_SIZE)
				return PAGE_SIZE - 1;
			continue;
		}

		for (j = 0; j < object->size + 1; j++) {
			error = mxt_read_object(data,
						object->type, j, &val);
			if (error)
				return error;

			count += snprintf(buf + count, PAGE_SIZE - count,
					"\t[%2d]: %02x (%d)\n", j, val, val);
			if (count >= PAGE_SIZE)
				return PAGE_SIZE - 1;
		}

		count += snprintf(buf + count, PAGE_SIZE - count, "\n");
		if (count >= PAGE_SIZE)
			return PAGE_SIZE - 1;
	}

	return count;
}

<<<<<<< HEAD
=======
static int strtobyte(const char *data, u8 *value)
{
	char str[3];

	str[0] = data[0];
	str[1] = data[1];
	str[2] = '\0';

	return kstrtou8(str, 16, value);
}

>>>>>>> refs/remotes/origin/cm-10.0
static int mxt_load_fw(struct device *dev, const char *fn)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	struct i2c_client *client = data->client;
	const struct firmware *fw = NULL;
	unsigned int frame_size;
<<<<<<< HEAD
	unsigned int pos = 0;
	int ret;

	ret = request_firmware(&fw, fn, dev);
	if (ret) {
		dev_err(dev, "Unable to open firmware %s\n", fn);
		return ret;
	}

	/* Change to the bootloader mode */
	mxt_write_object(data, MXT_GEN_COMMAND_T6,
			MXT_COMMAND_RESET, MXT_BOOT_VALUE);

	mxt_reset_delay(data);

	/* Change to slave address of bootloader */
	if (client->addr == MXT_APP_LOW)
		client->addr = MXT_BOOT_LOW;
	else
		client->addr = MXT_BOOT_HIGH;

	ret = mxt_check_bootloader(client, MXT_WAITING_BOOTLOAD_CMD);
	if (ret)
		goto out;

	/* Unlock bootloader */
	mxt_unlock_bootloader(client);
=======
	unsigned int retry = 0;
	unsigned int pos = 0;
	int ret, i, max_frame_size;
	u8 *frame;

	switch (data->info.family_id) {
	case MXT224_ID:
	case MXT224E_ID:
		max_frame_size = MXT_SINGLE_FW_MAX_FRAME_SIZE;
		break;
	case MXT1386_ID:
		max_frame_size = MXT_CHIPSET_FW_MAX_FRAME_SIZE;
		break;
	default:
		return -EINVAL;
	}

	frame = kmalloc(max_frame_size, GFP_KERNEL);
	if (!frame) {
		dev_err(dev, "Unable to allocate memory for frame data\n");
		return -ENOMEM;
	}

	ret = request_firmware(&fw, fn, dev);
	if (ret < 0) {
		dev_err(dev, "Unable to open firmware %s\n", fn);
		goto free_frame;
	}

	if (data->state != BOOTLOADER) {
		/* Change to the bootloader mode */
		mxt_write_object(data, MXT_GEN_COMMAND_T6,
				MXT_COMMAND_RESET, MXT_BOOT_VALUE);
		mxt_reset_delay(data);

		ret = mxt_switch_to_bootloader_address(data);
		if (ret)
			goto release_firmware;
	}

	ret = mxt_check_bootloader(client, MXT_WAITING_BOOTLOAD_CMD);
	if (ret) {
		/* Bootloader may still be unlocked from previous update
		 * attempt */
		ret = mxt_check_bootloader(client,
			MXT_WAITING_FRAME_DATA);

		if (ret)
			goto return_to_app_mode;
	} else {
		dev_info(dev, "Unlocking bootloader\n");
		/* Unlock bootloader */
		mxt_unlock_bootloader(client);
	}
>>>>>>> refs/remotes/origin/cm-10.0

	while (pos < fw->size) {
		ret = mxt_check_bootloader(client,
						MXT_WAITING_FRAME_DATA);
		if (ret)
<<<<<<< HEAD
			goto out;

		frame_size = ((*(fw->data + pos) << 8) | *(fw->data + pos + 1));
=======
			goto release_firmware;

		/* Get frame length MSB */
		ret = strtobyte(fw->data + pos, frame);
		if (ret)
			goto release_firmware;

		/* Get frame length LSB */
		ret = strtobyte(fw->data + pos + 2, frame + 1);
		if (ret)
			goto release_firmware;

		frame_size = ((*frame << 8) | *(frame + 1));
>>>>>>> refs/remotes/origin/cm-10.0

		/* We should add 2 at frame size as the the firmware data is not
		 * included the CRC bytes.
		 */
		frame_size += 2;

<<<<<<< HEAD
		/* Write one frame to device */
		mxt_fw_write(client, fw->data + pos, frame_size);

		ret = mxt_check_bootloader(client,
						MXT_FRAME_CRC_PASS);
		if (ret)
			goto out;

		pos += frame_size;

		dev_dbg(dev, "Updated %d bytes / %zd bytes\n", pos, fw->size);
	}

out:
	release_firmware(fw);

	/* Change to slave address of application */
	if (client->addr == MXT_BOOT_LOW)
		client->addr = MXT_APP_LOW;
	else
		client->addr = MXT_APP_HIGH;
=======
		if (frame_size > max_frame_size) {
			dev_err(dev, "Invalid frame size - %d\n", frame_size);
			ret = -EINVAL;
			goto release_firmware;
		}

		/* Convert frame data and CRC from hex to binary */
		for (i = 2; i < frame_size; i++) {
			ret = strtobyte(fw->data + pos + i * 2, frame + i);
			if (ret)
				goto release_firmware;
		}

		/* Write one frame to device */
		mxt_fw_write(client, frame, frame_size);

		ret = mxt_check_bootloader(client,
						MXT_FRAME_CRC_PASS);
		if (ret) {
			retry++;

			/* Back off by 20ms per retry */
			msleep(retry * 20);

			if (retry > 20)
				goto release_firmware;
		} else {
			retry = 0;
			pos += frame_size * 2;
			dev_dbg(dev, "Updated %d/%zd bytes\n", pos, fw->size);
		}
	}

return_to_app_mode:
	mxt_switch_to_appmode_address(data);
release_firmware:
	release_firmware(fw);
free_frame:
	kfree(frame);
>>>>>>> refs/remotes/origin/cm-10.0

	return ret;
}

<<<<<<< HEAD
=======
static const char *
mxt_search_fw_name(struct mxt_data *data, u8 bootldr_id)
{
	const struct mxt_platform_data *pdata = data->pdata;
	const struct mxt_config_info *cfg_info;
	const char *fw_name = NULL;
	int i;

	for (i = 0; i < pdata->config_array_size; i++) {
		cfg_info = &pdata->config_array[i];
		if (bootldr_id == cfg_info->bootldr_id && cfg_info->fw_name) {
			data->config_info = cfg_info;
			data->info.family_id = cfg_info->family_id;
			fw_name = cfg_info->fw_name;
		}
	}

	return fw_name;
}

>>>>>>> refs/remotes/origin/cm-10.0
static ssize_t mxt_update_fw_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	struct mxt_data *data = dev_get_drvdata(dev);
<<<<<<< HEAD
	int error;

	disable_irq(data->irq);

	error = mxt_load_fw(dev, MXT_FW_NAME);
=======
	int error, address;
	const char *fw_name;
	u8 bootldr_id;
	u8 cfg_version[MXT_CFG_VERSION_LEN] = {0};


	/* If fw_name is set, then the existing firmware has an upgrade */
	if (!data->fw_name) {
		/*
		 * If the device boots up in the bootloader mode, check if
		 * there is a firmware to upgrade.
		 */
		if (data->state == BOOTLOADER) {
			bootldr_id = mxt_get_bootloader_id(data->client);
			if (bootldr_id <= 0) {
				dev_err(dev,
					"Unable to retrieve bootloader id\n");
				return -EINVAL;
			}
			fw_name = mxt_search_fw_name(data, bootldr_id);
			if (fw_name == NULL) {
				dev_err(dev,
				"Unable to find fw from bootloader id\n");
				return -EINVAL;
			}
		} else {
			/* In APPMODE, if the f/w name does not exist, quit */
			dev_err(dev,
			"Firmware name not specified in platform data\n");
			return -EINVAL;
		}
	} else {
		fw_name = data->fw_name;
	}

	dev_info(dev, "Upgrading the firmware file to %s\n", fw_name);

	disable_irq(data->irq);

	error = mxt_load_fw(dev, fw_name);
>>>>>>> refs/remotes/origin/cm-10.0
	if (error) {
		dev_err(dev, "The firmware update failed(%d)\n", error);
		count = error;
	} else {
<<<<<<< HEAD
		dev_dbg(dev, "The firmware update succeeded\n");
=======
		dev_info(dev, "The firmware update succeeded\n");
>>>>>>> refs/remotes/origin/cm-10.0

		/* Wait for reset */
		msleep(MXT_FWRESET_TIME);

<<<<<<< HEAD
		kfree(data->object_table);
		data->object_table = NULL;
=======
		data->state = INIT;
		kfree(data->object_table);
		data->object_table = NULL;
		data->cfg_version_idx = 0;
		data->update_cfg = false;

		/* T38 object address might have changed, read it from
		   touch controller */
		address = mxt_get_object_address(dev, MXT_SPT_USERDATA_T38);
		if (address < 0) {
			dev_err(dev, "T38 required for touch operation\n");
			return -EINVAL;
		}

		data->t38_start_addr = address;

		error = __mxt_write_reg(data->client, data->t38_start_addr,
				sizeof(cfg_version), cfg_version);
		if (error)
			dev_err(dev,
			"Unable to zero out config version after fw upgrade\n");
>>>>>>> refs/remotes/origin/cm-10.0

		mxt_initialize(data);
	}

<<<<<<< HEAD
	enable_irq(data->irq);

	error = mxt_make_highchg(data);
	if (error)
		return error;
=======
	if (data->state == APPMODE) {
		enable_irq(data->irq);

		error = mxt_make_highchg(data);
		if (error)
			return error;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	return count;
}

static DEVICE_ATTR(object, 0444, mxt_object_show, NULL);
static DEVICE_ATTR(update_fw, 0664, NULL, mxt_update_fw_store);

static struct attribute *mxt_attrs[] = {
	&dev_attr_object.attr,
	&dev_attr_update_fw.attr,
	NULL
};

static const struct attribute_group mxt_attr_group = {
	.attrs = mxt_attrs,
};

static int mxt_start(struct mxt_data *data)
{
	int error;

	/* restore the old power state values and reenable touch */
	error = __mxt_write_reg(data->client, data->t7_start_addr,
				T7_DATA_SIZE, data->t7_data);
	if (error < 0) {
		dev_err(&data->client->dev,
			"failed to restore old power state\n");
		return error;
	}

<<<<<<< HEAD
	error = mxt_write_object(data,
			MXT_TOUCH_MULTI_T9, MXT_TOUCH_CTRL, data->t9_ctrl);
	if (error < 0) {
		dev_err(&data->client->dev, "failed to restore touch\n");
		return error;
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static int mxt_stop(struct mxt_data *data)
{
	int error;
	u8 t7_data[T7_DATA_SIZE] = {0};

<<<<<<< HEAD
	/* disable touch and configure deep sleep mode */
	error = mxt_write_object(data, MXT_TOUCH_MULTI_T9, MXT_TOUCH_CTRL, 0);
	if (error < 0) {
		dev_err(&data->client->dev, "failed to disable touch\n");
		return error;
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
	error = __mxt_write_reg(data->client, data->t7_start_addr,
				T7_DATA_SIZE, t7_data);
	if (error < 0) {
		dev_err(&data->client->dev,
			"failed to configure deep sleep mode\n");
		return error;
	}

	return 0;
}

static int mxt_input_open(struct input_dev *dev)
{
	struct mxt_data *data = input_get_drvdata(dev);
	int error;

<<<<<<< HEAD
	error = mxt_start(data);
	if (error < 0) {
		dev_err(&data->client->dev, "mxt_start failed in input_open\n");
		return error;
=======
	if (data->state == APPMODE) {
		error = mxt_start(data);
		if (error < 0) {
			dev_err(&data->client->dev, "mxt_start failed in input_open\n");
			return error;
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	return 0;
}

static void mxt_input_close(struct input_dev *dev)
{
	struct mxt_data *data = input_get_drvdata(dev);
	int error;

<<<<<<< HEAD
	error = mxt_stop(data);
	if (error < 0)
		dev_err(&data->client->dev, "mxt_stop failed in input_close\n");

=======
	if (data->state == APPMODE) {
		error = mxt_stop(data);
		if (error < 0)
			dev_err(&data->client->dev, "mxt_stop failed in input_close\n");
	}
}

static int reg_set_optimum_mode_check(struct regulator *reg, int load_uA)
{
	return (regulator_count_voltages(reg) > 0) ?
		regulator_set_optimum_mode(reg, load_uA) : 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int mxt_power_on(struct mxt_data *data, bool on)
{
	int rc;

	if (on == false)
		goto power_off;

<<<<<<< HEAD
	rc = regulator_set_optimum_mode(data->vcc_ana, MXT_ACTIVE_LOAD_UA);
=======
	rc = reg_set_optimum_mode_check(data->vcc_ana, MXT_ACTIVE_LOAD_UA);
>>>>>>> refs/remotes/origin/cm-10.0
	if (rc < 0) {
		dev_err(&data->client->dev,
			"Regulator vcc_ana set_opt failed rc=%d\n", rc);
		return rc;
	}

	rc = regulator_enable(data->vcc_ana);
	if (rc) {
		dev_err(&data->client->dev,
			"Regulator vcc_ana enable failed rc=%d\n", rc);
		goto error_reg_en_vcc_ana;
	}

	if (data->pdata->digital_pwr_regulator) {
<<<<<<< HEAD
		rc = regulator_set_optimum_mode(data->vcc_dig,
						MXT_ACTIVE_LOAD_DIG_UA);
=======
		rc = reg_set_optimum_mode_check(data->vcc_dig,
					MXT_ACTIVE_LOAD_DIG_UA);
>>>>>>> refs/remotes/origin/cm-10.0
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_dig set_opt failed rc=%d\n",
				rc);
			goto error_reg_opt_vcc_dig;
		}

		rc = regulator_enable(data->vcc_dig);
		if (rc) {
			dev_err(&data->client->dev,
				"Regulator vcc_dig enable failed rc=%d\n", rc);
			goto error_reg_en_vcc_dig;
		}
	}

	if (data->pdata->i2c_pull_up) {
<<<<<<< HEAD
		rc = regulator_set_optimum_mode(data->vcc_i2c, MXT_I2C_LOAD_UA);
=======
		rc = reg_set_optimum_mode_check(data->vcc_i2c, MXT_I2C_LOAD_UA);
>>>>>>> refs/remotes/origin/cm-10.0
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_i2c set_opt failed rc=%d\n", rc);
			goto error_reg_opt_i2c;
		}

		rc = regulator_enable(data->vcc_i2c);
		if (rc) {
			dev_err(&data->client->dev,
				"Regulator vcc_i2c enable failed rc=%d\n", rc);
			goto error_reg_en_vcc_i2c;
		}
	}

	msleep(130);

	return 0;

error_reg_en_vcc_i2c:
	if (data->pdata->i2c_pull_up)
<<<<<<< HEAD
		regulator_set_optimum_mode(data->vcc_i2c, 0);
=======
		reg_set_optimum_mode_check(data->vcc_i2c, 0);
>>>>>>> refs/remotes/origin/cm-10.0
error_reg_opt_i2c:
	if (data->pdata->digital_pwr_regulator)
		regulator_disable(data->vcc_dig);
error_reg_en_vcc_dig:
	if (data->pdata->digital_pwr_regulator)
<<<<<<< HEAD
		regulator_set_optimum_mode(data->vcc_dig, 0);
error_reg_opt_vcc_dig:
	regulator_disable(data->vcc_ana);
error_reg_en_vcc_ana:
	regulator_set_optimum_mode(data->vcc_ana, 0);
	return rc;

power_off:
	regulator_set_optimum_mode(data->vcc_ana, 0);
	regulator_disable(data->vcc_ana);
	if (data->pdata->digital_pwr_regulator) {
		regulator_set_optimum_mode(data->vcc_dig, 0);
		regulator_disable(data->vcc_dig);
	}
	if (data->pdata->i2c_pull_up) {
		regulator_set_optimum_mode(data->vcc_i2c, 0);
=======
		reg_set_optimum_mode_check(data->vcc_dig, 0);
error_reg_opt_vcc_dig:
	regulator_disable(data->vcc_ana);
error_reg_en_vcc_ana:
	reg_set_optimum_mode_check(data->vcc_ana, 0);
	return rc;

power_off:
	reg_set_optimum_mode_check(data->vcc_ana, 0);
	regulator_disable(data->vcc_ana);
	if (data->pdata->digital_pwr_regulator) {
		reg_set_optimum_mode_check(data->vcc_dig, 0);
		regulator_disable(data->vcc_dig);
	}
	if (data->pdata->i2c_pull_up) {
		reg_set_optimum_mode_check(data->vcc_i2c, 0);
>>>>>>> refs/remotes/origin/cm-10.0
		regulator_disable(data->vcc_i2c);
	}
	msleep(50);
	return 0;
}

static int mxt_regulator_configure(struct mxt_data *data, bool on)
{
	int rc;

	if (on == false)
		goto hw_shutdown;

	data->vcc_ana = regulator_get(&data->client->dev, "vdd_ana");
	if (IS_ERR(data->vcc_ana)) {
		rc = PTR_ERR(data->vcc_ana);
		dev_err(&data->client->dev,
			"Regulator get failed vcc_ana rc=%d\n", rc);
		return rc;
	}

	if (regulator_count_voltages(data->vcc_ana) > 0) {
		rc = regulator_set_voltage(data->vcc_ana, MXT_VTG_MIN_UV,
							MXT_VTG_MAX_UV);
		if (rc) {
			dev_err(&data->client->dev,
				"regulator set_vtg failed rc=%d\n", rc);
			goto error_set_vtg_vcc_ana;
		}
	}
	if (data->pdata->digital_pwr_regulator) {
		data->vcc_dig = regulator_get(&data->client->dev, "vdd_dig");
		if (IS_ERR(data->vcc_dig)) {
			rc = PTR_ERR(data->vcc_dig);
			dev_err(&data->client->dev,
				"Regulator get dig failed rc=%d\n", rc);
			goto error_get_vtg_vcc_dig;
		}

		if (regulator_count_voltages(data->vcc_dig) > 0) {
			rc = regulator_set_voltage(data->vcc_dig,
				MXT_VTG_DIG_MIN_UV, MXT_VTG_DIG_MAX_UV);
			if (rc) {
				dev_err(&data->client->dev,
					"regulator set_vtg failed rc=%d\n", rc);
				goto error_set_vtg_vcc_dig;
			}
		}
	}
	if (data->pdata->i2c_pull_up) {
		data->vcc_i2c = regulator_get(&data->client->dev, "vcc_i2c");
		if (IS_ERR(data->vcc_i2c)) {
			rc = PTR_ERR(data->vcc_i2c);
			dev_err(&data->client->dev,
				"Regulator get failed rc=%d\n",	rc);
			goto error_get_vtg_i2c;
		}
		if (regulator_count_voltages(data->vcc_i2c) > 0) {
			rc = regulator_set_voltage(data->vcc_i2c,
				MXT_I2C_VTG_MIN_UV, MXT_I2C_VTG_MAX_UV);
			if (rc) {
				dev_err(&data->client->dev,
					"regulator set_vtg failed rc=%d\n", rc);
				goto error_set_vtg_i2c;
			}
		}
	}

	return 0;

error_set_vtg_i2c:
	regulator_put(data->vcc_i2c);
error_get_vtg_i2c:
	if (data->pdata->digital_pwr_regulator)
		if (regulator_count_voltages(data->vcc_dig) > 0)
			regulator_set_voltage(data->vcc_dig, 0,
				MXT_VTG_DIG_MAX_UV);
error_set_vtg_vcc_dig:
	if (data->pdata->digital_pwr_regulator)
		regulator_put(data->vcc_dig);
error_get_vtg_vcc_dig:
	if (regulator_count_voltages(data->vcc_ana) > 0)
		regulator_set_voltage(data->vcc_ana, 0, MXT_VTG_MAX_UV);
error_set_vtg_vcc_ana:
	regulator_put(data->vcc_ana);
	return rc;

hw_shutdown:
	if (regulator_count_voltages(data->vcc_ana) > 0)
		regulator_set_voltage(data->vcc_ana, 0, MXT_VTG_MAX_UV);
	regulator_put(data->vcc_ana);
	if (data->pdata->digital_pwr_regulator) {
		if (regulator_count_voltages(data->vcc_dig) > 0)
			regulator_set_voltage(data->vcc_dig, 0,
						MXT_VTG_DIG_MAX_UV);
		regulator_put(data->vcc_dig);
	}
	if (data->pdata->i2c_pull_up) {
		if (regulator_count_voltages(data->vcc_i2c) > 0)
			regulator_set_voltage(data->vcc_i2c, 0,
						MXT_I2C_VTG_MAX_UV);
		regulator_put(data->vcc_i2c);
	}
	return 0;
}

#ifdef CONFIG_PM
static int mxt_regulator_lpm(struct mxt_data *data, bool on)
{

	int rc;

	if (on == false)
		goto regulator_hpm;

<<<<<<< HEAD
	rc = regulator_set_optimum_mode(data->vcc_ana, MXT_LPM_LOAD_UA);
=======
	rc = reg_set_optimum_mode_check(data->vcc_ana, MXT_LPM_LOAD_UA);
>>>>>>> refs/remotes/origin/cm-10.0
	if (rc < 0) {
		dev_err(&data->client->dev,
			"Regulator vcc_ana set_opt failed rc=%d\n", rc);
		goto fail_regulator_lpm;
	}

	if (data->pdata->digital_pwr_regulator) {
<<<<<<< HEAD
		rc = regulator_set_optimum_mode(data->vcc_dig,
=======
		rc = reg_set_optimum_mode_check(data->vcc_dig,
>>>>>>> refs/remotes/origin/cm-10.0
						MXT_LPM_LOAD_DIG_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_dig set_opt failed rc=%d\n", rc);
			goto fail_regulator_lpm;
		}
	}

	if (data->pdata->i2c_pull_up) {
<<<<<<< HEAD
		rc = regulator_set_optimum_mode(data->vcc_i2c,
=======
		rc = reg_set_optimum_mode_check(data->vcc_i2c,
>>>>>>> refs/remotes/origin/cm-10.0
						MXT_I2C_LPM_LOAD_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_i2c set_opt failed rc=%d\n", rc);
			goto fail_regulator_lpm;
		}
	}

	return 0;

regulator_hpm:

<<<<<<< HEAD
	rc = regulator_set_optimum_mode(data->vcc_ana, MXT_ACTIVE_LOAD_UA);
=======
	rc = reg_set_optimum_mode_check(data->vcc_ana, MXT_ACTIVE_LOAD_UA);
>>>>>>> refs/remotes/origin/cm-10.0
	if (rc < 0) {
		dev_err(&data->client->dev,
			"Regulator vcc_ana set_opt failed rc=%d\n", rc);
		goto fail_regulator_hpm;
	}

	if (data->pdata->digital_pwr_regulator) {
<<<<<<< HEAD
		rc = regulator_set_optimum_mode(data->vcc_dig,
=======
		rc = reg_set_optimum_mode_check(data->vcc_dig,
>>>>>>> refs/remotes/origin/cm-10.0
						 MXT_ACTIVE_LOAD_DIG_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_dig set_opt failed rc=%d\n", rc);
			goto fail_regulator_hpm;
		}
	}

	if (data->pdata->i2c_pull_up) {
<<<<<<< HEAD
		rc = regulator_set_optimum_mode(data->vcc_i2c, MXT_I2C_LOAD_UA);
=======
		rc = reg_set_optimum_mode_check(data->vcc_i2c, MXT_I2C_LOAD_UA);
>>>>>>> refs/remotes/origin/cm-10.0
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_i2c set_opt failed rc=%d\n", rc);
			goto fail_regulator_hpm;
		}
	}

	return 0;

fail_regulator_lpm:
<<<<<<< HEAD
	regulator_set_optimum_mode(data->vcc_ana, MXT_ACTIVE_LOAD_UA);
	if (data->pdata->digital_pwr_regulator)
		regulator_set_optimum_mode(data->vcc_dig,
						MXT_ACTIVE_LOAD_DIG_UA);
	if (data->pdata->i2c_pull_up)
		regulator_set_optimum_mode(data->vcc_i2c, MXT_I2C_LOAD_UA);
=======
	reg_set_optimum_mode_check(data->vcc_ana, MXT_ACTIVE_LOAD_UA);
	if (data->pdata->digital_pwr_regulator)
		reg_set_optimum_mode_check(data->vcc_dig,
					MXT_ACTIVE_LOAD_DIG_UA);
	if (data->pdata->i2c_pull_up)
		reg_set_optimum_mode_check(data->vcc_i2c, MXT_I2C_LOAD_UA);
>>>>>>> refs/remotes/origin/cm-10.0

	return rc;

fail_regulator_hpm:
<<<<<<< HEAD
	regulator_set_optimum_mode(data->vcc_ana, MXT_LPM_LOAD_UA);
	if (data->pdata->digital_pwr_regulator)
		regulator_set_optimum_mode(data->vcc_dig, MXT_LPM_LOAD_DIG_UA);
	if (data->pdata->i2c_pull_up)
		regulator_set_optimum_mode(data->vcc_i2c, MXT_I2C_LPM_LOAD_UA);
=======
	reg_set_optimum_mode_check(data->vcc_ana, MXT_LPM_LOAD_UA);
	if (data->pdata->digital_pwr_regulator)
		reg_set_optimum_mode_check(data->vcc_dig, MXT_LPM_LOAD_DIG_UA);
	if (data->pdata->i2c_pull_up)
		reg_set_optimum_mode_check(data->vcc_i2c, MXT_I2C_LPM_LOAD_UA);
>>>>>>> refs/remotes/origin/cm-10.0

	return rc;
}

static int mxt_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct mxt_data *data = i2c_get_clientdata(client);
	struct input_dev *input_dev = data->input_dev;
	int error;

	mutex_lock(&input_dev->mutex);

	if (input_dev->users) {
		error = mxt_stop(data);
		if (error < 0) {
			dev_err(dev, "mxt_stop failed in suspend\n");
			mutex_unlock(&input_dev->mutex);
			return error;
		}

	}

	mutex_unlock(&input_dev->mutex);

	/* put regulators in low power mode */
	error = mxt_regulator_lpm(data, true);
	if (error < 0) {
		dev_err(dev, "failed to enter low power mode\n");
		return error;
	}

	return 0;
}

static int mxt_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct mxt_data *data = i2c_get_clientdata(client);
	struct input_dev *input_dev = data->input_dev;
	int error;

	/* put regulators in high power mode */
	error = mxt_regulator_lpm(data, false);
	if (error < 0) {
		dev_err(dev, "failed to enter high power mode\n");
		return error;
	}

	mutex_lock(&input_dev->mutex);

	if (input_dev->users) {
		error = mxt_start(data);
		if (error < 0) {
			dev_err(dev, "mxt_start failed in resume\n");
			mutex_unlock(&input_dev->mutex);
			return error;
		}
	}

<<<<<<< HEAD
=======
	/* calibrate */
	if (data->pdata->need_calibration) {
		error = mxt_write_object(data, MXT_GEN_COMMAND_T6,
					MXT_COMMAND_CALIBRATE, 1);
		if (error < 0)
			dev_dbg(dev, "sending calibration command failed\n");
	}

>>>>>>> refs/remotes/origin/cm-10.0
	mutex_unlock(&input_dev->mutex);

	return 0;
}

#if defined(CONFIG_HAS_EARLYSUSPEND)
static void mxt_early_suspend(struct early_suspend *h)
{
	struct mxt_data *data = container_of(h, struct mxt_data, early_suspend);

	mxt_suspend(&data->client->dev);
}

static void mxt_late_resume(struct early_suspend *h)
{
	struct mxt_data *data = container_of(h, struct mxt_data, early_suspend);

	mxt_resume(&data->client->dev);
}
#endif

static const struct dev_pm_ops mxt_pm_ops = {
#ifndef CONFIG_HAS_EARLYSUSPEND
	.suspend	= mxt_suspend,
	.resume		= mxt_resume,
#endif
};
#endif

<<<<<<< HEAD
static int __devinit mxt_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	const struct mxt_platform_data *pdata = client->dev.platform_data;
	struct mxt_data *data;
	struct input_dev *input_dev;
	int error;
=======
static int mxt_debugfs_object_show(struct seq_file *m, void *v)
{
	struct mxt_data *data = m->private;
	struct mxt_object *object;
	struct device *dev = &data->client->dev;
	int i, j, k;
	int error;
	int obj_size;
	u8 val;

	for (i = 0; i < data->info.object_num; i++) {
		object = data->object_table + i;
		obj_size = object->size + 1;

		seq_printf(m, "Object[%d] (Type %d)\n", i + 1, object->type);

		for (j = 0; j < object->instances + 1; j++) {
			seq_printf(m, "[Instance %d]\n", j);

			for (k = 0; k < obj_size; k++) {
				error = mxt_read_object(data, object->type,
							j * obj_size + k, &val);
				if (error) {
					dev_err(dev,
						"Failed to read object %d "
						"instance %d at offset %d\n",
						object->type, j, k);
					return error;
				}

				seq_printf(m, "Byte %d: 0x%02x (%d)\n",
						k, val, val);
			}
		}
	}

	return 0;
}

static int mxt_debugfs_object_open(struct inode *inode, struct file *file)
{
	return single_open(file, mxt_debugfs_object_show, inode->i_private);
}

static const struct file_operations mxt_object_fops = {
	.owner		= THIS_MODULE,
	.open		= mxt_debugfs_object_open,
	.read		= seq_read,
	.release	= single_release,
};

static void __devinit mxt_debugfs_init(struct mxt_data *data)
{
	debug_base = debugfs_create_dir(MXT_DEBUGFS_DIR, NULL);
	if (IS_ERR_OR_NULL(debug_base))
		pr_err("atmel_mxt_ts: Failed to create debugfs dir\n");
	if (IS_ERR_OR_NULL(debugfs_create_file(MXT_DEBUGFS_FILE,
					       0444,
					       debug_base,
					       data,
					       &mxt_object_fops))) {
		pr_err("atmel_mxt_ts: Failed to create object file\n");
		debugfs_remove_recursive(debug_base);
	}
}

#ifdef CONFIG_OF
static int mxt_get_dt_coords(struct device *dev, char *name,
				struct mxt_platform_data *pdata)
{
	u32 coords[MXT_COORDS_ARR_SIZE];
	struct property *prop;
	struct device_node *np = dev->of_node;
	int coords_size, rc;

	prop = of_find_property(np, name, NULL);
	if (!prop)
		return -EINVAL;
	if (!prop->value)
		return -ENODATA;

	coords_size = prop->length / sizeof(u32);
	if (coords_size != MXT_COORDS_ARR_SIZE) {
		dev_err(dev, "invalid %s\n", name);
		return -EINVAL;
	}

	rc = of_property_read_u32_array(np, name, coords, coords_size);
	if (rc && (rc != -EINVAL)) {
		dev_err(dev, "Unable to read %s\n", name);
		return rc;
	}

	if (strncmp(name, "atmel,panel-coords",
			sizeof("atmel,panel-coords")) == 0) {
		pdata->panel_minx = coords[0];
		pdata->panel_miny = coords[1];
		pdata->panel_maxx = coords[2];
		pdata->panel_maxy = coords[3];
	} else if (strncmp(name, "atmel,display-coords",
			sizeof("atmel,display-coords")) == 0) {
		pdata->disp_minx = coords[0];
		pdata->disp_miny = coords[1];
		pdata->disp_maxx = coords[2];
		pdata->disp_maxy = coords[3];
	} else {
		dev_err(dev, "unsupported property %s\n", name);
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD
static int mxt_parse_config(struct device *dev, struct device_node *np,
				struct mxt_config_info *info)
{
	struct property *prop;
	u8 *temp_cfg;

	prop = of_find_property(np, "atmel,config", &info->config_length);
	if (!prop) {
		dev_err(dev, "Looking up %s property in node %s failed",
			"atmel,config", np->full_name);
		return -ENODEV;
	} else if (!info->config_length) {
		dev_err(dev, "Invalid length of configuration data\n");
		return -EINVAL;
	}

	temp_cfg = devm_kzalloc(dev,
			info->config_length * sizeof(u8), GFP_KERNEL);
	if (!temp_cfg) {
		dev_err(dev, "Unable to allocate memory to store cfg\n");
		return -ENOMEM;
	}

	memcpy(temp_cfg, prop->value, info->config_length);
	info->config = temp_cfg;

	return 0;
}

static int mxt_parse_dt(struct device *dev, struct mxt_platform_data *pdata)
{
	int rc;
	struct mxt_config_info *info;
	struct device_node *temp, *np = dev->of_node;
	struct property *prop;
	u32 temp_val;

	rc = mxt_get_dt_coords(dev, "atmel,panel-coords", pdata);
	if (rc)
		return rc;

	rc = mxt_get_dt_coords(dev, "atmel,display-coords", pdata);
	if (rc)
		return rc;

	/* regulator info */
	pdata->i2c_pull_up = of_property_read_bool(np, "atmel,i2c-pull-up");
	pdata->digital_pwr_regulator = of_property_read_bool(np,
						"atmel,dig-reg-support");
	/* reset, irq gpio info */
	pdata->reset_gpio = of_get_named_gpio_flags(np, "atmel,reset-gpio",
				0, &pdata->reset_gpio_flags);
	pdata->irq_gpio = of_get_named_gpio_flags(np, "atmel,irq-gpio",
				0, &pdata->irq_gpio_flags);

	/* keycodes for keyarray object*/
	prop = of_find_property(np, "atmel,key-codes", NULL);
	if (prop) {
		pdata->key_codes = devm_kzalloc(dev,
				sizeof(int) * MXT_KEYARRAY_MAX_KEYS,
				GFP_KERNEL);
		if (!pdata->key_codes)
			return -ENOMEM;
		if ((prop->length/sizeof(u32)) == MXT_KEYARRAY_MAX_KEYS) {
			rc = of_property_read_u32_array(np, "atmel,key-codes",
				pdata->key_codes, MXT_KEYARRAY_MAX_KEYS);
			if (rc) {
				dev_err(dev, "Unable to read key codes\n");
				return rc;
			}
		} else
			return -EINVAL;
	}

	/* need calibration during wakeup? */
	pdata->need_calibration = of_property_read_bool(np,
					"atmel,need-calibration");
	/* config array size */
	pdata->config_array_size = 0;
	temp = NULL;
	while ((temp = of_get_next_child(np, temp)))
		pdata->config_array_size++;

	if (!pdata->config_array_size)
		return 0;

	info = devm_kzalloc(dev, pdata->config_array_size *
				sizeof(struct mxt_config_info), GFP_KERNEL);
	if (!info) {
		dev_err(dev, "Unable to allocate memory\n");
		return -ENOMEM;
	}

	pdata->config_array  = info;

	for_each_child_of_node(np, temp) {
		rc = of_property_read_string(temp, "atmel,fw-name",
			&info->fw_name);
		if (rc && (rc != -EINVAL)) {
			dev_err(dev, "Unable to read fw name\n");
			return rc;
		}

		rc = of_property_read_u32(temp, "atmel,family-id", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read family id\n");
			return rc;
		} else
			info->family_id = (u8) temp_val;

		rc  = of_property_read_u32(temp, "atmel,variant-id", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read variant id\n");
			return rc;
		} else
			info->variant_id = (u8) temp_val;

		rc = of_property_read_u32(temp, "atmel,version", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read controller version\n");
			return rc;
		} else
			info->version = (u8) temp_val;

		rc = of_property_read_u32(temp, "atmel,build", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read build id\n");
			return rc;
		} else
			info->build = (u8) temp_val;

		info->bootldr_id = of_property_read_u32(temp,
					"atmel,bootldr-id", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read bootldr-id\n");
			return rc;
		} else
			info->bootldr_id = (u8) temp_val;

		rc = mxt_parse_config(dev, temp, info);
		if (rc) {
			dev_err(dev, "Unable to parse config data\n");
			return rc;
		}
		info++;
	}

	return 0;
}
#else
static int mxt_parse_dt(struct device *dev, struct mxt_platform_data *pdata)
{
	return -ENODEV;
}
#endif

static int __devinit mxt_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	struct mxt_platform_data *pdata;
	struct mxt_data *data;
	struct input_dev *input_dev;
	int error, i;

	if (client->dev.of_node) {
		pdata = devm_kzalloc(&client->dev,
			sizeof(struct mxt_platform_data), GFP_KERNEL);
		if (!pdata) {
			dev_err(&client->dev, "Failed to allocate memory\n");
			return -ENOMEM;
		}

		error = mxt_parse_dt(&client->dev, pdata);
		if (error)
			return error;
	} else
		pdata = client->dev.platform_data;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto err_free_object_table;
	info->matrix_ysize = val;

	dev_info(&client->dev,
			"Family ID: %u Variant ID: %u Major.Minor.Build: %u.%u.%02X\n",
			info->family_id, info->variant_id, info->version >> 4,
			info->version & 0xf, info->build);

	dev_info(&client->dev,
			"Matrix X Size: %u Matrix Y Size: %u Object Num: %u\n",
			info->matrix_xsize, info->matrix_ysize,
			info->object_num);

	return 0;

err_free_object_table:
	mxt_free_object_table(data);
	return error;
=======
static int compare_versions(const u8 *v1, const u8 *v2)
{
	int i;

	if (!v1 || !v2)
		return -EINVAL;

	/* The major version number stays the same across different versions for
	 * a particular controller on a target. The minor and sub-minor version
	 * numbers indicate which version is newer.
	 */
	if (v1[0] != v2[0])
		return -EINVAL;

	for (i = 1; i < MXT_CFG_VERSION_LEN; i++) {
		if (v1[i] > v2[i])
			return MXT_CFG_VERSION_LESS;	/* v2 is older */

		if (v1[i] < v2[i])
			return MXT_CFG_VERSION_GREATER;	/* v2 is newer */
	}

	return MXT_CFG_VERSION_EQUAL;	/* v1 and v2 are equal */
}

static void mxt_check_config_version(struct mxt_data *data,
			const struct mxt_config_info *cfg_info,
			bool match_major,
			const u8 **cfg_version_found,
			bool *found_cfg_major_match)
{
	const u8 *cfg_version;
	int result = -EINVAL;

	cfg_version = cfg_info->config + data->cfg_version_idx;

	if (*cfg_version_found)
		result = compare_versions(*cfg_version_found, cfg_version);

	if (match_major) {
		if (result >= MXT_CFG_VERSION_EQUAL)
			*found_cfg_major_match = true;

		if (result == MXT_CFG_VERSION_EQUAL ||
			result == MXT_CFG_VERSION_GREATER) {
			data->config_info = cfg_info;
			data->fw_name = cfg_info->fw_name;
			*cfg_version_found = cfg_version;
		}

		if (result == MXT_CFG_VERSION_GREATER)
			data->update_cfg = true;
	} else if (!*cfg_version_found || result == MXT_CFG_VERSION_GREATER) {
		data->config_info = cfg_info;
		data->fw_name = cfg_info->fw_name;
		data->update_cfg = true;
		*cfg_version_found = cfg_version;
	}
>>>>>>> refs/remotes/origin/cm-11.0
}

/* If the controller's config version has a non-zero major number, call this
 * function with match_major = true to look for the latest config present in
 * the pdata based on matching family id, variant id, f/w version, build, and
 * config major number. If the controller is programmed with wrong config data
 * previously, call this function with match_major = false to look for latest
 * config based on based on matching family id, variant id, f/w version and
 * build only.
 */
static int mxt_search_config_array(struct mxt_data *data, bool match_major)
{

	const struct mxt_platform_data *pdata = data->pdata;
	const struct mxt_config_info *cfg_info;
	const struct mxt_info *info = &data->info;
	const u8 *cfg_version_found;
	bool found_cfg_major_match = false;
	int i;

<<<<<<< HEAD
/* Firmware Version is returned as Major.Minor.Build */
static ssize_t mxt_fw_version_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	struct mxt_info *info = &data->info;
	return scnprintf(buf, PAGE_SIZE, "%u.%u.%02X\n",
			 info->version >> 4, info->version & 0xf, info->build);
}

/* Hardware Version is returned as FamilyID.VariantID */
static ssize_t mxt_hw_version_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	struct mxt_info *info = &data->info;
	return scnprintf(buf, PAGE_SIZE, "%u.%u\n",
			 info->family_id, info->variant_id);
}

static ssize_t mxt_show_instance(char *buf, int count,
				 struct mxt_object *object, int instance,
				 const u8 *val)
{
	int i;

	if (object->instances > 0)
		count += scnprintf(buf + count, PAGE_SIZE - count,
				   "Instance %u\n", instance);

	for (i = 0; i < object->size + 1; i++)
		count += scnprintf(buf + count, PAGE_SIZE - count,
				"\t[%2u]: %02x (%d)\n", i, val[i], val[i]);
	count += scnprintf(buf + count, PAGE_SIZE - count, "\n");

	return count;
}

static ssize_t mxt_object_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	struct mxt_object *object;
	int count = 0;
	int i, j;
	int error;
	u8 *obuf;

	/* Pre-allocate buffer large enough to hold max sized object. */
	obuf = kmalloc(256, GFP_KERNEL);
	if (!obuf)
		return -ENOMEM;

	error = 0;
	for (i = 0; i < data->info.object_num; i++) {
		object = data->object_table + i;

		if (!mxt_object_readable(object->type))
			continue;

		count += scnprintf(buf + count, PAGE_SIZE - count,
				"T%u:\n", object->type);

		for (j = 0; j < object->instances + 1; j++) {
			u16 size = object->size + 1;
			u16 addr = object->start_address + j * size;

			error = __mxt_read_reg(data->client, addr, size, obuf);
			if (error)
				goto done;

			count = mxt_show_instance(buf, count, object, j, obuf);
		}
	}

done:
	kfree(obuf);
	return error ?: count;
=======
	cfg_version_found = match_major ? data->cfg_version : NULL;

	for (i = 0; i < pdata->config_array_size; i++) {

		cfg_info = &pdata->config_array[i];

		if (!cfg_info->config || !cfg_info->config_length)
			continue;

		if (info->family_id == cfg_info->family_id &&
			info->variant_id == cfg_info->variant_id &&
			info->version == cfg_info->version &&
			info->build == cfg_info->build) {

			mxt_check_config_version(data, cfg_info, match_major,
				&cfg_version_found, &found_cfg_major_match);
		}
	}

	if (data->config_info || found_cfg_major_match)
		return 0;

	data->config_info = NULL;
	data->fw_name = NULL;

	return -EINVAL;
>>>>>>> refs/remotes/origin/cm-11.0
}

static int mxt_get_config(struct mxt_data *data)
{
	const struct mxt_platform_data *pdata = data->pdata;
	struct device *dev = &data->client->dev;
	struct mxt_object *object;
	int error;

	if (!pdata->config_array || !pdata->config_array_size) {
		dev_dbg(dev, "No cfg data provided by platform data\n");
		return 0;
	}

	/* Get current config version */
	object = mxt_get_object(data, MXT_SPT_USERDATA_T38);
	if (!object) {
		dev_err(dev, "Unable to obtain USERDATA object\n");
		return -EINVAL;
	}

	error = __mxt_read_reg(data->client, object->start_address,
				sizeof(data->cfg_version), data->cfg_version);
	if (error) {
		dev_err(dev, "Unable to read config version\n");
		return error;
	}
	dev_info(dev, "Current config version on the controller is %d.%d.%d\n",
			data->cfg_version[0], data->cfg_version[1],
			data->cfg_version[2]);

	/* It is possible that the config data on the controller is not
	 * versioned and the version number returns 0. In this case,
	 * find a match without the config version checking.
	 */
	error = mxt_search_config_array(data,
				data->cfg_version[0] != 0 ? true : false);
	if (error) {
		/* If a match wasn't found for a non-zero config version,
		 * it means the controller has the wrong config data. Search
		 * for a best match based on controller and firmware version,
		 * but not config version.
		 */
		if (data->cfg_version[0])
			error = mxt_search_config_array(data, false);
		if (error) {
			dev_err(dev,
				"Unable to find matching config in pdata\n");
			return error;
		}
	}

	return 0;
}

static void mxt_reset_delay(struct mxt_data *data)
{
	struct mxt_info *info = &data->info;

	switch (info->family_id) {
	case MXT224_ID:
		msleep(MXT224_RESET_TIME);
		break;
	case MXT224E_ID:
		msleep(MXT224E_RESET_TIME);
		break;
	case MXT1386_ID:
		msleep(MXT1386_RESET_TIME);
		break;
	default:
		msleep(MXT_RESET_TIME);
	}
}

static int mxt_backup_nv(struct mxt_data *data)
{
	int error;
	u8 command_register;
	int timeout_counter = 0;

	/* Backup to memory */
	mxt_write_object(data, MXT_GEN_COMMAND_T6,
			MXT_COMMAND_BACKUPNV,
			MXT_BACKUP_VALUE);
	msleep(MXT_BACKUP_TIME);

	do {
		error = mxt_read_object(data, MXT_GEN_COMMAND_T6,
					MXT_COMMAND_BACKUPNV,
					&command_register);
		if (error)
			return error;

		usleep_range(1000, 2000);

	} while ((command_register != 0) && (++timeout_counter <= 100));

	if (timeout_counter > 100) {
		dev_err(&data->client->dev, "No response after backup!\n");
		return -EIO;
	}

	/* Soft reset */
	mxt_write_object(data, MXT_GEN_COMMAND_T6, MXT_COMMAND_RESET, 1);

	mxt_reset_delay(data);

	return 0;
}

static int mxt_save_objects(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	struct mxt_object *t7_object;
	struct mxt_object *t9_object;
	struct mxt_object *t15_object;
	struct mxt_object *t42_object;
	int error;

	/* Store T7 and T9 locally, used in suspend/resume operations */
	t7_object = mxt_get_object(data, MXT_GEN_POWER_T7);
	if (!t7_object) {
		dev_err(&client->dev, "Failed to get T7 object\n");
		return -EINVAL;
	}

	data->t7_start_addr = t7_object->start_address;
	error = __mxt_read_reg(client, data->t7_start_addr,
				T7_DATA_SIZE, data->t7_data);
	if (error < 0) {
		dev_err(&client->dev,
			"Failed to save current power state\n");
		return error;
	}

	/* Store T9, T15's min and max report ids */
	t9_object = mxt_get_object(data, MXT_TOUCH_MULTI_T9);
	if (!t9_object) {
		dev_err(&client->dev, "Failed to get T9 object\n");
		return -EINVAL;
	}
	data->t9_max_reportid = t9_object->max_reportid;
	data->t9_min_reportid = t9_object->max_reportid -
					t9_object->num_report_ids + 1;

	if (data->pdata->key_codes) {
		t15_object = mxt_get_object(data, MXT_TOUCH_KEYARRAY_T15);
		if (!t15_object)
			dev_dbg(&client->dev, "T15 object is not available\n");
		else {
			data->t15_max_reportid = t15_object->max_reportid;
			data->t15_min_reportid = t15_object->max_reportid -
						t15_object->num_report_ids + 1;
		}
	}

	/* Store T42 min and max report ids */
	t42_object = mxt_get_object(data, MXT_PROCI_TOUCHSUPPRESSION_T42);
	if (!t42_object)
		dev_dbg(&client->dev, "T42 object is not available\n");
	else {
		data->t42_max_reportid = t42_object->max_reportid;
		data->t42_min_reportid = t42_object->max_reportid -
					t42_object->num_report_ids + 1;
	}

	return 0;
}

static int mxt_initialize(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	struct mxt_info *info = &data->info;
	int error;
	u8 val;
	const u8 *cfg_ver;

	error = mxt_get_info(data);
	if (error) {
		/* Try bootloader mode */
		error = mxt_switch_to_bootloader_address(data);
		if (error)
			return error;

		error = mxt_check_bootloader(client, MXT_APP_CRC_FAIL);
		if (error)
			return error;

		dev_err(&client->dev, "Application CRC failure\n");
		data->state = BOOTLOADER;

		return 0;
	}

	dev_info(&client->dev,
			"Family ID: %d Variant ID: %d Version: %d.%d "
			"Build: 0x%02X Object Num: %d\n",
			info->family_id, info->variant_id,
			info->version >> 4, info->version & 0xf,
			info->build, info->object_num);

	data->state = APPMODE;

	data->object_table = kcalloc(info->object_num,
				     sizeof(struct mxt_object),
				     GFP_KERNEL);
	if (!data->object_table) {
		dev_err(&client->dev, "Failed to allocate memory\n");
		return -ENOMEM;
	}

	/* Get object table information */
	error = mxt_get_object_table(data);
	if (error)
		goto free_object_table;

	/* Get config data from platform data */
	error = mxt_get_config(data);
	if (error)
		dev_dbg(&client->dev, "Config info not found.\n");

	/* Check register init values */
	if (data->config_info && data->config_info->config) {
		if (data->update_cfg) {
			error = mxt_check_reg_init(data);
			if (error) {
				dev_err(&client->dev,
					"Failed to check reg init value\n");
				goto free_object_table;
			}

			error = mxt_backup_nv(data);
			if (error) {
				dev_err(&client->dev, "Failed to back up NV\n");
				goto free_object_table;
			}

			cfg_ver = data->config_info->config +
							data->cfg_version_idx;
			dev_info(&client->dev,
				"Config updated from %d.%d.%d to %d.%d.%d\n",
				data->cfg_version[0], data->cfg_version[1],
				data->cfg_version[2],
				cfg_ver[0], cfg_ver[1], cfg_ver[2]);

			memcpy(data->cfg_version, cfg_ver, MXT_CFG_VERSION_LEN);
		}
	} else {
		dev_info(&client->dev,
			"No cfg data defined, skipping check reg init\n");
	}

	error = mxt_save_objects(data);
	if (error)
		goto free_object_table;

	/* Update matrix size at info struct */
	error = mxt_read_reg(client, MXT_MATRIX_X_SIZE, &val);
	if (error)
		goto free_object_table;
	info->matrix_xsize = val;

	error = mxt_read_reg(client, MXT_MATRIX_Y_SIZE, &val);
	if (error)
		goto free_object_table;
	info->matrix_ysize = val;

	dev_info(&client->dev,
			"Matrix X Size: %d Matrix Y Size: %d\n",
			info->matrix_xsize, info->matrix_ysize);

	return 0;

free_object_table:
	kfree(data->object_table);
	return error;
}

static ssize_t mxt_object_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	struct mxt_object *object;
	int count = 0;
	int i, j;
	int error;
	u8 val;

	for (i = 0; i < data->info.object_num; i++) {
		object = data->object_table + i;

		count += snprintf(buf + count, PAGE_SIZE - count,
				"Object[%d] (Type %d)\n",
				i + 1, object->type);
		if (count >= PAGE_SIZE)
			return PAGE_SIZE - 1;

		if (!mxt_object_readable(object->type)) {
			count += snprintf(buf + count, PAGE_SIZE - count,
					"\n");
			if (count >= PAGE_SIZE)
				return PAGE_SIZE - 1;
			continue;
		}

		for (j = 0; j < object->size + 1; j++) {
			error = mxt_read_object(data,
						object->type, j, &val);
			if (error)
				return error;

			count += snprintf(buf + count, PAGE_SIZE - count,
					"\t[%2d]: %02x (%d)\n", j, val, val);
			if (count >= PAGE_SIZE)
				return PAGE_SIZE - 1;
		}

		count += snprintf(buf + count, PAGE_SIZE - count, "\n");
		if (count >= PAGE_SIZE)
			return PAGE_SIZE - 1;
	}

	return count;
}

static int strtobyte(const char *data, u8 *value)
{
	char str[3];

	str[0] = data[0];
	str[1] = data[1];
	str[2] = '\0';

	return kstrtou8(str, 16, value);
}

static int mxt_load_fw(struct device *dev, const char *fn)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	struct i2c_client *client = data->client;
	const struct firmware *fw = NULL;
	unsigned int frame_size;
	unsigned int retry = 0;
	unsigned int pos = 0;
	int ret, i, max_frame_size;
	u8 *frame;

	switch (data->info.family_id) {
	case MXT224_ID:
	case MXT224E_ID:
		max_frame_size = MXT_SINGLE_FW_MAX_FRAME_SIZE;
		break;
	case MXT1386_ID:
		max_frame_size = MXT_CHIPSET_FW_MAX_FRAME_SIZE;
		break;
	default:
		return -EINVAL;
	}

	frame = kmalloc(max_frame_size, GFP_KERNEL);
	if (!frame) {
		dev_err(dev, "Unable to allocate memory for frame data\n");
		return -ENOMEM;
	}

	ret = request_firmware(&fw, fn, dev);
	if (ret < 0) {
		dev_err(dev, "Unable to open firmware %s\n", fn);
		goto free_frame;
	}

	if (data->state != BOOTLOADER) {
		/* Change to the bootloader mode */
		mxt_write_object(data, MXT_GEN_COMMAND_T6,
				MXT_COMMAND_RESET, MXT_BOOT_VALUE);
		mxt_reset_delay(data);

		ret = mxt_switch_to_bootloader_address(data);
		if (ret)
			goto release_firmware;
	}

	ret = mxt_check_bootloader(client, MXT_WAITING_BOOTLOAD_CMD);
	if (ret) {
		/* Bootloader may still be unlocked from previous update
		 * attempt */
		ret = mxt_check_bootloader(client,
			MXT_WAITING_FRAME_DATA);

		if (ret)
			goto return_to_app_mode;
	} else {
		dev_info(dev, "Unlocking bootloader\n");
		/* Unlock bootloader */
		mxt_unlock_bootloader(client);
	}

	while (pos < fw->size) {
		ret = mxt_check_bootloader(client,
						MXT_WAITING_FRAME_DATA);
		if (ret)
			goto release_firmware;

		/* Get frame length MSB */
		ret = strtobyte(fw->data + pos, frame);
		if (ret)
			goto release_firmware;

		/* Get frame length LSB */
		ret = strtobyte(fw->data + pos + 2, frame + 1);
		if (ret)
			goto release_firmware;

		frame_size = ((*frame << 8) | *(frame + 1));

		/* We should add 2 at frame size as the the firmware data is not
		 * included the CRC bytes.
		 */
		frame_size += 2;

		if (frame_size > max_frame_size) {
			dev_err(dev, "Invalid frame size - %d\n", frame_size);
			ret = -EINVAL;
			goto release_firmware;
		}

		/* Convert frame data and CRC from hex to binary */
		for (i = 2; i < frame_size; i++) {
			ret = strtobyte(fw->data + pos + i * 2, frame + i);
			if (ret)
				goto release_firmware;
		}

		/* Write one frame to device */
		mxt_fw_write(client, frame, frame_size);

		ret = mxt_check_bootloader(client,
						MXT_FRAME_CRC_PASS);
		if (ret) {
			retry++;

			/* Back off by 20ms per retry */
			msleep(retry * 20);

			if (retry > 20)
				goto release_firmware;
		} else {
			retry = 0;
			pos += frame_size * 2;
			dev_dbg(dev, "Updated %d/%zd bytes\n", pos, fw->size);
		}
	}

return_to_app_mode:
	mxt_switch_to_appmode_address(data);
release_firmware:
	release_firmware(fw);
free_frame:
	kfree(frame);

	return ret;
}

static const char *
mxt_search_fw_name(struct mxt_data *data, u8 bootldr_id)
{
	const struct mxt_platform_data *pdata = data->pdata;
	const struct mxt_config_info *cfg_info;
	const char *fw_name = NULL;
	int i;

	for (i = 0; i < pdata->config_array_size; i++) {
		cfg_info = &pdata->config_array[i];
		if (bootldr_id == cfg_info->bootldr_id && cfg_info->fw_name) {
			data->config_info = cfg_info;
			data->info.family_id = cfg_info->family_id;
			fw_name = cfg_info->fw_name;
		}
	}

	return fw_name;
}

static ssize_t mxt_update_fw_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	int error, address;
	const char *fw_name;
	u8 bootldr_id;
	u8 cfg_version[MXT_CFG_VERSION_LEN] = {0};


	/* If fw_name is set, then the existing firmware has an upgrade */
	if (!data->fw_name) {
		/*
		 * If the device boots up in the bootloader mode, check if
		 * there is a firmware to upgrade.
		 */
		if (data->state == BOOTLOADER) {
			bootldr_id = mxt_get_bootloader_id(data->client);
			if (bootldr_id <= 0) {
				dev_err(dev,
					"Unable to retrieve bootloader id\n");
				return -EINVAL;
			}
			fw_name = mxt_search_fw_name(data, bootldr_id);
			if (fw_name == NULL) {
				dev_err(dev,
				"Unable to find fw from bootloader id\n");
				return -EINVAL;
			}
		} else {
			/* In APPMODE, if the f/w name does not exist, quit */
			dev_err(dev,
			"Firmware name not specified in platform data\n");
			return -EINVAL;
		}
	} else {
		fw_name = data->fw_name;
	}

	dev_info(dev, "Upgrading the firmware file to %s\n", fw_name);

	disable_irq(data->irq);

	error = mxt_load_fw(dev, fw_name);
	if (error) {
		dev_err(dev, "The firmware update failed(%d)\n", error);
		count = error;
	} else {
		dev_info(dev, "The firmware update succeeded\n");

		/* Wait for reset */
		msleep(MXT_FWRESET_TIME);

		data->state = INIT;
		kfree(data->object_table);
		data->object_table = NULL;
		data->cfg_version_idx = 0;
		data->update_cfg = false;

		/* T38 object address might have changed, read it from
		   touch controller */
		address = mxt_get_object_address(dev, MXT_SPT_USERDATA_T38);
		if (address < 0) {
			dev_err(dev, "T38 required for touch operation\n");
			return -EINVAL;
		}

		data->t38_start_addr = address;

		error = __mxt_write_reg(data->client, data->t38_start_addr,
				sizeof(cfg_version), cfg_version);
		if (error)
			dev_err(dev,
			"Unable to zero out config version after fw upgrade\n");

		mxt_initialize(data);
	}

	if (data->state == APPMODE) {
		enable_irq(data->irq);

		error = mxt_make_highchg(data);
		if (error)
			return error;
	}

	return count;
}

static DEVICE_ATTR(object, 0444, mxt_object_show, NULL);
static DEVICE_ATTR(update_fw, 0664, NULL, mxt_update_fw_store);

static struct attribute *mxt_attrs[] = {
	&dev_attr_object.attr,
	&dev_attr_update_fw.attr,
	NULL
};

static const struct attribute_group mxt_attr_group = {
	.attrs = mxt_attrs,
};

static int mxt_start(struct mxt_data *data)
{
	int error;

	/* restore the old power state values and reenable touch */
	error = __mxt_write_reg(data->client, data->t7_start_addr,
				T7_DATA_SIZE, data->t7_data);
	if (error < 0) {
		dev_err(&data->client->dev,
			"failed to restore old power state\n");
		return error;
	}

	return 0;
}

static int mxt_stop(struct mxt_data *data)
{
	int error;
	u8 t7_data[T7_DATA_SIZE] = {0};

	error = __mxt_write_reg(data->client, data->t7_start_addr,
				T7_DATA_SIZE, t7_data);
	if (error < 0) {
		dev_err(&data->client->dev,
			"failed to configure deep sleep mode\n");
		return error;
	}

	return 0;
}

static int mxt_input_open(struct input_dev *dev)
{
	struct mxt_data *data = input_get_drvdata(dev);
	int error;

	if (data->state == APPMODE) {
		error = mxt_start(data);
		if (error < 0) {
			dev_err(&data->client->dev, "mxt_start failed in input_open\n");
			return error;
		}
	}

	return 0;
}

static void mxt_input_close(struct input_dev *dev)
{
	struct mxt_data *data = input_get_drvdata(dev);
	int error;

	if (data->state == APPMODE) {
		error = mxt_stop(data);
		if (error < 0)
			dev_err(&data->client->dev, "mxt_stop failed in input_close\n");
	}
}

static int reg_set_optimum_mode_check(struct regulator *reg, int load_uA)
{
	return (regulator_count_voltages(reg) > 0) ?
		regulator_set_optimum_mode(reg, load_uA) : 0;
}

static int mxt_power_on(struct mxt_data *data, bool on)
{
	int rc;

	if (on == false)
		goto power_off;

	rc = reg_set_optimum_mode_check(data->vcc_ana, MXT_ACTIVE_LOAD_UA);
	if (rc < 0) {
		dev_err(&data->client->dev,
			"Regulator vcc_ana set_opt failed rc=%d\n", rc);
		return rc;
	}

	rc = regulator_enable(data->vcc_ana);
	if (rc) {
		dev_err(&data->client->dev,
			"Regulator vcc_ana enable failed rc=%d\n", rc);
		goto error_reg_en_vcc_ana;
	}

	if (data->pdata->digital_pwr_regulator) {
		rc = reg_set_optimum_mode_check(data->vcc_dig,
					MXT_ACTIVE_LOAD_DIG_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_dig set_opt failed rc=%d\n",
				rc);
			goto error_reg_opt_vcc_dig;
		}

		rc = regulator_enable(data->vcc_dig);
		if (rc) {
			dev_err(&data->client->dev,
				"Regulator vcc_dig enable failed rc=%d\n", rc);
			goto error_reg_en_vcc_dig;
		}
	}

	if (data->pdata->i2c_pull_up) {
		rc = reg_set_optimum_mode_check(data->vcc_i2c, MXT_I2C_LOAD_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_i2c set_opt failed rc=%d\n", rc);
			goto error_reg_opt_i2c;
		}

		rc = regulator_enable(data->vcc_i2c);
		if (rc) {
			dev_err(&data->client->dev,
				"Regulator vcc_i2c enable failed rc=%d\n", rc);
			goto error_reg_en_vcc_i2c;
		}
	}

	msleep(130);

	return 0;

error_reg_en_vcc_i2c:
	if (data->pdata->i2c_pull_up)
		reg_set_optimum_mode_check(data->vcc_i2c, 0);
error_reg_opt_i2c:
	if (data->pdata->digital_pwr_regulator)
		regulator_disable(data->vcc_dig);
error_reg_en_vcc_dig:
	if (data->pdata->digital_pwr_regulator)
		reg_set_optimum_mode_check(data->vcc_dig, 0);
error_reg_opt_vcc_dig:
	regulator_disable(data->vcc_ana);
error_reg_en_vcc_ana:
	reg_set_optimum_mode_check(data->vcc_ana, 0);
	return rc;

power_off:
	reg_set_optimum_mode_check(data->vcc_ana, 0);
	regulator_disable(data->vcc_ana);
	if (data->pdata->digital_pwr_regulator) {
		reg_set_optimum_mode_check(data->vcc_dig, 0);
		regulator_disable(data->vcc_dig);
	}
	if (data->pdata->i2c_pull_up) {
		reg_set_optimum_mode_check(data->vcc_i2c, 0);
		regulator_disable(data->vcc_i2c);
	}
	msleep(50);
	return 0;
}

static int mxt_regulator_configure(struct mxt_data *data, bool on)
{
	int rc;

	if (on == false)
		goto hw_shutdown;

	data->vcc_ana = regulator_get(&data->client->dev, "vdd_ana");
	if (IS_ERR(data->vcc_ana)) {
		rc = PTR_ERR(data->vcc_ana);
		dev_err(&data->client->dev,
			"Regulator get failed vcc_ana rc=%d\n", rc);
		return rc;
	}

	if (regulator_count_voltages(data->vcc_ana) > 0) {
		rc = regulator_set_voltage(data->vcc_ana, MXT_VTG_MIN_UV,
							MXT_VTG_MAX_UV);
		if (rc) {
			dev_err(&data->client->dev,
				"regulator set_vtg failed rc=%d\n", rc);
			goto error_set_vtg_vcc_ana;
		}
	}
	if (data->pdata->digital_pwr_regulator) {
		data->vcc_dig = regulator_get(&data->client->dev, "vdd_dig");
		if (IS_ERR(data->vcc_dig)) {
			rc = PTR_ERR(data->vcc_dig);
			dev_err(&data->client->dev,
				"Regulator get dig failed rc=%d\n", rc);
			goto error_get_vtg_vcc_dig;
		}

		if (regulator_count_voltages(data->vcc_dig) > 0) {
			rc = regulator_set_voltage(data->vcc_dig,
				MXT_VTG_DIG_MIN_UV, MXT_VTG_DIG_MAX_UV);
			if (rc) {
				dev_err(&data->client->dev,
					"regulator set_vtg failed rc=%d\n", rc);
				goto error_set_vtg_vcc_dig;
			}
		}
	}
	if (data->pdata->i2c_pull_up) {
		data->vcc_i2c = regulator_get(&data->client->dev, "vcc_i2c");
		if (IS_ERR(data->vcc_i2c)) {
			rc = PTR_ERR(data->vcc_i2c);
			dev_err(&data->client->dev,
				"Regulator get failed rc=%d\n",	rc);
			goto error_get_vtg_i2c;
		}
		if (regulator_count_voltages(data->vcc_i2c) > 0) {
			rc = regulator_set_voltage(data->vcc_i2c,
				MXT_I2C_VTG_MIN_UV, MXT_I2C_VTG_MAX_UV);
			if (rc) {
				dev_err(&data->client->dev,
					"regulator set_vtg failed rc=%d\n", rc);
				goto error_set_vtg_i2c;
			}
		}
	}

	return 0;

error_set_vtg_i2c:
	regulator_put(data->vcc_i2c);
error_get_vtg_i2c:
	if (data->pdata->digital_pwr_regulator)
		if (regulator_count_voltages(data->vcc_dig) > 0)
			regulator_set_voltage(data->vcc_dig, 0,
				MXT_VTG_DIG_MAX_UV);
error_set_vtg_vcc_dig:
	if (data->pdata->digital_pwr_regulator)
		regulator_put(data->vcc_dig);
error_get_vtg_vcc_dig:
	if (regulator_count_voltages(data->vcc_ana) > 0)
		regulator_set_voltage(data->vcc_ana, 0, MXT_VTG_MAX_UV);
error_set_vtg_vcc_ana:
	regulator_put(data->vcc_ana);
	return rc;

hw_shutdown:
	if (regulator_count_voltages(data->vcc_ana) > 0)
		regulator_set_voltage(data->vcc_ana, 0, MXT_VTG_MAX_UV);
	regulator_put(data->vcc_ana);
	if (data->pdata->digital_pwr_regulator) {
		if (regulator_count_voltages(data->vcc_dig) > 0)
			regulator_set_voltage(data->vcc_dig, 0,
						MXT_VTG_DIG_MAX_UV);
		regulator_put(data->vcc_dig);
	}
	if (data->pdata->i2c_pull_up) {
		if (regulator_count_voltages(data->vcc_i2c) > 0)
			regulator_set_voltage(data->vcc_i2c, 0,
						MXT_I2C_VTG_MAX_UV);
		regulator_put(data->vcc_i2c);
	}
	return 0;
}

#ifdef CONFIG_PM
static int mxt_regulator_lpm(struct mxt_data *data, bool on)
{

	int rc;

	if (on == false)
		goto regulator_hpm;

	rc = reg_set_optimum_mode_check(data->vcc_ana, MXT_LPM_LOAD_UA);
	if (rc < 0) {
		dev_err(&data->client->dev,
			"Regulator vcc_ana set_opt failed rc=%d\n", rc);
		goto fail_regulator_lpm;
	}

	if (data->pdata->digital_pwr_regulator) {
		rc = reg_set_optimum_mode_check(data->vcc_dig,
						MXT_LPM_LOAD_DIG_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_dig set_opt failed rc=%d\n", rc);
			goto fail_regulator_lpm;
		}
	}

	if (data->pdata->i2c_pull_up) {
		rc = reg_set_optimum_mode_check(data->vcc_i2c,
						MXT_I2C_LPM_LOAD_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_i2c set_opt failed rc=%d\n", rc);
			goto fail_regulator_lpm;
		}
	}

	return 0;

regulator_hpm:

	rc = reg_set_optimum_mode_check(data->vcc_ana, MXT_ACTIVE_LOAD_UA);
	if (rc < 0) {
		dev_err(&data->client->dev,
			"Regulator vcc_ana set_opt failed rc=%d\n", rc);
		goto fail_regulator_hpm;
	}

	if (data->pdata->digital_pwr_regulator) {
		rc = reg_set_optimum_mode_check(data->vcc_dig,
						 MXT_ACTIVE_LOAD_DIG_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_dig set_opt failed rc=%d\n", rc);
			goto fail_regulator_hpm;
		}
	}

	if (data->pdata->i2c_pull_up) {
		rc = reg_set_optimum_mode_check(data->vcc_i2c, MXT_I2C_LOAD_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_i2c set_opt failed rc=%d\n", rc);
			goto fail_regulator_hpm;
		}
	}

	return 0;

fail_regulator_lpm:
	reg_set_optimum_mode_check(data->vcc_ana, MXT_ACTIVE_LOAD_UA);
	if (data->pdata->digital_pwr_regulator)
		reg_set_optimum_mode_check(data->vcc_dig,
					MXT_ACTIVE_LOAD_DIG_UA);
	if (data->pdata->i2c_pull_up)
		reg_set_optimum_mode_check(data->vcc_i2c, MXT_I2C_LOAD_UA);

	return rc;

fail_regulator_hpm:
	reg_set_optimum_mode_check(data->vcc_ana, MXT_LPM_LOAD_UA);
	if (data->pdata->digital_pwr_regulator)
		reg_set_optimum_mode_check(data->vcc_dig, MXT_LPM_LOAD_DIG_UA);
	if (data->pdata->i2c_pull_up)
		reg_set_optimum_mode_check(data->vcc_i2c, MXT_I2C_LPM_LOAD_UA);

	return rc;
}

static int mxt_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct mxt_data *data = i2c_get_clientdata(client);
	struct input_dev *input_dev = data->input_dev;
	int error;

	mutex_lock(&input_dev->mutex);

	if (input_dev->users) {
		error = mxt_stop(data);
		if (error < 0) {
			dev_err(dev, "mxt_stop failed in suspend\n");
			mutex_unlock(&input_dev->mutex);
			return error;
		}

	}

<<<<<<< HEAD
		mxt_free_object_table(data);
=======
	mutex_unlock(&input_dev->mutex);
>>>>>>> refs/remotes/origin/cm-11.0

	/* put regulators in low power mode */
	error = mxt_regulator_lpm(data, true);
	if (error < 0) {
		dev_err(dev, "failed to enter low power mode\n");
		return error;
	}

	return 0;
}

static int mxt_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct mxt_data *data = i2c_get_clientdata(client);
	struct input_dev *input_dev = data->input_dev;
	int error;

	/* put regulators in high power mode */
	error = mxt_regulator_lpm(data, false);
	if (error < 0) {
		dev_err(dev, "failed to enter high power mode\n");
		return error;
	}

	mutex_lock(&input_dev->mutex);

	if (input_dev->users) {
		error = mxt_start(data);
		if (error < 0) {
			dev_err(dev, "mxt_start failed in resume\n");
			mutex_unlock(&input_dev->mutex);
			return error;
		}
	}

	/* calibrate */
	if (data->pdata->need_calibration) {
		error = mxt_write_object(data, MXT_GEN_COMMAND_T6,
					MXT_COMMAND_CALIBRATE, 1);
		if (error < 0)
			dev_dbg(dev, "sending calibration command failed\n");
	}

	mutex_unlock(&input_dev->mutex);

	return 0;
}

<<<<<<< HEAD
static DEVICE_ATTR(fw_version, S_IRUGO, mxt_fw_version_show, NULL);
static DEVICE_ATTR(hw_version, S_IRUGO, mxt_hw_version_show, NULL);
static DEVICE_ATTR(object, S_IRUGO, mxt_object_show, NULL);
static DEVICE_ATTR(update_fw, S_IWUSR, NULL, mxt_update_fw_store);

static struct attribute *mxt_attrs[] = {
	&dev_attr_fw_version.attr,
	&dev_attr_hw_version.attr,
	&dev_attr_object.attr,
	&dev_attr_update_fw.attr,
	NULL
=======
#if defined(CONFIG_HAS_EARLYSUSPEND)
static void mxt_early_suspend(struct early_suspend *h)
{
	struct mxt_data *data = container_of(h, struct mxt_data, early_suspend);

	mxt_suspend(&data->client->dev);
}

static void mxt_late_resume(struct early_suspend *h)
{
	struct mxt_data *data = container_of(h, struct mxt_data, early_suspend);

	mxt_resume(&data->client->dev);
}
#endif

static const struct dev_pm_ops mxt_pm_ops = {
#ifndef CONFIG_HAS_EARLYSUSPEND
	.suspend	= mxt_suspend,
	.resume		= mxt_resume,
#endif
>>>>>>> refs/remotes/origin/cm-11.0
};
#endif

static int mxt_debugfs_object_show(struct seq_file *m, void *v)
{
	struct mxt_data *data = m->private;
	struct mxt_object *object;
	struct device *dev = &data->client->dev;
	int i, j, k;
	int error;
	int obj_size;
	u8 val;

	for (i = 0; i < data->info.object_num; i++) {
		object = data->object_table + i;
		obj_size = object->size + 1;

		seq_printf(m, "Object[%d] (Type %d)\n", i + 1, object->type);

		for (j = 0; j < object->instances + 1; j++) {
			seq_printf(m, "[Instance %d]\n", j);

			for (k = 0; k < obj_size; k++) {
				error = mxt_read_object(data, object->type,
							j * obj_size + k, &val);
				if (error) {
					dev_err(dev,
						"Failed to read object %d "
						"instance %d at offset %d\n",
						object->type, j, k);
					return error;
				}

				seq_printf(m, "Byte %d: 0x%02x (%d)\n",
						k, val, val);
			}
		}
	}

	return 0;
}

static int mxt_debugfs_object_open(struct inode *inode, struct file *file)
{
	return single_open(file, mxt_debugfs_object_show, inode->i_private);
}

static const struct file_operations mxt_object_fops = {
	.owner		= THIS_MODULE,
	.open		= mxt_debugfs_object_open,
	.read		= seq_read,
	.release	= single_release,
};

static void __devinit mxt_debugfs_init(struct mxt_data *data)
{
	debug_base = debugfs_create_dir(MXT_DEBUGFS_DIR, NULL);
	if (IS_ERR_OR_NULL(debug_base))
		pr_err("atmel_mxt_ts: Failed to create debugfs dir\n");
	if (IS_ERR_OR_NULL(debugfs_create_file(MXT_DEBUGFS_FILE,
					       0444,
					       debug_base,
					       data,
					       &mxt_object_fops))) {
		pr_err("atmel_mxt_ts: Failed to create object file\n");
		debugfs_remove_recursive(debug_base);
	}
}

#ifdef CONFIG_OF
static int mxt_get_dt_coords(struct device *dev, char *name,
				struct mxt_platform_data *pdata)
{
	u32 coords[MXT_COORDS_ARR_SIZE];
	struct property *prop;
	struct device_node *np = dev->of_node;
	int coords_size, rc;

	prop = of_find_property(np, name, NULL);
	if (!prop)
		return -EINVAL;
	if (!prop->value)
		return -ENODATA;

	coords_size = prop->length / sizeof(u32);
	if (coords_size != MXT_COORDS_ARR_SIZE) {
		dev_err(dev, "invalid %s\n", name);
		return -EINVAL;
	}

	rc = of_property_read_u32_array(np, name, coords, coords_size);
	if (rc && (rc != -EINVAL)) {
		dev_err(dev, "Unable to read %s\n", name);
		return rc;
	}

	if (strncmp(name, "atmel,panel-coords",
			sizeof("atmel,panel-coords")) == 0) {
		pdata->panel_minx = coords[0];
		pdata->panel_miny = coords[1];
		pdata->panel_maxx = coords[2];
		pdata->panel_maxy = coords[3];
	} else if (strncmp(name, "atmel,display-coords",
			sizeof("atmel,display-coords")) == 0) {
		pdata->disp_minx = coords[0];
		pdata->disp_miny = coords[1];
		pdata->disp_maxx = coords[2];
		pdata->disp_maxy = coords[3];
	} else {
		dev_err(dev, "unsupported property %s\n", name);
		return -EINVAL;
	}

	return 0;
}

static int mxt_parse_config(struct device *dev, struct device_node *np,
				struct mxt_config_info *info)
{
	struct property *prop;
	u8 *temp_cfg;

	prop = of_find_property(np, "atmel,config", &info->config_length);
	if (!prop) {
		dev_err(dev, "Looking up %s property in node %s failed",
			"atmel,config", np->full_name);
		return -ENODEV;
	} else if (!info->config_length) {
		dev_err(dev, "Invalid length of configuration data\n");
		return -EINVAL;
	}

	temp_cfg = devm_kzalloc(dev,
			info->config_length * sizeof(u8), GFP_KERNEL);
	if (!temp_cfg) {
		dev_err(dev, "Unable to allocate memory to store cfg\n");
		return -ENOMEM;
	}

	memcpy(temp_cfg, prop->value, info->config_length);
	info->config = temp_cfg;

	return 0;
}

static int mxt_parse_dt(struct device *dev, struct mxt_platform_data *pdata)
{
	int rc;
	struct mxt_config_info *info;
	struct device_node *temp, *np = dev->of_node;
	struct property *prop;
	u32 temp_val;

	rc = mxt_get_dt_coords(dev, "atmel,panel-coords", pdata);
	if (rc)
		return rc;

	rc = mxt_get_dt_coords(dev, "atmel,display-coords", pdata);
	if (rc)
		return rc;

	/* regulator info */
	pdata->i2c_pull_up = of_property_read_bool(np, "atmel,i2c-pull-up");
	pdata->digital_pwr_regulator = of_property_read_bool(np,
						"atmel,dig-reg-support");
	/* reset, irq gpio info */
	pdata->reset_gpio = of_get_named_gpio_flags(np, "atmel,reset-gpio",
				0, &pdata->reset_gpio_flags);
	pdata->irq_gpio = of_get_named_gpio_flags(np, "atmel,irq-gpio",
				0, &pdata->irq_gpio_flags);

	/* keycodes for keyarray object*/
	prop = of_find_property(np, "atmel,key-codes", NULL);
	if (prop) {
		pdata->key_codes = devm_kzalloc(dev,
				sizeof(int) * MXT_KEYARRAY_MAX_KEYS,
				GFP_KERNEL);
		if (!pdata->key_codes)
			return -ENOMEM;
		if ((prop->length/sizeof(u32)) == MXT_KEYARRAY_MAX_KEYS) {
			rc = of_property_read_u32_array(np, "atmel,key-codes",
				pdata->key_codes, MXT_KEYARRAY_MAX_KEYS);
			if (rc) {
				dev_err(dev, "Unable to read key codes\n");
				return rc;
			}
		} else
			return -EINVAL;
	}

	/* need calibration during wakeup? */
	pdata->need_calibration = of_property_read_bool(np,
					"atmel,need-calibration");
	/* config array size */
	pdata->config_array_size = 0;
	temp = NULL;
	while ((temp = of_get_next_child(np, temp)))
		pdata->config_array_size++;

	if (!pdata->config_array_size)
		return 0;

	info = devm_kzalloc(dev, pdata->config_array_size *
				sizeof(struct mxt_config_info), GFP_KERNEL);
	if (!info) {
		dev_err(dev, "Unable to allocate memory\n");
		return -ENOMEM;
	}

	pdata->config_array  = info;

	for_each_child_of_node(np, temp) {
		rc = of_property_read_string(temp, "atmel,fw-name",
			&info->fw_name);
		if (rc && (rc != -EINVAL)) {
			dev_err(dev, "Unable to read fw name\n");
			return rc;
		}

		rc = of_property_read_u32(temp, "atmel,family-id", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read family id\n");
			return rc;
		} else
			info->family_id = (u8) temp_val;

		rc  = of_property_read_u32(temp, "atmel,variant-id", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read variant id\n");
			return rc;
		} else
			info->variant_id = (u8) temp_val;

		rc = of_property_read_u32(temp, "atmel,version", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read controller version\n");
			return rc;
		} else
			info->version = (u8) temp_val;

		rc = of_property_read_u32(temp, "atmel,build", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read build id\n");
			return rc;
		} else
			info->build = (u8) temp_val;

		info->bootldr_id = of_property_read_u32(temp,
					"atmel,bootldr-id", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read bootldr-id\n");
			return rc;
		} else
			info->bootldr_id = (u8) temp_val;

		rc = mxt_parse_config(dev, temp, info);
		if (rc) {
			dev_err(dev, "Unable to parse config data\n");
			return rc;
		}
		info++;
	}

	return 0;
}
#else
static int mxt_parse_dt(struct device *dev, struct mxt_platform_data *pdata)
{
	return -ENODEV;
}
#endif

static int mxt_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	struct mxt_platform_data *pdata;
	struct mxt_data *data;
	struct input_dev *input_dev;
<<<<<<< HEAD
	int error;
	unsigned int num_mt_slots;
>>>>>>> refs/remotes/origin/master
=======
	int error, i;

	if (client->dev.of_node) {
		pdata = devm_kzalloc(&client->dev,
			sizeof(struct mxt_platform_data), GFP_KERNEL);
		if (!pdata) {
			dev_err(&client->dev, "Failed to allocate memory\n");
			return -ENOMEM;
		}

		error = mxt_parse_dt(&client->dev, pdata);
		if (error)
			return error;
	} else
		pdata = client->dev.platform_data;
>>>>>>> refs/remotes/origin/cm-11.0

	if (!pdata)
		return -EINVAL;

	data = kzalloc(sizeof(struct mxt_data), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!data || !input_dev) {
		dev_err(&client->dev, "Failed to allocate memory\n");
		error = -ENOMEM;
		goto err_free_mem;
	}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	input_dev->name = "Atmel maXTouch Touchscreen";
=======
	data->state = INIT;
	input_dev->name = "atmel_mxt_ts";
>>>>>>> refs/remotes/origin/cm-10.0
=======
	data->is_tp = pdata && pdata->is_tp;

	input_dev->name = (data->is_tp) ? "Atmel maXTouch Touchpad" :
					  "Atmel maXTouch Touchscreen";
	snprintf(data->phys, sizeof(data->phys), "i2c-%u-%04x/input0",
		 client->adapter->nr, client->addr);

	input_dev->phys = data->phys;

>>>>>>> refs/remotes/origin/master
=======
	data->state = INIT;
	input_dev->name = "atmel_mxt_ts";
>>>>>>> refs/remotes/origin/cm-11.0
	input_dev->id.bustype = BUS_I2C;
	input_dev->dev.parent = &client->dev;
	input_dev->open = mxt_input_open;
	input_dev->close = mxt_input_close;

	data->client = client;
	data->input_dev = input_dev;
	data->pdata = pdata;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	data->irq = client->irq;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	data->irq = client->irq;

	mxt_calc_resolution(data);
=======
>>>>>>> refs/remotes/origin/cm-11.0

	error = mxt_initialize(data);
	if (error)
		goto err_free_mem;
>>>>>>> refs/remotes/origin/master

	__set_bit(EV_ABS, input_dev->evbit);
	__set_bit(EV_KEY, input_dev->evbit);
	__set_bit(BTN_TOUCH, input_dev->keybit);
<<<<<<< HEAD
<<<<<<< HEAD
	__set_bit(INPUT_PROP_DIRECT, input_dev->propbit);

	/* For single touch */
	input_set_abs_params(input_dev, ABS_X,
<<<<<<< HEAD
			     0, data->pdata->x_size, 0, 0);
	input_set_abs_params(input_dev, ABS_Y,
			     0, data->pdata->y_size, 0, 0);
=======
			pdata->disp_minx, pdata->disp_maxx, 0, 0);
	input_set_abs_params(input_dev, ABS_Y,
			pdata->disp_miny, pdata->disp_maxy, 0, 0);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	if (data->is_tp) {
		int i;
		__set_bit(INPUT_PROP_POINTER, input_dev->propbit);
		__set_bit(INPUT_PROP_BUTTONPAD, input_dev->propbit);

		for (i = 0; i < MXT_NUM_GPIO; i++)
			if (pdata->key_map[i] != KEY_RESERVED)
				__set_bit(pdata->key_map[i], input_dev->keybit);

		__set_bit(BTN_TOOL_FINGER, input_dev->keybit);
		__set_bit(BTN_TOOL_DOUBLETAP, input_dev->keybit);
		__set_bit(BTN_TOOL_TRIPLETAP, input_dev->keybit);
		__set_bit(BTN_TOOL_QUADTAP, input_dev->keybit);
		__set_bit(BTN_TOOL_QUINTTAP, input_dev->keybit);

		input_abs_set_res(input_dev, ABS_X, MXT_PIXELS_PER_MM);
		input_abs_set_res(input_dev, ABS_Y, MXT_PIXELS_PER_MM);
		input_abs_set_res(input_dev, ABS_MT_POSITION_X,
				  MXT_PIXELS_PER_MM);
		input_abs_set_res(input_dev, ABS_MT_POSITION_Y,
				  MXT_PIXELS_PER_MM);
	}
=======
	__set_bit(INPUT_PROP_DIRECT, input_dev->propbit);
>>>>>>> refs/remotes/origin/cm-11.0

	/* For single touch */
	input_set_abs_params(input_dev, ABS_X,
			pdata->disp_minx, pdata->disp_maxx, 0, 0);
	input_set_abs_params(input_dev, ABS_Y,
<<<<<<< HEAD
			     0, data->max_y, 0, 0);
>>>>>>> refs/remotes/origin/master
=======
			pdata->disp_miny, pdata->disp_maxy, 0, 0);
>>>>>>> refs/remotes/origin/cm-11.0
	input_set_abs_params(input_dev, ABS_PRESSURE,
			     0, 255, 0, 0);

	/* For multi touch */
<<<<<<< HEAD
<<<<<<< HEAD
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR,
			     0, MXT_MAX_AREA, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_X,
			     0, data->pdata->x_size, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y,
			     0, data->pdata->y_size, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_PRESSURE,
			     0, 255, 0, 0);

=======
	input_mt_init_slots(input_dev, MXT_MAX_FINGER);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR,
			     0, MXT_MAX_AREA, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_X,
			pdata->disp_minx, pdata->disp_maxx, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y,
			pdata->disp_miny, pdata->disp_maxy, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_PRESSURE,
			     0, 255, 0, 0);

	/* set key array supported keys */
	if (pdata->key_codes) {
		for (i = 0; i < MXT_KEYARRAY_MAX_KEYS; i++) {
			if (pdata->key_codes[i])
				input_set_capability(input_dev, EV_KEY,
							pdata->key_codes[i]);
		}
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	input_set_drvdata(input_dev, data);
	i2c_set_clientdata(client, data);

	if (pdata->init_hw)
		error = pdata->init_hw(true);
	else
		error = mxt_regulator_configure(data, true);
	if (error) {
		dev_err(&client->dev, "Failed to intialize hardware\n");
		goto err_free_mem;
	}

	if (pdata->power_on)
		error = pdata->power_on(true);
	else
		error = mxt_power_on(data, true);
	if (error) {
		dev_err(&client->dev, "Failed to power on hardware\n");
		goto err_regulator_on;
	}

	if (gpio_is_valid(pdata->irq_gpio)) {
		/* configure touchscreen irq gpio */
<<<<<<< HEAD
<<<<<<< HEAD
		error = gpio_request(pdata->irq_gpio,
							"mxt_irq_gpio");
		if (error) {
			pr_err("%s: unable to request gpio [%d]\n", __func__,
=======
		error = gpio_request(pdata->irq_gpio, "mxt_irq_gpio");
		if (error) {
			dev_err(&client->dev, "unable to request gpio [%d]\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
		error = gpio_request(pdata->irq_gpio, "mxt_irq_gpio");
		if (error) {
			dev_err(&client->dev, "unable to request gpio [%d]\n",
>>>>>>> refs/remotes/origin/cm-11.0
						pdata->irq_gpio);
			goto err_power_on;
		}
		error = gpio_direction_input(pdata->irq_gpio);
		if (error) {
<<<<<<< HEAD
<<<<<<< HEAD
			pr_err("%s: unable to set_direction for gpio [%d]\n",
					__func__, pdata->irq_gpio);
			goto err_irq_gpio_req;
		}
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
			dev_err(&client->dev,
				"unable to set direction for gpio [%d]\n",
				pdata->irq_gpio);
			goto err_irq_gpio_req;
		}
		data->irq = client->irq = gpio_to_irq(pdata->irq_gpio);
	} else {
		dev_err(&client->dev, "irq gpio not provided\n");
		goto err_power_on;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}

	if (gpio_is_valid(pdata->reset_gpio)) {
		/* configure touchscreen reset out gpio */
<<<<<<< HEAD
<<<<<<< HEAD
		error = gpio_request(pdata->reset_gpio,
						"mxt_reset_gpio");
		if (error) {
			pr_err("%s: unable to request reset gpio %d\n",
				__func__, pdata->reset_gpio);
			goto err_irq_gpio_req;
		}

		error = gpio_direction_output(
					pdata->reset_gpio, 1);
		if (error) {
			pr_err("%s: unable to set direction for gpio %d\n",
				__func__, pdata->reset_gpio);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
		error = gpio_request(pdata->reset_gpio, "mxt_reset_gpio");
		if (error) {
			dev_err(&client->dev, "unable to request gpio [%d]\n",
						pdata->reset_gpio);
			goto err_irq_gpio_req;
		}

		error = gpio_direction_output(pdata->reset_gpio, 1);
		if (error) {
			dev_err(&client->dev,
				"unable to set direction for gpio [%d]\n",
				pdata->reset_gpio);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
			goto err_reset_gpio_req;
		}
	}

	mxt_reset_delay(data);
<<<<<<< HEAD
<<<<<<< HEAD

	error = mxt_initialize(data);
	if (error)
		goto err_reset_gpio_req;

=======
	error = mxt_initialize(data);
	if (error)
		goto err_reset_gpio_req;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	error = mxt_initialize(data);
	if (error)
		goto err_reset_gpio_req;
>>>>>>> refs/remotes/origin/cm-11.0
	error = request_threaded_irq(client->irq, NULL, mxt_interrupt,
			pdata->irqflags, client->dev.driver->name, data);
=======
	num_mt_slots = data->T9_reportid_max - data->T9_reportid_min + 1;
	error = input_mt_init_slots(input_dev, num_mt_slots, 0);
	if (error)
		goto err_free_object;
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR,
			     0, MXT_MAX_AREA, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_X,
			     0, data->max_x, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y,
			     0, data->max_y, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_PRESSURE,
			     0, 255, 0, 0);

	input_set_drvdata(input_dev, data);
	i2c_set_clientdata(client, data);

	error = request_threaded_irq(client->irq, NULL, mxt_interrupt,
				     pdata->irqflags | IRQF_ONESHOT,
				     client->name, data);
>>>>>>> refs/remotes/origin/master
	if (error) {
		dev_err(&client->dev, "Failed to register interrupt\n");
		goto err_free_object;
	}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	error = mxt_make_highchg(data);
	if (error)
		goto err_free_irq;
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (data->state == APPMODE) {
		error = mxt_make_highchg(data);
		if (error) {
			dev_err(&client->dev, "Failed to make high CHG\n");
			goto err_free_irq;
		}
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	error = mxt_make_highchg(data);
	if (error)
		goto err_free_irq;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	error = input_register_device(input_dev);
	if (error)
		goto err_free_irq;

	error = sysfs_create_group(&client->dev.kobj, &mxt_attr_group);
	if (error)
		goto err_unregister_device;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#if defined(CONFIG_HAS_EARLYSUSPEND)
	data->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN +
						MXT_SUSPEND_LEVEL;
	data->early_suspend.suspend = mxt_early_suspend;
	data->early_suspend.resume = mxt_late_resume;
	register_early_suspend(&data->early_suspend);
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
	mxt_debugfs_init(data);

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	mxt_debugfs_init(data);

>>>>>>> refs/remotes/origin/cm-11.0
	return 0;

err_unregister_device:
	input_unregister_device(input_dev);
	input_dev = NULL;
err_free_irq:
	free_irq(client->irq, data);
err_free_object:
	kfree(data->object_table);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
err_reset_gpio_req:
	if (gpio_is_valid(pdata->reset_gpio))
		gpio_free(pdata->reset_gpio);
err_irq_gpio_req:
	if (gpio_is_valid(pdata->irq_gpio))
		gpio_free(pdata->irq_gpio);
err_power_on:
	if (pdata->power_on)
		pdata->power_on(false);
	else
		mxt_power_on(data, false);
err_regulator_on:
	if (pdata->init_hw)
		pdata->init_hw(false);
	else
		mxt_regulator_configure(data, false);
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
err_free_mem:
	input_free_device(input_dev);
	kfree(data);
	return error;
}

<<<<<<< HEAD
static int __devexit mxt_remove(struct i2c_client *client)
=======
static int mxt_remove(struct i2c_client *client)
>>>>>>> refs/remotes/origin/master
{
	struct mxt_data *data = i2c_get_clientdata(client);

	sysfs_remove_group(&client->dev.kobj, &mxt_attr_group);
	free_irq(data->irq, data);
	input_unregister_device(data->input_dev);
<<<<<<< HEAD
<<<<<<< HEAD
#if defined(CONFIG_HAS_EARLYSUSPEND)
	unregister_early_suspend(&data->early_suspend);
#endif

	if (data->pdata->power_on)
		data->pdata->power_on(false);
	else
		mxt_power_on(data, false);

	if (data->pdata->init_hw)
		data->pdata->init_hw(false);
	else
		mxt_regulator_configure(data, false);

<<<<<<< HEAD
	kfree(data->object_table);
	kfree(data);

=======
	if (gpio_is_valid(data->pdata->reset_gpio))
		gpio_free(data->pdata->reset_gpio);

	if (gpio_is_valid(data->pdata->irq_gpio))
		gpio_free(data->pdata->irq_gpio);

	kfree(data->object_table);
	kfree(data);

	debugfs_remove_recursive(debug_base);

>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}
=======
	kfree(data->object_table);
	kfree(data);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int mxt_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct mxt_data *data = i2c_get_clientdata(client);
	struct input_dev *input_dev = data->input_dev;

	mutex_lock(&input_dev->mutex);

	if (input_dev->users)
		mxt_stop(data);

	mutex_unlock(&input_dev->mutex);

	return 0;
}
=======
#if defined(CONFIG_HAS_EARLYSUSPEND)
	unregister_early_suspend(&data->early_suspend);
#endif
>>>>>>> refs/remotes/origin/cm-11.0

	if (data->pdata->power_on)
		data->pdata->power_on(false);
	else
		mxt_power_on(data, false);

	if (data->pdata->init_hw)
		data->pdata->init_hw(false);
	else
		mxt_regulator_configure(data, false);

	if (gpio_is_valid(data->pdata->reset_gpio))
		gpio_free(data->pdata->reset_gpio);

	if (gpio_is_valid(data->pdata->irq_gpio))
		gpio_free(data->pdata->irq_gpio);

	kfree(data->object_table);
	kfree(data);

	debugfs_remove_recursive(debug_base);

	return 0;
}
<<<<<<< HEAD
#endif

static SIMPLE_DEV_PM_OPS(mxt_pm_ops, mxt_suspend, mxt_resume);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

static const struct i2c_device_id mxt_id[] = {
	{ "qt602240_ts", 0 },
	{ "atmel_mxt_ts", 0 },
<<<<<<< HEAD
=======
	{ "atmel_mxt_tp", 0 },
>>>>>>> refs/remotes/origin/master
	{ "mXT224", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, mxt_id);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#ifdef OF_CONFIG
static struct of_device_id mxt_match_table[] = {
	{ .compatible = "atmel,mxt-ts",},
	{ },
};
#else
#define mxt_match_table NULL
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

static struct i2c_driver mxt_driver = {
	.driver = {
		.name	= "atmel_mxt_ts",
		.owner	= THIS_MODULE,
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
		.of_match_table = mxt_match_table,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.of_match_table = mxt_match_table,
>>>>>>> refs/remotes/origin/cm-11.0
#ifdef CONFIG_PM
		.pm	= &mxt_pm_ops,
#endif
	},
	.probe		= mxt_probe,
	.remove		= __devexit_p(mxt_remove),
	.id_table	= mxt_id,
};

<<<<<<< HEAD
static int __init mxt_init(void)
{
	return i2c_add_driver(&mxt_driver);
}

static void __exit mxt_exit(void)
{
	i2c_del_driver(&mxt_driver);
}

module_init(mxt_init);
module_exit(mxt_exit);
=======
module_i2c_driver(mxt_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.pm	= &mxt_pm_ops,
	},
	.probe		= mxt_probe,
	.remove		= mxt_remove,
	.id_table	= mxt_id,
};

module_i2c_driver(mxt_driver);
>>>>>>> refs/remotes/origin/master

/* Module information */
MODULE_AUTHOR("Joonyoung Shim <jy0922.shim@samsung.com>");
MODULE_DESCRIPTION("Atmel maXTouch Touchscreen driver");
MODULE_LICENSE("GPL");
