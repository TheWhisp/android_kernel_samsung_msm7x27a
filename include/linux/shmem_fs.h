#ifndef __SHMEM_FS_H
#define __SHMEM_FS_H

#include <linux/swap.h>
#include <linux/mempolicy.h>
#include <linux/pagemap.h>
#include <linux/percpu_counter.h>
<<<<<<< HEAD

/* inode in-kernel data */

<<<<<<< HEAD
#define SHMEM_NR_DIRECT 16

#define SHMEM_SYMLINK_INLINE_LEN (SHMEM_NR_DIRECT * sizeof(swp_entry_t))

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/xattr.h>

/* inode in-kernel data */

>>>>>>> refs/remotes/origin/master
struct shmem_inode_info {
	spinlock_t		lock;
	unsigned long		flags;
	unsigned long		alloced;	/* data pages alloced to file */
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long		swapped;	/* subtotal assigned to swap */
	unsigned long		next_index;	/* highest alloced index + 1 */
	struct shared_policy	policy;		/* NUMA memory alloc policy */
	struct page		*i_indirect;	/* top indirect blocks page */
	union {
		swp_entry_t	i_direct[SHMEM_NR_DIRECT]; /* first blocks */
		char		inline_symlink[SHMEM_SYMLINK_INLINE_LEN];
	};
=======
=======
>>>>>>> refs/remotes/origin/master
	union {
		unsigned long	swapped;	/* subtotal assigned to swap */
		char		*symlink;	/* unswappable short symlink */
	};
	struct shared_policy	policy;		/* NUMA memory alloc policy */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	struct list_head	swaplist;	/* chain of maybes on swap */
	struct list_head	xattr_list;	/* list of shmem_xattr */
=======
	struct list_head	swaplist;	/* chain of maybes on swap */
	struct simple_xattrs	xattrs;		/* list of xattrs */
>>>>>>> refs/remotes/origin/master
	struct inode		vfs_inode;
};

struct shmem_sb_info {
	unsigned long max_blocks;   /* How many blocks are allowed */
	struct percpu_counter used_blocks;  /* How many are allocated */
	unsigned long max_inodes;   /* How many inodes are allowed */
	unsigned long free_inodes;  /* How many are left for allocation */
	spinlock_t stat_lock;	    /* Serialize shmem_sb_info changes */
<<<<<<< HEAD
	uid_t uid;		    /* Mount uid for root directory */
	gid_t gid;		    /* Mount gid for root directory */
<<<<<<< HEAD
	mode_t mode;		    /* Mount mode for root directory */
=======
	umode_t mode;		    /* Mount mode for root directory */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kuid_t uid;		    /* Mount uid for root directory */
	kgid_t gid;		    /* Mount gid for root directory */
	umode_t mode;		    /* Mount mode for root directory */
>>>>>>> refs/remotes/origin/master
	struct mempolicy *mpol;     /* default memory policy for mappings */
};

static inline struct shmem_inode_info *SHMEM_I(struct inode *inode)
{
	return container_of(inode, struct shmem_inode_info, vfs_inode);
}

/*
 * Functions in mm/shmem.c called directly from elsewhere:
 */
<<<<<<< HEAD
<<<<<<< HEAD
extern int init_tmpfs(void);
=======
extern int shmem_init(void);
>>>>>>> refs/remotes/origin/cm-10.0
extern int shmem_fill_super(struct super_block *sb, void *data, int silent);
extern struct file *shmem_file_setup(const char *name,
					loff_t size, unsigned long flags);
extern int shmem_zero_setup(struct vm_area_struct *);
extern int shmem_lock(struct file *file, int lock, struct user_struct *user);
<<<<<<< HEAD
=======
extern void shmem_unlock_mapping(struct address_space *mapping);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern int shmem_init(void);
extern int shmem_fill_super(struct super_block *sb, void *data, int silent);
extern struct file *shmem_file_setup(const char *name,
					loff_t size, unsigned long flags);
extern struct file *shmem_kernel_file_setup(const char *name, loff_t size,
					    unsigned long flags);
extern int shmem_zero_setup(struct vm_area_struct *);
extern int shmem_lock(struct file *file, int lock, struct user_struct *user);
extern void shmem_unlock_mapping(struct address_space *mapping);
>>>>>>> refs/remotes/origin/master
extern struct page *shmem_read_mapping_page_gfp(struct address_space *mapping,
					pgoff_t index, gfp_t gfp_mask);
extern void shmem_truncate_range(struct inode *inode, loff_t start, loff_t end);
extern int shmem_unuse(swp_entry_t entry, struct page *page);
<<<<<<< HEAD
<<<<<<< HEAD
extern void mem_cgroup_get_shmem_target(struct inode *inode, pgoff_t pgoff,
					struct page **pagep, swp_entry_t *ent);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static inline struct page *shmem_read_mapping_page(
				struct address_space *mapping, pgoff_t index)
{
	return shmem_read_mapping_page_gfp(mapping, index,
					mapping_gfp_mask(mapping));
}

#endif
