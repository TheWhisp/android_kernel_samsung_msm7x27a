#ifndef __MACH_ULPI_H
#define __MACH_ULPI_H

#ifdef CONFIG_USB_ULPI
<<<<<<< HEAD
struct otg_transceiver *imx_otg_ulpi_create(unsigned int flags);
#else
static inline struct otg_transceiver *imx_otg_ulpi_create(unsigned int flags)
=======
struct usb_phy *imx_otg_ulpi_create(unsigned int flags);
#else
static inline struct usb_phy *imx_otg_ulpi_create(unsigned int flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return NULL;
}
#endif

<<<<<<< HEAD
extern struct otg_io_access_ops mxc_ulpi_access_ops;
=======
extern struct usb_phy_io_ops mxc_ulpi_access_ops;
>>>>>>> refs/remotes/origin/cm-10.0

#endif /* __MACH_ULPI_H */

