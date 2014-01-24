/* linux/drivers/mmc/host/sdhci-s3c.c
 *
 * Copyright 2008 Openmoko Inc.
 * Copyright 2008 Simtec Electronics
 *      Ben Dooks <ben@simtec.co.uk>
 *      http://armlinux.simtec.co.uk/
 *
 * SDHCI (HSMMC) support for Samsung SoC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
=======
#include <linux/platform_data/mmc-sdhci-s3c.h>
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/gpio.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/pm.h>
#include <linux/pm_runtime.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

#include <linux/mmc/host.h>

#include <plat/sdhci.h>
#include <plat/regs-sdhci.h>

=======

#include <linux/mmc/host.h>

#include "sdhci-s3c-regs.h"
>>>>>>> refs/remotes/origin/master
#include "sdhci.h"

#define MAX_BUS_CLK	(4)

<<<<<<< HEAD
=======
/* Number of gpio's used is max data bus width + command and clock lines */
#define NUM_GPIOS(x)	(x + 2)

>>>>>>> refs/remotes/origin/master
/**
 * struct sdhci_s3c - S3C SDHCI instance
 * @host: The SDHCI host created
 * @pdev: The platform device we where created from.
 * @ioarea: The resource created when we claimed the IO area.
 * @pdata: The platform data for this controller.
 * @cur_clk: The index of the current bus clock.
 * @clk_io: The clock for the internal bus interface.
 * @clk_bus: The clocks that are available for the SD/MMC bus clock.
 */
struct sdhci_s3c {
	struct sdhci_host	*host;
	struct platform_device	*pdev;
	struct resource		*ioarea;
	struct s3c_sdhci_platdata *pdata;
	unsigned int		cur_clk;
	int			ext_cd_irq;
	int			ext_cd_gpio;

	struct clk		*clk_io;
	struct clk		*clk_bus[MAX_BUS_CLK];
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/**
 * struct sdhci_s3c_driver_data - S3C SDHCI platform specific driver data
 * @sdhci_quirks: sdhci host specific quirks.
 *
 * Specifies platform specific configuration of sdhci controller.
 * Note: A structure for driver specific platform data is used for future
 * expansion of its usage.
 */
struct sdhci_s3c_drv_data {
	unsigned int	sdhci_quirks;
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static inline struct sdhci_s3c *to_s3c(struct sdhci_host *host)
{
	return sdhci_priv(host);
}

/**
 * get_curclk - convert ctrl2 register to clock source number
 * @ctrl2: Control2 register value.
 */
static u32 get_curclk(u32 ctrl2)
{
	ctrl2 &= S3C_SDHCI_CTRL2_SELBASECLK_MASK;
	ctrl2 >>= S3C_SDHCI_CTRL2_SELBASECLK_SHIFT;

	return ctrl2;
}

static void sdhci_s3c_check_sclk(struct sdhci_host *host)
{
	struct sdhci_s3c *ourhost = to_s3c(host);
	u32 tmp = readl(host->ioaddr + S3C_SDHCI_CONTROL2);

	if (get_curclk(tmp) != ourhost->cur_clk) {
		dev_dbg(&ourhost->pdev->dev, "restored ctrl2 clock setting\n");

		tmp &= ~S3C_SDHCI_CTRL2_SELBASECLK_MASK;
		tmp |= ourhost->cur_clk << S3C_SDHCI_CTRL2_SELBASECLK_SHIFT;
<<<<<<< HEAD
<<<<<<< HEAD
		writel(tmp, host->ioaddr + 0x80);
=======
		writel(tmp, host->ioaddr + S3C_SDHCI_CONTROL2);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		writel(tmp, host->ioaddr + S3C_SDHCI_CONTROL2);
>>>>>>> refs/remotes/origin/master
	}
}

/**
 * sdhci_s3c_get_max_clk - callback to get maximum clock frequency.
 * @host: The SDHCI host instance.
 *
 * Callback to return the maximum clock rate acheivable by the controller.
*/
static unsigned int sdhci_s3c_get_max_clk(struct sdhci_host *host)
{
	struct sdhci_s3c *ourhost = to_s3c(host);
	struct clk *busclk;
	unsigned int rate, max;
	int clk;

	/* note, a reset will reset the clock source */

	sdhci_s3c_check_sclk(host);

	for (max = 0, clk = 0; clk < MAX_BUS_CLK; clk++) {
		busclk = ourhost->clk_bus[clk];
		if (!busclk)
			continue;

		rate = clk_get_rate(busclk);
		if (rate > max)
			max = rate;
	}

	return max;
}

/**
 * sdhci_s3c_consider_clock - consider one the bus clocks for current setting
 * @ourhost: Our SDHCI instance.
 * @src: The source clock index.
 * @wanted: The clock frequency wanted.
 */
static unsigned int sdhci_s3c_consider_clock(struct sdhci_s3c *ourhost,
					     unsigned int src,
					     unsigned int wanted)
{
	unsigned long rate;
	struct clk *clksrc = ourhost->clk_bus[src];
	int div;

	if (!clksrc)
		return UINT_MAX;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	 * Clock divider's step is different as 1 from that of host controller
	 * when 'clk_type' is S3C_SDHCI_CLK_DIV_EXTERNAL.
	 */
	if (ourhost->pdata->clk_type) {
=======
=======
>>>>>>> refs/remotes/origin/master
	 * If controller uses a non-standard clock division, find the best clock
	 * speed possible with selected clock source and skip the division.
	 */
	if (ourhost->host->quirks & SDHCI_QUIRK_NONSTANDARD_CLOCK) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		rate = clk_round_rate(clksrc, wanted);
		return wanted - rate;
	}

	rate = clk_get_rate(clksrc);

	for (div = 1; div < 256; div *= 2) {
		if ((rate / div) <= wanted)
			break;
	}

	dev_dbg(&ourhost->pdev->dev, "clk %d: rate %ld, want %d, got %ld\n",
		src, rate, wanted, rate / div);

<<<<<<< HEAD
	return (wanted - (rate / div));
=======
	return wanted - (rate / div);
>>>>>>> refs/remotes/origin/master
}

/**
 * sdhci_s3c_set_clock - callback on clock change
 * @host: The SDHCI host being changed
 * @clock: The clock rate being requested.
 *
 * When the card's clock is going to be changed, look at the new frequency
 * and find the best clock source to go with it.
*/
static void sdhci_s3c_set_clock(struct sdhci_host *host, unsigned int clock)
{
	struct sdhci_s3c *ourhost = to_s3c(host);
	unsigned int best = UINT_MAX;
	unsigned int delta;
	int best_src = 0;
	int src;
	u32 ctrl;

	/* don't bother if the clock is going off. */
	if (clock == 0)
		return;

	for (src = 0; src < MAX_BUS_CLK; src++) {
		delta = sdhci_s3c_consider_clock(ourhost, src, clock);
		if (delta < best) {
			best = delta;
			best_src = src;
		}
	}

	dev_dbg(&ourhost->pdev->dev,
		"selected source %d, clock %d, delta %d\n",
		 best_src, clock, best);

	/* select the new clock source */
<<<<<<< HEAD

	if (ourhost->cur_clk != best_src) {
		struct clk *clk = ourhost->clk_bus[best_src];

=======
	if (ourhost->cur_clk != best_src) {
		struct clk *clk = ourhost->clk_bus[best_src];

		clk_prepare_enable(clk);
		clk_disable_unprepare(ourhost->clk_bus[ourhost->cur_clk]);

>>>>>>> refs/remotes/origin/master
		/* turn clock off to card before changing clock source */
		writew(0, host->ioaddr + SDHCI_CLOCK_CONTROL);

		ourhost->cur_clk = best_src;
		host->max_clk = clk_get_rate(clk);

		ctrl = readl(host->ioaddr + S3C_SDHCI_CONTROL2);
		ctrl &= ~S3C_SDHCI_CTRL2_SELBASECLK_MASK;
		ctrl |= best_src << S3C_SDHCI_CTRL2_SELBASECLK_SHIFT;
		writel(ctrl, host->ioaddr + S3C_SDHCI_CONTROL2);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/* reconfigure the hardware for new clock rate */

	{
		struct mmc_ios ios;

		ios.clock = clock;

		if (ourhost->pdata->cfg_card)
			(ourhost->pdata->cfg_card)(ourhost->pdev, host->ioaddr,
						   &ios, NULL);
	}
=======
=======
>>>>>>> refs/remotes/origin/master
	/* reprogram default hardware configuration */
	writel(S3C64XX_SDHCI_CONTROL4_DRIVE_9mA,
		host->ioaddr + S3C64XX_SDHCI_CONTROL4);

	ctrl = readl(host->ioaddr + S3C_SDHCI_CONTROL2);
	ctrl |= (S3C64XX_SDHCI_CTRL2_ENSTAASYNCCLR |
		  S3C64XX_SDHCI_CTRL2_ENCMDCNFMSK |
		  S3C_SDHCI_CTRL2_ENFBCLKRX |
		  S3C_SDHCI_CTRL2_DFCNT_NONE |
		  S3C_SDHCI_CTRL2_ENCLKOUTHOLD);
	writel(ctrl, host->ioaddr + S3C_SDHCI_CONTROL2);

	/* reconfigure the controller for new clock rate */
	ctrl = (S3C_SDHCI_CTRL3_FCSEL1 | S3C_SDHCI_CTRL3_FCSEL0);
	if (clock < 25 * 1000000)
		ctrl |= (S3C_SDHCI_CTRL3_FCSEL3 | S3C_SDHCI_CTRL3_FCSEL2);
	writel(ctrl, host->ioaddr + S3C_SDHCI_CONTROL3);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/**
 * sdhci_s3c_get_min_clock - callback to get minimal supported clock value
 * @host: The SDHCI host being queried
 *
 * To init mmc host properly a minimal clock value is needed. For high system
 * bus clock's values the standard formula gives values out of allowed range.
 * The clock still can be set to lower values, if clock source other then
 * system bus is selected.
*/
static unsigned int sdhci_s3c_get_min_clock(struct sdhci_host *host)
{
	struct sdhci_s3c *ourhost = to_s3c(host);
	unsigned int delta, min = UINT_MAX;
	int src;

	for (src = 0; src < MAX_BUS_CLK; src++) {
		delta = sdhci_s3c_consider_clock(ourhost, src, 0);
		if (delta == UINT_MAX)
			continue;
		/* delta is a negative value in this case */
		if (-delta < min)
			min = -delta;
	}
	return min;
}

/* sdhci_cmu_get_max_clk - callback to get maximum clock frequency.*/
static unsigned int sdhci_cmu_get_max_clock(struct sdhci_host *host)
{
	struct sdhci_s3c *ourhost = to_s3c(host);

	return clk_round_rate(ourhost->clk_bus[ourhost->cur_clk], UINT_MAX);
}

/* sdhci_cmu_get_min_clock - callback to get minimal supported clock value. */
static unsigned int sdhci_cmu_get_min_clock(struct sdhci_host *host)
{
	struct sdhci_s3c *ourhost = to_s3c(host);

	/*
	 * initial clock can be in the frequency range of
	 * 100KHz-400KHz, so we set it as max value.
	 */
	return clk_round_rate(ourhost->clk_bus[ourhost->cur_clk], 400000);
}

/* sdhci_cmu_set_clock - callback on clock change.*/
static void sdhci_cmu_set_clock(struct sdhci_host *host, unsigned int clock)
{
	struct sdhci_s3c *ourhost = to_s3c(host);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	unsigned long timeout;
	u16 clk = 0;
>>>>>>> refs/remotes/origin/cm-10.0

	/* don't bother if the clock is going off */
	if (clock == 0)
		return;
=======
	struct device *dev = &ourhost->pdev->dev;
	unsigned long timeout;
	u16 clk = 0;

	/* If the clock is going off, set to 0 at clock control register */
	if (clock == 0) {
		sdhci_writew(host, 0, SDHCI_CLOCK_CONTROL);
		host->clock = clock;
		return;
	}
>>>>>>> refs/remotes/origin/master

	sdhci_s3c_set_clock(host, clock);

	clk_set_rate(ourhost->clk_bus[ourhost->cur_clk], clock);

	host->clock = clock;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

	clk = SDHCI_CLOCK_INT_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);

	/* Wait max 20 ms */
	timeout = 20;
	while (!((clk = sdhci_readw(host, SDHCI_CLOCK_CONTROL))
		& SDHCI_CLOCK_INT_STABLE)) {
		if (timeout == 0) {
<<<<<<< HEAD
			printk(KERN_ERR "%s: Internal clock never "
				"stabilised.\n", mmc_hostname(host->mmc));
=======
			dev_err(dev, "%s: Internal clock never stabilised.\n",
				mmc_hostname(host->mmc));
>>>>>>> refs/remotes/origin/master
			return;
		}
		timeout--;
		mdelay(1);
	}

	clk |= SDHCI_CLOCK_CARD_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * sdhci_s3c_platform_8bit_width - support 8bit buswidth
=======
}

/**
 * sdhci_s3c_platform_bus_width - support 8bit buswidth
>>>>>>> refs/remotes/origin/master
 * @host: The SDHCI host being queried
 * @width: MMC_BUS_WIDTH_ macro for the bus width being requested
 *
 * We have 8-bit width support but is not a v3 controller.
<<<<<<< HEAD
 * So we add platform_8bit_width() and support 8bit width.
 */
static int sdhci_s3c_platform_8bit_width(struct sdhci_host *host, int width)
=======
 * So we add platform_bus_width() and support 8bit width.
 */
static int sdhci_s3c_platform_bus_width(struct sdhci_host *host, int width)
>>>>>>> refs/remotes/origin/master
{
	u8 ctrl;

	ctrl = sdhci_readb(host, SDHCI_HOST_CONTROL);

	switch (width) {
	case MMC_BUS_WIDTH_8:
		ctrl |= SDHCI_CTRL_8BITBUS;
		ctrl &= ~SDHCI_CTRL_4BITBUS;
		break;
	case MMC_BUS_WIDTH_4:
		ctrl |= SDHCI_CTRL_4BITBUS;
		ctrl &= ~SDHCI_CTRL_8BITBUS;
		break;
	default:
		ctrl &= ~SDHCI_CTRL_4BITBUS;
		ctrl &= ~SDHCI_CTRL_8BITBUS;
		break;
	}

	sdhci_writeb(host, ctrl, SDHCI_HOST_CONTROL);

	return 0;
}

static struct sdhci_ops sdhci_s3c_ops = {
	.get_max_clock		= sdhci_s3c_get_max_clk,
	.set_clock		= sdhci_s3c_set_clock,
	.get_min_clock		= sdhci_s3c_get_min_clock,
<<<<<<< HEAD
	.platform_8bit_width	= sdhci_s3c_platform_8bit_width,
=======
	.platform_bus_width	= sdhci_s3c_platform_bus_width,
>>>>>>> refs/remotes/origin/master
};

static void sdhci_s3c_notify_change(struct platform_device *dev, int state)
{
	struct sdhci_host *host = platform_get_drvdata(dev);
<<<<<<< HEAD
=======
#ifdef CONFIG_PM_RUNTIME
	struct sdhci_s3c *sc = sdhci_priv(host);
#endif
>>>>>>> refs/remotes/origin/master
	unsigned long flags;

	if (host) {
		spin_lock_irqsave(&host->lock, flags);
		if (state) {
			dev_dbg(&dev->dev, "card inserted.\n");
<<<<<<< HEAD
=======
#ifdef CONFIG_PM_RUNTIME
			clk_prepare_enable(sc->clk_io);
#endif
>>>>>>> refs/remotes/origin/master
			host->flags &= ~SDHCI_DEVICE_DEAD;
			host->quirks |= SDHCI_QUIRK_BROKEN_CARD_DETECTION;
		} else {
			dev_dbg(&dev->dev, "card removed.\n");
			host->flags |= SDHCI_DEVICE_DEAD;
			host->quirks &= ~SDHCI_QUIRK_BROKEN_CARD_DETECTION;
<<<<<<< HEAD
=======
#ifdef CONFIG_PM_RUNTIME
			clk_disable_unprepare(sc->clk_io);
#endif
>>>>>>> refs/remotes/origin/master
		}
		tasklet_schedule(&host->card_tasklet);
		spin_unlock_irqrestore(&host->lock, flags);
	}
}

static irqreturn_t sdhci_s3c_gpio_card_detect_thread(int irq, void *dev_id)
{
	struct sdhci_s3c *sc = dev_id;
	int status = gpio_get_value(sc->ext_cd_gpio);
	if (sc->pdata->ext_cd_gpio_invert)
		status = !status;
	sdhci_s3c_notify_change(sc->pdev, status);
	return IRQ_HANDLED;
}

static void sdhci_s3c_setup_card_detect_gpio(struct sdhci_s3c *sc)
{
	struct s3c_sdhci_platdata *pdata = sc->pdata;
	struct device *dev = &sc->pdev->dev;

<<<<<<< HEAD
	if (gpio_request(pdata->ext_cd_gpio, "SDHCI EXT CD") == 0) {
=======
	if (devm_gpio_request(dev, pdata->ext_cd_gpio, "SDHCI EXT CD") == 0) {
>>>>>>> refs/remotes/origin/master
		sc->ext_cd_gpio = pdata->ext_cd_gpio;
		sc->ext_cd_irq = gpio_to_irq(pdata->ext_cd_gpio);
		if (sc->ext_cd_irq &&
		    request_threaded_irq(sc->ext_cd_irq, NULL,
					 sdhci_s3c_gpio_card_detect_thread,
<<<<<<< HEAD
					 IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
=======
					 IRQF_TRIGGER_RISING |
					 IRQF_TRIGGER_FALLING |
					 IRQF_ONESHOT,
>>>>>>> refs/remotes/origin/master
					 dev_name(dev), sc) == 0) {
			int status = gpio_get_value(sc->ext_cd_gpio);
			if (pdata->ext_cd_gpio_invert)
				status = !status;
			sdhci_s3c_notify_change(sc->pdev, status);
		} else {
			dev_warn(dev, "cannot request irq for card detect\n");
			sc->ext_cd_irq = 0;
		}
	} else {
		dev_err(dev, "cannot request gpio for card detect\n");
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static int __devinit sdhci_s3c_probe(struct platform_device *pdev)
{
	struct s3c_sdhci_platdata *pdata = pdev->dev.platform_data;
=======
static inline struct sdhci_s3c_drv_data *sdhci_s3c_get_driver_data(
			struct platform_device *pdev)
{
=======
#ifdef CONFIG_OF
static int sdhci_s3c_parse_dt(struct device *dev,
		struct sdhci_host *host, struct s3c_sdhci_platdata *pdata)
{
	struct device_node *node = dev->of_node;
	struct sdhci_s3c *ourhost = to_s3c(host);
	u32 max_width;
	int gpio;

	/* if the bus-width property is not specified, assume width as 1 */
	if (of_property_read_u32(node, "bus-width", &max_width))
		max_width = 1;
	pdata->max_width = max_width;

	/* get the card detection method */
	if (of_get_property(node, "broken-cd", NULL)) {
		pdata->cd_type = S3C_SDHCI_CD_NONE;
		return 0;
	}

	if (of_get_property(node, "non-removable", NULL)) {
		pdata->cd_type = S3C_SDHCI_CD_PERMANENT;
		return 0;
	}

	gpio = of_get_named_gpio(node, "cd-gpios", 0);
	if (gpio_is_valid(gpio)) {
		pdata->cd_type = S3C_SDHCI_CD_GPIO;
		pdata->ext_cd_gpio = gpio;
		ourhost->ext_cd_gpio = -1;
		if (of_get_property(node, "cd-inverted", NULL))
			pdata->ext_cd_gpio_invert = 1;
		return 0;
	} else if (gpio != -ENOENT) {
		dev_err(dev, "invalid card detect gpio specified\n");
		return -EINVAL;
	}

	/* assuming internal card detect that will be configured by pinctrl */
	pdata->cd_type = S3C_SDHCI_CD_INTERNAL;
	return 0;
}
#else
static int sdhci_s3c_parse_dt(struct device *dev,
		struct sdhci_host *host, struct s3c_sdhci_platdata *pdata)
{
	return -EINVAL;
}
#endif

static const struct of_device_id sdhci_s3c_dt_match[];

static inline struct sdhci_s3c_drv_data *sdhci_s3c_get_driver_data(
			struct platform_device *pdev)
{
#ifdef CONFIG_OF
	if (pdev->dev.of_node) {
		const struct of_device_id *match;
		match = of_match_node(sdhci_s3c_dt_match, pdev->dev.of_node);
		return (struct sdhci_s3c_drv_data *)match->data;
	}
#endif
>>>>>>> refs/remotes/origin/master
	return (struct sdhci_s3c_drv_data *)
			platform_get_device_id(pdev)->driver_data;
}

<<<<<<< HEAD
static int __devinit sdhci_s3c_probe(struct platform_device *pdev)
{
	struct s3c_sdhci_platdata *pdata;
	struct sdhci_s3c_drv_data *drv_data;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int sdhci_s3c_probe(struct platform_device *pdev)
{
	struct s3c_sdhci_platdata *pdata;
	struct sdhci_s3c_drv_data *drv_data;
>>>>>>> refs/remotes/origin/master
	struct device *dev = &pdev->dev;
	struct sdhci_host *host;
	struct sdhci_s3c *sc;
	struct resource *res;
	int ret, irq, ptr, clks;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!pdata) {
=======
	if (!pdev->dev.platform_data) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!pdev->dev.platform_data && !pdev->dev.of_node) {
>>>>>>> refs/remotes/origin/master
		dev_err(dev, "no device data specified\n");
		return -ENOENT;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(dev, "no irq specified\n");
		return irq;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "no memory specified\n");
		return -ENOENT;
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	host = sdhci_alloc_host(dev, sizeof(struct sdhci_s3c));
	if (IS_ERR(host)) {
		dev_err(dev, "sdhci_alloc_host() failed\n");
		return PTR_ERR(host);
	}
<<<<<<< HEAD

<<<<<<< HEAD
=======
	pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata) {
		ret = -ENOMEM;
		goto err_io_clk;
	}
	memcpy(pdata, pdev->dev.platform_data, sizeof(*pdata));

	drv_data = sdhci_s3c_get_driver_data(pdev);
>>>>>>> refs/remotes/origin/cm-10.0
	sc = sdhci_priv(host);
=======
	sc = sdhci_priv(host);

	pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata) {
		ret = -ENOMEM;
		goto err_pdata_io_clk;
	}

	if (pdev->dev.of_node) {
		ret = sdhci_s3c_parse_dt(&pdev->dev, host, pdata);
		if (ret)
			goto err_pdata_io_clk;
	} else {
		memcpy(pdata, pdev->dev.platform_data, sizeof(*pdata));
		sc->ext_cd_gpio = -1; /* invalid gpio number */
	}

	drv_data = sdhci_s3c_get_driver_data(pdev);
>>>>>>> refs/remotes/origin/master

	sc->host = host;
	sc->pdev = pdev;
	sc->pdata = pdata;
<<<<<<< HEAD
	sc->ext_cd_gpio = -1; /* invalid gpio number */

	platform_set_drvdata(pdev, host);

	sc->clk_io = clk_get(dev, "hsmmc");
	if (IS_ERR(sc->clk_io)) {
		dev_err(dev, "failed to get io clock\n");
		ret = PTR_ERR(sc->clk_io);
		goto err_io_clk;
	}

	/* enable the local io clock and keep it running for the moment. */
	clk_enable(sc->clk_io);

	for (clks = 0, ptr = 0; ptr < MAX_BUS_CLK; ptr++) {
		struct clk *clk;
<<<<<<< HEAD
		char *name = pdata->clocks[ptr];

		if (name == NULL)
			continue;

		clk = clk_get(dev, name);
		if (IS_ERR(clk)) {
			dev_err(dev, "failed to get clock %s\n", name);
=======
		char name[14];

		snprintf(name, 14, "mmc_busclk.%d", ptr);
		clk = clk_get(dev, name);
		if (IS_ERR(clk)) {
>>>>>>> refs/remotes/origin/cm-10.0
			continue;
		}
=======

	platform_set_drvdata(pdev, host);

	sc->clk_io = devm_clk_get(dev, "hsmmc");
	if (IS_ERR(sc->clk_io)) {
		dev_err(dev, "failed to get io clock\n");
		ret = PTR_ERR(sc->clk_io);
		goto err_pdata_io_clk;
	}

	/* enable the local io clock and keep it running for the moment. */
	clk_prepare_enable(sc->clk_io);

	for (clks = 0, ptr = 0; ptr < MAX_BUS_CLK; ptr++) {
		struct clk *clk;
		char name[14];

		snprintf(name, 14, "mmc_busclk.%d", ptr);
		clk = devm_clk_get(dev, name);
		if (IS_ERR(clk))
			continue;
>>>>>>> refs/remotes/origin/master

		clks++;
		sc->clk_bus[ptr] = clk;

		/*
		 * save current clock index to know which clock bus
		 * is used later in overriding functions.
		 */
		sc->cur_clk = ptr;

<<<<<<< HEAD
		clk_enable(clk);

=======
>>>>>>> refs/remotes/origin/master
		dev_info(dev, "clock source %d: %s (%ld Hz)\n",
			 ptr, name, clk_get_rate(clk));
	}

	if (clks == 0) {
		dev_err(dev, "failed to find any bus clocks\n");
		ret = -ENOENT;
		goto err_no_busclks;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	sc->ioarea = request_mem_region(res->start, resource_size(res),
					mmc_hostname(host->mmc));
	if (!sc->ioarea) {
		dev_err(dev, "failed to reserve register area\n");
		ret = -ENXIO;
		goto err_req_regs;
	}

	host->ioaddr = ioremap_nocache(res->start, resource_size(res));
=======
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	host->ioaddr = devm_request_and_ioremap(&pdev->dev, res);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!host->ioaddr) {
		dev_err(dev, "failed to map registers\n");
		ret = -ENXIO;
=======
#ifndef CONFIG_PM_RUNTIME
	clk_prepare_enable(sc->clk_bus[sc->cur_clk]);
#endif

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	host->ioaddr = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(host->ioaddr)) {
		ret = PTR_ERR(host->ioaddr);
>>>>>>> refs/remotes/origin/master
		goto err_req_regs;
	}

	/* Ensure we have minimal gpio selected CMD/CLK/Detect */
	if (pdata->cfg_gpio)
		pdata->cfg_gpio(pdev, pdata->max_width);

	host->hw_name = "samsung-hsmmc";
	host->ops = &sdhci_s3c_ops;
	host->quirks = 0;
<<<<<<< HEAD
=======
	host->quirks2 = 0;
>>>>>>> refs/remotes/origin/master
	host->irq = irq;

	/* Setup quirks for the controller */
	host->quirks |= SDHCI_QUIRK_NO_ENDATTR_IN_NOPDESC;
	host->quirks |= SDHCI_QUIRK_NO_HISPD_BIT;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (drv_data)
		host->quirks |= drv_data->sdhci_quirks;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (drv_data)
		host->quirks |= drv_data->sdhci_quirks;
>>>>>>> refs/remotes/origin/master

#ifndef CONFIG_MMC_SDHCI_S3C_DMA

	/* we currently see overruns on errors, so disable the SDMA
	 * support as well. */
	host->quirks |= SDHCI_QUIRK_BROKEN_DMA;

#endif /* CONFIG_MMC_SDHCI_S3C_DMA */

	/* It seems we do not get an DATA transfer complete on non-busy
	 * transfers, not sure if this is a problem with this specific
	 * SDHCI block, or a missing configuration that needs to be set. */
	host->quirks |= SDHCI_QUIRK_NO_BUSY_IRQ;

	/* This host supports the Auto CMD12 */
	host->quirks |= SDHCI_QUIRK_MULTIBLOCK_READ_ACMD12;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* Samsung SoCs need BROKEN_ADMA_ZEROLEN_DESC */
	host->quirks |= SDHCI_QUIRK_BROKEN_ADMA_ZEROLEN_DESC;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Samsung SoCs need BROKEN_ADMA_ZEROLEN_DESC */
	host->quirks |= SDHCI_QUIRK_BROKEN_ADMA_ZEROLEN_DESC;

>>>>>>> refs/remotes/origin/master
	if (pdata->cd_type == S3C_SDHCI_CD_NONE ||
	    pdata->cd_type == S3C_SDHCI_CD_PERMANENT)
		host->quirks |= SDHCI_QUIRK_BROKEN_CARD_DETECTION;

	if (pdata->cd_type == S3C_SDHCI_CD_PERMANENT)
		host->mmc->caps = MMC_CAP_NONREMOVABLE;

<<<<<<< HEAD
<<<<<<< HEAD
	if (pdata->host_caps)
		host->mmc->caps |= pdata->host_caps;
=======
=======
>>>>>>> refs/remotes/origin/master
	switch (pdata->max_width) {
	case 8:
		host->mmc->caps |= MMC_CAP_8_BIT_DATA;
	case 4:
		host->mmc->caps |= MMC_CAP_4_BIT_DATA;
		break;
	}

	if (pdata->pm_caps)
		host->mmc->pm_caps |= pdata->pm_caps;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	host->quirks |= (SDHCI_QUIRK_32BIT_DMA_ADDR |
			 SDHCI_QUIRK_32BIT_DMA_SIZE);

	/* HSMMC on Samsung SoCs uses SDCLK as timeout clock */
	host->quirks |= SDHCI_QUIRK_DATA_TIMEOUT_USES_SDCLK;

	/*
	 * If controller does not have internal clock divider,
	 * we can use overriding functions instead of default.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (pdata->clk_type) {
=======
	if (host->quirks & SDHCI_QUIRK_NONSTANDARD_CLOCK) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (host->quirks & SDHCI_QUIRK_NONSTANDARD_CLOCK) {
>>>>>>> refs/remotes/origin/master
		sdhci_s3c_ops.set_clock = sdhci_cmu_set_clock;
		sdhci_s3c_ops.get_min_clock = sdhci_cmu_get_min_clock;
		sdhci_s3c_ops.get_max_clock = sdhci_cmu_get_max_clock;
	}

	/* It supports additional host capabilities if needed */
	if (pdata->host_caps)
		host->mmc->caps |= pdata->host_caps;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = sdhci_add_host(host);
	if (ret) {
		dev_err(dev, "sdhci_add_host() failed\n");
		goto err_add_host;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (pdata->host_caps2)
		host->mmc->caps2 |= pdata->host_caps2;

	pm_runtime_enable(&pdev->dev);
	pm_runtime_set_autosuspend_delay(&pdev->dev, 50);
	pm_runtime_use_autosuspend(&pdev->dev);
	pm_suspend_ignore_children(&pdev->dev, 1);

	ret = sdhci_add_host(host);
	if (ret) {
		dev_err(dev, "sdhci_add_host() failed\n");
		pm_runtime_forbid(&pdev->dev);
		pm_runtime_get_noresume(&pdev->dev);
		goto err_req_regs;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	/* The following two methods of card detection might call
	   sdhci_s3c_notify_change() immediately, so they can be called
	   only after sdhci_add_host(). Setup errors are ignored. */
	if (pdata->cd_type == S3C_SDHCI_CD_EXTERNAL && pdata->ext_cd_init)
		pdata->ext_cd_init(&sdhci_s3c_notify_change);
	if (pdata->cd_type == S3C_SDHCI_CD_GPIO &&
	    gpio_is_valid(pdata->ext_cd_gpio))
		sdhci_s3c_setup_card_detect_gpio(sc);

<<<<<<< HEAD
	return 0;

<<<<<<< HEAD
 err_add_host:
	release_resource(sc->ioarea);
	kfree(sc->ioarea);

 err_req_regs:
	for (ptr = 0; ptr < MAX_BUS_CLK; ptr++) {
		clk_disable(sc->clk_bus[ptr]);
		clk_put(sc->clk_bus[ptr]);
=======
 err_req_regs:
	for (ptr = 0; ptr < MAX_BUS_CLK; ptr++) {
		if (sc->clk_bus[ptr]) {
			clk_disable(sc->clk_bus[ptr]);
			clk_put(sc->clk_bus[ptr]);
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

 err_no_busclks:
	clk_disable(sc->clk_io);
	clk_put(sc->clk_io);

 err_io_clk:
=======
#ifdef CONFIG_PM_RUNTIME
	if (pdata->cd_type != S3C_SDHCI_CD_INTERNAL)
		clk_disable_unprepare(sc->clk_io);
#endif
	return 0;

 err_req_regs:
#ifndef CONFIG_PM_RUNTIME
	clk_disable_unprepare(sc->clk_bus[sc->cur_clk]);
#endif

 err_no_busclks:
	clk_disable_unprepare(sc->clk_io);

 err_pdata_io_clk:
>>>>>>> refs/remotes/origin/master
	sdhci_free_host(host);

	return ret;
}

<<<<<<< HEAD
static int __devexit sdhci_s3c_remove(struct platform_device *pdev)
{
	struct s3c_sdhci_platdata *pdata = pdev->dev.platform_data;
	struct sdhci_host *host =  platform_get_drvdata(pdev);
	struct sdhci_s3c *sc = sdhci_priv(host);
	int ptr;
=======
static int sdhci_s3c_remove(struct platform_device *pdev)
{
	struct sdhci_host *host =  platform_get_drvdata(pdev);
	struct sdhci_s3c *sc = sdhci_priv(host);
	struct s3c_sdhci_platdata *pdata = sc->pdata;
>>>>>>> refs/remotes/origin/master

	if (pdata->cd_type == S3C_SDHCI_CD_EXTERNAL && pdata->ext_cd_cleanup)
		pdata->ext_cd_cleanup(&sdhci_s3c_notify_change);

	if (sc->ext_cd_irq)
		free_irq(sc->ext_cd_irq, sc);

<<<<<<< HEAD
	if (gpio_is_valid(sc->ext_cd_gpio))
		gpio_free(sc->ext_cd_gpio);

	sdhci_remove_host(host, 1);

<<<<<<< HEAD
=======
	pm_runtime_disable(&pdev->dev);

>>>>>>> refs/remotes/origin/cm-10.0
	for (ptr = 0; ptr < MAX_BUS_CLK; ptr++) {
		if (sc->clk_bus[ptr]) {
			clk_disable(sc->clk_bus[ptr]);
			clk_put(sc->clk_bus[ptr]);
		}
	}
	clk_disable(sc->clk_io);
	clk_put(sc->clk_io);

<<<<<<< HEAD
	iounmap(host->ioaddr);
	release_resource(sc->ioarea);
	kfree(sc->ioarea);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	sdhci_free_host(host);
	platform_set_drvdata(pdev, NULL);

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM

static int sdhci_s3c_suspend(struct platform_device *dev, pm_message_t pm)
{
	struct sdhci_host *host = platform_get_drvdata(dev);

	sdhci_suspend_host(host, pm);
	return 0;
}

static int sdhci_s3c_resume(struct platform_device *dev)
{
	struct sdhci_host *host = platform_get_drvdata(dev);

	sdhci_resume_host(host);
	return 0;
}

#else
#define sdhci_s3c_suspend NULL
#define sdhci_s3c_resume NULL
#endif

static struct platform_driver sdhci_s3c_driver = {
	.probe		= sdhci_s3c_probe,
	.remove		= __devexit_p(sdhci_s3c_remove),
	.suspend	= sdhci_s3c_suspend,
	.resume	        = sdhci_s3c_resume,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "s3c-sdhci",
	},
};

static int __init sdhci_s3c_init(void)
{
	return platform_driver_register(&sdhci_s3c_driver);
}

static void __exit sdhci_s3c_exit(void)
{
	platform_driver_unregister(&sdhci_s3c_driver);
}

module_init(sdhci_s3c_init);
module_exit(sdhci_s3c_exit);
=======
=======
#ifdef CONFIG_PM_RUNTIME
	if (pdata->cd_type != S3C_SDHCI_CD_INTERNAL)
		clk_prepare_enable(sc->clk_io);
#endif
	sdhci_remove_host(host, 1);

	pm_runtime_dont_use_autosuspend(&pdev->dev);
	pm_runtime_disable(&pdev->dev);

#ifndef CONFIG_PM_RUNTIME
	clk_disable_unprepare(sc->clk_bus[sc->cur_clk]);
#endif
	clk_disable_unprepare(sc->clk_io);

	sdhci_free_host(host);

	return 0;
}

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PM_SLEEP
static int sdhci_s3c_suspend(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);

	return sdhci_suspend_host(host);
}

static int sdhci_s3c_resume(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);

	return sdhci_resume_host(host);
}
#endif

#ifdef CONFIG_PM_RUNTIME
static int sdhci_s3c_runtime_suspend(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);
<<<<<<< HEAD

	return sdhci_runtime_suspend_host(host);
=======
	struct sdhci_s3c *ourhost = to_s3c(host);
	struct clk *busclk = ourhost->clk_io;
	int ret;

	ret = sdhci_runtime_suspend_host(host);

	clk_disable_unprepare(ourhost->clk_bus[ourhost->cur_clk]);
	clk_disable_unprepare(busclk);
	return ret;
>>>>>>> refs/remotes/origin/master
}

static int sdhci_s3c_runtime_resume(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);
<<<<<<< HEAD

	return sdhci_runtime_resume_host(host);
=======
	struct sdhci_s3c *ourhost = to_s3c(host);
	struct clk *busclk = ourhost->clk_io;
	int ret;

	clk_prepare_enable(busclk);
	clk_prepare_enable(ourhost->clk_bus[ourhost->cur_clk]);
	ret = sdhci_runtime_resume_host(host);
	return ret;
>>>>>>> refs/remotes/origin/master
}
#endif

#ifdef CONFIG_PM
static const struct dev_pm_ops sdhci_s3c_pmops = {
	SET_SYSTEM_SLEEP_PM_OPS(sdhci_s3c_suspend, sdhci_s3c_resume)
	SET_RUNTIME_PM_OPS(sdhci_s3c_runtime_suspend, sdhci_s3c_runtime_resume,
			   NULL)
};

#define SDHCI_S3C_PMOPS (&sdhci_s3c_pmops)

#else
#define SDHCI_S3C_PMOPS NULL
#endif

#if defined(CONFIG_CPU_EXYNOS4210) || defined(CONFIG_SOC_EXYNOS4212)
static struct sdhci_s3c_drv_data exynos4_sdhci_drv_data = {
	.sdhci_quirks = SDHCI_QUIRK_NONSTANDARD_CLOCK,
};
#define EXYNOS4_SDHCI_DRV_DATA ((kernel_ulong_t)&exynos4_sdhci_drv_data)
#else
#define EXYNOS4_SDHCI_DRV_DATA ((kernel_ulong_t)NULL)
#endif

static struct platform_device_id sdhci_s3c_driver_ids[] = {
	{
		.name		= "s3c-sdhci",
		.driver_data	= (kernel_ulong_t)NULL,
	}, {
		.name		= "exynos4-sdhci",
		.driver_data	= EXYNOS4_SDHCI_DRV_DATA,
	},
	{ }
};
MODULE_DEVICE_TABLE(platform, sdhci_s3c_driver_ids);

<<<<<<< HEAD
static struct platform_driver sdhci_s3c_driver = {
	.probe		= sdhci_s3c_probe,
	.remove		= __devexit_p(sdhci_s3c_remove),
=======
#ifdef CONFIG_OF
static const struct of_device_id sdhci_s3c_dt_match[] = {
	{ .compatible = "samsung,s3c6410-sdhci", },
	{ .compatible = "samsung,exynos4210-sdhci",
		.data = (void *)EXYNOS4_SDHCI_DRV_DATA },
	{},
};
MODULE_DEVICE_TABLE(of, sdhci_s3c_dt_match);
#endif

static struct platform_driver sdhci_s3c_driver = {
	.probe		= sdhci_s3c_probe,
	.remove		= sdhci_s3c_remove,
>>>>>>> refs/remotes/origin/master
	.id_table	= sdhci_s3c_driver_ids,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "s3c-sdhci",
<<<<<<< HEAD
=======
		.of_match_table = of_match_ptr(sdhci_s3c_dt_match),
>>>>>>> refs/remotes/origin/master
		.pm	= SDHCI_S3C_PMOPS,
	},
};

module_platform_driver(sdhci_s3c_driver);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("Samsung SDHCI (HSMMC) glue");
MODULE_AUTHOR("Ben Dooks, <ben@simtec.co.uk>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:s3c-sdhci");
