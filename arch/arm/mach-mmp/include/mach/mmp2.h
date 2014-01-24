#ifndef __ASM_MACH_MMP2_H
#define __ASM_MACH_MMP2_H

<<<<<<< HEAD
<<<<<<< HEAD
#include <plat/sdhci.h>
=======
#include <linux/platform_data/pxa_sdhci.h>
>>>>>>> refs/remotes/origin/cm-10.0

struct sys_timer;

extern struct sys_timer mmp2_timer;
=======
#include <linux/platform_data/pxa_sdhci.h>

extern void mmp2_timer_init(void);
>>>>>>> refs/remotes/origin/master
extern void __init mmp2_init_icu(void);
extern void __init mmp2_init_irq(void);
extern void mmp2_clear_pmic_int(void);

#include <linux/i2c.h>
#include <linux/i2c/pxa-i2c.h>
#include <mach/devices.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <mach/sram.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/platform_data/dma-mmp_tdma.h>
>>>>>>> refs/remotes/origin/master

extern struct pxa_device_desc mmp2_device_uart1;
extern struct pxa_device_desc mmp2_device_uart2;
extern struct pxa_device_desc mmp2_device_uart3;
extern struct pxa_device_desc mmp2_device_uart4;
extern struct pxa_device_desc mmp2_device_twsi1;
extern struct pxa_device_desc mmp2_device_twsi2;
extern struct pxa_device_desc mmp2_device_twsi3;
extern struct pxa_device_desc mmp2_device_twsi4;
extern struct pxa_device_desc mmp2_device_twsi5;
extern struct pxa_device_desc mmp2_device_twsi6;
extern struct pxa_device_desc mmp2_device_sdh0;
extern struct pxa_device_desc mmp2_device_sdh1;
extern struct pxa_device_desc mmp2_device_sdh2;
extern struct pxa_device_desc mmp2_device_sdh3;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
extern struct pxa_device_desc mmp2_device_asram;
extern struct pxa_device_desc mmp2_device_isram;

extern struct platform_device mmp2_device_gpio;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static inline int mmp2_add_uart(int id)
{
	struct pxa_device_desc *d = NULL;

	switch (id) {
	case 1: d = &mmp2_device_uart1; break;
	case 2: d = &mmp2_device_uart2; break;
	case 3: d = &mmp2_device_uart3; break;
	case 4: d = &mmp2_device_uart4; break;
	default:
		return -EINVAL;
	}

	return pxa_register_device(d, NULL, 0);
}

static inline int mmp2_add_twsi(int id, struct i2c_pxa_platform_data *data,
				  struct i2c_board_info *info, unsigned size)
{
	struct pxa_device_desc *d = NULL;
	int ret;

	switch (id) {
	case 1: d = &mmp2_device_twsi1; break;
	case 2: d = &mmp2_device_twsi2; break;
	case 3: d = &mmp2_device_twsi3; break;
	case 4: d = &mmp2_device_twsi4; break;
	case 5: d = &mmp2_device_twsi5; break;
	case 6: d = &mmp2_device_twsi6; break;
	default:
		return -EINVAL;
	}

	ret = i2c_register_board_info(id - 1, info, size);
	if (ret)
		return ret;

	return pxa_register_device(d, data, sizeof(*data));
}

static inline int mmp2_add_sdhost(int id, struct sdhci_pxa_platdata *data)
{
	struct pxa_device_desc *d = NULL;

	switch (id) {
	case 0: d = &mmp2_device_sdh0; break;
	case 1: d = &mmp2_device_sdh1; break;
	case 2: d = &mmp2_device_sdh2; break;
	case 3: d = &mmp2_device_sdh3; break;
	default:
		return -EINVAL;
	}

	return pxa_register_device(d, data, sizeof(*data));
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static inline int mmp2_add_asram(struct sram_platdata *data)
{
	return pxa_register_device(&mmp2_device_asram, data, sizeof(*data));
}

static inline int mmp2_add_isram(struct sram_platdata *data)
{
	return pxa_register_device(&mmp2_device_isram, data, sizeof(*data));
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif /* __ASM_MACH_MMP2_H */

