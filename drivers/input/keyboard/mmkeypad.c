#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/input.h>
#include <linux/input/matrix_keypad.h>
#include <linux/mfd/core.h>
#include <linux/mfd/stmpe.h>
#include "mmkeypad.h"

#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/spinlock.h>

#include <linux/delay.h>
static struct stmpe_variant_info stmpe1801 = {
	.name		= "stmpe1801",
	.id_val		= 0xc110,
	.id_mask	= 0xffff,
	.num_gpios	= 18,
	.af_bits	= 2,
	.blocks		= NULL,
	.num_blocks	= NULL,
	.num_irqs	= NULL,
	.enable		= NULL,
	.get_altfunc	= NULL,
	.enable_autosleep = NULL,
};

static const struct stmpe_keypad_variant stmpe_keypad_variants = {
	.auto_increment		= true,
	.num_data		= 5,
	.num_normal_data	= 3,
	.max_cols		= 10,
	.max_rows		= 8,
	.col_gpios		= 0x1fe00,/* GPIO 9 - 16 */
	.row_gpios		= 0x0007f,/* GPIO 0 - 6 */
};

static int __stmpe_reg_read(struct stmpe *stmpe, u8 reg)
{
	int ret;
	ret = i2c_smbus_read_byte_data(stmpe->i2c, reg);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to read reg %#x: %d\n",
			reg, ret);

	dev_vdbg(stmpe->dev, "rd: reg %#x => data %#x\n", reg, ret);

	return ret;
}

static int __stmpe_reg_write(struct stmpe *stmpe, u8 reg, u8 val)
{
	int ret;
	dev_vdbg(stmpe->dev, "wr: reg %#x <= %#x\n", reg, val);

	ret = i2c_smbus_write_byte_data(stmpe->i2c, reg, val);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to write reg %#x: %d\n",
			reg, ret);

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
	ret = i2c_smbus_read_i2c_block_data(stmpe->i2c, reg, length, values);
	
	if (ret < 0)
		dev_err(stmpe->dev, "failed to read regs %#x: %d\n",
			reg, ret);

	dev_vdbg(stmpe->dev, "rd: reg %#x (%d) => ret %#x\n", reg, length, ret);

	return ret;
}

static int __stmpe_block_write(struct stmpe *stmpe, u8 reg, u8 length,
			const u8 *values)
{
	int ret;
	dev_vdbg(stmpe->dev, "wr: regs %#x (%d)\n", reg, length);

	ret = i2c_smbus_write_i2c_block_data(stmpe->i2c, reg, length,
					     values);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to write regs %#x: %d\n",
			reg, ret);

	return ret;
}

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

int stmpe_keypad_read_data(struct stmpe_keypad *keypad, u8 *data)
{
	const struct stmpe_keypad_variant *variant = keypad->variant;
	struct stmpe *stmpe = keypad->stmpe;
	int ret;
	int i;

	if (variant->auto_increment)
		return stmpe_block_read(stmpe, STMPE_KPC_DATA_BYTE0,
					variant->num_data, data);

	for (i = 0; i < variant->num_data; i++) {
		ret = stmpe_reg_read(stmpe, STMPE_KPC_DATA_BYTE0 + i);
		if (ret < 0)
			return ret;

		data[i] = ret;
	}

	return 0;
}

static irqreturn_t stmpe_irq(int irq, void *data)
{
	struct stmpe_keypad *keypad = data;
	struct stmpe *stmpe = keypad->stmpe;
	struct input_dev *input = keypad->input;
	struct stmpe_variant_info *variant = stmpe->variant;

	u8 fifo[keypad->variant->num_data];
	int ret, i;
	int iStaValue;

	printk(KERN_INFO"stmpe_irq ................\n");

	/* Clear Interrupt */	
	iStaValue = stmpe_reg_read(stmpe, STMPE1801_REG_ISR_LSB);
	
	/* Qwerty Keypad */
	ret = stmpe_keypad_read_data(keypad, fifo);
	if (ret < 0)
		return IRQ_NONE;

	for (i = 0; i < keypad->variant->num_normal_data; i++) {
		u8 data = fifo[i];
		int col = (data & STMPE_KPC_DATA_COL) >> 3;
		int row = data & STMPE_KPC_DATA_ROW;

		int code = keypad->keymap[MATRIX_SCAN_CODE(row, col, STMPE_KEYPAD_COL_SHIFT)];
		bool up = data & STMPE_KPC_DATA_UP;

		if ((data & STMPE_KPC_DATA_NOKEY_MASK) ==
					STMPE_KPC_DATA_NOKEY_MASK)
			continue;

		input_event(input, EV_MSC, MSC_SCAN, code);
		input_report_key(input, code, !up);
		input_sync(input);
	}


	return IRQ_HANDLED;
}

static void stmpe_irq_remove(struct stmpe *stmpe)
{
}

static void stmpe_KeyLED_On(struct stmpe_keypad *keypad)
{
	int ret;
	struct stmpe *stmpe = keypad->stmpe;

	ret = stmpe_reg_write(stmpe, STMPE1801_REG_GPIO_SET_MID, 0x01);
	if (ret < 0)
		return ret;
}
static void stmpe_KeyLED_Off(struct stmpe_keypad *keypad)
{
	int ret;
	struct stmpe *stmpe = keypad->stmpe;

	ret = stmpe_reg_write(stmpe,
			STMPE1801_REG_GPIO_CLR_MID, 0x01);
	if (ret < 0)
		return ret;
}

struct device *kp_flip_dev;
struct stmpe *flip_stmpe;

void slide_open_led_on(void)
{
	int ret;

	ret = stmpe_reg_write(flip_stmpe, STMPE1801_REG_GPIO_SET_MID, 0x01);
}

void slide_close_led_off(void)
{
	int ret;

	ret = stmpe_reg_write(flip_stmpe, STMPE1801_REG_GPIO_CLR_MID, 0x01);
}

static ssize_t slide_test(struct device *dev, struct device_attribute *attr,
			char *buf, size_t size)
{
	int ret;
	u8 slide_ON;

	sscanf(buf, "%d", &slide_ON);

	if (slide_ON)
		ret = stmpe_reg_write(flip_stmpe,
			STMPE1801_REG_GPIO_SET_MID, 0x01);
	else
		ret = stmpe_reg_write(flip_stmpe,
			STMPE1801_REG_GPIO_CLR_MID, 0x01);

	return slide_ON;
}

static DEVICE_ATTR(brightness, S_IWUSR, NULL, slide_test);

static int __devinit stmpe_keypad_chip_init(struct stmpe_keypad *keypad)
{
	const struct stmpe_keypad_platform_data *plat = keypad->plat;
	const struct stmpe_keypad_variant *variant = keypad->variant;
	struct stmpe *stmpe = keypad->stmpe;
	int ret;

	if (plat->debounce_ms > STMPE_KEYPAD_MAX_DEBOUNCE)
		return -EINVAL;
	if (plat->scan_count > STMPE_KEYPAD_MAX_SCAN_COUNT)
		return -EINVAL;

	/* KPC Column */
	ret = stmpe_reg_write(stmpe, STMPE_KPC_COL_LOW, 0xFE);
	if (ret < 0)
		return ret;

	ret = stmpe_reg_write(stmpe, STMPE_KPC_COL_HIGH, 0x01);
	if (ret < 0)
		return ret;

	/* KPC Row */
	ret = stmpe_reg_write(stmpe, STMPE_KPC_ROW, 0x7F);
	if (ret < 0)
		return ret;

	/* KPC Control scan count, dedicated keys */
	ret = stmpe_reg_write(stmpe, STMPE_KPC_CTL_LOW, 0x50);
	if (ret < 0)
		return ret;

	/* KPC Control debounce time */
	ret = stmpe_reg_write(stmpe, STMPE_KPC_CTL_MID, 0x3E);
	if (ret < 0)
		return ret;

	/* Scan frequency */
	ret = stmpe_reg_write(stmpe, STMPE_KPC_CTL_HIGH, 0x43);
	if (ret < 0)
		return ret;

	ret = stmpe_reg_write(stmpe, STMPE_GPIO_DIR_MID, 0x01);
	if (ret < 0)
		return ret;

	ret = stmpe_reg_write(stmpe, STMPE_KPC_COMBI_KEY0, 0x07);
	if (ret < 0)
		return ret;

	ret = stmpe_reg_write(stmpe, STMPE_KPC_COMBI_KEY0, 0x3F);
	if (ret < 0)
		return ret;

	ret = stmpe_reg_write(stmpe, STMPE_KPC_COMBI_KEY0, 0x47);
	if (ret < 0)
		return ret;

	ret = stmpe_reg_write(stmpe, STMPE1801_REG_GPIO_CLR_MID, 0x01);
	if (ret < 0)
		return ret;

	/* Scan START */
	ret = stmpe_reg_write(stmpe, STMPE_KPC_CMD, 0x01);
	if (ret < 0)
		return ret;
	msleep(20);
	return 0;
}

static int __devinit stmpe_chip_INT_init(struct stmpe *stmpe)
{
	unsigned int irq_trigger = IRQF_TRIGGER_FALLING;

	struct stmpe_variant_info *variant = stmpe->variant;

	u8 icr = 0;
	unsigned int id;
	u8 data[2];
	int ret;

	ret = stmpe_block_read(stmpe, STMPE1801_REG_CHIP_ID, ARRAY_SIZE(data), data);
	if (ret < 0)
		return ret;
	msleep(50);
	id = ((unsigned int)data[0] << 8) | (unsigned int)data[1];

	if ((id & variant->id_mask) != variant->id_val) {
		dev_err(stmpe->dev, "unknown chip id: %#x\n", id);
		return -EINVAL;
	}
	dev_info(stmpe->dev, "%s detected, chip id: %#x\n", variant->name, id);

	ret = stmpe_reg_write(stmpe, STMPE1801_REG_SYS_CTRL, 0x86);
	if (ret < 0)
		return ret;
	msleep(20);
	ret = stmpe_reg_write(stmpe, STMPE1801_REG_IER_LSB, 0x16);
	if (ret < 0)
		return ret;
	msleep(20);
	icr = STMPE_ICR_LSB_GIM;
	ret = stmpe_reg_write(stmpe, STMPE1801_REG_ICR_LSB, icr);
	if (ret < 0)
		return ret;
	msleep(20);
	return 0;
}

static int __devinit stmpe_probe(struct i2c_client *i2c, const struct i2c_device_id *id)
{
	struct stmpe_keypad_platform_data *pdata = i2c->dev.platform_data;
	struct stmpe_keypad *keypad;
	struct input_dev *input;
	struct stmpe *stmpe;

	int ret, irq, i;

	if (!pdata)
		return -ENOMEM;

	flip_stmpe = stmpe = kzalloc(sizeof(*stmpe), GFP_KERNEL);
	if (!stmpe)
		return -ENOMEM;

	keypad = kzalloc(sizeof(struct stmpe_keypad), GFP_KERNEL);
	if (!keypad) {
		kfree(stmpe);
		return -ENOMEM;
	}

	mutex_init(&stmpe->irq_lock);
	mutex_init(&stmpe->lock);

	stmpe->dev = &i2c->dev;
	stmpe->i2c = i2c;
	stmpe->variant = &stmpe1801;
	stmpe->num_gpios = stmpe->variant->num_gpios;


	input = input_allocate_device();
	if (!input) {
		ret = -ENOMEM;
		goto out_free;
	}

	input->name = "STMPE_keypad";
	input->id.bustype = BUS_I2C;
	input->dev.parent = &i2c->dev;

	input_set_capability(input, EV_MSC, MSC_SCAN);

	__set_bit(EV_KEY, input->evbit);

	if (!pdata->no_autorepeat)
		__set_bit(EV_REP, input->evbit);

	input->keycode = keypad->keymap;
	input->keycodesize = sizeof(keypad->keymap[0]);
	input->keycodemax = ARRAY_SIZE(keypad->keymap);

	matrix_keypad_build_keymap(pdata->keymap_data, STMPE_KEYPAD_COL_SHIFT,
		input->keycode, input->keybit);

	for (i = 0; i < pdata->keymap_data->keymap_size; i++) {
		unsigned int key = pdata->keymap_data->keymap[i];

		keypad->cols |= 1 << KEY_COL(key);
		keypad->rows |= 1 << KEY_ROW(key);
	}

	keypad->stmpe = stmpe;
	keypad->plat = pdata;
	keypad->input = input;
	keypad->variant = &stmpe_keypad_variants;

	kp_flip_dev = device_create(sec_class, NULL, 0, NULL,
		 "sec_qtkey");
	if (!kp_flip_dev)
		printk(KERN_INFO"Failed to create device(sec_qtkey)!\n");

	if (device_create_file(kp_flip_dev, &dev_attr_brightness) < 0)
		printk(KERN_INFO"Failed to create file(%s)!\n",
			dev_attr_brightness.attr.name);

	i2c_set_clientdata(i2c, stmpe);

	ret = stmpe_chip_INT_init(stmpe);
	if (ret)
		goto out_free;

	ret = stmpe_keypad_chip_init(keypad);
	if (ret)
		goto out_free;
	
	ret = input_register_device(input);
	if (ret) {
		dev_err(&i2c->dev, "unable to register input device: %d\n", ret);
		goto out_freeinput;
	}

	ret = request_threaded_irq(stmpe->i2c->irq, NULL, stmpe_irq, 
			IRQF_TRIGGER_FALLING, "stmpe1801", keypad);
	if (ret) {
		dev_err(stmpe->dev, "failed to request IRQ : %d\n", ret);
		goto out_removeirq;
	}

	ret = enable_irq_wake(stmpe->i2c->irq);
	if (ret < 0)
		dev_err(stmpe->dev, "failed to enable wakeup src %d\n", ret);
	printk(KERN_INFO"stmpe1801_keypad probe ............ Done\n");
	return 0;

out_removedevs:
	free_irq(stmpe->i2c-irq, stmpe);

out_removeirq:
	stmpe_irq_remove(stmpe);
out_freeinput:
	input_free_device(input);
out_free:
	kfree(stmpe);
	kfree(keypad);

	return ret;
}

static int __devexit stmpe_remove(struct i2c_client *client)
{
	struct stmpe *stmpe = i2c_get_clientdata(client);
	free_irq(stmpe->i2c->irq, stmpe);
	stmpe_irq_remove(stmpe);
	kfree(stmpe);

	return 0;
}

#ifdef CONFIG_PM
static int stmpe_key_suspend(struct i2c_client *client, pm_message_t mesg)
{
/*
	int ret;
	ret = stmpe_reg_write(flip_stmpe, STMPE1801_REG_ICR_LSB, 0x0);
	if (ret < 0)
		return ret;

	if (device_may_wakeup(&client->dev))
		enable_irq_wake(client->irq);
*/
	return 0;
}

static int stmpe_key_resume(struct i2c_client *client)
{
/*
	int ret;

	if (device_may_wakeup(&client->dev))
		disable_irq_wake(client->irq);

	ret = stmpe_reg_write(flip_stmpe, STMPE1801_REG_ICR_LSB, 0x1);
	if (ret < 0)
		return ret;
*/
	return 0;
}
#else
#define stmpe_key_suspend	NULL
#define stmpe_key_resume	NULL
#endif

static const struct i2c_device_id stmpe_id[] = {
	{ "stmpe1801", 0 },
	{ }
};


MODULE_DEVICE_TABLE(i2c, stmpe_id);

static struct i2c_driver stmpe_driver = {
	.driver.name	= "stmpe1801",
	.driver.owner	= THIS_MODULE,
	.probe		= stmpe_probe,
	.remove		= __devexit_p(stmpe_remove),
	.suspend	= stmpe_key_suspend,
	.resume		= stmpe_key_resume,
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

