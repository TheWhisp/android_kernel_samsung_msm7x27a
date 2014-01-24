/*
 *  linux/drivers/mfd/ucb1x00-core.c
 *
 *  Copyright (C) 2001 Russell King, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 *  The UCB1x00 core driver provides basic services for handling IO,
 *  the ADC, interrupts, and accessing registers.  It is designed
 *  such that everything goes through this layer, thereby providing
 *  a consistent locking methodology, as well as allowing the drivers
 *  to be used on other non-MCP-enabled hardware platforms.
 *
 *  Note that all locks are private to this file.  Nothing else may
 *  touch them.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/mfd/ucb1x00.h>
#include <linux/gpio.h>
#include <linux/semaphore.h>

#include <mach/dma.h>
#include <mach/hardware.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/irq.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/mfd/ucb1x00.h>
#include <linux/pm.h>
#include <linux/gpio.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static DEFINE_MUTEX(ucb1x00_mutex);
static LIST_HEAD(ucb1x00_drivers);
static LIST_HEAD(ucb1x00_devices);

/**
 *	ucb1x00_io_set_dir - set IO direction
 *	@ucb: UCB1x00 structure describing chip
 *	@in:  bitfield of IO pins to be set as inputs
 *	@out: bitfield of IO pins to be set as outputs
 *
 *	Set the IO direction of the ten general purpose IO pins on
 *	the UCB1x00 chip.  The @in bitfield has priority over the
 *	@out bitfield, in that if you specify a pin as both input
 *	and output, it will end up as an input.
 *
 *	ucb1x00_enable must have been called to enable the comms
 *	before using this function.
 *
 *	This function takes a spinlock, disabling interrupts.
 */
void ucb1x00_io_set_dir(struct ucb1x00 *ucb, unsigned int in, unsigned int out)
{
	unsigned long flags;

	spin_lock_irqsave(&ucb->io_lock, flags);
	ucb->io_dir |= out;
	ucb->io_dir &= ~in;

	ucb1x00_reg_write(ucb, UCB_IO_DIR, ucb->io_dir);
	spin_unlock_irqrestore(&ucb->io_lock, flags);
}

/**
 *	ucb1x00_io_write - set or clear IO outputs
 *	@ucb:   UCB1x00 structure describing chip
 *	@set:   bitfield of IO pins to set to logic '1'
 *	@clear: bitfield of IO pins to set to logic '0'
 *
 *	Set the IO output state of the specified IO pins.  The value
 *	is retained if the pins are subsequently configured as inputs.
 *	The @clear bitfield has priority over the @set bitfield -
 *	outputs will be cleared.
 *
 *	ucb1x00_enable must have been called to enable the comms
 *	before using this function.
 *
 *	This function takes a spinlock, disabling interrupts.
 */
void ucb1x00_io_write(struct ucb1x00 *ucb, unsigned int set, unsigned int clear)
{
	unsigned long flags;

	spin_lock_irqsave(&ucb->io_lock, flags);
	ucb->io_out |= set;
	ucb->io_out &= ~clear;

	ucb1x00_reg_write(ucb, UCB_IO_DATA, ucb->io_out);
	spin_unlock_irqrestore(&ucb->io_lock, flags);
}

/**
 *	ucb1x00_io_read - read the current state of the IO pins
 *	@ucb: UCB1x00 structure describing chip
 *
 *	Return a bitfield describing the logic state of the ten
 *	general purpose IO pins.
 *
 *	ucb1x00_enable must have been called to enable the comms
 *	before using this function.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *	This function does not take any semaphores or spinlocks.
=======
 *	This function does not take any mutexes or spinlocks.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *	This function does not take any mutexes or spinlocks.
>>>>>>> refs/remotes/origin/master
 */
unsigned int ucb1x00_io_read(struct ucb1x00 *ucb)
{
	return ucb1x00_reg_read(ucb, UCB_IO_DATA);
}

static void ucb1x00_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
	struct ucb1x00 *ucb = container_of(chip, struct ucb1x00, gpio);
	unsigned long flags;

	spin_lock_irqsave(&ucb->io_lock, flags);
	if (value)
		ucb->io_out |= 1 << offset;
	else
		ucb->io_out &= ~(1 << offset);

<<<<<<< HEAD
<<<<<<< HEAD
	ucb1x00_reg_write(ucb, UCB_IO_DATA, ucb->io_out);
=======
	ucb1x00_enable(ucb);
	ucb1x00_reg_write(ucb, UCB_IO_DATA, ucb->io_out);
	ucb1x00_disable(ucb);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ucb1x00_enable(ucb);
	ucb1x00_reg_write(ucb, UCB_IO_DATA, ucb->io_out);
	ucb1x00_disable(ucb);
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&ucb->io_lock, flags);
}

static int ucb1x00_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	struct ucb1x00 *ucb = container_of(chip, struct ucb1x00, gpio);
<<<<<<< HEAD
<<<<<<< HEAD
	return ucb1x00_reg_read(ucb, UCB_IO_DATA) & (1 << offset);
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned val;

	ucb1x00_enable(ucb);
	val = ucb1x00_reg_read(ucb, UCB_IO_DATA);
	ucb1x00_disable(ucb);

	return val & (1 << offset);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static int ucb1x00_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
{
	struct ucb1x00 *ucb = container_of(chip, struct ucb1x00, gpio);
	unsigned long flags;

	spin_lock_irqsave(&ucb->io_lock, flags);
	ucb->io_dir &= ~(1 << offset);
<<<<<<< HEAD
<<<<<<< HEAD
	ucb1x00_reg_write(ucb, UCB_IO_DIR, ucb->io_dir);
=======
	ucb1x00_enable(ucb);
	ucb1x00_reg_write(ucb, UCB_IO_DIR, ucb->io_dir);
	ucb1x00_disable(ucb);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ucb1x00_enable(ucb);
	ucb1x00_reg_write(ucb, UCB_IO_DIR, ucb->io_dir);
	ucb1x00_disable(ucb);
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&ucb->io_lock, flags);

	return 0;
}

static int ucb1x00_gpio_direction_output(struct gpio_chip *chip, unsigned offset
		, int value)
{
	struct ucb1x00 *ucb = container_of(chip, struct ucb1x00, gpio);
	unsigned long flags;
<<<<<<< HEAD
<<<<<<< HEAD

	spin_lock_irqsave(&ucb->io_lock, flags);
	ucb->io_dir |= (1 << offset);
	ucb1x00_reg_write(ucb, UCB_IO_DIR, ucb->io_dir);

	if (value)
		ucb->io_out |= 1 << offset;
	else
		ucb->io_out &= ~(1 << offset);
	ucb1x00_reg_write(ucb, UCB_IO_DATA, ucb->io_out);
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned old, mask = 1 << offset;

	spin_lock_irqsave(&ucb->io_lock, flags);
	old = ucb->io_out;
	if (value)
		ucb->io_out |= mask;
	else
		ucb->io_out &= ~mask;

	ucb1x00_enable(ucb);
	if (old != ucb->io_out)
		ucb1x00_reg_write(ucb, UCB_IO_DATA, ucb->io_out);

	if (!(ucb->io_dir & mask)) {
		ucb->io_dir |= mask;
		ucb1x00_reg_write(ucb, UCB_IO_DIR, ucb->io_dir);
	}
	ucb1x00_disable(ucb);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&ucb->io_lock, flags);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static int ucb1x00_to_irq(struct gpio_chip *chip, unsigned offset)
{
	struct ucb1x00 *ucb = container_of(chip, struct ucb1x00, gpio);

	return ucb->irq_base > 0 ? ucb->irq_base + offset : -ENXIO;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * UCB1300 data sheet says we must:
 *  1. enable ADC	=> 5us (including reference startup time)
 *  2. select input	=> 51*tsibclk  => 4.3us
 *  3. start conversion	=> 102*tsibclk => 8.5us
 * (tsibclk = 1/11981000)
 * Period between SIB 128-bit frames = 10.7us
 */

/**
 *	ucb1x00_adc_enable - enable the ADC converter
 *	@ucb: UCB1x00 structure describing chip
 *
 *	Enable the ucb1x00 and ADC converter on the UCB1x00 for use.
 *	Any code wishing to use the ADC converter must call this
 *	function prior to using it.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *	This function takes the ADC semaphore to prevent two or more
=======
 *	This function takes the ADC mutex to prevent two or more
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *	This function takes the ADC mutex to prevent two or more
>>>>>>> refs/remotes/origin/master
 *	concurrent uses, and therefore may sleep.  As a result, it
 *	can only be called from process context, not interrupt
 *	context.
 *
 *	You should release the ADC as soon as possible using
 *	ucb1x00_adc_disable.
 */
void ucb1x00_adc_enable(struct ucb1x00 *ucb)
{
<<<<<<< HEAD
<<<<<<< HEAD
	down(&ucb->adc_sem);
=======
	mutex_lock(&ucb->adc_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mutex_lock(&ucb->adc_mutex);
>>>>>>> refs/remotes/origin/master

	ucb->adc_cr |= UCB_ADC_ENA;

	ucb1x00_enable(ucb);
	ucb1x00_reg_write(ucb, UCB_ADC_CR, ucb->adc_cr);
}

/**
 *	ucb1x00_adc_read - read the specified ADC channel
 *	@ucb: UCB1x00 structure describing chip
 *	@adc_channel: ADC channel mask
 *	@sync: wait for syncronisation pulse.
 *
 *	Start an ADC conversion and wait for the result.  Note that
 *	synchronised ADC conversions (via the ADCSYNC pin) must wait
 *	until the trigger is asserted and the conversion is finished.
 *
 *	This function currently spins waiting for the conversion to
 *	complete (2 frames max without sync).
 *
 *	If called for a synchronised ADC conversion, it may sleep
<<<<<<< HEAD
<<<<<<< HEAD
 *	with the ADC semaphore held.
=======
 *	with the ADC mutex held.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *	with the ADC mutex held.
>>>>>>> refs/remotes/origin/master
 */
unsigned int ucb1x00_adc_read(struct ucb1x00 *ucb, int adc_channel, int sync)
{
	unsigned int val;

	if (sync)
		adc_channel |= UCB_ADC_SYNC_ENA;

	ucb1x00_reg_write(ucb, UCB_ADC_CR, ucb->adc_cr | adc_channel);
	ucb1x00_reg_write(ucb, UCB_ADC_CR, ucb->adc_cr | adc_channel | UCB_ADC_START);

	for (;;) {
		val = ucb1x00_reg_read(ucb, UCB_ADC_DATA);
		if (val & UCB_ADC_DAT_VAL)
			break;
		/* yield to other processes */
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(1);
	}

	return UCB_ADC_DAT(val);
}

/**
 *	ucb1x00_adc_disable - disable the ADC converter
 *	@ucb: UCB1x00 structure describing chip
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *	Disable the ADC converter and release the ADC semaphore.
=======
 *	Disable the ADC converter and release the ADC mutex.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *	Disable the ADC converter and release the ADC mutex.
>>>>>>> refs/remotes/origin/master
 */
void ucb1x00_adc_disable(struct ucb1x00 *ucb)
{
	ucb->adc_cr &= ~UCB_ADC_ENA;
	ucb1x00_reg_write(ucb, UCB_ADC_CR, ucb->adc_cr);
	ucb1x00_disable(ucb);

<<<<<<< HEAD
<<<<<<< HEAD
	up(&ucb->adc_sem);
=======
	mutex_unlock(&ucb->adc_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mutex_unlock(&ucb->adc_mutex);
>>>>>>> refs/remotes/origin/master
}

/*
 * UCB1x00 Interrupt handling.
 *
 * The UCB1x00 can generate interrupts when the SIBCLK is stopped.
 * Since we need to read an internal register, we must re-enable
 * SIBCLK to talk to the chip.  We leave the clock running until
 * we have finished processing all interrupts from the chip.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static irqreturn_t ucb1x00_irq(int irqnr, void *devid)
{
	struct ucb1x00 *ucb = devid;
	struct ucb1x00_irq *irq;
=======
static void ucb1x00_irq(unsigned int irq, struct irq_desc *desc)
{
	struct ucb1x00 *ucb = irq_desc_get_handler_data(desc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void ucb1x00_irq(unsigned int irq, struct irq_desc *desc)
{
	struct ucb1x00 *ucb = irq_desc_get_handler_data(desc);
>>>>>>> refs/remotes/origin/master
	unsigned int isr, i;

	ucb1x00_enable(ucb);
	isr = ucb1x00_reg_read(ucb, UCB_IE_STATUS);
	ucb1x00_reg_write(ucb, UCB_IE_CLEAR, isr);
	ucb1x00_reg_write(ucb, UCB_IE_CLEAR, 0);

<<<<<<< HEAD
<<<<<<< HEAD
	for (i = 0, irq = ucb->irq_handler; i < 16 && isr; i++, isr >>= 1, irq++)
		if (isr & 1 && irq->fn)
			irq->fn(i, irq->devid);
	ucb1x00_disable(ucb);

	return IRQ_HANDLED;
}

/**
 *	ucb1x00_hook_irq - hook a UCB1x00 interrupt
 *	@ucb:   UCB1x00 structure describing chip
 *	@idx:   interrupt index
 *	@fn:    function to call when interrupt is triggered
 *	@devid: device id to pass to interrupt handler
 *
 *	Hook the specified interrupt.  You can only register one handler
 *	for each interrupt source.  The interrupt source is not enabled
 *	by this function; use ucb1x00_enable_irq instead.
 *
 *	Interrupt handlers will be called with other interrupts enabled.
 *
 *	Returns zero on success, or one of the following errors:
 *	 -EINVAL if the interrupt index is invalid
 *	 -EBUSY if the interrupt has already been hooked
 */
int ucb1x00_hook_irq(struct ucb1x00 *ucb, unsigned int idx, void (*fn)(int, void *), void *devid)
{
	struct ucb1x00_irq *irq;
	int ret = -EINVAL;

	if (idx < 16) {
		irq = ucb->irq_handler + idx;
		ret = -EBUSY;

		spin_lock_irq(&ucb->lock);
		if (irq->fn == NULL) {
			irq->devid = devid;
			irq->fn = fn;
			ret = 0;
		}
		spin_unlock_irq(&ucb->lock);
	}
	return ret;
}

/**
 *	ucb1x00_enable_irq - enable an UCB1x00 interrupt source
 *	@ucb: UCB1x00 structure describing chip
 *	@idx: interrupt index
 *	@edges: interrupt edges to enable
 *
 *	Enable the specified interrupt to trigger on %UCB_RISING,
 *	%UCB_FALLING or both edges.  The interrupt should have been
 *	hooked by ucb1x00_hook_irq.
 */
void ucb1x00_enable_irq(struct ucb1x00 *ucb, unsigned int idx, int edges)
{
	unsigned long flags;

	if (idx < 16) {
		spin_lock_irqsave(&ucb->lock, flags);

		ucb1x00_enable(ucb);
		if (edges & UCB_RISING) {
			ucb->irq_ris_enbl |= 1 << idx;
			ucb1x00_reg_write(ucb, UCB_IE_RIS, ucb->irq_ris_enbl);
		}
		if (edges & UCB_FALLING) {
			ucb->irq_fal_enbl |= 1 << idx;
			ucb1x00_reg_write(ucb, UCB_IE_FAL, ucb->irq_fal_enbl);
		}
		ucb1x00_disable(ucb);
		spin_unlock_irqrestore(&ucb->lock, flags);
	}
}

/**
 *	ucb1x00_disable_irq - disable an UCB1x00 interrupt source
 *	@ucb: UCB1x00 structure describing chip
 *	@edges: interrupt edges to disable
 *
 *	Disable the specified interrupt triggering on the specified
 *	(%UCB_RISING, %UCB_FALLING or both) edges.
 */
void ucb1x00_disable_irq(struct ucb1x00 *ucb, unsigned int idx, int edges)
{
	unsigned long flags;

	if (idx < 16) {
		spin_lock_irqsave(&ucb->lock, flags);

		ucb1x00_enable(ucb);
		if (edges & UCB_RISING) {
			ucb->irq_ris_enbl &= ~(1 << idx);
			ucb1x00_reg_write(ucb, UCB_IE_RIS, ucb->irq_ris_enbl);
		}
		if (edges & UCB_FALLING) {
			ucb->irq_fal_enbl &= ~(1 << idx);
			ucb1x00_reg_write(ucb, UCB_IE_FAL, ucb->irq_fal_enbl);
		}
		ucb1x00_disable(ucb);
		spin_unlock_irqrestore(&ucb->lock, flags);
	}
}

/**
 *	ucb1x00_free_irq - disable and free the specified UCB1x00 interrupt
 *	@ucb: UCB1x00 structure describing chip
 *	@idx: interrupt index
 *	@devid: device id.
 *
 *	Disable the interrupt source and remove the handler.  devid must
 *	match the devid passed when hooking the interrupt.
 *
 *	Returns zero on success, or one of the following errors:
 *	 -EINVAL if the interrupt index is invalid
 *	 -ENOENT if devid does not match
 */
int ucb1x00_free_irq(struct ucb1x00 *ucb, unsigned int idx, void *devid)
{
	struct ucb1x00_irq *irq;
	int ret;

	if (idx >= 16)
		goto bad;

	irq = ucb->irq_handler + idx;
	ret = -ENOENT;

	spin_lock_irq(&ucb->lock);
	if (irq->devid == devid) {
		ucb->irq_ris_enbl &= ~(1 << idx);
		ucb->irq_fal_enbl &= ~(1 << idx);

		ucb1x00_enable(ucb);
		ucb1x00_reg_write(ucb, UCB_IE_RIS, ucb->irq_ris_enbl);
		ucb1x00_reg_write(ucb, UCB_IE_FAL, ucb->irq_fal_enbl);
		ucb1x00_disable(ucb);

		irq->fn = NULL;
		irq->devid = NULL;
		ret = 0;
	}
	spin_unlock_irq(&ucb->lock);
	return ret;

bad:
	printk(KERN_ERR "Freeing bad UCB1x00 irq %d\n", idx);
	return -EINVAL;
}

=======
=======
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < 16 && isr; i++, isr >>= 1, irq++)
		if (isr & 1)
			generic_handle_irq(ucb->irq_base + i);
	ucb1x00_disable(ucb);
}

static void ucb1x00_irq_update(struct ucb1x00 *ucb, unsigned mask)
{
	ucb1x00_enable(ucb);
	if (ucb->irq_ris_enbl & mask)
		ucb1x00_reg_write(ucb, UCB_IE_RIS, ucb->irq_ris_enbl &
				  ucb->irq_mask);
	if (ucb->irq_fal_enbl & mask)
		ucb1x00_reg_write(ucb, UCB_IE_FAL, ucb->irq_fal_enbl &
				  ucb->irq_mask);
	ucb1x00_disable(ucb);
}

static void ucb1x00_irq_noop(struct irq_data *data)
{
}

static void ucb1x00_irq_mask(struct irq_data *data)
{
	struct ucb1x00 *ucb = irq_data_get_irq_chip_data(data);
	unsigned mask = 1 << (data->irq - ucb->irq_base);

	raw_spin_lock(&ucb->irq_lock);
	ucb->irq_mask &= ~mask;
	ucb1x00_irq_update(ucb, mask);
	raw_spin_unlock(&ucb->irq_lock);
}

static void ucb1x00_irq_unmask(struct irq_data *data)
{
	struct ucb1x00 *ucb = irq_data_get_irq_chip_data(data);
	unsigned mask = 1 << (data->irq - ucb->irq_base);

	raw_spin_lock(&ucb->irq_lock);
	ucb->irq_mask |= mask;
	ucb1x00_irq_update(ucb, mask);
	raw_spin_unlock(&ucb->irq_lock);
}

static int ucb1x00_irq_set_type(struct irq_data *data, unsigned int type)
{
	struct ucb1x00 *ucb = irq_data_get_irq_chip_data(data);
	unsigned mask = 1 << (data->irq - ucb->irq_base);

	raw_spin_lock(&ucb->irq_lock);
	if (type & IRQ_TYPE_EDGE_RISING)
		ucb->irq_ris_enbl |= mask;
	else
		ucb->irq_ris_enbl &= ~mask;

	if (type & IRQ_TYPE_EDGE_FALLING)
		ucb->irq_fal_enbl |= mask;
	else
		ucb->irq_fal_enbl &= ~mask;
	if (ucb->irq_mask & mask) {
		ucb1x00_reg_write(ucb, UCB_IE_RIS, ucb->irq_ris_enbl &
				  ucb->irq_mask);
		ucb1x00_reg_write(ucb, UCB_IE_FAL, ucb->irq_fal_enbl &
				  ucb->irq_mask);
	}
	raw_spin_unlock(&ucb->irq_lock);

	return 0;
}

static int ucb1x00_irq_set_wake(struct irq_data *data, unsigned int on)
{
	struct ucb1x00 *ucb = irq_data_get_irq_chip_data(data);
	struct ucb1x00_plat_data *pdata = ucb->mcp->attached_device.platform_data;
	unsigned mask = 1 << (data->irq - ucb->irq_base);

	if (!pdata || !pdata->can_wakeup)
		return -EINVAL;

	raw_spin_lock(&ucb->irq_lock);
	if (on)
		ucb->irq_wake |= mask;
	else
		ucb->irq_wake &= ~mask;
	raw_spin_unlock(&ucb->irq_lock);

	return 0;
}

static struct irq_chip ucb1x00_irqchip = {
	.name = "ucb1x00",
	.irq_ack = ucb1x00_irq_noop,
	.irq_mask = ucb1x00_irq_mask,
	.irq_unmask = ucb1x00_irq_unmask,
	.irq_set_type = ucb1x00_irq_set_type,
	.irq_set_wake = ucb1x00_irq_set_wake,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static int ucb1x00_add_dev(struct ucb1x00 *ucb, struct ucb1x00_driver *drv)
{
	struct ucb1x00_dev *dev;
	int ret = -ENOMEM;

	dev = kmalloc(sizeof(struct ucb1x00_dev), GFP_KERNEL);
	if (dev) {
		dev->ucb = ucb;
		dev->drv = drv;

		ret = drv->add(dev);

		if (ret == 0) {
<<<<<<< HEAD
			list_add(&dev->dev_node, &ucb->devs);
			list_add(&dev->drv_node, &drv->devs);
=======
			list_add_tail(&dev->dev_node, &ucb->devs);
			list_add_tail(&dev->drv_node, &drv->devs);
>>>>>>> refs/remotes/origin/cm-10.0
		} else {
			kfree(dev);
		}
	}
=======
static int ucb1x00_add_dev(struct ucb1x00 *ucb, struct ucb1x00_driver *drv)
{
	struct ucb1x00_dev *dev;
	int ret;

	dev = kmalloc(sizeof(struct ucb1x00_dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	dev->ucb = ucb;
	dev->drv = drv;

	ret = drv->add(dev);
	if (ret) {
		kfree(dev);
		return ret;
	}

	list_add_tail(&dev->dev_node, &ucb->devs);
	list_add_tail(&dev->drv_node, &drv->devs);

>>>>>>> refs/remotes/origin/master
	return ret;
}

static void ucb1x00_remove_dev(struct ucb1x00_dev *dev)
{
	dev->drv->remove(dev);
	list_del(&dev->dev_node);
	list_del(&dev->drv_node);
	kfree(dev);
}

/*
 * Try to probe our interrupt, rather than relying on lots of
 * hard-coded machine dependencies.  For reference, the expected
 * IRQ mappings are:
 *
 *  	Machine		Default IRQ
 *	adsbitsy	IRQ_GPCIN4
 *	cerf		IRQ_GPIO_UCB1200_IRQ
 *	flexanet	IRQ_GPIO_GUI
 *	freebird	IRQ_GPIO_FREEBIRD_UCB1300_IRQ
 *	graphicsclient	ADS_EXT_IRQ(8)
 *	graphicsmaster	ADS_EXT_IRQ(8)
 *	lart		LART_IRQ_UCB1200
 *	omnimeter	IRQ_GPIO23
 *	pfs168		IRQ_GPIO_UCB1300_IRQ
 *	simpad		IRQ_GPIO_UCB1300_IRQ
 *	shannon		SHANNON_IRQ_GPIO_IRQ_CODEC
 *	yopy		IRQ_GPIO_UCB1200_IRQ
 */
static int ucb1x00_detect_irq(struct ucb1x00 *ucb)
{
	unsigned long mask;

	mask = probe_irq_on();
	if (!mask) {
		probe_irq_off(mask);
		return NO_IRQ;
	}

	/*
	 * Enable the ADC interrupt.
	 */
	ucb1x00_reg_write(ucb, UCB_IE_RIS, UCB_IE_ADC);
	ucb1x00_reg_write(ucb, UCB_IE_FAL, UCB_IE_ADC);
	ucb1x00_reg_write(ucb, UCB_IE_CLEAR, 0xffff);
	ucb1x00_reg_write(ucb, UCB_IE_CLEAR, 0);

	/*
	 * Cause an ADC interrupt.
	 */
	ucb1x00_reg_write(ucb, UCB_ADC_CR, UCB_ADC_ENA);
	ucb1x00_reg_write(ucb, UCB_ADC_CR, UCB_ADC_ENA | UCB_ADC_START);

	/*
	 * Wait for the conversion to complete.
	 */
	while ((ucb1x00_reg_read(ucb, UCB_ADC_DATA) & UCB_ADC_DAT_VAL) == 0);
	ucb1x00_reg_write(ucb, UCB_ADC_CR, 0);

	/*
	 * Disable and clear interrupt.
	 */
	ucb1x00_reg_write(ucb, UCB_IE_RIS, 0);
	ucb1x00_reg_write(ucb, UCB_IE_FAL, 0);
	ucb1x00_reg_write(ucb, UCB_IE_CLEAR, 0xffff);
	ucb1x00_reg_write(ucb, UCB_IE_CLEAR, 0);

	/*
	 * Read triggered interrupt.
	 */
	return probe_irq_off(mask);
}

static void ucb1x00_release(struct device *dev)
{
	struct ucb1x00 *ucb = classdev_to_ucb1x00(dev);
	kfree(ucb);
}

static struct class ucb1x00_class = {
	.name		= "ucb1x00",
	.dev_release	= ucb1x00_release,
};

static int ucb1x00_probe(struct mcp *mcp)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct ucb1x00 *ucb;
	struct ucb1x00_driver *drv;
	unsigned int id;
	int ret = -ENODEV;
	int temp;

	mcp_enable(mcp);
	id = mcp_reg_read(mcp, UCB_ID);

	if (id != UCB_ID_1200 && id != UCB_ID_1300 && id != UCB_ID_TC35143) {
		printk(KERN_WARNING "UCB1x00 ID not found: %04x\n", id);
		goto err_disable;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct ucb1x00_plat_data *pdata = mcp->attached_device.platform_data;
	struct ucb1x00_driver *drv;
	struct ucb1x00 *ucb;
	unsigned id, i, irq_base;
	int ret = -ENODEV;

	/* Tell the platform to deassert the UCB1x00 reset */
	if (pdata && pdata->reset)
		pdata->reset(UCB_RST_PROBE);

	mcp_enable(mcp);
	id = mcp_reg_read(mcp, UCB_ID);
	mcp_disable(mcp);

	if (id != UCB_ID_1200 && id != UCB_ID_1300 && id != UCB_ID_TC35143) {
		printk(KERN_WARNING "UCB1x00 ID not found: %04x\n", id);
		goto out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	ucb = kzalloc(sizeof(struct ucb1x00), GFP_KERNEL);
	ret = -ENOMEM;
	if (!ucb)
<<<<<<< HEAD
<<<<<<< HEAD
		goto err_disable;


=======
		goto out;

	device_initialize(&ucb->dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto out;

	device_initialize(&ucb->dev);
>>>>>>> refs/remotes/origin/master
	ucb->dev.class = &ucb1x00_class;
	ucb->dev.parent = &mcp->attached_device;
	dev_set_name(&ucb->dev, "ucb1x00");

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_init(&ucb->lock);
	spin_lock_init(&ucb->io_lock);
	sema_init(&ucb->adc_sem, 1);

	ucb->id  = id;
	ucb->mcp = mcp;
	ucb->irq = ucb1x00_detect_irq(ucb);
	if (ucb->irq == NO_IRQ) {
		printk(KERN_ERR "UCB1x00: IRQ probe failed\n");
		ret = -ENODEV;
		goto err_free;
	}

	ucb->gpio.base = -1;
	if (mcp->gpio_base != 0) {
		ucb->gpio.label = dev_name(&ucb->dev);
		ucb->gpio.base = mcp->gpio_base;
=======
=======
>>>>>>> refs/remotes/origin/master
	raw_spin_lock_init(&ucb->irq_lock);
	spin_lock_init(&ucb->io_lock);
	mutex_init(&ucb->adc_mutex);

	ucb->id  = id;
	ucb->mcp = mcp;

	ret = device_add(&ucb->dev);
	if (ret)
		goto err_dev_add;

	ucb1x00_enable(ucb);
	ucb->irq = ucb1x00_detect_irq(ucb);
	ucb1x00_disable(ucb);
	if (ucb->irq == NO_IRQ) {
		dev_err(&ucb->dev, "IRQ probe failed\n");
		ret = -ENODEV;
		goto err_no_irq;
	}

	ucb->gpio.base = -1;
	irq_base = pdata ? pdata->irq_base : 0;
	ucb->irq_base = irq_alloc_descs(-1, irq_base, 16, -1);
	if (ucb->irq_base < 0) {
		dev_err(&ucb->dev, "unable to allocate 16 irqs: %d\n",
			ucb->irq_base);
<<<<<<< HEAD
=======
		ret = ucb->irq_base;
>>>>>>> refs/remotes/origin/master
		goto err_irq_alloc;
	}

	for (i = 0; i < 16; i++) {
		unsigned irq = ucb->irq_base + i;

		irq_set_chip_and_handler(irq, &ucb1x00_irqchip, handle_edge_irq);
		irq_set_chip_data(irq, ucb);
		set_irq_flags(irq, IRQF_VALID | IRQ_NOREQUEST);
	}

	irq_set_irq_type(ucb->irq, IRQ_TYPE_EDGE_RISING);
	irq_set_handler_data(ucb->irq, ucb);
	irq_set_chained_handler(ucb->irq, ucb1x00_irq);

	if (pdata && pdata->gpio_base) {
		ucb->gpio.label = dev_name(&ucb->dev);
		ucb->gpio.dev = &ucb->dev;
		ucb->gpio.owner = THIS_MODULE;
		ucb->gpio.base = pdata->gpio_base;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		ucb->gpio.ngpio = 10;
		ucb->gpio.set = ucb1x00_gpio_set;
		ucb->gpio.get = ucb1x00_gpio_get;
		ucb->gpio.direction_input = ucb1x00_gpio_direction_input;
		ucb->gpio.direction_output = ucb1x00_gpio_direction_output;
<<<<<<< HEAD
<<<<<<< HEAD
		ret = gpiochip_add(&ucb->gpio);
		if (ret)
			goto err_free;
	} else
		dev_info(&ucb->dev, "gpio_base not set so no gpiolib support");

	ret = request_irq(ucb->irq, ucb1x00_irq, IRQF_TRIGGER_RISING,
			  "UCB1x00", ucb);
	if (ret) {
		printk(KERN_ERR "ucb1x00: unable to grab irq%d: %d\n",
			ucb->irq, ret);
		goto err_gpio;
	}

	mcp_set_drvdata(mcp, ucb);

	ret = device_register(&ucb->dev);
	if (ret)
		goto err_irq;


	INIT_LIST_HEAD(&ucb->devs);
	mutex_lock(&ucb1x00_mutex);
	list_add(&ucb->node, &ucb1x00_devices);
=======
=======
>>>>>>> refs/remotes/origin/master
		ucb->gpio.to_irq = ucb1x00_to_irq;
		ret = gpiochip_add(&ucb->gpio);
		if (ret)
			goto err_gpio_add;
	} else
		dev_info(&ucb->dev, "gpio_base not set so no gpiolib support");

	mcp_set_drvdata(mcp, ucb);

	if (pdata)
		device_set_wakeup_capable(&ucb->dev, pdata->can_wakeup);

	INIT_LIST_HEAD(&ucb->devs);
	mutex_lock(&ucb1x00_mutex);
	list_add_tail(&ucb->node, &ucb1x00_devices);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	list_for_each_entry(drv, &ucb1x00_drivers, node) {
		ucb1x00_add_dev(ucb, drv);
	}
	mutex_unlock(&ucb1x00_mutex);

<<<<<<< HEAD
<<<<<<< HEAD
	goto out;

 err_irq:
	free_irq(ucb->irq, ucb);
 err_gpio:
	if (ucb->gpio.base != -1)
		temp = gpiochip_remove(&ucb->gpio);
 err_free:
	kfree(ucb);
 err_disable:
	mcp_disable(mcp);
 out:
=======
=======
>>>>>>> refs/remotes/origin/master
	return ret;

 err_gpio_add:
	irq_set_chained_handler(ucb->irq, NULL);
 err_irq_alloc:
	if (ucb->irq_base > 0)
		irq_free_descs(ucb->irq_base, 16);
 err_no_irq:
	device_del(&ucb->dev);
 err_dev_add:
	put_device(&ucb->dev);
 out:
	if (pdata && pdata->reset)
		pdata->reset(UCB_RST_PROBE_FAIL);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return ret;
}

static void ucb1x00_remove(struct mcp *mcp)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct ucb1x00_plat_data *pdata = mcp->attached_device.platform_data;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ucb1x00_plat_data *pdata = mcp->attached_device.platform_data;
>>>>>>> refs/remotes/origin/master
	struct ucb1x00 *ucb = mcp_get_drvdata(mcp);
	struct list_head *l, *n;
	int ret;

	mutex_lock(&ucb1x00_mutex);
	list_del(&ucb->node);
	list_for_each_safe(l, n, &ucb->devs) {
		struct ucb1x00_dev *dev = list_entry(l, struct ucb1x00_dev, dev_node);
		ucb1x00_remove_dev(dev);
	}
	mutex_unlock(&ucb1x00_mutex);

	if (ucb->gpio.base != -1) {
		ret = gpiochip_remove(&ucb->gpio);
		if (ret)
			dev_err(&ucb->dev, "Can't remove gpio chip: %d\n", ret);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	free_irq(ucb->irq, ucb);
	device_unregister(&ucb->dev);
=======
=======
>>>>>>> refs/remotes/origin/master
	irq_set_chained_handler(ucb->irq, NULL);
	irq_free_descs(ucb->irq_base, 16);
	device_unregister(&ucb->dev);

	if (pdata && pdata->reset)
		pdata->reset(UCB_RST_REMOVE);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

int ucb1x00_register_driver(struct ucb1x00_driver *drv)
{
	struct ucb1x00 *ucb;

	INIT_LIST_HEAD(&drv->devs);
	mutex_lock(&ucb1x00_mutex);
<<<<<<< HEAD
<<<<<<< HEAD
	list_add(&drv->node, &ucb1x00_drivers);
=======
	list_add_tail(&drv->node, &ucb1x00_drivers);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	list_add_tail(&drv->node, &ucb1x00_drivers);
>>>>>>> refs/remotes/origin/master
	list_for_each_entry(ucb, &ucb1x00_devices, node) {
		ucb1x00_add_dev(ucb, drv);
	}
	mutex_unlock(&ucb1x00_mutex);
	return 0;
}

void ucb1x00_unregister_driver(struct ucb1x00_driver *drv)
{
	struct list_head *n, *l;

	mutex_lock(&ucb1x00_mutex);
	list_del(&drv->node);
	list_for_each_safe(l, n, &drv->devs) {
		struct ucb1x00_dev *dev = list_entry(l, struct ucb1x00_dev, drv_node);
		ucb1x00_remove_dev(dev);
	}
	mutex_unlock(&ucb1x00_mutex);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int ucb1x00_suspend(struct mcp *mcp, pm_message_t state)
{
	struct ucb1x00 *ucb = mcp_get_drvdata(mcp);
	struct ucb1x00_dev *dev;

	mutex_lock(&ucb1x00_mutex);
	list_for_each_entry(dev, &ucb->devs, dev_node) {
		if (dev->drv->suspend)
			dev->drv->suspend(dev, state);
	}
	mutex_unlock(&ucb1x00_mutex);
	return 0;
}

static int ucb1x00_resume(struct mcp *mcp)
{
	struct ucb1x00 *ucb = mcp_get_drvdata(mcp);
	struct ucb1x00_dev *dev;

	ucb1x00_reg_write(ucb, UCB_IO_DIR, ucb->io_dir);
	mutex_lock(&ucb1x00_mutex);
	list_for_each_entry(dev, &ucb->devs, dev_node) {
		if (dev->drv->resume)
			dev->drv->resume(dev);
=======
static int ucb1x00_suspend(struct device *dev)
{
	struct ucb1x00_plat_data *pdata = dev->platform_data;
=======
#ifdef CONFIG_PM_SLEEP
static int ucb1x00_suspend(struct device *dev)
{
	struct ucb1x00_plat_data *pdata = dev_get_platdata(dev);
>>>>>>> refs/remotes/origin/master
	struct ucb1x00 *ucb = dev_get_drvdata(dev);
	struct ucb1x00_dev *udev;

	mutex_lock(&ucb1x00_mutex);
	list_for_each_entry(udev, &ucb->devs, dev_node) {
		if (udev->drv->suspend)
			udev->drv->suspend(udev);
	}
	mutex_unlock(&ucb1x00_mutex);

	if (ucb->irq_wake) {
		unsigned long flags;

		raw_spin_lock_irqsave(&ucb->irq_lock, flags);
		ucb1x00_enable(ucb);
		ucb1x00_reg_write(ucb, UCB_IE_RIS, ucb->irq_ris_enbl &
				  ucb->irq_wake);
		ucb1x00_reg_write(ucb, UCB_IE_FAL, ucb->irq_fal_enbl &
				  ucb->irq_wake);
		ucb1x00_disable(ucb);
		raw_spin_unlock_irqrestore(&ucb->irq_lock, flags);

		enable_irq_wake(ucb->irq);
	} else if (pdata && pdata->reset)
		pdata->reset(UCB_RST_SUSPEND);

	return 0;
}

static int ucb1x00_resume(struct device *dev)
{
<<<<<<< HEAD
	struct ucb1x00_plat_data *pdata = dev->platform_data;
=======
	struct ucb1x00_plat_data *pdata = dev_get_platdata(dev);
>>>>>>> refs/remotes/origin/master
	struct ucb1x00 *ucb = dev_get_drvdata(dev);
	struct ucb1x00_dev *udev;

	if (!ucb->irq_wake && pdata && pdata->reset)
		pdata->reset(UCB_RST_RESUME);

	ucb1x00_enable(ucb);
	ucb1x00_reg_write(ucb, UCB_IO_DATA, ucb->io_out);
	ucb1x00_reg_write(ucb, UCB_IO_DIR, ucb->io_dir);

	if (ucb->irq_wake) {
		unsigned long flags;

		raw_spin_lock_irqsave(&ucb->irq_lock, flags);
		ucb1x00_reg_write(ucb, UCB_IE_RIS, ucb->irq_ris_enbl &
				  ucb->irq_mask);
		ucb1x00_reg_write(ucb, UCB_IE_FAL, ucb->irq_fal_enbl &
				  ucb->irq_mask);
		raw_spin_unlock_irqrestore(&ucb->irq_lock, flags);

		disable_irq_wake(ucb->irq);
	}
	ucb1x00_disable(ucb);

	mutex_lock(&ucb1x00_mutex);
	list_for_each_entry(udev, &ucb->devs, dev_node) {
		if (udev->drv->resume)
			udev->drv->resume(udev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	mutex_unlock(&ucb1x00_mutex);
	return 0;
}
<<<<<<< HEAD

<<<<<<< HEAD
static struct mcp_driver ucb1x00_driver = {
	.drv		= {
		.name	= "ucb1x00",
	},
	.probe		= ucb1x00_probe,
	.remove		= ucb1x00_remove,
	.suspend	= ucb1x00_suspend,
	.resume		= ucb1x00_resume,
=======
=======
#endif

>>>>>>> refs/remotes/origin/master
static const struct dev_pm_ops ucb1x00_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(ucb1x00_suspend, ucb1x00_resume)
};

static struct mcp_driver ucb1x00_driver = {
	.drv		= {
		.name	= "ucb1x00",
		.owner	= THIS_MODULE,
		.pm	= &ucb1x00_pm_ops,
	},
	.probe		= ucb1x00_probe,
	.remove		= ucb1x00_remove,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

static int __init ucb1x00_init(void)
{
	int ret = class_register(&ucb1x00_class);
	if (ret == 0) {
		ret = mcp_driver_register(&ucb1x00_driver);
		if (ret)
			class_unregister(&ucb1x00_class);
	}
	return ret;
}

static void __exit ucb1x00_exit(void)
{
	mcp_driver_unregister(&ucb1x00_driver);
	class_unregister(&ucb1x00_class);
}

module_init(ucb1x00_init);
module_exit(ucb1x00_exit);

EXPORT_SYMBOL(ucb1x00_io_set_dir);
EXPORT_SYMBOL(ucb1x00_io_write);
EXPORT_SYMBOL(ucb1x00_io_read);

EXPORT_SYMBOL(ucb1x00_adc_enable);
EXPORT_SYMBOL(ucb1x00_adc_read);
EXPORT_SYMBOL(ucb1x00_adc_disable);

<<<<<<< HEAD
<<<<<<< HEAD
EXPORT_SYMBOL(ucb1x00_hook_irq);
EXPORT_SYMBOL(ucb1x00_free_irq);
EXPORT_SYMBOL(ucb1x00_enable_irq);
EXPORT_SYMBOL(ucb1x00_disable_irq);

EXPORT_SYMBOL(ucb1x00_register_driver);
EXPORT_SYMBOL(ucb1x00_unregister_driver);

=======
=======
>>>>>>> refs/remotes/origin/master
EXPORT_SYMBOL(ucb1x00_register_driver);
EXPORT_SYMBOL(ucb1x00_unregister_driver);

MODULE_ALIAS("mcp:ucb1x00");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
MODULE_AUTHOR("Russell King <rmk@arm.linux.org.uk>");
MODULE_DESCRIPTION("UCB1x00 core driver");
MODULE_LICENSE("GPL");
