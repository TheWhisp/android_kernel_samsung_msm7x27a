/*
 * Versatile Express Core Tile Cortex A9x4 Support
 */
#include <linux/init.h>
#include <linux/gfp.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/amba/bus.h>
#include <linux/amba/clcd.h>
#include <linux/clkdev.h>
<<<<<<< HEAD

#include <asm/hardware/arm_timer.h>
#include <asm/hardware/cache-l2x0.h>
#include <asm/hardware/gic.h>
#include <asm/pmu.h>
=======
#include <linux/vexpress.h>
#include <linux/irqchip/arm-gic.h>

#include <asm/hardware/arm_timer.h>
#include <asm/hardware/cache-l2x0.h>
>>>>>>> refs/remotes/origin/master
#include <asm/smp_scu.h>
#include <asm/smp_twd.h>

#include <mach/ct-ca9x4.h>

#include <asm/hardware/timer-sp.h>

#include <asm/mach/map.h>
#include <asm/mach/time.h>

#include "core.h"

#include <mach/motherboard.h>
<<<<<<< HEAD

#include <plat/clcd.h>

<<<<<<< HEAD
#define V2M_PA_CS7	0x10000000

static struct map_desc ct_ca9x4_io_desc[] __initdata = {
	{
		.virtual	= __MMIO_P2V(CT_CA9X4_MPIC),
		.pfn		= __phys_to_pfn(CT_CA9X4_MPIC),
		.length		= SZ_16K,
		.type		= MT_DEVICE,
	}, {
		.virtual	= __MMIO_P2V(CT_CA9X4_SP804_TIMER),
		.pfn		= __phys_to_pfn(CT_CA9X4_SP804_TIMER),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	}, {
		.virtual	= __MMIO_P2V(CT_CA9X4_L2CC),
		.pfn		= __phys_to_pfn(CT_CA9X4_L2CC),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
=======
=======
#include <mach/irqs.h>

#include <plat/clcd.h>

>>>>>>> refs/remotes/origin/master
static struct map_desc ct_ca9x4_io_desc[] __initdata = {
	{
		.virtual        = V2T_PERIPH,
		.pfn            = __phys_to_pfn(CT_CA9X4_MPIC),
		.length         = SZ_8K,
		.type           = MT_DEVICE,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	},
};

static void __init ct_ca9x4_map_io(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_LOCAL_TIMERS
	twd_base = MMIO_P2V(A9_MPCORE_TWD);
#endif
	iotable_init(ct_ca9x4_io_desc, ARRAY_SIZE(ct_ca9x4_io_desc));
}

static void __init ct_ca9x4_init_irq(void)
{
	gic_init(0, 29, MMIO_P2V(A9_MPCORE_GIC_DIST),
		 MMIO_P2V(A9_MPCORE_GIC_CPU));
}

#if 0
static void __init ct_ca9x4_timer_init(void)
{
	writel(0, MMIO_P2V(CT_CA9X4_TIMER0) + TIMER_CTRL);
	writel(0, MMIO_P2V(CT_CA9X4_TIMER1) + TIMER_CTRL);

	sp804_clocksource_init(MMIO_P2V(CT_CA9X4_TIMER1), "ct-timer1");
	sp804_clockevents_init(MMIO_P2V(CT_CA9X4_TIMER0), IRQ_CT_CA9X4_TIMER0,
		"ct-timer0");
}

static struct sys_timer ct_ca9x4_timer = {
	.init	= ct_ca9x4_timer_init,
};
#endif

=======
=======
>>>>>>> refs/remotes/origin/master
	iotable_init(ct_ca9x4_io_desc, ARRAY_SIZE(ct_ca9x4_io_desc));
}

#ifdef CONFIG_HAVE_ARM_TWD
static DEFINE_TWD_LOCAL_TIMER(twd_local_timer, A9_MPCORE_TWD, IRQ_LOCALTIMER);

static void __init ca9x4_twd_init(void)
{
	int err = twd_local_timer_register(&twd_local_timer);
	if (err)
		pr_err("twd_local_timer_register failed %d\n", err);
}
#else
#define ca9x4_twd_init()	do {} while(0)
#endif

static void __init ct_ca9x4_init_irq(void)
{
	gic_init(0, 29, ioremap(A9_MPCORE_GIC_DIST, SZ_4K),
		 ioremap(A9_MPCORE_GIC_CPU, SZ_256));
	ca9x4_twd_init();
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static void ct_ca9x4_clcd_enable(struct clcd_fb *fb)
{
	v2m_cfg_write(SYS_CFG_MUXFPGA | SYS_CFG_SITE_DB1, 0);
	v2m_cfg_write(SYS_CFG_DVIMODE | SYS_CFG_SITE_DB1, 2);
}

=======
>>>>>>> refs/remotes/origin/master
static int ct_ca9x4_clcd_setup(struct clcd_fb *fb)
{
	unsigned long framesize = 1024 * 768 * 2;

	fb->panel = versatile_clcd_get_panel("XVGA");
	if (!fb->panel)
		return -EINVAL;

	return versatile_clcd_setup_dma(fb, framesize);
}

static struct clcd_board ct_ca9x4_clcd_data = {
	.name		= "CT-CA9X4",
	.caps		= CLCD_CAP_5551 | CLCD_CAP_565,
	.check		= clcdfb_check,
	.decode		= clcdfb_decode,
<<<<<<< HEAD
	.enable		= ct_ca9x4_clcd_enable,
=======
>>>>>>> refs/remotes/origin/master
	.setup		= ct_ca9x4_clcd_setup,
	.mmap		= versatile_clcd_mmap_dma,
	.remove		= versatile_clcd_remove_dma,
};

<<<<<<< HEAD
<<<<<<< HEAD
static AMBA_DEVICE(clcd, "ct:clcd", CT_CA9X4_CLCDC, &ct_ca9x4_clcd_data);
static AMBA_DEVICE(dmc, "ct:dmc", CT_CA9X4_DMC, NULL);
static AMBA_DEVICE(smc, "ct:smc", CT_CA9X4_SMC, NULL);
static AMBA_DEVICE(gpio, "ct:gpio", CT_CA9X4_GPIO, NULL);
=======
=======
>>>>>>> refs/remotes/origin/master
static AMBA_AHB_DEVICE(clcd, "ct:clcd", 0, CT_CA9X4_CLCDC, IRQ_CT_CA9X4_CLCDC, &ct_ca9x4_clcd_data);
static AMBA_APB_DEVICE(dmc, "ct:dmc", 0, CT_CA9X4_DMC, IRQ_CT_CA9X4_DMC, NULL);
static AMBA_APB_DEVICE(smc, "ct:smc", 0, CT_CA9X4_SMC, IRQ_CT_CA9X4_SMC, NULL);
static AMBA_APB_DEVICE(gpio, "ct:gpio", 0, CT_CA9X4_GPIO, IRQ_CT_CA9X4_GPIO, NULL);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static struct amba_device *ct_ca9x4_amba_devs[] __initdata = {
	&clcd_device,
	&dmc_device,
	&smc_device,
	&gpio_device,
};

<<<<<<< HEAD

static long ct_round(struct clk *clk, unsigned long rate)
{
	return rate;
}

static int ct_set(struct clk *clk, unsigned long rate)
{
	return v2m_cfg_write(SYS_CFG_OSC | SYS_CFG_SITE_DB1 | 1, rate);
}

static const struct clk_ops osc1_clk_ops = {
	.round	= ct_round,
	.set	= ct_set,
};

static struct clk osc1_clk = {
	.ops	= &osc1_clk_ops,
	.rate	= 24000000,
};

static struct clk ct_sp804_clk = {
	.rate	= 1000000,
};

static struct clk_lookup lookups[] = {
	{	/* CLCD */
		.dev_id		= "ct:clcd",
		.clk		= &osc1_clk,
	}, {	/* SP804 timers */
		.dev_id		= "sp804",
		.con_id		= "ct-timer0",
		.clk		= &ct_sp804_clk,
	}, {	/* SP804 timers */
		.dev_id		= "sp804",
		.con_id		= "ct-timer1",
		.clk		= &ct_sp804_clk,
	},
};

=======
>>>>>>> refs/remotes/origin/master
static struct resource pmu_resources[] = {
	[0] = {
		.start	= IRQ_CT_CA9X4_PMU_CPU0,
		.end	= IRQ_CT_CA9X4_PMU_CPU0,
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		.start	= IRQ_CT_CA9X4_PMU_CPU1,
		.end	= IRQ_CT_CA9X4_PMU_CPU1,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_CT_CA9X4_PMU_CPU2,
		.end	= IRQ_CT_CA9X4_PMU_CPU2,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.start	= IRQ_CT_CA9X4_PMU_CPU3,
		.end	= IRQ_CT_CA9X4_PMU_CPU3,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device pmu_device = {
	.name		= "arm-pmu",
<<<<<<< HEAD
	.id		= ARM_PMU_DEVICE_CPU,
=======
	.id		= -1,
>>>>>>> refs/remotes/origin/master
	.num_resources	= ARRAY_SIZE(pmu_resources),
	.resource	= pmu_resources,
};

<<<<<<< HEAD
static void __init ct_ca9x4_init_early(void)
{
	clkdev_add_table(lookups, ARRAY_SIZE(lookups));
}
=======
static struct platform_device osc1_device = {
	.name		= "vexpress-osc",
	.id		= 1,
	.num_resources	= 1,
	.resource	= (struct resource []) {
		VEXPRESS_RES_FUNC(0xf, 1),
	},
};
>>>>>>> refs/remotes/origin/master

static void __init ct_ca9x4_init(void)
{
	int i;

#ifdef CONFIG_CACHE_L2X0
<<<<<<< HEAD
<<<<<<< HEAD
	void __iomem *l2x0_base = MMIO_P2V(CT_CA9X4_L2CC);
=======
	void __iomem *l2x0_base = ioremap(CT_CA9X4_L2CC, SZ_4K);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	void __iomem *l2x0_base = ioremap(CT_CA9X4_L2CC, SZ_4K);
>>>>>>> refs/remotes/origin/master

	/* set RAM latencies to 1 cycle for this core tile. */
	writel(0, l2x0_base + L2X0_TAG_LATENCY_CTRL);
	writel(0, l2x0_base + L2X0_DATA_LATENCY_CTRL);

	l2x0_init(l2x0_base, 0x00400000, 0xfe0fffff);
#endif

	for (i = 0; i < ARRAY_SIZE(ct_ca9x4_amba_devs); i++)
		amba_device_register(ct_ca9x4_amba_devs[i], &iomem_resource);

	platform_device_register(&pmu_device);
<<<<<<< HEAD
}

#ifdef CONFIG_SMP
<<<<<<< HEAD
static void ct_ca9x4_init_cpu_map(void)
{
	int i, ncores = scu_get_core_count(MMIO_P2V(A9_MPCORE_SCU));
=======
=======
	platform_device_register(&osc1_device);

	WARN_ON(clk_register_clkdev(vexpress_osc_setup(&osc1_device.dev),
			NULL, "ct:clcd"));
}

#ifdef CONFIG_SMP
>>>>>>> refs/remotes/origin/master
static void *ct_ca9x4_scu_base __initdata;

static void __init ct_ca9x4_init_cpu_map(void)
{
	int i, ncores;

	ct_ca9x4_scu_base = ioremap(A9_MPCORE_SCU, SZ_128);
	if (WARN_ON(!ct_ca9x4_scu_base))
		return;

	ncores = scu_get_core_count(ct_ca9x4_scu_base);

	if (ncores > nr_cpu_ids) {
		pr_warn("SMP: %u cores greater than maximum (%u), clipping\n",
			ncores, nr_cpu_ids);
		ncores = nr_cpu_ids;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	for (i = 0; i < ncores; ++i)
		set_cpu_possible(i, true);

	set_smp_cross_call(gic_raise_softirq);
}

<<<<<<< HEAD
static void ct_ca9x4_smp_enable(unsigned int max_cpus)
{
	int i;
	for (i = 0; i < max_cpus; i++)
		set_cpu_present(i, true);

	scu_enable(MMIO_P2V(A9_MPCORE_SCU));
=======
static void __init ct_ca9x4_smp_enable(unsigned int max_cpus)
{
	scu_enable(ct_ca9x4_scu_base);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	for (i = 0; i < ncores; ++i)
		set_cpu_possible(i, true);
}

static void __init ct_ca9x4_smp_enable(unsigned int max_cpus)
{
	scu_enable(ct_ca9x4_scu_base);
>>>>>>> refs/remotes/origin/master
}
#endif

struct ct_desc ct_ca9x4_desc __initdata = {
	.id		= V2M_CT_ID_CA9,
	.name		= "CA9x4",
	.map_io		= ct_ca9x4_map_io,
<<<<<<< HEAD
	.init_early	= ct_ca9x4_init_early,
=======
>>>>>>> refs/remotes/origin/master
	.init_irq	= ct_ca9x4_init_irq,
	.init_tile	= ct_ca9x4_init,
#ifdef CONFIG_SMP
	.init_cpu_map	= ct_ca9x4_init_cpu_map,
	.smp_enable	= ct_ca9x4_smp_enable,
#endif
};
