/* Copyright (c) 2009, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <media/msm_camera.h>
#include <mach/gpio.h>

#include "s5k4ecgx.h"
#include "s5k4ecgx_regs.h"
#include <mach/camera.h>
#include <mach/vreg.h>
#include <linux/io.h>

#define SENSOR_DEBUG 0

#undef CONFIG_LOAD_FILE
/*#define CONFIG_LOAD_FILE*/

#ifndef CONFIG_LOAD_FILE
#define S5K4ECGX_USE_BURSTMODE
/*#define WORKAROUND_FOR_LOW_SPEED_I2C*/
#endif

#ifdef S5K4ECGX_USE_BURSTMODE
#define S5K4ECGX_WRITE_LIST(A) \
	{\
		if (s5k4ecgx_status.id == 0x0011)\
			s5k4ecgx_sensor_burst_write\
			(A##_EVT1, (sizeof(A##_EVT1) /\
			sizeof(A##_EVT1[0])), #A"_EVT1");\
		else\
			s5k4ecgx_sensor_burst_write(A,\
			(sizeof(A) / sizeof(A[0])), #A);\
	}
#else
#define S5K4ECGX_WRITE_LIST(A) \
	{\
		if (s5k4ecgx_status.id == 0x0011)\
			s5k4ecgx_sensor_write_list\
			(A##_EVT1, (sizeof(A##_EVT1) /\
			sizeof(A##_EVT1[0])), #A"_EVT1");\
		else\
			s5k4ecgx_sensor_write_list(A,\
			(sizeof(A) / sizeof(A[0])), #A);\
	}
#endif

#define PREVIEW 1
#define SNAPSHOT 2

#define FACTORY_TEST 1

/* yjh_flash*/
#define CAM_FLASH_ENSET 84
#if (CONFIG_MACH_TREBON_HWREV == 0x0) || (CONFIG_MACH_GEIM_HWREV == 0x0)
#define CAM_FLASH_FLEN 58
#else
#define CAM_FLASH_FLEN 92
#endif

#define FULL_FLASH 20
#define PRE_FLASH 7
#define MOVIE_FLASH 7
#define MACRO_FLASH 14
#define PRE_FLASH_OFF -1
#define FLASH_OFF 0

#define CHECK_EFFECT 0x00000001
#define CHECK_BRIGHTNESS 0x00000002
#define CHECK_CONTRAST 0x00000004
#define CHECK_SATURATION 0x00000008
#define CHECK_SHARPNESS 0x00000010
#define CHECK_WB 0x00000020
#define CHECK_ISO 0x00000040
#define CHECK_AE 0x00000080
#define CHECK_SCENE 0x00000100
#define CHECK_AFMODE 0x00000200
#define CHECK_DTP 0x00000400
#define CHECK_SNAPSHOT_SIZE 0x00000800
#define CHECK_PREVIEW_SIZE 0x00001000
#define CHECK_ZOOM 0x00002000
#define CHECK_JPEGQUALITY 0x00004000
#define CHECK_AUTOCONTRAST 0x00008000
#define CHECK_FPS 0x00010000
#define CHECK_AE_AWB_LOCK 0x00020000

struct s5k4ecgx_status_struct {
	char camera_initailized;	/* 1 is not init a sensor */
	u32 need_configuration;
	char camera_mode;
	char effect;
	char brightness;
	char contrast;
	char saturation;
	char sharpness;
	char whiteBalance;
	char iso;
	char auto_exposure;
	char scene;
	char fps;
	char ae_lock;
	char awb_lock;
	char afmode;
	char afcanceled;
	char dtp;
	char snapshot_size;
	char preview_size;
	char zoom;
	char lowcap_on;
	char nightcap_on;
	char power_on;
	char camera_status;
	int flash_status;	/* yjh_flash */
	char flash_mode;
	int jpeg_quality;
	int auto_contrast;
	int current_lux;
	unsigned short id;
};

static struct s5k4ecgx_status_struct s5k4ecgx_status;

bool macro_mode;
bool torch_mode_on;
bool isPreviewReturnWrite;
static unsigned int i2c_retry;
static int flashOnFromApps;

struct s5k4ecgx_work {
	struct work_struct work;
};

static struct s5k4ecgx_work *s5k4ecgx_sensorw;
static struct i2c_client *s5k4ecgx_client;

struct s5k4ecgx_ctrl {
	const struct msm_camera_sensor_info *sensordata;
};

static struct s5k4ecgx_ctrl *s5k4ecgx_ctrl;

static DECLARE_WAIT_QUEUE_HEAD(s5k4ecgx_wait_queue);
DECLARE_MUTEX(s5k4ecgx_sem);

/*=============================================================
	EXTERNAL DECLARATIONS
==============================================================*/
/*struct s5k4ecgx_reg s5k4ecgx_regs;*/
static int cpufreq_direct_set_policy(unsigned int cpu, const char *buf);
#ifdef WORKAROUND_FOR_LOW_SPEED_I2C
static void pcam_msm_i2c_pwr_mgmt(struct i2c_adapter *adap, int on);
#endif
int *get_i2c_clock_addr(struct i2c_adapter *adap);

/*=============================================================*/

static int cam_hw_init(void);
static void cam_pw(int status);
static int s5k4ecgx_sensor_init_probe(void);

#ifdef CONFIG_LOAD_FILE
static int s5k4ecgx_regs_table_write(char *name);
#endif

static int s5k4ecgx_sensor_read(unsigned short subaddr, unsigned short *data)
{
	int ret;
	unsigned char buf[2];
	struct i2c_msg msg = { s5k4ecgx_client->addr, 0, 2, buf };

	buf[0] = (subaddr >> 8);
	buf[1] = (subaddr & 0xFF);

	if (!s5k4ecgx_status.power_on)
		return -EIO;

	ret = i2c_transfer(s5k4ecgx_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
	if (ret == -EIO)
		goto error;

	msg.flags = I2C_M_RD;

	ret = i2c_transfer(s5k4ecgx_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
	if (ret == -EIO)
		goto error;

	*data = ((buf[0] << 8) | buf[1]);
	/* [Arun c]Data should be written in Little Endian in parallel mode;
	   So there is no need for byte swapping here */
	/*data = *(unsigned long *)(&buf); */
error:
	return ret;
}

static int s5k4ecgx_sensor_write(unsigned short subaddr, unsigned short val)
{
	unsigned char buf[4];
	struct i2c_msg msg = { s5k4ecgx_client->addr, 0, 4, buf };

	/*CAMDRV_DEBUG("[PGH] on write func s5k4ecgx_client->addr : %x\n",
	   s5k4ecgx_client->addr);
	   CAMDRV_DEBUG("[PGH] on write func  s5k4ecgx_client->adapter->nr :
	   %d\n", s5k4ecgx_client->adapter->nr); */

	buf[0] = (subaddr >> 8);
	buf[1] = (subaddr & 0xFF);
	buf[2] = (val >> 8);
	buf[3] = (val & 0xFF);

	return i2c_transfer(s5k4ecgx_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
}

static int s5k4ecgx_sensor_write_list(const unsigned int *list, int size,
				      char *name)
{
	int ret = 0, i;
	unsigned short subaddr;
	unsigned short value;

	if (!s5k4ecgx_status.power_on)
		return -EIO;

#ifdef CONFIG_LOAD_FILE
	ret = s5k4ecgx_regs_table_write(name);
#else
	("s5k4ecgx_sensor_write_list : %s start\n", name);

	for (i = 0; i < size; i++) {
		/*CAMDRV_DEBUG("[PGH] %x
		   %x\n", list[i].subaddr, list[i].value); */
		subaddr = (list[i] >> 16);	/*address */
		value = (list[i] & 0xFFFF);	/*value */
		if (subaddr == 0xffff) {
			CAMDRV_DEBUG("SETFILE DELAY : %dms\n", value);
			msleep(value);
		} else {
			if (s5k4ecgx_sensor_write(subaddr, value) < 0) {
				pr_info("[S5K4ECGX]sensor_write_list fail\n");
				return -EIO;
			}
		}
	}
	CAMDRV_DEBUG("s5k4ecgx_sensor_write_list : %s end\n", name);
#endif
	return ret;
}

#ifdef S5K4ECGX_USE_BURSTMODE
#define BURST_MODE_BUFFER_MAX_SIZE 2700
unsigned char s5k4ecgx_buf_for_burstmode[BURST_MODE_BUFFER_MAX_SIZE];
static int s5k4ecgx_sensor_burst_write(const unsigned int *list, int size,
				       char *name)
{
	int i = 0;
	int idx = 0;
	int err = -EINVAL;
	int retry = 0;
	unsigned short subaddr = 0, next_subaddr = 0;
	unsigned short value = 0;
	struct i2c_msg msg = { s5k4ecgx_client->addr,
		0, 0, s5k4ecgx_buf_for_burstmode
	};

	CAMDRV_DEBUG("s5k4ecgx_sensor_burst_write : %s\n", name);

I2C_RETRY:
	idx = 0;
	for (i = 0; i < size; i++) {
		if (idx > (BURST_MODE_BUFFER_MAX_SIZE - 10)) {
			/*pr_info("[S5K4ECGX]s5k4ecgx_buf_for_burstmode
			    overflow will occur!!!\n");*/
			return err;
		}

		/*address */
		subaddr = (list[i] >> 16);
		if (subaddr == 0x0F12)	/*address */
			next_subaddr = list[i + 1] >> 16;
		value = (list[i] & 0xFFFF);	/*value */

		switch (subaddr) {
		case 0x0F12:
			/* make and fill buffer for burst mode write */
			if (idx == 0) {
				s5k4ecgx_buf_for_burstmode[idx++] = 0x0F;
				s5k4ecgx_buf_for_burstmode[idx++] = 0x12;
			}
			s5k4ecgx_buf_for_burstmode[idx++] = value >> 8;
			s5k4ecgx_buf_for_burstmode[idx++] = value & 0xFF;
			/*write in burstmode */
			if (next_subaddr != 0x0F12) {
				msg.len = idx;
				err =
				    i2c_transfer(s5k4ecgx_client->adapter, &msg,
						 1) == 1 ? 0 : -EIO;
				/*pr_info("s5k4ecgx_sensor_burst_write,
				   idx = %d\n",idx); */
				idx = 0;
			}
			break;
		case 0xFFFF:
			break;
		default:
			/* Set Address */
			idx = 0;
			err = s5k4ecgx_sensor_write(subaddr, value);
			break;
		}
	}
	if (unlikely(err < 0)) {
		pr_info("[S5K4ECGX]%s: register set failed. try again.\n",
			__func__);
		i2c_retry++;
		if ((retry++) < 10)
			goto I2C_RETRY;
		return err;
	}
	/*CAMDRV_DEBUG("s5k4ecgx_sensor_burst_write end!\n"); */
	return 0;
}
#endif /*S5K4ECGX_USE_BURSTMODE */

void s5k4ecgx_set_effect(char value)
{
	switch (value) {
	case EXT_CFG_EFFECT_NORMAL:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Effect_Normal);
		break;
	case EXT_CFG_EFFECT_NEGATIVE:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Effect_Negative);
		break;
	case EXT_CFG_EFFECT_MONO:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Effect_Black_White);
		break;
	case EXT_CFG_EFFECT_SEPIA:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Effect_Sepia);
		break;
	default:
		pr_info("[S5K4ECGX]Unexpected Effect mode : %d\n", value);
		break;
	}
}

void s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(int bit, int set)
{
	int REG_TC_DBG_AutoAlgEnBits = 0;

	/* Read 04E6 */
	s5k4ecgx_sensor_write(0x002C, 0x7000);
	s5k4ecgx_sensor_write(0x002E, 0x04E6);
	s5k4ecgx_sensor_read(0x0F12,
			     (unsigned short *)&REG_TC_DBG_AutoAlgEnBits);

	if (bit == 3 && set == true) {
		if (REG_TC_DBG_AutoAlgEnBits & 0x8 == 1)
			return;
		if (s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
			msleep(250);
		else
			msleep(100);
		REG_TC_DBG_AutoAlgEnBits = REG_TC_DBG_AutoAlgEnBits | 0x8;
		s5k4ecgx_sensor_write(0x0028, 0x7000);
		s5k4ecgx_sensor_write(0x002A, 0x04E6);
		s5k4ecgx_sensor_write(0x0F12, REG_TC_DBG_AutoAlgEnBits);
	} else if (bit == 3 && set == false) {
		if (REG_TC_DBG_AutoAlgEnBits & 0x8 == 0)
			return;
		if (s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
			msleep(250);
		else
			msleep(100);
		REG_TC_DBG_AutoAlgEnBits = REG_TC_DBG_AutoAlgEnBits & 0xFFF7;
		s5k4ecgx_sensor_write(0x0028, 0x7000);
		s5k4ecgx_sensor_write(0x002A, 0x04E6);
		s5k4ecgx_sensor_write(0x0F12, REG_TC_DBG_AutoAlgEnBits);
	} else if (bit == 5 && set == true) {
		if (REG_TC_DBG_AutoAlgEnBits & 0x20 == 1)
			return;
		if (s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
			msleep(250);
		else
			msleep(100);
		REG_TC_DBG_AutoAlgEnBits = REG_TC_DBG_AutoAlgEnBits | 0x20;
		s5k4ecgx_sensor_write(0x0028, 0x7000);
		s5k4ecgx_sensor_write(0x002A, 0x04E6);
		s5k4ecgx_sensor_write(0x0F12, REG_TC_DBG_AutoAlgEnBits);
	} else if (bit == 5 && set == false) {
		if (REG_TC_DBG_AutoAlgEnBits & 0x20 == 0)
			return;
		if (s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
			msleep(250);
		else
			msleep(100);
		REG_TC_DBG_AutoAlgEnBits = REG_TC_DBG_AutoAlgEnBits & 0xFFDF;
		s5k4ecgx_sensor_write(0x0028, 0x7000);
		s5k4ecgx_sensor_write(0x002A, 0x04E6);
		s5k4ecgx_sensor_write(0x0F12, REG_TC_DBG_AutoAlgEnBits);
	}

	return;
}

void s5k4ecgx_set_whitebalance(char value)
{
	switch (value) {
	case EXT_CFG_WB_AUTO:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(3, 1);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_WB_Auto);
		break;
	case EXT_CFG_WB_DAYLIGHT:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(3, 0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_WB_Sunny);
		break;
	case EXT_CFG_WB_CLOUDY:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(3, 0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_WB_Cloudy);
		break;
	case EXT_CFG_WB_FLUORESCENT:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(3, 0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_WB_Fluorescent);
		break;
	case EXT_CFG_WB_INCANDESCENT:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(3, 0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_WB_Tungsten);
		break;
	default:
		pr_info("[S5K4ECGX]Unexpected WB mode : %d\n", value);
		break;
	}
}

void s5k4ecgx_set_brightness(char value)
{
	switch (value) {
	case EXT_CFG_BR_STEP_P_4:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_EV_Plus_4);
		break;
	case EXT_CFG_BR_STEP_P_3:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_EV_Plus_3);
		break;
	case EXT_CFG_BR_STEP_P_2:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_EV_Plus_2);
		break;
	case EXT_CFG_BR_STEP_P_1:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_EV_Plus_1);
		break;
	case EXT_CFG_BR_STEP_0:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_EV_Default);
		break;
	case EXT_CFG_BR_STEP_M_1:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_EV_Minus_1);
		break;
	case EXT_CFG_BR_STEP_M_2:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_EV_Minus_2);
		break;
	case EXT_CFG_BR_STEP_M_3:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_EV_Minus_3);
		break;
	case EXT_CFG_BR_STEP_M_4:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_EV_Minus_4);
		break;
	default:
		pr_info("[S5K4ECGX]Unexpected BR mode : %d\n", value);
		break;
	}
}

void s5k4ecgx_set_iso(char value)
{
	switch (value) {
	case EXT_CFG_ISO_AUTO:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(5, 1);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_ISO_Auto);
		break;
	case EXT_CFG_ISO_50:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(5, 0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_ISO_50);
		break;
	case EXT_CFG_ISO_100:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(5, 0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_ISO_100);
		break;
	case EXT_CFG_ISO_200:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(5, 0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_ISO_200);
		break;
	case EXT_CFG_ISO_400:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(5, 0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_ISO_400);
		break;
	default:
		pr_info("[S5K4ECGX]Unexpected ISO mode : %d\n", value);
		break;
	}
}

void s5k4ecgx_set_metering(char value)
{
	switch (value) {
	case EXT_CFG_METERING_NORMAL:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Metering_Matrix);
		break;
	case EXT_CFG_METERING_SPOT:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Metering_Spot);
		break;
	case EXT_CFG_METERING_CENTER:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Metering_Center);
		break;
	default:
		pr_info("[S5K4ECGX]Unexpected METERING mode : %d\n", value);
		break;
	}
}

void s5k4ecgx_set_contrast(char value)
{
	switch (value) {
	case EXT_CFG_CR_STEP_M_2:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Contrast_Minus_2);
		break;
	case EXT_CFG_CR_STEP_M_1:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Contrast_Minus_1);
		break;
	case EXT_CFG_CR_STEP_0:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Contrast_Default);
		break;
	case EXT_CFG_CR_STEP_P_1:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Contrast_Plus_1);
		break;
	case EXT_CFG_CR_STEP_P_2:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Contrast_Plus_2);
		break;
	default:
		pr_info("[S5K4ECGX]Unexpected EXT_CFG_CR_CONTROL mode : %d\n",
			value);
		break;
	}
}

void s5k4ecgx_set_zoom(char value)
{
	switch (value) {
	case EXT_CFG_ZOOM_STEP_0:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_0);
		break;
	case EXT_CFG_ZOOM_STEP_1:
		if (s5k4ecgx_status.camera_mode == EXT_CFG_CAM_MODE_CAMERA) {
			switch (s5k4ecgx_status.snapshot_size) {
			case EXT_CFG_SNAPSHOT_SIZE_2048x1536_3M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_25_Zoom_1);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1600x1200_2M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_6_Zoom_1);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1280x960_1M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_1);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_1);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_1);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		} else {
			switch (s5k4ecgx_status.preview_size) {
			case EXT_CFG_PREVIEW_SIZE_720x480_D1:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_77_Zoom_1);
				break;
			case EXT_CFG_PREVIEW_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_1);
				break;
			case EXT_CFG_PREVIEW_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_1);
				break;
			case EXT_CFG_PREVIEW_SIZE_176x144_QCIF:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X4_Zoom_1);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		}
		break;
	case EXT_CFG_ZOOM_STEP_2:
		if (s5k4ecgx_status.camera_mode == EXT_CFG_CAM_MODE_CAMERA) {
			switch (s5k4ecgx_status.snapshot_size) {
			case EXT_CFG_SNAPSHOT_SIZE_2048x1536_3M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_25_Zoom_2);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1600x1200_2M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_6_Zoom_2);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1280x960_1M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_2);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_2);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_2);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		} else {
			switch (s5k4ecgx_status.preview_size) {
			case EXT_CFG_PREVIEW_SIZE_720x480_D1:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_77_Zoom_2);
				break;
			case EXT_CFG_PREVIEW_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_2);
				break;
			case EXT_CFG_PREVIEW_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_2);
				break;
			case EXT_CFG_PREVIEW_SIZE_176x144_QCIF:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X4_Zoom_2);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		}
		break;
	case EXT_CFG_ZOOM_STEP_3:
		if (s5k4ecgx_status.camera_mode == EXT_CFG_CAM_MODE_CAMERA) {
			switch (s5k4ecgx_status.snapshot_size) {
			case EXT_CFG_SNAPSHOT_SIZE_2048x1536_3M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_25_Zoom_3);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1600x1200_2M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_6_Zoom_3);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1280x960_1M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_3);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_3);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_3);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		} else {
			switch (s5k4ecgx_status.preview_size) {
			case EXT_CFG_PREVIEW_SIZE_720x480_D1:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_77_Zoom_3);
				break;
			case EXT_CFG_PREVIEW_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_3);
				break;
			case EXT_CFG_PREVIEW_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_3);
				break;
			case EXT_CFG_PREVIEW_SIZE_176x144_QCIF:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X4_Zoom_3);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		}
		break;
	case EXT_CFG_ZOOM_STEP_4:
		if (s5k4ecgx_status.camera_mode == EXT_CFG_CAM_MODE_CAMERA) {
			switch (s5k4ecgx_status.snapshot_size) {
			case EXT_CFG_SNAPSHOT_SIZE_2048x1536_3M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_25_Zoom_4);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1600x1200_2M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_6_Zoom_4);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1280x960_1M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_4);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_4);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_4);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		} else {
			switch (s5k4ecgx_status.preview_size) {
			case EXT_CFG_PREVIEW_SIZE_720x480_D1:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_77_Zoom_4);
				break;
			case EXT_CFG_PREVIEW_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_4);
				break;
			case EXT_CFG_PREVIEW_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_4);
				break;
			case EXT_CFG_PREVIEW_SIZE_176x144_QCIF:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X4_Zoom_4);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		}
		break;
	case EXT_CFG_ZOOM_STEP_5:
		if (s5k4ecgx_status.camera_mode == EXT_CFG_CAM_MODE_CAMERA) {
			switch (s5k4ecgx_status.snapshot_size) {
			case EXT_CFG_SNAPSHOT_SIZE_2048x1536_3M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_25_Zoom_5);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1600x1200_2M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_6_Zoom_5);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1280x960_1M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_5);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_5);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_5);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		} else {
			switch (s5k4ecgx_status.preview_size) {
			case EXT_CFG_PREVIEW_SIZE_720x480_D1:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_77_Zoom_5);
				break;
			case EXT_CFG_PREVIEW_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_5);
				break;
			case EXT_CFG_PREVIEW_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_5);
				break;
			case EXT_CFG_PREVIEW_SIZE_176x144_QCIF:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X4_Zoom_5);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		}
		break;
	case EXT_CFG_ZOOM_STEP_6:
		if (s5k4ecgx_status.camera_mode == EXT_CFG_CAM_MODE_CAMERA) {
			switch (s5k4ecgx_status.snapshot_size) {
			case EXT_CFG_SNAPSHOT_SIZE_2048x1536_3M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_25_Zoom_6);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1600x1200_2M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_6_Zoom_6);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1280x960_1M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_6);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_6);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_6);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		} else {
			switch (s5k4ecgx_status.preview_size) {
			case EXT_CFG_PREVIEW_SIZE_720x480_D1:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_77_Zoom_6);
				break;
			case EXT_CFG_PREVIEW_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_6);
				break;
			case EXT_CFG_PREVIEW_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_6);
				break;
			case EXT_CFG_PREVIEW_SIZE_176x144_QCIF:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X4_Zoom_6);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		}
		break;
	case EXT_CFG_ZOOM_STEP_7:
		if (s5k4ecgx_status.camera_mode == EXT_CFG_CAM_MODE_CAMERA) {
			switch (s5k4ecgx_status.snapshot_size) {
			case EXT_CFG_SNAPSHOT_SIZE_2048x1536_3M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_25_Zoom_7);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1600x1200_2M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_6_Zoom_7);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1280x960_1M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_7);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_7);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_7);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		} else {
			switch (s5k4ecgx_status.preview_size) {
			case EXT_CFG_PREVIEW_SIZE_720x480_D1:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_77_Zoom_7);
				break;
			case EXT_CFG_PREVIEW_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_7);
				break;
			case EXT_CFG_PREVIEW_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_7);
				break;
			case EXT_CFG_PREVIEW_SIZE_176x144_QCIF:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X4_Zoom_7);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		}
		break;
	case EXT_CFG_ZOOM_STEP_8:
		if (s5k4ecgx_status.camera_mode == EXT_CFG_CAM_MODE_CAMERA) {
			switch (s5k4ecgx_status.snapshot_size) {
			case EXT_CFG_SNAPSHOT_SIZE_2048x1536_3M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_25_Zoom_8);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1600x1200_2M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_6_Zoom_8);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_1280x960_1M:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_8);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_8);
				break;
			case EXT_CFG_SNAPSHOT_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_8);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		} else {
			switch (s5k4ecgx_status.preview_size) {
			case EXT_CFG_PREVIEW_SIZE_720x480_D1:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X1_77_Zoom_8);
				break;
			case EXT_CFG_PREVIEW_SIZE_640x480_VGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_8);
				break;
			case EXT_CFG_PREVIEW_SIZE_320x240_QVGA:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X2_Zoom_8);
				break;
			case EXT_CFG_PREVIEW_SIZE_176x144_QCIF:
				S5K4ECGX_WRITE_LIST(s5k4ecgx_X4_Zoom_8);
				break;
			default:
				pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n",
					value);
				break;
			}
		}
		break;
	default:
		pr_info("[S5K4ECGX]Unexpected ZOOM mode : %d\n", value);
		break;
	}
}

void s5k4ecgx_set_saturation(char value)
{
	switch (value) {
	case EXT_CFG_SA_STEP_M_2:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Saturation_Minus_2);
		break;
	case EXT_CFG_SA_STEP_M_1:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Saturation_Minus_1);
		break;
	case EXT_CFG_SA_STEP_0:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Saturation_Default);
		break;
	case EXT_CFG_SA_STEP_P_1:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Saturation_Plus_1);
		break;
	case EXT_CFG_SA_STEP_P_2:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Saturation_Plus_2);
		break;
	default:
		pr_info("[S5K4ECGX]Unexpected EXT_CFG_SA_CONTROL mode : %d\n",
			value);
		break;
	}
}

void s5k4ecgx_set_sharpness(char value)
{
	switch (value) {
	case EXT_CFG_SP_STEP_M_2:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Sharpness_Minus_2);
		break;
	case EXT_CFG_SP_STEP_M_1:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Sharpness_Minus_1);
		break;
	case EXT_CFG_SP_STEP_0:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Sharpness_Default);
		break;
	case EXT_CFG_SP_STEP_P_1:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Sharpness_Plus_1);
		break;
	case EXT_CFG_SP_STEP_P_2:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Sharpness_Plus_2);
		break;
	default:
		pr_info("[S5K4ECGX]Unexpected EXT_CFG_SP_CONTROL mode : %d\n",
			value);
		break;
	}
}

void s5k4ecgx_set_auto_contrast(char value)
{
	switch (value) {
	case EXT_CFG_AUTO_CONTRAST_ON:
		s5k4ecgx_set_contrast(EXT_CFG_CR_STEP_0);
		s5k4ecgx_set_saturation(EXT_CFG_SA_STEP_0);
		s5k4ecgx_set_sharpness(EXT_CFG_SP_STEP_0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Auto_Contrast_ON);
		break;
	case EXT_CFG_AUTO_CONTRAST_OFF:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Auto_Contrast_OFF);
		s5k4ecgx_set_contrast(s5k4ecgx_status.contrast);
		s5k4ecgx_set_saturation(s5k4ecgx_status.saturation);
		s5k4ecgx_set_sharpness(s5k4ecgx_status.sharpness);
		break;
	default:
		pr_info(" mode : %d\n", value);
		break;
	}
}

void s5k4ecgx_set_jpeg_quality(char value)
{
	switch (value) {
	case EXT_CFG_JPEG_QUALITY_SUPERFINE:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Jpeg_Quality_High);
		break;
	case EXT_CFG_JPEG_QUALITY_FINE:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Jpeg_Quality_Normal);
		break;
	case EXT_CFG_JPEG_QUALITY_NORMAL:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Jpeg_Quality_Low);
		break;
	default:
		pr_info(" mode : %d\n", value);
		break;
	}
}

void s5k4ecgx_set_fps(char value)
{
	switch (value) {
	case EXT_CFG_FRAME_AUTO:
		/*S5K4ECGX_WRITE_LIST(s5k4ecgx_FPS_15); */
		break;
	case EXT_CFG_FRAME_FIX_15:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_FPS_15);
		break;
	case EXT_CFG_FRAME_FIX_20:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_FPS_20);
		msleep(100);
		/*delay for pclk stabilization (noframe issue) 2011-01-20 */
		break;
	case EXT_CFG_FRAME_FIX_24:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_FPS_24);
		msleep(100); /*delay for pclk stabilization
		   (noframe issue) 2011-01-20 */
		break;
	case EXT_CFG_FRAME_FIX_25:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_FPS_25);
		msleep(100); /*delay for pclk stabilization
		   (noframe issue) 2011-01-20 */
		break;
	case EXT_CFG_FRAME_FIX_30:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_FPS_30);
		msleep(100);
		/*delay for pclk stabilization (noframe issue) 2011-01-20 */
		break;
	default:
		/*pr_info
		    ("[S5K4ECGX]Unexpected EXT_CFG_FRAME_CONTROL mode : %d\n",
		     value);*/
		break;
	}
}

static int s5k4ecgx_get_lux(int *lux)
{
	int msb = 0;
	int lsb = 0;
	int cur_lux = -1;

	s5k4ecgx_sensor_write(0x002C, 0x7000);
	s5k4ecgx_sensor_write(0x002E, 0x2C18);	/*for EVT 1.1 */
	s5k4ecgx_sensor_read(0x0F12, (unsigned short *)&lsb);
	s5k4ecgx_sensor_read(0x0F12, (unsigned short *)&msb);

	cur_lux = (msb << 16) | lsb;
	*lux = cur_lux;
	CAMDRV_DEBUG("%s, s5k4ecgx_status.current_lux is %d\n", __func__,
		     s5k4ecgx_status.current_lux);
	return cur_lux;
	/*this value is under 0x0032 in low light condition */
}

static int s5k4ecgx_set_flash_init(void)
{				/*yjh_flash */
	int i = 0;
	gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
	udelay(550);

	for (i = 0; i < 5; i++) {
		gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
		udelay(1);
		gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
		udelay(1);
	}

	udelay(1050);

	for (i = 0; i < 27; i++) {
		gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
		udelay(1);
		gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
		udelay(1);
	}
	udelay(1050);

	gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
	return;
}

static int s5k4ecgx_factory_flash(int lux_val)
{				/* yjh_flash */
	int i = 0;

	CAMDRV_DEBUG("%s, flash set is %d\n", __func__, lux_val);

	if (lux_val == 0) {
		gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
		return 0;
		}

	/* initailize falsh IC */
	gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
	gpio_set_value_cansleep(CAM_FLASH_FLEN, 0);
	usleep(1000);		/*to enter a shutdown mode */

	/* set to flash mode */

		/* set to movie mode */

		CAMDRV_DEBUG("%s, 2flash set is %d\n", __func__, lux_val);

		gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
		udelay(550);

		for (i = 0; i < 1; i++) {
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
			udelay(1);
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
			udelay(1);
		}

		udelay(1050);

		for (i = 0; i < 16; i++) {
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
			udelay(1);
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
			udelay(1);
		}
		udelay(1050);
		/*gpio_set_value_cansleep(CAM_FLASH_ENSET,0); //value set */



	/* setting a sensor #2 */

		    return 0;
}


static int s5k4ecgx_set_flash(int lux_val)
{				/* yjh_flash */
	if (torch_mode_on)
		return 0;

	int i = 0;

	CAMDRV_DEBUG("%s, flash set is %d\n", __func__, lux_val);
	CAMDRV_DEBUG("%s, flash_mode %d\n", s5k4ecgx_status.flash_mode);

	if (lux_val == 0) {
		/* set to no detect mode */
		gpio_set_value_cansleep(CAM_FLASH_FLEN, 0);
		udelay(550);

		gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
		udelay(550);

		for (i = 0; i < 1; i++) {
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
			udelay(1);
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
			udelay(1);
		}

		udelay(1050);

		for (i = 0; i < 13; i++) {
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
			udelay(1);
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
			udelay(1);
		}
		udelay(1050);
		/*gpio_set_value_cansleep(CAM_FLASH_ENSET,0); //value set */

	}

	if (s5k4ecgx_status.flash_mode == EXT_CFG_FLASH_OFF)
		return 0;

	/* initailize falsh IC */
	gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
	gpio_set_value_cansleep(CAM_FLASH_FLEN, 0);
	usleep(1000);		/*to enter a shutdown mode */

	/* set to flash mode */
	if (lux_val > 16) {
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Main_Flash_On);
		s5k4ecgx_set_flash_init();
		gpio_set_value_cansleep(CAM_FLASH_FLEN, 1);
/*       s5k4ecgx_status.flash_exifinfo = true;*/
#ifdef USE_FLASHOFF_TIMER
		add_timer(&flashoff_timer);	/*for prevent LED */
#endif
	} else if (lux_val == MACRO_FLASH) {
		CAMDRV_DEBUG("%s, 1flash set is %d\n", __func__, lux_val);
		/* set to movie mode */
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Main_Flash_On);
		gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
		udelay(550);

		for (i = 0; i < 1; i++) {
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
			udelay(1);
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
			udelay(1);
		}

		udelay(1050);

		for (i = 0; i < 16; i++) {
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
			udelay(1);
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
			udelay(1);
		}
		udelay(1050);
		/*gpio_set_value_cansleep(CAM_FLASH_ENSET,0); //value set
		   s5k4ecgx_status.flash_exifinfo = true; */
	} else if (lux_val > 0 && lux_val <= 16) {
		/* set to movie mode */
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Pre_Flash_On);
		CAMDRV_DEBUG("%s, 2flash set is %d\n", __func__, lux_val);

		gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
		udelay(550);

		for (i = 0; i < 1; i++) {
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
			udelay(1);
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
			udelay(1);
		}

		udelay(1050);

		for (i = 0; i < 16; i++) {
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 0);
			udelay(1);
			gpio_set_value_cansleep(CAM_FLASH_ENSET, 1);
			udelay(1);
		}
		udelay(1050);
		/*gpio_set_value_cansleep(CAM_FLASH_ENSET,0); //value set */

	}

	s5k4ecgx_status.flash_status = lux_val;
	CAMDRV_DEBUG("%s, 9flash set is %d\n", __func__, lux_val);
	/* setting a sensor #2 */
	if (lux_val == PRE_FLASH_OFF)
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Pre_Flash_Off)
		    else
	if (lux_val == FLASH_OFF && s5k4ecgx_status.afcanceled == false)
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Main_Flash_Off)
		    else
	if (lux_val == FLASH_OFF && s5k4ecgx_status.afcanceled == true)
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Pre_Flash_Off)

		    return 0;
}

int s5k4ecgx_set_af(char value)
{
	int val = 0, ret = 0;
	static int pre_flash_on;	/* yjh_flash */
	/*CAMDRV_DEBUG("%s : %d\n", __func__, value); */
	switch (value) {
	case EXT_CFG_AF_CHECK_STATUS:
		s5k4ecgx_sensor_write(0x002C, 0x7000);
		s5k4ecgx_sensor_write(0x002E, 0x2EEE);
		s5k4ecgx_sensor_read(0x0F12, (unsigned short *)&val);
		switch (val & 0xFF) {
		case 1:
			/*CAMDRV_DEBUG("%s : EXT_CFG_AF_CHECK_STATUS
			    -EXT_CFG_AF_PROGRESS \n", __func__);*/
			ret = EXT_CFG_AF_PROGRESS;
			break;
		case 2:
			/*CAMDRV_DEBUG("%s : EXT_CFG_AF_CHECK_STATUS
			    -EXT_CFG_AF_SUCCESS \n", __func__);*/
			ret = EXT_CFG_AF_SUCCESS;
			break;
		default:
			/*CAMDRV_DEBUG("%s : EXT_CFG_AF_CHECK_STATUS
			    -EXT_CFG_AF_LOWCONF \n", __func__);*/
			ret = EXT_CFG_AF_LOWCONF;
			break;
		}
		break;
	case EXT_CFG_AF_CHECK_2nd_STATUS:
		s5k4ecgx_sensor_write(0x002C, 0x7000);
		s5k4ecgx_sensor_write(0x002E, 0x2207);
		s5k4ecgx_sensor_read(0x0F12, (unsigned short *)&val);
		switch (val & 0xFF) {
		case 1:
			/*CAMDRV_DEBUG("%s : EXT_CFG_AF_CHECK_2nd_STATUS
			    -EXT_CFG_AF_PROGRESS \n", __func__);*/
			ret = EXT_CFG_AF_PROGRESS;
			break;
		case 0:
			/*CAMDRV_DEBUG("%s : EXT_CFG_AF_CHECK_2nd_STATUS
			    -EXT_CFG_AF_SUCCESS \n", __func__);*/
			ret = EXT_CFG_AF_SUCCESS;
			if (flashOnFromApps == 0)
				s5k4ecgx_set_flash(PRE_FLASH_OFF);
			break;
		default:
			/*CAMDRV_DEBUG("%s : EXT_CFG_AF_CHECK_2nd_STATUS
			    -EXT_CFG_AF_PROGRESS \n", __func__);*/
			ret = EXT_CFG_AF_PROGRESS;
			break;
		}
		break;
	case EXT_CFG_AF_CHECK_AE_STATUS:
		{
			s5k4ecgx_sensor_write(0x002C, 0x7000);
			s5k4ecgx_sensor_write(0x002E, 0x2C74);
			s5k4ecgx_sensor_read(0x0F12, (unsigned short *)&val);
			switch (val & 0xFF) {
			case 1:
				CAMDRV_DEBUG("%s : EXT_CFG_AF_CHECK_AE_STATUS\
				    -EXT_CFG_AE_STABLE \n", __func__);\
				ret = EXT_CFG_AE_STABLE;
				break;
			default:
				CAMDRV_DEBUG("%s : EXT_CFG_AF_CHECK_AE_STATUS\
				    -EXT_CFG_AE_UNSTABLE \n", __func__);\
				ret = EXT_CFG_AE_UNSTABLE;
				break;
			}
		}
		break;
	case EXT_CFG_AF_SET_NORMAL:
		CAMDRV_DEBUG("%s : EXT_CFG_AF_SET_NORMAL\n", __func__);
		macro_mode = 0;
		S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Normal_mode_1);
		if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
		    || (s5k4ecgx_status.scene == EXT_CFG_SCENE_FIREWORK))
			msleep(250);
		else
			msleep(200);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Normal_mode_2);
		if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
		    || (s5k4ecgx_status.scene == EXT_CFG_SCENE_FIREWORK))
			msleep(250);
		else
			msleep(200);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Normal_mode_3);
		if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
		    || (s5k4ecgx_status.scene == EXT_CFG_SCENE_FIREWORK))
			msleep(250);
		else
			msleep(200);
		break;
	case EXT_CFG_AF_SET_MACRO:
		CAMDRV_DEBUG("%s : EXT_CFG_AF_SET_MACRO\n", __func__);
		macro_mode = 1;
		S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Macro_mode_1);
		if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
		    || (s5k4ecgx_status.scene == EXT_CFG_SCENE_FIREWORK))
			msleep(250);
		else
			msleep(100);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Macro_mode_2);
		if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
		    || (s5k4ecgx_status.scene == EXT_CFG_SCENE_FIREWORK))
			msleep(250);
		else
			msleep(100);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Macro_mode_3);
		if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
		    || (s5k4ecgx_status.scene == EXT_CFG_SCENE_FIREWORK))
			msleep(250);
		else
			msleep(100);
		break;
	case EXT_CFG_AF_OFF:
		CAMDRV_DEBUG("%s : EXT_CFG_AF_OFF (afmode:%d)\n", __func__,
			     s5k4ecgx_status.afmode);
		s5k4ecgx_status.afcanceled = true;
		if (s5k4ecgx_status.afmode == EXT_CFG_AF_SET_NORMAL) {
			S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Normal_mode_1);
			if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
			    || (s5k4ecgx_status.scene ==
				EXT_CFG_SCENE_FIREWORK))
				msleep(250);
			else
				msleep(300);
			S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Normal_mode_2);
			if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
			    || (s5k4ecgx_status.scene ==
				EXT_CFG_SCENE_FIREWORK))
				msleep(250);
			else
				msleep(300);
			S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Normal_mode_3);
			if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
			    || (s5k4ecgx_status.scene ==
				EXT_CFG_SCENE_FIREWORK))
				msleep(250);
			else
				msleep(300);
		} else {
			S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Macro_mode_1);
			if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
			    || (s5k4ecgx_status.scene ==
				EXT_CFG_SCENE_FIREWORK))
				msleep(250);
			else
				msleep(300);
			S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Macro_mode_2);
			if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
			    || (s5k4ecgx_status.scene ==
				EXT_CFG_SCENE_FIREWORK))
				msleep(250);
			else
				msleep(300);
			S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Macro_mode_3);
			if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
			    || (s5k4ecgx_status.scene ==
				EXT_CFG_SCENE_FIREWORK))
				msleep(250);
			else
				msleep(300);
		}
		break;
	case EXT_CFG_AF_DO:
		CAMDRV_DEBUG("%s : EXT_CFG_AF_DO\n", __func__);
		s5k4ecgx_status.afcanceled = false;
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Single_AF_Start);
		break;
	case EXT_CFG_AF_SET_AE_FOR_FLASH:	/* yjh_flash */
		CAMDRV_DEBUG("%s : EXT_CFG_AF_SET_AE_FOR_FLASH\n", __func__);
		s5k4ecgx_get_lux(&s5k4ecgx_status.current_lux);
		if (s5k4ecgx_status.flash_mode != EXT_CFG_FLASH_OFF) {
			CAMDRV_DEBUG("%s FLASH_1 **\n", __func__);
			if (s5k4ecgx_status.flash_mode == EXT_CFG_FLASH_AUTO) {
				CAMDRV_DEBUG("%s FLASH_2 **\n", __func__);
				if (s5k4ecgx_status.current_lux > 0x0032)
					break;
			}
			s5k4ecgx_sensor_write(0x0028, 0x7000);
			s5k4ecgx_sensor_write(0x002A, 0x057C);
			s5k4ecgx_sensor_write(0x0F12, 0x0000);
			if (flashOnFromApps == 0) {
				if (s5k4ecgx_status.camera_mode ==
					EXT_CFG_CAM_MODE_FACTORY_TEST)
					s5k4ecgx_set_flash(MACRO_FLASH);
				else
					s5k4ecgx_set_flash(PRE_FLASH);
				CAMDRV_DEBUG("%s FLASH_3 **\n", __func__);
				msleep(500);
				pre_flash_on = 1;
				}
		}
		break;
	case EXT_CFG_AF_BACK_AE_FOR_FLASH:
		CAMDRV_DEBUG("%s : EXT_CFG_AF_BACK_AE_FOR_FLASH\n", __func__);
		if (pre_flash_on && flashOnFromApps == 0) {
			s5k4ecgx_sensor_write(0x0028, 0x7000);
			s5k4ecgx_sensor_write(0x002A, 0x057C);
			s5k4ecgx_sensor_write(0x0F12, 0x0002);
		}
		s5k4ecgx_set_flash(PRE_FLASH_OFF);
		pre_flash_on = 0;
		break;

	case EXT_CFG_AF_POWEROFF:
		CAMDRV_DEBUG("%s : EXT_CFG_AF_POWEROFF\n", __func__);
		if (!macro_mode) {
			S5K4ECGX_WRITE_LIST(s5k4ecgx_Preview_Return);
			S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Normal_mode_1);
			if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
				|| (s5k4ecgx_status.scene ==
				EXT_CFG_SCENE_FIREWORK))
				msleep(250);
			else
				usleep(200*1000);
			S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Normal_mode_2);
			if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
				|| (s5k4ecgx_status.scene ==
				EXT_CFG_SCENE_FIREWORK))
				usleep(250*1000);
			else
				usleep(200*1000);
			S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Normal_mode_3);
			if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
				|| (s5k4ecgx_status.scene ==
				EXT_CFG_SCENE_FIREWORK))
				msleep(250);
			else
				usleep(200*1000);
			} else {
			S5K4ECGX_WRITE_LIST(s5k4ecgx_Preview_Return);
			S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Normal_mode_1);
			if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
				|| (s5k4ecgx_status.scene ==
				EXT_CFG_SCENE_FIREWORK))
				msleep(250);
			else
				usleep(500*1000);
			S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Normal_mode_2);
			if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
				|| (s5k4ecgx_status.scene ==
				EXT_CFG_SCENE_FIREWORK))
				usleep(250*1000);
			else
				usleep(500*1000);
			S5K4ECGX_WRITE_LIST(s5k4ecgx_AF_Normal_mode_3);
			if ((s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT)
				|| (s5k4ecgx_status.scene ==
				EXT_CFG_SCENE_FIREWORK))
				msleep(250);
			else
				usleep(500*1000);
			}
		break;
	default:
			pr_info("[S5K4ECGX] Unexpected AF command : %d\n",
				value);
		break;
	}
	return ret;
}

void s5k4ecgx_set_DTP(char value)
{
	switch (value) {
	case EXT_CFG_DTP_OFF:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_DTP_stop);
		/*pr_info("hoon_1\n"); */
		break;
	case EXT_CFG_DTP_ON:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_DTP_init);
		/*pr_info("hoon_2\n");*/
		break;
	default:
		/*pr_info("[S5K4ECGX] Unexpected DTP control on EXT_CFG\n");*/
		break;
	}
}

#ifdef FACTORY_TEST
struct class *camera_class;
struct device *s5k4ecgx_dev;
static ssize_t cameraflash_file_cmd_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	/* Reserved */
	return 0;
}

static ssize_t cameraflash_file_cmd_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	int value;

	sscanf(buf, "%d", &value);

	if (value == 0) {
		printk(KERN_INFO "[Factory Test] flash OFF\n");
		s5k4ecgx_factory_flash(FLASH_OFF);
		torch_mode_on = 0;
	} else {
		printk(KERN_INFO "[Factory Test] flash ON\n");
		s5k4ecgx_factory_flash(MOVIE_FLASH);
		torch_mode_on = 1;
	}

	return size;
}

static DEVICE_ATTR(rear_flash,
	0660, cameraflash_file_cmd_show, cameraflash_file_cmd_store);

#endif

void s5k4ecgx_set_ae_lock(char value)
{
	switch (value) {
	case EXT_CFG_AE_LOCK:
		s5k4ecgx_status.ae_lock = EXT_CFG_AE_LOCK;
		S5K4ECGX_WRITE_LIST(s5k4ecgx_ae_lock);
		break;
	case EXT_CFG_AE_UNLOCK:
		s5k4ecgx_status.ae_lock = EXT_CFG_AE_UNLOCK;
		S5K4ECGX_WRITE_LIST(s5k4ecgx_ae_unlock);
		break;
	case EXT_CFG_AWB_LOCK:
		s5k4ecgx_status.awb_lock = EXT_CFG_AWB_LOCK;
		S5K4ECGX_WRITE_LIST(s5k4ecgx_awb_lock);
		break;
	case EXT_CFG_AWB_UNLOCK:
		s5k4ecgx_status.awb_lock = EXT_CFG_AWB_UNLOCK;
		S5K4ECGX_WRITE_LIST(s5k4ecgx_awb_unlock);
		break;
	default:
		pr_info("[S5K4ECGX]Unexpected AWB_AE mode : %d\n", value);
		break;
	}
}

void s5k4ecgx_set_scene(char value)
{
	/*pr_info("hoon2 = %d", value);*/
	if (value != EXT_CFG_SCENE_OFF) {
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Default);
		/*if(value == EXT_CFG_SCENE_TEXT)
		   s5k4ecgx_set_af(EXT_CFG_AF_SET_MACRO);
		   else s5k4ecgx_set_af(EXT_CFG_AF_SET_NORMAL); */
		if (s5k4ecgx_status.auto_contrast != EXT_CFG_AUTO_CONTRAST_OFF)
			s5k4ecgx_set_auto_contrast(EXT_CFG_AUTO_CONTRAST_OFF);
	}

	switch (value) {
	case EXT_CFG_SCENE_OFF:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Default);
		/*s5k4ecgx_set_af(s5k4ecgx_status.afmode); */
		if (s5k4ecgx_status.effect != EXT_CFG_EFFECT_NORMAL)
			s5k4ecgx_set_effect(s5k4ecgx_status.effect);
		if (s5k4ecgx_status.brightness != EXT_CFG_BR_STEP_0)
			s5k4ecgx_set_brightness(s5k4ecgx_status.brightness);
		if (s5k4ecgx_status.auto_exposure != EXT_CFG_METERING_NORMAL)
			s5k4ecgx_set_metering(s5k4ecgx_status.auto_exposure);
		if (s5k4ecgx_status.iso != EXT_CFG_ISO_AUTO)
			s5k4ecgx_set_iso(s5k4ecgx_status.iso);
		if (s5k4ecgx_status.contrast != EXT_CFG_CR_STEP_0)
			s5k4ecgx_set_contrast(s5k4ecgx_status.contrast);
		if (s5k4ecgx_status.saturation != EXT_CFG_SA_STEP_0)
			s5k4ecgx_set_saturation(s5k4ecgx_status.saturation);
		if (s5k4ecgx_status.sharpness != EXT_CFG_SP_STEP_0)
			s5k4ecgx_set_sharpness(s5k4ecgx_status.sharpness);
		if (s5k4ecgx_status.whiteBalance != EXT_CFG_WB_AUTO)
			s5k4ecgx_set_whitebalance(s5k4ecgx_status.whiteBalance);
		if (s5k4ecgx_status.auto_contrast != EXT_CFG_AUTO_CONTRAST_OFF)
			s5k4ecgx_set_auto_contrast
			    (s5k4ecgx_status.auto_contrast);
		break;
	case EXT_CFG_SCENE_PORTRAIT:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Portrait);
		break;
	case EXT_CFG_SCENE_LANDSCAPE:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Landscape);
		s5k4ecgx_set_metering(EXT_CFG_METERING_NORMAL);
		break;
	case EXT_CFG_SCENE_SPORTS:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Sports);
		break;
	case EXT_CFG_SCENE_PARTY:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(5, 0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Party_Indoor);
		break;
	case EXT_CFG_SCENE_BEACH:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(5, 0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Beach_Snow);
		break;
	case EXT_CFG_SCENE_SUNSET:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(3, 0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Sunset);
		break;
	case EXT_CFG_SCENE_DAWN:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(3, 0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Duskdawn);
		break;
	case EXT_CFG_SCENE_FALL:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Fall_Color);
		break;
	case EXT_CFG_SCENE_NIGHTSHOT:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Nightshot);
		break;
	case EXT_CFG_SCENE_BACKLIGHT:
		/*S5K4ECGX_WRITE_LIST(s5k4ecgx_Metering_Center); */
		if (s5k4ecgx_status.flash_mode == EXT_CFG_FLASH_OFF
		    && isPreviewReturnWrite)
			s5k4ecgx_set_metering(EXT_CFG_METERING_SPOT);
		else
			s5k4ecgx_set_metering(EXT_CFG_METERING_CENTER);
		/*if(s5k4ecgx_status.flash_mode == EXT_CFG_FLASH_ON
		   ||s5k4ecgx_status.flash_mode == EXT_CFG_FLASH_AUTO)
		   s5k4ecgx_set_metering(EXT_CFG_METERING_CENTER);
		   else s5k4ecgx_set_metering(EXT_CFG_METERING_SPOT); */
		break;
	case EXT_CFG_SCENE_FIREWORK:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Fireworks);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_ISO_50);
		break;
	case EXT_CFG_SCENE_TEXT:
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Text);
		break;
	case EXT_CFG_SCENE_CANDLE:
		s5k4ecgx_set_REG_TC_DBG_AutoAlgEnBits(3, 0);
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Scene_Candle_Light);
		break;
	default:
		pr_info("[S5K4ECGX]Unexpected SCENE mode : %d\n", value);
		break;
	}
}

void s5k4ecgx_set_capture_size(int size)
{
	/*pr_info("s5k4ecgx_set_capture_size %d\n", size);*/

	if (size == EXT_CFG_SNAPSHOT_SIZE_2560x1920_5M) {
		S5K4ECGX_WRITE_LIST(s5k4ecgx_5M_Capture);
		s5k4ecgx_set_zoom(EXT_CFG_ZOOM_STEP_0);
	} else if (size == EXT_CFG_SNAPSHOT_SIZE_2048x1536_3M)
		S5K4ECGX_WRITE_LIST(s5k4ecgx_3M_Capture)
		    else
	if (size == EXT_CFG_SNAPSHOT_SIZE_1600x1200_2M)
		S5K4ECGX_WRITE_LIST(s5k4ecgx_2M_Capture)
		    else
	if (size == EXT_CFG_SNAPSHOT_SIZE_1280x960_1M)
		S5K4ECGX_WRITE_LIST(s5k4ecgx_1M_Capture)
		    else
	if (size == EXT_CFG_SNAPSHOT_SIZE_640x480_VGA)
		S5K4ECGX_WRITE_LIST(s5k4ecgx_VGA_Capture)
		    else
	if (size == EXT_CFG_SNAPSHOT_SIZE_320x240_QVGA)
		S5K4ECGX_WRITE_LIST(s5k4ecgx_QVGA_Capture)
		    else
		pr_info("[S5K4ECGX]s5k4ecgx_set_capture : wrong size!!!\n");
	if (size != EXT_CFG_SNAPSHOT_SIZE_2560x1920_5M
	    && s5k4ecgx_status.zoom != EXT_CFG_ZOOM_STEP_0)
		s5k4ecgx_set_zoom(s5k4ecgx_status.zoom);

	if (size == EXT_CFG_SNAPSHOT_SIZE_320x240_QVGA) {
		/* change thumbnail size to 320x240 */
		s5k4ecgx_sensor_write(0x0028, 0x7000);
		s5k4ecgx_sensor_write(0x002A, 0x047E);
		s5k4ecgx_sensor_write(0x0F12, 0x0140);
		s5k4ecgx_sensor_write(0x0F12, 0x00F0);
	} else {
		/* change thumbnail size to 640x480 */
		s5k4ecgx_sensor_write(0x0028, 0x7000);
		s5k4ecgx_sensor_write(0x002A, 0x047E);
		s5k4ecgx_sensor_write(0x0F12, 0x0280);
		s5k4ecgx_sensor_write(0x0F12, 0x01E0);
	}
}

void s5k4ecgx_set_preview_size(int size)
{
	CAMDRV_DEBUG("s5k4ecgx_set_preview_size %d\n", size);

	if (size == EXT_CFG_PREVIEW_SIZE_640x480_VGA)
		S5K4ECGX_WRITE_LIST(s5k4ecgx_640_Preview)
		    else
	if (size == EXT_CFG_PREVIEW_SIZE_176x144_QCIF)
		S5K4ECGX_WRITE_LIST(s5k4ecgx_640_Preview)
		    else
	if (size == EXT_CFG_PREVIEW_SIZE_320x240_QVGA)
		S5K4ECGX_WRITE_LIST(s5k4ecgx_176_Preview)
		    else
	if (size == EXT_CFG_PREVIEW_SIZE_720x480_D1)
		S5K4ECGX_WRITE_LIST(s5k4ecgx_720_Preview)
		    else
		pr_info("[S5K4ECGX]s5k4ecgx_set_preview : wrong size!!!\n");
}

void s5k4ecgx_check_REG_TC_GP_EnablePreviewChanged(void)
{
	int cnt = 0;
	int REG_TC_GP_EnablePreviewChanged = 0;
	pr_info("[S5K4ECGX]s5k4ecgx_check_REG_TC_GP_EnablePreviewChanged\n");
	while (cnt < 150) {
		s5k4ecgx_sensor_write(0x002C, 0x7000);
		s5k4ecgx_sensor_write(0x002E, 0x0244);
		s5k4ecgx_sensor_read(0x0F12, (unsigned short *)
				     &REG_TC_GP_EnablePreviewChanged);
		if (!REG_TC_GP_EnablePreviewChanged)
			break;
		usleep(10000);
		cnt++;
	}
	if (cnt)
		pr_info("[S5K4ECGX] wait time for preview frame : %dms\n",
			cnt * 10);
	if (REG_TC_GP_EnablePreviewChanged)
		pr_info("[S5K4ECGX] start preview failed.\n");
}

int s5k4ecgx_mipi_mode(void)
{
	int rc = 0;
	struct msm_camera_csi_params s5k4ecgx_csi_params;

	CAMDRV_DEBUG("%s E\n", __func__);

	s5k4ecgx_csi_params.lane_cnt = 2;
	s5k4ecgx_csi_params.data_format = CSI_8BIT;
	s5k4ecgx_csi_params.lane_assign = 0xe4;
	s5k4ecgx_csi_params.dpcm_scheme = 0;
	s5k4ecgx_csi_params.settle_cnt = 0x14;
	rc = msm_camio_csi_config(&s5k4ecgx_csi_params);
	if (rc < 0)
		printk(KERN_ERR "config csi controller failed\n");

	CAMDRV_DEBUG("%s X\n", __func__);
	return rc;
}

void s5k4ecgx_set_preview(void)
{
	if (s5k4ecgx_status.camera_initailized == false) {
		s5k4ecgx_mipi_mode();
		msleep(300);
	}
	s5k4ecgx_set_flash(FLASH_OFF);

	if (s5k4ecgx_status.camera_initailized == false) {
#ifdef CONFIG_LOAD_FILE
		S5K4ECGX_WRITE_LIST(s5k4ecgx_init_reg1);
		msleep(100);
#endif
		S5K4ECGX_WRITE_LIST(s5k4ecgx_init_reg2);
	} else if (s5k4ecgx_status.camera_initailized == true) {
		if (!isPreviewReturnWrite) {
			S5K4ECGX_WRITE_LIST(s5k4ecgx_Preview_Return);
			isPreviewReturnWrite = true;
		}
	}
	s5k4ecgx_check_REG_TC_GP_EnablePreviewChanged();

#define DATA_LINE_CHECK
#ifdef DATA_LINE_CHECK
	if (s5k4ecgx_status.need_configuration & CHECK_DTP
	    && s5k4ecgx_status.dtp != EXT_CFG_DTP_OFF)
		s5k4ecgx_set_DTP(s5k4ecgx_status.dtp);
#endif

	/*S5K4ECGX_WRITE_LIST(s5k4ecgx_Flash_init);*/
	CAMDRV_DEBUG("[S5K4ECGX] sensor configuration start\n");
	/*S5K4ECGX_WRITE_LIST(s5k4ecgx_awb_ae_unlock); */
	if (s5k4ecgx_status.scene == EXT_CFG_SCENE_OFF) {
		if (s5k4ecgx_status.need_configuration & CHECK_EFFECT
		    && s5k4ecgx_status.effect != EXT_CFG_EFFECT_NORMAL)
			s5k4ecgx_set_effect(s5k4ecgx_status.effect);
		if (s5k4ecgx_status.need_configuration & CHECK_BRIGHTNESS
		    && s5k4ecgx_status.brightness != EXT_CFG_BR_STEP_0)
			s5k4ecgx_set_brightness(s5k4ecgx_status.brightness);
		if (s5k4ecgx_status.need_configuration & CHECK_AE
		    && s5k4ecgx_status.auto_exposure != EXT_CFG_METERING_CENTER)
			s5k4ecgx_set_metering(s5k4ecgx_status.auto_exposure);
		if (s5k4ecgx_status.need_configuration & CHECK_ISO
		    && s5k4ecgx_status.iso != EXT_CFG_ISO_AUTO)
			s5k4ecgx_set_iso(s5k4ecgx_status.iso);
		if (s5k4ecgx_status.need_configuration & CHECK_CONTRAST
		    && s5k4ecgx_status.contrast != EXT_CFG_CR_STEP_0)
			s5k4ecgx_set_contrast(s5k4ecgx_status.contrast);
		if (s5k4ecgx_status.need_configuration & CHECK_SATURATION
		    && s5k4ecgx_status.saturation != EXT_CFG_SA_STEP_0)
			s5k4ecgx_set_saturation(s5k4ecgx_status.saturation);
		if (s5k4ecgx_status.need_configuration & CHECK_SHARPNESS
		    && s5k4ecgx_status.sharpness != EXT_CFG_SP_STEP_0)
			s5k4ecgx_set_sharpness(s5k4ecgx_status.sharpness);
		if (s5k4ecgx_status.need_configuration & CHECK_WB
		    && s5k4ecgx_status.whiteBalance != EXT_CFG_WB_AUTO)
			s5k4ecgx_set_whitebalance(s5k4ecgx_status.whiteBalance);
		if (s5k4ecgx_status.need_configuration & CHECK_AFMODE
		    && s5k4ecgx_status.camera_initailized == false)
			s5k4ecgx_set_af(s5k4ecgx_status.afmode);
		if (s5k4ecgx_status.need_configuration & CHECK_AUTOCONTRAST
		    && s5k4ecgx_status.auto_contrast !=
		    EXT_CFG_AUTO_CONTRAST_OFF)
			s5k4ecgx_set_auto_contrast
			    (s5k4ecgx_status.auto_contrast);
	} else {
		s5k4ecgx_set_scene(s5k4ecgx_status.scene);
	}
	if (s5k4ecgx_status.need_configuration & CHECK_FPS)
		s5k4ecgx_set_fps(s5k4ecgx_status.fps);
	if (s5k4ecgx_status.need_configuration & CHECK_ZOOM
	    && s5k4ecgx_status.zoom != EXT_CFG_ZOOM_STEP_0)
		s5k4ecgx_set_zoom(s5k4ecgx_status.zoom);
	if (s5k4ecgx_status.need_configuration & CHECK_JPEGQUALITY
	    && s5k4ecgx_status.jpeg_quality != EXT_CFG_JPEG_QUALITY_SUPERFINE)
		s5k4ecgx_set_jpeg_quality(s5k4ecgx_status.jpeg_quality);
	if (s5k4ecgx_status.need_configuration & CHECK_SNAPSHOT_SIZE
	    && s5k4ecgx_status.snapshot_size !=
	    EXT_CFG_SNAPSHOT_SIZE_2560x1920_5M)
		s5k4ecgx_set_capture_size(s5k4ecgx_status.snapshot_size);

	/*delay for pclk stabilization (noframe issue) 2011-01-20 */
	if (s5k4ecgx_status.camera_initailized == false)
		msleep(100);

	s5k4ecgx_status.preview_size = EXT_CFG_PREVIEW_SIZE_640x480_VGA;
	s5k4ecgx_status.flash_status = FLASH_OFF;
	s5k4ecgx_status.camera_initailized = true;
	s5k4ecgx_status.lowcap_on = false;
	s5k4ecgx_status.nightcap_on = false;
	s5k4ecgx_status.afcanceled = false;
	s5k4ecgx_status.camera_status = PREVIEW;

	/*pr_info("s5k4ecgx_set_preview X\n");*/
}

void s5k4ecgx_check_REG_TC_GP_EnableCaptureChanged(void)
{
	int cnt = 0;
	int REG_TC_GP_EnableCaptureChanged = 0;

	while (cnt < 150) {
		s5k4ecgx_sensor_write(0x002C, 0x7000);
		s5k4ecgx_sensor_write(0x002E, 0x0244);
		s5k4ecgx_sensor_read(0x0F12, (unsigned short *)
				     &REG_TC_GP_EnableCaptureChanged);
		if (!REG_TC_GP_EnableCaptureChanged)
			break;
		usleep(10000);
		cnt++;
	}
	if (cnt)
		pr_info("[S5K4ECGX] wait time for capture frame : %dms\n",
			cnt * 10);
	if (REG_TC_GP_EnableCaptureChanged)
		pr_info("[S5K4ECGX] take picture failed.\n");
}

void s5k4ecgx_set_capture(void)
{				/* yjh_flash */
	isPreviewReturnWrite = false;
	s5k4ecgx_status.camera_status = SNAPSHOT;


	/* Check current lux */
	if (s5k4ecgx_status.flash_status == FLASH_OFF)
		s5k4ecgx_get_lux(&s5k4ecgx_status.current_lux);

	/* CASE 1 : Capture with no flash
	   (System lag will be measured in this condition) */
	if (s5k4ecgx_status.flash_mode != EXT_CFG_FLASH_ON
	    && s5k4ecgx_status.current_lux > 0x0032) {
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Capture_Start);
		s5k4ecgx_check_REG_TC_GP_EnableCaptureChanged();
		return;
	}
	/* CASE 2 : Flash turn on  in normal light condition */
	else if (s5k4ecgx_status.flash_mode == EXT_CFG_FLASH_ON
		 && s5k4ecgx_status.current_lux > 0x0032) {
		/* ae/awb unlock */
		if (s5k4ecgx_status.ae_lock == EXT_CFG_AE_LOCK)
			s5k4ecgx_set_ae_lock(EXT_CFG_AE_UNLOCK);
		if (s5k4ecgx_status.awb_lock == EXT_CFG_AWB_LOCK)
			s5k4ecgx_set_ae_lock(EXT_CFG_AWB_UNLOCK);

		/* use torch mode on below condition */
		if (s5k4ecgx_status.camera_mode == EXT_CFG_CAM_MODE_FACTORY_TEST
		    || s5k4ecgx_status.afmode == EXT_CFG_AF_SET_MACRO) {
			CAMDRV_DEBUG("%s FLASH_4 **\n", __func__);
			s5k4ecgx_set_flash(MACRO_FLASH);
		} else {
			CAMDRV_DEBUG("%s FLASH_5 **\n", __func__);
			s5k4ecgx_set_flash(FULL_FLASH);
		}
		msleep(200);

		S5K4ECGX_WRITE_LIST(s5k4ecgx_Capture_Start);
		s5k4ecgx_check_REG_TC_GP_EnableCaptureChanged();
		return;
	} else {
		/*low light condition */

		if (s5k4ecgx_status.scene == EXT_CFG_SCENE_NIGHTSHOT
		    || s5k4ecgx_status.scene == EXT_CFG_SCENE_FIREWORK) {
			S5K4ECGX_WRITE_LIST(s5k4ecgx_Night_Mode_On);
			msleep(250);

			S5K4ECGX_WRITE_LIST(s5k4ecgx_Capture_Start);
			/*msleep(300); */
			s5k4ecgx_check_REG_TC_GP_EnableCaptureChanged();
			s5k4ecgx_status.nightcap_on = true;
			return;
		} else
			/* CASE 4 : flash auto mode & low light condition */
		{
			S5K4ECGX_WRITE_LIST(s5k4ecgx_Low_Cap_On);
			if (s5k4ecgx_status.flash_mode != EXT_CFG_FLASH_OFF) {
				/* ae/awb unlock */
				if (s5k4ecgx_status.ae_lock == EXT_CFG_AE_LOCK)
					s5k4ecgx_set_ae_lock(EXT_CFG_AE_UNLOCK);
				if (s5k4ecgx_status.awb_lock ==
				    EXT_CFG_AWB_LOCK)
					s5k4ecgx_set_ae_lock
					    (EXT_CFG_AWB_UNLOCK);

				/* use torch mode on below condition */
				if (s5k4ecgx_status.camera_mode ==
				    EXT_CFG_CAM_MODE_FACTORY_TEST
				    || s5k4ecgx_status.afmode ==
				    EXT_CFG_AF_SET_MACRO) {
					CAMDRV_DEBUG("%s FLASH_6 **\n",
						     __func__);
					s5k4ecgx_set_flash(MACRO_FLASH);
				} else {
					CAMDRV_DEBUG("%s FLASH_7 **\n",
						     __func__);
					s5k4ecgx_set_flash(FULL_FLASH);
				}
			}
			/*if(s5k4ecgx_status.flash_exifinfo ==
			   true)msleep(200); */
			else
				msleep(120);

			S5K4ECGX_WRITE_LIST(s5k4ecgx_Capture_Start);
			s5k4ecgx_check_REG_TC_GP_EnableCaptureChanged();
			s5k4ecgx_status.lowcap_on = true;
			return;
		}
	}
}

static int s5k4ecgx_sensor_ext_config(void __user *arg)
{
	ioctl_pcam_info_8bit ctrl_info;
	if (copy_from_user
	    ((void *)&ctrl_info, (const void *)arg, sizeof(ctrl_info))) {
		CAMDRV_DEBUG("%s fail copy_from_user!\n", __func__);
	}
	/*
	   CAMDRV_DEBUG("TEST %d %d %d %d %d\n",
	   ctrl_info.mode, ctrl_info.address,\
	   ctrl_info.value_1, ctrl_info.value_2, ctrl_info.value_3);
	 */

	switch (ctrl_info.mode) {
	case EXT_CFG_GET_INFO:{
			unsigned short lsb, msb, a_gain, d_gain;
			s5k4ecgx_sensor_write(0xFCFC, 0xD000);
			s5k4ecgx_sensor_write(0x002C, 0x7000);
			s5k4ecgx_sensor_write(0x002E, 0x2BC0);
			/*8 */
			s5k4ecgx_sensor_read(0x0F12, (unsigned short *)&lsb);
			s5k4ecgx_sensor_read(0x0F12, (unsigned short *)&msb);
			 /*A*/
			    s5k4ecgx_sensor_read(0x0F12,
						 (unsigned short *)&a_gain);
			 /*C*/ s5k4ecgx_sensor_read(0x0F12,
						    (unsigned short *)&d_gain);
			 /*E*/ ctrl_info.value_1 = lsb;
			ctrl_info.value_2 = msb;
			ctrl_info.value_3 = a_gain;
			ctrl_info.address = d_gain;
			CAMDRV_DEBUG("exposure %x %x\n", lsb, msb);
			/*CAMDRV_DEBUG("rough_iso %x\n", rough_iso); */
		}
		break;
	case EXT_CFG_GET_FLASH_INFO:	/*yjh_flash */
		if (s5k4ecgx_status.flash_mode != EXT_CFG_FLASH_OFF
		    && s5k4ecgx_status.flash_status != FLASH_OFF
		    && s5k4ecgx_status.flash_status != PRE_FLASH_OFF) {
			s5k4ecgx_set_flash(FLASH_OFF);
		}
		CAMDRV_DEBUG("FLASH OFF_1\n");
/*CAMDRV_DEBUG("EXT_CFG_FLASH_INFO %d\n",
s5k4ecgx_status.flash_exifinfo);
ctrl_info.value_1 = s5k4ecgx_status.flash_exifinfo; */
		S5K4ECGX_WRITE_LIST(s5k4ecgx_Preview_Return);
		break;
	case EXT_CFG_FRAME_CONTROL:
		s5k4ecgx_status.fps = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_FPS;
		else
			s5k4ecgx_set_fps(s5k4ecgx_status.fps);
		break;
	case EXT_CFG_AF_CONTROL:
		if (ctrl_info.value_1 == 2 || ctrl_info.value_1 == 3)
			s5k4ecgx_status.afmode = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_AFMODE;
		else
			ctrl_info.value_3 = s5k4ecgx_set_af(ctrl_info.value_1);
		break;
	case EXT_CFG_EFFECT_CONTROL:
		s5k4ecgx_status.effect = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_EFFECT;
		else
			s5k4ecgx_set_effect(s5k4ecgx_status.effect);
		break;
	case EXT_CFG_WB_CONTROL:
		s5k4ecgx_status.whiteBalance = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_WB;
		else
			s5k4ecgx_set_whitebalance(s5k4ecgx_status.whiteBalance);
		break;
	case EXT_CFG_BR_CONTROL:
		s5k4ecgx_status.brightness = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_BRIGHTNESS;
		else
			s5k4ecgx_set_brightness(s5k4ecgx_status.brightness);
		break;
	case EXT_CFG_ISO_CONTROL:
		s5k4ecgx_status.iso = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_ISO;
		else
			s5k4ecgx_set_iso(s5k4ecgx_status.iso);
		break;
	case EXT_CFG_METERING_CONTROL:
		s5k4ecgx_status.auto_exposure = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_AE;
		else
			s5k4ecgx_set_metering(s5k4ecgx_status.auto_exposure);
		break;
	case EXT_CFG_SCENE_CONTROL:
		s5k4ecgx_status.scene = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_SCENE;
		else
			s5k4ecgx_set_scene(s5k4ecgx_status.scene);
		break;
	case EXT_CFG_AE_AWB_CONTROL:
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_AE_AWB_LOCK;
		else
			s5k4ecgx_set_ae_lock(ctrl_info.value_1);
		break;
	case EXT_CFG_CR_CONTROL:
		s5k4ecgx_status.contrast = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_CONTRAST;
		else
			s5k4ecgx_set_contrast(s5k4ecgx_status.contrast);
		break;
	case EXT_CFG_SA_CONTROL:
		s5k4ecgx_status.saturation = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_SATURATION;
		else
			s5k4ecgx_set_saturation(s5k4ecgx_status.saturation);
		break;
	case EXT_CFG_SP_CONTROL:
		s5k4ecgx_status.sharpness = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_SHARPNESS;
		else
			s5k4ecgx_set_sharpness(s5k4ecgx_status.sharpness);
		break;
	case EXT_CFG_CPU_CONTROL:
		switch (ctrl_info.value_1) {
		case EXT_CFG_CPU_CONSERVATIVE:
			CAMDRV_DEBUG("now conservative\n");
			/*cpufreq_direct_set_policy(0, "conservative"); */
			break;
		case EXT_CFG_CPU_ONDEMAND:
			CAMDRV_DEBUG("now ondemand\n");
			/*cpufreq_direct_set_policy(0, "ondemand"); */
			break;
		case EXT_CFG_CPU_PERFORMANCE:
			CAMDRV_DEBUG("now performance\n");
			/* cpufreq_direct_set_policy(0, "performance"); */
			break;
		default:
			pr_info
			    ("[S5K4ECGX] Unexpected CPU control on EXT_CFG\n");
			break;
		}
		break;
	case PCAM_SENSOR_RESET:
		pr_info
		    ("[S5K4ECGX] ************************CAM frame timeout\n");
		s5k4ecgx_status.camera_initailized = false;

		/*initailize power control pin */
		gpio_set_value_cansleep(0, 0);
		 /*RESET*/ gpio_set_value_cansleep(1, 0);
		 /*STBY*/ cam_pw(0);

		cam_pw(1);
		/* Input MCLK = 24MHz */
		msm_camio_clk_rate_set(24000000);
		gpio_tlmm_config(GPIO_CFG
				 (15, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN,
				  GPIO_CFG_16MA), GPIO_CFG_ENABLE);
		usleep(5000);
		gpio_set_value_cansleep(1, 1);
		 /*STBY*/ usleep(1000);
		gpio_set_value_cansleep(0, 1);
		 /*RESET*/ s5k4ecgx_sensor_init_probe();
		s5k4ecgx_set_preview();
		break;
	case EXT_CFG_DTP_CONTROL:
		s5k4ecgx_status.dtp = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_DTP;
		else
			s5k4ecgx_set_DTP(s5k4ecgx_status.dtp);
		if (ctrl_info.value_1 == 0)
			ctrl_info.value_3 = 2;
		else if (ctrl_info.value_1 == 1)
			ctrl_info.value_3 = 3;
		break;
	case EXT_CFG_SET_PICTURE_SIZE:
		s5k4ecgx_status.snapshot_size = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |=
			    CHECK_SNAPSHOT_SIZE;
		else
			s5k4ecgx_set_capture_size
			    (s5k4ecgx_status.snapshot_size);
		break;
		/*case PCAM_SET_CAPTURE_MODE:
		   s5k4ecgx_set_capture();
		   break; */
	case EXT_CFG_SET_FLASH_MODE:	/*yjh_flash */
		if (ctrl_info.value_1 != EXT_CFG_FLASH_TURN_ON
		    && ctrl_info.value_1 != EXT_CFG_FLASH_TURN_OFF) {
			pr_info("value_1 = %d", ctrl_info.value_1);
			s5k4ecgx_status.flash_mode = ctrl_info.value_1;
			if (s5k4ecgx_status.scene == EXT_CFG_SCENE_BACKLIGHT
			    && s5k4ecgx_status.flash_mode != EXT_CFG_FLASH_OFF)
				s5k4ecgx_set_metering(EXT_CFG_METERING_CENTER);
			else if (s5k4ecgx_status.scene ==
				 EXT_CFG_SCENE_BACKLIGHT
				 && s5k4ecgx_status.flash_mode ==
				 EXT_CFG_FLASH_OFF) {
				if (s5k4ecgx_status.auto_exposure ==
				    EXT_CFG_METERING_CENTER)
					s5k4ecgx_set_metering
					    (EXT_CFG_METERING_SPOT);
			}
		} else if (ctrl_info.value_1 == EXT_CFG_FLASH_TURN_ON) {
			pr_info("2ctrl_info.value_1 = %d\n", ctrl_info.value_1);
			if (s5k4ecgx_status.flash_mode == EXT_CFG_FLASH_AUTO) {
				s5k4ecgx_get_lux(&s5k4ecgx_status.current_lux);
				if (s5k4ecgx_status.current_lux < 0x0032) {
					s5k4ecgx_set_flash(MOVIE_FLASH);
					flashOnFromApps = 1;
					}
			} else {
				s5k4ecgx_set_flash(MOVIE_FLASH);
				flashOnFromApps = 1;
			}
		} else if (ctrl_info.value_1 == EXT_CFG_FLASH_TURN_OFF) {
			s5k4ecgx_set_flash(FLASH_OFF);
			flashOnFromApps = 0;
		}
		if (ctrl_info.value_1 == EXT_CFG_FLASH_OFF) {
			s5k4ecgx_set_flash(FLASH_OFF);
			flashOnFromApps = 0;
		}

		break;
	case EXT_CFG_SET_JPEG_QUALITY:
		s5k4ecgx_status.jpeg_quality = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_JPEGQUALITY;
		else
			s5k4ecgx_set_jpeg_quality(ctrl_info.value_1);
		break;
	case EXT_CFG_SET_AUTO_CONTRAST:
		s5k4ecgx_status.auto_contrast = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |=
			    CHECK_AUTOCONTRAST;
		else
			s5k4ecgx_set_auto_contrast(ctrl_info.value_1);
		break;
	case EXT_CFG_SET_ZOOM:
		s5k4ecgx_status.zoom = ctrl_info.value_1;
		if (!s5k4ecgx_status.camera_initailized)
			s5k4ecgx_status.need_configuration |= CHECK_ZOOM;
		else
			s5k4ecgx_set_zoom(ctrl_info.value_1);
		break;
	case EXT_CFG_SET_CAM_MODE:
		s5k4ecgx_status.camera_mode = ctrl_info.value_1;
		CAMDRV_DEBUG("CAMERA MODE : %d\n", s5k4ecgx_status.camera_mode);
		break;
	default:
		/*pr_info
		    ("[S5K4ECGX]Unexpected mode on sensor_rough_control : %d\n",
		     ctrl_info.mode);*/
		break;
	}

	if (copy_to_user
	    ((void *)arg, (const void *)&ctrl_info, sizeof(ctrl_info))) {
		pr_info("[S5K4ECGX]%s fail on copy_to_user!\n", __func__);
	}
}

void cam_pw(int status)
{

	unsigned int mclk_cfg;
	struct vreg *vreg_ldo6, *vreg_ldo15, *vreg_ldo17;

	if (status == 1) {

		vreg_ldo17 = vreg_get(NULL, "vcama");
		if (!vreg_ldo17) {
			/*pr_info("[s5k54ecgx]%s: VREG L17 get failed\n",
			__func__);*/
			}
		if (vreg_set_level(vreg_ldo17, 2800)) {
			/*pr_info("[s5k54ecgx]%s: vreg_set_level failed\n",
			__func__);*/
			}
		if (vreg_enable(vreg_ldo17)) {
			/*pr_info("![s5k54ecgx]%s: reg_enable failed\n",
			__func__);*/
			}
		usleep(10);

	/* CAM_C_EN 1.2V */
	gpio_set_value_cansleep(CAM_C_EN, 1);	/* cam_c_en */
	usleep(10);

	vreg_ldo15 = vreg_get(NULL, "vcamio");
	if (!vreg_ldo15) {
		/*pr_info("[s5k54ecgx]%s: VREG L15 get failed\n",
			__func__);*/
	}
	if (vreg_set_level(vreg_ldo15, 1800)) {
		/*pr_info("[s5k54ecgx]%s: vreg_set_level failed\n",
			__func__);*/
	}
	if (vreg_enable(vreg_ldo15)) {
		/*pr_info("![s5k54ecgx]%s: reg_enable failed\n",
			__func__);*/
	}
	usleep(250);

	/*msm_camio_clk_enable(CAMIO_CAM_MCLK_CLK); */
	gpio_tlmm_config(GPIO_CFG
			 (15, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL,
			  GPIO_CFG_8MA), GPIO_CFG_ENABLE);
	/*msm_camio_clk_rate_set(24000000);*/

	gpio_set_value_cansleep(CAM_5M_STBY, 1);	/* 5M_STBY */

	usleep(20);

	gpio_set_value_cansleep(CAM_5M_RST, 1); /* 5M_RST */


	/*gpio_set_value(79, 1); // CAM_AF_2V8 */
	gpio_set_value_cansleep(CAM_AF_EN, 1);	/*CAM_AF_2V8 */
	usleep(1000);

	} else {
		/* power off */
		/*pr_info("OFF\n");*/
		gpio_set_value_cansleep(CAM_5M_RST, 0);

		/*msm_camio_clk_disable(CAMIO_CAM_MCLK_CLK); */
		gpio_tlmm_config(GPIO_CFG
			 (15, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL,
			  GPIO_CFG_8MA), GPIO_CFG_ENABLE);

		gpio_set_value_cansleep(CAM_5M_STBY, 0);
		udelay(10);

		vreg_disable(vreg_get(NULL, "vcamio"));
		udelay(10);
		gpio_set_value_cansleep(CAM_C_EN, 0);
		udelay(10);
		vreg_disable(vreg_get(NULL, "vcama"));

		usleep(1000);

		gpio_set_value_cansleep(CAM_AF_EN, 0);	/*AM_AF_2V8 */
		udelay(10);

		if (!torch_mode_on) {
			gpio_set_value_cansleep(FLASH_EN, 0);	/*lash off */
			gpio_set_value_cansleep(FLASH_MODE, 0);
			}

/*pio_tlmm_config(GPIO_CFG(CAM_GPIO_SCL, 0, GPIO_CFG_OUTPUT,
GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
gpio_tlmm_config(GPIO_CFG(CAM_GPIO_SDA, 0, GPIO_CFG_OUTPUT,
GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
gpio_set_value(CAM_GPIO_SCL, 0);
gpio_set_value(CAM_GPIO_SDA, 0);*/
	}

}

static int cam_hw_init()
{
	int rc = 0;
	/*pr_info("[S5K4ECGX]cam_hw_init()\n");*/

	s5k4ecgx_status.power_on = true;
	s5k4ecgx_status.id = 0x11;

	return rc;
}

static long s5k4ecgx_config_effect(int mode, int effect)
{
	return 0;
/*
	long rc = 0;
	switch (mode) {
		case SENSOR_PREVIEW_MODE:
			CAMDRV_DEBUG("SENSOR_PREVIEW_MODE\n");
		break;
		case SENSOR_SNAPSHOT_MODE:
			CAMDRV_DEBUG("SENSOR_SNAPSHOT_MODE\n");
		break;
		default:
			CAMDRV_DEBUG("[PGH] %s default\n", __func__);
		break;
	}

	switch (effect) {
		case CAMERA_EFFECT_OFF:
			CAMDRV_DEBUG("CAMERA_EFFECT_OFF\n");
		break;
		case CAMERA_EFFECT_MONO:
			CAMDRV_DEBUG("CAMERA_EFFECT_MONO\n");
		break;
		case CAMERA_EFFECT_NEGATIVE:
			CAMDRV_DEBUG("CAMERA_EFFECT_NEGATIVE\n");
		break;
		case CAMERA_EFFECT_SOLARIZE:
			CAMDRV_DEBUG("CAMERA_EFFECT_SOLARIZE\n");
		break;
		case CAMERA_EFFECT_SEPIA:
			CAMDRV_DEBUG("CAMERA_EFFECT_SEPIA\n");
		break;
		default:
			pr_info("[S5K4ECGX]unexpeceted effect
			%s/%d\n", __func__, __LINE__);
		return -EINVAL;
	}
	s5k4ecgx_effect = effect;

	return rc;
	*/
}

static long s5k4ecgx_set_sensor_mode(int mode)
{
	CAMDRV_DEBUG("s5k4ecgx_set_sensor_mode : %d\n", mode);
	switch (mode) {
	case SENSOR_PREVIEW_MODE:
		s5k4ecgx_set_preview();
		break;
	case SENSOR_SNAPSHOT_MODE:
	case SENSOR_RAW_SNAPSHOT_MODE:
		if (s5k4ecgx_status.camera_status != SNAPSHOT)
			s5k4ecgx_set_capture();
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int s5k4ecgx_sensor_init_probe()
{
	int rc = 0;
	/*pr_info("[S5K4ECGX]sensor_init_probe()\n");*/

#ifndef CONFIG_LOAD_FILE
	rc = s5k4ecgx_sensor_burst_write(s5k4ecgx_init_reg1_EVT1,
					 (sizeof(s5k4ecgx_init_reg1_EVT1) /
					  sizeof(s5k4ecgx_init_reg1_EVT1[0])),
					 "s5k4ecgx_init_reg1_EVT1");
/*rc = s5k4ecgx_sensor_write(s5k4ecgx_init_reg1_EVT1,
(sizeof(s5k4ecgx_init_reg1_EVT1) / sizeof(s5k4ecgx_init_reg1_EVT1[0])),
"s5k4ecgx_init_reg1_EVT1");
CAMDRV_DEBUG("hoon_tunning2\n");*/
	usleep(10000);
#endif

	return rc;
}


int s5k4ecgx_sensor_init(const struct msm_camera_sensor_info *data)
{
	int rc = 0;

	s5k4ecgx_ctrl = kzalloc(sizeof(struct s5k4ecgx_ctrl), GFP_KERNEL);
	if (!s5k4ecgx_ctrl) {
		CDBG("s5k4ecgx_init failed!\n");
		rc = -ENOMEM;
		goto init_done;
	}

	if (data)
		s5k4ecgx_ctrl->sensordata = data;

	cam_pw(1);

	rc = cam_hw_init();
	if (rc < 0) {
		/*pr_info("[S5K4ECGX]cam_fw_init failed!\n");*/
		cam_pw(0);
		/*sleep current issue for sensor disconnection */
		goto init_fail;
	}
#ifdef CONFIG_LOAD_FILE
	s5k4ecgx_regs_table_init();
#endif

	rc = s5k4ecgx_sensor_init_probe();
	if (rc < 0) {
		CDBG("s5k4ecgx_sensor_init failed!\n");
		goto init_fail;
	}

init_done:
	return rc;

init_fail:
	kfree(s5k4ecgx_ctrl);
	return rc;
}

static int s5k4ecgx_init_client(struct i2c_client *client)
{
	/* Initialize the MSM_CAMI2C Chip */
	init_waitqueue_head(&s5k4ecgx_wait_queue);
	return 0;
}

/*int s5k4ecgx_sensor_ext_config(void __user *argp)
{
      printk("s5k4ecgx_sensor_ext_config\n");
      return 0;
}*/
int s5k4ecgx_sensor_config(void __user *argp)
{
	struct sensor_cfg_data cfg_data;
	long rc = 0;

	if (copy_from_user(&cfg_data,
			   (void *)argp, sizeof(struct sensor_cfg_data)))
		return -EFAULT;

	/* down(&s5k4ecgx_sem);
	   CDBG("s5k4ecgx_ioctl, cfgtype = %d, mode = %d\n", */
	CAMDRV_DEBUG("s5k4ecgx_ioctl, cfgtype = %d, mode = %d\n",
		     cfg_data.cfgtype, cfg_data.mode);
	switch (cfg_data.cfgtype) {
	case CFG_SET_MODE:
		rc = s5k4ecgx_set_sensor_mode(cfg_data.mode);
		break;
	case CFG_SET_EFFECT:
		rc = s5k4ecgx_config_effect(cfg_data.mode, cfg_data.cfg.effect);
		break;
	case CFG_GET_AF_MAX_STEPS:
	default:
		rc = -EINVAL;
		break;
	}
	/* up(&s5k4ecgx_sem); */

	return rc;
}

int s5k4ecgx_sensor_release(void)
{
	int rc = 0;

	/* down(&s5k4ecgx_sem); */
/*
	CAMDRV_DEBUG("lens moving to Base before CAM OFF\n");
	s5k4ecgx_sensor_write(0x0028, 0x7000);
	s5k4ecgx_sensor_write(0x002A, 0x0254);
	s5k4ecgx_sensor_write(0x0F12, 0x0030); //Lens Pistion (0x00 ~ 0xfF)
	normally (0x30 ~ 0x80)
	*/

	s5k4ecgx_status.camera_initailized = false;
	s5k4ecgx_status.need_configuration = 0;
	s5k4ecgx_status.effect = 0;
	s5k4ecgx_status.brightness = 0;
	s5k4ecgx_status.contrast = 0;
	s5k4ecgx_status.saturation = 0;
	s5k4ecgx_status.sharpness = 0;
	s5k4ecgx_status.whiteBalance = 0;
	s5k4ecgx_status.iso = 0;
	s5k4ecgx_status.auto_exposure = 0;
	s5k4ecgx_status.scene = 0;
	s5k4ecgx_status.afmode = EXT_CFG_AF_SET_NORMAL;
	s5k4ecgx_status.afcanceled = true;
	s5k4ecgx_status.dtp = 0;

	CAMDRV_DEBUG("s5k4ecgx_sensor_release\n");
	kfree(s5k4ecgx_ctrl);
	/* up(&s5k4ecgx_sem); */

#ifdef CONFIG_LOAD_FILE
	s5k4ecgx_regs_table_exit();
#endif

	cam_pw(0);		/* off camera LDOs */
	return rc;
}

static int s5k4ecgx_i2c_probe(struct i2c_client *client,
			      const struct i2c_device_id *id)
{
	int rc = 0;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		rc = -ENOTSUPP;
		goto probe_failure;
	}

	s5k4ecgx_sensorw = kzalloc(sizeof(struct s5k4ecgx_work), GFP_KERNEL);

	if (!s5k4ecgx_sensorw) {
		rc = -ENOMEM;
		goto probe_failure;
	}

	i2c_set_clientdata(client, s5k4ecgx_sensorw);
	s5k4ecgx_init_client(client);
	s5k4ecgx_client = client;

#ifdef FACTORY_TEST
	camera_class = class_create(THIS_MODULE, "camera");
	if (IS_ERR(camera_class))
		pr_err("Failed to create class(camera)!\n");
		s5k4ecgx_dev =
			device_create(camera_class, NULL, 0, NULL, "rear");
	if (IS_ERR(s5k4ecgx_dev)) {
		pr_err("Failed to create device!");
		goto probe_failure;
		}
	if (device_create_file(s5k4ecgx_dev, &dev_attr_rear_flash) < 0) {
		CDBG("failed to create device file, %s\n",
		dev_attr_rear_flash.attr.name);
		}
#endif

	/*pr_info("[S5K4ECGX]s5k4ecgx_probe succeeded!\n");*/

	return 0;

probe_failure:
	kfree(s5k4ecgx_sensorw);
	s5k4ecgx_sensorw = NULL;
	/*pr_info("[S5K4ECGX]s5k4ecgx_probe failed!\n");*/
	return rc;
}

static const struct i2c_device_id s5k4ecgx_i2c_id[] = {
	{"s5k4ecgx", 0},
	{},
};

static struct i2c_driver s5k4ecgx_i2c_driver = {
	.id_table = s5k4ecgx_i2c_id,
	.probe = s5k4ecgx_i2c_probe,
	.remove = __exit_p(s5k4ecgx_i2c_remove),
	.driver = {
		   .name = "s5k4ecgx",
		   },
};

static int s5k4ecgx_sensor_probe(const struct msm_camera_sensor_info *info,
				 struct msm_sensor_ctrl *s)
{

	int rc = i2c_add_driver(&s5k4ecgx_i2c_driver);
	if (rc < 0) {		/* || s5k4ecgx_client == NULL) { */
		/*pr_info("[S5K4ECGX]s5k4ecgx_sensor_probe fail\n");*/
		rc = -ENOTSUPP;
		goto probe_done;
	}

	/*initailize states */
	s5k4ecgx_status.camera_initailized = false;
	s5k4ecgx_status.need_configuration = 0;
	s5k4ecgx_status.camera_mode = EXT_CFG_CAM_MODE_CAMERA;
	s5k4ecgx_status.effect = EXT_CFG_EFFECT_NORMAL;
	s5k4ecgx_status.brightness = EXT_CFG_BR_STEP_0;
	s5k4ecgx_status.contrast = EXT_CFG_CR_STEP_0;
	s5k4ecgx_status.saturation = EXT_CFG_SA_STEP_0;
	s5k4ecgx_status.sharpness = EXT_CFG_SP_STEP_0;
	s5k4ecgx_status.whiteBalance = EXT_CFG_WB_AUTO;
	s5k4ecgx_status.iso = EXT_CFG_ISO_AUTO;
	s5k4ecgx_status.auto_exposure = EXT_CFG_METERING_NORMAL;
	s5k4ecgx_status.scene = EXT_CFG_SCENE_OFF;
	s5k4ecgx_status.afmode = EXT_CFG_AF_SET_NORMAL;
	s5k4ecgx_status.afcanceled = true;
	s5k4ecgx_status.dtp = EXT_CFG_DTP_OFF;
	s5k4ecgx_status.snapshot_size = EXT_CFG_SNAPSHOT_SIZE_2560x1920_5M;
	s5k4ecgx_status.preview_size = EXT_CFG_PREVIEW_SIZE_640x480_VGA;
	s5k4ecgx_status.flash_mode = EXT_CFG_FLASH_OFF;
	s5k4ecgx_status.flash_status = FLASH_OFF;
	s5k4ecgx_status.zoom = EXT_CFG_ZOOM_STEP_0;
	s5k4ecgx_status.current_lux = 0;
	s5k4ecgx_status.jpeg_quality = EXT_CFG_JPEG_QUALITY_SUPERFINE;
	s5k4ecgx_status.auto_contrast = EXT_CFG_AUTO_CONTRAST_OFF;

	/*cam_pw(1);
	usleep(10*1000);
	rc = s5k4ecgx_sensor_init_probe();*/

	s->s_init = s5k4ecgx_sensor_init;
	s->s_release = s5k4ecgx_sensor_release;
	s->s_config = s5k4ecgx_sensor_config;
	s->s_ext_config = s5k4ecgx_sensor_ext_config;
	s->s_camera_type = BACK_CAMERA_2D;
	s->s_mount_angle = 90;

	cam_pw(0);
probe_done:

	/*cam_pw(0);*/

	CDBG("%s %s:%d\n", __FILE__, __func__, __LINE__);
	return rc;
}

static int __s5k4ecgx_probe(struct platform_device *pdev)
{
	/*pr_info("======== S5K54ECGX probe ==========\n");*/
#if defined(CONFIG_SAMSUNG_LTN_COMMON)
/*latin_cam:jyc support antibanding for Latin*/
	int rv;
	rv = device_create_file(&pdev->dev, &s5k4ecgx_antibanding_attr);
	if (rv)
		/*pr_info("<=EXT_CFG=> s5k4ecgx_probe() :
		    device_create_file() is failed with %d", rv);*/
#endif

	return msm_camera_drv_start(pdev, s5k4ecgx_sensor_probe);
}

static struct platform_driver msm_camera_driver = {
	.probe = __s5k4ecgx_probe,
	.driver = {
		   .name = "msm_camera_s5k4ecgx",
		   .owner = THIS_MODULE,
		   },
};

static int __init s5k4ecgx_init(void)
{
	return platform_driver_register(&msm_camera_driver);
}

module_init(s5k4ecgx_init);

