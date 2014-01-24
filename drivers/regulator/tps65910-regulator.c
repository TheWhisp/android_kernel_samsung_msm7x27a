/*
 * tps65910.c  --  TI tps65910
 *
 * Copyright 2010 Texas Instruments Inc.
 *
 * Author: Graeme Gregory <gg@slimlogic.co.uk>
 * Author: Jorge Eduardo Candelaria <jedu@slimlogic.co.uk>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under  the terms of the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the License, or (at your
 *  option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
<<<<<<< HEAD
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/mfd/tps65910.h>

<<<<<<< HEAD
#define TPS65910_REG_VRTC		0
#define TPS65910_REG_VIO		1
#define TPS65910_REG_VDD1		2
#define TPS65910_REG_VDD2		3
#define TPS65910_REG_VDD3		4
#define TPS65910_REG_VDIG1		5
#define TPS65910_REG_VDIG2		6
#define TPS65910_REG_VPLL		7
#define TPS65910_REG_VDAC		8
#define TPS65910_REG_VAUX1		9
#define TPS65910_REG_VAUX2		10
#define TPS65910_REG_VAUX33		11
#define TPS65910_REG_VMMC		12

#define TPS65911_REG_VDDCTRL		4
#define TPS65911_REG_LDO1		5
#define TPS65911_REG_LDO2		6
#define TPS65911_REG_LDO3		7
#define TPS65911_REG_LDO4		8
#define TPS65911_REG_LDO5		9
#define TPS65911_REG_LDO6		10
#define TPS65911_REG_LDO7		11
#define TPS65911_REG_LDO8		12

#define TPS65910_NUM_REGULATOR		13
#define TPS65910_SUPPLY_STATE_ENABLED	0x1
=======
=======
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/mfd/tps65910.h>
#include <linux/regulator/of_regulator.h>

>>>>>>> refs/remotes/origin/master
#define TPS65910_SUPPLY_STATE_ENABLED	0x1
#define EXT_SLEEP_CONTROL (TPS65910_SLEEP_CONTROL_EXT_INPUT_EN1 |	\
			TPS65910_SLEEP_CONTROL_EXT_INPUT_EN2 |		\
			TPS65910_SLEEP_CONTROL_EXT_INPUT_EN3 |		\
			TPS65911_SLEEP_CONTROL_EXT_INPUT_SLEEP)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

/* supported VIO voltages in milivolts */
static const u16 VIO_VSEL_table[] = {
	1500, 1800, 2500, 3300,
=======

/* supported VIO voltages in microvolts */
static const unsigned int VIO_VSEL_table[] = {
	1500000, 1800000, 2500000, 3300000,
>>>>>>> refs/remotes/origin/master
};

/* VSEL tables for TPS65910 specific LDOs and dcdc's */

<<<<<<< HEAD
/* supported VDD3 voltages in milivolts */
static const u16 VDD3_VSEL_table[] = {
	5000,
};

/* supported VDIG1 voltages in milivolts */
static const u16 VDIG1_VSEL_table[] = {
	1200, 1500, 1800, 2700,
};

/* supported VDIG2 voltages in milivolts */
static const u16 VDIG2_VSEL_table[] = {
	1000, 1100, 1200, 1800,
};

/* supported VPLL voltages in milivolts */
static const u16 VPLL_VSEL_table[] = {
	1000, 1100, 1800, 2500,
};

/* supported VDAC voltages in milivolts */
static const u16 VDAC_VSEL_table[] = {
	1800, 2600, 2800, 2850,
};

/* supported VAUX1 voltages in milivolts */
static const u16 VAUX1_VSEL_table[] = {
	1800, 2500, 2800, 2850,
};

/* supported VAUX2 voltages in milivolts */
static const u16 VAUX2_VSEL_table[] = {
	1800, 2800, 2900, 3300,
};

/* supported VAUX33 voltages in milivolts */
static const u16 VAUX33_VSEL_table[] = {
	1800, 2000, 2800, 3300,
};

/* supported VMMC voltages in milivolts */
static const u16 VMMC_VSEL_table[] = {
	1800, 2800, 3000, 3300,
=======
/* supported VRTC voltages in microvolts */
static const unsigned int VRTC_VSEL_table[] = {
	1800000,
};

/* supported VDD3 voltages in microvolts */
static const unsigned int VDD3_VSEL_table[] = {
	5000000,
};

/* supported VDIG1 voltages in microvolts */
static const unsigned int VDIG1_VSEL_table[] = {
	1200000, 1500000, 1800000, 2700000,
};

/* supported VDIG2 voltages in microvolts */
static const unsigned int VDIG2_VSEL_table[] = {
	1000000, 1100000, 1200000, 1800000,
};

/* supported VPLL voltages in microvolts */
static const unsigned int VPLL_VSEL_table[] = {
	1000000, 1100000, 1800000, 2500000,
};

/* supported VDAC voltages in microvolts */
static const unsigned int VDAC_VSEL_table[] = {
	1800000, 2600000, 2800000, 2850000,
};

/* supported VAUX1 voltages in microvolts */
static const unsigned int VAUX1_VSEL_table[] = {
	1800000, 2500000, 2800000, 2850000,
};

/* supported VAUX2 voltages in microvolts */
static const unsigned int VAUX2_VSEL_table[] = {
	1800000, 2800000, 2900000, 3300000,
};

/* supported VAUX33 voltages in microvolts */
static const unsigned int VAUX33_VSEL_table[] = {
	1800000, 2000000, 2800000, 3300000,
};

/* supported VMMC voltages in microvolts */
static const unsigned int VMMC_VSEL_table[] = {
	1800000, 2800000, 3000000, 3300000,
>>>>>>> refs/remotes/origin/master
};

struct tps_info {
	const char *name;
<<<<<<< HEAD
	unsigned min_uV;
	unsigned max_uV;
<<<<<<< HEAD
	u8 table_len;
	const u16 *table;
=======
	u8 n_voltages;
	const u16 *voltage_table;
	int enable_time_us;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const char *vin_name;
	u8 n_voltages;
	const unsigned int *voltage_table;
	int enable_time_us;
>>>>>>> refs/remotes/origin/master
};

static struct tps_info tps65910_regs[] = {
	{
<<<<<<< HEAD
		.name = "VRTC",
<<<<<<< HEAD
=======
		.enable_time_us = 2200,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VIO",
		.min_uV = 1500000,
		.max_uV = 3300000,
<<<<<<< HEAD
		.table_len = ARRAY_SIZE(VIO_VSEL_table),
		.table = VIO_VSEL_table,
=======
		.n_voltages = ARRAY_SIZE(VIO_VSEL_table),
		.voltage_table = VIO_VSEL_table,
		.enable_time_us = 350,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VDD1",
		.min_uV = 600000,
		.max_uV = 4500000,
<<<<<<< HEAD
=======
		.enable_time_us = 350,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VDD2",
		.min_uV = 600000,
		.max_uV = 4500000,
<<<<<<< HEAD
=======
		.enable_time_us = 350,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VDD3",
		.min_uV = 5000000,
		.max_uV = 5000000,
<<<<<<< HEAD
		.table_len = ARRAY_SIZE(VDD3_VSEL_table),
		.table = VDD3_VSEL_table,
=======
		.n_voltages = ARRAY_SIZE(VDD3_VSEL_table),
		.voltage_table = VDD3_VSEL_table,
		.enable_time_us = 200,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VDIG1",
		.min_uV = 1200000,
		.max_uV = 2700000,
<<<<<<< HEAD
		.table_len = ARRAY_SIZE(VDIG1_VSEL_table),
		.table = VDIG1_VSEL_table,
=======
		.n_voltages = ARRAY_SIZE(VDIG1_VSEL_table),
		.voltage_table = VDIG1_VSEL_table,
		.enable_time_us = 100,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VDIG2",
		.min_uV = 1000000,
		.max_uV = 1800000,
<<<<<<< HEAD
		.table_len = ARRAY_SIZE(VDIG2_VSEL_table),
		.table = VDIG2_VSEL_table,
=======
		.n_voltages = ARRAY_SIZE(VDIG2_VSEL_table),
		.voltage_table = VDIG2_VSEL_table,
		.enable_time_us = 100,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VPLL",
		.min_uV = 1000000,
		.max_uV = 2500000,
<<<<<<< HEAD
		.table_len = ARRAY_SIZE(VPLL_VSEL_table),
		.table = VPLL_VSEL_table,
=======
		.n_voltages = ARRAY_SIZE(VPLL_VSEL_table),
		.voltage_table = VPLL_VSEL_table,
		.enable_time_us = 100,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VDAC",
		.min_uV = 1800000,
		.max_uV = 2850000,
<<<<<<< HEAD
		.table_len = ARRAY_SIZE(VDAC_VSEL_table),
		.table = VDAC_VSEL_table,
=======
		.n_voltages = ARRAY_SIZE(VDAC_VSEL_table),
		.voltage_table = VDAC_VSEL_table,
		.enable_time_us = 100,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VAUX1",
		.min_uV = 1800000,
		.max_uV = 2850000,
<<<<<<< HEAD
		.table_len = ARRAY_SIZE(VAUX1_VSEL_table),
		.table = VAUX1_VSEL_table,
=======
		.n_voltages = ARRAY_SIZE(VAUX1_VSEL_table),
		.voltage_table = VAUX1_VSEL_table,
		.enable_time_us = 100,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VAUX2",
		.min_uV = 1800000,
		.max_uV = 3300000,
<<<<<<< HEAD
		.table_len = ARRAY_SIZE(VAUX2_VSEL_table),
		.table = VAUX2_VSEL_table,
=======
		.n_voltages = ARRAY_SIZE(VAUX2_VSEL_table),
		.voltage_table = VAUX2_VSEL_table,
		.enable_time_us = 100,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VAUX33",
		.min_uV = 1800000,
		.max_uV = 3300000,
<<<<<<< HEAD
		.table_len = ARRAY_SIZE(VAUX33_VSEL_table),
		.table = VAUX33_VSEL_table,
=======
		.n_voltages = ARRAY_SIZE(VAUX33_VSEL_table),
		.voltage_table = VAUX33_VSEL_table,
		.enable_time_us = 100,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VMMC",
		.min_uV = 1800000,
		.max_uV = 3300000,
<<<<<<< HEAD
		.table_len = ARRAY_SIZE(VMMC_VSEL_table),
		.table = VMMC_VSEL_table,
=======
		.n_voltages = ARRAY_SIZE(VMMC_VSEL_table),
		.voltage_table = VMMC_VSEL_table,
		.enable_time_us = 100,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.name = "vrtc",
		.vin_name = "vcc7",
		.n_voltages = ARRAY_SIZE(VRTC_VSEL_table),
		.voltage_table = VRTC_VSEL_table,
		.enable_time_us = 2200,
	},
	{
		.name = "vio",
		.vin_name = "vccio",
		.n_voltages = ARRAY_SIZE(VIO_VSEL_table),
		.voltage_table = VIO_VSEL_table,
		.enable_time_us = 350,
	},
	{
		.name = "vdd1",
		.vin_name = "vcc1",
		.enable_time_us = 350,
	},
	{
		.name = "vdd2",
		.vin_name = "vcc2",
		.enable_time_us = 350,
	},
	{
		.name = "vdd3",
		.n_voltages = ARRAY_SIZE(VDD3_VSEL_table),
		.voltage_table = VDD3_VSEL_table,
		.enable_time_us = 200,
	},
	{
		.name = "vdig1",
		.vin_name = "vcc6",
		.n_voltages = ARRAY_SIZE(VDIG1_VSEL_table),
		.voltage_table = VDIG1_VSEL_table,
		.enable_time_us = 100,
	},
	{
		.name = "vdig2",
		.vin_name = "vcc6",
		.n_voltages = ARRAY_SIZE(VDIG2_VSEL_table),
		.voltage_table = VDIG2_VSEL_table,
		.enable_time_us = 100,
	},
	{
		.name = "vpll",
		.vin_name = "vcc5",
		.n_voltages = ARRAY_SIZE(VPLL_VSEL_table),
		.voltage_table = VPLL_VSEL_table,
		.enable_time_us = 100,
	},
	{
		.name = "vdac",
		.vin_name = "vcc5",
		.n_voltages = ARRAY_SIZE(VDAC_VSEL_table),
		.voltage_table = VDAC_VSEL_table,
		.enable_time_us = 100,
	},
	{
		.name = "vaux1",
		.vin_name = "vcc4",
		.n_voltages = ARRAY_SIZE(VAUX1_VSEL_table),
		.voltage_table = VAUX1_VSEL_table,
		.enable_time_us = 100,
	},
	{
		.name = "vaux2",
		.vin_name = "vcc4",
		.n_voltages = ARRAY_SIZE(VAUX2_VSEL_table),
		.voltage_table = VAUX2_VSEL_table,
		.enable_time_us = 100,
	},
	{
		.name = "vaux33",
		.vin_name = "vcc3",
		.n_voltages = ARRAY_SIZE(VAUX33_VSEL_table),
		.voltage_table = VAUX33_VSEL_table,
		.enable_time_us = 100,
	},
	{
		.name = "vmmc",
		.vin_name = "vcc3",
		.n_voltages = ARRAY_SIZE(VMMC_VSEL_table),
		.voltage_table = VMMC_VSEL_table,
		.enable_time_us = 100,
>>>>>>> refs/remotes/origin/master
	},
};

static struct tps_info tps65911_regs[] = {
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.name = "VIO",
		.min_uV = 1500000,
		.max_uV = 3300000,
		.table_len = ARRAY_SIZE(VIO_VSEL_table),
		.table = VIO_VSEL_table,
=======
		.name = "VRTC",
		.enable_time_us = 2200,
	},
	{
		.name = "VIO",
		.min_uV = 1500000,
		.max_uV = 3300000,
		.n_voltages = ARRAY_SIZE(VIO_VSEL_table),
		.voltage_table = VIO_VSEL_table,
		.enable_time_us = 350,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VDD1",
		.min_uV = 600000,
		.max_uV = 4500000,
<<<<<<< HEAD
=======
		.n_voltages = 73,
		.enable_time_us = 350,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VDD2",
		.min_uV = 600000,
		.max_uV = 4500000,
<<<<<<< HEAD
=======
		.n_voltages = 73,
		.enable_time_us = 350,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VDDCTRL",
		.min_uV = 600000,
		.max_uV = 1400000,
<<<<<<< HEAD
=======
		.n_voltages = 65,
		.enable_time_us = 900,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "LDO1",
		.min_uV = 1000000,
		.max_uV = 3300000,
<<<<<<< HEAD
=======
		.n_voltages = 47,
		.enable_time_us = 420,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "LDO2",
		.min_uV = 1000000,
		.max_uV = 3300000,
<<<<<<< HEAD
=======
		.n_voltages = 47,
		.enable_time_us = 420,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "LDO3",
		.min_uV = 1000000,
		.max_uV = 3300000,
<<<<<<< HEAD
=======
		.n_voltages = 24,
		.enable_time_us = 230,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "LDO4",
		.min_uV = 1000000,
		.max_uV = 3300000,
<<<<<<< HEAD
=======
		.n_voltages = 47,
		.enable_time_us = 230,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "LDO5",
		.min_uV = 1000000,
		.max_uV = 3300000,
<<<<<<< HEAD
=======
		.n_voltages = 24,
		.enable_time_us = 230,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "LDO6",
		.min_uV = 1000000,
		.max_uV = 3300000,
<<<<<<< HEAD
=======
		.n_voltages = 24,
		.enable_time_us = 230,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "LDO7",
		.min_uV = 1000000,
		.max_uV = 3300000,
<<<<<<< HEAD
=======
		.n_voltages = 24,
		.enable_time_us = 230,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "LDO8",
		.min_uV = 1000000,
		.max_uV = 3300000,
<<<<<<< HEAD
	},
};

struct tps65910_reg {
	struct regulator_desc desc[TPS65910_NUM_REGULATOR];
	struct tps65910 *mfd;
	struct regulator_dev *rdev[TPS65910_NUM_REGULATOR];
	struct tps_info *info[TPS65910_NUM_REGULATOR];
	struct mutex mutex;
	int mode;
	int  (*get_ctrl_reg)(int);
=======
		.n_voltages = 24,
=======
		.name = "vrtc",
		.vin_name = "vcc7",
		.enable_time_us = 2200,
	},
	{
		.name = "vio",
		.vin_name = "vccio",
		.n_voltages = ARRAY_SIZE(VIO_VSEL_table),
		.voltage_table = VIO_VSEL_table,
		.enable_time_us = 350,
	},
	{
		.name = "vdd1",
		.vin_name = "vcc1",
		.n_voltages = 0x4C,
		.enable_time_us = 350,
	},
	{
		.name = "vdd2",
		.vin_name = "vcc2",
		.n_voltages = 0x4C,
		.enable_time_us = 350,
	},
	{
		.name = "vddctrl",
		.n_voltages = 0x44,
		.enable_time_us = 900,
	},
	{
		.name = "ldo1",
		.vin_name = "vcc6",
		.n_voltages = 0x33,
		.enable_time_us = 420,
	},
	{
		.name = "ldo2",
		.vin_name = "vcc6",
		.n_voltages = 0x33,
		.enable_time_us = 420,
	},
	{
		.name = "ldo3",
		.vin_name = "vcc5",
		.n_voltages = 0x1A,
		.enable_time_us = 230,
	},
	{
		.name = "ldo4",
		.vin_name = "vcc5",
		.n_voltages = 0x33,
		.enable_time_us = 230,
	},
	{
		.name = "ldo5",
		.vin_name = "vcc4",
		.n_voltages = 0x1A,
		.enable_time_us = 230,
	},
	{
		.name = "ldo6",
		.vin_name = "vcc3",
		.n_voltages = 0x1A,
		.enable_time_us = 230,
	},
	{
		.name = "ldo7",
		.vin_name = "vcc3",
		.n_voltages = 0x1A,
		.enable_time_us = 230,
	},
	{
		.name = "ldo8",
		.vin_name = "vcc3",
		.n_voltages = 0x1A,
>>>>>>> refs/remotes/origin/master
		.enable_time_us = 230,
	},
};

#define EXT_CONTROL_REG_BITS(id, regs_offs, bits) (((regs_offs) << 8) | (bits))
static unsigned int tps65910_ext_sleep_control[] = {
	0,
	EXT_CONTROL_REG_BITS(VIO,    1, 0),
	EXT_CONTROL_REG_BITS(VDD1,   1, 1),
	EXT_CONTROL_REG_BITS(VDD2,   1, 2),
	EXT_CONTROL_REG_BITS(VDD3,   1, 3),
	EXT_CONTROL_REG_BITS(VDIG1,  0, 1),
	EXT_CONTROL_REG_BITS(VDIG2,  0, 2),
	EXT_CONTROL_REG_BITS(VPLL,   0, 6),
	EXT_CONTROL_REG_BITS(VDAC,   0, 7),
	EXT_CONTROL_REG_BITS(VAUX1,  0, 3),
	EXT_CONTROL_REG_BITS(VAUX2,  0, 4),
	EXT_CONTROL_REG_BITS(VAUX33, 0, 5),
	EXT_CONTROL_REG_BITS(VMMC,   0, 0),
};

static unsigned int tps65911_ext_sleep_control[] = {
	0,
	EXT_CONTROL_REG_BITS(VIO,     1, 0),
	EXT_CONTROL_REG_BITS(VDD1,    1, 1),
	EXT_CONTROL_REG_BITS(VDD2,    1, 2),
	EXT_CONTROL_REG_BITS(VDDCTRL, 1, 3),
	EXT_CONTROL_REG_BITS(LDO1,    0, 1),
	EXT_CONTROL_REG_BITS(LDO2,    0, 2),
	EXT_CONTROL_REG_BITS(LDO3,    0, 7),
	EXT_CONTROL_REG_BITS(LDO4,    0, 6),
	EXT_CONTROL_REG_BITS(LDO5,    0, 3),
	EXT_CONTROL_REG_BITS(LDO6,    0, 0),
	EXT_CONTROL_REG_BITS(LDO7,    0, 5),
	EXT_CONTROL_REG_BITS(LDO8,    0, 4),
};

struct tps65910_reg {
	struct regulator_desc *desc;
	struct tps65910 *mfd;
	struct regulator_dev **rdev;
	struct tps_info **info;
<<<<<<< HEAD
	struct mutex mutex;
=======
>>>>>>> refs/remotes/origin/master
	int num_regulators;
	int mode;
	int  (*get_ctrl_reg)(int);
	unsigned int *ext_sleep_control;
	unsigned int board_ext_control[TPS65910_NUM_REGS];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
};

static inline int tps65910_read(struct tps65910_reg *pmic, u8 reg)
{
	u8 val;
	int err;

	err = pmic->mfd->read(pmic->mfd, reg, 1, &val);
	if (err)
		return err;

	return val;
}

static inline int tps65910_write(struct tps65910_reg *pmic, u8 reg, u8 val)
{
	return pmic->mfd->write(pmic->mfd, reg, 1, &val);
}

static int tps65910_modify_bits(struct tps65910_reg *pmic, u8 reg,
					u8 set_mask, u8 clear_mask)
{
	int err, data;

	mutex_lock(&pmic->mutex);

	data = tps65910_read(pmic, reg);
	if (data < 0) {
		dev_err(pmic->mfd->dev, "Read from reg 0x%x failed\n", reg);
		err = data;
		goto out;
	}

	data &= ~clear_mask;
	data |= set_mask;
	err = tps65910_write(pmic, reg, data);
	if (err)
		dev_err(pmic->mfd->dev, "Write for reg 0x%x failed\n", reg);

out:
	mutex_unlock(&pmic->mutex);
	return err;
}

static int tps65910_reg_read(struct tps65910_reg *pmic, u8 reg)
{
	int data;

	mutex_lock(&pmic->mutex);

	data = tps65910_read(pmic, reg);
	if (data < 0)
		dev_err(pmic->mfd->dev, "Read from reg 0x%x failed\n", reg);

	mutex_unlock(&pmic->mutex);
	return data;
}

static int tps65910_reg_write(struct tps65910_reg *pmic, u8 reg, u8 val)
{
	int err;

	mutex_lock(&pmic->mutex);

	err = tps65910_write(pmic, reg, val);
	if (err < 0)
		dev_err(pmic->mfd->dev, "Write for reg 0x%x failed\n", reg);

	mutex_unlock(&pmic->mutex);
	return err;
}

=======
};

>>>>>>> refs/remotes/origin/master
static int tps65910_get_ctrl_register(int id)
{
	switch (id) {
	case TPS65910_REG_VRTC:
		return TPS65910_VRTC;
	case TPS65910_REG_VIO:
		return TPS65910_VIO;
	case TPS65910_REG_VDD1:
		return TPS65910_VDD1;
	case TPS65910_REG_VDD2:
		return TPS65910_VDD2;
	case TPS65910_REG_VDD3:
		return TPS65910_VDD3;
	case TPS65910_REG_VDIG1:
		return TPS65910_VDIG1;
	case TPS65910_REG_VDIG2:
		return TPS65910_VDIG2;
	case TPS65910_REG_VPLL:
		return TPS65910_VPLL;
	case TPS65910_REG_VDAC:
		return TPS65910_VDAC;
	case TPS65910_REG_VAUX1:
		return TPS65910_VAUX1;
	case TPS65910_REG_VAUX2:
		return TPS65910_VAUX2;
	case TPS65910_REG_VAUX33:
		return TPS65910_VAUX33;
	case TPS65910_REG_VMMC:
		return TPS65910_VMMC;
	default:
		return -EINVAL;
	}
}

static int tps65911_get_ctrl_register(int id)
{
	switch (id) {
	case TPS65910_REG_VRTC:
		return TPS65910_VRTC;
	case TPS65910_REG_VIO:
		return TPS65910_VIO;
	case TPS65910_REG_VDD1:
		return TPS65910_VDD1;
	case TPS65910_REG_VDD2:
		return TPS65910_VDD2;
	case TPS65911_REG_VDDCTRL:
		return TPS65911_VDDCTRL;
	case TPS65911_REG_LDO1:
		return TPS65911_LDO1;
	case TPS65911_REG_LDO2:
		return TPS65911_LDO2;
	case TPS65911_REG_LDO3:
		return TPS65911_LDO3;
	case TPS65911_REG_LDO4:
		return TPS65911_LDO4;
	case TPS65911_REG_LDO5:
		return TPS65911_LDO5;
	case TPS65911_REG_LDO6:
		return TPS65911_LDO6;
	case TPS65911_REG_LDO7:
		return TPS65911_LDO7;
	case TPS65911_REG_LDO8:
		return TPS65911_LDO8;
	default:
		return -EINVAL;
	}
}

<<<<<<< HEAD
static int tps65910_is_enabled(struct regulator_dev *dev)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int reg, value, id = rdev_get_id(dev);

	reg = pmic->get_ctrl_reg(id);
	if (reg < 0)
		return reg;

	value = tps65910_reg_read(pmic, reg);
	if (value < 0)
		return value;

	return value & TPS65910_SUPPLY_STATE_ENABLED;
}

static int tps65910_enable(struct regulator_dev *dev)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	struct tps65910 *mfd = pmic->mfd;
	int reg, id = rdev_get_id(dev);

	reg = pmic->get_ctrl_reg(id);
	if (reg < 0)
		return reg;

	return tps65910_set_bits(mfd, reg, TPS65910_SUPPLY_STATE_ENABLED);
}

static int tps65910_disable(struct regulator_dev *dev)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	struct tps65910 *mfd = pmic->mfd;
	int reg, id = rdev_get_id(dev);

	reg = pmic->get_ctrl_reg(id);
	if (reg < 0)
		return reg;

	return tps65910_clear_bits(mfd, reg, TPS65910_SUPPLY_STATE_ENABLED);
}

<<<<<<< HEAD
=======
static int tps65910_enable_time(struct regulator_dev *dev)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int id = rdev_get_id(dev);
	return pmic->info[id]->enable_time_us;
}
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
static int tps65910_set_mode(struct regulator_dev *dev, unsigned int mode)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	struct tps65910 *mfd = pmic->mfd;
	int reg, value, id = rdev_get_id(dev);

	reg = pmic->get_ctrl_reg(id);
	if (reg < 0)
		return reg;

	switch (mode) {
	case REGULATOR_MODE_NORMAL:
<<<<<<< HEAD
		return tps65910_modify_bits(pmic, reg, LDO_ST_ON_BIT,
							LDO_ST_MODE_BIT);
	case REGULATOR_MODE_IDLE:
		value = LDO_ST_ON_BIT | LDO_ST_MODE_BIT;
		return tps65910_set_bits(mfd, reg, value);
	case REGULATOR_MODE_STANDBY:
		return tps65910_clear_bits(mfd, reg, LDO_ST_ON_BIT);
=======
		return tps65910_reg_update_bits(pmic->mfd, reg,
						LDO_ST_MODE_BIT | LDO_ST_ON_BIT,
						LDO_ST_ON_BIT);
	case REGULATOR_MODE_IDLE:
		value = LDO_ST_ON_BIT | LDO_ST_MODE_BIT;
		return tps65910_reg_set_bits(mfd, reg, value);
	case REGULATOR_MODE_STANDBY:
		return tps65910_reg_clear_bits(mfd, reg, LDO_ST_ON_BIT);
>>>>>>> refs/remotes/origin/master
	}

	return -EINVAL;
}

static unsigned int tps65910_get_mode(struct regulator_dev *dev)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
<<<<<<< HEAD
	int reg, value, id = rdev_get_id(dev);
=======
	int ret, reg, value, id = rdev_get_id(dev);
>>>>>>> refs/remotes/origin/master

	reg = pmic->get_ctrl_reg(id);
	if (reg < 0)
		return reg;

<<<<<<< HEAD
	value = tps65910_reg_read(pmic, reg);
	if (value < 0)
		return value;

<<<<<<< HEAD
	if (value & LDO_ST_ON_BIT)
=======
	if (!(value & LDO_ST_ON_BIT))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = tps65910_reg_read(pmic->mfd, reg, &value);
	if (ret < 0)
		return ret;

	if (!(value & LDO_ST_ON_BIT))
>>>>>>> refs/remotes/origin/master
		return REGULATOR_MODE_STANDBY;
	else if (value & LDO_ST_MODE_BIT)
		return REGULATOR_MODE_IDLE;
	else
		return REGULATOR_MODE_NORMAL;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int tps65910_get_voltage_dcdc(struct regulator_dev *dev)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int id = rdev_get_id(dev), voltage = 0;
=======
static int tps65910_get_voltage_dcdc_sel(struct regulator_dev *dev)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int id = rdev_get_id(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int tps65910_get_voltage_dcdc_sel(struct regulator_dev *dev)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int ret, id = rdev_get_id(dev);
>>>>>>> refs/remotes/origin/master
	int opvsel = 0, srvsel = 0, vselmax = 0, mult = 0, sr = 0;

	switch (id) {
	case TPS65910_REG_VDD1:
<<<<<<< HEAD
		opvsel = tps65910_reg_read(pmic, TPS65910_VDD1_OP);
		mult = tps65910_reg_read(pmic, TPS65910_VDD1);
		mult = (mult & VDD1_VGAIN_SEL_MASK) >> VDD1_VGAIN_SEL_SHIFT;
		srvsel = tps65910_reg_read(pmic, TPS65910_VDD1_SR);
=======
		ret = tps65910_reg_read(pmic->mfd, TPS65910_VDD1_OP, &opvsel);
		if (ret < 0)
			return ret;
		ret = tps65910_reg_read(pmic->mfd, TPS65910_VDD1, &mult);
		if (ret < 0)
			return ret;
		mult = (mult & VDD1_VGAIN_SEL_MASK) >> VDD1_VGAIN_SEL_SHIFT;
		ret = tps65910_reg_read(pmic->mfd, TPS65910_VDD1_SR, &srvsel);
		if (ret < 0)
			return ret;
>>>>>>> refs/remotes/origin/master
		sr = opvsel & VDD1_OP_CMD_MASK;
		opvsel &= VDD1_OP_SEL_MASK;
		srvsel &= VDD1_SR_SEL_MASK;
		vselmax = 75;
		break;
	case TPS65910_REG_VDD2:
<<<<<<< HEAD
		opvsel = tps65910_reg_read(pmic, TPS65910_VDD2_OP);
		mult = tps65910_reg_read(pmic, TPS65910_VDD2);
		mult = (mult & VDD2_VGAIN_SEL_MASK) >> VDD2_VGAIN_SEL_SHIFT;
		srvsel = tps65910_reg_read(pmic, TPS65910_VDD2_SR);
=======
		ret = tps65910_reg_read(pmic->mfd, TPS65910_VDD2_OP, &opvsel);
		if (ret < 0)
			return ret;
		ret = tps65910_reg_read(pmic->mfd, TPS65910_VDD2, &mult);
		if (ret < 0)
			return ret;
		mult = (mult & VDD2_VGAIN_SEL_MASK) >> VDD2_VGAIN_SEL_SHIFT;
		ret = tps65910_reg_read(pmic->mfd, TPS65910_VDD2_SR, &srvsel);
		if (ret < 0)
			return ret;
>>>>>>> refs/remotes/origin/master
		sr = opvsel & VDD2_OP_CMD_MASK;
		opvsel &= VDD2_OP_SEL_MASK;
		srvsel &= VDD2_SR_SEL_MASK;
		vselmax = 75;
		break;
	case TPS65911_REG_VDDCTRL:
<<<<<<< HEAD
		opvsel = tps65910_reg_read(pmic, TPS65911_VDDCTRL_OP);
		srvsel = tps65910_reg_read(pmic, TPS65911_VDDCTRL_SR);
=======
		ret = tps65910_reg_read(pmic->mfd, TPS65911_VDDCTRL_OP,
					&opvsel);
		if (ret < 0)
			return ret;
		ret = tps65910_reg_read(pmic->mfd, TPS65911_VDDCTRL_SR,
					&srvsel);
		if (ret < 0)
			return ret;
>>>>>>> refs/remotes/origin/master
		sr = opvsel & VDDCTRL_OP_CMD_MASK;
		opvsel &= VDDCTRL_OP_SEL_MASK;
		srvsel &= VDDCTRL_SR_SEL_MASK;
		vselmax = 64;
		break;
	}

	/* multiplier 0 == 1 but 2,3 normal */
	if (!mult)
<<<<<<< HEAD
		mult=1;
=======
		mult = 1;
>>>>>>> refs/remotes/origin/master

	if (sr) {
		/* normalise to valid range */
		if (srvsel < 3)
			srvsel = 3;
		if (srvsel > vselmax)
			srvsel = vselmax;
<<<<<<< HEAD
<<<<<<< HEAD
		srvsel -= 3;

		voltage = (srvsel * VDD1_2_OFFSET + VDD1_2_MIN_VOLT) * 100;
=======
		return srvsel - 3;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return srvsel - 3;
>>>>>>> refs/remotes/origin/master
	} else {

		/* normalise to valid range*/
		if (opvsel < 3)
			opvsel = 3;
		if (opvsel > vselmax)
			opvsel = vselmax;
<<<<<<< HEAD
<<<<<<< HEAD
		opvsel -= 3;

		voltage = (opvsel * VDD1_2_OFFSET + VDD1_2_MIN_VOLT) * 100;
	}

	voltage *= mult;

	return voltage;
=======
		return opvsel - 3;
	}
	return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int tps65910_get_voltage(struct regulator_dev *dev)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int reg, value, id = rdev_get_id(dev), voltage = 0;
=======
		return opvsel - 3;
	}
	return -EINVAL;
}

static int tps65910_get_voltage_sel(struct regulator_dev *dev)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int ret, reg, value, id = rdev_get_id(dev);
>>>>>>> refs/remotes/origin/master

	reg = pmic->get_ctrl_reg(id);
	if (reg < 0)
		return reg;

<<<<<<< HEAD
	value = tps65910_reg_read(pmic, reg);
	if (value < 0)
		return value;
=======
	ret = tps65910_reg_read(pmic->mfd, reg, &value);
	if (ret < 0)
		return ret;
>>>>>>> refs/remotes/origin/master

	switch (id) {
	case TPS65910_REG_VIO:
	case TPS65910_REG_VDIG1:
	case TPS65910_REG_VDIG2:
	case TPS65910_REG_VPLL:
	case TPS65910_REG_VDAC:
	case TPS65910_REG_VAUX1:
	case TPS65910_REG_VAUX2:
	case TPS65910_REG_VAUX33:
	case TPS65910_REG_VMMC:
		value &= LDO_SEL_MASK;
		value >>= LDO_SEL_SHIFT;
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	voltage = pmic->info[id]->table[value] * 1000;
=======
	voltage = pmic->info[id]->voltage_table[value] * 1000;
>>>>>>> refs/remotes/origin/cm-10.0

	return voltage;
=======
	return value;
>>>>>>> refs/remotes/origin/master
}

static int tps65910_get_voltage_vdd3(struct regulator_dev *dev)
{
<<<<<<< HEAD
	return 5 * 1000 * 1000;
}

static int tps65911_get_voltage(struct regulator_dev *dev)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int step_mv, id = rdev_get_id(dev);
	u8 value, reg;

	reg = pmic->get_ctrl_reg(id);

	value = tps65910_reg_read(pmic, reg);
=======
	return dev->desc->volt_table[0];
}

static int tps65911_get_voltage_sel(struct regulator_dev *dev)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int ret, id = rdev_get_id(dev);
	unsigned int value, reg;

	reg = pmic->get_ctrl_reg(id);

	ret = tps65910_reg_read(pmic->mfd, reg, &value);
	if (ret < 0)
		return ret;
>>>>>>> refs/remotes/origin/master

	switch (id) {
	case TPS65911_REG_LDO1:
	case TPS65911_REG_LDO2:
	case TPS65911_REG_LDO4:
		value &= LDO1_SEL_MASK;
		value >>= LDO_SEL_SHIFT;
<<<<<<< HEAD
		/* The first 5 values of the selector correspond to 1V */
		if (value < 5)
			value = 0;
		else
			value -= 4;

		step_mv = 50;
=======
>>>>>>> refs/remotes/origin/master
		break;
	case TPS65911_REG_LDO3:
	case TPS65911_REG_LDO5:
	case TPS65911_REG_LDO6:
	case TPS65911_REG_LDO7:
	case TPS65911_REG_LDO8:
		value &= LDO3_SEL_MASK;
		value >>= LDO_SEL_SHIFT;
<<<<<<< HEAD
		/* The first 3 values of the selector correspond to 1V */
		if (value < 3)
			value = 0;
		else
			value -= 2;

		step_mv = 100;
		break;
	case TPS65910_REG_VIO:
<<<<<<< HEAD
		return pmic->info[id]->table[value] * 1000;
		break;
=======
		value &= LDO_SEL_MASK;
		value >>= LDO_SEL_SHIFT;
		return pmic->info[id]->voltage_table[value] * 1000;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		break;
	case TPS65910_REG_VIO:
		value &= LDO_SEL_MASK;
		value >>= LDO_SEL_SHIFT;
		break;
>>>>>>> refs/remotes/origin/master
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	return (LDO_MIN_VOLT + value * step_mv) * 1000;
}

<<<<<<< HEAD
static int tps65910_set_voltage_dcdc(struct regulator_dev *dev,
				unsigned selector)
=======
static int tps65910_set_voltage_dcdc_sel(struct regulator_dev *dev,
					 unsigned selector)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return value;
}

static int tps65910_set_voltage_dcdc_sel(struct regulator_dev *dev,
					 unsigned selector)
>>>>>>> refs/remotes/origin/master
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int id = rdev_get_id(dev), vsel;
	int dcdc_mult = 0;

	switch (id) {
	case TPS65910_REG_VDD1:
<<<<<<< HEAD
<<<<<<< HEAD
		dcdc_mult = (selector / VDD1_2_NUM_VOLTS) + 1;
		if (dcdc_mult == 1)
			dcdc_mult--;
		vsel = (selector % VDD1_2_NUM_VOLTS) + 3;
=======
=======
>>>>>>> refs/remotes/origin/master
		dcdc_mult = (selector / VDD1_2_NUM_VOLT_FINE) + 1;
		if (dcdc_mult == 1)
			dcdc_mult--;
		vsel = (selector % VDD1_2_NUM_VOLT_FINE) + 3;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

		tps65910_modify_bits(pmic, TPS65910_VDD1,
				(dcdc_mult << VDD1_VGAIN_SEL_SHIFT),
						VDD1_VGAIN_SEL_MASK);
		tps65910_reg_write(pmic, TPS65910_VDD1_OP, vsel);
		break;
	case TPS65910_REG_VDD2:
<<<<<<< HEAD
		dcdc_mult = (selector / VDD1_2_NUM_VOLTS) + 1;
		if (dcdc_mult == 1)
			dcdc_mult--;
		vsel = (selector % VDD1_2_NUM_VOLTS) + 3;
=======
=======

		tps65910_reg_update_bits(pmic->mfd, TPS65910_VDD1,
					 VDD1_VGAIN_SEL_MASK,
					 dcdc_mult << VDD1_VGAIN_SEL_SHIFT);
		tps65910_reg_write(pmic->mfd, TPS65910_VDD1_OP, vsel);
		break;
	case TPS65910_REG_VDD2:
>>>>>>> refs/remotes/origin/master
		dcdc_mult = (selector / VDD1_2_NUM_VOLT_FINE) + 1;
		if (dcdc_mult == 1)
			dcdc_mult--;
		vsel = (selector % VDD1_2_NUM_VOLT_FINE) + 3;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

		tps65910_modify_bits(pmic, TPS65910_VDD2,
				(dcdc_mult << VDD2_VGAIN_SEL_SHIFT),
						VDD1_VGAIN_SEL_MASK);
		tps65910_reg_write(pmic, TPS65910_VDD2_OP, vsel);
		break;
	case TPS65911_REG_VDDCTRL:
<<<<<<< HEAD
		vsel = selector;
=======
		vsel = selector + 3;
>>>>>>> refs/remotes/origin/cm-10.0
		tps65910_reg_write(pmic, TPS65911_VDDCTRL_OP, vsel);
=======

		tps65910_reg_update_bits(pmic->mfd, TPS65910_VDD2,
					 VDD1_VGAIN_SEL_MASK,
					 dcdc_mult << VDD2_VGAIN_SEL_SHIFT);
		tps65910_reg_write(pmic->mfd, TPS65910_VDD2_OP, vsel);
		break;
	case TPS65911_REG_VDDCTRL:
		vsel = selector + 3;
		tps65910_reg_write(pmic->mfd, TPS65911_VDDCTRL_OP, vsel);
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int tps65910_set_voltage(struct regulator_dev *dev, unsigned selector)
=======
static int tps65910_set_voltage_sel(struct regulator_dev *dev,
				    unsigned selector)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int tps65910_set_voltage_sel(struct regulator_dev *dev,
				    unsigned selector)
>>>>>>> refs/remotes/origin/master
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int reg, id = rdev_get_id(dev);

	reg = pmic->get_ctrl_reg(id);
	if (reg < 0)
		return reg;

	switch (id) {
	case TPS65910_REG_VIO:
	case TPS65910_REG_VDIG1:
	case TPS65910_REG_VDIG2:
	case TPS65910_REG_VPLL:
	case TPS65910_REG_VDAC:
	case TPS65910_REG_VAUX1:
	case TPS65910_REG_VAUX2:
	case TPS65910_REG_VAUX33:
	case TPS65910_REG_VMMC:
<<<<<<< HEAD
		return tps65910_modify_bits(pmic, reg,
				(selector << LDO_SEL_SHIFT), LDO_SEL_MASK);
=======
		return tps65910_reg_update_bits(pmic->mfd, reg, LDO_SEL_MASK,
						selector << LDO_SEL_SHIFT);
>>>>>>> refs/remotes/origin/master
	}

	return -EINVAL;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int tps65911_set_voltage(struct regulator_dev *dev, unsigned selector)
=======
static int tps65911_set_voltage_sel(struct regulator_dev *dev,
				    unsigned selector)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int tps65911_set_voltage_sel(struct regulator_dev *dev,
				    unsigned selector)
>>>>>>> refs/remotes/origin/master
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int reg, id = rdev_get_id(dev);

	reg = pmic->get_ctrl_reg(id);
	if (reg < 0)
		return reg;

	switch (id) {
	case TPS65911_REG_LDO1:
	case TPS65911_REG_LDO2:
	case TPS65911_REG_LDO4:
<<<<<<< HEAD
		return tps65910_modify_bits(pmic, reg,
				(selector << LDO_SEL_SHIFT), LDO1_SEL_MASK);
=======
		return tps65910_reg_update_bits(pmic->mfd, reg, LDO1_SEL_MASK,
						selector << LDO_SEL_SHIFT);
>>>>>>> refs/remotes/origin/master
	case TPS65911_REG_LDO3:
	case TPS65911_REG_LDO5:
	case TPS65911_REG_LDO6:
	case TPS65911_REG_LDO7:
	case TPS65911_REG_LDO8:
<<<<<<< HEAD
<<<<<<< HEAD
	case TPS65910_REG_VIO:
		return tps65910_modify_bits(pmic, reg,
				(selector << LDO_SEL_SHIFT), LDO3_SEL_MASK);
=======
		return tps65910_modify_bits(pmic, reg,
				(selector << LDO_SEL_SHIFT), LDO3_SEL_MASK);
	case TPS65910_REG_VIO:
		return tps65910_modify_bits(pmic, reg,
				(selector << LDO_SEL_SHIFT), LDO_SEL_MASK);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return tps65910_reg_update_bits(pmic->mfd, reg, LDO3_SEL_MASK,
						selector << LDO_SEL_SHIFT);
	case TPS65910_REG_VIO:
		return tps65910_reg_update_bits(pmic->mfd, reg, LDO_SEL_MASK,
						selector << LDO_SEL_SHIFT);
>>>>>>> refs/remotes/origin/master
	}

	return -EINVAL;
}


static int tps65910_list_voltage_dcdc(struct regulator_dev *dev,
					unsigned selector)
{
	int volt, mult = 1, id = rdev_get_id(dev);

	switch (id) {
	case TPS65910_REG_VDD1:
	case TPS65910_REG_VDD2:
<<<<<<< HEAD
<<<<<<< HEAD
		mult = (selector / VDD1_2_NUM_VOLTS) + 1;
		volt = VDD1_2_MIN_VOLT +
				(selector % VDD1_2_NUM_VOLTS) * VDD1_2_OFFSET;
=======
		mult = (selector / VDD1_2_NUM_VOLT_FINE) + 1;
		volt = VDD1_2_MIN_VOLT +
				(selector % VDD1_2_NUM_VOLT_FINE) * VDD1_2_OFFSET;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		mult = (selector / VDD1_2_NUM_VOLT_FINE) + 1;
		volt = VDD1_2_MIN_VOLT +
			(selector % VDD1_2_NUM_VOLT_FINE) * VDD1_2_OFFSET;
>>>>>>> refs/remotes/origin/master
		break;
	case TPS65911_REG_VDDCTRL:
		volt = VDDCTRL_MIN_VOLT + (selector * VDDCTRL_OFFSET);
		break;
	default:
		BUG();
		return -EINVAL;
	}

	return  volt * 100 * mult;
}

<<<<<<< HEAD
static int tps65910_list_voltage(struct regulator_dev *dev,
					unsigned selector)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int id = rdev_get_id(dev), voltage;

	if (id < TPS65910_REG_VIO || id > TPS65910_REG_VMMC)
		return -EINVAL;

<<<<<<< HEAD
	if (selector >= pmic->info[id]->table_len)
		return -EINVAL;
	else
		voltage = pmic->info[id]->table[selector] * 1000;
=======
	if (selector >= pmic->info[id]->n_voltages)
		return -EINVAL;
	else
		voltage = pmic->info[id]->voltage_table[selector] * 1000;
>>>>>>> refs/remotes/origin/cm-10.0

	return voltage;
}

=======
>>>>>>> refs/remotes/origin/master
static int tps65911_list_voltage(struct regulator_dev *dev, unsigned selector)
{
	struct tps65910_reg *pmic = rdev_get_drvdata(dev);
	int step_mv = 0, id = rdev_get_id(dev);

<<<<<<< HEAD
	switch(id) {
=======
	switch (id) {
>>>>>>> refs/remotes/origin/master
	case TPS65911_REG_LDO1:
	case TPS65911_REG_LDO2:
	case TPS65911_REG_LDO4:
		/* The first 5 values of the selector correspond to 1V */
		if (selector < 5)
			selector = 0;
		else
			selector -= 4;

		step_mv = 50;
		break;
	case TPS65911_REG_LDO3:
	case TPS65911_REG_LDO5:
	case TPS65911_REG_LDO6:
	case TPS65911_REG_LDO7:
	case TPS65911_REG_LDO8:
		/* The first 3 values of the selector correspond to 1V */
		if (selector < 3)
			selector = 0;
		else
			selector -= 2;

		step_mv = 100;
		break;
	case TPS65910_REG_VIO:
<<<<<<< HEAD
<<<<<<< HEAD
		return pmic->info[id]->table[selector] * 1000;
=======
		return pmic->info[id]->voltage_table[selector] * 1000;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return pmic->info[id]->voltage_table[selector];
>>>>>>> refs/remotes/origin/master
	default:
		return -EINVAL;
	}

	return (LDO_MIN_VOLT + selector * step_mv) * 1000;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static int tps65910_set_voltage_dcdc_time_sel(struct regulator_dev *dev,
		unsigned int old_selector, unsigned int new_selector)
{
	int id = rdev_get_id(dev);
	int old_volt, new_volt;

	old_volt = tps65910_list_voltage_dcdc(dev, old_selector);
	if (old_volt < 0)
		return old_volt;

	new_volt = tps65910_list_voltage_dcdc(dev, new_selector);
	if (new_volt < 0)
		return new_volt;

	/* VDD1 and VDD2 are 12.5mV/us, VDDCTRL is 100mV/20us */
	switch (id) {
	case TPS65910_REG_VDD1:
	case TPS65910_REG_VDD2:
		return DIV_ROUND_UP(abs(old_volt - new_volt), 12500);
	case TPS65911_REG_VDDCTRL:
		return DIV_ROUND_UP(abs(old_volt - new_volt), 5000);
	}
	return -EINVAL;
}

>>>>>>> refs/remotes/origin/cm-10.0
/* Regulator ops (except VRTC) */
static struct regulator_ops tps65910_ops_dcdc = {
	.is_enabled		= tps65910_is_enabled,
	.enable			= tps65910_enable,
	.disable		= tps65910_disable,
<<<<<<< HEAD
	.set_mode		= tps65910_set_mode,
	.get_mode		= tps65910_get_mode,
	.get_voltage		= tps65910_get_voltage_dcdc,
	.set_voltage_sel	= tps65910_set_voltage_dcdc,
=======
	.enable_time		= tps65910_enable_time,
=======
/* Regulator ops (except VRTC) */
static struct regulator_ops tps65910_ops_dcdc = {
	.is_enabled		= regulator_is_enabled_regmap,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
>>>>>>> refs/remotes/origin/master
	.set_mode		= tps65910_set_mode,
	.get_mode		= tps65910_get_mode,
	.get_voltage_sel	= tps65910_get_voltage_dcdc_sel,
	.set_voltage_sel	= tps65910_set_voltage_dcdc_sel,
<<<<<<< HEAD
	.set_voltage_time_sel	= tps65910_set_voltage_dcdc_time_sel,
>>>>>>> refs/remotes/origin/cm-10.0
	.list_voltage		= tps65910_list_voltage_dcdc,
};

static struct regulator_ops tps65910_ops_vdd3 = {
	.is_enabled		= tps65910_is_enabled,
	.enable			= tps65910_enable,
	.disable		= tps65910_disable,
<<<<<<< HEAD
=======
	.enable_time		= tps65910_enable_time,
>>>>>>> refs/remotes/origin/cm-10.0
	.set_mode		= tps65910_set_mode,
	.get_mode		= tps65910_get_mode,
	.get_voltage		= tps65910_get_voltage_vdd3,
	.list_voltage		= tps65910_list_voltage,
};

static struct regulator_ops tps65910_ops = {
	.is_enabled		= tps65910_is_enabled,
	.enable			= tps65910_enable,
	.disable		= tps65910_disable,
<<<<<<< HEAD
	.set_mode		= tps65910_set_mode,
	.get_mode		= tps65910_get_mode,
	.get_voltage		= tps65910_get_voltage,
	.set_voltage_sel	= tps65910_set_voltage,
=======
	.enable_time		= tps65910_enable_time,
	.set_mode		= tps65910_set_mode,
	.get_mode		= tps65910_get_mode,
	.get_voltage		= tps65910_get_voltage,
	.set_voltage_sel	= tps65910_set_voltage_sel,
>>>>>>> refs/remotes/origin/cm-10.0
	.list_voltage		= tps65910_list_voltage,
};

static struct regulator_ops tps65911_ops = {
	.is_enabled		= tps65910_is_enabled,
	.enable			= tps65910_enable,
	.disable		= tps65910_disable,
<<<<<<< HEAD
	.set_mode		= tps65910_set_mode,
	.get_mode		= tps65910_get_mode,
	.get_voltage		= tps65911_get_voltage,
	.set_voltage_sel	= tps65911_set_voltage,
	.list_voltage		= tps65911_list_voltage,
};

=======
	.enable_time		= tps65910_enable_time,
	.set_mode		= tps65910_set_mode,
	.get_mode		= tps65910_get_mode,
	.get_voltage		= tps65911_get_voltage,
	.set_voltage_sel	= tps65911_set_voltage_sel,
	.list_voltage		= tps65911_list_voltage,
=======
	.set_voltage_time_sel	= regulator_set_voltage_time_sel,
	.list_voltage		= tps65910_list_voltage_dcdc,
	.map_voltage		= regulator_map_voltage_ascend,
};

static struct regulator_ops tps65910_ops_vdd3 = {
	.is_enabled		= regulator_is_enabled_regmap,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.set_mode		= tps65910_set_mode,
	.get_mode		= tps65910_get_mode,
	.get_voltage		= tps65910_get_voltage_vdd3,
	.list_voltage		= regulator_list_voltage_table,
	.map_voltage		= regulator_map_voltage_ascend,
};

static struct regulator_ops tps65910_ops = {
	.is_enabled		= regulator_is_enabled_regmap,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.set_mode		= tps65910_set_mode,
	.get_mode		= tps65910_get_mode,
	.get_voltage_sel	= tps65910_get_voltage_sel,
	.set_voltage_sel	= tps65910_set_voltage_sel,
	.list_voltage		= regulator_list_voltage_table,
	.map_voltage		= regulator_map_voltage_ascend,
};

static struct regulator_ops tps65911_ops = {
	.is_enabled		= regulator_is_enabled_regmap,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.set_mode		= tps65910_set_mode,
	.get_mode		= tps65910_get_mode,
	.get_voltage_sel	= tps65911_get_voltage_sel,
	.set_voltage_sel	= tps65911_set_voltage_sel,
	.list_voltage		= tps65911_list_voltage,
	.map_voltage		= regulator_map_voltage_ascend,
>>>>>>> refs/remotes/origin/master
};

static int tps65910_set_ext_sleep_config(struct tps65910_reg *pmic,
		int id, int ext_sleep_config)
{
	struct tps65910 *mfd = pmic->mfd;
	u8 regoffs = (pmic->ext_sleep_control[id] >> 8) & 0xFF;
	u8 bit_pos = (1 << pmic->ext_sleep_control[id] & 0xFF);
	int ret;

	/*
	 * Regulator can not be control from multiple external input EN1, EN2
	 * and EN3 together.
	 */
	if (ext_sleep_config & EXT_SLEEP_CONTROL) {
		int en_count;
		en_count = ((ext_sleep_config &
				TPS65910_SLEEP_CONTROL_EXT_INPUT_EN1) != 0);
		en_count += ((ext_sleep_config &
				TPS65910_SLEEP_CONTROL_EXT_INPUT_EN2) != 0);
		en_count += ((ext_sleep_config &
				TPS65910_SLEEP_CONTROL_EXT_INPUT_EN3) != 0);
		en_count += ((ext_sleep_config &
				TPS65911_SLEEP_CONTROL_EXT_INPUT_SLEEP) != 0);
		if (en_count > 1) {
			dev_err(mfd->dev,
				"External sleep control flag is not proper\n");
			return -EINVAL;
		}
	}

	pmic->board_ext_control[id] = ext_sleep_config;

	/* External EN1 control */
	if (ext_sleep_config & TPS65910_SLEEP_CONTROL_EXT_INPUT_EN1)
<<<<<<< HEAD
		ret = tps65910_set_bits(mfd,
				TPS65910_EN1_LDO_ASS + regoffs, bit_pos);
	else
		ret = tps65910_clear_bits(mfd,
=======
		ret = tps65910_reg_set_bits(mfd,
				TPS65910_EN1_LDO_ASS + regoffs, bit_pos);
	else
		ret = tps65910_reg_clear_bits(mfd,
>>>>>>> refs/remotes/origin/master
				TPS65910_EN1_LDO_ASS + regoffs, bit_pos);
	if (ret < 0) {
		dev_err(mfd->dev,
			"Error in configuring external control EN1\n");
		return ret;
	}

	/* External EN2 control */
	if (ext_sleep_config & TPS65910_SLEEP_CONTROL_EXT_INPUT_EN2)
<<<<<<< HEAD
		ret = tps65910_set_bits(mfd,
				TPS65910_EN2_LDO_ASS + regoffs, bit_pos);
	else
		ret = tps65910_clear_bits(mfd,
=======
		ret = tps65910_reg_set_bits(mfd,
				TPS65910_EN2_LDO_ASS + regoffs, bit_pos);
	else
		ret = tps65910_reg_clear_bits(mfd,
>>>>>>> refs/remotes/origin/master
				TPS65910_EN2_LDO_ASS + regoffs, bit_pos);
	if (ret < 0) {
		dev_err(mfd->dev,
			"Error in configuring external control EN2\n");
		return ret;
	}

	/* External EN3 control for TPS65910 LDO only */
	if ((tps65910_chip_id(mfd) == TPS65910) &&
			(id >= TPS65910_REG_VDIG1)) {
		if (ext_sleep_config & TPS65910_SLEEP_CONTROL_EXT_INPUT_EN3)
<<<<<<< HEAD
			ret = tps65910_set_bits(mfd,
				TPS65910_EN3_LDO_ASS + regoffs, bit_pos);
		else
			ret = tps65910_clear_bits(mfd,
=======
			ret = tps65910_reg_set_bits(mfd,
				TPS65910_EN3_LDO_ASS + regoffs, bit_pos);
		else
			ret = tps65910_reg_clear_bits(mfd,
>>>>>>> refs/remotes/origin/master
				TPS65910_EN3_LDO_ASS + regoffs, bit_pos);
		if (ret < 0) {
			dev_err(mfd->dev,
				"Error in configuring external control EN3\n");
			return ret;
		}
	}

	/* Return if no external control is selected */
	if (!(ext_sleep_config & EXT_SLEEP_CONTROL)) {
		/* Clear all sleep controls */
<<<<<<< HEAD
		ret = tps65910_clear_bits(mfd,
			TPS65910_SLEEP_KEEP_LDO_ON + regoffs, bit_pos);
		if (!ret)
			ret = tps65910_clear_bits(mfd,
=======
		ret = tps65910_reg_clear_bits(mfd,
			TPS65910_SLEEP_KEEP_LDO_ON + regoffs, bit_pos);
		if (!ret)
			ret = tps65910_reg_clear_bits(mfd,
>>>>>>> refs/remotes/origin/master
				TPS65910_SLEEP_SET_LDO_OFF + regoffs, bit_pos);
		if (ret < 0)
			dev_err(mfd->dev,
				"Error in configuring SLEEP register\n");
		return ret;
	}

	/*
	 * For regulator that has separate operational and sleep register make
	 * sure that operational is used and clear sleep register to turn
	 * regulator off when external control is inactive
	 */
	if ((id == TPS65910_REG_VDD1) ||
		(id == TPS65910_REG_VDD2) ||
			((id == TPS65911_REG_VDDCTRL) &&
				(tps65910_chip_id(mfd) == TPS65911))) {
		int op_reg_add = pmic->get_ctrl_reg(id) + 1;
		int sr_reg_add = pmic->get_ctrl_reg(id) + 2;
<<<<<<< HEAD
		int opvsel = tps65910_reg_read(pmic, op_reg_add);
		int srvsel = tps65910_reg_read(pmic, sr_reg_add);
		if (opvsel & VDD1_OP_CMD_MASK) {
			u8 reg_val = srvsel & VDD1_OP_SEL_MASK;
			ret = tps65910_reg_write(pmic, op_reg_add, reg_val);
=======
		int opvsel, srvsel;

		ret = tps65910_reg_read(pmic->mfd, op_reg_add, &opvsel);
		if (ret < 0)
			return ret;
		ret = tps65910_reg_read(pmic->mfd, sr_reg_add, &srvsel);
		if (ret < 0)
			return ret;

		if (opvsel & VDD1_OP_CMD_MASK) {
			u8 reg_val = srvsel & VDD1_OP_SEL_MASK;

			ret = tps65910_reg_write(pmic->mfd, op_reg_add,
						 reg_val);
>>>>>>> refs/remotes/origin/master
			if (ret < 0) {
				dev_err(mfd->dev,
					"Error in configuring op register\n");
				return ret;
			}
		}
<<<<<<< HEAD
		ret = tps65910_reg_write(pmic, sr_reg_add, 0);
		if (ret < 0) {
			dev_err(mfd->dev, "Error in settting sr register\n");
=======
		ret = tps65910_reg_write(pmic->mfd, sr_reg_add, 0);
		if (ret < 0) {
			dev_err(mfd->dev, "Error in setting sr register\n");
>>>>>>> refs/remotes/origin/master
			return ret;
		}
	}

<<<<<<< HEAD
	ret = tps65910_clear_bits(mfd,
			TPS65910_SLEEP_KEEP_LDO_ON + regoffs, bit_pos);
	if (!ret) {
		if (ext_sleep_config & TPS65911_SLEEP_CONTROL_EXT_INPUT_SLEEP)
			ret = tps65910_set_bits(mfd,
				TPS65910_SLEEP_SET_LDO_OFF + regoffs, bit_pos);
		else
			ret = tps65910_clear_bits(mfd,
=======
	ret = tps65910_reg_clear_bits(mfd,
			TPS65910_SLEEP_KEEP_LDO_ON + regoffs, bit_pos);
	if (!ret) {
		if (ext_sleep_config & TPS65911_SLEEP_CONTROL_EXT_INPUT_SLEEP)
			ret = tps65910_reg_set_bits(mfd,
				TPS65910_SLEEP_SET_LDO_OFF + regoffs, bit_pos);
		else
			ret = tps65910_reg_clear_bits(mfd,
>>>>>>> refs/remotes/origin/master
				TPS65910_SLEEP_SET_LDO_OFF + regoffs, bit_pos);
	}
	if (ret < 0)
		dev_err(mfd->dev,
			"Error in configuring SLEEP register\n");

	return ret;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static __devinit int tps65910_probe(struct platform_device *pdev)
{
	struct tps65910 *tps65910 = dev_get_drvdata(pdev->dev.parent);
=======
#ifdef CONFIG_OF

static struct of_regulator_match tps65910_matches[] = {
	{ .name = "vrtc",	.driver_data = (void *) &tps65910_regs[0] },
	{ .name = "vio",	.driver_data = (void *) &tps65910_regs[1] },
	{ .name = "vdd1",	.driver_data = (void *) &tps65910_regs[2] },
	{ .name = "vdd2",	.driver_data = (void *) &tps65910_regs[3] },
	{ .name = "vdd3",	.driver_data = (void *) &tps65910_regs[4] },
	{ .name = "vdig1",	.driver_data = (void *) &tps65910_regs[5] },
	{ .name = "vdig2",	.driver_data = (void *) &tps65910_regs[6] },
	{ .name = "vpll",	.driver_data = (void *) &tps65910_regs[7] },
	{ .name = "vdac",	.driver_data = (void *) &tps65910_regs[8] },
	{ .name = "vaux1",	.driver_data = (void *) &tps65910_regs[9] },
	{ .name = "vaux2",	.driver_data = (void *) &tps65910_regs[10] },
	{ .name = "vaux33",	.driver_data = (void *) &tps65910_regs[11] },
	{ .name = "vmmc",	.driver_data = (void *) &tps65910_regs[12] },
};

static struct of_regulator_match tps65911_matches[] = {
	{ .name = "vrtc",	.driver_data = (void *) &tps65911_regs[0] },
	{ .name = "vio",	.driver_data = (void *) &tps65911_regs[1] },
	{ .name = "vdd1",	.driver_data = (void *) &tps65911_regs[2] },
	{ .name = "vdd2",	.driver_data = (void *) &tps65911_regs[3] },
	{ .name = "vddctrl",	.driver_data = (void *) &tps65911_regs[4] },
	{ .name = "ldo1",	.driver_data = (void *) &tps65911_regs[5] },
	{ .name = "ldo2",	.driver_data = (void *) &tps65911_regs[6] },
	{ .name = "ldo3",	.driver_data = (void *) &tps65911_regs[7] },
	{ .name = "ldo4",	.driver_data = (void *) &tps65911_regs[8] },
	{ .name = "ldo5",	.driver_data = (void *) &tps65911_regs[9] },
	{ .name = "ldo6",	.driver_data = (void *) &tps65911_regs[10] },
	{ .name = "ldo7",	.driver_data = (void *) &tps65911_regs[11] },
	{ .name = "ldo8",	.driver_data = (void *) &tps65911_regs[12] },
};

static struct tps65910_board *tps65910_parse_dt_reg_data(
		struct platform_device *pdev,
		struct of_regulator_match **tps65910_reg_matches)
{
	struct tps65910_board *pmic_plat_data;
	struct tps65910 *tps65910 = dev_get_drvdata(pdev->dev.parent);
	struct device_node *np, *regulators;
	struct of_regulator_match *matches;
	unsigned int prop;
	int idx = 0, ret, count;

	pmic_plat_data = devm_kzalloc(&pdev->dev, sizeof(*pmic_plat_data),
					GFP_KERNEL);

	if (!pmic_plat_data) {
		dev_err(&pdev->dev, "Failure to alloc pdata for regulators.\n");
		return NULL;
	}

	np = of_node_get(pdev->dev.parent->of_node);
	regulators = of_get_child_by_name(np, "regulators");
	if (!regulators) {
		dev_err(&pdev->dev, "regulator node not found\n");
		return NULL;
	}

	switch (tps65910_chip_id(tps65910)) {
	case TPS65910:
		count = ARRAY_SIZE(tps65910_matches);
		matches = tps65910_matches;
		break;
	case TPS65911:
		count = ARRAY_SIZE(tps65911_matches);
		matches = tps65911_matches;
		break;
	default:
		of_node_put(regulators);
		dev_err(&pdev->dev, "Invalid tps chip version\n");
		return NULL;
	}

	ret = of_regulator_match(&pdev->dev, regulators, matches, count);
	of_node_put(regulators);
	if (ret < 0) {
		dev_err(&pdev->dev, "Error parsing regulator init data: %d\n",
			ret);
		return NULL;
	}

	*tps65910_reg_matches = matches;

	for (idx = 0; idx < count; idx++) {
		if (!matches[idx].init_data || !matches[idx].of_node)
			continue;

		pmic_plat_data->tps65910_pmic_init_data[idx] =
							matches[idx].init_data;

		ret = of_property_read_u32(matches[idx].of_node,
				"ti,regulator-ext-sleep-control", &prop);
		if (!ret)
			pmic_plat_data->regulator_ext_sleep_control[idx] = prop;

	}

	return pmic_plat_data;
}
#else
static inline struct tps65910_board *tps65910_parse_dt_reg_data(
			struct platform_device *pdev,
			struct of_regulator_match **tps65910_reg_matches)
{
	*tps65910_reg_matches = NULL;
	return NULL;
}
#endif

static int tps65910_probe(struct platform_device *pdev)
{
	struct tps65910 *tps65910 = dev_get_drvdata(pdev->dev.parent);
	struct regulator_config config = { };
>>>>>>> refs/remotes/origin/master
	struct tps_info *info;
	struct regulator_init_data *reg_data;
	struct regulator_dev *rdev;
	struct tps65910_reg *pmic;
	struct tps65910_board *pmic_plat_data;
<<<<<<< HEAD
	int i, err;

	pmic_plat_data = dev_get_platdata(tps65910->dev);
	if (!pmic_plat_data)
		return -EINVAL;

<<<<<<< HEAD
	reg_data = pmic_plat_data->tps65910_pmic_init_data;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	pmic = kzalloc(sizeof(*pmic), GFP_KERNEL);
	if (!pmic)
		return -ENOMEM;

	mutex_init(&pmic->mutex);
=======
	struct of_regulator_match *tps65910_reg_matches = NULL;
	int i, err;

	pmic_plat_data = dev_get_platdata(tps65910->dev);
	if (!pmic_plat_data && tps65910->dev->of_node)
		pmic_plat_data = tps65910_parse_dt_reg_data(pdev,
						&tps65910_reg_matches);

	if (!pmic_plat_data) {
		dev_err(&pdev->dev, "Platform data not found\n");
		return -EINVAL;
	}

	pmic = devm_kzalloc(&pdev->dev, sizeof(*pmic), GFP_KERNEL);
	if (!pmic) {
		dev_err(&pdev->dev, "Memory allocation failed for pmic\n");
		return -ENOMEM;
	}

>>>>>>> refs/remotes/origin/master
	pmic->mfd = tps65910;
	platform_set_drvdata(pdev, pmic);

	/* Give control of all register to control port */
<<<<<<< HEAD
	tps65910_set_bits(pmic->mfd, TPS65910_DEVCTRL,
				DEVCTRL_SR_CTL_I2C_SEL_MASK);

	switch(tps65910_chip_id(tps65910)) {
	case TPS65910:
		pmic->get_ctrl_reg = &tps65910_get_ctrl_register;
<<<<<<< HEAD
=======
		pmic->num_regulators = ARRAY_SIZE(tps65910_regs);
		pmic->ext_sleep_control = tps65910_ext_sleep_control;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	tps65910_reg_set_bits(pmic->mfd, TPS65910_DEVCTRL,
				DEVCTRL_SR_CTL_I2C_SEL_MASK);

	switch (tps65910_chip_id(tps65910)) {
	case TPS65910:
		pmic->get_ctrl_reg = &tps65910_get_ctrl_register;
		pmic->num_regulators = ARRAY_SIZE(tps65910_regs);
		pmic->ext_sleep_control = tps65910_ext_sleep_control;
>>>>>>> refs/remotes/origin/master
		info = tps65910_regs;
		break;
	case TPS65911:
		pmic->get_ctrl_reg = &tps65911_get_ctrl_register;
<<<<<<< HEAD
<<<<<<< HEAD
=======
		pmic->num_regulators = ARRAY_SIZE(tps65911_regs);
		pmic->ext_sleep_control = tps65911_ext_sleep_control;
>>>>>>> refs/remotes/origin/cm-10.0
		info = tps65911_regs;
		break;
	default:
		pr_err("Invalid tps chip version\n");
<<<<<<< HEAD
		return -ENODEV;
	}

	for (i = 0; i < TPS65910_NUM_REGULATOR; i++, info++, reg_data++) {
=======
		kfree(pmic);
		return -ENODEV;
	}

	pmic->desc = kcalloc(pmic->num_regulators,
			sizeof(struct regulator_desc), GFP_KERNEL);
	if (!pmic->desc) {
		err = -ENOMEM;
		goto err_free_pmic;
	}

	pmic->info = kcalloc(pmic->num_regulators,
			sizeof(struct tps_info *), GFP_KERNEL);
	if (!pmic->info) {
		err = -ENOMEM;
		goto err_free_desc;
	}

	pmic->rdev = kcalloc(pmic->num_regulators,
			sizeof(struct regulator_dev *), GFP_KERNEL);
	if (!pmic->rdev) {
		err = -ENOMEM;
		goto err_free_info;
=======
		pmic->num_regulators = ARRAY_SIZE(tps65911_regs);
		pmic->ext_sleep_control = tps65911_ext_sleep_control;
		info = tps65911_regs;
		break;
	default:
		dev_err(&pdev->dev, "Invalid tps chip version\n");
		return -ENODEV;
	}

	pmic->desc = devm_kzalloc(&pdev->dev, pmic->num_regulators *
			sizeof(struct regulator_desc), GFP_KERNEL);
	if (!pmic->desc) {
		dev_err(&pdev->dev, "Memory alloc fails for desc\n");
		return -ENOMEM;
	}

	pmic->info = devm_kzalloc(&pdev->dev, pmic->num_regulators *
			sizeof(struct tps_info *), GFP_KERNEL);
	if (!pmic->info) {
		dev_err(&pdev->dev, "Memory alloc fails for info\n");
		return -ENOMEM;
	}

	pmic->rdev = devm_kzalloc(&pdev->dev, pmic->num_regulators *
			sizeof(struct regulator_dev *), GFP_KERNEL);
	if (!pmic->rdev) {
		dev_err(&pdev->dev, "Memory alloc fails for rdev\n");
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master
	}

	for (i = 0; i < pmic->num_regulators && i < TPS65910_NUM_REGS;
			i++, info++) {

		reg_data = pmic_plat_data->tps65910_pmic_init_data[i];

		/* Regulator API handles empty constraints but not NULL
		 * constraints */
		if (!reg_data)
			continue;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		/* Register the regulators */
		pmic->info[i] = info;

		pmic->desc[i].name = info->name;
<<<<<<< HEAD
		pmic->desc[i].id = i;
<<<<<<< HEAD
		pmic->desc[i].n_voltages = info->table_len;

		if (i == TPS65910_REG_VDD1 || i == TPS65910_REG_VDD2) {
			pmic->desc[i].ops = &tps65910_ops_dcdc;
=======
		pmic->desc[i].n_voltages = info->n_voltages;
=======
		pmic->desc[i].supply_name = info->vin_name;
		pmic->desc[i].id = i;
		pmic->desc[i].n_voltages = info->n_voltages;
		pmic->desc[i].enable_time = info->enable_time_us;
>>>>>>> refs/remotes/origin/master

		if (i == TPS65910_REG_VDD1 || i == TPS65910_REG_VDD2) {
			pmic->desc[i].ops = &tps65910_ops_dcdc;
			pmic->desc[i].n_voltages = VDD1_2_NUM_VOLT_FINE *
							VDD1_2_NUM_VOLT_COARSE;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		} else if (i == TPS65910_REG_VDD3) {
			if (tps65910_chip_id(tps65910) == TPS65910)
				pmic->desc[i].ops = &tps65910_ops_vdd3;
			else
				pmic->desc[i].ops = &tps65910_ops_dcdc;
		} else {
			if (tps65910_chip_id(tps65910) == TPS65910)
				pmic->desc[i].ops = &tps65910_ops;
			else
				pmic->desc[i].ops = &tps65911_ops;
		}

<<<<<<< HEAD
=======
=======
			pmic->desc[i].ramp_delay = 12500;
		} else if (i == TPS65910_REG_VDD3) {
			if (tps65910_chip_id(tps65910) == TPS65910) {
				pmic->desc[i].ops = &tps65910_ops_vdd3;
				pmic->desc[i].volt_table = info->voltage_table;
			} else {
				pmic->desc[i].ops = &tps65910_ops_dcdc;
				pmic->desc[i].ramp_delay = 5000;
			}
		} else {
			if (tps65910_chip_id(tps65910) == TPS65910) {
				pmic->desc[i].ops = &tps65910_ops;
				pmic->desc[i].volt_table = info->voltage_table;
			} else {
				pmic->desc[i].ops = &tps65911_ops;
			}
		}

>>>>>>> refs/remotes/origin/master
		err = tps65910_set_ext_sleep_config(pmic, i,
				pmic_plat_data->regulator_ext_sleep_control[i]);
		/*
		 * Failing on regulator for configuring externally control
		 * is not a serious issue, just throw warning.
		 */
		if (err < 0)
			dev_warn(tps65910->dev,
				"Failed to initialise ext control config\n");

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		pmic->desc[i].type = REGULATOR_VOLTAGE;
		pmic->desc[i].owner = THIS_MODULE;

		rdev = regulator_register(&pmic->desc[i],
<<<<<<< HEAD
				tps65910->dev, reg_data, pmic);
=======
				tps65910->dev, reg_data, pmic, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pmic->desc[i].type = REGULATOR_VOLTAGE;
		pmic->desc[i].owner = THIS_MODULE;
		pmic->desc[i].enable_reg = pmic->get_ctrl_reg(i);
		pmic->desc[i].enable_mask = TPS65910_SUPPLY_STATE_ENABLED;

		config.dev = tps65910->dev;
		config.init_data = reg_data;
		config.driver_data = pmic;
		config.regmap = tps65910->regmap;

		if (tps65910_reg_matches)
			config.of_node = tps65910_reg_matches[i].of_node;

		rdev = devm_regulator_register(&pdev->dev, &pmic->desc[i],
					       &config);
>>>>>>> refs/remotes/origin/master
		if (IS_ERR(rdev)) {
			dev_err(tps65910->dev,
				"failed to register %s regulator\n",
				pdev->name);
<<<<<<< HEAD
			err = PTR_ERR(rdev);
<<<<<<< HEAD
			goto err;
=======
			goto err_unregister_regulator;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			return PTR_ERR(rdev);
>>>>>>> refs/remotes/origin/master
		}

		/* Save regulator for cleanup */
		pmic->rdev[i] = rdev;
	}
	return 0;
<<<<<<< HEAD

<<<<<<< HEAD
err:
	while (--i >= 0)
		regulator_unregister(pmic->rdev[i]);

=======
err_unregister_regulator:
	while (--i >= 0)
		regulator_unregister(pmic->rdev[i]);
	kfree(pmic->rdev);
err_free_info:
	kfree(pmic->info);
err_free_desc:
	kfree(pmic->desc);
err_free_pmic:
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(pmic);
	return err;
}

static int __devexit tps65910_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct tps65910_reg *tps65910_reg = platform_get_drvdata(pdev);
	int i;

	for (i = 0; i < TPS65910_NUM_REGULATOR; i++)
		regulator_unregister(tps65910_reg->rdev[i]);

	kfree(tps65910_reg);
	return 0;
}

=======
	struct tps65910_reg *pmic = platform_get_drvdata(pdev);
	int i;

	for (i = 0; i < pmic->num_regulators; i++)
		regulator_unregister(pmic->rdev[i]);

	kfree(pmic->rdev);
	kfree(pmic->info);
	kfree(pmic->desc);
	kfree(pmic);
	return 0;
=======
>>>>>>> refs/remotes/origin/master
}

static void tps65910_shutdown(struct platform_device *pdev)
{
	struct tps65910_reg *pmic = platform_get_drvdata(pdev);
	int i;

	/*
	 * Before bootloader jumps to kernel, it makes sure that required
	 * external control signals are in desired state so that given rails
	 * can be configure accordingly.
	 * If rails are configured to be controlled from external control
	 * then before shutting down/rebooting the system, the external
	 * control configuration need to be remove from the rails so that
	 * its output will be available as per register programming even
	 * if external controls are removed. This is require when the POR
	 * value of the control signals are not in active state and before
	 * bootloader initializes it, the system requires the rail output
	 * to be active for booting.
	 */
	for (i = 0; i < pmic->num_regulators; i++) {
		int err;
		if (!pmic->rdev[i])
			continue;

		err = tps65910_set_ext_sleep_config(pmic, i, 0);
		if (err < 0)
			dev_err(&pdev->dev,
				"Error in clearing external control\n");
	}
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct platform_driver tps65910_driver = {
	.driver = {
		.name = "tps65910-pmic",
		.owner = THIS_MODULE,
	},
	.probe = tps65910_probe,
<<<<<<< HEAD
	.remove = __devexit_p(tps65910_remove),
<<<<<<< HEAD
=======
	.shutdown = tps65910_shutdown,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.shutdown = tps65910_shutdown,
>>>>>>> refs/remotes/origin/master
};

static int __init tps65910_init(void)
{
	return platform_driver_register(&tps65910_driver);
}
subsys_initcall(tps65910_init);

static void __exit tps65910_cleanup(void)
{
	platform_driver_unregister(&tps65910_driver);
}
module_exit(tps65910_cleanup);

MODULE_AUTHOR("Graeme Gregory <gg@slimlogic.co.uk>");
<<<<<<< HEAD
<<<<<<< HEAD
MODULE_DESCRIPTION("TPS6507x voltage regulator driver");
=======
MODULE_DESCRIPTION("TPS65910/TPS65911 voltage regulator driver");
>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_DESCRIPTION("TPS65910/TPS65911 voltage regulator driver");
>>>>>>> refs/remotes/origin/master
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:tps65910-pmic");
