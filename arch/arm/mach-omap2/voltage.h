/*
 * OMAP Voltage Management Routines
 *
 * Author: Thara Gopinath	<thara@ti.com>
 *
 * Copyright (C) 2009 Texas Instruments, Inc.
 * Thara Gopinath <thara@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ARCH_ARM_MACH_OMAP2_VOLTAGE_H
#define __ARCH_ARM_MACH_OMAP2_VOLTAGE_H

#include <linux/err.h>

<<<<<<< HEAD
#include "vc.h"
#include "vp.h"

<<<<<<< HEAD
=======
struct powerdomain;

>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/platform_data/voltage-omap.h>

#include "vc.h"
#include "vp.h"

struct powerdomain;

>>>>>>> refs/remotes/origin/master
/* XXX document */
#define VOLTSCALE_VPFORCEUPDATE		1
#define VOLTSCALE_VCBYPASS		2

/*
 * OMAP3 GENERIC setup times. Revisit to see if these needs to be
 * passed from board or PMIC file
 */
#define OMAP3_CLKSETUP		0xff
#define OMAP3_VOLTOFFSET	0xff
#define OMAP3_VOLTSETUP2	0xff

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * struct omap_vfsm_instance_data - per-voltage manager FSM register/bitfield
 * data
 * @voltsetup_mask: SETUP_TIME* bitmask in the PRM_VOLTSETUP* register
 * @voltsetup_reg: register offset of PRM_VOLTSETUP from PRM base
 * @voltsetup_shift: SETUP_TIME* field shift in the PRM_VOLTSETUP* register
 *
 * XXX What about VOLTOFFSET/VOLTCTRL?
 * XXX It is not necessary to have both a _mask and a _shift for the same
 *     bitfield - remove one!
 */
struct omap_vfsm_instance_data {
	u32 voltsetup_mask;
	u8 voltsetup_reg;
	u8 voltsetup_shift;
=======
=======
>>>>>>> refs/remotes/origin/master
 * struct omap_vfsm_instance - per-voltage manager FSM register/bitfield
 * data
 * @voltsetup_mask: SETUP_TIME* bitmask in the PRM_VOLTSETUP* register
 * @voltsetup_reg: register offset of PRM_VOLTSETUP from PRM base
<<<<<<< HEAD
=======
 * @voltsetup_off_reg: register offset of PRM_VOLTSETUP_OFF from PRM base
>>>>>>> refs/remotes/origin/master
 *
 * XXX What about VOLTOFFSET/VOLTCTRL?
 */
struct omap_vfsm_instance {
	u32 voltsetup_mask;
	u8 voltsetup_reg;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u8 voltsetup_off_reg;
>>>>>>> refs/remotes/origin/master
};

/**
 * struct voltagedomain - omap voltage domain global structure.
<<<<<<< HEAD
<<<<<<< HEAD
 * @name:	Name of the voltage domain which can be used as a unique
 *		identifier.
 */
struct voltagedomain {
	char *name;
=======
=======
>>>>>>> refs/remotes/origin/master
 * @name: Name of the voltage domain which can be used as a unique identifier.
 * @scalable: Whether or not this voltage domain is scalable
 * @node: list_head linking all voltage domains
 * @pwrdm_list: list_head linking all powerdomains in this voltagedomain
 * @vc: pointer to VC channel associated with this voltagedomain
 * @vp: pointer to VP associated with this voltagedomain
 * @read: read a VC/VP register
 * @write: write a VC/VP register
 * @read: read-modify-write a VC/VP register
 * @sys_clk: system clock name/frequency, used for various timing calculations
 * @scale: function used to scale the voltage of the voltagedomain
 * @nominal_volt: current nominal voltage for this voltage domain
 * @volt_data: voltage table having the distinct voltages supported
 *             by the domain and other associated per voltage data.
 */
struct voltagedomain {
	char *name;
	bool scalable;
	struct list_head node;
	struct list_head pwrdm_list;
	struct omap_vc_channel *vc;
	const struct omap_vfsm_instance *vfsm;
	struct omap_vp_instance *vp;
	struct omap_voltdm_pmic *pmic;
<<<<<<< HEAD
=======
	struct omap_vp_param *vp_param;
	struct omap_vc_param *vc_param;
>>>>>>> refs/remotes/origin/master

	/* VC/VP register access functions: SoC specific */
	u32 (*read) (u8 offset);
	void (*write) (u32 val, u8 offset);
	u32 (*rmw)(u32 mask, u32 bits, u8 offset);

	union {
		const char *name;
		u32 rate;
	} sys_clk;

	int (*scale) (struct voltagedomain *voltdm,
		      unsigned long target_volt);

	u32 nominal_volt;
	struct omap_volt_data *volt_data;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
};

/**
 * struct omap_volt_data - Omap voltage specific data.
 * @voltage_nominal:	The possible voltage value in uV
 * @sr_efuse_offs:	The offset of the efuse register(from system
 *			control module base address) from where to read
 *			the n-target value for the smartreflex module.
 * @sr_errminlimit:	Error min limit value for smartreflex. This value
 *			differs at differnet opp and thus is linked
 *			with voltage.
 * @vp_errorgain:	Error gain value for the voltage processor. This
 *			field also differs according to the voltage/opp.
 */
struct omap_volt_data {
	u32	volt_nominal;
	u32	sr_efuse_offs;
	u8	sr_errminlimit;
	u8	vp_errgain;
};

/**
<<<<<<< HEAD
 * struct omap_volt_pmic_info - PMIC specific data required by voltage driver.
 * @slew_rate:	PMIC slew rate (in uv/us)
 * @step_size:	PMIC voltage step size (in uv)
 * @vsel_to_uv:	PMIC API to convert vsel value to actual voltage in uV.
 * @uv_to_vsel:	PMIC API to convert voltage in uV to vsel value.
 */
struct omap_volt_pmic_info {
=======
=======
};

/* Min and max voltages from OMAP perspective */
#define OMAP3430_VP1_VLIMITTO_VDDMIN	850000
#define OMAP3430_VP1_VLIMITTO_VDDMAX	1425000
#define OMAP3430_VP2_VLIMITTO_VDDMIN	900000
#define OMAP3430_VP2_VLIMITTO_VDDMAX	1150000

#define OMAP3630_VP1_VLIMITTO_VDDMIN	900000
#define OMAP3630_VP1_VLIMITTO_VDDMAX	1350000
#define OMAP3630_VP2_VLIMITTO_VDDMIN	900000
#define OMAP3630_VP2_VLIMITTO_VDDMAX	1200000

#define OMAP4_VP_MPU_VLIMITTO_VDDMIN	830000
#define OMAP4_VP_MPU_VLIMITTO_VDDMAX	1410000
#define OMAP4_VP_IVA_VLIMITTO_VDDMIN	830000
#define OMAP4_VP_IVA_VLIMITTO_VDDMAX	1260000
#define OMAP4_VP_CORE_VLIMITTO_VDDMIN	830000
#define OMAP4_VP_CORE_VLIMITTO_VDDMAX	1200000

/**
>>>>>>> refs/remotes/origin/master
 * struct omap_voltdm_pmic - PMIC specific data required by voltage driver.
 * @slew_rate:	PMIC slew rate (in uv/us)
 * @step_size:	PMIC voltage step size (in uv)
 * @i2c_slave_addr: I2C slave address of PMIC
 * @volt_reg_addr: voltage configuration register address
 * @cmd_reg_addr: command (on, on-LP, ret, off) configuration register address
 * @i2c_high_speed: whether VC uses I2C high-speed mode to PMIC
 * @i2c_mcode: master code value for I2C high-speed preamble transmission
 * @vsel_to_uv:	PMIC API to convert vsel value to actual voltage in uV.
 * @uv_to_vsel:	PMIC API to convert voltage in uV to vsel value.
 */
struct omap_voltdm_pmic {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	int slew_rate;
	int step_size;
	u32 on_volt;
	u32 onlp_volt;
	u32 ret_volt;
	u32 off_volt;
	u16 volt_setup_time;
<<<<<<< HEAD
=======
	u16 i2c_slave_addr;
	u16 volt_reg_addr;
	u16 cmd_reg_addr;
>>>>>>> refs/remotes/origin/cm-10.0
	u8 vp_erroroffset;
	u8 vp_vstepmin;
	u8 vp_vstepmax;
	u8 vp_vddmin;
	u8 vp_vddmax;
	u8 vp_timeout_us;
<<<<<<< HEAD
	u8 i2c_slave_addr;
	u8 pmic_reg;
=======
	bool i2c_high_speed;
	u8 i2c_mcode;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int slew_rate;
	int step_size;
	u16 i2c_slave_addr;
	u16 volt_reg_addr;
	u16 cmd_reg_addr;
	u8 vp_erroroffset;
	u8 vp_vstepmin;
	u8 vp_vstepmax;
	u32 vddmin;
	u32 vddmax;
	u8 vp_timeout_us;
	bool i2c_high_speed;
	u32 i2c_pad_load;
	u8 i2c_mcode;
>>>>>>> refs/remotes/origin/master
	unsigned long (*vsel_to_uv) (const u8 vsel);
	u8 (*uv_to_vsel) (unsigned long uV);
};

<<<<<<< HEAD
<<<<<<< HEAD
/**
 * omap_vdd_info - Per Voltage Domain info
 *
 * @volt_data		: voltage table having the distinct voltages supported
 *			  by the domain and other associated per voltage data.
 * @pmic_info		: pmic specific parameters which should be populted by
 *			  the pmic drivers.
 * @vp_data		: the register values, shifts, masks for various
 *			  vp registers
 * @vp_rt_data          : VP data derived at runtime, not predefined
 * @vc_data		: structure containing various various vc registers,
 *			  shifts, masks etc.
 * @vfsm                : voltage manager FSM data
 * @voltdm		: pointer to the voltage domain structure
 * @debug_dir		: debug directory for this voltage domain.
 * @curr_volt		: current voltage for this vdd.
 * @vp_enabled		: flag to keep track of whether vp is enabled or not
 * @volt_scale		: API to scale the voltage of the vdd.
 */
struct omap_vdd_info {
	struct omap_volt_data *volt_data;
	struct omap_volt_pmic_info *pmic_info;
	struct omap_vp_instance_data *vp_data;
	struct omap_vp_runtime_data vp_rt_data;
	struct omap_vc_instance_data *vc_data;
	const struct omap_vfsm_instance_data *vfsm;
	struct voltagedomain voltdm;
	struct dentry *debug_dir;
	u32 curr_volt;
	bool vp_enabled;
	u32 (*read_reg) (u16 mod, u8 offset);
	void (*write_reg) (u32 val, u16 mod, u8 offset);
	int (*volt_scale) (struct omap_vdd_info *vdd,
		unsigned long target_volt);
};

unsigned long omap_vp_get_curr_volt(struct voltagedomain *voltdm);
void omap_vp_enable(struct voltagedomain *voltdm);
void omap_vp_disable(struct voltagedomain *voltdm);
int omap_voltage_scale_vdd(struct voltagedomain *voltdm,
		unsigned long target_volt);
void omap_voltage_reset(struct voltagedomain *voltdm);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct omap_vp_param {
	u32 vddmax;
	u32 vddmin;
};

struct omap_vc_param {
	u32 on;
	u32 onlp;
	u32 ret;
	u32 off;
};

>>>>>>> refs/remotes/origin/master
void omap_voltage_get_volttable(struct voltagedomain *voltdm,
		struct omap_volt_data **volt_data);
struct omap_volt_data *omap_voltage_get_voltdata(struct voltagedomain *voltdm,
		unsigned long volt);
<<<<<<< HEAD
<<<<<<< HEAD
unsigned long omap_voltage_get_nom_volt(struct voltagedomain *voltdm);
struct dentry *omap_voltage_get_dbgdir(struct voltagedomain *voltdm);
int __init omap_voltage_early_init(s16 prm_mod, s16 prm_irqst_mod,
				   struct omap_vdd_info *omap_vdd_array[],
				   u8 omap_vdd_count);
#ifdef CONFIG_PM
int omap_voltage_register_pmic(struct voltagedomain *voltdm,
		struct omap_volt_pmic_info *pmic_info);
void omap_change_voltscale_method(struct voltagedomain *voltdm,
		int voltscale_method);
/* API to get the voltagedomain pointer */
struct voltagedomain *omap_voltage_domain_lookup(char *name);

int omap_voltage_late_init(void);
#else
static inline int omap_voltage_register_pmic(struct voltagedomain *voltdm,
		struct omap_volt_pmic_info *pmic_info)
{
	return -EINVAL;
}
static inline  void omap_change_voltscale_method(struct voltagedomain *voltdm,
		int voltscale_method) {}
static inline int omap_voltage_late_init(void)
{
	return -EINVAL;
}
static inline struct voltagedomain *omap_voltage_domain_lookup(char *name)
{
	return ERR_PTR(-EINVAL);
}
#endif

=======
=======
>>>>>>> refs/remotes/origin/master
int omap_voltage_register_pmic(struct voltagedomain *voltdm,
			       struct omap_voltdm_pmic *pmic);
void omap_change_voltscale_method(struct voltagedomain *voltdm,
		int voltscale_method);
int omap_voltage_late_init(void);

extern void omap2xxx_voltagedomains_init(void);
extern void omap3xxx_voltagedomains_init(void);
extern void omap44xx_voltagedomains_init(void);
<<<<<<< HEAD
=======
extern void omap54xx_voltagedomains_init(void);
>>>>>>> refs/remotes/origin/master

struct voltagedomain *voltdm_lookup(const char *name);
void voltdm_init(struct voltagedomain **voltdm_list);
int voltdm_add_pwrdm(struct voltagedomain *voltdm, struct powerdomain *pwrdm);
int voltdm_for_each(int (*fn)(struct voltagedomain *voltdm, void *user),
		    void *user);
int voltdm_for_each_pwrdm(struct voltagedomain *voltdm,
			  int (*fn)(struct voltagedomain *voltdm,
				    struct powerdomain *pwrdm));
int voltdm_scale(struct voltagedomain *voltdm, unsigned long target_volt);
void voltdm_reset(struct voltagedomain *voltdm);
unsigned long voltdm_get_voltage(struct voltagedomain *voltdm);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif
