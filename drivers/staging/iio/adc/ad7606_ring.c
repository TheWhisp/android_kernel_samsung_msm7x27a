/*
<<<<<<< HEAD
 * Copyright 2011 Analog Devices Inc.
=======
 * Copyright 2011-2012 Analog Devices Inc.
>>>>>>> refs/remotes/origin/master
 *
 * Licensed under the GPL-2.
 *
 */

#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/sysfs.h>

#include "../iio.h"
#include "../ring_generic.h"
#include "../ring_sw.h"
#include "../trigger.h"
#include "../sysfs.h"

#include "ad7606.h"

int ad7606_scan_from_ring(struct iio_dev *indio_dev, unsigned ch)
{
	struct iio_ring_buffer *ring = indio_dev->ring;
	int ret;
	u16 *ring_data;

	ring_data = kmalloc(ring->access->get_bytes_per_datum(ring),
			    GFP_KERNEL);
	if (ring_data == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}
	ret = ring->access->read_last(ring, (u8 *) ring_data);
	if (ret)
		goto error_free_ring_data;

	ret = ring_data[ch];

error_free_ring_data:
	kfree(ring_data);
error_ret:
	return ret;
}

/**
 * ad7606_ring_preenable() setup the parameters of the ring before enabling
 *
 * The complex nature of the setting of the nuber of bytes per datum is due
 * to this driver currently ensuring that the timestamp is stored at an 8
 * byte boundary.
 **/
static int ad7606_ring_preenable(struct iio_dev *indio_dev)
{
	struct ad7606_state *st = iio_priv(indio_dev);
	struct iio_ring_buffer *ring = indio_dev->ring;
	size_t d_size;

	d_size = st->chip_info->num_channels *
		 st->chip_info->channels[0].scan_type.storagebits / 8;

	if (ring->scan_timestamp) {
		d_size += sizeof(s64);

		if (d_size % sizeof(s64))
			d_size += sizeof(s64) - (d_size % sizeof(s64));
	}

	if (ring->access->set_bytes_per_datum)
		ring->access->set_bytes_per_datum(ring, d_size);

	st->d_size = d_size;

	return 0;
}

=======

#include "../iio.h"
#include "../buffer.h"
#include "../ring_sw.h"
#include "../trigger_consumer.h"

#include "ad7606.h"

>>>>>>> refs/remotes/origin/cm-10.0
=======

#include <linux/iio/iio.h>
#include <linux/iio/buffer.h>
#include <linux/iio/trigger_consumer.h>
#include <linux/iio/triggered_buffer.h>

#include "ad7606.h"

>>>>>>> refs/remotes/origin/master
/**
 * ad7606_trigger_handler_th() th/bh of trigger launched polling to ring buffer
 *
 **/
static irqreturn_t ad7606_trigger_handler_th_bh(int irq, void *p)
{
	struct iio_poll_func *pf = p;
<<<<<<< HEAD
<<<<<<< HEAD
	struct iio_dev *indio_dev = pf->private_data;
	struct ad7606_state *st = iio_priv(indio_dev);
=======
	struct ad7606_state *st = iio_priv(pf->indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ad7606_state *st = iio_priv(pf->indio_dev);
>>>>>>> refs/remotes/origin/master

	gpio_set_value(st->pdata->gpio_convst, 1);

	return IRQ_HANDLED;
}

/**
 * ad7606_poll_bh_to_ring() bh of trigger launched polling to ring buffer
 * @work_s:	the work struct through which this was scheduled
 *
 * Currently there is no option in this driver to disable the saving of
 * timestamps within the ring.
 * I think the one copy of this at a time was to avoid problems if the
 * trigger was set far too high and the reads then locked up the computer.
 **/
static void ad7606_poll_bh_to_ring(struct work_struct *work_s)
{
	struct ad7606_state *st = container_of(work_s, struct ad7606_state,
						poll_work);
	struct iio_dev *indio_dev = iio_priv_to_dev(st);
<<<<<<< HEAD
<<<<<<< HEAD
	struct iio_ring_buffer *ring = indio_dev->ring;
=======
	struct iio_buffer *ring = indio_dev->buffer;
>>>>>>> refs/remotes/origin/cm-10.0
	s64 time_ns;
	__u8 *buf;
	int ret;

<<<<<<< HEAD
	buf = kzalloc(st->d_size, GFP_KERNEL);
	if (buf == NULL)
		return;

	if (st->have_frstdata) {
=======
	buf = kzalloc(ring->access->get_bytes_per_datum(ring),
		      GFP_KERNEL);
=======
	__u8 *buf;
	int ret;

	buf = kzalloc(indio_dev->scan_bytes, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (buf == NULL)
		return;

	if (gpio_is_valid(st->pdata->gpio_frstdata)) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		ret = st->bops->read_block(st->dev, 1, buf);
		if (ret)
			goto done;
		if (!gpio_get_value(st->pdata->gpio_frstdata)) {
			/* This should never happen. However
			 * some signal glitch caused by bad PCB desgin or
			 * electrostatic discharge, could cause an extra read
			 * or clock. This allows recovery.
			 */
			ad7606_reset(st);
			goto done;
		}
		ret = st->bops->read_block(st->dev,
			st->chip_info->num_channels - 1, buf + 2);
		if (ret)
			goto done;
	} else {
		ret = st->bops->read_block(st->dev,
			st->chip_info->num_channels, buf);
		if (ret)
			goto done;
	}

<<<<<<< HEAD
	time_ns = iio_get_time_ns();

	if (ring->scan_timestamp)
<<<<<<< HEAD
		memcpy(buf + st->d_size - sizeof(s64),
			&time_ns, sizeof(time_ns));

	ring->access->store_to(indio_dev->ring, buf, time_ns);
=======
		*((s64 *)(buf + ring->access->get_bytes_per_datum(ring) -
			  sizeof(s64))) = time_ns;

	ring->access->store_to(indio_dev->buffer, buf, time_ns);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	iio_push_to_buffers_with_timestamp(indio_dev, buf, iio_get_time_ns());
>>>>>>> refs/remotes/origin/master
done:
	gpio_set_value(st->pdata->gpio_convst, 0);
	iio_trigger_notify_done(indio_dev->trig);
	kfree(buf);
}

<<<<<<< HEAD
<<<<<<< HEAD
static const struct iio_ring_setup_ops ad7606_ring_setup_ops = {
	.preenable = &ad7606_ring_preenable,
	.postenable = &iio_triggered_ring_postenable,
	.predisable = &iio_triggered_ring_predisable,
=======
static const struct iio_buffer_setup_ops ad7606_ring_setup_ops = {
	.preenable = &iio_sw_buffer_preenable,
	.postenable = &iio_triggered_buffer_postenable,
	.predisable = &iio_triggered_buffer_predisable,
>>>>>>> refs/remotes/origin/cm-10.0
};

int ad7606_register_ring_funcs_and_init(struct iio_dev *indio_dev)
{
	struct ad7606_state *st = iio_priv(indio_dev);
	int ret;

<<<<<<< HEAD
	indio_dev->ring = iio_sw_rb_allocate(indio_dev);
	if (!indio_dev->ring) {
=======
	indio_dev->buffer = iio_sw_rb_allocate(indio_dev);
	if (!indio_dev->buffer) {
>>>>>>> refs/remotes/origin/cm-10.0
		ret = -ENOMEM;
		goto error_ret;
	}

<<<<<<< HEAD
	/* Effectively select the ring buffer implementation */
	indio_dev->ring->access = &ring_sw_access_funcs;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	indio_dev->pollfunc = iio_alloc_pollfunc(&ad7606_trigger_handler_th_bh,
						 &ad7606_trigger_handler_th_bh,
						 0,
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
	indio_dev->ring->setup_ops = &ad7606_ring_setup_ops;
	indio_dev->ring->scan_timestamp = true ;
=======
	indio_dev->setup_ops = &ad7606_ring_setup_ops;
	indio_dev->buffer->scan_timestamp = true ;
>>>>>>> refs/remotes/origin/cm-10.0

	INIT_WORK(&st->poll_work, &ad7606_poll_bh_to_ring);

	/* Flag that polled ring buffering is possible */
<<<<<<< HEAD
	indio_dev->modes |= INDIO_RING_TRIGGERED;
	return 0;

error_deallocate_sw_rb:
	iio_sw_rb_free(indio_dev->ring);
=======
	indio_dev->modes |= INDIO_BUFFER_TRIGGERED;
	return 0;

error_deallocate_sw_rb:
	iio_sw_rb_free(indio_dev->buffer);
>>>>>>> refs/remotes/origin/cm-10.0
error_ret:
	return ret;
=======
int ad7606_register_ring_funcs_and_init(struct iio_dev *indio_dev)
{
	struct ad7606_state *st = iio_priv(indio_dev);

	INIT_WORK(&st->poll_work, &ad7606_poll_bh_to_ring);

	return iio_triggered_buffer_setup(indio_dev,
		&ad7606_trigger_handler_th_bh, &ad7606_trigger_handler_th_bh,
		NULL);
>>>>>>> refs/remotes/origin/master
}

void ad7606_ring_cleanup(struct iio_dev *indio_dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
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
