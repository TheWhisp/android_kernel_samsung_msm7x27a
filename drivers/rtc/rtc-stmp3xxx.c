/*
 * Freescale STMP37XX/STMP378X Real Time Clock driver
 *
 * Copyright (c) 2007 Sigmatel, Inc.
 * Peter Hartley, <peter.hartley@sigmatel.com>
 *
 * Copyright 2008 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2008 Embedded Alley Solutions, Inc All Rights Reserved.
<<<<<<< HEAD
=======
 * Copyright 2011 Wolfram Sang, Pengutronix e.K.
>>>>>>> refs/remotes/origin/cm-10.0
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

#define STMP3XXX_RTC_CTRL			0x0
#define STMP3XXX_RTC_CTRL_SET			0x4
#define STMP3XXX_RTC_CTRL_CLR			0x8
#define STMP3XXX_RTC_CTRL_ALARM_IRQ_EN		0x00000001
#define STMP3XXX_RTC_CTRL_ONEMSEC_IRQ_EN	0x00000002
#define STMP3XXX_RTC_CTRL_ALARM_IRQ		0x00000004

#define STMP3XXX_RTC_STAT			0x10
#define STMP3XXX_RTC_STAT_STALE_SHIFT		16
#define STMP3XXX_RTC_STAT_RTC_PRESENT		0x80000000

#define STMP3XXX_RTC_SECONDS			0x30

#define STMP3XXX_RTC_ALARM			0x40

#define STMP3XXX_RTC_PERSISTENT0		0x60
#define STMP3XXX_RTC_PERSISTENT0_SET		0x64
#define STMP3XXX_RTC_PERSISTENT0_CLR		0x68
#define STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE_EN	0x00000002
#define STMP3XXX_RTC_PERSISTENT0_ALARM_EN	0x00000004
#define STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE	0x00000080

struct stmp3xxx_rtc_data {
	struct rtc_device *rtc;
	void __iomem *io;
	int irq_alarm;
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
}

/* Time read/write */
static int stmp3xxx_rtc_gettime(struct device *dev, struct rtc_time *rtc_tm)
{
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev);

	stmp3xxx_wait_time(rtc_data);
<<<<<<< HEAD
	rtc_time_to_tm(__raw_readl(rtc_data->io + HW_RTC_SECONDS), rtc_tm);
=======
	rtc_time_to_tm(readl(rtc_data->io + STMP3XXX_RTC_SECONDS), rtc_tm);
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static int stmp3xxx_rtc_set_mmss(struct device *dev, unsigned long t)
{
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev);

<<<<<<< HEAD
	__raw_writel(t, rtc_data->io + HW_RTC_SECONDS);
=======
	writel(t, rtc_data->io + STMP3XXX_RTC_SECONDS);
>>>>>>> refs/remotes/origin/cm-10.0
	stmp3xxx_wait_time(rtc_data);
	return 0;
}

/* interrupt(s) handler */
static irqreturn_t stmp3xxx_rtc_interrupt(int irq, void *dev_id)
{
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev_id);
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
	u32 status = readl(rtc_data->io + STMP3XXX_RTC_CTRL);

	if (status & STMP3XXX_RTC_CTRL_ALARM_IRQ) {
		writel(STMP3XXX_RTC_CTRL_ALARM_IRQ,
				rtc_data->io + STMP3XXX_RTC_CTRL_CLR);
		rtc_update_irq(rtc_data->rtc, 1, RTC_AF | RTC_IRQF);
		return IRQ_HANDLED;
	}

	return IRQ_NONE;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int stmp3xxx_alarm_irq_enable(struct device *dev, unsigned int enabled)
{
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev);
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
>>>>>>> refs/remotes/origin/cm-10.0
	}
	return 0;
}

static int stmp3xxx_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alm)
{
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev);

<<<<<<< HEAD
	rtc_time_to_tm(__raw_readl(rtc_data->io + HW_RTC_ALARM), &alm->time);
=======
	rtc_time_to_tm(readl(rtc_data->io + STMP3XXX_RTC_ALARM), &alm->time);
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static int stmp3xxx_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alm)
{
	unsigned long t;
	struct stmp3xxx_rtc_data *rtc_data = dev_get_drvdata(dev);

	rtc_tm_to_time(&alm->time, &t);
<<<<<<< HEAD
	__raw_writel(t, rtc_data->io + HW_RTC_ALARM);
=======
	writel(t, rtc_data->io + STMP3XXX_RTC_ALARM);

	stmp3xxx_alarm_irq_enable(dev, alm->enabled);

>>>>>>> refs/remotes/origin/cm-10.0
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

	return 0;
}

static int stmp3xxx_rtc_probe(struct platform_device *pdev)
{
	struct stmp3xxx_rtc_data *rtc_data;
	struct resource *r;
	int err;

	rtc_data = kzalloc(sizeof *rtc_data, GFP_KERNEL);
	if (!rtc_data)
		return -ENOMEM;

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r) {
		dev_err(&pdev->dev, "failed to get resource\n");
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
	writel(STMP3XXX_RTC_PERSISTENT0_ALARM_EN |
			STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE_EN |
			STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE,
			rtc_data->io + STMP3XXX_RTC_PERSISTENT0_CLR);

	writel(STMP3XXX_RTC_CTRL_ONEMSEC_IRQ_EN |
			STMP3XXX_RTC_CTRL_ALARM_IRQ_EN,
			rtc_data->io + STMP3XXX_RTC_CTRL_CLR);

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
{
	return 0;
}

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
	writel(STMP3XXX_RTC_PERSISTENT0_ALARM_EN |
			STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE_EN |
			STMP3XXX_RTC_PERSISTENT0_ALARM_WAKE,
			rtc_data->io + STMP3XXX_RTC_PERSISTENT0_CLR);
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
module_platform_driver(stmp3xxx_rtcdrv);

MODULE_DESCRIPTION("STMP3xxx RTC Driver");
MODULE_AUTHOR("dmitry pervushin <dpervushin@embeddedalley.com> and "
		"Wolfram Sang <w.sang@pengutronix.de>");
>>>>>>> refs/remotes/origin/cm-10.0
MODULE_LICENSE("GPL");
