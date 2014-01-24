/*
 *  linux/arch/arm/mach-integrator/core.c
 *
 *  Copyright (C) 2000-2003 Deep Blue Solutions Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation.
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/memblock.h>
#include <linux/sched.h>
#include <linux/smp.h>
<<<<<<< HEAD
#include <linux/termios.h>
#include <linux/amba/bus.h>
#include <linux/amba/serial.h>
#include <linux/io.h>
#include <linux/clkdev.h>

#include <mach/hardware.h>
#include <mach/platform.h>
<<<<<<< HEAD
#include <asm/irq.h>
#include <mach/cm.h>
#include <asm/system.h>
#include <asm/leds.h>
=======
#include <mach/cm.h>
#include <mach/irqs.h>

#include <asm/leds.h>
#include <asm/mach-types.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/mach/time.h>
#include <asm/pgtable.h>

static struct amba_pl010_data integrator_uart_data;

<<<<<<< HEAD
static struct amba_device rtc_device = {
	.dev		= {
		.init_name = "mb:15",
	},
	.res		= {
		.start	= INTEGRATOR_RTC_BASE,
		.end	= INTEGRATOR_RTC_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.irq		= { IRQ_RTCINT, NO_IRQ },
	.periphid	= 0x00041030,
};

static struct amba_device uart0_device = {
	.dev		= {
		.init_name = "mb:16",
		.platform_data = &integrator_uart_data,
	},
	.res		= {
		.start	= INTEGRATOR_UART0_BASE,
		.end	= INTEGRATOR_UART0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.irq		= { IRQ_UARTINT0, NO_IRQ },
	.periphid	= 0x0041010,
};

static struct amba_device uart1_device = {
	.dev		= {
		.init_name = "mb:17",
		.platform_data = &integrator_uart_data,
	},
	.res		= {
		.start	= INTEGRATOR_UART1_BASE,
		.end	= INTEGRATOR_UART1_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.irq		= { IRQ_UARTINT1, NO_IRQ },
	.periphid	= 0x0041010,
};

static struct amba_device kmi0_device = {
	.dev		= {
		.init_name = "mb:18",
	},
	.res		= {
		.start	= KMI0_BASE,
		.end	= KMI0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.irq		= { IRQ_KMIINT0, NO_IRQ },
	.periphid	= 0x00041050,
};

static struct amba_device kmi1_device = {
	.dev		= {
		.init_name = "mb:19",
	},
	.res		= {
		.start	= KMI1_BASE,
		.end	= KMI1_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.irq		= { IRQ_KMIINT1, NO_IRQ },
	.periphid	= 0x00041050,
};
=======
#define INTEGRATOR_RTC_IRQ	{ IRQ_RTCINT }
#define INTEGRATOR_UART0_IRQ	{ IRQ_UARTINT0 }
#define INTEGRATOR_UART1_IRQ	{ IRQ_UARTINT1 }
#define KMI0_IRQ		{ IRQ_KMIINT0 }
#define KMI1_IRQ		{ IRQ_KMIINT1 }

static AMBA_APB_DEVICE(rtc, "mb:15", 0,
	INTEGRATOR_RTC_BASE, INTEGRATOR_RTC_IRQ, NULL);

static AMBA_APB_DEVICE(uart0, "mb:16", 0,
	INTEGRATOR_UART0_BASE, INTEGRATOR_UART0_IRQ, &integrator_uart_data);

static AMBA_APB_DEVICE(uart1, "mb:17", 0,
	INTEGRATOR_UART1_BASE, INTEGRATOR_UART1_IRQ, &integrator_uart_data);

static AMBA_APB_DEVICE(kmi0, "mb:18", 0, KMI0_BASE, KMI0_IRQ, NULL);
static AMBA_APB_DEVICE(kmi1, "mb:19", 0, KMI1_BASE, KMI1_IRQ, NULL);
>>>>>>> refs/remotes/origin/cm-10.0

static struct amba_device *amba_devs[] __initdata = {
	&rtc_device,
	&uart0_device,
	&uart1_device,
	&kmi0_device,
	&kmi1_device,
};

/*
 * These are fixed clocks.
 */
static struct clk clk24mhz = {
	.rate	= 24000000,
};

static struct clk uartclk = {
	.rate	= 14745600,
};

static struct clk dummy_apb_pclk;

static struct clk_lookup lookups[] = {
	{	/* Bus clock */
		.con_id		= "apb_pclk",
		.clk		= &dummy_apb_pclk,
<<<<<<< HEAD
=======
	}, {
		/* Integrator/AP timer frequency */
		.dev_id		= "ap_timer",
		.clk		= &clk24mhz,
>>>>>>> refs/remotes/origin/cm-10.0
	}, {	/* UART0 */
		.dev_id		= "mb:16",
		.clk		= &uartclk,
	}, {	/* UART1 */
		.dev_id		= "mb:17",
		.clk		= &uartclk,
	}, {	/* KMI0 */
		.dev_id		= "mb:18",
		.clk		= &clk24mhz,
	}, {	/* KMI1 */
		.dev_id		= "mb:19",
		.clk		= &clk24mhz,
	}, {	/* MMCI - IntegratorCP */
		.dev_id		= "mb:1c",
		.clk		= &uartclk,
	}
};

void __init integrator_init_early(void)
{
	clkdev_add_table(lookups, ARRAY_SIZE(lookups));
}

static int __init integrator_init(void)
{
	int i;

<<<<<<< HEAD
=======
	/*
	 * The Integrator/AP lacks necessary AMBA PrimeCell IDs, so we need to
	 * hard-code them. The Integator/CP and forward have proper cell IDs.
	 * Else we leave them undefined to the bus driver can autoprobe them.
	 */
	if (machine_is_integrator()) {
		rtc_device.periphid	= 0x00041030;
		uart0_device.periphid	= 0x00041010;
		uart1_device.periphid	= 0x00041010;
		kmi0_device.periphid	= 0x00041050;
		kmi1_device.periphid	= 0x00041050;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	for (i = 0; i < ARRAY_SIZE(amba_devs); i++) {
		struct amba_device *d = amba_devs[i];
		amba_device_register(d, &iomem_resource);
	}

	return 0;
}

arch_initcall(integrator_init);

/*
 * On the Integrator platform, the port RTS and DTR are provided by
 * bits in the following SC_CTRLS register bits:
 *        RTS  DTR
 *  UART0  7    6
 *  UART1  5    4
 */
#define SC_CTRLC	IO_ADDRESS(INTEGRATOR_SC_CTRLC)
#define SC_CTRLS	IO_ADDRESS(INTEGRATOR_SC_CTRLS)

static void integrator_uart_set_mctrl(struct amba_device *dev, void __iomem *base, unsigned int mctrl)
{
	unsigned int ctrls = 0, ctrlc = 0, rts_mask, dtr_mask;

	if (dev == &uart0_device) {
		rts_mask = 1 << 4;
		dtr_mask = 1 << 5;
	} else {
		rts_mask = 1 << 6;
		dtr_mask = 1 << 7;
	}

	if (mctrl & TIOCM_RTS)
		ctrlc |= rts_mask;
	else
		ctrls |= rts_mask;

	if (mctrl & TIOCM_DTR)
		ctrlc |= dtr_mask;
	else
		ctrls |= dtr_mask;

	__raw_writel(ctrls, SC_CTRLS);
	__raw_writel(ctrlc, SC_CTRLC);
}

static struct amba_pl010_data integrator_uart_data = {
	.set_mctrl = integrator_uart_set_mctrl,
};

#define CM_CTRL	IO_ADDRESS(INTEGRATOR_HDR_CTRL)

static DEFINE_RAW_SPINLOCK(cm_lock);

/**
 * cm_control - update the CM_CTRL register.
 * @mask: bits to change
 * @set: bits to set
 */
void cm_control(u32 mask, u32 set)
{
	unsigned long flags;
	u32 val;

	raw_spin_lock_irqsave(&cm_lock, flags);
	val = readl(CM_CTRL) & ~mask;
	writel(val | set, CM_CTRL);
	raw_spin_unlock_irqrestore(&cm_lock, flags);
}

EXPORT_SYMBOL(cm_control);

=======
#include <linux/amba/bus.h>
#include <linux/amba/serial.h>
#include <linux/io.h>
#include <linux/stat.h>
#include <linux/of.h>
#include <linux/of_address.h>

#include <mach/hardware.h>
#include <mach/platform.h>

#include <asm/mach-types.h>
#include <asm/mach/time.h>
#include <asm/pgtable.h>

#include "cm.h"
#include "common.h"

static DEFINE_RAW_SPINLOCK(cm_lock);
static void __iomem *cm_base;

/**
 * cm_get - get the value from the CM_CTRL register
 */
u32 cm_get(void)
{
	return readl(cm_base + INTEGRATOR_HDR_CTRL_OFFSET);
}

/**
 * cm_control - update the CM_CTRL register.
 * @mask: bits to change
 * @set: bits to set
 */
void cm_control(u32 mask, u32 set)
{
	unsigned long flags;
	u32 val;

	raw_spin_lock_irqsave(&cm_lock, flags);
	val = readl(cm_base + INTEGRATOR_HDR_CTRL_OFFSET) & ~mask;
	writel(val | set, cm_base + INTEGRATOR_HDR_CTRL_OFFSET);
	raw_spin_unlock_irqrestore(&cm_lock, flags);
}

static const char *integrator_arch_str(u32 id)
{
	switch ((id >> 16) & 0xff) {
	case 0x00:
		return "ASB little-endian";
	case 0x01:
		return "AHB little-endian";
	case 0x03:
		return "AHB-Lite system bus, bi-endian";
	case 0x04:
		return "AHB";
	case 0x08:
		return "AHB system bus, ASB processor bus";
	default:
		return "Unknown";
	}
}

static const char *integrator_fpga_str(u32 id)
{
	switch ((id >> 12) & 0xf) {
	case 0x01:
		return "XC4062";
	case 0x02:
		return "XC4085";
	case 0x03:
		return "XVC600";
	case 0x04:
		return "EPM7256AE (Altera PLD)";
	default:
		return "Unknown";
	}
}

void cm_clear_irqs(void)
{
	/* disable core module IRQs */
	writel(0xffffffffU, cm_base + INTEGRATOR_HDR_IC_OFFSET +
		IRQ_ENABLE_CLEAR);
}

static const struct of_device_id cm_match[] = {
	{ .compatible = "arm,core-module-integrator"},
	{ },
};

void cm_init(void)
{
	struct device_node *cm = of_find_matching_node(NULL, cm_match);
	u32 val;

	if (!cm) {
		pr_crit("no core module node found in device tree\n");
		return;
	}
	cm_base = of_iomap(cm, 0);
	if (!cm_base) {
		pr_crit("could not remap core module\n");
		return;
	}
	cm_clear_irqs();
	val = readl(cm_base + INTEGRATOR_HDR_ID_OFFSET);
	pr_info("Detected ARM core module:\n");
	pr_info("    Manufacturer: %02x\n", (val >> 24));
	pr_info("    Architecture: %s\n", integrator_arch_str(val));
	pr_info("    FPGA: %s\n", integrator_fpga_str(val));
	pr_info("    Build: %02x\n", (val >> 4) & 0xFF);
	pr_info("    Rev: %c\n", ('A' + (val & 0x03)));
}

>>>>>>> refs/remotes/origin/master
/*
 * We need to stop things allocating the low memory; ideally we need a
 * better implementation of GFP_DMA which does not assume that DMA-able
 * memory starts at zero.
 */
void __init integrator_reserve(void)
{
	memblock_reserve(PHYS_OFFSET, __pa(swapper_pg_dir) - PHYS_OFFSET);
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

/*
 * To reset, we hit the on-board reset register in the system FPGA
 */
<<<<<<< HEAD
void integrator_restart(char mode, const char *cmd)
{
	cm_control(CM_CTRL_RESET, CM_CTRL_RESET);
}
>>>>>>> refs/remotes/origin/cm-10.0
=======
void integrator_restart(enum reboot_mode mode, const char *cmd)
{
	cm_control(CM_CTRL_RESET, CM_CTRL_RESET);
}

static u32 integrator_id;

static ssize_t intcp_get_manf(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
{
	return sprintf(buf, "%02x\n", integrator_id >> 24);
}

static struct device_attribute intcp_manf_attr =
	__ATTR(manufacturer,  S_IRUGO, intcp_get_manf,  NULL);

static ssize_t intcp_get_arch(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
{
	return sprintf(buf, "%s\n", integrator_arch_str(integrator_id));
}

static struct device_attribute intcp_arch_attr =
	__ATTR(architecture,  S_IRUGO, intcp_get_arch,  NULL);

static ssize_t intcp_get_fpga(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
{
	return sprintf(buf, "%s\n", integrator_fpga_str(integrator_id));
}

static struct device_attribute intcp_fpga_attr =
	__ATTR(fpga,  S_IRUGO, intcp_get_fpga,  NULL);

static ssize_t intcp_get_build(struct device *dev,
			       struct device_attribute *attr,
			       char *buf)
{
	return sprintf(buf, "%02x\n", (integrator_id >> 4) & 0xFF);
}

static struct device_attribute intcp_build_attr =
	__ATTR(build,  S_IRUGO, intcp_get_build,  NULL);



void integrator_init_sysfs(struct device *parent, u32 id)
{
	integrator_id = id;
	device_create_file(parent, &intcp_manf_attr);
	device_create_file(parent, &intcp_arch_attr);
	device_create_file(parent, &intcp_fpga_attr);
	device_create_file(parent, &intcp_build_attr);
}
>>>>>>> refs/remotes/origin/master
