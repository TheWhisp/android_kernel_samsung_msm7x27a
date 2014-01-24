/*
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{linux.intel,addtoit}.com)
 * Licensed under the GPL
 */

#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include "chan_kern.h"
=======
#include "chan.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include "os.h"
=======
#include "chan.h"
#include <os.h>
#include <irq_kern.h>
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_NOCONFIG_CHAN
static void *not_configged_init(char *str, int device,
				const struct chan_opts *opts)
{
	printk(KERN_ERR "Using a channel type which is configured out of "
	       "UML\n");
	return NULL;
}

static int not_configged_open(int input, int output, int primary, void *data,
			      char **dev_out)
{
	printk(KERN_ERR "Using a channel type which is configured out of "
	       "UML\n");
	return -ENODEV;
}

static void not_configged_close(int fd, void *data)
{
	printk(KERN_ERR "Using a channel type which is configured out of "
	       "UML\n");
}

static int not_configged_read(int fd, char *c_out, void *data)
{
	printk(KERN_ERR "Using a channel type which is configured out of "
	       "UML\n");
	return -EIO;
}

static int not_configged_write(int fd, const char *buf, int len, void *data)
{
	printk(KERN_ERR "Using a channel type which is configured out of "
	       "UML\n");
	return -EIO;
}

static int not_configged_console_write(int fd, const char *buf, int len)
{
	printk(KERN_ERR "Using a channel type which is configured out of "
	       "UML\n");
	return -EIO;
}

static int not_configged_window_size(int fd, void *data, unsigned short *rows,
				     unsigned short *cols)
{
	printk(KERN_ERR "Using a channel type which is configured out of "
	       "UML\n");
	return -ENODEV;
}

static void not_configged_free(void *data)
{
	printk(KERN_ERR "Using a channel type which is configured out of "
	       "UML\n");
}

static const struct chan_ops not_configged_ops = {
	.init		= not_configged_init,
	.open		= not_configged_open,
	.close		= not_configged_close,
	.read		= not_configged_read,
	.write		= not_configged_write,
	.console_write	= not_configged_console_write,
	.window_size	= not_configged_window_size,
	.free		= not_configged_free,
	.winch		= 0,
};
#endif /* CONFIG_NOCONFIG_CHAN */

<<<<<<< HEAD
static void tty_receive_char(struct tty_struct *tty, char ch)
{
	if (tty == NULL)
		return;

	if (I_IXON(tty) && !I_IXOFF(tty) && !tty->raw) {
		if (ch == STOP_CHAR(tty)) {
			stop_tty(tty);
			return;
		}
		else if (ch == START_CHAR(tty)) {
			start_tty(tty);
			return;
		}
	}

	tty_insert_flip_char(tty, ch, TTY_NORMAL);
}

=======
>>>>>>> refs/remotes/origin/master
static int open_one_chan(struct chan *chan)
{
	int fd, err;

	if (chan->opened)
		return 0;

	if (chan->ops->open == NULL)
		fd = 0;
	else fd = (*chan->ops->open)(chan->input, chan->output, chan->primary,
				     chan->data, &chan->dev);
	if (fd < 0)
		return fd;

	err = os_set_fd_block(fd, 0);
	if (err) {
		(*chan->ops->close)(fd, chan->data);
		return err;
	}

	chan->fd = fd;

	chan->opened = 1;
	return 0;
}

static int open_chan(struct list_head *chans)
{
	struct list_head *ele;
	struct chan *chan;
	int ret, err = 0;

	list_for_each(ele, chans) {
		chan = list_entry(ele, struct chan, list);
		ret = open_one_chan(chan);
		if (chan->primary)
			err = ret;
	}
	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
void chan_enable_winch(struct list_head *chans, struct tty_struct *tty)
{
	struct list_head *ele;
	struct chan *chan;

	list_for_each(ele, chans) {
		chan = list_entry(ele, struct chan, list);
		if (chan->primary && chan->output && chan->ops->winch) {
			register_winch(chan->fd, tty);
			return;
		}
	}
=======
void chan_enable_winch(struct chan *chan, struct tty_struct *tty)
{
	if (chan && chan->primary && chan->ops->winch)
		register_winch(chan->fd, tty);
=======
void chan_enable_winch(struct chan *chan, struct tty_port *port)
{
	if (chan && chan->primary && chan->ops->winch)
		register_winch(chan->fd, port);
>>>>>>> refs/remotes/origin/master
}

static void line_timer_cb(struct work_struct *work)
{
	struct line *line = container_of(work, struct line, task.work);

	if (!line->throttled)
<<<<<<< HEAD
		chan_interrupt(line, line->tty, line->driver->read_irq);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		chan_interrupt(line, line->driver->read_irq);
>>>>>>> refs/remotes/origin/master
}

int enable_chan(struct line *line)
{
	struct list_head *ele;
	struct chan *chan;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	INIT_DELAYED_WORK(&line->task, line_timer_cb);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	INIT_DELAYED_WORK(&line->task, line_timer_cb);

>>>>>>> refs/remotes/origin/master
	list_for_each(ele, &line->chan_list) {
		chan = list_entry(ele, struct chan, list);
		err = open_one_chan(chan);
		if (err) {
			if (chan->primary)
				goto out_close;

			continue;
		}

		if (chan->enabled)
			continue;
		err = line_setup_irq(chan->fd, chan->input, chan->output, line,
				     chan);
		if (err)
			goto out_close;

		chan->enabled = 1;
	}

	return 0;

 out_close:
<<<<<<< HEAD
<<<<<<< HEAD
	close_chan(&line->chan_list, 0);
=======
	close_chan(line);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	close_chan(line);
>>>>>>> refs/remotes/origin/master
	return err;
}

/* Items are added in IRQ context, when free_irq can't be called, and
 * removed in process context, when it can.
 * This handles interrupt sources which disappear, and which need to
 * be permanently disabled.  This is discovered in IRQ context, but
 * the freeing of the IRQ must be done later.
 */
static DEFINE_SPINLOCK(irqs_to_free_lock);
static LIST_HEAD(irqs_to_free);

void free_irqs(void)
{
	struct chan *chan;
	LIST_HEAD(list);
	struct list_head *ele;
	unsigned long flags;

	spin_lock_irqsave(&irqs_to_free_lock, flags);
	list_splice_init(&irqs_to_free, &list);
	spin_unlock_irqrestore(&irqs_to_free_lock, flags);

	list_for_each(ele, &list) {
		chan = list_entry(ele, struct chan, free_list);

		if (chan->input && chan->enabled)
<<<<<<< HEAD
			free_irq(chan->line->driver->read_irq, chan);
		if (chan->output && chan->enabled)
			free_irq(chan->line->driver->write_irq, chan);
=======
			um_free_irq(chan->line->driver->read_irq, chan);
		if (chan->output && chan->enabled)
			um_free_irq(chan->line->driver->write_irq, chan);
>>>>>>> refs/remotes/origin/master
		chan->enabled = 0;
	}
}

static void close_one_chan(struct chan *chan, int delay_free_irq)
{
	unsigned long flags;

	if (!chan->opened)
		return;

	if (delay_free_irq) {
		spin_lock_irqsave(&irqs_to_free_lock, flags);
		list_add(&chan->free_list, &irqs_to_free);
		spin_unlock_irqrestore(&irqs_to_free_lock, flags);
	}
	else {
		if (chan->input && chan->enabled)
<<<<<<< HEAD
			free_irq(chan->line->driver->read_irq, chan);
		if (chan->output && chan->enabled)
			free_irq(chan->line->driver->write_irq, chan);
=======
			um_free_irq(chan->line->driver->read_irq, chan);
		if (chan->output && chan->enabled)
			um_free_irq(chan->line->driver->write_irq, chan);
>>>>>>> refs/remotes/origin/master
		chan->enabled = 0;
	}
	if (chan->ops->close != NULL)
		(*chan->ops->close)(chan->fd, chan->data);

	chan->opened = 0;
	chan->fd = -1;
}

<<<<<<< HEAD
<<<<<<< HEAD
void close_chan(struct list_head *chans, int delay_free_irq)
=======
void close_chan(struct line *line)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void close_chan(struct line *line)
>>>>>>> refs/remotes/origin/master
{
	struct chan *chan;

	/* Close in reverse order as open in case more than one of them
	 * refers to the same device and they save and restore that device's
	 * state.  Then, the first one opened will have the original state,
	 * so it must be the last closed.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry_reverse(chan, chans, list) {
		close_one_chan(chan, delay_free_irq);
	}
}

void deactivate_chan(struct list_head *chans, int irq)
{
	struct list_head *ele;

	struct chan *chan;
	list_for_each(ele, chans) {
		chan = list_entry(ele, struct chan, list);

		if (chan->enabled && chan->input)
			deactivate_fd(chan->fd, irq);
	}
}

void reactivate_chan(struct list_head *chans, int irq)
{
	struct list_head *ele;
	struct chan *chan;

	list_for_each(ele, chans) {
		chan = list_entry(ele, struct chan, list);

		if (chan->enabled && chan->input)
			reactivate_fd(chan->fd, irq);
	}
}

int write_chan(struct list_head *chans, const char *buf, int len,
	       int write_irq)
{
	struct list_head *ele;
	struct chan *chan = NULL;
	int n, ret = 0;

	if (len == 0)
		return 0;

	list_for_each(ele, chans) {
		chan = list_entry(ele, struct chan, list);
		if (!chan->output || (chan->ops->write == NULL))
			continue;

		n = chan->ops->write(chan->fd, buf, len, chan->data);
		if (chan->primary) {
			ret = n;
			if ((ret == -EAGAIN) || ((ret >= 0) && (ret < len)))
				reactivate_fd(chan->fd, write_irq);
		}
=======
=======
>>>>>>> refs/remotes/origin/master
	list_for_each_entry_reverse(chan, &line->chan_list, list) {
		close_one_chan(chan, 0);
	}
}

void deactivate_chan(struct chan *chan, int irq)
{
	if (chan && chan->enabled)
		deactivate_fd(chan->fd, irq);
}

void reactivate_chan(struct chan *chan, int irq)
{
	if (chan && chan->enabled)
		reactivate_fd(chan->fd, irq);
}

int write_chan(struct chan *chan, const char *buf, int len,
	       int write_irq)
{
	int n, ret = 0;

	if (len == 0 || !chan || !chan->ops->write)
		return 0;

	n = chan->ops->write(chan->fd, buf, len, chan->data);
	if (chan->primary) {
		ret = n;
		if ((ret == -EAGAIN) || ((ret >= 0) && (ret < len)))
			reactivate_fd(chan->fd, write_irq);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
int console_write_chan(struct list_head *chans, const char *buf, int len)
{
	struct list_head *ele;
	struct chan *chan;
	int n, ret = 0;

	list_for_each(ele, chans) {
		chan = list_entry(ele, struct chan, list);
		if (!chan->output || (chan->ops->console_write == NULL))
			continue;

		n = chan->ops->console_write(chan->fd, buf, len);
		if (chan->primary)
			ret = n;
	}
=======
=======
>>>>>>> refs/remotes/origin/master
int console_write_chan(struct chan *chan, const char *buf, int len)
{
	int n, ret = 0;

	if (!chan || !chan->ops->console_write)
		return 0;

	n = chan->ops->console_write(chan->fd, buf, len);
	if (chan->primary)
		ret = n;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return ret;
}

int console_open_chan(struct line *line, struct console *co)
{
	int err;

	err = open_chan(&line->chan_list);
	if (err)
		return err;

	printk(KERN_INFO "Console initialized on /dev/%s%d\n", co->name,
	       co->index);
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
int chan_window_size(struct list_head *chans, unsigned short *rows_out,
		      unsigned short *cols_out)
{
	struct list_head *ele;
	struct chan *chan;

	list_for_each(ele, chans) {
		chan = list_entry(ele, struct chan, list);
		if (chan->primary) {
			if (chan->ops->window_size == NULL)
				return 0;
			return chan->ops->window_size(chan->fd, chan->data,
						      rows_out, cols_out);
		}
=======
=======
>>>>>>> refs/remotes/origin/master
int chan_window_size(struct line *line, unsigned short *rows_out,
		      unsigned short *cols_out)
{
	struct chan *chan;

	chan = line->chan_in;
	if (chan && chan->primary) {
		if (chan->ops->window_size == NULL)
			return 0;
		return chan->ops->window_size(chan->fd, chan->data,
					      rows_out, cols_out);
	}
	chan = line->chan_out;
	if (chan && chan->primary) {
		if (chan->ops->window_size == NULL)
			return 0;
		return chan->ops->window_size(chan->fd, chan->data,
					      rows_out, cols_out);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void free_one_chan(struct chan *chan, int delay_free_irq)
{
	list_del(&chan->list);

	close_one_chan(chan, delay_free_irq);
=======
=======
>>>>>>> refs/remotes/origin/master
static void free_one_chan(struct chan *chan)
{
	list_del(&chan->list);

	close_one_chan(chan, 0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (chan->ops->free != NULL)
		(*chan->ops->free)(chan->data);

	if (chan->primary && chan->output)
		ignore_sigio_fd(chan->fd);
	kfree(chan);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void free_chan(struct list_head *chans, int delay_free_irq)
=======
static void free_chan(struct list_head *chans)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void free_chan(struct list_head *chans)
>>>>>>> refs/remotes/origin/master
{
	struct list_head *ele, *next;
	struct chan *chan;

	list_for_each_safe(ele, next, chans) {
		chan = list_entry(ele, struct chan, list);
<<<<<<< HEAD
<<<<<<< HEAD
		free_one_chan(chan, delay_free_irq);
=======
		free_one_chan(chan);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		free_one_chan(chan);
>>>>>>> refs/remotes/origin/master
	}
}

static int one_chan_config_string(struct chan *chan, char *str, int size,
				  char **error_out)
{
	int n = 0;

	if (chan == NULL) {
		CONFIG_CHUNK(str, size, n, "none", 1);
		return n;
	}

	CONFIG_CHUNK(str, size, n, chan->ops->type, 0);

	if (chan->dev == NULL) {
		CONFIG_CHUNK(str, size, n, "", 1);
		return n;
	}

	CONFIG_CHUNK(str, size, n, ":", 0);
	CONFIG_CHUNK(str, size, n, chan->dev, 0);

	return n;
}

static int chan_pair_config_string(struct chan *in, struct chan *out,
				   char *str, int size, char **error_out)
{
	int n;

	n = one_chan_config_string(in, str, size, error_out);
	str += n;
	size -= n;

	if (in == out) {
		CONFIG_CHUNK(str, size, n, "", 1);
		return n;
	}

	CONFIG_CHUNK(str, size, n, ",", 1);
	n = one_chan_config_string(out, str, size, error_out);
	str += n;
	size -= n;
	CONFIG_CHUNK(str, size, n, "", 1);

	return n;
}

<<<<<<< HEAD
<<<<<<< HEAD
int chan_config_string(struct list_head *chans, char *str, int size,
		       char **error_out)
{
	struct list_head *ele;
	struct chan *chan, *in = NULL, *out = NULL;

	list_for_each(ele, chans) {
		chan = list_entry(ele, struct chan, list);
		if (!chan->primary)
			continue;
		if (chan->input)
			in = chan;
		if (chan->output)
			out = chan;
	}
=======
=======
>>>>>>> refs/remotes/origin/master
int chan_config_string(struct line *line, char *str, int size,
		       char **error_out)
{
	struct chan *in = line->chan_in, *out = line->chan_out;

	if (in && !in->primary)
		in = NULL;
	if (out && !out->primary)
		out = NULL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return chan_pair_config_string(in, out, str, size, error_out);
}

struct chan_type {
	char *key;
	const struct chan_ops *ops;
};

static const struct chan_type chan_table[] = {
	{ "fd", &fd_ops },

#ifdef CONFIG_NULL_CHAN
	{ "null", &null_ops },
#else
	{ "null", &not_configged_ops },
#endif

#ifdef CONFIG_PORT_CHAN
	{ "port", &port_ops },
#else
	{ "port", &not_configged_ops },
#endif

#ifdef CONFIG_PTY_CHAN
	{ "pty", &pty_ops },
	{ "pts", &pts_ops },
#else
	{ "pty", &not_configged_ops },
	{ "pts", &not_configged_ops },
#endif

#ifdef CONFIG_TTY_CHAN
	{ "tty", &tty_ops },
#else
	{ "tty", &not_configged_ops },
#endif

#ifdef CONFIG_XTERM_CHAN
	{ "xterm", &xterm_ops },
#else
	{ "xterm", &not_configged_ops },
#endif
};

static struct chan *parse_chan(struct line *line, char *str, int device,
			       const struct chan_opts *opts, char **error_out)
{
	const struct chan_type *entry;
	const struct chan_ops *ops;
	struct chan *chan;
	void *data;
	int i;

	ops = NULL;
	data = NULL;
	for(i = 0; i < ARRAY_SIZE(chan_table); i++) {
		entry = &chan_table[i];
		if (!strncmp(str, entry->key, strlen(entry->key))) {
			ops = entry->ops;
			str += strlen(entry->key);
			break;
		}
	}
	if (ops == NULL) {
		*error_out = "No match for configured backends";
		return NULL;
	}

	data = (*ops->init)(str, device, opts);
	if (data == NULL) {
		*error_out = "Configuration failed";
		return NULL;
	}

	chan = kmalloc(sizeof(*chan), GFP_ATOMIC);
	if (chan == NULL) {
		*error_out = "Memory allocation failed";
		return NULL;
	}
	*chan = ((struct chan) { .list	 	= LIST_HEAD_INIT(chan->list),
				 .free_list 	=
				 	LIST_HEAD_INIT(chan->free_list),
				 .line		= line,
				 .primary	= 1,
				 .input		= 0,
				 .output 	= 0,
				 .opened  	= 0,
				 .enabled  	= 0,
				 .fd 		= -1,
				 .ops 		= ops,
				 .data 		= data });
	return chan;
}

int parse_chan_pair(char *str, struct line *line, int device,
		    const struct chan_opts *opts, char **error_out)
{
	struct list_head *chans = &line->chan_list;
<<<<<<< HEAD
<<<<<<< HEAD
	struct chan *new, *chan;
	char *in, *out;

	if (!list_empty(chans)) {
		chan = list_entry(chans->next, struct chan, list);
		free_chan(chans, 0);
		INIT_LIST_HEAD(chans);
	}

=======
=======
>>>>>>> refs/remotes/origin/master
	struct chan *new;
	char *in, *out;

	if (!list_empty(chans)) {
		line->chan_in = line->chan_out = NULL;
		free_chan(chans);
		INIT_LIST_HEAD(chans);
	}

	if (!str)
		return 0;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	out = strchr(str, ',');
	if (out != NULL) {
		in = str;
		*out = '\0';
		out++;
		new = parse_chan(line, in, device, opts, error_out);
		if (new == NULL)
			return -1;

		new->input = 1;
		list_add(&new->list, chans);
<<<<<<< HEAD
<<<<<<< HEAD
=======
		line->chan_in = new;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		line->chan_in = new;
>>>>>>> refs/remotes/origin/master

		new = parse_chan(line, out, device, opts, error_out);
		if (new == NULL)
			return -1;

		list_add(&new->list, chans);
		new->output = 1;
<<<<<<< HEAD
<<<<<<< HEAD
=======
		line->chan_out = new;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		line->chan_out = new;
>>>>>>> refs/remotes/origin/master
	}
	else {
		new = parse_chan(line, str, device, opts, error_out);
		if (new == NULL)
			return -1;

		list_add(&new->list, chans);
		new->input = 1;
		new->output = 1;
<<<<<<< HEAD
<<<<<<< HEAD
=======
		line->chan_in = line->chan_out = new;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		line->chan_in = line->chan_out = new;
>>>>>>> refs/remotes/origin/master
	}
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
void chan_interrupt(struct list_head *chans, struct delayed_work *task,
		    struct tty_struct *tty, int irq)
{
	struct list_head *ele, *next;
	struct chan *chan;
	int err;
	char c;

	list_for_each_safe(ele, next, chans) {
		chan = list_entry(ele, struct chan, list);
		if (!chan->input || (chan->ops->read == NULL))
			continue;
		do {
			if (tty && !tty_buffer_request_room(tty, 1)) {
				schedule_delayed_work(task, 1);
				goto out;
			}
			err = chan->ops->read(chan->fd, &c, chan->data);
			if (err > 0)
				tty_receive_char(tty, c);
		} while (err > 0);

		if (err == 0)
			reactivate_fd(chan->fd, irq);
		if (err == -EIO) {
			if (chan->primary) {
				if (tty != NULL)
					tty_hangup(tty);
				close_chan(chans, 1);
				return;
			}
			else close_one_chan(chan, 1);
		}
=======
void chan_interrupt(struct line *line, struct tty_struct *tty, int irq)
{
=======
void chan_interrupt(struct line *line, int irq)
{
	struct tty_port *port = &line->port;
>>>>>>> refs/remotes/origin/master
	struct chan *chan = line->chan_in;
	int err;
	char c;

	if (!chan || !chan->ops->read)
		goto out;

	do {
<<<<<<< HEAD
		if (tty && !tty_buffer_request_room(tty, 1)) {
=======
		if (!tty_buffer_request_room(port, 1)) {
>>>>>>> refs/remotes/origin/master
			schedule_delayed_work(&line->task, 1);
			goto out;
		}
		err = chan->ops->read(chan->fd, &c, chan->data);
		if (err > 0)
<<<<<<< HEAD
			tty_receive_char(tty, c);
=======
			tty_insert_flip_char(port, c, TTY_NORMAL);
>>>>>>> refs/remotes/origin/master
	} while (err > 0);

	if (err == 0)
		reactivate_fd(chan->fd, irq);
	if (err == -EIO) {
		if (chan->primary) {
<<<<<<< HEAD
			if (tty != NULL)
				tty_hangup(tty);
=======
			tty_port_tty_hangup(&line->port, false);
>>>>>>> refs/remotes/origin/master
			if (line->chan_out != chan)
				close_one_chan(line->chan_out, 1);
		}
		close_one_chan(chan, 1);
		if (chan->primary)
			return;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	}
 out:
	if (tty)
		tty_flip_buffer_push(tty);
=======
	}
 out:
	tty_flip_buffer_push(port);
>>>>>>> refs/remotes/origin/master
}
