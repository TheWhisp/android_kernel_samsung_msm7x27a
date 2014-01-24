/*
 * Exynos Generic power domain support.
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Implementation of Exynos specific power domain control which is used in
 * conjunction with runtime-pm. Support for both device-tree and non-device-tree
 * based power domain support is included.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/io.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/pm_domain.h>
#include <linux/delay.h>
#include <linux/of_address.h>
<<<<<<< HEAD
=======
#include <linux/of_platform.h>
#include <linux/sched.h>
>>>>>>> refs/remotes/origin/master

#include <mach/regs-pmu.h>
#include <plat/devs.h>

/*
 * Exynos specific wrapper around the generic power domain
 */
struct exynos_pm_domain {
	void __iomem *base;
	char const *name;
	bool is_off;
	struct generic_pm_domain pd;
};

static int exynos_pd_power(struct generic_pm_domain *domain, bool power_on)
{
	struct exynos_pm_domain *pd;
	void __iomem *base;
	u32 timeout, pwr;
	char *op;

	pd = container_of(domain, struct exynos_pm_domain, pd);
	base = pd->base;

	pwr = power_on ? S5P_INT_LOCAL_PWR_EN : 0;
	__raw_writel(pwr, base);

	/* Wait max 1ms */
	timeout = 10;

	while ((__raw_readl(base + 0x4) & S5P_INT_LOCAL_PWR_EN)	!= pwr) {
		if (!timeout) {
			op = (power_on) ? "enable" : "disable";
			pr_err("Power domain %s %s failed\n", domain->name, op);
			return -ETIMEDOUT;
		}
		timeout--;
		cpu_relax();
		usleep_range(80, 100);
	}
	return 0;
}

static int exynos_pd_power_on(struct generic_pm_domain *domain)
{
	return exynos_pd_power(domain, true);
}

static int exynos_pd_power_off(struct generic_pm_domain *domain)
{
	return exynos_pd_power(domain, false);
}

<<<<<<< HEAD
#define EXYNOS_GPD(PD, BASE, NAME)			\
static struct exynos_pm_domain PD = {			\
	.base = (void __iomem *)BASE,			\
	.name = NAME,					\
	.pd = {						\
		.power_off = exynos_pd_power_off,	\
		.power_on = exynos_pd_power_on,	\
	},						\
}

#ifdef CONFIG_OF
static __init int exynos_pm_dt_parse_domains(void)
{
=======
static void exynos_add_device_to_domain(struct exynos_pm_domain *pd,
					 struct device *dev)
{
	int ret;

	dev_dbg(dev, "adding to power domain %s\n", pd->pd.name);

	while (1) {
		ret = pm_genpd_add_device(&pd->pd, dev);
		if (ret != -EAGAIN)
			break;
		cond_resched();
	}

	pm_genpd_dev_need_restore(dev, true);
}

static void exynos_remove_device_from_domain(struct device *dev)
{
	struct generic_pm_domain *genpd = dev_to_genpd(dev);
	int ret;

	dev_dbg(dev, "removing from power domain %s\n", genpd->name);

	while (1) {
		ret = pm_genpd_remove_device(genpd, dev);
		if (ret != -EAGAIN)
			break;
		cond_resched();
	}
}

static void exynos_read_domain_from_dt(struct device *dev)
{
	struct platform_device *pd_pdev;
	struct exynos_pm_domain *pd;
	struct device_node *node;

	node = of_parse_phandle(dev->of_node, "samsung,power-domain", 0);
	if (!node)
		return;
	pd_pdev = of_find_device_by_node(node);
	if (!pd_pdev)
		return;
	pd = platform_get_drvdata(pd_pdev);
	exynos_add_device_to_domain(pd, dev);
}

static int exynos_pm_notifier_call(struct notifier_block *nb,
				    unsigned long event, void *data)
{
	struct device *dev = data;

	switch (event) {
	case BUS_NOTIFY_BIND_DRIVER:
		if (dev->of_node)
			exynos_read_domain_from_dt(dev);

		break;

	case BUS_NOTIFY_UNBOUND_DRIVER:
		exynos_remove_device_from_domain(dev);

		break;
	}
	return NOTIFY_DONE;
}

static struct notifier_block platform_nb = {
	.notifier_call = exynos_pm_notifier_call,
};

static __init int exynos4_pm_init_power_domain(void)
{
	struct platform_device *pdev;
>>>>>>> refs/remotes/origin/master
	struct device_node *np;

	for_each_compatible_node(np, NULL, "samsung,exynos4210-pd") {
		struct exynos_pm_domain *pd;
<<<<<<< HEAD
=======
		int on;

		pdev = of_find_device_by_node(np);
>>>>>>> refs/remotes/origin/master

		pd = kzalloc(sizeof(*pd), GFP_KERNEL);
		if (!pd) {
			pr_err("%s: failed to allocate memory for domain\n",
					__func__);
			return -ENOMEM;
		}

<<<<<<< HEAD
		if (of_get_property(np, "samsung,exynos4210-pd-off", NULL))
			pd->is_off = true;
		pd->name = np->name;
=======
		pd->pd.name = kstrdup(np->name, GFP_KERNEL);
		pd->name = pd->pd.name;
>>>>>>> refs/remotes/origin/master
		pd->base = of_iomap(np, 0);
		pd->pd.power_off = exynos_pd_power_off;
		pd->pd.power_on = exynos_pd_power_on;
		pd->pd.of_node = np;
<<<<<<< HEAD
		pm_genpd_init(&pd->pd, NULL, false);
	}
	return 0;
}
#else
static __init int exynos_pm_dt_parse_domains(void)
{
	return 0;
}
#endif /* CONFIG_OF */

static __init void exynos_pm_add_dev_to_genpd(struct platform_device *pdev,
						struct exynos_pm_domain *pd)
{
	if (pdev->dev.bus) {
		if (pm_genpd_add_device(&pd->pd, &pdev->dev))
			pr_info("%s: error in adding %s device to %s power"
				"domain\n", __func__, dev_name(&pdev->dev),
				pd->name);
	}
}

EXYNOS_GPD(exynos4_pd_mfc, S5P_PMU_MFC_CONF, "pd-mfc");
EXYNOS_GPD(exynos4_pd_g3d, S5P_PMU_G3D_CONF, "pd-g3d");
EXYNOS_GPD(exynos4_pd_lcd0, S5P_PMU_LCD0_CONF, "pd-lcd0");
EXYNOS_GPD(exynos4_pd_lcd1, S5P_PMU_LCD1_CONF, "pd-lcd1");
EXYNOS_GPD(exynos4_pd_tv, S5P_PMU_TV_CONF, "pd-tv");
EXYNOS_GPD(exynos4_pd_cam, S5P_PMU_CAM_CONF, "pd-cam");
EXYNOS_GPD(exynos4_pd_gps, S5P_PMU_GPS_CONF, "pd-gps");

static struct exynos_pm_domain *exynos4_pm_domains[] = {
	&exynos4_pd_mfc,
	&exynos4_pd_g3d,
	&exynos4_pd_lcd0,
	&exynos4_pd_lcd1,
	&exynos4_pd_tv,
	&exynos4_pd_cam,
	&exynos4_pd_gps,
};

static __init int exynos4_pm_init_power_domain(void)
{
	int idx;

	if (of_have_populated_dt())
		return exynos_pm_dt_parse_domains();

	for (idx = 0; idx < ARRAY_SIZE(exynos4_pm_domains); idx++)
		pm_genpd_init(&exynos4_pm_domains[idx]->pd, NULL,
				exynos4_pm_domains[idx]->is_off);

#ifdef CONFIG_S5P_DEV_FIMD0
	exynos_pm_add_dev_to_genpd(&s5p_device_fimd0, &exynos4_pd_lcd0);
#endif
#ifdef CONFIG_S5P_DEV_TV
	exynos_pm_add_dev_to_genpd(&s5p_device_hdmi, &exynos4_pd_tv);
	exynos_pm_add_dev_to_genpd(&s5p_device_mixer, &exynos4_pd_tv);
#endif
#ifdef CONFIG_S5P_DEV_MFC
	exynos_pm_add_dev_to_genpd(&s5p_device_mfc, &exynos4_pd_mfc);
#endif
#ifdef CONFIG_S5P_DEV_FIMC0
	exynos_pm_add_dev_to_genpd(&s5p_device_fimc0, &exynos4_pd_cam);
#endif
#ifdef CONFIG_S5P_DEV_FIMC1
	exynos_pm_add_dev_to_genpd(&s5p_device_fimc1, &exynos4_pd_cam);
#endif
#ifdef CONFIG_S5P_DEV_FIMC2
	exynos_pm_add_dev_to_genpd(&s5p_device_fimc2, &exynos4_pd_cam);
#endif
#ifdef CONFIG_S5P_DEV_FIMC3
	exynos_pm_add_dev_to_genpd(&s5p_device_fimc3, &exynos4_pd_cam);
#endif
#ifdef CONFIG_S5P_DEV_CSIS0
	exynos_pm_add_dev_to_genpd(&s5p_device_mipi_csis0, &exynos4_pd_cam);
#endif
#ifdef CONFIG_S5P_DEV_CSIS1
	exynos_pm_add_dev_to_genpd(&s5p_device_mipi_csis1, &exynos4_pd_cam);
#endif
#ifdef CONFIG_S5P_DEV_G2D
	exynos_pm_add_dev_to_genpd(&s5p_device_g2d, &exynos4_pd_lcd0);
#endif
#ifdef CONFIG_S5P_DEV_JPEG
	exynos_pm_add_dev_to_genpd(&s5p_device_jpeg, &exynos4_pd_cam);
#endif
=======

		platform_set_drvdata(pdev, pd);

		on = __raw_readl(pd->base + 0x4) & S5P_INT_LOCAL_PWR_EN;

		pm_genpd_init(&pd->pd, NULL, !on);
	}

	bus_register_notifier(&platform_bus_type, &platform_nb);

>>>>>>> refs/remotes/origin/master
	return 0;
}
arch_initcall(exynos4_pm_init_power_domain);

<<<<<<< HEAD
static __init int exynos_pm_late_initcall(void)
=======
int __init exynos_pm_late_initcall(void)
>>>>>>> refs/remotes/origin/master
{
	pm_genpd_poweroff_unused();
	return 0;
}
<<<<<<< HEAD
late_initcall(exynos_pm_late_initcall);
=======
>>>>>>> refs/remotes/origin/master
