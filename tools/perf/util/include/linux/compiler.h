#ifndef _PERF_LINUX_COMPILER_H_
#define _PERF_LINUX_COMPILER_H_

#ifndef __always_inline
#define __always_inline	inline
#endif
#define __user
<<<<<<< HEAD
#define __attribute_const__
=======
#ifndef __attribute_const__
#define __attribute_const__
#endif
>>>>>>> refs/remotes/origin/cm-10.0

#define __used		__attribute__((__unused__))

#endif
