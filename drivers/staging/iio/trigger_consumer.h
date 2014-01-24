<<<<<<< HEAD

/* The industrial I/O core, trigger consumer handling functions
 *
 * Copyright (c) 2008 Jonathan Cameron
=======
/* The industrial I/O core, trigger consumer functions
 *
 * Copyright (c) 2008-2011 Jonathan Cameron
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

<<<<<<< HEAD
#ifdef CONFIG_IIO_TRIGGER
/**
 * iio_device_register_trigger_consumer() - set up an iio_dev to use triggers
 * @dev_info: iio_dev associated with the device that will consume the trigger
 **/
int iio_device_register_trigger_consumer(struct iio_dev *dev_info);

/**
 * iio_device_unregister_trigger_consumer() - reverse the registration process
 * @dev_info: iio_dev associated with the device that consumed the trigger
 **/
int iio_device_unregister_trigger_consumer(struct iio_dev *dev_info);

#else

/**
 * iio_device_register_trigger_consumer() - set up an iio_dev to use triggers
 * @dev_info: iio_dev associated with the device that will consume the trigger
 **/
static int iio_device_register_trigger_consumer(struct iio_dev *dev_info)
{
	return 0;
};

/**
 * iio_device_unregister_trigger_consumer() - reverse the registration process
 * @dev_info: iio_dev associated with the device that consumed the trigger
 **/
static int iio_device_unregister_trigger_consumer(struct iio_dev *dev_info)
{
	return 0;
};

#endif /* CONFIG_TRIGGER_CONSUMER */



=======
/**
 * struct iio_poll_func - poll function pair
 *
 * @indio_dev:			data specific to device (passed into poll func)
 * @h:				the function that is actually run on trigger
 * @thread:			threaded interrupt part
 * @type:			the type of interrupt (basically if oneshot)
 * @name:			name used to identify the trigger consumer.
 * @irq:			the corresponding irq as allocated from the
 *				trigger pool
 * @timestamp:			some devices need a timestamp grabbed as soon
 *				as possible after the trigger - hence handler
 *				passes it via here.
 **/
struct iio_poll_func {
	struct iio_dev *indio_dev;
	irqreturn_t (*h)(int irq, void *p);
	irqreturn_t (*thread)(int irq, void *p);
	int type;
	char *name;
	int irq;
	s64 timestamp;
};


struct iio_poll_func
*iio_alloc_pollfunc(irqreturn_t (*h)(int irq, void *p),
		    irqreturn_t (*thread)(int irq, void *p),
		    int type,
		    struct iio_dev *indio_dev,
		    const char *fmt,
		    ...);
void iio_dealloc_pollfunc(struct iio_poll_func *pf);
irqreturn_t iio_pollfunc_store_time(int irq, void *p);

void iio_trigger_notify_done(struct iio_trigger *trig);

/*
 * Two functions for common case where all that happens is a pollfunc
 * is attached and detached from a trigger
 */
int iio_triggered_buffer_postenable(struct iio_dev *indio_dev);
int iio_triggered_buffer_predisable(struct iio_dev *indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
