/*
 *  Amstrad E3 (Delta) keyboard port driver
 *
 *  Copyright (c) 2006 Matt Callow
 *  Copyright (c) 2010 Janusz Krzysztofik
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * Thanks to Cliff Lawson for his help
 *
 * The Amstrad Delta keyboard (aka mailboard) uses normal PC-AT style serial
 * transmission.  The keyboard port is formed of two GPIO lines, for clock
 * and data.  Due to strict timing requirements of the interface,
 * the serial data stream is read and processed by a FIQ handler.
 * The resulting words are fetched by this driver from a circular buffer.
 *
 * Standard AT keyboard driver (atkbd) is used for handling the keyboard data.
 * However, when used with the E3 mailboard that producecs non-standard
 * scancodes, a custom key table must be prepared and loaded from userspace.
 */
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/serio.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <asm/mach-types.h>
#include <plat/board-ams-delta.h>
=======
#include <linux/module.h>

#include <asm/mach-types.h>
#include <mach/board-ams-delta.h>
>>>>>>> refs/remotes/origin/master

#include <mach/ams-delta-fiq.h>

MODULE_AUTHOR("Matt Callow");
MODULE_DESCRIPTION("AMS Delta (E3) keyboard port driver");
MODULE_LICENSE("GPL");

static struct serio *ams_delta_serio;

static int check_data(int data)
{
	int i, parity = 0;

	/* check valid stop bit */
	if (!(data & 0x400)) {
		dev_warn(&ams_delta_serio->dev,
				"invalid stop bit, data=0x%X\n",
				data);
		return SERIO_FRAME;
	}
	/* calculate the parity */
	for (i = 1; i < 10; i++) {
		if (data & (1 << i))
			parity++;
	}
	/* it should be odd */
	if (!(parity & 0x01)) {
		dev_warn(&ams_delta_serio->dev,
				"paritiy check failed, data=0x%X parity=0x%X\n",
				data, parity);
		return SERIO_PARITY;
	}
	return 0;
}

static irqreturn_t ams_delta_serio_interrupt(int irq, void *dev_id)
{
	int *circ_buff = &fiq_buffer[FIQ_CIRC_BUFF];
	int data, dfl;
	u8 scancode;

	fiq_buffer[FIQ_IRQ_PEND] = 0;

	/*
	 * Read data from the circular buffer, check it
	 * and then pass it on the serio
	 */
	while (fiq_buffer[FIQ_KEYS_CNT] > 0) {

		data = circ_buff[fiq_buffer[FIQ_HEAD_OFFSET]++];
		fiq_buffer[FIQ_KEYS_CNT]--;
		if (fiq_buffer[FIQ_HEAD_OFFSET] == fiq_buffer[FIQ_BUF_LEN])
			fiq_buffer[FIQ_HEAD_OFFSET] = 0;

		dfl = check_data(data);
		scancode = (u8) (data >> 1) & 0xFF;
		serio_interrupt(ams_delta_serio, scancode, dfl);
	}
	return IRQ_HANDLED;
}

static int ams_delta_serio_open(struct serio *serio)
{
	/* enable keyboard */
<<<<<<< HEAD
<<<<<<< HEAD
	ams_delta_latch2_write(AMD_DELTA_LATCH2_KEYBRD_PWR,
			AMD_DELTA_LATCH2_KEYBRD_PWR);
=======
	gpio_set_value(AMS_DELTA_GPIO_PIN_KEYBRD_PWR, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	gpio_set_value(AMS_DELTA_GPIO_PIN_KEYBRD_PWR, 1);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static void ams_delta_serio_close(struct serio *serio)
{
	/* disable keyboard */
<<<<<<< HEAD
<<<<<<< HEAD
	ams_delta_latch2_write(AMD_DELTA_LATCH2_KEYBRD_PWR, 0);
}

=======
=======
>>>>>>> refs/remotes/origin/master
	gpio_set_value(AMS_DELTA_GPIO_PIN_KEYBRD_PWR, 0);
}

static const struct gpio ams_delta_gpios[] __initconst_or_module = {
	{
		.gpio	= AMS_DELTA_GPIO_PIN_KEYBRD_DATA,
		.flags	= GPIOF_DIR_IN,
		.label	= "serio-data",
	},
	{
		.gpio	= AMS_DELTA_GPIO_PIN_KEYBRD_CLK,
		.flags	= GPIOF_DIR_IN,
		.label	= "serio-clock",
	},
	{
		.gpio	= AMS_DELTA_GPIO_PIN_KEYBRD_PWR,
		.flags	= GPIOF_OUT_INIT_LOW,
		.label	= "serio-power",
	},
	{
		.gpio	= AMS_DELTA_GPIO_PIN_KEYBRD_DATAOUT,
		.flags	= GPIOF_OUT_INIT_LOW,
		.label	= "serio-dataout",
	},
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int __init ams_delta_serio_init(void)
{
	int err;

	if (!machine_is_ams_delta())
		return -ENODEV;

	ams_delta_serio = kzalloc(sizeof(struct serio), GFP_KERNEL);
	if (!ams_delta_serio)
		return -ENOMEM;

	ams_delta_serio->id.type = SERIO_8042;
	ams_delta_serio->open = ams_delta_serio_open;
	ams_delta_serio->close = ams_delta_serio_close;
	strlcpy(ams_delta_serio->name, "AMS DELTA keyboard adapter",
			sizeof(ams_delta_serio->name));
	strlcpy(ams_delta_serio->phys, "GPIO/serio0",
			sizeof(ams_delta_serio->phys));

<<<<<<< HEAD
<<<<<<< HEAD
	err = gpio_request(AMS_DELTA_GPIO_PIN_KEYBRD_DATA, "serio-data");
	if (err) {
		pr_err("ams_delta_serio: Couldn't request gpio pin for data\n");
		goto serio;
	}
	gpio_direction_input(AMS_DELTA_GPIO_PIN_KEYBRD_DATA);

	err = gpio_request(AMS_DELTA_GPIO_PIN_KEYBRD_CLK, "serio-clock");
	if (err) {
		pr_err("ams_delta_serio: couldn't request gpio pin for clock\n");
		goto gpio_data;
	}
	gpio_direction_input(AMS_DELTA_GPIO_PIN_KEYBRD_CLK);
=======
=======
>>>>>>> refs/remotes/origin/master
	err = gpio_request_array(ams_delta_gpios,
				ARRAY_SIZE(ams_delta_gpios));
	if (err) {
		pr_err("ams_delta_serio: Couldn't request gpio pins\n");
		goto serio;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	err = request_irq(gpio_to_irq(AMS_DELTA_GPIO_PIN_KEYBRD_CLK),
			ams_delta_serio_interrupt, IRQ_TYPE_EDGE_RISING,
			"ams-delta-serio", 0);
	if (err < 0) {
		pr_err("ams_delta_serio: couldn't request gpio interrupt %d\n",
				gpio_to_irq(AMS_DELTA_GPIO_PIN_KEYBRD_CLK));
<<<<<<< HEAD
<<<<<<< HEAD
		goto gpio_clk;
=======
		goto gpio;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto gpio;
>>>>>>> refs/remotes/origin/master
	}
	/*
	 * Since GPIO register handling for keyboard clock pin is performed
	 * at FIQ level, switch back from edge to simple interrupt handler
	 * to avoid bad interaction.
	 */
	irq_set_handler(gpio_to_irq(AMS_DELTA_GPIO_PIN_KEYBRD_CLK),
			handle_simple_irq);

	serio_register_port(ams_delta_serio);
	dev_info(&ams_delta_serio->dev, "%s\n", ams_delta_serio->name);

	return 0;
<<<<<<< HEAD
<<<<<<< HEAD
gpio_clk:
	gpio_free(AMS_DELTA_GPIO_PIN_KEYBRD_CLK);
gpio_data:
	gpio_free(AMS_DELTA_GPIO_PIN_KEYBRD_DATA);
=======
gpio:
	gpio_free_array(ams_delta_gpios,
			ARRAY_SIZE(ams_delta_gpios));
>>>>>>> refs/remotes/origin/cm-10.0
=======
gpio:
	gpio_free_array(ams_delta_gpios,
			ARRAY_SIZE(ams_delta_gpios));
>>>>>>> refs/remotes/origin/master
serio:
	kfree(ams_delta_serio);
	return err;
}
module_init(ams_delta_serio_init);

static void __exit ams_delta_serio_exit(void)
{
	serio_unregister_port(ams_delta_serio);
<<<<<<< HEAD
<<<<<<< HEAD
	free_irq(OMAP_GPIO_IRQ(AMS_DELTA_GPIO_PIN_KEYBRD_CLK), 0);
	gpio_free(AMS_DELTA_GPIO_PIN_KEYBRD_CLK);
	gpio_free(AMS_DELTA_GPIO_PIN_KEYBRD_DATA);
=======
	free_irq(gpio_to_irq(AMS_DELTA_GPIO_PIN_KEYBRD_CLK), 0);
	gpio_free_array(ams_delta_gpios,
			ARRAY_SIZE(ams_delta_gpios));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	free_irq(gpio_to_irq(AMS_DELTA_GPIO_PIN_KEYBRD_CLK), 0);
	gpio_free_array(ams_delta_gpios,
			ARRAY_SIZE(ams_delta_gpios));
>>>>>>> refs/remotes/origin/master
}
module_exit(ams_delta_serio_exit);
