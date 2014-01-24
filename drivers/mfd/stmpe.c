/*
<<<<<<< HEAD
<<<<<<< HEAD
=======
 * ST Microelectronics MFD: stmpe's driver
 *
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * ST Microelectronics MFD: stmpe's driver
 *
>>>>>>> refs/remotes/origin/master
 * Copyright (C) ST-Ericsson SA 2010
 *
 * License Terms: GNU General Public License, version 2
 * Author: Rabin Vincent <rabin.vincent@stericsson.com> for ST-Ericsson
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/mfd/core.h>
#include <linux/mfd/stmpe.h>
=======
=======
#include <linux/err.h>
>>>>>>> refs/remotes/origin/master
#include <linux/gpio.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
<<<<<<< HEAD
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/mfd/core.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/irqdomain.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/mfd/core.h>
#include <linux/delay.h>
>>>>>>> refs/remotes/origin/master
#include "stmpe.h"

static int __stmpe_enable(struct stmpe *stmpe, unsigned int blocks)
{
	return stmpe->variant->enable(stmpe, blocks, true);
}

static int __stmpe_disable(struct stmpe *stmpe, unsigned int blocks)
{
	return stmpe->variant->enable(stmpe, blocks, false);
}

static int __stmpe_reg_read(struct stmpe *stmpe, u8 reg)
{
	int ret;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = i2c_smbus_read_byte_data(stmpe->i2c, reg);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to read reg %#x: %d\n",
			reg, ret);
=======
	ret = stmpe->ci->read_byte(stmpe, reg);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to read reg %#x: %d\n", reg, ret);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = stmpe->ci->read_byte(stmpe, reg);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to read reg %#x: %d\n", reg, ret);
>>>>>>> refs/remotes/origin/master

	dev_vdbg(stmpe->dev, "rd: reg %#x => data %#x\n", reg, ret);

	return ret;
}

static int __stmpe_reg_write(struct stmpe *stmpe, u8 reg, u8 val)
{
	int ret;

	dev_vdbg(stmpe->dev, "wr: reg %#x <= %#x\n", reg, val);

<<<<<<< HEAD
<<<<<<< HEAD
	ret = i2c_smbus_write_byte_data(stmpe->i2c, reg, val);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to write reg %#x: %d\n",
			reg, ret);
=======
	ret = stmpe->ci->write_byte(stmpe, reg, val);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to write reg %#x: %d\n", reg, ret);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = stmpe->ci->write_byte(stmpe, reg, val);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to write reg %#x: %d\n", reg, ret);
>>>>>>> refs/remotes/origin/master

	return ret;
}

static int __stmpe_set_bits(struct stmpe *stmpe, u8 reg, u8 mask, u8 val)
{
	int ret;

	ret = __stmpe_reg_read(stmpe, reg);
	if (ret < 0)
		return ret;

	ret &= ~mask;
	ret |= val;

	return __stmpe_reg_write(stmpe, reg, ret);
}

static int __stmpe_block_read(struct stmpe *stmpe, u8 reg, u8 length,
			      u8 *values)
{
	int ret;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = i2c_smbus_read_i2c_block_data(stmpe->i2c, reg, length, values);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to read regs %#x: %d\n",
			reg, ret);
=======
	ret = stmpe->ci->read_block(stmpe, reg, length, values);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to read regs %#x: %d\n", reg, ret);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = stmpe->ci->read_block(stmpe, reg, length, values);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to read regs %#x: %d\n", reg, ret);
>>>>>>> refs/remotes/origin/master

	dev_vdbg(stmpe->dev, "rd: reg %#x (%d) => ret %#x\n", reg, length, ret);
	stmpe_dump_bytes("stmpe rd: ", values, length);

	return ret;
}

static int __stmpe_block_write(struct stmpe *stmpe, u8 reg, u8 length,
			const u8 *values)
{
	int ret;

	dev_vdbg(stmpe->dev, "wr: regs %#x (%d)\n", reg, length);
	stmpe_dump_bytes("stmpe wr: ", values, length);

<<<<<<< HEAD
<<<<<<< HEAD
	ret = i2c_smbus_write_i2c_block_data(stmpe->i2c, reg, length,
					     values);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to write regs %#x: %d\n",
			reg, ret);
=======
	ret = stmpe->ci->write_block(stmpe, reg, length, values);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to write regs %#x: %d\n", reg, ret);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = stmpe->ci->write_block(stmpe, reg, length, values);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to write regs %#x: %d\n", reg, ret);
>>>>>>> refs/remotes/origin/master

	return ret;
}

/**
 * stmpe_enable - enable blocks on an STMPE device
 * @stmpe:	Device to work on
 * @blocks:	Mask of blocks (enum stmpe_block values) to enable
 */
int stmpe_enable(struct stmpe *stmpe, unsigned int blocks)
{
	int ret;

	mutex_lock(&stmpe->lock);
	ret = __stmpe_enable(stmpe, blocks);
	mutex_unlock(&stmpe->lock);

	return ret;
}
EXPORT_SYMBOL_GPL(stmpe_enable);

/**
 * stmpe_disable - disable blocks on an STMPE device
 * @stmpe:	Device to work on
 * @blocks:	Mask of blocks (enum stmpe_block values) to enable
 */
int stmpe_disable(struct stmpe *stmpe, unsigned int blocks)
{
	int ret;

	mutex_lock(&stmpe->lock);
	ret = __stmpe_disable(stmpe, blocks);
	mutex_unlock(&stmpe->lock);

	return ret;
}
EXPORT_SYMBOL_GPL(stmpe_disable);

/**
 * stmpe_reg_read() - read a single STMPE register
 * @stmpe:	Device to read from
 * @reg:	Register to read
 */
int stmpe_reg_read(struct stmpe *stmpe, u8 reg)
{
	int ret;

	mutex_lock(&stmpe->lock);
	ret = __stmpe_reg_read(stmpe, reg);
	mutex_unlock(&stmpe->lock);

	return ret;
}
EXPORT_SYMBOL_GPL(stmpe_reg_read);

/**
 * stmpe_reg_write() - write a single STMPE register
 * @stmpe:	Device to write to
 * @reg:	Register to write
 * @val:	Value to write
 */
int stmpe_reg_write(struct stmpe *stmpe, u8 reg, u8 val)
{
	int ret;

	mutex_lock(&stmpe->lock);
	ret = __stmpe_reg_write(stmpe, reg, val);
	mutex_unlock(&stmpe->lock);

	return ret;
}
EXPORT_SYMBOL_GPL(stmpe_reg_write);

/**
 * stmpe_set_bits() - set the value of a bitfield in a STMPE register
 * @stmpe:	Device to write to
 * @reg:	Register to write
 * @mask:	Mask of bits to set
 * @val:	Value to set
 */
int stmpe_set_bits(struct stmpe *stmpe, u8 reg, u8 mask, u8 val)
{
	int ret;

	mutex_lock(&stmpe->lock);
	ret = __stmpe_set_bits(stmpe, reg, mask, val);
	mutex_unlock(&stmpe->lock);

	return ret;
}
EXPORT_SYMBOL_GPL(stmpe_set_bits);

/**
 * stmpe_block_read() - read multiple STMPE registers
 * @stmpe:	Device to read from
 * @reg:	First register
 * @length:	Number of registers
 * @values:	Buffer to write to
 */
int stmpe_block_read(struct stmpe *stmpe, u8 reg, u8 length, u8 *values)
{
	int ret;

	mutex_lock(&stmpe->lock);
	ret = __stmpe_block_read(stmpe, reg, length, values);
	mutex_unlock(&stmpe->lock);

	return ret;
}
EXPORT_SYMBOL_GPL(stmpe_block_read);

/**
 * stmpe_block_write() - write multiple STMPE registers
 * @stmpe:	Device to write to
 * @reg:	First register
 * @length:	Number of registers
 * @values:	Values to write
 */
int stmpe_block_write(struct stmpe *stmpe, u8 reg, u8 length,
		      const u8 *values)
{
	int ret;

	mutex_lock(&stmpe->lock);
	ret = __stmpe_block_write(stmpe, reg, length, values);
	mutex_unlock(&stmpe->lock);

	return ret;
}
EXPORT_SYMBOL_GPL(stmpe_block_write);

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * stmpe_set_altfunc: set the alternate function for STMPE pins
=======
 * stmpe_set_altfunc()- set the alternate function for STMPE pins
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * stmpe_set_altfunc()- set the alternate function for STMPE pins
>>>>>>> refs/remotes/origin/master
 * @stmpe:	Device to configure
 * @pins:	Bitmask of pins to affect
 * @block:	block to enable alternate functions for
 *
 * @pins is assumed to have a bit set for each of the bits whose alternate
 * function is to be changed, numbered according to the GPIOXY numbers.
 *
 * If the GPIO module is not enabled, this function automatically enables it in
 * order to perform the change.
 */
int stmpe_set_altfunc(struct stmpe *stmpe, u32 pins, enum stmpe_block block)
{
	struct stmpe_variant_info *variant = stmpe->variant;
	u8 regaddr = stmpe->regs[STMPE_IDX_GPAFR_U_MSB];
	int af_bits = variant->af_bits;
	int numregs = DIV_ROUND_UP(stmpe->num_gpios * af_bits, 8);
<<<<<<< HEAD
<<<<<<< HEAD
	int afperreg = 8 / af_bits;
	int mask = (1 << af_bits) - 1;
	u8 regs[numregs];
	int af;
	int ret;

=======
=======
>>>>>>> refs/remotes/origin/master
	int mask = (1 << af_bits) - 1;
	u8 regs[numregs];
	int af, afperreg, ret;

	if (!variant->get_altfunc)
		return 0;

	afperreg = 8 / af_bits;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	mutex_lock(&stmpe->lock);

	ret = __stmpe_enable(stmpe, STMPE_BLOCK_GPIO);
	if (ret < 0)
		goto out;

	ret = __stmpe_block_read(stmpe, regaddr, numregs, regs);
	if (ret < 0)
		goto out;

	af = variant->get_altfunc(stmpe, block);

	while (pins) {
		int pin = __ffs(pins);
		int regoffset = numregs - (pin / afperreg) - 1;
		int pos = (pin % afperreg) * (8 / afperreg);

		regs[regoffset] &= ~(mask << pos);
		regs[regoffset] |= af << pos;

		pins &= ~(1 << pin);
	}

	ret = __stmpe_block_write(stmpe, regaddr, numregs, regs);

out:
	mutex_unlock(&stmpe->lock);
	return ret;
}
EXPORT_SYMBOL_GPL(stmpe_set_altfunc);

/*
 * GPIO (all variants)
 */

static struct resource stmpe_gpio_resources[] = {
	/* Start and end filled dynamically */
	{
		.flags	= IORESOURCE_IRQ,
	},
};

<<<<<<< HEAD
static struct mfd_cell stmpe_gpio_cell = {
	.name		= "stmpe-gpio",
=======
static const struct mfd_cell stmpe_gpio_cell = {
	.name		= "stmpe-gpio",
	.of_compatible	= "st,stmpe-gpio",
>>>>>>> refs/remotes/origin/master
	.resources	= stmpe_gpio_resources,
	.num_resources	= ARRAY_SIZE(stmpe_gpio_resources),
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
static struct mfd_cell stmpe_gpio_cell_noirq = {
	.name		= "stmpe-gpio",
	/* gpio cell resources consist of an irq only so no resources here */
};

>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct mfd_cell stmpe_gpio_cell_noirq = {
	.name		= "stmpe-gpio",
	.of_compatible	= "st,stmpe-gpio",
	/* gpio cell resources consist of an irq only so no resources here */
};

>>>>>>> refs/remotes/origin/master
/*
 * Keypad (1601, 2401, 2403)
 */

static struct resource stmpe_keypad_resources[] = {
	{
		.name	= "KEYPAD",
<<<<<<< HEAD
		.start	= 0,
		.end	= 0,
=======
>>>>>>> refs/remotes/origin/master
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "KEYPAD_OVER",
<<<<<<< HEAD
		.start	= 1,
		.end	= 1,
=======
>>>>>>> refs/remotes/origin/master
		.flags	= IORESOURCE_IRQ,
	},
};

<<<<<<< HEAD
static struct mfd_cell stmpe_keypad_cell = {
	.name		= "stmpe-keypad",
=======
static const struct mfd_cell stmpe_keypad_cell = {
	.name		= "stmpe-keypad",
	.of_compatible  = "st,stmpe-keypad",
>>>>>>> refs/remotes/origin/master
	.resources	= stmpe_keypad_resources,
	.num_resources	= ARRAY_SIZE(stmpe_keypad_resources),
};

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Touchscreen (STMPE811)
=======
=======
>>>>>>> refs/remotes/origin/master
 * STMPE801
 */
static const u8 stmpe801_regs[] = {
	[STMPE_IDX_CHIP_ID]	= STMPE801_REG_CHIP_ID,
	[STMPE_IDX_ICR_LSB]	= STMPE801_REG_SYS_CTRL,
	[STMPE_IDX_GPMR_LSB]	= STMPE801_REG_GPIO_MP_STA,
	[STMPE_IDX_GPSR_LSB]	= STMPE801_REG_GPIO_SET_PIN,
	[STMPE_IDX_GPCR_LSB]	= STMPE801_REG_GPIO_SET_PIN,
	[STMPE_IDX_GPDR_LSB]	= STMPE801_REG_GPIO_DIR,
	[STMPE_IDX_IEGPIOR_LSB] = STMPE801_REG_GPIO_INT_EN,
	[STMPE_IDX_ISGPIOR_MSB] = STMPE801_REG_GPIO_INT_STA,

};

static struct stmpe_variant_block stmpe801_blocks[] = {
	{
		.cell	= &stmpe_gpio_cell,
		.irq	= 0,
		.block	= STMPE_BLOCK_GPIO,
	},
};

static struct stmpe_variant_block stmpe801_blocks_noirq[] = {
	{
		.cell	= &stmpe_gpio_cell_noirq,
		.block	= STMPE_BLOCK_GPIO,
	},
};

static int stmpe801_enable(struct stmpe *stmpe, unsigned int blocks,
			   bool enable)
{
	if (blocks & STMPE_BLOCK_GPIO)
		return 0;
	else
		return -EINVAL;
}

static struct stmpe_variant_info stmpe801 = {
	.name		= "stmpe801",
	.id_val		= STMPE801_ID,
	.id_mask	= 0xffff,
	.num_gpios	= 8,
	.regs		= stmpe801_regs,
	.blocks		= stmpe801_blocks,
	.num_blocks	= ARRAY_SIZE(stmpe801_blocks),
	.num_irqs	= STMPE801_NR_INTERNAL_IRQS,
	.enable		= stmpe801_enable,
};

static struct stmpe_variant_info stmpe801_noirq = {
	.name		= "stmpe801",
	.id_val		= STMPE801_ID,
	.id_mask	= 0xffff,
	.num_gpios	= 8,
	.regs		= stmpe801_regs,
	.blocks		= stmpe801_blocks_noirq,
	.num_blocks	= ARRAY_SIZE(stmpe801_blocks_noirq),
	.enable		= stmpe801_enable,
};

/*
 * Touchscreen (STMPE811 or STMPE610)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */

static struct resource stmpe_ts_resources[] = {
	{
		.name	= "TOUCH_DET",
<<<<<<< HEAD
		.start	= 0,
		.end	= 0,
=======
>>>>>>> refs/remotes/origin/master
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "FIFO_TH",
<<<<<<< HEAD
		.start	= 1,
		.end	= 1,
=======
>>>>>>> refs/remotes/origin/master
		.flags	= IORESOURCE_IRQ,
	},
};

<<<<<<< HEAD
static struct mfd_cell stmpe_ts_cell = {
	.name		= "stmpe-ts",
=======
static const struct mfd_cell stmpe_ts_cell = {
	.name		= "stmpe-ts",
	.of_compatible	= "st,stmpe-ts",
>>>>>>> refs/remotes/origin/master
	.resources	= stmpe_ts_resources,
	.num_resources	= ARRAY_SIZE(stmpe_ts_resources),
};

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * STMPE811
=======
 * STMPE811 or STMPE610
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * STMPE811 or STMPE610
>>>>>>> refs/remotes/origin/master
 */

static const u8 stmpe811_regs[] = {
	[STMPE_IDX_CHIP_ID]	= STMPE811_REG_CHIP_ID,
	[STMPE_IDX_ICR_LSB]	= STMPE811_REG_INT_CTRL,
	[STMPE_IDX_IER_LSB]	= STMPE811_REG_INT_EN,
	[STMPE_IDX_ISR_MSB]	= STMPE811_REG_INT_STA,
	[STMPE_IDX_GPMR_LSB]	= STMPE811_REG_GPIO_MP_STA,
	[STMPE_IDX_GPSR_LSB]	= STMPE811_REG_GPIO_SET_PIN,
	[STMPE_IDX_GPCR_LSB]	= STMPE811_REG_GPIO_CLR_PIN,
	[STMPE_IDX_GPDR_LSB]	= STMPE811_REG_GPIO_DIR,
	[STMPE_IDX_GPRER_LSB]	= STMPE811_REG_GPIO_RE,
	[STMPE_IDX_GPFER_LSB]	= STMPE811_REG_GPIO_FE,
	[STMPE_IDX_GPAFR_U_MSB]	= STMPE811_REG_GPIO_AF,
	[STMPE_IDX_IEGPIOR_LSB]	= STMPE811_REG_GPIO_INT_EN,
	[STMPE_IDX_ISGPIOR_MSB]	= STMPE811_REG_GPIO_INT_STA,
	[STMPE_IDX_GPEDR_MSB]	= STMPE811_REG_GPIO_ED,
};

static struct stmpe_variant_block stmpe811_blocks[] = {
	{
		.cell	= &stmpe_gpio_cell,
		.irq	= STMPE811_IRQ_GPIOC,
		.block	= STMPE_BLOCK_GPIO,
	},
	{
		.cell	= &stmpe_ts_cell,
		.irq	= STMPE811_IRQ_TOUCH_DET,
		.block	= STMPE_BLOCK_TOUCHSCREEN,
	},
};

static int stmpe811_enable(struct stmpe *stmpe, unsigned int blocks,
			   bool enable)
{
	unsigned int mask = 0;

	if (blocks & STMPE_BLOCK_GPIO)
		mask |= STMPE811_SYS_CTRL2_GPIO_OFF;

	if (blocks & STMPE_BLOCK_ADC)
		mask |= STMPE811_SYS_CTRL2_ADC_OFF;

	if (blocks & STMPE_BLOCK_TOUCHSCREEN)
		mask |= STMPE811_SYS_CTRL2_TSC_OFF;

	return __stmpe_set_bits(stmpe, STMPE811_REG_SYS_CTRL2, mask,
				enable ? 0 : mask);
}

static int stmpe811_get_altfunc(struct stmpe *stmpe, enum stmpe_block block)
{
	/* 0 for touchscreen, 1 for GPIO */
	return block != STMPE_BLOCK_TOUCHSCREEN;
}

static struct stmpe_variant_info stmpe811 = {
	.name		= "stmpe811",
	.id_val		= 0x0811,
	.id_mask	= 0xffff,
	.num_gpios	= 8,
	.af_bits	= 1,
	.regs		= stmpe811_regs,
	.blocks		= stmpe811_blocks,
	.num_blocks	= ARRAY_SIZE(stmpe811_blocks),
	.num_irqs	= STMPE811_NR_INTERNAL_IRQS,
	.enable		= stmpe811_enable,
	.get_altfunc	= stmpe811_get_altfunc,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/* Similar to 811, except number of gpios */
static struct stmpe_variant_info stmpe610 = {
	.name		= "stmpe610",
	.id_val		= 0x0811,
	.id_mask	= 0xffff,
	.num_gpios	= 6,
	.af_bits	= 1,
	.regs		= stmpe811_regs,
	.blocks		= stmpe811_blocks,
	.num_blocks	= ARRAY_SIZE(stmpe811_blocks),
	.num_irqs	= STMPE811_NR_INTERNAL_IRQS,
	.enable		= stmpe811_enable,
	.get_altfunc	= stmpe811_get_altfunc,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * STMPE1601
 */

static const u8 stmpe1601_regs[] = {
	[STMPE_IDX_CHIP_ID]	= STMPE1601_REG_CHIP_ID,
	[STMPE_IDX_ICR_LSB]	= STMPE1601_REG_ICR_LSB,
	[STMPE_IDX_IER_LSB]	= STMPE1601_REG_IER_LSB,
	[STMPE_IDX_ISR_MSB]	= STMPE1601_REG_ISR_MSB,
	[STMPE_IDX_GPMR_LSB]	= STMPE1601_REG_GPIO_MP_LSB,
	[STMPE_IDX_GPSR_LSB]	= STMPE1601_REG_GPIO_SET_LSB,
	[STMPE_IDX_GPCR_LSB]	= STMPE1601_REG_GPIO_CLR_LSB,
	[STMPE_IDX_GPDR_LSB]	= STMPE1601_REG_GPIO_SET_DIR_LSB,
	[STMPE_IDX_GPRER_LSB]	= STMPE1601_REG_GPIO_RE_LSB,
	[STMPE_IDX_GPFER_LSB]	= STMPE1601_REG_GPIO_FE_LSB,
	[STMPE_IDX_GPAFR_U_MSB]	= STMPE1601_REG_GPIO_AF_U_MSB,
	[STMPE_IDX_IEGPIOR_LSB]	= STMPE1601_REG_INT_EN_GPIO_MASK_LSB,
	[STMPE_IDX_ISGPIOR_MSB]	= STMPE1601_REG_INT_STA_GPIO_MSB,
	[STMPE_IDX_GPEDR_MSB]	= STMPE1601_REG_GPIO_ED_MSB,
};

static struct stmpe_variant_block stmpe1601_blocks[] = {
	{
		.cell	= &stmpe_gpio_cell,
<<<<<<< HEAD
		.irq	= STMPE24XX_IRQ_GPIOC,
=======
		.irq	= STMPE1601_IRQ_GPIOC,
>>>>>>> refs/remotes/origin/master
		.block	= STMPE_BLOCK_GPIO,
	},
	{
		.cell	= &stmpe_keypad_cell,
<<<<<<< HEAD
		.irq	= STMPE24XX_IRQ_KEYPAD,
=======
		.irq	= STMPE1601_IRQ_KEYPAD,
>>>>>>> refs/remotes/origin/master
		.block	= STMPE_BLOCK_KEYPAD,
	},
};

/* supported autosleep timeout delay (in msecs) */
static const int stmpe_autosleep_delay[] = {
	4, 16, 32, 64, 128, 256, 512, 1024,
};

static int stmpe_round_timeout(int timeout)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(stmpe_autosleep_delay); i++) {
		if (stmpe_autosleep_delay[i] >= timeout)
			return i;
	}

	/*
	 * requests for delays longer than supported should not return the
	 * longest supported delay
	 */
	return -EINVAL;
}

static int stmpe_autosleep(struct stmpe *stmpe, int autosleep_timeout)
{
	int ret;

	if (!stmpe->variant->enable_autosleep)
		return -ENOSYS;

	mutex_lock(&stmpe->lock);
	ret = stmpe->variant->enable_autosleep(stmpe, autosleep_timeout);
	mutex_unlock(&stmpe->lock);

	return ret;
}

/*
 * Both stmpe 1601/2403 support same layout for autosleep
 */
static int stmpe1601_autosleep(struct stmpe *stmpe,
		int autosleep_timeout)
{
	int ret, timeout;

	/* choose the best available timeout */
	timeout = stmpe_round_timeout(autosleep_timeout);
	if (timeout < 0) {
		dev_err(stmpe->dev, "invalid timeout\n");
		return timeout;
	}

	ret = __stmpe_set_bits(stmpe, STMPE1601_REG_SYS_CTRL2,
			STMPE1601_AUTOSLEEP_TIMEOUT_MASK,
			timeout);
	if (ret < 0)
		return ret;

	return __stmpe_set_bits(stmpe, STMPE1601_REG_SYS_CTRL2,
			STPME1601_AUTOSLEEP_ENABLE,
			STPME1601_AUTOSLEEP_ENABLE);
}

static int stmpe1601_enable(struct stmpe *stmpe, unsigned int blocks,
			    bool enable)
{
	unsigned int mask = 0;

	if (blocks & STMPE_BLOCK_GPIO)
		mask |= STMPE1601_SYS_CTRL_ENABLE_GPIO;

	if (blocks & STMPE_BLOCK_KEYPAD)
		mask |= STMPE1601_SYS_CTRL_ENABLE_KPC;

	return __stmpe_set_bits(stmpe, STMPE1601_REG_SYS_CTRL, mask,
				enable ? mask : 0);
}

static int stmpe1601_get_altfunc(struct stmpe *stmpe, enum stmpe_block block)
{
	switch (block) {
	case STMPE_BLOCK_PWM:
		return 2;

	case STMPE_BLOCK_KEYPAD:
		return 1;

	case STMPE_BLOCK_GPIO:
	default:
		return 0;
	}
}

static struct stmpe_variant_info stmpe1601 = {
	.name		= "stmpe1601",
	.id_val		= 0x0210,
	.id_mask	= 0xfff0,	/* at least 0x0210 and 0x0212 */
	.num_gpios	= 16,
	.af_bits	= 2,
	.regs		= stmpe1601_regs,
	.blocks		= stmpe1601_blocks,
	.num_blocks	= ARRAY_SIZE(stmpe1601_blocks),
	.num_irqs	= STMPE1601_NR_INTERNAL_IRQS,
	.enable		= stmpe1601_enable,
	.get_altfunc	= stmpe1601_get_altfunc,
	.enable_autosleep	= stmpe1601_autosleep,
};

/*
<<<<<<< HEAD
=======
 * STMPE1801
 */
static const u8 stmpe1801_regs[] = {
	[STMPE_IDX_CHIP_ID]	= STMPE1801_REG_CHIP_ID,
	[STMPE_IDX_ICR_LSB]	= STMPE1801_REG_INT_CTRL_LOW,
	[STMPE_IDX_IER_LSB]	= STMPE1801_REG_INT_EN_MASK_LOW,
	[STMPE_IDX_ISR_LSB]	= STMPE1801_REG_INT_STA_LOW,
	[STMPE_IDX_GPMR_LSB]	= STMPE1801_REG_GPIO_MP_LOW,
	[STMPE_IDX_GPSR_LSB]	= STMPE1801_REG_GPIO_SET_LOW,
	[STMPE_IDX_GPCR_LSB]	= STMPE1801_REG_GPIO_CLR_LOW,
	[STMPE_IDX_GPDR_LSB]	= STMPE1801_REG_GPIO_SET_DIR_LOW,
	[STMPE_IDX_GPRER_LSB]	= STMPE1801_REG_GPIO_RE_LOW,
	[STMPE_IDX_GPFER_LSB]	= STMPE1801_REG_GPIO_FE_LOW,
	[STMPE_IDX_IEGPIOR_LSB]	= STMPE1801_REG_INT_EN_GPIO_MASK_LOW,
	[STMPE_IDX_ISGPIOR_LSB]	= STMPE1801_REG_INT_STA_GPIO_LOW,
};

static struct stmpe_variant_block stmpe1801_blocks[] = {
	{
		.cell	= &stmpe_gpio_cell,
		.irq	= STMPE1801_IRQ_GPIOC,
		.block	= STMPE_BLOCK_GPIO,
	},
	{
		.cell	= &stmpe_keypad_cell,
		.irq	= STMPE1801_IRQ_KEYPAD,
		.block	= STMPE_BLOCK_KEYPAD,
	},
};

static int stmpe1801_enable(struct stmpe *stmpe, unsigned int blocks,
			    bool enable)
{
	unsigned int mask = 0;
	if (blocks & STMPE_BLOCK_GPIO)
		mask |= STMPE1801_MSK_INT_EN_GPIO;

	if (blocks & STMPE_BLOCK_KEYPAD)
		mask |= STMPE1801_MSK_INT_EN_KPC;

	return __stmpe_set_bits(stmpe, STMPE1801_REG_INT_EN_MASK_LOW, mask,
				enable ? mask : 0);
}

static int stmpe1801_reset(struct stmpe *stmpe)
{
	unsigned long timeout;
	int ret = 0;

	ret = __stmpe_set_bits(stmpe, STMPE1801_REG_SYS_CTRL,
		STMPE1801_MSK_SYS_CTRL_RESET, STMPE1801_MSK_SYS_CTRL_RESET);
	if (ret < 0)
		return ret;

	timeout = jiffies + msecs_to_jiffies(100);
	while (time_before(jiffies, timeout)) {
		ret = __stmpe_reg_read(stmpe, STMPE1801_REG_SYS_CTRL);
		if (ret < 0)
			return ret;
		if (!(ret & STMPE1801_MSK_SYS_CTRL_RESET))
			return 0;
		usleep_range(100, 200);
	};
	return -EIO;
}

static struct stmpe_variant_info stmpe1801 = {
	.name		= "stmpe1801",
	.id_val		= STMPE1801_ID,
	.id_mask	= 0xfff0,
	.num_gpios	= 18,
	.af_bits	= 0,
	.regs		= stmpe1801_regs,
	.blocks		= stmpe1801_blocks,
	.num_blocks	= ARRAY_SIZE(stmpe1801_blocks),
	.num_irqs	= STMPE1801_NR_INTERNAL_IRQS,
	.enable		= stmpe1801_enable,
	/* stmpe1801 do not have any gpio alternate function */
	.get_altfunc	= NULL,
};

/*
>>>>>>> refs/remotes/origin/master
 * STMPE24XX
 */

static const u8 stmpe24xx_regs[] = {
	[STMPE_IDX_CHIP_ID]	= STMPE24XX_REG_CHIP_ID,
	[STMPE_IDX_ICR_LSB]	= STMPE24XX_REG_ICR_LSB,
	[STMPE_IDX_IER_LSB]	= STMPE24XX_REG_IER_LSB,
	[STMPE_IDX_ISR_MSB]	= STMPE24XX_REG_ISR_MSB,
	[STMPE_IDX_GPMR_LSB]	= STMPE24XX_REG_GPMR_LSB,
	[STMPE_IDX_GPSR_LSB]	= STMPE24XX_REG_GPSR_LSB,
	[STMPE_IDX_GPCR_LSB]	= STMPE24XX_REG_GPCR_LSB,
	[STMPE_IDX_GPDR_LSB]	= STMPE24XX_REG_GPDR_LSB,
	[STMPE_IDX_GPRER_LSB]	= STMPE24XX_REG_GPRER_LSB,
	[STMPE_IDX_GPFER_LSB]	= STMPE24XX_REG_GPFER_LSB,
	[STMPE_IDX_GPAFR_U_MSB]	= STMPE24XX_REG_GPAFR_U_MSB,
	[STMPE_IDX_IEGPIOR_LSB]	= STMPE24XX_REG_IEGPIOR_LSB,
	[STMPE_IDX_ISGPIOR_MSB]	= STMPE24XX_REG_ISGPIOR_MSB,
	[STMPE_IDX_GPEDR_MSB]	= STMPE24XX_REG_GPEDR_MSB,
};

static struct stmpe_variant_block stmpe24xx_blocks[] = {
	{
		.cell	= &stmpe_gpio_cell,
		.irq	= STMPE24XX_IRQ_GPIOC,
		.block	= STMPE_BLOCK_GPIO,
	},
	{
		.cell	= &stmpe_keypad_cell,
		.irq	= STMPE24XX_IRQ_KEYPAD,
		.block	= STMPE_BLOCK_KEYPAD,
	},
};

static int stmpe24xx_enable(struct stmpe *stmpe, unsigned int blocks,
			    bool enable)
{
	unsigned int mask = 0;

	if (blocks & STMPE_BLOCK_GPIO)
		mask |= STMPE24XX_SYS_CTRL_ENABLE_GPIO;

	if (blocks & STMPE_BLOCK_KEYPAD)
		mask |= STMPE24XX_SYS_CTRL_ENABLE_KPC;

	return __stmpe_set_bits(stmpe, STMPE24XX_REG_SYS_CTRL, mask,
				enable ? mask : 0);
}

static int stmpe24xx_get_altfunc(struct stmpe *stmpe, enum stmpe_block block)
{
	switch (block) {
	case STMPE_BLOCK_ROTATOR:
		return 2;

	case STMPE_BLOCK_KEYPAD:
		return 1;

	case STMPE_BLOCK_GPIO:
	default:
		return 0;
	}
}

static struct stmpe_variant_info stmpe2401 = {
	.name		= "stmpe2401",
	.id_val		= 0x0101,
	.id_mask	= 0xffff,
	.num_gpios	= 24,
	.af_bits	= 2,
	.regs		= stmpe24xx_regs,
	.blocks		= stmpe24xx_blocks,
	.num_blocks	= ARRAY_SIZE(stmpe24xx_blocks),
	.num_irqs	= STMPE24XX_NR_INTERNAL_IRQS,
	.enable		= stmpe24xx_enable,
	.get_altfunc	= stmpe24xx_get_altfunc,
};

static struct stmpe_variant_info stmpe2403 = {
	.name		= "stmpe2403",
	.id_val		= 0x0120,
	.id_mask	= 0xffff,
	.num_gpios	= 24,
	.af_bits	= 2,
	.regs		= stmpe24xx_regs,
	.blocks		= stmpe24xx_blocks,
	.num_blocks	= ARRAY_SIZE(stmpe24xx_blocks),
	.num_irqs	= STMPE24XX_NR_INTERNAL_IRQS,
	.enable		= stmpe24xx_enable,
	.get_altfunc	= stmpe24xx_get_altfunc,
	.enable_autosleep	= stmpe1601_autosleep, /* same as stmpe1601 */
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct stmpe_variant_info *stmpe_variant_info[] = {
=======
static struct stmpe_variant_info *stmpe_variant_info[STMPE_NBR_PARTS] = {
	[STMPE610]	= &stmpe610,
	[STMPE801]	= &stmpe801,
>>>>>>> refs/remotes/origin/cm-10.0
	[STMPE811]	= &stmpe811,
	[STMPE1601]	= &stmpe1601,
=======
static struct stmpe_variant_info *stmpe_variant_info[STMPE_NBR_PARTS] = {
	[STMPE610]	= &stmpe610,
	[STMPE801]	= &stmpe801,
	[STMPE811]	= &stmpe811,
	[STMPE1601]	= &stmpe1601,
	[STMPE1801]	= &stmpe1801,
>>>>>>> refs/remotes/origin/master
	[STMPE2401]	= &stmpe2401,
	[STMPE2403]	= &stmpe2403,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * These devices can be connected in a 'no-irq' configuration - the irq pin
 * is not used and the device cannot interrupt the CPU. Here we only list
 * devices which support this configuration - the driver will fail probing
 * for any devices not listed here which are configured in this way.
 */
static struct stmpe_variant_info *stmpe_noirq_variant_info[STMPE_NBR_PARTS] = {
	[STMPE801]	= &stmpe801_noirq,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static irqreturn_t stmpe_irq(int irq, void *data)
{
	struct stmpe *stmpe = data;
	struct stmpe_variant_info *variant = stmpe->variant;
	int num = DIV_ROUND_UP(variant->num_irqs, 8);
<<<<<<< HEAD
	u8 israddr = stmpe->regs[STMPE_IDX_ISR_MSB];
=======
	u8 israddr;
>>>>>>> refs/remotes/origin/master
	u8 isr[num];
	int ret;
	int i;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (variant->id_val == STMPE801_ID) {
		handle_nested_irq(stmpe->irq_base);
		return IRQ_HANDLED;
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (variant->id_val == STMPE801_ID) {
		int base = irq_create_mapping(stmpe->domain, 0);

		handle_nested_irq(base);
		return IRQ_HANDLED;
	}

	if (variant->id_val == STMPE1801_ID)
		israddr = stmpe->regs[STMPE_IDX_ISR_LSB];
	else
		israddr = stmpe->regs[STMPE_IDX_ISR_MSB];

>>>>>>> refs/remotes/origin/master
	ret = stmpe_block_read(stmpe, israddr, num, isr);
	if (ret < 0)
		return IRQ_NONE;

	for (i = 0; i < num; i++) {
		int bank = num - i - 1;
		u8 status = isr[i];
		u8 clear;

		status &= stmpe->ier[bank];
		if (!status)
			continue;

		clear = status;
		while (status) {
			int bit = __ffs(status);
			int line = bank * 8 + bit;
<<<<<<< HEAD

			handle_nested_irq(stmpe->irq_base + line);
=======
			int nestedirq = irq_create_mapping(stmpe->domain, line);

			handle_nested_irq(nestedirq);
>>>>>>> refs/remotes/origin/master
			status &= ~(1 << bit);
		}

		stmpe_reg_write(stmpe, israddr + i, clear);
	}

	return IRQ_HANDLED;
}

static void stmpe_irq_lock(struct irq_data *data)
{
	struct stmpe *stmpe = irq_data_get_irq_chip_data(data);

	mutex_lock(&stmpe->irq_lock);
}

static void stmpe_irq_sync_unlock(struct irq_data *data)
{
	struct stmpe *stmpe = irq_data_get_irq_chip_data(data);
	struct stmpe_variant_info *variant = stmpe->variant;
	int num = DIV_ROUND_UP(variant->num_irqs, 8);
	int i;

	for (i = 0; i < num; i++) {
		u8 new = stmpe->ier[i];
		u8 old = stmpe->oldier[i];

		if (new == old)
			continue;

		stmpe->oldier[i] = new;
		stmpe_reg_write(stmpe, stmpe->regs[STMPE_IDX_IER_LSB] - i, new);
	}

	mutex_unlock(&stmpe->irq_lock);
}

static void stmpe_irq_mask(struct irq_data *data)
{
	struct stmpe *stmpe = irq_data_get_irq_chip_data(data);
<<<<<<< HEAD
	int offset = data->irq - stmpe->irq_base;
=======
	int offset = data->hwirq;
>>>>>>> refs/remotes/origin/master
	int regoffset = offset / 8;
	int mask = 1 << (offset % 8);

	stmpe->ier[regoffset] &= ~mask;
}

static void stmpe_irq_unmask(struct irq_data *data)
{
	struct stmpe *stmpe = irq_data_get_irq_chip_data(data);
<<<<<<< HEAD
	int offset = data->irq - stmpe->irq_base;
=======
	int offset = data->hwirq;
>>>>>>> refs/remotes/origin/master
	int regoffset = offset / 8;
	int mask = 1 << (offset % 8);

	stmpe->ier[regoffset] |= mask;
}

static struct irq_chip stmpe_irq_chip = {
	.name			= "stmpe",
	.irq_bus_lock		= stmpe_irq_lock,
	.irq_bus_sync_unlock	= stmpe_irq_sync_unlock,
	.irq_mask		= stmpe_irq_mask,
	.irq_unmask		= stmpe_irq_unmask,
};

<<<<<<< HEAD
static int __devinit stmpe_irq_init(struct stmpe *stmpe)
{
<<<<<<< HEAD
=======
	struct irq_chip *chip = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
	int num_irqs = stmpe->variant->num_irqs;
	int base = stmpe->irq_base;
	int irq;

<<<<<<< HEAD
	for (irq = base; irq < base + num_irqs; irq++) {
		irq_set_chip_data(irq, stmpe);
		irq_set_chip_and_handler(irq, &stmpe_irq_chip,
					 handle_edge_irq);
=======
	if (stmpe->variant->id_val != STMPE801_ID)
		chip = &stmpe_irq_chip;

	for (irq = base; irq < base + num_irqs; irq++) {
		irq_set_chip_data(irq, stmpe);
		irq_set_chip_and_handler(irq, chip, handle_edge_irq);
>>>>>>> refs/remotes/origin/cm-10.0
		irq_set_nested_thread(irq, 1);
#ifdef CONFIG_ARM
		set_irq_flags(irq, IRQF_VALID);
#else
		irq_set_noprobe(irq);
#endif
	}
=======
static int stmpe_irq_map(struct irq_domain *d, unsigned int virq,
                                irq_hw_number_t hwirq)
{
	struct stmpe *stmpe = d->host_data;
	struct irq_chip *chip = NULL;

	if (stmpe->variant->id_val != STMPE801_ID)
		chip = &stmpe_irq_chip;

	irq_set_chip_data(virq, stmpe);
	irq_set_chip_and_handler(virq, chip, handle_edge_irq);
	irq_set_nested_thread(virq, 1);
#ifdef CONFIG_ARM
	set_irq_flags(virq, IRQF_VALID);
#else
	irq_set_noprobe(virq);
#endif
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static void stmpe_irq_remove(struct stmpe *stmpe)
{
	int num_irqs = stmpe->variant->num_irqs;
	int base = stmpe->irq_base;
	int irq;

	for (irq = base; irq < base + num_irqs; irq++) {
#ifdef CONFIG_ARM
		set_irq_flags(irq, 0);
#endif
		irq_set_chip_and_handler(irq, NULL, NULL);
		irq_set_chip_data(irq, NULL);
	}
}

static int __devinit stmpe_chip_init(struct stmpe *stmpe)
=======
static void stmpe_irq_unmap(struct irq_domain *d, unsigned int virq)
{
#ifdef CONFIG_ARM
		set_irq_flags(virq, 0);
#endif
		irq_set_chip_and_handler(virq, NULL, NULL);
		irq_set_chip_data(virq, NULL);
}

static struct irq_domain_ops stmpe_irq_ops = {
        .map    = stmpe_irq_map,
        .unmap  = stmpe_irq_unmap,
        .xlate  = irq_domain_xlate_twocell,
};

static int stmpe_irq_init(struct stmpe *stmpe, struct device_node *np)
{
	int base = 0;
	int num_irqs = stmpe->variant->num_irqs;

	if (!np)
		base = stmpe->irq_base;

	stmpe->domain = irq_domain_add_simple(np, num_irqs, base,
					      &stmpe_irq_ops, stmpe);
	if (!stmpe->domain) {
		dev_err(stmpe->dev, "Failed to create irqdomain\n");
		return -ENOSYS;
	}

	return 0;
}

static int stmpe_chip_init(struct stmpe *stmpe)
>>>>>>> refs/remotes/origin/master
{
	unsigned int irq_trigger = stmpe->pdata->irq_trigger;
	int autosleep_timeout = stmpe->pdata->autosleep_timeout;
	struct stmpe_variant_info *variant = stmpe->variant;
<<<<<<< HEAD
<<<<<<< HEAD
	u8 icr = STMPE_ICR_LSB_GIM;
=======
	u8 icr = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u8 icr = 0;
>>>>>>> refs/remotes/origin/master
	unsigned int id;
	u8 data[2];
	int ret;

	ret = stmpe_block_read(stmpe, stmpe->regs[STMPE_IDX_CHIP_ID],
			       ARRAY_SIZE(data), data);
	if (ret < 0)
		return ret;

	id = (data[0] << 8) | data[1];
	if ((id & variant->id_mask) != variant->id_val) {
		dev_err(stmpe->dev, "unknown chip id: %#x\n", id);
		return -EINVAL;
	}

	dev_info(stmpe->dev, "%s detected, chip id: %#x\n", variant->name, id);

	/* Disable all modules -- subdrivers should enable what they need. */
	ret = stmpe_disable(stmpe, ~0);
	if (ret)
		return ret;

<<<<<<< HEAD
<<<<<<< HEAD
	if (irq_trigger == IRQF_TRIGGER_FALLING ||
	    irq_trigger == IRQF_TRIGGER_RISING)
		icr |= STMPE_ICR_LSB_EDGE;

	if (irq_trigger == IRQF_TRIGGER_RISING ||
	    irq_trigger == IRQF_TRIGGER_HIGH)
		icr |= STMPE_ICR_LSB_HIGH;

	if (stmpe->pdata->irq_invert_polarity)
		icr ^= STMPE_ICR_LSB_HIGH;
=======
=======
	if (id == STMPE1801_ID)	{
		ret =  stmpe1801_reset(stmpe);
		if (ret < 0)
			return ret;
	}

>>>>>>> refs/remotes/origin/master
	if (stmpe->irq >= 0) {
		if (id == STMPE801_ID)
			icr = STMPE801_REG_SYS_CTRL_INT_EN;
		else
			icr = STMPE_ICR_LSB_GIM;

		/* STMPE801 doesn't support Edge interrupts */
		if (id != STMPE801_ID) {
			if (irq_trigger == IRQF_TRIGGER_FALLING ||
					irq_trigger == IRQF_TRIGGER_RISING)
				icr |= STMPE_ICR_LSB_EDGE;
		}

		if (irq_trigger == IRQF_TRIGGER_RISING ||
				irq_trigger == IRQF_TRIGGER_HIGH) {
			if (id == STMPE801_ID)
				icr |= STMPE801_REG_SYS_CTRL_INT_HI;
			else
				icr |= STMPE_ICR_LSB_HIGH;
		}
<<<<<<< HEAD

		if (stmpe->pdata->irq_invert_polarity) {
			if (id == STMPE801_ID)
				icr ^= STMPE801_REG_SYS_CTRL_INT_HI;
			else
				icr ^= STMPE_ICR_LSB_HIGH;
		}
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	}
>>>>>>> refs/remotes/origin/master

	if (stmpe->pdata->autosleep) {
		ret = stmpe_autosleep(stmpe, autosleep_timeout);
		if (ret)
			return ret;
	}

	return stmpe_reg_write(stmpe, stmpe->regs[STMPE_IDX_ICR_LSB], icr);
}

<<<<<<< HEAD
static int __devinit stmpe_add_device(struct stmpe *stmpe,
				      struct mfd_cell *cell, int irq)
{
	return mfd_add_devices(stmpe->dev, stmpe->pdata->id, cell, 1,
			       NULL, stmpe->irq_base + irq);
}

static int __devinit stmpe_devices_init(struct stmpe *stmpe)
=======
static int stmpe_add_device(struct stmpe *stmpe, const struct mfd_cell *cell)
{
	return mfd_add_devices(stmpe->dev, stmpe->pdata->id, cell, 1,
			       NULL, stmpe->irq_base, stmpe->domain);
}

static int stmpe_devices_init(struct stmpe *stmpe)
>>>>>>> refs/remotes/origin/master
{
	struct stmpe_variant_info *variant = stmpe->variant;
	unsigned int platform_blocks = stmpe->pdata->blocks;
	int ret = -EINVAL;
<<<<<<< HEAD
	int i;
=======
	int i, j;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < variant->num_blocks; i++) {
		struct stmpe_variant_block *block = &variant->blocks[i];

		if (!(platform_blocks & block->block))
			continue;

<<<<<<< HEAD
		platform_blocks &= ~block->block;
		ret = stmpe_add_device(stmpe, block->cell, block->irq);
=======
		for (j = 0; j < block->cell->num_resources; j++) {
			struct resource *res =
				(struct resource *) &block->cell->resources[j];

			/* Dynamically fill in a variant's IRQ. */
			if (res->flags & IORESOURCE_IRQ)
				res->start = res->end = block->irq + j;
		}

		platform_blocks &= ~block->block;
		ret = stmpe_add_device(stmpe, block->cell);
>>>>>>> refs/remotes/origin/master
		if (ret)
			return ret;
	}

	if (platform_blocks)
		dev_warn(stmpe->dev,
			 "platform wants blocks (%#x) not present on variant",
			 platform_blocks);

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_PM
static int stmpe_suspend(struct device *dev)
{
	struct i2c_client *i2c = to_i2c_client(dev);

	if (device_may_wakeup(&i2c->dev))
		enable_irq_wake(i2c->irq);

	return 0;
}

static int stmpe_resume(struct device *dev)
{
	struct i2c_client *i2c = to_i2c_client(dev);

	if (device_may_wakeup(&i2c->dev))
		disable_irq_wake(i2c->irq);

	return 0;
}
#endif

static int __devinit stmpe_probe(struct i2c_client *i2c,
				 const struct i2c_device_id *id)
{
	struct stmpe_platform_data *pdata = i2c->dev.platform_data;
=======
/* Called from client specific probe routines */
int __devinit stmpe_probe(struct stmpe_client_info *ci, int partnum)
{
	struct stmpe_platform_data *pdata = dev_get_platdata(ci->dev);
>>>>>>> refs/remotes/origin/cm-10.0
	struct stmpe *stmpe;
	int ret;

	if (!pdata)
		return -EINVAL;

	stmpe = kzalloc(sizeof(struct stmpe), GFP_KERNEL);
=======
static void stmpe_of_probe(struct stmpe_platform_data *pdata,
			   struct device_node *np)
{
	struct device_node *child;

	pdata->id = of_alias_get_id(np, "stmpe-i2c");
	if (pdata->id < 0)
		pdata->id = -1;

	pdata->irq_trigger = IRQF_TRIGGER_NONE;

	of_property_read_u32(np, "st,autosleep-timeout",
			&pdata->autosleep_timeout);

	pdata->autosleep = (pdata->autosleep_timeout) ? true : false;

	for_each_child_of_node(np, child) {
		if (!strcmp(child->name, "stmpe_gpio")) {
			pdata->blocks |= STMPE_BLOCK_GPIO;
		} else if (!strcmp(child->name, "stmpe_keypad")) {
			pdata->blocks |= STMPE_BLOCK_KEYPAD;
		} else if (!strcmp(child->name, "stmpe_touchscreen")) {
			pdata->blocks |= STMPE_BLOCK_TOUCHSCREEN;
		} else if (!strcmp(child->name, "stmpe_adc")) {
			pdata->blocks |= STMPE_BLOCK_ADC;
		} else if (!strcmp(child->name, "stmpe_pwm")) {
			pdata->blocks |= STMPE_BLOCK_PWM;
		} else if (!strcmp(child->name, "stmpe_rotator")) {
			pdata->blocks |= STMPE_BLOCK_ROTATOR;
		}
	}
}

/* Called from client specific probe routines */
int stmpe_probe(struct stmpe_client_info *ci, int partnum)
{
	struct stmpe_platform_data *pdata = dev_get_platdata(ci->dev);
	struct device_node *np = ci->dev->of_node;
	struct stmpe *stmpe;
	int ret;

	if (!pdata) {
		if (!np)
			return -EINVAL;

		pdata = devm_kzalloc(ci->dev, sizeof(*pdata), GFP_KERNEL);
		if (!pdata)
			return -ENOMEM;

		stmpe_of_probe(pdata, np);

		if (of_find_property(np, "interrupts", NULL) == NULL)
			ci->irq = -1;
	}

	stmpe = devm_kzalloc(ci->dev, sizeof(struct stmpe), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!stmpe)
		return -ENOMEM;

	mutex_init(&stmpe->irq_lock);
	mutex_init(&stmpe->lock);

<<<<<<< HEAD
<<<<<<< HEAD
	stmpe->dev = &i2c->dev;
	stmpe->i2c = i2c;

	stmpe->pdata = pdata;
	stmpe->irq_base = pdata->irq_base;

	stmpe->partnum = id->driver_data;
	stmpe->variant = stmpe_variant_info[stmpe->partnum];
	stmpe->regs = stmpe->variant->regs;
	stmpe->num_gpios = stmpe->variant->num_gpios;

	i2c_set_clientdata(i2c, stmpe);

	ret = stmpe_chip_init(stmpe);
	if (ret)
		goto out_free;

	ret = stmpe_irq_init(stmpe);
	if (ret)
		goto out_free;

	ret = request_threaded_irq(stmpe->i2c->irq, NULL, stmpe_irq,
				   pdata->irq_trigger | IRQF_ONESHOT,
				   "stmpe", stmpe);
	if (ret) {
		dev_err(stmpe->dev, "failed to request IRQ: %d\n", ret);
		goto out_removeirq;
=======
=======
>>>>>>> refs/remotes/origin/master
	stmpe->dev = ci->dev;
	stmpe->client = ci->client;
	stmpe->pdata = pdata;
	stmpe->irq_base = pdata->irq_base;
	stmpe->ci = ci;
	stmpe->partnum = partnum;
	stmpe->variant = stmpe_variant_info[partnum];
	stmpe->regs = stmpe->variant->regs;
	stmpe->num_gpios = stmpe->variant->num_gpios;
	dev_set_drvdata(stmpe->dev, stmpe);

	if (ci->init)
		ci->init(stmpe);

	if (pdata->irq_over_gpio) {
<<<<<<< HEAD
		ret = gpio_request_one(pdata->irq_gpio, GPIOF_DIR_IN, "stmpe");
		if (ret) {
			dev_err(stmpe->dev, "failed to request IRQ GPIO: %d\n",
					ret);
			goto out_free;
=======
		ret = devm_gpio_request_one(ci->dev, pdata->irq_gpio,
				GPIOF_DIR_IN, "stmpe");
		if (ret) {
			dev_err(stmpe->dev, "failed to request IRQ GPIO: %d\n",
					ret);
			return ret;
>>>>>>> refs/remotes/origin/master
		}

		stmpe->irq = gpio_to_irq(pdata->irq_gpio);
	} else {
		stmpe->irq = ci->irq;
	}

	if (stmpe->irq < 0) {
		/* use alternate variant info for no-irq mode, if supported */
		dev_info(stmpe->dev,
			"%s configured in no-irq mode by platform data\n",
			stmpe->variant->name);
		if (!stmpe_noirq_variant_info[stmpe->partnum]) {
			dev_err(stmpe->dev,
				"%s does not support no-irq mode!\n",
				stmpe->variant->name);
<<<<<<< HEAD
			ret = -ENODEV;
			goto free_gpio;
		}
		stmpe->variant = stmpe_noirq_variant_info[stmpe->partnum];
=======
			return -ENODEV;
		}
		stmpe->variant = stmpe_noirq_variant_info[stmpe->partnum];
	} else if (pdata->irq_trigger == IRQF_TRIGGER_NONE) {
		pdata->irq_trigger = irq_get_trigger_type(stmpe->irq);
>>>>>>> refs/remotes/origin/master
	}

	ret = stmpe_chip_init(stmpe);
	if (ret)
<<<<<<< HEAD
		goto free_gpio;

	if (stmpe->irq >= 0) {
		ret = stmpe_irq_init(stmpe);
		if (ret)
			goto free_gpio;

		ret = request_threaded_irq(stmpe->irq, NULL, stmpe_irq,
				pdata->irq_trigger | IRQF_ONESHOT,
=======
		return ret;

	if (stmpe->irq >= 0) {
		ret = stmpe_irq_init(stmpe, np);
		if (ret)
			return ret;

		ret = devm_request_threaded_irq(ci->dev, stmpe->irq, NULL,
				stmpe_irq, pdata->irq_trigger | IRQF_ONESHOT,
>>>>>>> refs/remotes/origin/master
				"stmpe", stmpe);
		if (ret) {
			dev_err(stmpe->dev, "failed to request IRQ: %d\n",
					ret);
<<<<<<< HEAD
			goto out_removeirq;
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	ret = stmpe_devices_init(stmpe);
	if (ret) {
		dev_err(stmpe->dev, "failed to add children\n");
		goto out_removedevs;
	}

	return 0;

out_removedevs:
	mfd_remove_devices(stmpe->dev);
<<<<<<< HEAD
	free_irq(stmpe->i2c->irq, stmpe);
out_removeirq:
	stmpe_irq_remove(stmpe);
=======
	if (stmpe->irq >= 0)
		free_irq(stmpe->irq, stmpe);
out_removeirq:
	if (stmpe->irq >= 0)
		stmpe_irq_remove(stmpe);
free_gpio:
	if (pdata->irq_over_gpio)
		gpio_free(pdata->irq_gpio);
>>>>>>> refs/remotes/origin/cm-10.0
out_free:
	kfree(stmpe);
	return ret;
}

<<<<<<< HEAD
static int __devexit stmpe_remove(struct i2c_client *client)
{
	struct stmpe *stmpe = i2c_get_clientdata(client);

	mfd_remove_devices(stmpe->dev);

	free_irq(stmpe->i2c->irq, stmpe);
	stmpe_irq_remove(stmpe);
=======
=======
			return ret;
		}
	}

	ret = stmpe_devices_init(stmpe);
	if (!ret)
		return 0;

	dev_err(stmpe->dev, "failed to add children\n");
	mfd_remove_devices(stmpe->dev);

	return ret;
}

>>>>>>> refs/remotes/origin/master
int stmpe_remove(struct stmpe *stmpe)
{
	mfd_remove_devices(stmpe->dev);

<<<<<<< HEAD
	if (stmpe->irq >= 0) {
		free_irq(stmpe->irq, stmpe);
		stmpe_irq_remove(stmpe);
	}

	if (stmpe->pdata->irq_over_gpio)
		gpio_free(stmpe->pdata->irq_gpio);
>>>>>>> refs/remotes/origin/cm-10.0

	kfree(stmpe);

	return 0;
}

<<<<<<< HEAD
static const struct i2c_device_id stmpe_id[] = {
	{ "stmpe811", STMPE811 },
	{ "stmpe1601", STMPE1601 },
	{ "stmpe2401", STMPE2401 },
	{ "stmpe2403", STMPE2403 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, stmpe_id);

#ifdef CONFIG_PM
static const struct dev_pm_ops stmpe_dev_pm_ops = {
	.suspend	= stmpe_suspend,
	.resume		= stmpe_resume,
};
#endif

static struct i2c_driver stmpe_driver = {
	.driver.name	= "stmpe",
	.driver.owner	= THIS_MODULE,
#ifdef CONFIG_PM
	.driver.pm	= &stmpe_dev_pm_ops,
#endif
	.probe		= stmpe_probe,
	.remove		= __devexit_p(stmpe_remove),
	.id_table	= stmpe_id,
};

static int __init stmpe_init(void)
{
	return i2c_add_driver(&stmpe_driver);
}
subsys_initcall(stmpe_init);

static void __exit stmpe_exit(void)
{
	i2c_del_driver(&stmpe_driver);
}
module_exit(stmpe_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("STMPE MFD core driver");
MODULE_AUTHOR("Rabin Vincent <rabin.vincent@stericsson.com>");
=======
=======
	return 0;
}

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PM
static int stmpe_suspend(struct device *dev)
{
	struct stmpe *stmpe = dev_get_drvdata(dev);

	if (stmpe->irq >= 0 && device_may_wakeup(dev))
		enable_irq_wake(stmpe->irq);

	return 0;
}

static int stmpe_resume(struct device *dev)
{
	struct stmpe *stmpe = dev_get_drvdata(dev);

	if (stmpe->irq >= 0 && device_may_wakeup(dev))
		disable_irq_wake(stmpe->irq);

	return 0;
}

const struct dev_pm_ops stmpe_dev_pm_ops = {
	.suspend	= stmpe_suspend,
	.resume		= stmpe_resume,
};
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
