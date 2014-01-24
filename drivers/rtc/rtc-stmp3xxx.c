/*
 * Freescale STMP37XX/STMP378X Real Time Clock driver
 *
 * Copyright (c) 2007 Sigmatel, Inc.
 * Peter Hartley, <peter.hartley@sigmatel.com>
 *
 * Copyright 2008 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2008 Embedded Alley Solutions, Inc All Rights Reserved.
<<<<<<< HEAD
<<<<<<< HEAD
=======
 * Copyright 2011 Wolfram Sang, Pengutronix e.K.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright 2011 Wolfram Sang, Pengutronix e.K.
>>>>>>> refs/remotes/origin/master
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */
#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/io.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/rtc.h>
#include <linux/slab.h>

<<<<<<< HEAD
#include <mach/platform.h>
#include <mach/stmp3xxx.h>
#include <mach/regs-rtc.h>

struct stmp3xxx_rtc_data {
	struct rtc_device *rtc;
	unsigned irq_count;
	void __iomem *io;
	int irq_alarm, irq_1msec;
=======
#include <mach/common.h>
=======
#include <linux/io.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/rtc.h>
#include <linux/slab.h>
#include <linux/of_device.h>
#include <linux/of.h>
#include <linux/stmp_device.h>
#include <linux/stmp3xxx_rtc_wdt.h>
>>>>>>> refs/remotes/origin/master

#define STMP3XXX_RTC_CTRL			0x0
#define STMP3XXX_RTC_CTRL_SET			0x4
#define STMP3XXX_RTC_CTRL_CLR			0x8
#define STMP3XXX_RTC_CTRL_ALARM_IRQ_EN		0x00000001
#define STMP3XXX_RTC_CTRL_ONEMSEC_IRQ_EN	0x00000002
#define STMP3XXX_RTC_CTRL_ALARM_IRQ		0x00000004
<<<<<<< HEAD
=======
#define STMP3XXX_RTC_CTRL_WATCHDOGEN		0x00000010
>>>>>>> refs/remotes/origin/master

#define STMP3XXX_RTC_STAT			0x10
#define STMP3XXX_RTC_STAT_STALE_SHIFT		16
#define STMP3XXX_RTC_STAT_RTC_PRESENT		0x80000000

#define STMP3XXX_RTC_SECONDS			0x30

#define STMP3XXX_RTC_ALARM			0x40

<<<<<<< HEAD
=======
#define STMP3XXX_RTC_WATCHDOG			0x50

>>>>>>> refs/remotes/origin/master
#define STMP3XXX_RTC_PERSISTENT0		0x60
#define STMP3XXX_RTC_PERSISTENT0_SET		0x64
#define STMP3XXX_RTC_PERSISTENT0_CLR		0x68
#define STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE_EN	0x00000002
#define STMP3XXX_RTC_PERSISTENT0_ALARM_EN	0x00000004
#define STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE	0x00000080

<<<<<<< HEAD
=======
#define STMP3XXX_RTC_PERSISTENT1		0x70
/* missing bitmask in headers */
#define STMP3XXX_RTC_PERSISTENT1_FORCE_UPDATER	0x80000000

>>>>>>> refs/remotes/origin/master
struct stmp3xxx_rtc_data {
	struct rtc_device *rtc;
	void __iomem *io;
	int irq_alarm;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
};

static void stmp3xxx_wait_time(struct stmp3xxx_rtc_data *rtc_data)
{
	/*
	 * The datasheet doesn't say which way round the
	 * NEW_REGS/STALE_REGS bitfields go. In fact it's 0x1=P0,
	 * 0x2=P1, .., 0x20=P5, 0x40=ALARM, 0x80=SECONDS
	 */
<<<<<<< HEAD
	while (__raw_readl(rtc_data->io + HW_RTC_STAT) &
			BF(0x80, RTC_STAT_STALE_REGS))
=======
	while (readl(rtc_data->io + STMP3XXX_RTC_STAT) &
			(0x80 << STMP3XXX_RTC_STAT_STALE_SHIFT))
>>>>>>> refs/remotes/origin/cm-10.0
		cpu_relax();
=======
};

#if IS_ENABLED(CONFIG_STMP3XXX_RTC_WATCHDOG)
/**
 * stmp3xxx_wdt_set_timeout - configure the watchdog inside the STMP3xxx RTC
 * @dev: the parent device of the watchdog (= the RTC)
 * @timeout: the desired value for the timeout register of the watchdog.
 *           0 disables the watchdog
 *
 * The watchdog needs one register and two bits which are in the RTC domain.
 * To handle the resource conflict, the RTC driver will create another
 * platform_device for the watchdog driver as a child of the RTC device.
 * The watchdog driver is passed the below accessor function via platform_data
 * to configure the watchdog. Locking is not needed because accessing SET/CLR
 * registers is atomic.
 */

static void stmp3xxx_wdt_set_timeout(struct device *dev, u32 timeout)
{
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev);

	if (timeout) {
		writel(timeout, rtc_data->io + STMP3XXX_RTC_WATCHDOG);
		writel(STMP3XXX_RTC_CTRL_WATCHDOGEN,
		       rtc_data->io + STMP3XXX_RTC_CTRL + STMP_OFFSET_REG_SET);
		writel(STMP3XXX_RTC_PERSISTENT1_FORCE_UPDATER,
		       rtc_data->io + STMP3XXX_RTC_PERSISTENT1 + STMP_OFFSET_REG_SET);
	} else {
		writel(STMP3XXX_RTC_CTRL_WATCHDOGEN,
		       rtc_data->io + STMP3XXX_RTC_CTRL + STMP_OFFSET_REG_CLR);
		writel(STMP3XXX_RTC_PERSISTENT1_FORCE_UPDATER,
		       rtc_data->io + STMP3XXX_RTC_PERSISTENT1 + STMP_OFFSET_REG_CLR);
	}
}

static struct stmp3xxx_wdt_pdata wdt_pdata = {
	.wdt_set_timeout = stmp3xxx_wdt_set_timeout,
};

static void stmp3xxx_wdt_register(struct platform_device *rtc_pdev)
{
	struct platform_device *wdt_pdev =
		platform_device_alloc("stmp3xxx_rtc_wdt", rtc_pdev->id);

	if (wdt_pdev) {
		wdt_pdev->dev.parent = &rtc_pdev->dev;
		wdt_pdev->dev.platform_data = &wdt_pdata;
		platform_device_add(wdt_pdev);
	}
}
#else
static void stmp3xxx_wdt_register(struct platform_device *rtc_pdev)
{
}
#endif /* CONFIG_STMP3XXX_RTC_WATCHDOG */

static int stmp3xxx_wait_time(struct stmp3xxx_rtc_data *rtc_data)
{
	int timeout = 5000; /* 3ms according to i.MX28 Ref Manual */
	/*
	 * The i.MX28 Applications Processor Reference Manual, Rev. 1, 2010
	 * states:
	 * | The order in which registers are updated is
	 * | Persistent 0, 1, 2, 3, 4, 5, Alarm, Seconds.
	 * | (This list is in bitfield order, from LSB to MSB, as they would
	 * | appear in the STALE_REGS and NEW_REGS bitfields of the HW_RTC_STAT
	 * | register. For example, the Seconds register corresponds to
	 * | STALE_REGS or NEW_REGS containing 0x80.)
	 */
	do {
		if (!(readl(rtc_data->io + STMP3XXX_RTC_STAT) &
				(0x80 << STMP3XXX_RTC_STAT_STALE_SHIFT)))
			return 0;
		udelay(1);
	} while (--timeout > 0);
	return (readl(rtc_data->io + STMP3XXX_RTC_STAT) &
		(0x80 << STMP3XXX_RTC_STAT_STALE_SHIFT)) ? -ETIME : 0;
>>>>>>> refs/remotes/origin/master
}

/* Time read/write */
static int stmp3xxx_rtc_gettime(struct device *dev, struct rtc_time *rtc_tm)
{
<<<<<<< HEAD
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev);

	stmp3xxx_wait_time(rtc_data);
<<<<<<< HEAD
	rtc_time_to_tm(__raw_readl(rtc_data->io + HW_RTC_SECONDS), rtc_tm);
=======
	rtc_time_to_tm(readl(rtc_data->io + STMP3XXX_RTC_SECONDS), rtc_tm);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int ret;
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev);

	ret = stmp3xxx_wait_time(rtc_data);
	if (ret)
		return ret;

	rtc_time_to_tm(readl(rtc_data->io + STMP3XXX_RTC_SECONDS), rtc_tm);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int stmp3xxx_rtc_set_mmss(struct device *dev, unsigned long t)
{
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev);

<<<<<<< HEAD
<<<<<<< HEAD
	__raw_writel(t, rtc_data->io + HW_RTC_SECONDS);
=======
	writel(t, rtc_data->io + STMP3XXX_RTC_SECONDS);
>>>>>>> refs/remotes/origin/cm-10.0
	stmp3xxx_wait_time(rtc_data);
	return 0;
=======
	writel(t, rtc_data->io + STMP3XXX_RTC_SECONDS);
	return stmp3xxx_wait_time(rtc_data);
>>>>>>> refs/remotes/origin/master
}

/* interrupt(s) handler */
static irqreturn_t stmp3xxx_rtc_interrupt(int irq, void *dev_id)
{
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev_id);
<<<<<<< HEAD
<<<<<<< HEAD
	u32 status;
	u32 events = 0;

	status = __raw_readl(rtc_data->io + HW_RTC_CTRL) &
			(BM_RTC_CTRL_ALARM_IRQ | BM_RTC_CTRL_ONEMSEC_IRQ);

	if (status & BM_RTC_CTRL_ALARM_IRQ) {
		stmp3xxx_clearl(BM_RTC_CTRL_ALARM_IRQ,
				rtc_data->io + HW_RTC_CTRL);
		events |= RTC_AF | RTC_IRQF;
	}

	if (status & BM_RTC_CTRL_ONEMSEC_IRQ) {
		stmp3xxx_clearl(BM_RTC_CTRL_ONEMSEC_IRQ,
				rtc_data->io + HW_RTC_CTRL);
		if (++rtc_data->irq_count % 1000 == 0) {
			events |= RTC_UF | RTC_IRQF;
			rtc_data->irq_count = 0;
		}
	}

	if (events)
		rtc_update_irq(rtc_data->rtc, 1, events);

	return IRQ_HANDLED;
=======
=======
>>>>>>> refs/remotes/origin/master
	u32 status = readl(rtc_data->io + STMP3XXX_RTC_CTRL);

	if (status & STMP3XXX_RTC_CTRL_ALARM_IRQ) {
		writel(STMP3XXX_RTC_CTRL_ALARM_IRQ,
				rtc_data->io + STMP3XXX_RTC_CTRL_CLR);
		rtc_update_irq(rtc_data->rtc, 1, RTC_AF | RTC_IRQF);
		return IRQ_HANDLED;
	}

	return IRQ_NONE;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static int stmp3xxx_alarm_irq_enable(struct device *dev, unsigned int enabled)
{
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	void __iomem *p = rtc_data->io + HW_RTC_PERSISTENT0,
		     *ctl = rtc_data->io + HW_RTC_CTRL;

	if (enabled) {
		stmp3xxx_setl(BM_RTC_PERSISTENT0_ALARM_EN |
			      BM_RTC_PERSISTENT0_ALARM_WAKE_EN, p);
		stmp3xxx_setl(BM_RTC_CTRL_ALARM_IRQ_EN, ctl);
	} else {
		stmp3xxx_clearl(BM_RTC_PERSISTENT0_ALARM_EN |
			      BM_RTC_PERSISTENT0_ALARM_WAKE_EN, p);
		stmp3xxx_clearl(BM_RTC_CTRL_ALARM_IRQ_EN, ctl);
=======
=======
>>>>>>> refs/remotes/origin/master

	if (enabled) {
		writel(STMP3XXX_RTC_PERSISTENT0_ALARM_EN |
				STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE_EN,
				rtc_data->io + STMP3XXX_RTC_PERSISTENT0_SET);
		writel(STMP3XXX_RTC_CTRL_ALARM_IRQ_EN,
				rtc_data->io + STMP3XXX_RTC_CTRL_SET);
	} else {
		writel(STMP3XXX_RTC_PERSISTENT0_ALARM_EN |
				STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE_EN,
				rtc_data->io + STMP3XXX_RTC_PERSISTENT0_CLR);
		writel(STMP3XXX_RTC_CTRL_ALARM_IRQ_EN,
				rtc_data->io + STMP3XXX_RTC_CTRL_CLR);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	return 0;
}

static int stmp3xxx_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alm)
{
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev);

<<<<<<< HEAD
<<<<<<< HEAD
	rtc_time_to_tm(__raw_readl(rtc_data->io + HW_RTC_ALARM), &alm->time);
=======
	rtc_time_to_tm(readl(rtc_data->io + STMP3XXX_RTC_ALARM), &alm->time);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rtc_time_to_tm(readl(rtc_data->io + STMP3XXX_RTC_ALARM), &alm->time);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int stmp3xxx_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alm)
{
	unsigned long t;
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev);

	rtc_tm_to_time(&alm->time, &t);
<<<<<<< HEAD
<<<<<<< HEAD
	__raw_writel(t, rtc_data->io + HW_RTC_ALARM);
=======
=======
>>>>>>> refs/remotes/origin/master
	writel(t, rtc_data->io + STMP3XXX_RTC_ALARM);

	stmp3xxx_alarm_irq_enable(dev, alm->enabled);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct rtc_class_ops stmp3xxx_rtc_ops = {
	.alarm_irq_enable =
			  stmp3xxx_alarm_irq_enable,
	.read_time	= stmp3xxx_rtc_gettime,
	.set_mmss	= stmp3xxx_rtc_set_mmss,
	.read_alarm	= stmp3xxx_rtc_read_alarm,
	.set_alarm	= stmp3xxx_rtc_set_alarm,
};

static int stmp3xxx_rtc_remove(struct platform_device *pdev)
{
	struct stmp3xxx_rtc_data *rtc_data = platform_get_drvdata(pdev);

	if (!rtc_data)
		return 0;

<<<<<<< HEAD
<<<<<<< HEAD
	stmp3xxx_clearl(BM_RTC_CTRL_ONEMSEC_IRQ_EN | BM_RTC_CTRL_ALARM_IRQ_EN,
			rtc_data->io + HW_RTC_CTRL);
	free_irq(rtc_data->irq_alarm, &pdev->dev);
	free_irq(rtc_data->irq_1msec, &pdev->dev);
	rtc_device_unregister(rtc_data->rtc);
=======
	writel(STMP3XXX_RTC_CTRL_ALARM_IRQ_EN,
			rtc_data->io + STMP3XXX_RTC_CTRL_CLR);
	free_irq(rtc_data->irq_alarm, &pdev->dev);
	rtc_device_unregister(rtc_data->rtc);
	platform_set_drvdata(pdev, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
	iounmap(rtc_data->io);
	kfree(rtc_data);
=======
	writel(STMP3XXX_RTC_CTRL_ALARM_IRQ_EN,
			rtc_data->io + STMP3XXX_RTC_CTRL_CLR);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int stmp3xxx_rtc_probe(struct platform_device *pdev)
{
	struct stmp3xxx_rtc_data *rtc_data;
	struct resource *r;
	int err;

<<<<<<< HEAD
	rtc_data = kzalloc(sizeof *rtc_data, GFP_KERNEL);
=======
	rtc_data = devm_kzalloc(&pdev->dev, sizeof(*rtc_data), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!rtc_data)
		return -ENOMEM;

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r) {
		dev_err(&pdev->dev, "failed to get resource\n");
<<<<<<< HEAD
		err = -ENXIO;
		goto out_free;
	}

	rtc_data->io = ioremap(r->start, resource_size(r));
	if (!rtc_data->io) {
		dev_err(&pdev->dev, "ioremap failed\n");
		err = -EIO;
		goto out_free;
	}

	rtc_data->irq_alarm = platform_get_irq(pdev, 0);
<<<<<<< HEAD
	rtc_data->irq_1msec = platform_get_irq(pdev, 1);

	if (!(__raw_readl(HW_RTC_STAT + rtc_data->io) &
			BM_RTC_STAT_RTC_PRESENT)) {
=======

	if (!(readl(STMP3XXX_RTC_STAT + rtc_data->io) &
			STMP3XXX_RTC_STAT_RTC_PRESENT)) {
>>>>>>> refs/remotes/origin/cm-10.0
		dev_err(&pdev->dev, "no device onboard\n");
		err = -ENODEV;
		goto out_remap;
	}

<<<<<<< HEAD
	stmp3xxx_reset_block(rtc_data->io, true);
	stmp3xxx_clearl(BM_RTC_PERSISTENT0_ALARM_EN |
			BM_RTC_PERSISTENT0_ALARM_WAKE_EN |
			BM_RTC_PERSISTENT0_ALARM_WAKE,
			rtc_data->io + HW_RTC_PERSISTENT0);
=======
	platform_set_drvdata(pdev, rtc_data);

	mxs_reset_block(rtc_data->io);
=======
		return -ENXIO;
	}

	rtc_data->io = devm_ioremap(&pdev->dev, r->start, resource_size(r));
	if (!rtc_data->io) {
		dev_err(&pdev->dev, "ioremap failed\n");
		return -EIO;
	}

	rtc_data->irq_alarm = platform_get_irq(pdev, 0);

	if (!(readl(STMP3XXX_RTC_STAT + rtc_data->io) &
			STMP3XXX_RTC_STAT_RTC_PRESENT)) {
		dev_err(&pdev->dev, "no device onboard\n");
		return -ENODEV;
	}

	platform_set_drvdata(pdev, rtc_data);

	err = stmp_reset_block(rtc_data->io);
	if (err) {
		dev_err(&pdev->dev, "stmp_reset_block failed: %d\n", err);
		return err;
	}

>>>>>>> refs/remotes/origin/master
	writel(STMP3XXX_RTC_PERSISTENT0_ALARM_EN |
			STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE_EN |
			STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE,
			rtc_data->io + STMP3XXX_RTC_PERSISTENT0_CLR);

	writel(STMP3XXX_RTC_CTRL_ONEMSEC_IRQ_EN |
			STMP3XXX_RTC_CTRL_ALARM_IRQ_EN,
			rtc_data->io + STMP3XXX_RTC_CTRL_CLR);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	rtc_data->rtc = rtc_device_register(pdev->name, &pdev->dev,
				&stmp3xxx_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc_data->rtc)) {
		err = PTR_ERR(rtc_data->rtc);
		goto out_remap;
	}

<<<<<<< HEAD
	rtc_data->irq_count = 0;
	err = request_irq(rtc_data->irq_alarm, stmp3xxx_rtc_interrupt,
			IRQF_DISABLED, "RTC alarm", &pdev->dev);
=======
	err = request_irq(rtc_data->irq_alarm, stmp3xxx_rtc_interrupt, 0,
			"RTC alarm", &pdev->dev);
>>>>>>> refs/remotes/origin/cm-10.0
	if (err) {
		dev_err(&pdev->dev, "Cannot claim IRQ%d\n",
			rtc_data->irq_alarm);
		goto out_irq_alarm;
	}
<<<<<<< HEAD
	err = request_irq(rtc_data->irq_1msec, stmp3xxx_rtc_interrupt,
			IRQF_DISABLED, "RTC tick", &pdev->dev);
	if (err) {
		dev_err(&pdev->dev, "Cannot claim IRQ%d\n",
			rtc_data->irq_1msec);
		goto out_irq1;
	}

	platform_set_drvdata(pdev, rtc_data);

	return 0;

out_irq1:
	free_irq(rtc_data->irq_alarm, &pdev->dev);
out_irq_alarm:
	stmp3xxx_clearl(BM_RTC_CTRL_ONEMSEC_IRQ_EN | BM_RTC_CTRL_ALARM_IRQ_EN,
			rtc_data->io + HW_RTC_CTRL);
	rtc_device_unregister(rtc_data->rtc);
out_remap:
=======

	return 0;

out_irq_alarm:
	rtc_device_unregister(rtc_data->rtc);
out_remap:
	platform_set_drvdata(pdev, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
	iounmap(rtc_data->io);
out_free:
	kfree(rtc_data);
	return err;
}

#ifdef CONFIG_PM
static int stmp3xxx_rtc_suspend(struct platform_device *dev, pm_message_t state)
=======
	rtc_data->rtc = devm_rtc_device_register(&pdev->dev, pdev->name,
				&stmp3xxx_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc_data->rtc))
		return PTR_ERR(rtc_data->rtc);

	err = devm_request_irq(&pdev->dev, rtc_data->irq_alarm,
			stmp3xxx_rtc_interrupt, 0, "RTC alarm", &pdev->dev);
	if (err) {
		dev_err(&pdev->dev, "Cannot claim IRQ%d\n",
			rtc_data->irq_alarm);
		return err;
	}

	stmp3xxx_wdt_register(pdev);
	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int stmp3xxx_rtc_suspend(struct device *dev)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

<<<<<<< HEAD
static int stmp3xxx_rtc_resume(struct platform_device *dev)
{
	struct stmp3xxx_rtc_data *rtc_data = platform_get_drvdata(dev);

<<<<<<< HEAD
	stmp3xxx_reset_block(rtc_data->io, true);
	stmp3xxx_clearl(BM_RTC_PERSISTENT0_ALARM_EN |
			BM_RTC_PERSISTENT0_ALARM_WAKE_EN |
			BM_RTC_PERSISTENT0_ALARM_WAKE,
			rtc_data->io + HW_RTC_PERSISTENT0);
=======
	mxs_reset_block(rtc_data->io);
=======
static int stmp3xxx_rtc_resume(struct device *dev)
{
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev);

	stmp_reset_block(rtc_data->io);
>>>>>>> refs/remotes/origin/master
	writel(STMP3XXX_RTC_PERSISTENT0_ALARM_EN |
			STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE_EN |
			STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE,
			rtc_data->io + STMP3XXX_RTC_PERSISTENT0_CLR);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}
#else
#define stmp3xxx_rtc_suspend	NULL
#define stmp3xxx_rtc_resume	NULL
#endif

static struct platform_driver stmp3xxx_rtcdrv = {
	.probe		= stmp3xxx_rtc_probe,
	.remove		= stmp3xxx_rtc_remove,
	.suspend	= stmp3xxx_rtc_suspend,
	.resume		= stmp3xxx_rtc_resume,
	.driver		= {
		.name	= "stmp3xxx-rtc",
		.owner	= THIS_MODULE,
	},
};

<<<<<<< HEAD
static int __init stmp3xxx_rtc_init(void)
{
	return platform_driver_register(&stmp3xxx_rtcdrv);
}

static void __exit stmp3xxx_rtc_exit(void)
{
	platform_driver_unregister(&stmp3xxx_rtcdrv);
}

module_init(stmp3xxx_rtc_init);
module_exit(stmp3xxx_rtc_exit);

MODULE_DESCRIPTION("STMP3xxx RTC Driver");
MODULE_AUTHOR("dmitry pervushin <dpervushin@embeddedalley.com>");
=======
=======
	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(stmp3xxx_rtc_pm_ops, stmp3xxx_rtc_suspend,
			stmp3xxx_rtc_resume);

static const struct of_device_id rtc_dt_ids[] = {
	{ .compatible = "fsl,stmp3xxx-rtc", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, rtc_dt_ids);

static struct platform_driver stmp3xxx_rtcdrv = {
	.probe		= stmp3xxx_rtc_probe,
	.remove		= stmp3xxx_rtc_remove,
	.driver		= {
		.name	= "stmp3xxx-rtc",
		.owner	= THIS_MODULE,
		.pm	= &stmp3xxx_rtc_pm_ops,
		.of_match_table = rtc_dt_ids,
	},
};

>>>>>>> refs/remotes/origin/master
module_platform_driver(stmp3xxx_rtcdrv);

MODULE_DESCRIPTION("STMP3xxx RTC Driver");
MODULE_AUTHOR("dmitry pervushin <dpervushin@embeddedalley.com> and "
		"Wolfram Sang <w.sang@pengutronix.de>");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
MODULE_LICENSE("GPL");
