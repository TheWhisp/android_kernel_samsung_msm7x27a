/* The industrial I/O simple minimally locked ring buffer.
 *
 * Copyright (c) 2008 Jonathan Cameron
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include "ring_sw.h"
#include "trigger.h"

/**
 * struct iio_sw_ring_buffer - software ring buffer
 * @buf:		generic ring buffer elements
 * @data:		the ring buffer memory
 * @read_p:		read pointer (oldest available)
 * @write_p:		write pointer
<<<<<<< HEAD
 * @last_written_p:	read pointer (newest available)
 * @half_p:		half buffer length behind write_p (event generation)
 * @use_count:		reference count to prevent resizing when in use
 * @update_needed:	flag to indicated change in size requested
 * @use_lock:		lock to prevent change in size when in use
 *
 * Note that the first element of all ring buffers must be a
 * struct iio_ring_buffer.
**/
struct iio_sw_ring_buffer {
	struct iio_ring_buffer  buf;
	unsigned char		*data;
	unsigned char		*read_p;
	unsigned char		*write_p;
	unsigned char		*last_written_p;
	/* used to act as a point at which to signal an event */
	unsigned char		*half_p;
	int			use_count;
	int			update_needed;
	spinlock_t		use_lock;
=======
 * @half_p:		half buffer length behind write_p (event generation)
 * @update_needed:	flag to indicated change in size requested
 *
 * Note that the first element of all ring buffers must be a
 * struct iio_buffer.
**/
struct iio_sw_ring_buffer {
	struct iio_buffer  buf;
	unsigned char		*data;
	unsigned char		*read_p;
	unsigned char		*write_p;
	/* used to act as a point at which to signal an event */
	unsigned char		*half_p;
	int			update_needed;
>>>>>>> refs/remotes/origin/cm-10.0
};

#define iio_to_sw_ring(r) container_of(r, struct iio_sw_ring_buffer, buf)

static inline int __iio_allocate_sw_ring_buffer(struct iio_sw_ring_buffer *ring,
						int bytes_per_datum, int length)
{
	if ((length == 0) || (bytes_per_datum == 0))
		return -EINVAL;
<<<<<<< HEAD
	__iio_update_ring_buffer(&ring->buf, bytes_per_datum, length);
	ring->data = kmalloc(length*ring->buf.bytes_per_datum, GFP_ATOMIC);
	ring->read_p = NULL;
	ring->write_p = NULL;
	ring->last_written_p = NULL;
=======
	__iio_update_buffer(&ring->buf, bytes_per_datum, length);
	ring->data = kmalloc(length*ring->buf.bytes_per_datum, GFP_ATOMIC);
	ring->read_p = NULL;
	ring->write_p = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
	ring->half_p = NULL;
	return ring->data ? 0 : -ENOMEM;
}

<<<<<<< HEAD
static inline void __iio_init_sw_ring_buffer(struct iio_sw_ring_buffer *ring)
{
	spin_lock_init(&ring->use_lock);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static inline void __iio_free_sw_ring_buffer(struct iio_sw_ring_buffer *ring)
{
	kfree(ring->data);
}

<<<<<<< HEAD
static void iio_mark_sw_rb_in_use(struct iio_ring_buffer *r)
{
	struct iio_sw_ring_buffer *ring = iio_to_sw_ring(r);
	spin_lock(&ring->use_lock);
	ring->use_count++;
	spin_unlock(&ring->use_lock);
}

static void iio_unmark_sw_rb_in_use(struct iio_ring_buffer *r)
{
	struct iio_sw_ring_buffer *ring = iio_to_sw_ring(r);
	spin_lock(&ring->use_lock);
	ring->use_count--;
	spin_unlock(&ring->use_lock);
}


=======
>>>>>>> refs/remotes/origin/cm-10.0
/* Ring buffer related functionality */
/* Store to ring is typically called in the bh of a data ready interrupt handler
 * in the device driver */
/* Lock always held if their is a chance this may be called */
/* Only one of these per ring may run concurrently - enforced by drivers */
static int iio_store_to_sw_ring(struct iio_sw_ring_buffer *ring,
				unsigned char *data, s64 timestamp)
{
	int ret = 0;
	unsigned char *temp_ptr, *change_test_ptr;

	/* initial store */
	if (unlikely(ring->write_p == NULL)) {
		ring->write_p = ring->data;
		/* Doesn't actually matter if this is out of the set
		 * as long as the read pointer is valid before this
		 * passes it - guaranteed as set later in this function.
		 */
		ring->half_p = ring->data - ring->buf.length*ring->buf.bytes_per_datum/2;
	}
	/* Copy data to where ever the current write pointer says */
	memcpy(ring->write_p, data, ring->buf.bytes_per_datum);
	barrier();
	/* Update the pointer used to get most recent value.
	 * Always valid as either points to latest or second latest value.
	 * Before this runs it is null and read attempts fail with -EAGAIN.
	 */
<<<<<<< HEAD
	ring->last_written_p = ring->write_p;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	barrier();
	/* temp_ptr used to ensure we never have an invalid pointer
	 * it may be slightly lagging, but never invalid
	 */
	temp_ptr = ring->write_p + ring->buf.bytes_per_datum;
	/* End of ring, back to the beginning */
	if (temp_ptr == ring->data + ring->buf.length*ring->buf.bytes_per_datum)
		temp_ptr = ring->data;
	/* Update the write pointer
	 * always valid as long as this is the only function able to write.
	 * Care needed with smp systems to ensure more than one ring fill
	 * is never scheduled.
	 */
	ring->write_p = temp_ptr;

	if (ring->read_p == NULL)
		ring->read_p = ring->data;
	/* Buffer full - move the read pointer and create / escalate
	 * ring event */
	/* Tricky case - if the read pointer moves before we adjust it.
	 * Handle by not pushing if it has moved - may result in occasional
	 * unnecessary buffer full events when it wasn't quite true.
	 */
	else if (ring->write_p == ring->read_p) {
		change_test_ptr = ring->read_p;
		temp_ptr = change_test_ptr + ring->buf.bytes_per_datum;
		if (temp_ptr
		    == ring->data + ring->buf.length*ring->buf.bytes_per_datum) {
			temp_ptr = ring->data;
		}
		/* We are moving pointer on one because the ring is full.  Any
		 * change to the read pointer will be this or greater.
		 */
		if (change_test_ptr == ring->read_p)
			ring->read_p = temp_ptr;
	}
	/* investigate if our event barrier has been passed */
	/* There are definite 'issues' with this and chances of
	 * simultaneous read */
	/* Also need to use loop count to ensure this only happens once */
	ring->half_p += ring->buf.bytes_per_datum;
	if (ring->half_p == ring->data + ring->buf.length*ring->buf.bytes_per_datum)
		ring->half_p = ring->data;
	if (ring->half_p == ring->read_p) {
		ring->buf.stufftoread = true;
		wake_up_interruptible(&ring->buf.pollq);
	}
	return ret;
}

<<<<<<< HEAD
static int iio_read_first_n_sw_rb(struct iio_ring_buffer *r,
=======
static int iio_read_first_n_sw_rb(struct iio_buffer *r,
>>>>>>> refs/remotes/origin/cm-10.0
				  size_t n, char __user *buf)
{
	struct iio_sw_ring_buffer *ring = iio_to_sw_ring(r);

	u8 *initial_read_p, *initial_write_p, *current_read_p, *end_read_p;
	u8 *data;
	int ret, max_copied, bytes_to_rip, dead_offset;
<<<<<<< HEAD

	/* A userspace program has probably made an error if it tries to
	 *  read something that is not a whole number of bpds.
=======
	size_t data_available, buffer_size;

	/* A userspace program has probably made an error if it tries to
	 * read something that is not a whole number of bpds.
>>>>>>> refs/remotes/origin/cm-10.0
	 * Return an error.
	 */
	if (n % ring->buf.bytes_per_datum) {
		ret = -EINVAL;
		printk(KERN_INFO "Ring buffer read request not whole number of"
		       "samples: Request bytes %zd, Current bytes per datum %d\n",
		       n, ring->buf.bytes_per_datum);
		goto error_ret;
	}
<<<<<<< HEAD
	/* Limit size to whole of ring buffer */
	bytes_to_rip = min((size_t)(ring->buf.bytes_per_datum*ring->buf.length),
			   n);
=======

	buffer_size = ring->buf.bytes_per_datum*ring->buf.length;

	/* Limit size to whole of ring buffer */
	bytes_to_rip = min_t(size_t, buffer_size, n);
>>>>>>> refs/remotes/origin/cm-10.0

	data = kmalloc(bytes_to_rip, GFP_KERNEL);
	if (data == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}

	/* build local copy */
	initial_read_p = ring->read_p;
	if (unlikely(initial_read_p == NULL)) { /* No data here as yet */
		ret = 0;
		goto error_free_data_cpy;
	}

	initial_write_p = ring->write_p;

	/* Need a consistent pair */
	while ((initial_read_p != ring->read_p)
	       || (initial_write_p != ring->write_p)) {
		initial_read_p = ring->read_p;
		initial_write_p = ring->write_p;
	}
	if (initial_write_p == initial_read_p) {
		/* No new data available.*/
		ret = 0;
		goto error_free_data_cpy;
	}

<<<<<<< HEAD
	if (initial_write_p >= initial_read_p + bytes_to_rip) {
		/* write_p is greater than necessary, all is easy */
		max_copied = bytes_to_rip;
		memcpy(data, initial_read_p, max_copied);
		end_read_p = initial_read_p + max_copied;
	} else if (initial_write_p > initial_read_p) {
		/*not enough data to cpy */
		max_copied = initial_write_p - initial_read_p;
		memcpy(data, initial_read_p, max_copied);
		end_read_p = initial_write_p;
	} else {
		/* going through 'end' of ring buffer */
		max_copied = ring->data
			+ ring->buf.length*ring->buf.bytes_per_datum - initial_read_p;
		memcpy(data, initial_read_p, max_copied);
		/* possible we are done if we align precisely with end */
		if (max_copied == bytes_to_rip)
			end_read_p = ring->data;
		else if (initial_write_p
			 > ring->data + bytes_to_rip - max_copied) {
			/* enough data to finish */
			memcpy(data + max_copied, ring->data,
			       bytes_to_rip - max_copied);
			max_copied = bytes_to_rip;
			end_read_p = ring->data + (bytes_to_rip - max_copied);
		} else {  /* not enough data */
			memcpy(data + max_copied, ring->data,
			       initial_write_p - ring->data);
			max_copied += initial_write_p - ring->data;
			end_read_p = initial_write_p;
		}
	}
=======
	if (initial_write_p >= initial_read_p)
		data_available = initial_write_p - initial_read_p;
	else
		data_available = buffer_size - (initial_read_p - initial_write_p);

	if (data_available < bytes_to_rip)
		bytes_to_rip = data_available;

	if (initial_read_p + bytes_to_rip >= ring->data + buffer_size) {
		max_copied = ring->data + buffer_size - initial_read_p;
		memcpy(data, initial_read_p, max_copied);
		memcpy(data + max_copied, ring->data, bytes_to_rip - max_copied);
		end_read_p = ring->data + bytes_to_rip - max_copied;
	} else {
		memcpy(data, initial_read_p, bytes_to_rip);
		end_read_p = initial_read_p + bytes_to_rip;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	/* Now to verify which section was cleanly copied - i.e. how far
	 * read pointer has been pushed */
	current_read_p = ring->read_p;

	if (initial_read_p <= current_read_p)
		dead_offset = current_read_p - initial_read_p;
	else
<<<<<<< HEAD
		dead_offset = ring->buf.length*ring->buf.bytes_per_datum
			- (initial_read_p - current_read_p);
=======
		dead_offset = buffer_size - (initial_read_p - current_read_p);
>>>>>>> refs/remotes/origin/cm-10.0

	/* possible issue if the initial write has been lapped or indeed
	 * the point we were reading to has been passed */
	/* No valid data read.
	 * In this case the read pointer is already correct having been
	 * pushed further than we would look. */
<<<<<<< HEAD
	if (max_copied - dead_offset < 0) {
=======
	if (bytes_to_rip - dead_offset < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
		ret = 0;
		goto error_free_data_cpy;
	}

	/* setup the next read position */
	/* Beware, this may fail due to concurrency fun and games.
<<<<<<< HEAD
	 *  Possible that sufficient fill commands have run to push the read
=======
	 * Possible that sufficient fill commands have run to push the read
>>>>>>> refs/remotes/origin/cm-10.0
	 * pointer past where we would be after the rip. If this occurs, leave
	 * it be.
	 */
	/* Tricky - deal with loops */

	while (ring->read_p != end_read_p)
		ring->read_p = end_read_p;

<<<<<<< HEAD
	ret = max_copied - dead_offset;
=======
	ret = bytes_to_rip - dead_offset;
>>>>>>> refs/remotes/origin/cm-10.0

	if (copy_to_user(buf, data + dead_offset, ret))  {
		ret =  -EFAULT;
		goto error_free_data_cpy;
	}

	if (bytes_to_rip >= ring->buf.length*ring->buf.bytes_per_datum/2)
		ring->buf.stufftoread = 0;

error_free_data_cpy:
	kfree(data);
error_ret:

	return ret;
}

<<<<<<< HEAD
static int iio_store_to_sw_rb(struct iio_ring_buffer *r,
=======
static int iio_store_to_sw_rb(struct iio_buffer *r,
>>>>>>> refs/remotes/origin/cm-10.0
			      u8 *data,
			      s64 timestamp)
{
	struct iio_sw_ring_buffer *ring = iio_to_sw_ring(r);
	return iio_store_to_sw_ring(ring, data, timestamp);
}

<<<<<<< HEAD
static int iio_read_last_from_sw_ring(struct iio_sw_ring_buffer *ring,
				      unsigned char *data)
{
	unsigned char *last_written_p_copy;

	iio_mark_sw_rb_in_use(&ring->buf);
again:
	barrier();
	last_written_p_copy = ring->last_written_p;
	barrier(); /*unnessecary? */
	/* Check there is anything here */
	if (last_written_p_copy == NULL)
		return -EAGAIN;
	memcpy(data, last_written_p_copy, ring->buf.bytes_per_datum);

	if (unlikely(ring->last_written_p != last_written_p_copy))
		goto again;

	iio_unmark_sw_rb_in_use(&ring->buf);
	return 0;
}

static int iio_read_last_from_sw_rb(struct iio_ring_buffer *r,
			     unsigned char *data)
{
	return iio_read_last_from_sw_ring(iio_to_sw_ring(r), data);
}

static int iio_request_update_sw_rb(struct iio_ring_buffer *r)
=======
static int iio_request_update_sw_rb(struct iio_buffer *r)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int ret = 0;
	struct iio_sw_ring_buffer *ring = iio_to_sw_ring(r);

	r->stufftoread = false;
<<<<<<< HEAD
	spin_lock(&ring->use_lock);
	if (!ring->update_needed)
		goto error_ret;
	if (ring->use_count) {
		ret = -EAGAIN;
		goto error_ret;
	}
=======
	if (!ring->update_needed)
		goto error_ret;
>>>>>>> refs/remotes/origin/cm-10.0
	__iio_free_sw_ring_buffer(ring);
	ret = __iio_allocate_sw_ring_buffer(ring, ring->buf.bytes_per_datum,
					    ring->buf.length);
error_ret:
<<<<<<< HEAD
	spin_unlock(&ring->use_lock);
	return ret;
}

static int iio_get_bytes_per_datum_sw_rb(struct iio_ring_buffer *r)
=======
	return ret;
}

static int iio_get_bytes_per_datum_sw_rb(struct iio_buffer *r)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct iio_sw_ring_buffer *ring = iio_to_sw_ring(r);
	return ring->buf.bytes_per_datum;
}

<<<<<<< HEAD
static int iio_set_bytes_per_datum_sw_rb(struct iio_ring_buffer *r, size_t bpd)
{
	if (r->bytes_per_datum != bpd) {
		r->bytes_per_datum = bpd;
		if (r->access->mark_param_change)
			r->access->mark_param_change(r);
=======
static int iio_mark_update_needed_sw_rb(struct iio_buffer *r)
{
	struct iio_sw_ring_buffer *ring = iio_to_sw_ring(r);
	ring->update_needed = true;
	return 0;
}

static int iio_set_bytes_per_datum_sw_rb(struct iio_buffer *r, size_t bpd)
{
	if (r->bytes_per_datum != bpd) {
		r->bytes_per_datum = bpd;
		iio_mark_update_needed_sw_rb(r);
>>>>>>> refs/remotes/origin/cm-10.0
	}
	return 0;
}

<<<<<<< HEAD
static int iio_get_length_sw_rb(struct iio_ring_buffer *r)
=======
static int iio_get_length_sw_rb(struct iio_buffer *r)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return r->length;
}

<<<<<<< HEAD
static int iio_set_length_sw_rb(struct iio_ring_buffer *r, int length)
{
	if (r->length != length) {
		r->length = length;
		if (r->access->mark_param_change)
			r->access->mark_param_change(r);
=======
static int iio_set_length_sw_rb(struct iio_buffer *r, int length)
{
	if (r->length != length) {
		r->length = length;
		iio_mark_update_needed_sw_rb(r);
>>>>>>> refs/remotes/origin/cm-10.0
	}
	return 0;
}

<<<<<<< HEAD
static int iio_mark_update_needed_sw_rb(struct iio_ring_buffer *r)
{
	struct iio_sw_ring_buffer *ring = iio_to_sw_ring(r);
	ring->update_needed = true;
	return 0;
}

static void iio_sw_rb_release(struct device *dev)
{
	struct iio_ring_buffer *r = to_iio_ring_buffer(dev);
	iio_ring_access_release(&r->dev);
	kfree(iio_to_sw_ring(r));
}

static IIO_RING_ENABLE_ATTR;
static IIO_RING_BYTES_PER_DATUM_ATTR;
static IIO_RING_LENGTH_ATTR;
=======
static IIO_BUFFER_ENABLE_ATTR;
static IIO_BUFFER_LENGTH_ATTR;
>>>>>>> refs/remotes/origin/cm-10.0

/* Standard set of ring buffer attributes */
static struct attribute *iio_ring_attributes[] = {
	&dev_attr_length.attr,
<<<<<<< HEAD
	&dev_attr_bytes_per_datum.attr,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	&dev_attr_enable.attr,
	NULL,
};

static struct attribute_group iio_ring_attribute_group = {
	.attrs = iio_ring_attributes,
<<<<<<< HEAD
};

static const struct attribute_group *iio_ring_attribute_groups[] = {
	&iio_ring_attribute_group,
	NULL
};

static struct device_type iio_sw_ring_type = {
	.release = iio_sw_rb_release,
	.groups = iio_ring_attribute_groups,
};

struct iio_ring_buffer *iio_sw_rb_allocate(struct iio_dev *indio_dev)
{
	struct iio_ring_buffer *buf;
=======
	.name = "buffer",
};

static const struct iio_buffer_access_funcs ring_sw_access_funcs = {
	.store_to = &iio_store_to_sw_rb,
	.read_first_n = &iio_read_first_n_sw_rb,
	.request_update = &iio_request_update_sw_rb,
	.get_bytes_per_datum = &iio_get_bytes_per_datum_sw_rb,
	.set_bytes_per_datum = &iio_set_bytes_per_datum_sw_rb,
	.get_length = &iio_get_length_sw_rb,
	.set_length = &iio_set_length_sw_rb,
};

struct iio_buffer *iio_sw_rb_allocate(struct iio_dev *indio_dev)
{
	struct iio_buffer *buf;
>>>>>>> refs/remotes/origin/cm-10.0
	struct iio_sw_ring_buffer *ring;

	ring = kzalloc(sizeof *ring, GFP_KERNEL);
	if (!ring)
		return NULL;
	ring->update_needed = true;
	buf = &ring->buf;
<<<<<<< HEAD
	iio_ring_buffer_init(buf, indio_dev);
	__iio_init_sw_ring_buffer(ring);
	buf->dev.type = &iio_sw_ring_type;
	buf->dev.parent = &indio_dev->dev;
	dev_set_drvdata(&buf->dev, (void *)buf);
=======
	iio_buffer_init(buf);
	buf->attrs = &iio_ring_attribute_group;
	buf->access = &ring_sw_access_funcs;
>>>>>>> refs/remotes/origin/cm-10.0

	return buf;
}
EXPORT_SYMBOL(iio_sw_rb_allocate);

<<<<<<< HEAD
void iio_sw_rb_free(struct iio_ring_buffer *r)
{
	if (r)
		iio_put_ring_buffer(r);
}
EXPORT_SYMBOL(iio_sw_rb_free);

const struct iio_ring_access_funcs ring_sw_access_funcs = {
	.mark_in_use = &iio_mark_sw_rb_in_use,
	.unmark_in_use = &iio_unmark_sw_rb_in_use,
	.store_to = &iio_store_to_sw_rb,
	.read_last = &iio_read_last_from_sw_rb,
	.read_first_n = &iio_read_first_n_sw_rb,
	.mark_param_change = &iio_mark_update_needed_sw_rb,
	.request_update = &iio_request_update_sw_rb,
	.get_bytes_per_datum = &iio_get_bytes_per_datum_sw_rb,
	.set_bytes_per_datum = &iio_set_bytes_per_datum_sw_rb,
	.get_length = &iio_get_length_sw_rb,
	.set_length = &iio_set_length_sw_rb,
};
EXPORT_SYMBOL(ring_sw_access_funcs);

=======
void iio_sw_rb_free(struct iio_buffer *r)
{
	kfree(iio_to_sw_ring(r));
}
EXPORT_SYMBOL(iio_sw_rb_free);

>>>>>>> refs/remotes/origin/cm-10.0
MODULE_DESCRIPTION("Industrialio I/O software ring buffer");
MODULE_LICENSE("GPL");
