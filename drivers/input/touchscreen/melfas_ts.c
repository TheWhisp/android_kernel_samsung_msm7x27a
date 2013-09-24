/* drivers/input/touchscreen/melfas_ts.c
 *
 * Copyright (C) 2010 Melfas, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#define SEC_TSP
#ifdef SEC_TSP
//#define ENABLE_NOISE_TEST_MODE
#define TSP_FACTORY_TEST
#undef ENABLE_NOISE_TEST_MODE
#if defined (CONFIG_KOR_MODEL_SHV_E120S) || defined (CONFIG_KOR_MODEL_SHV_E120K) || defined (CONFIG_KOR_MODEL_SHV_E120L) \
    || defined (CONFIG_KOR_MODEL_SHV_E160S) || defined (CONFIG_KOR_MODEL_SHV_E160K) || defined(CONFIG_KOR_MODEL_SHV_E160L)
#define TSP_BOOST
#else
#undef TSP_BOOST
#endif
#endif
#undef TA_DETECTION

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/earlysuspend.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/melfas_ts.h>
#include <linux/mfd/pmic8058.h>
#ifdef SEC_TSP
#include <linux/gpio.h>
#endif

#if 1
#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#include <linux/string.h>
#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/firmware.h>
#include <asm/io.h>
#include <mach/gpio.h>
#include <mach/vreg.h>
#endif

#include <../mach-msm/smd_private.h>
#include <../mach-msm/smd_rpcrouter.h>

#define TS_MAX_Z_TOUCH			255
#define TS_MAX_W_TOUCH		100


#define TS_MAX_X_COORD		320
#define TS_MAX_Y_COORD		480

#ifdef SEC_TSP
#define P5_THRESHOLD			0x05
#define TS_READ_REGS_LEN		5
#define TS_WRITE_REGS_LEN		16
#endif

#define TS_READ_REGS_LEN		66
#define MELFAS_MAX_TOUCH		11

static int FW_VERSION;

#define DEBUG_PRINT			0

#define PRESS_KEY					1
#define RELEASE_KEY					0

#define SET_DOWNLOAD_BY_GPIO	1
#define TS_READ_VERSION_ADDR		0x1B
#define DOWNLOAD_RETRY_CNT		1

// TSP registors.
#define MIP_CONTACT_ON_EVENT_THRES	0x05
#define MIP_MOVING_EVENT_THRES		0x06
#define MIP_ACTIVE_REPORT_RATE		0x07
#define MIP_POSITION_FILTER_LEVEL	0x08

#define TS_READ_START_ADDR			0x0F
#define TS_READ_START_ADDR2			0x10

#if (CONFIG_MACH_JENA_HWREV == 0x0)
#define MIP_TSP_REVISION				0x1B
#else
#define MIP_TSP_REVISION				0xF0
#endif
#define MIP_HARDWARE_REVISION		0xF1
#define MIP_COMPATIBILITY_GROUP		0xF2
#define MIP_CORE_VERSION				0xF3
#define MIP_PRIVATECUSTOM_VERSION	0xF4
#define MIP_PUBLICCUSTOM_VERSION		0xF5
#define MIP_PRODUCT_CODE				0xF6

#define SET_TSP_CONFIG
#define TSP_PATTERN_TRACTKING
#undef TSP_PATTERN_TRACTKING

#if SET_DOWNLOAD_BY_GPIO
#include "mms100_download.h"
#endif // SET_DOWNLOAD_BY_GPIO

#define TS_READ_EXCITING_CH_ADDR	0x2E
#define TS_READ_SENSING_CH_ADDR	    0x2F
#define TS_WRITE_REGS_LEN		    16
#define RMI_ADDR_TESTMODE           0xA0
#define UNIVERSAL_CMD_RESULT_SIZE   0xAE
#define UNIVERSAL_CMD_RESULT        0xAF

#define	TSP_PWR_LDO_GPIO	41

unsigned long saved_rate;
static bool lock_status;
static int tsp_enabled;
int touch_is_pressed;
static int tsp_testmode = 0;
static int index =0;
static int gMenuKey_Intensity, gBackKey_Intensity;

extern unsigned int board_hw_revision;

#define MAX_RX_	10
#define MAX_TX_	15

static const uint16_t SCR_ABS_UPPER_SPEC[MAX_RX_][MAX_TX_] =
{
	{3575,  3495,  3470,  3459,  3444},
	{3438,  3426,  3424,  3419,  3419},
	{3419,  3415,  3411,  3410,  3370},
	{3548,  3488,  3459,  3444,  3424},
	{3413,  3400,  3391,  3389,  3383},
	{3379,  3378,  3375,  3373,  3379},
	{3554,  3500,  3475,  3464,  3448},
	{3441,  3431,  3430,  3424,  3423},
	{3423,  3420,  3418,  3416,  3374},
	{3550,  3500,  3475,  3465,  3449},
	{3444,  3433,  3431,  3428,  3424},
	{3424,  3423,  3420,  3419,  3376},
	{3548,  3501,  3476,  3466,  3450},
	{3444,  3433,  3433,  3429,  3425},
	{3425,  3424,  3421,  3420,  3378},
	{3549,  3499,  3475,  3463,  3449},
	{3441,  3431,  3431,  3428,  3424},
	{3424,  3423,  3420,  3419,  3376},
	{3555,  3499,  3476,  3461,  3450},
	{3440,  3434,  3433,  3428,  3425},
	{3425,  3424,  3423,  3420,  3378},
	{3556,  3499,  3476,  3463,  3450},
	{3441,  3434,  3434,  3430,  3425},
	{3425,  3425,  3424,  3423,  3380},
	{3549,  3486,  3459,  3440,  3424},
	{3410,  3401,  3399,  3391,  3389},
	{3384,  3380,  3380,  3379,  3385},
	{3555,  3488,  3470,  3453,  3444},
	{3433,  3429,  3450,  3429,  3423},
	{3423,  3421,  3421,  3420,  3380},

};

static const uint16_t SCR_ABS_LOWER_SPEC[MAX_RX_][MAX_TX_] =
{
	{2145, 2097, 2082, 2075, 2066},
	{2063, 2056, 2054, 2051, 2051},
	{2051, 2049, 2047, 2046, 2022},
	{2129, 2093, 2075, 2066, 2054},
	{2048, 2040, 2035, 2033, 2030},
	{2027, 2027, 2025, 2024, 2027},
	{2132, 2100, 2085, 2078, 2069},
	{2065, 2059, 2058, 2054, 2054},
	{2054, 2052, 2051, 2050, 2024},
	{2130, 2100, 2085, 2079, 2069},
	{2066, 2060, 2059, 2057, 2054},
	{2054, 2054, 2052, 2051, 2026},
	{2129, 2101, 2086, 2080, 2070},
	{2066, 2060, 2060, 2057, 2055},
	{2055, 2054, 2053, 2052, 2027},
	{2129, 2099, 2085, 2078, 2069},
	{2065, 2059, 2059, 2057, 2054},
	{2054, 2054, 2052, 2051, 2026},
	{2133, 2099, 2086, 2077, 2070},
	{2064, 2060, 2060, 2057, 2055},
	{2055, 2054, 2054, 2052, 2027},
	{2134, 2099, 2086, 2078, 2070},
	{2065, 2060, 2060, 2058, 2055},
	{2055, 2055, 2054, 2054, 2028},
	{2129, 2092, 2075, 2064, 2054},
	{2046, 2041, 2039, 2035, 2033},
	{2030, 2028, 2028, 2027, 2031},
	{2133, 2093, 2082, 2072, 2066},
	{2060, 2057, 2070, 2057, 2054},
	{2054, 2053, 2053, 2052, 2028},
};


static int g_exciting_ch, g_sensing_ch;
static unsigned char is_inputmethod = 0;
#ifdef TSP_BOOST
static unsigned char is_boost = 0;
#endif
enum
{
	None = 0,
	TOUCH_SCREEN,
	TOUCH_KEY
};

struct muti_touch_info
{
	int strength;
	int width;
	int posX;
	int posY;
};

struct melfas_ts_data
{
	uint16_t addr;
	struct i2c_client *client;
	struct input_dev *input_dev;
	struct melfas_tsi_platform_data *pdata;
	struct work_struct  work;
	struct melfas_version *version;
	uint32_t flags;
	int (*power)(int on);
	int (*gpio)(void);
#ifdef TA_DETECTION
	void (*register_cb)(void*);
	void (*read_ta_status)(void*);
#endif
#ifdef CONFIG_HAS_EARLYSUSPEND
	struct early_suspend early_suspend;
#endif
};

//struct melfas_ts_data *ts_data;
static struct melfas_ts_data *ts;

#ifdef SEC_TSP
extern struct class *sec_class;
struct device *sec_touchscreen_dev;
struct device *sec_touchkey_dev;

int menu_pressed;
int back_pressed;

#endif

#ifdef CONFIG_HAS_EARLYSUSPEND
static void melfas_ts_early_suspend(struct early_suspend *h);
static void melfas_ts_late_resume(struct early_suspend *h);
#endif
#ifdef SEC_TSP
static int melfas_ts_suspend(struct i2c_client *client, pm_message_t mesg);
static int melfas_ts_resume(struct i2c_client *client);
static void release_all_fingers(struct melfas_ts_data *ts);
static int melfas_set_config(struct i2c_client *client, u8 reg, u8 value);
static int melfas_i2c_write(struct i2c_client *client, char *buf, int length);
static void TSP_reboot(void);
#endif
static ssize_t	check_init_lowleveldata();

static struct muti_touch_info g_Mtouch_info[MELFAS_MAX_TOUCH];
int g_prevTouchStrength[MELFAS_MAX_TOUCH] = {-1,};
#define VREG_ENABLE		1
#define VREG_DISABLE	0

#define TOUCH_ON  1
#define TOUCH_OFF 0

static struct regulator *touch_regulator = NULL;

static void ts_power_enable(int en)
{
	printk(KERN_ERR "%s %s\n", __func__, (en) ? "on" : "off");

	gpio_request(TSP_PWR_LDO_GPIO, "tsp-power");

	if(en) {
		gpio_direction_output(TSP_PWR_LDO_GPIO, 1);
//		gpio_direction_output( 35, 1 ); //SCL
//		gpio_direction_output( 40 , 1 ); // SDA

	}
	else {
//		gpio_direction_output( 40 , 0 ); // SDA
//		gpio_direction_output( 35, 0 ); //SCL
		gpio_direction_output(TSP_PWR_LDO_GPIO, 0);
		}

	gpio_free(TSP_PWR_LDO_GPIO);
}


static int melfas_init_panel(struct melfas_ts_data *ts)
{
	int buf = 0x00;
	int ret;
	ret = i2c_master_send(ts->client, &buf, 1);

	ret = i2c_master_send(ts->client, &buf, 1);

	if(ret <0)
	{
		printk(KERN_ERR "%s : i2c_master_send() failed\n [%d]", __func__, ret);
		return 0;
	}

	return true;
}

#ifdef TA_DETECTION
static void tsp_ta_probe(int ta_status)
{
	u8 write_buffer[3];

	printk(KERN_ERR"[TSP] %s : TA is %s. \n", __func__, ta_status ? "on" : "off");
	if (tsp_enabled == false) {
		printk(KERN_ERR"[TSP] tsp_enabled is 0\n");
		return;
	}

	write_buffer[0] = 0xB0;
	write_buffer[1] = 0x11;

	if (ta_status)
		write_buffer[2] = 1;
	else
		write_buffer[2] = 0;

	melfas_i2c_write(ts_data->client, (char *)write_buffer, 3);
}
#endif

#ifdef TSP_BOOST
static void TSP_boost(struct melfas_ts_data *ts, bool onoff)
{
	printk(KERN_ERR "[TSP] TSP_boost %s\n", is_boost ? "ON" : "Off" );
	if (onoff) {
		melfas_set_config(ts->client, MIP_POSITION_FILTER_LEVEL, 2);
	} else {
		melfas_set_config(ts->client, MIP_POSITION_FILTER_LEVEL, 80);
	}
}
#endif

#ifdef TSP_PATTERN_TRACTKING
/* To do forced calibration when ghost touch occured at the same point
    for several second. */
#define MAX_GHOSTCHECK_FINGER				10
#define MAX_GHOSTTOUCH_COUNT					300		// 5s, 60Hz
#define MAX_GHOSTTOUCH_BY_PATTERNTRACKING	5
static int tcount_finger[MAX_GHOSTCHECK_FINGER] = {0,0,0,0,0,0,0,0,0,0};
static int touchbx[MAX_GHOSTCHECK_FINGER] = {0,0,0,0,0,0,0,0,0,0};
static int touchby[MAX_GHOSTCHECK_FINGER] = {0,0,0,0,0,0,0,0,0,0};
static int ghosttouchcount = 0;
static int cFailbyPattenTracking = 0;

static void clear_tcount(void)
{
	int i;
	for(i=0;i<MAX_GHOSTCHECK_FINGER;i++){
		tcount_finger[i] = 0;
		touchbx[i] = 0;
		touchby[i] = 0;
	}
}

static int diff_two_point(int x, int y, int oldx, int oldy)
{
	int diffx,diffy;
	int distance;

	diffx = x-oldx;
	diffy = y-oldy;
	distance = abs(diffx) + abs(diffy);

	if(distance < 3) return 1;
	else return 0;
}

static int tsp_pattern_tracking(struct melfas_ts_data *ts, int fingerindex, int x, int y)
{
	int i;
	int ghosttouch = 0;

	if( i == fingerindex){
		//if((touchbx[i] == x)&&(touchby[i] == y))
		if(diff_two_point(x,y, touchbx[i], touchby[i]))
		{
			tcount_finger[i] = tcount_finger[i]+1;
		}
		else
		{
			tcount_finger[i] = 0;
		}

		touchbx[i] = x;
		touchby[i] = y;

		if(tcount_finger[i]> MAX_GHOSTTOUCH_COUNT){
			ghosttouch = 1;
			ghosttouchcount++;
			printk(KERN_DEBUG "[TSP] SUNFLOWER (PATTERN TRACKING) %d\n",ghosttouchcount);
			clear_tcount();

			cFailbyPattenTracking++;
			if(cFailbyPattenTracking > MAX_GHOSTTOUCH_BY_PATTERNTRACKING){
				cFailbyPattenTracking = 0;
				printk("[TSP] Reboot.\n");
				TSP_reboot();
			}
			else{
				/* Do something for calibration */
			}
		}
	}
	return ghosttouch;
}
#endif

static void melfas_ts_get_data(struct work_struct *work)
{
	struct melfas_ts_data *ts = container_of(work, struct melfas_ts_data, work);
	int ret = 0, i;
	uint8_t buf[TS_READ_REGS_LEN];
	int read_num, FingerID;
	int _touch_is_pressed, line;
	int keyID = 0, touchType = 0, touchState = 0;

	if (tsp_enabled == false) {
		printk(KERN_ERR "[TSP ]%s. tsp_disabled.\n", __func__);
		return;
	}
#if DEBUG_PRINT
	printk(KERN_ERR "%s start\n", __func__);

	if(ts ==NULL)
			printk(KERN_ERR "%s : TS NULL\n", __func__);
#endif

	buf[0] = TS_READ_START_ADDR;

	ret = i2c_master_send(ts->client, buf, 1);
	if(ret < 0)
	{
		line = __LINE__;
		goto tsp_error;
	}
	ret = i2c_master_recv(ts->client, buf, 1);
	if(ret < 0)
	{
		line = __LINE__;
		goto tsp_error;
	}

	read_num = buf[0];

	if(read_num>0)
	{
		buf[0] = TS_READ_START_ADDR2;

		ret = i2c_master_send(ts->client, buf, 1);
		if(ret < 0)
		{
			line = __LINE__;
			goto tsp_error;
		}
		ret = i2c_master_recv(ts->client, buf, read_num);
		if(ret < 0)
		{
			line = __LINE__;
			goto tsp_error;
		}

		for(i=0; i<read_num; i=i+6)
		{
		if (buf[i] == 0x0F) {
			printk(KERN_ERR "%s : ESD-DETECTED!!!\n", __func__);
			line = __LINE__;
			goto tsp_error;
		}
				touchType = (buf[i] >> 5) & 0x03;  //lyon.cho

				if (touchType == TOUCH_SCREEN)
				{
			FingerID = (buf[i] & 0x0F) - 1;
			g_Mtouch_info[FingerID].posX= (uint16_t)(buf[i+1] & 0x0F) << 8 | buf[i+2];
			g_Mtouch_info[FingerID].posY= (uint16_t)(buf[i+1] & 0xF0) << 4 | buf[i+3];

			if((buf[i] & 0x80)==0)
				g_Mtouch_info[FingerID].strength = 0;
			else
				g_Mtouch_info[FingerID].strength = buf[i+4];

			g_Mtouch_info[FingerID].width= buf[i+5];
				}
				else if (touchType == TOUCH_KEY)
				{
					keyID = (buf[i] & 0x0F);
					touchState = (buf[i] & 0x80);
					gMenuKey_Intensity = 0;
					gBackKey_Intensity = 0;

			printk(KERN_ERR "[TSP] keyID : %d, touchstate : %d\n"
					, keyID, touchState);

					if (keyID == 0x1)
					{
						if (touchState)
							menu_pressed = 1;
						else
							menu_pressed = 0;

						gMenuKey_Intensity = buf[i + 5];
						input_report_key(
						ts->input_dev, KEY_MENU,
						touchState ?
						PRESS_KEY : RELEASE_KEY);
					}

					if (keyID == 0x2)
					{
						if (touchState)
							back_pressed = 1;
						else
							back_pressed = 0;

						gBackKey_Intensity = buf[i + 5];
						input_report_key(
						ts->input_dev, KEY_BACK,
						touchState ?
						PRESS_KEY : RELEASE_KEY);
					}
				}
		}

	}

	_touch_is_pressed=0;

	for(i=0; i<MELFAS_MAX_TOUCH; i++)
	{
		if(g_Mtouch_info[i].strength== -1) {
			g_prevTouchStrength[i] = -1;
			continue;
		}

#ifdef TSP_PATTERN_TRACTKING
		tsp_pattern_tracking(ts, i, g_Mtouch_info[i].posX, g_Mtouch_info[i].posY);
#endif
		input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, i);
		input_report_abs(ts->input_dev, ABS_MT_POSITION_X, g_Mtouch_info[i].posX);
		input_report_abs(ts->input_dev, ABS_MT_POSITION_Y, g_Mtouch_info[i].posY);
		input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, g_Mtouch_info[i].strength );
		input_report_abs(ts->input_dev, ABS_MT_WIDTH_MAJOR, g_Mtouch_info[i].width);
		input_mt_sync(ts->input_dev);
#if DEBUG_PRINT
		printk(KERN_ERR "[TSP] ID: %d, State : %d, x: %d, y: %d, z: %d w: %d\n",
			i, (g_Mtouch_info[i].strength>0), g_Mtouch_info[i].posX, g_Mtouch_info[i].posY, g_Mtouch_info[i].strength, g_Mtouch_info[i].width);
#endif

		if(g_Mtouch_info[i].strength == 0) {
			g_Mtouch_info[i].strength = -1;
			printk(KERN_ERR "[TSP] %d - R\n",i);
		}
		
		if(g_Mtouch_info[i].strength > 0 && g_prevTouchStrength[i] < 0)
			printk(KERN_ERR "[TSP] %d - P\n",i);

		if(g_Mtouch_info[i].strength > 0)
			_touch_is_pressed = 1;
				
		g_prevTouchStrength[i] = g_Mtouch_info[i].strength;

	}
	input_sync(ts->input_dev);
	touch_is_pressed = _touch_is_pressed;

	return ;

tsp_error:
	printk(KERN_ERR "[TSP] %s: i2c failed(%d)\n", __func__, line);
	TSP_reboot();


}

static irqreturn_t melfas_ts_irq_handler(int irq, void *handle)
{
	struct melfas_ts_data *ts = (struct melfas_ts_data *)handle;
#if DEBUG_PRINT
	printk(KERN_ERR "melfas_ts_irq_handler\n");
#endif

	melfas_ts_get_data(&ts->work);

	return IRQ_HANDLED;
}

#ifdef SEC_TSP
static int melfas_i2c_read(struct i2c_client *client, u16 addr, u16 length, u8 *value)
{
	struct i2c_adapter *adapter = client->adapter;
	struct i2c_msg msg[2];

	msg[0].addr  = client->addr;
	msg[0].flags = 0x00;
	msg[0].len   = 2;
	msg[0].buf   = (u8 *) &addr;

	msg[1].addr  = client->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len   = length;
	msg[1].buf   = (u8 *) value;

	if  (i2c_transfer(adapter, msg, 2) == 2)
		return 0;
	else
		return -EIO;

}


static int melfas_i2c_write(struct i2c_client *client, char *buf, int length)
{
	int i;
	char data[TS_WRITE_REGS_LEN];

	if (length > TS_WRITE_REGS_LEN) {
		pr_err("[TSP] size error - %s\n", __func__);
		return -EINVAL;
	}

	for (i = 0; i < length; i++)
		data[i] = *buf++;

	i = i2c_master_send(client, (char *)data, length);

	if (i == length)
		return length;
	else
		return -EIO;
}

static ssize_t set_tsp_firm_version_show(struct device *dev, struct device_attribute *attr, char *buf)
{
//struct melfas_ts_data *ts = dev_get_drvdata(dev);

	return sprintf(buf, "%#02x, %#02x, %#02x\n", ts->version->core, ts->version->private, ts->version->public);
}

static ssize_t set_tsp_firm_version_read_show(struct device *dev, struct device_attribute *attr, char *buf)
{
//struct melfas_ts_data *ts = dev_get_drvdata(dev);
	u8 fw_latest_version, privatecustom_version, publiccustom_version;
	int ret;
	uint8_t buff[4] = {0,};

	buff[0] = MIP_TSP_REVISION;
	ret = i2c_master_send(ts->client, &buff, 1);
	if(ret < 0)
	{
		printk(KERN_ERR "%s : i2c_master_send [%d]\n", __func__, ret);
	}

	ret = i2c_master_recv(ts->client, &buff, 7);
	if(ret < 0)
	{
		printk(KERN_ERR "%s : i2c_master_recv [%d]\n", __func__, ret);
	}
	fw_latest_version		= buff[3];
	privatecustom_version	= buff[4];
	publiccustom_version	= buff[5];

	return sprintf(buf, "%#02x, %#02x, %#02x\n", fw_latest_version, privatecustom_version, publiccustom_version);
}

static ssize_t set_tsp_threshold_mode_show(struct device *dev, struct device_attribute *attr, char *buf)
{
//struct melfas_ts_data *ts = dev_get_drvdata(dev);
	u8 threshold;

	melfas_i2c_read(ts->client, P5_THRESHOLD, 1, &threshold);

	return sprintf(buf, "%d\n", threshold);
}

ssize_t set_tsp_for_inputmethod_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	printk(KERN_ERR "[TSP] %s is called.. is_inputmethod=%d\n", __func__, is_inputmethod);
	if (is_inputmethod)
		*buf = '1';
	else
		*buf = '0';

	return 0;
}
ssize_t set_tsp_for_inputmethod_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
//struct melfas_ts_data *ts = dev_get_drvdata(dev);
	unsigned int register_address = 0;

	if (tsp_enabled == false) {
		printk(KERN_ERR "[TSP ]%s. tsp_enabled is 0\n", __func__);
		return 1;
	}


	if (*buf == '1' && (!is_inputmethod)) {
		is_inputmethod = 1;
		printk(KERN_ERR "[TSP] Set TSP inputmethod IN\n");
		/* to do */
	} else if (*buf == '0' && (is_inputmethod)) {
		is_inputmethod = 0;
		printk(KERN_ERR "[TSP] Set TSP inputmethod OUT\n");
		/* to do */
	}
	return 1;
}

static ssize_t tsp_call_release_touch(struct device *dev, struct device_attribute *attr, char *buf)
{
//struct melfas_ts_data *ts = dev_get_drvdata(dev);

	printk(" %s is called\n", __func__);
	TSP_reboot();

	return sprintf(buf,"0\n");
}

static ssize_t tsp_touchtype_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	char temp[15];

	sprintf(temp, "TSP : MMS144\n");
	strcat(buf, temp);
	return strlen(buf);
}

#ifdef TSP_BOOST
ssize_t set_tsp_for_boost_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	printk(KERN_ERR "[TSP] %s is called.. is_inputmethod=%d\n", __func__, is_boost);
	if (is_boost)
		*buf = '1';
	else
		*buf = '0';

	return 0;
}
ssize_t set_tsp_for_boost_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
//struct melfas_ts_data *ts = dev_get_drvdata(dev);
	u16 obj_address = 0;
	u16 size_one;
	int ret;
	u8 value;
	int jump_limit = 0;
	int mrgthr = 0;
	u8 val = 0;
	unsigned int register_address = 0;

	if (tsp_enabled == false) {
		printk(KERN_ERR "[TSP ]%s. tsp_enabled is 0\n", __func__);
		return 1;
	}

	if (*buf == '1' && (!is_boost)) {
		is_boost = 1;
	} else if (*buf == '0' && (is_boost)) {
		is_boost = 0;
	}
	printk(KERN_ERR "[TSP] set_tsp_for_boost_store() called. %s!\n", is_boost ? "On" : "Off" );
	TSP_boost(ts, is_boost);

	return 1;
}
#endif
#if 0
static DEVICE_ATTR(tsp_threshold, S_IRUGO | S_IWUSR | S_IWGRP, set_tsp_threshold_mode_show, NULL);
static DEVICE_ATTR(set_tsp_for_inputmethod, S_IRUGO | S_IWUSR | S_IWGRP, set_tsp_for_inputmethod_show, set_tsp_for_inputmethod_store); /* For 3x4 Input Method, Jump limit changed API */
static DEVICE_ATTR(call_release_touch, S_IRUGO | S_IWUSR | S_IWGRP, tsp_call_release_touch, NULL);
static DEVICE_ATTR(mxt_touchtype, S_IRUGO | S_IWUSR | S_IWGRP,	tsp_touchtype_show, NULL);
#ifdef TSP_BOOST
static DEVICE_ATTR(set_tsp_boost, S_IRUGO | S_IWUSR | S_IWGRP, set_tsp_for_boost_show, set_tsp_for_boost_store); /* Control wait_filter to boost response. */
#endif

static struct attribute *sec_touch_attributes[] = {
	&dev_attr_tsp_firm_version_phone.attr,
	&dev_attr_tsp_firm_version_panel.attr,
	&dev_attr_tsp_threshold.attr,
	&dev_attr_set_tsp_for_inputmethod.attr,
	&dev_attr_call_release_touch.attr,
	&dev_attr_mxt_touchtype.attr,
#ifdef TSP_BOOST
	&dev_attr_set_tsp_boost.attr,
#endif
	NULL,
};
#endif
#if 0
static struct attribute_group sec_touch_attr_group = {
	.attrs = sec_touch_attributes,
};
#endif
#endif

#ifdef TSP_FACTORY_TEST
static bool debug_print = true;
static u16 inspection_data[180] = { 0, };
static u16 lntensity_data[180] = { 0, };
static u16 CmDelta_data[176] = { 0, }; /* inspection */
static u16 CmABS_data[176] = { 0, }; /* reference */

static ssize_t set_tsp_module_on_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int ret;
//struct melfas_ts_data *ts = dev_get_drvdata(dev);

	ret = melfas_ts_resume(ts->client);

	if (ret  = 0)
		*buf = '1';
	else
		*buf = '0';

	msleep(500);
	return 0;
}

static ssize_t set_tsp_module_off_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int ret;
//struct melfas_ts_data *ts = dev_get_drvdata(dev);

	ret = melfas_ts_suspend(ts->client, PMSG_SUSPEND);

	if (ret  = 0)
		*buf = '1';
	else
		*buf = '0';

	return 0;
}

/* CM ABS */
static int check_debug_data(struct melfas_ts_data *ts)
{
	u8 setLowLevelData[4];
	u8 read_data_buf[50] = {0,};
	u16 read_data_buf1[50] = {0,};
	int read_data_len, sensing_ch, exciting_ch;
	int ret, i,j, status;
	int size;

	tsp_testmode = 1;

	printk("[TSP] %s entered. line : %d, \n", __func__,__LINE__);

	disable_irq(ts->client->irq);

	exciting_ch = g_exciting_ch;
	sensing_ch = g_sensing_ch;

	/* Read Reference Data */
	setLowLevelData[0] = 0xA0; /* UNIVERSAL_CMD */
	setLowLevelData[1] = 0x40; /* UNIVCMD_ENTER_TEST_MODE */

	ret = melfas_i2c_write(ts->client, setLowLevelData, 2);

	while (gpio_get_value(GPIO_TOUCH_INT))
		;

	ret = melfas_i2c_read(ts->client, 0xAE, 1, read_data_buf);

	printk(KERN_ERR "\n\n --- CM_ABS --- ");

	/* Read Reference Data */
	setLowLevelData[0] = 0xA0;
	setLowLevelData[1] = 0x43; /* UNIVCMD_TEST_CM_ABS */

	ret = melfas_i2c_write(ts->client, setLowLevelData, 2);

	while (gpio_get_value(GPIO_TOUCH_INT))
		;

	ret = melfas_i2c_read(ts->client, 0xAE, 1, read_data_buf);

	printk(KERN_ERR "[TSP] %s ret= %d\n", __func__, ret);

	for (i = 0; i < sensing_ch; i++) {
		for (j = 0; j < exciting_ch; j++) {
			setLowLevelData[0] = 0xA0;
			setLowLevelData[1] = 0x44;
			setLowLevelData[2] = j;
			setLowLevelData[3] = i;

			ret = melfas_i2c_write(ts->client, setLowLevelData, 4);

			while (gpio_get_value(GPIO_TOUCH_INT))
				;

			ret = melfas_i2c_read(ts->client, 0xAE,
				1, read_data_buf);

			size = read_data_buf[0];

			ret = melfas_i2c_read(ts->client, 0xAF,
				size, read_data_buf);

			CmABS_data[(i * exciting_ch) + j]
				= (read_data_buf[0] |  read_data_buf[1] << 8);

			if ((CmABS_data[(i * exciting_ch) + j]
					>= SCR_ABS_LOWER_SPEC[i][j])
				&& (CmABS_data[(i * exciting_ch) + j]
					<= SCR_ABS_UPPER_SPEC[i][j]))
				status = 1; /* fail */
			else
				status = 0; /* pass */
		}
	}

	printk(KERN_ERR "[TSP] CmABS_data\n");

	for (i = 0; i < exciting_ch * sensing_ch; i++) {
		if (0 == i % exciting_ch)
			printk("\n");
		printk("%4d, ", CmABS_data[i]);
	}
	printk("\n");

	/* Read Reference Data */
	setLowLevelData[0] = 0xA0;
	setLowLevelData[1] = 0x4F;

	ret = melfas_i2c_write(ts->client, setLowLevelData, 2);

	enable_irq(ts->client->irq);
	tsp_testmode = 0;
	TSP_reboot();

	printk(KERN_ERR "%s : end\n", __func__);

	return status;
}

/* inspection = CmDelta_data */
static int check_delta_data(struct melfas_ts_data *ts)
{
	u8 setLowLevelData[4];
	u8 read_data_buf[50] = {0,};
	u16 read_data_buf1[50] = {0,};
	int read_data_len,sensing_ch, exciting_ch;
	int ret, i, j, status;
	int size;

	printk(KERN_ERR "[TSP] %s entered. line : %d,\n", __func__, __LINE__);

	disable_irq(ts->client->irq);

	exciting_ch = g_exciting_ch;
	sensing_ch	 = g_sensing_ch;

	/* Read Reference Data */
	setLowLevelData[0] = 0xA0; /* UNIVERSAL_CMD */
	setLowLevelData[1] = 0x40; /* UNIVCMD_ENTER_TEST_MODE */

	ret = melfas_i2c_write(ts->client, setLowLevelData, 2);

	while (gpio_get_value(GPIO_TOUCH_INT))
		;

	ret = melfas_i2c_read(ts->client, 0xAE, 1, read_data_buf);

	printk(KERN_ERR "\n\n --- CM_DELTA --- ");

	/* Read Reference Data */
	setLowLevelData[0] = 0xA0;
	setLowLevelData[1] = 0x41;

	ret = melfas_i2c_write(ts->client, setLowLevelData, 2);

	while (gpio_get_value(GPIO_TOUCH_INT))
		;

	ret = melfas_i2c_read(ts->client, 0xAE, 1, read_data_buf);

	for (i = 0; i < sensing_ch; i++) {
		for (j = 0; j < exciting_ch; j++) {
			setLowLevelData[0] = 0xA0;
			setLowLevelData[1] = 0x42;
			setLowLevelData[2] = j; /* Exciting CH. */
			setLowLevelData[3] = i; /* Sensing CH. */

			ret = melfas_i2c_write(ts->client, setLowLevelData, 4);

			while (gpio_get_value(GPIO_TOUCH_INT))
				;

			ret = melfas_i2c_read(ts->client, 0xAE,
				1, read_data_buf);

			size = read_data_buf[0];

			ret = melfas_i2c_read(ts->client, 0xAF,
				read_data_buf[0], read_data_buf);

			CmDelta_data[(i * exciting_ch) + j]
				= (read_data_buf[0] |  read_data_buf[1] << 8);

		}
	}

	printk(KERN_ERR "[TSP] CmDelta_data\n");

	for (i = 0; i < exciting_ch * sensing_ch; i++) {
		if (0 == i % exciting_ch)
			printk("\n");
		printk("%4d, ", CmDelta_data[i]);
	}
	printk("\n");

	/* Read Reference Data */
	setLowLevelData[0] = 0xA0;
	setLowLevelData[1] = 0x4F;

	ret = melfas_i2c_write(ts->client, setLowLevelData, 2);
	enable_irq(ts->client->irq);
	tsp_testmode = 0;
	TSP_reboot();

	printk(KERN_ERR "%s : end\n", __func__);

	return status;
}

static int atoi(char *str)
{
	int result = 0;
	int count = 0;

	if (str == NULL)
		return result;

	while (str[count] != NULL && str[count] >= '0' && str[count] <= '9') {
		result = result * 10 + str[count] - '0';
		++count;
	}
	return result;
}

ssize_t disp_all_refdata_show(struct device *dev, \
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, 5, "%u\n", CmABS_data[index]);
}

ssize_t disp_all_refdata_store(struct device *dev, \
	struct device_attribute *attr, const char *buf, size_t size)
{
	index = atoi(buf);

	printk(KERN_ERR "%s : value %d\n", __func__, index);

	return size;
}

static ssize_t set_all_delta_mode_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int status = 0;

/* Delta */
	check_delta_data(ts);

	set_tsp_module_off_show(dev, attr, buf);
	set_tsp_module_on_show(dev, attr, buf);

	return sprintf(buf, "%u\n", status);
}

static ssize_t set_all_refer_mode_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int status = 0;

/* ABS */
	status = check_debug_data(ts);

	set_tsp_module_off_show(dev, attr, buf);
	set_tsp_module_on_show(dev, attr, buf);

	return sprintf(buf, "%u\n", status);
}

ssize_t disp_all_deltadata_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	printk("disp_all_deltadata_show : value %d\n", CmDelta_data[index]);
	return sprintf(buf, "%u\n",  CmDelta_data[index]);
}


ssize_t disp_all_deltadata_store(struct device *dev, struct device_attribute *attr,
								   const char *buf, size_t size)
{
	index = atoi(buf);
	printk(KERN_ERR "Delta data %d", index);
	return size;
}

static void check_intensity_data(struct melfas_ts_data *ts)
{
	int16_t menu = 0;
	int16_t back = 0;
	u8 setLowLevelData[6];
	u8 read_data_buf[50] = {0,};
	int sensing_ch, exciting_ch;
	int ret, i,j;

	printk(KERN_ERR "[TSP] %s entered. line : %d\n", __func__, __LINE__);

	menu = gMenuKey_Intensity;
	back = gBackKey_Intensity;

	exciting_ch = g_exciting_ch;
	sensing_ch = g_sensing_ch;

	disable_irq(ts->client->irq);

	for (i = 0; i < sensing_ch; i++) {
		for (j = 0 ; j < exciting_ch; j++) {
			setLowLevelData[0] = 0xB0; /* VENDOR_SPECIFIC_CMD */
			setLowLevelData[1] = 0x1A; /* VENDOR_CMD_SS_TSP_S */
			setLowLevelData[2] = j;
			setLowLevelData[3] = i;
			setLowLevelData[4] = 0x00; /* Reserved */
			setLowLevelData[5] = 0x04;

			ret = melfas_i2c_write(ts->client, setLowLevelData, 6);
			ret = melfas_i2c_read(ts->client, 0xBF,
				1, read_data_buf);

			lntensity_data[(i * exciting_ch) + j]
				= read_data_buf[0];
		}

/*
		if (i == 0)
				lntensity_data[(i * exciting_ch) + j-1] = menu;
		else if (i == 1)
				lntensity_data[(i * exciting_ch) + j-1] = back;
		*/
	}


	setLowLevelData[0] = 0xB0; /* VENDOR_SPECIFIC_CMD */
	setLowLevelData[1] = 0x1A; /* VENDOR_CMD_SS_TSP_S */
	setLowLevelData[2] = 0x00;
	setLowLevelData[3] = 0x00;
	setLowLevelData[4] = 0x00; /* Reserved */
	setLowLevelData[5] = 0x05; /* VENDOR_CMD_SS_TSP_S_EXIT_MODE */
	ret = melfas_i2c_write(ts->client, setLowLevelData, 6);

#if 1
	printk(KERN_ERR "[TSP] lntensity_data\n");

	for (i = 0; i < exciting_ch * sensing_ch; i++) {
		if (0 == i % exciting_ch)
			printk("\n");
		printk("%4d, ", lntensity_data[i]);
	}
	printk("\n");
#endif

	enable_irq(ts->client->irq);
	msleep(20);
	check_delta_data(ts);

	printk(KERN_ERR "%s : end\n", __func__);
}

static ssize_t set_refer0_mode_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	u16 refrence = 0;
//	struct melfas_ts_data *ts = dev_get_drvdata(dev);

	check_intensity_data(ts);

	refrence = inspection_data[28];
	return sprintf(buf, "%u\n", refrence);
}

static ssize_t set_refer1_mode_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	u16 refrence = 0;
	refrence = inspection_data[288];
	return sprintf(buf, "%u\n", refrence);
}

static ssize_t set_refer2_mode_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	u16 refrence = 0;
	refrence = inspection_data[194];
	return sprintf(buf, "%u\n", refrence);
}

static ssize_t set_refer3_mode_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	u16 refrence = 0;
	refrence = inspection_data[49];
	return sprintf(buf, "%u\n", refrence);
}

static ssize_t set_refer4_mode_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	u16 refrence = 0;
	refrence = inspection_data[309];
	return sprintf(buf, "%u\n", refrence);
}

static ssize_t set_intensity0_mode_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	u16 intensity = 0;
	intensity = lntensity_data[28];
	return sprintf(buf, "%u\n", intensity);
}

static ssize_t set_intensity1_mode_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	u16 intensity = 0;
	intensity = lntensity_data[288];
	return sprintf(buf, "%u\n", intensity);
}

static ssize_t set_intensity2_mode_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	u16 intensity = 0;
	intensity = lntensity_data[194];
	return sprintf(buf, "%u\n", intensity);
}

static ssize_t set_intensity3_mode_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	u16 intensity = 0;
	intensity = lntensity_data[49];
	return sprintf(buf, "%u\n", intensity);
}

static ssize_t set_intensity4_mode_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	u16 intensity = 0;
	intensity = lntensity_data[309];
	return sprintf(buf, "%u\n", intensity);
}


//static DEVICE_ATTR(set_module_on, S_IRUGO | S_IWUSR | S_IWGRP, set_tsp_module_on_show, NULL);
//static DEVICE_ATTR(set_module_off, S_IRUGO | S_IWUSR | S_IWGRP, set_tsp_module_off_show, NULL);
//static DEVICE_ATTR(set_all_refer, S_IRUGO | S_IWUSR | S_IWGRP, set_all_refer_mode_show, NULL);
//static DEVICE_ATTR(disp_all_refdata, S_IRUGO | S_IWUSR | S_IWGRP, disp_all_refdata_show, disp_all_refdata_store);
//static DEVICE_ATTR(set_all_delta, S_IRUGO | S_IWUSR | S_IWGRP, set_all_delta_mode_show, NULL);
//static DEVICE_ATTR(disp_all_deltadata, S_IRUGO | S_IWUSR | S_IWGRP, disp_all_deltadata_show, disp_all_deltadata_store);
static DEVICE_ATTR(set_refer0, S_IRUGO | S_IWUSR | S_IWGRP, set_refer0_mode_show, NULL);
static DEVICE_ATTR(set_delta0, S_IRUGO | S_IWUSR | S_IWGRP, set_intensity0_mode_show, NULL);
static DEVICE_ATTR(set_refer1, S_IRUGO | S_IWUSR | S_IWGRP, set_refer1_mode_show, NULL);
static DEVICE_ATTR(set_delta1, S_IRUGO | S_IWUSR | S_IWGRP, set_intensity1_mode_show, NULL);
static DEVICE_ATTR(set_refer2, S_IRUGO | S_IWUSR | S_IWGRP, set_refer2_mode_show, NULL);
static DEVICE_ATTR(set_delta2, S_IRUGO | S_IWUSR | S_IWGRP, set_intensity2_mode_show, NULL);
static DEVICE_ATTR(set_refer3, S_IRUGO | S_IWUSR | S_IWGRP, set_refer3_mode_show, NULL);
static DEVICE_ATTR(set_delta3, S_IRUGO | S_IWUSR | S_IWGRP, set_intensity3_mode_show, NULL);
static DEVICE_ATTR(set_refer4, S_IRUGO | S_IWUSR | S_IWGRP, set_refer4_mode_show, NULL);
static DEVICE_ATTR(set_delta4, S_IRUGO | S_IWUSR | S_IWGRP, set_intensity4_mode_show, NULL);
static DEVICE_ATTR(set_threshould, S_IRUGO | S_IWUSR | S_IWGRP, set_tsp_threshold_mode_show, NULL);	/* touch threshold return */

static struct attribute *sec_touch_facotry_attributes[] = {
//	&dev_attr_set_module_on.attr,
//	&dev_attr_set_module_off.attr,
//	&dev_attr_set_all_refer.attr,
//	&dev_attr_disp_all_refdata.attr,
//	&dev_attr_set_all_delta.attr,
//	&dev_attr_disp_all_deltadata.attr,
	&dev_attr_set_refer0.attr,
	&dev_attr_set_delta0.attr,
	&dev_attr_set_refer1.attr,
	&dev_attr_set_delta1.attr,
	&dev_attr_set_refer2.attr,
	&dev_attr_set_delta2.attr,
	&dev_attr_set_refer3.attr,
	&dev_attr_set_delta3.attr,
	&dev_attr_set_refer4.attr,
	&dev_attr_set_delta4.attr,
	&dev_attr_set_threshould.attr,
	NULL,
};

static struct attribute_group sec_touch_factory_attr_group = {
	.attrs = sec_touch_facotry_attributes,
};
#endif


static void release_all_fingers(struct melfas_ts_data *ts)
{
	int i;

	printk(KERN_ERR "%s start.\n", __func__);
	for(i=0; i<MELFAS_MAX_TOUCH; i++) {
		if(-1 == g_Mtouch_info[i].strength) {
			g_Mtouch_info[i].posX = 0;
			g_Mtouch_info[i].posY = 0;
			continue;
		}
	printk(KERN_ERR "%s %s(%d)\n", __func__, ts->input_dev->name, i);

		g_Mtouch_info[i].strength = 0;

		input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, i);
		input_report_abs(ts->input_dev, ABS_MT_POSITION_X, g_Mtouch_info[i].posX);
		input_report_abs(ts->input_dev, ABS_MT_POSITION_Y, g_Mtouch_info[i].posY);
		input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, g_Mtouch_info[i].strength);
		input_report_abs(ts->input_dev, ABS_MT_WIDTH_MAJOR, g_Mtouch_info[i].strength);
		input_mt_sync(ts->input_dev);

		g_Mtouch_info[i].posX = 0;
		g_Mtouch_info[i].posY = 0;

		if(0 == g_Mtouch_info[i].strength)
			g_Mtouch_info[i].strength = -1;
	}
	input_sync(ts->input_dev);
}

static void TSP_reboot(void)
{

#if 1
	printk(KERN_ERR "%s start!\n", __func__);
	disable_irq_nosync(ts->client->irq);

	release_all_fingers(ts);

	ts_power_enable(0);
	msleep(500);
	ts_power_enable(1);


	enable_irq(ts->client->irq);

#else
	if(tsp_enabled == false)
		return;

	printk(KERN_ERR "%s satrt!\n", __func__);

	disable_irq_nosync(ts->client->irq);
	tsp_enabled = false;
	touch_is_pressed = 0;

	release_all_fingers(ts);
	ts->gpio();
	ts->power(false);
	msleep(200);
	ts->power(true);
#ifdef TSP_BOOST
	TSP_boost(ts, is_boost);
#endif
	enable_irq(ts->client->irq);

	tsp_enabled = true;
#endif
};

void TSP_force_released(void)
{
	printk(KERN_ERR "%s satrt!\n", __func__);

	if (tsp_enabled == false) {
		printk(KERN_ERR "[TSP] Disabled\n");
		return;
	}
	release_all_fingers(ts);

	touch_is_pressed = 0;
};
EXPORT_SYMBOL(TSP_force_released);

void TSP_ESD_seq(void)
{
	TSP_reboot();
	printk(KERN_ERR "%s satrt!\n", __func__);
};
EXPORT_SYMBOL(tsp_call_release_touch);

#ifdef SET_TSP_CONFIG
static int melfas_set_config(struct i2c_client *client, u8 reg, u8 value)
{
	u8 buffer[2];
	int ret;
	struct melfas_ts_data *ts = i2c_get_clientdata(client);

	buffer[0] = reg;
	buffer[1] = value;
	ret = melfas_i2c_write(ts->client, (char *)buffer, 2);

	return ret;
}
#endif

int tsp_i2c_read_melfas(u8 reg, unsigned char *rbuf, int buf_size) //same with tsp_i2c_read()
{
	int i, ret=-1;
	struct i2c_msg rmsg;
	uint8_t start_reg;
  int retry = 3;

	for (i = 0; i < retry; i++)
	{
		rmsg.addr = ts->client->addr;
		rmsg.flags = 0;//I2C_M_WR;
		rmsg.len = 1;
		rmsg.buf = &start_reg;
		start_reg = reg;

		ret = i2c_transfer(ts->client->adapter, &rmsg, 1);

		if(ret >= 0)
		{
			rmsg.flags = I2C_M_RD;
			rmsg.len = buf_size;
			rmsg.buf = rbuf;
			ret = i2c_transfer(ts->client->adapter, &rmsg, 1 );

			if (ret >= 0)
				break; // i2c success
		}

		if( i == (retry - 1) )
		{
			printk("[TSP] Error code : %d, %d\n", __LINE__, ret );
		}
	}

	return ret;
}

#if 0
static int melfas_i2c_read(struct i2c_client* p_client, u8 reg, u8* data, int len)
{
	struct i2c_msg msg;

	/* set start register for burst read */
	/* send separate i2c msg to give STOP signal after writing. */
	/* Continous start is not allowed for cypress touch sensor. */

	msg.addr = p_client->addr;
	msg.flags = 0;
	msg.len = 1;
	msg.buf = &reg;

	if (1 != i2c_transfer(p_client->adapter, &msg, 1))
	{
		printk("[TSP][MMS128][%s] set data pointer fail! reg(%x)\n", __func__, reg);
		return -EIO;
	}

	/* begin to read from the starting address */

	msg.addr = p_client->addr;
	msg.flags = I2C_M_RD;
	msg.len = len;
	msg.buf = data;

	if (1 != i2c_transfer(p_client->adapter, &msg, 1))
	{
		printk("[TSP][MMS128][%s] fail! reg(%x)\n", __func__, reg);
		return -EIO;
	}

	return 0;
}
#endif

static ssize_t firmware_panel_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	int ret, i;
	uint8_t buf_tmp[6] = {0,};
	int retry = 2;
	printk(KERN_ERR "HWREV = %d\n", board_hw_revision);
#if (CONFIG_MACH_JENA_HWREV == 0x0)
	ret = melfas_i2c_read(ts->client, MIP_TSP_REVISION, 3, buf_tmp);
#else
	ret = melfas_i2c_read(ts->client, MIP_TSP_REVISION, 6, buf_tmp);
#endif

	// i2c read retry
	if(ret < 0)
	{
		for (i = 0; i < retry; i++)
		{
#if (CONFIG_MACH_JENA_HWREV == 0x0)
	ret = melfas_i2c_read(ts->client, MIP_TSP_REVISION, 3, buf_tmp);
#else
	ret = melfas_i2c_read(ts->client, MIP_TSP_REVISION, 6, buf_tmp);
#endif
			if(ret > 0)
				break;
		}
	}
#if (CONFIG_MACH_JENA_HWREV == 0x0)
	printk(KERN_ERR "firmware_phone_show : [%d], [%d], [%d]\n", \
		buf_tmp[0], buf_tmp[1], buf_tmp[2]);
#else
	printk(KERN_ERR "firmware_phone_show : [%d], [%d], [%d], [%d]\n", \
		buf_tmp[1], buf_tmp[2], buf_tmp[3], buf_tmp[5]);
#endif

#if (CONFIG_MACH_JENA_HWREV == 0x0)
	return sprintf(buf, "%d\n", buf_tmp[2]);
#else
	return sprintf(buf, "%d\n", buf_tmp[5]);
#endif
}

static ssize_t firmware_phone_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	int NEW_FIRMWARE_VERSION = FW_VERSION;

	// Read TSP version of Phone
	return sprintf(buf, "%d\n", NEW_FIRMWARE_VERSION);// kernel source version
}


static ssize_t firmware_show(struct device *dev, struct device_attribute *attr, char *buf)  //AAAAA
{
	u8 buf1[6] = {0,};
	int hw_rev, fw_ver, phone_ver;

#if (CONFIG_MACH_JENA_HWREV == 0x0)
	if (0 == melfas_i2c_read(ts->client, MIP_TSP_REVISION, 3, buf1))
#else
	if (0 == melfas_i2c_read(ts->client, MIP_TSP_REVISION, 6, buf1))
#endif
	{
#if (CONFIG_MACH_JENA_HWREV == 0x0)
		hw_rev = buf1[0];
		fw_ver = buf1[2];
#else
		hw_rev = buf1[1];
		fw_ver = buf1[5];
#endif
		phone_ver = FW_VERSION;

		/*
			TSP phone Firmware version : phone_ver (xx)
			TSP IC	  Firmware version   : fw_ver (xx)
			HW				  version	  : hw_rev  (xxx)
		*/

		sprintf(buf, "%03X%02X%02X\n", hw_rev, fw_ver, phone_ver); // 10003xx

		printk("[TSP][MMS128][%s]  phone_ver=%d, fw_ver=%d, hw_rev=%d\n",buf, phone_ver,fw_ver, hw_rev );

	}
	else
	{
		printk("[TSP][MMS128][%s] Can't find HW Ver, FW ver!\n", __func__);
	}

	return sprintf(buf, "%s", buf);
}

static ssize_t firmware_store(struct device *dev, struct device_attribute *attr,
						const char *buf, size_t size)
{
	int ret;

	printk(KERN_INFO "START firmware store\n");

	ts_power_enable(0);
	msleep(500);
	ts_power_enable(1);
	msleep(500);

	disable_irq(ts->client->irq);
	local_irq_disable();

//	cancel_work_sync(&ts->work_timer);
//	hrtimer_cancel(&ts->timer);

	ret = mcsdl_download_binary_data( 1 );

	local_irq_enable();
	enable_irq(ts->client->irq);

#if 0
	hrtimer_start(&ts->timer, ktime_set(0, 200000000), HRTIMER_MODE_REL);

#if defined (__TOUCH_TA_CHECK__)		// for AT&T Charger
	b_Firmware_store = false;
#endif

	if(ret == MCSDL_RET_SUCCESS)
		firmware_ret_val = 1;
	else
		firmware_ret_val = 0;
#endif
	printk("[TSP] Firmware update end!!\n" );


	ts_power_enable(0);
	msleep(500);
	ts_power_enable(1);

	printk(KERN_INFO "firmware store END\n");
	return 0;
}

static ssize_t tsp_threshold_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	//struct mcs8000_ts_driver *ts = dev_get_drvdata(dev);
	u8 threshold=32;

	return sprintf(buf, "%d\n", threshold);
}

static ssize_t tsp_firm_update_status_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	//struct mcs8000_ts_driver *ts = dev_get_drvdata(dev);
	u8 firm_update_status=0;

	return sprintf(buf, "%d\n", firm_update_status);
}

static ssize_t touchkey_back_show(struct device *dev,	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, sizeof(int), "%x\n", back_pressed);
}

static ssize_t touchkey_menu_show(struct device *dev,	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, sizeof(int), "%x\n", menu_pressed);
}

static ssize_t	check_init_lowleveldata()
{
	u8 read_buf[1] = {0,};
	int ret=1;

//	if(init_lowleveldata)
//	{
		//////////////////////
		// read Exciting CH.
		//////////////////////
		ret = melfas_i2c_read(ts->client, 0x2e, 1, read_buf);
		if(ret < 0)
		{
			printk("[TSP] Exciting CH. melfas_i2c_read fail! %s : %d, \n", __func__,__LINE__);
			return 0;
		}
		g_exciting_ch = read_buf[0]; // 15

		//////////////////////
		// read Sensing CH.
		//////////////////////
		ret = melfas_i2c_read(ts->client, 0x2f, 1, read_buf);
		if(ret < 0)
		{
			printk("[TSP] Sensing CH. melfas_i2c_read fail! %s : %d, \n", __func__,__LINE__);
			return 0;
		}
		g_sensing_ch = read_buf[0]; // 10

//		init_lowleveldata = false;
//	}
	return ret;
}

static int start_rawcounter = 1;
static ssize_t tkey_rawcounter_store(struct device *dev, \
struct device_attribute *attr, const char *buf, size_t size)
{
	char *after;

	unsigned long value = simple_strtoul(buf, &after, 10);
	printk(KERN_INFO "[TSP] %s, %d, value=%d\n", __func__, __LINE__, value);

	return size;
}

static ssize_t tkey_rawcounter_show(struct device *dev, \
struct device_attribute *attr, char *buf)
{
	printk(KERN_ERR "[TSP] menukey : %d, backKey : %d\n", \
	gMenuKey_Intensity, gBackKey_Intensity);
	mdelay(1);
	return snprintf(buf, 10, "%d %d\n", \
	gMenuKey_Intensity, gBackKey_Intensity);
}

static ssize_t set_module_off_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	TSP_reboot();

	msleep(300);
	printk("set_tsp_test_mode_disable0 \n");

	tsp_testmode = 0;

	return 0;
}

static ssize_t set_module_on_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	tsp_testmode = 1;
	printk("set_tsp_test_mode_enable0 \n");

	mdelay(50);
	ts_power_enable(0);
	mdelay(500);
	ts_power_enable(1);
	return 0;
}

static ssize_t touchtype_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return snprintf(buf, 15, "MMS128\n");
}

static ssize_t set_all_intensity_mode_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int status = 0;

	check_intensity_data(ts);

	set_tsp_module_off_show(dev, attr, buf);
	set_tsp_module_on_show(dev, attr, buf);

	return sprintf(buf, "%u\n", status);
}

ssize_t disp_all_intendata_store(struct device *dev, struct device_attribute *attr,
								   const char *buf, size_t size)
{
	index = atoi(buf);
	printk(KERN_ERR "Intensity data %d", index);
	return size;
}

ssize_t disp_all_intendata_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	printk("disp_all_intendata_show : value %d, index=%d\n", lntensity_data[index], index);
    return sprintf(buf, "%u\n",  lntensity_data[index]);
}

static ssize_t rawdata_pass_fail_melfas(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	u8 setLowLevelData[2] = {0x09, 0x01,};
	u8 read_data_buf[50] = {0,};
	u16 read_data_buf1[50] = {0,};
	int read_data_len,sensing_ch;
	int ret, i,j;

	tsp_testmode = 1;

	disable_irq(ts->client->irq);

	read_data_len = g_exciting_ch * 2;
	sensing_ch	 = g_sensing_ch;

	for(i = 0; i < sensing_ch; i++ )
	{
		////////////////////////
		// Writing Low Level Data(2)
		////////////////////////
		ret = melfas_i2c_write(ts->client, setLowLevelData, 2);

		//////////////////////
		// Checking INT
		//////////////////////
		while(gpio_get_value(GPIO_TOUCH_INT)) // wait for Low
		{
			udelay(50);
		}

		udelay(300);

		//////////////////////
		// Read Data Buf
		//////////////////////
		ret = melfas_i2c_read(ts->client, 0xb2, read_data_len, read_data_buf);

		if(ret < 0)
	            printk("can't read rawdata_pass_fail_tst200 Data %dth\n", i);

		udelay(5);

		for(j = 0 ;j < read_data_len / 2; j++)
		{
			read_data_buf1[j] = (read_data_buf[j*2] <<8) + read_data_buf[j*2+1];
//			printk(" %d", read_data_buf1[j]);

			if((SCR_ABS_UPPER_SPEC[i][j] < read_data_buf1[j])
				|| (SCR_ABS_LOWER_SPEC[i][j] > read_data_buf1[j]))
			{
				printk("\n SCR_ABS_UPPER_SPEC[i][j] = %d", SCR_ABS_UPPER_SPEC[i][j]);
				printk("\n SCR_ABS_LOWER_SPEC[i][j] = %d", SCR_ABS_LOWER_SPEC[i][j]);
				printk("\n i=%d, j=%d,read_data_buf1[j]=%d", i,j,read_data_buf1[j]);
				enable_irq(ts->client->irq);
				udelay(10);
				TSP_reboot();
				return sprintf(buf, "0"); // fail
			}
		}

		printk("\n");

#if 1
		printk("[%d]:", i);
		for(j=0;j<read_data_len;j++)
		{
			printk("[%03d],",read_data_buf[j]);
		}
		printk("\n");

#endif
		msleep(1);

	}

	enable_irq(ts->client->irq);

	tsp_testmode = 0;

	TSP_reboot();

    return sprintf(buf, "1"); // success
 }

//lyon.cho
static ssize_t touch_sensitivity_show(struct device *dev,	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, sizeof(int), "%x\n", 0);  //temp
}

static ssize_t touchkey_firm_store(struct device *dev,	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, sizeof(int), "%x\n", 0);  //temp
}


static DEVICE_ATTR(tsp_firm_version_phone, S_IRUGO | S_IWUSR | S_IWGRP, firmware_phone_show, NULL);
static DEVICE_ATTR(tsp_firm_version_panel, S_IRUGO | S_IWUSR | S_IWGRP, firmware_panel_show, NULL);
static DEVICE_ATTR(tsp_firm_update, S_IRUGO | S_IWUSR | S_IWGRP, NULL, firmware_store);
static DEVICE_ATTR(tsp_threshold, S_IRUGO | S_IWUSR | S_IWGRP, tsp_threshold_show, NULL);
static DEVICE_ATTR(tsp_firm_update_status, S_IRUGO | S_IWUSR | S_IWGRP, tsp_firm_update_status_show, NULL);

//lyon.cho
static DEVICE_ATTR(set_all_reference, S_IRUGO | S_IWUSR | S_IWGRP, set_all_refer_mode_show, NULL);
static DEVICE_ATTR(disp_all_refdata, S_IRUGO | S_IWUSR | S_IWGRP,	disp_all_refdata_show, disp_all_refdata_store);

static DEVICE_ATTR(set_all_inspection, S_IRUGO | S_IWUSR | S_IWGRP, set_all_delta_mode_show, NULL);
static DEVICE_ATTR(disp_all_insdata, S_IRUGO | S_IWUSR | S_IWGRP,	disp_all_deltadata_show, disp_all_refdata_store);

static DEVICE_ATTR(set_all_intensity, S_IRUGO | S_IWUSR | S_IWGRP, set_all_intensity_mode_show, NULL);
static DEVICE_ATTR(disp_all_intdata, S_IRUGO | S_IWUSR | S_IWGRP, disp_all_intendata_show, disp_all_intendata_store);


static DEVICE_ATTR(touchtype, S_IRUGO | S_IWUSR | S_IWGRP,
			touchtype_show, NULL);
static DEVICE_ATTR(set_module_off, S_IRUGO | S_IWUSR | S_IWGRP,
			set_module_off_show, NULL);
static DEVICE_ATTR(set_module_on, S_IRUGO | S_IWUSR | S_IWGRP,
			set_module_on_show, NULL);

//lyon.cho
static DEVICE_ATTR(firmware	, S_IRUGO | S_IWUSR | S_IWGRP,
			firmware_show, firmware_store);
//static DEVICE_ATTR(firmware_ret	, S_IRUGO | S_IWUSR | S_IWOTH | S_IXOTH, firmware_ret_show, firmware_ret_store);
static DEVICE_ATTR(raw_value, 0444, rawdata_pass_fail_melfas, NULL) ;

static DEVICE_ATTR(touchkey_back, S_IRUGO | S_IWUSR | S_IWGRP, touchkey_back_show, NULL);
static DEVICE_ATTR(touchkey_menu, S_IRUGO | S_IWUSR | S_IWGRP, touchkey_menu_show, NULL);
static DEVICE_ATTR(touchkey_raw_data, S_IRUGO | \
     S_IWUSR | S_IWGRP, tkey_rawcounter_show, tkey_rawcounter_store);
static DEVICE_ATTR(touch_sensitivity, S_IRUGO | S_IWUSR | S_IWGRP, touch_sensitivity_show, NULL);
static DEVICE_ATTR(touchkey_firm_update, S_IRUGO | S_IWUSR | S_IWGRP, NULL, touchkey_firm_store);

static int tsp_reboot_count = 0;
static int melfas_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
//	struct melfas_tsi_platform_data *data;

//	struct device *sec_touchscreen;
#if 0//def SEC_TSP
	struct device *qt602240_noise_test;
#ifdef TA_DETECTION
	bool ta_status;
#endif
#endif

	int ret = 0, i;

	uint8_t buf[6] = {0, };


#if DEBUG_PRINT
	printk(KERN_ERR "%s start.\n", __func__);
#endif

	mdelay(50);
	ts_power_enable(0);
	mdelay(500);
	ts_power_enable(1);
	msleep(300);

	g_exciting_ch = 15;
	g_sensing_ch = 10;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		printk(KERN_ERR "%s: need I2C_FUNC_I2C\n", __func__);
        ret = -ENODEV;
        goto err_check_functionality_failed;
	}

    ts = kmalloc(sizeof(struct melfas_ts_data), GFP_KERNEL);
    if (ts == NULL)
    {
        printk(KERN_ERR "%s: failed to create a state of melfas-ts\n", __func__);
        ret = -ENOMEM;
        goto err_alloc_data_failed;
    }
#if 0
	ts_data = ts;
	data = client->dev.platform_data;
	ts->power = data->power;
	ts->gpio = data->gpio;
	ts->version = data->version;
#ifdef TA_DETECTION
	ts->register_cb = data->register_cb;
	ts->read_ta_status = data->read_ta_status;
#endif
    ts->client = client;
    i2c_set_clientdata(client, ts);
	ts->power(true);
#endif
	ts->client = client;
	i2c_set_clientdata(client, ts);
    ret = i2c_master_send(ts->client, &buf, 1);
	mdelay(200);
#if 1 //DEBUG_PRINT
	printk(KERN_ERR "%s: i2c_master_send() [%d], Add[%d]\n", __func__, ret, ts->client->addr);
#endif

#if SET_DOWNLOAD_BY_GPIO

#if 0
	buf[0] = TS_READ_VERSION_ADDR;
	ret = i2c_master_send(ts->client, &buf, 1);
	if (ret < 0)
	{
		printk(KERN_ERR "melfas_ts_work_func : i2c_master_send [%d]\n", ret);
	}

	ret = i2c_master_recv(ts->client, &buf, 3);
#endif
#if (CONFIG_MACH_JENA_HWREV == 0x0)
	ret = melfas_i2c_read(ts->client, MIP_TSP_REVISION, 3, buf);
#else
	ret = melfas_i2c_read(ts->client, MIP_TSP_REVISION, 6, buf);
#endif
	if (ret < 0)
	{
		printk(KERN_ERR "melfas_ts_work_func : i2c_master_recv [%d]\n", ret);
	}
#if (CONFIG_MACH_JENA_HWREV == 0x0)
		printk(KERN_ERR "melfas_ts_SET_DOWNLOAD_BY_GPIO : \
		[%d], [%d], [%d]\n", buf[0], buf[1], buf[2]);
#else
		printk(KERN_ERR "melfas_ts_SET_DOWNLOAD_BY_GPIO : \
		[%d], [%d], [%d], [%d]\n", buf[1], buf[2], buf[3], buf[5]);
#endif
#if (CONFIG_MACH_JENA_HWREV == 0x0)
	if (buf[2] < FW_VERSION)
#else
	if (buf[5] < FW_VERSION)
#endif
	{
		/* enable gpio */

		for (i = 0; i < DOWNLOAD_RETRY_CNT; i++)
		{
			//ret = mcsdl_download_binary_file();
			ret = mcsdl_download_binary_data( 1 );
		printk(KERN_ERR "mcsdl_download_binary_data : [%d]\n", ret);
			if (ret != 0)
				printk(KERN_ERR "SET Download Fail - error code [%d]\n", ret);
			else
				break;
		}

		/* disable gpio */

	}
#endif // SET_DOWNLOAD_BY_GPIO
	ts->input_dev = input_allocate_device();
    if (!ts->input_dev)
    {
		printk(KERN_ERR "%s: Not enough memory\n", __func__);
		ret = -ENOMEM;
		goto err_input_dev_alloc_failed;
	}
	ts->input_dev->name = "sec_touchscreen" ;


	ts->input_dev->evbit[0] = BIT_MASK(EV_ABS) | BIT_MASK(EV_KEY);


	ts->input_dev->keybit[BIT_WORD(KEY_MENU)] |= BIT_MASK(KEY_MENU);
	ts->input_dev->keybit[BIT_WORD(KEY_HOME)] |= BIT_MASK(KEY_HOME);
	ts->input_dev->keybit[BIT_WORD(KEY_BACK)] |= BIT_MASK(KEY_BACK);
	ts->input_dev->keybit[BIT_WORD(KEY_SEARCH)] |= BIT_MASK(KEY_SEARCH);

//	__set_bit(BTN_TOUCH, ts->input_dev->keybit);
//	__set_bit(EV_ABS,  ts->input_dev->evbit);
//	ts->input_dev->evbit[0] =  BIT_MASK(EV_SYN) | BIT_MASK(EV_ABS) | BIT_MASK(EV_KEY);

	input_set_abs_params(ts->input_dev, ABS_MT_POSITION_X, 0, TS_MAX_X_COORD, 0, 0);
	input_set_abs_params(ts->input_dev, ABS_MT_POSITION_Y, 0, TS_MAX_Y_COORD, 0, 0);
	input_set_abs_params(ts->input_dev, ABS_MT_TOUCH_MAJOR, 0, TS_MAX_Z_TOUCH, 0, 0);
	input_set_abs_params(ts->input_dev, ABS_MT_TRACKING_ID, 0, MELFAS_MAX_TOUCH-1, 0, 0);
	input_set_abs_params(ts->input_dev, ABS_MT_WIDTH_MAJOR, 0, TS_MAX_W_TOUCH, 0, 0);
//	__set_bit(EV_SYN, ts->input_dev->evbit);
//	__set_bit(EV_KEY, ts->input_dev->evbit);


    ret = input_register_device(ts->input_dev);
    if (ret)
    {
        printk(KERN_ERR "%s: Failed to register device\n", __func__);
        ret = -ENOMEM;
        goto err_input_register_device_failed;
    }

    if (ts->client->irq)
    {
#if DEBUG_PRINT
        printk(KERN_ERR "%s: trying to request irq: %s-%d\n", __func__, ts->client->name, ts->client->irq);
#endif
	ret = request_threaded_irq(client->irq, NULL, melfas_ts_irq_handler,IRQF_TRIGGER_LOW | IRQF_ONESHOT, ts->client->name, ts);
        if (ret > 0)
        {
            printk(KERN_ERR "%s: Can't allocate irq %d, ret %d\n", __func__, ts->client->irq, ret);
            ret = -EBUSY;
            goto err_request_irq;
        }
    }

	for (i = 0; i < MELFAS_MAX_TOUCH ; i++)  /* _SUPPORT_MULTITOUCH_ */
		g_Mtouch_info[i].strength = -1;

	tsp_enabled = true;

#if 0//def TA_DETECTION
	printk(KERN_ERR "[TSP] tsp_enabled is %d", tsp_enabled);
	data->register_cb(tsp_ta_probe);
	if (data->read_ta_status) {
		data->read_ta_status(&ta_status);
		printk(KERN_ERR "[TSP] ta_status is %d", ta_status);
		tsp_ta_probe(ta_status);
	}
#endif
#if DEBUG_PRINT
	printk(KERN_ERR "%s: succeed to register input device\n", __func__);
#endif

#if 0//def SEC_TSP
	sec_touchscreen = device_create(sec_class, NULL, 0, ts, "sec_touchscreen");
	if (IS_ERR(sec_touchscreen))
		pr_err("[TSP] Failed to create device for the sysfs\n");

	ret = sysfs_create_group(&sec_touchscreen->kobj, &sec_touch_attr_group);
	if (ret)
		pr_err("[TSP] Failed to create sysfs group\n");
#endif

#if 0//def TSP_FACTORY_TEST
	qt602240_noise_test = device_create(sec_class, NULL, 0, ts, "qt602240_noise_test");
	if (IS_ERR(qt602240_noise_test))
		pr_err("[TSP] Failed to create device for the sysfs\n");

	ret = sysfs_create_group(&qt602240_noise_test->kobj, &sec_touch_factory_attr_group);
	if (ret)
		pr_err("[TSP] Failed to create sysfs group\n");
#endif

#ifdef CONFIG_HAS_EARLYSUSPEND
	ts->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	ts->early_suspend.suspend = melfas_ts_early_suspend;
	ts->early_suspend.resume = melfas_ts_late_resume;
	register_early_suspend(&ts->early_suspend);
#endif

#if USE_TEST_RAW_TH_DATA_MODE
	sema_init(&touch_dev->raw_data_lock, 1);

	misc_touch_dev = touch_dev;

	// Fops Device
	ret = misc_register(&touch_misc_device);
	if (ret) {
		zinitix_debug_msg("Fail to register touch misc device.\n");
	}

	if(device_create_file(touch_misc_device.this_device, &dev_attr_get_touch_test_raw_data)<0)		//sys/class/misc/touch_misc_fops/....
		printk("Failed to create device file(%s)!\n", dev_attr_get_touch_test_raw_data.attr.name);

	if(device_create_file(touch_misc_device.this_device, &dev_attr_raw_enable)<0)		//sys/class/misc/touch_misc_fops/....
		printk("Failed to create device file(%s)!\n", dev_attr_raw_enable.attr.name);

	if(device_create_file(touch_misc_device.this_device, &dev_attr_raw_disable)<0)		//sys/class/misc/touch_misc_fops/....
		printk("Failed to create device file(%s)!\n", dev_attr_raw_disable.attr.name);

	if(device_create_file(touch_misc_device.this_device, &dev_attr_raw_show)<0)		//sys/class/misc/touch_misc_fops/....
		printk("Failed to create device file(%s)!\n", dev_attr_raw_show.attr.name);


#endif

	/* sys fs */
//	sec_class = class_create(THIS_MODULE, "sec");
//	if (IS_ERR(sec_class))
//		pr_err("Failed to create class(sec)!\n");

	sec_touchscreen_dev = device_create(sec_class, NULL, 0, NULL, "sec_touchscreen");
	if (IS_ERR(sec_touchscreen_dev))
		pr_err("Failed to create device(sec_touchscreen)!\n");

	if (device_create_file(sec_touchscreen_dev, &dev_attr_tsp_firm_version_phone) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_tsp_firm_version_phone.attr.name);

	if (device_create_file(sec_touchscreen_dev, &dev_attr_tsp_firm_version_panel) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_tsp_firm_version_panel.attr.name);

	if (device_create_file(sec_touchscreen_dev, &dev_attr_tsp_firm_update) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_tsp_firm_update.attr.name);

	if (device_create_file(sec_touchscreen_dev, &dev_attr_tsp_threshold) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_tsp_threshold.attr.name);

	if (device_create_file(sec_touchscreen_dev, &dev_attr_tsp_firm_update_status) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_tsp_firm_update_status.attr.name);

	if (device_create_file(sec_touchscreen_dev,
			&dev_attr_set_all_reference) < 0)
		pr_err("Failed to create device file(%s)!\n",
				dev_attr_set_all_reference.attr.name);
	if (device_create_file(sec_touchscreen_dev,
			&dev_attr_disp_all_refdata) < 0)
		pr_err("Failed to create device file(%s)!\n",
				dev_attr_disp_all_refdata.attr.name);

	if (device_create_file(sec_touchscreen_dev,
			&dev_attr_set_all_inspection) < 0)
		pr_err("Failed to create device file(%s)!\n",
				dev_attr_set_all_inspection.attr.name);
	if (device_create_file(sec_touchscreen_dev,
			&dev_attr_disp_all_insdata) < 0)
		pr_err("Failed to create device file(%s)!\n",
				dev_attr_disp_all_insdata.attr.name);


	if (device_create_file(sec_touchscreen_dev,
			&dev_attr_set_all_intensity) < 0)
		pr_err("Failed to create device file(%s)!\n",
				dev_attr_set_all_intensity.attr.name);

	if (device_create_file(sec_touchscreen_dev,
			&dev_attr_disp_all_intdata) < 0)
		pr_err("Failed to create device file(%s)!\n",
				dev_attr_disp_all_intdata.attr.name);

	if (device_create_file(sec_touchscreen_dev,
			&dev_attr_firmware) < 0)
		pr_err("Failed to create device file(%s)!\n",
				dev_attr_firmware.attr.name);

	if (device_create_file(sec_touchscreen_dev,
			&dev_attr_raw_value) < 0)
		pr_err("Failed to create device file(%s)!\n",
				dev_attr_raw_value.attr.name);


	if (device_create_file(sec_touchscreen_dev, &dev_attr_touchtype) < 0)
		pr_err("Failed to create device file(%s)!\n",
				dev_attr_touchtype.attr.name);

	if (device_create_file(sec_touchscreen_dev,
			&dev_attr_set_module_off) < 0)
		pr_err("Failed to create device file(%s)!\n",
				dev_attr_set_module_off.attr.name);

	if (device_create_file(sec_touchscreen_dev,
			    &dev_attr_set_module_on) < 0)
		pr_err("Failed to create device file(%s)!\n",
				dev_attr_set_module_on.attr.name);


	sec_touchkey_dev = device_create(sec_class,NULL, 0, NULL, "sec_touchkey");
	if (IS_ERR(sec_touchkey_dev))
		pr_err("Failed to create device(sec_touchscreen)!\n");

	if (device_create_file(sec_touchkey_dev, &dev_attr_touchkey_back) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_touchkey_back.attr.name);

	if (device_create_file(sec_touchkey_dev, &dev_attr_touchkey_menu) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_touchkey_menu.attr.name);

	if (device_create_file(sec_touchkey_dev,
			&dev_attr_touchkey_raw_data) < 0)
		pr_err("Failed to create device file(%s)!\n",
			dev_attr_touchkey_raw_data.attr.name);

	if (device_create_file(sec_touchkey_dev, &dev_attr_touch_sensitivity) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_touch_sensitivity.attr.name);

	if (device_create_file(sec_touchkey_dev, &dev_attr_touchkey_firm_update) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_touchkey_firm_update.attr.name);

	return 0;

#if 0//def TSP_BOOST
	TSP_boost(ts, is_boost);
#endif
#if DEBUG_PRINT
	printk(KERN_INFO "%s: Start touchscreen. name: %s, irq: %d\n", __func__, ts->client->name, ts->client->irq);
#endif
	return 0;

err_request_irq:
	printk(KERN_ERR "melfas-ts: err_request_irq failed\n");
	free_irq(client->irq, ts);
err_input_register_device_failed:
	printk(KERN_ERR "melfas-ts: err_input_register_device failed\n");
	input_free_device(ts->input_dev);
err_input_dev_alloc_failed:
	printk(KERN_ERR "melfas-ts: err_input_dev_alloc failed\n");
err_alloc_data_failed:
	printk(KERN_ERR "melfas-ts: err_alloc_data failed_\n");
err_detect_failed:
	ts->power(false);
	printk(KERN_ERR "melfas-ts: err_detect failed\n");
	kfree(ts);
#if 0
	if(tsp_reboot_count < 3){
		tsp_reboot_count++;
		goto init_again;
	}
#endif
err_check_functionality_failed:
	printk(KERN_ERR "melfas-ts: err_check_functionality failed_\n");

	return ret;
}

static int melfas_ts_remove(struct i2c_client *client)
{
	struct melfas_ts_data *ts = i2c_get_clientdata(client);

	unregister_early_suspend(&ts->early_suspend);
	free_irq(client->irq, ts);
	ts->power(false);
	input_unregister_device(ts->input_dev);
	kfree(ts);
	return 0;
}

static int melfas_ts_suspend(struct i2c_client *client, pm_message_t mesg)
{
	int ret;
	struct melfas_ts_data *ts = i2c_get_clientdata(client);
	disable_irq(client->irq);
	tsp_enabled = false;
	release_all_fingers(ts);
	touch_is_pressed = 0;
//	ts->gpio();
//	ts->power(false);

	ts_power_enable(0);

	return 0;
}

static int melfas_ts_resume(struct i2c_client *client)
{
	struct melfas_ts_data *ts = i2c_get_clientdata(client);
#if 0//def TA_DETECTION
	bool ta_status=0;
#endif
//	ts->power(true);
#if 0//def TSP_BOOST
	TSP_boost(ts, is_boost);
#endif
	enable_irq(client->irq); // scl wave
	tsp_enabled = true;
#if 0//def TA_DETECTION
	if (ts->read_ta_status) {
		ts->read_ta_status(&ta_status);
		printk(KERN_ERR "[TSP] ta_status is %d", ta_status);
		tsp_ta_probe(ta_status);
	}
#endif
	ts_power_enable(1);

	return 0;
}

#ifdef CONFIG_HAS_EARLYSUSPEND
static void melfas_ts_early_suspend(struct early_suspend *h)
{
	struct melfas_ts_data *ts;
	ts = container_of(h, struct melfas_ts_data, early_suspend);
	melfas_ts_suspend(ts->client, PMSG_SUSPEND);
}

static void melfas_ts_late_resume(struct early_suspend *h)
{
	struct melfas_ts_data *ts;
	ts = container_of(h, struct melfas_ts_data, early_suspend);
	melfas_ts_resume(ts->client);
}
#endif

static const struct i2c_device_id melfas_ts_id[] =
{
    { "sec_touch", 0 },
    { }
};

static struct i2c_driver melfas_ts_driver =
{
    .driver = {
    .name = "sec_touch",
    },
    .id_table = melfas_ts_id,
    .probe = melfas_ts_probe,
    .remove = __devexit_p(melfas_ts_remove),
#ifndef CONFIG_HAS_EARLYSUSPEND
	.suspend		= melfas_ts_suspend,
	.resume		= melfas_ts_resume,
#endif
};

#ifdef CONFIG_BATTERY_SEC
extern unsigned int is_lpcharging_state(void);
#endif

static int __devinit melfas_ts_init(void)
{
#ifdef CONFIG_BATTERY_SEC
	if (is_lpcharging_state()) {
		pr_info("%s : LPM Charging Mode! return 0\n", __func__);
		return 0;
	}
#endif

	if (board_hw_revision == 0)
		FW_VERSION = 0x05;
	else if (board_hw_revision == 1)
		FW_VERSION = 0x08;
	else
		FW_VERSION = 0x27;
	return i2c_add_driver(&melfas_ts_driver);
}

static void __exit melfas_ts_exit(void)
{
	i2c_del_driver(&melfas_ts_driver);
}

MODULE_DESCRIPTION("Driver for Melfas MTSI Touchscreen Controller");
MODULE_AUTHOR("MinSang, Kim <kimms@melfas.com>");
MODULE_VERSION("0.1");
MODULE_LICENSE("GPL");

module_init(melfas_ts_init);
module_exit(melfas_ts_exit);
