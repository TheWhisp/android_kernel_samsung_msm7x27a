/*
 * Generic Platform Camera Driver Header
 *
 * Copyright (C) 2008 Magnus Damm
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __SOC_CAMERA_H__
#define __SOC_CAMERA_H__

#include <linux/videodev2.h>
#include <media/soc_camera.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <media/v4l2-mediabus.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <media/v4l2-mediabus.h>
>>>>>>> refs/remotes/origin/master

struct device;

struct soc_camera_platform_info {
	const char *format_name;
	unsigned long format_depth;
	struct v4l2_mbus_framefmt format;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long bus_param;
	struct device *dev;
=======
	unsigned long mbus_param;
	enum v4l2_mbus_type mbus_type;
	struct soc_camera_device *icd;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned long mbus_param;
	enum v4l2_mbus_type mbus_type;
	struct soc_camera_device *icd;
>>>>>>> refs/remotes/origin/master
	int (*set_capture)(struct soc_camera_platform_info *info, int enable);
};

static inline void soc_camera_platform_release(struct platform_device **pdev)
{
	*pdev = NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int soc_camera_platform_add(const struct soc_camera_link *icl,
					  struct device *dev,
=======
static inline int soc_camera_platform_add(struct soc_camera_device *icd,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline int soc_camera_platform_add(struct soc_camera_device *icd,
>>>>>>> refs/remotes/origin/master
					  struct platform_device **pdev,
					  struct soc_camera_link *plink,
					  void (*release)(struct device *dev),
					  int id)
{
<<<<<<< HEAD
	struct soc_camera_platform_info *info = plink->priv;
	int ret;

<<<<<<< HEAD
	if (icl != plink)
=======
	if (icd->link != plink)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct soc_camera_subdev_desc *ssdd =
		(struct soc_camera_subdev_desc *)plink;
	struct soc_camera_platform_info *info = ssdd->drv_priv;
	int ret;

	if (&icd->sdesc->subdev_desc != ssdd)
>>>>>>> refs/remotes/origin/master
		return -ENODEV;

	if (*pdev)
		return -EBUSY;

	*pdev = platform_device_alloc("soc_camera_platform", id);
	if (!*pdev)
		return -ENOMEM;

<<<<<<< HEAD
<<<<<<< HEAD
	info->dev = dev;
=======
	info->icd = icd;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	info->icd = icd;
>>>>>>> refs/remotes/origin/master

	(*pdev)->dev.platform_data = info;
	(*pdev)->dev.release = release;

	ret = platform_device_add(*pdev);
	if (ret < 0) {
		platform_device_put(*pdev);
		*pdev = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
		info->dev = NULL;
=======
		info->icd = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		info->icd = NULL;
>>>>>>> refs/remotes/origin/master
	}

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void soc_camera_platform_del(const struct soc_camera_link *icl,
					   struct platform_device *pdev,
					   const struct soc_camera_link *plink)
{
	if (icl != plink || !pdev)
=======
=======
>>>>>>> refs/remotes/origin/master
static inline void soc_camera_platform_del(const struct soc_camera_device *icd,
					   struct platform_device *pdev,
					   const struct soc_camera_link *plink)
{
<<<<<<< HEAD
	if (icd->link != plink || !pdev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const struct soc_camera_subdev_desc *ssdd =
		(const struct soc_camera_subdev_desc *)plink;
	if (&icd->sdesc->subdev_desc != ssdd || !pdev)
>>>>>>> refs/remotes/origin/master
		return;

	platform_device_unregister(pdev);
}

#endif /* __SOC_CAMERA_H__ */
