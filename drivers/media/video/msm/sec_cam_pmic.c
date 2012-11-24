
/***************************************************************
CAMERA Power control
****************************************************************/

#include "sec_cam_pmic.h"

#include <mach/gpio.h>
#include <linux/gpio.h>

#include <linux/clk.h>
#include <linux/io.h>
#include <mach/board.h>
#include <mach/msm_iomap.h>

#include <linux/regulator/consumer.h>
#include <mach/vreg.h>
#include <mach/camera.h>
#include "sec_cam_pmic.h"
#define CAM_TEST_REV03		/*temp, rev03 */

/*struct regulator *l6, *l15, *l17*/
struct regulator *l6, *l15, *l17;
struct regulator *s2, *s3;

/*
#ifdef CAM_TEST_REV03
struct regulator *l8, *l15, *l10, *lvs0,  *s2, *lvs1;//temp
#else
struct regulator *l8, *l15, *l24, *lvs0,  *s2, *lvs1;
#endif
*/

/* CAM power
	CAM_SENSOR_A_2.8		:  VREG_L17		: l17
	CAM_SENSOR_IO_1.8	: VREG_L15		: l15
	3M_CORE_1.2			: VREG_L6		: l6

	3M_AF_2.8			: VREG_L15A		: l15
	VT_CORE_1.8			: VREG_L10A		: 10
	VT_CAM_1.5			: VREG_L10A		: l10
*/
void cam_ldo_power_on2(void)
{
	int ret;
	pr_info("#### cam_ldo_power_on ####\n");
}


#if defined(CONFIG_SR300PC20)
/* CAM power
	CAM_SENSOR_A_2.8		:  VREG_L17		: l17
	CAM_SENSOR_IO_1.8	: VREG_L15		: l15
	3M_CORE_1.2			: VREG_L6		: l6
*/
void cam_ldo_power_on(void)
{
	int ret;
	struct vreg *l6, *l15, *l17;
	unsigned int mclk_cfg;
	pr_info("#### cam_ldo_power_on ####\n");

	l15 = vreg_get(NULL, "vcamio");

	if (!l15)
		pr_err("[SR300PC20]%s: VREG L15 get failed\n", __func__);

	if (vreg_set_level(l15, 1800))
		pr_err("[SR300PC20]%s: vreg_set_level failed\n", __func__);

	if (vreg_enable(l15))
		pr_err("[SR300PC20]%s: reg_enable failed\n", __func__);

	usleep(10);

	l17 = vreg_get(NULL, "vcama");

	if (!l17)
		pr_err("[SR300PC20]%s: VREG L17 get failed\n", __func__);

	if (vreg_set_level(l17, 2800))
		pr_err("[SR300PC20]%s: vreg_set_level failed\n", __func__);

	if (vreg_enable(l17))
		pr_err("![SR300PC20]%s: reg_enable failed\n", __func__);

	usleep(10);

	l6 = vreg_get(NULL, "vcamc");

	if (!l6)
		pr_err("[SR300PC20]%s: VREG L6 get failed\n", __func__);

	if (vreg_set_level(l6, 1200))
		pr_err("[SR300PC20]%s: vreg_set_level failed\n", __func__);

	if (vreg_enable(l6))
		pr_err("!![SR300PC20]%s:   reg_enable failed\n", __func__);

	usleep(10);
}

void cam_ldo_power_off(void)
{
	int ret;
	struct vreg *l6, *l15, *l17;

	pr_info("#### cam_ldo_power_off ####\n");

	l6 = vreg_get(NULL, "vcamc");
	l15 = vreg_get(NULL, "vcamio");
	l17 = vreg_get(NULL, "vcama");

	vreg_disable(l6);
	usleep(10);

	vreg_disable(l17);
	usleep(10);

	vreg_disable(l15);
	usleep(10);

	gpio_set_value_cansleep(CAM_IO_EN, LOW);

}
#else  /* no CONFIG_SR300PC20 */
void cam_ldo_power_on(void)
{
	int ret;
	struct vreg *l6, *l15, *l17;
	unsigned int mclk_cfg;
	pr_info("#### cam_ldo_power_on ####\n");

	l17 = vreg_get(NULL, "vcama");

	if (!l17)
		pr_info("[S5K5CCAF]%s: VREG L17 get failed\n", __func__);

	if (vreg_set_level(l17, 2800))
		pr_info("[S5K5CCAF]%s: vreg_set_level failed\n", __func__);

	if (vreg_enable(l17))
		pr_info("![S5K5CCAF]%s: reg_enable failed\n", __func__);

	usleep(10000);

	l6 = vreg_get(NULL, "vcamc");

	if (!l6)
		pr_info("[S5K5CCAF]%s: VREG L6 get failed\n", __func__);

	if (vreg_set_level(l6, 1200))
		pr_info("[S5K5CCAF]%s: vreg_set_level failed\n", __func__);

	if (vreg_enable(l6))
		pr_info("!![S5K5CCAF]%s:   reg_enable failed\n", __func__);

	usleep(10000);

	l15 = vreg_get(NULL, "vcamio");

	if (!l15)
		pr_info("[S5K5CCAF]%s: VREG L15 get failed\n", __func__);

	if (vreg_set_level(l15, 1800))
		pr_info("[S5K5CCAF]%s: vreg_set_level failed\n", __func__);

	if (vreg_enable(l15))
		pr_info("[S5K5CCAF]%s: reg_enable failed\n", __func__);

	usleep(10000);
}

void cam_ldo_power_off(void)
{
	int ret;
	struct vreg *l6, *l15, *l17;

	pr_info("#### cam_ldo_power_off ####\n");

	l6 = vreg_get(NULL, "vcamc");
	l15 = vreg_get(NULL, "vcama");
	l17 = vreg_get(NULL, "vcamio");

	vreg_disable(l6);
	usleep(1000);

	vreg_disable(l15);
	usleep(1000);

	vreg_disable(l17);
	usleep(1000);

	gpio_set_value_cansleep(CAM_IO_EN, LOW);

}
#endif	/* end CONFIG_SR300PC20 */


