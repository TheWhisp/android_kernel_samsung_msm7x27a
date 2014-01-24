<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (c) 2011, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/of_device.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/of_device.h>
>>>>>>> refs/remotes/origin/cm-11.0
#include <sound/core.h>
#include <sound/soc.h>
#include <sound/pcm.h>

static struct snd_pcm_ops msm_pcm_hostless_ops = {};

static struct snd_soc_platform_driver msm_soc_hostless_platform = {
	.ops		= &msm_pcm_hostless_ops,
};

static __devinit int msm_pcm_hostless_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (pdev->dev.of_node)
		dev_set_name(&pdev->dev, "%s", "msm-pcm-hostless");

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (pdev->dev.of_node)
		dev_set_name(&pdev->dev, "%s", "msm-pcm-hostless");

>>>>>>> refs/remotes/origin/cm-11.0
	pr_debug("%s: dev name %s\n", __func__, dev_name(&pdev->dev));
	return snd_soc_register_platform(&pdev->dev,
				   &msm_soc_hostless_platform);
}

static int msm_pcm_hostless_remove(struct platform_device *pdev)
{
	snd_soc_unregister_platform(&pdev->dev);
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static const struct of_device_id msm_pcm_hostless_dt_match[] = {
	{.compatible = "qcom,msm-pcm-hostless"},
	{}
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
static struct platform_driver msm_pcm_hostless_driver = {
	.driver = {
		.name = "msm-pcm-hostless",
		.owner = THIS_MODULE,
<<<<<<< HEAD
<<<<<<< HEAD
=======
		.of_match_table = msm_pcm_hostless_dt_match,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.of_match_table = msm_pcm_hostless_dt_match,
>>>>>>> refs/remotes/origin/cm-11.0
	},
	.probe = msm_pcm_hostless_probe,
	.remove = __devexit_p(msm_pcm_hostless_remove),
};

static int __init msm_soc_platform_init(void)
{
	return platform_driver_register(&msm_pcm_hostless_driver);
}
module_init(msm_soc_platform_init);

static void __exit msm_soc_platform_exit(void)
{
	platform_driver_unregister(&msm_pcm_hostless_driver);
}
module_exit(msm_soc_platform_exit);

MODULE_DESCRIPTION("Hostless platform driver");
MODULE_LICENSE("GPL v2");
