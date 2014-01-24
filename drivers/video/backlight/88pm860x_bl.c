/*
 * Backlight driver for Marvell Semiconductor 88PM8606
 *
 * Copyright (C) 2009 Marvell International Ltd.
 *	Haojian Zhuang <haojian.zhuang@marvell.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/kernel.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> refs/remotes/origin/master
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/fb.h>
#include <linux/i2c.h>
#include <linux/backlight.h>
#include <linux/mfd/88pm860x.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master

#define MAX_BRIGHTNESS		(0xFF)
#define MIN_BRIGHTNESS		(0)

#define CURRENT_BITMASK		(0x1F << 1)

struct pm860x_backlight_data {
	struct pm860x_chip *chip;
	struct i2c_client *i2c;
	int	current_brightness;
	int	port;
	int	pwm;
	int	iset;
<<<<<<< HEAD
};

static inline int wled_a(int port)
{
	int ret;

	ret = ((port - PM8606_BACKLIGHT1) << 1) + 2;
	return ret;
}

static inline int wled_b(int port)
{
	int ret;

	ret = ((port - PM8606_BACKLIGHT1) << 1) + 3;
	return ret;
}

/* WLED2 & WLED3 share the same IDC */
static inline int wled_idc(int port)
{
	int ret;

	switch (port) {
	case PM8606_BACKLIGHT1:
	case PM8606_BACKLIGHT2:
		ret = ((port - PM8606_BACKLIGHT1) << 1) + 3;
		break;
	case PM8606_BACKLIGHT3:
	default:
		ret = ((port - PM8606_BACKLIGHT2) << 1) + 3;
		break;
	}
	return ret;
}

<<<<<<< HEAD
=======
=======
	int	reg_duty_cycle;
	int	reg_always_on;
	int	reg_current;
};

>>>>>>> refs/remotes/origin/master
static int backlight_power_set(struct pm860x_chip *chip, int port,
		int on)
{
	int ret = -EINVAL;

	switch (port) {
<<<<<<< HEAD
	case PM8606_BACKLIGHT1:
		ret = on ? pm8606_osc_enable(chip, WLED1_DUTY) :
			pm8606_osc_disable(chip, WLED1_DUTY);
		break;
	case PM8606_BACKLIGHT2:
		ret = on ? pm8606_osc_enable(chip, WLED2_DUTY) :
			pm8606_osc_disable(chip, WLED2_DUTY);
		break;
	case PM8606_BACKLIGHT3:
=======
	case 0:
		ret = on ? pm8606_osc_enable(chip, WLED1_DUTY) :
			pm8606_osc_disable(chip, WLED1_DUTY);
		break;
	case 1:
		ret = on ? pm8606_osc_enable(chip, WLED2_DUTY) :
			pm8606_osc_disable(chip, WLED2_DUTY);
		break;
	case 2:
>>>>>>> refs/remotes/origin/master
		ret = on ? pm8606_osc_enable(chip, WLED3_DUTY) :
			pm8606_osc_disable(chip, WLED3_DUTY);
		break;
	}
	return ret;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int pm860x_backlight_set(struct backlight_device *bl, int brightness)
{
	struct pm860x_backlight_data *data = bl_get_data(bl);
	struct pm860x_chip *chip = data->chip;
	unsigned char value;
	int ret;

	if (brightness > MAX_BRIGHTNESS)
		value = MAX_BRIGHTNESS;
	else
		value = brightness;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (brightness)
		backlight_power_set(chip, data->port, 1);

>>>>>>> refs/remotes/origin/cm-10.0
	ret = pm860x_reg_write(data->i2c, wled_a(data->port), value);
=======
	if (brightness)
		backlight_power_set(chip, data->port, 1);

	ret = pm860x_reg_write(data->i2c, data->reg_duty_cycle, value);
>>>>>>> refs/remotes/origin/master
	if (ret < 0)
		goto out;

	if ((data->current_brightness == 0) && brightness) {
		if (data->iset) {
<<<<<<< HEAD
			ret = pm860x_set_bits(data->i2c, wled_idc(data->port),
=======
			ret = pm860x_set_bits(data->i2c, data->reg_current,
>>>>>>> refs/remotes/origin/master
					      CURRENT_BITMASK, data->iset);
			if (ret < 0)
				goto out;
		}
		if (data->pwm) {
			ret = pm860x_set_bits(data->i2c, PM8606_PWM,
					      PM8606_PWM_FREQ_MASK, data->pwm);
			if (ret < 0)
				goto out;
		}
		if (brightness == MAX_BRIGHTNESS) {
			/* set WLED_ON bit as 100% */
<<<<<<< HEAD
			ret = pm860x_set_bits(data->i2c, wled_b(data->port),
=======
			ret = pm860x_set_bits(data->i2c, data->reg_always_on,
>>>>>>> refs/remotes/origin/master
					      PM8606_WLED_ON, PM8606_WLED_ON);
		}
	} else {
		if (brightness == MAX_BRIGHTNESS) {
			/* set WLED_ON bit as 100% */
<<<<<<< HEAD
			ret = pm860x_set_bits(data->i2c, wled_b(data->port),
					      PM8606_WLED_ON, PM8606_WLED_ON);
		} else {
			/* clear WLED_ON bit since it's not 100% */
			ret = pm860x_set_bits(data->i2c, wled_b(data->port),
=======
			ret = pm860x_set_bits(data->i2c, data->reg_always_on,
					      PM8606_WLED_ON, PM8606_WLED_ON);
		} else {
			/* clear WLED_ON bit since it's not 100% */
			ret = pm860x_set_bits(data->i2c, data->reg_always_on,
>>>>>>> refs/remotes/origin/master
					      PM8606_WLED_ON, 0);
		}
	}
	if (ret < 0)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (brightness == 0)
		backlight_power_set(chip, data->port, 0);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (brightness == 0)
		backlight_power_set(chip, data->port, 0);

>>>>>>> refs/remotes/origin/master
	dev_dbg(chip->dev, "set brightness %d\n", value);
	data->current_brightness = value;
	return 0;
out:
<<<<<<< HEAD
	dev_dbg(chip->dev, "set brightness %d failure with return "
		"value:%d\n", value, ret);
=======
	dev_dbg(chip->dev, "set brightness %d failure with return value: %d\n",
		value, ret);
>>>>>>> refs/remotes/origin/master
	return ret;
}

static int pm860x_backlight_update_status(struct backlight_device *bl)
{
	int brightness = bl->props.brightness;

	if (bl->props.power != FB_BLANK_UNBLANK)
		brightness = 0;

	if (bl->props.fb_blank != FB_BLANK_UNBLANK)
		brightness = 0;

	if (bl->props.state & BL_CORE_SUSPENDED)
		brightness = 0;

	return pm860x_backlight_set(bl, brightness);
}

static int pm860x_backlight_get_brightness(struct backlight_device *bl)
{
	struct pm860x_backlight_data *data = bl_get_data(bl);
	struct pm860x_chip *chip = data->chip;
	int ret;

<<<<<<< HEAD
	ret = pm860x_reg_read(data->i2c, wled_a(data->port));
=======
	ret = pm860x_reg_read(data->i2c, data->reg_duty_cycle);
>>>>>>> refs/remotes/origin/master
	if (ret < 0)
		goto out;
	data->current_brightness = ret;
	dev_dbg(chip->dev, "get brightness %d\n", data->current_brightness);
	return data->current_brightness;
out:
	return -EINVAL;
}

static const struct backlight_ops pm860x_backlight_ops = {
	.options	= BL_CORE_SUSPENDRESUME,
	.update_status	= pm860x_backlight_update_status,
	.get_brightness	= pm860x_backlight_get_brightness,
};

<<<<<<< HEAD
static int pm860x_backlight_probe(struct platform_device *pdev)
{
	struct pm860x_chip *chip = dev_get_drvdata(pdev->dev.parent);
	struct pm860x_backlight_pdata *pdata = NULL;
=======
#ifdef CONFIG_OF
static int pm860x_backlight_dt_init(struct platform_device *pdev,
				    struct pm860x_backlight_data *data,
				    char *name)
{
	struct device_node *nproot, *np;
	int iset = 0;

	nproot = of_node_get(pdev->dev.parent->of_node);
	if (!nproot)
		return -ENODEV;
	nproot = of_find_node_by_name(nproot, "backlights");
	if (!nproot) {
		dev_err(&pdev->dev, "failed to find backlights node\n");
		return -ENODEV;
	}
	for_each_child_of_node(nproot, np) {
		if (!of_node_cmp(np->name, name)) {
			of_property_read_u32(np, "marvell,88pm860x-iset",
					     &iset);
			data->iset = PM8606_WLED_CURRENT(iset);
			of_property_read_u32(np, "marvell,88pm860x-pwm",
					     &data->pwm);
			break;
		}
	}
	of_node_put(nproot);
	return 0;
}
#else
#define pm860x_backlight_dt_init(x, y, z)	(-1)
#endif

static int pm860x_backlight_probe(struct platform_device *pdev)
{
	struct pm860x_chip *chip = dev_get_drvdata(pdev->dev.parent);
	struct pm860x_backlight_pdata *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> refs/remotes/origin/master
	struct pm860x_backlight_data *data;
	struct backlight_device *bl;
	struct resource *res;
	struct backlight_properties props;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned char value;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	char name[MFD_NAME_SIZE];
	int ret;

	res = platform_get_resource(pdev, IORESOURCE_IO, 0);
	if (res == NULL) {
		dev_err(&pdev->dev, "No I/O resource!\n");
		return -EINVAL;
	}

	pdata = pdev->dev.platform_data;
	if (pdata == NULL) {
		dev_err(&pdev->dev, "platform data isn't assigned to "
			"backlight\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	data = kzalloc(sizeof(struct pm860x_backlight_data), GFP_KERNEL);
=======
	data = devm_kzalloc(&pdev->dev, sizeof(struct pm860x_backlight_data),
			    GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
	if (data == NULL)
		return -ENOMEM;
	strncpy(name, res->name, MFD_NAME_SIZE);
	data->chip = chip;
	data->i2c = (chip->id == CHIP_PM8606) ? chip->client	\
			: chip->companion;
	data->current_brightness = MAX_BRIGHTNESS;
	data->pwm = pdata->pwm;
	data->iset = pdata->iset;
	data->port = pdata->flags;
	if (data->port < 0) {
		dev_err(&pdev->dev, "wrong platform data is assigned");
<<<<<<< HEAD
		kfree(data);
=======
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;
=======
	char name[MFD_NAME_SIZE];
	int ret = 0;

	data = devm_kzalloc(&pdev->dev, sizeof(struct pm860x_backlight_data),
			    GFP_KERNEL);
	if (data == NULL)
		return -ENOMEM;
	res = platform_get_resource_byname(pdev, IORESOURCE_REG, "duty cycle");
	if (!res) {
		dev_err(&pdev->dev, "No REG resource for duty cycle\n");
		return -ENXIO;
	}
	data->reg_duty_cycle = res->start;
	res = platform_get_resource_byname(pdev, IORESOURCE_REG, "always on");
	if (!res) {
		dev_err(&pdev->dev, "No REG resorce for always on\n");
		return -ENXIO;
	}
	data->reg_always_on = res->start;
	res = platform_get_resource_byname(pdev, IORESOURCE_REG, "current");
	if (!res) {
		dev_err(&pdev->dev, "No REG resource for current\n");
		return -ENXIO;
	}
	data->reg_current = res->start;

	memset(name, 0, MFD_NAME_SIZE);
	sprintf(name, "backlight-%d", pdev->id);
	data->port = pdev->id;
	data->chip = chip;
	data->i2c = (chip->id == CHIP_PM8606) ? chip->client : chip->companion;
	data->current_brightness = MAX_BRIGHTNESS;
	if (pm860x_backlight_dt_init(pdev, data, name)) {
		if (pdata) {
			data->pwm = pdata->pwm;
			data->iset = pdata->iset;
		}
>>>>>>> refs/remotes/origin/master
	}

	memset(&props, 0, sizeof(struct backlight_properties));
	props.type = BACKLIGHT_RAW;
	props.max_brightness = MAX_BRIGHTNESS;
<<<<<<< HEAD
	bl = backlight_device_register(name, &pdev->dev, data,
					&pm860x_backlight_ops, &props);
	if (IS_ERR(bl)) {
		dev_err(&pdev->dev, "failed to register backlight\n");
<<<<<<< HEAD
		kfree(data);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bl = devm_backlight_device_register(&pdev->dev, name, &pdev->dev, data,
					&pm860x_backlight_ops, &props);
	if (IS_ERR(bl)) {
		dev_err(&pdev->dev, "failed to register backlight\n");
>>>>>>> refs/remotes/origin/master
		return PTR_ERR(bl);
	}
	bl->props.brightness = MAX_BRIGHTNESS;

	platform_set_drvdata(pdev, bl);

<<<<<<< HEAD
<<<<<<< HEAD
	/* Enable reference VSYS */
	ret = pm860x_reg_read(data->i2c, PM8606_VSYS);
	if (ret < 0)
		goto out;
	if ((ret & PM8606_VSYS_EN) == 0) {
		value = ret | PM8606_VSYS_EN;
		ret = pm860x_reg_write(data->i2c, PM8606_VSYS, value);
		if (ret < 0)
			goto out;
	}
	/* Enable reference OSC */
	ret = pm860x_reg_read(data->i2c, PM8606_MISC);
	if (ret < 0)
		goto out;
	if ((ret & PM8606_MISC_OSC_EN) == 0) {
		value = ret | PM8606_MISC_OSC_EN;
		ret = pm860x_reg_write(data->i2c, PM8606_MISC, value);
		if (ret < 0)
			goto out;
	}
=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* read current backlight */
	ret = pm860x_backlight_get_brightness(bl);
	if (ret < 0)
		goto out;

	backlight_update_status(bl);
	return 0;
out:
	backlight_device_unregister(bl);
<<<<<<< HEAD
	kfree(data);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static int pm860x_backlight_remove(struct platform_device *pdev)
{
	struct backlight_device *bl = platform_get_drvdata(pdev);
<<<<<<< HEAD
	struct pm860x_backlight_data *data = bl_get_data(bl);

	backlight_device_unregister(bl);
	kfree(data);
=======

	backlight_device_unregister(bl);
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
=======
	/* read current backlight */
	ret = pm860x_backlight_get_brightness(bl);
	if (ret < 0)
		return ret;

	backlight_update_status(bl);
	return 0;
>>>>>>> refs/remotes/origin/master
}

static struct platform_driver pm860x_backlight_driver = {
	.driver		= {
		.name	= "88pm860x-backlight",
		.owner	= THIS_MODULE,
	},
	.probe		= pm860x_backlight_probe,
<<<<<<< HEAD
	.remove		= pm860x_backlight_remove,
};

<<<<<<< HEAD
static int __init pm860x_backlight_init(void)
{
	return platform_driver_register(&pm860x_backlight_driver);
}
module_init(pm860x_backlight_init);

static void __exit pm860x_backlight_exit(void)
{
	platform_driver_unregister(&pm860x_backlight_driver);
}
module_exit(pm860x_backlight_exit);
=======
module_platform_driver(pm860x_backlight_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
};

module_platform_driver(pm860x_backlight_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("Backlight Driver for Marvell Semiconductor 88PM8606");
MODULE_AUTHOR("Haojian Zhuang <haojian.zhuang@marvell.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:88pm860x-backlight");
