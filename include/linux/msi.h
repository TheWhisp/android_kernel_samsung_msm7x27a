#ifndef LINUX_MSI_H
#define LINUX_MSI_H

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/kobject.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/kobject.h>
>>>>>>> refs/remotes/origin/master
#include <linux/list.h>

struct msi_msg {
	u32	address_lo;	/* low 32 bits of msi message address */
	u32	address_hi;	/* high 32 bits of msi message address */
	u32	data;		/* 16 bits of msi message data */
};

/* Helper functions */
struct irq_data;
struct msi_desc;
<<<<<<< HEAD
extern void mask_msi_irq(struct irq_data *data);
extern void unmask_msi_irq(struct irq_data *data);
extern void __read_msi_msg(struct msi_desc *entry, struct msi_msg *msg);
extern void __get_cached_msi_msg(struct msi_desc *entry, struct msi_msg *msg);
extern void __write_msi_msg(struct msi_desc *entry, struct msi_msg *msg);
extern void read_msi_msg(unsigned int irq, struct msi_msg *msg);
extern void get_cached_msi_msg(unsigned int irq, struct msi_msg *msg);
extern void write_msi_msg(unsigned int irq, struct msi_msg *msg);
=======
void mask_msi_irq(struct irq_data *data);
void unmask_msi_irq(struct irq_data *data);
void __read_msi_msg(struct msi_desc *entry, struct msi_msg *msg);
void __get_cached_msi_msg(struct msi_desc *entry, struct msi_msg *msg);
void __write_msi_msg(struct msi_desc *entry, struct msi_msg *msg);
void read_msi_msg(unsigned int irq, struct msi_msg *msg);
void get_cached_msi_msg(unsigned int irq, struct msi_msg *msg);
void write_msi_msg(unsigned int irq, struct msi_msg *msg);
>>>>>>> refs/remotes/origin/master

struct msi_desc {
	struct {
		__u8	is_msix	: 1;
		__u8	multiple: 3;	/* log2 number of messages */
<<<<<<< HEAD
		__u8	maskbit	: 1; 	/* mask-pending bit supported ?   */
		__u8	is_64	: 1;	/* Address size: 0=32bit 1=64bit  */
		__u8	pos;	 	/* Location of the msi capability */
		__u16	entry_nr;    	/* specific enabled entry 	  */
		unsigned default_irq;	/* default pre-assigned irq	  */
=======
		__u8	maskbit	: 1;	/* mask-pending bit supported ? */
		__u8	is_64	: 1;	/* Address size: 0=32bit 1=64bit */
		__u8	pos;		/* Location of the msi capability */
		__u16	entry_nr;	/* specific enabled entry */
		unsigned default_irq;	/* default pre-assigned irq */
>>>>>>> refs/remotes/origin/master
	} msi_attrib;

	u32 masked;			/* mask bits */
	unsigned int irq;
<<<<<<< HEAD
=======
	unsigned int nvec_used;		/* number of messages */
>>>>>>> refs/remotes/origin/master
	struct list_head list;

	union {
		void __iomem *mask_base;
		u8 mask_pos;
	};
	struct pci_dev *dev;

	/* Last set MSI message */
	struct msi_msg msg;
<<<<<<< HEAD
<<<<<<< HEAD
=======

	struct kobject kobj;
>>>>>>> refs/remotes/origin/cm-10.0
};

/*
 * The arch hook for setup up msi irqs
 */
int arch_setup_msi_irq(struct pci_dev *dev, struct msi_desc *desc);
void arch_teardown_msi_irq(unsigned int irq);
extern int arch_setup_msi_irqs(struct pci_dev *dev, int nvec, int type);
extern void arch_teardown_msi_irqs(struct pci_dev *dev);
extern int arch_msi_check_device(struct pci_dev* dev, int nvec, int type);

=======

	struct kobject kobj;
};

/*
 * The arch hooks to setup up msi irqs. Those functions are
 * implemented as weak symbols so that they /can/ be overriden by
 * architecture specific code if needed.
 */
int arch_setup_msi_irq(struct pci_dev *dev, struct msi_desc *desc);
void arch_teardown_msi_irq(unsigned int irq);
int arch_setup_msi_irqs(struct pci_dev *dev, int nvec, int type);
void arch_teardown_msi_irqs(struct pci_dev *dev);
int arch_msi_check_device(struct pci_dev* dev, int nvec, int type);
void arch_restore_msi_irqs(struct pci_dev *dev, int irq);

void default_teardown_msi_irqs(struct pci_dev *dev);
void default_restore_msi_irqs(struct pci_dev *dev, int irq);
u32 default_msi_mask_irq(struct msi_desc *desc, u32 mask, u32 flag);
u32 default_msix_mask_irq(struct msi_desc *desc, u32 flag);

struct msi_chip {
	struct module *owner;
	struct device *dev;
	struct device_node *of_node;
	struct list_head list;

	int (*setup_irq)(struct msi_chip *chip, struct pci_dev *dev,
			 struct msi_desc *desc);
	void (*teardown_irq)(struct msi_chip *chip, unsigned int irq);
	int (*check_device)(struct msi_chip *chip, struct pci_dev *dev,
			    int nvec, int type);
};
>>>>>>> refs/remotes/origin/master

#endif /* LINUX_MSI_H */
