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
#include <mach/pmic.h>

#include <mach/camera.h>
#include <mach/vreg.h>
#include <linux/io.h>

/*#include "msm_camera_gpio.h"*/
#if (CONFIG_MACH_JENA_HWREV == 0x0)
#include "mt9v113_jena.h"
#else
#include "mt9v113.h"
#endif

#define SENSOR_DEBUG 0

#if (CONFIG_MACH_TREBON_HWREV == 0x0) || (CONFIG_MACH_GEIM_HWREV == 0x0)
#define CAM_FLASH_EN    58
#define CAM_VGA_STBY	18
#define CAM_VGA_RST	    98
#else
#define CAM_FLASH_EN    92
#define CAM_VGA_STBY	93
#define CAM_VGA_RST	    89
#endif
#define CAM_FLASH_MODE  84	/*4*/
#define CAM_MEGA_STBY	96	/*4*/
#define CAM_MEGA_RST	85	/*4*/
#define CAM_MEGA_AF_EN  49
#define CAM_GPIO_SCL	60
#define CAM_GPIO_SDA	61

/*#define CONFIG_LOAD_FILE*/

static char mt9v113_sensor_init_done = 1;

/*Gopeace LeeSangmin DJ26 add*/
static unsigned short mt9v113_sensor_version = 0xff;

struct mt9v113_work {
	struct work_struct work;
};

static struct mt9v113_work *mt9v113_sensorw;
static struct i2c_client *mt9v113_client;

struct mt9v113_ctrl {
	const struct msm_camera_sensor_info *sensordata;

};

/*Gopeace LeeSangmin Add*/
struct mt9v113_data_save {
	char mEffect;
	char mBrightness;
	char mContrast;
	char mSaturation;
	char mSharpness;
	char mWhiteBalance;
	char mISO;
	char mAutoExposure;
	/*char mScene; */
	char mDTP;
	char mInit;
	char mPrettyEffect;
	char mVtMode;
	char mFPS;
};

static struct mt9v113_ctrl *mt9v113_ctrl;
static struct mt9v113_data_save mt9v113_data;

static DECLARE_WAIT_QUEUE_HEAD(mt9v113_wait_queue);
/*DECLARE_MUTEX(mt9v113_sem);*/
static int16_t mt9v113_effect = CAMERA_EFFECT_OFF;

/*=============================================================
	EXTERNAL DECLARATIONS
==============================================================*/
/*=============================================================*/

#ifdef CONFIG_LOAD_FILE
static int mt9v113_regs_table_write(char *name);
#endif

/**
 * mt9v113_i2c_read: Read (I2C) multiple bytes to the camera sensor
 * @client: pointer to i2c_client
 * @cmd: command register
 * @data: data to be read
 *
 * Returns 0 on success, <0 on error
 */

static int mt9v113_sensor_read(unsigned short subaddr, unsigned short *data)
{
	int ret;
	unsigned char buf[2];
	struct i2c_msg msg = { mt9v113_client->addr, 0, 2, buf };

	buf[0] = (subaddr >> 8);
	buf[1] = (subaddr & 0xFF);

	ret = i2c_transfer(mt9v113_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
	if (ret == -EIO)
		goto error;

	msg.flags = I2C_M_RD;

	ret = i2c_transfer(mt9v113_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
	if (ret == -EIO)
		goto error;

	*data = ((buf[0] << 8) | buf[1]);

error:
	return ret;
}

static int mt9v113_sensor_write(unsigned short subaddr, unsigned short val)
{
	unsigned char buf[4];
	struct i2c_msg msg = { mt9v113_client->addr, 0, 4, buf };

	/*pr_info("[PGH] on write func mt9v113_client->addr : %x\n",
	   mt9v113_client->addr);
	   pr_info("[PGH] on write func mt9v113_client->adapter->nr : %d\n",
	   mt9v113_client->adapter->nr);

	   CAMDRV_DEBUG("[PGH] on write func subaddr:%x, val:%x\n",
	   subaddr, val); */

	buf[0] = (subaddr >> 8);
	buf[1] = (subaddr & 0xFF);
	buf[2] = (val >> 8);
	buf[3] = (val & 0xFF);

	return i2c_transfer(mt9v113_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
}

static int mt9v113_sensor_polling(void)
{
	int i = 0;
	unsigned short data;
	CAMDRV_DEBUG("mt9v113_sensor_polling start");

	for (i = 0; i < 10; i++) {
		msleep(30);
		mt9v113_sensor_write(0x098C, 0xA103);
		mt9v113_sensor_read(0x0990, &data);
		if (data == 0)
			break;
	}

	CAMDRV_DEBUG("mt9v113_sensor_polling end");
}

static int mt9v113_sensor_write_list(const unsigned long *list, int size,
				     char *name)
{
	int ret, i;
	unsigned short subaddr;
	unsigned short value;

	ret = 0;
#ifdef CONFIG_LOAD_FILE
	ret = mt9v113_regs_table_write(name);
#else

	pr_info("mt9v113_sensor_write_list : %s\n", name);

	for (i = 0; i < size; i++) {
		/*CAMDRV_DEBUG("[PGH] %x      %x\n",
		   list[i].subaddr, list[i].value); */
		subaddr = ((list[i]) >> 16);	/*address */
		value = ((list[i]) & 0xFFFF);	/*value */
		if (subaddr == 0xffff) {
			if (value == 0xffff) {
				mt9v113_sensor_polling();
			} else {
				CAMDRV_DEBUG("SETFILE DELAY : %dms", value);
				msleep(value);
			}
		} else {
			if (mt9v113_sensor_write(subaddr, value) < 0) {
				pr_info("sensor_write_list fail-_-\n");
				return -EIO;
			}
		}
	}
#endif
	return ret;
}

void mt9v113_effect_control(char value)
{
}

void mt9v113_pretty_control(char value)
{

}

void mt9v113_VT_pretty_control(char value)
{
	switch (value) {
	case EXT_CFG_PRETTY_LEVEL_0:
	{
		CAMDRV_DEBUG("EXT_CFG_PRETTY_LEVEL_0");
		mt9v113_sensor_write_list(mt9v113_VT_Pretty_Level_0,
			MT9V113_VT_PRETTY_LEVEL_0_REGS,
			"mt9v113_VT_Pretty_Level_0");
	}
	break;
	case EXT_CFG_PRETTY_LEVEL_1:
	{
		CAMDRV_DEBUG("EXT_CFG_PRETTY_LEVEL_1");
		mt9v113_sensor_write_list(mt9v113_VT_Pretty_Level_1,
			MT9V113_VT_PRETTY_LEVEL_1_REGS,
			"mt9v113_VT_Pretty_Level_1");
	}
	break;
	case EXT_CFG_PRETTY_LEVEL_2:
	{
		CAMDRV_DEBUG("EXT_CFG_PRETTY_LEVEL_2");
		mt9v113_sensor_write_list(mt9v113_VT_Pretty_Level_2,
			MT9V113_VT_PRETTY_LEVEL_2_REGS,
			"mt9v113_VT_Pretty_Level_2");
	}
	break;
	case EXT_CFG_PRETTY_LEVEL_3:
	{
		CAMDRV_DEBUG("EXT_CFG_PRETTY_LEVEL_3");
		mt9v113_sensor_write_list(mt9v113_VT_Pretty_Level_3,
			MT9V113_VT_PRETTY_LEVEL_3_REGS,
			"mt9v113_VT_Pretty_Level_3");
	}
	break;
	default:
	{
		pr_info("<Unexpected Pretty Effect mode : %d\n", value);
	}
	break;
	}
}


void mt9v113_whitebalance_control(char value)
{

}

void mt9v113_VT_brightness_control(char value)
{
	pr_info("<=PCAM=> VT Brightness Control 0x%x\n", value);
	switch (value) {
	case EXT_CFG_BR_STEP_P_4:{
		CAMDRV_DEBUG("EXT_CFG_BR_STEP_P_4");
		mt9v113_sensor_write_list(mt9v113_VT_brightness_p_4,
		MT9V113_VT_BRIGHTNESS_P_4_REGS,\
		 "mt9v113_VT_brightness_p_4");
	}
	break;

	case EXT_CFG_BR_STEP_P_3:{
		CAMDRV_DEBUG("EXT_CFG_BR_STEP_P_3");
		mt9v113_sensor_write_list(mt9v113_VT_brightness_p_3,
			MT9V113_VT_BRIGHTNESS_P_3_REGS,\
		 "mt9v113_VT_brightness_p_3");
	}
	break;
	case EXT_CFG_BR_STEP_P_2:{
		CAMDRV_DEBUG("EXT_CFG_BR_STEP_P_2");
		mt9v113_sensor_write_list(mt9v113_VT_brightness_p_2,
			MT9V113_VT_BRIGHTNESS_P_2_REGS,\
		 "mt9v113_VT_brightness_p_2");
	}
	break;
	case EXT_CFG_BR_STEP_P_1:{
		CAMDRV_DEBUG("EXT_CFG_BR_STEP_P_1");
		mt9v113_sensor_write_list(mt9v113_VT_brightness_p_1,
			MT9V113_VT_BRIGHTNESS_P_1_REGS,\
		 "mt9v113_VT_brightness_p_1");
	}
	break;
	case EXT_CFG_BR_STEP_0:{
		CAMDRV_DEBUG("EXT_CFG_BR_STEP_0");
		mt9v113_sensor_write_list(mt9v113_VT_brightness_0,
			MT9V113_VT_BRIGHTNESS_0_REGS, \
		"mt9v113_VT_brightness_0");
	}
	break;
	case EXT_CFG_BR_STEP_M_1:{
		CAMDRV_DEBUG("EXT_CFG_BR_STEP_M_1");
		mt9v113_sensor_write_list(mt9v113_VT_brightness_m_1,
			MT9V113_VT_BRIGHTNESS_M_1_REGS, \
		"mt9v113_VT_brightness_m_1");
		}
	break;
	case EXT_CFG_BR_STEP_M_2:{
		CAMDRV_DEBUG("EXT_CFG_BR_STEP_M_2");
		mt9v113_sensor_write_list(mt9v113_VT_brightness_m_2,
			MT9V113_VT_BRIGHTNESS_M_2_REGS, \
		"mt9v113_VT_brightness_m_2");
		}
	break;
	case EXT_CFG_BR_STEP_M_3: {
		CAMDRV_DEBUG("EXT_CFG_BR_STEP_M_3");
		mt9v113_sensor_write_list(mt9v113_VT_brightness_m_3,
			MT9V113_VT_BRIGHTNESS_M_3_REGS, \
		"mt9v113_VT_brightness_m_3");
		}
		break;
	case EXT_CFG_BR_STEP_M_4:{
		CAMDRV_DEBUG("EXT_CFG_BR_STEP_M_4");
		mt9v113_sensor_write_list(mt9v113_VT_brightness_m_4,
			MT9V113_VT_BRIGHTNESS_M_4_REGS,
			"mt9v113_VT_brightness_m_4");
		}
		break;
		default:{
			pr_info("Unexpected _BR mode : %d\n",  value);
		}
		break;
	}
}


void mt9v113_brightness_control(char value)
{
	pr_info("<=PCAM=> Brightness Control 0x%x\n", value);

	switch (value) {
	case EXT_CFG_BR_STEP_P_4:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_P_4");
			mt9v113_sensor_write_list(mt9v113_brightness_p_4,
						  MT9V113_BRIGHTNESS_P_4_REGS,
						  "mt9v113_brightness_p_4");
		}
		break;

	case EXT_CFG_BR_STEP_P_3:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_P_3");
			mt9v113_sensor_write_list(mt9v113_brightness_p_3,
						  MT9V113_BRIGHTNESS_P_3_REGS,
						  "mt9v113_brightness_p_3");
		}
		break;

	case EXT_CFG_BR_STEP_P_2:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_P_2");
			mt9v113_sensor_write_list(mt9v113_brightness_p_2,
						  MT9V113_BRIGHTNESS_P_2_REGS,
						  "mt9v113_brightness_p_2");
		}
		break;

	case EXT_CFG_BR_STEP_P_1:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_P_1");
			mt9v113_sensor_write_list(mt9v113_brightness_p_1,
						  MT9V113_BRIGHTNESS_P_1_REGS,
						  "mt9v113_brightness_p_1");
		}
		break;

	case EXT_CFG_BR_STEP_0:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_0");
			mt9v113_sensor_write_list(mt9v113_brightness_0,
						  MT9V113_BRIGHTNESS_0_REGS,
						  "mt9v113_brightness_0");
		}
		break;

	case EXT_CFG_BR_STEP_M_1:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_M_1");
			mt9v113_sensor_write_list(mt9v113_brightness_m_1,
						  MT9V113_BRIGHTNESS_M_1_REGS,
						  "mt9v113_brightness_m_1");
		}
		break;

	case EXT_CFG_BR_STEP_M_2:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_M_2");
			mt9v113_sensor_write_list(mt9v113_brightness_m_2,
						  MT9V113_BRIGHTNESS_M_2_REGS,
						  "mt9v113_brightness_m_2");
		}
		break;

	case EXT_CFG_BR_STEP_M_3:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_M_3");
			mt9v113_sensor_write_list(mt9v113_brightness_m_3,
						  MT9V113_BRIGHTNESS_M_3_REGS,
						  "mt9v113_brightness_m_3");
		}
		break;

	case EXT_CFG_BR_STEP_M_4:{
			CAMDRV_DEBUG("EXT_CFG_BR_STEP_M_4");
			mt9v113_sensor_write_list(mt9v113_brightness_m_4,
						  MT9V113_BRIGHTNESS_M_4_REGS,
						  "mt9v113_brightness_m_4");
		}
		break;

	default:{
			pr_info("<=PCAM=> Unexpected BR mode : %d\n", value);
		}
		break;

	}

}

void mt9v113_FPS_control(char value)
{
	pr_info("mt9v113_FPS_control() value:%d", value);
	switch (value) {
	case EXT_CFG_FRAME_AUTO:
	{
		CAMDRV_DEBUG("EXT_CFG_FRAME_AUTO\n");
	}
	break;
	case EXT_CFG_FRAME_FIX_15:
	{
		CAMDRV_DEBUG("EXT_CFG_FRAME_FIX_15\n");
		mt9v113_sensor_write_list(mt9v113_15_fps,
			MT9V113_15_FPS_REGS, "mt9v113_15_fps");
	}
	break;
	case EXT_CFG_FRAME_FIX_7:
	{
		CAMDRV_DEBUG("EXT_CFG_FRAME_FIX_7\n");
		mt9v113_sensor_write_list(mt9v113_7_fps,
			MT9V113_7_FPS_REGS, "mt9v113_7_fps");
	}
	break;
	default:
	{
		CAMDRV_DEBUG("Unexpected mode : %d\n", value);
	}
	break;
	}
}


void mt9v113_VT_FPS_control(char value)
{
	pr_info("mt9v113_VT_FPS_control() value:%d", value);

	switch (value) {
	case EXT_CFG_FRAME_AUTO:
	{
		CAMDRV_DEBUG("EXT_CFG_FRAME_AUTO\n");
	}
	break;
	case EXT_CFG_FRAME_FIX_15:
	{
		CAMDRV_DEBUG("EXT_CFG_FRAME_FIX_15\n");
		mt9v113_sensor_write_list(mt9v113_VT_15_fps,
			MT9V113_VT_15_FPS_REGS, "mt9v113_15_fps");
	}
	break;
	case EXT_CFG_FRAME_FIX_7:
	{
		CAMDRV_DEBUG("EXT_CFG_FRAME_FIX_7\n");
		mt9v113_sensor_write_list(mt9v113_VT_7_fps,
			MT9V113_VT_7_FPS_REGS, "mt9v113_7_fps");
	}
	break;
	default:
	{
		CAMDRV_DEBUG("Unexpected mode : %d\n", value);
	}
	break;
	}
}



static void mt9v113_set_power(int status)
{

	struct vreg *vreg_ldo6, *vreg_ldo15, *vreg_ldo17;
	pr_info("mt9v113_set_power : POWER ON");

	vreg_ldo6 = vreg_get(NULL, "vcamc");
	vreg_ldo15 = vreg_get(NULL, "vcamio");
	vreg_ldo17 = vreg_get(NULL, "vcama");

	int value1, value2, value3, value4 = 0;

	if (status == 1) {	/*POWER ON */

		pr_info("mt9v113_set_power : POWER ON");
		/* gpio & pmic config */
		gpio_tlmm_config(GPIO_CFG
				 (CAM_MEGA_RST, 0, GPIO_CFG_OUTPUT,
				  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
				 GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG
				 (CAM_MEGA_STBY, 0, GPIO_CFG_OUTPUT,
				  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
				 GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG
				 (CAM_VGA_RST, 0, GPIO_CFG_OUTPUT,
				  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
				 GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG
				 (CAM_VGA_STBY, 0, GPIO_CFG_OUTPUT,
				  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
				 GPIO_CFG_ENABLE);

		gpio_set_value(CAM_MEGA_RST, 0);
		gpio_set_value(CAM_MEGA_STBY, 0);
		gpio_set_value(CAM_VGA_STBY, 0);
		gpio_set_value(CAM_VGA_RST, 0);
		usleep(3000);

		/*vreg_disable(vreg_ldo6); */
		vreg_disable(vreg_ldo15);
		vreg_disable(vreg_ldo17);

		gpio_set_value(CAM_MEGA_AF_EN, 0);
		gpio_set_value(CAM_FLASH_EN, 0);
		gpio_set_value(CAM_FLASH_MODE, 0);
		usleep(1000);

       /** Power sequence **/
		vreg_set_level(vreg_ldo15, 1800);	/*IO &VVT */
		vreg_set_level(vreg_ldo17, 2800);
		/*A*/ gpio_set_value(CAM_VGA_RST, 1);
		usleep(2000);
		vreg_enable(vreg_ldo15);	/*IO_EN & VGA_CORE */
		udelay(10);
		vreg_enable(vreg_ldo17);	/*A_EN */
		usleep(2000);	/* >1ms */

		gpio_set_value(CAM_VGA_RST, 0);
		udelay(50);	/* >50ms */

		/*msm_camio_clk_enable(CAMIO_CAM_MCLK_CLK); */
		gpio_tlmm_config(GPIO_CFG
				 (15, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL,
				  GPIO_CFG_16MA), GPIO_CFG_ENABLE);
		msm_camio_clk_rate_set(24000000);
		usleep(2000);	/* >10us (temp) */

		gpio_set_value(CAM_VGA_RST, 1);
		usleep(1000);	/* >1ms */


	} else {		/*POWER OFF */



		CAMDRV_DEBUG("mt9v113_set_power : POWER OFF");

		gpio_set_value(CAM_VGA_RST, 0);
		usleep(2000);	/* >50us */
		/* msm_camio_clk_disable(CAMIO_CAM_MCLK_CLK);
		   gpio_set_value(CAM_MEGA_STBY, 0);
		   gpio_set_value(CAM_VGA_RST, 0);
		   gpio_set_value(CAM_VGA_STBY, 0); */

		vreg_disable(vreg_get(NULL, "vcamio"));
		udelay(10);
		vreg_disable(vreg_get(NULL, "vcama"));
		usleep(1000);

		gpio_set_value(CAM_MEGA_AF_EN, 0);
		gpio_set_value(CAM_FLASH_EN, 0);
		gpio_set_value(CAM_FLASH_MODE, 0);
		usleep(1000);
		/* for sleep current
		   gpio_tlmm_config(GPIO_CFG(CAM_GPIO_SCL, 0, GPIO_CFG_OUTPUT,
		   GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		   gpio_tlmm_config(GPIO_CFG(CAM_GPIO_SDA, 0, GPIO_CFG_OUTPUT,
		   GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		   gpio_set_value(CAM_GPIO_SCL, 0);
		   gpio_set_value(CAM_GPIO_SDA, 0); */

	}

}


void mt9v113_DTP_control(char value)
{
	switch (value) {
	case EXT_CFG_DTP_OFF:
		{
			CAMDRV_DEBUG("DTP OFF");
			mt9v113_data.mDTP = 0;
			mt9v113_sensor_write_list(mt9v113_dtp_off,
						  MT9V113_DTP_OFF_REGS,
						  "mt9v113_dtp_off");
			/*mt9v113_reset(); */
		}
		break;

	case EXT_CFG_DTP_ON:
		{
			CAMDRV_DEBUG("DTP ON");
			mt9v113_data.mDTP = 1;
			mt9v113_sensor_write_list(mt9v113_dtp_on,
						  MT9V113_DTP_ON_REGS,
						  "mt9v113_dtp_on");
		}
		break;

	default:
		{
			pr_info("<=PCAM=> unexpected DTP control on PCAM\n");
		}
		break;
	}
}


int mt9v113_sensor_ext_config(void __user *arg)
{
	long rc = 0;

	ioctl_pcam_info_8bit ctrl_info;

	CAMDRV_DEBUG("START");

	if (copy_from_user
	    ((void *)&ctrl_info, (const void *)arg, sizeof(ctrl_info))) {
		pr_info("<=PCAM=> %s fail copy_from_user!\n", __func__);
	}

	pr_info("<=PCAM=> TEST %d %d %d %d %d\n", ctrl_info.mode,
		ctrl_info.address, ctrl_info.value_1, ctrl_info.value_2,
		ctrl_info.value_3);

	switch (ctrl_info.mode) {
	case EXT_CFG_GET_INFO:
		{
		}
		break;

	case EXT_CFG_VT_MODE_CONTROL:
		{
			if (1 == ctrl_info.value_1 || 2 == ctrl_info.value_1) {
				pr_info("EXT_CFG_VT_MODE\n");
				mt9v113_data.mVtMode = 1;
				/*mVtMode = 1;*/
			} else if (0 == ctrl_info.value_1) {
				pr_info("EXT_CFG_NORMAL_MODE\n");
				mt9v113_data.mVtMode = 0;
				/*mVtMode = 0;*/
			} else {
				CAMDRV_DEBUG("EXT_CFG_MODE CHANGE ERROR\n");
			}
		}
		break;

	case EXT_CFG_FRAME_CONTROL:
		{
			mt9v113_data.mFPS = ctrl_info.value_1;
			if (0 == mt9v113_data.mVtMode)
				mt9v113_FPS_control(mt9v113_data.mFPS);
			if (1 == mt9v113_data.mVtMode)
				mt9v113_VT_FPS_control(mt9v113_data.mFPS);
		}
		break;

	case EXT_CFG_EFFECT_CONTROL:
		{
			mt9v113_data.mEffect = ctrl_info.value_1;
			mt9v113_effect_control(mt9v113_data.mEffect);
		}		/*end of EXT_CFG_EFFECT_CONTROL */
		break;

	case EXT_CFG_WB_CONTROL:
		{
			mt9v113_data.mWhiteBalance = ctrl_info.value_1;
			mt9v113_whitebalance_control
			    (mt9v113_data.mWhiteBalance);
		}		/*end of EXT_CFG_WB_CONTROL */
		break;

	case EXT_CFG_BR_CONTROL:
		{
			mt9v113_data.mBrightness = ctrl_info.value_1;
			/*if(mInit) */
			if (0 == mt9v113_data.mVtMode)
				CAMDRV_DEBUG("hoon1 = %d\n", ctrl_info.value_1);
			mt9v113_brightness_control(mt9v113_data.mBrightness);
			if (1 == mt9v113_data.mVtMode)
				CAMDRV_DEBUG("hoon2 = %d\n", ctrl_info.value_1);
			mt9v113_VT_brightness_control(mt9v113_data.mBrightness);

		}		/*end of EXT_CFG_BR_CONTROL */
		break;

	case EXT_CFG_ISO_CONTROL:
		{
			mt9v113_data.mISO = ctrl_info.value_1;
			/*sensor_iso_control(mISO); */
		}
		break;

	case EXT_CFG_METERING_CONTROL:
		{
			mt9v113_data.mAutoExposure = ctrl_info.value_1;
			/*sensor_metering_control(mAutoExposure); */

		}		/*end of case */
		break;

	case EXT_CFG_AE_AWB_CONTROL:
		{
		}
		break;

	case EXT_CFG_PRETTY_CONTROL:
		{
			mt9v113_data.mPrettyEffect = ctrl_info.value_1;
			if (0 == mt9v113_data.mVtMode)
				mt9v113_pretty_control
				    (mt9v113_data.mPrettyEffect);
			if (1 == mt9v113_data.mVtMode)
				mt9v113_VT_pretty_control
				    (mt9v113_data.mPrettyEffect);
		}
		break;

	case EXT_CFG_CR_CONTROL:
		{
			mt9v113_data.mContrast = ctrl_info.value_1;
			/*if(mInit) */
			/*sensor_contrast_control(mContrast); */
		}
		break;

	case EXT_CFG_SA_CONTROL:
		{
			mt9v113_data.mSaturation = ctrl_info.value_1;
			/*if(mInit) */
			/*sensor_saturation_control(mSaturation); */
		}
		break;

	case EXT_CFG_SP_CONTROL:
		{
			mt9v113_data.mSharpness = ctrl_info.value_1;
			/*if(mInit) */
			/*sensor_sharpness_control(mSharpness); */
		}
		break;

	case EXT_CFG_CPU_CONTROL:
		{
		}

		break;

	case EXT_CFG_DTP_CONTROL:
		{
			mt9v113_DTP_control(ctrl_info.value_1);

			if (ctrl_info.value_1 == 0)
				ctrl_info.value_3 = 2;

			else if (ctrl_info.value_1 == 1)
				ctrl_info.value_3 = 3;
		}
		break;

	default:{
			pr_info
			    ("<=PCAM=> Unexpected\
			    mode on mt9v113_sensor_control : %d\n",\
			    ctrl_info.mode);
		}
		break;
	}

	if (copy_to_user
	    ((void *)arg, (const void *)&ctrl_info, sizeof(ctrl_info))) {
		pr_info("<=PCAM=> %s fail on copy_to_user!\n", __func__);
	}

	return rc;
}

/*===================================================================
FUNCTION      CAMSENSOR_mt9v113_CHECK_SENSOR_REV
===================================================================*/

static unsigned char mt9v113_check_sensor_rev(void)
{
	unsigned char id = 0xff;
	int ret0, ret1;

	mt9v113_sensor_version = 0xff;
/*
    ret0 = mt9v113_sensor_write( 0x03, 0x00);
	ret1 = mt9v113_sensor_read( 0x04, &id);

	pr_info("mt9v113_check_sensor_rev!!! ret0:
	%d ret1:%d\n",ret0,ret1);

    if( id == 0x8c )
    {
    pr_info("============================\n");
    pr_info("[cam] MT9V113\n");
    pr_info("============================\n");
	mt9v113_sensor_version = 0x8c;//MT9V113
    }
    else
    {
		// retry check sensor revision
		ret0 = mt9v113_sensor_write( 0x03, 0x00);
		ret1 = mt9v113_sensor_read( 0x04, &id);

		pr_info("mt9v113_check_sensor_rev!!! ret0:%d ret1:
		%d\n",ret0,ret1);

		if( id == 0x8c )
		{
			pr_info("============================\n");
			pr_info("[cam] MT9V113\n");
			pr_info("============================\n");

			mt9v113_sensor_version	= 0x8c; //MT9V113
		}
		else
		{
			pr_info("------------------------------------\n");
			pr_info("   [cam] INVALID SENSOR  : %d\n",id);
			pr_info("------------------------------------\n");

			mt9v113_sensor_version = 0xFF; //No sensor
		}
    }*/
	return mt9v113_sensor_version;
}

static int mt9v113_hw_init()
{

	int rc = 0;
	unsigned short id = 0;	/*CAM FOR FW */

	CAMDRV_DEBUG("next mt9v113_hw_init");
/*
    id = mt9v113_check_sensor_rev();

    if(id != 0x8c)
    {
		pr_info("<=PCAM=> WRONG SENSOR ID => id 0x%x\n", id);
		rc = -1;
    }
	else
	{
		pr_info("<=PCAM=> CURRENT FRONT SENSOR ID => id 0x%x\n", id);
	}
*/
	return rc;
}

static long mt9v113_set_effect(int mode, int effect)
{
	long rc = 0;

	switch (mode) {
	case SENSOR_PREVIEW_MODE:
		CAMDRV_DEBUG("SENSOR_PREVIEW_MODE");
		break;

	case SENSOR_SNAPSHOT_MODE:
		CAMDRV_DEBUG("SENSOR_SNAPSHOT_MODE");
		break;

	default:
		pr_info("[PGH] %s default\n", __func__);
		break;
	}

	switch (effect) {
	case CAMERA_EFFECT_OFF:{
			CAMDRV_DEBUG("CAMERA_EFFECT_OFF");
		}
		break;

	case CAMERA_EFFECT_MONO:{
			CAMDRV_DEBUG("CAMERA_EFFECT_MONO");
		}
		break;

	case CAMERA_EFFECT_NEGATIVE:{
			CAMDRV_DEBUG("CAMERA_EFFECT_NEGATIVE");
		}
		break;

	case CAMERA_EFFECT_SOLARIZE:{
			CAMDRV_DEBUG("CAMERA_EFFECT_SOLARIZE");
		}
		break;

	case CAMERA_EFFECT_SEPIA:{
			CAMDRV_DEBUG("CAMERA_EFFECT_SEPIA");
		}
		break;

	default:{
			pr_info("<=PCAM=> unexpeceted effect  %s/%d\n",
				__func__, __LINE__);
			return -EINVAL;
		}

	}
	mt9v113_effect = effect;

	return rc;
}

static int mt9v113_debugprint_preview_data(void)
{
/*	unsigned char read_value = 0;

	pr_info( "mt9v113_preview start\n");
	pr_info( "==============================\n");
	mt9v113_sensor_write( 0xef, 0x03);
	mt9v113_sensor_read( 0x33, &read_value);
	pr_info("Normal Light Contrast : 0x%2x\n", read_value);
	mt9v113_sensor_read( 0x34, &read_value);
	pr_info("Low Light Contrast    : 0x%2x\n", read_value);
	mt9v113_sensor_read( 0x01, &read_value);
	pr_info("AE Target             : 0x%2x\n", read_value);
	mt9v113_sensor_read( 0x02, &read_value);
	pr_info("AE Threshold          : 0x%2x\n", read_value);
	mt9v113_sensor_read( 0x67, &read_value);
	pr_info("AE Speed              : 0x%2x\n", read_value);
	pr_info( "==============================\n");
*/
	return 0;
}

int mt9v113_mipi_mode(void)
{
	int rc = 0;
	struct msm_camera_csi_params mt9v113_csi_params;

	CAMDRV_DEBUG("%s E\n", __func__);

	mt9v113_csi_params.lane_cnt = 1;
	mt9v113_csi_params.data_format = CSI_8BIT;
	mt9v113_csi_params.lane_assign = 0xe4;
	mt9v113_csi_params.dpcm_scheme = 0;
	mt9v113_csi_params.settle_cnt = 0x14;
	rc = msm_camio_csi_config(&mt9v113_csi_params);
	if (rc < 0)
		printk(KERN_ERR "config csi controller failed\n");

	CAMDRV_DEBUG("%s X\n", __func__);
	return rc;
}

static int mt9v113_start_preview(void)
{
	mt9v113_debugprint_preview_data();

	mt9v113_mipi_mode();
	usleep(5000);

	if (mt9v113_data.mDTP == 1) {
		mt9v113_sensor_write_list(mt9v113_init_reg, MT9V113_INIT_REGS,
					  "mt9v113_init_reg");
		mt9v113_sensor_write_list(mt9v113_dtp_on, MT9V113_DTP_ON_REGS,
					  "mt9v113_dtp_on");
	} else {
		mt9v113_DTP_control(EXT_CFG_DTP_OFF);

		if (0 == mt9v113_data.mVtMode) {
			pr_info("mt9v113 Normal Preview start");
			mt9v113_sensor_write_list(mt9v113_init_reg,
						  MT9V113_INIT_REGS,
						  "mt9v113_init_reg");
			mt9v113_brightness_control(mt9v113_data.mBrightness);
		} else if (1 == mt9v113_data.mVtMode) {
			pr_info("mt9v113 VtMode Preview start");
			mt9v113_sensor_write_list(mt9v113_VT_init_reg,
						  MT9V113_VT_INIT_REGS,
						  "mt9v113_VT_init_reg");
			mt9v113_FPS_control(mt9v113_data.mFPS);
			mt9v113_VT_brightness_control(mt9v113_data.mBrightness);
		}
	}

	return 0;
}

static long mt9v113_set_sensor_mode(int mode)
{
	pr_info("mt9v113_set_sensor_mode start : %d\n", mode);

	switch (mode) {
	case SENSOR_PREVIEW_MODE:
		{
			CAMDRV_DEBUG("PREVIEW~~~\n");
			mt9v113_start_preview();
		}
		break;

	case SENSOR_SNAPSHOT_MODE:
		{
			CAMDRV_DEBUG("SNAPSHOT~~~\n");
		}
		break;

	case SENSOR_RAW_SNAPSHOT_MODE:
		{
			CAMDRV_DEBUG("RAW_SNAPSHOT NOT SUPPORT!!");
		}
		break;

	default:
		{
			return -EINVAL;
		}
	}

	return 0;
}

static int mt9v113_sensor_init_probe(const struct msm_camera_sensor_info *data)
{
	int rc = 0;
	return rc;
}


int mt9v113_sensor_init(const struct msm_camera_sensor_info *data)
{
	int rc = 0;

	mt9v113_data.mDTP = 0;
	mt9v113_data.mBrightness = EXT_CFG_BR_STEP_0;
	mt9v113_data.mVtMode = 0;

	mt9v113_ctrl = kzalloc(sizeof(struct mt9v113_ctrl), GFP_KERNEL);
	if (!mt9v113_ctrl) {
		CDBG("mt9v113_init failed!\n");
		rc = -ENOMEM;
		goto init_done;
	}

	if (data)
		mt9v113_ctrl->sensordata = data;

	rc = mt9v113_sensor_init_probe(data);

	mt9v113_set_power(1);

	rc = mt9v113_hw_init();

	if (rc < 0) {
		pr_info("<=PCAM=> cam_fw_init failed!\n");
		goto init_fail;	/*Gopeace LeeSangmin DJ26 For test */
	}
#ifdef CONFIG_LOAD_FILE
	mt9v113_regs_table_init();
#endif

	rc = mt9v113_sensor_init_probe(data);
	if (rc < 0) {
		CDBG("mt9v113_sensor_init failed!\n");
		goto init_fail;
	}

init_done:
	return rc;

init_fail:
	kfree(mt9v113_ctrl);
	return rc;
}

static int mt9v113_init_client(struct i2c_client *client)
{
	/* Initialize the MSM_CAMI2C Chip */
	init_waitqueue_head(&mt9v113_wait_queue);
	return 0;
}

int mt9v113_sensor_config(void __user *argp)
{
	struct sensor_cfg_data cfg_data;
	long rc = 0;

	if (copy_from_user(&cfg_data,
			   (void *)argp, sizeof(struct sensor_cfg_data)))
		return -EFAULT;

	/* down(&mt9v113_sem); */

	CDBG("mt9v113_ioctl, cfgtype = %d, mode = %d\n",
	     cfg_data.cfgtype, cfg_data.mode);

	switch (cfg_data.cfgtype) {
	case CFG_SET_MODE:
		rc = mt9v113_set_sensor_mode(cfg_data.mode);
		break;

	case CFG_SET_EFFECT:
		rc = mt9v113_set_effect(cfg_data.mode, cfg_data.cfg.effect);
		break;

	case CFG_GET_AF_MAX_STEPS:
	default:
		rc = -EINVAL;
		break;
	}

	/* up(&mt9v113_sem); */

	return rc;
}

int mt9v113_sensor_release(void)
{
	int rc = 0;

	/*If did not init below that, it can keep the previous status.
	   it depend on concept by PCAM */

	mt9v113_data.mEffect = 0;
	mt9v113_data.mBrightness = EXT_CFG_BR_STEP_0;
	mt9v113_data.mContrast = 0;
	mt9v113_data.mSaturation = 0;
	mt9v113_data.mSharpness = 0;
	mt9v113_data.mWhiteBalance = 0;
	mt9v113_data.mISO = 0;
	mt9v113_data.mAutoExposure = 0;
	/*mt9v113_data.mScene = 0; */
	/*mAfMode = 0; */
	mt9v113_data.mDTP = 0;
	mt9v113_data.mInit = 0;
	mt9v113_data.mFPS = 0;
	mt9v113_data.mVtMode = 0;

	pr_info("<=PCAM=> mt9v113_sensor_release\n");

	kfree(mt9v113_ctrl);

#ifdef CONFIG_LOAD_FILE
	mt9v113_regs_table_exit();
#endif

	mt9v113_set_power(0);
	return rc;
}

static int mt9v113_i2c_probe(struct i2c_client *client,
			     const struct i2c_device_id *id)
{
	int rc = 0;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		rc = -ENOTSUPP;
		goto probe_failure;
	}

	mt9v113_sensorw = kzalloc(sizeof(struct mt9v113_work), GFP_KERNEL);

	if (!mt9v113_sensorw) {
		rc = -ENOMEM;
		goto probe_failure;
	}

	i2c_set_clientdata(client, mt9v113_sensorw);
	mt9v113_init_client(client);
	mt9v113_client = client;

	pr_info("mt9v113_probe succeeded!\n");

	return 0;

probe_failure:
	kfree(mt9v113_sensorw);
	mt9v113_sensorw = NULL;
	CDBG("mt9v113_probe failed!\n");
	return rc;
}

static const struct i2c_device_id mt9v113_i2c_id[] = {
	{"mt9v113", 0},
	{},
};

static struct i2c_driver mt9v113_i2c_driver = {
	.id_table = mt9v113_i2c_id,
	.probe = mt9v113_i2c_probe,
	.remove = __exit_p(mt9v113_i2c_remove),
	.driver = {
		   .name = "mt9v113",
		   },
};

static int mt9v113_sensor_probe(const struct msm_camera_sensor_info *info,
				struct msm_sensor_ctrl *s)
{
	unsigned char id;
	int rc = i2c_add_driver(&mt9v113_i2c_driver);
	if (rc < 0 || mt9v113_sensorw == NULL) {
		rc = -ENOTSUPP;
		goto probe_done;
	}

	mt9v113_set_power(1);

	rc = mt9v113_sensor_write_list(mt9v113_init_reg, MT9V113_INIT_REGS,
				       "mt9v113_init_reg");
	if (rc < 0) {
		pr_info("mt9v113_sensor_write_list i2c fail");
		goto probe_done;
	}

	mt9v113_set_power(0);

	s->s_init = mt9v113_sensor_init;
	s->s_release = mt9v113_sensor_release;
	s->s_config = mt9v113_sensor_config;
	/*for samsung camsensor control temp*/
	s->s_ext_config = mt9v113_sensor_ext_config;

	s->s_camera_type = FRONT_CAMERA_2D;
	s->s_mount_angle = 90;

probe_done:

	mt9v113_set_power(0);

	pr_info("%s %s:%d\n", __FILE__, __func__, __LINE__);
	return rc;
}

static int __mt9v113_probe(struct platform_device *pdev)
{
	pr_info("======== MT9V113 probe ==========\n");
	return msm_camera_drv_start(pdev, mt9v113_sensor_probe);
}

static struct platform_driver msm_camera_driver = {
	.probe = __mt9v113_probe,
	.driver = {
		   .name = "msm_camera_mt9v113",
		   .owner = THIS_MODULE,
		   },
};

static int __init mt9v113_init(void)
{
	return platform_driver_register(&msm_camera_driver);
}

module_init(mt9v113_init);
