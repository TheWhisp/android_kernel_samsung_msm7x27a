/* arch/arm/mach-msm/vreg.c
 *
 * Copyright (C) 2008 Google, Inc.
 * Copyright (c) 2009-2011 Code Aurora Forum. All rights reserved.
 * Author: Brian Swetland <swetland@google.com>
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

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/string.h>
#include <mach/vreg.h>

#include "proc_comm.h"

#if defined(CONFIG_MSM_VREG_SWITCH_INVERTED)
#define VREG_SWITCH_ENABLE 0
#define VREG_SWITCH_DISABLE 1
#else
#define VREG_SWITCH_ENABLE 1
#define VREG_SWITCH_DISABLE 0
#endif

struct vreg {
	const char *name;
	unsigned id;
	int status;
	unsigned refcnt;
	unsigned int initstate;
	unsigned int always_on;
};

#define VREG(_name, _id, _status, _refcnt) \
	{ .name = _name, .id = _id, .status = _status, .refcnt = _refcnt }

static struct vreg vregs[] = {
	VREG("msma",	0, 0, 0),
	VREG("msmp",	1, 0, 0),
	VREG("msme1",	2, 0, 0),
	VREG("msmc1",	3, 0, 0),
	VREG("msmc2",	4, 0, 0),
	VREG("gp3",	5, 0, 0),
	VREG("msme2",	6, 0, 0),
	VREG("gp4",	7, 0, 0),
	VREG("gp1",	8, 0, 0),
	VREG("tcxo",	9, 0, 0),
	VREG("pa",	10, 0, 0),
	VREG("rftx",	11, 0, 0),
	VREG("rfrx1",	12, 0, 0),
	VREG("rfrx2",	13, 0, 0),
	VREG("synt",	14, 0, 0),
	VREG("wlan",	15, 0, 0),
	VREG("usb",	16, 0, 0),
	VREG("boost",	17, 0, 0),
	VREG("mmc",	18, 0, 0),
	VREG("ruim",	19, 0, 0),
	VREG("msmc0",	20, 0, 0),
	VREG("gp2",	21, 0, 0),
	VREG("gp5",	22, 0, 0),
	VREG("gp6",	23, 0, 0),
	VREG("rf",	24, 0, 0),
	VREG("rf_vco",	26, 0, 0),
	VREG("mpll",	27, 0, 0),
	VREG("s2",	28, 0, 0),
	VREG("s3",	29, 0, 0),
	VREG("rfubm",	30, 0, 0),
	VREG("ncp",	31, 0, 0),
	VREG("gp7",	32, 0, 0),
	VREG("gp8",	33, 0, 0),
	VREG("gp9",	34, 0, 0),
	VREG("gp10",	35, 0, 0),
	VREG("gp11",	36, 0, 0),
	VREG("gp12",	37, 0, 0),
	VREG("gp13",	38, 0, 0),
	VREG("gp14",	39, 0, 0),
	VREG("gp15",	40, 0, 0),
	VREG("gp16",	41, 0, 0),
	VREG("gp17",	42, 0, 0),
	VREG("s4",	43, 0, 0),
	VREG("usb2",	44, 0, 0),
	VREG("wlan2",	45, 0, 0),
	VREG("xo_out",	46, 0, 0),
	VREG("lvsw0",	47, 0, 0),
	VREG("lvsw1",	48, 0, 0),
	VREG("mddi",	49, 0, 0),
	VREG("pllx",	50, 0, 0),
	VREG("wlan3",	51, 0, 0),
	VREG("emmc",	52, 0,	0),
	VREG("wlan_tcx0", 53, 0, 0),
	VREG("usim2",	54, 0, 0),
	VREG("usim",	55, 0, 0),
	VREG("bt",	56, 0, 0),
	VREG("wlan4",	57, 0, 0),
};

struct msm7x2xa_vreg {
	const char *circuit_name;
	const char *vreg_name;
	unsigned int voltage;
	const char *ldo;
	struct vreg *vreg;
	unsigned int initstate;
	unsigned int always_on;
};

static struct msm7x2xa_vreg jena_rev01_vreg[] = {
	[0] = {
		.circuit_name = "vreg_msmc1",
		.vreg_name = "msmc1",
		.initstate = 1,
		.voltage = 1100,
		.ldo = "VSW1",
		.always_on = 0,
	},
	[1] = {
		.circuit_name = "vreg_msmc2",
		.vreg_name = "msmc2",
		.initstate = 1,
		.voltage = 1100,
		.ldo = "VSW2",
		.always_on = 0,
	},
	[2] = {
		.circuit_name = "vreg_msme",
		.vreg_name = "msme1",
		.initstate = 1,
		.voltage = 1800,
		.ldo = "VSW3",
		.always_on = 1,
	},
	[3] = {
		.circuit_name = "vreg_pa",
		.vreg_name = "rf",
		.initstate = 1,
		.voltage = 2100,
		.ldo = "VSW4",
		.always_on = 0,
	},
	[4] = {
		.circuit_name = "vreg_vib",
		.vreg_name = "rfrx1",
		.initstate = 0,
		.voltage = 3300,
		.ldo = "L1",
		.always_on = 0,
	},
	[5] = {
		.circuit_name = "vreg_rf",
		.vreg_name = "rfrx2",
		.initstate = 1,
		.voltage = 2100,
		.ldo = "L2",
		.always_on = 0,
	},
	[6] = {
		.circuit_name = "vreg_msm_dsi",
		.vreg_name = "mddi",
		.initstate = 1,
		.voltage = 1200,
		.ldo = "L3",
		.always_on = 0,
	},
	[7] = {
		.circuit_name = "vreg_msm_pll",
		.vreg_name = "pllx",
		.initstate = 1,
		.voltage = 1100,
		.ldo = "L4",
		.always_on = 0,
	},
	[8] = {
		.circuit_name = "not_used",
		.vreg_name = "wlan2",
		.initstate = 0,
		.voltage = 0,
		.ldo = "L5",
		.always_on = 0,
	},
	[9] = {
		.circuit_name = "vcamc",
		.vreg_name = "wlan3",
		.initstate = 0,
		.voltage = 1200,
		.ldo = "L6",
		.always_on = 0,
	},
	[10] = {
		.circuit_name = "vreg_msma",
		.vreg_name = "msma",
		.initstate = 1,
		.voltage = 2600,
		.ldo = "L7",
		.always_on = 0,
	},
	[11] = {
		.circuit_name = "vreg_tcxo",
		.vreg_name = "tcxo",
		.initstate = 1,
		.voltage = 2800,
		.ldo = "L8",
		.always_on = 0,
	},
	[12] = {
		.circuit_name = "vreg_usb18",
		.vreg_name = "usb2",
		.initstate = 1,
		.voltage = 1800,
		.ldo = "L9",
		.always_on = 0,
	},
	[13] = {
		.circuit_name = "vbt",
		.vreg_name = "emmc",
		.initstate = 0,
		.voltage = 2850,
		.ldo = "L10",
		.always_on = 0,
	},
	[14] = {
		.circuit_name = "vreg_adc",
		.vreg_name = "wlan_tcx0",
		.initstate = 0,
		.voltage = 1800,
		.ldo = "L11",
		.always_on = 0,
	},
	[15] = {
		.circuit_name = "vreg_msmp",
		.vreg_name = "gp2",
		.initstate = 1,
		.voltage = 3300,
		.ldo = "L12",
		.always_on = 0,
	},
	[16] = {
		.circuit_name = "vreg_tflash",
		.vreg_name = "mmc",
		.initstate = 1,
		.voltage = 2850,
		.ldo = "L13",
		.always_on = 0,
	},
	[17] = {
		.circuit_name = "vreg_usb30",
		.vreg_name = "usb",
		.initstate = 1,
		.voltage = 3000,
		.ldo = "L14",
		.always_on = 0,
	},
	[18] = {
		.circuit_name = "vcamio",
		.vreg_name = "usim2",
		.initstate = 0,
		.voltage = 1800,
		.ldo = "L15",
		.always_on = 0,
	},
	[19] = {
		.circuit_name = "vreg_usim",
		.vreg_name = "ruim",
		.initstate = 1,
		.voltage = 3000,
		.ldo = "L16",
		.always_on = 0,
	},
	[20] = {
		.circuit_name = "vcama",
		.vreg_name = "bt",
		.initstate = 0,
		.voltage = 2800,
		.ldo = "L17",
		.always_on = 0,
	},
	[21] = {
		.circuit_name = "vreg_rfrx",
		.vreg_name = "rftx",
		.initstate = 1,
		.voltage = 2700,
		.ldo = "L18",
		.always_on = 0,
	},
	[22] = {
		.circuit_name = "vlcd",
		.vreg_name = "wlan4",
		.initstate = 1,
		.voltage = 3000,
		.ldo = "L19",
		.always_on = 0,
	},
};

static struct msm7x2xa_vreg *_vreg;
static unsigned int _vreg_size;
static u32 debug_suspend;

static void vreg_show_all_vregs(void)
{
	int n;
	struct vreg *vreg;
	unsigned int onoff;

	for (n = 0; n < _vreg_size; n++) {
		vreg = _vreg[n].vreg;
		onoff = vreg->refcnt | vreg->initstate;
		pr_info("[%s] %s %s refcnt %d init %d, always %s\n", __func__,
				_vreg[n].circuit_name, onoff ? "on" : "off",
				vreg->refcnt, vreg->initstate,
				vreg->always_on ? "on" : "off");
	}
}

void vreg_suspend_stats(void)
{
	unsigned int n;
	unsigned int cnt = 0;
	struct vreg *vreg;
	unsigned int onoff[_vreg_size];

	for (n = 0; n < _vreg_size; n++) {
		vreg = _vreg[n].vreg;
		onoff[n] = vreg->refcnt | vreg->initstate | vreg->always_on;
		if (onoff[n])
			cnt++;
	}

	pr_info("[%s] %d enabled: VSW: %s%s%s%s\n",
			__func__, cnt,
			onoff[0] ? "1, " : "", onoff[1] ? "2, " : "",
			onoff[2] ? "3, " : "", onoff[3] ? "4" : "");

	pr_info("[%s] LDO: %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
			__func__,
			onoff[4] ? "1, " : "", onoff[5] ? "2, " : "",
			onoff[6] ? "3, " : "", onoff[7] ? "4, " : "",
			onoff[8] ? "5, " : "", onoff[9] ? "6, " : "",
			onoff[10] ? "7, " : "", onoff[11] ? "8, " : "",
			onoff[12] ? "9, " : "", onoff[13] ? "10, " : "",
			onoff[14] ? "11, " : "", onoff[15] ? "12, " : "",
			onoff[16] ? "13, " : "", onoff[17] ? "14, " : "",
			onoff[18] ? "15, " : "", onoff[19] ? "16, " : "",
			onoff[20] ? "17, " : "", onoff[21] ? "18, " : "",
			onoff[22] ? "19" : "");
}

static struct vreg *_vreg_get(struct device *dev, const char *id)
{
	int n;
	for (n = 0; n < ARRAY_SIZE(vregs); n++) {
		if (!strncmp(vregs[n].name, id, 20))
			return vregs + n;
	}
	return ERR_PTR(-ENOENT);
}

struct vreg *vreg_get(struct device *dev, const char *id)
{
	int n;
	for (n = 0; n < _vreg_size; n++) {
		if (!strncmp(_vreg[n].circuit_name, id, 20))
			return _vreg_get(NULL, _vreg[n].vreg_name);
	}
	return ERR_PTR(-ENOENT);
}
EXPORT_SYMBOL(vreg_get);

void vreg_put(struct vreg *vreg)
{
}

int vreg_enable(struct vreg *vreg)
{
	unsigned id = vreg->id;
	int enable = VREG_SWITCH_ENABLE;

	if (vreg->always_on)
		return 0;

	if (vreg->refcnt == 0)
		vreg->status = msm_proc_comm(PCOM_VREG_SWITCH, &id, &enable);

	if ((vreg->refcnt < UINT_MAX) && (!vreg->status))
		vreg->refcnt++;

	return vreg->status;
}
EXPORT_SYMBOL(vreg_enable);

int vreg_disable(struct vreg *vreg)
{
	unsigned id = vreg->id;
	int disable = VREG_SWITCH_DISABLE;

	if (!vreg->refcnt && !vreg->initstate)
		return 0;

	if (vreg->always_on)
		return 0;

	if ((vreg->refcnt == 1) || (!vreg->refcnt && vreg->initstate))
		vreg->status = msm_proc_comm(PCOM_VREG_SWITCH, &id, &disable);

	if (!vreg->status && vreg->refcnt)
		vreg->refcnt--;

	if (!vreg->refcnt && vreg->initstate)
		vreg->initstate = 0;

	return vreg->status;
}
EXPORT_SYMBOL(vreg_disable);

int vreg_set_level(struct vreg *vreg, unsigned mv)
{
	unsigned id = vreg->id;

	if (vreg->always_on)
		return 0;

	vreg->status = msm_proc_comm(PCOM_VREG_SET_LEVEL, &id, &mv);
	return vreg->status;
}
EXPORT_SYMBOL(vreg_set_level);

int vreg_set_pulldown(struct vreg *vreg, unsigned onoff)
{
	unsigned id = vreg->id;

	return msm_proc_comm(PCOM_VREG_PULLDOWN, &onoff, &id);
}
EXPORT_SYMBOL(vreg_set_pulldown);

#if defined(CONFIG_DEBUG_FS)

static int vreg_debug_set(void *data, u64 val)
{
	struct vreg *vreg = data;

	pr_info("[%s] %s set to %d\n", __func__, vreg->name, (u32)val);

	switch (val) {
	case 0:
		vreg_disable(vreg);
		break;
	case 1:
		vreg_enable(vreg);
		break;
	default:
		vreg_set_level(vreg, val);
		break;
	}
	return 0;
}

static int vreg_debug_get(void *data, u64 *val)
{
	struct vreg *vreg = data;

	if (!vreg->status)
		*val = 0;
	else
		*val = 1;

	pr_info("[%s] %s refcnt %d init %d\n", __func__,
			vreg->name, vreg->refcnt, vreg->initstate);

	return 0;
}

static int vreg_debug_count_set(void *data, u64 val)
{
	struct vreg *vreg = data;
	if (val > UINT_MAX)
		val = UINT_MAX;
	vreg->refcnt = val;
	return 0;
}

static int vreg_debug_count_get(void *data, u64 *val)
{
	struct vreg *vreg = data;

	*val = vreg->refcnt;

	return 0;
}

static int vreg_show_all_get(void *data, u64 *val)
{
	vreg_show_all_vregs();
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(vreg_fops, vreg_debug_get,
		vreg_debug_set, "%llu\n");
DEFINE_SIMPLE_ATTRIBUTE(vreg_count_fops, vreg_debug_count_get,
		vreg_debug_count_set, "%llu\n");
DEFINE_SIMPLE_ATTRIBUTE(vreg_show_all_fops, vreg_show_all_get,
		NULL, "%llu\n");

static int vreg_debug_init(void)
{
	int n;
	struct dentry *dent;
	char name[32];
	const char *refcnt_name = "_refcnt";

	dent = debugfs_create_dir("vreg", 0);
	if (IS_ERR(dent))
		return 0;

	for (n = 0; n < _vreg_size; n++) {
		(void) debugfs_create_file(_vreg[n].circuit_name, 0644,
					   dent, _vreg[n].vreg, &vreg_fops);

		strlcpy(name, _vreg[n].circuit_name, sizeof(name));
		strlcat(name, refcnt_name, sizeof(name));
		(void) debugfs_create_file(name, 0644,
				dent, _vreg[n].vreg, &vreg_count_fops);
	}
	(void) debugfs_create_file("showall", 0444,
			dent, NULL, &vreg_show_all_fops);

	(void) debugfs_create_u32("dbgsuspend", S_IRUGO | S_IWUSR,
				dent, &debug_suspend);
	return 0;
}
#endif

void msm7x27a_vreg_init(unsigned int hwrev)
{
	int n;
	struct vreg *vreg;
	int enable = VREG_SWITCH_ENABLE;
	int status;

	switch (hwrev) {
	case 0:
		_vreg = jena_rev01_vreg;
		_vreg_size = ARRAY_SIZE(jena_rev01_vreg);
		break;
	case 1:
		_vreg = jena_rev01_vreg;
		_vreg_size = ARRAY_SIZE(jena_rev01_vreg);
		break;
	case 2:
	default:
		_vreg = jena_rev01_vreg;
		_vreg_size = ARRAY_SIZE(jena_rev01_vreg);
		break;
		;
	}

	for (n = 0; n < _vreg_size; n++) {
		vreg = _vreg_get(NULL, _vreg[n].vreg_name);
		vreg->initstate = _vreg[n].initstate;
		vreg->always_on = _vreg[n].always_on;
		_vreg[n].vreg = vreg;
		if (vreg->always_on) {
			status = msm_proc_comm(PCOM_VREG_SET_LEVEL,
					&(vreg->id), &(_vreg[n].voltage));
			if (!status)
				status = msm_proc_comm(PCOM_VREG_SWITCH,
						&(vreg->id), &enable);
			if (status)
				pr_info("[%s] turn on error: %s\n", __func__,
						_vreg[n].circuit_name);
			else
				pr_info("[%s] %s set to %d always on\n",
						__func__, _vreg[n].circuit_name,
						_vreg[n].voltage);
		}
	}


#if defined(CONFIG_DEBUG_FS)
	vreg_debug_init();
#endif
}
