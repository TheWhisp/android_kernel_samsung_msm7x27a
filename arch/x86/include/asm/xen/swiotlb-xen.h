#ifndef _ASM_X86_SWIOTLB_XEN_H
#define _ASM_X86_SWIOTLB_XEN_H

#ifdef CONFIG_SWIOTLB_XEN
extern int xen_swiotlb;
extern int __init pci_xen_swiotlb_detect(void);
extern void __init pci_xen_swiotlb_init(void);
<<<<<<< HEAD
=======
extern int pci_xen_swiotlb_init_late(void);
>>>>>>> refs/remotes/origin/master
#else
#define xen_swiotlb (0)
static inline int __init pci_xen_swiotlb_detect(void) { return 0; }
static inline void __init pci_xen_swiotlb_init(void) { }
<<<<<<< HEAD
=======
static inline int pci_xen_swiotlb_init_late(void) { return -ENXIO; }
>>>>>>> refs/remotes/origin/master
#endif

#endif /* _ASM_X86_SWIOTLB_XEN_H */
