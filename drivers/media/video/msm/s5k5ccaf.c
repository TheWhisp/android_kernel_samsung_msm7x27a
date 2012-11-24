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

/* 1/5" s5k5ccgx*/

#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <media/msm_camera.h>
#include <mach/gpio.h>
#include <mach/pmic.h>

#include "sr300pc20r1_mipi_0826.h"
#include <mach/camera.h>
#include <mach/vreg.h>
#include <linux/io.h>

#include "sec_cam_pmic.h"

/*#define SENSOR_DEBUG 0*/

/*#define CONFIG_LOAD_FILE*/

#ifdef CONFIG_LOAD_FILE
#define S5K5CCAF_WRITE_LIST(A)	\
	s5k5ccaf_sensor_write_list(A, (sizeof(A) / sizeof(A[0])), #A);
#define S5K5CCAF_WRITE_LIST_BURST(A)	\
	s5k5ccaf_sensor_write_list(A, (sizeof(A) / sizeof(A[0])), #A);
#else
#define S5K5CCAF_WRITE_LIST(A)	\
	s5k5ccaf_sensor_write_list(A, (sizeof(A) / sizeof(A[0])), #A);
#define S5K5CCAF_WRITE_LIST_BURST(A)	\
	s5k5ccaf_sensor_write_list(A, (sizeof(A) / sizeof(A[0])), #A);
#endif

static char first_start_camera;	/*  1 is not init a sensor */
static char set_init0;
static char af_low_lux;

static char mEffect = EXT_CFG_EFFECT_NORMAL;
static char mBrightness = EXT_CFG_BR_STEP_0;
static char mContrast = EXT_CFG_CR_STEP_0;
static char mSaturation = EXT_CFG_SA_STEP_0;
static char mSharpness = EXT_CFG_SP_STEP_0;
static char mWhiteBalance = EXT_CFG_WB_AUTO;
static char mISO = EXT_CFG_ISO_AUTO;
static char mAutoExposure = EXT_CFG_METERING_NORMAL;
static char mScene = EXT_CFG_SCENE_OFF;
static char mAfMode;
static char mDTP;
static char mInit;
/*static char mMode = 0;*/

struct s5k5ccaf_work {
	struct work_struct work;
};

static struct s5k5ccaf_work *s5k5ccaf_sensorw;
static struct i2c_client *s5k5ccaf_client;

struct s5k5ccaf_ctrl {
	const struct msm_camera_sensor_info *sensordata;
};
static unsigned int config_csi2;
static struct s5k5ccaf_ctrl *s5k5ccaf_ctrl;
static DECLARE_WAIT_QUEUE_HEAD(s5k5ccaf_wait_queue);
DECLARE_MUTEX(s5k5ccaf_sem);
static int16_t s5k5ccaf_effect = CAMERA_EFFECT_OFF;

/*=============================================================
	EXTERNAL DECLARATIONS
==============================================================*/
/*struct s5k5ccaf_reg s5k5ccaf_regs;*/
static void pcam_msm_i2c_pwr_mgmt(struct i2c_adapter *adap, int on);
int *get_i2c_clock_addr(struct i2c_adapter *adap);
/*=============================================================*/

static int cam_hw_init(void);
static void s5k5ccaf_set_power(int status);
static void s5k5ccaf_reset_power(void);
static void s5k5ccaf_set_preview(void);

#ifdef CONFIG_LOAD_FILE
static int s5k5ccaf_regs_table_write(char *name);
#endif

static int s5k5ccaf_sensor_read(unsigned short subaddr, unsigned short *data)
{
	int ret;
	unsigned char buf[2];
	struct i2c_msg msg = { s5k5ccaf_client->addr, 0, 2, buf };

	buf[0] = (subaddr >> 8);
	buf[1] = (subaddr & 0xFF);

	ret = i2c_transfer(s5k5ccaf_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
	if (ret == -EIO)
		goto error;

	msg.flags = I2C_M_RD;

	ret = i2c_transfer(s5k5ccaf_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
	if (ret == -EIO)
		goto error;

/*	*data = ((buf[0] << 8) | buf[1]); //check*/
	*data = buf[0];		/*check */

error:
	return ret;
}

static int s5k5ccaf_sensor_write(unsigned short subaddr, unsigned short val)
{
				/*lyon.cho */
	unsigned char buf[2];
	struct i2c_msg msg = { s5k5ccaf_client->addr, 0, 2, buf };

	pr_info("[PGH] on write func s5k5ccaf_client->addr : %x\n",
	       s5k5ccaf_client->addr);
	pr_info("[PGH] on write func  s5k5ccaf_client->adapter->nr : %d\n",
	       s5k5ccaf_client->adapter->nr);

	buf[0] = (subaddr);
/*	buf[1] = (subaddr & 0xFF);*/
	buf[1] = (val);
/*	buf[3] = (val & 0xFF);*/
	return i2c_transfer(s5k5ccaf_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
}

/*static int s5k5ccaf_sensor_write_list(struct samsung_short_t *list,
int size, char *name)*/
static int s5k5ccaf_sensor_write_list(const u32 *list, int size, char *name)
{
	int ret = 0;
	unsigned short subaddr = 0;
	unsigned short value = 0;
	int i = 0;
	pr_info("s5k5ccaf_sensor_write_list : %s\n", name);

#ifdef CONFIG_LOAD_FILE
	ret = s5k5ccaf_regs_table_write(name);
#else

	for (i = 0; i < size; i++) {
		CAMDRV_DEBUG("[PGH] %x      %x\n", list[i], list[i]);
		subaddr = (list[i] >> 8);	/*address */
/*	if (subaddr == 0x0F12) next_subaddr= (list[i+1]>> 16); //address*/
		value = (list[i] & 0xFF);	/*value */

		if (subaddr == 0xff) {
			/*msleep(value);*/
			pr_info("[s5k5ccaf] msleep %d\n", value);
		} else {
			/*  s5k5ccaf_sensor_write(subaddr, value); */
			if (s5k5ccaf_sensor_write(subaddr, value) < 0) {
				pr_info("[s5k5ccaf] sensor_write_list fail...-_-\n");
				return -EIO;
			}
		}
	}

#endif
	return ret;
}

#define BURST_MODE_BUFFER_MAX_SIZE 2700
unsigned char s5k5ccaf_buf_for_burstmode[BURST_MODE_BUFFER_MAX_SIZE];
/*static int s5k5ccaf_sensor_burst_write_list(struct samsung_short_t *list,
int size, char *name)*/
static int s5k5ccaf_sensor_burst_write_list(const u32 *list, int size,
char *name)
{
	int err = -EINVAL;
	int i = 0;
	int idx = 0;

	unsigned short subaddr = 0, next_subaddr = 0;
	unsigned short value = 0;

	struct i2c_msg msg = { s5k5ccaf_client->addr, 0, 0,
		s5k5ccaf_buf_for_burstmode };
	pr_info("s5k5ccaf_sensor_burst_write_list : %s\n", name);

	for (i = 0; i < size; i++) {
		if (idx > (BURST_MODE_BUFFER_MAX_SIZE - 10)) {
			pr_info("<=PCAM=> BURST MODE buffer overflow!!!\n");
			return err;
		}

		subaddr = (list[i] >> 8);	/*address */
/*if(subaddr == 0x0F12) next_subaddr= (list[i+1] >> 16); //address*/
		value = (list[i] & 0xFF);	/*value */

		switch (subaddr) {
		case 0xFF:
			msleep(value);
			break;

		default:
			idx = 0;
			err = s5k5ccaf_sensor_write(subaddr, value);
			break;
		}
	}

	if (unlikely(err < 0)) {
		pr_info("%s: register set failed\n", __func__);
		return err;
	}
	return 0;

}

unsigned short s5k5ccaf_get_lux(void)
{
	unsigned short msb, lsb, cur_lux;

	return cur_lux;
}

void s5k5ccaf_effect_control(char value)
{

	/*int *i2c_clk_addr; //TEMP Dirty Code, Do not use it!
	 *i2c_clk_addr = 0xd500c004;
	 */
	switch (value) {
	case EXT_CFG_EFFECT_NORMAL:{
			CAMDRV_DEBUG("EXT_CFG_EFFECT_NORMAL");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_effect_off);
		}
		break;

	case EXT_CFG_EFFECT_NEGATIVE:{
			CAMDRV_DEBUG("EXT_CFG_EFFECT_NEGATIVE");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_effect_negative);
		}
		break;

	case EXT_CFG_EFFECT_MONO:{
			CAMDRV_DEBUG("EXT_CFG_EFFECT_MONO");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_effect_mono);
		}
		break;

	case EXT_CFG_EFFECT_SEPIA:{
			CAMDRV_DEBUG("EXT_CFG_EFFECT_SEPIA");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_effect_sepia);
		}
		break;

	default:{
		pr_info("<=PCAM=> Unexpected Effect mode : %d\n", value);
		}
		break;
	}

}

void s5k5ccaf_whitebalance_control(char value)
{
	switch (value) {
	case EXT_CFG_WB_AUTO:{
			CAMDRV_DEBUG("EXT_CFG_WB_AUTO");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_wb_auto);
		}
		break;

	case EXT_CFG_WB_DAYLIGHT:{
			CAMDRV_DEBUG("EXT_CFG_WB_DAYLIGHT");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_wb_daylight);
		}
		break;

	case EXT_CFG_WB_CLOUDY:{
			CAMDRV_DEBUG("EXT_CFG_WB_CLOUDY");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_wb_cloudy);
		}
		break;

	case EXT_CFG_WB_FLUORESCENT:{
			CAMDRV_DEBUG("EXT_CFG_WB_FLUORESCENT");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_wb_fluorescent);
		}
		break;

	case EXT_CFG_WB_INCANDESCENT:{
			CAMDRV_DEBUG("EXT_CFG_WB_INCANDESCENT");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_wb_incandescent);
		}
		break;

	default:{
			pr_info("<=PCAM=> Unexpected WB mode : %d\n", value);
		}
		break;

	}

}

void s5k5ccaf_brightness_control(char value)
{
	switch (value) {
	case EXT_CFG_BR_STEP_P_4:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_P_4");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_brightness_p_4);
		}
		break;

	case EXT_CFG_BR_STEP_P_3:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_P_3");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_brightness_p_3);
		}
		break;

	case EXT_CFG_BR_STEP_P_2:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_P_2");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_brightness_p_2);
		}
		break;

	case EXT_CFG_BR_STEP_P_1:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_P_1");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_brightness_p_1);
		}
		break;

	case EXT_CFG_BR_STEP_0:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_0");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_brightness_0);
		}
		break;

	case EXT_CFG_BR_STEP_M_1:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_M_1");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_brightness_m_1);
		}
		break;

	case EXT_CFG_BR_STEP_M_2:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_M_2");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_brightness_m_2);
		}
		break;

	case EXT_CFG_BR_STEP_M_3:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_M_3");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_brightness_m_3);
		}
		break;

	case EXT_CFG_BR_STEP_M_4:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_M_4");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_brightness_m_4);
		}
		break;

	default:{
			pr_info("<=PCAM=> Unexpected BR mode : %d\n", value);
		}
		break;

	}

}

void s5k5ccaf_iso_control(char value)
{
	switch (value) {
	case EXT_CFG_ISO_AUTO:{
			CAMDRV_DEBUG("EXT_CFG_ISO_AUTO");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_iso_auto);
		}
		break;

	case EXT_CFG_ISO_50:{
			CAMDRV_DEBUG("EXT_CFG_ISO_50");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_iso_50);
		}
		break;

	case EXT_CFG_ISO_100:{
			CAMDRV_DEBUG("EXT_CFG_ISO_100");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_iso_100);
		}
		break;

	case EXT_CFG_ISO_200:{
			CAMDRV_DEBUG("EXT_CFG_ISO_200");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_iso_200);
		}
		break;

	case EXT_CFG_ISO_400:{
			CAMDRV_DEBUG("EXT_CFG_ISO_400");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_iso_400);
		}
		break;

	default:{
			pr_info("<=PCAM=> Unexpected ISO mode : %d\n", value);
		}
		break;

	}

}

void s5k5ccaf_metering_control(char value)
{
	switch (value) {
	case EXT_CFG_METERING_NORMAL:{
			CAMDRV_DEBUG("EXT_CFG_METERING_NORMAL");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_metering_normal);
		}
		break;

	case EXT_CFG_METERING_SPOT:{
			CAMDRV_DEBUG("EXT_CFG_METERING_SPOT");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_metering_spot);
		}
		break;

	case EXT_CFG_METERING_CENTER:{
			CAMDRV_DEBUG("EXT_CFG_METERING_CENTER");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_metering_center);
		}
		break;

	default:{
			pr_info("<=PCAM=> Unexpected METERING mode : %d\n",
			       value);
		}
		break;
	}

}

void s5k5ccaf_scene_control(char value)
{
	switch (value) {
	case EXT_CFG_SCENE_OFF:{
			CAMDRV_DEBUG("EXT_CFG_SCENE_OFF");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
		}
		break;

	case EXT_CFG_SCENE_PORTRAIT:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
			CAMDRV_DEBUG("EXT_CFG_SCENE_PORTRAIT");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_portrait);
		}
		break;

	case EXT_CFG_SCENE_LANDSCAPE:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
			CAMDRV_DEBUG("EXT_CFG_SCENE_LANDSCAPE");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_landscape);
		}
		break;

	case EXT_CFG_SCENE_SPORTS:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
			CAMDRV_DEBUG("EXT_CFG_SCENE_SPORTS");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_sports);
		}
		break;

	case EXT_CFG_SCENE_PARTY:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
			CAMDRV_DEBUG("EXT_CFG_SCENE_PARTY");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_party);
		}
		break;

	case EXT_CFG_SCENE_BEACH:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
			CAMDRV_DEBUG("EXT_CFG_SCENE_BEACH");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_beach);
		}
		break;

	case EXT_CFG_SCENE_SUNSET:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
			CAMDRV_DEBUG("EXT_CFG_SCENE_SUNSET");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_sunset);
		}
		break;

	case EXT_CFG_SCENE_DAWN:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
			CAMDRV_DEBUG("EXT_CFG_SCENE_DAWN");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_dawn);
		}
		break;

	case EXT_CFG_SCENE_FALL:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
			CAMDRV_DEBUG("EXT_CFG_SCENE_FALL");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_fall);
		}
		break;

	case EXT_CFG_SCENE_NIGHTSHOT:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
			CAMDRV_DEBUG("EXT_CFG_SCENE_NIGHTSHOT");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_nightshot);
		}
		break;

	case EXT_CFG_SCENE_BACKLIGHT:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
			CAMDRV_DEBUG("EXT_CFG_SCENE_BACKLIGHT");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_backlight);
		}
		break;

	case EXT_CFG_SCENE_FIREWORK:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
			CAMDRV_DEBUG("EXT_CFG_SCENE_FIREWORK");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_firework);
		}
		break;

	case EXT_CFG_SCENE_TEXT:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
			CAMDRV_DEBUG("EXT_CFG_SCENE_TEXT");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_text);
		}
		break;

	case EXT_CFG_SCENE_CANDLE:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_off);
			CAMDRV_DEBUG("EXT_CFG_SCENE_CANDLE");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_scene_candle);
		}
		break;

	default:{
			pr_info("<=PCAM=> Unexpected SCENE mode : %d\n", value);
		}
		break;
	}
}

void s5k5ccaf_contrast_control(char value)
{
	switch (value) {
	case EXT_CFG_CR_STEP_M_2:{
			CAMDRV_DEBUG("EXT_CFG_CR_STEP_M_2");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_contrast_m_2);
		}
		break;

	case EXT_CFG_CR_STEP_M_1:{
			CAMDRV_DEBUG("EXT_CFG_CR_STEP_M_1");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_contrast_m_1);
		}
		break;

	case EXT_CFG_CR_STEP_0:{
			CAMDRV_DEBUG("EXT_CFG_CR_STEP_0");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_contrast_0);
		}
		break;

	case EXT_CFG_CR_STEP_P_1:{
			CAMDRV_DEBUG("EXT_CFG_CR_STEP_P_1");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_contrast_p_1);
		}
		break;

	case EXT_CFG_CR_STEP_P_2:{
			CAMDRV_DEBUG("EXT_CFG_CR_STEP_P_2");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_contrast_p_2);
		}
		break;

	default:{
		pr_info("<=PCAM=> Unexpected EXT_CFG_CR_CONTROL mode : %d\n",
			value);
		}
		break;
	}
}

void s5k5ccaf_saturation_control(char value)
{
	switch (value) {
	case EXT_CFG_SA_STEP_M_2:{
			CAMDRV_DEBUG("EXT_CFG_SA_STEP_M_2");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_saturation_m_2);
		}
		break;

	case EXT_CFG_SA_STEP_M_1:{
			CAMDRV_DEBUG("EXT_CFG_SA_STEP_M_1");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_saturation_m_1);
		}
		break;

	case EXT_CFG_SA_STEP_0:{
			CAMDRV_DEBUG("EXT_CFG_SA_STEP_0");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_saturation_0);
		}
		break;

	case EXT_CFG_SA_STEP_P_1:{
			CAMDRV_DEBUG("EXT_CFG_SA_STEP_P_1");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_saturation_p_1);
		}
		break;

	case EXT_CFG_SA_STEP_P_2:{
			CAMDRV_DEBUG("EXT_CFG_SA_STEP_P_2");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_saturation_p_2);
		}
		break;

	default:{
		pr_info("<=PCAM=> Unexpected EXT_CFG_SA_CONTROL mode : %d\n",
			value);
		}
		break;
	}

}

void s5k5ccaf_sharpness_control(char value)
{
	switch (value) {
	case EXT_CFG_SP_STEP_M_2:{
			CAMDRV_DEBUG("EXT_CFG_SP_STEP_M_2");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_sharpness_m_2);
		}
		break;

	case EXT_CFG_SP_STEP_M_1:{
			CAMDRV_DEBUG("EXT_CFG_SP_STEP_M_1");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_sharpness_m_1);
		}
		break;

	case EXT_CFG_SP_STEP_0:{
			CAMDRV_DEBUG("EXT_CFG_SP_STEP_0");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_sharpness_0);
		}
		break;

	case EXT_CFG_SP_STEP_P_1:{
			CAMDRV_DEBUG("EXT_CFG_SP_STEP_P_1");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_sharpness_p_1);
		}
		break;

	case EXT_CFG_SP_STEP_P_2:{
			CAMDRV_DEBUG("EXT_CFG_SP_STEP_P_2");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_sharpness_p_2);
		}
		break;

	default:{
		pr_info("<=PCAM=> Unexpected EXT_CFG_SP_CONTROL mode : %d\n",
			value);
		}
		break;
	}
}

int s5k5ccaf_af_control(char value)
{
	switch (value) {

	case EXT_CFG_AF_CHECK_STATUS:{
			unsigned short af_status;
			CAMDRV_DEBUG("EXT_CFG_AF_CHECK_STATUS\n");
			switch (af_status) {
			case 0:
			case 3:
			case 4:
			case 6:
			case 8:
				return EXT_CFG_AF_LOWCONF;
			case 1:
				return EXT_CFG_AF_PROGRESS;
			case 2:
				return EXT_CFG_AF_SUCCESS;
			case 9:
				return EXT_CFG_AF_CANCELED;
			case 10:
				return EXT_CFG_AF_TIMEOUT;
			default:
				return af_status;

			}
		}
		break;

	case EXT_CFG_AF_OFF:{
			CAMDRV_DEBUG("EXT_CFG_AF_OFF");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_af_off);
		}
		break;

	case EXT_CFG_AF_SET_NORMAL:{
			CAMDRV_DEBUG("EXT_CFG_AF_SET_NORMAL");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_af_normal_on);
		}
		break;

	case EXT_CFG_AF_SET_MACRO:{
			CAMDRV_DEBUG("EXT_CFG_AF_SET_MACRO");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_af_macro_on);
		}
		break;

	case EXT_CFG_AF_DO:{
			S5K5CCAF_WRITE_LIST(s5k5ccaf_af_do);
		}
		break;

	case EXT_CFG_AF_ABORT:{
			CAMDRV_DEBUG("EXT_CFG_AF_ABORT");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_af_abort);
		}
		break;

	case EXT_CFG_AF_CHECK_2nd_STATUS:{
		}
		break;

	default:{
			pr_info("<=PCAM=> unexpected AF command : %d\n", value);
		}
		break;

	}
	return 0;
}

void s5k5ccaf_DTP_control(char value)
{
	switch (value) {
	case EXT_CFG_DTP_OFF:{
			CAMDRV_DEBUG("DTP OFF");
			/*S5K5CCAF_WRITE_LIST(s5k5ccaf_dtp_off); */
			s5k5ccaf_reset_power();
			S5K5CCAF_WRITE_LIST_BURST(s5k5ccaf_pre_init0);
			first_start_camera = 1;
			s5k5ccaf_set_preview();
		}
		break;

	case EXT_CFG_DTP_ON:{
			CAMDRV_DEBUG("DTP ON");
			S5K5CCAF_WRITE_LIST(s5k5ccaf_DTP_init0);
		}
		break;

	default:{
			pr_info("<=PCAM=> unexpected DTP control on PCAM\n");
		}
		break;
	}
}

static int s5k5ccaf_sensor_ext_config(void __user *arg)
{
	long rc = 0;
	ioctl_pcam_info_8bit ctrl_info;

	if (copy_from_user
		((void *)&ctrl_info, (const void *)arg, sizeof(ctrl_info))) {
		pr_info("<=PCAM=> %s fail copy_from_user!\n", __func__);
	}

	pr_info("[s5k5ccaf] sensor_ext_config %d %d %d %d %d\n",
		ctrl_info.mode,
		ctrl_info.address, ctrl_info.value_1, ctrl_info.value_2,
		ctrl_info.value_3);

	switch (ctrl_info.mode) {
	case EXT_CFG_GET_INFO:{
			unsigned short lsb, msb, rough_iso;
		}
		break;

	case EXT_CFG_FRAME_CONTROL:
		{
			switch (ctrl_info.value_1) {
			case EXT_CFG_FRAME_AUTO:{
					CAMDRV_DEBUG("EXT_CFG_FRAME_AUTO");
				}
				break;

			case EXT_CFG_FRAME_FIX_15:{
					CAMDRV_DEBUG("EXT_CFG_FRAME_FIX_15");
					S5K5CCAF_WRITE_LIST(s5k5ccaf_fps_15fix);
				}
				break;

			case EXT_CFG_FRAME_FIX_24:{
					CAMDRV_DEBUG("EXT_CFG_FRAME_FIX_24");
/*S5K5CCAF_WRITE_LIST(s5k5ccaf_24_fixed_fps); */
				}
				break;

			case EXT_CFG_FRAME_FIX_30:{
					CAMDRV_DEBUG("EXT_CFG_FRAME_FIX_30");
					S5K5CCAF_WRITE_LIST(s5k5ccaf_fps_30fix);
				}
				break;

			default:{
				pr_info("<=PCAM=> Unexpected EXT_CFG_FRAME_CONTROL mode : %d\n",
					ctrl_info.value_1);
				}
				break;

			}
		}
		break;

	case EXT_CFG_AF_CONTROL:
		{
			mAfMode = ctrl_info.value_1;
			ctrl_info.value_3 = s5k5ccaf_af_control(mAfMode);
		}
		break;

		/*end of EXT_CFG_EFFECT_CONTROL */
	case EXT_CFG_EFFECT_CONTROL:
		{
			mEffect = ctrl_info.value_1;
			s5k5ccaf_effect_control(mEffect);

		}
		break;

		/*end of EXT_CFG_WB_CONTROL */
	case EXT_CFG_WB_CONTROL:
		{
			mWhiteBalance = ctrl_info.value_1;
			s5k5ccaf_whitebalance_control(mWhiteBalance);

		}
		break;

		/*end of EXT_CFG_BR_CONTROL */
	case EXT_CFG_BR_CONTROL:
		{
			mBrightness = ctrl_info.value_1;
			if (mInit)
				s5k5ccaf_brightness_control(mBrightness);

		}
		break;

	case EXT_CFG_ISO_CONTROL:
		{
			mISO = ctrl_info.value_1;
			s5k5ccaf_iso_control(mISO);

		}
		break;

		/*end of case */
	case EXT_CFG_METERING_CONTROL:
		{
			mAutoExposure = ctrl_info.value_1;
			s5k5ccaf_metering_control(mAutoExposure);

		}
		break;

	case EXT_CFG_SCENE_CONTROL:
		{
			mScene = ctrl_info.value_1;
			s5k5ccaf_scene_control(mScene);

		}
		break;

	case EXT_CFG_AE_AWB_CONTROL:
		{
			switch (ctrl_info.value_1) {
			case EXT_CFG_AE_LOCK:
				CAMDRV_DEBUG("EXT_CFG_AE_LOCK");
				S5K5CCAF_WRITE_LIST(s5k5ccaf_ae_lock);
				break;

			case EXT_CFG_AE_UNLOCK:
				CAMDRV_DEBUG("EXT_CFG_AE_UNLOCK");
				S5K5CCAF_WRITE_LIST(s5k5ccaf_ae_unlock);
				break;

			case EXT_CFG_AWB_LOCK:
				CAMDRV_DEBUG("EXT_CFG_AWB_LOCK");
				S5K5CCAF_WRITE_LIST(s5k5ccaf_awb_lock);
				break;

			case EXT_CFG_AWB_UNLOCK:
				CAMDRV_DEBUG("EXT_CFG_AWB_UNLOCK");
				S5K5CCAF_WRITE_LIST(s5k5ccaf_awb_unlock);
				break;

			case EXT_CFG_AE_AWB_LOCK:{
					CAMDRV_DEBUG("EXT_CFG_AWB_AE_LOCK");

					if (mWhiteBalance == 0) {
						S5K5CCAF_WRITE_LIST
						    (s5k5ccaf_ae_lock);
						S5K5CCAF_WRITE_LIST
						    (s5k5ccaf_awb_lock);
					} else {
						S5K5CCAF_WRITE_LIST
						    (s5k5ccaf_ae_lock);
					}
				}
				break;

			case EXT_CFG_AE_AWB_UNLOCK:{
					CAMDRV_DEBUG("EXT_CFG_AWB_AE_UNLOCK");
					if (mWhiteBalance == 0) {
						S5K5CCAF_WRITE_LIST
						    (s5k5ccaf_ae_unlock);
						S5K5CCAF_WRITE_LIST
						    (s5k5ccaf_awb_unlock);
					} else {
						S5K5CCAF_WRITE_LIST
						    (s5k5ccaf_ae_unlock);
					}
				}
				break;

			default:{
					pr_info("<=PCAM=> Unexpected AWB_AE mode : %d\n",
						ctrl_info.value_1);
				}
				break;

			}
		}
		break;

	case EXT_CFG_CR_CONTROL:
		{
			mContrast = ctrl_info.value_1;
			if (mInit)
				s5k5ccaf_contrast_control(mContrast);

		}
		break;

	case EXT_CFG_SA_CONTROL:
		{
			mSaturation = ctrl_info.value_1;
			if (mInit)
				s5k5ccaf_saturation_control(mSaturation);

		}
		break;

	case EXT_CFG_SP_CONTROL:
		{
			mSharpness = ctrl_info.value_1;
			if (mInit)
				s5k5ccaf_sharpness_control(mSharpness);

		}
		break;

	case EXT_CFG_CPU_CONTROL:
		{
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
			} else {
				s5k5ccaf_DTP_control(ctrl_info.value_1);

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

			pr_info("<=PCAM=> check current module status : %x\n",
				ctrl_info.value_3);
			pr_info("<=PCAM=> PINON/OFF : %d\n", gpio_get_value(0));
		}
		break;

	default:{
		pr_info("<=PCAM=> Unexpected mode on sensor_rough_control : %d\n",
				ctrl_info.mode);
		}
		break;
	}
/*
if (copy_to_user
((void *)arg, (const void *)&ctrl_info, sizeof(ctrl_info)))	{
pr_info("<=PCAM=> %s fail on copy_to_user!\n", __func__);
} */

	if (copy_to_user
	    ((void *)arg, (const void *)&ctrl_info, sizeof(ctrl_info))) {
		pr_info("<=PCAM=> %s fail on copy_to_user!\n", __func__);
	}
	return rc;
}

int s5k5ccaf_mipi_mode(int mode)
{
	int rc = 0;
	struct msm_camera_csi_params s5k5ccaf_csi_params;

	CAMDRV_DEBUG("%s E", __func__);

	if (!config_csi2) {
		s5k5ccaf_csi_params.lane_cnt = 1;
		s5k5ccaf_csi_params.data_format = CSI_8BIT;
		s5k5ccaf_csi_params.lane_assign = 0xe4;
		s5k5ccaf_csi_params.dpcm_scheme = 0;
		s5k5ccaf_csi_params.settle_cnt = 0x14;	/*lyon.cho 24->0x14 */
		rc = msm_camio_csi_config(&s5k5ccaf_csi_params);
		if (rc < 0)
			printk(KERN_ERR "config csi controller failed\n");
		config_csi2 = 1;
	}
	CAMDRV_DEBUG("%s X", __func__);
	return rc;
}

static void s5k5ccaf_set_preview(void)
{
	int rc = 0;
/*	unsigned short read_value;*/
	unsigned short id = 0;
	int first_awb_delay = 0;

	CAMDRV_DEBUG("%s E", __func__);
	CAMDRV_DEBUG("I2C address : 0x%x", s5k5ccaf_client->addr);

	s5k5ccaf_mipi_mode(1);
	msleep(100);		/* => Please add some delay */

	if (first_start_camera) {
		S5K5CCAF_WRITE_LIST_BURST(s5k5ccaf_init0);
		usleep(10000);
		first_start_camera = 0;
		mInit = 1;
		first_awb_delay = 1;
	}

	if (mDTP == 1) {
		S5K5CCAF_WRITE_LIST(s5k5ccaf_dtp_on);
	} else {
		S5K5CCAF_WRITE_LIST(s5k5ccaf_update_preview_setting);

		s5k5ccaf_scene_control(mScene);

		if (mScene == EXT_CFG_SCENE_OFF) {
			s5k5ccaf_effect_control(mEffect);
			s5k5ccaf_whitebalance_control(mWhiteBalance);
			s5k5ccaf_iso_control(mISO);
			s5k5ccaf_metering_control(mAutoExposure);
			s5k5ccaf_contrast_control(mContrast);
			s5k5ccaf_saturation_control(mSaturation);
			s5k5ccaf_sharpness_control(mSharpness);
			s5k5ccaf_brightness_control(mBrightness);
			/*s5k5ccaf_af_control(mAfMode); */
		}
		pr_info("%camera write success");

	}
}

void s5k5ccaf_set_capture(void)
{
	/*unsigned short cur_lux = s5k5ccaf_get_lux(); */

	pr_info("s5k5ccaf_set_capture");
	CAMDRV_DEBUG("Normal Snapshot!\n");
	pr_info("Normal Snapshot!\n");
		S5K5CCAF_WRITE_LIST(s5k5ccaf_snapshot);
}

static long s5k5ccaf_set_sensor_mode(int mode)
{
	CAMDRV_DEBUG("s5k5ccaf_set_sensor_mode : %d", mode);
	switch (mode) {
	case SENSOR_PREVIEW_MODE:
		s5k5ccaf_set_preview();
		break;
	case SENSOR_SNAPSHOT_MODE:
		s5k5ccaf_set_capture();
		break;
	case SENSOR_RAW_SNAPSHOT_MODE:
		CAMDRV_DEBUG("RAW_SNAPSHOT NOT SUPPORT!!");
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static void s5k5ccaf_reset_power(void)
{
	s5k5ccaf_set_power(0);
	/* add delay */
	usleep(10000);
	s5k5ccaf_set_power(1);
}

void s5k5ccaf_set_power(int status)
{
	int rc;
	unsigned int mclk_cfg;
	struct clk *clk = NULL;

	if (status == 1) {	/*POWER ON */
		CAMDRV_DEBUG("s5k5ccaf_set_power : POWER ON");
		cam_ldo_power_on();

		/* Enable MCLK */
		/*msm_camio_clk_enable(CAMIO_CAM_MCLK_CLK);*/
		gpio_tlmm_config(GPIO_CFG
				 (15, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL,
				  GPIO_CFG_16MA), GPIO_CFG_ENABLE);
		msm_camio_clk_rate_set(24000000);
		udelay(10);	/* >10us (temp) */

		gpio_set_value(96, 1);	/*standby */
		usleep(11000);

		gpio_set_value(85, 1);	/*reset */
		udelay(10);

	} else {		/*POWER OFF */
		CAMDRV_DEBUG("s5k5ccaf_set_power : POWER OFF");

		gpio_set_value_cansleep(85, LOW);
		udelay(10);	/* >50us */
		msm_camio_clk_disable(CAMIO_CAM_MCLK_CLK);
		udelay(10);

		gpio_set_value_cansleep(96, LOW);
		udelay(10);
		cam_ldo_power_off();
	}

}

static int s5k5ccaf_check_sensor_id(void)
{
	int rc = 0;
	int i = 0;
	int id = 0;

	pr_info("[S5K5CCGX] s5k5ccaf_check_sensor_id()\n");

	/* read firmware id */
	return rc;
}

#ifdef CONFIG_LOAD_FILE

#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>

#include <linux/uaccess.h>

static char *s5k5ccaf_regs_table;

static int s5k5ccaf_regs_table_size;

void s5k5ccaf_regs_table_init(void)
{
	struct file *filp;
	char *dp;
	long l;
	loff_t pos;
/*	int i;*/
	int ret;
	mm_segment_t fs = get_fs();

	pr_info("%s %d\n", __func__, __LINE__);

	set_fs(get_ds());

	filp =
	open_namei("/mnt/sdcard/external_sd/sr300pc20r1_mipi_0826.h",
	O_RDONLY, 0);

	if (IS_ERR(filp)) {
		pr_info("file open error\n");
		return;
	}
	l = filp->f_path.dentry->d_inode->i_size;
	pr_info("l = %ld\n", l);
	dp = kmalloc(l, GFP_KERNEL);
	if (dp == NULL) {
		pr_info("Out of Memory\n");
		filp_close(filp, current->files);
	}
	pos = 0;
	memset(dp, 0, l);
	ret = vfs_read(filp, (char __user *)dp, l, &pos);
	if (ret != l) {
		pr_info("Failed to read file ret = %d\n", ret);
		kfree(dp);
		filp_close(filp, current->files);
		return;
	}

	filp_close(filp, current->files);

	set_fs(fs);

	s5k5ccaf_regs_table = dp;

	s5k5ccaf_regs_table_size = l;

	*((s5k5ccaf_regs_table + s5k5ccaf_regs_table_size) - 1) = '\0';

/*	printk("s5k5ccaf_regs_table 0x%x, %ld\n", dp, l);*/
}

void s5k5ccaf_regs_table_exit(void)
{
	pr_info("%s %d\n", __func__, __LINE__);
}

static int s5k5ccaf_is_hexnum(char *num)
{
	int i;
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

static int s5k5ccaf_regs_table_write(char *name)
{
	char *start, *end, *reg, *temp;	/*, *data; */
	unsigned short addr, value;
	unsigned long data;
	char data_buf[7];

	addr = value = 0;

	*(data_buf + 6) = '\0';

	start = strnstr(s5k5ccaf_regs_table, name);

	end = strnstr(start, "};");

	while (1) {
		/* Find Address */
		reg = strnstr(start, "0x");
		if (reg)
			start = (reg + 6);

		if ((reg == NULL) || (reg > end)) {
			pr_info("[s5k5ccaf] write end of %s\n", name);
			break;
		}
		/* Write Value to Address */
		memcpy(data_buf, reg, 6);

		if (s5k5ccaf_is_hexnum(data_buf) == 0) {
			pr_info("[s5k5ccaf] it's not hex number %s\n",
				data_buf);
			continue;
		}

		data = (unsigned long)kstrtoul(data_buf, NULL, 16);
		/*printk("data 0x%08x\n", data); */
		addr = (data >> 8);
		value = (data & 0xff);
		if (addr == 0xff) {
			msleep(value);
			pr_info("delay 0x%04x, value 0x%04x\n", addr, value);
		} else {
			if (s5k5ccaf_sensor_write(addr, value) < 0) {
				pr_info
					("[s5k5ccaf] %s fail on sensor_write :
					addr[0x%04x],
					value[0x%04x]\n",
					__func__, addr, value);
			}
			pr_info
			    ("[s5k5ccaf] %s success on sensor_write :
			    addr[0x%04x],
			    value[0x%04x]\n",
			    __func__, addr, value);
		}
	}

	return 0;
}
#endif

int s5k5ccaf_sensor_init(const struct msm_camera_sensor_info *data)
{
	int rc = 0;
/*	int i = 0;*/

	s5k5ccaf_ctrl = kzalloc(sizeof(struct s5k5ccaf_ctrl), GFP_KERNEL);
	if (!s5k5ccaf_ctrl) {
		CDBG("s5k5ccaf_init failed!\n");
		rc = -ENOMEM;
		goto init_done;
	}

	if (data)
		s5k5ccaf_ctrl->sensordata = data;

	first_start_camera = 1;
	config_csi2 = 0;
#ifdef CONFIG_LOAD_FILE
	s5k5ccaf_regs_table_init();
#endif
	s5k5ccaf_set_power(1);
	usleep(10000);

	rc = s5k5ccaf_check_sensor_id();
	if (rc < 0) {
		CDBG("s5k5ccaf_sensor_init failed!\n");
		goto init_fail;
	}
	S5K5CCAF_WRITE_LIST(s5k5ccaf_pre_init0);

init_done:
	return rc;

init_fail:
	kfree(s5k5ccaf_ctrl);
	return rc;
}

static int s5k5ccaf_init_client(struct i2c_client *client)
{
	/* Initialize the MSM_CAMI2C Chip */
	init_waitqueue_head(&s5k5ccaf_wait_queue);
	return 0;
}

int s5k5ccaf_sensor_config(void __user *argp)
{
	struct sensor_cfg_data cfg_data;
	long rc = 0;

	pr_info("s5k5ccaf_sensor_config\n");

	if (copy_from_user(&cfg_data,
			   (void *)argp, sizeof(struct sensor_cfg_data)))
		return -EFAULT;

	/* down(&s5k5ccaf_sem); */

	CDBG("s5k5ccaf_ioctl, cfgtype = %d, mode = %d\n",
	     cfg_data.cfgtype, cfg_data.mode);

	switch (cfg_data.cfgtype) {
	case CFG_SET_MODE:
		rc = s5k5ccaf_set_sensor_mode(cfg_data.mode);
		break;

	case CFG_SET_EFFECT:
/*rc = s5k5ccaf_set_effect(cfg_data.mode, cfg_data.cfg.effect); */
		break;

	case CFG_GET_AF_MAX_STEPS:
	default:
		rc = -EINVAL;
		pr_info("s5k5ccaf_sensor_config : Invalid cfgtype ! %d\n",
		       cfg_data.cfgtype);
		break;
	}

	/* up(&s5k5ccaf_sem); */

	return rc;
}

int s5k5ccaf_sensor_release(void)
{
	int rc = 0;
	/*int *switch_i2c_addr; //TEMP Dirty Code, Do not use it! */

	first_start_camera = 1;
	set_init0 = 0;
	/*If did not init below that, it can keep the previous status.
	*it depend on concept by PCAM
	*/
	mEffect = EXT_CFG_EFFECT_NORMAL;
	mBrightness = EXT_CFG_BR_STEP_0;
	mContrast = EXT_CFG_CR_STEP_0;
	mSaturation = EXT_CFG_SA_STEP_0;
	mSharpness = EXT_CFG_SP_STEP_0;
	mWhiteBalance = EXT_CFG_WB_AUTO;
	mISO = EXT_CFG_ISO_AUTO;
	mAutoExposure = EXT_CFG_METERING_NORMAL;
	mScene = EXT_CFG_SCENE_OFF;
	/*mAfMode = 0; */
	mDTP = 0;
	mInit = 0;

	/*TEMP Dirty Code, Do not use it! PCAM...
	 *switch_i2c_addr = 0xd500c010;
	 *writel(1, switch_i2c_addr);
	 */

	if (mAfMode == EXT_CFG_AF_SET_MACRO) {
		pr_info("[S5K5CCAF] wait change lens position\n");
		s5k5ccaf_af_control(EXT_CFG_AF_SET_NORMAL);
		msleep(150);
	} else
		pr_info("[S5K5CCAF] mAfMode : %d\n", mAfMode);

	pr_info("[S5K5CCAF] s5k5ccaf_sensor_release\n");

	kfree(s5k5ccaf_ctrl);
	/* up(&s5k5ccaf_sem); */

#ifdef CONFIG_LOAD_FILE
	s5k5ccaf_regs_table_exit();
#endif

	s5k5ccaf_set_power(0);
	return rc;
}

static int s5k5ccaf_i2c_probe(struct i2c_client *client,
			      const struct i2c_device_id *id)
{
	int rc = 0;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		rc = -ENOTSUPP;
		goto probe_failure;
	}

	s5k5ccaf_sensorw = kzalloc(sizeof(struct s5k5ccaf_work), GFP_KERNEL);

	if (!s5k5ccaf_sensorw) {
		rc = -ENOMEM;
		goto probe_failure;
	}

	i2c_set_clientdata(client, s5k5ccaf_sensorw);
	s5k5ccaf_init_client(client);
	s5k5ccaf_client = client;

	CDBG("s5k5ccaf_probe succeeded!\n");

	return 0;

probe_failure:
	kfree(s5k5ccaf_sensorw);
	s5k5ccaf_sensorw = NULL;
	CDBG("s5k5ccaf_probe failed!\n");
	return rc;
}

static const struct i2c_device_id s5k5ccaf_i2c_id[] = {
	{"s5k5ccaf", 0},
	{},
};

static struct i2c_driver s5k5ccaf_i2c_driver = {
	.id_table = s5k5ccaf_i2c_id,
	.probe = s5k5ccaf_i2c_probe,
	.remove = __exit_p(s5k5ccaf_i2c_remove),
	.driver = {
		   .name = "s5k5ccaf",
		   },
};

static int s5k5ccaf_sensor_probe(const struct msm_camera_sensor_info *info,
				 struct msm_sensor_ctrl *s)
{
/*	unsigned short read_value;*/
	unsigned short id = 0;

	int rc = i2c_add_driver(&s5k5ccaf_i2c_driver);
	if (rc < 0 || s5k5ccaf_client == NULL) {
		rc = -ENOTSUPP;
		goto probe_done;
	}
	s5k5ccaf_set_power(1);
/*	usleep(10000);

	rc = s5k5ccaf_check_sensor_id();
	if (rc < 0) {
		CDBG("s5k5ccaf_sensor_init failed!\n");
		goto probe_done;
	}
*/
	s->s_init = s5k5ccaf_sensor_init;
	s->s_release = s5k5ccaf_sensor_release;
	s->s_config = s5k5ccaf_sensor_config;
	s->s_ext_config = s5k5ccaf_sensor_ext_config;

	s->s_camera_type = BACK_CAMERA_2D;
	s->s_mount_angle = 90;

probe_done:

	s5k5ccaf_set_power(0);

	pr_info("%s %s:%d\n", __FILE__, __func__, __LINE__);
	return rc;
}

static int __s5k5ccaf_probe(struct platform_device *pdev)
{
	pr_info("############# S5K5CCAF probe ##############\n");
	return msm_camera_drv_start(pdev, s5k5ccaf_sensor_probe);
}

static struct platform_driver msm_camera_driver = {
	.probe = __s5k5ccaf_probe,
	.driver = {
		   .name = "msm_camera_s5k5ccaf",
		   .owner = THIS_MODULE,
		   },
};

static int __init s5k5ccaf_init(void)
{
	return platform_driver_register(&msm_camera_driver);
}

module_init(s5k5ccaf_init);
