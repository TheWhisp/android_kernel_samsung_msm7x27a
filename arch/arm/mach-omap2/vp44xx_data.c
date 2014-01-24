/*
 * OMAP3 Voltage Processor (VP) data
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

#include "vp.h"

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static const struct omap_vp_ops omap4_vp_ops = {
	.check_txdone = omap4_prm_vp_check_txdone,
	.clear_txdone = omap4_prm_vp_clear_txdone,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * VP data common to 44xx chips
 * XXX This stuff presumably belongs in the vp44xx.c or vp.c file.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static const struct omap_vp_common_data omap4_vp_common = {
	.vpconfig_erroroffset_shift = OMAP4430_ERROROFFSET_SHIFT,
	.vpconfig_errorgain_mask = OMAP4430_ERRORGAIN_MASK,
	.vpconfig_errorgain_shift = OMAP4430_ERRORGAIN_SHIFT,
	.vpconfig_initvoltage_shift = OMAP4430_INITVOLTAGE_SHIFT,
=======
static const struct omap_vp_common omap4_vp_common = {
	.vpconfig_erroroffset_mask = OMAP4430_ERROROFFSET_MASK,
	.vpconfig_errorgain_mask = OMAP4430_ERRORGAIN_MASK,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct omap_vp_common omap4_vp_common = {
	.vpconfig_erroroffset_mask = OMAP4430_ERROROFFSET_MASK,
	.vpconfig_errorgain_mask = OMAP4430_ERRORGAIN_MASK,
>>>>>>> refs/remotes/origin/master
	.vpconfig_initvoltage_mask = OMAP4430_INITVOLTAGE_MASK,
	.vpconfig_timeouten = OMAP4430_TIMEOUTEN_MASK,
	.vpconfig_initvdd = OMAP4430_INITVDD_MASK,
	.vpconfig_forceupdate = OMAP4430_FORCEUPDATE_MASK,
	.vpconfig_vpenable = OMAP4430_VPENABLE_MASK,
	.vstepmin_smpswaittimemin_shift = OMAP4430_SMPSWAITTIMEMIN_SHIFT,
	.vstepmax_smpswaittimemax_shift = OMAP4430_SMPSWAITTIMEMAX_SHIFT,
	.vstepmin_stepmin_shift = OMAP4430_VSTEPMIN_SHIFT,
	.vstepmax_stepmax_shift = OMAP4430_VSTEPMAX_SHIFT,
	.vlimitto_vddmin_shift = OMAP4430_VDDMIN_SHIFT,
	.vlimitto_vddmax_shift = OMAP4430_VDDMAX_SHIFT,
	.vlimitto_timeout_shift = OMAP4430_TIMEOUT_SHIFT,
<<<<<<< HEAD
<<<<<<< HEAD
};

static const struct omap_vp_prm_irqst_data omap4_vp_mpu_prm_irqst_data = {
	.prm_irqst_reg = OMAP4_PRM_IRQSTATUS_MPU_2_OFFSET,
	.tranxdone_status = OMAP4430_VP_MPU_TRANXDONE_ST_MASK,
};

struct omap_vp_instance_data omap4_vp_mpu_data = {
	.vp_common = &omap4_vp_common,
=======
=======
>>>>>>> refs/remotes/origin/master
	.vpvoltage_mask = OMAP4430_VPVOLTAGE_MASK,
	.ops = &omap4_vp_ops,
};

struct omap_vp_instance omap4_vp_mpu = {
	.id = OMAP4_VP_VDD_MPU_ID,
	.common = &omap4_vp_common,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.vpconfig = OMAP4_PRM_VP_MPU_CONFIG_OFFSET,
	.vstepmin = OMAP4_PRM_VP_MPU_VSTEPMIN_OFFSET,
	.vstepmax = OMAP4_PRM_VP_MPU_VSTEPMAX_OFFSET,
	.vlimitto = OMAP4_PRM_VP_MPU_VLIMITTO_OFFSET,
	.vstatus = OMAP4_PRM_VP_MPU_STATUS_OFFSET,
	.voltage = OMAP4_PRM_VP_MPU_VOLTAGE_OFFSET,
<<<<<<< HEAD
<<<<<<< HEAD
	.prm_irqst_data = &omap4_vp_mpu_prm_irqst_data,
};

static const struct omap_vp_prm_irqst_data omap4_vp_iva_prm_irqst_data = {
	.prm_irqst_reg = OMAP4_PRM_IRQSTATUS_MPU_OFFSET,
	.tranxdone_status = OMAP4430_VP_IVA_TRANXDONE_ST_MASK,
};

struct omap_vp_instance_data omap4_vp_iva_data = {
	.vp_common = &omap4_vp_common,
=======
=======
>>>>>>> refs/remotes/origin/master
};

struct omap_vp_instance omap4_vp_iva = {
	.id = OMAP4_VP_VDD_IVA_ID,
	.common = &omap4_vp_common,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.vpconfig = OMAP4_PRM_VP_IVA_CONFIG_OFFSET,
	.vstepmin = OMAP4_PRM_VP_IVA_VSTEPMIN_OFFSET,
	.vstepmax = OMAP4_PRM_VP_IVA_VSTEPMAX_OFFSET,
	.vlimitto = OMAP4_PRM_VP_IVA_VLIMITTO_OFFSET,
	.vstatus = OMAP4_PRM_VP_IVA_STATUS_OFFSET,
	.voltage = OMAP4_PRM_VP_IVA_VOLTAGE_OFFSET,
<<<<<<< HEAD
<<<<<<< HEAD
	.prm_irqst_data = &omap4_vp_iva_prm_irqst_data,
};

static const struct omap_vp_prm_irqst_data omap4_vp_core_prm_irqst_data = {
	.prm_irqst_reg = OMAP4_PRM_IRQSTATUS_MPU_OFFSET,
	.tranxdone_status = OMAP4430_VP_CORE_TRANXDONE_ST_MASK,
};

struct omap_vp_instance_data omap4_vp_core_data = {
	.vp_common = &omap4_vp_common,
=======
=======
>>>>>>> refs/remotes/origin/master
};

struct omap_vp_instance omap4_vp_core = {
	.id = OMAP4_VP_VDD_CORE_ID,
	.common = &omap4_vp_common,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.vpconfig = OMAP4_PRM_VP_CORE_CONFIG_OFFSET,
	.vstepmin = OMAP4_PRM_VP_CORE_VSTEPMIN_OFFSET,
	.vstepmax = OMAP4_PRM_VP_CORE_VSTEPMAX_OFFSET,
	.vlimitto = OMAP4_PRM_VP_CORE_VLIMITTO_OFFSET,
	.vstatus = OMAP4_PRM_VP_CORE_STATUS_OFFSET,
	.voltage = OMAP4_PRM_VP_CORE_VOLTAGE_OFFSET,
<<<<<<< HEAD
<<<<<<< HEAD
	.prm_irqst_data = &omap4_vp_core_prm_irqst_data,
};

=======
};
>>>>>>> refs/remotes/origin/cm-10.0
=======
};

struct omap_vp_param omap4_mpu_vp_data = {
	.vddmin			= OMAP4_VP_MPU_VLIMITTO_VDDMIN,
	.vddmax			= OMAP4_VP_MPU_VLIMITTO_VDDMAX,
};

struct omap_vp_param omap4_iva_vp_data = {
	.vddmin			= OMAP4_VP_IVA_VLIMITTO_VDDMIN,
	.vddmax			= OMAP4_VP_IVA_VLIMITTO_VDDMAX,
};

struct omap_vp_param omap4_core_vp_data = {
	.vddmin			= OMAP4_VP_CORE_VLIMITTO_VDDMIN,
	.vddmax			= OMAP4_VP_CORE_VLIMITTO_VDDMAX,
};
>>>>>>> refs/remotes/origin/master
