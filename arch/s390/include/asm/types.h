/*
<<<<<<< HEAD
 *  include/asm-s390/types.h
 *
=======
>>>>>>> refs/remotes/origin/master
 *  S390 version
 *
 *  Derived from "include/asm-i386/types.h"
 */
<<<<<<< HEAD

#ifndef _S390_TYPES_H
#define _S390_TYPES_H

#include <asm-generic/int-ll64.h>

#ifndef __ASSEMBLY__

<<<<<<< HEAD
typedef unsigned short umode_t;

=======
>>>>>>> refs/remotes/origin/cm-10.0
/* A address type so that arithmetic can be done on it & it can be upgraded to
   64 bit when necessary 
*/
typedef unsigned long addr_t; 
typedef __signed__ long saddr_t;

#endif /* __ASSEMBLY__ */
=======
#ifndef _S390_TYPES_H
#define _S390_TYPES_H

#include <uapi/asm/types.h>
>>>>>>> refs/remotes/origin/master

/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
<<<<<<< HEAD
#ifdef __KERNEL__

#ifndef __ASSEMBLY__

#ifndef __s390x__
=======

#ifndef __ASSEMBLY__

#ifndef CONFIG_64BIT
>>>>>>> refs/remotes/origin/master
typedef union {
	unsigned long long pair;
	struct {
		unsigned long even;
		unsigned long odd;
	} subreg;
} register_pair;

<<<<<<< HEAD
#endif /* ! __s390x__   */
#endif /* __ASSEMBLY__  */
#endif /* __KERNEL__    */
=======
#endif /* ! CONFIG_64BIT   */
#endif /* __ASSEMBLY__  */
>>>>>>> refs/remotes/origin/master
#endif /* _S390_TYPES_H */
