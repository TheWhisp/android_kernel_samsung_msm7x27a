/* Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

#include <linux/delay.h>
#include <linux/module.h>
#include <mach/gpio.h>
#include <mach/pmic.h>
#include <mach/vreg.h>
#include "msm_fb.h"

#ifdef CONFIG_FB_MSM_TRY_MDDI_CATCH_LCDC_PRISM
#include "mddihosti.h"
#endif

#include "lcdc_s6d16a0x_jena.h"
#include "lcdc_s6d_backlight.h"


#define LCDC_DEBUG

#ifdef LCDC_DEBUG
#define DPRINT(x...)	printk("s6d16a0x_JENA " x)
#else
#define DPRINT(x...)	
#endif

//#define RDDIDIF			0x04		// 4 parameters
#define RDID1			0xDA		// 2 parameters
#define RDID2			0xDB		// 2 parameters
#define RDID3			0xDC		// 2 parameters

#define POWER_ON_SETTINGS(a)	(int)(sizeof(a)/sizeof(struct setting_table))
#define POWER_OFF_SETTINGS	(int)(sizeof(power_off_setting_table)/sizeof(struct setting_table))



static int spi_cs;
static int spi_sclk;
static int spi_sdi;

static int lcd_reset;

struct s6d16a0x_state_type {
	boolean disp_initialized;
	boolean display_on;
	boolean disp_powered_up;
};

static struct s6d16a0x_state_type s6d16a0x_state = { 0 };
static struct msm_panel_common_pdata *lcdc_s6d16a0x_pdata;

static int lcd_prf = 0;

extern int board_hw_revision;

#if 0
int lcd_on_state_for_debug;
EXPORT_SYMBOL(lcd_on_state_for_debug);
#endif

static DEFINE_SPINLOCK(lcd_ctrl_irq_lock);
static DEFINE_SEMAPHORE(backlight_sem);

#define DEFAULT_USLEEP	1	

#if 0
static void read_ldi_register(u8 addr, u8 *buf, int count)
{
	long i, j;

	gpio_set_value(spi_cs, 1);
	udelay(DEFAULT_USLEEP);
	gpio_set_value(spi_sclk, 1); 
	udelay(DEFAULT_USLEEP);

	/* Write Command */
	gpio_set_value(spi_cs, 0);
	udelay(DEFAULT_USLEEP);
	gpio_set_value(spi_sclk, 0); 
	udelay(DEFAULT_USLEEP);		
	gpio_set_value(spi_sdi, 0); 
	udelay(DEFAULT_USLEEP);
	
	gpio_set_value(spi_sclk, 1); 
	udelay(DEFAULT_USLEEP); 

   	for (i = 7; i >= 0; i--) { 
		gpio_set_value(spi_sclk, 0);
		udelay(DEFAULT_USLEEP);
		if ((addr >> i) & 0x1)
			gpio_set_value(spi_sdi, 1);
		else
			gpio_set_value(spi_sdi, 0);
		udelay(DEFAULT_USLEEP);	
		gpio_set_value(spi_sclk, 1);
		udelay(DEFAULT_USLEEP);	
	}

	//swith input
	gpio_direction_input(spi_sdi);

	if(count > 1) {
		//dummy clock cycle
		gpio_set_value(spi_sclk, 0);
		udelay(DEFAULT_USLEEP); 	
		gpio_set_value(spi_sclk, 1);
		udelay(DEFAULT_USLEEP);	
	}

	/* Read Parameter */
	if (count > 0) {
		for (j = 0; j < count; j++) {

			for (i = 7; i >= 0; i--) { 
				gpio_set_value(spi_sclk, 0);
				udelay(DEFAULT_USLEEP);	
				// read bit
				if(gpio_get_value(spi_sdi))
					buf[j] |= (0x1<<i);
				else
					buf[j] &= ~(0x1<<i);
				gpio_set_value(spi_sclk, 1);
				udelay(DEFAULT_USLEEP);	
			}

		}
	}

	gpio_set_value(spi_cs, 1);
	udelay(DEFAULT_USLEEP);	

	//switch output
	gpio_direction_output(spi_sdi, 0);
}
#endif

static void setting_table_write(struct setting_table *table)
{
	long i, j;
	unsigned long irqflags;
	spin_lock_irqsave(&lcd_ctrl_irq_lock, irqflags);

	gpio_set_value(spi_cs, 1);
	udelay(DEFAULT_USLEEP);
	gpio_set_value(spi_sclk, 1); 
	udelay(DEFAULT_USLEEP);

	/* Write Command */
	gpio_set_value(spi_cs, 0);
	udelay(DEFAULT_USLEEP);
	gpio_set_value(spi_sclk, 0); 
	udelay(DEFAULT_USLEEP);		
	gpio_set_value(spi_sdi, 0); 
	udelay(DEFAULT_USLEEP);
	
	gpio_set_value(spi_sclk, 1); 
	udelay(DEFAULT_USLEEP); 

   	for (i = 7; i >= 0; i--) { 
		gpio_set_value(spi_sclk, 0);
		udelay(DEFAULT_USLEEP);
		if ((table->command >> i) & 0x1)
			gpio_set_value(spi_sdi, 1);
		else
			gpio_set_value(spi_sdi, 0);
		udelay(DEFAULT_USLEEP);	
		gpio_set_value(spi_sclk, 1);
		udelay(DEFAULT_USLEEP);	
	}

	gpio_set_value(spi_cs, 1);
	udelay(DEFAULT_USLEEP);	

	/* Write Parameter */
	if ((table->parameters) > 0) {
		for (j = 0; j < table->parameters; j++) {
			gpio_set_value(spi_cs, 0); 
			udelay(DEFAULT_USLEEP); 	
			
			gpio_set_value(spi_sclk, 0); 
			udelay(DEFAULT_USLEEP); 	
			gpio_set_value(spi_sdi, 1); 
			udelay(DEFAULT_USLEEP);
			gpio_set_value(spi_sclk, 1); 
			udelay(DEFAULT_USLEEP); 	

			for (i = 7; i >= 0; i--) { 
				gpio_set_value(spi_sclk, 0);
				udelay(DEFAULT_USLEEP);	
				if ((table->parameter[j] >> i) & 0x1)
					gpio_set_value(spi_sdi, 1);
				else
					gpio_set_value(spi_sdi, 0);
				udelay(DEFAULT_USLEEP);	
				gpio_set_value(spi_sclk, 1);
				udelay(DEFAULT_USLEEP);					
			}

				gpio_set_value(spi_cs, 1);
				udelay(DEFAULT_USLEEP);	
		}
	}
	spin_unlock_irqrestore(&lcd_ctrl_irq_lock, irqflags);
	mdelay(table->wait);
}

static void spi_init(void)
{
	/* Set the output so that we dont disturb the slave device */
	gpio_set_value(spi_sclk, 0);
	gpio_set_value(spi_sdi, 0);

	/* Set the Chip Select De-asserted */
	gpio_set_value(spi_cs, 0);

}

#define VREG_ENABLE		1
#define VREG_DISABLE	0

static void s6d16a0x_vreg_config(int vreg_en)
{
	int rc;
	struct vreg *vreg_lcd =NULL;

	if (vreg_lcd == NULL) {
		vreg_lcd = vreg_get(NULL, "vlcd");

		if (IS_ERR(vreg_lcd)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "vlcd4", PTR_ERR(vreg_lcd));
			return;
		}

		rc = vreg_set_level(vreg_lcd, 3000);
		if (rc) {
			printk(KERN_ERR "%s: LCD set_level failed (%d)\n",
				__func__, rc);
		}
	}

	if (vreg_en) {
		rc = vreg_enable(vreg_lcd);
		if (rc) {
			printk(KERN_ERR "%s: LCD enable failed (%d)\n",
				 __func__, rc);
		}
	} else {
		rc = vreg_disable(vreg_lcd);
		if (rc) {
			printk(KERN_ERR "%s: LCD disable failed (%d)\n",
				 __func__, rc);
		}
	}
}

static void s6d16a0x_disp_powerup(void)
{
	DPRINT("start %s\n", __func__);	

	if (!s6d16a0x_state.disp_powered_up && !s6d16a0x_state.display_on) {

		s6d16a0x_vreg_config(VREG_ENABLE);
		msleep(10);
		gpio_tlmm_config(GPIO_CFG(lcd_reset, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL
			                      , GPIO_CFG_2MA), GPIO_CFG_ENABLE);
			
		gpio_set_value(lcd_reset, 0);
		udelay(50);
		gpio_set_value(lcd_reset, 1);
		udelay(50);
		gpio_set_value(lcd_reset, 0);
		udelay(50);
		gpio_set_value(lcd_reset, 1);
		msleep(10);		
		s6d16a0x_state.disp_powered_up = TRUE;
	}
}

static void s6d16a0x_disp_powerdown(void)
{
	DPRINT("start %s\n", __func__);	

		gpio_tlmm_config(GPIO_CFG(lcd_reset, 0, GPIO_CFG_OUTPUT
			                      , GPIO_CFG_NO_PULL
			                      , GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		gpio_set_value(lcd_reset, 0);

	s6d16a0x_vreg_config(VREG_DISABLE);
	msleep(1);

	s6d16a0x_state.disp_powered_up = FALSE;
}

static void s6d16a0x_disp_on(void)
{
	int i;

	DPRINT("start %s\n", __func__);	

	if (s6d16a0x_state.disp_powered_up && !s6d16a0x_state.display_on) {
		printk("HW rev is %d, apply %d's init sequence\n"
			    ,board_hw_revision,board_hw_revision);

		for (i = 0; i < POWER_ON_SETTINGS(power_on_setting_table_lsi); i++)
			setting_table_write(&power_on_setting_table_lsi[i]);			

		s6d16a0x_state.display_on = TRUE;
//		lcd_on_state_for_debug = TRUE;
	}
}

static int lcdc_s6d16a0x_panel_on(struct platform_device *pdev)
{
	DPRINT("start %s\n", __func__);	

	if (!s6d16a0x_state.disp_initialized) {
		/* Configure reset GPIO that drives DAC */
		lcdc_s6d16a0x_pdata->panel_config_gpio(1);
		s6d16a0x_disp_powerup();
		spi_init();	/* LCD needs SPI */

		s6d16a0x_disp_on();
		
#if 0 //def LCD_DET_ENABLE
		if (irq_disabled) {      
			enable_irq(MSM_GPIO_TO_INT(GPIO_LCD_DET));
			irq_disabled = FALSE;
			printk("%s - irq-disabled is changed to %d, and ESD_count is %d\n"
				   , __func__, irq_disabled, ESD_count );
		}
#endif
		s6d16a0x_state.disp_initialized = TRUE;
	}
	return 0;
}

static int lcdc_s6d16a0x_panel_off(struct platform_device *pdev)
{
	int i;

	DPRINT("start %s\n", __func__);	

	if (s6d16a0x_state.disp_powered_up && s6d16a0x_state.display_on) {
#if 0 //def LCD_DET_ENABLE
  		disable_irq_nosync(MSM_GPIO_TO_INT(GPIO_LCD_DET));
  		irq_disabled = TRUE;
		printk ( "%s - irq-disabled is changed to %d\n", __func__, irq_disabled );
#endif

		for (i = 0; i < POWER_OFF_SETTINGS; i++)
			setting_table_write(&power_off_setting_table[i]);	
		lcdc_s6d16a0x_pdata->panel_config_gpio(0);
		//msleep(120);
		s6d16a0x_state.display_on = FALSE;
		//lcd_on_state_for_debug = FALSE;
		s6d16a0x_state.disp_initialized = FALSE;
		s6d16a0x_disp_powerdown();
		lcd_prf = 0;
	}

	return 0;
}

static void lcdc_s6d16a0x_set_backlight(struct msm_fb_data_type *mfd)
{
	int bl_value = mfd->bl_level;
	static int lockup_count = 0;

	up(&backlight_sem);
	printk("[BACLKIGHT] : %d\n",bl_value);
	if(!bl_value) {
		// Turn off Backlight, don't check disp_initialized value */
		lcd_prf = 1;

	} else {
		if(lcd_prf)
			return;

		while(!s6d16a0x_state.disp_initialized) {
			msleep(100);
			lockup_count++;

			if(lockup_count > 50) {
				printk("Prevent infinite loop(wait for 5000ms)\n");
				printk("LCD can't initialize with in %d ms\n", lockup_count*100);
				lockup_count = 0; // init lockup_count;

				down(&backlight_sem);
				return;
			}
		}
	}

	lcdc_s6d_set_brightness_by_ktd259(bl_value);

	down(&backlight_sem);
}

static int __devinit s6d16a0x_probe(struct platform_device *pdev)
{
	if (pdev->id == 0) {
		s6d16a0x_state.disp_initialized = TRUE;
		s6d16a0x_state.disp_powered_up = TRUE;
		s6d16a0x_state.display_on = TRUE;
		
		lcdc_s6d16a0x_pdata = pdev->dev.platform_data;
		spi_sclk = *(lcdc_s6d16a0x_pdata->gpio_num);
		spi_cs   = *(lcdc_s6d16a0x_pdata->gpio_num + 1);
		spi_sdi  = *(lcdc_s6d16a0x_pdata->gpio_num + 2);
		lcd_reset= *(lcdc_s6d16a0x_pdata->gpio_num + 3);

		spi_init();
		
#if 0 //def LCD_DET_ENABLE
		gpio_tlmm_config(GPIO_CFG(GPIO_LCD_DET, 0, GPIO_CFG_INPUT, 
		                         GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		                 GPIO_CFG_ENABLE );

		err = request_irq(MSM_GPIO_TO_INT(GPIO_LCD_DET), 
			              s6d16a0x_esd_irq_handler, IRQF_TRIGGER_RISING, 
			              "LCD_ESD_DET", (void*)pdev->dev.platform_data );
		if (err) {
			printk ( "%s, request_irq failed %d(ESD_DET), ret= %d\n", __func__,
				     GPIO_LCD_DET, err );
		} else {
			printk ( "%s - irq is requested normally\n", __func__ );
		}
#endif

		return 0;
	}
	
	msm_fb_add_device(pdev);

	/* lcdc_s6d16a0x_panel_on(pdev); */
	
	return 0;
}

static void s6d16a0x_shutdown(struct platform_device *pdev)
{
	lcdc_s6d_set_brightness_by_ktd259(0);
	lcdc_s6d16a0x_panel_off(pdev);
}

static struct platform_driver this_driver = {
	.probe  = s6d16a0x_probe,
	.shutdown	= s6d16a0x_shutdown,
	.driver = {
		.name   = "lcdc_s6d16a0x_hvga",
	},
};

static struct msm_fb_panel_data s6d16a0x_panel_data = {
	.on = lcdc_s6d16a0x_panel_on,
	.off = lcdc_s6d16a0x_panel_off,
	.set_backlight = lcdc_s6d16a0x_set_backlight,
};

static struct platform_device this_device = {
	.name   = "lcdc_s6d16a0x_hvga",
	.id	= 1,
	.dev	= {
		.platform_data = &s6d16a0x_panel_data,
	}
};

#define LCDC_FB_XRES	320
#define LCDC_FB_YRES	480

#define LCDC_HBP		32//15
#define LCDC_HPW		4//5
#define LCDC_HFP		2//16//15
#define LCDC_VBP		7//8
#define LCDC_VPW		4//2
#define LCDC_VFP		18//8

#define LCDC_PCLK		(LCDC_FB_XRES + LCDC_HBP + LCDC_HPW + LCDC_HFP) \
	                     * (LCDC_FB_YRES + LCDC_VBP + LCDC_VPW + LCDC_VFP) * 60

static int __init lcdc_s6d16a0x_panel_init(void)
{
	int ret;
	struct msm_panel_info *pinfo;

#ifdef CONFIG_FB_MSM_TRY_MDDI_CATCH_LCDC_PRISM
	ret = msm_fb_detect_client("lcdc_s6d16a0x_hvga");
	if (ret) {
		printk(KERN_ERR "%s: msm_fb_detect_client failed!\n", __func__); 
		return 0;
	}
#endif

	ret = platform_driver_register(&this_driver);
	if (ret)
		return ret;

	pinfo = &s6d16a0x_panel_data.panel_info;
	pinfo->xres = LCDC_FB_XRES;
	pinfo->yres = LCDC_FB_YRES;
	MSM_FB_SINGLE_MODE_PANEL(pinfo);
	pinfo->type = LCDC_PANEL;
	pinfo->pdest = DISPLAY_1;
	pinfo->wait_cycle = 0;
	pinfo->bpp = 24;
	pinfo->fb_num = 2;
	pinfo->clk_rate = (16384 * 1000);//LCDC_PCLK;//(9388 * 1000);
	pinfo->bl_max = 255;
	pinfo->bl_min = 1;

	pinfo->lcdc.h_back_porch = LCDC_HBP;
	pinfo->lcdc.h_front_porch = LCDC_HFP;
	pinfo->lcdc.h_pulse_width = LCDC_HPW;
	pinfo->lcdc.v_back_porch = LCDC_VBP;
	pinfo->lcdc.v_front_porch = LCDC_VFP;
	pinfo->lcdc.v_pulse_width = LCDC_VPW;
	pinfo->lcdc.border_clr = 0;     /* blk */
	pinfo->lcdc.underflow_clr = 0xff;       /* blue */
	pinfo->lcdc.hsync_skew = 0;

	ret = platform_device_register(&this_device);
	if (ret) {
		printk(KERN_ERR "%s not able to register the device\n",
			 __func__);
		platform_driver_unregister(&this_driver);
	}
	return ret;
}

device_initcall(lcdc_s6d16a0x_panel_init);
