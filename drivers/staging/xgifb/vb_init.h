#ifndef _VBINIT_
#define _VBINIT_
<<<<<<< HEAD
<<<<<<< HEAD
extern unsigned char XGIInitNew(struct xgi_hw_device_info *HwDeviceExtension);
=======
extern unsigned char XGIInitNew(struct pci_dev *pdev);
>>>>>>> refs/remotes/origin/cm-10.0
extern struct XGI21_LVDSCapStruct  XGI21_LCDCapList[13];
=======
extern unsigned char XGIInitNew(struct pci_dev *pdev);
extern void XGIRegInit(struct vb_device_info *, unsigned long);
>>>>>>> refs/remotes/origin/master
#endif

