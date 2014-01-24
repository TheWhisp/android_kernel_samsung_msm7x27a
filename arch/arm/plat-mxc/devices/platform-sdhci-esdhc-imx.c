/*
 * Copyright (C) 2010 Pengutronix, Wolfram Sang <w.sang@pengutronix.de>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.
 */

#include <mach/hardware.h>
#include <mach/devices-common.h>
#include <mach/esdhc.h>

<<<<<<< HEAD
#define imx_sdhci_esdhc_imx_data_entry_single(soc, _id, hwid) \
	{								\
=======
#define imx_sdhci_esdhc_imx_data_entry_single(soc, _devid, _id, hwid) \
	{								\
		.devid = _devid,					\
>>>>>>> refs/remotes/origin/cm-10.0
		.id = _id,						\
		.iobase = soc ## _ESDHC ## hwid ## _BASE_ADDR,	\
		.irq = soc ## _INT_ESDHC ## hwid,			\
	}

<<<<<<< HEAD
#define imx_sdhci_esdhc_imx_data_entry(soc, id, hwid)	\
	[id] = imx_sdhci_esdhc_imx_data_entry_single(soc, id, hwid)
=======
#define imx_sdhci_esdhc_imx_data_entry(soc, devid, id, hwid)	\
	[id] = imx_sdhci_esdhc_imx_data_entry_single(soc, devid, id, hwid)
>>>>>>> refs/remotes/origin/cm-10.0

#ifdef CONFIG_SOC_IMX25
const struct imx_sdhci_esdhc_imx_data
imx25_sdhci_esdhc_imx_data[] __initconst = {
#define imx25_sdhci_esdhc_imx_data_entry(_id, _hwid)			\
<<<<<<< HEAD
	imx_sdhci_esdhc_imx_data_entry(MX25, _id, _hwid)
=======
	imx_sdhci_esdhc_imx_data_entry(MX25, "sdhci-esdhc-imx25", _id, _hwid)
>>>>>>> refs/remotes/origin/cm-10.0
	imx25_sdhci_esdhc_imx_data_entry(0, 1),
	imx25_sdhci_esdhc_imx_data_entry(1, 2),
};
#endif /* ifdef CONFIG_SOC_IMX25 */

#ifdef CONFIG_SOC_IMX35
const struct imx_sdhci_esdhc_imx_data
imx35_sdhci_esdhc_imx_data[] __initconst = {
#define imx35_sdhci_esdhc_imx_data_entry(_id, _hwid)			\
<<<<<<< HEAD
	imx_sdhci_esdhc_imx_data_entry(MX35, _id, _hwid)
=======
	imx_sdhci_esdhc_imx_data_entry(MX35, "sdhci-esdhc-imx35", _id, _hwid)
>>>>>>> refs/remotes/origin/cm-10.0
	imx35_sdhci_esdhc_imx_data_entry(0, 1),
	imx35_sdhci_esdhc_imx_data_entry(1, 2),
	imx35_sdhci_esdhc_imx_data_entry(2, 3),
};
#endif /* ifdef CONFIG_SOC_IMX35 */

#ifdef CONFIG_SOC_IMX51
const struct imx_sdhci_esdhc_imx_data
imx51_sdhci_esdhc_imx_data[] __initconst = {
#define imx51_sdhci_esdhc_imx_data_entry(_id, _hwid)			\
<<<<<<< HEAD
	imx_sdhci_esdhc_imx_data_entry(MX51, _id, _hwid)
=======
	imx_sdhci_esdhc_imx_data_entry(MX51, "sdhci-esdhc-imx51", _id, _hwid)
>>>>>>> refs/remotes/origin/cm-10.0
	imx51_sdhci_esdhc_imx_data_entry(0, 1),
	imx51_sdhci_esdhc_imx_data_entry(1, 2),
	imx51_sdhci_esdhc_imx_data_entry(2, 3),
	imx51_sdhci_esdhc_imx_data_entry(3, 4),
};
#endif /* ifdef CONFIG_SOC_IMX51 */

#ifdef CONFIG_SOC_IMX53
const struct imx_sdhci_esdhc_imx_data
imx53_sdhci_esdhc_imx_data[] __initconst = {
#define imx53_sdhci_esdhc_imx_data_entry(_id, _hwid)			\
<<<<<<< HEAD
	imx_sdhci_esdhc_imx_data_entry(MX53, _id, _hwid)
=======
	imx_sdhci_esdhc_imx_data_entry(MX53, "sdhci-esdhc-imx53", _id, _hwid)
>>>>>>> refs/remotes/origin/cm-10.0
	imx53_sdhci_esdhc_imx_data_entry(0, 1),
	imx53_sdhci_esdhc_imx_data_entry(1, 2),
	imx53_sdhci_esdhc_imx_data_entry(2, 3),
	imx53_sdhci_esdhc_imx_data_entry(3, 4),
};
#endif /* ifdef CONFIG_SOC_IMX53 */

<<<<<<< HEAD
=======
static const struct esdhc_platform_data default_esdhc_pdata __initconst = {
	.wp_type = ESDHC_WP_NONE,
	.cd_type = ESDHC_CD_NONE,
};

>>>>>>> refs/remotes/origin/cm-10.0
struct platform_device *__init imx_add_sdhci_esdhc_imx(
		const struct imx_sdhci_esdhc_imx_data *data,
		const struct esdhc_platform_data *pdata)
{
	struct resource res[] = {
		{
			.start = data->iobase,
			.end = data->iobase + SZ_16K - 1,
			.flags = IORESOURCE_MEM,
		}, {
			.start = data->irq,
			.end = data->irq,
			.flags = IORESOURCE_IRQ,
		},
	};

<<<<<<< HEAD
	return imx_add_platform_device("sdhci-esdhc-imx", data->id, res,
=======
	/*
	 * If machine does not provide pdata, use the default one
	 * which means no WP/CD support
	 */
	if (!pdata)
		pdata = &default_esdhc_pdata;

	return imx_add_platform_device(data->devid, data->id, res,
>>>>>>> refs/remotes/origin/cm-10.0
			ARRAY_SIZE(res), pdata, sizeof(*pdata));
}
