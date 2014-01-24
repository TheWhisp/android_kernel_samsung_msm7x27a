/*
 * arch/arm/mach-mv78xx0/irq.c
 *
 * MV78xx0 IRQ handling.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
<<<<<<< HEAD
<<<<<<< HEAD

=======
#include <linux/gpio.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/irq.h>
<<<<<<< HEAD
#include <asm/gpio.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <mach/bridge-regs.h>
#include <plat/irq.h>
#include "common.h"

static void gpio_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	BUG_ON(irq < IRQ_MV78XX0_GPIO_0_7 || irq > IRQ_MV78XX0_GPIO_24_31);

	orion_gpio_irq_handler((irq - IRQ_MV78XX0_GPIO_0_7) << 3);
}

void __init mv78xx0_init_irq(void)
{
	orion_irq_init(0, (void __iomem *)(IRQ_VIRT_BASE + IRQ_MASK_LOW_OFF));
	orion_irq_init(32, (void __iomem *)(IRQ_VIRT_BASE + IRQ_MASK_HIGH_OFF));
	orion_irq_init(64, (void __iomem *)(IRQ_VIRT_BASE + IRQ_MASK_ERR_OFF));
=======
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <mach/bridge-regs.h>
#include <plat/orion-gpio.h>
#include <plat/irq.h>
#include "common.h"

static int __initdata gpio0_irqs[4] = {
	IRQ_MV78XX0_GPIO_0_7,
	IRQ_MV78XX0_GPIO_8_15,
	IRQ_MV78XX0_GPIO_16_23,
	IRQ_MV78XX0_GPIO_24_31,
};

void __init mv78xx0_init_irq(void)
{
	orion_irq_init(0, IRQ_VIRT_BASE + IRQ_MASK_LOW_OFF);
	orion_irq_init(32, IRQ_VIRT_BASE + IRQ_MASK_HIGH_OFF);
	orion_irq_init(64, IRQ_VIRT_BASE + IRQ_MASK_ERR_OFF);
>>>>>>> refs/remotes/origin/master

	/*
	 * Initialize gpiolib for GPIOs 0-31.  (The GPIO interrupt mask
	 * registers for core #1 are at an offset of 0x18 from those of
	 * core #0.)
	 */
<<<<<<< HEAD
	orion_gpio_init(0, 32, GPIO_VIRT_BASE,
			mv78xx0_core_index() ? 0x18 : 0,
			IRQ_MV78XX0_GPIO_START);
	irq_set_chained_handler(IRQ_MV78XX0_GPIO_0_7, gpio_irq_handler);
	irq_set_chained_handler(IRQ_MV78XX0_GPIO_8_15, gpio_irq_handler);
	irq_set_chained_handler(IRQ_MV78XX0_GPIO_16_23, gpio_irq_handler);
	irq_set_chained_handler(IRQ_MV78XX0_GPIO_24_31, gpio_irq_handler);
=======
	orion_gpio_init(NULL, 0, 32, GPIO_VIRT_BASE,
			mv78xx0_core_index() ? 0x18 : 0,
			IRQ_MV78XX0_GPIO_START, gpio0_irqs);
>>>>>>> refs/remotes/origin/master
}
