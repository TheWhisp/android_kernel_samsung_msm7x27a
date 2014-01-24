/*
 * lp5523.c - LP5523 LED Driver
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
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
#include <linux/leds-lp5523.h>
#include <linux/workqueue.h>
#include <linux/slab.h>

#define LP5523_REG_ENABLE		0x00
#define LP5523_REG_OP_MODE		0x01
#define LP5523_REG_RATIOMETRIC_MSB	0x02
#define LP5523_REG_RATIOMETRIC_LSB	0x03
#define LP5523_REG_ENABLE_LEDS_MSB	0x04
#define LP5523_REG_ENABLE_LEDS_LSB	0x05
#define LP5523_REG_LED_CNTRL_BASE	0x06
#define LP5523_REG_LED_PWM_BASE		0x16
#define LP5523_REG_LED_CURRENT_BASE	0x26
#define LP5523_REG_CONFIG		0x36
#define LP5523_REG_CHANNEL1_PC		0x37
#define LP5523_REG_CHANNEL2_PC		0x38
#define LP5523_REG_CHANNEL3_PC		0x39
#define LP5523_REG_STATUS		0x3a
#define LP5523_REG_GPO			0x3b
#define LP5523_REG_VARIABLE		0x3c
#define LP5523_REG_RESET		0x3d
#define LP5523_REG_TEMP_CTRL		0x3e
#define LP5523_REG_TEMP_READ		0x3f
#define LP5523_REG_TEMP_WRITE		0x40
#define LP5523_REG_LED_TEST_CTRL	0x41
#define LP5523_REG_LED_TEST_ADC		0x42
#define LP5523_REG_ENG1_VARIABLE	0x45
#define LP5523_REG_ENG2_VARIABLE	0x46
#define LP5523_REG_ENG3_VARIABLE	0x47
#define LP5523_REG_MASTER_FADER1	0x48
#define LP5523_REG_MASTER_FADER2	0x49
#define LP5523_REG_MASTER_FADER3	0x4a
#define LP5523_REG_CH1_PROG_START	0x4c
#define LP5523_REG_CH2_PROG_START	0x4d
#define LP5523_REG_CH3_PROG_START	0x4e
#define LP5523_REG_PROG_PAGE_SEL	0x4f
#define LP5523_REG_PROG_MEM		0x50

#define LP5523_CMD_LOAD			0x15 /* 00010101 */
#define LP5523_CMD_RUN			0x2a /* 00101010 */
#define LP5523_CMD_DISABLED		0x00 /* 00000000 */

=======
#include <linux/delay.h>
#include <linux/firmware.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/leds.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/platform_data/leds-lp55xx.h>
#include <linux/slab.h>

#include "leds-lp55xx-common.h"

#define LP5523_PROGRAM_LENGTH		32
#define LP5523_MAX_LEDS			9

/* Registers */
#define LP5523_REG_ENABLE		0x00
#define LP5523_REG_OP_MODE		0x01
#define LP5523_REG_ENABLE_LEDS_MSB	0x04
#define LP5523_REG_ENABLE_LEDS_LSB	0x05
#define LP5523_REG_LED_PWM_BASE		0x16
#define LP5523_REG_LED_CURRENT_BASE	0x26
#define LP5523_REG_CONFIG		0x36
#define LP5523_REG_STATUS		0x3A
#define LP5523_REG_RESET		0x3D
#define LP5523_REG_LED_TEST_CTRL	0x41
#define LP5523_REG_LED_TEST_ADC		0x42
#define LP5523_REG_CH1_PROG_START	0x4C
#define LP5523_REG_CH2_PROG_START	0x4D
#define LP5523_REG_CH3_PROG_START	0x4E
#define LP5523_REG_PROG_PAGE_SEL	0x4F
#define LP5523_REG_PROG_MEM		0x50

/* Bit description in registers */
>>>>>>> refs/remotes/origin/master
#define LP5523_ENABLE			0x40
#define LP5523_AUTO_INC			0x40
#define LP5523_PWR_SAVE			0x20
#define LP5523_PWM_PWR_SAVE		0x04
<<<<<<< HEAD
#define LP5523_CP_1			0x08
#define LP5523_CP_1_5			0x10
#define LP5523_CP_AUTO			0x18
#define LP5523_INT_CLK			0x01
#define LP5523_AUTO_CLK			0x02
#define LP5523_EN_LEDTEST		0x80
#define LP5523_LEDTEST_DONE		0x80

#define LP5523_DEFAULT_CURRENT		50 /* microAmps */
#define LP5523_PROGRAM_LENGTH		32 /* in bytes */
#define LP5523_PROGRAM_PAGES		6
#define LP5523_ADC_SHORTCIRC_LIM	80

#define LP5523_LEDS			9
#define LP5523_ENGINES			3

#define LP5523_ENG_MASK_BASE		0x30 /* 00110000 */

#define LP5523_ENG_STATUS_MASK          0x07 /* 00000111 */

#define LP5523_IRQ_FLAGS                IRQF_TRIGGER_FALLING

#define LP5523_EXT_CLK_USED		0x08

#define LED_ACTIVE(mux, led)		(!!(mux & (0x0001 << led)))
#define SHIFT_MASK(id)			(((id) - 1) * 2)

struct lp5523_engine {
	int		id;
	u8		mode;
	u8		prog_page;
	u8		mux_page;
	u16		led_mux;
	u8		engine_mask;
};

struct lp5523_led {
	int			id;
	u8			chan_nr;
	u8			led_current;
	u8			max_current;
	struct led_classdev     cdev;
	struct work_struct	brightness_work;
	u8			brightness;
};

struct lp5523_chip {
	struct mutex		lock; /* Serialize control */
	struct i2c_client	*client;
	struct lp5523_engine	engines[LP5523_ENGINES];
	struct lp5523_led	leds[LP5523_LEDS];
	struct lp5523_platform_data *pdata;
	u8			num_channels;
	u8			num_leds;
};

static inline struct lp5523_led *cdev_to_led(struct led_classdev *cdev)
{
	return container_of(cdev, struct lp5523_led, cdev);
}

static inline struct lp5523_chip *engine_to_lp5523(struct lp5523_engine *engine)
{
	return container_of(engine, struct lp5523_chip,
			    engines[engine->id - 1]);
}

static inline struct lp5523_chip *led_to_lp5523(struct lp5523_led *led)
{
	return container_of(led, struct lp5523_chip,
			    leds[led->id]);
}

static int lp5523_set_mode(struct lp5523_engine *engine, u8 mode);
static int lp5523_set_engine_mode(struct lp5523_engine *engine, u8 mode);
<<<<<<< HEAD
static int lp5523_load_program(struct lp5523_engine *engine, u8 *pattern);
=======
static int lp5523_load_program(struct lp5523_engine *engine, const u8 *pattern);
>>>>>>> refs/remotes/origin/cm-10.0

static void lp5523_led_brightness_work(struct work_struct *work);

static int lp5523_write(struct i2c_client *client, u8 reg, u8 value)
{
	return i2c_smbus_write_byte_data(client, reg, value);
}

static int lp5523_read(struct i2c_client *client, u8 reg, u8 *buf)
{
	s32 ret = i2c_smbus_read_byte_data(client, reg);

	if (ret < 0)
		return -EIO;

	*buf = ret;
	return 0;
}

static int lp5523_detect(struct i2c_client *client)
{
	int ret;
	u8 buf;

	ret = lp5523_write(client, LP5523_REG_ENABLE, 0x40);
	if (ret)
		return ret;
	ret = lp5523_read(client, LP5523_REG_ENABLE, &buf);
	if (ret)
		return ret;
	if (buf == 0x40)
		return 0;
	else
		return -ENODEV;
}

static int lp5523_configure(struct i2c_client *client)
{
	struct lp5523_chip *chip = i2c_get_clientdata(client);
	int ret = 0;
	u8 status;

	/* one pattern per engine setting led mux start and stop addresses */
<<<<<<< HEAD
	u8 pattern[][LP5523_PROGRAM_LENGTH] =  {
=======
	static const u8 pattern[][LP5523_PROGRAM_LENGTH] =  {
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define LP5523_CP_AUTO			0x18
#define LP5523_AUTO_CLK			0x02

#define LP5523_EN_LEDTEST		0x80
#define LP5523_LEDTEST_DONE		0x80
#define LP5523_RESET			0xFF
#define LP5523_ADC_SHORTCIRC_LIM	80
#define LP5523_EXT_CLK_USED		0x08
#define LP5523_ENG_STATUS_MASK		0x07

/* Memory Page Selection */
#define LP5523_PAGE_ENG1		0
#define LP5523_PAGE_ENG2		1
#define LP5523_PAGE_ENG3		2
#define LP5523_PAGE_MUX1		3
#define LP5523_PAGE_MUX2		4
#define LP5523_PAGE_MUX3		5

/* Program Memory Operations */
#define LP5523_MODE_ENG1_M		0x30	/* Operation Mode Register */
#define LP5523_MODE_ENG2_M		0x0C
#define LP5523_MODE_ENG3_M		0x03
#define LP5523_LOAD_ENG1		0x10
#define LP5523_LOAD_ENG2		0x04
#define LP5523_LOAD_ENG3		0x01

#define LP5523_ENG1_IS_LOADING(mode)	\
	((mode & LP5523_MODE_ENG1_M) == LP5523_LOAD_ENG1)
#define LP5523_ENG2_IS_LOADING(mode)	\
	((mode & LP5523_MODE_ENG2_M) == LP5523_LOAD_ENG2)
#define LP5523_ENG3_IS_LOADING(mode)	\
	((mode & LP5523_MODE_ENG3_M) == LP5523_LOAD_ENG3)

#define LP5523_EXEC_ENG1_M		0x30	/* Enable Register */
#define LP5523_EXEC_ENG2_M		0x0C
#define LP5523_EXEC_ENG3_M		0x03
#define LP5523_EXEC_M			0x3F
#define LP5523_RUN_ENG1			0x20
#define LP5523_RUN_ENG2			0x08
#define LP5523_RUN_ENG3			0x02

#define LED_ACTIVE(mux, led)		(!!(mux & (0x0001 << led)))

enum lp5523_chip_id {
	LP5523,
	LP55231,
};

static int lp5523_init_program_engine(struct lp55xx_chip *chip);

static inline void lp5523_wait_opmode_done(void)
{
	usleep_range(1000, 2000);
}

static void lp5523_set_led_current(struct lp55xx_led *led, u8 led_current)
{
	led->led_current = led_current;
	lp55xx_write(led->chip, LP5523_REG_LED_CURRENT_BASE + led->chan_nr,
		led_current);
}

static int lp5523_post_init_device(struct lp55xx_chip *chip)
{
	int ret;

	ret = lp55xx_write(chip, LP5523_REG_ENABLE, LP5523_ENABLE);
	if (ret)
		return ret;

	/* Chip startup time is 500 us, 1 - 2 ms gives some margin */
	usleep_range(1000, 2000);

	ret = lp55xx_write(chip, LP5523_REG_CONFIG,
			    LP5523_AUTO_INC | LP5523_PWR_SAVE |
			    LP5523_CP_AUTO | LP5523_AUTO_CLK |
			    LP5523_PWM_PWR_SAVE);
	if (ret)
		return ret;

	/* turn on all leds */
	ret = lp55xx_write(chip, LP5523_REG_ENABLE_LEDS_MSB, 0x01);
	if (ret)
		return ret;

	ret = lp55xx_write(chip, LP5523_REG_ENABLE_LEDS_LSB, 0xff);
	if (ret)
		return ret;

	return lp5523_init_program_engine(chip);
}

static void lp5523_load_engine(struct lp55xx_chip *chip)
{
	enum lp55xx_engine_index idx = chip->engine_idx;
	u8 mask[] = {
		[LP55XX_ENGINE_1] = LP5523_MODE_ENG1_M,
		[LP55XX_ENGINE_2] = LP5523_MODE_ENG2_M,
		[LP55XX_ENGINE_3] = LP5523_MODE_ENG3_M,
	};

	u8 val[] = {
		[LP55XX_ENGINE_1] = LP5523_LOAD_ENG1,
		[LP55XX_ENGINE_2] = LP5523_LOAD_ENG2,
		[LP55XX_ENGINE_3] = LP5523_LOAD_ENG3,
	};

	lp55xx_update_bits(chip, LP5523_REG_OP_MODE, mask[idx], val[idx]);

	lp5523_wait_opmode_done();
}

static void lp5523_load_engine_and_select_page(struct lp55xx_chip *chip)
{
	enum lp55xx_engine_index idx = chip->engine_idx;
	u8 page_sel[] = {
		[LP55XX_ENGINE_1] = LP5523_PAGE_ENG1,
		[LP55XX_ENGINE_2] = LP5523_PAGE_ENG2,
		[LP55XX_ENGINE_3] = LP5523_PAGE_ENG3,
	};

	lp5523_load_engine(chip);

	lp55xx_write(chip, LP5523_REG_PROG_PAGE_SEL, page_sel[idx]);
}

static void lp5523_stop_engine(struct lp55xx_chip *chip)
{
	lp55xx_write(chip, LP5523_REG_OP_MODE, 0);
	lp5523_wait_opmode_done();
}

static void lp5523_turn_off_channels(struct lp55xx_chip *chip)
{
	int i;

	for (i = 0; i < LP5523_MAX_LEDS; i++)
		lp55xx_write(chip, LP5523_REG_LED_PWM_BASE + i, 0);
}

static void lp5523_run_engine(struct lp55xx_chip *chip, bool start)
{
	int ret;
	u8 mode;
	u8 exec;

	/* stop engine */
	if (!start) {
		lp5523_stop_engine(chip);
		lp5523_turn_off_channels(chip);
		return;
	}

	/*
	 * To run the engine,
	 * operation mode and enable register should updated at the same time
	 */

	ret = lp55xx_read(chip, LP5523_REG_OP_MODE, &mode);
	if (ret)
		return;

	ret = lp55xx_read(chip, LP5523_REG_ENABLE, &exec);
	if (ret)
		return;

	/* change operation mode to RUN only when each engine is loading */
	if (LP5523_ENG1_IS_LOADING(mode)) {
		mode = (mode & ~LP5523_MODE_ENG1_M) | LP5523_RUN_ENG1;
		exec = (exec & ~LP5523_EXEC_ENG1_M) | LP5523_RUN_ENG1;
	}

	if (LP5523_ENG2_IS_LOADING(mode)) {
		mode = (mode & ~LP5523_MODE_ENG2_M) | LP5523_RUN_ENG2;
		exec = (exec & ~LP5523_EXEC_ENG2_M) | LP5523_RUN_ENG2;
	}

	if (LP5523_ENG3_IS_LOADING(mode)) {
		mode = (mode & ~LP5523_MODE_ENG3_M) | LP5523_RUN_ENG3;
		exec = (exec & ~LP5523_EXEC_ENG3_M) | LP5523_RUN_ENG3;
	}

	lp55xx_write(chip, LP5523_REG_OP_MODE, mode);
	lp5523_wait_opmode_done();

	lp55xx_update_bits(chip, LP5523_REG_ENABLE, LP5523_EXEC_M, exec);
}

static int lp5523_init_program_engine(struct lp55xx_chip *chip)
{
	int i;
	int j;
	int ret;
	u8 status;
	/* one pattern per engine setting LED MUX start and stop addresses */
	static const u8 pattern[][LP5523_PROGRAM_LENGTH] =  {
>>>>>>> refs/remotes/origin/master
		{ 0x9c, 0x30, 0x9c, 0xb0, 0x9d, 0x80, 0xd8, 0x00, 0},
		{ 0x9c, 0x40, 0x9c, 0xc0, 0x9d, 0x80, 0xd8, 0x00, 0},
		{ 0x9c, 0x50, 0x9c, 0xd0, 0x9d, 0x80, 0xd8, 0x00, 0},
	};

<<<<<<< HEAD
	ret |= lp5523_write(client, LP5523_REG_ENABLE, LP5523_ENABLE);
	/* Chip startup time is 500 us, 1 - 2 ms gives some margin */
	usleep_range(1000, 2000);

	ret |= lp5523_write(client, LP5523_REG_CONFIG,
			    LP5523_AUTO_INC | LP5523_PWR_SAVE |
			    LP5523_CP_AUTO | LP5523_AUTO_CLK |
			    LP5523_PWM_PWR_SAVE);

	/* turn on all leds */
	ret |= lp5523_write(client, LP5523_REG_ENABLE_LEDS_MSB, 0x01);
	ret |= lp5523_write(client, LP5523_REG_ENABLE_LEDS_LSB, 0xff);

	/* hardcode 32 bytes of memory for each engine from program memory */
	ret |= lp5523_write(client, LP5523_REG_CH1_PROG_START, 0x00);
	ret |= lp5523_write(client, LP5523_REG_CH2_PROG_START, 0x10);
	ret |= lp5523_write(client, LP5523_REG_CH3_PROG_START, 0x20);

	/* write led mux address space for each channel */
	ret |= lp5523_load_program(&chip->engines[0], pattern[0]);
	ret |= lp5523_load_program(&chip->engines[1], pattern[1]);
	ret |= lp5523_load_program(&chip->engines[2], pattern[2]);

	if (ret) {
		dev_err(&client->dev, "could not load mux programs\n");
		return -1;
	}

	/* set all engines exec state and mode to run 00101010 */
	ret |= lp5523_write(client, LP5523_REG_ENABLE,
			    (LP5523_CMD_RUN | LP5523_ENABLE));

	ret |= lp5523_write(client, LP5523_REG_OP_MODE, LP5523_CMD_RUN);

	if (ret) {
		dev_err(&client->dev, "could not start mux programs\n");
		return -1;
	}

	/* Let the programs run for couple of ms and check the engine status */
	usleep_range(3000, 6000);
	lp5523_read(client, LP5523_REG_STATUS, &status);
	status &= LP5523_ENG_STATUS_MASK;

	if (status == LP5523_ENG_STATUS_MASK) {
		dev_dbg(&client->dev, "all engines configured\n");
	} else {
		dev_info(&client->dev, "status == %x\n", status);
		dev_err(&client->dev, "cound not configure LED engine\n");
		return -1;
	}

	dev_info(&client->dev, "disabling engines\n");

	ret |= lp5523_write(client, LP5523_REG_OP_MODE, LP5523_CMD_DISABLED);

	return ret;
}

static int lp5523_set_engine_mode(struct lp5523_engine *engine, u8 mode)
{
	struct lp5523_chip *chip = engine_to_lp5523(engine);
	struct i2c_client *client = chip->client;
	int ret;
	u8 engine_state;

	ret = lp5523_read(client, LP5523_REG_OP_MODE, &engine_state);
	if (ret)
		goto fail;

	engine_state &= ~(engine->engine_mask);

	/* set mode only for this engine */
	mode &= engine->engine_mask;

	engine_state |= mode;

	ret |= lp5523_write(client, LP5523_REG_OP_MODE, engine_state);
fail:
	return ret;
}

static int lp5523_load_mux(struct lp5523_engine *engine, u16 mux)
{
	struct lp5523_chip *chip = engine_to_lp5523(engine);
	struct i2c_client *client = chip->client;
	int ret = 0;

	ret |= lp5523_set_engine_mode(engine, LP5523_CMD_LOAD);

	ret |= lp5523_write(client, LP5523_REG_PROG_PAGE_SEL, engine->mux_page);
	ret |= lp5523_write(client, LP5523_REG_PROG_MEM,
			    (u8)(mux >> 8));
	ret |= lp5523_write(client, LP5523_REG_PROG_MEM + 1, (u8)(mux));
	engine->led_mux = mux;

	return ret;
}

<<<<<<< HEAD
static int lp5523_load_program(struct lp5523_engine *engine, u8 *pattern)
=======
static int lp5523_load_program(struct lp5523_engine *engine, const u8 *pattern)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct lp5523_chip *chip = engine_to_lp5523(engine);
	struct i2c_client *client = chip->client;

	int ret = 0;

	ret |= lp5523_set_engine_mode(engine, LP5523_CMD_LOAD);

	ret |= lp5523_write(client, LP5523_REG_PROG_PAGE_SEL,
			    engine->prog_page);
	ret |= i2c_smbus_write_i2c_block_data(client, LP5523_REG_PROG_MEM,
					      LP5523_PROGRAM_LENGTH, pattern);

	return ret;
}

static int lp5523_run_program(struct lp5523_engine *engine)
{
	struct lp5523_chip *chip = engine_to_lp5523(engine);
	struct i2c_client *client = chip->client;
	int ret;

	ret = lp5523_write(client, LP5523_REG_ENABLE,
					LP5523_CMD_RUN | LP5523_ENABLE);
	if (ret)
		goto fail;

	ret = lp5523_set_engine_mode(engine, LP5523_CMD_RUN);
fail:
	return ret;
}

static int lp5523_mux_parse(const char *buf, u16 *mux, size_t len)
{
	int i;
	u16 tmp_mux = 0;
	len = len < LP5523_LEDS ? len : LP5523_LEDS;
=======
	/* hardcode 32 bytes of memory for each engine from program memory */
	ret = lp55xx_write(chip, LP5523_REG_CH1_PROG_START, 0x00);
	if (ret)
		return ret;

	ret = lp55xx_write(chip, LP5523_REG_CH2_PROG_START, 0x10);
	if (ret)
		return ret;

	ret = lp55xx_write(chip, LP5523_REG_CH3_PROG_START, 0x20);
	if (ret)
		return ret;

	/* write LED MUX address space for each engine */
	for (i = LP55XX_ENGINE_1; i <= LP55XX_ENGINE_3; i++) {
		chip->engine_idx = i;
		lp5523_load_engine_and_select_page(chip);

		for (j = 0; j < LP5523_PROGRAM_LENGTH; j++) {
			ret = lp55xx_write(chip, LP5523_REG_PROG_MEM + j,
					pattern[i - 1][j]);
			if (ret)
				goto out;
		}
	}

	lp5523_run_engine(chip, true);

	/* Let the programs run for couple of ms and check the engine status */
	usleep_range(3000, 6000);
	lp55xx_read(chip, LP5523_REG_STATUS, &status);
	status &= LP5523_ENG_STATUS_MASK;

	if (status != LP5523_ENG_STATUS_MASK) {
		dev_err(&chip->cl->dev,
			"cound not configure LED engine, status = 0x%.2x\n",
			status);
		ret = -1;
	}

out:
	lp5523_stop_engine(chip);
	return ret;
}

static int lp5523_update_program_memory(struct lp55xx_chip *chip,
					const u8 *data, size_t size)
{
	u8 pattern[LP5523_PROGRAM_LENGTH] = {0};
	unsigned cmd;
	char c[3];
	int nrchars;
	int ret;
	int offset = 0;
	int i = 0;

	while ((offset < size - 1) && (i < LP5523_PROGRAM_LENGTH)) {
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

	for (i = 0; i < LP5523_PROGRAM_LENGTH; i++) {
		ret = lp55xx_write(chip, LP5523_REG_PROG_MEM + i, pattern[i]);
		if (ret)
			return -EINVAL;
	}

	return size;

err:
	dev_err(&chip->cl->dev, "wrong pattern format\n");
	return -EINVAL;
}

static void lp5523_firmware_loaded(struct lp55xx_chip *chip)
{
	const struct firmware *fw = chip->fw;

	if (fw->size > LP5523_PROGRAM_LENGTH) {
		dev_err(&chip->cl->dev, "firmware data size overflow: %zu\n",
			fw->size);
		return;
	}

	/*
	 * Program momery sequence
	 *  1) set engine mode to "LOAD"
	 *  2) write firmware data into program memory
	 */

	lp5523_load_engine_and_select_page(chip);
	lp5523_update_program_memory(chip, fw->data, fw->size);
}

static ssize_t show_engine_mode(struct device *dev,
				struct device_attribute *attr,
				char *buf, int nr)
{
	struct lp55xx_led *led = i2c_get_clientdata(to_i2c_client(dev));
	struct lp55xx_chip *chip = led->chip;
	enum lp55xx_engine_mode mode = chip->engines[nr - 1].mode;

	switch (mode) {
	case LP55XX_ENGINE_RUN:
		return sprintf(buf, "run\n");
	case LP55XX_ENGINE_LOAD:
		return sprintf(buf, "load\n");
	case LP55XX_ENGINE_DISABLED:
	default:
		return sprintf(buf, "disabled\n");
	}
}
show_mode(1)
show_mode(2)
show_mode(3)

static ssize_t store_engine_mode(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t len, int nr)
{
	struct lp55xx_led *led = i2c_get_clientdata(to_i2c_client(dev));
	struct lp55xx_chip *chip = led->chip;
	struct lp55xx_engine *engine = &chip->engines[nr - 1];

	mutex_lock(&chip->lock);

	chip->engine_idx = nr;

	if (!strncmp(buf, "run", 3)) {
		lp5523_run_engine(chip, true);
		engine->mode = LP55XX_ENGINE_RUN;
	} else if (!strncmp(buf, "load", 4)) {
		lp5523_stop_engine(chip);
		lp5523_load_engine(chip);
		engine->mode = LP55XX_ENGINE_LOAD;
	} else if (!strncmp(buf, "disabled", 8)) {
		lp5523_stop_engine(chip);
		engine->mode = LP55XX_ENGINE_DISABLED;
	}

	mutex_unlock(&chip->lock);

	return len;
}
store_mode(1)
store_mode(2)
store_mode(3)

static int lp5523_mux_parse(const char *buf, u16 *mux, size_t len)
{
	u16 tmp_mux = 0;
	int i;

	len = min_t(int, len, LP5523_MAX_LEDS);

>>>>>>> refs/remotes/origin/master
	for (i = 0; i < len; i++) {
		switch (buf[i]) {
		case '1':
			tmp_mux |= (1 << i);
			break;
		case '0':
			break;
		case '\n':
			i = len;
			break;
		default:
			return -1;
		}
	}
	*mux = tmp_mux;

	return 0;
}

static void lp5523_mux_to_array(u16 led_mux, char *array)
{
	int i, pos = 0;
<<<<<<< HEAD
	for (i = 0; i < LP5523_LEDS; i++)
=======
	for (i = 0; i < LP5523_MAX_LEDS; i++)
>>>>>>> refs/remotes/origin/master
		pos += sprintf(array + pos, "%x", LED_ACTIVE(led_mux, i));

	array[pos] = '\0';
}

<<<<<<< HEAD
/*--------------------------------------------------------------*/
/*			Sysfs interface				*/
/*--------------------------------------------------------------*/

=======
>>>>>>> refs/remotes/origin/master
static ssize_t show_engine_leds(struct device *dev,
			    struct device_attribute *attr,
			    char *buf, int nr)
{
<<<<<<< HEAD
	struct i2c_client *client = to_i2c_client(dev);
	struct lp5523_chip *chip = i2c_get_clientdata(client);
	char mux[LP5523_LEDS + 1];
=======
	struct lp55xx_led *led = i2c_get_clientdata(to_i2c_client(dev));
	struct lp55xx_chip *chip = led->chip;
	char mux[LP5523_MAX_LEDS + 1];
>>>>>>> refs/remotes/origin/master

	lp5523_mux_to_array(chip->engines[nr - 1].led_mux, mux);

	return sprintf(buf, "%s\n", mux);
}
<<<<<<< HEAD

#define show_leds(nr)							\
static ssize_t show_engine##nr##_leds(struct device *dev,		\
			    struct device_attribute *attr,		\
			    char *buf)					\
{									\
	return show_engine_leds(dev, attr, buf, nr);			\
}
=======
>>>>>>> refs/remotes/origin/master
show_leds(1)
show_leds(2)
show_leds(3)

<<<<<<< HEAD
=======
static int lp5523_load_mux(struct lp55xx_chip *chip, u16 mux, int nr)
{
	struct lp55xx_engine *engine = &chip->engines[nr - 1];
	int ret;
	u8 mux_page[] = {
		[LP55XX_ENGINE_1] = LP5523_PAGE_MUX1,
		[LP55XX_ENGINE_2] = LP5523_PAGE_MUX2,
		[LP55XX_ENGINE_3] = LP5523_PAGE_MUX3,
	};

	lp5523_load_engine(chip);

	ret = lp55xx_write(chip, LP5523_REG_PROG_PAGE_SEL, mux_page[nr]);
	if (ret)
		return ret;

	ret = lp55xx_write(chip, LP5523_REG_PROG_MEM , (u8)(mux >> 8));
	if (ret)
		return ret;

	ret = lp55xx_write(chip, LP5523_REG_PROG_MEM + 1, (u8)(mux));
	if (ret)
		return ret;

	engine->led_mux = mux;
	return 0;
}

>>>>>>> refs/remotes/origin/master
static ssize_t store_engine_leds(struct device *dev,
			     struct device_attribute *attr,
			     const char *buf, size_t len, int nr)
{
<<<<<<< HEAD
	struct i2c_client *client = to_i2c_client(dev);
	struct lp5523_chip *chip = i2c_get_clientdata(client);
=======
	struct lp55xx_led *led = i2c_get_clientdata(to_i2c_client(dev));
	struct lp55xx_chip *chip = led->chip;
	struct lp55xx_engine *engine = &chip->engines[nr - 1];
>>>>>>> refs/remotes/origin/master
	u16 mux = 0;
	ssize_t ret;

	if (lp5523_mux_parse(buf, &mux, len))
		return -EINVAL;

	mutex_lock(&chip->lock);
<<<<<<< HEAD
	ret = -EINVAL;
	if (chip->engines[nr - 1].mode != LP5523_CMD_LOAD)
		goto leave;

	if (lp5523_load_mux(&chip->engines[nr - 1], mux))
=======

	chip->engine_idx = nr;
	ret = -EINVAL;

	if (engine->mode != LP55XX_ENGINE_LOAD)
		goto leave;

	if (lp5523_load_mux(chip, mux, nr))
>>>>>>> refs/remotes/origin/master
		goto leave;

	ret = len;
leave:
	mutex_unlock(&chip->lock);
	return ret;
}
<<<<<<< HEAD

#define store_leds(nr)						\
static ssize_t store_engine##nr##_leds(struct device *dev,	\
			     struct device_attribute *attr,	\
			     const char *buf, size_t len)	\
{								\
	return store_engine_leds(dev, attr, buf, len, nr);	\
}
=======
>>>>>>> refs/remotes/origin/master
store_leds(1)
store_leds(2)
store_leds(3)

<<<<<<< HEAD
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
	lp5523_load_engine_and_select_page(chip);
	ret = lp5523_update_program_memory(chip, buf, len);

	mutex_unlock(&chip->lock);

	return ret;
}
store_load(1)
store_load(2)
store_load(3)

>>>>>>> refs/remotes/origin/master
static ssize_t lp5523_selftest(struct device *dev,
			       struct device_attribute *attr,
			       char *buf)
{
<<<<<<< HEAD
	struct i2c_client *client = to_i2c_client(dev);
	struct lp5523_chip *chip = i2c_get_clientdata(client);
	int i, ret, pos = 0;
	int led = 0;
=======
	struct lp55xx_led *led = i2c_get_clientdata(to_i2c_client(dev));
	struct lp55xx_chip *chip = led->chip;
	struct lp55xx_platform_data *pdata = chip->pdata;
	int i, ret, pos = 0;
>>>>>>> refs/remotes/origin/master
	u8 status, adc, vdd;

	mutex_lock(&chip->lock);

<<<<<<< HEAD
	ret = lp5523_read(chip->client, LP5523_REG_STATUS, &status);
=======
	ret = lp55xx_read(chip, LP5523_REG_STATUS, &status);
>>>>>>> refs/remotes/origin/master
	if (ret < 0)
		goto fail;

	/* Check that ext clock is really in use if requested */
<<<<<<< HEAD
	if ((chip->pdata) && (chip->pdata->clock_mode == LP5523_CLOCK_EXT))
		if  ((status & LP5523_EXT_CLK_USED) == 0)
			goto fail;

	/* Measure VDD (i.e. VBAT) first (channel 16 corresponds to VDD) */
	lp5523_write(chip->client, LP5523_REG_LED_TEST_CTRL,
				    LP5523_EN_LEDTEST | 16);
	usleep_range(3000, 6000); /* ADC conversion time is typically 2.7 ms */
	ret = lp5523_read(chip->client, LP5523_REG_STATUS, &status);
	if (!(status & LP5523_LEDTEST_DONE))
		usleep_range(3000, 6000); /* Was not ready. Wait little bit */

	ret |= lp5523_read(chip->client, LP5523_REG_LED_TEST_ADC, &vdd);
	vdd--;	/* There may be some fluctuation in measurement */

	for (i = 0; i < LP5523_LEDS; i++) {
		/* Skip non-existing channels */
		if (chip->pdata->led_config[i].led_current == 0)
			continue;

		/* Set default current */
		lp5523_write(chip->client,
			LP5523_REG_LED_CURRENT_BASE + i,
			chip->pdata->led_config[i].led_current);

		lp5523_write(chip->client, LP5523_REG_LED_PWM_BASE + i, 0xff);
		/* let current stabilize 2 - 4ms before measurements start */
		usleep_range(2000, 4000);
		lp5523_write(chip->client,
			     LP5523_REG_LED_TEST_CTRL,
			     LP5523_EN_LEDTEST | i);
		/* ADC conversion time is 2.7 ms typically */
		usleep_range(3000, 6000);
		ret = lp5523_read(chip->client, LP5523_REG_STATUS, &status);
		if (!(status & LP5523_LEDTEST_DONE))
			usleep_range(3000, 6000);/* Was not ready. Wait. */
		ret |= lp5523_read(chip->client, LP5523_REG_LED_TEST_ADC, &adc);
=======
	if (pdata->clock_mode == LP55XX_CLOCK_EXT) {
		if  ((status & LP5523_EXT_CLK_USED) == 0)
			goto fail;
	}

	/* Measure VDD (i.e. VBAT) first (channel 16 corresponds to VDD) */
	lp55xx_write(chip, LP5523_REG_LED_TEST_CTRL, LP5523_EN_LEDTEST | 16);
	usleep_range(3000, 6000); /* ADC conversion time is typically 2.7 ms */
	ret = lp55xx_read(chip, LP5523_REG_STATUS, &status);
	if (ret < 0)
		goto fail;

	if (!(status & LP5523_LEDTEST_DONE))
		usleep_range(3000, 6000); /* Was not ready. Wait little bit */

	ret = lp55xx_read(chip, LP5523_REG_LED_TEST_ADC, &vdd);
	if (ret < 0)
		goto fail;

	vdd--;	/* There may be some fluctuation in measurement */

	for (i = 0; i < LP5523_MAX_LEDS; i++) {
		/* Skip non-existing channels */
		if (pdata->led_config[i].led_current == 0)
			continue;

		/* Set default current */
		lp55xx_write(chip, LP5523_REG_LED_CURRENT_BASE + i,
			pdata->led_config[i].led_current);

		lp55xx_write(chip, LP5523_REG_LED_PWM_BASE + i, 0xff);
		/* let current stabilize 2 - 4ms before measurements start */
		usleep_range(2000, 4000);
		lp55xx_write(chip, LP5523_REG_LED_TEST_CTRL,
			     LP5523_EN_LEDTEST | i);
		/* ADC conversion time is 2.7 ms typically */
		usleep_range(3000, 6000);
		ret = lp55xx_read(chip, LP5523_REG_STATUS, &status);
		if (ret < 0)
			goto fail;

		if (!(status & LP5523_LEDTEST_DONE))
			usleep_range(3000, 6000);/* Was not ready. Wait. */

		ret = lp55xx_read(chip, LP5523_REG_LED_TEST_ADC, &adc);
		if (ret < 0)
			goto fail;
>>>>>>> refs/remotes/origin/master

		if (adc >= vdd || adc < LP5523_ADC_SHORTCIRC_LIM)
			pos += sprintf(buf + pos, "LED %d FAIL\n", i);

<<<<<<< HEAD
		lp5523_write(chip->client, LP5523_REG_LED_PWM_BASE + i, 0x00);

		/* Restore current */
		lp5523_write(chip->client,
			LP5523_REG_LED_CURRENT_BASE + i,
			chip->leds[led].led_current);
=======
		lp55xx_write(chip, LP5523_REG_LED_PWM_BASE + i, 0x00);

		/* Restore current */
		lp55xx_write(chip, LP5523_REG_LED_CURRENT_BASE + i,
			led->led_current);
>>>>>>> refs/remotes/origin/master
		led++;
	}
	if (pos == 0)
		pos = sprintf(buf, "OK\n");
	goto release_lock;
fail:
	pos = sprintf(buf, "FAIL\n");

release_lock:
	mutex_unlock(&chip->lock);

	return pos;
}

<<<<<<< HEAD
static void lp5523_set_brightness(struct led_classdev *cdev,
			     enum led_brightness brightness)
{
	struct lp5523_led *led = cdev_to_led(cdev);

	led->brightness = (u8)brightness;

	schedule_work(&led->brightness_work);
}

static void lp5523_led_brightness_work(struct work_struct *work)
{
	struct lp5523_led *led = container_of(work,
					      struct lp5523_led,
					      brightness_work);
	struct lp5523_chip *chip = led_to_lp5523(led);
	struct i2c_client *client = chip->client;

	mutex_lock(&chip->lock);

	lp5523_write(client, LP5523_REG_LED_PWM_BASE + led->chan_nr,
		     led->brightness);

	mutex_unlock(&chip->lock);
}

static int lp5523_do_store_load(struct lp5523_engine *engine,
				const char *buf, size_t len)
{
	struct lp5523_chip *chip = engine_to_lp5523(engine);
	struct i2c_client *client = chip->client;
	int  ret, nrchars, offset = 0, i = 0;
	char c[3];
	unsigned cmd;
	u8 pattern[LP5523_PROGRAM_LENGTH] = {0};

	while ((offset < len - 1) && (i < LP5523_PROGRAM_LENGTH)) {
		/* separate sscanfs because length is working only for %s */
		ret = sscanf(buf + offset, "%2s%n ", c, &nrchars);
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

	if (engine->mode == LP5523_CMD_LOAD)
		ret = lp5523_load_program(engine, pattern);
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
	struct lp5523_chip *chip = i2c_get_clientdata(client);
	return lp5523_do_store_load(&chip->engines[nr - 1], buf, len);
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

static ssize_t show_engine_mode(struct device *dev,
				struct device_attribute *attr,
				char *buf, int nr)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct lp5523_chip *chip = i2c_get_clientdata(client);
	switch (chip->engines[nr - 1].mode) {
	case LP5523_CMD_RUN:
		return sprintf(buf, "run\n");
	case LP5523_CMD_LOAD:
		return sprintf(buf, "load\n");
	case LP5523_CMD_DISABLED:
		return sprintf(buf, "disabled\n");
	default:
		return sprintf(buf, "disabled\n");
	}
}

#define show_mode(nr)							\
static ssize_t show_engine##nr##_mode(struct device *dev,		\
				    struct device_attribute *attr,	\
				    char *buf)				\
{									\
	return show_engine_mode(dev, attr, buf, nr);			\
}
show_mode(1)
show_mode(2)
show_mode(3)

static ssize_t store_engine_mode(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t len, int nr)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct lp5523_chip *chip = i2c_get_clientdata(client);
	struct lp5523_engine *engine = &chip->engines[nr - 1];
	mutex_lock(&chip->lock);

	if (!strncmp(buf, "run", 3))
		lp5523_set_mode(engine, LP5523_CMD_RUN);
	else if (!strncmp(buf, "load", 4))
		lp5523_set_mode(engine, LP5523_CMD_LOAD);
	else if (!strncmp(buf, "disabled", 8))
		lp5523_set_mode(engine, LP5523_CMD_DISABLED);

	mutex_unlock(&chip->lock);
	return len;
}

#define store_mode(nr)							\
static ssize_t store_engine##nr##_mode(struct device *dev,		\
				     struct device_attribute *attr,	\
				     const char *buf, size_t len)	\
{									\
	return store_engine_mode(dev, attr, buf, len, nr);		\
}
store_mode(1)
store_mode(2)
store_mode(3)

static ssize_t show_max_current(struct device *dev,
			    struct device_attribute *attr,
			    char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct lp5523_led *led = cdev_to_led(led_cdev);

	return sprintf(buf, "%d\n", led->max_current);
}

static ssize_t show_current(struct device *dev,
			    struct device_attribute *attr,
			    char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct lp5523_led *led = cdev_to_led(led_cdev);

	return sprintf(buf, "%d\n", led->led_current);
}

static ssize_t store_current(struct device *dev,
			     struct device_attribute *attr,
			     const char *buf, size_t len)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct lp5523_led *led = cdev_to_led(led_cdev);
	struct lp5523_chip *chip = led_to_lp5523(led);
	ssize_t ret;
	unsigned long curr;

	if (strict_strtoul(buf, 0, &curr))
		return -EINVAL;

	if (curr > led->max_current)
		return -EINVAL;

	mutex_lock(&chip->lock);
	ret = lp5523_write(chip->client,
			LP5523_REG_LED_CURRENT_BASE + led->chan_nr,
			(u8)curr);
	mutex_unlock(&chip->lock);

	if (ret < 0)
		return ret;

	led->led_current = (u8)curr;

	return len;
}

/* led class device attributes */
static DEVICE_ATTR(led_current, S_IRUGO | S_IWUSR, show_current, store_current);
static DEVICE_ATTR(max_current, S_IRUGO , show_max_current, NULL);

static struct attribute *lp5523_led_attributes[] = {
	&dev_attr_led_current.attr,
	&dev_attr_max_current.attr,
	NULL,
};

static struct attribute_group lp5523_led_attribute_group = {
	.attrs = lp5523_led_attributes
};

/* device attributes */
static DEVICE_ATTR(engine1_mode, S_IRUGO | S_IWUSR,
		   show_engine1_mode, store_engine1_mode);
static DEVICE_ATTR(engine2_mode, S_IRUGO | S_IWUSR,
		   show_engine2_mode, store_engine2_mode);
static DEVICE_ATTR(engine3_mode, S_IRUGO | S_IWUSR,
		   show_engine3_mode, store_engine3_mode);
static DEVICE_ATTR(engine1_leds, S_IRUGO | S_IWUSR,
		   show_engine1_leds, store_engine1_leds);
static DEVICE_ATTR(engine2_leds, S_IRUGO | S_IWUSR,
		   show_engine2_leds, store_engine2_leds);
static DEVICE_ATTR(engine3_leds, S_IRUGO | S_IWUSR,
		   show_engine3_leds, store_engine3_leds);
static DEVICE_ATTR(engine1_load, S_IWUSR, NULL, store_engine1_load);
static DEVICE_ATTR(engine2_load, S_IWUSR, NULL, store_engine2_load);
static DEVICE_ATTR(engine3_load, S_IWUSR, NULL, store_engine3_load);
static DEVICE_ATTR(selftest, S_IRUGO, lp5523_selftest, NULL);
=======
static void lp5523_led_brightness_work(struct work_struct *work)
{
	struct lp55xx_led *led = container_of(work, struct lp55xx_led,
					      brightness_work);
	struct lp55xx_chip *chip = led->chip;

	mutex_lock(&chip->lock);
	lp55xx_write(chip, LP5523_REG_LED_PWM_BASE + led->chan_nr,
		     led->brightness);
	mutex_unlock(&chip->lock);
}

static LP55XX_DEV_ATTR_RW(engine1_mode, show_engine1_mode, store_engine1_mode);
static LP55XX_DEV_ATTR_RW(engine2_mode, show_engine2_mode, store_engine2_mode);
static LP55XX_DEV_ATTR_RW(engine3_mode, show_engine3_mode, store_engine3_mode);
static LP55XX_DEV_ATTR_RW(engine1_leds, show_engine1_leds, store_engine1_leds);
static LP55XX_DEV_ATTR_RW(engine2_leds, show_engine2_leds, store_engine2_leds);
static LP55XX_DEV_ATTR_RW(engine3_leds, show_engine3_leds, store_engine3_leds);
static LP55XX_DEV_ATTR_WO(engine1_load, store_engine1_load);
static LP55XX_DEV_ATTR_WO(engine2_load, store_engine2_load);
static LP55XX_DEV_ATTR_WO(engine3_load, store_engine3_load);
static LP55XX_DEV_ATTR_RO(selftest, lp5523_selftest);
>>>>>>> refs/remotes/origin/master

static struct attribute *lp5523_attributes[] = {
	&dev_attr_engine1_mode.attr,
	&dev_attr_engine2_mode.attr,
	&dev_attr_engine3_mode.attr,
<<<<<<< HEAD
	&dev_attr_selftest.attr,
	&dev_attr_engine1_load.attr,
	&dev_attr_engine1_leds.attr,
	&dev_attr_engine2_load.attr,
	&dev_attr_engine2_leds.attr,
	&dev_attr_engine3_load.attr,
	&dev_attr_engine3_leds.attr,
=======
	&dev_attr_engine1_load.attr,
	&dev_attr_engine2_load.attr,
	&dev_attr_engine3_load.attr,
	&dev_attr_engine1_leds.attr,
	&dev_attr_engine2_leds.attr,
	&dev_attr_engine3_leds.attr,
	&dev_attr_selftest.attr,
	NULL,
>>>>>>> refs/remotes/origin/master
};

static const struct attribute_group lp5523_group = {
	.attrs = lp5523_attributes,
};

<<<<<<< HEAD
static int lp5523_register_sysfs(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	int ret;

	ret = sysfs_create_group(&dev->kobj, &lp5523_group);
	if (ret < 0)
		return ret;

	return 0;
}

static void lp5523_unregister_sysfs(struct i2c_client *client)
{
	struct lp5523_chip *chip = i2c_get_clientdata(client);
	struct device *dev = &client->dev;
	int i;

	sysfs_remove_group(&dev->kobj, &lp5523_group);

	for (i = 0; i < chip->num_leds; i++)
		sysfs_remove_group(&chip->leds[i].cdev.dev->kobj,
				&lp5523_led_attribute_group);
}

/*--------------------------------------------------------------*/
/*			Set chip operating mode			*/
/*--------------------------------------------------------------*/
static int lp5523_set_mode(struct lp5523_engine *engine, u8 mode)
{
	int ret = 0;

	/* if in that mode already do nothing, except for run */
	if (mode == engine->mode && mode != LP5523_CMD_RUN)
		return 0;

	if (mode == LP5523_CMD_RUN) {
		ret = lp5523_run_program(engine);
	} else if (mode == LP5523_CMD_LOAD) {
		lp5523_set_engine_mode(engine, LP5523_CMD_DISABLED);
		lp5523_set_engine_mode(engine, LP5523_CMD_LOAD);
	} else if (mode == LP5523_CMD_DISABLED) {
		lp5523_set_engine_mode(engine, LP5523_CMD_DISABLED);
	}

	engine->mode = mode;

	return ret;
}

/*--------------------------------------------------------------*/
/*			Probe, Attach, Remove			*/
/*--------------------------------------------------------------*/
static int __init lp5523_init_engine(struct lp5523_engine *engine, int id)
{
	if (id < 1 || id > LP5523_ENGINES)
		return -1;
	engine->id = id;
	engine->engine_mask = LP5523_ENG_MASK_BASE >> SHIFT_MASK(id);
	engine->prog_page = id - 1;
	engine->mux_page = id + 2;

	return 0;
}

static int __devinit lp5523_init_led(struct lp5523_led *led, struct device *dev,
			   int chan, struct lp5523_platform_data *pdata)
{
	char name[32];
	int res;

	if (chan >= LP5523_LEDS)
		return -EINVAL;

	if (pdata->led_config[chan].led_current) {
		led->led_current = pdata->led_config[chan].led_current;
		led->max_current = pdata->led_config[chan].max_current;
		led->chan_nr = pdata->led_config[chan].chan_nr;

		if (led->chan_nr >= LP5523_LEDS) {
			dev_err(dev, "Use channel numbers between 0 and %d\n",
				LP5523_LEDS - 1);
			return -EINVAL;
		}

		snprintf(name, sizeof(name), "%s:channel%d",
			pdata->label ?: "lp5523", chan);

		led->cdev.name = name;
		led->cdev.brightness_set = lp5523_set_brightness;
		res = led_classdev_register(dev, &led->cdev);
		if (res < 0) {
			dev_err(dev, "couldn't register led on channel %d\n",
				chan);
			return res;
		}
		res = sysfs_create_group(&led->cdev.dev->kobj,
				&lp5523_led_attribute_group);
		if (res < 0) {
			dev_err(dev, "couldn't register current attribute\n");
			led_classdev_unregister(&led->cdev);
			return res;
		}
	} else {
		led->led_current = 0;
	}
	return 0;
}

<<<<<<< HEAD
static struct i2c_driver lp5523_driver;

=======
>>>>>>> refs/remotes/origin/cm-10.0
static int __devinit lp5523_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct lp5523_chip		*chip;
	struct lp5523_platform_data	*pdata;
	int ret, i, led;

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

	lp5523_write(client, LP5523_REG_RESET, 0xff);
	usleep_range(10000, 20000); /*
				     * Exact value is not available. 10 - 20ms
				     * appears to be enough for reset.
				     */
	ret = lp5523_detect(client);
	if (ret)
		goto fail2;

	dev_info(&client->dev, "LP5523 Programmable led chip found\n");

	/* Initialize engines */
	for (i = 0; i < ARRAY_SIZE(chip->engines); i++) {
		ret = lp5523_init_engine(&chip->engines[i], i + 1);
		if (ret) {
			dev_err(&client->dev, "error initializing engine\n");
			goto fail2;
		}
	}
	ret = lp5523_configure(client);
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

		ret = lp5523_init_led(&chip->leds[led], &client->dev, i, pdata);
		if (ret) {
			dev_err(&client->dev, "error initializing leds\n");
			goto fail3;
		}
		chip->num_leds++;

		chip->leds[led].id = led;
		/* Set LED current */
		lp5523_write(client,
			  LP5523_REG_LED_CURRENT_BASE + chip->leds[led].chan_nr,
			  chip->leds[led].led_current);

		INIT_WORK(&(chip->leds[led].brightness_work),
			lp5523_led_brightness_work);

		led++;
	}

	ret = lp5523_register_sysfs(client);
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
=======
/* Chip specific configurations */
static struct lp55xx_device_config lp5523_cfg = {
	.reset = {
		.addr = LP5523_REG_RESET,
		.val  = LP5523_RESET,
	},
	.enable = {
		.addr = LP5523_REG_ENABLE,
		.val  = LP5523_ENABLE,
	},
	.max_channel  = LP5523_MAX_LEDS,
	.post_init_device   = lp5523_post_init_device,
	.brightness_work_fn = lp5523_led_brightness_work,
	.set_led_current    = lp5523_set_led_current,
	.firmware_cb        = lp5523_firmware_loaded,
	.run_engine         = lp5523_run_engine,
	.dev_attr_group     = &lp5523_group,
};

static int lp5523_probe(struct i2c_client *client,
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
	chip->cfg = &lp5523_cfg;

	mutex_init(&chip->lock);

	i2c_set_clientdata(client, led);

	ret = lp55xx_init_device(chip);
	if (ret)
		goto err_init;

	dev_info(&client->dev, "%s Programmable led chip found\n", id->name);

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
>>>>>>> refs/remotes/origin/master
	return ret;
}

static int lp5523_remove(struct i2c_client *client)
{
<<<<<<< HEAD
	struct lp5523_chip *chip = i2c_get_clientdata(client);
	int i;

	lp5523_unregister_sysfs(client);

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
	struct lp55xx_led *led = i2c_get_clientdata(client);
	struct lp55xx_chip *chip = led->chip;

	lp5523_stop_engine(chip);
	lp55xx_unregister_sysfs(chip);
	lp55xx_unregister_leds(led, chip);
	lp55xx_deinit_device(chip);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static const struct i2c_device_id lp5523_id[] = {
<<<<<<< HEAD
	{ "lp5523", 0 },
=======
	{ "lp5523",  LP5523 },
	{ "lp55231", LP55231 },
>>>>>>> refs/remotes/origin/master
	{ }
};

MODULE_DEVICE_TABLE(i2c, lp5523_id);

<<<<<<< HEAD
static struct i2c_driver lp5523_driver = {
	.driver = {
		.name	= "lp5523",
=======
#ifdef CONFIG_OF
static const struct of_device_id of_lp5523_leds_match[] = {
	{ .compatible = "national,lp5523", },
	{},
};

MODULE_DEVICE_TABLE(of, of_lp5523_leds_match);
#endif

static struct i2c_driver lp5523_driver = {
	.driver = {
		.name	= "lp5523x",
		.of_match_table = of_match_ptr(of_lp5523_leds_match),
>>>>>>> refs/remotes/origin/master
	},
	.probe		= lp5523_probe,
	.remove		= lp5523_remove,
	.id_table	= lp5523_id,
};

<<<<<<< HEAD
<<<<<<< HEAD
static int __init lp5523_init(void)
{
	int ret;

	ret = i2c_add_driver(&lp5523_driver);

	if (ret < 0)
		printk(KERN_ALERT "Adding lp5523 driver failed\n");

	return ret;
}

static void __exit lp5523_exit(void)
{
	i2c_del_driver(&lp5523_driver);
}

module_init(lp5523_init);
module_exit(lp5523_exit);
=======
module_i2c_driver(lp5523_driver);
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_AUTHOR("Mathias Nyman <mathias.nyman@nokia.com>");
=======
module_i2c_driver(lp5523_driver);

MODULE_AUTHOR("Mathias Nyman <mathias.nyman@nokia.com>");
MODULE_AUTHOR("Milo Kim <milo.kim@ti.com>");
>>>>>>> refs/remotes/origin/master
MODULE_DESCRIPTION("LP5523 LED engine");
MODULE_LICENSE("GPL");
