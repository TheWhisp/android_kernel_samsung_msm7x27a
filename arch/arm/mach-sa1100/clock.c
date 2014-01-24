/*
 *  linux/arch/arm/mach-sa1100/clock.c
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/clk.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include <mach/hardware.h>

/*
 * Very simple clock implementation - we only have one clock to deal with.
 */
struct clk {
	unsigned int		enabled;
};

static void clk_gpio27_enable(void)
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/io.h>
#include <linux/clkdev.h>

#include <mach/hardware.h>

struct clkops {
	void			(*enable)(struct clk *);
	void			(*disable)(struct clk *);
};

struct clk {
	const struct clkops	*ops;
	unsigned int		enabled;
};

#define DEFINE_CLK(_name, _ops)				\
struct clk clk_##_name = {				\
		.ops	= _ops,				\
	}

static DEFINE_SPINLOCK(clocks_lock);

static void clk_gpio27_enable(struct clk *clk)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	/*
	 * First, set up the 3.6864MHz clock on GPIO 27 for the SA-1111:
	 * (SA-1110 Developer's Manual, section 9.1.2.1)
	 */
	GAFR |= GPIO_32_768kHz;
	GPDR |= GPIO_32_768kHz;
	TUCR = TUCR_3_6864MHz;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void clk_gpio27_disable(void)
=======
static void clk_gpio27_disable(struct clk *clk)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void clk_gpio27_disable(struct clk *clk)
>>>>>>> refs/remotes/origin/master
{
	TUCR = 0;
	GPDR &= ~GPIO_32_768kHz;
	GAFR &= ~GPIO_32_768kHz;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk clk_gpio27;

static DEFINE_SPINLOCK(clocks_lock);

struct clk *clk_get(struct device *dev, const char *id)
{
	const char *devname = dev_name(dev);

	return strcmp(devname, "sa1111.0") ? ERR_PTR(-ENOENT) : &clk_gpio27;
}
EXPORT_SYMBOL(clk_get);

void clk_put(struct clk *clk)
{
}
EXPORT_SYMBOL(clk_put);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
int clk_enable(struct clk *clk)
{
	unsigned long flags;

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_irqsave(&clocks_lock, flags);
	if (clk->enabled++ == 0)
		clk_gpio27_enable();
	spin_unlock_irqrestore(&clocks_lock, flags);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (clk) {
		spin_lock_irqsave(&clocks_lock, flags);
		if (clk->enabled++ == 0)
			clk->ops->enable(clk);
		spin_unlock_irqrestore(&clocks_lock, flags);
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}
EXPORT_SYMBOL(clk_enable);

void clk_disable(struct clk *clk)
{
	unsigned long flags;

<<<<<<< HEAD
<<<<<<< HEAD
	WARN_ON(clk->enabled == 0);

	spin_lock_irqsave(&clocks_lock, flags);
	if (--clk->enabled == 0)
		clk_gpio27_disable();
	spin_unlock_irqrestore(&clocks_lock, flags);
}
EXPORT_SYMBOL(clk_disable);

unsigned long clk_get_rate(struct clk *clk)
{
	return 3686400;
}
EXPORT_SYMBOL(clk_get_rate);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (clk) {
		WARN_ON(clk->enabled == 0);
		spin_lock_irqsave(&clocks_lock, flags);
		if (--clk->enabled == 0)
			clk->ops->disable(clk);
		spin_unlock_irqrestore(&clocks_lock, flags);
	}
}
EXPORT_SYMBOL(clk_disable);

const struct clkops clk_gpio27_ops = {
	.enable		= clk_gpio27_enable,
	.disable	= clk_gpio27_disable,
};

static DEFINE_CLK(gpio27, &clk_gpio27_ops);

static struct clk_lookup sa11xx_clkregs[] = {
	CLKDEV_INIT("sa1111.0", NULL, &clk_gpio27),
	CLKDEV_INIT("sa1100-rtc", NULL, NULL),
};

static int __init sa11xx_clk_init(void)
{
	clkdev_add_table(sa11xx_clkregs, ARRAY_SIZE(sa11xx_clkregs));
	return 0;
}
core_initcall(sa11xx_clk_init);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
