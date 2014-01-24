<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
/* Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
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
 *
<<<<<<< HEAD
<<<<<<< HEAD
 */
#include <linux/bitmap.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/irq.h>

#include <mach/msm_iomap.h>
#include <mach/gpiomux.h>
#include "gpio-msm-common.h"
=======
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
=======
>>>>>>> refs/remotes/origin/cm-11.0
 */
#include <linux/bitmap.h>
#include <linux/bitops.h>
<<<<<<< HEAD
#include <linux/err.h>
=======
#include <linux/delay.h>
>>>>>>> refs/remotes/origin/cm-11.0
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/irqchip/chained_irq.h>
#include <linux/irq.h>
<<<<<<< HEAD
#include <linux/irqdomain.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/slab.h>

#define MAX_NR_GPIO 300
>>>>>>> refs/remotes/origin/master
=======

#include <mach/msm_iomap.h>
#include <mach/gpiomux.h>
#include "gpio-msm-common.h"
>>>>>>> refs/remotes/origin/cm-11.0

/* Bits of interest in the GPIO_IN_OUT register.
 */
enum {
<<<<<<< HEAD
<<<<<<< HEAD
	GPIO_IN_BIT  = 0,
	GPIO_OUT_BIT = 1
=======
	GPIO_IN  = 0,
	GPIO_OUT = 1
>>>>>>> refs/remotes/origin/master
=======
	GPIO_IN_BIT  = 0,
	GPIO_OUT_BIT = 1
>>>>>>> refs/remotes/origin/cm-11.0
};

/* Bits of interest in the GPIO_INTR_STATUS register.
 */
enum {
<<<<<<< HEAD
<<<<<<< HEAD
	INTR_STATUS_BIT = 0,
=======
	INTR_STATUS = 0,
>>>>>>> refs/remotes/origin/master
=======
	INTR_STATUS_BIT = 0,
>>>>>>> refs/remotes/origin/cm-11.0
};

/* Bits of interest in the GPIO_CFG register.
 */
enum {
<<<<<<< HEAD
<<<<<<< HEAD
	GPIO_OE_BIT = 9,
};

/* Bits of interest in the GPIO_INTR_CFG register.
 */
enum {
	INTR_ENABLE_BIT        = 0,
	INTR_POL_CTL_BIT       = 1,
	INTR_DECT_CTL_BIT      = 2,
	INTR_RAW_STATUS_EN_BIT = 3,
=======
	GPIO_OE = 9,
=======
	GPIO_OE_BIT = 9,
>>>>>>> refs/remotes/origin/cm-11.0
};

/* Bits of interest in the GPIO_INTR_CFG register.
 */
enum {
<<<<<<< HEAD
	INTR_ENABLE        = 0,
	INTR_POL_CTL       = 1,
	INTR_DECT_CTL      = 2,
	INTR_RAW_STATUS_EN = 3,
>>>>>>> refs/remotes/origin/master
=======
	INTR_ENABLE_BIT        = 0,
	INTR_POL_CTL_BIT       = 1,
	INTR_DECT_CTL_BIT      = 2,
	INTR_RAW_STATUS_EN_BIT = 3,
>>>>>>> refs/remotes/origin/cm-11.0
};

/* Codes of interest in GPIO_INTR_CFG_SU.
 */
enum {
	TARGET_PROC_SCORPION = 4,
	TARGET_PROC_NONE     = 7,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
/*
 * There is no 'DC_POLARITY_LO' because the GIC is incapable
 * of asserting on falling edge or level-low conditions.  Even though
 * the registers allow for low-polarity inputs, the case can never arise.
 */
enum {
	DC_POLARITY_HI	= BIT(11),
	DC_IRQ_ENABLE	= BIT(3),
};

/*
 * When a GPIO triggers, two separate decisions are made, controlled
 * by two separate flags.
 *
 * - First, INTR_RAW_STATUS_EN controls whether or not the GPIO_INTR_STATUS
 * register for that GPIO will be updated to reflect the triggering of that
 * gpio.  If this bit is 0, this register will not be updated.
 * - Second, INTR_ENABLE controls whether an interrupt is triggered.
 *
 * If INTR_ENABLE is set and INTR_RAW_STATUS_EN is NOT set, an interrupt
 * can be triggered but the status register will not reflect it.
 */
#define INTR_RAW_STATUS_EN BIT(INTR_RAW_STATUS_EN_BIT)
#define INTR_ENABLE        BIT(INTR_ENABLE_BIT)
#define INTR_DECT_CTL_EDGE BIT(INTR_DECT_CTL_BIT)
#define INTR_POL_CTL_HI    BIT(INTR_POL_CTL_BIT)

#define GPIO_INTR_CFG_SU(gpio)    (MSM_TLMM_BASE + 0x0400 + (0x04 * (gpio)))
#define DIR_CONN_INTR_CFG_SU(irq) (MSM_TLMM_BASE + 0x0700 + (0x04 * (irq)))
#define GPIO_CONFIG(gpio)         (MSM_TLMM_BASE + 0x1000 + (0x10 * (gpio)))
#define GPIO_IN_OUT(gpio)         (MSM_TLMM_BASE + 0x1004 + (0x10 * (gpio)))
#define GPIO_INTR_CFG(gpio)       (MSM_TLMM_BASE + 0x1008 + (0x10 * (gpio)))
#define GPIO_INTR_STATUS(gpio)    (MSM_TLMM_BASE + 0x100c + (0x10 * (gpio)))

<<<<<<< HEAD
static inline void set_gpio_bits(unsigned n, void __iomem *reg)
{
	__raw_writel(__raw_readl(reg) | n, reg);
}

static inline void clr_gpio_bits(unsigned n, void __iomem *reg)
{
	__raw_writel(__raw_readl(reg) & ~n, reg);
}

unsigned __msm_gpio_get_inout(unsigned gpio)
{
	return __raw_readl(GPIO_IN_OUT(gpio)) & BIT(GPIO_IN_BIT);
}

void __msm_gpio_set_inout(unsigned gpio, unsigned val)
{
	__raw_writel(val ? BIT(GPIO_OUT_BIT) : 0, GPIO_IN_OUT(gpio));
}

void __msm_gpio_set_config_direction(unsigned gpio, int input, int val)
{
	if (input)
		clr_gpio_bits(BIT(GPIO_OE_BIT), GPIO_CONFIG(gpio));
	else {
		__msm_gpio_set_inout(gpio, val);
		set_gpio_bits(BIT(GPIO_OE_BIT), GPIO_CONFIG(gpio));
	}
}

void __msm_gpio_set_polarity(unsigned gpio, unsigned val)
{
	if (val)
		clr_gpio_bits(INTR_POL_CTL_HI, GPIO_INTR_CFG(gpio));
	else
		set_gpio_bits(INTR_POL_CTL_HI, GPIO_INTR_CFG(gpio));
}

unsigned __msm_gpio_get_intr_status(unsigned gpio)
{
	return __raw_readl(GPIO_INTR_STATUS(gpio)) &
					BIT(INTR_STATUS_BIT);
}

void __msm_gpio_set_intr_status(unsigned gpio)
{
	__raw_writel(BIT(INTR_STATUS_BIT), GPIO_INTR_STATUS(gpio));
}

unsigned __msm_gpio_get_intr_config(unsigned gpio)
{
	return __raw_readl(GPIO_INTR_CFG(gpio));
}

void __msm_gpio_set_intr_cfg_enable(unsigned gpio, unsigned val)
{
	if (val) {
		set_gpio_bits(INTR_ENABLE, GPIO_INTR_CFG(gpio));

	} else {
		clr_gpio_bits(INTR_ENABLE, GPIO_INTR_CFG(gpio));
	}
}

void __msm_gpio_set_intr_cfg_type(unsigned gpio, unsigned type)
{
	unsigned cfg;

	/* RAW_STATUS_EN is left on for all gpio irqs. Due to the
	 * internal circuitry of TLMM, toggling the RAW_STATUS
	 * could cause the INTR_STATUS to be set for EDGE interrupts.
	 */
	cfg  = __msm_gpio_get_intr_config(gpio);
	cfg |= INTR_RAW_STATUS_EN;
	__raw_writel(cfg, GPIO_INTR_CFG(gpio));
	__raw_writel(TARGET_PROC_SCORPION, GPIO_INTR_CFG_SU(gpio));

	cfg  = __msm_gpio_get_intr_config(gpio);
	if (type & IRQ_TYPE_EDGE_BOTH)
		cfg |= INTR_DECT_CTL_EDGE;
	else
		cfg &= ~INTR_DECT_CTL_EDGE;

	if (type & (IRQ_TYPE_EDGE_RISING | IRQ_TYPE_LEVEL_HIGH))
		cfg |= INTR_POL_CTL_HI;
	else
		cfg &= ~INTR_POL_CTL_HI;

	__raw_writel(cfg, GPIO_INTR_CFG(gpio));
	/* Sometimes it might take a little while to update
	 * the interrupt status after the RAW_STATUS is enabled
	 * We clear the interrupt status before enabling the
	 * interrupt in the unmask call-back.
	 */
	udelay(5);
}

void __gpio_tlmm_config(unsigned config)
{
	uint32_t flags;
	unsigned gpio = GPIO_PIN(config);

	flags = ((GPIO_DIR(config) << 9) & (0x1 << 9)) |
		((GPIO_DRVSTR(config) << 6) & (0x7 << 6)) |
		((GPIO_FUNC(config) << 2) & (0xf << 2)) |
		((GPIO_PULL(config) & 0x3));
	__raw_writel(flags, GPIO_CONFIG(gpio));
}

void __msm_gpio_install_direct_irq(unsigned gpio, unsigned irq,
					unsigned int input_polarity)
{
	uint32_t bits;

	__raw_writel(__raw_readl(GPIO_CONFIG(gpio)) | BIT(GPIO_OE_BIT),
		GPIO_CONFIG(gpio));
	__raw_writel(__raw_readl(GPIO_INTR_CFG(gpio)) &
		~(INTR_RAW_STATUS_EN | INTR_ENABLE),
		GPIO_INTR_CFG(gpio));
	__raw_writel(DC_IRQ_ENABLE | TARGET_PROC_NONE,
		GPIO_INTR_CFG_SU(gpio));

	bits = TARGET_PROC_SCORPION | (gpio << 3);
	if (input_polarity)
		bits |= DC_POLARITY_HI;
	__raw_writel(bits, DIR_CONN_INTR_CFG_SU(irq));
}
=======
/**
 * struct msm_gpio_dev: the MSM8660 SoC GPIO device structure
 *
 * @enabled_irqs: a bitmap used to optimize the summary-irq handler.  By
 * keeping track of which gpios are unmasked as irq sources, we avoid
 * having to do readl calls on hundreds of iomapped registers each time
 * the summary interrupt fires in order to locate the active interrupts.
 *
 * @wake_irqs: a bitmap for tracking which interrupt lines are enabled
 * as wakeup sources.  When the device is suspended, interrupts which are
 * not wakeup sources are disabled.
 *
 * @dual_edge_irqs: a bitmap used to track which irqs are configured
 * as dual-edge, as this is not supported by the hardware and requires
 * some special handling in the driver.
 */
struct msm_gpio_dev {
	struct gpio_chip gpio_chip;
	DECLARE_BITMAP(enabled_irqs, MAX_NR_GPIO);
	DECLARE_BITMAP(wake_irqs, MAX_NR_GPIO);
	DECLARE_BITMAP(dual_edge_irqs, MAX_NR_GPIO);
	struct irq_domain *domain;
	int summary_irq;
	void __iomem *msm_tlmm_base;
};

static struct msm_gpio_dev msm_gpio;

#define GPIO_INTR_CFG_SU(gpio)    (msm_gpio.msm_tlmm_base + 0x0400 + \
								(0x04 * (gpio)))
#define GPIO_CONFIG(gpio)         (msm_gpio.msm_tlmm_base + 0x1000 + \
								(0x10 * (gpio)))
#define GPIO_IN_OUT(gpio)         (msm_gpio.msm_tlmm_base + 0x1004 + \
								(0x10 * (gpio)))
#define GPIO_INTR_CFG(gpio)       (msm_gpio.msm_tlmm_base + 0x1008 + \
								(0x10 * (gpio)))
#define GPIO_INTR_STATUS(gpio)    (msm_gpio.msm_tlmm_base + 0x100c + \
								(0x10 * (gpio)))

static DEFINE_SPINLOCK(tlmm_lock);

static inline struct msm_gpio_dev *to_msm_gpio_dev(struct gpio_chip *chip)
{
	return container_of(chip, struct msm_gpio_dev, gpio_chip);
}

=======
>>>>>>> refs/remotes/origin/cm-11.0
static inline void set_gpio_bits(unsigned n, void __iomem *reg)
{
	__raw_writel(__raw_readl(reg) | n, reg);
}

static inline void clr_gpio_bits(unsigned n, void __iomem *reg)
{
<<<<<<< HEAD
	return 0;
=======
	__raw_writel(__raw_readl(reg) & ~n, reg);
>>>>>>> refs/remotes/origin/cm-11.0
}

unsigned __msm_gpio_get_inout(unsigned gpio)
{
<<<<<<< HEAD
	return;
=======
	return __raw_readl(GPIO_IN_OUT(gpio)) & BIT(GPIO_IN_BIT);
>>>>>>> refs/remotes/origin/cm-11.0
}

void __msm_gpio_set_inout(unsigned gpio, unsigned val)
{
<<<<<<< HEAD
	struct msm_gpio_dev *g_dev = to_msm_gpio_dev(chip);
	struct irq_domain *domain = g_dev->domain;

	return irq_create_mapping(domain, offset);
=======
	__raw_writel(val ? BIT(GPIO_OUT_BIT) : 0, GPIO_IN_OUT(gpio));
>>>>>>> refs/remotes/origin/cm-11.0
}

void __msm_gpio_set_config_direction(unsigned gpio, int input, int val)
{
<<<<<<< HEAD
	struct irq_data *irq_data = irq_get_irq_data(irq);

	return irq_data->hwirq;
}


/* For dual-edge interrupts in software, since the hardware has no
 * such support:
 *
 * At appropriate moments, this function may be called to flip the polarity
 * settings of both-edge irq lines to try and catch the next edge.
 *
 * The attempt is considered successful if:
 * - the status bit goes high, indicating that an edge was caught, or
 * - the input value of the gpio doesn't change during the attempt.
 * If the value changes twice during the process, that would cause the first
 * test to fail but would force the second, as two opposite
 * transitions would cause a detection no matter the polarity setting.
 *
 * The do-loop tries to sledge-hammer closed the timing hole between
 * the initial value-read and the polarity-write - if the line value changes
 * during that window, an interrupt is lost, the new polarity setting is
 * incorrect, and the first success test will fail, causing a retry.
 *
 * Algorithm comes from Google's msmgpio driver, see mach-msm/gpio.c.
 */
static void msm_gpio_update_dual_edge_pos(unsigned gpio)
{
	int loop_limit = 100;
	unsigned val, val2, intstat;

	do {
		val = readl(GPIO_IN_OUT(gpio)) & BIT(GPIO_IN);
		if (val)
			clear_gpio_bits(BIT(INTR_POL_CTL), GPIO_INTR_CFG(gpio));
		else
			set_gpio_bits(BIT(INTR_POL_CTL), GPIO_INTR_CFG(gpio));
		val2 = readl(GPIO_IN_OUT(gpio)) & BIT(GPIO_IN);
		intstat = readl(GPIO_INTR_STATUS(gpio)) & BIT(INTR_STATUS);
		if (intstat || val == val2)
			return;
	} while (loop_limit-- > 0);
	pr_err("%s: dual-edge irq failed to stabilize, "
	       "interrupts dropped. %#08x != %#08x\n",
	       __func__, val, val2);
=======
	if (input)
		clr_gpio_bits(BIT(GPIO_OE_BIT), GPIO_CONFIG(gpio));
	else {
		__msm_gpio_set_inout(gpio, val);
		set_gpio_bits(BIT(GPIO_OE_BIT), GPIO_CONFIG(gpio));
	}
}

void __msm_gpio_set_polarity(unsigned gpio, unsigned val)
{
	if (val)
		clr_gpio_bits(INTR_POL_CTL_HI, GPIO_INTR_CFG(gpio));
	else
		set_gpio_bits(INTR_POL_CTL_HI, GPIO_INTR_CFG(gpio));
>>>>>>> refs/remotes/origin/cm-11.0
}

unsigned __msm_gpio_get_intr_status(unsigned gpio)
{
	return __raw_readl(GPIO_INTR_STATUS(gpio)) &
					BIT(INTR_STATUS_BIT);
}

void __msm_gpio_set_intr_status(unsigned gpio)
{
<<<<<<< HEAD
	int gpio = msm_irq_to_gpio(&msm_gpio.gpio_chip, d->irq);
	unsigned long irq_flags;

	spin_lock_irqsave(&tlmm_lock, irq_flags);
	writel(TARGET_PROC_NONE, GPIO_INTR_CFG_SU(gpio));
	clear_gpio_bits(BIT(INTR_RAW_STATUS_EN) | BIT(INTR_ENABLE), GPIO_INTR_CFG(gpio));
	__clear_bit(gpio, msm_gpio.enabled_irqs);
	spin_unlock_irqrestore(&tlmm_lock, irq_flags);
=======
	__raw_writel(BIT(INTR_STATUS_BIT), GPIO_INTR_STATUS(gpio));
>>>>>>> refs/remotes/origin/cm-11.0
}

unsigned __msm_gpio_get_intr_config(unsigned gpio)
{
<<<<<<< HEAD
	int gpio = msm_irq_to_gpio(&msm_gpio.gpio_chip, d->irq);
	unsigned long irq_flags;

	spin_lock_irqsave(&tlmm_lock, irq_flags);
	__set_bit(gpio, msm_gpio.enabled_irqs);
	set_gpio_bits(BIT(INTR_RAW_STATUS_EN) | BIT(INTR_ENABLE), GPIO_INTR_CFG(gpio));
	writel(TARGET_PROC_SCORPION, GPIO_INTR_CFG_SU(gpio));
	spin_unlock_irqrestore(&tlmm_lock, irq_flags);
=======
	return __raw_readl(GPIO_INTR_CFG(gpio));
>>>>>>> refs/remotes/origin/cm-11.0
}

void __msm_gpio_set_intr_cfg_enable(unsigned gpio, unsigned val)
{
	if (val) {
		set_gpio_bits(INTR_ENABLE, GPIO_INTR_CFG(gpio));

	} else {
<<<<<<< HEAD
		bits &= ~BIT(INTR_DECT_CTL);
		__irq_set_handler_locked(d->irq, handle_level_irq);
		__clear_bit(gpio, msm_gpio.dual_edge_irqs);
	}

	if (flow_type & (IRQ_TYPE_EDGE_RISING | IRQ_TYPE_LEVEL_HIGH))
		bits |= BIT(INTR_POL_CTL);
	else
		bits &= ~BIT(INTR_POL_CTL);

	writel(bits, GPIO_INTR_CFG(gpio));

	if ((flow_type & IRQ_TYPE_EDGE_BOTH) == IRQ_TYPE_EDGE_BOTH)
		msm_gpio_update_dual_edge_pos(gpio);

	spin_unlock_irqrestore(&tlmm_lock, irq_flags);

	return 0;
}

/*
 * When the summary IRQ is raised, any number of GPIO lines may be high.
 * It is the job of the summary handler to find all those GPIO lines
 * which have been set as summary IRQ lines and which are triggered,
 * and to call their interrupt handlers.
 */
static void msm_summary_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	unsigned long i;
	struct irq_chip *chip = irq_desc_get_chip(desc);

	chained_irq_enter(chip, desc);

	for_each_set_bit(i, msm_gpio.enabled_irqs, MAX_NR_GPIO) {
		if (readl(GPIO_INTR_STATUS(i)) & BIT(INTR_STATUS))
			generic_handle_irq(irq_find_mapping(msm_gpio.domain,
								i));
=======
		clr_gpio_bits(INTR_ENABLE, GPIO_INTR_CFG(gpio));
>>>>>>> refs/remotes/origin/cm-11.0
	}
}

void __msm_gpio_set_intr_cfg_type(unsigned gpio, unsigned type)
{
<<<<<<< HEAD
	int gpio = msm_irq_to_gpio(&msm_gpio.gpio_chip, d->irq);

	if (on) {
		if (bitmap_empty(msm_gpio.wake_irqs, MAX_NR_GPIO))
			irq_set_irq_wake(msm_gpio.summary_irq, 1);
		set_bit(gpio, msm_gpio.wake_irqs);
	} else {
		clear_bit(gpio, msm_gpio.wake_irqs);
		if (bitmap_empty(msm_gpio.wake_irqs, MAX_NR_GPIO))
			irq_set_irq_wake(msm_gpio.summary_irq, 0);
	}
=======
	unsigned cfg;
>>>>>>> refs/remotes/origin/cm-11.0

	/* RAW_STATUS_EN is left on for all gpio irqs. Due to the
	 * internal circuitry of TLMM, toggling the RAW_STATUS
	 * could cause the INTR_STATUS to be set for EDGE interrupts.
	 */
	cfg  = __msm_gpio_get_intr_config(gpio);
	cfg |= INTR_RAW_STATUS_EN;
	__raw_writel(cfg, GPIO_INTR_CFG(gpio));
	__raw_writel(TARGET_PROC_SCORPION, GPIO_INTR_CFG_SU(gpio));

<<<<<<< HEAD
static struct lock_class_key msm_gpio_lock_class;

static int msm_gpio_irq_domain_map(struct irq_domain *d, unsigned int irq,
				   irq_hw_number_t hwirq)
{
	irq_set_lockdep_class(irq, &msm_gpio_lock_class);
	irq_set_chip_and_handler(irq, &msm_gpio_irq_chip,
			handle_level_irq);
	set_irq_flags(irq, IRQF_VALID);

	return 0;
}

static const struct irq_domain_ops msm_gpio_irq_domain_ops = {
	.xlate = irq_domain_xlate_twocell,
	.map = msm_gpio_irq_domain_map,
};

static int msm_gpio_probe(struct platform_device *pdev)
{
	int ret, ngpio;
	struct resource *res;

	if (of_property_read_u32(pdev->dev.of_node, "ngpio", &ngpio)) {
		dev_err(&pdev->dev, "%s: ngpio property missing\n", __func__);
		return -EINVAL;
	}

	if (ngpio > MAX_NR_GPIO)
		WARN(1, "ngpio exceeds the MAX_NR_GPIO. Increase MAX_NR_GPIO\n");

	bitmap_zero(msm_gpio.enabled_irqs, MAX_NR_GPIO);
	bitmap_zero(msm_gpio.wake_irqs, MAX_NR_GPIO);
	bitmap_zero(msm_gpio.dual_edge_irqs, MAX_NR_GPIO);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	msm_gpio.msm_tlmm_base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(msm_gpio.msm_tlmm_base))
		return PTR_ERR(msm_gpio.msm_tlmm_base);

	msm_gpio.gpio_chip.ngpio = ngpio;
	msm_gpio.gpio_chip.label = pdev->name;
	msm_gpio.gpio_chip.dev = &pdev->dev;
	msm_gpio.gpio_chip.base = 0;
	msm_gpio.gpio_chip.direction_input = msm_gpio_direction_input;
	msm_gpio.gpio_chip.direction_output = msm_gpio_direction_output;
	msm_gpio.gpio_chip.get = msm_gpio_get;
	msm_gpio.gpio_chip.set = msm_gpio_set;
	msm_gpio.gpio_chip.to_irq = msm_gpio_to_irq;
	msm_gpio.gpio_chip.request = msm_gpio_request;
	msm_gpio.gpio_chip.free = msm_gpio_free;

	ret = gpiochip_add(&msm_gpio.gpio_chip);
	if (ret < 0) {
		dev_err(&pdev->dev, "gpiochip_add failed with error %d\n", ret);
		return ret;
	}

	msm_gpio.summary_irq = platform_get_irq(pdev, 0);
	if (msm_gpio.summary_irq < 0) {
		dev_err(&pdev->dev, "No Summary irq defined for msmgpio\n");
		return msm_gpio.summary_irq;
	}

	msm_gpio.domain = irq_domain_add_linear(pdev->dev.of_node, ngpio,
						&msm_gpio_irq_domain_ops,
						&msm_gpio);
	if (!msm_gpio.domain)
		return -ENODEV;

	irq_set_chained_handler(msm_gpio.summary_irq, msm_summary_irq_handler);

	return 0;
}

static const struct of_device_id msm_gpio_of_match[] = {
	{ .compatible = "qcom,msm-gpio", },
	{ },
};
MODULE_DEVICE_TABLE(of, msm_gpio_of_match);

static int msm_gpio_remove(struct platform_device *dev)
=======
	cfg  = __msm_gpio_get_intr_config(gpio);
	if (type & IRQ_TYPE_EDGE_BOTH)
		cfg |= INTR_DECT_CTL_EDGE;
	else
		cfg &= ~INTR_DECT_CTL_EDGE;

	if (type & (IRQ_TYPE_EDGE_RISING | IRQ_TYPE_LEVEL_HIGH))
		cfg |= INTR_POL_CTL_HI;
	else
		cfg &= ~INTR_POL_CTL_HI;

	__raw_writel(cfg, GPIO_INTR_CFG(gpio));
	/* Sometimes it might take a little while to update
	 * the interrupt status after the RAW_STATUS is enabled
	 * We clear the interrupt status before enabling the
	 * interrupt in the unmask call-back.
	 */
	udelay(5);
}

void __gpio_tlmm_config(unsigned config)
>>>>>>> refs/remotes/origin/cm-11.0
{
	uint32_t flags;
	unsigned gpio = GPIO_PIN(config);

<<<<<<< HEAD
	irq_set_handler(msm_gpio.summary_irq, NULL);

	return 0;
}

static struct platform_driver msm_gpio_driver = {
	.probe = msm_gpio_probe,
	.remove = msm_gpio_remove,
	.driver = {
		.name = "msmgpio",
		.owner = THIS_MODULE,
		.of_match_table = msm_gpio_of_match,
	},
};

module_platform_driver(msm_gpio_driver)

MODULE_AUTHOR("Gregory Bean <gbean@codeaurora.org>");
MODULE_DESCRIPTION("Driver for Qualcomm MSM TLMMv2 SoC GPIOs");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:msmgpio");
>>>>>>> refs/remotes/origin/master
=======
	flags = ((GPIO_DIR(config) << 9) & (0x1 << 9)) |
		((GPIO_DRVSTR(config) << 6) & (0x7 << 6)) |
		((GPIO_FUNC(config) << 2) & (0xf << 2)) |
		((GPIO_PULL(config) & 0x3));
	__raw_writel(flags, GPIO_CONFIG(gpio));
}

void __msm_gpio_install_direct_irq(unsigned gpio, unsigned irq,
					unsigned int input_polarity)
{
	uint32_t bits;

	__raw_writel(__raw_readl(GPIO_CONFIG(gpio)) | BIT(GPIO_OE_BIT),
		GPIO_CONFIG(gpio));
	__raw_writel(__raw_readl(GPIO_INTR_CFG(gpio)) &
		~(INTR_RAW_STATUS_EN | INTR_ENABLE),
		GPIO_INTR_CFG(gpio));
	__raw_writel(DC_IRQ_ENABLE | TARGET_PROC_NONE,
		GPIO_INTR_CFG_SU(gpio));

	bits = TARGET_PROC_SCORPION | (gpio << 3);
	if (input_polarity)
		bits |= DC_POLARITY_HI;
	__raw_writel(bits, DIR_CONN_INTR_CFG_SU(irq));
}
>>>>>>> refs/remotes/origin/cm-11.0
