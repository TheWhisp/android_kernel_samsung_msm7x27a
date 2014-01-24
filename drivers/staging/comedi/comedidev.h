/*
    include/linux/comedidev.h
    header file for kernel-only structures, variables, and constants

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 1997-2000 David A. Schleef <ds@schleef.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
<<<<<<< HEAD

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

=======
>>>>>>> refs/remotes/origin/master
*/

#ifndef _COMEDIDEV_H
#define _COMEDIDEV_H

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/vmalloc.h>
#include <linux/dma-mapping.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/timer.h>

#include "comedi.h"

#define DPRINTK(format, args...)	do {		\
	if (comedi_debug)				\
		printk(KERN_DEBUG "comedi: " format , ## args);	\
} while (0)

=======
#include <linux/dma-mapping.h>
#include <linux/mutex.h>
#include <linux/spinlock_types.h>
#include <linux/rwsem.h>
#include <linux/kref.h>

#include "comedi.h"

>>>>>>> refs/remotes/origin/master
#define COMEDI_VERSION(a, b, c) (((a) << 16) + ((b) << 8) + (c))
#define COMEDI_VERSION_CODE COMEDI_VERSION(COMEDI_MAJORVERSION, \
	COMEDI_MINORVERSION, COMEDI_MICROVERSION)
#define COMEDI_RELEASE VERSION

<<<<<<< HEAD
#define PCI_VENDOR_ID_ADLINK		0x144a
#define PCI_VENDOR_ID_ICP		0x104c
#define PCI_VENDOR_ID_CONTEC		0x1221

#define COMEDI_NUM_MINORS 0x100
#define COMEDI_NUM_BOARD_MINORS 0x30
#define COMEDI_FIRST_SUBDEVICE_MINOR COMEDI_NUM_BOARD_MINORS

<<<<<<< HEAD
#define COMEDI_DEVICE_CREATE(cs, parent, devt, drvdata, device, fmt...) \
	device_create(cs, ((parent) ? (parent) : (device)), devt, drvdata, fmt)

=======
>>>>>>> refs/remotes/origin/cm-10.0
struct comedi_subdevice {
	struct comedi_device *device;
=======
#define COMEDI_NUM_BOARD_MINORS 0x30

struct comedi_subdevice {
	struct comedi_device *device;
	int index;
>>>>>>> refs/remotes/origin/master
	int type;
	int n_chan;
	int subdev_flags;
	int len_chanlist;	/* maximum length of channel/gain list */

	void *private;

	struct comedi_async *async;

	void *lock;
	void *busy;
	unsigned runflags;
	spinlock_t spin_lock;

<<<<<<< HEAD
	int io_bits;
=======
	unsigned int io_bits;
>>>>>>> refs/remotes/origin/master

	unsigned int maxdata;	/* if maxdata==0, use list */
	const unsigned int *maxdata_list;	/* list is channel specific */

<<<<<<< HEAD
	unsigned int flags;
	const unsigned int *flaglist;

	unsigned int settling_time_0;

=======
>>>>>>> refs/remotes/origin/master
	const struct comedi_lrange *range_table;
	const struct comedi_lrange *const *range_table_list;

	unsigned int *chanlist;	/* driver-owned chanlist (not used) */

	int (*insn_read) (struct comedi_device *, struct comedi_subdevice *,
			  struct comedi_insn *, unsigned int *);
	int (*insn_write) (struct comedi_device *, struct comedi_subdevice *,
			   struct comedi_insn *, unsigned int *);
	int (*insn_bits) (struct comedi_device *, struct comedi_subdevice *,
			  struct comedi_insn *, unsigned int *);
	int (*insn_config) (struct comedi_device *, struct comedi_subdevice *,
			    struct comedi_insn *, unsigned int *);

	int (*do_cmd) (struct comedi_device *, struct comedi_subdevice *);
	int (*do_cmdtest) (struct comedi_device *, struct comedi_subdevice *,
			   struct comedi_cmd *);
	int (*poll) (struct comedi_device *, struct comedi_subdevice *);
	int (*cancel) (struct comedi_device *, struct comedi_subdevice *);
	/* int (*do_lock)(struct comedi_device *, struct comedi_subdevice *); */
	/* int (*do_unlock)(struct comedi_device *, \
			struct comedi_subdevice *); */

	/* called when the buffer changes */
	int (*buf_change) (struct comedi_device *dev,
			   struct comedi_subdevice *s, unsigned long new_size);

	void (*munge) (struct comedi_device *dev, struct comedi_subdevice *s,
		       void *data, unsigned int num_bytes,
		       unsigned int start_chan_index);
	enum dma_data_direction async_dma_dir;

	unsigned int state;

	struct device *class_dev;
	int minor;
};

struct comedi_buf_page {
	void *virt_addr;
	dma_addr_t dma_addr;
};

<<<<<<< HEAD
=======
struct comedi_buf_map {
	struct device *dma_hw_dev;
	struct comedi_buf_page *page_list;
	unsigned int n_pages;
	enum dma_data_direction dma_dir;
	struct kref refcount;
};

>>>>>>> refs/remotes/origin/master
struct comedi_async {
	struct comedi_subdevice *subdevice;

	void *prealloc_buf;	/* pre-allocated buffer */
	unsigned int prealloc_bufsz;	/* buffer size, in bytes */
<<<<<<< HEAD
	/* virtual and dma address of each page */
	struct comedi_buf_page *buf_page_list;
	unsigned n_buf_pages;	/* num elements in buf_page_list */

	unsigned int max_bufsize;	/* maximum buffer size, bytes */
	/* current number of mmaps of prealloc_buf */
	unsigned int mmap_count;
=======
	struct comedi_buf_map *buf_map;	/* map of buffer pages */

	unsigned int max_bufsize;	/* maximum buffer size, bytes */
>>>>>>> refs/remotes/origin/master

	/* byte count for writer (write completed) */
	unsigned int buf_write_count;
	/* byte count for writer (allocated for writing) */
	unsigned int buf_write_alloc_count;
	/* byte count for reader (read completed) */
	unsigned int buf_read_count;
	/* byte count for reader (allocated for reading) */
	unsigned int buf_read_alloc_count;

	unsigned int buf_write_ptr;	/* buffer marker for writer */
	unsigned int buf_read_ptr;	/* buffer marker for reader */

	unsigned int cur_chan;	/* useless channel marker for interrupt */
	/* number of bytes that have been received for current scan */
	unsigned int scan_progress;
	/* keeps track of where we are in chanlist as for munging */
	unsigned int munge_chan;
	/* number of bytes that have been munged */
	unsigned int munge_count;
	/* buffer marker for munging */
	unsigned int munge_ptr;

	unsigned int events;	/* events that have occurred */

	struct comedi_cmd cmd;

	wait_queue_head_t wait_head;

<<<<<<< HEAD
	/* callback stuff */
	unsigned int cb_mask;
	int (*cb_func) (unsigned int flags, void *);
	void *cb_arg;
=======
	unsigned int cb_mask;
>>>>>>> refs/remotes/origin/master

	int (*inttrig) (struct comedi_device *dev, struct comedi_subdevice *s,
			unsigned int x);
};

struct comedi_driver {
	struct comedi_driver *next;

	const char *driver_name;
	struct module *module;
	int (*attach) (struct comedi_device *, struct comedi_devconfig *);
<<<<<<< HEAD
	int (*detach) (struct comedi_device *);
=======
	void (*detach) (struct comedi_device *);
	int (*auto_attach) (struct comedi_device *, unsigned long);
>>>>>>> refs/remotes/origin/master

	/* number of elements in board_name and board_id arrays */
	unsigned int num_names;
	const char *const *board_name;
	/* offset in bytes from one board name pointer to the next */
	int offset;
};

struct comedi_device {
	int use_count;
	struct comedi_driver *driver;
	void *private;

	struct device *class_dev;
	int minor;
<<<<<<< HEAD
=======
	unsigned int detach_count;
>>>>>>> refs/remotes/origin/master
	/* hw_dev is passed to dma_alloc_coherent when allocating async buffers
	 * for subdevices that have async_dma_dir set to something other than
	 * DMA_NONE */
	struct device *hw_dev;

	const char *board_name;
	const void *board_ptr;
<<<<<<< HEAD
	int attached;
	spinlock_t spinlock;
	struct mutex mutex;
	int in_request_module;
=======
	bool attached:1;
	bool in_request_module:1;
	bool ioenabled:1;
	spinlock_t spinlock;
	struct mutex mutex;
	struct rw_semaphore attach_lock;
	struct kref refcount;
>>>>>>> refs/remotes/origin/master

	int n_subdevices;
	struct comedi_subdevice *subdevices;

	/* dumb */
	unsigned long iobase;
<<<<<<< HEAD
=======
	unsigned long iolen;
>>>>>>> refs/remotes/origin/master
	unsigned int irq;

	struct comedi_subdevice *read_subdev;
	struct comedi_subdevice *write_subdev;

	struct fasync_struct *async_queue;

	int (*open) (struct comedi_device *dev);
	void (*close) (struct comedi_device *dev);
};

<<<<<<< HEAD
struct comedi_device_file_info {
	struct comedi_device *device;
	struct comedi_subdevice *read_subdevice;
	struct comedi_subdevice *write_subdevice;
	struct device *hardware_device;
};

#ifdef CONFIG_COMEDI_DEBUG
extern int comedi_debug;
#else
static const int comedi_debug;
#endif
=======
static inline const void *comedi_board(const struct comedi_device *dev)
{
	return dev->board_ptr;
}
>>>>>>> refs/remotes/origin/master

/*
 * function prototypes
 */

void comedi_event(struct comedi_device *dev, struct comedi_subdevice *s);
void comedi_error(const struct comedi_device *dev, const char *s);

/* we can expand the number of bits used to encode devices/subdevices into
 the minor number soon, after more distros support > 8 bit minor numbers
 (like after Debian Etch gets released) */
enum comedi_minor_bits {
	COMEDI_DEVICE_MINOR_MASK = 0xf,
	COMEDI_SUBDEVICE_MINOR_MASK = 0xf0
};
static const unsigned COMEDI_SUBDEVICE_MINOR_SHIFT = 4;
static const unsigned COMEDI_SUBDEVICE_MINOR_OFFSET = 1;

<<<<<<< HEAD
struct comedi_device_file_info *comedi_get_device_file_info(unsigned minor);

static inline struct comedi_subdevice *comedi_get_read_subdevice(
	const struct comedi_device_file_info *info)
{
	if (info->read_subdevice)
		return info->read_subdevice;
	if (info->device == NULL)
		return NULL;
	return info->device->read_subdev;
}

static inline struct comedi_subdevice *comedi_get_write_subdevice(
	const struct comedi_device_file_info *info)
{
	if (info->write_subdevice)
		return info->write_subdevice;
	if (info->device == NULL)
		return NULL;
	return info->device->write_subdev;
}

void comedi_device_detach(struct comedi_device *dev);
int comedi_device_attach(struct comedi_device *dev,
			 struct comedi_devconfig *it);
int comedi_driver_register(struct comedi_driver *);
int comedi_driver_unregister(struct comedi_driver *);
=======
struct comedi_device *comedi_dev_get_from_minor(unsigned minor);
int comedi_dev_put(struct comedi_device *dev);
>>>>>>> refs/remotes/origin/master

void init_polling(void);
void cleanup_polling(void);
void start_polling(struct comedi_device *);
void stop_polling(struct comedi_device *);

<<<<<<< HEAD
#ifdef CONFIG_PROC_FS
void comedi_proc_init(void);
void comedi_proc_cleanup(void);
#else
static inline void comedi_proc_init(void)
{
}

static inline void comedi_proc_cleanup(void)
{
}
#endif

/* subdevice runflags */
enum subdevice_runflags {
	SRF_USER = 0x00000001,
=======
/* subdevice runflags */
enum subdevice_runflags {
>>>>>>> refs/remotes/origin/master
	SRF_RT = 0x00000002,
	/* indicates an COMEDI_CB_ERROR event has occurred since the last
	 * command was started */
	SRF_ERROR = 0x00000004,
<<<<<<< HEAD
	SRF_RUNNING = 0x08000000
};

int comedi_check_chanlist(struct comedi_subdevice *s,
			  int n,
			  unsigned int *chanlist);
unsigned comedi_get_subdevice_runflags(struct comedi_subdevice *s);
=======
	SRF_RUNNING = 0x08000000,
	SRF_FREE_SPRIV = 0x80000000,	/* free s->private on detach */
};

bool comedi_is_subdevice_running(struct comedi_subdevice *s);

void *comedi_alloc_spriv(struct comedi_subdevice *s, size_t size);

int comedi_check_chanlist(struct comedi_subdevice *s,
			  int n,
			  unsigned int *chanlist);
>>>>>>> refs/remotes/origin/master

/* range stuff */

#define RANGE(a, b)		{(a)*1e6, (b)*1e6, 0}
#define RANGE_ext(a, b)		{(a)*1e6, (b)*1e6, RF_EXTERNAL}
#define RANGE_mA(a, b)		{(a)*1e6, (b)*1e6, UNIT_mA}
#define RANGE_unitless(a, b)	{(a)*1e6, (b)*1e6, 0}
#define BIP_RANGE(a)		{-(a)*1e6, (a)*1e6, 0}
#define UNI_RANGE(a)		{0, (a)*1e6, 0}

extern const struct comedi_lrange range_bipolar10;
extern const struct comedi_lrange range_bipolar5;
extern const struct comedi_lrange range_bipolar2_5;
extern const struct comedi_lrange range_unipolar10;
extern const struct comedi_lrange range_unipolar5;
<<<<<<< HEAD
=======
extern const struct comedi_lrange range_unipolar2_5;
extern const struct comedi_lrange range_0_20mA;
extern const struct comedi_lrange range_4_20mA;
extern const struct comedi_lrange range_0_32mA;
>>>>>>> refs/remotes/origin/master
extern const struct comedi_lrange range_unknown;

#define range_digital		range_unipolar5

#if __GNUC__ >= 3
#define GCC_ZERO_LENGTH_ARRAY
#else
#define GCC_ZERO_LENGTH_ARRAY 0
#endif

struct comedi_lrange {
	int length;
	struct comedi_krange range[GCC_ZERO_LENGTH_ARRAY];
};

<<<<<<< HEAD
/* some silly little inline functions */

static inline int alloc_subdevices(struct comedi_device *dev,
				   unsigned int num_subdevices)
{
	unsigned i;

	dev->n_subdevices = num_subdevices;
	dev->subdevices =
	    kcalloc(num_subdevices, sizeof(struct comedi_subdevice),
		    GFP_KERNEL);
	if (!dev->subdevices)
		return -ENOMEM;
	for (i = 0; i < num_subdevices; ++i) {
		dev->subdevices[i].device = dev;
		dev->subdevices[i].async_dma_dir = DMA_NONE;
		spin_lock_init(&dev->subdevices[i].spin_lock);
		dev->subdevices[i].minor = -1;
	}
	return 0;
}

static inline int alloc_private(struct comedi_device *dev, int size)
{
	dev->private = kzalloc(size, GFP_KERNEL);
	if (!dev->private)
		return -ENOMEM;
	return 0;
=======
static inline bool comedi_range_is_bipolar(struct comedi_subdevice *s,
					   unsigned int range)
{
	return s->range_table->range[range].min < 0;
}

static inline bool comedi_range_is_unipolar(struct comedi_subdevice *s,
					    unsigned int range)
{
	return s->range_table->range[range].min >= 0;
}

static inline bool comedi_chan_range_is_bipolar(struct comedi_subdevice *s,
						unsigned int chan,
						unsigned int range)
{
	return s->range_table_list[chan]->range[range].min < 0;
}

static inline bool comedi_chan_range_is_unipolar(struct comedi_subdevice *s,
						 unsigned int chan,
						 unsigned int range)
{
	return s->range_table_list[chan]->range[range].min >= 0;
}

/* munge between offset binary and two's complement values */
static inline unsigned int comedi_offset_munge(struct comedi_subdevice *s,
					       unsigned int val)
{
	return val ^ s->maxdata ^ (s->maxdata >> 1);
>>>>>>> refs/remotes/origin/master
}

static inline unsigned int bytes_per_sample(const struct comedi_subdevice *subd)
{
	if (subd->subdev_flags & SDF_LSAMPL)
		return sizeof(unsigned int);
	else
		return sizeof(short);
}

<<<<<<< HEAD
/* must be used in attach to set dev->hw_dev if you wish to dma directly
into comedi's buffer */
static inline void comedi_set_hw_dev(struct comedi_device *dev,
				     struct device *hw_dev)
{
	if (dev->hw_dev)
		put_device(dev->hw_dev);

	dev->hw_dev = hw_dev;
	if (dev->hw_dev) {
		dev->hw_dev = get_device(dev->hw_dev);
		BUG_ON(dev->hw_dev == NULL);
	}
}

int comedi_buf_put(struct comedi_async *async, short x);
int comedi_buf_get(struct comedi_async *async, short *x);

unsigned int comedi_buf_write_n_available(struct comedi_async *async);
unsigned int comedi_buf_write_alloc(struct comedi_async *async,
				    unsigned int nbytes);
unsigned int comedi_buf_write_alloc_strict(struct comedi_async *async,
					   unsigned int nbytes);
unsigned comedi_buf_write_free(struct comedi_async *async, unsigned int nbytes);
unsigned comedi_buf_read_alloc(struct comedi_async *async, unsigned nbytes);
unsigned comedi_buf_read_free(struct comedi_async *async, unsigned int nbytes);
unsigned int comedi_buf_read_n_available(struct comedi_async *async);
=======
/*
 * Must set dev->hw_dev if you wish to dma directly into comedi's buffer.
 * Also useful for retrieving a previously configured hardware device of
 * known bus type.  Set automatically for auto-configured devices.
 * Automatically set to NULL when detaching hardware device.
 */
int comedi_set_hw_dev(struct comedi_device *dev, struct device *hw_dev);

unsigned int comedi_buf_write_alloc(struct comedi_async *, unsigned int);
unsigned int comedi_buf_write_free(struct comedi_async *, unsigned int);

unsigned int comedi_buf_read_n_available(struct comedi_async *);
unsigned int comedi_buf_read_alloc(struct comedi_async *, unsigned int);
unsigned int comedi_buf_read_free(struct comedi_async *, unsigned int);

int comedi_buf_put(struct comedi_async *, unsigned short);
int comedi_buf_get(struct comedi_async *, unsigned short *);

>>>>>>> refs/remotes/origin/master
void comedi_buf_memcpy_to(struct comedi_async *async, unsigned int offset,
			  const void *source, unsigned int num_bytes);
void comedi_buf_memcpy_from(struct comedi_async *async, unsigned int offset,
			    void *destination, unsigned int num_bytes);
<<<<<<< HEAD
static inline unsigned comedi_buf_write_n_allocated(struct comedi_async *async)
{
	return async->buf_write_alloc_count - async->buf_write_count;
}

static inline unsigned comedi_buf_read_n_allocated(struct comedi_async *async)
{
	return async->buf_read_alloc_count - async->buf_read_count;
}

static inline void *comedi_aux_data(int options[], int n)
{
	unsigned long address;
	unsigned long addressLow;
	int bit_shift;
	if (sizeof(int) >= sizeof(void *))
		address = options[COMEDI_DEVCONF_AUX_DATA_LO];
	else {
		address = options[COMEDI_DEVCONF_AUX_DATA_HI];
		bit_shift = sizeof(int) * 8;
		address <<= bit_shift;
		addressLow = options[COMEDI_DEVCONF_AUX_DATA_LO];
		addressLow &= (1UL << bit_shift) - 1;
		address |= addressLow;
	}
	if (n >= 1)
		address += options[COMEDI_DEVCONF_AUX_DATA0_LENGTH];
	if (n >= 2)
		address += options[COMEDI_DEVCONF_AUX_DATA1_LENGTH];
	if (n >= 3)
		address += options[COMEDI_DEVCONF_AUX_DATA2_LENGTH];
	BUG_ON(n > 3);
	return (void *)address;
}

int comedi_alloc_subdevice_minor(struct comedi_device *dev,
				 struct comedi_subdevice *s);
void comedi_free_subdevice_minor(struct comedi_subdevice *s);
int comedi_pci_auto_config(struct pci_dev *pcidev, const char *board_name);
void comedi_pci_auto_unconfig(struct pci_dev *pcidev);
struct usb_device;		/* forward declaration */
int comedi_usb_auto_config(struct usb_device *usbdev, const char *board_name);
void comedi_usb_auto_unconfig(struct usb_device *usbdev);

#ifdef CONFIG_COMEDI_PCI_DRIVERS
#define CONFIG_COMEDI_PCI
#endif
#ifdef CONFIG_COMEDI_PCI_DRIVERS_MODULE
#define CONFIG_COMEDI_PCI
#endif
#ifdef CONFIG_COMEDI_PCMCIA_DRIVERS
#define CONFIG_COMEDI_PCMCIA
#endif
#ifdef CONFIG_COMEDI_PCMCIA_DRIVERS_MODULE
#define CONFIG_COMEDI_PCMCIA
#endif
=======

/* drivers.c - general comedi driver functions */

int comedi_dio_insn_config(struct comedi_device *, struct comedi_subdevice *,
			   struct comedi_insn *, unsigned int *data,
			   unsigned int mask);
unsigned int comedi_dio_update_state(struct comedi_subdevice *,
				     unsigned int *data);

void *comedi_alloc_devpriv(struct comedi_device *, size_t);
int comedi_alloc_subdevices(struct comedi_device *, int);

int comedi_load_firmware(struct comedi_device *, struct device *,
			 const char *name,
			 int (*cb)(struct comedi_device *,
				   const u8 *data, size_t size,
				   unsigned long context),
			 unsigned long context);

int __comedi_request_region(struct comedi_device *,
			    unsigned long start, unsigned long len);
int comedi_request_region(struct comedi_device *,
			  unsigned long start, unsigned long len);
void comedi_legacy_detach(struct comedi_device *);

int comedi_auto_config(struct device *, struct comedi_driver *,
		       unsigned long context);
void comedi_auto_unconfig(struct device *);

int comedi_driver_register(struct comedi_driver *);
void comedi_driver_unregister(struct comedi_driver *);

/**
 * module_comedi_driver() - Helper macro for registering a comedi driver
 * @__comedi_driver: comedi_driver struct
 *
 * Helper macro for comedi drivers which do not do anything special in module
 * init/exit. This eliminates a lot of boilerplate. Each module may only use
 * this macro once, and calling it replaces module_init() and module_exit().
 */
#define module_comedi_driver(__comedi_driver) \
	module_driver(__comedi_driver, comedi_driver_register, \
			comedi_driver_unregister)

#ifdef CONFIG_COMEDI_PCI_DRIVERS

/* comedi_pci.c - comedi PCI driver specific functions */

/*
 * PCI Vendor IDs not in <linux/pci_ids.h>
 */
#define PCI_VENDOR_ID_KOLTER		0x1001
#define PCI_VENDOR_ID_ICP		0x104c
#define PCI_VENDOR_ID_DT		0x1116
#define PCI_VENDOR_ID_IOTECH		0x1616
#define PCI_VENDOR_ID_CONTEC		0x1221
#define PCI_VENDOR_ID_RTD		0x1435
#define PCI_VENDOR_ID_HUMUSOFT		0x186c

struct pci_dev;
struct pci_driver;

struct pci_dev *comedi_to_pci_dev(struct comedi_device *);

int comedi_pci_enable(struct comedi_device *);
void comedi_pci_disable(struct comedi_device *);

int comedi_pci_auto_config(struct pci_dev *, struct comedi_driver *,
			   unsigned long context);
void comedi_pci_auto_unconfig(struct pci_dev *);

int comedi_pci_driver_register(struct comedi_driver *, struct pci_driver *);
void comedi_pci_driver_unregister(struct comedi_driver *, struct pci_driver *);

/**
 * module_comedi_pci_driver() - Helper macro for registering a comedi PCI driver
 * @__comedi_driver: comedi_driver struct
 * @__pci_driver: pci_driver struct
 *
 * Helper macro for comedi PCI drivers which do not do anything special
 * in module init/exit. This eliminates a lot of boilerplate. Each
 * module may only use this macro once, and calling it replaces
 * module_init() and module_exit()
 */
#define module_comedi_pci_driver(__comedi_driver, __pci_driver) \
	module_driver(__comedi_driver, comedi_pci_driver_register, \
			comedi_pci_driver_unregister, &(__pci_driver))

#else

/*
 * Some of the comedi mixed ISA/PCI drivers call the PCI specific
 * functions. Provide some dummy functions if CONFIG_COMEDI_PCI_DRIVERS
 * is not enabled.
 */

static inline struct pci_dev *comedi_to_pci_dev(struct comedi_device *dev)
{
	return NULL;
}

static inline int comedi_pci_enable(struct comedi_device *dev)
{
	return -ENOSYS;
}

static inline void comedi_pci_disable(struct comedi_device *dev)
{
}

#endif /* CONFIG_COMEDI_PCI_DRIVERS */

#ifdef CONFIG_COMEDI_PCMCIA_DRIVERS

/* comedi_pcmcia.c - comedi PCMCIA driver specific functions */

struct pcmcia_driver;
struct pcmcia_device;

struct pcmcia_device *comedi_to_pcmcia_dev(struct comedi_device *);

int comedi_pcmcia_enable(struct comedi_device *,
			 int (*conf_check)(struct pcmcia_device *, void *));
void comedi_pcmcia_disable(struct comedi_device *);

int comedi_pcmcia_auto_config(struct pcmcia_device *, struct comedi_driver *);
void comedi_pcmcia_auto_unconfig(struct pcmcia_device *);

int comedi_pcmcia_driver_register(struct comedi_driver *,
					struct pcmcia_driver *);
void comedi_pcmcia_driver_unregister(struct comedi_driver *,
					struct pcmcia_driver *);

/**
 * module_comedi_pcmcia_driver() - Helper macro for registering a comedi PCMCIA driver
 * @__comedi_driver: comedi_driver struct
 * @__pcmcia_driver: pcmcia_driver struct
 *
 * Helper macro for comedi PCMCIA drivers which do not do anything special
 * in module init/exit. This eliminates a lot of boilerplate. Each
 * module may only use this macro once, and calling it replaces
 * module_init() and module_exit()
 */
#define module_comedi_pcmcia_driver(__comedi_driver, __pcmcia_driver) \
	module_driver(__comedi_driver, comedi_pcmcia_driver_register, \
			comedi_pcmcia_driver_unregister, &(__pcmcia_driver))

#endif /* CONFIG_COMEDI_PCMCIA_DRIVERS */

#ifdef CONFIG_COMEDI_USB_DRIVERS

/* comedi_usb.c - comedi USB driver specific functions */

struct usb_driver;
struct usb_interface;

struct usb_interface *comedi_to_usb_interface(struct comedi_device *);
struct usb_device *comedi_to_usb_dev(struct comedi_device *);

int comedi_usb_auto_config(struct usb_interface *, struct comedi_driver *,
			   unsigned long context);
void comedi_usb_auto_unconfig(struct usb_interface *);

int comedi_usb_driver_register(struct comedi_driver *, struct usb_driver *);
void comedi_usb_driver_unregister(struct comedi_driver *, struct usb_driver *);

/**
 * module_comedi_usb_driver() - Helper macro for registering a comedi USB driver
 * @__comedi_driver: comedi_driver struct
 * @__usb_driver: usb_driver struct
 *
 * Helper macro for comedi USB drivers which do not do anything special
 * in module init/exit. This eliminates a lot of boilerplate. Each
 * module may only use this macro once, and calling it replaces
 * module_init() and module_exit()
 */
#define module_comedi_usb_driver(__comedi_driver, __usb_driver) \
	module_driver(__comedi_driver, comedi_usb_driver_register, \
			comedi_usb_driver_unregister, &(__usb_driver))

#endif /* CONFIG_COMEDI_USB_DRIVERS */
>>>>>>> refs/remotes/origin/master

#endif /* _COMEDIDEV_H */
