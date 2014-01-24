/*
<<<<<<< HEAD
 * gaccess.h -  access guest memory
=======
 * access.h -  access guest memory
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Copyright IBM Corp. 2008,2009
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
	}
	return 0;
}

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
