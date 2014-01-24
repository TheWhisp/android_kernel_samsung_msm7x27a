/*
 * Copyright 2010 Tilera Corporation. All Rights Reserved.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *   NON INFRINGEMENT.  See the GNU General Public License for
 *   more details.
 *
 * These routines make two important assumptions:
 *
 * 1. atomic_t is really an int and can be freely cast back and forth
 *    (validated in __init_atomic_per_cpu).
 *
 * 2. userspace uses sys_cmpxchg() for all atomic operations, thus using
 *    the same locking convention that all the kernel atomic routines use.
 */

#ifndef _ASM_TILE_FUTEX_H
#define _ASM_TILE_FUTEX_H

#ifndef __ASSEMBLY__

#include <linux/futex.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
<<<<<<< HEAD

extern struct __get_user futex_set(u32 __user *v, int i);
extern struct __get_user futex_add(u32 __user *v, int n);
extern struct __get_user futex_or(u32 __user *v, int n);
extern struct __get_user futex_andn(u32 __user *v, int n);
extern struct __get_user futex_cmpxchg(u32 __user *v, int o, int n);

#ifndef __tilegx__
extern struct __get_user futex_xor(u32 __user *v, int n);
#else
static inline struct __get_user futex_xor(u32 __user *uaddr, int n)
{
	struct __get_user asm_ret = __get_user_4(uaddr);
	if (!asm_ret.err) {
		int oldval, newval;
		do {
			oldval = asm_ret.val;
			newval = oldval ^ n;
			asm_ret = futex_cmpxchg(uaddr, oldval, newval);
		} while (asm_ret.err == 0 && oldval != asm_ret.val);
	}
	return asm_ret;
}
=======
#include <asm/atomic.h>

/*
 * Support macros for futex operations.  Do not use these macros directly.
 * They assume "ret", "val", "oparg", and "uaddr" in the lexical context.
 * __futex_cmpxchg() additionally assumes "oldval".
 */

#ifdef __tilegx__

#define __futex_asm(OP) \
	asm("1: {" #OP " %1, %3, %4; movei %0, 0 }\n"		\
	    ".pushsection .fixup,\"ax\"\n"			\
	    "0: { movei %0, %5; j 9f }\n"			\
	    ".section __ex_table,\"a\"\n"			\
	    ".align 8\n"					\
	    ".quad 1b, 0b\n"					\
	    ".popsection\n"					\
	    "9:"						\
	    : "=r" (ret), "=r" (val), "+m" (*(uaddr))		\
	    : "r" (uaddr), "r" (oparg), "i" (-EFAULT))

#define __futex_set() __futex_asm(exch4)
#define __futex_add() __futex_asm(fetchadd4)
#define __futex_or() __futex_asm(fetchor4)
#define __futex_andn() ({ oparg = ~oparg; __futex_asm(fetchand4); })
#define __futex_cmpxchg() \
	({ __insn_mtspr(SPR_CMPEXCH_VALUE, oldval); __futex_asm(cmpexch4); })

#define __futex_xor()						\
	({							\
		u32 oldval, n = oparg;				\
		if ((ret = __get_user(oldval, uaddr)) == 0) {	\
			do {					\
				oparg = oldval ^ n;		\
				__futex_cmpxchg();		\
			} while (ret == 0 && oldval != val);	\
		}						\
	})

/* No need to prefetch, since the atomic ops go to the home cache anyway. */
#define __futex_prolog()

#else

#define __futex_call(FN)						\
	{								\
		struct __get_user gu = FN((u32 __force *)uaddr, lock, oparg); \
		val = gu.val;						\
		ret = gu.err;						\
	}

#define __futex_set() __futex_call(__atomic_xchg)
#define __futex_add() __futex_call(__atomic_xchg_add)
#define __futex_or() __futex_call(__atomic_or)
#define __futex_andn() __futex_call(__atomic_andn)
#define __futex_xor() __futex_call(__atomic_xor)

#define __futex_cmpxchg()						\
	{								\
		struct __get_user gu = __atomic_cmpxchg((u32 __force *)uaddr, \
							lock, oldval, oparg); \
		val = gu.val;						\
		ret = gu.err;						\
	}

/*
 * Find the lock pointer for the atomic calls to use, and issue a
 * prefetch to the user address to bring it into cache.  Similar to
 * __atomic_setup(), but we can't do a read into the L1 since it might
 * fault; instead we do a prefetch into the L2.
 */
#define __futex_prolog()					\
	int *lock;						\
	__insn_prefetch(uaddr);					\
	lock = __atomic_hashed_lock((int __force *)uaddr)
>>>>>>> refs/remotes/origin/master
#endif

static inline int futex_atomic_op_inuser(int encoded_op, u32 __user *uaddr)
{
	int op = (encoded_op >> 28) & 7;
	int cmp = (encoded_op >> 24) & 15;
	int oparg = (encoded_op << 8) >> 20;
	int cmparg = (encoded_op << 20) >> 20;
<<<<<<< HEAD
	int ret;
	struct __get_user asm_ret;
=======
	int uninitialized_var(val), ret;

	__futex_prolog();

	/* The 32-bit futex code makes this assumption, so validate it here. */
	BUILD_BUG_ON(sizeof(atomic_t) != sizeof(int));
>>>>>>> refs/remotes/origin/master

	if (encoded_op & (FUTEX_OP_OPARG_SHIFT << 28))
		oparg = 1 << oparg;

	if (!access_ok(VERIFY_WRITE, uaddr, sizeof(u32)))
		return -EFAULT;

	pagefault_disable();
	switch (op) {
	case FUTEX_OP_SET:
<<<<<<< HEAD
		asm_ret = futex_set(uaddr, oparg);
		break;
	case FUTEX_OP_ADD:
		asm_ret = futex_add(uaddr, oparg);
		break;
	case FUTEX_OP_OR:
		asm_ret = futex_or(uaddr, oparg);
		break;
	case FUTEX_OP_ANDN:
		asm_ret = futex_andn(uaddr, oparg);
		break;
	case FUTEX_OP_XOR:
		asm_ret = futex_xor(uaddr, oparg);
		break;
	default:
		asm_ret.err = -ENOSYS;
	}
	pagefault_enable();

	ret = asm_ret.err;

	if (!ret) {
		switch (cmp) {
		case FUTEX_OP_CMP_EQ:
			ret = (asm_ret.val == cmparg);
			break;
		case FUTEX_OP_CMP_NE:
			ret = (asm_ret.val != cmparg);
			break;
		case FUTEX_OP_CMP_LT:
			ret = (asm_ret.val < cmparg);
			break;
		case FUTEX_OP_CMP_GE:
			ret = (asm_ret.val >= cmparg);
			break;
		case FUTEX_OP_CMP_LE:
			ret = (asm_ret.val <= cmparg);
			break;
		case FUTEX_OP_CMP_GT:
			ret = (asm_ret.val > cmparg);
=======
		__futex_set();
		break;
	case FUTEX_OP_ADD:
		__futex_add();
		break;
	case FUTEX_OP_OR:
		__futex_or();
		break;
	case FUTEX_OP_ANDN:
		__futex_andn();
		break;
	case FUTEX_OP_XOR:
		__futex_xor();
		break;
	default:
		ret = -ENOSYS;
		break;
	}
	pagefault_enable();

	if (!ret) {
		switch (cmp) {
		case FUTEX_OP_CMP_EQ:
			ret = (val == cmparg);
			break;
		case FUTEX_OP_CMP_NE:
			ret = (val != cmparg);
			break;
		case FUTEX_OP_CMP_LT:
			ret = (val < cmparg);
			break;
		case FUTEX_OP_CMP_GE:
			ret = (val >= cmparg);
			break;
		case FUTEX_OP_CMP_LE:
			ret = (val <= cmparg);
			break;
		case FUTEX_OP_CMP_GT:
			ret = (val > cmparg);
>>>>>>> refs/remotes/origin/master
			break;
		default:
			ret = -ENOSYS;
		}
	}
	return ret;
}

static inline int futex_atomic_cmpxchg_inatomic(u32 *uval, u32 __user *uaddr,
<<<<<<< HEAD
						u32 oldval, u32 newval)
{
	struct __get_user asm_ret;
=======
						u32 oldval, u32 oparg)
{
	int ret, val;

	__futex_prolog();
>>>>>>> refs/remotes/origin/master

	if (!access_ok(VERIFY_WRITE, uaddr, sizeof(u32)))
		return -EFAULT;

<<<<<<< HEAD
	asm_ret = futex_cmpxchg(uaddr, oldval, newval);
	*uval = asm_ret.val;
	return asm_ret.err;
}

#ifndef __tilegx__
/* Return failure from the atomic wrappers. */
struct __get_user __atomic_bad_address(int __user *addr);
#endif
=======
	__futex_cmpxchg();

	*uval = val;
	return ret;
}
>>>>>>> refs/remotes/origin/master

#endif /* !__ASSEMBLY__ */

#endif /* _ASM_TILE_FUTEX_H */
