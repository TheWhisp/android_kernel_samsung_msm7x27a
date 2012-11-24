#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/earlysuspend.h>
#include "sensors_head.h"

#define I2C_RETRY_DELAY		5
#define I2C_RETRIES		1

#define I2C_HSCD_ADDR (0x0d)
#define I2C_BUS_NUMBER	1
#define HSCD_CHIP_ID 0x49

#define HSCD_DRIVER_NAME "hscd_i2c"

#define HSCD_STBA		0x0B
#define HSCD_STBB		0x0C
#define HSCD_XOUT		0x10
#define HSCD_YOUT		0x12
#define HSCD_ZOUT		0x14
#define HSCD_XOUT_H		0x11
#define HSCD_XOUT_L		0x10
#define HSCD_YOUT_H		0x13
#define HSCD_YOUT_L		0x12
#define HSCD_ZOUT_H		0x15
#define HSCD_ZOUT_L		0x14

#define HSCD_STATUS		0x18
#define HSCD_CTRL1		0x1b
#define HSCD_CTRL2		0x1c
#define HSCD_CTRL3		0x1d
#ifdef HSCDTD002A
#define HSCD_CTRL4	0x28
#endif

/*#define ALPS_DEBUG*/
#define SUSPEND_MODE_RETURN 1
static struct i2c_driver hscd_driver;
static struct i2c_client *client_hscd;

static atomic_t flgEna;
static atomic_t delay;

static struct device *magnetic_device;
#define RUNNING_INPUT_EVENT 1
#define STOP_INPUT_EVENT	0
#define SUSPEND_MODE_RETURN 1
static int flagPtr;
static int probe_done;

struct hscd_i2c_data {
	struct i2c_client *this_client;
	struct input_dev *input_dev;
	struct early_suspend early_suspend;
};
struct hscd_i2c_data *hscd_data;
static void hscd_early_suspend(struct early_suspend *handler);
static void hscd_early_resume(struct early_suspend *handler);

static int hscd_i2c_readm(char *rxData, int length)
{
	int err;
	int tries = 0;

	struct i2c_msg msgs[] = {
		{
			.addr = hscd_data->this_client->addr,
			.flags = 0,
			.len = 1,
			.buf = rxData,
		},
		{
			.addr = hscd_data->this_client->addr,
			.flags = I2C_M_RD,
			.len = length,
			.buf = rxData,
		},
	};

	do {
		err = i2c_transfer(hscd_data->this_client->adapter, msgs, 2);
	} while ((err != 2) && (++tries < I2C_RETRIES));

	if (err != 2) {
		dev_err(&hscd_data->this_client->adapter->dev, "read transfer error\n");
		err = -EIO;
	} else {
		err = 0;
	}

	return err;
}

static int hscd_i2c_writem(char *txData, int length)
{
	int err;
	int tries = 0;
#ifdef ALPS_DEBUG
	int i;
#endif

	struct i2c_msg msg[] = {
		{
			.addr = hscd_data->this_client->addr,
			.flags = 0,
			.len = length,
			.buf = txData,
		},
	};

#ifdef ALPS_DEBUG
	pr_info("[HSCD] i2c_writem : ");
	for (i = 0; i < length; i++)
		pr_info("0X%02X, ", txData[i]);
	pr_info("\n");
#endif

	do {
		err = i2c_transfer(hscd_data->this_client->adapter, msg, 1);
	} while ((err != 1) && (++tries < I2C_RETRIES));

	if (err != 1) {
		dev_err(&hscd_data->this_client->adapter->dev, "write transfer error\n");
		err = -EIO;
	} else {
		err = 0;
	}

	return err;
}

int hscd_self_test_A(void)
{
	u8 sx[2], cr1[1];

	/* Control resister1 backup  */
	cr1[0] = HSCD_CTRL1;
	if (hscd_i2c_readm(cr1, 1))
		return 1;
#ifdef ALPS_DEBUG
	else
		pr_info("[HSCD] Control resister1 value, %02X\n", cr1[0]);
#endif
	msleep(20);
	/* Stndby Mode  */
	if (cr1[0] & 0x80) {
		sx[0] = HSCD_CTRL1;
		sx[1] = 0x60;
		if (hscd_i2c_writem(sx, 2))
			return 1;
	}

	/* Get inital value of self-test-A register  */
	sx[0] = HSCD_STBA;
	if (hscd_i2c_readm(sx, 1))
		return 1;
#ifdef ALPS_DEBUG
	else
		pr_info("[HSCD] self test A register value, %02X\n", sx[0]);
#endif
	if (sx[0] != 0x55) {
		pr_info("error: self-test-A, initial value is %02X\n", sx[0]);
		return 2;
	}

	/* do self-test-A  */
	sx[0] = HSCD_CTRL3;
	sx[1] = 0x20;
	if (hscd_i2c_writem(sx, 2))
		return 1;
	msleep(20);

	/* Get 1st value of self-test-A register  */
	sx[0] = HSCD_STBA;
	if (hscd_i2c_readm(sx, 1))
		return 1;
#ifdef ALPS_DEBUG
	else
		pr_info("[HSCD] self test A register value, %02X\n", sx[0]);
#endif
	if (sx[0] != 0xAA) {
		pr_info("error: self-test-A, 1st value is %02X\n", sx[0]);
		return 3;
	}
	msleep(20);

	/* Get 2nd value of self-test-A register  */
	sx[0] = HSCD_STBA;
	if (hscd_i2c_readm(sx, 1))
		return 1;
#ifdef ALPS_DEBUG
	else
		pr_info("[HSCD] self test A register value, %02X\n", sx[0]);
#endif
	if (sx[0] != 0x55) {
		pr_info("error: self-test-A, 2nd value is %02X\n", sx[0]);
		return 4;
	}

	/* Active Mode  */
	if (cr1[0] & 0x80) {
		sx[0] = HSCD_CTRL1;
		sx[1] = cr1[0];
		if (hscd_i2c_writem(sx, 2))
			return 1;
	}

	return 0;
}
EXPORT_SYMBOL(hscd_self_test_A);

int hscd_self_test_B(void)
{
	int rc = 0;
	u8 sx[2], cr1[1];


	/* Control resister1 backup  */
	cr1[0] = HSCD_CTRL1;
	if (hscd_i2c_readm(cr1, 1))
		return 1;
#ifdef ALPS_DEBUG
	else
		pr_info("[HSCD] Control resister1 value, %02X\n", cr1[0]);
#endif
	msleep(20);

	/* Get inital value of self-test-B register  */
	sx[0] = HSCD_STBB;
	if (hscd_i2c_readm(sx, 1))
		return 1;
#ifdef ALPS_DEBUG
	else
		pr_info("[HSCD] self test B register value, %02X\n", sx[0]);
#endif
	if (sx[0] != 0x55) {
		pr_info("error: self-test-B, initial value is %02X\n", sx[0]);
		return 2;
	}

	/* Active mode (Force state)  */
	sx[0] = HSCD_CTRL1;
	sx[1] = 0xC2;
	if (hscd_i2c_writem(sx, 2))
		return 1;
	msleep(20);

	do {
		/* do self-test-B  */
		sx[0] = HSCD_CTRL3;
		sx[1] = 0x10;
		if (hscd_i2c_writem(sx, 2)) {
			rc = 1;
			break;
		}
		msleep(20);

		/* Get 1st value of self-test-A register  */
		sx[0] = HSCD_STBB;
		if (hscd_i2c_readm(sx, 1)) {
			rc = 1;
			break;
		}
#ifdef ALPS_DEBUG
		else
			pr_info("[HSCD] self test B value, %02X\n", sx[0]);
#endif
		if (sx[0] != 0xAA) {
			if ((sx[0] < 0x01) || (sx[0] > 0x07)) {
				pr_info("error: self-test-B 1st %02X\n", sx[0]);
				rc = 3;
				break;
			}
		}
		msleep(20);

		/* Get 2nd value of self-test-B register  */
		sx[0] = HSCD_STBB;
		if (hscd_i2c_readm(sx, 1)) {
			rc = 1;
			break;
		}
#ifdef ALPS_DEBUG
		else
			pr_info("[HSCD] self test B value, %02X\n", sx[0]);
#endif
		if (sx[0] != 0x55) {
			pr_info("error: self-test-B, 2nd %02X\n", sx[0]);
			rc = 4;
			break;
		}
	} while (0) ;

	/* Active Mode  */
	if (cr1[0] & 0x80) {
		sx[0] = HSCD_CTRL1;
		sx[1] = cr1[0];
		if (hscd_i2c_writem(sx, 2))
			return 1;
	}

	return rc;
}
EXPORT_SYMBOL(hscd_self_test_B);

int hscd_get_magnetic_field_data(int *xyz)
{
	static int print_i;
	int err = -1;
	int i;
	u8 sx[6];

	sx[0] = HSCD_XOUT;
	err = hscd_i2c_readm(sx, 6);
	if (err < 0)
		return err;
	for (i = 0; i < 3; i++)
		xyz[i] = (int)((short)((sx[2*i+1] << 8) | (sx[2*i])));
#ifdef ALPS_DEBUG
	print_i++;
	if (print_i > 250) {
		pr_info("[HSCD] x:%d, y:%d, z:%d\n",
			xyz[0], xyz[1], xyz[2]);
		print_i = 0;
	}
#endif
	if (flagPtr == STOP_INPUT_EVENT)
		return SUSPEND_MODE_RETURN;
	return err;
}
EXPORT_SYMBOL(hscd_get_magnetic_field_data);

void hscd_activate(int flgatm, int flg, int dtime)
{
	u8 buf[2];

	if (flg != 0)
		flg = 1;

	if      (dtime <=  10)
		buf[1] = (0x60 | 3<<2);		/*100Hz- 10msec*/
	else if (dtime <=  20)
		buf[1] = (0x60 | 2<<2);		/*50Hz- 20msec*/
	else if (dtime <=  60)
		buf[1] = (0x60 | 1<<2);		/*20Hz- 50msec*/
	else
		buf[1] = (0x60 | 0<<2);		/*10Hz-100msec*/
	buf[0]  = HSCD_CTRL1;

	if (flagPtr == STOP_INPUT_EVENT)
		buf[1] |= (0<<7);
	else
		buf[1] |= (flg<<7);
	buf[1] |= 0x60;
	/*RS1:1(Reverse Input Substraction drive), RS2:1(13bit)*/
	hscd_i2c_writem(buf, 2);

	if (flagPtr == RUNNING_INPUT_EVENT) {
		if (flg) {
			buf[0] = HSCD_CTRL3;
			buf[1] = 0x02;
			hscd_i2c_writem(buf, 2);
		}
	}

	if (flgatm) {
		atomic_set(&flgEna, flg);
		atomic_set(&delay, dtime);
	}
}
EXPORT_SYMBOL(hscd_activate);

static void hscd_register_init(void)
{
#ifdef ALPS_DEBUG
	pr_info("[HSCD] register_init\n");
#endif
}

static ssize_t selttest_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int result1 = hscd_self_test_A();
	int result2 = hscd_self_test_B();
	if (result1 == 0)
		result1 = 1;
	else
		result1 = 0;
	if (result2 == 0)
		result2 = 1;
	else
		result2 = 0;
#ifdef ALPS_DEBUG
	pr_info("Selftest Result is %d, %d\n", result1, result2);
#endif
	return snprintf(buf, PAGE_SIZE, "%d, %d\n", result1, result2);
}
static ssize_t status_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int result = hscd_self_test_B();
	if (result == 0)
		result = 1;
	else
		result = 0;
	return snprintf(buf, PAGE_SIZE, "%d,%d\n", result, 0);
}
static ssize_t dac_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d,%d,%d\n", 0, 0, 0);
}
static ssize_t adc_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int d[3];
	hscd_get_magnetic_field_data(d);
#ifdef ALPS_DEBUG
	pr_info("[HSCD] x:%d y:%d z:%d\n", d[0], d[1], d[2]);
#endif
	return snprintf(buf, PAGE_SIZE, "%d,%d,%d\n", d[0], d[1], d[2]);
}

int hscd_open()
{
	return probe_done;
}
EXPORT_SYMBOL(hscd_open);

static DEVICE_ATTR(selftest, 0664, selttest_show, NULL);
static DEVICE_ATTR(status, 0664, status_show, NULL);
static DEVICE_ATTR(dac, 0644, dac_show, NULL);
static DEVICE_ATTR(adc, 0644, adc_show, NULL);

static struct device_attribute *magnetic_attrs[] = {
	&dev_attr_selftest,
	&dev_attr_status,
	&dev_attr_dac,
	&dev_attr_adc,
	NULL,
};
static ssize_t mag_enable_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int result = hscd_self_test_A();
	if (result == 0)
		result = 1;
	else
		result = 0;
	return snprintf(buf, PAGE_SIZE, "%d\n", result);
}
static struct device_attribute dev_attr_enable =
__ATTR(enable, S_IRUGO | S_IWUSR | S_IWGRP,
		mag_enable_show, NULL);
static struct attribute *mag_sysfs_attrs[] = {
	&dev_attr_enable.attr,
	NULL
};
static struct attribute_group mag_attribute_group = {
	.attrs = mag_sysfs_attrs,
};
static int hscd_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int d[3];
	int err = 0;
	/*struct input_dev *input_dev;*/
	struct hscd_i2c_data *hscd;
	u8 buf[2] = {0};

	pr_info("[HSCD] probe\n");
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->adapter->dev, "client not i2c capable\n");
		return -ENOMEM;
	}

	hscd = kzalloc(sizeof(struct hscd_i2c_data), GFP_KERNEL);
	if (!hscd) {
		dev_err(&client->dev,
			"failed to allocate memory for module data\n");
		err = -ENOMEM;
		goto exit_alloc_data_failed;
	}

	i2c_set_clientdata(client, hscd->this_client);

	hscd->this_client = client;
	hscd_data = hscd;
	hscd_register_init();

	dev_info(&hscd->this_client->adapter->dev, "detected HSCD magnetic sensor\n");

	buf[0] = 0x0F;
	err = hscd_i2c_readm(buf, 1);
	if (err) {
		pr_info("[HSCD] read error\n");
		err = -ENOMEM;
		goto err_input_register_device;
	} else {
#ifdef ALPS_DEBUG
		pr_info("[HSCD] chip ID = %x\n", buf[0]);
#endif
		if (buf[0] != HSCD_CHIP_ID)
			goto err_input_register_device;
	}

	flagPtr = RUNNING_INPUT_EVENT;
	hscd_activate(0, 1, atomic_read(&delay));
	err = hscd_get_magnetic_field_data(d);
	if (err) {
		pr_info("[HSCD] Read Data error\n");
		goto err_input_register_device;
	}
#ifdef ALPS_DEBUG
	pr_info("[HSCD] x:%d y:%d z:%d\n", d[0], d[1], d[2]);
#endif
	hscd_activate(0, 0, atomic_read(&delay));

	sensors_register(magnetic_device, NULL,
		magnetic_attrs, "magnetic_sensor");

	hscd->input_dev = input_allocate_device();
	if (!hscd->input_dev) {
		pr_info("%s: could not allocate input device\n", __func__);
		return -ENOMEM;
	}
	err = input_register_device(hscd->input_dev);
	if (err < 0) {
		pr_info("%s: could not register input device\n", __func__);
		input_free_device(hscd->input_dev);
		goto err_input_register_device;
	}
	hscd->input_dev->name = "magnetic_sensor";
	err = sysfs_create_group(&hscd->input_dev->dev.kobj,
		&mag_attribute_group);
	if (err) {
		pr_info("Creating bh1721 attribute group failed");
		goto error_device;
	}

	hscd->early_suspend.suspend = hscd_early_suspend;
	hscd->early_suspend.resume = hscd_early_resume;
	register_early_suspend(&hscd->early_suspend);
	probe_done = PROBE_SUCCESS;
	return 0;
error_device:
	input_unregister_device(hscd->input_dev);
err_input_register_device:
	kfree(hscd);
exit_alloc_data_failed:
	return err;
}

static void hscd_shutdown(struct i2c_client *client)
{
#ifdef ALPS_DEBUG
	pr_info("[HSCD] shutdown\n");
#endif
	hscd_activate(0, 0, atomic_read(&delay));
}

static int hscd_suspend(struct i2c_client *client, pm_message_t mesg)
{
#ifdef ALPS_DEBUG
	pr_info("[HSCD] suspend\n");
#endif
	hscd_activate(0, 0, atomic_read(&delay));
	flagPtr = STOP_INPUT_EVENT;
	return 0;
}

static int hscd_resume(struct i2c_client *client)
{
#ifdef ALPS_DEBUG
	pr_info("[HSCD] resume\n");
#endif
	flagPtr = RUNNING_INPUT_EVENT;

	hscd_activate(0, atomic_read(&flgEna), atomic_read(&delay));

	return 0;
}

static void hscd_early_suspend(struct early_suspend *handler)
{
	struct hscd_i2c_data *hscd_data;
	hscd_data = container_of(handler, struct hscd_i2c_data, early_suspend);

	pr_info("%s\n", __func__);

	hscd_suspend(hscd_data->this_client, PMSG_SUSPEND);
}

static void hscd_early_resume(struct early_suspend *handler)
{
	struct hscd_i2c_data *hscd_data;
	hscd_data = container_of(handler, struct hscd_i2c_data, early_suspend);

	pr_info("%s\n", __func__);

	hscd_resume(hscd_data->this_client);
}

static const struct i2c_device_id ALPS_id[] = {
	{ HSCD_DRIVER_NAME, 0 },
	{ }
};

static struct i2c_driver hscd_driver = {
	.probe    = hscd_probe,
	.id_table = ALPS_id,
	.driver   = {
		.name	= HSCD_DRIVER_NAME,
	},
	.shutdown		= hscd_shutdown,
	/*.suspend		= hscd_suspend,*/
	/*.resume		= hscd_resume,*/
};

static int __init hscd_init(void)
{
/*	struct i2c_board_info i2c_info;
	struct i2c_adapter *adapter;*/
	int rc;
	probe_done = PROBE_FAIL;
#ifdef ALPS_DEBUG
	pr_info("[HSCD] init\n");
#endif
	atomic_set(&flgEna, 0);
	atomic_set(&delay, 200);


	rc = i2c_add_driver(&hscd_driver);
	if (rc != 0) {
		pr_info("can't add i2c driver\n");
		rc = -ENOTSUPP;
		return rc;
	}
/*
	memset(&i2c_info, 0, sizeof(struct i2c_board_info));
	i2c_info.addr = I2C_HSCD_ADDR;
	strlcpy(i2c_info.type, HSCD_DRIVER_NAME , I2C_NAME_SIZE);

	adapter = i2c_get_adapter(I2C_BUS_NUMBER);
	if (!adapter) {
		pr_info("can't get i2c adapter %d\n", I2C_BUS_NUMBER);
		rc = -ENOTSUPP;
		goto probe_done;
	}

	client_hscd = i2c_new_device(adapter, &i2c_info);
	client_hscd->adapter->timeout = 0;
	client_hscd->adapter->retries = 0;

	i2c_put_adapter(adapter);
	if (!hscd_data->this_client) {
		pr_info("can't add i2c device at 0x%x\n",
			(unsigned int)i2c_info.addr);
		rc = -ENOTSUPP;
		}
#ifdef ALPS_DEBUG
pr_info("hscd_open Init end!!!!\n");
#endif

probe_done:
*/
	return rc;
}

static void __exit hscd_exit(void)
{
#ifdef ALPS_DEBUG
	pr_info("[HSCD] exit\n");
#endif
	i2c_del_driver(&hscd_driver);
}

module_init(hscd_init);
module_exit(hscd_exit);

MODULE_DESCRIPTION("Alps hscd Device");
MODULE_AUTHOR("ALPS");
MODULE_LICENSE("GPL v2");
