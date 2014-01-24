/*
 *  PCA953x 4/8/16 bit I/O ports
 *
 *  Copyright (C) 2005 Ben Gardner <bgardner@wabtec.com>
 *  Copyright (C) 2007 Marvell International Ltd.
 *
 *  Derived from drivers/i2c/chips/pca9539.c
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
<<<<<<< HEAD
#include <linux/i2c.h>
#include <linux/i2c/pca953x.h>
=======
#include <linux/irqdomain.h>
#include <linux/i2c.h>
#include <linux/platform_data/pca953x.h>
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>
#ifdef CONFIG_OF_GPIO
#include <linux/of_platform.h>
#endif

#define PCA953X_INPUT		0
#define PCA953X_OUTPUT		1
#define PCA953X_INVERT		2
#define PCA953X_DIRECTION	3

<<<<<<< HEAD
=======
#define REG_ADDR_AI		0x80

>>>>>>> refs/remotes/origin/master
#define PCA957X_IN		0
#define PCA957X_INVRT		1
#define PCA957X_BKEN		2
#define PCA957X_PUPD		3
#define PCA957X_CFG		4
#define PCA957X_OUT		5
#define PCA957X_MSK		6
#define PCA957X_INTS		7

#define PCA_GPIO_MASK		0x00FF
#define PCA_INT			0x0100
#define PCA953X_TYPE		0x1000
#define PCA957X_TYPE		0x2000

static const struct i2c_device_id pca953x_id[] = {
<<<<<<< HEAD
=======
	{ "pca9505", 40 | PCA953X_TYPE | PCA_INT, },
>>>>>>> refs/remotes/origin/master
	{ "pca9534", 8  | PCA953X_TYPE | PCA_INT, },
	{ "pca9535", 16 | PCA953X_TYPE | PCA_INT, },
	{ "pca9536", 4  | PCA953X_TYPE, },
	{ "pca9537", 4  | PCA953X_TYPE | PCA_INT, },
	{ "pca9538", 8  | PCA953X_TYPE | PCA_INT, },
	{ "pca9539", 16 | PCA953X_TYPE | PCA_INT, },
	{ "pca9554", 8  | PCA953X_TYPE | PCA_INT, },
	{ "pca9555", 16 | PCA953X_TYPE | PCA_INT, },
	{ "pca9556", 8  | PCA953X_TYPE, },
	{ "pca9557", 8  | PCA953X_TYPE, },
	{ "pca9574", 8  | PCA957X_TYPE | PCA_INT, },
	{ "pca9575", 16 | PCA957X_TYPE | PCA_INT, },

	{ "max7310", 8  | PCA953X_TYPE, },
	{ "max7312", 16 | PCA953X_TYPE | PCA_INT, },
	{ "max7313", 16 | PCA953X_TYPE | PCA_INT, },
	{ "max7315", 8  | PCA953X_TYPE | PCA_INT, },
	{ "pca6107", 8  | PCA953X_TYPE | PCA_INT, },
	{ "tca6408", 8  | PCA953X_TYPE | PCA_INT, },
	{ "tca6416", 16 | PCA953X_TYPE | PCA_INT, },
<<<<<<< HEAD
	/* NYET:  { "tca6424", 24, }, */
=======
	{ "tca6424", 24 | PCA953X_TYPE | PCA_INT, },
>>>>>>> refs/remotes/origin/master
	{ }
};
MODULE_DEVICE_TABLE(i2c, pca953x_id);

<<<<<<< HEAD
struct pca953x_chip {
	unsigned gpio_start;
	uint16_t reg_output;
	uint16_t reg_direction;
=======
#define MAX_BANK 5
#define BANK_SZ 8

#define NBANK(chip) (chip->gpio_chip.ngpio / BANK_SZ)

struct pca953x_chip {
	unsigned gpio_start;
	u8 reg_output[MAX_BANK];
	u8 reg_direction[MAX_BANK];
>>>>>>> refs/remotes/origin/master
	struct mutex i2c_lock;

#ifdef CONFIG_GPIO_PCA953X_IRQ
	struct mutex irq_lock;
<<<<<<< HEAD
	uint16_t irq_mask;
	uint16_t irq_stat;
	uint16_t irq_trig_raise;
	uint16_t irq_trig_fall;
	int	 irq_base;
=======
	u8 irq_mask[MAX_BANK];
	u8 irq_stat[MAX_BANK];
	u8 irq_trig_raise[MAX_BANK];
	u8 irq_trig_fall[MAX_BANK];
	struct irq_domain *domain;
>>>>>>> refs/remotes/origin/master
#endif

	struct i2c_client *client;
	struct gpio_chip gpio_chip;
	const char *const *names;
	int	chip_type;
};

<<<<<<< HEAD
static int pca953x_write_reg(struct pca953x_chip *chip, int reg, uint16_t val)
=======
static int pca953x_read_single(struct pca953x_chip *chip, int reg, u32 *val,
				int off)
{
	int ret;
	int bank_shift = fls((chip->gpio_chip.ngpio - 1) / BANK_SZ);
	int offset = off / BANK_SZ;

	ret = i2c_smbus_read_byte_data(chip->client,
				(reg << bank_shift) + offset);
	*val = ret;

	if (ret < 0) {
		dev_err(&chip->client->dev, "failed reading register\n");
		return ret;
	}

	return 0;
}

static int pca953x_write_single(struct pca953x_chip *chip, int reg, u32 val,
				int off)
{
	int ret = 0;
	int bank_shift = fls((chip->gpio_chip.ngpio - 1) / BANK_SZ);
	int offset = off / BANK_SZ;

	ret = i2c_smbus_write_byte_data(chip->client,
					(reg << bank_shift) + offset, val);

	if (ret < 0) {
		dev_err(&chip->client->dev, "failed writing register\n");
		return ret;
	}

	return 0;
}

static int pca953x_write_regs(struct pca953x_chip *chip, int reg, u8 *val)
>>>>>>> refs/remotes/origin/master
{
	int ret = 0;

	if (chip->gpio_chip.ngpio <= 8)
<<<<<<< HEAD
		ret = i2c_smbus_write_byte_data(chip->client, reg, val);
	else {
		switch (chip->chip_type) {
		case PCA953X_TYPE:
			ret = i2c_smbus_write_word_data(chip->client,
							reg << 1, val);
			break;
		case PCA957X_TYPE:
			ret = i2c_smbus_write_byte_data(chip->client, reg << 1,
							val & 0xff);
=======
		ret = i2c_smbus_write_byte_data(chip->client, reg, *val);
	else if (chip->gpio_chip.ngpio >= 24) {
		int bank_shift = fls((chip->gpio_chip.ngpio - 1) / BANK_SZ);
		ret = i2c_smbus_write_i2c_block_data(chip->client,
					(reg << bank_shift) | REG_ADDR_AI,
					NBANK(chip), val);
	} else {
		switch (chip->chip_type) {
		case PCA953X_TYPE:
			ret = i2c_smbus_write_word_data(chip->client,
							reg << 1, (u16) *val);
			break;
		case PCA957X_TYPE:
			ret = i2c_smbus_write_byte_data(chip->client, reg << 1,
							val[0]);
>>>>>>> refs/remotes/origin/master
			if (ret < 0)
				break;
			ret = i2c_smbus_write_byte_data(chip->client,
							(reg << 1) + 1,
<<<<<<< HEAD
							(val & 0xff00) >> 8);
=======
							val[1]);
>>>>>>> refs/remotes/origin/master
			break;
		}
	}

	if (ret < 0) {
		dev_err(&chip->client->dev, "failed writing register\n");
		return ret;
	}

	return 0;
}

<<<<<<< HEAD
static int pca953x_read_reg(struct pca953x_chip *chip, int reg, uint16_t *val)
{
	int ret;

	if (chip->gpio_chip.ngpio <= 8)
		ret = i2c_smbus_read_byte_data(chip->client, reg);
	else
		ret = i2c_smbus_read_word_data(chip->client, reg << 1);

=======
static int pca953x_read_regs(struct pca953x_chip *chip, int reg, u8 *val)
{
	int ret;

	if (chip->gpio_chip.ngpio <= 8) {
		ret = i2c_smbus_read_byte_data(chip->client, reg);
		*val = ret;
	} else if (chip->gpio_chip.ngpio >= 24) {
		int bank_shift = fls((chip->gpio_chip.ngpio - 1) / BANK_SZ);

		ret = i2c_smbus_read_i2c_block_data(chip->client,
					(reg << bank_shift) | REG_ADDR_AI,
					NBANK(chip), val);
	} else {
		ret = i2c_smbus_read_word_data(chip->client, reg << 1);
		val[0] = (u16)ret & 0xFF;
		val[1] = (u16)ret >> 8;
	}
>>>>>>> refs/remotes/origin/master
	if (ret < 0) {
		dev_err(&chip->client->dev, "failed reading register\n");
		return ret;
	}

<<<<<<< HEAD
	*val = (uint16_t)ret;
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int pca953x_gpio_direction_input(struct gpio_chip *gc, unsigned off)
{
	struct pca953x_chip *chip;
<<<<<<< HEAD
	uint16_t reg_val;
=======
	u8 reg_val;
>>>>>>> refs/remotes/origin/master
	int ret, offset = 0;

	chip = container_of(gc, struct pca953x_chip, gpio_chip);

	mutex_lock(&chip->i2c_lock);
<<<<<<< HEAD
	reg_val = chip->reg_direction | (1u << off);
=======
	reg_val = chip->reg_direction[off / BANK_SZ] | (1u << (off % BANK_SZ));
>>>>>>> refs/remotes/origin/master

	switch (chip->chip_type) {
	case PCA953X_TYPE:
		offset = PCA953X_DIRECTION;
		break;
	case PCA957X_TYPE:
		offset = PCA957X_CFG;
		break;
	}
<<<<<<< HEAD
	ret = pca953x_write_reg(chip, offset, reg_val);
	if (ret)
		goto exit;

	chip->reg_direction = reg_val;
=======
	ret = pca953x_write_single(chip, offset, reg_val, off);
	if (ret)
		goto exit;

	chip->reg_direction[off / BANK_SZ] = reg_val;
>>>>>>> refs/remotes/origin/master
	ret = 0;
exit:
	mutex_unlock(&chip->i2c_lock);
	return ret;
}

static int pca953x_gpio_direction_output(struct gpio_chip *gc,
		unsigned off, int val)
{
	struct pca953x_chip *chip;
<<<<<<< HEAD
	uint16_t reg_val;
=======
	u8 reg_val;
>>>>>>> refs/remotes/origin/master
	int ret, offset = 0;

	chip = container_of(gc, struct pca953x_chip, gpio_chip);

	mutex_lock(&chip->i2c_lock);
	/* set output level */
	if (val)
<<<<<<< HEAD
		reg_val = chip->reg_output | (1u << off);
	else
		reg_val = chip->reg_output & ~(1u << off);
=======
		reg_val = chip->reg_output[off / BANK_SZ]
			| (1u << (off % BANK_SZ));
	else
		reg_val = chip->reg_output[off / BANK_SZ]
			& ~(1u << (off % BANK_SZ));
>>>>>>> refs/remotes/origin/master

	switch (chip->chip_type) {
	case PCA953X_TYPE:
		offset = PCA953X_OUTPUT;
		break;
	case PCA957X_TYPE:
		offset = PCA957X_OUT;
		break;
	}
<<<<<<< HEAD
	ret = pca953x_write_reg(chip, offset, reg_val);
	if (ret)
		goto exit;

	chip->reg_output = reg_val;

	/* then direction */
	reg_val = chip->reg_direction & ~(1u << off);
=======
	ret = pca953x_write_single(chip, offset, reg_val, off);
	if (ret)
		goto exit;

	chip->reg_output[off / BANK_SZ] = reg_val;

	/* then direction */
	reg_val = chip->reg_direction[off / BANK_SZ] & ~(1u << (off % BANK_SZ));
>>>>>>> refs/remotes/origin/master
	switch (chip->chip_type) {
	case PCA953X_TYPE:
		offset = PCA953X_DIRECTION;
		break;
	case PCA957X_TYPE:
		offset = PCA957X_CFG;
		break;
	}
<<<<<<< HEAD
	ret = pca953x_write_reg(chip, offset, reg_val);
	if (ret)
		goto exit;

	chip->reg_direction = reg_val;
=======
	ret = pca953x_write_single(chip, offset, reg_val, off);
	if (ret)
		goto exit;

	chip->reg_direction[off / BANK_SZ] = reg_val;
>>>>>>> refs/remotes/origin/master
	ret = 0;
exit:
	mutex_unlock(&chip->i2c_lock);
	return ret;
}

static int pca953x_gpio_get_value(struct gpio_chip *gc, unsigned off)
{
	struct pca953x_chip *chip;
<<<<<<< HEAD
	uint16_t reg_val;
=======
	u32 reg_val;
>>>>>>> refs/remotes/origin/master
	int ret, offset = 0;

	chip = container_of(gc, struct pca953x_chip, gpio_chip);

	mutex_lock(&chip->i2c_lock);
	switch (chip->chip_type) {
	case PCA953X_TYPE:
		offset = PCA953X_INPUT;
		break;
	case PCA957X_TYPE:
		offset = PCA957X_IN;
		break;
	}
<<<<<<< HEAD
	ret = pca953x_read_reg(chip, offset, &reg_val);
=======
	ret = pca953x_read_single(chip, offset, &reg_val, off);
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&chip->i2c_lock);
	if (ret < 0) {
		/* NOTE:  diagnostic already emitted; that's all we should
		 * do unless gpio_*_value_cansleep() calls become different
		 * from their nonsleeping siblings (and report faults).
		 */
		return 0;
	}

<<<<<<< HEAD
	return (reg_val & (1u << off)) ? 1 : 0;
=======
	return (reg_val & (1u << (off % BANK_SZ))) ? 1 : 0;
>>>>>>> refs/remotes/origin/master
}

static void pca953x_gpio_set_value(struct gpio_chip *gc, unsigned off, int val)
{
	struct pca953x_chip *chip;
<<<<<<< HEAD
	uint16_t reg_val;
=======
	u8 reg_val;
>>>>>>> refs/remotes/origin/master
	int ret, offset = 0;

	chip = container_of(gc, struct pca953x_chip, gpio_chip);

	mutex_lock(&chip->i2c_lock);
	if (val)
<<<<<<< HEAD
		reg_val = chip->reg_output | (1u << off);
	else
		reg_val = chip->reg_output & ~(1u << off);
=======
		reg_val = chip->reg_output[off / BANK_SZ]
			| (1u << (off % BANK_SZ));
	else
		reg_val = chip->reg_output[off / BANK_SZ]
			& ~(1u << (off % BANK_SZ));
>>>>>>> refs/remotes/origin/master

	switch (chip->chip_type) {
	case PCA953X_TYPE:
		offset = PCA953X_OUTPUT;
		break;
	case PCA957X_TYPE:
		offset = PCA957X_OUT;
		break;
	}
<<<<<<< HEAD
	ret = pca953x_write_reg(chip, offset, reg_val);
	if (ret)
		goto exit;

	chip->reg_output = reg_val;
=======
	ret = pca953x_write_single(chip, offset, reg_val, off);
	if (ret)
		goto exit;

	chip->reg_output[off / BANK_SZ] = reg_val;
>>>>>>> refs/remotes/origin/master
exit:
	mutex_unlock(&chip->i2c_lock);
}

static void pca953x_setup_gpio(struct pca953x_chip *chip, int gpios)
{
	struct gpio_chip *gc;

	gc = &chip->gpio_chip;

	gc->direction_input  = pca953x_gpio_direction_input;
	gc->direction_output = pca953x_gpio_direction_output;
	gc->get = pca953x_gpio_get_value;
	gc->set = pca953x_gpio_set_value;
<<<<<<< HEAD
	gc->can_sleep = 1;
=======
	gc->can_sleep = true;
>>>>>>> refs/remotes/origin/master

	gc->base = chip->gpio_start;
	gc->ngpio = gpios;
	gc->label = chip->client->name;
	gc->dev = &chip->client->dev;
	gc->owner = THIS_MODULE;
	gc->names = chip->names;
}

#ifdef CONFIG_GPIO_PCA953X_IRQ
static int pca953x_gpio_to_irq(struct gpio_chip *gc, unsigned off)
{
	struct pca953x_chip *chip;

	chip = container_of(gc, struct pca953x_chip, gpio_chip);
<<<<<<< HEAD
	return chip->irq_base + off;
=======
	return irq_create_mapping(chip->domain, off);
>>>>>>> refs/remotes/origin/master
}

static void pca953x_irq_mask(struct irq_data *d)
{
	struct pca953x_chip *chip = irq_data_get_irq_chip_data(d);

<<<<<<< HEAD
	chip->irq_mask &= ~(1 << (d->irq - chip->irq_base));
=======
	chip->irq_mask[d->hwirq / BANK_SZ] &= ~(1 << (d->hwirq % BANK_SZ));
>>>>>>> refs/remotes/origin/master
}

static void pca953x_irq_unmask(struct irq_data *d)
{
	struct pca953x_chip *chip = irq_data_get_irq_chip_data(d);

<<<<<<< HEAD
	chip->irq_mask |= 1 << (d->irq - chip->irq_base);
=======
	chip->irq_mask[d->hwirq / BANK_SZ] |= 1 << (d->hwirq % BANK_SZ);
>>>>>>> refs/remotes/origin/master
}

static void pca953x_irq_bus_lock(struct irq_data *d)
{
	struct pca953x_chip *chip = irq_data_get_irq_chip_data(d);

	mutex_lock(&chip->irq_lock);
}

static void pca953x_irq_bus_sync_unlock(struct irq_data *d)
{
	struct pca953x_chip *chip = irq_data_get_irq_chip_data(d);
<<<<<<< HEAD
	uint16_t new_irqs;
	uint16_t level;

	/* Look for any newly setup interrupt */
	new_irqs = chip->irq_trig_fall | chip->irq_trig_raise;
	new_irqs &= ~chip->reg_direction;

	while (new_irqs) {
		level = __ffs(new_irqs);
		pca953x_gpio_direction_input(&chip->gpio_chip, level);
		new_irqs &= ~(1 << level);
=======
	u8 new_irqs;
	int level, i;

	/* Look for any newly setup interrupt */
	for (i = 0; i < NBANK(chip); i++) {
		new_irqs = chip->irq_trig_fall[i] | chip->irq_trig_raise[i];
		new_irqs &= ~chip->reg_direction[i];

		while (new_irqs) {
			level = __ffs(new_irqs);
			pca953x_gpio_direction_input(&chip->gpio_chip,
							level + (BANK_SZ * i));
			new_irqs &= ~(1 << level);
		}
>>>>>>> refs/remotes/origin/master
	}

	mutex_unlock(&chip->irq_lock);
}

static int pca953x_irq_set_type(struct irq_data *d, unsigned int type)
{
	struct pca953x_chip *chip = irq_data_get_irq_chip_data(d);
<<<<<<< HEAD
	uint16_t level = d->irq - chip->irq_base;
	uint16_t mask = 1 << level;
=======
	int bank_nb = d->hwirq / BANK_SZ;
	u8 mask = 1 << (d->hwirq % BANK_SZ);
>>>>>>> refs/remotes/origin/master

	if (!(type & IRQ_TYPE_EDGE_BOTH)) {
		dev_err(&chip->client->dev, "irq %d: unsupported type %d\n",
			d->irq, type);
		return -EINVAL;
	}

	if (type & IRQ_TYPE_EDGE_FALLING)
<<<<<<< HEAD
		chip->irq_trig_fall |= mask;
	else
		chip->irq_trig_fall &= ~mask;

	if (type & IRQ_TYPE_EDGE_RISING)
		chip->irq_trig_raise |= mask;
	else
		chip->irq_trig_raise &= ~mask;
=======
		chip->irq_trig_fall[bank_nb] |= mask;
	else
		chip->irq_trig_fall[bank_nb] &= ~mask;

	if (type & IRQ_TYPE_EDGE_RISING)
		chip->irq_trig_raise[bank_nb] |= mask;
	else
		chip->irq_trig_raise[bank_nb] &= ~mask;
>>>>>>> refs/remotes/origin/master

	return 0;
}

static struct irq_chip pca953x_irq_chip = {
	.name			= "pca953x",
	.irq_mask		= pca953x_irq_mask,
	.irq_unmask		= pca953x_irq_unmask,
	.irq_bus_lock		= pca953x_irq_bus_lock,
	.irq_bus_sync_unlock	= pca953x_irq_bus_sync_unlock,
	.irq_set_type		= pca953x_irq_set_type,
};

<<<<<<< HEAD
static uint16_t pca953x_irq_pending(struct pca953x_chip *chip)
{
	uint16_t cur_stat;
	uint16_t old_stat;
	uint16_t pending;
	uint16_t trigger;
	int ret, offset = 0;
=======
static u8 pca953x_irq_pending(struct pca953x_chip *chip, u8 *pending)
{
	u8 cur_stat[MAX_BANK];
	u8 old_stat[MAX_BANK];
	u8 pendings = 0;
	u8 trigger[MAX_BANK], triggers = 0;
	int ret, i, offset = 0;
>>>>>>> refs/remotes/origin/master

	switch (chip->chip_type) {
	case PCA953X_TYPE:
		offset = PCA953X_INPUT;
		break;
	case PCA957X_TYPE:
		offset = PCA957X_IN;
		break;
	}
<<<<<<< HEAD
	ret = pca953x_read_reg(chip, offset, &cur_stat);
=======
	ret = pca953x_read_regs(chip, offset, cur_stat);
>>>>>>> refs/remotes/origin/master
	if (ret)
		return 0;

	/* Remove output pins from the equation */
<<<<<<< HEAD
	cur_stat &= chip->reg_direction;

	old_stat = chip->irq_stat;
	trigger = (cur_stat ^ old_stat) & chip->irq_mask;

	if (!trigger)
		return 0;

	chip->irq_stat = cur_stat;

	pending = (old_stat & chip->irq_trig_fall) |
		  (cur_stat & chip->irq_trig_raise);
	pending &= trigger;

	return pending;
=======
	for (i = 0; i < NBANK(chip); i++)
		cur_stat[i] &= chip->reg_direction[i];

	memcpy(old_stat, chip->irq_stat, NBANK(chip));

	for (i = 0; i < NBANK(chip); i++) {
		trigger[i] = (cur_stat[i] ^ old_stat[i]) & chip->irq_mask[i];
		triggers += trigger[i];
	}

	if (!triggers)
		return 0;

	memcpy(chip->irq_stat, cur_stat, NBANK(chip));

	for (i = 0; i < NBANK(chip); i++) {
		pending[i] = (old_stat[i] & chip->irq_trig_fall[i]) |
			(cur_stat[i] & chip->irq_trig_raise[i]);
		pending[i] &= trigger[i];
		pendings += pending[i];
	}

	return pendings;
>>>>>>> refs/remotes/origin/master
}

static irqreturn_t pca953x_irq_handler(int irq, void *devid)
{
	struct pca953x_chip *chip = devid;
<<<<<<< HEAD
	uint16_t pending;
	uint16_t level;

	pending = pca953x_irq_pending(chip);

	if (!pending)
		return IRQ_HANDLED;

	do {
		level = __ffs(pending);
		handle_nested_irq(level + chip->irq_base);

		pending &= ~(1 << level);
	} while (pending);
=======
	u8 pending[MAX_BANK];
	u8 level;
	int i;

	if (!pca953x_irq_pending(chip, pending))
		return IRQ_HANDLED;

	for (i = 0; i < NBANK(chip); i++) {
		while (pending[i]) {
			level = __ffs(pending[i]);
			handle_nested_irq(irq_find_mapping(chip->domain,
							level + (BANK_SZ * i)));
			pending[i] &= ~(1 << level);
		}
	}
>>>>>>> refs/remotes/origin/master

	return IRQ_HANDLED;
}

<<<<<<< HEAD
=======
static int pca953x_gpio_irq_map(struct irq_domain *d, unsigned int irq,
		       irq_hw_number_t hwirq)
{
	irq_clear_status_flags(irq, IRQ_NOREQUEST);
	irq_set_chip_data(irq, d->host_data);
	irq_set_chip(irq, &pca953x_irq_chip);
	irq_set_nested_thread(irq, true);
#ifdef CONFIG_ARM
	set_irq_flags(irq, IRQF_VALID);
#else
	irq_set_noprobe(irq);
#endif

	return 0;
}

static const struct irq_domain_ops pca953x_irq_simple_ops = {
	.map = pca953x_gpio_irq_map,
	.xlate = irq_domain_xlate_twocell,
};

>>>>>>> refs/remotes/origin/master
static int pca953x_irq_setup(struct pca953x_chip *chip,
			     const struct i2c_device_id *id,
			     int irq_base)
{
	struct i2c_client *client = chip->client;
<<<<<<< HEAD
	int ret, offset = 0;

	if (irq_base != -1
			&& (id->driver_data & PCA_INT)) {
		int lvl;
=======
	int ret, i, offset = 0;

	if (irq_base != -1
			&& (id->driver_data & PCA_INT)) {
>>>>>>> refs/remotes/origin/master

		switch (chip->chip_type) {
		case PCA953X_TYPE:
			offset = PCA953X_INPUT;
			break;
		case PCA957X_TYPE:
			offset = PCA957X_IN;
			break;
		}
<<<<<<< HEAD
		ret = pca953x_read_reg(chip, offset, &chip->irq_stat);
		if (ret)
			goto out_failed;
=======
		ret = pca953x_read_regs(chip, offset, chip->irq_stat);
		if (ret)
			return ret;
>>>>>>> refs/remotes/origin/master

		/*
		 * There is no way to know which GPIO line generated the
		 * interrupt.  We have to rely on the previous read for
		 * this purpose.
		 */
<<<<<<< HEAD
		chip->irq_stat &= chip->reg_direction;
		mutex_init(&chip->irq_lock);

		chip->irq_base = irq_alloc_descs(-1, irq_base, chip->gpio_chip.ngpio, -1);
		if (chip->irq_base < 0)
			goto out_failed;

		for (lvl = 0; lvl < chip->gpio_chip.ngpio; lvl++) {
			int irq = lvl + chip->irq_base;

			irq_clear_status_flags(irq, IRQ_NOREQUEST);
			irq_set_chip_data(irq, chip);
			irq_set_chip(irq, &pca953x_irq_chip);
			irq_set_nested_thread(irq, true);
#ifdef CONFIG_ARM
			set_irq_flags(irq, IRQF_VALID);
#else
			irq_set_noprobe(irq);
#endif
		}

		ret = request_threaded_irq(client->irq,
=======
		for (i = 0; i < NBANK(chip); i++)
			chip->irq_stat[i] &= chip->reg_direction[i];
		mutex_init(&chip->irq_lock);

		chip->domain = irq_domain_add_simple(client->dev.of_node,
						chip->gpio_chip.ngpio,
						irq_base,
						&pca953x_irq_simple_ops,
						chip);
		if (!chip->domain)
			return -ENODEV;

		ret = devm_request_threaded_irq(&client->dev,
					client->irq,
>>>>>>> refs/remotes/origin/master
					   NULL,
					   pca953x_irq_handler,
					   IRQF_TRIGGER_LOW | IRQF_ONESHOT,
					   dev_name(&client->dev), chip);
		if (ret) {
			dev_err(&client->dev, "failed to request irq %d\n",
				client->irq);
<<<<<<< HEAD
			goto out_failed;
=======
			return ret;
>>>>>>> refs/remotes/origin/master
		}

		chip->gpio_chip.to_irq = pca953x_gpio_to_irq;
	}

	return 0;
<<<<<<< HEAD

out_failed:
	chip->irq_base = -1;
	return ret;
}

static void pca953x_irq_teardown(struct pca953x_chip *chip)
{
	if (chip->irq_base != -1) {
		irq_free_descs(chip->irq_base, chip->gpio_chip.ngpio);
		free_irq(chip->client->irq, chip);
	}
}
=======
}

>>>>>>> refs/remotes/origin/master
#else /* CONFIG_GPIO_PCA953X_IRQ */
static int pca953x_irq_setup(struct pca953x_chip *chip,
			     const struct i2c_device_id *id,
			     int irq_base)
{
	struct i2c_client *client = chip->client;

	if (irq_base != -1 && (id->driver_data & PCA_INT))
		dev_warn(&client->dev, "interrupt support not compiled in\n");

	return 0;
}
<<<<<<< HEAD

static void pca953x_irq_teardown(struct pca953x_chip *chip)
{
}
=======
>>>>>>> refs/remotes/origin/master
#endif

/*
 * Handlers for alternative sources of platform_data
 */
#ifdef CONFIG_OF_GPIO
/*
 * Translate OpenFirmware node properties into platform_data
 * WARNING: This is DEPRECATED and will be removed eventually!
 */
static void
<<<<<<< HEAD
pca953x_get_alt_pdata(struct i2c_client *client, int *gpio_base, int *invert)
=======
pca953x_get_alt_pdata(struct i2c_client *client, int *gpio_base, u32 *invert)
>>>>>>> refs/remotes/origin/master
{
	struct device_node *node;
	const __be32 *val;
	int size;

	node = client->dev.of_node;
	if (node == NULL)
		return;

	*gpio_base = -1;
	val = of_get_property(node, "linux,gpio-base", &size);
	WARN(val, "%s: device-tree property 'linux,gpio-base' is deprecated!", __func__);
	if (val) {
		if (size != sizeof(*val))
			dev_warn(&client->dev, "%s: wrong linux,gpio-base\n",
				 node->full_name);
		else
			*gpio_base = be32_to_cpup(val);
	}

	val = of_get_property(node, "polarity", NULL);
	WARN(val, "%s: device-tree property 'polarity' is deprecated!", __func__);
	if (val)
		*invert = *val;
}
#else
static void
<<<<<<< HEAD
pca953x_get_alt_pdata(struct i2c_client *client, int *gpio_base, int *invert)
=======
pca953x_get_alt_pdata(struct i2c_client *client, int *gpio_base, u32 *invert)
>>>>>>> refs/remotes/origin/master
{
	*gpio_base = -1;
}
#endif

<<<<<<< HEAD
static int __devinit device_pca953x_init(struct pca953x_chip *chip, int invert)
{
	int ret;

	ret = pca953x_read_reg(chip, PCA953X_OUTPUT, &chip->reg_output);
	if (ret)
		goto out;

	ret = pca953x_read_reg(chip, PCA953X_DIRECTION,
			       &chip->reg_direction);
=======
static int device_pca953x_init(struct pca953x_chip *chip, u32 invert)
{
	int ret;
	u8 val[MAX_BANK];

	ret = pca953x_read_regs(chip, PCA953X_OUTPUT, chip->reg_output);
	if (ret)
		goto out;

	ret = pca953x_read_regs(chip, PCA953X_DIRECTION,
			       chip->reg_direction);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto out;

	/* set platform specific polarity inversion */
<<<<<<< HEAD
	ret = pca953x_write_reg(chip, PCA953X_INVERT, invert);
=======
	if (invert)
		memset(val, 0xFF, NBANK(chip));
	else
		memset(val, 0, NBANK(chip));

	ret = pca953x_write_regs(chip, PCA953X_INVERT, val);
>>>>>>> refs/remotes/origin/master
out:
	return ret;
}

<<<<<<< HEAD
static int __devinit device_pca957x_init(struct pca953x_chip *chip, int invert)
{
	int ret;
	uint16_t val = 0;

	/* Let every port in proper state, that could save power */
	pca953x_write_reg(chip, PCA957X_PUPD, 0x0);
	pca953x_write_reg(chip, PCA957X_CFG, 0xffff);
	pca953x_write_reg(chip, PCA957X_OUT, 0x0);

	ret = pca953x_read_reg(chip, PCA957X_IN, &val);
	if (ret)
		goto out;
	ret = pca953x_read_reg(chip, PCA957X_OUT, &chip->reg_output);
	if (ret)
		goto out;
	ret = pca953x_read_reg(chip, PCA957X_CFG, &chip->reg_direction);
=======
static int device_pca957x_init(struct pca953x_chip *chip, u32 invert)
{
	int ret;
	u8 val[MAX_BANK];

	ret = pca953x_read_regs(chip, PCA957X_OUT, chip->reg_output);
	if (ret)
		goto out;
	ret = pca953x_read_regs(chip, PCA957X_CFG, chip->reg_direction);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto out;

	/* set platform specific polarity inversion */
<<<<<<< HEAD
	pca953x_write_reg(chip, PCA957X_INVRT, invert);

	/* To enable register 6, 7 to controll pull up and pull down */
	pca953x_write_reg(chip, PCA957X_BKEN, 0x202);
=======
	if (invert)
		memset(val, 0xFF, NBANK(chip));
	else
		memset(val, 0, NBANK(chip));
	pca953x_write_regs(chip, PCA957X_INVRT, val);

	/* To enable register 6, 7 to controll pull up and pull down */
	memset(val, 0x02, NBANK(chip));
	pca953x_write_regs(chip, PCA957X_BKEN, val);
>>>>>>> refs/remotes/origin/master

	return 0;
out:
	return ret;
}

<<<<<<< HEAD
static int __devinit pca953x_probe(struct i2c_client *client,
=======
static int pca953x_probe(struct i2c_client *client,
>>>>>>> refs/remotes/origin/master
				   const struct i2c_device_id *id)
{
	struct pca953x_platform_data *pdata;
	struct pca953x_chip *chip;
<<<<<<< HEAD
	int irq_base=0, invert=0;
	int ret;

	chip = kzalloc(sizeof(struct pca953x_chip), GFP_KERNEL);
	if (chip == NULL)
		return -ENOMEM;

	pdata = client->dev.platform_data;
=======
	int irq_base = 0;
	int ret;
	u32 invert = 0;

	chip = devm_kzalloc(&client->dev,
			sizeof(struct pca953x_chip), GFP_KERNEL);
	if (chip == NULL)
		return -ENOMEM;

	pdata = dev_get_platdata(&client->dev);
>>>>>>> refs/remotes/origin/master
	if (pdata) {
		irq_base = pdata->irq_base;
		chip->gpio_start = pdata->gpio_base;
		invert = pdata->invert;
		chip->names = pdata->names;
	} else {
		pca953x_get_alt_pdata(client, &chip->gpio_start, &invert);
#ifdef CONFIG_OF_GPIO
		/* If I2C node has no interrupts property, disable GPIO interrupts */
		if (of_find_property(client->dev.of_node, "interrupts", NULL) == NULL)
			irq_base = -1;
#endif
	}

	chip->client = client;

	chip->chip_type = id->driver_data & (PCA953X_TYPE | PCA957X_TYPE);

	mutex_init(&chip->i2c_lock);

	/* initialize cached registers from their original values.
	 * we can't share this chip with another i2c master.
	 */
	pca953x_setup_gpio(chip, id->driver_data & PCA_GPIO_MASK);

	if (chip->chip_type == PCA953X_TYPE)
		ret = device_pca953x_init(chip, invert);
	else
		ret = device_pca957x_init(chip, invert);
	if (ret)
<<<<<<< HEAD
		goto out_failed;

	ret = pca953x_irq_setup(chip, id, irq_base);
	if (ret)
		goto out_failed;

	ret = gpiochip_add(&chip->gpio_chip);
	if (ret)
		goto out_failed_irq;
=======
		return ret;

	ret = pca953x_irq_setup(chip, id, irq_base);
	if (ret)
		return ret;

	ret = gpiochip_add(&chip->gpio_chip);
	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/master

	if (pdata && pdata->setup) {
		ret = pdata->setup(client, chip->gpio_chip.base,
				chip->gpio_chip.ngpio, pdata->context);
		if (ret < 0)
			dev_warn(&client->dev, "setup failed, %d\n", ret);
	}

	i2c_set_clientdata(client, chip);
	return 0;
<<<<<<< HEAD

out_failed_irq:
	pca953x_irq_teardown(chip);
out_failed:
	kfree(chip);
	return ret;
=======
>>>>>>> refs/remotes/origin/master
}

static int pca953x_remove(struct i2c_client *client)
{
<<<<<<< HEAD
	struct pca953x_platform_data *pdata = client->dev.platform_data;
=======
	struct pca953x_platform_data *pdata = dev_get_platdata(&client->dev);
>>>>>>> refs/remotes/origin/master
	struct pca953x_chip *chip = i2c_get_clientdata(client);
	int ret = 0;

	if (pdata && pdata->teardown) {
		ret = pdata->teardown(client, chip->gpio_chip.base,
				chip->gpio_chip.ngpio, pdata->context);
		if (ret < 0) {
			dev_err(&client->dev, "%s failed, %d\n",
					"teardown", ret);
			return ret;
		}
	}

	ret = gpiochip_remove(&chip->gpio_chip);
	if (ret) {
		dev_err(&client->dev, "%s failed, %d\n",
				"gpiochip_remove()", ret);
		return ret;
	}

<<<<<<< HEAD
	pca953x_irq_teardown(chip);
	kfree(chip);
	return 0;
}

static struct i2c_driver pca953x_driver = {
	.driver = {
		.name	= "pca953x",
=======
	return 0;
}

static const struct of_device_id pca953x_dt_ids[] = {
	{ .compatible = "nxp,pca9505", },
	{ .compatible = "nxp,pca9534", },
	{ .compatible = "nxp,pca9535", },
	{ .compatible = "nxp,pca9536", },
	{ .compatible = "nxp,pca9537", },
	{ .compatible = "nxp,pca9538", },
	{ .compatible = "nxp,pca9539", },
	{ .compatible = "nxp,pca9554", },
	{ .compatible = "nxp,pca9555", },
	{ .compatible = "nxp,pca9556", },
	{ .compatible = "nxp,pca9557", },
	{ .compatible = "nxp,pca9574", },
	{ .compatible = "nxp,pca9575", },

	{ .compatible = "maxim,max7310", },
	{ .compatible = "maxim,max7312", },
	{ .compatible = "maxim,max7313", },
	{ .compatible = "maxim,max7315", },

	{ .compatible = "ti,pca6107", },
	{ .compatible = "ti,tca6408", },
	{ .compatible = "ti,tca6416", },
	{ .compatible = "ti,tca6424", },
	{ }
};

MODULE_DEVICE_TABLE(of, pca953x_dt_ids);

static struct i2c_driver pca953x_driver = {
	.driver = {
		.name	= "pca953x",
		.of_match_table = pca953x_dt_ids,
>>>>>>> refs/remotes/origin/master
	},
	.probe		= pca953x_probe,
	.remove		= pca953x_remove,
	.id_table	= pca953x_id,
};

static int __init pca953x_init(void)
{
	return i2c_add_driver(&pca953x_driver);
}
/* register after i2c postcore initcall and before
 * subsys initcalls that may rely on these GPIOs
 */
subsys_initcall(pca953x_init);

static void __exit pca953x_exit(void)
{
	i2c_del_driver(&pca953x_driver);
}
module_exit(pca953x_exit);

MODULE_AUTHOR("eric miao <eric.miao@marvell.com>");
MODULE_DESCRIPTION("GPIO expander driver for PCA953x");
MODULE_LICENSE("GPL");
