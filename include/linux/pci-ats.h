#ifndef LINUX_PCI_ATS_H
#define LINUX_PCI_ATS_H

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/pci.h>

>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/pci.h>

>>>>>>> refs/remotes/origin/master
/* Address Translation Service */
struct pci_ats {
	int pos;        /* capability position */
	int stu;        /* Smallest Translation Unit */
	int qdep;       /* Invalidate Queue Depth */
	int ref_cnt;    /* Physical Function reference count */
	unsigned int is_enabled:1;      /* Enable bit is set */
};

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_PCI_IOV
=======
#ifdef CONFIG_PCI_ATS
>>>>>>> refs/remotes/origin/cm-10.0

extern int pci_enable_ats(struct pci_dev *dev, int ps);
extern void pci_disable_ats(struct pci_dev *dev);
extern int pci_ats_queue_depth(struct pci_dev *dev);
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_PCI_ATS

int pci_enable_ats(struct pci_dev *dev, int ps);
void pci_disable_ats(struct pci_dev *dev);
int pci_ats_queue_depth(struct pci_dev *dev);

>>>>>>> refs/remotes/origin/master
/**
 * pci_ats_enabled - query the ATS status
 * @dev: the PCI device
 *
 * Returns 1 if ATS capability is enabled, or 0 if not.
 */
static inline int pci_ats_enabled(struct pci_dev *dev)
{
	return dev->ats && dev->ats->is_enabled;
}

<<<<<<< HEAD
<<<<<<< HEAD
#else /* CONFIG_PCI_IOV */
=======
#else /* CONFIG_PCI_ATS */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#else /* CONFIG_PCI_ATS */
>>>>>>> refs/remotes/origin/master

static inline int pci_enable_ats(struct pci_dev *dev, int ps)
{
	return -ENODEV;
}

static inline void pci_disable_ats(struct pci_dev *dev)
{
}

static inline int pci_ats_queue_depth(struct pci_dev *dev)
{
	return -ENODEV;
}

static inline int pci_ats_enabled(struct pci_dev *dev)
{
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
#endif /* CONFIG_PCI_IOV */
=======
=======
>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_PCI_ATS */

#ifdef CONFIG_PCI_PRI

<<<<<<< HEAD
extern int  pci_enable_pri(struct pci_dev *pdev, u32 reqs);
extern void pci_disable_pri(struct pci_dev *pdev);
extern bool pci_pri_enabled(struct pci_dev *pdev);
extern int  pci_reset_pri(struct pci_dev *pdev);
extern bool pci_pri_stopped(struct pci_dev *pdev);
extern int  pci_pri_status(struct pci_dev *pdev);
=======
int pci_enable_pri(struct pci_dev *pdev, u32 reqs);
void pci_disable_pri(struct pci_dev *pdev);
bool pci_pri_enabled(struct pci_dev *pdev);
int pci_reset_pri(struct pci_dev *pdev);
bool pci_pri_stopped(struct pci_dev *pdev);
int pci_pri_status(struct pci_dev *pdev);
>>>>>>> refs/remotes/origin/master

#else /* CONFIG_PCI_PRI */

static inline int pci_enable_pri(struct pci_dev *pdev, u32 reqs)
{
	return -ENODEV;
}

static inline void pci_disable_pri(struct pci_dev *pdev)
{
}

static inline bool pci_pri_enabled(struct pci_dev *pdev)
{
	return false;
}

static inline int pci_reset_pri(struct pci_dev *pdev)
{
	return -ENODEV;
}

static inline bool pci_pri_stopped(struct pci_dev *pdev)
{
	return true;
}

static inline int pci_pri_status(struct pci_dev *pdev)
{
	return -ENODEV;
}
#endif /* CONFIG_PCI_PRI */

#ifdef CONFIG_PCI_PASID

<<<<<<< HEAD
extern int pci_enable_pasid(struct pci_dev *pdev, int features);
extern void pci_disable_pasid(struct pci_dev *pdev);
extern int pci_pasid_features(struct pci_dev *pdev);
extern int pci_max_pasids(struct pci_dev *pdev);
=======
int pci_enable_pasid(struct pci_dev *pdev, int features);
void pci_disable_pasid(struct pci_dev *pdev);
int pci_pasid_features(struct pci_dev *pdev);
int pci_max_pasids(struct pci_dev *pdev);
>>>>>>> refs/remotes/origin/master

#else  /* CONFIG_PCI_PASID */

static inline int pci_enable_pasid(struct pci_dev *pdev, int features)
{
	return -EINVAL;
}

static inline void pci_disable_pasid(struct pci_dev *pdev)
{
}

static inline int pci_pasid_features(struct pci_dev *pdev)
{
	return -EINVAL;
}

static inline int pci_max_pasids(struct pci_dev *pdev)
{
	return -EINVAL;
}

#endif /* CONFIG_PCI_PASID */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#endif /* LINUX_PCI_ATS_H*/
