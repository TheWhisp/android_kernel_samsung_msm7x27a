/*
 * arch/xtensa/platforms/iss/console.c
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001-2005 Tensilica Inc.
 *   Authors	Christian Zankel, Joe Taylor
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/console.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/major.h>
#include <linux/param.h>
#include <linux/seq_file.h>
#include <linux/serial.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/serialP.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#include <asm/uaccess.h>
#include <asm/irq.h>

#include <platform/simcall.h>

#include <linux/tty.h>
#include <linux/tty_flip.h>

#ifdef SERIAL_INLINE
#define _INLINE_ inline
#endif

#define SERIAL_MAX_NUM_LINES 1
<<<<<<< HEAD
#define SERIAL_TIMER_VALUE (20 * HZ)

static struct tty_driver *serial_driver;
<<<<<<< HEAD
=======
static struct tty_port serial_port;
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define SERIAL_TIMER_VALUE (HZ / 10)

static struct tty_driver *serial_driver;
static struct tty_port serial_port;
>>>>>>> refs/remotes/origin/master
static struct timer_list serial_timer;

static DEFINE_SPINLOCK(timer_lock);

<<<<<<< HEAD
int errno;

static int __simc (int a, int b, int c, int d, int e, int f) __attribute__((__noinline__));
static int __simc (int a, int b, int c, int d, int e, int f)
{
	int ret;
	__asm__ __volatile__ ("simcall\n"
			"mov %0, a2\n"
			"mov %1, a3\n" : "=a" (ret), "=a" (errno)
			: : "a2", "a3");
	return ret;
}

=======
>>>>>>> refs/remotes/origin/master
static char *serial_version = "0.1";
static char *serial_name = "ISS serial driver";

/*
 * This routine is called whenever a serial port is opened.  It
 * enables interrupts for a serial port, linking in its async structure into
 * the IRQ chain.   It also performs the serial-specific
 * initialization for the tty structure.
 */

static void rs_poll(unsigned long);

static int rs_open(struct tty_struct *tty, struct file * filp)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int line = tty->index;

	if ((line < 0) || (line >= SERIAL_MAX_NUM_LINES))
		return -ENODEV;

	spin_lock(&timer_lock);

	if (tty->count == 1) {
		init_timer(&serial_timer);
		serial_timer.data = (unsigned long) tty;
		serial_timer.function = rs_poll;
=======
	tty->port = &serial_port;
	spin_lock(&timer_lock);
	if (tty->count == 1) {
		setup_timer(&serial_timer, rs_poll, (unsigned long)tty);
>>>>>>> refs/remotes/origin/cm-10.0
		mod_timer(&serial_timer, jiffies + SERIAL_TIMER_VALUE);
	}
	spin_unlock(&timer_lock);
=======
	tty->port = &serial_port;
	spin_lock_bh(&timer_lock);
	if (tty->count == 1) {
		setup_timer(&serial_timer, rs_poll,
				(unsigned long)&serial_port);
		mod_timer(&serial_timer, jiffies + SERIAL_TIMER_VALUE);
	}
	spin_unlock_bh(&timer_lock);
>>>>>>> refs/remotes/origin/master

	return 0;
}


/*
 * ------------------------------------------------------------
 * iss_serial_close()
 *
 * This routine is called when the serial port gets closed.  First, we
 * wait for the last remaining data to be sent.  Then, we unlink its
 * async structure from the interrupt chain if necessary, and we free
 * that IRQ if nothing is left in the chain.
 * ------------------------------------------------------------
 */
static void rs_close(struct tty_struct *tty, struct file * filp)
{
<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock(&timer_lock);
	if (tty->count == 1)
		del_timer_sync(&serial_timer);
	spin_unlock(&timer_lock);
=======
=======
>>>>>>> refs/remotes/origin/master
	spin_lock_bh(&timer_lock);
	if (tty->count == 1)
		del_timer_sync(&serial_timer);
	spin_unlock_bh(&timer_lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}


static int rs_write(struct tty_struct * tty,
		    const unsigned char *buf, int count)
{
	/* see drivers/char/serialX.c to reference original version */

<<<<<<< HEAD
	__simc (SYS_write, 1, (unsigned long)buf, count, 0, 0);
=======
	simc_write(1, buf, count);
>>>>>>> refs/remotes/origin/master
	return count;
}

static void rs_poll(unsigned long priv)
{
<<<<<<< HEAD
	struct tty_struct* tty = (struct tty_struct*) priv;

	struct timeval tv = { .tv_sec = 0, .tv_usec = 0 };
=======
	struct tty_port *port = (struct tty_port *)priv;
>>>>>>> refs/remotes/origin/master
	int i = 0;
	unsigned char c;

	spin_lock(&timer_lock);

<<<<<<< HEAD
	while (__simc(SYS_select_one, 0, XTISS_SELECT_ONE_READ, (int)&tv,0,0)){
		__simc (SYS_read, 0, (unsigned long)&c, 1, 0, 0);
		tty_insert_flip_char(tty, c, TTY_NORMAL);
=======
	while (simc_poll(0)) {
		simc_read(0, &c, 1);
		tty_insert_flip_char(port, c, TTY_NORMAL);
>>>>>>> refs/remotes/origin/master
		i++;
	}

	if (i)
<<<<<<< HEAD
		tty_flip_buffer_push(tty);
=======
		tty_flip_buffer_push(port);
>>>>>>> refs/remotes/origin/master


	mod_timer(&serial_timer, jiffies + SERIAL_TIMER_VALUE);
	spin_unlock(&timer_lock);
}


static int rs_put_char(struct tty_struct *tty, unsigned char ch)
{
<<<<<<< HEAD
	char buf[2];

	buf[0] = ch;
	buf[1] = '\0';		/* Is this NULL necessary? */
	__simc (SYS_write, 1, (unsigned long) buf, 1, 0, 0);
	return 1;
=======
	return rs_write(tty, &ch, 1);
>>>>>>> refs/remotes/origin/master
}

static void rs_flush_chars(struct tty_struct *tty)
{
}

static int rs_write_room(struct tty_struct *tty)
{
	/* Let's say iss can always accept 2K characters.. */
	return 2 * 1024;
}

static int rs_chars_in_buffer(struct tty_struct *tty)
{
	/* the iss doesn't buffer characters */
	return 0;
}

static void rs_hangup(struct tty_struct *tty)
{
	/* Stub, once again.. */
}

static void rs_wait_until_sent(struct tty_struct *tty, int timeout)
{
	/* Stub, once again.. */
}

static int rs_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "serinfo:1.0 driver:%s\n", serial_version);
	return 0;
}

static int rs_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, rs_proc_show, NULL);
}

static const struct file_operations rs_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= rs_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static const struct tty_operations serial_ops = {
	.open = rs_open,
	.close = rs_close,
	.write = rs_write,
	.put_char = rs_put_char,
	.flush_chars = rs_flush_chars,
	.write_room = rs_write_room,
	.chars_in_buffer = rs_chars_in_buffer,
	.hangup = rs_hangup,
	.wait_until_sent = rs_wait_until_sent,
	.proc_fops = &rs_proc_fops,
};

int __init rs_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	serial_driver = alloc_tty_driver(1);
=======
	tty_port_init(&serial_port);

	serial_driver = alloc_tty_driver(SERIAL_MAX_NUM_LINES);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	tty_port_init(&serial_port);

	serial_driver = alloc_tty_driver(SERIAL_MAX_NUM_LINES);
>>>>>>> refs/remotes/origin/master

	printk ("%s %s\n", serial_name, serial_version);

	/* Initialize the tty_driver structure */

<<<<<<< HEAD
<<<<<<< HEAD
	serial_driver->owner = THIS_MODULE;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	serial_driver->driver_name = "iss_serial";
	serial_driver->name = "ttyS";
	serial_driver->major = TTY_MAJOR;
	serial_driver->minor_start = 64;
	serial_driver->type = TTY_DRIVER_TYPE_SERIAL;
	serial_driver->subtype = SERIAL_TYPE_NORMAL;
	serial_driver->init_termios = tty_std_termios;
	serial_driver->init_termios.c_cflag =
		B9600 | CS8 | CREAD | HUPCL | CLOCAL;
	serial_driver->flags = TTY_DRIVER_REAL_RAW;

	tty_set_operations(serial_driver, &serial_ops);
<<<<<<< HEAD
=======
	tty_port_link_device(&serial_port, serial_driver, 0);
>>>>>>> refs/remotes/origin/master

	if (tty_register_driver(serial_driver))
		panic("Couldn't register serial driver\n");
	return 0;
}


static __exit void rs_exit(void)
{
	int error;

	if ((error = tty_unregister_driver(serial_driver)))
		printk("ISS_SERIAL: failed to unregister serial driver (%d)\n",
		       error);
	put_tty_driver(serial_driver);
<<<<<<< HEAD
=======
	tty_port_destroy(&serial_port);
>>>>>>> refs/remotes/origin/master
}


/* We use `late_initcall' instead of just `__initcall' as a workaround for
 * the fact that (1) simcons_tty_init can't be called before tty_init,
 * (2) tty_init is called via `module_init', (3) if statically linked,
 * module_init == device_init, and (4) there's no ordering of init lists.
 * We can do this easily because simcons is always statically linked, but
 * other tty drivers that depend on tty_init and which must use
 * `module_init' to declare their init routines are likely to be broken.
 */

late_initcall(rs_init);


#ifdef CONFIG_SERIAL_CONSOLE

static void iss_console_write(struct console *co, const char *s, unsigned count)
{
	int len = strlen(s);

	if (s != 0 && *s != 0)
<<<<<<< HEAD
		__simc (SYS_write, 1, (unsigned long)s,
			count < len ? count : len,0,0);
=======
		simc_write(1, s, count < len ? count : len);
>>>>>>> refs/remotes/origin/master
}

static struct tty_driver* iss_console_device(struct console *c, int *index)
{
	*index = c->index;
	return serial_driver;
}


static struct console sercons = {
	.name = "ttyS",
	.write = iss_console_write,
	.device = iss_console_device,
	.flags = CON_PRINTBUFFER,
	.index = -1
};

static int __init iss_console_init(void)
{
	register_console(&sercons);
	return 0;
}

console_initcall(iss_console_init);

#endif /* CONFIG_SERIAL_CONSOLE */

