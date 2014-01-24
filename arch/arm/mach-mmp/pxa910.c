/*
 *  linux/arch/arm/mach-mmp/pxa910.c
 *
 *  Code specific to PXA910
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/io.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/platform_device.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <asm/mach/time.h>
#include <mach/addr-map.h>
#include <mach/regs-apbc.h>
#include <mach/regs-apmu.h>
#include <mach/cputype.h>
#include <mach/irqs.h>
<<<<<<< HEAD
#include <mach/gpio.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <mach/dma.h>
#include <mach/mfp.h>
#include <mach/devices.h>

#include "common.h"
#include "clock.h"
=======
#include <linux/irq.h>
#include <linux/irqchip/mmp.h>
#include <linux/platform_device.h>

#include <asm/hardware/cache-tauros2.h>
#include <asm/mach/time.h>
#include <mach/addr-map.h>
#include <mach/regs-apbc.h>
#include <mach/cputype.h>
#include <mach/irqs.h>
#include <mach/dma.h>
#include <mach/mfp.h>
#include <mach/devices.h>
#include <mach/pm-pxa910.h>
#include <mach/pxa910.h>

#include "common.h"
>>>>>>> refs/remotes/origin/master

#define MFPR_VIRT_BASE	(APB_VIRT_BASE + 0x1e000)

static struct mfp_addr_map pxa910_mfp_addr_map[] __initdata =
{
	MFP_ADDR_X(GPIO0, GPIO54, 0xdc),
	MFP_ADDR_X(GPIO67, GPIO98, 0x1b8),
	MFP_ADDR_X(GPIO100, GPIO109, 0x238),

	MFP_ADDR(GPIO123, 0xcc),
	MFP_ADDR(GPIO124, 0xd0),

	MFP_ADDR(DF_IO0, 0x40),
	MFP_ADDR(DF_IO1, 0x3c),
	MFP_ADDR(DF_IO2, 0x38),
	MFP_ADDR(DF_IO3, 0x34),
	MFP_ADDR(DF_IO4, 0x30),
	MFP_ADDR(DF_IO5, 0x2c),
	MFP_ADDR(DF_IO6, 0x28),
	MFP_ADDR(DF_IO7, 0x24),
	MFP_ADDR(DF_IO8, 0x20),
	MFP_ADDR(DF_IO9, 0x1c),
	MFP_ADDR(DF_IO10, 0x18),
	MFP_ADDR(DF_IO11, 0x14),
	MFP_ADDR(DF_IO12, 0x10),
	MFP_ADDR(DF_IO13, 0xc),
	MFP_ADDR(DF_IO14, 0x8),
	MFP_ADDR(DF_IO15, 0x4),

	MFP_ADDR(DF_nCS0_SM_nCS2, 0x44),
	MFP_ADDR(DF_nCS1_SM_nCS3, 0x48),
	MFP_ADDR(SM_nCS0, 0x4c),
	MFP_ADDR(SM_nCS1, 0x50),
	MFP_ADDR(DF_WEn, 0x54),
	MFP_ADDR(DF_REn, 0x58),
	MFP_ADDR(DF_CLE_SM_OEn, 0x5c),
	MFP_ADDR(DF_ALE_SM_WEn, 0x60),
	MFP_ADDR(SM_SCLK, 0x64),
	MFP_ADDR(DF_RDY0, 0x68),
	MFP_ADDR(SM_BE0, 0x6c),
	MFP_ADDR(SM_BE1, 0x70),
	MFP_ADDR(SM_ADV, 0x74),
	MFP_ADDR(DF_RDY1, 0x78),
	MFP_ADDR(SM_ADVMUX, 0x7c),
	MFP_ADDR(SM_RDY, 0x80),

	MFP_ADDR_X(MMC1_DAT7, MMC1_WP, 0x84),

	MFP_ADDR_END,
};

<<<<<<< HEAD
<<<<<<< HEAD
#define APMASK(i)	(GPIO_REGS_VIRT + BANK_OFF(i) + 0x09c)

static void __init pxa910_init_gpio(void)
{
	int i;

	/* enable GPIO clock */
	__raw_writel(APBC_APBCLK | APBC_FNCLK, APBC_PXA910_GPIO);

	/* unmask GPIO edge detection for all 4 banks - APMASKx */
	for (i = 0; i < 4; i++)
		__raw_writel(0xffffffff, APMASK(i));

	pxa_init_gpio(IRQ_PXA910_AP_GPIO, 0, 127, NULL);
}

void __init pxa910_init_irq(void)
{
	icu_init_irq();
	pxa910_init_gpio();
=======
void __init pxa910_init_irq(void)
{
	icu_init_irq();
>>>>>>> refs/remotes/origin/cm-10.0
}

/* APB peripheral clocks */
static APBC_CLK(uart1, PXA910_UART0, 1, 14745600);
static APBC_CLK(uart2, PXA910_UART1, 1, 14745600);
static APBC_CLK(twsi0, PXA168_TWSI0, 1, 33000000);
static APBC_CLK(twsi1, PXA168_TWSI1, 1, 33000000);
static APBC_CLK(pwm1, PXA910_PWM1, 1, 13000000);
static APBC_CLK(pwm2, PXA910_PWM2, 1, 13000000);
static APBC_CLK(pwm3, PXA910_PWM3, 1, 13000000);
static APBC_CLK(pwm4, PXA910_PWM4, 1, 13000000);
<<<<<<< HEAD
=======
static APBC_CLK(gpio, PXA910_GPIO, 0, 13000000);
static APBC_CLK(rtc, PXA910_RTC, 8, 32768);
>>>>>>> refs/remotes/origin/cm-10.0

static APMU_CLK(nand, NAND, 0x19b, 156000000);
static APMU_CLK(u2o, USB, 0x1b, 480000000);

/* device and clock bindings */
static struct clk_lookup pxa910_clkregs[] = {
	INIT_CLKREG(&clk_uart1, "pxa2xx-uart.0", NULL),
	INIT_CLKREG(&clk_uart2, "pxa2xx-uart.1", NULL),
	INIT_CLKREG(&clk_twsi0, "pxa2xx-i2c.0", NULL),
	INIT_CLKREG(&clk_twsi1, "pxa2xx-i2c.1", NULL),
	INIT_CLKREG(&clk_pwm1, "pxa910-pwm.0", NULL),
	INIT_CLKREG(&clk_pwm2, "pxa910-pwm.1", NULL),
	INIT_CLKREG(&clk_pwm3, "pxa910-pwm.2", NULL),
	INIT_CLKREG(&clk_pwm4, "pxa910-pwm.3", NULL),
	INIT_CLKREG(&clk_nand, "pxa3xx-nand", NULL),
<<<<<<< HEAD
	INIT_CLKREG(&clk_u2o, "pxa-u2o", "U2OCLK"),
=======
	INIT_CLKREG(&clk_gpio, "pxa-gpio", NULL),
	INIT_CLKREG(&clk_u2o, "pxa-u2o", "U2OCLK"),
	INIT_CLKREG(&clk_rtc, "sa1100-rtc", NULL),
>>>>>>> refs/remotes/origin/cm-10.0
};

static int __init pxa910_init(void)
{
	if (cpu_is_pxa910()) {
		mfp_init_base(MFPR_VIRT_BASE);
		mfp_init_addr(pxa910_mfp_addr_map);
		pxa_init_dma(IRQ_PXA910_DMA_INT0, 32);
		clkdev_add_table(ARRAY_AND_SIZE(pxa910_clkregs));
=======
void __init pxa910_init_irq(void)
{
	icu_init_irq();
#ifdef CONFIG_PM
	icu_irq_chip.irq_set_wake = pxa910_set_wake;
#endif
}

static int __init pxa910_init(void)
{
	if (cpu_is_pxa910()) {
#ifdef CONFIG_CACHE_TAUROS2
		tauros2_init(0);
#endif
		mfp_init_base(MFPR_VIRT_BASE);
		mfp_init_addr(pxa910_mfp_addr_map);
		pxa_init_dma(IRQ_PXA910_DMA_INT0, 32);
		pxa910_clk_init();
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}
postcore_initcall(pxa910_init);

/* system timer - clock enabled, 3.25MHz */
#define TIMER_CLK_RST	(APBC_APBCLK | APBC_FNCLK | APBC_FNCLKSEL(3))
<<<<<<< HEAD

static void __init pxa910_timer_init(void)
{
	/* reset and configure */
	__raw_writel(APBC_APBCLK | APBC_RST, APBC_PXA910_TIMERS);
	__raw_writel(TIMER_CLK_RST, APBC_PXA910_TIMERS);
=======
#define APBC_TIMERS	APBC_REG(0x34)

void __init pxa910_timer_init(void)
{
	/* reset and configure */
	__raw_writel(APBC_APBCLK | APBC_RST, APBC_TIMERS);
	__raw_writel(TIMER_CLK_RST, APBC_TIMERS);
>>>>>>> refs/remotes/origin/master

	timer_init(IRQ_PXA910_AP1_TIMER1);
}

<<<<<<< HEAD
struct sys_timer pxa910_timer = {
	.init	= pxa910_timer_init,
};

=======
>>>>>>> refs/remotes/origin/master
/* on-chip devices */

/* NOTE: there are totally 3 UARTs on PXA910:
 *
 *   UART1   - Slow UART (can be used both by AP and CP)
 *   UART2/3 - Fast UART
 *
 * To be backward compatible with the legacy FFUART/BTUART/STUART sequence,
 * they are re-ordered as:
 *
 *   pxa910_device_uart1 - UART2 as FFUART
 *   pxa910_device_uart2 - UART3 as BTUART
 *
 * UART1 is not used by AP for the moment.
 */
PXA910_DEVICE(uart1, "pxa2xx-uart", 0, UART2, 0xd4017000, 0x30, 21, 22);
PXA910_DEVICE(uart2, "pxa2xx-uart", 1, UART3, 0xd4018000, 0x30, 23, 24);
PXA910_DEVICE(twsi0, "pxa2xx-i2c", 0, TWSI0, 0xd4011000, 0x28);
PXA910_DEVICE(twsi1, "pxa2xx-i2c", 1, TWSI1, 0xd4025000, 0x28);
PXA910_DEVICE(pwm1, "pxa910-pwm", 0, NONE, 0xd401a000, 0x10);
PXA910_DEVICE(pwm2, "pxa910-pwm", 1, NONE, 0xd401a400, 0x10);
PXA910_DEVICE(pwm3, "pxa910-pwm", 2, NONE, 0xd401a800, 0x10);
PXA910_DEVICE(pwm4, "pxa910-pwm", 3, NONE, 0xd401ac00, 0x10);
PXA910_DEVICE(nand, "pxa3xx-nand", -1, NAND, 0xd4283000, 0x80, 97, 99);
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
PXA910_DEVICE(disp, "mmp-disp", 0, LCD, 0xd420b000, 0x1ec);
PXA910_DEVICE(fb, "mmp-fb", -1, NONE, 0, 0);
PXA910_DEVICE(panel, "tpo-hvga", -1, NONE, 0, 0);
>>>>>>> refs/remotes/origin/master

struct resource pxa910_resource_gpio[] = {
	{
		.start	= 0xd4019000,
		.end	= 0xd4019fff,
		.flags	= IORESOURCE_MEM,
	}, {
		.start	= IRQ_PXA910_AP_GPIO,
		.end	= IRQ_PXA910_AP_GPIO,
		.name	= "gpio_mux",
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device pxa910_device_gpio = {
<<<<<<< HEAD
	.name		= "pxa-gpio",
=======
	.name		= "mmp-gpio",
>>>>>>> refs/remotes/origin/master
	.id		= -1,
	.num_resources	= ARRAY_SIZE(pxa910_resource_gpio),
	.resource	= pxa910_resource_gpio,
};

static struct resource pxa910_resource_rtc[] = {
	{
		.start	= 0xd4010000,
		.end	= 0xd401003f,
		.flags	= IORESOURCE_MEM,
	}, {
		.start	= IRQ_PXA910_RTC_INT,
		.end	= IRQ_PXA910_RTC_INT,
		.name	= "rtc 1Hz",
		.flags	= IORESOURCE_IRQ,
	}, {
		.start	= IRQ_PXA910_RTC_ALARM,
		.end	= IRQ_PXA910_RTC_ALARM,
		.name	= "rtc alarm",
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device pxa910_device_rtc = {
	.name		= "sa1100-rtc",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(pxa910_resource_rtc),
	.resource	= pxa910_resource_rtc,
};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
