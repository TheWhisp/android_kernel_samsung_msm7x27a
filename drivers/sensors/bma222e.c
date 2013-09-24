#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/earlysuspend.h>
#include "sensors_head.h"

#define I2C_RETRY_DELAY	5
#define I2C_RETRIES		1

/* Register Name for BMA222E */
#define BMA222E_XOUT		0x02
#define BMA222E_YOUT		0x04
#define BMA222E_ZOUT		0x06
#define BMA222E_TEMP		0x08
#define BMA222E_REG0B		0x0B
#define BMA222E_REG0A		0x0A
#define BMA222E_REG0F		0X0F
#define BMA222E_REG10		0x10
#define BMA222E_REG11		0x11
/*#define BMA222E_REG14		0x14*/
#define RUNNING_INPUT_EVENT 1
#define STOP_INPUT_EVENT	0
#define SUSPEND_MODE_RETURN 1
static int flagPtr;
/* Bandwidth */
#define BANDWIDTH_07_81		0x08
#define BANDWIDTH_15_63		0x09
#define BANDWIDTH_31_25		0x0A
#define BANDWIDTH_62_50		0x0B
#define BANDWIDTH_125		0x0C
#define BANDWIDTH_250		0x0D
/*add register for calibartion*/
#define BMA222E_EEPROM_CTRL_REG                  0x33
#define BMA222E_OFFSET_CTRL_REG                  0x36
#define BMA222E_OFFSET_PARAMS_REG                0x37
#define BMA222E_OFFSET_FILT_X_REG                0x38
#define BMA222E_OFFSET_FILT_Y_REG                0x39
#define BMA222E_OFFSET_FILT_Z_REG                0x3A

#define BMA222E_EN_FAST_COMP__POS                5
#define BMA222E_EN_FAST_COMP__LEN                2
#define BMA222E_EN_FAST_COMP__MSK                0x60
#define BMA222E_EN_FAST_COMP__REG                BMA222E_OFFSET_CTRL_REG

#define BMA222E_COMP_TARGET_OFFSET_X__POS        1
#define BMA222E_COMP_TARGET_OFFSET_X__LEN        2
#define BMA222E_COMP_TARGET_OFFSET_X__MSK        0x06
#define BMA222E_COMP_TARGET_OFFSET_X__REG        BMA222E_OFFSET_PARAMS_REG

#define BMA222E_COMP_TARGET_OFFSET_Y__POS        3
#define BMA222E_COMP_TARGET_OFFSET_Y__LEN        2
#define BMA222E_COMP_TARGET_OFFSET_Y__MSK        0x18
#define BMA222E_COMP_TARGET_OFFSET_Y__REG        BMA222E_OFFSET_PARAMS_REG

#define BMA222E_COMP_TARGET_OFFSET_Z__POS        5
#define BMA222E_COMP_TARGET_OFFSET_Z__LEN        2
#define BMA222E_COMP_TARGET_OFFSET_Z__MSK        0x60
#define BMA222E_COMP_TARGET_OFFSET_Z__REG        BMA222E_OFFSET_PARAMS_REG

#define BMA222E_FAST_COMP_RDY_S__POS             4
#define BMA222E_FAST_COMP_RDY_S__LEN             1
#define BMA222E_FAST_COMP_RDY_S__MSK             0x10
#define BMA222E_FAST_COMP_RDY_S__REG             BMA222E_OFFSET_CTRL_REG

#define BMA222E_GET_BITSLICE(regvar, bitname)\
	((regvar & bitname##__MSK) >> bitname##__POS)
#define BMA222E_SET_BITSLICE(regvar, bitname, val)\
	((regvar & ~bitname##__MSK) | ((val<<bitname##__POS)&bitname##__MSK))
/** \endcond */

/* SETTING THIS BIT  UNLOCK'S WRITING SETTING REGISTERS TO EEPROM */

#define BMA222E_UNLOCK_EE_WRITE_SETTING__POS     0
#define BMA222E_UNLOCK_EE_WRITE_SETTING__LEN     1
#define BMA222E_UNLOCK_EE_WRITE_SETTING__MSK     0x01
#define BMA222E_UNLOCK_EE_WRITE_SETTING__REG     BMA222E_EEPROM_CTRL_REG


/* SETTING THIS BIT STARTS WRITING SETTING REGISTERS TO EEPROM */

#define BMA222E_START_EE_WRITE_SETTING__POS      1
#define BMA222E_START_EE_WRITE_SETTING__LEN      1
#define BMA222E_START_EE_WRITE_SETTING__MSK      0x02
#define BMA222E_START_EE_WRITE_SETTING__REG      BMA222E_EEPROM_CTRL_REG

/* STATUS OF WRITING TO EEPROM */

#define BMA222E_EE_WRITE_SETTING_S__POS          2
#define BMA222E_EE_WRITE_SETTING_S__LEN          1
#define BMA222E_EE_WRITE_SETTING_S__MSK          0x04
#define BMA222E_EE_WRITE_SETTING_S__REG          BMA222E_EEPROM_CTRL_REG


#define CALIBRATION_FILE_PATH	"/efs/calibration_data"
#define CALIBRATION_DATA_AMOUNT	20

#define CALIBRATION_RUNNING 2
#define CALIBRATION_DATA_HAVE 1
#define CALIBRATION_NONE 0


#define BMA222E_DRIVER_NAME "bma222e"
#define I2C_BMA222E_ADDR (0x18)
#define BMA222E_CHIP_ID 0xF8


#define I2C_BUS_NUMBER	1

/*#define ALPS_DEBUG*/
struct class *bma222e_class;
struct bma222e_data {
	struct i2c_client *this_client;
	struct input_dev *input_dev;
	struct early_suspend early_suspend;
	int cal_data[3];
	int bma222e_state;
};

struct bma222e_data *bma222e_data;

static void bma222e_early_suspend(struct early_suspend *handler);
static void bma222e_early_resume(struct early_suspend *handler);

static struct i2c_driver bma222e_driver;
static struct i2c_client *client_bma222e;

static atomic_t flgEna;
static atomic_t delay;
static int probe_done;

static struct device *bma222e_device;

static int bma222e_i2c_writem(u8 *txData, int length)
{
	int err;
	int tries = 0;
#ifdef ALPS_DEBUG
	int i;
#endif

	struct i2c_msg msg[] = {
		{
		 .addr = bma222e_data->this_client->addr,
		 .flags = 0,
		 .len = length,
		 .buf = txData,
		 },
	};

#ifdef ALPS_DEBUG
	pr_info("[ACC] i2c_writem : ");
	for (i = 0; i < length ; i++)
		pr_info("0X%02X, ", txData[i]);
	pr_info("\n");
#endif

	do {
		err = i2c_transfer(bma222e_data->this_client->adapter, msg, 1);
	} while ((err != 1) && (++tries < I2C_RETRIES));

	if (err != 1) {
		dev_err(&bma222e_data->this_client->adapter->dev, "write transfer error\n");
		err = -EIO;
	} else {
		err = 0;
	}

	return err;
}
static int bma222e_i2c_readm(u8 *rxData, int length)
{
	int err;
	int tries = 0;

	struct i2c_msg msgs[] = {
		{
		 .addr = bma222e_data->this_client->addr,
		 .flags = 0,
		 .len = 1,
		 .buf = rxData,
		 },
		{
		 .addr = bma222e_data->this_client->addr,
		 .flags = I2C_M_RD,
		 .len = length,
		 .buf = rxData,
		 },
	};

	do {
		err = i2c_transfer(bma222e_data->this_client->adapter, msgs, 2);
	} while ((err != 2) && (++tries < I2C_RETRIES));

	if (err != 2) {
		dev_err(&bma222e_data->this_client->adapter->dev, "read transfer error\n");
		err = -EIO;
	} else {
		err = 0;
	}

	return err;
}
static void bma222e_register_init(void)
{
#ifdef ALPS_DEBUG
	pr_info("[ACC] register_init\n");
#endif
}

static int bma222e_read_cal_offset(int *buffer)
{
	u8 buf[2] = {0};
	int err = 0;
	buf[0] = BMA222E_OFFSET_FILT_X_REG;
	err = bma222e_i2c_readm(buf, 1);
	if (err)
		return 1;
	buffer[0] = (int)((u8)buf[0]);
	buf[0] = BMA222E_OFFSET_FILT_Y_REG;
	err = bma222e_i2c_readm(buf, 1);
	if (err)
		return 1;
	buffer[1] = (int)((u8)buf[0]);
	buf[0] = BMA222E_OFFSET_FILT_Z_REG;
	err = bma222e_i2c_readm(buf, 1);
	if (err)
		return 1;
	buffer[2] = (int)((u8)buf[0]);
	return 0;
}

int bma222e_get_acceleration_data(int *xyz)
{
	static int print_i;
	int err = -1;
	int i;
	u8 sx[6];

	sx[0] = BMA222E_XOUT;
	err = bma222e_i2c_readm(sx, 6);
	if (err < 0)
		return err;
	for (i = 0; i < 3; i++) {
		if (bma222e_data->bma222e_state == CALIBRATION_DATA_HAVE) {
			xyz[i] = (int)((s8)sx[2 * i + 1]);
			xyz[i] = (xyz[i] - bma222e_data->cal_data[i]) * 4;
		} else if (bma222e_data->bma222e_state == CALIBRATION_NONE) {
			xyz[i] = (int)((s8)sx[2 * i + 1]);
			xyz[i] = (xyz[i] * 4);
		} else {
			xyz[i] = (int)((s8)sx[2 * i + 1]);
		}
	}
#ifdef ALPS_DEBUG
	print_i++;
	if (print_i > 250) {
		pr_info("[ACC] x:%d, y:%d, z:%d\n",
			xyz[0], xyz[1], xyz[2]);
		print_i = 0;
	}
#endif
	if (flagPtr == STOP_INPUT_EVENT)
		return SUSPEND_MODE_RETURN;
	return err;
}
EXPORT_SYMBOL(bma222e_get_acceleration_data);

void bma222e_activate(int flgatm, int flg, int dtime)
{
	u8 buf[2];

	if (flg != 0)
		flg = 1;

	buf[0] = BMA222E_REG0F;
	buf[1] = 0x03;		/*g-range +/-2g*/
	bma222e_i2c_writem(buf, 2);
	buf[0] = BMA222E_REG10;
	buf[1] = BANDWIDTH_15_63;
	bma222e_i2c_writem(buf, 2);
	buf[0] = BMA222E_REG11;	/*Power modes*/
	if (flg == 0) {
		buf[1] = 0x80; /*sleep*/
	} else {
		if (flagPtr == STOP_INPUT_EVENT)
			buf[1] = 0x80;
		else
			buf[1] = 0x00;
/*	if (dtime <=  10)
		buf[1] = 0x12;	// 6ms
	else if (dtime <=  20)
		buf[1] = 0x14;	// 10ms
	else if (dtime <=  50)
		buf[1] = 0x16;	// 25ms
	else
	buf[1] = 0x18;	// 50ms*/
	}
	bma222e_i2c_writem(buf, 2);

	if (flgatm) {
		atomic_set(&flgEna, flg);
		atomic_set(&delay, dtime);
	}
}
EXPORT_SYMBOL(bma222e_activate);

static ssize_t raw_data_read(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int xyz[3] = {0}; /*count = 0;*/
	bma222e_get_acceleration_data(xyz);
#ifdef ALPS_DEBUG
	pr_info("x: %d,y: %d,z: %d\n",
		xyz[0]/4, xyz[1]/4, xyz[2]/4);
#endif
	return snprintf(buf, PAGE_SIZE, "%d,%d,%d\n",
		xyz[0]/4, xyz[1]/4, xyz[2]/4);
}

static ssize_t raw_data_write(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	pr_info("raw_data_write is work");
	return size;
}

static int set_offset(int cmd_offset_type)
{
	u8 buf[2] = {0};
	int err = -1;

	buf[0] = BMA222E_OFFSET_PARAMS_REG;
	err = bma222e_i2c_readm(buf, 1);
	if (err)
		return 1;

	switch (cmd_offset_type) {
	case 1:
		buf[1] = BMA222E_SET_BITSLICE(buf[0],
			BMA222E_COMP_TARGET_OFFSET_X, 0);
		break;
	case 2:
		buf[1] = BMA222E_SET_BITSLICE(buf[0],
			BMA222E_COMP_TARGET_OFFSET_Y, 0);
		break;
	case 3:
#if defined(CONFIG_MACH_GEIM)
			buf[1] = BMA222E_SET_BITSLICE(buf[0],
				BMA222E_COMP_TARGET_OFFSET_Z, 2);
#else
			buf[1] = BMA222E_SET_BITSLICE(buf[0],
				BMA222E_COMP_TARGET_OFFSET_Z, 1);
#endif
		break;
	default:
			return 1;
	}

	buf[0] = BMA222E_OFFSET_PARAMS_REG;
	err = bma222e_i2c_writem(buf, 2);
	if (err)
		return 1;

	buf[0] = BMA222E_EN_FAST_COMP__REG;
	err = bma222e_i2c_readm(buf, 1);
	if (err)
		return 1;

	buf[1] = BMA222E_SET_BITSLICE(buf[0],
		BMA222E_EN_FAST_COMP, cmd_offset_type);
	buf[0] = BMA222E_EN_FAST_COMP__REG;
	err = bma222e_i2c_writem(buf, 2);
	if (err)
		return 1;

	err = 0;
	do {

		buf[0] = BMA222E_OFFSET_CTRL_REG;
		err = bma222e_i2c_readm(buf, 1);
		if (err)
			return 1;
		buf[1] = BMA222E_GET_BITSLICE(buf[0], BMA222E_FAST_COMP_RDY_S);
		if (err++ > 20)
			return 1;
	} while (buf[1] == 0);
	return 0;
}

static int bma222e_calibration(int is_cal_erase)
{
	int xyz[3], i, err = 0;
	int sum[3] = {0};
	mm_segment_t old_fs;
	struct file *cal_filp = NULL;

	bma222e_data->bma222e_state = CALIBRATION_RUNNING;

	if (is_cal_erase) {
		xyz[0] = 0;
		xyz[1] = 0;
		xyz[2] = 0;
	} else {
		for (i = 0 ; i < CALIBRATION_DATA_AMOUNT ; i++) {
			bma222e_get_acceleration_data(xyz);
			sum[0] += xyz[0];
			sum[1] += xyz[1];
			sum[2] += xyz[2];
		}
		bma222e_data->cal_data[0] = (sum[0] / CALIBRATION_DATA_AMOUNT);
		bma222e_data->cal_data[1] = (sum[1] / CALIBRATION_DATA_AMOUNT);
		if (sum[2] > 0)
			bma222e_data->cal_data[2]
			= (sum[2] / CALIBRATION_DATA_AMOUNT - 64);
		else
			bma222e_data->cal_data[2]
			= (sum[2] / CALIBRATION_DATA_AMOUNT + 64);
		msleep(20);
	}
#ifdef ALPS_DEBUG
	pr_info("[ACC] %s: cal data (%d,%d,%d)\n",
		__func__, bma222e_data->cal_data[0],
		bma222e_data->cal_data[1],
		bma222e_data->cal_data[2]);
#endif
	old_fs = get_fs();
	set_fs(KERNEL_DS);

	cal_filp = filp_open(CALIBRATION_FILE_PATH,
		O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (IS_ERR(cal_filp)) {
		pr_err("[ACC] %s: Can't open calibration file\n",
			__func__);
		set_fs(old_fs);
		err = PTR_ERR(cal_filp);
		return err;
	}

	err = cal_filp->f_op->write(cal_filp,
		(char *)&bma222e_data->cal_data,
		3 * sizeof(int), &cal_filp->f_pos);
	if (err != 3 * sizeof(int)) {
		pr_err("[ACC] %s: Can't write the cal data to file\n",
			__func__);
		err = -EIO;
	}

	if (err < 0)
		bma222e_data->bma222e_state = CALIBRATION_NONE;
	else
		bma222e_data->bma222e_state = CALIBRATION_DATA_HAVE;
	filp_close(cal_filp, current->files);
	set_fs(old_fs);

	return 0;
}

int bma222e_open()
{
	int err = 0, i;
	mm_segment_t old_fs;
	struct file *cal_filp = NULL;

	if (probe_done == PROBE_SUCCESS) {
		old_fs = get_fs();
		set_fs(KERNEL_DS);

		cal_filp = filp_open(CALIBRATION_FILE_PATH,
			O_RDONLY, 0666);
		if (IS_ERR(cal_filp)) {
			pr_err("[ACC] %s: Can't open calibration file\n",
				__func__);
			set_fs(old_fs);
			err = PTR_ERR(cal_filp);
			return err;
		}
		err = cal_filp->f_op->read(cal_filp,
			(char *)&bma222e_data->cal_data,
			3 * sizeof(int), &cal_filp->f_pos);

		if (err != 3 * sizeof(int)) {
			pr_err("[ACC] %s: Can't read the cal data from file\n",
				__func__);
			filp_close(cal_filp, current->files);
			set_fs(old_fs);
			return -EIO;
		}
#ifdef ALPS_DEBUG
		pr_info("[ACC] %s: (%d,%d,%d)\n", __func__,
			bma222e_data->cal_data[0],
			bma222e_data->cal_data[1],
			bma222e_data->cal_data[2]);
#endif
		for (i = 0 ; i < 3 ; i++) {
			if (bma222e_data->cal_data[i] < -128 ||
				128 < bma222e_data->cal_data[i]) {

				filp_close(cal_filp, current->files);
				bma222e_data->bma222e_state = CALIBRATION_NONE;
				return err = -EIO;
			}
		}
		if (bma222e_data->cal_data[0] == 0
			&& bma222e_data->cal_data[1] == 0
			&& bma222e_data->cal_data[2] == 0)
			bma222e_data->bma222e_state = CALIBRATION_NONE;
		else
			bma222e_data->bma222e_state = CALIBRATION_DATA_HAVE;
		filp_close(cal_filp, current->files);
		set_fs(old_fs);
	}
	return probe_done;
}
EXPORT_SYMBOL(bma222e_open);

static ssize_t cal_offset_read(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int err = -1;
	int xyz[3] = {0};
	err = bma222e_read_cal_offset(xyz);
	if (err != 0) {
		pr_info("bma222e_read_cal_offset fail.\n");
		return snprintf(buf, PAGE_SIZE, "%d\n", err);
	}
#ifdef ALPS_DEBUG
	pr_info("x: %d,y: %d,z: %d\n", xyz[0], xyz[1], xyz[2]);
#endif
	return snprintf(buf, PAGE_SIZE, "%d\n", 1);
}
static ssize_t cal_offset_write(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	int err = -1;
	/*int data[3] = {0};*/
	err = bma222e_calibration(0);
	if (err != 0)
		pr_info("bma222e_calibration fail.\n");
	else
		pr_info("bma222e_calibration success.\n");
	snprintf(buf, PAGE_SIZE, "%d\n", err);
	return size;
}
static DEVICE_ATTR(raw_data, S_IRUGO | S_IWUSR | S_IWGRP,
	raw_data_read, raw_data_write);
static DEVICE_ATTR(calibration, S_IRUGO | S_IWUSR | S_IWGRP,
	cal_offset_read, cal_offset_write);
static struct device_attribute *bma222e_attrs[] = {
	&dev_attr_raw_data,
	&dev_attr_calibration,
	NULL,
};
static ssize_t bma222e_enable_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", 1);
}
static struct device_attribute dev_attr_enable =
	__ATTR(enable, S_IRUGO | S_IWUSR | S_IWGRP,
	       bma222e_enable_show, NULL);
static struct attribute *bma222e_sysfs_attrs[] = {
	&dev_attr_enable.attr,
	NULL
};
static struct attribute_group bma222e_attribute_group = {
	.attrs = bma222e_sysfs_attrs,
};

static int bma222e_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	/*struct input_dev *input_dev;*/
	struct bma222e_data *bma222e;
	int err;
	int d[3];
	u8 buf[2] = {0};

	pr_info("[ACC] probe\n");

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->adapter->dev, "client not i2c capable\n");
		return -ENOMEM;
	}

	bma222e = kzalloc(sizeof(struct bma222e_data), GFP_KERNEL);
	if (!bma222e) {
		dev_err(&client->dev,
			"failed to allocate memory for module data\n");
		err = -ENOMEM;
		goto exit_bma222e_alloc_data_failed;
	}

	i2c_set_clientdata(client, bma222e->this_client);

	bma222e->this_client = client;
	bma222e_data = bma222e;
	bma222e_register_init();

	dev_info(&client->adapter->dev, "detected accelerometer\n");

	buf[0] = 0x00;
	err = bma222e_i2c_readm(buf, 1);
	if (err) {
		pr_info("[ACC] read error\n");
		err = -ENOMEM;
		goto exit_bma222e_input_register_device;
	} else {
#ifdef ALPS_DEBUG
		pr_info("[ACC] chip ID = %x\n", buf[0]);
#endif
		if (buf[0] != BMA222E_CHIP_ID)
			goto exit_bma222e_input_register_device;

	}

	bma222e_activate(0, 1, atomic_read(&delay));
	bma222e_get_acceleration_data(d);
#ifdef ALPS_DEBUG
	pr_info("[ACC] x:%d y:%d z:%d\n", d[0], d[1], d[2]);
#endif
	bma222e_activate(0, 0, atomic_read(&delay));
	flagPtr = RUNNING_INPUT_EVENT;

	sensors_register(bma222e_device,
		NULL, bma222e_attrs,
		"accelerometer_sensor");
	bma222e->input_dev = input_allocate_device();
	if (!bma222e->input_dev) {
		pr_info("%s: could not allocate input device\n", __func__);
		return -ENOMEM;
	}
	err = input_register_device(bma222e->input_dev);
	if (err < 0) {
		pr_info("%s: could not register input device\n", __func__);
		input_free_device(bma222e->input_dev);
		goto exit_bma222e_input_register_device;
	}

	bma222e->input_dev->name = "accelerometer_sensor";
	err = sysfs_create_group(&bma222e->input_dev->dev.kobj,
						&bma222e_attribute_group);
	if (err) {
		pr_info("Creating bh1721 attribute group failed");
		goto exit_bma222e_device;
	}

	bma222e->early_suspend.suspend = bma222e_early_suspend;
	bma222e->early_suspend.resume = bma222e_early_resume;
	register_early_suspend(&bma222e->early_suspend);

	probe_done = PROBE_SUCCESS;
	return 0;

exit_bma222e_device:
	pr_info("exit_bma222e_device\n");
	input_unregister_device(bma222e->input_dev);
exit_bma222e_input_register_device:
	pr_info("exit_bma222e_input_register_device\n");
exit_bma222e_alloc_client_failed:
	pr_info("exit_bma222e_alloc_client_failed\n");
	kfree(bma222e);
exit_bma222e_alloc_data_failed:
	pr_info("exit_bma222e_alloc_data_failed\n");

	return err;
}

static void bma222e_shutdown(struct i2c_client *client)
{
#ifdef ALPS_DEBUG
	pr_info("[ACC] shutdown\n");
#endif
	bma222e_activate(0, 0, atomic_read(&delay));
}

static int bma222e_suspend(struct i2c_client *client, pm_message_t mesg)
{
#ifdef ALPS_DEBUG
	pr_info("[ACC] suspend\n");
#endif
	bma222e_activate(0, 0, atomic_read(&delay));
	flagPtr = STOP_INPUT_EVENT;
	return 0;
}

static int bma222e_resume(struct i2c_client *client)
{
#ifdef ALPS_DEBUG
	pr_info("[ACC] resume\n");
#endif
	flagPtr = RUNNING_INPUT_EVENT;

	bma222e_activate(0, atomic_read(&flgEna), atomic_read(&delay));

	return 0;
}

static void bma222e_early_suspend(struct early_suspend *handler)
{
	struct bma222e_data *bma222e;
	bma222e = container_of(handler, struct bma222e_data, early_suspend);

	pr_info("%s\n", __func__);

	bma222e_suspend(bma222e->this_client, PMSG_SUSPEND);
}

static void bma222e_early_resume(struct early_suspend *handler)
{
	struct bma222e_data *bma222e;
	bma222e = container_of(handler, struct bma222e_data, early_suspend);

	pr_info("%s\n", __func__);

	bma222e_resume(bma222e->this_client);
}

static const struct i2c_device_id bma222e_id[] = {
	{ BMA222E_DRIVER_NAME, 0 },
	{ }
};

static struct i2c_driver bma222e_driver = {
	.probe     = bma222e_probe,
	.id_table  = bma222e_id,
	.driver    = {
	.name	= BMA222E_DRIVER_NAME,
	},
	.shutdown	= bma222e_shutdown,
	/*.suspend	= bma222_suspend,*/
	/*.resume	= bma222_resume,*/
};

static int __init bma222e_init(void)
{
/*	struct i2c_board_info i2c_info;
	struct i2c_adapter *adapter;*/
	int rc = 0;
	probe_done = PROBE_FAIL;

#ifdef ALPS_DEBUG
	pr_info("[ACC] init\n");
#endif
	atomic_set(&flgEna, 0);
	atomic_set(&delay, 200);

	rc = i2c_add_driver(&bma222e_driver);
	if (rc != 0) {
		pr_info("can't add i2c driver\n");
		rc = -ENOTSUPP;
		return rc;
	}
/*
	memset(&i2c_info, 0, sizeof(struct i2c_board_info));
	i2c_info.addr = I2C_BMA222_ADDR;
	strlcpy(i2c_info.type, BMA222_DRIVER_NAME , I2C_NAME_SIZE);

	adapter = i2c_get_adapter(I2C_BUS_NUMBER);
	if (!adapter) {
		pr_info("can't get i2c adapter %d\n", I2C_BUS_NUMBER);
		rc = -ENOTSUPP;
		goto probe_done;
	}


	client_bma222 = i2c_new_device(adapter, &i2c_info);
	client_bma222->adapter->timeout = 0;
	client_bma222->adapter->retries = 0;


	i2c_put_adapter(adapter);
	if (!bma222_data->this_client) {
		pr_info("can't add i2c device at 0x%x\n",
			(unsigned int)i2c_info.addr);
		rc = -ENOTSUPP;
	}

#ifdef ALPS_DEBUG
	pr_info("bma222_open end !!!!\n");
#endif

probe_done:
*/
	return rc;
}

static void __exit bma222e_exit(void)
{
#ifdef ALPS_DEBUG
	pr_info("[ACC] exit\n");
#endif
	i2c_del_driver(&bma222e_driver);
}

module_init(bma222e_init);
module_exit(bma222e_exit);

MODULE_DESCRIPTION("Alps acc Device");
MODULE_AUTHOR("ALPS");
MODULE_LICENSE("GPL v2");
