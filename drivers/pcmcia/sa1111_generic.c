/*
 * linux/drivers/pcmcia/sa1111_generic.c
 *
 * We implement the generic parts of a SA1111 PCMCIA driver.  This
 * basically means we handle everything except controlling the
 * power.  Power is machine specific...
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/slab.h>

#include <pcmcia/ss.h>

#include <mach/hardware.h>
#include <asm/hardware/sa1111.h>
#include <asm/irq.h>

#include "sa1111_generic.h"

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * These are offsets from the above base.
 */
#define PCCR	0x0000
#define PCSSR	0x0004
#define PCSR	0x0008

#define PCSR_S0_READY	(1<<0)
#define PCSR_S1_READY	(1<<1)
#define PCSR_S0_DETECT	(1<<2)
#define PCSR_S1_DETECT	(1<<3)
#define PCSR_S0_VS1	(1<<4)
#define PCSR_S0_VS2	(1<<5)
#define PCSR_S1_VS1	(1<<6)
#define PCSR_S1_VS2	(1<<7)
#define PCSR_S0_WP	(1<<8)
#define PCSR_S1_WP	(1<<9)
#define PCSR_S0_BVD1	(1<<10)
#define PCSR_S0_BVD2	(1<<11)
#define PCSR_S1_BVD1	(1<<12)
#define PCSR_S1_BVD2	(1<<13)

#define PCCR_S0_RST	(1<<0)
#define PCCR_S1_RST	(1<<1)
#define PCCR_S0_FLT	(1<<2)
#define PCCR_S1_FLT	(1<<3)
#define PCCR_S0_PWAITEN	(1<<4)
#define PCCR_S1_PWAITEN	(1<<5)
#define PCCR_S0_PSE	(1<<6)
#define PCCR_S1_PSE	(1<<7)

#define PCSSR_S0_SLEEP	(1<<0)
#define PCSSR_S1_SLEEP	(1<<1)

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define IDX_IRQ_S0_READY_NINT	(0)
#define IDX_IRQ_S0_CD_VALID	(1)
#define IDX_IRQ_S0_BVD1_STSCHG	(2)
#define IDX_IRQ_S1_READY_NINT	(3)
#define IDX_IRQ_S1_CD_VALID	(4)
#define IDX_IRQ_S1_BVD1_STSCHG	(5)

<<<<<<< HEAD
<<<<<<< HEAD
static struct pcmcia_irqs irqs[] = {
	{ 0, NO_IRQ, "SA1111 PCMCIA card detect" },
	{ 0, NO_IRQ, "SA1111 PCMCIA BVD1"        },
	{ 1, NO_IRQ, "SA1111 CF card detect"     },
	{ 1, NO_IRQ, "SA1111 CF BVD1"            },
};

static int sa1111_pcmcia_hw_init(struct soc_pcmcia_socket *skt)
{
	return soc_pcmcia_request_irqs(skt, irqs, ARRAY_SIZE(irqs));
}

static void sa1111_pcmcia_hw_shutdown(struct soc_pcmcia_socket *skt)
{
	soc_pcmcia_free_irqs(skt, irqs, ARRAY_SIZE(irqs));
}

void sa1111_pcmcia_socket_state(struct soc_pcmcia_socket *skt, struct pcmcia_state *state)
{
	struct sa1111_pcmcia_socket *s = to_skt(skt);
	unsigned long status = sa1111_readl(s->dev->mapbase + SA1111_PCSR);
=======
=======
>>>>>>> refs/remotes/origin/master
void sa1111_pcmcia_socket_state(struct soc_pcmcia_socket *skt, struct pcmcia_state *state)
{
	struct sa1111_pcmcia_socket *s = to_skt(skt);
	unsigned long status = sa1111_readl(s->dev->mapbase + PCSR);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	switch (skt->nr) {
	case 0:
		state->detect = status & PCSR_S0_DETECT ? 0 : 1;
		state->ready  = status & PCSR_S0_READY  ? 1 : 0;
		state->bvd1   = status & PCSR_S0_BVD1   ? 1 : 0;
		state->bvd2   = status & PCSR_S0_BVD2   ? 1 : 0;
		state->wrprot = status & PCSR_S0_WP     ? 1 : 0;
		state->vs_3v  = status & PCSR_S0_VS1    ? 0 : 1;
		state->vs_Xv  = status & PCSR_S0_VS2    ? 0 : 1;
		break;

	case 1:
		state->detect = status & PCSR_S1_DETECT ? 0 : 1;
		state->ready  = status & PCSR_S1_READY  ? 1 : 0;
		state->bvd1   = status & PCSR_S1_BVD1   ? 1 : 0;
		state->bvd2   = status & PCSR_S1_BVD2   ? 1 : 0;
		state->wrprot = status & PCSR_S1_WP     ? 1 : 0;
		state->vs_3v  = status & PCSR_S1_VS1    ? 0 : 1;
		state->vs_Xv  = status & PCSR_S1_VS2    ? 0 : 1;
		break;
	}
}

int sa1111_pcmcia_configure_socket(struct soc_pcmcia_socket *skt, const socket_state_t *state)
{
	struct sa1111_pcmcia_socket *s = to_skt(skt);
	unsigned int pccr_skt_mask, pccr_set_mask, val;
	unsigned long flags;

	switch (skt->nr) {
	case 0:
		pccr_skt_mask = PCCR_S0_RST|PCCR_S0_FLT|PCCR_S0_PWAITEN|PCCR_S0_PSE;
		break;

	case 1:
		pccr_skt_mask = PCCR_S1_RST|PCCR_S1_FLT|PCCR_S1_PWAITEN|PCCR_S1_PSE;
		break;

	default:
		return -1;
	}

	pccr_set_mask = 0;

	if (state->Vcc != 0)
		pccr_set_mask |= PCCR_S0_PWAITEN|PCCR_S1_PWAITEN;
	if (state->Vcc == 50)
		pccr_set_mask |= PCCR_S0_PSE|PCCR_S1_PSE;
	if (state->flags & SS_RESET)
		pccr_set_mask |= PCCR_S0_RST|PCCR_S1_RST;
	if (state->flags & SS_OUTPUT_ENA)
		pccr_set_mask |= PCCR_S0_FLT|PCCR_S1_FLT;

	local_irq_save(flags);
<<<<<<< HEAD
<<<<<<< HEAD
	val = sa1111_readl(s->dev->mapbase + SA1111_PCCR);
	val &= ~pccr_skt_mask;
	val |= pccr_set_mask & pccr_skt_mask;
	sa1111_writel(val, s->dev->mapbase + SA1111_PCCR);
=======
=======
>>>>>>> refs/remotes/origin/master
	val = sa1111_readl(s->dev->mapbase + PCCR);
	val &= ~pccr_skt_mask;
	val |= pccr_set_mask & pccr_skt_mask;
	sa1111_writel(val, s->dev->mapbase + PCCR);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	local_irq_restore(flags);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
void sa1111_pcmcia_socket_init(struct soc_pcmcia_socket *skt)
{
	soc_pcmcia_enable_irqs(skt, irqs, ARRAY_SIZE(irqs));
}

static void sa1111_pcmcia_socket_suspend(struct soc_pcmcia_socket *skt)
{
	soc_pcmcia_disable_irqs(skt, irqs, ARRAY_SIZE(irqs));
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
int sa1111_pcmcia_add(struct sa1111_dev *dev, struct pcmcia_low_level *ops,
	int (*add)(struct soc_pcmcia_socket *))
{
	struct sa1111_pcmcia_socket *s;
	int i, ret = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	ops->hw_init = sa1111_pcmcia_hw_init;
	ops->hw_shutdown = sa1111_pcmcia_hw_shutdown;
	ops->socket_state = sa1111_pcmcia_socket_state;
	ops->socket_suspend = sa1111_pcmcia_socket_suspend;
=======
	ops->socket_state = sa1111_pcmcia_socket_state;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ops->socket_state = sa1111_pcmcia_socket_state;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < ops->nr; i++) {
		s = kzalloc(sizeof(*s), GFP_KERNEL);
		if (!s)
			return -ENOMEM;

		s->soc.nr = ops->first + i;
<<<<<<< HEAD
<<<<<<< HEAD
		s->soc.ops = ops;
		s->soc.socket.owner = ops->owner;
		s->soc.socket.dev.parent = &dev->dev;
		s->soc.socket.pci_irq = s->soc.nr ?
				dev->irq[IDX_IRQ_S0_READY_NINT] :
				dev->irq[IDX_IRQ_S1_READY_NINT];
		s->dev = dev;
=======
=======
>>>>>>> refs/remotes/origin/master
		soc_pcmcia_init_one(&s->soc, ops, &dev->dev);
		s->dev = dev;
		if (s->soc.nr) {
			s->soc.socket.pci_irq = dev->irq[IDX_IRQ_S1_READY_NINT];
			s->soc.stat[SOC_STAT_CD].irq = dev->irq[IDX_IRQ_S1_CD_VALID];
			s->soc.stat[SOC_STAT_CD].name = "SA1111 CF card detect";
			s->soc.stat[SOC_STAT_BVD1].irq = dev->irq[IDX_IRQ_S1_BVD1_STSCHG];
			s->soc.stat[SOC_STAT_BVD1].name = "SA1111 CF BVD1";
		} else {
			s->soc.socket.pci_irq = dev->irq[IDX_IRQ_S0_READY_NINT];
			s->soc.stat[SOC_STAT_CD].irq = dev->irq[IDX_IRQ_S0_CD_VALID];
			s->soc.stat[SOC_STAT_CD].name = "SA1111 PCMCIA card detect";
			s->soc.stat[SOC_STAT_BVD1].irq = dev->irq[IDX_IRQ_S0_BVD1_STSCHG];
			s->soc.stat[SOC_STAT_BVD1].name = "SA1111 PCMCIA BVD1";
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		ret = add(&s->soc);
		if (ret == 0) {
			s->next = dev_get_drvdata(&dev->dev);
			dev_set_drvdata(&dev->dev, s);
		} else
			kfree(s);
	}

	return ret;
}

static int pcmcia_probe(struct sa1111_dev *dev)
{
	void __iomem *base;
<<<<<<< HEAD
<<<<<<< HEAD

	dev_set_drvdata(&dev->dev, NULL);

	if (!request_mem_region(dev->res.start, 512,
				SA1111_DRIVER_NAME(dev)))
		return -EBUSY;

	base = dev->mapbase;

	/* Initialize PCMCIA IRQs */
	irqs[0].irq = dev->irq[IDX_IRQ_S0_CD_VALID];
	irqs[1].irq = dev->irq[IDX_IRQ_S0_BVD1_STSCHG];
	irqs[2].irq = dev->irq[IDX_IRQ_S1_CD_VALID];
	irqs[3].irq = dev->irq[IDX_IRQ_S1_BVD1_STSCHG];

	/*
	 * Initialise the suspend state.
	 */
	sa1111_writel(PCSSR_S0_SLEEP | PCSSR_S1_SLEEP, base + SA1111_PCSSR);
	sa1111_writel(PCCR_S0_FLT | PCCR_S1_FLT, base + SA1111_PCCR);
=======
=======
>>>>>>> refs/remotes/origin/master
	int ret;

	ret = sa1111_enable_device(dev);
	if (ret)
		return ret;

	dev_set_drvdata(&dev->dev, NULL);

	if (!request_mem_region(dev->res.start, 512, SA1111_DRIVER_NAME(dev))) {
		sa1111_disable_device(dev);
		return -EBUSY;
	}

	base = dev->mapbase;

	/*
	 * Initialise the suspend state.
	 */
	sa1111_writel(PCSSR_S0_SLEEP | PCSSR_S1_SLEEP, base + PCSSR);
	sa1111_writel(PCCR_S0_FLT | PCCR_S1_FLT, base + PCCR);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_SA1100_BADGE4
	pcmcia_badge4_init(&dev->dev);
#endif
#ifdef CONFIG_SA1100_JORNADA720
	pcmcia_jornada720_init(&dev->dev);
#endif
#ifdef CONFIG_ARCH_LUBBOCK
	pcmcia_lubbock_init(dev);
#endif
#ifdef CONFIG_ASSABET_NEPONSET
	pcmcia_neponset_init(dev);
#endif
	return 0;
}

<<<<<<< HEAD
static int __devexit pcmcia_remove(struct sa1111_dev *dev)
=======
static int pcmcia_remove(struct sa1111_dev *dev)
>>>>>>> refs/remotes/origin/master
{
	struct sa1111_pcmcia_socket *next, *s = dev_get_drvdata(&dev->dev);

	dev_set_drvdata(&dev->dev, NULL);

<<<<<<< HEAD
<<<<<<< HEAD
	for (; next = s->next, s; s = next) {
=======
	for (; s; s = next) {
		next = s->next;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (; s; s = next) {
		next = s->next;
>>>>>>> refs/remotes/origin/master
		soc_pcmcia_remove_one(&s->soc);
		kfree(s);
	}

	release_mem_region(dev->res.start, 512);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	sa1111_disable_device(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sa1111_disable_device(dev);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct sa1111_driver pcmcia_driver = {
	.drv = {
		.name	= "sa1111-pcmcia",
	},
	.devid		= SA1111_DEVID_PCMCIA,
	.probe		= pcmcia_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(pcmcia_remove),
=======
	.remove		= pcmcia_remove,
>>>>>>> refs/remotes/origin/master
};

static int __init sa1111_drv_pcmcia_init(void)
{
	return sa1111_driver_register(&pcmcia_driver);
}

static void __exit sa1111_drv_pcmcia_exit(void)
{
	sa1111_driver_unregister(&pcmcia_driver);
}

fs_initcall(sa1111_drv_pcmcia_init);
module_exit(sa1111_drv_pcmcia_exit);

MODULE_DESCRIPTION("SA1111 PCMCIA card socket driver");
MODULE_LICENSE("GPL");
