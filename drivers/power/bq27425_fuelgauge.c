/*
 *  bq27425 fuelgauge
 *  fuel-gauge systems for lithium-ion (Li+) batteries
 *
 *  Copyright (C) 2010 Samsung Electronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/types.h>
#include <linux/pm.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/firmware.h>
#include <linux/wakelock.h>
#include <linux/blkdev.h>
#include <linux/workqueue.h>
#include <linux/rtc.h>
#include <mach/gpio.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mutex.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/power_supply.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/device.h>
#include <linux/gpio.h>


/* TI Standard Commands in Firmware Mode*/
#define CNTL			0x00 /*Control()*/
#define TEMP			0x02 /*Temperature()*/
#define VOLT			0x04 /*Voltage()*/
#define FLAGS			0x06 /*Flags()*/
#define NAC			0x08 /*NominalAvailableCapacity()*/
#define FAC			0x0a /*FullAvailableCapacity()*/
#define RM			0x0c /*emainingCapacity()*/
#define FCC			0x0e /*FullChargeCapacity()*/
#define AI			0x10 /*AverageCurrent()*/
#define SI			0x12 /*StandbyCurrent()*/
#define MLI			0x14 /*MaxLoadCurrent()*/
#define AP			0x18 /*AveragePower()*/
#define SOC			0x1c /*StateOfCharge()*/
#define ITEMP			0x1e /*IntTemperature()*/
#define SOH			0x20 /*StateOfHealth()*/

#define OPCODE_SET		0x3e
#define OPCODE_SET1		0x3f
#define	TEMP_SET		0x40

#define SAMSUNG_OPCODE	0xA1FC

#define ERESET			1

#if defined(CONFIG_MACH_GEIM)
#define FUEL_I2C_SCL 79
#define FUEL_I2C_SDA 78
#else
#define FUEL_I2C_SCL 78
#define FUEL_I2C_SDA 79
#endif


static struct i2c_driver fg_i2c_driver;
static struct i2c_client *fg_i2c_client;

static int is_reset_soc;

int is_attached;

struct fuel_gauge_data {
	struct work_struct work;
};

static int bq27425_write_reg(struct i2c_client *client,
			int reg, u8 *buf)
{
	int ret;

	ret = i2c_smbus_write_i2c_block_data(client, reg, 1, buf);

	if (ret < 0)
		dev_err(&client->dev, "%s: err %d\n", __func__, ret);

	return ret;
}

static int bq27425_write_reg_word(struct i2c_client *client,
				int reg, u8 *buf)
{
	int ret;

	ret = i2c_smbus_write_i2c_block_data(client, reg, 2, buf);

	if (ret < 0)
		dev_err(&client->dev, "%s: err %d\n", __func__, ret);

	return ret;
}

static int bq27425_read_reg(struct i2c_client *client,
			int reg, u8 *buf)
{
	int ret;

	ret = i2c_smbus_read_i2c_block_data(client, reg, 2, buf);

	if (ret < 0)
		dev_err(&client->dev, "%s: err %d\n", __func__, ret);

	return ret;
}

static int bq27425_read_reg_mid(struct i2c_client *client,
			int reg, u8 *buf)
{
	int ret;

	ret = i2c_smbus_read_i2c_block_data(client, reg, 1, buf);

	if (ret < 0)
		dev_err(&client->dev, "%s: err %d\n", __func__, ret);

	return ret;
}

static int bq27425_read32_reg(struct i2c_client *client,
			int reg, u8 *buf)
{
	int ret;

	ret = i2c_smbus_read_i2c_block_data(client, reg, 8, buf);

	if (ret < 0)
		dev_err(&client->dev, "%s: err %d\n", __func__, ret);

	return ret;
}

static int bq27425_set_sealmode(void)
{
	struct i2c_client *client = fg_i2c_client;

	u8 data[2];
	u32 res;
	int ret1, ret2 ;

	data[0] = 0x20;
	data[1] = 0x00;

	pr_info("[BATT]set seal mode\n");

	ret1 = bq27425_write_reg(client, CNTL, &data[0]);
	ret2 = bq27425_write_reg(client, CNTL+1, &data[1]);

	if (ret1 < 0 || ret2 < 0) {
		pr_info("%s[BATT] set sealmode failed\n", __func__);
		return -ERESET;
	}
	/*bq27425_read_reg(client, CNTL, &data);*/

	res = (data[1]&0xff) << 8 + (data[0]&0xff);

	return res;

}

static int bq27425_set_unsealmode(void)
{
	struct i2c_client *client = fg_i2c_client;

	u8 data[2];

	pr_info("[BATT] set unseal mode\n");

	data[0] = 0x14;
	data[1] = 0x04;
	bq27425_write_reg(client, CNTL, &data[0]);
	bq27425_write_reg(client, CNTL+1, &data[1]);
	bq27425_read_reg_mid(client, CNTL, &data[0]);
	bq27425_read_reg_mid(client, CNTL, &data[1]);

	data[0] = 0x72;
	data[1] = 0x36;
	bq27425_write_reg(client, CNTL, &data[0]);
	bq27425_write_reg(client, CNTL+1, &data[1]);
	bq27425_read_reg_mid(client, CNTL, &data[0]);
	bq27425_read_reg_mid(client, CNTL, &data[1]);

	return 1;

}

int bq27425_reset_soc(void)
{
	struct i2c_client *client = fg_i2c_client;

	u8 data[2];
	u8 lsbctrl = 0xff;
	u8 msbctrl = 0xff;

	/* Reduce kernel log*/
	/*pr_info("[bq27425] %s start_reset_soc\n", __func__);*/

	bq27425_set_unsealmode();

	data[0] = 0x41;
	data[1] = 0x00;

	bq27425_write_reg(client, CNTL, &data[0]);
	bq27425_write_reg(client, CNTL+1, &data[1]);
	bq27425_read_reg(client, CNTL, &lsbctrl);
	bq27425_read_reg(client, CNTL, &msbctrl);
	msleep(3000);

	/*pr_info("[bq27425] finish reset soc\n");*/

	return 1;

}

unsigned int bq27425_get_vcell(void)
{
	struct i2c_client *client = fg_i2c_client;

	u32 vcell = 0;
	u8 data[2];

	if (bq27425_read_reg(client, VOLT, data) < 0)
		return -ERESET;

	vcell = ((data[0]) + (data[1]<<8));

	return vcell;
}

unsigned int bq27425_get_soc(void)
{
	struct i2c_client *client = fg_i2c_client;

	u8 data[2];
	int soc;

	bq27425_read_reg(client, SOC, data);

	soc = ((data[0]) + (data[1]<<8));

	if (soc > 100)
		soc = 100;
	if (soc < 0)
		soc = 0;

	return soc;

}

unsigned int bq27425_get_flag(void)
{
	struct i2c_client *client = fg_i2c_client;

	u8 data[2];
	int ret = 0;

	if (bq27425_read_reg(client, FLAGS, data) < 0)
		return -ERESET;

	ret = data[0] & 0x08;

	if (ret == 8)
		return 1;
	else
		return -ERESET;

}

static int bq27425_get_opcode(void)
{
	struct i2c_client *client = fg_i2c_client;

	u8 data[8] = {0,};
	u8 raw_data[2];
	raw_data[0] = 0x52;
	raw_data[1] = 0x00;
	int ret ;
	int i = 0;

	bq27425_write_reg(client, 0x3e, &raw_data[0]);
	bq27425_write_reg(client, 0x3f, &raw_data[1]);
	for (i = 0 ; i < 8 ; i++) {
		bq27425_read_reg_mid(client, TEMP_SET+i, &data[i]);
		pr_info("[BATT]read OPcode data[%d] 0x%x\n",
			i, data[i]);
	}

	ret = (data[6]) + (data[5]<<8);

	return ret;

}

static int bq27425_get_cfgcode(void)
{
	struct i2c_client *client = fg_i2c_client;

	u8 data[8] = {0,};
	u8 raw_data[2];
	raw_data[0] = 0x3a;
	raw_data[1] = 0x00;
	int ret ;
	int i = 0;

	bq27425_write_reg(client, OPCODE_SET, &raw_data[0]);
	bq27425_write_reg(client, OPCODE_SET1, &raw_data[1]);
	for (i = 0 ; i < 2 ; i++)
		bq27425_read_reg_mid(client, TEMP_SET+i, &data[i]);

	pr_info("[BATT]read cfg code data0 0x%x\n", data[0]);
	pr_info("[BATT]read cfg code  data1 0x%x\n", data[1]);

	ret = (data[1]<<8) + (data[0]);

	return ret;

}

static int bq27425_get_cfgcode_seal(void)
{
	struct i2c_client *client = fg_i2c_client;

	u8 data[8] = {0,};
	u8 raw_data[2];
	raw_data[0] = 0x3a;
	raw_data[1] = 0x01;
	int ret ;
	int i = 0;

	bq27425_write_reg(client, OPCODE_SET, &raw_data[0]);
	bq27425_write_reg(client, OPCODE_SET1, &raw_data[1]);
	for (i = 0 ; i < 2 ; i++)
		bq27425_read_reg_mid(client, TEMP_SET+i, &data[i]);

	pr_info("[BATT] seal read cfg code data0 0x%x\n", data[0]);
	pr_info("[BATT]seal read cfg code  data1 0x%x\n", data[1]);

	ret = (data[1]<<8) + (data[0]);

	return ret;

}
int bq27425_soft_reset_soc(void)
{
	struct i2c_client *client = fg_i2c_client;

	u8 data[2];


	pr_info("[bq27425] %s start_soft reset_soc\n", __func__);

	bq27425_set_unsealmode();

	data[0] = 0x42;
	data[1] = 0x00;

	bq27425_write_reg(client, CNTL, &data[0]);
	bq27425_write_reg(client, CNTL+1, &data[1]);

	msleep(3000);

	pr_info("[bq27425] finish soft reset soc\n");

	return 1;

}


unsigned int bq27425_get_fuelalert_code(void)
{
	struct i2c_client *client = fg_i2c_client;

	u8 data[5] = {0,};
	u8 raw_data[2];
	raw_data[0] = 0x31;
	raw_data[1] = 0x00;
	int ret ;
	int i = 0;

	pr_info("[BATT]start read alert code\n");
	bq27425_set_unsealmode();

	bq27425_write_reg(client, OPCODE_SET, &raw_data[0]);
	bq27425_write_reg(client, OPCODE_SET1, &raw_data[1]);
	for (i = 0 ; i < 5 ; i++) {
		bq27425_read_reg_mid(client, TEMP_SET+i, &data[i]);
		pr_info("[BATT]read alert code data[%d] 0x%x\n",
			i, data[i]);
	}

	ret = (data[1]<<8) + (data[0]);

	bq27425_set_sealmode();

	return ret;

}

unsigned int bq27425_get_cutoff_voltage(void)
{
	struct i2c_client *client = fg_i2c_client;

	u8 data[32] = {0,};
	u8 raw_data[2];
	raw_data[0] = 0x52;
	raw_data[1] = 0x00;
	int ret ;
	int i = 0;

	pr_info("[BATT]start read alert code\n");
	bq27425_set_unsealmode();

	bq27425_write_reg(client, OPCODE_SET, &raw_data[0]);
	bq27425_write_reg(client, OPCODE_SET1, &raw_data[1]);
	for (i = 0 ; i < 32 ; i++) {
		bq27425_read_reg_mid(client, TEMP_SET+i, &data[i]);
		pr_info("[BATT]read cutoff code data[%d] 0x%x\n",
			i, data[i]);
	}


	ret = (data[1]<<8) + (data[0]);

	bq27425_set_sealmode();

	return ret;

}

static int bq27425_set_cutoff_voltage(void)
{
	struct i2c_client *client = fg_i2c_client;

	u8 data[32];
	u8 offset[2];
	u8 cntl_data[2];
	u8 ret;
	u8 check_sum = 0x79;
	u8 dtt = 0xff;

	u32 i ;
	data[0] = 0x46;
	data[1] = 0x80;
	data[2] = 0xb4;
	data[3] = 0x00;
	data[4] = 0x00;
	data[5] = 0xa1;
	data[6] = 0xfc;
	data[7] = 0x81;
	data[8] = 0x0e;
	data[9] = 0xda;
	data[10] = 0x0e;
	data[11] = 0x98;
	data[12] = 0x05;
	data[13] = 0x14;
	data[14] = 0x12;
	data[15] = 0xca;
	data[16] = 0x12;
	data[17] = 0x6f;
	data[18] = 0x0d;
	data[19] = 0x16;
	data[20] = 0x00;
	data[21] = 0xc8;
	data[22] = 0x00;
	data[23] = 0x32;
	data[24] = 0x00;
	data[25] = 0x14;
	data[26] = 0x03;
	data[27] = 0xe8;
	data[28] = 0x00;
	data[29] = 0x01;
	data[30] = 0x00;
	data[31] = 0x64;

	offset[0] = 0x52;
	offset[1] = 0x00;

	cntl_data[0] = 0x13;
	cntl_data[1] = 0x00;

	pr_info("%s[BATT]  start write soc1\n", __func__);

	bq27425_set_unsealmode();
	/*Access data flash*/
	bq27425_write_reg(client, CNTL, &cntl_data[0]);
	bq27425_write_reg(client, CNTL+1, &cntl_data[1]);
	/*Prepare to write data*/
	bq27425_write_reg(client, 0x3e, &offset[0]);
	bq27425_write_reg(client, 0x3f, &offset[1]);

	/*Write data to cfg*/
	for (i = 0 ; i < 32 ; i++)
		bq27425_write_reg(client, 0x40 + i, &data[i]);
	/*check sum*/
	ret = bq27425_write_reg(client, 0x60, &check_sum);
	pr_info("%s[BATT]check_sum data %d\n", __func__, ret);
	bq27425_read_reg_mid(client, 0x61, &dtt);

	if (ret == 0x00) {
		pr_info("%s[BATT]sucess write data\n", __func__);
		return 1 ;
	} else {
		pr_info("%s[BATT]fail write data\n", __func__);
	}

	return 0;

}

static int bq27425_set_fuelalert_code(void)
{

	struct i2c_client *client = fg_i2c_client;

	u8 data[5];
	u8 offset[2];
	u8 cntl_data[2];
	u8 ret;
	u8 check_sum = 0xC3;
	u8 sum;
	u8 test;
	u8 dtt = 0xff;

	int i = 0;

	data[0] = 0x01;
	data[1] = 0x05;
	data[2] = 0x00;
	data[3] = 0x04;
	data[4] = 0x32;

	offset[0] = 0x31;
	offset[1] = 0x00;

	cntl_data[0] = 0x13;
	cntl_data[1] = 0x00;

	pr_info("%s[BATT]  start write soc1\n", __func__);

	bq27425_set_unsealmode();
	/*Access data flash*/
	bq27425_write_reg(client, CNTL, &cntl_data[0]);
	bq27425_write_reg(client, CNTL+1, &cntl_data[1]);
	/*Prepare to write data*/
	bq27425_write_reg(client, 0x3e, &offset[0]);
	bq27425_write_reg(client, 0x3f, &offset[1]);

	/*Write data to cfg*/
	bq27425_write_reg(client, 0x40, &data[0]);
	bq27425_write_reg(client, 0x41, &data[1]);
	bq27425_write_reg(client, 0x42, &data[2]);
	bq27425_write_reg(client, 0x43, &data[3]);
	bq27425_write_reg(client, 0x44, &data[4]);
	/*check sum*/
	ret = bq27425_write_reg(client, 0x60, &check_sum);
	bq27425_read_reg_mid(client, 0x61, &dtt);

	if (ret == 0x00) {
		pr_info("%s[BATT]sucess write data\n", __func__);
		return 1 ;
	} else
		pr_info("%s[BATT]fail write data\n", __func__);

	return 0;

}


static int bq27425_set_cfgcode(void)
{

	struct i2c_client *client = fg_i2c_client;

	unsigned char data[2];
	unsigned char offset[2];
	unsigned char cntl_data[2];
	unsigned char check_sum = 0xFA;

	data[0] = 0x02;
	data[1] = 0x03;

	offset[0] = 0x3a;
	offset[1] = 0x00;

	cntl_data[0] = 0x13;
	cntl_data[1] = 0x00;

	/*Access data flash*/
	bq27425_write_reg(client, CNTL, &cntl_data[0]);
	bq27425_write_reg(client, CNTL+1, &cntl_data[1]);

	/*Prepare to write data*/
	bq27425_write_reg(client, 0x3e, &offset[0]);
	bq27425_write_reg(client, 0x3f, &offset[1]);

	/*Write data to cfg*/
	bq27425_write_reg(client, 0x40, &data[0]);
	bq27425_write_reg(client, 0x41, &data[1]);
	/*check sum*/
	bq27425_write_reg(client, 0x60, &check_sum);
	bq27425_read_reg_mid(client, 0x61, &data[0]);

	return 0;

}


int bq27425_get_temperature(void)
{
	struct i2c_client *client = fg_i2c_client;
	u8 data[2];
	int temper = 0;

	if (bq27425_read_reg(client, TEMP, data) < 0)
		return -ERESET;

	temper = (((data[0]) + (data[1]<<8)) / 10) - 273;

	return temper;
}

int bq27425_get_fullcap(void)
{
	struct i2c_client *client = fg_i2c_client;
	u8 data[2];
	int res = 0;

	if (bq27425_read_reg(client, FAC, data) < 0)
		return -ERESET;

	res = ((data[0]) + (data[1]<<8));
	pr_info("[BATT] FullCap %d\n", res);

	return res;

}

int bq27425_get_remcap(void)
{
	struct i2c_client *client = fg_i2c_client;
	u8 data[2];
	int res = 0;

	if (bq27425_read_reg(client, RM, data) < 0)
		return -ERESET;

	res = ((data[0]) + (data[1]<<8));

	pr_info("[BATT] RemCap %d\n", res);

	return res;

}


int bq27425_get_current(void)
{
	struct i2c_client *client = fg_i2c_client;
	char data[2];
	int bq_current ;

	bq27425_read_reg(client, AI, data);

	if(data[1] & 0x80)
		bq_current = -(~((data[0]) + (data[1]<<8)) + 0x01 - 0xffff0000);
	else
		bq_current = ((data[0]) + (data[1]<<8));

	return bq_current;
}

static int bq27425_remove(struct i2c_client *client)
{
	struct bq27425_chip *chip = i2c_get_clientdata(client);

	kfree(chip);

	return 0;
}

#define bq27425_suspend NULL
#define bq27425_resume NULL

static int fuel_gauge_init_client(struct i2c_client *client)
{
	return 0;
}


static int __devinit bq27425_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	struct fuel_gauge_data *mt;
	int err = -1;
	int ret = 0;

	pr_info("%s[BATT] bq27425 probe: fuel_gauge_probe\n", __func__);

	gpio_tlmm_config(GPIO_CFG(FUEL_I2C_SDA , 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(FUEL_I2C_SCL , 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);


	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
		goto exit_check_functionality_failed;

	mt = kzalloc(sizeof(struct fuel_gauge_data), GFP_KERNEL);
	if (!mt) {
		err = -ENOMEM;
		goto exit_alloc_data_failed;
	}

	i2c_set_clientdata(client, mt);
	fuel_gauge_init_client(client);
	fg_i2c_client = client;

	is_attached = 1;

	ret = bq27425_get_cfgcode_seal();
	pr_info("%s [BATT] CFG Write Sucess :%d\n", __func__, ret);

	return 0;

exit_alloc_data_failed:
exit_check_functionality_failed:
	pr_err("%s: Error! (%d)\n", __func__, err);
	return err;
}

static const struct i2c_device_id bq27425_id[] = {
	{"bq27425", 0},
	{}
};
/*MODULE_DEVICE_TABLE(i2c, fg_device_id);*/

static struct i2c_driver fg_i2c_driver = {
	.driver = {
		.name = "bq27425",
		.owner = THIS_MODULE,
	},
	.probe		= bq27425_probe,
	.remove		= bq27425_remove,
	.suspend	= bq27425_suspend,
	.resume		= bq27425_resume,
	.id_table	= bq27425_id,
};

static int __init bq27425_init(void)
{
	int err;

	pr_info("%s[bq27425]bq27425_init entry\n", __func__);

	err = i2c_add_driver(&fg_i2c_driver);
	if (err)
		pr_info("%s[bq27425] driver failed "
		       "(errno = %d)\n", __func__, err);
	else
		pr_info("%s[bq27425] added driver %s, (err: %d)\n",
		       __func__, fg_i2c_driver.driver.name, err);
	return err;

}
/*subsys_initcall(bq27425_init);*/
module_init(bq27425_init);


static void __exit bq27425_exit(void)
{
	i2c_del_driver(&fg_i2c_driver);
}
module_exit(bq27425_exit);

MODULE_DESCRIPTION("BQ27425 Fuel Gauge");
MODULE_LICENSE("GPL");
