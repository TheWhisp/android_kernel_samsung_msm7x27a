/* Copyright (c) 2011, Code GEIM Forum. All rights reserved.
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

/* 1/5" sr300pc20*/

#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <media/msm_camera.h>
#include <mach/gpio.h>
#include <mach/pmic.h>

#if defined(CONFIG_MACH_JENA_TELSTRA)
#if (CONFIG_MACH_JENA_HWREV == 0x0) || (CONFIG_MACH_JENA_HWREV == 0x1) || (CONFIG_MACH_JENA_HWREV == 0x2)
#include "sr300pc20_S6500T.h"
#else
#include "sr300pc20_jena.h"
#endif
#elif defined(CONFIG_MACH_JENA_AUSVHA)
#include "sr300pc20_jena.h"
#elif defined(CONFIG_MACH_JENA)
#include "sr300pc20_jena.h"
#else
#include "sr300pc20.h"
#endif

#include <mach/camera.h>
#include <mach/vreg.h>
#include <linux/io.h>

#include "sec_cam_pmic.h"

/*#define SENSOR_DEBUG 0*/

/*#define CONFIG_LOAD_FILE*/

#define PCAM_ENABLE_DEBUG

#ifdef PCAM_ENABLE_DEBUG
#define CAMDRV_DEBUG(fmt, arg...)\
	do {					\
		pr_info("[SR300PC20_DEBUG] %s:%d:" fmt "\n",	\
			__func__, __LINE__, ##arg);		\
	}							\
	while (0)
#else
#define CAMDRV_DEBUG(fmt, arg...)
#endif

#ifdef CONFIG_LOAD_FILE
#define SR300PC20_WRITE_LIST(A)\
	sr300pc20_sensor_write_list(A, (sizeof(A) / sizeof(A[0])), #A);
#define SR300PC20_WRITE_LIST_BURST(A)\
	sr300pc20_sensor_burst_write_list(A, (sizeof(A) / sizeof(A[0])), #A);
#else
#define SR300PC20_WRITE_LIST(A)\
	sr300pc20_sensor_write_list(A, (sizeof(A) / sizeof(A[0])), #A);
#define SR300PC20_WRITE_LIST_BURST(A)\
	sr300pc20_sensor_burst_write_list(A, (sizeof(A) / sizeof(A[0])), #A);
#endif

#define UPTO_MSEC 1000

static char first_start_camera = 1;	/*  1 is not init a sensor */
static char set_init0;

static char mEffect = EXT_CFG_EFFECT_NORMAL;
static char mBrightness = EXT_CFG_BR_STEP_0;
static char mContrast = EXT_CFG_CR_STEP_0;
static char mSaturation = EXT_CFG_SA_STEP_0;
static char mSharpness = EXT_CFG_SP_STEP_0;
static char mWhiteBalance = EXT_CFG_WB_AUTO;
static char mISO = EXT_CFG_ISO_AUTO;
static char mAutoExposure = EXT_CFG_METERING_NORMAL;
static char mScene = EXT_CFG_SCENE_OFF;
static char mCameraMode = EXT_CFG_CAMERA_MODE;
static char mDTP;
static char mInit;

struct sr300pc20_work {
	struct work_struct work;
};

static struct sr300pc20_work *sr300pc20_sensorw;
static struct i2c_client *sr300pc20_client;

struct sr300pc20_ctrl {
	const struct msm_camera_sensor_info *sensordata;
};

static unsigned int config_csi2;
static struct sr300pc20_ctrl *sr300pc20_ctrl;
static void sr300pc20_set_power(int status);
static DECLARE_WAIT_QUEUE_HEAD(sr300pc20_wait_queue);
DECLARE_MUTEX(sr300pc20_sem);

static void sr300pc20_reset_power(void);
static int sr300pc20_set_preview(void);

#ifdef CONFIG_LOAD_FILE
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/mm.h>

static char *sr300pc20_regs_table;

static int sr300pc20_regs_table_size;

static int sr300pc20_regs_table_write(char *name);
static int sr300pc20_regs_table_burst_write(char *name);

#define MAX_SETTING_NAME 30
#define TUNNING_FILE_PATH "/mnt/sdcard/sr300pc20.h"

#endif

#define BURST_MODE_BUFFER_MAX_SIZE 255
#define BURST_REG 0x0e
#define DELAY_REG 0xff
unsigned char sr300pc20_buf_for_burstmode[BURST_MODE_BUFFER_MAX_SIZE];

static int sr300pc20_sensor_read(unsigned char subaddr, unsigned char *data)
{
	int ret = 0;
	unsigned char buf[1] = { 0 };
	struct i2c_msg msg = { sr300pc20_client->addr, 0, 1, buf };

#if defined(SENSOR_DEBUG)
	pr_err("[ 0x%x ]\n", subaddr);
#endif

	buf[0] = (subaddr & 0xFF);

	ret = i2c_transfer(sr300pc20_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
	if (ret == -EIO) {
		pr_err("[%s : %d] i2c_transfer fail\n", __func__, __LINE__);
		goto error;
	}

	msg.flags = I2C_M_RD;

	ret = i2c_transfer(sr300pc20_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
	if (ret == -EIO) {
		pr_err("[%s : %d] i2c_transfer fail\n", __func__, __LINE__);
		goto error;
	}

	*data = buf[0];		/*check */

error:
	return ret;
}

static int sr300pc20_sensor_write(unsigned char subaddr, unsigned char val)
{
	unsigned char buf[2] = { 0 };
	struct i2c_msg msg = { sr300pc20_client->addr, 0, 2, buf };

#if defined(SENSOR_DEBUG)
	pr_err("[ 0x%x %x ]\n", subaddr, data);
#endif

	buf[0] = (subaddr);
	buf[1] = (val);

	return i2c_transfer(sr300pc20_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
}

/*static int sr300pc20_sensor_write_list
(struct samsung_short_t *list,int size, char *name)*/
static int sr300pc20_sensor_write_list(const u16 *list, int size, char *name)
{
	int ret = 0;
	unsigned char subaddr = 0;
	unsigned char value = 0;
	int i = 0;
	CAMDRV_DEBUG("%s\n", name);

#ifdef CONFIG_LOAD_FILE
	ret = sr300pc20_regs_table_write(name);
#else

	for (i = 0; i < size; i++) {
		subaddr = (list[i] >> 8);	/*address */
		value = (list[i] & 0xFF);	/*value */

		if (subaddr == DELAY_REG) {
			msleep(value * 10);	/*one step is 10ms */
			CAMDRV_DEBUG("[sr300pc20] msleep %d msec\n",
				     value * 10);
		} else {
			if (sr300pc20_sensor_write(subaddr, value) < 0) {
				pr_err
				    ("[%s : %d] sr300pc20_sensor_write fail\n",
				     __func__, __LINE__);
				return -EIO;
			}
		}
	}
#endif
	return ret;
}

static int sr300pc20_sensor_burst_write_list(const u16 *list, int size,
					     char *name)
{
	int err = -EINVAL;
	int i = 0;
	int idx = 0;
	unsigned short subaddr = 0;
	unsigned short value = 0;
	int burst_flag = 0;
	int burst_cnt = 0;
	struct i2c_msg msg = { sr300pc20_client->addr,
		0, 0, sr300pc20_buf_for_burstmode
	};

	CAMDRV_DEBUG("%s, size = %d\n", name, size);

#ifdef CONFIG_LOAD_FILE
	err = sr300pc20_regs_table_burst_write(name);
#else
	for (i = 0; i < size; i++) {
		if (idx > (BURST_MODE_BUFFER_MAX_SIZE - 10)) {
			pr_err("[%s:%d]Burst mode buffer overflow! "
			       "Burst Count %d\n",
			       __func__, __LINE__, burst_cnt);
			pr_err("[%s:%d]count %d, addr %x "
			       "value %x\n", __func__, __LINE__, i,
			       (list[i] >> 8) & 0xff, list[i] & 0xFF);
			pr_err("[%s:%d]addr %x value %x\n",
			       __func__, __LINE__,
			       (list[i - 1] >> 8) & 0xff, list[i - 1] & 0xFF);
			pr_err("[%s:%d]addr %x value %x\n",
			       __func__, __LINE__,
			       (list[i - 2] >> 8) & 0xff, list[i - 2] & 0xFF);
			err = -EIO;
			return err;
		}
		subaddr = (list[i] >> 8);
		value = (list[i] & 0xFF);
		if (burst_flag == 0) {
			switch (subaddr) {
			case BURST_REG:
				if (value != 0x00) {
					burst_flag = 1;
					burst_cnt++;
				}
				break;
			case DELAY_REG:
				msleep(value * 10);	/* a step is 10ms */
				break;
			default:
				idx = 0;
				err = sr300pc20_sensor_write(subaddr, value);
				break;
			}
		} else if (burst_flag == 1) {
			if (subaddr == BURST_REG && value == 0x00) {
				msg.len = idx;
				CAMDRV_DEBUG("burst_cnt %d, idx %d\n",
					     burst_cnt, idx);
				err = i2c_transfer(sr300pc20_client->adapter,
						   &msg, 1);
				if (err < 0) {
					pr_err("[%s:%d]Burst write fail!\n",
					       __func__, __LINE__);
					return err;
				}
				idx = 0;
				burst_flag = 0;
			} else {
				if (idx == 0) {
					sr300pc20_buf_for_burstmode[idx++] =
					    subaddr;
					sr300pc20_buf_for_burstmode[idx++] =
					    value;
				} else {
					sr300pc20_buf_for_burstmode[idx++] =
					    value;
				}
			}
		}
	}
#endif

	if (unlikely(err < 0)) {
		pr_err("[%s:%d] register set failed\n", __func__, __LINE__);
		return err;
	}

	return err;
}

static int sr300pc20_effect_control(char value)
{
	int err = 0;

	CAMDRV_DEBUG("Enter [value = %d]\n", value);

	switch (value) {
	case EXT_CFG_EFFECT_NORMAL:{
			err = SR300PC20_WRITE_LIST(sr300pc20_effect_off);
		}
		break;

	case EXT_CFG_EFFECT_NEGATIVE:{
			err = SR300PC20_WRITE_LIST(sr300pc20_effect_negative);
		}
		break;

	case EXT_CFG_EFFECT_MONO:{
			err = SR300PC20_WRITE_LIST(sr300pc20_effect_mono);
		}
		break;

	case EXT_CFG_EFFECT_SEPIA:{
			err = SR300PC20_WRITE_LIST(sr300pc20_effect_sepia);
		}
		break;

	default:{
			pr_warning(" Unexpected Effect mode :" "%d\n", value);
		}
		break;
	}

	if (err != 0)
		pr_err("[%s : %d] %d fail\n", __func__, __LINE__, value);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static int sr300pc20_whitebalance_control(char value)
{
	int err = 0;

	CAMDRV_DEBUG("Enter [value = %d]\n", value);

	switch (value) {
	case EXT_CFG_WB_AUTO:{
			err = SR300PC20_WRITE_LIST(sr300pc20_wb_auto);
		}
		break;

	case EXT_CFG_WB_DAYLIGHT:{
			err = SR300PC20_WRITE_LIST(sr300pc20_wb_daylight);
		}
		break;

	case EXT_CFG_WB_CLOUDY:{
			err = SR300PC20_WRITE_LIST(sr300pc20_wb_cloudy);
		}
		break;

	case EXT_CFG_WB_FLUORESCENT:{
			err = SR300PC20_WRITE_LIST(sr300pc20_wb_fluorescent);
		}
		break;

	case EXT_CFG_WB_INCANDESCENT:{
			err = SR300PC20_WRITE_LIST(sr300pc20_wb_incandescent);
		}
		break;

	default:{
			pr_warning(" Unexpected WB mode : %d\n", value);
		}
		break;

	}

	if (err != 0)
		pr_err("[%s : %d] %d fail\n", __func__, __LINE__, value);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static int sr300pc20_brightness_control(char value)
{

	int err = 0;

	CAMDRV_DEBUG("Enter [value = %d]\n", value);

	switch (value) {
	case EXT_CFG_BR_STEP_P_4:{
			err = SR300PC20_WRITE_LIST(sr300pc20_brightness_p_4);
		}
		break;

	case EXT_CFG_BR_STEP_P_3:{
			err = SR300PC20_WRITE_LIST(sr300pc20_brightness_p_3);
		}
		break;

	case EXT_CFG_BR_STEP_P_2:{
			err = SR300PC20_WRITE_LIST(sr300pc20_brightness_p_2);
		}
		break;

	case EXT_CFG_BR_STEP_P_1:{
			err = SR300PC20_WRITE_LIST(sr300pc20_brightness_p_1);
		}
		break;

	case EXT_CFG_BR_STEP_0:{
			err = SR300PC20_WRITE_LIST(sr300pc20_brightness_0);
		}
		break;

	case EXT_CFG_BR_STEP_M_1:{
			err = SR300PC20_WRITE_LIST(sr300pc20_brightness_m_1);
		}
		break;

	case EXT_CFG_BR_STEP_M_2:{
			err = SR300PC20_WRITE_LIST(sr300pc20_brightness_m_2);
		}
		break;

	case EXT_CFG_BR_STEP_M_3:{
			err = SR300PC20_WRITE_LIST(sr300pc20_brightness_m_3);
		}
		break;

	case EXT_CFG_BR_STEP_M_4:{
			err = SR300PC20_WRITE_LIST(sr300pc20_brightness_m_4);
		}
		break;

	default:{
			pr_warning(" Unexpected BR mode : %d\n", value);
		}
		break;

	}

	if (err != 0)
		pr_err("[%s : %d] %d fail\n", __func__, __LINE__, value);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static int sr300pc20_iso_control(char value)
{
	int err = 0;

	CAMDRV_DEBUG("Enter [value = %d]\n", value);

	switch (value) {
	case EXT_CFG_ISO_AUTO:{
			err = SR300PC20_WRITE_LIST(sr300pc20_iso_auto);
		}
		break;

	case EXT_CFG_ISO_50:{
			err = SR300PC20_WRITE_LIST(sr300pc20_iso_50);
		}
		break;

	case EXT_CFG_ISO_100:{
			err = SR300PC20_WRITE_LIST(sr300pc20_iso_100);
		}
		break;

	case EXT_CFG_ISO_200:{
			err = SR300PC20_WRITE_LIST(sr300pc20_iso_200);
		}
		break;

	case EXT_CFG_ISO_400:{
			err = SR300PC20_WRITE_LIST(sr300pc20_iso_400);
		}
		break;

	default:{
			pr_warning(" Unexpected ISO mode : %d\n", value);
		}
		break;

	}

	if (err != 0)
		pr_err("[%s : %d] %d fail\n", __func__, __LINE__, value);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static int sr300pc20_metering_control(char value)
{
	int err = 0;

	CAMDRV_DEBUG("Enter [value = %d]\n", value);

	switch (value) {
	case EXT_CFG_METERING_NORMAL:{
			err = SR300PC20_WRITE_LIST(sr300pc20_metering_normal);
		}
		break;

	case EXT_CFG_METERING_SPOT:{
			err = SR300PC20_WRITE_LIST(sr300pc20_metering_spot);
		}
		break;

	case EXT_CFG_METERING_CENTER:{
			err = SR300PC20_WRITE_LIST(sr300pc20_metering_center);
		}
		break;

	default:{
			pr_warning(" Unexpected METERING mode : %d\n", value);
		}
		break;
	}

	if (err != 0)
		pr_err("[%s : %d] %d fail\n", __func__, __LINE__, value);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static int sr300pc20_scene_control(char value)
{
	int err = 0;

	CAMDRV_DEBUG("Enter [value = %d]\n", value);

	err = SR300PC20_WRITE_LIST(sr300pc20_scene_off);
	if (err != 0)
		pr_err("[%s : %d] sr300pc20_scene_off control fail\n",
		       __func__, __LINE__);


	if (value != EXT_CFG_SCENE_OFF) {
		err = SR300PC20_WRITE_LIST(sr300pc20_effect_off);
		if (err != 0)
			pr_err("[%s : %d] sr300pc20_effect_off control fail\n",
					__func__, __LINE__);
	}





	switch (value) {
	case EXT_CFG_SCENE_OFF:
		break;

	case EXT_CFG_SCENE_PORTRAIT:{
			err = SR300PC20_WRITE_LIST(sr300pc20_scene_portrait);
		}
		break;

	case EXT_CFG_SCENE_LANDSCAPE:{
			err = SR300PC20_WRITE_LIST(sr300pc20_scene_landscape);
		}
		break;

	case EXT_CFG_SCENE_SPORTS:{
			err = SR300PC20_WRITE_LIST(sr300pc20_scene_sports);
		}
		break;

	case EXT_CFG_SCENE_PARTY:{
			err = SR300PC20_WRITE_LIST(sr300pc20_scene_party);
		}
		break;

	case EXT_CFG_SCENE_BEACH:{
			err = SR300PC20_WRITE_LIST(sr300pc20_scene_beach);
		}
		break;

	case EXT_CFG_SCENE_SUNSET:{
			err = SR300PC20_WRITE_LIST(sr300pc20_scene_sunset);
		}
		break;

	case EXT_CFG_SCENE_DAWN:{
			err = SR300PC20_WRITE_LIST(sr300pc20_scene_dawn);
		}
		break;

	case EXT_CFG_SCENE_FALL:{
			err = SR300PC20_WRITE_LIST(sr300pc20_scene_fall);
		}
		break;

	case EXT_CFG_SCENE_NIGHTSHOT:{
			err = SR300PC20_WRITE_LIST(sr300pc20_scene_nightshot);
		}
		break;

	case EXT_CFG_SCENE_BACKLIGHT:{
			err = SR300PC20_WRITE_LIST(sr300pc20_scene_backlight);
		}
		break;

	case EXT_CFG_SCENE_FIREWORK:{
			err = SR300PC20_WRITE_LIST(sr300pc20_scene_firework);
		}
		break;

	case EXT_CFG_SCENE_TEXT:{
			err = SR300PC20_WRITE_LIST(sr300pc20_scene_text);
		}
		break;

	case EXT_CFG_SCENE_CANDLE:{
			err = SR300PC20_WRITE_LIST(sr300pc20_scene_candle);
		}
		break;

	default:{
			pr_warning(" Unexpected SCENE mode : %d\n", value);
		}
		break;
	}

	if (err != 0)
		pr_err("[%s : %d] %d fail\n", __func__, __LINE__, value);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static int sr300pc20_contrast_control(char value)
{
	int err = 0;

	CAMDRV_DEBUG("Enter [value = %d]\n", value);

	switch (value) {
	case EXT_CFG_CR_STEP_M_2:{
			err = SR300PC20_WRITE_LIST(sr300pc20_contrast_m_2);
		}
		break;

	case EXT_CFG_CR_STEP_M_1:{
			err = SR300PC20_WRITE_LIST(sr300pc20_contrast_m_1);
		}
		break;

	case EXT_CFG_CR_STEP_0:{
			err = SR300PC20_WRITE_LIST(sr300pc20_contrast_0);
		}
		break;

	case EXT_CFG_CR_STEP_P_1:{
			err = SR300PC20_WRITE_LIST(sr300pc20_contrast_p_1);
		}
		break;

	case EXT_CFG_CR_STEP_P_2:{
			err = SR300PC20_WRITE_LIST(sr300pc20_contrast_p_2);
		}
		break;

	default:{
			pr_warning
			    (" Unexpected EXT_CFG_CR_CONTROL mode :"
			     "%d\n", value);
		}
		break;
	}

	if (err != 0)
		pr_err("[%s : %d] %d fail\n", __func__, __LINE__, value);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static int sr300pc20_saturation_control(char value)
{
	int err = 0;

	CAMDRV_DEBUG("Enter [value = %d]\n", value);

	switch (value) {
	case EXT_CFG_SA_STEP_M_2:{
			err = SR300PC20_WRITE_LIST(sr300pc20_saturation_m_2);
		}
		break;

	case EXT_CFG_SA_STEP_M_1:{
			err = SR300PC20_WRITE_LIST(sr300pc20_saturation_m_1);
		}
		break;

	case EXT_CFG_SA_STEP_0:{
			err = SR300PC20_WRITE_LIST(sr300pc20_saturation_0);
		}
		break;

	case EXT_CFG_SA_STEP_P_1:{
			err = SR300PC20_WRITE_LIST(sr300pc20_saturation_p_1);
		}
		break;

	case EXT_CFG_SA_STEP_P_2:{
			err = SR300PC20_WRITE_LIST(sr300pc20_saturation_p_2);
		}
		break;

	default:{
			pr_warning
			    (" Unexpected EXT_CFG_SA_CONTROL mode :"
			     "%d\n", value);
		}
		break;
	}

	if (err != 0)
		pr_err("[%s : %d] %d fail\n", __func__, __LINE__, value);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static int sr300pc20_sharpness_control(char value)
{
	int err = 0;

	CAMDRV_DEBUG("Enter [value = %d]\n", value);

	switch (value) {
	case EXT_CFG_SP_STEP_M_2:{
			err = SR300PC20_WRITE_LIST(sr300pc20_sharpness_m_2);
		}
		break;

	case EXT_CFG_SP_STEP_M_1:{
			err = SR300PC20_WRITE_LIST(sr300pc20_sharpness_m_1);
		}
		break;

	case EXT_CFG_SP_STEP_0:{
			err = SR300PC20_WRITE_LIST(sr300pc20_sharpness_0);
		}
		break;

	case EXT_CFG_SP_STEP_P_1:{
			err = SR300PC20_WRITE_LIST(sr300pc20_sharpness_p_1);
		}
		break;

	case EXT_CFG_SP_STEP_P_2:{
			err = SR300PC20_WRITE_LIST(sr300pc20_sharpness_p_2);
		}
		break;

	default:{
			pr_warning
			    (" Unexpected EXT_CFG_SP_CONTROL mode :"
			     "%d\n", value);
		}
		break;
	}

	if (err != 0)
		pr_err("[%s : %d] %d fail\n", __func__, __LINE__, value);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static int sr300pc20_DTP_control(char value)
{
	int err = 0;

	CAMDRV_DEBUG("[%s]\n", value ? "DTP ON" : "DTP OFF");

	switch (value) {
	case EXT_CFG_DTP_OFF:{
			err = SR300PC20_WRITE_LIST(sr300pc20_dtp_off);
			first_start_camera = 1;
			/*sr300pc20_set_preview(); */
		}
		break;

	case EXT_CFG_DTP_ON:{
			err = SR300PC20_WRITE_LIST(sr300pc20_dtp_on);
		}
		break;

	default:{
			pr_warning(" unexpected DTP control\n");
		}
		break;
	}

	if (err != 0)
		pr_err("[%s : %d] %d fail\n", __func__, __LINE__, value);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static int sr300pc20_sensor_ext_config(void __user *arg)
{
	int err = 0;
	ioctl_pcam_info_8bit ctrl_info;

	if (copy_from_user
	    ((void *)&ctrl_info, (const void *)arg, sizeof(ctrl_info))) {
		pr_err(" %s fail copy_from_user!\n", __func__);
	}

	CAMDRV_DEBUG("%d %d %d %d %d\n",
		     ctrl_info.mode, ctrl_info.address, ctrl_info.value_1,
		     ctrl_info.value_2, ctrl_info.value_3);

	if (mScene != EXT_CFG_SCENE_OFF) {
		if (ctrl_info.mode == EXT_CFG_EFFECT_CONTROL ||
		    ctrl_info.mode == EXT_CFG_WB_CONTROL ||
		    ctrl_info.mode == EXT_CFG_BR_CONTROL ||
		    ctrl_info.mode == EXT_CFG_ISO_CONTROL ||
		    ctrl_info.mode == EXT_CFG_METERING_CONTROL ||
		    ctrl_info.mode == EXT_CFG_CR_CONTROL ||
		    ctrl_info.mode == EXT_CFG_SA_CONTROL ||
		    ctrl_info.mode == EXT_CFG_SP_CONTROL) {
			pr_warn("[%s : %d]It doesn't accept this control "
				"because now scene mode is set!!\n",
				__func__, __LINE__);
			pr_warn("[%s : %d]This mode is %d\n\n",
				__func__, __LINE__, ctrl_info.mode);
			return err;
		}
	}

	switch (ctrl_info.mode) {
	case EXT_CFG_GET_INFO:{
/* EXT_CFG_GET_INFO cmd means driver transfers sensor information*/
/* to upper layer. In case of this exposure time and ISO */
			unsigned short lsb = 0, msb = 0;
			unsigned short a_gain = 0;
			unsigned char data = 0;

			/* page mode 0x20 */
			sr300pc20_sensor_write(0x03, 0x20);

			/* read exposure time */
			sr300pc20_sensor_read(0xa0, &data);
			msb = (unsigned short)data;
			sr300pc20_sensor_read(0xa1, &data);
			msb = (msb << 8) | (unsigned short)data;
			sr300pc20_sensor_read(0xa2, &data);
			lsb = (unsigned short)data;
			sr300pc20_sensor_read(0xa3, &data);
			lsb = (lsb << 8) | (unsigned short)data;

			/* read ISO gain */
			sr300pc20_sensor_read(0x50, &data);
			a_gain = (unsigned short)data;

			ctrl_info.value_1 = lsb;
			ctrl_info.value_2 = msb;
			ctrl_info.value_3 = a_gain;
			CAMDRV_DEBUG("exposure %x\n", (msb << 16) | lsb);
			CAMDRV_DEBUG("ISO gain is %x\n", a_gain);
		}
		break;

	case EXT_CFG_FRAME_CONTROL:
		{
			switch (ctrl_info.value_1) {

			case EXT_CFG_FRAME_FIX_15:{
					err = SR300PC20_WRITE_LIST
					    (sr300pc20_fps_15fix);
				}
				break;

			case EXT_CFG_FRAME_FIX_24:{
					err = SR300PC20_WRITE_LIST
					    (sr300pc20_fps_24fix);
				}
				break;

			case EXT_CFG_FRAME_FIX_30:{
					err = SR300PC20_WRITE_LIST
					    (sr300pc20_fps_30fix);
				}
				break;

			default:{
					pr_warning
					    (" Unexpected"
					     "EXT_CFG_FRAME_CONTROL mode :"
					     "%d\n", ctrl_info.value_1);
				}
				break;

			}
		}
		break;

	case EXT_CFG_EFFECT_CONTROL:
		{
			mEffect = ctrl_info.value_1;
			err = sr300pc20_effect_control(mEffect);

		}
		break;

	case EXT_CFG_WB_CONTROL:
		{
			mWhiteBalance = ctrl_info.value_1;
			err = sr300pc20_whitebalance_control(mWhiteBalance);

		}
		break;

	case EXT_CFG_BR_CONTROL:
		{
			mBrightness = ctrl_info.value_1;
			if (mInit)
				err = sr300pc20_brightness_control(mBrightness);

		}
		break;

	case EXT_CFG_ISO_CONTROL:
		{
			mISO = ctrl_info.value_1;
			err = sr300pc20_iso_control(mISO);

		}
		break;

	case EXT_CFG_METERING_CONTROL:
		{
			mAutoExposure = ctrl_info.value_1;
			err = sr300pc20_metering_control(mAutoExposure);

		}
		break;

	case EXT_CFG_SCENE_CONTROL:
		{
			mScene = ctrl_info.value_1;
			err = sr300pc20_scene_control(mScene);

		}
		break;

	case EXT_CFG_AE_AWB_CONTROL:
		{
			switch (ctrl_info.value_1) {
			case EXT_CFG_AE_LOCK:{
					err =
					    SR300PC20_WRITE_LIST
					    (sr300pc20_ae_lock);
				}
				break;

			case EXT_CFG_AE_UNLOCK:{
					err = SR300PC20_WRITE_LIST
					    (sr300pc20_ae_unlock);
				}
				break;

			case EXT_CFG_AWB_LOCK:{
					err = SR300PC20_WRITE_LIST
					    (sr300pc20_awb_lock);
				}
				break;

			case EXT_CFG_AWB_UNLOCK:{
					err = SR300PC20_WRITE_LIST
					    (sr300pc20_awb_unlock);
				}
				break;

			case EXT_CFG_AE_AWB_LOCK:{
					if (mWhiteBalance == 0) {
						err = SR300PC20_WRITE_LIST
						    (sr300pc20_ae_lock);
						err = SR300PC20_WRITE_LIST
						    (sr300pc20_awb_lock);
					} else {
						err = SR300PC20_WRITE_LIST
						    (sr300pc20_ae_lock);
					}
				}
				break;

			case EXT_CFG_AE_AWB_UNLOCK:{
					if (mWhiteBalance == 0) {
						err = SR300PC20_WRITE_LIST
						    (sr300pc20_ae_unlock);
						err = SR300PC20_WRITE_LIST
						    (sr300pc20_awb_unlock);
					} else {
						err = SR300PC20_WRITE_LIST
						    (sr300pc20_ae_unlock);
					}
				}
				break;

			default:{
					pr_warning
					    (" Unexpected AWB_AE mode :"
					     "%d\n", ctrl_info.value_1);
				}
				break;

			}
		}
		break;

	case EXT_CFG_CR_CONTROL:
		{
			mContrast = ctrl_info.value_1;
			if (mInit)
				err = sr300pc20_contrast_control(mContrast);
		}
		break;

	case EXT_CFG_SA_CONTROL:
		{
			mSaturation = ctrl_info.value_1;
			if (mInit)
				err = sr300pc20_saturation_control(mSaturation);
		}
		break;

	case EXT_CFG_SP_CONTROL:
		{
			mSharpness = ctrl_info.value_1;
			if (mInit)
				err = sr300pc20_sharpness_control(mSharpness);
		}
		break;

	case EXT_CFG_DTP_CONTROL:
		{
			if (mInit == 0) {
				if (ctrl_info.value_1 == 0)
					ctrl_info.value_3 = 2;

				else if (ctrl_info.value_1 == 1)
					ctrl_info.value_3 = 3;

				mDTP = 1;
			}

			else {
				err = sr300pc20_DTP_control(ctrl_info.value_1);

				if (ctrl_info.value_1 == 0)
					ctrl_info.value_3 = 2;

				else if (ctrl_info.value_1 == 1)
					ctrl_info.value_3 = 3;

				mDTP = 0;
			}

		}
		break;

	case EXT_CFG_GET_MODULE_STATUS:
		{
			unsigned short id = 0;	/*CAM FOR FW */
			/*ctrl_info.value_3 = gpio_get_value(0); */

			ctrl_info.value_3 = id;

			CAMDRV_DEBUG
			    (" check current module status : %x\n",
			     ctrl_info.value_3);
			CAMDRV_DEBUG(" PINON/OFF : %d\n", gpio_get_value(0));
		}
		break;

	case EXT_CFG_SET_CAM_MODE:
		{
			mCameraMode = ctrl_info.value_1;
			if (mCameraMode == EXT_CFG_CAMCORDER_MODE) {
				pr_err("[%s : %d] Record is starting!!\n",
				       __func__, __LINE__);
			} else {
				pr_err("[%s : %d] Record stopped!!\n",
				       __func__, __LINE__);
			}
		}
		break;

	default:{
			pr_warning
			    (" Unexpected mode on sensor_rough_control :"
			     "%d\n", ctrl_info.mode);
		}
		break;
	}

	if (copy_to_user
	    ((void *)arg, (const void *)&ctrl_info, sizeof(ctrl_info))) {
		pr_err(" %s fail on copy_to_user!\n", __func__);
	}

	if (err != 0)
		pr_err("[%s : %d] fail %d %d %d %d %d\n", __func__,
		       __LINE__, ctrl_info.mode, ctrl_info.address,
		       ctrl_info.value_1, ctrl_info.value_2, ctrl_info.value_3);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static int sr300pc20_mipi_mode(int mode)
{
	int rc = 0;
	struct msm_camera_csi_params sr300pc20_csi_params;

	CAMDRV_DEBUG("Enter!!\n");

	if (!config_csi2) {
		sr300pc20_csi_params.lane_cnt = 1;
		sr300pc20_csi_params.data_format = CSI_8BIT;
		sr300pc20_csi_params.lane_assign = 0xe4;
		sr300pc20_csi_params.dpcm_scheme = 0;
		sr300pc20_csi_params.settle_cnt = 0x14;	/*lyon.cho 24->0x14 */
		rc = msm_camio_csi_config(&sr300pc20_csi_params);
		if (rc < 0) {
			pr_err("config csi controller failed\n");
			return -EIO;
		}
		config_csi2 = 1;
	}
	CAMDRV_DEBUG("Exit!!\n");
	return rc;
}

static int sr300pc20_set_preview(void)
{
	int err = 0;
	int first_awb_delay = 0;

	CAMDRV_DEBUG("mDTP = %d\n", mDTP);

	err = sr300pc20_mipi_mode(1);
	if (err != 0) {
		pr_err("[%s : %d] sr300pc20_mipi_mode fail\n", __func__,
		       __LINE__);
		return -EIO;
	}

	msleep(30);		/*=> Please add some delay*/

	if (first_start_camera) {
		err = SR300PC20_WRITE_LIST_BURST(sr300pc20_init0);
		if (err != 0) {
			pr_err("[%s : %d] sr300pc20_init0 failed\n", __func__,
			       __LINE__);
			return -EIO;
		}
		CAMDRV_DEBUG("sr300pc20_init0 is writing done\n");

		msleep(20);
		first_start_camera = 0;
		mInit = 1;
		first_awb_delay = 1;

		err = sr300pc20_brightness_control(mBrightness);
		err = sr300pc20_effect_control(mEffect);
		err = sr300pc20_whitebalance_control(mWhiteBalance);
		err = sr300pc20_iso_control(mISO);
	}

	if (mDTP == 1) {
		err = SR300PC20_WRITE_LIST(sr300pc20_dtp_on);
	} else {
		err = SR300PC20_WRITE_LIST(sr300pc20_update_preview_setting);
		if (err != 0)
			pr_err("[%s : %d] sr300pc20_update_preview_setting "
			       "fail\n", __func__, __LINE__);
		pr_err("[%s:%d] Preivew setting is done!!", __func__, __LINE__);
	}

	if (err != 0)
		pr_err("[%s : %d] fail\n", __func__, __LINE__);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static int sr300pc20_set_capture(void)
{
	int err = 0;

	CAMDRV_DEBUG("Enter!!\n");

	if (mScene == EXT_CFG_SCENE_NIGHTSHOT) {
		unsigned char lux_val = 0;

		/* page mode 0x20 */
		sr300pc20_sensor_write(0x03, 0x20);

		/* read lux value */
		sr300pc20_sensor_read(0xb1, &lux_val);

		CAMDRV_DEBUG("[Nightshot] lux_val = 0x%x\n", lux_val);

		if (lux_val < 0x16) {
			err = SR300PC20_WRITE_LIST
				(sr300pc20_night_snapshot);
		} else {
			err = SR300PC20_WRITE_LIST(sr300pc20_snapshot);
		}
	} else if (mScene == EXT_CFG_SCENE_FIREWORK) {
		unsigned char lux_val = 0;

		/* page mode 0x20 */
		sr300pc20_sensor_write(0x03, 0x20);

		/* read lux value */
		sr300pc20_sensor_read(0xb1, &lux_val);

		CAMDRV_DEBUG("[Nightshot] lux_val = 0x%x\n", lux_val);

		if (lux_val < 0x16) {
			err = SR300PC20_WRITE_LIST
			    (sr300pc20_fireworks_snapshot);
		} else {
			err = SR300PC20_WRITE_LIST(sr300pc20_snapshot);
		}
	} else {
		err = SR300PC20_WRITE_LIST(sr300pc20_snapshot);
	}

	if (err != 0)
		pr_err("[%s : %d] fail\n", __func__, __LINE__);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static long sr300pc20_set_sensor_mode(int mode)
{
	int err = 0;

	CAMDRV_DEBUG("Enter [mode = %d]\n", mode);

	switch (mode) {
	case SENSOR_PREVIEW_MODE:
		err = sr300pc20_set_preview();
		break;
	case SENSOR_SNAPSHOT_MODE:
		err = sr300pc20_set_capture();
		break;
	case SENSOR_RAW_SNAPSHOT_MODE:
		pr_err("RAW_SNAPSHOT NOT SUPPORT!!");
		break;
	default:
		return -EINVAL;
	}

	if (err != 0)
		pr_err("[%s : %d] mode is %d fail\n", __func__, __LINE__, mode);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static void sr300pc20_reset_power(void)
{
	pr_err("[%s : %d] entered\n", __func__, __LINE__);
	sr300pc20_set_power(0);
	/*add delay */
	usleep(10 * UPTO_MSEC);	/* 10 msec */
	sr300pc20_set_power(1);
	pr_err("[%s : %d] exit\n", __func__, __LINE__);
}

void sr300pc20_set_power(int status)
{
	pr_err("[SR300PC20 %s]\n", status ? "POWER ON" : "POWER OFF");

	if (status == 1) {	/*POWER ON */
		cam_ldo_power_on();
#if defined(CONFIG_MACH_JENA)
		usleep(75 * UPTO_MSEC);	/* delay 75msec */
#else
		usleep(25 * UPTO_MSEC);	/* delay 25msec */
#endif

		/* Enable MCLK */
		msm_camio_clk_rate_set(24000000);	/* 24 Mhz */
		usleep(10);
		msm_camio_clk_enable(CAMIO_CAM_MCLK_CLK);
		usleep(23 * UPTO_MSEC);	/* delay 23msec */

		gpio_set_value(96, 1);	/*standby */
		usleep(12 * UPTO_MSEC);	/* delay 12msec */

		gpio_set_value(85, 1);	/*reset */
		usleep(1 * UPTO_MSEC);	/* delay 1msec  */

	} else {		/*POWER OFF */
		usleep(1 * UPTO_MSEC);	/* 1 msec */
		gpio_set_value_cansleep(85, LOW);	/* reset */
		usleep(10 * UPTO_MSEC);	/* >10 msec */

		msm_camio_clk_disable(CAMIO_CAM_MCLK_CLK);
		usleep(1 * UPTO_MSEC);	/* 1 msec */

		gpio_set_value_cansleep(96, LOW);	/* standby */
		usleep(10);
		cam_ldo_power_off();
	}
}

static int sr300pc20_check_sensor_id(void)
{
	int err = 0;
	unsigned char id = 0;
	const char page_mode_big = 0x03, page_mode_little = 0x00;
	const char read_addr = 0x04;

	CAMDRV_DEBUG("Enter!!\n");

	/* read device id */
	err = sr300pc20_sensor_write(page_mode_big, page_mode_little);
	if (err != 0)
		pr_err("[%s : %d] sr300pc20_sensor_write fail\n", __func__,
		       __LINE__);

	err = sr300pc20_sensor_read(read_addr, &id);	/*address */
	if (err != 0)
		pr_err("[%s : %d] sr300pc20_sensor_read fail\n", __func__,
		       __LINE__);

	if (id != 0xa4) {
		pr_err("[SR300PC20] WRONG SENSOR FW => id 0x%x\n", id);
		err = -1;
	} else {
		CAMDRV_DEBUG("[SR300PC20] CURRENT SENSOR FW => id 0x%x\n", id);
	}
	if (err != 0)
		pr_err("[%s : %d] fail\n", __func__, __LINE__);
	else
		CAMDRV_DEBUG("Exit!!\n");

	return err;
}

#ifdef CONFIG_LOAD_FILE

static int sr300pc20_regs_table_init(void)
{
	struct file *filp;
	char *dp = NULL;
	long l = 0;
	loff_t pos;
	int ret = -1;
	mm_segment_t fs = get_fs();

	pr_err("[%s : %d] Entered\n", __func__, __LINE__);

	set_fs(get_ds());

	filp = filp_open(TUNNING_FILE_PATH, O_RDONLY, 0);

	if (IS_ERR(filp)) {
		pr_err("[%s : %d]file open error\n", __func__, __LINE__);
		return -ret;
	}
	l = filp->f_path.dentry->d_inode->i_size;
	pr_info("l = %ld\n", l);
	dp = vmalloc(l);
	if (dp == NULL) {
		pr_err("[%s : %d] Out of Memory\n", __func__, __LINE__);
		filp_close(filp, current->files);
		return ret;
	}
	pos = 0;
	memset(dp, 0, l);
	ret = vfs_read(filp, (char __user *)dp, l, &pos);
	if (ret != l) {
		pr_err("[%s : %d] Failed to read file ret = %d\n",
		       __func__, __LINE__, ret);
		if (dp != NULL)
			vfree(dp);
		filp_close(filp, current->files);
		ret = -EIO;
		return ret;
	}

	filp_close(filp, current->files);

	set_fs(fs);

	sr300pc20_regs_table = dp;

	sr300pc20_regs_table_size = l;

	*((sr300pc20_regs_table + sr300pc20_regs_table_size) - 1) = '\0';

	pr_err("Exit!!\n");

	ret = 0;
	return ret;
}

void sr300pc20_regs_table_exit(void)
{
	CAMDRV_DEBUG("Enter!!\n");
	if (sr300pc20_regs_table != NULL) {
		vfree(sr300pc20_regs_table);
		sr300pc20_regs_table = NULL;
	}
	CAMDRV_DEBUG("Exit!!\n");
}

static int sr300pc20_is_hexnum(char *num)
{
	int i = 0;
	for (i = 2; num[i] != '\0'; i++) {
		if (!((num[i] >= '0' && num[5] <= '9')
		      || (num[5] >= 'a' && num[5] <= 'f') || (num[5] >= 'A'
							      && num[5] <=
							      'F'))) {
			return 0;
		}
	}
	return 1;
}

static int sr300pc20_regs_table_write(char *name)
{
	char *start = NULL, *end = NULL, *reg = NULL, *temp = NULL;
	unsigned char addr = 0, value = 0;
	unsigned short data = 0;
	char data_buf[7] = { 0 };
	int err = 0;

	CAMDRV_DEBUG("Enter!!\n");

	addr = value = 0;

	*(data_buf + 6) = '\0';

	start = strnstr(sr300pc20_regs_table, name, sr300pc20_regs_table_size);
	if (start == NULL) {
		pr_err("[%s : %d] start is NULL\n", __func__, __LINE__);
		err = -EIO;
		return err;
	}

	end = strnstr(start, "};", sr300pc20_regs_table_size);
	if (end == NULL) {
		pr_err("[%s : %d] end is NULL\n", __func__, __LINE__);
		err = -EIO;
		return err;
	}

	while (1) {
		/* Find Address */
		reg = strnstr(start, "0x", sr300pc20_regs_table_size);
		if (reg)
			start = (reg + 6);

		if ((reg == NULL) || (reg > end)) {
			pr_err("[%s : %d] write end of %s\n",
			       __func__, __LINE__, name);
			break;
		}
		/* Write Value to Address */
		memcpy(data_buf, reg, 6);

		if (sr300pc20_is_hexnum(data_buf) == 0) {
			pr_err("[%s : %d] it's not hex number %s\n",
			       __func__, __LINE__, data_buf);
			continue;
		}

		err = kstrtou16(data_buf, 16, &data);
		if (err < 0) {
			pr_err("[%s : %d] kstrtou16 failed\n",
			       __func__, __LINE__);
		}
		addr = (data >> 8);
		value = (data & 0xff);

		if (addr == 0xff) {
			msleep(value * 10);	/*one step is 10ms */
			CAMDRV_DEBUG("delay %d msec\n", value * 10);
		} else {
			if (sr300pc20_sensor_write(addr, value) < 0) {
				pr_err
				    ("[%s : %d] fail on sensor_write :"
				     "addr[0x%04x], value[0x%04x]\n",
				     __func__, __LINE__, addr, value);
				err = -EIO;
				return err;
			}
			CAMDRV_DEBUG
			    ("success on sensor_write :"
			     "addr[0x%04x], value[0x%04x]\n", addr, value);
		}
	}

	CAMDRV_DEBUG("Exit!!\n");

	return err;
}

static int sr300pc20_regs_table_burst_write(char *name)
{
	char *start = NULL, *end = NULL;
	char *reg = NULL, *temp = NULL;
	unsigned char addr = 0, value = 0;
	unsigned short data = 0;
	char data_buf[7] = { 0 };
	int idx = 0;
	int err = 0;
	int burst_flag = 0;
	int burst_cnt = 0;
	struct i2c_msg msg = { sr300pc20_client->addr,
		0, 0, sr300pc20_buf_for_burstmode
	};

	CAMDRV_DEBUG("Enter!!\n");

	addr = value = 0;

	*(data_buf + 6) = '\0';

	start = strnstr(sr300pc20_regs_table, name, sr300pc20_regs_table_size);
	if (start == NULL) {
		pr_err("[%s : %d] start is NULL\n", __func__, __LINE__);
		err = -EIO;
		return err;
	}

	end = strnstr(start, "};", sr300pc20_regs_table_size);
	if (end == NULL) {
		pr_err("[%s : %d] end is NULL\n", __func__, __LINE__);
		err = -EIO;
		return err;
	}

	while (1) {
		/* Find Address */
		reg = strnstr(start, "0x", sr300pc20_regs_table_size);
		if (reg)
			start = (reg + 6);

		if ((reg == NULL) || (reg > end)) {
			pr_err("[%s : %d] write end of %s\n",
			       __func__, __LINE__, name);
			break;
		}
		/* Write Value to Address */
		memcpy(data_buf, reg, 6);

		if (sr300pc20_is_hexnum(data_buf) == 0) {
			pr_err("[%s : %d] it's not hex number %s\n",
			       __func__, __LINE__, data_buf);
			continue;
		}

		err = kstrtou16(data_buf, 16, &data);
		if (err < 0) {
			pr_err("[%s : %d] kstrtou16 failed\n",
			       __func__, __LINE__);
		}
		addr = (data >> 8);
		value = (data & 0xff);

		if (idx > (BURST_MODE_BUFFER_MAX_SIZE - 10)) {
			pr_err("[%s : %d]Burst mode buffer overflow! "
			       "Burst Count %d\n",
			       __func__, __LINE__, burst_cnt);
			pr_err("[%s : %d] addr %x "
			       "value %x\n", __func__, __LINE__,
			       (data >> 8) & 0xff, data & 0xFF);

			err = -EIO;
			return err;
		}

		if (burst_flag == 0) {
			switch (addr) {
			case BURST_REG:
				if (value != 0x00) {
					burst_flag = 1;
					burst_cnt++;
				}
				break;
			case DELAY_REG:
				msleep(value * 10);	/* a step is 10ms */
				break;
			default:
				idx = 0;
				err = sr300pc20_sensor_write(addr, value);
				break;
			}
		} else if (burst_flag == 1) {
			if (addr == BURST_REG && value == 0x00) {
				msg.len = idx;
				err = i2c_transfer(sr300pc20_client->adapter,
						   &msg, 1) == 1 ? 0 : -EIO;
				idx = 0;
				burst_flag = 0;
			} else {
				if (idx == 0) {
					sr300pc20_buf_for_burstmode[idx++] =
					    addr;
					sr300pc20_buf_for_burstmode[idx++] =
					    value;
				} else
					sr300pc20_buf_for_burstmode[idx++] =
					    value;
			}
		}
	}

	CAMDRV_DEBUG("Exit!!\n");

	return err;
}

#endif

int sr300pc20_sensor_init(const struct msm_camera_sensor_info *data)
{
	int rc = 0;

	CAMDRV_DEBUG("Enter!!\n");

	sr300pc20_ctrl = kzalloc(sizeof(struct sr300pc20_ctrl), GFP_KERNEL);
	if (!sr300pc20_ctrl) {
		pr_err("sr300pc20_init failed!\n");
		rc = -ENOMEM;
		goto init_done;
	}

	if (data)
		sr300pc20_ctrl->sensordata = data;

	first_start_camera = 1;
	config_csi2 = 0;
#ifdef CONFIG_LOAD_FILE
	rc = sr300pc20_regs_table_init();
	if (rc < 0) {
		pr_err("[%s : %d] sr300pc20_regs_table_init fail\n",
		       __func__, __LINE__);
		goto init_fail;
	}
#endif
	sr300pc20_set_power(1);
	usleep(1 * UPTO_MSEC);	/* 1msec */

	rc = sr300pc20_check_sensor_id();
	if (rc < 0) {
		pr_err("[%s : %d] sr300pc20_check_sensor_id fail\n",
		       __func__, __LINE__);
		goto init_fail;
	}
	rc = SR300PC20_WRITE_LIST(sr300pc20_pre_init0);
	if (rc < 0) {
		pr_err("[%s : %d] sr300pc20_pre_init0 set fail\n",
		       __func__, __LINE__);
		goto init_fail;
	}

init_done:
	CAMDRV_DEBUG("Exit!!\n");
	return rc;

init_fail:
	kfree(sr300pc20_ctrl);
	return rc;
}

static int sr300pc20_init_client(struct i2c_client *client)
{
	/* Initialize the MSM_CAMI2C Chip */
	init_waitqueue_head(&sr300pc20_wait_queue);
	return 0;
}

int sr300pc20_sensor_config(void __user *argp)
{
	struct sensor_cfg_data cfg_data;
	long rc = 0;

	if (copy_from_user(&cfg_data,
			   (void *)argp, sizeof(struct sensor_cfg_data)))
		return -EFAULT;

	pr_err("[%s : %d] cfgtype = %d, mode = %d\n",
	       __func__, __LINE__, cfg_data.cfgtype, cfg_data.mode);

	switch (cfg_data.cfgtype) {
	case CFG_SET_MODE:
		rc = sr300pc20_set_sensor_mode(cfg_data.mode);
		break;

	case CFG_SET_EFFECT:
		/*rc = sr300pc20_set_effect
		   (cfg_data.mode, cfg_data.cfg.effect); */
		break;

	default:
		rc = -EINVAL;
		pr_err("sr300pc20_sensor_config : Invalid cfgtype ! %d\n",
		       cfg_data.cfgtype);
		break;
	}
	if (rc != 0)
		pr_err("[%s : %d] fail\n", __func__, __LINE__);

	return rc;
}

int sr300pc20_sensor_release(void)
{
	int rc = 0;
	/*int *switch_i2c_addr; //TEMP Dirty Code, Do not use it! */

	first_start_camera = 1;
	set_init0 = 0;

	/*If did not init below that, it can keep the previous status.
	   it depend on concept by PCAM */
	mEffect = EXT_CFG_EFFECT_NORMAL;
	mBrightness = EXT_CFG_BR_STEP_0;
	mContrast = EXT_CFG_CR_STEP_0;
	mSaturation = EXT_CFG_SA_STEP_0;
	mSharpness = EXT_CFG_SP_STEP_0;
	mWhiteBalance = EXT_CFG_WB_AUTO;
	mISO = EXT_CFG_ISO_AUTO;
	mAutoExposure = EXT_CFG_METERING_NORMAL;
	mScene = EXT_CFG_SCENE_OFF;
	mDTP = 0;
	mInit = 0;

	CAMDRV_DEBUG("Enter!!\n");

	kfree(sr300pc20_ctrl);

#ifdef CONFIG_LOAD_FILE
	sr300pc20_regs_table_exit();
#endif

	sr300pc20_set_power(0);

	CAMDRV_DEBUG("Exit!!\n");

	return rc;
}

static int sr300pc20_i2c_probe(struct i2c_client *client,
			       const struct i2c_device_id *id)
{
	int rc = 0;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		rc = -ENOTSUPP;
		goto probe_failure;
	}

	sr300pc20_sensorw = kzalloc(sizeof(struct sr300pc20_work), GFP_KERNEL);

	if (!sr300pc20_sensorw) {
		rc = -ENOMEM;
		goto probe_failure;
	}

	i2c_set_clientdata(client, sr300pc20_sensorw);
	sr300pc20_init_client(client);
	sr300pc20_client = client;

	pr_err("sr300pc20_probe succeeded!\n");

	return 0;

probe_failure:
	kfree(sr300pc20_sensorw);
	sr300pc20_sensorw = NULL;
	pr_err("sr300pc20_probe failed!\n");
	return rc;
}

static const struct i2c_device_id sr300pc20_i2c_id[] = {
	{"sr300pc20", 0},
	{},
};

static struct i2c_driver sr300pc20_i2c_driver = {
	.id_table = sr300pc20_i2c_id,
	.probe = sr300pc20_i2c_probe,
	.remove = __exit_p(sr300pc20_i2c_remove),
	.driver = {
		   .name = "sr300pc20",
		   },
};

static int sr300pc20_sensor_probe(const struct msm_camera_sensor_info *info,
				  struct msm_sensor_ctrl *s)
{
	int rc = i2c_add_driver(&sr300pc20_i2c_driver);
	if (rc < 0 || sr300pc20_client == NULL) {
		rc = -ENOTSUPP;
		goto probe_done;
	}

	s->s_init = sr300pc20_sensor_init;
	s->s_release = sr300pc20_sensor_release;
	s->s_config = sr300pc20_sensor_config;
	s->s_ext_config = sr300pc20_sensor_ext_config;

	s->s_camera_type = BACK_CAMERA_2D;
	s->s_mount_angle = 90;

probe_done:
	pr_info("%s:%d\n", __func__, __LINE__);
	return rc;
}

static int __sr300pc20_probe(struct platform_device *pdev)
{
	pr_err("############# SR300PC20 probe ##############\n");
	return msm_camera_drv_start(pdev, sr300pc20_sensor_probe);
}

static struct platform_driver msm_camera_driver = {
	.probe = __sr300pc20_probe,
	.driver = {
		   .name = "msm_camera_sr300pc20",
		   .owner = THIS_MODULE,
		   },
};

static int __init sr300pc20_init(void)
{
	return platform_driver_register(&msm_camera_driver);
}

module_init(sr300pc20_init);
