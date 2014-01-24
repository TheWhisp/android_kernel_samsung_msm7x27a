#ifndef _LINUX_VIRTIO_H
#define _LINUX_VIRTIO_H
/* Everything a virtio driver needs to work with any particular virtio
 * implementation. */
#include <linux/types.h>
#include <linux/scatterlist.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/mod_devicetable.h>
#include <linux/gfp.h>
<<<<<<< HEAD
=======
#include <linux/vringh.h>
>>>>>>> refs/remotes/origin/master

/**
 * virtqueue - a queue to register buffers for sending or receiving.
 * @list: the chain of virtqueues for this device
 * @callback: the function to call when buffers are consumed (can be NULL).
 * @name: the name of this virtqueue (mainly for debugging)
 * @vdev: the virtio device this queue was created for.
 * @priv: a pointer for the virtqueue implementation to use.
<<<<<<< HEAD
=======
 * @index: the zero-based ordinal number for this queue.
 * @num_free: number of elements we expect to be able to fit.
 *
 * A note on @num_free: with indirect buffers, each buffer needs one
 * element in the queue, otherwise a buffer will need one element per
 * sg element.
>>>>>>> refs/remotes/origin/master
 */
struct virtqueue {
	struct list_head list;
	void (*callback)(struct virtqueue *vq);
	const char *name;
	struct virtio_device *vdev;
<<<<<<< HEAD
	void *priv;
};

<<<<<<< HEAD
/**
 * operations for virtqueue
 * virtqueue_add_buf: expose buffer to other end
 *	vq: the struct virtqueue we're talking about.
 *	sg: the description of the buffer(s).
 *	out_num: the number of sg readable by other side
 *	in_num: the number of sg which are writable (after readable ones)
 *	data: the token identifying the buffer.
 *	gfp: how to do memory allocations (if necessary).
 *      Returns remaining capacity of queue (sg segments) or a negative error.
 * virtqueue_kick: update after add_buf
 *	vq: the struct virtqueue
 *	After one or more add_buf calls, invoke this to kick the other side.
 * virtqueue_get_buf: get the next used buffer
 *	vq: the struct virtqueue we're talking about.
 *	len: the length written into the buffer
 *	Returns NULL or the "data" token handed to add_buf.
 * virtqueue_disable_cb: disable callbacks
 *	vq: the struct virtqueue we're talking about.
 *	Note that this is not necessarily synchronous, hence unreliable and only
 *	useful as an optimization.
 * virtqueue_enable_cb: restart callbacks after disable_cb.
 *	vq: the struct virtqueue we're talking about.
 *	This re-enables callbacks; it returns "false" if there are pending
 *	buffers in the queue, to detect a possible race between the driver
 *	checking for more work, and enabling callbacks.
 * virtqueue_enable_cb_delayed: restart callbacks after disable_cb.
 *	vq: the struct virtqueue we're talking about.
 *	This re-enables callbacks but hints to the other side to delay
 *	interrupts until most of the available buffers have been processed;
 *	it returns "false" if there are many pending buffers in the queue,
 *	to detect a possible race between the driver checking for more work,
 *	and enabling callbacks.
 * virtqueue_detach_unused_buf: detach first unused buffer
 * 	vq: the struct virtqueue we're talking about.
 * 	Returns NULL or the "data" token handed to add_buf
 *
 * Locking rules are straightforward: the driver is responsible for
 * locking.  No two operations may be invoked simultaneously, with the exception
 * of virtqueue_disable_cb.
 *
 * All operations can be called in any context.
 */

int virtqueue_add_buf_gfp(struct virtqueue *vq,
			  struct scatterlist sg[],
			  unsigned int out_num,
			  unsigned int in_num,
			  void *data,
			  gfp_t gfp);

static inline int virtqueue_add_buf(struct virtqueue *vq,
				    struct scatterlist sg[],
				    unsigned int out_num,
				    unsigned int in_num,
				    void *data)
{
	return virtqueue_add_buf_gfp(vq, sg, out_num, in_num, data, GFP_ATOMIC);
}

void virtqueue_kick(struct virtqueue *vq);

=======
int virtqueue_add_buf(struct virtqueue *vq,
		      struct scatterlist sg[],
		      unsigned int out_num,
		      unsigned int in_num,
		      void *data,
		      gfp_t gfp);

void virtqueue_kick(struct virtqueue *vq);

bool virtqueue_kick_prepare(struct virtqueue *vq);

void virtqueue_notify(struct virtqueue *vq);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned int index;
	unsigned int num_free;
	void *priv;
};

int virtqueue_add_outbuf(struct virtqueue *vq,
			 struct scatterlist sg[], unsigned int num,
			 void *data,
			 gfp_t gfp);

int virtqueue_add_inbuf(struct virtqueue *vq,
			struct scatterlist sg[], unsigned int num,
			void *data,
			gfp_t gfp);

int virtqueue_add_sgs(struct virtqueue *vq,
		      struct scatterlist *sgs[],
		      unsigned int out_sgs,
		      unsigned int in_sgs,
		      void *data,
		      gfp_t gfp);

bool virtqueue_kick(struct virtqueue *vq);

bool virtqueue_kick_prepare(struct virtqueue *vq);

bool virtqueue_notify(struct virtqueue *vq);

>>>>>>> refs/remotes/origin/master
void *virtqueue_get_buf(struct virtqueue *vq, unsigned int *len);

void virtqueue_disable_cb(struct virtqueue *vq);

bool virtqueue_enable_cb(struct virtqueue *vq);

unsigned virtqueue_enable_cb_prepare(struct virtqueue *vq);

bool virtqueue_poll(struct virtqueue *vq, unsigned);

bool virtqueue_enable_cb_delayed(struct virtqueue *vq);

void *virtqueue_detach_unused_buf(struct virtqueue *vq);

<<<<<<< HEAD
<<<<<<< HEAD
=======
unsigned int virtqueue_get_vring_size(struct virtqueue *vq);

>>>>>>> refs/remotes/origin/cm-10.0
=======
unsigned int virtqueue_get_vring_size(struct virtqueue *vq);

bool virtqueue_is_broken(struct virtqueue *vq);

>>>>>>> refs/remotes/origin/master
/**
 * virtio_device - representation of a device using virtio
 * @index: unique position on the virtio bus
 * @dev: underlying device.
 * @id: the device type identification (used to match it with a driver).
 * @config: the configuration ops for this device.
<<<<<<< HEAD
=======
 * @vringh_config: configuration ops for host vrings.
>>>>>>> refs/remotes/origin/master
 * @vqs: the list of virtqueues for this device.
 * @features: the features supported by both driver and device.
 * @priv: private pointer for the driver's use.
 */
struct virtio_device {
	int index;
	struct device dev;
	struct virtio_device_id id;
<<<<<<< HEAD
	struct virtio_config_ops *config;
=======
	const struct virtio_config_ops *config;
	const struct vringh_config_ops *vringh_config;
>>>>>>> refs/remotes/origin/master
	struct list_head vqs;
	/* Note that this is a Linux set_bit-style bitmap. */
	unsigned long features[1];
	void *priv;
};

<<<<<<< HEAD
#define dev_to_virtio(dev) container_of(dev, struct virtio_device, dev)
=======
static inline struct virtio_device *dev_to_virtio(struct device *_dev)
{
	return container_of(_dev, struct virtio_device, dev);
}

>>>>>>> refs/remotes/origin/master
int register_virtio_device(struct virtio_device *dev);
void unregister_virtio_device(struct virtio_device *dev);

/**
 * virtio_driver - operations for a virtio I/O driver
 * @driver: underlying device driver (populate name and owner).
 * @id_table: the ids serviced by this driver.
<<<<<<< HEAD
<<<<<<< HEAD
 * @feature_table: an array of feature numbers supported by this device.
 * @feature_table_size: number of entries in the feature table array.
 * @probe: the function to call when a device is found.  Returns 0 or -errno.
 * @remove: the function when a device is removed.
=======
=======
>>>>>>> refs/remotes/origin/master
 * @feature_table: an array of feature numbers supported by this driver.
 * @feature_table_size: number of entries in the feature table array.
 * @probe: the function to call when a device is found.  Returns 0 or -errno.
 * @remove: the function to call when a device is removed.
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * @config_changed: optional function to call when the device configuration
 *    changes; may be called in interrupt context.
 */
struct virtio_driver {
	struct device_driver driver;
	const struct virtio_device_id *id_table;
	const unsigned int *feature_table;
	unsigned int feature_table_size;
	int (*probe)(struct virtio_device *dev);
<<<<<<< HEAD
	void (*remove)(struct virtio_device *dev);
	void (*config_changed)(struct virtio_device *dev);
<<<<<<< HEAD
=======
=======
	void (*scan)(struct virtio_device *dev);
	void (*remove)(struct virtio_device *dev);
	void (*config_changed)(struct virtio_device *dev);
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PM
	int (*freeze)(struct virtio_device *dev);
	int (*restore)(struct virtio_device *dev);
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
};

int register_virtio_driver(struct virtio_driver *drv);
void unregister_virtio_driver(struct virtio_driver *drv);
=======
};

static inline struct virtio_driver *drv_to_virtio(struct device_driver *drv)
{
	return container_of(drv, struct virtio_driver, driver);
}

int register_virtio_driver(struct virtio_driver *drv);
void unregister_virtio_driver(struct virtio_driver *drv);

/* module_virtio_driver() - Helper macro for drivers that don't do
 * anything special in module init/exit.  This eliminates a lot of
 * boilerplate.  Each module may only use this macro once, and
 * calling it replaces module_init() and module_exit()
 */
#define module_virtio_driver(__virtio_driver) \
	module_driver(__virtio_driver, register_virtio_driver, \
			unregister_virtio_driver)
>>>>>>> refs/remotes/origin/master
#endif /* _LINUX_VIRTIO_H */
