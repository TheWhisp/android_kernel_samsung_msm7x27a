#include <linux/interrupt.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/sysfs.h>
#include <linux/slab.h>

#include "../iio.h"
#include "../sysfs.h"
#include "../ring_sw.h"
#include "../kfifo_buf.h"
#include "accel.h"
#include "../trigger.h"
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/gpio.h>
#include <linux/mutex.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>
#include <linux/export.h>

<<<<<<< HEAD
#include "../iio.h"
#include "../ring_sw.h"
#include "../kfifo_buf.h"
#include "../trigger.h"
#include "../trigger_consumer.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include "lis3l02dq.h"

/**
 * combine_8_to_16() utility function to munge to u8s into u16
=======
#include <linux/iio/iio.h>
#include <linux/iio/kfifo_buf.h>
#include <linux/iio/trigger.h>
#include <linux/iio/trigger_consumer.h>
#include "lis3l02dq.h"

/**
 * combine_8_to_16() utility function to munge two u8s into u16
>>>>>>> refs/remotes/origin/master
 **/
static inline u16 combine_8_to_16(u8 lower, u8 upper)
{
	u16 _lower = lower;
	u16 _upper = upper;
	return _lower | (_upper << 8);
}

/**
 * lis3l02dq_data_rdy_trig_poll() the event handler for the data rdy trig
 **/
irqreturn_t lis3l02dq_data_rdy_trig_poll(int irq, void *private)
{
	struct iio_dev *indio_dev = private;
	struct lis3l02dq_state *st = iio_priv(indio_dev);

	if (st->trigger_on) {
		iio_trigger_poll(st->trig, iio_get_time_ns());
		return IRQ_HANDLED;
	} else
		return IRQ_WAKE_THREAD;
}

<<<<<<< HEAD
<<<<<<< HEAD
/**
 * lis3l02dq_read_accel_from_ring() individual acceleration read from ring
 **/
ssize_t lis3l02dq_read_accel_from_ring(struct iio_ring_buffer *ring,
				       int index,
				       int *val)
{
	int ret;
	s16 *data;

	if (!iio_scan_mask_query(ring, index))
		return -EINVAL;

	if (!ring->access->read_last)
		return -EBUSY;

	data = kmalloc(ring->access->get_bytes_per_datum(ring),
		       GFP_KERNEL);
	if (data == NULL)
		return -ENOMEM;

	ret = ring->access->read_last(ring, (u8 *)data);
	if (ret)
		goto error_free_data;
	*val = data[bitmap_weight(&ring->scan_mask, index)];
error_free_data:
	kfree(data);

	return ret;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static const u8 read_all_tx_array[] = {
	LIS3L02DQ_READ_REG(LIS3L02DQ_REG_OUT_X_L_ADDR), 0,
	LIS3L02DQ_READ_REG(LIS3L02DQ_REG_OUT_X_H_ADDR), 0,
	LIS3L02DQ_READ_REG(LIS3L02DQ_REG_OUT_Y_L_ADDR), 0,
	LIS3L02DQ_READ_REG(LIS3L02DQ_REG_OUT_Y_H_ADDR), 0,
	LIS3L02DQ_READ_REG(LIS3L02DQ_REG_OUT_Z_L_ADDR), 0,
	LIS3L02DQ_READ_REG(LIS3L02DQ_REG_OUT_Z_H_ADDR), 0,
};

/**
 * lis3l02dq_read_all() Reads all channels currently selected
<<<<<<< HEAD
 * @st:		device specific state
=======
 * @indio_dev:	IIO device state
>>>>>>> refs/remotes/origin/master
 * @rx_array:	(dma capable) receive array, must be at least
 *		4*number of channels
 **/
static int lis3l02dq_read_all(struct iio_dev *indio_dev, u8 *rx_array)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct iio_ring_buffer *ring = indio_dev->ring;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct lis3l02dq_state *st = iio_priv(indio_dev);
	struct spi_transfer *xfers;
	struct spi_message msg;
	int ret, i, j = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	xfers = kzalloc((ring->scan_count) * 2
			* sizeof(*xfers), GFP_KERNEL);
=======
	xfers = kcalloc(bitmap_weight(indio_dev->active_scan_mask,
				      indio_dev->masklength) * 2,
			sizeof(*xfers), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	xfers = kcalloc(bitmap_weight(indio_dev->active_scan_mask,
				      indio_dev->masklength) * 2,
			sizeof(*xfers), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!xfers)
		return -ENOMEM;

	mutex_lock(&st->buf_lock);

	for (i = 0; i < ARRAY_SIZE(read_all_tx_array)/4; i++)
<<<<<<< HEAD
<<<<<<< HEAD
		if (ring->scan_mask & (1 << i)) {
=======
		if (test_bit(i, indio_dev->active_scan_mask)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (test_bit(i, indio_dev->active_scan_mask)) {
>>>>>>> refs/remotes/origin/master
			/* lower byte */
			xfers[j].tx_buf = st->tx + 2*j;
			st->tx[2*j] = read_all_tx_array[i*4];
			st->tx[2*j + 1] = 0;
			if (rx_array)
				xfers[j].rx_buf = rx_array + j*2;
			xfers[j].bits_per_word = 8;
			xfers[j].len = 2;
			xfers[j].cs_change = 1;
			j++;

			/* upper byte */
			xfers[j].tx_buf = st->tx + 2*j;
			st->tx[2*j] = read_all_tx_array[i*4 + 2];
			st->tx[2*j + 1] = 0;
			if (rx_array)
				xfers[j].rx_buf = rx_array + j*2;
			xfers[j].bits_per_word = 8;
			xfers[j].len = 2;
			xfers[j].cs_change = 1;
			j++;
		}

	/* After these are transmitted, the rx_buff should have
	 * values in alternate bytes
	 */
	spi_message_init(&msg);
<<<<<<< HEAD
<<<<<<< HEAD
	for (j = 0; j < ring->scan_count * 2; j++)
=======
	for (j = 0; j < bitmap_weight(indio_dev->active_scan_mask,
				      indio_dev->masklength) * 2; j++)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (j = 0; j < bitmap_weight(indio_dev->active_scan_mask,
				      indio_dev->masklength) * 2; j++)
>>>>>>> refs/remotes/origin/master
		spi_message_add_tail(&xfers[j], &msg);

	ret = spi_sync(st->us, &msg);
	mutex_unlock(&st->buf_lock);
	kfree(xfers);

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int lis3l02dq_get_ring_element(struct iio_dev *indio_dev,
=======
static int lis3l02dq_get_buffer_element(struct iio_dev *indio_dev,
>>>>>>> refs/remotes/origin/cm-10.0
				u8 *buf)
{
	int ret, i;
	u8 *rx_array ;
	s16 *data = (s16 *)buf;
<<<<<<< HEAD

	rx_array = kzalloc(4 * (indio_dev->ring->scan_count), GFP_KERNEL);
=======
=======
static int lis3l02dq_get_buffer_element(struct iio_dev *indio_dev,
				u8 *buf)
{
	int ret, i;
	u8 *rx_array;
	s16 *data = (s16 *)buf;
>>>>>>> refs/remotes/origin/master
	int scan_count = bitmap_weight(indio_dev->active_scan_mask,
				       indio_dev->masklength);

	rx_array = kzalloc(4 * scan_count, GFP_KERNEL);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (rx_array == NULL)
		return -ENOMEM;
	ret = lis3l02dq_read_all(indio_dev, rx_array);
	if (ret < 0)
		return ret;
<<<<<<< HEAD
	for (i = 0; i < indio_dev->ring->scan_count; i++)
=======
	for (i = 0; i < scan_count; i++)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (rx_array == NULL)
		return -ENOMEM;
	ret = lis3l02dq_read_all(indio_dev, rx_array);
	if (ret < 0) {
		kfree(rx_array);
		return ret;
	}
	for (i = 0; i < scan_count; i++)
>>>>>>> refs/remotes/origin/master
		data[i] = combine_8_to_16(rx_array[i*4+1],
					rx_array[i*4+3]);
	kfree(rx_array);

	return i*sizeof(data[0]);
}

static irqreturn_t lis3l02dq_trigger_handler(int irq, void *p)
{
	struct iio_poll_func *pf = p;
<<<<<<< HEAD
<<<<<<< HEAD
	struct iio_dev *indio_dev = pf->private_data;
	struct iio_ring_buffer *ring = indio_dev->ring;
	int len = 0;
	size_t datasize = ring->access->get_bytes_per_datum(ring);
=======
	struct iio_dev *indio_dev = pf->indio_dev;
	struct iio_buffer *buffer = indio_dev->buffer;
	int len = 0;
	size_t datasize = buffer->access->get_bytes_per_datum(buffer);
>>>>>>> refs/remotes/origin/cm-10.0
	char *data = kmalloc(datasize, GFP_KERNEL);

	if (data == NULL) {
		dev_err(indio_dev->dev.parent,
<<<<<<< HEAD
			"memory alloc failed in ring bh");
		return -ENOMEM;
	}

	if (ring->scan_count)
		len = lis3l02dq_get_ring_element(indio_dev, data);

	  /* Guaranteed to be aligned with 8 byte boundary */
	if (ring->scan_timestamp)
		*(s64 *)(((phys_addr_t)data + len
				+ sizeof(s64) - 1) & ~(sizeof(s64) - 1))
			= pf->timestamp;
	ring->access->store_to(ring, (u8 *)data, pf->timestamp);
=======
			"memory alloc failed in buffer bh");
		return -ENOMEM;
	}
=======
	struct iio_dev *indio_dev = pf->indio_dev;
	int len = 0;
	char *data;

	data = kmalloc(indio_dev->scan_bytes, GFP_KERNEL);
	if (data == NULL)
		goto done;
>>>>>>> refs/remotes/origin/master

	if (!bitmap_empty(indio_dev->active_scan_mask, indio_dev->masklength))
		len = lis3l02dq_get_buffer_element(indio_dev, data);

<<<<<<< HEAD
	  /* Guaranteed to be aligned with 8 byte boundary */
	if (buffer->scan_timestamp)
		*(s64 *)(((phys_addr_t)data + len
				+ sizeof(s64) - 1) & ~(sizeof(s64) - 1))
			= pf->timestamp;
	buffer->access->store_to(buffer, (u8 *)data, pf->timestamp);
>>>>>>> refs/remotes/origin/cm-10.0

	iio_trigger_notify_done(indio_dev->trig);
	kfree(data);
=======
	iio_push_to_buffers_with_timestamp(indio_dev, data, pf->timestamp);

	kfree(data);
done:
	iio_trigger_notify_done(indio_dev->trig);
>>>>>>> refs/remotes/origin/master
	return IRQ_HANDLED;
}

/* Caller responsible for locking as necessary. */
static int
<<<<<<< HEAD
__lis3l02dq_write_data_ready_config(struct device *dev, bool state)
=======
__lis3l02dq_write_data_ready_config(struct iio_dev *indio_dev, bool state)
>>>>>>> refs/remotes/origin/master
{
	int ret;
	u8 valold;
	bool currentlyset;
<<<<<<< HEAD
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct lis3l02dq_state *st = iio_priv(indio_dev);

/* Get the current event mask register */
=======
	struct lis3l02dq_state *st = iio_priv(indio_dev);

	/* Get the current event mask register */
>>>>>>> refs/remotes/origin/master
	ret = lis3l02dq_spi_read_reg_8(indio_dev,
				       LIS3L02DQ_REG_CTRL_2_ADDR,
				       &valold);
	if (ret)
		goto error_ret;
<<<<<<< HEAD
/* Find out if data ready is already on */
	currentlyset
		= valold & LIS3L02DQ_REG_CTRL_2_ENABLE_DATA_READY_GENERATION;

/* Disable requested */
	if (!state && currentlyset) {
		/* disable the data ready signal */
		valold &= ~LIS3L02DQ_REG_CTRL_2_ENABLE_DATA_READY_GENERATION;

		/* The double write is to overcome a hardware bug?*/
=======
	/* Find out if data ready is already on */
	currentlyset
		= valold & LIS3L02DQ_REG_CTRL_2_ENABLE_DATA_READY_GENERATION;

	/* Disable requested */
	if (!state && currentlyset) {
		/* Disable the data ready signal */
		valold &= ~LIS3L02DQ_REG_CTRL_2_ENABLE_DATA_READY_GENERATION;

		/* The double write is to overcome a hardware bug? */
>>>>>>> refs/remotes/origin/master
		ret = lis3l02dq_spi_write_reg_8(indio_dev,
						LIS3L02DQ_REG_CTRL_2_ADDR,
						valold);
		if (ret)
			goto error_ret;
		ret = lis3l02dq_spi_write_reg_8(indio_dev,
						LIS3L02DQ_REG_CTRL_2_ADDR,
						valold);
		if (ret)
			goto error_ret;
		st->trigger_on = false;
<<<<<<< HEAD
/* Enable requested */
	} else if (state && !currentlyset) {
		/* if not set, enable requested */
		/* first disable all events */
=======
	/* Enable requested */
	} else if (state && !currentlyset) {
		/* If not set, enable requested
		 * first disable all events */
>>>>>>> refs/remotes/origin/master
		ret = lis3l02dq_disable_all_events(indio_dev);
		if (ret < 0)
			goto error_ret;

		valold = ret |
			LIS3L02DQ_REG_CTRL_2_ENABLE_DATA_READY_GENERATION;

		st->trigger_on = true;
		ret = lis3l02dq_spi_write_reg_8(indio_dev,
						LIS3L02DQ_REG_CTRL_2_ADDR,
						valold);
		if (ret)
			goto error_ret;
	}

	return 0;
error_ret:
	return ret;
}

/**
 * lis3l02dq_data_rdy_trigger_set_state() set datardy interrupt state
 *
 * If disabling the interrupt also does a final read to ensure it is clear.
 * This is only important in some cases where the scan enable elements are
<<<<<<< HEAD
<<<<<<< HEAD
 * switched before the ring is reenabled.
=======
 * switched before the buffer is reenabled.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * switched before the buffer is reenabled.
>>>>>>> refs/remotes/origin/master
 **/
static int lis3l02dq_data_rdy_trigger_set_state(struct iio_trigger *trig,
						bool state)
{
<<<<<<< HEAD
	struct iio_dev *indio_dev = trig->private_data;
	int ret = 0;
	u8 t;

	__lis3l02dq_write_data_ready_config(&indio_dev->dev, state);
	if (state == false) {
		/*
<<<<<<< HEAD
		 * A possible quirk with teh handler is currently worked around
=======
		 * A possible quirk with the handler is currently worked around
>>>>>>> refs/remotes/origin/cm-10.0
		 *  by ensuring outstanding read events are cleared.
=======
	struct iio_dev *indio_dev = iio_trigger_get_drvdata(trig);
	int ret = 0;
	u8 t;

	__lis3l02dq_write_data_ready_config(indio_dev, state);
	if (!state) {
		/*
		 * A possible quirk with the handler is currently worked around
		 * by ensuring outstanding read events are cleared.
>>>>>>> refs/remotes/origin/master
		 */
		ret = lis3l02dq_read_all(indio_dev, NULL);
	}
	lis3l02dq_spi_read_reg_8(indio_dev,
				 LIS3L02DQ_REG_WAKE_UP_SRC_ADDR,
				 &t);
	return ret;
}

/**
<<<<<<< HEAD
 * lis3l02dq_trig_try_reen() try renabling irq for data rdy trigger
=======
 * lis3l02dq_trig_try_reen() try reenabling irq for data rdy trigger
>>>>>>> refs/remotes/origin/master
 * @trig:	the datardy trigger
 */
static int lis3l02dq_trig_try_reen(struct iio_trigger *trig)
{
<<<<<<< HEAD
	struct iio_dev *indio_dev = trig->private_data;
	struct lis3l02dq_state *st = iio_priv(indio_dev);
	int i;

	/* If gpio still high (or high again) */
	/* In theory possible we will need to do this several times */
	for (i = 0; i < 5; i++)
		if (gpio_get_value(irq_to_gpio(st->us->irq)))
=======
	struct iio_dev *indio_dev = iio_trigger_get_drvdata(trig);
	struct lis3l02dq_state *st = iio_priv(indio_dev);
	int i;

	/* If gpio still high (or high again)
	 * In theory possible we will need to do this several times */
	for (i = 0; i < 5; i++)
		if (gpio_get_value(st->gpio))
>>>>>>> refs/remotes/origin/master
			lis3l02dq_read_all(indio_dev, NULL);
		else
			break;
	if (i == 5)
<<<<<<< HEAD
		printk(KERN_INFO
		       "Failed to clear the interrupt for lis3l02dq\n");
=======
		pr_info("Failed to clear the interrupt for lis3l02dq\n");
>>>>>>> refs/remotes/origin/master

	/* irq reenabled so success! */
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static const struct iio_trigger_ops lis3l02dq_trigger_ops = {
	.owner = THIS_MODULE,
	.set_trigger_state = &lis3l02dq_data_rdy_trigger_set_state,
	.try_reenable = &lis3l02dq_trig_try_reen,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
int lis3l02dq_probe_trigger(struct iio_dev *indio_dev)
{
	int ret;
	struct lis3l02dq_state *st = iio_priv(indio_dev);

<<<<<<< HEAD
	st->trig = iio_allocate_trigger("lis3l02dq-dev%d", indio_dev->id);
=======
	st->trig = iio_trigger_alloc("lis3l02dq-dev%d", indio_dev->id);
>>>>>>> refs/remotes/origin/master
	if (!st->trig) {
		ret = -ENOMEM;
		goto error_ret;
	}

	st->trig->dev.parent = &st->us->dev;
<<<<<<< HEAD
<<<<<<< HEAD
	st->trig->owner = THIS_MODULE;
	st->trig->private_data = indio_dev;
	st->trig->set_trigger_state = &lis3l02dq_data_rdy_trigger_set_state;
	st->trig->try_reenable = &lis3l02dq_trig_try_reen;
=======
	st->trig->ops = &lis3l02dq_trigger_ops;
	st->trig->private_data = indio_dev;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	st->trig->ops = &lis3l02dq_trigger_ops;
	iio_trigger_set_drvdata(st->trig, indio_dev);
>>>>>>> refs/remotes/origin/master
	ret = iio_trigger_register(st->trig);
	if (ret)
		goto error_free_trig;

	return 0;

error_free_trig:
<<<<<<< HEAD
	iio_free_trigger(st->trig);
=======
	iio_trigger_free(st->trig);
>>>>>>> refs/remotes/origin/master
error_ret:
	return ret;
}

void lis3l02dq_remove_trigger(struct iio_dev *indio_dev)
{
	struct lis3l02dq_state *st = iio_priv(indio_dev);

	iio_trigger_unregister(st->trig);
<<<<<<< HEAD
	iio_free_trigger(st->trig);
}

<<<<<<< HEAD
void lis3l02dq_unconfigure_ring(struct iio_dev *indio_dev)
{
	iio_dealloc_pollfunc(indio_dev->pollfunc);
	lis3l02dq_free_buf(indio_dev->ring);
}

static int lis3l02dq_ring_postenable(struct iio_dev *indio_dev)
=======
void lis3l02dq_unconfigure_buffer(struct iio_dev *indio_dev)
{
	iio_dealloc_pollfunc(indio_dev->pollfunc);
	lis3l02dq_free_buf(indio_dev->buffer);
}

static int lis3l02dq_buffer_postenable(struct iio_dev *indio_dev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	iio_trigger_free(st->trig);
}

void lis3l02dq_unconfigure_buffer(struct iio_dev *indio_dev)
{
	iio_dealloc_pollfunc(indio_dev->pollfunc);
	iio_kfifo_free(indio_dev->buffer);
}

static int lis3l02dq_buffer_postenable(struct iio_dev *indio_dev)
>>>>>>> refs/remotes/origin/master
{
	/* Disable unwanted channels otherwise the interrupt will not clear */
	u8 t;
	int ret;
	bool oneenabled = false;

	ret = lis3l02dq_spi_read_reg_8(indio_dev,
				       LIS3L02DQ_REG_CTRL_1_ADDR,
				       &t);
	if (ret)
		goto error_ret;

<<<<<<< HEAD
<<<<<<< HEAD
	if (iio_scan_mask_query(indio_dev->ring, 0)) {
=======
	if (test_bit(0, indio_dev->active_scan_mask)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (test_bit(0, indio_dev->active_scan_mask)) {
>>>>>>> refs/remotes/origin/master
		t |= LIS3L02DQ_REG_CTRL_1_AXES_X_ENABLE;
		oneenabled = true;
	} else
		t &= ~LIS3L02DQ_REG_CTRL_1_AXES_X_ENABLE;
<<<<<<< HEAD
<<<<<<< HEAD
	if (iio_scan_mask_query(indio_dev->ring, 1)) {
=======
	if (test_bit(1, indio_dev->active_scan_mask)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (test_bit(1, indio_dev->active_scan_mask)) {
>>>>>>> refs/remotes/origin/master
		t |= LIS3L02DQ_REG_CTRL_1_AXES_Y_ENABLE;
		oneenabled = true;
	} else
		t &= ~LIS3L02DQ_REG_CTRL_1_AXES_Y_ENABLE;
<<<<<<< HEAD
<<<<<<< HEAD
	if (iio_scan_mask_query(indio_dev->ring, 2)) {
=======
	if (test_bit(2, indio_dev->active_scan_mask)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (test_bit(2, indio_dev->active_scan_mask)) {
>>>>>>> refs/remotes/origin/master
		t |= LIS3L02DQ_REG_CTRL_1_AXES_Z_ENABLE;
		oneenabled = true;
	} else
		t &= ~LIS3L02DQ_REG_CTRL_1_AXES_Z_ENABLE;

	if (!oneenabled) /* what happens in this case is unknown */
		return -EINVAL;
	ret = lis3l02dq_spi_write_reg_8(indio_dev,
					LIS3L02DQ_REG_CTRL_1_ADDR,
					t);
	if (ret)
		goto error_ret;

<<<<<<< HEAD
<<<<<<< HEAD
	return iio_triggered_ring_postenable(indio_dev);
=======
	return iio_triggered_buffer_postenable(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return iio_triggered_buffer_postenable(indio_dev);
>>>>>>> refs/remotes/origin/master
error_ret:
	return ret;
}

/* Turn all channels on again */
<<<<<<< HEAD
<<<<<<< HEAD
static int lis3l02dq_ring_predisable(struct iio_dev *indio_dev)
=======
static int lis3l02dq_buffer_predisable(struct iio_dev *indio_dev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int lis3l02dq_buffer_predisable(struct iio_dev *indio_dev)
>>>>>>> refs/remotes/origin/master
{
	u8 t;
	int ret;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = iio_triggered_ring_predisable(indio_dev);
=======
	ret = iio_triggered_buffer_predisable(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = iio_triggered_buffer_predisable(indio_dev);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto error_ret;

	ret = lis3l02dq_spi_read_reg_8(indio_dev,
				       LIS3L02DQ_REG_CTRL_1_ADDR,
				       &t);
	if (ret)
		goto error_ret;
	t |= LIS3L02DQ_REG_CTRL_1_AXES_X_ENABLE |
		LIS3L02DQ_REG_CTRL_1_AXES_Y_ENABLE |
		LIS3L02DQ_REG_CTRL_1_AXES_Z_ENABLE;

	ret = lis3l02dq_spi_write_reg_8(indio_dev,
					LIS3L02DQ_REG_CTRL_1_ADDR,
					t);

error_ret:
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static const struct iio_ring_setup_ops lis3l02dq_ring_setup_ops = {
	.preenable = &iio_sw_ring_preenable,
	.postenable = &lis3l02dq_ring_postenable,
	.predisable = &lis3l02dq_ring_predisable,
};

int lis3l02dq_configure_ring(struct iio_dev *indio_dev)
{
	int ret;
	struct iio_ring_buffer *ring;

	ring = lis3l02dq_alloc_buf(indio_dev);
	if (!ring)
		return -ENOMEM;

	indio_dev->ring = ring;
	/* Effectively select the ring buffer implementation */
	indio_dev->ring->access = &lis3l02dq_access_funcs;
	ring->bpe = 2;

	ring->scan_timestamp = true;
	ring->setup_ops = &lis3l02dq_ring_setup_ops;
	ring->owner = THIS_MODULE;

	/* Set default scan mode */
	iio_scan_mask_set(ring, 0);
	iio_scan_mask_set(ring, 1);
	iio_scan_mask_set(ring, 2);
=======
static const struct iio_buffer_setup_ops lis3l02dq_buffer_setup_ops = {
	.preenable = &iio_sw_buffer_preenable,
=======
static const struct iio_buffer_setup_ops lis3l02dq_buffer_setup_ops = {
>>>>>>> refs/remotes/origin/master
	.postenable = &lis3l02dq_buffer_postenable,
	.predisable = &lis3l02dq_buffer_predisable,
};

int lis3l02dq_configure_buffer(struct iio_dev *indio_dev)
{
	int ret;
	struct iio_buffer *buffer;

<<<<<<< HEAD
	buffer = lis3l02dq_alloc_buf(indio_dev);
	if (!buffer)
		return -ENOMEM;

	indio_dev->buffer = buffer;

	buffer->scan_timestamp = true;
	indio_dev->setup_ops = &lis3l02dq_buffer_setup_ops;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	buffer = iio_kfifo_allocate(indio_dev);
	if (!buffer)
		return -ENOMEM;

	iio_device_attach_buffer(indio_dev, buffer);

	buffer->scan_timestamp = true;
	indio_dev->setup_ops = &lis3l02dq_buffer_setup_ops;
>>>>>>> refs/remotes/origin/master

	/* Functions are NULL as we set handler below */
	indio_dev->pollfunc = iio_alloc_pollfunc(&iio_pollfunc_store_time,
						 &lis3l02dq_trigger_handler,
						 0,
						 indio_dev,
						 "lis3l02dq_consumer%d",
						 indio_dev->id);

	if (indio_dev->pollfunc == NULL) {
		ret = -ENOMEM;
		goto error_iio_sw_rb_free;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	indio_dev->modes |= INDIO_RING_TRIGGERED;
	return 0;

error_iio_sw_rb_free:
	lis3l02dq_free_buf(indio_dev->ring);
=======
=======
>>>>>>> refs/remotes/origin/master
	indio_dev->modes |= INDIO_BUFFER_TRIGGERED;
	return 0;

error_iio_sw_rb_free:
<<<<<<< HEAD
	lis3l02dq_free_buf(indio_dev->buffer);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	iio_kfifo_free(indio_dev->buffer);
>>>>>>> refs/remotes/origin/master
	return ret;
}
