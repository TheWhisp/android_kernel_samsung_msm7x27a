/*
 * Copyright (C) 2008 IBM Corporation
 * Author: Mimi Zohar <zohar@us.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 */

#ifndef _LINUX_IMA_H
#define _LINUX_IMA_H

#include <linux/fs.h>
struct linux_binprm;

#ifdef CONFIG_IMA
extern int ima_bprm_check(struct linux_binprm *bprm);
<<<<<<< HEAD
<<<<<<< HEAD
extern int ima_inode_alloc(struct inode *inode);
extern void ima_inode_free(struct inode *inode);
=======
>>>>>>> refs/remotes/origin/cm-10.0
extern int ima_file_check(struct file *file, int mask);
extern void ima_file_free(struct file *file);
extern int ima_file_mmap(struct file *file, unsigned long prot);
=======
extern int ima_file_check(struct file *file, int mask);
extern void ima_file_free(struct file *file);
extern int ima_file_mmap(struct file *file, unsigned long prot);
extern int ima_module_check(struct file *file);
>>>>>>> refs/remotes/origin/master

#else
static inline int ima_bprm_check(struct linux_binprm *bprm)
{
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int ima_inode_alloc(struct inode *inode)
=======
static inline int ima_file_check(struct file *file, int mask)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

<<<<<<< HEAD
static inline void ima_inode_free(struct inode *inode)
=======
static inline void ima_file_free(struct file *file)
>>>>>>> refs/remotes/origin/master
{
	return;
}

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-10.0
static inline int ima_file_check(struct file *file, int mask)
=======
static inline int ima_file_mmap(struct file *file, unsigned long prot)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

<<<<<<< HEAD
static inline void ima_file_free(struct file *file)
=======
static inline int ima_module_check(struct file *file)
{
	return 0;
}

#endif /* CONFIG_IMA */

#ifdef CONFIG_IMA_APPRAISE
extern void ima_inode_post_setattr(struct dentry *dentry);
extern int ima_inode_setxattr(struct dentry *dentry, const char *xattr_name,
		       const void *xattr_value, size_t xattr_value_len);
extern int ima_inode_removexattr(struct dentry *dentry, const char *xattr_name);
#else
static inline void ima_inode_post_setattr(struct dentry *dentry)
>>>>>>> refs/remotes/origin/master
{
	return;
}

<<<<<<< HEAD
static inline int ima_file_mmap(struct file *file, unsigned long prot)
{
	return 0;
}
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
#endif /* CONFIG_IMA_H */
=======
static inline int ima_inode_setxattr(struct dentry *dentry,
				     const char *xattr_name,
				     const void *xattr_value,
				     size_t xattr_value_len)
{
	return 0;
}

static inline int ima_inode_removexattr(struct dentry *dentry,
					const char *xattr_name)
{
	return 0;
}
#endif /* CONFIG_IMA_APPRAISE */
>>>>>>> refs/remotes/origin/master
#endif /* _LINUX_IMA_H */
