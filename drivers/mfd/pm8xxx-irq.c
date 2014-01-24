/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2011, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
 * Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define pr_fmt(fmt)	"%s: " fmt, __func__

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-11.0
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/mfd/pm8xxx/core.h>
#include <linux/mfd/pm8xxx/irq.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

/* PMIC8xxx IRQ */

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define SSBI_REG_ADDR_IRQ_ROOT(base)		(base + 0)
#define SSBI_REG_ADDR_IRQ_M_STATUS1(base)	(base + 1)
#define SSBI_REG_ADDR_IRQ_M_STATUS2(base)	(base + 2)
#define SSBI_REG_ADDR_IRQ_M_STATUS3(base)	(base + 3)
#define SSBI_REG_ADDR_IRQ_M_STATUS4(base)	(base + 4)
#define SSBI_REG_ADDR_IRQ_BLK_SEL(base)		(base + 5)
#define SSBI_REG_ADDR_IRQ_IT_STATUS(base)	(base + 6)
#define SSBI_REG_ADDR_IRQ_CONFIG(base)		(base + 7)
#define SSBI_REG_ADDR_IRQ_RT_STATUS(base)	(base + 8)
<<<<<<< HEAD
=======
#define	SSBI_REG_ADDR_IRQ_BASE		0x1BB

#define	SSBI_REG_ADDR_IRQ_ROOT		(SSBI_REG_ADDR_IRQ_BASE + 0)
#define	SSBI_REG_ADDR_IRQ_M_STATUS1	(SSBI_REG_ADDR_IRQ_BASE + 1)
#define	SSBI_REG_ADDR_IRQ_M_STATUS2	(SSBI_REG_ADDR_IRQ_BASE + 2)
#define	SSBI_REG_ADDR_IRQ_M_STATUS3	(SSBI_REG_ADDR_IRQ_BASE + 3)
#define	SSBI_REG_ADDR_IRQ_M_STATUS4	(SSBI_REG_ADDR_IRQ_BASE + 4)
#define	SSBI_REG_ADDR_IRQ_BLK_SEL	(SSBI_REG_ADDR_IRQ_BASE + 5)
#define	SSBI_REG_ADDR_IRQ_IT_STATUS	(SSBI_REG_ADDR_IRQ_BASE + 6)
#define	SSBI_REG_ADDR_IRQ_CONFIG	(SSBI_REG_ADDR_IRQ_BASE + 7)
#define	SSBI_REG_ADDR_IRQ_RT_STATUS	(SSBI_REG_ADDR_IRQ_BASE + 8)
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

#define	PM_IRQF_LVL_SEL			0x01	/* level select */
#define	PM_IRQF_MASK_FE			0x02	/* mask falling edge */
#define	PM_IRQF_MASK_RE			0x04	/* mask rising edge */
#define	PM_IRQF_CLR			0x08	/* clear interrupt */
#define	PM_IRQF_BITS_MASK		0x70
#define	PM_IRQF_BITS_SHIFT		4
#define	PM_IRQF_WRITE			0x80

#define	PM_IRQF_MASK_ALL		(PM_IRQF_MASK_FE | \
					PM_IRQF_MASK_RE)

struct pm_irq_chip {
	struct device		*dev;
	spinlock_t		pm_irq_lock;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int		base_addr;
=======
>>>>>>> refs/remotes/origin/master
=======
	unsigned int		base_addr;
>>>>>>> refs/remotes/origin/cm-11.0
	unsigned int		devirq;
	unsigned int		irq_base;
	unsigned int		num_irqs;
	unsigned int		num_blocks;
	unsigned int		num_masters;
	u8			config[0];
};

static int pm8xxx_read_root_irq(const struct pm_irq_chip *chip, u8 *rp)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return pm8xxx_readb(chip->dev,
			SSBI_REG_ADDR_IRQ_ROOT(chip->base_addr), rp);
=======
	return pm8xxx_readb(chip->dev, SSBI_REG_ADDR_IRQ_ROOT, rp);
>>>>>>> refs/remotes/origin/master
=======
	return pm8xxx_readb(chip->dev,
			SSBI_REG_ADDR_IRQ_ROOT(chip->base_addr), rp);
>>>>>>> refs/remotes/origin/cm-11.0
}

static int pm8xxx_read_master_irq(const struct pm_irq_chip *chip, u8 m, u8 *bp)
{
	return pm8xxx_readb(chip->dev,
<<<<<<< HEAD
<<<<<<< HEAD
			SSBI_REG_ADDR_IRQ_M_STATUS1(chip->base_addr) + m, bp);
=======
			SSBI_REG_ADDR_IRQ_M_STATUS1 + m, bp);
>>>>>>> refs/remotes/origin/master
=======
			SSBI_REG_ADDR_IRQ_M_STATUS1(chip->base_addr) + m, bp);
>>>>>>> refs/remotes/origin/cm-11.0
}

static int pm8xxx_read_block_irq(struct pm_irq_chip *chip, u8 bp, u8 *ip)
{
	int	rc;

	spin_lock(&chip->pm_irq_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	rc = pm8xxx_writeb(chip->dev,
			SSBI_REG_ADDR_IRQ_BLK_SEL(chip->base_addr), bp);
=======
	rc = pm8xxx_writeb(chip->dev, SSBI_REG_ADDR_IRQ_BLK_SEL, bp);
>>>>>>> refs/remotes/origin/master
=======
	rc = pm8xxx_writeb(chip->dev,
			SSBI_REG_ADDR_IRQ_BLK_SEL(chip->base_addr), bp);
>>>>>>> refs/remotes/origin/cm-11.0
	if (rc) {
		pr_err("Failed Selecting Block %d rc=%d\n", bp, rc);
		goto bail;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	rc = pm8xxx_readb(chip->dev,
			SSBI_REG_ADDR_IRQ_IT_STATUS(chip->base_addr), ip);
=======
	rc = pm8xxx_readb(chip->dev, SSBI_REG_ADDR_IRQ_IT_STATUS, ip);
>>>>>>> refs/remotes/origin/master
=======
	rc = pm8xxx_readb(chip->dev,
			SSBI_REG_ADDR_IRQ_IT_STATUS(chip->base_addr), ip);
>>>>>>> refs/remotes/origin/cm-11.0
	if (rc)
		pr_err("Failed Reading Status rc=%d\n", rc);
bail:
	spin_unlock(&chip->pm_irq_lock);
	return rc;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int pm8xxx_read_config_irq(struct pm_irq_chip *chip, u8 bp, u8 cp, u8 *r)
=======
static int pm8xxx_config_irq(struct pm_irq_chip *chip, u8 bp, u8 cp)
>>>>>>> refs/remotes/origin/master
=======
static int pm8xxx_read_config_irq(struct pm_irq_chip *chip, u8 bp, u8 cp, u8 *r)
>>>>>>> refs/remotes/origin/cm-11.0
{
	int	rc;

	spin_lock(&chip->pm_irq_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	rc = pm8xxx_writeb(chip->dev,
			SSBI_REG_ADDR_IRQ_BLK_SEL(chip->base_addr), bp);
=======
	rc = pm8xxx_writeb(chip->dev, SSBI_REG_ADDR_IRQ_BLK_SEL, bp);
>>>>>>> refs/remotes/origin/master
=======
	rc = pm8xxx_writeb(chip->dev,
			SSBI_REG_ADDR_IRQ_BLK_SEL(chip->base_addr), bp);
>>>>>>> refs/remotes/origin/cm-11.0
	if (rc) {
		pr_err("Failed Selecting Block %d rc=%d\n", bp, rc);
		goto bail;
	}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	cp &= ~PM_IRQF_WRITE;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cp &= ~PM_IRQF_WRITE;
>>>>>>> refs/remotes/origin/cm-11.0
	rc = pm8xxx_writeb(chip->dev,
			SSBI_REG_ADDR_IRQ_CONFIG(chip->base_addr), cp);
	if (rc)
		pr_err("Failed Configuring IRQ rc=%d\n", rc);

	rc = pm8xxx_readb(chip->dev,
			SSBI_REG_ADDR_IRQ_CONFIG(chip->base_addr), r);
	if (rc)
		pr_err("Failed reading IRQ rc=%d\n", rc);
bail:
	spin_unlock(&chip->pm_irq_lock);
	return rc;
}

static int pm8xxx_write_config_irq(struct pm_irq_chip *chip, u8 bp, u8 cp)
{
	int	rc;

	spin_lock(&chip->pm_irq_lock);
	rc = pm8xxx_writeb(chip->dev,
			SSBI_REG_ADDR_IRQ_BLK_SEL(chip->base_addr), bp);
	if (rc) {
		pr_err("Failed Selecting Block %d rc=%d\n", bp, rc);
		goto bail;
	}
<<<<<<< HEAD
<<<<<<< HEAD

=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/*
	 * Set the write bit here as this could be a unrequested irq
	 * whose PM_IRQF_WRITE bit is not set
	 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	cp |= PM_IRQF_WRITE;
	rc = pm8xxx_writeb(chip->dev,
			SSBI_REG_ADDR_IRQ_CONFIG(chip->base_addr), cp);
=======
	cp |= PM_IRQF_WRITE;
	rc = pm8xxx_writeb(chip->dev, SSBI_REG_ADDR_IRQ_CONFIG, cp);
>>>>>>> refs/remotes/origin/master
=======
	cp |= PM_IRQF_WRITE;
	rc = pm8xxx_writeb(chip->dev,
			SSBI_REG_ADDR_IRQ_CONFIG(chip->base_addr), cp);
>>>>>>> refs/remotes/origin/cm-11.0
	if (rc)
		pr_err("Failed Configuring IRQ rc=%d\n", rc);
bail:
	spin_unlock(&chip->pm_irq_lock);
	return rc;
}

static int pm8xxx_irq_block_handler(struct pm_irq_chip *chip, int block)
{
	int pmirq, irq, i, ret = 0;
	u8 bits;

	ret = pm8xxx_read_block_irq(chip, block, &bits);
	if (ret) {
		pr_err("Failed reading %d block ret=%d", block, ret);
		return ret;
	}
	if (!bits) {
		pr_err("block bit set in master but no irqs: %d", block);
		return 0;
	}

	/* Check IRQ bits */
	for (i = 0; i < 8; i++) {
		if (bits & (1 << i)) {
			pmirq = block * 8 + i;
			irq = pmirq + chip->irq_base;
			generic_handle_irq(irq);
		}
	}
	return 0;
}

static int pm8xxx_irq_master_handler(struct pm_irq_chip *chip, int master)
{
	u8 blockbits;
	int block_number, i, ret = 0;

	ret = pm8xxx_read_master_irq(chip, master, &blockbits);
	if (ret) {
		pr_err("Failed to read master %d ret=%d\n", master, ret);
		return ret;
	}
	if (!blockbits) {
		pr_err("master bit set in root but no blocks: %d", master);
		return 0;
	}

	for (i = 0; i < 8; i++)
		if (blockbits & (1 << i)) {
			block_number = master * 8 + i;	/* block # */
			ret |= pm8xxx_irq_block_handler(chip, block_number);
		}
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static irqreturn_t pm8xxx_irq_handler(int irq, void *data)
{
	struct pm_irq_chip *chip = data;
=======
static void pm8xxx_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	struct pm_irq_chip *chip = irq_desc_get_handler_data(desc);
	struct irq_chip *irq_chip = irq_desc_get_chip(desc);
>>>>>>> refs/remotes/origin/master
=======
static irqreturn_t pm8xxx_irq_handler(int irq, void *data)
{
	struct pm_irq_chip *chip = data;
>>>>>>> refs/remotes/origin/cm-11.0
	u8	root;
	int	i, ret, masters = 0;

	ret = pm8xxx_read_root_irq(chip, &root);
	if (ret) {
		pr_err("Can't read root status ret=%d\n", ret);
<<<<<<< HEAD
<<<<<<< HEAD
		return IRQ_HANDLED;
=======
		return;
>>>>>>> refs/remotes/origin/master
=======
		return IRQ_HANDLED;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	/* on pm8xxx series masters start from bit 1 of the root */
	masters = root >> 1;

	/* Read allowed masters for blocks. */
	for (i = 0; i < chip->num_masters; i++)
		if (masters & (1 << i))
			pm8xxx_irq_master_handler(chip, i);

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	return IRQ_HANDLED;
}

static void pm8xxx_irq_mask(struct irq_data *d)
{
	struct pm_irq_chip *chip = irq_data_get_irq_chip_data(d);
	unsigned int pmirq = d->irq - chip->irq_base;
	int	master, irq_bit;
	u8	block, config;

	block = pmirq / 8;
	master = block / 8;
	irq_bit = pmirq % 8;

	if (chip->config[pmirq] == 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		pr_warn("masking rouge irq=%d pmirq=%d\n", d->irq, pmirq);
=======
		pr_warn("masking rogue irq=%d pmirq=%d\n", d->irq, pmirq);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_warn("masking rogue irq=%d pmirq=%d\n", d->irq, pmirq);
>>>>>>> refs/remotes/origin/cm-11.0
		chip->config[pmirq] = irq_bit << PM_IRQF_BITS_SHIFT;
	}

	config = chip->config[pmirq] | PM_IRQF_MASK_ALL;
	pm8xxx_write_config_irq(chip, block, config);
<<<<<<< HEAD
=======
	irq_chip->irq_ack(&desc->irq_data);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static void pm8xxx_irq_mask_ack(struct irq_data *d)
{
	struct pm_irq_chip *chip = irq_data_get_irq_chip_data(d);
	unsigned int pmirq = d->irq - chip->irq_base;
	int	master, irq_bit;
	u8	block, config;

	block = pmirq / 8;
	master = block / 8;
	irq_bit = pmirq % 8;

<<<<<<< HEAD
<<<<<<< HEAD
	if (chip->config[pmirq] == 0) {
<<<<<<< HEAD
		pr_warn("mask acking rouge irq=%d pmirq=%d\n", d->irq, pmirq);
=======
		pr_warn("mask acking rogue irq=%d pmirq=%d\n", d->irq, pmirq);
>>>>>>> refs/remotes/origin/cm-10.0
		chip->config[pmirq] = irq_bit << PM_IRQF_BITS_SHIFT;
	}

	config = chip->config[pmirq] | PM_IRQF_MASK_ALL | PM_IRQF_CLR;
	pm8xxx_write_config_irq(chip, block, config);
=======
	config = chip->config[pmirq] | PM_IRQF_MASK_ALL | PM_IRQF_CLR;
	pm8xxx_config_irq(chip, block, config);
>>>>>>> refs/remotes/origin/master
=======
	if (chip->config[pmirq] == 0) {
		pr_warn("mask acking rogue irq=%d pmirq=%d\n", d->irq, pmirq);
		chip->config[pmirq] = irq_bit << PM_IRQF_BITS_SHIFT;
	}

	config = chip->config[pmirq] | PM_IRQF_MASK_ALL | PM_IRQF_CLR;
	pm8xxx_write_config_irq(chip, block, config);
>>>>>>> refs/remotes/origin/cm-11.0
}

static void pm8xxx_irq_unmask(struct irq_data *d)
{
	struct pm_irq_chip *chip = irq_data_get_irq_chip_data(d);
	unsigned int pmirq = d->irq - chip->irq_base;
	int	master, irq_bit;
<<<<<<< HEAD
<<<<<<< HEAD
	u8	block, config, hw_conf;
=======
	u8	block, config;
>>>>>>> refs/remotes/origin/master
=======
	u8	block, config, hw_conf;
>>>>>>> refs/remotes/origin/cm-11.0

	block = pmirq / 8;
	master = block / 8;
	irq_bit = pmirq % 8;

	config = chip->config[pmirq];
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	pm8xxx_read_config_irq(chip, block, config, &hw_conf);
	/* check if it is masked */
	if ((hw_conf & PM_IRQF_MASK_ALL) == PM_IRQF_MASK_ALL)
		pm8xxx_write_config_irq(chip, block, config);
<<<<<<< HEAD
=======
	pm8xxx_config_irq(chip, block, config);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static int pm8xxx_irq_set_type(struct irq_data *d, unsigned int flow_type)
{
	struct pm_irq_chip *chip = irq_data_get_irq_chip_data(d);
	unsigned int pmirq = d->irq - chip->irq_base;
	int master, irq_bit;
	u8 block, config;

	block = pmirq / 8;
	master = block / 8;
	irq_bit  = pmirq % 8;

	chip->config[pmirq] = (irq_bit << PM_IRQF_BITS_SHIFT)
							| PM_IRQF_MASK_ALL;
	if (flow_type & (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING)) {
		if (flow_type & IRQF_TRIGGER_RISING)
			chip->config[pmirq] &= ~PM_IRQF_MASK_RE;
		if (flow_type & IRQF_TRIGGER_FALLING)
			chip->config[pmirq] &= ~PM_IRQF_MASK_FE;
	} else {
		chip->config[pmirq] |= PM_IRQF_LVL_SEL;

		if (flow_type & IRQF_TRIGGER_HIGH)
			chip->config[pmirq] &= ~PM_IRQF_MASK_RE;
		else
			chip->config[pmirq] &= ~PM_IRQF_MASK_FE;
	}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/*
	 * The PM_IRQF_WRITE flag serves as an indication that this interrupt
	 * been requested
	 */
	chip->config[pmirq] |= PM_IRQF_WRITE;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	config = chip->config[pmirq] | PM_IRQF_CLR;
	return pm8xxx_write_config_irq(chip, block, config);
=======
	config = chip->config[pmirq] | PM_IRQF_CLR;
	return pm8xxx_config_irq(chip, block, config);
>>>>>>> refs/remotes/origin/master
=======
	config = chip->config[pmirq] | PM_IRQF_CLR;
	return pm8xxx_write_config_irq(chip, block, config);
>>>>>>> refs/remotes/origin/cm-11.0
}

static int pm8xxx_irq_set_wake(struct irq_data *d, unsigned int on)
{
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int pm8xxx_irq_read_line(struct irq_data *d)
{
	struct pm_irq_chip *chip = irq_data_get_irq_chip_data(d);

	return pm8xxx_get_irq_stat(chip, d->irq);
}

<<<<<<< HEAD
static struct irq_chip pm8xxx_irq_chip = {
	.name		= "pm8xxx",
	.irq_mask	= pm8xxx_irq_mask,
=======
static struct irq_chip pm8xxx_irq_chip = {
	.name		= "pm8xxx",
>>>>>>> refs/remotes/origin/master
=======
static struct irq_chip pm8xxx_irq_chip = {
	.name		= "pm8xxx",
	.irq_mask	= pm8xxx_irq_mask,
>>>>>>> refs/remotes/origin/cm-11.0
	.irq_mask_ack	= pm8xxx_irq_mask_ack,
	.irq_unmask	= pm8xxx_irq_unmask,
	.irq_set_type	= pm8xxx_irq_set_type,
	.irq_set_wake	= pm8xxx_irq_set_wake,
<<<<<<< HEAD
<<<<<<< HEAD
	.irq_read_line	= pm8xxx_irq_read_line,
=======
>>>>>>> refs/remotes/origin/master
=======
	.irq_read_line	= pm8xxx_irq_read_line,
>>>>>>> refs/remotes/origin/cm-11.0
	.flags		= IRQCHIP_MASK_ON_SUSPEND,
};

/**
 * pm8xxx_get_irq_stat - get the status of the irq line
 * @chip: pointer to identify a pmic irq controller
 * @irq: the irq number
 *
 * The pm8xxx gpio and mpp rely on the interrupt block to read
 * the values on their pins. This function is to facilitate reading
 * the status of a gpio or an mpp line. The caller has to convert the
 * gpio number to irq number.
 *
 * RETURNS:
 * an int indicating the value read on that line
 */
int pm8xxx_get_irq_stat(struct pm_irq_chip *chip, int irq)
{
	int pmirq, rc;
	u8  block, bits, bit;
	unsigned long flags;

	if (chip == NULL || irq < chip->irq_base ||
			irq >= chip->irq_base + chip->num_irqs)
		return -EINVAL;

	pmirq = irq - chip->irq_base;

	block = pmirq / 8;
	bit = pmirq % 8;

	spin_lock_irqsave(&chip->pm_irq_lock, flags);

<<<<<<< HEAD
<<<<<<< HEAD
	rc = pm8xxx_writeb(chip->dev,
			SSBI_REG_ADDR_IRQ_BLK_SEL(chip->base_addr), block);
=======
	rc = pm8xxx_writeb(chip->dev, SSBI_REG_ADDR_IRQ_BLK_SEL, block);
>>>>>>> refs/remotes/origin/master
=======
	rc = pm8xxx_writeb(chip->dev,
			SSBI_REG_ADDR_IRQ_BLK_SEL(chip->base_addr), block);
>>>>>>> refs/remotes/origin/cm-11.0
	if (rc) {
		pr_err("Failed Selecting block irq=%d pmirq=%d blk=%d rc=%d\n",
			irq, pmirq, block, rc);
		goto bail_out;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	rc = pm8xxx_readb(chip->dev,
			SSBI_REG_ADDR_IRQ_RT_STATUS(chip->base_addr), &bits);
=======
	rc = pm8xxx_readb(chip->dev, SSBI_REG_ADDR_IRQ_RT_STATUS, &bits);
>>>>>>> refs/remotes/origin/master
=======
	rc = pm8xxx_readb(chip->dev,
			SSBI_REG_ADDR_IRQ_RT_STATUS(chip->base_addr), &bits);
>>>>>>> refs/remotes/origin/cm-11.0
	if (rc) {
		pr_err("Failed Configuring irq=%d pmirq=%d blk=%d rc=%d\n",
			irq, pmirq, block, rc);
		goto bail_out;
	}

	rc = (bits & (1 << bit)) ? 1 : 0;

bail_out:
	spin_unlock_irqrestore(&chip->pm_irq_lock, flags);

	return rc;
}
EXPORT_SYMBOL_GPL(pm8xxx_get_irq_stat);

<<<<<<< HEAD
struct pm_irq_chip *  __devinit pm8xxx_irq_init(struct device *dev,
=======
struct pm_irq_chip *  pm8xxx_irq_init(struct device *dev,
>>>>>>> refs/remotes/origin/master
				const struct pm8xxx_irq_platform_data *pdata)
{
	struct pm_irq_chip  *chip;
	int devirq, rc;
	unsigned int pmirq;

	if (!pdata) {
		pr_err("No platform data\n");
		return ERR_PTR(-EINVAL);
	}

	devirq = pdata->devirq;
	if (devirq < 0) {
		pr_err("missing devirq\n");
		rc = devirq;
		return ERR_PTR(-EINVAL);
	}

	chip = kzalloc(sizeof(struct pm_irq_chip)
			+ sizeof(u8) * pdata->irq_cdata.nirqs, GFP_KERNEL);
	if (!chip) {
		pr_err("Cannot alloc pm_irq_chip struct\n");
		return ERR_PTR(-EINVAL);
	}

	chip->dev = dev;
	chip->devirq = devirq;
	chip->irq_base = pdata->irq_base;
	chip->num_irqs = pdata->irq_cdata.nirqs;
<<<<<<< HEAD
<<<<<<< HEAD
	chip->base_addr = pdata->irq_cdata.base_addr;
=======
>>>>>>> refs/remotes/origin/master
=======
	chip->base_addr = pdata->irq_cdata.base_addr;
>>>>>>> refs/remotes/origin/cm-11.0
	chip->num_blocks = DIV_ROUND_UP(chip->num_irqs, 8);
	chip->num_masters = DIV_ROUND_UP(chip->num_blocks, 8);
	spin_lock_init(&chip->pm_irq_lock);

	for (pmirq = 0; pmirq < chip->num_irqs; pmirq++) {
		irq_set_chip_and_handler(chip->irq_base + pmirq,
				&pm8xxx_irq_chip,
				handle_level_irq);
		irq_set_chip_data(chip->irq_base + pmirq, chip);
#ifdef CONFIG_ARM
		set_irq_flags(chip->irq_base + pmirq, IRQF_VALID);
#else
		irq_set_noprobe(chip->irq_base + pmirq);
#endif
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (devirq != 0) {
		rc = request_irq(devirq, pm8xxx_irq_handler,
				pdata->irq_trigger_flag,
				"pm8xxx_usr_irq", chip);
		if (rc) {
			pr_err("failed to request_irq for %d rc=%d\n",
								devirq, rc);
		} else {
			irq_set_irq_wake(devirq, 1);
		}
	}
<<<<<<< HEAD
=======
	irq_set_irq_type(devirq, pdata->irq_trigger_flag);
	irq_set_handler_data(devirq, chip);
	irq_set_chained_handler(devirq, pm8xxx_irq_handler);
	set_irq_wake(devirq, 1);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	return chip;
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
int __devexit pm8xxx_irq_exit(struct pm_irq_chip *chip)
=======
int pm8xxx_irq_exit(struct pm_irq_chip *chip)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int pm8xxx_irq_exit(struct pm_irq_chip *chip)
>>>>>>> refs/remotes/origin/master
=======
int pm8xxx_irq_exit(struct pm_irq_chip *chip)
>>>>>>> refs/remotes/origin/cm-11.0
{
	irq_set_chained_handler(chip->devirq, NULL);
	kfree(chip);
	return 0;
}
