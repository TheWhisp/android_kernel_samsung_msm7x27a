#ifndef __ASM_SH_KFR2R09_H
#define __ASM_SH_KFR2R09_H

#include <video/sh_mobile_lcdc.h>

#if defined(CONFIG_FB_SH_MOBILE_LCDC) || defined(CONFIG_FB_SH_MOBILE_LCDC_MODULE)
<<<<<<< HEAD
void kfr2r09_lcd_on(void *board_data, struct fb_info *info);
void kfr2r09_lcd_off(void *board_data);
int kfr2r09_lcd_setup(void *board_data, void *sys_ops_handle,
		      struct sh_mobile_lcdc_sys_bus_ops *sys_ops);
void kfr2r09_lcd_start(void *board_data, void *sys_ops_handle,
		       struct sh_mobile_lcdc_sys_bus_ops *sys_ops);
#else
static void kfr2r09_lcd_on(void *board_data) {}
static void kfr2r09_lcd_off(void *board_data) {}
static int kfr2r09_lcd_setup(void *board_data, void *sys_ops_handle,
=======
void kfr2r09_lcd_on(void);
void kfr2r09_lcd_off(void);
int kfr2r09_lcd_setup(void *sys_ops_handle,
		      struct sh_mobile_lcdc_sys_bus_ops *sys_ops);
void kfr2r09_lcd_start(void *sys_ops_handle,
		       struct sh_mobile_lcdc_sys_bus_ops *sys_ops);
#else
static void kfr2r09_lcd_on(void) {}
static void kfr2r09_lcd_off(void) {}
static int kfr2r09_lcd_setup(void *sys_ops_handle,
>>>>>>> refs/remotes/origin/cm-10.0
				struct sh_mobile_lcdc_sys_bus_ops *sys_ops)
{
	return -ENODEV;
}
<<<<<<< HEAD
static void kfr2r09_lcd_start(void *board_data, void *sys_ops_handle,
=======
static void kfr2r09_lcd_start(void *sys_ops_handle,
>>>>>>> refs/remotes/origin/cm-10.0
				struct sh_mobile_lcdc_sys_bus_ops *sys_ops)
{
}
#endif

#endif /* __ASM_SH_KFR2R09_H */
