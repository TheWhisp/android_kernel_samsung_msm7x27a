#ifndef _FCOE_LIBFCOE_H_
#define _FCOE_LIBFCOE_H_

extern unsigned int libfcoe_debug_logging;
<<<<<<< HEAD
#define LIBFCOE_LOGGING	    0x01 /* General logging, not categorized */
#define LIBFCOE_FIP_LOGGING 0x02 /* FIP logging */
#define LIBFCOE_TRANSPORT_LOGGING	0x04 /* FCoE transport logging */
=======
#define LIBFCOE_LOGGING	          0x01 /* General logging, not categorized */
#define LIBFCOE_FIP_LOGGING       0x02 /* FIP logging */
#define LIBFCOE_TRANSPORT_LOGGING 0x04 /* FCoE transport logging */
#define LIBFCOE_SYSFS_LOGGING     0x08 /* fcoe_sysfs logging */
>>>>>>> refs/remotes/origin/master

#define LIBFCOE_CHECK_LOGGING(LEVEL, CMD)		\
do {							\
	if (unlikely(libfcoe_debug_logging & LEVEL))	\
		do {					\
			CMD;				\
		} while (0);				\
} while (0)

#define LIBFCOE_DBG(fmt, args...)					\
	LIBFCOE_CHECK_LOGGING(LIBFCOE_LOGGING,				\
<<<<<<< HEAD
			      printk(KERN_INFO "libfcoe: " fmt, ##args);)

#define LIBFCOE_FIP_DBG(fip, fmt, args...)				\
	LIBFCOE_CHECK_LOGGING(LIBFCOE_FIP_LOGGING,			\
			      printk(KERN_INFO "host%d: fip: " fmt,	\
				     (fip)->lp->host->host_no, ##args);)

#define LIBFCOE_TRANSPORT_DBG(fmt, args...)				\
	LIBFCOE_CHECK_LOGGING(LIBFCOE_TRANSPORT_LOGGING,		\
			      printk(KERN_INFO "%s: " fmt,		\
				     __func__, ##args);)
=======
			      pr_info("libfcoe: " fmt, ##args);)

#define LIBFCOE_FIP_DBG(fip, fmt, args...)				\
	LIBFCOE_CHECK_LOGGING(LIBFCOE_FIP_LOGGING,			\
			      pr_info("host%d: fip: " fmt,		\
				      (fip)->lp->host->host_no, ##args);)

#define LIBFCOE_TRANSPORT_DBG(fmt, args...)				\
	LIBFCOE_CHECK_LOGGING(LIBFCOE_TRANSPORT_LOGGING,		\
			      pr_info("%s: " fmt, __func__, ##args);)

#define LIBFCOE_SYSFS_DBG(cdev, fmt, args...)				\
	LIBFCOE_CHECK_LOGGING(LIBFCOE_SYSFS_LOGGING,			\
			      pr_info("ctlr_%d: " fmt, cdev->id, ##args);)
>>>>>>> refs/remotes/origin/master

#endif /* _FCOE_LIBFCOE_H_ */
