#ifndef __ASM_MACH_PXA168_H
#define __ASM_MACH_PXA168_H

<<<<<<< HEAD
struct sys_timer;

extern struct sys_timer pxa168_timer;
extern void __init pxa168_init_irq(void);
<<<<<<< HEAD
=======
extern void pxa168_restart(char, const char *);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/reboot.h>

extern void pxa168_timer_init(void);
extern void __init icu_init_irq(void);
extern void __init pxa168_init_irq(void);
extern void pxa168_restart(enum reboot_mode, const char *);
>>>>>>> refs/remotes/origin/master
extern void pxa168_clear_keypad_wakeup(void);

#include <linux/i2c.h>
#include <linux/i2c/pxa-i2c.h>
#include <mach/devices.h>
<<<<<<< HEAD
#include <plat/pxa3xx_nand.h>
#include <video/pxa168fb.h>
#include <plat/pxa27x_keypad.h>
#include <mach/cputype.h>
<<<<<<< HEAD

extern struct pxa_device_desc pxa168_device_uart1;
extern struct pxa_device_desc pxa168_device_uart2;
=======
#include <linux/pxa168_eth.h>
=======
#include <linux/platform_data/mtd-nand-pxa3xx.h>
#include <video/pxa168fb.h>
#include <linux/platform_data/keypad-pxa27x.h>
#include <mach/cputype.h>
#include <linux/pxa168_eth.h>
#include <linux/platform_data/mv_usb.h>
>>>>>>> refs/remotes/origin/master

extern struct pxa_device_desc pxa168_device_uart1;
extern struct pxa_device_desc pxa168_device_uart2;
extern struct pxa_device_desc pxa168_device_uart3;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
extern struct pxa_device_desc pxa168_device_twsi0;
extern struct pxa_device_desc pxa168_device_twsi1;
extern struct pxa_device_desc pxa168_device_pwm1;
extern struct pxa_device_desc pxa168_device_pwm2;
extern struct pxa_device_desc pxa168_device_pwm3;
extern struct pxa_device_desc pxa168_device_pwm4;
extern struct pxa_device_desc pxa168_device_ssp1;
extern struct pxa_device_desc pxa168_device_ssp2;
extern struct pxa_device_desc pxa168_device_ssp3;
extern struct pxa_device_desc pxa168_device_ssp4;
extern struct pxa_device_desc pxa168_device_ssp5;
extern struct pxa_device_desc pxa168_device_nand;
extern struct pxa_device_desc pxa168_device_fb;
extern struct pxa_device_desc pxa168_device_keypad;
<<<<<<< HEAD
<<<<<<< HEAD
=======
extern struct pxa_device_desc pxa168_device_eth;

struct pxa168_usb_pdata {
	/* If NULL, default phy init routine for PXA168 would be called */
	int (*phy_init)(void __iomem *usb_phy_reg_base);
};
/* pdata can be NULL */
int __init pxa168_add_usb_host(struct pxa168_usb_pdata *pdata);

extern struct platform_device pxa168_device_gpio;
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern struct pxa_device_desc pxa168_device_eth;

/* pdata can be NULL */
extern int __init pxa168_add_usb_host(struct mv_usb_platform_data *pdata);


extern struct platform_device pxa168_device_gpio;
>>>>>>> refs/remotes/origin/master

static inline int pxa168_add_uart(int id)
{
	struct pxa_device_desc *d = NULL;

	switch (id) {
	case 1: d = &pxa168_device_uart1; break;
	case 2: d = &pxa168_device_uart2; break;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	case 3: d = &pxa168_device_uart3; break;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case 3: d = &pxa168_device_uart3; break;
>>>>>>> refs/remotes/origin/master
	}

	if (d == NULL)
		return -EINVAL;

	return pxa_register_device(d, NULL, 0);
}

static inline int pxa168_add_twsi(int id, struct i2c_pxa_platform_data *data,
				  struct i2c_board_info *info, unsigned size)
{
	struct pxa_device_desc *d = NULL;
	int ret;

	switch (id) {
	case 0: d = &pxa168_device_twsi0; break;
	case 1: d = &pxa168_device_twsi1; break;
	default:
		return -EINVAL;
	}

	ret = i2c_register_board_info(id, info, size);
	if (ret)
		return ret;

	return pxa_register_device(d, data, sizeof(*data));
}

static inline int pxa168_add_pwm(int id)
{
	struct pxa_device_desc *d = NULL;

	switch (id) {
	case 1: d = &pxa168_device_pwm1; break;
	case 2: d = &pxa168_device_pwm2; break;
	case 3: d = &pxa168_device_pwm3; break;
	case 4: d = &pxa168_device_pwm4; break;
	default:
		return -EINVAL;
	}

	return pxa_register_device(d, NULL, 0);
}

static inline int pxa168_add_ssp(int id)
{
	struct pxa_device_desc *d = NULL;

	switch (id) {
	case 1: d = &pxa168_device_ssp1; break;
	case 2: d = &pxa168_device_ssp2; break;
	case 3: d = &pxa168_device_ssp3; break;
	case 4: d = &pxa168_device_ssp4; break;
	case 5: d = &pxa168_device_ssp5; break;
	default:
		return -EINVAL;
	}
	return pxa_register_device(d, NULL, 0);
}

static inline int pxa168_add_nand(struct pxa3xx_nand_platform_data *info)
{
	return pxa_register_device(&pxa168_device_nand, info, sizeof(*info));
}

static inline int pxa168_add_fb(struct pxa168fb_mach_info *mi)
{
	return pxa_register_device(&pxa168_device_fb, mi, sizeof(*mi));
}

static inline int pxa168_add_keypad(struct pxa27x_keypad_platform_data *data)
{
	if (cpu_is_pxa168())
		data->clear_wakeup_event = pxa168_clear_keypad_wakeup;

	return pxa_register_device(&pxa168_device_keypad, data, sizeof(*data));
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static inline int pxa168_add_eth(struct pxa168_eth_platform_data *data)
{
	return pxa_register_device(&pxa168_device_eth, data, sizeof(*data));
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif /* __ASM_MACH_PXA168_H */
