/*
 * ring_hw.h - common functionality for iio hardware ring buffers
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
<<<<<<< HEAD
 * Copyright (c) 2009 Jonathan Cameron <jic23@cam.ac.uk>
=======
 * Copyright (c) 2009 Jonathan Cameron <jic23@kernel.org>
>>>>>>> refs/remotes/origin/master
 *
 */

/**
 * struct iio_hw_ring_buffer- hardware ring buffer
 * @buf:	generic ring buffer elements
 * @private:	device specific data
 */
<<<<<<< HEAD
<<<<<<< HEAD
struct iio_hw_ring_buffer {
	struct iio_ring_buffer buf;
	void *private;
};

#define iio_to_hw_ring_buf(r) container_of(r, struct iio_hw_ring_buffer, buf)
=======
=======
>>>>>>> refs/remotes/origin/master
struct iio_hw_buffer {
	struct iio_buffer buf;
	void *private;
};

#define iio_to_hw_buf(r) container_of(r, struct iio_hw_buffer, buf)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
