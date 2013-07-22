/* Copyright (c) 2009, Code Aurora Forum. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of Code Aurora Forum, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef __MSM_BATTERY_H__
#define __MSM_BATTERY_H__

#define NO_CHG     0x00000000
#define AC_CHG     0x00000001
#define USB_CHG    0x00000002

enum cable_type_t {
	CABLE_TYPE_UNKNOWN = 0,
	CABLE_TYPE_USB,
	CABLE_TYPE_TA,
    CABLE_TYPE_USB_OTG,
};

enum acc_type_t {
	ACC_TYPE_NONE = 0,
	ACC_TYPE_USB,
	ACC_TYPE_CHARGER,
	ACC_TYPE_CAR_DOCK,
	ACC_TYPE_DESK_DOCK,
	ACC_TYPE_JIG,
};

enum ovp_type_t {
	OVP_TYPE_NONE = 0,
	OVP_TYPE_OVP,
	OVP_TYPE_RECOVER,
};

extern bool power_down;
extern int charging_boot;
static int set_timer;
static int fuel_alert_det;

struct msm_battery_callback {
	void (*set_cable)(struct msm_battery_callback *ptr,
		enum cable_type_t status);
	void (*set_acc_type)(struct msm_battery_callback *ptr,
		enum acc_type_t status);
	void (*set_ovp_type)(struct msm_battery_callback *ptr,
		enum ovp_type_t status);
};

struct msm_charger_data {
	void (*register_callbacks)(struct msm_battery_callback *ptr);
};

struct msm_psy_batt_pdata {
	struct msm_charger_data	*charger;
	u32 voltage_max_design;
	u32 voltage_min_design;
	u32 avail_chg_sources;
	u32 batt_technology;
};

void msm_battery_fuel_alert(void);

#endif
