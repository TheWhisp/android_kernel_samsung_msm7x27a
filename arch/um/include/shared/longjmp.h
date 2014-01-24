#ifndef __UML_LONGJMP_H
#define __UML_LONGJMP_H

<<<<<<< HEAD
#include "sysdep/archsetjmp.h"
#include "os.h"
=======
#include <sysdep/archsetjmp.h>
#include <os.h>
>>>>>>> refs/remotes/origin/master

extern int setjmp(jmp_buf);
extern void longjmp(jmp_buf, int);

#define UML_LONGJMP(buf, val) do { \
	longjmp(*buf, val);	\
} while(0)

#define UML_SETJMP(buf) ({ \
	int n;	   \
	volatile int enable;	\
	enable = get_signals(); \
	n = setjmp(*buf); \
	if(n != 0) \
		set_signals(enable); \
	n; })

#endif
