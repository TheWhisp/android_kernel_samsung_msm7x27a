/*
<<<<<<< HEAD
 *  include/asm-s390/mman.h
 *
=======
>>>>>>> refs/remotes/origin/master
 *  S390 version
 *
 *  Derived from "include/asm-i386/mman.h"
 */
<<<<<<< HEAD

#ifndef __S390_MMAN_H__
#define __S390_MMAN_H__

#include <asm-generic/mman.h>

<<<<<<< HEAD
#if defined(__KERNEL__) && !defined(__ASSEMBLY__) && defined(CONFIG_64BIT)
int s390_mmap_check(unsigned long addr, unsigned long len);
#define arch_mmap_check(addr,len,flags)	s390_mmap_check(addr,len)
#endif
=======
#if defined(__KERNEL__)
#if !defined(__ASSEMBLY__) && defined(CONFIG_64BIT)
int s390_mmap_check(unsigned long addr, unsigned long len);
#define arch_mmap_check(addr,len,flags)	s390_mmap_check(addr,len)
#endif
#endif
>>>>>>> refs/remotes/origin/cm-10.0

=======
#ifndef __S390_MMAN_H__
#define __S390_MMAN_H__

#include <uapi/asm/mman.h>

#if !defined(__ASSEMBLY__) && defined(CONFIG_64BIT)
int s390_mmap_check(unsigned long addr, unsigned long len, unsigned long flags);
#define arch_mmap_check(addr, len, flags) s390_mmap_check(addr, len, flags)
#endif
>>>>>>> refs/remotes/origin/master
#endif /* __S390_MMAN_H__ */
