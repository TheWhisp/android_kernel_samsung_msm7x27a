#ifndef _LINUX_TTY_FLIP_H
#define _LINUX_TTY_FLIP_H

<<<<<<< HEAD
extern int tty_buffer_request_room(struct tty_struct *tty, size_t size);
extern int tty_insert_flip_string_flags(struct tty_struct *tty, const unsigned char *chars, const char *flags, size_t size);
extern int tty_insert_flip_string_fixed_flag(struct tty_struct *tty, const unsigned char *chars, char flag, size_t size);
extern int tty_prepare_flip_string(struct tty_struct *tty, unsigned char **chars, size_t size);
extern int tty_prepare_flip_string_flags(struct tty_struct *tty, unsigned char **chars, char **flags, size_t size);
void tty_schedule_flip(struct tty_struct *tty);

static inline int tty_insert_flip_char(struct tty_struct *tty,
					unsigned char ch, char flag)
{
	struct tty_buffer *tb = tty->buf.tail;
	if (tb && tb->used < tb->size) {
		tb->flag_buf_ptr[tb->used] = flag;
		tb->char_buf_ptr[tb->used++] = ch;
		return 1;
	}
	return tty_insert_flip_string_flags(tty, &ch, &flag, 1);
}

static inline int tty_insert_flip_string(struct tty_struct *tty, const unsigned char *chars, size_t size)
{
	return tty_insert_flip_string_fixed_flag(tty, chars, TTY_NORMAL, size);
}

=======
extern int tty_buffer_set_limit(struct tty_port *port, int limit);
extern int tty_buffer_space_avail(struct tty_port *port);
extern int tty_buffer_request_room(struct tty_port *port, size_t size);
extern int tty_insert_flip_string_flags(struct tty_port *port,
		const unsigned char *chars, const char *flags, size_t size);
extern int tty_insert_flip_string_fixed_flag(struct tty_port *port,
		const unsigned char *chars, char flag, size_t size);
extern int tty_prepare_flip_string(struct tty_port *port,
		unsigned char **chars, size_t size);
extern void tty_flip_buffer_push(struct tty_port *port);
void tty_schedule_flip(struct tty_port *port);

static inline int tty_insert_flip_char(struct tty_port *port,
					unsigned char ch, char flag)
{
	struct tty_buffer *tb = port->buf.tail;
	int change;

	change = (tb->flags & TTYB_NORMAL) && (flag != TTY_NORMAL);
	if (!change && tb->used < tb->size) {
		if (~tb->flags & TTYB_NORMAL)
			*flag_buf_ptr(tb, tb->used) = flag;
		*char_buf_ptr(tb, tb->used++) = ch;
		return 1;
	}
	return tty_insert_flip_string_flags(port, &ch, &flag, 1);
}

static inline int tty_insert_flip_string(struct tty_port *port,
		const unsigned char *chars, size_t size)
{
	return tty_insert_flip_string_fixed_flag(port, chars, TTY_NORMAL, size);
}

extern void tty_buffer_lock_exclusive(struct tty_port *port);
extern void tty_buffer_unlock_exclusive(struct tty_port *port);

>>>>>>> refs/remotes/origin/master
#endif /* _LINUX_TTY_FLIP_H */
