#ifndef _LINUX_SEQ_FILE_H
#define _LINUX_SEQ_FILE_H

#include <linux/types.h>
#include <linux/string.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/bug.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/bug.h>
>>>>>>> refs/remotes/origin/master
#include <linux/mutex.h>
#include <linux/cpumask.h>
#include <linux/nodemask.h>

struct seq_operations;
struct file;
struct path;
struct inode;
struct dentry;
<<<<<<< HEAD
=======
struct user_namespace;
>>>>>>> refs/remotes/origin/master

struct seq_file {
	char *buf;
	size_t size;
	size_t from;
	size_t count;
<<<<<<< HEAD
=======
	size_t pad_until;
>>>>>>> refs/remotes/origin/master
	loff_t index;
	loff_t read_pos;
	u64 version;
	struct mutex lock;
	const struct seq_operations *op;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int poll_event;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int poll_event;
#ifdef CONFIG_USER_NS
	struct user_namespace *user_ns;
#endif
>>>>>>> refs/remotes/origin/master
	void *private;
};

struct seq_operations {
	void * (*start) (struct seq_file *m, loff_t *pos);
	void (*stop) (struct seq_file *m, void *v);
	void * (*next) (struct seq_file *m, void *v, loff_t *pos);
	int (*show) (struct seq_file *m, void *v);
};

#define SEQ_SKIP 1

/**
 * seq_get_buf - get buffer to write arbitrary data to
 * @m: the seq_file handle
 * @bufp: the beginning of the buffer is stored here
 *
 * Return the number of bytes available in the buffer, or zero if
 * there's no space.
 */
static inline size_t seq_get_buf(struct seq_file *m, char **bufp)
{
	BUG_ON(m->count > m->size);
	if (m->count < m->size)
		*bufp = m->buf + m->count;
	else
		*bufp = NULL;

	return m->size - m->count;
}

/**
 * seq_commit - commit data to the buffer
 * @m: the seq_file handle
 * @num: the number of bytes to commit
 *
 * Commit @num bytes of data written to a buffer previously acquired
 * by seq_buf_get.  To signal an error condition, or that the data
 * didn't fit in the available space, pass a negative @num value.
 */
static inline void seq_commit(struct seq_file *m, int num)
{
	if (num < 0) {
		m->count = m->size;
	} else {
		BUG_ON(m->count + num > m->size);
		m->count += num;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
char *mangle_path(char *s, char *p, char *esc);
=======
char *mangle_path(char *s, const char *p, const char *esc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
/**
 * seq_setwidth - set padding width
 * @m: the seq_file handle
 * @size: the max number of bytes to pad.
 *
 * Call seq_setwidth() for setting max width, then call seq_printf() etc. and
 * finally call seq_pad() to pad the remaining bytes.
 */
static inline void seq_setwidth(struct seq_file *m, size_t size)
{
	m->pad_until = m->count + size;
}
void seq_pad(struct seq_file *m, char c);

char *mangle_path(char *s, const char *p, const char *esc);
>>>>>>> refs/remotes/origin/master
int seq_open(struct file *, const struct seq_operations *);
ssize_t seq_read(struct file *, char __user *, size_t, loff_t *);
loff_t seq_lseek(struct file *, loff_t, int);
int seq_release(struct inode *, struct file *);
int seq_escape(struct seq_file *, const char *, const char *);
int seq_putc(struct seq_file *m, char c);
int seq_puts(struct seq_file *m, const char *s);
int seq_write(struct seq_file *seq, const void *data, size_t len);

<<<<<<< HEAD
<<<<<<< HEAD
int seq_printf(struct seq_file *, const char *, ...)
	__attribute__ ((format (printf,2,3)));

int seq_path(struct seq_file *, struct path *, char *);
int seq_dentry(struct seq_file *, struct dentry *, char *);
int seq_path_root(struct seq_file *m, struct path *path, struct path *root,
		  char *esc);
=======
__printf(2, 3) int seq_printf(struct seq_file *, const char *, ...);
=======
__printf(2, 3) int seq_printf(struct seq_file *, const char *, ...);
__printf(2, 0) int seq_vprintf(struct seq_file *, const char *, va_list args);
>>>>>>> refs/remotes/origin/master

int seq_path(struct seq_file *, const struct path *, const char *);
int seq_dentry(struct seq_file *, struct dentry *, const char *);
int seq_path_root(struct seq_file *m, const struct path *path,
		  const struct path *root, const char *esc);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
int seq_bitmap(struct seq_file *m, const unsigned long *bits,
				   unsigned int nr_bits);
static inline int seq_cpumask(struct seq_file *m, const struct cpumask *mask)
{
	return seq_bitmap(m, cpumask_bits(mask), nr_cpu_ids);
}

static inline int seq_nodemask(struct seq_file *m, nodemask_t *mask)
{
	return seq_bitmap(m, mask->bits, MAX_NUMNODES);
}

int seq_bitmap_list(struct seq_file *m, const unsigned long *bits,
		unsigned int nr_bits);

static inline int seq_cpumask_list(struct seq_file *m,
				   const struct cpumask *mask)
{
	return seq_bitmap_list(m, cpumask_bits(mask), nr_cpu_ids);
}

static inline int seq_nodemask_list(struct seq_file *m, nodemask_t *mask)
{
	return seq_bitmap_list(m, mask->bits, MAX_NUMNODES);
}

int single_open(struct file *, int (*)(struct seq_file *, void *), void *);
<<<<<<< HEAD
=======
int single_open_size(struct file *, int (*)(struct seq_file *, void *), void *, size_t);
>>>>>>> refs/remotes/origin/master
int single_release(struct inode *, struct file *);
void *__seq_open_private(struct file *, const struct seq_operations *, int);
int seq_open_private(struct file *, const struct seq_operations *, int);
int seq_release_private(struct inode *, struct file *);
<<<<<<< HEAD
<<<<<<< HEAD

#define SEQ_START_TOKEN ((void *)1)

=======
=======
>>>>>>> refs/remotes/origin/master
int seq_put_decimal_ull(struct seq_file *m, char delimiter,
			unsigned long long num);
int seq_put_decimal_ll(struct seq_file *m, char delimiter,
			long long num);

<<<<<<< HEAD
#define SEQ_START_TOKEN ((void *)1)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline struct user_namespace *seq_user_ns(struct seq_file *seq)
{
#ifdef CONFIG_USER_NS
	return seq->user_ns;
#else
	extern struct user_namespace init_user_ns;
	return &init_user_ns;
#endif
}

#define SEQ_START_TOKEN ((void *)1)
>>>>>>> refs/remotes/origin/master
/*
 * Helpers for iteration over list_head-s in seq_files
 */

extern struct list_head *seq_list_start(struct list_head *head,
		loff_t pos);
extern struct list_head *seq_list_start_head(struct list_head *head,
		loff_t pos);
extern struct list_head *seq_list_next(void *v, struct list_head *head,
		loff_t *ppos);

/*
 * Helpers for iteration over hlist_head-s in seq_files
 */

extern struct hlist_node *seq_hlist_start(struct hlist_head *head,
					  loff_t pos);
extern struct hlist_node *seq_hlist_start_head(struct hlist_head *head,
					       loff_t pos);
extern struct hlist_node *seq_hlist_next(void *v, struct hlist_head *head,
					 loff_t *ppos);

extern struct hlist_node *seq_hlist_start_rcu(struct hlist_head *head,
					      loff_t pos);
extern struct hlist_node *seq_hlist_start_head_rcu(struct hlist_head *head,
						   loff_t pos);
extern struct hlist_node *seq_hlist_next_rcu(void *v,
						   struct hlist_head *head,
						   loff_t *ppos);
<<<<<<< HEAD
=======

/* Helpers for iterating over per-cpu hlist_head-s in seq_files */
extern struct hlist_node *seq_hlist_start_percpu(struct hlist_head __percpu *head, int *cpu, loff_t pos);

extern struct hlist_node *seq_hlist_next_percpu(void *v, struct hlist_head __percpu *head, int *cpu, loff_t *pos);

>>>>>>> refs/remotes/origin/master
#endif
