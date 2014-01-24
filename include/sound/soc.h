/*
 * linux/sound/soc.h -- ALSA SoC Layer
 *
 * Author:		Liam Girdwood
 * Created:		Aug 11th 2005
 * Copyright:	Wolfson Microelectronics. PLC.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __LINUX_SND_SOC_H
#define __LINUX_SND_SOC_H

<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> refs/remotes/origin/master
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/notifier.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/regmap.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <sound/core.h>
#include <sound/pcm.h>
=======
#include <linux/regmap.h>
#include <linux/log2.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/compress_driver.h>
>>>>>>> refs/remotes/origin/master
#include <sound/control.h>
#include <sound/ac97_codec.h>

/*
 * Convenience kcontrol builders
 */
<<<<<<< HEAD
<<<<<<< HEAD
#define SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .shift = xshift, .rshift = xshift, .max = xmax, \
	.platform_max = xmax, .invert = xinvert})
#define SOC_SINGLE_VALUE_EXT(xreg, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .max = xmax, .platform_max = xmax, .invert = xinvert})
=======
#define SOC_DOUBLE_VALUE(xreg, shift_left, shift_right, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .rreg = xreg, .shift = shift_left, \
	.rshift = shift_right, .max = xmax, .platform_max = xmax, \
	.invert = xinvert})
#define SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert) \
	SOC_DOUBLE_VALUE(xreg, xshift, xshift, xmax, xinvert)
=======
#define SOC_DOUBLE_VALUE(xreg, shift_left, shift_right, xmax, xinvert, xautodisable) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .rreg = xreg, .shift = shift_left, \
	.rshift = shift_right, .max = xmax, .platform_max = xmax, \
	.invert = xinvert, .autodisable = xautodisable})
#define SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert, xautodisable) \
	SOC_DOUBLE_VALUE(xreg, xshift, xshift, xmax, xinvert, xautodisable)
>>>>>>> refs/remotes/origin/master
#define SOC_SINGLE_VALUE_EXT(xreg, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .max = xmax, .platform_max = xmax, .invert = xinvert})
#define SOC_DOUBLE_R_VALUE(xlreg, xrreg, xshift, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xlreg, .rreg = xrreg, .shift = xshift, .rshift = xshift, \
	.max = xmax, .platform_max = xmax, .invert = xinvert})
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define SOC_DOUBLE_R_RANGE_VALUE(xlreg, xrreg, xshift, xmin, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xlreg, .rreg = xrreg, .shift = xshift, .rshift = xshift, \
	.min = xmin, .max = xmax, .platform_max = xmax, .invert = xinvert})
>>>>>>> refs/remotes/origin/master
#define SOC_SINGLE(xname, reg, shift, max, invert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw,\
	.put = snd_soc_put_volsw, \
<<<<<<< HEAD
	.private_value =  SOC_SINGLE_VALUE(reg, shift, max, invert) }
=======
	.private_value = SOC_SINGLE_VALUE(reg, shift, max, invert, 0) }
#define SOC_SINGLE_RANGE(xname, xreg, xshift, xmin, xmax, xinvert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.info = snd_soc_info_volsw_range, .get = snd_soc_get_volsw_range, \
	.put = snd_soc_put_volsw_range, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .rreg = xreg, .shift = xshift, \
		 .rshift = xshift,  .min = xmin, .max = xmax, \
		 .platform_max = xmax, .invert = xinvert} }
>>>>>>> refs/remotes/origin/master
#define SOC_SINGLE_TLV(xname, reg, shift, max, invert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw,\
	.put = snd_soc_put_volsw, \
<<<<<<< HEAD
	.private_value =  SOC_SINGLE_VALUE(reg, shift, max, invert) }
#define SOC_SINGLE_S8_TLV(xname, xreg, xmin, xmax, tlv_array) \
{	.iface  = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array), \
	.info   = snd_soc_info_volsw_s8, .get = snd_soc_get_volsw_s8, \
	.put    = snd_soc_put_volsw_s8, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .min = xmin, .max = xmax, \
		 .platform_max = xmax} }
<<<<<<< HEAD
<<<<<<< HEAD
#define SOC_DOUBLE(xname, xreg, shift_left, shift_right, xmax, xinvert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw, \
	.put = snd_soc_put_volsw, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .shift = shift_left, .rshift = shift_right, \
		 .max = xmax, .platform_max = xmax, .invert = xinvert} }
#define SOC_DOUBLE_R(xname, reg_left, reg_right, xshift, xmax, xinvert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.info = snd_soc_info_volsw_2r, \
	.get = snd_soc_get_volsw_2r, .put = snd_soc_put_volsw_2r, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = reg_left, .rreg = reg_right, .shift = xshift, \
		.max = xmax, .platform_max = xmax, .invert = xinvert} }
#define SOC_DOUBLE_TLV(xname, xreg, shift_left, shift_right, xmax, xinvert, tlv_array) \
=======
=======
	.private_value = SOC_SINGLE_VALUE(reg, shift, max, invert, 0) }
#define SOC_SINGLE_SX_TLV(xname, xreg, xshift, xmin, xmax, tlv_array) \
{       .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
	SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array),\
	.info = snd_soc_info_volsw, \
	.get = snd_soc_get_volsw_sx,\
	.put = snd_soc_put_volsw_sx, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .rreg = xreg, \
		.shift = xshift, .rshift = xshift, \
		.max = xmax, .min = xmin} }
#define SOC_SINGLE_RANGE_TLV(xname, xreg, xshift, xmin, xmax, xinvert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw_range, \
	.get = snd_soc_get_volsw_range, .put = snd_soc_put_volsw_range, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .rreg = xreg, .shift = xshift, \
		 .rshift = xshift, .min = xmin, .max = xmax, \
		 .platform_max = xmax, .invert = xinvert} }
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define SOC_DOUBLE(xname, reg, shift_left, shift_right, max, invert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw, \
	.put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_VALUE(reg, shift_left, shift_right, \
<<<<<<< HEAD
					  max, invert) }
=======
					  max, invert, 0) }
>>>>>>> refs/remotes/origin/master
#define SOC_DOUBLE_R(xname, reg_left, reg_right, xshift, xmax, xinvert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.info = snd_soc_info_volsw, \
	.get = snd_soc_get_volsw, .put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_R_VALUE(reg_left, reg_right, xshift, \
					    xmax, xinvert) }
<<<<<<< HEAD
#define SOC_DOUBLE_TLV(xname, reg, shift_left, shift_right, max, invert, tlv_array) \
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define SOC_DOUBLE_R_RANGE(xname, reg_left, reg_right, xshift, xmin, \
			   xmax, xinvert)		\
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.info = snd_soc_info_volsw_range, \
	.get = snd_soc_get_volsw_range, .put = snd_soc_put_volsw_range, \
	.private_value = SOC_DOUBLE_R_RANGE_VALUE(reg_left, reg_right, \
					    xshift, xmin, xmax, xinvert) }
#define SOC_DOUBLE_TLV(xname, reg, shift_left, shift_right, max, invert, tlv_array) \
>>>>>>> refs/remotes/origin/master
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw, \
	.put = snd_soc_put_volsw, \
<<<<<<< HEAD
<<<<<<< HEAD
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .shift = shift_left, .rshift = shift_right,\
		 .max = xmax, .platform_max = xmax, .invert = xinvert} }
=======
	.private_value = SOC_DOUBLE_VALUE(reg, shift_left, shift_right, \
					  max, invert) }
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.private_value = SOC_DOUBLE_VALUE(reg, shift_left, shift_right, \
					  max, invert, 0) }
>>>>>>> refs/remotes/origin/master
#define SOC_DOUBLE_R_TLV(xname, reg_left, reg_right, xshift, xmax, xinvert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
<<<<<<< HEAD
<<<<<<< HEAD
	.info = snd_soc_info_volsw_2r, \
	.get = snd_soc_get_volsw_2r, .put = snd_soc_put_volsw_2r, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = reg_left, .rreg = reg_right, .shift = xshift, \
		.max = xmax, .platform_max = xmax, .invert = xinvert} }
=======
=======
>>>>>>> refs/remotes/origin/master
	.info = snd_soc_info_volsw, \
	.get = snd_soc_get_volsw, .put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_R_VALUE(reg_left, reg_right, xshift, \
					    xmax, xinvert) }
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define SOC_DOUBLE_R_RANGE_TLV(xname, reg_left, reg_right, xshift, xmin, \
			       xmax, xinvert, tlv_array)		\
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw_range, \
	.get = snd_soc_get_volsw_range, .put = snd_soc_put_volsw_range, \
	.private_value = SOC_DOUBLE_R_RANGE_VALUE(reg_left, reg_right, \
					    xshift, xmin, xmax, xinvert) }
#define SOC_DOUBLE_R_SX_TLV(xname, xreg, xrreg, xshift, xmin, xmax, tlv_array) \
{       .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
	SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = snd_soc_get_volsw_sx, \
	.put = snd_soc_put_volsw_sx, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .rreg = xrreg, \
		.shift = xshift, .rshift = xshift, \
		.max = xmax, .min = xmin} }
>>>>>>> refs/remotes/origin/master
#define SOC_DOUBLE_S8_TLV(xname, xreg, xmin, xmax, tlv_array) \
{	.iface  = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		  SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array), \
	.info   = snd_soc_info_volsw_s8, .get = snd_soc_get_volsw_s8, \
	.put    = snd_soc_put_volsw_s8, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
<<<<<<< HEAD
<<<<<<< HEAD
		{.reg = xreg, .rshift = 8, .min = xmin, .max = xmax, \
=======
		{.reg = xreg, .min = xmin, .max = xmax, \
>>>>>>> refs/remotes/origin/cm-10.0
		 .platform_max = xmax} }
#define SOC_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmax, xtexts) \
{	.reg = xreg, .shift_l = xshift_l, .shift_r = xshift_r, \
	.max = xmax, .texts = xtexts }
=======
		{.reg = xreg, .min = xmin, .max = xmax, \
		 .platform_max = xmax} }
#define SOC_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmax, xtexts) \
{	.reg = xreg, .shift_l = xshift_l, .shift_r = xshift_r, \
	.max = xmax, .texts = xtexts, \
	.mask = xmax ? roundup_pow_of_two(xmax) - 1 : 0}
>>>>>>> refs/remotes/origin/master
#define SOC_ENUM_SINGLE(xreg, xshift, xmax, xtexts) \
	SOC_ENUM_DOUBLE(xreg, xshift, xshift, xmax, xtexts)
#define SOC_ENUM_SINGLE_EXT(xmax, xtexts) \
{	.max = xmax, .texts = xtexts }
#define SOC_VALUE_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmask, xmax, xtexts, xvalues) \
{	.reg = xreg, .shift_l = xshift_l, .shift_r = xshift_r, \
	.mask = xmask, .max = xmax, .texts = xtexts, .values = xvalues}
#define SOC_VALUE_ENUM_SINGLE(xreg, xshift, xmask, xmax, xtexts, xvalues) \
	SOC_VALUE_ENUM_DOUBLE(xreg, xshift, xshift, xmask, xmax, xtexts, xvalues)
#define SOC_ENUM(xname, xenum) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname,\
	.info = snd_soc_info_enum_double, \
	.get = snd_soc_get_enum_double, .put = snd_soc_put_enum_double, \
	.private_value = (unsigned long)&xenum }
#define SOC_VALUE_ENUM(xname, xenum) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname,\
	.info = snd_soc_info_enum_double, \
	.get = snd_soc_get_value_enum_double, \
	.put = snd_soc_put_value_enum_double, \
	.private_value = (unsigned long)&xenum }
#define SOC_SINGLE_EXT(xname, xreg, xshift, xmax, xinvert,\
	 xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
<<<<<<< HEAD
	.private_value = SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert) }
<<<<<<< HEAD
#define SOC_DOUBLE_EXT(xname, xreg, shift_left, shift_right, xmax, xinvert,\
=======
#define SOC_DOUBLE_EXT(xname, reg, shift_left, shift_right, max, invert,\
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.private_value = SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert, 0) }
#define SOC_DOUBLE_EXT(xname, reg, shift_left, shift_right, max, invert,\
>>>>>>> refs/remotes/origin/master
	 xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
<<<<<<< HEAD
<<<<<<< HEAD
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .shift = shift_left, .rshift = shift_right, \
		 .max = xmax, .platform_max = xmax, .invert = xinvert} }
#define SOC_SINGLE_MULTI_EXT(xname, xreg, xshift, xmax, xinvert, xcount,\
	 xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
=======
	.private_value = \
		SOC_DOUBLE_VALUE(reg, shift_left, shift_right, max, invert) }
 #define SOC_SINGLE_MULTI_EXT(xname, xreg, xshift, xmax, xinvert, xcount,\
	xhandler_get, xhandler_put) \
{      .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	.info = snd_soc_info_multi_ext, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = (unsigned long)&(struct soc_multi_mixer_control) \
		{.reg = xreg, .shift = xshift, .rshift = xshift, .max = xmax, \
		.count = xcount, .platform_max = xmax, .invert = xinvert} }
<<<<<<< HEAD
=======
	.private_value = \
		SOC_DOUBLE_VALUE(reg, shift_left, shift_right, max, invert, 0) }
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define SOC_SINGLE_EXT_TLV(xname, xreg, xshift, xmax, xinvert,\
	 xhandler_get, xhandler_put, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
<<<<<<< HEAD
	.private_value = SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert) }
=======
	.private_value = SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert, 0) }
>>>>>>> refs/remotes/origin/master
#define SOC_DOUBLE_EXT_TLV(xname, xreg, shift_left, shift_right, xmax, xinvert,\
	 xhandler_get, xhandler_put, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		 SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
<<<<<<< HEAD
<<<<<<< HEAD
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .shift = shift_left, .rshift = shift_right, \
		.max = xmax, .platform_max = xmax, .invert = xinvert} }
=======
	.private_value = SOC_DOUBLE_VALUE(xreg, shift_left, shift_right, \
					  xmax, xinvert) }
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.private_value = SOC_DOUBLE_VALUE(xreg, shift_left, shift_right, \
					  xmax, xinvert, 0) }
>>>>>>> refs/remotes/origin/master
#define SOC_DOUBLE_R_EXT_TLV(xname, reg_left, reg_right, xshift, xmax, xinvert,\
	 xhandler_get, xhandler_put, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		 SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p = (tlv_array), \
<<<<<<< HEAD
<<<<<<< HEAD
	.info = snd_soc_info_volsw_2r, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = reg_left, .rreg = reg_right, .shift = xshift, \
		.max = xmax, .platform_max = xmax, .invert = xinvert} }
=======
=======
>>>>>>> refs/remotes/origin/master
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = SOC_DOUBLE_R_VALUE(reg_left, reg_right, xshift, \
					    xmax, xinvert) }
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define SOC_SINGLE_BOOL_EXT(xname, xdata, xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_bool_ext, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = xdata }
#define SOC_ENUM_EXT(xname, xenum, xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
<<<<<<< HEAD
	.info = snd_soc_info_enum_ext, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = (unsigned long)&xenum }

#define SOC_DOUBLE_R_SX_TLV(xname, xreg_left, xreg_right, xshift,\
		xmin, xmax, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		  SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw_2r_sx, \
	.get = snd_soc_get_volsw_2r_sx, \
	.put = snd_soc_put_volsw_2r_sx, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg_left, \
		 .rreg = xreg_right, .shift = xshift, \
		 .min = xmin, .max = xmax} }

<<<<<<< HEAD
=======
	.info = snd_soc_info_enum_double, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = (unsigned long)&xenum }

#define SND_SOC_BYTES(xname, xbase, xregs)		      \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname,   \
	.info = snd_soc_bytes_info, .get = snd_soc_bytes_get, \
	.put = snd_soc_bytes_put, .private_value =	      \
		((unsigned long)&(struct soc_bytes)           \
		{.base = xbase, .num_regs = xregs }) }

#define SND_SOC_BYTES_MASK(xname, xbase, xregs, xmask)	      \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname,   \
	.info = snd_soc_bytes_info, .get = snd_soc_bytes_get, \
	.put = snd_soc_bytes_put, .private_value =	      \
		((unsigned long)&(struct soc_bytes)           \
		{.base = xbase, .num_regs = xregs,	      \
		 .mask = xmask }) }
=======
>>>>>>> refs/remotes/origin/cm-11.0

#define SOC_SINGLE_XR_SX(xname, xregbase, xregcount, xnbits, \
		xmin, xmax, xinvert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.info = snd_soc_info_xr_sx, .get = snd_soc_get_xr_sx, \
	.put = snd_soc_put_xr_sx, \
	.private_value = (unsigned long)&(struct soc_mreg_control) \
		{.regbase = xregbase, .regcount = xregcount, .nbits = xnbits, \
		.invert = xinvert, .min = xmin, .max = xmax} }

#define SOC_SINGLE_STROBE(xname, xreg, xshift, xinvert) \
	SOC_SINGLE_EXT(xname, xreg, xshift, 1, xinvert, \
		snd_soc_get_strobe, snd_soc_put_strobe)
>>>>>>> refs/remotes/origin/master

/*
 * Simplified versions of above macros, declaring a struct and calculating
 * ARRAY_SIZE internally
 */
#define SOC_ENUM_DOUBLE_DECL(name, xreg, xshift_l, xshift_r, xtexts) \
	struct soc_enum name = SOC_ENUM_DOUBLE(xreg, xshift_l, xshift_r, \
						ARRAY_SIZE(xtexts), xtexts)
#define SOC_ENUM_SINGLE_DECL(name, xreg, xshift, xtexts) \
	SOC_ENUM_DOUBLE_DECL(name, xreg, xshift, xshift, xtexts)
#define SOC_ENUM_SINGLE_EXT_DECL(name, xtexts) \
	struct soc_enum name = SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(xtexts), xtexts)
#define SOC_VALUE_ENUM_DOUBLE_DECL(name, xreg, xshift_l, xshift_r, xmask, xtexts, xvalues) \
	struct soc_enum name = SOC_VALUE_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmask, \
							ARRAY_SIZE(xtexts), xtexts, xvalues)
#define SOC_VALUE_ENUM_SINGLE_DECL(name, xreg, xshift, xmask, xtexts, xvalues) \
	SOC_VALUE_ENUM_DOUBLE_DECL(name, xreg, xshift, xshift, xmask, xtexts, xvalues)

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0

/* DAI Link Host Mode Support */
#define SND_SOC_DAI_LINK_NO_HOST		0x1
#define SND_SOC_DAI_LINK_OPT_HOST		0x2

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define snd_soc_get_enum_text(soc_enum, idx) \
	(soc_enum->texts ? soc_enum->texts[idx] : soc_enum->dtexts[idx])


<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
/*
 * Component probe and remove ordering levels for components with runtime
 * dependencies.
 */
#define SND_SOC_COMP_ORDER_FIRST		-2
#define SND_SOC_COMP_ORDER_EARLY		-1
#define SND_SOC_COMP_ORDER_NORMAL		0
#define SND_SOC_COMP_ORDER_LATE		1
#define SND_SOC_COMP_ORDER_LAST		2

<<<<<<< HEAD
<<<<<<< HEAD
/* DAI Link Host Mode Support */
#define SND_SOC_DAI_LINK_NO_HOST		0x1
#define SND_SOC_DAI_LINK_OPT_HOST		0x2

#define snd_soc_get_enum_text(soc_enum, idx) \
	(soc_enum->texts ? soc_enum->texts[idx] : soc_enum->dtexts[idx])


=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Bias levels
 *
 * @ON:      Bias is fully on for audio playback and capture operations.
 * @PREPARE: Prepare for audio operations. Called before DAPM switching for
 *           stream start and stop operations.
 * @STANDBY: Low power standby state when no playback/capture operations are
 *           in progress. NOTE: The transition time between STANDBY and ON
 *           should be as fast as possible and no longer than 10ms.
 * @OFF:     Power Off. No restrictions on transition times.
 */
enum snd_soc_bias_level {
<<<<<<< HEAD
<<<<<<< HEAD
	SND_SOC_BIAS_OFF,
	SND_SOC_BIAS_STANDBY,
	SND_SOC_BIAS_PREPARE,
	SND_SOC_BIAS_ON,
};

=======
=======
>>>>>>> refs/remotes/origin/master
	SND_SOC_BIAS_OFF = 0,
	SND_SOC_BIAS_STANDBY = 1,
	SND_SOC_BIAS_PREPARE = 2,
	SND_SOC_BIAS_ON = 3,
};

struct device_node;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct snd_jack;
struct snd_soc_card;
struct snd_soc_pcm_stream;
struct snd_soc_ops;
struct snd_soc_pcm_runtime;
struct snd_soc_dai;
struct snd_soc_dai_driver;
struct snd_soc_platform;
struct snd_soc_dai_link;
struct snd_soc_platform_driver;
struct snd_soc_codec;
struct snd_soc_codec_driver;
<<<<<<< HEAD
=======
struct snd_soc_component;
struct snd_soc_component_driver;
>>>>>>> refs/remotes/origin/master
struct soc_enum;
struct snd_soc_jack;
struct snd_soc_jack_zone;
struct snd_soc_jack_pin;
<<<<<<< HEAD
struct snd_soc_cache_ops;
<<<<<<< HEAD
<<<<<<< HEAD
struct snd_soc_dsp_link;
=======
struct snd_soc_dpcm_link;
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct snd_soc_dpcm_link;
>>>>>>> refs/remotes/origin/cm-11.0
#include <sound/soc-dapm.h>

#ifdef CONFIG_GPIOLIB
struct snd_soc_jack_gpio;
#endif

typedef int (*hw_write_t)(void *,const char* ,int);

extern struct snd_ac97_bus_ops soc_ac97_ops;
=======
#include <sound/soc-dapm.h>
#include <sound/soc-dpcm.h>

struct snd_soc_jack_gpio;

typedef int (*hw_write_t)(void *,const char* ,int);

extern struct snd_ac97_bus_ops *soc_ac97_ops;
>>>>>>> refs/remotes/origin/master

enum snd_soc_control_type {
	SND_SOC_I2C = 1,
	SND_SOC_SPI,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	SND_SOC_REGMAP,
>>>>>>> refs/remotes/origin/cm-10.0
};

enum snd_soc_compress_type {
	SND_SOC_FLAT_COMPRESSION = 1,
<<<<<<< HEAD
	SND_SOC_LZO_COMPRESSION,
	SND_SOC_RBTREE_COMPRESSION
};

enum snd_soc_pcm_subclass {
	SND_SOC_MUTEX_FE	= 0,
	SND_SOC_MUTEX_BE	= 1,
};

int snd_soc_codec_set_sysclk(struct snd_soc_codec *codec, int clk_id,
			     unsigned int freq, int dir);
=======
=======
	SND_SOC_REGMAP,
>>>>>>> refs/remotes/origin/master
};

enum snd_soc_pcm_subclass {
	SND_SOC_PCM_CLASS_PCM	= 0,
	SND_SOC_PCM_CLASS_BE	= 1,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
/*
 * Dynamic PCM DAI link states.
 */
enum snd_soc_dpcm_state {
	SND_SOC_DPCM_STATE_NEW	= 0,
	SND_SOC_DPCM_STATE_OPEN,
	SND_SOC_DPCM_STATE_HW_PARAMS,
	SND_SOC_DPCM_STATE_PREPARE,
	SND_SOC_DPCM_STATE_START,
	SND_SOC_DPCM_STATE_STOP,
	SND_SOC_DPCM_STATE_PAUSED,
	SND_SOC_DPCM_STATE_SUSPEND,
	SND_SOC_DPCM_STATE_HW_FREE,
	SND_SOC_DPCM_STATE_CLOSE,
};

/*
 * Dynamic PCM trigger ordering. Triggering flexibility is required as some
 * DSPs require triggering before/after their clients/hosts.
 *
 * i.e. some clients may want to manually order this call in their PCM
 * trigger() whilst others will just use the regular core ordering.
 */
enum snd_soc_dpcm_trigger {
	SND_SOC_DPCM_TRIGGER_PRE		= 0,
	SND_SOC_DPCM_TRIGGER_POST,
	SND_SOC_DPCM_TRIGGER_BESPOKE,
<<<<<<< HEAD
=======
enum snd_soc_card_subclass {
	SND_SOC_CARD_CLASS_INIT		= 0,
	SND_SOC_CARD_CLASS_RUNTIME	= 1,
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
};

int snd_soc_codec_set_sysclk(struct snd_soc_codec *codec, int clk_id,
			     int source, unsigned int freq, int dir);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
int snd_soc_codec_set_pll(struct snd_soc_codec *codec, int pll_id, int source,
			  unsigned int freq_in, unsigned int freq_out);

int snd_soc_register_card(struct snd_soc_card *card);
int snd_soc_unregister_card(struct snd_soc_card *card);
<<<<<<< HEAD
=======
int devm_snd_soc_register_card(struct device *dev, struct snd_soc_card *card);
>>>>>>> refs/remotes/origin/master
int snd_soc_suspend(struct device *dev);
int snd_soc_resume(struct device *dev);
int snd_soc_poweroff(struct device *dev);
int snd_soc_register_platform(struct device *dev,
<<<<<<< HEAD
		struct snd_soc_platform_driver *platform_drv);
void snd_soc_unregister_platform(struct device *dev);
=======
		const struct snd_soc_platform_driver *platform_drv);
void snd_soc_unregister_platform(struct device *dev);
int snd_soc_add_platform(struct device *dev, struct snd_soc_platform *platform,
		const struct snd_soc_platform_driver *platform_drv);
void snd_soc_remove_platform(struct snd_soc_platform *platform);
struct snd_soc_platform *snd_soc_lookup_platform(struct device *dev);
>>>>>>> refs/remotes/origin/master
int snd_soc_register_codec(struct device *dev,
		const struct snd_soc_codec_driver *codec_drv,
		struct snd_soc_dai_driver *dai_drv, int num_dai);
void snd_soc_unregister_codec(struct device *dev);
<<<<<<< HEAD
=======
int snd_soc_register_component(struct device *dev,
			 const struct snd_soc_component_driver *cmpnt_drv,
			 struct snd_soc_dai_driver *dai_drv, int num_dai);
int devm_snd_soc_register_component(struct device *dev,
			 const struct snd_soc_component_driver *cmpnt_drv,
			 struct snd_soc_dai_driver *dai_drv, int num_dai);
void snd_soc_unregister_component(struct device *dev);
>>>>>>> refs/remotes/origin/master
int snd_soc_codec_volatile_register(struct snd_soc_codec *codec,
				    unsigned int reg);
int snd_soc_codec_readable_register(struct snd_soc_codec *codec,
				    unsigned int reg);
int snd_soc_codec_writable_register(struct snd_soc_codec *codec,
				    unsigned int reg);
int snd_soc_codec_set_cache_io(struct snd_soc_codec *codec,
			       int addr_bits, int data_bits,
			       enum snd_soc_control_type control);
int snd_soc_cache_sync(struct snd_soc_codec *codec);
int snd_soc_cache_init(struct snd_soc_codec *codec);
int snd_soc_cache_exit(struct snd_soc_codec *codec);
int snd_soc_cache_write(struct snd_soc_codec *codec,
			unsigned int reg, unsigned int value);
int snd_soc_cache_read(struct snd_soc_codec *codec,
		       unsigned int reg, unsigned int *value);
<<<<<<< HEAD
int snd_soc_default_volatile_register(struct snd_soc_codec *codec,
				      unsigned int reg);
int snd_soc_default_readable_register(struct snd_soc_codec *codec,
				      unsigned int reg);
int snd_soc_default_writable_register(struct snd_soc_codec *codec,
				      unsigned int reg);
<<<<<<< HEAD
unsigned int snd_soc_platform_read(struct snd_soc_platform *platform,
					unsigned int reg);
unsigned int snd_soc_platform_write(struct snd_soc_platform *platform,
					unsigned int reg, unsigned int val);
=======
=======
>>>>>>> refs/remotes/origin/master
int snd_soc_platform_read(struct snd_soc_platform *platform,
					unsigned int reg);
int snd_soc_platform_write(struct snd_soc_platform *platform,
					unsigned int reg, unsigned int val);
int soc_new_pcm(struct snd_soc_pcm_runtime *rtd, int num);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
int soc_new_compress(struct snd_soc_pcm_runtime *rtd, int num);
>>>>>>> refs/remotes/origin/master

struct snd_pcm_substream *snd_soc_get_dai_substream(struct snd_soc_card *card,
		const char *dai_link, int stream);
struct snd_soc_pcm_runtime *snd_soc_get_pcm_runtime(struct snd_soc_card *card,
		const char *dai_link);

struct snd_pcm_substream *snd_soc_get_dai_substream(struct snd_soc_card *card,
		const char *dai_link, int stream);
struct snd_soc_pcm_runtime *snd_soc_get_pcm_runtime(struct snd_soc_card *card,
		const char *dai_link);

/* Utility functions to get clock rates from various things */
int snd_soc_calc_frame_size(int sample_size, int channels, int tdm_slots);
int snd_soc_params_to_frame_size(struct snd_pcm_hw_params *params);
int snd_soc_calc_bclk(int fs, int sample_size, int channels, int tdm_slots);
int snd_soc_params_to_bclk(struct snd_pcm_hw_params *parms);

/* set runtime hw params */
int snd_soc_set_runtime_hwparams(struct snd_pcm_substream *substream,
	const struct snd_pcm_hardware *hw);

<<<<<<< HEAD
=======
int snd_soc_platform_trigger(struct snd_pcm_substream *substream,
		int cmd, struct snd_soc_platform *platform);

>>>>>>> refs/remotes/origin/master
/* Jack reporting */
int snd_soc_jack_new(struct snd_soc_codec *codec, const char *id, int type,
		     struct snd_soc_jack *jack);
void snd_soc_jack_report(struct snd_soc_jack *jack, int status, int mask);
<<<<<<< HEAD
<<<<<<< HEAD
void snd_soc_jack_report_no_dapm(struct snd_soc_jack *jack, int status,
				 int mask);
=======
>>>>>>> refs/remotes/origin/master
=======
void snd_soc_jack_report_no_dapm(struct snd_soc_jack *jack, int status,
				 int mask);
>>>>>>> refs/remotes/origin/cm-11.0
int snd_soc_jack_add_pins(struct snd_soc_jack *jack, int count,
			  struct snd_soc_jack_pin *pins);
void snd_soc_jack_notifier_register(struct snd_soc_jack *jack,
				    struct notifier_block *nb);
void snd_soc_jack_notifier_unregister(struct snd_soc_jack *jack,
				      struct notifier_block *nb);
int snd_soc_jack_add_zones(struct snd_soc_jack *jack, int count,
			  struct snd_soc_jack_zone *zones);
int snd_soc_jack_get_type(struct snd_soc_jack *jack, int micbias_voltage);
#ifdef CONFIG_GPIOLIB
int snd_soc_jack_add_gpios(struct snd_soc_jack *jack, int count,
			struct snd_soc_jack_gpio *gpios);
void snd_soc_jack_free_gpios(struct snd_soc_jack *jack, int count,
			struct snd_soc_jack_gpio *gpios);
<<<<<<< HEAD
=======
#else
static inline int snd_soc_jack_add_gpios(struct snd_soc_jack *jack, int count,
					 struct snd_soc_jack_gpio *gpios)
{
	return 0;
}

static inline void snd_soc_jack_free_gpios(struct snd_soc_jack *jack, int count,
					   struct snd_soc_jack_gpio *gpios)
{
}
>>>>>>> refs/remotes/origin/master
#endif

/* codec register bit access */
int snd_soc_update_bits(struct snd_soc_codec *codec, unsigned short reg,
				unsigned int mask, unsigned int value);
int snd_soc_update_bits_locked(struct snd_soc_codec *codec,
			       unsigned short reg, unsigned int mask,
			       unsigned int value);
int snd_soc_test_bits(struct snd_soc_codec *codec, unsigned short reg,
				unsigned int mask, unsigned int value);

int snd_soc_new_ac97_codec(struct snd_soc_codec *codec,
	struct snd_ac97_bus_ops *ops, int num);
void snd_soc_free_ac97_codec(struct snd_soc_codec *codec);

<<<<<<< HEAD
=======
int snd_soc_set_ac97_ops(struct snd_ac97_bus_ops *ops);
int snd_soc_set_ac97_ops_of_reset(struct snd_ac97_bus_ops *ops,
		struct platform_device *pdev);

>>>>>>> refs/remotes/origin/master
/*
 *Controls
 */
struct snd_kcontrol *snd_soc_cnew(const struct snd_kcontrol_new *_template,
<<<<<<< HEAD
<<<<<<< HEAD
				  void *data, char *long_name,
				  const char *prefix);
<<<<<<< HEAD
int snd_soc_add_controls(struct snd_soc_codec *codec,
	const struct snd_kcontrol_new *controls, int num_controls);
int snd_soc_add_platform_controls(struct snd_soc_platform *platform,
	const struct snd_kcontrol_new *controls, int num_controls);
=======
=======
				  void *data, const char *long_name,
=======
				  void *data, char *long_name,
>>>>>>> refs/remotes/origin/cm-11.0
				  const char *prefix);
struct snd_kcontrol *snd_soc_card_get_kcontrol(struct snd_soc_card *soc_card,
					       const char *name);
>>>>>>> refs/remotes/origin/master
int snd_soc_add_codec_controls(struct snd_soc_codec *codec,
	const struct snd_kcontrol_new *controls, int num_controls);
int snd_soc_add_platform_controls(struct snd_soc_platform *platform,
	const struct snd_kcontrol_new *controls, int num_controls);
int snd_soc_add_card_controls(struct snd_soc_card *soc_card,
	const struct snd_kcontrol_new *controls, int num_controls);
int snd_soc_add_dai_controls(struct snd_soc_dai *dai,
	const struct snd_kcontrol_new *controls, int num_controls);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
int snd_soc_info_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_info_enum_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
=======
int snd_soc_info_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
>>>>>>> refs/remotes/origin/master
int snd_soc_get_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_get_value_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_value_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_info_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
int snd_soc_info_multi_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
>>>>>>> refs/remotes/origin/cm-10.0
=======
int snd_soc_info_multi_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
>>>>>>> refs/remotes/origin/cm-11.0
int snd_soc_info_volsw_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
=======
>>>>>>> refs/remotes/origin/master
#define snd_soc_info_bool_ext		snd_ctl_boolean_mono_info
int snd_soc_get_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
<<<<<<< HEAD
<<<<<<< HEAD
int snd_soc_info_volsw_2r(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_get_volsw_2r(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_volsw_2r(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
=======
#define snd_soc_get_volsw_2r snd_soc_get_volsw
#define snd_soc_put_volsw_2r snd_soc_put_volsw
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define snd_soc_get_volsw_2r snd_soc_get_volsw
#define snd_soc_put_volsw_2r snd_soc_put_volsw
int snd_soc_get_volsw_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_volsw_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
>>>>>>> refs/remotes/origin/master
int snd_soc_info_volsw_s8(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_get_volsw_s8(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_volsw_s8(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
<<<<<<< HEAD
<<<<<<< HEAD
int snd_soc_info_multi_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
=======
>>>>>>> refs/remotes/origin/cm-10.0
int snd_soc_limit_volume(struct snd_soc_codec *codec,
	const char *name, int max);
int snd_soc_info_volsw_2r_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_get_volsw_2r_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_volsw_2r_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);

/**
 * struct snd_soc_reg_access - Describes whether a given register is
 * readable, writable or volatile.
 *
 * @reg: the register number
 * @read: whether this register is readable
 * @write: whether this register is writable
 * @vol: whether this register is volatile
 */
struct snd_soc_reg_access {
	u16 reg;
	u16 read;
	u16 write;
	u16 vol;
};
=======
int snd_soc_info_volsw_range(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_put_volsw_range(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_get_volsw_range(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_limit_volume(struct snd_soc_codec *codec,
	const char *name, int max);
int snd_soc_bytes_info(struct snd_kcontrol *kcontrol,
		       struct snd_ctl_elem_info *uinfo);
int snd_soc_bytes_get(struct snd_kcontrol *kcontrol,
		      struct snd_ctl_elem_value *ucontrol);
int snd_soc_bytes_put(struct snd_kcontrol *kcontrol,
		      struct snd_ctl_elem_value *ucontrol);
int snd_soc_info_xr_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_get_xr_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_xr_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_get_strobe(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_strobe(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
>>>>>>> refs/remotes/origin/master

/**
 * struct snd_soc_jack_pin - Describes a pin to update based on jack detection
 *
 * @pin:    name of the pin to update
 * @mask:   bits to check for in reported jack status
 * @invert: if non-zero then pin is enabled when status is not reported
 */
struct snd_soc_jack_pin {
	struct list_head list;
	const char *pin;
	int mask;
	bool invert;
};

/**
 * struct snd_soc_jack_zone - Describes voltage zones of jack detection
 *
 * @min_mv: start voltage in mv
 * @max_mv: end voltage in mv
 * @jack_type: type of jack that is expected for this voltage
 * @debounce_time: debounce_time for jack, codec driver should wait for this
 *		duration before reading the adc for voltages
 * @:list: list container
 */
struct snd_soc_jack_zone {
	unsigned int min_mv;
	unsigned int max_mv;
	unsigned int jack_type;
	unsigned int debounce_time;
	struct list_head list;
};

/**
 * struct snd_soc_jack_gpio - Describes a gpio pin for jack detection
 *
 * @gpio:         gpio number
 * @name:         gpio name
 * @report:       value to report when jack detected
 * @invert:       report presence in low state
 * @debouce_time: debouce time in ms
 * @wake:	  enable as wake source
 * @jack_status_check: callback function which overrides the detection
 *		       to provide more complex checks (eg, reading an
 *		       ADC).
 */
<<<<<<< HEAD
#ifdef CONFIG_GPIOLIB
=======
>>>>>>> refs/remotes/origin/master
struct snd_soc_jack_gpio {
	unsigned int gpio;
	const char *name;
	int report;
	int invert;
	int debounce_time;
	bool wake;

	struct snd_soc_jack *jack;
	struct delayed_work work;

	int (*jack_status_check)(void);
};
<<<<<<< HEAD
#endif

struct snd_soc_jack {
=======

struct snd_soc_jack {
	struct mutex mutex;
>>>>>>> refs/remotes/origin/master
	struct snd_jack *jack;
	struct snd_soc_codec *codec;
	struct list_head pins;
	int status;
	struct blocking_notifier_head notifier;
	struct list_head jack_zones;
};

/* SoC PCM stream information */
struct snd_soc_pcm_stream {
	const char *stream_name;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	const char *aif_name;	/* DAPM AIF widget name */
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	const char *aif_name;	/* DAPM AIF widget name */
>>>>>>> refs/remotes/origin/cm-11.0
	u64 formats;			/* SNDRV_PCM_FMTBIT_* */
	unsigned int rates;		/* SNDRV_PCM_RATE_* */
	unsigned int rate_min;		/* min rate */
	unsigned int rate_max;		/* max rate */
	unsigned int channels_min;	/* min channels */
	unsigned int channels_max;	/* max channels */
<<<<<<< HEAD
<<<<<<< HEAD
=======
	unsigned int sig_bits;		/* number of bits of content */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned int sig_bits;		/* number of bits of content */
>>>>>>> refs/remotes/origin/master
};

/* SoC audio ops */
struct snd_soc_ops {
	int (*startup)(struct snd_pcm_substream *);
	void (*shutdown)(struct snd_pcm_substream *);
	int (*hw_params)(struct snd_pcm_substream *, struct snd_pcm_hw_params *);
	int (*hw_free)(struct snd_pcm_substream *);
	int (*prepare)(struct snd_pcm_substream *);
	int (*trigger)(struct snd_pcm_substream *, int);
};

<<<<<<< HEAD
/* SoC cache ops */
struct snd_soc_cache_ops {
	const char *name;
	enum snd_soc_compress_type id;
	int (*init)(struct snd_soc_codec *codec);
	int (*exit)(struct snd_soc_codec *codec);
	int (*read)(struct snd_soc_codec *codec, unsigned int reg,
		unsigned int *value);
	int (*write)(struct snd_soc_codec *codec, unsigned int reg,
		unsigned int value);
	int (*sync)(struct snd_soc_codec *codec);
=======
struct snd_soc_compr_ops {
	int (*startup)(struct snd_compr_stream *);
	void (*shutdown)(struct snd_compr_stream *);
	int (*set_params)(struct snd_compr_stream *);
	int (*trigger)(struct snd_compr_stream *);
};

/* component interface */
struct snd_soc_component_driver {
	const char *name;

	/* DT */
	int (*of_xlate_dai_name)(struct snd_soc_component *component,
				 struct of_phandle_args *args,
				 const char **dai_name);
};

struct snd_soc_component {
	const char *name;
	int id;
	struct device *dev;
	struct list_head list;

	struct snd_soc_dai_driver *dai_drv;
	int num_dai;

	const struct snd_soc_component_driver *driver;
>>>>>>> refs/remotes/origin/master
};

/* SoC Audio Codec device */
struct snd_soc_codec {
	const char *name;
	const char *name_prefix;
	int id;
	struct device *dev;
	const struct snd_soc_codec_driver *driver;

	struct mutex mutex;
	struct snd_soc_card *card;
	struct list_head list;
	struct list_head card_list;
	int num_dai;
<<<<<<< HEAD
	enum snd_soc_compress_type compress_type;
	size_t reg_size;	/* reg_cache_size * reg_word_size */
=======
>>>>>>> refs/remotes/origin/master
	int (*volatile_register)(struct snd_soc_codec *, unsigned int);
	int (*readable_register)(struct snd_soc_codec *, unsigned int);
	int (*writable_register)(struct snd_soc_codec *, unsigned int);

	/* runtime */
	struct snd_ac97 *ac97;  /* for ad-hoc ac97 devices */
	unsigned int active;
	unsigned int cache_bypass:1; /* Suppress access to the cache */
	unsigned int suspended:1; /* Codec is in suspend PM state */
	unsigned int probed:1; /* Codec has been probed */
	unsigned int ac97_registered:1; /* Codec has been AC97 registered */
	unsigned int ac97_created:1; /* Codec has been created by SoC */
<<<<<<< HEAD
	unsigned int sysfs_registered:1; /* codec has been sysfs registered */
	unsigned int cache_init:1; /* codec cache has been initialized */
<<<<<<< HEAD
=======
	unsigned int using_regmap:1; /* using regmap access */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned int cache_init:1; /* codec cache has been initialized */
	unsigned int using_regmap:1; /* using regmap access */
>>>>>>> refs/remotes/origin/master
	u32 cache_only;  /* Suppress writes to hardware */
	u32 cache_sync; /* Cache needs to be synced to hardware */

	/* codec IO */
	void *control_data; /* codec control (i2c/3wire) data */
<<<<<<< HEAD
	enum snd_soc_control_type control_type;
=======
>>>>>>> refs/remotes/origin/master
	hw_write_t hw_write;
	unsigned int (*hw_read)(struct snd_soc_codec *, unsigned int);
	unsigned int (*read)(struct snd_soc_codec *, unsigned int);
	int (*write)(struct snd_soc_codec *, unsigned int, unsigned int);
<<<<<<< HEAD
	int (*bulk_write_raw)(struct snd_soc_codec *, unsigned int, const void *, size_t);
	void *reg_cache;
	const void *reg_def_copy;
	const struct snd_soc_cache_ops *cache_ops;
	struct mutex cache_rw_mutex;
<<<<<<< HEAD

	/* dapm */
	struct snd_soc_dapm_context dapm;
=======
	int val_bytes;

	/* dapm */
	struct snd_soc_dapm_context dapm;
	unsigned int ignore_pmdown_time:1; /* pmdown_time is ignored at stop */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	void *reg_cache;
	struct mutex cache_rw_mutex;
	int val_bytes;

	/* component */
	struct snd_soc_component component;

	/* dapm */
	struct snd_soc_dapm_context dapm;
	unsigned int ignore_pmdown_time:1; /* pmdown_time is ignored at stop */
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_codec_root;
	struct dentry *debugfs_reg;
<<<<<<< HEAD
	struct dentry *debugfs_dapm;
=======
>>>>>>> refs/remotes/origin/master
#endif
};

/* codec driver */
struct snd_soc_codec_driver {

	/* driver ops */
	int (*probe)(struct snd_soc_codec *);
	int (*remove)(struct snd_soc_codec *);
<<<<<<< HEAD
<<<<<<< HEAD
	int (*suspend)(struct snd_soc_codec *,
			pm_message_t state);
=======
	int (*suspend)(struct snd_soc_codec *);
>>>>>>> refs/remotes/origin/cm-10.0
	int (*resume)(struct snd_soc_codec *);
=======
	int (*suspend)(struct snd_soc_codec *);
	int (*resume)(struct snd_soc_codec *);
	struct snd_soc_component_driver component_driver;
>>>>>>> refs/remotes/origin/master

	/* Default control and setup, added after probe() is run */
	const struct snd_kcontrol_new *controls;
	int num_controls;
	const struct snd_soc_dapm_widget *dapm_widgets;
	int num_dapm_widgets;
	const struct snd_soc_dapm_route *dapm_routes;
	int num_dapm_routes;

	/* codec wide operations */
	int (*set_sysclk)(struct snd_soc_codec *codec,
<<<<<<< HEAD
<<<<<<< HEAD
			  int clk_id, unsigned int freq, int dir);
=======
			  int clk_id, int source, unsigned int freq, int dir);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			  int clk_id, int source, unsigned int freq, int dir);
>>>>>>> refs/remotes/origin/master
	int (*set_pll)(struct snd_soc_codec *codec, int pll_id, int source,
		unsigned int freq_in, unsigned int freq_out);

	/* codec IO */
	unsigned int (*read)(struct snd_soc_codec *, unsigned int);
	int (*write)(struct snd_soc_codec *, unsigned int, unsigned int);
	int (*display_register)(struct snd_soc_codec *, char *,
				size_t, unsigned int);
	int (*volatile_register)(struct snd_soc_codec *, unsigned int);
	int (*readable_register)(struct snd_soc_codec *, unsigned int);
	int (*writable_register)(struct snd_soc_codec *, unsigned int);
<<<<<<< HEAD
<<<<<<< HEAD
	short reg_cache_size;
=======
	unsigned int reg_cache_size;
>>>>>>> refs/remotes/origin/cm-10.0
	short reg_cache_step;
	short reg_word_size;
	const void *reg_cache_default;
	short reg_access_size;
	const struct snd_soc_reg_access *reg_access_default;
	enum snd_soc_compress_type compress_type;
=======
	unsigned int reg_cache_size;
	short reg_cache_step;
	short reg_word_size;
	const void *reg_cache_default;
>>>>>>> refs/remotes/origin/master

	/* codec bias level */
	int (*set_bias_level)(struct snd_soc_codec *,
			      enum snd_soc_bias_level level);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	bool idle_bias_off;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bool idle_bias_off;
>>>>>>> refs/remotes/origin/master

	void (*seq_notifier)(struct snd_soc_dapm_context *,
			     enum snd_soc_dapm_type, int);

<<<<<<< HEAD
<<<<<<< HEAD
	/* probe ordering - for components with runtime dependencies */
	int probe_order;
	int remove_order;

	/* codec stream completion event */
	int (*stream_event)(struct snd_soc_dapm_context *dapm);
=======
	/* codec stream completion event */
	int (*stream_event)(struct snd_soc_dapm_context *dapm, int event);

	/* probe ordering - for components with runtime dependencies */
	int probe_order;
	int remove_order;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* codec stream completion event */
	int (*stream_event)(struct snd_soc_dapm_context *dapm, int event);

	/* probe ordering - for components with runtime dependencies */
	int probe_order;
	int remove_order;
>>>>>>> refs/remotes/origin/master
};

/* SoC platform interface */
struct snd_soc_platform_driver {

	int (*probe)(struct snd_soc_platform *);
	int (*remove)(struct snd_soc_platform *);
	int (*suspend)(struct snd_soc_dai *dai);
	int (*resume)(struct snd_soc_dai *dai);

	/* pcm creation and destruction */
	int (*pcm_new)(struct snd_soc_pcm_runtime *);
	void (*pcm_free)(struct snd_pcm *);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	/* Default control and setup, added after probe() is run */
	const struct snd_kcontrol_new *controls;
	int num_controls;
	const struct snd_soc_dapm_widget *dapm_widgets;
	int num_dapm_widgets;
	const struct snd_soc_dapm_route *dapm_routes;
	int num_dapm_routes;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * For platform caused delay reporting.
	 * Optional.
	 */
	snd_pcm_sframes_t (*delay)(struct snd_pcm_substream *,
		struct snd_soc_dai *);

<<<<<<< HEAD
	/* platform stream ops */
	struct snd_pcm_ops *ops;

<<<<<<< HEAD
=======
	/* platform stream completion event */
	int (*stream_event)(struct snd_soc_dapm_context *dapm, int event);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* platform stream pcm ops */
	const struct snd_pcm_ops *ops;

	/* platform stream compress ops */
	const struct snd_compr_ops *compr_ops;

	/* platform stream completion event */
	int (*stream_event)(struct snd_soc_dapm_context *dapm, int event);

>>>>>>> refs/remotes/origin/master
	/* probe ordering - for components with runtime dependencies */
	int probe_order;
	int remove_order;

<<<<<<< HEAD
<<<<<<< HEAD
	int (*stream_event)(struct snd_soc_dapm_context *dapm);
	int (*bespoke_trigger)(struct snd_pcm_substream *, int);

	/* platform DAPM IO TODO: refactor this */
	unsigned int (*read)(struct snd_soc_platform *, unsigned int);
	int (*write)(struct snd_soc_platform *, unsigned int, unsigned int);
=======
	/* platform IO - used for platform DAPM */
	unsigned int (*read)(struct snd_soc_platform *, unsigned int);
	int (*write)(struct snd_soc_platform *, unsigned int, unsigned int);

	int (*bespoke_trigger)(struct snd_pcm_substream *, int);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* platform IO - used for platform DAPM */
	unsigned int (*read)(struct snd_soc_platform *, unsigned int);
	int (*write)(struct snd_soc_platform *, unsigned int, unsigned int);
<<<<<<< HEAD
	int (*bespoke_trigger)(struct snd_pcm_substream *, int);
>>>>>>> refs/remotes/origin/master
=======

	int (*bespoke_trigger)(struct snd_pcm_substream *, int);
>>>>>>> refs/remotes/origin/cm-11.0
};

struct snd_soc_platform {
	const char *name;
	int id;
	struct device *dev;
<<<<<<< HEAD
	struct snd_soc_platform_driver *driver;
<<<<<<< HEAD
=======
	const struct snd_soc_platform_driver *driver;
	struct mutex mutex;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	unsigned int suspended:1; /* platform is suspended */
	unsigned int probed:1;

<<<<<<< HEAD
<<<<<<< HEAD
	struct snd_card *snd_card;
	struct snd_soc_card *card;
	struct list_head list;
	struct list_head card_list;
	int num_dai;

	/* dapm */
=======
	struct snd_soc_card *card;
	struct list_head list;
	struct list_head card_list;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct snd_soc_card *card;
	struct list_head list;
	struct list_head card_list;

>>>>>>> refs/remotes/origin/master
	struct snd_soc_dapm_context dapm;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_platform_root;
<<<<<<< HEAD
	struct dentry *debugfs_dapm;
=======
>>>>>>> refs/remotes/origin/master
#endif
};

struct snd_soc_dai_link {
	/* config - must be set by machine driver */
	const char *name;			/* Codec name */
	const char *stream_name;		/* Stream name */
<<<<<<< HEAD
	const char *codec_name;		/* for multi-codec */
<<<<<<< HEAD
	const char *platform_name;	/* for multi-platform */
	const char *cpu_dai_name;
	const char *codec_dai_name;

	struct snd_soc_dsp_link *dsp_link;
=======
	const struct device_node *codec_of_node;
	const char *platform_name;	/* for multi-platform */
	const struct device_node *platform_of_node;
	const char *cpu_dai_name;
	const struct device_node *cpu_dai_of_node;
	const char *codec_dai_name;
=======
	/*
	 * You MAY specify the link's CPU-side device, either by device name,
	 * or by DT/OF node, but not both. If this information is omitted,
	 * the CPU-side DAI is matched using .cpu_dai_name only, which hence
	 * must be globally unique. These fields are currently typically used
	 * only for codec to codec links, or systems using device tree.
	 */
	const char *cpu_name;
	const struct device_node *cpu_of_node;
	/*
	 * You MAY specify the DAI name of the CPU DAI. If this information is
	 * omitted, the CPU-side DAI is matched using .cpu_name/.cpu_of_node
	 * only, which only works well when that device exposes a single DAI.
	 */
	const char *cpu_dai_name;
	/*
	 * You MUST specify the link's codec, either by device name, or by
	 * DT/OF node, but not both.
	 */
	const char *codec_name;
	const struct device_node *codec_of_node;
	/* You MUST specify the DAI name within the codec */
	const char *codec_dai_name;
	/*
	 * You MAY specify the link's platform/PCM/DMA driver, either by
	 * device name, or by DT/OF node, but not both. Some forms of link
	 * do not need a platform.
	 */
	const char *platform_name;
	const struct device_node *platform_of_node;
	int be_id;	/* optional ID for machine driver BE identification */

	const struct snd_soc_pcm_stream *params;
>>>>>>> refs/remotes/origin/master

	unsigned int dai_fmt;           /* format to set on init */

	enum snd_soc_dpcm_trigger trigger[2]; /* trigger type for DPCM */

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* Keep DAI active over suspend */
	unsigned int ignore_suspend:1;

	/* Symmetry requirements */
	unsigned int symmetric_rates:1;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* No PCM created for this DAI link */
	unsigned int no_pcm:1;
	/* This DAI link can change CODEC and platform at runtime*/
	unsigned int dynamic:1;
<<<<<<< HEAD
<<<<<<< HEAD
	/* This DAI link has no codec side driver*/
	unsigned int no_codec:1;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* This DAI has a Backend ID */
	unsigned int be_id;
	/* This DAI can support no host IO (no pcm data is copied to from host) */
	unsigned int no_host_mode:2;
<<<<<<< HEAD

<<<<<<< HEAD
=======
	/* pmdown_time is ignored at stop */
	unsigned int ignore_pmdown_time:1;

>>>>>>> refs/remotes/origin/cm-10.0
	/* codec/machine specific init - e.g. add machine controls */
	int (*init)(struct snd_soc_pcm_runtime *rtd);

	/* hw_params re-writing for BE and FE sync */
=======
	unsigned int symmetric_channels:1;
	unsigned int symmetric_samplebits:1;

	/* Do not create a PCM for this DAI link (Backend link) */
	unsigned int no_pcm:1;

	/* This DAI link can route to other DAI links at runtime (Frontend)*/
	unsigned int dynamic:1;

	/* DPCM capture and Playback support */
	unsigned int dpcm_capture:1;
	unsigned int dpcm_playback:1;
=======
>>>>>>> refs/remotes/origin/cm-11.0

	/* pmdown_time is ignored at stop */
	unsigned int ignore_pmdown_time:1;

	/* codec/machine specific init - e.g. add machine controls */
	int (*init)(struct snd_soc_pcm_runtime *rtd);

<<<<<<< HEAD
	/* optional hw_params re-writing for BE and FE sync */
>>>>>>> refs/remotes/origin/master
=======
	/* hw_params re-writing for BE and FE sync */
>>>>>>> refs/remotes/origin/cm-11.0
	int (*be_hw_params_fixup)(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params);

	/* machine stream operations */
<<<<<<< HEAD
	struct snd_soc_ops *ops;
=======
	const struct snd_soc_ops *ops;
	const struct snd_soc_compr_ops *compr_ops;

	/* For unidirectional dai links */
	bool playback_only;
	bool capture_only;
>>>>>>> refs/remotes/origin/master
};

struct snd_soc_codec_conf {
	const char *dev_name;

	/*
	 * optional map of kcontrol, widget and path name prefixes that are
	 * associated per device
	 */
	const char *name_prefix;
<<<<<<< HEAD

	/*
	 * set this to the desired compression type if you want to
	 * override the one supplied in codec->driver->compress_type
	 */
	enum snd_soc_compress_type compress_type;
=======
>>>>>>> refs/remotes/origin/master
};

struct snd_soc_aux_dev {
	const char *name;		/* Codec name */
	const char *codec_name;		/* for multi-codec */

	/* codec/machine specific init - e.g. add machine controls */
	int (*init)(struct snd_soc_dapm_context *dapm);
};

/* SoC card */
struct snd_soc_card {
	const char *name;
	const char *long_name;
	const char *driver_name;
	struct device *dev;
	struct snd_card *snd_card;
	struct module *owner;

	struct list_head list;
	struct mutex mutex;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	struct mutex dapm_mutex;
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct mutex dpcm_mutex;

	struct mutex dapm_mutex;
	struct mutex dapm_power_mutex;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	struct mutex dsp_mutex;
	spinlock_t dsp_spinlock;
=======
	struct mutex dapm_mutex;
>>>>>>> refs/remotes/origin/master
=======
	struct mutex dsp_mutex;
	spinlock_t dsp_spinlock;
>>>>>>> refs/remotes/origin/cm-11.0

	bool instantiated;

	int (*probe)(struct snd_soc_card *card);
	int (*late_probe)(struct snd_soc_card *card);
	int (*remove)(struct snd_soc_card *card);

	/* the pre and post PM functions are used to do any PM work before and
	 * after the codec and DAI's do any PM work. */
	int (*suspend_pre)(struct snd_soc_card *card);
	int (*suspend_post)(struct snd_soc_card *card);
	int (*resume_pre)(struct snd_soc_card *card);
	int (*resume_post)(struct snd_soc_card *card);

	/* callbacks */
	int (*set_bias_level)(struct snd_soc_card *,
<<<<<<< HEAD
<<<<<<< HEAD
			      enum snd_soc_bias_level level);
	int (*set_bias_level_post)(struct snd_soc_card *,
=======
=======
>>>>>>> refs/remotes/origin/master
			      struct snd_soc_dapm_context *dapm,
			      enum snd_soc_bias_level level);
	int (*set_bias_level_post)(struct snd_soc_card *,
				   struct snd_soc_dapm_context *dapm,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				   enum snd_soc_bias_level level);

	long pmdown_time;

	/* CPU <--> Codec DAI links  */
	struct snd_soc_dai_link *dai_link;
	int num_links;
	struct snd_soc_pcm_runtime *rtd;
	int num_rtd;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int num_playback_channels;
	int num_capture_channels;
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	int num_playback_channels;
	int num_capture_channels;
>>>>>>> refs/remotes/origin/cm-11.0

	/* optional codec specific configuration */
	struct snd_soc_codec_conf *codec_conf;
	int num_configs;

	/*
	 * optional auxiliary devices such as amplifiers or codecs with DAI
	 * link unused
	 */
	struct snd_soc_aux_dev *aux_dev;
	int num_aux_devs;
	struct snd_soc_pcm_runtime *rtd_aux;
	int num_aux_rtd;

	const struct snd_kcontrol_new *controls;
	int num_controls;

	/*
	 * Card-specific routes and widgets.
	 */
	const struct snd_soc_dapm_widget *dapm_widgets;
	int num_dapm_widgets;
	const struct snd_soc_dapm_route *dapm_routes;
	int num_dapm_routes;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	bool fully_routed;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bool fully_routed;
>>>>>>> refs/remotes/origin/master

	struct work_struct deferred_resume_work;

	/* lists of probed devices belonging to this card */
	struct list_head codec_dev_list;
	struct list_head platform_dev_list;
	struct list_head dai_dev_list;

	struct list_head widgets;
	struct list_head paths;
	struct list_head dapm_list;
<<<<<<< HEAD
<<<<<<< HEAD

	/* Generic DAPM context for the card */
	struct snd_soc_dapm_context dapm;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct list_head dapm_dirty;

	/* Generic DAPM context for the card */
	struct snd_soc_dapm_context dapm;
	struct snd_soc_dapm_stats dapm_stats;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct snd_soc_dapm_update *update;
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_card_root;
	struct dentry *debugfs_pop_time;
#endif
	u32 pop_time;

	void *drvdata;
};

<<<<<<< HEAD
<<<<<<< HEAD
/* DSP runtime data */
<<<<<<< HEAD
struct snd_soc_dsp_runtime {
=======
struct snd_soc_dpcm_runtime {
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* DSP runtime data */
struct snd_soc_dpcm_runtime {
>>>>>>> refs/remotes/origin/cm-11.0
	struct list_head be_clients;
	struct list_head fe_clients;
	int users;
	struct snd_pcm_runtime *runtime;
<<<<<<< HEAD
<<<<<<< HEAD
	struct snd_pcm_hw_params params;
	int runtime_update;
	bool hwparam_set;
};

/* SoC machine DAI configuration, glues a codec and cpu DAI together */
struct snd_soc_pcm_runtime  {
	struct device dev;
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct snd_pcm_hw_params hw_params;
	int runtime_update;
	enum snd_soc_dpcm_state state;
};

/* SoC machine DAI configuration, glues a codec and cpu DAI together */
struct snd_soc_pcm_runtime {
	struct device *dev;
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* SoC machine DAI configuration, glues a codec and cpu DAI together */
struct snd_soc_pcm_runtime {
	struct device *dev;
>>>>>>> refs/remotes/origin/master
	struct snd_soc_card *card;
	struct snd_soc_dai_link *dai_link;
	struct mutex pcm_mutex;
	enum snd_soc_pcm_subclass pcm_subclass;
	struct snd_pcm_ops ops;

<<<<<<< HEAD
	unsigned int complete:1;
	unsigned int dev_registered:1;

<<<<<<< HEAD
<<<<<<< HEAD
	/* DSP runtime data */
	struct snd_soc_dsp_runtime dsp[2];

	/* Symmetry data - only valid if symmetry is being enforced */
	unsigned int rate;
=======
	/* Dynamic PCM BE runtime data */
	struct snd_soc_dpcm_runtime dpcm[2];

>>>>>>> refs/remotes/origin/cm-10.0
	long pmdown_time;

	/* runtime devices */
	struct snd_pcm *pcm;
=======
	unsigned int dev_registered:1;

	/* Dynamic PCM BE runtime data */
	struct snd_soc_dpcm_runtime dpcm[2];
	int fe_compr;
=======
	/* Dynamic PCM BE runtime data */
	struct snd_soc_dpcm_runtime dpcm[2];
>>>>>>> refs/remotes/origin/cm-11.0

	long pmdown_time;
	unsigned char pop_wait:1;

	/* runtime devices */
	struct snd_pcm *pcm;
	struct snd_compr *compr;
>>>>>>> refs/remotes/origin/master
	struct snd_soc_codec *codec;
	struct snd_soc_platform *platform;
	struct snd_soc_dai *codec_dai;
	struct snd_soc_dai *cpu_dai;

	struct delayed_work delayed_work;
<<<<<<< HEAD
<<<<<<< HEAD

#ifdef CONFIG_DEBUG_FS
<<<<<<< HEAD
	struct dentry *debugfs_dsp_root;
=======
	struct dentry *debugfs_dpcm_root;
	struct dentry *debugfs_dpcm_state;
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_dpcm_root;
	struct dentry *debugfs_dpcm_state;
>>>>>>> refs/remotes/origin/master
=======

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_dpcm_root;
	struct dentry *debugfs_dpcm_state;
>>>>>>> refs/remotes/origin/cm-11.0
#endif
};

/* mixer control */
struct soc_mixer_control {
	int min, max, platform_max;
<<<<<<< HEAD
	unsigned int reg, rreg, shift, rshift, invert;
};
<<<<<<< HEAD
<<<<<<< HEAD

/* mixer multiple input control */
=======
>>>>>>> refs/remotes/origin/cm-10.0
struct soc_multi_mixer_control {
	int min, max, platform_max, count;
	unsigned int reg, rreg, shift, rshift, invert;
};

<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======
	int reg, rreg;
	unsigned int shift, rshift;
	unsigned int invert:1;
	unsigned int autodisable:1;
};

struct soc_bytes {
	int base;
	int num_regs;
	u32 mask;
};

/* multi register control */
struct soc_mreg_control {
	long min, max;
	unsigned int regbase, regcount, nbits, invert;
};

>>>>>>> refs/remotes/origin/master
=======
struct soc_multi_mixer_control {
	int min, max, platform_max, count;
	unsigned int reg, rreg, shift, rshift, invert;
};


>>>>>>> refs/remotes/origin/cm-11.0
/* enumerated kcontrol */
struct soc_enum {
	unsigned short reg;
	unsigned short reg2;
	unsigned char shift_l;
	unsigned char shift_r;
	unsigned int max;
	unsigned int mask;
	const char * const *texts;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	char **dtexts;
	const unsigned int *values;
	void *dapm;
=======
	const unsigned int *values;
>>>>>>> refs/remotes/origin/master
};

/* codec IO */
unsigned int snd_soc_read(struct snd_soc_codec *codec, unsigned int reg);
unsigned int snd_soc_write(struct snd_soc_codec *codec,
			   unsigned int reg, unsigned int val);
<<<<<<< HEAD
unsigned int snd_soc_bulk_write_raw(struct snd_soc_codec *codec,
				    unsigned int reg, const void *data, size_t len);
=======
>>>>>>> refs/remotes/origin/master

/* device driver data */

static inline void snd_soc_card_set_drvdata(struct snd_soc_card *card,
		void *data)
{
	card->drvdata = data;
}

static inline void *snd_soc_card_get_drvdata(struct snd_soc_card *card)
{
	return card->drvdata;
}

static inline void snd_soc_codec_set_drvdata(struct snd_soc_codec *codec,
		void *data)
{
	dev_set_drvdata(codec->dev, data);
}

static inline void *snd_soc_codec_get_drvdata(struct snd_soc_codec *codec)
{
	return dev_get_drvdata(codec->dev);
}

static inline void snd_soc_platform_set_drvdata(struct snd_soc_platform *platform,
		void *data)
{
	dev_set_drvdata(platform->dev, data);
}

static inline void *snd_soc_platform_get_drvdata(struct snd_soc_platform *platform)
{
	return dev_get_drvdata(platform->dev);
}

static inline void snd_soc_pcm_set_drvdata(struct snd_soc_pcm_runtime *rtd,
		void *data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	dev_set_drvdata(&rtd->dev, data);
=======
	dev_set_drvdata(rtd->dev, data);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev_set_drvdata(rtd->dev, data);
>>>>>>> refs/remotes/origin/master
}

static inline void *snd_soc_pcm_get_drvdata(struct snd_soc_pcm_runtime *rtd)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return dev_get_drvdata(&rtd->dev);
=======
	return dev_get_drvdata(rtd->dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return dev_get_drvdata(rtd->dev);
>>>>>>> refs/remotes/origin/master
}

static inline void snd_soc_initialize_card_lists(struct snd_soc_card *card)
{
	INIT_LIST_HEAD(&card->dai_dev_list);
	INIT_LIST_HEAD(&card->codec_dev_list);
	INIT_LIST_HEAD(&card->platform_dev_list);
	INIT_LIST_HEAD(&card->widgets);
	INIT_LIST_HEAD(&card->paths);
	INIT_LIST_HEAD(&card->dapm_list);
}

<<<<<<< HEAD
<<<<<<< HEAD
int snd_soc_util_init(void);
void snd_soc_util_exit(void);

=======
=======
>>>>>>> refs/remotes/origin/master
static inline bool snd_soc_volsw_is_stereo(struct soc_mixer_control *mc)
{
	if (mc->reg == mc->rreg && mc->shift == mc->rshift)
		return 0;
	/*
	 * mc->reg == mc->rreg && mc->shift != mc->rshift, or
	 * mc->reg != mc->rreg means that the control is
	 * stereo (bits in one register or in two registers)
	 */
	return 1;
}

int snd_soc_util_init(void);
void snd_soc_util_exit(void);

int snd_soc_of_parse_card_name(struct snd_soc_card *card,
			       const char *propname);
int snd_soc_of_parse_audio_routing(struct snd_soc_card *card,
				   const char *propname);
<<<<<<< HEAD

>>>>>>> refs/remotes/origin/cm-10.0
=======
unsigned int snd_soc_of_parse_daifmt(struct device_node *np,
				     const char *prefix);
int snd_soc_of_get_dai_name(struct device_node *of_node,
			    const char **dai_name);

>>>>>>> refs/remotes/origin/master
#include <sound/soc-dai.h>

#ifdef CONFIG_DEBUG_FS
extern struct dentry *snd_soc_debugfs_root;
#endif

extern const struct dev_pm_ops snd_soc_pm_ops;

#endif
