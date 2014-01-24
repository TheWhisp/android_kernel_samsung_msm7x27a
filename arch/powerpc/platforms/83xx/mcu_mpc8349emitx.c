/*
 * Power Management and GPIO expander driver for MPC8349E-mITX-compatible MCU
 *
 * Copyright (c) 2008  MontaVista Software, Inc.
 *
 * Author: Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/kthread.h>
#include <linux/reboot.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/kthread.h>
#include <linux/reboot.h>
>>>>>>> refs/remotes/origin/master
#include <asm/prom.h>
#include <asm/machdep.h>

/*
 * I don't have specifications for the MCU firmware, I found this register
 * and bits positions by the trial&error method.
 */
#define MCU_REG_CTRL	0x20
#define MCU_CTRL_POFF	0x40
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define MCU_CTRL_BTN	0x80
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define MCU_CTRL_BTN	0x80
>>>>>>> refs/remotes/origin/master

#define MCU_NUM_GPIO	2

struct mcu {
	struct mutex lock;
	struct i2c_client *client;
	struct gpio_chip gc;
	u8 reg_ctrl;
};

static struct mcu *glob_mcu;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
struct task_struct *shutdown_thread;
static int shutdown_thread_fn(void *data)
{
	int ret;
	struct mcu *mcu = glob_mcu;

	while (!kthread_should_stop()) {
		ret = i2c_smbus_read_byte_data(mcu->client, MCU_REG_CTRL);
		if (ret < 0)
			pr_err("MCU status reg read failed.\n");
		mcu->reg_ctrl = ret;


		if (mcu->reg_ctrl & MCU_CTRL_BTN) {
			i2c_smbus_write_byte_data(mcu->client, MCU_REG_CTRL,
						  mcu->reg_ctrl & ~MCU_CTRL_BTN);

			ctrl_alt_del();
		}

		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(HZ);
	}

	return 0;
}

static ssize_t show_status(struct device *d,
			   struct device_attribute *attr, char *buf)
{
	int ret;
	struct mcu *mcu = glob_mcu;

	ret = i2c_smbus_read_byte_data(mcu->client, MCU_REG_CTRL);
	if (ret < 0)
		return -ENODEV;
	mcu->reg_ctrl = ret;

	return sprintf(buf, "%02x\n", ret);
}
static DEVICE_ATTR(status, S_IRUGO, show_status, NULL);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void mcu_power_off(void)
{
	struct mcu *mcu = glob_mcu;

	pr_info("Sending power-off request to the MCU...\n");
	mutex_lock(&mcu->lock);
<<<<<<< HEAD
<<<<<<< HEAD
	i2c_smbus_write_byte_data(glob_mcu->client, MCU_REG_CTRL,
=======
	i2c_smbus_write_byte_data(mcu->client, MCU_REG_CTRL,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	i2c_smbus_write_byte_data(mcu->client, MCU_REG_CTRL,
>>>>>>> refs/remotes/origin/master
				  mcu->reg_ctrl | MCU_CTRL_POFF);
	mutex_unlock(&mcu->lock);
}

static void mcu_gpio_set(struct gpio_chip *gc, unsigned int gpio, int val)
{
	struct mcu *mcu = container_of(gc, struct mcu, gc);
	u8 bit = 1 << (4 + gpio);

	mutex_lock(&mcu->lock);
	if (val)
		mcu->reg_ctrl &= ~bit;
	else
		mcu->reg_ctrl |= bit;

	i2c_smbus_write_byte_data(mcu->client, MCU_REG_CTRL, mcu->reg_ctrl);
	mutex_unlock(&mcu->lock);
}

static int mcu_gpio_dir_out(struct gpio_chip *gc, unsigned int gpio, int val)
{
	mcu_gpio_set(gc, gpio, val);
	return 0;
}

static int mcu_gpiochip_add(struct mcu *mcu)
{
	struct device_node *np;
	struct gpio_chip *gc = &mcu->gc;

	np = of_find_compatible_node(NULL, NULL, "fsl,mcu-mpc8349emitx");
	if (!np)
		return -ENODEV;

	gc->owner = THIS_MODULE;
	gc->label = np->full_name;
	gc->can_sleep = 1;
	gc->ngpio = MCU_NUM_GPIO;
	gc->base = -1;
	gc->set = mcu_gpio_set;
	gc->direction_output = mcu_gpio_dir_out;
	gc->of_node = np;

	return gpiochip_add(gc);
}

static int mcu_gpiochip_remove(struct mcu *mcu)
{
	return gpiochip_remove(&mcu->gc);
}

<<<<<<< HEAD
static int __devinit mcu_probe(struct i2c_client *client,
			       const struct i2c_device_id *id)
=======
static int mcu_probe(struct i2c_client *client, const struct i2c_device_id *id)
>>>>>>> refs/remotes/origin/master
{
	struct mcu *mcu;
	int ret;

	mcu = kzalloc(sizeof(*mcu), GFP_KERNEL);
	if (!mcu)
		return -ENOMEM;

	mutex_init(&mcu->lock);
	mcu->client = client;
	i2c_set_clientdata(client, mcu);

	ret = i2c_smbus_read_byte_data(mcu->client, MCU_REG_CTRL);
	if (ret < 0)
		goto err;
	mcu->reg_ctrl = ret;

	ret = mcu_gpiochip_add(mcu);
	if (ret)
		goto err;

	/* XXX: this is potentially racy, but there is no lock for ppc_md */
	if (!ppc_md.power_off) {
		glob_mcu = mcu;
		ppc_md.power_off = mcu_power_off;
		dev_info(&client->dev, "will provide power-off service\n");
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	if (device_create_file(&client->dev, &dev_attr_status))
		dev_err(&client->dev,
			"couldn't create device file for status\n");

	shutdown_thread = kthread_run(shutdown_thread_fn, NULL,
				      "mcu-i2c-shdn");

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
err:
	kfree(mcu);
	return ret;
}

<<<<<<< HEAD
static int __devexit mcu_remove(struct i2c_client *client)
=======
static int mcu_remove(struct i2c_client *client)
>>>>>>> refs/remotes/origin/master
{
	struct mcu *mcu = i2c_get_clientdata(client);
	int ret;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	kthread_stop(shutdown_thread);

	device_remove_file(&client->dev, &dev_attr_status);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (glob_mcu == mcu) {
		ppc_md.power_off = NULL;
		glob_mcu = NULL;
	}

	ret = mcu_gpiochip_remove(mcu);
	if (ret)
		return ret;
<<<<<<< HEAD
	i2c_set_clientdata(client, NULL);
=======
>>>>>>> refs/remotes/origin/master
	kfree(mcu);
	return 0;
}

static const struct i2c_device_id mcu_ids[] = {
	{ "mcu-mpc8349emitx", },
	{},
};
MODULE_DEVICE_TABLE(i2c, mcu_ids);

<<<<<<< HEAD
static struct of_device_id mcu_of_match_table[] __devinitdata = {
=======
static struct of_device_id mcu_of_match_table[] = {
>>>>>>> refs/remotes/origin/master
	{ .compatible = "fsl,mcu-mpc8349emitx", },
	{ },
};

static struct i2c_driver mcu_driver = {
	.driver = {
		.name = "mcu-mpc8349emitx",
		.owner = THIS_MODULE,
		.of_match_table = mcu_of_match_table,
	},
	.probe = mcu_probe,
<<<<<<< HEAD
	.remove	= __devexit_p(mcu_remove),
	.id_table = mcu_ids,
};

static int __init mcu_init(void)
{
	return i2c_add_driver(&mcu_driver);
}
module_init(mcu_init);

static void __exit mcu_exit(void)
{
	i2c_del_driver(&mcu_driver);
}
module_exit(mcu_exit);
=======
	.remove	= mcu_remove,
	.id_table = mcu_ids,
};

module_i2c_driver(mcu_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("Power Management and GPIO expander driver for "
		   "MPC8349E-mITX-compatible MCU");
MODULE_AUTHOR("Anton Vorontsov <avorontsov@ru.mvista.com>");
MODULE_LICENSE("GPL");
