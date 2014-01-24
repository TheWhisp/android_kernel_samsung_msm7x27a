/*
 * Copyright (C) 2005-2007 Takahiro Hirofuchi
 */

<<<<<<< HEAD
<<<<<<< HEAD
#ifndef _USBIP_COMMON_H
#define _USBIP_COMMON_H

#include <unistd.h>
#include <stdint.h>
#include <syslog.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

#include <sysfs/libsysfs.h>
#include <netdb.h>
#include <sys/socket.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#ifndef __USBIP_COMMON_H
#define __USBIP_COMMON_H

#include <sysfs/libsysfs.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <syslog.h>
#include <unistd.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#ifndef USBIDS_FILE
#define USBIDS_FILE "/usr/share/hwdata/usb.ids"
#endif

#ifndef VHCI_STATE_PATH
#define VHCI_STATE_PATH "/var/run/vhci_hcd"
#endif

<<<<<<< HEAD
<<<<<<< HEAD
//#include <linux/usb_ch9.h>
=======
=======
>>>>>>> refs/remotes/origin/master
/* kernel module names */
#define USBIP_CORE_MOD_NAME	"usbip-core"
#define USBIP_HOST_DRV_NAME	"usbip-host"
#define USBIP_VHCI_DRV_NAME	"vhci_hcd"

extern int usbip_use_syslog;
extern int usbip_use_stderr;
extern int usbip_use_debug ;

#define PROGNAME "usbip"

#define pr_fmt(fmt)	"%s: %s: " fmt "\n", PROGNAME
#define dbg_fmt(fmt)	pr_fmt("%s:%d:[%s] " fmt), "debug",	\
		        __FILE__, __LINE__, __FUNCTION__

#define err(fmt, args...)						\
	do {								\
		if (usbip_use_syslog) {					\
			syslog(LOG_ERR, pr_fmt(fmt), "error", ##args);	\
		}							\
		if (usbip_use_stderr) {					\
			fprintf(stderr, pr_fmt(fmt), "error", ##args);	\
		}							\
	} while (0)

#define info(fmt, args...)						\
	do {								\
		if (usbip_use_syslog) {					\
			syslog(LOG_INFO, pr_fmt(fmt), "info", ##args);	\
		}							\
		if (usbip_use_stderr) {					\
			fprintf(stderr, pr_fmt(fmt), "info", ##args);	\
		}							\
	} while (0)

#define dbg(fmt, args...)						\
	do {								\
	if (usbip_use_debug) {						\
		if (usbip_use_syslog) {					\
			syslog(LOG_DEBUG, dbg_fmt(fmt), ##args);	\
		}							\
		if (usbip_use_stderr) {					\
			fprintf(stderr, dbg_fmt(fmt), ##args);		\
		}							\
	}								\
	} while (0)

#define BUG()						\
	do {						\
		err("sorry, it's a bug!");		\
		abort();				\
	} while (0)

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
enum usb_device_speed {
	USB_SPEED_UNKNOWN = 0,                  /* enumerating */
	USB_SPEED_LOW, USB_SPEED_FULL,          /* usb 1.1 */
	USB_SPEED_HIGH,                         /* usb 2.0 */
	USB_SPEED_VARIABLE                      /* wireless (usb 2.5) */
};

/* FIXME: how to sync with drivers/usbip_common.h ? */
<<<<<<< HEAD
enum usbip_device_status{
=======
enum usbip_device_status {
>>>>>>> refs/remotes/origin/master
	/* sdev is available. */
	SDEV_ST_AVAILABLE = 0x01,
	/* sdev is now used. */
	SDEV_ST_USED,
	/* sdev is unusable because of a fatal error. */
	SDEV_ST_ERROR,

	/* vdev does not connect a remote device. */
	VDEV_ST_NULL,
	/* vdev is used, but the USB address is not assigned yet */
	VDEV_ST_NOTASSIGNED,
	VDEV_ST_USED,
	VDEV_ST_ERROR
};

<<<<<<< HEAD
<<<<<<< HEAD
extern int usbip_use_syslog;
extern int usbip_use_stderr;
extern int usbip_use_debug ;

#define err(fmt, args...)	do { \
	if (usbip_use_syslog) { \
		syslog(LOG_ERR, "usbip err: %13s:%4d (%-12s) " fmt "\n", \
			__FILE__, __LINE__, __FUNCTION__,  ##args); \
	} \
	if (usbip_use_stderr) { \
		fprintf(stderr, "usbip err: %13s:%4d (%-12s) " fmt "\n", \
			__FILE__, __LINE__, __FUNCTION__,  ##args); \
	} \
} while (0)

#define notice(fmt, args...)	do { \
	if (usbip_use_syslog) { \
		syslog(LOG_DEBUG, "usbip: " fmt, ##args); \
	} \
	if (usbip_use_stderr) { \
		fprintf(stderr, "usbip: " fmt "\n",  ##args); \
	} \
} while (0)

#define info(fmt, args...)	do { \
	if (usbip_use_syslog) { \
		syslog(LOG_DEBUG, fmt, ##args); \
	} \
	if (usbip_use_stderr) { \
		fprintf(stderr, fmt "\n",  ##args); \
	} \
} while (0)

#define dbg(fmt, args...)	do { \
	if (usbip_use_debug) { \
		if (usbip_use_syslog) { \
			syslog(LOG_DEBUG, "usbip dbg: %13s:%4d (%-12s) " fmt, \
				__FILE__, __LINE__, __FUNCTION__,  ##args); \
		} \
		if (usbip_use_stderr) { \
			fprintf(stderr, "usbip dbg: %13s:%4d (%-12s) " fmt "\n", \
				__FILE__, __LINE__, __FUNCTION__,  ##args); \
		} \
	} \
} while (0)


#define BUG()	do { err("sorry, it's a bug"); abort(); } while (0)


struct usb_interface {
=======
struct usbip_usb_interface {
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct usbip_usb_interface {
>>>>>>> refs/remotes/origin/master
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t padding;	/* alignment */
} __attribute__((packed));

<<<<<<< HEAD
<<<<<<< HEAD


struct usb_device {
=======
struct usbip_usb_device {
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct usbip_usb_device {
>>>>>>> refs/remotes/origin/master
	char path[SYSFS_PATH_MAX];
	char busid[SYSFS_BUS_ID_SIZE];

	uint32_t busnum;
	uint32_t devnum;
	uint32_t speed;

	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;

	uint8_t bDeviceClass;
	uint8_t bDeviceSubClass;
	uint8_t bDeviceProtocol;
	uint8_t bConfigurationValue;
	uint8_t bNumConfigurations;
	uint8_t bNumInterfaces;
} __attribute__((packed));

#define to_string(s)	#s

<<<<<<< HEAD
<<<<<<< HEAD
void dump_usb_interface(struct usb_interface *);
void dump_usb_device(struct usb_device *);
int read_usb_device(struct sysfs_device *sdev, struct usb_device *udev);
int read_attr_value(struct sysfs_device *dev, const char *name, const char *format);
int read_usb_interface(struct usb_device *udev, int i, struct usb_interface *uinf);
=======
void dump_usb_interface(struct usbip_usb_interface *);
void dump_usb_device(struct usbip_usb_device *);
int read_usb_device(struct sysfs_device *sdev, struct usbip_usb_device *udev);
int read_attr_value(struct sysfs_device *dev, const char *name, const char *format);
int read_usb_interface(struct usbip_usb_device *udev, int i,
		       struct usbip_usb_interface *uinf);
>>>>>>> refs/remotes/origin/cm-10.0
=======
void dump_usb_interface(struct usbip_usb_interface *);
void dump_usb_device(struct usbip_usb_device *);
int read_usb_device(struct sysfs_device *sdev, struct usbip_usb_device *udev);
int read_attr_value(struct sysfs_device *dev, const char *name,
		    const char *format);
int read_usb_interface(struct usbip_usb_device *udev, int i,
		       struct usbip_usb_interface *uinf);
>>>>>>> refs/remotes/origin/master

const char *usbip_speed_string(int num);
const char *usbip_status_string(int32_t status);

int usbip_names_init(char *);
void usbip_names_free(void);
<<<<<<< HEAD
void usbip_names_get_product(char *buff, size_t size, uint16_t vendor, uint16_t product);
void usbip_names_get_class(char *buff, size_t size, uint8_t class, uint8_t subclass, uint8_t protocol);

<<<<<<< HEAD
#endif
=======
#endif /* __USBIP_COMMON_H */
>>>>>>> refs/remotes/origin/cm-10.0
=======
void usbip_names_get_product(char *buff, size_t size, uint16_t vendor,
			     uint16_t product);
void usbip_names_get_class(char *buff, size_t size, uint8_t class,
			   uint8_t subclass, uint8_t protocol);

#endif /* __USBIP_COMMON_H */
>>>>>>> refs/remotes/origin/master
