#ifndef _LINUX_COREDUMP_H
#define _LINUX_COREDUMP_H

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/fs.h>
<<<<<<< HEAD
=======
#include <asm/siginfo.h>
>>>>>>> refs/remotes/origin/master

/*
 * These are the only things you should do on a core-file: use only these
 * functions to write out all the necessary info.
 */
<<<<<<< HEAD
extern int dump_write(struct file *file, const void *addr, int nr);
extern int dump_seek(struct file *file, loff_t off);
=======
struct coredump_params;
extern int dump_skip(struct coredump_params *cprm, size_t nr);
extern int dump_emit(struct coredump_params *cprm, const void *addr, int nr);
extern int dump_align(struct coredump_params *cprm, int align);
#ifdef CONFIG_COREDUMP
extern void do_coredump(const siginfo_t *siginfo);
#else
static inline void do_coredump(const siginfo_t *siginfo) {}
#endif
>>>>>>> refs/remotes/origin/master

#endif /* _LINUX_COREDUMP_H */
