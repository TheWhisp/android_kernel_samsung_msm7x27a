/*
<<<<<<< HEAD
 *  linux/arch/s390/mm/mmap.c
 *
=======
>>>>>>> refs/remotes/origin/master
 *  flexible mmap layout support
 *
 * Copyright 2003-2004 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * Started by Ingo Molnar <mingo@elte.hu>
 */

#include <linux/personality.h>
#include <linux/mm.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/mman.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/mman.h>
>>>>>>> refs/remotes/origin/master
#include <linux/module.h>
#include <linux/random.h>
#include <linux/compat.h>
#include <asm/pgalloc.h>

static unsigned long stack_maxrandom_size(void)
{
	if (!(current->flags & PF_RANDOMIZE))
		return 0;
	if (current->personality & ADDR_NO_RANDOMIZE)
		return 0;
	return STACK_RND_MASK << PAGE_SHIFT;
}

/*
 * Top of mmap area (just below the process stack).
 *
 * Leave at least a ~32 MB hole.
 */
#define MIN_GAP (32*1024*1024)
#define MAX_GAP (STACK_TOP/6*5)

static inline int mmap_is_legacy(void)
{
	if (current->personality & ADDR_COMPAT_LAYOUT)
		return 1;
	if (rlimit(RLIMIT_STACK) == RLIM_INFINITY)
		return 1;
	return sysctl_legacy_va_layout;
}

static unsigned long mmap_rnd(void)
{
	if (!(current->flags & PF_RANDOMIZE))
		return 0;
	/* 8MB randomization for mmap_base */
	return (get_random_int() & 0x7ffUL) << PAGE_SHIFT;
}

<<<<<<< HEAD
=======
static unsigned long mmap_base_legacy(void)
{
	return TASK_UNMAPPED_BASE + mmap_rnd();
}

>>>>>>> refs/remotes/origin/master
static inline unsigned long mmap_base(void)
{
	unsigned long gap = rlimit(RLIMIT_STACK);

	if (gap < MIN_GAP)
		gap = MIN_GAP;
	else if (gap > MAX_GAP)
		gap = MAX_GAP;
	gap &= PAGE_MASK;
	return STACK_TOP - stack_maxrandom_size() - mmap_rnd() - gap;
}

#ifndef CONFIG_64BIT

/*
 * This function, called very early during the creation of a new
 * process VM image, sets up which VM layout function to use:
 */
void arch_pick_mmap_layout(struct mm_struct *mm)
{
	/*
	 * Fall back to the standard layout if the personality
	 * bit is set, or if the expected stack growth is unlimited:
	 */
	if (mmap_is_legacy()) {
<<<<<<< HEAD
		mm->mmap_base = TASK_UNMAPPED_BASE;
		mm->get_unmapped_area = arch_get_unmapped_area;
		mm->unmap_area = arch_unmap_area;
	} else {
		mm->mmap_base = mmap_base();
		mm->get_unmapped_area = arch_get_unmapped_area_topdown;
		mm->unmap_area = arch_unmap_area_topdown;
	}
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(arch_pick_mmap_layout);
=======
>>>>>>> refs/remotes/origin/cm-10.0

#else

int s390_mmap_check(unsigned long addr, unsigned long len)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (!is_compat_task() &&
	    len >= TASK_SIZE && TASK_SIZE < (1UL << 53))
		return crst_table_upgrade(current->mm, 1UL << 53);
=======
	int rc;

	if (!is_compat_task() &&
=======
	int rc;

	if (!is_compat_task() &&
>>>>>>> refs/remotes/origin/cm-11.0
	    len >= TASK_SIZE && TASK_SIZE < (1UL << 53)) {
		rc = crst_table_upgrade(current->mm, 1UL << 53);
		if (rc)
			return rc;
		update_mm(current->mm, current);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
		mm->mmap_base = mmap_base_legacy();
		mm->get_unmapped_area = arch_get_unmapped_area;
	} else {
		mm->mmap_base = mmap_base();
		mm->get_unmapped_area = arch_get_unmapped_area_topdown;
	}
}

#else

int s390_mmap_check(unsigned long addr, unsigned long len, unsigned long flags)
{
	if (is_compat_task() || (TASK_SIZE >= (1UL << 53)))
		return 0;
	if (!(flags & MAP_FIXED))
		addr = 0;
	if ((addr + len) >= TASK_SIZE)
		return crst_table_upgrade(current->mm, 1UL << 53);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	return 0;
}

static unsigned long
s390_get_unmapped_area(struct file *filp, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
{
	struct mm_struct *mm = current->mm;
	unsigned long area;
	int rc;

	area = arch_get_unmapped_area(filp, addr, len, pgoff, flags);
	if (!(area & ~PAGE_MASK))
		return area;
	if (area == -ENOMEM && !is_compat_task() && TASK_SIZE < (1UL << 53)) {
		/* Upgrade the page table to 4 levels and retry. */
		rc = crst_table_upgrade(mm, 1UL << 53);
		if (rc)
			return (unsigned long) rc;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
		update_mm(mm, current);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
		update_mm(mm, current);
>>>>>>> refs/remotes/origin/cm-11.0
		area = arch_get_unmapped_area(filp, addr, len, pgoff, flags);
	}
	return area;
}

static unsigned long
s390_get_unmapped_area_topdown(struct file *filp, const unsigned long addr,
			  const unsigned long len, const unsigned long pgoff,
			  const unsigned long flags)
{
	struct mm_struct *mm = current->mm;
	unsigned long area;
	int rc;

	area = arch_get_unmapped_area_topdown(filp, addr, len, pgoff, flags);
	if (!(area & ~PAGE_MASK))
		return area;
	if (area == -ENOMEM && !is_compat_task() && TASK_SIZE < (1UL << 53)) {
		/* Upgrade the page table to 4 levels and retry. */
		rc = crst_table_upgrade(mm, 1UL << 53);
		if (rc)
			return (unsigned long) rc;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
		update_mm(mm, current);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
		update_mm(mm, current);
>>>>>>> refs/remotes/origin/cm-11.0
		area = arch_get_unmapped_area_topdown(filp, addr, len,
						      pgoff, flags);
	}
	return area;
}
/*
 * This function, called very early during the creation of a new
 * process VM image, sets up which VM layout function to use:
 */
void arch_pick_mmap_layout(struct mm_struct *mm)
{
	/*
	 * Fall back to the standard layout if the personality
	 * bit is set, or if the expected stack growth is unlimited:
	 */
	if (mmap_is_legacy()) {
<<<<<<< HEAD
		mm->mmap_base = TASK_UNMAPPED_BASE;
		mm->get_unmapped_area = s390_get_unmapped_area;
		mm->unmap_area = arch_unmap_area;
	} else {
		mm->mmap_base = mmap_base();
		mm->get_unmapped_area = s390_get_unmapped_area_topdown;
		mm->unmap_area = arch_unmap_area_topdown;
	}
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(arch_pick_mmap_layout);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
		mm->mmap_base = mmap_base_legacy();
		mm->get_unmapped_area = s390_get_unmapped_area;
	} else {
		mm->mmap_base = mmap_base();
		mm->get_unmapped_area = s390_get_unmapped_area_topdown;
	}
}
>>>>>>> refs/remotes/origin/master

#endif
