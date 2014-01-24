/*
 * OMAP4 Voltage Controller (VC) data
 *
 * Copyright (C) 2007, 2010 Texas Instruments, Inc.
 * Rajendra Nayak <rnayak@ti.com>
 * Lesly A M <x0080970@ti.com>
 * Thara Gopinath <thara@ti.com>
 *
 * Copyright (C) 2008, 2011 Nokia Corporation
 * Kalle Jokiniemi
 * Paul Walmsley
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/io.h>
#include <linux/err.h>
#include <linux/init.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <plat/common.h>
=======
#include "common.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "common.h"
>>>>>>> refs/remotes/origin/master

#include "prm44xx.h"
#include "prm-regbits-44xx.h"
#include "voltage.h"

#include "vc.h"

/*
 * VC data common to 44xx chips
 * XXX This stuff presumably belongs in the vc3xxx.c or vc.c file.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static const struct omap_vc_common_data omap4_vc_common = {
	.smps_sa_reg = OMAP4_PRM_VC_SMPS_SA_OFFSET,
	.smps_volra_reg = OMAP4_PRM_VC_VAL_SMPS_RA_VOL_OFFSET,
=======
static const struct omap_vc_common omap4_vc_common = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct omap_vc_common omap4_vc_common = {
>>>>>>> refs/remotes/origin/master
	.bypass_val_reg = OMAP4_PRM_VC_VAL_BYPASS_OFFSET,
	.data_shift = OMAP4430_DATA_SHIFT,
	.slaveaddr_shift = OMAP4430_SLAVEADDR_SHIFT,
	.regaddr_shift = OMAP4430_REGADDR_SHIFT,
	.valid = OMAP4430_VALID_MASK,
	.cmd_on_shift = OMAP4430_ON_SHIFT,
	.cmd_on_mask = OMAP4430_ON_MASK,
	.cmd_onlp_shift = OMAP4430_ONLP_SHIFT,
	.cmd_ret_shift = OMAP4430_RET_SHIFT,
	.cmd_off_shift = OMAP4430_OFF_SHIFT,
<<<<<<< HEAD
<<<<<<< HEAD
};

/* VC instance data for each controllable voltage line */
struct omap_vc_instance_data omap4_vc_mpu_data = {
	.vc_common = &omap4_vc_common,
	.cmdval_reg = OMAP4_PRM_VC_VAL_CMD_VDD_MPU_L_OFFSET,
	.smps_sa_shift = OMAP4430_SA_VDD_MPU_L_PRM_VC_SMPS_SA_SHIFT,
	.smps_sa_mask = OMAP4430_SA_VDD_MPU_L_PRM_VC_SMPS_SA_MASK,
	.smps_volra_shift = OMAP4430_VOLRA_VDD_MPU_L_SHIFT,
	.smps_volra_mask = OMAP4430_VOLRA_VDD_MPU_L_MASK,
};

struct omap_vc_instance_data omap4_vc_iva_data = {
	.vc_common = &omap4_vc_common,
	.cmdval_reg = OMAP4_PRM_VC_VAL_CMD_VDD_IVA_L_OFFSET,
	.smps_sa_shift = OMAP4430_SA_VDD_IVA_L_PRM_VC_SMPS_SA_SHIFT,
	.smps_sa_mask = OMAP4430_SA_VDD_IVA_L_PRM_VC_SMPS_SA_MASK,
	.smps_volra_shift = OMAP4430_VOLRA_VDD_IVA_L_SHIFT,
	.smps_volra_mask = OMAP4430_VOLRA_VDD_IVA_L_MASK,
};

struct omap_vc_instance_data omap4_vc_core_data = {
	.vc_common = &omap4_vc_common,
	.cmdval_reg = OMAP4_PRM_VC_VAL_CMD_VDD_CORE_L_OFFSET,
	.smps_sa_shift = OMAP4430_SA_VDD_CORE_L_0_6_SHIFT,
	.smps_sa_mask = OMAP4430_SA_VDD_CORE_L_0_6_MASK,
	.smps_volra_shift = OMAP4430_VOLRA_VDD_CORE_L_SHIFT,
	.smps_volra_mask = OMAP4430_VOLRA_VDD_CORE_L_MASK,
=======
=======
>>>>>>> refs/remotes/origin/master
	.i2c_cfg_reg = OMAP4_PRM_VC_CFG_I2C_MODE_OFFSET,
	.i2c_cfg_hsen_mask = OMAP4430_HSMODEEN_MASK,
	.i2c_mcode_mask	 = OMAP4430_HSMCODE_MASK,
};

/* VC instance data for each controllable voltage line */
struct omap_vc_channel omap4_vc_mpu = {
	.flags = OMAP_VC_CHANNEL_DEFAULT | OMAP_VC_CHANNEL_CFG_MUTANT,
	.common = &omap4_vc_common,
	.smps_sa_reg = OMAP4_PRM_VC_SMPS_SA_OFFSET,
	.smps_volra_reg = OMAP4_PRM_VC_VAL_SMPS_RA_VOL_OFFSET,
	.smps_cmdra_reg = OMAP4_PRM_VC_VAL_SMPS_RA_CMD_OFFSET,
	.cfg_channel_reg = OMAP4_PRM_VC_CFG_CHANNEL_OFFSET,
	.cmdval_reg = OMAP4_PRM_VC_VAL_CMD_VDD_MPU_L_OFFSET,
	.smps_sa_mask = OMAP4430_SA_VDD_MPU_L_PRM_VC_SMPS_SA_MASK,
	.smps_volra_mask = OMAP4430_VOLRA_VDD_MPU_L_MASK,
	.smps_cmdra_mask = OMAP4430_CMDRA_VDD_MPU_L_MASK,
	.cfg_channel_sa_shift = OMAP4430_SA_VDD_MPU_L_SHIFT,
};

struct omap_vc_channel omap4_vc_iva = {
	.common = &omap4_vc_common,
	.smps_sa_reg = OMAP4_PRM_VC_SMPS_SA_OFFSET,
	.smps_volra_reg = OMAP4_PRM_VC_VAL_SMPS_RA_VOL_OFFSET,
	.smps_cmdra_reg = OMAP4_PRM_VC_VAL_SMPS_RA_CMD_OFFSET,
	.cfg_channel_reg = OMAP4_PRM_VC_CFG_CHANNEL_OFFSET,
	.cmdval_reg = OMAP4_PRM_VC_VAL_CMD_VDD_IVA_L_OFFSET,
	.smps_sa_mask = OMAP4430_SA_VDD_IVA_L_PRM_VC_SMPS_SA_MASK,
	.smps_volra_mask = OMAP4430_VOLRA_VDD_IVA_L_MASK,
	.smps_cmdra_mask = OMAP4430_CMDRA_VDD_IVA_L_MASK,
	.cfg_channel_sa_shift = OMAP4430_SA_VDD_IVA_L_SHIFT,
};

struct omap_vc_channel omap4_vc_core = {
	.common = &omap4_vc_common,
	.smps_sa_reg = OMAP4_PRM_VC_SMPS_SA_OFFSET,
	.smps_volra_reg = OMAP4_PRM_VC_VAL_SMPS_RA_VOL_OFFSET,
	.smps_cmdra_reg = OMAP4_PRM_VC_VAL_SMPS_RA_CMD_OFFSET,
	.cfg_channel_reg = OMAP4_PRM_VC_CFG_CHANNEL_OFFSET,
	.cmdval_reg = OMAP4_PRM_VC_VAL_CMD_VDD_CORE_L_OFFSET,
	.smps_sa_mask = OMAP4430_SA_VDD_CORE_L_0_6_MASK,
	.smps_volra_mask = OMAP4430_VOLRA_VDD_CORE_L_MASK,
	.smps_cmdra_mask = OMAP4430_CMDRA_VDD_CORE_L_MASK,
	.cfg_channel_sa_shift = OMAP4430_SA_VDD_CORE_L_SHIFT,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
};

=======
};

/*
 * Voltage levels for different operating modes: on, sleep, retention and off
 */
#define OMAP4_ON_VOLTAGE_UV			1375000
#define OMAP4_ONLP_VOLTAGE_UV			1375000
#define OMAP4_RET_VOLTAGE_UV			837500
#define OMAP4_OFF_VOLTAGE_UV			0

struct omap_vc_param omap4_mpu_vc_data = {
	.on			= OMAP4_ON_VOLTAGE_UV,
	.onlp			= OMAP4_ONLP_VOLTAGE_UV,
	.ret			= OMAP4_RET_VOLTAGE_UV,
	.off			= OMAP4_OFF_VOLTAGE_UV,
};

struct omap_vc_param omap4_iva_vc_data = {
	.on			= OMAP4_ON_VOLTAGE_UV,
	.onlp			= OMAP4_ONLP_VOLTAGE_UV,
	.ret			= OMAP4_RET_VOLTAGE_UV,
	.off			= OMAP4_OFF_VOLTAGE_UV,
};

struct omap_vc_param omap4_core_vc_data = {
	.on			= OMAP4_ON_VOLTAGE_UV,
	.onlp			= OMAP4_ONLP_VOLTAGE_UV,
	.ret			= OMAP4_RET_VOLTAGE_UV,
	.off			= OMAP4_OFF_VOLTAGE_UV,
};
>>>>>>> refs/remotes/origin/master
