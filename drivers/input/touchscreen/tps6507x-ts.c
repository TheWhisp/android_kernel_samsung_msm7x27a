/*
<<<<<<< HEAD
<<<<<<< HEAD
 * drivers/input/touchscreen/tps6507x_ts.c
 *
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * Touchscreen driver for the tps6507x chip.
 *
 * Copyright (c) 2009 RidgeRun (todd.fischer@ridgerun.com)
 *
 * Credits:
 *
 *    Using code from tsc2007, MtekVision Co., Ltd.
 *
 * For licencing details see kernel-base/COPYING
 *
 * TPS65070, TPS65073, TPS650731, and TPS650732 support
 * 10 bit touch screen interface.
 */

#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/input.h>
<<<<<<< HEAD
=======
#include <linux/input-polldev.h>
>>>>>>> refs/remotes/origin/master
#include <linux/platform_device.h>
#include <linux/mfd/tps6507x.h>
#include <linux/input/tps6507x-ts.h>
#include <linux/delay.h>

#define TSC_DEFAULT_POLL_PERIOD 30 /* ms */
#define TPS_DEFAULT_MIN_PRESSURE 0x30
#define MAX_10BIT ((1 << 10) - 1)

#define	TPS6507X_ADCONFIG_CONVERT_TS (TPS6507X_ADCONFIG_AD_ENABLE | \
					 TPS6507X_ADCONFIG_START_CONVERSION | \
					 TPS6507X_ADCONFIG_INPUT_REAL_TSC)
#define	TPS6507X_ADCONFIG_POWER_DOWN_TS (TPS6507X_ADCONFIG_INPUT_REAL_TSC)

struct ts_event {
	u16	x;
	u16	y;
	u16	pressure;
};

struct tps6507x_ts {
<<<<<<< HEAD
	struct input_dev	*input_dev;
	struct device		*dev;
	char			phys[32];
	struct delayed_work	work;
	unsigned		polling;	/* polling is active */
	struct ts_event		tc;
	struct tps6507x_dev	*mfd;
	u16			model;
	unsigned		pendown;
	int			irq;
	void			(*clear_penirq)(void);
	unsigned long		poll_period;	/* ms */
	u16			min_pressure;
	int			vref;		/* non-zero to leave vref on */
=======
	struct device		*dev;
	struct input_polled_dev	*poll_dev;
	struct tps6507x_dev	*mfd;
	char			phys[32];
	struct ts_event		tc;
	u16			min_pressure;
	bool			pendown;
>>>>>>> refs/remotes/origin/master
};

static int tps6507x_read_u8(struct tps6507x_ts *tsc, u8 reg, u8 *data)
{
	int err;

	err = tsc->mfd->read_dev(tsc->mfd, reg, 1, data);

	if (err)
		return err;

	return 0;
}

static int tps6507x_write_u8(struct tps6507x_ts *tsc, u8 reg, u8 data)
{
	return tsc->mfd->write_dev(tsc->mfd, reg, 1, &data);
}

static s32 tps6507x_adc_conversion(struct tps6507x_ts *tsc,
				   u8 tsc_mode, u16 *value)
{
	s32 ret;
	u8 adc_status;
	u8 result;

	/* Route input signal to A/D converter */

	ret = tps6507x_write_u8(tsc, TPS6507X_REG_TSCMODE, tsc_mode);
	if (ret) {
		dev_err(tsc->dev, "TSC mode read failed\n");
		goto err;
	}

	/* Start A/D conversion */

	ret = tps6507x_write_u8(tsc, TPS6507X_REG_ADCONFIG,
				TPS6507X_ADCONFIG_CONVERT_TS);
	if (ret) {
		dev_err(tsc->dev, "ADC config write failed\n");
		return ret;
	}

	do {
		ret = tps6507x_read_u8(tsc, TPS6507X_REG_ADCONFIG,
				       &adc_status);
		if (ret) {
			dev_err(tsc->dev, "ADC config read failed\n");
			goto err;
		}
	} while (adc_status & TPS6507X_ADCONFIG_START_CONVERSION);

	ret = tps6507x_read_u8(tsc, TPS6507X_REG_ADRESULT_2, &result);
	if (ret) {
		dev_err(tsc->dev, "ADC result 2 read failed\n");
		goto err;
	}

	*value = (result & TPS6507X_REG_ADRESULT_2_MASK) << 8;

	ret = tps6507x_read_u8(tsc, TPS6507X_REG_ADRESULT_1, &result);
	if (ret) {
		dev_err(tsc->dev, "ADC result 1 read failed\n");
		goto err;
	}

	*value |= result;

	dev_dbg(tsc->dev, "TSC channel %d = 0x%X\n", tsc_mode, *value);

err:
	return ret;
}

/* Need to call tps6507x_adc_standby() after using A/D converter for the
 * touch screen interrupt to work properly.
 */

static s32 tps6507x_adc_standby(struct tps6507x_ts *tsc)
{
	s32 ret;
	s32 loops = 0;
	u8 val;

	ret = tps6507x_write_u8(tsc,  TPS6507X_REG_ADCONFIG,
				TPS6507X_ADCONFIG_INPUT_TSC);
	if (ret)
		return ret;

	ret = tps6507x_write_u8(tsc, TPS6507X_REG_TSCMODE,
				TPS6507X_TSCMODE_STANDBY);
	if (ret)
		return ret;

	ret = tps6507x_read_u8(tsc, TPS6507X_REG_INT, &val);
	if (ret)
		return ret;

	while (val & TPS6507X_REG_TSC_INT) {
		mdelay(10);
		ret = tps6507x_read_u8(tsc, TPS6507X_REG_INT, &val);
		if (ret)
			return ret;
		loops++;
	}

	return ret;
}

<<<<<<< HEAD
static void tps6507x_ts_handler(struct work_struct *work)
{
	struct tps6507x_ts *tsc =  container_of(work,
				struct tps6507x_ts, work.work);
	struct input_dev *input_dev = tsc->input_dev;
	int pendown;
	int schd;
	int poll = 0;
	s32 ret;

	ret =  tps6507x_adc_conversion(tsc, TPS6507X_TSCMODE_PRESSURE,
				       &tsc->tc.pressure);
=======
static void tps6507x_ts_poll(struct input_polled_dev *poll_dev)
{
	struct tps6507x_ts *tsc = poll_dev->private;
	struct input_dev *input_dev = poll_dev->input;
	bool pendown;
	s32 ret;

	ret = tps6507x_adc_conversion(tsc, TPS6507X_TSCMODE_PRESSURE,
				      &tsc->tc.pressure);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto done;

	pendown = tsc->tc.pressure > tsc->min_pressure;

	if (unlikely(!pendown && tsc->pendown)) {
		dev_dbg(tsc->dev, "UP\n");
		input_report_key(input_dev, BTN_TOUCH, 0);
		input_report_abs(input_dev, ABS_PRESSURE, 0);
		input_sync(input_dev);
<<<<<<< HEAD
		tsc->pendown = 0;
=======
		tsc->pendown = false;
>>>>>>> refs/remotes/origin/master
	}

	if (pendown) {

		if (!tsc->pendown) {
			dev_dbg(tsc->dev, "DOWN\n");
			input_report_key(input_dev, BTN_TOUCH, 1);
		} else
			dev_dbg(tsc->dev, "still down\n");

		ret =  tps6507x_adc_conversion(tsc, TPS6507X_TSCMODE_X_POSITION,
					       &tsc->tc.x);
		if (ret)
			goto done;

		ret =  tps6507x_adc_conversion(tsc, TPS6507X_TSCMODE_Y_POSITION,
					       &tsc->tc.y);
		if (ret)
			goto done;

		input_report_abs(input_dev, ABS_X, tsc->tc.x);
		input_report_abs(input_dev, ABS_Y, tsc->tc.y);
		input_report_abs(input_dev, ABS_PRESSURE, tsc->tc.pressure);
		input_sync(input_dev);
<<<<<<< HEAD
		tsc->pendown = 1;
		poll = 1;
	}

done:
	/* always poll if not using interrupts */
	poll = 1;

	if (poll) {
		schd = schedule_delayed_work(&tsc->work,
					msecs_to_jiffies(tsc->poll_period));
		if (schd)
			tsc->polling = 1;
		else {
			tsc->polling = 0;
			dev_err(tsc->dev, "re-schedule failed");
		}
	} else
		tsc->polling = 0;

	ret = tps6507x_adc_standby(tsc);
=======
		tsc->pendown = true;
	}

done:
	tps6507x_adc_standby(tsc);
>>>>>>> refs/remotes/origin/master
}

static int tps6507x_ts_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	int error;
	struct tps6507x_ts *tsc;
	struct tps6507x_dev *tps6507x_dev = dev_get_drvdata(pdev->dev.parent);
	struct touchscreen_init_data *init_data;
	struct input_dev *input_dev;
	struct tps6507x_board *tps_board;
	int schd;

	/**
	 * tps_board points to pmic related constants
	 * coming from the board-evm file.
	 */

	tps_board = (struct tps6507x_board *)tps6507x_dev->dev->platform_data;

	if (!tps_board) {
		dev_err(tps6507x_dev->dev,
			"Could not find tps6507x platform data\n");
		return -EIO;
	}

	/**
	 * init_data points to array of regulator_init structures
	 * coming from the board-evm file.
	 */

=======
	struct tps6507x_dev *tps6507x_dev = dev_get_drvdata(pdev->dev.parent);
	const struct tps6507x_board *tps_board;
	const struct touchscreen_init_data *init_data;
	struct tps6507x_ts *tsc;
	struct input_polled_dev *poll_dev;
	struct input_dev *input_dev;
	int error;

	/*
	 * tps_board points to pmic related constants
	 * coming from the board-evm file.
	 */
	tps_board = dev_get_platdata(tps6507x_dev->dev);
	if (!tps_board) {
		dev_err(tps6507x_dev->dev,
			"Could not find tps6507x platform data\n");
		return -ENODEV;
	}

	/*
	 * init_data points to array of regulator_init structures
	 * coming from the board-evm file.
	 */
>>>>>>> refs/remotes/origin/master
	init_data = tps_board->tps6507x_ts_init_data;

	tsc = kzalloc(sizeof(struct tps6507x_ts), GFP_KERNEL);
	if (!tsc) {
		dev_err(tps6507x_dev->dev, "failed to allocate driver data\n");
<<<<<<< HEAD
		error = -ENOMEM;
		goto err0;
	}

	tps6507x_dev->ts = tsc;
	tsc->mfd = tps6507x_dev;
	tsc->dev = tps6507x_dev->dev;
	input_dev = input_allocate_device();
	if (!input_dev) {
		dev_err(tsc->dev, "Failed to allocate input device.\n");
		error = -ENOMEM;
		goto err1;
	}

=======
		return -ENOMEM;
	}

	tsc->mfd = tps6507x_dev;
	tsc->dev = tps6507x_dev->dev;
	tsc->min_pressure = init_data ?
			init_data->min_pressure : TPS_DEFAULT_MIN_PRESSURE;

	snprintf(tsc->phys, sizeof(tsc->phys),
		 "%s/input0", dev_name(tsc->dev));

	poll_dev = input_allocate_polled_device();
	if (!poll_dev) {
		dev_err(tsc->dev, "Failed to allocate polled input device.\n");
		error = -ENOMEM;
		goto err_free_mem;
	}

	tsc->poll_dev = poll_dev;

	poll_dev->private = tsc;
	poll_dev->poll = tps6507x_ts_poll;
	poll_dev->poll_interval = init_data ?
			init_data->poll_period : TSC_DEFAULT_POLL_PERIOD;

	input_dev = poll_dev->input;
>>>>>>> refs/remotes/origin/master
	input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
	input_dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);

	input_set_abs_params(input_dev, ABS_X, 0, MAX_10BIT, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0, MAX_10BIT, 0, 0);
	input_set_abs_params(input_dev, ABS_PRESSURE, 0, MAX_10BIT, 0, 0);

	input_dev->name = "TPS6507x Touchscreen";
<<<<<<< HEAD
	input_dev->id.bustype = BUS_I2C;
	input_dev->dev.parent = tsc->dev;

	snprintf(tsc->phys, sizeof(tsc->phys),
		 "%s/input0", dev_name(tsc->dev));
	input_dev->phys = tsc->phys;

	dev_dbg(tsc->dev, "device: %s\n", input_dev->phys);

	input_set_drvdata(input_dev, tsc);

	tsc->input_dev = input_dev;

	INIT_DELAYED_WORK(&tsc->work, tps6507x_ts_handler);

	if (init_data) {
		tsc->poll_period = init_data->poll_period;
		tsc->vref = init_data->vref;
		tsc->min_pressure = init_data->min_pressure;
		input_dev->id.vendor = init_data->vendor;
		input_dev->id.product = init_data->product;
		input_dev->id.version = init_data->version;
	} else {
		tsc->poll_period = TSC_DEFAULT_POLL_PERIOD;
		tsc->min_pressure = TPS_DEFAULT_MIN_PRESSURE;
=======
	input_dev->phys = tsc->phys;
	input_dev->dev.parent = tsc->dev;
	input_dev->id.bustype = BUS_I2C;
	if (init_data) {
		input_dev->id.vendor = init_data->vendor;
		input_dev->id.product = init_data->product;
		input_dev->id.version = init_data->version;
>>>>>>> refs/remotes/origin/master
	}

	error = tps6507x_adc_standby(tsc);
	if (error)
<<<<<<< HEAD
		goto err2;

	error = input_register_device(input_dev);
	if (error)
		goto err2;

	schd = schedule_delayed_work(&tsc->work,
				     msecs_to_jiffies(tsc->poll_period));

	if (schd)
		tsc->polling = 1;
	else {
		tsc->polling = 0;
		dev_err(tsc->dev, "schedule failed");
		goto err2;
	 }
	platform_set_drvdata(pdev, tps6507x_dev);

	return 0;

err2:
	cancel_delayed_work_sync(&tsc->work);
	input_free_device(input_dev);
err1:
	kfree(tsc);
	tps6507x_dev->ts = NULL;
err0:
	return error;
}

static int __devexit tps6507x_ts_remove(struct platform_device *pdev)
{
	struct tps6507x_dev *tps6507x_dev = platform_get_drvdata(pdev);
	struct tps6507x_ts *tsc = tps6507x_dev->ts;
	struct input_dev *input_dev = tsc->input_dev;

	cancel_delayed_work_sync(&tsc->work);

	input_unregister_device(input_dev);

	tps6507x_dev->ts = NULL;
=======
		goto err_free_polled_dev;

	error = input_register_polled_device(poll_dev);
	if (error)
		goto err_free_polled_dev;

	platform_set_drvdata(pdev, tsc);

	return 0;

err_free_polled_dev:
	input_free_polled_device(poll_dev);
err_free_mem:
	kfree(tsc);
	return error;
}

static int tps6507x_ts_remove(struct platform_device *pdev)
{
	struct tps6507x_ts *tsc = platform_get_drvdata(pdev);
	struct input_polled_dev *poll_dev = tsc->poll_dev;

	input_unregister_polled_device(poll_dev);
	input_free_polled_device(poll_dev);

>>>>>>> refs/remotes/origin/master
	kfree(tsc);

	return 0;
}

static struct platform_driver tps6507x_ts_driver = {
	.driver = {
		.name = "tps6507x-ts",
		.owner = THIS_MODULE,
	},
	.probe = tps6507x_ts_probe,
<<<<<<< HEAD
	.remove = __devexit_p(tps6507x_ts_remove),
};
<<<<<<< HEAD

static int __init tps6507x_ts_init(void)
{
	return platform_driver_register(&tps6507x_ts_driver);
}
module_init(tps6507x_ts_init);

static void __exit tps6507x_ts_exit(void)
{
	platform_driver_unregister(&tps6507x_ts_driver);
}
module_exit(tps6507x_ts_exit);
=======
module_platform_driver(tps6507x_ts_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.remove = tps6507x_ts_remove,
};
module_platform_driver(tps6507x_ts_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Todd Fischer <todd.fischer@ridgerun.com>");
MODULE_DESCRIPTION("TPS6507x - TouchScreen driver");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
<<<<<<< HEAD
MODULE_ALIAS("platform:tps6507x-tsc");
=======
MODULE_ALIAS("platform:tps6507x-ts");
>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_ALIAS("platform:tps6507x-ts");
>>>>>>> refs/remotes/origin/master
