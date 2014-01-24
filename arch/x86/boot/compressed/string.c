#include "misc.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

int memcmp(const void *s1, const void *s2, size_t len)
{
	u8 diff;
	asm("repe; cmpsb; setnz %0"
	    : "=qm" (diff), "+D" (s1), "+S" (s2), "+c" (len));
	return diff;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include "../string.c"
