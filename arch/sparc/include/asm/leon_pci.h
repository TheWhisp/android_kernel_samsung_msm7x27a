/*
 * asm/leon_pci.h
 *
 * Copyright (C) 2011 Aeroflex Gaisler AB, Daniel Hellstrom
 */

#ifndef _ASM_LEON_PCI_H_
#define _ASM_LEON_PCI_H_

/* PCI related definitions */
struct leon_pci_info {
	struct pci_ops *ops;
	struct resource	io_space;
	struct resource	mem_space;
<<<<<<< HEAD
<<<<<<< HEAD
	int (*map_irq)(struct pci_dev *dev, u8 slot, u8 pin);
=======
	int (*map_irq)(const struct pci_dev *dev, u8 slot, u8 pin);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct resource	busn;
	int (*map_irq)(const struct pci_dev *dev, u8 slot, u8 pin);
>>>>>>> refs/remotes/origin/master
};

extern void leon_pci_init(struct platform_device *ofdev,
				struct leon_pci_info *info);

#endif /* _ASM_LEON_PCI_H_ */
