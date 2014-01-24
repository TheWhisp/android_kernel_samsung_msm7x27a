#ifndef _LINUX_STDDEF_H
#define _LINUX_STDDEF_H

<<<<<<< HEAD
#include <linux/compiler.h>

<<<<<<< HEAD
#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif

#ifdef __KERNEL__
=======
#ifdef __KERNEL__

#undef NULL
#define NULL ((void *)0)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <uapi/linux/stddef.h>


#undef NULL
#define NULL ((void *)0)
>>>>>>> refs/remotes/origin/master

enum {
	false	= 0,
	true	= 1
};

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
>>>>>>> refs/remotes/origin/master
#endif
