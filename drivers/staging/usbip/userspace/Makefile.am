SUBDIRS := libsrc src
<<<<<<< HEAD
includedir := @includedir@/usbip
include_HEADERS := $(addprefix libsrc/, \
		     usbip.h usbip_common.h vhci_driver.h stub_driver.h)

dist_man_MANS := $(addprefix doc/, usbip.8 usbipd.8 usbip_bind_driver.8)

if INSTALL_USBIDS
pkgdata_DATA := usb.ids
EXTRA_DIST := $(pkgdata_DATA)
endif
=======
includedir = @includedir@/usbip
include_HEADERS := $(addprefix libsrc/, \
		     usbip_common.h vhci_driver.h usbip_host_driver.h)

dist_man_MANS := $(addprefix doc/, usbip.8 usbipd.8 usbip_bind_driver.8)
>>>>>>> refs/remotes/origin/cm-10.0
