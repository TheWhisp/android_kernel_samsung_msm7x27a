/*
 * Copyright (C) ST-Ericsson SA 2010
 *
 * Author: Rabin Vincent <rabin.vincent@stericsson.com> for ST-Ericsson
<<<<<<< HEAD
<<<<<<< HEAD
=======
 * Author: Lee Jones <lee.jones@linaro.org> for ST-Ericsson
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Author: Lee Jones <lee.jones@linaro.org> for ST-Ericsson
>>>>>>> refs/remotes/origin/master
 * License terms: GNU General Public License (GPL) version 2
 */

#include <linux/platform_device.h>
#include <linux/io.h>
<<<<<<< HEAD
#include <linux/clk.h>
#include <linux/mfd/db8500-prcmu.h>
#include <linux/mfd/db5500-prcmu.h>
<<<<<<< HEAD

#include <asm/cacheflush.h>
#include <asm/hardware/cache-l2x0.h>
#include <asm/hardware/gic.h>
#include <asm/mach/map.h>
#include <asm/localtimer.h>

#include <plat/mtu.h>
=======
=======
#include <linux/mfd/dbx500-prcmu.h>
>>>>>>> refs/remotes/origin/master
#include <linux/clksrc-dbx500-prcmu.h>
#include <linux/sys_soc.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/of.h>
#include <linux/of_irq.h>
<<<<<<< HEAD

#include <asm/hardware/gic.h>
#include <asm/mach/map.h>

>>>>>>> refs/remotes/origin/cm-10.0
#include <mach/hardware.h>
#include <mach/setup.h>
#include <mach/devices.h>

#include "clock.h"

void __iomem *_PRCMU_BASE;

<<<<<<< HEAD
#ifdef CONFIG_CACHE_L2X0
static void __iomem *l2x0_base;
#endif
=======
static const struct of_device_id ux500_dt_irq_match[] = {
	{ .compatible = "arm,cortex-a9-gic", .data = gic_of_init, },
	{},
};
>>>>>>> refs/remotes/origin/cm-10.0

=======
#include <linux/irq.h>
#include <linux/irqchip.h>
#include <linux/irqchip/arm-gic.h>
#include <linux/platform_data/clk-ux500.h>
#include <linux/platform_data/arm-ux500-pm.h>

#include <asm/mach/map.h>

#include "setup.h"
#include "devices.h"

#include "board-mop500.h"
#include "db8500-regs.h"
#include "id.h"

void ux500_restart(enum reboot_mode mode, const char *cmd)
{
	local_irq_disable();
	local_fiq_disable();

	prcmu_system_reset(0);
}

/*
 * FIXME: Should we set up the GPIO domain here?
 *
 * The problem is that we cannot put the interrupt resources into the platform
 * device until the irqdomain has been added. Right now, we set the GIC interrupt
 * domain from init_irq(), then load the gpio driver from
 * core_initcall(nmk_gpio_init) and add the platform devices from
 * arch_initcall(customize_machine).
 *
 * This feels fragile because it depends on the gpio device getting probed
 * _before_ any device uses the gpio interrupts.
*/
>>>>>>> refs/remotes/origin/master
void __init ux500_init_irq(void)
{
	void __iomem *dist_base;
	void __iomem *cpu_base;

<<<<<<< HEAD
	if (cpu_is_u5500()) {
		dist_base = __io_address(U5500_GIC_DIST_BASE);
		cpu_base = __io_address(U5500_GIC_CPU_BASE);
	} else if (cpu_is_u8500()) {
=======
	gic_arch_extn.flags = IRQCHIP_SKIP_SET_WAKE | IRQCHIP_MASK_ON_SUSPEND;

	if (cpu_is_u8500_family() || cpu_is_ux540_family()) {
>>>>>>> refs/remotes/origin/master
		dist_base = __io_address(U8500_GIC_DIST_BASE);
		cpu_base = __io_address(U8500_GIC_CPU_BASE);
	} else
		ux500_unknown_soc();

<<<<<<< HEAD
<<<<<<< HEAD
	gic_init(0, 29, dist_base, cpu_base);
=======
#ifdef CONFIG_OF
	if (of_have_populated_dt())
		of_irq_init(ux500_dt_irq_match);
	else
#endif
		gic_init(0, 29, dist_base, cpu_base);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_OF
	if (of_have_populated_dt())
		irqchip_init();
	else
#endif
		gic_init(0, 29, dist_base, cpu_base);
>>>>>>> refs/remotes/origin/master

	/*
	 * Init clocks here so that they are available for system timer
	 * initialization.
	 */
<<<<<<< HEAD
	if (cpu_is_u5500())
		db5500_prcmu_early_init();
	if (cpu_is_u8500())
<<<<<<< HEAD
		prcmu_early_init();
	clk_init();
}

#ifdef CONFIG_CACHE_L2X0
static inline void ux500_cache_wait(void __iomem *reg, unsigned long mask)
{
	/* wait for the operation to complete */
	while (readl_relaxed(reg) & mask)
		;
}

static inline void ux500_cache_sync(void)
{
	void __iomem *base = l2x0_base;

	writel_relaxed(0, base + L2X0_CACHE_SYNC);
	ux500_cache_wait(base + L2X0_CACHE_SYNC, 1);
}

/*
 * The L2 cache cannot be turned off in the non-secure world.
 * Dummy until a secure service is in place.
 */
static void ux500_l2x0_disable(void)
{
}

/*
 * This is only called when doing a kexec, just after turning off the L2
 * and L1 cache, and it is surrounded by a spinlock in the generic version.
 * However, we're not really turning off the L2 cache right now and the
 * PL310 does not support exclusive accesses (used to implement the spinlock).
 * So, the invalidation needs to be done without the spinlock.
 */
static void ux500_l2x0_inv_all(void)
{
	void __iomem *base = l2x0_base;
	uint32_t l2x0_way_mask = (1<<16) - 1;	/* Bitmask of active ways */

	/* invalidate all ways */
	writel_relaxed(l2x0_way_mask, base + L2X0_INV_WAY);
	ux500_cache_wait(base + L2X0_INV_WAY, l2x0_way_mask);
	ux500_cache_sync();
}

static int __init ux500_l2x0_unlock(void)
{
	int i;

	/*
	 * Unlock Data and Instruction Lock if locked. Ux500 U-Boot versions
	 * apparently locks both caches before jumping to the kernel. The
	 * l2x0 core will not touch the unlock registers if the l2x0 is
	 * already enabled, so we do it right here instead. The PL310 has
	 * 8 sets of registers, one per possible CPU.
	 */
	for (i = 0; i < 8; i++) {
		writel_relaxed(0x0, l2x0_base + L2X0_LOCKDOWN_WAY_D_BASE +
			       i * L2X0_LOCKDOWN_STRIDE);
		writel_relaxed(0x0, l2x0_base + L2X0_LOCKDOWN_WAY_I_BASE +
			       i * L2X0_LOCKDOWN_STRIDE);
	}
	return 0;
}

static int __init ux500_l2x0_init(void)
{
	if (cpu_is_u5500())
		l2x0_base = __io_address(U5500_L2CC_BASE);
	else if (cpu_is_u8500())
		l2x0_base = __io_address(U8500_L2CC_BASE);
	else
		ux500_unknown_soc();

	/* Unlock before init */
	ux500_l2x0_unlock();

	/* 64KB way size, 8 way associativity, force WA */
	l2x0_init(l2x0_base, 0x3e060000, 0xc0000fff);

	/* Override invalidate function */
	outer_cache.disable = ux500_l2x0_disable;
	outer_cache.inv_all = ux500_l2x0_inv_all;

	return 0;
}
early_initcall(ux500_l2x0_init);
#endif

static void __init ux500_timer_init(void)
{
#ifdef CONFIG_LOCAL_TIMERS
	/* Setup the local timer base */
	if (cpu_is_u5500())
		twd_base = __io_address(U5500_TWD_BASE);
	else if (cpu_is_u8500())
		twd_base = __io_address(U8500_TWD_BASE);
	else
		ux500_unknown_soc();
#endif
	if (cpu_is_u5500())
		mtu_base = __io_address(U5500_MTU0_BASE);
	else if (cpu_is_u8500ed())
		mtu_base = __io_address(U8500_MTU0_BASE_ED);
	else if (cpu_is_u8500())
		mtu_base = __io_address(U8500_MTU0_BASE);
	else
		ux500_unknown_soc();

	nmdk_timer_init();
}

struct sys_timer ux500_timer = {
	.init	= ux500_timer_init,
};
=======
		db8500_prcmu_early_init();
	clk_init();
=======
	if (cpu_is_u8500_family()) {
		prcmu_early_init(U8500_PRCMU_BASE, SZ_8K - 1);
		ux500_pm_init(U8500_PRCMU_BASE, SZ_8K - 1);

		if (of_have_populated_dt())
			u8500_of_clk_init(U8500_CLKRST1_BASE,
					  U8500_CLKRST2_BASE,
					  U8500_CLKRST3_BASE,
					  U8500_CLKRST5_BASE,
					  U8500_CLKRST6_BASE);
		else
			u8500_clk_init(U8500_CLKRST1_BASE, U8500_CLKRST2_BASE,
				       U8500_CLKRST3_BASE, U8500_CLKRST5_BASE,
				       U8500_CLKRST6_BASE);
	} else if (cpu_is_u9540()) {
		prcmu_early_init(U8500_PRCMU_BASE, SZ_8K - 1);
		ux500_pm_init(U8500_PRCMU_BASE, SZ_8K - 1);
		u9540_clk_init(U8500_CLKRST1_BASE, U8500_CLKRST2_BASE,
			       U8500_CLKRST3_BASE, U8500_CLKRST5_BASE,
			       U8500_CLKRST6_BASE);
	} else if (cpu_is_u8540()) {
		prcmu_early_init(U8500_PRCMU_BASE, SZ_8K + SZ_4K - 1);
		ux500_pm_init(U8500_PRCMU_BASE, SZ_8K + SZ_4K - 1);
		u8540_clk_init(U8500_CLKRST1_BASE, U8500_CLKRST2_BASE,
			       U8500_CLKRST3_BASE, U8500_CLKRST5_BASE,
			       U8500_CLKRST6_BASE);
	}
>>>>>>> refs/remotes/origin/master
}

static const char * __init ux500_get_machine(void)
{
	return kasprintf(GFP_KERNEL, "DB%4x", dbx500_partnumber());
}

static const char * __init ux500_get_family(void)
{
	return kasprintf(GFP_KERNEL, "ux500");
}

static const char * __init ux500_get_revision(void)
{
	unsigned int rev = dbx500_revision();

	if (rev == 0x01)
		return kasprintf(GFP_KERNEL, "%s", "ED");
	else if (rev >= 0xA0)
		return kasprintf(GFP_KERNEL, "%d.%d",
				 (rev >> 4) - 0xA + 1, rev & 0xf);

	return kasprintf(GFP_KERNEL, "%s", "Unknown");
}

static ssize_t ux500_get_process(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	if (dbx500_id.process == 0x00)
		return sprintf(buf, "Standard\n");

	return sprintf(buf, "%02xnm\n", dbx500_id.process);
}

static void __init soc_info_populate(struct soc_device_attribute *soc_dev_attr,
				     const char *soc_id)
{
	soc_dev_attr->soc_id   = soc_id;
	soc_dev_attr->machine  = ux500_get_machine();
	soc_dev_attr->family   = ux500_get_family();
	soc_dev_attr->revision = ux500_get_revision();
}

struct device_attribute ux500_soc_attr =
	__ATTR(process,  S_IRUGO, ux500_get_process,  NULL);

struct device * __init ux500_soc_device_init(const char *soc_id)
{
	struct device *parent;
	struct soc_device *soc_dev;
	struct soc_device_attribute *soc_dev_attr;

	soc_dev_attr = kzalloc(sizeof(*soc_dev_attr), GFP_KERNEL);
	if (!soc_dev_attr)
		return ERR_PTR(-ENOMEM);

	soc_info_populate(soc_dev_attr, soc_id);

	soc_dev = soc_device_register(soc_dev_attr);
<<<<<<< HEAD
	if (IS_ERR_OR_NULL(soc_dev)) {
=======
	if (IS_ERR(soc_dev)) {
>>>>>>> refs/remotes/origin/master
	        kfree(soc_dev_attr);
		return NULL;
	}

	parent = soc_device_to_device(soc_dev);
<<<<<<< HEAD
	if (!IS_ERR_OR_NULL(parent))
		device_create_file(parent, &ux500_soc_attr);

	return parent;
}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	device_create_file(parent, &ux500_soc_attr);

	return parent;
}
>>>>>>> refs/remotes/origin/master
