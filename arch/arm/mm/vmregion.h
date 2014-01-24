#ifndef VMREGION_H
#define VMREGION_H

#include <linux/spinlock.h>
#include <linux/list.h>

struct page;

struct arm_vmregion_head {
	spinlock_t		vm_lock;
	struct list_head	vm_list;
	unsigned long		vm_start;
	unsigned long		vm_end;
};

struct arm_vmregion {
	struct list_head	vm_list;
	unsigned long		vm_start;
	unsigned long		vm_end;
<<<<<<< HEAD
<<<<<<< HEAD
	struct page		*vm_pages;
=======
	void			*priv;
>>>>>>> refs/remotes/origin/cm-11.0
	int			vm_active;
};

struct arm_vmregion *arm_vmregion_alloc(struct arm_vmregion_head *, size_t, size_t, gfp_t);
=======
	void			*priv;
	int			vm_active;
	const void		*caller;
};

struct arm_vmregion *arm_vmregion_alloc(struct arm_vmregion_head *, size_t, size_t, gfp_t, const void *);
>>>>>>> refs/remotes/origin/cm-10.0
struct arm_vmregion *arm_vmregion_find(struct arm_vmregion_head *, unsigned long);
struct arm_vmregion *arm_vmregion_find_remove(struct arm_vmregion_head *, unsigned long);
void arm_vmregion_free(struct arm_vmregion_head *, struct arm_vmregion *);

<<<<<<< HEAD
=======
int arm_vmregion_create_proc(const char *, struct arm_vmregion_head *);

>>>>>>> refs/remotes/origin/cm-10.0
#endif
