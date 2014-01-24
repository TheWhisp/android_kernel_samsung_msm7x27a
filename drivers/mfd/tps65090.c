/*
 * Core driver for TI TPS65090 PMIC family
 *
 * Copyright (c) 2012, NVIDIA CORPORATION.  All rights reserved.

 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.

 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/mfd/core.h>
#include <linux/mfd/tps65090.h>
<<<<<<< HEAD
#include <linux/regmap.h>
=======
#include <linux/of.h>
#include <linux/of_device.h>
>>>>>>> refs/remotes/origin/master
#include <linux/err.h>

#define NUM_INT_REG 2
#define TOTAL_NUM_REG 0x18

/* interrupt status registers */
#define TPS65090_INT_STS	0x0
#define TPS65090_INT_STS2	0x1

/* interrupt mask registers */
#define TPS65090_INT_MSK	0x2
#define TPS65090_INT_MSK2	0x3

<<<<<<< HEAD
struct tps65090_irq_data {
	u8		mask_reg;
	u8		mask_pos;
};

#define TPS65090_IRQ(_reg, _mask_pos)		\
	{					\
		.mask_reg	= (_reg),	\
		.mask_pos	= (_mask_pos),	\
	}

static const struct tps65090_irq_data tps65090_irqs[] = {
	[0]		= TPS65090_IRQ(0, 0),
	[1]		= TPS65090_IRQ(0, 1),
	[2]		= TPS65090_IRQ(0, 2),
	[3]		= TPS65090_IRQ(0, 3),
	[4]		= TPS65090_IRQ(0, 4),
	[5]		= TPS65090_IRQ(0, 5),
	[6]		= TPS65090_IRQ(0, 6),
	[7]		= TPS65090_IRQ(0, 7),
	[8]		= TPS65090_IRQ(1, 0),
	[9]		= TPS65090_IRQ(1, 1),
	[10]		= TPS65090_IRQ(1, 2),
	[11]		= TPS65090_IRQ(1, 3),
	[12]		= TPS65090_IRQ(1, 4),
	[13]		= TPS65090_IRQ(1, 5),
	[14]		= TPS65090_IRQ(1, 6),
	[15]		= TPS65090_IRQ(1, 7),
};

static struct mfd_cell tps65090s[] = {
	{
		.name = "tps65910-pmic",
	},
	{
		.name = "tps65910-regulator",
	},
};

struct tps65090 {
	struct mutex		lock;
	struct device		*dev;
	struct i2c_client	*client;
	struct regmap		*rmap;
	struct irq_chip		irq_chip;
	struct mutex		irq_lock;
	int			irq_base;
	unsigned int		id;
};

int tps65090_write(struct device *dev, int reg, uint8_t val)
{
	struct tps65090 *tps = dev_get_drvdata(dev);
	return regmap_write(tps->rmap, reg, val);
}
EXPORT_SYMBOL_GPL(tps65090_write);

int tps65090_read(struct device *dev, int reg, uint8_t *val)
{
	struct tps65090 *tps = dev_get_drvdata(dev);
	unsigned int temp_val;
	int ret;
	ret = regmap_read(tps->rmap, reg, &temp_val);
	if (!ret)
		*val = temp_val;
	return ret;
}
EXPORT_SYMBOL_GPL(tps65090_read);

int tps65090_set_bits(struct device *dev, int reg, uint8_t bit_num)
{
	struct tps65090 *tps = dev_get_drvdata(dev);
	return regmap_update_bits(tps->rmap, reg, BIT(bit_num), ~0u);
}
EXPORT_SYMBOL_GPL(tps65090_set_bits);

int tps65090_clr_bits(struct device *dev, int reg, uint8_t bit_num)
{
	struct tps65090 *tps = dev_get_drvdata(dev);
	return regmap_update_bits(tps->rmap, reg, BIT(bit_num), 0u);
}
EXPORT_SYMBOL_GPL(tps65090_clr_bits);

static void tps65090_irq_lock(struct irq_data *data)
{
	struct tps65090 *tps65090 = irq_data_get_irq_chip_data(data);

	mutex_lock(&tps65090->irq_lock);
}

static void tps65090_irq_mask(struct irq_data *irq_data)
{
	struct tps65090 *tps65090 = irq_data_get_irq_chip_data(irq_data);
	unsigned int __irq = irq_data->hwirq;
	const struct tps65090_irq_data *data = &tps65090_irqs[__irq];

	tps65090_set_bits(tps65090->dev, (TPS65090_INT_MSK + data->mask_reg),
		data->mask_pos);
}

static void tps65090_irq_unmask(struct irq_data *irq_data)
{
	struct tps65090 *tps65090 = irq_data_get_irq_chip_data(irq_data);
	unsigned int __irq = irq_data->irq - tps65090->irq_base;
	const struct tps65090_irq_data *data = &tps65090_irqs[__irq];

	tps65090_clr_bits(tps65090->dev, (TPS65090_INT_MSK + data->mask_reg),
		data->mask_pos);
}

static void tps65090_irq_sync_unlock(struct irq_data *data)
{
	struct tps65090 *tps65090 = irq_data_get_irq_chip_data(data);

	mutex_unlock(&tps65090->irq_lock);
}

static irqreturn_t tps65090_irq(int irq, void *data)
{
	struct tps65090 *tps65090 = data;
	int ret = 0;
	u8 status, mask;
	unsigned long int acks = 0;
	int i;

	for (i = 0; i < NUM_INT_REG; i++) {
		ret = tps65090_read(tps65090->dev, TPS65090_INT_MSK + i, &mask);
		if (ret < 0) {
			dev_err(tps65090->dev,
				"failed to read mask reg [addr:%d]\n",
				TPS65090_INT_MSK + i);
			return IRQ_NONE;
		}
		ret = tps65090_read(tps65090->dev, TPS65090_INT_STS + i,
			&status);
		if (ret < 0) {
			dev_err(tps65090->dev,
				"failed to read status reg [addr:%d]\n",
				 TPS65090_INT_STS + i);
			return IRQ_NONE;
		}
		if (status) {
			/* Ack only those interrupts which are not masked */
			status &= (~mask);
			ret = tps65090_write(tps65090->dev,
					TPS65090_INT_STS + i, status);
			if (ret < 0) {
				dev_err(tps65090->dev,
					"failed to write interrupt status\n");
				return IRQ_NONE;
			}
			acks |= (status << (i * 8));
		}
	}

	for_each_set_bit(i, &acks, ARRAY_SIZE(tps65090_irqs))
		handle_nested_irq(tps65090->irq_base + i);
	return acks ? IRQ_HANDLED : IRQ_NONE;
}

static int __devinit tps65090_irq_init(struct tps65090 *tps65090, int irq,
	int irq_base)
{
	int i, ret;

	if (!irq_base) {
		dev_err(tps65090->dev, "IRQ base not set\n");
		return -EINVAL;
	}

	mutex_init(&tps65090->irq_lock);

	for (i = 0; i < NUM_INT_REG; i++)
		tps65090_write(tps65090->dev, TPS65090_INT_MSK + i, 0xFF);

	for (i = 0; i < NUM_INT_REG; i++)
		tps65090_write(tps65090->dev, TPS65090_INT_STS + i, 0xff);

	tps65090->irq_base = irq_base;
	tps65090->irq_chip.name = "tps65090";
	tps65090->irq_chip.irq_mask = tps65090_irq_mask;
	tps65090->irq_chip.irq_unmask = tps65090_irq_unmask;
	tps65090->irq_chip.irq_bus_lock = tps65090_irq_lock;
	tps65090->irq_chip.irq_bus_sync_unlock = tps65090_irq_sync_unlock;

	for (i = 0; i < ARRAY_SIZE(tps65090_irqs); i++) {
		int __irq = i + tps65090->irq_base;
		irq_set_chip_data(__irq, tps65090);
		irq_set_chip_and_handler(__irq, &tps65090->irq_chip,
					 handle_simple_irq);
		irq_set_nested_thread(__irq, 1);
#ifdef CONFIG_ARM
		set_irq_flags(__irq, IRQF_VALID);
#endif
	}

	ret = request_threaded_irq(irq, NULL, tps65090_irq, IRQF_ONESHOT,
				"tps65090", tps65090);
	if (!ret) {
		device_init_wakeup(tps65090->dev, 1);
		enable_irq_wake(irq);
	}

	return ret;
}

static bool is_volatile_reg(struct device *dev, unsigned int reg)
{
	if ((reg == TPS65090_INT_STS) || (reg == TPS65090_INT_STS))
=======
#define TPS65090_INT1_MASK_VAC_STATUS_CHANGE		1
#define TPS65090_INT1_MASK_VSYS_STATUS_CHANGE		2
#define TPS65090_INT1_MASK_BAT_STATUS_CHANGE		3
#define TPS65090_INT1_MASK_CHARGING_STATUS_CHANGE	4
#define TPS65090_INT1_MASK_CHARGING_COMPLETE		5
#define TPS65090_INT1_MASK_OVERLOAD_DCDC1		6
#define TPS65090_INT1_MASK_OVERLOAD_DCDC2		7
#define TPS65090_INT2_MASK_OVERLOAD_DCDC3		0
#define TPS65090_INT2_MASK_OVERLOAD_FET1		1
#define TPS65090_INT2_MASK_OVERLOAD_FET2		2
#define TPS65090_INT2_MASK_OVERLOAD_FET3		3
#define TPS65090_INT2_MASK_OVERLOAD_FET4		4
#define TPS65090_INT2_MASK_OVERLOAD_FET5		5
#define TPS65090_INT2_MASK_OVERLOAD_FET6		6
#define TPS65090_INT2_MASK_OVERLOAD_FET7		7

static struct resource charger_resources[] = {
	{
		.start  = TPS65090_IRQ_VAC_STATUS_CHANGE,
		.end    = TPS65090_IRQ_VAC_STATUS_CHANGE,
		.flags  = IORESOURCE_IRQ,
	}
};

static const struct mfd_cell tps65090s[] = {
	{
		.name = "tps65090-pmic",
	},
	{
		.name = "tps65090-charger",
		.num_resources = ARRAY_SIZE(charger_resources),
		.resources = &charger_resources[0],
		.of_compatible = "ti,tps65090-charger",
	},
};

static const struct regmap_irq tps65090_irqs[] = {
	/* INT1 IRQs*/
	[TPS65090_IRQ_VAC_STATUS_CHANGE] = {
			.mask = TPS65090_INT1_MASK_VAC_STATUS_CHANGE,
	},
	[TPS65090_IRQ_VSYS_STATUS_CHANGE] = {
			.mask = TPS65090_INT1_MASK_VSYS_STATUS_CHANGE,
	},
	[TPS65090_IRQ_BAT_STATUS_CHANGE] = {
			.mask = TPS65090_INT1_MASK_BAT_STATUS_CHANGE,
	},
	[TPS65090_IRQ_CHARGING_STATUS_CHANGE] = {
			.mask = TPS65090_INT1_MASK_CHARGING_STATUS_CHANGE,
	},
	[TPS65090_IRQ_CHARGING_COMPLETE] = {
			.mask = TPS65090_INT1_MASK_CHARGING_COMPLETE,
	},
	[TPS65090_IRQ_OVERLOAD_DCDC1] = {
			.mask = TPS65090_INT1_MASK_OVERLOAD_DCDC1,
	},
	[TPS65090_IRQ_OVERLOAD_DCDC2] = {
			.mask = TPS65090_INT1_MASK_OVERLOAD_DCDC2,
	},
	/* INT2 IRQs*/
	[TPS65090_IRQ_OVERLOAD_DCDC3] = {
			.reg_offset = 1,
			.mask = TPS65090_INT2_MASK_OVERLOAD_DCDC3,
	},
	[TPS65090_IRQ_OVERLOAD_FET1] = {
			.reg_offset = 1,
			.mask = TPS65090_INT2_MASK_OVERLOAD_FET1,
	},
	[TPS65090_IRQ_OVERLOAD_FET2] = {
			.reg_offset = 1,
			.mask = TPS65090_INT2_MASK_OVERLOAD_FET2,
	},
	[TPS65090_IRQ_OVERLOAD_FET3] = {
			.reg_offset = 1,
			.mask = TPS65090_INT2_MASK_OVERLOAD_FET3,
	},
	[TPS65090_IRQ_OVERLOAD_FET4] = {
			.reg_offset = 1,
			.mask = TPS65090_INT2_MASK_OVERLOAD_FET4,
	},
	[TPS65090_IRQ_OVERLOAD_FET5] = {
			.reg_offset = 1,
			.mask = TPS65090_INT2_MASK_OVERLOAD_FET5,
	},
	[TPS65090_IRQ_OVERLOAD_FET6] = {
			.reg_offset = 1,
			.mask = TPS65090_INT2_MASK_OVERLOAD_FET6,
	},
	[TPS65090_IRQ_OVERLOAD_FET7] = {
			.reg_offset = 1,
			.mask = TPS65090_INT2_MASK_OVERLOAD_FET7,
	},
};

static struct regmap_irq_chip tps65090_irq_chip = {
	.name = "tps65090",
	.irqs = tps65090_irqs,
	.num_irqs = ARRAY_SIZE(tps65090_irqs),
	.num_regs = NUM_INT_REG,
	.status_base = TPS65090_INT_STS,
	.mask_base = TPS65090_INT_MSK,
	.mask_invert = true,
};

static bool is_volatile_reg(struct device *dev, unsigned int reg)
{
	if ((reg == TPS65090_INT_STS) || (reg == TPS65090_INT_STS2))
>>>>>>> refs/remotes/origin/master
		return true;
	else
		return false;
}

static const struct regmap_config tps65090_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = TOTAL_NUM_REG,
	.num_reg_defaults_raw = TOTAL_NUM_REG,
	.cache_type = REGCACHE_RBTREE,
	.volatile_reg = is_volatile_reg,
};

<<<<<<< HEAD
static int __devinit tps65090_i2c_probe(struct i2c_client *client,
					const struct i2c_device_id *id)
{
	struct tps65090_platform_data *pdata = client->dev.platform_data;
	struct tps65090 *tps65090;
	int ret;

	if (!pdata) {
		dev_err(&client->dev, "tps65090 requires platform data\n");
		return -EINVAL;
	}

	tps65090 = devm_kzalloc(&client->dev, sizeof(struct tps65090),
		GFP_KERNEL);
	if (tps65090 == NULL)
		return -ENOMEM;

	tps65090->client = client;
	tps65090->dev = &client->dev;
	i2c_set_clientdata(client, tps65090);

	mutex_init(&tps65090->lock);

	if (client->irq) {
		ret = tps65090_irq_init(tps65090, client->irq, pdata->irq_base);
		if (ret) {
			dev_err(&client->dev, "IRQ init failed with err: %d\n",
				ret);
			goto err_exit;
		}
	}

	tps65090->rmap = regmap_init_i2c(tps65090->client,
		&tps65090_regmap_config);
	if (IS_ERR(tps65090->rmap)) {
		dev_err(&client->dev, "regmap_init failed with err: %ld\n",
			PTR_ERR(tps65090->rmap));
		goto err_irq_exit;
	};

	ret = mfd_add_devices(tps65090->dev, -1, tps65090s,
		ARRAY_SIZE(tps65090s), NULL, 0);
	if (ret) {
		dev_err(&client->dev, "add mfd devices failed with err: %d\n",
			ret);
		goto err_regmap_exit;
=======
#ifdef CONFIG_OF
static const struct of_device_id tps65090_of_match[] = {
	{ .compatible = "ti,tps65090",},
	{},
};
MODULE_DEVICE_TABLE(of, tps65090_of_match);
#endif

static int tps65090_i2c_probe(struct i2c_client *client,
					const struct i2c_device_id *id)
{
	struct tps65090_platform_data *pdata = dev_get_platdata(&client->dev);
	int irq_base = 0;
	struct tps65090 *tps65090;
	int ret;

	if (!pdata && !client->dev.of_node) {
		dev_err(&client->dev,
			"tps65090 requires platform data or of_node\n");
		return -EINVAL;
	}

	if (pdata)
		irq_base = pdata->irq_base;

	tps65090 = devm_kzalloc(&client->dev, sizeof(*tps65090), GFP_KERNEL);
	if (!tps65090) {
		dev_err(&client->dev, "mem alloc for tps65090 failed\n");
		return -ENOMEM;
	}

	tps65090->dev = &client->dev;
	i2c_set_clientdata(client, tps65090);

	tps65090->rmap = devm_regmap_init_i2c(client, &tps65090_regmap_config);
	if (IS_ERR(tps65090->rmap)) {
		ret = PTR_ERR(tps65090->rmap);
		dev_err(&client->dev, "regmap_init failed with err: %d\n", ret);
		return ret;
	}

	if (client->irq) {
		ret = regmap_add_irq_chip(tps65090->rmap, client->irq,
			IRQF_ONESHOT | IRQF_TRIGGER_LOW, irq_base,
			&tps65090_irq_chip, &tps65090->irq_data);
			if (ret) {
				dev_err(&client->dev,
					"IRQ init failed with err: %d\n", ret);
			return ret;
		}
	}

	ret = mfd_add_devices(tps65090->dev, -1, tps65090s,
		ARRAY_SIZE(tps65090s), NULL,
		0, regmap_irq_get_domain(tps65090->irq_data));
	if (ret) {
		dev_err(&client->dev, "add mfd devices failed with err: %d\n",
			ret);
		goto err_irq_exit;
>>>>>>> refs/remotes/origin/master
	}

	return 0;

<<<<<<< HEAD
err_regmap_exit:
	regmap_exit(tps65090->rmap);

err_irq_exit:
	if (client->irq)
		free_irq(client->irq, tps65090);
err_exit:
	return ret;
}

static int __devexit tps65090_i2c_remove(struct i2c_client *client)
=======
err_irq_exit:
	if (client->irq)
		regmap_del_irq_chip(client->irq, tps65090->irq_data);
	return ret;
}

static int tps65090_i2c_remove(struct i2c_client *client)
>>>>>>> refs/remotes/origin/master
{
	struct tps65090 *tps65090 = i2c_get_clientdata(client);

	mfd_remove_devices(tps65090->dev);
<<<<<<< HEAD
	regmap_exit(tps65090->rmap);
	if (client->irq)
		free_irq(client->irq, tps65090);
=======
	if (client->irq)
		regmap_del_irq_chip(client->irq, tps65090->irq_data);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM
static int tps65090_i2c_suspend(struct i2c_client *client, pm_message_t state)
{
	if (client->irq)
		disable_irq(client->irq);
	return 0;
}

static int tps65090_i2c_resume(struct i2c_client *client)
{
	if (client->irq)
		enable_irq(client->irq);
	return 0;
}
#endif

=======
>>>>>>> refs/remotes/origin/master
static const struct i2c_device_id tps65090_id_table[] = {
	{ "tps65090", 0 },
	{ },
};
MODULE_DEVICE_TABLE(i2c, tps65090_id_table);

static struct i2c_driver tps65090_driver = {
	.driver	= {
		.name	= "tps65090",
		.owner	= THIS_MODULE,
<<<<<<< HEAD
	},
	.probe		= tps65090_i2c_probe,
	.remove		= __devexit_p(tps65090_i2c_remove),
#ifdef CONFIG_PM
	.suspend	= tps65090_i2c_suspend,
	.resume		= tps65090_i2c_resume,
#endif
=======
		.of_match_table = of_match_ptr(tps65090_of_match),
	},
	.probe		= tps65090_i2c_probe,
	.remove		= tps65090_i2c_remove,
>>>>>>> refs/remotes/origin/master
	.id_table	= tps65090_id_table,
};

static int __init tps65090_init(void)
{
	return i2c_add_driver(&tps65090_driver);
}
subsys_initcall(tps65090_init);

static void __exit tps65090_exit(void)
{
	i2c_del_driver(&tps65090_driver);
}
module_exit(tps65090_exit);

MODULE_DESCRIPTION("TPS65090 core driver");
MODULE_AUTHOR("Venu Byravarasu <vbyravarasu@nvidia.com>");
MODULE_LICENSE("GPL v2");
