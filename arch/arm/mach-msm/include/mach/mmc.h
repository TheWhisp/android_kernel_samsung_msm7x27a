/*
 *  arch/arm/include/asm/mach/mmc.h
 */
#ifndef ASMARM_MACH_MMC_H
#define ASMARM_MACH_MMC_H

#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/sdio_func.h>

<<<<<<< HEAD
struct embedded_sdio_data {
	struct sdio_cis cis;
	struct sdio_cccr cccr;
	struct sdio_embedded_func *funcs;
	int num_funcs;
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
struct msm_mmc_gpio {
	unsigned no;
	const char *name;
};

struct msm_mmc_gpio_data {
	struct msm_mmc_gpio *gpio;
	u8 size;
};

struct msm_mmc_platform_data {
	unsigned int ocr_mask;			/* available voltages */
	u32 (*translate_vdd)(struct device *, unsigned int);
	unsigned int (*status)(struct device *);
<<<<<<< HEAD
	struct embedded_sdio_data *embedded_sdio;
	int (*register_status_notify)(void (*callback)(int card_present, void *dev_id), void *dev_id);
	struct msm_mmc_gpio_data *gpio_data;
=======
	int (*register_status_notify)(void (*callback)(int card_present, void *dev_id), void *dev_id);
	struct msm_mmc_gpio_data *gpio_data;
	void (*init_card)(struct mmc_card *card);
>>>>>>> refs/remotes/origin/cm-10.0
};

#endif
