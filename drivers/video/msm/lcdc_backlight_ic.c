/*
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 and only version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * END
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <linux/delay.h>
#include <mach/gpio.h>
#include "lcdc_backlight_ic.h"

static int lcd_brightness = -1;

#if defined(CONFIG_MACH_TREBON)
struct brt_value brt_table_aat[] = {
		{ 255,	7 }, /* Max */
		{ 244,	9 },
		{ 233,	10 },
		{ 222,	11 },
		{ 211,	12 },
		{ 200,	13 },
		{ 189,	14 },
		{ 178,	16 },
		{ 166,	18 },
		{ 154,	20 }, /* default */
		{ 141,	22 },
		{ 127,	23 },
		{ 113,	24 },
		{ 99,	25 },
		{ 85,	26 },
		{ 71,	27 },
		{ 57,	28 },
		{ 43,	29 },
		{ 30,	31 }, /* Min */
		{ 20,	31 }, /* Dimming */
		{ 0,	32 }, /* Off */
};
#elif defined(CONFIG_MACH_JENA)
#if defined(CONFIG_BACKLIGHT_KTD253)
struct brt_value brt_table_aat[] = {
		{ 255,	6 }, /* Max */
		{ 244,	7 },
		{ 233,	9 },
		{ 222,	11 },
		{ 211,	13 },
		{ 200,	15 },
		{ 189,	16 },
		{ 178,	17 },
		{ 166,	18 },
		{ 154,	19 }, /* default */
		{ 141,	21 },
		{ 127,	22 },
		{ 113,	23 },
		{ 99,	24 },
		{ 85,	25 },
		{ 71,	26 },
		{ 57,	27 },
		{ 43,	29 },
		{ 30,	31 }, /* Min */
		{ 20,	31 }, /* Dimming */
		{ 0,	32 }, /* Off */
};
#else
struct brt_value brt_table_aat[] = {
		{ 255,	7 }, /* Max */
		{ 244,	9 },
		{ 233,	10 },
		{ 222,	11 },
		{ 211,	12 },
		{ 200,	13 },
		{ 189,	14 },
		{ 178,	16 },
		{ 166,	18 },
		{ 154,	20 }, /* default */
		{ 141,	22 },
		{ 127,	23 },
		{ 113,	24 },
		{ 99,	25 },
		{ 85,	26 },
		{ 71,	27 },
		{ 57,	28 },
		{ 43,	29 },
		{ 30,	31 }, /* Min */
		{ 20,	31 }, /* Dimming */
		{ 0,	32 }, /* Off */
};
#endif
#elif defined(CONFIG_MACH_GEIM)
struct brt_value brt_table_aat[] = {
		{ 255,	1 }, /* Max */
		{ 244,	3 },
		{ 233,	4 },
		{ 222,	6 },
		{ 211,	8 },
		{ 200,	10 },
		{ 189,	12 },
		{ 178,	14 },
		{ 166,	16 },
		{ 154,	17 }, /* default */
		{ 141,	18 },
		{ 127,	20 },
		{ 113,	22 },
		{ 99,	24 },
		{ 85,	26 },
		{ 71,	28 },
		{ 57,	29 },
		{ 43,	30 },
		{ 30,	31 }, /* Min */
		{ 20,	31 }, /* Dimming */
		{ 0,	32 }, /* Off */
};
#endif
#define MAX_BRT_STAGE_AAT (int)(sizeof(brt_table_aat)/sizeof(struct brt_value))

static DEFINE_SPINLOCK(bl_ctrl_lock);

void aat1401_set_brightness(int level)
{
	int tune_level = 0;
	int i;

	spin_lock(&bl_ctrl_lock);
	if (level > 0) {
		if (level < MIN_BRIGHTNESS_VALUE) {
			tune_level = AAT_DIMMING_VALUE; /* DIMMING */
		} else {
			for (i = 0; i < MAX_BRT_STAGE_AAT - 1; i++) {
				if (level <= brt_table_aat[i].level
					&& level > brt_table_aat[i+1].level) {
					tune_level = brt_table_aat[i].tune_level;
					break;
				}
			}
		}
	} /*  BACKLIGHT is KTD model */

	if (!tune_level) {
		gpio_set_value(GPIO_BL_CTRL, 0);
		mdelay(3);
	} else {
		for (; tune_level > 0; tune_level--) {
			gpio_set_value(GPIO_BL_CTRL, 0);
			udelay(3);
			gpio_set_value(GPIO_BL_CTRL, 1);
			udelay(3);
		}
	}
	mdelay(1);
	spin_unlock(&bl_ctrl_lock);
}

void ktd253_set_brightness(int level)
{
	int pulse;
	int tune_level = 0;
	int i;

	spin_lock(&bl_ctrl_lock);
	if (level > 0) {
		if (level < MIN_BRIGHTNESS_VALUE) {
			tune_level = AAT_DIMMING_VALUE; /* DIMMING */
		} else {
			for (i = 0; i < MAX_BRT_STAGE_AAT - 1; i++) {
				if (level <= brt_table_aat[i].level
					&& level > brt_table_aat[i+1].level) {
					tune_level = brt_table_aat[i].tune_level;
					break;
				}
			}
		}
	} /*  BACKLIGHT is KTD model */

	if (!tune_level) {
		gpio_set_value(GPIO_BL_CTRL, 0);
		mdelay(3);
		lcd_brightness = tune_level;
	} else {
		if (unlikely(lcd_brightness < 0)) {
			int val = gpio_get_value(GPIO_BL_CTRL);
			if (val) {
				lcd_brightness = 0;
			gpio_set_value(GPIO_BL_CTRL, 0);
			mdelay(3);
				printk(KERN_INFO "LCD Baklight init in boot time on kernel\n");
			}
		}
		if (!lcd_brightness) {
			gpio_set_value(GPIO_BL_CTRL, 1);
			udelay(3);
			lcd_brightness = MAX_BRIGHTNESS_IN_BLU;
		}

		pulse = (tune_level - lcd_brightness + MAX_BRIGHTNESS_IN_BLU)
						% MAX_BRIGHTNESS_IN_BLU;

		for (; pulse > 0; pulse--) {
			gpio_set_value(GPIO_BL_CTRL, 0);
			udelay(3);
			gpio_set_value(GPIO_BL_CTRL, 1);
			udelay(3);
		}

		lcd_brightness = tune_level;
	}
	mdelay(1);
	spin_unlock(&bl_ctrl_lock);
}

void backlight_ic_set_brightness(int level)
{
#if defined(CONFIG_MACH_TREBON) || defined(CONFIG_MACH_GEIM)
	ktd253_set_brightness(level);
#elif defined(CONFIG_MACH_JENA)
#if defined(CONFIG_BACKLIGHT_KTD253)
	ktd253_set_brightness(level);
#else
	aat1401_set_brightness(level);
#endif
#endif
}
