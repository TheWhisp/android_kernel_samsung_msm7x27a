/*
    comedi/drivers/serial2002.c
    Skeleton code for a Comedi driver

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 2002 Anders Blomdell <anders.blomdell@control.lth.se>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
<<<<<<< HEAD

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

=======
>>>>>>> refs/remotes/origin/master
*/

/*
Driver: serial2002
Description: Driver for serial connected hardware
Devices:
Author: Anders Blomdell
Updated: Fri,  7 Jun 2002 12:56:45 -0700
Status: in development

*/

<<<<<<< HEAD
#include "../comedidev.h"

#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/sched.h>
#include <linux/slab.h>

<<<<<<< HEAD
#include <asm/termios.h>
=======
#include <linux/termios.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
#include "../comedidev.h"

#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include <linux/termios.h>
>>>>>>> refs/remotes/origin/master
#include <asm/ioctls.h>
#include <linux/serial.h>
#include <linux/poll.h>

<<<<<<< HEAD
/*
 * Board descriptions for two imaginary boards.  Describing the
 * boards in this way is optional, and completely driver-dependent.
 * Some drivers use arrays such as this, other do not.
 */
struct serial2002_board {
	const char *name;
};

static const struct serial2002_board serial2002_boards[] = {
	{
	 .name = "serial2002"}
};

/*
 * Useful for shorthand access to the particular board structure
 */
#define thisboard ((const struct serial2002_board *)dev->board_ptr)

=======
>>>>>>> refs/remotes/origin/master
struct serial2002_range_table_t {

	/*  HACK... */
	int length;
	struct comedi_krange range;
};

struct serial2002_private {

	int port;		/*  /dev/ttyS<port> */
	int speed;		/*  baudrate */
	struct file *tty;
	unsigned int ao_readback[32];
	unsigned char digital_in_mapping[32];
	unsigned char digital_out_mapping[32];
	unsigned char analog_in_mapping[32];
	unsigned char analog_out_mapping[32];
	unsigned char encoder_in_mapping[32];
	struct serial2002_range_table_t in_range[32], out_range[32];
};

<<<<<<< HEAD
/*
 * most drivers define the following macro to make it easy to
 * access the private structure.
 */
#define devpriv ((struct serial2002_private *)dev->private)

static int serial2002_attach(struct comedi_device *dev,
			     struct comedi_devconfig *it);
static int serial2002_detach(struct comedi_device *dev);
struct comedi_driver driver_serial2002 = {
	.driver_name = "serial2002",
	.module = THIS_MODULE,
	.attach = serial2002_attach,
	.detach = serial2002_detach,
	.board_name = &serial2002_boards[0].name,
	.offset = sizeof(struct serial2002_board),
	.num_names = ARRAY_SIZE(serial2002_boards),
};

static int serial2002_di_rinsn(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data);
static int serial2002_do_winsn(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data);
static int serial2002_ai_rinsn(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data);
static int serial2002_ao_winsn(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data);
static int serial2002_ao_rinsn(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data);

=======
>>>>>>> refs/remotes/origin/master
struct serial_data {
	enum { is_invalid, is_digital, is_channel } kind;
	int index;
	unsigned long value;
};

<<<<<<< HEAD
static long tty_ioctl(struct file *f, unsigned op, unsigned long param)
=======
/*
 * The configuration serial_data.value read from the device is
 * a bitmask that defines specific options of a channel:
 *
 * 4:0 - the channel to configure
 * 7:5 - the kind of channel
 * 9:8 - the command used to configure the channel
 *
 * The remaining bits vary in use depending on the command:
 *
 * BITS     15:10 - the channel bits (maxdata)
 * MIN/MAX  12:10 - the units multiplier for the scale
 *          13    - the sign of the scale
 *          33:14 - the base value for the range
 */
#define S2002_CFG_CHAN(x)		((x) & 0x1f)
#define S2002_CFG_KIND(x)		(((x) >> 5) & 0x7)
#define S2002_CFG_KIND_INVALID		0
#define S2002_CFG_KIND_DIGITAL_IN	1
#define S2002_CFG_KIND_DIGITAL_OUT	2
#define S2002_CFG_KIND_ANALOG_IN	3
#define S2002_CFG_KIND_ANALOG_OUT	4
#define S2002_CFG_KIND_ENCODER_IN	5
#define S2002_CFG_CMD(x)		(((x) >> 8) & 0x3)
#define S2002_CFG_CMD_BITS		0
#define S2002_CFG_CMD_MIN		1
#define S2002_CFG_CMD_MAX		2
#define S2002_CFG_BITS(x)		(((x) >> 10) & 0x3f)
#define S2002_CFG_UNITS(x)		(((x) >> 10) & 0x7)
#define S2002_CFG_SIGN(x)		(((x) >> 13) & 0x1)
#define S2002_CFG_BASE(x)		(((x) >> 14) & 0xfffff)

static long serial2002_tty_ioctl(struct file *f, unsigned op,
				 unsigned long param)
>>>>>>> refs/remotes/origin/master
{
	if (f->f_op->unlocked_ioctl)
		return f->f_op->unlocked_ioctl(f, op, param);

	return -ENOSYS;
}

<<<<<<< HEAD
static int tty_write(struct file *f, unsigned char *buf, int count)
{
=======
static int serial2002_tty_write(struct file *f, unsigned char *buf, int count)
{
	const char __user *p = (__force const char __user *)buf;
>>>>>>> refs/remotes/origin/master
	int result;
	mm_segment_t oldfs;

	oldfs = get_fs();
	set_fs(KERNEL_DS);
	f->f_pos = 0;
<<<<<<< HEAD
	result = f->f_op->write(f, buf, count, &f->f_pos);
=======
	result = f->f_op->write(f, p, count, &f->f_pos);
>>>>>>> refs/remotes/origin/master
	set_fs(oldfs);
	return result;
}

<<<<<<< HEAD
#if 0
/*
 * On 2.6.26.3 this occaisonally gave me page faults, worked around by
 * settings.c_cc[VMIN] = 0; settings.c_cc[VTIME] = 0
 */
static int tty_available(struct file *f)
{
	long result = 0;
	mm_segment_t oldfs;

	oldfs = get_fs();
	set_fs(KERNEL_DS);
	tty_ioctl(f, FIONREAD, (unsigned long)&result);
	set_fs(oldfs);
	return result;
}
#endif

static int tty_read(struct file *f, int timeout)
{
=======
static int serial2002_tty_readb(struct file *f, unsigned char *buf)
{
	char __user *p = (__force char __user *)buf;

	f->f_pos = 0;
	return f->f_op->read(f, p, 1, &f->f_pos);
}

static void serial2002_tty_read_poll_wait(struct file *f, int timeout)
{
	struct poll_wqueues table;
	struct timeval start, now;

	do_gettimeofday(&start);
	poll_initwait(&table);
	while (1) {
		long elapsed;
		int mask;

		mask = f->f_op->poll(f, &table.pt);
		if (mask & (POLLRDNORM | POLLRDBAND | POLLIN |
			    POLLHUP | POLLERR)) {
			break;
		}
		do_gettimeofday(&now);
		elapsed = (1000000 * (now.tv_sec - start.tv_sec) +
			  now.tv_usec - start.tv_usec);
		if (elapsed > timeout)
			break;
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(((timeout - elapsed) * HZ) / 10000);
	}
	poll_freewait(&table);
}

static int serial2002_tty_read(struct file *f, int timeout)
{
	unsigned char ch;
>>>>>>> refs/remotes/origin/master
	int result;

	result = -1;
	if (!IS_ERR(f)) {
		mm_segment_t oldfs;

		oldfs = get_fs();
		set_fs(KERNEL_DS);
		if (f->f_op->poll) {
<<<<<<< HEAD
			struct poll_wqueues table;
			struct timeval start, now;

			do_gettimeofday(&start);
			poll_initwait(&table);
			while (1) {
				long elapsed;
				int mask;

				mask = f->f_op->poll(f, &table.pt);
				if (mask & (POLLRDNORM | POLLRDBAND | POLLIN |
					    POLLHUP | POLLERR)) {
					break;
				}
				do_gettimeofday(&now);
				elapsed =
				    (1000000 * (now.tv_sec - start.tv_sec) +
				     now.tv_usec - start.tv_usec);
<<<<<<< HEAD
				if (elapsed > timeout) {
					break;
				}
=======
				if (elapsed > timeout)
					break;
>>>>>>> refs/remotes/origin/cm-10.0
				set_current_state(TASK_INTERRUPTIBLE);
				schedule_timeout(((timeout -
						   elapsed) * HZ) / 10000);
			}
			poll_freewait(&table);
			{
				unsigned char ch;

				f->f_pos = 0;
<<<<<<< HEAD
				if (f->f_op->read(f, &ch, 1, &f->f_pos) == 1) {
					result = ch;
				}
=======
				if (f->f_op->read(f, &ch, 1, &f->f_pos) == 1)
					result = ch;
>>>>>>> refs/remotes/origin/cm-10.0
			}
=======
			serial2002_tty_read_poll_wait(f, timeout);

			if (serial2002_tty_readb(f, &ch) == 1)
				result = ch;
>>>>>>> refs/remotes/origin/master
		} else {
			/* Device does not support poll, busy wait */
			int retries = 0;
			while (1) {
<<<<<<< HEAD
				unsigned char ch;

				retries++;
<<<<<<< HEAD
				if (retries >= timeout) {
					break;
				}
=======
				if (retries >= timeout)
					break;
>>>>>>> refs/remotes/origin/cm-10.0

				f->f_pos = 0;
				if (f->f_op->read(f, &ch, 1, &f->f_pos) == 1) {
=======
				retries++;
				if (retries >= timeout)
					break;

				if (serial2002_tty_readb(f, &ch) == 1) {
>>>>>>> refs/remotes/origin/master
					result = ch;
					break;
				}
				udelay(100);
			}
		}
		set_fs(oldfs);
	}
	return result;
}

<<<<<<< HEAD
static void tty_setspeed(struct file *f, int speed)
{
=======
static void serial2002_tty_setspeed(struct file *f, int speed)
{
	struct termios termios;
	struct serial_struct serial;
>>>>>>> refs/remotes/origin/master
	mm_segment_t oldfs;

	oldfs = get_fs();
	set_fs(KERNEL_DS);
<<<<<<< HEAD
	{
		/*  Set speed */
		struct termios settings;

		tty_ioctl(f, TCGETS, (unsigned long)&settings);
/* printk("Speed: %d\n", settings.c_cflag & (CBAUD | CBAUDEX)); */
		settings.c_iflag = 0;
		settings.c_oflag = 0;
		settings.c_lflag = 0;
		settings.c_cflag = CLOCAL | CS8 | CREAD;
		settings.c_cc[VMIN] = 0;
		settings.c_cc[VTIME] = 0;
		switch (speed) {
		case 2400:{
				settings.c_cflag |= B2400;
			}
			break;
		case 4800:{
				settings.c_cflag |= B4800;
			}
			break;
		case 9600:{
				settings.c_cflag |= B9600;
			}
			break;
		case 19200:{
				settings.c_cflag |= B19200;
			}
			break;
		case 38400:{
				settings.c_cflag |= B38400;
			}
			break;
		case 57600:{
				settings.c_cflag |= B57600;
			}
			break;
		case 115200:{
				settings.c_cflag |= B115200;
			}
			break;
		default:{
				settings.c_cflag |= B9600;
			}
			break;
		}
		tty_ioctl(f, TCSETS, (unsigned long)&settings);
/* printk("Speed: %d\n", settings.c_cflag & (CBAUD | CBAUDEX)); */
	}
	{
		/*  Set low latency */
		struct serial_struct settings;

		tty_ioctl(f, TIOCGSERIAL, (unsigned long)&settings);
		settings.flags |= ASYNC_LOW_LATENCY;
		tty_ioctl(f, TIOCSSERIAL, (unsigned long)&settings);
	}
=======

	/* Set speed */
	serial2002_tty_ioctl(f, TCGETS, (unsigned long)&termios);
	termios.c_iflag = 0;
	termios.c_oflag = 0;
	termios.c_lflag = 0;
	termios.c_cflag = CLOCAL | CS8 | CREAD;
	termios.c_cc[VMIN] = 0;
	termios.c_cc[VTIME] = 0;
	switch (speed) {
	case 2400:
		termios.c_cflag |= B2400;
		break;
	case 4800:
		termios.c_cflag |= B4800;
		break;
	case 9600:
		termios.c_cflag |= B9600;
		break;
	case 19200:
		termios.c_cflag |= B19200;
		break;
	case 38400:
		termios.c_cflag |= B38400;
		break;
	case 57600:
		termios.c_cflag |= B57600;
		break;
	case 115200:
		termios.c_cflag |= B115200;
		break;
	default:
		termios.c_cflag |= B9600;
		break;
	}
	serial2002_tty_ioctl(f, TCSETS, (unsigned long)&termios);

	/* Set low latency */
	serial2002_tty_ioctl(f, TIOCGSERIAL, (unsigned long)&serial);
	serial.flags |= ASYNC_LOW_LATENCY;
	serial2002_tty_ioctl(f, TIOCSSERIAL, (unsigned long)&serial);
>>>>>>> refs/remotes/origin/master

	set_fs(oldfs);
}

<<<<<<< HEAD
static void poll_digital(struct file *f, int channel)
=======
static void serial2002_poll_digital(struct file *f, int channel)
>>>>>>> refs/remotes/origin/master
{
	char cmd;

	cmd = 0x40 | (channel & 0x1f);
<<<<<<< HEAD
	tty_write(f, &cmd, 1);
}

static void poll_channel(struct file *f, int channel)
=======
	serial2002_tty_write(f, &cmd, 1);
}

static void serial2002_poll_channel(struct file *f, int channel)
>>>>>>> refs/remotes/origin/master
{
	char cmd;

	cmd = 0x60 | (channel & 0x1f);
<<<<<<< HEAD
	tty_write(f, &cmd, 1);
}

static struct serial_data serial_read(struct file *f, int timeout)
=======
	serial2002_tty_write(f, &cmd, 1);
}

static struct serial_data serial2002_read(struct file *f, int timeout)
>>>>>>> refs/remotes/origin/master
{
	struct serial_data result;
	int length;

	result.kind = is_invalid;
	result.index = 0;
	result.value = 0;
	length = 0;
	while (1) {
<<<<<<< HEAD
		int data = tty_read(f, timeout);

		length++;
		if (data < 0) {
<<<<<<< HEAD
			printk("serial2002 error\n");
=======
			printk(KERN_ERR "serial2002 error\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		int data = serial2002_tty_read(f, timeout);

		length++;
		if (data < 0) {
>>>>>>> refs/remotes/origin/master
			break;
		} else if (data & 0x80) {
			result.value = (result.value << 7) | (data & 0x7f);
		} else {
			if (length == 1) {
				switch ((data >> 5) & 0x03) {
<<<<<<< HEAD
				case 0:{
						result.value = 0;
						result.kind = is_digital;
					}
					break;
				case 1:{
						result.value = 1;
						result.kind = is_digital;
					}
=======
				case 0:
					result.value = 0;
					result.kind = is_digital;
					break;
				case 1:
					result.value = 1;
					result.kind = is_digital;
>>>>>>> refs/remotes/origin/master
					break;
				}
			} else {
				result.value =
				    (result.value << 2) | ((data & 0x60) >> 5);
				result.kind = is_channel;
			}
			result.index = data & 0x1f;
			break;
		}
	}
	return result;

}

<<<<<<< HEAD
static void serial_write(struct file *f, struct serial_data data)
=======
static void serial2002_write(struct file *f, struct serial_data data)
>>>>>>> refs/remotes/origin/master
{
	if (data.kind == is_digital) {
		unsigned char ch =
		    ((data.value << 5) & 0x20) | (data.index & 0x1f);
<<<<<<< HEAD
		tty_write(f, &ch, 1);
=======
		serial2002_tty_write(f, &ch, 1);
>>>>>>> refs/remotes/origin/master
	} else {
		unsigned char ch[6];
		int i = 0;
		if (data.value >= (1L << 30)) {
			ch[i] = 0x80 | ((data.value >> 30) & 0x03);
			i++;
		}
		if (data.value >= (1L << 23)) {
			ch[i] = 0x80 | ((data.value >> 23) & 0x7f);
			i++;
		}
		if (data.value >= (1L << 16)) {
			ch[i] = 0x80 | ((data.value >> 16) & 0x7f);
			i++;
		}
		if (data.value >= (1L << 9)) {
			ch[i] = 0x80 | ((data.value >> 9) & 0x7f);
			i++;
		}
		ch[i] = 0x80 | ((data.value >> 2) & 0x7f);
		i++;
		ch[i] = ((data.value << 5) & 0x60) | (data.index & 0x1f);
		i++;
<<<<<<< HEAD
		tty_write(f, ch, i);
	}
}

static int serial_2002_open(struct comedi_device *dev)
{
	int result;
	char port[20];

	sprintf(port, "/dev/ttyS%d", devpriv->port);
	devpriv->tty = filp_open(port, O_RDWR, 0);
	if (IS_ERR(devpriv->tty)) {
		result = (int)PTR_ERR(devpriv->tty);
<<<<<<< HEAD
		printk("serial_2002: file open error = %d\n", result);
=======
		printk(KERN_ERR "serial_2002: file open error = %d\n", result);
>>>>>>> refs/remotes/origin/cm-10.0
	} else {
		struct config_t {

			short int kind;
			short int bits;
			int min;
			int max;
		};

		struct config_t *dig_in_config;
		struct config_t *dig_out_config;
		struct config_t *chan_in_config;
		struct config_t *chan_out_config;
		int i;

		result = 0;
		dig_in_config = kcalloc(32, sizeof(struct config_t),
				GFP_KERNEL);
		dig_out_config = kcalloc(32, sizeof(struct config_t),
				GFP_KERNEL);
		chan_in_config = kcalloc(32, sizeof(struct config_t),
				GFP_KERNEL);
		chan_out_config = kcalloc(32, sizeof(struct config_t),
				GFP_KERNEL);
		if (!dig_in_config || !dig_out_config
		    || !chan_in_config || !chan_out_config) {
			result = -ENOMEM;
			goto err_alloc_configs;
		}

		tty_setspeed(devpriv->tty, devpriv->speed);
		poll_channel(devpriv->tty, 31);	/*  Start reading configuration */
		while (1) {
			struct serial_data data;

			data = serial_read(devpriv->tty, 1000);
			if (data.kind != is_channel || data.index != 31
			    || !(data.value & 0xe0)) {
				break;
			} else {
				int command, channel, kind;
				struct config_t *cur_config = NULL;

				channel = data.value & 0x1f;
				kind = (data.value >> 5) & 0x7;
				command = (data.value >> 8) & 0x3;
				switch (kind) {
				case 1:{
						cur_config = dig_in_config;
					}
					break;
				case 2:{
						cur_config = dig_out_config;
					}
					break;
				case 3:{
						cur_config = chan_in_config;
					}
					break;
				case 4:{
						cur_config = chan_out_config;
					}
					break;
				case 5:{
						cur_config = chan_in_config;
					}
					break;
				}

				if (cur_config) {
					cur_config[channel].kind = kind;
					switch (command) {
					case 0:{
							cur_config[channel].bits
							    =
							    (data.value >> 10) &
							    0x3f;
						}
						break;
					case 1:{
							int unit, sign, min;
							unit =
							    (data.value >> 10) &
							    0x7;
							sign =
							    (data.value >> 13) &
							    0x1;
							min =
							    (data.value >> 14) &
							    0xfffff;

							switch (unit) {
							case 0:{
									min =
									    min
									    *
									    1000000;
								}
								break;
							case 1:{
									min =
									    min
									    *
									    1000;
								}
								break;
							case 2:{
									min =
									    min
									    * 1;
								}
								break;
							}
<<<<<<< HEAD
							if (sign) {
								min = -min;
							}
=======
							if (sign)
								min = -min;
>>>>>>> refs/remotes/origin/cm-10.0
							cur_config[channel].min
							    = min;
						}
						break;
					case 2:{
							int unit, sign, max;
							unit =
							    (data.value >> 10) &
							    0x7;
							sign =
							    (data.value >> 13) &
							    0x1;
							max =
							    (data.value >> 14) &
							    0xfffff;

							switch (unit) {
							case 0:{
									max =
									    max
									    *
									    1000000;
								}
								break;
							case 1:{
									max =
									    max
									    *
									    1000;
								}
								break;
							case 2:{
									max =
									    max
									    * 1;
								}
								break;
							}
<<<<<<< HEAD
							if (sign) {
								max = -max;
							}
=======
							if (sign)
								max = -max;
>>>>>>> refs/remotes/origin/cm-10.0
							cur_config[channel].max
							    = max;
						}
						break;
					}
				}
			}
		}
		for (i = 0; i <= 4; i++) {
			/*  Fill in subdev data */
			struct config_t *c;
			unsigned char *mapping = NULL;
			struct serial2002_range_table_t *range = NULL;
			int kind = 0;

			switch (i) {
			case 0:{
					c = dig_in_config;
					mapping = devpriv->digital_in_mapping;
					kind = 1;
				}
				break;
			case 1:{
					c = dig_out_config;
					mapping = devpriv->digital_out_mapping;
					kind = 2;
				}
				break;
			case 2:{
					c = chan_in_config;
					mapping = devpriv->analog_in_mapping;
					range = devpriv->in_range;
					kind = 3;
				}
				break;
			case 3:{
					c = chan_out_config;
					mapping = devpriv->analog_out_mapping;
					range = devpriv->out_range;
					kind = 4;
				}
				break;
			case 4:{
					c = chan_in_config;
					mapping = devpriv->encoder_in_mapping;
					range = devpriv->in_range;
					kind = 5;
				}
				break;
			default:{
					c = NULL;
				}
				break;
			}
			if (c) {
				struct comedi_subdevice *s;
				const struct comedi_lrange **range_table_list =
				    NULL;
				unsigned int *maxdata_list;
				int j, chan;

				for (chan = 0, j = 0; j < 32; j++) {
<<<<<<< HEAD
					if (c[j].kind == kind) {
						chan++;
					}
=======
					if (c[j].kind == kind)
						chan++;
>>>>>>> refs/remotes/origin/cm-10.0
				}
				s = &dev->subdevices[i];
				s->n_chan = chan;
				s->maxdata = 0;
				kfree(s->maxdata_list);
				s->maxdata_list = maxdata_list =
				    kmalloc(sizeof(unsigned int) * s->n_chan,
					    GFP_KERNEL);
				if (!s->maxdata_list)
					break;	/* error handled below */
				kfree(s->range_table_list);
				s->range_table = NULL;
				s->range_table_list = NULL;
				if (range) {
					s->range_table_list = range_table_list =
					    kmalloc(sizeof
						    (struct
						     serial2002_range_table_t) *
						    s->n_chan, GFP_KERNEL);
					if (!s->range_table_list)
						break;	/* err handled below */
				}
				for (chan = 0, j = 0; j < 32; j++) {
					if (c[j].kind == kind) {
<<<<<<< HEAD
						if (mapping) {
							mapping[chan] = j;
						}
=======
						if (mapping)
							mapping[chan] = j;
>>>>>>> refs/remotes/origin/cm-10.0
						if (range) {
							range[j].length = 1;
							range[j].range.min =
							    c[j].min;
							range[j].range.max =
							    c[j].max;
							range_table_list[chan] =
							    (const struct
							     comedi_lrange *)
							    &range[j];
						}
						maxdata_list[chan] =
						    ((long long)1 << c[j].bits)
						    - 1;
						chan++;
					}
				}
			}
		}
		if (i <= 4) {
			/* Failed to allocate maxdata_list or range_table_list
			 * for a subdevice that needed it.  */
			result = -ENOMEM;
			for (i = 0; i <= 4; i++) {
				struct comedi_subdevice *s;

				s = &dev->subdevices[i];
				kfree(s->maxdata_list);
				s->maxdata_list = NULL;
				kfree(s->range_table_list);
				s->range_table_list = NULL;
			}
		}

err_alloc_configs:
		kfree(dig_in_config);
		kfree(dig_out_config);
		kfree(chan_in_config);
		kfree(chan_out_config);

		if (result) {
			if (devpriv->tty) {
				filp_close(devpriv->tty, 0);
				devpriv->tty = NULL;
			}
		}
	}
	return result;
}

static void serial_2002_close(struct comedi_device *dev)
{
<<<<<<< HEAD
	if (!IS_ERR(devpriv->tty) && (devpriv->tty != 0)) {
		filp_close(devpriv->tty, 0);
	}
=======
	if (!IS_ERR(devpriv->tty) && (devpriv->tty != 0))
		filp_close(devpriv->tty, 0);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int serial2002_di_rinsn(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data)
{
=======
		serial2002_tty_write(f, ch, i);
	}
}

struct config_t {
	short int kind;
	short int bits;
	int min;
	int max;
};

static int serial2002_setup_subdevice(struct comedi_subdevice *s,
				      struct config_t *cfg,
				      struct serial2002_range_table_t *range,
				      unsigned char *mapping,
				      int kind)
{
	const struct comedi_lrange **range_table_list = NULL;
	unsigned int *maxdata_list;
	int j, chan;

	for (chan = 0, j = 0; j < 32; j++) {
		if (cfg[j].kind == kind)
			chan++;
	}
	s->n_chan = chan;
	s->maxdata = 0;
	kfree(s->maxdata_list);
	maxdata_list = kmalloc(sizeof(unsigned int) * s->n_chan, GFP_KERNEL);
	if (!maxdata_list)
		return -ENOMEM;
	s->maxdata_list = maxdata_list;
	kfree(s->range_table_list);
	s->range_table = NULL;
	s->range_table_list = NULL;
	if (kind == 1 || kind == 2) {
		s->range_table = &range_digital;
	} else if (range) {
		range_table_list =
			kmalloc(sizeof(struct serial2002_range_table_t) *
				s->n_chan, GFP_KERNEL);
		if (!range_table_list)
			return -ENOMEM;
		s->range_table_list = range_table_list;
	}
	for (chan = 0, j = 0; j < 32; j++) {
		if (cfg[j].kind == kind) {
			if (mapping)
				mapping[chan] = j;
			if (range) {
				range[j].length = 1;
				range[j].range.min = cfg[j].min;
				range[j].range.max = cfg[j].max;
				range_table_list[chan] =
				    (const struct comedi_lrange *)&range[j];
			}
			maxdata_list[chan] = ((long long)1 << cfg[j].bits) - 1;
			chan++;
		}
	}
	return 0;
}

static int serial2002_setup_subdevs(struct comedi_device *dev)
{
	struct serial2002_private *devpriv = dev->private;
	struct config_t *di_cfg;
	struct config_t *do_cfg;
	struct config_t *ai_cfg;
	struct config_t *ao_cfg;
	struct config_t *cfg;
	struct comedi_subdevice *s;
	int result = 0;
	int i;

	/* Allocate the temporary structs to hold the configuration data */
	di_cfg = kcalloc(32, sizeof(*cfg), GFP_KERNEL);
	do_cfg = kcalloc(32, sizeof(*cfg), GFP_KERNEL);
	ai_cfg = kcalloc(32, sizeof(*cfg), GFP_KERNEL);
	ao_cfg = kcalloc(32, sizeof(*cfg), GFP_KERNEL);
	if (!di_cfg || !do_cfg || !ai_cfg || !ao_cfg) {
		result = -ENOMEM;
		goto err_alloc_configs;
	}

	/* Read the configuration from the connected device */
	serial2002_tty_setspeed(devpriv->tty, devpriv->speed);
	serial2002_poll_channel(devpriv->tty, 31);
	while (1) {
		struct serial_data data;

		data = serial2002_read(devpriv->tty, 1000);
		if (data.kind != is_channel || data.index != 31 ||
		    S2002_CFG_KIND(data.value) == S2002_CFG_KIND_INVALID) {
			break;
		} else {
			int channel = S2002_CFG_CHAN(data.value);
			int range = S2002_CFG_BASE(data.value);

			switch (S2002_CFG_KIND(data.value)) {
			case S2002_CFG_KIND_DIGITAL_IN:
				cfg = di_cfg;
				break;
			case S2002_CFG_KIND_DIGITAL_OUT:
				cfg = do_cfg;
				break;
			case S2002_CFG_KIND_ANALOG_IN:
				cfg = ai_cfg;
				break;
			case S2002_CFG_KIND_ANALOG_OUT:
				cfg = ao_cfg;
				break;
			case S2002_CFG_KIND_ENCODER_IN:
				cfg = ai_cfg;
				break;
			default:
				cfg = NULL;
				break;
			}
			if (!cfg)
				continue;	/* unknown kind, skip it */

			cfg[channel].kind = S2002_CFG_KIND(data.value);

			switch (S2002_CFG_CMD(data.value)) {
			case S2002_CFG_CMD_BITS:
				cfg[channel].bits = S2002_CFG_BITS(data.value);
				break;
			case S2002_CFG_CMD_MIN:
			case S2002_CFG_CMD_MAX:
				switch (S2002_CFG_UNITS(data.value)) {
				case 0:
					range *= 1000000;
					break;
				case 1:
					range *= 1000;
					break;
				case 2:
					range *= 1;
					break;
				}
				if (S2002_CFG_SIGN(data.value))
					range = -range;
				if (S2002_CFG_CMD(data.value) ==
				    S2002_CFG_CMD_MIN)
					cfg[channel].min = range;
				else
					cfg[channel].max = range;
				break;
			}
		}
	}

	/* Fill in subdevice data */
	for (i = 0; i <= 4; i++) {
		unsigned char *mapping = NULL;
		struct serial2002_range_table_t *range = NULL;
		int kind = 0;

		s = &dev->subdevices[i];

		switch (i) {
		case 0:
			cfg = di_cfg;
			mapping = devpriv->digital_in_mapping;
			kind = S2002_CFG_KIND_DIGITAL_IN;
			break;
		case 1:
			cfg = do_cfg;
			mapping = devpriv->digital_out_mapping;
			kind = S2002_CFG_KIND_DIGITAL_OUT;
			break;
		case 2:
			cfg = ai_cfg;
			mapping = devpriv->analog_in_mapping;
			range = devpriv->in_range;
			kind = S2002_CFG_KIND_ANALOG_IN;
			break;
		case 3:
			cfg = ao_cfg;
			mapping = devpriv->analog_out_mapping;
			range = devpriv->out_range;
			kind = S2002_CFG_KIND_ANALOG_OUT;
			break;
		case 4:
			cfg = ai_cfg;
			mapping = devpriv->encoder_in_mapping;
			range = devpriv->in_range;
			kind = S2002_CFG_KIND_ENCODER_IN;
			break;
		}

		if (serial2002_setup_subdevice(s, cfg, range, mapping, kind))
			break;	/* err handled below */
	}
	if (i <= 4) {
		/*
		 * Failed to allocate maxdata_list or range_table_list
		 * for a subdevice that needed it.
		 */
		result = -ENOMEM;
		for (i = 0; i <= 4; i++) {
			s = &dev->subdevices[i];
			kfree(s->maxdata_list);
			s->maxdata_list = NULL;
			kfree(s->range_table_list);
			s->range_table_list = NULL;
		}
	}

err_alloc_configs:
	kfree(di_cfg);
	kfree(do_cfg);
	kfree(ai_cfg);
	kfree(ao_cfg);

	if (result) {
		if (devpriv->tty) {
			filp_close(devpriv->tty, NULL);
			devpriv->tty = NULL;
		}
	}

	return result;
}

static int serial2002_open(struct comedi_device *dev)
{
	struct serial2002_private *devpriv = dev->private;
	int result;
	char port[20];

	sprintf(port, "/dev/ttyS%d", devpriv->port);
	devpriv->tty = filp_open(port, O_RDWR, 0);
	if (IS_ERR(devpriv->tty)) {
		result = (int)PTR_ERR(devpriv->tty);
		dev_err(dev->class_dev, "file open error = %d\n", result);
	} else {
		result = serial2002_setup_subdevs(dev);
	}
	return result;
}

static void serial2002_close(struct comedi_device *dev)
{
	struct serial2002_private *devpriv = dev->private;

	if (!IS_ERR(devpriv->tty) && devpriv->tty)
		filp_close(devpriv->tty, NULL);
}

static int serial2002_di_insn_read(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   struct comedi_insn *insn,
				   unsigned int *data)
{
	struct serial2002_private *devpriv = dev->private;
>>>>>>> refs/remotes/origin/master
	int n;
	int chan;

	chan = devpriv->digital_in_mapping[CR_CHAN(insn->chanspec)];
	for (n = 0; n < insn->n; n++) {
		struct serial_data read;

<<<<<<< HEAD
		poll_digital(devpriv->tty, chan);
		while (1) {
			read = serial_read(devpriv->tty, 1000);
<<<<<<< HEAD
			if (read.kind != is_digital || read.index == chan) {
				break;
			}
=======
			if (read.kind != is_digital || read.index == chan)
				break;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		serial2002_poll_digital(devpriv->tty, chan);
		while (1) {
			read = serial2002_read(devpriv->tty, 1000);
			if (read.kind != is_digital || read.index == chan)
				break;
>>>>>>> refs/remotes/origin/master
		}
		data[n] = read.value;
	}
	return n;
}

<<<<<<< HEAD
static int serial2002_do_winsn(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data)
{
=======
static int serial2002_do_insn_write(struct comedi_device *dev,
				    struct comedi_subdevice *s,
				    struct comedi_insn *insn,
				    unsigned int *data)
{
	struct serial2002_private *devpriv = dev->private;
>>>>>>> refs/remotes/origin/master
	int n;
	int chan;

	chan = devpriv->digital_out_mapping[CR_CHAN(insn->chanspec)];
	for (n = 0; n < insn->n; n++) {
		struct serial_data write;

		write.kind = is_digital;
		write.index = chan;
		write.value = data[n];
<<<<<<< HEAD
		serial_write(devpriv->tty, write);
=======
		serial2002_write(devpriv->tty, write);
>>>>>>> refs/remotes/origin/master
	}
	return n;
}

<<<<<<< HEAD
static int serial2002_ai_rinsn(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data)
{
=======
static int serial2002_ai_insn_read(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   struct comedi_insn *insn,
				   unsigned int *data)
{
	struct serial2002_private *devpriv = dev->private;
>>>>>>> refs/remotes/origin/master
	int n;
	int chan;

	chan = devpriv->analog_in_mapping[CR_CHAN(insn->chanspec)];
	for (n = 0; n < insn->n; n++) {
		struct serial_data read;

<<<<<<< HEAD
		poll_channel(devpriv->tty, chan);
		while (1) {
			read = serial_read(devpriv->tty, 1000);
<<<<<<< HEAD
			if (read.kind != is_channel || read.index == chan) {
				break;
			}
=======
			if (read.kind != is_channel || read.index == chan)
				break;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		serial2002_poll_channel(devpriv->tty, chan);
		while (1) {
			read = serial2002_read(devpriv->tty, 1000);
			if (read.kind != is_channel || read.index == chan)
				break;
>>>>>>> refs/remotes/origin/master
		}
		data[n] = read.value;
	}
	return n;
}

<<<<<<< HEAD
static int serial2002_ao_winsn(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data)
{
=======
static int serial2002_ao_insn_write(struct comedi_device *dev,
				    struct comedi_subdevice *s,
				    struct comedi_insn *insn,
				    unsigned int *data)
{
	struct serial2002_private *devpriv = dev->private;
>>>>>>> refs/remotes/origin/master
	int n;
	int chan;

	chan = devpriv->analog_out_mapping[CR_CHAN(insn->chanspec)];
	for (n = 0; n < insn->n; n++) {
		struct serial_data write;

		write.kind = is_channel;
		write.index = chan;
		write.value = data[n];
<<<<<<< HEAD
		serial_write(devpriv->tty, write);
=======
		serial2002_write(devpriv->tty, write);
>>>>>>> refs/remotes/origin/master
		devpriv->ao_readback[chan] = data[n];
	}
	return n;
}

<<<<<<< HEAD
static int serial2002_ao_rinsn(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data)
{
	int n;
	int chan = CR_CHAN(insn->chanspec);

<<<<<<< HEAD
	for (n = 0; n < insn->n; n++) {
		data[n] = devpriv->ao_readback[chan];
	}
=======
	for (n = 0; n < insn->n; n++)
		data[n] = devpriv->ao_readback[chan];
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int serial2002_ao_insn_read(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   struct comedi_insn *insn,
				   unsigned int *data)
{
	struct serial2002_private *devpriv = dev->private;
	int n;
	int chan = CR_CHAN(insn->chanspec);

	for (n = 0; n < insn->n; n++)
		data[n] = devpriv->ao_readback[chan];
>>>>>>> refs/remotes/origin/master

	return n;
}

<<<<<<< HEAD
static int serial2002_ei_rinsn(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data)
{
=======
static int serial2002_encoder_insn_read(struct comedi_device *dev,
					struct comedi_subdevice *s,
					struct comedi_insn *insn,
					unsigned int *data)
{
	struct serial2002_private *devpriv = dev->private;
>>>>>>> refs/remotes/origin/master
	int n;
	int chan;

	chan = devpriv->encoder_in_mapping[CR_CHAN(insn->chanspec)];
	for (n = 0; n < insn->n; n++) {
		struct serial_data read;

<<<<<<< HEAD
		poll_channel(devpriv->tty, chan);
		while (1) {
			read = serial_read(devpriv->tty, 1000);
<<<<<<< HEAD
			if (read.kind != is_channel || read.index == chan) {
				break;
			}
=======
			if (read.kind != is_channel || read.index == chan)
				break;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		serial2002_poll_channel(devpriv->tty, chan);
		while (1) {
			read = serial2002_read(devpriv->tty, 1000);
			if (read.kind != is_channel || read.index == chan)
				break;
>>>>>>> refs/remotes/origin/master
		}
		data[n] = read.value;
	}
	return n;
}

static int serial2002_attach(struct comedi_device *dev,
			     struct comedi_devconfig *it)
{
<<<<<<< HEAD
	struct comedi_subdevice *s;

<<<<<<< HEAD
	printk("comedi%d: serial2002: ", dev->minor);
	dev->board_name = thisboard->name;
	if (alloc_private(dev, sizeof(struct serial2002_private)) < 0) {
		return -ENOMEM;
	}
=======
	dev_dbg(dev->hw_dev, "comedi%d: attached\n", dev->minor);
	dev->board_name = thisboard->name;
	if (alloc_private(dev, sizeof(struct serial2002_private)) < 0)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0
	dev->open = serial_2002_open;
	dev->close = serial_2002_close;
	devpriv->port = it->options[0];
	devpriv->speed = it->options[1];
<<<<<<< HEAD
	printk("/dev/ttyS%d @ %d\n", devpriv->port, devpriv->speed);
=======
	dev_dbg(dev->hw_dev, "/dev/ttyS%d @ %d\n", devpriv->port,
		devpriv->speed);
>>>>>>> refs/remotes/origin/cm-10.0

	if (alloc_subdevices(dev, 5) < 0)
		return -ENOMEM;

	/* digital input subdevice */
	s = dev->subdevices + 0;
	s->type = COMEDI_SUBD_DI;
	s->subdev_flags = SDF_READABLE;
	s->n_chan = 0;
	s->maxdata = 1;
	s->range_table = &range_digital;
	s->insn_read = &serial2002_di_rinsn;

	/* digital output subdevice */
	s = dev->subdevices + 1;
	s->type = COMEDI_SUBD_DO;
	s->subdev_flags = SDF_WRITEABLE;
	s->n_chan = 0;
	s->maxdata = 1;
	s->range_table = &range_digital;
	s->insn_write = &serial2002_do_winsn;

	/* analog input subdevice */
	s = dev->subdevices + 2;
	s->type = COMEDI_SUBD_AI;
	s->subdev_flags = SDF_READABLE | SDF_GROUND;
	s->n_chan = 0;
	s->maxdata = 1;
	s->range_table = 0;
	s->insn_read = &serial2002_ai_rinsn;

	/* analog output subdevice */
	s = dev->subdevices + 3;
	s->type = COMEDI_SUBD_AO;
	s->subdev_flags = SDF_WRITEABLE;
	s->n_chan = 0;
	s->maxdata = 1;
	s->range_table = 0;
	s->insn_write = &serial2002_ao_winsn;
	s->insn_read = &serial2002_ao_rinsn;

	/* encoder input subdevice */
	s = dev->subdevices + 4;
	s->type = COMEDI_SUBD_COUNTER;
	s->subdev_flags = SDF_READABLE | SDF_LSAMPL;
	s->n_chan = 0;
	s->maxdata = 1;
	s->range_table = 0;
	s->insn_read = &serial2002_ei_rinsn;

	return 1;
}

static int serial2002_detach(struct comedi_device *dev)
=======
	struct serial2002_private *devpriv;
	struct comedi_subdevice *s;
	int ret;

	devpriv = comedi_alloc_devpriv(dev, sizeof(*devpriv));
	if (!devpriv)
		return -ENOMEM;

	devpriv->port = it->options[0];
	devpriv->speed = it->options[1];

	ret = comedi_alloc_subdevices(dev, 5);
	if (ret)
		return ret;

	/* digital input subdevice */
	s = &dev->subdevices[0];
	s->type		= COMEDI_SUBD_DI;
	s->subdev_flags	= SDF_READABLE;
	s->n_chan	= 0;
	s->maxdata	= 1;
	s->range_table	= &range_digital;
	s->insn_read	= serial2002_di_insn_read;

	/* digital output subdevice */
	s = &dev->subdevices[1];
	s->type		= COMEDI_SUBD_DO;
	s->subdev_flags	= SDF_WRITEABLE;
	s->n_chan	= 0;
	s->maxdata	= 1;
	s->range_table	= &range_digital;
	s->insn_write	= serial2002_do_insn_write;

	/* analog input subdevice */
	s = &dev->subdevices[2];
	s->type		= COMEDI_SUBD_AI;
	s->subdev_flags	= SDF_READABLE | SDF_GROUND;
	s->n_chan	= 0;
	s->maxdata	= 1;
	s->range_table	= NULL;
	s->insn_read	= serial2002_ai_insn_read;

	/* analog output subdevice */
	s = &dev->subdevices[3];
	s->type		= COMEDI_SUBD_AO;
	s->subdev_flags	= SDF_WRITEABLE;
	s->n_chan	= 0;
	s->maxdata	= 1;
	s->range_table	= NULL;
	s->insn_write	= serial2002_ao_insn_write;
	s->insn_read	= serial2002_ao_insn_read;

	/* encoder input subdevice */
	s = &dev->subdevices[4];
	s->type		= COMEDI_SUBD_COUNTER;
	s->subdev_flags	= SDF_READABLE | SDF_LSAMPL;
	s->n_chan	= 0;
	s->maxdata	= 1;
	s->range_table	= NULL;
	s->insn_read	= serial2002_encoder_insn_read;

	dev->open	= serial2002_open;
	dev->close	= serial2002_close;

	return 0;
}

static void serial2002_detach(struct comedi_device *dev)
>>>>>>> refs/remotes/origin/master
{
	struct comedi_subdevice *s;
	int i;

<<<<<<< HEAD
<<<<<<< HEAD
	printk("comedi%d: serial2002: remove\n", dev->minor);
=======
	dev_dbg(dev->hw_dev, "comedi%d: remove\n", dev->minor);
>>>>>>> refs/remotes/origin/cm-10.0
	for (i = 0; i < 5; i++) {
=======
	for (i = 0; i < dev->n_subdevices; i++) {
>>>>>>> refs/remotes/origin/master
		s = &dev->subdevices[i];
		kfree(s->maxdata_list);
		kfree(s->range_table_list);
	}
<<<<<<< HEAD
	return 0;
}

static int __init driver_serial2002_init_module(void)
{
	return comedi_driver_register(&driver_serial2002);
}

static void __exit driver_serial2002_cleanup_module(void)
{
	comedi_driver_unregister(&driver_serial2002);
}

module_init(driver_serial2002_init_module);
module_exit(driver_serial2002_cleanup_module);
=======
}

static struct comedi_driver serial2002_driver = {
	.driver_name	= "serial2002",
	.module		= THIS_MODULE,
	.attach		= serial2002_attach,
	.detach		= serial2002_detach,
};
module_comedi_driver(serial2002_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi low-level driver");
MODULE_LICENSE("GPL");
