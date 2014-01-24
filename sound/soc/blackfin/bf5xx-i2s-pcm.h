/*
<<<<<<< HEAD
 * linux/sound/arm/bf5xx-i2s-pcm.h -- ALSA PCM interface for the Blackfin
 *
 * Copyright 2007 Analog Device Inc.
 *
=======
>>>>>>> refs/remotes/origin/master
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

<<<<<<< HEAD
#ifndef _BF5XX_I2S_PCM_H
#define _BF5XX_I2S_PCM_H

struct bf5xx_pcm_dma_params {
	char *name;			/* stream identifier */
};

struct bf5xx_gpio {
	u32 sys;
	u32 rx;
	u32 tx;
	u32 clk;
	u32 frm;
=======
#ifndef _BF5XX_TDM_PCM_H
#define _BF5XX_TDM_PCM_H

#define BFIN_TDM_DAI_MAX_SLOTS 8

struct bf5xx_i2s_pcm_data {
	unsigned int map[BFIN_TDM_DAI_MAX_SLOTS];
	bool tdm_mode;
>>>>>>> refs/remotes/origin/master
};

#endif
