#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/irq.h>
#include <linux/dma-mapping.h>
#include <asm/prom.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/8xx_immap.h>

#include "mpc8xx_pic.h"


#define PIC_VEC_SPURRIOUS      15

extern int cpm_get_irq(struct pt_regs *regs);

<<<<<<< HEAD
static struct irq_host *mpc8xx_pic_host;
#define NR_MASK_WORDS   ((NR_IRQS + 31) / 32)
static unsigned long ppc_cached_irq_mask[NR_MASK_WORDS];
static sysconf8xx_t __iomem *siu_reg;

int cpm_get_irq(struct pt_regs *regs);

static void mpc8xx_unmask_irq(struct irq_data *d)
{
	int	bit, word;
	unsigned int irq_nr = (unsigned int)irqd_to_hwirq(d);

	bit = irq_nr & 0x1f;
	word = irq_nr >> 5;

	ppc_cached_irq_mask[word] |= (1 << (31-bit));
	out_be32(&siu_reg->sc_simask, ppc_cached_irq_mask[word]);
=======
static struct irq_domain *mpc8xx_pic_host;
static unsigned long mpc8xx_cached_irq_mask;
static sysconf8xx_t __iomem *siu_reg;

static inline unsigned long mpc8xx_irqd_to_bit(struct irq_data *d)
{
	return 0x80000000 >> irqd_to_hwirq(d);
}

static void mpc8xx_unmask_irq(struct irq_data *d)
{
	mpc8xx_cached_irq_mask |= mpc8xx_irqd_to_bit(d);
	out_be32(&siu_reg->sc_simask, mpc8xx_cached_irq_mask);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void mpc8xx_mask_irq(struct irq_data *d)
{
<<<<<<< HEAD
	int	bit, word;
	unsigned int irq_nr = (unsigned int)irqd_to_hwirq(d);

	bit = irq_nr & 0x1f;
	word = irq_nr >> 5;

	ppc_cached_irq_mask[word] &= ~(1 << (31-bit));
	out_be32(&siu_reg->sc_simask, ppc_cached_irq_mask[word]);
=======
	mpc8xx_cached_irq_mask &= ~mpc8xx_irqd_to_bit(d);
	out_be32(&siu_reg->sc_simask, mpc8xx_cached_irq_mask);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void mpc8xx_ack(struct irq_data *d)
{
<<<<<<< HEAD
	int	bit;
	unsigned int irq_nr = (unsigned int)irqd_to_hwirq(d);

	bit = irq_nr & 0x1f;
	out_be32(&siu_reg->sc_sipend, 1 << (31-bit));
=======
	out_be32(&siu_reg->sc_sipend, mpc8xx_irqd_to_bit(d));
>>>>>>> refs/remotes/origin/cm-10.0
}

static void mpc8xx_end_irq(struct irq_data *d)
{
<<<<<<< HEAD
	int bit, word;
	unsigned int irq_nr = (unsigned int)irqd_to_hwirq(d);

	bit = irq_nr & 0x1f;
	word = irq_nr >> 5;

	ppc_cached_irq_mask[word] |= (1 << (31-bit));
	out_be32(&siu_reg->sc_simask, ppc_cached_irq_mask[word]);
=======
	mpc8xx_cached_irq_mask |= mpc8xx_irqd_to_bit(d);
	out_be32(&siu_reg->sc_simask, mpc8xx_cached_irq_mask);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int mpc8xx_set_irq_type(struct irq_data *d, unsigned int flow_type)
{
<<<<<<< HEAD
	if (flow_type & IRQ_TYPE_EDGE_FALLING) {
		irq_hw_number_t hw = (unsigned int)irqd_to_hwirq(d);
		unsigned int siel = in_be32(&siu_reg->sc_siel);

		/* only external IRQ senses are programmable */
		if ((hw & 1) == 0) {
			siel |= (0x80000000 >> hw);
			out_be32(&siu_reg->sc_siel, siel);
			__irq_set_handler_locked(d->irq, handle_edge_irq);
		}
=======
	/* only external IRQ senses are programmable */
	if ((flow_type & IRQ_TYPE_EDGE_FALLING) && !(irqd_to_hwirq(d) & 1)) {
		unsigned int siel = in_be32(&siu_reg->sc_siel);
		siel |= mpc8xx_irqd_to_bit(d);
		out_be32(&siu_reg->sc_siel, siel);
		__irq_set_handler_locked(d->irq, handle_edge_irq);
>>>>>>> refs/remotes/origin/cm-10.0
	}
	return 0;
}

static struct irq_chip mpc8xx_pic = {
	.name = "MPC8XX SIU",
	.irq_unmask = mpc8xx_unmask_irq,
	.irq_mask = mpc8xx_mask_irq,
	.irq_ack = mpc8xx_ack,
	.irq_eoi = mpc8xx_end_irq,
	.irq_set_type = mpc8xx_set_irq_type,
};

unsigned int mpc8xx_get_irq(void)
{
	int irq;

	/* For MPC8xx, read the SIVEC register and shift the bits down
	 * to get the irq number.
	 */
	irq = in_be32(&siu_reg->sc_sivec) >> 26;

	if (irq == PIC_VEC_SPURRIOUS)
		irq = NO_IRQ;

        return irq_linear_revmap(mpc8xx_pic_host, irq);

}

<<<<<<< HEAD
static int mpc8xx_pic_host_map(struct irq_host *h, unsigned int virq,
=======
static int mpc8xx_pic_host_map(struct irq_domain *h, unsigned int virq,
>>>>>>> refs/remotes/origin/cm-10.0
			  irq_hw_number_t hw)
{
	pr_debug("mpc8xx_pic_host_map(%d, 0x%lx)\n", virq, hw);

	/* Set default irq handle */
	irq_set_chip_and_handler(virq, &mpc8xx_pic, handle_level_irq);
	return 0;
}


<<<<<<< HEAD
static int mpc8xx_pic_host_xlate(struct irq_host *h, struct device_node *ct,
=======
static int mpc8xx_pic_host_xlate(struct irq_domain *h, struct device_node *ct,
>>>>>>> refs/remotes/origin/cm-10.0
			    const u32 *intspec, unsigned int intsize,
			    irq_hw_number_t *out_hwirq, unsigned int *out_flags)
{
	static unsigned char map_pic_senses[4] = {
		IRQ_TYPE_EDGE_RISING,
		IRQ_TYPE_LEVEL_LOW,
		IRQ_TYPE_LEVEL_HIGH,
		IRQ_TYPE_EDGE_FALLING,
	};

<<<<<<< HEAD
=======
	if (intspec[0] > 0x1f)
		return 0;

>>>>>>> refs/remotes/origin/cm-10.0
	*out_hwirq = intspec[0];
	if (intsize > 1 && intspec[1] < 4)
		*out_flags = map_pic_senses[intspec[1]];
	else
		*out_flags = IRQ_TYPE_NONE;

	return 0;
}


<<<<<<< HEAD
static struct irq_host_ops mpc8xx_pic_host_ops = {
=======
static struct irq_domain_ops mpc8xx_pic_host_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
	.map = mpc8xx_pic_host_map,
	.xlate = mpc8xx_pic_host_xlate,
};

int mpc8xx_pic_init(void)
{
	struct resource res;
	struct device_node *np;
	int ret;

	np = of_find_compatible_node(NULL, NULL, "fsl,pq1-pic");
	if (np == NULL)
		np = of_find_node_by_type(NULL, "mpc8xx-pic");
	if (np == NULL) {
		printk(KERN_ERR "Could not find fsl,pq1-pic node\n");
		return -ENOMEM;
	}

	ret = of_address_to_resource(np, 0, &res);
	if (ret)
		goto out;

<<<<<<< HEAD
	siu_reg = ioremap(res.start, res.end - res.start + 1);
=======
	siu_reg = ioremap(res.start, resource_size(&res));
>>>>>>> refs/remotes/origin/cm-10.0
	if (siu_reg == NULL) {
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	mpc8xx_pic_host = irq_alloc_host(np, IRQ_HOST_MAP_LINEAR,
					 64, &mpc8xx_pic_host_ops, 64);
=======
	mpc8xx_pic_host = irq_domain_add_linear(np, 64, &mpc8xx_pic_host_ops, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
	if (mpc8xx_pic_host == NULL) {
		printk(KERN_ERR "MPC8xx PIC: failed to allocate irq host!\n");
		ret = -ENOMEM;
		goto out;
	}
	return 0;

out:
	of_node_put(np);
	return ret;
}
