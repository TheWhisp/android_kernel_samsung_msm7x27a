/* Inject a hwpoison memory failure on a arbitrary pfn */
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/pagemap.h>
#include <linux/hugetlb.h>
#include "internal.h"

static struct dentry *hwpoison_dir;

static int hwpoison_inject(void *data, u64 val)
{
	unsigned long pfn = val;
	struct page *p;
	struct page *hpage;
	int err;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

<<<<<<< HEAD
	if (!hwpoison_filter_enable)
		goto inject;
=======
>>>>>>> refs/remotes/origin/master
	if (!pfn_valid(pfn))
		return -ENXIO;

	p = pfn_to_page(pfn);
	hpage = compound_head(p);
	/*
	 * This implies unable to support free buddy pages.
	 */
	if (!get_page_unless_zero(hpage))
		return 0;

<<<<<<< HEAD
=======
	if (!hwpoison_filter_enable)
		goto inject;

>>>>>>> refs/remotes/origin/master
	if (!PageLRU(p) && !PageHuge(p))
		shake_page(p, 0);
	/*
	 * This implies unable to support non-LRU pages.
	 */
	if (!PageLRU(p) && !PageHuge(p))
		return 0;

	/*
	 * do a racy check with elevated page count, to make sure PG_hwpoison
	 * will only be set for the targeted owner (or on a free page).
	 * We temporarily take page lock for try_get_mem_cgroup_from_page().
<<<<<<< HEAD
<<<<<<< HEAD
	 * __memory_failure() will redo the check reliably inside page lock.
=======
	 * memory_failure() will redo the check reliably inside page lock.
>>>>>>> refs/remotes/origin/cm-10.0
=======
	 * memory_failure() will redo the check reliably inside page lock.
>>>>>>> refs/remotes/origin/master
	 */
	lock_page(hpage);
	err = hwpoison_filter(hpage);
	unlock_page(hpage);
	if (err)
		return 0;

inject:
<<<<<<< HEAD
	printk(KERN_INFO "Injecting memory failure at pfn %lx\n", pfn);
<<<<<<< HEAD
	return __memory_failure(pfn, 18, MF_COUNT_INCREASED);
=======
	return memory_failure(pfn, 18, MF_COUNT_INCREASED);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_info("Injecting memory failure at pfn %#lx\n", pfn);
	return memory_failure(pfn, 18, MF_COUNT_INCREASED);
>>>>>>> refs/remotes/origin/master
}

static int hwpoison_unpoison(void *data, u64 val)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	return unpoison_memory(val);
}

DEFINE_SIMPLE_ATTRIBUTE(hwpoison_fops, NULL, hwpoison_inject, "%lli\n");
DEFINE_SIMPLE_ATTRIBUTE(unpoison_fops, NULL, hwpoison_unpoison, "%lli\n");

static void pfn_inject_exit(void)
{
	if (hwpoison_dir)
		debugfs_remove_recursive(hwpoison_dir);
}

static int pfn_inject_init(void)
{
	struct dentry *dentry;

	hwpoison_dir = debugfs_create_dir("hwpoison", NULL);
	if (hwpoison_dir == NULL)
		return -ENOMEM;

	/*
	 * Note that the below poison/unpoison interfaces do not involve
	 * hardware status change, hence do not require hardware support.
	 * They are mainly for testing hwpoison in software level.
	 */
<<<<<<< HEAD
	dentry = debugfs_create_file("corrupt-pfn", 0600, hwpoison_dir,
=======
	dentry = debugfs_create_file("corrupt-pfn", 0200, hwpoison_dir,
>>>>>>> refs/remotes/origin/master
					  NULL, &hwpoison_fops);
	if (!dentry)
		goto fail;

<<<<<<< HEAD
	dentry = debugfs_create_file("unpoison-pfn", 0600, hwpoison_dir,
=======
	dentry = debugfs_create_file("unpoison-pfn", 0200, hwpoison_dir,
>>>>>>> refs/remotes/origin/master
				     NULL, &unpoison_fops);
	if (!dentry)
		goto fail;

	dentry = debugfs_create_u32("corrupt-filter-enable", 0600,
				    hwpoison_dir, &hwpoison_filter_enable);
	if (!dentry)
		goto fail;

	dentry = debugfs_create_u32("corrupt-filter-dev-major", 0600,
				    hwpoison_dir, &hwpoison_filter_dev_major);
	if (!dentry)
		goto fail;

	dentry = debugfs_create_u32("corrupt-filter-dev-minor", 0600,
				    hwpoison_dir, &hwpoison_filter_dev_minor);
	if (!dentry)
		goto fail;

	dentry = debugfs_create_u64("corrupt-filter-flags-mask", 0600,
				    hwpoison_dir, &hwpoison_filter_flags_mask);
	if (!dentry)
		goto fail;

	dentry = debugfs_create_u64("corrupt-filter-flags-value", 0600,
				    hwpoison_dir, &hwpoison_filter_flags_value);
	if (!dentry)
		goto fail;

<<<<<<< HEAD
#ifdef	CONFIG_CGROUP_MEM_RES_CTLR_SWAP
=======
#ifdef CONFIG_MEMCG_SWAP
>>>>>>> refs/remotes/origin/master
	dentry = debugfs_create_u64("corrupt-filter-memcg", 0600,
				    hwpoison_dir, &hwpoison_filter_memcg);
	if (!dentry)
		goto fail;
#endif

	return 0;
fail:
	pfn_inject_exit();
	return -ENOMEM;
}

module_init(pfn_inject_init);
module_exit(pfn_inject_exit);
MODULE_LICENSE("GPL");
