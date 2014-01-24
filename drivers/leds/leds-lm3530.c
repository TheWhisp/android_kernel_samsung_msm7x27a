/*
 * Copyright (C) 2011 ST-Ericsson SA.
 * Copyright (C) 2009 Motorola, Inc.
 *
 * License Terms: GNU General Public License v2
 *
 * Simple driver for National Semiconductor LM3530 Backlight driver chip
 *
 * Author: Shreshtha Kumar SAHU <shreshthakumar.sahu@stericsson.com>
 * based on leds-lm3530.c by Dan Murphy <D.Murphy@motorola.com>
 */

#include <linux/i2c.h>
#include <linux/leds.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/led-lm3530.h>
#include <linux/types.h>
#include <linux/regulator/consumer.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master

#define LM3530_LED_DEV "lcd-backlight"
#define LM3530_NAME "lm3530-led"

#define LM3530_GEN_CONFIG		0x10
#define LM3530_ALS_CONFIG		0x20
#define LM3530_BRT_RAMP_RATE		0x30
<<<<<<< HEAD
<<<<<<< HEAD
#define LM3530_ALS_ZONE_REG		0x40
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define LM3530_ALS_IMP_SELECT		0x41
#define LM3530_BRT_CTRL_REG		0xA0
#define LM3530_ALS_ZB0_REG		0x60
#define LM3530_ALS_ZB1_REG		0x61
#define LM3530_ALS_ZB2_REG		0x62
#define LM3530_ALS_ZB3_REG		0x63
#define LM3530_ALS_Z0T_REG		0x70
#define LM3530_ALS_Z1T_REG		0x71
#define LM3530_ALS_Z2T_REG		0x72
#define LM3530_ALS_Z3T_REG		0x73
#define LM3530_ALS_Z4T_REG		0x74
<<<<<<< HEAD
<<<<<<< HEAD
#define LM3530_REG_MAX			15
=======
#define LM3530_REG_MAX			14
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define LM3530_REG_MAX			14
>>>>>>> refs/remotes/origin/master

/* General Control Register */
#define LM3530_EN_I2C_SHIFT		(0)
#define LM3530_RAMP_LAW_SHIFT		(1)
#define LM3530_MAX_CURR_SHIFT		(2)
#define LM3530_EN_PWM_SHIFT		(5)
#define LM3530_PWM_POL_SHIFT		(6)
#define LM3530_EN_PWM_SIMPLE_SHIFT	(7)

#define LM3530_ENABLE_I2C		(1 << LM3530_EN_I2C_SHIFT)
#define LM3530_ENABLE_PWM		(1 << LM3530_EN_PWM_SHIFT)
#define LM3530_POL_LOW			(1 << LM3530_PWM_POL_SHIFT)
#define LM3530_ENABLE_PWM_SIMPLE	(1 << LM3530_EN_PWM_SIMPLE_SHIFT)

/* ALS Config Register Options */
#define LM3530_ALS_AVG_TIME_SHIFT	(0)
#define LM3530_EN_ALS_SHIFT		(3)
#define LM3530_ALS_SEL_SHIFT		(5)

#define LM3530_ENABLE_ALS		(3 << LM3530_EN_ALS_SHIFT)

/* Brightness Ramp Rate Register */
#define LM3530_BRT_RAMP_FALL_SHIFT	(0)
#define LM3530_BRT_RAMP_RISE_SHIFT	(3)

/* ALS Resistor Select */
#define LM3530_ALS1_IMP_SHIFT		(0)
#define LM3530_ALS2_IMP_SHIFT		(4)

/* Zone Boundary Register defaults */
<<<<<<< HEAD
<<<<<<< HEAD
#define LM3530_DEF_ZB_0			(0x33)
#define LM3530_DEF_ZB_1			(0x66)
#define LM3530_DEF_ZB_2			(0x99)
#define LM3530_DEF_ZB_3			(0xCC)

/* Zone Target Register defaults */
#define LM3530_DEF_ZT_0			(0x19)
#define LM3530_DEF_ZT_1			(0x33)
#define LM3530_DEF_ZT_2			(0x4C)
#define LM3530_DEF_ZT_3			(0x66)
#define LM3530_DEF_ZT_4			(0x7F)
=======
=======
>>>>>>> refs/remotes/origin/master
#define LM3530_ALS_ZB_MAX		(4)
#define LM3530_ALS_WINDOW_mV		(1000)
#define LM3530_ALS_OFFSET_mV		(4)

/* Zone Target Register defaults */
#define LM3530_DEF_ZT_0			(0x7F)
#define LM3530_DEF_ZT_1			(0x66)
#define LM3530_DEF_ZT_2			(0x4C)
#define LM3530_DEF_ZT_3			(0x33)
#define LM3530_DEF_ZT_4			(0x19)

/* 7 bits are used for the brightness : LM3530_BRT_CTRL_REG */
#define MAX_BRIGHTNESS			(127)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

struct lm3530_mode_map {
	const char *mode;
	enum lm3530_mode mode_val;
};

static struct lm3530_mode_map mode_map[] = {
	{ "man", LM3530_BL_MODE_MANUAL },
	{ "als", LM3530_BL_MODE_ALS },
	{ "pwm", LM3530_BL_MODE_PWM },
};

/**
 * struct lm3530_data
 * @led_dev: led class device
 * @client: i2c client
 * @pdata: LM3530 platform data
 * @mode: mode of operation - manual, ALS, PWM
 * @regulator: regulator
 * @brighness: previous brightness value
 * @enable: regulator is enabled
 */
struct lm3530_data {
	struct led_classdev led_dev;
	struct i2c_client *client;
	struct lm3530_platform_data *pdata;
	enum lm3530_mode mode;
	struct regulator *regulator;
	enum led_brightness brightness;
	bool enable;
};

<<<<<<< HEAD
=======
/*
 * struct lm3530_als_data
 * @config  : value of ALS configuration register
 * @imp_sel : value of ALS resistor select register
 * @zone    : values of ALS ZB(Zone Boundary) registers
 */
struct lm3530_als_data {
	u8 config;
	u8 imp_sel;
	u8 zones[LM3530_ALS_ZB_MAX];
};

>>>>>>> refs/remotes/origin/master
static const u8 lm3530_reg[LM3530_REG_MAX] = {
	LM3530_GEN_CONFIG,
	LM3530_ALS_CONFIG,
	LM3530_BRT_RAMP_RATE,
<<<<<<< HEAD
<<<<<<< HEAD
	LM3530_ALS_ZONE_REG,
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	LM3530_ALS_IMP_SELECT,
	LM3530_BRT_CTRL_REG,
	LM3530_ALS_ZB0_REG,
	LM3530_ALS_ZB1_REG,
	LM3530_ALS_ZB2_REG,
	LM3530_ALS_ZB3_REG,
	LM3530_ALS_Z0T_REG,
	LM3530_ALS_Z1T_REG,
	LM3530_ALS_Z2T_REG,
	LM3530_ALS_Z3T_REG,
	LM3530_ALS_Z4T_REG,
};

static int lm3530_get_mode_from_str(const char *str)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(mode_map); i++)
		if (sysfs_streq(str, mode_map[i].mode))
			return mode_map[i].mode_val;

<<<<<<< HEAD
	return -1;
}

static int lm3530_init_registers(struct lm3530_data *drvdata)
{
	int ret = 0;
	int i;
	u8 gen_config;
	u8 als_config = 0;
	u8 brt_ramp;
	u8 als_imp_sel = 0;
	u8 brightness;
	u8 reg_val[LM3530_REG_MAX];
<<<<<<< HEAD
	struct lm3530_platform_data *pltfm = drvdata->pdata;
	struct i2c_client *client = drvdata->client;

	gen_config = (pltfm->brt_ramp_law << LM3530_RAMP_LAW_SHIFT) |
			((pltfm->max_current & 7) << LM3530_MAX_CURR_SHIFT);

	if (drvdata->mode == LM3530_BL_MODE_MANUAL ||
	    drvdata->mode == LM3530_BL_MODE_ALS)
		gen_config |= (LM3530_ENABLE_I2C);

	if (drvdata->mode == LM3530_BL_MODE_ALS) {
		als_config =
			(pltfm->als_avrg_time << LM3530_ALS_AVG_TIME_SHIFT) |
			(LM3530_ENABLE_ALS) |
			(pltfm->als_input_mode << LM3530_ALS_SEL_SHIFT);

		als_imp_sel =
			(pltfm->als1_resistor_sel << LM3530_ALS1_IMP_SHIFT) |
			(pltfm->als2_resistor_sel << LM3530_ALS2_IMP_SHIFT);
	}

	if (drvdata->mode == LM3530_BL_MODE_PWM)
		gen_config |= (LM3530_ENABLE_PWM) |
				(pltfm->pwm_pol_hi << LM3530_PWM_POL_SHIFT) |
				(LM3530_ENABLE_PWM_SIMPLE);

	brt_ramp = (pltfm->brt_ramp_fall << LM3530_BRT_RAMP_FALL_SHIFT) |
			(pltfm->brt_ramp_rise << LM3530_BRT_RAMP_RISE_SHIFT);
=======
	u8 zones[LM3530_ALS_ZB_MAX];
	u32 als_vmin, als_vmax, als_vstep;
	struct lm3530_platform_data *pdata = drvdata->pdata;
	struct i2c_client *client = drvdata->client;
	struct lm3530_pwm_data *pwm = &pdata->pwm_data;
=======
	return -EINVAL;
}

static void lm3530_als_configure(struct lm3530_platform_data *pdata,
				struct lm3530_als_data *als)
{
	int i;
	u32 als_vmin, als_vmax, als_vstep;

	if (pdata->als_vmax == 0) {
		pdata->als_vmin = 0;
		pdata->als_vmax = LM3530_ALS_WINDOW_mV;
	}

	als_vmin = pdata->als_vmin;
	als_vmax = pdata->als_vmax;

	if ((als_vmax - als_vmin) > LM3530_ALS_WINDOW_mV)
		pdata->als_vmax = als_vmax = als_vmin + LM3530_ALS_WINDOW_mV;

	/* n zone boundary makes n+1 zones */
	als_vstep = (als_vmax - als_vmin) / (LM3530_ALS_ZB_MAX + 1);

	for (i = 0; i < LM3530_ALS_ZB_MAX; i++)
		als->zones[i] = (((als_vmin + LM3530_ALS_OFFSET_mV) +
			als_vstep + (i * als_vstep)) * LED_FULL) / 1000;

	als->config =
		(pdata->als_avrg_time << LM3530_ALS_AVG_TIME_SHIFT) |
		(LM3530_ENABLE_ALS) |
		(pdata->als_input_mode << LM3530_ALS_SEL_SHIFT);

	als->imp_sel =
		(pdata->als1_resistor_sel << LM3530_ALS1_IMP_SHIFT) |
		(pdata->als2_resistor_sel << LM3530_ALS2_IMP_SHIFT);
}

static int lm3530_led_enable(struct lm3530_data *drvdata)
{
	int ret;

	if (drvdata->enable)
		return 0;

	ret = regulator_enable(drvdata->regulator);
	if (ret) {
		dev_err(drvdata->led_dev.dev, "Failed to enable vin:%d\n", ret);
		return ret;
	}

	drvdata->enable = true;
	return 0;
}

static void lm3530_led_disable(struct lm3530_data *drvdata)
{
	int ret;

	if (!drvdata->enable)
		return;

	ret = regulator_disable(drvdata->regulator);
	if (ret) {
		dev_err(drvdata->led_dev.dev, "Failed to disable vin:%d\n",
			ret);
		return;
	}

	drvdata->enable = false;
}

static int lm3530_init_registers(struct lm3530_data *drvdata)
{
	int ret = 0;
	int i;
	u8 gen_config;
	u8 brt_ramp;
	u8 brightness;
	u8 reg_val[LM3530_REG_MAX];
	struct lm3530_platform_data *pdata = drvdata->pdata;
	struct i2c_client *client = drvdata->client;
	struct lm3530_pwm_data *pwm = &pdata->pwm_data;
	struct lm3530_als_data als;

	memset(&als, 0, sizeof(struct lm3530_als_data));
>>>>>>> refs/remotes/origin/master

	gen_config = (pdata->brt_ramp_law << LM3530_RAMP_LAW_SHIFT) |
			((pdata->max_current & 7) << LM3530_MAX_CURR_SHIFT);

	switch (drvdata->mode) {
	case LM3530_BL_MODE_MANUAL:
<<<<<<< HEAD
	case LM3530_BL_MODE_ALS:
		gen_config |= LM3530_ENABLE_I2C;
=======
		gen_config |= LM3530_ENABLE_I2C;
		break;
	case LM3530_BL_MODE_ALS:
		gen_config |= LM3530_ENABLE_I2C;
		lm3530_als_configure(pdata, &als);
>>>>>>> refs/remotes/origin/master
		break;
	case LM3530_BL_MODE_PWM:
		gen_config |= LM3530_ENABLE_PWM | LM3530_ENABLE_PWM_SIMPLE |
			      (pdata->pwm_pol_hi << LM3530_PWM_POL_SHIFT);
		break;
	}

<<<<<<< HEAD
	if (drvdata->mode == LM3530_BL_MODE_ALS) {
		if (pdata->als_vmax == 0) {
			pdata->als_vmin = 0;
			pdata->als_vmax = LM3530_ALS_WINDOW_mV;
		}

		als_vmin = pdata->als_vmin;
		als_vmax = pdata->als_vmax;

		if ((als_vmax - als_vmin) > LM3530_ALS_WINDOW_mV)
			pdata->als_vmax = als_vmax =
				als_vmin + LM3530_ALS_WINDOW_mV;

		/* n zone boundary makes n+1 zones */
		als_vstep = (als_vmax - als_vmin) / (LM3530_ALS_ZB_MAX + 1);

		for (i = 0; i < LM3530_ALS_ZB_MAX; i++)
			zones[i] = (((als_vmin + LM3530_ALS_OFFSET_mV) +
					als_vstep + (i * als_vstep)) * LED_FULL)
					/ 1000;

		als_config =
			(pdata->als_avrg_time << LM3530_ALS_AVG_TIME_SHIFT) |
			(LM3530_ENABLE_ALS) |
			(pdata->als_input_mode << LM3530_ALS_SEL_SHIFT);

		als_imp_sel =
			(pdata->als1_resistor_sel << LM3530_ALS1_IMP_SHIFT) |
			(pdata->als2_resistor_sel << LM3530_ALS2_IMP_SHIFT);

	}

	brt_ramp = (pdata->brt_ramp_fall << LM3530_BRT_RAMP_FALL_SHIFT) |
			(pdata->brt_ramp_rise << LM3530_BRT_RAMP_RISE_SHIFT);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	brt_ramp = (pdata->brt_ramp_fall << LM3530_BRT_RAMP_FALL_SHIFT) |
			(pdata->brt_ramp_rise << LM3530_BRT_RAMP_RISE_SHIFT);
>>>>>>> refs/remotes/origin/master

	if (drvdata->brightness)
		brightness = drvdata->brightness;
	else
<<<<<<< HEAD
<<<<<<< HEAD
		brightness = drvdata->brightness = pltfm->brt_val;
=======
=======
>>>>>>> refs/remotes/origin/master
		brightness = drvdata->brightness = pdata->brt_val;

	if (brightness > drvdata->led_dev.max_brightness)
		brightness = drvdata->led_dev.max_brightness;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	reg_val[0] = gen_config;	/* LM3530_GEN_CONFIG */
	reg_val[1] = als_config;	/* LM3530_ALS_CONFIG */
	reg_val[2] = brt_ramp;		/* LM3530_BRT_RAMP_RATE */
<<<<<<< HEAD
	reg_val[3] = 0x00;		/* LM3530_ALS_ZONE_REG */
	reg_val[4] = als_imp_sel;	/* LM3530_ALS_IMP_SELECT */
	reg_val[5] = brightness;	/* LM3530_BRT_CTRL_REG */
	reg_val[6] = LM3530_DEF_ZB_0;	/* LM3530_ALS_ZB0_REG */
	reg_val[7] = LM3530_DEF_ZB_1;	/* LM3530_ALS_ZB1_REG */
	reg_val[8] = LM3530_DEF_ZB_2;	/* LM3530_ALS_ZB2_REG */
	reg_val[9] = LM3530_DEF_ZB_3;	/* LM3530_ALS_ZB3_REG */
	reg_val[10] = LM3530_DEF_ZT_0;	/* LM3530_ALS_Z0T_REG */
	reg_val[11] = LM3530_DEF_ZT_1;	/* LM3530_ALS_Z1T_REG */
	reg_val[12] = LM3530_DEF_ZT_2;	/* LM3530_ALS_Z2T_REG */
	reg_val[13] = LM3530_DEF_ZT_3;	/* LM3530_ALS_Z3T_REG */
	reg_val[14] = LM3530_DEF_ZT_4;	/* LM3530_ALS_Z4T_REG */
=======
	reg_val[3] = als_imp_sel;	/* LM3530_ALS_IMP_SELECT */
	reg_val[4] = brightness;	/* LM3530_BRT_CTRL_REG */
	reg_val[5] = zones[0];		/* LM3530_ALS_ZB0_REG */
	reg_val[6] = zones[1];		/* LM3530_ALS_ZB1_REG */
	reg_val[7] = zones[2];		/* LM3530_ALS_ZB2_REG */
	reg_val[8] = zones[3];		/* LM3530_ALS_ZB3_REG */
=======

	reg_val[0] = gen_config;	/* LM3530_GEN_CONFIG */
	reg_val[1] = als.config;	/* LM3530_ALS_CONFIG */
	reg_val[2] = brt_ramp;		/* LM3530_BRT_RAMP_RATE */
	reg_val[3] = als.imp_sel;	/* LM3530_ALS_IMP_SELECT */
	reg_val[4] = brightness;	/* LM3530_BRT_CTRL_REG */
	reg_val[5] = als.zones[0];	/* LM3530_ALS_ZB0_REG */
	reg_val[6] = als.zones[1];	/* LM3530_ALS_ZB1_REG */
	reg_val[7] = als.zones[2];	/* LM3530_ALS_ZB2_REG */
	reg_val[8] = als.zones[3];	/* LM3530_ALS_ZB3_REG */
>>>>>>> refs/remotes/origin/master
	reg_val[9] = LM3530_DEF_ZT_0;	/* LM3530_ALS_Z0T_REG */
	reg_val[10] = LM3530_DEF_ZT_1;	/* LM3530_ALS_Z1T_REG */
	reg_val[11] = LM3530_DEF_ZT_2;	/* LM3530_ALS_Z2T_REG */
	reg_val[12] = LM3530_DEF_ZT_3;	/* LM3530_ALS_Z3T_REG */
	reg_val[13] = LM3530_DEF_ZT_4;	/* LM3530_ALS_Z4T_REG */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	if (!drvdata->enable) {
		ret = regulator_enable(drvdata->regulator);
		if (ret) {
			dev_err(&drvdata->client->dev,
					"Enable regulator failed\n");
			return ret;
		}
		drvdata->enable = true;
	}

	for (i = 0; i < LM3530_REG_MAX; i++) {
<<<<<<< HEAD
=======
=======

	ret = lm3530_led_enable(drvdata);
	if (ret)
		return ret;

	for (i = 0; i < LM3530_REG_MAX; i++) {
>>>>>>> refs/remotes/origin/master
		/* do not update brightness register when pwm mode */
		if (lm3530_reg[i] == LM3530_BRT_CTRL_REG &&
		    drvdata->mode == LM3530_BL_MODE_PWM) {
			if (pwm->pwm_set_intensity)
				pwm->pwm_set_intensity(reg_val[i],
					drvdata->led_dev.max_brightness);
			continue;
		}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		ret = i2c_smbus_write_byte_data(client,
				lm3530_reg[i], reg_val[i]);
		if (ret)
			break;
	}

	return ret;
}

static void lm3530_brightness_set(struct led_classdev *led_cdev,
				     enum led_brightness brt_val)
{
	int err;
	struct lm3530_data *drvdata =
	    container_of(led_cdev, struct lm3530_data, led_dev);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct lm3530_platform_data *pdata = drvdata->pdata;
	struct lm3530_pwm_data *pwm = &pdata->pwm_data;
	u8 max_brightness = led_cdev->max_brightness;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct lm3530_platform_data *pdata = drvdata->pdata;
	struct lm3530_pwm_data *pwm = &pdata->pwm_data;
	u8 max_brightness = led_cdev->max_brightness;
>>>>>>> refs/remotes/origin/master

	switch (drvdata->mode) {
	case LM3530_BL_MODE_MANUAL:

		if (!drvdata->enable) {
			err = lm3530_init_registers(drvdata);
			if (err) {
				dev_err(&drvdata->client->dev,
					"Register Init failed: %d\n", err);
				break;
			}
		}

		/* set the brightness in brightness control register*/
		err = i2c_smbus_write_byte_data(drvdata->client,
<<<<<<< HEAD
<<<<<<< HEAD
				LM3530_BRT_CTRL_REG, brt_val / 2);
=======
				LM3530_BRT_CTRL_REG, brt_val);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				LM3530_BRT_CTRL_REG, brt_val);
>>>>>>> refs/remotes/origin/master
		if (err)
			dev_err(&drvdata->client->dev,
				"Unable to set brightness: %d\n", err);
		else
<<<<<<< HEAD
<<<<<<< HEAD
			drvdata->brightness = brt_val / 2;
=======
			drvdata->brightness = brt_val;
>>>>>>> refs/remotes/origin/cm-10.0

		if (brt_val == 0) {
			err = regulator_disable(drvdata->regulator);
			if (err)
				dev_err(&drvdata->client->dev,
					"Disable regulator failed\n");
			drvdata->enable = false;
		}
=======
			drvdata->brightness = brt_val;

		if (brt_val == 0)
			lm3530_led_disable(drvdata);
>>>>>>> refs/remotes/origin/master
		break;
	case LM3530_BL_MODE_ALS:
		break;
	case LM3530_BL_MODE_PWM:
<<<<<<< HEAD
<<<<<<< HEAD
=======
		if (pwm->pwm_set_intensity)
			pwm->pwm_set_intensity(brt_val, max_brightness);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (pwm->pwm_set_intensity)
			pwm->pwm_set_intensity(brt_val, max_brightness);
>>>>>>> refs/remotes/origin/master
		break;
	default:
		break;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static ssize_t lm3530_mode_get(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct lm3530_data *drvdata;
	int i, len = 0;

	drvdata = container_of(led_cdev, struct lm3530_data, led_dev);
	for (i = 0; i < ARRAY_SIZE(mode_map); i++)
		if (drvdata->mode == mode_map[i].mode_val)
			len += sprintf(buf + len, "[%s] ", mode_map[i].mode);
		else
			len += sprintf(buf + len, "%s ", mode_map[i].mode);

	len += sprintf(buf + len, "\n");

	return len;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static ssize_t lm3530_mode_set(struct device *dev, struct device_attribute
				   *attr, const char *buf, size_t size)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int err;
	struct i2c_client *client = container_of(
					dev->parent, struct i2c_client, dev);
	struct lm3530_data *drvdata = i2c_get_clientdata(client);
	int mode;

=======
=======
>>>>>>> refs/remotes/origin/master
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct lm3530_data *drvdata;
	struct lm3530_pwm_data *pwm;
	u8 max_brightness;
	int mode, err;

	drvdata = container_of(led_cdev, struct lm3530_data, led_dev);
	pwm = &drvdata->pdata->pwm_data;
	max_brightness = led_cdev->max_brightness;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	mode = lm3530_get_mode_from_str(buf);
	if (mode < 0) {
		dev_err(dev, "Invalid mode\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	if (mode == LM3530_BL_MODE_MANUAL)
		drvdata->mode = LM3530_BL_MODE_MANUAL;
	else if (mode == LM3530_BL_MODE_ALS)
		drvdata->mode = LM3530_BL_MODE_ALS;
	else if (mode == LM3530_BL_MODE_PWM) {
		dev_err(dev, "PWM mode not supported\n");
		return -EINVAL;
	}
=======
=======
	mode = lm3530_get_mode_from_str(buf);
	if (mode < 0) {
		dev_err(dev, "Invalid mode\n");
		return mode;
	}

>>>>>>> refs/remotes/origin/master
	drvdata->mode = mode;

	/* set pwm to low if unnecessary */
	if (mode != LM3530_BL_MODE_PWM && pwm->pwm_set_intensity)
		pwm->pwm_set_intensity(0, max_brightness);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	err = lm3530_init_registers(drvdata);
	if (err) {
		dev_err(dev, "Setting %s Mode failed :%d\n", buf, err);
		return err;
	}

	return sizeof(drvdata->mode);
}
<<<<<<< HEAD
<<<<<<< HEAD

static DEVICE_ATTR(mode, 0644, NULL, lm3530_mode_set);
=======
static DEVICE_ATTR(mode, 0644, lm3530_mode_get, lm3530_mode_set);
>>>>>>> refs/remotes/origin/cm-10.0

static int __devinit lm3530_probe(struct i2c_client *client,
			   const struct i2c_device_id *id)
{
	struct lm3530_platform_data *pdata = client->dev.platform_data;
=======
static DEVICE_ATTR(mode, 0644, lm3530_mode_get, lm3530_mode_set);

static int lm3530_probe(struct i2c_client *client,
			   const struct i2c_device_id *id)
{
	struct lm3530_platform_data *pdata = dev_get_platdata(&client->dev);
>>>>>>> refs/remotes/origin/master
	struct lm3530_data *drvdata;
	int err = 0;

	if (pdata == NULL) {
		dev_err(&client->dev, "platform data required\n");
<<<<<<< HEAD
		err = -ENODEV;
		goto err_out;
=======
		return -ENODEV;
>>>>>>> refs/remotes/origin/master
	}

	/* BL mode */
	if (pdata->mode > LM3530_BL_MODE_PWM) {
		dev_err(&client->dev, "Illegal Mode request\n");
<<<<<<< HEAD
		err = -EINVAL;
		goto err_out;
=======
		return -EINVAL;
>>>>>>> refs/remotes/origin/master
	}

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->dev, "I2C_FUNC_I2C not supported\n");
<<<<<<< HEAD
		err = -EIO;
		goto err_out;
	}

	drvdata = kzalloc(sizeof(struct lm3530_data), GFP_KERNEL);
	if (drvdata == NULL) {
		err = -ENOMEM;
		goto err_out;
	}
=======
		return -EIO;
	}

	drvdata = devm_kzalloc(&client->dev, sizeof(struct lm3530_data),
				GFP_KERNEL);
	if (drvdata == NULL)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master

	drvdata->mode = pdata->mode;
	drvdata->client = client;
	drvdata->pdata = pdata;
	drvdata->brightness = LED_OFF;
	drvdata->enable = false;
	drvdata->led_dev.name = LM3530_LED_DEV;
	drvdata->led_dev.brightness_set = lm3530_brightness_set;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	drvdata->led_dev.max_brightness = MAX_BRIGHTNESS;
>>>>>>> refs/remotes/origin/cm-10.0

	i2c_set_clientdata(client, drvdata);

	drvdata->regulator = regulator_get(&client->dev, "vin");
=======
	drvdata->led_dev.max_brightness = MAX_BRIGHTNESS;

	i2c_set_clientdata(client, drvdata);

	drvdata->regulator = devm_regulator_get(&client->dev, "vin");
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(drvdata->regulator)) {
		dev_err(&client->dev, "regulator get failed\n");
		err = PTR_ERR(drvdata->regulator);
		drvdata->regulator = NULL;
<<<<<<< HEAD
		goto err_regulator_get;
=======
		return err;
>>>>>>> refs/remotes/origin/master
	}

	if (drvdata->pdata->brt_val) {
		err = lm3530_init_registers(drvdata);
		if (err < 0) {
			dev_err(&client->dev,
				"Register Init failed: %d\n", err);
<<<<<<< HEAD
			err = -ENODEV;
			goto err_reg_init;
=======
			return err;
>>>>>>> refs/remotes/origin/master
		}
	}
	err = led_classdev_register(&client->dev, &drvdata->led_dev);
	if (err < 0) {
		dev_err(&client->dev, "Register led class failed: %d\n", err);
<<<<<<< HEAD
		err = -ENODEV;
		goto err_class_register;
=======
		return err;
>>>>>>> refs/remotes/origin/master
	}

	err = device_create_file(drvdata->led_dev.dev, &dev_attr_mode);
	if (err < 0) {
		dev_err(&client->dev, "File device creation failed: %d\n", err);
		err = -ENODEV;
		goto err_create_file;
	}

	return 0;

err_create_file:
	led_classdev_unregister(&drvdata->led_dev);
<<<<<<< HEAD
err_class_register:
err_reg_init:
	regulator_put(drvdata->regulator);
err_regulator_get:
<<<<<<< HEAD
	i2c_set_clientdata(client, NULL);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(drvdata);
err_out:
	return err;
}

static int __devexit lm3530_remove(struct i2c_client *client)
=======
	return err;
}

static int lm3530_remove(struct i2c_client *client)
>>>>>>> refs/remotes/origin/master
{
	struct lm3530_data *drvdata = i2c_get_clientdata(client);

	device_remove_file(drvdata->led_dev.dev, &dev_attr_mode);

<<<<<<< HEAD
	if (drvdata->enable)
		regulator_disable(drvdata->regulator);
	regulator_put(drvdata->regulator);
	led_classdev_unregister(&drvdata->led_dev);
	kfree(drvdata);
=======
	lm3530_led_disable(drvdata);
	led_classdev_unregister(&drvdata->led_dev);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static const struct i2c_device_id lm3530_id[] = {
	{LM3530_NAME, 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, lm3530_id);

static struct i2c_driver lm3530_i2c_driver = {
	.probe = lm3530_probe,
<<<<<<< HEAD
<<<<<<< HEAD
	.remove = lm3530_remove,
=======
	.remove = __devexit_p(lm3530_remove),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.remove = lm3530_remove,
>>>>>>> refs/remotes/origin/master
	.id_table = lm3530_id,
	.driver = {
		.name = LM3530_NAME,
		.owner = THIS_MODULE,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
static int __init lm3530_init(void)
{
	return i2c_add_driver(&lm3530_i2c_driver);
}

static void __exit lm3530_exit(void)
{
	i2c_del_driver(&lm3530_i2c_driver);
}

module_init(lm3530_init);
module_exit(lm3530_exit);
=======
module_i2c_driver(lm3530_i2c_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_i2c_driver(lm3530_i2c_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("Back Light driver for LM3530");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Shreshtha Kumar SAHU <shreshthakumar.sahu@stericsson.com>");
