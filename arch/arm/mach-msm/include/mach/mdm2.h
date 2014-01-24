<<<<<<< HEAD
/* Copyright (c) 2011, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
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

#ifndef _ARCH_ARM_MACH_MSM_MDM2_H
#define _ARCH_ARM_MACH_MSM_MDM2_H

<<<<<<< HEAD
struct mdm_platform_data {
	char *mdm_version;
=======
struct mdm_vddmin_resource {
	int rpm_id;
	int ap2mdm_vddmin_gpio;
	unsigned int modes;
	unsigned int drive_strength;
	int mdm2ap_vddmin_gpio;
};

struct mdm_platform_data {
	char *mdm_version;
	int ramdump_delay_ms;
	int soft_reset_inverted;
	int early_power_on;
	int sfr_query;
	int no_powerdown_after_ramdumps;
	struct mdm_vddmin_resource *vddmin_resource;
	struct platform_device *peripheral_platform_device;
	const unsigned int ramdump_timeout_ms;
>>>>>>> refs/remotes/origin/cm-10.0
};

#endif

