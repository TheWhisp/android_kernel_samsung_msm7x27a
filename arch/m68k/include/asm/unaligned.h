#ifndef _ASM_M68K_UNALIGNED_H
#define _ASM_M68K_UNALIGNED_H


<<<<<<< HEAD
#ifdef CONFIG_COLDFIRE
=======
#ifdef CONFIG_CPU_HAS_NO_UNALIGNED
>>>>>>> refs/remotes/origin/master
#include <linux/unaligned/be_struct.h>
#include <linux/unaligned/le_byteshift.h>
#include <linux/unaligned/generic.h>

#define get_unaligned	__get_unaligned_be
#define put_unaligned	__put_unaligned_be

#else
/*
<<<<<<< HEAD
 * The m68k can do unaligned accesses itself. 
=======
 * The m68k can do unaligned accesses itself.
>>>>>>> refs/remotes/origin/master
 */
#include <linux/unaligned/access_ok.h>
#include <linux/unaligned/generic.h>

#define get_unaligned	__get_unaligned_be
#define put_unaligned	__put_unaligned_be

#endif

#endif /* _ASM_M68K_UNALIGNED_H */
