#ifndef __LINUX_COMPILER_H
#error "Please don't include <linux/compiler-gcc3.h> directly, include <linux/compiler.h> instead."
#endif

<<<<<<< HEAD
#if __GNUC_MINOR__ < 2
# error Sorry, your compiler is too old - please upgrade it.
#endif

#if __GNUC_MINOR__ >= 3
=======
#if GCC_VERSION < 30200
# error Sorry, your compiler is too old - please upgrade it.
#endif

#if GCC_VERSION >= 30300
>>>>>>> refs/remotes/origin/master
# define __used			__attribute__((__used__))
#else
# define __used			__attribute__((__unused__))
#endif

<<<<<<< HEAD
#if __GNUC_MINOR__ >= 4
=======
#if GCC_VERSION >= 30400
>>>>>>> refs/remotes/origin/master
#define __must_check		__attribute__((warn_unused_result))
#endif

#ifdef CONFIG_GCOV_KERNEL
<<<<<<< HEAD
# if __GNUC_MINOR__ < 4
=======
# if GCC_VERSION < 30400
>>>>>>> refs/remotes/origin/master
#   error "GCOV profiling support for gcc versions below 3.4 not included"
# endif /* __GNUC_MINOR__ */
#endif /* CONFIG_GCOV_KERNEL */
