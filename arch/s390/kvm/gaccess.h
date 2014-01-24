/*
<<<<<<< HEAD
<<<<<<< HEAD
 * gaccess.h -  access guest memory
=======
 * access.h -  access guest memory
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Copyright IBM Corp. 2008,2009
=======
 * access guest memory
 *
 * Copyright IBM Corp. 2008, 2009
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (version 2 only)
 * as published by the Free Software Foundation.
 *
 *    Author(s): Carsten Otte <cotte@de.ibm.com>
 */

#ifndef __KVM_S390_GACCESS_H
#define __KVM_S390_GACCESS_H

#include <linux/compiler.h>
#include <linux/kvm_host.h>
#include <asm/uaccess.h>
#include "kvm-s390.h"

<<<<<<< HEAD
static inline void __user *__guestaddr_to_user(struct kvm_vcpu *vcpu,
					       unsigned long guestaddr)
{
	unsigned long prefix  = vcpu->arch.sie_block->prefix;
<<<<<<< HEAD
	unsigned long origin  = vcpu->arch.sie_block->gmsor;
	unsigned long memsize = kvm_s390_vcpu_get_memsize(vcpu);
=======
>>>>>>> refs/remotes/origin/cm-10.0

	if (guestaddr < 2 * PAGE_SIZE)
		guestaddr += prefix;
	else if ((guestaddr >= prefix) && (guestaddr < prefix + 2 * PAGE_SIZE))
		guestaddr -= prefix;

<<<<<<< HEAD
	if (guestaddr > memsize)
		return (void __user __force *) ERR_PTR(-EFAULT);

	guestaddr += origin;

	return (void __user *) guestaddr;
=======
	return (void __user *) gmap_fault(guestaddr, vcpu->arch.gmap);
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline int get_guest_u64(struct kvm_vcpu *vcpu, unsigned long guestaddr,
				u64 *result)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	BUG_ON(guestaddr & 7);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return get_user(*result, (unsigned long __user *) uptr);
}

static inline int get_guest_u32(struct kvm_vcpu *vcpu, unsigned long guestaddr,
				u32 *result)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	BUG_ON(guestaddr & 3);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return get_user(*result, (u32 __user *) uptr);
}

static inline int get_guest_u16(struct kvm_vcpu *vcpu, unsigned long guestaddr,
				u16 *result)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	BUG_ON(guestaddr & 1);

	if (IS_ERR(uptr))
		return PTR_ERR(uptr);

	return get_user(*result, (u16 __user *) uptr);
}

static inline int get_guest_u8(struct kvm_vcpu *vcpu, unsigned long guestaddr,
			       u8 *result)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return get_user(*result, (u8 __user *) uptr);
}

static inline int put_guest_u64(struct kvm_vcpu *vcpu, unsigned long guestaddr,
				u64 value)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	BUG_ON(guestaddr & 7);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return put_user(value, (u64 __user *) uptr);
}

static inline int put_guest_u32(struct kvm_vcpu *vcpu, unsigned long guestaddr,
				u32 value)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	BUG_ON(guestaddr & 3);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return put_user(value, (u32 __user *) uptr);
}

static inline int put_guest_u16(struct kvm_vcpu *vcpu, unsigned long guestaddr,
				u16 value)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	BUG_ON(guestaddr & 1);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return put_user(value, (u16 __user *) uptr);
}

static inline int put_guest_u8(struct kvm_vcpu *vcpu, unsigned long guestaddr,
			       u8 value)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return put_user(value, (u8 __user *) uptr);
}


static inline int __copy_to_guest_slow(struct kvm_vcpu *vcpu,
				       unsigned long guestdest,
<<<<<<< HEAD
				       const void *from, unsigned long n)
{
	int rc;
	unsigned long i;
	const u8 *data = from;
=======
				       void *from, unsigned long n)
{
	int rc;
	unsigned long i;
	u8 *data = from;
>>>>>>> refs/remotes/origin/cm-10.0

	for (i = 0; i < n; i++) {
		rc = put_guest_u8(vcpu, guestdest++, *(data++));
		if (rc < 0)
			return rc;
	}
	return 0;
}

<<<<<<< HEAD
static inline int copy_to_guest(struct kvm_vcpu *vcpu, unsigned long guestdest,
				const void *from, unsigned long n)
{
	unsigned long prefix  = vcpu->arch.sie_block->prefix;
	unsigned long origin  = vcpu->arch.sie_block->gmsor;
	unsigned long memsize = kvm_s390_vcpu_get_memsize(vcpu);
=======
static inline int __copy_to_guest_fast(struct kvm_vcpu *vcpu,
				       unsigned long guestdest,
				       void *from, unsigned long n)
{
	int r;
	void __user *uptr;
	unsigned long size;

	if (guestdest + n < guestdest)
		return -EFAULT;

	/* simple case: all within one segment table entry? */
	if ((guestdest & PMD_MASK) == ((guestdest+n) & PMD_MASK)) {
		uptr = (void __user *) gmap_fault(guestdest, vcpu->arch.gmap);

		if (IS_ERR((void __force *) uptr))
			return PTR_ERR((void __force *) uptr);

		r = copy_to_user(uptr, from, n);

		if (r)
			r = -EFAULT;

		goto out;
	}

	/* copy first segment */
	uptr = (void __user *)gmap_fault(guestdest, vcpu->arch.gmap);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	size = PMD_SIZE - (guestdest & ~PMD_MASK);

	r = copy_to_user(uptr, from, size);

	if (r) {
		r = -EFAULT;
		goto out;
	}
	from += size;
	n -= size;
	guestdest += size;

	/* copy full segments */
	while (n >= PMD_SIZE) {
		uptr = (void __user *)gmap_fault(guestdest, vcpu->arch.gmap);

		if (IS_ERR((void __force *) uptr))
			return PTR_ERR((void __force *) uptr);

		r = copy_to_user(uptr, from, PMD_SIZE);

		if (r) {
			r = -EFAULT;
			goto out;
		}
		from += PMD_SIZE;
		n -= PMD_SIZE;
		guestdest += PMD_SIZE;
	}

	/* copy the tail segment */
	if (n) {
		uptr = (void __user *)gmap_fault(guestdest, vcpu->arch.gmap);

		if (IS_ERR((void __force *) uptr))
			return PTR_ERR((void __force *) uptr);

		r = copy_to_user(uptr, from, n);

		if (r)
			r = -EFAULT;
	}
out:
	return r;
}

static inline int copy_to_guest_absolute(struct kvm_vcpu *vcpu,
					 unsigned long guestdest,
					 void *from, unsigned long n)
{
	return __copy_to_guest_fast(vcpu, guestdest, from, n);
}

static inline int copy_to_guest(struct kvm_vcpu *vcpu, unsigned long guestdest,
				void *from, unsigned long n)
{
	unsigned long prefix  = vcpu->arch.sie_block->prefix;
>>>>>>> refs/remotes/origin/cm-10.0

	if ((guestdest < 2 * PAGE_SIZE) && (guestdest + n > 2 * PAGE_SIZE))
		goto slowpath;

	if ((guestdest < prefix) && (guestdest + n > prefix))
		goto slowpath;

	if ((guestdest < prefix + 2 * PAGE_SIZE)
	    && (guestdest + n > prefix + 2 * PAGE_SIZE))
		goto slowpath;

	if (guestdest < 2 * PAGE_SIZE)
		guestdest += prefix;
	else if ((guestdest >= prefix) && (guestdest < prefix + 2 * PAGE_SIZE))
		guestdest -= prefix;

<<<<<<< HEAD
	if (guestdest + n > memsize)
		return -EFAULT;

	if (guestdest + n < guestdest)
		return -EFAULT;

	guestdest += origin;

	return copy_to_user((void __user *) guestdest, from, n);
=======
	return __copy_to_guest_fast(vcpu, guestdest, from, n);
>>>>>>> refs/remotes/origin/cm-10.0
slowpath:
	return __copy_to_guest_slow(vcpu, guestdest, from, n);
}

static inline int __copy_from_guest_slow(struct kvm_vcpu *vcpu, void *to,
					 unsigned long guestsrc,
					 unsigned long n)
{
	int rc;
	unsigned long i;
	u8 *data = to;

	for (i = 0; i < n; i++) {
		rc = get_guest_u8(vcpu, guestsrc++, data++);
		if (rc < 0)
			return rc;
=======
/* Convert real to absolute address by applying the prefix of the CPU */
static inline unsigned long kvm_s390_real_to_abs(struct kvm_vcpu *vcpu,
						 unsigned long gaddr)
{
	unsigned long prefix  = vcpu->arch.sie_block->prefix;
	if (gaddr < 2 * PAGE_SIZE)
		gaddr += prefix;
	else if (gaddr >= prefix && gaddr < prefix + 2 * PAGE_SIZE)
		gaddr -= prefix;
	return gaddr;
}

static inline void __user *__gptr_to_uptr(struct kvm_vcpu *vcpu,
					  void __user *gptr,
					  int prefixing)
{
	unsigned long gaddr = (unsigned long) gptr;
	unsigned long uaddr;

	if (prefixing)
		gaddr = kvm_s390_real_to_abs(vcpu, gaddr);
	uaddr = gmap_fault(gaddr, vcpu->arch.gmap);
	if (IS_ERR_VALUE(uaddr))
		uaddr = -EFAULT;
	return (void __user *)uaddr;
}

#define get_guest(vcpu, x, gptr)				\
({								\
	__typeof__(gptr) __uptr = __gptr_to_uptr(vcpu, gptr, 1);\
	int __mask = sizeof(__typeof__(*(gptr))) - 1;		\
	int __ret;						\
								\
	if (IS_ERR((void __force *)__uptr)) {			\
		__ret = PTR_ERR((void __force *)__uptr);	\
	} else {						\
		BUG_ON((unsigned long)__uptr & __mask);		\
		__ret = get_user(x, __uptr);			\
	}							\
	__ret;							\
})

#define put_guest(vcpu, x, gptr)				\
({								\
	__typeof__(gptr) __uptr = __gptr_to_uptr(vcpu, gptr, 1);\
	int __mask = sizeof(__typeof__(*(gptr))) - 1;		\
	int __ret;						\
								\
	if (IS_ERR((void __force *)__uptr)) {			\
		__ret = PTR_ERR((void __force *)__uptr);	\
	} else {						\
		BUG_ON((unsigned long)__uptr & __mask);		\
		__ret = put_user(x, __uptr);			\
	}							\
	__ret;							\
})

static inline int __copy_guest(struct kvm_vcpu *vcpu, unsigned long to,
			       unsigned long from, unsigned long len,
			       int to_guest, int prefixing)
{
	unsigned long _len, rc;
	void __user *uptr;

	while (len) {
		uptr = to_guest ? (void __user *)to : (void __user *)from;
		uptr = __gptr_to_uptr(vcpu, uptr, prefixing);
		if (IS_ERR((void __force *)uptr))
			return -EFAULT;
		_len = PAGE_SIZE - ((unsigned long)uptr & (PAGE_SIZE - 1));
		_len = min(_len, len);
		if (to_guest)
			rc = copy_to_user((void __user *) uptr, (void *)from, _len);
		else
			rc = copy_from_user((void *)to, (void __user *)uptr, _len);
		if (rc)
			return -EFAULT;
		len -= _len;
		from += _len;
		to += _len;
>>>>>>> refs/remotes/origin/master
	}
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int copy_from_guest(struct kvm_vcpu *vcpu, void *to,
				  unsigned long guestsrc, unsigned long n)
{
	unsigned long prefix  = vcpu->arch.sie_block->prefix;
	unsigned long origin  = vcpu->arch.sie_block->gmsor;
	unsigned long memsize = kvm_s390_vcpu_get_memsize(vcpu);

	if ((guestsrc < 2 * PAGE_SIZE) && (guestsrc + n > 2 * PAGE_SIZE))
		goto slowpath;

	if ((guestsrc < prefix) && (guestsrc + n > prefix))
		goto slowpath;

	if ((guestsrc < prefix + 2 * PAGE_SIZE)
	    && (guestsrc + n > prefix + 2 * PAGE_SIZE))
		goto slowpath;

	if (guestsrc < 2 * PAGE_SIZE)
		guestsrc += prefix;
	else if ((guestsrc >= prefix) && (guestsrc < prefix + 2 * PAGE_SIZE))
		guestsrc -= prefix;

	if (guestsrc + n > memsize)
		return -EFAULT;

	if (guestsrc + n < guestsrc)
		return -EFAULT;

	guestsrc += origin;

	return copy_from_user(to, (void __user *) guestsrc, n);
slowpath:
	return __copy_from_guest_slow(vcpu, to, guestsrc, n);
}

static inline int copy_to_guest_absolute(struct kvm_vcpu *vcpu,
					 unsigned long guestdest,
					 const void *from, unsigned long n)
{
	unsigned long origin  = vcpu->arch.sie_block->gmsor;
	unsigned long memsize = kvm_s390_vcpu_get_memsize(vcpu);

	if (guestdest + n > memsize)
		return -EFAULT;

	if (guestdest + n < guestdest)
		return -EFAULT;

	guestdest += origin;

	return copy_to_user((void __user *) guestdest, from, n);
=======
static inline int __copy_from_guest_fast(struct kvm_vcpu *vcpu, void *to,
					 unsigned long guestsrc,
					 unsigned long n)
{
	int r;
	void __user *uptr;
	unsigned long size;

	if (guestsrc + n < guestsrc)
		return -EFAULT;

	/* simple case: all within one segment table entry? */
	if ((guestsrc & PMD_MASK) == ((guestsrc+n) & PMD_MASK)) {
		uptr = (void __user *) gmap_fault(guestsrc, vcpu->arch.gmap);

		if (IS_ERR((void __force *) uptr))
			return PTR_ERR((void __force *) uptr);

		r = copy_from_user(to, uptr, n);

		if (r)
			r = -EFAULT;

		goto out;
	}

	/* copy first segment */
	uptr = (void __user *)gmap_fault(guestsrc, vcpu->arch.gmap);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	size = PMD_SIZE - (guestsrc & ~PMD_MASK);

	r = copy_from_user(to, uptr, size);

	if (r) {
		r = -EFAULT;
		goto out;
	}
	to += size;
	n -= size;
	guestsrc += size;

	/* copy full segments */
	while (n >= PMD_SIZE) {
		uptr = (void __user *)gmap_fault(guestsrc, vcpu->arch.gmap);

		if (IS_ERR((void __force *) uptr))
			return PTR_ERR((void __force *) uptr);

		r = copy_from_user(to, uptr, PMD_SIZE);

		if (r) {
			r = -EFAULT;
			goto out;
		}
		to += PMD_SIZE;
		n -= PMD_SIZE;
		guestsrc += PMD_SIZE;
	}

	/* copy the tail segment */
	if (n) {
		uptr = (void __user *)gmap_fault(guestsrc, vcpu->arch.gmap);

		if (IS_ERR((void __force *) uptr))
			return PTR_ERR((void __force *) uptr);

		r = copy_from_user(to, uptr, n);

		if (r)
			r = -EFAULT;
	}
out:
	return r;
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline int copy_from_guest_absolute(struct kvm_vcpu *vcpu, void *to,
					   unsigned long guestsrc,
					   unsigned long n)
{
<<<<<<< HEAD
	unsigned long origin  = vcpu->arch.sie_block->gmsor;
	unsigned long memsize = kvm_s390_vcpu_get_memsize(vcpu);

	if (guestsrc + n > memsize)
		return -EFAULT;

	if (guestsrc + n < guestsrc)
		return -EFAULT;

	guestsrc += origin;

	return copy_from_user(to, (void __user *) guestsrc, n);
=======
	return __copy_from_guest_fast(vcpu, to, guestsrc, n);
}

static inline int copy_from_guest(struct kvm_vcpu *vcpu, void *to,
				  unsigned long guestsrc, unsigned long n)
{
	unsigned long prefix  = vcpu->arch.sie_block->prefix;

	if ((guestsrc < 2 * PAGE_SIZE) && (guestsrc + n > 2 * PAGE_SIZE))
		goto slowpath;

	if ((guestsrc < prefix) && (guestsrc + n > prefix))
		goto slowpath;

	if ((guestsrc < prefix + 2 * PAGE_SIZE)
	    && (guestsrc + n > prefix + 2 * PAGE_SIZE))
		goto slowpath;

	if (guestsrc < 2 * PAGE_SIZE)
		guestsrc += prefix;
	else if ((guestsrc >= prefix) && (guestsrc < prefix + 2 * PAGE_SIZE))
		guestsrc -= prefix;

	return __copy_from_guest_fast(vcpu, to, guestsrc, n);
slowpath:
	return __copy_from_guest_slow(vcpu, to, guestsrc, n);
>>>>>>> refs/remotes/origin/cm-10.0
}
#endif
=======
#define copy_to_guest(vcpu, to, from, size) \
	__copy_guest(vcpu, to, (unsigned long)from, size, 1, 1)
#define copy_from_guest(vcpu, to, from, size) \
	__copy_guest(vcpu, (unsigned long)to, from, size, 0, 1)
#define copy_to_guest_absolute(vcpu, to, from, size) \
	__copy_guest(vcpu, to, (unsigned long)from, size, 1, 0)
#define copy_from_guest_absolute(vcpu, to, from, size) \
	__copy_guest(vcpu, (unsigned long)to, from, size, 0, 0)

#endif /* __KVM_S390_GACCESS_H */
>>>>>>> refs/remotes/origin/master
