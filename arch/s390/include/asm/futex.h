#ifndef _ASM_S390_FUTEX_H
#define _ASM_S390_FUTEX_H

<<<<<<< HEAD
#ifdef __KERNEL__

=======
>>>>>>> refs/remotes/origin/master
#include <linux/futex.h>
#include <linux/uaccess.h>
#include <asm/errno.h>

static inline int futex_atomic_op_inuser (int encoded_op, u32 __user *uaddr)
{
	int op = (encoded_op >> 28) & 7;
	int cmp = (encoded_op >> 24) & 15;
	int oparg = (encoded_op << 8) >> 20;
	int cmparg = (encoded_op << 20) >> 20;
	int oldval, ret;

	if (encoded_op & (FUTEX_OP_OPARG_SHIFT << 28))
		oparg = 1 << oparg;

<<<<<<< HEAD
	if (! access_ok (VERIFY_WRITE, uaddr, sizeof(u32)))
		return -EFAULT;

=======
>>>>>>> refs/remotes/origin/master
	pagefault_disable();
	ret = uaccess.futex_atomic_op(op, uaddr, oparg, &oldval);
	pagefault_enable();

	if (!ret) {
		switch (cmp) {
		case FUTEX_OP_CMP_EQ: ret = (oldval == cmparg); break;
		case FUTEX_OP_CMP_NE: ret = (oldval != cmparg); break;
		case FUTEX_OP_CMP_LT: ret = (oldval < cmparg); break;
		case FUTEX_OP_CMP_GE: ret = (oldval >= cmparg); break;
		case FUTEX_OP_CMP_LE: ret = (oldval <= cmparg); break;
		case FUTEX_OP_CMP_GT: ret = (oldval > cmparg); break;
		default: ret = -ENOSYS;
		}
	}
	return ret;
}

static inline int futex_atomic_cmpxchg_inatomic(u32 *uval, u32 __user *uaddr,
						u32 oldval, u32 newval)
{
<<<<<<< HEAD
	if (! access_ok (VERIFY_WRITE, uaddr, sizeof(u32)))
		return -EFAULT;

	return uaccess.futex_atomic_cmpxchg(uval, uaddr, oldval, newval);
}

#endif /* __KERNEL__ */
=======
	return uaccess.futex_atomic_cmpxchg(uval, uaddr, oldval, newval);
}

>>>>>>> refs/remotes/origin/master
#endif /* _ASM_S390_FUTEX_H */
