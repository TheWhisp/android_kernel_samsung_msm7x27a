/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
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
 */

#include <linux/delay.h>
#include <linux/debugfs.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <media/msm_camera.h>
#include <media/v4l2-subdev.h>
#include <linux/gpio.h>
#include <mach/camera.h>

#include <asm/mach-types.h>
#include <mach/vreg.h>
#include <linux/io.h>

#include "msm.h"
#include "msm_ispif.h"
#include "msm_sensor.h"

#include "sr300pc20.h"
#include "sr300pc20_regs.h"

#define SENSOR_NAME "sr300pc20"

#ifdef CONFIG_MACH_NEVIS3G
#define CAM_IO_EN   	124
#elif defined(CONFIG_MACH_INFINITE_DUOS_CTC)
#define CAM_A_EN   		128
#define REAR_CAM_STBY	96
#define REAR_CAM_RESET	85
#else
#define CAM_IO_EN   	4
#endif

#define REAR_CAM_STBY	96
#define REAR_CAM_RESET	85

#define BURST_MODE_BUFFER_MAX_SIZE 255
#define BURST_REG 0x0e
#define DELAY_REG 0xff
unsigned char sr300pc20_buf_for_burstmode[BURST_MODE_BUFFER_MAX_SIZE];

#define SR300PC20_WRITE_LIST(A)\
	sr300pc20_sensor_write_list(A, (sizeof(A) / sizeof(A[0])), #A);
#define SR300PC20_WRITE_LIST_BURST(A)\
	sr300pc20_sensor_burst_write(A, (sizeof(A) / sizeof(A[0])), #A);

#undef CONFIG_LOAD_FILE
#ifdef CONFIG_LOAD_FILE

#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

static char *sr300pc20_regs_table;
static int sr300pc20_regs_table_size;

static int sr300pc20_regs_table_write(char *name);
static int sr300pc20_regs_table_burst_write(char *name);

#define MAX_SETTING_NAME 30
#define TUNNING_FILE_PATH "/data/sr300pc20_regs.h"
#endif

extern int msm_camera_antibanding_get(); /*add anti-banding code */

static int sr300pc20_copy_files_for_60hz(void){

#define COPY_FROM_60HZ_TABLE(TABLE_NAME, ANTI_BANDING_SETTING) \
	memcpy (TABLE_NAME, TABLE_NAME##_##ANTI_BANDING_SETTING, \
	sizeof(TABLE_NAME))

	CAM_DEBUG("%s: Enter \n",__func__);

	COPY_FROM_60HZ_TABLE (sr300pc20_init_reg2, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_FPS_15, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_FPS_25, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_FPS_30, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Scene_Default, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Scene_Nightshot, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Scene_Backlight, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Scene_Landscape, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Scene_Sports, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Scene_Party_Indoor, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Scene_Beach_Snow, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Scene_Sunset, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Scene_Duskdawn, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Scene_Fall_Color, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Scene_Fireworks, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Scene_Text, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Scene_Candle_Light, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Capture_Start, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Capture_night, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Capture_fireworks, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_Preview_Return, 60hz);
	COPY_FROM_60HZ_TABLE (sr300pc20_640_Preview, 60hz);

	CAM_DEBUG("%s: copy done!\n", __func__);
}

static int sr300pc20_check_table_size_for_60hz(void){
#define IS_SAME_NUM_OF_ROWS(TABLE_NAME) \
	(sizeof(TABLE_NAME) == sizeof(TABLE_NAME##_60hz))

	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_init_reg2) ) return (-1);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_FPS_15) ) return (-2);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_FPS_25) ) return (-3);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_FPS_30) ) return (-4);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Scene_Default) ) return (-5);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Scene_Nightshot) ) return (-6);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Scene_Backlight) ) return (-7);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Scene_Landscape) ) return (-8);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Scene_Sports) ) return (-9);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Scene_Party_Indoor) ) return (-10);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Scene_Beach_Snow) ) return (-11);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Scene_Sunset) ) return (-12);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Scene_Duskdawn) ) return (-13);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Scene_Fall_Color) ) return (-14);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Scene_Fireworks) ) return (-15);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Scene_Text) ) return (-16);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Scene_Candle_Light) ) return (-17);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Capture_Start) ) return (-18);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Capture_night) ) return (-19);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Capture_fireworks) ) return (-20);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_Preview_Return) ) return (-21);
	if ( !IS_SAME_NUM_OF_ROWS(sr300pc20_640_Preview) ) return (-22);

	CAM_DEBUG("%s: Success !\n", __func__);
	return 0;
}

static int sr300pc20_sensor_read(unsigned char subaddr, unsigned char *data)
{
	int ret = 0;
	unsigned char buf[1] = { 0 };
	struct i2c_msg msg = { sr300pc20_client->addr >> 1, 0, 1, buf };

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
	struct i2c_msg msg = { sr300pc20_client->addr >> 1, 0, 2, buf };

	buf[0] = (subaddr);
	buf[1] = (val);

	return i2c_transfer(sr300pc20_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
}

static int sr300pc20_sensor_write_list(const unsigned short *list, int size,
						char *name)
{
	int ret = 0, i;
	unsigned char subaddr;
	unsigned char value;

	CAM_DEBUG("%s\n", name);

#ifdef CONFIG_LOAD_FILE
	ret = sr300pc20_regs_table_write(name);
#else

	for (i = 0; i < size; i++) {
		subaddr = (list[i] >> 8);	/*address */
		value = (list[i] & 0xFF);	/*value */

		if (subaddr == DELAY_REG) {
			msleep(value * 10);	/*one step is 10ms */
			CAM_DEBUG("[sr300pc20] msleep %d msec\n",
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

static int sr300pc20_sensor_burst_write(const unsigned short *list, int size,
						char *name)
{
	int err = -EINVAL;
	int i = 0;
	int idx = 0;
	unsigned char subaddr = 0;
	unsigned char value = 0;
	int burst_flag = 0;
	int burst_cnt = 0;
	struct i2c_msg msg = { sr300pc20_client->addr >> 1,
		0, 0, sr300pc20_buf_for_burstmode
	};

	CAM_DEBUG("%s, size = %d\n", name, size);

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
				/*CAM_DEBUG("burst_cnt %d, idx %d\n",
					     burst_cnt, idx);*/
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

#ifdef CONFIG_LOAD_FILE

void sr300pc20_regs_table_init(void)
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
	CAM_DEBUG("Enter!!\n");
	if (sr300pc20_regs_table != NULL) {
		vfree(sr300pc20_regs_table);
		sr300pc20_regs_table = NULL;
	}
	CAM_DEBUG("Exit!!\n");
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

	CAM_DEBUG("Enter!!\n");

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
			CAM_DEBUG("delay %d msec\n", value * 10);
		} else {
			if (sr300pc20_sensor_write(addr, value) < 0) {
				pr_err
				    ("[%s : %d] fail on sensor_write :"
				     "addr[0x%04x], value[0x%04x]\n",
				     __func__, __LINE__, addr, value);
				err = -EIO;
				return err;
			}
			CAM_DEBUG
			    ("success on sensor_write :"
			     "addr[0x%04x], value[0x%04x]\n", addr, value);
		}
	}

	CAM_DEBUG("Exit!!\n");

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
	struct i2c_msg msg = { sr300pc20_client->addr >> 1,
		0, 0, sr300pc20_buf_for_burstmode
	};

	CAM_DEBUG("Enter!!\n");

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

	CAM_DEBUG("Exit!!\n");

	return err;
}
#endif

static int sr300pc20_set_exif(void)
{
    int err = 0;
    unsigned char a = 0, b = 0, c = 0, d = 0;
    unsigned char data = 0;
    unsigned int OPCLK = 54000000 / 2; // PCLK:54Mhz,

    cam_info(" E");

    /* page mode 0x20 */
    sr300pc20_sensor_write(0x03, 0x20);

    /* read exposure time */
    sr300pc20_sensor_read(0xa0, &a);
    sr300pc20_sensor_read(0xa1, &b);
    sr300pc20_sensor_read(0xa2, &c);
    sr300pc20_sensor_read(0xa3, &d);

    sr300pc20_exif.shutterspeed = OPCLK / (unsigned int)((a << 24) | (b << 16) | (c << 8) | d );
    CAM_DEBUG("Exposure time = %d\n", sr300pc20_exif.shutterspeed);

    /* read ISO gain */
    sr300pc20_sensor_read(0x50, &data);

    if(data < 0x25)
        sr300pc20_exif.iso = 50;
    else if(data < 0x5C)
        sr300pc20_exif.iso = 100;
    else if(data < 0x83)
        sr300pc20_exif.iso = 200;
    else if(data < 0xF1)
        sr300pc20_exif.iso = 400;
    else
        sr300pc20_exif.iso = 800;

    CAM_DEBUG("ISO = %d\n", sr300pc20_exif.iso);

	return err;
}


static unsigned short sr300pc20_get_exif(int exif_cmd, unsigned short value2)
{
	unsigned short val = 0;

	cam_info(" E %d", exif_cmd);

	switch (exif_cmd) {
	case EXIF_EXPOSURE_TIME:
		val = sr300pc20_exif.shutterspeed;
		break;

	case EXIF_ISO:
		val = sr300pc20_exif.iso;
		break;

	default:
		break;
	}

	return val;
}

unsigned char sr300pc20_get_LuxValue(void)
{
    unsigned char lux_val = 0;

    /* page mode 0x20 */
    sr300pc20_sensor_write(0x03, 0x20);
    /* read lux value */
    sr300pc20_sensor_read(0xb1, &lux_val);

	sr300pc20_control.lux = 0xFF & lux_val;
	CAM_DEBUG(" Lux = %d", sr300pc20_control.lux);

    return sr300pc20_control.lux;
}

void sr300pc20_set_preview_size(int32_t index)
{
    cam_info("index %d", index);

/* Don't need to change preview size, it use alway VGA */
    switch (index) {

    case PREVIEW_SIZE_VGA:
        SR300PC20_WRITE_LIST(sr300pc20_640_Preview);
        break;
#ifdef CONFIG_MACH_INFINITE_DUOS_CTC
    case PREVIEW_SIZE_720_D1:
		SR300PC20_WRITE_LIST(sr300pc20_720x480_Preview);
		break;

	case PREVIEW_SIZE_WVGA:
		SR300PC20_WRITE_LIST(sr300pc20_800x480_Preview);
		break;

	case PREVIEW_SIZE_W960:
		SR300PC20_WRITE_LIST(sr300pc20_960x720_Preview);
		break;
#endif
    default:
        SR300PC20_WRITE_LIST(sr300pc20_640_Preview);
        break;
    }

    sr300pc20_control.settings.preview_size_idx = index;
}

void sr300pc20_set_picture_size(int32_t index)
{
	cam_info(" %d -> %d\n", sr300pc20_control.settings.capture_size, index);

/*  Roy EUR OPEN doesn't use capture scaling from sensor.
    This model use VFE downscale of MSM7227A.
    So, Sensor should be setting fullsize capture data */
/*
	switch (index) {

	case MSM_V4L2_PICTURE_SIZE_2048x1536_3M:
		SR300PC20_WRITE_LIST(sr300pc20_3M_Capture);
		break;

	case MSM_V4L2_PICTURE_SIZE_1600x1200_2M:
		SR300PC20_WRITE_LIST(sr300pc20_2M_Capture);
		break;

	case MSM_V4L2_PICTURE_SIZE_1280x960_1M:
		SR300PC20_WRITE_LIST(sr300pc20_1M_Capture);
		break;
        
	case MSM_V4L2_PICTURE_SIZE_640x480_VGA:
		SR300PC20_WRITE_LIST(sr300pc20_VGA_Capture);
		break;

	default:
		SR300PC20_WRITE_LIST(sr300pc20_3M_Capture);
		break;
	}
*/

	sr300pc20_control.settings.capture_size = index;
}

static int32_t sr300pc20_sensor_setting(int update_type, int rt)
{
	int32_t rc = 0;

	cam_info(" E");

	switch (update_type) {
	case REG_INIT:
		break;

	case UPDATE_PERIODIC:
        CAM_DEBUG(" UPDATE_PERIODIC");
		msm_sensor_enable_debugfs(sr300pc20_control.s_ctrl);
		if (rt == RES_PREVIEW || rt == RES_CAPTURE) {
			CAM_DEBUG(" UPDATE_PERIODIC : delay");
			config_csi2 = 1;
			msleep(30);
		}
		break;
	default:
		rc = -EINVAL;
		break;
	}

	return rc;
}

void sr300pc20_set_frame_rate(int32_t fps)
{
	cam_info(" %d", fps);

	switch (fps) {
	case 15:
		SR300PC20_WRITE_LIST(sr300pc20_FPS_15);
		break;

	case 20:
		SR300PC20_WRITE_LIST(sr300pc20_FPS_20);
		break;

	case 24:
		SR300PC20_WRITE_LIST(sr300pc20_FPS_24);
		break;

	case 25:
		SR300PC20_WRITE_LIST(sr300pc20_FPS_25);
		break;

	case 30:
		SR300PC20_WRITE_LIST(sr300pc20_FPS_30);
		break;

	default:
		break;
	}

	sr300pc20_control.settings.fps = fps;
}

static int sr300pc20_set_effect(int effect)
{
	CAM_DEBUG(" %d", effect);

	switch (effect) {
	case CAMERA_EFFECT_OFF:
		SR300PC20_WRITE_LIST(sr300pc20_Effect_Normal);
		break;

	case CAMERA_EFFECT_MONO:
		SR300PC20_WRITE_LIST(sr300pc20_Effect_Black_White);
		break;

	case CAMERA_EFFECT_NEGATIVE:
		SR300PC20_WRITE_LIST(sr300pc20_Effect_Negative);
		break;

	case CAMERA_EFFECT_SEPIA:
		SR300PC20_WRITE_LIST(sr300pc20_Effect_Sepia);
		break;

	default:
		cam_info(" effect : default");
		SR300PC20_WRITE_LIST(sr300pc20_Effect_Normal);
		return 0;
	}

	sr300pc20_control.settings.effect = effect;

	return 0;
}

static int sr300pc20_set_whitebalance(int wb)
{
	CAM_DEBUG(" %d", wb);

	switch (wb) {
	case CAMERA_WHITE_BALANCE_AUTO:
		SR300PC20_WRITE_LIST(sr300pc20_WB_Auto);
		break;

	case CAMERA_WHITE_BALANCE_INCANDESCENT:
		SR300PC20_WRITE_LIST(sr300pc20_WB_Incandescent);
		break;

	case CAMERA_WHITE_BALANCE_FLUORESCENT:
		SR300PC20_WRITE_LIST(sr300pc20_WB_Fluorescent);
		break;

	case CAMERA_WHITE_BALANCE_DAYLIGHT:
		SR300PC20_WRITE_LIST(sr300pc20_WB_Sunny);
		break;

	case CAMERA_WHITE_BALANCE_CLOUDY_DAYLIGHT:
		SR300PC20_WRITE_LIST(sr300pc20_WB_Cloudy);
		break;

	default:
		cam_info(" WB : default");
		return 0;
	}

	sr300pc20_control.settings.wb = wb;

	return 0;
}

static void sr300pc20_check_dataline(int val)
{
	if (val) {
		CAM_DEBUG(" DTP ON");
		SR300PC20_WRITE_LIST(sr300pc20_DTP_init);
		sr300pc20_control.dtpTest = 1;

	} else {
		CAM_DEBUG(" DTP OFF");
		SR300PC20_WRITE_LIST(sr300pc20_DTP_stop);
		sr300pc20_control.dtpTest = 0;
	}
}

static void sr300pc20_set_ae_awb(int lock)
{
	if (lock) {
		CAM_DEBUG(" AE_AWB LOCK");
		SR300PC20_WRITE_LIST(sr300pc20_ae_lock);
		SR300PC20_WRITE_LIST(sr300pc20_awb_lock);
	} else {
		CAM_DEBUG(" AE_AWB UNLOCK");
		SR300PC20_WRITE_LIST(sr300pc20_ae_unlock);
		SR300PC20_WRITE_LIST(sr300pc20_awb_unlock);
	}
}

static void sr300pc20_set_ev(int ev)
{
	CAM_DEBUG(" %d", ev);

	switch (ev) {
	case CAMERA_EV_M4:
		SR300PC20_WRITE_LIST(sr300pc20_EV_Minus_4);
		break;

	case CAMERA_EV_M3:
		SR300PC20_WRITE_LIST(sr300pc20_EV_Minus_3);
		break;

	case CAMERA_EV_M2:
		SR300PC20_WRITE_LIST(sr300pc20_EV_Minus_2);
		break;

	case CAMERA_EV_M1:
		SR300PC20_WRITE_LIST(sr300pc20_EV_Minus_1);
		break;

	case CAMERA_EV_DEFAULT:
		SR300PC20_WRITE_LIST(sr300pc20_EV_Default);
		break;

	case CAMERA_EV_P1:
		SR300PC20_WRITE_LIST(sr300pc20_EV_Plus_1);
		break;

	case CAMERA_EV_P2:
		SR300PC20_WRITE_LIST(sr300pc20_EV_Plus_2);
		break;

	case CAMERA_EV_P3:
		SR300PC20_WRITE_LIST(sr300pc20_EV_Plus_3);
		break;

	case CAMERA_EV_P4:
		SR300PC20_WRITE_LIST(sr300pc20_EV_Plus_4);
		break;
	default:
		cam_info(" ev : default");
		break;
	}

	sr300pc20_control.settings.brightness = ev;
}

static void sr300pc20_set_scene_mode(int mode)
{
	CAM_DEBUG(" %d", mode);

	switch (mode) {
	case CAMERA_SCENE_AUTO:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Default);
		break;

	case CAMERA_SCENE_LANDSCAPE:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Landscape);
		break;

	case CAMERA_SCENE_DAWN:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Duskdawn);
		break;

	case CAMERA_SCENE_BEACH:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Beach_Snow);
		break;

	case CAMERA_SCENE_SUNSET:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Sunset);
		break;

	case CAMERA_SCENE_NIGHT:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Nightshot);
		break;

	case CAMERA_SCENE_PORTRAIT:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Portrait);
		break;

	case CAMERA_SCENE_AGAINST_LIGHT:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Backlight);
		break;

	case CAMERA_SCENE_SPORT:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Sports);
		break;

	case CAMERA_SCENE_FALL:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Fall_Color);
		break;

	case CAMERA_SCENE_TEXT:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Text);
		break;

	case CAMERA_SCENE_CANDLE:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Candle_Light);
		break;

	case CAMERA_SCENE_FIRE:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Fireworks);
		break;

	case CAMERA_SCENE_PARTY:
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Party_Indoor);
		break;

	default:
		cam_info(" scene : default");
		SR300PC20_WRITE_LIST(sr300pc20_Scene_Default);
		break;
	}

	sr300pc20_control.settings.scenemode = mode;
}

static void sr300pc20_set_iso(int iso)
{
	CAM_DEBUG(" %d", iso);

	switch (iso) {
	case CAMERA_ISO_MODE_AUTO:
		SR300PC20_WRITE_LIST(sr300pc20_ISO_Auto);
		break;

	case CAMERA_ISO_MODE_50:
		SR300PC20_WRITE_LIST(sr300pc20_ISO_50);
		break;

	case CAMERA_ISO_MODE_100:
		SR300PC20_WRITE_LIST(sr300pc20_ISO_100);
		break;

	case CAMERA_ISO_MODE_200:
		SR300PC20_WRITE_LIST(sr300pc20_ISO_200);
		break;

	case CAMERA_ISO_MODE_400:
		SR300PC20_WRITE_LIST(sr300pc20_ISO_400);
		break;

	default:
		cam_info(" iso : default");
		break;
	}

	sr300pc20_control.settings.iso = iso;
}

static void sr300pc20_set_metering(int mode)
{
	CAM_DEBUG(" %d", mode);

	switch (mode) {
	case CAMERA_CENTER_WEIGHT:
		SR300PC20_WRITE_LIST(sr300pc20_Metering_Center);
		break;

	case CAMERA_AVERAGE:
		SR300PC20_WRITE_LIST(sr300pc20_Metering_Matrix);
		break;

	case CAMERA_SPOT:
		SR300PC20_WRITE_LIST(sr300pc20_Metering_Spot);
		break;

	default:
		cam_info(" AE : default");
		break;
	}

	sr300pc20_control.settings.metering = mode;
}

static struct msm_cam_clk_info cam_clk_info[] = {
	{"cam_m_clk", MSM_SENSOR_MCLK_24HZ},
};

static int sr300pc20_sensor_match_id(struct msm_sensor_ctrl_t *s_ctrl)
{
	int rc = 0;
	unsigned char chipid = 0, version = 0;

	sr300pc20_sensor_write(0x03, 0x00);
	rc = sr300pc20_sensor_read(0x04, &chipid);

	if (rc < 0) {
		pr_err("%s: %s: read id failed\n", __func__,
		s_ctrl->sensordata->sensor_name);
		return rc;
	}

	cam_info("msm_sensor %s id: 0x%x, version = 0x%x \n",
			s_ctrl->sensordata->sensor_name, chipid, version);

	if (chipid != s_ctrl->sensor_id_info->sensor_id ||
		version != s_ctrl->sensor_id_info->sensor_version) {
		pr_err("msm_sensor_match_id chip id doesnot match\n");
		return -ENODEV;
	}
	return rc;
}

static int sr300pc20_sensor_power_up(struct msm_sensor_ctrl_t *s_ctrl)
{
	int rc = 0;
	struct msm_camera_sensor_info *data = s_ctrl->sensordata;

	cam_info(" E\n");

	/* GPIO 15 : CAM_MCLK Configuration for IORA */
	gpio_tlmm_config(GPIO_CFG(15, 1, GPIO_CFG_OUTPUT,
			GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);

#ifdef CONFIG_MACH_INFINITE_DUOS_CTC
	rc = gpio_request(CAM_A_EN, "cam_a_en");
#else
	rc = gpio_request(CAM_IO_EN, "cam_io_en");
#endif
	rc = gpio_request(REAR_CAM_RESET, "3m_cam_reset"); // reset by GPIO 85
	rc = gpio_request(REAR_CAM_STBY, "3m_cam_stby");   // stand by GPIO 96

#ifdef CONFIG_MACH_INFINITE_DUOS_CTC
	gpio_direction_output(CAM_A_EN, 0);
#else
	gpio_direction_output(CAM_IO_EN, 0);
#endif
	gpio_direction_output(REAR_CAM_RESET, 0);
	gpio_direction_output(REAR_CAM_STBY, 0);

#ifdef CONFIG_MACH_INFINITE_DUOS_CTC
	gpio_set_value_cansleep(CAM_A_EN, 0);
#else
	gpio_set_value_cansleep(CAM_IO_EN, 0);
#endif
	gpio_set_value_cansleep(REAR_CAM_RESET, 0);
	gpio_set_value_cansleep(REAR_CAM_STBY, 0);

#ifdef CONFIG_MACH_INFINITE_DUOS_CTC
	gpio_set_value(CAM_A_EN, 1);
#else
	gpio_set_value(CAM_IO_EN, 1);
#endif

	s_ctrl->reg_ptr = kzalloc(sizeof(struct regulator *)
			* data->sensor_platform_info->num_vreg, GFP_KERNEL);
	if (!s_ctrl->reg_ptr) {
		pr_err("%s: could not allocate mem for regulators\n",
			__func__);
		return -ENOMEM;
	}
	rc = msm_camera_config_vreg(&s_ctrl->sensor_i2c_client->client->dev,
			s_ctrl->sensordata->sensor_platform_info->cam_vreg,
			s_ctrl->sensordata->sensor_platform_info->num_vreg,
			s_ctrl->reg_ptr, 1);
	if (rc < 0) {
		pr_err("%s: regulator on failed\n", __func__);
		goto config_vreg_failed;
	}

	rc = msm_camera_enable_vreg(&s_ctrl->sensor_i2c_client->client->dev,
			s_ctrl->sensordata->sensor_platform_info->cam_vreg,
			s_ctrl->sensordata->sensor_platform_info->num_vreg,
			s_ctrl->reg_ptr, 1);

	if (rc < 0) {
		pr_err("%s: enable regulator failed\n", __func__);
		goto enable_vreg_failed;
	}


	msleep(1);

	if (s_ctrl->clk_rate != 0)
		cam_clk_info->clk_rate = s_ctrl->clk_rate;

	rc = msm_cam_clk_enable(&s_ctrl->sensor_i2c_client->client->dev,
		cam_clk_info, &s_ctrl->cam_clk, ARRAY_SIZE(cam_clk_info), 1);
	if (rc < 0) {
		pr_err("%s: clk enable failed\n", __func__);
		goto enable_clk_failed;
	}

	msleep(1);
	gpio_set_value_cansleep(REAR_CAM_STBY, 1);		//STBYN :: MSM_GPIO 96
	msleep(1);
	gpio_set_value_cansleep(REAR_CAM_RESET, 1);	//RSTN :: MSM_GPIO 85
	msleep(1);
	usleep_range(1000, 2000);

	if (data->sensor_platform_info->ext_power_ctrl != NULL)
		data->sensor_platform_info->ext_power_ctrl(1);

	if (data->sensor_platform_info->i2c_conf &&
		data->sensor_platform_info->i2c_conf->use_i2c_mux)
		msm_sensor_enable_i2c_mux(data->sensor_platform_info->i2c_conf);


	cam_info(" X\n");

	return rc;

enable_clk_failed:
config_gpio_failed:
	msm_camera_enable_vreg(&s_ctrl->sensor_i2c_client->client->dev,
			s_ctrl->sensordata->sensor_platform_info->cam_vreg,
			s_ctrl->sensordata->sensor_platform_info->num_vreg,
			s_ctrl->reg_ptr, 0);

enable_vreg_failed:
	msm_camera_config_vreg(&s_ctrl->sensor_i2c_client->client->dev,
		s_ctrl->sensordata->sensor_platform_info->cam_vreg,
		s_ctrl->sensordata->sensor_platform_info->num_vreg,
		s_ctrl->reg_ptr, 0);
config_vreg_failed:
request_gpio_failed:
	kfree(s_ctrl->reg_ptr);
	return rc;
}

static int sr300pc20_sensor_power_down(struct msm_sensor_ctrl_t *s_ctrl)
{
	int rc = 0;
	struct msm_camera_sensor_info *data = s_ctrl->sensordata;

	cam_info(" E");

	if (data->sensor_platform_info->i2c_conf &&
		data->sensor_platform_info->i2c_conf->use_i2c_mux)
		msm_sensor_disable_i2c_mux(
			data->sensor_platform_info->i2c_conf);

	if (data->sensor_platform_info->ext_power_ctrl != NULL)
		data->sensor_platform_info->ext_power_ctrl(0);
	msleep(1);
	
	gpio_set_value_cansleep(REAR_CAM_RESET, 0);	//RSTN :: MSM_GPIO 85
	msm_cam_clk_enable(&s_ctrl->sensor_i2c_client->client->dev,
		cam_clk_info, &s_ctrl->cam_clk, ARRAY_SIZE(cam_clk_info), 0);
	msleep(1);
	
	gpio_set_value_cansleep(REAR_CAM_STBY, 0);		//STBYN :: MSM_GPIO 96
	msleep(1);
	
	msm_camera_enable_vreg(&s_ctrl->sensor_i2c_client->client->dev,
	s_ctrl->sensordata->sensor_platform_info->cam_vreg,
	s_ctrl->sensordata->sensor_platform_info->num_vreg,
	s_ctrl->reg_ptr, 0);
	msm_camera_config_vreg(&s_ctrl->sensor_i2c_client->client->dev,
	s_ctrl->sensordata->sensor_platform_info->cam_vreg,
	s_ctrl->sensordata->sensor_platform_info->num_vreg,
	s_ctrl->reg_ptr, 0);
	kfree(s_ctrl->reg_ptr);
	usleep(100);

#ifdef CONFIG_MACH_INFINITE_DUOS_CTC
	gpio_set_value_cansleep(CAM_A_EN, 0);
#else
	gpio_set_value_cansleep(CAM_IO_EN, 0);
#endif
	msleep(1);

	/* GPIO 15 : CAM_MCLK Configuration for IORA */
	gpio_tlmm_config(GPIO_CFG(15, 0, GPIO_CFG_OUTPUT,
			GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);

	gpio_free(REAR_CAM_STBY);
	gpio_free(REAR_CAM_RESET);
#ifdef CONFIG_MACH_INFINITE_DUOS_CTC	
	gpio_free(CAM_A_EN);
#else
	gpio_free(CAM_IO_EN);
#endif

#ifdef CONFIG_LOAD_FILE
	sr300pc20_regs_table_exit();
#endif
	cam_info(" X");

	return rc;
}

void sensor_native_control(void __user *arg)
{
	struct ioctl_native_cmd ctrl_info;
	struct msm_camera_v4l2_ioctl_t *ioctl_ptr = arg;

	if (copy_from_user(&ctrl_info,
		(void __user *)ioctl_ptr->ioctl_ptr,
		sizeof(ctrl_info))) {
		pr_err("fail copy_from_user!\n");
		goto FAIL_END;
	}

	cam_info("mode : %d \n", ctrl_info.mode);
	

	switch (ctrl_info.mode) {	/* for fast initialising */
	case EXT_CAM_EV:
		CAM_DEBUG("EXT_CAM_EV\n");
		if (sr300pc20_control.settings.scene == CAMERA_SCENE_BEACH)
			break;
		else {
			sr300pc20_set_ev(ctrl_info.value_1);
			break;
		}

	case EXT_CAM_WB:
		CAM_DEBUG("EXT_CAM_WB\n");
		sr300pc20_set_whitebalance(ctrl_info.value_1);
		sr300pc20_control.awb_mode = ctrl_info.value_1;
		break;

	case EXT_CAM_METERING:
		CAM_DEBUG("EXT_CAM_METERING\n");
		sr300pc20_set_metering(ctrl_info.value_1);
		break;

	case EXT_CAM_ISO:
		CAM_DEBUG("EXT_CAM_ISO\n");
		sr300pc20_set_iso(ctrl_info.value_1);
		break;

	case EXT_CAM_EFFECT:
		CAM_DEBUG("EXT_CAM_EFFECT START\n");
		sr300pc20_set_effect(ctrl_info.value_1);
		CAM_DEBUG("EXT_CAM_EFFECT END\n");
		
		break;

	case EXT_CAM_SCENE_MODE:
		CAM_DEBUG("EXT_CAM_SCENE_MODE\n");
		sr300pc20_set_scene_mode(ctrl_info.value_1);
		break;

	case EXT_CAM_MOVIE_MODE:
		CAM_DEBUG("EXT_CAM_MOVIE_MODE\n");
		CAM_DEBUG(" MOVIE mode : %d", ctrl_info.value_1);
		sr300pc20_control.cam_mode = ctrl_info.value_1;
		break;

	case EXT_CAM_PREVIEW_SIZE:
		CAM_DEBUG("EXT_CAM_PREVIEW_SIZE\n");
		sr300pc20_set_preview_size(ctrl_info.value_1);
		break;

	case CFG_SET_PICTURE_SIZE:
		CAM_DEBUG("CFG_SET_PICTURE_SIZE\n");
		sr300pc20_set_picture_size(ctrl_info.value_1);
		break;

	case EXT_CAM_DTP_TEST:
		sr300pc20_check_dataline(ctrl_info.value_1);
		break;

	case EXT_CAM_SET_AE_AWB:
		CAM_DEBUG("EXT_CAM_SET_AE_AWB\n");
		sr300pc20_set_ae_awb(ctrl_info.value_1);
		break;

	case EXT_CAM_EXIF:
		CAM_DEBUG("EXT_CAM_EXIF %d, %d \n", ctrl_info.address, ctrl_info.value_2);
		ctrl_info.value_1 = sr300pc20_get_exif(ctrl_info.address,
			ctrl_info.value_2);
		CAM_DEBUG("EXT_CAM_EXIF:DONE\n");
		break;

	case EXT_CAM_VT_MODE:
		CAM_DEBUG(" VT mode : %d", ctrl_info.value_1);
		sr300pc20_control.vtcall_mode = ctrl_info.value_1;
		break;

	case EXT_CAM_SET_FPS:
		CAM_DEBUG("EXT_CAM_SET_FPS\n");
		sr300pc20_set_frame_rate(ctrl_info.value_1);
		break;

	case EXT_CAM_SAMSUNG_CAMERA:
		CAM_DEBUG(" SAMSUNG camera : %d", ctrl_info.value_1);
		sr300pc20_control.samsungapp = ctrl_info.value_1;
		break;

	default:
		break;
	}

	if (copy_to_user((void __user *)ioctl_ptr->ioctl_ptr,
		  (const void *)&ctrl_info,
			sizeof(ctrl_info))) {
		pr_err("fail copy_to_user!\n");
		goto FAIL_END;
	}

	return;

FAIL_END:
	CAM_DEBUG("Error : can't handle native control\n");

}

static struct v4l2_subdev_info sr300pc20_subdev_info[] = {
	{
		.code   = V4L2_MBUS_FMT_YUYV8_2X8,
		.colorspace = V4L2_COLORSPACE_JPEG,
		.fmt    = 1,
		.order    = 0,
	},
	/* more can be supported, to be added later */
};

void sr300pc20_sensor_start_stream(struct msm_sensor_ctrl_t *s_ctrl)
{
	CAM_DEBUG(" E\n");

	int rc = 0;
	if (msm_camera_antibanding_get() == CAMERA_ANTIBANDING_60HZ) {
		rc = sr300pc20_check_table_size_for_60hz();
		if(rc != 0) {
			CAM_DEBUG("%s: Fail - the table num is %d \n", __func__, rc);
		}
		sr300pc20_copy_files_for_60hz();
	}

#ifdef CONFIG_LOAD_FILE
	sr300pc20_regs_table_init();
#endif
	/*SR300PC20_WRITE_LIST(sr300pc20_init_reg1);*/
	SR300PC20_WRITE_LIST_BURST(sr300pc20_init_reg2);
	CAM_DEBUG(" X\n");

}

void sr300pc20_sensor_stop_stream(struct msm_sensor_ctrl_t *s_ctrl)
{
	CAM_DEBUG(" E\n");
}

void sr300pc20_sensor_preview_mode(struct msm_sensor_ctrl_t *s_ctrl)
{
	CAM_DEBUG(" E\n");
#ifdef CONFIG_MACH_INFINITE_DUOS_CTC
	switch (sr300pc20_control.settings.preview_size_idx) {

		case PREVIEW_SIZE_VGA:
			SR300PC20_WRITE_LIST(sr300pc20_640_Preview);
			break;

		case PREVIEW_SIZE_720_D1:
			SR300PC20_WRITE_LIST(sr300pc20_720x480_Preview);
			break;

		case PREVIEW_SIZE_WVGA:
			SR300PC20_WRITE_LIST(sr300pc20_800x480_Preview);
			break;

		case PREVIEW_SIZE_W960:
			SR300PC20_WRITE_LIST(sr300pc20_960x720_Preview);
		break;

		default:
			SR300PC20_WRITE_LIST(sr300pc20_640_Preview);
		break;
	}
#else
	SR300PC20_WRITE_LIST(sr300pc20_Preview_Return);
#endif
	CAM_DEBUG(" X\n");
}

void sr300pc20_sensor_capture_mode(struct msm_sensor_ctrl_t *s_ctrl)
{
    unsigned char lux_val = 0;

    CAM_DEBUG(" E\n");

    lux_val = sr300pc20_get_LuxValue();

    if (sr300pc20_control.settings.scenemode == CAMERA_SCENE_NIGHT) {
        CAM_DEBUG(" SCENE_NIGHTSHOT\n");

        if (lux_val < 0x16) {
            SR300PC20_WRITE_LIST(sr300pc20_Capture_night);
        } else {
            SR300PC20_WRITE_LIST(sr300pc20_Capture_Start);
        }
    } else if (sr300pc20_control.settings.scenemode == CAMERA_SCENE_FIRE) {
        CAM_DEBUG(" SCENE_FIREWORK\n");

        if (lux_val < 0x16) {
            SR300PC20_WRITE_LIST(sr300pc20_Capture_fireworks);
        } else {
            SR300PC20_WRITE_LIST(sr300pc20_Capture_Start);
        }
    } else {
        SR300PC20_WRITE_LIST(sr300pc20_Capture_Start);
    }

    /*Read exif information from sensor*/
    sr300pc20_set_exif();

    CAM_DEBUG(" X\n");
}

static int sr300pc20_i2c_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int rc = 0;
	struct msm_sensor_ctrl_t *s_ctrl;

	cam_info("%s_i2c_probe called \n", client->name);

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		rc = -ENOTSUPP;
		cam_err("i2c_check_functionality failed\n");
		rc = -EFAULT;
		return rc;
	}

	s_ctrl = (struct msm_sensor_ctrl_t *)(id->driver_data);
	if (s_ctrl->sensor_i2c_client != NULL) {
		s_ctrl->sensor_i2c_client->client = client;
		if (s_ctrl->sensor_i2c_addr != 0)
			s_ctrl->sensor_i2c_client->client->addr =
				s_ctrl->sensor_i2c_addr;
	} else {
		cam_err("s_ctrl->sensor_i2c_client is NULL\n");
		rc = -EFAULT;
		return rc;
	}

	s_ctrl->sensordata = client->dev.platform_data;
	if (s_ctrl->sensordata == NULL) {
		cam_err("NULL sensor data \n");
		return -EFAULT;
	}

	rc = s_ctrl->func_tbl->sensor_power_up(s_ctrl);
	if (rc < 0) {
		cam_err("%s %s power up failed\n", __func__, client->name);
		return rc;
	}

	sr300pc20_client = client;
	sr300pc20_dev = s_ctrl->sensor_i2c_client->client->dev;

	if (s_ctrl->func_tbl->sensor_match_id)
		rc = s_ctrl->func_tbl->sensor_match_id(s_ctrl);
	else
		rc = msm_sensor_match_id(s_ctrl);

	if (rc < 0)
		goto probe_failure;

	cam_err("sr300pc20_probe succeeded! \n");

	snprintf(s_ctrl->sensor_v4l2_subdev.name,
		sizeof(s_ctrl->sensor_v4l2_subdev.name), "%s", id->name);
	v4l2_i2c_subdev_init(&s_ctrl->sensor_v4l2_subdev, client,
		s_ctrl->sensor_v4l2_subdev_ops);
	s_ctrl->sensor_v4l2_subdev.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
	media_entity_init(&s_ctrl->sensor_v4l2_subdev.entity, 0, NULL, 0);
	s_ctrl->sensor_v4l2_subdev.entity.type = MEDIA_ENT_T_V4L2_SUBDEV;
	s_ctrl->sensor_v4l2_subdev.entity.group_id = SENSOR_DEV;
	s_ctrl->sensor_v4l2_subdev.entity.name =
		s_ctrl->sensor_v4l2_subdev.name;
	msm_sensor_register(&s_ctrl->sensor_v4l2_subdev);
	s_ctrl->sensor_v4l2_subdev.entity.revision =
		s_ctrl->sensor_v4l2_subdev.devnode->num;
	goto power_down;

probe_failure:
	cam_err("sr300pc20_probe failed! rc = %d \n", rc);
    s_ctrl->func_tbl->sensor_power_down(s_ctrl);
	return rc;

power_down:
	if (rc > 0)
		rc = 0;
	s_ctrl->func_tbl->sensor_power_down(s_ctrl);
	return rc;
}

static struct v4l2_subdev_video_ops sr300pc20_subdev_video_ops = {
	.enum_mbus_fmt = msm_sensor_v4l2_enum_fmt,
};

static struct msm_sensor_output_info_t sr300pc20_dimensions[] = {
	{
		.x_output = 0x800,
		.y_output = 0x600,
		.line_length_pclk = 0x800,
		.frame_length_lines = 0x600,
		.vt_pixel_clk = 9216000,
		.op_pixel_clk = 9216000,
		.binning_factor = 1,
	},
	{
#ifdef CONFIG_MACH_INFINITE_DUOS_CTC
		.x_output = 800,//max preview size
		.y_output = 480,//max preview size
#else
		.x_output = 0x280,
		.y_output = 0x1E0,
#endif
		.line_length_pclk = 0x280,
		.frame_length_lines = 0x1E0,
		.vt_pixel_clk = 9216000,
		.op_pixel_clk = 9216000,
		.binning_factor = 1,
	},
};

static struct msm_camera_csi_params sr300pc20_csi_params = {
	.data_format = CSI_8BIT,
	.lane_cnt    = 1,
	.lane_assign = 0xe4,
	.dpcm_scheme = 0,
	.settle_cnt  = 0x13,
};

static struct msm_camera_csi_params *sr300pc20_csi_params_array[] = {
	&sr300pc20_csi_params,
	&sr300pc20_csi_params,
};

static struct msm_sensor_output_reg_addr_t sr300pc20_reg_addr = {
	.x_output = 0x280,
	.y_output = 0x1E0,
	.line_length_pclk = 0x280,
	.frame_length_lines = 0x1E0,
};

static struct msm_sensor_id_info_t sr300pc20_id_info = {
	.sensor_id_reg_addr = 0x04,
	.sensor_id = 0xA4,
	.sensor_version = 0x00,
};

static const struct i2c_device_id sr300pc20_i2c_id[] = {
	{SENSOR_NAME, (kernel_ulong_t)&sr300pc20_s_ctrl},
	{}
};

static struct i2c_driver sr300pc20_i2c_driver = {
	.id_table = sr300pc20_i2c_id,
	.probe = sr300pc20_i2c_probe,
	.driver = {
		   .name = "sr300pc20",
	},
};

static struct msm_camera_i2c_client sr300pc20_sensor_i2c_client = {
	.addr_type = MSM_CAMERA_I2C_BYTE_ADDR,
};

static int __init msm_sensor_init_module(void)
{
	int rc = 0;
	cam_info("SR300PC20\n");

	rc = i2c_add_driver(&sr300pc20_i2c_driver);

	return rc;
}

static struct msm_camera_i2c_reg_conf sr300pc20_capture_settings[] = {
	{0xFF, 0x00},
};

static struct msm_camera_i2c_reg_conf sr300pc20_preview_return_settings[] = {
	{0xFF, 0x00},
};

static struct msm_camera_i2c_conf_array sr300pc20_confs[] = {
	{&sr300pc20_capture_settings[0],
	ARRAY_SIZE(sr300pc20_capture_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
	{&sr300pc20_preview_return_settings[0],
	ARRAY_SIZE(sr300pc20_preview_return_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
};

static struct v4l2_subdev_core_ops sr300pc20_subdev_core_ops = {
	.s_ctrl = msm_sensor_v4l2_s_ctrl,
	.queryctrl = msm_sensor_v4l2_query_ctrl,
	.ioctl = msm_sensor_subdev_ioctl,
	.s_power = msm_sensor_power,
};

static struct v4l2_subdev_ops sr300pc20_subdev_ops = {
	.core = &sr300pc20_subdev_core_ops,
	.video  = &sr300pc20_subdev_video_ops,
};

static struct msm_sensor_fn_t sr300pc20_func_tbl = {
	.sensor_start_stream = sr300pc20_sensor_start_stream,
	.sensor_stop_stream = sr300pc20_sensor_stop_stream,
	.sensor_preview_mode = sr300pc20_sensor_preview_mode,
	.sensor_capture_mode = sr300pc20_sensor_capture_mode,
	.sensor_set_fps = NULL,
	.sensor_write_exp_gain = NULL,
	.sensor_write_snapshot_exp_gain = NULL,
	.sensor_csi_setting = msm_sensor_setting1,
	.sensor_set_sensor_mode = msm_sensor_set_sensor_mode,
	.sensor_mode_init = msm_sensor_mode_init,
	.sensor_get_output_info = msm_sensor_get_output_info,
	.sensor_config = msm_sensor_config,
	.sensor_power_up = sr300pc20_sensor_power_up,
	.sensor_power_down =  sr300pc20_sensor_power_down,
	.sensor_match_id = sr300pc20_sensor_match_id,
	.sensor_adjust_frame_lines = NULL,
	.sensor_get_csi_params = msm_sensor_get_csi_params,
};

static struct msm_sensor_reg_t sr300pc20_regs = {
	.default_data_type = MSM_CAMERA_I2C_BYTE_DATA,
	.output_settings = &sr300pc20_dimensions[0],
	.num_conf = ARRAY_SIZE(sr300pc20_confs),
};

static struct msm_sensor_ctrl_t sr300pc20_s_ctrl = {
	.msm_sensor_reg = &sr300pc20_regs,
	.sensor_i2c_client = &sr300pc20_sensor_i2c_client,
	.sensor_i2c_addr = 0x40,
	.sensor_output_reg_addr = &sr300pc20_reg_addr,
	.sensor_id_info = &sr300pc20_id_info,
	.cam_mode = MSM_SENSOR_MODE_INVALID,
	.csic_params = &sr300pc20_csi_params_array[0],
	.msm_sensor_mutex = &sr300pc20_mut,
	.sensor_i2c_driver = &sr300pc20_i2c_driver,
	.sensor_v4l2_subdev_info = sr300pc20_subdev_info,
	.sensor_v4l2_subdev_info_size = ARRAY_SIZE(sr300pc20_subdev_info),
	.sensor_v4l2_subdev_ops = &sr300pc20_subdev_ops,
	.func_tbl = &sr300pc20_func_tbl,
	.clk_rate = MSM_SENSOR_MCLK_24HZ,
};

module_init(msm_sensor_init_module);
MODULE_DESCRIPTION("Samsung 3MP YUV sensor driver");
MODULE_LICENSE("GPL v2");
