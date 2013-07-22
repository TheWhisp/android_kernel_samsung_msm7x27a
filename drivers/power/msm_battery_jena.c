/* Copyright (c) 2009-2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * this needs to be before <linux/kernel.h> is loaded,
 * and <linux/sched.h> loads <linux/kernel.h>
 */
#include <linux/device.h>

#define MAX17043_FUEL_GAUGE

#include <linux/slab.h>
#include <linux/earlysuspend.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/workqueue.h>
#include <linux/fsaxxxx_usbsw.h>
#include <linux/atomic.h>
#include <mach/msm_rpcrouter.h>
#include <mach/msm_battery_jena.h>
#include <mach/bq27425_fuelguage.h>
#include "../../arch/arm/mach-msm/proc_comm.h"
#include <mach/pmic.h>
#include <linux/android_alarm.h>
/*from S+*/
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/wakelock.h>

#include <linux/timer.h>
#include <linux/delay.h>

static struct wake_lock vbus_wake_lock;
static struct wake_lock lpm_wake_lock;
static struct wake_lock fuel_alert_wake_lock;
static int fuel_alert_det;

#ifdef DEBUG
#undef pr_debug
#define pr_debug pr_info
#endif

#define BATTERY_RPC_PROG		0x30000089
#define BATTERY_RPC_VER_1_1		0x00010001
#define BATTERY_RPC_VER_2_1		0x00020001
#define BATTERY_RPC_VER_4_1		0x00040001
#define BATTERY_RPC_VER_5_1		0x00050001

#define BATTERY_RPC_CB_PROG	(BATTERY_RPC_PROG | 0x01000000)

#define CHG_RPC_PROG		0x3000001a
#define CHG_RPC_VER_1_1		0x00010001
#define CHG_RPC_VER_1_3		0x00010003
#define CHG_RPC_VER_2_2		0x00020002
#define CHG_RPC_VER_3_1		0x00030001
#define CHG_RPC_VER_4_1		0x00040001

#define BATTERY_REGISTER_PROC			2
#define BATTERY_MODIFY_CLIENT_PROC		4
#define BATTERY_DEREGISTER_CLIENT_PROC	5
#define BATTERY_READ_MV_PROC			12
#define BATTERY_ENABLE_DISABLE_FILTER_PROC	14

#define VBATT_FILTER				2

#define BATTERY_CB_TYPE_PROC			1
#define BATTERY_CB_ID_ALL_ACTIV		1
#define BATTERY_CB_ID_LOW_VOL			2

#define BATTERY_LOW				3200
#define BATTERY_HIGH				4300

#define ONCRPC_CHG_GET_GENERAL_STATUS_PROC	12
#define ONCRPC_CHARGER_API_VERSIONS_PROC	0xffffffff

#define BATT_RPC_TIMEOUT	5000	/* 5 sec */

#define INVALID_BATT_HANDLE		-1

#define RPC_TYPE_REQ		0
#define RPC_TYPE_REPLY		1
#define RPC_REQ_REPLY_COMMON_HEADER_SIZE	(3 * sizeof(uint32_t))

#define DEBUG 1


typedef enum {
	STOP_CHARGING,
	START_CHARGING
} chg_enable_type;


enum chg_type {
	USB_CHG_TYPE__SDP,
	USB_CHG_TYPE__CARKIT,
	USB_CHG_TYPE__WALLCHARGER,
	USB_CHG_TYPE__INVALID
};

/*Maybe iT is not used in here*/
#ifdef CONFIG_MACH_GEIM
const int temp_table[][2] = {
	/* ADC, Temperature (C) */
	{813, -10},
	{808, -9},
	{803, -8},
	{798, -7},
	{793, -6},
	{788, -5},
	{783, -4},
	{778, -3},
	{773, -2},
	{768, -1},
	{762, 0},
	{756, 1},
	{750, 2},
	{744, 3},
	{738, 4},
	{731, 5},
	{724, 6},
	{718, 7},
	{711, 8},
	{704, 9},
	{696, 10},
	{688, 11},
	{681, 12},
	{674, 13},
	{667, 14},
	{660, 15},
	{652, 16},
	{644, 17},
	{636, 18},
	{628, 19},
	{620, 20},
	{612, 21},
	{604, 22},
	{595, 23},
	{587, 24},
	{578, 25},
	{569, 26},
	{559, 27},
	{550, 28},
	{541, 29},
	{532, 30},
	{522, 31},
	{513, 32},
	{503, 33},
	{494, 34},
	{484, 35},
	{475, 36},
	{466, 37},
	{457, 38},
	{448, 39},
	{439, 40},
	{430, 41},
	{421, 42},
	{412, 43},
	{403, 44},
	{395, 45},
	{387, 46},
	{379, 47},
	{371, 48},
	{363, 49},
	{355, 50},
	{348, 51},
	{341, 52},
	{334, 53},
	{327, 54},
	{320, 55},
	{313, 56},
	{306, 57},
	{299, 58},
	{292, 59},
	{285, 60},
	{278, 61},
	{271, 62},
	{264, 63},
	{257, 64},
	{250, 65},
	{243, 66},
	{236, 67},
};
#else
const int temp_table[][2] = {
	/* ADC, Temperature (C) */
	{860, -20},
	{847, -15},
	{831, -10},
	{808, -5},
	{782, 0},
	{740, 5},
	{711, 10},
	{682, 15},
	{624, 20},
	{587, 25},
	{538, 30},
	{504, 35},
	{449, 40},
	{413, 45},
	{370, 50},
	{330, 55},
	{289, 60}
};

#endif

#define AVERAGE_COUNT				10

#ifdef __FULL_CHARGE_TEST__
#define TOTAL_CHARGING_TIME			(1 * 60*HZ)
#define TOTAL_RECHARGING_TIME			(1 * 60*HZ)
#else
#define TOTAL_CHARGING_TIME			(5 * 60*60*HZ)
#define TOTAL_RECHARGING_TIME			(90 * 60*HZ)
#endif
#define TOTAL_WATING_TIME			(20 * HZ)
#define WAKE_LOCK_TIME_OUT			(5 * HZ)
#define ALARM_POLLING_TIME_SHORT		(1 * 60)
#define ALARM_POLLING_TIME_LONG		(10 * 60)

#define BAT_USE_TIMER_EXPIRE			(10 * 60*HZ)


#ifdef CONFIG_MACH_GEIM
#define BATT_CHECK_INTERVAL			(10 * HZ)
#else
#define BATT_CHECK_INTERVAL			(30 * HZ)
#endif
#define TEMP_TABLE_OFFSET			30

#ifdef CONFIG_MACH_GEIM
#define BATT_TEMP_EVENT_BLOCK			60
#define BATT_TEMP_HIGH_BLOCK			47
#define BATT_TEMP_HIGH_RECOVER			43
#define BATT_TEMP_LOW_BLOCK			(-5)
#define BATT_TEMP_LOW_RECOVER			0

#define BATT_TEMP_HIGH_BLOCK_LPM		47
#define BATT_TEMP_HIGH_RECOVER_LPM		43
#define BATT_TEMP_LOW_BLOCK_LPM		(-5)
#define BATT_TEMP_LOW_RECOVER_LPM		0

#else
#define BATT_TEMP_HIGH_BLOCK			60
#define BATT_TEMP_HIGH_RECOVER			40
#define BATT_TEMP_LOW_BLOCK			(-5)
#define BATT_TEMP_LOW_RECOVER			0

#define BATT_TEMP_HIGH_BLOCK_LPM		60
#define BATT_TEMP_HIGH_RECOVER_LPM		40
#define BATT_TEMP_LOW_BLOCK_LPM		(-5)
#define BATT_TEMP_LOW_RECOVER_LPM		0

#endif

#ifdef CONFIG_MACH_GEIM
#define USE_CALL			(0x1 << 0)
#define USE_VIDEO			(0x1 << 1)
#define USE_MUSIC			(0x1 << 2)
#define USE_BROWSER		(0x1 << 3)
#define USE_HOTSPOT			(0x1 << 4)
#define USE_CAMERA			(0x1 << 5)
#define USE_DATA_CALL		(0x1 << 6)
#define USE_GPS			(0x1 << 7)
#endif /*CONFIG_MACH_GEIM*/


#ifdef CONFIG_MACH_GEIM
#define BATT_FULL_CHARGING_CURRENT	90
#define BATT_FULL_CHARGING_VOLTAGE      4180
#else
#define BATT_FULL_CHARGING_CURRENT	90
#define BATT_FULL_CHARGING_VOLTAGE      4180
#endif /*CONFIG_FUEL_GPIO*/

#define BATT_RECHARGING_VOLTAGE_1	4130
#define BATT_RECHARGING_VOLTAGE_2	4000

enum {
	BATTERY_REGISTRATION_SUCCESSFUL = 0,
	BATTERY_DEREGISTRATION_SUCCESSFUL = BATTERY_REGISTRATION_SUCCESSFUL,
	BATTERY_MODIFICATION_SUCCESSFUL = BATTERY_REGISTRATION_SUCCESSFUL,
	BATTERY_INTERROGATION_SUCCESSFUL = BATTERY_REGISTRATION_SUCCESSFUL,
	BATTERY_CLIENT_TABLE_FULL = 1,
	BATTERY_REG_PARAMS_WRONG = 2,
	BATTERY_DEREGISTRATION_FAILED = 4,
	BATTERY_MODIFICATION_FAILED = 8,
	BATTERY_INTERROGATION_FAILED = 16,
	/* Client's filter could not be set because perhaps it does not exist */
	BATTERY_SET_FILTER_FAILED         = 32,
	/* Client's could not be found for enabling or disabling the individual
	 * client */
	BATTERY_ENABLE_DISABLE_INDIVIDUAL_CLIENT_FAILED  = 64,
	BATTERY_LAST_ERROR = 128,
};

enum {
	BATTERY_VOLTAGE_UP = 0,
	BATTERY_VOLTAGE_DOWN,
	BATTERY_VOLTAGE_ABOVE_THIS_LEVEL,
	BATTERY_VOLTAGE_BELOW_THIS_LEVEL,
	BATTERY_VOLTAGE_LEVEL,
	BATTERY_ALL_ACTIVITY,
	VBATT_CHG_EVENTS,
	BATTERY_VOLTAGE_UNKNOWN,
};

/* for Rev 03*/
#ifndef CONFIG_BQ27425_FUEL_GAUGE
#define BATT_LOW_VOLT		3400
#define BATT_LEVEL1_VOLT	3620
#define BATT_LEVEL2_VOLT	3700
#define BATT_LEVEL3_VOLT	3740
#define BATT_LEVEL4_VOLT	3790
#define BATT_LEVEL5_VOLT	3860
#define BATT_LEVEL6_VOLT	3960
#define BATT_FULL_VOLT		4200
#define BATT_RECHAR_VOLT	4140


#define BATT_LOW_ADC		2315
#define BATT_LEVEL1_ADC	2680
#define BATT_LEVEL2_ADC	2825
#define BATT_LEVEL3_ADC	2895
#define BATT_LEVEL4_ADC	2985
#define BATT_LEVEL5_ADC	3105
#define BATT_LEVEL6_ADC	3275
#define BATT_LEVEL7_ADC	3394
#define BATT_LEVEL8_ADC	3513
#define BATT_LEVEL9_ADC	3547
#define BATT_FULL_ADC		3685

static int g_chg_en;
static int prev_scaled_level;
static int chg_polling_cnt;
static int bSetTimer;

#endif/*CONFIG_BQ27425_FUEL_GAUGE*/

/*
 * This enum contains defintions of the charger hardware status
 */
enum chg_charger_status_type {
	/* The charger is good      */
	CHARGER_STATUS_GOOD,
	/* The charger is bad       */
	CHARGER_STATUS_BAD,
	/* The charger is weak      */
	CHARGER_STATUS_WEAK,
	/* Invalid charger status.  */
	CHARGER_STATUS_INVALID
};

/*
 *This enum contains defintions of the charger hardware type
 */
enum chg_charger_hardware_type {
	/* The charger is removed                 */
	CHARGER_TYPE_NONE,
	/* The charger is a regular wall charger   */
	CHARGER_TYPE_WALL,
	/* The charger is a PC USB                 */
	CHARGER_TYPE_USB_PC,
	/* The charger is a wall USB charger       */
	CHARGER_TYPE_USB_WALL,
	/* The charger is a USB carkit             */
	CHARGER_TYPE_USB_CARKIT,
	/* Invalid charger hardware status.        */
	CHARGER_TYPE_INVALID
};

/*
 *  This enum contains defintions of the battery status
 */
enum chg_battery_status_type {
	/* The battery is good        */
	BATTERY_STATUS_GOOD,
	/* The battery is cold/hot    */
	BATTERY_STATUS_BAD_TEMP,
	/* The battery is bad         */
	BATTERY_STATUS_BAD,
	/* The battery is removed     */
	BATTERY_STATUS_REMOVED,		/* on v2.2 only */
	BATTERY_STATUS_INVALID_v1 = BATTERY_STATUS_REMOVED,
	/* Invalid battery status.    */
	BATTERY_STATUS_INVALID
};

/*
 *This enum contains defintions of the battery voltage level
 */
enum chg_battery_level_type {
	/* The battery voltage is dead/very low (less than 3.2V) */
	BATTERY_LEVEL_DEAD,
	/* The battery voltage is weak/low (between 3.2V and 3.4V) */
	BATTERY_LEVEL_WEAK,
	/* The battery voltage is good/normal(between 3.4V and 4.2V) */
	BATTERY_LEVEL_GOOD,
	/* The battery voltage is up to full (close to 4.2V) */
	BATTERY_LEVEL_FULL,
	/* Invalid battery voltage level. */
	BATTERY_LEVEL_INVALID
};




/*From S+ bat driver*/
int batt_jig_on_status;
EXPORT_SYMBOL(batt_jig_on_status);

/* sys fs */
struct class *jig_class;
EXPORT_SYMBOL(jig_class);
struct device *jig_dev;
EXPORT_SYMBOL(jig_dev);

static ssize_t jig_show(struct device *dev,
	struct device_attribute *attr, char *buf);
static DEVICE_ATTR(jig , 664,
	jig_show, NULL);

static ssize_t jig_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, sizeof(buf), "%d\n", batt_jig_on_status);
}
/* sys fs  End S+ bat driver*/


struct rpc_reply_batt_chg_v1 {
	struct rpc_reply_hdr hdr;
	u32	more_data;
	u32	charger_status;			/*nc*/
	u32	charger_type;			/*nc*/
	u32	battery_status;			/*nc*/
	u32	battery_level;			/*nc*/
	u32 battery_voltage;		/*nc*/
	u32	battery_temp;			/*nc*/
	u32 battery_temp_adc;
	u32	chg_current;
	u32 batt_id;
	u32 battery_temp_degree;
};

struct rpc_reply_batt_chg_v2 {
	struct rpc_reply_batt_chg_v1	v1;

	u32	is_charger_valid;
	u32	is_charging;
	u32	is_battery_valid;
	u32	ui_event;
};

union rpc_reply_batt_chg {
	struct rpc_reply_batt_chg_v1	v1;
	struct rpc_reply_batt_chg_v2	v2;
};

static union rpc_reply_batt_chg rep_batt_chg;

struct msm_battery_info {
	u32 voltage_max_design;
	u32 voltage_min_design;
	u32 chg_api_version;
	u32 batt_technology;
	u32 batt_api_version;

	u32 avail_chg_sources;
	u32 current_chg_source;		/*nc*/
	u32 chg_temp_event_check;
	u32 talk_gsm;
	u32 data_call;

	u32 batt_status;
	u32 batt_health;
	u32 charger_valid;			/*nc*/
	u32 batt_valid;
	u32 batt_capacity;		/* in percentage */

	u32 charger_status;		/*nc*/
	u32 charger_type;
	u32 battery_status;		/*nc*/
	u32 battery_level;		/*nc*/
	u32 battery_voltage;

	u32 fg_soc;
	u32 battery_vol;		/*volt*/
	u32 batt_temp_check;
	u32 batt_full_check;
	u32 charging_source;

	int battery_temp;		/* in celsius */
	int batt_temp_degc;
	u32 chg_current_adc;
	u32 batt_recharging;
	/*fuelgauge*/
	u32	fuel_alert;
	u32 fuel_alert_adc;
	u32 fuel_temp;
	u32 batt_fuel_current;

	u32 batt_type;

	u32 batt_vf_adc;
	u32 vf;
	int battery_temp_adc;
	u32 battery_temp_proc;
	s32 batt_temp_aver;
	u32 batt_vol_adc;
	u32 batt_vol_aver;

	u32 chargingblock_clear;
	u32 batt_voltage_now;		/*low bat*/

	u32 suspend_status;

	s32 batt_handle;

	struct msm_charger_data *pdata;

	enum cable_type_t	cable_status;
	enum acc_type_t		acc_status;
	enum ovp_type_t		ovp_status;

	struct msm_battery_callback callback;

	struct power_supply *msm_psy_ac;
	struct power_supply *msm_psy_usb;
	struct power_supply *msm_psy_batt;
	struct power_supply *current_ps;	/*nc*/

	struct alarm		alarm;
	struct msm_rpc_client *batt_client;
	struct msm_rpc_endpoint *chg_ep;

	struct workqueue_struct *msm_batt_wq;
	struct timer_list timer;
	struct timer_list bat_use_timer;

	wait_queue_head_t wait_q;

	u32 vbatt_modify_reply_avail;	/*nc*/
	struct early_suspend early_suspend;

	u32 batt_slate_mode;
	u32 off_backlight;

	u32 batt_use;
	u32 batt_use_wait;
	u32 resume_flag;
	u32 boot_flag;

	ktime_t                 last_poll;
};

static struct msm_battery_info msm_batt_info = {
	.batt_handle = INVALID_BATT_HANDLE,
	.charger_type = CHARGER_TYPE_NONE,
	.battery_voltage = BATTERY_HIGH,
	.batt_status = POWER_SUPPLY_STATUS_DISCHARGING,
	.batt_health = POWER_SUPPLY_HEALTH_GOOD,
	.batt_valid  = 1,
	.battery_temp = 230,
	.batt_slate_mode = 0,
	.off_backlight = 0,
};

static enum power_supply_property msm_power_props[] = {
	POWER_SUPPLY_PROP_ONLINE,
};

static enum power_supply_property msm_batt_power_props[] = {
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_HEALTH,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_TECHNOLOGY,
	POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN,
	POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_CAPACITY,
	POWER_SUPPLY_PROP_TEMP,
	POWER_SUPPLY_PROP_TYPE,
};

static char *msm_power_supplied_to[] = {
	"battery",
};

const char *oem_str[] = {
	"PCOM_OEM_CHARGING_INFO",
	"PCOM_OEM_POWER_KEY_GET",
	"PCOM_OEM_SYS_CMD",
	"PCOM_OEM_SAMSUNG_LAST",
};

enum {
	SMEM_PROC_COMM_CHARGING_CURRENT = 0,
	SMEM_PROC_COMM_CHARGING_VF,
	SMEM_PROC_COMM_CHARGING_TEMP_ADC,
	SMEM_PROC_COMM_CHARGING_TEMP_DEGREE,
	SMEM_PROC_COMM_CHARGING_VOLTAGE,
	SMEM_PROC_COMM_CHARGING_CHARGE_STATE,
	SMEM_PROC_COMM_CHARGING_FUEL_ALERT_CHECK
};

const char *oem_chg_str[] = {
	"SMEM_PROC_COMM_CHARGING_CURRENT",
	"SMEM_PROC_COMM_CHARGING_VF",
	"SMEM_PROC_COMM_CHARGING_TEMP_ADC",
	"SMEM_PROC_COMM_CHARGING_TEMP_DEGREE",
	"SMEM_PROC_COMM_CHARGING_VOLTAGE",
	"SMEM_PROC_COMM_CHARGING_CHARGE_STATE",
	"SMEM_PROC_COMM_CHARGING_FUEL_ALERT_CHECK"
};

static int pm_msm_proc_comm(u32 cmd, u32 *data1, u32 *data2)
{
	pr_info("%s,\td1=%s,\td2=%d\n",
			oem_str[cmd - PCOM_OEM_CHARGING_INFO],
			oem_chg_str[*data1], *data2);
	return msm_proc_comm(cmd, data1, data2);
}

static void bc_read_status(struct msm_battery_info *mi)
{
	mi->batt_type = POWER_SUPPLY_TECHNOLOGY_LION;
}

static int msm_batt_power_get_property(struct power_supply *psy,
				       enum power_supply_property psp,
				       union power_supply_propval *val);

static int msm_power_get_property(struct power_supply *psy,
				  enum power_supply_property psp,
				  union power_supply_propval *val);

static struct power_supply msm_psy_ac = {
	.name = "ac",
	.type = POWER_SUPPLY_TYPE_MAINS,
	.supplied_to = msm_power_supplied_to,
	.num_supplicants = ARRAY_SIZE(msm_power_supplied_to),
	.properties = msm_power_props,
	.num_properties = ARRAY_SIZE(msm_power_props),
	.get_property = msm_power_get_property,
};

static struct power_supply msm_psy_usb = {
	.name = "usb",
	.type = POWER_SUPPLY_TYPE_USB,
	.supplied_to = msm_power_supplied_to,
	.num_supplicants = ARRAY_SIZE(msm_power_supplied_to),
	.properties = msm_power_props,
	.num_properties = ARRAY_SIZE(msm_power_props),
	.get_property = msm_power_get_property,
};

static struct power_supply msm_psy_batt = {
	.name = "battery",
	.type = POWER_SUPPLY_TYPE_BATTERY,
	.properties = msm_batt_power_props,
	.num_properties = ARRAY_SIZE(msm_batt_power_props),
	.get_property = msm_batt_power_get_property,
};

static unsigned int charging_start_time;

static int msm_batt_driver_init;
static int msm_batt_unhandled_interrupt;

extern void hsusb_chg_connected(enum chg_type chgtype);
extern void hsusb_chg_vbus_draw(unsigned mA);


#ifdef CONFIG_BQ27425_FUEL_GAUGE
static u32 get_voltage_from_fuelgauge(void);
static u32 get_level_from_fuelgauge(void);
static int get_temp_from_fuelgauge(void);
static int get_current_from_fuelgauge(void);
static u32 set_reset_fuelgague(void);
static u32 get_vf_open_from_fuelgauge(void);
#endif /* CONFIG_BQ27425_FUEL_GAUGE */


int batt_restart(void);


static ssize_t msm_batt_show_property(struct device *dev,
		struct device_attribute *attr, char *buf);
static ssize_t msm_batt_store_property(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count);

static int msm_batt_average_chg_current(int chg_current_adc);

static void msm_batt_check_event(struct work_struct *work);
static void msm_batt_set_cable_bt(struct work_struct *work);
static void msm_batt_ovp_bt(struct work_struct *work);
static void msm_batt_cable_status_update(void);

static void msm_batt_update_psy_status(void);

/* charging absolute time control */
static void msm_batt_set_charging_start_time(chg_enable_type enable);
static int msm_batt_is_over_abs_time(void);

static void msm_batt_update_psy_status(void);
static void msm_batt_chg_en(chg_enable_type enable);
static DECLARE_WORK(msm_batt_work, msm_batt_check_event);
static DECLARE_WORK(msm_batt_wq_setcable, msm_batt_set_cable_bt);
static DECLARE_WORK(msm_batt_wq_ovp, msm_batt_set_cable_bt);
struct workqueue_struct *msm_batt_cable_wq;

static void batt_timeover(unsigned long arg)
{
	pr_info("[BATT] %s: timer !!\n", __func__);
	queue_work(msm_batt_info.msm_batt_wq, &msm_batt_work);
	mod_timer(&msm_batt_info.timer, (jiffies + BATT_CHECK_INTERVAL));
}

void msm_battery_fuel_alert(void)
{
	int data1 = SMEM_PROC_COMM_CHARGING_FUEL_ALERT_CHECK;
	int data2 = 0;
	int res = pm_msm_proc_comm(PCOM_OEM_CHARGING_INFO, &data1, &data2);

	pr_info("[BATT] fuel_alert cleared:%d\n", data2);
	/* block bellow until fuel guage modified */
/*
	queue_work(msm_batt_info.msm_batt_wq, &msm_batt_work);
	mod_timer(&msm_batt_info.timer, (jiffies + BATT_CHECK_INTERVAL));
*/
}

static void msm_batt_check_event(struct work_struct *work)
{
	pr_info("[BATT] %s: work !!\n", __func__);
	msm_batt_update_psy_status();
}

#define MSM_BATTERY_ATTR(_name)		\
{						\
	.attr = { .name = #_name, .mode = 0664 },	\
	.show = msm_batt_show_property,		\
	.store = msm_batt_store_property,		\
}

static struct device_attribute msm_battery_attrs[] = {
#ifdef CONFIG_BQ27425_FUEL_GAUGE
	MSM_BATTERY_ATTR(fg_read_soc),
	MSM_BATTERY_ATTR(fg_reset_soc),
#endif	/* CONFIG_BQ27425_FUEL_GAUGE */
	MSM_BATTERY_ATTR(batt_vol),
	MSM_BATTERY_ATTR(batt_vol_adc),
	MSM_BATTERY_ATTR(batt_temp_check),
	MSM_BATTERY_ATTR(batt_full_check),
	MSM_BATTERY_ATTR(charging_source),
	MSM_BATTERY_ATTR(batt_chg_current),
	MSM_BATTERY_ATTR(batt_temp_adc),
	MSM_BATTERY_ATTR(batt_temp),
	MSM_BATTERY_ATTR(batt_temp_aver),
	MSM_BATTERY_ATTR(batt_fuel_current),
	MSM_BATTERY_ATTR(batt_vf_adc),
	MSM_BATTERY_ATTR(batt_vf),
	MSM_BATTERY_ATTR(batt_type),
#ifdef __BATT_TEST_DEVICE__
	MSM_BATTERY_ATTR(batt_temp_test_adc),
#endif
	MSM_BATTERY_ATTR(chargingblock_clear),
	MSM_BATTERY_ATTR(batt_slate_mode),
	MSM_BATTERY_ATTR(batt_temp_degc),
	MSM_BATTERY_ATTR(chg_temp_event_check),
	MSM_BATTERY_ATTR(vt_call),
	MSM_BATTERY_ATTR(talk_wcdma),
	MSM_BATTERY_ATTR(talk_gsm),
	/*MSM_BATTERY_ATTR(data_call),*/
	MSM_BATTERY_ATTR(off_backlight),
#ifdef CONFIG_MACH_GEIM
	MSM_BATTERY_ATTR(call),
	MSM_BATTERY_ATTR(video),
	MSM_BATTERY_ATTR(music),
	MSM_BATTERY_ATTR(browser),
	MSM_BATTERY_ATTR(hotspot),
	MSM_BATTERY_ATTR(camera),
	MSM_BATTERY_ATTR(data_call),
	MSM_BATTERY_ATTR(gps),
	MSM_BATTERY_ATTR(batt_use),
#endif /*CONFIG_MACH_GEIM*/

};

enum {
#ifdef CONFIG_BQ27425_FUEL_GAUGE
	FG_READ_SOC = 0,
	FG_RESET_SOC,
#endif	/* CONFIG_BQ27425_FUEL_GAUGE */
	BATT_VOL,
	BATT_VOL_ADC,
	BATT_TEMP_CHECK,
	BATT_FULL_CHECK,
	CHARGING_SOURCE,
	BATT_CHG_CURRENT,
	BATT_TEMP_ADC,
	BATT_TEMP,
	BATTERY_TEMP_AVER,
	BATT_FUEL_CURRENT,
	BATT_VF_ADC,
	BATT_VF,
	BATT_TYPE,
#ifdef __BATT_TEST_DEVICE__
	BATT_TEMP_TEST_ADC,
#endif
	CHARGINGBLOCK_CLEAR,
	BATT_SLATE_MODE,
	BATT_TEMP_DEGC,
	CHG_TEMP_EVENT_CHECK,
	VT_CALL,
	TALK_WCDMA,
	TALK_GSM,
	/*DATA_CALL,*/
	OFF_BACKLIGHT,
#ifdef CONFIG_MACH_GEIM
	BATT_USE_CALL,
	BATT_USE_VIDEO,
	BATT_USE_MUSIC,
	BATT_USE_BROWSER,
	BATT_USE_HOTSPOT,
	BATT_USE_CAMERA,
	BATT_USE_DATA_CALL,
	BATT_USE_GPS,
	BATT_USE,
#endif /*CONFIG_MACH_GEIM*/
	BATT_TEST_MODE,
};

static int msm_batt_create_attrs(struct device *dev)
{
	int i, rc;

	for (i = 0; i < ARRAY_SIZE(msm_battery_attrs); i++)	{
		rc = device_create_file(dev, &msm_battery_attrs[i]);
		if (rc)
			goto failed;
	}
	goto succeed;

failed:
	while (i--)
		device_remove_file(dev, &msm_battery_attrs[i]);

succeed:
	return rc;
}

static void msm_batt_remove_attrs(struct device *dev)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(msm_battery_attrs); i++)
		device_remove_file(dev, &msm_battery_attrs[i]);

}

#ifdef CONFIG_MACH_GEIM

void msm_bat_use_timer_func(unsigned long param)
{
	struct msm_battery_info *chg = (struct msm_battery_info *)param;

	msm_batt_info.batt_use &= (~msm_batt_info.batt_use_wait);
	pr_info("[BATT]batt_use timer expired (0x%x)\n",
			msm_batt_info.batt_use);
#if 0
	if (msm_batt_info.batt_status == POWER_SUPPLY_STATUS_CHARGING &&
		(msm_batt_info.cable_status == CABLE_TYPE_TA)  &&
		(msm_batt_info.batt_use == 0)) {
		pr_info("%s[BATT] restart charging for TA\n", __func__);
		msm_batt_chg_en(true);
	}
#endif

}

void msm_bat_use_module(int module, int enable)
{
	if (!enable && (msm_batt_info.batt_use
			== msm_batt_info.batt_use_wait)) {
		pr_info("/BATT_USE/ ignore duplicated of same event\n");
		return;
	}

	del_timer_sync(&msm_batt_info.bat_use_timer);
	msm_batt_info.batt_use &= (~msm_batt_info.batt_use_wait);

	if (enable) {
#if 0
		if (msm_batt_info.batt_status ==
			POWER_SUPPLY_STATUS_CHARGING &&
			(msm_batt_info.cable_status == CABLE_TYPE_TA)  &&
			(msm_batt_info.batt_use == 0)) {
			pr_info("%s[BATT] restart charging for TA\n", __func__);
			msm_batt_chg_en(true);
			}
#endif
		msm_batt_info.batt_use_wait = 0;
		msm_batt_info.batt_use |= module;

		/* debug msg */
		if (module == USE_CALL)
			pr_info("/BATT_USE/ use module (call) 0x%x\n",
				msm_batt_info.batt_use);
		else if (module == USE_VIDEO)
			pr_info("/BATT_USE/ use module (video) 0x%x\n",
				msm_batt_info.batt_use);
		else if (module == USE_MUSIC)
			pr_info("/BATT_USE/ use module (music) 0x%x\n",
				msm_batt_info.batt_use);
		else if (module == USE_BROWSER)
			pr_info("/BATT_USE/ use module (browser) 0x%x\n",
				msm_batt_info.batt_use);
		else if (module == USE_HOTSPOT)
			pr_info("/BATT_USE/ use module (hotspot) 0x%x\n",
				msm_batt_info.batt_use);
		else if (module == USE_CAMERA)
			pr_info("/BATT_USE/ use module (camera) 0x%x\n",
				msm_batt_info.batt_use);
		else if (module == USE_DATA_CALL)
			pr_info("/BATT_USE/ use module (data call) 0x%x\n",
				msm_batt_info.batt_use);
		else if (module == USE_GPS)
			pr_info("/BATT_USE/ use module (gps) 0x%x\n",
				msm_batt_info.batt_use);
	} else {
		if (msm_batt_info.batt_use == 0) {
			pr_info("/BATT_USE/ nothing to clear\n");
			return;	/* nothing to clear */
		}
		msm_batt_info.batt_use_wait = module;
		mod_timer(&msm_batt_info.bat_use_timer,
				jiffies + BAT_USE_TIMER_EXPIRE);
		pr_info("/BATT_USE/ start timer (curr 0x%x, wait 0x%x)\n",
			msm_batt_info.batt_use, msm_batt_info.batt_use_wait);

	}
}
#endif /*CONFIG_MACH_GEIM*/


static ssize_t msm_batt_show_property(struct device *dev,
				       struct device_attribute *attr,
				       char *buf)
{
	int i = 0;
	const ptrdiff_t offset = attr - msm_battery_attrs;

	switch (offset) {
#ifdef CONFIG_BQ27425_FUEL_GAUGE
	case FG_READ_SOC:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.batt_capacity);
		break;
	case FG_RESET_SOC:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			set_reset_fuelgague());
		break;
#endif /*CONFIG_BQ27425_FUEL_GAUGE*/
	case BATT_VOL:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.battery_vol);
		break;
	case BATT_VOL_ADC:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.batt_vol_adc);
		break;
	case BATT_TEMP_CHECK:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.batt_temp_check);
		break;
	case BATT_FULL_CHECK:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.batt_full_check);
		break;
	case CHARGING_SOURCE:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.charging_source);
		break;
	case BATT_CHG_CURRENT:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.chg_current_adc);
		break;
	case BATT_TEMP_ADC:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.battery_temp_adc);
		break;
	case BATT_TEMP:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.battery_temp);
		break;
	case BATTERY_TEMP_AVER:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.batt_temp_aver);
		break;
	case BATT_FUEL_CURRENT:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.batt_fuel_current);
		break;
	case BATT_VF_ADC:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.batt_vf_adc);
		break;
	case BATT_VF:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.batt_vf_adc);
	case BATT_TYPE:
		i += scnprintf(buf + i, PAGE_SIZE - i, "SDI_SDI\n");
		break;
#ifdef __BATT_TEST_DEVICE__
	case BATT_TEMP_TEST_ADC:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
				temp_test_adc);
		break;
#endif
	case CHARGINGBLOCK_CLEAR:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.chargingblock_clear);
		break;

	case BATT_SLATE_MODE:
		/*FOR SLATE TEST Don't charge by USB*/
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.batt_slate_mode);
		break;
	case BATT_TEMP_DEGC:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.batt_temp_degc);
		break;

	case CHG_TEMP_EVENT_CHECK:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.chg_temp_event_check);
		break;

	case VT_CALL:
	case TALK_WCDMA:
	case TALK_GSM:
#ifdef CONFIG_MACH_GEIM
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			(msm_batt_info.batt_use & USE_CALL) ? 1 : 0);
#else
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.talk_gsm);
#endif /*CONFIG_MACH_GEIM*/
		break;

/*	case DATA_CALL:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.data_call);
		break;*/

	case OFF_BACKLIGHT:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.off_backlight);
		break;
#ifdef CONFIG_MACH_GEIM
	case BATT_USE_CALL:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			(msm_batt_info.batt_use & USE_CALL) ? 1 : 0);
		break;
	case BATT_USE_VIDEO:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			(msm_batt_info.batt_use & USE_VIDEO) ? 1 : 0);
		break;
	case BATT_USE_MUSIC:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			(msm_batt_info.batt_use & USE_MUSIC) ? 1 : 0);
		break;
	case BATT_USE_BROWSER:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			(msm_batt_info.batt_use & USE_BROWSER) ? 1 : 0);
		break;
	case BATT_USE_HOTSPOT:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			(msm_batt_info.batt_use & USE_HOTSPOT) ? 1 : 0);
		break;
	case BATT_USE_CAMERA:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			(msm_batt_info.batt_use & USE_CAMERA) ? 1 : 0);
		break;
	case BATT_USE_DATA_CALL:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			(msm_batt_info.batt_use & USE_DATA_CALL) ? 1 : 0);
		break;
	case BATT_USE_GPS:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			(msm_batt_info.batt_use & USE_GPS) ? 1 : 0);
		break;
	case BATT_USE:
		i += scnprintf(buf + i, PAGE_SIZE - i, "%d\n",
			msm_batt_info.batt_use);
		break;
#endif /*CONFIG_MACH_GEIM*/


	default:
		i = -EINVAL;
	}

	return i;
}

static ssize_t msm_batt_store_property(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf, size_t count)
{
	int x = 0;
	int ret = 0;
	const ptrdiff_t offset = attr - msm_battery_attrs;


	switch (offset) {
#ifdef CONFIG_BQ27425_FUEL_GAUGE
	case FG_RESET_SOC:
		if (sscanf(buf, "%d\n", &x) == 1)
			ret = count;
		break;
#endif	/* CONFIG_BQ27425_FUEL_GAUGE */
#ifdef __BATT_TEST_DEVICE__
	case BATT_TEMP_TEST_ADC:
		if (sscanf(buf, "%d\n", &x) == 1) {
			if (x == 0)
				temp_test_adc = 0;
			else
				temp_test_adc = x;
			ret = count;
		}
		break;
#endif
	case CHARGINGBLOCK_CLEAR:
		if (sscanf(buf, "%d\n", &x) == 1) {
			pr_debug("\n[BATT] %s: chargingblock_clear -> "
				"write 0x%x\n\n", __func__, x);
			msm_batt_info.chargingblock_clear = x;
			ret = count;
		}
		break;

	case BATT_SLATE_MODE:
		if (sscanf(buf, "%d\n", &x) == 1) {
			msm_batt_info.batt_slate_mode = x;
			msm_batt_update_psy_status();
			ret = count;
		}
	    break;

	case CHG_TEMP_EVENT_CHECK:
		if (sscanf(buf, "%d\n", &x) == 1) {
			msm_batt_info.chg_temp_event_check = x;
			ret = count;
		}
		break;

	case TALK_GSM:
		if (sscanf(buf, "%d\n", &x) == 1) {
			msm_batt_info.talk_gsm = x;
			ret = count;
		}
		break;

	/*case DATA_CALL:
		if (sscanf(buf, "%d\n", &x) == 1) {
			msm_batt_info.data_call = x;
			ret = count;
		}
		break;*/

	case OFF_BACKLIGHT:
		if (sscanf(buf, "%d\n", &x) == 1) {
			msm_batt_info.off_backlight = x;
			ret = count;
		}
		break;
#ifdef CONFIG_MACH_GEIM
	case BATT_USE_CALL:
		if (sscanf(buf, "%d\n", &x) == 1) {
			/*during_call = x;*/
			msm_bat_use_module(USE_CALL, x);
			ret = count;
		}
		break;
	case BATT_USE_VIDEO:
		if (sscanf(buf, "%d\n", &x) == 1) {
			msm_bat_use_module(USE_VIDEO, x);
			ret = count;
		}
		break;
	case BATT_USE_MUSIC:
		if (sscanf(buf, "%d\n", &x) == 1) {
			msm_bat_use_module(USE_MUSIC, x);
			ret = count;
		}
		break;
	case BATT_USE_BROWSER:
		if (sscanf(buf, "%d\n", &x) == 1) {
			msm_bat_use_module(USE_BROWSER, x);
			ret = count;
		}
		break;
	case BATT_USE_HOTSPOT:
		if (sscanf(buf, "%d\n", &x) == 1) {
			msm_bat_use_module(USE_HOTSPOT, x);
			ret = count;
		}
		break;
	case BATT_USE_CAMERA:
		if (sscanf(buf, "%d\n", &x) == 1) {
			msm_bat_use_module(USE_CAMERA, x);
			ret = count;
		}
		break;
	case BATT_USE_DATA_CALL:
		if (sscanf(buf, "%d\n", &x) == 1) {
			msm_bat_use_module(USE_DATA_CALL, x);
			ret = count;
		}
		break;
	case BATT_USE_GPS:
		if (sscanf(buf, "%d\n", &x) == 1) {
			msm_bat_use_module(USE_GPS, x);
			ret = count;
		}
		break;
#endif /*CONFIG_MACH_GEIM*/
	default:
		return -EINVAL;
	}	/* end of switch */

	return ret;
}

static int msm_power_get_property(struct power_supply *psy,
				  enum power_supply_property psp,
				  union power_supply_propval *val)
{
	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		if (psy->type == POWER_SUPPLY_TYPE_MAINS)
			val->intval = msm_batt_info.charging_source & AC_CHG
			    ? 1 : 0;

		if (psy->type == POWER_SUPPLY_TYPE_USB) {
			if (msm_batt_info.batt_slate_mode)
				msm_batt_info.charging_source = NO_CHG;

			val->intval = msm_batt_info.charging_source & USB_CHG
			    ? 1 : 0;
		}
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

#ifdef CONFIG_BQ27425_FUEL_GAUGE
static u32 get_voltage_from_fuelgauge(void)
{
	if (is_attached)
		return bq27425_get_vcell();
	return 3700;
}

static u32 get_level_from_fuelgauge(void)
{
	if (is_attached)
		return bq27425_get_soc();
	return 70;
}

static int get_temp_from_fuelgauge(void)
{
	if (is_attached)
		return bq27425_get_temperature();
	return 30;
}

static int get_current_from_fuelgauge(void)
{
	if (is_attached)
		return bq27425_get_current();
	return 1;
}

static u32 set_reset_fuelgague(void)
{
	if (is_attached)
		return bq27425_reset_soc();
	return 1;
}

static u32 get_vf_open_from_fuelgauge(void)
{
	if (is_attached)
		return bq27425_get_flag();
	return 2;
}
#endif /* CONFIG_BQ27425_FUEL_GAUGE */

void msm_set_cable(struct msm_battery_callback *ptr,
			enum cable_type_t status)
{
	flush_work_sync(msm_batt_cable_wq);
	pr_info("%s : [BATT] cable_status(%d)\n", __func__, status);
	msm_batt_info.cable_status = status;
	queue_work(msm_batt_cable_wq, &msm_batt_wq_setcable);
}

static void msm_batt_set_cable_bt(struct work_struct *work)
{
	struct msm_battery_info *chg = &msm_batt_info;
	pr_info("%s : [BATT] cable_status(%d)\n", __func__, chg->cable_status);

	if (chg->cable_status == CABLE_TYPE_UNKNOWN) {
		msm_batt_info.charging_source = NO_CHG;
		msm_batt_info.batt_status =
			POWER_SUPPLY_STATUS_DISCHARGING;
		msm_batt_info.charger_type =
			CHARGER_TYPE_NONE;
		msm_batt_info.batt_full_check = 0;
		msm_batt_info.batt_recharging = 0;
		msm_batt_chg_en(false);
	} else {
		if (msm_batt_info.batt_health !=
			POWER_SUPPLY_HEALTH_UNSPEC_FAILURE)
			msm_batt_info.batt_health =
			POWER_SUPPLY_HEALTH_GOOD;
		else
			goto skip;
		if (chg->cable_status == CABLE_TYPE_TA) {
			msm_batt_info.charging_source = AC_CHG;
			msm_batt_info.charger_type = CHARGER_TYPE_WALL;
		} else {
			msm_batt_info.charging_source = USB_CHG;
			msm_batt_info.charger_type =
				CHARGER_TYPE_USB_PC;
		}
		msm_batt_info.batt_status =
			POWER_SUPPLY_STATUS_CHARGING;

		if (chg->ovp_status == 1) {
			msm_batt_info.batt_status =
			POWER_SUPPLY_STATUS_NOT_CHARGING;
			msm_batt_info.batt_full_check = 0;
			msm_batt_info.batt_recharging = 0;
			if (charging_boot == 1) {
				msm_batt_info.charging_source = NO_CHG;
		msm_batt_info.batt_status =
			POWER_SUPPLY_STATUS_DISCHARGING;
				msm_batt_info.charger_type =
					CHARGER_TYPE_NONE;
			}
		msm_batt_chg_en(false);
		} else {
			msm_batt_chg_en(true);
		}
	}
skip:
	power_supply_changed(&msm_psy_ac);
	power_supply_changed(&msm_psy_usb);
	pr_info("%s : [BATT] batt_status(%d) charger_type(%d) source(%d)\n",
		__func__, msm_batt_info.batt_status,
		msm_batt_info.charger_type, msm_batt_info.charging_source);
	if (set_timer == 1) {
		pr_info("[BATT] timer setting enable\n");
		queue_work(msm_batt_info.msm_batt_wq, &msm_batt_work);
		mod_timer(&msm_batt_info.timer,
			(jiffies + BATT_CHECK_INTERVAL));
	}
}

void msm_set_acc_type(struct msm_battery_callback *ptr,
				enum acc_type_t status)
{
	struct msm_battery_info *chg =
		container_of(ptr, struct msm_battery_info, callback);

	chg->acc_status = status;
	pr_info("%s : [BATT] acc_status = %d\n", __func__, chg->acc_status);
}

void msm_set_ovp_type(struct msm_battery_callback *ptr,
				enum ovp_type_t status)
{
	flush_work_sync(msm_batt_cable_wq);
	pr_info("[BATT:%s] ovp_status(%d)\n", __func__, status);
	msm_batt_info.ovp_status = status;
	queue_work(msm_batt_cable_wq, &msm_batt_wq_ovp);
}

static void msm_batt_ovp_bt(struct work_struct *work)
{
	struct msm_battery_info *chg = &msm_batt_info;

	pr_info("[BATT:%s] ovp_status(%d)\n", __func__, chg->ovp_status);

	if (chg->ovp_status == 1 && msm_batt_info.batt_status ==
		POWER_SUPPLY_STATUS_CHARGING) {
		pr_info("%s : [BATT] do ovp protection\n", __func__);

		msm_batt_info.batt_status =
			POWER_SUPPLY_STATUS_NOT_CHARGING;
		msm_batt_info.batt_full_check = 0;
		msm_batt_info.batt_recharging = 0;
		if (charging_boot == 1) {
			msm_batt_info.charging_source = NO_CHG;
			msm_batt_info.batt_status =
				POWER_SUPPLY_STATUS_DISCHARGING;
			msm_batt_info.charger_type =
				CHARGER_TYPE_NONE;
		}
		msm_batt_chg_en(false);
		power_supply_changed(&msm_psy_batt);
		if (charging_boot == 1) {
			power_supply_changed(&msm_psy_ac);
			power_supply_changed(&msm_psy_usb);
		}
	} else {
		if (chg->cable_status != 0
			&& msm_batt_info.batt_status ==
				POWER_SUPPLY_STATUS_NOT_CHARGING) {
			msm_batt_info.batt_status =
				POWER_SUPPLY_STATUS_CHARGING;
			msm_batt_chg_en(true);
			power_supply_changed(&msm_psy_batt);
		}
	}
}

static int msm_batt_power_get_property(struct power_supply *psy,
				       enum power_supply_property psp,
				       union power_supply_propval *val)
{
	switch (psp) {
	case POWER_SUPPLY_PROP_STATUS:
		val->intval = msm_batt_info.batt_status;
		break;
	case POWER_SUPPLY_PROP_HEALTH:
		val->intval = msm_batt_info.batt_health;
		break;
	case POWER_SUPPLY_PROP_PRESENT:
		val->intval = msm_batt_info.batt_valid;
		break;
	case POWER_SUPPLY_PROP_TECHNOLOGY:
		val->intval = msm_batt_info.batt_technology;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN:
		val->intval = msm_batt_info.voltage_max_design;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN:
		val->intval = msm_batt_info.voltage_min_design;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		val->intval = msm_batt_info.batt_voltage_now;
		break;
	case POWER_SUPPLY_PROP_CAPACITY:
		val->intval = msm_batt_info.batt_capacity;
		break;
	case POWER_SUPPLY_PROP_TEMP:
		val->intval = msm_batt_info.battery_temp;
		break;
	case POWER_SUPPLY_PROP_TYPE:
		val->intval = msm_batt_info.batt_type;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static void msm_batt_set_alarm(int seconds)
{
	ktime_t low_interval = ktime_set(seconds - 10, 0);
	ktime_t slack = ktime_set(20, 0);
	ktime_t next;

	pr_info("%s[BATT] set alarm\n", __func__);
	next = ktime_add(msm_batt_info.last_poll, low_interval);
	alarm_start_range(&msm_batt_info.alarm, next,
				ktime_add(next, slack));

}

static void msm_batt_alarm_manager(struct alarm *alarm)
{
	queue_work(msm_batt_info.msm_batt_wq, &msm_batt_work);
}

/*for rev03 version*/
#ifndef CONFIG_BQ27425_FUEL_GAUGE

int calculate_batt_level(int batt_volt)
{
	int scaled_level = 0;

	if (g_chg_en == 0) {
		chg_polling_cnt = 0;
		if (batt_volt >= BATT_RECHAR_VOLT) {
			scaled_level = 100;
			if ((prev_scaled_level < scaled_level) &&
				(prev_scaled_level != 0))
				scaled_level = prev_scaled_level;
		} else if (batt_volt >=  BATT_LEVEL6_VOLT) {
			scaled_level = ((batt_volt - BATT_LEVEL6_VOLT+1)*19)
				/(BATT_RECHAR_VOLT - BATT_LEVEL6_VOLT);
			scaled_level = scaled_level + 80;
			if ((prev_scaled_level < scaled_level) &&
				(prev_scaled_level != 0))
				scaled_level = prev_scaled_level;
		} else if (batt_volt >= BATT_LEVEL5_VOLT) {
			scaled_level = ((batt_volt - BATT_LEVEL5_VOLT)*15)
				/(BATT_LEVEL6_VOLT - BATT_LEVEL5_VOLT);
			scaled_level = scaled_level + 65;
		} else if (batt_volt >= BATT_LEVEL4_VOLT) {/*64% ~ 50%*/
			scaled_level = ((batt_volt - BATT_LEVEL4_VOLT)*15)
				/(BATT_LEVEL5_VOLT - BATT_LEVEL4_VOLT);
			scaled_level = scaled_level + 50;
		} else if (batt_volt >= BATT_LEVEL3_VOLT) { /*49% ~ 35%*/
			scaled_level = ((batt_volt - BATT_LEVEL3_VOLT)*15)
				/(BATT_LEVEL4_VOLT - BATT_LEVEL3_VOLT);
			scaled_level = scaled_level + 35;
		} else if (batt_volt >= BATT_LEVEL2_VOLT) {/*34% ~ 20%*/
			scaled_level = ((batt_volt - BATT_LEVEL2_VOLT)*15)
				/(BATT_LEVEL3_VOLT - BATT_LEVEL2_VOLT);
			scaled_level = scaled_level + 20;
		} else if (batt_volt >= BATT_LEVEL1_VOLT) {/*19% ~ 5%*/
			scaled_level = ((batt_volt - BATT_LEVEL1_VOLT)*15)
				/(BATT_LEVEL2_VOLT - BATT_LEVEL1_VOLT);
			scaled_level = scaled_level + 5;
		} else if (batt_volt > BATT_LOW_VOLT) {/*4% ~ 1%*/
			scaled_level = ((batt_volt - BATT_LOW_VOLT)*4)
				/(BATT_LEVEL1_VOLT - BATT_LOW_VOLT);
			scaled_level = scaled_level + 1;
		} else {
			if (msm_batt_info.charger_type == CHARGER_TYPE_NONE)
				scaled_level = 0;
			else
				scaled_level = 1;
		}

	} else {
		if (batt_volt >= BATT_RECHAR_VOLT) {/*100%*/
			if (prev_scaled_level >= 91) {
				if (prev_scaled_level >= 91 &&
					prev_scaled_level < 94)
					scaled_level = 91;
				if (prev_scaled_level >= 94 &&
					prev_scaled_level < 97)
					scaled_level = 94;
				if (prev_scaled_level >= 97 &&
					prev_scaled_level < 100)
					scaled_level = 97;
				if (prev_scaled_level == 100)
					scaled_level = 100;

				if (chg_polling_cnt != 0) {
					if (scaled_level != 100) {
						if ((chg_polling_cnt % 60) == 0)
							scaled_level += 3;
						if (scaled_level == 100)
							chg_polling_cnt = 0;
					}
				}
			} else {
				scaled_level = 91;
			}
		} else if (batt_volt >= BATT_LEVEL6_VOLT) {/*99% ~ 80%*/
			scaled_level = ((batt_volt - BATT_LEVEL6_VOLT+1)*10)
				/(BATT_RECHAR_VOLT - BATT_LEVEL6_VOLT);
			scaled_level = scaled_level+80;

			if (prev_scaled_level > scaled_level)
				scaled_level = prev_scaled_level;

		} else if (batt_volt >= BATT_LEVEL5_VOLT) {
			scaled_level = ((batt_volt - BATT_LEVEL5_VOLT)*15)
				/(BATT_LEVEL6_VOLT - BATT_LEVEL5_VOLT);
			scaled_level = scaled_level+65;
		} else if (batt_volt >= BATT_LEVEL4_VOLT) {
			scaled_level = ((batt_volt - BATT_LEVEL4_VOLT)*15)
				/(BATT_LEVEL5_VOLT - BATT_LEVEL4_VOLT);
			scaled_level = scaled_level+50;
		} else if (batt_volt >= BATT_LEVEL3_VOLT) {
			scaled_level = ((batt_volt - BATT_LEVEL3_VOLT)*15)
				/(BATT_LEVEL4_VOLT - BATT_LEVEL3_VOLT);
			scaled_level = scaled_level+35;
		} else if (batt_volt >= BATT_LEVEL2_VOLT) {
			scaled_level = ((batt_volt - BATT_LEVEL2_VOLT)*15)
				/(BATT_LEVEL3_VOLT - BATT_LEVEL2_VOLT);
			scaled_level = scaled_level+20;
		} else if (batt_volt >= BATT_LEVEL1_VOLT) {
			scaled_level = ((batt_volt - BATT_LEVEL1_VOLT)*15)
				/(BATT_LEVEL2_VOLT - BATT_LEVEL1_VOLT);
			scaled_level = scaled_level+5;
		} else if (batt_volt > BATT_LOW_VOLT) {
			scaled_level = ((batt_volt - BATT_LOW_VOLT)*4)
				/(BATT_LEVEL1_VOLT - BATT_LOW_VOLT);
			scaled_level = scaled_level+1;
		} else {
			if (msm_batt_info.charger_type == CHARGER_TYPE_NONE)
				scaled_level = 0;
			else
				scaled_level = 1;
		}
	}

	pr_info("[Battery] %s : batt_volt %d, scaled_level %d, g_chg_en %d\n",
		__func__, batt_volt, scaled_level, g_chg_en);

	prev_scaled_level = scaled_level;
	msm_batt_info.batt_capacity = scaled_level;
	return 1;
}

#define BATT_CAL_CHG 150

int calculate_batt_voltage(int vbatt_adc)
{
	int batt_volt = 0;
	static int prevVal;
	int chg_comp = 0;

#ifdef __CONTROL_CHARGING_SUDDEN_LEVEL_UP__
	if (!prevVal)
		prevVal = vbatt_adc;

	if ((vbatt_adc <= BATT_FULL_ADC) && (vbatt_adc > BATT_LEVEL9_ADC))
		chg_comp = 70;
	if ((vbatt_adc <= BATT_LEVEL9_ADC) && (vbatt_adc > BATT_LEVEL8_ADC))
		chg_comp = 210;
	if ((vbatt_adc <= BATT_LEVEL8_ADC) && (vbatt_adc > BATT_LEVEL7_ADC))
		chg_comp = 240;
	if ((vbatt_adc <= BATT_LEVEL7_ADC) && (vbatt_adc > BATT_LEVEL6_ADC))
		chg_comp = 280;
	if (vbatt_adc <= BATT_LEVEL6_ADC)
		chg_comp = 320;

	if (vbatt_adc < BATT_FULL_ADC) {
		if (g_chg_en) {
			if (prevVal < (vbatt_adc-chg_comp)) {
				vbatt_adc = vbatt_adc-chg_comp;
				pr_info("[Battery] vbatt_adc-BATT_CAL_CHG\n");
			} else {
				vbatt_adc = prevVal;
				pr_info("[Battery] chg_en & prevVal\n");
			}
		} else {
			if (prevVal < vbatt_adc)
				vbatt_adc = prevVal;
		}
	}
	prevVal = vbatt_adc;
#endif

	pr_info("[Battery] %s : vbatt_adc %d\n", __func__, vbatt_adc);

	if (vbatt_adc >= BATT_FULL_ADC) {
		batt_volt = BATT_FULL_VOLT;
	} else if (vbatt_adc >=  BATT_LEVEL6_ADC) {
		batt_volt = ((vbatt_adc - BATT_LEVEL6_ADC)*
			(BATT_FULL_VOLT - BATT_LEVEL6_VOLT))/
			(BATT_FULL_ADC - BATT_LEVEL6_ADC);
		batt_volt = batt_volt+BATT_LEVEL6_VOLT;
	} else if (vbatt_adc >=  BATT_LEVEL5_ADC) {
		batt_volt = ((vbatt_adc - BATT_LEVEL5_ADC)*
			(BATT_LEVEL6_VOLT - BATT_LEVEL5_VOLT))/
			(BATT_LEVEL6_ADC - BATT_LEVEL5_ADC);
		batt_volt = batt_volt+BATT_LEVEL5_VOLT;
	} else if (vbatt_adc >=  BATT_LEVEL4_ADC) {
		batt_volt = ((vbatt_adc - BATT_LEVEL4_ADC)*
			(BATT_LEVEL5_VOLT - BATT_LEVEL4_VOLT))/
			(BATT_LEVEL5_ADC - BATT_LEVEL4_ADC);
		batt_volt = batt_volt+BATT_LEVEL4_VOLT;
	} else if (vbatt_adc >=  BATT_LEVEL3_ADC) {
		batt_volt = ((vbatt_adc - BATT_LEVEL3_ADC)*
			(BATT_LEVEL4_VOLT - BATT_LEVEL3_VOLT))/
			(BATT_LEVEL4_ADC - BATT_LEVEL3_ADC);
		batt_volt = batt_volt+BATT_LEVEL3_VOLT;
	} else if (vbatt_adc >=  BATT_LEVEL2_ADC) {
		batt_volt = ((vbatt_adc - BATT_LEVEL2_ADC)*
			(BATT_LEVEL3_VOLT - BATT_LEVEL2_VOLT))/
			(BATT_LEVEL3_ADC - BATT_LEVEL2_ADC);
		batt_volt = batt_volt+BATT_LEVEL2_VOLT;
	} else if (vbatt_adc >=  BATT_LEVEL1_ADC) {
		batt_volt = ((vbatt_adc - BATT_LEVEL1_ADC)*
			(BATT_LEVEL2_VOLT - BATT_LEVEL1_VOLT))/
			(BATT_LEVEL2_ADC - BATT_LEVEL1_ADC);
		batt_volt = batt_volt+BATT_LEVEL1_VOLT;
	} else if (vbatt_adc >  BATT_LOW_ADC) {
		batt_volt = ((vbatt_adc - BATT_LOW_ADC)*
			(BATT_LEVEL1_VOLT - BATT_LOW_VOLT))/
			(BATT_LEVEL1_ADC - BATT_LOW_ADC);
		batt_volt = batt_volt+BATT_LOW_VOLT;
	} else {
		batt_volt = BATT_LOW_VOLT;
	}
	pr_info("[Battery] %s : vbatt_adc %d & batt_volt %d\n",
		__func__, vbatt_adc, batt_volt);

	return batt_volt;
}
#endif /*CONFIG_MAX17043_FUEL_GAUGE*//*end for rev 03 version for cal voltage*/

static void msm_batt_chg_en(chg_enable_type enable)
{
	if (enable == START_CHARGING) {
		if (msm_batt_info.charging_source == NO_CHG) {
			pr_err("[BATT] %s: charging_source not defined!\n",
				__func__);
			return ;
		}

		if (msm_batt_info.charging_source & AC_CHG) {
#if 0
			if (msm_batt_info.batt_use == 1) {
				pr_info("%s[BATT] set event charging current\n",
					__func__);
				hsusb_chg_connected(USB_CHG_TYPE__SDP);
				hsusb_chg_vbus_draw(500);
			} else {
#endif
			pr_info("[BATT] %s: Start charging! (AC)\n",
				__func__);
				hsusb_chg_connected
					(USB_CHG_TYPE__WALLCHARGER);
				hsusb_chg_vbus_draw(650);
#if 0
			}
#endif

		} else {
			pr_info("[BATT] %s: Start charging! (USB)\n",
				__func__);
			hsusb_chg_connected(USB_CHG_TYPE__SDP);
			hsusb_chg_vbus_draw(500);	/*USB charging 450mA*/
		}

		msm_batt_set_charging_start_time(START_CHARGING);
		pr_info("[BATT] %s: Starp charging! (0x%x,"
			" full_check = %d)\n", __func__,
			msm_batt_info.charging_source,
			msm_batt_info.batt_full_check);
		if (fuel_alert_det) {
			wake_unlock(&fuel_alert_wake_lock);
			pr_info("[BATT:%s] fuel alert unlock", __func__);
		fuel_alert_det = 0;
		}

	} else {
		msm_batt_set_charging_start_time(STOP_CHARGING);

		if (msm_batt_info.charging_source == NO_CHG)
			hsusb_chg_connected(USB_CHG_TYPE__INVALID);
		else
			hsusb_chg_vbus_draw(0);

		msm_batt_average_chg_current(-1); /*initialize all*/

		pr_info("[BATT] %s: Stop charging! (charging_source = 0x%x, "
			"full_check = %d)\n", __func__,
			msm_batt_info.charging_source,
			msm_batt_info.batt_full_check);
	}
}

static int msm_batt_average_chg_current(int chg_current_adc)
{
	static int history[AVERAGE_COUNT] = {0};
	static int count;
	static int index;
	int i, sum, max, min, ret;

	if (chg_current_adc == 0)
		return 0;

	if (chg_current_adc < 0) {
		count = 0;
		index = 0;
		for (i = 0; i < AVERAGE_COUNT; i++)
			history[i] = 0;

		return 0;
	}

	if (count == 0)
		for (i = 0; i < AVERAGE_COUNT; i++)
			history[i] = chg_current_adc;

	if (index >= count)
		count++;

	max = min = history[0];
	sum = 0;

	for (i = 0; i < AVERAGE_COUNT; i++) {
		if (i == index)
			history[i] = chg_current_adc;

		if (max < history[i])
			max = history[i];
		if (min > history[i])
			min = history[i];

		sum += history[i];
	}

	ret = ((sum-max-min) / (AVERAGE_COUNT-2));

	index++;

	if (index == AVERAGE_COUNT) {
		history[0] = ret;
		index = 1;
	}

	pr_debug("[BATT] %s: adc=%d, sum=%d, max=%d, min=%d, ret=%d\n",
		__func__, chg_current_adc, sum, max, min, ret);

	if (count < AVERAGE_COUNT)
		return BATT_FULL_CHARGING_CURRENT+50;

	if (ret != msm_batt_info.chg_current_adc) {
		msm_batt_info.chg_current_adc = ret;
		return 1;
	} else {
		return 0;
	}
}

static int msm_batt_check_full_charging(int chg_current_adc)
{
	static unsigned int time_after_under_tsh;

#if defined(CONFIG_MACH_JENA)
	if (msm_batt_info.charging_source == NO_CHG)
		return 0;
#else
	if (chg_current_adc == 0 ||
		msm_batt_info.charging_source == NO_CHG)
		return 0;
#endif

	if (msm_batt_is_over_abs_time()) {
		pr_info("[BATT] %s: Fully charged, over abs time! (recharging=%d)\n",
			__func__, msm_batt_info.batt_recharging);
		msm_batt_info.batt_full_check = 1;
		msm_batt_info.batt_recharging = 0;
#if defined(CONFIG_MACH_JENA)
		if (msm_batt_info.battery_vol >= 4100) {
			msm_batt_info.batt_status = POWER_SUPPLY_STATUS_FULL;
			msm_batt_info.batt_capacity = 100;
		} else
			msm_batt_info.batt_status = POWER_SUPPLY_STATUS_DISCHARGING;
#else
		msm_batt_info.batt_status = POWER_SUPPLY_STATUS_FULL;
		msm_batt_info.batt_capacity = 100;
#endif
		msm_batt_chg_en(STOP_CHARGING);
		return 1;
	}

#if defined(CONFIG_MACH_JENA)
	if (chg_current_adc == 0)
		return 0;
#endif

	if (msm_batt_info.battery_vol >= BATT_FULL_CHARGING_VOLTAGE) {
		if (chg_current_adc < BATT_FULL_CHARGING_CURRENT &&
			msm_batt_info.batt_capacity >= 99)	{
			if (time_after_under_tsh == 0) {
				time_after_under_tsh = jiffies;
			} else {
				if (time_after((unsigned long)jiffies,
					(unsigned long)(time_after_under_tsh
					+ TOTAL_WATING_TIME))) {
					pr_info("[BATT] %s: Fully charged, "
						"(voltage=%d, ICHG=%d)\n",
						__func__,
						msm_batt_info.battery_vol,
						chg_current_adc);
					msm_batt_info.batt_full_check = 1;
					msm_batt_info.batt_recharging = 0;
					msm_batt_info.batt_status =
						POWER_SUPPLY_STATUS_FULL;
					msm_batt_info.batt_capacity = 100;
					time_after_under_tsh = 0;
					msm_batt_chg_en(STOP_CHARGING);
					return 1;
				}
			}
		} else {
			time_after_under_tsh = 0;
		}
	}

	return 0;
}

static int msm_batt_check_recharging(void)
{
	static unsigned int time_after_vol1, time_after_vol2;

	if ((msm_batt_info.batt_full_check == 0) ||
		(msm_batt_info.batt_recharging == 1) ||
		(msm_batt_info.batt_health != POWER_SUPPLY_HEALTH_GOOD)) {
		time_after_vol1 = 0;
		time_after_vol2 = 0;
		return 0;
	}

	/* check 1st voltage */
	if (msm_batt_info.battery_vol <= BATT_RECHARGING_VOLTAGE_1) {
		if (time_after_vol1 == 0)
			time_after_vol1 = jiffies;

		if (time_after((unsigned long)jiffies,
			(unsigned long)(time_after_vol1 + TOTAL_WATING_TIME))) {
			pr_info("[BATT] %s: Recharging ! (voltage1 = %d)\n",
				__func__, msm_batt_info.battery_vol);
			msm_batt_info.batt_recharging = 1;
#if defined(CONFIG_MACH_JENA)
		if (msm_batt_info.battery_vol < 4000)
			msm_batt_info.batt_status = POWER_SUPPLY_STATUS_CHARGING;
#endif
			msm_batt_chg_en(START_CHARGING);
			return 1;
		}
	} else {
		time_after_vol1 = 0;
	}
	/* check 2nd voltage */
	if (msm_batt_info.battery_vol <= BATT_RECHARGING_VOLTAGE_2) {
		if (time_after_vol2 == 0)
			time_after_vol2 = jiffies;

		if (time_after((unsigned long)jiffies,
			(unsigned long)(time_after_vol2 + TOTAL_WATING_TIME))) {
			pr_info("[BATT] %s: Recharging ! (voltage2 = %d)\n",
				__func__, msm_batt_info.battery_vol);
			msm_batt_info.batt_recharging = 1;
#if defined(CONFIG_MACH_JENA)
		if (msm_batt_info.battery_vol < 4000)
			msm_batt_info.batt_status = POWER_SUPPLY_STATUS_CHARGING;
#endif
			msm_batt_chg_en(START_CHARGING);
			return 1;
		}
	} else {
		time_after_vol2 = 0;
	}

	return 0;
}

static int msm_batt_check_level(int battery_level)
{


	if (msm_batt_info.batt_status != POWER_SUPPLY_STATUS_FULL &&
		(battery_level == 100))
		battery_level = 99;

	if (msm_batt_info.batt_capacity != battery_level) {
		if (msm_batt_info.batt_status != POWER_SUPPLY_STATUS_FULL) {
			if (msm_batt_info.boot_flag) {
				msm_batt_info.boot_flag = 0;
			msm_batt_info.batt_capacity = battery_level;
			} else {
				if (msm_batt_info.resume_flag) {
					pr_info("[BATT] resume!\n");
					if (battery_level == 0)
						msm_batt_info.batt_capacity
							= 2;
					else
						msm_batt_info.batt_capacity
							= battery_level;
					msm_batt_info.resume_flag = 0;
				} else {
				if (msm_batt_info.batt_capacity
					> battery_level) {
						msm_batt_info.batt_capacity
							-= 1;
					}
				else if (msm_batt_info.batt_capacity
					< battery_level) {
					if (battery_level > 10 ||
					msm_batt_info.batt_status !=
					POWER_SUPPLY_STATUS_DISCHARGING)
						msm_batt_info.batt_capacity
							+= 1;
					}
				}
			}
		} else {
			msm_batt_info.batt_capacity = 100;
		}
		pr_info("[BATT]AVE_SOC : %d\n", msm_batt_info.batt_capacity);
	}

	if (msm_batt_info.battery_vol <= BATTERY_LOW)
		return 1;

	return 0;
}

static int msm_batt_average_temperature(int temp_adc)
{
	static int history[AVERAGE_COUNT] = {0};
	static int count;
	static int index;
	int i, sum, max, min, ret;

	if (temp_adc == 0)
		return 0;

#ifdef __BATT_TEST_DEVICE__
		if (temp_test_adc)
			return temp_test_adc;
#endif

	if (count == 0)
		for (i = 0; i < AVERAGE_COUNT; i++)
			history[i] = temp_adc;

	if (index >= count)
		count++;

	max = min = history[0];
	sum = 0;

	for (i = 0; i < AVERAGE_COUNT; i++) {
		if (i == index)
			history[i] = temp_adc;

		if (max < history[i])
			max = history[i];
		if (min > history[i])
			min = history[i];

		sum += history[i];
	}

	ret = ((sum-max-min) / (AVERAGE_COUNT-2));

	index++;

	if (index == AVERAGE_COUNT) {
		history[0] = ret;
		index = 1;
	}

	pr_debug("[BATT] %s: adc=%d, sum=%d, max=%d, min=%d, ret=%d\n",
		__func__, temp_adc, sum, max, min, ret);
	return ret;
}

static int msm_batt_control_temperature(int temp_adc)
{
	int prev_health = msm_batt_info.batt_health;
	int new_health = prev_health;
	int array_size = 0;
	int i;
	int degree;

	int high_block;
	int high_recover;
	int low_block;
	int low_recover;

	static char *health_text[] = {
		"Unknown", "Good", "Overheat", "Dead", "Over voltage",
		"Unspecified failure", "Cold",
	};

	if (charging_boot == 1) {/*lpm mode*/
		high_block = BATT_TEMP_HIGH_BLOCK_LPM;
		high_recover = BATT_TEMP_HIGH_RECOVER_LPM;
		low_block = BATT_TEMP_LOW_BLOCK_LPM;
		low_recover = BATT_TEMP_LOW_RECOVER_LPM;
	} else {
		high_block = BATT_TEMP_HIGH_BLOCK;
		high_recover = BATT_TEMP_HIGH_RECOVER;
		low_block = BATT_TEMP_LOW_BLOCK;
		low_recover = BATT_TEMP_LOW_RECOVER;
	}

#ifdef CONFIG_MACH_GEIM
	if (msm_batt_info.batt_use && !charging_boot) {
		pr_info("%s[BATT] set event block\n", __func__);
		high_block = BATT_TEMP_EVENT_BLOCK;
	}
#endif

#ifdef __AUTO_TEMP_TEST__
	static unsigned int auto_test_start_time;
	static unsigned int auto_test_interval = (2 * 60*HZ);
	static int auto_test_mode;
	/* 0: normal (recover cold), 1: force overheat,
	  * 2: normal (recover overheat), 3: force cold
	  */

	if (msm_batt_info.charging_source != NO_CHG) {
		if (auto_test_start_time == 0)
			auto_test_start_time = jiffies;

		if (time_after((unsigned long)jiffies, (unsigned long)
			(auto_test_start_time + auto_test_interval))) {
			auto_test_mode++;
			if (auto_test_mode > 3)
				auto_test_mode = 0;
			auto_test_start_time = jiffies;
		}
		pr_debug("[BATT] auto test mode = %d (0:normal,1:overheat,"
			"2:normal,3:cold)\n", auto_test_mode);

		if (auto_test_mode == 1) {
			temp_adc = high_block + 10;
			msm_batt_info.battery_temp_adc = temp_adc;
		} else if (auto_test_mode == 3) {
			temp_adc = high_block - 10;
			msm_batt_info.battery_temp_adc = temp_adc;
		}
	} else {
		auto_test_start_time = 0;
		auto_test_mode = 0;
	}
#endif
/*
	array_size = ARRAY_SIZE(temp_table);
	for (i = 0; i <= (array_size - 1); i++) {
		if (i == 0) {
			if (temp_adc >= temp_table[0][0]) {
				degree = temp_table[0][1];
				break;
			} else if (temp_adc <= temp_table[array_size-1][0]) {
				degree = temp_table[array_size-1][1];
				break;
			}
		}

		if (temp_table[i][0] < temp_adc &&
			temp_table[i-1][0] >= temp_adc)
			degree = temp_table[i-1][1];
	}
*/
	msm_batt_info.battery_temp = temp_adc * 10;
	pr_info("%s [BATT] battery_temp : %d temp_adc : %d\n", __func__,
		msm_batt_info.battery_temp, temp_adc);
	/*  TODO:  check application */

	if (prev_health == POWER_SUPPLY_HEALTH_UNSPEC_FAILURE)
		return 0;

	if (temp_adc >= high_block) {
		if (prev_health != POWER_SUPPLY_HEALTH_OVERHEAT)
			new_health = POWER_SUPPLY_HEALTH_OVERHEAT;
#ifdef CONFIG_MACH_GEIM
	} else if ((temp_adc <= high_block) && (temp_adc >= high_recover)
			&& msm_batt_info.batt_use) {
		if (prev_health == POWER_SUPPLY_HEALTH_OVERHEAT)
			new_health = POWER_SUPPLY_HEALTH_GOOD;
#endif
	} else if ((temp_adc <= high_recover)
		&& (temp_adc >= low_recover)) {
		if ((prev_health == POWER_SUPPLY_HEALTH_OVERHEAT) ||
			(prev_health == POWER_SUPPLY_HEALTH_COLD))
			new_health = POWER_SUPPLY_HEALTH_GOOD;
	} else if (temp_adc <= low_block) {
		if (prev_health != POWER_SUPPLY_HEALTH_COLD)
			new_health = POWER_SUPPLY_HEALTH_COLD;
	}

	if (msm_batt_info.charging_source == NO_CHG) {
		if ((low_block < temp_adc)
			&& (temp_adc < high_block)) {
			if ((prev_health == POWER_SUPPLY_HEALTH_OVERHEAT) ||
				(prev_health == POWER_SUPPLY_HEALTH_COLD))
				new_health = POWER_SUPPLY_HEALTH_GOOD;
		}
	}

	if (msm_batt_info.chargingblock_clear != 0x0)
		new_health = POWER_SUPPLY_HEALTH_GOOD;

	if (prev_health != new_health) {
		if (msm_batt_info.charging_source == NO_CHG) {
			pr_info("[BATT] %s: Health changed by temperature!"
				" (ADC = %d, %s-> %s)\n", __func__,
				temp_adc, health_text[prev_health],
				health_text[new_health]);
			msm_batt_info.batt_status =
				POWER_SUPPLY_STATUS_DISCHARGING;
		} else {
			if (new_health != POWER_SUPPLY_HEALTH_GOOD)	{
				pr_info("[BATT] %s: Block charging! (ADC ="
					" %d, %s-> %s)\n", __func__,
					temp_adc, health_text[prev_health],
					health_text[new_health]);
				msm_batt_info.batt_status =
					POWER_SUPPLY_STATUS_NOT_CHARGING;
				msm_batt_chg_en(STOP_CHARGING);
			} else {
				pr_info("[BATT] %s: Recover charging! (ADC"
					" = %d, %s-> %s)\n", __func__,
					temp_adc, health_text[prev_health],
					health_text[new_health]);
				msm_batt_info.batt_status =
					POWER_SUPPLY_STATUS_CHARGING;
				msm_batt_chg_en(START_CHARGING);
			}
		}

		msm_batt_info.batt_health = new_health;
		return 1;
	}
	return 0;
}

#ifndef CONFIG_BQ27425_FUEL_GAUGE
#ifndef CONFIG_BATTERY_MSM_FAKE
struct msm_batt_get_volt_ret_data {
	u32 battery_vol;
};
#endif
static int msm_batt_get_volt_ret_func(struct msm_rpc_client *batt_client,
				       void *buf, void *data)
{
	struct msm_batt_get_volt_ret_data *data_ptr, *buf_ptr;

	data_ptr = (struct msm_batt_get_volt_ret_data *)data;
	buf_ptr = (struct msm_batt_get_volt_ret_data *)buf;

	data_ptr->battery_vol = be32_to_cpu(buf_ptr->battery_vol);

	return 0;
}


static u32 msm_batt_get_vbatt_voltage(void)
{
	int rc;

	struct msm_batt_get_volt_ret_data rep;

	rc = msm_rpc_client_req(msm_batt_info.batt_client,
			BATTERY_READ_MV_PROC,
			NULL, NULL,
			msm_batt_get_volt_ret_func, &rep,
			msecs_to_jiffies(BATT_RPC_TIMEOUT));

	if (rc < 0) {
		pr_err("%s: FAIL: vbatt get volt. rc=%d\n", __func__, rc);
		return 0;
	}

	return rep.battery_vol;

}
#endif /*CONFIG_BQ27425_FUEL_GAUGE*/

#define	be32_to_cpu_self(v)	(v = be32_to_cpu(v))
#define	be16_to_cpu_self(v)	(v = be16_to_cpu(v))

static int msm_batt_get_batt_chg_status(void)
{
	int rc;
	struct rpc_req_batt_chg {
		struct rpc_request_hdr hdr;
		u32 more_data;
	} req_batt_chg;
	struct rpc_reply_batt_chg_v1 *v1p;

	req_batt_chg.more_data = cpu_to_be32(1);

	memset(&rep_batt_chg, 0, sizeof(rep_batt_chg));

	v1p = &rep_batt_chg.v1;
	rc = msm_rpc_call_reply(msm_batt_info.chg_ep,
				ONCRPC_CHG_GET_GENERAL_STATUS_PROC,
				&req_batt_chg, sizeof(req_batt_chg),
				&rep_batt_chg, sizeof(rep_batt_chg),
				msecs_to_jiffies(BATT_RPC_TIMEOUT));
	if (rc < 0) {
		pr_err("%s: ERROR. msm_rpc_call_reply failed! proc=%d rc=%d\n",
		       __func__, ONCRPC_CHG_GET_GENERAL_STATUS_PROC, rc);
		return rc;
	} else if (be32_to_cpu(v1p->more_data)) {
		be32_to_cpu_self(v1p->charger_status);
		be32_to_cpu_self(v1p->charger_type);
		be32_to_cpu_self(v1p->battery_status);
		be32_to_cpu_self(v1p->battery_level);
		be32_to_cpu_self(v1p->battery_voltage);
		be32_to_cpu_self(v1p->battery_temp);
		be32_to_cpu_self(v1p->battery_temp_adc);
		be32_to_cpu_self(v1p->chg_current);
		be32_to_cpu_self(v1p->batt_id);
		be32_to_cpu_self(v1p->battery_temp_degree);
	} else {
		pr_err("%s: No battery/charger data in RPC reply\n", __func__);
		return -EIO;
	}

	return 0;
}
#ifdef CONFIG_BQ27425_FUEL_GAUGE
static int msm_batt_check_vfopen()
{
	int ret = 0;
	if (msm_batt_info.acc_status == ACC_TYPE_JIG)
		return 1;
	ret = get_vf_open_from_fuelgauge();

	if (ret == -1) {
		pr_info("%s[BATT] battery is not attached\n", __func__);
		msm_batt_info.batt_health = POWER_SUPPLY_HEALTH_UNSPEC_FAILURE;
		msm_batt_info.batt_status = POWER_SUPPLY_STATUS_NOT_CHARGING;
		msm_batt_chg_en(STOP_CHARGING);
	} else if (ret == 1) {
		/*pr_info("%s[BATT] battery is attached\n", __func__);*/
		if (msm_batt_info.batt_health ==
			POWER_SUPPLY_HEALTH_UNSPEC_FAILURE)
			msm_batt_info.batt_health = POWER_SUPPLY_HEALTH_GOOD;
		msm_batt_unhandled_interrupt = 1;
	} else {
		pr_info("%s[BATT]fuelgauge is not attached\n", __func__);
	}
	return ret;
}
#endif

static void msm_batt_update_psy_status(void)
{
	u32	charger_status;
	u32	charger_type;
	u32	battery_status;
	u32	battery_level;
	u32	battery_vol;
	int	battery_temp_adc;
	u32	chg_current_adc;

	int	battery_temp;
	u32	battery_current;


	u32 status_changed = 0;
	u32 i = 0;
	u32 rc;

	static int batt_prestatus;
	static int flag;

	struct timespec ts;

	/*(+)Open Check*/
	if (!flag) {
		msm_batt_chg_en(STOP_CHARGING);
		pr_info("[BATT] %s: STOP Charging\n", __func__);
		msleep(3);
		msm_batt_chg_en(START_CHARGING);
		pr_info("[BATT] %s: Start Charging\n", __func__);
		flag = 1;
	/*Check LPM mode*/
		if (charging_boot) {
			msm_batt_info.batt_voltage_now = charging_boot;
			batt_prestatus = msm_batt_info.batt_status;
			msm_batt_info.batt_status = 0;
			}
	} else {
		batt_prestatus = msm_batt_info.batt_status;
	}

	/* Get general status from CP by RPC */
	if (msm_batt_get_batt_chg_status())
		return;

	bc_read_status(&msm_batt_info);

#ifndef CONFIG_BQ27425_FUEL_GAUGE
	charger_status = rep_batt_chg.v1.charger_status;
	charger_type = rep_batt_chg.v1.charger_type;
	battery_status = rep_batt_chg.v1.battery_status;
	battery_level = rep_batt_chg.v1.battery_level;
	battery_vol = rep_batt_chg.v1.battery_voltage;
	battery_temp = rep_batt_chg.v1.battery_temp;
	chg_current_adc = rep_batt_chg.v1.chg_current;

	if (charging_boot)
		msm_batt_info.batt_status = batt_prestatus;

	battery_temp = msm_batt_info.batt_temp_aver;
	msm_batt_info.battery_vol = msm_batt_get_vbatt_voltage();
	msm_batt_info.batt_voltage_now = msm_batt_info.battery_vol * 1000;
	msm_batt_info.batt_vol_adc = msm_batt_info.battery_vol;

	pr_info("%s [BATTERY] Rev03 level : %d, voltage : %d"
		", temp : %d\n", __func__, battery_level,
		msm_batt_info.battery_vol, battery_temp);
#endif
#ifdef CONFIG_BQ27425_FUEL_GAUGE
	battery_level = get_level_from_fuelgauge();
	/*msm_batt_info.chg_current_adc = battery_level;*/
	msm_batt_info.battery_vol = get_voltage_from_fuelgauge();
	msm_batt_info.batt_voltage_now = msm_batt_info.battery_vol * 1000;
	msm_batt_info.batt_fuel_current = get_current_from_fuelgauge();
	msm_batt_info.chg_current_adc = msm_batt_info.batt_fuel_current;
	battery_temp = get_temp_from_fuelgauge();
	msm_batt_info.batt_temp_aver =  battery_temp * 10;

	msm_batt_info.batt_vol_adc = msm_batt_info.battery_vol;

	if (charging_boot)
		msm_batt_info.batt_status = batt_prestatus;

	/*chg_current_adc = rep_batt_chg.v1.chg_current;*/
	battery_temp_adc = rep_batt_chg.v1.battery_temp_adc;
	/*msm_batt_info.battery_temp = rep_batt_chg.v1.battery_temp_degree;*/

	pr_info("%s [BATTERY] Rev05 level : %d, voltage : %d, temp_ap %d"
		" temp_cp : %d, current_fg : %d cur_cp :%d status %d"
		"battery_temp_adc\n",
		__func__, battery_level, msm_batt_info.battery_vol,
		msm_batt_info.battery_temp, msm_batt_info.batt_temp_aver,
		msm_batt_info.batt_fuel_current, chg_current_adc,
		msm_batt_info.batt_status, battery_temp_adc);
#endif	/* CONFIG_MAX17043_FUEL_GAUGE */

	if ((msm_batt_info.batt_status == POWER_SUPPLY_STATUS_CHARGING) ||
		(msm_batt_info.batt_recharging == 1))
		if (chg_current_adc < 30 || chg_current_adc > 900)
			chg_current_adc = 0;
	else
		chg_current_adc = 0;
#ifdef CONFIG_BQ27425_FUEL_GAUGE
	if (msm_batt_info.cable_status != CABLE_TYPE_UNKNOWN)
		msm_batt_check_vfopen();
#endif
	/* check temperature */
	msm_batt_info.battery_temp_adc =
	msm_batt_average_temperature(battery_temp_adc);

	msm_batt_control_temperature(battery_temp);

	/* check full charging */
	/*msm_batt_average_chg_current(msm_batt_info.batt_fuel_current);*/
	if (msm_batt_info.cable_status != CABLE_TYPE_UNKNOWN ||
		msm_batt_info.charger_type != CHARGER_TYPE_NONE)
		msm_batt_check_full_charging(msm_batt_info.chg_current_adc);

	/* check recharging */
	msm_batt_check_recharging();

#ifdef CONFIG_BQ27425_FUEL_GAUGE
		msm_batt_check_level(battery_level);
#else
	calculate_batt_level(msm_batt_info.battery_vol);
#endif

	/* temperature health for power off charging */
	if (msm_batt_info.batt_health == POWER_SUPPLY_HEALTH_GOOD)
		msm_batt_info.batt_temp_check = 1;
	else
		msm_batt_info.batt_temp_check = 0;
	/* recheck battery status*/
	if ((msm_batt_info.cable_status == CABLE_TYPE_UNKNOWN ||
		msm_batt_info.charger_type == CHARGER_TYPE_NONE) &&
		(msm_batt_info.batt_status ==
			POWER_SUPPLY_STATUS_CHARGING ||
		msm_batt_info.batt_status ==
			POWER_SUPPLY_STATUS_FULL)) {
		msm_batt_info.batt_status =
			POWER_SUPPLY_STATUS_DISCHARGING;
		pr_info("[BATT]forced change charging mode\n");
		}

#ifndef DEBUG
	if (msm_batt_info.charging_source != NO_CHG) {
#endif
		pr_info("[BATT] %s: charger_status= %d, charger_type= %d,"
			"battery_status=%d, battery_temp_adc=%d,"
			"chg_current=%d, battery_temp=%d, health %d"
			"status = %d\n",
			__func__, msm_batt_info.charger_status,
			msm_batt_info.charger_type,
			msm_batt_info.battery_status,
			msm_batt_info.battery_temp_adc,
			msm_batt_info.chg_current_adc,
			msm_batt_info.battery_temp_adc,
			msm_batt_info.batt_health,
			msm_batt_info.batt_status);
#ifndef DEBUG
	}
#endif

	power_supply_changed(&msm_psy_batt);

	msm_batt_info.last_poll = alarm_get_elapsed_realtime();
	ts = ktime_to_timespec(msm_batt_info.last_poll);

	if (msm_batt_unhandled_interrupt)
		msm_batt_unhandled_interrupt = 0;

}

#ifdef CONFIG_HAS_EARLYSUSPEND
struct batt_modify_client_req {

	u32 client_handle;

	/* The voltage at which callback (CB) should be called. */
	u32 desired_batt_voltage;

	/* The direction when the CB should be called. */
	u32 voltage_direction;

	/* The registered callback to be called when voltage and
	 * direction specs are met. */
	u32 batt_cb_id;

	/* The call back data */
	u32 cb_data;
};

struct batt_modify_client_rep {
	u32 result;
};

static int msm_batt_modify_client_arg_func(struct msm_rpc_client *batt_client,
				       void *buf, void *data)
{
	struct batt_modify_client_req *batt_modify_client_req =
		(struct batt_modify_client_req *)data;
	u32 *req = (u32 *)buf;
	int size = 0;

	*req = cpu_to_be32(batt_modify_client_req->client_handle);
	size += sizeof(u32);
	req++;

	*req = cpu_to_be32(batt_modify_client_req->desired_batt_voltage);
	size += sizeof(u32);
	req++;

	*req = cpu_to_be32(batt_modify_client_req->voltage_direction);
	size += sizeof(u32);
	req++;

	*req = cpu_to_be32(batt_modify_client_req->batt_cb_id);
	size += sizeof(u32);
	req++;

	*req = cpu_to_be32(batt_modify_client_req->cb_data);
	size += sizeof(u32);

	return size;
}

static int msm_batt_modify_client_ret_func(struct msm_rpc_client *batt_client,
				       void *buf, void *data)
{
	struct  batt_modify_client_rep *data_ptr, *buf_ptr;

	data_ptr = (struct batt_modify_client_rep *)data;
	buf_ptr = (struct batt_modify_client_rep *)buf;

	data_ptr->result = be32_to_cpu(buf_ptr->result);

	return 0;
}

static int msm_batt_modify_client(u32 client_handle, u32 desired_batt_voltage,
	     u32 voltage_direction, u32 batt_cb_id, u32 cb_data)
{
	int rc;

	struct batt_modify_client_req  req;
	struct batt_modify_client_rep rep;

	req.client_handle = client_handle;
	req.desired_batt_voltage = desired_batt_voltage;
	req.voltage_direction = voltage_direction;
	req.batt_cb_id = batt_cb_id;
	req.cb_data = cb_data;

	rc = msm_rpc_client_req(msm_batt_info.batt_client,
			BATTERY_MODIFY_CLIENT_PROC,
			msm_batt_modify_client_arg_func, &req,
			msm_batt_modify_client_ret_func, &rep,
			msecs_to_jiffies(BATT_RPC_TIMEOUT));

	if (rc < 0) {
		pr_err("%s: ERROR. failed to modify  Vbatt client\n",
		       __func__);
		return rc;
	}

	if (rep.result != BATTERY_MODIFICATION_SUCCESSFUL) {
		pr_err("%s: ERROR. modify client failed. result = %u\n",
		       __func__, rep.result);
		return -EIO;
	}

	return 0;
}

void msm_batt_early_suspend(struct early_suspend *h)
{
	int rc;

	pr_debug("%s: enter\n", __func__);

	if (msm_batt_info.batt_handle != INVALID_BATT_HANDLE) {
		rc = msm_batt_modify_client(msm_batt_info.batt_handle,
				BATTERY_LOW, BATTERY_VOLTAGE_BELOW_THIS_LEVEL,
				BATTERY_CB_ID_LOW_VOL, BATTERY_LOW);

		if (rc < 0) {
			pr_err("%s: msm_batt_modify_client. rc=%d\n",
			       __func__, rc);
			return;
		}
		msm_batt_info.suspend_status = 1;
	} else {
		pr_err("%s: ERROR. invalid batt_handle\n", __func__);
		return;
	}

	pr_debug("%s: exit\n", __func__);
}

void msm_batt_late_resume(struct early_suspend *h)
{
	int rc;

	pr_debug("%s: enter\n", __func__);

	if (msm_batt_info.batt_handle != INVALID_BATT_HANDLE) {
		rc = msm_batt_modify_client(msm_batt_info.batt_handle,
				BATTERY_LOW, BATTERY_ALL_ACTIVITY,
			       BATTERY_CB_ID_ALL_ACTIV, BATTERY_ALL_ACTIVITY);
		if (rc < 0) {
			pr_err("%s: msm_batt_modify_client FAIL rc=%d\n",
			       __func__, rc);
			return;
		}
		msm_batt_info.suspend_status = 0;
	} else {
		pr_err("%s: ERROR. invalid batt_handle\n", __func__);
		return;
	}

	pr_debug("%s: exit\n", __func__);
}
#endif

struct msm_batt_vbatt_filter_req {
	u32 batt_handle;
	u32 enable_filter;
	u32 vbatt_filter;
};

struct msm_batt_vbatt_filter_rep {
	u32 result;
};

static int msm_batt_filter_arg_func(struct msm_rpc_client *batt_client,

		void *buf, void *data)
{
	struct msm_batt_vbatt_filter_req *vbatt_filter_req =
		(struct msm_batt_vbatt_filter_req *)data;
	u32 *req = (u32 *)buf;
	int size = 0;

	*req = cpu_to_be32(vbatt_filter_req->batt_handle);
	size += sizeof(u32);
	req++;

	*req = cpu_to_be32(vbatt_filter_req->enable_filter);
	size += sizeof(u32);
	req++;

	*req = cpu_to_be32(vbatt_filter_req->vbatt_filter);
	size += sizeof(u32);
	return size;
}

static int msm_batt_filter_ret_func(struct msm_rpc_client *batt_client,
				       void *buf, void *data)
{

	struct msm_batt_vbatt_filter_rep *data_ptr, *buf_ptr;

	data_ptr = (struct msm_batt_vbatt_filter_rep *)data;
	buf_ptr = (struct msm_batt_vbatt_filter_rep *)buf;

	data_ptr->result = be32_to_cpu(buf_ptr->result);
	return 0;
}

static int msm_batt_enable_filter(u32 vbatt_filter)
{
	int rc;
	struct  msm_batt_vbatt_filter_req  vbatt_filter_req;
	struct  msm_batt_vbatt_filter_rep  vbatt_filter_rep;

	vbatt_filter_req.batt_handle = msm_batt_info.batt_handle;
	vbatt_filter_req.enable_filter = 1;
	vbatt_filter_req.vbatt_filter = vbatt_filter;

	rc = msm_rpc_client_req(msm_batt_info.batt_client,
			BATTERY_ENABLE_DISABLE_FILTER_PROC,
			msm_batt_filter_arg_func, &vbatt_filter_req,
			msm_batt_filter_ret_func, &vbatt_filter_rep,
			msecs_to_jiffies(BATT_RPC_TIMEOUT));

	if (rc < 0) {
		pr_err("%s: FAIL: enable vbatt filter. rc=%d\n",
		       __func__, rc);
		return rc;
	}

	if (vbatt_filter_rep.result != BATTERY_DEREGISTRATION_SUCCESSFUL) {
		pr_err("%s: FAIL: enable vbatt filter: result=%d\n",
		       __func__, vbatt_filter_rep.result);
		return -EIO;
	}

	pr_debug("%s: enable vbatt filter: OK\n", __func__);
	return rc;
}

struct batt_client_registration_req {
	/* The voltage at which callback (CB) should be called. */
	u32 desired_batt_voltage;

	/* The direction when the CB should be called. */
	u32 voltage_direction;

	/* The registered callback to be called when voltage and
	 * direction specs are met. */
	u32 batt_cb_id;

	/* The call back data */
	u32 cb_data;
	u32 more_data;
	u32 batt_error;
};

struct batt_client_registration_req_4_1 {
	/* The voltage at which callback (CB) should be called. */
	u32 desired_batt_voltage;

	/* The direction when the CB should be called. */
	u32 voltage_direction;

	/* The registered callback to be called when voltage and
	 * direction specs are met. */
	u32 batt_cb_id;

	/* The call back data */
	u32 cb_data;
	u32 batt_error;
};

struct batt_client_registration_rep {
	u32 batt_handle;
};

struct batt_client_registration_rep_4_1 {
	u32 batt_handle;
	u32 more_data;
	u32 err;
};

static int msm_batt_register_arg_func(struct msm_rpc_client *batt_client,
				       void *buf, void *data)
{
	struct batt_client_registration_req *batt_reg_req =
		(struct batt_client_registration_req *)data;

	u32 *req = (u32 *)buf;
	int size = 0;


	if (msm_batt_info.batt_api_version == BATTERY_RPC_VER_4_1) {
		*req = cpu_to_be32(batt_reg_req->desired_batt_voltage);
		size += sizeof(u32);
		req++;

		*req = cpu_to_be32(batt_reg_req->voltage_direction);
		size += sizeof(u32);
		req++;

		*req = cpu_to_be32(batt_reg_req->batt_cb_id);
		size += sizeof(u32);
		req++;

		*req = cpu_to_be32(batt_reg_req->cb_data);
		size += sizeof(u32);
		req++;

		*req = cpu_to_be32(batt_reg_req->batt_error);
		size += sizeof(u32);

		return size;
	} else {
	*req = cpu_to_be32(batt_reg_req->desired_batt_voltage);
	size += sizeof(u32);
	req++;

	*req = cpu_to_be32(batt_reg_req->voltage_direction);
	size += sizeof(u32);
	req++;

	*req = cpu_to_be32(batt_reg_req->batt_cb_id);
	size += sizeof(u32);
	req++;

	*req = cpu_to_be32(batt_reg_req->cb_data);
	size += sizeof(u32);
	req++;

	*req = cpu_to_be32(batt_reg_req->more_data);
	size += sizeof(u32);
	req++;

	*req = cpu_to_be32(batt_reg_req->batt_error);
	size += sizeof(u32);

	return size;
	}

}

static int msm_batt_register_ret_func(struct msm_rpc_client *batt_client,
				       void *buf, void *data)
{
	struct batt_client_registration_rep *data_ptr, *buf_ptr;
	struct batt_client_registration_rep_4_1 *data_ptr_4_1, *buf_ptr_4_1;

	if (msm_batt_info.batt_api_version == BATTERY_RPC_VER_4_1) {
		data_ptr_4_1 = (struct batt_client_registration_rep_4_1 *)data;
		buf_ptr_4_1 = (struct batt_client_registration_rep_4_1 *)buf;

		data_ptr_4_1->batt_handle
			= be32_to_cpu(buf_ptr_4_1->batt_handle);
		data_ptr_4_1->more_data
			= be32_to_cpu(buf_ptr_4_1->more_data);
		data_ptr_4_1->err = be32_to_cpu(buf_ptr_4_1->err);
		return 0;
	} else {
	data_ptr = (struct batt_client_registration_rep *)data;
	buf_ptr = (struct batt_client_registration_rep *)buf;

	data_ptr->batt_handle = be32_to_cpu(buf_ptr->batt_handle);

	return 0;
}
}

static int msm_batt_register(u32 desired_batt_voltage,
			     u32 voltage_direction, u32 batt_cb_id, u32 cb_data)
{
	struct batt_client_registration_req batt_reg_req;
	struct batt_client_registration_req_4_1 batt_reg_req_4_1;
	struct batt_client_registration_rep batt_reg_rep;
	struct batt_client_registration_rep_4_1 batt_reg_rep_4_1;
	void *request;
	void *reply;
	int rc;

	if (msm_batt_info.batt_api_version == BATTERY_RPC_VER_4_1) {
		batt_reg_req_4_1.desired_batt_voltage = desired_batt_voltage;
		batt_reg_req_4_1.voltage_direction = voltage_direction;
		batt_reg_req_4_1.batt_cb_id = batt_cb_id;
		batt_reg_req_4_1.cb_data = cb_data;
		batt_reg_req_4_1.batt_error = 1;
		request = &batt_reg_req_4_1;
	} else {
	batt_reg_req.desired_batt_voltage = desired_batt_voltage;
	batt_reg_req.voltage_direction = voltage_direction;
	batt_reg_req.batt_cb_id = batt_cb_id;
	batt_reg_req.cb_data = cb_data;
	batt_reg_req.more_data = 1;
	batt_reg_req.batt_error = 0;
		request = &batt_reg_req;
	}

	if (msm_batt_info.batt_api_version == BATTERY_RPC_VER_4_1)
		reply = &batt_reg_rep_4_1;
	else
		reply = &batt_reg_rep;

	rc = msm_rpc_client_req(msm_batt_info.batt_client,
			BATTERY_REGISTER_PROC,
			msm_batt_register_arg_func, request,
			msm_batt_register_ret_func, reply,
			msecs_to_jiffies(BATT_RPC_TIMEOUT));

	if (rc < 0) {
		pr_err("%s: FAIL: vbatt register. rc=%d\n", __func__, rc);
		return rc;
	}

	if (msm_batt_info.batt_api_version == BATTERY_RPC_VER_4_1) {
		if (batt_reg_rep_4_1.more_data != 0
			&& batt_reg_rep_4_1.err
				!= BATTERY_REGISTRATION_SUCCESSFUL) {
			pr_err("%s: vBatt Registration Failed proc_num=%d\n"
					, __func__, BATTERY_REGISTER_PROC);
			return -EIO;
		}
		msm_batt_info.batt_handle = batt_reg_rep_4_1.batt_handle;
	} else {
		msm_batt_info.batt_handle = batt_reg_rep.batt_handle;
	}

	pr_debug("%s: got handle = %d\n", __func__, msm_batt_info.batt_handle);

	return 0;
}

struct batt_client_deregister_req {
	u32 batt_handle;
};

struct batt_client_deregister_rep {
	u32 batt_error;
};

static int msm_batt_deregister_arg_func(struct msm_rpc_client *batt_client,
				       void *buf, void *data)
{
	struct batt_client_deregister_req *deregister_req =
		(struct  batt_client_deregister_req *)data;
	u32 *req = (u32 *)buf;
	int size = 0;

	*req = cpu_to_be32(deregister_req->batt_handle);
	size += sizeof(u32);

	return size;
}

static int msm_batt_deregister_ret_func(struct msm_rpc_client *batt_client,
				       void *buf, void *data)
{
	struct batt_client_deregister_rep *data_ptr, *buf_ptr;

	data_ptr = (struct batt_client_deregister_rep *)data;
	buf_ptr = (struct batt_client_deregister_rep *)buf;

	data_ptr->batt_error = be32_to_cpu(buf_ptr->batt_error);

	return 0;
}

static int msm_batt_deregister(u32 batt_handle)
{
	int rc;
	struct batt_client_deregister_req req;
	struct batt_client_deregister_rep rep;

	req.batt_handle = batt_handle;

	rc = msm_rpc_client_req(msm_batt_info.batt_client,
			BATTERY_DEREGISTER_CLIENT_PROC,
			msm_batt_deregister_arg_func, &req,
			msm_batt_deregister_ret_func, &rep,
			msecs_to_jiffies(BATT_RPC_TIMEOUT));

	if (rc < 0) {
		pr_err("%s: FAIL: vbatt deregister. rc=%d\n", __func__, rc);
		return rc;
	}

	if (rep.batt_error != BATTERY_DEREGISTRATION_SUCCESSFUL) {
		pr_err("%s: vbatt deregistration FAIL. error=%d, handle=%d\n",
		       __func__, rep.batt_error, batt_handle);
		return -EIO;
	}

	return 0;
}

#ifdef CONFIG_PM
static int msm_batt_suspend(struct platform_device *pdev,
		pm_message_t state)
{
	pr_debug("[BATT] %s\n", __func__);
	if (msm_batt_info.batt_capacity < 10) {
		msm_batt_set_alarm(ALARM_POLLING_TIME_SHORT);
		pr_info("%s[BATT] set alarm for long time\n", __func__);
	} else {
		msm_batt_set_alarm(ALARM_POLLING_TIME_LONG);
		pr_info("%s[BATT] set alarm for shor time\n", __func__);
	}
	del_timer_sync(&msm_batt_info.timer);

	return 0;
}

static int msm_batt_resume(struct platform_device *pdev)
{
	pr_debug("[BATT] %s\n", __func__);

	pr_info("[BATT] set alarm for resume\n");

	queue_work(msm_batt_info.msm_batt_wq, &msm_batt_work);
	mod_timer(&msm_batt_info.timer, (jiffies + BATT_CHECK_INTERVAL));

	return 0;
}
#else
#define msm_batt_suspend NULL
#define msm_batt_resume NULL
#endif

int batt_restart(void)
{
	if (msm_batt_driver_init) {
		del_timer_sync(&msm_batt_info.timer);
		queue_work(msm_batt_info.msm_batt_wq, &msm_batt_work);
		mod_timer(&msm_batt_info.timer,
			(jiffies + BATT_CHECK_INTERVAL));
	} else {
		pr_err("[BATT] %s: Battery driver is not ready !!\n",
				__func__);
		msm_batt_unhandled_interrupt = 1;
	}

	pr_info("[BATT] %s: Bat restart is complite\n", __func__);

	return 0;
}
EXPORT_SYMBOL(batt_restart);

/*End from Bat driver */
static int msm_batt_cleanup(void)
{
	int rc = 0;

	pr_info("[BATT] %s\n", __func__);

	if (msm_batt_info.pdata->register_callbacks)
		msm_batt_info.pdata->register_callbacks(NULL);

	alarm_cancel(&msm_batt_info.alarm);
	del_timer_sync(&msm_batt_info.timer);
	destroy_workqueue(msm_batt_cable_wq);
	destroy_workqueue(msm_batt_info.msm_batt_wq);
	msm_batt_remove_attrs(msm_psy_batt.dev);

	if (msm_batt_info.batt_handle != INVALID_BATT_HANDLE) {

		rc = msm_batt_deregister(msm_batt_info.batt_handle);
		if (rc < 0)
			pr_err("%s: FAIL: msm_batt_deregister. rc=%d\n",
			       __func__, rc);
	}

	msm_batt_info.batt_handle = INVALID_BATT_HANDLE;

	if (msm_batt_info.batt_client)
		msm_rpc_unregister_client(msm_batt_info.batt_client);

	if (msm_batt_info.msm_psy_ac)
		power_supply_unregister(msm_batt_info.msm_psy_ac);

	if (msm_batt_info.msm_psy_usb)
		power_supply_unregister(msm_batt_info.msm_psy_usb);
	if (msm_batt_info.msm_psy_batt)
		power_supply_unregister(msm_batt_info.msm_psy_batt);

	if (msm_batt_info.chg_ep) {
		rc = msm_rpc_close(msm_batt_info.chg_ep);
		if (rc < 0) {
			pr_err("%s: FAIL. msm_rpc_close(chg_ep). rc=%d\n",
			       __func__, rc);
		}
	}

#ifdef CONFIG_HAS_EARLYSUSPEND
	if (msm_batt_info.early_suspend.suspend == msm_batt_early_suspend)
		unregister_early_suspend(&msm_batt_info.early_suspend);
#endif
	return rc;
}

int msm_batt_get_charger_api_version(void)
{
	int rc ;
	struct rpc_reply_hdr *reply;

	struct rpc_req_chg_api_ver {
		struct rpc_request_hdr hdr;
		u32 more_data;
	} req_chg_api_ver;

	struct rpc_rep_chg_api_ver {
		struct rpc_reply_hdr hdr;
		u32 num_of_chg_api_versions;
		u32 *chg_api_versions;
	};

	u32 num_of_versions;

	struct rpc_rep_chg_api_ver *rep_chg_api_ver;


	req_chg_api_ver.more_data = cpu_to_be32(1);

	msm_rpc_setup_req(&req_chg_api_ver.hdr, CHG_RPC_PROG, CHG_RPC_VER_1_1,
			  ONCRPC_CHARGER_API_VERSIONS_PROC);

	rc = msm_rpc_write(msm_batt_info.chg_ep, &req_chg_api_ver,
			sizeof(req_chg_api_ver));
	if (rc < 0) {
		pr_err("%s: FAIL: msm_rpc_write. proc=0x%08x, rc=%d\n",
		       __func__, ONCRPC_CHARGER_API_VERSIONS_PROC, rc);
		return rc;
	}

	for (;;) {
		rc = msm_rpc_read(msm_batt_info.chg_ep, (void *) &reply, -1,
				BATT_RPC_TIMEOUT);
		if (rc < 0)
			return rc;
		if (rc < RPC_REQ_REPLY_COMMON_HEADER_SIZE) {
			pr_err("%s: LENGTH ERR: msm_rpc_read. rc=%d (<%d)\n",
			       __func__, rc, RPC_REQ_REPLY_COMMON_HEADER_SIZE);

			rc = -EIO;
			break;
		}
		/* we should not get RPC REQ or call packets -- ignore them */
		if (reply->type == RPC_TYPE_REQ) {
			pr_err("%s: TYPE ERR: type=%d (!=%d)\n",
			       __func__, reply->type, RPC_TYPE_REQ);
			kfree(reply);
			continue;
		}

		/* If an earlier call timed out, we could get the (no
		 * longer wanted) reply for it.	 Ignore replies that
		 * we don't expect
		 */
		if (reply->xid != req_chg_api_ver.hdr.xid) {
			pr_err("%s: XID ERR: xid=%d (!=%d)\n", __func__,
			       reply->xid, req_chg_api_ver.hdr.xid);
			kfree(reply);
			continue;
		}
		if (reply->reply_stat != RPCMSG_REPLYSTAT_ACCEPTED) {
			rc = -EPERM;
			break;
		}
		if (reply->data.acc_hdr.accept_stat !=
				RPC_ACCEPTSTAT_SUCCESS) {
			rc = -EINVAL;
			break;
		}

		rep_chg_api_ver = (struct rpc_rep_chg_api_ver *)reply;

		num_of_versions =
			be32_to_cpu(rep_chg_api_ver->num_of_chg_api_versions);

		rep_chg_api_ver->chg_api_versions =  (u32 *)
			((u8 *) reply + sizeof(struct rpc_reply_hdr) +
			sizeof(rep_chg_api_ver->num_of_chg_api_versions));

		rc = be32_to_cpu(
			rep_chg_api_ver->chg_api_versions[num_of_versions - 1]);

		pr_debug("%s: num_of_chg_api_versions = %u. "
			"The chg api version = 0x%08x\n", __func__,
			num_of_versions, rc);
		break;
	}
	kfree(reply);
	return rc;
}

static int msm_batt_cb_func(struct msm_rpc_client *client,
			    void *buffer, int in_size)
{
	int rc = 0;
	struct rpc_request_hdr *req;
	u32 procedure;
	u32 accept_status;

	req = (struct rpc_request_hdr *)buffer;
	procedure = be32_to_cpu(req->procedure);

	switch (procedure) {
	case BATTERY_CB_TYPE_PROC:
		accept_status = RPC_ACCEPTSTAT_SUCCESS;
		break;

	default:
		accept_status = RPC_ACCEPTSTAT_PROC_UNAVAIL;
		pr_err("%s: ERROR. procedure (%d) not supported\n",
		       __func__, procedure);
		break;
	}

	msm_rpc_start_accepted_reply(msm_batt_info.batt_client,
			be32_to_cpu(req->xid), accept_status);

	rc = msm_rpc_send_accepted_reply(msm_batt_info.batt_client, 0);
	if (rc)
		pr_err("%s: FAIL: sending reply. rc=%d\n", __func__, rc);

	if (accept_status == RPC_ACCEPTSTAT_SUCCESS && set_timer == 1) {
		pr_info("[BATT] %s: RPC_ACCEPTSTAT_SUCCESS !!\n", __func__);
		del_timer_sync(&msm_batt_info.timer);
		queue_work(msm_batt_info.msm_batt_wq, &msm_batt_work);
		mod_timer(&msm_batt_info.timer,
			(jiffies + BATT_CHECK_INTERVAL));
	}

	return rc;
}

static void msm_batt_set_charging_start_time(chg_enable_type enable)
{
	if (enable == START_CHARGING)
		charging_start_time = jiffies;
	else
		charging_start_time = 0;
}

static int msm_batt_is_over_abs_time(void)
{
	unsigned int total_time;

	if (charging_start_time == 0)
		return 0;

	if (msm_batt_info.batt_full_check == 1)
		total_time = TOTAL_RECHARGING_TIME;
	else
		total_time = TOTAL_CHARGING_TIME;

	if (time_after((unsigned long)jiffies,
		(unsigned long)(charging_start_time + total_time))) {
		pr_debug("[BATT] %s: abs time is over !!\n", __func__);
		return 1;
	} else {
		return 0;
	}
}

static int __devinit msm_batt_probe(struct platform_device *pdev)
{
	int rc;
	struct msm_psy_batt_pdata *pdata = pdev->dev.platform_data;

	msm_batt_info.pdata = pdata->charger;

	if (pdev->id != -1) {
		dev_err(&pdev->dev,
			"%s: MSM chipsets Can only support one"
			" battery ", __func__);
		return -EINVAL;
	}

	msm_batt_info.batt_slate_mode = 0;

	if (pdata->avail_chg_sources & AC_CHG) {
		rc = power_supply_register(&pdev->dev, &msm_psy_ac);
		if (rc < 0) {
			dev_err(&pdev->dev,
				"%s: power_supply_register failed"
				" rc = %d\n", __func__, rc);
			msm_batt_cleanup();
			return rc;
		}
		msm_batt_info.msm_psy_ac = &msm_psy_ac;
		msm_batt_info.avail_chg_sources |= AC_CHG;
	}

	if (pdata->avail_chg_sources & USB_CHG) {
		rc = power_supply_register(&pdev->dev, &msm_psy_usb);
		if (rc < 0) {
			dev_err(&pdev->dev,
				"%s: power_supply_register failed"
				" rc = %d\n", __func__, rc);
			msm_batt_cleanup();
			return rc;
		}
		msm_batt_info.msm_psy_usb = &msm_psy_usb;
		msm_batt_info.avail_chg_sources |= USB_CHG;
	}

	if (!msm_batt_info.msm_psy_ac && !msm_batt_info.msm_psy_usb) {

		dev_err(&pdev->dev,
			"%s: No external Power supply(AC or USB)"
			"is avilable\n", __func__);
		msm_batt_cleanup();
		return -ENODEV;
	}

	msm_batt_info.voltage_max_design = pdata->voltage_max_design;
	msm_batt_info.voltage_min_design = pdata->voltage_min_design;
	msm_batt_info.batt_technology = pdata->batt_technology;

	if (!msm_batt_info.voltage_min_design)
		msm_batt_info.voltage_min_design = BATTERY_LOW;
	if (!msm_batt_info.voltage_max_design)
		msm_batt_info.voltage_max_design = BATTERY_HIGH;
	if (msm_batt_info.batt_technology == POWER_SUPPLY_TECHNOLOGY_UNKNOWN)
		msm_batt_info.batt_technology = POWER_SUPPLY_TECHNOLOGY_LION;

	rc = power_supply_register(&pdev->dev, &msm_psy_batt);
	if (rc < 0) {
		dev_err(&pdev->dev, "%s: power_supply_register failed"
			" rc=%d\n", __func__, rc);
		msm_batt_cleanup();
		return rc;
	}
	msm_batt_info.msm_psy_batt = &msm_psy_batt;

	rc = msm_batt_register(BATTERY_LOW, BATTERY_ALL_ACTIVITY,
			       BATTERY_CB_ID_ALL_ACTIV, BATTERY_ALL_ACTIVITY);
	if (rc < 0) {
		dev_err(&pdev->dev,
			"%s: msm_batt_register failed rc = %d\n", __func__, rc);
		msm_batt_cleanup();
		return rc;
	}

	rc =  msm_batt_enable_filter(VBATT_FILTER);

	if (rc < 0) {
		dev_err(&pdev->dev,
			"%s: msm_batt_enable_filter failed rc = %d\n",
			__func__, rc);
		msm_batt_cleanup();
		return rc;
	}

	msm_batt_create_attrs(msm_psy_batt.dev);

#ifdef CONFIG_HAS_EARLYSUSPEND
	msm_batt_info.early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN;
	msm_batt_info.early_suspend.suspend = msm_batt_early_suspend;
	msm_batt_info.early_suspend.resume = msm_batt_late_resume;
	register_early_suspend(&msm_batt_info.early_suspend);
#endif

	setup_timer(&msm_batt_info.timer, batt_timeover, 0);
	/*mod_timer(&msm_batt_info.timer, (jiffies + BATT_CHECK_INTERVAL));*/
#ifdef CONFIG_MACH_GEIM
	setup_timer(&msm_batt_info.bat_use_timer, msm_bat_use_timer_func, 0);
#endif /*CONFIG_MACH_GEIM*/
	wake_lock_init(&vbus_wake_lock, WAKE_LOCK_SUSPEND, "vbus_wake_lock");
	wake_lock_init(&fuel_alert_wake_lock,
			WAKE_LOCK_SUSPEND, "fuel_alert_wake_lock");

	if (charging_boot == 1) {
		wake_lock_init(&lpm_wake_lock, WAKE_LOCK_SUSPEND,
			"lpm_wake_lock");
		wake_lock(&lpm_wake_lock);
	}

#ifdef CONFIG_BQ27425_FUEL_GAUGE
	if (is_attached) {
		msm_batt_info.batt_capacity = get_level_from_fuelgauge();
		msm_batt_info.battery_vol = get_voltage_from_fuelgauge();
		msm_batt_info.batt_voltage_now =
			msm_batt_info.battery_vol * 1000;
		pr_info("[BATTERY] get bat leel and voltage from fuelgauge\n");
	}
#endif
	msm_batt_info.callback.set_cable = msm_set_cable;
	msm_batt_info.callback.set_acc_type = msm_set_acc_type;
	msm_batt_info.callback.set_ovp_type = msm_set_ovp_type;

	if (msm_batt_info.pdata->register_callbacks)
		msm_batt_info.pdata->register_callbacks
		(&msm_batt_info.callback);


	/* sys fs */
	jig_class = class_create(THIS_MODULE, "jig");
	if (IS_ERR(jig_class))
		pr_err("Failed to create class(jig)!\n");

	jig_dev = device_create(jig_class, NULL, 0, NULL, "jig");
	if (IS_ERR(jig_dev))
		pr_err("Failed to create device(jig)!\n");

	if (device_create_file(jig_dev, &dev_attr_jig) < 0)
		pr_err("Failed to create device file(%s)!\n",
			dev_attr_jig.attr.name);
	/* sys fs */
	msm_batt_driver_init = 1;
	set_timer = 1;
	msm_batt_info.resume_flag = 0;
	msm_batt_info.boot_flag = 1;
	msm_batt_info.batt_use = 0;

	/* block bellow until fuel guage modified */
#if 0
	if (!rc && msm_batt_info.batt_capacity < 4) {
		wake_lock(&fuel_alert_wake_lock);
		pr_info("[BATT] start_wake_lock for fuelgague\n");
		fuel_alert_det = 1;
	} else {
		fuel_alert_det = 0;
	}
#endif

	pr_debug("[BATT] %s : success!\n", __func__);
	power_supply_changed(&msm_psy_ac);
	power_supply_changed(&msm_psy_usb);

	alarm_init(&msm_batt_info.alarm,
			ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP,
			msm_batt_alarm_manager);
		queue_work(msm_batt_info.msm_batt_wq,
			&msm_batt_work);
		mod_timer(&msm_batt_info.timer,
			(jiffies + BATT_CHECK_INTERVAL));

	return 0;
}

static int __devexit msm_batt_remove(struct platform_device *pdev)
{
	int rc;
	pr_info("%s\n", __func__);
	rc = msm_batt_cleanup();

	wake_lock_destroy(&fuel_alert_wake_lock);
	wake_lock_destroy(&vbus_wake_lock);

	if (charging_boot)
		wake_lock_destroy(&lpm_wake_lock);

	if (rc < 0) {
		dev_err(&pdev->dev,
			"%s: msm_batt_cleanup  failed rc=%d\n", __func__, rc);
		return rc;
	}
	return 0;
}

static void msm_batt_shutdown(struct platform_device *pdev)
{
	int rc;
	pr_info("%s\n", __func__);
	rc = msm_batt_cleanup();

	if (rc < 0) {
		dev_err(&pdev->dev,
			"%s: msm_batt_cleanup  failed rc=%d\n", __func__, rc);
	}

}


static const struct dev_pm_ops msm_batt_pm_ops = {
	.prepare = msm_batt_suspend,
	.complete = msm_batt_resume,
};

static struct platform_driver msm_batt_driver = {
	.probe = msm_batt_probe,
	.remove = __devexit_p(msm_batt_remove),
	.shutdown = msm_batt_shutdown,
	.driver = {
			.name = "msm-battery",
			.pm = &msm_batt_pm_ops,
			},
};

static int __devinit msm_batt_init_rpc(void)
{
	int rc;

	msm_batt_info.msm_batt_wq =
		create_singlethread_workqueue("msm_battery");
	msm_batt_cable_wq =
		create_singlethread_workqueue("msm_bat_setcable");

	if (!msm_batt_info.msm_batt_wq) {
		printk(KERN_ERR "%s: create workque failed\n", __func__);
		return -ENOMEM;
	}

	msm_batt_info.chg_ep =
		msm_rpc_connect_compatible(CHG_RPC_PROG, CHG_RPC_VER_4_1, 0);
	msm_batt_info.chg_api_version =  CHG_RPC_VER_4_1;
	if (msm_batt_info.chg_ep == NULL) {
		pr_err("%s: rpc connect CHG_RPC_PROG = NULL\n", __func__);
		return -ENODEV;
	}

	if (IS_ERR(msm_batt_info.chg_ep)) {
		msm_batt_info.chg_ep = msm_rpc_connect_compatible(
				CHG_RPC_PROG, CHG_RPC_VER_3_1, 0);
		msm_batt_info.chg_api_version =  CHG_RPC_VER_3_1;
	}
	if (IS_ERR(msm_batt_info.chg_ep)) {
		msm_batt_info.chg_ep = msm_rpc_connect_compatible(
				CHG_RPC_PROG, CHG_RPC_VER_1_1, 0);
		msm_batt_info.chg_api_version =  CHG_RPC_VER_1_1;
	}
	if (IS_ERR(msm_batt_info.chg_ep)) {
		msm_batt_info.chg_ep = msm_rpc_connect_compatible(
				CHG_RPC_PROG, CHG_RPC_VER_1_3, 0);
		msm_batt_info.chg_api_version =  CHG_RPC_VER_1_3;
	}
	if (IS_ERR(msm_batt_info.chg_ep)) {
		msm_batt_info.chg_ep = msm_rpc_connect_compatible(
				CHG_RPC_PROG, CHG_RPC_VER_2_2, 0);
		msm_batt_info.chg_api_version =  CHG_RPC_VER_2_2;
	}
	if (IS_ERR(msm_batt_info.chg_ep)) {
		rc = PTR_ERR(msm_batt_info.chg_ep);
		pr_err("%s: FAIL: rpc connect for CHG_RPC_PROG. rc=%d\n",
		       __func__, rc);
		msm_batt_info.chg_ep = NULL;
		return rc;
	}

	/* Get the real 1.x version */
	if (msm_batt_info.chg_api_version == CHG_RPC_VER_1_1)
		msm_batt_info.chg_api_version =
			msm_batt_get_charger_api_version();

	/* Fall back to 1.1 for default */
	if (msm_batt_info.chg_api_version < 0)
		msm_batt_info.chg_api_version = CHG_RPC_VER_1_1;
	msm_batt_info.batt_api_version =  BATTERY_RPC_VER_4_1;

	msm_batt_info.batt_client =
		msm_rpc_register_client("battery", BATTERY_RPC_PROG,
					BATTERY_RPC_VER_4_1,
					1, msm_batt_cb_func);

	if (msm_batt_info.batt_client == NULL) {
		pr_err("%s: FAIL: rpc_register_client. batt_client=NULL\n",
		       __func__);
		return -ENODEV;
	}
	if (IS_ERR(msm_batt_info.batt_client)) {
		msm_batt_info.batt_client =
			msm_rpc_register_client("battery", BATTERY_RPC_PROG,
						BATTERY_RPC_VER_1_1,
						1, msm_batt_cb_func);
		msm_batt_info.batt_api_version =  BATTERY_RPC_VER_1_1;
	}
	if (IS_ERR(msm_batt_info.batt_client)) {
		msm_batt_info.batt_client =
			msm_rpc_register_client("battery", BATTERY_RPC_PROG,
						BATTERY_RPC_VER_2_1,
						1, msm_batt_cb_func);
		msm_batt_info.batt_api_version =  BATTERY_RPC_VER_2_1;
	}
	if (IS_ERR(msm_batt_info.batt_client)) {
		msm_batt_info.batt_client =
			msm_rpc_register_client("battery", BATTERY_RPC_PROG,
						BATTERY_RPC_VER_5_1,
						1, msm_batt_cb_func);
		msm_batt_info.batt_api_version =  BATTERY_RPC_VER_5_1;
	}
	if (IS_ERR(msm_batt_info.batt_client)) {
		rc = PTR_ERR(msm_batt_info.batt_client);
		pr_err("%s: ERROR: rpc_register_client: rc = %d\n ",
		       __func__, rc);
		msm_batt_info.batt_client = NULL;
		return rc;
	}

	rc = platform_driver_register(&msm_batt_driver);

	if (rc < 0)
		pr_err("%s: FAIL: platform_driver_register. rc = %d\n",
		       __func__, rc);

	return rc;
}

static int __init msm_batt_init(void)
{
	int rc;

	pr_debug("%s: enter\n", __func__);

	rc = msm_batt_init_rpc();

	if (rc < 0) {
		pr_err("%s: FAIL: msm_batt_init_rpc.  rc=%d\n", __func__, rc);
		msm_batt_cleanup();
		return rc;
	}

	pr_info("%s: Charger/Battery = 0x%08x/0x%08x (RPC version)\n",
		__func__, msm_batt_info.chg_api_version,
		msm_batt_info.batt_api_version);

	return 0;
}

static void __exit msm_batt_exit(void)
{
	platform_driver_unregister(&msm_batt_driver);
}

late_initcall(msm_batt_init);
module_exit(msm_batt_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Kiran Kandi, Qualcomm Innovation Center, Inc.");
MODULE_DESCRIPTION("Battery driver for Qualcomm MSM chipsets.");
MODULE_VERSION("1.0");
MODULE_ALIAS("platform:msm_battery");
