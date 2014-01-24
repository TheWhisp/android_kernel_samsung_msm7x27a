/*
 * Copyright (C) 2010 Pengutronix
 * Uwe Kleine-Koenig <u.kleine-koenig@pengutronix.de>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.
 */
<<<<<<< HEAD
=======
#include <linux/dma-mapping.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/sizes.h>
#include <mach/hardware.h>
#include <mach/devices-common.h>

<<<<<<< HEAD
#define imx_fec_data_entry_single(soc)					\
	{								\
=======
#define imx_fec_data_entry_single(soc, _devid)				\
	{								\
		.devid = _devid,					\
>>>>>>> refs/remotes/origin/cm-10.0
		.iobase = soc ## _FEC_BASE_ADDR,			\
		.irq = soc ## _INT_FEC,					\
	}

#ifdef CONFIG_SOC_IMX25
const struct imx_fec_data imx25_fec_data __initconst =
<<<<<<< HEAD
	imx_fec_data_entry_single(MX25);
=======
	imx_fec_data_entry_single(MX25, "imx25-fec");
>>>>>>> refs/remotes/origin/cm-10.0
#endif /* ifdef CONFIG_SOC_IMX25 */

#ifdef CONFIG_SOC_IMX27
const struct imx_fec_data imx27_fec_data __initconst =
<<<<<<< HEAD
	imx_fec_data_entry_single(MX27);
#endif /* ifdef CONFIG_SOC_IMX27 */

#ifdef CONFIG_SOC_IMX35
const struct imx_fec_data imx35_fec_data __initconst =
	imx_fec_data_entry_single(MX35);
#endif

#ifdef CONFIG_SOC_IMX50
const struct imx_fec_data imx50_fec_data __initconst =
	imx_fec_data_entry_single(MX50);
#endif

#ifdef CONFIG_SOC_IMX51
const struct imx_fec_data imx51_fec_data __initconst =
	imx_fec_data_entry_single(MX51);
#endif

#ifdef CONFIG_SOC_IMX53
const struct imx_fec_data imx53_fec_data __initconst =
	imx_fec_data_entry_single(MX53);
=======
	imx_fec_data_entry_single(MX27, "imx27-fec");
#endif /* ifdef CONFIG_SOC_IMX27 */

#ifdef CONFIG_SOC_IMX35
/* i.mx35 has the i.mx27 type fec */
const struct imx_fec_data imx35_fec_data __initconst =
	imx_fec_data_entry_single(MX35, "imx27-fec");
#endif

#ifdef CONFIG_SOC_IMX50
/* i.mx50 has the i.mx25 type fec */
const struct imx_fec_data imx50_fec_data __initconst =
	imx_fec_data_entry_single(MX50, "imx25-fec");
#endif

#ifdef CONFIG_SOC_IMX51
/* i.mx51 has the i.mx27 type fec */
const struct imx_fec_data imx51_fec_data __initconst =
	imx_fec_data_entry_single(MX51, "imx27-fec");
#endif

#ifdef CONFIG_SOC_IMX53
/* i.mx53 has the i.mx25 type fec */
const struct imx_fec_data imx53_fec_data __initconst =
	imx_fec_data_entry_single(MX53, "imx25-fec");
>>>>>>> refs/remotes/origin/cm-10.0
#endif

struct platform_device *__init imx_add_fec(
		const struct imx_fec_data *data,
		const struct fec_platform_data *pdata)
{
	struct resource res[] = {
		{
			.start = data->iobase,
			.end = data->iobase + SZ_4K - 1,
			.flags = IORESOURCE_MEM,
		}, {
			.start = data->irq,
			.end = data->irq,
			.flags = IORESOURCE_IRQ,
		},
	};

<<<<<<< HEAD
	return imx_add_platform_device_dmamask("fec", 0,
=======
	return imx_add_platform_device_dmamask(data->devid, 0,
>>>>>>> refs/remotes/origin/cm-10.0
			res, ARRAY_SIZE(res),
			pdata, sizeof(*pdata), DMA_BIT_MASK(32));
}
