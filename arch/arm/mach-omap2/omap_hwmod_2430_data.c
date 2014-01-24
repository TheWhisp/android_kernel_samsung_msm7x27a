/*
 * omap_hwmod_2430_data.c - hardware modules present on the OMAP2430 chips
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (C) 2009-2010 Nokia Corporation
=======
 * Copyright (C) 2009-2011 Nokia Corporation
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (C) 2009-2011 Nokia Corporation
 * Copyright (C) 2012 Texas Instruments, Inc.
>>>>>>> refs/remotes/origin/master
 * Paul Walmsley
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * XXX handle crossbar/shared link difference for L3?
 * XXX these should be marked initdata for multi-OMAP kernels
 */
<<<<<<< HEAD
#include <plat/omap_hwmod.h>
#include <mach/irqs.h>
#include <plat/cpu.h>
#include <plat/dma.h>
#include <plat/serial.h>
#include <plat/i2c.h>
#include <plat/gpio.h>
#include <plat/mcbsp.h>
#include <plat/mcspi.h>
#include <plat/dmtimer.h>
#include <plat/mmc.h>
#include <plat/l3_2xxx.h>

#include "omap_hwmod_common_data.h"

#include "prm-regbits-24xx.h"
#include "cm-regbits-24xx.h"
=======

#include <linux/i2c-omap.h>
#include <linux/platform_data/asoc-ti-mcbsp.h>
#include <linux/platform_data/spi-omap2-mcspi.h>
#include <linux/omap-dma.h>
#include <linux/platform_data/mailbox-omap.h>
#include <plat/dmtimer.h>

#include "omap_hwmod.h"
#include "mmc.h"
#include "l3_2xxx.h"

#include "soc.h"
#include "omap_hwmod_common_data.h"
#include "prm-regbits-24xx.h"
#include "cm-regbits-24xx.h"
#include "i2c.h"
>>>>>>> refs/remotes/origin/master
#include "wd_timer.h"

/*
 * OMAP2430 hardware module integration data
 *
<<<<<<< HEAD
 * ALl of the data in this section should be autogeneratable from the
=======
 * All of the data in this section should be autogeneratable from the
>>>>>>> refs/remotes/origin/master
 * TI hardware database or other technical documentation.  Data that
 * is driver-specific or driver-kernel integration-specific belongs
 * elsewhere.
 */

<<<<<<< HEAD
static struct omap_hwmod omap2430_mpu_hwmod;
static struct omap_hwmod omap2430_iva_hwmod;
static struct omap_hwmod omap2430_l3_main_hwmod;
static struct omap_hwmod omap2430_l4_core_hwmod;
static struct omap_hwmod omap2430_dss_core_hwmod;
static struct omap_hwmod omap2430_dss_dispc_hwmod;
static struct omap_hwmod omap2430_dss_rfbi_hwmod;
static struct omap_hwmod omap2430_dss_venc_hwmod;
static struct omap_hwmod omap2430_wd_timer2_hwmod;
static struct omap_hwmod omap2430_gpio1_hwmod;
static struct omap_hwmod omap2430_gpio2_hwmod;
static struct omap_hwmod omap2430_gpio3_hwmod;
static struct omap_hwmod omap2430_gpio4_hwmod;
static struct omap_hwmod omap2430_gpio5_hwmod;
static struct omap_hwmod omap2430_dma_system_hwmod;
static struct omap_hwmod omap2430_mcbsp1_hwmod;
static struct omap_hwmod omap2430_mcbsp2_hwmod;
static struct omap_hwmod omap2430_mcbsp3_hwmod;
static struct omap_hwmod omap2430_mcbsp4_hwmod;
static struct omap_hwmod omap2430_mcbsp5_hwmod;
static struct omap_hwmod omap2430_mcspi1_hwmod;
static struct omap_hwmod omap2430_mcspi2_hwmod;
static struct omap_hwmod omap2430_mcspi3_hwmod;
static struct omap_hwmod omap2430_mmc1_hwmod;
static struct omap_hwmod omap2430_mmc2_hwmod;

/* L3 -> L4_CORE interface */
static struct omap_hwmod_ocp_if omap2430_l3_main__l4_core = {
	.master	= &omap2430_l3_main_hwmod,
	.slave	= &omap2430_l4_core_hwmod,
	.user	= OCP_USER_MPU | OCP_USER_SDMA,
};

/* MPU -> L3 interface */
static struct omap_hwmod_ocp_if omap2430_mpu__l3_main = {
	.master = &omap2430_mpu_hwmod,
	.slave	= &omap2430_l3_main_hwmod,
	.user	= OCP_USER_MPU,
};

/* Slave interfaces on the L3 interconnect */
static struct omap_hwmod_ocp_if *omap2430_l3_main_slaves[] = {
	&omap2430_mpu__l3_main,
};

/* DSS -> l3 */
static struct omap_hwmod_ocp_if omap2430_dss__l3 = {
	.master		= &omap2430_dss_core_hwmod,
	.slave		= &omap2430_l3_main_hwmod,
	.fw = {
		.omap2 = {
			.l3_perm_bit  = OMAP2_L3_CORE_FW_CONNID_DSS,
			.flags	= OMAP_FIREWALL_L3,
		}
	},
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* Master interfaces on the L3 interconnect */
static struct omap_hwmod_ocp_if *omap2430_l3_main_masters[] = {
	&omap2430_l3_main__l4_core,
};

/* L3 */
static struct omap_hwmod omap2430_l3_main_hwmod = {
	.name		= "l3_main",
	.class		= &l3_hwmod_class,
	.masters	= omap2430_l3_main_masters,
	.masters_cnt	= ARRAY_SIZE(omap2430_l3_main_masters),
	.slaves		= omap2430_l3_main_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_l3_main_slaves),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.flags		= HWMOD_NO_IDLEST,
};

static struct omap_hwmod omap2430_l4_wkup_hwmod;
static struct omap_hwmod omap2430_uart1_hwmod;
static struct omap_hwmod omap2430_uart2_hwmod;
static struct omap_hwmod omap2430_uart3_hwmod;
static struct omap_hwmod omap2430_i2c1_hwmod;
static struct omap_hwmod omap2430_i2c2_hwmod;

static struct omap_hwmod omap2430_usbhsotg_hwmod;

/* l3_core -> usbhsotg  interface */
static struct omap_hwmod_ocp_if omap2430_usbhsotg__l3 = {
	.master		= &omap2430_usbhsotg_hwmod,
	.slave		= &omap2430_l3_main_hwmod,
	.clk		= "core_l3_ck",
	.user		= OCP_USER_MPU,
};

<<<<<<< HEAD
/* I2C IP block address space length (in bytes) */
#define OMAP2_I2C_AS_LEN		128

/* L4 CORE -> I2C1 interface */
static struct omap_hwmod_addr_space omap2430_i2c1_addr_space[] = {
	{
		.pa_start	= 0x48070000,
		.pa_end		= 0x48070000 + OMAP2_I2C_AS_LEN - 1,
		.flags		= ADDR_TYPE_RT,
	},
};

=======
/* L4 CORE -> I2C1 interface */
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if omap2430_l4_core__i2c1 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_i2c1_hwmod,
	.clk		= "i2c1_ick",
<<<<<<< HEAD
	.addr		= omap2430_i2c1_addr_space,
	.addr_cnt	= ARRAY_SIZE(omap2430_i2c1_addr_space),
=======
	.addr		= omap2_i2c1_addr_space,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 CORE -> I2C2 interface */
<<<<<<< HEAD
static struct omap_hwmod_addr_space omap2430_i2c2_addr_space[] = {
	{
		.pa_start	= 0x48072000,
		.pa_end		= 0x48072000 + OMAP2_I2C_AS_LEN - 1,
		.flags		= ADDR_TYPE_RT,
	},
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if omap2430_l4_core__i2c2 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_i2c2_hwmod,
	.clk		= "i2c2_ick",
<<<<<<< HEAD
	.addr		= omap2430_i2c2_addr_space,
	.addr_cnt	= ARRAY_SIZE(omap2430_i2c2_addr_space),
=======
	.addr		= omap2_i2c2_addr_space,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4_CORE -> L4_WKUP interface */
static struct omap_hwmod_ocp_if omap2430_l4_core__l4_wkup = {
	.master	= &omap2430_l4_core_hwmod,
	.slave	= &omap2430_l4_wkup_hwmod,
	.user	= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 CORE -> UART1 interface */
<<<<<<< HEAD
static struct omap_hwmod_addr_space omap2430_uart1_addr_space[] = {
	{
		.pa_start	= OMAP2_UART1_BASE,
		.pa_end		= OMAP2_UART1_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if omap2_l4_core__uart1 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_uart1_hwmod,
	.clk		= "uart1_ick",
<<<<<<< HEAD
	.addr		= omap2430_uart1_addr_space,
	.addr_cnt	= ARRAY_SIZE(omap2430_uart1_addr_space),
=======
	.addr		= omap2xxx_uart1_addr_space,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 CORE -> UART2 interface */
<<<<<<< HEAD
static struct omap_hwmod_addr_space omap2430_uart2_addr_space[] = {
	{
		.pa_start	= OMAP2_UART2_BASE,
		.pa_end		= OMAP2_UART2_BASE + SZ_1K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if omap2_l4_core__uart2 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_uart2_hwmod,
	.clk		= "uart2_ick",
<<<<<<< HEAD
	.addr		= omap2430_uart2_addr_space,
	.addr_cnt	= ARRAY_SIZE(omap2430_uart2_addr_space),
=======
	.addr		= omap2xxx_uart2_addr_space,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 PER -> UART3 interface */
<<<<<<< HEAD
static struct omap_hwmod_addr_space omap2430_uart3_addr_space[] = {
	{
		.pa_start	= OMAP2_UART3_BASE,
		.pa_end		= OMAP2_UART3_BASE + SZ_1K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if omap2_l4_core__uart3 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_uart3_hwmod,
	.clk		= "uart3_ick",
<<<<<<< HEAD
	.addr		= omap2430_uart3_addr_space,
	.addr_cnt	= ARRAY_SIZE(omap2430_uart3_addr_space),
=======
	.addr		= omap2xxx_uart3_addr_space,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/*
* usbhsotg interface data
*/
static struct omap_hwmod_addr_space omap2430_usbhsotg_addrs[] = {
	{
		.pa_start	= OMAP243X_HS_BASE,
		.pa_end		= OMAP243X_HS_BASE + SZ_4K - 1,
		.flags		= ADDR_TYPE_RT
	},
<<<<<<< HEAD
=======
	{ }
>>>>>>> refs/remotes/origin/cm-10.0
};

/*  l4_core ->usbhsotg  interface */
static struct omap_hwmod_ocp_if omap2430_l4_core__usbhsotg = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_usbhsotg_hwmod,
	.clk		= "usb_l4_ick",
	.addr		= omap2430_usbhsotg_addrs,
<<<<<<< HEAD
	.addr_cnt	= ARRAY_SIZE(omap2430_usbhsotg_addrs),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *omap2430_usbhsotg_masters[] = {
	&omap2430_usbhsotg__l3,
};

static struct omap_hwmod_ocp_if *omap2430_usbhsotg_slaves[] = {
	&omap2430_l4_core__usbhsotg,
};

/* L4 CORE -> MMC1 interface */
<<<<<<< HEAD
static struct omap_hwmod_addr_space omap2430_mmc1_addr_space[] = {
	{
		.pa_start	= 0x4809c000,
		.pa_end		= 0x4809c1ff,
		.flags		= ADDR_TYPE_RT,
	},
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if omap2430_l4_core__mmc1 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_mmc1_hwmod,
	.clk		= "mmchs1_ick",
	.addr		= omap2430_mmc1_addr_space,
<<<<<<< HEAD
	.addr_cnt	= ARRAY_SIZE(omap2430_mmc1_addr_space),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 CORE -> MMC2 interface */
<<<<<<< HEAD
static struct omap_hwmod_addr_space omap2430_mmc2_addr_space[] = {
	{
		.pa_start	= 0x480b4000,
		.pa_end		= 0x480b41ff,
		.flags		= ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if omap2430_l4_core__mmc2 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_mmc2_hwmod,
	.addr		= omap2430_mmc2_addr_space,
	.clk		= "mmchs2_ick",
	.addr_cnt	= ARRAY_SIZE(omap2430_mmc2_addr_space),
=======
static struct omap_hwmod_ocp_if omap2430_l4_core__mmc2 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_mmc2_hwmod,
	.clk		= "mmchs2_ick",
	.addr		= omap2430_mmc2_addr_space,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* Slave interfaces on the L4_CORE interconnect */
static struct omap_hwmod_ocp_if *omap2430_l4_core_slaves[] = {
	&omap2430_l3_main__l4_core,
};

/* Master interfaces on the L4_CORE interconnect */
static struct omap_hwmod_ocp_if *omap2430_l4_core_masters[] = {
	&omap2430_l4_core__l4_wkup,
	&omap2430_l4_core__mmc1,
	&omap2430_l4_core__mmc2,
};

/* L4 CORE */
static struct omap_hwmod omap2430_l4_core_hwmod = {
	.name		= "l4_core",
	.class		= &l4_hwmod_class,
	.masters	= omap2430_l4_core_masters,
	.masters_cnt	= ARRAY_SIZE(omap2430_l4_core_masters),
	.slaves		= omap2430_l4_core_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_l4_core_slaves),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.flags		= HWMOD_NO_IDLEST,
};

/* Slave interfaces on the L4_WKUP interconnect */
static struct omap_hwmod_ocp_if *omap2430_l4_wkup_slaves[] = {
	&omap2430_l4_core__l4_wkup,
	&omap2_l4_core__uart1,
	&omap2_l4_core__uart2,
	&omap2_l4_core__uart3,
};

/* Master interfaces on the L4_WKUP interconnect */
static struct omap_hwmod_ocp_if *omap2430_l4_wkup_masters[] = {
};

/* l4 core -> mcspi1 interface */
<<<<<<< HEAD
static struct omap_hwmod_addr_space omap2430_mcspi1_addr_space[] = {
	{
		.pa_start	= 0x48098000,
		.pa_end		= 0x480980ff,
		.flags		= ADDR_TYPE_RT,
	},
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if omap2430_l4_core__mcspi1 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_mcspi1_hwmod,
	.clk		= "mcspi1_ick",
<<<<<<< HEAD
	.addr		= omap2430_mcspi1_addr_space,
	.addr_cnt	= ARRAY_SIZE(omap2430_mcspi1_addr_space),
=======
	.addr		= omap2_mcspi1_addr_space,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4 core -> mcspi2 interface */
<<<<<<< HEAD
static struct omap_hwmod_addr_space omap2430_mcspi2_addr_space[] = {
	{
		.pa_start	= 0x4809a000,
		.pa_end		= 0x4809a0ff,
		.flags		= ADDR_TYPE_RT,
	},
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if omap2430_l4_core__mcspi2 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_mcspi2_hwmod,
	.clk		= "mcspi2_ick",
<<<<<<< HEAD
	.addr		= omap2430_mcspi2_addr_space,
	.addr_cnt	= ARRAY_SIZE(omap2430_mcspi2_addr_space),
=======
	.addr		= omap2_mcspi2_addr_space,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4 core -> mcspi3 interface */
<<<<<<< HEAD
static struct omap_hwmod_addr_space omap2430_mcspi3_addr_space[] = {
	{
		.pa_start	= 0x480b8000,
		.pa_end		= 0x480b80ff,
		.flags		= ADDR_TYPE_RT,
	},
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if omap2430_l4_core__mcspi3 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_mcspi3_hwmod,
	.clk		= "mcspi3_ick",
	.addr		= omap2430_mcspi3_addr_space,
<<<<<<< HEAD
	.addr_cnt	= ARRAY_SIZE(omap2430_mcspi3_addr_space),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 WKUP */
static struct omap_hwmod omap2430_l4_wkup_hwmod = {
	.name		= "l4_wkup",
	.class		= &l4_hwmod_class,
	.masters	= omap2430_l4_wkup_masters,
	.masters_cnt	= ARRAY_SIZE(omap2430_l4_wkup_masters),
	.slaves		= omap2430_l4_wkup_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_l4_wkup_slaves),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.flags		= HWMOD_NO_IDLEST,
};

/* Master interfaces on the MPU device */
static struct omap_hwmod_ocp_if *omap2430_mpu_masters[] = {
	&omap2430_mpu__l3_main,
};

/* MPU */
static struct omap_hwmod omap2430_mpu_hwmod = {
	.name		= "mpu",
	.class		= &mpu_hwmod_class,
	.main_clk	= "mpu_ck",
	.masters	= omap2430_mpu_masters,
	.masters_cnt	= ARRAY_SIZE(omap2430_mpu_masters),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

/*
 * IVA2_1 interface data
 */

/* IVA2 <- L3 interface */
static struct omap_hwmod_ocp_if omap2430_l3__iva = {
	.master		= &omap2430_l3_main_hwmod,
	.slave		= &omap2430_iva_hwmod,
	.clk		= "dsp_fck",
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

static struct omap_hwmod_ocp_if *omap2430_iva_masters[] = {
	&omap2430_l3__iva,
};

/*
 * IVA2 (IVA2)
 */

static struct omap_hwmod omap2430_iva_hwmod = {
	.name		= "iva",
	.class		= &iva_hwmod_class,
	.masters	= omap2430_iva_masters,
	.masters_cnt	= ARRAY_SIZE(omap2430_iva_masters),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
};

/* Timer Common */
static struct omap_hwmod_class_sysconfig omap2430_timer_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_CLOCKACTIVITY |
			   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
			   SYSC_HAS_AUTOIDLE),
=======
/*
 * IP blocks
 */

/* IVA2 (IVA2) */
static struct omap_hwmod_rst_info omap2430_iva_resets[] = {
	{ .name = "logic", .rst_shift = 0 },
	{ .name = "mmu", .rst_shift = 1 },
};

static struct omap_hwmod omap2430_iva_hwmod = {
	.name		= "iva",
	.class		= &iva_hwmod_class,
	.clkdm_name	= "dsp_clkdm",
	.rst_lines	= omap2430_iva_resets,
	.rst_lines_cnt	= ARRAY_SIZE(omap2430_iva_resets),
	.main_clk	= "dsp_fck",
};

/* I2C common */
static struct omap_hwmod_class_sysconfig i2c_sysc = {
	.rev_offs	= 0x00,
	.sysc_offs	= 0x20,
	.syss_offs	= 0x10,
	.sysc_flags	= (SYSC_HAS_SOFTRESET | SYSC_HAS_AUTOIDLE |
			   SYSS_HAS_RESET_STATUS),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class i2c_class = {
	.name		= "i2c",
	.sysc		= &i2c_sysc,
	.rev		= OMAP_I2C_IP_VERSION_1,
	.reset		= &omap_i2c_reset,
};

static struct omap_i2c_dev_attr i2c_dev_attr = {
	.fifo_depth	= 8, /* bytes */
	.flags		= OMAP_I2C_FLAG_BUS_SHIFT_2 |
			  OMAP_I2C_FLAG_FORCE_19200_INT_CLK,
};

/* I2C1 */
static struct omap_hwmod omap2430_i2c1_hwmod = {
	.name		= "i2c1",
	.flags		= HWMOD_16BIT_REG,
	.mpu_irqs	= omap2_i2c1_mpu_irqs,
	.sdma_reqs	= omap2_i2c1_sdma_reqs,
	.main_clk	= "i2chs1_fck",
	.prcm		= {
		.omap2 = {
			/*
			 * NOTE: The CM_FCLKEN* and CM_ICLKEN* for
			 * I2CHS IP's do not follow the usual pattern.
			 * prcm_reg_id alone cannot be used to program
			 * the iclk and fclk. Needs to be handled using
			 * additional flags when clk handling is moved
			 * to hwmod framework.
			 */
			.module_offs = CORE_MOD,
			.prcm_reg_id = 1,
			.module_bit = OMAP2430_EN_I2CHS1_SHIFT,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP2430_ST_I2CHS1_SHIFT,
		},
	},
	.class		= &i2c_class,
	.dev_attr	= &i2c_dev_attr,
};

/* I2C2 */
static struct omap_hwmod omap2430_i2c2_hwmod = {
	.name		= "i2c2",
	.flags		= HWMOD_16BIT_REG,
	.mpu_irqs	= omap2_i2c2_mpu_irqs,
	.sdma_reqs	= omap2_i2c2_sdma_reqs,
	.main_clk	= "i2chs2_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 1,
			.module_bit = OMAP2430_EN_I2CHS2_SHIFT,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP2430_ST_I2CHS2_SHIFT,
		},
	},
	.class		= &i2c_class,
	.dev_attr	= &i2c_dev_attr,
};

/* gpio5 */
static struct omap_hwmod_irq_info omap243x_gpio5_irqs[] = {
	{ .irq = 33 + OMAP_INTC_START, }, /* INT_24XX_GPIO_BANK5 */
	{ .irq = -1 },
};

static struct omap_hwmod omap2430_gpio5_hwmod = {
	.name		= "gpio5",
	.flags		= HWMOD_CONTROL_OPT_CLKS_IN_RESET,
	.mpu_irqs	= omap243x_gpio5_irqs,
	.main_clk	= "gpio5_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 2,
			.module_bit = OMAP2430_EN_GPIO5_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_GPIO5_SHIFT,
		},
	},
	.class		= &omap2xxx_gpio_hwmod_class,
	.dev_attr	= &omap2xxx_gpio_dev_attr,
};

/* dma attributes */
static struct omap_dma_dev_attr dma_dev_attr = {
	.dev_caps  = RESERVE_CHANNEL | DMA_LINKED_LCH | GLOBAL_PRIORITY |
				IS_CSSA_32 | IS_CDSA_32 | IS_RW_PRIORITY,
	.lch_count = 32,
};

static struct omap_hwmod omap2430_dma_system_hwmod = {
	.name		= "dma",
	.class		= &omap2xxx_dma_hwmod_class,
	.mpu_irqs	= omap2_dma_system_irqs,
	.main_clk	= "core_l3_ck",
	.dev_attr	= &dma_dev_attr,
	.flags		= HWMOD_NO_IDLEST,
};

/* mailbox */
static struct omap_mbox_dev_info omap2430_mailbox_info[] = {
	{ .name = "dsp", .tx_id = 0, .rx_id = 1 },
};

static struct omap_mbox_pdata omap2430_mailbox_attrs = {
	.num_users	= 4,
	.num_fifos	= 6,
	.info_cnt	= ARRAY_SIZE(omap2430_mailbox_info),
	.info		= omap2430_mailbox_info,
};

static struct omap_hwmod_irq_info omap2430_mailbox_irqs[] = {
	{ .irq = 26 + OMAP_INTC_START, },
	{ .irq = -1 },
};

static struct omap_hwmod omap2430_mailbox_hwmod = {
	.name		= "mailbox",
	.class		= &omap2xxx_mailbox_hwmod_class,
	.mpu_irqs	= omap2430_mailbox_irqs,
	.main_clk	= "mailboxes_ick",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_MAILBOXES_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_MAILBOXES_SHIFT,
		},
	},
	.dev_attr	= &omap2430_mailbox_attrs,
};

/* mcspi3 */
static struct omap_hwmod_irq_info omap2430_mcspi3_mpu_irqs[] = {
	{ .irq = 91 + OMAP_INTC_START, },
	{ .irq = -1 },
};

static struct omap_hwmod_dma_info omap2430_mcspi3_sdma_reqs[] = {
	{ .name = "tx0", .dma_req = 15 }, /* DMA_SPI3_TX0 */
	{ .name = "rx0", .dma_req = 16 }, /* DMA_SPI3_RX0 */
	{ .name = "tx1", .dma_req = 23 }, /* DMA_SPI3_TX1 */
	{ .name = "rx1", .dma_req = 24 }, /* DMA_SPI3_RX1 */
	{ .dma_req = -1 }
};

static struct omap2_mcspi_dev_attr omap_mcspi3_dev_attr = {
	.num_chipselect = 2,
};

static struct omap_hwmod omap2430_mcspi3_hwmod = {
	.name		= "mcspi3",
	.mpu_irqs	= omap2430_mcspi3_mpu_irqs,
	.sdma_reqs	= omap2430_mcspi3_sdma_reqs,
	.main_clk	= "mcspi3_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 2,
			.module_bit = OMAP2430_EN_MCSPI3_SHIFT,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_MCSPI3_SHIFT,
		},
	},
	.class		= &omap2xxx_mcspi_class,
	.dev_attr	= &omap_mcspi3_dev_attr,
};

/* usbhsotg */
static struct omap_hwmod_class_sysconfig omap2430_usbhsotg_sysc = {
	.rev_offs	= 0x0400,
	.sysc_offs	= 0x0404,
	.syss_offs	= 0x0408,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_MIDLEMODE|
			  SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
			  SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART |
			  MSTANDBY_FORCE | MSTANDBY_NO | MSTANDBY_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class usbotg_class = {
	.name = "usbotg",
	.sysc = &omap2430_usbhsotg_sysc,
};

/* usb_otg_hs */
static struct omap_hwmod_irq_info omap2430_usbhsotg_mpu_irqs[] = {

	{ .name = "mc", .irq = 92 + OMAP_INTC_START, },
	{ .name = "dma", .irq = 93 + OMAP_INTC_START, },
	{ .irq = -1 },
};

static struct omap_hwmod omap2430_usbhsotg_hwmod = {
	.name		= "usb_otg_hs",
	.mpu_irqs	= omap2430_usbhsotg_mpu_irqs,
	.main_clk	= "usbhs_ick",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP2430_EN_USBHS_MASK,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP2430_ST_USBHS_SHIFT,
		},
	},
	.class		= &usbotg_class,
	/*
	 * Erratum ID: i479  idle_req / idle_ack mechanism potentially
	 * broken when autoidle is enabled
	 * workaround is to disable the autoidle bit at module level.
	 */
	.flags		= HWMOD_NO_OCP_AUTOIDLE | HWMOD_SWSUP_SIDLE
				| HWMOD_SWSUP_MSTANDBY,
};

/*
 * 'mcbsp' class
 * multi channel buffered serial port controller
 */

static struct omap_hwmod_class_sysconfig omap2430_mcbsp_sysc = {
	.rev_offs	= 0x007C,
	.sysc_offs	= 0x008C,
	.sysc_flags	= (SYSC_HAS_SOFTRESET),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class omap2430_mcbsp_hwmod_class = {
	.name = "mcbsp",
	.sysc = &omap2430_mcbsp_sysc,
	.rev  = MCBSP_CONFIG_TYPE2,
};

static struct omap_hwmod_opt_clk mcbsp_opt_clks[] = {
	{ .role = "pad_fck", .clk = "mcbsp_clks" },
	{ .role = "prcm_fck", .clk = "func_96m_ck" },
};

/* mcbsp1 */
static struct omap_hwmod_irq_info omap2430_mcbsp1_irqs[] = {
	{ .name = "tx",		.irq = 59 + OMAP_INTC_START, },
	{ .name = "rx",		.irq = 60 + OMAP_INTC_START, },
	{ .name = "ovr",	.irq = 61 + OMAP_INTC_START, },
	{ .name = "common",	.irq = 64 + OMAP_INTC_START, },
	{ .irq = -1 },
};

static struct omap_hwmod omap2430_mcbsp1_hwmod = {
	.name		= "mcbsp1",
	.class		= &omap2430_mcbsp_hwmod_class,
	.mpu_irqs	= omap2430_mcbsp1_irqs,
	.sdma_reqs	= omap2_mcbsp1_sdma_reqs,
	.main_clk	= "mcbsp1_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_MCBSP1_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_MCBSP1_SHIFT,
		},
	},
	.opt_clks	= mcbsp_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(mcbsp_opt_clks),
};

/* mcbsp2 */
static struct omap_hwmod_irq_info omap2430_mcbsp2_irqs[] = {
	{ .name = "tx",		.irq = 62 + OMAP_INTC_START, },
	{ .name = "rx",		.irq = 63 + OMAP_INTC_START, },
	{ .name = "common",	.irq = 16 + OMAP_INTC_START, },
	{ .irq = -1 },
};

static struct omap_hwmod omap2430_mcbsp2_hwmod = {
	.name		= "mcbsp2",
	.class		= &omap2430_mcbsp_hwmod_class,
	.mpu_irqs	= omap2430_mcbsp2_irqs,
	.sdma_reqs	= omap2_mcbsp2_sdma_reqs,
	.main_clk	= "mcbsp2_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_MCBSP2_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_MCBSP2_SHIFT,
		},
	},
	.opt_clks	= mcbsp_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(mcbsp_opt_clks),
};

/* mcbsp3 */
static struct omap_hwmod_irq_info omap2430_mcbsp3_irqs[] = {
	{ .name = "tx",		.irq = 89 + OMAP_INTC_START, },
	{ .name = "rx",		.irq = 90 + OMAP_INTC_START, },
	{ .name = "common",	.irq = 17 + OMAP_INTC_START, },
	{ .irq = -1 },
};

static struct omap_hwmod omap2430_mcbsp3_hwmod = {
	.name		= "mcbsp3",
	.class		= &omap2430_mcbsp_hwmod_class,
	.mpu_irqs	= omap2430_mcbsp3_irqs,
	.sdma_reqs	= omap2_mcbsp3_sdma_reqs,
	.main_clk	= "mcbsp3_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP2430_EN_MCBSP3_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_MCBSP3_SHIFT,
		},
	},
	.opt_clks	= mcbsp_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(mcbsp_opt_clks),
};

/* mcbsp4 */
static struct omap_hwmod_irq_info omap2430_mcbsp4_irqs[] = {
	{ .name = "tx",		.irq = 54 + OMAP_INTC_START, },
	{ .name = "rx",		.irq = 55 + OMAP_INTC_START, },
	{ .name = "common",	.irq = 18 + OMAP_INTC_START, },
	{ .irq = -1 },
};

static struct omap_hwmod_dma_info omap2430_mcbsp4_sdma_chs[] = {
	{ .name = "rx", .dma_req = 20 },
	{ .name = "tx", .dma_req = 19 },
	{ .dma_req = -1 }
};

static struct omap_hwmod omap2430_mcbsp4_hwmod = {
	.name		= "mcbsp4",
	.class		= &omap2430_mcbsp_hwmod_class,
	.mpu_irqs	= omap2430_mcbsp4_irqs,
	.sdma_reqs	= omap2430_mcbsp4_sdma_chs,
	.main_clk	= "mcbsp4_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP2430_EN_MCBSP4_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_MCBSP4_SHIFT,
		},
	},
	.opt_clks	= mcbsp_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(mcbsp_opt_clks),
};

/* mcbsp5 */
static struct omap_hwmod_irq_info omap2430_mcbsp5_irqs[] = {
	{ .name = "tx",		.irq = 81 + OMAP_INTC_START, },
	{ .name = "rx",		.irq = 82 + OMAP_INTC_START, },
	{ .name = "common",	.irq = 19 + OMAP_INTC_START, },
	{ .irq = -1 },
};

static struct omap_hwmod_dma_info omap2430_mcbsp5_sdma_chs[] = {
	{ .name = "rx", .dma_req = 22 },
	{ .name = "tx", .dma_req = 21 },
	{ .dma_req = -1 }
};

static struct omap_hwmod omap2430_mcbsp5_hwmod = {
	.name		= "mcbsp5",
	.class		= &omap2430_mcbsp_hwmod_class,
	.mpu_irqs	= omap2430_mcbsp5_irqs,
	.sdma_reqs	= omap2430_mcbsp5_sdma_chs,
	.main_clk	= "mcbsp5_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP2430_EN_MCBSP5_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_MCBSP5_SHIFT,
		},
	},
	.opt_clks	= mcbsp_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(mcbsp_opt_clks),
};

/* MMC/SD/SDIO common */
static struct omap_hwmod_class_sysconfig omap2430_mmc_sysc = {
	.rev_offs	= 0x1fc,
	.sysc_offs	= 0x10,
	.syss_offs	= 0x14,
	.sysc_flags	= (SYSC_HAS_CLOCKACTIVITY | SYSC_HAS_SIDLEMODE |
			   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
			   SYSC_HAS_AUTOIDLE | SYSS_HAS_RESET_STATUS),
>>>>>>> refs/remotes/origin/master
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

<<<<<<< HEAD
static struct omap_hwmod_class omap2430_timer_hwmod_class = {
	.name = "timer",
	.sysc = &omap2430_timer_sysc,
	.rev = OMAP_TIMER_IP_VERSION_1,
=======
};

/* always-on timers dev attribute */
static struct omap_timer_capability_dev_attr capability_alwon_dev_attr = {
	.timer_capability       = OMAP_TIMER_ALWON,
};

/* pwm timers dev attribute */
static struct omap_timer_capability_dev_attr capability_pwm_dev_attr = {
	.timer_capability       = OMAP_TIMER_HAS_PWM,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* timer1 */
static struct omap_hwmod omap2430_timer1_hwmod;
<<<<<<< HEAD
static struct omap_hwmod_irq_info omap2430_timer1_mpu_irqs[] = {
	{ .irq = 37, },
};
=======
>>>>>>> refs/remotes/origin/cm-10.0

static struct omap_hwmod_addr_space omap2430_timer1_addrs[] = {
	{
		.pa_start	= 0x49018000,
		.pa_end		= 0x49018000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
<<<<<<< HEAD
=======
	{ }
>>>>>>> refs/remotes/origin/cm-10.0
};

/* l4_wkup -> timer1 */
static struct omap_hwmod_ocp_if omap2430_l4_wkup__timer1 = {
	.master		= &omap2430_l4_wkup_hwmod,
	.slave		= &omap2430_timer1_hwmod,
	.clk		= "gpt1_ick",
	.addr		= omap2430_timer1_addrs,
<<<<<<< HEAD
	.addr_cnt	= ARRAY_SIZE(omap2430_timer1_addrs),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer1 slave port */
static struct omap_hwmod_ocp_if *omap2430_timer1_slaves[] = {
	&omap2430_l4_wkup__timer1,
};

/* timer1 hwmod */
static struct omap_hwmod omap2430_timer1_hwmod = {
	.name		= "timer1",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_timer1_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_timer1_mpu_irqs),
=======
	.mpu_irqs	= omap2_timer1_mpu_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpt1_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPT1_SHIFT,
			.module_offs = WKUP_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPT1_SHIFT,
		},
	},
<<<<<<< HEAD
	.slaves		= omap2430_timer1_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer1_slaves),
	.class		= &omap2430_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
=======
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= omap2430_timer1_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer1_slaves),
	.class		= &omap2xxx_timer_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* timer2 */
static struct omap_hwmod omap2430_timer2_hwmod;
<<<<<<< HEAD
static struct omap_hwmod_irq_info omap2430_timer2_mpu_irqs[] = {
	{ .irq = 38, },
};

static struct omap_hwmod_addr_space omap2430_timer2_addrs[] = {
	{
		.pa_start	= 0x4802a000,
		.pa_end		= 0x4802a000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
};
=======
>>>>>>> refs/remotes/origin/cm-10.0

/* l4_core -> timer2 */
static struct omap_hwmod_ocp_if omap2430_l4_core__timer2 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_timer2_hwmod,
	.clk		= "gpt2_ick",
<<<<<<< HEAD
	.addr		= omap2430_timer2_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_timer2_addrs),
=======
	.addr		= omap2xxx_timer2_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer2 slave port */
static struct omap_hwmod_ocp_if *omap2430_timer2_slaves[] = {
	&omap2430_l4_core__timer2,
};

/* timer2 hwmod */
static struct omap_hwmod omap2430_timer2_hwmod = {
	.name		= "timer2",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_timer2_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_timer2_mpu_irqs),
=======
	.mpu_irqs	= omap2_timer2_mpu_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpt2_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPT2_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPT2_SHIFT,
		},
	},
<<<<<<< HEAD
	.slaves		= omap2430_timer2_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer2_slaves),
	.class		= &omap2430_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
=======
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= omap2430_timer2_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer2_slaves),
	.class		= &omap2xxx_timer_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* timer3 */
static struct omap_hwmod omap2430_timer3_hwmod;
<<<<<<< HEAD
static struct omap_hwmod_irq_info omap2430_timer3_mpu_irqs[] = {
	{ .irq = 39, },
};

static struct omap_hwmod_addr_space omap2430_timer3_addrs[] = {
	{
		.pa_start	= 0x48078000,
		.pa_end		= 0x48078000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
};
=======
>>>>>>> refs/remotes/origin/cm-10.0

/* l4_core -> timer3 */
static struct omap_hwmod_ocp_if omap2430_l4_core__timer3 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_timer3_hwmod,
	.clk		= "gpt3_ick",
<<<<<<< HEAD
	.addr		= omap2430_timer3_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_timer3_addrs),
=======
	.addr		= omap2xxx_timer3_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer3 slave port */
static struct omap_hwmod_ocp_if *omap2430_timer3_slaves[] = {
	&omap2430_l4_core__timer3,
};

/* timer3 hwmod */
static struct omap_hwmod omap2430_timer3_hwmod = {
	.name		= "timer3",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_timer3_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_timer3_mpu_irqs),
=======
	.mpu_irqs	= omap2_timer3_mpu_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpt3_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPT3_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPT3_SHIFT,
		},
	},
<<<<<<< HEAD
	.slaves		= omap2430_timer3_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer3_slaves),
	.class		= &omap2430_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
=======
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= omap2430_timer3_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer3_slaves),
	.class		= &omap2xxx_timer_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* timer4 */
static struct omap_hwmod omap2430_timer4_hwmod;
<<<<<<< HEAD
static struct omap_hwmod_irq_info omap2430_timer4_mpu_irqs[] = {
	{ .irq = 40, },
};

static struct omap_hwmod_addr_space omap2430_timer4_addrs[] = {
	{
		.pa_start	= 0x4807a000,
		.pa_end		= 0x4807a000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
};
=======
>>>>>>> refs/remotes/origin/cm-10.0

/* l4_core -> timer4 */
static struct omap_hwmod_ocp_if omap2430_l4_core__timer4 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_timer4_hwmod,
	.clk		= "gpt4_ick",
<<<<<<< HEAD
	.addr		= omap2430_timer4_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_timer4_addrs),
=======
	.addr		= omap2xxx_timer4_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer4 slave port */
static struct omap_hwmod_ocp_if *omap2430_timer4_slaves[] = {
	&omap2430_l4_core__timer4,
};

/* timer4 hwmod */
static struct omap_hwmod omap2430_timer4_hwmod = {
	.name		= "timer4",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_timer4_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_timer4_mpu_irqs),
=======
	.mpu_irqs	= omap2_timer4_mpu_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpt4_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPT4_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPT4_SHIFT,
		},
	},
<<<<<<< HEAD
	.slaves		= omap2430_timer4_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer4_slaves),
	.class		= &omap2430_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
=======
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= omap2430_timer4_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer4_slaves),
	.class		= &omap2xxx_timer_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* timer5 */
static struct omap_hwmod omap2430_timer5_hwmod;
<<<<<<< HEAD
static struct omap_hwmod_irq_info omap2430_timer5_mpu_irqs[] = {
	{ .irq = 41, },
};

static struct omap_hwmod_addr_space omap2430_timer5_addrs[] = {
	{
		.pa_start	= 0x4807c000,
		.pa_end		= 0x4807c000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
};
=======
>>>>>>> refs/remotes/origin/cm-10.0

/* l4_core -> timer5 */
static struct omap_hwmod_ocp_if omap2430_l4_core__timer5 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_timer5_hwmod,
	.clk		= "gpt5_ick",
<<<<<<< HEAD
	.addr		= omap2430_timer5_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_timer5_addrs),
=======
	.addr		= omap2xxx_timer5_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer5 slave port */
static struct omap_hwmod_ocp_if *omap2430_timer5_slaves[] = {
	&omap2430_l4_core__timer5,
};

/* timer5 hwmod */
static struct omap_hwmod omap2430_timer5_hwmod = {
	.name		= "timer5",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_timer5_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_timer5_mpu_irqs),
=======
	.mpu_irqs	= omap2_timer5_mpu_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpt5_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPT5_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPT5_SHIFT,
		},
	},
<<<<<<< HEAD
	.slaves		= omap2430_timer5_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer5_slaves),
	.class		= &omap2430_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
=======
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= omap2430_timer5_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer5_slaves),
	.class		= &omap2xxx_timer_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* timer6 */
static struct omap_hwmod omap2430_timer6_hwmod;
<<<<<<< HEAD
static struct omap_hwmod_irq_info omap2430_timer6_mpu_irqs[] = {
	{ .irq = 42, },
};

static struct omap_hwmod_addr_space omap2430_timer6_addrs[] = {
	{
		.pa_start	= 0x4807e000,
		.pa_end		= 0x4807e000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
};
=======
>>>>>>> refs/remotes/origin/cm-10.0

/* l4_core -> timer6 */
static struct omap_hwmod_ocp_if omap2430_l4_core__timer6 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_timer6_hwmod,
	.clk		= "gpt6_ick",
<<<<<<< HEAD
	.addr		= omap2430_timer6_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_timer6_addrs),
=======
	.addr		= omap2xxx_timer6_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer6 slave port */
static struct omap_hwmod_ocp_if *omap2430_timer6_slaves[] = {
	&omap2430_l4_core__timer6,
};

/* timer6 hwmod */
static struct omap_hwmod omap2430_timer6_hwmod = {
	.name		= "timer6",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_timer6_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_timer6_mpu_irqs),
=======
	.mpu_irqs	= omap2_timer6_mpu_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpt6_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPT6_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPT6_SHIFT,
		},
	},
<<<<<<< HEAD
	.slaves		= omap2430_timer6_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer6_slaves),
	.class		= &omap2430_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
=======
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= omap2430_timer6_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer6_slaves),
	.class		= &omap2xxx_timer_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* timer7 */
static struct omap_hwmod omap2430_timer7_hwmod;
<<<<<<< HEAD
static struct omap_hwmod_irq_info omap2430_timer7_mpu_irqs[] = {
	{ .irq = 43, },
};

static struct omap_hwmod_addr_space omap2430_timer7_addrs[] = {
	{
		.pa_start	= 0x48080000,
		.pa_end		= 0x48080000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
};
=======
>>>>>>> refs/remotes/origin/cm-10.0

/* l4_core -> timer7 */
static struct omap_hwmod_ocp_if omap2430_l4_core__timer7 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_timer7_hwmod,
	.clk		= "gpt7_ick",
<<<<<<< HEAD
	.addr		= omap2430_timer7_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_timer7_addrs),
=======
	.addr		= omap2xxx_timer7_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer7 slave port */
static struct omap_hwmod_ocp_if *omap2430_timer7_slaves[] = {
	&omap2430_l4_core__timer7,
};

/* timer7 hwmod */
static struct omap_hwmod omap2430_timer7_hwmod = {
	.name		= "timer7",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_timer7_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_timer7_mpu_irqs),
=======
	.mpu_irqs	= omap2_timer7_mpu_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpt7_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPT7_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPT7_SHIFT,
		},
	},
<<<<<<< HEAD
	.slaves		= omap2430_timer7_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer7_slaves),
	.class		= &omap2430_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
=======
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= omap2430_timer7_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer7_slaves),
	.class		= &omap2xxx_timer_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* timer8 */
static struct omap_hwmod omap2430_timer8_hwmod;
<<<<<<< HEAD
static struct omap_hwmod_irq_info omap2430_timer8_mpu_irqs[] = {
	{ .irq = 44, },
};

static struct omap_hwmod_addr_space omap2430_timer8_addrs[] = {
	{
		.pa_start	= 0x48082000,
		.pa_end		= 0x48082000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
};
=======
>>>>>>> refs/remotes/origin/cm-10.0

/* l4_core -> timer8 */
static struct omap_hwmod_ocp_if omap2430_l4_core__timer8 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_timer8_hwmod,
	.clk		= "gpt8_ick",
<<<<<<< HEAD
	.addr		= omap2430_timer8_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_timer8_addrs),
=======
	.addr		= omap2xxx_timer8_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer8 slave port */
static struct omap_hwmod_ocp_if *omap2430_timer8_slaves[] = {
	&omap2430_l4_core__timer8,
};

/* timer8 hwmod */
static struct omap_hwmod omap2430_timer8_hwmod = {
	.name		= "timer8",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_timer8_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_timer8_mpu_irqs),
=======
	.mpu_irqs	= omap2_timer8_mpu_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpt8_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPT8_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPT8_SHIFT,
		},
	},
<<<<<<< HEAD
	.slaves		= omap2430_timer8_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer8_slaves),
	.class		= &omap2430_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
=======
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= omap2430_timer8_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer8_slaves),
	.class		= &omap2xxx_timer_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* timer9 */
static struct omap_hwmod omap2430_timer9_hwmod;
<<<<<<< HEAD
static struct omap_hwmod_irq_info omap2430_timer9_mpu_irqs[] = {
	{ .irq = 45, },
};

static struct omap_hwmod_addr_space omap2430_timer9_addrs[] = {
	{
		.pa_start	= 0x48084000,
		.pa_end		= 0x48084000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
};
=======
>>>>>>> refs/remotes/origin/cm-10.0

/* l4_core -> timer9 */
static struct omap_hwmod_ocp_if omap2430_l4_core__timer9 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_timer9_hwmod,
	.clk		= "gpt9_ick",
<<<<<<< HEAD
	.addr		= omap2430_timer9_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_timer9_addrs),
=======
	.addr		= omap2xxx_timer9_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer9 slave port */
static struct omap_hwmod_ocp_if *omap2430_timer9_slaves[] = {
	&omap2430_l4_core__timer9,
};

/* timer9 hwmod */
static struct omap_hwmod omap2430_timer9_hwmod = {
	.name		= "timer9",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_timer9_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_timer9_mpu_irqs),
=======
	.mpu_irqs	= omap2_timer9_mpu_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpt9_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPT9_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPT9_SHIFT,
		},
	},
<<<<<<< HEAD
	.slaves		= omap2430_timer9_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer9_slaves),
	.class		= &omap2430_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
=======
	.dev_attr	= &capability_pwm_dev_attr,
	.slaves		= omap2430_timer9_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer9_slaves),
	.class		= &omap2xxx_timer_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* timer10 */
static struct omap_hwmod omap2430_timer10_hwmod;
<<<<<<< HEAD
static struct omap_hwmod_irq_info omap2430_timer10_mpu_irqs[] = {
	{ .irq = 46, },
};

static struct omap_hwmod_addr_space omap2430_timer10_addrs[] = {
	{
		.pa_start	= 0x48086000,
		.pa_end		= 0x48086000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
};
=======
>>>>>>> refs/remotes/origin/cm-10.0

/* l4_core -> timer10 */
static struct omap_hwmod_ocp_if omap2430_l4_core__timer10 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_timer10_hwmod,
	.clk		= "gpt10_ick",
<<<<<<< HEAD
	.addr		= omap2430_timer10_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_timer10_addrs),
=======
	.addr		= omap2_timer10_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer10 slave port */
static struct omap_hwmod_ocp_if *omap2430_timer10_slaves[] = {
	&omap2430_l4_core__timer10,
};

/* timer10 hwmod */
static struct omap_hwmod omap2430_timer10_hwmod = {
	.name		= "timer10",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_timer10_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_timer10_mpu_irqs),
=======
	.mpu_irqs	= omap2_timer10_mpu_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpt10_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPT10_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPT10_SHIFT,
		},
	},
<<<<<<< HEAD
	.slaves		= omap2430_timer10_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer10_slaves),
	.class		= &omap2430_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
=======
	.dev_attr	= &capability_pwm_dev_attr,
	.slaves		= omap2430_timer10_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer10_slaves),
	.class		= &omap2xxx_timer_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* timer11 */
static struct omap_hwmod omap2430_timer11_hwmod;
<<<<<<< HEAD
static struct omap_hwmod_irq_info omap2430_timer11_mpu_irqs[] = {
	{ .irq = 47, },
};

static struct omap_hwmod_addr_space omap2430_timer11_addrs[] = {
	{
		.pa_start	= 0x48088000,
		.pa_end		= 0x48088000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
};
=======
>>>>>>> refs/remotes/origin/cm-10.0

/* l4_core -> timer11 */
static struct omap_hwmod_ocp_if omap2430_l4_core__timer11 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_timer11_hwmod,
	.clk		= "gpt11_ick",
<<<<<<< HEAD
	.addr		= omap2430_timer11_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_timer11_addrs),
=======
	.addr		= omap2_timer11_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer11 slave port */
static struct omap_hwmod_ocp_if *omap2430_timer11_slaves[] = {
	&omap2430_l4_core__timer11,
};

/* timer11 hwmod */
static struct omap_hwmod omap2430_timer11_hwmod = {
	.name		= "timer11",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_timer11_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_timer11_mpu_irqs),
=======
	.mpu_irqs	= omap2_timer11_mpu_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpt11_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPT11_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPT11_SHIFT,
		},
	},
<<<<<<< HEAD
	.slaves		= omap2430_timer11_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer11_slaves),
	.class		= &omap2430_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
=======
	.dev_attr	= &capability_pwm_dev_attr,
	.slaves		= omap2430_timer11_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer11_slaves),
	.class		= &omap2xxx_timer_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* timer12 */
static struct omap_hwmod omap2430_timer12_hwmod;
<<<<<<< HEAD
static struct omap_hwmod_irq_info omap2430_timer12_mpu_irqs[] = {
	{ .irq = 48, },
};

static struct omap_hwmod_addr_space omap2430_timer12_addrs[] = {
	{
		.pa_start	= 0x4808a000,
		.pa_end		= 0x4808a000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
};
=======
>>>>>>> refs/remotes/origin/cm-10.0

/* l4_core -> timer12 */
static struct omap_hwmod_ocp_if omap2430_l4_core__timer12 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_timer12_hwmod,
	.clk		= "gpt12_ick",
<<<<<<< HEAD
	.addr		= omap2430_timer12_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_timer12_addrs),
=======
	.addr		= omap2xxx_timer12_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer12 slave port */
static struct omap_hwmod_ocp_if *omap2430_timer12_slaves[] = {
	&omap2430_l4_core__timer12,
};

/* timer12 hwmod */
static struct omap_hwmod omap2430_timer12_hwmod = {
	.name		= "timer12",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_timer12_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_timer12_mpu_irqs),
=======
	.mpu_irqs	= omap2xxx_timer12_mpu_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpt12_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPT12_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPT12_SHIFT,
		},
	},
<<<<<<< HEAD
	.slaves		= omap2430_timer12_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer12_slaves),
	.class		= &omap2430_timer_hwmod_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
=======
	.dev_attr	= &capability_pwm_dev_attr,
	.slaves		= omap2430_timer12_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_timer12_slaves),
	.class		= &omap2xxx_timer_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct omap_hwmod_class omap2430_mmc_class = {
	.name = "mmc",
	.sysc = &omap2430_mmc_sysc,
};

/* MMC/SD/SDIO1 */
static struct omap_hwmod_irq_info omap2430_mmc1_mpu_irqs[] = {
	{ .irq = 83 + OMAP_INTC_START, },
	{ .irq = -1 },
};

static struct omap_hwmod_dma_info omap2430_mmc1_sdma_reqs[] = {
	{ .name = "tx",	.dma_req = 61 }, /* DMA_MMC1_TX */
	{ .name = "rx",	.dma_req = 62 }, /* DMA_MMC1_RX */
	{ .dma_req = -1 }
};

static struct omap_hwmod_opt_clk omap2430_mmc1_opt_clks[] = {
	{ .role = "dbck", .clk = "mmchsdb1_fck" },
};

static struct omap_mmc_dev_attr mmc1_dev_attr = {
	.flags = OMAP_HSMMC_SUPPORTS_DUAL_VOLT,
};

static struct omap_hwmod omap2430_mmc1_hwmod = {
	.name		= "mmc1",
	.flags		= HWMOD_CONTROL_OPT_CLKS_IN_RESET,
	.mpu_irqs	= omap2430_mmc1_mpu_irqs,
	.sdma_reqs	= omap2430_mmc1_sdma_reqs,
	.opt_clks	= omap2430_mmc1_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(omap2430_mmc1_opt_clks),
	.main_clk	= "mmchs1_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 2,
			.module_bit  = OMAP2430_EN_MMCHS1_SHIFT,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_MMCHS1_SHIFT,
		},
	},
	.dev_attr	= &mmc1_dev_attr,
	.class		= &omap2430_mmc_class,
};

/* MMC/SD/SDIO2 */
static struct omap_hwmod_irq_info omap2430_mmc2_mpu_irqs[] = {
	{ .irq = 86 + OMAP_INTC_START, },
	{ .irq = -1 },
};

static struct omap_hwmod_dma_info omap2430_mmc2_sdma_reqs[] = {
	{ .name = "tx",	.dma_req = 47 }, /* DMA_MMC2_TX */
	{ .name = "rx",	.dma_req = 48 }, /* DMA_MMC2_RX */
	{ .dma_req = -1 }
};

static struct omap_hwmod_opt_clk omap2430_mmc2_opt_clks[] = {
	{ .role = "dbck", .clk = "mmchsdb2_fck" },
};

static struct omap_hwmod omap2430_mmc2_hwmod = {
	.name		= "mmc2",
	.flags		= HWMOD_CONTROL_OPT_CLKS_IN_RESET,
	.mpu_irqs	= omap2430_mmc2_mpu_irqs,
	.sdma_reqs	= omap2430_mmc2_sdma_reqs,
	.opt_clks	= omap2430_mmc2_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(omap2430_mmc2_opt_clks),
	.main_clk	= "mmchs2_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 2,
			.module_bit  = OMAP2430_EN_MMCHS2_SHIFT,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_MMCHS2_SHIFT,
		},
	},
	.class		= &omap2430_mmc_class,
};

/* HDQ1W/1-wire */
static struct omap_hwmod omap2430_hdq1w_hwmod = {
	.name		= "hdq1w",
	.mpu_irqs	= omap2_hdq1w_mpu_irqs,
	.main_clk	= "hdq_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_HDQ_SHIFT,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_HDQ_SHIFT,
		},
	},
	.class		= &omap2_hdq1w_class,
};

/*
 * interfaces
 */

/* L3 -> L4_CORE interface */
/* l3_core -> usbhsotg  interface */
static struct omap_hwmod_ocp_if omap2430_usbhsotg__l3 = {
	.master		= &omap2430_usbhsotg_hwmod,
	.slave		= &omap2xxx_l3_main_hwmod,
	.clk		= "core_l3_ck",
	.user		= OCP_USER_MPU,
};

/* L4 CORE -> I2C1 interface */
static struct omap_hwmod_ocp_if omap2430_l4_core__i2c1 = {
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_i2c1_hwmod,
	.clk		= "i2c1_ick",
	.addr		= omap2_i2c1_addr_space,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 CORE -> I2C2 interface */
static struct omap_hwmod_ocp_if omap2430_l4_core__i2c2 = {
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_i2c2_hwmod,
	.clk		= "i2c2_ick",
	.addr		= omap2_i2c2_addr_space,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

static struct omap_hwmod_addr_space omap2430_usbhsotg_addrs[] = {
	{
		.pa_start	= OMAP243X_HS_BASE,
		.pa_end		= OMAP243X_HS_BASE + SZ_4K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

/*  l4_core ->usbhsotg  interface */
static struct omap_hwmod_ocp_if omap2430_l4_core__usbhsotg = {
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_usbhsotg_hwmod,
	.clk		= "usb_l4_ick",
	.addr		= omap2430_usbhsotg_addrs,
	.user		= OCP_USER_MPU,
};

/* L4 CORE -> MMC1 interface */
static struct omap_hwmod_ocp_if omap2430_l4_core__mmc1 = {
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_mmc1_hwmod,
	.clk		= "mmchs1_ick",
	.addr		= omap2430_mmc1_addr_space,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 CORE -> MMC2 interface */
static struct omap_hwmod_ocp_if omap2430_l4_core__mmc2 = {
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_mmc2_hwmod,
	.clk		= "mmchs2_ick",
	.addr		= omap2430_mmc2_addr_space,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4 core -> mcspi3 interface */
static struct omap_hwmod_ocp_if omap2430_l4_core__mcspi3 = {
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_mcspi3_hwmod,
	.clk		= "mcspi3_ick",
	.addr		= omap2430_mcspi3_addr_space,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* IVA2 <- L3 interface */
static struct omap_hwmod_ocp_if omap2430_l3__iva = {
	.master		= &omap2xxx_l3_main_hwmod,
	.slave		= &omap2430_iva_hwmod,
	.clk		= "core_l3_ck",
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

static struct omap_hwmod_addr_space omap2430_timer1_addrs[] = {
	{
		.pa_start	= 0x49018000,
		.pa_end		= 0x49018000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

/* l4_wkup -> timer1 */
static struct omap_hwmod_ocp_if omap2430_l4_wkup__timer1 = {
	.master		= &omap2xxx_l4_wkup_hwmod,
	.slave		= &omap2xxx_timer1_hwmod,
	.clk		= "gpt1_ick",
	.addr		= omap2430_timer1_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
>>>>>>> refs/remotes/origin/master
};

/* l4_wkup -> wd_timer2 */
static struct omap_hwmod_addr_space omap2430_wd_timer2_addrs[] = {
	{
		.pa_start	= 0x49016000,
		.pa_end		= 0x4901607f,
		.flags		= ADDR_TYPE_RT
	},
<<<<<<< HEAD
<<<<<<< HEAD
=======
	{ }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_ocp_if omap2430_l4_wkup__wd_timer2 = {
	.master		= &omap2430_l4_wkup_hwmod,
	.slave		= &omap2430_wd_timer2_hwmod,
	.clk		= "mpu_wdt_ick",
	.addr		= omap2430_wd_timer2_addrs,
<<<<<<< HEAD
	.addr_cnt	= ARRAY_SIZE(omap2430_wd_timer2_addrs),
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/*
 * 'wd_timer' class
 * 32-bit watchdog upward counter that generates a pulse on the reset pin on
 * overflow condition
 */

static struct omap_hwmod_class_sysconfig omap2430_wd_timer_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_EMUFREE | SYSC_HAS_SOFTRESET |
			   SYSC_HAS_AUTOIDLE | SYSS_HAS_RESET_STATUS),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class omap2430_wd_timer_hwmod_class = {
	.name		= "wd_timer",
	.sysc		= &omap2430_wd_timer_sysc,
	.pre_shutdown	= &omap2_wd_timer_disable
};

=======
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

>>>>>>> refs/remotes/origin/cm-10.0
/* wd_timer2 */
static struct omap_hwmod_ocp_if *omap2430_wd_timer2_slaves[] = {
	&omap2430_l4_wkup__wd_timer2,
};

static struct omap_hwmod omap2430_wd_timer2_hwmod = {
	.name		= "wd_timer2",
<<<<<<< HEAD
	.class		= &omap2430_wd_timer_hwmod_class,
=======
	.class		= &omap2xxx_wd_timer_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "mpu_wdt_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_MPU_WDT_SHIFT,
			.module_offs = WKUP_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_MPU_WDT_SHIFT,
		},
	},
	.slaves		= omap2430_wd_timer2_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_wd_timer2_slaves),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
};

/* UART */

static struct omap_hwmod_class_sysconfig uart_sysc = {
	.rev_offs	= 0x50,
	.sysc_offs	= 0x54,
	.syss_offs	= 0x58,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE |
			   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
			   SYSC_HAS_AUTOIDLE | SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class uart_class = {
	.name = "uart",
	.sysc = &uart_sysc,
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

/* UART1 */

<<<<<<< HEAD
static struct omap_hwmod_irq_info uart1_mpu_irqs[] = {
	{ .irq = INT_24XX_UART1_IRQ, },
};

static struct omap_hwmod_dma_info uart1_sdma_reqs[] = {
	{ .name = "rx",	.dma_req = OMAP24XX_DMA_UART1_RX, },
	{ .name = "tx",	.dma_req = OMAP24XX_DMA_UART1_TX, },
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if *omap2430_uart1_slaves[] = {
	&omap2_l4_core__uart1,
};

static struct omap_hwmod omap2430_uart1_hwmod = {
	.name		= "uart1",
<<<<<<< HEAD
	.mpu_irqs	= uart1_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart1_mpu_irqs),
	.sdma_reqs	= uart1_sdma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart1_sdma_reqs),
=======
	.mpu_irqs	= omap2_uart1_mpu_irqs,
	.sdma_reqs	= omap2_uart1_sdma_reqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "uart1_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_UART1_SHIFT,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_EN_UART1_SHIFT,
		},
	},
	.slaves		= omap2430_uart1_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_uart1_slaves),
<<<<<<< HEAD
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
	.class		= &omap2_uart_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* UART2 */

<<<<<<< HEAD
static struct omap_hwmod_irq_info uart2_mpu_irqs[] = {
	{ .irq = INT_24XX_UART2_IRQ, },
};

static struct omap_hwmod_dma_info uart2_sdma_reqs[] = {
	{ .name = "rx",	.dma_req = OMAP24XX_DMA_UART2_RX, },
	{ .name = "tx",	.dma_req = OMAP24XX_DMA_UART2_TX, },
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if *omap2430_uart2_slaves[] = {
	&omap2_l4_core__uart2,
};

static struct omap_hwmod omap2430_uart2_hwmod = {
	.name		= "uart2",
<<<<<<< HEAD
	.mpu_irqs	= uart2_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart2_mpu_irqs),
	.sdma_reqs	= uart2_sdma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart2_sdma_reqs),
=======
	.mpu_irqs	= omap2_uart2_mpu_irqs,
	.sdma_reqs	= omap2_uart2_sdma_reqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "uart2_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_UART2_SHIFT,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_EN_UART2_SHIFT,
		},
	},
	.slaves		= omap2430_uart2_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_uart2_slaves),
<<<<<<< HEAD
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
	.class		= &omap2_uart_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* UART3 */

<<<<<<< HEAD
static struct omap_hwmod_irq_info uart3_mpu_irqs[] = {
	{ .irq = INT_24XX_UART3_IRQ, },
};

static struct omap_hwmod_dma_info uart3_sdma_reqs[] = {
	{ .name = "rx",	.dma_req = OMAP24XX_DMA_UART3_RX, },
	{ .name = "tx",	.dma_req = OMAP24XX_DMA_UART3_TX, },
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if *omap2430_uart3_slaves[] = {
	&omap2_l4_core__uart3,
};

static struct omap_hwmod omap2430_uart3_hwmod = {
	.name		= "uart3",
<<<<<<< HEAD
	.mpu_irqs	= uart3_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart3_mpu_irqs),
	.sdma_reqs	= uart3_sdma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart3_sdma_reqs),
=======
	.mpu_irqs	= omap2_uart3_mpu_irqs,
	.sdma_reqs	= omap2_uart3_sdma_reqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "uart3_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 2,
			.module_bit = OMAP24XX_EN_UART3_SHIFT,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP24XX_EN_UART3_SHIFT,
		},
	},
	.slaves		= omap2430_uart3_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_uart3_slaves),
<<<<<<< HEAD
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
};

/*
 * 'dss' class
 * display sub-system
 */

static struct omap_hwmod_class_sysconfig omap2430_dss_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_SOFTRESET | SYSC_HAS_AUTOIDLE),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class omap2430_dss_hwmod_class = {
	.name = "dss",
	.sysc = &omap2430_dss_sysc,
};

static struct omap_hwmod_dma_info omap2430_dss_sdma_chs[] = {
	{ .name = "dispc", .dma_req = 5 },
=======
	.class		= &omap2_uart_class,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* dss */
/* dss master ports */
static struct omap_hwmod_ocp_if *omap2430_dss_masters[] = {
	&omap2430_dss__l3,
};

<<<<<<< HEAD
static struct omap_hwmod_addr_space omap2430_dss_addrs[] = {
	{
		.pa_start	= 0x48050000,
		.pa_end		= 0x480503FF,
		.flags		= ADDR_TYPE_RT
	},
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
/* l4_core -> dss */
static struct omap_hwmod_ocp_if omap2430_l4_core__dss = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_dss_core_hwmod,
	.clk		= "dss_ick",
<<<<<<< HEAD
	.addr		= omap2430_dss_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_dss_addrs),
=======
	.addr		= omap2_dss_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* dss slave ports */
static struct omap_hwmod_ocp_if *omap2430_dss_slaves[] = {
	&omap2430_l4_core__dss,
};

static struct omap_hwmod_opt_clk dss_opt_clks[] = {
<<<<<<< HEAD
=======
	/*
	 * The DSS HW needs all DSS clocks enabled during reset. The dss_core
	 * driver does not use these clocks.
	 */
>>>>>>> refs/remotes/origin/cm-10.0
	{ .role = "tv_clk", .clk = "dss_54m_fck" },
	{ .role = "sys_clk", .clk = "dss2_fck" },
};

static struct omap_hwmod omap2430_dss_core_hwmod = {
	.name		= "dss_core",
<<<<<<< HEAD
	.class		= &omap2430_dss_hwmod_class,
	.main_clk	= "dss1_fck", /* instead of dss_fck */
	.sdma_reqs	= omap2430_dss_sdma_chs,
	.sdma_reqs_cnt	= ARRAY_SIZE(omap2430_dss_sdma_chs),
=======
	.class		= &omap2_dss_hwmod_class,
	.main_clk	= "dss1_fck", /* instead of dss_fck */
	.sdma_reqs	= omap2xxx_dss_sdma_chs,
>>>>>>> refs/remotes/origin/cm-10.0
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_DSS1_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_stdby_bit = OMAP24XX_ST_DSS_SHIFT,
		},
	},
	.opt_clks	= dss_opt_clks,
	.opt_clks_cnt = ARRAY_SIZE(dss_opt_clks),
	.slaves		= omap2430_dss_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_dss_slaves),
	.masters	= omap2430_dss_masters,
	.masters_cnt	= ARRAY_SIZE(omap2430_dss_masters),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
	.flags		= HWMOD_NO_IDLEST,
};

/*
 * 'dispc' class
 * display controller
 */

static struct omap_hwmod_class_sysconfig omap2430_dispc_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_MIDLEMODE |
			   SYSC_HAS_SOFTRESET | SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART |
			   MSTANDBY_FORCE | MSTANDBY_NO | MSTANDBY_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class omap2430_dispc_hwmod_class = {
	.name = "dispc",
	.sysc = &omap2430_dispc_sysc,
};

static struct omap_hwmod_irq_info omap2430_dispc_irqs[] = {
	{ .irq = 25 },
};

static struct omap_hwmod_addr_space omap2430_dss_dispc_addrs[] = {
	{
		.pa_start	= 0x48050400,
		.pa_end		= 0x480507FF,
		.flags		= ADDR_TYPE_RT
	},
=======
	.flags		= HWMOD_NO_IDLEST | HWMOD_CONTROL_OPT_CLKS_IN_RESET,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* l4_core -> dss_dispc */
static struct omap_hwmod_ocp_if omap2430_l4_core__dss_dispc = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_dss_dispc_hwmod,
	.clk		= "dss_ick",
<<<<<<< HEAD
	.addr		= omap2430_dss_dispc_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_dss_dispc_addrs),
=======
	.addr		= omap2_dss_dispc_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* dss_dispc slave ports */
static struct omap_hwmod_ocp_if *omap2430_dss_dispc_slaves[] = {
	&omap2430_l4_core__dss_dispc,
};

static struct omap_hwmod omap2430_dss_dispc_hwmod = {
	.name		= "dss_dispc",
<<<<<<< HEAD
	.class		= &omap2430_dispc_hwmod_class,
	.mpu_irqs	= omap2430_dispc_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_dispc_irqs),
=======
	.class		= &omap2_dispc_hwmod_class,
	.mpu_irqs	= omap2_dispc_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "dss1_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_DSS1_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_stdby_bit = OMAP24XX_ST_DSS_SHIFT,
		},
	},
	.slaves		= omap2430_dss_dispc_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_dss_dispc_slaves),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
	.flags		= HWMOD_NO_IDLEST,
};

/*
 * 'rfbi' class
 * remote frame buffer interface
 */

static struct omap_hwmod_class_sysconfig omap2430_rfbi_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_SOFTRESET |
			   SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class omap2430_rfbi_hwmod_class = {
	.name = "rfbi",
	.sysc = &omap2430_rfbi_sysc,
};

static struct omap_hwmod_addr_space omap2430_dss_rfbi_addrs[] = {
	{
		.pa_start	= 0x48050800,
		.pa_end		= 0x48050BFF,
		.flags		= ADDR_TYPE_RT
	},
=======
	.flags		= HWMOD_NO_IDLEST,
	.dev_attr	= &omap2_3_dss_dispc_dev_attr
>>>>>>> refs/remotes/origin/cm-10.0
};

/* l4_core -> dss_rfbi */
static struct omap_hwmod_ocp_if omap2430_l4_core__dss_rfbi = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_dss_rfbi_hwmod,
	.clk		= "dss_ick",
<<<<<<< HEAD
	.addr		= omap2430_dss_rfbi_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_dss_rfbi_addrs),
=======
	.addr		= omap2_dss_rfbi_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* dss_rfbi slave ports */
static struct omap_hwmod_ocp_if *omap2430_dss_rfbi_slaves[] = {
	&omap2430_l4_core__dss_rfbi,
};

<<<<<<< HEAD
static struct omap_hwmod omap2430_dss_rfbi_hwmod = {
	.name		= "dss_rfbi",
	.class		= &omap2430_rfbi_hwmod_class,
=======
static struct omap_hwmod_opt_clk dss_rfbi_opt_clks[] = {
	{ .role = "ick", .clk = "dss_ick" },
};

static struct omap_hwmod omap2430_dss_rfbi_hwmod = {
	.name		= "dss_rfbi",
	.class		= &omap2_rfbi_hwmod_class,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "dss1_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_DSS1_SHIFT,
			.module_offs = CORE_MOD,
		},
	},
<<<<<<< HEAD
	.slaves		= omap2430_dss_rfbi_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_dss_rfbi_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
	.flags		= HWMOD_NO_IDLEST,
};

/*
 * 'venc' class
 * video encoder
 */

static struct omap_hwmod_class omap2430_venc_hwmod_class = {
	.name = "venc",
};

/* dss_venc */
static struct omap_hwmod_addr_space omap2430_dss_venc_addrs[] = {
	{
		.pa_start	= 0x48050C00,
		.pa_end		= 0x48050FFF,
		.flags		= ADDR_TYPE_RT
	},
};

=======
	.opt_clks	= dss_rfbi_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(dss_rfbi_opt_clks),
	.slaves		= omap2430_dss_rfbi_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_dss_rfbi_slaves),
	.flags		= HWMOD_NO_IDLEST,
};

>>>>>>> refs/remotes/origin/cm-10.0
/* l4_core -> dss_venc */
static struct omap_hwmod_ocp_if omap2430_l4_core__dss_venc = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_dss_venc_hwmod,
<<<<<<< HEAD
	.clk		= "dss_54m_fck",
	.addr		= omap2430_dss_venc_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_dss_venc_addrs),
	.flags		= OCPIF_SWSUP_IDLE,
=======
	.clk		= "dss_ick",
	.addr		= omap2_dss_venc_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* dss_venc slave ports */
static struct omap_hwmod_ocp_if *omap2430_dss_venc_slaves[] = {
	&omap2430_l4_core__dss_venc,
};

static struct omap_hwmod omap2430_dss_venc_hwmod = {
	.name		= "dss_venc",
<<<<<<< HEAD
	.class		= &omap2430_venc_hwmod_class,
	.main_clk	= "dss1_fck",
=======
	.class		= &omap2_venc_hwmod_class,
	.main_clk	= "dss_54m_fck",
>>>>>>> refs/remotes/origin/cm-10.0
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_DSS1_SHIFT,
			.module_offs = CORE_MOD,
		},
	},
	.slaves		= omap2430_dss_venc_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_dss_venc_slaves),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.flags		= HWMOD_NO_IDLEST,
};

/* I2C common */
static struct omap_hwmod_class_sysconfig i2c_sysc = {
	.rev_offs	= 0x00,
	.sysc_offs	= 0x20,
	.syss_offs	= 0x10,
	.sysc_flags	= (SYSC_HAS_SOFTRESET | SYSC_HAS_AUTOIDLE |
			   SYSS_HAS_RESET_STATUS),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class i2c_class = {
	.name		= "i2c",
	.sysc		= &i2c_sysc,
<<<<<<< HEAD
=======
	.rev		= OMAP_I2C_IP_VERSION_1,
	.reset		= &omap_i2c_reset,
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_i2c_dev_attr i2c_dev_attr = {
	.fifo_depth	= 8, /* bytes */
<<<<<<< HEAD
=======
	.flags		= OMAP_I2C_FLAG_APPLY_ERRATA_I207 |
			  OMAP_I2C_FLAG_BUS_SHIFT_2 |
			  OMAP_I2C_FLAG_FORCE_19200_INT_CLK,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* I2C1 */

<<<<<<< HEAD
static struct omap_hwmod_irq_info i2c1_mpu_irqs[] = {
	{ .irq = INT_24XX_I2C1_IRQ, },
};

static struct omap_hwmod_dma_info i2c1_sdma_reqs[] = {
	{ .name = "tx", .dma_req = OMAP24XX_DMA_I2C1_TX },
	{ .name = "rx", .dma_req = OMAP24XX_DMA_I2C1_RX },
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if *omap2430_i2c1_slaves[] = {
	&omap2430_l4_core__i2c1,
};

static struct omap_hwmod omap2430_i2c1_hwmod = {
	.name		= "i2c1",
<<<<<<< HEAD
	.mpu_irqs	= i2c1_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(i2c1_mpu_irqs),
	.sdma_reqs	= i2c1_sdma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(i2c1_sdma_reqs),
=======
	.flags		= HWMOD_16BIT_REG,
	.mpu_irqs	= omap2_i2c1_mpu_irqs,
	.sdma_reqs	= omap2_i2c1_sdma_reqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "i2chs1_fck",
	.prcm		= {
		.omap2 = {
			/*
			 * NOTE: The CM_FCLKEN* and CM_ICLKEN* for
			 * I2CHS IP's do not follow the usual pattern.
			 * prcm_reg_id alone cannot be used to program
			 * the iclk and fclk. Needs to be handled using
			 * additional flags when clk handling is moved
			 * to hwmod framework.
			 */
			.module_offs = CORE_MOD,
			.prcm_reg_id = 1,
			.module_bit = OMAP2430_EN_I2CHS1_SHIFT,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP2430_ST_I2CHS1_SHIFT,
		},
	},
	.slaves		= omap2430_i2c1_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_i2c1_slaves),
	.class		= &i2c_class,
	.dev_attr	= &i2c_dev_attr,
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

/* I2C2 */

<<<<<<< HEAD
static struct omap_hwmod_irq_info i2c2_mpu_irqs[] = {
	{ .irq = INT_24XX_I2C2_IRQ, },
};

static struct omap_hwmod_dma_info i2c2_sdma_reqs[] = {
	{ .name = "tx", .dma_req = OMAP24XX_DMA_I2C2_TX },
	{ .name = "rx", .dma_req = OMAP24XX_DMA_I2C2_RX },
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if *omap2430_i2c2_slaves[] = {
	&omap2430_l4_core__i2c2,
};

static struct omap_hwmod omap2430_i2c2_hwmod = {
	.name		= "i2c2",
<<<<<<< HEAD
	.mpu_irqs	= i2c2_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(i2c2_mpu_irqs),
	.sdma_reqs	= i2c2_sdma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(i2c2_sdma_reqs),
=======
	.flags		= HWMOD_16BIT_REG,
	.mpu_irqs	= omap2_i2c2_mpu_irqs,
	.sdma_reqs	= omap2_i2c2_sdma_reqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "i2chs2_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 1,
			.module_bit = OMAP2430_EN_I2CHS2_SHIFT,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP2430_ST_I2CHS2_SHIFT,
		},
	},
	.slaves		= omap2430_i2c2_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_i2c2_slaves),
	.class		= &i2c_class,
	.dev_attr	= &i2c_dev_attr,
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

/* l4_wkup -> gpio1 */
static struct omap_hwmod_addr_space omap2430_gpio1_addr_space[] = {
	{
		.pa_start	= 0x4900C000,
		.pa_end		= 0x4900C1ff,
		.flags		= ADDR_TYPE_RT
	},
<<<<<<< HEAD
=======
	{ }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_ocp_if omap2430_l4_wkup__gpio1 = {
	.master		= &omap2430_l4_wkup_hwmod,
	.slave		= &omap2430_gpio1_hwmod,
	.clk		= "gpios_ick",
	.addr		= omap2430_gpio1_addr_space,
<<<<<<< HEAD
	.addr_cnt	= ARRAY_SIZE(omap2430_gpio1_addr_space),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4_wkup -> gpio2 */
static struct omap_hwmod_addr_space omap2430_gpio2_addr_space[] = {
	{
		.pa_start	= 0x4900E000,
		.pa_end		= 0x4900E1ff,
		.flags		= ADDR_TYPE_RT
	},
<<<<<<< HEAD
=======
	{ }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_ocp_if omap2430_l4_wkup__gpio2 = {
	.master		= &omap2430_l4_wkup_hwmod,
	.slave		= &omap2430_gpio2_hwmod,
	.clk		= "gpios_ick",
	.addr		= omap2430_gpio2_addr_space,
<<<<<<< HEAD
	.addr_cnt	= ARRAY_SIZE(omap2430_gpio2_addr_space),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4_wkup -> gpio3 */
static struct omap_hwmod_addr_space omap2430_gpio3_addr_space[] = {
	{
		.pa_start	= 0x49010000,
		.pa_end		= 0x490101ff,
		.flags		= ADDR_TYPE_RT
	},
<<<<<<< HEAD
=======
	{ }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_ocp_if omap2430_l4_wkup__gpio3 = {
	.master		= &omap2430_l4_wkup_hwmod,
	.slave		= &omap2430_gpio3_hwmod,
	.clk		= "gpios_ick",
	.addr		= omap2430_gpio3_addr_space,
<<<<<<< HEAD
	.addr_cnt	= ARRAY_SIZE(omap2430_gpio3_addr_space),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4_wkup -> gpio4 */
static struct omap_hwmod_addr_space omap2430_gpio4_addr_space[] = {
	{
		.pa_start	= 0x49012000,
		.pa_end		= 0x490121ff,
		.flags		= ADDR_TYPE_RT
	},
<<<<<<< HEAD
=======
	{ }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_ocp_if omap2430_l4_wkup__gpio4 = {
	.master		= &omap2430_l4_wkup_hwmod,
	.slave		= &omap2430_gpio4_hwmod,
	.clk		= "gpios_ick",
	.addr		= omap2430_gpio4_addr_space,
<<<<<<< HEAD
	.addr_cnt	= ARRAY_SIZE(omap2430_gpio4_addr_space),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4_core -> gpio5 */
static struct omap_hwmod_addr_space omap2430_gpio5_addr_space[] = {
	{
		.pa_start	= 0x480B6000,
		.pa_end		= 0x480B61ff,
		.flags		= ADDR_TYPE_RT
	},
<<<<<<< HEAD
=======
	{ }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_ocp_if omap2430_l4_core__gpio5 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_gpio5_hwmod,
	.clk		= "gpio5_ick",
	.addr		= omap2430_gpio5_addr_space,
<<<<<<< HEAD
	.addr_cnt	= ARRAY_SIZE(omap2430_gpio5_addr_space),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* gpio dev_attr */
static struct omap_gpio_dev_attr gpio_dev_attr = {
	.bank_width = 32,
	.dbck_flag = false,
};

<<<<<<< HEAD
static struct omap_hwmod_class_sysconfig omap243x_gpio_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_ENAWAKEUP | SYSC_HAS_SIDLEMODE |
			   SYSC_HAS_SOFTRESET | SYSC_HAS_AUTOIDLE |
			   SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

/*
 * 'gpio' class
 * general purpose io module
 */
static struct omap_hwmod_class omap243x_gpio_hwmod_class = {
	.name = "gpio",
	.sysc = &omap243x_gpio_sysc,
	.rev = 0,
};

/* gpio1 */
static struct omap_hwmod_irq_info omap243x_gpio1_irqs[] = {
	{ .irq = 29 }, /* INT_24XX_GPIO_BANK1 */
};

=======
/* gpio1 */
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if *omap2430_gpio1_slaves[] = {
	&omap2430_l4_wkup__gpio1,
};

static struct omap_hwmod omap2430_gpio1_hwmod = {
	.name		= "gpio1",
	.flags		= HWMOD_CONTROL_OPT_CLKS_IN_RESET,
<<<<<<< HEAD
	.mpu_irqs	= omap243x_gpio1_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap243x_gpio1_irqs),
=======
	.mpu_irqs	= omap2_gpio1_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpios_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPIOS_SHIFT,
			.module_offs = WKUP_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_EN_GPIOS_SHIFT,
		},
	},
	.slaves		= omap2430_gpio1_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_gpio1_slaves),
<<<<<<< HEAD
	.class		= &omap243x_gpio_hwmod_class,
	.dev_attr	= &gpio_dev_attr,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
};

/* gpio2 */
static struct omap_hwmod_irq_info omap243x_gpio2_irqs[] = {
	{ .irq = 30 }, /* INT_24XX_GPIO_BANK2 */
};

=======
	.class		= &omap2xxx_gpio_hwmod_class,
	.dev_attr	= &gpio_dev_attr,
};

/* gpio2 */
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if *omap2430_gpio2_slaves[] = {
	&omap2430_l4_wkup__gpio2,
};

static struct omap_hwmod omap2430_gpio2_hwmod = {
	.name		= "gpio2",
	.flags		= HWMOD_CONTROL_OPT_CLKS_IN_RESET,
<<<<<<< HEAD
	.mpu_irqs	= omap243x_gpio2_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap243x_gpio2_irqs),
=======
	.mpu_irqs	= omap2_gpio2_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpios_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPIOS_SHIFT,
			.module_offs = WKUP_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPIOS_SHIFT,
		},
	},
	.slaves		= omap2430_gpio2_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_gpio2_slaves),
<<<<<<< HEAD
	.class		= &omap243x_gpio_hwmod_class,
	.dev_attr	= &gpio_dev_attr,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
};

/* gpio3 */
static struct omap_hwmod_irq_info omap243x_gpio3_irqs[] = {
	{ .irq = 31 }, /* INT_24XX_GPIO_BANK3 */
};

=======
	.class		= &omap2xxx_gpio_hwmod_class,
	.dev_attr	= &gpio_dev_attr,
};

/* gpio3 */
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if *omap2430_gpio3_slaves[] = {
	&omap2430_l4_wkup__gpio3,
};

static struct omap_hwmod omap2430_gpio3_hwmod = {
	.name		= "gpio3",
	.flags		= HWMOD_CONTROL_OPT_CLKS_IN_RESET,
<<<<<<< HEAD
	.mpu_irqs	= omap243x_gpio3_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap243x_gpio3_irqs),
=======
	.mpu_irqs	= omap2_gpio3_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpios_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPIOS_SHIFT,
			.module_offs = WKUP_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPIOS_SHIFT,
		},
	},
	.slaves		= omap2430_gpio3_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_gpio3_slaves),
<<<<<<< HEAD
	.class		= &omap243x_gpio_hwmod_class,
	.dev_attr	= &gpio_dev_attr,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
};

/* gpio4 */
static struct omap_hwmod_irq_info omap243x_gpio4_irqs[] = {
	{ .irq = 32 }, /* INT_24XX_GPIO_BANK4 */
};

=======
	.class		= &omap2xxx_gpio_hwmod_class,
	.dev_attr	= &gpio_dev_attr,
};

/* gpio4 */
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if *omap2430_gpio4_slaves[] = {
	&omap2430_l4_wkup__gpio4,
};

static struct omap_hwmod omap2430_gpio4_hwmod = {
	.name		= "gpio4",
	.flags		= HWMOD_CONTROL_OPT_CLKS_IN_RESET,
<<<<<<< HEAD
	.mpu_irqs	= omap243x_gpio4_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap243x_gpio4_irqs),
=======
	.mpu_irqs	= omap2_gpio4_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpios_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_GPIOS_SHIFT,
			.module_offs = WKUP_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_GPIOS_SHIFT,
		},
	},
	.slaves		= omap2430_gpio4_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_gpio4_slaves),
<<<<<<< HEAD
	.class		= &omap243x_gpio_hwmod_class,
	.dev_attr	= &gpio_dev_attr,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
	.class		= &omap2xxx_gpio_hwmod_class,
	.dev_attr	= &gpio_dev_attr,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* gpio5 */
static struct omap_hwmod_irq_info omap243x_gpio5_irqs[] = {
	{ .irq = 33 }, /* INT_24XX_GPIO_BANK5 */
<<<<<<< HEAD
=======
	{ .irq = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_ocp_if *omap2430_gpio5_slaves[] = {
	&omap2430_l4_core__gpio5,
};

static struct omap_hwmod omap2430_gpio5_hwmod = {
	.name		= "gpio5",
	.flags		= HWMOD_CONTROL_OPT_CLKS_IN_RESET,
	.mpu_irqs	= omap243x_gpio5_irqs,
<<<<<<< HEAD
	.mpu_irqs_cnt	= ARRAY_SIZE(omap243x_gpio5_irqs),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "gpio5_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 2,
			.module_bit = OMAP2430_EN_GPIO5_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_GPIO5_SHIFT,
		},
	},
	.slaves		= omap2430_gpio5_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_gpio5_slaves),
<<<<<<< HEAD
	.class		= &omap243x_gpio_hwmod_class,
	.dev_attr	= &gpio_dev_attr,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
};

/* dma_system */
static struct omap_hwmod_class_sysconfig omap2430_dma_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x002c,
	.syss_offs	= 0x0028,
	.sysc_flags	= (SYSC_HAS_SOFTRESET | SYSC_HAS_MIDLEMODE |
			   SYSC_HAS_CLOCKACTIVITY | SYSC_HAS_EMUFREE |
			   SYSC_HAS_AUTOIDLE | SYSS_HAS_RESET_STATUS),
	.idlemodes	= (MSTANDBY_FORCE | MSTANDBY_NO | MSTANDBY_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class omap2430_dma_hwmod_class = {
	.name = "dma",
	.sysc = &omap2430_dma_sysc,
=======
	.class		= &omap2xxx_gpio_hwmod_class,
	.dev_attr	= &gpio_dev_attr,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* dma attributes */
static struct omap_dma_dev_attr dma_dev_attr = {
	.dev_caps  = RESERVE_CHANNEL | DMA_LINKED_LCH | GLOBAL_PRIORITY |
				IS_CSSA_32 | IS_CDSA_32 | IS_RW_PRIORITY,
	.lch_count = 32,
};

<<<<<<< HEAD
static struct omap_hwmod_irq_info omap2430_dma_system_irqs[] = {
	{ .name = "0", .irq = 12 }, /* INT_24XX_SDMA_IRQ0 */
	{ .name = "1", .irq = 13 }, /* INT_24XX_SDMA_IRQ1 */
	{ .name = "2", .irq = 14 }, /* INT_24XX_SDMA_IRQ2 */
	{ .name = "3", .irq = 15 }, /* INT_24XX_SDMA_IRQ3 */
};

static struct omap_hwmod_addr_space omap2430_dma_system_addrs[] = {
	{
		.pa_start	= 0x48056000,
		.pa_end		= 0x48056fff,
		.flags		= ADDR_TYPE_RT
	},
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
/* dma_system -> L3 */
static struct omap_hwmod_ocp_if omap2430_dma_system__l3 = {
	.master		= &omap2430_dma_system_hwmod,
	.slave		= &omap2430_l3_main_hwmod,
	.clk		= "core_l3_ck",
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* dma_system master ports */
static struct omap_hwmod_ocp_if *omap2430_dma_system_masters[] = {
	&omap2430_dma_system__l3,
};

/* l4_core -> dma_system */
static struct omap_hwmod_ocp_if omap2430_l4_core__dma_system = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_dma_system_hwmod,
	.clk		= "sdma_ick",
<<<<<<< HEAD
	.addr		= omap2430_dma_system_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_dma_system_addrs),
=======
	.addr		= omap2_dma_system_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* dma_system slave ports */
static struct omap_hwmod_ocp_if *omap2430_dma_system_slaves[] = {
	&omap2430_l4_core__dma_system,
};

static struct omap_hwmod omap2430_dma_system_hwmod = {
	.name		= "dma",
<<<<<<< HEAD
	.class		= &omap2430_dma_hwmod_class,
	.mpu_irqs	= omap2430_dma_system_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_dma_system_irqs),
=======
	.class		= &omap2xxx_dma_hwmod_class,
	.mpu_irqs	= omap2_dma_system_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "core_l3_ck",
	.slaves		= omap2430_dma_system_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_dma_system_slaves),
	.masters	= omap2430_dma_system_masters,
	.masters_cnt	= ARRAY_SIZE(omap2430_dma_system_masters),
	.dev_attr	= &dma_dev_attr,
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
	.flags		= HWMOD_NO_IDLEST,
};

/*
 * 'mailbox' class
 * mailbox module allowing communication between the on-chip processors
 * using a queued mailbox-interrupt mechanism.
 */

static struct omap_hwmod_class_sysconfig omap2430_mailbox_sysc = {
	.rev_offs	= 0x000,
	.sysc_offs	= 0x010,
	.syss_offs	= 0x014,
	.sysc_flags	= (SYSC_HAS_CLOCKACTIVITY | SYSC_HAS_SIDLEMODE |
				SYSC_HAS_SOFTRESET | SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class omap2430_mailbox_hwmod_class = {
	.name = "mailbox",
	.sysc = &omap2430_mailbox_sysc,
};

=======
	.flags		= HWMOD_NO_IDLEST,
};

>>>>>>> refs/remotes/origin/cm-10.0
/* mailbox */
static struct omap_hwmod omap2430_mailbox_hwmod;
static struct omap_hwmod_irq_info omap2430_mailbox_irqs[] = {
	{ .irq = 26 },
<<<<<<< HEAD
};

static struct omap_hwmod_addr_space omap2430_mailbox_addrs[] = {
	{
		.pa_start	= 0x48094000,
		.pa_end		= 0x480941ff,
		.flags		= ADDR_TYPE_RT,
	},
=======
	{ .irq = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

/* l4_core -> mailbox */
static struct omap_hwmod_ocp_if omap2430_l4_core__mailbox = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_mailbox_hwmod,
<<<<<<< HEAD
	.addr		= omap2430_mailbox_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_mailbox_addrs),
=======
	.addr		= omap2_mailbox_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* mailbox slave ports */
static struct omap_hwmod_ocp_if *omap2430_mailbox_slaves[] = {
	&omap2430_l4_core__mailbox,
};

static struct omap_hwmod omap2430_mailbox_hwmod = {
	.name		= "mailbox",
<<<<<<< HEAD
	.class		= &omap2430_mailbox_hwmod_class,
	.mpu_irqs	= omap2430_mailbox_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_mailbox_irqs),
=======
	.class		= &omap2xxx_mailbox_hwmod_class,
	.mpu_irqs	= omap2430_mailbox_irqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "mailboxes_ick",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_MAILBOXES_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_MAILBOXES_SHIFT,
		},
	},
	.slaves		= omap2430_mailbox_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_mailbox_slaves),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
};

/*
 * 'mcspi' class
 * multichannel serial port interface (mcspi) / master/slave synchronous serial
 * bus
 */

static struct omap_hwmod_class_sysconfig omap2430_mcspi_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_CLOCKACTIVITY | SYSC_HAS_SIDLEMODE |
				SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
				SYSC_HAS_AUTOIDLE | SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class omap2430_mcspi_class = {
	.name = "mcspi",
	.sysc = &omap2430_mcspi_sysc,
	.rev = OMAP2_MCSPI_REV,
};

/* mcspi1 */
static struct omap_hwmod_irq_info omap2430_mcspi1_mpu_irqs[] = {
	{ .irq = 65 },
};

static struct omap_hwmod_dma_info omap2430_mcspi1_sdma_reqs[] = {
	{ .name = "tx0", .dma_req = 35 }, /* DMA_SPI1_TX0 */
	{ .name = "rx0", .dma_req = 36 }, /* DMA_SPI1_RX0 */
	{ .name = "tx1", .dma_req = 37 }, /* DMA_SPI1_TX1 */
	{ .name = "rx1", .dma_req = 38 }, /* DMA_SPI1_RX1 */
	{ .name = "tx2", .dma_req = 39 }, /* DMA_SPI1_TX2 */
	{ .name = "rx2", .dma_req = 40 }, /* DMA_SPI1_RX2 */
	{ .name = "tx3", .dma_req = 41 }, /* DMA_SPI1_TX3 */
	{ .name = "rx3", .dma_req = 42 }, /* DMA_SPI1_RX3 */
};

=======
};

/* mcspi1 */
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if *omap2430_mcspi1_slaves[] = {
	&omap2430_l4_core__mcspi1,
};

static struct omap2_mcspi_dev_attr omap_mcspi1_dev_attr = {
	.num_chipselect = 4,
};

static struct omap_hwmod omap2430_mcspi1_hwmod = {
	.name		= "mcspi1_hwmod",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_mcspi1_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_mcspi1_mpu_irqs),
	.sdma_reqs	= omap2430_mcspi1_sdma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(omap2430_mcspi1_sdma_reqs),
=======
	.mpu_irqs	= omap2_mcspi1_mpu_irqs,
	.sdma_reqs	= omap2_mcspi1_sdma_reqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "mcspi1_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_MCSPI1_SHIFT,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_MCSPI1_SHIFT,
		},
	},
	.slaves		= omap2430_mcspi1_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_mcspi1_slaves),
<<<<<<< HEAD
	.class		= &omap2430_mcspi_class,
	.dev_attr       = &omap_mcspi1_dev_attr,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
};

/* mcspi2 */
static struct omap_hwmod_irq_info omap2430_mcspi2_mpu_irqs[] = {
	{ .irq = 66 },
};

static struct omap_hwmod_dma_info omap2430_mcspi2_sdma_reqs[] = {
	{ .name = "tx0", .dma_req = 43 }, /* DMA_SPI2_TX0 */
	{ .name = "rx0", .dma_req = 44 }, /* DMA_SPI2_RX0 */
	{ .name = "tx1", .dma_req = 45 }, /* DMA_SPI2_TX1 */
	{ .name = "rx1", .dma_req = 46 }, /* DMA_SPI2_RX1 */
};

=======
	.class		= &omap2xxx_mcspi_class,
	.dev_attr	= &omap_mcspi1_dev_attr,
};

/* mcspi2 */
>>>>>>> refs/remotes/origin/cm-10.0
static struct omap_hwmod_ocp_if *omap2430_mcspi2_slaves[] = {
	&omap2430_l4_core__mcspi2,
};

static struct omap2_mcspi_dev_attr omap_mcspi2_dev_attr = {
	.num_chipselect = 2,
};

static struct omap_hwmod omap2430_mcspi2_hwmod = {
	.name		= "mcspi2_hwmod",
<<<<<<< HEAD
	.mpu_irqs	= omap2430_mcspi2_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_mcspi2_mpu_irqs),
	.sdma_reqs	= omap2430_mcspi2_sdma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(omap2430_mcspi2_sdma_reqs),
=======
	.mpu_irqs	= omap2_mcspi2_mpu_irqs,
	.sdma_reqs	= omap2_mcspi2_sdma_reqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "mcspi2_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_MCSPI2_SHIFT,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_MCSPI2_SHIFT,
		},
	},
	.slaves		= omap2430_mcspi2_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_mcspi2_slaves),
<<<<<<< HEAD
	.class		= &omap2430_mcspi_class,
	.dev_attr       = &omap_mcspi2_dev_attr,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
	.class		= &omap2xxx_mcspi_class,
	.dev_attr	= &omap_mcspi2_dev_attr,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* mcspi3 */
static struct omap_hwmod_irq_info omap2430_mcspi3_mpu_irqs[] = {
	{ .irq = 91 },
<<<<<<< HEAD
=======
	{ .irq = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_dma_info omap2430_mcspi3_sdma_reqs[] = {
	{ .name = "tx0", .dma_req = 15 }, /* DMA_SPI3_TX0 */
	{ .name = "rx0", .dma_req = 16 }, /* DMA_SPI3_RX0 */
	{ .name = "tx1", .dma_req = 23 }, /* DMA_SPI3_TX1 */
	{ .name = "rx1", .dma_req = 24 }, /* DMA_SPI3_RX1 */
<<<<<<< HEAD
=======
	{ .dma_req = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_ocp_if *omap2430_mcspi3_slaves[] = {
	&omap2430_l4_core__mcspi3,
};

static struct omap2_mcspi_dev_attr omap_mcspi3_dev_attr = {
	.num_chipselect = 2,
};

static struct omap_hwmod omap2430_mcspi3_hwmod = {
	.name		= "mcspi3_hwmod",
	.mpu_irqs	= omap2430_mcspi3_mpu_irqs,
<<<<<<< HEAD
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_mcspi3_mpu_irqs),
	.sdma_reqs	= omap2430_mcspi3_sdma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(omap2430_mcspi3_sdma_reqs),
=======
	.sdma_reqs	= omap2430_mcspi3_sdma_reqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "mcspi3_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 2,
			.module_bit = OMAP2430_EN_MCSPI3_SHIFT,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_MCSPI3_SHIFT,
		},
	},
	.slaves		= omap2430_mcspi3_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_mcspi3_slaves),
<<<<<<< HEAD
	.class		= &omap2430_mcspi_class,
	.dev_attr       = &omap_mcspi3_dev_attr,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
	.class		= &omap2xxx_mcspi_class,
	.dev_attr	= &omap_mcspi3_dev_attr,
>>>>>>> refs/remotes/origin/cm-10.0
};

/*
 * usbhsotg
 */
static struct omap_hwmod_class_sysconfig omap2430_usbhsotg_sysc = {
	.rev_offs	= 0x0400,
	.sysc_offs	= 0x0404,
	.syss_offs	= 0x0408,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_MIDLEMODE|
			  SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
			  SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART |
			  MSTANDBY_FORCE | MSTANDBY_NO | MSTANDBY_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class usbotg_class = {
	.name = "usbotg",
	.sysc = &omap2430_usbhsotg_sysc,
};

/* usb_otg_hs */
static struct omap_hwmod_irq_info omap2430_usbhsotg_mpu_irqs[] = {

	{ .name = "mc", .irq = 92 },
	{ .name = "dma", .irq = 93 },
<<<<<<< HEAD
=======
	{ .irq = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod omap2430_usbhsotg_hwmod = {
	.name		= "usb_otg_hs",
	.mpu_irqs	= omap2430_usbhsotg_mpu_irqs,
<<<<<<< HEAD
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_usbhsotg_mpu_irqs),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "usbhs_ick",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP2430_EN_USBHS_MASK,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP2430_ST_USBHS_SHIFT,
		},
	},
	.masters	= omap2430_usbhsotg_masters,
	.masters_cnt	= ARRAY_SIZE(omap2430_usbhsotg_masters),
	.slaves		= omap2430_usbhsotg_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_usbhsotg_slaves),
	.class		= &usbotg_class,
	/*
	 * Erratum ID: i479  idle_req / idle_ack mechanism potentially
	 * broken when autoidle is enabled
	 * workaround is to disable the autoidle bit at module level.
	 */
	.flags		= HWMOD_NO_OCP_AUTOIDLE | HWMOD_SWSUP_SIDLE
				| HWMOD_SWSUP_MSTANDBY,
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

/*
 * 'mcbsp' class
 * multi channel buffered serial port controller
 */

static struct omap_hwmod_class_sysconfig omap2430_mcbsp_sysc = {
	.rev_offs	= 0x007C,
	.sysc_offs	= 0x008C,
	.sysc_flags	= (SYSC_HAS_SOFTRESET),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class omap2430_mcbsp_hwmod_class = {
	.name = "mcbsp",
	.sysc = &omap2430_mcbsp_sysc,
	.rev  = MCBSP_CONFIG_TYPE2,
};

/* mcbsp1 */
static struct omap_hwmod_irq_info omap2430_mcbsp1_irqs[] = {
	{ .name = "tx",		.irq = 59 },
	{ .name = "rx",		.irq = 60 },
	{ .name = "ovr",	.irq = 61 },
	{ .name = "common",	.irq = 64 },
<<<<<<< HEAD
};

static struct omap_hwmod_dma_info omap2430_mcbsp1_sdma_chs[] = {
	{ .name = "rx", .dma_req = 32 },
	{ .name = "tx", .dma_req = 31 },
};

static struct omap_hwmod_addr_space omap2430_mcbsp1_addrs[] = {
	{
		.name		= "mpu",
		.pa_start	= 0x48074000,
		.pa_end		= 0x480740ff,
		.flags		= ADDR_TYPE_RT
	},
=======
	{ .irq = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

/* l4_core -> mcbsp1 */
static struct omap_hwmod_ocp_if omap2430_l4_core__mcbsp1 = {
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_mcbsp1_hwmod,
	.clk		= "mcbsp1_ick",
<<<<<<< HEAD
	.addr		= omap2430_mcbsp1_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_mcbsp1_addrs),
=======
	.addr		= omap2_mcbsp1_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* mcbsp1 slave ports */
static struct omap_hwmod_ocp_if *omap2430_mcbsp1_slaves[] = {
	&omap2430_l4_core__mcbsp1,
};

static struct omap_hwmod omap2430_mcbsp1_hwmod = {
	.name		= "mcbsp1",
	.class		= &omap2430_mcbsp_hwmod_class,
	.mpu_irqs	= omap2430_mcbsp1_irqs,
<<<<<<< HEAD
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_mcbsp1_irqs),
	.sdma_reqs	= omap2430_mcbsp1_sdma_chs,
	.sdma_reqs_cnt	= ARRAY_SIZE(omap2430_mcbsp1_sdma_chs),
=======
	.sdma_reqs	= omap2_mcbsp1_sdma_reqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "mcbsp1_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_MCBSP1_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_MCBSP1_SHIFT,
		},
	},
	.slaves		= omap2430_mcbsp1_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_mcbsp1_slaves),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

/* mcbsp2 */
static struct omap_hwmod_irq_info omap2430_mcbsp2_irqs[] = {
	{ .name = "tx",		.irq = 62 },
	{ .name = "rx",		.irq = 63 },
	{ .name = "common",	.irq = 16 },
<<<<<<< HEAD
};

static struct omap_hwmod_dma_info omap2430_mcbsp2_sdma_chs[] = {
	{ .name = "rx", .dma_req = 34 },
	{ .name = "tx", .dma_req = 33 },
};

static struct omap_hwmod_addr_space omap2430_mcbsp2_addrs[] = {
	{
		.name		= "mpu",
		.pa_start	= 0x48076000,
		.pa_end		= 0x480760ff,
		.flags		= ADDR_TYPE_RT
	},
=======
	{ .irq = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
=======
	{ }
};

static struct omap_hwmod_ocp_if omap2430_l4_wkup__wd_timer2 = {
	.master		= &omap2xxx_l4_wkup_hwmod,
	.slave		= &omap2xxx_wd_timer2_hwmod,
	.clk		= "mpu_wdt_ick",
	.addr		= omap2430_wd_timer2_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4_wkup -> gpio1 */
static struct omap_hwmod_addr_space omap2430_gpio1_addr_space[] = {
	{
		.pa_start	= 0x4900C000,
		.pa_end		= 0x4900C1ff,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if omap2430_l4_wkup__gpio1 = {
	.master		= &omap2xxx_l4_wkup_hwmod,
	.slave		= &omap2xxx_gpio1_hwmod,
	.clk		= "gpios_ick",
	.addr		= omap2430_gpio1_addr_space,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4_wkup -> gpio2 */
static struct omap_hwmod_addr_space omap2430_gpio2_addr_space[] = {
	{
		.pa_start	= 0x4900E000,
		.pa_end		= 0x4900E1ff,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if omap2430_l4_wkup__gpio2 = {
	.master		= &omap2xxx_l4_wkup_hwmod,
	.slave		= &omap2xxx_gpio2_hwmod,
	.clk		= "gpios_ick",
	.addr		= omap2430_gpio2_addr_space,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4_wkup -> gpio3 */
static struct omap_hwmod_addr_space omap2430_gpio3_addr_space[] = {
	{
		.pa_start	= 0x49010000,
		.pa_end		= 0x490101ff,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if omap2430_l4_wkup__gpio3 = {
	.master		= &omap2xxx_l4_wkup_hwmod,
	.slave		= &omap2xxx_gpio3_hwmod,
	.clk		= "gpios_ick",
	.addr		= omap2430_gpio3_addr_space,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4_wkup -> gpio4 */
static struct omap_hwmod_addr_space omap2430_gpio4_addr_space[] = {
	{
		.pa_start	= 0x49012000,
		.pa_end		= 0x490121ff,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if omap2430_l4_wkup__gpio4 = {
	.master		= &omap2xxx_l4_wkup_hwmod,
	.slave		= &omap2xxx_gpio4_hwmod,
	.clk		= "gpios_ick",
	.addr		= omap2430_gpio4_addr_space,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4_core -> gpio5 */
static struct omap_hwmod_addr_space omap2430_gpio5_addr_space[] = {
	{
		.pa_start	= 0x480B6000,
		.pa_end		= 0x480B61ff,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if omap2430_l4_core__gpio5 = {
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_gpio5_hwmod,
	.clk		= "gpio5_ick",
	.addr		= omap2430_gpio5_addr_space,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* dma_system -> L3 */
static struct omap_hwmod_ocp_if omap2430_dma_system__l3 = {
	.master		= &omap2430_dma_system_hwmod,
	.slave		= &omap2xxx_l3_main_hwmod,
	.clk		= "core_l3_ck",
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4_core -> dma_system */
static struct omap_hwmod_ocp_if omap2430_l4_core__dma_system = {
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_dma_system_hwmod,
	.clk		= "sdma_ick",
	.addr		= omap2_dma_system_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4_core -> mailbox */
static struct omap_hwmod_ocp_if omap2430_l4_core__mailbox = {
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_mailbox_hwmod,
	.addr		= omap2_mailbox_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4_core -> mcbsp1 */
static struct omap_hwmod_ocp_if omap2430_l4_core__mcbsp1 = {
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_mcbsp1_hwmod,
	.clk		= "mcbsp1_ick",
	.addr		= omap2_mcbsp1_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
>>>>>>> refs/remotes/origin/master
};

/* l4_core -> mcbsp2 */
static struct omap_hwmod_ocp_if omap2430_l4_core__mcbsp2 = {
<<<<<<< HEAD
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_mcbsp2_hwmod,
	.clk		= "mcbsp2_ick",
<<<<<<< HEAD
	.addr		= omap2430_mcbsp2_addrs,
	.addr_cnt	= ARRAY_SIZE(omap2430_mcbsp2_addrs),
=======
	.addr		= omap2xxx_mcbsp2_addrs,
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* mcbsp2 slave ports */
static struct omap_hwmod_ocp_if *omap2430_mcbsp2_slaves[] = {
	&omap2430_l4_core__mcbsp2,
};

static struct omap_hwmod omap2430_mcbsp2_hwmod = {
	.name		= "mcbsp2",
	.class		= &omap2430_mcbsp_hwmod_class,
	.mpu_irqs	= omap2430_mcbsp2_irqs,
<<<<<<< HEAD
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_mcbsp2_irqs),
	.sdma_reqs	= omap2430_mcbsp2_sdma_chs,
	.sdma_reqs_cnt	= ARRAY_SIZE(omap2430_mcbsp2_sdma_chs),
=======
	.sdma_reqs	= omap2_mcbsp2_sdma_reqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "mcbsp2_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP24XX_EN_MCBSP2_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP24XX_ST_MCBSP2_SHIFT,
		},
	},
	.slaves		= omap2430_mcbsp2_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_mcbsp2_slaves),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

/* mcbsp3 */
static struct omap_hwmod_irq_info omap2430_mcbsp3_irqs[] = {
	{ .name = "tx",		.irq = 89 },
	{ .name = "rx",		.irq = 90 },
	{ .name = "common",	.irq = 17 },
<<<<<<< HEAD
};

static struct omap_hwmod_dma_info omap2430_mcbsp3_sdma_chs[] = {
	{ .name = "rx", .dma_req = 18 },
	{ .name = "tx", .dma_req = 17 },
=======
	{ .irq = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

=======
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_mcbsp2_hwmod,
	.clk		= "mcbsp2_ick",
	.addr		= omap2xxx_mcbsp2_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

>>>>>>> refs/remotes/origin/master
static struct omap_hwmod_addr_space omap2430_mcbsp3_addrs[] = {
	{
		.name		= "mpu",
		.pa_start	= 0x4808C000,
		.pa_end		= 0x4808C0ff,
		.flags		= ADDR_TYPE_RT
	},
<<<<<<< HEAD
<<<<<<< HEAD
=======
	{ }
>>>>>>> refs/remotes/origin/cm-10.0
=======
	{ }
>>>>>>> refs/remotes/origin/master
};

/* l4_core -> mcbsp3 */
static struct omap_hwmod_ocp_if omap2430_l4_core__mcbsp3 = {
<<<<<<< HEAD
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_mcbsp3_hwmod,
	.clk		= "mcbsp3_ick",
	.addr		= omap2430_mcbsp3_addrs,
<<<<<<< HEAD
	.addr_cnt	= ARRAY_SIZE(omap2430_mcbsp3_addrs),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* mcbsp3 slave ports */
static struct omap_hwmod_ocp_if *omap2430_mcbsp3_slaves[] = {
	&omap2430_l4_core__mcbsp3,
};

static struct omap_hwmod omap2430_mcbsp3_hwmod = {
	.name		= "mcbsp3",
	.class		= &omap2430_mcbsp_hwmod_class,
	.mpu_irqs	= omap2430_mcbsp3_irqs,
<<<<<<< HEAD
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_mcbsp3_irqs),
	.sdma_reqs	= omap2430_mcbsp3_sdma_chs,
	.sdma_reqs_cnt	= ARRAY_SIZE(omap2430_mcbsp3_sdma_chs),
=======
	.sdma_reqs	= omap2_mcbsp3_sdma_reqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "mcbsp3_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP2430_EN_MCBSP3_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_MCBSP3_SHIFT,
		},
	},
	.slaves		= omap2430_mcbsp3_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_mcbsp3_slaves),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

/* mcbsp4 */
static struct omap_hwmod_irq_info omap2430_mcbsp4_irqs[] = {
	{ .name = "tx",		.irq = 54 },
	{ .name = "rx",		.irq = 55 },
	{ .name = "common",	.irq = 18 },
<<<<<<< HEAD
=======
	{ .irq = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_dma_info omap2430_mcbsp4_sdma_chs[] = {
	{ .name = "rx", .dma_req = 20 },
	{ .name = "tx", .dma_req = 19 },
<<<<<<< HEAD
=======
	{ .dma_req = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

=======
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_mcbsp3_hwmod,
	.clk		= "mcbsp3_ick",
	.addr		= omap2430_mcbsp3_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

>>>>>>> refs/remotes/origin/master
static struct omap_hwmod_addr_space omap2430_mcbsp4_addrs[] = {
	{
		.name		= "mpu",
		.pa_start	= 0x4808E000,
		.pa_end		= 0x4808E0ff,
		.flags		= ADDR_TYPE_RT
	},
<<<<<<< HEAD
<<<<<<< HEAD
=======
	{ }
>>>>>>> refs/remotes/origin/cm-10.0
=======
	{ }
>>>>>>> refs/remotes/origin/master
};

/* l4_core -> mcbsp4 */
static struct omap_hwmod_ocp_if omap2430_l4_core__mcbsp4 = {
<<<<<<< HEAD
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_mcbsp4_hwmod,
	.clk		= "mcbsp4_ick",
	.addr		= omap2430_mcbsp4_addrs,
<<<<<<< HEAD
	.addr_cnt	= ARRAY_SIZE(omap2430_mcbsp4_addrs),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* mcbsp4 slave ports */
static struct omap_hwmod_ocp_if *omap2430_mcbsp4_slaves[] = {
	&omap2430_l4_core__mcbsp4,
};

static struct omap_hwmod omap2430_mcbsp4_hwmod = {
	.name		= "mcbsp4",
	.class		= &omap2430_mcbsp_hwmod_class,
	.mpu_irqs	= omap2430_mcbsp4_irqs,
<<<<<<< HEAD
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_mcbsp4_irqs),
	.sdma_reqs	= omap2430_mcbsp4_sdma_chs,
	.sdma_reqs_cnt	= ARRAY_SIZE(omap2430_mcbsp4_sdma_chs),
=======
	.sdma_reqs	= omap2430_mcbsp4_sdma_chs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "mcbsp4_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP2430_EN_MCBSP4_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_MCBSP4_SHIFT,
		},
	},
	.slaves		= omap2430_mcbsp4_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_mcbsp4_slaves),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

/* mcbsp5 */
static struct omap_hwmod_irq_info omap2430_mcbsp5_irqs[] = {
	{ .name = "tx",		.irq = 81 },
	{ .name = "rx",		.irq = 82 },
	{ .name = "common",	.irq = 19 },
<<<<<<< HEAD
=======
	{ .irq = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_dma_info omap2430_mcbsp5_sdma_chs[] = {
	{ .name = "rx", .dma_req = 22 },
	{ .name = "tx", .dma_req = 21 },
<<<<<<< HEAD
=======
	{ .dma_req = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

=======
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_mcbsp4_hwmod,
	.clk		= "mcbsp4_ick",
	.addr		= omap2430_mcbsp4_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

>>>>>>> refs/remotes/origin/master
static struct omap_hwmod_addr_space omap2430_mcbsp5_addrs[] = {
	{
		.name		= "mpu",
		.pa_start	= 0x48096000,
		.pa_end		= 0x480960ff,
		.flags		= ADDR_TYPE_RT
	},
<<<<<<< HEAD
<<<<<<< HEAD
=======
	{ }
>>>>>>> refs/remotes/origin/cm-10.0
=======
	{ }
>>>>>>> refs/remotes/origin/master
};

/* l4_core -> mcbsp5 */
static struct omap_hwmod_ocp_if omap2430_l4_core__mcbsp5 = {
<<<<<<< HEAD
	.master		= &omap2430_l4_core_hwmod,
	.slave		= &omap2430_mcbsp5_hwmod,
	.clk		= "mcbsp5_ick",
	.addr		= omap2430_mcbsp5_addrs,
<<<<<<< HEAD
	.addr_cnt	= ARRAY_SIZE(omap2430_mcbsp5_addrs),
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* mcbsp5 slave ports */
static struct omap_hwmod_ocp_if *omap2430_mcbsp5_slaves[] = {
	&omap2430_l4_core__mcbsp5,
};

static struct omap_hwmod omap2430_mcbsp5_hwmod = {
	.name		= "mcbsp5",
	.class		= &omap2430_mcbsp_hwmod_class,
	.mpu_irqs	= omap2430_mcbsp5_irqs,
<<<<<<< HEAD
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_mcbsp5_irqs),
	.sdma_reqs	= omap2430_mcbsp5_sdma_chs,
	.sdma_reqs_cnt	= ARRAY_SIZE(omap2430_mcbsp5_sdma_chs),
=======
	.sdma_reqs	= omap2430_mcbsp5_sdma_chs,
>>>>>>> refs/remotes/origin/cm-10.0
	.main_clk	= "mcbsp5_fck",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP2430_EN_MCBSP5_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_MCBSP5_SHIFT,
		},
	},
	.slaves		= omap2430_mcbsp5_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_mcbsp5_slaves),
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

/* MMC/SD/SDIO common */

static struct omap_hwmod_class_sysconfig omap2430_mmc_sysc = {
	.rev_offs	= 0x1fc,
	.sysc_offs	= 0x10,
	.syss_offs	= 0x14,
	.sysc_flags	= (SYSC_HAS_CLOCKACTIVITY | SYSC_HAS_SIDLEMODE |
			   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
			   SYSC_HAS_AUTOIDLE | SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class omap2430_mmc_class = {
	.name = "mmc",
	.sysc = &omap2430_mmc_sysc,
};

/* MMC/SD/SDIO1 */

static struct omap_hwmod_irq_info omap2430_mmc1_mpu_irqs[] = {
	{ .irq = 83 },
<<<<<<< HEAD
=======
	{ .irq = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_dma_info omap2430_mmc1_sdma_reqs[] = {
	{ .name = "tx",	.dma_req = 61 }, /* DMA_MMC1_TX */
	{ .name = "rx",	.dma_req = 62 }, /* DMA_MMC1_RX */
<<<<<<< HEAD
=======
	{ .dma_req = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_opt_clk omap2430_mmc1_opt_clks[] = {
	{ .role = "dbck", .clk = "mmchsdb1_fck" },
};

static struct omap_hwmod_ocp_if *omap2430_mmc1_slaves[] = {
	&omap2430_l4_core__mmc1,
};

static struct omap_mmc_dev_attr mmc1_dev_attr = {
	.flags = OMAP_HSMMC_SUPPORTS_DUAL_VOLT,
};

static struct omap_hwmod omap2430_mmc1_hwmod = {
	.name		= "mmc1",
	.flags		= HWMOD_CONTROL_OPT_CLKS_IN_RESET,
	.mpu_irqs	= omap2430_mmc1_mpu_irqs,
<<<<<<< HEAD
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_mmc1_mpu_irqs),
	.sdma_reqs	= omap2430_mmc1_sdma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(omap2430_mmc1_sdma_reqs),
=======
	.sdma_reqs	= omap2430_mmc1_sdma_reqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.opt_clks	= omap2430_mmc1_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(omap2430_mmc1_opt_clks),
	.main_clk	= "mmchs1_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 2,
			.module_bit  = OMAP2430_EN_MMCHS1_SHIFT,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_MMCHS1_SHIFT,
		},
	},
	.dev_attr	= &mmc1_dev_attr,
	.slaves		= omap2430_mmc1_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_mmc1_slaves),
	.class		= &omap2430_mmc_class,
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

/* MMC/SD/SDIO2 */

static struct omap_hwmod_irq_info omap2430_mmc2_mpu_irqs[] = {
	{ .irq = 86 },
<<<<<<< HEAD
=======
	{ .irq = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_dma_info omap2430_mmc2_sdma_reqs[] = {
	{ .name = "tx",	.dma_req = 47 }, /* DMA_MMC2_TX */
	{ .name = "rx",	.dma_req = 48 }, /* DMA_MMC2_RX */
<<<<<<< HEAD
=======
	{ .dma_req = -1 }
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct omap_hwmod_opt_clk omap2430_mmc2_opt_clks[] = {
	{ .role = "dbck", .clk = "mmchsdb2_fck" },
};

static struct omap_hwmod_ocp_if *omap2430_mmc2_slaves[] = {
	&omap2430_l4_core__mmc2,
};

static struct omap_hwmod omap2430_mmc2_hwmod = {
	.name		= "mmc2",
	.flags		= HWMOD_CONTROL_OPT_CLKS_IN_RESET,
	.mpu_irqs	= omap2430_mmc2_mpu_irqs,
<<<<<<< HEAD
	.mpu_irqs_cnt	= ARRAY_SIZE(omap2430_mmc2_mpu_irqs),
	.sdma_reqs	= omap2430_mmc2_sdma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(omap2430_mmc2_sdma_reqs),
=======
	.sdma_reqs	= omap2430_mmc2_sdma_reqs,
>>>>>>> refs/remotes/origin/cm-10.0
	.opt_clks	= omap2430_mmc2_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(omap2430_mmc2_opt_clks),
	.main_clk	= "mmchs2_fck",
	.prcm		= {
		.omap2 = {
			.module_offs = CORE_MOD,
			.prcm_reg_id = 2,
			.module_bit  = OMAP2430_EN_MMCHS2_SHIFT,
			.idlest_reg_id = 2,
			.idlest_idle_bit = OMAP2430_ST_MMCHS2_SHIFT,
		},
	},
	.slaves		= omap2430_mmc2_slaves,
	.slaves_cnt	= ARRAY_SIZE(omap2430_mmc2_slaves),
	.class		= &omap2430_mmc_class,
<<<<<<< HEAD
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

static __initdata struct omap_hwmod *omap2430_hwmods[] = {
	&omap2430_l3_main_hwmod,
	&omap2430_l4_core_hwmod,
	&omap2430_l4_wkup_hwmod,
	&omap2430_mpu_hwmod,
	&omap2430_iva_hwmod,

	&omap2430_timer1_hwmod,
	&omap2430_timer2_hwmod,
	&omap2430_timer3_hwmod,
	&omap2430_timer4_hwmod,
	&omap2430_timer5_hwmod,
	&omap2430_timer6_hwmod,
	&omap2430_timer7_hwmod,
	&omap2430_timer8_hwmod,
	&omap2430_timer9_hwmod,
	&omap2430_timer10_hwmod,
	&omap2430_timer11_hwmod,
	&omap2430_timer12_hwmod,

	&omap2430_wd_timer2_hwmod,
	&omap2430_uart1_hwmod,
	&omap2430_uart2_hwmod,
	&omap2430_uart3_hwmod,
	/* dss class */
	&omap2430_dss_core_hwmod,
	&omap2430_dss_dispc_hwmod,
	&omap2430_dss_rfbi_hwmod,
	&omap2430_dss_venc_hwmod,
	/* i2c class */
	&omap2430_i2c1_hwmod,
	&omap2430_i2c2_hwmod,
	&omap2430_mmc1_hwmod,
	&omap2430_mmc2_hwmod,

	/* gpio class */
	&omap2430_gpio1_hwmod,
	&omap2430_gpio2_hwmod,
	&omap2430_gpio3_hwmod,
	&omap2430_gpio4_hwmod,
	&omap2430_gpio5_hwmod,

	/* dma_system class*/
	&omap2430_dma_system_hwmod,

	/* mcbsp class */
	&omap2430_mcbsp1_hwmod,
	&omap2430_mcbsp2_hwmod,
	&omap2430_mcbsp3_hwmod,
	&omap2430_mcbsp4_hwmod,
	&omap2430_mcbsp5_hwmod,

	/* mailbox class */
	&omap2430_mailbox_hwmod,

	/* mcspi class */
	&omap2430_mcspi1_hwmod,
	&omap2430_mcspi2_hwmod,
	&omap2430_mcspi3_hwmod,

	/* usbotg class*/
	&omap2430_usbhsotg_hwmod,

=======
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_mcbsp5_hwmod,
	.clk		= "mcbsp5_ick",
	.addr		= omap2430_mcbsp5_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* l4_core -> hdq1w */
static struct omap_hwmod_ocp_if omap2430_l4_core__hdq1w = {
	.master		= &omap2xxx_l4_core_hwmod,
	.slave		= &omap2430_hdq1w_hwmod,
	.clk		= "hdq_ick",
	.addr		= omap2_hdq1w_addr_space,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
	.flags		= OMAP_FIREWALL_L4 | OCPIF_SWSUP_IDLE,
};

/* l4_wkup -> 32ksync_counter */
static struct omap_hwmod_addr_space omap2430_counter_32k_addrs[] = {
	{
		.pa_start	= 0x49020000,
		.pa_end		= 0x4902001f,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_addr_space omap2430_gpmc_addrs[] = {
	{
		.pa_start	= 0x6e000000,
		.pa_end		= 0x6e000fff,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if omap2430_l4_wkup__counter_32k = {
	.master		= &omap2xxx_l4_wkup_hwmod,
	.slave		= &omap2xxx_counter_32k_hwmod,
	.clk		= "sync_32k_ick",
	.addr		= omap2430_counter_32k_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

static struct omap_hwmod_ocp_if omap2430_l3__gpmc = {
	.master		= &omap2xxx_l3_main_hwmod,
	.slave		= &omap2xxx_gpmc_hwmod,
	.clk		= "core_l3_ck",
	.addr		= omap2430_gpmc_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

static struct omap_hwmod_ocp_if *omap2430_hwmod_ocp_ifs[] __initdata = {
	&omap2xxx_l3_main__l4_core,
	&omap2xxx_mpu__l3_main,
	&omap2xxx_dss__l3,
	&omap2430_usbhsotg__l3,
	&omap2430_l4_core__i2c1,
	&omap2430_l4_core__i2c2,
	&omap2xxx_l4_core__l4_wkup,
	&omap2_l4_core__uart1,
	&omap2_l4_core__uart2,
	&omap2_l4_core__uart3,
	&omap2430_l4_core__usbhsotg,
	&omap2430_l4_core__mmc1,
	&omap2430_l4_core__mmc2,
	&omap2xxx_l4_core__mcspi1,
	&omap2xxx_l4_core__mcspi2,
	&omap2430_l4_core__mcspi3,
	&omap2430_l3__iva,
	&omap2430_l4_wkup__timer1,
	&omap2xxx_l4_core__timer2,
	&omap2xxx_l4_core__timer3,
	&omap2xxx_l4_core__timer4,
	&omap2xxx_l4_core__timer5,
	&omap2xxx_l4_core__timer6,
	&omap2xxx_l4_core__timer7,
	&omap2xxx_l4_core__timer8,
	&omap2xxx_l4_core__timer9,
	&omap2xxx_l4_core__timer10,
	&omap2xxx_l4_core__timer11,
	&omap2xxx_l4_core__timer12,
	&omap2430_l4_wkup__wd_timer2,
	&omap2xxx_l4_core__dss,
	&omap2xxx_l4_core__dss_dispc,
	&omap2xxx_l4_core__dss_rfbi,
	&omap2xxx_l4_core__dss_venc,
	&omap2430_l4_wkup__gpio1,
	&omap2430_l4_wkup__gpio2,
	&omap2430_l4_wkup__gpio3,
	&omap2430_l4_wkup__gpio4,
	&omap2430_l4_core__gpio5,
	&omap2430_dma_system__l3,
	&omap2430_l4_core__dma_system,
	&omap2430_l4_core__mailbox,
	&omap2430_l4_core__mcbsp1,
	&omap2430_l4_core__mcbsp2,
	&omap2430_l4_core__mcbsp3,
	&omap2430_l4_core__mcbsp4,
	&omap2430_l4_core__mcbsp5,
	&omap2430_l4_core__hdq1w,
	&omap2xxx_l4_core__rng,
	&omap2xxx_l4_core__sham,
	&omap2xxx_l4_core__aes,
	&omap2430_l4_wkup__counter_32k,
	&omap2430_l3__gpmc,
>>>>>>> refs/remotes/origin/master
	NULL,
};

int __init omap2430_hwmod_init(void)
{
<<<<<<< HEAD
	return omap_hwmod_register(omap2430_hwmods);
=======
	omap_hwmod_init();
	return omap_hwmod_register_links(omap2430_hwmod_ocp_ifs);
>>>>>>> refs/remotes/origin/master
}
