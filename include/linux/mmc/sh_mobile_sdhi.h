<<<<<<< HEAD
<<<<<<< HEAD
#ifndef __SH_MOBILE_SDHI_H__
#define __SH_MOBILE_SDHI_H__
=======
#ifndef LINUX_MMC_SH_MOBILE_SDHI_H
#define LINUX_MMC_SH_MOBILE_SDHI_H
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifndef LINUX_MMC_SH_MOBILE_SDHI_H
#define LINUX_MMC_SH_MOBILE_SDHI_H
>>>>>>> refs/remotes/origin/master

#include <linux/types.h>

struct platform_device;
<<<<<<< HEAD
struct tmio_mmc_data;

<<<<<<< HEAD
=======
=======

>>>>>>> refs/remotes/origin/master
#define SH_MOBILE_SDHI_IRQ_CARD_DETECT	"card_detect"
#define SH_MOBILE_SDHI_IRQ_SDCARD	"sdcard"
#define SH_MOBILE_SDHI_IRQ_SDIO		"sdio"

/**
 * struct sh_mobile_sdhi_ops - SDHI driver callbacks
 * @cd_wakeup:		trigger a card-detection run
 */
struct sh_mobile_sdhi_ops {
	void (*cd_wakeup)(const struct platform_device *pdev);
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct sh_mobile_sdhi_info {
	int dma_slave_tx;
	int dma_slave_rx;
	unsigned long tmio_flags;
	unsigned long tmio_caps;
<<<<<<< HEAD
	u32 tmio_ocr_mask;	/* available MMC voltages */
<<<<<<< HEAD
	struct tmio_mmc_data *pdata;
	void (*set_pwr)(struct platform_device *pdev, int state);
	int (*get_cd)(struct platform_device *pdev);
};

#endif /* __SH_MOBILE_SDHI_H__ */
=======
	unsigned int cd_gpio;
	struct tmio_mmc_data *pdata;
	void (*set_pwr)(struct platform_device *pdev, int state);
	int (*get_cd)(struct platform_device *pdev);
=======
	unsigned long tmio_caps2;
	u32 tmio_ocr_mask;	/* available MMC voltages */
	unsigned int cd_gpio;
>>>>>>> refs/remotes/origin/master

	/* callbacks for board specific setup code */
	int (*init)(struct platform_device *pdev,
		    const struct sh_mobile_sdhi_ops *ops);
	void (*cleanup)(struct platform_device *pdev);
};

#endif /* LINUX_MMC_SH_MOBILE_SDHI_H */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
