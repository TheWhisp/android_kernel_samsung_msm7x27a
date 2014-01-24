/*
 * Copyright (C) 2007-2011 Freescale Semiconductor, Inc.
 *
 * Author: Tony Li <tony.li@freescale.com>
 *	   Jason Jin <Jason.jin@freescale.com>
 *
 * The hwirq alloc and free code reuse from sysdev/mpic_msi.c
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the
 * License.
 *
 */
#include <linux/irq.h>
#include <linux/bootmem.h>
#include <linux/msi.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/of_platform.h>
#include <sysdev/fsl_soc.h>
#include <asm/prom.h>
#include <asm/hw_irq.h>
#include <asm/ppc-pci.h>
#include <asm/mpic.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <asm/fsl_hcalls.h>

>>>>>>> refs/remotes/origin/cm-10.0
#include "fsl_msi.h"
#include "fsl_pci.h"

LIST_HEAD(msi_head);

struct fsl_msi_feature {
	u32 fsl_pic_ip;
<<<<<<< HEAD
	u32 msiir_offset;
=======
	u32 msiir_offset; /* Offset of MSIIR, relative to start of MSIR bank */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/fsl_hcalls.h>

#include "fsl_msi.h"
#include "fsl_pci.h"

#define MSIIR_OFFSET_MASK	0xfffff
#define MSIIR_IBS_SHIFT		0
#define MSIIR_SRS_SHIFT		5
#define MSIIR1_IBS_SHIFT	4
#define MSIIR1_SRS_SHIFT	0
#define MSI_SRS_MASK		0xf
#define MSI_IBS_MASK		0x1f

#define msi_hwirq(msi, msir_index, intr_index) \
		((msir_index) << (msi)->srs_shift | \
		 ((intr_index) << (msi)->ibs_shift))

static LIST_HEAD(msi_head);

struct fsl_msi_feature {
	u32 fsl_pic_ip;
	u32 msiir_offset; /* Offset of MSIIR, relative to start of MSIR bank */
>>>>>>> refs/remotes/origin/master
};

struct fsl_msi_cascade_data {
	struct fsl_msi *msi_data;
	int index;
};

static inline u32 fsl_msi_read(u32 __iomem *base, unsigned int reg)
{
	return in_be32(base + (reg >> 2));
}

/*
 * We do not need this actually. The MSIR register has been read once
 * in the cascade interrupt. So, this MSI interrupt has been acked
*/
static void fsl_msi_end_irq(struct irq_data *d)
{
}

static struct irq_chip fsl_msi_chip = {
	.irq_mask	= mask_msi_irq,
	.irq_unmask	= unmask_msi_irq,
	.irq_ack	= fsl_msi_end_irq,
	.name		= "FSL-MSI",
};

<<<<<<< HEAD
<<<<<<< HEAD
static int fsl_msi_host_map(struct irq_host *h, unsigned int virq,
=======
static int fsl_msi_host_map(struct irq_domain *h, unsigned int virq,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int fsl_msi_host_map(struct irq_domain *h, unsigned int virq,
>>>>>>> refs/remotes/origin/master
				irq_hw_number_t hw)
{
	struct fsl_msi *msi_data = h->host_data;
	struct irq_chip *chip = &fsl_msi_chip;

	irq_set_status_flags(virq, IRQ_TYPE_EDGE_FALLING);

	irq_set_chip_data(virq, msi_data);
	irq_set_chip_and_handler(virq, chip, handle_edge_irq);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct irq_host_ops fsl_msi_host_ops = {
=======
static const struct irq_domain_ops fsl_msi_host_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct irq_domain_ops fsl_msi_host_ops = {
>>>>>>> refs/remotes/origin/master
	.map = fsl_msi_host_map,
};

static int fsl_msi_init_allocator(struct fsl_msi *msi_data)
{
<<<<<<< HEAD
	int rc;

	rc = msi_bitmap_alloc(&msi_data->bitmap, NR_MSI_IRQS,
=======
	int rc, hwirq;

	rc = msi_bitmap_alloc(&msi_data->bitmap, NR_MSI_IRQS_MAX,
>>>>>>> refs/remotes/origin/master
			      msi_data->irqhost->of_node);
	if (rc)
		return rc;

<<<<<<< HEAD
	rc = msi_bitmap_reserve_dt_hwirqs(&msi_data->bitmap);
	if (rc < 0) {
		msi_bitmap_free(&msi_data->bitmap);
		return rc;
	}
=======
	/*
	 * Reserve all the hwirqs
	 * The available hwirqs will be released in fsl_msi_setup_hwirq()
	 */
	for (hwirq = 0; hwirq < NR_MSI_IRQS_MAX; hwirq++)
		msi_bitmap_reserve_hwirq(&msi_data->bitmap, hwirq);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int fsl_msi_check_device(struct pci_dev *pdev, int nvec, int type)
{
	if (type == PCI_CAP_ID_MSIX)
		pr_debug("fslmsi: MSI-X untested, trying anyway.\n");

	return 0;
}

static void fsl_teardown_msi_irqs(struct pci_dev *pdev)
{
	struct msi_desc *entry;
	struct fsl_msi *msi_data;

	list_for_each_entry(entry, &pdev->msi_list, list) {
		if (entry->irq == NO_IRQ)
			continue;
		msi_data = irq_get_chip_data(entry->irq);
		irq_set_msi_desc(entry->irq, NULL);
		msi_bitmap_free_hwirqs(&msi_data->bitmap,
				       virq_to_hw(entry->irq), 1);
		irq_dispose_mapping(entry->irq);
	}

	return;
}

static void fsl_compose_msi_msg(struct pci_dev *pdev, int hwirq,
				struct msi_msg *msg,
				struct fsl_msi *fsl_msi_data)
{
	struct fsl_msi *msi_data = fsl_msi_data;
	struct pci_controller *hose = pci_bus_to_host(pdev->bus);
<<<<<<< HEAD
<<<<<<< HEAD
	u64 base = fsl_pci_immrbar_base(hose);

	msg->address_lo = msi_data->msi_addr_lo + lower_32_bits(base);
	msg->address_hi = msi_data->msi_addr_hi + upper_32_bits(base);
=======
	u64 address; /* Physical address of the MSIIR */
	int len;
	const u64 *reg;
=======
	u64 address; /* Physical address of the MSIIR */
	int len;
	const __be64 *reg;
>>>>>>> refs/remotes/origin/master

	/* If the msi-address-64 property exists, then use it */
	reg = of_get_property(hose->dn, "msi-address-64", &len);
	if (reg && (len == sizeof(u64)))
		address = be64_to_cpup(reg);
	else
		address = fsl_pci_immrbar_base(hose) + msi_data->msiir_offset;

	msg->address_lo = lower_32_bits(address);
	msg->address_hi = upper_32_bits(address);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	msg->data = hwirq;

	pr_debug("%s: allocated srs: %d, ibs: %d\n",
		__func__, hwirq / IRQS_PER_MSI_REG, hwirq % IRQS_PER_MSI_REG);
=======

	msg->data = hwirq;

	pr_debug("%s: allocated srs: %d, ibs: %d\n", __func__,
		 (hwirq >> msi_data->srs_shift) & MSI_SRS_MASK,
		 (hwirq >> msi_data->ibs_shift) & MSI_IBS_MASK);
>>>>>>> refs/remotes/origin/master
}

static int fsl_setup_msi_irqs(struct pci_dev *pdev, int nvec, int type)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct pci_controller *hose = pci_bus_to_host(pdev->bus);
	struct device_node *np;
	phandle phandle = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct pci_controller *hose = pci_bus_to_host(pdev->bus);
	struct device_node *np;
	phandle phandle = 0;
>>>>>>> refs/remotes/origin/master
	int rc, hwirq = -ENOMEM;
	unsigned int virq;
	struct msi_desc *entry;
	struct msi_msg msg;
	struct fsl_msi *msi_data;

<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry(entry, &pdev->msi_list, list) {
		list_for_each_entry(msi_data, &msi_head, list) {
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * If the PCI node has an fsl,msi property, then we need to use it
	 * to find the specific MSI.
	 */
	np = of_parse_phandle(hose->dn, "fsl,msi", 0);
	if (np) {
		if (of_device_is_compatible(np, "fsl,mpic-msi") ||
		    of_device_is_compatible(np, "fsl,vmpic-msi"))
			phandle = np->phandle;
		else {
			dev_err(&pdev->dev,
				"node %s has an invalid fsl,msi phandle %u\n",
				hose->dn->full_name, np->phandle);
			return -EINVAL;
		}
	}

	list_for_each_entry(entry, &pdev->msi_list, list) {
		/*
		 * Loop over all the MSI devices until we find one that has an
		 * available interrupt.
		 */
		list_for_each_entry(msi_data, &msi_head, list) {
			/*
			 * If the PCI node has an fsl,msi property, then we
			 * restrict our search to the corresponding MSI node.
			 * The simplest way is to skip over MSI nodes with the
			 * wrong phandle. Under the Freescale hypervisor, this
			 * has the additional benefit of skipping over MSI
			 * nodes that are not mapped in the PAMU.
			 */
			if (phandle && (phandle != msi_data->phandle))
				continue;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			hwirq = msi_bitmap_alloc_hwirqs(&msi_data->bitmap, 1);
			if (hwirq >= 0)
				break;
		}

		if (hwirq < 0) {
			rc = hwirq;
<<<<<<< HEAD
<<<<<<< HEAD
			pr_debug("%s: fail allocating msi interrupt\n",
					__func__);
=======
			dev_err(&pdev->dev, "could not allocate MSI interrupt\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
			dev_err(&pdev->dev, "could not allocate MSI interrupt\n");
>>>>>>> refs/remotes/origin/master
			goto out_free;
		}

		virq = irq_create_mapping(msi_data->irqhost, hwirq);

		if (virq == NO_IRQ) {
<<<<<<< HEAD
<<<<<<< HEAD
			pr_debug("%s: fail mapping hwirq 0x%x\n",
					__func__, hwirq);
=======
			dev_err(&pdev->dev, "fail mapping hwirq %i\n", hwirq);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			dev_err(&pdev->dev, "fail mapping hwirq %i\n", hwirq);
>>>>>>> refs/remotes/origin/master
			msi_bitmap_free_hwirqs(&msi_data->bitmap, hwirq, 1);
			rc = -ENOSPC;
			goto out_free;
		}
		/* chip_data is msi_data via host->hostdata in host->map() */
		irq_set_msi_desc(virq, entry);

		fsl_compose_msi_msg(pdev, hwirq, &msg, msi_data);
		write_msi_msg(virq, &msg);
	}
	return 0;

out_free:
	/* free by the caller of this function */
	return rc;
}

static void fsl_msi_cascade(unsigned int irq, struct irq_desc *desc)
{
	struct irq_chip *chip = irq_desc_get_chip(desc);
	struct irq_data *idata = irq_desc_get_irq_data(desc);
	unsigned int cascade_irq;
	struct fsl_msi *msi_data;
	int msir_index = -1;
	u32 msir_value = 0;
	u32 intr_index;
	u32 have_shift = 0;
	struct fsl_msi_cascade_data *cascade_data;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	unsigned int ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	cascade_data = irq_get_handler_data(irq);
	msi_data = cascade_data->msi_data;

	raw_spin_lock(&desc->lock);
	if ((msi_data->feature &  FSL_PIC_IP_MASK) == FSL_PIC_IP_IPIC) {
		if (chip->irq_mask_ack)
			chip->irq_mask_ack(idata);
		else {
			chip->irq_mask(idata);
			chip->irq_ack(idata);
		}
	}

	if (unlikely(irqd_irq_inprogress(idata)))
		goto unlock;

	msir_index = cascade_data->index;

<<<<<<< HEAD
	if (msir_index >= NR_MSI_REG)
=======
	if (msir_index >= NR_MSI_REG_MAX)
>>>>>>> refs/remotes/origin/master
		cascade_irq = NO_IRQ;

	irqd_set_chained_irq_inprogress(idata);
	switch (msi_data->feature & FSL_PIC_IP_MASK) {
	case FSL_PIC_IP_MPIC:
		msir_value = fsl_msi_read(msi_data->msi_regs,
			msir_index * 0x10);
		break;
	case FSL_PIC_IP_IPIC:
		msir_value = fsl_msi_read(msi_data->msi_regs, msir_index * 0x4);
		break;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	case FSL_PIC_IP_VMPIC:
=======
#ifdef CONFIG_EPAPR_PARAVIRT
	case FSL_PIC_IP_VMPIC: {
		unsigned int ret;
>>>>>>> refs/remotes/origin/master
		ret = fh_vmpic_get_msir(virq_to_hw(irq), &msir_value);
		if (ret) {
			pr_err("fsl-msi: fh_vmpic_get_msir() failed for "
			       "irq %u (ret=%u)\n", irq, ret);
			msir_value = 0;
		}
		break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	}
#endif
>>>>>>> refs/remotes/origin/master
	}

	while (msir_value) {
		intr_index = ffs(msir_value) - 1;

		cascade_irq = irq_linear_revmap(msi_data->irqhost,
<<<<<<< HEAD
				msir_index * IRQS_PER_MSI_REG +
					intr_index + have_shift);
=======
				msi_hwirq(msi_data, msir_index,
					  intr_index + have_shift));
>>>>>>> refs/remotes/origin/master
		if (cascade_irq != NO_IRQ)
			generic_handle_irq(cascade_irq);
		have_shift += intr_index + 1;
		msir_value = msir_value >> (intr_index + 1);
	}
	irqd_clr_chained_irq_inprogress(idata);

	switch (msi_data->feature & FSL_PIC_IP_MASK) {
	case FSL_PIC_IP_MPIC:
<<<<<<< HEAD
<<<<<<< HEAD
=======
	case FSL_PIC_IP_VMPIC:
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case FSL_PIC_IP_VMPIC:
>>>>>>> refs/remotes/origin/master
		chip->irq_eoi(idata);
		break;
	case FSL_PIC_IP_IPIC:
		if (!irqd_irq_disabled(idata) && chip->irq_unmask)
			chip->irq_unmask(idata);
		break;
	}
unlock:
	raw_spin_unlock(&desc->lock);
}

static int fsl_of_msi_remove(struct platform_device *ofdev)
{
	struct fsl_msi *msi = platform_get_drvdata(ofdev);
	int virq, i;
	struct fsl_msi_cascade_data *cascade_data;

	if (msi->list.prev != NULL)
		list_del(&msi->list);
<<<<<<< HEAD
	for (i = 0; i < NR_MSI_REG; i++) {
=======
	for (i = 0; i < NR_MSI_REG_MAX; i++) {
>>>>>>> refs/remotes/origin/master
		virq = msi->msi_virqs[i];
		if (virq != NO_IRQ) {
			cascade_data = irq_get_handler_data(virq);
			kfree(cascade_data);
			irq_dispose_mapping(virq);
		}
	}
	if (msi->bitmap.bitmap)
		msi_bitmap_free(&msi->bitmap);
<<<<<<< HEAD
<<<<<<< HEAD
	iounmap(msi->msi_regs);
=======
	if ((msi->feature & FSL_PIC_IP_MASK) != FSL_PIC_IP_VMPIC)
		iounmap(msi->msi_regs);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if ((msi->feature & FSL_PIC_IP_MASK) != FSL_PIC_IP_VMPIC)
		iounmap(msi->msi_regs);
>>>>>>> refs/remotes/origin/master
	kfree(msi);

	return 0;
}

<<<<<<< HEAD
static int __devinit fsl_msi_setup_hwirq(struct fsl_msi *msi,
					 struct platform_device *dev,
					 int offset, int irq_index)
{
	struct fsl_msi_cascade_data *cascade_data = NULL;
	int virt_msir;
=======
static struct lock_class_key fsl_msi_irq_class;

static int fsl_msi_setup_hwirq(struct fsl_msi *msi, struct platform_device *dev,
			       int offset, int irq_index)
{
	struct fsl_msi_cascade_data *cascade_data = NULL;
	int virt_msir, i;
>>>>>>> refs/remotes/origin/master

	virt_msir = irq_of_parse_and_map(dev->dev.of_node, irq_index);
	if (virt_msir == NO_IRQ) {
		dev_err(&dev->dev, "%s: Cannot translate IRQ index %d\n",
			__func__, irq_index);
		return 0;
	}

	cascade_data = kzalloc(sizeof(struct fsl_msi_cascade_data), GFP_KERNEL);
	if (!cascade_data) {
		dev_err(&dev->dev, "No memory for MSI cascade data\n");
		return -ENOMEM;
	}
<<<<<<< HEAD

	msi->msi_virqs[irq_index] = virt_msir;
<<<<<<< HEAD
	cascade_data->index = offset + irq_index;
=======
	cascade_data->index = offset;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	irq_set_lockdep_class(virt_msir, &fsl_msi_irq_class);
	msi->msi_virqs[irq_index] = virt_msir;
	cascade_data->index = offset;
>>>>>>> refs/remotes/origin/master
	cascade_data->msi_data = msi;
	irq_set_handler_data(virt_msir, cascade_data);
	irq_set_chained_handler(virt_msir, fsl_msi_cascade);

<<<<<<< HEAD
=======
	/* Release the hwirqs corresponding to this MSI register */
	for (i = 0; i < IRQS_PER_MSI_REG; i++)
		msi_bitmap_free_hwirqs(&msi->bitmap,
				       msi_hwirq(msi, offset, i), 1);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static const struct of_device_id fsl_of_msi_ids[];
<<<<<<< HEAD
static int __devinit fsl_of_msi_probe(struct platform_device *dev)
{
	const struct of_device_id *match;
	struct fsl_msi *msi;
	struct resource res;
	int err, i, j, irq_index, count;
	int rc;
	const u32 *p;
	struct fsl_msi_feature *features;
	int len;
	u32 offset;
	static const u32 all_avail[] = { 0, NR_MSI_IRQS };
=======
static int fsl_of_msi_probe(struct platform_device *dev)
{
	const struct of_device_id *match;
	struct fsl_msi *msi;
	struct resource res, msiir;
	int err, i, j, irq_index, count;
	const u32 *p;
	const struct fsl_msi_feature *features;
	int len;
	u32 offset;
>>>>>>> refs/remotes/origin/master

	match = of_match_device(fsl_of_msi_ids, &dev->dev);
	if (!match)
		return -EINVAL;
	features = match->data;

	printk(KERN_DEBUG "Setting up Freescale MSI support\n");

	msi = kzalloc(sizeof(struct fsl_msi), GFP_KERNEL);
	if (!msi) {
		dev_err(&dev->dev, "No memory for MSI structure\n");
		return -ENOMEM;
	}
	platform_set_drvdata(dev, msi);

<<<<<<< HEAD
<<<<<<< HEAD
	msi->irqhost = irq_alloc_host(dev->dev.of_node, IRQ_HOST_MAP_LINEAR,
				      NR_MSI_IRQS, &fsl_msi_host_ops, 0);
=======
	msi->irqhost = irq_domain_add_linear(dev->dev.of_node,
				      NR_MSI_IRQS, &fsl_msi_host_ops, msi);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	msi->irqhost = irq_domain_add_linear(dev->dev.of_node,
				      NR_MSI_IRQS_MAX, &fsl_msi_host_ops, msi);
>>>>>>> refs/remotes/origin/master

	if (msi->irqhost == NULL) {
		dev_err(&dev->dev, "No memory for MSI irqhost\n");
		err = -ENOMEM;
		goto error_out;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/* Get the MSI reg base */
	err = of_address_to_resource(dev->dev.of_node, 0, &res);
	if (err) {
		dev_err(&dev->dev, "%s resource error!\n",
				dev->dev.of_node->full_name);
		goto error_out;
	}

	msi->msi_regs = ioremap(res.start, res.end - res.start + 1);
	if (!msi->msi_regs) {
		dev_err(&dev->dev, "ioremap problem failed\n");
		goto error_out;
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Under the Freescale hypervisor, the msi nodes don't have a 'reg'
	 * property.  Instead, we use hypercalls to access the MSI.
	 */
	if ((features->fsl_pic_ip & FSL_PIC_IP_MASK) != FSL_PIC_IP_VMPIC) {
		err = of_address_to_resource(dev->dev.of_node, 0, &res);
		if (err) {
			dev_err(&dev->dev, "invalid resource for node %s\n",
				dev->dev.of_node->full_name);
			goto error_out;
		}

		msi->msi_regs = ioremap(res.start, resource_size(&res));
		if (!msi->msi_regs) {
			err = -ENOMEM;
			dev_err(&dev->dev, "could not map node %s\n",
				dev->dev.of_node->full_name);
			goto error_out;
		}
		msi->msiir_offset =
			features->msiir_offset + (res.start & 0xfffff);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

		/*
		 * First read the MSIIR/MSIIR1 offset from dts
		 * On failure use the hardcode MSIIR offset
		 */
		if (of_address_to_resource(dev->dev.of_node, 1, &msiir))
			msi->msiir_offset = features->msiir_offset +
					    (res.start & MSIIR_OFFSET_MASK);
		else
			msi->msiir_offset = msiir.start & MSIIR_OFFSET_MASK;
>>>>>>> refs/remotes/origin/master
	}

	msi->feature = features->fsl_pic_ip;

<<<<<<< HEAD
<<<<<<< HEAD
	msi->irqhost->host_data = msi;

	msi->msi_addr_hi = 0x0;
	msi->msi_addr_lo = features->msiir_offset + (res.start & 0xfffff);
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Remember the phandle, so that we can match with any PCI nodes
	 * that have an "fsl,msi" property.
	 */
	msi->phandle = dev->dev.of_node->phandle;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	rc = fsl_msi_init_allocator(msi);
	if (rc) {
=======

	err = fsl_msi_init_allocator(msi);
	if (err) {
>>>>>>> refs/remotes/origin/master
		dev_err(&dev->dev, "Error allocating MSI bitmap\n");
		goto error_out;
	}

	p = of_get_property(dev->dev.of_node, "msi-available-ranges", &len);
<<<<<<< HEAD
	if (p && len % (2 * sizeof(u32)) != 0) {
		dev_err(&dev->dev, "%s: Malformed msi-available-ranges property\n",
			__func__);
		err = -EINVAL;
		goto error_out;
	}

<<<<<<< HEAD
	if (!p)
		p = all_avail;
=======
	if (!p) {
		p = all_avail;
		len = sizeof(all_avail);
	}
>>>>>>> refs/remotes/origin/cm-10.0

	for (irq_index = 0, i = 0; i < len / (2 * sizeof(u32)); i++) {
		if (p[i * 2] % IRQS_PER_MSI_REG ||
		    p[i * 2 + 1] % IRQS_PER_MSI_REG) {
			printk(KERN_WARNING "%s: %s: msi available range of %u at %u is not IRQ-aligned\n",
			       __func__, dev->dev.of_node->full_name,
			       p[i * 2 + 1], p[i * 2]);
=======

	if (of_device_is_compatible(dev->dev.of_node, "fsl,mpic-msi-v4.3")) {
		msi->srs_shift = MSIIR1_SRS_SHIFT;
		msi->ibs_shift = MSIIR1_IBS_SHIFT;
		if (p)
			dev_warn(&dev->dev, "%s: dose not support msi-available-ranges property\n",
				__func__);

		for (irq_index = 0; irq_index < NR_MSI_REG_MSIIR1;
		     irq_index++) {
			err = fsl_msi_setup_hwirq(msi, dev,
						  irq_index, irq_index);
			if (err)
				goto error_out;
		}
	} else {
		static const u32 all_avail[] =
			{ 0, NR_MSI_REG_MSIIR * IRQS_PER_MSI_REG };

		msi->srs_shift = MSIIR_SRS_SHIFT;
		msi->ibs_shift = MSIIR_IBS_SHIFT;

		if (p && len % (2 * sizeof(u32)) != 0) {
			dev_err(&dev->dev, "%s: Malformed msi-available-ranges property\n",
				__func__);
>>>>>>> refs/remotes/origin/master
			err = -EINVAL;
			goto error_out;
		}

<<<<<<< HEAD
		offset = p[i * 2] / IRQS_PER_MSI_REG;
		count = p[i * 2 + 1] / IRQS_PER_MSI_REG;

		for (j = 0; j < count; j++, irq_index++) {
<<<<<<< HEAD
			err = fsl_msi_setup_hwirq(msi, dev, offset, irq_index);
=======
			err = fsl_msi_setup_hwirq(msi, dev, offset + j, irq_index);
>>>>>>> refs/remotes/origin/cm-10.0
			if (err)
				goto error_out;
=======
		if (!p) {
			p = all_avail;
			len = sizeof(all_avail);
		}

		for (irq_index = 0, i = 0; i < len / (2 * sizeof(u32)); i++) {
			if (p[i * 2] % IRQS_PER_MSI_REG ||
			    p[i * 2 + 1] % IRQS_PER_MSI_REG) {
				pr_warn("%s: %s: msi available range of %u at %u is not IRQ-aligned\n",
				       __func__, dev->dev.of_node->full_name,
				       p[i * 2 + 1], p[i * 2]);
				err = -EINVAL;
				goto error_out;
			}

			offset = p[i * 2] / IRQS_PER_MSI_REG;
			count = p[i * 2 + 1] / IRQS_PER_MSI_REG;

			for (j = 0; j < count; j++, irq_index++) {
				err = fsl_msi_setup_hwirq(msi, dev, offset + j,
							  irq_index);
				if (err)
					goto error_out;
			}
>>>>>>> refs/remotes/origin/master
		}
	}

	list_add_tail(&msi->list, &msi_head);

	/* The multiple setting ppc_md.setup_msi_irqs will not harm things */
	if (!ppc_md.setup_msi_irqs) {
		ppc_md.setup_msi_irqs = fsl_setup_msi_irqs;
		ppc_md.teardown_msi_irqs = fsl_teardown_msi_irqs;
		ppc_md.msi_check_device = fsl_msi_check_device;
	} else if (ppc_md.setup_msi_irqs != fsl_setup_msi_irqs) {
		dev_err(&dev->dev, "Different MSI driver already installed!\n");
		err = -ENODEV;
		goto error_out;
	}
	return 0;
error_out:
	fsl_of_msi_remove(dev);
	return err;
}

static const struct fsl_msi_feature mpic_msi_feature = {
	.fsl_pic_ip = FSL_PIC_IP_MPIC,
	.msiir_offset = 0x140,
};

static const struct fsl_msi_feature ipic_msi_feature = {
	.fsl_pic_ip = FSL_PIC_IP_IPIC,
	.msiir_offset = 0x38,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static const struct fsl_msi_feature vmpic_msi_feature = {
	.fsl_pic_ip = FSL_PIC_IP_VMPIC,
	.msiir_offset = 0,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static const struct of_device_id fsl_of_msi_ids[] = {
	{
		.compatible = "fsl,mpic-msi",
		.data = (void *)&mpic_msi_feature,
	},
	{
		.compatible = "fsl,ipic-msi",
		.data = (void *)&ipic_msi_feature,
	},
<<<<<<< HEAD
=======
	{
		.compatible = "fsl,vmpic-msi",
		.data = (void *)&vmpic_msi_feature,
	},
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct of_device_id fsl_of_msi_ids[] = {
	{
		.compatible = "fsl,mpic-msi",
		.data = &mpic_msi_feature,
	},
	{
		.compatible = "fsl,mpic-msi-v4.3",
		.data = &mpic_msi_feature,
	},
	{
		.compatible = "fsl,ipic-msi",
		.data = &ipic_msi_feature,
	},
#ifdef CONFIG_EPAPR_PARAVIRT
	{
		.compatible = "fsl,vmpic-msi",
		.data = &vmpic_msi_feature,
	},
#endif
>>>>>>> refs/remotes/origin/master
	{}
};

static struct platform_driver fsl_of_msi_driver = {
	.driver = {
		.name = "fsl-msi",
		.owner = THIS_MODULE,
		.of_match_table = fsl_of_msi_ids,
	},
	.probe = fsl_of_msi_probe,
	.remove = fsl_of_msi_remove,
};

static __init int fsl_of_msi_init(void)
{
	return platform_driver_register(&fsl_of_msi_driver);
}

subsys_initcall(fsl_of_msi_init);
