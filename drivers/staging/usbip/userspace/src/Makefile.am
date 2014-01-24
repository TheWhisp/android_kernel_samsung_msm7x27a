<<<<<<< HEAD
<<<<<<< HEAD
AM_CPPFLAGS := -I$(top_srcdir)/libsrc -DUSBIDS_FILE='"@USBIDS_DIR@/usb.ids"'
AM_CFLAGS   := @EXTRA_CFLAGS@ @PACKAGE_CFLAGS@
LDADD       := $(top_srcdir)/libsrc/libusbip.la @PACKAGE_LIBS@

sbin_PROGRAMS := usbip usbipd usbip_bind_driver

usbip_SOURCES := usbip.c usbip_network.c usbip_network.h
usbipd_SOURCES := usbipd.c usbip_network.c usbip_network.h
usbip_bind_driver_SOURCES := bind-driver.c utils.c utils.h \
			     usbip_network.h usbip_network.c
=======
AM_CPPFLAGS = -I$(top_srcdir)/libsrc -DUSBIDS_FILE='"@USBIDS_DIR@/usb.ids"'
AM_CFLAGS   = @EXTRA_CFLAGS@ @PACKAGE_CFLAGS@
LDADD       = $(top_builddir)/libsrc/libusbip.la @PACKAGE_LIBS@

sbin_PROGRAMS := usbip usbipd

usbip_SOURCES := usbip.c utils.c usbip_network.c \
		 usbip_attach.c usbip_detach.c usbip_list.c \
		 usbip_bind.c usbip_unbind.c

usbipd_SOURCES := usbipd.c usbip_network.c
>>>>>>> refs/remotes/origin/cm-10.0
=======
AM_CPPFLAGS = -I$(top_srcdir)/libsrc -DUSBIDS_FILE='"@USBIDS_DIR@/usb.ids"'
AM_CFLAGS   = @EXTRA_CFLAGS@
LDADD       = $(top_builddir)/libsrc/libusbip.la

sbin_PROGRAMS := usbip usbipd

usbip_SOURCES := usbip.h utils.h usbip.c utils.c usbip_network.c \
		 usbip_attach.c usbip_detach.c usbip_list.c \
		 usbip_bind.c usbip_unbind.c usbip_port.c


usbipd_SOURCES := usbip_network.h usbipd.c usbip_network.c
>>>>>>> refs/remotes/origin/master
