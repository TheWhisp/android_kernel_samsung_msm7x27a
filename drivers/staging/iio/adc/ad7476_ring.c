/*
 * Copyright 2010 Analog Devices Inc.
 * Copyright (C) 2008 Jonathan Cameron
 *
 * Licensed under the GPL-2 or later.
 *
 * ad7476_ring.c
 */

#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/sysfs.h>
#include <linux/spi/spi.h>

#include "../iio.h"
#include "../ring_generic.h"
#include "../ring_sw.h"
#include "../trigger.h"
#include "../sysfs.h"

#include "ad7476.h"

int ad7476_scan_from_ring(struct ad7476_state *st)
{
	struct iio_ring_buffer *ring = st->indio_dev->ring;
	int ret;
	u8 *ring_data;

	ring_data = kmalloc(ring->access->get_bytes_per_datum(ring),
			    GFP_KERNEL);
	if (ring_data == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}
	ret = ring->access->read_last(ring, ring_data);
	if (ret)
		goto error_free_ring_data;

	ret = (ring_data[0] << 8) | ring_data[1];

error_free_ring_data:
	kfree(ring_data);
error_ret:
	return ret;
}

/**
 * ad7476_ring_preenable() setup the parameters of the ring before enabling
 *
 * The complex nature of the setting of the nuber of bytes per datum is due
=======
#include <linux/spi/spi.h>

#include "../iio.h"
#include "../buffer.h"
#include "../ring_sw.h"
#include "../trigger_consumer.h"

#include "ad7476.h"

/**
 * ad7476_ring_preenable() setup the parameters of the ring before enabling
 *
 * The complex nature of the setting of the number of bytes per datum is due
>>>>>>> refs/remotes/origin/cm-10.0
 * to this driver currently ensuring that the timestamp is stored at an 8
 * byte boundary.
 **/
static int ad7476_ring_preenable(struct iio_dev *indio_dev)
{
<<<<<<< HEAD
	struct ad7476_state *st = indio_dev->dev_data;
	struct iio_ring_buffer *ring = indio_dev->ring;

	st->d_size = ring->scan_count *
=======
	struct ad7476_state *st = iio_priv(indio_dev);
	struct iio_buffer *ring = indio_dev->buffer;

	st->d_size = bitmap_weight(indio_dev->active_scan_mask,
				   indio_dev->masklength) *
>>>>>>> refs/remotes/origin/cm-10.0
		st->chip_info->channel[0].scan_type.storagebits / 8;

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

static irqreturn_t ad7476_trigger_handler(int irq, void  *p)
{
	struct iio_poll_func *pf = p;
<<<<<<< HEAD
	struct iio_dev *indio_dev = pf->private_data;
	struct ad7476_state *st = iio_dev_get_devdata(indio_dev);
=======
	struct iio_dev *indio_dev = pf->indio_dev;
	struct ad7476_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	s64 time_ns;
	__u8 *rxbuf;
	int b_sent;

	rxbuf = kzalloc(st->d_size, GFP_KERNEL);
	if (rxbuf == NULL)
		return -ENOMEM;

	b_sent = spi_read(st->spi, rxbuf,
			  st->chip_info->channel[0].scan_type.storagebits / 8);
	if (b_sent < 0)
		goto done;

	time_ns = iio_get_time_ns();

<<<<<<< HEAD
	if (indio_dev->ring->scan_timestamp)
		memcpy(rxbuf + st->d_size - sizeof(s64),
			&time_ns, sizeof(time_ns));

	indio_dev->ring->access->store_to(indio_dev->ring, rxbuf, time_ns);
=======
	if (indio_dev->buffer->scan_timestamp)
		memcpy(rxbuf + st->d_size - sizeof(s64),
			&time_ns, sizeof(time_ns));

	indio_dev->buffer->access->store_to(indio_dev->buffer, rxbuf, time_ns);
>>>>>>> refs/remotes/origin/cm-10.0
done:
	iio_trigger_notify_done(indio_dev->trig);
	kfree(rxbuf);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static const struct iio_ring_setup_ops ad7476_ring_setup_ops = {
	.preenable = &ad7476_ring_preenable,
	.postenable = &iio_triggered_ring_postenable,
	.predisable = &iio_triggered_ring_predisable,
=======
static const struct iio_buffer_setup_ops ad7476_ring_setup_ops = {
	.preenable = &ad7476_ring_preenable,
	.postenable = &iio_triggered_buffer_postenable,
	.predisable = &iio_triggered_buffer_predisable,
>>>>>>> refs/remotes/origin/cm-10.0
};

int ad7476_register_ring_funcs_and_init(struct iio_dev *indio_dev)
{
<<<<<<< HEAD
	struct ad7476_state *st = indio_dev->dev_data;
	int ret = 0;

	indio_dev->ring = iio_sw_rb_allocate(indio_dev);
	if (!indio_dev->ring) {
		ret = -ENOMEM;
		goto error_ret;
	}
	/* Effectively select the ring buffer implementation */
	indio_dev->ring->access = &ring_sw_access_funcs;
=======
	struct ad7476_state *st = iio_priv(indio_dev);
	int ret = 0;

	indio_dev->buffer = iio_sw_rb_allocate(indio_dev);
	if (!indio_dev->buffer) {
		ret = -ENOMEM;
		goto error_ret;
	}
>>>>>>> refs/remotes/origin/cm-10.0
	indio_dev->pollfunc
		= iio_alloc_pollfunc(NULL,
				     &ad7476_trigger_handler,
				     IRQF_ONESHOT,
				     indio_dev,
				     "%s_consumer%d",
				     spi_get_device_id(st->spi)->name,
				     indio_dev->id);
	if (indio_dev->pollfunc == NULL) {
		ret = -ENOMEM;
		goto error_deallocate_sw_rb;
	}

	/* Ring buffer functions - here trigger setup related */
<<<<<<< HEAD
	indio_dev->ring->setup_ops = &ad7476_ring_setup_ops;
	indio_dev->ring->scan_timestamp = true;

	/* Flag that polled ring buffering is possible */
	indio_dev->modes |= INDIO_RING_TRIGGERED;
	return 0;

error_deallocate_sw_rb:
	iio_sw_rb_free(indio_dev->ring);
=======
	indio_dev->setup_ops = &ad7476_ring_setup_ops;
	indio_dev->buffer->scan_timestamp = true;

	/* Flag that polled ring buffering is possible */
	indio_dev->modes |= INDIO_BUFFER_TRIGGERED;
	return 0;

error_deallocate_sw_rb:
	iio_sw_rb_free(indio_dev->buffer);
>>>>>>> refs/remotes/origin/cm-10.0
error_ret:
	return ret;
}

void ad7476_ring_cleanup(struct iio_dev *indio_dev)
{
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
}
