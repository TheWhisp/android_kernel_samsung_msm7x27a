/*
 * descriptor table internals; you almost certainly want file.h instead.
 */

#ifndef __LINUX_FDTABLE_H
#define __LINUX_FDTABLE_H

#include <linux/posix_types.h>
#include <linux/compiler.h>
#include <linux/spinlock.h>
#include <linux/rcupdate.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/fs.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/master

/*
 * The default fd array needs to be at least BITS_PER_LONG,
 * as this is the granularity returned by copy_fdset().
 */
#define NR_OPEN_DEFAULT BITS_PER_LONG

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * The embedded_fd_set is a small fd_set,
 * suitable for most tasks (which open <= BITS_PER_LONG files)
 */
struct embedded_fd_set {
	unsigned long fds_bits[1];
};

struct fdtable {
	unsigned int max_fds;
	struct file __rcu **fd;      /* current fd array */
	fd_set *close_on_exec;
	fd_set *open_fds;
=======
=======
>>>>>>> refs/remotes/origin/master
struct fdtable {
	unsigned int max_fds;
	struct file __rcu **fd;      /* current fd array */
	unsigned long *close_on_exec;
	unsigned long *open_fds;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	struct rcu_head rcu;
	struct fdtable *next;
};

<<<<<<< HEAD
=======
static inline void __set_close_on_exec(int fd, struct fdtable *fdt)
{
	__set_bit(fd, fdt->close_on_exec);
}

static inline void __clear_close_on_exec(int fd, struct fdtable *fdt)
{
	__clear_bit(fd, fdt->close_on_exec);
}

=======
	struct rcu_head rcu;
};

>>>>>>> refs/remotes/origin/master
static inline bool close_on_exec(int fd, const struct fdtable *fdt)
{
	return test_bit(fd, fdt->close_on_exec);
}

<<<<<<< HEAD
static inline void __set_open_fd(int fd, struct fdtable *fdt)
{
	__set_bit(fd, fdt->open_fds);
}

static inline void __clear_open_fd(int fd, struct fdtable *fdt)
{
	__clear_bit(fd, fdt->open_fds);
}

=======
>>>>>>> refs/remotes/origin/master
static inline bool fd_is_open(int fd, const struct fdtable *fdt)
{
	return test_bit(fd, fdt->open_fds);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Open file table structure
 */
struct files_struct {
  /*
   * read mostly part
   */
	atomic_t count;
	struct fdtable __rcu *fdt;
	struct fdtable fdtab;
  /*
   * written part on a separate cache line in SMP
   */
	spinlock_t file_lock ____cacheline_aligned_in_smp;
	int next_fd;
<<<<<<< HEAD
<<<<<<< HEAD
	struct embedded_fd_set close_on_exec_init;
	struct embedded_fd_set open_fds_init;
=======
	unsigned long close_on_exec_init[1];
	unsigned long open_fds_init[1];
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned long close_on_exec_init[1];
	unsigned long open_fds_init[1];
>>>>>>> refs/remotes/origin/master
	struct file __rcu * fd_array[NR_OPEN_DEFAULT];
};

#define rcu_dereference_check_fdtable(files, fdtfd) \
	(rcu_dereference_check((fdtfd), \
<<<<<<< HEAD
<<<<<<< HEAD
			       rcu_read_lock_held() || \
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			       lockdep_is_held(&(files)->file_lock) || \
			       atomic_read(&(files)->count) == 1 || \
			       rcu_my_thread_group_empty()))

#define files_fdtable(files) \
		(rcu_dereference_check_fdtable((files), (files)->fdt))

struct file_operations;
struct vfsmount;
struct dentry;

<<<<<<< HEAD
extern int expand_files(struct files_struct *, int nr);
extern void free_fdtable_rcu(struct rcu_head *rcu);
extern void __init files_defer_init(void);

static inline void free_fdtable(struct fdtable *fdt)
{
	call_rcu(&fdt->rcu, free_fdtable_rcu);
}

=======
extern void __init files_defer_init(void);

>>>>>>> refs/remotes/origin/master
static inline struct file * fcheck_files(struct files_struct *files, unsigned int fd)
{
	struct file * file = NULL;
	struct fdtable *fdt = files_fdtable(files);

	if (fd < fdt->max_fds)
		file = rcu_dereference_check_fdtable(files, fdt->fd[fd]);
	return file;
}

/*
 * Check whether the specified fd has an open file.
 */
#define fcheck(fd)	fcheck_files(current->files, fd)

struct task_struct;

struct files_struct *get_files_struct(struct task_struct *);
void put_files_struct(struct files_struct *fs);
void reset_files_struct(struct files_struct *);
int unshare_files(struct files_struct **);
struct files_struct *dup_fd(struct files_struct *, int *);
<<<<<<< HEAD
=======
void do_close_on_exec(struct files_struct *);
int iterate_fd(struct files_struct *, unsigned,
		int (*)(const void *, struct file *, unsigned),
		const void *);

extern int __alloc_fd(struct files_struct *files,
		      unsigned start, unsigned end, unsigned flags);
extern void __fd_install(struct files_struct *files,
		      unsigned int fd, struct file *file);
extern int __close_fd(struct files_struct *files,
		      unsigned int fd);
>>>>>>> refs/remotes/origin/master

extern struct kmem_cache *files_cachep;

#endif /* __LINUX_FDTABLE_H */
