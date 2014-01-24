/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2008 Maxime Bizon <mbizon@freebox.fr>
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/bootmem.h>
#include <linux/ioport.h>
#include <linux/pm.h>
#include <asm/bootinfo.h>
#include <asm/time.h>
#include <asm/reboot.h>
#include <asm/cacheflush.h>
#include <bcm63xx_board.h>
#include <bcm63xx_cpu.h>
#include <bcm63xx_regs.h>
#include <bcm63xx_io.h>

void bcm63xx_machine_halt(void)
{
	printk(KERN_INFO "System halted\n");
	while (1)
		;
}

static void bcm6348_a1_reboot(void)
{
	u32 reg;

	/* soft reset all blocks */
<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "soft-reseting all blocks ...\n");
=======
	printk(KERN_INFO "soft-resetting all blocks ...\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	printk(KERN_INFO "soft-resetting all blocks ...\n");
>>>>>>> refs/remotes/origin/master
	reg = bcm_perf_readl(PERF_SOFTRESET_REG);
	reg &= ~SOFTRESET_6348_ALL;
	bcm_perf_writel(reg, PERF_SOFTRESET_REG);
	mdelay(10);

	reg = bcm_perf_readl(PERF_SOFTRESET_REG);
	reg |= SOFTRESET_6348_ALL;
	bcm_perf_writel(reg, PERF_SOFTRESET_REG);
	mdelay(10);

	/* Jump to the power on address. */
	printk(KERN_INFO "jumping to reset vector.\n");
	/* set high vectors (base at 0xbfc00000 */
	set_c0_status(ST0_BEV | ST0_ERL);
	/* run uncached in kseg0 */
	change_c0_config(CONF_CM_CMASK, CONF_CM_UNCACHED);
	__flush_cache_all();
	/* remove all wired TLB entries */
	write_c0_wired(0);
	__asm__ __volatile__(
		"jr\t%0"
		:
		: "r" (0xbfc00000));
	while (1)
		;
}

void bcm63xx_machine_reboot(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u32 reg;

	/* mask and clear all external irq */
	reg = bcm_perf_readl(PERF_EXTIRQ_CFG_REG);
	reg &= ~EXTIRQ_CFG_MASK_ALL;
	reg |= EXTIRQ_CFG_CLEAR_ALL;
	bcm_perf_writel(reg, PERF_EXTIRQ_CFG_REG);
=======
=======
>>>>>>> refs/remotes/origin/master
	u32 reg, perf_regs[2] = { 0, 0 };
	unsigned int i;

	/* mask and clear all external irq */
	switch (bcm63xx_get_cpu_id()) {
<<<<<<< HEAD
	case BCM6338_CPU_ID:
		perf_regs[0] = PERF_EXTIRQ_CFG_REG_6338;
		break;
=======
	case BCM3368_CPU_ID:
		perf_regs[0] = PERF_EXTIRQ_CFG_REG_3368;
		break;
	case BCM6328_CPU_ID:
		perf_regs[0] = PERF_EXTIRQ_CFG_REG_6328;
		break;
	case BCM6338_CPU_ID:
		perf_regs[0] = PERF_EXTIRQ_CFG_REG_6338;
		break;
	case BCM6345_CPU_ID:
		perf_regs[0] = PERF_EXTIRQ_CFG_REG_6345;
		break;
>>>>>>> refs/remotes/origin/master
	case BCM6348_CPU_ID:
		perf_regs[0] = PERF_EXTIRQ_CFG_REG_6348;
		break;
	case BCM6358_CPU_ID:
		perf_regs[0] = PERF_EXTIRQ_CFG_REG_6358;
		break;
<<<<<<< HEAD
	}

	for (i = 0; i < 2; i++) {
=======
	case BCM6362_CPU_ID:
		perf_regs[0] = PERF_EXTIRQ_CFG_REG_6362;
		break;
	}

	for (i = 0; i < 2; i++) {
		if (!perf_regs[i])
			break;

>>>>>>> refs/remotes/origin/master
		reg = bcm_perf_readl(perf_regs[i]);
		if (BCMCPU_IS_6348()) {
			reg &= ~EXTIRQ_CFG_MASK_ALL_6348;
			reg |= EXTIRQ_CFG_CLEAR_ALL_6348;
		} else {
			reg &= ~EXTIRQ_CFG_MASK_ALL;
			reg |= EXTIRQ_CFG_CLEAR_ALL;
		}
		bcm_perf_writel(reg, perf_regs[i]);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (BCMCPU_IS_6348() && (bcm63xx_get_cpu_rev() == 0xa1))
		bcm6348_a1_reboot();

	printk(KERN_INFO "triggering watchdog soft-reset...\n");
<<<<<<< HEAD
	reg = bcm_perf_readl(PERF_SYS_PLL_CTL_REG);
	reg |= SYS_PLL_SOFT_RESET;
	bcm_perf_writel(reg, PERF_SYS_PLL_CTL_REG);
=======
	if (BCMCPU_IS_6328()) {
		bcm_wdt_writel(1, WDT_SOFTRESET_REG);
	} else {
		reg = bcm_perf_readl(PERF_SYS_PLL_CTL_REG);
		reg |= SYS_PLL_SOFT_RESET;
		bcm_perf_writel(reg, PERF_SYS_PLL_CTL_REG);
	}
>>>>>>> refs/remotes/origin/master
	while (1)
		;
}

static void __bcm63xx_machine_reboot(char *p)
{
	bcm63xx_machine_reboot();
}

/*
 * return system type in /proc/cpuinfo
 */
const char *get_system_type(void)
{
	static char buf[128];
<<<<<<< HEAD
	snprintf(buf, sizeof(buf), "bcm63xx/%s (0x%04x/0x%04X)",
=======
	snprintf(buf, sizeof(buf), "bcm63xx/%s (0x%04x/0x%02X)",
>>>>>>> refs/remotes/origin/master
		 board_get_name(),
		 bcm63xx_get_cpu_id(), bcm63xx_get_cpu_rev());
	return buf;
}

void __init plat_time_init(void)
{
	mips_hpt_frequency = bcm63xx_get_cpu_freq() / 2;
}

void __init plat_mem_setup(void)
{
	add_memory_region(0, bcm63xx_get_memory_size(), BOOT_MEM_RAM);

	_machine_halt = bcm63xx_machine_halt;
	_machine_restart = __bcm63xx_machine_reboot;
	pm_power_off = bcm63xx_machine_halt;

	set_io_port_base(0);
	ioport_resource.start = 0;
	ioport_resource.end = ~0;

	board_setup();
}

int __init bcm63xx_register_devices(void)
{
	return board_register_devices();
}

<<<<<<< HEAD
<<<<<<< HEAD
arch_initcall(bcm63xx_register_devices);
=======
device_initcall(bcm63xx_register_devices);
>>>>>>> refs/remotes/origin/cm-10.0
=======
arch_initcall(bcm63xx_register_devices);
>>>>>>> refs/remotes/origin/master
