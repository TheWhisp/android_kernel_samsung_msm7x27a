/*
<<<<<<< HEAD
 * Copyright (C) 2010 Michael Hennerich, Analog Devices Inc.
=======
 * Copyright (C) 2010-2012 Michael Hennerich, Analog Devices Inc.
>>>>>>> refs/remotes/origin/master
 * Copyright (C) 2008-2010 Jonathan Cameron
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * ad799x_ring.c
 */

#include <linux/interrupt.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
=======
#include <linux/slab.h>
#include <linux/kernel.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/slab.h>
#include <linux/kernel.h>
>>>>>>> refs/remotes/origin/master
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/bitops.h>

<<<<<<< HEAD
#include "../iio.h"
<<<<<<< HEAD
#include "../ring_generic.h"
#include "../ring_sw.h"
#include "../trigger.h"
#include "../sysfs.h"

#include "ad799x.h"

int ad799x_single_channel_from_ring(struct ad799x_state *st, long mask)
{
	struct iio_ring_buffer *ring = iio_priv_to_dev(st)->ring;
	int count = 0, ret;
	u16 *ring_data;

	if (!(ring->scan_mask & mask)) {
		ret = -EBUSY;
		goto error_ret;
	}

	ring_data = kmalloc(ring->access->get_bytes_per_datum(ring),
			    GFP_KERNEL);
	if (ring_data == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}
	ret = ring->access->read_last(ring, (u8 *) ring_data);
	if (ret)
		goto error_free_ring_data;
	/* Need a count of channels prior to this one */
	mask >>= 1;
	while (mask) {
		if (mask & ring->scan_mask)
			count++;
		mask >>= 1;
	}

	ret = be16_to_cpu(ring_data[count]);

error_free_ring_data:
	kfree(ring_data);
error_ret:
	return ret;
}

/**
 * ad799x_ring_preenable() setup the parameters of the ring before enabling
 *
 * The complex nature of the setting of the nuber of bytes per datum is due
=======
#include "../buffer.h"
#include "../ring_sw.h"
#include "../trigger_consumer.h"

#include "ad799x.h"

/**
 * ad799x_ring_preenable() setup the parameters of the ring before enabling
 *
 * The complex nature of the setting of the number of bytes per datum is due
>>>>>>> refs/remotes/origin/cm-10.0
 * to this driver currently ensuring that the timestamp is stored at an 8
 * byte boundary.
 **/
static int ad799x_ring_preenable(struct iio_dev *indio_dev)
{
<<<<<<< HEAD
	struct iio_ring_buffer *ring = indio_dev->ring;
	struct ad799x_state *st = iio_dev_get_devdata(indio_dev);
=======
	struct iio_buffer *ring = indio_dev->buffer;
	struct ad799x_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * Need to figure out the current mode based upon the requested
	 * scan mask in iio_dev
	 */

	if (st->id == ad7997 || st->id == ad7998)
<<<<<<< HEAD
		ad7997_8_set_scan_mode(st, ring->scan_mask);

	st->d_size = ring->scan_count * 2;
=======
		ad7997_8_set_scan_mode(st, *indio_dev->active_scan_mask);

	st->d_size = bitmap_weight(indio_dev->active_scan_mask,
				   indio_dev->masklength) * 2;
>>>>>>> refs/remotes/origin/cm-10.0

	if (ring->scan_timestamp) {
		st->d_size += sizeof(s64);

		if (st->d_size % sizeof(s64))
			st->d_size += sizeof(s64) - (st->d_size % sizeof(s64));
	}

<<<<<<< HEAD
	if (indio_dev->ring->access->set_bytes_per_datum)
		indio_dev->ring->access->set_bytes_per_datum(indio_dev->ring,
							    st->d_size);
=======
	if (indio_dev->buffer->access->set_bytes_per_datum)
		indio_dev->buffer->access->
			set_bytes_per_datum(indio_dev->buffer, st->d_size);
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

=======
#include <linux/iio/iio.h>
#include <linux/iio/buffer.h>
#include <linux/iio/trigger_consumer.h>
#include <linux/iio/triggered_buffer.h>

#include "ad799x.h"

>>>>>>> refs/remotes/origin/master
/**
 * ad799x_trigger_handler() bh of trigger launched polling to ring buffer
 *
 * Currently there is no option in this driver to disable the saving of
 * timestamps within the ring.
 **/

static irqreturn_t ad799x_trigger_handler(int irq, void *p)
{
	struct iio_poll_func *pf = p;
<<<<<<< HEAD
<<<<<<< HEAD
	struct iio_dev *indio_dev = pf->private_data;
	struct ad799x_state *st = iio_dev_get_devdata(indio_dev);
	struct iio_ring_buffer *ring = indio_dev->ring;
=======
	struct iio_dev *indio_dev = pf->indio_dev;
	struct ad799x_state *st = iio_priv(indio_dev);
	struct iio_buffer *ring = indio_dev->buffer;
>>>>>>> refs/remotes/origin/cm-10.0
	s64 time_ns;
	__u8 *rxbuf;
	int b_sent;
	u8 cmd;

	rxbuf = kmalloc(st->d_size, GFP_KERNEL);
	if (rxbuf == NULL)
		goto out;

=======
	struct iio_dev *indio_dev = pf->indio_dev;
	struct ad799x_state *st = iio_priv(indio_dev);
	int b_sent;
	u8 cmd;

>>>>>>> refs/remotes/origin/master
	switch (st->id) {
	case ad7991:
	case ad7995:
	case ad7999:
<<<<<<< HEAD
<<<<<<< HEAD
		cmd = st->config | (ring->scan_mask << AD799X_CHANNEL_SHIFT);
=======
		cmd = st->config |
			(*indio_dev->active_scan_mask << AD799X_CHANNEL_SHIFT);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		cmd = st->config |
			(*indio_dev->active_scan_mask << AD799X_CHANNEL_SHIFT);
>>>>>>> refs/remotes/origin/master
		break;
	case ad7992:
	case ad7993:
	case ad7994:
<<<<<<< HEAD
<<<<<<< HEAD
		cmd = (ring->scan_mask << AD799X_CHANNEL_SHIFT) |
=======
		cmd = (*indio_dev->active_scan_mask << AD799X_CHANNEL_SHIFT) |
>>>>>>> refs/remotes/origin/cm-10.0
=======
		cmd = (*indio_dev->active_scan_mask << AD799X_CHANNEL_SHIFT) |
>>>>>>> refs/remotes/origin/master
			AD7998_CONV_RES_REG;
		break;
	case ad7997:
	case ad7998:
		cmd = AD7997_8_READ_SEQUENCE | AD7998_CONV_RES_REG;
		break;
	default:
		cmd = 0;
	}

	b_sent = i2c_smbus_read_i2c_block_data(st->client,
<<<<<<< HEAD
<<<<<<< HEAD
			cmd, ring->scan_count * 2, rxbuf);
=======
			cmd, bitmap_weight(indio_dev->active_scan_mask,
					   indio_dev->masklength) * 2, rxbuf);
>>>>>>> refs/remotes/origin/cm-10.0
	if (b_sent < 0)
		goto done;

	time_ns = iio_get_time_ns();

	if (ring->scan_timestamp)
		memcpy(rxbuf + st->d_size - sizeof(s64),
			&time_ns, sizeof(time_ns));

<<<<<<< HEAD
	ring->access->store_to(indio_dev->ring, rxbuf, time_ns);
=======
	ring->access->store_to(indio_dev->buffer, rxbuf, time_ns);
>>>>>>> refs/remotes/origin/cm-10.0
done:
	kfree(rxbuf);
	if (b_sent < 0)
		return b_sent;
=======
			cmd, st->transfer_size, st->rx_buf);
	if (b_sent < 0)
		goto out;

	iio_push_to_buffers_with_timestamp(indio_dev, st->rx_buf,
			iio_get_time_ns());
>>>>>>> refs/remotes/origin/master
out:
	iio_trigger_notify_done(indio_dev->trig);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
<<<<<<< HEAD
static const struct iio_ring_setup_ops ad799x_buf_setup_ops = {
	.preenable = &ad799x_ring_preenable,
	.postenable = &iio_triggered_ring_postenable,
	.predisable = &iio_triggered_ring_predisable,
=======
static const struct iio_buffer_setup_ops ad799x_buf_setup_ops = {
	.preenable = &ad799x_ring_preenable,
	.postenable = &iio_triggered_buffer_postenable,
	.predisable = &iio_triggered_buffer_predisable,
>>>>>>> refs/remotes/origin/cm-10.0
};

int ad799x_register_ring_funcs_and_init(struct iio_dev *indio_dev)
{
	int ret = 0;

<<<<<<< HEAD
	indio_dev->ring = iio_sw_rb_allocate(indio_dev);
	if (!indio_dev->ring) {
		ret = -ENOMEM;
		goto error_ret;
	}
	/* Effectively select the ring buffer implementation */
	indio_dev->ring->access = &ring_sw_access_funcs;
=======
	indio_dev->buffer = iio_sw_rb_allocate(indio_dev);
	if (!indio_dev->buffer) {
		ret = -ENOMEM;
		goto error_ret;
	}
>>>>>>> refs/remotes/origin/cm-10.0
	indio_dev->pollfunc = iio_alloc_pollfunc(NULL,
						 &ad799x_trigger_handler,
						 IRQF_ONESHOT,
						 indio_dev,
						 "%s_consumer%d",
						 indio_dev->name,
						 indio_dev->id);
	if (indio_dev->pollfunc == NULL) {
		ret = -ENOMEM;
		goto error_deallocate_sw_rb;
	}

	/* Ring buffer functions - here trigger setup related */
<<<<<<< HEAD
	indio_dev->ring->setup_ops = &ad799x_buf_setup_ops;
	indio_dev->ring->scan_timestamp = true;

	/* Flag that polled ring buffering is possible */
	indio_dev->modes |= INDIO_RING_TRIGGERED;
	return 0;

error_deallocate_sw_rb:
	iio_sw_rb_free(indio_dev->ring);
=======
	indio_dev->setup_ops = &ad799x_buf_setup_ops;
	indio_dev->buffer->scan_timestamp = true;

	/* Flag that polled ring buffering is possible */
	indio_dev->modes |= INDIO_BUFFER_TRIGGERED;
	return 0;

error_deallocate_sw_rb:
	iio_sw_rb_free(indio_dev->buffer);
>>>>>>> refs/remotes/origin/cm-10.0
error_ret:
	return ret;
=======
int ad799x_register_ring_funcs_and_init(struct iio_dev *indio_dev)
{
	return iio_triggered_buffer_setup(indio_dev, NULL,
		&ad799x_trigger_handler, NULL);
>>>>>>> refs/remotes/origin/master
}

void ad799x_ring_cleanup(struct iio_dev *indio_dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	/* ensure that the trigger has been detached */
	if (indio_dev->trig) {
		iio_put_trigger(indio_dev->trig);
		iio_trigger_dettach_poll_func(indio_dev->trig,
					      indio_dev->pollfunc);
	}
	iio_dealloc_pollfunc(indio_dev->pollfunc);
	iio_sw_rb_free(indio_dev->ring);
=======
	iio_dealloc_pollfunc(indio_dev->pollfunc);
	iio_sw_rb_free(indio_dev->buffer);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	iio_triggered_buffer_cleanup(indio_dev);
>>>>>>> refs/remotes/origin/master
}
