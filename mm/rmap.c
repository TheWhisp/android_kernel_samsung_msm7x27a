/*
 * mm/rmap.c - physical to virtual reverse mappings
 *
 * Copyright 2001, Rik van Riel <riel@conectiva.com.br>
 * Released under the General Public License (GPL).
 *
 * Simple, low overhead reverse mapping scheme.
 * Please try to keep this thing as modular as possible.
 *
 * Provides methods for unmapping each kind of mapped page:
 * the anon methods track anonymous pages, and
 * the file methods track pages belonging to an inode.
 *
 * Original design by Rik van Riel <riel@conectiva.com.br> 2001
 * File methods by Dave McCracken <dmccr@us.ibm.com> 2003, 2004
 * Anonymous methods by Andrea Arcangeli <andrea@suse.de> 2004
 * Contributions by Hugh Dickins 2003, 2004
 */

/*
 * Lock ordering in mm:
 *
 * inode->i_mutex	(while writing or truncating, not reading or faulting)
<<<<<<< HEAD
<<<<<<< HEAD
 *   inode->i_alloc_sem (vmtruncate_range)
=======
>>>>>>> refs/remotes/origin/cm-10.0
 *   mm->mmap_sem
 *     page->flags PG_locked (lock_page)
 *       mapping->i_mmap_mutex
 *         anon_vma->mutex
=======
 *   mm->mmap_sem
 *     page->flags PG_locked (lock_page)
 *       mapping->i_mmap_mutex
 *         anon_vma->rwsem
>>>>>>> refs/remotes/origin/master
 *           mm->page_table_lock or pte_lock
 *             zone->lru_lock (in mark_page_accessed, isolate_lru_page)
 *             swap_lock (in swap_duplicate, swap_info_get)
 *               mmlist_lock (in mmput, drain_mmlist and others)
 *               mapping->private_lock (in __set_page_dirty_buffers)
 *               inode->i_lock (in set_page_dirty's __mark_inode_dirty)
<<<<<<< HEAD
<<<<<<< HEAD
 *               inode_wb_list_lock (in set_page_dirty's __mark_inode_dirty)
 *                 sb_lock (within inode_lock in fs/fs-writeback.c)
 *                 mapping->tree_lock (widely used, in set_page_dirty,
 *                           in arch-dependent flush_dcache_mmap_lock,
 *                           within inode_wb_list_lock in __sync_single_inode)
=======
=======
>>>>>>> refs/remotes/origin/master
 *               bdi.wb->list_lock (in set_page_dirty's __mark_inode_dirty)
 *                 sb_lock (within inode_lock in fs/fs-writeback.c)
 *                 mapping->tree_lock (widely used, in set_page_dirty,
 *                           in arch-dependent flush_dcache_mmap_lock,
 *                           within bdi.wb->list_lock in __sync_single_inode)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * anon_vma->mutex,mapping->i_mutex      (memory_failure, collect_procs_anon)
=======
 *
 * anon_vma->rwsem,mapping->i_mutex      (memory_failure, collect_procs_anon)
>>>>>>> refs/remotes/origin/master
 *   ->tasklist_lock
 *     pte map lock
 */

#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/swap.h>
#include <linux/swapops.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/ksm.h>
#include <linux/rmap.h>
#include <linux/rcupdate.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/memcontrol.h>
#include <linux/mmu_notifier.h>
#include <linux/migrate.h>
#include <linux/hugetlb.h>
#include <linux/backing-dev.h>

#include <asm/tlbflush.h>

#include "internal.h"

static struct kmem_cache *anon_vma_cachep;
static struct kmem_cache *anon_vma_chain_cachep;

static inline struct anon_vma *anon_vma_alloc(void)
{
	struct anon_vma *anon_vma;

	anon_vma = kmem_cache_alloc(anon_vma_cachep, GFP_KERNEL);
	if (anon_vma) {
		atomic_set(&anon_vma->refcount, 1);
		/*
		 * Initialise the anon_vma root to point to itself. If called
		 * from fork, the root will be reset to the parents anon_vma.
		 */
		anon_vma->root = anon_vma;
	}

	return anon_vma;
}

static inline void anon_vma_free(struct anon_vma *anon_vma)
{
	VM_BUG_ON(atomic_read(&anon_vma->refcount));

	/*
<<<<<<< HEAD
	 * Synchronize against page_lock_anon_vma() such that
=======
	 * Synchronize against page_lock_anon_vma_read() such that
>>>>>>> refs/remotes/origin/master
	 * we can safely hold the lock without the anon_vma getting
	 * freed.
	 *
	 * Relies on the full mb implied by the atomic_dec_and_test() from
	 * put_anon_vma() against the acquire barrier implied by
<<<<<<< HEAD
	 * mutex_trylock() from page_lock_anon_vma(). This orders:
	 *
	 * page_lock_anon_vma()		VS	put_anon_vma()
	 *   mutex_trylock()			  atomic_dec_and_test()
	 *   LOCK				  MB
	 *   atomic_read()			  mutex_is_locked()
=======
	 * down_read_trylock() from page_lock_anon_vma_read(). This orders:
	 *
	 * page_lock_anon_vma_read()	VS	put_anon_vma()
	 *   down_read_trylock()		  atomic_dec_and_test()
	 *   LOCK				  MB
	 *   atomic_read()			  rwsem_is_locked()
>>>>>>> refs/remotes/origin/master
	 *
	 * LOCK should suffice since the actual taking of the lock must
	 * happen _before_ what follows.
	 */
<<<<<<< HEAD
	if (mutex_is_locked(&anon_vma->root->mutex)) {
		anon_vma_lock(anon_vma);
		anon_vma_unlock(anon_vma);
=======
	if (rwsem_is_locked(&anon_vma->root->rwsem)) {
		anon_vma_lock_write(anon_vma);
		anon_vma_unlock_write(anon_vma);
>>>>>>> refs/remotes/origin/master
	}

	kmem_cache_free(anon_vma_cachep, anon_vma);
}

static inline struct anon_vma_chain *anon_vma_chain_alloc(gfp_t gfp)
{
	return kmem_cache_alloc(anon_vma_chain_cachep, gfp);
}

static void anon_vma_chain_free(struct anon_vma_chain *anon_vma_chain)
{
	kmem_cache_free(anon_vma_chain_cachep, anon_vma_chain);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void anon_vma_chain_link(struct vm_area_struct *vma,
				struct anon_vma_chain *avc,
				struct anon_vma *anon_vma)
{
	avc->vma = vma;
	avc->anon_vma = anon_vma;
	list_add(&avc->same_vma, &vma->anon_vma_chain);
<<<<<<< HEAD

	/*
	 * It's critical to add new vmas to the tail of the anon_vma,
	 * see comment in huge_memory.c:__split_huge_page().
	 */
	list_add_tail(&avc->same_anon_vma, &anon_vma->head);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	anon_vma_interval_tree_insert(avc, &anon_vma->rb_root);
}

>>>>>>> refs/remotes/origin/master
/**
 * anon_vma_prepare - attach an anon_vma to a memory region
 * @vma: the memory region in question
 *
 * This makes sure the memory mapping described by 'vma' has
 * an 'anon_vma' attached to it, so that we can associate the
 * anonymous pages mapped into it with that anon_vma.
 *
 * The common case will be that we already have one, but if
 * not we either need to find an adjacent mapping that we
 * can re-use the anon_vma from (very common when the only
 * reason for splitting a vma has been mprotect()), or we
 * allocate a new one.
 *
 * Anon-vma allocations are very subtle, because we may have
<<<<<<< HEAD
 * optimistically looked up an anon_vma in page_lock_anon_vma()
=======
 * optimistically looked up an anon_vma in page_lock_anon_vma_read()
>>>>>>> refs/remotes/origin/master
 * and that may actually touch the spinlock even in the newly
 * allocated vma (it depends on RCU to make sure that the
 * anon_vma isn't actually destroyed).
 *
 * As a result, we need to do proper anon_vma locking even
 * for the new allocation. At the same time, we do not want
 * to do any locking for the common case of already having
 * an anon_vma.
 *
 * This must be called with the mmap_sem held for reading.
 */
int anon_vma_prepare(struct vm_area_struct *vma)
{
	struct anon_vma *anon_vma = vma->anon_vma;
	struct anon_vma_chain *avc;

	might_sleep();
	if (unlikely(!anon_vma)) {
		struct mm_struct *mm = vma->vm_mm;
		struct anon_vma *allocated;

		avc = anon_vma_chain_alloc(GFP_KERNEL);
		if (!avc)
			goto out_enomem;

		anon_vma = find_mergeable_anon_vma(vma);
		allocated = NULL;
		if (!anon_vma) {
			anon_vma = anon_vma_alloc();
			if (unlikely(!anon_vma))
				goto out_enomem_free_avc;
			allocated = anon_vma;
		}

<<<<<<< HEAD
		anon_vma_lock(anon_vma);
=======
		anon_vma_lock_write(anon_vma);
>>>>>>> refs/remotes/origin/master
		/* page_table_lock to protect against threads */
		spin_lock(&mm->page_table_lock);
		if (likely(!vma->anon_vma)) {
			vma->anon_vma = anon_vma;
<<<<<<< HEAD
<<<<<<< HEAD
			avc->anon_vma = anon_vma;
			avc->vma = vma;
			list_add(&avc->same_vma, &vma->anon_vma_chain);
			list_add_tail(&avc->same_anon_vma, &anon_vma->head);
=======
			anon_vma_chain_link(vma, avc, anon_vma);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			anon_vma_chain_link(vma, avc, anon_vma);
>>>>>>> refs/remotes/origin/master
			allocated = NULL;
			avc = NULL;
		}
		spin_unlock(&mm->page_table_lock);
<<<<<<< HEAD
		anon_vma_unlock(anon_vma);
=======
		anon_vma_unlock_write(anon_vma);
>>>>>>> refs/remotes/origin/master

		if (unlikely(allocated))
			put_anon_vma(allocated);
		if (unlikely(avc))
			anon_vma_chain_free(avc);
	}
	return 0;

 out_enomem_free_avc:
	anon_vma_chain_free(avc);
 out_enomem:
	return -ENOMEM;
}

/*
 * This is a useful helper function for locking the anon_vma root as
 * we traverse the vma->anon_vma_chain, looping over anon_vma's that
 * have the same vma.
 *
 * Such anon_vma's should have the same root, so you'd expect to see
 * just a single mutex_lock for the whole traversal.
 */
static inline struct anon_vma *lock_anon_vma_root(struct anon_vma *root, struct anon_vma *anon_vma)
{
	struct anon_vma *new_root = anon_vma->root;
	if (new_root != root) {
		if (WARN_ON_ONCE(root))
<<<<<<< HEAD
			mutex_unlock(&root->mutex);
		root = new_root;
		mutex_lock(&root->mutex);
=======
			up_write(&root->rwsem);
		root = new_root;
		down_write(&root->rwsem);
>>>>>>> refs/remotes/origin/master
	}
	return root;
}

static inline void unlock_anon_vma_root(struct anon_vma *root)
{
	if (root)
<<<<<<< HEAD
		mutex_unlock(&root->mutex);
}

<<<<<<< HEAD
static void anon_vma_chain_link(struct vm_area_struct *vma,
				struct anon_vma_chain *avc,
				struct anon_vma *anon_vma)
{
	avc->vma = vma;
	avc->anon_vma = anon_vma;
	list_add(&avc->same_vma, &vma->anon_vma_chain);

	/*
	 * It's critical to add new vmas to the tail of the anon_vma,
	 * see comment in huge_memory.c:__split_huge_page().
	 */
	list_add_tail(&avc->same_anon_vma, &anon_vma->head);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
		up_write(&root->rwsem);
}

>>>>>>> refs/remotes/origin/master
/*
 * Attach the anon_vmas from src to dst.
 * Returns 0 on success, -ENOMEM on failure.
 */
int anon_vma_clone(struct vm_area_struct *dst, struct vm_area_struct *src)
{
	struct anon_vma_chain *avc, *pavc;
	struct anon_vma *root = NULL;

	list_for_each_entry_reverse(pavc, &src->anon_vma_chain, same_vma) {
		struct anon_vma *anon_vma;

		avc = anon_vma_chain_alloc(GFP_NOWAIT | __GFP_NOWARN);
		if (unlikely(!avc)) {
			unlock_anon_vma_root(root);
			root = NULL;
			avc = anon_vma_chain_alloc(GFP_KERNEL);
			if (!avc)
				goto enomem_failure;
		}
		anon_vma = pavc->anon_vma;
		root = lock_anon_vma_root(root, anon_vma);
		anon_vma_chain_link(dst, avc, anon_vma);
	}
	unlock_anon_vma_root(root);
	return 0;

 enomem_failure:
	unlink_anon_vmas(dst);
	return -ENOMEM;
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
=======
 * Some rmap walk that needs to find all ptes/hugepmds without false
 * negatives (like migrate and split_huge_page) running concurrent
 * with operations that copy or move pagetables (like mremap() and
 * fork()) to be safe. They depend on the anon_vma "same_anon_vma"
 * list to be in a certain order: the dst_vma must be placed after the
 * src_vma in the list. This is always guaranteed by fork() but
 * mremap() needs to call this function to enforce it in case the
 * dst_vma isn't newly allocated and chained with the anon_vma_clone()
 * function but just an extension of a pre-existing vma through
 * vma_merge.
 *
 * NOTE: the same_anon_vma list can still be changed by other
 * processes while mremap runs because mremap doesn't hold the
 * anon_vma mutex to prevent modifications to the list while it
 * runs. All we need to enforce is that the relative order of this
 * process vmas isn't changing (we don't care about other vmas
 * order). Each vma corresponds to an anon_vma_chain structure so
 * there's no risk that other processes calling anon_vma_moveto_tail()
 * and changing the same_anon_vma list under mremap() will screw with
 * the relative order of this process vmas in the list, because we
 * they can't alter the order of any vma that belongs to this
 * process. And there can't be another anon_vma_moveto_tail() running
 * concurrently with mremap() coming from this process because we hold
 * the mmap_sem for the whole mremap(). fork() ordering dependency
 * also shouldn't be affected because fork() only cares that the
 * parent vmas are placed in the list before the child vmas and
 * anon_vma_moveto_tail() won't reorder vmas from either the fork()
 * parent or child.
 */
void anon_vma_moveto_tail(struct vm_area_struct *dst)
{
	struct anon_vma_chain *pavc;
	struct anon_vma *root = NULL;

	list_for_each_entry_reverse(pavc, &dst->anon_vma_chain, same_vma) {
		struct anon_vma *anon_vma = pavc->anon_vma;
		VM_BUG_ON(pavc->vma != dst);
		root = lock_anon_vma_root(root, anon_vma);
		list_del(&pavc->same_anon_vma);
		list_add_tail(&pavc->same_anon_vma, &anon_vma->head);
	}
	unlock_anon_vma_root(root);
}

/*
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * Attach vma to its own anon_vma, as well as to the anon_vmas that
 * the corresponding VMA in the parent process is attached to.
 * Returns 0 on success, non-zero on failure.
 */
int anon_vma_fork(struct vm_area_struct *vma, struct vm_area_struct *pvma)
{
	struct anon_vma_chain *avc;
	struct anon_vma *anon_vma;

	/* Don't bother if the parent process has no anon_vma here. */
	if (!pvma->anon_vma)
		return 0;

	/*
	 * First, attach the new VMA to the parent VMA's anon_vmas,
	 * so rmap can find non-COWed pages in child processes.
	 */
	if (anon_vma_clone(vma, pvma))
		return -ENOMEM;

	/* Then add our own anon_vma. */
	anon_vma = anon_vma_alloc();
	if (!anon_vma)
		goto out_error;
	avc = anon_vma_chain_alloc(GFP_KERNEL);
	if (!avc)
		goto out_error_free_anon_vma;

	/*
	 * The root anon_vma's spinlock is the lock actually used when we
	 * lock any of the anon_vmas in this anon_vma tree.
	 */
	anon_vma->root = pvma->anon_vma->root;
	/*
	 * With refcounts, an anon_vma can stay around longer than the
	 * process it belongs to. The root anon_vma needs to be pinned until
	 * this anon_vma is freed, because the lock lives in the root.
	 */
	get_anon_vma(anon_vma->root);
	/* Mark this anon_vma as the one where our new (COWed) pages go. */
	vma->anon_vma = anon_vma;
<<<<<<< HEAD
	anon_vma_lock(anon_vma);
	anon_vma_chain_link(vma, avc, anon_vma);
	anon_vma_unlock(anon_vma);
=======
	anon_vma_lock_write(anon_vma);
	anon_vma_chain_link(vma, avc, anon_vma);
	anon_vma_unlock_write(anon_vma);
>>>>>>> refs/remotes/origin/master

	return 0;

 out_error_free_anon_vma:
	put_anon_vma(anon_vma);
 out_error:
	unlink_anon_vmas(vma);
	return -ENOMEM;
}

void unlink_anon_vmas(struct vm_area_struct *vma)
{
	struct anon_vma_chain *avc, *next;
	struct anon_vma *root = NULL;

	/*
	 * Unlink each anon_vma chained to the VMA.  This list is ordered
	 * from newest to oldest, ensuring the root anon_vma gets freed last.
	 */
	list_for_each_entry_safe(avc, next, &vma->anon_vma_chain, same_vma) {
		struct anon_vma *anon_vma = avc->anon_vma;

		root = lock_anon_vma_root(root, anon_vma);
<<<<<<< HEAD
		list_del(&avc->same_anon_vma);
=======
		anon_vma_interval_tree_remove(avc, &anon_vma->rb_root);
>>>>>>> refs/remotes/origin/master

		/*
		 * Leave empty anon_vmas on the list - we'll need
		 * to free them outside the lock.
		 */
<<<<<<< HEAD
		if (list_empty(&anon_vma->head))
=======
		if (RB_EMPTY_ROOT(&anon_vma->rb_root))
>>>>>>> refs/remotes/origin/master
			continue;

		list_del(&avc->same_vma);
		anon_vma_chain_free(avc);
	}
	unlock_anon_vma_root(root);

	/*
	 * Iterate the list once more, it now only contains empty and unlinked
	 * anon_vmas, destroy them. Could not do before due to __put_anon_vma()
<<<<<<< HEAD
	 * needing to acquire the anon_vma->root->mutex.
=======
	 * needing to write-acquire the anon_vma->root->rwsem.
>>>>>>> refs/remotes/origin/master
	 */
	list_for_each_entry_safe(avc, next, &vma->anon_vma_chain, same_vma) {
		struct anon_vma *anon_vma = avc->anon_vma;

		put_anon_vma(anon_vma);

		list_del(&avc->same_vma);
		anon_vma_chain_free(avc);
	}
}

static void anon_vma_ctor(void *data)
{
	struct anon_vma *anon_vma = data;

<<<<<<< HEAD
	mutex_init(&anon_vma->mutex);
	atomic_set(&anon_vma->refcount, 0);
	INIT_LIST_HEAD(&anon_vma->head);
=======
	init_rwsem(&anon_vma->rwsem);
	atomic_set(&anon_vma->refcount, 0);
	anon_vma->rb_root = RB_ROOT;
>>>>>>> refs/remotes/origin/master
}

void __init anon_vma_init(void)
{
	anon_vma_cachep = kmem_cache_create("anon_vma", sizeof(struct anon_vma),
			0, SLAB_DESTROY_BY_RCU|SLAB_PANIC, anon_vma_ctor);
	anon_vma_chain_cachep = KMEM_CACHE(anon_vma_chain, SLAB_PANIC);
}

/*
 * Getting a lock on a stable anon_vma from a page off the LRU is tricky!
 *
 * Since there is no serialization what so ever against page_remove_rmap()
 * the best this function can do is return a locked anon_vma that might
 * have been relevant to this page.
 *
 * The page might have been remapped to a different anon_vma or the anon_vma
 * returned may already be freed (and even reused).
 *
 * In case it was remapped to a different anon_vma, the new anon_vma will be a
 * child of the old anon_vma, and the anon_vma lifetime rules will therefore
 * ensure that any anon_vma obtained from the page will still be valid for as
 * long as we observe page_mapped() [ hence all those page_mapped() tests ].
 *
 * All users of this function must be very careful when walking the anon_vma
 * chain and verify that the page in question is indeed mapped in it
 * [ something equivalent to page_mapped_in_vma() ].
 *
 * Since anon_vma's slab is DESTROY_BY_RCU and we know from page_remove_rmap()
 * that the anon_vma pointer from page->mapping is valid if there is a
 * mapcount, we can dereference the anon_vma after observing those.
 */
struct anon_vma *page_get_anon_vma(struct page *page)
{
	struct anon_vma *anon_vma = NULL;
	unsigned long anon_mapping;

	rcu_read_lock();
	anon_mapping = (unsigned long) ACCESS_ONCE(page->mapping);
	if ((anon_mapping & PAGE_MAPPING_FLAGS) != PAGE_MAPPING_ANON)
		goto out;
	if (!page_mapped(page))
		goto out;

	anon_vma = (struct anon_vma *) (anon_mapping - PAGE_MAPPING_ANON);
	if (!atomic_inc_not_zero(&anon_vma->refcount)) {
		anon_vma = NULL;
		goto out;
	}

	/*
	 * If this page is still mapped, then its anon_vma cannot have been
	 * freed.  But if it has been unmapped, we have no security against the
	 * anon_vma structure being freed and reused (for another anon_vma:
	 * SLAB_DESTROY_BY_RCU guarantees that - so the atomic_inc_not_zero()
	 * above cannot corrupt).
	 */
	if (!page_mapped(page)) {
		put_anon_vma(anon_vma);
		anon_vma = NULL;
	}
out:
	rcu_read_unlock();

	return anon_vma;
}

/*
 * Similar to page_get_anon_vma() except it locks the anon_vma.
 *
 * Its a little more complex as it tries to keep the fast path to a single
 * atomic op -- the trylock. If we fail the trylock, we fall back to getting a
 * reference like with page_get_anon_vma() and then block on the mutex.
 */
<<<<<<< HEAD
struct anon_vma *page_lock_anon_vma(struct page *page)
=======
struct anon_vma *page_lock_anon_vma_read(struct page *page)
>>>>>>> refs/remotes/origin/master
{
	struct anon_vma *anon_vma = NULL;
	struct anon_vma *root_anon_vma;
	unsigned long anon_mapping;

	rcu_read_lock();
	anon_mapping = (unsigned long) ACCESS_ONCE(page->mapping);
	if ((anon_mapping & PAGE_MAPPING_FLAGS) != PAGE_MAPPING_ANON)
		goto out;
	if (!page_mapped(page))
		goto out;

	anon_vma = (struct anon_vma *) (anon_mapping - PAGE_MAPPING_ANON);
	root_anon_vma = ACCESS_ONCE(anon_vma->root);
<<<<<<< HEAD
	if (mutex_trylock(&root_anon_vma->mutex)) {
=======
	if (down_read_trylock(&root_anon_vma->rwsem)) {
>>>>>>> refs/remotes/origin/master
		/*
		 * If the page is still mapped, then this anon_vma is still
		 * its anon_vma, and holding the mutex ensures that it will
		 * not go away, see anon_vma_free().
		 */
		if (!page_mapped(page)) {
<<<<<<< HEAD
			mutex_unlock(&root_anon_vma->mutex);
=======
			up_read(&root_anon_vma->rwsem);
>>>>>>> refs/remotes/origin/master
			anon_vma = NULL;
		}
		goto out;
	}

	/* trylock failed, we got to sleep */
	if (!atomic_inc_not_zero(&anon_vma->refcount)) {
		anon_vma = NULL;
		goto out;
	}

	if (!page_mapped(page)) {
		put_anon_vma(anon_vma);
		anon_vma = NULL;
		goto out;
	}

	/* we pinned the anon_vma, its safe to sleep */
	rcu_read_unlock();
<<<<<<< HEAD
	anon_vma_lock(anon_vma);
=======
	anon_vma_lock_read(anon_vma);
>>>>>>> refs/remotes/origin/master

	if (atomic_dec_and_test(&anon_vma->refcount)) {
		/*
		 * Oops, we held the last refcount, release the lock
		 * and bail -- can't simply use put_anon_vma() because
<<<<<<< HEAD
		 * we'll deadlock on the anon_vma_lock() recursion.
		 */
		anon_vma_unlock(anon_vma);
=======
		 * we'll deadlock on the anon_vma_lock_write() recursion.
		 */
		anon_vma_unlock_read(anon_vma);
>>>>>>> refs/remotes/origin/master
		__put_anon_vma(anon_vma);
		anon_vma = NULL;
	}

	return anon_vma;

out:
	rcu_read_unlock();
	return anon_vma;
}

<<<<<<< HEAD
void page_unlock_anon_vma(struct anon_vma *anon_vma)
{
	anon_vma_unlock(anon_vma);
=======
void page_unlock_anon_vma_read(struct anon_vma *anon_vma)
{
	anon_vma_unlock_read(anon_vma);
>>>>>>> refs/remotes/origin/master
}

/*
 * At what user virtual address is page expected in @vma?
<<<<<<< HEAD
 * Returns virtual address or -EFAULT if page's index/offset is not
 * within the range mapped the @vma.
 */
inline unsigned long
vma_address(struct page *page, struct vm_area_struct *vma)
{
	pgoff_t pgoff = page->index << (PAGE_CACHE_SHIFT - PAGE_SHIFT);
	unsigned long address;

	if (unlikely(is_vm_hugetlb_page(vma)))
		pgoff = page->index << huge_page_order(page_hstate(page));
	address = vma->vm_start + ((pgoff - vma->vm_pgoff) << PAGE_SHIFT);
	if (unlikely(address < vma->vm_start || address >= vma->vm_end)) {
		/* page should be within @vma mapping range */
		return -EFAULT;
	}
=======
 */
static inline unsigned long
__vma_address(struct page *page, struct vm_area_struct *vma)
{
	pgoff_t pgoff = page->index << (PAGE_CACHE_SHIFT - PAGE_SHIFT);

	if (unlikely(is_vm_hugetlb_page(vma)))
		pgoff = page->index << huge_page_order(page_hstate(page));

	return vma->vm_start + ((pgoff - vma->vm_pgoff) << PAGE_SHIFT);
}

inline unsigned long
vma_address(struct page *page, struct vm_area_struct *vma)
{
	unsigned long address = __vma_address(page, vma);

	/* page should be within @vma mapping range */
	VM_BUG_ON(address < vma->vm_start || address >= vma->vm_end);

>>>>>>> refs/remotes/origin/master
	return address;
}

/*
 * At what user virtual address is page expected in vma?
 * Caller should check the page is actually part of the vma.
 */
unsigned long page_address_in_vma(struct page *page, struct vm_area_struct *vma)
{
<<<<<<< HEAD
=======
	unsigned long address;
>>>>>>> refs/remotes/origin/master
	if (PageAnon(page)) {
		struct anon_vma *page__anon_vma = page_anon_vma(page);
		/*
		 * Note: swapoff's unuse_vma() is more efficient with this
		 * check, and needs it to match anon_vma when KSM is active.
		 */
		if (!vma->anon_vma || !page__anon_vma ||
		    vma->anon_vma->root != page__anon_vma->root)
			return -EFAULT;
	} else if (page->mapping && !(vma->vm_flags & VM_NONLINEAR)) {
		if (!vma->vm_file ||
		    vma->vm_file->f_mapping != page->mapping)
			return -EFAULT;
	} else
		return -EFAULT;
<<<<<<< HEAD
	return vma_address(page, vma);
=======
	address = __vma_address(page, vma);
	if (unlikely(address < vma->vm_start || address >= vma->vm_end))
		return -EFAULT;
	return address;
}

pmd_t *mm_find_pmd(struct mm_struct *mm, unsigned long address)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd = NULL;

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		goto out;

	pud = pud_offset(pgd, address);
	if (!pud_present(*pud))
		goto out;

	pmd = pmd_offset(pud, address);
	if (!pmd_present(*pmd))
		pmd = NULL;
out:
	return pmd;
>>>>>>> refs/remotes/origin/master
}

/*
 * Check that @page is mapped at @address into @mm.
 *
 * If @sync is false, page_check_address may perform a racy check to avoid
 * the page table lock when the pte is not present (helpful when reclaiming
 * highly shared pages).
 *
 * On success returns with pte mapped and locked.
 */
pte_t *__page_check_address(struct page *page, struct mm_struct *mm,
			  unsigned long address, spinlock_t **ptlp, int sync)
{
<<<<<<< HEAD
	pgd_t *pgd;
	pud_t *pud;
=======
>>>>>>> refs/remotes/origin/master
	pmd_t *pmd;
	pte_t *pte;
	spinlock_t *ptl;

	if (unlikely(PageHuge(page))) {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		/* when pud is not present, pte will be NULL */
		pte = huge_pte_offset(mm, address);
		if (!pte)
			return NULL;

<<<<<<< HEAD
<<<<<<< HEAD
=======
		pte = huge_pte_offset(mm, address);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
		ptl = &mm->page_table_lock;
		goto check;
	}

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		return NULL;

	pud = pud_offset(pgd, address);
	if (!pud_present(*pud))
		return NULL;

	pmd = pmd_offset(pud, address);
	if (!pmd_present(*pmd))
		return NULL;
=======
		ptl = huge_pte_lockptr(page_hstate(page), mm, pte);
		goto check;
	}

	pmd = mm_find_pmd(mm, address);
	if (!pmd)
		return NULL;

>>>>>>> refs/remotes/origin/master
	if (pmd_trans_huge(*pmd))
		return NULL;

	pte = pte_offset_map(pmd, address);
	/* Make a quick check before getting the lock */
	if (!sync && !pte_present(*pte)) {
		pte_unmap(pte);
		return NULL;
	}

	ptl = pte_lockptr(mm, pmd);
check:
	spin_lock(ptl);
	if (pte_present(*pte) && page_to_pfn(page) == pte_pfn(*pte)) {
		*ptlp = ptl;
		return pte;
	}
	pte_unmap_unlock(pte, ptl);
	return NULL;
}

/**
 * page_mapped_in_vma - check whether a page is really mapped in a VMA
 * @page: the page to test
 * @vma: the VMA to test
 *
 * Returns 1 if the page is mapped into the page tables of the VMA, 0
 * if the page is not mapped into the page tables of this VMA.  Only
 * valid for normal file or anonymous VMAs.
 */
int page_mapped_in_vma(struct page *page, struct vm_area_struct *vma)
{
	unsigned long address;
	pte_t *pte;
	spinlock_t *ptl;

<<<<<<< HEAD
	address = vma_address(page, vma);
	if (address == -EFAULT)		/* out of vma range */
=======
	address = __vma_address(page, vma);
	if (unlikely(address < vma->vm_start || address >= vma->vm_end))
>>>>>>> refs/remotes/origin/master
		return 0;
	pte = page_check_address(page, vma->vm_mm, address, &ptl, 1);
	if (!pte)			/* the page is not in this mm */
		return 0;
	pte_unmap_unlock(pte, ptl);

	return 1;
}

<<<<<<< HEAD
/*
 * Subfunctions of page_referenced: page_referenced_one called
 * repeatedly from either page_referenced_anon or page_referenced_file.
 */
int page_referenced_one(struct page *page, struct vm_area_struct *vma,
			unsigned long address, unsigned int *mapcount,
			unsigned long *vm_flags)
{
	struct mm_struct *mm = vma->vm_mm;
	int referenced = 0;
=======
struct page_referenced_arg {
	int mapcount;
	int referenced;
	unsigned long vm_flags;
	struct mem_cgroup *memcg;
};
/*
 * arg: page_referenced_arg will be passed
 */
int page_referenced_one(struct page *page, struct vm_area_struct *vma,
			unsigned long address, void *arg)
{
	struct mm_struct *mm = vma->vm_mm;
	spinlock_t *ptl;
	int referenced = 0;
	struct page_referenced_arg *pra = arg;
>>>>>>> refs/remotes/origin/master

	if (unlikely(PageTransHuge(page))) {
		pmd_t *pmd;

<<<<<<< HEAD
		spin_lock(&mm->page_table_lock);
=======
>>>>>>> refs/remotes/origin/master
		/*
		 * rmap might return false positives; we must filter
		 * these out using page_check_address_pmd().
		 */
		pmd = page_check_address_pmd(page, mm, address,
<<<<<<< HEAD
					     PAGE_CHECK_ADDRESS_PMD_FLAG);
		if (!pmd) {
			spin_unlock(&mm->page_table_lock);
			goto out;
		}

		if (vma->vm_flags & VM_LOCKED) {
			spin_unlock(&mm->page_table_lock);
			*mapcount = 0;	/* break early from loop */
			*vm_flags |= VM_LOCKED;
			goto out;
=======
					     PAGE_CHECK_ADDRESS_PMD_FLAG, &ptl);
		if (!pmd)
			return SWAP_AGAIN;

		if (vma->vm_flags & VM_LOCKED) {
			spin_unlock(ptl);
			pra->vm_flags |= VM_LOCKED;
			return SWAP_FAIL; /* To break the loop */
>>>>>>> refs/remotes/origin/master
		}

		/* go ahead even if the pmd is pmd_trans_splitting() */
		if (pmdp_clear_flush_young_notify(vma, address, pmd))
			referenced++;
<<<<<<< HEAD
		spin_unlock(&mm->page_table_lock);
	} else {
		pte_t *pte;
		spinlock_t *ptl;
=======
		spin_unlock(ptl);
	} else {
		pte_t *pte;
>>>>>>> refs/remotes/origin/master

		/*
		 * rmap might return false positives; we must filter
		 * these out using page_check_address().
		 */
		pte = page_check_address(page, mm, address, &ptl, 0);
		if (!pte)
<<<<<<< HEAD
			goto out;

		if (vma->vm_flags & VM_LOCKED) {
			pte_unmap_unlock(pte, ptl);
			*mapcount = 0;	/* break early from loop */
			*vm_flags |= VM_LOCKED;
			goto out;
=======
			return SWAP_AGAIN;

		if (vma->vm_flags & VM_LOCKED) {
			pte_unmap_unlock(pte, ptl);
			pra->vm_flags |= VM_LOCKED;
			return SWAP_FAIL; /* To break the loop */
>>>>>>> refs/remotes/origin/master
		}

		if (ptep_clear_flush_young_notify(vma, address, pte)) {
			/*
			 * Don't treat a reference through a sequentially read
			 * mapping as such.  If the page has been used in
			 * another mapping, we will catch it; if this other
			 * mapping is already gone, the unmap path will have
			 * set PG_referenced or activated the page.
			 */
<<<<<<< HEAD
			if (likely(!VM_SequentialReadHint(vma)))
=======
			if (likely(!(vma->vm_flags & VM_SEQ_READ)))
>>>>>>> refs/remotes/origin/master
				referenced++;
		}
		pte_unmap_unlock(pte, ptl);
	}

<<<<<<< HEAD
	/* Pretend the page is referenced if the task has the
	   swap token and is in the middle of a page fault. */
	if (mm != current->mm && has_swap_token(mm) &&
			rwsem_is_locked(&mm->mmap_sem))
		referenced++;

	(*mapcount)--;

	if (referenced)
		*vm_flags |= vma->vm_flags;
out:
	return referenced;
}

static int page_referenced_anon(struct page *page,
<<<<<<< HEAD
				struct mem_cgroup *mem_cont,
=======
				struct mem_cgroup *memcg,
>>>>>>> refs/remotes/origin/cm-10.0
				unsigned long *vm_flags)
{
	unsigned int mapcount;
	struct anon_vma *anon_vma;
	struct anon_vma_chain *avc;
	int referenced = 0;

	anon_vma = page_lock_anon_vma(page);
	if (!anon_vma)
		return referenced;

	mapcount = page_mapcount(page);
	list_for_each_entry(avc, &anon_vma->head, same_anon_vma) {
		struct vm_area_struct *vma = avc->vma;
		unsigned long address = vma_address(page, vma);
		if (address == -EFAULT)
			continue;
		/*
		 * If we are reclaiming on behalf of a cgroup, skip
		 * counting on behalf of references from different
		 * cgroups
		 */
<<<<<<< HEAD
		if (mem_cont && !mm_match_cgroup(vma->vm_mm, mem_cont))
=======
		if (memcg && !mm_match_cgroup(vma->vm_mm, memcg))
>>>>>>> refs/remotes/origin/cm-10.0
			continue;
		referenced += page_referenced_one(page, vma, address,
						  &mapcount, vm_flags);
		if (!mapcount)
			break;
	}

	page_unlock_anon_vma(anon_vma);
	return referenced;
}

/**
 * page_referenced_file - referenced check for object-based rmap
 * @page: the page we're checking references on.
<<<<<<< HEAD
 * @mem_cont: target memory controller
=======
 * @memcg: target memory control group
>>>>>>> refs/remotes/origin/cm-10.0
 * @vm_flags: collect encountered vma->vm_flags who actually referenced the page
 *
 * For an object-based mapped page, find all the places it is mapped and
 * check/clear the referenced flag.  This is done by following the page->mapping
 * pointer, then walking the chain of vmas it holds.  It returns the number
 * of references it found.
 *
 * This function is only called from page_referenced for object-based pages.
 */
static int page_referenced_file(struct page *page,
<<<<<<< HEAD
				struct mem_cgroup *mem_cont,
=======
				struct mem_cgroup *memcg,
>>>>>>> refs/remotes/origin/cm-10.0
				unsigned long *vm_flags)
{
	unsigned int mapcount;
	struct address_space *mapping = page->mapping;
	pgoff_t pgoff = page->index << (PAGE_CACHE_SHIFT - PAGE_SHIFT);
	struct vm_area_struct *vma;
	struct prio_tree_iter iter;
	int referenced = 0;

	/*
	 * The caller's checks on page->mapping and !PageAnon have made
	 * sure that this is a file page: the check for page->mapping
	 * excludes the case just before it gets set on an anon page.
	 */
	BUG_ON(PageAnon(page));

	/*
	 * The page lock not only makes sure that page->mapping cannot
	 * suddenly be NULLified by truncation, it makes sure that the
	 * structure at mapping cannot be freed and reused yet,
	 * so we can safely take mapping->i_mmap_mutex.
	 */
	BUG_ON(!PageLocked(page));

	mutex_lock(&mapping->i_mmap_mutex);

	/*
	 * i_mmap_mutex does not stabilize mapcount at all, but mapcount
	 * is more likely to be accurate if we note it after spinning.
	 */
	mapcount = page_mapcount(page);

	vma_prio_tree_foreach(vma, &iter, &mapping->i_mmap, pgoff, pgoff) {
		unsigned long address = vma_address(page, vma);
		if (address == -EFAULT)
			continue;
		/*
		 * If we are reclaiming on behalf of a cgroup, skip
		 * counting on behalf of references from different
		 * cgroups
		 */
<<<<<<< HEAD
		if (mem_cont && !mm_match_cgroup(vma->vm_mm, mem_cont))
=======
		if (memcg && !mm_match_cgroup(vma->vm_mm, memcg))
>>>>>>> refs/remotes/origin/cm-10.0
			continue;
		referenced += page_referenced_one(page, vma, address,
						  &mapcount, vm_flags);
		if (!mapcount)
			break;
	}

	mutex_unlock(&mapping->i_mmap_mutex);
	return referenced;
=======
	if (referenced) {
		pra->referenced++;
		pra->vm_flags |= vma->vm_flags;
	}

	pra->mapcount--;
	if (!pra->mapcount)
		return SWAP_SUCCESS; /* To break the loop */

	return SWAP_AGAIN;
}

static bool invalid_page_referenced_vma(struct vm_area_struct *vma, void *arg)
{
	struct page_referenced_arg *pra = arg;
	struct mem_cgroup *memcg = pra->memcg;

	if (!mm_match_cgroup(vma->vm_mm, memcg))
		return true;

	return false;
>>>>>>> refs/remotes/origin/master
}

/**
 * page_referenced - test if the page was referenced
 * @page: the page to test
 * @is_locked: caller holds lock on the page
<<<<<<< HEAD
<<<<<<< HEAD
 * @mem_cont: target memory controller
=======
 * @memcg: target memory cgroup
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * @memcg: target memory cgroup
>>>>>>> refs/remotes/origin/master
 * @vm_flags: collect encountered vma->vm_flags who actually referenced the page
 *
 * Quick test_and_clear_referenced for all mappings to a page,
 * returns the number of ptes which referenced the page.
 */
int page_referenced(struct page *page,
		    int is_locked,
<<<<<<< HEAD
<<<<<<< HEAD
		    struct mem_cgroup *mem_cont,
=======
		    struct mem_cgroup *memcg,
>>>>>>> refs/remotes/origin/cm-10.0
		    unsigned long *vm_flags)
{
	int referenced = 0;
	int we_locked = 0;

	*vm_flags = 0;
	if (page_mapped(page) && page_rmapping(page)) {
		if (!is_locked && (!PageAnon(page) || PageKsm(page))) {
			we_locked = trylock_page(page);
			if (!we_locked) {
				referenced++;
				goto out;
			}
		}
		if (unlikely(PageKsm(page)))
<<<<<<< HEAD
			referenced += page_referenced_ksm(page, mem_cont,
								vm_flags);
		else if (PageAnon(page))
			referenced += page_referenced_anon(page, mem_cont,
								vm_flags);
		else if (page->mapping)
			referenced += page_referenced_file(page, mem_cont,
								vm_flags);
		if (we_locked)
			unlock_page(page);
	}
out:
	if (page_test_and_clear_young(page_to_pfn(page)))
		referenced++;

=======
			referenced += page_referenced_ksm(page, memcg,
								vm_flags);
		else if (PageAnon(page))
			referenced += page_referenced_anon(page, memcg,
								vm_flags);
		else if (page->mapping)
			referenced += page_referenced_file(page, memcg,
								vm_flags);
		if (we_locked)
			unlock_page(page);

		if (page_test_and_clear_young(page_to_pfn(page)))
			referenced++;
	}
out:
>>>>>>> refs/remotes/origin/cm-10.0
	return referenced;
}

static int page_mkclean_one(struct page *page, struct vm_area_struct *vma,
			    unsigned long address)
=======
		    struct mem_cgroup *memcg,
		    unsigned long *vm_flags)
{
	int ret;
	int we_locked = 0;
	struct page_referenced_arg pra = {
		.mapcount = page_mapcount(page),
		.memcg = memcg,
	};
	struct rmap_walk_control rwc = {
		.rmap_one = page_referenced_one,
		.arg = (void *)&pra,
		.anon_lock = page_lock_anon_vma_read,
	};

	*vm_flags = 0;
	if (!page_mapped(page))
		return 0;

	if (!page_rmapping(page))
		return 0;

	if (!is_locked && (!PageAnon(page) || PageKsm(page))) {
		we_locked = trylock_page(page);
		if (!we_locked)
			return 1;
	}

	/*
	 * If we are reclaiming on behalf of a cgroup, skip
	 * counting on behalf of references from different
	 * cgroups
	 */
	if (memcg) {
		rwc.invalid_vma = invalid_page_referenced_vma;
	}

	ret = rmap_walk(page, &rwc);
	*vm_flags = pra.vm_flags;

	if (we_locked)
		unlock_page(page);

	return pra.referenced;
}

static int page_mkclean_one(struct page *page, struct vm_area_struct *vma,
			    unsigned long address, void *arg)
>>>>>>> refs/remotes/origin/master
{
	struct mm_struct *mm = vma->vm_mm;
	pte_t *pte;
	spinlock_t *ptl;
	int ret = 0;
<<<<<<< HEAD
=======
	int *cleaned = arg;
>>>>>>> refs/remotes/origin/master

	pte = page_check_address(page, mm, address, &ptl, 1);
	if (!pte)
		goto out;

	if (pte_dirty(*pte) || pte_write(*pte)) {
		pte_t entry;

		flush_cache_page(vma, address, pte_pfn(*pte));
<<<<<<< HEAD
		entry = ptep_clear_flush_notify(vma, address, pte);
=======
		entry = ptep_clear_flush(vma, address, pte);
>>>>>>> refs/remotes/origin/master
		entry = pte_wrprotect(entry);
		entry = pte_mkclean(entry);
		set_pte_at(mm, address, pte, entry);
		ret = 1;
	}

	pte_unmap_unlock(pte, ptl);
<<<<<<< HEAD
out:
	return ret;
}

static int page_mkclean_file(struct address_space *mapping, struct page *page)
{
	pgoff_t pgoff = page->index << (PAGE_CACHE_SHIFT - PAGE_SHIFT);
	struct vm_area_struct *vma;
	struct prio_tree_iter iter;
	int ret = 0;

	BUG_ON(PageAnon(page));

	mutex_lock(&mapping->i_mmap_mutex);
	vma_prio_tree_foreach(vma, &iter, &mapping->i_mmap, pgoff, pgoff) {
		if (vma->vm_flags & VM_SHARED) {
			unsigned long address = vma_address(page, vma);
			if (address == -EFAULT)
				continue;
			ret += page_mkclean_one(page, vma, address);
		}
	}
	mutex_unlock(&mapping->i_mmap_mutex);
	return ret;
=======

	if (ret) {
		mmu_notifier_invalidate_page(mm, address);
		(*cleaned)++;
	}
out:
	return SWAP_AGAIN;
}

static bool invalid_mkclean_vma(struct vm_area_struct *vma, void *arg)
{
	if (vma->vm_flags & VM_SHARED)
		return 0;

	return 1;
>>>>>>> refs/remotes/origin/master
}

int page_mkclean(struct page *page)
{
<<<<<<< HEAD
	int ret = 0;

	BUG_ON(!PageLocked(page));

	if (page_mapped(page)) {
		struct address_space *mapping = page_mapping(page);
		if (mapping)
			ret = page_mkclean_file(mapping, page);
	}

	return ret;
=======
	int cleaned = 0;
	struct address_space *mapping;
	struct rmap_walk_control rwc = {
		.arg = (void *)&cleaned,
		.rmap_one = page_mkclean_one,
		.invalid_vma = invalid_mkclean_vma,
	};

	BUG_ON(!PageLocked(page));

	if (!page_mapped(page))
		return 0;

	mapping = page_mapping(page);
	if (!mapping)
		return 0;

	rmap_walk(page, &rwc);

	return cleaned;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(page_mkclean);

/**
 * page_move_anon_rmap - move a page to our anon_vma
 * @page:	the page to move to our anon_vma
 * @vma:	the vma the page belongs to
 * @address:	the user virtual address mapped
 *
 * When a page belongs exclusively to one process after a COW event,
 * that page can be moved into the anon_vma that belongs to just that
 * process, so the rmap code will not search the parent or sibling
 * processes.
 */
void page_move_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address)
{
	struct anon_vma *anon_vma = vma->anon_vma;

	VM_BUG_ON(!PageLocked(page));
	VM_BUG_ON(!anon_vma);
	VM_BUG_ON(page->index != linear_page_index(vma, address));

	anon_vma = (void *) anon_vma + PAGE_MAPPING_ANON;
	page->mapping = (struct address_space *) anon_vma;
}

/**
 * __page_set_anon_rmap - set up new anonymous rmap
 * @page:	Page to add to rmap	
 * @vma:	VM area to add page to.
 * @address:	User virtual address of the mapping	
 * @exclusive:	the page is exclusively owned by the current process
 */
static void __page_set_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address, int exclusive)
{
	struct anon_vma *anon_vma = vma->anon_vma;

	BUG_ON(!anon_vma);

	if (PageAnon(page))
		return;

	/*
	 * If the page isn't exclusively mapped into this vma,
	 * we must use the _oldest_ possible anon_vma for the
	 * page mapping!
	 */
	if (!exclusive)
		anon_vma = anon_vma->root;

	anon_vma = (void *) anon_vma + PAGE_MAPPING_ANON;
	page->mapping = (struct address_space *) anon_vma;
	page->index = linear_page_index(vma, address);
}

/**
 * __page_check_anon_rmap - sanity check anonymous rmap addition
 * @page:	the page to add the mapping to
 * @vma:	the vm area in which the mapping is added
 * @address:	the user virtual address mapped
 */
static void __page_check_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address)
{
#ifdef CONFIG_DEBUG_VM
	/*
	 * The page's anon-rmap details (mapping and index) are guaranteed to
	 * be set up correctly at this point.
	 *
	 * We have exclusion against page_add_anon_rmap because the caller
	 * always holds the page locked, except if called from page_dup_rmap,
	 * in which case the page is already known to be setup.
	 *
	 * We have exclusion against page_add_new_anon_rmap because those pages
	 * are initially only visible via the pagetables, and the pte is locked
	 * over the call to page_add_new_anon_rmap.
	 */
	BUG_ON(page_anon_vma(page)->root != vma->anon_vma->root);
	BUG_ON(page->index != linear_page_index(vma, address));
#endif
}

/**
 * page_add_anon_rmap - add pte mapping to an anonymous page
 * @page:	the page to add the mapping to
 * @vma:	the vm area in which the mapping is added
 * @address:	the user virtual address mapped
 *
 * The caller needs to hold the pte lock, and the page must be locked in
 * the anon_vma case: to serialize mapping,index checking after setting,
 * and to ensure that PageAnon is not being upgraded racily to PageKsm
 * (but PageKsm is never downgraded to PageAnon).
 */
void page_add_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address)
{
	do_page_add_anon_rmap(page, vma, address, 0);
}

/*
 * Special version of the above for do_swap_page, which often runs
 * into pages that are exclusively owned by the current process.
 * Everybody else should continue to use page_add_anon_rmap above.
 */
void do_page_add_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address, int exclusive)
{
	int first = atomic_inc_and_test(&page->_mapcount);
	if (first) {
<<<<<<< HEAD
		if (!PageTransHuge(page))
			__inc_zone_page_state(page, NR_ANON_PAGES);
		else
			__inc_zone_page_state(page,
					      NR_ANON_TRANSPARENT_HUGEPAGES);
=======
		if (PageTransHuge(page))
			__inc_zone_page_state(page,
					      NR_ANON_TRANSPARENT_HUGEPAGES);
		__mod_zone_page_state(page_zone(page), NR_ANON_PAGES,
				hpage_nr_pages(page));
>>>>>>> refs/remotes/origin/master
	}
	if (unlikely(PageKsm(page)))
		return;

	VM_BUG_ON(!PageLocked(page));
	/* address might be in next vma when migration races vma_adjust */
	if (first)
		__page_set_anon_rmap(page, vma, address, exclusive);
	else
		__page_check_anon_rmap(page, vma, address);
}

/**
 * page_add_new_anon_rmap - add pte mapping to a new anonymous page
 * @page:	the page to add the mapping to
 * @vma:	the vm area in which the mapping is added
 * @address:	the user virtual address mapped
 *
 * Same as page_add_anon_rmap but must only be called on *new* pages.
 * This means the inc-and-test can be bypassed.
 * Page does not have to be locked.
 */
void page_add_new_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address)
{
	VM_BUG_ON(address < vma->vm_start || address >= vma->vm_end);
	SetPageSwapBacked(page);
	atomic_set(&page->_mapcount, 0); /* increment count (starts at -1) */
<<<<<<< HEAD
	if (!PageTransHuge(page))
		__inc_zone_page_state(page, NR_ANON_PAGES);
	else
		__inc_zone_page_state(page, NR_ANON_TRANSPARENT_HUGEPAGES);
	__page_set_anon_rmap(page, vma, address, 1);
	if (page_evictable(page, vma))
		lru_cache_add_lru(page, LRU_ACTIVE_ANON);
	else
=======
	if (PageTransHuge(page))
		__inc_zone_page_state(page, NR_ANON_TRANSPARENT_HUGEPAGES);
	__mod_zone_page_state(page_zone(page), NR_ANON_PAGES,
			hpage_nr_pages(page));
	__page_set_anon_rmap(page, vma, address, 1);
	if (!mlocked_vma_newpage(vma, page)) {
		SetPageActive(page);
		lru_cache_add(page);
	} else
>>>>>>> refs/remotes/origin/master
		add_page_to_unevictable_list(page);
}

/**
 * page_add_file_rmap - add pte mapping to a file page
 * @page: the page to add the mapping to
 *
 * The caller needs to hold the pte lock.
 */
void page_add_file_rmap(struct page *page)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	bool locked;
	unsigned long flags;

	mem_cgroup_begin_update_page_stat(page, &locked, &flags);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (atomic_inc_and_test(&page->_mapcount)) {
		__inc_zone_page_state(page, NR_FILE_MAPPED);
		mem_cgroup_inc_page_stat(page, MEMCG_NR_FILE_MAPPED);
	}
<<<<<<< HEAD
=======
	mem_cgroup_end_update_page_stat(page, &locked, &flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (atomic_inc_and_test(&page->_mapcount)) {
		__inc_zone_page_state(page, NR_FILE_MAPPED);
		mem_cgroup_inc_page_stat(page, MEM_CGROUP_STAT_FILE_MAPPED);
	}
	mem_cgroup_end_update_page_stat(page, &locked, &flags);
>>>>>>> refs/remotes/origin/master
}

/**
 * page_remove_rmap - take down pte mapping from a page
 * @page: page to remove mapping from
 *
 * The caller needs to hold the pte lock.
 */
void page_remove_rmap(struct page *page)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct address_space *mapping = page_mapping(page);
<<<<<<< HEAD

	/* page still mapped by someone else? */
	if (!atomic_add_negative(-1, &page->_mapcount))
		return;
=======
=======
>>>>>>> refs/remotes/origin/master
=======
	struct address_space *mapping = page_mapping(page);
>>>>>>> refs/remotes/origin/cm-11.0
	bool anon = PageAnon(page);
	bool locked;
	unsigned long flags;

	/*
	 * The anon case has no mem_cgroup page_stat to update; but may
	 * uncharge_page() below, where the lock ordering can deadlock if
	 * we hold the lock against page_stat move: so avoid it on anon.
	 */
	if (!anon)
		mem_cgroup_begin_update_page_stat(page, &locked, &flags);

	/* page still mapped by someone else? */
	if (!atomic_add_negative(-1, &page->_mapcount))
		goto out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * Now that the last pte has gone, s390 must transfer dirty
	 * flag from storage key to struct page.  We can usually skip
	 * this if the page is anon, so about to be freed; but perhaps
	 * not if it's in swapcache - there might be another pte slot
	 * containing the swap entry, but page not yet written to swap.
	 *
	 * And we can skip it on file pages, so long as the filesystem
	 * participates in dirty tracking; but need to catch shm and tmpfs
	 * and ramfs pages which have been modified since creation by read
	 * fault.
	 *
	 * Note that mapping must be decided above, before decrementing
	 * mapcount (which luckily provides a barrier): once page is unmapped,
	 * it could be truncated and page->mapping reset to NULL at any moment.
	 * Note also that we are relying on page_mapping(page) to set mapping
	 * to &swapper_space when PageSwapCache(page).
	 */
	if (mapping && !mapping_cap_account_dirty(mapping) &&
	    page_test_and_clear_dirty(page_to_pfn(page), 1))
		set_page_dirty(page);
	/*
=======

	/*
>>>>>>> refs/remotes/origin/master
	 * Hugepages are not counted in NR_ANON_PAGES nor NR_FILE_MAPPED
	 * and not charged by memcg for now.
	 */
	if (unlikely(PageHuge(page)))
<<<<<<< HEAD
<<<<<<< HEAD
		return;
	if (PageAnon(page)) {
=======
		goto out;
	if (anon) {
>>>>>>> refs/remotes/origin/cm-10.0
		mem_cgroup_uncharge_page(page);
		if (!PageTransHuge(page))
			__dec_zone_page_state(page, NR_ANON_PAGES);
		else
			__dec_zone_page_state(page,
					      NR_ANON_TRANSPARENT_HUGEPAGES);
	} else {
		__dec_zone_page_state(page, NR_FILE_MAPPED);
		mem_cgroup_dec_page_stat(page, MEMCG_NR_FILE_MAPPED);
	}
=======
		goto out;
	if (anon) {
		mem_cgroup_uncharge_page(page);
		if (PageTransHuge(page))
			__dec_zone_page_state(page,
					      NR_ANON_TRANSPARENT_HUGEPAGES);
		__mod_zone_page_state(page_zone(page), NR_ANON_PAGES,
				-hpage_nr_pages(page));
	} else {
		__dec_zone_page_state(page, NR_FILE_MAPPED);
		mem_cgroup_dec_page_stat(page, MEM_CGROUP_STAT_FILE_MAPPED);
		mem_cgroup_end_update_page_stat(page, &locked, &flags);
	}
	if (unlikely(PageMlocked(page)))
		clear_page_mlock(page);
>>>>>>> refs/remotes/origin/master
	/*
	 * It would be tidy to reset the PageAnon mapping here,
	 * but that might overwrite a racing page_add_anon_rmap
	 * which increments mapcount after us but sets mapping
	 * before us: so leave the reset to free_hot_cold_page,
	 * and remember that it's only reliable while mapped.
	 * Leaving it set also helps swapoff to reinstate ptes
	 * faster for those pages still in swapcache.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
=======
out:
	if (!anon)
		mem_cgroup_end_update_page_stat(page, &locked, &flags);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Subfunctions of try_to_unmap: try_to_unmap_one called
<<<<<<< HEAD
 * repeatedly from either try_to_unmap_anon or try_to_unmap_file.
=======
 * repeatedly from try_to_unmap_ksm, try_to_unmap_anon or try_to_unmap_file.
>>>>>>> refs/remotes/origin/cm-10.0
 */
int try_to_unmap_one(struct page *page, struct vm_area_struct *vma,
		     unsigned long address, enum ttu_flags flags)
=======
	return;
out:
	if (!anon)
		mem_cgroup_end_update_page_stat(page, &locked, &flags);
}

/*
 * @arg: enum ttu_flags will be passed to this argument
 */
int try_to_unmap_one(struct page *page, struct vm_area_struct *vma,
		     unsigned long address, void *arg)
>>>>>>> refs/remotes/origin/master
{
	struct mm_struct *mm = vma->vm_mm;
	pte_t *pte;
	pte_t pteval;
	spinlock_t *ptl;
	int ret = SWAP_AGAIN;
<<<<<<< HEAD
=======
	enum ttu_flags flags = (enum ttu_flags)arg;
>>>>>>> refs/remotes/origin/master

	pte = page_check_address(page, mm, address, &ptl, 0);
	if (!pte)
		goto out;

	/*
	 * If the page is mlock()d, we cannot swap it out.
	 * If it's recently referenced (perhaps page_referenced
	 * skipped over this mm) then we should reactivate it.
	 */
	if (!(flags & TTU_IGNORE_MLOCK)) {
		if (vma->vm_flags & VM_LOCKED)
			goto out_mlock;

		if (TTU_ACTION(flags) == TTU_MUNLOCK)
			goto out_unmap;
	}
	if (!(flags & TTU_IGNORE_ACCESS)) {
		if (ptep_clear_flush_young_notify(vma, address, pte)) {
			ret = SWAP_FAIL;
			goto out_unmap;
		}
  	}

	/* Nuke the page table entry. */
	flush_cache_page(vma, address, page_to_pfn(page));
<<<<<<< HEAD
	pteval = ptep_clear_flush_notify(vma, address, pte);
=======
	pteval = ptep_clear_flush(vma, address, pte);
>>>>>>> refs/remotes/origin/master

	/* Move the dirty bit to the physical page now the pte is gone. */
	if (pte_dirty(pteval))
		set_page_dirty(page);

	/* Update high watermark before we lower rss */
	update_hiwater_rss(mm);

	if (PageHWPoison(page) && !(flags & TTU_IGNORE_HWPOISON)) {
<<<<<<< HEAD
		if (PageAnon(page))
			dec_mm_counter(mm, MM_ANONPAGES);
		else
			dec_mm_counter(mm, MM_FILEPAGES);
		set_pte_at(mm, address, pte,
				swp_entry_to_pte(make_hwpoison_entry(page)));
	} else if (PageAnon(page)) {
		swp_entry_t entry = { .val = page_private(page) };
=======
		if (!PageHuge(page)) {
			if (PageAnon(page))
				dec_mm_counter(mm, MM_ANONPAGES);
			else
				dec_mm_counter(mm, MM_FILEPAGES);
		}
		set_pte_at(mm, address, pte,
			   swp_entry_to_pte(make_hwpoison_entry(page)));
	} else if (PageAnon(page)) {
		swp_entry_t entry = { .val = page_private(page) };
		pte_t swp_pte;
>>>>>>> refs/remotes/origin/master

		if (PageSwapCache(page)) {
			/*
			 * Store the swap location in the pte.
			 * See handle_pte_fault() ...
			 */
			if (swap_duplicate(entry) < 0) {
				set_pte_at(mm, address, pte, pteval);
				ret = SWAP_FAIL;
				goto out_unmap;
			}
			if (list_empty(&mm->mmlist)) {
				spin_lock(&mmlist_lock);
				if (list_empty(&mm->mmlist))
					list_add(&mm->mmlist, &init_mm.mmlist);
				spin_unlock(&mmlist_lock);
			}
			dec_mm_counter(mm, MM_ANONPAGES);
			inc_mm_counter(mm, MM_SWAPENTS);
<<<<<<< HEAD
<<<<<<< HEAD
		} else if (PAGE_MIGRATION) {
=======
		} else if (IS_ENABLED(CONFIG_MIGRATION)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		} else if (IS_ENABLED(CONFIG_MIGRATION)) {
>>>>>>> refs/remotes/origin/master
			/*
			 * Store the pfn of the page in a special migration
			 * pte. do_swap_page() will wait until the migration
			 * pte is removed and then restart fault handling.
			 */
			BUG_ON(TTU_ACTION(flags) != TTU_MIGRATION);
			entry = make_migration_entry(page, pte_write(pteval));
		}
<<<<<<< HEAD
		set_pte_at(mm, address, pte, swp_entry_to_pte(entry));
		BUG_ON(pte_file(*pte));
<<<<<<< HEAD
	} else if (PAGE_MIGRATION && (TTU_ACTION(flags) == TTU_MIGRATION)) {
=======
	} else if (IS_ENABLED(CONFIG_MIGRATION) &&
		   (TTU_ACTION(flags) == TTU_MIGRATION)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		swp_pte = swp_entry_to_pte(entry);
		if (pte_soft_dirty(pteval))
			swp_pte = pte_swp_mksoft_dirty(swp_pte);
		set_pte_at(mm, address, pte, swp_pte);
		BUG_ON(pte_file(*pte));
	} else if (IS_ENABLED(CONFIG_MIGRATION) &&
		   (TTU_ACTION(flags) == TTU_MIGRATION)) {
>>>>>>> refs/remotes/origin/master
		/* Establish migration entry for a file page */
		swp_entry_t entry;
		entry = make_migration_entry(page, pte_write(pteval));
		set_pte_at(mm, address, pte, swp_entry_to_pte(entry));
	} else
		dec_mm_counter(mm, MM_FILEPAGES);

	page_remove_rmap(page);
	page_cache_release(page);

out_unmap:
	pte_unmap_unlock(pte, ptl);
<<<<<<< HEAD
=======
	if (ret != SWAP_FAIL)
		mmu_notifier_invalidate_page(mm, address);
>>>>>>> refs/remotes/origin/master
out:
	return ret;

out_mlock:
	pte_unmap_unlock(pte, ptl);


	/*
	 * We need mmap_sem locking, Otherwise VM_LOCKED check makes
	 * unstable result and race. Plus, We can't wait here because
<<<<<<< HEAD
	 * we now hold anon_vma->mutex or mapping->i_mmap_mutex.
=======
	 * we now hold anon_vma->rwsem or mapping->i_mmap_mutex.
>>>>>>> refs/remotes/origin/master
	 * if trylock failed, the page remain in evictable lru and later
	 * vmscan could retry to move the page to unevictable lru if the
	 * page is actually mlocked.
	 */
	if (down_read_trylock(&vma->vm_mm->mmap_sem)) {
		if (vma->vm_flags & VM_LOCKED) {
			mlock_vma_page(page);
			ret = SWAP_MLOCK;
		}
		up_read(&vma->vm_mm->mmap_sem);
	}
	return ret;
}

/*
 * objrmap doesn't work for nonlinear VMAs because the assumption that
 * offset-into-file correlates with offset-into-virtual-addresses does not hold.
 * Consequently, given a particular page and its ->index, we cannot locate the
 * ptes which are mapping that page without an exhaustive linear search.
 *
 * So what this code does is a mini "virtual scan" of each nonlinear VMA which
 * maps the file to which the target page belongs.  The ->vm_private_data field
 * holds the current cursor into that scan.  Successive searches will circulate
 * around the vma's virtual address space.
 *
 * So as more replacement pressure is applied to the pages in a nonlinear VMA,
 * more scanning pressure is placed against them as well.   Eventually pages
 * will become fully unmapped and are eligible for eviction.
 *
 * For very sparsely populated VMAs this is a little inefficient - chances are
 * there there won't be many ptes located within the scan cluster.  In this case
 * maybe we could scan further - to the end of the pte page, perhaps.
 *
 * Mlocked pages:  check VM_LOCKED under mmap_sem held for read, if we can
 * acquire it without blocking.  If vma locked, mlock the pages in the cluster,
 * rather than unmapping them.  If we encounter the "check_page" that vmscan is
 * trying to unmap, return SWAP_MLOCK, else default SWAP_AGAIN.
 */
#define CLUSTER_SIZE	min(32*PAGE_SIZE, PMD_SIZE)
#define CLUSTER_MASK	(~(CLUSTER_SIZE - 1))

static int try_to_unmap_cluster(unsigned long cursor, unsigned int *mapcount,
		struct vm_area_struct *vma, struct page *check_page)
{
	struct mm_struct *mm = vma->vm_mm;
<<<<<<< HEAD
	pgd_t *pgd;
	pud_t *pud;
=======
>>>>>>> refs/remotes/origin/master
	pmd_t *pmd;
	pte_t *pte;
	pte_t pteval;
	spinlock_t *ptl;
	struct page *page;
	unsigned long address;
<<<<<<< HEAD
=======
	unsigned long mmun_start;	/* For mmu_notifiers */
	unsigned long mmun_end;		/* For mmu_notifiers */
>>>>>>> refs/remotes/origin/master
	unsigned long end;
	int ret = SWAP_AGAIN;
	int locked_vma = 0;

	address = (vma->vm_start + cursor) & CLUSTER_MASK;
	end = address + CLUSTER_SIZE;
	if (address < vma->vm_start)
		address = vma->vm_start;
	if (end > vma->vm_end)
		end = vma->vm_end;

<<<<<<< HEAD
	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		return ret;

	pud = pud_offset(pgd, address);
	if (!pud_present(*pud))
		return ret;

	pmd = pmd_offset(pud, address);
	if (!pmd_present(*pmd))
		return ret;
=======
	pmd = mm_find_pmd(mm, address);
	if (!pmd)
		return ret;

	mmun_start = address;
	mmun_end   = end;
	mmu_notifier_invalidate_range_start(mm, mmun_start, mmun_end);
>>>>>>> refs/remotes/origin/master

	/*
	 * If we can acquire the mmap_sem for read, and vma is VM_LOCKED,
	 * keep the sem while scanning the cluster for mlocking pages.
	 */
	if (down_read_trylock(&vma->vm_mm->mmap_sem)) {
		locked_vma = (vma->vm_flags & VM_LOCKED);
		if (!locked_vma)
			up_read(&vma->vm_mm->mmap_sem); /* don't need it */
	}

	pte = pte_offset_map_lock(mm, pmd, address, &ptl);

	/* Update high watermark before we lower rss */
	update_hiwater_rss(mm);

	for (; address < end; pte++, address += PAGE_SIZE) {
		if (!pte_present(*pte))
			continue;
		page = vm_normal_page(vma, address, *pte);
		BUG_ON(!page || PageAnon(page));

		if (locked_vma) {
			mlock_vma_page(page);   /* no-op if already mlocked */
			if (page == check_page)
				ret = SWAP_MLOCK;
			continue;	/* don't unmap */
		}

		if (ptep_clear_flush_young_notify(vma, address, pte))
			continue;

		/* Nuke the page table entry. */
		flush_cache_page(vma, address, pte_pfn(*pte));
<<<<<<< HEAD
		pteval = ptep_clear_flush_notify(vma, address, pte);

		/* If nonlinear, store the file page offset in the pte. */
		if (page->index != linear_page_index(vma, address))
			set_pte_at(mm, address, pte, pgoff_to_pte(page->index));
=======
		pteval = ptep_clear_flush(vma, address, pte);

		/* If nonlinear, store the file page offset in the pte. */
		if (page->index != linear_page_index(vma, address)) {
			pte_t ptfile = pgoff_to_pte(page->index);
			if (pte_soft_dirty(pteval))
				pte_file_mksoft_dirty(ptfile);
			set_pte_at(mm, address, pte, ptfile);
		}
>>>>>>> refs/remotes/origin/master

		/* Move the dirty bit to the physical page now the pte is gone. */
		if (pte_dirty(pteval))
			set_page_dirty(page);

		page_remove_rmap(page);
		page_cache_release(page);
		dec_mm_counter(mm, MM_FILEPAGES);
		(*mapcount)--;
	}
	pte_unmap_unlock(pte - 1, ptl);
<<<<<<< HEAD
=======
	mmu_notifier_invalidate_range_end(mm, mmun_start, mmun_end);
>>>>>>> refs/remotes/origin/master
	if (locked_vma)
		up_read(&vma->vm_mm->mmap_sem);
	return ret;
}

<<<<<<< HEAD
bool is_vma_temporary_stack(struct vm_area_struct *vma)
{
	int maybe_stack = vma->vm_flags & (VM_GROWSDOWN | VM_GROWSUP);

	if (!maybe_stack)
		return false;

	if ((vma->vm_flags & VM_STACK_INCOMPLETE_SETUP) ==
						VM_STACK_INCOMPLETE_SETUP)
		return true;

	return false;
}

/**
 * try_to_unmap_anon - unmap or unlock anonymous page using the object-based
 * rmap method
 * @page: the page to unmap/unlock
 * @flags: action and flags
 *
 * Find all the mappings of a page using the mapping pointer and the vma chains
 * contained in the anon_vma struct it points to.
 *
 * This function is only called from try_to_unmap/try_to_munlock for
 * anonymous pages.
 * When called from try_to_munlock(), the mmap_sem of the mm containing the vma
 * where the page was found will be held for write.  So, we won't recheck
 * vm_flags for that VMA.  That should be OK, because that vma shouldn't be
 * 'LOCKED.
 */
static int try_to_unmap_anon(struct page *page, enum ttu_flags flags)
{
	struct anon_vma *anon_vma;
	struct anon_vma_chain *avc;
	int ret = SWAP_AGAIN;

	anon_vma = page_lock_anon_vma(page);
	if (!anon_vma)
		return ret;

	list_for_each_entry(avc, &anon_vma->head, same_anon_vma) {
		struct vm_area_struct *vma = avc->vma;
		unsigned long address;

		/*
		 * During exec, a temporary VMA is setup and later moved.
		 * The VMA is moved under the anon_vma lock but not the
		 * page tables leading to a race where migration cannot
		 * find the migration ptes. Rather than increasing the
		 * locking requirements of exec(), migration skips
		 * temporary VMAs until after exec() completes.
		 */
<<<<<<< HEAD
		if (PAGE_MIGRATION && (flags & TTU_MIGRATION) &&
=======
		if (IS_ENABLED(CONFIG_MIGRATION) && (flags & TTU_MIGRATION) &&
>>>>>>> refs/remotes/origin/cm-10.0
				is_vma_temporary_stack(vma))
			continue;

		address = vma_address(page, vma);
		if (address == -EFAULT)
			continue;
		ret = try_to_unmap_one(page, vma, address, flags);
		if (ret != SWAP_AGAIN || !page_mapped(page))
			break;
	}

	page_unlock_anon_vma(anon_vma);
	return ret;
}

/**
 * try_to_unmap_file - unmap/unlock file page using the object-based rmap method
 * @page: the page to unmap/unlock
 * @flags: action and flags
 *
 * Find all the mappings of a page using the mapping pointer and the vma chains
 * contained in the address_space struct it points to.
 *
 * This function is only called from try_to_unmap/try_to_munlock for
 * object-based pages.
 * When called from try_to_munlock(), the mmap_sem of the mm containing the vma
 * where the page was found will be held for write.  So, we won't recheck
 * vm_flags for that VMA.  That should be OK, because that vma shouldn't be
 * 'LOCKED.
 */
static int try_to_unmap_file(struct page *page, enum ttu_flags flags)
{
	struct address_space *mapping = page->mapping;
	pgoff_t pgoff = page->index << (PAGE_CACHE_SHIFT - PAGE_SHIFT);
	struct vm_area_struct *vma;
	struct prio_tree_iter iter;
=======
static int try_to_unmap_nonlinear(struct page *page,
		struct address_space *mapping, struct vm_area_struct *vma)
{
>>>>>>> refs/remotes/origin/master
	int ret = SWAP_AGAIN;
	unsigned long cursor;
	unsigned long max_nl_cursor = 0;
	unsigned long max_nl_size = 0;
	unsigned int mapcount;

<<<<<<< HEAD
	mutex_lock(&mapping->i_mmap_mutex);
	vma_prio_tree_foreach(vma, &iter, &mapping->i_mmap, pgoff, pgoff) {
		unsigned long address = vma_address(page, vma);
		if (address == -EFAULT)
			continue;
		ret = try_to_unmap_one(page, vma, address, flags);
		if (ret != SWAP_AGAIN || !page_mapped(page))
			goto out;
	}

	if (list_empty(&mapping->i_mmap_nonlinear))
		goto out;

	/*
	 * We don't bother to try to find the munlocked page in nonlinears.
	 * It's costly. Instead, later, page reclaim logic may call
	 * try_to_unmap(TTU_MUNLOCK) and recover PG_mlocked lazily.
	 */
	if (TTU_ACTION(flags) == TTU_MUNLOCK)
		goto out;

	list_for_each_entry(vma, &mapping->i_mmap_nonlinear,
						shared.vm_set.list) {
=======
	list_for_each_entry(vma,
		&mapping->i_mmap_nonlinear, shared.nonlinear) {

>>>>>>> refs/remotes/origin/master
		cursor = (unsigned long) vma->vm_private_data;
		if (cursor > max_nl_cursor)
			max_nl_cursor = cursor;
		cursor = vma->vm_end - vma->vm_start;
		if (cursor > max_nl_size)
			max_nl_size = cursor;
	}

	if (max_nl_size == 0) {	/* all nonlinears locked or reserved ? */
<<<<<<< HEAD
		ret = SWAP_FAIL;
		goto out;
=======
		return SWAP_FAIL;
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * We don't try to search for this page in the nonlinear vmas,
	 * and page_referenced wouldn't have found it anyway.  Instead
	 * just walk the nonlinear vmas trying to age and unmap some.
	 * The mapcount of the page we came in with is irrelevant,
	 * but even so use it as a guide to how hard we should try?
	 */
	mapcount = page_mapcount(page);
	if (!mapcount)
<<<<<<< HEAD
		goto out;
=======
		return ret;

>>>>>>> refs/remotes/origin/master
	cond_resched();

	max_nl_size = (max_nl_size + CLUSTER_SIZE - 1) & CLUSTER_MASK;
	if (max_nl_cursor == 0)
		max_nl_cursor = CLUSTER_SIZE;

	do {
<<<<<<< HEAD
		list_for_each_entry(vma, &mapping->i_mmap_nonlinear,
						shared.vm_set.list) {
			cursor = (unsigned long) vma->vm_private_data;
			while ( cursor < max_nl_cursor &&
=======
		list_for_each_entry(vma,
			&mapping->i_mmap_nonlinear, shared.nonlinear) {

			cursor = (unsigned long) vma->vm_private_data;
			while (cursor < max_nl_cursor &&
>>>>>>> refs/remotes/origin/master
				cursor < vma->vm_end - vma->vm_start) {
				if (try_to_unmap_cluster(cursor, &mapcount,
						vma, page) == SWAP_MLOCK)
					ret = SWAP_MLOCK;
				cursor += CLUSTER_SIZE;
				vma->vm_private_data = (void *) cursor;
				if ((int)mapcount <= 0)
<<<<<<< HEAD
					goto out;
=======
					return ret;
>>>>>>> refs/remotes/origin/master
			}
			vma->vm_private_data = (void *) max_nl_cursor;
		}
		cond_resched();
		max_nl_cursor += CLUSTER_SIZE;
	} while (max_nl_cursor <= max_nl_size);

	/*
	 * Don't loop forever (perhaps all the remaining pages are
	 * in locked vmas).  Reset cursor on all unreserved nonlinear
	 * vmas, now forgetting on which ones it had fallen behind.
	 */
<<<<<<< HEAD
	list_for_each_entry(vma, &mapping->i_mmap_nonlinear, shared.vm_set.list)
		vma->vm_private_data = NULL;
out:
	mutex_unlock(&mapping->i_mmap_mutex);
	return ret;
}

=======
	list_for_each_entry(vma, &mapping->i_mmap_nonlinear, shared.nonlinear)
		vma->vm_private_data = NULL;

	return ret;
}

bool is_vma_temporary_stack(struct vm_area_struct *vma)
{
	int maybe_stack = vma->vm_flags & (VM_GROWSDOWN | VM_GROWSUP);

	if (!maybe_stack)
		return false;

	if ((vma->vm_flags & VM_STACK_INCOMPLETE_SETUP) ==
						VM_STACK_INCOMPLETE_SETUP)
		return true;

	return false;
}

static bool invalid_migration_vma(struct vm_area_struct *vma, void *arg)
{
	return is_vma_temporary_stack(vma);
}

static int page_not_mapped(struct page *page)
{
	return !page_mapped(page);
};

>>>>>>> refs/remotes/origin/master
/**
 * try_to_unmap - try to remove all page table mappings to a page
 * @page: the page to get unmapped
 * @flags: action and flags
 *
 * Tries to remove all the page table entries which are mapping this
 * page, used in the pageout path.  Caller must hold the page lock.
 * Return values are:
 *
 * SWAP_SUCCESS	- we succeeded in removing all mappings
 * SWAP_AGAIN	- we missed a mapping, try again later
 * SWAP_FAIL	- the page is unswappable
 * SWAP_MLOCK	- page is mlocked.
 */
int try_to_unmap(struct page *page, enum ttu_flags flags)
{
	int ret;
<<<<<<< HEAD

	BUG_ON(!PageLocked(page));
	VM_BUG_ON(!PageHuge(page) && PageTransHuge(page));

	if (unlikely(PageKsm(page)))
		ret = try_to_unmap_ksm(page, flags);
	else if (PageAnon(page))
		ret = try_to_unmap_anon(page, flags);
	else
		ret = try_to_unmap_file(page, flags);
=======
	struct rmap_walk_control rwc = {
		.rmap_one = try_to_unmap_one,
		.arg = (void *)flags,
		.done = page_not_mapped,
		.file_nonlinear = try_to_unmap_nonlinear,
		.anon_lock = page_lock_anon_vma_read,
	};

	VM_BUG_ON(!PageHuge(page) && PageTransHuge(page));

	/*
	 * During exec, a temporary VMA is setup and later moved.
	 * The VMA is moved under the anon_vma lock but not the
	 * page tables leading to a race where migration cannot
	 * find the migration ptes. Rather than increasing the
	 * locking requirements of exec(), migration skips
	 * temporary VMAs until after exec() completes.
	 */
	if (flags & TTU_MIGRATION && !PageKsm(page) && PageAnon(page))
		rwc.invalid_vma = invalid_migration_vma;

	ret = rmap_walk(page, &rwc);

>>>>>>> refs/remotes/origin/master
	if (ret != SWAP_MLOCK && !page_mapped(page))
		ret = SWAP_SUCCESS;
	return ret;
}

/**
 * try_to_munlock - try to munlock a page
 * @page: the page to be munlocked
 *
 * Called from munlock code.  Checks all of the VMAs mapping the page
 * to make sure nobody else has this page mlocked. The page will be
 * returned with PG_mlocked cleared if no other vmas have it mlocked.
 *
 * Return values are:
 *
 * SWAP_AGAIN	- no vma is holding page mlocked, or,
 * SWAP_AGAIN	- page mapped in mlocked vma -- couldn't acquire mmap sem
 * SWAP_FAIL	- page cannot be located at present
 * SWAP_MLOCK	- page is now mlocked.
 */
int try_to_munlock(struct page *page)
{
<<<<<<< HEAD
	VM_BUG_ON(!PageLocked(page) || PageLRU(page));

	if (unlikely(PageKsm(page)))
		return try_to_unmap_ksm(page, TTU_MUNLOCK);
	else if (PageAnon(page))
		return try_to_unmap_anon(page, TTU_MUNLOCK);
	else
		return try_to_unmap_file(page, TTU_MUNLOCK);
=======
	int ret;
	struct rmap_walk_control rwc = {
		.rmap_one = try_to_unmap_one,
		.arg = (void *)TTU_MUNLOCK,
		.done = page_not_mapped,
		/*
		 * We don't bother to try to find the munlocked page in
		 * nonlinears. It's costly. Instead, later, page reclaim logic
		 * may call try_to_unmap() and recover PG_mlocked lazily.
		 */
		.file_nonlinear = NULL,
		.anon_lock = page_lock_anon_vma_read,

	};

	VM_BUG_ON(!PageLocked(page) || PageLRU(page));

	ret = rmap_walk(page, &rwc);
	return ret;
>>>>>>> refs/remotes/origin/master
}

void __put_anon_vma(struct anon_vma *anon_vma)
{
	struct anon_vma *root = anon_vma->root;

	if (root != anon_vma && atomic_dec_and_test(&root->refcount))
		anon_vma_free(root);

	anon_vma_free(anon_vma);
}

<<<<<<< HEAD
#ifdef CONFIG_MIGRATION
/*
 * rmap_walk() and its helpers rmap_walk_anon() and rmap_walk_file():
 * Called by migrate.c to remove migration ptes, but might be used more later.
 */
static int rmap_walk_anon(struct page *page, int (*rmap_one)(struct page *,
		struct vm_area_struct *, unsigned long, void *), void *arg)
{
	struct anon_vma *anon_vma;
	struct anon_vma_chain *avc;
	int ret = SWAP_AGAIN;

	/*
	 * Note: remove_migration_ptes() cannot use page_lock_anon_vma()
=======
static struct anon_vma *rmap_walk_anon_lock(struct page *page,
					struct rmap_walk_control *rwc)
{
	struct anon_vma *anon_vma;

	if (rwc->anon_lock)
		return rwc->anon_lock(page);

	/*
	 * Note: remove_migration_ptes() cannot use page_lock_anon_vma_read()
>>>>>>> refs/remotes/origin/master
	 * because that depends on page_mapped(); but not all its usages
	 * are holding mmap_sem. Users without mmap_sem are required to
	 * take a reference count to prevent the anon_vma disappearing
	 */
	anon_vma = page_anon_vma(page);
	if (!anon_vma)
<<<<<<< HEAD
		return ret;
	anon_vma_lock(anon_vma);
	list_for_each_entry(avc, &anon_vma->head, same_anon_vma) {
		struct vm_area_struct *vma = avc->vma;
		unsigned long address = vma_address(page, vma);
		if (address == -EFAULT)
			continue;
		ret = rmap_one(page, vma, address, arg);
		if (ret != SWAP_AGAIN)
			break;
	}
	anon_vma_unlock(anon_vma);
	return ret;
}

static int rmap_walk_file(struct page *page, int (*rmap_one)(struct page *,
		struct vm_area_struct *, unsigned long, void *), void *arg)
{
	struct address_space *mapping = page->mapping;
	pgoff_t pgoff = page->index << (PAGE_CACHE_SHIFT - PAGE_SHIFT);
	struct vm_area_struct *vma;
	struct prio_tree_iter iter;
	int ret = SWAP_AGAIN;

	if (!mapping)
		return ret;
	mutex_lock(&mapping->i_mmap_mutex);
	vma_prio_tree_foreach(vma, &iter, &mapping->i_mmap, pgoff, pgoff) {
		unsigned long address = vma_address(page, vma);
		if (address == -EFAULT)
			continue;
		ret = rmap_one(page, vma, address, arg);
		if (ret != SWAP_AGAIN)
			break;
	}
	/*
	 * No nonlinear handling: being always shared, nonlinear vmas
	 * never contain migration ptes.  Decide what to do about this
	 * limitation to linear when we need rmap_walk() on nonlinear.
	 */
=======
		return NULL;

	anon_vma_lock_read(anon_vma);
	return anon_vma;
}

/*
 * rmap_walk_anon - do something to anonymous page using the object-based
 * rmap method
 * @page: the page to be handled
 * @rwc: control variable according to each walk type
 *
 * Find all the mappings of a page using the mapping pointer and the vma chains
 * contained in the anon_vma struct it points to.
 *
 * When called from try_to_munlock(), the mmap_sem of the mm containing the vma
 * where the page was found will be held for write.  So, we won't recheck
 * vm_flags for that VMA.  That should be OK, because that vma shouldn't be
 * LOCKED.
 */
static int rmap_walk_anon(struct page *page, struct rmap_walk_control *rwc)
{
	struct anon_vma *anon_vma;
	pgoff_t pgoff = page->index << (PAGE_CACHE_SHIFT - PAGE_SHIFT);
	struct anon_vma_chain *avc;
	int ret = SWAP_AGAIN;

	anon_vma = rmap_walk_anon_lock(page, rwc);
	if (!anon_vma)
		return ret;

	anon_vma_interval_tree_foreach(avc, &anon_vma->rb_root, pgoff, pgoff) {
		struct vm_area_struct *vma = avc->vma;
		unsigned long address = vma_address(page, vma);

		if (rwc->invalid_vma && rwc->invalid_vma(vma, rwc->arg))
			continue;

		ret = rwc->rmap_one(page, vma, address, rwc->arg);
		if (ret != SWAP_AGAIN)
			break;
		if (rwc->done && rwc->done(page))
			break;
	}
	anon_vma_unlock_read(anon_vma);
	return ret;
}

/*
 * rmap_walk_file - do something to file page using the object-based rmap method
 * @page: the page to be handled
 * @rwc: control variable according to each walk type
 *
 * Find all the mappings of a page using the mapping pointer and the vma chains
 * contained in the address_space struct it points to.
 *
 * When called from try_to_munlock(), the mmap_sem of the mm containing the vma
 * where the page was found will be held for write.  So, we won't recheck
 * vm_flags for that VMA.  That should be OK, because that vma shouldn't be
 * LOCKED.
 */
static int rmap_walk_file(struct page *page, struct rmap_walk_control *rwc)
{
	struct address_space *mapping = page->mapping;
	pgoff_t pgoff = page->index << compound_order(page);
	struct vm_area_struct *vma;
	int ret = SWAP_AGAIN;

	/*
	 * The page lock not only makes sure that page->mapping cannot
	 * suddenly be NULLified by truncation, it makes sure that the
	 * structure at mapping cannot be freed and reused yet,
	 * so we can safely take mapping->i_mmap_mutex.
	 */
	VM_BUG_ON(!PageLocked(page));

	if (!mapping)
		return ret;
	mutex_lock(&mapping->i_mmap_mutex);
	vma_interval_tree_foreach(vma, &mapping->i_mmap, pgoff, pgoff) {
		unsigned long address = vma_address(page, vma);

		if (rwc->invalid_vma && rwc->invalid_vma(vma, rwc->arg))
			continue;

		ret = rwc->rmap_one(page, vma, address, rwc->arg);
		if (ret != SWAP_AGAIN)
			goto done;
		if (rwc->done && rwc->done(page))
			goto done;
	}

	if (!rwc->file_nonlinear)
		goto done;

	if (list_empty(&mapping->i_mmap_nonlinear))
		goto done;

	ret = rwc->file_nonlinear(page, mapping, vma);

done:
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&mapping->i_mmap_mutex);
	return ret;
}

<<<<<<< HEAD
int rmap_walk(struct page *page, int (*rmap_one)(struct page *,
		struct vm_area_struct *, unsigned long, void *), void *arg)
{
	VM_BUG_ON(!PageLocked(page));

	if (unlikely(PageKsm(page)))
		return rmap_walk_ksm(page, rmap_one, arg);
	else if (PageAnon(page))
		return rmap_walk_anon(page, rmap_one, arg);
	else
		return rmap_walk_file(page, rmap_one, arg);
}
#endif /* CONFIG_MIGRATION */
=======
int rmap_walk(struct page *page, struct rmap_walk_control *rwc)
{
	if (unlikely(PageKsm(page)))
		return rmap_walk_ksm(page, rwc);
	else if (PageAnon(page))
		return rmap_walk_anon(page, rwc);
	else
		return rmap_walk_file(page, rwc);
}
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_HUGETLB_PAGE
/*
 * The following three functions are for anonymous (private mapped) hugepages.
 * Unlike common anonymous pages, anonymous hugepages have no accounting code
 * and no lru code, because we handle hugepages differently from common pages.
 */
static void __hugepage_set_anon_rmap(struct page *page,
	struct vm_area_struct *vma, unsigned long address, int exclusive)
{
	struct anon_vma *anon_vma = vma->anon_vma;

	BUG_ON(!anon_vma);

	if (PageAnon(page))
		return;
	if (!exclusive)
		anon_vma = anon_vma->root;

	anon_vma = (void *) anon_vma + PAGE_MAPPING_ANON;
	page->mapping = (struct address_space *) anon_vma;
	page->index = linear_page_index(vma, address);
}

void hugepage_add_anon_rmap(struct page *page,
			    struct vm_area_struct *vma, unsigned long address)
{
	struct anon_vma *anon_vma = vma->anon_vma;
	int first;

	BUG_ON(!PageLocked(page));
	BUG_ON(!anon_vma);
	/* address might be in next vma when migration races vma_adjust */
	first = atomic_inc_and_test(&page->_mapcount);
	if (first)
		__hugepage_set_anon_rmap(page, vma, address, 0);
}

void hugepage_add_new_anon_rmap(struct page *page,
			struct vm_area_struct *vma, unsigned long address)
{
	BUG_ON(address < vma->vm_start || address >= vma->vm_end);
	atomic_set(&page->_mapcount, 0);
	__hugepage_set_anon_rmap(page, vma, address, 1);
}
#endif /* CONFIG_HUGETLB_PAGE */
