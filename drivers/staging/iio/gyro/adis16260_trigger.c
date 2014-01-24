#include <linux/interrupt.h>
<<<<<<< HEAD
#include <linux/irq.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/spi/spi.h>

#include "../iio.h"
#include "../sysfs.h"
=======
#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/export.h>

#include "../iio.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include "../trigger.h"
#include "adis16260.h"

/**
 * adis16260_data_rdy_trigger_set_state() set datardy interrupt state
 **/
static int adis16260_data_rdy_trigger_set_state(struct iio_trigger *trig,
						bool state)
{
<<<<<<< HEAD
	struct adis16260_state *st = trig->private_data;
	struct iio_dev *indio_dev = st->indio_dev;
=======
	struct iio_dev *indio_dev = trig->private_data;
>>>>>>> refs/remotes/origin/cm-10.0

	dev_dbg(&indio_dev->dev, "%s (%d)\n", __func__, state);
	return adis16260_set_irq(indio_dev, state);
}

<<<<<<< HEAD
int adis16260_probe_trigger(struct iio_dev *indio_dev)
{
	int ret;
	struct adis16260_state *st = indio_dev->dev_data;
=======
static const struct iio_trigger_ops adis16260_trigger_ops = {
	.owner = THIS_MODULE,
	.set_trigger_state = &adis16260_data_rdy_trigger_set_state,
};

int adis16260_probe_trigger(struct iio_dev *indio_dev)
{
	int ret;
	struct adis16260_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	st->trig = iio_allocate_trigger("%s-dev%d",
					spi_get_device_id(st->us)->name,
					indio_dev->id);
	if (st->trig == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}

	ret = request_irq(st->us->irq,
			  &iio_trigger_generic_data_rdy_poll,
			  IRQF_TRIGGER_RISING,
			  "adis16260",
			  st->trig);
	if (ret)
		goto error_free_trig;

	st->trig->dev.parent = &st->us->dev;
<<<<<<< HEAD
	st->trig->owner = THIS_MODULE;
	st->trig->private_data = st;
	st->trig->set_trigger_state = &adis16260_data_rdy_trigger_set_state;
=======
	st->trig->ops = &adis16260_trigger_ops;
	st->trig->private_data = indio_dev;
>>>>>>> refs/remotes/origin/cm-10.0
	ret = iio_trigger_register(st->trig);

	/* select default trigger */
	indio_dev->trig = st->trig;
	if (ret)
		goto error_free_irq;

	return 0;

error_free_irq:
	free_irq(st->us->irq, st->trig);
error_free_trig:
	iio_free_trigger(st->trig);
error_ret:
	return ret;
}

void adis16260_remove_trigger(struct iio_dev *indio_dev)
{
<<<<<<< HEAD
	struct adis16260_state *state = indio_dev->dev_data;

	iio_trigger_unregister(state->trig);
	free_irq(state->us->irq, state->trig);
	iio_free_trigger(state->trig);
=======
	struct adis16260_state *st = iio_priv(indio_dev);

	iio_trigger_unregister(st->trig);
	free_irq(st->us->irq, st->trig);
	iio_free_trigger(st->trig);
>>>>>>> refs/remotes/origin/cm-10.0
}
