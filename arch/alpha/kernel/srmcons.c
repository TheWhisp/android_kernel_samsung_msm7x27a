/*
 *	linux/arch/alpha/kernel/srmcons.c
 *
 * Callback based driver for SRM Console console device.
 * (TTY driver and console driver)
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>

#include <asm/console.h>
#include <asm/uaccess.h>


static DEFINE_SPINLOCK(srmcons_callback_lock);
static int srm_is_registered_console = 0;

/* 
 * The TTY driver
 */
#define MAX_SRM_CONSOLE_DEVICES 1	/* only support 1 console device */

struct srmcons_private {
<<<<<<< HEAD
<<<<<<< HEAD
	struct tty_struct *tty;
	struct timer_list timer;
	spinlock_t lock;
};
=======
	struct tty_port port;
	struct timer_list timer;
} srmcons_singleton;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct tty_port port;
	struct timer_list timer;
} srmcons_singleton;
>>>>>>> refs/remotes/origin/master

typedef union _srmcons_result {
	struct {
		unsigned long c :61;
		unsigned long status :3;
	} bits;
	long as_long;
} srmcons_result;

/* called with callback_lock held */
static int
<<<<<<< HEAD
srmcons_do_receive_chars(struct tty_struct *tty)
=======
srmcons_do_receive_chars(struct tty_port *port)
>>>>>>> refs/remotes/origin/master
{
	srmcons_result result;
	int count = 0, loops = 0;

	do {
		result.as_long = callback_getc(0);
		if (result.bits.status < 2) {
<<<<<<< HEAD
			tty_insert_flip_char(tty, (char)result.bits.c, 0);
=======
			tty_insert_flip_char(port, (char)result.bits.c, 0);
>>>>>>> refs/remotes/origin/master
			count++;
		}
	} while((result.bits.status & 1) && (++loops < 10));

	if (count)
<<<<<<< HEAD
		tty_schedule_flip(tty);
=======
		tty_schedule_flip(port);
>>>>>>> refs/remotes/origin/master

	return count;
}

static void
srmcons_receive_chars(unsigned long data)
{
	struct srmcons_private *srmconsp = (struct srmcons_private *)data;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct tty_port *port = &srmconsp->port;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct tty_port *port = &srmconsp->port;
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	int incr = 10;

	local_irq_save(flags);
	if (spin_trylock(&srmcons_callback_lock)) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (!srmcons_do_receive_chars(srmconsp->tty))
=======
		if (!srmcons_do_receive_chars(port->tty))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (!srmcons_do_receive_chars(port))
>>>>>>> refs/remotes/origin/master
			incr = 100;
		spin_unlock(&srmcons_callback_lock);
	} 

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock(&srmconsp->lock);
	if (srmconsp->tty) {
		srmconsp->timer.expires = jiffies + incr;
		add_timer(&srmconsp->timer);
	}
	spin_unlock(&srmconsp->lock);
=======
=======
>>>>>>> refs/remotes/origin/master
	spin_lock(&port->lock);
	if (port->tty)
		mod_timer(&srmconsp->timer, jiffies + incr);
	spin_unlock(&port->lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	local_irq_restore(flags);
}

/* called with callback_lock held */
static int
<<<<<<< HEAD
srmcons_do_write(struct tty_struct *tty, const char *buf, int count)
=======
srmcons_do_write(struct tty_port *port, const char *buf, int count)
>>>>>>> refs/remotes/origin/master
{
	static char str_cr[1] = "\r";
	long c, remaining = count;
	srmcons_result result;
	char *cur;
	int need_cr;

	for (cur = (char *)buf; remaining > 0; ) {
		need_cr = 0;
		/* 
		 * Break it up into reasonable size chunks to allow a chance
		 * for input to get in
		 */
		for (c = 0; c < min_t(long, 128L, remaining) && !need_cr; c++)
			if (cur[c] == '\n')
				need_cr = 1;
		
		while (c > 0) {
			result.as_long = callback_puts(0, cur, c);
			c -= result.bits.c;
			remaining -= result.bits.c;
			cur += result.bits.c;

			/*
<<<<<<< HEAD
			 * Check for pending input iff a tty was provided
			 */
			if (tty)
				srmcons_do_receive_chars(tty);
=======
			 * Check for pending input iff a tty port was provided
			 */
			if (port)
				srmcons_do_receive_chars(port);
>>>>>>> refs/remotes/origin/master
		}

		while (need_cr) {
			result.as_long = callback_puts(0, str_cr, 1);
			if (result.bits.c > 0)
				need_cr = 0;
		}
	}
	return count;
}

static int
srmcons_write(struct tty_struct *tty,
	      const unsigned char *buf, int count)
{
	unsigned long flags;

	spin_lock_irqsave(&srmcons_callback_lock, flags);
<<<<<<< HEAD
	srmcons_do_write(tty, (const char *) buf, count);
=======
	srmcons_do_write(tty->port, (const char *) buf, count);
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&srmcons_callback_lock, flags);

	return count;
}

static int
srmcons_write_room(struct tty_struct *tty)
{
	return 512;
}

static int
srmcons_chars_in_buffer(struct tty_struct *tty)
{
	return 0;
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
srmcons_get_private_struct(struct srmcons_private **ps)
{
	static struct srmcons_private *srmconsp = NULL;
	static DEFINE_SPINLOCK(srmconsp_lock);
	unsigned long flags;
	int retval = 0;

	if (srmconsp == NULL) {
		srmconsp = kmalloc(sizeof(*srmconsp), GFP_KERNEL);
		spin_lock_irqsave(&srmconsp_lock, flags);

		if (srmconsp == NULL)
			retval = -ENOMEM;
		else {
			srmconsp->tty = NULL;
			spin_lock_init(&srmconsp->lock);
			init_timer(&srmconsp->timer);
		}

		spin_unlock_irqrestore(&srmconsp_lock, flags);
	}

	*ps = srmconsp;
	return retval;
}

static int
srmcons_open(struct tty_struct *tty, struct file *filp)
{
	struct srmcons_private *srmconsp;
	unsigned long flags;
	int retval;

	retval = srmcons_get_private_struct(&srmconsp);
	if (retval)
		return retval;

	spin_lock_irqsave(&srmconsp->lock, flags);

	if (!srmconsp->tty) {
		tty->driver_data = srmconsp;

		srmconsp->tty = tty;
		srmconsp->timer.function = srmcons_receive_chars;
		srmconsp->timer.data = (unsigned long)srmconsp;
		srmconsp->timer.expires = jiffies + 10;
		add_timer(&srmconsp->timer);
	}

	spin_unlock_irqrestore(&srmconsp->lock, flags);
=======
=======
>>>>>>> refs/remotes/origin/master
srmcons_open(struct tty_struct *tty, struct file *filp)
{
	struct srmcons_private *srmconsp = &srmcons_singleton;
	struct tty_port *port = &srmconsp->port;
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);

	if (!port->tty) {
		tty->driver_data = srmconsp;
		tty->port = port;
		port->tty = tty; /* XXX proper refcounting */
		mod_timer(&srmconsp->timer, jiffies + 10);
	}

	spin_unlock_irqrestore(&port->lock, flags);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

static void
srmcons_close(struct tty_struct *tty, struct file *filp)
{
	struct srmcons_private *srmconsp = tty->driver_data;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long flags;

	spin_lock_irqsave(&srmconsp->lock, flags);

	if (tty->count == 1) {
		srmconsp->tty = NULL;
		del_timer(&srmconsp->timer);
	}

	spin_unlock_irqrestore(&srmconsp->lock, flags);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct tty_port *port = &srmconsp->port;
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);

	if (tty->count == 1) {
		port->tty = NULL;
		del_timer(&srmconsp->timer);
	}

	spin_unlock_irqrestore(&port->lock, flags);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}


static struct tty_driver *srmcons_driver;

static const struct tty_operations srmcons_ops = {
	.open		= srmcons_open,
	.close		= srmcons_close,
	.write		= srmcons_write,
	.write_room	= srmcons_write_room,
	.chars_in_buffer= srmcons_chars_in_buffer,
};

static int __init
srmcons_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	tty_port_init(&srmcons_singleton.port);
	setup_timer(&srmcons_singleton.timer, srmcons_receive_chars,
			(unsigned long)&srmcons_singleton);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	setup_timer(&srmcons_singleton.timer, srmcons_receive_chars,
			(unsigned long)&srmcons_singleton);
>>>>>>> refs/remotes/origin/master
	if (srm_is_registered_console) {
		struct tty_driver *driver;
		int err;

		driver = alloc_tty_driver(MAX_SRM_CONSOLE_DEVICES);
		if (!driver)
			return -ENOMEM;
<<<<<<< HEAD
=======

		tty_port_init(&srmcons_singleton.port);

>>>>>>> refs/remotes/origin/master
		driver->driver_name = "srm";
		driver->name = "srm";
		driver->major = 0; 	/* dynamic */
		driver->minor_start = 0;
		driver->type = TTY_DRIVER_TYPE_SYSTEM;
		driver->subtype = SYSTEM_TYPE_SYSCONS;
		driver->init_termios = tty_std_termios;
		tty_set_operations(driver, &srmcons_ops);
<<<<<<< HEAD
		err = tty_register_driver(driver);
		if (err) {
			put_tty_driver(driver);
=======
		tty_port_link_device(&srmcons_singleton.port, driver, 0);
		err = tty_register_driver(driver);
		if (err) {
			put_tty_driver(driver);
			tty_port_destroy(&srmcons_singleton.port);
>>>>>>> refs/remotes/origin/master
			return err;
		}
		srmcons_driver = driver;
	}

	return -ENODEV;
}

module_init(srmcons_init);


/*
 * The console driver
 */
static void
srm_console_write(struct console *co, const char *s, unsigned count)
{
	unsigned long flags;

	spin_lock_irqsave(&srmcons_callback_lock, flags);
	srmcons_do_write(NULL, s, count);
	spin_unlock_irqrestore(&srmcons_callback_lock, flags);
}

static struct tty_driver *
srm_console_device(struct console *co, int *index)
{
	*index = co->index;
	return srmcons_driver;
}

static int
srm_console_setup(struct console *co, char *options)
{
	return 0;
}

static struct console srmcons = {
	.name		= "srm",
	.write		= srm_console_write,
	.device		= srm_console_device,
	.setup		= srm_console_setup,
	.flags		= CON_PRINTBUFFER | CON_BOOT,
	.index		= -1,
};

void __init
register_srm_console(void)
{
	if (!srm_is_registered_console) {
		callback_open_console();
		register_console(&srmcons);
		srm_is_registered_console = 1;
	}
}

void __init
unregister_srm_console(void)
{
	if (srm_is_registered_console) {
		callback_close_console();
		unregister_console(&srmcons);
		srm_is_registered_console = 0;
	}
}
