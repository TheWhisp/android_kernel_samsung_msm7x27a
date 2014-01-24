#include <linux/init.h>
#include <linux/pci.h>
<<<<<<< HEAD
=======
#include <asm/mips-boards/piix4.h>
>>>>>>> refs/remotes/origin/master

/* PCI interrupt pins */
#define PCIA		1
#define PCIB		2
#define PCIC		3
#define PCID		4

/* This table is filled in by interrogating the PIIX4 chip */
<<<<<<< HEAD
static char pci_irq[5] __initdata;

static char irq_tab[][5] __initdata = {
	/*      INTA    INTB    INTC    INTD */
=======
static char pci_irq[5] = {
};

static char irq_tab[][5] __initdata = {
	/*	INTA	INTB	INTC	INTD */
>>>>>>> refs/remotes/origin/master
	{0,	0,	0,	0,	0 },	/*  0: GT64120 PCI bridge */
	{0,	0,	0,	0,	0 },	/*  1: Unused */
	{0,	0,	0,	0,	0 },	/*  2: Unused */
	{0,	0,	0,	0,	0 },	/*  3: Unused */
	{0,	0,	0,	0,	0 },	/*  4: Unused */
	{0,	0,	0,	0,	0 },	/*  5: Unused */
	{0,	0,	0,	0,	0 },	/*  6: Unused */
	{0,	0,	0,	0,	0 },	/*  7: Unused */
	{0,	0,	0,	0,	0 },	/*  8: Unused */
	{0,	0,	0,	0,	0 },	/*  9: Unused */
<<<<<<< HEAD
	{0,	0,	0,	0,	PCID },	/* 10: PIIX4 USB */
=======
	{0,	0,	0,	0,	PCID }, /* 10: PIIX4 USB */
>>>>>>> refs/remotes/origin/master
	{0,	PCIB,	0,	0,	0 },	/* 11: AMD 79C973 Ethernet */
	{0,	PCIC,	0,	0,	0 },	/* 12: Crystal 4281 Sound */
	{0,	0,	0,	0,	0 },	/* 13: Unused */
	{0,	0,	0,	0,	0 },	/* 14: Unused */
	{0,	0,	0,	0,	0 },	/* 15: Unused */
	{0,	0,	0,	0,	0 },	/* 16: Unused */
	{0,	0,	0,	0,	0 },	/* 17: Bonito/SOC-it PCI Bridge*/
<<<<<<< HEAD
	{0,	PCIA,	PCIB,	PCIC,	PCID },	/* 18: PCI Slot 1 */
	{0,	PCIB,	PCIC,	PCID,	PCIA },	/* 19: PCI Slot 2 */
	{0,	PCIC,	PCID,	PCIA,	PCIB },	/* 20: PCI Slot 3 */
=======
	{0,	PCIA,	PCIB,	PCIC,	PCID }, /* 18: PCI Slot 1 */
	{0,	PCIB,	PCIC,	PCID,	PCIA }, /* 19: PCI Slot 2 */
	{0,	PCIC,	PCID,	PCIA,	PCIB }, /* 20: PCI Slot 3 */
>>>>>>> refs/remotes/origin/master
	{0,	PCID,	PCIA,	PCIB,	PCIC }	/* 21: PCI Slot 4 */
};

int __init pcibios_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
{
	int virq;
	virq = irq_tab[slot][pin];
	return pci_irq[virq];
}

/* Do platform specific device initialization at pci_enable_device() time */
int pcibios_plat_dev_init(struct pci_dev *dev)
{
	return 0;
}

<<<<<<< HEAD
static void __init malta_piix_func0_fixup(struct pci_dev *pdev)
{
	unsigned char reg_val;
	static int piixirqmap[16] __initdata = {  /* PIIX PIRQC[A:D] irq mappings */
		0,  0, 	0,  3,
		4,  5,  6,  7,
=======
static void malta_piix_func0_fixup(struct pci_dev *pdev)
{
	unsigned char reg_val;
	/* PIIX PIRQC[A:D] irq mappings */
	static int piixirqmap[PIIX4_FUNC0_PIRQRC_IRQ_ROUTING_MAX] = {
		0,  0,	0,  3,
		4,  5,	6,  7,
>>>>>>> refs/remotes/origin/master
		0,  9, 10, 11,
		12, 0, 14, 15
	};
	int i;

	/* Interrogate PIIX4 to get PCI IRQ mapping */
	for (i = 0; i <= 3; i++) {
<<<<<<< HEAD
		pci_read_config_byte(pdev, 0x60+i, &reg_val);
		if (reg_val & 0x80)
			pci_irq[PCIA+i] = 0;	/* Disabled */
		else
			pci_irq[PCIA+i] = piixirqmap[reg_val & 15];
=======
		pci_read_config_byte(pdev, PIIX4_FUNC0_PIRQRC+i, &reg_val);
		if (reg_val & PIIX4_FUNC0_PIRQRC_IRQ_ROUTING_DISABLE)
			pci_irq[PCIA+i] = 0;	/* Disabled */
		else
			pci_irq[PCIA+i] = piixirqmap[reg_val &
				PIIX4_FUNC0_PIRQRC_IRQ_ROUTING_MASK];
>>>>>>> refs/remotes/origin/master
	}

	/* Done by YAMON 2.00 onwards */
	if (PCI_SLOT(pdev->devfn) == 10) {
		/*
		 * Set top of main memory accessible by ISA or DMA
		 * devices to 16 Mb.
		 */
<<<<<<< HEAD
		pci_read_config_byte(pdev, 0x69, &reg_val);
		pci_write_config_byte(pdev, 0x69, reg_val | 0xf0);
=======
		pci_read_config_byte(pdev, PIIX4_FUNC0_TOM, &reg_val);
		pci_write_config_byte(pdev, PIIX4_FUNC0_TOM, reg_val |
				PIIX4_FUNC0_TOM_TOP_OF_MEMORY_MASK);
>>>>>>> refs/remotes/origin/master
	}
}

DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82371AB_0,
	 malta_piix_func0_fixup);

<<<<<<< HEAD
static void __init malta_piix_func1_fixup(struct pci_dev *pdev)
=======
static void malta_piix_func1_fixup(struct pci_dev *pdev)
>>>>>>> refs/remotes/origin/master
{
	unsigned char reg_val;

	/* Done by YAMON 2.02 onwards */
	if (PCI_SLOT(pdev->devfn) == 10) {
		/*
		 * IDE Decode enable.
		 */
<<<<<<< HEAD
		pci_read_config_byte(pdev, 0x41, &reg_val);
		pci_write_config_byte(pdev, 0x41, reg_val|0x80);
		pci_read_config_byte(pdev, 0x43, &reg_val);
		pci_write_config_byte(pdev, 0x43, reg_val|0x80);
=======
		pci_read_config_byte(pdev, PIIX4_FUNC1_IDETIM_PRIMARY_HI,
			&reg_val);
		pci_write_config_byte(pdev, PIIX4_FUNC1_IDETIM_PRIMARY_HI,
			reg_val|PIIX4_FUNC1_IDETIM_PRIMARY_HI_IDE_DECODE_EN);
		pci_read_config_byte(pdev, PIIX4_FUNC1_IDETIM_SECONDARY_HI,
			&reg_val);
		pci_write_config_byte(pdev, PIIX4_FUNC1_IDETIM_SECONDARY_HI,
			reg_val|PIIX4_FUNC1_IDETIM_SECONDARY_HI_IDE_DECODE_EN);
>>>>>>> refs/remotes/origin/master
	}
}

DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82371AB,
	 malta_piix_func1_fixup);
<<<<<<< HEAD
=======

/* Enable PCI 2.1 compatibility in PIIX4 */
static void quirk_dlcsetup(struct pci_dev *dev)
{
	u8 odlc, ndlc;

	(void) pci_read_config_byte(dev, PIIX4_FUNC0_DLC, &odlc);
	/* Enable passive releases and delayed transaction */
	ndlc = odlc | PIIX4_FUNC0_DLC_USBPR_EN |
		      PIIX4_FUNC0_DLC_PASSIVE_RELEASE_EN |
		      PIIX4_FUNC0_DLC_DELAYED_TRANSACTION_EN;
	(void) pci_write_config_byte(dev, PIIX4_FUNC0_DLC, ndlc);
}

DECLARE_PCI_FIXUP_FINAL(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82371AB_0,
	quirk_dlcsetup);
>>>>>>> refs/remotes/origin/master
