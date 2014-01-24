/*
 * linux/arch/arm/mach-sa1100/neponset.c
<<<<<<< HEAD
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/ioport.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/irq.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>
#include <asm/mach/serial_sa1100.h>
#include <mach/assabet.h>
#include <mach/neponset.h>
#include <asm/hardware/sa1111.h>
#include <asm/sizes.h>

/*
 * Install handler for Neponset IRQ.  Note that we have to loop here
 * since the ETHERNET and USAR IRQs are level based, and we need to
 * ensure that the IRQ signal is deasserted before returning.  This
 * is rather unfortunate.
 */
static void
neponset_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	unsigned int irr;

	while (1) {
		/*
		 * Acknowledge the parent IRQ.
		 */
		desc->irq_data.chip->irq_ack(&desc->irq_data);

		/*
		 * Read the interrupt reason register.  Let's have all
		 * active IRQ bits high.  Note: there is a typo in the
		 * Neponset user's guide for the SA1111 IRR level.
		 */
		irr = IRR ^ (IRR_ETHERNET | IRR_USAR);

		if ((irr & (IRR_ETHERNET | IRR_USAR | IRR_SA1111)) == 0)
			break;

		/*
		 * Since there is no individual mask, we have to
		 * mask the parent IRQ.  This is safe, since we'll
		 * recheck the register for any pending IRQs.
		 */
		if (irr & (IRR_ETHERNET | IRR_USAR)) {
			desc->irq_data.chip->irq_mask(&desc->irq_data);

			/*
			 * Ack the interrupt now to prevent re-entering
			 * this neponset handler.  Again, this is safe
			 * since we'll check the IRR register prior to
			 * leaving.
			 */
			desc->irq_data.chip->irq_ack(&desc->irq_data);

			if (irr & IRR_ETHERNET) {
				generic_handle_irq(IRQ_NEPONSET_SMC9196);
			}

			if (irr & IRR_USAR) {
				generic_handle_irq(IRQ_NEPONSET_USAR);
			}

			desc->irq_data.chip->irq_unmask(&desc->irq_data);
		}

		if (irr & IRR_SA1111) {
			generic_handle_irq(IRQ_NEPONSET_SA1111);
		}
=======
 */
#include <linux/err.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/serial_core.h>
#include <linux/slab.h>

#include <asm/mach-types.h>
#include <asm/mach/map.h>
#include <asm/mach/serial_sa1100.h>
#include <asm/hardware/sa1111.h>
#include <asm/sizes.h>

#include <mach/hardware.h>
#include <mach/assabet.h>
#include <mach/neponset.h>
#include <mach/irqs.h>

#define NEP_IRQ_SMC91X	0
#define NEP_IRQ_USAR	1
#define NEP_IRQ_SA1111	2
#define NEP_IRQ_NR	3

#define WHOAMI		0x00
#define LEDS		0x10
#define SWPK		0x20
#define IRR		0x24
#define KP_Y_IN		0x80
#define KP_X_OUT	0x90
#define NCR_0		0xa0
#define MDM_CTL_0	0xb0
#define MDM_CTL_1	0xb4
#define AUD_CTL		0xc0

#define IRR_ETHERNET	(1 << 0)
#define IRR_USAR	(1 << 1)
#define IRR_SA1111	(1 << 2)

#define MDM_CTL0_RTS1	(1 << 0)
#define MDM_CTL0_DTR1	(1 << 1)
#define MDM_CTL0_RTS2	(1 << 2)
#define MDM_CTL0_DTR2	(1 << 3)

#define MDM_CTL1_CTS1	(1 << 0)
#define MDM_CTL1_DSR1	(1 << 1)
#define MDM_CTL1_DCD1	(1 << 2)
#define MDM_CTL1_CTS2	(1 << 3)
#define MDM_CTL1_DSR2	(1 << 4)
#define MDM_CTL1_DCD2	(1 << 5)

#define AUD_SEL_1341	(1 << 0)
#define AUD_MUTE_1341	(1 << 1)

extern void sa1110_mb_disable(void);

struct neponset_drvdata {
	void __iomem *base;
	struct platform_device *sa1111;
	struct platform_device *smc91x;
	unsigned irq_base;
#ifdef CONFIG_PM_SLEEP
	u32 ncr0;
	u32 mdm_ctl_0;
#endif
};

static void __iomem *nep_base;

void neponset_ncr_frob(unsigned int mask, unsigned int val)
{
	void __iomem *base = nep_base;

	if (base) {
		unsigned long flags;
		unsigned v;

		local_irq_save(flags);
		v = readb_relaxed(base + NCR_0);
		writeb_relaxed((v & ~mask) | val, base + NCR_0);
		local_irq_restore(flags);
	} else {
		WARN(1, "nep_base unset\n");
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

static void neponset_set_mctrl(struct uart_port *port, u_int mctrl)
{
<<<<<<< HEAD
	u_int mdm_ctl0 = MDM_CTL_0;

=======
	void __iomem *base = nep_base;
	u_int mdm_ctl0;

	if (!base)
		return;

	mdm_ctl0 = readb_relaxed(base + MDM_CTL_0);
>>>>>>> refs/remotes/origin/cm-10.0
	if (port->mapbase == _Ser1UTCR0) {
		if (mctrl & TIOCM_RTS)
			mdm_ctl0 &= ~MDM_CTL0_RTS2;
		else
			mdm_ctl0 |= MDM_CTL0_RTS2;

		if (mctrl & TIOCM_DTR)
			mdm_ctl0 &= ~MDM_CTL0_DTR2;
		else
			mdm_ctl0 |= MDM_CTL0_DTR2;
	} else if (port->mapbase == _Ser3UTCR0) {
		if (mctrl & TIOCM_RTS)
			mdm_ctl0 &= ~MDM_CTL0_RTS1;
		else
			mdm_ctl0 |= MDM_CTL0_RTS1;

		if (mctrl & TIOCM_DTR)
			mdm_ctl0 &= ~MDM_CTL0_DTR1;
		else
			mdm_ctl0 |= MDM_CTL0_DTR1;
	}

<<<<<<< HEAD
	MDM_CTL_0 = mdm_ctl0;
=======
	writeb_relaxed(mdm_ctl0, base + MDM_CTL_0);
>>>>>>> refs/remotes/origin/cm-10.0
}

static u_int neponset_get_mctrl(struct uart_port *port)
{
<<<<<<< HEAD
	u_int ret = TIOCM_CD | TIOCM_CTS | TIOCM_DSR;
	u_int mdm_ctl1 = MDM_CTL_1;

=======
	void __iomem *base = nep_base;
	u_int ret = TIOCM_CD | TIOCM_CTS | TIOCM_DSR;
	u_int mdm_ctl1;

	if (!base)
		return ret;

	mdm_ctl1 = readb_relaxed(base + MDM_CTL_1);
>>>>>>> refs/remotes/origin/cm-10.0
	if (port->mapbase == _Ser1UTCR0) {
		if (mdm_ctl1 & MDM_CTL1_DCD2)
			ret &= ~TIOCM_CD;
		if (mdm_ctl1 & MDM_CTL1_CTS2)
			ret &= ~TIOCM_CTS;
		if (mdm_ctl1 & MDM_CTL1_DSR2)
			ret &= ~TIOCM_DSR;
	} else if (port->mapbase == _Ser3UTCR0) {
		if (mdm_ctl1 & MDM_CTL1_DCD1)
			ret &= ~TIOCM_CD;
		if (mdm_ctl1 & MDM_CTL1_CTS1)
			ret &= ~TIOCM_CTS;
		if (mdm_ctl1 & MDM_CTL1_DSR1)
			ret &= ~TIOCM_DSR;
	}

	return ret;
}

static struct sa1100_port_fns neponset_port_fns __devinitdata = {
	.set_mctrl	= neponset_set_mctrl,
	.get_mctrl	= neponset_get_mctrl,
};

<<<<<<< HEAD
static int __devinit neponset_probe(struct platform_device *dev)
{
	sa1100_register_uart_fns(&neponset_port_fns);

	/*
	 * Install handler for GPIO25.
	 */
	irq_set_irq_type(IRQ_GPIO25, IRQ_TYPE_EDGE_RISING);
	irq_set_chained_handler(IRQ_GPIO25, neponset_irq_handler);

	/*
	 * We would set IRQ_GPIO25 to be a wake-up IRQ, but
	 * unfortunately something on the Neponset activates
	 * this IRQ on sleep (ethernet?)
	 */
#if 0
	enable_irq_wake(IRQ_GPIO25);
#endif

	/*
	 * Setup other Neponset IRQs.  SA1111 will be done by the
	 * generic SA1111 code.
	 */
	irq_set_handler(IRQ_NEPONSET_SMC9196, handle_simple_irq);
	set_irq_flags(IRQ_NEPONSET_SMC9196, IRQF_VALID | IRQF_PROBE);
	irq_set_handler(IRQ_NEPONSET_USAR, handle_simple_irq);
	set_irq_flags(IRQ_NEPONSET_USAR, IRQF_VALID | IRQF_PROBE);

	/*
	 * Disable GPIO 0/1 drivers so the buttons work on the module.
	 */
	NCR_0 = NCR_GP01_OFF;

	return 0;
}

#ifdef CONFIG_PM

/*
 * LDM power management.
 */
static unsigned int neponset_saved_state;

static int neponset_suspend(struct platform_device *dev, pm_message_t state)
{
	/*
	 * Save state.
	 */
	neponset_saved_state = NCR_0;

	return 0;
}

static int neponset_resume(struct platform_device *dev)
{
	NCR_0 = neponset_saved_state;

	return 0;
}

#else
#define neponset_suspend NULL
#define neponset_resume  NULL
#endif

static struct platform_driver neponset_device_driver = {
	.probe		= neponset_probe,
	.suspend	= neponset_suspend,
	.resume		= neponset_resume,
	.driver		= {
		.name	= "neponset",
	},
};

static struct resource neponset_resources[] = {
	[0] = {
		.start	= 0x10000000,
		.end	= 0x17ffffff,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device neponset_device = {
	.name		= "neponset",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(neponset_resources),
	.resource	= neponset_resources,
};

static struct resource sa1111_resources[] = {
	[0] = {
		.start	= 0x40000000,
		.end	= 0x40001fff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_NEPONSET_SA1111,
		.end	= IRQ_NEPONSET_SA1111,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct sa1111_platform_data sa1111_info = {
	.irq_base	= IRQ_BOARD_END,
};

static u64 sa1111_dmamask = 0xffffffffUL;

static struct platform_device sa1111_device = {
	.name		= "sa1111",
	.id		= 0,
	.dev		= {
		.dma_mask = &sa1111_dmamask,
		.coherent_dma_mask = 0xffffffff,
		.platform_data = &sa1111_info,
	},
	.num_resources	= ARRAY_SIZE(sa1111_resources),
	.resource	= sa1111_resources,
};

static struct resource smc91x_resources[] = {
	[0] = {
		.name	= "smc91x-regs",
		.start	= SA1100_CS3_PHYS,
		.end	= SA1100_CS3_PHYS + 0x01ffffff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_NEPONSET_SMC9196,
		.end	= IRQ_NEPONSET_SMC9196,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.name	= "smc91x-attrib",
		.start	= SA1100_CS3_PHYS + 0x02000000,
		.end	= SA1100_CS3_PHYS + 0x03ffffff,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device smc91x_device = {
	.name		= "smc91x",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(smc91x_resources),
	.resource	= smc91x_resources,
};

static struct platform_device *devices[] __initdata = {
	&neponset_device,
	&sa1111_device,
	&smc91x_device,
};

extern void sa1110_mb_disable(void);

static int __init neponset_init(void)
{
	platform_driver_register(&neponset_device_driver);

	/*
	 * The Neponset is only present on the Assabet machine type.
	 */
	if (!machine_is_assabet())
		return -ENODEV;

	/*
	 * Ensure that the memory bus request/grant signals are setup,
	 * and the grant is held in its inactive state, whether or not
	 * we actually have a Neponset attached.
	 */
	sa1110_mb_disable();

	if (!machine_has_neponset()) {
		printk(KERN_DEBUG "Neponset expansion board not present\n");
		return -ENODEV;
	}

	if (WHOAMI != 0x11) {
		printk(KERN_WARNING "Neponset board detected, but "
			"wrong ID: %02x\n", WHOAMI);
		return -ENODEV;
	}

	return platform_add_devices(devices, ARRAY_SIZE(devices));
}

subsys_initcall(neponset_init);

static struct map_desc neponset_io_desc[] __initdata = {
	{	/* System Registers */
		.virtual	=  0xf3000000,
		.pfn		= __phys_to_pfn(0x10000000),
		.length		= SZ_1M,
		.type		= MT_DEVICE
	}, {	/* SA-1111 */
		.virtual	=  0xf4000000,
		.pfn		= __phys_to_pfn(0x40000000),
		.length		= SZ_1M,
		.type		= MT_DEVICE
	}
};

void __init neponset_map_io(void)
{
	iotable_init(neponset_io_desc, ARRAY_SIZE(neponset_io_desc));
}
=======
/*
 * Install handler for Neponset IRQ.  Note that we have to loop here
 * since the ETHERNET and USAR IRQs are level based, and we need to
 * ensure that the IRQ signal is deasserted before returning.  This
 * is rather unfortunate.
 */
static void neponset_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	struct neponset_drvdata *d = irq_desc_get_handler_data(desc);
	unsigned int irr;

	while (1) {
		/*
		 * Acknowledge the parent IRQ.
		 */
		desc->irq_data.chip->irq_ack(&desc->irq_data);

		/*
		 * Read the interrupt reason register.  Let's have all
		 * active IRQ bits high.  Note: there is a typo in the
		 * Neponset user's guide for the SA1111 IRR level.
		 */
		irr = readb_relaxed(d->base + IRR);
		irr ^= IRR_ETHERNET | IRR_USAR;

		if ((irr & (IRR_ETHERNET | IRR_USAR | IRR_SA1111)) == 0)
			break;

		/*
		 * Since there is no individual mask, we have to
		 * mask the parent IRQ.  This is safe, since we'll
		 * recheck the register for any pending IRQs.
		 */
		if (irr & (IRR_ETHERNET | IRR_USAR)) {
			desc->irq_data.chip->irq_mask(&desc->irq_data);

			/*
			 * Ack the interrupt now to prevent re-entering
			 * this neponset handler.  Again, this is safe
			 * since we'll check the IRR register prior to
			 * leaving.
			 */
			desc->irq_data.chip->irq_ack(&desc->irq_data);

			if (irr & IRR_ETHERNET)
				generic_handle_irq(d->irq_base + NEP_IRQ_SMC91X);

			if (irr & IRR_USAR)
				generic_handle_irq(d->irq_base + NEP_IRQ_USAR);

			desc->irq_data.chip->irq_unmask(&desc->irq_data);
		}

		if (irr & IRR_SA1111)
			generic_handle_irq(d->irq_base + NEP_IRQ_SA1111);
	}
}

/* Yes, we really do not have any kind of masking or unmasking */
static void nochip_noop(struct irq_data *irq)
{
}

static struct irq_chip nochip = {
	.name = "neponset",
	.irq_ack = nochip_noop,
	.irq_mask = nochip_noop,
	.irq_unmask = nochip_noop,
};

static struct sa1111_platform_data sa1111_info = {
	.disable_devs	= SA1111_DEVID_PS2_MSE,
};

static int __devinit neponset_probe(struct platform_device *dev)
{
	struct neponset_drvdata *d;
	struct resource *nep_res, *sa1111_res, *smc91x_res;
	struct resource sa1111_resources[] = {
		DEFINE_RES_MEM(0x40000000, SZ_8K),
		{ .flags = IORESOURCE_IRQ },
	};
	struct platform_device_info sa1111_devinfo = {
		.parent = &dev->dev,
		.name = "sa1111",
		.id = 0,
		.res = sa1111_resources,
		.num_res = ARRAY_SIZE(sa1111_resources),
		.data = &sa1111_info,
		.size_data = sizeof(sa1111_info),
		.dma_mask = 0xffffffffUL,
	};
	struct resource smc91x_resources[] = {
		DEFINE_RES_MEM_NAMED(SA1100_CS3_PHYS,
			0x02000000, "smc91x-regs"),
		DEFINE_RES_MEM_NAMED(SA1100_CS3_PHYS + 0x02000000,
			0x02000000, "smc91x-attrib"),
		{ .flags = IORESOURCE_IRQ },
	};
	struct platform_device_info smc91x_devinfo = {
		.parent = &dev->dev,
		.name = "smc91x",
		.id = 0,
		.res = smc91x_resources,
		.num_res = ARRAY_SIZE(smc91x_resources),
	};
	int ret, irq;

	if (nep_base)
		return -EBUSY;

	irq = ret = platform_get_irq(dev, 0);
	if (ret < 0)
		goto err_alloc;

	nep_res = platform_get_resource(dev, IORESOURCE_MEM, 0);
	smc91x_res = platform_get_resource(dev, IORESOURCE_MEM, 1);
	sa1111_res = platform_get_resource(dev, IORESOURCE_MEM, 2);
	if (!nep_res || !smc91x_res || !sa1111_res) {
		ret = -ENXIO;
		goto err_alloc;
	}

	d = kzalloc(sizeof(*d), GFP_KERNEL);
	if (!d) {
		ret = -ENOMEM;
		goto err_alloc;
	}

	d->base = ioremap(nep_res->start, SZ_4K);
	if (!d->base) {
		ret = -ENOMEM;
		goto err_ioremap;
	}

	if (readb_relaxed(d->base + WHOAMI) != 0x11) {
		dev_warn(&dev->dev, "Neponset board detected, but wrong ID: %02x\n",
			 readb_relaxed(d->base + WHOAMI));
		ret = -ENODEV;
		goto err_id;
	}

	ret = irq_alloc_descs(-1, IRQ_BOARD_START, NEP_IRQ_NR, -1);
	if (ret <= 0) {
		dev_err(&dev->dev, "unable to allocate %u irqs: %d\n",
			NEP_IRQ_NR, ret);
		if (ret == 0)
			ret = -ENOMEM;
		goto err_irq_alloc;
	}

	d->irq_base = ret;

	irq_set_chip_and_handler(d->irq_base + NEP_IRQ_SMC91X, &nochip,
		handle_simple_irq);
	set_irq_flags(d->irq_base + NEP_IRQ_SMC91X, IRQF_VALID | IRQF_PROBE);
	irq_set_chip_and_handler(d->irq_base + NEP_IRQ_USAR, &nochip,
		handle_simple_irq);
	set_irq_flags(d->irq_base + NEP_IRQ_USAR, IRQF_VALID | IRQF_PROBE);
	irq_set_chip(d->irq_base + NEP_IRQ_SA1111, &nochip);

	irq_set_irq_type(irq, IRQ_TYPE_EDGE_RISING);
	irq_set_handler_data(irq, d);
	irq_set_chained_handler(irq, neponset_irq_handler);

	/*
	 * We would set IRQ_GPIO25 to be a wake-up IRQ, but unfortunately
	 * something on the Neponset activates this IRQ on sleep (eth?)
	 */
#if 0
	enable_irq_wake(irq);
#endif

	dev_info(&dev->dev, "Neponset daughter board, providing IRQ%u-%u\n",
		 d->irq_base, d->irq_base + NEP_IRQ_NR - 1);
	nep_base = d->base;

	sa1100_register_uart_fns(&neponset_port_fns);

	/* Ensure that the memory bus request/grant signals are setup */
	sa1110_mb_disable();

	/* Disable GPIO 0/1 drivers so the buttons work on the Assabet */
	writeb_relaxed(NCR_GP01_OFF, d->base + NCR_0);

	sa1111_resources[0].parent = sa1111_res;
	sa1111_resources[1].start = d->irq_base + NEP_IRQ_SA1111;
	sa1111_resources[1].end = d->irq_base + NEP_IRQ_SA1111;
	d->sa1111 = platform_device_register_full(&sa1111_devinfo);

	smc91x_resources[0].parent = smc91x_res;
	smc91x_resources[1].parent = smc91x_res;
	smc91x_resources[2].start = d->irq_base + NEP_IRQ_SMC91X;
	smc91x_resources[2].end = d->irq_base + NEP_IRQ_SMC91X;
	d->smc91x = platform_device_register_full(&smc91x_devinfo);

	platform_set_drvdata(dev, d);

	return 0;

 err_irq_alloc:
 err_id:
	iounmap(d->base);
 err_ioremap:
	kfree(d);
 err_alloc:
	return ret;
}

static int __devexit neponset_remove(struct platform_device *dev)
{
	struct neponset_drvdata *d = platform_get_drvdata(dev);
	int irq = platform_get_irq(dev, 0);

	if (!IS_ERR(d->sa1111))
		platform_device_unregister(d->sa1111);
	if (!IS_ERR(d->smc91x))
		platform_device_unregister(d->smc91x);
	irq_set_chained_handler(irq, NULL);
	irq_free_descs(d->irq_base, NEP_IRQ_NR);
	nep_base = NULL;
	iounmap(d->base);
	kfree(d);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int neponset_suspend(struct device *dev)
{
	struct neponset_drvdata *d = dev_get_drvdata(dev);

	d->ncr0 = readb_relaxed(d->base + NCR_0);
	d->mdm_ctl_0 = readb_relaxed(d->base + MDM_CTL_0);

	return 0;
}

static int neponset_resume(struct device *dev)
{
	struct neponset_drvdata *d = dev_get_drvdata(dev);

	writeb_relaxed(d->ncr0, d->base + NCR_0);
	writeb_relaxed(d->mdm_ctl_0, d->base + MDM_CTL_0);

	return 0;
}

static const struct dev_pm_ops neponset_pm_ops = {
	.suspend_noirq = neponset_suspend,
	.resume_noirq = neponset_resume,
	.freeze_noirq = neponset_suspend,
	.restore_noirq = neponset_resume,
};
#define PM_OPS &neponset_pm_ops
#else
#define PM_OPS NULL
#endif

static struct platform_driver neponset_device_driver = {
	.probe		= neponset_probe,
	.remove		= __devexit_p(neponset_remove),
	.driver		= {
		.name	= "neponset",
		.owner	= THIS_MODULE,
		.pm	= PM_OPS,
	},
};

static int __init neponset_init(void)
{
	return platform_driver_register(&neponset_device_driver);
}

subsys_initcall(neponset_init);
>>>>>>> refs/remotes/origin/cm-10.0
