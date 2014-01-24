/* linux/include/asm-arm/arch-msm/hsusb.h
 *
 * Copyright (C) 2008 Google, Inc.
 * Author: Brian Swetland <swetland@google.com>
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2009-2012, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2009-2011, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
 * Copyright (c) 2009-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
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

#ifndef __ASM_ARCH_MSM_HSUSB_H
#define __ASM_ARCH_MSM_HSUSB_H

#include <linux/types.h>
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/usb/otg.h>
#include <linux/wakelock.h>
#include <linux/pm_qos_params.h>
=======
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/usb/otg.h>
#include <linux/wakelock.h>
=======
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/usb/otg.h>
#include <linux/wakelock.h>
>>>>>>> refs/remotes/origin/cm-11.0
#include <linux/pm_qos.h>
#include <linux/hrtimer.h>

/*
 * The following are bit fields describing the usb_request.udc_priv word.
 * These bit fields are set by function drivers that wish to queue
 * usb_requests with sps/bam parameters.
 */
#define MSM_PIPE_ID_MASK		(0x1F)
#define MSM_TX_PIPE_ID_OFS		(16)
#define MSM_SPS_MODE			BIT(5)
#define MSM_IS_FINITE_TRANSFER		BIT(6)
#define MSM_PRODUCER			BIT(7)
#define MSM_DISABLE_WB			BIT(8)
#define MSM_ETD_IOC			BIT(9)
#define MSM_INTERNAL_MEM		BIT(10)
#define MSM_VENDOR_ID			BIT(16)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/usb/otg.h>
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

/**
 * Supported USB modes
 *
 * USB_PERIPHERAL       Only peripheral mode is supported.
 * USB_HOST             Only host mode is supported.
 * USB_OTG              OTG mode is supported.
 *
 */
enum usb_mode_type {
	USB_NONE = 0,
	USB_PERIPHERAL,
	USB_HOST,
	USB_OTG,
};

/**
 * OTG control
 *
 * OTG_NO_CONTROL	Id/VBUS notifications not required. Useful in host
 *                      only configuration.
 * OTG_PHY_CONTROL	Id/VBUS notifications comes form USB PHY.
 * OTG_PMIC_CONTROL	Id/VBUS notifications comes from PMIC hardware.
 * OTG_USER_CONTROL	Id/VBUS notifcations comes from User via sysfs.
 *
 */
enum otg_control_type {
	OTG_NO_CONTROL = 0,
	OTG_PHY_CONTROL,
	OTG_PMIC_CONTROL,
	OTG_USER_CONTROL,
};

/**
 * PHY used in
 *
 * INVALID_PHY			Unsupported PHY
 * CI_45NM_INTEGRATED_PHY	Chipidea 45nm integrated PHY
 * SNPS_28NM_INTEGRATED_PHY	Synopsis 28nm integrated PHY
 *
 */
enum msm_usb_phy_type {
	INVALID_PHY = 0,
	CI_45NM_INTEGRATED_PHY,
	SNPS_28NM_INTEGRATED_PHY,
};

#define IDEV_CHG_MAX	1500
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define IDEV_CHG_MIN	500
#define IUNIT		100

#define IDEV_ACA_CHG_MAX	1500
#define IDEV_ACA_CHG_LIMIT	500

<<<<<<< HEAD
=======
#define IUNIT		100

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
/**
 * Different states involved in USB charger detection.
 *
 * USB_CHG_STATE_UNDEFINED	USB charger is not connected or detection
 *                              process is not yet started.
 * USB_CHG_STATE_WAIT_FOR_DCD	Waiting for Data pins contact.
 * USB_CHG_STATE_DCD_DONE	Data pin contact is detected.
 * USB_CHG_STATE_PRIMARY_DONE	Primary detection is completed (Detects
 *                              between SDP and DCP/CDP).
 * USB_CHG_STATE_SECONDARY_DONE	Secondary detection is completed (Detects
 *                              between DCP and CDP).
 * USB_CHG_STATE_DETECTED	USB charger type is determined.
 *
 */
enum usb_chg_state {
	USB_CHG_STATE_UNDEFINED = 0,
	USB_CHG_STATE_WAIT_FOR_DCD,
	USB_CHG_STATE_DCD_DONE,
	USB_CHG_STATE_PRIMARY_DONE,
	USB_CHG_STATE_SECONDARY_DONE,
	USB_CHG_STATE_DETECTED,
};

/**
 * USB charger types
 *
 * USB_INVALID_CHARGER	Invalid USB charger.
 * USB_SDP_CHARGER	Standard downstream port. Refers to a downstream port
 *                      on USB2.0 compliant host/hub.
 * USB_DCP_CHARGER	Dedicated charger port (AC charger/ Wall charger).
 * USB_CDP_CHARGER	Charging downstream port. Enumeration can happen and
 *                      IDEV_CHG_MAX can be drawn irrespective of USB state.
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
 * USB_ACA_A_CHARGER	B-device is connected on accessory port with charger
 *                      connected on charging port. This configuration allows
 *                      charging in host mode.
 * USB_ACA_B_CHARGER	No device (or A-device without VBUS) is connected on
 *                      accessory port with charger connected on charging port.
 * USB_ACA_C_CHARGER	A-device (with VBUS) is connected on
 *                      accessory port with charger connected on charging port.
 * USB_ACA_DOCK_CHARGER	A docking station that has one upstream port and one
 *			or more downstream ports. Capable of supplying
 *			IDEV_CHG_MAX irrespective of devices connected on
 *			accessory ports.
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
 * USB_PROPRIETARY_CHARGER A proprietary charger pull DP and DM to specific
 *			voltages between 2.0-3.3v for identification.
 *
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *
>>>>>>> refs/remotes/origin/master
 */
enum usb_chg_type {
	USB_INVALID_CHARGER = 0,
	USB_SDP_CHARGER,
	USB_DCP_CHARGER,
	USB_CDP_CHARGER,
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	USB_ACA_A_CHARGER,
	USB_ACA_B_CHARGER,
	USB_ACA_C_CHARGER,
	USB_ACA_DOCK_CHARGER,
<<<<<<< HEAD
<<<<<<< HEAD
};

/**
 * SPS Pipes direction.
 *
 * USB_TO_PEER_PERIPHERAL	USB (as Producer) to other
 *                          peer peripheral.
 * PEER_PERIPHERAL_TO_USB	Other Peripheral to
 *                          USB (as consumer).
 */
enum usb_bam_pipe_dir {
	USB_TO_PEER_PERIPHERAL,
	PEER_PERIPHERAL_TO_USB,
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	USB_PROPRIETARY_CHARGER,
};

/**
 * Used different VDDCX voltage voting mechnism
 * VDDCX_CORNER       Vote for VDDCX Corner voltage
 * VDDCX              Vote for VDDCX Absolute voltage
 */
enum usb_vdd_type {
	VDDCX_CORNER = 0,
	VDDCX,
	VDD_TYPE_MAX,
};

/**
 * Used different VDDCX voltage values
 */
enum usb_vdd_value {
	VDD_NONE = 0,
	VDD_MIN,
	VDD_MAX,
	VDD_VAL_MAX,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
};

/**
 * struct msm_otg_platform_data - platform device data
 *              for msm_otg driver.
 * @phy_init_seq: PHY configuration sequence. val, reg pairs
 *              terminated by -1.
<<<<<<< HEAD
<<<<<<< HEAD
 * @vbus_power: VBUS power on/off routine.It should return result
 *		as success(zero value) or failure(non-zero value).
=======
 * @vbus_power: VBUS power on/off routine.
>>>>>>> refs/remotes/origin/master
=======
 * @vbus_power: VBUS power on/off routine.It should return result
 *		as success(zero value) or failure(non-zero value).
>>>>>>> refs/remotes/origin/cm-11.0
 * @power_budget: VBUS power budget in mA (0 will be treated as 500mA).
 * @mode: Supported mode (OTG/peripheral/host).
 * @otg_control: OTG switch controlled by user/Id pin
 * @default_mode: Default operational mode. Applicable only if
 *              OTG switch is controller by user.
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
 * @pmic_id_irq: IRQ number assigned for PMIC USB ID line.
 * @mhl_enable: indicates MHL connector or not.
 * @disable_reset_on_disconnect: perform USB PHY and LINK reset
 *              on USB cable disconnection.
<<<<<<< HEAD
<<<<<<< HEAD
 * @swfi_latency: miminum latency to allow swfi.
 * @enable_dcd: Enable Data Contact Detection circuit. if not set
 *              wait for 600msec before proceeding to primary
 *              detection.
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
 * @enable_dcd: Enable Data Contact Detection circuit. if not set
 *              wait for 600msec before proceeding to primary
 *              detection.
 * @enable_lpm_on_suspend: Enable the USB core to go into Low
 *              Power Mode, when USB bus is suspended but cable
 *              is connected.
 * @core_clk_always_on_workaround: Don't disable core_clk when
 *              USB enters LPM.
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
 * @bus_scale_table: parameters for bus bandwidth requirements
 */
struct msm_otg_platform_data {
	int *phy_init_seq;
	int (*vbus_power)(bool on);
=======
 * @pclk_src_name: pclk is derived from ebi1_usb_clk in case of 7x27 and 8k
 *              dfab_usb_hs_clk in case of 8660 and 8960.
 */
struct msm_otg_platform_data {
	int *phy_init_seq;
	void (*vbus_power)(bool on);
>>>>>>> refs/remotes/origin/master
=======
 * @bus_scale_table: parameters for bus bandwidth requirements
 */
struct msm_otg_platform_data {
	int *phy_init_seq;
	int (*vbus_power)(bool on);
>>>>>>> refs/remotes/origin/cm-11.0
	unsigned power_budget;
	enum usb_mode_type mode;
	enum otg_control_type otg_control;
	enum usb_mode_type default_mode;
	enum msm_usb_phy_type phy_type;
	void (*setup_gpio)(enum usb_otg_state state);
<<<<<<< HEAD
<<<<<<< HEAD
	int pmic_id_irq;
	bool mhl_enable;
	bool disable_reset_on_disconnect;
<<<<<<< HEAD
	u32 swfi_latency;
	bool enable_dcd;
	struct msm_bus_scale_pdata *bus_scale_table;
};

=======
=======
	int pmic_id_irq;
	bool mhl_enable;
	bool disable_reset_on_disconnect;
>>>>>>> refs/remotes/origin/cm-11.0
	bool enable_dcd;
	bool enable_lpm_on_dev_suspend;
	bool core_clk_always_on_workaround;
	struct msm_bus_scale_pdata *bus_scale_table;
<<<<<<< HEAD
};

/* Timeout (in msec) values (min - max) associated with OTG timers */

#define TA_WAIT_VRISE	100	/* ( - 100)  */
#define TA_WAIT_VFALL	500	/* ( - 1000) */

/*
 * This option is set for embedded hosts or OTG devices in which leakage
 * currents are very minimal.
 */
#ifdef CONFIG_USB_OTG
#define TA_WAIT_BCON	30000	/* (1100 - 30000) */
#else
#define TA_WAIT_BCON	-1
#endif

#define TA_AIDL_BDIS	500	/* (200 - ) */
#define TA_BIDL_ADIS	155	/* (155 - 200) */
#define TB_SRP_FAIL	6000	/* (5000 - 6000) */
#define TB_ASE0_BRST	200	/* (155 - ) */

/* TB_SSEND_SRP and TB_SE0_SRP are combined */
#define TB_SRP_INIT	2000	/* (1500 - ) */

#define TA_TST_MAINT	10100	/* (9900 - 10100) */
#define TB_TST_SRP	3000	/* ( - 5000) */
#define TB_TST_CONFIG	300

/* Timeout variables */

#define A_WAIT_VRISE	0
#define A_WAIT_VFALL	1
#define A_WAIT_BCON	2
#define A_AIDL_BDIS	3
#define A_BIDL_ADIS	4
#define B_SRP_FAIL	5
#define B_ASE0_BRST	6
#define A_TST_MAINT	7
#define B_TST_SRP	8
#define B_TST_CONFIG	9

>>>>>>> refs/remotes/origin/cm-10.0
=======
	char *pclk_src_name;
};

>>>>>>> refs/remotes/origin/master
=======
};

/* Timeout (in msec) values (min - max) associated with OTG timers */

#define TA_WAIT_VRISE	100	/* ( - 100)  */
#define TA_WAIT_VFALL	500	/* ( - 1000) */

/*
 * This option is set for embedded hosts or OTG devices in which leakage
 * currents are very minimal.
 */
#ifdef CONFIG_USB_OTG
#define TA_WAIT_BCON	30000	/* (1100 - 30000) */
#else
#define TA_WAIT_BCON	-1
#endif

#define TA_AIDL_BDIS	500	/* (200 - ) */
#define TA_BIDL_ADIS	155	/* (155 - 200) */
#define TB_SRP_FAIL	6000	/* (5000 - 6000) */
#define TB_ASE0_BRST	200	/* (155 - ) */

/* TB_SSEND_SRP and TB_SE0_SRP are combined */
#define TB_SRP_INIT	2000	/* (1500 - ) */

#define TA_TST_MAINT	10100	/* (9900 - 10100) */
#define TB_TST_SRP	3000	/* ( - 5000) */
#define TB_TST_CONFIG	300

/* Timeout variables */

#define A_WAIT_VRISE	0
#define A_WAIT_VFALL	1
#define A_WAIT_BCON	2
#define A_AIDL_BDIS	3
#define A_BIDL_ADIS	4
#define B_SRP_FAIL	5
#define B_ASE0_BRST	6
#define A_TST_MAINT	7
#define B_TST_SRP	8
#define B_TST_CONFIG	9

>>>>>>> refs/remotes/origin/cm-11.0
/**
 * struct msm_otg: OTG driver data. Shared by HCD and DCD.
 * @otg: USB OTG Transceiver structure.
 * @pdata: otg device platform data.
 * @irq: IRQ number assigned for HSUSB controller.
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
 * @clk: clock struct of alt_core_clk.
 * @pclk: clock struct of iface_clk.
 * @phy_reset_clk: clock struct of phy_clk.
 * @core_clk: clock struct of core_bus_clk.
<<<<<<< HEAD
=======
 * @clk: clock struct of usb_hs_clk.
 * @pclk: clock struct of usb_hs_pclk.
 * @pclk_src: pclk source for voting.
 * @phy_reset_clk: clock struct of usb_phy_clk.
 * @core_clk: clock struct of usb_hs_core_clk.
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
 * @regs: ioremapped register base address.
 * @inputs: OTG state machine inputs(Id, SessValid etc).
 * @sm_work: OTG state machine work.
 * @in_lpm: indicates low power mode (LPM) state.
 * @async_int: Async interrupt arrived.
 * @cur_power: The amount of mA available from downstream port.
 * @chg_work: Charger detection work.
 * @chg_state: The state of charger detection process.
 * @chg_type: The type of charger attached.
 * @dcd_retires: The retry count used to track Data contact
 *               detection process.
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
 * @wlock: Wake lock struct to prevent system suspend when
 *               USB is active.
 * @usbdev_nb: The notifier block used to know about the B-device
 *             connected. Useful only when ACA_A charger is
 *             connected.
 * @mA_port: The amount of current drawn by the attached B-device.
<<<<<<< HEAD
<<<<<<< HEAD
 * @pm_qos_req_dma: miminum DMA latency to vote against idle power
	collapse when cable is connected.
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
 * @id_timer: The timer used for polling ID line to detect ACA states.
 * @xo_handle: TCXO buffer handle
 * @bus_perf_client: Bus performance client handle to request BUS bandwidth
 */
struct msm_otg {
<<<<<<< HEAD
	struct otg_transceiver otg;
=======
	struct usb_phy phy;
>>>>>>> refs/remotes/origin/cm-10.0
=======
 */
struct msm_otg {
	struct usb_phy phy;
>>>>>>> refs/remotes/origin/master
	struct msm_otg_platform_data *pdata;
	int irq;
	struct clk *clk;
	struct clk *pclk;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct clk *pclk_src;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct clk *phy_reset_clk;
	struct clk *core_clk;
	void __iomem *regs;
#define ID		0
#define B_SESS_VLD	1
<<<<<<< HEAD
<<<<<<< HEAD
#define ID_A		2
#define ID_B		3
#define ID_C		4
<<<<<<< HEAD
	unsigned long inputs;
	struct work_struct sm_work;
=======
#define A_BUS_DROP	5
#define A_BUS_REQ	6
#define A_SRP_DET	7
#define A_VBUS_VLD	8
#define B_CONN		9
#define ADP_CHANGE	10
#define POWER_UP	11
#define A_CLR_ERR	12
#define A_BUS_RESUME	13
#define A_BUS_SUSPEND	14
#define A_CONN		15
#define B_BUS_REQ	16
	unsigned long inputs;
	struct work_struct sm_work;
	bool sm_work_pending;
	atomic_t pm_suspended;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned long inputs;
	struct work_struct sm_work;
>>>>>>> refs/remotes/origin/master
=======
#define ID_A		2
#define ID_B		3
#define ID_C		4
#define A_BUS_DROP	5
#define A_BUS_REQ	6
#define A_SRP_DET	7
#define A_VBUS_VLD	8
#define B_CONN		9
#define ADP_CHANGE	10
#define POWER_UP	11
#define A_CLR_ERR	12
#define A_BUS_RESUME	13
#define A_BUS_SUSPEND	14
#define A_CONN		15
#define B_BUS_REQ	16
	unsigned long inputs;
	struct work_struct sm_work;
	bool sm_work_pending;
	atomic_t pm_suspended;
>>>>>>> refs/remotes/origin/cm-11.0
	atomic_t in_lpm;
	int async_int;
	unsigned cur_power;
	struct delayed_work chg_work;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct delayed_work pmic_id_status_work;
>>>>>>> refs/remotes/origin/cm-10.0
	enum usb_chg_state chg_state;
	enum usb_chg_type chg_type;
	u8 dcd_retries;
	struct wake_lock wlock;
	struct notifier_block usbdev_nb;
	unsigned mA_port;
	struct timer_list id_timer;
	unsigned long caps;
	struct msm_xo_voter *xo_handle;
	uint32_t bus_perf_client;
	/*
	 * Allowing PHY power collpase turns off the HSUSB 3.3v and 1.8v
	 * analog regulators while going to low power mode.
	 * Currently only 8960(28nm PHY) has the support to allowing PHY
	 * power collapse since it doesn't have leakage currents while
	 * turning off the power rails.
	 */
#define ALLOW_PHY_POWER_COLLAPSE	BIT(0)
	/*
	 * Allow PHY RETENTION mode before turning off the digital
	 * voltage regulator(VDDCX).
	 */
#define ALLOW_PHY_RETENTION		BIT(1)
	  /*
<<<<<<< HEAD
	   * Disable the OTG comparators to save more power
	   * if depends on PMIC for VBUS and ID interrupts.
	   */
#define ALLOW_PHY_COMP_DISABLE		BIT(2)
	unsigned long lpm_flags;
#define PHY_PWR_COLLAPSED		BIT(0)
#define PHY_RETENTIONED			BIT(1)
#define PHY_OTG_COMP_DISABLED		BIT(2)
	struct pm_qos_request_list pm_qos_req_dma;
	int reset_counter;
=======
	   * Allow putting the core in Low Power mode, when
	   * USB bus is suspended but cable is connected.
	   */
#define ALLOW_LPM_ON_DEV_SUSPEND	    BIT(2)
	unsigned long lpm_flags;
#define PHY_PWR_COLLAPSED		BIT(0)
#define PHY_RETENTIONED			BIT(1)
#define XO_SHUTDOWN			BIT(2)
	int reset_counter;
	unsigned long b_last_se0_sess;
	unsigned long tmouts;
	u8 active_tmout;
	struct hrtimer timer;
	enum usb_vdd_type vdd_type;
>>>>>>> refs/remotes/origin/cm-10.0
};

struct msm_hsic_host_platform_data {
	unsigned strobe;
	unsigned data;
<<<<<<< HEAD
	unsigned hub_reset;
=======
	struct msm_bus_scale_pdata *bus_scale_table;
	unsigned log2_irq_thresh;
	u32 swfi_latency;
};

struct msm_usb_host_platform_data {
	unsigned int power_budget;
	unsigned int dock_connect_irq;
};

/**
 * struct msm_hsic_peripheral_platform_data: HSIC peripheral
 * platform data.
 * @core_clk_always_on_workaround: Don't disable core_clk when
 *                                 HSIC enters LPM.
 */
struct msm_hsic_peripheral_platform_data {
	bool core_clk_always_on_workaround;
>>>>>>> refs/remotes/origin/cm-10.0
};

struct usb_bam_pipe_connect {
	u32 src_phy_addr;
	int src_pipe_index;
	u32 dst_phy_addr;
	int dst_pipe_index;
	u32 data_fifo_base_offset;
	u32 data_fifo_size;
	u32 desc_fifo_base_offset;
	u32 desc_fifo_size;
};

struct msm_usb_bam_platform_data {
	struct usb_bam_pipe_connect *connections;
<<<<<<< HEAD
	unsigned long usb_bam_phy_base;
	unsigned long usb_bam_phy_size;
	int usb_bam_num_pipes;
};
=======
	int usb_active_bam;
	int usb_bam_num_pipes;
};

enum usb_bam {
	HSUSB_BAM = 0,
	HSIC_BAM,
};

int msm_ep_config(struct usb_ep *ep);
int msm_ep_unconfig(struct usb_ep *ep);
int msm_data_fifo_config(struct usb_ep *ep, u32 addr, u32 size);

>>>>>>> refs/remotes/origin/cm-10.0
=======
=======
	struct delayed_work pmic_id_status_work;
>>>>>>> refs/remotes/origin/cm-11.0
	enum usb_chg_state chg_state;
	enum usb_chg_type chg_type;
	u8 dcd_retries;
	struct wake_lock wlock;
	struct notifier_block usbdev_nb;
	unsigned mA_port;
	struct timer_list id_timer;
	unsigned long caps;
	struct msm_xo_voter *xo_handle;
	uint32_t bus_perf_client;
	/*
	 * Allowing PHY power collpase turns off the HSUSB 3.3v and 1.8v
	 * analog regulators while going to low power mode.
	 * Currently only 8960(28nm PHY) has the support to allowing PHY
	 * power collapse since it doesn't have leakage currents while
	 * turning off the power rails.
	 */
#define ALLOW_PHY_POWER_COLLAPSE	BIT(0)
	/*
	 * Allow PHY RETENTION mode before turning off the digital
	 * voltage regulator(VDDCX).
	 */
#define ALLOW_PHY_RETENTION		BIT(1)
	  /*
	   * Allow putting the core in Low Power mode, when
	   * USB bus is suspended but cable is connected.
	   */
#define ALLOW_LPM_ON_DEV_SUSPEND	    BIT(2)
	unsigned long lpm_flags;
#define PHY_PWR_COLLAPSED		BIT(0)
#define PHY_RETENTIONED			BIT(1)
#define XO_SHUTDOWN			BIT(2)
	int reset_counter;
	unsigned long b_last_se0_sess;
	unsigned long tmouts;
	u8 active_tmout;
	struct hrtimer timer;
	enum usb_vdd_type vdd_type;
};

struct msm_hsic_host_platform_data {
	unsigned strobe;
	unsigned data;
	struct msm_bus_scale_pdata *bus_scale_table;
	unsigned log2_irq_thresh;
	u32 swfi_latency;
};

struct msm_usb_host_platform_data {
	unsigned int power_budget;
	unsigned int dock_connect_irq;
};

/**
 * struct msm_hsic_peripheral_platform_data: HSIC peripheral
 * platform data.
 * @core_clk_always_on_workaround: Don't disable core_clk when
 *                                 HSIC enters LPM.
 */
struct msm_hsic_peripheral_platform_data {
	bool core_clk_always_on_workaround;
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
struct usb_bam_pipe_connect {
	u32 src_phy_addr;
	int src_pipe_index;
	u32 dst_phy_addr;
	int dst_pipe_index;
	u32 data_fifo_base_offset;
	u32 data_fifo_size;
	u32 desc_fifo_base_offset;
	u32 desc_fifo_size;
};

struct msm_usb_bam_platform_data {
	struct usb_bam_pipe_connect *connections;
	int usb_active_bam;
	int usb_bam_num_pipes;
};

enum usb_bam {
	HSUSB_BAM = 0,
	HSIC_BAM,
};

int msm_ep_config(struct usb_ep *ep);
int msm_ep_unconfig(struct usb_ep *ep);
int msm_data_fifo_config(struct usb_ep *ep, u32 addr, u32 size);

>>>>>>> refs/remotes/origin/cm-11.0
#endif
