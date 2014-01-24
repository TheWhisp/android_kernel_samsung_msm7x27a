/******************************************************************************
 * privcmd.c
 *
 * Interface to privileged domain-0 commands.
 *
 * Copyright (c) 2002-2004, K A Fraser, B Dragovic
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) "xen:" KBUILD_MODNAME ": " fmt

>>>>>>> refs/remotes/origin/master
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/uaccess.h>
#include <linux/swap.h>
#include <linux/highmem.h>
#include <linux/pagemap.h>
#include <linux/seq_file.h>
#include <linux/miscdevice.h>

#include <asm/pgalloc.h>
#include <asm/pgtable.h>
#include <asm/tlb.h>
#include <asm/xen/hypervisor.h>
#include <asm/xen/hypercall.h>

#include <xen/xen.h>
#include <xen/privcmd.h>
#include <xen/interface/xen.h>
#include <xen/features.h>
#include <xen/page.h>
#include <xen/xen-ops.h>
<<<<<<< HEAD
=======
#include <xen/balloon.h>
>>>>>>> refs/remotes/origin/master

#include "privcmd.h"

MODULE_LICENSE("GPL");

<<<<<<< HEAD
#ifndef HAVE_ARCH_PRIVCMD_MMAP
static int privcmd_enforce_singleshot_mapping(struct vm_area_struct *vma);
#endif
=======
#define PRIV_VMA_LOCKED ((void *)1)

static int privcmd_vma_range_is_mapped(
               struct vm_area_struct *vma,
               unsigned long addr,
               unsigned long nr_pages);
>>>>>>> refs/remotes/origin/master

static long privcmd_ioctl_hypercall(void __user *udata)
{
	struct privcmd_hypercall hypercall;
	long ret;

	if (copy_from_user(&hypercall, udata, sizeof(hypercall)))
		return -EFAULT;

	ret = privcmd_call(hypercall.op,
			   hypercall.arg[0], hypercall.arg[1],
			   hypercall.arg[2], hypercall.arg[3],
			   hypercall.arg[4]);

	return ret;
}

static void free_page_list(struct list_head *pages)
{
	struct page *p, *n;

	list_for_each_entry_safe(p, n, pages, lru)
		__free_page(p);

	INIT_LIST_HEAD(pages);
}

/*
 * Given an array of items in userspace, return a list of pages
 * containing the data.  If copying fails, either because of memory
 * allocation failure or a problem reading user memory, return an
 * error code; its up to the caller to dispose of any partial list.
 */
static int gather_array(struct list_head *pagelist,
			unsigned nelem, size_t size,
<<<<<<< HEAD
			void __user *data)
=======
			const void __user *data)
>>>>>>> refs/remotes/origin/master
{
	unsigned pageidx;
	void *pagedata;
	int ret;

	if (size > PAGE_SIZE)
		return 0;

	pageidx = PAGE_SIZE;
	pagedata = NULL;	/* quiet, gcc */
	while (nelem--) {
		if (pageidx > PAGE_SIZE-size) {
			struct page *page = alloc_page(GFP_KERNEL);

			ret = -ENOMEM;
			if (page == NULL)
				goto fail;

			pagedata = page_address(page);

			list_add_tail(&page->lru, pagelist);
			pageidx = 0;
		}

		ret = -EFAULT;
		if (copy_from_user(pagedata + pageidx, data, size))
			goto fail;

		data += size;
		pageidx += size;
	}

	ret = 0;

fail:
	return ret;
}

/*
 * Call function "fn" on each element of the array fragmented
 * over a list of pages.
 */
static int traverse_pages(unsigned nelem, size_t size,
			  struct list_head *pos,
			  int (*fn)(void *data, void *state),
			  void *state)
{
	void *pagedata;
	unsigned pageidx;
	int ret = 0;

	BUG_ON(size > PAGE_SIZE);

	pageidx = PAGE_SIZE;
	pagedata = NULL;	/* hush, gcc */

	while (nelem--) {
		if (pageidx > PAGE_SIZE-size) {
			struct page *page;
			pos = pos->next;
			page = list_entry(pos, struct page, lru);
			pagedata = page_address(page);
			pageidx = 0;
		}

		ret = (*fn)(pagedata + pageidx, state);
		if (ret)
			break;
		pageidx += size;
	}

	return ret;
}

struct mmap_mfn_state {
	unsigned long va;
	struct vm_area_struct *vma;
	domid_t domain;
};

static int mmap_mfn_range(void *data, void *state)
{
	struct privcmd_mmap_entry *msg = data;
	struct mmap_mfn_state *st = state;
	struct vm_area_struct *vma = st->vma;
	int rc;

	/* Do not allow range to wrap the address space. */
	if ((msg->npages > (LONG_MAX >> PAGE_SHIFT)) ||
	    ((unsigned long)(msg->npages << PAGE_SHIFT) >= -st->va))
		return -EINVAL;

	/* Range chunks must be contiguous in va space. */
	if ((msg->va != st->va) ||
	    ((msg->va+(msg->npages<<PAGE_SHIFT)) > vma->vm_end))
		return -EINVAL;

	rc = xen_remap_domain_mfn_range(vma,
					msg->va & PAGE_MASK,
					msg->mfn, msg->npages,
					vma->vm_page_prot,
<<<<<<< HEAD
					st->domain);
=======
					st->domain, NULL);
>>>>>>> refs/remotes/origin/master
	if (rc < 0)
		return rc;

	st->va += msg->npages << PAGE_SHIFT;

	return 0;
}

static long privcmd_ioctl_mmap(void __user *udata)
{
	struct privcmd_mmap mmapcmd;
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	int rc;
	LIST_HEAD(pagelist);
	struct mmap_mfn_state state;

<<<<<<< HEAD
	if (!xen_initial_domain())
		return -EPERM;
=======
	/* We only support privcmd_ioctl_mmap_batch for auto translated. */
	if (xen_feature(XENFEAT_auto_translated_physmap))
		return -ENOSYS;
>>>>>>> refs/remotes/origin/master

	if (copy_from_user(&mmapcmd, udata, sizeof(mmapcmd)))
		return -EFAULT;

	rc = gather_array(&pagelist,
			  mmapcmd.num, sizeof(struct privcmd_mmap_entry),
			  mmapcmd.entry);

	if (rc || list_empty(&pagelist))
		goto out;

	down_write(&mm->mmap_sem);

	{
		struct page *page = list_first_entry(&pagelist,
						     struct page, lru);
		struct privcmd_mmap_entry *msg = page_address(page);

		vma = find_vma(mm, msg->va);
		rc = -EINVAL;

<<<<<<< HEAD
		if (!vma || (msg->va != vma->vm_start) ||
		    !privcmd_enforce_singleshot_mapping(vma))
			goto out_up;
=======
		if (!vma || (msg->va != vma->vm_start) || vma->vm_private_data)
			goto out_up;
		vma->vm_private_data = PRIV_VMA_LOCKED;
>>>>>>> refs/remotes/origin/master
	}

	state.va = vma->vm_start;
	state.vma = vma;
	state.domain = mmapcmd.dom;

	rc = traverse_pages(mmapcmd.num, sizeof(struct privcmd_mmap_entry),
			    &pagelist,
			    mmap_mfn_range, &state);


out_up:
	up_write(&mm->mmap_sem);

out:
	free_page_list(&pagelist);

	return rc;
}

struct mmap_batch_state {
	domid_t domain;
	unsigned long va;
	struct vm_area_struct *vma;
<<<<<<< HEAD
	int err;

	xen_pfn_t __user *user;
};

=======
	int index;
	/* A tristate:
	 *      0 for no errors
	 *      1 if at least one error has happened (and no
	 *          -ENOENT errors have happened)
	 *      -ENOENT if at least 1 -ENOENT has happened.
	 */
	int global_error;
	int version;

	/* User-space mfn array to store errors in the second pass for V1. */
	xen_pfn_t __user *user_mfn;
	/* User-space int array to store errors in the second pass for V2. */
	int __user *user_err;
};

/* auto translated dom0 note: if domU being created is PV, then mfn is
 * mfn(addr on bus). If it's auto xlated, then mfn is pfn (input to HAP).
 */
>>>>>>> refs/remotes/origin/master
static int mmap_batch_fn(void *data, void *state)
{
	xen_pfn_t *mfnp = data;
	struct mmap_batch_state *st = state;
<<<<<<< HEAD

	if (xen_remap_domain_mfn_range(st->vma, st->va & PAGE_MASK, *mfnp, 1,
				       st->vma->vm_page_prot, st->domain) < 0) {
		*mfnp |= 0xf0000000U;
		st->err++;
=======
	struct vm_area_struct *vma = st->vma;
	struct page **pages = vma->vm_private_data;
	struct page *cur_page = NULL;
	int ret;

	if (xen_feature(XENFEAT_auto_translated_physmap))
		cur_page = pages[st->index++];

	ret = xen_remap_domain_mfn_range(st->vma, st->va & PAGE_MASK, *mfnp, 1,
					 st->vma->vm_page_prot, st->domain,
					 &cur_page);

	/* Store error code for second pass. */
	if (st->version == 1) {
		if (ret < 0) {
			/*
			 * V1 encodes the error codes in the 32bit top nibble of the
			 * mfn (with its known limitations vis-a-vis 64 bit callers).
			 */
			*mfnp |= (ret == -ENOENT) ?
						PRIVCMD_MMAPBATCH_PAGED_ERROR :
						PRIVCMD_MMAPBATCH_MFN_ERROR;
		}
	} else { /* st->version == 2 */
		*((int *) mfnp) = ret;
	}

	/* And see if it affects the global_error. */
	if (ret < 0) {
		if (ret == -ENOENT)
			st->global_error = -ENOENT;
		else {
			/* Record that at least one error has happened. */
			if (st->global_error == 0)
				st->global_error = 1;
		}
>>>>>>> refs/remotes/origin/master
	}
	st->va += PAGE_SIZE;

	return 0;
}

static int mmap_return_errors(void *data, void *state)
{
<<<<<<< HEAD
	xen_pfn_t *mfnp = data;
	struct mmap_batch_state *st = state;

	return put_user(*mfnp, st->user++);
=======
	struct mmap_batch_state *st = state;

	if (st->version == 1) {
		xen_pfn_t mfnp = *((xen_pfn_t *) data);
		if (mfnp & PRIVCMD_MMAPBATCH_MFN_ERROR)
			return __put_user(mfnp, st->user_mfn++);
		else
			st->user_mfn++;
	} else { /* st->version == 2 */
		int err = *((int *) data);
		if (err)
			return __put_user(err, st->user_err++);
		else
			st->user_err++;
	}

	return 0;
}

/* Allocate pfns that are then mapped with gmfns from foreign domid. Update
 * the vma with the page info to use later.
 * Returns: 0 if success, otherwise -errno
 */
static int alloc_empty_pages(struct vm_area_struct *vma, int numpgs)
{
	int rc;
	struct page **pages;

	pages = kcalloc(numpgs, sizeof(pages[0]), GFP_KERNEL);
	if (pages == NULL)
		return -ENOMEM;

	rc = alloc_xenballooned_pages(numpgs, pages, 0);
	if (rc != 0) {
		pr_warn("%s Could not alloc %d pfns rc:%d\n", __func__,
			numpgs, rc);
		kfree(pages);
		return -ENOMEM;
	}
	BUG_ON(vma->vm_private_data != NULL);
	vma->vm_private_data = pages;

	return 0;
>>>>>>> refs/remotes/origin/master
}

static struct vm_operations_struct privcmd_vm_ops;

<<<<<<< HEAD
static long privcmd_ioctl_mmap_batch(void __user *udata)
{
	int ret;
	struct privcmd_mmapbatch m;
=======
static long privcmd_ioctl_mmap_batch(void __user *udata, int version)
{
	int ret;
	struct privcmd_mmapbatch_v2 m;
>>>>>>> refs/remotes/origin/master
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	unsigned long nr_pages;
	LIST_HEAD(pagelist);
	struct mmap_batch_state state;

<<<<<<< HEAD
	if (!xen_initial_domain())
		return -EPERM;

	if (copy_from_user(&m, udata, sizeof(m)))
		return -EFAULT;
=======
	switch (version) {
	case 1:
		if (copy_from_user(&m, udata, sizeof(struct privcmd_mmapbatch)))
			return -EFAULT;
		/* Returns per-frame error in m.arr. */
		m.err = NULL;
		if (!access_ok(VERIFY_WRITE, m.arr, m.num * sizeof(*m.arr)))
			return -EFAULT;
		break;
	case 2:
		if (copy_from_user(&m, udata, sizeof(struct privcmd_mmapbatch_v2)))
			return -EFAULT;
		/* Returns per-frame error code in m.err. */
		if (!access_ok(VERIFY_WRITE, m.err, m.num * (sizeof(*m.err))))
			return -EFAULT;
		break;
	default:
		return -EINVAL;
	}
>>>>>>> refs/remotes/origin/master

	nr_pages = m.num;
	if ((m.num <= 0) || (nr_pages > (LONG_MAX >> PAGE_SHIFT)))
		return -EINVAL;

<<<<<<< HEAD
	ret = gather_array(&pagelist, m.num, sizeof(xen_pfn_t),
			   m.arr);

	if (ret || list_empty(&pagelist))
		goto out;
=======
	ret = gather_array(&pagelist, m.num, sizeof(xen_pfn_t), m.arr);

	if (ret)
		goto out;
	if (list_empty(&pagelist)) {
		ret = -EINVAL;
		goto out;
	}

	if (version == 2) {
		/* Zero error array now to only copy back actual errors. */
		if (clear_user(m.err, sizeof(int) * m.num)) {
			ret = -EFAULT;
			goto out;
		}
	}
>>>>>>> refs/remotes/origin/master

	down_write(&mm->mmap_sem);

	vma = find_vma(mm, m.addr);
<<<<<<< HEAD
	ret = -EINVAL;
	if (!vma ||
	    vma->vm_ops != &privcmd_vm_ops ||
	    (m.addr != vma->vm_start) ||
	    ((m.addr + (nr_pages << PAGE_SHIFT)) != vma->vm_end) ||
	    !privcmd_enforce_singleshot_mapping(vma)) {
		up_write(&mm->mmap_sem);
		goto out;
	}

	state.domain = m.dom;
	state.vma = vma;
	state.va = m.addr;
	state.err = 0;

	ret = traverse_pages(m.num, sizeof(xen_pfn_t),
			     &pagelist, mmap_batch_fn, &state);

	up_write(&mm->mmap_sem);

	if (state.err > 0) {
		state.user = m.arr;
		ret = traverse_pages(m.num, sizeof(xen_pfn_t),
			       &pagelist,
			       mmap_return_errors, &state);
	}

out:
	free_page_list(&pagelist);

	return ret;
=======
	if (!vma ||
	    vma->vm_ops != &privcmd_vm_ops) {
		ret = -EINVAL;
		goto out_unlock;
	}

	/*
	 * Caller must either:
	 *
	 * Map the whole VMA range, which will also allocate all the
	 * pages required for the auto_translated_physmap case.
	 *
	 * Or
	 *
	 * Map unmapped holes left from a previous map attempt (e.g.,
	 * because those foreign frames were previously paged out).
	 */
	if (vma->vm_private_data == NULL) {
		if (m.addr != vma->vm_start ||
		    m.addr + (nr_pages << PAGE_SHIFT) != vma->vm_end) {
			ret = -EINVAL;
			goto out_unlock;
		}
		if (xen_feature(XENFEAT_auto_translated_physmap)) {
			ret = alloc_empty_pages(vma, m.num);
			if (ret < 0)
				goto out_unlock;
		} else
			vma->vm_private_data = PRIV_VMA_LOCKED;
	} else {
		if (m.addr < vma->vm_start ||
		    m.addr + (nr_pages << PAGE_SHIFT) > vma->vm_end) {
			ret = -EINVAL;
			goto out_unlock;
		}
		if (privcmd_vma_range_is_mapped(vma, m.addr, nr_pages)) {
			ret = -EINVAL;
			goto out_unlock;
		}
	}

	state.domain        = m.dom;
	state.vma           = vma;
	state.va            = m.addr;
	state.index         = 0;
	state.global_error  = 0;
	state.version       = version;

	/* mmap_batch_fn guarantees ret == 0 */
	BUG_ON(traverse_pages(m.num, sizeof(xen_pfn_t),
			     &pagelist, mmap_batch_fn, &state));

	up_write(&mm->mmap_sem);

	if (state.global_error) {
		/* Write back errors in second pass. */
		state.user_mfn = (xen_pfn_t *)m.arr;
		state.user_err = m.err;
		ret = traverse_pages(m.num, sizeof(xen_pfn_t),
							 &pagelist, mmap_return_errors, &state);
	} else
		ret = 0;

	/* If we have not had any EFAULT-like global errors then set the global
	 * error to -ENOENT if necessary. */
	if ((ret == 0) && (state.global_error == -ENOENT))
		ret = -ENOENT;

out:
	free_page_list(&pagelist);
	return ret;

out_unlock:
	up_write(&mm->mmap_sem);
	goto out;
>>>>>>> refs/remotes/origin/master
}

static long privcmd_ioctl(struct file *file,
			  unsigned int cmd, unsigned long data)
{
	int ret = -ENOSYS;
	void __user *udata = (void __user *) data;

	switch (cmd) {
	case IOCTL_PRIVCMD_HYPERCALL:
		ret = privcmd_ioctl_hypercall(udata);
		break;

	case IOCTL_PRIVCMD_MMAP:
		ret = privcmd_ioctl_mmap(udata);
		break;

	case IOCTL_PRIVCMD_MMAPBATCH:
<<<<<<< HEAD
		ret = privcmd_ioctl_mmap_batch(udata);
=======
		ret = privcmd_ioctl_mmap_batch(udata, 1);
		break;

	case IOCTL_PRIVCMD_MMAPBATCH_V2:
		ret = privcmd_ioctl_mmap_batch(udata, 2);
>>>>>>> refs/remotes/origin/master
		break;

	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

<<<<<<< HEAD
=======
static void privcmd_close(struct vm_area_struct *vma)
{
	struct page **pages = vma->vm_private_data;
	int numpgs = (vma->vm_end - vma->vm_start) >> PAGE_SHIFT;
	int rc;

	if (!xen_feature(XENFEAT_auto_translated_physmap) || !numpgs || !pages)
		return;

	rc = xen_unmap_domain_mfn_range(vma, numpgs, pages);
	if (rc == 0)
		free_xenballooned_pages(numpgs, pages);
	else
		pr_crit("unable to unmap MFN range: leaking %d pages. rc=%d\n",
			numpgs, rc);
	kfree(pages);
}

>>>>>>> refs/remotes/origin/master
static int privcmd_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	printk(KERN_DEBUG "privcmd_fault: vma=%p %lx-%lx, pgoff=%lx, uv=%p\n",
	       vma, vma->vm_start, vma->vm_end,
	       vmf->pgoff, vmf->virtual_address);

	return VM_FAULT_SIGBUS;
}

static struct vm_operations_struct privcmd_vm_ops = {
<<<<<<< HEAD
=======
	.close = privcmd_close,
>>>>>>> refs/remotes/origin/master
	.fault = privcmd_fault
};

static int privcmd_mmap(struct file *file, struct vm_area_struct *vma)
{
<<<<<<< HEAD
	/* Unsupported for auto-translate guests. */
	if (xen_feature(XENFEAT_auto_translated_physmap))
		return -ENOSYS;

	/* DONTCOPY is essential for Xen because copy_page_range doesn't know
	 * how to recreate these mappings */
	vma->vm_flags |= VM_RESERVED | VM_IO | VM_DONTCOPY | VM_PFNMAP;
=======
	/* DONTCOPY is essential for Xen because copy_page_range doesn't know
	 * how to recreate these mappings */
	vma->vm_flags |= VM_IO | VM_PFNMAP | VM_DONTCOPY |
			 VM_DONTEXPAND | VM_DONTDUMP;
>>>>>>> refs/remotes/origin/master
	vma->vm_ops = &privcmd_vm_ops;
	vma->vm_private_data = NULL;

	return 0;
}

<<<<<<< HEAD
static int privcmd_enforce_singleshot_mapping(struct vm_area_struct *vma)
{
	return (xchg(&vma->vm_private_data, (void *)1) == NULL);
=======
/*
 * For MMAPBATCH*. This allows asserting the singleshot mapping
 * on a per pfn/pte basis. Mapping calls that fail with ENOENT
 * can be then retried until success.
 */
static int is_mapped_fn(pte_t *pte, struct page *pmd_page,
	                unsigned long addr, void *data)
{
	return pte_none(*pte) ? 0 : -EBUSY;
}

static int privcmd_vma_range_is_mapped(
	           struct vm_area_struct *vma,
	           unsigned long addr,
	           unsigned long nr_pages)
{
	return apply_to_page_range(vma->vm_mm, addr, nr_pages << PAGE_SHIFT,
				   is_mapped_fn, NULL) != 0;
>>>>>>> refs/remotes/origin/master
}

const struct file_operations xen_privcmd_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = privcmd_ioctl,
	.mmap = privcmd_mmap,
};
EXPORT_SYMBOL_GPL(xen_privcmd_fops);

static struct miscdevice privcmd_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "xen/privcmd",
	.fops = &xen_privcmd_fops,
};

static int __init privcmd_init(void)
{
	int err;

	if (!xen_domain())
		return -ENODEV;

	err = misc_register(&privcmd_dev);
	if (err != 0) {
<<<<<<< HEAD
		printk(KERN_ERR "Could not register Xen privcmd device\n");
=======
		pr_err("Could not register Xen privcmd device\n");
>>>>>>> refs/remotes/origin/master
		return err;
	}
	return 0;
}

static void __exit privcmd_exit(void)
{
	misc_deregister(&privcmd_dev);
}

module_init(privcmd_init);
module_exit(privcmd_exit);
