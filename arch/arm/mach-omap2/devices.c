/*
 * linux/arch/arm/mach-omap2/devices.c
 *
 * OMAP2 platform device setup/initialization
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
<<<<<<< HEAD
<<<<<<< HEAD

=======
#include <linux/gpio.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/gpio.h>
>>>>>>> refs/remotes/origin/master
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/of.h>
#include <linux/platform_data/omap4-keypad.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <mach/hardware.h>
#include <mach/irqs.h>
#include <asm/mach-types.h>
#include <asm/mach/map.h>
#include <asm/pmu.h>

<<<<<<< HEAD
#include <plat/tc.h>
#include <plat/board.h>
#include <plat/mcbsp.h>
#include <mach/gpio.h>
=======
#include "iomap.h"
#include <plat/board.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <plat/mmc.h>
#include <plat/dma.h>
#include <plat/omap_hwmod.h>
#include <plat/omap_device.h>
#include <plat/omap4-keypad.h>

#include "mux.h"
#include "control.h"
#include "devices.h"
=======
#include <linux/of.h>
#include <linux/pinctrl/machine.h>
#include <linux/platform_data/omap4-keypad.h>
#include <linux/platform_data/mailbox-omap.h>

#include <asm/mach-types.h>
#include <asm/mach/map.h>

#include <linux/omap-dma.h>

#include "iomap.h"
#include "omap_hwmod.h"
#include "omap_device.h"
#include "omap4-keypad.h"

#include "soc.h"
#include "common.h"
#include "mux.h"
#include "control.h"
#include "devices.h"
#include "display.h"
>>>>>>> refs/remotes/origin/master

#define L3_MODULES_MAX_LEN 12
#define L3_MODULES 3

static int __init omap3_l3_init(void)
{
<<<<<<< HEAD
	int l;
	struct omap_hwmod *oh;
<<<<<<< HEAD
	struct omap_device *od;
=======
	struct platform_device *pdev;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct omap_hwmod *oh;
	struct platform_device *pdev;
>>>>>>> refs/remotes/origin/master
	char oh_name[L3_MODULES_MAX_LEN];

	/*
	 * To avoid code running on other OMAPs in
	 * multi-omap builds
	 */
	if (!(cpu_is_omap34xx()))
		return -ENODEV;

<<<<<<< HEAD
	l = snprintf(oh_name, L3_MODULES_MAX_LEN, "l3_main");
=======
	snprintf(oh_name, L3_MODULES_MAX_LEN, "l3_main");
>>>>>>> refs/remotes/origin/master

	oh = omap_hwmod_lookup(oh_name);

	if (!oh)
		pr_err("could not look up %s\n", oh_name);

<<<<<<< HEAD
<<<<<<< HEAD
	od = omap_device_build("omap_l3_smx", 0, oh, NULL, 0,
							   NULL, 0, 0);

	WARN(IS_ERR(od), "could not build omap_device for %s\n", oh_name);

	return IS_ERR(od) ? PTR_ERR(od) : 0;
=======
	pdev = omap_device_build("omap_l3_smx", 0, oh, NULL, 0,
							   NULL, 0, 0);

	WARN(IS_ERR(pdev), "could not build omap_device for %s\n", oh_name);

	return IS_ERR(pdev) ? PTR_ERR(pdev) : 0;
>>>>>>> refs/remotes/origin/cm-10.0
}
postcore_initcall(omap3_l3_init);

static int __init omap4_l3_init(void)
{
	int l, i;
	struct omap_hwmod *oh[3];
<<<<<<< HEAD
	struct omap_device *od;
	char oh_name[L3_MODULES_MAX_LEN];

=======
=======
	pdev = omap_device_build("omap_l3_smx", 0, oh, NULL, 0);

	WARN(IS_ERR(pdev), "could not build omap_device for %s\n", oh_name);

	return PTR_RET(pdev);
}
omap_postcore_initcall(omap3_l3_init);

static int __init omap4_l3_init(void)
{
	int i;
	struct omap_hwmod *oh[3];
>>>>>>> refs/remotes/origin/master
	struct platform_device *pdev;
	char oh_name[L3_MODULES_MAX_LEN];

	/* If dtb is there, the devices will be created dynamically */
	if (of_have_populated_dt())
		return -ENODEV;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * To avoid code running on other OMAPs in
	 * multi-omap builds
	 */
<<<<<<< HEAD
	if (!(cpu_is_omap44xx()))
		return -ENODEV;

	for (i = 0; i < L3_MODULES; i++) {
		l = snprintf(oh_name, L3_MODULES_MAX_LEN, "l3_main_%d", i+1);
=======
	if (!cpu_is_omap44xx() && !soc_is_omap54xx())
		return -ENODEV;

	for (i = 0; i < L3_MODULES; i++) {
		snprintf(oh_name, L3_MODULES_MAX_LEN, "l3_main_%d", i+1);
>>>>>>> refs/remotes/origin/master

		oh[i] = omap_hwmod_lookup(oh_name);
		if (!(oh[i]))
			pr_err("could not look up %s\n", oh_name);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	od = omap_device_build_ss("omap_l3_noc", 0, oh, 3, NULL,
						     0, NULL, 0, 0);

	WARN(IS_ERR(od), "could not build omap_device for %s\n", oh_name);

	return IS_ERR(od) ? PTR_ERR(od) : 0;
=======
	pdev = omap_device_build_ss("omap_l3_noc", 0, oh, 3, NULL,
						     0, NULL, 0, 0);

	WARN(IS_ERR(pdev), "could not build omap_device for %s\n", oh_name);

	return IS_ERR(pdev) ? PTR_ERR(pdev) : 0;
>>>>>>> refs/remotes/origin/cm-10.0
}
postcore_initcall(omap4_l3_init);
=======
	pdev = omap_device_build_ss("omap_l3_noc", 0, oh, 3, NULL, 0);

	WARN(IS_ERR(pdev), "could not build omap_device for %s\n", oh_name);

	return PTR_RET(pdev);
}
omap_postcore_initcall(omap4_l3_init);
>>>>>>> refs/remotes/origin/master

#if defined(CONFIG_VIDEO_OMAP2) || defined(CONFIG_VIDEO_OMAP2_MODULE)

static struct resource omap2cam_resources[] = {
	{
		.start		= OMAP24XX_CAMERA_BASE,
		.end		= OMAP24XX_CAMERA_BASE + 0xfff,
		.flags		= IORESOURCE_MEM,
	},
	{
<<<<<<< HEAD
		.start		= INT_24XX_CAM_IRQ,
=======
		.start		= 24 + OMAP_INTC_START,
>>>>>>> refs/remotes/origin/master
		.flags		= IORESOURCE_IRQ,
	}
};

static struct platform_device omap2cam_device = {
	.name		= "omap24xxcam",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(omap2cam_resources),
	.resource	= omap2cam_resources,
};
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
#if defined(CONFIG_IOMMU_API)

#include <plat/iommu.h>

>>>>>>> refs/remotes/origin/cm-10.0
=======
#if defined(CONFIG_IOMMU_API)

#include <linux/platform_data/iommu-omap.h>

>>>>>>> refs/remotes/origin/master
static struct resource omap3isp_resources[] = {
	{
		.start		= OMAP3430_ISP_BASE,
		.end		= OMAP3430_ISP_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_CCP2_BASE,
		.end		= OMAP3430_ISP_CCP2_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_CCDC_BASE,
		.end		= OMAP3430_ISP_CCDC_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_HIST_BASE,
		.end		= OMAP3430_ISP_HIST_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_H3A_BASE,
		.end		= OMAP3430_ISP_H3A_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_PREV_BASE,
		.end		= OMAP3430_ISP_PREV_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_RESZ_BASE,
		.end		= OMAP3430_ISP_RESZ_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_SBL_BASE,
		.end		= OMAP3430_ISP_SBL_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_CSI2A_REGS1_BASE,
		.end		= OMAP3430_ISP_CSI2A_REGS1_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_CSIPHY2_BASE,
		.end		= OMAP3430_ISP_CSIPHY2_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3630_ISP_CSI2A_REGS2_BASE,
		.end		= OMAP3630_ISP_CSI2A_REGS2_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3630_ISP_CSI2C_REGS1_BASE,
		.end		= OMAP3630_ISP_CSI2C_REGS1_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3630_ISP_CSIPHY1_BASE,
		.end		= OMAP3630_ISP_CSIPHY1_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3630_ISP_CSI2C_REGS2_BASE,
		.end		= OMAP3630_ISP_CSI2C_REGS2_END,
		.flags		= IORESOURCE_MEM,
	},
	{
<<<<<<< HEAD
		.start		= INT_34XX_CAM_IRQ,
=======
		.start		= OMAP343X_CTRL_BASE + OMAP343X_CONTROL_CSIRXFE,
		.end		= OMAP343X_CTRL_BASE + OMAP343X_CONTROL_CSIRXFE + 3,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP343X_CTRL_BASE + OMAP3630_CONTROL_CAMERA_PHY_CTRL,
		.end		= OMAP343X_CTRL_BASE + OMAP3630_CONTROL_CAMERA_PHY_CTRL + 3,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= 24 + OMAP_INTC_START,
>>>>>>> refs/remotes/origin/master
		.flags		= IORESOURCE_IRQ,
	}
};

static struct platform_device omap3isp_device = {
	.name		= "omap3isp",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(omap3isp_resources),
	.resource	= omap3isp_resources,
};

<<<<<<< HEAD
<<<<<<< HEAD
int omap3_init_camera(struct isp_platform_data *pdata)
{
	omap3isp_device.dev.platform_data = pdata;
	return platform_device_register(&omap3isp_device);
}

=======
static struct omap_iommu_arch_data omap3_isp_iommu = {
	.name = "isp",
=======
static struct omap_iommu_arch_data omap3_isp_iommu = {
	.name = "mmu_isp",
>>>>>>> refs/remotes/origin/master
};

int omap3_init_camera(struct isp_platform_data *pdata)
{
	omap3isp_device.dev.platform_data = pdata;
	omap3isp_device.dev.archdata.iommu = &omap3_isp_iommu;

	return platform_device_register(&omap3isp_device);
}

#else /* !CONFIG_IOMMU_API */

int omap3_init_camera(struct isp_platform_data *pdata)
{
	return 0;
}

#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static inline void omap_init_camera(void)
{
#if defined(CONFIG_VIDEO_OMAP2) || defined(CONFIG_VIDEO_OMAP2_MODULE)
	if (cpu_is_omap24xx())
		platform_device_register(&omap2cam_device);
#endif
}

<<<<<<< HEAD
<<<<<<< HEAD
struct omap_device_pm_latency omap_keyboard_latency[] = {
	{
		.deactivate_func = omap_device_idle_hwmods,
		.activate_func   = omap_device_enable_hwmods,
		.flags = OMAP_DEVICE_LATENCY_AUTO_ADJUST,
	},
};

int __init omap4_keyboard_init(struct omap4_keypad_platform_data
						*sdp4430_keypad_data)
{
	struct omap_device *od;
=======
=======
>>>>>>> refs/remotes/origin/master
int __init omap4_keyboard_init(struct omap4_keypad_platform_data
			*sdp4430_keypad_data, struct omap_board_data *bdata)
{
	struct platform_device *pdev;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct omap_hwmod *oh;
	struct omap4_keypad_platform_data *keypad_data;
	unsigned int id = -1;
	char *oh_name = "kbd";
	char *name = "omap4-keypad";

	oh = omap_hwmod_lookup(oh_name);
	if (!oh) {
		pr_err("Could not look up %s\n", oh_name);
		return -ENODEV;
	}

	keypad_data = sdp4430_keypad_data;

<<<<<<< HEAD
<<<<<<< HEAD
	od = omap_device_build(name, id, oh, keypad_data,
			sizeof(struct omap4_keypad_platform_data),
			omap_keyboard_latency,
			ARRAY_SIZE(omap_keyboard_latency), 0);

	if (IS_ERR(od)) {
		WARN(1, "Can't build omap_device for %s:%s.\n",
						name, oh->name);
		return PTR_ERR(od);
	}
=======
	pdev = omap_device_build(name, id, oh, keypad_data,
			sizeof(struct omap4_keypad_platform_data), NULL, 0, 0);
=======
	pdev = omap_device_build(name, id, oh, keypad_data,
				 sizeof(struct omap4_keypad_platform_data));
>>>>>>> refs/remotes/origin/master

	if (IS_ERR(pdev)) {
		WARN(1, "Can't build omap_device for %s:%s.\n",
						name, oh->name);
		return PTR_ERR(pdev);
	}
	oh->mux = omap_hwmod_mux_init(bdata->pads, bdata->pads_cnt);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
#if defined(CONFIG_OMAP_MBOX_FWK) || defined(CONFIG_OMAP_MBOX_FWK_MODULE)
<<<<<<< HEAD
static struct omap_device_pm_latency mbox_latencies[] = {
	[0] = {
		.activate_func = omap_device_enable_hwmods,
		.deactivate_func = omap_device_idle_hwmods,
		.flags = OMAP_DEVICE_LATENCY_AUTO_ADJUST,
	},
};

static inline void omap_init_mbox(void)
{
	struct omap_hwmod *oh;
	struct omap_device *od;
=======
=======
#if defined(CONFIG_OMAP2PLUS_MBOX) || defined(CONFIG_OMAP2PLUS_MBOX_MODULE)
>>>>>>> refs/remotes/origin/master
static inline void __init omap_init_mbox(void)
{
	struct omap_hwmod *oh;
	struct platform_device *pdev;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct omap_mbox_pdata *pdata;
>>>>>>> refs/remotes/origin/master

	oh = omap_hwmod_lookup("mailbox");
	if (!oh) {
		pr_err("%s: unable to find hwmod\n", __func__);
		return;
	}
<<<<<<< HEAD

<<<<<<< HEAD
	od = omap_device_build("omap-mailbox", -1, oh, NULL, 0,
				mbox_latencies, ARRAY_SIZE(mbox_latencies), 0);
	WARN(IS_ERR(od), "%s: could not build device, err %ld\n",
						__func__, PTR_ERR(od));
=======
	pdev = omap_device_build("omap-mailbox", -1, oh, NULL, 0, NULL, 0, 0);
	WARN(IS_ERR(pdev), "%s: could not build device, err %ld\n",
						__func__, PTR_ERR(pdev));
>>>>>>> refs/remotes/origin/cm-10.0
}
#else
static inline void omap_init_mbox(void) { }
#endif /* CONFIG_OMAP_MBOX_FWK */

static inline void omap_init_sti(void) {}

#if defined CONFIG_ARCH_OMAP4

static struct platform_device codec_dmic0 = {
	.name	= "dmic-codec",
	.id	= 0,
};

static struct platform_device codec_dmic1 = {
	.name	= "dmic-codec",
	.id	= 1,
};

static struct platform_device codec_dmic2 = {
	.name	= "dmic-codec",
	.id	= 2,
};

static struct platform_device omap_abe_dai = {
	.name	= "omap-abe-dai",
	.id	= -1,
};

static inline void omap_init_abe(void)
{
	platform_device_register(&codec_dmic0);
	platform_device_register(&codec_dmic1);
	platform_device_register(&codec_dmic2);
	platform_device_register(&omap_abe_dai);
}
#else
static inline void omap_init_abe(void) {}
#endif

<<<<<<< HEAD
=======
	if (!oh->dev_attr) {
		pr_err("%s: hwmod doesn't have valid attrs\n", __func__);
		return;
	}

	pdata = (struct omap_mbox_pdata *)oh->dev_attr;
	pdev = omap_device_build("omap-mailbox", -1, oh, pdata, sizeof(*pdata));
	WARN(IS_ERR(pdev), "%s: could not build device, err %ld\n",
						__func__, PTR_ERR(pdev));
}
#else
static inline void omap_init_mbox(void) { }
#endif /* CONFIG_OMAP2PLUS_MBOX */

static inline void omap_init_sti(void) {}

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
#if defined(CONFIG_SND_SOC) || defined(CONFIG_SND_SOC_MODULE)

static struct platform_device omap_pcm = {
	.name	= "omap-pcm-audio",
	.id	= -1,
};

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * OMAP2420 has 2 McBSP ports
 * OMAP2430 has 5 McBSP ports
 * OMAP3 has 5 McBSP ports
 * OMAP4 has 4 McBSP ports
 */
OMAP_MCBSP_PLATFORM_DEVICE(1);
OMAP_MCBSP_PLATFORM_DEVICE(2);
OMAP_MCBSP_PLATFORM_DEVICE(3);
OMAP_MCBSP_PLATFORM_DEVICE(4);
OMAP_MCBSP_PLATFORM_DEVICE(5);

static void omap_init_audio(void)
{
	platform_device_register(&omap_mcbsp1);
	platform_device_register(&omap_mcbsp2);
	if (cpu_is_omap243x() || cpu_is_omap34xx() || cpu_is_omap44xx()) {
		platform_device_register(&omap_mcbsp3);
		platform_device_register(&omap_mcbsp4);
	}
	if (cpu_is_omap243x() || cpu_is_omap34xx())
		platform_device_register(&omap_mcbsp5);

=======
static void omap_init_audio(void)
{
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void omap_init_audio(void)
{
>>>>>>> refs/remotes/origin/master
	platform_device_register(&omap_pcm);
}

#else
static inline void omap_init_audio(void) {}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
#if defined(CONFIG_SND_OMAP_SOC_MCPDM) || \
		defined(CONFIG_SND_OMAP_SOC_MCPDM_MODULE)

static void __init omap_init_mcpdm(void)
{
	struct omap_hwmod *oh;
	struct platform_device *pdev;

	oh = omap_hwmod_lookup("mcpdm");
	if (!oh) {
		printk(KERN_ERR "Could not look up mcpdm hw_mod\n");
		return;
	}

	pdev = omap_device_build("omap-mcpdm", -1, oh, NULL, 0, NULL, 0, 0);
	WARN(IS_ERR(pdev), "Can't build omap_device for omap-mcpdm.\n");
}
#else
static inline void omap_init_mcpdm(void) {}
#endif

#if defined(CONFIG_SND_OMAP_SOC_DMIC) || \
		defined(CONFIG_SND_OMAP_SOC_DMIC_MODULE)

static void __init omap_init_dmic(void)
=======
#if defined(CONFIG_SND_OMAP_SOC_OMAP_HDMI) || \
		defined(CONFIG_SND_OMAP_SOC_OMAP_HDMI_MODULE)

static struct platform_device omap_hdmi_audio = {
	.name	= "omap-hdmi-audio",
	.id	= -1,
};

static void __init omap_init_hdmi_audio(void)
>>>>>>> refs/remotes/origin/master
{
	struct omap_hwmod *oh;
	struct platform_device *pdev;

<<<<<<< HEAD
	oh = omap_hwmod_lookup("dmic");
	if (!oh) {
		printk(KERN_ERR "Could not look up mcpdm hw_mod\n");
		return;
	}

	pdev = omap_device_build("omap-dmic", -1, oh, NULL, 0, NULL, 0, 0);
	WARN(IS_ERR(pdev), "Can't build omap_device for omap-dmic.\n");
}
#else
static inline void omap_init_dmic(void) {}
#endif

>>>>>>> refs/remotes/origin/cm-10.0
#if defined(CONFIG_SPI_OMAP24XX) || defined(CONFIG_SPI_OMAP24XX_MODULE)

#include <plat/mcspi.h>

<<<<<<< HEAD
struct omap_device_pm_latency omap_mcspi_latency[] = {
	[0] = {
		.deactivate_func = omap_device_idle_hwmods,
		.activate_func   = omap_device_enable_hwmods,
		.flags		 = OMAP_DEVICE_LATENCY_AUTO_ADJUST,
	},
};

static int omap_mcspi_init(struct omap_hwmod *oh, void *unused)
{
	struct omap_device *od;
=======
static int __init omap_mcspi_init(struct omap_hwmod *oh, void *unused)
{
	struct platform_device *pdev;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	oh = omap_hwmod_lookup("dss_hdmi");
	if (!oh)
		return;

	pdev = omap_device_build("omap-hdmi-audio-dai", -1, oh, NULL, 0);
	WARN(IS_ERR(pdev),
	     "Can't build omap_device for omap-hdmi-audio-dai.\n");

	platform_device_register(&omap_hdmi_audio);
}
#else
static inline void omap_init_hdmi_audio(void) {}
#endif

#if defined(CONFIG_SPI_OMAP24XX) || defined(CONFIG_SPI_OMAP24XX_MODULE)

#include <linux/platform_data/spi-omap2-mcspi.h>

static int __init omap_mcspi_init(struct omap_hwmod *oh, void *unused)
{
	struct platform_device *pdev;
>>>>>>> refs/remotes/origin/master
	char *name = "omap2_mcspi";
	struct omap2_mcspi_platform_config *pdata;
	static int spi_num;
	struct omap2_mcspi_dev_attr *mcspi_attrib = oh->dev_attr;

	pdata = kzalloc(sizeof(*pdata), GFP_KERNEL);
	if (!pdata) {
		pr_err("Memory allocation for McSPI device failed\n");
		return -ENOMEM;
	}

	pdata->num_cs = mcspi_attrib->num_chipselect;
	switch (oh->class->rev) {
	case OMAP2_MCSPI_REV:
	case OMAP3_MCSPI_REV:
			pdata->regs_offset = 0;
			break;
	case OMAP4_MCSPI_REV:
			pdata->regs_offset = OMAP4_MCSPI_REG_OFFSET;
			break;
	default:
			pr_err("Invalid McSPI Revision value\n");
<<<<<<< HEAD
<<<<<<< HEAD
=======
			kfree(pdata);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			kfree(pdata);
>>>>>>> refs/remotes/origin/master
			return -EINVAL;
	}

	spi_num++;
<<<<<<< HEAD
<<<<<<< HEAD
	od = omap_device_build(name, spi_num, oh, pdata,
				sizeof(*pdata),	omap_mcspi_latency,
				ARRAY_SIZE(omap_mcspi_latency), 0);
	WARN(IS_ERR(od), "Can't build omap_device for %s:%s\n",
=======
	pdev = omap_device_build(name, spi_num, oh, pdata,
				sizeof(*pdata),	NULL, 0, 0);
	WARN(IS_ERR(pdev), "Can't build omap_device for %s:%s\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pdev = omap_device_build(name, spi_num, oh, pdata, sizeof(*pdata));
	WARN(IS_ERR(pdev), "Can't build omap_device for %s:%s\n",
>>>>>>> refs/remotes/origin/master
				name, oh->name);
	kfree(pdata);
	return 0;
}

static void omap_init_mcspi(void)
{
	omap_hwmod_for_each_by_class("mcspi", omap_mcspi_init, NULL);
}

#else
static inline void omap_init_mcspi(void) {}
#endif

<<<<<<< HEAD
static struct resource omap2_pmu_resource = {
	.start	= 3,
	.end	= 3,
	.flags	= IORESOURCE_IRQ,
};

static struct resource omap3_pmu_resource = {
	.start	= INT_34XX_BENCH_MPU_EMUL,
	.end	= INT_34XX_BENCH_MPU_EMUL,
	.flags	= IORESOURCE_IRQ,
};

static struct platform_device omap_pmu_device = {
	.name		= "arm-pmu",
	.id		= ARM_PMU_DEVICE_CPU,
	.num_resources	= 1,
};

static void omap_init_pmu(void)
{
	if (cpu_is_omap24xx())
		omap_pmu_device.resource = &omap2_pmu_resource;
	else if (cpu_is_omap34xx())
		omap_pmu_device.resource = &omap3_pmu_resource;
	else
		return;

	platform_device_register(&omap_pmu_device);
}


#if defined(CONFIG_CRYPTO_DEV_OMAP_SHAM) || defined(CONFIG_CRYPTO_DEV_OMAP_SHAM_MODULE)

#ifdef CONFIG_ARCH_OMAP2
static struct resource omap2_sham_resources[] = {
	{
		.start	= OMAP24XX_SEC_SHA1MD5_BASE,
		.end	= OMAP24XX_SEC_SHA1MD5_BASE + 0x64,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_24XX_SHA1MD5,
		.flags	= IORESOURCE_IRQ,
	}
};
static int omap2_sham_resources_sz = ARRAY_SIZE(omap2_sham_resources);
#else
#define omap2_sham_resources		NULL
#define omap2_sham_resources_sz		0
#endif

#ifdef CONFIG_ARCH_OMAP3
static struct resource omap3_sham_resources[] = {
	{
		.start	= OMAP34XX_SEC_SHA1MD5_BASE,
		.end	= OMAP34XX_SEC_SHA1MD5_BASE + 0x64,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_34XX_SHA1MD52_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= OMAP34XX_DMA_SHA1MD5_RX,
		.flags	= IORESOURCE_DMA,
	}
};
static int omap3_sham_resources_sz = ARRAY_SIZE(omap3_sham_resources);
#else
#define omap3_sham_resources		NULL
#define omap3_sham_resources_sz		0
#endif

static struct platform_device sham_device = {
	.name		= "omap-sham",
	.id		= -1,
};

static void omap_init_sham(void)
{
	if (cpu_is_omap24xx()) {
		sham_device.resource = omap2_sham_resources;
		sham_device.num_resources = omap2_sham_resources_sz;
	} else if (cpu_is_omap34xx()) {
		sham_device.resource = omap3_sham_resources;
		sham_device.num_resources = omap3_sham_resources_sz;
	} else {
		pr_err("%s: platform not supported\n", __func__);
		return;
	}
	platform_device_register(&sham_device);
}
#else
static inline void omap_init_sham(void) { }
#endif

#if defined(CONFIG_CRYPTO_DEV_OMAP_AES) || defined(CONFIG_CRYPTO_DEV_OMAP_AES_MODULE)

#ifdef CONFIG_ARCH_OMAP2
static struct resource omap2_aes_resources[] = {
	{
		.start	= OMAP24XX_SEC_AES_BASE,
		.end	= OMAP24XX_SEC_AES_BASE + 0x4C,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= OMAP24XX_DMA_AES_TX,
		.flags	= IORESOURCE_DMA,
	},
	{
		.start	= OMAP24XX_DMA_AES_RX,
		.flags	= IORESOURCE_DMA,
	}
};
static int omap2_aes_resources_sz = ARRAY_SIZE(omap2_aes_resources);
#else
#define omap2_aes_resources		NULL
#define omap2_aes_resources_sz		0
#endif

#ifdef CONFIG_ARCH_OMAP3
static struct resource omap3_aes_resources[] = {
	{
		.start	= OMAP34XX_SEC_AES_BASE,
		.end	= OMAP34XX_SEC_AES_BASE + 0x4C,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= OMAP34XX_DMA_AES2_TX,
		.flags	= IORESOURCE_DMA,
	},
	{
		.start	= OMAP34XX_DMA_AES2_RX,
		.flags	= IORESOURCE_DMA,
	}
};
static int omap3_aes_resources_sz = ARRAY_SIZE(omap3_aes_resources);
#else
#define omap3_aes_resources		NULL
#define omap3_aes_resources_sz		0
#endif

static struct platform_device aes_device = {
	.name		= "omap-aes",
	.id		= -1,
};

static void omap_init_aes(void)
{
	if (cpu_is_omap24xx()) {
		aes_device.resource = omap2_aes_resources;
		aes_device.num_resources = omap2_aes_resources_sz;
	} else if (cpu_is_omap34xx()) {
		aes_device.resource = omap3_aes_resources;
		aes_device.num_resources = omap3_aes_resources_sz;
	} else {
		pr_err("%s: platform not supported\n", __func__);
		return;
	}
	platform_device_register(&aes_device);
}

#else
static inline void omap_init_aes(void) { }
#endif

/*-------------------------------------------------------------------------*/

#if defined(CONFIG_MMC_OMAP) || defined(CONFIG_MMC_OMAP_MODULE)

static inline void omap242x_mmc_mux(struct omap_mmc_platform_data
							*mmc_controller)
{
	if ((mmc_controller->slots[0].switch_pin > 0) && \
		(mmc_controller->slots[0].switch_pin < OMAP_MAX_GPIO_LINES))
		omap_mux_init_gpio(mmc_controller->slots[0].switch_pin,
					OMAP_PIN_INPUT_PULLUP);
	if ((mmc_controller->slots[0].gpio_wp > 0) && \
		(mmc_controller->slots[0].gpio_wp < OMAP_MAX_GPIO_LINES))
		omap_mux_init_gpio(mmc_controller->slots[0].gpio_wp,
					OMAP_PIN_INPUT_PULLUP);

	omap_mux_init_signal("sdmmc_cmd", 0);
	omap_mux_init_signal("sdmmc_clki", 0);
	omap_mux_init_signal("sdmmc_clko", 0);
	omap_mux_init_signal("sdmmc_dat0", 0);
	omap_mux_init_signal("sdmmc_dat_dir0", 0);
	omap_mux_init_signal("sdmmc_cmd_dir", 0);
	if (mmc_controller->slots[0].caps & MMC_CAP_4_BIT_DATA) {
		omap_mux_init_signal("sdmmc_dat1", 0);
		omap_mux_init_signal("sdmmc_dat2", 0);
		omap_mux_init_signal("sdmmc_dat3", 0);
		omap_mux_init_signal("sdmmc_dat_dir1", 0);
		omap_mux_init_signal("sdmmc_dat_dir2", 0);
		omap_mux_init_signal("sdmmc_dat_dir3", 0);
	}

	/*
	 * Use internal loop-back in MMC/SDIO Module Input Clock
	 * selection
	 */
	if (mmc_controller->slots[0].internal_clock) {
		u32 v = omap_ctrl_readl(OMAP2_CONTROL_DEVCONF0);
		v |= (1 << 24);
		omap_ctrl_writel(v, OMAP2_CONTROL_DEVCONF0);
	}
}

void __init omap242x_init_mmc(struct omap_mmc_platform_data **mmc_data)
{
	char *name = "mmci-omap";

	if (!mmc_data[0]) {
		pr_err("%s fails: Incomplete platform data\n", __func__);
		return;
	}

	omap242x_mmc_mux(mmc_data[0]);
	omap_mmc_add(name, 0, OMAP2_MMC1_BASE, OMAP2420_MMC_SIZE,
					INT_24XX_MMC_IRQ, mmc_data[0]);
}

#endif

/*-------------------------------------------------------------------------*/

#if defined(CONFIG_HDQ_MASTER_OMAP) || defined(CONFIG_HDQ_MASTER_OMAP_MODULE)
<<<<<<< HEAD
#if defined(CONFIG_SOC_OMAP2430) || defined(CONFIG_SOC_OMAP3430)
#define OMAP_HDQ_BASE	0x480B2000
#endif
=======
#define OMAP_HDQ_BASE	0x480B2000
>>>>>>> refs/remotes/origin/cm-10.0
static struct resource omap_hdq_resources[] = {
	{
		.start		= OMAP_HDQ_BASE,
		.end		= OMAP_HDQ_BASE + 0x1C,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= INT_24XX_HDQ_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};
static struct platform_device omap_hdq_dev = {
	.name = "omap_hdq",
	.id = 0,
	.dev = {
		.platform_data = NULL,
	},
	.num_resources	= ARRAY_SIZE(omap_hdq_resources),
	.resource	= omap_hdq_resources,
};
static inline void omap_hdq_init(void)
{
<<<<<<< HEAD
	(void) platform_device_register(&omap_hdq_dev);
=======
	if (cpu_is_omap2420())
		return;

	platform_device_register(&omap_hdq_dev);
>>>>>>> refs/remotes/origin/cm-10.0
}
#else
static inline void omap_hdq_init(void) {}
#endif

/*---------------------------------------------------------------------------*/

=======
/**
 * omap_init_rng - bind the RNG hwmod to the RNG omap_device
 *
 * Bind the RNG hwmod to the RNG omap_device.  No return value.
 */
static void omap_init_rng(void)
{
	struct omap_hwmod *oh;
	struct platform_device *pdev;

	oh = omap_hwmod_lookup("rng");
	if (!oh)
		return;

	pdev = omap_device_build("omap_rng", -1, oh, NULL, 0);
	WARN(IS_ERR(pdev), "Can't build omap_device for omap_rng\n");
}

static void __init omap_init_sham(void)
{
	struct omap_hwmod *oh;
	struct platform_device *pdev;

	oh = omap_hwmod_lookup("sham");
	if (!oh)
		return;

	pdev = omap_device_build("omap-sham", -1, oh, NULL, 0);
	WARN(IS_ERR(pdev), "Can't build omap_device for omap-sham\n");
}

static void __init omap_init_aes(void)
{
	struct omap_hwmod *oh;
	struct platform_device *pdev;

	oh = omap_hwmod_lookup("aes");
	if (!oh)
		return;

	pdev = omap_device_build("omap-aes", -1, oh, NULL, 0);
	WARN(IS_ERR(pdev), "Can't build omap_device for omap-aes\n");
}

/*-------------------------------------------------------------------------*/

>>>>>>> refs/remotes/origin/master
#if defined(CONFIG_VIDEO_OMAP2_VOUT) || \
	defined(CONFIG_VIDEO_OMAP2_VOUT_MODULE)
#if defined(CONFIG_FB_OMAP2) || defined(CONFIG_FB_OMAP2_MODULE)
static struct resource omap_vout_resource[3 - CONFIG_FB_OMAP2_NUM_FBS] = {
};
#else
static struct resource omap_vout_resource[2] = {
};
#endif

static struct platform_device omap_vout_device = {
	.name		= "omap_vout",
	.num_resources	= ARRAY_SIZE(omap_vout_resource),
	.resource 	= &omap_vout_resource[0],
	.id		= -1,
};
<<<<<<< HEAD
static void omap_init_vout(void)
{
	if (platform_device_register(&omap_vout_device) < 0)
		printk(KERN_ERR "Unable to register OMAP-VOUT device\n");
}
#else
static inline void omap_init_vout(void) {}
=======

int __init omap_init_vout(void)
{
	return platform_device_register(&omap_vout_device);
}
#else
int __init omap_init_vout(void) { return 0; }
>>>>>>> refs/remotes/origin/master
#endif

/*-------------------------------------------------------------------------*/

static int __init omap2_init_devices(void)
{
<<<<<<< HEAD
=======
	/* Enable dummy states for those platforms without pinctrl support */
	if (!of_have_populated_dt())
		pinctrl_provide_dummies();

>>>>>>> refs/remotes/origin/master
	/*
	 * please keep these calls, and their implementations above,
	 * in alphabetical order so they're easier to sort through.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	omap_init_abe();
	omap_init_audio();
<<<<<<< HEAD
=======
	omap_init_mcpdm();
	omap_init_dmic();
>>>>>>> refs/remotes/origin/cm-10.0
	omap_init_camera();
	omap_init_mbox();
	omap_init_mcspi();
	omap_init_pmu();
	omap_hdq_init();
	omap_init_sti();
	omap_init_sham();
	omap_init_aes();
	omap_init_vout();

	return 0;
}
arch_initcall(omap2_init_devices);

#if defined(CONFIG_OMAP_WATCHDOG) || defined(CONFIG_OMAP_WATCHDOG_MODULE)
<<<<<<< HEAD
static struct omap_device_pm_latency omap_wdt_latency[] = {
	[0] = {
		.deactivate_func = omap_device_idle_hwmods,
		.activate_func   = omap_device_enable_hwmods,
		.flags		 = OMAP_DEVICE_LATENCY_AUTO_ADJUST,
	},
};

static int __init omap_init_wdt(void)
{
	int id = -1;
	struct omap_device *od;
=======
static int __init omap_init_wdt(void)
{
	int id = -1;
	struct platform_device *pdev;
>>>>>>> refs/remotes/origin/cm-10.0
	struct omap_hwmod *oh;
	char *oh_name = "wd_timer2";
	char *dev_name = "omap_wdt";

	if (!cpu_class_is_omap2())
		return 0;

	oh = omap_hwmod_lookup(oh_name);
	if (!oh) {
		pr_err("Could not look up wd_timer%d hwmod\n", id);
		return -EINVAL;
	}

<<<<<<< HEAD
	od = omap_device_build(dev_name, id, oh, NULL, 0,
				omap_wdt_latency,
				ARRAY_SIZE(omap_wdt_latency), 0);
	WARN(IS_ERR(od), "Can't build omap_device for %s:%s.\n",
=======
	pdev = omap_device_build(dev_name, id, oh, NULL, 0, NULL, 0, 0);
	WARN(IS_ERR(pdev), "Can't build omap_device for %s:%s.\n",
>>>>>>> refs/remotes/origin/cm-10.0
				dev_name, oh->name);
	return 0;
}
subsys_initcall(omap_init_wdt);
#endif
=======
	omap_init_audio();
	omap_init_camera();
	omap_init_hdmi_audio();
	omap_init_mbox();
	/* If dtb is there, the devices will be created dynamically */
	if (!of_have_populated_dt()) {
		omap_init_mcspi();
		omap_init_sham();
		omap_init_aes();
		omap_init_rng();
	}
	omap_init_sti();

	return 0;
}
omap_arch_initcall(omap2_init_devices);
>>>>>>> refs/remotes/origin/master
