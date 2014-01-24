#ifndef _DYNAMIC_DEBUG_H
#define _DYNAMIC_DEBUG_H

<<<<<<< HEAD
<<<<<<< HEAD
/* dynamic_printk_enabled, and dynamic_printk_enabled2 are bitmasks in which
 * bit n is set to 1 if any modname hashes into the bucket n, 0 otherwise. They
 * use independent hash functions, to reduce the chance of false positives.
 */
extern long long dynamic_debug_enabled;
extern long long dynamic_debug_enabled2;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * An instance of this structure is created in a special
 * ELF section at every dynamic debug callsite.  At runtime,
 * the special section is treated as an array of these.
 */
struct _ddebug {
	/*
	 * These fields are used to drive the user interface
	 * for selecting and displaying debug callsites.
	 */
	const char *modname;
	const char *function;
	const char *filename;
	const char *format;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int lineno:24;
=======
	unsigned int lineno:18;
>>>>>>> refs/remotes/origin/cm-10.0
	/*
 	 * The flags field controls the behaviour at the callsite.
 	 * The bits here are changed dynamically when the user
	 * writes commands to <debugfs>/dynamic_debug/control
	 */
<<<<<<< HEAD
#define _DPRINTK_FLAGS_PRINT   (1<<0)  /* printk() a message using the format */
=======
#define _DPRINTK_FLAGS_NONE	0
#define _DPRINTK_FLAGS_PRINT	(1<<0) /* printk() a message using the format */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned int lineno:18;
	/*
	 * The flags field controls the behaviour at the callsite.
	 * The bits here are changed dynamically when the user
	 * writes commands to <debugfs>/dynamic_debug/control
	 */
#define _DPRINTK_FLAGS_NONE	0
#define _DPRINTK_FLAGS_PRINT	(1<<0) /* printk() a message using the format */
>>>>>>> refs/remotes/origin/master
#define _DPRINTK_FLAGS_INCL_MODNAME	(1<<1)
#define _DPRINTK_FLAGS_INCL_FUNCNAME	(1<<2)
#define _DPRINTK_FLAGS_INCL_LINENO	(1<<3)
#define _DPRINTK_FLAGS_INCL_TID		(1<<4)
<<<<<<< HEAD
<<<<<<< HEAD
#define _DPRINTK_FLAGS_DEFAULT 0
	unsigned int flags:8;
	char enabled;
=======
=======
>>>>>>> refs/remotes/origin/master
#if defined DEBUG
#define _DPRINTK_FLAGS_DEFAULT _DPRINTK_FLAGS_PRINT
#else
#define _DPRINTK_FLAGS_DEFAULT 0
#endif
	unsigned int flags:8;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
} __attribute__((aligned(8)));


int ddebug_add_module(struct _ddebug *tab, unsigned int n,
				const char *modname);

#if defined(CONFIG_DYNAMIC_DEBUG)
extern int ddebug_remove_module(const char *mod_name);
<<<<<<< HEAD
<<<<<<< HEAD
extern int __dynamic_pr_debug(struct _ddebug *descriptor, const char *fmt, ...)
	__attribute__ ((format (printf, 2, 3)));

#define dynamic_pr_debug(fmt, ...) do {					\
	static struct _ddebug descriptor				\
	__used								\
	__attribute__((section("__verbose"), aligned(8))) =		\
	{ KBUILD_MODNAME, __func__, __FILE__, fmt, __LINE__,		\
		_DPRINTK_FLAGS_DEFAULT };				\
	if (unlikely(descriptor.enabled))				\
		__dynamic_pr_debug(&descriptor, pr_fmt(fmt), ##__VA_ARGS__); \
	} while (0)


#define dynamic_dev_dbg(dev, fmt, ...) do {				\
	static struct _ddebug descriptor				\
	__used								\
	__attribute__((section("__verbose"), aligned(8))) =		\
	{ KBUILD_MODNAME, __func__, __FILE__, fmt, __LINE__,		\
		_DPRINTK_FLAGS_DEFAULT };				\
	if (unlikely(descriptor.enabled))				\
		dev_printk(KERN_DEBUG, dev, fmt, ##__VA_ARGS__);	\
	} while (0)
=======
extern __printf(2, 3)
int __dynamic_pr_debug(struct _ddebug *descriptor, const char *fmt, ...);

=======
extern __printf(2, 3)
int __dynamic_pr_debug(struct _ddebug *descriptor, const char *fmt, ...);

extern int ddebug_dyndbg_module_param_cb(char *param, char *val,
					const char *modname);

>>>>>>> refs/remotes/origin/master
struct device;

extern __printf(3, 4)
int __dynamic_dev_dbg(struct _ddebug *descriptor, const struct device *dev,
		      const char *fmt, ...);

struct net_device;

extern __printf(3, 4)
int __dynamic_netdev_dbg(struct _ddebug *descriptor,
			 const struct net_device *dev,
			 const char *fmt, ...);

#define DEFINE_DYNAMIC_DEBUG_METADATA(name, fmt)		\
<<<<<<< HEAD
	static struct _ddebug __used __aligned(8)		\
=======
	static struct _ddebug  __aligned(8)			\
>>>>>>> refs/remotes/origin/master
	__attribute__((section("__verbose"))) name = {		\
		.modname = KBUILD_MODNAME,			\
		.function = __func__,				\
		.filename = __FILE__,				\
		.format = (fmt),				\
		.lineno = __LINE__,				\
		.flags =  _DPRINTK_FLAGS_DEFAULT,		\
	}

#define dynamic_pr_debug(fmt, ...)				\
do {								\
	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, fmt);		\
	if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT))	\
		__dynamic_pr_debug(&descriptor, pr_fmt(fmt),	\
				   ##__VA_ARGS__);		\
} while (0)

#define dynamic_dev_dbg(dev, fmt, ...)				\
do {								\
	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, fmt);		\
	if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT))	\
		__dynamic_dev_dbg(&descriptor, dev, fmt,	\
				  ##__VA_ARGS__);		\
} while (0)

#define dynamic_netdev_dbg(dev, fmt, ...)			\
do {								\
	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, fmt);		\
	if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT))	\
		__dynamic_netdev_dbg(&descriptor, dev, fmt,	\
				     ##__VA_ARGS__);		\
} while (0)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

#else

=======

#define dynamic_hex_dump(prefix_str, prefix_type, rowsize,	\
			 groupsize, buf, len, ascii)		\
do {								\
	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor,		\
		__builtin_constant_p(prefix_str) ? prefix_str : "hexdump");\
	if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT))	\
		print_hex_dump(KERN_DEBUG, prefix_str,		\
			       prefix_type, rowsize, groupsize,	\
			       buf, len, ascii);		\
} while (0)

#else

#include <linux/string.h>
#include <linux/errno.h>

>>>>>>> refs/remotes/origin/master
static inline int ddebug_remove_module(const char *mod)
{
	return 0;
}

<<<<<<< HEAD
=======
static inline int ddebug_dyndbg_module_param_cb(char *param, char *val,
						const char *modname)
{
	if (strstr(param, "dyndbg")) {
		/* avoid pr_warn(), which wants pr_fmt() fully defined */
		printk(KERN_WARNING "dyndbg param is supported only in "
			"CONFIG_DYNAMIC_DEBUG builds\n");
		return 0; /* allow and ignore */
	}
	return -EINVAL;
}

>>>>>>> refs/remotes/origin/master
#define dynamic_pr_debug(fmt, ...)					\
	do { if (0) printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__); } while (0)
#define dynamic_dev_dbg(dev, fmt, ...)					\
	do { if (0) dev_printk(KERN_DEBUG, dev, fmt, ##__VA_ARGS__); } while (0)
#endif

#endif
