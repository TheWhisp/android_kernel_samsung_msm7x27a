/*
 * LP5521 LED chip driver.
 *
 * Copyright (C) 2010 Nokia Corporation
<<<<<<< HEAD
 *
 * Contact: Samu Onkalo <samu.p.onkalo@nokia.com>
=======
 * Copyright (C) 2012 Texas Instruments
 *
 * Contact: Samu Onkalo <samu.p.onkalo@nokia.com>
 *          Milo(Woogyom) Kim <milo.kim@ti.com>
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/ctype.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/leds.h>
#include <linux/leds-lp5521.h>
#include <linux/workqueue.h>
#include <linux/slab.h>

#define LP5521_PROGRAM_LENGTH		32	/* in bytes */

#define LP5521_MAX_LEDS			3	/* Maximum number of LEDs */
#define LP5521_MAX_ENGINES		3	/* Maximum number of engines */

#define LP5521_ENG_MASK_BASE		0x30	/* 00110000 */
#define LP5521_ENG_STATUS_MASK		0x07	/* 00000111 */

#define LP5521_CMD_LOAD			0x15	/* 00010101 */
#define LP5521_CMD_RUN			0x2a	/* 00101010 */
#define LP5521_CMD_DIRECT		0x3f	/* 00111111 */
#define LP5521_CMD_DISABLED		0x00	/* 00000000 */
=======
#include <linux/delay.h>
#include <linux/firmware.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/leds.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/platform_data/leds-lp55xx.h>
#include <linux/slab.h>
#include <linux/of.h>

#include "leds-lp55xx-common.h"

#define LP5521_PROGRAM_LENGTH		32
#define LP5521_MAX_LEDS			3
#define LP5521_CMD_DIRECT		0x3F
>>>>>>> refs/remotes/origin/master

/* Registers */
#define LP5521_REG_ENABLE		0x00
#define LP5521_REG_OP_MODE		0x01
#define LP5521_REG_R_PWM		0x02
#define LP5521_REG_G_PWM		0x03
#define LP5521_REG_B_PWM		0x04
#define LP5521_REG_R_CURRENT		0x05
#define LP5521_REG_G_CURRENT		0x06
#define LP5521_REG_B_CURRENT		0x07
#define LP5521_REG_CONFIG		0x08
<<<<<<< HEAD
#define LP5521_REG_R_CHANNEL_PC		0x09
#define LP5521_REG_G_CHANNEL_PC		0x0A
#define LP5521_REG_B_CHANNEL_PC		0x0B
#define LP5521_REG_STATUS		0x0C
#define LP5521_REG_RESET		0x0D
#define LP5521_REG_GPO			0x0E
=======
#define LP5521_REG_STATUS		0x0C
#define LP5521_REG_RESET		0x0D
>>>>>>> refs/remotes/origin/master
#define LP5521_REG_R_PROG_MEM		0x10
#define LP5521_REG_G_PROG_MEM		0x30
#define LP5521_REG_B_PROG_MEM		0x50

<<<<<<< HEAD
#define LP5521_PROG_MEM_BASE		LP5521_REG_R_PROG_MEM
#define LP5521_PROG_MEM_SIZE		0x20

/* Base register to set LED current */
#define LP5521_REG_LED_CURRENT_BASE	LP5521_REG_R_CURRENT

=======
/* Base register to set LED current */
#define LP5521_REG_LED_CURRENT_BASE	LP5521_REG_R_CURRENT
>>>>>>> refs/remotes/origin/master
/* Base register to set the brightness */
#define LP5521_REG_LED_PWM_BASE		LP5521_REG_R_PWM

/* Bits in ENABLE register */
#define LP5521_MASTER_ENABLE		0x40	/* Chip master enable */
#define LP5521_LOGARITHMIC_PWM		0x80	/* Logarithmic PWM adjustment */
#define LP5521_EXEC_RUN			0x2A
<<<<<<< HEAD
<<<<<<< HEAD

/* Bits in CONFIG register */
=======
#define LP5521_ENABLE_DEFAULT	\
	(LP5521_MASTER_ENABLE | LP5521_LOGARITHMIC_PWM)
#define LP5521_ENABLE_RUN_PROGRAM	\
	(LP5521_ENABLE_DEFAULT | LP5521_EXEC_RUN)

/* CONFIG register */
>>>>>>> refs/remotes/origin/master
#define LP5521_PWM_HF			0x40	/* PWM: 0 = 256Hz, 1 = 558Hz */
#define LP5521_PWRSAVE_EN		0x20	/* 1 = Power save mode */
#define LP5521_CP_MODE_OFF		0	/* Charge pump (CP) off */
#define LP5521_CP_MODE_BYPASS		8	/* CP forced to bypass mode */
#define LP5521_CP_MODE_1X5		0x10	/* CP forced to 1.5x mode */
#define LP5521_CP_MODE_AUTO		0x18	/* Automatic mode selection */
<<<<<<< HEAD
#define LP5521_R_TO_BATT		4	/* R out: 0 = CP, 1 = Vbat */
#define LP5521_CLK_SRC_EXT		0	/* Ext-clk source (CLK_32K) */
#define LP5521_CLK_INT			1	/* Internal clock */
#define LP5521_CLK_AUTO			2	/* Automatic clock selection */
=======
#define LP5521_ENABLE_DEFAULT	\
	(LP5521_MASTER_ENABLE | LP5521_LOGARITHMIC_PWM)
#define LP5521_ENABLE_RUN_PROGRAM	\
	(LP5521_ENABLE_DEFAULT | LP5521_EXEC_RUN)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define LP5521_R_TO_BATT		0x04	/* R out: 0 = CP, 1 = Vbat */
#define LP5521_CLK_INT			0x01	/* Internal clock */
#define LP5521_DEFAULT_CFG		\
	(LP5521_PWM_HF | LP5521_PWRSAVE_EN | LP5521_CP_MODE_AUTO)
>>>>>>> refs/remotes/origin/master

/* Status */
#define LP5521_EXT_CLK_USED		0x08

<<<<<<< HEAD
<<<<<<< HEAD
=======
/* default R channel current register value */
#define LP5521_REG_R_CURR_DEFAULT	0xAF

/* Pattern Mode */
#define PATTERN_OFF	0

>>>>>>> refs/remotes/origin/cm-10.0
struct lp5521_engine {
	int		id;
	u8		mode;
	u8		prog_page;
	u8		engine_mask;
};

struct lp5521_led {
	int			id;
	u8			chan_nr;
	u8			led_current;
	u8			max_current;
	struct led_classdev	cdev;
	struct work_struct	brightness_work;
	u8			brightness;
};

struct lp5521_chip {
	struct lp5521_platform_data *pdata;
	struct mutex		lock; /* Serialize control */
	struct i2c_client	*client;
	struct lp5521_engine	engines[LP5521_MAX_ENGINES];
	struct lp5521_led	leds[LP5521_MAX_LEDS];
	u8			num_channels;
	u8			num_leds;
};

static inline struct lp5521_led *cdev_to_led(struct led_classdev *cdev)
{
	return container_of(cdev, struct lp5521_led, cdev);
}

static inline struct lp5521_chip *engine_to_lp5521(struct lp5521_engine *engine)
{
	return container_of(engine, struct lp5521_chip,
			    engines[engine->id - 1]);
}

static inline struct lp5521_chip *led_to_lp5521(struct lp5521_led *led)
{
	return container_of(led, struct lp5521_chip,
			    leds[led->id]);
}

static void lp5521_led_brightness_work(struct work_struct *work);

static inline int lp5521_write(struct i2c_client *client, u8 reg, u8 value)
{
	return i2c_smbus_write_byte_data(client, reg, value);
}

static int lp5521_read(struct i2c_client *client, u8 reg, u8 *buf)
{
	s32 ret;

	ret = i2c_smbus_read_byte_data(client, reg);
	if (ret < 0)
		return -EIO;

	*buf = ret;
	return 0;
}

static int lp5521_set_engine_mode(struct lp5521_engine *engine, u8 mode)
{
	struct lp5521_chip *chip = engine_to_lp5521(engine);
	struct i2c_client *client = chip->client;
	int ret;
	u8 engine_state;

	/* Only transition between RUN and DIRECT mode are handled here */
	if (mode == LP5521_CMD_LOAD)
		return 0;

	if (mode == LP5521_CMD_DISABLED)
		mode = LP5521_CMD_DIRECT;

	ret = lp5521_read(client, LP5521_REG_OP_MODE, &engine_state);
<<<<<<< HEAD
=======
	if (ret < 0)
		return ret;
>>>>>>> refs/remotes/origin/cm-10.0

	/* set mode only for this engine */
	engine_state &= ~(engine->engine_mask);
	mode &= engine->engine_mask;
	engine_state |= mode;
<<<<<<< HEAD
	ret |= lp5521_write(client, LP5521_REG_OP_MODE, engine_state);

	return ret;
=======
	return lp5521_write(client, LP5521_REG_OP_MODE, engine_state);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int lp5521_load_program(struct lp5521_engine *eng, const u8 *pattern)
{
	struct lp5521_chip *chip = engine_to_lp5521(eng);
	struct i2c_client *client = chip->client;
	int ret;
	int addr;
	u8 mode;

	/* move current engine to direct mode and remember the state */
	ret = lp5521_set_engine_mode(eng, LP5521_CMD_DIRECT);
<<<<<<< HEAD
<<<<<<< HEAD
	/* Mode change requires min 500 us delay. 1 - 2 ms  with margin */
	usleep_range(1000, 2000);
	ret |= lp5521_read(client, LP5521_REG_OP_MODE, &mode);
=======
	if (ret)
		return ret;

	/* Mode change requires min 500 us delay. 1 - 2 ms  with margin */
	usleep_range(1000, 2000);
	ret = lp5521_read(client, LP5521_REG_OP_MODE, &mode);
	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (ret)
		return ret;

	/* Mode change requires min 500 us delay. 1 - 2 ms  with margin */
	usleep_range(1000, 2000);
	ret = lp5521_read(client, LP5521_REG_OP_MODE, &mode);
	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/cm-11.0

	/* For loading, all the engines to load mode */
	lp5521_write(client, LP5521_REG_OP_MODE, LP5521_CMD_DIRECT);
	/* Mode change requires min 500 us delay. 1 - 2 ms  with margin */
	usleep_range(1000, 2000);
	lp5521_write(client, LP5521_REG_OP_MODE, LP5521_CMD_LOAD);
	/* Mode change requires min 500 us delay. 1 - 2 ms  with margin */
	usleep_range(1000, 2000);

	addr = LP5521_PROG_MEM_BASE + eng->prog_page * LP5521_PROG_MEM_SIZE;
	i2c_smbus_write_i2c_block_data(client,
				addr,
				LP5521_PROG_MEM_SIZE,
				pattern);

<<<<<<< HEAD
<<<<<<< HEAD
	ret |= lp5521_write(client, LP5521_REG_OP_MODE, mode);
	return ret;
=======
	return lp5521_write(client, LP5521_REG_OP_MODE, mode);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return lp5521_write(client, LP5521_REG_OP_MODE, mode);
>>>>>>> refs/remotes/origin/cm-11.0
}

static int lp5521_set_led_current(struct lp5521_chip *chip, int led, u8 curr)
{
	return lp5521_write(chip->client,
		    LP5521_REG_LED_CURRENT_BASE + chip->leds[led].chan_nr,
		    curr);
}

static void lp5521_init_engine(struct lp5521_chip *chip)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(chip->engines); i++) {
		chip->engines[i].id = i + 1;
		chip->engines[i].engine_mask = LP5521_ENG_MASK_BASE >> (i * 2);
		chip->engines[i].prog_page = i;
	}
}

static int lp5521_configure(struct i2c_client *client)
{
	struct lp5521_chip *chip = i2c_get_clientdata(client);
	int ret;
<<<<<<< HEAD
=======
	u8 cfg;
>>>>>>> refs/remotes/origin/cm-10.0

	lp5521_init_engine(chip);

	/* Set all PWMs to direct control mode */
<<<<<<< HEAD
	ret = lp5521_write(client, LP5521_REG_OP_MODE, 0x3F);

	/* Enable auto-powersave, set charge pump to auto, red to battery */
	ret |= lp5521_write(client, LP5521_REG_CONFIG,
		LP5521_PWRSAVE_EN | LP5521_CP_MODE_AUTO | LP5521_R_TO_BATT);
=======
	ret = lp5521_write(client, LP5521_REG_OP_MODE, LP5521_CMD_DIRECT);

	cfg = chip->pdata->update_config ?
		: (LP5521_PWRSAVE_EN | LP5521_CP_MODE_AUTO | LP5521_R_TO_BATT);
	ret |= lp5521_write(client, LP5521_REG_CONFIG, cfg);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Initialize all channels PWM to zero -> leds off */
	ret |= lp5521_write(client, LP5521_REG_R_PWM, 0);
	ret |= lp5521_write(client, LP5521_REG_G_PWM, 0);
	ret |= lp5521_write(client, LP5521_REG_B_PWM, 0);

	/* Set engines are set to run state when OP_MODE enables engines */
	ret |= lp5521_write(client, LP5521_REG_ENABLE,
<<<<<<< HEAD
			LP5521_MASTER_ENABLE | LP5521_LOGARITHMIC_PWM |
			LP5521_EXEC_RUN);
=======
			LP5521_ENABLE_RUN_PROGRAM);
>>>>>>> refs/remotes/origin/cm-10.0
	/* enable takes 500us. 1 - 2 ms leaves some margin */
	usleep_range(1000, 2000);

	return ret;
}

static int lp5521_run_selftest(struct lp5521_chip *chip, char *buf)
{
	int ret;
	u8 status;

	ret = lp5521_read(chip->client, LP5521_REG_STATUS, &status);
	if (ret < 0)
		return ret;

	/* Check that ext clock is really in use if requested */
	if (chip->pdata && chip->pdata->clock_mode == LP5521_CLOCK_EXT)
		if  ((status & LP5521_EXT_CLK_USED) == 0)
			return -EIO;
	return 0;
}

static void lp5521_set_brightness(struct led_classdev *cdev,
			     enum led_brightness brightness)
{
	struct lp5521_led *led = cdev_to_led(cdev);
	led->brightness = (u8)brightness;
	schedule_work(&led->brightness_work);
}

static void lp5521_led_brightness_work(struct work_struct *work)
{
	struct lp5521_led *led = container_of(work,
					      struct lp5521_led,
					      brightness_work);
	struct lp5521_chip *chip = led_to_lp5521(led);
	struct i2c_client *client = chip->client;

	mutex_lock(&chip->lock);
	lp5521_write(client, LP5521_REG_LED_PWM_BASE + led->chan_nr,
		led->brightness);
	mutex_unlock(&chip->lock);
}

/* Detect the chip by setting its ENABLE register and reading it back. */
static int lp5521_detect(struct i2c_client *client)
{
	int ret;
	u8 buf;

<<<<<<< HEAD
	ret = lp5521_write(client, LP5521_REG_ENABLE,
			LP5521_MASTER_ENABLE | LP5521_LOGARITHMIC_PWM);
=======
	ret = lp5521_write(client, LP5521_REG_ENABLE, LP5521_ENABLE_DEFAULT);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret)
		return ret;
	/* enable takes 500us. 1 - 2 ms leaves some margin */
	usleep_range(1000, 2000);
	ret = lp5521_read(client, LP5521_REG_ENABLE, &buf);
	if (ret)
		return ret;
<<<<<<< HEAD
	if (buf != (LP5521_MASTER_ENABLE | LP5521_LOGARITHMIC_PWM))
=======
	if (buf != LP5521_ENABLE_DEFAULT)
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENODEV;
=======
/* default R channel current register value */
#define LP5521_REG_R_CURR_DEFAULT	0xAF

/* Reset register value */
#define LP5521_RESET			0xFF

/* Program Memory Operations */
#define LP5521_MODE_R_M			0x30	/* Operation Mode Register */
#define LP5521_MODE_G_M			0x0C
#define LP5521_MODE_B_M			0x03
#define LP5521_LOAD_R			0x10
#define LP5521_LOAD_G			0x04
#define LP5521_LOAD_B			0x01

#define LP5521_R_IS_LOADING(mode)	\
	((mode & LP5521_MODE_R_M) == LP5521_LOAD_R)
#define LP5521_G_IS_LOADING(mode)	\
	((mode & LP5521_MODE_G_M) == LP5521_LOAD_G)
#define LP5521_B_IS_LOADING(mode)	\
	((mode & LP5521_MODE_B_M) == LP5521_LOAD_B)

#define LP5521_EXEC_R_M			0x30	/* Enable Register */
#define LP5521_EXEC_G_M			0x0C
#define LP5521_EXEC_B_M			0x03
#define LP5521_EXEC_M			0x3F
#define LP5521_RUN_R			0x20
#define LP5521_RUN_G			0x08
#define LP5521_RUN_B			0x02

static inline void lp5521_wait_opmode_done(void)
{
	/* operation mode change needs to be longer than 153 us */
	usleep_range(200, 300);
}

static inline void lp5521_wait_enable_done(void)
{
	/* it takes more 488 us to update ENABLE register */
	usleep_range(500, 600);
}

static void lp5521_set_led_current(struct lp55xx_led *led, u8 led_current)
{
	led->led_current = led_current;
	lp55xx_write(led->chip, LP5521_REG_LED_CURRENT_BASE + led->chan_nr,
		led_current);
}

static void lp5521_load_engine(struct lp55xx_chip *chip)
{
	enum lp55xx_engine_index idx = chip->engine_idx;
	u8 mask[] = {
		[LP55XX_ENGINE_1] = LP5521_MODE_R_M,
		[LP55XX_ENGINE_2] = LP5521_MODE_G_M,
		[LP55XX_ENGINE_3] = LP5521_MODE_B_M,
	};

	u8 val[] = {
		[LP55XX_ENGINE_1] = LP5521_LOAD_R,
		[LP55XX_ENGINE_2] = LP5521_LOAD_G,
		[LP55XX_ENGINE_3] = LP5521_LOAD_B,
	};

	lp55xx_update_bits(chip, LP5521_REG_OP_MODE, mask[idx], val[idx]);

	lp5521_wait_opmode_done();
}

static void lp5521_stop_engine(struct lp55xx_chip *chip)
{
	lp55xx_write(chip, LP5521_REG_OP_MODE, 0);
	lp5521_wait_opmode_done();
}

static void lp5521_run_engine(struct lp55xx_chip *chip, bool start)
{
	int ret;
	u8 mode;
	u8 exec;

	/* stop engine */
	if (!start) {
		lp5521_stop_engine(chip);
		lp55xx_write(chip, LP5521_REG_OP_MODE, LP5521_CMD_DIRECT);
		lp5521_wait_opmode_done();
		return;
	}

	/*
	 * To run the engine,
	 * operation mode and enable register should updated at the same time
	 */

	ret = lp55xx_read(chip, LP5521_REG_OP_MODE, &mode);
	if (ret)
		return;

	ret = lp55xx_read(chip, LP5521_REG_ENABLE, &exec);
	if (ret)
		return;

	/* change operation mode to RUN only when each engine is loading */
	if (LP5521_R_IS_LOADING(mode)) {
		mode = (mode & ~LP5521_MODE_R_M) | LP5521_RUN_R;
		exec = (exec & ~LP5521_EXEC_R_M) | LP5521_RUN_R;
	}

	if (LP5521_G_IS_LOADING(mode)) {
		mode = (mode & ~LP5521_MODE_G_M) | LP5521_RUN_G;
		exec = (exec & ~LP5521_EXEC_G_M) | LP5521_RUN_G;
	}

	if (LP5521_B_IS_LOADING(mode)) {
		mode = (mode & ~LP5521_MODE_B_M) | LP5521_RUN_B;
		exec = (exec & ~LP5521_EXEC_B_M) | LP5521_RUN_B;
	}

	lp55xx_write(chip, LP5521_REG_OP_MODE, mode);
	lp5521_wait_opmode_done();

	lp55xx_update_bits(chip, LP5521_REG_ENABLE, LP5521_EXEC_M, exec);
	lp5521_wait_enable_done();
}

static int lp5521_update_program_memory(struct lp55xx_chip *chip,
					const u8 *data, size_t size)
{
	enum lp55xx_engine_index idx = chip->engine_idx;
	u8 pattern[LP5521_PROGRAM_LENGTH] = {0};
	u8 addr[] = {
		[LP55XX_ENGINE_1] = LP5521_REG_R_PROG_MEM,
		[LP55XX_ENGINE_2] = LP5521_REG_G_PROG_MEM,
		[LP55XX_ENGINE_3] = LP5521_REG_B_PROG_MEM,
	};
	unsigned cmd;
	char c[3];
	int nrchars;
	int ret;
	int offset = 0;
	int i = 0;

	while ((offset < size - 1) && (i < LP5521_PROGRAM_LENGTH)) {
		/* separate sscanfs because length is working only for %s */
		ret = sscanf(data + offset, "%2s%n ", c, &nrchars);
		if (ret != 1)
			goto err;

		ret = sscanf(c, "%2x", &cmd);
		if (ret != 1)
			goto err;

		pattern[i] = (u8)cmd;
		offset += nrchars;
		i++;
	}

	/* Each instruction is 16bit long. Check that length is even */
	if (i % 2)
		goto err;

	for (i = 0; i < LP5521_PROGRAM_LENGTH; i++) {
		ret = lp55xx_write(chip, addr[idx] + i, pattern[i]);
		if (ret)
			return -EINVAL;
	}

	return size;

err:
	dev_err(&chip->cl->dev, "wrong pattern format\n");
	return -EINVAL;
}

static void lp5521_firmware_loaded(struct lp55xx_chip *chip)
{
	const struct firmware *fw = chip->fw;

	if (fw->size > LP5521_PROGRAM_LENGTH) {
		dev_err(&chip->cl->dev, "firmware data size overflow: %zu\n",
			fw->size);
		return;
	}

	/*
	 * Program momery sequence
	 *  1) set engine mode to "LOAD"
	 *  2) write firmware data into program memory
	 */

	lp5521_load_engine(chip);
	lp5521_update_program_memory(chip, fw->data, fw->size);
}

static int lp5521_post_init_device(struct lp55xx_chip *chip)
{
	int ret;
	u8 val;

	/*
	 * Make sure that the chip is reset by reading back the r channel
	 * current reg. This is dummy read is required on some platforms -
	 * otherwise further access to the R G B channels in the
	 * LP5521_REG_ENABLE register will not have any effect - strange!
	 */
	ret = lp55xx_read(chip, LP5521_REG_R_CURRENT, &val);
	if (ret) {
		dev_err(&chip->cl->dev, "error in resetting chip\n");
		return ret;
	}
	if (val != LP5521_REG_R_CURR_DEFAULT) {
		dev_err(&chip->cl->dev,
			"unexpected data in register (expected 0x%x got 0x%x)\n",
			LP5521_REG_R_CURR_DEFAULT, val);
		ret = -EINVAL;
		return ret;
	}
	usleep_range(10000, 20000);

	/* Set all PWMs to direct control mode */
	ret = lp55xx_write(chip, LP5521_REG_OP_MODE, LP5521_CMD_DIRECT);

	/* Update configuration for the clock setting */
	val = LP5521_DEFAULT_CFG;
	if (!lp55xx_is_extclk_used(chip))
		val |= LP5521_CLK_INT;

	ret = lp55xx_write(chip, LP5521_REG_CONFIG, val);
	if (ret)
		return ret;

	/* Initialize all channels PWM to zero -> leds off */
	lp55xx_write(chip, LP5521_REG_R_PWM, 0);
	lp55xx_write(chip, LP5521_REG_G_PWM, 0);
	lp55xx_write(chip, LP5521_REG_B_PWM, 0);

	/* Set engines are set to run state when OP_MODE enables engines */
	ret = lp55xx_write(chip, LP5521_REG_ENABLE, LP5521_ENABLE_RUN_PROGRAM);
	if (ret)
		return ret;

	lp5521_wait_enable_done();
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
/* Set engine mode and create appropriate sysfs attributes, if required. */
static int lp5521_set_mode(struct lp5521_engine *engine, u8 mode)
{
	int ret = 0;

	/* if in that mode already do nothing, except for run */
	if (mode == engine->mode && mode != LP5521_CMD_RUN)
		return 0;

	if (mode == LP5521_CMD_RUN) {
		ret = lp5521_set_engine_mode(engine, LP5521_CMD_RUN);
	} else if (mode == LP5521_CMD_LOAD) {
		lp5521_set_engine_mode(engine, LP5521_CMD_DISABLED);
		lp5521_set_engine_mode(engine, LP5521_CMD_LOAD);
	} else if (mode == LP5521_CMD_DISABLED) {
		lp5521_set_engine_mode(engine, LP5521_CMD_DISABLED);
	}

	engine->mode = mode;

	return ret;
}

static int lp5521_do_store_load(struct lp5521_engine *engine,
				const char *buf, size_t len)
{
	struct lp5521_chip *chip = engine_to_lp5521(engine);
	struct i2c_client *client = chip->client;
	int  ret, nrchars, offset = 0, i = 0;
	char c[3];
	unsigned cmd;
	u8 pattern[LP5521_PROGRAM_LENGTH] = {0};

	while ((offset < len - 1) && (i < LP5521_PROGRAM_LENGTH)) {
		/* separate sscanfs because length is working only for %s */
		ret = sscanf(buf + offset, "%2s%n ", c, &nrchars);
		if (ret != 2)
			goto fail;
		ret = sscanf(c, "%2x", &cmd);
		if (ret != 1)
			goto fail;
		pattern[i] = (u8)cmd;

		offset += nrchars;
		i++;
	}

	/* Each instruction is 16bit long. Check that length is even */
	if (i % 2)
		goto fail;

	mutex_lock(&chip->lock);
	if (engine->mode == LP5521_CMD_LOAD)
		ret = lp5521_load_program(engine, pattern);
	else
		ret = -EINVAL;
	mutex_unlock(&chip->lock);

	if (ret) {
		dev_err(&client->dev, "failed loading pattern\n");
		return ret;
	}

	return len;
fail:
	dev_err(&client->dev, "wrong pattern format\n");
	return -EINVAL;
}

static ssize_t store_engine_load(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t len, int nr)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct lp5521_chip *chip = i2c_get_clientdata(client);
	return lp5521_do_store_load(&chip->engines[nr - 1], buf, len);
}

#define store_load(nr)							\
static ssize_t store_engine##nr##_load(struct device *dev,		\
				     struct device_attribute *attr,	\
				     const char *buf, size_t len)	\
{									\
	return store_engine_load(dev, attr, buf, len, nr);		\
}
store_load(1)
store_load(2)
store_load(3)

=======
static int lp5521_run_selftest(struct lp55xx_chip *chip, char *buf)
{
	struct lp55xx_platform_data *pdata = chip->pdata;
	int ret;
	u8 status;

	ret = lp55xx_read(chip, LP5521_REG_STATUS, &status);
	if (ret < 0)
		return ret;

	if (pdata->clock_mode != LP55XX_CLOCK_EXT)
		return 0;

	/* Check that ext clock is really in use if requested */
	if  ((status & LP5521_EXT_CLK_USED) == 0)
		return -EIO;

	return 0;
}

static void lp5521_led_brightness_work(struct work_struct *work)
{
	struct lp55xx_led *led = container_of(work, struct lp55xx_led,
					      brightness_work);
	struct lp55xx_chip *chip = led->chip;

	mutex_lock(&chip->lock);
	lp55xx_write(chip, LP5521_REG_LED_PWM_BASE + led->chan_nr,
		led->brightness);
	mutex_unlock(&chip->lock);
}

>>>>>>> refs/remotes/origin/master
static ssize_t show_engine_mode(struct device *dev,
				struct device_attribute *attr,
				char *buf, int nr)
{
<<<<<<< HEAD
	struct i2c_client *client = to_i2c_client(dev);
	struct lp5521_chip *chip = i2c_get_clientdata(client);
	switch (chip->engines[nr - 1].mode) {
	case LP5521_CMD_RUN:
		return sprintf(buf, "run\n");
	case LP5521_CMD_LOAD:
		return sprintf(buf, "load\n");
	case LP5521_CMD_DISABLED:
		return sprintf(buf, "disabled\n");
=======
	struct lp55xx_led *led = i2c_get_clientdata(to_i2c_client(dev));
	struct lp55xx_chip *chip = led->chip;
	enum lp55xx_engine_mode mode = chip->engines[nr - 1].mode;

	switch (mode) {
	case LP55XX_ENGINE_RUN:
		return sprintf(buf, "run\n");
	case LP55XX_ENGINE_LOAD:
		return sprintf(buf, "load\n");
	case LP55XX_ENGINE_DISABLED:
>>>>>>> refs/remotes/origin/master
	default:
		return sprintf(buf, "disabled\n");
	}
}
<<<<<<< HEAD

#define show_mode(nr)							\
static ssize_t show_engine##nr##_mode(struct device *dev,		\
				    struct device_attribute *attr,	\
				    char *buf)				\
{									\
	return show_engine_mode(dev, attr, buf, nr);			\
}
=======
>>>>>>> refs/remotes/origin/master
show_mode(1)
show_mode(2)
show_mode(3)

static ssize_t store_engine_mode(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t len, int nr)
{
<<<<<<< HEAD
	struct i2c_client *client = to_i2c_client(dev);
	struct lp5521_chip *chip = i2c_get_clientdata(client);
	struct lp5521_engine *engine = &chip->engines[nr - 1];
	mutex_lock(&chip->lock);

	if (!strncmp(buf, "run", 3))
		lp5521_set_mode(engine, LP5521_CMD_RUN);
	else if (!strncmp(buf, "load", 4))
		lp5521_set_mode(engine, LP5521_CMD_LOAD);
	else if (!strncmp(buf, "disabled", 8))
		lp5521_set_mode(engine, LP5521_CMD_DISABLED);

	mutex_unlock(&chip->lock);
	return len;
}

#define store_mode(nr)							\
static ssize_t store_engine##nr##_mode(struct device *dev,		\
				     struct device_attribute *attr,	\
				     const char *buf, size_t len)	\
{									\
	return store_engine_mode(dev, attr, buf, len, nr);		\
=======
	struct lp55xx_led *led = i2c_get_clientdata(to_i2c_client(dev));
	struct lp55xx_chip *chip = led->chip;
	struct lp55xx_engine *engine = &chip->engines[nr - 1];

	mutex_lock(&chip->lock);

	chip->engine_idx = nr;

	if (!strncmp(buf, "run", 3)) {
		lp5521_run_engine(chip, true);
		engine->mode = LP55XX_ENGINE_RUN;
	} else if (!strncmp(buf, "load", 4)) {
		lp5521_stop_engine(chip);
		lp5521_load_engine(chip);
		engine->mode = LP55XX_ENGINE_LOAD;
	} else if (!strncmp(buf, "disabled", 8)) {
		lp5521_stop_engine(chip);
		engine->mode = LP55XX_ENGINE_DISABLED;
	}

	mutex_unlock(&chip->lock);

	return len;
>>>>>>> refs/remotes/origin/master
}
store_mode(1)
store_mode(2)
store_mode(3)

<<<<<<< HEAD
static ssize_t show_max_current(struct device *dev,
			    struct device_attribute *attr,
			    char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct lp5521_led *led = cdev_to_led(led_cdev);

	return sprintf(buf, "%d\n", led->max_current);
}

static ssize_t show_current(struct device *dev,
			    struct device_attribute *attr,
			    char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct lp5521_led *led = cdev_to_led(led_cdev);

	return sprintf(buf, "%d\n", led->led_current);
}

static ssize_t store_current(struct device *dev,
			     struct device_attribute *attr,
			     const char *buf, size_t len)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct lp5521_led *led = cdev_to_led(led_cdev);
	struct lp5521_chip *chip = led_to_lp5521(led);
	ssize_t ret;
	unsigned long curr;

<<<<<<< HEAD
	if (strict_strtoul(buf, 0, &curr))
=======
	if (kstrtoul(buf, 0, &curr))
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;

	if (curr > led->max_current)
		return -EINVAL;

	mutex_lock(&chip->lock);
	ret = lp5521_set_led_current(chip, led->id, curr);
	mutex_unlock(&chip->lock);

	if (ret < 0)
		return ret;

	led->led_current = (u8)curr;

	return len;
}
=======
static ssize_t store_engine_load(struct device *dev,
			     struct device_attribute *attr,
			     const char *buf, size_t len, int nr)
{
	struct lp55xx_led *led = i2c_get_clientdata(to_i2c_client(dev));
	struct lp55xx_chip *chip = led->chip;
	int ret;

	mutex_lock(&chip->lock);

	chip->engine_idx = nr;
	lp5521_load_engine(chip);
	ret = lp5521_update_program_memory(chip, buf, len);

	mutex_unlock(&chip->lock);

	return ret;
}
store_load(1)
store_load(2)
store_load(3)
>>>>>>> refs/remotes/origin/master

static ssize_t lp5521_selftest(struct device *dev,
			       struct device_attribute *attr,
			       char *buf)
{
<<<<<<< HEAD
	struct i2c_client *client = to_i2c_client(dev);
	struct lp5521_chip *chip = i2c_get_clientdata(client);
=======
	struct lp55xx_led *led = i2c_get_clientdata(to_i2c_client(dev));
	struct lp55xx_chip *chip = led->chip;
>>>>>>> refs/remotes/origin/master
	int ret;

	mutex_lock(&chip->lock);
	ret = lp5521_run_selftest(chip, buf);
	mutex_unlock(&chip->lock);
<<<<<<< HEAD
	return sprintf(buf, "%s\n", ret ? "FAIL" : "OK");
}

<<<<<<< HEAD
=======
static void lp5521_clear_program_memory(struct i2c_client *cl)
{
	int i;
	u8 rgb_mem[] = {
		LP5521_REG_R_PROG_MEM,
		LP5521_REG_G_PROG_MEM,
		LP5521_REG_B_PROG_MEM,
	};

	for (i = 0; i < ARRAY_SIZE(rgb_mem); i++) {
		lp5521_write(cl, rgb_mem[i], 0);
		lp5521_write(cl, rgb_mem[i] + 1, 0);
	}
}

static void lp5521_write_program_memory(struct i2c_client *cl,
				u8 base, u8 *rgb, int size)
{
	int i;

	if (!rgb || size <= 0)
		return;

	for (i = 0; i < size; i++)
		lp5521_write(cl, base + i, *(rgb + i));

	lp5521_write(cl, base + i, 0);
	lp5521_write(cl, base + i + 1, 0);
}

static inline struct lp5521_led_pattern *lp5521_get_pattern
					(struct lp5521_chip *chip, u8 offset)
{
	struct lp5521_led_pattern *ptn;
	ptn = chip->pdata->patterns + (offset - 1);
	return ptn;
}

static void lp5521_run_led_pattern(int mode, struct lp5521_chip *chip)
{
	struct lp5521_led_pattern *ptn;
	struct i2c_client *cl = chip->client;
	int num_patterns = chip->pdata->num_patterns;

	if (mode > num_patterns || !(chip->pdata->patterns))
		return;

	if (mode == PATTERN_OFF) {
		lp5521_write(cl, LP5521_REG_ENABLE, LP5521_ENABLE_DEFAULT);
		usleep_range(1000, 2000);
		lp5521_write(cl, LP5521_REG_OP_MODE, LP5521_CMD_DIRECT);
	} else {
		ptn = lp5521_get_pattern(chip, mode);
		if (!ptn)
			return;

		lp5521_write(cl, LP5521_REG_OP_MODE, LP5521_CMD_LOAD);
		usleep_range(1000, 2000);

		lp5521_clear_program_memory(cl);

		lp5521_write_program_memory(cl, LP5521_REG_R_PROG_MEM,
					ptn->r, ptn->size_r);
		lp5521_write_program_memory(cl, LP5521_REG_G_PROG_MEM,
					ptn->g, ptn->size_g);
		lp5521_write_program_memory(cl, LP5521_REG_B_PROG_MEM,
					ptn->b, ptn->size_b);

		lp5521_write(cl, LP5521_REG_OP_MODE, LP5521_CMD_RUN);
		usleep_range(1000, 2000);
		lp5521_write(cl, LP5521_REG_ENABLE, LP5521_ENABLE_RUN_PROGRAM);
	}
}

static ssize_t store_led_pattern(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t len)
{
	struct lp5521_chip *chip = i2c_get_clientdata(to_i2c_client(dev));
	unsigned long val;
	int ret;

	ret = strict_strtoul(buf, 16, &val);
	if (ret)
		return ret;

	lp5521_run_led_pattern(val, chip);

	return len;
}

>>>>>>> refs/remotes/origin/cm-10.0
/* led class device attributes */
static DEVICE_ATTR(led_current, S_IRUGO | S_IWUSR, show_current, store_current);
static DEVICE_ATTR(max_current, S_IRUGO , show_max_current, NULL);

static struct attribute *lp5521_led_attributes[] = {
	&dev_attr_led_current.attr,
	&dev_attr_max_current.attr,
	NULL,
};

static struct attribute_group lp5521_led_attribute_group = {
	.attrs = lp5521_led_attributes
};

/* device attributes */
static DEVICE_ATTR(engine1_mode, S_IRUGO | S_IWUSR,
		   show_engine1_mode, store_engine1_mode);
static DEVICE_ATTR(engine2_mode, S_IRUGO | S_IWUSR,
		   show_engine2_mode, store_engine2_mode);
static DEVICE_ATTR(engine3_mode, S_IRUGO | S_IWUSR,
		   show_engine3_mode, store_engine3_mode);
static DEVICE_ATTR(engine1_load, S_IWUSR, NULL, store_engine1_load);
static DEVICE_ATTR(engine2_load, S_IWUSR, NULL, store_engine2_load);
static DEVICE_ATTR(engine3_load, S_IWUSR, NULL, store_engine3_load);
static DEVICE_ATTR(selftest, S_IRUGO, lp5521_selftest, NULL);
<<<<<<< HEAD
=======
static DEVICE_ATTR(led_pattern, S_IWUSR, NULL, store_led_pattern);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	return scnprintf(buf, PAGE_SIZE, "%s\n", ret ? "FAIL" : "OK");
}

/* device attributes */
static LP55XX_DEV_ATTR_RW(engine1_mode, show_engine1_mode, store_engine1_mode);
static LP55XX_DEV_ATTR_RW(engine2_mode, show_engine2_mode, store_engine2_mode);
static LP55XX_DEV_ATTR_RW(engine3_mode, show_engine3_mode, store_engine3_mode);
static LP55XX_DEV_ATTR_WO(engine1_load, store_engine1_load);
static LP55XX_DEV_ATTR_WO(engine2_load, store_engine2_load);
static LP55XX_DEV_ATTR_WO(engine3_load, store_engine3_load);
static LP55XX_DEV_ATTR_RO(selftest, lp5521_selftest);
>>>>>>> refs/remotes/origin/master

static struct attribute *lp5521_attributes[] = {
	&dev_attr_engine1_mode.attr,
	&dev_attr_engine2_mode.attr,
	&dev_attr_engine3_mode.attr,
<<<<<<< HEAD
	&dev_attr_selftest.attr,
	&dev_attr_engine1_load.attr,
	&dev_attr_engine2_load.attr,
	&dev_attr_engine3_load.attr,
<<<<<<< HEAD
=======
	&dev_attr_led_pattern.attr,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	&dev_attr_engine1_load.attr,
	&dev_attr_engine2_load.attr,
	&dev_attr_engine3_load.attr,
	&dev_attr_selftest.attr,
>>>>>>> refs/remotes/origin/master
	NULL
};

static const struct attribute_group lp5521_group = {
	.attrs = lp5521_attributes,
};

<<<<<<< HEAD
static int lp5521_register_sysfs(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	return sysfs_create_group(&dev->kobj, &lp5521_group);
}

static void lp5521_unregister_sysfs(struct i2c_client *client)
{
	struct lp5521_chip *chip = i2c_get_clientdata(client);
	struct device *dev = &client->dev;
	int i;

	sysfs_remove_group(&dev->kobj, &lp5521_group);

	for (i = 0; i < chip->num_leds; i++)
		sysfs_remove_group(&chip->leds[i].cdev.dev->kobj,
				&lp5521_led_attribute_group);
}

static int __devinit lp5521_init_led(struct lp5521_led *led,
				struct i2c_client *client,
				int chan, struct lp5521_platform_data *pdata)
{
	struct device *dev = &client->dev;
	char name[32];
	int res;

	if (chan >= LP5521_MAX_LEDS)
		return -EINVAL;

	if (pdata->led_config[chan].led_current == 0)
		return 0;

	led->led_current = pdata->led_config[chan].led_current;
	led->max_current = pdata->led_config[chan].max_current;
	led->chan_nr = pdata->led_config[chan].chan_nr;

	if (led->chan_nr >= LP5521_MAX_LEDS) {
		dev_err(dev, "Use channel numbers between 0 and %d\n",
			LP5521_MAX_LEDS - 1);
		return -EINVAL;
	}

<<<<<<< HEAD
	snprintf(name, sizeof(name), "%s:channel%d",
			pdata->label ?: client->name, chan);
	led->cdev.brightness_set = lp5521_set_brightness;
	led->cdev.name = name;
=======
	led->cdev.brightness_set = lp5521_set_brightness;
	if (pdata->led_config[chan].name) {
		led->cdev.name = pdata->led_config[chan].name;
	} else {
		snprintf(name, sizeof(name), "%s:channel%d",
			pdata->label ?: client->name, chan);
		led->cdev.name = name;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	res = led_classdev_register(dev, &led->cdev);
	if (res < 0) {
		dev_err(dev, "couldn't register led on channel %d\n", chan);
		return res;
	}

	res = sysfs_create_group(&led->cdev.dev->kobj,
			&lp5521_led_attribute_group);
	if (res < 0) {
		dev_err(dev, "couldn't register current attribute\n");
		led_classdev_unregister(&led->cdev);
		return res;
	}
	return 0;
}

static int __devinit lp5521_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct lp5521_chip		*chip;
	struct lp5521_platform_data	*pdata;
	int ret, i, led;
<<<<<<< HEAD
=======
	u8 buf;
>>>>>>> refs/remotes/origin/cm-10.0

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	i2c_set_clientdata(client, chip);
	chip->client = client;

	pdata = client->dev.platform_data;

	if (!pdata) {
		dev_err(&client->dev, "no platform data\n");
		ret = -EINVAL;
		goto fail1;
	}

	mutex_init(&chip->lock);

	chip->pdata   = pdata;

	if (pdata->setup_resources) {
		ret = pdata->setup_resources();
		if (ret < 0)
			goto fail1;
	}

	if (pdata->enable) {
		pdata->enable(0);
		usleep_range(1000, 2000); /* Keep enable down at least 1ms */
		pdata->enable(1);
		usleep_range(1000, 2000); /* 500us abs min. */
	}

	lp5521_write(client, LP5521_REG_RESET, 0xff);
	usleep_range(10000, 20000); /*
				     * Exact value is not available. 10 - 20ms
				     * appears to be enough for reset.
				     */
<<<<<<< HEAD
=======

	/*
	 * Make sure that the chip is reset by reading back the r channel
	 * current reg. This is dummy read is required on some platforms -
	 * otherwise further access to the R G B channels in the
	 * LP5521_REG_ENABLE register will not have any effect - strange!
	 */
	ret = lp5521_read(client, LP5521_REG_R_CURRENT, &buf);
	if (ret || buf != LP5521_REG_R_CURR_DEFAULT) {
		dev_err(&client->dev, "error in resetting chip\n");
		goto fail2;
	}
	usleep_range(10000, 20000);

>>>>>>> refs/remotes/origin/cm-10.0
	ret = lp5521_detect(client);

	if (ret) {
		dev_err(&client->dev, "Chip not found\n");
		goto fail2;
	}

	dev_info(&client->dev, "%s programmable led chip found\n", id->name);

	ret = lp5521_configure(client);
	if (ret < 0) {
		dev_err(&client->dev, "error configuring chip\n");
		goto fail2;
	}

	/* Initialize leds */
	chip->num_channels = pdata->num_channels;
	chip->num_leds = 0;
	led = 0;
	for (i = 0; i < pdata->num_channels; i++) {
		/* Do not initialize channels that are not connected */
		if (pdata->led_config[i].led_current == 0)
			continue;

		ret = lp5521_init_led(&chip->leds[led], client, i, pdata);
		if (ret) {
			dev_err(&client->dev, "error initializing leds\n");
			goto fail3;
		}
		chip->num_leds++;

		chip->leds[led].id = led;
		/* Set initial LED current */
		lp5521_set_led_current(chip, led,
				chip->leds[led].led_current);

		INIT_WORK(&(chip->leds[led].brightness_work),
			lp5521_led_brightness_work);

		led++;
	}

	ret = lp5521_register_sysfs(client);
	if (ret) {
		dev_err(&client->dev, "registering sysfs failed\n");
		goto fail3;
	}
	return ret;
fail3:
	for (i = 0; i < chip->num_leds; i++) {
		led_classdev_unregister(&chip->leds[i].cdev);
		cancel_work_sync(&chip->leds[i].brightness_work);
	}
fail2:
	if (pdata->enable)
		pdata->enable(0);
	if (pdata->release_resources)
		pdata->release_resources();
fail1:
	kfree(chip);
	return ret;
}

<<<<<<< HEAD
static int lp5521_remove(struct i2c_client *client)
=======
static int __devexit lp5521_remove(struct i2c_client *client)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct lp5521_chip *chip = i2c_get_clientdata(client);
	int i;

<<<<<<< HEAD
=======
	lp5521_run_led_pattern(PATTERN_OFF, chip);
>>>>>>> refs/remotes/origin/cm-10.0
	lp5521_unregister_sysfs(client);

	for (i = 0; i < chip->num_leds; i++) {
		led_classdev_unregister(&chip->leds[i].cdev);
		cancel_work_sync(&chip->leds[i].brightness_work);
	}

	if (chip->pdata->enable)
		chip->pdata->enable(0);
	if (chip->pdata->release_resources)
		chip->pdata->release_resources();
	kfree(chip);
=======
/* Chip specific configurations */
static struct lp55xx_device_config lp5521_cfg = {
	.reset = {
		.addr = LP5521_REG_RESET,
		.val  = LP5521_RESET,
	},
	.enable = {
		.addr = LP5521_REG_ENABLE,
		.val  = LP5521_ENABLE_DEFAULT,
	},
	.max_channel  = LP5521_MAX_LEDS,
	.post_init_device   = lp5521_post_init_device,
	.brightness_work_fn = lp5521_led_brightness_work,
	.set_led_current    = lp5521_set_led_current,
	.firmware_cb        = lp5521_firmware_loaded,
	.run_engine         = lp5521_run_engine,
	.dev_attr_group     = &lp5521_group,
};

static int lp5521_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int ret;
	struct lp55xx_chip *chip;
	struct lp55xx_led *led;
	struct lp55xx_platform_data *pdata;
	struct device_node *np = client->dev.of_node;

	if (!dev_get_platdata(&client->dev)) {
		if (np) {
			ret = lp55xx_of_populate_pdata(&client->dev, np);
			if (ret < 0)
				return ret;
		} else {
			dev_err(&client->dev, "no platform data\n");
			return -EINVAL;
		}
	}
	pdata = dev_get_platdata(&client->dev);

	chip = devm_kzalloc(&client->dev, sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	led = devm_kzalloc(&client->dev,
			sizeof(*led) * pdata->num_channels, GFP_KERNEL);
	if (!led)
		return -ENOMEM;

	chip->cl = client;
	chip->pdata = pdata;
	chip->cfg = &lp5521_cfg;

	mutex_init(&chip->lock);

	i2c_set_clientdata(client, led);

	ret = lp55xx_init_device(chip);
	if (ret)
		goto err_init;

	dev_info(&client->dev, "%s programmable led chip found\n", id->name);

	ret = lp55xx_register_leds(led, chip);
	if (ret)
		goto err_register_leds;

	ret = lp55xx_register_sysfs(chip);
	if (ret) {
		dev_err(&client->dev, "registering sysfs failed\n");
		goto err_register_sysfs;
	}

	return 0;

err_register_sysfs:
	lp55xx_unregister_leds(led, chip);
err_register_leds:
	lp55xx_deinit_device(chip);
err_init:
	return ret;
}

static int lp5521_remove(struct i2c_client *client)
{
	struct lp55xx_led *led = i2c_get_clientdata(client);
	struct lp55xx_chip *chip = led->chip;

	lp5521_stop_engine(chip);
	lp55xx_unregister_sysfs(chip);
	lp55xx_unregister_leds(led, chip);
	lp55xx_deinit_device(chip);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static const struct i2c_device_id lp5521_id[] = {
	{ "lp5521", 0 }, /* Three channel chip */
	{ }
};
MODULE_DEVICE_TABLE(i2c, lp5521_id);

<<<<<<< HEAD
static struct i2c_driver lp5521_driver = {
	.driver = {
		.name	= "lp5521",
	},
	.probe		= lp5521_probe,
<<<<<<< HEAD
=======
#ifdef CONFIG_OF
static const struct of_device_id of_lp5521_leds_match[] = {
	{ .compatible = "national,lp5521", },
	{},
};

MODULE_DEVICE_TABLE(of, of_lp5521_leds_match);
#endif
static struct i2c_driver lp5521_driver = {
	.driver = {
		.name	= "lp5521",
		.of_match_table = of_match_ptr(of_lp5521_leds_match),
	},
	.probe		= lp5521_probe,
>>>>>>> refs/remotes/origin/master
	.remove		= lp5521_remove,
	.id_table	= lp5521_id,
};

<<<<<<< HEAD
static int __init lp5521_init(void)
{
	int ret;

	ret = i2c_add_driver(&lp5521_driver);

	if (ret < 0)
		printk(KERN_ALERT "Adding lp5521 driver failed\n");

	return ret;
}

static void __exit lp5521_exit(void)
{
	i2c_del_driver(&lp5521_driver);
}

module_init(lp5521_init);
module_exit(lp5521_exit);
=======
	.remove		= __devexit_p(lp5521_remove),
	.id_table	= lp5521_id,
};

module_i2c_driver(lp5521_driver);
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_AUTHOR("Mathias Nyman, Yuri Zaporozhets, Samu Onkalo");
=======
module_i2c_driver(lp5521_driver);

MODULE_AUTHOR("Mathias Nyman, Yuri Zaporozhets, Samu Onkalo");
MODULE_AUTHOR("Milo Kim <milo.kim@ti.com>");
>>>>>>> refs/remotes/origin/master
MODULE_DESCRIPTION("LP5521 LED engine");
MODULE_LICENSE("GPL v2");
