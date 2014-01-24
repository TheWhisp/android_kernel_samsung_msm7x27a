/**
 * @file oprof.h
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon <levon@movementarian.org>
 */

#ifndef OPROF_H
#define OPROF_H

int oprofile_setup(void);
void oprofile_shutdown(void);

int oprofilefs_register(void);
void oprofilefs_unregister(void);

int oprofile_start(void);
void oprofile_stop(void);

struct oprofile_operations;

extern unsigned long oprofile_buffer_size;
extern unsigned long oprofile_cpu_buffer_size;
extern unsigned long oprofile_buffer_watershed;
extern unsigned long oprofile_time_slice;

extern struct oprofile_operations oprofile_ops;
extern unsigned long oprofile_started;
extern unsigned long oprofile_backtrace_depth;

<<<<<<< HEAD
struct super_block;
struct dentry;

void oprofile_create_files(struct super_block *sb, struct dentry *root);
int oprofile_timer_init(struct oprofile_operations *ops);
<<<<<<< HEAD
void oprofile_timer_exit(void);
=======
=======
struct dentry;

void oprofile_create_files(struct dentry *root);
int oprofile_timer_init(struct oprofile_operations *ops);
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_OPROFILE_NMI_TIMER
int op_nmi_timer_init(struct oprofile_operations *ops);
#else
static inline int op_nmi_timer_init(struct oprofile_operations *ops)
{
	return -ENODEV;
}
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

int oprofile_set_ulong(unsigned long *addr, unsigned long val);
int oprofile_set_timeout(unsigned long time);

#endif /* OPROF_H */
