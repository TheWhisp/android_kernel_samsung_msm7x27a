#
# Makefile for the industrial I/O core.
#

<<<<<<< HEAD
obj-$(CONFIG_IIO) += industrialio.o
<<<<<<< HEAD
industrialio-y := industrialio-core.o
industrialio-$(CONFIG_IIO_RING_BUFFER) += industrialio-ring.o
=======
industrialio-y := industrialio-core.o industrialio-event.o inkern.o
industrialio-$(CONFIG_IIO_BUFFER) += industrialio-buffer.o
>>>>>>> refs/remotes/origin/cm-10.0
industrialio-$(CONFIG_IIO_TRIGGER) += industrialio-trigger.o

obj-$(CONFIG_IIO_SW_RING) += ring_sw.o
obj-$(CONFIG_IIO_KFIFO_BUF) += kfifo_buf.o

<<<<<<< HEAD
obj-y += accel/
obj-y += adc/
obj-y += addac/
obj-y += dac/
obj-y += dds/
obj-y += gyro/
=======
=======
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_IIO_SIMPLE_DUMMY) += iio_dummy.o
iio_dummy-y := iio_simple_dummy.o
iio_dummy-$(CONFIG_IIO_SIMPLE_DUMMY_EVENTS) += iio_simple_dummy_events.o
iio_dummy-$(CONFIG_IIO_SIMPLE_DUMMY_BUFFER) += iio_simple_dummy_buffer.o

obj-$(CONFIG_IIO_DUMMY_EVGEN) += iio_dummy_evgen.o

<<<<<<< HEAD
obj-$(CONFIG_IIO_ST_HWMON) += iio_hwmon.o

=======
>>>>>>> refs/remotes/origin/master
obj-y += accel/
obj-y += adc/
obj-y += addac/
obj-y += cdc/
<<<<<<< HEAD
obj-y += dac/
obj-y += dds/
obj-y += gyro/
obj-y += impedance-analyzer/
>>>>>>> refs/remotes/origin/cm-10.0
obj-y += imu/
=======
obj-y += frequency/
obj-y += gyro/
obj-y += impedance-analyzer/
>>>>>>> refs/remotes/origin/master
obj-y += light/
obj-y += magnetometer/
obj-y += meter/
obj-y += resolver/
obj-y += trigger/
